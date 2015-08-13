#ifndef PATHPLANNING_H
#define PATHPLANNING_H

#include <QList>
#include <QPair>

#include <atomic>

#include "Data/obstacle.h"
#include "Structs/pathplotdata.h"

class QElapsedTimer;

class PathPlanning:public QObject
{
    Q_OBJECT

public Q_SLOTS:
    /**
     * @brief planPath this method is called in the robot-thread and will plan the new path
     */
    void planPath();

Q_SIGNALS:
    /**
     * @brief sendUpdatedWaypoints: This signal will emit the newest generated waypoints to the pathrealizer
     * @param[out] waypoints (QList of QPair(double,double)): list with the generated waypoints
     */
    void sendUpdatedWaypoints(QList< QPair<double,double> > waypoints);

    /**
     * @brief pathDisplay, signal for displaying the path planning data in GUI-tab
     * @param[in] dataPacket (PathPlotData)
     */
    void pathDisplay(PathPlotData dataPacket);

public:

    /**
     * @brief PathPlanning constructor of wavefront pathplanning
     */
    PathPlanning();
    ~PathPlanning();

    /**
     * @brief streamPathEnabled determines, if the stream in GUI is enabled.
     */
    static std::atomic_bool streamPathEnabled;

    /**
     * @brief grid2XY calculate real-world X,Y coordinates for any given grid rotation and spacing
     * @param a (const int): component in a-space
     * @param b (const int): component in a-space
     * @param gridRotation (const double): rotation of the given grid
     * @param gridSpacing: (const double) distance between mesh-cells
     * @return the transformed grid.
     */
    static const QPair<double,double> grid2XY(const int a, const int b, const double gridRotation, const double gridSpacing);

    /**
     * @brief grid2AB // calculate grid coordinates (A,B) from XY for any given grid rotation and spacing
     * @param x (const double):component in carthesian space
     * @param y (const double):component in carthesian space
     * @param gridRotation (const double): rotation of the grid
     * @param gridSpacing (const double): space among mesh cells.
     * @return the grid in a;b-components
     */
    static const QPair<double,double> grid2AB(const double x, const double y, const double gridRotation, const double gridSpacing);

    /**
     * @brief getEnabled return if the pathplanning is enabled
     * @return if is enabled
     */
    static bool getEnabled();

    /**
     * @brief setEnabled: set if the pathplanning should be enabled
     * @param[in] value (bool)
     */
    static void setEnabled(const bool &value);

    /**
     * @brief getAvoidRestOfField determines if points outside the field should be ignord.
     * @return bool
     */
    static bool getAvoidRestOfField();

    /**
     * @brief setAvoidRestOfField simple setter of avoidRestOfField-member
     * @param[in] value (const bool)
     */
    static void setAvoidRestOfField(const bool &value);

    static bool getIgnorePucks();
    static void setIgnorePucks(const bool &value);

private:
    class GridPoint; // Declaration: See below

    Obstacle robot;                             /**< own robot obstacle*/
    double robotX;                              /**< x pos in m of robot*/
    double robotY;                              /**< y pos in m of robot*/
    double robotRot;                            /**< robots orientation in rad */
    int robotA;                                 /**< a pos of robot->transformed*/
    int robotB;                                 /**< b pos of robot->transformed*/
    double targetX;                             /**< target position in x in m */
    double targetY;                             /**< target position in x in m */
    double targetRot;                           /**< target orientation in rad*/
    int targetA;                                /**< target position in a*/
    int targetB;                                /**< target position in b*/
    double gridRotation;                        /**< current grid rotation*/
    double gridSpacing;                         /**< current spacing amoung mesh-cells*/
    int gridSizeA;                              /**< number of cells in a-direction*/
    int gridSizeB;                              /**< number of cells in b-direction*/
    double minA, maxA, minB, maxB;              /**< */
    QVector<QVector<GridPoint> > grid;          /**< grid for calculate the path*/
    QElapsedTimer *timer;                       /**< timer, how long the algorithm needed for path calculation */
    QList<Obstacle> obstaclesPuck;              /**< list of puck -> handeled as obstacle*/
    QList<Obstacle> obstaclesPole;              /**< list of pole-> handeled as obstacle*/
    QList<Obstacle> obstaclesEnemy;             /**< list of foes -> handeled as obstacle*/

