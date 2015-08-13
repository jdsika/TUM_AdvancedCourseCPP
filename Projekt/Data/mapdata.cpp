#include "mapdata.h"

#include "Data/define.h"
#include "Sensor/cam.h"
#include "Structs/position.h"
#include "Sensor/orientierung.h"
#include "AI/pathplanning.h"

#include <QDebug>

QList<Obstacle> MapData::obstaclesPoles         = QList<Obstacle>();
QList<Obstacle> MapData::obstaclesPucks         = QList<Obstacle>();
QList<Obstacle> MapData::obstaclesUnidentified  = QList<Obstacle>();
QList<Obstacle> MapData::obstaclesTargets       = QList<Obstacle>();
Obstacle        MapData::obstacleMe         = Obstacle();
Obstacle        MapData::obstacleOpponent   = Obstacle();
Obstacle        MapData::obstacleDummy      = Obstacle();

// QMutex
QMutex MapData::mutexRobotME(QMutex::Recursive);
QMutex MapData::mutexRobotOpponent(QMutex::Recursive);
QMutex MapData::mutexRobotDummy(QMutex::Recursive);
QMutex MapData::mutexPucks(QMutex::Recursive);
QMutex MapData::mutexTargets(QMutex::Recursive);
QMutex MapData::mutexPoles(QMutex::Recursive);
QMutex MapData::mutexUnidentified(QMutex::Recursive);
QMutex MapData::mutexDisableEmergency;
QMutex MapData::mutexSimulationDetected;
QMutex MapData::mutexTeamColor;
QMutex MapData::mutexPuckInFork;
QMutex MapData::mutexPointerToPathPlanner;
QMutex MapData::mutexTargetNearEnemy;

//Bool
bool MapData::disableEmergency = false;
bool MapData::simulationDetected = true;
bool MapData::puckInFork = false;
bool MapData::targetNearEnemy = false;

PathPlanning* MapData::pointerToPathPlanner = nullptr;

// init the teamColor
CamColor MapData::teamColor = CamColor::NONE;

QList<Obstacle> MapData::getListByType(const ObstacleType &type)
{
    QList<Obstacle> tempList = QList<Obstacle>();

    switch (type) {
    case ObstacleType::POLE:
    {
        QMutexLocker locker_1 (&mutexPoles);
        return obstaclesPoles;
    }
    case ObstacleType::ME:
    {
        QMutexLocker locker_2 (&mutexRobotME);
        if (obstacleMe.getInitialized())
            return tempList << obstacleMe;
        else
            return tempList;
    }
    case ObstacleType::OPPONENT:
    {
        QMutexLocker locker_3 (&mutexRobotOpponent);

        if (obstacleOpponent.getInitialized())
            return tempList << obstacleOpponent;
        else
            return tempList;
    }
    case ObstacleType::PUCK:
    {
        QMutexLocker locker_4 (&mutexPucks);
        return obstaclesPucks;
    }
    case ObstacleType::UNIDENTIFIED:
    {
        QMutexLocker locker_5 (&mutexUnidentified);
        return obstaclesUnidentified;
    }
    case ObstacleType::TARGET:
    {
        QMutexLocker locker_6 (&mutexTargets);
        return obstaclesTargets;
    }
    case ObstacleType::DUMMY:
    {
        QMutexLocker locker_7 (&mutexRobotDummy);
        if (obstacleDummy.getInitialized())
            return tempList << obstacleDummy;
        else
            return tempList;
    }
    default:
    {
        /// \todo warning scheimssen
        return tempList;
    }
    }
}

bool MapData::getDisableEmergency()
{
    QMutexLocker locker (&mutexDisableEmergency);
    return disableEmergency;
}

void MapData::setDisableEmergency(bool value)
{
    QMutexLocker locker (&mutexDisableEmergency);
    disableEmergency = value;
}

bool MapData::getSimulationDetected()
{
    QMutexLocker locker (&mutexSimulationDetected);
    return simulationDetected;
}

void MapData::setSimulationDetected(bool value)
{
    QMutexLocker locker (&mutexSimulationDetected);
    simulationDetected = value;
}

bool MapData::isPuckInFork()
{
    QMutexLocker locker (&mutexPuckInFork);
    return puckInFork;
}

