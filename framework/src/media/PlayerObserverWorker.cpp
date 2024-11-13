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
#include <functional>
#include <debug.h>
#include "PlayerObserverWorker.h"

#ifndef CONFIG_MEDIA_PLAYER_OBSERVER_STACKSIZE
#define CONFIG_MEDIA_PLAYER_OBSERVER_STACKSIZE 2048
#endif
#ifndef CONFIG_MEDIA_PLAYER_OBSERVER_THREAD_PRIORITY
#define CONFIG_MEDIA_PLAYER_OBSERVER_THREAD_PRIORITY 199
#endif

namespace media {
PlayerObserverWorker::PlayerObserverWorker()
{
	mThreadName = "PlayerObserverWorker";
	mStacksize = CONFIG_MEDIA_PLAYER_OBSERVER_STACKSIZE;
	mPriority = CONFIG_MEDIA_PLAYER_OBSERVER_THREAD_PRIORITY;
}

PlayerObserverWorker::~PlayerObserverWorker()
{
}

PlayerObserverWorker &PlayerObserverWorker::getWorker()
{
	static PlayerObserverWorker worker;
	return worker;
}
} // namespace media
