#include "log.h"

#include <atomic>
#include <iostream>

#include <QMutex>
#include <QStringList>
#include <QDebug>

#include "GUI/mainwindow.h"

std::atomic_bool Log::streamLogEnabled(false);
LogParams Log::logParams;
MainWindow* Log::mainWindow = nullptr;


void Log::setMainWindowReference(MainWindow *mainWindow)
{
     Log::mainWindow = mainWindow;
}

/**
 * @brief customLogger is a MessageHandler which redirect the qDebug(), qWarning(), qCritical() and qFatal() into our GUI.
 * @param[in] type (QtMsgType) one of QtDebugMsg, QtWarningMsg, QtCriticalMsg, QtFatalMsg
 * @param[in] context (QMessageLogContext) the messages origin
 * @param[in] msg (QString) the message.
 */
void Log::customLogger(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex logMutex;
    logMutex.lock();

    switch(type){
    case QtDebugMsg:
        std::cout << "D " << msg.toStdString() << std::endl;
        break;
    case QtWarningMsg:
        std::cerr << "W " << msg.toStdString() << std::endl;
        break;
    case QtCriticalMsg:
        std::cerr << "C " << msg.toStdString() << std::endl;
        break;
    case QtFatalMsg:
        std::cerr << "F " << msg.toStdString() << std::endl;
        break;
    }

    if(Log::streamLogEnabled) {

        QStringList path = QString(context.file).split("/");
        QString folder = path.at(std::max(path.size()-2,0));

        bool logToGUI = false;
        if(logParams.logActor && folder.compare("Actor")==0) logToGUI = true;
        if(logParams.logAI && folder.compare("AI")==0) logToGUI = true;
        if(logParams.logData && folder.compare("Data")==0) logToGUI = true;
        if(logParams.logPlots && folder.compare("Plots")==0) logToGUI = true;
        if(logParams.logSensor && folder.compare("Sensor")==0) logToGUI = true;
        if(logParams.logMain && folder.compare("Main")==0) logToGUI = true;

        if(type == QtDebugMsg && logParams.logLevel <= LogParams::DEBUG){
            QString html = "<span style=\"color: blue;\"><span style=\"font-size: 8pt;\">";
            html.append(QString(context.function).toHtmlEscaped());
            html.append(":</span> ");
            html.append(QString(msg).toHtmlEscaped());
            html.append("</span><br/>");
            if(mainWindow && logToGUI) QMetaObject::invokeMethod(mainWindow, "slotLog", Qt::AutoConnection , Q_ARG(QString, html));
        } else if(type == QtWarningMsg && logParams.logLevel <= LogParams::WARNING){
            QString html = "<span style=\"color: orange;\"><span style=\"font-size: 8pt;\">";
            html.append(QString(context.function).toHtmlEscaped());
            html.append(":</span> ");
            html.append(QString(msg).toHtmlEscaped());
            html.append("</span><br/>");
            if(mainWindow && logToGUI) QMetaObject::invokeMethod(mainWindow, "slotLog", Qt::AutoConnection, Q_ARG(QString, html));
        } else if(type == QtCriticalMsg && logParams.logLevel <= LogParams::CRITICAL){
            QString html = "<span style=\"color: red;\"><span style=\"font-size: 8pt;\">";
            html.append(QString(context.function).toHtmlEscaped());
            html.append(":</span> ");
            html.append(QString(msg).toHtmlEscaped());
            html.append("</span><br/>");
            if(mainWindow && logToGUI) QMetaObject::invokeMethod(mainWindow, "slotLog", Qt::AutoConnection, Q_ARG(QString, html));
        } else if(type == QtFatalMsg && logParams.logLevel <= LogParams::FATAL){
            QString html = "<span style=\"color:white; bg-color:red;\"><span style=\"font-size: 8pt;\">";
            html.append(QString(context.function).toHtmlEscaped());
            html.append(":</span> ");
            html.append(QString(msg).toHtmlEscaped());
            html.append("</span><br/>");
            if(mainWindow && logToGUI) QMetaObject::invokeMethod(mainWindow, "slotLog", Qt::AutoConnection, Q_ARG(QString, html));
        }
    }

    logMutex.unlock();

}
