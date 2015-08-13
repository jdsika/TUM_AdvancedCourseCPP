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

#ifndef TESTGUI_H
#define TESTGUI_H

#include <QLineEdit>
#include <QListWidget>
#include <QRadioButton>
#include <QTimer>
#include <QWidget>
#include <QMap>

#include "referee.h"

class TestGui: public QWidget
{
	Q_OBJECT

	public:
		TestGui(QWidget *parent = 0);

	private:
		int myPosID;
		int myOriID;
		QLineEdit *edID;
		QLineEdit *edPort;
		QLineEdit *edIP;
		QLineEdit *edRatio;
		QLineEdit *edX;
		QLineEdit *edY;
		QListWidget *list;
		QRadioButton *yellowRbtn;
		QRadioButton *blueRbtn;
		QTimer *aliveTimer;
		Referee *referee;

	private Q_SLOTS:
		void slotConnect();
		void slotReportReady();
		void slotReportDone();
		void slotReportGoal();
		void slotSendAlive();
		void slotToggleAliveTimer(bool);
		void slotTellAbRatio();
		void slotTellEgoPos();
		void slotGameStart();
		void slotDetectionStart();
		void slotGameOver();
		void slotAbValues(double a, double b);
		void slotStopMovement();
		void slotTellTeamColor();
		void slotTeamColor(TeamColor);
};

#endif /* TESTGUI_H */
