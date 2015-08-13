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

#ifndef HERMES_CODES_H
#define HERMES_CODES_H

// Message types
// Control messages
#define HERMES_CONNECT				1
#define HERMES_STATUS				2

#define HERMES_LEFT_PLAYGROUND		3
#define HERMES_GAME_START			4
#define HERMES_GAME_OVER			5
#define HERMES_READY				6
#define HERMES_DONE					7
#define HERMES_ERROR				8
#define HERMES_KEEP_ALIVE			9

// Data messages for angelina
#define HERMES_DATA_T1				11
#define HERMES_DATA_T2				12
#define HERMES_DATA_T3				13
#define HERMES_DATA_T4				14

// Data messages for the robots
#define HERMES_SCORE				21
#define HERMES_A_B					22

#define HERMES_LOOKINGFOR			31
#define HERMES_ANGELINAFOUND		32

#define HERMES_DETECTION_START		41
#define HERMES_STOP_MOVEMENT		42
#define HERMES_TEAMCOLOR			43

#endif
