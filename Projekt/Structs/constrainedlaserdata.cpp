#include "constrainedlaserdata.h"


ConstrainedLaserData::ConstrainedLaserData()
{
    m_filteredDepth = QList<double>();
    m_rawDepths = QList<double>();
    m_angles = QList<double>();
}

ConstrainedLaserData::~ConstrainedLaserData()
{
    this->clearData();
}

void ConstrainedLaserData::clearData()
{
    m_filteredDepth.clear();
    m_rawDepths.clear();
    m_angles.clear();
}

QList<double> ConstrainedLaserData::filteredDepths() const
{
    return m_filteredDepth;
}

void ConstrainedLaserData::addFilteredDepth(const double &value)
{
    m_filteredDepth.append(value);
}

QList<double> ConstrainedLaserData::rawDepths() const
{
    return m_rawDepths;
}

void ConstrainedLaserData::addRawDepth(const double &value)
{
    m_rawDepths.append(value);
}

QList<double> ConstrainedLaserData::angles() const
{
    return m_angles;
}

void ConstrainedLaserData::addAngle(const double &value)
{
    m_angles.append(value);
}




