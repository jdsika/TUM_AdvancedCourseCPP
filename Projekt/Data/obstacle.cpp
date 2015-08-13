#include "obstacle.h"
#include "Data/define.h"

ObstacleColor Obstacle::getColor() const
{
    return enumColor;
}

void Obstacle::setColor(const ObstacleColor &value)
{
    enumColor = value;
}

ObstacleType Obstacle::getType() const
{
    return enumType;
}

void Obstacle::setType(const ObstacleType &value)
{
    enumType = value;
}

ObstacleStatus Obstacle::getStatus() const
{
    return enumStatus;
}

void Obstacle::setStatus(const ObstacleStatus &value)
{
    enumStatus = value;
}

QPair<double, double> Obstacle::getCoords() const
{
    return qMakePair(this->cPosition.x(), this->cPosition.y());
}

void Obstacle::setCoords(const QPair<double, double> &value)
{
    cPosition.x(value.first);
    cPosition.y(value.second);
    cPosition.setCertainty(1.0);
}

bool Obstacle::getInitialized() const
{
    return bInitialized;
}

void Obstacle::setInitialized(bool value)
{
    bInitialized = value;
}

QTime Obstacle::getLastUpdate() const
{
    return cLastUpdate;
}

void Obstacle::setLastUpdate(const QTime &value)
{
    cLastUpdate = value;
}

/**< radian between 0 and 2*PI*/
double Obstacle::getOrientation() const
{
    return cPosition.rot();
}

/**< position.rot( value ) constrains radian between 0 and 2*PI*/
void Obstacle::setOrientation(double value)
{
    cPosition.rot(value);
}

double Obstacle::getDistanceTo(const Obstacle& obstacle) const
{
    double dX = this->cPosition.x() - obstacle.getPosition().x();
    double dY = this->cPosition.y() - obstacle.getPosition().y();
    return std::sqrt(dX*dX + dY*dY);
}

Position Obstacle::getPosition() const
{
    return cPosition;
}

void Obstacle::setPosition(const Position &value)
{
    cPosition = value;
}

bool Obstacle::isInSpecifiedArea(FieldArea area) const
{
    bool isInArea = false;

    switch(area)
    {
    case FieldArea::GOAL_ZONE_LEFT:
    {
        if (this->getPosition().x() > config::geoGoalMarginSide &&
            this->getPosition().x() < config::geoGoalMarginSide + 1.0 / 3.0 * config::geoGoalWidth &&
            this->getPosition().y() > config::geoFieldHeight - 2.0 * config::geoPol_1_2 &&
            this->getPosition().y() < config::geoFieldHeight - 1.0 * config::geoPol_1_2)
            isInArea = true;
    }
        break;
    case FieldArea::GOAL_ZONE_MID:
    {
        if (this->getPosition().x() >= config::geoGoalMarginSide + 1.0 / 3.0 * config::geoGoalWidth &&
            this->getPosition().x() < config::geoGoalMarginSide + 2.0 / 3.0 * config::geoGoalWidth &&
            this->getPosition().y() > config::geoFieldHeight - 2.0 * config::geoPol_1_2 &&
            this->getPosition().y() < config::geoFieldHeight - 1.0 * config::geoPol_1_2)
            isInArea = true;

    }
        break;
    case FieldArea::GOAL_ZONE_RIGHT:
    {
        if (this->getPosition().x() >= config::geoGoalMarginSide + 2.0 / 3.0 * config::geoGoalWidth &&
            this->getPosition().x() < config::geoGoalMarginSide + 3.0 / 3.0 * config::geoGoalWidth &&
            this->getPosition().y() > config::geoFieldHeight - 2.0 * config::geoPol_1_2 &&
            this->getPosition().y() < config::geoFieldHeight - 1.0 * config::geoPol_1_2)
            isInArea = true;
    }
        break;
    case FieldArea::ENEMY_GOAL_INFLUENCE:
    {
        if (this->getPosition().x() > config::geoFieldWidth / 3.0 * 1.0 - config::SENSOR_RADIUS_ROBOT &&
            this->getPosition().x() < config::geoFieldWidth / 3.0 * 2.0 + config::SENSOR_RADIUS_ROBOT &&
            this->getPosition().y() > config::geoFieldHeight - 2.0 * config::geoPol_1_2 - config::SENSOR_RADIUS_ROBOT &&
            this->getPosition().y() < config::geoFieldHeight - 1.0 * config::geoPol_1_2 + config::SENSOR_RADIUS_ROBOT)
            isInArea = true;
    }
        break;
    case FieldArea::POLE_AREA:
    {
        if ((this->getPosition().x() > 0 - 0.5 &&
            this->getPosition().x() < 0 + 0.1) ||
            (this->getPosition().x() > config::geoFieldWidth - 0.1 &&
            this->getPosition().x() < config::geoFieldWidth + 0.5))
            isInArea = true;
    }
        break;
    case FieldArea::ENEMY_AREA:
    {
        if (this->getPosition().x() > 0 &&
            this->getPosition().x() < config::geoFieldWidth &&
            this->getPosition().y() > config::geoFieldHeight - (config::geoPol_1_2 + config::geoPol_2_3) &&
            this->getPosition().y() < config::geoFieldHeight - config::geoPol_1_2)
            isInArea = true;
    }
        break;
    case FieldArea::ENEMY_GOAL_AREA:
    {
        if (this->getPosition().x() > config::geoFieldWidth / 3.0 * 1.0 &&
            this->getPosition().x() < config::geoFieldWidth / 3.0 * 2.0 &&
            this->getPosition().y() > config::geoFieldHeight - 2.0 * config::geoPol_1_2 &&
            this->getPosition().y() < config::geoFieldHeight - 1.0 * config::geoPol_1_2)
            isInArea = true;
    }
        break;
    case FieldArea::ENEMY_BEHINDGOAL_AREA:
    {
        if (this->getPosition().x() > 0 &&
            this->getPosition().x() < config::geoFieldWidth &&
            this->getPosition().y() > config::geoFieldHeight - config::geoPol_1_2 &&
            this->getPosition().y() < config::geoFieldHeight)
            isInArea = true;
    }
        break;
    case FieldArea::MY_AREA:
    {
        if (this->getPosition().x() > 0 &&
            this->getPosition().x() < config::geoFieldWidth &&
            this->getPosition().y() > config::geoPol_1_2 &&
            this->getPosition().y() < config::geoPol_1_2 + config::geoPol_2_3)
            isInArea = true;
    }
        break;
    case FieldArea::MY_GOAL_AREA:
    {
        if (this->getPosition().x() > config::geoFieldWidth / 3.0 * 1.0 &&
            this->getPosition().x() < config::geoFieldWidth / 3.0 * 2.0 &&
            this->getPosition().y() > config::geoPol_1_2 &&
            this->getPosition().y() < 2 * config::geoPol_1_2)
            isInArea = true;
    }
        break;
    case FieldArea::MY_BEHIND_GOAL_AREA:
    {
        if (this->getPosition().x() > 0 &&
            this->getPosition().x() < config::geoFieldWidth &&
            this->getPosition().y() > 0 &&
            this->getPosition().y() < config::geoPol_1_2)
            isInArea = true;
    }
        break;
    case FieldArea::NEUTRAL_AREA:
    {
        if (this->getPosition().x() > 0 &&
            this->getPosition().x() < config::geoFieldWidth &&
            this->getPosition().y() > config::geoPol_1_2 + config::geoPol_2_3 &&
            this->getPosition().y() < config::geoFieldHeight - (config::geoPol_1_2 + config::geoPol_2_3))
            isInArea = true;
    }
        break;
    case FieldArea::OUT_OF_AREA:
    {
        if (this->getPosition().x() < 0 ||
            this->getPosition().x() > config::geoFieldWidth ||
            this->getPosition().y() < 0 ||
            this->getPosition().y() > config::geoFieldHeight)
            isInArea = true;
    }
    }
    return isInArea;
}

