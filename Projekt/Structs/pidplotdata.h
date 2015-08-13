#ifndef PIDPLOTDATA_H
#define PIDPLOTDATA_H

#include <QList>
#include <QObject>

/**
 * @brief The PIDPlotData struct represents the data of the PID controler of the last n-time steps
 */
struct PIDPlotData
{
    QList<double> time;         /**< list of time*/
    QList<double> winkelSoll;   /**< list of reference angles*/
    QList<double> winkelIst;    /**< list of current angles*/
    QList<double> distanzSoll;  /**< list of reference distances*/
    QList<double> distanzIst;   /**< list of current distances*/
};

Q_DECLARE_METATYPE(PIDPlotData)

#endif // PIDPLOTDATA_H