void MapData::setPuckInFork(bool value)
{
    QMutexLocker locker (&mutexPuckInFork);
    puckInFork = value;
}

void MapData::setPointerToPathPlanner(PathPlanning* value)
{
    QMutexLocker locker (&mutexPointerToPathPlanner);
    pointerToPathPlanner = value;
}

PathPlanning *MapData::getPointerToPathPlanner()
{
    QMutexLocker locker (&mutexPointerToPathPlanner);

    return pointerToPathPlanner;
}


bool MapData::getTargetNearEnemy()
{
    QMutexLocker locker(&mutexTargetNearEnemy);

    return targetNearEnemy;
}

void MapData::setTargetNearEnemy(bool value)
{
    QMutexLocker locker(&mutexTargetNearEnemy);

    targetNearEnemy = value;
}

void MapData::setActualColor(const TeamColor color)
{
    QMutexLocker locker(&mutexTeamColor);
    switch(color){
    case TeamColor::blue:
        teamColor = CamColor::BLUE;
        break;
    case TeamColor::yellow:
        teamColor = CamColor::YELLOW;
        break;
    default:
        qWarning() << "Unbekannte tatsächliche Teamfarbe: " << static_cast<int>(color);
        break;
    }
}

void MapData::setProbableColor(const CamColor color)
{
    QMutexLocker locker(&mutexTeamColor);
    teamColor = color;
}

CamColor MapData::getTeamColor()
{
    QMutexLocker locker(&mutexTeamColor);
    return teamColor;
}

QList<Obstacle> MapData::getObstacle(const ObstacleType &type)
{
    return getListByType(type);
}

QList<Obstacle> MapData::getObstacle(const ObstacleType &type, const ObstacleColor &color)
{
    QList<Obstacle> completeList = getListByType(type);
    QList<Obstacle> partialList  = QList<Obstacle>();

    QList<Obstacle>::const_iterator i;
    for (i = completeList.constBegin(); i != completeList.constEnd(); ++i) {
        if ( (*i).getColor() == color)
            partialList += (*i);
    }

    return partialList;
}

QList<Obstacle> MapData::getObstacle(const ObstacleType &type, const ObstacleStatus &status)
{
    QList<Obstacle> completeList = getListByType(type);
    QList<Obstacle> partialList  = QList<Obstacle>();

    QList<Obstacle>::const_iterator i;
    for (i = completeList.constBegin(); i != completeList.constEnd(); ++i) {
        if ( (*i).getStatus() == status)
            partialList += (*i);
    }

    return partialList;
}

QList<Obstacle> MapData::getObstacle(const ObstacleType &type, const ObstacleColor &color, const ObstacleStatus &status)
{
    QList<Obstacle> completeList = getObstacle(type, color);
    QList<Obstacle> partialList  = QList<Obstacle>();

    QList<Obstacle>::const_iterator i;
    for (i = completeList.constBegin(); i != completeList.constEnd(); ++i) {
        if ( (*i).getStatus() == status)
            partialList += (*i);
    }

    return partialList;
}

Position MapData::getRobotPosition(ObstacleType type)
{
    switch (type) {
    case ObstacleType::ME:
    {
        QMutexLocker locker (&mutexRobotME);
        return obstacleMe.getPosition();
    }
    case ObstacleType::OPPONENT:
    {
        QMutexLocker locker (&mutexRobotOpponent);
        return obstacleOpponent.getPosition();
    }
    case ObstacleType::DUMMY:
    {
        QMutexLocker locker (&mutexRobotDummy);
        return obstacleDummy.getPosition();
    }
    default:
        return Position();
    }
}

bool MapData::deleteObstacle(ObstacleType type)
{
    switch (type) {
    case ObstacleType::ME:
    {
        QMutexLocker locker (&mutexRobotME);
        obstacleMe = Obstacle();
    }
        break;
    case ObstacleType::OPPONENT:
    {
        QMutexLocker locker (&mutexRobotOpponent);
        obstacleOpponent = Obstacle();
    }
        break;
    case ObstacleType::DUMMY:
    {
        QMutexLocker locker (&mutexRobotDummy);
        obstacleDummy = Obstacle();
    }
        break;
    default: //no action for other types till now
        return false;
    }

    return true;
}

