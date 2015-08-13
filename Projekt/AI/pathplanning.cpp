#include "pathplanning.h"
#include "Data/define.h"

#include <QtConcurrent/QtConcurrentMap>
#include <QDebug>
#include <QElapsedTimer>

#include <iostream>

#include "Data/define.h"
#include "Data/mapdata.h"

std::atomic_bool PathPlanning::streamPathEnabled (false);
std::atomic_bool PathPlanning::enabled (true);
std::atomic_bool PathPlanning::avoidRestOfField (false);
std::atomic_bool PathPlanning::ignorePucks (false);

/**
 * @brief round helper function for integer rounding
 * @param r
 * @return the next integer value
 */
double round (double r){
    int tmp = static_cast<int> (r);
    tmp += (r-tmp>=.5) - (r-tmp<=-.5);
    return tmp;
}


/**
 * @brief PathPlanning::PathPlanning Constructor
 */
PathPlanning::PathPlanning()
{
    timer = new QElapsedTimer;
    timer->start();
}

PathPlanning::~PathPlanning()
{
    delete timer;
    qDebug()<<"DELETE PathPlanning";
}


// -----------------------------
// ----------- SLOTS -----------
// -----------------------------
void PathPlanning::planPath()
{
    // Auto-Enable when starting a PathPlanning
//    PathPlanning::setEnabled(true);

    // Enabled flag stays on true if not changed externally, so this is an "infinite" loop
    if(PathPlanning::getEnabled()){

        // The PathPlanning should not run faster than 50ms (That means it's probably not working properly anyways)
        if(timer->elapsed() < 50){
            int remainingTime = 50 - timer->elapsed();
            QThread::msleep(remainingTime);
        }
        timer->start();

        // find the robot's position
        QList<Obstacle> robotObstacleMe = MapData::getObstacle(ObstacleType::ME);
        if (robotObstacleMe.isEmpty()){
            qCritical() << "MapData::getObstacle(type_me) returns empty list";
            //continue; // start over with the while loop
            return; //carlo
        }
        robot = robotObstacleMe.first();
        robotX = robot.getCoords().first;
        robotY = robot.getCoords().second;
        robotRot = robot.getOrientation();

        // find the target position
        QList<Obstacle> targetObstacles = MapData::getObstacle(ObstacleType::TARGET);
        if(targetObstacles.isEmpty()){
            // Wenn keine Ziele vorhanden, nichts tun.
            return; //carlo
        }
        targetX = targetObstacles.first().getCoords().first;
        targetY = targetObstacles.first().getCoords().second;
        targetRot = targetObstacles.first().getOrientation();


        // If the robot has almost reached its target, no need to do any pathplanning
        double targetDist = robot.getDistanceTo(targetObstacles.first());
        if (targetDist <= config::pathPlanningEnabledUpwardsOfThisDistance){

            // Nur das Ziel als Wegpunkt ausgeben
            QList<QPair<double,double> > waypoints = QList<QPair<double,double> >();
            waypoints.append(targetObstacles.first().getCoords());
            Q_EMIT sendUpdatedWaypoints(waypoints);

            // Display map and path in GUI
            if(streamPathEnabled){
                PathPlotData dataPacket;
                dataPacket.dataType = PathPlotData::WAYPOINTS;
                dataPacket.data = QVector<PathPlotData::Point>(1*1);
                int index=0;
                for(int a=0; a<1; ++a){
                    for(int b=0; b<1; ++b){
                        dataPacket.data[index].x = 0;
                        dataPacket.data[index].y = 0;
                        dataPacket.data[index].value = 0;
                        index++;
                    }
                }
                dataPacket.robot = robot.getCoords();
                dataPacket.target = QPair<double,double> ( targetX, targetY );
                dataPacket.waypoints = waypoints;

                Q_EMIT pathDisplay(dataPacket);
            }

        } else { // Das Ziel ist weit entfernt und die Pfadplanung muss ausgeführt werden

            // actual work happening: create the grid, calculate grid values, find optimal path on grid.
            generateGrid();
            double timeGrid = timer->elapsed();
            calculatePathCosts();
            double timeCost = timer->elapsed();
            QPair<QList<QPair<double,double> >,QList<QPair<double,double> > > r = calculateWaypoints();
            QList<QPair<double,double> > waypoints = r.first;
            double timeComplete = timer->elapsed();

            if (config::enableDebugPathPlanning)
            {
                std::cout << "PathPlanning::slotPlanPath(): Path calculated after " << timer->elapsed()
                        << "ms (grid: " << (100*timeGrid/timeComplete)
                        << "%, cost: " << (100*(timeCost-timeGrid)/timeComplete)
                        << "%, path: " << (100*(timeComplete-timeCost)/timeComplete)
                        << "%)" << std::endl;
            }

            // we are done!
            Q_EMIT sendUpdatedWaypoints(waypoints);

            // Display map and path in GUI
            if(streamPathEnabled){
                PathPlotData dataPacket;
                dataPacket.dataType = PathPlotData::WAYPOINTS;
                dataPacket.data = QVector<PathPlotData::Point>(gridSizeA*gridSizeB);
                int index=0;
                for(int a=0; a<gridSizeA; ++a){
                    for(int b=0; b<gridSizeB; ++b){
                        dataPacket.data[index].x = grid[a][b].positionX;
                        dataPacket.data[index].y = grid[a][b].positionY;
                        dataPacket.data[index].value = grid[a][b].value;
                        index++;
                    }
                }
                dataPacket.robot = robot.getCoords();
                dataPacket.target = QPair<double,double> ( targetX, targetY );
                dataPacket.waypoints = waypoints;

                Q_EMIT pathDisplay(dataPacket);
            }
        }

//        std::cout << "done after " << timer.elapsed() << "ms" << std::endl;
    }

}

