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
 * @file rtplayer_lite.h
 *
 * @brief Provides the APIs to implement operations related to manage playback.
 * These player interfaces can be used to control playback of audio files and streams(via https or rtsp),
 * register observer functions, and control the feature status.
 * Playback control includs start, stop, pause, resume, rewind and so on.
 *
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef AMEBA_FWK_MEDIA_PLAYBACK_MEDIA_RTPLAYER_LITE_H
#define AMEBA_FWK_MEDIA_PLAYBACK_MEDIA_RTPLAYER_LITE_H

#include "osal_c/osal_types.h"
#include "cutils/parcel.h"

#include "rtplayer_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RTPlayerLite RTPlayerLite;
typedef struct RTPlayerLiteCallback RTPlayerLiteCallback;

/**
 * @brief Provides calback interfaces.
 */
struct RTPlayerLiteCallback {
	/**
	 * @brief Called when player status is changed.
	 *
	 * @param listener The RTPlayerLiteCallback object pointer.
	 * @param player The RTPlayerLite object pointer.
	 * @param state The player status, one of {@link RTPlayerStates}.
	 * @since 1.0
	 * @version 1.0
	 */
	void (*OnRTPlayerLiteStateChanged)(const struct RTPlayerLiteCallback *listener, const struct RTPlayerLite *player, int state);

	/**
	 * @brief Called when player information is received.
	 *
	 * @param listener The RTPlayerLiteCallback object pointer.
	 * @param player The RTPlayerLite object pointer.
	 * @param info Indicates the information type. For details, see {@link RTPlayerInfos}.
	 * @param extra Indicates the information code.
	 * @since 1.0
	 * @version 1.0
	 */
	void (*OnRTPlayerLiteInfo)(const struct RTPlayerLiteCallback *listener, const struct RTPlayerLite *player, int info, int extra);

	/**
	 * @brief Called when a player error occurs.
	 *
	 * @param listener The RTPlayerLiteCallback object pointer.
	 * @param player The RTPlayerLite object pointer.
	 * @param error Indicates the error type. For details, see {@link RTPlayerErrors}.
	 * @param extra Indicates the error code.
	 * @since 1.0
	 * @version 1.0
	 */
	void (*OnRTPlayerLiteError)(const struct RTPlayerLiteCallback *listener, const struct RTPlayerLite *player, int error, int extra);
};

/**
 * @brief Creates RTPlayerLite.
 *
 * @return a new RTPlayerLite object pointer.
 * @since 1.0
 * @version 1.0
 */
struct RTPlayerLite *RTPlayerLite_Create(void);

/**
 * @brief Destory RTPlayerLite.
 *
 * @param player The RTPlayerLite object pointer.
 * @since 1.0
 * @version 1.0
 */
void	RTPlayerLite_Destory(struct RTPlayerLite *player);

/**
 * @brief Sets the source(url) to use.
 *
 * @param player The RTPlayerLite object pointer.
 * @param url The url of the file you want to play.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | playback is not in idle state.
 * OSAL_ERR_UNKNOWN_ERROR | operation is failed.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTPlayerLite_SetSource(struct RTPlayerLite *player, const char *url);

/**
 * @brief Prepares the player for playback, synchronously.
 *
 * @param player The RTPlayerLite object pointer.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | playback is not in idle state.
 * OSAL_ERR_UNKNOWN_ERROR | operation is failed.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTPlayerLite_Prepare(struct RTPlayerLite *player);

/**
 * @brief Prepares the player for playback, asynchronously.
 * A call to {@link RTPlayerLite_PrepareAsync()} (asynchronous) will first transfers
 * the playbackt to the RTPLAYER_PREPARING state after the call returns
 * (which occurs almost right away) while the internal player engine continues
 * working on the rest of preparation work until the preparation work completes.
 * When the preparation completes, the internal player engine then calls a user
 * supplied callback method OnRTPlayerLiteStateChanged(..., RTPLAYER_PREPARED)
 * of the RTPlayerLiteCallbacks interface, if an RTPlayerLiteCallbacks object is registered
 * beforehand via {@link RTPlayerLite_SetCallbacks()}.
 *
 * @param player The RTPlayerLite object pointer.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | playback is not in idle state.
 * OSAL_ERR_UNKNOWN_ERROR | operation is failed.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTPlayerLite_PrepareAsync(struct RTPlayerLite *player);

/**
* @brief Starts or resumes playback. If playback had previously been paused,
* playback will continue from where it was paused. If playback had
* been stopped, or never started before, playback will start at the
* beginning.
*
* @param player The RTPlayerLite object pointer.
* @return Returns a value listed below: \n
* rt_status_t | Description
* ----------------------| -----------------------
* OSAL_OK | the operation is successful.
* OSAL_ERR_INVALID_OPERATION | playback is not in idle state.
* OSAL_ERR_UNKNOWN_ERROR | operation is failed.
* @since 1.0
* @version 1.0
*/
rt_status_t RTPlayerLite_Start(struct RTPlayerLite *player);

