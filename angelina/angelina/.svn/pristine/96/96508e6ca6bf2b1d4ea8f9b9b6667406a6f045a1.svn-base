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

#include "resultframe.h"

#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QLCDNumber>
#include <QTcpSocket>
#include <QTime>

#include "ratiotask.h"
#include "colortask.h"
#include "timeoutclock.h"
#include "playground.h"
#include "hermescodes.h"
#include "clock.h"
#include "angelinaapplication.h"

ResultFrame* ResultFrame::frameLeft = 0;
ResultFrame* ResultFrame::frameRight = 0;

ResultFrame::ResultFrame(const QColor &color, int frameSide, QWidget *parent) :
	QFrame(parent), mySocket(0), myColor(color), teamID(0), totalPoints(0), state(FrameRobotDisconnected), seconds(-1),
			messageSize(0), elapsedTime(0)
{
        taskMap.clear();
	// Set up watchdog/alive timer
	aliveTimer = new TimeoutClock;
	connect(aliveTimer, SIGNAL(ring()), this, SLOT(slotDisqualify()));

	// Central layout for the whole results display
	QVBoxLayout *mainLayout = new QVBoxLayout();
	setLayout(mainLayout);

	// Change widget background to team color
	QPalette pal = palette();
	pal.setColor(QPalette::Window, color);
	setPalette(pal);
	setAutoFillBackground(true);

	// Widgets for a/b ratio task
/*	RatioTask *ratioTask = new RatioTask(this);
	taskMap.insert(HERMES_DATA_T1, ratioTask);
	mainLayout->addWidget(ratioTask);
*/
	// Widgets for team color task
	ColorTask *colorTask = new ColorTask(this);
	taskMap.insert(HERMES_DATA_T3, colorTask);
	mainLayout->addWidget(colorTask);

	// Show current score centered
	mainLayout->addStretch();

	// Huge display of current robot score
	// first the label
	QLabel *scoreLabel = new QLabel("Score", this);
	QFont scoreLabelFont;
	scoreLabelFont.setPointSize(32);
	scoreLabelFont.setWeight(QFont::Bold);
	scoreLabel->setFont(scoreLabelFont);
	scoreLabel->setAlignment(Qt::AlignCenter);
	mainLayout->addWidget(scoreLabel);

	// and now the actual score even bigger
	score = new QLabel(QString::number(totalPoints), this);
	QFont scoreFont;
	scoreFont.setPointSize(64);
	scoreFont.setWeight(QFont::Bold);
	score->setFont(scoreFont);
	score->setAlignment(Qt::AlignCenter);
	mainLayout->addWidget(score);

	// Aligns the rest of the frame to bottom
	mainLayout->addStretch();

	// Status messages from / to robot
	listWidget = new QListWidget(this);
	mainLayout->addWidget(listWidget);

	// Manual controls
	// Award points
	QGridLayout *scoreLayout = new QGridLayout();
	plusFiveBtn = new QPushButton("+5", this);
	plusFifteenBtn = new QPushButton("+15", this);
	minusFiveBtn = new QPushButton("-5", this);
	minusFifteenBtn = new QPushButton("-15", this);
	scoreLayout->addWidget(plusFiveBtn, 1, 2);
	scoreLayout->addWidget(plusFifteenBtn, 2, 2);
	scoreLayout->addWidget(minusFiveBtn, 1, 1);
	scoreLayout->addWidget(minusFifteenBtn, 2, 1);
	mainLayout->addLayout(scoreLayout);
	signalMapper = new QSignalMapper(this);
	signalMapper->setMapping(plusFiveBtn, 5);
	signalMapper->setMapping(plusFifteenBtn, 15);
	signalMapper->setMapping(minusFiveBtn, -5);
	signalMapper->setMapping(minusFifteenBtn, -15);
	// Connect the buttons to the mapper
	connect(plusFiveBtn, SIGNAL(clicked()), signalMapper, SLOT(map()));
	connect(plusFifteenBtn, SIGNAL(clicked()), signalMapper, SLOT(map()));
	connect(minusFiveBtn, SIGNAL(clicked()), signalMapper, SLOT(map()));
	connect(minusFifteenBtn, SIGNAL(clicked()), signalMapper, SLOT(map()));
	// Connect the mapper to addPoints()
	connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(addPoints(int)));


	// Start/Resume robot
	QHBoxLayout *stopResLayout = new QHBoxLayout();
	stopButton = new QPushButton("Stop", this);
	resumeButton = new QPushButton("Resume", this);
	stopResLayout->addWidget(stopButton);
	stopResLayout->addWidget(resumeButton);
	mainLayout->addLayout(stopResLayout);
	connect(stopButton, SIGNAL(clicked()), this, SLOT(stopOperations()));
	connect(resumeButton, SIGNAL(clicked()), this, SLOT(resumeOperations()));

	// Disqualify robot
	disqualifyButton = new QPushButton("Disqualify", this);
	mainLayout->addWidget(disqualifyButton);
	connect(disqualifyButton, SIGNAL(clicked()), this, SLOT(confirmDisqualification()));

	Options options = angelinaApp->getOptions();

	switch (frameSide)
	{
		case FrameLeft:
			frameLeft = this;
			break;

		case FrameRight:
			frameRight = this;
			if (options.robotCount == AngelinaApplication::OneRobot)
			{
				state = FrameRobotReady;
			}
			break;
	}
}

