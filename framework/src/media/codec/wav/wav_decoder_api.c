/******************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

#include <debug.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <debug.h>
#include "../../utils/internal_defs.h"
#include "../../utils/rbs.h"
#include "../../audio/resample/samplerate.h"
#include "../audio_decoder.h"
#include "wav_decoder_api.h"


/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define WAVE_HEADER_LENGTH      44
#define WAVE_HDR_CHUNK_ID       "RIFF"
#define WAVE_FORMAT_STR         "WAVE"
#define WAVE_FMT_CHUNK_ID       "fmt "
#define WAVE_DATA_CHUNK_ID      "data"
#define WAVE_FMT_CHUNK_SIZE     16
#define WAVE_FORMAT_PCM         0x0001 // uncompressed PCM
#define WAVE_FORMAT_EXTENSIBLE  0XFFFE // for multi channel

#define DATAFORMAT_SUBTYPE_PCM  subTypePCM
#define BYTES_PER_SAMPLE        sizeof(signed short)

/****************************************************************************
 * Private Declarations
 ****************************************************************************/
struct wave_format_ex_s {
	uint16_t wFormatTag;
	uint16_t nChannels;
	uint32_t nSamplesPerSec;
	uint32_t nAvgBytesPerSec;
	uint16_t nBlockAlign;
	uint16_t wBitsPerSample;
	uint16_t cbSize;
};
typedef struct wave_format_ex_s wave_format_ex_t;
typedef struct wave_format_ex_s *wave_format_ex_p;

struct wave_format_extensible_s {
	wave_format_ex_t tFormat;
	union {
		uint16_t wValidBitsPerSample; /* bits of precision */
		uint16_t wSamplesPerBlock;    /* valid if wBitsPerSample==0 */
		uint16_t wReserved;           /* If neither applies, set to zero. */
	} uSamples;
	uint32_t dwChannelMask;           /* which channels are present in stream */
	uint8_t subFormat[32];            /* GUID */
};
typedef struct wave_format_extensible_s wave_format_extensible_t;
typedef struct wave_format_extensible_s *wave_format_extensible_p;

