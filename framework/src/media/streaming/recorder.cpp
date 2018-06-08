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

#include <tinyara/config.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <pthread.h>
#include <debug.h>
#include "recorder.h"
#include "../utils/internal_defs.h"

namespace media {

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define PV_SUCCESS OK
#define PV_FAILURE ERROR

/****************************************************************************
 * Private Declarations
 ****************************************************************************/

/**
 * @struct  priv_data_s
 * @brief   Player private data structure define.
 */
struct priv_data_s {
	ssize_t mCurrentPos;        /* read position when encoding */
};

typedef struct priv_data_s priv_data_t;
typedef struct priv_data_s *priv_data_p;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static size_t _input_callback(void *data, rbstream_p rbsp)
{
	aud_encoder_p encoder = (aud_encoder_p) data;
	assert(encoder != NULL);

	size_t wlen = 0;
	RETURN_VAL_IF_FAIL((encoder->input_func != NULL), wlen);
	wlen = encoder->input_func(encoder->cb_data, encoder);

	return wlen;
}

static int _init_encoder(aud_encoder_p encoder, void *enc_ext)
{
	priv_data_p priv = (priv_data_p) encoder->priv_data;
	assert(priv != NULL);

	switch (encoder->audio_type) {
#ifdef CONFIG_CODEC_LIBOPUS
	case AUDIO_TYPE_OPUS: {
		encoder->enc_ext = calloc(1, sizeof(opus_enc_external_t));
		RETURN_VAL_IF_FAIL((encoder->enc_ext != NULL), PV_FAILURE);

		encoder->enc_mem = calloc(1, opus_encoderMemRequirements());
		RETURN_VAL_IF_FAIL((encoder->enc_mem != NULL), PV_FAILURE);

		opus_enc_external_t *opus_ext = (opus_enc_external_t *) encoder->enc_ext;
		*opus_ext = * ((opus_enc_external_t *) enc_ext);

		// TODO: check opus_ext-> param validation
		RETURN_VAL_IF_FAIL((opus_ext->pInputBuffer != NULL), PV_FAILURE);
		RETURN_VAL_IF_FAIL((opus_ext->pOutputBuffer != NULL), PV_FAILURE);

		opus_resetEncoder(encoder->enc_mem);
		int err = opus_initEncoder((opus_enc_external_t *) encoder->enc_ext, encoder->enc_mem);
		RETURN_VAL_IF_FAIL((err == OPUS_OK), PV_FAILURE);

		priv->mCurrentPos = 0;
		break;
	}
#endif

	default:
		// Maybe do not need to init, return success.
		return PV_SUCCESS;
	}

	return PV_SUCCESS;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

// check if the given auido type is supportted or not
bool aud_encoder_check_audio_type(int audio_type)
{
	switch (audio_type) {
#ifdef CONFIG_CODEC_LIBOPUS
	case AUDIO_TYPE_OPUS:
		return true;
#endif

	default:
		return false;
	}
}

size_t aud_encoder_pushdata(aud_encoder_p encoder, const void *data, size_t len)
{
	assert(encoder != NULL);
	assert(data != NULL);

	static pthread_mutex_t s_mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_lock(&s_mutex);
	len = rbs_write(data, 1, len, encoder->rbsp);
	pthread_mutex_unlock(&s_mutex);

	return len;
}

size_t aud_encoder_dataspace(aud_encoder_p encoder)
{
	assert(encoder != NULL);

	return rb_avail(&encoder->ringbuffer);
}

bool aud_encoder_dataspace_is_empty(aud_encoder_p encoder)
{
	assert(encoder != NULL);

	return (rb_used(&encoder->ringbuffer) == SIZE_ZERO);
}

int aud_encoder_getframe(aud_encoder_p encoder, void *data, size_t len)
{
	assert(encoder != NULL);

	priv_data_p priv = (priv_data_p) encoder->priv_data;
	assert(priv != NULL);

	switch (encoder->audio_type) {
#ifdef CONFIG_CODEC_LIBOPUS
	case AUDIO_TYPE_OPUS: {
		opus_enc_external_t *opus_ext = (opus_enc_external_t *) encoder->enc_ext;

		int analysis_frame_size = opus_ext->inputSampleRate * opus_ext->frameSizeMS / 1000;
		if (opus_ext->frameSizeMS == 25)
			analysis_frame_size /= 10;

		size_t size = analysis_frame_size * opus_ext->inputChannels * sizeof(signed short);

		int ret = rbs_seek(encoder->rbsp, priv->mCurrentPos, SEEK_SET);
		RETURN_VAL_IF_FAIL((ret == OK), PV_FAILURE);


		RETURN_VAL_IF_FAIL((size <= (size_t) opus_ext->inputBufferMaxLength), PV_FAILURE);
		size_t rlen = rbs_read((void *) opus_ext->pInputBuffer, 1, size, encoder->rbsp);
		RETURN_VAL_IF_FAIL((rlen == size), PV_FAILURE);
		opus_ext->inputBufferCurrentLength = rlen;

		priv->mCurrentPos += rlen;
		rbs_seek_ext(encoder->rbsp, priv->mCurrentPos, SEEK_SET);

		ret = opus_frameEncode(opus_ext, encoder->enc_mem);
		RETURN_VAL_IF_FAIL((ret == OK), PV_FAILURE);

		RETURN_VAL_IF_FAIL(((int32_t) len >= opus_ext->outputDataSize), PV_FAILURE);
		// TODO: get part of data

		memcpy(data, opus_ext->pOutputBuffer, opus_ext->outputDataSize);
		return opus_ext->outputDataSize;
	}
#endif

	default:
		medwdbg("[%s] unsupported audio type: %d\n", __FUNCTION__, encoder->audio_type);
		return PV_FAILURE;
	}
}

int aud_encoder_init(aud_encoder_p encoder, size_t rbuf_size, audio_type_t audio_type, void *enc_ext)
{
	assert(encoder != NULL);
	RETURN_VAL_IF_FAIL(aud_encoder_check_audio_type(audio_type), PV_FAILURE);

	encoder->audio_type = audio_type;

	// init private data
	priv_data_p priv = (priv_data_p) malloc(sizeof(priv_data_t));
	RETURN_VAL_IF_FAIL((priv != NULL), PV_FAILURE);
	priv->mCurrentPos = 0;
	encoder->priv_data = priv;

	int ret = _init_encoder(encoder, enc_ext);
	RETURN_VAL_IF_FAIL((ret == PV_SUCCESS), PV_FAILURE);

	// init ring-buffer and open it as a stream
	rb_init(&encoder->ringbuffer, rbuf_size);
	encoder->rbsp = rbs_open(&encoder->ringbuffer, _input_callback, (void *)encoder);
	RETURN_VAL_IF_FAIL((encoder->rbsp != NULL), PV_FAILURE);

	rbs_ctrl(encoder->rbsp, OPTION_ALLOW_TO_DEQUEUE, 1);
	return PV_SUCCESS;
}

int aud_encoder_register_callbacks(aud_encoder_p encoder, void *user_data, input_func_f input_func)
{
	// init encoder data
	encoder->cb_data = user_data;
	encoder->input_func = input_func;

	return PV_SUCCESS;
}

int aud_encoder_finish(aud_encoder_p encoder)
{
	assert(encoder != NULL);

	// close stream
	rbs_close(encoder->rbsp);
	encoder->rbsp = NULL;

	// free ring-buffer instance
	rb_free(&encoder->ringbuffer);

	// free encoder external buffer
	if (encoder->enc_ext != NULL) {
		free(encoder->enc_ext);
		encoder->enc_ext = NULL;
	}

	// free encoder buffer
	if (encoder->enc_mem != NULL) {
		#ifdef CONFIG_CODEC_LIBOPUS
		opus_uninitEncoder(encoder->enc_mem);
		#endif
		free(encoder->enc_mem);
		encoder->enc_mem = NULL;
	}

	// free private data buffer
	if (encoder->priv_data != NULL) {
		free(encoder->priv_data);
		encoder->priv_data = NULL;
	}

	return PV_SUCCESS;
}

} // namespace media

