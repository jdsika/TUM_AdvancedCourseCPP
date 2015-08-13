#ifndef ACTORLOWLEVEL_H
#define ACTORLOWLEVEL_H

#include <QObject>

#include "Structs/position.h"

// Forward declare
namespace PlayerCc{
class Position2dProxy;
}

/**
 * @brief The ActorLowLevel class
 * Class for accessing the PlayerCc::Position2dProxy, as single point of access.
 *
 */
class ActorLowLevel : public QObject
{
    Q_OBJECT

public:
    /** @brief Default-Constructor */
    ActorLowLevel();
    /** @brief The Desturctor will clean the heap objects -> e.g: PositionProxy2D*/
    ~ActorLowLevel();

    /** @brief oveRobot is the most important function for robots actors
        @param[in] speed (double) in m/s
        @param[in] turn (double) in rad/s*/
    void moveRobot(double speed, double turn);

public Q_SLOTS:
    /** @brief setRobotRemoteControllParams will set the class internal private
     *  attributes for velocity and turnrate and is used by the remote controll of the GUI
     *  @param[in] vel (double): Velocity of robot in m/s
     *  @param[in] deg (double): Angle in degree to turn the robot */
    void setRobotRemoteControllParams(double velocity, double turnangle);

    /**
     * @brief setRobotControllParams will set the new velocity commands from controller.
     * @param[in] velocity (double) in m/s
     * @param[in] turnangle (double) in rad/s
     */
    void setRobotControllParams(double velocity, double turnangle);

    /**
     * @brief slotEmergencyStopEnabled if an emergency occures, the velocity is set to zero.
     * @param[in] isEmergency (bool) send by sensors.
     */
    void slotEmergencyStopEnabled(bool isEmergency);


    /**
     * @brief setOdometry will set the odometry via the player proxy
     * @param[in] Position struct with x(double) position in x direction in m,
     * y (double) position in y direction in m, orientation (double) in radian
     */
    void setOdometry(Position currentPosition);

private:
    PlayerCc::Position2dProxy*  positionProxy; /**< positionProxy of the player stage => initialised on heap*/
    double previousTurnRate; /**< internal param to save the previous turn rate of remote control with collision avoidance*/
    double previousVelocity; /**< internal param to sage the previous velocity of remote control with collision avoidance*/
    double tempVelocity; /**< used for internal triangle swap*/
    double tempTurnRate; /**< used for internal triangle swap*/

    bool moveForward; /**< Check if driving forward is allowed*/
    bool isRefereeEmergency; /**< true if the game engine killed the path planning*/
};

#endif // ACTORLOWLEVEL_H
