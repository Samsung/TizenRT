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
#include <cstdio>
#include <string>
#include <map>
#include <sstream>
#include <unistd.h>
#include <mutex>
#include "PlayerController.h"
#include "media/MediaPlayer.h"
#include "audio/SoundManager.h"

using namespace media;

PlayerController::PlayerController() : mNextPlayerId(1)
{
}

PlayerController::~PlayerController()
{
}

PlayerController& PlayerController::getInstance() {
    static PlayerController instance;
    return instance;
}

int PlayerController::registerPlayer(MediaPlayer* player, stream_policy_t stream_policy) {
    std::lock_guard<std::mutex> lock(mMutex);
    int playerId = mNextPlayerId++;
    mPlayers[playerId].player = player;
    mPlayers[playerId].stream_policy = stream_policy;
    return playerId;
}

void PlayerController::handleCommand(int playerId, const std::string& action, int volume) {
    if (mPlayers.find(playerId) == mPlayers.end()) {
        printf("Player %d not found\n", playerId);
        return;
    }

    MediaPlayer* player = mPlayers[playerId].player;
    stream_policy_t stream_policy = mPlayers[playerId].stream_policy;

    if (action == "pause") {
        if (player->isPlaying()) {
            player->pause();
        } else {
            printf("Player not running, skipping pause request\n");
        }
    } else if (action == "stop") {
        if (player->isPlaying()) {
            player->stop();
        } else {
            printf("Player not running, skipping stop request\n");
        }
    } else if (action == "start") {
        if (!player->isPlaying()) {
            player->start();
        } else {
            printf("Player already running, skipping start request\n");
        }
    } else if (action == "volume") {
        player->setVolume(volume);
    } else if (action == "mute") {
        setStreamMute(stream_policy, true);
    } else if (action == "unmute") {
        setStreamMute(stream_policy, false);
    } else {
        printf("Unknown command: %s\n", action.c_str());
    }
}

extern "C" {
int playercontroller_main(int argc, char* argv[]) {
    auto& controller = PlayerController::getInstance();

    if (argc != 3 && argc != 4) {
        printf("Usage: player <player_num> <command> [volume]\n");
        printf("Available commands:\n");
        printf("  start              - Start playback\n");
        printf("  pause              - Pause playback\n");
        printf("  stop               - Stop playback\n");
        printf("  mute               - Mute the stream\n");
        printf("  unmute             - Unmute the stream\n");
        printf("  volume <value>     - Set volume (0 - 15)\n");
        return -1;
    }

    int playerId = atoi(argv[1]);
    std::string command = argv[2];
    int volume = 0;

    if (command == "volume" && argc != 4) {
        printf("Volume value required for volume command\n");
        return -1;
    }

    if (command == "volume") {
        volume = atoi(argv[3]);
    }

    controller.handleCommand(playerId, command, volume);
    return 0;
}
}
