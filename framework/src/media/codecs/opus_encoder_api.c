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
#include <pthread.h>
#include "opus_encoder_api.h"

// Opus packet header is self-defined, 4 bytes syncword + 4 bytes packet length.
#define OPUS_PACKET_HEADER_LEN 8

#ifdef CONFIG_OPUS_CODEC_PTHREAD
struct opus_enc_service_s {
	uint32_t magic;
	pthread_t thread;
	pthread_mutex_t invoke_mutex;
	pthread_cond_t invoke_cond;

	pthread_mutex_t respond_mutex;
	pthread_cond_t respond_cond;

	OpusEncoder *st;
	opus_enc_external_t *ext;
	int32_t payload_size;
};

typedef struct opus_enc_service_s opus_enc_service_t;
typedef struct opus_enc_service_s *opus_enc_service_p;

#define MAGIC_WORD 0x5a5aa5a5
#define INVALID_THREAD_ID 0

static void *opus_encoder_thread(void *param)
{
	opus_enc_service_p opus_svr = (opus_enc_service_p) param;

	while (1) {
		// Receive encoding request
		pthread_mutex_lock(&opus_svr->invoke_mutex);
		if (opus_svr->ext == NULL) {
			pthread_cond_wait(&opus_svr->invoke_cond, &opus_svr->invoke_mutex);
		}

		if (opus_svr->ext == NULL) {
			// Invalid pointer after condition wakeup, exit thread.
			break;
		}

		medvdbg("opus encoding...\n");
		unsigned char *out_data = ((unsigned char *)opus_svr->ext->pOutputBuffer) + OPUS_PACKET_HEADER_LEN;
		int analysis_frame_size = opus_svr->ext->inputSampleRate * opus_svr->ext->frame_size_ms / 1000;
		if (opus_svr->ext->frame_size_ms == 25) {
			analysis_frame_size /= 10;
		}

		opus_svr->payload_size = opus_encode(opus_svr->st, \
									opus_svr->ext->pInputBuffer, \
									analysis_frame_size, \
									out_data, \
									opus_svr->ext->outputBufferMaxLength);
		opus_svr->st = NULL;
		opus_svr->ext = NULL;
		medvdbg("opus encode end! payload_size %d\n", opus_svr->payload_size);

		pthread_mutex_unlock(&opus_svr->invoke_mutex);

		// Encoding finished, notify result.
		pthread_mutex_lock(&opus_svr->respond_mutex);
		pthread_cond_signal(&opus_svr->respond_cond);
		pthread_mutex_unlock(&opus_svr->respond_mutex);
	}

	return NULL;
}
#endif  // CONFIG_OPUS_CODEC_PTHREAD

static OpusEncoder *getOpusEncoder(void *pMem)
{
#ifdef CONFIG_OPUS_CODEC_PTHREAD
	return ((OpusEncoder *) ((uint8_t *) (pMem) + sizeof(opus_enc_service_t)));
#else
	return ((OpusEncoder *) (pMem));
#endif
}

static void int2char(opus_uint32 i, unsigned char ch[4])
{
	ch[0] = i >> 24;
	ch[1] = (i >> 16) & 0xFF;
	ch[2] = (i >> 8) & 0xFF;
	ch[3] = i & 0xFF;
}

uint32_t opus_encoderMemRequirements(void)
{
	uint32_t size = 0;

#ifdef CONFIG_OPUS_CODEC_PTHREAD
	size += sizeof(opus_enc_service_t);
#endif

	int channels = 2; // max 2
	size += opus_encoder_get_size(channels);

	return size;
}

