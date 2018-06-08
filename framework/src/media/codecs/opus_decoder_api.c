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

#include <opus/opus.h>
#include <debug.h>
#include <assert.h>
#include <stdio.h>
#include <pthread.h>
#include "opus_decoder_api.h"

// Opus packet header is self-defined, 4 bytes syncword + 4 bytes packet length.
#define OPUS_PACKET_HEADER_LEN 8

#ifdef CONFIG_OPUS_CODEC_PTHREAD
struct opus_dec_service_s {
	uint32_t magic;
	pthread_t thread;
	pthread_mutex_t invoke_mutex;
	pthread_cond_t invoke_cond;

	pthread_mutex_t respond_mutex;
	pthread_cond_t respond_cond;

	OpusDecoder *st;
	opus_dec_external_t *ext;
	int32_t frame_size;
};

typedef struct opus_dec_service_s opus_dec_service_t;
typedef struct opus_dec_service_s *opus_dec_service_p;

#define MAGIC_WORD 0x5a5aa5a5
#define INVALID_THREAD_ID 0

static void *opus_decoder_thread(void *param)
{
	opus_dec_service_p opus_svr = (opus_dec_service_p) param;

	while (1) {
		// Receive decoding request
		pthread_mutex_lock(&opus_svr->invoke_mutex);
		if (opus_svr->ext == NULL) {
			pthread_cond_wait(&opus_svr->invoke_cond, &opus_svr->invoke_mutex);
		}

		if (opus_svr->ext == NULL) {
			// Invalid pointer after condition wakeup, exit thread.
			break;
		}

		medvdbg("opus decoding...\n");
		opus_svr->frame_size = opus_decode(opus_svr->st, \
								opus_svr->ext->pInputBuffer + OPUS_PACKET_HEADER_LEN, \
								opus_svr->ext->inputBufferCurrentLength - OPUS_PACKET_HEADER_LEN, \
								opus_svr->ext->pOutputBuffer, \
								opus_svr->ext->outputBufferMaxLength / sizeof(signed short), \
								0);
		opus_svr->st = NULL;
		opus_svr->ext = NULL;
		medvdbg("opus decode end! frame_size %d\n", opus_svr->frame_size);

		pthread_mutex_unlock(&opus_svr->invoke_mutex);

		// Decoding finished, notify result.
		pthread_mutex_lock(&opus_svr->respond_mutex);
		pthread_cond_signal(&opus_svr->respond_cond);
		pthread_mutex_unlock(&opus_svr->respond_mutex);
	}

	return NULL;
}
#endif // CONFIG_OPUS_CODEC_PTHREAD

static OpusDecoder *getOpusDecoder(void *pMem)
{
#ifdef CONFIG_OPUS_CODEC_PTHREAD
	return ((OpusDecoder *) ((uint8_t *) (pMem) + sizeof(opus_dec_service_t)));
#else
	return ((OpusDecoder *) (pMem));
#endif
}

uint32_t opus_decoderMemRequirements(void)
{
	uint32_t size = 0;

#ifdef CONFIG_OPUS_CODEC_PTHREAD
	size += sizeof(opus_dec_service_t);
#endif

	int channels = 2; // max 2
	size += opus_decoder_get_size(channels);

	return size;
}

