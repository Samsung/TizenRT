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
 * @brief Declares variables for media framework.
 *
 *
 * @since 1.0
 * @version 1.0
 */

/**
 * @file rtplayer_common.h
 *
 * @brief Provides status, errors, info variables for media framework..
 *
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef AMEBA_FWK_MEDIA_PLAYBACK_MEDIA_RTPLAYER_COMMON_H
#define AMEBA_FWK_MEDIA_PLAYBACK_MEDIA_RTPLAYER_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Defines all the player status.
 *
 * @since 1.0
 * @version 1.0
 */
enum RTPlayerStates {
	/** Used to indicate an idle state */
	RTPLAYER_IDLE = 0,
	/** Used to indicate player is preparing */
	RTPLAYER_PREPARING = 1,
	/** Used to indicate player prepared */
	RTPLAYER_PREPARED = 2,
	/** Used to indicate player started */
	RTPLAYER_STARTED = 3,
	/** Used to indicate player paused */
	RTPLAYER_PAUSED = 4,
	/** Used to indicate player stopped */
	RTPLAYER_STOPPED = 5,
	/** Used to indicate player completed */
	RTPLAYER_PLAYBACK_COMPLETE = 6,
	/** Used to indicate rewind player completed */
	RTPLAYER_REWIND_COMPLETE = 7,
	/** Used to indicate player error */
	RTPLAYER_ERROR = 8,
};

/**
 * @brief Defines the player errors.
 *
 * @since 1.0
 * @version 1.0
 */
enum RTPlayerErrors {
	/** Used to indicate a player error */
	RTPLAYER_ERROR_UNKNOWN = 0,
};

/**
 * @brief Defines the extra informations of a player.
 *
 * @since 1.0
 * @version 1.0
 */
enum RTPlayerInfos {
	/** Player is temporarily pausing playback internally in order to buffer more data. */
	RTPLAYER_INFO_BUFFERING_START = 0,
	/** Player is resuming playback after filling buffers. */
	RTPLAYER_INFO_BUFFERING_END = 1,
	/** Player buffered data percentage update. */
	RTPLAYER_INFO_BUFFERING_INFO_UPDATE = 2,
	/** The media is not rewindable (e.g live stream). */
	RTPLAYER_INFO_NOT_REWINDABLE = 3,
};

/**
 * @brief Defines the invoke method id of a player.
 *
 * @since 1.0
 * @version 1.0
 */
enum RTPlayerInvokeIds {
	/** This method id is responsible for setting extractor probe mode.
	 * The request flow is like this:
	 * size_t mode = 1; // 0: normal probe; 1: fast probe
	 * Parcel *request = Parcel_Create();
	 * Parcel_WriteInt32(request, RTPLAYER_INVOKE_ID_SET_PROBE_MODE);
	 * Parcel_WriteInt32(request, mode);
	 * RTPlayer_Invoke(player, request, NULL);
	 * Parcel_Destroy(request);
	 */
	RTPLAYER_INVOKE_ID_SET_PROBE_MODE = 0,

	/** This method id is responsible for setting buffering parameters for streaming source.
	 * The request flow is like this:
	 * size_t initial_ms = 500;
	 * size_t resume_ms = 1000;
	 * size_t max_buffer_duration_ms = 2000;
	 * Parcel *request = Parcel_Create();
	 * Parcel_WriteInt32(request, RTPLAYER_INVOKE_ID_SET_STREAMING_BUFFERING_PARAMS);
	 * Parcel_WriteInt32(request, initial_ms);
	 * Parcel_WriteInt32(request, resume_ms);
	 * Parcel_WriteInt32(request, max_buffer_duration_ms);
	 * RTPlayer_Invoke(player, request, NULL);
	 * Parcel_Destroy(request);
	 */
	RTPLAYER_INVOKE_ID_SET_STREAMING_BUFFERING_PARAMS = 1,

	/** This method id is responsible for setting extractor probe mode.
	 * The request flow is like this:
	 * size_t buffer_size = 256*1024;
	 * size_t prepared_timeout_ms = 2000;
	 * size_t socket_connect_timeout_ms = 500;
	 * size_t send_timeout_ms = 2000;
	 * size_t recv_timeout_ms = 2000;
	 * size_t read_retry_time_ms = 5000;
	 * Parcel *request = Parcel_Create();
	 * Parcel_WriteInt32(request, RTPLAYER_INVOKE_ID_SET_HTTP_PARAMS);
	 * Parcel_WriteInt32(request, buffer_size);
	 * Parcel_WriteInt32(request, prepared_timeout_ms);
	 * Parcel_WriteInt32(request, socket_connect_timeout_ms);
	 * Parcel_WriteInt32(request, send_timeout_ms);
	 * Parcel_WriteInt32(request, recv_timeout_ms);
	 * Parcel_WriteInt32(request, read_retry_time_ms);
	 * RTPlayer_Invoke(player, request, NULL);
	 * Parcel_Destroy(request);
	 */
	RTPLAYER_INVOKE_ID_SET_HTTP_PARAMS = 2,

	/** This method id is responsible for getting current time.
	 * The request flow is like this:
	 * Parcel *request = Parcel_Create();
	 * Parcel *reply = Parcel_Create();
	 * Parcel_WriteInt32(request, RTPLAYER_INVOKE_ID_GET_CURRENT_TIME);
	 * if (RTPlayer_Invoke(player, request, reply) == OK) {
	 *     int64_t media_us = Parcel_ReadInt64(reply);
	 * }
	 * Parcel_Destroy(request);
	 * Parcel_Destroy(reply);
	 */
	RTPLAYER_INVOKE_ID_GET_CURRENT_TIME = 3,

	/** This method id is responsible for getting cached address and size when cache function enable.
	 * The request flow is like this:
	 * Parcel *request = Parcel_Create();
	 * Parcel *reply = Parcel_Create();
	 * Parcel_WriteInt32(request, RTPLAYER_INVOKE_ID_GET_CACHE_ADDRESS_AND_LENGTH);
	 * char* url = "http://172.29.35.145/River.mp3";
	 * Parcel_WriteCString(request, url);
	 * if (RTPlayer_Invoke(player, request, reply) == OK) {
	 *     void* address = Parcel_ReadPointer(reply);
	 *     int64_t size = Parcel_ReadInt64(reply);
	 * }
	 * Parcel_Destroy(request);
	 * Parcel_Destroy(reply);
	 */
	RTPLAYER_INVOKE_ID_GET_CACHE_ADDRESS_AND_LENGTH = 4,
};

#ifdef __cplusplus
}
#endif

#endif // AMEBA_FWK_MEDIA_PLAYBACK_MEDIA_RTPLAYER_COMMON_H
/** @} */