Obstacle::Obstacle()
{
    enumColor = ObstacleColor::UNIDENTIFIED;
    enumType = ObstacleType::UNIDENTIFIED;
    enumStatus = ObstacleStatus::UNDEFINED;
    cPosition = Position();
    bInitialized = false;
    cLastUpdate = QTime::currentTime();
}

Obstacle::Obstacle(Position m_Position)
{
    enumColor = ObstacleColor::UNIDENTIFIED;
    enumType = ObstacleType::UNIDENTIFIED;
    enumStatus = ObstacleStatus::UNDEFINED;
    this->setPosition(m_Position);
    bInitialized = true;
    cLastUpdate = QTime::currentTime();
}

Obstacle::Obstacle(Position m_Position,
                   ObstacleType m_enumType) :
    Obstacle(m_Position)
{
    enumType = m_enumType;
}

Obstacle::Obstacle(Position m_Position,
                   ObstacleColor m_enumColor,
                   ObstacleType m_enumType) :
    Obstacle(m_Position,
             m_enumType)
{
    enumColor = m_enumColor;
}

Obstacle::Obstacle(Position m_Position,
                   ObstacleStatus m_enumStatus,
                   ObstacleType m_enumType) :
    Obstacle(m_Position,
             m_enumType)
{
    enumStatus = m_enumStatus;
}

Obstacle::Obstacle(Position m_Position,
                   ObstacleColor m_enumColor,
                   ObstacleType m_enumType,
                   ObstacleStatus m_enumStatus) :
    Obstacle(m_Position,
             m_enumColor,
             m_enumType)
{
    enumStatus = m_enumStatus;
}

Obstacle::Obstacle(QPair<double, double> m_qpairCoords)
{
    enumColor = ObstacleColor::UNIDENTIFIED;
    enumType = ObstacleType::UNIDENTIFIED;
    enumStatus = ObstacleStatus::UNDEFINED;
    cPosition = Position(m_qpairCoords);
    bInitialized = true;
    cLastUpdate = QTime::currentTime();
}