// -----------------------------
// ------ MEMBER FUNCTIONS -----
// -----------------------------

void PathPlanning::generateGrid()
{
    // While the world coordinate system uses the axes X,Y whith their origin at the
    // bottom left of the arena, the grid uses a A,B system that has its origin on the
    // robot. The A axis points from the robot to the target.

    // find grid spacing. it will be a value close to gridSpacingBase, but both the robot and target are guaranteed to be exactly on grid points
    double dX = targetX - robotX;
    double dY = targetY - robotY;
    double targetDistance = std::sqrt(dX*dX+dY*dY);
    if(targetDistance / config::pathGridSpacingBase >= 1.50) {
        gridSpacing = targetDistance / round(targetDistance / config::pathGridSpacingBase);
    } else if(targetDistance <= config::pathPlanningEnabledUpwardsOfThisDistance) {
        gridSpacing = config::pathGridSpacingBase;
    } else {
        gridSpacing = targetDistance;
    }

    // find grid rotation. the B axis points from the robot to the target.
    gridRotation = getGridRotation();

    // calculate A/B coordinates
    QPair<double,double> robotAB = grid2AB(robotX,robotY);
    robotA = static_cast<int>(round(robotAB.first));
    robotB = static_cast<int>(round(robotAB.second));
    QPair<double,double> targetAB = grid2AB(targetX,targetY);
    targetA = static_cast<int>(round(targetAB.first));
    targetB = static_cast<int>(round(targetAB.second));


    // find the extrema to know how big the grid needs to be
    QList<QPair<double,double> > corners;
    corners.append(grid2AB(-0.5,0.5));
    corners.append(grid2AB(3.5,-0.5));
    corners.append(grid2AB(-0.5,5.5));
    corners.append(grid2AB(3.5,5.5));
    minA=9999;
    minB=9999;
    maxA=-9999;
    maxB=-9999;
    for(int i=0; i<corners.size(); ++i){
        if(minA > std::floor(corners[i].first)) minA = std::floor(corners[i].first);
        if(minB > std::floor(corners[i].second)) minB = std::floor(corners[i].second);
        if(maxA < std::ceil(corners[i].first)) maxA = std::ceil(corners[i].first);
        if(maxB < std::ceil(corners[i].second)) maxB = std::ceil(corners[i].second);
    }
//    std::cout << "grid bounds: " << minA << ", " << minB << ", " << maxA << ", " << maxB << std::endl;

    gridSizeA = static_cast<int>(maxA-minA);
    gridSizeB = static_cast<int>(maxB-minB);

    // constrain robot and target positions. this should not be necessary :P
    targetA = (targetA < minA)? minA : targetA;
    targetA = (targetA > maxA-1)? maxA-1 : targetA;
    targetB = (targetB < minB)? minB : targetB;
    targetB = (targetB > maxB-1)? maxB-1 : targetB;
    robotA = (robotA < minA)? minA : robotA;
    robotA = (robotA > maxA-1)? maxA-1 : robotA;
    robotB = (robotB < minB)? minB : robotB;
    robotB = (robotB > maxB-1)? maxB-1 : robotB;

    // get all obstacles from the map
    obstaclesEnemy = MapData::getObstacle(ObstacleType::OPPONENT);
    obstaclesPole = MapData::getObstacle(ObstacleType::POLE);
    obstaclesPole.append(MapData::getObstacle(ObstacleType::UNIDENTIFIED));
    if(ignorePucks)
        obstaclesPuck.clear();
    else
        obstaclesPuck = MapData::getObstacle(ObstacleType::PUCK);

    /// \todo: Maybe remove a puck from the list if it is the target?

    // remove puck from obstacle list if it is being moved by our robot
    for(int i=0; i<obstaclesPuck.size(); ++i){
        if(obstaclesPuck.at(i).getStatus() == ObstacleStatus::ISMOVING){
                obstaclesPuck.removeAt(i);
        }
    }

    // generate grid
    grid.resize(gridSizeA);
    QVector<GridPoint> gridRow(gridSizeB);
    for(int a=minA; a<maxA; a++){
        for(int b=minB; b<maxB; b++){
            // create a point
            gridRow[b-minB]=GridPoint(this, a, b);
        }
        QtConcurrent::blockingMap(gridRow, &PathPlanning::initGridPoint); // initialize all grid points concurrently (fast)
        grid[a-minA]=gridRow;
    }

    // special care for the target and robot grid points
    grid[targetA-minA][targetB-minB].value = 0;
    grid[targetA-minA][targetB-minB].active = true;
    grid[targetA-minA][targetB-minB].type = GridPoint::TARGET;
    grid[robotA-minA][robotB-minB].type = GridPoint::ROBOT;

}

