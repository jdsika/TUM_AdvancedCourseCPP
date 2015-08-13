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


#ifndef PILLAR_H
#define PILLAR_H

#include <QGraphicsItem>

//! Graphical representation of a pillar bounding the Playground
/*
 */
class Pillar:public QGraphicsItem
{

	public:

		//! Constructs a Pillar object
		/*
		 */
		Pillar();

		//! Destroys the object, does nothing
		/*
		 */
		~Pillar();

		//! Displays the distance value
		/*!
		 * \param distance the distance value 
		 */
		void showDistance(double distance);
		
		//! Hides the distance from being displayed
		/*
		 */
		void hideDistance();

		//! Draws the Item
		/*!
		 * Used from Qt's main loop.
		 * \param *painter the painter
		 * \param *option style information
		 * \param *widget the widget
		 */
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

		//! The items bounding rect
		/*
		 */
		QRectF boundingRect() const;




	private:

		//! Distance of the current measument
		/*
		 */
		double myDistance;

		//! Flag if the distance is shown
		/*
		 */
		bool shown;
		
		
};

#endif
