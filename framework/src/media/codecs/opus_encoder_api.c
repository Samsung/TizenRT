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
#include <string.h>
#include <debug.h>
#include <assert.h>
#include <pthread.h>
#include "opus_encoder_api.h"

// Opus packet header is self-defined, 4 bytes syncword + 4 bytes packet length.
#define OPUS_PACKET_HEADER_LEN 8

// Support 2 channels (stereo) at most
#define CHANNEL_NUM_MAX 2


static OpusEncoder *getOpusEncoder(void *pMem)
{
	return ((OpusEncoder *) (pMem));
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
	return opus_encoder_get_size(CHANNEL_NUM_MAX);
}

int32_t opus_initEncoder(opus_enc_external_t *pExt, void *pMem)
{
	OpusEncoder *st = getOpusEncoder(pMem);
	int err = opus_encoder_init(st, pExt->inputSampleRate, pExt->inputChannels, pExt->applicationMode);
	if (err != OPUS_OK) {
		meddbg("opus_encoder_init err %d\n", err);
		return err;
	}

	opus_encoder_ctl(st, OPUS_SET_BITRATE(pExt->bitrate));
	opus_encoder_ctl(st, OPUS_SET_BANDWIDTH(pExt->bandWidth));
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

	return OPUS_OK;
}

int32_t opus_uninitEncoder(void *pMem)
{
	return OPUS_OK;
}

void opus_resetEncoder(void *pMem)
{
}

int32_t opus_frameEncode(opus_enc_external_t *pExt, void *pMem)
{
	OpusEncoder *st = getOpusEncoder(pMem);
	unsigned char *out_data = ((unsigned char *)pExt->pOutputBuffer) + OPUS_PACKET_HEADER_LEN;
	int analysis_frame_size = pExt->inputSampleRate * pExt->frameSizeMS / 1000;
	int32_t payload_size;

	payload_size = opus_encode(st, pExt->pInputBuffer,
							   analysis_frame_size,
							   out_data,
							   pExt->outputBufferMaxLength);

	int nb_encoded = opus_packet_get_samples_per_frame(out_data, pExt->inputSampleRate)
					 * opus_packet_get_nb_frames(out_data, payload_size);
	int remaining = analysis_frame_size - nb_encoded;

	medvdbg("opus_frameEncode payload_size %d, nb_encoded %d, remaining %d\n", payload_size, nb_encoded, remaining);
	assert(remaining == 0);

	// Sync word
	out_data = (unsigned char *)pExt->pOutputBuffer;
	strncpy((char *)out_data, "Opus", 4);
	out_data += 4;

	// Packet length
	int2char(payload_size, out_data);
	out_data += 4;

	pExt->outputDataSize = OPUS_PACKET_HEADER_LEN + payload_size;
	return OPUS_OK;
}

