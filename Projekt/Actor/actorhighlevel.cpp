#include "actorhighlevel.h"

#include <QDebug>
#include <QElapsedTimer>
#include <QTimer>
#include <QMutexLocker>
#include <QCoreApplication>
#include <QThread>

#include "AI/pathplanning.h"
#include "Data/mapdata.h"
#include "Data/define.h"
#include "Sensor/sensorLowLevel.h"

#include "Structs/pathplotdata.h"


std::atomic_bool ActorHighLevel::streamPIDEnabled (false);

StatePathProcessing ActorHighLevel::state = StatePathProcessing::RUNNING;
QMutex ActorHighLevel::mutexState;

ActorHighLevel::ActorHighLevel()
{
    this->internalWP = QList< QPair<double,double> >();

    positionWasReached = false;

    elapsedTime = new QElapsedTimer();
    timerPIDPlot = new QTimer;

    mutexPidHist = new QMutex;

    quitting = false;
    enabled = false;

    // PID Plot in GUI
    connect(timerPIDPlot, SIGNAL(timeout()), this, SLOT(slotTimerSendPIDPlot()));
    timerPIDPlot->setInterval(1000.0/20.0); // 20Hz plotting
    timerPIDPlot->start();

    // Default PID Parameters
    PID_A_P = .2; // The actual values will be sent from the GUI on startup
    PID_A_I = 0;
    PID_A_D = -5;
    PID_V_P = .2;
    PID_V_I = 0;
    PID_V_D = -5;

    resetPIDtempVars();

    additionalReleasePuckDistance = 0.0;

    timeOfStart = QDateTime::currentMSecsSinceEpoch() / 1000.0;
}

ActorHighLevel::~ActorHighLevel()
{
    internalWP.clear();
    delete elapsedTime;
    delete timerPIDPlot;
    delete mutexPidHist;
}

void ActorHighLevel::ignoreSignals()
{
    quitting = true;
    enabled = false;
}

QVector<double> ActorHighLevel::lowPass(QVector<double> in, double alpha)
{
    int l = in.length();
    if (l == 0)
        return QVector< double >();

    // Gewichtung berechnen, falls auf default (0)
    if(alpha == 0){
        double rate     = 1.0 / config::actorPeriodMotionControl;
        double slowRate = 1.0 / (config::actorPeriodMotionControl+1);
        alpha = rate / (rate + slowRate);
    }

    // Vorwärts filtern
    QVector< double > forward(l);
    forward.first() = in.first();
    for(int i=1; i < l; ++i){
        forward[i]  = alpha * in[i]  + (1-alpha) * in[i-1];
    }

    // Rückwärts filtern
    QVector< double > backward(l);
    backward.last() = in.last();
    for(int i=l-2; i >= 0; --i){
        backward[i]  = alpha * in[i]  + (1-alpha) * in[i+1];
    }

    // Überlagern
    QVector< double > out(l);
    for(int i=0; i < l; ++i){
        out[i]  = (forward[i]  + backward[i] ) / 2;
    }

    return out;
}

const double ActorHighLevel::constrainAngle(const double inRad){
    double outRad = inRad;
    if(std::isinf(outRad) || std::isnan(outRad))
        return 0;
    while (outRad >  M_PI)
        outRad -= 2*M_PI;
    while (outRad < -M_PI)
        outRad += 2*M_PI;
    return outRad;
}

QVector<double> ActorHighLevel::takeDimension(const QVector<QPair<double, double> > in, const int dimension){
    int l = in.length();
    if(l==0) return QVector<double>();
    QVector<double> out(l);
    if(dimension==0){
        for(int i=0; i<l; ++i){
            out[i] = in[i].first;
        }
    } else {
        for(int i=0; i<l; ++i){
            out[i] = in[i].second;
        }
    }
    return out;
}

