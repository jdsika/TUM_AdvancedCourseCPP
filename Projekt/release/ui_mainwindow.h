/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "GUI/cvimagewidget.h"
#include "GUI/qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_9;
    QGraphicsView *graphicsView;
    QTabWidget *tabWidget;
    QWidget *tabSteuerung;
    QGridLayout *gridLayout_6;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupMap;
    QGridLayout *gridLayout_17;
    QCheckBox *checkBox_showTrackingOpponent;
    QGroupBox *groupSetMap;
    QGridLayout *gridLayout_18;
    QComboBox *comboBox_SetType;
    QSpinBox *spinBox_orientation;
    QPushButton *pushButton_ClearTargets;
    QCheckBox *checkBox_showTrackingSelf;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpacerItem *horizontalSpacer_11;
    QSpinBox *refreshTime;
    QGroupBox *Handsteuerung;
    QGridLayout *gridLayout_13;
    QPushButton *pushButton_forward;
    QPushButton *pushButton_strongright;
    QPushButton *pushButton_stop;
    QPushButton *pushButton_back;
    QPushButton *pushButton_strongleft;
    QPushButton *pushButton_right;
    QPushButton *pushButton_left;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupLaser;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout_31;
    QLabel *label_131;
    QCheckBox *streamSensor;
    QSpinBox *spinBox_kernel;
    QCustomPlot *laserPlot;
    QGroupBox *groupState;
    QGridLayout *gridLayout_20;
    QPushButton *btn_StartGame;
    QPushButton *pushButton_StartOrientation;
    QPushButton *btn_ReleasePuck;
    QGroupBox *groupInfo;
    QFormLayout *formLayout;
    QLabel *label_16;
    QLabel *labelAkku;
    QLabel *label_15;
    QLabel *labelZeit;
    QWidget *tabPath;
    QGridLayout *gridLayout;
    QGroupBox *groupPath;
    QGridLayout *gridLayout_8;
    QCustomPlot *pathPlot;
    QCheckBox *streamPath;
    QGroupBox *groupPID;
    QVBoxLayout *verticalLayout_7;
    QCheckBox *streamPID;
    QCustomPlot *plotPID;
    QGridLayout *gridLayout_7;
    QDoubleSpinBox *spinPIDAP;
    QLabel *label_25;
    QLabel *label_30;
    QLabel *label_28;
    QLabel *label_26;
    QLabel *label_23;
    QDoubleSpinBox *spinPIDAD;
    QLabel *label_24;
    QLabel *label_29;
    QDoubleSpinBox *spinPIDVD;
    QDoubleSpinBox *spinPIDVI;
    QDoubleSpinBox *spinPIDVP;
    QDoubleSpinBox *spinPIDAI;
    QLabel *label_27;
    QWidget *tabCam;
    QGridLayout *gridLayout_11;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_8;
    QSpinBox *camSourceSpin;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_11;
    QLineEdit *sizeX;
    QLabel *label_12;
    QLineEdit *sizeY;
    QLabel *label_19;
    QSpacerItem *horizontalSpacer;
    QGridLayout *gridLayout_5;
    QSpacerItem *horizontalSpacer_8;
    QLabel *label_2;
    QLabel *label_21;
    QGraphicsView *hue_gmin;
    QGraphicsView *hue_ymax;
    QLabel *label_5;
    QGraphicsView *hue_gmax;
    QLabel *label_3;
    QGraphicsView *hue_ymin;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer_9;
    QLabel *label_20;
    QLabel *label_6;
    QLabel *label_22;
    QGraphicsView *hue_bmin;
    QLabel *label_7;
    QGraphicsView *hue_bmax;
    QSpacerItem *horizontalSpacer_10;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_9;
    QSpinBox *updateSpinner;
    QLabel *label_10;
    QCheckBox *cbStream;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *btnDetectColor;
    QLabel *label_17;
    QLabel *labelColorDetect;
    QSpacerItem *horizontalSpacer_12;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_10;
    QSpacerItem *horizontalSpacer_6;
    CVImageWidget *camView;
    QSpacerItem *horizontalSpacer_7;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_3;
    QWidget *tabLog;
    QGridLayout *gridLayout_12;
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout_4;
    QCheckBox *logCBAI;
    QCheckBox *logCBSensor;
    QCheckBox *logCBActor;
    QCheckBox *logCBData;
    QCheckBox *logCBOther;
    QCheckBox *logCBPlot;
    QLabel *label_13;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *streamLog;
    QSpacerItem *horizontalSpacer_15;
    QLabel *label_14;
    QComboBox *logLevel;
    QSpacerItem *horizontalSpacer_5;
    QPlainTextEdit *logView;
    QHBoxLayout *horizontalLayout_6;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1210, 668);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout_9 = new QHBoxLayout(centralWidget);
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setMinimumSize(QSize(450, 650));
        graphicsView->setMaximumSize(QSize(450, 650));
        graphicsView->viewport()->setProperty("cursor", QVariant(QCursor(Qt::CrossCursor)));
        graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsView->setInteractive(true);
        graphicsView->setSceneRect(QRectF(0, 0, 450, 650));

        horizontalLayout_9->addWidget(graphicsView, 0, Qt::AlignLeft|Qt::AlignTop);

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setCursor(QCursor(Qt::ArrowCursor));
        tabSteuerung = new QWidget();
        tabSteuerung->setObjectName(QStringLiteral("tabSteuerung"));
        gridLayout_6 = new QGridLayout(tabSteuerung);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        frame = new QFrame(tabSteuerung);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupMap = new QGroupBox(frame);
        groupMap->setObjectName(QStringLiteral("groupMap"));
        gridLayout_17 = new QGridLayout(groupMap);
        gridLayout_17->setSpacing(6);
        gridLayout_17->setContentsMargins(11, 11, 11, 11);
        gridLayout_17->setObjectName(QStringLiteral("gridLayout_17"));
        checkBox_showTrackingOpponent = new QCheckBox(groupMap);
        checkBox_showTrackingOpponent->setObjectName(QStringLiteral("checkBox_showTrackingOpponent"));

        gridLayout_17->addWidget(checkBox_showTrackingOpponent, 1, 1, 1, 1);

        groupSetMap = new QGroupBox(groupMap);
        groupSetMap->setObjectName(QStringLiteral("groupSetMap"));
        gridLayout_18 = new QGridLayout(groupSetMap);
        gridLayout_18->setSpacing(6);
        gridLayout_18->setContentsMargins(11, 11, 11, 11);
        gridLayout_18->setObjectName(QStringLiteral("gridLayout_18"));
        comboBox_SetType = new QComboBox(groupSetMap);
        comboBox_SetType->setObjectName(QStringLiteral("comboBox_SetType"));

        gridLayout_18->addWidget(comboBox_SetType, 0, 0, 1, 1);

        spinBox_orientation = new QSpinBox(groupSetMap);
        spinBox_orientation->setObjectName(QStringLiteral("spinBox_orientation"));
        spinBox_orientation->setMaximum(360);
        spinBox_orientation->setSingleStep(45);
        spinBox_orientation->setValue(90);

        gridLayout_18->addWidget(spinBox_orientation, 0, 1, 1, 1);

        pushButton_ClearTargets = new QPushButton(groupSetMap);
        pushButton_ClearTargets->setObjectName(QStringLiteral("pushButton_ClearTargets"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton_ClearTargets->sizePolicy().hasHeightForWidth());
        pushButton_ClearTargets->setSizePolicy(sizePolicy);

        gridLayout_18->addWidget(pushButton_ClearTargets, 1, 0, 1, 2);


        gridLayout_17->addWidget(groupSetMap, 2, 1, 1, 1);

        checkBox_showTrackingSelf = new QCheckBox(groupMap);
        checkBox_showTrackingSelf->setObjectName(QStringLiteral("checkBox_showTrackingSelf"));

        gridLayout_17->addWidget(checkBox_showTrackingSelf, 0, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(groupMap);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_11);

        refreshTime = new QSpinBox(groupMap);
        refreshTime->setObjectName(QStringLiteral("refreshTime"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(refreshTime->sizePolicy().hasHeightForWidth());
        refreshTime->setSizePolicy(sizePolicy1);
        refreshTime->setMinimum(10);
        refreshTime->setMaximum(1000);
        refreshTime->setSingleStep(10);
        refreshTime->setValue(50);

        horizontalLayout->addWidget(refreshTime);


        gridLayout_17->addLayout(horizontalLayout, 5, 1, 1, 1);


        verticalLayout->addWidget(groupMap);

        Handsteuerung = new QGroupBox(frame);
        Handsteuerung->setObjectName(QStringLiteral("Handsteuerung"));
        gridLayout_13 = new QGridLayout(Handsteuerung);
        gridLayout_13->setSpacing(6);
        gridLayout_13->setContentsMargins(11, 11, 11, 11);
        gridLayout_13->setObjectName(QStringLiteral("gridLayout_13"));
        pushButton_forward = new QPushButton(Handsteuerung);
        pushButton_forward->setObjectName(QStringLiteral("pushButton_forward"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pushButton_forward->sizePolicy().hasHeightForWidth());
        pushButton_forward->setSizePolicy(sizePolicy2);

        gridLayout_13->addWidget(pushButton_forward, 1, 1, 1, 1);

        pushButton_strongright = new QPushButton(Handsteuerung);
        pushButton_strongright->setObjectName(QStringLiteral("pushButton_strongright"));
        sizePolicy2.setHeightForWidth(pushButton_strongright->sizePolicy().hasHeightForWidth());
        pushButton_strongright->setSizePolicy(sizePolicy2);

        gridLayout_13->addWidget(pushButton_strongright, 1, 2, 1, 1);

        pushButton_stop = new QPushButton(Handsteuerung);
        pushButton_stop->setObjectName(QStringLiteral("pushButton_stop"));
        sizePolicy2.setHeightForWidth(pushButton_stop->sizePolicy().hasHeightForWidth());
        pushButton_stop->setSizePolicy(sizePolicy2);

        gridLayout_13->addWidget(pushButton_stop, 0, 0, 1, 3);

        pushButton_back = new QPushButton(Handsteuerung);
        pushButton_back->setObjectName(QStringLiteral("pushButton_back"));
        sizePolicy2.setHeightForWidth(pushButton_back->sizePolicy().hasHeightForWidth());
        pushButton_back->setSizePolicy(sizePolicy2);

        gridLayout_13->addWidget(pushButton_back, 2, 1, 1, 1);

        pushButton_strongleft = new QPushButton(Handsteuerung);
        pushButton_strongleft->setObjectName(QStringLiteral("pushButton_strongleft"));
        sizePolicy2.setHeightForWidth(pushButton_strongleft->sizePolicy().hasHeightForWidth());
        pushButton_strongleft->setSizePolicy(sizePolicy2);

        gridLayout_13->addWidget(pushButton_strongleft, 1, 0, 1, 1);

        pushButton_right = new QPushButton(Handsteuerung);
        pushButton_right->setObjectName(QStringLiteral("pushButton_right"));
        sizePolicy2.setHeightForWidth(pushButton_right->sizePolicy().hasHeightForWidth());
        pushButton_right->setSizePolicy(sizePolicy2);

        gridLayout_13->addWidget(pushButton_right, 2, 2, 1, 1);

        pushButton_left = new QPushButton(Handsteuerung);
        pushButton_left->setObjectName(QStringLiteral("pushButton_left"));
        sizePolicy2.setHeightForWidth(pushButton_left->sizePolicy().hasHeightForWidth());
        pushButton_left->setSizePolicy(sizePolicy2);

        gridLayout_13->addWidget(pushButton_left, 2, 0, 1, 1);


        verticalLayout->addWidget(Handsteuerung);


        gridLayout_6->addWidget(frame, 0, 0, 1, 1);

        frame_2 = new QFrame(tabSteuerung);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        groupLaser = new QGroupBox(frame_2);
        groupLaser->setObjectName(QStringLiteral("groupLaser"));
        gridLayout_2 = new QGridLayout(groupLaser);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_31 = new QGridLayout();
        gridLayout_31->setSpacing(6);
        gridLayout_31->setObjectName(QStringLiteral("gridLayout_31"));
        label_131 = new QLabel(groupLaser);
        label_131->setObjectName(QStringLiteral("label_131"));

        gridLayout_31->addWidget(label_131, 0, 1, 1, 1);

        streamSensor = new QCheckBox(groupLaser);
        streamSensor->setObjectName(QStringLiteral("streamSensor"));
        sizePolicy.setHeightForWidth(streamSensor->sizePolicy().hasHeightForWidth());
        streamSensor->setSizePolicy(sizePolicy);

        gridLayout_31->addWidget(streamSensor, 0, 0, 1, 1);

        spinBox_kernel = new QSpinBox(groupLaser);
        spinBox_kernel->setObjectName(QStringLiteral("spinBox_kernel"));
        spinBox_kernel->setMinimum(1);
        spinBox_kernel->setMaximum(15);
        spinBox_kernel->setSingleStep(2);
        spinBox_kernel->setValue(5);

        gridLayout_31->addWidget(spinBox_kernel, 0, 2, 1, 1);


        gridLayout_2->addLayout(gridLayout_31, 0, 0, 1, 1);

        laserPlot = new QCustomPlot(groupLaser);
        laserPlot->setObjectName(QStringLiteral("laserPlot"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(laserPlot->sizePolicy().hasHeightForWidth());
        laserPlot->setSizePolicy(sizePolicy3);
        laserPlot->setMinimumSize(QSize(0, 250));

        gridLayout_2->addWidget(laserPlot, 1, 0, 1, 1);

        groupState = new QGroupBox(groupLaser);
        groupState->setObjectName(QStringLiteral("groupState"));
        gridLayout_20 = new QGridLayout(groupState);
        gridLayout_20->setSpacing(6);
        gridLayout_20->setContentsMargins(11, 11, 11, 11);
        gridLayout_20->setObjectName(QStringLiteral("gridLayout_20"));
        btn_StartGame = new QPushButton(groupState);
        btn_StartGame->setObjectName(QStringLiteral("btn_StartGame"));

        gridLayout_20->addWidget(btn_StartGame, 2, 0, 1, 1);

        pushButton_StartOrientation = new QPushButton(groupState);
        pushButton_StartOrientation->setObjectName(QStringLiteral("pushButton_StartOrientation"));
        sizePolicy1.setHeightForWidth(pushButton_StartOrientation->sizePolicy().hasHeightForWidth());
        pushButton_StartOrientation->setSizePolicy(sizePolicy1);

        gridLayout_20->addWidget(pushButton_StartOrientation, 0, 0, 1, 1);

        btn_ReleasePuck = new QPushButton(groupState);
        btn_ReleasePuck->setObjectName(QStringLiteral("btn_ReleasePuck"));

        gridLayout_20->addWidget(btn_ReleasePuck, 3, 0, 1, 1);


        gridLayout_2->addWidget(groupState, 3, 0, 1, 1);

        groupInfo = new QGroupBox(groupLaser);
        groupInfo->setObjectName(QStringLiteral("groupInfo"));
        formLayout = new QFormLayout(groupInfo);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        label_16 = new QLabel(groupInfo);
        label_16->setObjectName(QStringLiteral("label_16"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_16);

        labelAkku = new QLabel(groupInfo);
        labelAkku->setObjectName(QStringLiteral("labelAkku"));

        formLayout->setWidget(0, QFormLayout::FieldRole, labelAkku);

        label_15 = new QLabel(groupInfo);
        label_15->setObjectName(QStringLiteral("label_15"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_15);

        labelZeit = new QLabel(groupInfo);
        labelZeit->setObjectName(QStringLiteral("labelZeit"));

        formLayout->setWidget(1, QFormLayout::FieldRole, labelZeit);


        gridLayout_2->addWidget(groupInfo, 2, 0, 1, 1);


        verticalLayout_2->addWidget(groupLaser);


        gridLayout_6->addWidget(frame_2, 0, 1, 1, 1);

        tabWidget->addTab(tabSteuerung, QString());
        tabPath = new QWidget();
        tabPath->setObjectName(QStringLiteral("tabPath"));
        gridLayout = new QGridLayout(tabPath);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        groupPath = new QGroupBox(tabPath);
        groupPath->setObjectName(QStringLiteral("groupPath"));
        gridLayout_8 = new QGridLayout(groupPath);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        pathPlot = new QCustomPlot(groupPath);
        pathPlot->setObjectName(QStringLiteral("pathPlot"));
        sizePolicy3.setHeightForWidth(pathPlot->sizePolicy().hasHeightForWidth());
        pathPlot->setSizePolicy(sizePolicy3);

        gridLayout_8->addWidget(pathPlot, 1, 0, 1, 1);

        streamPath = new QCheckBox(groupPath);
        streamPath->setObjectName(QStringLiteral("streamPath"));

        gridLayout_8->addWidget(streamPath, 0, 0, 1, 1);


        gridLayout->addWidget(groupPath, 0, 0, 1, 1);

        groupPID = new QGroupBox(tabPath);
        groupPID->setObjectName(QStringLiteral("groupPID"));
        verticalLayout_7 = new QVBoxLayout(groupPID);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        streamPID = new QCheckBox(groupPID);
        streamPID->setObjectName(QStringLiteral("streamPID"));

        verticalLayout_7->addWidget(streamPID);

        plotPID = new QCustomPlot(groupPID);
        plotPID->setObjectName(QStringLiteral("plotPID"));
        sizePolicy3.setHeightForWidth(plotPID->sizePolicy().hasHeightForWidth());
        plotPID->setSizePolicy(sizePolicy3);
        plotPID->setMinimumSize(QSize(0, 150));

        verticalLayout_7->addWidget(plotPID);

        gridLayout_7 = new QGridLayout();
        gridLayout_7->setSpacing(6);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        gridLayout_7->setContentsMargins(-1, 12, -1, -1);
        spinPIDAP = new QDoubleSpinBox(groupPID);
        spinPIDAP->setObjectName(QStringLiteral("spinPIDAP"));
        spinPIDAP->setDecimals(3);
        spinPIDAP->setMinimum(0);
        spinPIDAP->setMaximum(9999);
        spinPIDAP->setSingleStep(0.1);
        spinPIDAP->setValue(0.5);

        gridLayout_7->addWidget(spinPIDAP, 0, 2, 1, 1);

        label_25 = new QLabel(groupPID);
        label_25->setObjectName(QStringLiteral("label_25"));
        label_25->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_7->addWidget(label_25, 0, 3, 1, 1);

        label_30 = new QLabel(groupPID);
        label_30->setObjectName(QStringLiteral("label_30"));
        label_30->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_7->addWidget(label_30, 1, 5, 1, 1);

        label_28 = new QLabel(groupPID);
        label_28->setObjectName(QStringLiteral("label_28"));
        label_28->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_7->addWidget(label_28, 1, 1, 1, 1);

        label_26 = new QLabel(groupPID);
        label_26->setObjectName(QStringLiteral("label_26"));
        label_26->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_7->addWidget(label_26, 0, 5, 1, 1);

        label_23 = new QLabel(groupPID);
        label_23->setObjectName(QStringLiteral("label_23"));

        gridLayout_7->addWidget(label_23, 0, 0, 1, 1);

        spinPIDAD = new QDoubleSpinBox(groupPID);
        spinPIDAD->setObjectName(QStringLiteral("spinPIDAD"));
        spinPIDAD->setDecimals(3);
        spinPIDAD->setMinimum(-9999);
        spinPIDAD->setMaximum(0);
        spinPIDAD->setSingleStep(0.1);
        spinPIDAD->setValue(-5);

        gridLayout_7->addWidget(spinPIDAD, 0, 6, 1, 1);

        label_24 = new QLabel(groupPID);
        label_24->setObjectName(QStringLiteral("label_24"));
        label_24->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_7->addWidget(label_24, 0, 1, 1, 1);

        label_29 = new QLabel(groupPID);
        label_29->setObjectName(QStringLiteral("label_29"));

        gridLayout_7->addWidget(label_29, 1, 0, 1, 1);

        spinPIDVD = new QDoubleSpinBox(groupPID);
        spinPIDVD->setObjectName(QStringLiteral("spinPIDVD"));
        spinPIDVD->setDecimals(3);
        spinPIDVD->setMinimum(-9999);
        spinPIDVD->setMaximum(0);
        spinPIDVD->setSingleStep(0.1);
        spinPIDVD->setValue(-8);

        gridLayout_7->addWidget(spinPIDVD, 1, 6, 1, 1);

        spinPIDVI = new QDoubleSpinBox(groupPID);
        spinPIDVI->setObjectName(QStringLiteral("spinPIDVI"));
        spinPIDVI->setDecimals(3);
        spinPIDVI->setMinimum(0);
        spinPIDVI->setMaximum(9999);
        spinPIDVI->setSingleStep(0.01);
        spinPIDVI->setValue(0.01);

        gridLayout_7->addWidget(spinPIDVI, 1, 4, 1, 1);

        spinPIDVP = new QDoubleSpinBox(groupPID);
        spinPIDVP->setObjectName(QStringLiteral("spinPIDVP"));
        spinPIDVP->setDecimals(3);
        spinPIDVP->setMinimum(0);
        spinPIDVP->setMaximum(9999);
        spinPIDVP->setSingleStep(0.1);
        spinPIDVP->setValue(0.5);

        gridLayout_7->addWidget(spinPIDVP, 1, 2, 1, 1);

        spinPIDAI = new QDoubleSpinBox(groupPID);
        spinPIDAI->setObjectName(QStringLiteral("spinPIDAI"));
        spinPIDAI->setDecimals(3);
        spinPIDAI->setMinimum(0);
        spinPIDAI->setMaximum(9999);
        spinPIDAI->setSingleStep(0.01);
        spinPIDAI->setValue(0.01);

        gridLayout_7->addWidget(spinPIDAI, 0, 4, 1, 1);

        label_27 = new QLabel(groupPID);
        label_27->setObjectName(QStringLiteral("label_27"));
        label_27->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_7->addWidget(label_27, 1, 3, 1, 1);


        verticalLayout_7->addLayout(gridLayout_7);


        gridLayout->addWidget(groupPID, 0, 1, 1, 1);

        tabWidget->addTab(tabPath, QString());
        tabCam = new QWidget();
        tabCam->setObjectName(QStringLiteral("tabCam"));
        gridLayout_11 = new QGridLayout(tabCam);
        gridLayout_11->setSpacing(6);
        gridLayout_11->setContentsMargins(11, 11, 11, 11);
        gridLayout_11->setObjectName(QStringLiteral("gridLayout_11"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_8 = new QLabel(tabCam);
        label_8->setObjectName(QStringLiteral("label_8"));

        horizontalLayout_7->addWidget(label_8);

        camSourceSpin = new QSpinBox(tabCam);
        camSourceSpin->setObjectName(QStringLiteral("camSourceSpin"));

        horizontalLayout_7->addWidget(camSourceSpin);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_3);

        label_11 = new QLabel(tabCam);
        label_11->setObjectName(QStringLiteral("label_11"));

        horizontalLayout_7->addWidget(label_11);

        sizeX = new QLineEdit(tabCam);
        sizeX->setObjectName(QStringLiteral("sizeX"));
        sizePolicy1.setHeightForWidth(sizeX->sizePolicy().hasHeightForWidth());
        sizeX->setSizePolicy(sizePolicy1);

        horizontalLayout_7->addWidget(sizeX);

        label_12 = new QLabel(tabCam);
        label_12->setObjectName(QStringLiteral("label_12"));

        horizontalLayout_7->addWidget(label_12);

        sizeY = new QLineEdit(tabCam);
        sizeY->setObjectName(QStringLiteral("sizeY"));
        sizePolicy1.setHeightForWidth(sizeY->sizePolicy().hasHeightForWidth());
        sizeY->setSizePolicy(sizePolicy1);

        horizontalLayout_7->addWidget(sizeY);

        label_19 = new QLabel(tabCam);
        label_19->setObjectName(QStringLiteral("label_19"));

        horizontalLayout_7->addWidget(label_19);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer);


        verticalLayout_5->addLayout(horizontalLayout_7);

        gridLayout_5 = new QGridLayout();
        gridLayout_5->setSpacing(6);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_8, 0, 5, 1, 1);

        label_2 = new QLabel(tabCam);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_5->addWidget(label_2, 0, 0, 1, 1);

        label_21 = new QLabel(tabCam);
        label_21->setObjectName(QStringLiteral("label_21"));

        gridLayout_5->addWidget(label_21, 1, 1, 1, 1);

        hue_gmin = new QGraphicsView(tabCam);
        hue_gmin->setObjectName(QStringLiteral("hue_gmin"));
        QSizePolicy sizePolicy4(QSizePolicy::Maximum, QSizePolicy::Ignored);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(hue_gmin->sizePolicy().hasHeightForWidth());
        hue_gmin->setSizePolicy(sizePolicy4);
        hue_gmin->setMinimumSize(QSize(0, 0));
        hue_gmin->setMaximumSize(QSize(48, 16777215));

        gridLayout_5->addWidget(hue_gmin, 0, 2, 1, 1);

        hue_ymax = new QGraphicsView(tabCam);
        hue_ymax->setObjectName(QStringLiteral("hue_ymax"));
        sizePolicy4.setHeightForWidth(hue_ymax->sizePolicy().hasHeightForWidth());
        hue_ymax->setSizePolicy(sizePolicy4);
        hue_ymax->setMinimumSize(QSize(0, 0));
        hue_ymax->setMaximumSize(QSize(48, 16777215));

        gridLayout_5->addWidget(hue_ymax, 1, 4, 1, 1);

        label_5 = new QLabel(tabCam);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_5->addWidget(label_5, 1, 3, 1, 1);

        hue_gmax = new QGraphicsView(tabCam);
        hue_gmax->setObjectName(QStringLiteral("hue_gmax"));
        sizePolicy4.setHeightForWidth(hue_gmax->sizePolicy().hasHeightForWidth());
        hue_gmax->setSizePolicy(sizePolicy4);
        hue_gmax->setMinimumSize(QSize(0, 0));
        hue_gmax->setMaximumSize(QSize(48, 16777215));

        gridLayout_5->addWidget(hue_gmax, 0, 4, 1, 1);

        label_3 = new QLabel(tabCam);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_5->addWidget(label_3, 0, 3, 1, 1);

        hue_ymin = new QGraphicsView(tabCam);
        hue_ymin->setObjectName(QStringLiteral("hue_ymin"));
        sizePolicy4.setHeightForWidth(hue_ymin->sizePolicy().hasHeightForWidth());
        hue_ymin->setSizePolicy(sizePolicy4);
        hue_ymin->setMinimumSize(QSize(0, 0));
        hue_ymin->setMaximumSize(QSize(48, 16777215));

        gridLayout_5->addWidget(hue_ymin, 1, 2, 1, 1);

        label_4 = new QLabel(tabCam);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_5->addWidget(label_4, 1, 0, 1, 1);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_9, 1, 5, 1, 1);

        label_20 = new QLabel(tabCam);
        label_20->setObjectName(QStringLiteral("label_20"));

        gridLayout_5->addWidget(label_20, 0, 1, 1, 1);

        label_6 = new QLabel(tabCam);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout_5->addWidget(label_6, 2, 0, 1, 1);

        label_22 = new QLabel(tabCam);
        label_22->setObjectName(QStringLiteral("label_22"));

        gridLayout_5->addWidget(label_22, 2, 1, 1, 1);

        hue_bmin = new QGraphicsView(tabCam);
        hue_bmin->setObjectName(QStringLiteral("hue_bmin"));
        sizePolicy4.setHeightForWidth(hue_bmin->sizePolicy().hasHeightForWidth());
        hue_bmin->setSizePolicy(sizePolicy4);
        hue_bmin->setMinimumSize(QSize(0, 0));
        hue_bmin->setMaximumSize(QSize(48, 16777215));

        gridLayout_5->addWidget(hue_bmin, 2, 2, 1, 1);

        label_7 = new QLabel(tabCam);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout_5->addWidget(label_7, 2, 3, 1, 1);

        hue_bmax = new QGraphicsView(tabCam);
        hue_bmax->setObjectName(QStringLiteral("hue_bmax"));
        sizePolicy4.setHeightForWidth(hue_bmax->sizePolicy().hasHeightForWidth());
        hue_bmax->setSizePolicy(sizePolicy4);
        hue_bmax->setMinimumSize(QSize(0, 0));
        hue_bmax->setMaximumSize(QSize(48, 16777215));

        gridLayout_5->addWidget(hue_bmax, 2, 4, 1, 1);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_10, 2, 5, 1, 1);


        verticalLayout_5->addLayout(gridLayout_5);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        label_9 = new QLabel(tabCam);
        label_9->setObjectName(QStringLiteral("label_9"));

        horizontalLayout_8->addWidget(label_9);

        updateSpinner = new QSpinBox(tabCam);
        updateSpinner->setObjectName(QStringLiteral("updateSpinner"));
        updateSpinner->setMinimum(10);
        updateSpinner->setMaximum(5000);
        updateSpinner->setValue(25);

        horizontalLayout_8->addWidget(updateSpinner);

        label_10 = new QLabel(tabCam);
        label_10->setObjectName(QStringLiteral("label_10"));

        horizontalLayout_8->addWidget(label_10);

        cbStream = new QCheckBox(tabCam);
        cbStream->setObjectName(QStringLiteral("cbStream"));
        cbStream->setChecked(false);

        horizontalLayout_8->addWidget(cbStream);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_2);


        verticalLayout_5->addLayout(horizontalLayout_8);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        btnDetectColor = new QPushButton(tabCam);
        btnDetectColor->setObjectName(QStringLiteral("btnDetectColor"));

        horizontalLayout_4->addWidget(btnDetectColor);

        label_17 = new QLabel(tabCam);
        label_17->setObjectName(QStringLiteral("label_17"));

        horizontalLayout_4->addWidget(label_17);

        labelColorDetect = new QLabel(tabCam);
        labelColorDetect->setObjectName(QStringLiteral("labelColorDetect"));

        horizontalLayout_4->addWidget(labelColorDetect);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_12);


        verticalLayout_5->addLayout(horizontalLayout_4);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        horizontalLayout_10->setContentsMargins(-1, 0, -1, -1);
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_6);

        camView = new CVImageWidget(tabCam);
        camView->setObjectName(QStringLiteral("camView"));
        QSizePolicy sizePolicy5(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(camView->sizePolicy().hasHeightForWidth());
        camView->setSizePolicy(sizePolicy5);
        camView->setMinimumSize(QSize(120, 80));
        camView->setAutoFillBackground(false);

        horizontalLayout_10->addWidget(camView);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_7);


        verticalLayout_5->addLayout(horizontalLayout_10);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));

        verticalLayout_5->addLayout(horizontalLayout_3);


        gridLayout_11->addLayout(verticalLayout_5, 0, 0, 1, 1);

        tabWidget->addTab(tabCam, QString());
        tabLog = new QWidget();
        tabLog->setObjectName(QStringLiteral("tabLog"));
        gridLayout_12 = new QGridLayout(tabLog);
        gridLayout_12->setSpacing(6);
        gridLayout_12->setContentsMargins(11, 11, 11, 11);
        gridLayout_12->setObjectName(QStringLiteral("gridLayout_12"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(6);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setSpacing(6);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        logCBAI = new QCheckBox(tabLog);
        logCBAI->setObjectName(QStringLiteral("logCBAI"));
        logCBAI->setChecked(true);

        gridLayout_4->addWidget(logCBAI, 1, 1, 1, 1);

        logCBSensor = new QCheckBox(tabLog);
        logCBSensor->setObjectName(QStringLiteral("logCBSensor"));
        logCBSensor->setChecked(true);

        gridLayout_4->addWidget(logCBSensor, 1, 3, 1, 1);

        logCBActor = new QCheckBox(tabLog);
        logCBActor->setObjectName(QStringLiteral("logCBActor"));
        logCBActor->setChecked(true);

        gridLayout_4->addWidget(logCBActor, 1, 0, 1, 1);

        logCBData = new QCheckBox(tabLog);
        logCBData->setObjectName(QStringLiteral("logCBData"));
        logCBData->setChecked(true);

        gridLayout_4->addWidget(logCBData, 1, 2, 1, 1);

        logCBOther = new QCheckBox(tabLog);
        logCBOther->setObjectName(QStringLiteral("logCBOther"));
        logCBOther->setChecked(true);

        gridLayout_4->addWidget(logCBOther, 1, 5, 1, 1);

        logCBPlot = new QCheckBox(tabLog);
        logCBPlot->setObjectName(QStringLiteral("logCBPlot"));
        logCBPlot->setChecked(true);

        gridLayout_4->addWidget(logCBPlot, 1, 4, 1, 1);


        gridLayout_3->addLayout(gridLayout_4, 4, 0, 1, 1);

        label_13 = new QLabel(tabLog);
        label_13->setObjectName(QStringLiteral("label_13"));

        gridLayout_3->addWidget(label_13, 3, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        streamLog = new QCheckBox(tabLog);
        streamLog->setObjectName(QStringLiteral("streamLog"));

        horizontalLayout_2->addWidget(streamLog);

        horizontalSpacer_15 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_15);

        label_14 = new QLabel(tabLog);
        label_14->setObjectName(QStringLiteral("label_14"));

        horizontalLayout_2->addWidget(label_14);

        logLevel = new QComboBox(tabLog);
        logLevel->setObjectName(QStringLiteral("logLevel"));

        horizontalLayout_2->addWidget(logLevel);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_5);


        gridLayout_3->addLayout(horizontalLayout_2, 2, 0, 1, 1);

        logView = new QPlainTextEdit(tabLog);
        logView->setObjectName(QStringLiteral("logView"));
        logView->setBackgroundVisible(false);

        gridLayout_3->addWidget(logView, 10, 0, 1, 1);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));

        gridLayout_3->addLayout(horizontalLayout_6, 9, 0, 1, 1);


        gridLayout_12->addLayout(gridLayout_3, 0, 0, 1, 1);

        tabWidget->addTab(tabLog, QString());

        horizontalLayout_9->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);
        comboBox_SetType->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        groupMap->setTitle(QApplication::translate("MainWindow", "Map", 0));
        checkBox_showTrackingOpponent->setText(QApplication::translate("MainWindow", "Show tracking opponent", 0));
        groupSetMap->setTitle(QApplication::translate("MainWindow", "Manual Set Type", 0));
        pushButton_ClearTargets->setText(QApplication::translate("MainWindow", "Clear all targets", 0));
        checkBox_showTrackingSelf->setText(QApplication::translate("MainWindow", "Show self tracking ", 0));
        label->setText(QApplication::translate("MainWindow", "Update Period:", 0));
        refreshTime->setSuffix(QApplication::translate("MainWindow", " ms", 0));
        Handsteuerung->setTitle(QApplication::translate("MainWindow", "Handsteuerung", 0));
        pushButton_forward->setText(QApplication::translate("MainWindow", "Vor", 0));
        pushButton_strongright->setText(QApplication::translate("MainWindow", ">>>", 0));
        pushButton_stop->setText(QApplication::translate("MainWindow", "STOP", 0));
        pushButton_back->setText(QApplication::translate("MainWindow", "R\303\274ck", 0));
        pushButton_strongleft->setText(QApplication::translate("MainWindow", "<<<", 0));
        pushButton_right->setText(QApplication::translate("MainWindow", ">", 0));
        pushButton_left->setText(QApplication::translate("MainWindow", "<", 0));
        groupLaser->setTitle(QApplication::translate("MainWindow", "Lasersensor", 0));
        label_131->setText(QApplication::translate("MainWindow", "Median Filter Kernel:", 0));
        streamSensor->setText(QApplication::translate("MainWindow", "Stream", 0));
        groupState->setTitle(QApplication::translate("MainWindow", "State Control", 0));
        btn_StartGame->setText(QApplication::translate("MainWindow", "Start Game", 0));
        pushButton_StartOrientation->setText(QApplication::translate("MainWindow", "Begin Orientation", 0));
        btn_ReleasePuck->setText(QApplication::translate("MainWindow", "Test Gather Puck", 0));
        groupInfo->setTitle(QApplication::translate("MainWindow", "Info", 0));
        label_16->setText(QApplication::translate("MainWindow", "Akku:", 0));
        labelAkku->setText(QApplication::translate("MainWindow", "0.0 V", 0));
        label_15->setText(QApplication::translate("MainWindow", "Zeit:", 0));
        labelZeit->setText(QApplication::translate("MainWindow", "0:00", 0));
        tabWidget->setTabText(tabWidget->indexOf(tabSteuerung), QApplication::translate("MainWindow", "Steuerung", 0));
        groupPath->setTitle(QApplication::translate("MainWindow", "Path", 0));
        streamPath->setText(QApplication::translate("MainWindow", "Stream", 0));
        groupPID->setTitle(QApplication::translate("MainWindow", "PID", 0));
        streamPID->setText(QApplication::translate("MainWindow", "Stream", 0));
        spinPIDAP->setPrefix(QString());
        label_25->setText(QApplication::translate("MainWindow", "I", 0));
        label_30->setText(QApplication::translate("MainWindow", "D", 0));
        label_28->setText(QApplication::translate("MainWindow", "P", 0));
        label_26->setText(QApplication::translate("MainWindow", "D", 0));
        label_23->setText(QApplication::translate("MainWindow", "Angle:", 0));
        label_24->setText(QApplication::translate("MainWindow", "P", 0));
        label_29->setText(QApplication::translate("MainWindow", "Velocity:", 0));
        label_27->setText(QApplication::translate("MainWindow", "I", 0));
        tabWidget->setTabText(tabWidget->indexOf(tabPath), QApplication::translate("MainWindow", "Path/PID", 0));
        label_8->setText(QApplication::translate("MainWindow", "Source", 0));
        label_11->setText(QApplication::translate("MainWindow", "Size:", 0));
        sizeX->setText(QApplication::translate("MainWindow", "320", 0));
        label_12->setText(QApplication::translate("MainWindow", "x", 0));
        sizeY->setText(QApplication::translate("MainWindow", "240", 0));
        label_19->setText(QApplication::translate("MainWindow", "px", 0));
        label_2->setText(QApplication::translate("MainWindow", "Green Pole:", 0));
        label_21->setText(QApplication::translate("MainWindow", "min", 0));
        label_5->setText(QApplication::translate("MainWindow", "max", 0));
        label_3->setText(QApplication::translate("MainWindow", "max", 0));
        label_4->setText(QApplication::translate("MainWindow", "Yellow Puck:", 0));
        label_20->setText(QApplication::translate("MainWindow", "min", 0));
        label_6->setText(QApplication::translate("MainWindow", "Blue Puck:", 0));
        label_22->setText(QApplication::translate("MainWindow", "min", 0));
        label_7->setText(QApplication::translate("MainWindow", "max", 0));
        label_9->setText(QApplication::translate("MainWindow", "Update Period:", 0));
        label_10->setText(QApplication::translate("MainWindow", "ms", 0));
        cbStream->setText(QApplication::translate("MainWindow", "Stream", 0));
        btnDetectColor->setText(QApplication::translate("MainWindow", "Farbe jetzt pr\303\274fen", 0));
        label_17->setText(QApplication::translate("MainWindow", "Zuletzt erkannte Farbe: ", 0));
        labelColorDetect->setText(QApplication::translate("MainWindow", "?", 0));
        tabWidget->setTabText(tabWidget->indexOf(tabCam), QApplication::translate("MainWindow", "Cam", 0));
        logCBAI->setText(QApplication::translate("MainWindow", "AI", 0));
        logCBSensor->setText(QApplication::translate("MainWindow", "Sensor", 0));
        logCBActor->setText(QApplication::translate("MainWindow", "Actor", 0));
        logCBData->setText(QApplication::translate("MainWindow", "Data", 0));
        logCBOther->setText(QApplication::translate("MainWindow", "Main", 0));
        logCBPlot->setText(QApplication::translate("MainWindow", "Plots", 0));
        label_13->setText(QApplication::translate("MainWindow", "Display these logs:", 0));
        streamLog->setText(QApplication::translate("MainWindow", "Stream", 0));
        label_14->setText(QApplication::translate("MainWindow", "Log Level:", 0));
        logLevel->setCurrentText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tabLog), QApplication::translate("MainWindow", "Log", 0));
        Q_UNUSED(MainWindow);
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
