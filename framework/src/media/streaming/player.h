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

#ifndef STREAMING_PLAYER_H
#define STREAMING_PLAYER_H

#include "../utils/rb.h"
#include "../utils/rbs.h"
#include <audiocodec/mp3dec/pvmp3decoder_api.h>
#include <audiocodec/aacdec/pvmp4audiodecoder_api.h>
#include <media/MediaTypes.h>
#ifdef CONFIG_CODEC_LIBOPUS
#include "../codecs/opus_decoder_api.h"
#endif

namespace media {

struct pv_player_s;
typedef struct pv_player_s pv_player_t;
typedef struct pv_player_s *pv_player_p;

/**
 * @brief  input callback routine type.
 *
 * @param  user_data: Pointer to user callback data registered with callback routine
 * @param  player: Pointer to player object
 * @return size of audio data pushed to player via pv_player_pushdata().
 *         in case of return 0, that means end of stream, then decoding will be terminated.
 */
typedef size_t(*input_func_f)(void *user_data, pv_player_p player);

/**
 * @struct  pv_player_s
 * @brief   Player structure, support decoding various types of audio stream, see enum audio_type_e.
 *          User should define and mantain player object in stack or heap, and give object pointer
 *          to pv_player_init(), and specify ring-buffer size at the same time.
 */
struct pv_player_s {
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
 * @brief  stream player initialize.
 *
 * @param  player : Pointer to player object
 * @param  rbuf_size: specify ring-buffer size for decoder
 * @return 0 on success, otherwise, return -1.
 */
int pv_player_init(pv_player_p player, size_t rbuf_size);

/**
 * @brief  stream player register callbacks.
 *         It's an optional function, it's possible to push data at any time.
 * @param  player : Pointer to player object
 * @param  data: ponter to user data, will be passed to callback routine
 * @param  input: audio data input callback routine
 * @return 0 on success, otherwise, return -1.
 */
int pv_player_register_callbacks(pv_player_p player, void *data, input_func_f input);

/**
 * @brief  stream player deinitialize.
 *
 * @param  player : Pointer to player object
 * @return 0 on success, otherwise, return -1.
 */
int pv_player_finish(pv_player_p player);

/**
 * @brief  push audio source data to internal ring-buffer of player.
 *         It's possible to call this API to push data at any time,
 *         It's required to call this API to push data when user input callback called.
 *
 * @param  player : Pointer to player object
 * @param  data: Pointer to buffer, contains audio source data.
 * @param  len: size in bytes of audio source data contained in buffer.
 * @return size in bytes of data actually accepted by player.
 */
size_t pv_player_pushdata(pv_player_p player, const void *data, size_t len);

/**
 * @brief  get free data space in player, which means the maximum of data to push.
 *
 * @param  player : Pointer to player object
 * @return size in bytes of free space in internal ring-buffer of player.
 */
size_t pv_player_dataspace(pv_player_p player);

/**
 * @brief  check data space is empty or not.
 *
 * @param  player : Pointer to player object
 * @return true if data space is empty. Otherwise, false.
 */
bool pv_player_dataspace_is_empty(pv_player_p player);

/**
 * @brief  get audio type from audio stream.
 *
 * @param  player : Pointer to player object
 * @return audio type enum value.
 * @see    enum audio_type_e
 */
int pv_player_get_audio_type(pv_player_p player);

/**
 * @brief  initilize decoder after get audio type.
 *         frame data is stored in player structure.
 * @param  player : Pointer to player object
 * @param  audio_type : specify the audio type of the input audio stream
 *         you can get the type via pv_player_get_audio_type().
 * @param  dec_ext : Pointer to decoder external structure object,
 *         caller must init external structure according to 'audio_type'.
 * @return 0 on success, -1 on failure.
 * @see pv_player_get_audio_type()
 */
int pv_player_init_decoder(pv_player_p player, int audio_type, void *dec_ext);

/**
 * @brief  Get sample frames in 16bit-PCM format.
 *
 * @param  player : Pointer to player object
 * @param  buf : Pointer to output buffer, which store outputed PCM data
 * @param  max : Max bytes of output buffer, it should be 16bit-PCM frame aligned.
 * @param  sr : Sample rate (Hz) of the output PCM data
 * @param  ch : Number of channels of the output PCM data
 * @return bytes of PCM data saved in 'buf', usually it's same as 'max'.
 *         it may be less than 'max' when there's no more data to decode.
 */
size_t pv_player_get_frames(pv_player_p player, unsigned char *buf, size_t max, unsigned int *sr, unsigned short *ch);

} // namespace media

#endif /* STREAMING_PLAYER_H */