void ResultFrame::reset()
{
	if (mySocket)
	{
		mySocket->disconnectFromHost();
		delete mySocket;
		mySocket = 0;
	}

	aliveTimer->reset();
	listWidget->clear();

	TaskList taskList = taskMap.values();
	for (int i = 0; i < taskList.size(); i++)
	{
		taskList[i]->reset();
	}

	totalPoints = 0;
	teamID = 0;

	state = FrameRobotDisconnected;
}

void ResultFrame::setSocket(QTcpSocket *socket)
{
	mySocket = socket;
	state = FrameRobotConnected;
	connect(mySocket, SIGNAL(readyRead()), this, SLOT(slotRead()));
	connect(mySocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
}

QColor ResultFrame::getColor() const
{
	return myColor;
}

QTcpSocket* ResultFrame::getSocket() const
{
	return mySocket;
}

void ResultFrame::message(const QString &text)
{
	int minutes = elapsedTime/60;
	int seconds = elapsedTime%60;
	QString message = QString("(%1:%2) ").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));

	message += text;
	listWidget->addItem(message);

    if (listWidget->count() > 20)
    {
        QListWidgetItem *item = listWidget->takeItem(0);
        delete item;
    }

	listWidget->scrollToBottom();
}

int ResultFrame::getState() const
{
	return state;
}

void ResultFrame::addPoints(int points)
{
	totalPoints += points;
	score->setText(QString::number(totalPoints));
}

double ResultFrame::getTotalPoints() const
{
	score->setText(QString::number(totalPoints));
	return totalPoints;
}

int ResultFrame::getTeamID() const
{
	return teamID;
}

ResultFrame* ResultFrame::getLeft()
{
	return frameLeft;
}

ResultFrame* ResultFrame::getRight()
{
	return frameRight;
}

void ResultFrame::slotSendStart()
{
	if (state == FrameRobotReady && mySocket)
	{
		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_4_0);
		out << (quint16) 0;
		out << (quint16) HERMES_DETECTION_START;
		out.device()->seek(0);
		out << (quint16) (block.size() - sizeof(quint16));
		mySocket->write(block);
		start();
		message("-> Detection Start");
	}
}

void ResultFrame::slotDisqualify()
{
	Options options = angelinaApp->getOptions();

	int connected = ResultFrame::FrameRobotConnected | ResultFrame::FrameRobotReady;
	if (mySocket && (getState() & connected))
	{
		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_4_0);
		out << (quint16) 0;
		out << (quint16) HERMES_STOP_MOVEMENT;
		out.device()->seek(0);
		out << (quint16) (block.size() - sizeof(quint16));
		qDebug() << "-> Immediate stop: Disqualification";
		mySocket->write(block);
	}
	if (options.disqualifyMode == AngelinaApplication::DisconnectOnDisqualify && mySocket)
	{
		mySocket->disconnectFromHost();
	}
}

void ResultFrame::start()
{
        qDebug()<<"TaskMapSize:"<<taskMap.size();


	TaskList taskList = taskMap.values();
	for (int i = 0; i < taskList.size(); i++)
	{
		taskList[i]->start();
	}
	aliveTimer->countdown(45);
}

void ResultFrame::recieveID(QDataStream &stream)
{
	quint16 id;
	stream >> id;
	teamID = (int) id;

	TaskList taskList = taskMap.values();
	for (int i = 0; i < taskList.size(); i++)
	{
		taskList[i]->setTeamID(teamID);
	}

	Playground *playground = Playground::getInstance();
	if (myColor == Qt::yellow)
		playground->placeRobot(myColor, teamID, playground->getA()*(2.0 + 1.0/4.0), playground->getB()/2.0);
	else
		playground->placeRobot(myColor, teamID, playground->getA()*(3.0/4.0), playground->getB()/2.0);
}

