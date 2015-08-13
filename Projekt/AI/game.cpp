#include "game.h"

#include <QDebug>
#include <QCoreApplication>
#include <QThread>
#include <QElapsedTimer>

#include "AI/gameengine.h"
#include "AI/pathplanning.h"
#include "Data/mapdata.h"
#include "Sensor/cam.h"
#include "Sensor/sensorLowLevel.h"
#include "Data/define.h"

Game::Game(GameEngine *gameEngine) :
    gameEngine(gameEngine)
{
    quitting = false;
    handleEnemyPuckState = FIND_SLOT;
    subAnnoyState = DRIVE_RIGHT;
    currentPuck = Obstacle();
    lastTargetedPuck = Obstacle(qMakePair(9999, 9999)); // sehr weit weg, damit jeder puck besser ist
    currentPuckPrio = -1;
    isGameStarted = false;
    timerLostPuck = new QElapsedTimer;
    timerUpdateGoalSlot = new QElapsedTimer;

    goalSlotCounterList = QList<QPair<Position, double>>() << qMakePair(config::gameGoalSlotL, 0.0)
                                                           << qMakePair(config::gameGoalSlotM, 0.0)
                                                           << qMakePair(config::gameGoalSlotR, 0.0)
                                                           << qMakePair(config::gameGoalSlotOutsideLeft, 0.0)
                                                           << qMakePair(config::gameGoalSlotOutsideRight, 0.0);
}

Game::~Game()
{
    delete timerLostPuck;
    delete timerUpdateGoalSlot;
}

void Game::quit(){
    quitting = true;
}

void Game::slotStartGame()
{
    PathPlanning::setAvoidRestOfField(false); // In der Spielphase darf die Pfadplanung das gesamte Spielfeld beplanen
    MapData::clearTargets(); // Lösche eventuelle unerreichte Parkposition-Targets wenn Spiel anfängt
    state = SELECT_PUCK;
    isGameStarted = true;
    if(config::enableDebugGame) {
        qDebug() << "START GAME - Statemachine set to SELECT_PUCK";
    }
}

void Game::run()
{
    while(!quitting){

        while(!isGameStarted){
            if(quitting) return;
            QThread::msleep(50);
            QCoreApplication::processEvents();
            QCoreApplication::sendPostedEvents();
        }

        switch(state){
        case SELECT_PUCK:
        {
            lastTargetedPuck = Obstacle(qMakePair(9999, 9999)); // sehr weit weg, damit jeder puck besser ist

            if(!findAndSelectBestPuck()) // Keine Pucks gefunden
            {
                state = ANNOY_FOE;
                qDebug() << "Keine Pucks gefunden. Gehe in ANNOY_FOE state.";
            }
            else // Aufnehmbaren Puck gefunden
            {
                createTargetInfrontPuck();
                state = DRIVE_TO_PUCK;
            }
        }
            break;

        case DRIVE_TO_PUCK:
        {
            if(MapData::getObstacle(ObstacleType::TARGET).isEmpty()) // --> Ziel erreicht
            {
                // Überprüfen, ob immernoch ein Puck in der /nähe/ der Aufnahmeposition ist
                if( MapData::getObstacle(ObstacleType::PUCK).contains(currentPuck) ) {
                    if(config::enableDebugGame)
                        qDebug() << "Signal gather puck to Actor";
                    Q_EMIT signalGatherPuck();
                    state = GATHERING_PUCK;

                    if(config::enableDebugGame)
                        qDebug() << "Statemachine set to GATHERING_PUCK because we reached the pre-gathering-target";
                }
                else // der puck ist verschwunden :(
                {
                    qDebug() << "Der Puck den wir aufnehmen wollten ist verschwunden :( Finde einen neuen mit SELECT_PUCK";
                    state = SELECT_PUCK;
                }

            }
            else // --> Ziel noch nicht erreicht
            {
                if(!findAndSelectBestPuck()) // Kein Puck gefunden!
                {
                    state = ANNOY_FOE;
                    qDebug() << "Keine Pucks gefunden. Gehe in ANNOY_FOE state.";
                }
                else // Aufnehmbaren Puck gefunden. Ist immer sehr wahrscheinlich der selbe den wir eh schon anfahren.
                {
                    createTargetInfrontPuck();
                }
            }
        }
            break;

        case GATHERING_PUCK: // is handled in slot: slotActorHighLevelIsDoneWithPuck
            break;

        case SELECT_GOAL_SLOT:
        {
            // reset counter for find goal position
            for(int i = 0; i < goalSlotCounterList.size(); ++i)
                goalSlotCounterList[i].second = 0.0;

            findBestGoalSlot();
            state = DRIVE_TO_GOAL;

            // find goal slot will only be updated every 10ms max
            timerUpdateGoalSlot->start();

            if(config::enableDebugGame)
                qDebug() << "Statemachine set to DRIVE_TO_GOAL";
        }
            break;

        case DRIVE_TO_GOAL:
        {      
            if(timerUpdateGoalSlot->isValid() && timerUpdateGoalSlot->elapsed() > 100)   // Puck nicht mehr in der Gabel
            {
                timerUpdateGoalSlot->start();

                // Solange Ziel nicht erreicht, nimm immer den besten Goalslot
                if(!MapData::getObstacle(ObstacleType::TARGET).isEmpty()){
                    findBestGoalSlot();
                }
            }

            this->driveToGoal();
        }
            break;

        case DRIVE_TO_DUMP:
        {
            // Ruft die handleEnemyPuck State Maschine auf, bis diese fertig ist
            handleEnemyPuck();
        }
            break;

        case WAIT_FOR_RELEASE: // is handled in slot: slotActorHighLevelIsDoneWithPuck
            break;

        case ANNOY_FOE:
        {
            if(findAndSelectBestPuck() == true)
            {
                // Es gibt (wieder) aufnehmbare Pucks!
                state = SELECT_PUCK;
                PathPlanning::setIgnorePucks(false);
                qDebug() << "Kehre aus ANNOY_FOE zurück zu SELECT_PUCK";
            }
            else // Keine akzeptablen Pucks gefunden
            {
                // Fahre in unserem Tor hin und her
                PathPlanning::setIgnorePucks(true);
                annoyFoeState();
            }
        }
            break;

        default:
        {
            qWarning() << "Unknown Game state " << static_cast<int>(state) << ", switching to SELECT_PUCK state";
            state = SELECT_PUCK;
        }
            break;

        } //end switch

        QCoreApplication::sendPostedEvents();
        QCoreApplication::processEvents();
        QThread::msleep(10);

    } // while(!quitting)
}

