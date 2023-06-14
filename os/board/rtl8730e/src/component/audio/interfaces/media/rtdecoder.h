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
 * @file rtdecoder.h
 *
 * @brief Declares the APIs for decoding audio data.
 * These interfaces can be used to start, stop, flush decoder and
 * get decoded audio raw data.
 *
 *
 * @since 1.0
 * @version 1.0
 */
#ifndef AMEBA_FWK_MEDIA_PLAYBACK_MEDIA_RTDECODER_H
#define AMEBA_FWK_MEDIA_PLAYBACK_MEDIA_RTDECODER_H

#include "osal_c/osal_types.h"

__BEGIN_DECLS

struct RTMediaFormat;
typedef struct RTMediaFormat RTMediaFormat;

/**
 * @brief Defines all the decoder buffer informations.
 *
 * @since 1.0
 * @version 1.0
 */
struct RTDecoderBufferInfo {
	/** the offset from the start of the decoder buffer */
	int32_t offset;
	/** the buffer size of the decoder buffer */
	int32_t size;
	/** the presentation time stamp of the decoder buffer */
	int64_t pts;
	/** one of the {@link RTDecoderFlags} */
	uint32_t flags;
};
typedef struct RTDecoderBufferInfo RTDecoderBufferInfo;

/**
 * @brief Defines all the decoder flags.
 *
 * @since 1.0
 * @version 1.0
 */
enum RTDecoderFlags {
	/** indicate reaching the end of stream */
	RTDECODER_BUFFERFLAG_END_OF_STREAM = 4,
	/** indicate output buffers changed */
	RTDECODER_INFO_OUTPUT_BUFFERS_CHANGED = -3,
	/** indicate output format changed */
	RTDECODER_INFO_OUTPUT_FORMAT_CHANGED = -2,
	/** indicate no output buffer now, try again later */
	RTDECODER_INFO_TRY_AGAIN_LATER = -1,
};

/**
 * @brief Create RTDecoder by mime type. Most applications will use this, specifying a
 * mime type obtained from media demux.
 *
 * @param mime_type The mime type of the decoder you want to create.
 * @return a new RTDecoder object pointer.
 * @since 1.0
 * @version 1.0
 */
struct RTDecoder *RTDecoder_Create(char *mime_type);

/**
 * @brief Destory RTDecoder.
 *
 * @param decoder The RTDecoder object pointer.
 * @since 1.0
 * @version 1.0
 */
void	RTDecoder_Destory(struct RTDecoder *decoder);

/**
 * @brief Configure the decoder. For decoding you would typically get the format from a demux.
 *
 * @param decoder The RTDecoder object pointer.
 * @param media_format The {@link RTMediaFormat} gained from a demux.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_UNKNOWN_ERROR | operation is failed.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTDecoder_Configure(struct RTDecoder *decoder, struct RTMediaFormat *media_format);

/**
 * @brief Start the decoder. A decoder must be configured before it can be started, and must be started
 * before buffers can be sent to it.
 *
 * @param decoder The RTDecoder object pointer.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_UNKNOWN_ERROR | operation is failed.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTDecoder_Start(struct RTDecoder *decoder);

/**
 * @brief Stop the decoder.
 *
 * @param decoder The RTDecoder object pointer.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_UNKNOWN_ERROR | operation is failed.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTDecoder_Stop(struct RTDecoder *decoder);

/**
 * @brief Flush the decoder.
 *
 * @param decoder The RTDecoder object pointer.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_UNKNOWN_ERROR | operation is failed.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTDecoder_Flush(struct RTDecoder *decoder);

/**
 * @brief Get the index of the next available input buffer. An app will typically use this with
 * {@link RTDecoder#RTDecoder_GetInputBuffer} to get a pointer to the buffer, then copy the data to be encoded or decoded
 * into the buffer before passing it to the decoder.
 *
 * @param decoder The RTDecoder object pointer.
 * @param timeout_us The time out nanoseconds for {@link RTDecoder#RTDecoder_DequeueInputBuffer}.
 * @return Returns an index of an available input buffer.
 * @since 1.0
 * @version 1.0
 */