bool MapData::setObstacle(const QList<Obstacle> &list)
{
    // FIRST LOCK: ME
    Position roboPos = MapData::getRobotPosition(ObstacleType::ME);
    Position enemyRoboPos = MapData::getRobotPosition(ObstacleType::OPPONENT);

    // all elements must be added successfully
    bool bOK = true;

    for(int i = 0; i < list.size(); ++i) {
        bOK &= organizeObstacles(list.at(i), roboPos);
    }

    // cleanup method for obstacles in map
    cleanup(roboPos, enemyRoboPos);

    return bOK;
}

bool MapData::setObstacle(const QList<Obstacle> &list, const Position &currentRoboPos)
{
    Position enemyRoboPos = MapData::getRobotPosition(ObstacleType::OPPONENT);
    // all elements must be added successfully
    bool bOK = true;

    for(int i = 0; i < list.size(); ++i) {
        bOK &= organizeObstacles(list.at(i), currentRoboPos);
    }

    // cleanup method for obstacles in map
    cleanup(currentRoboPos, enemyRoboPos);

    return bOK;
}

bool MapData::setObstacle(const Obstacle &value)
{
    // FIRST LOCK: ME
    Position roboPos = MapData::getRobotPosition(ObstacleType::ME);
    Position enemyRoboPos = MapData::getRobotPosition(ObstacleType::OPPONENT);

    bool bOK = organizeObstacles(value, roboPos);

    // cleanup method for obstacles in map
    cleanup(roboPos, enemyRoboPos);

    return bOK;
}

bool MapData::setObstacle(const Obstacle &value, const Position &currentRoboPos)
{
    Position enemyRoboPos = MapData::getRobotPosition(ObstacleType::OPPONENT);
    bool bOK = organizeObstacles(value, currentRoboPos);

    // cleanup method for obstacles in map
    cleanup(currentRoboPos, enemyRoboPos);

    return bOK;
}

void MapData::clearTargets()
{
    QMutexLocker locker_1 (&mutexTargets);
    // items are not a pointer type -> qDeleteAll() not necessary
    obstaclesTargets.clear();
}

Obstacle MapData::getFirstTarget()
{
    QMutexLocker locker (&mutexTargets);

    if (obstaclesTargets.isEmpty())
        return Obstacle();
    else
        return obstaclesTargets.first();
}

void MapData::deleteFirstTarget()
{
    // LOCK OPPONENT FIRST
    Position enemyRobotPosition = MapData::getRobotPosition(ObstacleType::OPPONENT);
    QMutexLocker lockerTargets (&mutexTargets);

    int listSize = obstaclesTargets.size();

    if ( listSize == 0 ) {
        // no target in list
        return;
    }
    else if ( listSize == 1) {
        obstaclesTargets.removeFirst();
        return;
    }
    else {
        // the second target may now be close to the enemy
        // we have to check here, if the enemy is currently close to the next target
        MapData::checkForEnemyNearTraget(enemyRobotPosition);

        // if a target was deleted we need at least one more path planning for the next traget
        QMetaObject::invokeMethod(MapData::getPointerToPathPlanner(), "planPath", Qt::AutoConnection);
    }
}