void Game::findBestGoalSlot()
{
    Position targetGoalSlot;
    Position myPosition = MapData::getRobotPosition(ObstacleType::ME);
    Position enemyPosition = MapData::getRobotPosition(ObstacleType::OPPONENT);

    // Anzahl Pucks in Tordritteln zählen
    int numPucksL=0, numPucksM=0, numPucksR=0;
    Q_FOREACH(Obstacle puck, MapData::getObstacle(ObstacleType::PUCK)){
        if(puck.getStatus() == ObstacleStatus::ISMOVING)
            continue; // Den Puck in der Gabel nicht im Tor mitzählen

        if (puck.isInSpecifiedArea(FieldArea::GOAL_ZONE_LEFT)){
            numPucksL++;
        }
        if (puck.isInSpecifiedArea(FieldArea::GOAL_ZONE_MID)){
            numPucksM++;
        }
        if (puck.isInSpecifiedArea(FieldArea::GOAL_ZONE_RIGHT)){
            numPucksR++;
        }
    }

    double distEnemyToSlotL = enemyPosition.getDistanceTo(config::gameGoalSlotL);
    double distEnemyToSlotM = enemyPosition.getDistanceTo(config::gameGoalSlotM);
    double distEnemyToSlotR = enemyPosition.getDistanceTo(config::gameGoalSlotR);

    // Wenn Gegner in linkem Goal Slot
    if ( distEnemyToSlotL < config::gameMinimumDistanceToEnemyRobot)
    {
        if(numPucksM < numPucksR){
            targetGoalSlot = config::gameGoalSlotM;
        }
        else{
            targetGoalSlot = config::gameGoalSlotR;
        }
    }
    // Mittlerer Slot
    else if (distEnemyToSlotM < config::gameMinimumDistanceToEnemyRobot)
    {
        if(numPucksL < numPucksR)
        {
            targetGoalSlot = config::gameGoalSlotL;
        }
        else{
            targetGoalSlot = config::gameGoalSlotR;
        }
    }
    // Rechter Slot
    else if (distEnemyToSlotR < config::gameMinimumDistanceToEnemyRobot)
    {
        if(numPucksL < numPucksM){
            targetGoalSlot = config::gameGoalSlotL;
        }
        else {
            targetGoalSlot = config::gameGoalSlotM;
        }
    }
    // Gegner beeinträchtigt keinen Slot
    else
    {
        // IF Robo links
        if (myPosition.x() < 1.0 / 3.0 * config::geoFieldWidth )
        {   // robot auf der linken seite
            if(numPucksL <= numPucksM){
                targetGoalSlot = config::gameGoalSlotL;

                if( numPucksL > numPucksR)
                    targetGoalSlot = config::gameGoalSlotR;
            }
            else {
                targetGoalSlot = config::gameGoalSlotM;

                if(numPucksM > numPucksR)
                    targetGoalSlot = config::gameGoalSlotR;
            }
        }
        else if (myPosition.x() >= 1.0 / 3.0 * config::geoFieldWidth &&
                 myPosition.x() <= 2.0 / 3.0 * config::geoFieldWidth)
        {   // robot in der mitte
            if(numPucksM <= numPucksL &&
               numPucksM <= numPucksR){
                targetGoalSlot = config::gameGoalSlotM;
            }
            else {
                if( numPucksL > numPucksR)
                    targetGoalSlot = config::gameGoalSlotR;
                else
                    targetGoalSlot = config::gameGoalSlotL;
            }
        }
        else { // robot rechts
            if(numPucksR <= numPucksM){
                targetGoalSlot = config::gameGoalSlotR;

                if( numPucksR > numPucksL)
                    targetGoalSlot = config::gameGoalSlotL;
            }
            else {
                targetGoalSlot = config::gameGoalSlotM;

                if(numPucksM > numPucksL)
                    targetGoalSlot = config::gameGoalSlotL;
            }

        }
    }

    // Überprüfen ob der Gegner versucht das Tor zu blockieren
    // In diesem Fall den Puck bloß bis ins Feld schieben
    QList<Obstacle> enemies = MapData::getObstacle(ObstacleType::OPPONENT);
    if(!enemies.isEmpty() &&
        enemies.first().isInSpecifiedArea(FieldArea::ENEMY_GOAL_INFLUENCE))
    {
        //choose right or left spot
        double distToRight = myPosition.getDistanceTo(config::gameGoalSlotOutsideRight);
        double distToLeft = myPosition.getDistanceTo(config::gameGoalSlotOutsideLeft);

        if( distToLeft > distToRight) {
            if(enemyPosition.isConsimilarTo(config::gameGoalSlotOutsideRight, 2 * config::SENSOR_RADIUS_ROBOT))
                targetGoalSlot = config::gameGoalSlotOutsideLeft;
            else
                targetGoalSlot = config::gameGoalSlotOutsideRight;
        } else {
            if(enemyPosition.isConsimilarTo(config::gameGoalSlotOutsideLeft, 2 * config::SENSOR_RADIUS_ROBOT))
                targetGoalSlot = config::gameGoalSlotOutsideRight;
            else
                targetGoalSlot = config::gameGoalSlotOutsideLeft;
        }
    }

    /******************************************/
    /*          avoide fast slot jumping    ***/
    /******************************************/
    // decicion making for changes in select goal
    double overallCount = 1.0;
    int slotIndex = -1;
    for(int i = 0; i < goalSlotCounterList.size(); ++i) {
        overallCount += goalSlotCounterList.at(i).second;
        if(goalSlotCounterList.at(i).first == targetGoalSlot)
            slotIndex = i;
    }
    // der neue wert geht mit 10% der gesamtanzahl an iterationen ein
    goalSlotCounterList[slotIndex].second += 0.1 * overallCount;

    double highestValue = 0.0;  

    for(int i = 0; i < goalSlotCounterList.size(); ++i) {
        if(goalSlotCounterList.at(i).second > highestValue) {
            highestValue = goalSlotCounterList.at(i).second;
            targetGoalSlot = goalSlotCounterList.at(i).first;
            slotIndex = i;
        }
    }

    // Target tatsächlich in Map setzen
    MapData::clearTargets();
    Obstacle goalTarget = Obstacle(targetGoalSlot,
                                   ObstacleColor::UNIDENTIFIED,
                                   ObstacleType::TARGET,
                                   ObstacleStatus::UNBLOCKED);

    MapData::setObstacle(goalTarget);
}

