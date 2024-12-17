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

/**
 * @defgroup MEDIA MEDIA
 * @brief Provides APIs for Media Framework
 * @{
 */

/**
 * @file media/MediaPlayer.h
 * @brief Media MediaPlayer APIs
 */

#ifndef __MEDIA_MEDIAPLAYER_H
#define __MEDIA_MEDIAPLAYER_H

#include <memory>
#include <media/stream_info.h>
#include <media/InputDataSource.h>
#include <media/MediaPlayerObserverInterface.h>

namespace media {
/**
 * @brief result of call the apis
 * @details @b #include <media/MediaPlayer.h>
 * @since TizenRT v2.0
 */
enum player_error_e : int {
	/** MediaPlayer Error case */
	PLAYER_ERROR_NOT_ALIVE = -10,
	PLAYER_ERROR_FOCUS_NOT_READY,
	PLAYER_ERROR_PLAYBACK_FINISHED,
	PLAYER_ERROR_INVALID_STATE,
	PLAYER_ERROR_INVALID_OPERATION,
	PLAYER_ERROR_INVALID_PARAMETER,
	PLAYER_ERROR_INTERNAL_OPERATION_FAILED,
	PLAYER_ERROR_FILE_OPEN_FAILED,
	PLAYER_ERROR_OUT_OF_MEMORY,
	PLAYER_ERROR_DEVICE_NOT_SUPPORTED,
	/** MediaPlayer Success case */
	PLAYER_ERROR_NONE = 0
};

typedef enum player_error_e player_error_t;
const int PLAYER_OK = PLAYER_ERROR_NONE;
typedef int player_result_t;

class MediaPlayerImpl;

/**
 * @class
 * @brief This class implements the MediaPlayer capability agent.
 * @details @b #include <media/MediaPlayer.h>
 * @since TizenRT v2.0
*/
class MediaPlayer
{
public:
	/**
	 * @brief Construct an empty MediaPlayer.
	 * @details @b #include <media/MediaPlayer.h>
	 * @since TizenRT v2.0
	 */
	MediaPlayer();

	/**
	 * @brief Deconstruct an empty MediaPlayer.
	 * @details @b #include <media/MediaPlayer.h>
	 * @since TizenRT v2.0
	 */
	~MediaPlayer();

	/**
	 * @brief Create the MediaPlayer for playback
	 * @details @b #include <media/MediaPlayer.h>
	 * This function is a synchronous API
	 * @return The result of the create operation
	 * @since TizenRT v2.0
	 */
	player_result_t create();

	/**
	 * @brief Destroy MediaPlayer
	 * @details @b #include <media/MediaPlayer.h>
	 * This function is a synchronous API
	 * @return The result of the destroy operation
	 * @since TizenRT v2.0
	 */
	player_result_t destroy();

	/**
	 * @brief Allocate and prepare resources related to the player, it should be called before start
	 * @details @b #include <media/MediaPlayer.h>
	 * This function is a synchronous API
	 * If it takes a lot of time until it's ready, please use `prepareAsync` instead
	 * @return The result of the prepare operation
	 * @since TizenRT v2.0
	 */
	player_result_t prepare();

	/**
	 * @brief Prepare resources related to the player asynchronously,
	 * start() should be called when onAsyncPrepared callback invoked.
	 * @details @b #include <media/MediaPlayer.h>
	 * This function is an asynchronous API
	 * Order to MediaPlayerWorker prepare resources through the queue
	 * @return The result of the prepare operation
	 * @since TizenRT v2.0
	 */
	player_result_t prepareAsync();

	/**
	 * @brief Releases allocated resources related to the player.
	 * @details @b #include <media/MediaPlayer.h>
	 * This function is a synchronous API
	 * @return The result of the unpreapre operation
	 * @since TizenRT v2.0
	 */
	player_result_t unprepare();

	/**
	 * @brief Reset allocated resources related to the player, after the API, player is in state PLAYER_STATE_IDLE.
	 * @details @b #include <media/MediaPlayer.h>
	 * This function is a synchronous API
	 * @return The result of the reset operation
	 * @since TizenLite v5.0
	 */
	player_result_t reset();

