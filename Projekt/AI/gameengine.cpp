#include "gameengine.h"

#include <QDebug>
#include <QThread>

#include <referee.h>

#include "Data/define.h"
#include "AI/game.h"
#include "AI/pathplanning.h"
#include "Data/mapdata.h"
#include "Sensor/cam.h"
#include "Structs/position.h"
#include "Actor/actorhighlevel.h"

GameEngine::GameEngine()
{
    // Referee erzeugen und einstellen
    referee = new Referee(config::teamID);
    referee->setVerbose(config::refVerbose);

    // the referee is connected
    connect(referee, SIGNAL(connected()),
            this, SLOT(slotRefConnected()));

    // the connection failed
    connect(referee, SIGNAL(connectFailed()),
            this, SLOT(slotRefConnectFailed()));

    // the referee is now disconnected
    connect(referee, SIGNAL(disconnected()),
            this, SLOT(slotRefDisconnected()));

    // we may now detect our position
    connect(referee, SIGNAL(detectionStart()),
            this, SLOT(slotRefDetectionStart()));

    // the detection phase is over and we receive the correct color
    connect(referee, SIGNAL(trueColorOfTeam(TeamColor)),
            this, SLOT(slotRefTrueColorOfTeam(TeamColor)));

    // the game starts and we can start to put pucks into the goal zone
    connect(referee, SIGNAL(gameStart()),
            this, SLOT(slotRefGameStart()));

    // emergency signal -> we have to stop!!!!
    connect(referee, SIGNAL(stopMovement()),
            this, SLOT(slotRefStopMovement()));

    // the game is now over and we have to stop the robot
    connect(referee, SIGNAL(gameOver()),
            this, SLOT(slotRefGameOver()));

    // Alive and EgoPos Timers
    timerAlive = new QTimer();
    timerAlive->setInterval(config::periodAlive);
    connect(timerAlive, SIGNAL(timeout()), this, SLOT(slotTimerAlive()));

    timerEgoPos = new QTimer();
    timerEgoPos->setInterval(config::periodEgoPos);
    connect(timerEgoPos, SIGNAL(timeout()), this, SLOT(slotTimerEgoPos()));

    timerTillAnnoyEnemy = new QTimer();
    timerTillAnnoyEnemy->setInterval(config::periodTillAnnoy);
    connect(timerTillAnnoyEnemy,SIGNAL(timeout()),this,SLOT(slotTimerAnnoy()));
}

GameEngine::~GameEngine()
{
    timerAlive->stop();
    timerEgoPos->stop();
    timerTillAnnoyEnemy->stop();
    delete timerAlive;
    delete timerEgoPos;
    delete timerTillAnnoyEnemy;
    delete referee;
}

void GameEngine::startGameEngine()
{
    state = INIT;

    // connect to the referee server
    referee->connectToServer(config::refIP, config::refPort);

    // Timer starten. Wenn die Verbindung noch nicht steht, ist das nicht so schlimm.
    timerAlive->start();
    timerEgoPos->start();


    qDebug() << "Game Engine gestartet";
}

// Dieser Slot soll von der Orientation aufgerufen werden, sobald unsere Seite feststeht.
void GameEngine::slotDetectionFinished(CamColor color)
{
    if(state != DETECTION){
        qWarning() << "Game Engine is not in Detection state. This might not be the right time to tell our color. Will do it anyways now.";
    }

    qDebug() << "Sending our color now: " << static_cast<int>(color);

    switch(color){
    case CamColor::NONE:
    {
    if (config::enableDebugSensorHighLevel)
        qDebug() << "Wait for Color";
    }
        break;
    case CamColor::BLUE:
    {
        referee->tellTeamColor(TeamColor::blue);
    }
        break;
    case CamColor::YELLOW:
    {
        referee->tellTeamColor(TeamColor::yellow);
    }
    default:
    {
        ///\todo: Was tun wenn wir keine Frabe erkennen?
        if (config::enableDebugSensorHighLevel)
            qWarning() << "Unbekannte tatsächliche Teamfarbe: " << static_cast<int>(color);
    }
        break;
    }

    referee->reportReady();
    state = WAIT_FOR_GAME;
}

