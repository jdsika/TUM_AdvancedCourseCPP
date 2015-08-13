#include "orientierung.h"

#include <QtCore>
#include <QDebug>
#include <QLineF>

#include "Data/define.h"
#include "Data/mapdata.h"
#include "Structs/position.h"

#include "Sensor/trilateration.h"


bool Orientation::checkObjectsOnLine(const Position &erstesObjekt, const Position &zweitesObjekt, const Position &drittesObjekt)
{
    QPair<double,double> erstesKart  = QPair<double,double>(erstesObjekt.x() * qCos(erstesObjekt.y()),
                                                            erstesObjekt.x() * qSin(erstesObjekt.y()));

    QPair<double,double> zweitesKart = QPair<double,double>(zweitesObjekt.x() * qCos(zweitesObjekt.y()),
                                                            zweitesObjekt.x() * qSin(zweitesObjekt.y()));

    QPair<double,double> drittesKart = QPair<double,double>(drittesObjekt.x() * qCos(drittesObjekt.y()),
                                                            drittesObjekt.x() * qSin(drittesObjekt.y()));

    double A = zweitesKart.first  - erstesKart.first;
    double B = zweitesKart.second - erstesKart.second;
    double C = drittesKart.first  - erstesKart.first;
    double D = drittesKart.second - erstesKart.second;

    double distance = fabs(A * D - C * B) / qSqrt(C * C + D * D);

    if (distance < config::ORIENTATION_APPROXIMATION_VALUE)
        return true;
    else
        return false;

}

// Wird nach der Initialisierung des Sensors aufgerufen
// In der Startphase werden die Abstände aller gefunden Objekte untersucht,
// ob sie den Pfostenabständen entsprechen Wurden zwei Pfopsten eindeutig idetifiziert,
// wir anhand des Abstandes der Schnittpunkt berechnet
// und als Position des Roboters eingetragen
// QVector3D enthält: tiefe, winkel radian, breite
Position Orientation::beginOrientation(QList<Position> &objects)
{

    if(config::enableDebugOrientation)
        qDebug() << "Start new Orientation";

    int vectorSize = objects.size();
    double distance1 = 0.0;
    double distance2 = 0.0;
    double distance3 = 0.0;

    for(int firstLoop = 0; firstLoop < vectorSize; ++firstLoop)
    {
        // reducing the possible combinations
        for(int secondLoop = firstLoop + 1; secondLoop < vectorSize; ++secondLoop)
        {
            distance1 = distancePolar(objects.at(firstLoop).y(), objects.at(firstLoop).x(),
                                      objects.at(secondLoop).y(), objects.at(secondLoop).x());


            if (distance1 > config::geoPol_1_2 - config::ORIENTATION_APPROXIMATION_VALUE &&
                    distance1 < config::geoPol_1_2 + config::ORIENTATION_APPROXIMATION_VALUE)
            {
                // FOUND DISTANCE 1_2 !!!
                for(int thirdLoop = 0; thirdLoop < vectorSize; ++thirdLoop)
                {            
                    if (thirdLoop != secondLoop)
                    {
                        distance2 = distancePolar(objects.at(secondLoop).y(), objects.at(secondLoop).x(),
                                                  objects.at(thirdLoop).y(), objects.at(thirdLoop).x());

                        if (distance2 > config::geoPol_2_3 - config::ORIENTATION_APPROXIMATION_VALUE &&
                                distance2 < config::geoPol_2_3 + config::ORIENTATION_APPROXIMATION_VALUE)
                        {
                            if(checkObjectsOnLine(objects.at(firstLoop),objects.at(secondLoop),objects.at(thirdLoop)))
                            {
                                if(config::enableDebugOrientation)
                                    qDebug() << "Folge erkannt! - Variation 1";
                                Position berechnet = getGlobalPosition(objects.at(firstLoop),objects.at(secondLoop),objects.at(thirdLoop));
                                if (berechnet.isPositionInStartField())
                                    return berechnet;
                            }
                            if(config::enableDebugOrientation)
                                qDebug() << "Positionen gefunden, aber nicht auf einer Gerade";
                        }
                    }

                    if (thirdLoop != firstLoop) {
                        distance3 = distancePolar(objects.at(firstLoop).y(), objects.at(firstLoop).x(),
                                                  objects.at(thirdLoop).y(), objects.at(thirdLoop).x());

                        if (distance3 > config::geoPol_2_3 - config::ORIENTATION_APPROXIMATION_VALUE &&
                                distance3 < config::geoPol_2_3 + config::ORIENTATION_APPROXIMATION_VALUE)
                        {
                            if(checkObjectsOnLine(objects.at(firstLoop),objects.at(secondLoop),objects.at(thirdLoop)))
                            {
                                if(config::enableDebugOrientation)
                                    qDebug() << "Folge erkannt! - Variation 2";
                                Position berechnet = getGlobalPosition(objects.at(secondLoop),objects.at(firstLoop),objects.at(thirdLoop));
                                if (berechnet.isPositionInStartField())
                                    return berechnet;
                            }
                            if(config::enableDebugOrientation)
                                qDebug() << "Positionen gefunden, aber nicht auf einer Gerade";
                        }
                    }
                }
            }
        }
    }

    if(config::enableDebugOrientation)
        qWarning() << "Roboter muss drehen, da wir unser Szenario nicht sehen!";
    return Position();

}


