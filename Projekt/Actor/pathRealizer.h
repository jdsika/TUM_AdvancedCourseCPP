#ifndef PATHREALIZER_H
#define PATHREALIZER_H

#include <QTimer>
#include <QMutex>
#include <atomic>

#include "Data/obstacle.h"
#include "Plots/pathplotdata.h"
#include "Plots/pidplotdata.h"
#include "Actor/pidparams.h"
#include "Actor/spline.h"


class QElapsedTimer;

/**
 * @brief The PathRealizer class
 * This class is responsible for realising a given path (array of points)
 * It is required, that the robot did not drive backwards because it has not any sensors on its back.
 * The internal state-machine is called with an heartbeat signal (TIMER)
 */
class PathRealizer:public QObject
{
    Q_OBJECT

public:
    /**
     * @brief PathRealizer => default constructor intialising member variables with default values
     */
    PathRealizer();
    /**
      *@brief ~PathRealizer => default destructor which will clear the heap and delete other objects
      */
    ~PathRealizer();

    static std::atomic_bool streamPIDEnabled;

    /**
     * @brief The StatePathProcessing enum
     * internal state-machine
     */
    enum StatePathProcessing { STOP, /**< first enum value, represents the stop state */
                               RUNNING /**< second enum value, represents the running and therefore pathrealising state*/
                             };

public Q_SLOTS:
    /**
     * @brief slotUpdateWaypoints receive computed waypoints vom AI/pathplanning.
     * Will hard reset the waypoints if new waypoints are available.
     * @param[in] waypoints (QList of QPair of <double,double>) representing the given waypoints in (x,y).
     */
    void slotUpdateWaypoints(QList< QPair<double,double> > waypoints);

    /**
     * @brief slotMotionControl => This method represents the internal state-machine implementing a PID-controller for
     * motion control.
     */
    void slotMotionControl();

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

private Q_SLOTS:
    /**
     * @brief slotTimerSendPIDPlot this method will update the data in GUI.
     */
    void slotTimerSendPIDPlot();

private:

    StatePathProcessing state; /**< internal state-machine*/
    Obstacle robotObstacle; /**< current pose of the robot including, position in x, in y and orientation*/

    QList< QPair<double,double> > internalWP; /**< internal member-variable to save the received waypoints*/
    int numWP; /**< length of the internal waypoints */

    tkqt::spline splineX; /**< cubic hermite spline created of received waypoints in x direction */
    tkqt::spline splineY; /**< cubic hermite spline created of received waypoints in y direction */
    tkqt::spline velProfile; /**< from spline derived velocity profile caused by the waviness of the resulting spline */

    QTimer timerMotionControl; /**< internal heartbeat to call the controler*/

    // Sending PID data to plot
    QMutex pidHistMutex; /**< Mutex to prohibit racing condition*/
    QTimer timerPIDPlot; /**< update timer for the PID-plot*/
    qint64 timeOfStart; /**< ms since epoch, used to display time since program start in pid plot*/
    QList <double> pidHistTime, /**< Display the past n-secounds in GUI plot*/
    pidHistWinkelSoll, /**< Display the past n-secounds in GUI plot*/
    pidHistWinkelIst, /**< Display the past n-secounds in GUI plot*/
    pidHistDistIst, /**< Display the past n-secounds in GUI plot*/
    pidHistDistSoll; /**< Display the past n-secounds in GUI plot*/

    double splineProgress; /**< current position on spline */
    double maxWaviness; /**< maximal value of waviness */
    double integrationTime; /**< time integration*/
    QElapsedTimer* elapsedTime; /**< member object for determination of new time delta*/

    double periodMotionControl; /**< Time of one period => internal heartbeat*/
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

    /**
     * @brief lowPass => This method will lowpass filter the given data
     * @param[in] in (QVector<double>) vector of values
     * @param[in] alpha (double) weighting of previous values during iteration
     * @return QVector<double> as filtered values
     */
    QVector<double> lowPass(QVector<double> in, double alpha = 0);

    /**
     * @brief getVelocityProfile this method will return the internal velocity profile(weaviness) of a spline.
     * @return QVector<double> the velocity profile of a spline
     */
    QVector<double> getVelocityProfile();

    /**
     * @brief splineToQVector => This method will convert an tkqt::spline Obj into an QVector
     * @param[in] spline (tkqt::spline) spline based on a metric
     * @param[in] metric (QVector<double>) as distance measurement of the spline
     * @return QVector<double> the converted spline
     */
    QVector<double> splineToQVector(tkqt::spline spline, QVector<double> metric);

    /**
     * @brief getDistance => This function will calculate the distance between two points given as QPairs.
     * @param a (QPair<double,double>) first point in x and y
     * @param b (QPair<double,double>) second point in x and y
     * @return the distance between a and b
     */
    inline double getDistance(QPair<double,double> a, QPair<double,double> b){
        double dX = a.first-b.first;
        double dY = a.second-b.second;
        return sqrt(dX*dX+dY*dY);
    }

    // Constrain an angle to -PI...+PI. This might not be the fastest way to do it, but who cares.
    /**
     * @brief constrainAngle => This method is responsible to prohibit a phase-shift.
     * @param[in] inRad (double) angle in rad which should be checked
     * @return the corrected angle in rad as double
     */
    static const inline double constrainAngle(const double inRad){
        double outRad = inRad;
        while (outRad >  M_PI) outRad -= 2*M_PI;
        while (outRad < -M_PI) outRad += 2*M_PI;
        return outRad;
    }

    /**
     * @brief takeDimension => This method will reduce the dimensions of a given QVector of points.
     * @param[in] in (QVector<QPair<double,double>> multi-dimensional array of points
     * @param[in] dimension (int) the desired dimension which should returned.
     * @return an empty QVector<double> if length of QVector is equal to zero, otherwise the flatten (one dimensional) QVector<double>
     */
    static QVector<double> takeDimension(const QVector< QPair<double,double> > in, const int dimension);

};

#endif // PATHREALIZER_H
