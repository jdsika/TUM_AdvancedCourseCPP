#include "sensorhighlevel.h"

#include <QtMath>
#include <QDebug>
#include <QtAlgorithms>
#include <QMutexLocker>
#include <QMutex>
#include <QElapsedTimer>
#include <QThread>

#include "Data/define.h"
#include "Data/mapdata.h"
#include "Sensor/orientierung.h"
#include "Sensor/medianfilter.h"
#include "Sensor/medianfilter_new.h"
#include "Sensor/cam.h"
#include "AI/pathplanning.h"


std::atomic_bool SensorHighLevel::streamSensorEnabled(false);

SensorHighLevel::SensorHighLevel()
{
    mutexFilterParameter = new QMutex;
    mutexState = new QMutex;
    mutexTeamColor = new QMutex;

    timerWaitForValidColorFrame = new QElapsedTimer;
    timerToAbondonColorDetection = new QElapsedTimer;

    transmissionPosition = Position();
    previousPosition = Position();
    currentPosition = Position();
    dummyPosition = Position();

    constrainedData = ConstrainedLaserData();
    previousObjects = QList<Position>();
    currentObjects = QList<Position>();

    hadEmergency = false;
    targetsSet = false;
    isInSlowTurn = false;
    prepositionInitialized = false;

    counter = 0; 
    this->setState(SensorStates::WAIT);
    this->setTeamColor(CamColor::NONE);

    // all pole positions
    cPolePositions = QVector<Position>() << Position(0.0, 0.0)
                                         << Position(0.0, 5.0/12.0)
                                         << Position(0.0, 5.0/3.0)
                                         << Position(0.0, 75.0/30.0)
                                         << Position(0.0, 10.0/3.0)
                                         << Position(0.0, 55.0/12.0)
                                         << Position(0.0, 5.0)
                                         << Position(3.0, 0.0)
                                         << Position(3.0, 5.0/12.0)
                                         << Position(3.0, 5.0/3.0)
                                         << Position(3.0, 75.0/30.0)
                                         << Position(3.0, 10.0/3.0)
                                         << Position(3.0, 55.0/12.0)
                                         << Position(3.0, 5.0);
}

SensorHighLevel::~SensorHighLevel()
{
    delete mutexFilterParameter;
    delete mutexState;
    delete mutexTeamColor;

    delete timerWaitForValidColorFrame;
    delete timerToAbondonColorDetection;

    cPolePositions.clear();

    if (config::enableDebugSensorHighLevel)
        qDebug() << "DELETE 'SensorHighLvl'";
}