int32_t opus_initEncoder(opus_enc_external_t *pExt, void *pMem)
{
	OpusEncoder *st = getOpusEncoder(pMem);
	int err = opus_encoder_init(st, pExt->inputSampleRate, pExt->inputChannels, pExt->opus_application);
	if (err != OPUS_OK) {
		meddbg("opus_encoder_init err %d\n", err);
		return err;
	}

	opus_encoder_ctl(st, OPUS_SET_BITRATE(pExt->bitrate_bps));
	opus_encoder_ctl(st, OPUS_SET_BANDWIDTH(pExt->band_width));
	opus_encoder_ctl(st, OPUS_SET_VBR(1));
	opus_encoder_ctl(st, OPUS_SET_VBR_CONSTRAINT(0));
	opus_encoder_ctl(st, OPUS_SET_COMPLEXITY(pExt->complexity));
	opus_encoder_ctl(st, OPUS_SET_INBAND_FEC(0));
	opus_encoder_ctl(st, OPUS_SET_FORCE_CHANNELS(OPUS_AUTO));
	opus_encoder_ctl(st, OPUS_SET_DTX(0));
	opus_encoder_ctl(st, OPUS_SET_PACKET_LOSS_PERC(0));
	opus_encoder_ctl(st, OPUS_SET_SIGNAL(OPUS_SIGNAL_VOICE));
	opus_encoder_ctl(st, OPUS_SET_LSB_DEPTH(16));
	opus_encoder_ctl(st, OPUS_SET_EXPERT_FRAME_DURATION(OPUS_FRAMESIZE_ARG));

#ifdef CONFIG_OPUS_CODEC_PTHREAD
	opus_enc_service_p opus_svr = (opus_enc_service_p) pMem;

	if (opus_svr->magic != MAGIC_WORD || opus_svr->thread == INVALID_THREAD_ID) {
		opus_svr->magic = MAGIC_WORD;
		opus_svr->st = NULL;
		opus_svr->ext = NULL;
		opus_svr->payload_size = 0;

		pthread_mutex_init(&(opus_svr->invoke_mutex), NULL);
		pthread_cond_init(&(opus_svr->invoke_cond), NULL);
		pthread_mutex_init(&(opus_svr->respond_mutex), NULL);
		pthread_cond_init(&(opus_svr->respond_cond), NULL);

		pthread_attr_t attr;
		int status;
		status = pthread_attr_init(&attr);
		if (status != 0) {
			meddbg("thread : pthread_attr_init failed, status=%d\n", status);
		}

		/* Set the stacksize */
		status = pthread_attr_setstacksize(&attr, CONFIG_OPUS_ENCODE_PTHREAD_STACKSIZE);
		if (status != 0) {
			meddbg("thread : pthread_attr_setstacksize failed, status=%d\n", status);
		}

		status = pthread_create(&opus_svr->thread, &attr, opus_encoder_thread, opus_svr);
		if (status != 0) {
			meddbg("thread: pthread_create failed, status=%d\n", status);
			opus_svr->thread = INVALID_THREAD_ID;
			return OPUS_INTERNAL_ERROR;
		}
	}
#endif // CONFIG_OPUS_CODEC_PTHREAD

	return OPUS_OK;
}

int32_t opus_uninitEncoder(void *pMem)
{
#ifdef CONFIG_OPUS_CODEC_PTHREAD
	opus_enc_service_p opus_svr = (opus_enc_service_p) pMem;

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

void opus_resetEncoder(void *pMem)
{
}

int32_t opus_frameEncode(opus_enc_external_t *pExt, void *pMem)
{
	OpusEncoder *st = getOpusEncoder(pMem);
	unsigned char *out_data = ((unsigned char *)pExt->pOutputBuffer) + OPUS_PACKET_HEADER_LEN;
	int analysis_frame_size = pExt->inputSampleRate * pExt->frame_size_ms / 1000;

	if (pExt->frame_size_ms == 25) {
		analysis_frame_size /= 10;
	}

	int32_t payload_size;

#ifdef CONFIG_OPUS_CODEC_PTHREAD
	opus_enc_service_p opus_svr = (opus_enc_service_p) pMem;

	// Request to Encode
	pthread_mutex_lock(&opus_svr->invoke_mutex);

	opus_svr->st = st;
	opus_svr->ext = pExt;
	pthread_cond_signal(&opus_svr->invoke_cond);

	// Lock respond-mutex before unlock invoke-mutex
	pthread_mutex_lock(&opus_svr->respond_mutex);
	pthread_mutex_unlock(&opus_svr->invoke_mutex);

	// Start encoding
	pthread_cond_wait(&opus_svr->respond_cond, &opus_svr->respond_mutex);

	// Encoding finished
	payload_size = opus_svr->payload_size;

	pthread_mutex_unlock(&opus_svr->respond_mutex);
#else
	payload_size = opus_encode(st, pExt->pInputBuffer,
							   analysis_frame_size,
							   out_data,
							   pExt->outputBufferMaxLength);
#endif // CONFIG_OPUS_CODEC_PTHREAD
	int nb_encoded = opus_packet_get_samples_per_frame(out_data, pExt->inputSampleRate)
					 * opus_packet_get_nb_frames(out_data, payload_size);
	int remaining = analysis_frame_size - nb_encoded;

	medvdbg("opus_frameEncode payload_size %d, nb_encoded %d, remaining %d\n", payload_size, nb_encoded, remaining);
	assert(remaining == 0);

	// Sync word
	out_data = (unsigned char *)pExt->pOutputBuffer;
	strncpy(out_data, "Opus", 4);
	out_data += 4;

	// Packet length
	int2char(payload_size, out_data);
	out_data += 4;

	pExt->outputDataSize = OPUS_PACKET_HEADER_LEN + payload_size;
	return OPUS_OK;
}

