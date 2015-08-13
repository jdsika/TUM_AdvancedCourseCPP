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


#include "task.h"

#include "resultframe.h"
#include "playground.h"
#include "angelinaapplication.h"

#include <QWidget>
#include <QProgressBar>
#include <QHBoxLayout>

Task::Task(const QString &name, int maxPoints, ResultFrame *resultFrame):
		QGroupBox(name, resultFrame),
		myResultFrame(resultFrame),
		myTeamID(0),
		myPoints(0),
		state(TaskDisabled)
{
	QFont font;
	font.setBold(true);
	setFont(font);	
	QHBoxLayout *mainLayout = new QHBoxLayout;
	setLayout(mainLayout);
	
	customLayout = new QHBoxLayout;
	mainLayout->addLayout(customLayout);
	mainLayout->addStretch();
	progressBar = new QProgressBar(this);
	progressBar->setMaximum(maxPoints);
	progressBar->setFixedWidth(100);
	progressBar->setFormat("%v/%m");
	progressBar->setValue(0);
	mainLayout->addWidget(progressBar);
}

void Task::addPoints(int points)
{
	myResultFrame->addPoints(points);
	myPoints += points;
	progressBar->setValue(myPoints);
}

void Task::setTeamID(int teamId)
{
	myTeamID = teamId;
}

	

void Task::addCustomWidget(QWidget *widget)
{
	customLayout->addWidget(widget);
}


int Task::getState() const
{
	return state;
}

void Task::start()
{
	state = TaskEnabled;
}

void Task::stop()
{
	state = TaskDisabled;
}

void Task::reset()
{
	progressBar->setValue(0);
	stop();
	myTeamID = 0;
	myPoints = 0;
}

void Task::message(const QString &text)
{
	myResultFrame->message(text);
}