void MapData::cleanup(const Position &currentRoboPos, const Position &enemyRobotPosition)
{
    // is locked within the whole function
    QMutexLocker lockPole(&mutexPoles);

    QMutexLocker lockPucks(&mutexPucks);

    // Sobald auch nur ein Puck in der Gabel ist, wird esGibtEinenPuckInDerGabel true gesetzt und bleiben
    Position roboPos = currentRoboPos;

    bool esGibtEinenPuckInDerGabel = false;

    QList<int> indexToRemove = QList<int>();

    // Pucks aufräumen
    // Für jeden Puck in der Map:
    for(int j=0; j<obstaclesPucks.size(); ++j)
    {
        /***************************************************/
        // Wenn der Puck nahe am gegnerischen Roboter ist, ist es wahrscheinlich aber gar kein Puck, sondern Kabel am Roboter oder so.
        // In dem Fall wollen wir den "Puck" gar nicht in die Map speichern
        /***************************************************/
        if(enemyRobotPosition.certainty() != 0.0)
        {
            double distToEnemy = obstaclesPucks.at(j).getPosition().getDistanceTo(enemyRobotPosition);
            if(distToEnemy < config::mapIgnorePuckInsideEnemyDistance){
                // Das hier scheint tatsächlich eher ein Kabel zu sein.
                indexToRemove << j;
                continue; // Mit nächstem Puck weitermachen
            }
        }

        /***************************************************/
        // suche nach identischen Pucks in der map und merge
        /***************************************************/
        for(int i = 0; i < obstaclesPucks.size() && i != j; ++i) {
            if(obstaclesPucks.at(j).getPosition().isConsimilarTo(obstaclesPucks.at(i).getPosition(), config::mapPuckPuckFusionDistance))
            {
                // Das hier scheint ein bestehender Puck zu sein --> mergen
                obstaclesPucks[i].mergeWith(obstaclesPucks.at(j));
                //add j to remove list
                indexToRemove << j;
            }
        }

        /********************************************************************/
        /*** Überprüfe, ob ein anderer Puck sehr nahe zu Pole ist         ***/ /// MUTEX POLE
        /********************************************************************/
        int indexPuckPole = -1;

        indexPuckPole = obstaclesPoles.indexOf(obstaclesPucks.at(j));

        if(indexPuckPole == -1) {
            // nichts tun
        }
        else {
            // Das hier scheint ein bestehender Puck zu sein --> mergen
            obstaclesPoles[indexPuckPole].mergeWith(obstaclesPucks.at(j));
             indexToRemove << j;
        }

        // hier wird es eine funktion geben, die alle pucks im gegenerischen tor blocked setzt
        // ausnahmen sind pucks, die die gerade als ISMOVING gekennzeichnet sind, damit ich sie
        // auf die korrekte position im tor fahren kann
        if(obstaclesPucks.at(j).isInSpecifiedArea(FieldArea::ENEMY_GOAL_AREA) &&
           obstaclesPucks.at(j).getStatus() != ObstacleStatus::ISMOVING)
        {
            obstaclesPucks[j].setStatus(ObstacleStatus::BLOCKED);
        }

        // suche nach pucks in meiner goal area
        if(obstaclesPucks.at(j).isInSpecifiedArea(FieldArea::MY_GOAL_AREA) &&
           obstaclesPucks.at(j).getStatus() != ObstacleStatus::ISMOVING)
        {
            obstaclesPucks[j].setStatus(ObstacleStatus::INMYGOAL);
        }

        Position gabelPos = Position(roboPos.x() + config::mapAbstandRoboterZentrumZuGabel * cos(roboPos.rot()),
                                     roboPos.y() + config::mapAbstandRoboterZentrumZuGabel * sin(roboPos.rot()));

        bool hasSimilarDistance = obstaclesPucks.at(j).getPosition().isConsimilarTo(gabelPos);

        Position gabelPosMin = Position(roboPos.x() + config::mapToleranzBisWohinEinPuckInDerGabelIstMIN * cos(roboPos.rot()),
                                        roboPos.y() + config::mapToleranzBisWohinEinPuckInDerGabelIstMIN * sin(roboPos.rot()));

        Position gabelPosMax = Position(roboPos.x() + config::mapToleranzBisWohinEinPuckInDerGabelIstMAX * cos(roboPos.rot()),
                                        roboPos.y() + config::mapToleranzBisWohinEinPuckInDerGabelIstMAX * sin(roboPos.rot()));

        esGibtEinenPuckInDerGabel |= hasSimilarDistance && Orientation::checkObjectsOnLine(gabelPosMin,obstaclesPucks.at(j).getPosition(),gabelPosMax);
    }

    int lastIndex = -1;

    for(int i = 0; i < indexToRemove.size(); ++i) {
        if(lastIndex != indexToRemove.at(i))
            obstaclesPucks.removeAt(indexToRemove.at(i));
        lastIndex = indexToRemove.at(i);
    }

    // set bool to verify if a puck is inside the fork
    MapData::setPuckInFork(esGibtEinenPuckInDerGabel);
}

void MapData::checkForEnemyNearTraget(const Position &enemyRobotPosition)
{
    Obstacle currentTargetObstacle = MapData::getFirstTarget();

    if( enemyRobotPosition.certainty() != 0.0 &&
        currentTargetObstacle.getInitialized())
    {
        Position targetPosition = currentTargetObstacle.getPosition();

        if(targetPosition.isConsimilarTo(enemyRobotPosition, 2.0 * config::SENSOR_RADIUS_ROBOT)) {
            MapData::setTargetNearEnemy(true);
        }
        else {
            MapData::setTargetNearEnemy(false);
        }
    }

    return;
}