	/**
	 * @brief Start playback.
	 * @details @b #include <media/MediaPlayer.h>
	 * This function is a asynchronous API
	 * Order to MediaPlayerWorker begin playback through the queue
	 * @return The result of the unprepare operation
	 * @since TizenRT v2.0
	 */
	player_result_t start();

	/**
	 * @brief Pause playback.
	 * @details @b #include <media/MediaPlayer.h>
	 * This function is a asynchronous API
	 * Order to MediaPlayerWorker pause playback through the queue
	 * @return The result of the pause operation
	 * @since TizenRT v2.0
	 */
	player_result_t pause();

	/**
	 * @brief Stop playback.
	 * @details @b #include <media/MediaPlayer.h>
	 * This function is a asynchronous API
	 * Order to MediaPlayerWorker stop playback through the queue
	 * @return The result of the stop operation
	 * @since TizenRT v2.0
	 */
	player_result_t stop();

	/**
	 * @brief Get the current volume
	 * @details @b #include <media/MediaPlayer.h>
	 * This function is a synchronous API
	 * @return The value of current volume
	 * @since TizenRT v2.0
	 */
	player_result_t getVolume(uint8_t *volume);

	/**
	 * @brief Get the maximum output volume
	 * @details @b #include <media/MediaPlayer.h>
	 * This function is a synchronous API
	 * @return The value of the maximum volume
	 * @since TizenRT v2.0
	 */
	player_result_t getMaxVolume(uint8_t *volume);

	/**
	 * @brief Set the volume adjusted
	 * @details @b #include <media/MediaPlayer.h>
	 * This function is a synchronous API
	 * @param[in] vol The volume value to set
	 * @since TizenRT v2.0
	 */
	player_result_t setVolume(uint8_t);

	/**
	 * @brief Set the DataSource of input data
	 * @details @b #include <media/MediaPlayer.h>
	 * This function is a synchronous API
	 * @param[in] dataSource The dataSource that the config of input data
	 * @return The result of the setDataSource operation
	 * @since TizenRT v2.0
	 */
	player_result_t setDataSource(std::unique_ptr<stream::InputDataSource>);

	/**
	 * @brief Set the observer of MediaPlayer
	 * @details @b #include <media/MediaPlayer.h>
	 * This function is a synchronous API
	 * It sets the user's function
	 * @param[in] observer The callback to be set for Media Player Observer.
	 * @return The result of the setObserver operation
	 * @since TizenRT v2.0
	 */
	player_result_t setObserver(std::shared_ptr<MediaPlayerObserverInterface>);

	/**
	 * @brief MediaPlayer operator==
	 * @details @b #include <media/MediaPlayer.h>
	 * This function is a synchronous API
	 * Compares the MediaPlayer objects for equality
	 * @return The result of the compare operation for MediaPlayer object
	 * @since TizenRT v2.0
	 */
	bool operator==(const MediaPlayer &rhs);

	/**
	 * @brief Get whether the player is playing
	 * @details @b #include <media/MediaPlayer.h>
	 * This function is a synchronous API
	 * @return true if player is playing
	 * @since TizenRT v2.1 PRE
	 */
	bool isPlaying();

	/**
	 * @brief set stream_info of MediaPlayer
	 * @details @b #include <media/MediaPlayer.h>
	 * This function is a synchronous API
	 * param[in] stream_info shared_ptr of stream_info_t
	 * @return The result of the setStreamInfo operation
	 */
	player_result_t setStreamInfo(std::shared_ptr<stream_info_t> stream_info);

	/**
	 * @brief Set Playback to be looping or non-looping
	 * @details @b #include <media/MediaPlayer.h>
	 * This function is a synchronous API
	 * @return true if looping is set properly
	 * @since TizenRT v5.0
	 */
	player_result_t setLooping(bool loop);

private:
	std::shared_ptr<MediaPlayerImpl> mPMpImpl;
	uint64_t mId;
};
} // namespace media
#endif
/** @} */ // end of MEDIA group
