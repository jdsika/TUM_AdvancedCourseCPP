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

#ifndef CLOCK_H
#define CLOCK_H

#include <QTime>
#include <QObject>

#include "qsingleton.h"

class QTimer;

//! Clock for time measurement during the challenge
/*!
 * Counts seconds during the Qt main loop. Has no graphical representation
 * but can be accessed via singleton pattern.
 */
class Clock: public QObject, public QSingleton<Clock>
{

	Q_OBJECT
	Q_SINGLETON(Clock)

	public:
		//! Starts the clock
		/*!
		 * A timer is started, which fires every second.
		 * No reset is performed!
		 */
		void start();

		//! Stops the clock
		/*!
		 * Stop is a combination of pause and reset.
		 */
		void stop();

		//! Pauses the clock
		/*!
		 * Pause means to stop incrementing seconds and time
		 */
		void pause();

		//! Resumes the clock
		/*!
		 * Resume is the inverse function of pause
		 */
		void resume();

		//! Reset
		/*!
		 * Resets time and seconds to zero, but
		 * does not pause the clock.
		 */
		void reset();

		//! Returns the seconds
		int getSeconds() const;

		//! Returns a QTime object
		/*!
		 * The time object is constructed from the counted seconds 
		 */
		QTime getTime() const;

	private:

		//! Locked constructor
		/*!
		 * Initiates a QTime. Does not start after construction
		 */
		Clock();

		//! Counted seconds
		int seconds;

		//! Timer triggered by the main loop
		QTimer *timer;

	private slots:
		//! Increases the seconds
		/*!
		 * Connected to the timers's timeout signal.
		 */
		void slotIncrease();
};

#endif