void SensorHighLevel::slotGetLaserData(QVector<double> rawData, Position positionSignal)
{
    // primitive Kollisionsvermeidung fuer handsteuerung
    if(!PathPlanning::getEnabled())
        avoideCollision(rawData);

    transmissionPosition = positionSignal;

    QVector<double> filteredData;

    // wenn der kernel auf 1 gesetzt wird dann wird der filter auch in der realität ausgeschaltet
    mutexFilterParameter->lock();
    int kernel = filterParameter.kernel;
    mutexFilterParameter->unlock();

    // Rohdaten Median (nicht Mittelwert) filtern ueber den Winkel (nicht die Zeit)
    filteredData = QVector<double>::fromStdVector(MedianFilter::filter(rawData.toStdVector(), kernel));

    // Gefilterte Daten anzeigen
    if(SensorHighLevel::streamSensorEnabled)
        Q_EMIT signalSendLaserData(LaserPlotData(filteredData, LaserPlotData::MEDIAN));

    // Datenpunkte ausserhalb der Arena oder einer Recihweite entfernen
    constrainData(filteredData, rawData, constrainedData);

    // Keys nach grad umwandeln und in Graph anzeigen
    if(SensorHighLevel::streamSensorEnabled) {
        QVector<double> keysDegrees(constrainedData.angles().length());
        for(int i=0; i< constrainedData.angles().length(); ++i){
            keysDegrees[i] = (constrainedData.angles().at(i) - M_PI_2) * 180.0 / M_PI;
        }

        Q_EMIT signalSendLaserData(LaserPlotData(keysDegrees, constrainedData.filteredDepths().toVector(), LaserPlotData::REDUCED));
    }

    // Extrahiert Objekte und speichert lokale Koordinaten und Breite des Objektes
    currentObjects = extractObjects(constrainedData);

    // Objekte in GUI Laser Display anzeigen
    if(SensorHighLevel::streamSensorEnabled) {
        LaserPlotData laserData;
        laserData.dataType = LaserPlotData::OBJECTS;
        laserData.data = QVector<double> (currentObjects.size());
        laserData.angles = QVector<double> (currentObjects.size());
        laserData.sizes = QVector<double> (currentObjects.size());
        for(int i = 0; i < currentObjects.size(); ++i) {
            laserData.data[i] = currentObjects.at(i).x(); // tiefe
            laserData.angles[i] = (currentObjects.at(i).y() - M_PI_2) * 180.0 / M_PI; // winkel
            laserData.sizes[i] = currentObjects.at(i).rot();    // breite
        }
        Q_EMIT signalSendLaserData(laserData);
    }

    // internal state machine
    switch (this->getState()) {
    case SensorStates::WAIT:
    {
        if (config::enableDebugSensorHighLevel)
            qDebug() << "Waiting for AI to start";
    }
        break;
    case SensorStates::ORIENTATION:
    {
        Position orientData = Orientation::beginOrientation(currentObjects);

        ///\todo magic number for certainty here
        if (orientData.certainty() > 0.8)
        {
            Q_EMIT signalSendRobotControlParams(0,0);

            ///\todo consider removing the counter and replace through = new QElapsedTimer;
            if(counter > config::SENSOR_WAIT_COUNTER)
            {
                previousPosition = orientData;

                currentPosition = MapData::getRobotPosition(ObstacleType::ME);

                dummyPosition = orientData;

                //if ( currentPosition.r() > )
                dummyAngleOffset = dummyPosition.rot() - currentPosition.rot();
                qDebug() << "Dummy Offset: " << dummyAngleOffset;

                // Setze Dummy bei gerade eben erkannter Position
                MapData::setObstacle(Obstacle(dummyPosition, ObstacleType::DUMMY));

                ///\todo angle range?
                minAngle = fmod(currentPosition.rot() + M_PI - config::SENSOR_DELTA_ANGLE / 2.0, M_PI*2);
                maxAngle = fmod(currentPosition.rot() + M_PI + config::SENSOR_DELTA_ANGLE, M_PI*2);

                qDebug() << "SensorState changed: "  << "OIRENTATION_VALIDATION";
                counter = 0; // Muss bei jedem Statewechsel zurückgesetzt werden, weil der Roboter beim Statewechsel noch 5 mal Laserdaten im Stehen messen soll
                this->setState(SensorStates::ORIENTATION_VALIDATION);
            }
            else
            {
                counter ++;
                if (config::enableDebugSensorHighLevel)
                    qDebug()<<"WAIT FIRST";
            }
        }
        else
        {
            //qDebug() << "Nichts erkannt";
            // Drehen, langsam, bis Abstände erkannt werden
            Q_EMIT signalSendRobotControlParams(0,1.5*config::GUIREMOTETURNRATE);
        }
    }
        break;
    case SensorStates::ORIENTATION_VALIDATION:
    {
        currentPosition = MapData::getRobotPosition(ObstacleType::ME);

        dummyPosition.rot(currentPosition.rot() + dummyAngleOffset);
        //qDebug() << " new dummy Orientation: " << dummyPosition.r();

        MapData::setObstacle(Obstacle(dummyPosition, ObstacleType::DUMMY));

        if (config::enableDebugSensorHighLevel)
            qDebug()<<"ANGLE DISTANCE::: "<< minAngle <<" - "<< maxAngle << "  -  " << currentPosition.rot();

        // condition is working!
        if(( minAngle < currentPosition.rot() &&
             maxAngle > currentPosition.rot() )
            ||
           ((maxAngle<minAngle) &&
            (minAngle > currentPosition.rot() ||
             maxAngle < currentPosition.rot())) ||
             isInSlowTurn)
        {
            if(counter == 0)
                Q_EMIT signalSendRobotControlParams(0,0);

            ///\todo consider removing the counter and replace through = new QElapsedTimer;
            if (counter > config::SENSOR_WAIT_COUNTER )
            {
                Position orientData = Orientation::beginOrientation(currentObjects);

                ObstacleType type;

                double abstandZuVorherigerPosition = sqrt(pow(orientData.x() - previousPosition.x(), 2) + pow(orientData.y() - previousPosition.y(), 2));

                bool deviationIsOk = abstandZuVorherigerPosition < config::SENSOR_MEASUREMENT_DEVIATION ;

                if (orientData.certainty() > 0.8 && deviationIsOk)
                {
                    if (config::enableDebugSensorHighLevel)
                        qDebug() << "MEASUREMENT ::::::" << previousPosition.x() << "orientData.x" << orientData.x();

                    // Mean value here?
                    Position finalPos(0.5*previousPosition.x() + 0.5*orientData.x(),
                                      0.5*previousPosition.y() + 0.5*orientData.y(),
                                      orientData.rot(),
                                      orientData.certainty());

                    if (MapData::getSimulationDetected())
                    {
                        type = ObstacleType::DUMMY;

                        if (config::enableDebugSensorHighLevel)
                            qDebug() << "type: DUMMY!";

                        MapData::setObstacle(Obstacle(finalPos,
                                                      ObstacleColor::UNIDENTIFIED,
                                                      type,
                                                      ObstacleStatus::UNBLOCKED));
                    }
                    else
                    {
                        type = ObstacleType::ME;

                        if (config::enableDebugSensorHighLevel)
                            qDebug() << "type: ME!";

                        Q_EMIT signalSendOdometryData(finalPos);
                    }

                    QList<Obstacle> cPoleList = QList<Obstacle>();

                    for (int i = 0; i < cPolePositions.size(); ++i) {
                        cPoleList << Obstacle(cPolePositions.at(i), ObstacleType::POLE);
                    }

                    MapData::setObstacle(cPoleList);

                    targetsSet = false;

                    if (config::enableDebugSensorHighLevel)
                        qDebug() << "SensorState changed: "  << "COLOR_DETECTION_START";


                    counter = 0; // Muss bei jedem Statewechsel zurückgesetzt werden, weil der Roboter beim Statewechsel noch 5 mal Laserdaten im Stehen messen soll

                    PathPlanning::setAvoidRestOfField(true); // Der Pfadplanung mitteilen, dass sie nur innerhalb des eigenen Halbfelds planen darf
                    this->setState(SensorStates::COLOR_DETECTION_START);

                }
                else
                {
                    counter ++;

                    if (config::enableDebugSensorHighLevel)
                        qDebug()<<"very slow turn for next LaserData";

                    // will be called once!
                    if (counter == config::SENSOR_WAIT_COUNTER + 2)
                    {
                        // now it can turn over the maxAngle
                        isInSlowTurn = true;
                        // Drehen, langsam, bis Abstände erkannt werden
                        Q_EMIT signalSendRobotControlParams(0,0.8*config::GUIREMOTETURNRATE);
                    }

                    // wenn wir zu lange gewartet haben oder die abweichung zu hoch war bei einer gemeldeten position von beginnOrientation
                    if (counter > config::SENSOR_WAIT_COUNTER * 4 ||
                        (!deviationIsOk && orientData.certainty() > 0.8))
                    {
                        isInSlowTurn = false;
                        Q_EMIT signalSendRobotControlParams(0,0);
                        counter = 0; // Muss bei jedem Statewechsel zurückgesetzt werden, weil der Roboter beim Statewechsel noch 5 mal Laserdaten im Stehen messen soll

                        this->setState(SensorStates::ORIENTATION);
                    }
                }
            }
            else
            {
                counter++;
            }

        }
        else
        {
            // the angle rotation speed is connected to the angle to go
            double angleToGo;

            if (minAngle > M_PI) {
                angleToGo = minAngle - currentPosition.rot();
            }
            else {
                if ( currentPosition.rot() > M_PI)
                    angleToGo = (2*M_PI - currentPosition.rot()) + minAngle;
                else
                    angleToGo = minAngle - currentPosition.rot();
            }

            if (config::enableDebugSensorHighLevel)
                qDebug() << "<<<<<<<<<<<<<<< ANGLE TO GO: " << angleToGo;

            // Schnell um 180deg drehen
            Q_EMIT signalSendRobotControlParams(0,(3*angleToGo/ M_PI + 1)*1.5*config::GUIREMOTETURNRATE);
        }
    }
        break;
    case SensorStates::COLOR_DETECTION_START:
    {
        // Erkennt Obstacles
        recognition(currentObjects, transmissionPosition);


        //Positioning
        if (!targetsSet)
        {
            QPair<double,double> torSeite = (dummyPosition.x() > 1.5) ?
                        qMakePair(config::geoFieldWidth * 0.667, // Rechte Torkante
                                  config::geoPol_1_2 * 1.5) : // Mittlere Torhöhe
                        qMakePair(config::geoFieldWidth * 0.333, // Rechte Torkante
                                  config::geoPol_1_2 * 1.5); // Mittlere Torhöhe

            MapData::setObstacle(Obstacle( torSeite,
                                           ObstacleColor::UNIDENTIFIED,
                                           ObstacleType::TARGET,
                                           ObstacleStatus::UNBLOCKED,
                                           (dummyPosition.x() > 1.5) ? M_PI : 0) // Nach links/rechts gucken
                                 );

            // wait 500ms for color detection
            timerWaitForValidColorFrame->start();
            targetsSet = true;
        }
        else
        {
            // the only 2 targets which have been set till now are completed
            if(MapData::getObstacle(ObstacleType::TARGET).isEmpty() &&
               timerWaitForValidColorFrame->isValid() &&
               timerWaitForValidColorFrame->elapsed() > 500)
            {
                if(config::enableDebugSensorHighLevel)
                    qDebug() << "Wait 500ms to be sure to get the latest cam frame";

                // start again for 50ms redetection cycle
                timerWaitForValidColorFrame->start();
                timerToAbondonColorDetection->start();

                Q_EMIT signalStartColorDetection();

                if(config::enableDebugSensorHighLevel) {
                    qDebug() << "Position erreicht --> Farbe prüfen";
                    qDebug() << "SensorState changed: "  << "COLOR_DETECTION_WAIT";
                }
                this->setState(SensorStates::COLOR_DETECTION_WAIT);

                //Löschen des Dummy
                MapData::deleteObstacle(ObstacleType::DUMMY);
            }
        }
    }
        break;
    case SensorStates::COLOR_DETECTION_WAIT:
    {
        // Erkennt Obstacles
        recognition(currentObjects, transmissionPosition);

        // wait 5secs to abort color detection
        if(timerToAbondonColorDetection->isValid() &&
           timerToAbondonColorDetection->elapsed() > 5000)
        {
            qDebug() << "Failed overall color detection";
            timerToAbondonColorDetection->invalidate();
            timerWaitForValidColorFrame->invalidate();
            driveToPreposition();
            this->setState(SensorStates::PRE_GAME_STATE);
        }

        if(timerWaitForValidColorFrame->isValid() &&
           timerWaitForValidColorFrame->elapsed() > 50)
        {
            timerWaitForValidColorFrame->start();
            // QThread::sleep removed problems with recognition of objects
            Q_EMIT signalStartColorDetection();
        }

        // the rest is in the slotColorDetected()
    }
        break;
    case SensorStates::PRE_GAME_STATE:
    {
        // Erkennt Obstacles
        recognition(currentObjects, transmissionPosition);

        // fährt zur Preposition und setzt State auf RECODNITION wenn er dort angekommen ist
        driveToPreposition();

    }
        break;
    case SensorStates::RECOGNITION:
    {
        // Erkennt Obstacles
        recognition(currentObjects, transmissionPosition);
    }
        break;
    default:
    {
        if (config::enableDebugSensorHighLevel)
            qDebug() << "HighLevelSensor unknown state";
    }
    }
}

