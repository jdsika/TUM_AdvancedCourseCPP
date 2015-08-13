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

#ifndef TASK_H
#define TASK_H

#include <QGroupBox>
#include <QString>

class QWidget;
class QProgressBar;
class QHBoxLayout;
class ResultFrame;

//! Base class for all tasks
/*!
 * Task is the GUI and logic representation of all tasks. Every task is presented in the
 * Resultframes on each side of the Playground.
 */
class Task: public QGroupBox
{
	Q_OBJECT

	public:
		//! Enum to represent the tasks state
		enum TaskState
		{
			//! task is enabled
			TaskEnabled,
			//! task is disabled
			TaskDisabled,
		};

		//! Constructs a Task object
		/*!
		 * The Task object is constructed with the given headline, the given points to acquire
		 * and the ResultFrame as parent in the Qt-Gui hierarchy. Initiates the GUI and connects the
		 * newPoints signal.
		 * \param &name the headline
		 * \param maxPoints maximum points to get
		 * \param *resultFrame the parent widget
		 */
		Task(const QString &name, int maxPoints, ResultFrame *resultFrame);

		//! Adds achieved points for this task
		/*!
		 *  Also updates the  GUI and emits the newPoints signal.
		 *  \param points points to add
		 */
		void addPoints(int points);

		//! Sets the team ID
		/*!
		 * Used to check who has to solve this task.
		 * \param id the team ID
		 */
		void setTeamID(int id);

		//! Adds a custom widget as child to the task
		/*!
		 * Used to have different GUI representations.
		 * \param *widget The widget 
		 */
		void addCustomWidget(QWidget *widget);

		//! Returns the tasks state
		int getState() const;

		//! Starts the task (enable)
		virtual void start();

		//! Stops the task (disable)
		virtual void stop();

		//! Handles a data stream
		/*!
		 * Has to be reimplemented to receive data to evaluate.
		 * \param &stream the data stream to handle
		 */
		virtual void data(QDataStream &stream) = 0;

		//! Resets the task
		/*!
		 * Stops the task, and resets all internal values as in the constructor.
		 */
		virtual void reset();

	Q_SIGNALS:
		//! Notifies score changes
		void newPoints();

	protected:
		//! Displays a message
		/*!
		 * The message is displayed in bottom of the resultframe,
		 * \param &text the message text
		 */
		void message(const QString &text);

		//! The parent result frame
		ResultFrame *myResultFrame;

		//! The team ID
		int myTeamID;

		//! The score
		int myPoints;

	private:
		//! Progressbar to display the score
		QProgressBar *progressBar;

		//! Layout for the custom widget
		QHBoxLayout *customLayout;

		//! The tasks state
		int state;
};

#endif /* TASK_H */
