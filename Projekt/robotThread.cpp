#include "robotThread.h"

#include "mainwindow.h"

#include <QDebug>

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

extern MainWindow* mainWindow;

/**
 * @brief RobotThread::RobotThread
 */
RobotThread::RobotThread()
{
    // necessary to send this type over qued connections
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

    qDebug() << "INIT 'RobotThread'";

    //initialize Objects for Actors and Sensory
    actorLowLevel   = new ActorLowLevel();
    actorHighLevel    = new ActorHighLevel();
    sensorHighLevel = new SensorHighLevel();
    sensorLowLevel  = new SensorLowLevel();
    pathPlanner     = new PathPlanning();
    gameEngine      = new GameEngine();
    cam             = new Cam();

    // moving objects to different threads
    sensorLowLevel->moveToThread(&threadRobotLowLevel);
    sensorHighLevel->moveToThread(&threadSensorHighLevel);
    actorLowLevel->moveToThread(&threadRobotLowLevel);
    actorHighLevel->moveToThread(&threadActorHighLevel);
    pathPlanner->moveToThread(&threadPathPlanner);
    gameEngine->moveToThread(&threadGameEngine);
    cam->moveToThread(&threadCam);

    // Name the threads (helpful for debugging)
    threadRobotLowLevel.setObjectName("Robot Low Level Thread");
    threadSensorHighLevel.setObjectName("High Level Sensor Thread");
    threadPathRealizer.setObjectName("Path Realizer Thread");
    threadPathPlanner.setObjectName("Path Planner Thread");
    threadGameEngine.setObjectName("Game Engine Thread");
    threadCam.setObjectName("Cam Thread");


    // ***********************************************************************
    // DELETE LATER
    connect(&threadRobotLowLevel, SIGNAL(finished()),
            sensorLowLevel, SLOT(deleteLater()));

    connect(&threadSensorHighLevel, SIGNAL(finished()),
            sensorHighLevel, SLOT(deleteLater()));

    connect(&threadRobotLowLevel, SIGNAL(finished()),
            actorLowLevel, SLOT(deleteLater()));

    connect(&threadActorHighLevel, SIGNAL(finished()),
            actorHighLevel, SLOT(deleteLater()));

    connect(&threadPathPlanner, SIGNAL(finished()),
            pathPlanner, SLOT(deleteLater()));

    connect(&threadCam, SIGNAL(finished()),
            cam, SLOT(deleteLater()));

    connect(&threadGameEngine, SIGNAL(finished()),
            gameEngine, SLOT(deleteLater()));

    // ***********************************************************************
    // SIGNALS FROM GUI

    // start Orientation
    connect(mainWindow,SIGNAL(signalStartOrientation(bool)),
            sensorHighLevel, SLOT(slotStartDetection(bool)));

    // remote controle over GUI
    connect(mainWindow,SIGNAL(robotRemoteControllUpdate(double,double)),
            actorLowLevel,SLOT(setRobotRemoteControllParams(double,double)));


    // remote Odometry update
    connect(mainWindow,SIGNAL(updateRemoteOdometry(Position)),
            actorLowLevel, SLOT(setOdometry(Position)));

    // Cam Display
    // Camera Parameters in GUI have changed, send to Cam class
    connect(mainWindow, SIGNAL(signalChangeCamParams(CameraParams)),
            cam, SLOT(slotSetCameraParams(CameraParams)));



    // Set Median Filter
    connect(mainWindow, SIGNAL(signalChangeFilterParams(FilterParams)),
            sensorHighLevel, SLOT(slotSetFilterParams(FilterParams)));

    // PID Tab
    connect(mainWindow, SIGNAL(signalChangePIDParams(PIDParams)),
            actorHighLevel, SLOT(slotChangePIDParams(PIDParams)));


    // ***********************************************************************
    // SIGNALS TO GUI

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

    // ***********************************************************************

    // emergency detected/received
    connect(sensorHighLevel,SIGNAL(signalEmergencyStopEnabled(bool)), // emergency stop durch kollisionsvermeidung
            actorLowLevel,SLOT(slotEmergencyStopEnabled(bool)));

    connect(gameEngine, SIGNAL(signalEmergencyStopEnabled(bool)), // stopMovement() von Referee
            actorLowLevel, SLOT(slotEmergencyStopEnabled(bool)));

    //send turn command from high sensor to low level actor
    connect(sensorHighLevel,SIGNAL(signalSendRobotControlParams(double,double)),
            actorLowLevel,SLOT(setRobotControllParams(double,double)));

    // controle command from High Level Aktor
    connect(actorHighLevel,SIGNAL(signalSendRobotControlParams(double,double)),
            actorLowLevel,SLOT(setRobotControllParams(double,double)));

    // Get waypoints from path planning into the path realizer
    connect(pathPlanner, SIGNAL(sendUpdatedWaypoints(QList< QPair<double,double> >)),
            actorHighLevel, SLOT(slotUpdateWaypoints(QList< QPair<double,double> >)));

    // SENSOR DATA TO HIGH LEVEL SENSOR
    connect(sensorLowLevel,SIGNAL(laserDataReady(QVector<double>)),
            sensorHighLevel, SLOT(getLaserData(QVector<double>)));

    connect(sensorLowLevel,SIGNAL(sonarDataReady(QVector<double>)),
            sensorHighLevel, SLOT(getSonarData(QVector<double>)));

    // start color detection from sensor
    connect(sensorHighLevel, SIGNAL(signalStartColorDetection()),
            cam, SLOT(slotStartColorDetection()));

    // color succesfully detected
    connect(cam, SIGNAL(signalColorDetected(CamColor)),
            sensorHighLevel, SLOT(slotColorDetected(CamColor)));

    // ***********************************************************************
    // update Odometry
    connect(sensorLowLevel,SIGNAL(updateOdometry()),
            actorLowLevel, SLOT(getOdometry()));

    connect(sensorHighLevel, SIGNAL(sendOdometryData(Position)),
            actorLowLevel, SLOT(setOdometry(Position)));

    // ***********************************************************************
    // Signals from GameEngine
    connect(gameEngine,SIGNAL(signalStartDetection(bool)),
            sensorHighLevel, SLOT(slotStartDetection(bool)));

    // Signals to GameEngine
    connect(sensorHighLevel, SIGNAL(signalSendTeamColor(CamColor)),
            gameEngine, SLOT(slotDetectionFinished(CamColor)));

    // starting the eventloop of all threads
    threadRobotLowLevel.start();
    threadSensorHighLevel.start();
    threadPathRealizer.start();
    threadPathPlanner.start();
    threadCam.start();
    threadGameEngine.start();


    // Starte Game Engine State Machine
    gameEngine->start();

    // Start the PathPlanning "loop"
    /// \todo: Das ist erstmal nur zum Debuggen drinnen.
    // Spaeter darf die Pfadplanung nur bei beginn der Spielphase aus der GameEngine heraus angestossen werden
    QMetaObject::invokeMethod(pathPlanner, "planPath");
}

/** Destructor*/
RobotThread::~RobotThread()
{
    qDebug() << "DELETE 'RobotThread'";

    pathPlanner->setEnabled(false); // Stop the PathPlanning Loop

    // quit threads
    threadRobotLowLevel.quit();
    threadSensorHighLevel.quit();
    threadPathRealizer.quit();
    threadPathPlanner.quit();
    threadCam.quit();
    threadGameEngine.quit();

    // wait for threads to finish
    threadRobotLowLevel.wait();
    threadSensorHighLevel.wait();
    threadPathRealizer.wait();
    threadPathPlanner.wait();
    threadCam.wait();
    threadGameEngine.wait();

    // delete objects
    //delete sensorHighLevel;
    //delete Pathplanner;
    //delete ActorLowLvl;
    //delete SensorLowLvl;
    //delete ActorHighLvl;
    //delete cam;
    //delete gameEngine;
}