void SensorHighLevel::driveToPreposition()
{
    // Target finden und setzen
    if(!prepositionInitialized)
    {
        double distance = 99;
        Position target = Position(config::geoFieldWidth/2,(config::geoFieldHeight / 3) - config::DISTANCE_TO_WAITING_LINE, M_PI/2);
        QList<Obstacle> puckList = MapData::getObstacle(ObstacleType::PUCK);

        for (int i = 0; i < puckList.size(); ++i)
        {
            if (puckList.at(i).isInSpecifiedArea(FieldArea::NEUTRAL_AREA))
            {    // Puck muss sich in der neutralen Zone befinden
                Position puckPos = puckList.at(i).getPosition();

                bool isNotCloseToPole = puckPos.x() > 1.3 * config::SENSOR_RADIUS_ROBOT &&
                                        puckPos.x() < config::geoFieldWidth - 1.3 * config::SENSOR_RADIUS_ROBOT;

                if (isNotCloseToPole && puckPos.y() < distance)
                {
                    distance = puckPos.y();
                    target.x(puckPos.x());
                }
            }
        }

        MapData::clearTargets();
        MapData::setObstacle(Obstacle(target,ObstacleType::TARGET));
        prepositionInitialized = true;
    }

    // Am Target angekommen --> nur noch erkennen
    if(MapData::getObstacle(ObstacleType::TARGET).size() == 0)
        this->setState(SensorStates::RECOGNITION);
}

