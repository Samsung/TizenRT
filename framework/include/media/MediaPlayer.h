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

#ifndef __MEDIA_MEDIAPLAYER_H
#define __MEDIA_MEDIAPLAYER_H

#include <memory>
#include <media/MediaPlayerObserverInterface.h>
#include <media/InputDataSource.h>

namespace media {
/**
 * @brief result of call the apis
 * @details @b #include <media/MediaPlayer.h>
 * @since TizenRT v2.0 PRE
 */
typedef enum player_result_e {
	/** MediaPlayer Error case */
	PLAYER_ERROR,
	/** MediaPlayer Success case */
	PLAYER_OK
} player_result_t;

class MediaPlayerImpl;

/**
 * @class 
 * @brief This class implements the MediaPlayer capability agent.
 * @details @b #include <media/MediaPlayer.h>
 * @since TizenRT v2.0 PRE
*/
class MediaPlayer
{
public:
	/**
	 * @brief Constructs an empty MediaPlayer.
	 * @details @b #include <media/MediaPlayer.h>
	 * @since TizenRT v2.0 PRE
	 */
	MediaPlayer();
	/**
	 * @brief Deconstructs an empty MediaPlayer.
	 * @details @b #include <media/MediaPlayer.h>
	 * @since TizenRT v2.0 PRE
	 */
	~MediaPlayer();
	/**
	 * @brief Start the MediaPlayerWorker.
	 * @details @b #include <media/MediaPlayer.h>
	 * This function is sync call apis
	 * @return The result of the create operation
	 * @since TizenRT v2.0 PRE
	 */
	player_result_t create();
	/**
	 * @brief Stop the MediaPlayerWorker.
	 * @details @b #include <media/MediaPlayer.h>
	 * This function is sync call apis
	 * @return The result of the destroy operation
	 * @since TizenRT v2.0 PRE
	 */
	player_result_t destroy();
	/**
	 * @brief Allocate and prepare resources related to the player.
	 * @details @b #include <media/MediaPlayer.h>
	 * This function is sync call apis
	 * @return The result of the prepare operation
	 * @since TizenRT v2.0 PRE
	 */
	player_result_t prepare();
	/**
	 * @brief Releases allocated resources related to the player.
	 * @details @b #include <media/MediaPlayer.h>
	 * This function is sync call apis
	 * @return The result of the unpreapre operation
	 * @since TizenRT v2.0 PRE
	 */
	player_result_t unprepare();
	/**
	 * @brief Start playback.
	 * @details @b #include <media/MediaPlayer.h>
	 * This function is async call apis
	 * Order to MediaPlayerWorker begin playback through the queue
	 * @return The result of the unpreapre operation
	 * @since TizenRT v2.0 PRE
	 */
	player_result_t start();
	/**
	 * @brief Pause playback.
	 * @details @b #include <media/MediaPlayer.h>
	 * This function is async call apis
	 * Order to MediaPlayerWorker pause playback through the queue
	 * @return The result of the pause operation
	 * @since TizenRT v2.0 PRE
	 */
	player_result_t pause();
	/**
	 * @brief Stop playback.
	 * @details @b #include <media/MediaPlayer.h>
	 * This function is async call apis
	 * Order to MediaPlayerWorker stop playback through the queue
	 * @return The result of the stop operation
	 * @since TizenRT v2.0 PRE
	 */
	player_result_t stop();
	/**
	 * @brief Gets the current volume
	 * @details @b #include <media/MediaPlayer.h>
	 * This function is sync call apis
	 * @return The value of current volume
	 * @since TizenRT v2.0 PRE
	 */
	int getVolume();
	/**
	 * @brief Sets the volume adjusted
	 * @details @b #include <media/MediaPlayer.h>
	 * This function is sync call apis
	 * @param[in] vol The vol that the value of mic volume
	 * @since TizenRT v2.0 PRE
	 */
	player_result_t setVolume(int);
	/**
	 * @brief Sets the DatSource of input data
	 * @details @b #include <media/MediaPlayer.h>
	 * This function is sync call apis
	 * @param[in] dataSource The dataSource that the config of input data
	 * @return The result of the setDataSource operation
	 * @since TizenRT v2.0 PRE
	 */
	player_result_t setDataSource(std::unique_ptr<stream::InputDataSource>);
	/**
	 * @brief Sets the observer of MediaPlayer
	 * @details @b #include <media/MediaPlayer.h>
	 * This function is sync call apis
	 * It sets the user's function
	 * @since TizenRT v2.0 PRE
	 */
	void setObserver(std::shared_ptr<MediaPlayerObserverInterface>);
	/**
	 * @brief Gets the current state of MediaPlayer
	 * @details @b #include <media/MediaPlayer.h>
	 * This function is sync call apis
	 * @return The value of current state
	 * @since TizenRT v2.0 PRE
	 */
	
	player_result_t seekTo(int);

private:
	std::shared_ptr<MediaPlayerImpl> mPMpImpl;
};
} // namespace media
#endif
