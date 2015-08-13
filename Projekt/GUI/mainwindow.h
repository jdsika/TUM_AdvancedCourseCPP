#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QPair>
#include <QElapsedTimer>

#include <opencv2/core/core.hpp> // cv::Mat

#include "Structs/logparams.h"
#include "Structs/laserplotdata.h"
#include "Structs/cameraparams.h"
#include "Structs/filterparams.h"

enum class CamColor;

struct PathPlotData;
struct PIDPlotData;

class Position;
class PIDParams;
class QCPGraph;
class PathPlanning;
class QCPColorMap;
class QCPCurve;
class QCPScatterStyle;
class QTimer;
class QGraphicsScene;


namespace Ui {
    class MainWindow;
    class QTimer;
}

/**
 * @brief The MainWindow class creates the GUI and connect user actions with programm functionalities
 * for displaying and recording gathered data.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public Q_SLOTS:
    /**
     * @brief slotLog will display logging messages
     * @param[in] html
     */
    void slotLog(QString html);

    /**
     * @brief slotLaserDisplay will display the gathered laserData
     * @param[in] laserData
     */
    void slotLaserDisplay(LaserPlotData laserData);

    /**
     * @brief updatePathDisplay will update the path information
     * @param[in] dataPacket
     */
    void updatePathDisplay(PathPlotData dataPacket);

    /**
     * @brief slotDisplayFrame will show given frames gathered by the cam
     * @param[in] mat
     */
    void slotDisplayFrame(cv::Mat mat);

    /**
     * @brief slotPIDPlot will plot gathered PID-informations
     * @param[in] d
     */
    void slotPIDPlot(PIDPlotData d);

    /**
     * @brief slotRestartTimerDisplay
     */
    void slotRestartTimerDisplay();

    /**
     * @brief slotUpdateColorLabel
     * @param[in] color
     */
    void slotUpdateColorLabel(CamColor color);

Q_SIGNALS:

    /**
     * @brief robotRemoteControllUpdate will send remote control parameter to lowLevelActor
     * @param[in] velocity in m/s
     * @param[in] degreeturn in rad/s
     */
    void robotRemoteControllUpdate(double velocity, double degreeturn);

    /**
     * @brief signalStartOrientation will emulate the signal from refree to start orientation
     * @param[in] change (bool)
     */
    void signalStartOrientation( bool change);

    /**
     * @brief updateRemoteOdometry
     *
     */
    void updateRemoteOdometry(Position);

    /**
     * @brief signalChangeCamParams will send the changed cam parameters
     * @param[in] cp (CameraParams)
     */
    void signalChangeCamParams(CameraParams cp);

    /**
     * @brief signalChangeFilterParams will emit the changed filter params
     * @param[in] cp (FilterParams)
     */
    void signalChangeFilterParams(FilterParams cp);

    /**
     * @brief signalChangePIDParams will emit the changed PID params
     * @param[in] p (PIDParams-Enum)
     */
    void signalChangePIDParams(PIDParams p);

    /**
     * @brief signalTestPuckRelease emit the signal for releasing the pucks
     */
    void signalTestPuckRelease();

    /**
     * @brief signalTestStartGame emit the signal to start the game (refree signal)
     */
    void signalTestStartGame();

    /**
     * @brief signalTestColorDetect emit the signal for color detection
     */
    void signalTestColorDetect();

public:
    /**
     * @brief setup will transmit the GUI params to created RoboThread
     */
    void setup();

