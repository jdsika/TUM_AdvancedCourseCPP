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


#include "playground.h"
#include "pillar.h"
#include "defines.h"
#include "angelinaapplication.h"

#include <math.h>

#include <QPainter>

void Playground::reset()
{
	robots.clear();
}

void Playground::placeRobot(const QColor &color, int teamID, double xpos, double ypos)
{
	robots[teamID].color = color;
	robots[teamID].x = xpos;
	robots[teamID].y = ypos;
	update();
}

void Playground::activateMeasurement(const QPointF &pos)
{
	for(int i = 0; i < pillarList.size(); i++)
	{
		QPointF distVec = pillarList[i]->pos() - pos;
		double dist = sqrt(distVec.x() * distVec.x() + distVec.y() * distVec.y());
		pillarList[i]->showDistance(dist);
	}
	update();
}

void Playground::deactivateMeasurement()
{
	for(int i = 0; i < pillarList.size(); i++)
	{
		pillarList[i]->hideDistance();
	}
	update();
}

void Playground::generateNewShape(double a, double b)
{
	// TODO: Remove this function and also shape(), otherwise the collision detection of QGraphicsScene will fail
	shapeP = QPainterPath();
	if(a && b)
	{
		myA = a * PPME;
		myB = b * PPME;
	}
	double currentX = 0.0;
	double currentY = 0.0;

	shapeP.addRect(currentX, currentY, 3.0*myA, myB);

	// Upper border of playground
	pillarList[0]->setPos(currentX, currentY);
	currentX += myA/4.0;
	pillarList[1]->setPos(currentX, currentY);
	currentX += myA*(3.0/4.0);
	pillarList[2]->setPos(currentX, currentY);
	currentX += myA/2.0;
	pillarList[3]->setPos(currentX, currentY);
	currentX += myA/2.0;
	pillarList[4]->setPos(currentX, currentY);
	currentX += myA*(3.0/4.0);
	pillarList[5]->setPos(currentX, currentY);
	currentX += myA/4.0;
	pillarList[6]->setPos(currentX, currentY);

	// Lower border of playground
	currentX = 0.0;
	currentY += myB;
	pillarList[7]->setPos(currentX, currentY);
	currentX += myA/4.0;
	pillarList[8]->setPos(currentX, currentY);
	currentX += myA*(3.0/4.0);
	pillarList[9]->setPos(currentX, currentY);
	currentX += myA/2.0;
	pillarList[10]->setPos(currentX, currentY);
	currentX += myA/2.0;
	pillarList[11]->setPos(currentX, currentY);
	currentX += myA*(3.0/4.0);
	pillarList[12]->setPos(currentX, currentY);
	currentX += myA/4.0;
	pillarList[13]->setPos(currentX, currentY);
}

void Playground::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	painter->setRenderHint(QPainter::Antialiasing);

	// Actual playground
	painter->setPen(Qt::black);
	painter->setBrush(Qt::lightGray);
	painter->drawPath(shapeP);
	
	// Blue goal
	painter->setPen(Qt::NoPen);
	painter->setBrush(QColor(128,128,255));
	painter->drawRect(myA/4.0, myB/2.0 - myB/6.0, myA/4.0, myB/3.0);

	// Yellow goal
	painter->setBrush(Qt::yellow);
	painter->drawRect(3.0*myA - myA/4.0, myB/2.0 - myB/6.0, -myA/4.0, myB/3.0);

	// Red lines (goals, center)
	painter->setPen(QPen(Qt::red, 2.0, Qt::SolidLine));
	painter->drawLine(myA/4.0, 0, myA/4.0, myB);
	painter->drawLine(1.5*myA, 0, 1.5*myA, myB);
	painter->drawLine((2.0 + 3.0/4.0)*myA, 0, (2.0 + 3.0/4.0)*myA, myB);

	// Blue lines (neutral zone)
	painter->setPen(QPen(Qt::blue, 2.0, Qt::SolidLine));
	painter->drawLine(myA, 0, myA, myB);
	painter->drawLine(2.0*myA, 0, 2.0*myA, myB);

	//Robots
	foreach(Robot robot, robots)
	{
		painter->setPen(Qt::black);
		painter->setBrush(robot.color);
		/* Actual robot is 30x20 cm, but we don't know the orientation, so we're
		 * painting a circle.
		 */
		painter->drawEllipse(-0.15*PPME + robot.x*PPME, -0.15*PPME + robot.y*PPME, 0.3*PPME, 0.3*PPME);
	}
}

QRectF Playground::boundingRect() const
{
	// Additional surroundings, looks better than no border at all
	return QRectF(-0.5*PPME, -0.5*PPME, 3.0*myA + 1.0*PPME, myB + 1.0*PPME);
}

double Playground::getA() const
{
	return myA / PPME;
}

double Playground::getB() const
{
	return myB / PPME;
}


Playground::Playground():
	myA(angelinaApp->getDefaultA() * PPME),
	myB(angelinaApp->getDefaultB() * PPME)
{
	for(int i = 0; i < 14; i++)
	{
		Pillar *pillar = new Pillar;
		pillar->setParentItem(this);
		pillarList << pillar;
	}
	generateNewShape();
}
