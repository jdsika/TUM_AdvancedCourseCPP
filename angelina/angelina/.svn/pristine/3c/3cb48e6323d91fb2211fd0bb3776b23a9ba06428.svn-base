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

#ifndef RATIOTASK_H
#define RATIOTASK_H

#include "task.h"

class QLabel;

class ResultFrame;
class TimeoutClock;

//! Task to find out the Playground dimensions
/*
 * To fulfill the task the right ratio of a/b (see Playground) has to be sent.
 * In addition the task returns the true absolute values, either if the ratio was correct or
 * if 90 seconds have passed.
 */
class RatioTask: public Task
{
	Q_OBJECT

	public:

		//! Constructs a RatioTask object
		/*!
		 * \param *parent the ResultFrame as parent in the sense of Qt
		 */
		RatioTask(ResultFrame *parent = 0);

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

		//! Parses the received data
		/*!
		 * \param &stream the data stream
		 */
		virtual void data(QDataStream &stream);

		//! Resets the task to its initial state
		virtual void reset();

	private:
		//! Countdown as time limit to achieve this task
		TimeoutClock *timer;
		//! Label describing the task
		QLabel *ratioInfo;
		//! Flag if values where sent
		bool done;

	private Q_SLOTS:
		//! Executed if the timer fires
		/*!
		 * Sends the absolute playground dimensions.
		 * Also used if the answer was in time!
		 */
		void slotTimeOver();
};

#endif /* RATIOTASK_H */