// Kollisionsvermeidung fuer Handsteuerung ohne Pfadplanung
void SensorHighLevel::avoideCollision(QVector<double> &rawDepthsVector)
{
    if(rawDepthsVector.length() == 0)
        return;

    // set true if detects emergency
    bool isEmergency = false;

    // how many data points have been send?
    int iDataCount = rawDepthsVector.size();

    // each data point covers "dRadPerValue" RAD of the 180degree in front of robot
    double dRadPerValue = M_PI / static_cast<double>(iDataCount);

    // calculate how many data points have to be evaluated to detect an obstacle in front of the robot
    int iHalfRange = ceil(atan2(2*config::SENSOR_RADIUS_ROBOT, (config::SENSOR_COLLISION_AT + 2*config::SENSOR_RADIUS_ROBOT))
                          / dRadPerValue);

    int iFrom = floor(iDataCount / 2.0) - iHalfRange;
    int iTo   = floor(iDataCount / 2.0) + iHalfRange;

    // there is an emergency, if only one Data Point detects an obstacle
    for ( int i = iFrom; i <= iTo; ++i)
        isEmergency |= (rawDepthsVector.at(i) < config::SENSOR_COLLISION_AT);

    // send signal if the state of emergency has changed
    if (isEmergency != hadEmergency) {
        hadEmergency = isEmergency;
        Q_EMIT signalEmergencyStopEnabled(isEmergency);
    }
}

