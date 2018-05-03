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

#include <thread>
#include <memory>
#include <mutex>
#include <media/MediaPlayer.h>
#include "MediaQueue.h"

namespace media {
class PlayerWorker
{
public:
	PlayerWorker();
	~PlayerWorker();
	static PlayerWorker &getWorker();

	void startPlayer(std::shared_ptr<MediaPlayerImpl>);
	void stopPlayer(std::shared_ptr<MediaPlayerImpl>);
	void pausePlayer(std::shared_ptr<MediaPlayerImpl>);
	player_result_t startWorker();
	void stopWorker();
	MediaQueue &getQueue();

private:
	int entry();
	void increaseRef();
	void decreaseRef();

private:
	int mRefCnt;
	std::atomic<bool> mIsRunning;
	std::thread mWorkerThread;
	std::shared_ptr<MediaPlayerImpl> mCurPlayer;
	MediaQueue mWorkerQueue; // worker queue
	std::mutex mRefMtx;		 // reference cnt mutex
};
} // namespace media
#endif