/**
 * @brief Adding items to the map according to their type
 */
bool MapData::organizeObstacles(const Obstacle &constObstacle, const Position &currentRoboPos)
{
    bool bOK = true;

    Obstacle newObstacle = constObstacle;

    // the minimum requirement for an obstacle is a valid coords pair
    if (std::isnan(newObstacle.getCoords().first)
            || std::isnan(newObstacle.getCoords().second))
        return !bOK;

    ObstacleType type = newObstacle.getType();
    int index = -1;

    /// \todo: klären wie verschiewdene obstacles gemerged werden
    // the obstacles are saved destinctly by type
    switch (type) {
    case ObstacleType::POLE:
    {
        QMutexLocker lockerUnidentified (&mutexUnidentified);
        QMutexLocker lockerPoles (&mutexPoles);

        index = obstaclesPoles.indexOf(newObstacle);

        if( index != -1) {
            // merging he existing item with the new one
            obstaclesPoles[index].mergeWith(newObstacle);
        }
        else if (obstaclesPoles.size() < config::obstacleNumberOfPoles)
        {
            // adding item at the beginning of the list
            obstaclesPoles.prepend(newObstacle);
        }
        else
        {
            bOK = false;
        }

        int unidentifiedIndex = obstaclesUnidentified.indexOf(newObstacle);
        // Wenn ein Objekt vorher nicht identifiziert war (Ein Element in der obstalcesUnidetified Liste ist), jetzt aber identifiziert wurde, dann lösche es aus dieser Liste
        if (unidentifiedIndex != -1) {
            obstaclesUnidentified.removeAt(unidentifiedIndex);
        }
    }
        break;
    case ObstacleType::ME:
    {
        QMutexLocker lockerSetMe (&mutexRobotME);
        obstacleMe = newObstacle;
    }
        break;
    case ObstacleType::OPPONENT:
    {
        QMutexLocker lockerOpponent (&mutexRobotOpponent);
        // hier muss validiert werden ob die neue position realistisch ist und ob er sich so schnell bewegt haben kann!
        // ist im pole areal
        bool isInPoleArea = newObstacle.isInSpecifiedArea(FieldArea::POLE_AREA);

        // entscheide ob neue position übernommen werden soll
        if(!isInPoleArea) {
            obstacleOpponent = newObstacle;

            // we have to check here, if the enemy is currently close to the next target
            MapData::checkForEnemyNearTraget(obstacleOpponent.getPosition());
        }
    }
        break;
    case ObstacleType::PUCK:
    {
        QMutexLocker lockerForUnidentified (&mutexUnidentified);
        QMutexLocker lockPoles(&mutexPoles);
        QMutexLocker lockPucks(&mutexPucks);

        bool thisPuckShouldBeAddedToTheList = true;

        /********************************************************************/
        /*** Überprüfe, ob ein anderer Puck sehr nahe zu Pole ist         ***/ /// MUTEX POLE
        /********************************************************************/
        int indexPuckPole = -1;

        indexPuckPole = obstaclesPoles.indexOf(newObstacle);

        if(indexPuckPole == -1) {
            thisPuckShouldBeAddedToTheList = true;
        }
        else {
            // Das hier scheint ein bestehender Puck zu sein --> mergen
            obstaclesPoles[indexPuckPole].mergeWith(newObstacle);
            thisPuckShouldBeAddedToTheList = false;
        }

        /**********************************************/
        /***        check if puck is in fork        ***/        /// ISINFORK
        /**********************************************/
        Position roboPos = currentRoboPos;

        Position gabelPos = Position(roboPos.x() + config::mapAbstandRoboterZentrumZuGabel * cos(roboPos.rot()),
                                     roboPos.y() + config::mapAbstandRoboterZentrumZuGabel * sin(roboPos.rot()));

        // benutzt standard delta von 10cm
        bool hasSimilarDistance = newObstacle.getPosition().isConsimilarTo(gabelPos);

        // er ist nah genug für einen moving puck
        // nun muss ich ausschließen, dass er nicht neben dem ROBO ist!
        // ergo auf einer linie mit einer min und einer maxposition vor dem roboter
        if(hasSimilarDistance)
        {
            // NEUER ANSATZ: minimum position bis maximum position und sehen ob newObstacle auf linie liegt
            Position gabelPosMin = Position(roboPos.x() + config::mapToleranzBisWohinEinPuckInDerGabelIstMIN * cos(roboPos.rot()),
                                            roboPos.y() + config::mapToleranzBisWohinEinPuckInDerGabelIstMIN * sin(roboPos.rot()));

            Position gabelPosMax = Position(roboPos.x() + config::mapToleranzBisWohinEinPuckInDerGabelIstMAX * cos(roboPos.rot()),
                                            roboPos.y() + config::mapToleranzBisWohinEinPuckInDerGabelIstMAX * sin(roboPos.rot()));

            bool isInFork = Orientation::checkObjectsOnLine(gabelPosMin,newObstacle.getPosition(),gabelPosMax);
            // der abstand zur gabel liegt zwischen 17 und 27cm
            if(isInFork)
            {   // wenn in der gabel als blocked gesetzt wurde will ich das nicht überschreiben
                if (newObstacle.getStatus() != ObstacleStatus::BLOCKED) {
                    // Der Neue Puck bekommt ISMOVING.
                    newObstacle.setStatus(ObstacleStatus::ISMOVING);
                }
            }
            MapData::setPuckInFork(isInFork);
        }

        /**********************************************/
        /*** closest ISMOVING Puck will be replaced ***/    /// MUTEX PUCKS
        /**********************************************/
        // Wenn der neue Puck den Status ISMOVING hat, sollen alle anderen ihn verlieren
        // falls der neue BLOCKED ist, dann soll er den ISMOVING ersetzen
        // der puck der am nähesten dran ist, wird durch den neuen ersetzt
        if ( newObstacle.getStatus() == ObstacleStatus::ISMOVING||
             newObstacle.getStatus() == ObstacleStatus::BLOCKED)
        {
            double minDist = std::numeric_limits<double>::infinity();
            int indexOfClosestPuck = -1;

            for(int i = 0; i < obstaclesPucks.size(); ++i){
                if(obstaclesPucks.at(i).getStatus() == ObstacleStatus::ISMOVING)
                {
                    double dist = newObstacle.getDistanceTo(obstaclesPucks.at(i));
                    if(dist < minDist) {
                        minDist = dist;
                        if (indexOfClosestPuck != -1) // setzt alten ISMOVING auf unblocked
                            obstaclesPucks[indexOfClosestPuck].setStatus(ObstacleStatus::UNBLOCKED);
                        indexOfClosestPuck = i;
                    }
                }
            }
            if(std::isinf(minDist)){
                // Es gibt noch keine Pucks in der Map
                thisPuckShouldBeAddedToTheList = true;
            } else {

                obstaclesPucks.replace(indexOfClosestPuck,newObstacle);
                // nun wurde dieser puck aufgenommen
                thisPuckShouldBeAddedToTheList = false;
            }
        }


        /********************************************************************/
        /*** Überprüfe, ob ein anderer Puck sehr nahe zu diesem Neuen ist ***/ /// MUTEX PUCKS
        /********************************************************************/
        int indexPuckPuck = -1;

        indexPuckPuck = obstaclesPucks.indexOf(newObstacle);

        if(indexPuckPuck == -1) {
            thisPuckShouldBeAddedToTheList = true;
        }
        else {
            // Das hier scheint ein bestehender Puck zu sein --> mergen
            obstaclesPucks[indexPuckPuck].mergeWith(newObstacle);
            thisPuckShouldBeAddedToTheList = false;
        }

        /********************************************************************/
        /***                Adde und entferne alle über 6                 ***/      /// MUTEX PUCKS
        /********************************************************************/
        if(thisPuckShouldBeAddedToTheList) {
            // Diesen neuen Puck endlich zur Liste hinzufügen

            // Es darf nicht mehr als 6 Pucks geben
            if (obstaclesPucks.size() <= config::obstacleNumberOfPucks) {
                // Der neue Puck sollte den Status Unblocked haben, außer er wurde vorher schon als
                // ISMOVING markiert, zB weil er das Ziel der KI ist oder sich in der Gabel befindet
                if(newObstacle.getStatus() != ObstacleStatus::ISMOVING)
                    newObstacle.setStatus(ObstacleStatus::UNBLOCKED);

                // Dieser _neue_ Puck hat eine unbekannte Farbe
                if(newObstacle.getColor() == ObstacleColor::NOMODIFY)
                    newObstacle.setColor(ObstacleColor::UNIDENTIFIED);

                obstaclesPucks.prepend(newObstacle);
            } else {
                // Zuviele Pucks --> Puck mit ältestem Timestamp löschen
                QList<Obstacle>::iterator iteratorToOldestPuck;

                iteratorToOldestPuck = std::max_element(obstaclesPucks.begin(),
                                                        obstaclesPucks.end(),
                                                        &MapData::compareObstacleTimestamps);

                obstaclesPucks.erase(iteratorToOldestPuck);
            }
        }

        int unidentifiedIndex = obstaclesUnidentified.indexOf(newObstacle);
        // Wenn ein Objekt vorher nicht identifiziert war (Ein Element in der obstalcesUnidetified Liste ist), jetzt aber identifiziert wurde, dann lösche es aus dieser Liste
        if (unidentifiedIndex != -1) {
            obstaclesUnidentified.removeAt(unidentifiedIndex);
        }
    }
        break;
    case ObstacleType::UNIDENTIFIED:
    {
        QMutexLocker lockerUnidentifiedObstacles (&mutexUnidentified);

        // is the unidentified object already listed?
        index = obstaclesUnidentified.indexOf(newObstacle);
        if( index != -1) {
            // merging he existing item with the new one
            obstaclesUnidentified[index].mergeWith(newObstacle);
        }
        else {
            QMutexLocker lockerPoles (&mutexPoles);
            index = obstaclesPoles.indexOf(newObstacle);
            if (index != -1) {
                // adding item at the beginning of the list
                obstaclesPoles[index].mergeWith(newObstacle);
            }
            else {
                QMutexLocker lockerPucks (&mutexPucks);
                index = obstaclesPucks.indexOf(newObstacle);
                if (index != -1) {
                    // adding item at the beginning of the list
                    obstaclesPucks[index].mergeWith(newObstacle);
                }
                else {
                    obstaclesUnidentified.append(newObstacle);

                    if (obstaclesUnidentified.size() > config::obstacleNumberOfPucks - obstaclesPucks.size())
                    {
                        QList<Obstacle>::iterator iteratorToOldestPuck;

                        iteratorToOldestPuck = std::max_element(obstaclesUnidentified.begin(),
                                                                obstaclesUnidentified.end(),
                                                                &MapData::compareObstacleTimestamps);

                        obstaclesUnidentified.erase(iteratorToOldestPuck);

                        if(config::enableDebugMapData)
                            qDebug() << "*** Erased old Unidentifed Obstacle ***";
                    }
                }
            }
        }
    }
        break;
    case ObstacleType::TARGET:
    {
        // LOCK OPPONENT FIRST
        Position enemyRobotPosition = MapData::getRobotPosition(ObstacleType::OPPONENT);

        QMutexLocker lockerTargets(&mutexTargets);
        QMutexLocker lockerPucks(&mutexPucks);

        // search in the puck list with a tolerance of config::guiPuckKlickTolerance
        index = obstaclesPucks.indexOf(newObstacle);
        if( index != -1) {
            // the target was set on a puck -> puck ISMOVING
            obstaclesPucks[index].setStatus(ObstacleStatus::ISMOVING);
        }

        obstaclesTargets.append(newObstacle);

        // we have to check here, if the enemy is currently close to the next target
        MapData::checkForEnemyNearTraget(enemyRobotPosition);

        QMetaObject::invokeMethod(MapData::getPointerToPathPlanner(), "planPath", Qt::AutoConnection);
    }
        break;
    case ObstacleType::DUMMY:
    {
        QMutexLocker locker_7 (&mutexRobotME);
        obstacleDummy = newObstacle;
    }
        break;
    default:
    {
        /// \todo: warning für default state
        bOK = false;
    }
        break;
    }

    return bOK;
}

bool MapData::compareObstacleTimestamps(Obstacle i, Obstacle j)
{
    return i.getLastUpdate() > j.getLastUpdate();
}

