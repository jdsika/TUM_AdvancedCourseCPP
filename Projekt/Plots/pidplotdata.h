#ifndef PIDPLOTDATA_H
#define PIDPLOTDATA_H

#include <QList>
#include <QObject>

/**
 * @brief The PIDPlotData struct represents the data of the PID controler of the last n-time steps
 */
struct PIDPlotData
{
    QList<double> time;
    QList<double> winkelSoll;
    QList<double> winkelIst;
    QList<double> distanzSoll;
    QList<double> distanzIst;

};

Q_DECLARE_METATYPE(PIDPlotData)

#endif // PIDPLOTDATA_H
