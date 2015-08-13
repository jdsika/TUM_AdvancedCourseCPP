#ifndef LOGPARAMS_H
#define LOGPARAMS_H

/**
 * @brief The LogParams struct describes the current logging level
 */
struct LogParams
{
    /**
     * @brief The logLevelEnum enum determining the current log level
     */
    enum logLevelEnum{DEBUG=0, WARNING=1, CRITICAL=2, FATAL=3} logLevel;

    bool logActor, logAI, logData, logPlots, logSensor, logMain; /**< check if log level was activated*/
};

#endif // LOGPARAMS_H