ssize_t RTDecoder_DequeueInputBuffer(struct RTDecoder *decoder, int64_t timeout_us);

/**
 * @brief Get an input buffer. The specified buffer index must have been previously obtained from
 * {@link RTDecoder#RTDecoder_DequeueInputBuffer}, and not yet queued.
 *
 * @param decoder The RTDecoder object pointer.
 * @param idx The index of an available input buffer gained after {@link RTDecoder#RTDecoder_DequeueInputBuffer}.
 * @param out_size The size of buffer need to feed decoder that has been gained after this function.
 * @return Returns the input buffer pointer.
 * @since 1.0
 * @version 1.0
 */
void *RTDecoder_GetInputBuffer(struct RTDecoder *decoder, size_t idx, size_t *out_size);

/**
 * @brief Queue the specified buffer to the decoder for processing.
 *
 * @param decoder The RTDecoder object pointer.
 * @param idx The index of an available input buffer gained after {@link RTDecoder#RTDecoder_DequeueInputBuffer}.
 * @param offset The offset from the beginning of the buffer.
 * @param size The size of the enqueued buffer.
 * @param time The pts of the enqueued buffer, gained by {@link RTDemux#RTDemux_GetSampleTime}.
 * @param flags The flags, could be {@link RTDecoderFlags#RTDECODER_BUFFERFLAG_END_OF_STREAM} or 0.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_UNKNOWN_ERROR | operation is failed.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTDecoder_QueueInputBuffer(struct RTDecoder *decoder,
									   size_t idx,
									   size_t offset,
									   size_t size,
									   uint64_t time,
									   uint32_t flags);

/**
 * @brief Get the index of the next available buffer of processed data.
 *
 * @param decoder The RTDecoder object pointer.
 * @param info The {@link RTDecoder#RTDecoderBufferInfo} of the output processed data.
 * @param timeout_us The time out nanoseconds for {@link RTDecoder#RTDecoder_DequeueOutputBuffer}.
 *
 * @return Returns the output buffer index if the operation is successful; returns an error code listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_UNKNOWN_ERROR | operation is failed.
 * @since 1.0
 * @version 1.0
 */
ssize_t RTDecoder_DequeueOutputBuffer(struct RTDecoder *decoder, struct RTDecoderBufferInfo *info, int64_t timeout_us);

/**
 * @brief Get an output buffer. The specified buffer index must have been previously obtained from
 * {@link RTDecoder#RTDecoder_DequeueOutputBuffer}, and not yet queued.
 *
 * @param decoder The RTDecoder object pointer.
 * @param idx The index of an available output buffer gained after {@link RTDecoder#RTDecoder_DequeueOutputBuffer}.
 * @param out_size The size of buffer gained after this function.
 * @return Returns the output buffer pointer.
 * @since 1.0
 * @version 1.0
 */
void *RTDecoder_GetOutputBuffer(struct RTDecoder *decoder, size_t idx, size_t *out_size);

/**
 * @brief If you are done with a buffer, use this call to return the buffer to
 * the decoder.
 *
 * @param decoder The RTDecoder object pointer.
 * @param idx The index of the output buffer you want to release.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_UNKNOWN_ERROR | operation is failed.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTDecoder_ReleaseOutputBuffer(struct RTDecoder *decoder, size_t idx);

/**
 * @brief Get output format after decoding.
 *
 * @param decoder The RTDecoder object pointer.
 * @param rate The audio sample rate after decoding.
 * @param channel_count The audio channel count after decoding.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_UNKNOWN_ERROR | operation is failed.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTDecoder_GetOutputFormat(struct RTDecoder *decoder, int32_t *rate, int32_t *channel_count);

__END_DECLS

#endif // AMEBA_FWK_MEDIA_PLAYBACK_MEDIA_RTDECODER_H
/** @} */