void ActorHighLevel::slotUpdateWaypoints(QList< QPair<double,double> > waypoints)
{
    // signals will be ignored, if quitting is true
    if (!quitting)
    {
        //wenn ein anderer Status activ ist, soll dieser nicht gestört werden
        switch(this->getState()) {
        case StatePathProcessing::STOP:
        case StatePathProcessing::RUNNING:
        {
            internalWP = waypoints;
            numWP = internalWP.length();
            // Anhalten, falls es nicht genug Wegpunkte gibt
            if(numWP == 0) {
                this->setState(StatePathProcessing::STOP);
                enabled = true;
                this->startPIDController();
                return;
            }
            else {
                this->setState(StatePathProcessing::RUNNING);
            }
        }
            break;
        default: // jeder andere Status soll beendet werden
        {
            // nichts machen!
            return;
        } // default
        } // switch

        // Ein Spline mit zwei Stützpunkten kann nicht generiert werden, schummele einen mittleren dazu.
        if(numWP == 1){
            double midX = (internalWP.first().first  + internalWP.last().first )/2;
            double midY = (internalWP.first().second + internalWP.last().second)/2;
            internalWP.insert(1, QPair<double,double> (midX, midY));
            ++numWP;
        }

        // Wegpunkte Tiefpassfiltern
        QVector<double> wpLowPassX = lowPass(takeDimension(internalWP.toVector(), 0), config::actorWPLowPassAlpha);
        QVector<double> wpLowPassY = lowPass(takeDimension(internalWP.toVector(), 1), config::actorWPLowPassAlpha);

        // Spline Metrik erzeugen
        QVector<double> splineMetric(numWP);
        for(int i=0; i<numWP; ++i){
            splineMetric[i] = static_cast<double>(i);
        }

        // Spline generieren
        splineX.set_points(splineMetric, wpLowPassX);
        splineY.set_points(splineMetric, wpLowPassY);

        // Send generated spline to path display tab
        if(PathPlanning::streamPathEnabled) {
            static const int numSplinePoints = 100;
            PathPlotData dataPacket;
            dataPacket.dataType = PathPlotData::SPLINE;
            dataPacket.splineLength = numWP;
            QVector<double> splineVectX(numSplinePoints), splineVectY(numSplinePoints);
            for(int i=0; i<numSplinePoints; i++){
                double progress = static_cast<double>(i*dataPacket.splineLength)/static_cast<double>(numSplinePoints-1);
                splineVectX[i] = splineX(progress);
                splineVectY[i] = splineY(progress);
            }
            dataPacket.splineX = splineVectX;
            dataPacket.splineY = splineVectY;
            Q_EMIT signalSplinePlot(dataPacket);
        }

        // Start timer (verwendet für berechnung der I und D Anteile)
        elapsedTime->restart();

        // PID wieder starten
        enabled = true;
        this->startPIDController();
    }
}

void ActorHighLevel::slotReleasePuck()
{
    // signals will be ignored, if quitting is true
    if (!quitting)
    {
        if(config::enableDebugActorHighLevel)
            qDebug() << "Starte Puck Release";

        // Roboterkoordinaten holen
        releasePuckOrigin = MapData::getRobotPosition();

        resetPIDtempVars(); // PID zurücksetzen

        this->setState(StatePathProcessing::RELEASE_PUCK);
        enabled = true;
        this->startPIDController();
        /// \todo piepsen?
    }
}

void ActorHighLevel::slotPushAndReleasePuck(double m_releasePuckDistance)
{
    // signals will be ignored, if quitting is true
    if (!quitting)
    {
        if(config::enableDebugActorHighLevel)
            qDebug() << "Starte Puck Push";

        additionalReleasePuckDistance = m_releasePuckDistance;

        // Roboterkoordinaten holen
        releasePuckOrigin = MapData::getRobotPosition();

        resetPIDtempVars(); // PID zurücksetzen

        this->setState(StatePathProcessing::PUSH_AND_RELEASE_PUCK);
        enabled = true;
        this->startPIDController();
    }
}

void ActorHighLevel::slotGatherPuck()
{
    // signals will be ignored, if quitting is true
    if (!quitting)
    {
        if(config::enableDebugActorHighLevel)
            qDebug() << "Starte Gather Puck";

        // Roboterkoordinaten holen
        releasePuckOrigin = MapData::getRobotPosition();

        resetPIDtempVars(); // PID zurücksetzen

        this->setState(StatePathProcessing::GATHER_PUCK);
        enabled = true;
        this->startPIDController();
    }
}


