/******************************************************************
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

#ifndef STREAMING_ENCODER_H
#define STREAMING_ENCODER_H

#include "../utils/rb.h"
#include "../utils/rbs.h"
#include <audiocodec/opus/opus_encoder_api.h>
#include <media/MediaTypes.h>

namespace media {

struct aud_encoder_s;
typedef struct aud_encoder_s aud_encoder_t;
typedef struct aud_encoder_s *aud_encoder_p;

/**
 * @brief  input callback routine type.
 *
 * @param  user_data: Pointer to user callback data register via aud_encoder_init()
 * @param  encoder: Pointer to encoder object
 * @return size of audio data pushed to encoder via aud_encoder_pushdata().
 *         in case of return 0, that means end of PCM stream, then encoding will be terminated.
 */
typedef size_t(*input_func_f)(void *user_data, aud_encoder_p encoder);

/**
 * @struct  aud_encoder_s
 * @brief   Recorder structure, support encoding PCM stream to specified audio type, see enum audio_type_e.
 *          Currently, support encode type is OPUS. And might support other type in future.
 *          User should define and mantain encoder object in stack or heap, and give object pointer
 *          to aud_encoder_init(), and specify input callback.
 */
struct aud_encoder_s {
	int audio_type;             /* the format of output audio type desired, e.g. opus */

	// encoder buffer
	void *enc_ext;              /* encoder external struct, user can pass configuration from it */
	void *enc_mem;              /* encoder required memory, used internally */

	// user callback func and data
	void *cb_data;              /* data ptr user registered, be passed to callback function. */
	input_func_f input_func;    /* input callback, be called when encoder request more data. */

	// internal member used by encoder
	rb_t ringbuffer;            /* ring-buffer object */
	rbstream_p rbsp;            /* ring-buffer stream handle, co-work with above ring-buffer */

	// private data
	void *priv_data;            /* pointer to private data */
};

/**
 * @brief  stream encoder initialize.
 *
 * @param  encoder : Pointer to encoder object
 * @param  rbuf_size : specify ring-buffer size for encoder
 * @param  audio_type : output encoded audio type
 * @param  enc_ext : Pointer to encoder external struct, ex: opus use opus_enc_external_t
 * @return 0 on success, otherwise, return -1.
 */
int aud_encoder_init(aud_encoder_p encoder, size_t rbuf_size, audio_type_t audio_type, void *enc_ext);

/**
 * @brief  stream encoder register user data and user input callback.
 *
 * @param  encoder : Pointer to encoder object
 * @param  data: ponter to user callback data
 * @param  input: audio data input callback routine
 * @return 0 on success, otherwise, return -1.
 */
int aud_encoder_register_callbacks(aud_encoder_p encoder, void *data, input_func_f input);

/**
 * @brief  stream encoder deinitialize.
 *
 * @param  encoder : Pointer to encoder object
 * @return 0 on success, otherwise, return -1.
 */
int aud_encoder_finish(aud_encoder_p encoder);

/**
 * @brief  push audio source data to internal ring-buffer of encoder.
 *         It's possible to call this API to push data at any time during aud_encoder_run() running,
 *         It's required to call this API to push data when user input callback called.
 *
 * @param  encoder : Pointer to encoder object
 * @param  data: Pointer to buffer, contains source PCM data.
 * @param  len: size in bytes of audio source data contained in buffer.
 * @return size in bytes of data actually accepted by encoder.
 */
size_t aud_encoder_pushdata(aud_encoder_p encoder, const void *data, size_t len);

/**
 * @brief  get free data space in encoder, which means the maximum of data to push.
 *
 * @param  encoder : Pointer to encoder object
 * @return size in bytes of free space in internal ring-buffer of encoder.
 */
size_t aud_encoder_dataspace(aud_encoder_p encoder);

/**
 * @brief  check data space is empty or not.
 *
 * @param  encoder : Pointer to encoder object
 * @return true if data space is empty. Otherwise, false.
 */
bool aud_encoder_dataspace_is_empty(aud_encoder_p encoder);

/**
 * @brief  get encoder output frame .
 *         ex: OPUS frame data is stored in opus_enc_external_t structure.
 * @param  encoder : Pointer to encoder object
 * @param  data : Pointer to output data buffer
 * @param  len : max bytes of output buffer
 * @return size in bytes of audio data when success, or return -1.
 */
int aud_encoder_getframe(aud_encoder_p encoder, void * data, size_t len);

} // namespace media

#endif /* STREAMING_ENCODER_H */

