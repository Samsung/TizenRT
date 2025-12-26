/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <map>
#include <mutex>
#include <string>
#include "media/MediaPlayer.h"

using namespace media;

/**
 * @struct player_info_s
 * @brief Structure to hold MediaPlayer instance and its associated stream policy.
 */
typedef struct player_info_s {
    MediaPlayer *player;            /**< Pointer to the MediaPlayer instance. */
    stream_policy_t stream_policy;  /**< The stream policy associated with this player. */
} player_info_t;

/**
 * @class PlayerController
 * @brief A singleton class for controlling multiple MediaPlayer instances.
 * Each player is uniquely identified and can be controlled independently through a set of defined commands.
 */
class PlayerController {
private:
    std::map<int, player_info_t> mPlayers;
    int mNextPlayerId;
    std::mutex mMutex;

    /**
     * @brief Private constructor for the singleton PlayerController class.
     */
    PlayerController();

    /**
     * @brief Private destructor for the singleton PlayerController class.
     */
    ~PlayerController();

public:
    /**
     * @brief Returns the singleton instance of the PlayerController class.
     * @return A reference to the singleton PlayerController instance.
     */
    static PlayerController& getInstance();

    /**
     * @brief Registers a MediaPlayer instance with the controller.
     * @param player A pointer to the MediaPlayer instance to be registered.
     * @param stream_policy Stream policy associated with this player.
     * @return int unique player ID assigned to the registered player. This ID is used to issue commands to this player.
     */
    int registerPlayer(MediaPlayer* player, stream_policy_t stream_policy);

    /**
     * @brief Handles and executes a given command for a specified player.
     * @param playerId Unique ID of the player to controll.
     * @param action A string representing the command to be executed e.g. 'start', 'stop', 'pause', 'volume', 'mute', & 'unmute'.
     * @param volume An integer specifying the volume level. This parameter is only used when the `action` is 'volume'. It defaults to 0.
     */
    void handleCommand(int playerId, const std::string& action, int volume = 0);
};

#endif // PLAYERCONTROLLER_H
