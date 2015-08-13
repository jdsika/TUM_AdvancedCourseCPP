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

#ifndef RESULTFRAME_H
#define RESULTFRAME_H

#include <QFrame>
#include <QLabel>
#include <QList>
#include <QListWidget>
#include <QMap>
#include <QPushButton>
#include <QSignalMapper>

class QTcpSocket;

class Task;
class Clock;
class TimeoutClock;

typedef QList<Task*> TaskList;
typedef QMap<int, Task*> TaskMap;

//! Central class for robot message handling (GUI & Logic)
/*!
 * The ResultFrame handles the incoming data from the robots,
 * and displays the results. It contains all the tasks and manages 
 * eventual disqualification.
 */
class ResultFrame: public QFrame
{
	Q_OBJECT

	public:
		//! Enum represents the side of the frame
		enum FrameSide
		{
			//! frame is on the left side of the GUI
			FrameLeft,
			//! frame is on the right side of the GUI
			FrameRight,
		};

		//! Enum represents the state of the frame and the associated robot
		enum FrameRobotState
		{
			//! no connection between robot and frame
			FrameRobotDisconnected,
			//! connection has been established
			FrameRobotConnected,
			//! robot is ready for start
			FrameRobotReady,
			//! robot has finished
			FrameRobotFinished,
		};

		//! Construct a ResultFrame object
		/*!
		 * The ResultFrame is integrated in an QDockWidget, and assigned
		 * to a static pointer for the given side.
		 * \param color the background color of the frame
		 * \param frameSide the side of the frame
		 * \param parent the parent widget
		 */
		ResultFrame(const QColor &color, int frameSide, QWidget *parent = 0);

		//! Resets the frame
		/*!
		 * This method also resets all associated tasks, disconnects from the robot
		 * and stops all timers. The base state is returned .
		 */
		void reset();

		//! Set a socket
		/*!
		 * A socket is set to receive and send packages.
		 * \param socket
		 */
		void setSocket(QTcpSocket *socket);

		//! Returns the background color of the frame (-> team color)
		QColor getColor() const;

		//! Returns the socket in use
		QTcpSocket* getSocket() const;

		//! Displays a message
		/*!
		 * The message is displayed at the bottom of the ResultFrame
		 * \param text the message text
		 */
		void message(const QString &text);

		//! Returns the frame state
		int getState() const;

		//! Returns the total score
		/*!
		 * With time factor.
		 */
		double getTotalPoints() const;

		//! Returns the team ID
		int getTeamID() const;

		//! Returns the left frame
		static ResultFrame* getLeft();

		//! Returns the right frame
		static ResultFrame* getRight();

	public Q_SLOTS:
		//! Adds points to the total score
		/*!
		 * \param points the points to add
		 */
		void addPoints(int points);

		//! Sends the start message
		/*!
		 * Start message is send to the associated robot.
		 * The method start() is executed
		 */
		void slotSendStart();

		//! Disqualifies the robot
		/*!
		 * If the command line option -d was specified also disconnects from
		 * the associated robot and ends the match otherwise only shows the disqualify message
		 * in the console
		 */
		void slotDisqualify();
		//! Asks the user if he really wants to disqualify and reacts accordingly
		void confirmDisqualification();
		//! Tells the robot to resume normal operation and movement
		void resumeOperations();
		//! Tells the robot to stop engines (puck fell over, other reasons)
		void stopOperations();
		//! Stores the elapsed time locally
		void updateElapsedTime(unsigned long int);
		//! Tells the robot that the game is over
		void sendGameOver();

	Q_SIGNALS:
		//! This signal is emitted when the robot is ready to start
		void readyToStart();

		//! This signal is emitted when the robot has finished its tasks
		void done();

                //! This signal is emitted when the stop button was pressed
                void stopTimer();

                //! This signal is sent to resume the global timer
                void resumeTimer();

	private:
		//! Starts all internal things
		/*!
		 * This method starts all tasks and the alive timer.
		 */
		void start();

		//! Receives the team ID
		/*!
		 * Sets the team ID for all tasks and finds a suitable starting position
		 * for the robot.
		 * \param stream data stream containing the id
		 */
		void recieveID(QDataStream &stream);

		//! Countdown timer for the alive beacon
		TimeoutClock *aliveTimer;
		//! Map for the tasks
		TaskMap taskMap;
		//! GUI to display text messages
		QListWidget *listWidget;
		//! The socket for communication
		QTcpSocket *mySocket;
		//! Background color for the frame
		QColor myColor;
		//! Team ID for the frame
		int teamID;
		//! Total points without time factor
		int totalPoints;
		//! The state of the frame
		int state;
		//! Time the tasks took
		int seconds;
		//! Saves the size of a package
		int messageSize;
		//! The left frame
		static ResultFrame *frameLeft;
		//! The right frame
		static ResultFrame *frameRight;
		//! Elapsed time for displaying robot events
		unsigned long int elapsedTime;

		//! Most important Label ever: Current Score
		QLabel *score;

		/**********************************************
		 * Widgets for manually controlling the robot
		 **********************************************/
		QPushButton *plusFiveBtn;
		QPushButton *plusFifteenBtn;
		QPushButton *minusFiveBtn;
		QPushButton *minusFifteenBtn;
		// Used to call addPoints() correctly by the buttons above
		QSignalMapper *signalMapper;
		QPushButton *stopButton;
		QPushButton *resumeButton;
		QPushButton *disqualifyButton;

	private Q_SLOTS:
		//! Reads incoming messages
		/*!
		 * Incoming messages are passed on to the corresponding task!
		 */
		void slotRead();
		//! Receives a disconnect initiated from the robot
		void slotDisconnected();
};

#endif /* RESULTFRAME_H */
