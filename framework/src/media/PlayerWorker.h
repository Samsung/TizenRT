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

#ifndef __MEDIA_PLAYERWORKER_HPP
#define __MEDIA_PLAYERWORKER_HPP

#include <memory>
#include <list>
#include <media/MediaPlayer.h>
#include "MediaWorker.h"

namespace media {
class PlayerWorker : public MediaWorker
{
public:
	static PlayerWorker &getWorker();

	void addPlayer(std::shared_ptr<MediaPlayerImpl> player);
	void removePlayer(std::shared_ptr<MediaPlayerImpl> player);

private:
	PlayerWorker();
	virtual ~PlayerWorker();
	bool processLoop() override;
	std::list<std::shared_ptr<MediaPlayerImpl>>::iterator find(std::shared_ptr<MediaPlayerImpl> player);

private:
	std::list<std::shared_ptr<MediaPlayerImpl>> mPlayerList;
	std::chrono::milliseconds mTimeout;
};
} // namespace media
#endif