// Funktion setzt die Position und Priorität des bestmöglichsten Pucks
// Wenn dieser Puck nicht gefunden wurde, wird FALSE zurück gegeben
bool Game::findAndSelectBestPuck()
{
    QList<Obstacle> allUnblockedPucks = MapData::getObstacle(ObstacleType::PUCK,
                                                    ObstacleStatus::UNBLOCKED);

    // Entfernen von Pucks außerhalb des Area
    for (int i = allUnblockedPucks.size(); i > 0 ; i--)
    {
        if(allUnblockedPucks.at(i-1).isInSpecifiedArea(FieldArea::OUT_OF_AREA))
            allUnblockedPucks.removeAt(i-1);
    }

    // Für den Fall, dass es keine UNBLOCKED Pucks gibt
    if(allUnblockedPucks.isEmpty())
        return false;

    QList<int> kostenListe = QList<int>();
    currentPuckPrio = -1;

    for(int i = 0; i < allUnblockedPucks.size(); i++)
    {
        int kosten = 0;

        // Puck ist weit weg von einem Pole oder nah an einem Puck      (1:2)
        if (isPuckAwayFromPole(allUnblockedPucks.at(i))){
            kosten += 1;
        } else {
            kosten += 2;
        }

        ///\todo eigentlich würde ich hier lieber etwas besser differenzieren
        // pucks in meinem areal müssen als erstes entfernt werden! (1:3)
        if (allUnblockedPucks.at(i).isInSpecifiedArea(FieldArea::MY_GOAL_AREA)) {
            kosten += 1;
        } else if (allUnblockedPucks.at(i).isInSpecifiedArea(FieldArea::MY_AREA)) {
            kosten += 1;
        } else if (allUnblockedPucks.at(i).isInSpecifiedArea(FieldArea::NEUTRAL_AREA)) {
            kosten += 1;
        } else {
            kosten += 3; // alle anderen Fälle haben geringere Priorität
        }

        // Puck ist weit weg vom Gegner oder nah am Gegner              (1:5)
        if (isPuckAwayFromEnemy(allUnblockedPucks.at(i))) {
            kosten += 1;
        } else {
            kosten += 5;
        }

        // Überprüft ob es sich um die beste Priorität handelt
        if (kosten < currentPuckPrio ||
                currentPuckPrio < 0)
            currentPuckPrio = kosten;

        kostenListe.append(kosten);
    }

    QList<Obstacle> puckBestPrio;

    // Liste erzeugen mit den Pucks, die die niedrigste Priorität haben
    for (int i = 0; i < allUnblockedPucks.size(); i++)
    {
        if (kostenListe.at(i) == currentPuckPrio)
            puckBestPrio.append(allUnblockedPucks.at(i));
    }

    // Zur Sicherheit testen ob noch Pucks in der Liste sind
    if(puckBestPrio.isEmpty())
        return false;


    // Puck finden, der zusätzlich zur niedrigsten Priorität am nächsten an unserem Roboter ist
    double distance = lastTargetedPuck.getDistanceTo(MapData::getRobotPosition());
    currentPuck = lastTargetedPuck;

    Q_FOREACH(Obstacle puckToUse, puckBestPrio)
    {
        double currentDistance = puckToUse.getDistanceTo(MapData::getRobotPosition());
        if ( (config::gameWievielBesserMussEinPuckSeinUmDasZielZuWechseln + 1) * currentDistance < distance)
        {
            currentPuck = puckToUse;
            distance = currentDistance;
        } // Wenn die Distanz nie deutlich kleiner ist, wird currentPuck nie aktualisiert
    }

    // Diesen Puck zur Überprüfung mit den späteren Möglichen Pucks abspeichern
    lastTargetedPuck = currentPuck;

    return true;
}

