#ifndef LOG_H
#define LOG_H

#include <atomic>

#include <QtMsgHandler>

#include "Structs/logparams.h"

class MainWindow;

/// \see http://qt-project.org/doc/qt-5/qtglobal.html#qInstallMessageHandler
/**
 * @brief The Log class
 */
class Log
{
public:
    static std::atomic_bool streamLogEnabled;
    static LogParams logParams;

    static void setMainWindowReference(MainWindow* mainWindow);

    static void customLogger(QtMsgType type, const QMessageLogContext &context, const QString &msg);
private:
    static MainWindow* mainWindow;
    Log(); // hidden

};

#endif // LOG_H
