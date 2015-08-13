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

#include "ratiotask.h"

#include <QLabel>
#include <QTcpSocket>

#include "timeoutclock.h"
#include "resultframe.h"
#include "hermescodes.h"
#include "playground.h"

#define RESETTEXT "A / B = ?"

RatioTask::RatioTask(ResultFrame *parent) :
	Task("Ratio", 10, parent), done(false)
{
	ratioInfo = new QLabel(RESETTEXT, this);
	addCustomWidget(ratioInfo);
	timer = new TimeoutClock(this);
	connect(timer, SIGNAL(ring()), this, SLOT(slotTimeOver()));
}

void RatioTask::start()
{
	timer->countdown(90);
	Task::start();
}

void RatioTask::stop()
{
	timer->reset();
	Task::stop();
}

void RatioTask::data(QDataStream &stream)
{
		qreal r;
		stream >> r;
		double ratio = (double) r;
		QString text = QString("<- Ratio %1").arg(ratio);
		ratioInfo->setText(QString("A / B = %1").arg(ratio));
		message(text);
		Playground *playground = Playground::getInstance();
		double exactRatio = playground->getA() / playground->getB();
		double pError = qAbs((exactRatio - ratio) / exactRatio) * 100.0;

        if (!done)
        {
                done = true;

		if (pError <= 15.00)
		{
			addPoints(10);
		}

                // send the correct values for a and b to the client
                timer->reset();
                slotTimeOver();
	}
}

void RatioTask::reset()
{
	done = false;
	ratioInfo->setText(RESETTEXT);
	Task::reset();
}

void RatioTask::slotTimeOver()
{
	Playground *playground = Playground::getInstance();
	QTcpSocket *socket = myResultFrame->getSocket();

	int connected = ResultFrame::FrameRobotConnected | ResultFrame::FrameRobotReady;
	if (socket && (myResultFrame->getState() & connected))
	{
		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_4_0);
		out << (quint16) 0;
		out << (quint16) HERMES_A_B;
		out << (qreal) playground->getA();
		out << (qreal) playground->getB();
		out.device()->seek(0);
		out << (quint16) (block.size() - sizeof(quint16));
		socket->write(block);
		message(QString("-> A: %1, B: %2").arg(playground->getA()).arg(playground->getB()));
	}
}