bool Game::isPuckAwayFromPole(Obstacle puck)
{
    Q_FOREACH(Obstacle pole, MapData::getObstacle(ObstacleType::POLE))
    {
        if (puck.getDistanceTo(pole) < config::puckIsCloseToPoleDistance)
            return false;
    }
    return true;
}

bool Game::isPuckAwayFromEnemy(Obstacle puck){
    Q_FOREACH(Obstacle pole, MapData::getObstacle(ObstacleType::OPPONENT))
    {
        if (puck.getDistanceTo(pole) < config::puckIsCloseToPoleDistance)
            return false;
    }
    return true;
}

/**
 * @brief Game::createTargetFromPuck: Create motion target for given puck. (Direction: goal of foe)
 *
 * @param curPuck
 */
void Game::createTargetInfrontPuck()
{
    if (currentPuckPrio < 0 || currentPuckPrio > 10)
    {
        qDebug() << "Error: currentPuckPrio falsch gesetzt";
        state = SELECT_PUCK;
        return;
    }

    // Wenn alle Pucks nah am Gegner
    if(currentPuckPrio >= 7)
    {
        Position zurMitteAusrichten = MapData::getRobotPosition();
        getPositionToTurnRoboToMiddlePoint(zurMitteAusrichten);

        MapData::clearTargets();
        MapData::setObstacle(Obstacle(zurMitteAusrichten,ObstacleType::TARGET));

        state = SELECT_PUCK;
        return;
    }

    Position targetPos;

    // Wenn der Puck nahe an einem Pole ist
    if(currentPuckPrio == 4 ||
            currentPuckPrio == 6)
        targetPos = getTargetForPuckBeforePole();

    // Wenn der Puck im Feld steht
    if(currentPuckPrio == 3 ||
            currentPuckPrio == 5)
        targetPos = getTargetForPuck();

    Obstacle newTarget = Obstacle(targetPos,ObstacleType::TARGET);

    // Wenn es bereits ein Target gibt, und dieses ähnlich ist, dann verhindere zu krasses wackeln per Tiefpassfilter
    if(!MapData::getObstacle(ObstacleType::TARGET).isEmpty()) {
        Position newP = newTarget.getPosition();
        Position oldP = MapData::getFirstTarget().getPosition();
        if(newP.isConsimilarTo(oldP, config::gameBisZuWelchemAbstandWirdZielwackelnGefiltert)) {
            double lpLow = config::gameZielwackelfilterTiefpassKoeffizient;
            double lpHigh = 1 - lpLow;
            double newX = lpHigh * oldP.x() + lpLow * newP.x();
            double newY = lpHigh * oldP.y() + lpLow * newP.y();
//            double newR = SensorLowLevel::angleWeightedAverage(oldP.rot(), lpHigh, newP.rot(), lpLow);
            double newR = atan2(currentPuck.getPosition().y() - newY, currentPuck.getPosition().x() - newX);
            newTarget.setPosition(Position(newX, newY, newR));
        }
    }

    // Update the target position
    MapData::clearTargets();
    MapData::setObstacle(newTarget);
}


