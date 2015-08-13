#include "mainwindow.h"

#include <cmath>
#include <QDebug>
#include <QTimer>
#include <QGraphicsScene>

#include "Logging/log.h"
#include "Logging/LogParams.h"

#include "Plots/pathplotdata.h"
#include "Plots/pidplotdata.h"
#include "Plots/LaserPlotData.h"

#include "ui_mainwindow.h"

#include "Logging/log.h"
#include "Data/define.h"
#include "Data/mapdata.h"
#include "AI/pathplanning.h"
#include "Sensor/sensorhighlevel.h"
#include "Sensor/cam.h"
#include "Actor/actorhighlevel.h"
#include "Structs/pathplotdata.h"
#include "Structs/pidplotdata.h"
#include "Structs/laserplotdata.h"
#include "Structs/pidparams.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QCPScatterStyle* scatterLaserObjects = new QCPScatterStyle;
    QCPScatterStyle* scatterRobotStyle = new QCPScatterStyle;
    QCPScatterStyle* scatterWPStyle = new QCPScatterStyle;

    this->m_changeOrientation = false;

    // GUI window name
    setWindowTitle(tr("Zentrale - Team 7"));

    //create map
    map = new QGraphicsScene(this);
    map->setBackgroundBrush(QBrush(QColor("black")));
    ui->graphicsView->setScene(map);
    ui->graphicsView->scale(1,1);  //zoom map

    //Refresh timer
    refreshtimer = new QTimer(this);
    refreshtimer->setInterval(ui->refreshTime->value()); //Aktualisierungsrate aus der GUI in ms
    connect(refreshtimer, SIGNAL(timeout()), SLOT(refresh()));
    connect(ui->setRefresh, SIGNAL(clicked()), SLOT(setrefreshrate()));
    refreshtimer->start();  //Starten den Refresher, sonst geht garnix ;)

    //Handcontroll
    connect(ui->pushButton_forward, SIGNAL(clicked()), SLOT(forward()));
    connect(ui->pushButton_back, SIGNAL(clicked()), SLOT(back()));
    connect(ui->pushButton_left, SIGNAL(clicked()), SLOT(left()));
    connect(ui->pushButton_right, SIGNAL(clicked()), SLOT(right()));
    connect(ui->pushButton_stop, SIGNAL(clicked()), SLOT(stop()));
    connect(ui->pushButton_strongleft, SIGNAL(clicked()), SLOT(strongleft()));
    connect(ui->pushButton_strongright, SIGNAL(clicked()), SLOT(strongright()));
    connect(ui->pushButton_SetFilter, SIGNAL(clicked()), SLOT(changeFilterParams()));

    //Orientation Setup
    connect(ui->pushButton_Start, SIGNAL(clicked()), SLOT(orientationSetup()));

    // Clear targets
    connect(ui->pushButton_ClearTargets, SIGNAL(clicked()), SLOT(clearTargets()));

    //Set Combo Boxes
    ui->comboBox_SetType->addItem("Opponent", config::GUIopponent);
    ui->comboBox_SetType->addItem("Me", config::GUIself);
    ui->comboBox_SetType->addItem("Puck Opponent", config::GUIpuckOpponent);
    ui->comboBox_SetType->addItem("Puck Me", config::GUIpuckSelf);
    ui->comboBox_SetType->addItem("Puck Undef.", config::GUIpuckUndef);
    ui->comboBox_SetType->addItem("Target", config::GUItarget);
    ui->comboBox_SetType->setCurrentIndex(config::GUItarget); // default to target

    // Sensor Display
    graphLaserObjects = ui->laserPlot->addGraph();
    scatterLaserObjects->setSize(10); //px
    scatterLaserObjects->setShape(QCPScatterStyle::ssCircle);
    scatterLaserObjects->setBrush(QBrush(QColor("red")));
    scatterLaserObjects->setPen(QPen(Qt::NoPen));
    graphLaserObjects->setScatterStyle(*scatterLaserObjects);
    graphLaserObjects->setLineStyle(QCPGraph::lsNone);
    graphLaserObjects->setName("Objects");

    graphLaserRaw = ui->laserPlot->addGraph();
    graphLaserRaw->setName("Raw");
    graphLaserRaw->setPen(QPen(QBrush(QColor("black")),1));
    graphLaserReduced = ui->laserPlot->addGraph();
    graphLaserReduced->setName("Reduced");
    graphLaserReduced->setPen(QPen(QBrush(QColor("green")),5));
    graphLaserMedian = ui->laserPlot->addGraph();
    graphLaserMedian->setName("Median");
    graphLaserMedian->setPen(QPen(QBrush(QColor("blue")),1));

    ui->laserPlot->xAxis->setLabel("degree");
    ui->laserPlot->yAxis->setLabel("range");
    ui->laserPlot->xAxis->setRange(-90,90); // degrees
    ui->laserPlot->yAxis->setRange(0,8); // meter
    ui->laserPlot->legend->setVisible(true);
    ui->laserPlot->legend->setBorderPen(Qt::NoPen);
    ui->laserPlot->legend->setBrush(QBrush(QColor(0,0,0,128))); // transparent black legend bg
    ui->laserPlot->legend->setTextColor(QColor("white"));
    ui->laserPlot->legend->setFont(QFont("serif", 6));
    ui->laserPlot->legend->setIconSize(12,12);

    // Path Display
    colorMap = new QCPColorMap(ui->pathPlot->xAxis, ui->pathPlot->yAxis);
    ui->pathPlot->addPlottable(colorMap);
    // scatter graph um dinge auf der map einzublenden
    graphRobotScatter = ui->pathPlot->addGraph();
    scatterRobotStyle->setSize(15); //px
    scatterRobotStyle->setShape(QCPScatterStyle::ssSquare);
    scatterRobotStyle->setBrush(QBrush(QColor("red")));
    scatterRobotStyle->setPen(QPen(QBrush(QColor("black")),1));
    graphRobotScatter->setScatterStyle(*scatterRobotStyle);
    graphRobotScatter->setLineStyle(QCPGraph::lsNone);
    graphRobotScatter->setName("Robot");

    graphWPScatter = ui->pathPlot->addGraph();
    scatterWPStyle->setSize(5); //px
    scatterWPStyle->setShape(QCPScatterStyle::ssCircle);
    scatterWPStyle->setBrush(QBrush(QColor(127,127,127))); //gray
    scatterWPStyle->setPen(QPen(Qt::NoPen));
    graphWPScatter->setScatterStyle(*scatterWPStyle);
    graphWPScatter->setLineStyle(QCPGraph::lsNone);
    graphWPScatter->setName("Waypoints");

    graphSplineCurve = new QCPCurve(ui->pathPlot->xAxis, ui->pathPlot->yAxis);
    ui->pathPlot->addPlottable(graphSplineCurve);
    graphSplineCurve->setPen(QPen(QBrush(QColor(0,255,0)),2)); // green
    graphSplineCurve->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
    graphSplineCurve->setLineStyle(QCPCurve::lsLine);
    graphSplineCurve->setName("Spline Path");

    colorMap->setGradient(QCPColorGradient::gpThermal);
    ui->pathPlot->xAxis->setVisible(false);
    ui->pathPlot->yAxis->setVisible(false);
    ui->pathPlot->legend->setVisible(true);
    ui->pathPlot->legend->removeAt(0); // remove colorMap item
    ui->pathPlot->legend->setBorderPen(Qt::NoPen);
    ui->pathPlot->legend->setBrush(QBrush(QColor(0,0,0,128))); // transparent black legend bg
    ui->pathPlot->legend->setTextColor(QColor("white"));
    ui->pathPlot->legend->setFont(QFont("serif", 6));
    ui->pathPlot->legend->setIconSize(8,8);
    colorMap->data()->setSize(80,120);
    colorMap->data()->setRange(QCPRange(-0.5, 3.5), QCPRange(-0.5, 5.5));

    // Cam Tab
    graphicsScene_1 = new QGraphicsScene(this);
    graphicsScene_2 = new QGraphicsScene(this);
    graphicsScene_3 = new QGraphicsScene(this);
    graphicsScene_4 = new QGraphicsScene(this);
    graphicsScene_5 = new QGraphicsScene(this);
    graphicsScene_6 = new QGraphicsScene(this);

    ui->hue_1->setScene(graphicsScene_1);
    ui->hue_2->setScene(graphicsScene_2);
    ui->hue_3->setScene(graphicsScene_3);
    ui->hue_4->setScene(graphicsScene_4);
    ui->hue_5->setScene(graphicsScene_5);
    ui->hue_6->setScene(graphicsScene_6);
    colors.append(QColor::fromHsvF(140.0/360, 0.2, 0.2)); // green
    colors.append(QColor::fromHsvF(190.0/360, 0.8, 0.8));
    colors.append(QColor::fromHsvF(40.0/360, 0.2, 0.2)); // yellow
    colors.append(QColor::fromHsvF(70.0/360, 0.8, 0.8));
    colors.append(QColor::fromHsvF(200.0/360, 0.2, 0.2)); // blue
    colors.append(QColor::fromHsvF(250.0/360, 0.8, 0.8));
    ui->hue_1->setBackgroundBrush(QBrush(colors[0]));
    ui->hue_2->setBackgroundBrush(QBrush(colors[1]));
    ui->hue_3->setBackgroundBrush(QBrush(colors[2]));
    ui->hue_4->setBackgroundBrush(QBrush(colors[3]));
    ui->hue_5->setBackgroundBrush(QBrush(colors[4]));
    ui->hue_6->setBackgroundBrush(QBrush(colors[5]));

    // Log Tab
    ui->logLevel->addItem("Debug");
    ui->logLevel->addItem("Warning");
    ui->logLevel->addItem("Critical");
    ui->logLevel->addItem("Fatal");

    // PID Tab
    graphPIDASoll = ui->plotPID->addGraph(ui->plotPID->xAxis, ui->plotPID->yAxis);
    graphPIDASoll->setName("Winkel Soll");
    graphPIDASoll->setPen(QPen(QBrush(QColor("black")),1));
    graphPIDAIst = ui->plotPID->addGraph(ui->plotPID->xAxis, ui->plotPID->yAxis);
    graphPIDAIst->setName("Winkel Ist");
    graphPIDAIst->setPen(QPen(QBrush(QColor("red")),2));
    graphPIDDIst = ui->plotPID->addGraph(ui->plotPID->xAxis, ui->plotPID->yAxis);
    graphPIDDIst->setName("Dist Ist");
    graphPIDDIst->setPen(QPen(QBrush(QColor("green")),2));
    graphPIDDSoll = ui->plotPID->addGraph(ui->plotPID->xAxis, ui->plotPID->yAxis);
    graphPIDDSoll->setName("Dist Soll");
    graphPIDDSoll->setPen(QPen(QBrush(QColor("blue")),1));
    ui->plotPID->legend->setVisible(true);
    ui->plotPID->legend->setBorderPen(Qt::NoPen);
    ui->plotPID->legend->setBrush(QBrush(QColor(0,0,0,128))); // transparent black legend bg
    ui->plotPID->legend->setTextColor(QColor("white"));
    ui->plotPID->legend->setFont(QFont("serif", 6));
    ui->plotPID->legend->setIconSize(12,12);
    ui->plotPID->xAxis->setLabel("Zeit seit Programmstart (s)");

}

