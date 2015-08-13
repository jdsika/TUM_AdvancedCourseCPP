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

#ifndef PLAYGROUND_H
#define PLAYGROUND_H

#include <QGraphicsItem>
#include <QList>
#include <QPair>

#include "qsingleton.h"

class Pillar;
class StartPosition;

typedef QList<Pillar*> PillarList;
typedef QList<StartPosition*> StartPositionList;

//! GUI representation of the playground
/*!
 *	The playground is shown in the center of the main window.
 *	It contains all data which has to be retrieved.
 */
class Playground:public QGraphicsItem, public QSingleton<Playground>
{
	Q_SINGLETON(Playground)

	public:
		//! Creates a Playground object
		Playground();

		//! Resets the playground
		/*!
		 * The playground is reset to its initial state.
		 */
		void reset();

		//! Draws a robot on the playground / updates the coordinates for drawing
		/*!
		 * \param &color color of the robot
		 * \param teamID uniquely identifies the robot
		 * \param xpos horizontal distance from origin
		 * \param ypos vertical distance from origin
		 */
		void placeRobot(const QColor &color, int teamID, double xpos, double ypos);

		//! Displays the distances between a point and the pillars
		/*!
		 * \param pos the point
		 */
		void activateMeasurement(const QPointF &pos);

		//! Hides the distances again
		void deactivateMeasurement();

		//! Generates a new playground shape
		/*!
		 * \param a the a parameter see specification
		 * \param b the b parameter see specification
		 */
		void generateNewShape(double a = 0,double b = 0);

		//! Draws the Item
		/*!
		 * Used from Qt's main loop.
		 * \param *painter the painter
		 * \param *option style information
		 * \param *widget the widget
		 */
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
		//! The items bounding rect
		QRectF boundingRect() const;
		//! Returns the a parameter
		double getA() const;
		//!Returns the a parameter
		double getB() const;

	private:
		//! List of pillars
		PillarList pillarList;
		//!  The shape of the playground
		QPainterPath shapeP;
		//! The a parameter
		double myA;
		//! The b parameter
		double myB;

		//! The locked start position index
		/*!
		 * The start position, which is empty, but lies on an occupied side.
		 */
		int lockedStartPositionIndex;

		struct Robot {
			QColor color;
			double x;
			double y;
		};

		QHash<int, Robot> robots;
};

#endif /* PLAYGROUND_H */
