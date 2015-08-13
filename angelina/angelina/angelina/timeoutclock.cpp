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

#include "timeoutclock.h"

TimeoutClock::TimeoutClock(QObject *parent) :
	QTimer(parent), currentTime(0), maxTime(0), state(TimerStoped)
{
	connect(this, SIGNAL(timeout()), this, SLOT(slotTick()));
}

int TimeoutClock::getState() const
{
	return state;
}

void TimeoutClock::reset()
{
	pause();
	maxTime = 0;
	currentTime = 0;
	emit newTime(maxTime - currentTime);
}

void TimeoutClock::pause()
{
	stop();
}

void TimeoutClock::resume()
{
	QTimer::start(1000);
}

void TimeoutClock::countdown(int seconds)
{
	reset();
	maxTime = seconds;
	QTimer::start(1000);
}

void TimeoutClock::slotTick()
{
	if (maxTime == currentTime)
	{
		emit ring();
		reset();
	}
	else
	{
		emit newTime(maxTime - currentTime);
		currentTime++;

	}
}