MainWindow::~MainWindow()
{
    ui->pathPlot->removePlottable(colorMap);

    delete ui;
    delete scatterLaserObjects;
    delete scatterRobotStyle;
    delete scatterWPStyle;

    delete map;
    delete colorMap;
    delete refreshtimer;
    delete graphSplineCurve;

    delete graphicsScene_1;
    delete graphicsScene_2;
    delete graphicsScene_3;
    delete graphicsScene_4;
    delete graphicsScene_5;
    delete graphicsScene_6;
}

// Log
void MainWindow::slotLog(QString html)
{
    ui->logView->appendHtml(html);
}

void MainWindow::setup()
{
    // Setup, after construction of Robothread class
    //orientationSetup();
    changeCamParams();
    changeFilterParams();
    changePIDParams();
    changeLogParams();
}

void MainWindow::refresh()
{
    drawMap();
}

/***********************************************************/
/****                 MAP DRAWING                       ****/
/***********************************************************/

void MainWindow::drawMap()
{

    map->clear();  //clears map

    //Define colours and shape
    QPen pen_opponent;
    pen_opponent.setWidth(1);
    pen_opponent.setColor(Qt::red);
    QPen pen_me;
    pen_me.setWidth(1);
    pen_me.setColor(Qt::green);
    QPen pen_dummy;
    pen_dummy.setWidth(1);
    pen_dummy.setColor(Qt::cyan);
    QPen pen_ziel;
    pen_ziel.setWidth(1);
    pen_ziel.setColor(Qt::gray);
    QPen pen_pole;
    pen_pole.setWidth(1);
    pen_pole.setColor(Qt::darkGreen);
    QPen pen_puckSelf;
    pen_puckSelf.setWidth(1);
    pen_puckSelf.setColor(Qt::green);
    QPen pen_puckOpponent;
    pen_puckOpponent.setWidth(1);
    pen_puckOpponent.setColor(Qt::red);
    QPen pen_puckUndef;
    pen_puckUndef.setWidth(1);
    pen_puckUndef.setColor(Qt::gray);
    QPen pen_field;
    pen_field.setWidth(1);
    pen_field.setColor(Qt::lightGray);
    QPen pen_field_secondarylines;
    pen_field_secondarylines.setWidth(1);
    pen_field_secondarylines.setColor(Qt::darkGray);
    QBrush brush_goal_blue(QColor(0, 0, 0xFF, 0x80));
    QBrush brush_goal_yellow(QColor(0xFF, 0xFF, 0, 0x80));
    QBrush brush_goal_none(QColor(0xFF, 0xFF, 0xFF, 0x40));

    /***********************************************************/
    /****              Draw MAP                             ****/
    /***********************************************************/

    //qDebug() << "DRAW";

    /* #####    FIELD   #####   */
    map->addRect(config::BORDERSIDE_CM,
                 config::BORDERTOP_CM,
                 config::FIELDWIDTH_CM,
                 config::FIELDHEIGHT_CM,
                 pen_field);//->setPen(normal);

    /* ##### Fieldmarkings ##### */

    QBrush brushTop, brushBottom;
    if(MapData::getTeamColor() == CamColor::YELLOW)
    {
        brushBottom = brush_goal_blue;
        brushTop = brush_goal_yellow;
    } else if(MapData::getTeamColor() == CamColor::BLUE)
    {
        brushTop = brush_goal_blue;
        brushBottom = brush_goal_yellow;
    } else
    {
        brushTop = brush_goal_none;
        brushBottom = brush_goal_none;
    }
    map->addRect(100+config::BORDERSIDE_CM,(config::FIELDHEIGHT_CM+config::BORDERTOP_CM-(config::POL_1_2*100)-42), 100, 42, pen_field_secondarylines, brushTop);
    map->addRect(100+config::BORDERSIDE_CM,(config::BORDERTOP_CM+config::POL_1_2*100), 100, 42, pen_field_secondarylines, brushBottom);
    map->addLine(
                config::BORDERSIDE_CM,
                (config::POL_1_2+config::POL_2_3+config::POL_3_4)*100+config::BORDERTOP_CM,
                config::BORDERSIDE_CM+(config::POL_1_14)*100,
                (config::POL_1_2+config::POL_2_3+config::POL_3_4)*100+config::BORDERTOP_CM,
                pen_field_secondarylines
                );
    map->addLine(
                config::BORDERSIDE_CM,
                (config::POL_1_2+config::POL_2_3)*100+config::BORDERTOP_CM,
                config::BORDERSIDE_CM+(config::POL_1_14)*100,
                (config::POL_1_2+config::POL_2_3)*100+config::BORDERTOP_CM,
                pen_field_secondarylines
                );
    map->addLine(
                config::BORDERSIDE_CM,
                (config::POL_1_2+config::POL_2_3+config::POL_3_4+config::POL_3_4)*100+config::BORDERTOP_CM,
                config::BORDERSIDE_CM+(config::POL_1_14)*100,
                (config::POL_1_2+config::POL_2_3+config::POL_3_4+config::POL_3_4)*100+config::BORDERTOP_CM,
                pen_field_secondarylines
                );
    map->addLine(
                config::BORDERSIDE_CM,
                (config::POL_1_2+config::POL_2_3+config::POL_3_4+config::POL_3_4+config::POL_2_3)*100+config::BORDERTOP_CM,
                config::BORDERSIDE_CM+(config::POL_1_14)*100,
                (config::POL_1_2+config::POL_2_3+config::POL_3_4+config::POL_3_4+config::POL_2_3)*100+config::BORDERTOP_CM,
                pen_field_secondarylines
                );
    map->addLine(
                config::BORDERSIDE_CM,
                (config::POL_1_2)*100+config::BORDERTOP_CM,
                config::BORDERSIDE_CM+(config::POL_1_14)*100,
                (config::POL_1_2)*100+config::BORDERTOP_CM,
                pen_field_secondarylines
                );


    /* ##### Tracking ##### */
    if(ui->checkBox_showTrackingSelf->isChecked() && (!TrackingMe.isEmpty()))
    {
        for(int i=0;i<(TrackingMe.size()-1);i++)
        {
            map->addRect((TrackingMe.at(i).first)-2,(TrackingMe.at(i).second)-2,4,4,pen_me);  // Positionsdarstellung Zeppelin - Rechtecke
        }
    }

    if(ui->checkBox_showTrackingOpponent->isChecked() && (!TrackingYou.isEmpty()))
    {
        for(int i=0;i<(TrackingYou.size()-1);i++)
        {
            map->addRect((TrackingYou.at(i).first)-2,(TrackingYou.at(i).second)-2,4,4,pen_opponent);  // Positionsdarstellung Zeppelin - Rechtecke
        }
    }
    /*  #####    Begrenzungspfosten #####   */
    QList<Obstacle> bufPole;
    bufPole = MapData::getObstacle(ObstacleType::POLE);
    if(!bufPole.isEmpty())
    {
        QList<Obstacle>::const_iterator i;
        for(i = bufPole.constBegin(); i != bufPole.constEnd(); ++i)
        {
            if((*i).getInitialized())
            {
                int x = convertX((*i).getCoords().first);
                int y = convertX((*i).getCoords().second);
                map->addEllipse(x - config::POLESIZE_CM/2,
                                y - config::POLESIZE_CM/2,
                                config::POLESIZE_CM,
                                config::POLESIZE_CM,
                                pen_pole);  // Positionsdarstellung Begrenzungspfosten
            }
        }
    }

    /*  #####    Pucks  #####   */
    QList<Obstacle> bufPucks;
    bufPucks = MapData::getObstacle(ObstacleType::PUCK);
    if(!bufPucks.isEmpty())
    {
        for(int i=0;i<(bufPucks.size());i++)
        {
            if(bufPucks.at(0).getInitialized())
            {
                int x = convertX(bufPucks.at(i).getCoords().first);
                int y = convertY(bufPucks.at(i).getCoords().second);
                switch(bufPucks.at(i).getColor())
                {
                case ObstacleColor::UNIDENTIFIED:
                    map->addEllipse(x - config::PUCKSIZE_CM/2,
                                    y - config::PUCKSIZE_CM/2,
                                    config::PUCKSIZE_CM,
                                    config::PUCKSIZE_CM,
                                    pen_puckUndef);
                    break;
                case ObstacleColor::ME:
                    map->addEllipse(x - config::PUCKSIZE_CM/2,
                                    y - config::PUCKSIZE_CM/2,
                                    config::PUCKSIZE_CM,
                                    config::PUCKSIZE_CM,
                                    pen_puckSelf);
                    break;
                case ObstacleColor::OPPONENT:
                    map->addEllipse(x - config::PUCKSIZE_CM/2,
                                    y - config::PUCKSIZE_CM/2,
                                    config::PUCKSIZE_CM,
                                    config::PUCKSIZE_CM,
                                    pen_puckOpponent);
                    break;
                default:
                    break;
                }
            }
        }
    }

    /*  #####    Robot Me #####   */
    QList<Obstacle> bufMe;
    bufMe = MapData::getObstacle(ObstacleType::ME);
    if(!bufMe.isEmpty())
    {
        if(bufMe.first().getInitialized())
        {
            int x = convertX(bufMe.first().getCoords().first);
            int y = convertY(bufMe.first().getCoords().second);
            map->addEllipse(x - config::ROBOSIZE_CM/2,
                            y - config::ROBOSIZE_CM/2,
                            config::ROBOSIZE_CM,
                            config::ROBOSIZE_CM
                            ,pen_me);

            double bufMeOrientation = bufMe.first().getOrientation();
            if(!std::isnan(bufMeOrientation))
            {
                double delta_x = config::ROBOSIZE_CM*cos(bufMeOrientation);
                double delta_y = config::ROBOSIZE_CM*-sin(bufMeOrientation);
                map->addLine(x,y,x + delta_x,y + delta_y,pen_me);

                double laser_x = 1000*cos(M_PI/2+bufMeOrientation);
                double laser_y = 1000*-sin(M_PI/2+bufMeOrientation);
                map->addLine(x,y,x + laser_x,y + laser_y,pen_puckUndef);
                laser_x = 1000*cos(M_PI*3/2+bufMeOrientation);
                laser_y = 1000*-sin(M_PI*3/2+bufMeOrientation);
                map->addLine(x,y,x + laser_x,y + laser_y,pen_puckUndef);
            }

            QPair<int, int> track;
            track.first=x;
            track.second=y;
            TrackingMe.prepend(track);
        }
    }

    /*  #####    Robot Dummy #####   */
    QList<Obstacle> bufDummy;
    bufDummy = MapData::getObstacle(ObstacleType::DUMMY);
    if(!bufDummy.isEmpty())
    {
        if(bufDummy.first().getInitialized())
        {
            int x = convertX(bufDummy.first().getCoords().first);
            int y = convertY(bufDummy.first().getCoords().second);
            map->addEllipse(x - config::ROBOSIZE_CM/2,
                            y - config::ROBOSIZE_CM/2,
                            config::ROBOSIZE_CM,
                            config::ROBOSIZE_CM
                            ,pen_dummy);

            double dummyOrientation = bufDummy.first().getOrientation();
            if(!std::isnan(dummyOrientation))
            {
                double delta_x = config::ROBOSIZE_CM*cos(dummyOrientation);
                double delta_y = config::ROBOSIZE_CM*-sin(dummyOrientation);
                map->addLine(x,y,x + delta_x,y + delta_y,pen_dummy);
            }
        }
    }

    /*  #####    Robot You #####   */
    QList<Obstacle> bufYou;
    bufYou = MapData::getObstacle(ObstacleType::OPPONENT);
    if(!bufYou.isEmpty())
    {
        if(bufYou.first().getInitialized())
        {
            int x = convertX(bufYou.first().getCoords().first);
            int y = convertY(bufYou.first().getCoords().second);
            map->addEllipse(x - config::ROBOSIZE_CM/2,
                            y - config::ROBOSIZE_CM/2,
                            config::ROBOSIZE_CM,
                            config::ROBOSIZE_CM,
                            pen_opponent);

            QPair<int, int> track;
            track.first=x;
            track.second=y;
            TrackingYou.prepend(track);
        }
    }

    /*  #####    Target #####   */
    QList<Obstacle> bufTarget;
    bufTarget = MapData::getObstacle(ObstacleType::TARGET);
    if(!bufTarget.isEmpty())
    {
        for(int i=0;i<(bufTarget.size());i++)
        {
            if(bufTarget.at(i).getInitialized())
            {
                int x = convertX(bufTarget.at(i).getCoords().first);
                int y = convertY(bufTarget.at(i).getCoords().second);
                map->addEllipse(x - config::TARGETSIZE_CM/2,
                                y - config::TARGETSIZE_CM/2,
                                config::TARGETSIZE_CM,
                                config::TARGETSIZE_CM,
                                pen_ziel);

                if(!std::isnan(bufTarget.at(i).getOrientation()))
                {
                    double delta_x = config::TARGETSIZE_CM*cos(bufTarget.at(i).getOrientation());
                    double delta_y = config::TARGETSIZE_CM*-sin(bufTarget.at(i).getOrientation());

                    map->addLine(x,y,x+delta_x,y+delta_y,pen_ziel);
                }
            }
        }
    }
    /*  #####    UnidentTarget #####   */
    QList<Obstacle> unidentObj;
    unidentObj = MapData::getObstacle(ObstacleType::UNIDENTIFIED);
    //qDebug()<<"Obj list size"<<unidentObj.size();
    if(!unidentObj.isEmpty())
    {
        for(int i=0;i<(unidentObj.size());i++)
        {
            if(unidentObj.at(i).getInitialized())
            {
                int x = convertX(unidentObj.at(i).getCoords().first);
                int y = convertY(unidentObj.at(i).getCoords().second);
                map->addEllipse(x - config::TARGETSIZE_CM/2,
                                y - config::TARGETSIZE_CM/2,
                                config::TARGETSIZE_CM,
                                config::TARGETSIZE_CM,
                                pen_puckUndef);

                if(!std::isnan(unidentObj.at(i).getOrientation()))
                {
                    double delta_x = config::TARGETSIZE_CM*cos(unidentObj.at(i).getOrientation());
                    double delta_y = config::TARGETSIZE_CM*-sin(unidentObj.at(i).getOrientation());

                    map->addLine(x,y,x+delta_x,y+delta_y,pen_puckUndef);
                }
            }
        }
    }



    /* ##### Fieldmarkings ##### */
    map->addRect(100+config::BORDERSIDE_CM,(config::FIELDHEIGHT_CM+config::BORDERTOP_CM-(config::POL_1_2*100)-42), 100, 42, pen_field_secondarylines);
    map->addRect(100+config::BORDERSIDE_CM,(config::BORDERTOP_CM+config::POL_1_2*100), 100, 42, pen_field_secondarylines);
    map->addLine(
                config::BORDERSIDE_CM,
                (config::POL_1_2+config::POL_2_3+config::POL_3_4)*100+config::BORDERTOP_CM,
                config::BORDERSIDE_CM+(config::POL_1_14)*100,
                (config::POL_1_2+config::POL_2_3+config::POL_3_4)*100+config::BORDERTOP_CM,
                pen_field_secondarylines
                );
    map->addLine(
                config::BORDERSIDE_CM,
                (config::POL_1_2+config::POL_2_3)*100+config::BORDERTOP_CM,
                config::BORDERSIDE_CM+(config::POL_1_14)*100,
                (config::POL_1_2+config::POL_2_3)*100+config::BORDERTOP_CM,
                pen_field_secondarylines
                );
    map->addLine(
                config::BORDERSIDE_CM,
                (config::POL_1_2+config::POL_2_3+config::POL_3_4+config::POL_3_4)*100+config::BORDERTOP_CM,
                config::BORDERSIDE_CM+(config::POL_1_14)*100,
                (config::POL_1_2+config::POL_2_3+config::POL_3_4+config::POL_3_4)*100+config::BORDERTOP_CM,
                pen_field_secondarylines
                );
    map->addLine(
                config::BORDERSIDE_CM,
                (config::POL_1_2+config::POL_2_3+config::POL_3_4+config::POL_3_4+config::POL_2_3)*100+config::BORDERTOP_CM,
                config::BORDERSIDE_CM+(config::POL_1_14)*100,
                (config::POL_1_2+config::POL_2_3+config::POL_3_4+config::POL_3_4+config::POL_2_3)*100+config::BORDERTOP_CM,
                pen_field_secondarylines
                );
    map->addLine(
                config::BORDERSIDE_CM,
                (config::POL_1_2)*100+config::BORDERTOP_CM,
                config::BORDERSIDE_CM+(config::POL_1_14)*100,
                (config::POL_1_2)*100+config::BORDERTOP_CM,
                pen_field_secondarylines
                );


    /* ##### Tracking ##### */
    if(ui->checkBox_showTrackingSelf->isChecked() && (!TrackingMe.isEmpty()))
    {
        for(int i=0;i<(TrackingMe.size()-1);i++)
        {
            map->addRect((TrackingMe.at(i).first)-2,(TrackingMe.at(i).second)-2,4,4,pen_me);  // Positionsdarstellung Zeppelin - Rechtecke
        }
    }

    if(ui->checkBox_showTrackingOpponent->isChecked() && (!TrackingYou.isEmpty()))
    {
        for(int i=0;i<(TrackingYou.size()-1);i++)
        {
            map->addRect((TrackingYou.at(i).first)-2,(TrackingYou.at(i).second)-2,4,4,pen_opponent);  // Positionsdarstellung Zeppelin - Rechtecke
        }
    }

}

