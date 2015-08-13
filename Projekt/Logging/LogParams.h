#ifndef LOGPARAMS_H
#define LOGPARAMS_H

/**
 * @brief The LogParams struct describes the current logging level
 */
struct LogParams
{
    enum logLevelEnum{DEBUG=0, WARNING=1, CRITICAL=2, FATAL=3} logLevel;
    bool logActor, logAI, logData, logPlots, logSensor, logOthers;
};

#endif // LOGPARAMS_H
