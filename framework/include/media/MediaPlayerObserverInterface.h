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
	 * @brief Id means unique id of MediaPlayer.
	 * @details @b #include <media/MediaPlayerObserverInterface.h>
	 * support multi-recorders according to device specifications
	 * @since TizenRT v2.0 PRE
	 */
	using Id = uint64_t;
	/**
	 * @brief informs the user of the playback has begun.
	 * @details @b #include <media/MediaPlayerObserverInterface.h>
	 * @since TizenRT v2.0 PRE
	 */
	virtual void onPlaybackStarted(Id id) = 0;
	/**
	 * @brief informs the user of the playback has finished.
	 * @details @b #include <media/MediaPlayerObserverInterface.h>
	 * @since TizenRT v2.0 PRE
	 */
	virtual void onPlaybackFinished(Id id) = 0;
	/**
	 * @brief informs the user of the error state of player operation
	 * @details @b #include <media/MediaPlayerObserverInterface.h>
	 * @since TizenRT v2.0 PRE
	 */
	virtual void onPlaybackError(Id id) = 0;
	/**
	 * @brief informs the user of the playback has paused.
	 * @details @b #include <media/MediaPlayerObserverInterface.h>
	 * @since TizenRT v2.0 PRE
	 */
	virtual void onPlaybackPaused(Id id) = 0;
};
} // namespace media

#endif
/** @} */ // end of MEDIA group