void Game::slotActorHighLevelIsDoneWithPuck()
{
    if(state == GATHERING_PUCK) // Wir wollten einen Puck aufnehmen, das ist angeblich geschehen
    {
        if(!MapData::isPuckInFork()) // Puck ist nicht in der Gabel
        {
            if(config::enableDebugGame) {
                qDebug() << "Puck aufnehmen fehlgeschlagen - Statemachine set to SELECT_PUCK";
            }

            state = SELECT_PUCK;
            return;
        }
        else // Puck ist korrekt aufgenommen worden
        {
            if(config::enableDebugGame)
                qDebug() << "Puck ist korrekt aufgenommen worden";

            // Jetzt farbe identifizieren
            colorfail = 0;
            Q_EMIT signalStartColorDetectAI(); // Kommt zurück in slotColorDetect()
        }

    }
    else if (state == DRIVE_TO_DUMP) // Wir haben einen Puck aus dem Spiel heraus gefahren
    {
        handleEnemyPuckState = FIND_SLOT;
        state = SELECT_PUCK;

        if(config::enableDebugGame)
            qDebug() << "Statemachine set to SELECT_PUCK, because we finished dumping a puck";
    }
    else if (state == WAIT_FOR_RELEASE) // Wir haben einen Puck im gegnerischen Tor platziert
    {
        Q_EMIT signalReportGoal();
        state = SELECT_PUCK;
        if(config::enableDebugGame)
            qDebug()<<"GOAL! - Statemachine set to SELECT_PUCK";
    }

}

/**
 * @brief Game::driveToGoal drive to foes goal and score.
 * @return True if a goal was scored
 */
void Game::driveToGoal()
{
    if(!MapData::isPuckInFork()){
        // only start if invalid
        if(!timerLostPuck->isValid())
            timerLostPuck->start();
    }
    else {
         timerLostPuck->invalidate();
    }

    if(timerLostPuck->isValid() && timerLostPuck->elapsed() > 500)   // Puck nicht mehr in der Gabel
    {
        timerLostPuck->invalidate();

        if(config::enableDebugGame) {
            qDebug() << "Lost Puck - Statemachine set to SELECT_PUCK";
        }

        state = SELECT_PUCK;

        return;
    }


    if(MapData::getObstacle(ObstacleType::TARGET).isEmpty())
    {
        Position robPos = MapData::getRobotPosition(ObstacleType::ME);
        Position puckPos = this->getParkPosition(robPos);

        MapData::setObstacle(Obstacle(puckPos,
                                      ObstacleColor::ME,
                                      ObstacleType::PUCK,
                                      ObstacleStatus::BLOCKED )
                             );

        //if scored before: set current puck to blocked status
        currentPuck.setStatus(ObstacleStatus::BLOCKED);
        MapData::setObstacle(currentPuck);

        if(config::enableDebugGame){
             qDebug()<<"SET PUCK AS BLOCKED";
        }

        //drive back and turn
        //Carlo: wir wollen hier nicht zu weit zurück fahren!
        Q_EMIT signalPushAndRelease(0.0);

        timerUpdateGoalSlot->invalidate();

        state = WAIT_FOR_RELEASE;
        if(config::enableDebugGame)
            qDebug() << "Statemachine set to WAIT_FOR_RELEASE";

    }
}

