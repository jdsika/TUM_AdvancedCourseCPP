#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QList>
#include <QPair>

#include <atomic>

#include "Data/mapdata.h"
#include "Structs/position.h"


class GameEngine;
class Position;
class MapData;
class QElapsedTimer;

class Game : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Game constructor child object of gameEngine
     * @param gameEngine
     */
    Game(GameEngine *gameEngine);

    /**
     * @brief Game destructor
     */
    ~Game();

    /**
     * @brief The GameState enum, internal state machine which represents the game logic
     */
    enum GameState {
        SELECT_PUCK,
        DRIVE_TO_PUCK,
        GATHERING_PUCK,
        SELECT_GOAL_SLOT,
        DRIVE_TO_GOAL,
        DRIVE_TO_DUMP,
        ANNOY_FOE,
        WAIT_FOR_RELEASE
    };

    /**
     * @brief The SubGameStateAnnoy enum : This enum represents sub-state machine in the annoy foe game state. For cyclic generation of motion
     * targets in own goal area.
     */
    enum SubGameStateAnnoy {
        DRIVE_RIGHT,
        DRIVE_LEFT,
    };

    /**
     * @brief The HandleEnemyPuckState enum, represents the sub-state machine for handling pucks with foes color.
     */
    enum HandleEnemyPuckState {
        FIND_SLOT,
        DRIVE_2_SLOT,
        RELEASE
    };

Q_SIGNALS:
    /**
     * @brief signalReportGoal, signal for communication with refree object.
     */
    void signalReportGoal();

    /**
     * @brief signalPuckRelease, signal to actorHighlevel to start the puck release, without pathplanning
     */
    void signalPuckRelease();

    /**
     * @brief signalStartColorDetectAI, signal to cam-object to start the color-recognition for determination of the pucks color.
     */
    void signalStartColorDetectAI();

    /**
     * @brief signalPushAndRelease, signal to actorHighlevel to move forwards and backwards a defined distance to push the pucks of foes on the boarder between some poles.
     */
    void signalPushAndRelease(double releasePuckDistance);

    /**
     * @brief signalGatherPuck, signal to actorHighlevel to start the puck gathering => move straight forward without pathplanning.
     */
    void signalGatherPuck();


public Q_SLOTS:
    /**
     * @brief slotStartGame, slot to listen to the refree signal, when the normal game starts.
     */
    void slotStartGame();

    /**
     * @brief slotColorDetect, slot and implementation of the gathering-puck state and is connected with the cam object for color recognition.
     * @param[in] color
     */
    void slotColorDetect(CamColor color);

    /**
     * @brief slotActorHighLevelIsDoneWithPuck, connects to the actorHighLevel signal if the releasePuck, gatheringPuck or pushAndReleasePuck motion is done.
     */
    void slotActorHighLevelIsDoneWithPuck();

    /**
     * @brief quit method to terminate the internal game-state loop
     */
    void quit();

    /**
     * @brief run cyclic method which represents the game logic.
     */
    void run();

    /**
     * @brief slotAnnoyFoe switch the game state to annoy foe
     */
    void slotAnnoyFoe();

private:
    std::atomic_bool quitting; /**< bool if the state-machine should be terminated*/
    std::atomic_bool isGameStarted; /**< bool if the game was started by refree signal*/
    GameEngine *gameEngine; /**< pointer to the parent object*/
    GameState state;    /**< current state of the internal state-machine*/
    SubGameStateAnnoy subAnnoyState;    /**< current state of the internal sub-state-machine for annoying foes*/
    HandleEnemyPuckState handleEnemyPuckState; /**< current state of the internal sub-state-machine for handling foes pucks*/
    Obstacle currentPuck;   /**< current target puck, which is determined as best option to score*/
    Obstacle lastTargetedPuck;
    int currentPuckPrio;
    int colorfail;

    /**
     * @brief updateBestPuckPosition, will determine the puck-list in MapData to get the newest puck-target
     */
    bool findAndSelectBestPuck();

    /**
     * @brief findBestPuck, will determine the best puck to score (min(dist(puck<->bot))).
     * @param[in] pucksList (List of obstacle):reference of a list of obstacle pucks to find the puck with shortest distance to the robot
     */
    void findBestPuck(QList<Obstacle> &pucksList);

    /**
     * @brief createTargetFromPuck, will create a motion target from given puck-position
     * @param[in] curPuck (Obstacle): reference of the best puck
     */
    void createTargetInfrontPuck();

    /**
     * @brief makeAGoal method to drive to foes goal and score with gathered puck
     */
    void driveToGoal();


    /**
     * @brief annoyFoeState, representation of the annoy-foe state, will drive to home base and clear our goal.
     */
    void annoyFoeState();

    /**
     * @brief getParkPosition will determine the puck-position in foes goal
     * @param[in] robPos (Position)-> current position of the rob
     * @return a position for current puck to create a target in foe's goal
     */
    Position getParkPosition(const Position &robPos);

    /**
     * @brief handleEnemyPuck, implementation of DRIVE_TO_DUMP-state.
     */
    void handleEnemyPuck();

    /**
     * @brief findDumpSlot, will evaluate which slot is the best for dumping gathered foe's puck.
     * @param[in] ownPosition (Position) of our robot.
     * @return the postion for current dump.
     */
    Position findDumpSlot(Position ownPosition);

    /**
     * @brief checkTargetPuckAvailable, will check if the selected puck is still available.
     * @return bool, if the selected puck is available
     */
    bool checkTargetPuckAvailable();

    bool isPuckAwayFromPole(Obstacle puck);

    bool isPuckAwayFromEnemy(Obstacle puck);

    void getPositionToTurnRoboToMiddlePoint(Position &robot);

    Position getTargetForPuckBeforePole();
    Position getTargetForPuck();
    void findBestGoalSlot();

    QElapsedTimer *timerLostPuck;
    QElapsedTimer *timerUpdateGoalSlot;

    QList<QPair<Position,double>> goalSlotCounterList;
};

#endif // GAME_H