void ResultFrame::slotRead()
{
	Playground *playground = Playground::getInstance();

	QDataStream in(mySocket);
	in.setVersion(QDataStream::Qt_4_0);

	if (messageSize == 0)
	{
		if ((int) mySocket->bytesAvailable() < (int) sizeof(quint16))
		{
			return;
		}
		quint16 size;
		in >> size;
		messageSize = (int) size;
	}

	if ((int) mySocket->bytesAvailable() < (int) messageSize)
	{
		return;
	}

	//Here we have received a complete message
	bool lastPacket = false;
	do
	{
		quint16 id;
		in >> id;
		int code = (int) id;

		qreal pX;
		qreal pY;
		double posX;
		double posY;
		QString positionText;
                Task *task;

		switch (code)
		{
			case HERMES_CONNECT:
				message("<- Connected");
				recieveID(in);
				break;

			case HERMES_READY:
				message("<- Ready");
				state = FrameRobotReady;
				emit readyToStart();
				break;

			case HERMES_DONE:
				message("<- Done");
				state = FrameRobotFinished;
				emit done();
				seconds = Clock::getInstance()->getSeconds();
				mySocket->disconnectFromHost();
				break;

			case HERMES_KEEP_ALIVE:
				message("<- Alive");
				aliveTimer->countdown(45);
				break;

			// T2 == Current Position
			case HERMES_DATA_T2:
				in >> pX;
				in >> pY;
				posX = (double) pX;
				posY = (double) pY;
				positionText = QString("<- Position updated: X: %1 Y: %2").arg(posX).arg(posY);
				playground->placeRobot(myColor, teamID, posX, posY);
				message(positionText);
				break;

			case HERMES_DATA_T4:
				message("<- Goal scored!");
				break;

//            case HERMES_DATA_T1:
			case HERMES_DATA_T3:
				task = taskMap[code];
				if (task && task->isEnabled()) {
					task->data(in);
				}
				else {
					mySocket->readAll();
				}
				break;

			default:
				qDebug() << "DEFAULT CODE!!!: " << code;
				mySocket->readAll();
				break;
		}

		if (!in.atEnd())
		{
			lastPacket = false;
			if ((int) mySocket->bytesAvailable() < (int) sizeof(quint16))
			{
				mySocket->readAll();
				lastPacket = true;
			}
			else
			{
				quint16 size;
				in >> size;
				messageSize = (int) size;
			}
		}
		else
		{
			lastPacket = true;
		}

	} while (!lastPacket);

	messageSize = 0;
}

void ResultFrame::slotDisconnected()
{
	state = FrameRobotDisconnected;
}

void ResultFrame::sendGameOver()
{
	int connected = ResultFrame::FrameRobotConnected | ResultFrame::FrameRobotReady;
	if (mySocket && (getState() & connected))
	{
		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_4_0);
		out << (quint16) 0;
		out << (quint16) HERMES_GAME_OVER;
		out.device()->seek(0);
		out << (quint16) (block.size() - sizeof(quint16));
		qDebug() << "-> Game Over";
		mySocket->write(block);
	}
}

void ResultFrame::confirmDisqualification()
{
	QMessageBox msgBox;
	msgBox.setIcon(QMessageBox::Question);
	msgBox.setWindowTitle(QString("Disqualify team %1?").arg(teamID));
	msgBox.setText(QString("Do you really want to disqualify team %1?").arg(teamID));
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::No);
	int result = msgBox.exec();
	if (result == QMessageBox::Yes)
		slotDisqualify();
}

void ResultFrame::resumeOperations()
{
	int connected = ResultFrame::FrameRobotConnected | ResultFrame::FrameRobotReady;
	if (mySocket && (getState() & connected))
	{
		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_4_0);
		out << (quint16) 0;
		if (elapsedTime < 90)
			out << (quint16) HERMES_DETECTION_START;
		else
			out << (quint16) HERMES_GAME_START;
		out.device()->seek(0);
		out << (quint16) (block.size() - sizeof(quint16));
		mySocket->write(block);
		message("-> Off you go!");
                resumeTimer();
	}
}

void ResultFrame::stopOperations()
{
	int connected = ResultFrame::FrameRobotConnected | ResultFrame::FrameRobotReady;
	if (mySocket && (getState() & connected))
	{
		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_4_0);
		out << (quint16) 0;
		out << (quint16) HERMES_STOP_MOVEMENT;
		out.device()->seek(0);
		out << (quint16) (block.size() - sizeof(quint16));
		mySocket->write(block);
		message("-> Stay put!");
                stopTimer();
	}
}

void ResultFrame::updateElapsedTime(unsigned long int newTime)
{
	elapsedTime = newTime;

	if (state == FrameRobotReady && mySocket && newTime == 90)
	{
		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_4_0);
		out << (quint16) 0;
		out << (quint16) HERMES_GAME_START;
		out.device()->seek(0);
		out << (quint16) (block.size() - sizeof(quint16));
		mySocket->write(block);
		start();
		message("-> Let the games begin!");
	}
}