const double PathPlanning::getGridRotation()
{
    double dX = targetX - robotX;
    double dY = targetY - robotY;
    double angleInRad = std::atan2(dX,dY);
    return angleInRad;
}

// Dijkstra - Koennen wir eventuell A* implementieren? Kann man in rekursiver funktionsweise perfomance steigern?
void PathPlanning::calculatePathCosts()
{
    // while there are active points...
    bool activePointsFound = true;
    int iterations = 0;
    while(activePointsFound){
        // for each point...
        activePointsFound = false;
        for(int a=0; a<gridSizeA; ++a){
            for(int b=0; b<gridSizeB; ++b){

                if(grid[a][b].active == true){
                    activePointsFound = true;
                }else{
                    continue; // Der Punkt ist nicht aktiv, mache beim naechsten in der for schleife weiter
                }

                // for each active point's neighbor...
                QList<GridPoint*> neighbors = grid[a][b].getNeighbors();
                for(int i=0; i<neighbors.size(); ++i){

                    // calculate the cost from the target to this point
                    double adjacencyCost = config::pathAdjacencyMultiplier * grid[a][b].getDistance(neighbors[i]);
                    double newCost = grid[a][b].value + neighbors[i]->intrinsicCost + adjacencyCost;

                    // if the cost is less than its current value, update it and activate it
                    if( newCost < neighbors[i]->value ){
                        neighbors[i]->value = newCost;
                        neighbors[i]->active = true;
                    }
                }

                // all its neighbors have been checked, deactivate this point.
                grid[a][b].active = false;
                iterations++;
            }
        }
    }

//    std::cout << "PathPlanning::calculatePathCosts(): Pfadkosten in " << iterations << " Iterationen berechnet." << std::endl;

}