void Game::annoyFoeState()
{
    switch(subAnnoyState)
    {
    case(DRIVE_RIGHT):
    {
        if(MapData::getObstacle(ObstacleType::TARGET).isEmpty()) // Target reached
        {
            // Wenn der Gegner nicht nahe am Ziel ist, Ziel auf der anderen Seite setzen
            if(!MapData::getRobotPosition(ObstacleType::OPPONENT).isConsimilarTo(config::gameAnnoyPositionL, config::gameWieWeitMussDerGegnerVomZielEntferntSeinImAnnoyModus))
            {
                subAnnoyState = DRIVE_LEFT;
                MapData::clearTargets();
                MapData::setObstacle(Obstacle(config::gameAnnoyPositionL,
                                              ObstacleType::TARGET));
            }
        }
        else
        {
            // Wenn der Gegner nicht nahe am Ziel ist, Ziel weiterhin setzen
            if(!MapData::getRobotPosition(ObstacleType::OPPONENT).isConsimilarTo(config::gameAnnoyPositionR, config::gameWieWeitMussDerGegnerVomZielEntferntSeinImAnnoyModus))
            {
                MapData::clearTargets();
                MapData::setObstacle(Obstacle(config::gameAnnoyPositionR,
                                              ObstacleType::TARGET));
            }
        }
        break;
    }
    case(DRIVE_LEFT):
    {
        if(MapData::getObstacle(ObstacleType::TARGET).isEmpty()) // Target reached
        {
            // Wenn der Gegner nicht nahe am Ziel ist, Ziel auf der anderen Seite setzen
            if(!MapData::getRobotPosition(ObstacleType::OPPONENT).isConsimilarTo(config::gameAnnoyPositionR, config::gameWieWeitMussDerGegnerVomZielEntferntSeinImAnnoyModus))
            {
                subAnnoyState = DRIVE_RIGHT;
                MapData::clearTargets();
                MapData::setObstacle(Obstacle(config::gameAnnoyPositionR,
                                              ObstacleType::TARGET));
            }
        }
        else
        {
            // Wenn der Gegner nicht nahe am Ziel ist, Ziel weiterhin setzen
            if(!MapData::getRobotPosition(ObstacleType::OPPONENT).isConsimilarTo(config::gameAnnoyPositionL, config::gameWieWeitMussDerGegnerVomZielEntferntSeinImAnnoyModus))
            {
                MapData::clearTargets();
                MapData::setObstacle(Obstacle(config::gameAnnoyPositionL,
                                              ObstacleType::TARGET));
            }
        }
        break;
    }
    }
}

void Game::handleEnemyPuck()
{
    switch(handleEnemyPuckState){
    case FIND_SLOT:{
        if(config::enableDebugGame)
            qDebug() << "Looking for Spot to DUMP enemy Puck";
        Position dumpSlot = findDumpSlot(MapData::getRobotPosition(ObstacleType::ME));  // finde Slot Position
        MapData::setObstacle(Obstacle(dumpSlot,ObstacleType::TARGET));                  // lasse Roboter zum Slot fahren

        if(config::enableDebugGame)
            qDebug() << "Driving to Dumping Spot";

        handleEnemyPuckState = DRIVE_2_SLOT;
    }
        //        break; Hier nicht notwendig!
    case DRIVE_2_SLOT:{
        if(!MapData::isPuckInFork()) {
            if(!timerLostPuck->isValid())
                timerLostPuck->start();
        }
        else {
            timerLostPuck->invalidate();
        }

        if(timerLostPuck->isValid() && timerLostPuck->elapsed() > 500)   // Puck nicht mehr in der Gabel
        {
            timerLostPuck->invalidate();
            handleEnemyPuckState = FIND_SLOT;
            state = SELECT_PUCK;
            if(config::enableDebugGame)
                qDebug() << "Puck verloren! Statemachine set to SELECT_PUCK";
            return;
        }
        if(MapData::getObstacle(ObstacleType::TARGET).isEmpty())                        // Slot erreicht
        {
            if(config::enableDebugGame) {
                qDebug() << "Dumping Spot reached - Calling PushAndRelease()";
            }
            handleEnemyPuckState = RELEASE;

            // Carlo: beim dumpen müssen wir etwas weiter zurück setzen!
            Q_EMIT signalPushAndRelease(config::actorPushPuckDistance);
        }
    }
        break;
    case RELEASE:
        break;
    }
}


