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

#include <QLabel>
#include <QTcpSocket>

#include "colortask.h"
#include "resultframe.h"
#include "hermescodes.h"
#include "playground.h"

ColorTask::ColorTask(ResultFrame *parent) :
	Task("Team Color", 10, parent), done(false)
{
	QLabel *colorInfo = new QLabel("Team Color: ", this);
	addCustomWidget(colorInfo);
	timer = new TimeoutClock(this);
	connect(timer, SIGNAL(ring()), this, SLOT(slotTimeOver()));
}

void ColorTask::data(QDataStream &stream)
{
	quint16 c;
	QString text;
	QColor submittedColor;

	stream >> c;
	int color = c;

	stop();

	switch (color)
	{
		case 0:
			text = QString("<- Team color: Yellow");
			submittedColor = QColor(Qt::yellow);
			break;
		case 1:
			text = QString("<- Team color: Blue");
			submittedColor = QColor(128,128,255);
			break;
		default:
			text = QString("<- Unknown color or program bug.");
			submittedColor = QColor(Qt::black);
	}
	message(text);

        if(!done)
        {
                done = true;
                if (submittedColor == myResultFrame->getColor())
                {
                        addPoints(10);
                        timer->reset();
                        slotTimeOver();
                }
                else
                {
                        timer->reset();
                        slotTimeOver();
                }
        }
}

void ColorTask::reset()
{
	done = false;
	Task::reset();
}

void ColorTask::start()
{
	timer->countdown(90);
	Task::start();
}

void ColorTask::stop()
{
	timer->reset();
	Task::stop();
}

void ColorTask::slotTimeOver()
{
	QTcpSocket *socket = myResultFrame->getSocket();

	int connected = ResultFrame::FrameRobotConnected | ResultFrame::FrameRobotReady;
	if (socket && (myResultFrame->getState() & connected))
	{
		QString text;
		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_4_0);
		out << (quint16) 0;
		out << (quint16) HERMES_TEAMCOLOR;
		int realTeamColor;
		if (myResultFrame->getColor()==QColor(Qt::yellow))
		{
			realTeamColor = 0;
			text = "-> Team color: Yellow";
		}
		else
		{
			realTeamColor = 1;
			text = "-> Team color: Blue";
		}
		out << (quint16) realTeamColor;
		out.device()->seek(0);
		out << (quint16) (block.size() - sizeof(quint16));
		socket->write(block);
		message(text);
	}
}

