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


#include "pillar.h"

#include <QPainter>

#include "defines.h"

Pillar::Pillar():
		myDistance(0.0),
		shown(false)
{
	setZValue(2.0);
}

Pillar::~Pillar()
{
}

void Pillar::showDistance(double distance)
{
	shown = true;
	myDistance = distance;
}

void Pillar::hideDistance()
{
	shown = false;
}

void Pillar::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	painter->setRenderHint(QPainter::Antialiasing);
	if(shown)
	{
		painter->rotate(-90.0);
		painter->drawText(0,-5, QString("%1").arg(myDistance / PPME,0, 'f', 2));
	}
	painter->setPen(Qt::NoPen);
	
	painter->setBrush(Qt::green);
	painter->drawEllipse((int)(-0.1 * PPME), (int)(-0.1 * PPME), (int)(0.2 * PPME),(int)(0.2 * PPME));
}

QRectF Pillar::boundingRect() const
{
	return QRectF(-0.1 * PPME, -0.1 * PPME, 0.1 * PPME, 0.1 * PPME);
}