// Is not called periodically from a timer slot
void ActorHighLevel::startPIDController()
{
    while(enabled)
    {
        // Roboterposition abspeichern
        robotPosition = MapData::getRobotPosition(ObstacleType::ME);

        // we dont have a position right now -> try again in 50ms
        if(robotPosition.certainty() == 0.0) {
            QThread::msleep(50);
            // Process Qt Event Queue and wait a little
            QCoreApplication::processEvents();
            QCoreApplication::sendPostedEvents();

            continue;
        }

        // Zeit seit letzter Berechnung bestimmen, um korrekt zu integrieren / differenzieren
        double elapsedMs = elapsedTime->restart();
        double timeSinceStart = (QDateTime::currentMSecsSinceEpoch() / 1000.0) - timeOfStart;

        switch (this->getState()) {
        case StatePathProcessing::GATHER_PUCK:
        {
            double deltaX = releasePuckOrigin.x()- robotPosition.x();
            double deltaY = releasePuckOrigin.y()- robotPosition.y();
            double dist = sqrt(deltaX*deltaX+deltaY*deltaY); // Bereits gefahrene Distanz
            double deltaL = config::actorGatherPuckDistance - dist; // Distanz zur Sollposition

            // Sind wir weit genug gefahren?
            if(dist >= config::actorGatherPuckDistance) {
                if(config::enableDebugActorHighLevel)
                    qDebug() << "Puck gathered, weil dist == " << dist << " >= config::actorGatherPuckDistance == " << config::actorGatherPuckDistance;
                resetPIDtempVars();
                this->setState(StatePathProcessing::RUNNING);
                Q_EMIT signalSendRobotControlParams(0, 0);
                Q_EMIT signalPuckDone(); // Signal an KI, dass Gathering fertig ist
                break;
            }

            // PID Regler Abstand
            double dDeltaL = (elapsedMs) ? (deltaL-lastDeltaL)/elapsedMs : 0;
            lastDeltaL = deltaL;
            iDeltaL += deltaL * elapsedMs;
            iDeltaL = qMin(qMax(iDeltaL, -config::actorMaxI), config::actorMaxI);
            // Sollgeschwindigkeit
            double robotV = +1.0 * (PID_V_P * deltaL + PID_V_I * iDeltaL + PID_V_D * dDeltaL);

            // Daten zu history hinzufuegen fuer spaeteren plot
            if(ActorHighLevel::streamPIDEnabled) {
                mutexPidHist->lock();
                pidHistTime.append(timeSinceStart);
                pidHistWinkelIst.append(constrainAngle(robotPosition.rot()));
                pidHistWinkelSoll.append(0.0);
                pidHistDistIst.append(dist);
                pidHistDistSoll.append(deltaL);
                mutexPidHist->unlock();
            }

            // Emit the calculated control values to the low level actor module
            Q_EMIT signalSendRobotControlParams(robotV, 0.0);

            break;

        }
        case StatePathProcessing::PUSH_AND_RELEASE_PUCK:
        {
            double deltaX = releasePuckOrigin.x() - robotPosition.x();
            double deltaY = releasePuckOrigin.y() - robotPosition.y();
            double dist = sqrt(deltaX*deltaX+deltaY*deltaY); // Bereits gefahrene Distanz

            // 25cm (NEIN 20cm) minus das bereits gefahrene
            ///\laurenz hier sollte doch 20cm unten und oben stehen?
            double deltaL = /*config::actorReleasePuckDistance*/
                            config::actorPushPuckDistance
                            - dist; // Distanz zur Sollposition

            // hier wird gefragt ob wir mehr als 20cm gefahren sind

            // Sind wir weit genug gefahren?
            if(dist >= config::actorPushPuckDistance) {
                if(config::enableDebugActorHighLevel)
                    qDebug() << "Puck pushed, weil dist =" << dist;

                resetPIDtempVars();
                // nehme wieder die tatsächlich aktuelle position
                releasePuckOrigin = MapData::getRobotPosition(ObstacleType::ME);
                this->setState(StatePathProcessing::RELEASE_PUCK_FROM_PUSH);

                if(config::enableDebugActorHighLevel)
                    qDebug() << "Starting Puck release";

                Q_EMIT signalSendRobotControlParams(0, 0);
                break; // springe raus
            }

            // PID Regler Abstand
            double dDeltaL = (elapsedMs) ? (deltaL-lastDeltaL)/elapsedMs : 0;
            lastDeltaL = deltaL;
            iDeltaL += deltaL * elapsedMs;
            iDeltaL = qMin(qMax(iDeltaL, -config::actorMaxI), config::actorMaxI);
            double robotV = +1.0 * (PID_V_P * deltaL + PID_V_I * iDeltaL + PID_V_D * dDeltaL); // Sollgeschwindigkeit

            // Daten zu history hinzufuegen fuer spaeteren plot
            if(ActorHighLevel::streamPIDEnabled) {
                mutexPidHist->lock();
                pidHistTime.append(timeSinceStart);
                pidHistWinkelIst.append(constrainAngle(robotPosition.rot()));
                pidHistWinkelSoll.append(0.0);
                pidHistDistIst.append(dist);
                pidHistDistSoll.append(deltaL);
                mutexPidHist->unlock();
            }

            // Emit the calculated control values to the low level actor module
            Q_EMIT signalSendRobotControlParams(robotV, 0.0);

            break;
        }
        case StatePathProcessing::RELEASE_PUCK_FROM_PUSH:
        {
            double deltaX = releasePuckOrigin.x() - robotPosition.x();
            double deltaY = releasePuckOrigin.y() - robotPosition.y();
            double dist = sqrt(deltaX*deltaX+deltaY*deltaY); // Bereits gefahrene Distanz

            // (additional distance is 20 or 0) + 25cm + 2,5 cm
            // Carlo: habe die unterscheidung eingebaut um den puck correct ins tor zu legen
            // und den beim DUMP trotzdem weit genug zurück zu fahren
            double distanceToGoBack = additionalReleasePuckDistance +
                                      config::actorReleasePuckDistance +
                                      config::actorPushAndReleaseAdditionalReverseDist;

            // 25cm + 20cm + 2,5 cm
            double deltaL = distanceToGoBack - dist; // Distanz zur Sollposition

            // Sind wir weit genug gefahren?
            if(dist >= distanceToGoBack)
            {
                if(config::enableDebugActorHighLevel)
                    qDebug() << "Puck released, weil dist =" << dist;
                resetPIDtempVars();
                this->setState(StatePathProcessing::RUNNING);
                Q_EMIT signalSendRobotControlParams(0, 0);
                Q_EMIT signalPuckDone(); // Signal an KI, dass Release fertig ist
                break;
            }

            // PID Regler Abstand
            double dDeltaL = (elapsedMs) ? (deltaL-lastDeltaL)/elapsedMs : 0;
            lastDeltaL = deltaL;
            iDeltaL += deltaL * elapsedMs;
            iDeltaL = qMin(qMax(iDeltaL, -config::actorMaxI), config::actorMaxI);

            // Sollgeschwindigkeit
            double robotV = -1.0 * (PID_V_P * deltaL + PID_V_I * iDeltaL + PID_V_D * dDeltaL);

            // Daten zu history hinzufuegen fuer spaeteren plot
            if(ActorHighLevel::streamPIDEnabled) {
                mutexPidHist->lock();
                pidHistTime.append(timeSinceStart);
                pidHistWinkelIst.append(constrainAngle(robotPosition.rot()));
                pidHistWinkelSoll.append(0.0);
                pidHistDistIst.append(dist);
                pidHistDistSoll.append(deltaL);
                mutexPidHist->unlock();
            }

            // Emit the calculated control values to the low level actor module
            Q_EMIT signalSendRobotControlParams(robotV, 0.0);

            break;
        }
        case StatePathProcessing::RELEASE_PUCK:
        {
            double deltaX = releasePuckOrigin.x() - robotPosition.x();
            double deltaY = releasePuckOrigin.y() - robotPosition.y();
            double dist = sqrt(deltaX*deltaX+deltaY*deltaY); // Bereits gefahrene Distanz
            double deltaL = config::actorReleasePuckDistance - dist; // Distanz zur Sollposition

            // Sind wir weit genug gefahren?
            if(dist >= config::actorReleasePuckDistance) {
                if(config::enableDebugActorHighLevel)
                    qDebug() << "Puck released, weil dist =" << dist;

                resetPIDtempVars();
                this->setState(StatePathProcessing::RUNNING);
                Q_EMIT signalSendRobotControlParams(0, 0);
                Q_EMIT signalPuckDone(); // Signal an KI, dass Release fertig ist
                break;
            }

            // PID Regler Abstand
            double dDeltaL = (elapsedMs) ? (deltaL-lastDeltaL)/elapsedMs : 0;
            lastDeltaL = deltaL;
            iDeltaL += deltaL * elapsedMs;
            iDeltaL = qMin(qMax(iDeltaL, -config::actorMaxI), config::actorMaxI);

            // Sollgeschwindigkeit
            double robotV = -1.0 * (PID_V_P * deltaL + PID_V_I * iDeltaL + PID_V_D * dDeltaL);

            // Daten zu history hinzufuegen fuer spaeteren plot
            if(ActorHighLevel::streamPIDEnabled) {
                mutexPidHist->lock();
                pidHistTime.append(timeSinceStart);
                pidHistWinkelIst.append(constrainAngle(robotPosition.rot()));
                pidHistWinkelSoll.append(0.0);
                pidHistDistIst.append(dist);
                pidHistDistSoll.append(deltaL);
                mutexPidHist->unlock();
            }

            // Emit the calculated control values to the low level actor module
            Q_EMIT signalSendRobotControlParams(robotV, 0.0);

            break;
        }
        case StatePathProcessing::RUNNING:
        {
            // Wenn es keine Wegpunkte gibt
            if(internalWP.isEmpty() || MapData::getObstacle(ObstacleType::TARGET).isEmpty()) {
                // Leave the state
                this->setState(StatePathProcessing::STOP);
                break;
            }

            // Turn off primitive collision avoidance
            MapData::setDisableEmergency(true);

            // Wenn der Roboter auf dem letzten Wegpunkt steht
            Obstacle target = MapData::getFirstTarget();

            bool targetHasOrientation = !std::isnan(target.getOrientation()) && !std::isinf(target.getOrientation());
            double targetDist = robotPosition.getDistanceTo(target.getPosition());
            double targetDistDiff = targetDist - targetDistLast;
            targetDistLast = targetDist;
            if(targetDistDiff == 0) targetDistDiff = targetDistDiffLast;
            targetDistDiffLast = targetDistDiff;
            bool targetSeemsReached = targetDistDiff > config::actorWaypointReachedDiffChange && targetDist <= config::actorWaypointReachedDistance;
            if( positionWasReached || targetSeemsReached ) {
                // Roboter ist/war an Position

                if(positionWasReached == false){
                    qDebug() << "Ziel erreicht mit Abstand" << targetDist << " - Stelle jetzt Winkel perfekt ein";
                }

                positionWasReached = true; // Roboter hat das Ziel erreicht. Wenn das Ziel davonwackelt, ist es immernoch erreicht.

                // Abweichung vom einzustellenden Winkel (kann undefinierte Werte annehmen, falls das Target keinen Winkel besitzt)
                double angleDeviation = fabs(target.getOrientation() - robotPosition.rot());
                if(std::isnan(angleDeviation)) angleDeviation = 0;
                while(angleDeviation > M_PI) angleDeviation -= M_PI; // Die Winkelabweichung darf nie über 180° sein

                if(!targetHasOrientation || (targetHasOrientation && angleDeviation <= config::actorWaypointMaxAngleDeviation ) ) {
                    // Winkel ist OK
                    if(config::enableDebugActorHighLevel)
                        qDebug() << "Ziel erreicht mit Winkelabweichung" << (angleDeviation/M_PI*180.0) << "°";

                    positionWasReached = false; // Zurücksetzen, weil als nächstes ein neues Ziel kommt
                    MapData::deleteFirstTarget(); // Delete this target
                    this->setState(StatePathProcessing::STOP); // Leave the state
                    Q_EMIT signalSendRobotControlParams(0.0, 0.0); // Zur Sicherheit Stop senden
                    break;
                } else {
                    if(config::enableDebugActorHighLevel)
                        qDebug() << "Winkel noch nicht OK - Abweichung " << (angleDeviation/M_PI*180.0) << "°";
                }
            }

            // Nähesten Punkt auf dem Spline finden.
            double minP=0, minL=9999;
            for(double p = 0; p<numWP; p+=0.001){ // Sicher wär ne Newton Iteration oder so besser, aber so gehts auch.
                double deltaX = splineX(p) - robotPosition.x();
                double deltaY = splineY(p) - robotPosition.y();
                double deltaL = sqrt(deltaX*deltaX+deltaY*deltaY); // Distanz des Roboters zu diesem Punkt auf dem Spline
                if(deltaL < minL){
                    minP = p;
                    minL = deltaL;
                }
            }

            // Die Sollposition ist vom nähesten Spline Punkt 0.5m weiter auf dem Spline entfernt, jedoch nie weiter weg als das Ziel selbst
            double progressOnSpline = minP + config::actorDistanceOfTargetOnSpline;
            double sollX, sollY;// Sollposition
            if(progressOnSpline <= numWP) {
                sollX = splineX(progressOnSpline);
                sollY = splineY(progressOnSpline);
            } else {
                sollX = target.getCoords().first;
                sollY = target.getCoords().second;
            }
            double deltaX = sollX - robotPosition.x(); // Abweichung von der Sollposition
            double deltaY = sollY - robotPosition.y(); // Abweichung von der Sollposition
            double deltaL = sqrt(deltaX*deltaX+deltaY*deltaY); // Distanz zur Sollposition

            double sollA;
            if(targetHasOrientation && deltaL < config::actorWaypointReachedDistance) { // Roboter ist im Prinzip schon da und muss sich noch vernünftig ausrichten
                sollA = /*constrainAngle*/(target.getOrientation()); // Vorgegebener Anfahrtswinkel
            } else { // Roboter muss beim Fahren in Fahrtrichtung gucken
                sollA = /*constrainAngle*/(atan2(deltaY, deltaX)); // Winkel zur Sollposition
            }
            double deltaA = constrainAngle(sollA) - constrainAngle(robotPosition.rot()); // Abweichung vom Winkel zur Sollposition
             deltaA = constrainAngle(deltaA);

            // PID Regler Rotation(sgeschwindigkeit)
//            deltaA = SensorLowLevel::angleWeightedAverage(deltaA, config::actorLowPass, lastDeltaA, 1-config::actorLowPass); // Tiefpassfilter
            double dDeltaA = (elapsedMs)? (deltaA-lastDeltaA)/elapsedMs : 0; // Differenzialanteil
            lastDeltaA = deltaA;
            iDeltaA += deltaA * elapsedMs; // Integralanteil
            iDeltaA = qMin(qMax(iDeltaA, -config::actorMaxI), config::actorMaxI); // Begrenzen auf -10...+10
            double robotR = PID_A_P * deltaA + PID_A_I * iDeltaA + PID_A_D * dDeltaA; // Solldrehgeschwindigkeit

            if(config::enableDebugActorHighLevel)
                qDebug() << "deltaA="<<deltaA << ", iDeltaA=" << iDeltaA << ", dDeltaA=" << dDeltaA << "robotR=" << robotR;

            // PID Regler Abstand
            double dDeltaL = (elapsedMs) ? (deltaL-lastDeltaL)/elapsedMs : 0;
            lastDeltaL = deltaL;
            iDeltaL += deltaL * elapsedMs;
            iDeltaL = qMin(qMax(iDeltaL, -config::actorMaxI), config::actorMaxI);
            double robotV = PID_V_P * deltaL + PID_V_I * iDeltaL + PID_V_D * dDeltaL; // Sollgeschwindigkeit

            // Vorwärtsgeschwindigkeit reduzieren, wenn Winkelabweichung zu groß
            double angleLimiter = 1;
            if(fabs(deltaA) > config::actorMinAngleLimiter && fabs(deltaA) < config::actorMaxAngleLimiter) {
                angleLimiter = 1 - (fabs(deltaA) - config::actorMinAngleLimiter) / (config::actorMaxAngleLimiter - config::actorMinAngleLimiter); // 0...1
            } else if ( fabs(deltaA) > config::actorMaxAngleLimiter ) {
                angleLimiter = 0;
            }
            robotV = robotV * angleLimiter;

            // Wenn es nur noch um die Drehung geht, gar nicht vorwärts fahren
            if(positionWasReached){
                robotV = 0.0;
            }

            // if enemy position is next to target -> wait
            Position enemyPosition = MapData::getRobotPosition(ObstacleType::OPPONENT);

            if(enemyPosition.isConsimilarTo(target.getPosition(), 2.5 * config::SENSOR_RADIUS_ROBOT))
            {
                robotV = 0;
                robotR = 0;

                resetPIDtempVars();
            }

            // Emit the calculated control values to the low level actor module
            Q_EMIT signalSendRobotControlParams(robotV,robotR);

            // Daten zu history hinzufuegen fuer spaeteren plot
            if(ActorHighLevel::streamPIDEnabled) {
                mutexPidHist->lock();
                pidHistTime.append(timeSinceStart);
                pidHistWinkelIst.append(constrainAngle(robotPosition.rot()));
                pidHistWinkelSoll.append(sollA);
                pidHistDistIst.append(robotV);
                pidHistDistSoll.append(deltaL);
                mutexPidHist->unlock();
            }
            break;
        }
        case StatePathProcessing::STOP:
        {
            // Clear remaining waypoints
            internalWP.clear();

            // Stop robot motion
            Q_EMIT signalSendRobotControlParams(0.0, 0.0);

            // (Re-)Enable primitive collision avoidance in manual control mode
            MapData::setDisableEmergency(false);

            // avoid looping in here again all the time
            enabled = false;

            break;
        }
        } // switch

        // Process Qt Event Queue and wait a little
        QCoreApplication::processEvents();
        QCoreApplication::sendPostedEvents();

        // we dont want 100% cpu time for the PID
        QThread::msleep(10);
    }
}

