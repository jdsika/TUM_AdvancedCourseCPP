#include "sensorLowLevel.h"

#include <QDebug>
#include <QThread>
#include <QCoreApplication>

#include <libplayerc++/playerc++.h>

#include "Structs/laserplotdata.h"
#include "Data/define.h"
#include "Data/mapdata.h"
#include "Sensor/sensorhighlevel.h"
#include "Main/player.h"

SensorLowLevel::SensorLowLevel()
{
    quitting = false;
    state = INIT;

    robot = PlayerX::getInstance();
    robot->SetDataMode(PLAYER_DATAMODE_PUSH);
    robot->SetReplaceRule(true);
    laserProxy  = new PlayerCc::RangerProxy(robot,1);
//    try {
//        laserProxy->RequestConfigure();
//        laserProxy->Configure(laserProxy->GetMinAngle(),
//                              laserProxy->GetMaxAngle(),
//                              laserProxy->GetAngularRes(),
//                              laserProxy->GetMinRange(),
//                              laserProxy->GetMaxRange(),
//                              laserProxy->GetRangeRes(),
//                              32); // Frequency
//    } catch (PlayerCc::PlayerError err) {
//        qCritical() << QString::fromStdString(err.GetErrorStr());
//    }
    positionProxy = new PlayerCc::Position2dProxy(robot);

    previousOdometryPosition = Position();
    currentOdometryPosition = Position();
    currentOdometryTime = 0.0;
    previousOdometryTime = 0.0;
    laserTime = -1.0; // -1 verhindert sandwichfall am anfang

    elapsedTimer.start();
}

SensorLowLevel::~SensorLowLevel()
{
    if (config::enableDebugSensorLowLevel)
        qDebug() << "DELETE 'SensorLowLvl'";

    delete laserProxy;
    delete positionProxy;
}

void SensorLowLevel::quit(){
    quitting = true;
}

void SensorLowLevel::run(){
    while(!quitting){
        readSensorData();
        QCoreApplication::processEvents();
        QCoreApplication::sendPostedEvents();
    }
}

