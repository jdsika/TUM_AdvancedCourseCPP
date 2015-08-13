#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <QObject>
#include <QTimer>

#include <referee.h> // For TeamColor

class Game;
enum class CamColor;

class GameEngine : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief GameEngine, constructor of the game engine. This class will communicate with the refree obj.
     */
    GameEngine();
    ~GameEngine();

    /**
     * @brief The StateNameEnum enum represents the current state, emited by refree signals.
     */
    enum StateNameEnum {INIT, WAIT_FOR_DETECTION, DETECTION, WAIT_FOR_GAME, GAME, STOP};

    /**
     * @brief getState
     * @return s the current internal state 
     */
    StateNameEnum getState() const { return state; }

    /**
     * @brief startGameEngine. This method will be invoked by robot-thread for starting the game engine.
     */
    void startGameEngine();

Q_SIGNALS:
    /**
     * @brief signalEmergencyStopEnabled, if this signal is emited, the actor low level will be stopped immediately.
     * @param[in] enableEmergencyStop (bool)
     */
    void signalEmergencyStopEnabled(bool enableEmergencyStop);

    /**
     * @brief signalStartGame. This signal will be emited to init the state-machine of the game-obj.
     */
    void signalStartGame();

    /**
     * @brief signalStartDetection will be emited to start the orientation state in sensor-highlevel.
     * @param[in] start (bool) if the detection should be started.
     */
    void signalStartDetection(bool startGameEngine);

    /**
     * @brief signalAnnoyFoe will be emited if 4:15 mins are finished to switch to annoy enemy.
     */
    void signalAnnoyFoe();
public Q_SLOTS:
    /**
     * @brief slotDetectionFinished will get the right color from angelina after 1:30 mins. => save it to our static map obj.
     * @param[in] color (CamColor)
     */
    void slotDetectionFinished(CamColor color);

    /**
     * @brief slotReportGoal, if we scored a goal-> tell it angelina.
     */
    void slotReportGoal();



private Q_SLOTS:
    /**
     * @brief slotTimerAlive, which is needed to send in defined time slots a keep alive signal to angelina to avoid disqualification.
     */
    void slotTimerAlive();

    /**
     * @brief slotTimerEgoPos, will tell angelina our determined position.
     */
    void slotTimerEgoPos();

    /**
     * @brief slotRefConnected to refree signal
     */
    void slotRefConnected();

    /**
     * @brief slotRefConnectFailed to refree signal
     */
    void slotRefConnectFailed();

    /**
     * @brief slotRefDisconnected to refree signal
     */
    void slotRefDisconnected();

    /**
     * @brief slotRefDetectionStart to refree signal
     */
    void slotRefDetectionStart();

    /**
     * @brief slotRefTrueColorOfTeam to refree signal
     */
    void slotRefTrueColorOfTeam(TeamColor color);

    /**
     * @brief slotRefGameStart to refree signal
     */
    void slotRefGameStart();

    /**
     * @brief slotRefStopMovement to refree signal
     */
    void slotRefStopMovement();

    /**
     * @brief slotRefGameOver to refree signal
     */
    void slotRefGameOver();

    /**
     * @brief slotTimerAnnoy until the timer will set the game state to annoy foe
     */
    void slotTimerAnnoy();

private:

    Referee *referee; /**< refree-obj*/

    QTimer *timerAlive,     /**< timer to tell angelina that our robot is still alive*/
           *timerEgoPos,    /**< timer to tell angelina were we are*/
           *timerTillAnnoyEnemy; /**< timer to switch to annoy enemy*/

    StateNameEnum state;    /**< current state of refree states*/
};

#endif // GAMEENGINE_H