QPair<QList<QPair<double,double> >,QList<QPair<double,double> > > PathPlanning::calculateWaypoints()
{
    QList<GridPoint*> list = QList<GridPoint*>();
    GridPoint* point = &grid[robotA-minA][robotB-minB]; // we start at the robot's grid point

    // find the way to the target by following the gradient
    bool targetReached = false;
    int iterations = 0;
    while(!targetReached && iterations < config::pathMaxWPIterations){
        point = point->getGradientPoint();
        if(point == NULL) break; // something bad happened
        list.append(point);
        if(point->type == GridPoint::TARGET){
            targetReached = true;
        }
        iterations++;
    }
    if(!targetReached){
        list.clear();
        std::cerr << "PathPlanning::calculateWaypoints(): could not reach target" << std::endl;
    }

    // extract only coordinates
    QList<QPair<double,double> > waypoints = QList<QPair<double,double> >();
    QList<QPair<double,double> > gridpoints = QList<QPair<double,double> >();
    for(int i=0; i<list.size(); i++){
        QPair<double,double> wp = QPair<double,double>(list[i]->positionX, list[i]->positionY);
        QPair<double,double> gp = QPair<double,double>(list[i]->gridA-minA, list[i]->gridB-minB);
        waypoints.append(wp);
        gridpoints.append(gp);
    }
    QPair<QList<QPair<double,double> >,QList<QPair<double,double> > > r;
    r.first = waypoints;
    r.second = gridpoints;

    return r;
}

// calculate real-world X,Y coordinates for current grid
const QPair<double,double> PathPlanning::grid2XY(const int a, const int b){
    return grid2XY(a, b, gridRotation, gridSpacing);
}

// calculate grid coordinates (A,B) from XY for current grid
const QPair<double,double> PathPlanning::grid2AB(const double x, const double y){
    return grid2AB(x, y, gridRotation, gridSpacing);
}

// calculate real-world X,Y coordinates for any given grid rotation and spacing
const QPair<double,double> PathPlanning::grid2XY(
        const int a, const int b, const double gridRotation, const double gridSpacing){
    double x =  (a) * gridSpacing * std::cos(gridRotation) + (b) * gridSpacing * std::sin(gridRotation);
    double y = -(a) * gridSpacing * std::sin(gridRotation) + (b) * gridSpacing * std::cos(gridRotation);
    return QPair<double,double>(x,y);
}

// calculate grid coordinates (A,B) from XY for any given grid rotation and spacing
const QPair<double,double> PathPlanning::grid2AB(
        const double x, const double y, const double gridRotation, const double gridSpacing){
    double a = ((x) * std::cos(gridRotation) - (y) * std::sin(gridRotation))/gridSpacing;
    double b = ((x) * std::sin(gridRotation) + (y) * std::cos(gridRotation))/gridSpacing;
    return QPair<double,double>(a,b);
}

void PathPlanning::initGridPoint(PathPlanning::GridPoint &point)
{
    point.init(point);
}


bool PathPlanning::getAvoidRestOfField()
{
    return avoidRestOfField;
}

void PathPlanning::setAvoidRestOfField(const bool &value)
{
    avoidRestOfField = value;
}

bool PathPlanning::getIgnorePucks()
{
    return ignorePucks;
}

void PathPlanning::setIgnorePucks(const bool &value)
{
    ignorePucks = value;
}


bool PathPlanning::getEnabled()
{
    return enabled;
}

void PathPlanning::setEnabled(const bool &value)
{
    enabled = value;
}

// -----------------------------
// ------ MEMBER FUNCTIONS -----
// --- of the GridPoint class --
// -----------------------------

// empty constructor for vector initialization
PathPlanning::GridPoint::GridPoint(){}

// proper constructor for generating a valid grid point
PathPlanning::GridPoint::GridPoint(PathPlanning* pathPlanning, int gridA, int gridB)
{
    this->pathPlanning = pathPlanning;
    this->gridA = gridA;
    this->gridB = gridB;

    intrinsicCost = 0;
    value = std::numeric_limits<double>::infinity();
    type = GRID;
    isOutsideArena = false;
    active = false;
}

