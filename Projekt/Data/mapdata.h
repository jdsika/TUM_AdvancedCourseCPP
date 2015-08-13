#ifndef MAPDATA_H
#define MAPDATA_H

#include "Data/obstacle.h"
#include <referee.h>

#include <QList>
#include <QMutex>

enum class CamColor;
class Position;
class PathPlanning;

/**
 * @brief The MapData class is a static class for inter-thread
 * communication and saving information for other parts of the programm
 */

class MapData
{    
    // hidden
private:
    // hidden constructor
    /**
     * @brief MapData hidden constructor
     */
    MapData();
    // hidden destructor
    /**
      *@brief ~MapData hidden destructor
      */
    ~MapData();
    // hidden copy constructor
    /**
     * @brief MapData as hidden copy constructor
     */
    MapData(const MapData &);

    // hidden assign operator
    /**
     * @brief operator =  we leave just the declarations, so the compiler will warn us
    // if we try to use those two functions by accident

     * @return assigned value
     */
    MapData& operator=(const MapData &);


    // elements to be found on the map
private:
    static PathPlanning *pointerToPathPlanner;           /**< For invoke method */
    static QList<Obstacle>  obstaclesPoles,             /**< QList of obstacles identified as poles*/
                            obstaclesPucks,             /**< QList of obstacles identified as pucks*/
                            obstaclesUnidentified,      /**< QList of obstacles being undefined*/
                            obstaclesTargets;           /**< QList of obstacles which are movement targets*/

    static Obstacle obstacleMe,                         /**< The own robot is an obstacle too*/
                    obstacleOpponent,                   /**< The foe robot is also an obstacle*/
                    obstacleDummy;                      /**< An obstacle object for testing*/

public:
    static QMutex mutexRobotME,                         /**< Mutex to ensure the secure multi-threaded access on my robot object */
                  mutexRobotOpponent,                   /**< Mutex to ensure the secure multi-threaded access on enemy robot object */
                  mutexRobotDummy,                      /**< Mutex to ensure the secure multi-threaded access on dummy robot object */
                  mutexTargets,                         /**< Mutex to ensure the secure multi-threaded access on target list */
                  mutexPucks,                           /**< Mutex to ensure the secure multi-threaded access on puck list */
                  mutexPoles,                           /**< Mutex to ensure the secure multi-threaded access on pole list */
                  mutexUnidentified,                    /**< Mutex to ensure the secure multi-threaded access on unidentified list */
                  mutexDisableEmergency,                /**< Mutex to ensure the secure multi-threaded access on disable emergency variable*/
                  mutexSimulationDetected,              /**< Mutex to ensure the secure multi-threaded access on simulation detected variable*/
                  mutexTeamColor,                       /**< Mutex to ensure the secure multi-threaded access on team color enum*/
                  mutexPuckInFork,                      /**< Mutex to ensure the secure multi-threaded access on PuckInFork*/
                  mutexPointerToPathPlanner,            /**< Mutex to ensure the secure multi-threaded access on Pointer*/
                  mutexTargetNearEnemy;                 /**< Mutex to ensure the secure multi-threaded access on TargetNearEnemy*/

    static bool disableEmergency;                       /**< Boolean if the emergency signal should be considered or not */
    static bool simulationDetected;                     /**< Boolean if a simulation is detected or not */
    static bool puckInFork;                             /**< Boolean if a Puck is in the fork */
    static bool targetNearEnemy;                        /**< Boolean if the current target is nex to the enemy robot */

    static CamColor teamColor;                          /**< Enumeration of team colors <blue>,<yellow>*/

    /**
     * @brief setActualColor , sets the current team color
     * @param[in] color (TeamColor enumeration)
     */
    static void setActualColor(const TeamColor color);

    /**
     * @brief setProbableColor
     * @param[in] color
     */
    static void setProbableColor(const CamColor color);

    /**
     * @brief getTeamColor
     * @return
     */
    static CamColor getTeamColor();

    /**
     * @brief getObstacle return the obstacleList of given type
     * @param[in] type (ObstacleType)
     * @return a QList of Obstacle for given type
     */
    static QList<Obstacle> getObstacle(const ObstacleType   &type);

    /**
     * @brief getObstacle return the obstacleList for given type and color
     * @param[in] type (ObstacleType)
     * @param[in] color (ObstacleColor)
     * @return a QList of Obstacle for given type and color
     */
    static QList<Obstacle> getObstacle(const ObstacleType   &type,
                                       const ObstacleColor  &color);

    /**
     * @brief getObstacle return the obstacleList for given type and color
     * @param[in] type (ObstacleType)
     * @param[in] status (ObstacleStatus)
     * @return a QList of Obstacle for given type and status
     */
    static QList<Obstacle> getObstacle(const ObstacleType   &type,
                                       const ObstacleStatus &status);

