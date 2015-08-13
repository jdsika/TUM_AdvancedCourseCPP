#include <QApplication>
#include <QDebug>

#include "Logging/log.h"
#include "GUI/mainwindow.h"
#include "Main/robotThread.h"
#include "Main/player.h"

/**
 * @brief main will create the GUI and the RobotThread and waits until all components are finished
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{

    // See if we can connect to Player. Starts an instance if there is none
    PlayerX::startPlayer();

    // Use GUI Logger instead of stdout when using qDebug() and others
    qInstallMessageHandler(Log::customLogger);

    // Create the main thread which forks all the other threads
    QApplication qApplication(argc, argv);
    qApplication.thread()->setObjectName("Main Thread");
    MainWindow mainWindow;
    RobotThread* robotThread = new RobotThread(&mainWindow);

    // Start GUI and initialize systems with default values from it
    mainWindow.show();
    mainWindow.setup();

    // Let the Log print messages to the GUI
    Log::setMainWindowReference(&mainWindow);

    // Start the event queues. This blocks until program termination
    int guireturn = qApplication.exec();

    qDebug() << "---";
    qDebug() << "Application will end now";

    // If we get here, the program is terminating
    delete robotThread;
    PlayerX::stopPlayerIfStarted(); // Stop player if we started it
    Log::setMainWindowReference(nullptr); // Keep Log from accessing the Mainwindow during its deletion

    return guireturn;
}