CamColor SensorHighLevel::getTeamColor() const
{
    QMutexLocker locker (mutexTeamColor);
    return teamColor;
}

void SensorHighLevel::setTeamColor(const CamColor &value)
{
    QMutexLocker locker (mutexTeamColor);
    teamColor = value;
}

/**
 * @brief SensorHighLevel::recognition Soll Objekte,
 * die sich im angemessenen Abstand befinden erkennen und ggf identifizieren
 * @param objects QVector3Ds mit x=entfernung, y=winkel (0°=links vom roboter, 90°=vorm roboter), z=breite
 * @return
 */
void SensorHighLevel::recognition(QList<Position> &objects, Position &transmissionPosition)
{
    // calculate the correct global position of all objects
    for (int i = 0; i < objects.size(); ++i) {
        objects[i] = Orientation::getGlobalPolarPosition(objects.at(i), transmissionPosition);
    }

    bool listIsEqual = true;

    if(previousObjects.size() != objects.size()) {
        listIsEqual = false;
    }
    else {
        bool hasElement = false;

        for(int i = 0; i < objects.size(); ++i) {
            for (int j = 0; j < previousObjects.size(); ++j) {
                if( objects.at(i) == previousObjects.at(j)) {
                    hasElement = true;
                    break;
                }
                else {
                    hasElement = false;
                }
            }
            listIsEqual = hasElement;
            if(!listIsEqual)
                break;
        }
    }

    // targets must be updated
    for (int i = 0; i < objects.size(); ++i){

        if ( objects.at(i).sizeType() == SizeType::POLE )
        {
            MapData::setObstacle(Obstacle(objects.at(i),
                                          ObstacleStatus::BLOCKED,
                                          ObstacleType::POLE)
                                 );
        }
        else if (objects.at(i).sizeType() == SizeType::PUCK)
        {
            MapData::setObstacle(Obstacle(objects.at(i),
                                          ObstacleColor::NOMODIFY,
                                          ObstacleType::PUCK,
                                          ObstacleStatus::NOMODIFY),
                                 transmissionPosition
                                 );
        }
        else if (objects.at(i).sizeType() == SizeType::ROBOT)
        {
            MapData::setObstacle(Obstacle(objects.at(i),
                                          ObstacleType::OPPONENT)
                                 );
        }
        else // THIS WILL NOT HAPPEN TILL NOW!!!
        {
            MapData::setObstacle(Obstacle(objects.at(i),
                                          ObstacleStatus::NOMODIFY,
                                          ObstacleType::NOMODIFY)
                                 );
        }
    }

    // only with big differences the path will be planned again
    if (!listIsEqual)
        Q_EMIT signalPlanNewPath();

    // speichere die neuen objekte ab
    previousObjects = objects;
}

/**
 * @brief SensorHighLevel::extractObjects Geht durch die sensorData list und überprüft auf entfernungen zwischen zwei sensordatenpunkten
 * @param sensorData Eingangsvektor
 * @param objects Referenz auf Vektor für Rückgabe - QVector3D beinhaltet Tiefe - Winkel (rad) - Breite
 */
