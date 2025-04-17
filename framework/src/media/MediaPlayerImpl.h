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
#include "InputHandler.h"
#include "MediaQueue.h"

namespace media {
/**
 * @brief current state of MediaPlayer.
 * @details @b #include <media/MediaPlayer.h>
 * @since TizenRT v2.0
 */
typedef enum player_state_e {
	/** MediaPlayer object was created */
	PLAYER_STATE_NONE,
	/** MediaPlayer worker object was created */
	PLAYER_STATE_IDLE,
	/** MediaPlayer datasource configured */
	PLAYER_STATE_CONFIGURED,
	/** MediaPlayer is preparing datasource */
	PLAYER_STATE_PREPARING,
	/** MediaPlayer ready to play */
	PLAYER_STATE_READY,
	/** MediaPlayer do playing */
	PLAYER_STATE_PLAYING,
	/** MediaPlayer pause to play */
	PLAYER_STATE_PAUSED,
	/** MediaPlayer completed play */
	PLAYER_STATE_COMPLETED
} player_state_t;

const char *const player_state_names[] = {
	"PLAYER_STATE_NONE",
	"PLAYER_STATE_IDLE",
	"PLAYER_STATE_CONFIGURED",
	"PLAYER_STATE_PREPARING",
	"PLAYER_STATE_READY",
	"PLAYER_STATE_PLAYING",
	"PLAYER_STATE_PAUSED",
	"PLAYER_STATE_COMPLETED"
};

typedef enum player_observer_command_e {
	PLAYER_OBSERVER_COMMAND_ASYNC_PREPARED,
	PLAYER_OBSERVER_COMMAND_FINISHED,
	PLAYER_OBSERVER_COMMAND_PLAYBACK_ERROR,
	PLAYER_OBSERVER_COMMAND_BUFFER_OVERRUN,
	PLAYER_OBSERVER_COMMAND_BUFFER_UNDERRUN,
	PLAYER_OBSERVER_COMMAND_BUFFER_UPDATED,
	PLAYER_OBSERVER_COMMAND_BUFFER_STATECHANGED,
	PLAYER_OBSERVER_COMMAND_BUFFER_DATAREACHED,
} player_observer_command_t;

typedef enum player_event_e {
	PLAYER_EVENT_SOURCE_OPEN_FAILED,
	PLAYER_EVENT_SOURCE_PREPARED,
} player_event_t;

class MediaPlayerImpl : public std::enable_shared_from_this<MediaPlayerImpl>
{
public:
	MediaPlayerImpl(MediaPlayer &player);
	~MediaPlayerImpl();

	player_result_t create();
	player_result_t destroy();
	player_result_t prepare();
	player_result_t prepareAsync();
	player_result_t unprepare();
	player_result_t reset();
	player_result_t start();
	player_result_t pause();
	player_result_t stop();

	player_result_t getVolume(uint8_t *vol);
	player_result_t getMaxVolume(uint8_t *vol);
	player_result_t setVolume(uint8_t vol);

	player_result_t setDataSource(std::unique_ptr<stream::InputDataSource>);
	player_result_t setObserver(std::shared_ptr<MediaPlayerObserverInterface>);
	player_result_t setStreamInfo(std::shared_ptr<stream_info_t> stream_info);

	player_state_t getState();
	bool isPlaying();

	void notifySync();
	void notifyObserver(player_observer_command_t cmd, ...);
	void notifyAsync(player_event_t event);
	void playback();
	player_result_t setLooping(bool loop);

private:
	void createPlayer(player_result_t &ret);
	void destroyPlayer(player_result_t &ret);
	void preparePlayer(player_result_t &ret);
	void prepareAsyncPlayer();
	void unpreparePlayer(player_result_t &ret);
	player_result_t unpreparePlayback(void);
	void startPlayer(player_result_t &ret);
	void stopPlayer(player_result_t &ret);
	player_result_t stopPlayback(bool drain);
	void stopPlaybackInternal(bool drain);
	void pausePlayer(player_result_t &ret);
	void getPlayerVolume(uint8_t *vol, player_result_t &ret);
	void getPlayerMaxVolume(uint8_t *vol, player_result_t &ret);
	void setPlayerVolume(uint8_t vol, player_result_t &ret);
	void setPlayerObserver(std::shared_ptr<MediaPlayerObserverInterface> observer, player_result_t &ret);
	void unsetPlayerObserver();
	void setPlayerDataSource(std::shared_ptr<stream::InputDataSource> dataSource, player_result_t &ret);
	void setPlayerStreamInfo(std::shared_ptr<stream_info_t> stream_info, player_result_t &ret);
	stream_focus_state_t getStreamFocusState(void);
	void setPlayerLooping(bool loop, player_result_t &ret);
	player_result_t playbackFinished(void);
	void dequeueAndRunObserverCallback();

private:
	MediaPlayer &mPlayer;
	std::atomic<player_state_t> mCurState;
	unsigned char *mBuffer;
	int mBufSize;
	std::mutex mCmdMtx;
	std::condition_variable mSyncCv;
	std::shared_ptr<stream_info_t> mStreamInfo;
	std::shared_ptr<MediaPlayerObserverInterface> mPlayerObserver;
	stream::InputHandler mInputHandler;
	MediaQueue mObserverQueue;
};
} // namespace media
#endif