void MainWindow::setrefreshrate()
{

    refreshtimer->setInterval(ui->refreshTime->value());
    qDebug() << "Refreshrate geändert:" << ui->refreshTime->value();
}

int MainWindow::convertX(float x)
{
    float buf =0;
    int xScreen=0;
    buf= x * 100;
    xScreen = (int)(buf+0.5) + config::BORDERSIDE_CM;
    return xScreen;
}

int MainWindow::convertY(float y)
{
    float buf = 0;
    int yScreen = 0;
    buf = config::FIELDHEIGHT_CM-(y*100);
    yScreen= (int)(buf+0.5) + config::BORDERTOP_CM;
    return yScreen;

}

void MainWindow::slotLaserDisplay(LaserPlotData laserData)
{
    QVector<double> key = QVector<double>(laserData.data.size());
    for(int i=0; i<laserData.data.size(); ++i){
        key[i] = -90.0 + 180.0*i/(laserData.data.size()-1);
    }

    switch (laserData.dataType){
    case LaserPlotData::RAW:
    {
        graphLaserRaw->setData(key, laserData.data);
        break;
    }
    case LaserPlotData::REDUCED:
    {
        QVector<double> angles = laserData.angles;
        QVector<double> values = laserData.data;
        // Angles auf Lücken >5° überprüfen
        for(int i=1; i<angles.length(); ++i){
            if(fabs(angles[i-1] - angles[i]) > 2){
                angles.insert(i, angles[i-1]+0.1);
                values.insert(i, std::numeric_limits<double>::quiet_NaN());
                i++;
                angles.insert(i, angles[i-1]-0.1);
                values.insert(i, std::numeric_limits<double>::quiet_NaN());
                i++;
            }
        }
        graphLaserReduced->setData(angles, values);
        break;
    }
    case LaserPlotData::AVERAGE:
    {
        graphLaserReduced->setData(key, laserData.data);
        break;
    }
    case LaserPlotData::MEDIAN:
    {
        graphLaserMedian->setData(key, laserData.data);
        break;
    }
    case LaserPlotData::OBJECTS:
    {
        graphLaserObjects->setData(laserData.angles, laserData.data);
        break;
    }
    default:
        qWarning() << "LaserPlotData Datatype" << static_cast<int>(laserData.dataType) << "unknown";
    }

    ui->laserPlot->replot();

}

