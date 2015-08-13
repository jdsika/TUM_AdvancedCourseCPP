/***************************************************************************
 *   Server GUI "angelina" for "Projektkurs C++" WS 07/08				   *
 *   																	   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "mainwindow.h"

#include <QAction>
#include <QLineEdit>
#include <QProgressBar>
#include <QTcpSocket>
#include <QMenuBar>
#include <QToolBar>
#include <QDockWidget>
#include <QTimer>

#include "angelinaapplication.h"
#include "playground.h"
#include "editplaygrounddialog.h"
#include "editoptionsdialog.h"
#include "server.h"
#include "resultframe.h"
#include "clock.h"

void MainWindow::slotEditPlayground()
{
	Playground *playground = Playground::getInstance();

	double a = playground->getA();
	double b = playground->getB();

	EditPlayGroundDialog editPlayGroundDialog(&a, &b);
	editPlayGroundDialog.exec();

	playground->generateNewShape(a, b);
	angelinaApp->saveConfig();

	// Redraw and fit new playground
	view->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void MainWindow::slotEditOptions()
{
	Options options = angelinaApp->getOptions();
	EditOptionsDialog editOptionsDialog(&options);
	editOptionsDialog.exec();
}

void MainWindow::slotListen()
{
	Server *server = Server::getInstance();
	if (server->listen(QHostAddress::Any, lePortNr->text().toUInt()))
	{
		pbWorking->setMaximum(0);
		lePortNr->setEnabled(false);
		actions.listen->setEnabled(false);
		actions.disconnect->setEnabled(true);
		actions.editPlayground->setEnabled(false);
		actions.editOptions->setEnabled(false);
	}
}

void MainWindow::slotNewConnection()
{
	Options options = angelinaApp->getOptions();

	pbWorking->setMaximum(2);
	if (state < GUIFirstConnection)
	{
		pbWorking->setValue(1);
		state = GUIFirstConnection;

		if (options.robotCount == AngelinaApplication::OneRobot)
		{
			state = GUISecondConnection;
		}
	}
	else if (state < GUISecondConnection)
	{

		state = GUISecondConnection;
	}

	if (state == GUISecondConnection)
	{
		pbWorking->setValue(2);
        Server *server = Server::getInstance();
        server->close();
	}

}

void MainWindow::slotStart()
{
	actions.start->setEnabled(false);
	actions.gameOver->setEnabled(true);

	Clock::getInstance()->start();

	ResultFrame::getLeft()->slotSendStart();
	ResultFrame::getRight()->slotSendStart();

	globalTimer->start(1000);
}

void MainWindow::slotGameOver()
{
	ResultFrame *frameLeft = ResultFrame::getLeft();
	ResultFrame *frameRight = ResultFrame::getRight();

	frameLeft->sendGameOver();
	frameRight->sendGameOver();

	frameLeft->getSocket()->disconnectFromHost();
	frameRight->getSocket()->disconnectFromHost();
	frameLeft->message(QString("Final score: %1").arg(frameLeft->getTotalPoints()));
	frameRight->message(QString("Final score: %1").arg(frameRight->getTotalPoints()));

	actions.gameOver->setEnabled(false);
	globalTimer->stop();
	actions.start->setText("Start");
}

void MainWindow::slotNewReady()
{
	if (ResultFrame::getLeft()->getState() == ResultFrame::FrameRobotReady && ResultFrame::getRight()->getState()
			== ResultFrame::FrameRobotReady)
	{
		actions.start->setEnabled(true);

		Options options = angelinaApp->getOptions();
		if (options.connectionMode == AngelinaApplication::ConnectionAuto)
		{
			QTimer *timer = new QTimer(this);
			timer->setSingleShot(true);
			connect(timer, SIGNAL(timeout()), this, SLOT(slotStart()));
			timer->start(1500);
		}
	}
}

void MainWindow::slotNewFinished()
{
	ResultFrame *frameLeft = ResultFrame::getLeft();
	ResultFrame *frameRight = ResultFrame::getRight();
	if (frameLeft->getState() == ResultFrame::FrameRobotFinished && frameRight->getState()
			== ResultFrame::FrameRobotFinished)
	{
		frameLeft->message(QString("Final score. %1").arg(/*angelina->getFrameScore(resFrame1)*/1));
		frameRight->message(QString("Final score. %1").arg(/*angelina->getFrameScore(resFrame2)*/2));
		actions.gameOver->setEnabled(false);
	}
}