/**
 * @brief Stops playback after playback has been started or paused.
 *
 * @param player The RTPlayerLite object pointer.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | playback is not in idle state.
 * OSAL_ERR_UNKNOWN_ERROR | operation is failed.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTPlayerLite_Stop(struct RTPlayerLite *player);

/**
 * @brief Pauses playback. Call RTPlayerLite_Start() to resume.
 *
 * @param player The RTPlayerLite object pointer.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | playback is not in idle state.
 * OSAL_ERR_UNKNOWN_ERROR | operation is failed.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTPlayerLite_Pause(struct RTPlayerLite *player);

/**
 * @brief Moves the media to specified time position.
 *
 * @param player The RTPlayerLite object pointer.
 * @param msec The offset in milliseconds from the start to rewind to.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | playback is not in idle state.
 * OSAL_ERR_UNKNOWN_ERROR | operation is failed.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTPlayerLite_Rewind(struct RTPlayerLite *player, int64_t msec);

/**
 * @brief Resets the Player to its uninitialized state. After calling
 * this method, you will have to initialize it again by setting the
 * source and calling RTPlayerLite_Prepare() or RTPlayerLite_PrepareAsync().
 *
 * @param player The RTPlayerLite object pointer.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | playback is not in idle state.
 * OSAL_ERR_UNKNOWN_ERROR | operation is failed.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTPlayerLite_Reset(struct RTPlayerLite *player);

/**
 * @brief Gets the current playback position.
 *
 * @param player The RTPlayerLite object pointer.
 * @param msec The current time updated during this function.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | playback is not in idle state.
 * OSAL_ERR_UNKNOWN_ERROR | operation is failed.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTPlayerLite_GetCurrentTime(struct RTPlayerLite *player, int64_t *msec);

/**
 * @brief Gets the duration of the file.
 *
 * @param player The RTPlayerLite object pointer.
 * @param msec The duration in milliseconds, if no duration is available
 *         (for example, if streaming live content), -1 is updated.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | playback is not in idle state.
 * OSAL_ERR_UNKNOWN_ERROR | operation is failed.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTPlayerLite_GetDuration(struct RTPlayerLite *player, int64_t *msec);

/**
 * @brief Checks whether the player is playing.
 *
 * @param player The RTPlayerLite object pointer.
 * @return 1 if currently playing, 0 otherwise.
 * @since 1.0
 * @version 1.0
 */
int RTPlayerLite_IsPlaying(struct RTPlayerLite *player);

/**
 * @brief Sets player callbacks.
 *
 * @param player The RTPlayerLite object pointer.
 * @param callbacks The {@link RTPlayerLiteCallbacks} instance used to receive player messages.
 * @since 1.0
 * @version 1.0
 */
void RTPlayerLite_SetCallback(struct RTPlayerLite *player, struct RTPlayerLiteCallback *callbacks);

/**
 * @brief Sets player volume when play started.
 *
 * @param player The RTPlayerLite object pointer.
 * @param left The volume of left channel.
 * @param right The volume of right channel.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | playback is not in idle state.
 * OSAL_ERR_UNKNOWN_ERROR | operation is failed.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTPlayerLite_SetVolume(struct RTPlayerLite *player, float left, float right);

/**
 * @brief Sets player speed when play started.
 *
 * @param player The RTPlayerLite object pointer.
 * @param speed The speed of player.
 * @param speed The pitch of player.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | playback is not in idle state.
 * OSAL_ERR_UNKNOWN_ERROR | operation is failed.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTPlayerLite_SetSpeed(struct RTPlayerLite *player, float speed, float pitch);

/**
 * @brief Invokes player request.
 *
 * @param request The request parcel object pointer.
 * @param reply The reply parcel object pointer..
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | playback is not in idle state.
 * OSAL_ERR_UNKNOWN_ERROR | operation is failed.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTPlayerLite_Invoke(struct RTPlayerLite *player, Parcel *request, Parcel *reply);

#ifdef __cplusplus
}
#endif

#endif // AMEBA_FWK_MEDIA_PLAYBACK_MEDIA_RTPLAYER_LITE_H
/** @} */