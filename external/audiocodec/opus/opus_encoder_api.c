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
#include <audiocodec/opus/opus_encoder_api.h>
#include <debug.h>
#include <assert.h>
#include <pthread.h>

// Opus packet header is self-defined, 4 bytes syncword + 4 bytes packet length.
#define OPUS_PACKET_HEADER_LEN 8

#define OPUS_SELF_THREAD

#ifdef OPUS_SELF_THREAD
static pthread_mutex_t invoke_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t invoke_cond = PTHREAD_COND_INITIALIZER;

static pthread_mutex_t respond_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t respond_cond = PTHREAD_COND_INITIALIZER;

static OpusEncoder *sg_st = NULL;
static opus_enc_external_t *sg_pExt = NULL;
static int32_t sg_payload_size = 0;

static void *opus_encoder_thread(void *param)
{
	while (1) {
		// Receive encoding request
		pthread_mutex_lock(&invoke_mutex);
		if (sg_pExt == NULL) {
			pthread_cond_wait(&invoke_cond, &invoke_mutex);
		}

		medvdbg("opus encoding...\n");
		unsigned char *out_data = ((unsigned char *)sg_pExt->pOutputBuffer) + OPUS_PACKET_HEADER_LEN;
		int analysis_frame_size = sg_pExt->inputSampleRate * sg_pExt->frame_size_ms / 1000;
		if (sg_pExt->frame_size_ms == 25) {
			analysis_frame_size /= 10;
		}

		sg_payload_size = opus_encode(sg_st, \
									sg_pExt->pInputBuffer, \
									analysis_frame_size, \
									out_data, \
									sg_pExt->outputBufferMaxLength);
		sg_st = NULL;
		sg_pExt = NULL;
		medvdbg("opus encode end! payload_size %d\n", sg_payload_size);

		pthread_mutex_unlock(&invoke_mutex);

		// Encoding finished, notify result.
		pthread_mutex_lock(&respond_mutex);
		pthread_cond_signal(&respond_cond);
		pthread_mutex_unlock(&respond_mutex);
	}

	return NULL;
}

#endif  // OPUS_SELF_THREAD

static void int_to_char(opus_uint32 i, unsigned char ch[4])
{
	ch[0] = i >> 24;
	ch[1] = (i >> 16) & 0xFF;
	ch[2] = (i >> 8) & 0xFF;
	ch[3] = i & 0xFF;
}

uint32_t opus_encoderMemRequirements(void)
{
	int channels = 2; // max 2
	return opus_encoder_get_size(channels);
}

int32_t opus_initEncoder(opus_enc_external_t *pExt, void *pMem)
{
	OpusEncoder *st = (OpusEncoder *) pMem;
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

#ifdef OPUS_SELF_THREAD
	sg_st = NULL;
	sg_pExt = NULL;

	pthread_t thread;
	pthread_attr_t attr;
	int status;
	status = pthread_attr_init(&attr);
	if (status != 0) {
		meddbg("thread : pthread_attr_init failed, status=%d\n", status);
	}

	/* Set the stacksize */
	status = pthread_attr_setstacksize(&attr, 48 * 1024);
	if (status != 0) {
		meddbg("thread : pthread_attr_setstacksize failed, status=%d\n", status);
	}

	status = pthread_create(&thread, &attr, opus_encoder_thread, (void *)NULL);
	if (status != 0) {
		meddbg("thread: pthread_create failed, status=%d\n", status);
	}
#endif // OPUS_SELF_THREAD

	return OPUS_OK;
}


void opus_resetEncoder(void *pMem)
{
}

int32_t opus_frameEncode(opus_enc_external_t *pExt, void *pMem)
{
	OpusEncoder *st = (OpusEncoder *) pMem;
	unsigned char *out_data = ((unsigned char *)pExt->pOutputBuffer) + OPUS_PACKET_HEADER_LEN;
	int analysis_frame_size = pExt->inputSampleRate * pExt->frame_size_ms / 1000;

	if (pExt->frame_size_ms == 25) {
		analysis_frame_size /= 10;
	}

	int32_t payload_size;

#ifdef OPUS_SELF_THREAD
	// Request to Encode
	pthread_mutex_lock(&invoke_mutex);

	sg_st = st;
	sg_pExt = pExt;
	pthread_cond_signal(&invoke_cond);

	// Lock respond-mutex before unlock invoke-mutex
	pthread_mutex_lock(&respond_mutex);
	pthread_mutex_unlock(&invoke_mutex);

	// Start encoding
	pthread_cond_wait(&respond_cond, &respond_mutex);

	// Encoding finished
	payload_size = sg_payload_size;

	pthread_mutex_unlock(&respond_mutex);
#else
	payload_size = opus_encode(st, pExt->pInputBuffer,
							   analysis_frame_size,
							   out_data,
							   pExt->outputBufferMaxLength);
#endif // OPUS_SELF_THREAD
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
	int_to_char(payload_size, out_data);
	out_data += 4;

	pExt->outputDataSize = OPUS_PACKET_HEADER_LEN + payload_size;
	return OPUS_OK;
}

