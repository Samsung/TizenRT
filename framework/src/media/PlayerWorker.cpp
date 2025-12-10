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
#include "audio/audio_manager.h"

#ifndef CONFIG_MEDIA_PLAYER_STACKSIZE
#define CONFIG_MEDIA_PLAYER_STACKSIZE 4096
#endif

#ifndef CONFIG_MEDIA_PLAYER_THREAD_PRIORITY
#define CONFIG_MEDIA_PLAYER_THREAD_PRIORITY 199
#endif

using namespace std;

namespace media {
PlayerWorker::PlayerWorker()
{
	mThreadName = "PlayerWorker";
	mStacksize = CONFIG_MEDIA_PLAYER_STACKSIZE;
	mPriority = CONFIG_MEDIA_PLAYER_THREAD_PRIORITY;
	mTimeout = get_output_read_timeout();
	meddbg("Output audio read timeout: %lld\n", mTimeout);
}

PlayerWorker::~PlayerWorker()
{
}

PlayerWorker& PlayerWorker::getWorker()
{
	static PlayerWorker worker;
	return worker;
}

bool PlayerWorker::processLoop()
{
	if (mPlayerList.empty()) {
		return false;
	}

	std::list<std::shared_ptr<MediaPlayerImpl>> mPlayerListCopy(mPlayerList);

	bool ret = false;
	auto t_deadline = std::chrono::steady_clock::now() + mTimeout;
	auto itr = mPlayerListCopy.begin();
	uint8_t idx = 0;

	while (itr != mPlayerListCopy.end()) {
		auto player = *itr;
		if (player && player->getState() == PLAYER_STATE_PLAYING) {
			/* ToDo: Adjust this timeout in a better way */
			player->playback(std::chrono::duration_cast<std::chrono::milliseconds>((t_deadline - std::chrono::steady_clock::now())) / (mPlayerListCopy.size() - idx), idx);
			ret = true;
		}

		if (player && player->getState() == PLAYER_STATE_COMPLETING) {
			player->playbackFinished();
			ret = true;
		}
		itr++;
		idx++;
	}

	return ret;
}

void PlayerWorker::addPlayer(std::shared_ptr<MediaPlayerImpl> player)
{
	std::list<std::shared_ptr<MediaPlayerImpl>>::iterator itr  = find(player);
	if (itr != mPlayerList.end()) {
		meddbg("Player already added in the list\n");
		return;
	}
	mPlayerList.push_back(player);
}

void PlayerWorker::removePlayer(std::shared_ptr<MediaPlayerImpl> player)
{
	mPlayerList.remove(player);
}

std::list<std::shared_ptr<MediaPlayerImpl>>::iterator PlayerWorker::find(std::shared_ptr<MediaPlayerImpl> player)
{
	auto itr = mPlayerList.begin();
	while (itr != mPlayerList.end()) {
		if (*itr == player) {
			return itr;
		}
		itr++;
	}
	return itr;
}

} // namespace media
