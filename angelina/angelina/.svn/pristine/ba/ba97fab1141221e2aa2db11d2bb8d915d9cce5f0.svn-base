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

#include "clock.h"

#include <QTimer>

Clock::Clock() :
	QObject(), QSingleton<Clock> (), seconds(0)
{
	//setText(QTime(0,0,0,0).toString("mm:ss"));
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(slotIncrease()));
}

void Clock::start()
{
	timer->start(1000);
}

void Clock::stop()
{
	pause();
	reset();
}

void Clock::pause()
{
	timer->stop();
}

void Clock::resume()
{
	timer->start(1000);
}

void Clock::reset()
{
	seconds = 0;
}

int Clock::getSeconds() const
{
	return seconds;
}

QTime Clock::getTime() const
{
	return QTime().addSecs(seconds);;
}

void Clock::slotIncrease()
{
	seconds++;
}
