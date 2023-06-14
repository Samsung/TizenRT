/*
 * Copyright (c) 2021 Realtek, LLC.
 * All rights reserved.
 *
 * Licensed under the Realtek License, Version 1.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License from Realtek
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @addtogroup Media
 * @{
 *
 * @brief Declares APIs for media framework.
 *
 *
 * @since 1.0
 * @version 1.0
 */

/**
 * @file rtfiplayer.h
 *
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef AMEBA_FWK_MEDIA_PLAYBACK_MEDIA_RTFIPLAYER_H
#define AMEBA_FWK_MEDIA_PLAYBACK_MEDIA_RTFIPLAYER_H

#include "osal_c/osal_types.h"
#include "cutils/parcel.h"

#include "rtplayer_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RTFiPlayer RTFiPlayer;
typedef struct RTFiPlayerCallback RTFiPlayerCallback;

/**
 * @brief Provides calback interfaces.
 */
struct RTFiPlayerCallback {
	/**
	 * @brief Called when fiplayer status is changed.
	 *
	 * @param listener The RTFiPlayerCallback object pointer.
	 * @param player The RTFiPlayer object pointer.
	 * @param state The player status, one of {@link RTPlayerStates}.
	 * @since 1.0
	 * @version 1.0
	 */
	void (*OnRTFiPlayerStateChanged)(const struct RTFiPlayerCallback *listener, const struct RTFiPlayer *player, int state);

	/**
	 * @brief Called when fiplayer information is received.
	 *
	 * @param listener The RTFiPlayerCallback object pointer.
	 * @param player The RTFiPlayer object pointer.
	 * @param info Indicates the information type. For details, see {@link RTPlayerInfos}.
	 * @param extra Indicates the information code.
	 * @since 1.0
	 * @version 1.0
	 */
	void (*OnRTFiPlayerInfo)(const struct RTFiPlayerCallback *listener, const struct RTFiPlayer *player, int info, int extra);

	/**
	 * @brief Called when a fiplayer error occurs.
	 *
	 * @param listener The RTFiPlayerCallback object pointer.
	 * @param player The RTFiPlayer object pointer.
	 * @param error Indicates the error type. For details, see {@link RTPlayerErrors}.
	 * @param extra Indicates the error code.
	 * @since 1.0
	 * @version 1.0
	 */
	void (*OnRTFiPlayerError)(const struct RTFiPlayerCallback *listener, const struct RTFiPlayer *player, int error, int extra);
};

/**
 * @brief Creates RTFiPlayer.
 *
 * @return a new RTFiPlayer object pointer.
 * @since 1.0
 * @version 1.0
 */
struct RTFiPlayer *RTFiPlayer_Create(void);

/**
 * @brief Destory RTFiPlayer.
 *
 * @param player The RTFiPlayer object pointer.
 * @since 1.0
 * @version 1.0
 */
void	RTFiPlayer_Destory(struct RTFiPlayer *player);


rt_status_t RTFiPlayer_SetStreamSource(struct RTFiPlayer *player, struct RTStreamSource *source);

/**
 * @brief Prepares the fiplayer for playback, synchronously.
 *
 * @param player The RTFiPlayer object pointer.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | playback is not in idle state.
 * OSAL_ERR_UNKNOWN_ERROR | operation is failed.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTFiPlayer_Prepare(struct RTFiPlayer *player);

/**
 * @brief Prepares the fiplayer for playback, asynchronously.
 * A call to {@link RTFiPlayer_PrepareAsync()} (asynchronous) will first transfers
 * the playbackt to the RTPLAYER_PREPARING state after the call returns
 * (which occurs almost right away) while the internal player engine continues
 * working on the rest of preparation work until the preparation work completes.
 * When the preparation completes, the internal player engine then calls a user
 * supplied callback method OnRTPlayerStateChanged(..., RTPLAYER_PREPARED)
 * of the RTPlayerCallbacks interface, if an RTPlayerCallbacks object is registered
 * beforehand via {@link RTFiPlayer_SetCallbacks()}.
 *
 * @param player The RTPlayer object pointer.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | playback is not in idle state.
 * OSAL_ERR_UNKNOWN_ERROR | operation is failed.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTFiPlayer_PrepareAsync(struct RTFiPlayer *player);

/**
* @brief Starts or resumes playback. If playback had previously been paused,
* playback will continue from where it was paused. If playback had
* been stopped, or never started before, playback will start at the
* beginning.
*
* @param player The RTFiPlayer object pointer.
* @return Returns a value listed below: \n
* rt_status_t | Description
* ----------------------| -----------------------
* OSAL_OK | the operation is successful.
* OSAL_ERR_INVALID_OPERATION | playback is not in idle state.
* OSAL_ERR_UNKNOWN_ERROR | operation is failed.
* @since 1.0
* @version 1.0
*/
rt_status_t RTFiPlayer_Start(struct RTFiPlayer *player);


//rt_status_t RTPlayer_SetLooping(struct RTPlayer *player, int8_t loop);


rt_status_t RTFiPlayer_Stop(struct RTFiPlayer *player);

/**
 * @brief Pauses playback. Call RTFiPlayer_Start() to resume.
 *
 * @param player The RTFiPlayer object pointer.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | playback is not in idle state.
 * OSAL_ERR_UNKNOWN_ERROR | operation is failed.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTFiPlayer_Pause(struct RTFiPlayer *player);


//rt_status_t RTPlayer_Rewind(struct RTPlayer *player, int64_t msec);

/**
 * @brief Resets the Player to its uninitialized state. After calling
 * this method, you will have to initialize it again by setting the
 * source and calling RTFiPlayer_Prepare() or RTFiPlayer_PrepareAsync().
 *
 * @param player The RTFiPlayer object pointer.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | playback is not in idle state.
 * OSAL_ERR_UNKNOWN_ERROR | operation is failed.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTFiPlayer_Reset(struct RTFiPlayer *player);


//rt_status_t RTPlayer_GetCurrentTime(struct RTPlayer *player, int64_t *msec);


//rt_status_t RTPlayer_GetDuration(struct RTPlayer *player, int64_t *msec);

/**
 * @brief Checks whether the player is playing.
 *
 * @param player The RTFiPlayer object pointer.
 * @return 1 if currently playing, 0 otherwise.
 * @since 1.0
 * @version 1.0
 */
int RTFiPlayer_IsPlaying(struct RTFiPlayer *player);

/**
 * @brief Sets fiplayer callbacks.
 *
 * @param player The RTFiPlayer object pointer.
 * @param callbacks The {@link RTFiPlayerCallbacks} instance used to receive fiplayer messages.
 * @since 1.0
 * @version 1.0
 */
void RTFiPlayer_SetCallback(struct RTFiPlayer *player, struct RTFiPlayerCallback *callbacks);


//rt_status_t RTPlayer_SetVolume(struct RTPlayer *player, float left, float right);


//rt_status_t RTPlayer_SetSpeed(struct RTPlayer *player, float speed, float pitch);


//rt_status_t RTPlayer_Invoke(struct RTPlayer *player, Parcel *request, Parcel *reply);

#ifdef __cplusplus
}
#endif

#endif // AMEBA_FWK_MEDIA_PLAYBACK_MEDIA_RTFIPLAYER_H
/** @} */