void SensorLowLevel::readSensorData()
{
    if (!robot->Connected()) {
        return;
    }

    double time = 0;
    try{
        // this blocks until new data comes
        robot->Read();
        time = elapsedTimer.elapsed();
    } catch(PlayerCc::PlayerError ex) {
        qWarning() << "Player Error:" << QString::fromStdString(ex.GetErrorStr());
        return;
    }

    switch (state) {
    case INIT:
    {
        // Im Init state wird die Länge des Laser-Arrays abgefragt. Damit wird erkannt, ob wir uns in einer
        // Simulation befinden oder nicht. Außerdem wird das laserData Vector zur speicherung späterer Daten
        // in der rcihtigen Größe angelegt

        if (laserProxy->IsValid() &&
            laserProxy->IsFresh() &&
            positionProxy->IsValid() &&
            positionProxy->IsFresh()) // I want to get a valid time for the first odometry
        {
            // get all ranger attributes
            laserProxy->RequestConfigure();

            if (config::enableDebugSensorLowLevel) {
                qDebug() << "LowLvlSensor Thread:" << QThread::currentThreadId();
                qDebug() << "#############################";
                //qDebug() << "Laser: PointCount: "   << laserProxy->GetPointCount();
                qDebug() << "Laser: Range Count: "  << laserProxy->GetRangeCount();
                qDebug() << "Laser: Min Range: "    << laserProxy->GetMinRange();
                qDebug() << "Laser: Max Range: "    << laserProxy->GetMaxRange();
                qDebug() << "Laser: Index: "        << laserProxy->GetIndex();
                qDebug() << "Laser: Angle Res: "    << laserProxy->GetAngularRes();
                qDebug() << "Laser: Min Angle: "    << laserProxy->GetMinAngle();
                qDebug() << "Laser: Max Angle: "    << laserProxy->GetMaxAngle();
                qDebug() << "Laser: Element Count: "<< laserProxy->GetElementCount();
                qDebug() << "Laser: Frequency: "    << laserProxy->GetFrequency();
                qDebug() << "Laser: IntensCnt: "    << laserProxy->GetIntensityCount();
                qDebug() << "Laser: Range Res: "    << laserProxy->GetRangeRes();
                qDebug() << "#############################";
            }

            laserArrayLength = laserProxy->GetRangeCount();

            /// the sensor count of the real robot has 361 values
            if (laserArrayLength <= 360){
                MapData::setSimulationDetected(true);
                qDebug() << "Laser has <=360 points --> sim";
            } else {
                MapData::setSimulationDetected(false);
                qDebug() << "Laser has >360 points --> real";
            }

            Q_EMIT signalSimulationDetect();

            laserData = QVector<double> (laserArrayLength); // Den Vektor für spätere Daten anlegen

            // init value for odometry time
            previousOdometryTime = time;
            currentOdometryTime = previousOdometryTime;
            previousOdometryPosition = Position(positionProxy->GetXPos(),
                                                positionProxy->GetYPos(),
                                                positionProxy->GetYaw());
            currentOdometryPosition = previousOdometryPosition;

            // need next fresh data in RUN state
            positionProxy->NotFresh();
            laserProxy->NotFresh();

            // In den Run State übergehen. Kein Break!
            state = RUN;
        } else {
            // Init noch nicht fertig, weil Laserdaten nicht valid.
            break;
        }
    }
    case RUN:
    {
        // Der Run state ist der Normalfall.
        // Sind neue Laserdaten angekommen?
        if(laserProxy->IsValid() && laserProxy->IsFresh()){
            // Die Daten einlesen
            laserTime = time;
            for ( int i = 0; i < laserArrayLength; ++i) {
                laserData[i] = laserProxy->GetRange(laserArrayLength - i - 1);
            }

            // Player mitteilen, dass diese Daten jetzt nicht mehr neu für uns sind.
            laserProxy->NotFresh();

//            if (config::enableDebugSensorLowLevel)
//                qDebug() << "Laser TimeStamp : " << laserTime;

            if(SensorHighLevel::streamSensorEnabled)
                Q_EMIT signalLaserPlotRaw(LaserPlotData(laserData, LaserPlotData::RAW));

        }
        // Sind neue Odometriedaten angekommen?
        if(positionProxy->IsValid() && positionProxy->IsFresh()){
            // remember previous position
            previousOdometryPosition = currentOdometryPosition;
            previousOdometryTime = currentOdometryTime;

            // Neue Daten einlesen
            currentOdometryTime = time;
            currentOdometryPosition = Position(positionProxy->GetXPos(),
                                               positionProxy->GetYPos(),
                                               positionProxy->GetYaw());

            // Player mitteilen, dass diese Daten jetzt nicht mehr neu für uns sind.
            positionProxy->NotFresh();

//            if (config::enableDebugSensorLowLevel)
//                qDebug() << "Odometry TimeStamp : " << currentOdometryTime;

            // Auf Sandwichfall überprüfen
            if(previousOdometryTime <= laserTime && laserTime <= currentOdometryTime) {
                // Lineare Interpolation
                double zeitAbstand = currentOdometryTime - previousOdometryTime;
                double anteilNachher = (zeitAbstand>0)? (laserTime - previousOdometryTime) / zeitAbstand : 1;
                double anteilVorher = 1 - anteilNachher;
                double iX = anteilVorher * previousOdometryPosition.x() + anteilNachher * currentOdometryPosition.x();
                double iY = anteilVorher * previousOdometryPosition.y() + anteilNachher * currentOdometryPosition.y();
                double iR = angleWeightedAverage(previousOdometryPosition.rot(), anteilVorher, currentOdometryPosition.rot(), anteilNachher);
                Position interpolatedPosition = Position(iX, iY, iR);

                if (config::enableDebugSensorLowLevel){
                    qDebug() << "Sandwichfall:" << previousOdometryTime << laserTime << currentOdometryTime;
                    qDebug() << "Interpolation" << static_cast<int>(anteilVorher*100) << "% -" << static_cast<int>(anteilNachher*100) << "%";
                }

                // Daten senden
                // @laurenz: ich denke es sollte die tatsächliche Position in der map stehen
                MapData::setObstacle(Obstacle(currentOdometryPosition,
                                              ObstacleColor::ME,
                                              ObstacleType::ME,
                                              ObstacleStatus::UNBLOCKED));

                Q_EMIT signalLaserDataReady(laserData, interpolatedPosition);

            }

        }
        if (config::enableDebugSensorLowLevel)
            qDebug() << " ";

        break;
    }
    } // switch(state)
}

/// @see http://stackoverflow.com/a/1687116
double SensorLowLevel::angleWeightedAverage(const double &angle1,
                                            const double &weight1,
                                            const double &angle2,
                                            const double &weight2)
{
    double x = weight1 * cos(angle1) + weight2 * cos(angle2);
    double y = weight1 * sin(angle1) + weight2 * sin(angle2);
    return atan2(y,x);
}

