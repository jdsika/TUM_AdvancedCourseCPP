#ifndef PLAYERX_H // calling this PLAYER_H will get you a shitload of errors from libplayerc++
#define PLAYERX_H

#include <string>

// Forward declare
namespace PlayerCc{
class PlayerClient;
}

/**
 * @brief The Player class this class contains the instance of the player client to access in 'global' scope
 */
class PlayerX
{
public:
    /**
     * @brief startPlayer
     */
    static void startPlayer();
    /**
     * @brief stopPlayerIfStarted
     */
    static void stopPlayerIfStarted();

    /**
     * @brief getInstance
     * @return the static instance of a playerclient object on heap
     */
    static PlayerCc::PlayerClient* getInstance();
private:
    static bool didWeStartPlayerOurselves;
    static std::string getSelfpath();
};

#endif // PLAYERX_H