Position Orientation::getGlobalPosition(const Position &poleA, const Position &poleB, const Position &poleC)
{

    QPair<double,double> outerIntersection;

    double delta_X;
    double delta_Y;
    double orientation;
    double worldX;
    double worldY;

    int retVal= trilateration::circle_circle_intersection(0, config::geoPol_1_2, poleB.x(),
                                                          0, config::geoPol_1_2 + config::geoPol_2_3, poleC.x(),
                                                          &outerIntersection.first, &outerIntersection.second,
                                                          &delta_X, &delta_Y);

    if(!retVal){
        if(config::enableDebugOrientation)
            qWarning()<<"Error during cycle intersection";
        return Position();

    }

    //    orientation = atan2(yiprime,xiprime)+poleA.second-M_PI_2;
    if(poleA.y() <= poleC.y())
    {
        //links
        orientation = M_PI_2 + atan2(delta_Y,delta_X)+poleA.y();
        worldY = delta_Y;
        worldX = delta_X;
    }
    else
    {
        //rechts
        //Spiegeln der x Koordinaten an der rechten Aussenlinie
        orientation = -atan2(delta_Y,delta_X)+poleA.y() - M_PI_2;
        worldX = config::geoPol_1_14 - delta_X;
        worldY = delta_Y;

    }
    return Position(worldX,worldY,orientation,1.0);
}

// Abstand zwischen zwei 2D Punkten (Polar)
double Orientation::distancePolar(const double &angleA, const double &depthA,
                                   const double &angleB, const double &depthB)
{
    // c² = a² + b² - 2*ab*cos(gamma)
    return qSqrt(depthA * depthA +
                 depthB * depthB -
                 2 * depthA * depthB *
                 qCos(fabs(angleB - angleA)));
}

// Abstand zwischen zwei 2D Punkten (Polar)
double Orientation::distancePolar(const double &depthA, const double &depthB, const double &angleBetweenAB)
{
    // c² = a² + b² - 2*ab*cos(gamma)
    return qSqrt(depthA * depthA +
                 depthB * depthB -
                 2 * depthA * depthB *
                 qCos(angleBetweenAB));
}

double Orientation::angleBetweenAB(const double &depthA, const double &depthB, const double &distC)
{
    return qAcos( (depthA * depthA +
                   depthB * depthB -
                   distC * distC ) /
                  ( 2 * depthA * depthB));
}

Position Orientation::getGlobalPolarPosition(const Position &relativePosition, const Position &robotPosition)
{
    double absAngle = fmod(robotPosition.rot() - (relativePosition.y() - M_PI_2), 2*M_PI);

    return Position(robotPosition.x() + config::ORIENTATION_SENSOR_ODOMETRIE_DELTA * cos(robotPosition.rot()) + relativePosition.x() * cos(absAngle),
                    robotPosition.y() + config::ORIENTATION_SENSOR_ODOMETRIE_DELTA * sin(robotPosition.rot()) + relativePosition.x() * sin(absAngle),
                    absAngle,
                    relativePosition.sizeType());
}
