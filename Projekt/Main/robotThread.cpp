#include "robotThread.h"

#include <QDebug>
#include <QThread>
#include <QCoreApplication>

#include "GUI/mainwindow.h"
#include "Data/define.h"
#include "player.h"
#include "Actor/actorLowLevel.h"
#include "Actor/actorhighlevel.h"
#include "Sensor/sensorhighlevel.h"
#include "Sensor/sensorLowLevel.h"
#include "AI/pathplanning.h"
#include "AI/gameengine.h"
#include "Structs/pathplotdata.h"
#include "Sensor/cam.h"
#include "AI/game.h"


/**
 * @brief RobotThread::RobotThread
 */
RobotThread::RobotThread(MainWindow *mainWindow) : mainWindow(mainWindow)
{
    // necessary to send this type over queued connections
    qRegisterMetaType< QVector<double> >("QVector<double>");
    qRegisterMetaType< QList<Obstacle> >("QList<Obstacle>");
    qRegisterMetaType< QList< QPair<double,double> > >("QList< QPair<double,double> >"); // fuer path planning -> path realizer
    qRegisterMetaType< PathPlotData >("PathPlotData");
    qRegisterMetaType< FilterParams >("FilterParams");
    qRegisterMetaType< CameraParams >("CameraParams");
    qRegisterMetaType< PIDParams >("PIDParams");
    qRegisterMetaType< PIDPlotData >("PIDPlotData");
    qRegisterMetaType< cv::Mat >("cv::Mat");
    qRegisterMetaType< QString >("QString"); //Log
    qRegisterMetaType< LaserPlotData > ("LaserPlotData");
    qRegisterMetaType< Position > ("Position");
    qRegisterMetaType< TeamColor >("TeamColor");
    qRegisterMetaType< CamColor >("CamColor");

    // Construct our modules
    actorLowLevel = new ActorLowLevel();
    actorHighLevel = new ActorHighLevel();
    sensorHighLevel = new SensorHighLevel();
    sensorLowLevel = new SensorLowLevel();
    pathPlanner = new PathPlanning();
    gameEngine = new GameEngine();
    cam = new Cam();
    game = new Game(gameEngine);


    // Move objects to different threads
    sensorLowLevel->moveToThread(&threadRobotLowLevel);
    sensorHighLevel->moveToThread(&threadSensorHighLevel);
    actorLowLevel->moveToThread(&threadRobotLowLevel);
    actorHighLevel->moveToThread(&threadActorHighLevel);
    pathPlanner->moveToThread(&threadPathPlanner);
    gameEngine->moveToThread(&threadGameEngine);
    cam->moveToThread(&threadCam);
    game->moveToThread(&threadGame);

    // Name the threads (helpful for debugging)
    threadRobotLowLevel.setObjectName("Robot Low Level Thread");
    threadSensorHighLevel.setObjectName("Sensor High Level Thread");
    threadActorHighLevel.setObjectName("Actor High Level Thread");
    threadPathPlanner.setObjectName("Path Planning Thread");
    threadGameEngine.setObjectName("Game Engine Thread");
    threadCam.setObjectName("Cam Thread");
    threadGame.setObjectName("Game Thread");

    // Print Thread IDs
//    qDebug() << "Threads:" << this->thread() << "," <<
//                sensorLowLevel->thread() << "," <<
//                sensorHighLevel->thread() << "," <<
//                actorHighLevel->thread() << "," <<
//                pathPlanner->thread() << "," <<
//                gameEngine->thread() << "," <<
//                cam->thread() << "," <<
//                game->thread();

    // ***********************************************************************
    // DELETE LATER
    connect(&threadRobotLowLevel, SIGNAL(finished()),
            sensorLowLevel, SLOT(deleteLater()));

    connect(&threadRobotLowLevel, SIGNAL(finished()),
            actorLowLevel, SLOT(deleteLater()));

    connect(&threadSensorHighLevel, SIGNAL(finished()),
            sensorHighLevel, SLOT(deleteLater()));

    connect(&threadActorHighLevel, SIGNAL(finished()),
            actorHighLevel, SLOT(deleteLater()));

    connect(&threadPathPlanner, SIGNAL(finished()),
            pathPlanner, SLOT(deleteLater()));

    connect(&threadCam, SIGNAL(finished()),
            cam, SLOT(deleteLater()));

    connect(&threadGameEngine, SIGNAL(finished()),
            gameEngine, SLOT(deleteLater()));

    connect(&threadGame, SIGNAL(finished()),
            game, SLOT(deleteLater()));

    // ***********************************************************************
    // SIGNALS FROM GUI

    // Test Puck Release
    connect(mainWindow, SIGNAL(signalTestPuckRelease()),
            actorHighLevel, SLOT(slotGatherPuck()));

    // Test der Kameraerkennung
    connect(mainWindow, SIGNAL(signalTestColorDetect()),
            cam, SLOT(slotStartColorDetection()));

    // test der KI
    connect(mainWindow,SIGNAL(signalTestStartGame()),
            gameEngine,SLOT(slotRefGameStart()));

    // start Orientation
    connect(mainWindow,SIGNAL(signalStartOrientation(bool)),
            sensorHighLevel, SLOT(slotStartDetection(bool)));

    // remote controle over GUI
    connect(mainWindow,SIGNAL(robotRemoteControllUpdate(double,double)),
            actorLowLevel,SLOT(setRobotRemoteControllParams(double,double)));

    // remote Odometry update
    connect(mainWindow,SIGNAL(updateRemoteOdometry(Position)),
            actorLowLevel, SLOT(setOdometry(Position)),
            Qt::ConnectionType::QueuedConnection);

    // Cam Display
    // Camera Parameters in GUI have changed, send to Cam class
    connect(mainWindow, SIGNAL(signalChangeCamParams(CameraParams)),
            cam, SLOT(slotSetCameraParams(CameraParams)));

    // Set Median Filter
    connect(mainWindow, SIGNAL(signalChangeFilterParams(FilterParams)),
            sensorHighLevel, SLOT(slotSetFilterParams(FilterParams)));

    // Set PIDs
    connect(mainWindow, SIGNAL(signalChangePIDParams(PIDParams)),
            actorHighLevel, SLOT(slotChangePIDParams(PIDParams)));


    // ***********************************************************************
    // SIGNALS TO GUI

    // update simulation detect
    connect(sensorLowLevel, SIGNAL(signalSimulationDetect()), // raw data
            mainWindow, SLOT(slotSimulationDetect()));

    // Sensor Display
    // Display the sensor data in GUI
    connect(sensorLowLevel, SIGNAL(signalLaserPlotRaw(LaserPlotData)), // raw data
            mainWindow, SLOT(slotLaserDisplay(LaserPlotData)));

    connect(sensorHighLevel, SIGNAL(signalSendLaserData(LaserPlotData)), // filtered data
            mainWindow, SLOT(slotLaserDisplay(LaserPlotData)));

    // Path Display
    // Display potential map and raw waypoints
    connect(pathPlanner, SIGNAL(pathDisplay(PathPlotData)),
            mainWindow, SLOT(updatePathDisplay(PathPlotData)));

    // Display calculated spline
    connect(actorHighLevel, SIGNAL(signalSplinePlot(PathPlotData)),
            mainWindow, SLOT(updatePathDisplay(PathPlotData)));

    // Display PID Plot
    connect(actorHighLevel, SIGNAL(signalPIDPlot(PIDPlotData)),
            mainWindow, SLOT(slotPIDPlot(PIDPlotData)));

    // Cam grabbed a frame, display in GUI
    connect(cam, SIGNAL(signalDisplayFrame(cv::Mat)),
            mainWindow, SLOT(slotDisplayFrame(cv::Mat)));

    connect(cam, SIGNAL(signalColorDetected(CamColor)),
            mainWindow, SLOT(slotUpdateColorLabel(CamColor)));

    // ***********************************************************************

    // emergency detected/received
    connect(sensorHighLevel,SIGNAL(signalEmergencyStopEnabled(bool)),
            actorLowLevel,SLOT(slotEmergencyStopEnabled(bool)));

    // stopMovement() von Referee
    connect(gameEngine, SIGNAL(signalEmergencyStopEnabled(bool)),
            actorLowLevel, SLOT(slotEmergencyStopEnabled(bool)));

    // send turn command from high level sensor to low level actor
    connect(sensorHighLevel,SIGNAL(signalSendRobotControlParams(double,double)),
            actorLowLevel,SLOT(setRobotControllParams(double,double)));

    // controle command from High Level Aktor
    connect(actorHighLevel,SIGNAL(signalSendRobotControlParams(double,double)),
            actorLowLevel,SLOT(setRobotControllParams(double,double)));

    // Get waypoints from path planning into the path realizer
    connect(pathPlanner, SIGNAL(sendUpdatedWaypoints(QList< QPair<double,double> >)),
            actorHighLevel, SLOT(slotUpdateWaypoints(QList< QPair<double,double> >)));

    // SENSOR DATA TO HIGH LEVEL SENSOR
    connect(sensorLowLevel,SIGNAL(signalLaserDataReady(QVector<double>, Position)),
            sensorHighLevel, SLOT(slotGetLaserData(QVector<double>, Position)));

    // start color detection from sensor
    connect(sensorHighLevel, SIGNAL(signalStartColorDetection()),
            cam, SLOT(slotStartColorDetection()));

    // color succesfully detected
    connect(cam, SIGNAL(signalColorDetected(CamColor)),
            sensorHighLevel, SLOT(slotColorDetected(CamColor)));

    // ***********************************************************************

    connect(sensorHighLevel, SIGNAL(signalSendOdometryData(Position)),
            actorLowLevel, SLOT(setOdometry(Position)),
            Qt::ConnectionType::QueuedConnection);

    connect(sensorHighLevel, SIGNAL(signalPlanNewPath()),
            pathPlanner, SLOT(planPath()));

    // ***********************************************************************
    // Signals from GameEngine
    connect(gameEngine,SIGNAL(signalStartDetection(bool)),
            sensorHighLevel, SLOT(slotStartDetection(bool)));

    // Bei Detection Phase Start Timer in GUI neu starten
    connect(gameEngine,SIGNAL(signalStartDetection(bool)),
            mainWindow,SLOT(slotRestartTimerDisplay()));

    connect(gameEngine,SIGNAL(signalAnnoyFoe()),
            game, SLOT(slotAnnoyFoe()));

    // Signals to GameEngine
    connect(game, SIGNAL(signalReportGoal()),
            gameEngine, SLOT(slotReportGoal()));

    connect(sensorHighLevel, SIGNAL(signalSendTeamColor(CamColor)),
            gameEngine, SLOT(slotDetectionFinished(CamColor)));

    connect(cam,SIGNAL(signalColorDetected(CamColor)),
            game,SLOT(slotColorDetect(CamColor)));

    connect(game,SIGNAL(signalStartColorDetectAI()),
            cam,SLOT(slotStartColorDetection()));

    connect(gameEngine,SIGNAL(signalStartGame()),
            game,SLOT(slotStartGame()));

    connect(game,SIGNAL(signalPuckRelease()),
            actorHighLevel,SLOT(slotReleasePuck()));

    connect(game,SIGNAL(signalPushAndRelease(double)),
            actorHighLevel,SLOT(slotPushAndReleasePuck(double)));

    connect(game, SIGNAL(signalGatherPuck()),
            actorHighLevel,SLOT(slotGatherPuck()));

    connect(actorHighLevel,SIGNAL(signalPuckDone()),
            game,SLOT(slotActorHighLevelIsDoneWithPuck()));


    // Starting the eventloop of all threads
    threadRobotLowLevel.start();
    threadSensorHighLevel.start();
    threadActorHighLevel.start();
    threadPathPlanner.start();
    threadCam.start();
    threadGameEngine.start();
    threadGame.start();

    MapData::setPointerToPathPlanner(pathPlanner);

    // Starte Modul Loops
    gameEngine->startGameEngine();
    QMetaObject::invokeMethod(sensorLowLevel, "run", Qt::AutoConnection);
    QMetaObject::invokeMethod(game, "run", Qt::AutoConnection);
    QMetaObject::invokeMethod(pathPlanner, "planPath", Qt::AutoConnection);
}

/** Destructor*/
RobotThread::~RobotThread()
{
    qDebug() << "DELETE RobotThread";

    game->quit();
    actorHighLevel->ignoreSignals();
    QCoreApplication::removePostedEvents(actorHighLevel); // Ensure no robotControl signals go out, because the player instance will be killed
    pathPlanner->setEnabled(false); // Stop the PathPlanning Loop
    sensorLowLevel->quit(); // Stop the sensor read loop

    // quit threads
    threadRobotLowLevel.quit();
    threadSensorHighLevel.quit();
    threadActorHighLevel.quit();
    threadPathPlanner.quit();
    threadCam.quit();
    threadGameEngine.quit();
    threadGame.quit();

    // wait for threads to finish
    threadRobotLowLevel.wait();
    threadSensorHighLevel.wait();
    threadActorHighLevel.wait();
    threadPathPlanner.wait();
    threadCam.wait();
    threadGameEngine.wait();
    threadGame.wait();
}