// GUID for PCM sub-format
static const uint8_t subTypePCM[] = {
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static uint32_t _le_num_at(const uint8_t *ptr, uint8_t bytes)
{
	unsigned int ret = 0;
	for (int i = bytes - 1; i >= 0; i--) {
		ret <<= 8;
		ret |= ptr[i];
	}
	return ret;
}

static ssize_t _source_read_at(rbstream_p fp, ssize_t offset, void *data, size_t size)
{
	int retVal = rbs_seek(fp, offset, SEEK_SET);
	RETURN_VAL_IF_FAIL((retVal == OK), SIZE_ZERO);

	return rbs_read(data, 1, size, fp);
}

static bool _format_parse(void *buffer, uint32_t size, wave_format_extensible_p ext)
{
	RETURN_VAL_IF_FAIL((buffer != NULL || size >= 8 + WAVE_FMT_CHUNK_SIZE), false);
	RETURN_VAL_IF_FAIL((ext != NULL), false);

	RETURN_VAL_IF_FAIL((memcmp(buffer, WAVE_FMT_CHUNK_ID, 4) == OK), false);

	wave_format_ex_p ex = &ext->tFormat;
	uint8_t *fmtChunk = (uint8_t *)buffer;
	uint32_t chunkSize = _le_num_at(&fmtChunk[4], 4);

	ex->wFormatTag = _le_num_at(&fmtChunk[8], 2);
	if ((ex->wFormatTag == WAVE_FORMAT_PCM) && (chunkSize == WAVE_FMT_CHUNK_SIZE)) {
		ex->cbSize = 0; // uncompressed PCM
	} else if (ex->wFormatTag == WAVE_FORMAT_EXTENSIBLE) {
		RETURN_VAL_IF_FAIL((size >= 8 + chunkSize), false);
		ex->cbSize = _le_num_at(&fmtChunk[8 + WAVE_FMT_CHUNK_SIZE], 2);
		RETURN_VAL_IF_FAIL((WAVE_FMT_CHUNK_SIZE + 2u + ex->cbSize <= chunkSize), false);
	} else {
		meddbg("Unsupported format %u, chunkSize %u\n", ex->wFormatTag, chunkSize);
		return false;
	}

	ex->nChannels = _le_num_at(&fmtChunk[10], 2);
	if (ex->nChannels > 6) {
		// we support 5.1 channel at most
		meddbg("Unsupported channels: %u\n", ex->nChannels);
		return false;
	}

	ex->nSamplesPerSec = _le_num_at(&fmtChunk[12], 4);
	ex->nAvgBytesPerSec = _le_num_at(&fmtChunk[16], 4);
	ex->nBlockAlign = _le_num_at(&fmtChunk[20], 2);
	ex->wBitsPerSample = _le_num_at(&fmtChunk[22], 2);
	if (ex->wBitsPerSample != 16) {
		// we support AUDIO_FORMAT_TYPE_S16_LE only
		meddbg("Unsupported bitsPerSample: %u\n", ex->wBitsPerSample);
		return false;
	}

	if (ex->cbSize != 0) {
		// There's extensible params
		uint8_t *extraData = &fmtChunk[8 + WAVE_FMT_CHUNK_SIZE + 2];
		ext->uSamples.wValidBitsPerSample = _le_num_at(&extraData[0], 2);
		if (ext->uSamples.wValidBitsPerSample != 16) {
			// we support AUDIO_FORMAT_TYPE_S16_LE only
			meddbg("Unsupported validBitsPerSample: %u\n", ext->uSamples.wValidBitsPerSample);
			return false;
		}

		ext->dwChannelMask = _le_num_at(&extraData[2], 4);
		RETURN_VAL_IF_FAIL((ex->cbSize <= sizeof(ext->subFormat) + 6), false);
		memcpy((void *)ext->subFormat, (const void *)&extraData[6], ex->cbSize - 6);
		if (memcmp(DATAFORMAT_SUBTYPE_PCM, ext->subFormat, sizeof(DATAFORMAT_SUBTYPE_PCM)) != OK) {
			meddbg("Unsupported sub format...\n");
			return false;
		}
	}

	return true;
}


// Initialize the wav reader.
int wav_init(audio_decoder_p decoder, void *dec_ext)
{
	decoder->dec_ext = calloc(1, sizeof(wav_dec_external_t));
	RETURN_VAL_IF_FAIL((decoder->dec_ext != NULL), AUDIO_DECODER_ERROR);

	*((wav_dec_external_t *)decoder->dec_ext) = *((wav_dec_external_t *)dec_ext);

	decoder->dec_mem = calloc(1, sizeof(wave_format_extensible_t));
	RETURN_VAL_IF_FAIL((decoder->dec_mem != NULL), AUDIO_DECODER_ERROR);
	memset(decoder->dec_mem, 0, sizeof(wave_format_extensible_t));

	return AUDIO_DECODER_OK;
}

bool wav_check_type(rbstream_p rbsp)
{
	uint8_t waveheader[WAVE_HEADER_LENGTH];
	ssize_t rlen = _source_read_at(rbsp, 0, waveheader, sizeof(waveheader));
	RETURN_VAL_IF_FAIL(((size_t)rlen == sizeof(waveheader)), false);
	// check "RIFF" chunk
	RETURN_VAL_IF_FAIL((memcmp(&waveheader[0], WAVE_HDR_CHUNK_ID, 4) == OK), false);
	RETURN_VAL_IF_FAIL((memcmp(&waveheader[8], WAVE_FORMAT_STR, 4) == OK), false);
	// check "fmt" sub-chunk
	RETURN_VAL_IF_FAIL((memcmp(&waveheader[12], WAVE_FMT_CHUNK_ID, 4) == OK), false);
	return true;
}

// Get frames as much as possible according to buffer size.
bool wav_get_frame(rbstream_p mFp, ssize_t *offset, void *dec_mem, void *buffer, uint32_t *size)
{
	wave_format_extensible_p ext = (wave_format_extensible_p)dec_mem;
	ssize_t n;
	ssize_t _offset = *offset;
	if (_offset == 0) {
		if (!wav_check_type(mFp)) {
			meddbg("wav_check_type failed!\n");
			return false;
		}

		_offset = 12; // "fmt" chunk start
		uint8_t sizeBuf[4];
		n = _source_read_at(mFp, _offset + 4, sizeBuf, sizeof(sizeBuf));
		RETURN_VAL_IF_FAIL(((size_t)n == sizeof(sizeBuf)), false);
		uint32_t chunkSize = _le_num_at(&sizeBuf[0], 4);
		uint32_t fmtChunkSize = 8 + chunkSize;
		uint8_t *fmtChunk = (uint8_t *)malloc(sizeof(uint8_t) * fmtChunkSize);
		RETURN_VAL_IF_FAIL(fmtChunk, false);
		n = _source_read_at(mFp, _offset, fmtChunk, fmtChunkSize);
		bool ret_format_parse = _format_parse((void *)fmtChunk, fmtChunkSize, ext);
		free(fmtChunk);
		RETURN_VAL_IF_FAIL((size_t)n == fmtChunkSize, false);
		RETURN_VAL_IF_FAIL(ret_format_parse, false);
		_offset += (8 + chunkSize);

		uint8_t subChunk[8];
		while (1) {
			n = _source_read_at(mFp, _offset, subChunk, sizeof(subChunk));
			RETURN_VAL_IF_FAIL(((size_t)n == sizeof(subChunk)), false);
			_offset += n;
			if (memcmp(&subChunk[0], WAVE_DATA_CHUNK_ID, 4) == OK) {
				// "data" chunk found
				break;
			}

			// Ignore all other extra chunks
			uint32_t subChunkSize = _le_num_at(&subChunk[4], 4);
			_offset += subChunkSize;
		}

		int ret = rbs_seek_ext(mFp, _offset, SEEK_SET);
		RETURN_VAL_IF_FAIL((ret == OK), false);
		*offset = _offset;
	}

	n = _source_read_at(mFp, _offset, buffer, *size);
	RETURN_VAL_IF_FAIL((n > 0), false);

	size_t frames = n / BYTES_PER_SAMPLE / ext->tFormat.nChannels;
	*size = frames * ext->tFormat.nChannels * BYTES_PER_SAMPLE;
	RETURN_VAL_IF_FAIL((*size > 0), false);

	_offset += *size;
	rbs_seek_ext(mFp, _offset, SEEK_SET);
	*offset = _offset;
	return true;
}

int wav_decode_frame(wav_dec_external_p dec_ext, void *dec_mem, src_handle_t *resampler)
{
	wave_format_extensible_p wav_ext = (wave_format_extensible_p)dec_mem;
	wave_format_ex_p wav_fmt_ex = &wav_ext->tFormat;

	// support uncompressed PCM(16-bit) only, just do rechanneling if necessary.
	int32_t nFrames = dec_ext->inputBufferCurrentLength / BYTES_PER_SAMPLE / wav_fmt_ex->nChannels;

	// if user specified a different channel number
	if (wav_fmt_ex->nChannels != dec_ext->desiredChannels) {
		// then process rechanneling
		if (*resampler == NULL) {
			*resampler = src_init(CONFIG_AUDIO_RESAMPLER_BUFSIZE);
			if (*resampler == NULL) {
				meddbg("resampler init failed!\n");
				return AUDIO_DECODER_ERROR;
			}
		}

		src_data_t srcData = { 0, };
		srcData.origin_channel_num = wav_fmt_ex->nChannels;
		srcData.desired_channel_num = dec_ext->desiredChannels;
		srcData.origin_sample_rate = wav_fmt_ex->nSamplesPerSec;
		srcData.desired_sample_rate = wav_fmt_ex->nSamplesPerSec;
		srcData.origin_sample_width = SAMPLE_WIDTH_16BITS;
		srcData.desired_sample_width = SAMPLE_WIDTH_16BITS;
		srcData.data_in = dec_ext->pInputBuffer;
		srcData.input_frames = nFrames;
		srcData.data_out = dec_ext->pOutputBuffer;
		srcData.out_buf_length = dec_ext->outputBufferMaxLength;

		int ret = src_simple(*resampler, &srcData);
		if (ret != SRC_ERR_NO_ERROR) {
			meddbg("Fail to rechannel %d -> %d, err %d\n", wav_fmt_ex->nChannels, dec_ext->desiredChannels, ret);
			return AUDIO_DECODER_ERROR;
		}

		if (srcData.output_frames_gen != srcData.input_frames) {
			meddbg("need to reconfig: inputBufferMaxLength <= outputBufferMaxLength\n");
			return AUDIO_DECODER_ERROR;
		}
	} else {
		memcpy(dec_ext->pOutputBuffer, dec_ext->pInputBuffer, dec_ext->inputBufferCurrentLength);
	}

	dec_ext->outputFrameSize = nFrames;
	dec_ext->samplingRate = wav_fmt_ex->nSamplesPerSec;
	return AUDIO_DECODER_OK;
}