QList<Position> SensorHighLevel::extractObjects(const ConstrainedLaserData &constrainedData)
{
    // current state
    SensorStates tempState = this->getState();

    // vector with found objects
    QList<Position> objects = QList<Position>();

    // minimum value count for 1 object
    int minObjectValues;

    if(MapData::getSimulationDetected())
        minObjectValues = 2;
    else
        minObjectValues = 2;

    // check sizes of both vectors
    int angleVectorSize = constrainedData.angles().size();
    int depthVectorSize = constrainedData.filteredDepths().size();

    if(angleVectorSize == 0 ||
       angleVectorSize != depthVectorSize)
        return objects;

    // Der erste in den daten enthaltene wert ist auf jeden fall teil eines objekts, daher daten hier initialisieren
    int objectBeginning = 0;
    int objectEnd = 0;

    // distance between two polar coordinates
    double dist = 0.0;
    Position polarCoordsPositionObject;

    // start with second item in the vector
    for (int i = 1; i < angleVectorSize; ++i)
    {

        // angleA, depthA, angleB, depthB
        dist = Orientation::distancePolar(constrainedData.angles().at(i-1), constrainedData.filteredDepths().at(i-1),
                                           constrainedData.angles().at(i),   constrainedData.filteredDepths().at(i));

        // Wenn die nebeneinanderliegenden Punkte nahe zu einander sind: die objektgrenze verschieben
        // Problem: Ohne einen Punkt ganz rechts, wird das letzte Objekt nicht erkannt -> zweite Bedingung notwendig
        if(dist < config::SENSOR_MAX_DISTANCE_OF_OBJ && i != angleVectorSize - 1)
        {
            objectEnd = i;

        } else { // die punkte liegen nicht nahe beieinander: objektkante

            // für den letzten Punkt muss die letzte Koordinate korrekt gesetzt werden!
            if (dist < config::SENSOR_MAX_DISTANCE_OF_OBJ && i == angleVectorSize - 1)
                objectEnd = i;

            // ab 3 Punkten is es ein Objekt!
            ///\todo also ich mache es hier mal von der simulation abhängig, ob 2 oder 3 werte für ein object genügen!
            if (objectEnd - objectBeginning >= minObjectValues)
            {
                // calculateObjCenter gibt zurück Position(realDepth, realAngle, radius, sizeType) statt (x,y,rot,sizeType)
                polarCoordsPositionObject = calculateObjCenter(constrainedData, objectBeginning, objectEnd, tempState);

                // position ist missbraucht - rot = width
                double objectWidth = polarCoordsPositionObject.rot();

                bool isValidObject = true;

                switch(tempState)
                {
                case SensorStates::WAIT:
                case SensorStates::ORIENTATION:
                case SensorStates::ORIENTATION_VALIDATION:
                {
                    // in these states only poles are relevant - everything else will be discarded
                    if(MapData::getSimulationDetected())
                        isValidObject = objectWidth < 2.1 * config::geoPoleRadiusSim;
                    else
                        isValidObject = objectWidth < 2.1 * config::geoPoleRadiusReal;
                }
                    break;
                default: // all other states
                {
                    // if the object width is bigger than the robo diameter
                    // it wont be added -> it migth be a wall
                    isValidObject = objectWidth < 2.1* config::SENSOR_RADIUS_ROBOT;
                }
                }

                if(isValidObject)
                {
                    // dieses object darf übernommen werden
                    objects.append(polarCoordsPositionObject);
                }
            }

            objectBeginning = i;
            objectEnd  = i;
        }
    }

    return objects;
}