void MainWindow::slotDisconnect()
{
	Server *server = Server::getInstance();
	server->close();

	Clock::getInstance()->reset();

	ResultFrame::getLeft()->reset();
	ResultFrame::getRight()->reset();

	Playground::getInstance()->reset();

	state = GUIReset;

	pbWorking->setValue(0);
	pbWorking->setMaximum(2);
	pbWorking->update();
	lePortNr->setEnabled(true);

	actions.editPlayground->setEnabled(true);
	actions.editOptions->setEnabled(true);
	actions.listen->setEnabled(true);
	actions.disconnect->setEnabled(false);
	actions.start->setEnabled(false);
	actions.start->setText("Start");
	actions.gameOver->setEnabled(false);
	globalTimer->stop();
	elapsedTime = 0;
}

void MainWindow::globalTimerTick()
{
	elapsedTime++;

        actions.start->setText(QString("Running (%1:%2)").arg(elapsedTime/60, 2, 10, QChar('0')).arg(elapsedTime%60, 2, 10, QChar('0')));
	Q_EMIT elapsedTimeChanged(elapsedTime);
}

void MainWindow::resizeEvent(QResizeEvent * /* event */)
{
	view->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void MainWindow::showEvent(QShowEvent * /* event */)
{
	view->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void MainWindow::slotStopTimer()
{
        actions.start->setText(QString("Stopped (%1:%2)").arg(elapsedTime/60, 2, 10, QChar('0')).arg(elapsedTime%60, 2, 10, QChar('0')));
        globalTimer->stop();
}

void MainWindow::slotResumeTimer()
{
        if(elapsedTime > 0)
            globalTimer->start(1000);
}

MainWindow::MainWindow() :
	QMainWindow(), QSingleton<MainWindow> (), state(GUIReset)
{
	Server *server = Server::getInstance();
	connect(server, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));

	ResultFrame *frameLeft = new ResultFrame(QColor(128, 128, 255), ResultFrame::FrameLeft, this);
	connect(frameLeft, SIGNAL(readyToStart()), this, SLOT(slotNewReady()));
	connect(frameLeft, SIGNAL(done()), this, SLOT(slotNewFinished()));
        connect(frameLeft, SIGNAL(stopTimer()), this, SLOT(slotStopTimer()));
        connect(frameLeft, SIGNAL(resumeTimer()), this, SLOT(slotResumeTimer()));
        connect(this, SIGNAL(elapsedTimeChanged(unsigned long int)), frameLeft, SLOT(updateElapsedTime(unsigned long int)));

        ResultFrame *frameRight = new ResultFrame(Qt::yellow, ResultFrame::FrameRight, this);
	connect(frameRight, SIGNAL(readyToStart()), this, SLOT(slotNewReady()));
	connect(frameRight, SIGNAL(done()), this, SLOT(slotNewFinished()));
        connect(frameRight, SIGNAL(stopTimer()), this, SLOT(slotStopTimer()));
        connect(frameRight, SIGNAL(resumeTimer()), this, SLOT(slotResumeTimer()));
        connect(this, SIGNAL(elapsedTimeChanged(unsigned long int)), frameRight, SLOT(updateElapsedTime(unsigned long int)));

	setWindowTitle(angelinaApp->applicationName());

	createActions();
	createMenus();
	createToolBars();
	createCentralWidget();
	createDockWidgets();

	Options options = angelinaApp->getOptions();
	if (options.connectionMode == AngelinaApplication::ConnectionAuto)
	{
		slotListen();
	}
	elapsedTime = 0;
	globalTimer = new QTimer();
	connect(globalTimer, SIGNAL(timeout()), this, SLOT(globalTimerTick()));
}

void MainWindow::createActions()
{
	Icons *icons = Icons::getInstance();

	actions.editPlayground = new QAction(icons->get(Icons::IconEditPlayground), "Edit Playground", this);
	connect(actions.editPlayground, SIGNAL(triggered()), this, SLOT(slotEditPlayground()));

	actions.editOptions = new QAction("Options", this);
	connect(actions.editOptions, SIGNAL(triggered()), this, SLOT(slotEditOptions()));

	actions.quit = new QAction("Exit", this);
	connect(actions.quit, SIGNAL(triggered()), angelinaApp, SLOT(quit()));

	actions.listen = new QAction(icons->get(Icons::IconListen), "Listen", this);
	connect(actions.listen, SIGNAL(triggered()), this, SLOT(slotListen()));

	actions.disconnect = new QAction(icons->get(Icons::IconDisconnect), "Disconnect", this);
	connect(actions.disconnect, SIGNAL(triggered()), this, SLOT(slotDisconnect()));
	actions.disconnect->setEnabled(false);

	actions.start = new QAction(icons->get(Icons::IconStart), "Start", this);
	connect(actions.start, SIGNAL(triggered()), this, SLOT(slotStart()));
	actions.start->setEnabled(false);

	actions.gameOver = new QAction(icons->get(Icons::IconGameOver), "Game Over", this);
	connect(actions.gameOver, SIGNAL(triggered()), this, SLOT(slotGameOver()));

	actions.gameOver->setEnabled(false);
}

void MainWindow::createMenus()
{
	menus.file = menuBar()->addMenu("File");
	menus.file->addAction(actions.editPlayground);
	menus.file->addAction(actions.editOptions);
	menus.file->addAction(actions.quit);

	menus.network = menuBar()->addMenu("Network");
	menus.network->addAction(actions.listen);
	menus.network->addAction(actions.disconnect);

	menus.challenge = menuBar()->addMenu("Challenge");
	menus.challenge->addAction(actions.start);
	menus.challenge->addAction(actions.gameOver);
}

void MainWindow::createToolBars()
{
	toolbars.file = addToolBar("File");
	toolbars.file->addAction(actions.editPlayground);
	toolbars.file->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

	toolbars.network = addToolBar("Network");
	toolbars.network->addAction(actions.listen);
	toolbars.network->addAction(actions.disconnect);
	toolbars.network->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

	lePortNr = new QLineEdit("10000", this);
	lePortNr->setMaxLength(6);
	lePortNr->setMaximumWidth(60);

	toolbars.network->addWidget(lePortNr);
	toolbars.network->addSeparator();

	pbWorking = new QProgressBar(this);
	pbWorking->setMinimumWidth(60);
	pbWorking->setTextVisible(false);
	pbWorking->setMaximum(2);
	pbWorking->setValue(0);

	toolbars.network->addWidget(pbWorking);

	toolbars.challenge = addToolBar("Challenge");
	toolbars.challenge->addAction(actions.start);
	toolbars.challenge->addAction(actions.gameOver);
	toolbars.challenge->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
}

void MainWindow::createCentralWidget()
{
	Playground *playground = Playground::getInstance();
	scene = new QGraphicsScene(this);

	playground->setRotation(90.0);
	scene->addItem(playground);
	scene->setBackgroundBrush(Qt::darkGray);

	view = new QGraphicsView(scene, this);
	setCentralWidget(view);
}

void MainWindow::createDockWidgets()
{
	ResultFrame *frameLeft = ResultFrame::getLeft();
	ResultFrame *frameRight = ResultFrame::getRight();

	QDockWidget *dockLeft = new QDockWidget("Robot Left", this);
	dockLeft->setWidget(frameLeft);
	addDockWidget(Qt::LeftDockWidgetArea, dockLeft);

	QDockWidget *dockRight = new QDockWidget("Robot Right", this);
	dockRight->setWidget(frameRight);
	addDockWidget(Qt::RightDockWidgetArea, dockRight);

	Server *server = Server::getInstance();
	server->setResultFrames(frameLeft, frameRight);
}
