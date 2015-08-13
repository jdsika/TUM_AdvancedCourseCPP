#ifndef CONSTRAINEDLASERDATA_H
#define CONSTRAINEDLASERDATA_H

#include <QList>

/**
 * @brief The ConstrainedLaserData class is a Datapacket containing the processed data from the
 * LowLevelSensor and is used for sharing references of the data through the SensorHighLevel
 */
class ConstrainedLaserData
{
public:
    /**
     * @brief ConstrainedLaserData
     */
    ConstrainedLaserData();

    ~ConstrainedLaserData();

    /**
     * @brief clearData does delete all the items collected in the internal lists
     */
    void clearData();

    /**
     * @brief filteredDepths
     * @return QList<double> of filtered laser data by the median filter
     */
    QList<double> filteredDepths() const;

    /**
     * @brief addFilteredDepth to the internal QList
     * @param[in] value (double)
     */
    void addFilteredDepth(const double &value);

    /**
     * @brief rawDepths
     * @return QList<double> of raw laser data from the sensor
     */
    QList<double> rawDepths() const;

    /**
     * @brief addRawDepth
     * @param[in] value (double)
     */
    void addRawDepth(const double &value);

    /**
     * @brief angles
     * @return QList<double> of corresponding angles to the constrained laser data
     */
    QList<double> angles() const;

    /**
     * @brief addAngle
     * @param[in] value (double)
     */
    void addAngle(const double &value);

private:
    QList<double> m_filteredDepth; /**< Containes the depths processed by the median filter*/
    QList<double> m_rawDepths; /**< Containes the depths directly received by the LowLevelSensor*/
    QList<double> m_angles; /**< Containes the corresponding angles to both depth lists*/

};

#endif // CONSTRAINEDLASERDATA_H
