#ifndef SENSOR_H
#define SENSOR_H

#include <QObject>
#include <QPair>
#include <QVector>
#include <QVector3D>
#include <QMutex>
#include <QElapsedTimer>

#include <atomic>

#include "Data/obstacle.h"
#include "Plots/LaserPlotData.h"

enum class CamColor;

struct FilterParams
{
    int ObsFilterAnzahl, ObsFilterSchnitt, PosFilterAnzahl, PosFilterSchnitt;
};

Q_DECLARE_METATYPE(FilterParams)

enum class SensorStates
{
    WAIT,
    ORIENTATION,
    RECOGNITION,
    ORIENTATION_VALIDATION,
    COLOR_DETECTION_START,
    COLOR_DETECTION_WAIT
};

class SensorHighLevel : public QObject
{
    Q_OBJECT

public:
    //constructor & destructor
    SensorHighLevel();
    ~SensorHighLevel();

    static std::atomic_bool streamSensorEnabled;

    // getter/setter for internal SensorState
    SensorStates getState() const;
    void setState(const SensorStates &value);

    CamColor getTeamColor() const;
    void setTeamColor(const CamColor &value);

public Q_SLOTS:
    void getLaserData(QVector<double> sensorData);
    void getSonarData(QVector<double> sonarData);
    void slotSetFilterParams(FilterParams cameraParams);
    void slotStartDetection(bool start);
    void slotColorDetected(CamColor color);

Q_SIGNALS:

    void signalSendRobotControlParams(double velocity, double turnangle);
    void signalEmergencyStopEnabled( bool );
    void signalSendLaserData(LaserPlotData laserData);
    void sendOdometryData( Position );
    void signalSendTeamColor( CamColor color);
    void signalStartColorDetection();

private:
    bool hadEmergency;
    bool targetsSet;

    QElapsedTimer timeSinceStart;

    static QMutex mutexFilterParameter;
    FilterParams filterParameter;

private:
    bool recognition(QVector<QVector3D> &objects);

    QVector<QVector3D> extractObjects(QPair<QVector<double>, QVector<double> > &anglesAndDepths);

    QPair<QVector<double>, QVector<double> > constrainData(const QVector<double> &rawDepthsVector);

    QPair<double,double> calculateObjCenter(QPair<double,double> firstCoordinate, QPair<double,double> lastCoordinate);
    QPair<double,double> convertPolToGlobalCoordinates(QPair<double,double> coordinates);

    double distanceKartesisch(QPair<double,double> point_a, QPair<double,double> point_b);
    double distancePolar(QPair<double,double> point_a, QPair<double,double> point_b);

    static QMutex mutexState;
    SensorStates currentState;

    Position previousPosition;
    Position currentPosition;

    int counter;
    QVector<QVector<QVector3D> > collectorObj;

    void avoideCollision( QVector<double> &sensorData);

    // min and max angle to 180 deg rotation
    double minAngle;
    double maxAngle;

    //COLOR Detect
    static QMutex mutexTeamColor;
    CamColor teamColor;
};

#endif // SENSOR_H
