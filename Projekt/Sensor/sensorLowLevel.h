#ifndef SENSORLOWLEVEL_H
#define SENSORLOWLEVEL_H

#include <QVector>
#include <QObject>
#include <QElapsedTimer>

#include <atomic>

#include "Structs/laserplotdata.h"
#include "Structs/position.h"

// Forward declare
namespace PlayerCc
{
class RangerProxy;
class Position2dProxy;
class PlayerClient;
}

/**
 * @brief The SensorLowLevel class is collecting odometry and laser data from the player client
 */
class SensorLowLevel : public QObject
{
    Q_OBJECT

    /**
     * @brief The SensorState enum for internal initialization of the class
     */
    enum SensorState {
            INIT, RUN
        };

public:
    /**
     * @brief SensorLowLevel
     */
    SensorLowLevel();
    ~SensorLowLevel();

    /**
     * @brief angleWeightedAverage will correctly interpolate between two angles of 0 to 2*M_PI rad
     * @param[in] angle1 (double&)
     * @param[in] weight1 (double&)
     * @param[in] angle2 (double&)
     * @param[in] weight2 (double&)
     * @return the interpolated and weighted new angle value
     */
    static double angleWeightedAverage(const double &angle1,
                                const double &weight1,
                                const double &angle2,
                                const double &weight2);

public Q_SLOTS:
    /**
     * @brief quit the internal endless loop for data collection
     */
    void quit();
    /**
     * @brief run starts the internal endless loop for data collection
     */
    void run();

Q_SIGNALS:
    /**
     * @brief signalLaserDataReady signal which sends the current laser data along with the interpolated position
     */
    void signalLaserDataReady(QVector<double>, Position);

    /**
     * @brief signalLaserPlotRaw signal for the raw laser data to show in the gui
     * @param laserPlotData for the GUI
     */
    void signalLaserPlotRaw(LaserPlotData laserPlotData);

    /**
     * @brief signalSimulationDetect signals when the LowLevelSensor has detected if it is a simulation or not
     */
    void signalSimulationDetect();

private:
    SensorState state; /**< holds the internal state of the LowLevelSensor*/

    PlayerCc::PlayerClient *robot; /**< pointer to the global static player client */
    PlayerCc::Position2dProxy *positionProxy; /**< position proxy for odometry data reading */
    PlayerCc::RangerProxy *laserProxy; /**< laser proxy for the laser data reading */

    QVector<double> laserData; /**< array which contains the received laser data */
    int laserArrayLength; /**< the legth determines if it is real(>360) or simulation(360) */

    QElapsedTimer elapsedTimer; /**< This timer is needed in order to interpolate the position*/

    Position previousOdometryPosition; /**< previous Odometry position */
    Position currentOdometryPosition; /**< current Odometry position */

    double previousOdometryTime; /**< previous Odometry timestamp */
    double currentOdometryTime; /**< current Odometry timestamp */
    double laserTime; /**< laser data timestamp */

    std::atomic_bool quitting; /**< is set to true via Signal&Slot if the no more laser data is needed */

    /**
     * @brief readSensorData will be called endlessly and block till it received new data
     */
    void readSensorData();
};

#endif // SENSORLOWLEVEL_H
