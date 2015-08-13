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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLineEdit>
#include <QGraphicsView>
#include <QMainWindow>
#include <QMenu>
#include <QProgressBar>
#include <QTimer>
#include <QToolBar>

#include "playground.h"
#include "qsingleton.h"
#include "resultframe.h"

//! The grouped GUI menus
struct Menus
{
		//! The file menu
		QMenu *file;
		//! The network menu
		QMenu *network;
		//! The challenge menu
		QMenu *challenge;
};

//! The grouped GUI toolbars
struct ToolBars
{
		//! The file toolbar
		QToolBar *file;
		//! The control toolbar
		QToolBar *control;
		//! The network toolbar
		QToolBar *network;
		//! The challenge toolbar
		QToolBar *challenge;
};

//! The grouped GUI actions
struct Actions
{
		//! Action to edit the Playground
		QAction *editPlayground;
		//! Action to edit the options
		QAction *editOptions;
		//! Action to place persons
		QAction *placePersons;
		//! Action to quit
		QAction *quit;
		//! Action to listen
		QAction *listen;
		//! Action to disconnect
		QAction *disconnect;
		//! Action to start
		QAction *start;
		//! Action to end the challenge
		QAction *gameOver;
};

//! Applications main window
/*!
 * Holds all GUI elements consisting of the ResultFrames (left and right QDockWidgets)
 * and the playground in a QGraphicsView.
 */
class MainWindow: public QMainWindow, public QSingleton<MainWindow>
{
	Q_OBJECT
	Q_SINGLETON(MainWindow)

	public:
		//! Enum to represent the GUI state
		enum GUIState
		{
			GUIReset, //! GUI has been reset
			GUIFirstConnection, //! The first robot has connected
			GUISecondConnection, //! The second robot has connected
			GUIPlacedPersons,
		//! The persons have been placed
		};

	public slots:
		//! Edit the Playground
		void slotEditPlayground();
		//! Edit the options
		void slotEditOptions();
		//! Start listening for robots
		void slotListen();
		//! Receives new incoming connections
		void slotNewConnection();
		//! Starts the challenge
		void slotStart();
		//! Ends the challenge
		void slotGameOver();
		//! Receives the ready signals
		void slotNewReady();
		//! Receives done signals
		void slotNewFinished();
		//! Disconnects and resets all
		void slotDisconnect();
		//! Processes timer events
		void globalTimerTick();
		//! Stops the timer
		void slotStopTimer();
		//! Resumes the timer
		void slotResumeTimer();

	Q_SIGNALS:
		void elapsedTimeChanged(unsigned long int elapsedTime);

	protected:
		//! Event handler to scale the playground to window dimensions
		void resizeEvent(QResizeEvent *event);
		//! Does the same thing as resizeEvent, necessary to ensure proper display on startup
		void showEvent(QShowEvent *event);

	private:
		//! Creates a a MainWindow object
		/*!
		 * All GUI elements are initiated. If you want to change the frame
		 * colors of the different teams, this is the right place to look for it.
		 */
		MainWindow();
		//! Creates the GUI actions
		void createActions();
		//! Creates the GUI menus
		void createMenus();
		//! Creates the GUI toolbar
		void createToolBars();
		//! Creates the Playground
		void createCentralWidget();
		//! Creates the ResultFrame Objects on the left and right
		void createDockWidgets();
		//! GUI menus
		Menus menus;
		//! GUI toolbars
		ToolBars toolbars;
		//! GUI actions
		Actions actions;
		//! Line edit for the port number
		QLineEdit *lePortNr;
		//! Progressbar to show the connections
		QProgressBar *pbWorking;
		//! The main window state
		int state;
		//! Global game timer
		QTimer *globalTimer;
		//! Elapsed seconds since game start
		unsigned long int elapsedTime;
		//! Central widget for playground display
		QGraphicsView *view;
		//! Container for items on the playground
		QGraphicsScene *scene;
};

#endif /* MAINWINDOW_H */
