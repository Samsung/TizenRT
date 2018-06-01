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
#include <audiocodec/opus/opus_decoder_api.h>
#include <debug.h>
#include <assert.h>
#include <stdio.h>
#include <pthread.h>

#define OPUS_PACKET_HEADER_LEN 8	/* 4 bytes SYNC + 4 bytes packet length */

#define OPUS_SELF_THREAD

#ifdef OPUS_SELF_THREAD
static pthread_mutex_t invoke_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t invoke_cond = PTHREAD_COND_INITIALIZER;

static pthread_mutex_t respond_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t respond_cond = PTHREAD_COND_INITIALIZER;

static OpusDecoder *sg_st = NULL;
static opus_dec_external_t *sg_pExt = NULL;
static int32_t sg_frame_size = 0;

static void *opus_decoder_thread(void *param)
{
	while (1) {
		// Receive decoding request
		pthread_mutex_lock(&invoke_mutex);
		if (sg_pExt == NULL) {
			pthread_cond_wait(&invoke_cond, &invoke_mutex);
		}

		medvdbg("opus decoding...\n");
		sg_frame_size = opus_decode(sg_st, \
									sg_pExt->pInputBuffer + OPUS_PACKET_HEADER_LEN, \
									sg_pExt->inputBufferCurrentLength - OPUS_PACKET_HEADER_LEN, \
									sg_pExt->pOutputBuffer, \
									sg_pExt->outputFrameSize, 0);
		sg_st = NULL;
		sg_pExt = NULL;
		medvdbg("opus decode end!\n");

		pthread_mutex_unlock(&invoke_mutex);

		// Decoding finished, notify result.
		pthread_mutex_lock(&respond_mutex);
		pthread_cond_signal(&respond_cond);
		pthread_mutex_unlock(&respond_mutex);
	}

	return NULL;
}

#endif // OPUS_SELF_THREAD

uint32_t opus_decoderMemRequirements(void)
{
	int channels = 2; // max 2
	return opus_decoder_get_size(channels);
}

int32_t opus_initDecoder(opus_dec_external_t *pExt, void *pMem)
{
	OpusDecoder *st = (OpusDecoder *) pMem;
	int err = opus_decoder_init(st, pExt->desiredSampleRate, pExt->desiredChannels);
	if (err != OPUS_OK) {
		meddbg("opus_decoder_init err %d\n", err);
		return err;
	}

#ifdef OPUS_SELF_THREAD
	sg_st = NULL;
	sg_pExt = NULL;

	pthread_t thread;
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
	status = pthread_attr_setstacksize(&attr, 48 * 1024);
	if (status != 0) {
		meddbg("thread : pthread_attr_setstacksize failed, status=%d\n", status);
	}

	status = pthread_create(&thread, &attr, opus_decoder_thread, (void *)NULL);
	if (status != 0) {
		meddbg("thread: pthread_create failed, status=%d\n", status);

	}
#endif // OPUS_SELF_THREAD

	return OPUS_OK;
}

void opus_resetDecoder(void *pMem)
{
}

int32_t opus_frameDecode(opus_dec_external_t *pExt, void *pMem)
{
	OpusDecoder *st = (OpusDecoder *) pMem;
	int32_t frame_size;

#ifdef OPUS_SELF_THREAD
	// Request to decode
	pthread_mutex_lock(&invoke_mutex);

	sg_st = st;
	sg_pExt = pExt;
	pthread_cond_signal(&invoke_cond);

	// Lock respond-mutex before unlock invoke-mutex
	pthread_mutex_lock(&respond_mutex);
	pthread_mutex_unlock(&invoke_mutex);

	// Start decoding in thread
	pthread_cond_wait(&respond_cond, &respond_mutex);

	// Decoding finished
	frame_size = sg_frame_size;

	pthread_mutex_unlock(&respond_mutex);
#else
	frame_size = opus_decode(st, \
  							pExt->pInputBuffer + OPUS_PACKET_HEADER_LEN, \
  							pExt->inputBufferCurrentLength - OPUS_PACKET_HEADER_LEN, \
  							pExt->pOutputBuffer, \
  							pExt->outputFrameSize, 0);
#endif // OPUS_SELF_THREAD

	if (frame_size < 0) {
		pExt->outputFrameSize = 0;
		return frame_size;
	}

	pExt->outputFrameSize = frame_size;
	return OPUS_OK;
}