void MainWindow::updatePathDisplay(PathPlotData dataPacket)
{
    switch(dataPacket.dataType){
    case PathPlotData::SPLINE: // from PathRealizer
    {
        graphSplineCurve->setData(dataPacket.splineX, dataPacket.splineY);
    }
        break;
    default:
    case PathPlotData::WAYPOINTS: // from PathPlaning
    {

        // Potential-map
        colorMap->data()->fill(-1.0);
        // fill the map with known values
        Q_FOREACH(PathPlotData::Point p, dataPacket.data){
            if(std::isinf(p.value) || std::isnan(p.value)) p.value = 0;
            colorMap->data()->setData(p.x, p.y, p.value);
        }
        // interpolate clean spots on the map. this takes almost no time (<1ms)
        for(int x=0; x<80; ++x){
            for(int y=0; y<120; ++y){
                if(colorMap->data()->cell(x,y) == -1.0){
                    double accumulate = 0;
                    double numAccumulations = 0;
                    if(x > 0 && colorMap->data()->cell(x-1,y) != -1){
                        accumulate += colorMap->data()->cell(x-1,y);
                        numAccumulations++;
                    }
                    if(x < 79 && colorMap->data()->cell(x+1,y) != -1){
                        accumulate += colorMap->data()->cell(x+1,y);
                        numAccumulations++;
                    }
                    if(y > 0 && colorMap->data()->cell(x,y-1) != -1){
                        accumulate += colorMap->data()->cell(x,y-1);
                        numAccumulations++;
                    }
                    if(y < 119 && colorMap->data()->cell(x,y+1) != -1){
                        accumulate += colorMap->data()->cell(x,y+1);
                        numAccumulations++;
                    }
                    double average = (numAccumulations)? accumulate / numAccumulations : 0;
                    colorMap->data()->setCell(x,y,average);
                }
            }
        }
        colorMap->rescaleDataRange();

        // scatter plot fuer robot und waypoints
        QVector<double> robotX(1), robotY(1), wpX(dataPacket.waypoints.size()), wpY(dataPacket.waypoints.size());
        robotX[0] = dataPacket.robot.first;
        robotY[0] = dataPacket.robot.second;
        for(int i=0; i<dataPacket.waypoints.size(); ++i){
            wpX[i] = dataPacket.waypoints[i].first;
            wpY[i] = dataPacket.waypoints[i].second;
        }
        graphRobotScatter->setData(robotX, robotY);
        graphWPScatter->setData(wpX, wpY);

        // remove old spline data if no spline is sent
        if(dataPacket.waypoints.size() == 0)
            graphSplineCurve->clearData();

        // scale axes
        ui->pathPlot->xAxis->setRange(QCPRange(-0.5, 3.5));
        ui->pathPlot->yAxis->setRange(QCPRange(-0.5, 5.5));

        // adjust plot widget size
        int width = 4.0 * ui->pathPlot->height() / 6.0;
        int maxwidth = ui->pathHorizontalLayout->geometry().width();
        width = (width > maxwidth)? maxwidth : width;
        ui->pathPlot->setFixedWidth(width);
    }
        break;
    }

    // Plot redraw
    ui->pathPlot->replot();

}