void GameEngine::slotReportGoal()
{
    if(referee && referee->isConnected())
        referee->reportGoal();
}

// Wird periodisch aufgerufen um einen Heartbeat an den Refere Server zu senden
void GameEngine::slotTimerAlive()
{
    if(referee && referee->isConnected())
        referee->sendAlive();
}

// Wird periodisch aufgerufen um dem Refere Server die eigene Position mitzuteilen
void GameEngine::slotTimerEgoPos()
{
    Position robotPos = MapData::getRobotPosition(ObstacleType::ME);

    // Wenn wir Gelb sind, müssen wir unser Koordinatensystem drehen
    if(MapData::getTeamColor() == CamColor::YELLOW) {
        robotPos.x(3 - robotPos.x());
        robotPos.y(5 - robotPos.y());
    }

    if(referee && referee->isConnected())
        referee->tellEgoPos(robotPos.y(), robotPos.x());

}

void GameEngine::slotTimerAnnoy()
{
    qDebug()<<"TOGGLE TO ANNOY ENEMY IN GAME";
    Q_EMIT signalAnnoyFoe();
}

void GameEngine::slotRefConnected()
{
    qDebug() << "Referee is now connected";

    if(state == INIT){
        state = WAIT_FOR_DETECTION;
        referee->reportReady(); // Report that we are ready for it and wait for detection phase to begin
    } else {
        qWarning() << "Referee connection established, but the Game Engine state was not INIT. doing nothing.";
    }
}

void GameEngine::slotRefConnectFailed()
{
    qWarning() << "Connecting to Referee failed. NOT restarting the Game Engine. Doing nothing.";
}

void GameEngine::slotRefDisconnected()
{
    qWarning() << "Referee connection lost. Restarting the Game Engine";
    startGameEngine();
}

void GameEngine::slotRefDetectionStart()
{
    timerTillAnnoyEnemy->start();
    qDebug()<<"START TIMER TO ANNOY ENEMY";
    qDebug() << "Detection phase has begun";
    state = DETECTION;

    // Start detection algorithms now
    Q_EMIT signalStartDetection(true); // Alles noetige sollte mit diesem Signal ausgeloest werden
}

void GameEngine::slotRefTrueColorOfTeam(TeamColor color)
{
    qDebug() << "Received our actual team color: "
             << ((color==TeamColor::blue)?
                     "Blue"
                   :((color==TeamColor::yellow)?
                         "Yellow"
                       :"Unknown Color"));

    // Tell the Map our actual color here
    MapData::setActualColor(color);
}

void GameEngine::slotRefGameStart()
{
    qDebug() << "Game phase has begun";
    if(state != WAIT_FOR_GAME){
        qDebug() << " The Game Engine did not expect the game to start now. Nothing we can do about that, though.";
    }
    state = GAME;

    // Start game algorithms now
    Q_EMIT signalStartGame(); // Alles noetige sollte mit diesem Signal ausgeloest werden
}

void GameEngine::slotRefStopMovement()
{
    qWarning() << "Referee commands immediate stop";
    state = STOP;
    PathPlanning::setEnabled(false);
    MapData::clearTargets();
    MapData::setDisableEmergency(false);
    Q_EMIT signalEmergencyStopEnabled(true);
}

void GameEngine::slotRefGameOver()
{
    qDebug() << "The game is over. Stopping now";
    if(state != WAIT_FOR_GAME){
        qDebug() << " The Game Engine did not expect the game to end. If this happens, we really fucked up.";
    }
    state = STOP;
    ActorHighLevel::setState(StatePathProcessing::STOP);
    PathPlanning::setEnabled(false);
    MapData::clearTargets();
    MapData::setDisableEmergency(false);

    Q_EMIT signalEmergencyStopEnabled(true);
}



