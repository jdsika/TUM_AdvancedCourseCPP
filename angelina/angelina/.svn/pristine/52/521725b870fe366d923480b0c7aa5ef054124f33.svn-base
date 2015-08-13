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

#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "referee.h"
#include "testgui.h"

TestGui::TestGui(QWidget *parent) :
	QWidget(parent), myPosID(0), myOriID(0), referee(0)
{
	QVBoxLayout *mainLayout = new QVBoxLayout;
	setLayout(mainLayout);

	QHBoxLayout *connectLayout = new QHBoxLayout;
	mainLayout->addLayout(connectLayout);

	QPushButton *bConnect = new QPushButton("Connect", this);
	connect(bConnect, SIGNAL(clicked()), this, SLOT(slotConnect()));
	connectLayout->addWidget(bConnect);

	edID = new QLineEdit("1", this);
	connectLayout->addWidget(edID);

	edIP = new QLineEdit("127.0.0.1", this);
	connectLayout->addWidget(edIP);

	edPort = new QLineEdit("10000", this);
	connectLayout->addWidget(edPort);

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	mainLayout->addLayout(buttonLayout);

	QPushButton *bReady = new QPushButton("Send Ready", this);
	connect(bReady, SIGNAL(clicked()), this, SLOT(slotReportReady()));
	buttonLayout->addWidget(bReady);

	QPushButton *bFinished = new QPushButton("Send Finished", this);
	connect(bFinished, SIGNAL(clicked()), this, SLOT(slotReportDone()));
	buttonLayout->addWidget(bFinished);

	QPushButton *bGoal = new QPushButton("Send Goal", this);
	connect(bGoal, SIGNAL(clicked()), this, SLOT(slotReportGoal()));
	buttonLayout->addWidget(bGoal);

	aliveTimer = new QTimer(this);
	connect(aliveTimer, SIGNAL(timeout()), this, SLOT(slotSendAlive()));
	QPushButton *bAlive = new QPushButton("Send Alive", this);
	bAlive->setCheckable(true);
	connect(bAlive, SIGNAL(toggled(bool)), this, SLOT(slotToggleAliveTimer(bool)));
	buttonLayout->addWidget(bAlive);

	QHBoxLayout *ratioLayout = new QHBoxLayout;
	mainLayout->addLayout(ratioLayout);
	QPushButton *bRatio = new QPushButton("Send Ratio", this);
	connect(bRatio, SIGNAL(clicked()), this, SLOT(slotTellAbRatio()));
	ratioLayout->addWidget(bRatio);
	QLabel *lRatio = new QLabel("a/b Ratio", this);
	ratioLayout->addWidget(lRatio);
	edRatio = new QLineEdit(this);
	ratioLayout->addWidget(edRatio);

	QHBoxLayout *egoLayout = new QHBoxLayout;
	mainLayout->addLayout(egoLayout);
	QPushButton *bSend = new QPushButton("Send Pos", this);
	connect(bSend, SIGNAL(clicked()), this, SLOT(slotTellEgoPos()));
	egoLayout->addWidget(bSend);

	QLabel *lX = new QLabel("X:", this);
	egoLayout->addWidget(lX);
	edX = new QLineEdit(this);
	egoLayout->addWidget(edX);

	QLabel *lY = new QLabel("Y:", this);
	egoLayout->addWidget(lY);
	edY = new QLineEdit(this);
	egoLayout->addWidget(edY);

	QHBoxLayout *colourLayout = new QHBoxLayout;
	mainLayout->addLayout(colourLayout);
	QPushButton *colourSend = new QPushButton("Send Team", this);
	connect(colourSend, SIGNAL(clicked()), this, SLOT(slotTellTeamColor()));
	QLabel *colourLabel = new QLabel("Team color:", this);
	yellowRbtn = new QRadioButton("Yellow", this);
	// Ensure that the two radio buttons have different states
	yellowRbtn->toggle();
	blueRbtn = new QRadioButton("Blue", this);
	colourLayout->addWidget(colourSend);
	colourLayout->addWidget(colourLabel);
	colourLayout->addWidget(yellowRbtn);
	colourLayout->addWidget(blueRbtn);

	list = new QListWidget(this);
	mainLayout->addWidget(list);
}

void TestGui::slotConnect()
{
	list->clear();
	if (referee)
		delete referee;
	referee = new Referee(edID->text().toInt(), this);
	connect(referee, SIGNAL(gameStart()), this, SLOT(slotGameStart()));
	connect(referee, SIGNAL(detectionStart()), this, SLOT(slotDetectionStart()));
	connect(referee, SIGNAL(gameOver()), this, SLOT(slotGameOver()));
	connect(referee, SIGNAL(abValues(double,double)), this, SLOT(slotAbValues(double,double)));
	connect(referee, SIGNAL(stopMovement()), this, SLOT(slotStopMovement()));
	connect(referee, SIGNAL(trueColorOfTeam(TeamColor)), this, SLOT(slotTeamColor(TeamColor)));
	referee->connectToServer(edIP->text(), edPort->text().toInt());
}

void TestGui::slotReportReady()
{
	referee->reportReady();
	list->addItem("-> Ready");
}

void TestGui::slotReportDone()
{
	referee->reportDone();
	list->addItem("-> Done");
}

void TestGui::slotReportGoal()
{
	referee->reportGoal();
	list->addItem("-> Goal");
}

void TestGui::slotSendAlive()
{
	referee->sendAlive();
	list->addItem("-> Alive");
}

void TestGui::slotToggleAliveTimer(bool autotransmit)
{
	if (autotransmit)
		aliveTimer->start(20000);
	else
		aliveTimer->stop();
}

void TestGui::slotTellAbRatio()
{
	double ratio = edRatio->text().toDouble();
	referee->tellAbRatio(ratio);
	list->addItem(QString("-> Ratio: %1").arg(ratio));
}

void TestGui::slotTellEgoPos()
{
	double x = edX->text().toDouble();
	double y = edY->text().toDouble();
	referee->tellEgoPos(x, y);
	list->addItem(QString("-> Position update: x: %1, y: %2").arg(x).arg(y));
}

void TestGui::slotGameStart()
{
	list->addItem("<- GameStart");
}

void TestGui::slotDetectionStart()
{
	list->addItem("<- Detection Time running");
}

void TestGui::slotGameOver()
{
	list->addItem("<- Game Over");
}

void TestGui::slotStopMovement()
{
	list->addItem("<- Stop immediately!");
}

void TestGui::slotAbValues(double a, double b)
{
	list->addItem(QString("-> a: %1, b: %2").arg(a).arg(b));
}

void TestGui::slotTellTeamColor()
{
	TeamColor tColour;

	if (yellowRbtn->isChecked() && !blueRbtn->isChecked())
	{
		tColour = yellow;
		list->addItem("-> Team color: Yellow");
	}
	else
	{
		tColour = blue;
		list->addItem("-> Team color: Blue");
	}
	referee->tellTeamColor(tColour);
}

void TestGui::slotTeamColor(TeamColor color)
{
	if (color == yellow)
		list->addItem("<- Team color: Yellow");
	else
		list->addItem("<- Team color: Blue");
}