void MainWindow::slotDisplayFrame(cv::Mat mat)
{
    ui->camView->showImage(mat);
}

void MainWindow::slotPIDPlot(PIDPlotData d)
{
    if(d.time.length() == 0) return;

    static const double pidPlotHistorySize = 30.0; // sec

    // Alte Daten entfernen
    graphPIDAIst->removeDataBefore( d.time.last() - pidPlotHistorySize);
    graphPIDASoll->removeDataBefore(d.time.last() - pidPlotHistorySize);
    graphPIDDIst->removeDataBefore( d.time.last() - pidPlotHistorySize);
    graphPIDDSoll->removeDataBefore(d.time.last() - pidPlotHistorySize);

    // Neue Daten anhaengen
    graphPIDASoll->addData(d.time.toVector(), d.winkelSoll.toVector());
    graphPIDAIst->addData( d.time.toVector(), d.winkelIst.toVector());
    graphPIDDSoll->addData(d.time.toVector(), d.distanzSoll.toVector());
    graphPIDDIst->addData( d.time.toVector(), d.distanzIst.toVector());

    ui->plotPID->yAxis->rescale(); // ist das noetig?
    //    ui->plotPID->yAxis->setRange(ui->plotPID->yAxis->range().lower - 2.0, ui->plotPID->yAxis->range().upper + 2.0);
    ui->plotPID->xAxis->setRange(d.time.last() - pidPlotHistorySize,  d.time.last());

    // Neu zeichnen. Ist das noetig?
    ui->plotPID->replot();
}

