#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "Structs/position.h"

#include <QPair>
#include <QTime>

/**
 * @brief The FieldArea enum defines the different areas of the field
 */
enum class FieldArea {
    OUT_OF_AREA = 0, /**<  position is not in the game area */
    MY_AREA, /**< Position is on my side of the field */
    MY_GOAL_AREA, /**< Position is in my goal area */
    MY_BEHIND_GOAL_AREA, /**< Position is behind my goal area */
    ENEMY_AREA, /**< Position is on the enemy side of the field */
    ENEMY_GOAL_AREA, /**< Position is in the enemy goal area */
    ENEMY_BEHINDGOAL_AREA, /**< Position is behind the enemy goal area */
    NEUTRAL_AREA, /**< Position is in the neutral area of the field */
    POLE_AREA, /**< Position in the area around the poles*/
    ENEMY_GOAL_INFLUENCE, /**< Position in the area around the Goal*/
    GOAL_ZONE_LEFT,        /**< checking if an obstacle lies within a certain goal zone */
    GOAL_ZONE_MID,          /**< checking if an obstacle lies within a certain goal zone */
    GOAL_ZONE_RIGHT         /**< checking if an obstacle lies within a certain goal zone */

};

/**
 * @brief The ObstacleColor enum will represent the color of the enum fields
 */
enum class ObstacleColor {
    NOMODIFY = 0,       /**< enum which wont modified */
    UNIDENTIFIED,   /**< If color is unidentified*/
    POLE,           /**< pole color */
    ME,             /**< own color*/
    OPPONENT        /**< foe color*/
};

/**
 * @brief The ObstacleType enum which represents the obstacleType
 */
enum class ObstacleType {
    NOMODIFY = 0,           /**< enum which wont modified */
    UNIDENTIFIED,       /**< unidentified obstacle type*/
    DUMMY,              /**< dummy type for testing purpose*/
    POLE,               /**< pole type */
    PUCK,               /**< puck type*/
    OPPONENT,           /**< type for our foe*/
    ME,                 /**< type for own robot*/
    TARGET,             /**< type for target*/
};

/**
 * @brief The ObstacleStatus enum
 */
enum class ObstacleStatus {
    NOMODIFY = 0,       /**< enum which wont modified*/
    UNDEFINED,          /**< if the status is undefined */
    UNBLOCKED,          /**< if status is unblocked*/
    ISMOVING,           /**< if the puck is moved by the robot*/
    BLOCKED,            /**< if status is blocked*/
    INMYGOAL,           /**< if puck is in my own goal*/
    INENEMYGOAL,        /**< if puck is in my enemy goal*/
};


/**
 * @brief The Obstacle class describes all objects on field as obstacles, which can be distinguish by type
 */
class Obstacle
{
private:
    ObstacleColor enumColor; /**< member variable of color enum*/
    ObstacleType enumType; /**< member variable of type enum*/
    ObstacleStatus enumStatus; /**< member variable of status enum*/
    Position cPosition; /**< coordinates in x,y, orientation in radian*/
    bool bInitialized; /**< represents whether the class is initialised*/
    QTime cLastUpdate; /**< represents the last update time*/

public:
    /**
     * @brief Obstacle
     */
    Obstacle();

    /**
     * @brief Obstacle
     * @param[in] m_Position (class Position)
     */
    Obstacle( Position m_Position);

    /**
     * @brief Obstacle create an obstacle by setting qpairCoords and enumType
     * @param[in] m_Position (class Position)
     * @param[in] m_enumType (Type of ObstacleType enum)
     */
    Obstacle( Position m_Position,
              ObstacleType m_enumType);

    /**
     * @brief Obstacle create an obstacle by setting qpairCoords, enumColor and enumType
     * @param[in] m_Position (class Position)
     * @param[in] m_enumColor (Type of ObstacleType enum)
     * @param[in] m_enumType (Type from ObstacleColor enum)
     */
    Obstacle( Position m_Position,
              ObstacleColor m_enumColor,
              ObstacleType m_enumType);

    /**
     * @brief Obstacle create an obstacle by setting qpairCoords, enumStatus and enumType
     * @param[in] m_Position (class Position)
     * @param[in] m_enumStatus (Type of ObstacleStatus enum)
     * @param[in] m_enumType (Type from ObstacleColor enum)
     */
    Obstacle( Position m_Position,
              ObstacleStatus m_enumStatus,
              ObstacleType m_enumType);

    /**
     * @brief Obstacle create an obstacle by setting qpairCoords, enumColor, enumType and enumStatus
     * @param[in] m_Position (class Position)
     * @param[in] m_enumColor (Type of ObstacleType enum)
     * @param[in] m_enumType (Type from ObstacleColor enum)
     * @param[in] m_enumStatus (Type of ObstacleStatus enum)
     */
    Obstacle( Position m_Position,
              ObstacleColor m_enumColor,
              ObstacleType m_enumType,
              ObstacleStatus m_enumStatus);

    /**
     * @brief Obstacle create an obstacle by setting qpairCoords.
     * @param[in] m_qpairCoords (QPair<double,double>)
     */
    Obstacle( QPair<double, double> m_qpairCoords);