private:
    Ui::MainWindow *ui;  /**< main user interface*/
    QGraphicsScene *map; /**< graphic for displaying the robot with pucks and poles*/
    QTimer *refreshtimer; /**< Timer to refresh the GUI*/

    QElapsedTimer timer; ///< Für Anzeige vergangener zeit

    /**
     * @brief DrawMap will draw the data from map to GUI
     */
    void drawMap();

    /**
     * @brief convertX the x values to screen resolution
     * @param[in] x (double)
     * @return the converted x values
     */
    int convertX(float x);

    /**
     * @brief convertY the y values to screen resolution
     * @param[in] y (double)
     * @return the converted y values
     */
    int convertY(float y);
    //double robotVelocity;
    //double robotTurnRate;
    QList<QPair<int, int> > TrackingMe; /**< tracking own robot*/
    QList<QPair<int, int> > TrackingYou; /**< of foe bot */

    // Sensor Display
    QCPGraph *graphLaserRaw;        /**< raw graph of laser data*/
    QCPGraph *graphLaserReduced;    /**< reduced graph laser data*/
    QCPGraph *graphLaserMedian;     /**< median filtered laser data*/
    QCPGraph *graphLaserObjects;    /**< recognised laser objects*/
    QCPScatterStyle* scatterLaserObjects; /**< Laser object as scatterplot*/

    // Path Display
    QCPColorMap* colorMap;              /**< create the colormap for heat map of path planning*/
    QCPScatterStyle* scatterRobotStyle; /**< Create the robot as scatter plot*/
    QCPScatterStyle* scatterWPStyle;  /**< Waypoints as scatter plot*/
    QCPGraph* graphRobotScatter;   /**< scatter graph for robot*/
    QCPGraph* graphWPScatter; /**< scatter graphf for waypoints */
    QCPCurve* graphSplineCurve; /**< create spline curve*/

    // Cam Tab
    QList<QColor> colors; /**< coler of the cam tabbed view*/
    /**
     * @brief sendCamParams send the cam params if button is pressed
     */
    void changeCamParams();

    // Log Tab
    /**
     * @brief changeLogParams chages the logging params if GUI values changed
     */
    void changeLogParams();

    // PID Tab
    /**
     * @brief changePIDParams will emit the new params for the PID controler
     */
    void changePIDParams();
    QCPGraph *graphPIDASoll, /**< desired PID angel*/
            *graphPIDAIst,  /**< current PID angel*/
            *graphPIDDSoll, /**< desired PID velocity*/
            *graphPIDDIst; /**< current PID velocity*/


