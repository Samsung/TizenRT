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

// Support 2 channels (stereo) at most
#define CHANNEL_NUM_MAX 2

static OpusDecoder *getOpusDecoder(void *pMem)
{
	return ((OpusDecoder *) (pMem));
}

uint32_t opus_decoderMemRequirements(void)
{
	return opus_decoder_get_size(CHANNEL_NUM_MAX);
}

int32_t opus_initDecoder(opus_dec_external_t *pExt, void *pMem)
{
	OpusDecoder *st = getOpusDecoder(pMem);
	int err = opus_decoder_init(st, pExt->desiredSampleRate, pExt->desiredChannels);
	if (err != OPUS_OK) {
		meddbg("opus_decoder_init err %d\n", err);
		return err;
	}

	return OPUS_OK;
}

int32_t opus_uninitDecoder(void *pMem)
{
	return OPUS_OK;
}

void opus_resetDecoder(void *pMem)
{
}

int32_t opus_frameDecode(opus_dec_external_t *pExt, void *pMem)
{
	OpusDecoder *st = getOpusDecoder(pMem);
	int32_t frame_size;

	frame_size = opus_decode(st, \
  							pExt->pInputBuffer + OPUS_PACKET_HEADER_LEN, \
  							pExt->inputBufferCurrentLength - OPUS_PACKET_HEADER_LEN, \
  							pExt->pOutputBuffer, \
  							pExt->outputBufferMaxLength / sizeof(signed short), \
  							0);

	if (frame_size < 0) {
		pExt->outputFrameSize = 0;
		return frame_size;
	}

	pExt->outputFrameSize = frame_size;
	return OPUS_OK;
}

