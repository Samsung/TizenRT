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
 * @file rtdemux.h
 *
 * @brief Declares the APIs for demuxing an audio file.
 * These interfaces can be used to demux an audio file, get audio format
 * informations, audio data and so on.
 *
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef AMEBA_FWK_MEDIA_PLAYBACK_MEDIA_RTDEMUX_H
#define AMEBA_FWK_MEDIA_PLAYBACK_MEDIA_RTDEMUX_H

#include "osal_c/osal_types.h"

__BEGIN_DECLS

struct RTMediaFormat;
typedef struct RTMediaFormat RTMediaFormat;

/**
 * @brief Creates RTDemux.
 *
 * @return a new RTDemux object pointer.
 * @since 1.0
 * @version 1.0
 */
struct RTDemux *RTDemux_Create();

/**
 * @brief Destory RTDemux.
 *
 * @param demux The RTDemux object pointer.
 * @since 1.0
 * @version 1.0
 */
void	RTDemux_Destory(struct RTDemux *demux);

/**
 * @brief Set the file descriptor from which the demux will read.
 *
 * @param demux The RTDemux object pointer.
 * @param path The path of the file you want to demux.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_UNKNOWN_ERROR | operation is failed.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTDemux_SetSource(struct RTDemux *demux, const char *path);

/**
 * @brief Get the number of tracks in the previously specified media file.
 *
 * @param demux The RTDemux object pointer.
 * @return Returns track count.
 * @since 1.0
 * @version 1.0
 */
int RTDemux_GetTrackCount(struct RTDemux *demux);

/**
 * @brief Get the format of the specified track. The caller must free the returned format.
 *
 * @param demux The RTDemux object pointer.
 * @param idx The index of the track.
 * @param media_format The {@link RTMediaFormat} which will be gained during this function.
 * @param mime The mime information of the file which will be gained during this function.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_UNKNOWN_ERROR | operation is failed.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTDemux_GetTrackFormat(struct RTDemux *demux, size_t idx, struct RTMediaFormat **media_format, char **mime);

/**
 * @brief Select the specified track.
 * Selecting the same track multiple times has no effect, the track is
 * only selected once.
 *
 * @param demux The RTDemux object pointer.
 * @param idx The index of the track.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_UNKNOWN_ERROR | operation is failed.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTDemux_SelectTrack(struct RTDemux *demux, size_t idx);

/**
 * @brief Unselect the specified track.
 *
 * @param demux The RTDemux object pointer.
 * @param idx The index of the track.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_UNKNOWN_ERROR | operation is failed.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTDemux_UnselectTrack(struct RTDemux *demux, size_t idx);

/**
 * @brief Read the current sample data.
 *
 * @param demux The RTDemux object pointer.
 * @param buffer The buffer pointer that stores the data, gained by {@link RTDecoder_GetInputBuffer}.
 * @param capacity The max buffer size that can be read, gained by {@link RTDecoder_GetInputBuffer}.
 * @return Returns a sample size that has been actually demuxed.
 * @since 1.0
 * @version 1.0
 */
ssize_t RTDemux_ReadSampleData(struct RTDemux *demux, void *buffer, size_t capacity);

/**
 * @brief Get the current sample's presentation time in microseconds.
 *
 * @param demux The RTDemux object pointer.
 * @return Returns the current sample's presentation time in microseconds
 * or -1 if no more samples are available.
 * @since 1.0
 * @version 1.0
 */
int64_t RTDemux_GetSampleTime(struct RTDemux *demux);

/**
 * @brief Advance to the next sample.
 *
 * @param demux The RTDemux object pointer.
 * @return Returns 0 if no more sample data is available (end of stream).
 * @since 1.0
 * @version 1.0
 */
int RTDemux_Advance(struct RTDemux *demux);

/**
 * @brief Moves the media to specified time position.
 *
 * @param demux The RTDemux object pointer.
 * @param time_us The offset in milliseconds from the start to rewind to.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_UNKNOWN_ERROR | operation is failed.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTDemux_Rewind(struct RTDemux *demux, int64_t time_us);

__END_DECLS

#endif // AMEBA_FWK_MEDIA_PLAYBACK_MEDIA_RTDEMUX_H
/** @} */
