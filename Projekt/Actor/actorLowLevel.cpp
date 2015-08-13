#include "actorLowLevel.h"

#include "Data/define.h"
#include "Data/mapdata.h"
#include "Main/player.h"

#include "AI/pathplanning.h"

#include <QDebug>
#include <QTime>
#include <QThread>

#include <libplayerc++/playerc++.h>

ActorLowLevel::ActorLowLevel()
{
    previousTurnRate    = 0.0;
    previousVelocity    = 0.0;
    tempVelocity        = 0.0;
    tempTurnRate        = 0.0;
    moveForward         = true;
    isRefereeEmergency  = false;

    positionProxy = new PlayerCc::Position2dProxy(PlayerX::getInstance());

    positionProxy->SetMotorEnable(true);
}


ActorLowLevel::~ActorLowLevel()
{
    if (config::enableDebugActorLowLevel)
        qDebug() << "DELETE 'ActorLowLvl'";

    delete positionProxy;
}


void ActorLowLevel::moveRobot(double speed, double turn)
{
    if (config::enableDebugActorLowLevel)
        qDebug() << "MOVE ROBOT: " << QTime::currentTime();

    // NO LOCK REQUIRED
    // http://stackoverflow.com/questions/15051553/qt-signals-queuedconnection-and-directconnection
    positionProxy->SetSpeed(speed,turn);//PlayerCc::dtor(previousTurnRate));
}

//QSLOT for remote controll

void ActorLowLevel::setRobotRemoteControllParams( double velocity, double turnrate )
{   
    if (isRefereeEmergency)
        return;

    if (moveForward) {

        if ( !std::isnan(velocity)) {
            if ( velocity != 0.0)
                previousVelocity += velocity;
            else
                previousVelocity = 0.0;
        }

        if ( !std::isnan(turnrate)) {
            if ( turnrate != 0.0)
                previousTurnRate += turnrate;
            else
                previousTurnRate = 0.0;
        }
    }
    else {
        if ( !std::isnan(velocity)) {
            if ( velocity < 0.0)
                previousVelocity += velocity;
            else
                previousVelocity = 0.0;
        }

        if ( !std::isnan(turnrate)) {
            if ( turnrate != 0.0)
                previousTurnRate += turnrate;
            else
                previousTurnRate = 0.0;
        }
    }

    if (config::enableDebugActorLowLevel)
        qDebug() << "CARLO: " << "Recived Vals: "<<velocity<<"m/s "<<turnrate<<"rad/s";

    this->moveRobot(previousVelocity,previousTurnRate);
}

void ActorLowLevel::setRobotControllParams(double velocity, double turnangle)
{
    if (config::enableDebugActorLowLevel)
        qDebug()<< " " << "Recived Vals: "<<velocity<<"m/s "<<turnangle<<"rad/s";

    this->moveRobot(velocity,turnangle);
}

//QSLOT for emergency detected by SensorHighLevel

void ActorLowLevel::slotEmergencyStopEnabled(bool isEmergency)
{
    if (config::enableDebugActorLowLevel)
        qDebug()<< "CARLO: EMERGENCY";

    if (!MapData::getDisableEmergency())
    {
        if (PathPlanning::getEnabled())
        {
            // stop moving forward, if an emergency is detected
            moveForward = !isEmergency;

            if ( isEmergency) {
                // remeber the "pre-emergency" speed
                tempVelocity = previousVelocity;
                // now set the speed to 0.0, but don't alter the turnrate
                this->setRobotRemoteControllParams(0.0, std::numeric_limits<double>::quiet_NaN());
            }
            else {
                // the emergency has passed, go back to old speed
                // only if the speed has still the same direction
                if (previousVelocity == 0.0)
                    previousVelocity = tempVelocity;
                this->setRobotRemoteControllParams(std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN());
            }
        }
        else
        {
            // stop moving forward, if an emergency is detected
            isRefereeEmergency = isEmergency;

            this->setRobotRemoteControllParams(0.0, 0.0);
        }
    }
}


void ActorLowLevel::setOdometry(Position currentPosition)
{
    qDebug() << "Odometrie gesetzt!";
    positionProxy->SetOdometry(currentPosition.x(),
                               currentPosition.y(),
                               currentPosition.rot());
}



