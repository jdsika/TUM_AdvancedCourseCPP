#ifndef SENSOR_H
#define SENSOR_H

#include <QObject>
#include <QPair>
#include <QVector>
#include <QList>

#include <atomic>

#include "Structs/position.h"
#include "Structs/laserplotdata.h"
#include "Structs/filterparams.h"
#include "Structs/constrainedlaserdata.h"

class QMutex;
class QElapsedTimer;

enum class CamColor;

/**
 * @brief The SensorStates enum containing all needed states oth the HighLevelSensor
 */
enum class SensorStates
{
    WAIT,
    ORIENTATION,
    RECOGNITION,
    PRE_GAME_STATE,         // State vor ablauf der 90s. Kann raus gesprungen werden wenn >90s
    ORIENTATION_VALIDATION,
    COLOR_DETECTION_START,
    COLOR_DETECTION_WAIT
};

/**
 * @brief The SensorHighLevel class is responsible for the processing of the raw laser data and adds all objects to the MapData
 */
class SensorHighLevel : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief SensorHighLevel
     */
    SensorHighLevel();
    ~SensorHighLevel();

    static std::atomic_bool streamSensorEnabled;

    /**
     * @brief getState
     * @return the current state of the HighLevelSensor
     */
    SensorStates getState() const;

    /**
     * @brief setState
     * @param[in] value (SensorStates&)
     */
    void setState(const SensorStates &value);

    /**
     * @brief getTeamColor
     * @return
     */
    CamColor getTeamColor() const;

    /**
     * @brief setTeamColor
     * @param[in] value (CamColor)
     */
    void setTeamColor(const CamColor &value);

public Q_SLOTS:
    void slotGetLaserData(QVector<double> sensorData, Position positionSignal);
    void slotSetFilterParams(FilterParams cameraParams);
    void slotStartDetection(bool start);
    void slotColorDetected(CamColor color);
    //void slotPuckGrabbed(bool isGrabbed); //Direkt in der Map implementiert


Q_SIGNALS:
    void signalSendRobotControlParams(double velocity, double turnangle);
    void signalEmergencyStopEnabled( bool );
    void signalSendLaserData(LaserPlotData laserData);
    void signalSendOdometryData( Position finalPosition);
    void signalSendTeamColor( CamColor color);
    void signalStartColorDetection();
    void signalPlanNewPath();

private:
    /**
     * @brief avoideCollision will only
     * @param rawDepthsVector
     */
    void avoideCollision(QVector<double> &rawDepthsVector);

    /**
     * @brief recognition
     * @param objects
     * @param transmissionPosition
     */
    void recognition(QList<Position> &objects, Position &transmissionPosition);

    /**
     * @brief extractObjects
     * @param constrainedData
     * @return
     */
    QList<Position> extractObjects(const ConstrainedLaserData &constrainedData);

    /**
     * @brief constrainData
     * @param filteredDepthsVector
     * @param rawDepthsVector
     * @param constrainedData
     */
    void constrainData(const QVector<double> &filteredDepthsVector, const QVector<double> &rawDepthsVector, ConstrainedLaserData &constrainedData);

    /**
     * @brief calculateObjCenter
     * @param constrainedData
     * @param objectBeginn
     * @param objectEnd
     * @param tempState
     * @return
     */
    Position calculateObjCenter(const ConstrainedLaserData &constrainedData, int objectBeginn, int objectEnd, SensorStates &tempState);

    /**
     * @brief driveToPreposition
     * @param setUp
     */
    void driveToPreposition();


    /**
     * @brief puckGrabbed check if we grabbed the puck
     */
    void puckGrabbed();

    bool isInSlowTurn;                      /**< Bool needed for orientation validation */
    bool hadEmergency;                      /**< Is set by the old collision avoidance */
    bool targetsSet;                        /**< If the target is set on the color field */
    bool prepositionInitialized;            /**< Bool is true if the preposition position was initialized */
    double minAngle;                        /**< The min Angle the roboter should turn to*/// min and max angle to 180 deg rotation
    double maxAngle;                        /**< The max Angle the roboter should turn to*/
    double dummyAngleOffset;                /**< Angle difference between dummy and (unset) robot*/
    int counter;                            /**< Counter to collect senor data*/

    QElapsedTimer *timerWaitForValidColorFrame;          /**< Pointer to timer to recheck color frame*/
    QElapsedTimer *timerToAbondonColorDetection;          /**< Pointer to timer to abondon color checking*/
    QMutex *mutexState;                     /**< Pointer to mutex to get the state form outside*/
    SensorStates currentState;              /**< saves the current state*/
    QMutex *mutexFilterParameter;           /**< Pointer to mutex to set parameters by the GUI*/
    FilterParams filterParameter;           /**< All parameters set by the GUI are stored into filterParameter*/
    QMutex *mutexTeamColor;                 /**< Pointer to mutex to cummunicate with cam */
    CamColor teamColor;                     /**< saves the color of the team */
    Position transmissionPosition;          /**< The position set to vertify if seen objects are in the field */
    Position previousPosition;              /**< First Position detected, used for validation */
    Position currentPosition;               /**< Calculated Position of the robot*/
    Position dummyPosition;                 /**< Position of the Dummy. Set after first orientation and rotating with robot*/
    ConstrainedLaserData constrainedData;   /**< Sensor Data with out of field lieres removed*/
    QVector<Position> cPolePositions;       /**< Vector with Positions of the poles */
    QList<Position> previousObjects;        /**< A list which contains the objects from the last run*/
    QList<Position> currentObjects;         /**< A list which contains the objects from the current run*/
};

#endif // SENSOR_H