void SensorHighLevel::constrainData(const QVector<double> &filteredDepthsVector,
                                    const QVector<double> &rawDepthsVector,
                                    ConstrainedLaserData &constrainedData)
{
    constrainedData.clearData();

    SensorStates tempState = this->getState();

    int dataInLength = filteredDepthsVector.length();

    // Neu: Für die Validierung wird das Feld auf das Orientierungsfeld begrenzt, anschließend nicht mehr...
    double lengthY;
    double worldOrientationOfPoint;
    double worldXOfPoint;
    double worldYOfPoint;

    double maxRange = (tempState == SensorStates::ORIENTATION) ? config::SENSOR_MAX_RANGE_ORIENTATION : config::SENSOR_MAX_RANGE_RECOGNITION;

    for (int i = 0; i < dataInLength; ++i)
    {
        double currentAngle = i * M_PI / (dataInLength-1);

        // keine datenpunkte, die weiter als maxrange entfernt sind
        if(filteredDepthsVector.at(i) > maxRange){
            continue;
        }

        switch(tempState) {
        //case SensorStates::WAIT:
        case SensorStates::COLOR_DETECTION_START:
        case SensorStates::COLOR_DETECTION_WAIT:
        case SensorStates::PRE_GAME_STATE:
        case SensorStates::RECOGNITION:
        {
            lengthY = config::geoFieldHeight;
            worldOrientationOfPoint = transmissionPosition.rot() + M_PI_2 - currentAngle; // winkel, auf arena bezogen
            worldXOfPoint = transmissionPosition.x() + filteredDepthsVector.at(i) * cos(worldOrientationOfPoint);
            worldYOfPoint = transmissionPosition.y() + filteredDepthsVector.at(i) * sin(worldOrientationOfPoint);
        }
            break;
        case SensorStates::ORIENTATION_VALIDATION:
        {
            dummyPosition = MapData::getRobotPosition(ObstacleType::DUMMY);

            lengthY = config::geoPol_1_2 + config::geoPol_2_3;
            worldOrientationOfPoint = dummyPosition.rot() + M_PI_2 - currentAngle; // winkel, auf arena bezogen
            worldXOfPoint = dummyPosition.x() + filteredDepthsVector.at(i) * cos(worldOrientationOfPoint);
            worldYOfPoint = dummyPosition.y() + filteredDepthsVector.at(i) * sin(worldOrientationOfPoint);
        }
            break;
        default: //SensorStates::ORIENTATION
        {
            // wenn wir hier ankommen, sollte der punkt in ordnung sein
            constrainedData.addFilteredDepth(filteredDepthsVector.at(i));
            constrainedData.addRawDepth(rawDepthsVector.at(i));
            constrainedData.addAngle(currentAngle);

            continue;
        }
        }

        // ist der datenpunkt innerhalb des spielfelds?
        if(   worldXOfPoint < 0-config::SENSOR_OUT_OF_FIELD_TOLERANCE ||
              worldYOfPoint < 0-config::SENSOR_OUT_OF_FIELD_TOLERANCE ||
              worldXOfPoint > config::geoFieldWidth + config::SENSOR_OUT_OF_FIELD_TOLERANCE ||
              worldYOfPoint > lengthY + config::SENSOR_OUT_OF_FIELD_TOLERANCE ){
            continue;
        }

        // wenn wir hier ankommen, sollte der punkt in ordnung sein
        constrainedData.addFilteredDepth(filteredDepthsVector.at(i));
        constrainedData.addRawDepth(rawDepthsVector.at(i));
        constrainedData.addAngle(currentAngle);
    }
}

// Neue Filter-Parameter aus der GUI erhalten
void SensorHighLevel::slotSetFilterParams(FilterParams cp)
{
    QMutexLocker locker(mutexFilterParameter);

    filterParameter.kernel=cp.kernel;

    if (config::enableDebugSensorHighLevel)
        qDebug()<<"New Kernel parameter";
}

void SensorHighLevel::slotStartDetection(bool start)
{
    if(start)
    {
        //qDebug() << "start detection";
        this->setState(SensorStates::ORIENTATION);
    }
    else {
        Q_EMIT signalSendRobotControlParams(0,0);
        this->setState(SensorStates::WAIT);
    }
}

void SensorHighLevel::slotColorDetected(CamColor color)
{
    this->setTeamColor(color);

    if ( getState() == SensorStates::COLOR_DETECTION_WAIT )
    {
        CamColor tempColor = this->getTeamColor();

        switch(tempColor){
        case CamColor::BLUE:
        {
            MapData::setProbableColor(tempColor);
            Q_EMIT signalSendTeamColor( tempColor);
            driveToPreposition();
            this->setState(SensorStates::PRE_GAME_STATE);
            timerToAbondonColorDetection->invalidate();
            timerWaitForValidColorFrame->invalidate();
        }
            break;
        case CamColor::YELLOW:
        {
            MapData::setProbableColor(tempColor);
            Q_EMIT signalSendTeamColor( tempColor);
            driveToPreposition();
            this->setState(SensorStates::PRE_GAME_STATE);
            timerToAbondonColorDetection->invalidate();
            timerWaitForValidColorFrame->invalidate();
        }
        default:
        {
            if(config::enableDebugSensorHighLevel) {
                qWarning() << "Unbekannte tatsächliche Teamfarbe: "
                           << static_cast<int>(tempColor)
                           << ", versuche es vielleicht in 50ms nochmal";
            }
        }
            break;
        } // switch color
    } // if correct state
}

SensorStates SensorHighLevel::getState() const
{
    QMutexLocker locker (mutexState);
    return currentState;
}

void SensorHighLevel::setState(const SensorStates &value)
{
    QMutexLocker locker (mutexState);
    currentState = value;
}


