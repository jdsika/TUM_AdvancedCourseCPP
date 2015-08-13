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


#ifndef ANGELINAAPPLICATION_H
#define ANGELINAAPPLICATION_H



#include <QApplication>
#include <QFileInfo>
#include <QIcon>
#include <QMap>
#include <QMainWindow>

#include "defines.h"
#include "hermescodes.h"
#include "playground.h"
#include "qsingleton.h"
#include "resultframe.h"
#include "server.h"

typedef QMap<int, QIcon> IconMap;

//! Storage for icons
/*!
 * Stores all icons needed by the application's GUI.
 * The singleton is setup during application start.
 */
class Icons:public QSingleton<Icons>
{
	Q_SINGLETON(Icons)

	public:
		//! Enum for all provided types
		enum IconType
		{
			IconListen,				//! Icon for the listen action
			IconEditPlayground,		//! Icon for the edit playground action
			IconPlacePersons,		//! Icon for the place persons action
			IconDisconnect,			//! Icon for the disconnect action
			IconStart,				//! Icon for the start actions
			IconGameOver			//! Icon for the game over action

		};
		//! Loads all icons
		/*!
		 * The icons are expected to be in a folder named 'icons'.
		 * Searching is started at the location of the binary, and continued
		 * up to root.
		 */
		void load();
		//! Return an icon defined by 'iconType'
		/*!
		 * If 'iconType does not exist an empty (default) item is returned
		 * \param iconType Type of the icon defined in IconType
		 */
		QIcon get(int iconType);

	private:
		//! Map to store the icons
		IconMap iconMap;
};

//! Struct for options
struct Options
{
		//! Display mode
		int displayMode;
		//! Mode for disqualification
		int disqualifyMode;
		//! Mode for connection
		int connectionMode;
		//! Robot count
		int robotCount;
};

//! Core class
/*! 
 *  Controls and manages the global issues concerning the application.
 *	Behaves like a standard QApplication, and can be accessed via a global
 *  pointer (AngelinaApplication *angelinaApp)
 */
class AngelinaApplication:public QApplication
{
	Q_OBJECT
	
	public:
		//! Enum for the overall application state
		enum State
		{
			ChallengeReset,			//! application has started, nothing has happened so far.
			ChallengeListening,		//! application is listening for incoming connections.
			ChallengeConnected,		//! one or two teams have connected (see test mode).
			ChallengeWaitForStart,  //! application waits for the start signal (ready from the clients)
			ChallengeStarted,		//! challenge is running
			ChallengeFinished,		//! challenge has finished, points are displayed
		};

		//! Enum for different display modes
		enum DisplayMode
		{
			DisplayFullscreen,		//! application is shown in fullscreen mode for presentation
			DisplayWindowed,		//! application is shown windowed
		};
		
		//! Enum for different disqualify modes
		enum DisqualifyMode
		{
			DisconnectOnDisqualify,	//! Disconnects the robot if disqualified
			MessageOnDisqualify,	//! displays a message on disqualify
		};

		//! Enum for different connection modes
		enum ConnectionMode
		{
			ConnectionAuto,			//! Automatically establishes connection
			ConnectionManual,		//! Connection is established by user
		};

		//! Enum for test mode with only one robot
		enum RobotCount
		{
			OneRobot,				//! runs with only one robot the other is ignored
			TwoRobot,				//! runs with tow robots the default
		};

		//! Constructs the main application
		/*!
		 * Commandline arguments are passed through. Available options (above the default Qt ones):
		 * -f [file] use file for configuration
		 * -h displays usage information
		 *
		 * \param &argc argument count
		 * \param **argv argument vector
		 * \param *ok true on success
		 */
		AngelinaApplication(int &argc, char **argv, bool *ok);

		//! Destroys the main application
		/*!
		 * Makes sure too disconnect first. GUI destruction 
		 * is done using Qt's parent system.
		 */
		~AngelinaApplication();
		
		//! Parses arguments for this application
		/*!
		 * \param &argc argument count
		 * \param **argv argument vector
		 * \param &fileInfo information for loading
		 */
		void parse(int &argc, char **argv, QFileInfo &fileInfo);

		//! Loads the team file
		/*!
		 * \param &fileInfo the file info
		 */
		void loadTeams(const QFileInfo &fileInfo);

		//! Displays the usage
		/*!
		 */
		void usage() const;

		//! Loads the configuration file
		/*!
		 * Returns true on success
		 */
		bool loadConfig();

		//! Saves config file
		/*!
		 * Saves to a file in configFileInfo.
		 */
		void saveConfig();

		//! Saves config file
		/*!
		 * \param &fileInfo the file info
		 * \param force force to save
		 */
		void saveConfig(const QFileInfo &fileInfo, bool force = false);

		//! Initiates the GUI
		void setup();
		
		//! Returns the default a value
		double getDefaultA() const;

		//! Returns the default b value
		double getDefaultB() const;

		//! Returns the options
		Options getOptions() const;

		//! Sets the options
		/*!
		 * Displaymode is adjusted
		 * \param op the options
		 */
		void setOptions(Options op);
		
		//double getFrameScore(ResultFrame *frame);

	private:
		//! Sets up everything to default
		/*!
		 * Used if no configuration file is found.
		 */
		void setupDefaultOptions();

		//! Applications options
		Options options;

		//! Application state
		int myState;

		//! Default a value
		double defaultA;

		//! Default b value
		double defaultB;

		//! The server
		Server *server;

		//! File information for configuration file
		QFileInfo configFileInfo;
};

extern AngelinaApplication *angelinaApp;

#endif
