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
PlayerWorker::PlayerWorker() : mCurPlayer(nullptr)
{
	mThreadName = "PlayerWorker";
	mStacksize = CONFIG_MEDIA_PLAYER_STACKSIZE;
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
	if (mCurPlayer && (mCurPlayer->getState() == PLAYER_STATE_PLAYING)) {
		mCurPlayer->playback();
		return true;
	}

	return false;
}

void PlayerWorker::setPlayer(std::shared_ptr<MediaPlayerImpl> player)
{
	mCurPlayer = player;
}

std::shared_ptr<MediaPlayerImpl> PlayerWorker::getPlayer()
{
	return mCurPlayer;
}

} // namespace media
