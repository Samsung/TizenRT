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

#ifndef __MEDIA_MEDIAPLAYERIMPL_H
#define __MEDIA_MEDIAPLAYERIMPL_H

#include <thread>
#include <mutex>
#include <condition_variable>

#include <media/MediaPlayer.h>
#include <media/InputDataSource.h>
#include <media/MediaPlayerObserverInterface.h>

#include "PlayerObserverWorker.h"

namespace media {
/**
 * @brief current state of MediaPlayer.
 * @details @b #include <media/MediaPlayer.h>
 * @since TizenRT v2.0 PRE
 */
typedef enum player_state_e {
	/** MediaPlayer object was created */
	PLAYER_STATE_NONE,
	/** MediaPlayer worker object was created */
	PLAYER_STATE_IDLE,
	/** MediaPlayer ready to play */
	PLAYER_STATE_READY,
	/** MediaPlayer do playing */
	PLAYER_STATE_PLAYING,
	/** MediaPlayer pause to play */
	PLAYER_STATE_PAUSED
} player_state_t;

class MediaPlayerImpl : public std::enable_shared_from_this<MediaPlayerImpl>
{
public:
	MediaPlayerImpl();
	~MediaPlayerImpl();

	player_result_t create();
	player_result_t destroy();
	player_result_t prepare();
	player_result_t unprepare();
	player_result_t start();
	player_result_t pause();
	player_result_t stop();

	int getVolume();
	player_result_t setVolume(int);

	player_result_t setDataSource(std::unique_ptr<stream::InputDataSource>);
	void setObserver(std::shared_ptr<MediaPlayerObserverInterface>);

	player_state_t getState();
	player_result_t seekTo(int);

	void notifySync();
	void notifyObserver(player_observer_command_t cmd);

	int playback(int size);

public:
	std::atomic<player_state_t> mCurState;
	unsigned char* mBuffer;
	int mBufSize;
	std::mutex mCmdMtx;
	std::condition_variable mSyncCv;
	std::shared_ptr<MediaPlayerObserverInterface> mPlayerObserver;
	std::unique_ptr<stream::InputDataSource> mInputDataSource;
	int mId;
};
} // namespace media
#endif
