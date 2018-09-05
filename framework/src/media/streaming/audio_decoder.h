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

#ifndef STREAMING_DECODER_H
#define STREAMING_DECODER_H

#include "../utils/rb.h"
#include "../utils/rbs.h"
#include <audiocodec/mp3dec/pvmp3decoder_api.h>
#include <audiocodec/aacdec/pvmp4audiodecoder_api.h>
#include <media/MediaTypes.h>
#ifdef CONFIG_CODEC_LIBOPUS
#include "../codecs/opus_decoder_api.h"
#endif

#define AUDIO_DECODER_OK 0
#define AUDIO_DECODER_ERROR -1

namespace media {

struct audio_decoder_s;
typedef struct audio_decoder_s audio_decoder_t;
typedef struct audio_decoder_s *audio_decoder_p;

/**
 * @brief  input callback routine type.
 *
 * @param  user_data: Pointer to user callback data registered with callback routine
 * @param  decoder: Pointer to decoder object
 * @return size of audio data pushed to decoder via audio_decoder_pushdata().
 *         in case of return 0, that means end of stream, then decoding will be terminated.
 */
typedef size_t(*input_func_f)(void *user_data, audio_decoder_p decoder);

/**
 * @struct  audio_decoder_s
 * @brief   Decoder structure, support decoding various types of audio stream, see enum audio_type_e.
 *          User should define and mantain decoder object in stack or heap, and give object pointer
 *          to audio_decoder_init(), and specify ring-buffer size at the same time.
 */
struct audio_decoder_s {
	int audio_type;             /* indicates the format of current audio stream, see enum audio_type_e */

	// decoder buffer
	void *dec_ext;              /* decoder external struct, configured by user according to audio_type */
	void *dec_mem;              /* decoder required memory, used internally */

	// user callback func and data
	void *cb_data;              /* data ptr user registered, be passed to callback function. */
	input_func_f input_func;    /* input callback, be called when decoder request more data. */

	// internal member used by decoder
	rb_t ringbuffer;            /* ring-buffer object */
	rbstream_p rbsp;            /* ring-buffer stream handle, co-work with above ring-buffer */

	// private data
	void *priv_data;            /* pointer to private data */
};

/**
 * @brief  stream decoder initialize.
 *
 * @param  decoder : Pointer to decoder object
 * @param  rbuf_size: specify ring-buffer size for decoder
 * @return 0 on success, otherwise, return -1.
 */
int audio_decoder_init(audio_decoder_p decoder, size_t rbuf_size);

/**
 * @brief  stream decoder register callbacks.
 *         It's an optional function, it's possible to push data at any time.
 * @param  decoder : Pointer to decoder object
 * @param  data: ponter to user data, will be passed to callback routine
 * @param  input: audio data input callback routine
 * @return 0 on success, otherwise, return -1.
 */
int audio_decoder_register_callbacks(audio_decoder_p decoder, void *data, input_func_f input);

/**
 * @brief  stream decoder deinitialize.
 *
 * @param  decoder : Pointer to decoder object
 * @return 0 on success, otherwise, return -1.
 */
int audio_decoder_finish(audio_decoder_p decoder);

/**
 * @brief  push audio source data to internal ring-buffer of decoder.
 *         It's possible to call this API to push data at any time,
 *         It's required to call this API to push data when user input callback called.
 *
 * @param  decoder : Pointer to decoder object
 * @param  data: Pointer to buffer, contains audio source data.
 * @param  len: size in bytes of audio source data contained in buffer.
 * @return size in bytes of data actually accepted by decoder.
 */
size_t audio_decoder_pushdata(audio_decoder_p decoder, const void *data, size_t len);

/**
 * @brief  get free data space in decoder, which means the maximum of data to push.
 *
 * @param  decoder : Pointer to decoder object
 * @return size in bytes of free space in internal ring-buffer of decoder.
 */
size_t audio_decoder_dataspace(audio_decoder_p decoder);

/**
 * @brief  check data space is empty or not.
 *
 * @param  decoder : Pointer to decoder object
 * @return true if data space is empty. Otherwise, false.
 */
bool audio_decoder_dataspace_is_empty(audio_decoder_p decoder);

/**
 * @brief  get audio type from audio stream.
 *
 * @param  decoder : Pointer to decoder object
 * @return audio type enum value.
 * @see    enum audio_type_e
 */
int audio_decoder_get_audio_type(audio_decoder_p decoder);

/**
 * @brief  initilize decoder after get audio type.
 *         frame data is stored in decoder structure.
 * @param  decoder : Pointer to decoder object
 * @param  audio_type : specify the audio type of the input audio stream
 *         you can get the type via audio_decoder_get_audio_type().
 * @param  dec_ext : Pointer to decoder external structure object,
 *         caller must init external structure according to 'audio_type'.
 * @return 0 on success, -1 on failure.
 * @see audio_decoder_get_audio_type()
 */
int audio_decoder_configure(audio_decoder_p decoder, int audio_type, void *dec_ext);

/**
 * @brief  Get sample frames in 16bit-PCM format.
 *
 * @param  decoder : Pointer to decoder object
 * @param  buf : Pointer to output buffer, which store outputed PCM data
 * @param  max : Max bytes of output buffer, it should be 16bit-PCM frame aligned.
 * @param  sr : Sample rate (Hz) of the output PCM data
 * @param  ch : Number of channels of the output PCM data
 * @return bytes of PCM data saved in 'buf', usually it's same as 'max'.
 *         it may be less than 'max' when there's no more data to decode.
 */
size_t audio_decoder_get_frames(audio_decoder_p decoder, unsigned char *buf, size_t max, unsigned int *sr, unsigned short *ch);

} // namespace media

#endif /* STREAMING_DECODER_H */

