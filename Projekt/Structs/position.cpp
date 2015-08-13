#include "position.h"

#include "Data/define.h"

double Position::certainty() const
{
    return m_certainty;
}

void Position::setCertainty(double certainty)
{
    m_certainty = certainty;
}
double Position::x() const
{
    return m_x;
}

void Position::x(double value)
{
    m_x = value;
}
double Position::y() const
{
    return m_y;
}

void Position::y(double value)
{
    m_y = value;
}
double Position::rot() const
{
    return m_rot;
}

void Position::rot(double value)
{
    /// all values will be stored in a range from 0 to 2*M_PI
    while (value > 2* M_PI) value -= 2*M_PI;
    while (value < 0.0) value += 2*M_PI;

    m_rot = value;   
}
SizeType Position::sizeType() const
{
    return m_size;
}

void Position::sizeType(SizeType value)
{
    m_size = value;
}

Position::Position()
{
    m_x = std::numeric_limits<double>::quiet_NaN();
    m_y = std::numeric_limits<double>::quiet_NaN();
    m_rot = std::numeric_limits<double>::quiet_NaN();
    m_size = SizeType::UNKNOWN;
    m_certainty = 0.0;
}

Position::Position(double x, double y)
    : Position()
{
    m_x = x;
    m_y = y;
    m_certainty = 1.0;
}

Position::Position(QPair<double, double> qPairPosition)
    : Position(qPairPosition.first, qPairPosition.second)
{
    // nichts
}

Position::Position(double x, double y, double rot)
    : Position(x, y)
{
    /// all values will be stored in a range from 0 to 2*M_PI
    while (rot > 2* M_PI) rot -= 2*M_PI;
    while (rot < 0.0) rot += 2*M_PI;

    m_rot = rot;
}

Position::Position(double x, double y, double rot, double certainty)
    : Position(x, y, rot)
{
    m_certainty = certainty;
}

Position::Position(double x, double y, double rot, SizeType size)
    : Position(x, y, rot)
{
    m_size = size;
}

///\todo we may change this condition here
bool Position::operator ==(const Position &b) const
{
    return this->isConsimilarTo(b);
}

bool Position::isPositionInStartField() const
{
    if (this->x() < 0 ||
        this->x() > config::geoFieldWidth ||
        this->y() < 0 ||
        this->y() > (config::geoPol_1_2 + config::geoPol_2_3))
        return false;
    return true;
}

double Position::getDistanceTo(const Position &b) const
{
    // calculates the distance between the two positions
    double dX = this->x() - b.x();
    double dY = this->y() - b.y();

    return sqrt(dX*dX+dY*dY);
}

bool Position::isConsimilarTo(const Position &b, const double &tolerance) const
{
    double delta = this->getDistanceTo(b);

    // if the distance is within the tolerance it returns true
    return tolerance > delta;
}

bool Position::isConsimilarTo(const Position &b) const
{
    double delta = this->getDistanceTo(b);

    // if no tolerance value is given, the config value is used
    return config::obstacleCoordinateTolerance > delta;
}
