#ifndef ACTORHIGHLEVEL_H
#define ACTORHIGHLEVEL_H

#include "Data/obstacle.h"
#include "Structs/pidplotdata.h"
#include "Structs/pidparams.h"
#include "Actor/spline.h"
#include <QMutex>

#include "atomic"

struct PathPlotData;

class QElapsedTimer;
class QTimer;

/**
 * @brief The StatePathProcessing enum
 * internal state-machine
 */
enum class StatePathProcessing { STOP, /**< first enum value, represents the stop state */
                                 RUNNING, /**< second enum value, represents the running and therefore pathrealising state*/
                                 RELEASE_PUCK,
                                 RELEASE_PUCK_FROM_PUSH,
                                 PUSH_AND_RELEASE_PUCK,
                                 GATHER_PUCK
                                 };

/**
 * @brief The PathRealizer class
 * This class is responsible for realising a given path (array of points)
 * It is required, that the robot did not drive backwards because it has not any sensors on its back.
 * The internal state-machine is called with an heartbeat signal (TIMER)
 */
class ActorHighLevel:public QObject
{
    Q_OBJECT

public:
    /**
     * @brief PathRealizer => default constructor intialising member variables with default values
     */
    ActorHighLevel();
    /**
      *@brief ~PathRealizer => default destructor which will clear the heap and delete other objects
      */
    ~ActorHighLevel();

    static std::atomic_bool streamPIDEnabled;

    /**
     * @brief getState
     * @return
     */
    static StatePathProcessing getState();

    /**
     * @brief setstate
     * @param newState
     */
    static void setState(const StatePathProcessing &newState);

    /**
     * @brief ignoreSignals to ignore all incomming signals which would start the PIDController
     */
    void ignoreSignals();

private:
    /**
     * @brief startPIDController => This method represents the internal state-machine implementing a PID-controller for
     * motion control.
     */
    void startPIDController();

public Q_SLOTS:
    /**
     * @brief slotUpdateWaypoints receive computed waypoints vom AI/pathplanning.
     * Will hard reset the waypoints if new waypoints are available.
     * @param[in] waypoints (QList of QPair of <double,double>) representing the given waypoints in (x,y).
     */
    void slotUpdateWaypoints(QList< QPair<double,double> > waypoints);

    /**
     * @brief slotReleasePuck, release the puck by moving backwards without pathplanning
     */
    void slotReleasePuck();

    /**
     * @brief slotPushAndReleasePuck, move forwards to push a puck on field line, than move the same distance backwards.
     * @param[in] m_releasePuckDistance (double)
     */
    void slotPushAndReleasePuck(double m_releasePuckDistance);

    /**
     * @brief slotGatherPuck, move forwards for gathering the puck
     */
    void slotGatherPuck();

    /**
     * @brief slotChangePIDParams => This method will change the PID-values caused by changing the values in the GUI.
     * @param[in] p (struct of double values) for changing the PID-controler values.
     */
    void slotChangePIDParams(PIDParams p);

Q_SIGNALS:
    /**
     * @brief signalSendRobotControlParams => This method will emit the new velocity and turnrate to the actorLowLevel class.
     * @param[in] velocity (double) m/s
     * @param[in] turnangle (double) rad/s
     */
    void signalSendRobotControlParams(double velocity, double turnangle);

    /**
     * @brief signalSplinePlot this method will emit a data-struct, to display the path in GUI
     * @param[in] pathPlotData (PathPlotData) an struct which included the values of the GUI tab to display the path.
     */
    void signalSplinePlot(PathPlotData pathPlotData);

    /**
     * @brief signalPIDPlot this method will emit a data-struct to display the current PID-values
     * @param[in] d (struct of QList<double>) an struct with data for the GUI plot of PID-values.
     */
    void signalPIDPlot(PIDPlotData d);

    /**
     * @brief signalReleaseDone wird gesendet, wenn RELEASE_PUCK fertig ist (auch der Fall wenn PUSH_AND_RELEASE_PUCK eingestellt war) oder GATHER_PUCK fertig ist
     */
    void signalPuckDone();

private Q_SLOTS:
    /**
     * @brief slotTimerSendPIDPlot this method will update the data in GUI.
     */
    void slotTimerSendPIDPlot();

private:  
    static QMutex mutexState; /**< Mutex to prohibit racing condition*/

    std::atomic_bool quitting;
    std::atomic_bool enabled;

    static StatePathProcessing state; /**< internal state-machine*/

    Position robotPosition;       /**< current position of the robot including: x, y and orientation*/
    Position releasePuckOrigin;   /**< current position of the robot at the time of the beginning of release puck including: x, y and orientation*/

    QList< QPair<double,double> > internalWP; /**< internal member-variable to save the received waypoints*/
    int numWP; /**< length of the internal waypoints */

    tkqt::spline splineX; /**< cubic hermite spline created of received waypoints in x direction */
    tkqt::spline splineY; /**< cubic hermite spline created of received waypoints in y direction */

    bool positionWasReached;

    // Sending PID data to plot
    QMutex *mutexPidHist; /**< Mutex to prohibit racing condition*/

    QTimer *timerPIDPlot; /**< update timer for the PID-plot*/
    qint64 timeOfStart; /**< ms since epoch, used to display time since program start in pid plot*/
    QList <double> pidHistTime; /**< Display the past n-secounds in GUI plot*/
    QList <double> pidHistWinkelSoll; /**< Display the past n-secounds in GUI plot*/
    QList <double> pidHistWinkelIst; /**< Display the past n-secounds in GUI plot*/
    QList <double> pidHistDistIst; /**< Display the past n-secounds in GUI plot*/
    QList <double> pidHistDistSoll; /**< Display the past n-secounds in GUI plot*/

    QElapsedTimer *elapsedTime; /**< member object for determination of new time delta*/

    double PID_A_P; /**< P-part of the angle-PID*/
    double PID_A_I; /**< I-part of the angle-PID*/
    double PID_A_D; /**< D-part of the angle-PID*/
    double PID_V_P; /**< P-part of the velocity-PID*/
    double PID_V_I; /**< I-part of the velocity-PID*/
    double PID_V_D; /**< D-part of the velocity-PID*/

    double lastDeltaA; /**< previous desired angle derivation*/
    double iDeltaA; /**< desired angle derivation*/
    double lastDeltaL; /**< pervious desired length derivation */
    double iDeltaL; /**< desiered length derivation*/
    double targetDistLast;
    double targetDistDiffLast;
    double additionalReleasePuckDistance;

    /**
     * @brief resetPIDtempVars
     */
    void resetPIDtempVars();

    /**
     * @brief lowPass => This method will lowpass filter the given data
     * @param[in] in (QVector<double>) vector of values
     * @param[in] alpha (double) weighting of previous values during iteration
     * @return QVector<double> as filtered values
     */
    QVector<double> lowPass(QVector<double> in, double alpha = 0);

    /**
     * @brief constrainAngle => This method is responsible to prohibit a phase-shift.
     * @param[in] inRad (double) angle in rad which should be checked
     * @return the corrected angle in rad as double
     */
    static const double constrainAngle(const double inRad);

    /**
     * @brief takeDimension => This method will reduce the dimensions of a given QVector of points.
     * @param[in] in (QVector<QPair<double,double>> multi-dimensional array of points
     * @param[in] dimension (int) the desired dimension which should returned.
     * @return an empty QVector<double> if length of QVector is equal to zero, otherwise the flatten (one dimensional) QVector<double>
     */
    static QVector<double> takeDimension(const QVector< QPair<double,double> > in, const int dimension);
};

#endif // ACTORHIGHLEVEL_H