// returning coordinates of the middle of the object,  - depth - angle and width
Position SensorHighLevel::calculateObjCenter(const ConstrainedLaserData &constrainedData,
                                             int objectBeginn, int objectEnd, SensorStates &tempState)
{
    // Neuer Code
    double radiusPole = config::geoPoleRadiusReal;
    double radiusPuck = config::geoPuckRadiusTopReal;
    double radius; // radius of different regognized elements
    double distAB;  // distance polar between beginn and end
    double theta; // angle of triangle(pA,pB,Mid) at pA
    double phi; // angle of triangle(robo,pA,pB) at pA

    double realAngle; // the correct value for polar depth
    double realDepth; // the correct value for polar angle

    double pAAngle = constrainedData.angles().at(objectBeginn);
    double pADepth = constrainedData.filteredDepths().at(objectBeginn);
    double pBAngle = constrainedData.angles().at(objectEnd);
    double pBDepth = constrainedData.filteredDepths().at(objectEnd);

    /*
    qDebug() << "Object Values left to right:";
    for(int i = objectBeginn; i<= objectEnd; ++i)
        qDebug() << "Value_" << i << ": " << anglesAndDepths.second.at(i);
    */

    double meanAngle = 0.5 * (pAAngle + pBAngle);
    double meanDepth = (pADepth > pBDepth) ? pADepth : pBDepth;

    //
    SizeType sizeType = SizeType::UNKNOWN;

    // distance between pA and pB
    distAB = Orientation::distancePolar(pAAngle, pADepth,
                                         pBAngle, pBDepth);

    if(MapData::getSimulationDetected())
    {
        radiusPole = config::geoPoleRadiusSim;
        radiusPuck = config::geoPuckRadiusTopSim;
    }

    switch (tempState) {
    case SensorStates::WAIT:
    case SensorStates::ORIENTATION:
    case SensorStates::ORIENTATION_VALIDATION:
    {
        if (distAB >= 2*radiusPole)
            radius = 0.5 * distAB + distAB/100;
        else
            radius = radiusPole;

        sizeType = SizeType::POLE;
    }
        break;
    default: // CARLO WARUM MACHST DU KEINE KOMMENTARE!!1!elf Ist wahrscheinlich für SensorStates::RECOGNITION gedacht?!
    {        // @Laurenz: JA, andere states auch aber das passiert fast nie
        Position absolutePos = Orientation::getGlobalPolarPosition(Position(meanDepth, meanAngle, distAB, 1.0),
                                                                      transmissionPosition);
        ///\todo may be cheating here with the distAB
        if (cPolePositions.contains(absolutePos)) {
            if (distAB >= 2*radiusPole)
                radius = 0.5 * distAB + distAB/100;
            else
                radius = radiusPole;

            sizeType = SizeType::POLE;
        }
        else if (distAB > config::SENSOR_OBJECTWIDTH_ROBO) {
            radius = 0.5 * distAB + distAB/100;

            sizeType = SizeType::ROBOT;
        }
        else {
            if (distAB >= 2*radiusPuck)
                radius = 0.5 * distAB + distAB/100;
            else
                radius = radiusPuck;

            sizeType = SizeType::PUCK;
        }
    }
    }

    // theta (angle of triangle(pA,pB,Mid) at pA)
    theta = 0.5 * (M_PI - qAcos(1 - distAB*distAB / (2 * radius * radius)));

    // angle pA,pB, robot
    phi = Orientation::angleBetweenAB(distAB, pADepth, pBDepth);

    // distance mid - robo
    realDepth = Orientation::distancePolar(radius, pADepth, theta + phi);

    realAngle = pAAngle + Orientation::angleBetweenAB(pADepth, realDepth, radius);


    if (config::enableDebugSensorHighLevel)
    {
        qDebug() << "****";
        qDebug() << "ObjectAngle_" << objectBeginn << ": " << pAAngle;
        qDebug() << "ObjectAngle_" << objectEnd << ": " << pBAngle;
        qDebug() << "ObjectDepth_" << objectBeginn << ": " << pADepth;
        qDebug() << "ObjectDepth_" << objectEnd << ": " << pBDepth;
        qDebug() << "ObjectWidth: " << distAB;
        qDebug() << "Theta: " << theta;
        qDebug() << "phi: " << phi;
        qDebug() << "RealDepth: " << realDepth;
        qDebug() << "RealAngle: " << realAngle;
        qDebug() << "RealWidth: " << 2 * radius;
        qDebug() << "****";
    }

    return Position(realDepth, realAngle, radius, sizeType);
}