Obstacle::Obstacle(QPair<double, double> m_qpairCoords,
                   ObstacleType m_enumType) :
    Obstacle(m_qpairCoords)
{
    enumType = m_enumType;

    if (m_enumType == ObstacleType::POLE)
        enumStatus = ObstacleStatus::BLOCKED;
    else
        enumStatus = ObstacleStatus::NOMODIFY;

}

Obstacle::Obstacle(QPair<double, double> m_qpairCoords,
                   ObstacleColor m_enumColor,
                   ObstacleType m_enumType) :
    Obstacle(m_qpairCoords,
             m_enumType)
{
    enumColor = m_enumColor;
}

Obstacle::Obstacle(QPair<double, double> m_qpairCoords,
                   ObstacleStatus m_enumStatus,
                   ObstacleType m_enumType) :
    Obstacle(m_qpairCoords,
             m_enumType)
{
    enumStatus = m_enumStatus;
}

Obstacle::Obstacle(QPair<double, double> m_qpairCoords,
                   ObstacleColor m_enumColor,
                   ObstacleType m_enumType,
                   ObstacleStatus m_enumStatus) :
    Obstacle(m_qpairCoords,
             m_enumStatus,
             m_enumType)
{
    enumColor  = m_enumColor;
}

Obstacle::Obstacle(QPair<double, double> m_qpairCoords,
                   ObstacleColor m_enumColor,
                   ObstacleType m_enumType,
                   ObstacleStatus m_enumStatus,
                   double m_dOrientation) :
    Obstacle(m_qpairCoords,
             m_enumColor,
             m_enumType,
             m_enumStatus)
{
    cPosition.rot(m_dOrientation);
}

Obstacle::~Obstacle()
{
}

bool Obstacle::operator == (const Obstacle &b) const
{
    // it will check if the distance is too close to be a seperate obstacle
    double tolerance = config::mapPuckPuckFusionDistance;

    ObstacleType thisType = this->getType();
    ObstacleType otherType = b.getType();

    switch(thisType)
    {
    case ObstacleType::POLE:
    {
        if(otherType == ObstacleType::PUCK)
            tolerance = config::mapPolePuckFusionDistance;
    }
        break;
    case ObstacleType::PUCK:
    {
        if(otherType == ObstacleType::POLE)
        {
            tolerance = config::mapPolePuckFusionDistance;
        }
        else if (otherType == ObstacleType::TARGET)
        {
            // a target is compared with a puck -> check if it has to be set to ISMOVING
            tolerance = config::guiPuckKlickTolerance;
        }
    }
        break;
    case ObstacleType::OPPONENT:
    case ObstacleType::ME:
    {
        tolerance = config::mapIgnorePuckInsideEnemyDistance;
    }
        break;
    default:
    {
        // default tolerance
        tolerance = config::obstacleCoordinateTolerance;
    }
    }

    return this->getPosition().isConsimilarTo(b.getPosition(), tolerance);
}

bool Obstacle::operator < (const Obstacle &b) const
{
    /// \todo check condition
    /*
    _Obstacle_Colors        enumColor;
    _Obstacle_Type          enumType;
    _Obstacle_Status        enumStatus;
    QPair<double, double>   qpairCoords;
    double                  dOrientation;
    bool                    bInitialized;
    QTime                   cLastUpdate;
    */

    return (( static_cast<int>(this->getColor())
              + static_cast<int>(this->getStatus())
              + static_cast<int>(this->getType()))
          < ( static_cast<int>(b.getColor())
              + static_cast<int>(b.getStatus())
              + static_cast<int>(b.getType())));

}

void Obstacle::mergeWith(const Obstacle &newObst)
{
    ///\todo merge conditions are a topic for discussion
    // the newObst would have the color "NOMODIFY" if the obstacle in the MAP should not be modified
    if(newObst.getColor() != ObstacleColor::NOMODIFY &&
       newObst.getColor() != ObstacleColor::UNIDENTIFIED)
        this->setColor(newObst.getColor());

    // das blocked wird hier aufgehoben, aber ja in der cleanup function wieder gesetzt -> eventuell kein problem
    if(newObst.getStatus() != ObstacleStatus::NOMODIFY &&
       newObst.getStatus() != ObstacleStatus::UNDEFINED &&
       newObst.getStatus() != ObstacleStatus::BLOCKED &&
       newObst.getStatus() != ObstacleStatus::INMYGOAL &&
       newObst.getStatus() != ObstacleStatus::INENEMYGOAL )
        this->setStatus(newObst.getStatus());

    if( newObst.getInitialized())
        this->setInitialized( true );

    this->setLastUpdate(newObst.getLastUpdate());

    QPair<double,double> newCoords;

    ///\todo average of coordinates
    if(this->getType() == ObstacleType::POLE) {
        newCoords = qMakePair(0.99 * this->getCoords().first + 0.01 * newObst.getCoords().first,
                              0.99 * this->getCoords().second + 0.01 * newObst.getCoords().second);
    } else {
        newCoords = qMakePair(0.5 * (this->getCoords().first + newObst.getCoords().first),
                              0.5 * (this->getCoords().second + newObst.getCoords().second));
    }
    this->setCoords(newCoords);

    this->setOrientation(newObst.getOrientation());

    return;
}