private Q_SLOTS:

    /**
     * @brief slotSimulationDetect if the we noticed that we are in simulation
     */
    void slotSimulationDetect();

    /**
     * @brief refresh the GUI
     */
    void refresh();

    //Handcontroll Buttons
    /**
     * @brief forward button of remote control
     */
    void forward();

    /**
     * @brief back button of remote control
     */
    void back();

    /**
     * @brief left button of remote control
     */
    void left();

    /**
     * @brief right button of remote control
     */
    void right ();

    /**
     * @brief strongleft button of remote control
     */
    void strongleft();

    /**
     * @brief strongright button of remote control
     */
    void strongright();

    /**
     * @brief stop button of remote control
     */
    void stop();

    /**
     * @brief mousePressEvent if user clicked
     * @param[in] event (QMouseEvent)
     */
    void mousePressEvent(QMouseEvent *event);

    /**
     * @brief clearTargets will reset the target list of waypoints
     */
    void clearTargets();

    // Cam Tab
    /**
     * @brief on_cbStream_stateChanged emit the changed arguments for cam
     * @param[in] arg1 (int)
     */
    void on_cbStream_stateChanged(int arg1);

    /**
     * @brief on_camSourceSpin_valueChanged emit the changed arguments for cam
     * @param[in] arg1(int)
     */
    void on_camSourceSpin_valueChanged(int arg1);

    /**
     * @brief on_updateSpinner_valueChanged emit the changed arguments for cam
     * @param[in] arg1(int)
     */
    void on_updateSpinner_valueChanged(int arg1);

    /**
     * @brief on_sizeX_textChanged emit the changed arguments for cam
     * @param[in] arg1(QString)
     */
    void on_sizeX_textChanged(const QString &arg1);

    /**
     * @brief on_sizeY_textChanged emit the changed arguments for cam
     * @param[in] arg1(QString)
     */
    void on_sizeY_textChanged(const QString &arg1);


    // Log Tab
    /**
     * @brief on_logCBOther_stateChanged if Other combobox changed -> emit the changed logging values
     * @param[in] arg1(int)
     */
    void on_logCBOther_stateChanged(int arg1);

    /**
     * @brief on_logCBActor_stateChanged if Actor combobox changed -> emit the changed logging values
     * @param[in] arg1(int)
     */
    void on_logCBActor_stateChanged(int arg1);

    /**
     * @brief on_logCBAI_stateChanged if AI combobox changed -> emit the changed logging values
     * @param[in] arg1(int)
     */
    void on_logCBAI_stateChanged(int arg1);

    /**
     * @brief on_logCBData_stateChanged if Data combobox changed -> emit the changed logging values
     * @param[in] arg1(int)
     */
    void on_logCBData_stateChanged(int arg1);

    /**
     * @brief on_logCBSensor_stateChanged if Sensor combobox changed -> emit the changed logging values
     * @param[in] arg1(int)
     */
    void on_logCBSensor_stateChanged(int arg1);

    /**
     * @brief on_logCBPlot_stateChanged if Plot combobox changed -> emit the changed logging values
     * @param[in] arg1(int)
     */
    void on_logCBPlot_stateChanged(int arg1);

    /**
     * @brief on_logLevel_currentIndexChanged if logLevel changed -> emit the changed logging values
     * @param[in] index(int)
     */
    void on_logLevel_currentIndexChanged(int index);
    // Sensor Tab

    /**
     * @brief sendFilterParams, emit the gathered filter data
     */
    void changeFilterParams();
    // PID Tab
    /**
     * @brief on_spinPIDAP_valueChanged if angle PID P value changed -> emit the changed PID values
     * @param[in] arg1 (double)
     */
    void on_spinPIDAP_valueChanged(double arg1);

    /**
     * @brief on_spinPIDAI_valueChanged if angle PID I value changed -> emit the changed PID values
     * @param[in] arg1 (double)
     */
    void on_spinPIDAI_valueChanged(double arg1);

    /**
     * @brief on_spinPIDAD_valueChanged if angle PID D value changed -> emit the changed PID values
     * @param[in] arg1 (double)
     */
    void on_spinPIDAD_valueChanged(double arg1);

    /**
     * @brief on_spinPIDVP_valueChanged if velocity PID P value changed -> emit the changed PID values
     * @param[in] arg1 (double)
     */
    void on_spinPIDVP_valueChanged(double arg1);

    /**
     * @brief on_spinPIDVI_valueChanged if velocity PID I value changed -> emit the changed PID values
     * @param[in] arg1 (double)
     */
    void on_spinPIDVI_valueChanged(double arg1);

    /**
     * @brief on_spinPIDVD_valueChanged if velocity PID D value changed -> emit the changed PID values
     * @param[in] arg1 (double)
     */
    void on_spinPIDVD_valueChanged(double arg1);

    /**
     * @brief on_streamSensor_stateChanged check box if we want to see the sensor stream
     * @param[in] arg1 (int)
     */
    void on_streamSensor_stateChanged(int arg1);

    /**
     * @brief on_streamPath_stateChanged check box if we want to see the path stream
     * @param[in] arg1(int)
     */
    void on_streamPath_stateChanged(int arg1);

    /**
     * @brief on_streamPID_stateChanged check box if we want to see the PID-stream
     * @param[in] arg1 (int)
     */
    void on_streamPID_stateChanged(int arg1);

    /**
     * @brief on_streamLog_stateChanged check box if we want to see the Log-stream
     * @param[in] arg1(int)
     */
    void on_streamLog_stateChanged(int arg1);

    /**
     * @brief on_btn_ReleasePuck_clicked if button released the puck
     */
    void on_btn_ReleasePuck_clicked();

    /**
     * @brief on_btn_StartGame_clicked if botton to start the game is clicked
     */
    void on_btn_StartGame_clicked();

    /**
     * @brief on_spinBox_kernel_valueChanged if kernel size of spin box has changed
     * @param[in] arg1(int)
     */
    void on_spinBox_kernel_valueChanged(int arg1);

    /**
     * @brief on_refreshTime_valueChanged if refresh time spin box changed.
     * @param[in] arg1(int)
     */
    void on_refreshTime_valueChanged(int arg1);

    /**
     * @brief on_pushButton_StartOrientation_clicked if start orientation was clicked
     */
    void on_pushButton_StartOrientation_clicked();

    /**
     * @brief on_btnDetectColor_clicked if color detection started
     */
    void on_btnDetectColor_clicked();
};

#endif // MAINWINDOW_H
