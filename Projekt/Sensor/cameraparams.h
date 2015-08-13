#ifndef CAMERAPARAMS_H
#define CAMERAPARAMS_H

#include <QColor>

/**
 * @brief The CameraParams struct represents the cam params for calibration
 */
struct CameraParams
{
    int source,      /**< stream source*/
        updatePeriod, /**< stream update rate*/
        width, /**< stream width*/
        height; /**< stream height*/

    QColor colorGreenMin, /**< minimal value for color green*/
           colorGreenMax, /**< maximal value for color green*/
           colorBlueMin, /**< minimal value for color blue*/
           colorBlueMax, /**< maximal value for color blue*/
           colorYellowMin, /**< minimal value for color yellow*/
           colorYellowMax;/**< maximal value for color yelow*/
};

Q_DECLARE_METATYPE(CameraParams)

#endif // CAMERAPARAMS_H