// does the initialization for a grid point, e.g. intrinsic cost calculation
void PathPlanning::GridPoint::init(GridPoint &point)
{
    // calculate real-world X,Y coordinates for this point
    QPair<double,double> pos = point.pathPlanning->grid2XY(point.gridA,point.gridB);
    point.positionX = pos.first;
    point.positionY = pos.second;
    if((point.positionX < config::pathArenaMinX || point.positionX > config::pathArenaMaxX ||
       point.positionY < config::pathArenaMinY || point.positionY > config::pathArenaMaxY)
            || (avoidRestOfField == true && (point.positionX < config::pathArenaMinX || point.positionX > config::pathArenaMaxX ||
                                             point.positionY < config::pathArenaMinY || point.positionY > config::pathArenaFieldAvoidMaxY) )
            ){
        point.isOutsideArena = true;
    }
    point.calculateIntrinsicCost();
}

void PathPlanning::GridPoint::calculateIntrinsicCost()
{
    this->intrinsicCost = 0;

    // Calculate the intrinsic cost of this point, based on the distances to all obstacles
    Q_FOREACH(Obstacle puck, pathPlanning->obstaclesPuck){
        double dist = Obstacle(QPair<double,double>(this->positionX,this->positionY)).getDistanceTo(puck);
        if(dist < config::pathPuckCloseDist){
            this->intrinsicCost += config::pathPuckCloseCost;
        } else if (dist < config::pathPuckFarDist){
            this->intrinsicCost += config::pathPuckStartCost
                    - config::pathPuckStartCost * (dist - config::pathPuckCloseDist)
                    / (config::pathPuckFarDist - config::pathPuckCloseDist);
        }
    }
    Q_FOREACH(Obstacle pole, pathPlanning->obstaclesPole){
        double dist = Obstacle(QPair<double,double>(this->positionX,this->positionY)).getDistanceTo(pole);
        if(dist < config::pathPoleCloseDist){
            this->intrinsicCost += config::pathPoleCloseCost;
        } else if (dist < config::pathPoleFarDist){
            this->intrinsicCost += config::pathPoleStartCost
                    - config::pathPoleStartCost * (dist - config::pathPoleCloseDist)
                    / (config::pathPoleFarDist - config::pathPoleCloseDist);
        }
    }
    Q_FOREACH(Obstacle enemy, pathPlanning->obstaclesEnemy){
        double dist = Obstacle(QPair<double,double>(this->positionX,this->positionY)).getDistanceTo(enemy);
        if(dist < config::pathEnemyCloseDist){
            this->intrinsicCost += config::pathEnemyCloseCost;
        } else if (dist < config::pathEnemyFarDist){
            this->intrinsicCost += config::pathEnemyStartCost
                    - config::pathEnemyStartCost * (dist - config::pathEnemyCloseDist)
                    / (config::pathEnemyFarDist - config::pathEnemyCloseDist);
        }
    }

    // If the point is close to the target, add costs in such a way that the robot will approach from the desired angle
    double dX = pathPlanning->targetX - this->positionX;
    double dY = pathPlanning->targetY - this->positionY;
    double distToTarget = sqrt(dX*dX + dY*dY);
    bool isTargetRotValid = !std::isnan(pathPlanning->targetRot) && !std::isinf(pathPlanning->targetRot);
    if( isTargetRotValid && distToTarget < config::pathTargetApproachAngleInfluenceDistance ) {
        // Kosten abhängig vom Winkel
        double angleToTarget = atan2(this->positionY-pathPlanning->targetY, this->positionX-pathPlanning->targetX); // Der Winkel von diesem Punkt zum Zielpunkt
        double approachAngle = constrainAngle(pathPlanning->targetRot + M_PI);
        double deltaAngle = fabs(constrainAngle(angleToTarget - approachAngle)); // Wie weit weicht angleToTarget vom gewünschten Anfahrtswinkel ab
        double deviationFactor = std::min(deltaAngle / config::pathTargetApproachAngleMaxDeviationWithoutFullCost, 1.0);
        double approachCostFull = config::pathTargetApproachAngleFullCost * deviationFactor;
        // Abschwächung mit der Distanz
        double reductionByDistanceFactor = (config::pathTargetApproachAngleInfluenceDistance - distToTarget) / config::pathTargetApproachAngleInfluenceDistance; // 0...1 , weil distToTarget ist zwischen 0 und targetApproachAngleInfluenceDistance (siehe if-bed.)
        double approachCostReduced = approachCostFull * reductionByDistanceFactor;
        // Auf Kosten des Punkts addieren
        this->intrinsicCost += approachCostReduced;
    }

}