    /**
     * @brief Obstacle create an obstacle by setting qpairCoords and enumType
     * @param[in] m_qpairCoords (QPair<double,double>)
     * @param[in] m_enumType    (Type of ObstacleType enum)
     */
    Obstacle( QPair<double, double> m_qpairCoords,
              ObstacleType m_enumType);

    /**
     * @brief Obstacle create an obstacle by setting qpairCoords, enumColor and enumType
     * @param[in] m_qpairCoords (QPair<double,double>)
     * @param[in] m_enumColor (Type of ObstacleType enum)
     * @param[in] m_enumType (Type from ObstacleColor enum)
     */
    Obstacle( QPair<double, double> m_qpairCoords,
              ObstacleColor m_enumColor,
              ObstacleType m_enumType);

    /**
     * @brief Obstacle create an obstacle by setting qpairCoords, enumStatus and enumType
     * @param[in] m_qpairCoords (QPair<double,double>)
     * @param[in] m_enumStatus (Type of ObstacleStatus enum)
     * @param[in] m_enumType (Type from ObstacleColor enum)
     */
    Obstacle( QPair<double, double> m_qpairCoords,
              ObstacleStatus m_enumStatus,
              ObstacleType m_enumType);

    /**
     * @brief Obstacle create an obstacle by setting qpairCoords, enumColor, enumType and enumStatus
     * @param[in] m_qpairCoords (QPair<double,double>)
     * @param[in] m_enumColor (Type of ObstacleType enum)
     * @param[in] m_enumType (Type from ObstacleColor enum)
     * @param[in] m_enumStatus (Type of ObstacleStatus enum)
     */
    Obstacle( QPair<double, double> m_qpairCoords,
              ObstacleColor m_enumColor,
              ObstacleType m_enumType,
              ObstacleStatus m_enumStatus);

    /**
     * @brief Obstacle create an obstacle by setting qpairCoords, enumColor, enumType, enumStatus, dOrientation
     * @param[in] m_qpairCoords (QPair<double,double>)
     * @param[in] m_enumColor (Type of ObstacleType enum)
     * @param[in] m_enumType (Type from ObstacleColor enum)
     * @param[in] m_enumStatus (Type of ObstacleStatus enum)
     * @param[in] m_dOrientation (double)
     */
    Obstacle( QPair<double, double> m_qpairCoords,
              ObstacleColor m_enumColor,
              ObstacleType m_enumType,
              ObstacleStatus m_enumStatus,
              double m_dOrientation);

    /**
     * @brief ~Obstacle
     */
    virtual ~Obstacle();

    /**
     * @brief operator ==
     * @param b
     * @return if two obstacles are equal
     */
    bool operator == ( const Obstacle &b) const;

    /**
     * @brief operator <
     * @param b
     * @return which of the given obstacles is greater
     */
    bool operator < ( const Obstacle &b) const;

    /**
     * @brief mergeWith will merge two obstacles if possible
     * @param b (reference on an Obstacle)
     */
    void mergeWith (const Obstacle &newObst );

    /**
     * @brief getColor
     * @return the colorEnum
     */
    ObstacleColor getColor() const;

    /**
     * @brief setColor will set the obstacleColor to the given value.
     * @param[in] value (reference of ObstacleColor)
     */
    void setColor(const ObstacleColor &value);

    /**
     * @brief getType
     * @return the type Enum
     */
    ObstacleType getType() const;

    /**
     * @brief setType of ObstacleType enum
     * @param[in] value (reference ObstacleType)
     */
    void setType(const ObstacleType &value);

    /**
     * @brief getStatus
     * @return the ObstacleStatus enum
     */
    ObstacleStatus getStatus() const;

    /**
     * @brief setStatus the ObstacleStatus enum
     * @param[in] value (reference ObstacleStatus)
     */
    void setStatus(const ObstacleStatus &value);

    /**
     * @brief getCoords
     * @return a QPair<double,double> with the current coordinates
     */
    QPair<double, double> getCoords() const;

    /**
     * @brief setCoords
     * @param[in] value (QPair<double,double>)
     */
    void setCoords(const QPair<double, double> &value);

    /**
     * @brief getInitialized
     * @return the value of bInitialized
     */
    bool getInitialized() const;

    /**
     * @brief setInitialized of bInitialized
     * @param[in] value (bool)
     */
    void setInitialized(bool value);

    /**
     * @brief getLastUpdate-time
     * @return the last update time (QTime)
     */
    QTime getLastUpdate() const;

    /**
     * @brief setLastUpdate time
     * @param[in] value (reference of QTime)
     */
    void setLastUpdate(const QTime &value);

    /**
     * @brief getOrientation
     * @return the current orientation (double)
     */
    double getOrientation() const;

    /**
     * @brief setOrientation
     * @param[in] value
     */
    void setOrientation(double value);

    /**
     * @brief getDistanceTo
     * @param[in] obstacle
     * @return the distance to the obstacle
     */
    double getDistanceTo(const Obstacle &obstacle) const;

    /**
     * @brief getCPosition
     * @return
     */
    Position getPosition() const;

    /**
     * @brief setCPosition
     * @param value
     */
    void setPosition(const Position &value);

    /**
     * @brief isInGoalArea
     * @param[in] myGoalArea (FieldArea)
     * @return true if the obstacle is in the specified area
     */
    bool isInSpecifiedArea(FieldArea area) const;
};

#endif // OBSTACLE_H
