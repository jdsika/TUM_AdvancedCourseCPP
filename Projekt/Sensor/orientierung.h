#ifndef ORIENTIERUNG_H
#define ORIENTIERUNG_H

#include <QVector>

class Position;

/**
 * @brief The Orientation class try to compute the position and the orientation of the robot due to distance values
 * and angles from sensor data
 */
class Orientation
{

private:
    // hidden constructor
    /**
     * @brief Orientation hidden constructor
     */
    Orientation();
    // hidden destructor
    /**
      *@brief ~Orientation hidden destructor
      */
    ~Orientation();
    // hidden copy constructor
    /**
     * @brief Orientation as hidden copy constructor
     */
    Orientation(const Orientation &);

    // hidden assign operator
    /**
     * @brief operator =  we leave just the declarations, so the compiler will warn us
    // if we try to use those two functions by accident

     * @return assigned value
     */
    Orientation& operator=(const Orientation &);

public:
    /**
     * @brief beginOrientation will start the orientation phase and try to find known distances for localisation
     * @param[in] QList<Position> &objects
     * @return a position with a certain degree of ensureance.
     */
    static Position beginOrientation(QList<Position> &objects);

    /**
     * @brief distancePolar
     * @param[in] depthA (double&)
     * @param[in] angleA (double&)
     * @param[in] depthB (double&)
     * @param[in] angleB (double&)
     * @return the distance between two polar coordinates
     */
    static double distancePolar(const double &angleA, const double &depthA, const double &angleB, const double &depthB);

    /**
     * @brief distancePolar
     * @param[in] depthA (double&)
     * @param[in] depthB (double&)
     * @param[in] angleBetweenAB (double&)
     * @return the distance between two polar coordinates
     */
    static double distancePolar(const double &depthA, const double &depthB, const double &angleBetweenAB);

    /**
     * @brief angleBetweenAB
     * @param[in] depthA (double&)
     * @param[in] depthB (double&)
     * @param[in] distC (double&)
     * @return the angle between the distance a and distance b
     */
    static double angleBetweenAB(const double &depthA, const double &depthB, const double &distC);

    /**
     * @brief getGlobalPolarPosition
     * @param[in] relativePosition (realDepth, realAngle, radius, sizeType)
     * @param[in] robotPosition (x, y, rot )
     * @return a Position value which is transformed from the robot perspective to the global coordinate system
     */
    static Position getGlobalPolarPosition(const Position &relativePosition, const Position &robotPosition);

    /**
     * @brief checkObjectsOnLine
     * @param[in] erstesObjekt (Position&)
     * @param[in] zweitesObjekt (Position&)
     * @param[in] drittesObjekt (Position&)
     * @return true if the three objects are on a straight line within a small error
     */
    static bool checkObjectsOnLine(const Position &erstesObjekt, const Position &zweitesObjekt, const Position &drittesObjekt);

private:
    /**
     * @brief getGlobalPosition
     * @param poleA Position Pole 1 (depth (m), angle (rad) )
     * @param poleB Position Pole 2 (depth (m), angle (rad) )
     * @param poleC Position Pole 3 (depth (m), angle (rad) )
     * @return Roboterposition
     */
    static Position getGlobalPosition(const Position &poleA, const Position &poleB, const Position &poleC);
};



#endif // ORIENTIERUNG_H
