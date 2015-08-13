#ifndef POSITION_H
#define POSITION_H

#include <QObject>
#include <QPair>

/**
 * @brief The SizeType enum
 */
enum class SizeType {
    UNKNOWN,
    ROBOT,
    PUCK,
    POLE
};

/**
 * @brief The Position struct will represent the current pose of the robot
 */
class Position
{
private:
    double m_x; /**< position in x in m*/
    double m_y; /**< position in y in m*/
    double m_rot; /**< radian between 0 and 2*PI*/
    SizeType m_size; /**< how big an object at a position is*/
    double m_certainty; /**< how reliable a position is*/

public:
    /**
     * @brief x
     * @return
     */
    double x() const;

    /**
     * @brief x
     * @param[in] value (double)
     */
    void x(double value);

    /**
     * @brief y
     * @return
     */
    double y() const;

    /**
     * @brief y
     * @param[in] value (double)
     */
    void y(double value);

    /**
     * @brief rot
     * @return
     */
    double rot() const;

    /**
     * @brief rot
     * @param[in] value (double)
     */
    void rot(double value);

    /**
     * @brief sizeType
     * @return
     */
    SizeType sizeType() const;

    /**
     * @brief sizeType
     * @param[in] value (SizeType)
     */
    void sizeType(SizeType value);

    /**
     * @brief certainty
     * @return
     */
    double certainty() const;

    /**
     * @brief setCertainty
     * @param[in] value (certainty)
     */
    void setCertainty(double certainty);

    /**
     * @brief Position Default constructor
     */
    Position();

    /**
     * @brief Position
     * @param[in] x (double)
     * @param[in] y (double)
     */
    Position(double x, double y);

    /**
     * @brief Position
     * @param[in] qPairPosition (QPair<double,double>)
     */
    Position(QPair<double,double> qPairPosition);

    /**
     * @brief Position
     * @param[in] x (double)
     * @param[in] y (double)
     * @param[in] rot (double)
     */
    Position(double x, double y, double rot);

    /**
     * @brief Position constructor
     * @param[in] x (double)
     * @param[in] y (double)
     * @param[in] rot (double)
     * @param[in] certainty (double)
     */
    Position(double x, double y, double rot, double certainty);

    /**
     * @brief Position
     * @param[in] x (double)
     * @param[in] y (double)
     * @param[in] rot (double)
     * @param[in] size (SizeType)
     */
    Position(double x, double y, double rot, SizeType size);

    /**
     * @brief getDistanceTo
     * @param[in] b (Position&)
     * @return kartesian distance between both positions
     */
    double getDistanceTo(const Position &b) const;


    /**
     * @brief isSimilarPosition
     * @param[in] b (Position&)
     * @param[in] tolerance (double&)
     * @return true if the distance is in tolerance range
     */
    bool isConsimilarTo(const Position &b, const double &tolerance) const;

    /**
     * @brief isSimilarPosition
     * @param[in] b (Position&)
     * @return true if the distance is in tolerance range
     */
    bool isConsimilarTo(const Position &b) const;

    /**
     * @brief operator ==
     * @param[in] b (Position&)
     * @return
     */
    bool operator == (const Position &b) const;

    /**
     * @brief isPositionInStartField
     * @return true if the position is within the start area
     */
    bool isPositionInStartField() const;
};


Q_DECLARE_METATYPE(Position)

#endif // POSITION_H