Position Game::findDumpSlot(Position ownPosition)
{
    QList<Position> slotList = QList<Position>();

    slotList << Position(0+config::geoRobotForkCenterDist,config::DUMP_SLOT_2_3,M_PI)
             << Position(0+config::geoRobotForkCenterDist,config::DUMP_SLOT_5_6,M_PI)
             << Position(config::geoFieldWidth - config::geoRobotForkCenterDist, config::DUMP_SLOT_2_3,0)
             << Position(config::geoFieldWidth - config::geoRobotForkCenterDist, config::DUMP_SLOT_5_6,0);
//             << Position(0.5*config::geoFieldWidth, 0 + config::geoRobotForkCenterDist, 3*M_PI/2)
//             << Position(0.5*config::geoFieldWidth, config::geoFieldHeight - config::geoRobotForkCenterDist, M_PI/2);

//    slot.append(Position(0+config::DIST_BETWEEN_LASER_AND_FORK_CENTER,config::DUMP_SLOT_3_4,M_PI));
//    slot.append( Position(0+config::DIST_BETWEEN_LASER_AND_FORK_CENTER,config::DUMP_SLOT_4_5,M_PI));
//    slot.append(Position(config::SENSOR_FIELDWIDTH_M - config::DIST_BETWEEN_LASER_AND_FORK_CENTER, config::DUMP_SLOT_3_4,0));
//    slot.append(Position(config::SENSOR_FIELDWIDTH_M - config::DIST_BETWEEN_LASER_AND_FORK_CENTER, config::DUMP_SLOT_4_5,0));

    // return at least 1 valid position
    Position dumpSlotPosition = slotList.first();
    Position enemyRobotPosition = MapData::getRobotPosition(ObstacleType::OPPONENT);

    double dist = std::numeric_limits<double>::infinity();

    for (int i = 0; i < slotList.length(); ++i)
    {
        double newDist = slotList.at(i).getDistanceTo(ownPosition);
        double distSlotToEnemy = slotList.at(i).getDistanceTo(enemyRobotPosition);

        if ( newDist < dist &&
             distSlotToEnemy > config::gameMinimumDistanceEnemyToDumpSlot)
        {
            dist = newDist;
            dumpSlotPosition = slotList.at(i);
        }
    }

    return dumpSlotPosition;
}

Position Game::getParkPosition(const Position &robPos)
{
    double dX = cos(robPos.rot())*config::geoRobotForkCenterDist;
    double dY = sin(robPos.rot())*config::geoRobotForkCenterDist;
    return Position(robPos.x()+dX,robPos.y()+dY);

}

bool Game::checkTargetPuckAvailable(){
    bool puckStillAvailable = false;
    // Überprüft, ob der Puck noch an seiner Stelle steht
    QList<Obstacle> pucks = MapData::getObstacle(ObstacleType::PUCK);
    for (int i = 0; i < pucks.size(); i++){
        if (currentPuck.getDistanceTo(pucks.at(i)) < config::TARGET_POLE_VARIANCE)
        {
            puckStillAvailable = true;
        }
    }
    return puckStillAvailable;
}

void Game::slotColorDetect(CamColor color)
{
    if(config::enableDebugGame)
        qDebug()<<"SLOT COLOR DETECT CALLED";


    switch(color){
    case CamColor::BLUE:
    {
        //own color
        if(MapData::getTeamColor()==CamColor::BLUE)
        {
            //puck aufnehmen
            currentPuck.setColor(ObstacleColor::ME);
            MapData::setObstacle(currentPuck);
            state = SELECT_GOAL_SLOT;

            if(config::enableDebugGame)
                qDebug() << "Statemachine set to SELECT_GOAL_SLOT because we recognized this puck as ours (blue)";
        }
        else{
            currentPuck.setColor(ObstacleColor::OPPONENT);
            MapData::setObstacle(currentPuck);
            handleEnemyPuckState = FIND_SLOT;
            state = DRIVE_TO_DUMP;

            if(config::enableDebugGame)
                qDebug() << "Statemachine set to DRIVE_TO_DUMP";
        }
    }
        break;
    case CamColor::YELLOW:
    {
        //own color
        if(MapData::getTeamColor()==CamColor::YELLOW){
            //puck aufnehmen
            currentPuck.setColor(ObstacleColor::ME);
            MapData::setObstacle(currentPuck);
            state = SELECT_GOAL_SLOT;

            if(config::enableDebugGame)
                qDebug() << "Statemachine set to SELECT_GOAL_SLOT because we recognized this puck as ours (yellow)";
        }
        else{
            currentPuck.setColor(ObstacleColor::OPPONENT);
            MapData::setObstacle(currentPuck);
            handleEnemyPuckState = FIND_SLOT;
            state = DRIVE_TO_DUMP;

            if(config::enableDebugGame)
                qDebug() << "Statemachine set to DRIVE_TO_DUMP";
        }

    }
        break;
    default:
        if(colorfail > 6)
        {
            if(config::enableDebugGame)
                qDebug() << "Could not find out color. Giving up. Statemachine set to SELECT_PUCK";
            state = SELECT_PUCK;
        }
        else
        {
            colorfail++;
            QThread::msleep(50);
            Q_EMIT signalStartColorDetectAI();
        }

    }

}