void ActorHighLevel::slotChangePIDParams(PIDParams p)
{
    PID_A_P = p.PID_A_P;
    PID_A_I = p.PID_A_I;
    PID_A_D = p.PID_A_D;
    PID_V_P = p.PID_V_P;
    PID_V_I = p.PID_V_I;
    PID_V_D = p.PID_V_D;
}

void ActorHighLevel::slotTimerSendPIDPlot()
{
    if(ActorHighLevel::streamPIDEnabled){
        PIDPlotData d;

        mutexPidHist->lock();

        d.time = pidHistTime;
        d.winkelIst = pidHistWinkelIst;
        d.winkelSoll = pidHistWinkelSoll;
        d.distanzIst = pidHistDistIst;
        d.distanzSoll = pidHistDistSoll;

        pidHistTime.clear();
        pidHistWinkelIst.clear();
        pidHistWinkelSoll.clear();
        pidHistDistIst.clear();
        pidHistDistSoll.clear();

        mutexPidHist->unlock();

        Q_EMIT signalPIDPlot(d);
    }
}

void ActorHighLevel::resetPIDtempVars()
{
    targetDistLast = 10; // hoher wert
    targetDistDiffLast = 0;
    iDeltaL = 0;
    lastDeltaL = 0;
    lastDeltaA = 0;
    iDeltaA = 0;
}

StatePathProcessing ActorHighLevel::getState()
{
    QMutexLocker lockState(&mutexState);
    return state;
}

void ActorHighLevel::setState(const StatePathProcessing &newState)
{
    QMutexLocker lockState(&mutexState);
    state = newState;
}
