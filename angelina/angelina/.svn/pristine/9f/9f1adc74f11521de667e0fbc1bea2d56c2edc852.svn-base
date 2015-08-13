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

#ifndef COLORTASK_H
#define COLORTASK_H

#include "task.h"
#include "timeoutclock.h"

class QLabel;
class ResultFrame;

//! Task to find out what our team color is
class ColorTask: public Task
{
	Q_OBJECT
	public:
		//! Constructs a ColorTask object
		/*!
		 * \param parent the ResultFrame
		 */
		ColorTask(ResultFrame *parent = 0);

		//! Parses incoming data from the robot
		/*!
		 * \param stream the data stream
		 */
		void data(QDataStream &stream);

		//! Resets the task to its initial state
		virtual void reset();

		//! Starts the task
		/*!
		 * Starts the timer and enables the task.
		 */
		virtual void start();

		//! Stops the task
		/*!
		 * Stops the timer and disables the task.
		 */
		virtual void stop();

	private:
		//! Flag if values where sent
		bool done;
		//! Label describing the task
		QLabel *colorInfo;
		//! Countdown as time limit to achieve this task
		TimeoutClock *timer;

	private Q_SLOTS:
		//! Executed if the timer fires
		/*!
		 * Sends the true team color.
		 * Also used if the answer was in time!
		 */
		void slotTimeOver();
};

#endif /* COLORTASK_H */