void MainWindow::forward()
{
    qDebug() << "VOR";
    Q_EMIT robotRemoteControllUpdate( config::GUIREMOTEVELOCITY, std::numeric_limits<double>::quiet_NaN() );
}

void MainWindow::back()
{
    qDebug() << "ZURUECK";
    Q_EMIT robotRemoteControllUpdate(-config::GUIREMOTEVELOCITY, std::numeric_limits<double>::quiet_NaN() );
}

void MainWindow::left()
{
    qDebug() << "LINKS";
    Q_EMIT robotRemoteControllUpdate(std::numeric_limits<double>::quiet_NaN(), config::GUIREMOTETURNRATE );
}

void MainWindow::right()
{
    qDebug() << "RECHTS";
    Q_EMIT robotRemoteControllUpdate(std::numeric_limits<double>::quiet_NaN(), -config::GUIREMOTETURNRATE );
}

void MainWindow::strongleft()
{
    qDebug() << "STARK-LINKS";
    Q_EMIT robotRemoteControllUpdate(std::numeric_limits<double>::quiet_NaN(), 5*config::GUIREMOTETURNRATE );
}

void MainWindow::strongright()
{
    qDebug() << "STARK-RECHTS";
    Q_EMIT robotRemoteControllUpdate(std::numeric_limits<double>::quiet_NaN(), 5*(-config::GUIREMOTETURNRATE) );
}

