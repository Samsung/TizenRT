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

#include "rbs.h"
#include <audiocodec/mp3dec/pvmp3decoder_api.h>
#include <audiocodec/aacdec/pvmp4audiodecoder_api.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Audio type detected by player from audio stream.
 */
enum audio_type_e {
	AUDIO_TYPE_INVALID = 0,
	AUDIO_TYPE_UNKNOWN = AUDIO_TYPE_INVALID,
	AUDIO_TYPE_MP3 = 1,
	AUDIO_TYPE_AAC = 2,

	// add new type above.
	AUDIO_TYPE_MAX
};

typedef enum audio_type_e audio_type_t;

/**
 * Deprecated, to be removed.
 */
enum {
	type_unknown = AUDIO_TYPE_UNKNOWN,
	type_mp3 = AUDIO_TYPE_MP3,
	type_aac = AUDIO_TYPE_AAC,
	type_max,
};

/**
 * @struct  pcm_data_s
 * @brief   Define PCM data output structure, inludes sample rate, channel number,
 *          and samples buffer and count.
 */
struct pcm_data_s {
	unsigned int samplerate;    /* sampling frequency (Hz) */
	unsigned short channels;    /* number of channels */
	unsigned short length;      /* number of the output samples in 16-bit words */
	signed short *samples;      /* PCM output 16-bit samples, include both L/R ch for stereo case */
};

typedef struct pcm_data_s pcm_data_t;
typedef struct pcm_data_s *pcm_data_p;

struct pv_player_s;
typedef struct pv_player_s pv_player_t;
typedef struct pv_player_s *pv_player_p;

/**
 * @brief  configure callback routine type.
 *
 * @param  user_data: Pointer to user callback data register via pv_player_init()
 * @param  audio_type: the audio format of current playing audio, type_mp3, type_aac
 * @param  dec_cfg: Pointer to an object of tPVMP3DecoderExternal or tPVMP4AudioDecoderExternal,
 *         according to certain audio_type.
 * @return 0 on success, otherwise, return -1.
 */
typedef int (*config_func_f)(void *user_data, int audio_type, void *dec_cfg);

/**
 * @brief  input callback routine type.
 *
 * @param  user_data: Pointer to user callback data register via pv_player_init()
 * @param  player: Pointer to player object
 * @return size of audio data pushed to player via pv_player_pushdata().
 *         in case of return 0, that means end of stream, then decoding will be terminated.
 */
typedef size_t(*input_func_f)(void *user_data, pv_player_p player);

/**
 * @brief  output callback routine type.
 *
 * @param  user_data: Pointer to user callback data register via pv_player_init()
 * @param  player: Pointer to player object
 * @param  pcm_dat: Pointer to an object of structure pcm_data_s.
 * @return value not be used now.
 */
typedef size_t(*output_func_f)(void *user_data, pv_player_p player, pcm_data_p pcm_dat);

/**
 * @struct  pv_player_s
 * @brief   Player structure, support decoding various types of audio stream, see enum audio_type_e.
 *          User should define and mantain player object in stack or heap, and give object pointer
 *          to pv_player_init(), and specify input/output/config callback routines at the same time.
 */
struct pv_player_s {
	int audio_type;             /* indicates the format of current audio stream, mp3 or aac */

	// decoder buffer
	void *dec_ext;              /* decoder external struct, configured by user via config_func_f */
	void *dec_mem;              /* decoder required memory, used internally */

	// user callback func and data
	void *cb_data;              /* data ptr user registered, be passed to callback function. */
	config_func_f config_func;
	input_func_f input_func;    /* input callback, be called when decoder request more data. */
	output_func_f output_func;  /* output callback, be called when decoder output PCM data. */

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
 * @param  data: ponter to user callback data
 * @param  config: decoder config callback routine
 * @param  input: audio data input callback routine
 * @param  output: PCM data output callback routine
 * @return 0 on success, otherwise, return -1.
 */
int pv_player_init(pv_player_p player, size_t rbuf_size, void *data, config_func_f config, input_func_f input, output_func_f output);

/**
 * @brief  stream player deinitialize.
 *
 * @param  player : Pointer to player object
 * @return 0 on success, otherwise, return -1.
 */
int pv_player_finish(pv_player_p player);

/**
 * @brief  stream player main-loop.

 *         read audio data from ring-buffer, after recognized the audio type,
 *         invoke config callback to configure decoder, and then start decoding.
 *         user config/input/output callback would be called during this main-loop
 *
 * @param  player : Pointer to player object
 * @return 0 on success, otherwise, return -1.
 */
int pv_player_run(pv_player_p player);

/**
 * @brief  push audio source data to internal ring-buffer of player.
 *         It's possible to call this API to push data at any time during pv_player_run() running,
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

int _get_audio_type(rbstream_p rbsp);
bool _get_frame(pv_player_p player);
int _init_decoder(pv_player_p player);
int _frame_decoder(pv_player_p player, pcm_data_p pcm);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* STREAMING_PLAYER_H */

