#include "player.h"

#include <string>
#include <unistd.h>

#include <QMutex>
#include <QMutexLocker>
#include <QGlobalStatic>
#include <QDebug>
#include <QElapsedTimer>
#include <QThread>

#include <libplayerc++/playerc++.h>

// Construct a global static object
Q_GLOBAL_STATIC_WITH_ARGS(
        PlayerCc::PlayerClient, // Type
        playerInstance, // Variable name
        (PlayerCc::PLAYER_HOSTNAME,PlayerCc::PLAYER_PORTNUM) // Arguments (defaults -> localhost and 6665)
        )


bool PlayerX::didWeStartPlayerOurselves = false;

std::string PlayerX::getSelfpath() {
    char buff[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", buff, sizeof(buff)-1);
    if (len != -1) {
      buff[len] = '\0';
      return std::string(buff);
    } else {
     /* handle error condition */
    }
    return "";
}

void PlayerX::startPlayer()
{
    bool isPlayerRunning = true;
    try {
        // This will try to get a connection with Player
        PlayerX::getInstance()->SetReplaceRule(true);
    } catch (PlayerCc::PlayerError ex) {
        // The connection has failed. Probably no Player process
        isPlayerRunning = false;
    }

    if(!isPlayerRunning) {
        qDebug() << "Could not establish a connection to Player.";

        // Construct the path of the startplayersim.sh script from the path of the program binary
        QString selfPath = QString::fromStdString(getSelfpath());
        QStringList pathList = selfPath.split(QChar('/'));
        pathList.removeLast();
        selfPath = pathList.join('/');
        selfPath.prepend("cd ");
        selfPath.append("/.. && ./startplayersim.sh >/dev/null 2>&1");
        qDebug() << "Starting my own Player now with" << selfPath;

        // CD to the script dir and run it. discard all output.
        int ret = system(selfPath.toStdString().c_str());
        if(ret) {
            qWarning() << "System call returned" << ret << "- Exit now.";
            exit(ret);
        } else {
            didWeStartPlayerOurselves = true;

            // Now try to connect to Player. This will not work instantly, so try several times until timeout (5s).
            QElapsedTimer timer;
            timer.start();
            bool playerWasStarted = false;
            while(timer.elapsed() < 5000 && !playerWasStarted){
                try {
                    // Try to get a connection, again
                    PlayerX::getInstance()->SetReplaceRule(true);
                    playerWasStarted = true; // if we get here, the above command worked
                } catch (PlayerCc::PlayerError ex) {
                    // We got an exception during connect
                    qDebug() << "trying again in 200ms";
                    playerWasStarted = false;
                    QThread::msleep(200);
                }
            }
            if(!playerWasStarted) {
                qWarning() << "Could not get a player connection, again. Exit now." << ret;
                exit(-1);
            } else {
                qDebug() << "Connection to player is OK";
            }
        }
    }

}

void PlayerX::stopPlayerIfStarted()
{
    if(didWeStartPlayerOurselves) {
        qDebug() << "Trying to stop all player processes, because we started one on our own.";
        PlayerX::getInstance()->Stop(); // This might not be enough. Couldn't find a way to destruct the playerclient object...
        QElapsedTimer timer;
        timer.start();
        while(PlayerX::getInstance()->Connected() && timer.elapsed() < 1000){
            QThread::msleep(200);
        }
        int ret = system("killall player");
        if(ret) {
            qWarning() << "System call returned" << ret;
        }
    }
}

PlayerCc::PlayerClient* PlayerX::getInstance()
{
    static QMutex mutex;
    static QMutexLocker locker(&mutex);
    return playerInstance();
}

