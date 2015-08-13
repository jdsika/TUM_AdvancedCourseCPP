#ifndef ROBOTTHREAD_H
#define ROBOTTHREAD_H

#include <QThread>

// Forward Declarations
class ActorLowLevel;
class ActorHighLevel;
class SensorHighLevel;
class SensorLowLevel;
class PathPlanning;
class GameEngine;
class Cam;

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
    RobotThread();
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
    // Attributes
    ActorLowLevel *actorLowLevel; /**< actor low level instance for interaction with the engines*/
    PathRealizer *pathRealizer;  /**< the actor high level instance which is responsible for path realizing*/
    SensorHighLevel *sensorHighLevel; /**< sensor high level is responsible for object recognition and position evaluation*/
    SensorLowLevel *sensorLowLevel; /**< will read the data from laser */
    PathPlanning *pathPlanner; /**< will calculate the best path based on a given target and recognised obstacles*/
    GameEngine *gameEngine; /**< will communicate with the angelina serve*/
    Cam *cam; /**< a cam object for gathering a video stream*/

    // different threads
    QThread threadRobotLowLevel; /**< Thread in which the player instance is accessible*/
    QThread threadPathRealizer; /**< Pathrealising and motion controler is computed in an own thread*/
    QThread threadPathPlanner;  /**< Pathplanning will compute the path in own thread*/
    QThread threadCam;          /**< gathering cam data should not slow down other threads therefore it gets an own one*/
    QThread threadGameEngine;   /**< the comunication with the game server have to be stable*/
    QThread threadSensorHighLevel; /**< Thread for the High Level Sensor*/
};


#endif // ROBOTTHREAD_H
