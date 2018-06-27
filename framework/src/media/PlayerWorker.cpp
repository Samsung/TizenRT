/* ****************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#include <tinyara/config.h>
#include <debug.h>

#include "PlayerWorker.h"
#include "MediaPlayerImpl.h"

#ifndef CONFIG_MEDIA_PLAYER_STACKSIZE
#define CONFIG_MEDIA_PLAYER_STACKSIZE 4096
#endif

using namespace std;

namespace media {
PlayerWorker::PlayerWorker()
{
	mThreadName = "PlayerWorker";
	mStacksize = CONFIG_MEDIA_PLAYER_STACKSIZE;
}

PlayerWorker::~PlayerWorker()
{
}

PlayerWorker &PlayerWorker::getWorker()
{
	static PlayerWorker worker;
	return worker;
}

bool PlayerWorker::processLoop()
{
	if (!mFocusList.empty()) {
		auto focusedPlayer = mFocusList.front();
		if (focusedPlayer->getState() == PLAYER_STATE_PLAYING) {
			focusedPlayer->playback();
			return true;
		}
	}

	return false;
}

bool PlayerWorker::requestFocus(std::shared_ptr<MediaPlayerImpl> player)
{
	if (!mFocusList.empty()) {
		auto focusedPlayer = mFocusList.front();
		if (!focusedPlayer->resetAudioStream()) {
			meddbg("fail to reset audio stream\n");
			return false;
		}
	}

	if (player->setAudioStream()) {
		mFocusList.push_front(player);
		medvdbg("mFocusList.size() = %d\n", mFocusList.size());
		return true;
	}
	return false;
}

bool PlayerWorker::abandonFocus(std::shared_ptr<MediaPlayerImpl> player)
{
	/* If the abandoned player is focused, reset pre-focused player and set newly focused player */
	auto playerIterator = mFocusList.begin();
	if (*playerIterator == player) {
		if (player->resetAudioStream()) {
			playerIterator = mFocusList.erase(playerIterator);
			medvdbg("mFocusList.size() = %d\n", mFocusList.size());
			if (!mFocusList.empty()) {
				return (*playerIterator)->setAudioStream();
			}
			return true;
		} else {
			return false;
		}
	} else {
		playerIterator++;
	}

	/* Else, just find player and erase it */
	while (playerIterator != mFocusList.end()) {
		if (*playerIterator == player) {
			playerIterator = mFocusList.erase(playerIterator);
			medvdbg("mFocusList.size() = %d\n", mFocusList.size());
			return true;
		} else {
			playerIterator++;
		}
	}

	meddbg("Player is not removed from mFocusList list\n");
	return false;
}
} // namespace media
