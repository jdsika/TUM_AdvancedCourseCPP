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

#include "referee.h"
#include "hermescodes.h"
#include "hermes.h"

Referee::Referee(int teamID, QObject *parent) :
	QObject(parent), messengerOfTheGods(new Hermes(this)), wLimit(1024), myTeamID(teamID), messageSize(0), connection(
			false), testMode(false), verbose(true), ready(false)
{
	connect(messengerOfTheGods, SIGNAL(readyRead()), this, SLOT(slotRead()));
	connect(messengerOfTheGods, SIGNAL(connected()), this, SLOT(slotConnected()));
	connect(messengerOfTheGods, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
}

Referee::~Referee()
{
	if (messengerOfTheGods->isOpen())
	{
		messengerOfTheGods->close();
	}
}

void Referee::setVerbose(bool enabled)
{
	verbose = enabled;
}

bool Referee::isVerbose()
{
	return verbose;
}

void Referee::connectToServer(const QString &ip, int port)
{
	if (!connection)
		messengerOfTheGods->connectToHost(ip, port);
	if (messengerOfTheGods->waitForConnected(1000))
	{
		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_4_0);
		out << (quint16) 0;
		out << (quint16) HERMES_CONNECT;
		out << (quint16) myTeamID;
		out.device()->seek(0);
		out << (quint16) (block.size() - sizeof(quint16));
		messengerOfTheGods->write(block);
                Q_EMIT connected();
	}
	else
	{
		testMode = true;
		qDebug() << "Keinen Server gefunden => Testmodus";
		qDebug() << "Alle gesendeten Packete werden hier ausgegeben falls verbose=true";
		qDebug()
                                << "Um mit dem Server zu kommunizieren, stellt sicher, dass er laeuft (listening) und baut die Verbindung erneut auf";
                Q_EMIT connectFailed();
	}
}

void Referee::reportReady()
{
	if (connection && !ready)
	{
		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_4_0);
		out << (quint16) 0;
		out << (quint16) HERMES_READY;
		out.device()->seek(0);
		out << (quint16) (block.size() - sizeof(quint16));
		messengerOfTheGods->write(block);
		ready = true;
	}
	if (verbose)
		qDebug() << "reportReady";
}

void Referee::reportDone()
{
	if (connection)
	{
		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_4_0);
		out << (quint16) 0;
		out << (quint16) HERMES_DONE;
		out.device()->seek(0);
		out << (quint16) (block.size() - sizeof(quint16));
		messengerOfTheGods->write(block);
	}
	if (verbose)
		qDebug() << "reportDone";

}

void Referee::sendAlive()
{
	if (connection)
	{
		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_4_0);
		out << (quint16) 0;
		out << (quint16) HERMES_KEEP_ALIVE;
		out.device()->seek(0);
		out << (quint16) (block.size() - sizeof(quint16));
		messengerOfTheGods->write(block);
	}
	if (verbose)
		qDebug() << "sendAlive";
}

/* Methods to report task results */

void Referee::tellAbRatio(double ratio)
{
	if (connection)
	{
		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_4_0);
		out << (quint16) 0;
		out << (quint16) HERMES_DATA_T1;
		out << (qreal) ratio;
		out.device()->seek(0);
		out << (quint16) (block.size() - sizeof(quint16));
		messengerOfTheGods->write(block);
	}
	if (verbose)
	{
		qDebug() << "tellAbRatio";
		qDebug() << "ratio: " << ratio;
	}
}

void Referee::tellEgoPos(double posX, double posY)
{
	if (connection)
	{
		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_4_0);
		out << (quint16) 0;
		out << (quint16) HERMES_DATA_T2;
		out << (qreal) posX;
		out << (qreal) posY;
		out.device()->seek(0);
		out << (quint16) (block.size() - sizeof(quint16));
		messengerOfTheGods->write(block);
	}
	if (verbose)
	{
		qDebug() << "tellEgoPos";
		qDebug() << "posX: " << posX;
		qDebug() << "posY: " << posY;
	}
}

void Referee::tellTeamColor(TeamColor color)
{
	if (connection)
	{
		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_4_0);
		out << (quint16) 0;
		out << (quint16) HERMES_DATA_T3;
		out << (quint16) color;
		out.device()->seek(0);
		out << (quint16) (block.size() - sizeof(quint16));
		messengerOfTheGods->write(block);
	}
	if (verbose)
	{
		qDebug() << "tellTeamColor";
		QString text;
		switch (color)
		{
			case yellow:
				text = "Yellow";
				break;
			case blue:
				text = "Blue";
				break;
			default:
				break;
		}
		qDebug() << "Told TeamColor = " << text;
	}
}

void Referee::reportGoal()
{
	if (connection)
	{
		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_4_0);
		out << (quint16) 0;
		out << (quint16) HERMES_DATA_T4;
		out.device()->seek(0);
		out << (quint16) (block.size() - sizeof(quint16));
		messengerOfTheGods->write(block);
	}
	if (verbose)
	{
		qDebug() << "Goal!";
	}
}

void Referee::slotRead()
{
	QDataStream in(messengerOfTheGods);
	in.setVersion(QDataStream::Qt_4_0);

	if (messageSize == 0)
	{
		if ((int) messengerOfTheGods->bytesAvailable() < (int) sizeof(quint16))
			return;
		quint16 size;
		in >> size;
		messageSize = (int) size;
	}

	if ((int) messengerOfTheGods->bytesAvailable() < (int) messageSize)
		return;

	// Here we have received a complete message
	bool lastPacket = false;
	do
	{
		quint16 id;
		in >> id;
		int code = (int) id;
		qreal a, b;
		quint16 color;

		switch (code)
		{
			case HERMES_GAME_START:
				Q_EMIT gameStart();
				break;

			case HERMES_DETECTION_START:
				Q_EMIT detectionStart();
				break;

			case HERMES_GAME_OVER:
				Q_EMIT gameOver();
				break;

			case HERMES_A_B:
				in >> a;
				in >> b;
				Q_EMIT abValues((double) a, (double) b);
				break;

			case HERMES_TEAMCOLOR:
				in >> color;
				Q_EMIT trueColorOfTeam((TeamColor) color);
				break;

			case HERMES_STOP_MOVEMENT:
				Q_EMIT stopMovement();
				break;

			default:
				messengerOfTheGods->readAll();
				break;
		}
		if (!in.atEnd())
		{
			lastPacket = false;
			if ((int) messengerOfTheGods->bytesAvailable() < (int) sizeof(quint16))
			{
				messengerOfTheGods->readAll();
				lastPacket = true;
			}
			else
			{
				quint16 size;
				in >> size;
				messageSize = (int) size;
			}
		}
		else
		{
			lastPacket = true;
		}

	} while (!lastPacket);

	messageSize = 0;
}

void Referee::slotConnected()
{
	if (verbose)
		qDebug() << "Verbunden";
	connection = true;
}

void Referee::slotDisconnected()
{
	if (verbose)
		qDebug() << "Getrennt";
	Q_EMIT
	disconnected();
	connection = false;
	ready = false;
}

bool Referee::isConnected()
{
        return connection;
}
