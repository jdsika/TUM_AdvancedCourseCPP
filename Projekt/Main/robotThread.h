/*! \mainpage
 *
 * TUM EI LDV CPP LK 14 15
 *   *** Prolog ***
 * Die Dokumentation zum Leistungskurs C++ besteht aus zwei Grundlegenden Elementen.
 * Der Programmcode ist konsequent und ausführlich im "doxygen-Stil" Kommentiert, um eine
 * automatische Dokumentation mit Doxygen zu ermöglichen. Diese kann mit "make doc" erstellt werden.
 * Des Weiteren werden alle Ideen und Informationen zum Projekt in diesem Wiki gespeichert, um zu
 * garantieren, dass die Entwicklung des Projekts für den Außenstehenden nachzuvollziehen ist.
 *
 * \author Carlo van Driesten
 * \author Konrad Vowinkel
 * \author Laurenz Altenmüller
 * \author Quirin Körner
 * \author  Jan Ehrensperger
 *
 */

#ifndef ROBOTTHREAD_H
#define ROBOTTHREAD_H

#include <QThread>

// Forward Declarations
class MainWindow;
class ActorLowLevel;
class ActorHighLevel;
class SensorHighLevel;
class SensorLowLevel;
class PathPlanning;
class GameEngine;
class Cam;
class Game;

/**
 * @brief The RobotThread class is responsible for the communication of all classes and is
 * the software representation of the robot, all threads are forked there and will joinen in the end.
 * The class will move diverent tasks to different threads
 */
class RobotThread : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief RobotThread will initialise all objects
     */
    RobotThread(MainWindow* mainWindow);

    /**
      * @brief ~RobotTread will free the heap space after programs termination
      */
    ~RobotThread();

    /**
     * @brief getPathPlanning
     * @return a pointer to an pathPlanner
     */
    const PathPlanning* getPathPlanning() {return pathPlanner;}

private:
    MainWindow* mainWindow; /**< pointer to mainWindow for easy connecting of signals and slots*/

    // Attributes
    ActorLowLevel *actorLowLevel; /**< actor low level instance for interaction with the engines*/
    ActorHighLevel *actorHighLevel;  /**< the actor high level instance which is responsible for path realizing*/
    SensorHighLevel *sensorHighLevel; /**< sensor high level is responsible for object recognition and position evaluation*/
    SensorLowLevel *sensorLowLevel; /**< will read the data from laser */
    PathPlanning *pathPlanner; /**< will calculate the best path based on a given target and recognised obstacles*/
    GameEngine *gameEngine; /**< will communicate with the angelina serve*/
    Cam *cam; /**< a cam object for gathering a video stream*/
    Game *game; /**< will be the Game*/

    // different threads
    QThread threadRobotLowLevel; /**< Thread in which the player instance is accessible*/
    QThread threadActorHighLevel; /**< Pathrealising and motion controler is computed in an own thread*/
    QThread threadSensorHighLevel; /**< Object recognition is computed in an own thread*/
    QThread threadPathPlanner;  /**< Pathplanning will compute the path in own thread*/
    QThread threadCam;          /**< gathering cam data should not slow down other threads therefore it gets an own one*/
    QThread threadGameEngine;   /**< the comunication with the game server have to be stable*/
    QThread threadGame;
};


#endif // ROBOTTHREAD_H