void MainWindow::stop()
{
    qDebug() << "STOP";
    Q_EMIT robotRemoteControllUpdate(0.0, 0.0 );

}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)  {
        int x = event->x();
        int y = event->y();

        if(ui->hue_1->underMouse()){
            QColor pick = QColorDialog::getColor(colors[0],this,"Pick hue 1 color");
            if(pick.isValid()){
                colors[0] = pick;
                ui->hue_1->setBackgroundBrush(QBrush(colors[0]));
                changeCamParams();
            }
        } else if(ui->hue_2->underMouse()){
            QColor pick = QColorDialog::getColor(colors[1],this,"Pick hue 2 color");
            if(pick.isValid()){
                colors[1] = pick;
                ui->hue_2->setBackgroundBrush(QBrush(colors[1]));
                changeCamParams();
            }
        }
        if(ui->hue_3->underMouse()){
            QColor pick = QColorDialog::getColor(colors[2],this,"Pick hue 3 color");
            if(pick.isValid()){
                colors[2] = pick;
                ui->hue_3->setBackgroundBrush(QBrush(colors[2]));
                changeCamParams();
            }
        } else if(ui->hue_4->underMouse()){
            QColor pick = QColorDialog::getColor(colors[3],this,"Pick hue 4 color");
            if(pick.isValid()){
                colors[3] = pick;
                ui->hue_4->setBackgroundBrush(QBrush(colors[3]));
                changeCamParams();
            }
        } else if(ui->hue_5->underMouse()){
            QColor pick = QColorDialog::getColor(colors[4],this,"Pick hue 5 color");
            if(pick.isValid()){
                colors[4] = pick;
                ui->hue_5->setBackgroundBrush(QBrush(colors[4]));
                changeCamParams();
            }
        } else if(ui->hue_6->underMouse()){
            QColor pick = QColorDialog::getColor(colors[5],this,"Pick hue 6 color");
            if(pick.isValid()){
                colors[5] = pick;
                ui->hue_6->setBackgroundBrush(QBrush(colors[5]));
                changeCamParams();
            }
        }

        if(x>5 && x< config::BORDERSIDE_CM*2+config::FIELDWIDTH_CM+5 && y>5 && y<config::BORDERTOP_CM*2+5+config::FIELDHEIGHT_CM )
            // It IS in the map area
        {
            qDebug() << "Maus klick erkannt";
            qDebug() << "X: "<< x << " Y: "<< y;
            QPair<double,double> newCor;
            newCor.first=(x*1.0-5-config::BORDERSIDE_CM)/100;
            newCor.second=(config::FIELDHEIGHT_CM-(y*1.0-5-config::BORDERTOP_CM))/100;
            double winkel;
            //qDebug() << "xNEW: "<< xNEW << " yNEW: "<< yNEW;

            switch(ui->comboBox_SetType->currentIndex())
                //decide which object is set on the click position
            {
            case config::GUIopponent:
            {
                MapData::setObstacle(Obstacle(newCor,
                                              ObstacleColor::OPPONENT,
                                              ObstacleType::OPPONENT,
                                              ObstacleStatus::UNBLOCKED));
                qDebug() << "Gegner";
            }
                break;
            case config::GUIself:
            {
                winkel=(ui->spinBox_orientation->value())*M_PI/180.0;
                MapData::setObstacle(Obstacle(newCor,
                                              ObstacleColor::ME,
                                              ObstacleType::ME,
                                              ObstacleStatus::UNBLOCKED,
                                              winkel));

                Position tempPos = Position(newCor.first, newCor.second,winkel, 1.0);

                Q_EMIT updateRemoteOdometry(tempPos);
                qDebug() << "set robot me";
            }
                break;
            case config::GUIpuckOpponent:
            {
                MapData::setObstacle(Obstacle(newCor,
                                              ObstacleColor::OPPONENT,
                                              ObstacleType::PUCK,
                                              ObstacleStatus::UNBLOCKED));
                qDebug() << "set puck opponent";
            }
                break;
            case config::GUIpuckSelf:
            {
                MapData::setObstacle(Obstacle(newCor,
                                              ObstacleColor::ME,
                                              ObstacleType::PUCK,
                                              ObstacleStatus::UNBLOCKED));
                qDebug() << "set puck me";
            }
                break;
            case config::GUIpuckUndef:
            {
                MapData::setObstacle(Obstacle(newCor,
                                              ObstacleColor::UNIDENTIFIED,
                                              ObstacleType::PUCK,
                                              ObstacleStatus::UNBLOCKED));
                qDebug() << "set puck undef.";
            }
                break;
            case config::GUItarget:
            {
                winkel=(ui->spinBox_orientation->value())*M_PI/180.0;
                MapData::setObstacle(Obstacle(newCor,
                                              ObstacleColor::UNIDENTIFIED,
                                              ObstacleType::TARGET,
                                              ObstacleStatus::UNBLOCKED,
                                              winkel));
                qDebug() << "set target";
            }
                break;
            }
        }
    }
}

