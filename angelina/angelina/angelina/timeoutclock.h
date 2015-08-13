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

#ifndef TIMEOUTCLOCK_H
#define TIMEOUTCLOCK_H

#include <QTimer>

//! Implements a countdown timer
/*!
 * Works as a subclass of QTimer, and runs in the Qt mainloop.
 * The time unit is seconds.
 */
class TimeoutClock: public QTimer
{

	Q_OBJECT

	public:

		//! Enum to represent the timer state
		enum TimerState
		{
			//! Timer is running
			TimerRunning = 0x1,
			//! Timer is stopped (reset and paused)
			TimerStoped = 0x6,
			//! Timer is paused
			TimerPaused = 0x2,
		};

		//! Constructs a timeout clock
		/*!
		 * As a subclass from QObject the TimeoutClock is integrated
		 * in Qt's parent hierarchy. The timer's timeout signal is connected to
		 * 'slotTick'
		 * \param *parent the parent object
		 */
		TimeoutClock(QObject *parent = 0);


		//! Returns the state of the timer
		int getState() const;

		//! Resets the timer
		/*!
		 * Reseting means to stop the timeout signal and to set
		 * all numerical values to zero. This behavior differs from
		 * the Clock class.
		 */
		void reset();

		//! Pauses the timer
		/*!
		 * Stops the timer, but leaves internal values unchanged.
		 */
		void pause();

		//! Resumes the timer
		/*!
		 * Starts or restarts the timeout signal.
		 */
		void resume();

		//! Starts a countdown
		/*!
		 * The countdown is started with the given value.
		 * Before starting the timer is reset.
		 * \param seconds the seconds to countdown
		 */
		void countdown(int seconds);

	signals:

		//! Rings the timer
		/*!
		 * Signal is emitted when countdown reaches zero.
		 */
		void ring();

		//! Sends out the new countdown value
		/*!
		 Signal is emitted when ever the countdown value changes
		 \param value new value for the countdown.
		 */
		void newTime(int value);

	private:

		//! Current time while counting down
		int currentTime;

		//! The time to countdown
		int maxTime;

		//! State of the timer
		int state;

	private slots:

		//! Performs the countdown
		/*!
		 * Slot is connected to the timer timeout signal.
		 * Emits 'ring' if the countdown reaches zero.
		 */
		void slotTick();
};

#endif
