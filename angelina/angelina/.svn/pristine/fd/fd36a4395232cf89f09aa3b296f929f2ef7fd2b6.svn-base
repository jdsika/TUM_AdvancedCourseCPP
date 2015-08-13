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

#include <QDebug>
#include <QDir>
#include <QTcpSocket>
#include <QDomDocument>
#include <QTime>

#include "angelinaapplication.h"
#include "mainwindow.h"
#include "server.h"
#include "playground.h"
#include "resultframe.h"

#define TEAMSIZE 6
#define NUMBEROFTEAMS 8

void Icons::load()
{
	iconMap.insert(IconListen, QIcon(":/icons/listen.png"));
	iconMap.insert(IconEditPlayground, QIcon((":/icons/editplayground.png")));
	iconMap.insert(IconPlacePersons, QIcon(":/icons/persons.png"));
	iconMap.insert(IconDisconnect, QIcon(":/icons/disconnect.png"));
	iconMap.insert(IconStart, QIcon(":/icons/start.png"));
	iconMap.insert(IconGameOver, QIcon(":/icons/gameover.png"));
}

QIcon Icons::get(int iconType)
{
	return iconMap[iconType];
}

AngelinaApplication::AngelinaApplication(int &argc, char **argv, bool *ok):
	QApplication(argc, argv)
{
	angelinaApp = this;

	qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

	myState = ChallengeReset;
	
	setupDefaultOptions();
	
	// Team
	QFileInfo teamFileInfo;
	parse(argc, argv, teamFileInfo);

	setApplicationName("P3DX Challenge");
	
	// Options
	*ok = loadConfig();

	Icons *icons = Icons::getInstance();
	icons->load();
	setWindowIcon(icons->get(Icons::IconStart));
}

AngelinaApplication::~AngelinaApplication()
{
	if(server->isListening()) 
	{
		server->close();
	}
}

void AngelinaApplication::parse(int &argc, char **argv, QFileInfo &fileInfo)
{
	switch(argc)
	{
		case 1:
			return;

		case 2:
		default:
		{
			QString arg(argv[1]);
			if(arg == "-h" || arg == "-help")
			{
				usage();
			}
			else if(arg == "-f" || arg == "-file")
			{
				qWarning() << "missig file";
				usage();
			}
			else
			{
				qWarning() << "unknown option";
				usage();
			}
			break;
		}
		case 3:
		{
			QString arg(argv[1]);
			if(arg == "-f" || arg == "-file")
			{
				fileInfo = QFileInfo(argv[2]);
			}
			break;
		}
	}
}

void AngelinaApplication::usage() const
{
	qDebug() << "usage:";
	qDebug() << "-f | -file [filename]	loads teams from file";
	qDebug() << "-h | -help				displays this text";
}

bool AngelinaApplication::loadConfig()
{
	
	QDir home = QDir::home();
	if(!home.cd(".angelina") && !home.mkdir(".angelina"))
	{
		return false;
	}
	home.cd(".angelina");
	configFileInfo = QFileInfo(home.filePath("angelina.cfg"));
	if(!configFileInfo.exists())
	{
		saveConfig(configFileInfo, true);
		return true;
	}

	QFile configFile(configFileInfo.absoluteFilePath());
	QDomDocument confDoc;
	if(!configFile.open(QFile::ReadOnly) || !confDoc.setContent(&configFile, false))
	{
		return false;
	}

	// Loading
	QDomElement eRoot = confDoc.documentElement();
	QDomElement eOptions = eRoot.firstChildElement("Options");
	defaultA = eOptions.attribute("a").toDouble();
	defaultB = eOptions.attribute("b").toDouble();
	options.displayMode = eOptions.attribute("DisplayMode").toInt();
	options.disqualifyMode = eOptions.attribute("DisqualifyMode").toInt();
	options.connectionMode = eOptions.attribute("ConnectionMode").toInt();
	options.robotCount = eOptions.attribute("RobotCount").toInt();
	return true;
}

void AngelinaApplication::saveConfig()
{
	saveConfig(configFileInfo);
}

void AngelinaApplication::saveConfig(const QFileInfo &fileInfo, bool force)
{
	if(!force && !fileInfo.exists())
	{
		return;
	}

	QDomDocument confDoc("AngelinaConfiguration");
	QDomElement eRoot = confDoc.createElement("ConfigData");
	confDoc.appendChild(eRoot);
	QDomElement eOptions = confDoc.createElement("Options");
	eRoot.appendChild(eOptions);
	Playground *playground = Playground::getInstance();
	eOptions.setAttribute("a", QString("%1").arg(playground->getA()));
	eOptions.setAttribute("b", QString("%1").arg(playground->getB()));
	eOptions.setAttribute("DisplayMode", QString("%1").arg(options.displayMode));
	eOptions.setAttribute("DisqualifyMode", QString("%1").arg(options.disqualifyMode));
	eOptions.setAttribute("ConnectionMode", QString("%1").arg(options.connectionMode));
	eOptions.setAttribute("RobotCount", QString("%1").arg(options.robotCount));

	QFile configFile(fileInfo.absoluteFilePath());
	if(!configFile.open(QFile::WriteOnly))
	{
		return;
	}
	configFile.write(confDoc.toByteArray(4));


	configFile.close();
}

void AngelinaApplication::setup()
{
	server = Server::getInstance();
	server->setParent(this);
	MainWindow *mainWindow = MainWindow::getInstance();
	mainWindow->resize(1024, 768);
	
	switch(options.displayMode)
	{
		case DisplayFullscreen:
			mainWindow->showFullScreen();
			break;
		case DisplayWindowed:
		default:
			mainWindow->show();
			break;
	}
	mainWindow->show();
}

double AngelinaApplication::getDefaultA() const
{
	return defaultA;
}

double AngelinaApplication::getDefaultB() const
{
	return defaultB;
}

Options AngelinaApplication::getOptions() const
{
	return options;
}

void AngelinaApplication::setOptions(Options op)
{
	options = op;
	if(options.displayMode == DisplayFullscreen)
	{
		MainWindow::getInstance()->showFullScreen();
	}
	else
	{
		MainWindow::getInstance()->showNormal();
	}
	saveConfig(configFileInfo);
}

void AngelinaApplication::setupDefaultOptions()
{
	options.displayMode = DisplayWindowed;
	options.disqualifyMode = MessageOnDisqualify;
	options.connectionMode = ConnectionManual;
	options.robotCount = TwoRobot;
    defaultA = 1.66;
	defaultB = 3.0;
}

AngelinaApplication *angelinaApp;