void MainWindow::orientationSetup()
{
    this->m_changeOrientation = !this->m_changeOrientation;

    ui->pushButton_Start->setFlat(m_changeOrientation);

    Q_EMIT signalStartOrientation(this->m_changeOrientation);
}

void MainWindow::clearTargets()
{
    // always send update, if map has been changed
    MapData::clearTargets();
}

// Cam Tab
void MainWindow::on_camSourceSpin_valueChanged(int arg1){ changeCamParams(); }
void MainWindow::on_updateSpinner_valueChanged(int arg1){ changeCamParams(); }
void MainWindow::on_sizeX_textChanged(const QString &arg1){ changeCamParams(); }
void MainWindow::on_sizeY_textChanged(const QString &arg1){ changeCamParams(); }
void MainWindow::changeCamParams()
{
    CameraParams cp;
    cp.source = ui->camSourceSpin->value();
    cp.updatePeriod = ui->updateSpinner->value();
    cp.width = ui->sizeX->text().toInt();
    cp.height = ui->sizeY->text().toInt();
    cp.colorGreenMin = colors[0];
    cp.colorGreenMax = colors[1];
    cp.colorYellowMin = colors[2];
    cp.colorYellowMax = colors[3];
    cp.colorBlueMin = colors[4];
    cp.colorBlueMax = colors[5];

    Q_EMIT signalChangeCamParams(cp);
}

// Log Tab
void MainWindow::on_logCBOther_stateChanged(int arg1){ changeLogParams(); }
void MainWindow::on_logCBActor_stateChanged(int arg1){ changeLogParams(); }
void MainWindow::on_logCBAI_stateChanged(int arg1){ changeLogParams(); }
void MainWindow::on_logCBData_stateChanged(int arg1){ changeLogParams(); }
void MainWindow::on_logCBSensor_stateChanged(int arg1){ changeLogParams(); }
void MainWindow::on_logCBPlot_stateChanged(int arg1){ changeLogParams(); }
void MainWindow::on_logLevel_currentIndexChanged(int index){ changeLogParams(); }

void MainWindow::changeLogParams()
{
    Log::logParams.logLevel = static_cast<LogParams::logLevelEnum>(ui->logLevel->currentIndex());
    Log::logParams.logOthers = ui->logCBOther->isChecked();
    Log::logParams.logActor = ui->logCBActor->isChecked();
    Log::logParams.logAI = ui->logCBAI->isChecked();
    Log::logParams.logData = ui->logCBData->isChecked();
    Log::logParams.logPlots = ui->logCBPlot->isChecked();
    Log::logParams.logSensor = ui->logCBSensor->isChecked();
}


void MainWindow::changeFilterParams()
{
    FilterParams cp;
    cp.ObsFilterAnzahl = ui->spinBox_ObsFiltAnzahl->value();
    cp.ObsFilterSchnitt = ui->spinBox_ObsFilGeschnitten->value();
    cp.PosFilterAnzahl = ui->spinBox_PosFilterAnzahl->value();
    cp.PosFilterSchnitt = ui->spinBox_PosFilterGeschnitten->value();
    Q_EMIT signalChangeFilterParams(cp);
}

void MainWindow::on_spinPIDAP_valueChanged(double arg1) { changePIDParams(); }
void MainWindow::on_spinPIDAI_valueChanged(double arg1) { changePIDParams(); }
void MainWindow::on_spinPIDAD_valueChanged(double arg1) { changePIDParams(); }
void MainWindow::on_spinPIDVP_valueChanged(double arg1) { changePIDParams(); }
void MainWindow::on_spinPIDVI_valueChanged(double arg1) { changePIDParams(); }
void MainWindow::on_spinPIDVD_valueChanged(double arg1) { changePIDParams(); }

void MainWindow::changePIDParams()
{
    PIDParams p;
    p.PID_A_P = ui->spinPIDAP->value();
    p.PID_A_I = ui->spinPIDAI->value();
    p.PID_A_D = ui->spinPIDAD->value();
    p.PID_V_P = ui->spinPIDVP->value();
    p.PID_V_I = ui->spinPIDVI->value();
    p.PID_V_D = ui->spinPIDVD->value();

    Q_EMIT signalChangePIDParams(p);
}

void MainWindow::on_cbStream_stateChanged(int arg1)
{
    Cam::streamCamEnabled = (arg1 == Qt::Checked)? true : false;
    qDebug() << "streamCamEnabled ="<<Cam::streamCamEnabled;
}

void MainWindow::on_streamSensor_stateChanged(int arg1)
{
    SensorHighLevel::streamSensorEnabled = (arg1 == Qt::Checked)? true : false;
    qDebug() << "streamSensorEnabled ="<<SensorHighLevel::streamSensorEnabled;
}

void MainWindow::on_streamPath_stateChanged(int arg1)
{
    PathPlanning::streamPathEnabled = (arg1 == Qt::Checked)? true : false;
    qDebug() << "streamPathEnabled ="<<PathPlanning::streamPathEnabled;
}

void MainWindow::on_streamPID_stateChanged(int arg1)
{
    ActorHighLevel::streamPIDEnabled = (arg1 == Qt::Checked)? true : false;
    qDebug() << "streamPIDEnabled ="<<ActorHighLevel::streamPIDEnabled;
}

void MainWindow::on_streamLog_stateChanged(int arg1)
{

    Log::streamLogEnabled = (arg1 == Qt::Checked)? true : false;
    qDebug() << "streamLogEnabled ="<<Log::streamLogEnabled;

}
