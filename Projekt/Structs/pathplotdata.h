#ifndef PATHPLOTDATA_H
#define PATHPLOTDATA_H

#include <QVector>
#include <QPair>
#include <QObject>

/**
 * @brief The PathPlotData struct is the data holder for plotting the path
 */
struct PathPlotData
{
    /**
     * @brief The DataTypeEnum enum for waypoints or spline, enum showing the source of the data
     */
    enum DataTypeEnum {WAYPOINTS,
                       SPLINE
                      } dataType;

    /**
     * @brief PathPlotData is Default constructor (empty vectors etc).
     */
    PathPlotData()
    {
        data = QVector< Point >();
        waypoints = QList<QPair<double, double> >();
        splineX = QVector<double>();
        splineY = QVector<double>();
        robot = QPair<double,double>();
        target = QPair<double,double>();
    }

    /**
     * @brief The Point struct represent Waypoints data coming from PathPlanning
     */
    struct Point {
        double x, /**< x value in m*/
               y, /**< y value in m*/
               value; /**< scalar potential field*/
    };
    QVector< Point > data; /**< grid points containing the travel cost values*/
    int dataSizeX,  /**< size in x direction*/
        dataSizeY;  /**< size in y direction*/

    QList<QPair<double, double> > waypoints; /**< caluclated waypoints in X/Y space*/
    QPair<double,double> robot; /**< position of the robot (at path planning start) */
    QPair<double,double> target; /**< target position (at path planning start) */

    QVector<double> splineX, /**< spline in x direction*/
                    splineY; /**< spline in y direction*/
    double splineLength; /**< length of the spline QVectors*/
};

Q_DECLARE_METATYPE(PathPlotData)

#endif //PATHPLOTDATA_H
