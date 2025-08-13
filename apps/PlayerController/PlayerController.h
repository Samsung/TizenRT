/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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

typedef struct player_info_s {
    MediaPlayer *player;
    stream_policy_t stream_policy;
} player_info_t;

class PlayerController {
private:
    std::map<int, player_info_t> mPlayers;
    int mNextPlayerId;
    std::mutex mMutex;

    PlayerController();
    ~PlayerController();

public:
    static PlayerController& getInstance();

    int registerPlayer(MediaPlayer* player, stream_policy_t stream_policy);
    void handleCommand(int playerId, const std::string& action, int volume = 0);
};

#endif // PLAYERCONTROLLER_H