void Game::getPositionToTurnRoboToMiddlePoint(Position &robot)
{
    double x = fabs(robot.x() - (config::geoFieldWidth / 2));
    double y = fabs(robot.y() - (config::geoFieldHeight / 2));
    double beta = atan(x/y);
    if(robot.x() > (config::geoFieldWidth / 2))
    {
        if (robot.y() < (config::geoFieldHeight / 2)){
            robot.rot((3*M_PI/2) + beta);
        } else {
            robot.rot((3*M_PI/2) - beta);
        }
    } else {
        if (robot.y() < (config::geoFieldHeight / 2)){
            robot.rot((M_PI/2) - beta);
        } else {
            robot.rot((M_PI/2) + beta);
        }
    }
}

Position Game::getTargetForPuckBeforePole()
{
    double distance = 99;
    Obstacle pole;
    Q_FOREACH(Obstacle poleUndef, MapData::getObstacle(ObstacleType::POLE))
    {
        double dist = currentPuck.getDistanceTo(poleUndef);
        if ( dist < distance)
        {
            distance = dist;
            pole = poleUndef;
        }
    }

    Position newPosition;
    double distanceToPole = distance + config::DIST_TO_PUCK_BEFORE_GATHERING_IT;
    double x = fabs(pole.getPosition().x() - currentPuck.getPosition().x());
    double y = fabs(pole.getPosition().y() - currentPuck.getPosition().y());
    double beta = atan(y/x);

    // Pole auf linker Seite
    if(pole.getPosition().x() < config::geoFieldWidth)
    {
        // Pole unter Puck
        if(pole.getPosition().y() < currentPuck.getPosition().y())
        {
            newPosition.y(pole.getPosition().y() + distanceToPole*sin(beta));
            newPosition.rot(M_PI + beta);
        }
        else // Pole über Puck
        {
            newPosition.y(pole.getPosition().y() - distanceToPole*sin(beta));
            newPosition.rot(M_PI - beta);
        }
        newPosition.x(pole.getPosition().x() + distanceToPole*cos(beta));
    }
    else    // Pole auf rechter Seite
    {
        // Pole unter Puck
        if(pole.getPosition().y() < currentPuck.getPosition().y())
        {
            newPosition.y(pole.getPosition().y() + distanceToPole*sin(beta));
            newPosition.rot(2*M_PI - beta);
        }
        else // Pole über Puck
        {
            newPosition.y(pole.getPosition().y() - distanceToPole*sin(beta));
            newPosition.rot(beta);
        }
        newPosition.x(pole.getPosition().x() - distanceToPole*cos(beta));
    }

    return newPosition;
}

Position Game::getTargetForPuck()
{
    Position newPosition;
    Position myPosition = MapData::getRobotPosition();

    double x = fabs(myPosition.x() - currentPuck.getPosition().x());
    double y = fabs(myPosition.y() - currentPuck.getPosition().y());
    double beta = atan(y/x);

    // Robo über Puck
    if (myPosition.y() > currentPuck.getPosition().y())
    {
        // Robo links von Puck
        if (myPosition.x() < currentPuck.getPosition().x())
        {
            newPosition.x(currentPuck.getPosition().x() - config::DIST_TO_PUCK_BEFORE_GATHERING_IT * cos(beta));
            newPosition.rot(2*M_PI - beta);
        }
        else // Robo rechts von Puck
        {
            newPosition.x(currentPuck.getPosition().x() + config::DIST_TO_PUCK_BEFORE_GATHERING_IT * cos(beta));
            newPosition.rot(M_PI + beta);
        }
        newPosition.y(currentPuck.getPosition().y() + config::DIST_TO_PUCK_BEFORE_GATHERING_IT * sin(beta));
    }
    else // Robo unter Puck
    {
        // Robo links von Puck
        if (myPosition.x() < currentPuck.getPosition().x())
        {
            newPosition.x(currentPuck.getPosition().x() - config::DIST_TO_PUCK_BEFORE_GATHERING_IT * cos(beta));
            newPosition.rot(beta);
        }
        else // Robo rechts von Puck
        {
            newPosition.x(currentPuck.getPosition().x() + config::DIST_TO_PUCK_BEFORE_GATHERING_IT * cos(beta));
            newPosition.rot(M_PI - beta);
        }
        newPosition.y(currentPuck.getPosition().y() - config::DIST_TO_PUCK_BEFORE_GATHERING_IT * sin(beta));
    }

    return newPosition;
}

void Game::slotAnnoyFoe()
{
    qDebug()<<"CHECKED SYS";
    MapData::clearTargets();
    state = WAIT_FOR_RELEASE;
    this->annoyFoeState();
}