double PathPlanning::GridPoint::constrainAngle(const double in){
    double out = in;
    while(out > M_PI)  out -= 2*M_PI;
    while(out < -M_PI) out += 2*M_PI;
    return out;
}


double PathPlanning::GridPoint::getDistance(const PathPlanning::GridPoint *point)
{
    double dX = this->positionX - point->positionX;
    double dY = this->positionY - point->positionY;
    return std::sqrt(dX*dX + dY*dY);
}

QList<PathPlanning::GridPoint*> PathPlanning::GridPoint::getNeighbors(bool includeOutsideArena)
{
    QList<PathPlanning::GridPoint*> list = QList<PathPlanning::GridPoint*>();
    GridPoint* point;
    double minA = pathPlanning->minA;
    double minB = pathPlanning->minB;
    double maxA = pathPlanning->maxA;
    double maxB = pathPlanning->maxB;

    // right column neighbors
    if (this->gridA+2 < maxA) {
        // upper
        if(this->gridB+2 < maxB){
            point = &(pathPlanning->grid[this->gridA+1-minA][this->gridB+1-minB]);
            if(!point->isOutsideArena || includeOutsideArena) list << point;
        }
        // middle
        {
            point = &(pathPlanning->grid[this->gridA+1-minA][this->gridB-minB]);
            if(!point->isOutsideArena || includeOutsideArena) list << point;
        }
        // lower
        if(this->gridB > minB){
            point = &(pathPlanning->grid[this->gridA+1-minA][this->gridB-1-minB]);
            if(!point->isOutsideArena || includeOutsideArena) list << point;
        }
    }

    // top
    if (this->gridB+2 < maxB){
        point = &(pathPlanning->grid[this->gridA-minA][this->gridB+1-minB]);
        if(!point->isOutsideArena || includeOutsideArena) list << point;
    }
    // bottom
    if (this->gridB > minB){
        point = &(pathPlanning->grid[this->gridA-minA][this->gridB-1-minB]);
        if(!point->isOutsideArena || includeOutsideArena) list << point;
    }

    // left column
    if (this->gridA > minA){
        // upper
        if(this->gridB+2 < maxB){
            point = &(pathPlanning->grid[this->gridA-1-minA][this->gridB+1-minB]);
            if(!point->isOutsideArena || includeOutsideArena) list << point;
        }
        // middle
        {
            point = &(pathPlanning->grid[this->gridA-1-minA][this->gridB-minB]);
            if(!point->isOutsideArena || includeOutsideArena) list << point;
        }
        // lower
        if(this->gridB > minB){
            point = &(pathPlanning->grid[this->gridA-1-minA][this->gridB-1-minB]);
            if(!point->isOutsideArena || includeOutsideArena) list << point;
        }
    }

    return list;
}

PathPlanning::GridPoint *PathPlanning::GridPoint::getGradientPoint()
{
    // find neighbor with lowest value
    QList<GridPoint*> neighbors = this->getNeighbors();

    // if no neighbors were found, try again, but include neighbors outside the arena
    if(neighbors.isEmpty()){
        neighbors = this->getNeighbors(true);
    }

    // this should not happen
    if(neighbors.isEmpty()){
        std::cerr << "PathPlanning::GridPoint::getGradientPoint(): Path planning failed, because a grid point has no neighbors" << std::endl;
        return NULL;
    }

    // find the neighbor with the smallest value. this is the gradient point
    GridPoint* gradientPoint = neighbors[0];
    Q_FOREACH(GridPoint* neighbor, neighbors){
        if(neighbor->value < gradientPoint->value){
            gradientPoint = neighbor;
        }
    }

    return gradientPoint;
}