    /**
     * @brief getObstacle return the obstacleList of given type, color and status
     * @param[in] type (ObstacleType)
     * @param[in] color (ObstacleColor)
     * @param[in] status (ObstacleStatus)
     * @return a QList of Obstacle for given type, color and status.
     */
    static QList<Obstacle> getObstacle(const ObstacleType   &type,
                                       const ObstacleColor  &color,
                                       const ObstacleStatus &status);
    /**
     * @brief setObstacle will write a QList to merging process
     * @param[in] list (QList of Obstacle)
     * @return true if successful
     */
    static bool setObstacle(const QList<Obstacle> &list);

    /**
     * @brief setObstacle
     * @param[in] list (QList of Obstacle)
     * @param[in] currentRoboPos
     * @return true if successful
     */
    static bool setObstacle(const QList<Obstacle> &list, const Position &currentRoboPos);

    /**
     * @brief setObstacle will write a single value to merging process
     * @param[in] value
     * @return true if successful
     */
    static bool setObstacle(const Obstacle &value);

    /**
     * @brief setObstacle will write a single value to merging process
     * @param[in] value
     * @param[in] currentRoboPos
     * @return true if successful
     */
    static bool setObstacle(const Obstacle &value, const Position &currentRoboPos);

    /**
     * @brief getRobotPosition will return a Position struct with the current XYR values of a robot
     * @param[in] type (OPPONENT or ME)
     * @return Position of the desired robot
     */
    static Position getRobotPosition(ObstacleType type = ObstacleType::ME);

    /**
     * @brief deleteObstacle
     * @param[in] type (ME, DUMMY, OPPONENT)
     * @return
     */
    static bool deleteObstacle(ObstacleType type = ObstacleType::DUMMY);

    /**
     * @brief clearTargets will remove all target waypoints
     */
    static void clearTargets ( void );

    /**
     * @brief getFirstTarget will return the first targetpoint
     * @return an Obstacle being the first target point of target-list
     */
    static Obstacle getFirstTarget( void );

    /**
     * @brief deleteFirstTarget will delete the first item in targetliste
     */
    static void deleteFirstTarget( void);

    /**
     * @brief getDisableEmergency getter for disableEmergency
     * @return a boolen if emergency is enabled
     */
    static bool getDisableEmergency();

    /**
     * @brief setDisableEmergency setter for disableEmergency
     * @param[in] value (boolean)
     */
    static void setDisableEmergency(bool value);

    /**
     * @brief getSimulationDetected getter for simulationDetected
     * @return a boolen if emergency is enabled
     */
    static bool getSimulationDetected();

    /**
     * @brief setSimulationDetected setter for simulationDetected
     * @param[in] value (boolean)
     */
    static void setSimulationDetected(bool value);

    /**
     * @brief getPuckInFork
     * @return
     */
    static bool isPuckInFork();

    /**
     * @brief setPuckInFork
     * @param[in] value (bool)
     */
    static void setPuckInFork(bool value);

    /**
     * @brief setPointerToPathPlanner
     * @param[in] value (PathPlanning*)
     */
    static void setPointerToPathPlanner(PathPlanning *value);

    /**
     * @brief getPointerToPathPlanner
     * @return PathPlanning*
     */
    static PathPlanning* getPointerToPathPlanner();

    /**
     * @brief getTargetNearEnemy
     * @return true if the target is near the enemy robot
     */
    static bool getTargetNearEnemy();

    /**
     * @brief setTargetNearEnemy
     * @param[in] value (bool)
     */
    static void setTargetNearEnemy(bool value);

private:
    /**
     * @brief organizeObstacles is responsible for merging different obstacles
     * @param[in] constObstacle (Obstacle) will be stored in representing list<Obstacle>
     * @param[in] currentRoboPos (Position)
     * @return true if successful
     */
    static bool organizeObstacles( const Obstacle &constObstacle, const Position &currentRoboPos);

    /**
     * @brief MapData::compareObstacleTimes
     * @param[in] i Obstacle 1
     * @param[in] j Obstacle 2
     * @return
     */
    static bool compareObstacleTimestamps(Obstacle i, Obstacle j);

    /**
     * @brief getListByType will return a QList of Obstacles for given ObstacleType
     * @param[in] type (ObstacleType)
     * @return a QList of Obstacle for given ObstacleType
     */
    static QList<Obstacle> getListByType( const ObstacleType &type);

    /**
     * @brief cleanup functions to tidy up the MapData
     * @param[in] currentRoboPos (Position)
     * @param[in] enemyRobotPosition (Position)
     */
    static void cleanup(const Position &currentRoboPos, const Position &enemyRobotPosition);

    /**
     * @brief checkForEnemyNearTraget is verifiying if the enemy position is close to the target
     * @param[in] enemyRobotPosition (Position)
     */
    static void checkForEnemyNearTraget(const Position &enemyRobotPosition);
};

#endif // MAPDATA_H
