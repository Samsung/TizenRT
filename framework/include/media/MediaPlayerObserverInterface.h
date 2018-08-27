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
 * @ingroup MEDIA
 * @{
 */

/**
 * @file media/MediaPlayerObserverInterface.h
 * @brief Media MediaPlayerObserverInterface APIs
 */

#ifndef __MEDIA_MEDIAPLAYEROBSERVERINTERFACE_H
#define __MEDIA_MEDIAPLAYEROBSERVERINTERFACE_H

namespace media {
class MediaPlayer;
enum player_error_e : int;
typedef enum player_error_e player_error_t;

/**
 * @class
 * @brief This class provides an interface to the user.
 * @details @b #include <media/MediaPlayerObserverInterface.h>
 * This class informs the user of the playback state of MediaPlayer
 * This class informs the user of the error state of MediaPlayer
 * @since TizenRT v2.0 PRE
 */
class MediaPlayerObserverInterface
{
public:
	/**
	 * @brief informs the user of the playback has begun.
	 * @details @b #include <media/MediaPlayerObserverInterface.h>
	 * @since TizenRT v2.0 PRE
	 */
	virtual void onPlaybackStarted(MediaPlayer &mediaPlayer) = 0;
	/**
	 * @brief informs the user of the playback has finished.
	 * @details @b #include <media/MediaPlayerObserverInterface.h>
	 * @since TizenRT v2.0 PRE
	 */
	virtual void onPlaybackFinished(MediaPlayer &mediaPlayer) = 0;
	/**
	 * @brief informs the user of the error state of player operation
	 * @details @b #include <media/MediaPlayerObserverInterface.h>
	 * @since TizenRT v2.0 PRE
	 */
	virtual void onPlaybackError(MediaPlayer &mediaPlayer, player_error_t error) = 0;
	/**
	 * @brief informs the user of the error state of player operation
	 * @details @b #include <media/MediaPlayerObserverInterface.h>
	 * @since TizenRT v2.0 PRE
	 */
	virtual void onStartError(MediaPlayer &mediaPlayer, player_error_t error) = 0;
	/**
	 * @brief informs the user of the error state of player operation
	 * @details @b #include <media/MediaPlayerObserverInterface.h>
	 * @since TizenRT v2.0 PRE
	 */
	virtual void onStopError(MediaPlayer &mediaPlayer, player_error_t error) = 0;
	/**
	 * @brief informs the user of the error state of player operation
	 * @details @b #include <media/MediaPlayerObserverInterface.h>
	 * @since TizenRT v2.0 PRE
	 */
	virtual void onPauseError(MediaPlayer &mediaPlayer, player_error_t error) = 0;
	/**
	 * @brief informs the user of the playback has paused.
	 * @details @b #include <media/MediaPlayerObserverInterface.h>
	 * @since TizenRT v2.0 PRE
	 */
	virtual void onPlaybackPaused(MediaPlayer &mediaPlayer) = 0;
	/**
	 * @brief informs the user the playback buffer state: overrun.
	 * @details @b #include <media/MediaPlayerObserverInterface.h>
	 * @since TizenRT v2.0 PRE
	 */
	virtual void onPlaybackBufferOverrun(MediaPlayer &mediaPlayer) {}
	/**
	 * @brief informs the user the playback buffer state: underrun.
	 * @details @b #include <media/MediaPlayerObserverInterface.h>
	 * @since TizenRT v2.0 PRE
	 */
	virtual void onPlaybackBufferUnderrun(MediaPlayer &mediaPlayer) {}
	/**
	 * @brief informs the user the playback buffer updated.
	 * @details @b #include <media/MediaPlayerObserverInterface.h>
	 * @since TizenRT v2.0 PRE
	 */
	virtual void onPlaybackBufferUpdated(MediaPlayer &mediaPlayer, size_t bytes) {}
	/**
	 * @brief informs the user the playback buffer state changed
	 * @details @b #include <media/MediaPlayerObserverInterface.h>
	 * @since TizenRT v2.0 PRE
	 */
	virtual void onPlaybackBufferStateChanged(MediaPlayer &mediaPlayer, buffer_state_t state) {}
};
} // namespace media

#endif
/** @} */ // end of MEDIA group
