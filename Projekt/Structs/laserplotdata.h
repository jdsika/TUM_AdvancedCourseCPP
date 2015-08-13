#ifndef LASERPLOTDATA_H
#define LASERPLOTDATA_H

#include <QVector>
#include <QObject>

/**
 * @brief The LaserPlotData struct is the Datapacket for plotting the laser data.
 */
struct LaserPlotData
{
    enum DataTypeEnum { RAW,        /**< RAW datatype*/
                        REDUCED,    /**< REDUCED datatype*/
                        AVERAGE,    /**< AVERAGE datatype*/
                        MEDIAN,     /**< MEDIAN datatype*/
                        OBJECTS     /**< OBJECTS datatype*/
                      };

    /**
     * @brief LaserPlotData
     */
    LaserPlotData(){}

    /**
     * @brief LaserPlotData will initialise the struct with data and dataType
     * @param[in] data (QVector<double>)
     * @param[in] dataType (DataTypeEnum)
     */
    LaserPlotData(QVector<double> data, DataTypeEnum dataType) : data(data), dataType(dataType) {}

    /**
     * @brief LaserPlotData will initialise the struct with keys, values and dataType
     * @param[in] keys (QVector<double>)
     * @param[in] values (QVector<double>)
     * @param[in] dataType (DataTypeEnum)
     */
    LaserPlotData(QVector<double> keys, QVector<double> values, DataTypeEnum dataType) : angles(keys), data(values), dataType(dataType) {}

    // Objects data type
    QVector<double> angles; /**< angles of data */
    QVector<double> sizes; /**< sizes of data*/

    // general
    QVector<double> data;  /**< Databody*/
    DataTypeEnum dataType; /**< type enumeration*/
};

Q_DECLARE_METATYPE(LaserPlotData)


#endif // LASERPLOTDATA_H
