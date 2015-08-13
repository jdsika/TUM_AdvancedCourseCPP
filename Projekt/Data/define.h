#ifndef DEFINE_H
#define DEFINE_H

#define _USE_MATH_DEFINES

#include <QString>
#include <QBrush>
#include <QPen>

#include "Structs/position.h"

//!
//!This namespace will cover the static const declaration from global namespace
//!
namespace config {


/***********************************************************/
/****                      GEOMETRY                     ****/
/***********************************************************/
static const double geoFieldWidth = 3.0;                    /**< Field width in m*/
static const double geoFieldHeight = 5.0;                   /**< Field height in m*/
static const double geoGoalWidth = 1.0;                     /**< Width of the colored goal area */
static const double geoGoalHeight = (5.0/3.0)/4.0;          /**< Height of the colored goal area*/
static const double geoGoalMarginBottom = (5.0/3.0)/4.0;
static const double geoGoalMarginSide = 1.0;
static const double geoRobotForkCenterDist = 0.20;          /**< distance between laser and puckfork center from laser sensor*/
static const double geoPol_1_2 = (5.0/3.0)/4.0;             /**< a/4 distance between pole 1&2 and 13&14  */
static const double geoPol_2_3 = 0.75*(5.0/3.0);            /**< 3a/4 distance between pole 2&3 and 12&13*/
static const double geoPol_3_4 = (5.0/3.0)/2.0;             /**< a/2 distance between pole 3&4 and 11&12*/
static const double geoPol_1_14 = geoFieldWidth;            /**< b distance between 1&14 ; 2&13 ; 3&12 and 4&11*/
static const double geoPoleRadiusReal = 0.03;               /**< pole radius in m real value*/
static const double geoPoleRadiusSim = 0.06;                /**< pole radius in m simu value*/
static const double geoPuckRadiusTopReal = 0.02;            /**< puck radius in m real value*/
static const double geoPuckRadiusTopSim = 0.03525;          /**< puck radius in m simu value*/
static const double geoPuckRadiusBottom = 0.13 / 2.0;       /**< puck radius in m*/

/***********************************************************/
/****              CONSTS FOR GAMEENGINE_H              ****/
/***********************************************************/
static const int    teamID = 7;                 /**< groups team id*/
static const double periodAlive = 1000;         /**< how often the keep alive signal should be send to server*/
static const double periodEgoPos = 250;         /**< how often our position will be transmitted*/
static const QString refIP = "192.168.1.2";       /**< Angelina IP */
static const int    refPort = 10000;            /**< Angelina Port */
static const bool   refVerbose = false;         /**< allow debugging-msgs of referee*/
static const int periodTillAnnoy = 250000;      /**< how often will we update the internal time*/

/***********************************************************/
/****               CONST FOR MAINWINDOW_H              ****/
/***********************************************************/
static const int GUIopponent = 0;               /**< Definition of the Foe*/
static const int GUIself = 1;                   /**< Definition of self*/
static const int GUIpuckOpponent = 2;           /**< Definition of the opponent puck*/
static const int GUIpuckSelf = 3;               /**< Definition of the own puck*/
static const int GUIpuckUndef = 4;              /**< Definition of the unknown puck*/
static const int GUItarget = 5;                 /**< Definition of the target (movement)*/
static const double GUIREMOTEVELOCITY = 0.1;    /**< Remote control tangential velocity*/
static const double GUIREMOTETURNRATE = 0.174;  /**< Remote control angular velocity*/

static const double guiPuckKlickTolerance = 10/100.0; ///< wie nah muss auf das Zentrum eines Pucks in der Map geklickt werden, um ihn als geklickt wahrzunehmen. In Meter, bzw 100px

static const QPen guiPenOpponent(QBrush(QColor(201,40,27)), 4.0, Qt::SolidLine);
static const QPen guiPenMe(QBrush(QColor(109,191,55)), 4.0, Qt::SolidLine);
static const QBrush guiBrushMe(QBrush(QColor(109,191,55)));
static const QPen guiPenOrient(QBrush(QColor(133,100,84)), 1.0, Qt::SolidLine);
static const QPen guiPenDummy(QBrush(QColor(Qt::cyan)), 1.0, Qt::SolidLine);
static const QPen guiPenTarget(QBrush(QColor(Qt::gray)), 1.0, Qt::SolidLine);
static const QPen guiPenPole(QBrush(QColor(109,191,55)), 1.0, Qt::SolidLine);
static const QBrush guiBrushPole(QBrush(QColor(109,191,55)));
static const QPen guiPenPuckMe(QBrush(QColor(109,191,55)), 2.0, Qt::SolidLine);
static const QPen guiPenPuckOpponent(QBrush(QColor(201,40,27)), 2.0, Qt::SolidLine);
static const QPen guiPenPuckUndef(QBrush(QColor(127,127,127)), 2.0, Qt::SolidLine);
static const QPen guiPenPuckMeOuter(QBrush(QColor(109,191,55, 127)), 2.0, Qt::SolidLine);
static const QPen guiPenPuckOpponentOuter(QBrush(QColor(201,40,27, 127)), 2.0, Qt::SolidLine);
static const QPen guiPenPuckUndefOuter(QBrush(QColor(127,127,127,127)), 2.0, Qt::SolidLine);
static const QPen guiPenFieldPrimary(QBrush(QColor(Qt::white)), 1.0, Qt::SolidLine); //Feldumrandung
static const QPen guiPenFieldSecondary(QBrush(QColor(Qt::lightGray)), 1.0, Qt::SolidLine); // Feldaufteilungslinien
static const QBrush guiBrushPuckMoving(QColor(QColor(215, 69, 232))); // Lila - Puck moving
static const QBrush guiBrushPuckBlocked(QColor(QColor(Qt::black)));
static const QBrush guiBrushGoalBlue(QColor(QColor(38,66,115)));
static const QBrush guiBrushGoalYellow(QColor(QColor(255,211,36)));
static const QBrush guiBrushGoalUndef(QColor(Qt::lightGray));

static const int ROBOSIZE_CM = 40;          /**< Robots diameter in cm */
static const int PUCKSIZE_INNER_CM = 5;     /**< Puck size  in cm */
static const int PUCKSIZE_OUTER_CM = 13;    /**< Puck size  in cm */
static const int POLESIZE_CM = 6;           /**< Pole size in cm */
static const int BORDERTOP_CM = 75;         /**< Distance between start region (right and left) in cm*/
static const int BORDERSIDE_CM = 75;        /**< Distance between (side) in cm*/
static const int FIELDHEIGHT_CM = geoFieldHeight*100;      /**< Field height in cm*/
static const int FIELDWIDTH_CM = geoFieldWidth*100;       /**< Field width in cm*/
static const int TARGETSIZE_CM = 20;                      /**< Target size in cm*/
static const int PATH_SHOWRES = 160;                     /**< resolution of pathplanning map */

/***********************************************************/
/****                   CONSTS FOR SENSOR_H             ****/
/***********************************************************/
// MaxRange
static const double SENSOR_OUT_OF_FIELD_TOLERANCE = 0.15;                                   /**< Region of influence from outer field*/
static const double SENSOR_OBJECTWIDTH_ROBO = 0.25;                                         /**< everything over this size seems to be the opponent */
static const double SENSOR_MAX_DISTANCE_OF_OBJ = 0.08;                                      /**< maximal distance to an obj (area of interest)*/
static const double SENSOR_MAX_RANGE_ORIENTATION = 3.40;                                    /**< maximal value during the orientation stage*/
static const double SENSOR_MAX_RANGE_RECOGNITION = sqrt(geoFieldWidth * geoFieldWidth +
                                                        geoFieldHeight* geoFieldHeight) +
                                                        SENSOR_OUT_OF_FIELD_TOLERANCE;      /**< maximal value during the recognition stage*/

static const double SENSOR_COLLISION_AT = 0.5;                  /**< Recognition of collisions in m*/
static const double SENSOR_DELTA_ANGLE = M_PI/180*10;           /** Angledelta for 180° rotation in sensor*/
static const double SENSOR_RADIUS_ROBOT = 0.23;                 /**< robot radius in m*/
static const int    SENSOR_WAIT_COUNTER = 10;                   /**< Iterations to wait*/
static const double SENSOR_MEASUREMENT_DEVIATION = 0.15;        /**< Abort if deviation is higher than Xcm*/

/***********************************************************/
/****                   CONSTS FOR CAM                  ****/
/***********************************************************/
//all values relative to the dimensions of campicture
static const double CAM_ROI_WIDTH_RELATIVE = 0.2;                                           /**< Width of ROI for color detection*/
static const double CAM_ROI_HEIGHT_RELATIVE = 0.3;                                          /**< Height of ROI for color detection*/
static const double CAM_ROI_OFFSET_HORIZONTAL_RELATIVE = 0.5 - CAM_ROI_WIDTH_RELATIVE / 2;  /**< OFFSET for the height of ROI (relative to center of picture)*/
static const double CAM_ROI_OFFSET_VERTICAL_RELATIVE = 1 - 0.05 - CAM_ROI_HEIGHT_RELATIVE;  /**< OFFSET for the height of ROI (relative to center of picture)*/
static const double CAM_PERCENTAGE_NON_COLOR_DETECTION = 0.5;                               /**< With 50% non-color you cannot recognize any color at all*/


/***********************************************************/
/****            CONSTS FOR ORIENTIERUNG_H              ****/
/***********************************************************/
static const double ORIENTATION_APPROXIMATION_VALUE = 0.05;     /**< value of allowed distance variance of the poles */
static const double ORIENTATION_SENSOR_ODOMETRIE_DELTA = 0.03;  /**< delta between the x,y odometry origin and the laser sensor origin*/

/***********************************************************/
/****              CONSTS FOR GAME_H                    ****/
/***********************************************************/
static const double DIST_TO_PUCK_BEFORE_GATHERING_IT = 0.40;    /**< distance from robot center to puck*/

static const Position gameGoalSlotL(geoGoalMarginSide - geoRobotForkCenterDist,
                                    geoFieldHeight - geoGoalMarginBottom - geoGoalHeight/2.0,
                                    0);             /**< A position slot on the left side of the enemy goal area to place puck*/

static const Position gameGoalSlotM(geoGoalMarginSide + 0.5 * geoGoalWidth,
                                    geoFieldHeight - geoGoalMarginBottom - geoGoalHeight - geoRobotForkCenterDist,
                                    M_PI_2);        /**< A position slot in the middle of the enemy goal area to place puck*/

static const Position gameGoalSlotR(geoGoalMarginSide + 1.0 * geoGoalWidth + geoRobotForkCenterDist,
                                    geoFieldHeight - geoGoalMarginBottom - geoGoalHeight/2.0,
                                    M_PI);          /**< A position slot on the right side of the enemy goal area to place puck*/

static const Position gameGoalSlotOutsideLeft(0.50 * geoGoalMarginSide,
                                              geoFieldHeight - geoGoalMarginBottom - 2* geoGoalHeight,
                                              3.0/4.0 * M_PI);  /**< A position slot outside of the enemy goal area to place puck*/

static const Position gameGoalSlotOutsideRight(geoFieldWidth - 0.50 * geoGoalMarginSide,
                                                geoFieldHeight - geoGoalMarginBottom - 2* geoGoalHeight,
                                                1.0/4.0 * M_PI);  /**< A position slot outside of the enemy goal area to place puck*/

static const Position gameAnnoyPositionL(geoGoalMarginSide,
                                         geoGoalMarginBottom + geoGoalHeight/2.0,
                                         0);    /**< A position slot in order to annoy the enemy while standing in my goal area*/
static const Position gameAnnoyPositionR(geoGoalMarginSide + geoGoalWidth,
                                         geoGoalMarginBottom + geoGoalHeight/2.0,
                                         M_PI); /**< A position slot in order to annoy the enemy while standing in my goal area*/

static const double gameWieWeitMussDerGegnerVomZielEntferntSeinImAnnoyModus = 1.0;
static const double DUMP_SLOT_2_3 = 1.042;              /**< y-Value for the SLOT between the Pole 2&3*/
static const double DUMP_SLOT_3_4 = 2.083;              /**< y-Value for the SLOT between the Pole 3&4*/
static const double DUMP_SLOT_4_5 = 2.917;              /**< y-Value for the SLOT between the Pole 4&5*/
static const double DUMP_SLOT_5_6 = 3.958;              /**< y-Value for the SLOT between the Pole 5&6*/
static const double TARGET_POLE_VARIANCE = 0.13;        /**< Wert um die der Target Pole verschoben sein darf*/
static const double DISTANCE_TO_WAITING_LINE =  DIST_TO_PUCK_BEFORE_GATHERING_IT;    /**< m die vor der neutralen Zone eingehalten werden*/
static const double gameBisZuWelchemAbstandWirdZielwackelnGefiltert = 0.10;
static const double gameZielwackelfilterTiefpassKoeffizient = 1e-1;
static const double gameWievielBesserMussEinPuckSeinUmDasZielZuWechseln = 0.1;       ///< in Prozent
static const double gameMinimumDistanceEnemyToDumpSlot = 1.0;                        ///< Wie weit muss der Gegner davon entfernt sein, damit ein Dump Slot ausgewählt werden kann
static const double gameMinimumDistanceToEnemyRobot = 1.5 * SENSOR_RADIUS_ROBOT;

// zum finden idealer Pucks
static const double puckIsCloseToPoleDistance = 0.50;

/***********************************************************/
/****              CONSTS FOR PATHPLANNING_H            ****/
/***********************************************************/
static const int    pathMaxWPIterations = 10000;                        /**< how many times to get the next gradient point to find the target, before giving up*/
static const double pathGridSpacingBase   = 0.05;                       /**< value to use for path planning grid. smaller is more precise and computationally expensive. in meters */
static const double pathPlanningEnabledUpwardsOfThisDistance = 0.10;    /**< Wie Nah muss der Roboter dem Ziel sein, damit die Pfadplanung keine Punkte mehr erzeugt, sondern nur den Zielpunkt direkt ausgibt*/
static const double pathArenaMinX = 0;                                  /**< min of x-axis*/
static const double pathArenaMaxX = geoFieldWidth;                      /**< max of x-axis*/
static const double pathArenaMinY = 0;                                  /**< min of y-axis*/
static const double pathArenaMaxY = geoFieldHeight;                     /**< max of y-axis*/
static const double pathArenaFieldAvoidMaxY = geoPol_1_2 + geoPol_2_3;  /**< max of y-axis in field avoidance mode*/

static const double pathRobotRadius = 0.26;

static const double pathPoleCloseDist = pathRobotRadius + 0.10 + geoPoleRadiusReal; /**< the robot can't approach closer than this. in meters*/
static const double pathPoleCloseCost = 100.0;                                      /**< a high value that makes the robot not want to collide (close to the acceptable detour for not coming here)*/
static const double pathPoleStartCost = 5.0;                                        /**< a medium value that makes the robot only go here if necessary (close to the acceptable detour for not coming here)*/
static const double pathPoleFarDist   = pathPoleCloseDist + 0.2;                    /**< an obstacle does not cause costs if it is further away than this. in meters*/

static const double pathEnemyCloseDist = pathRobotRadius + 0.10 + pathRobotRadius;  /**< the robot can't approach closer than this. in meters*/
static const double pathEnemyCloseCost = 10.0;                                      /**< a high value that makes the robot not want to collide (close to the acceptable detour for not coming here)*/
static const double pathEnemyStartCost = 1.0;                                       /**< a medium value that makes the robot only go here if necessary (close to the acceptable detour for not coming here)*/
static const double pathEnemyFarDist   = pathEnemyCloseDist + 0.30;                 /**< an obstacle does not cause costs if it is further away than this. in meters*/

static const double pathPuckCloseDist = pathRobotRadius + 0.01 + geoPuckRadiusBottom;   /**< the robot can't approach closer than this. in meters*/
static const double pathPuckCloseCost = 0.1;                                            /**< a high value that makes the robot not want to collide (close to the acceptable detour for not coming here)*/
static const double pathPuckStartCost = 0.05;                                           /**< a medium value that makes the robot only go here if necessary (close to the acceptable detour for not coming here)*/
static const double pathPuckFarDist   = pathPuckCloseDist + 0.10;                       /**< an obstacle does not cause costs if it is further away than this. in meters*/

static const double pathTargetApproachAngleInfluenceDistance = 0.0;                         /**< Bis zu welcher Entfernung vom Ziel wird der Anfahrtswinkel die intrinsischen Kosten beeinflussen?*/
static const double pathTargetApproachAngleMaxDeviationWithoutFullCost = 15.0 *M_PI/180.0;  /**< Beeinflusst Öffnungswinkel des Anfahrtswinkel-Potentialfelds*/
static const double pathTargetApproachAngleFullCost = 100.0;

static const double pathAdjacencyMultiplier = 50.0;

/***********************************************************/
/****              CONSTS FOR ACTORHIGHLEVEL_H          ****/
/***********************************************************/
static const double actorWaypointReachedDistance = 0.08;                        ///< Theshold in m to destination is reached
static const double actorWaypointReachedDiffChange = 0;
static const double actorWaypointMaxAngleDeviation = 2.5 /180.0*M_PI;           ///< Wie weit darf der Roboterwinkel vom Zielwinkel abweichen um noch als erreicht zu gelten
static const double actorDistanceOfTargetOnSpline = 0.2/ pathGridSpacingBase;   ///< Wie weit soll der PID-Sollpunkt dem der Roboter hinterherfährt auf dem Spline maximal entfernt sein (in #Wegpunkten, muss keine ganze Zahl sein)
static const double actorGatherPuckDistance = DIST_TO_PUCK_BEFORE_GATHERING_IT -
                                              geoRobotForkCenterDist + 0.05;    ///< Wie weit wird vorwärts gefahren um einen Puck aufzunehmen
static const double actorReleasePuckDistance = 0.25;                            ///< Wie weit wird beim Puck loslassen zurückgefahren
static const double actorPushPuckDistance = 0.20;                               ///< Wie weit wird der Puck aus der Arena gefahren
static const double actorPushAndReleaseAdditionalReverseDist = 0.025;           ///< Wie weit wird mehr zurück gefahren als vor bei push and release
static const double actorPeriodMotionControl = 1000.0 / 200.0;                  ///< Wie schnell wird der PID Regler ausgeführt (1000.0 / x Hz)
static const double actorWPLowPassAlpha = 1e-20;                                ///< Koeffizient bei Wegpunkt-Tiefpass. Sollte nahe, aber nicht 0 sein. Guter Wert: (1.0 / config::actorPeriodMotionControl) / ((1.0 / config::actorPeriodMotionControl) + (config::actorPeriodMotionControl+1))
static const double actorMinAngleLimiter = 15.0 *M_PI/180.0;
static const double actorMaxAngleLimiter = 60.0 *M_PI/180.0;
static const double actorMaxI = 10;                                             ///< Maximaler PID-I-Anteil
static const double actorLowPass = 1e-0;                                        ///< Tiefpassfilterkoeffizient für Winkel (1e-10 = stark, 1e-0 = aus)

/***********************************************************/
/****               CONSTS FOR OBSTACLE_H               ****/
/***********************************************************/
static const double obstacleCoordinateTolerance = 0.10;                         /**< tolerance in m in which an two obstacles will recognised as one*/
static const int obstacleNumberOfPucks = 6;                                     /**< number of pucks*/
static const int obstacleNumberOfPoles = 14;                                    /**< number of poles*/

/***********************************************************/
/****               CONSTS FOR MAPDATA                  ****/
/***********************************************************/
static const double mapPolePuckFusionDistance = geoPoleRadiusReal + geoPuckRadiusBottom + 0.20;
static const double mapPuckPuckFusionDistance = 3 * geoPuckRadiusBottom; ///< Abstand, bei dem zwei Pucks zu einem zusammengefasst werden (m). (Wenn der Abstand zwischen zwei Pucks exakt 2*Puckradius ist, berühren sie sich bereits)
static const double mapIgnorePuckInsideEnemyDistance = SENSOR_RADIUS_ROBOT + 0.5 * geoPuckRadiusBottom; ///< Wenn ein Puck innerhalb diesen Abstands vom Gegner erkannt wird, ist es gar kein Puck
static const double mapAbstandRoboterZentrumZuGabel = 0.20;
static const double mapToleranzBisWohinEinPuckInDerGabelIstMIN = 0.17;  /**< Minimum value for puck is in fork calculation */
static const double mapToleranzBisWohinEinPuckInDerGabelIstMAX = 0.27;  /**< Maximum value for puck is in fork calculation */

/***********************************************************/
/****                   CONSTS FOR DEBUG                ****/
/***********************************************************/
// Orientation
static const bool enableDebugMapData = false;           /**< Enable Debug information for MapData*/
static const bool enableDebugOrientation = false;       /**< Enable Debug information for orientation*/
static const bool enableDebugActorLowLevel = false;     /**< Enable Debug information for actorLowLevel*/
static const bool enableDebugActorHighLevel = false;    /**< Enable Debug information for actorHighLevel*/
static const bool enableDebugSensorLowLevel = false;    /**< Enable Debug information for sensorLowLevel*/
static const bool enableDebugSensorHighLevel = false;   /**< Enable Debug information for sensorHighLevel*/
static const bool enableDebugPathPlanning = false;      /**< Enable Debug information for PathPlanning*/
static const bool enableDebugGame = false;               /**< Enable Debug information for Game.cpp*/
static const bool enableDebugMainwindow = false;        /**< Enable Debug information for mainwindow.cpp*/
static const bool enableDebugCam = false;               /**< Enable Debug information for cam.cpp*/
}

#endif // DEFINE_H
