#ifndef PIDPARAMS_H
#define PIDPARAMS_H

#include <QObject>

/**
 * @brief The PIDParams struct, values for the PID controler for angular PID and velocity PID.
 */
struct PIDParams
{
        double PID_A_P; /**< P of angular PID*/
        double PID_A_I; /**< I of angular PID*/
        double PID_A_D; /**< D of angular PID*/
        double PID_V_P; /**< P of velocity PID*/
        double PID_V_I; /**< I of velocity PID*/
        double PID_V_D; /**< D of velocity PID*/
};

Q_DECLARE_METATYPE(PIDParams)


#endif // PIDPARAMS_H