int32_t opus_initDecoder(opus_dec_external_t *pExt, void *pMem)
{
	OpusDecoder *st = getOpusDecoder(pMem);
	int err = opus_decoder_init(st, pExt->desiredSampleRate, pExt->desiredChannels);
	if (err != OPUS_OK) {
		meddbg("opus_decoder_init err %d\n", err);
		return err;
	}

#ifdef CONFIG_OPUS_CODEC_PTHREAD
	opus_dec_service_p opus_svr = (opus_dec_service_p) pMem;

	if (opus_svr->magic != MAGIC_WORD || opus_svr->thread == INVALID_THREAD_ID) {
		opus_svr->magic = MAGIC_WORD;
		opus_svr->st = NULL;
		opus_svr->ext = NULL;
		opus_svr->frame_size = 0;

		pthread_mutex_init(&(opus_svr->invoke_mutex), NULL);
		pthread_cond_init(&(opus_svr->invoke_cond), NULL);
		pthread_mutex_init(&(opus_svr->respond_mutex), NULL);
		pthread_cond_init(&(opus_svr->respond_cond), NULL);

		pthread_attr_t attr;
		struct sched_param sparam;
		int status;

		status = pthread_attr_init(&attr);
		if (status != 0) {
			meddbg("thread : pthread_attr_init failed, status=%d\n", status);

		}

		sparam.sched_priority = 100;
		status = pthread_attr_setschedparam(&attr, &sparam);
		if (status != 0) {
			meddbg("thread : pthread_attr_setschedparam failed, status=%d\n", status);
		}

		/* Set the stacksize */
		status = pthread_attr_setstacksize(&attr, CONFIG_OPUS_DECODE_PTHREAD_STACKSIZE);
		if (status != 0) {
			meddbg("thread : pthread_attr_setstacksize failed, status=%d\n", status);
		}

		status = pthread_create(&opus_svr->thread, &attr, opus_decoder_thread, opus_svr);
		if (status != 0) {
			meddbg("thread: pthread_create failed, status=%d\n", status);
			opus_svr->thread = INVALID_THREAD_ID;
			return OPUS_INTERNAL_ERROR;
		}
	}
#endif // CONFIG_OPUS_CODEC_PTHREAD

	return OPUS_OK;
}

int32_t opus_uninitDecoder(void *pMem)
{
#ifdef CONFIG_OPUS_CODEC_PTHREAD
	opus_dec_service_p opus_svr = (opus_dec_service_p) pMem;

	if ((opus_svr->magic == MAGIC_WORD) && (opus_svr->thread != INVALID_THREAD_ID)) {
		// Reset ext pointer and notify to exit thread.
		pthread_mutex_lock(&opus_svr->invoke_mutex);
		opus_svr->st = NULL;
		opus_svr->ext = NULL;
		pthread_cond_signal(&opus_svr->invoke_cond);
		pthread_mutex_unlock(&opus_svr->invoke_mutex);

		int status;
		status = pthread_join(opus_svr->thread, NULL);
		if (status != 0) {
			meddbg("thread: pthread_join failed, status=%d\n", status);
		} else {
			opus_svr->thread = INVALID_THREAD_ID;
		}
	}
#endif // CONFIG_OPUS_CODEC_PTHREAD
	return OPUS_OK;
}

void opus_resetDecoder(void *pMem)
{
}

int32_t opus_frameDecode(opus_dec_external_t *pExt, void *pMem)
{
	OpusDecoder *st = getOpusDecoder(pMem);
	int32_t frame_size;

#ifdef CONFIG_OPUS_CODEC_PTHREAD
	opus_dec_service_p opus_svr = (opus_dec_service_p) pMem;

	// Request to decode
	pthread_mutex_lock(&opus_svr->invoke_mutex);

	opus_svr->st = st;
	opus_svr->ext = pExt;
	pthread_cond_signal(&opus_svr->invoke_cond);

	// Lock respond-mutex before unlock invoke-mutex
	pthread_mutex_lock(&opus_svr->respond_mutex);
	pthread_mutex_unlock(&opus_svr->invoke_mutex);

	// Start decoding in thread
	pthread_cond_wait(&opus_svr->respond_cond, &opus_svr->respond_mutex);

	// Decoding finished
	frame_size = opus_svr->frame_size;

	pthread_mutex_unlock(&opus_svr->respond_mutex);
#else
	frame_size = opus_decode(st, \
  							pExt->pInputBuffer + OPUS_PACKET_HEADER_LEN, \
  							pExt->inputBufferCurrentLength - OPUS_PACKET_HEADER_LEN, \
  							pExt->pOutputBuffer, \
  							pExt->outputBufferMaxLength / sizeof(signed short), \
  							0);
#endif // CONFIG_OPUS_CODEC_PTHREAD

	if (frame_size < 0) {
		pExt->outputFrameSize = 0;
		return frame_size;
	}

	pExt->outputFrameSize = frame_size;
	return OPUS_OK;
}