    static std::atomic_bool enabled;            /**< should the pathplanning algorithm be enabled*/
    static std::atomic_bool avoidRestOfField;   /**< shall the outer side of the field be avoided.*/
    static std::atomic_bool ignorePucks;        /**< should pucks be ignored when planning a path? */

    /**
     * @brief generateGrid will generate a new grid with updated obstacle distances
     */
    void generateGrid();

    /**
     * @brief calculatePathCosts calculate the path costs as sum of intrinsic and extrinsic costs.
     */
    void calculatePathCosts();

    /**
     * @brief calculateWaypoints will calculate the new waypoints from path
     * @return a qpair of grid- and waypoints
     */
    QPair<QList<QPair<double, double> >, QList<QPair<double, double> > > calculateWaypoints();

    /**
     * @brief getGridRotation
     * @return the grid rotation
     */
    const double getGridRotation();

    /**
     * @brief gridIndex returns the 2D-mesh index by given a and b index
     * @param[in] a (int)
     * @param[in] b (int)
     * @return the index in 2D-Mesh of give a and b index
     */
    int gridIndex(int a, int b);

    /**
     * @brief grid2XY converte a given a,b-grid to xy-grid
     * @param a
     * @param b
     * @return a qpair(double,double) of the converted ab grid
     */
    const QPair<double,double> grid2XY(const int a, const int b);

    /**
     * @brief grid2AB converte a given xy-grid to a,b-grid
     * @param a
     * @param b
     * @return a qpair(double,double) of the converted xy grid
     */
    const QPair<double,double> grid2AB(const double x, const double y);


    class GridPoint{
    public:
        /**
         * @brief GridPoint constructor of private member class representing a single grid point
         */
        GridPoint();
        /**
         * @brief GridPoint overloaded constructor  of private member class
         * @param[in] pathPlanning (pointer on the current pathplanning obj)
         * @param[in] gridA (int)
         * @param[in] gridB (int)
         */
        GridPoint(PathPlanning* pathPlanning, int gridA, int gridB);

            PathPlanning* pathPlanning;     /**< member variable of parent obj adress*/
            double positionX;               /**< position representation in x (carthesian space)*/
            double positionY;               /**< position representation in y (carthesian space)*/
            int gridA;                      /**< */
            int gridB;                      /**< */
            double intrinsicCost;           /**< intrinisc costs for this grid point */
            double value;                   /**< overall costs*/
            enum PointType {GRID, TARGET, ROBOT} type;  /**< represent which type this gridpoint is*/
            bool isOutsideArena;                        /**< bool if the gridpoint is outside the arena*/
            bool active;                                /**< bool if the pathplanning is active*/

            /**
             * @brief init // does the initialization for a grid point, e.g. intrinsic cost calculation
             * @param point
             */
            void init(GridPoint &point);
            /**
             * @brief calculateIntrinsicCost will calculate the intrinsic costs for this grid point
             */
            void calculateIntrinsicCost();

            /**
             * @brief getDistance returns the distance to given Gridpoint
             * @param[in] point (pointer to GridPoint)
             * @return the distance to given gridpoint
             */
            double getDistance(const GridPoint* point);

            /**
             * @brief getNeighbors will return the neighbour grid points for this grid point
             * @param[in] includeOutsideArena (bool) if area from outer arena should be included
             * @return a qlist of gridpoint pointers.
             */
            QList<GridPoint*> getNeighbors(bool includeOutsideArena = false);

            /**
             * @brief getGradientPoint will return the neighbour with the smallest value
             * @return pointer to neighbour with lowest value counter
             */
            GridPoint* getGradientPoint();
    private:
            /**
             * @brief constrainAngle will return the adjusted angle (0-360° in rad)
             * @param in (double) angle in rad
             * @return (double)
             */
            double constrainAngle(const double in);
    }; //END class definition GridPoint.


    /**
     * @brief initGridPoint // does the initialization for any grid point, e.g. intrinsic cost calculation
     * @param point (reference of a grid-point)
     */
    static void initGridPoint(GridPoint &point);



};

#endif // PATHPLANNING_H
