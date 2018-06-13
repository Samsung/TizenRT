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
#include "player.h"
#include "../utils/internal_defs.h"

namespace media {

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define PV_SUCCESS OK
#define PV_FAILURE ERROR

// MP3 tag frame header len
#define MP3_HEAD_ID3_TAG_LEN 10

// MP3 ID3v2 frame len, 6th~9th bytes in header
#define MP3_HEAD_ID3_FRAME_GETSIZE(buf)  (((buf[6] & 0x7f) << 21) \
										| ((buf[7] & 0x7f) << 14) \
										| ((buf[8] & 0x7f) << 7) \
										| ((buf[9] & 0x7f)))

// Mask to verfiy the MP3 header, all bits should be '1' for all MP3 frames.
#define MP3_FRAME_VERIFY_MASK 0xffe00000

// Mask to extract the version, layer, sampling rate parts of the MP3 header,
// which should be same for all MP3 frames.
#define MP3_FRAME_HEADER_MASK 0xfffe0c00

// AAC ADIF header sync data and len
#define AAC_ADIF_SYNC_DATA "ADIF"
#define AAC_ADIF_SYNC_LEN  4

// AAC ADTS frame header len
#define AAC_ADTS_FRAME_HEADER_LEN 9

// AAC ADTS frame sync verify
#define AAC_ADTS_SYNC_VERIFY(buf) ((buf[0] == 0xff) && ((buf[1] & 0xf6) == 0xf0))

// AAC ADTS Frame size value stores in 13 bits started at the 31th bit from header
#define AAC_ADTS_FRAME_GETSIZE(buf) ((buf[3] & 0x03) << 11 | buf[4] << 3 | buf[5] >> 5)

// Read bytes each time from stream each time, while frame resyn.
#define FRAME_RESYNC_READ_BYTES      (1024)
// Max bytes could be read from stream in total, while frame resyn.
#define FRAME_RESYNC_MAX_CHECK_BYTES (8 * 1024)

// MPEG version
#define MPEG_VERSION_1         3
#define MPEG_VERSION_2         2
#define MPEG_VERSION_UNDEFINED 1
#define MPEG_VERSION_2_5       0
#define MP3_FRAME_GET_MPEG_VERSION(header) (((header) >> 19) & 0x3)

// MPEG layer
#define MPEG_LAYER_1         3
#define MPEG_LAYER_2         2
#define MPEG_LAYER_3         1
#define MPEG_LAYER_UNDEFINED 0
#define MP3_FRAME_GET_MPEG_LAYER(header) (((header) >> 17) & 0x3)

// Bitrate index
#define BITRATE_IDX_FREE 0x0  // Variable Bit Rate
#define BITRATE_IDX_BAD  0xf  // Not allow
#define MP3_FRAME_GET_BITRATE_IDX(header) (((header) >> 12) & 0xf)

// Sample Rate index
#define SAMPLE_RATE_IDX_UNDEFINED 0x3
#define MP3_FRAME_GET_SR_IDX(header) (((header) >> 10) & 0x3)

// Padding flag
#define MP3_FRAME_GET_PADDING(header) ((header >> 9) & 0x1)

// Frame size = frame samples * (1 / sample rate) * bitrate / 8 + padding
//            = frame samples * bitrate / 8 / sample rate + padding
// Number of frame samples is constant value as below table:
//        MPEG1  MPEG2(LSF)  MPEG2.5(LSF)
// Layer1  384     384         384
// Layer2  1152    1152        1152
// Layer3  1152    576         576
#define MPEG_LAYER1_FRAME_SIZE(sr, br, pad)  (384 * ((br) * 1000) / 8 / (sr) + ((pad) * 4))
#define MPEG_LAYER2_FRAME_SIZE(sr, br, pad)  (1152 * ((br) * 1000) / 8 / (sr) + (pad))
#define MPEG1_LAYER2_LAYER3_FRAME_SIZE(sr, br, pad) MPEG_LAYER2_FRAME_SIZE(sr, br, pad)
#define MPEG2_LAYER3_FRAME_SIZE(sr, br, pad) (576 * ((br) * 1000) / 8 / (sr) + (pad))

// Frame Resync, match more frame headers for confirming.
#define FRAME_MATCH_REQUIRED 2

#define U32_LEN_IN_BYTES (sizeof(uint32_t) / sizeof(uint8_t))

// Opus packet header is self-defined, 4 bytes syncword + 4 bytes packet length.
#define OPUS_PACKET_HEADER_LEN 8

// Opus packet sync verify
#define OPUS_PACKET_SYNC_VERIFY(buf) (strncmp((const char *)buf, "Opus", 4) == 0)
#define OPUS_PACKET_GETSIZE(buf) (OPUS_PACKET_HEADER_LEN + _u32_at(buf+4))

#define BYTES_PER_SAMPLE sizeof(signed short)

/****************************************************************************
 * Private Declarations
 ****************************************************************************/

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

/**
 * @struct  priv_data_s
 * @brief   Player private data structure define.
 */
struct priv_data_s {
	ssize_t mCurrentPos;        /* read position when decoding */
	uint32_t mFixedHeader;      /* mp3 frame header */
	pcm_data_t pcm;             /* a recorder of pcm data info */
};

typedef struct priv_data_s priv_data_t;
typedef struct priv_data_s *priv_data_p;

// Sample Rate(in Hz) tables
static const int kSamplingRateV1[] = {
	44100, 48000, 32000
};

static const int kSamplingRateV2[] = {
	22050, 24000, 16000
};

static const int kSamplingRateV2_5[] = {
	11025, 12000, 8000
};

// Bit Rate (in kbps) tables
// V1 - MPEG 1, V2 - MPEG 2 and MPEG 2.5
// L1 - Layer 1, L2 - Layer 2, L3 - Layer 3
static const int kBitrateV1L1[] = {
	32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448
};

static const int kBitrateV2L1[] = {
	32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256
};

static const int kBitrateV1L2[] = {
	32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384
};

static const int kBitrateV1L3[] = {
	32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320
};

static const int kBitrateV2L3[] = {
	8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static uint32_t _u32_at(const uint8_t *ptr)
{
	return ptr[0] << 24 | ptr[1] << 16 | ptr[2] << 8 | ptr[3];
}

static bool _parse_header(uint32_t header, size_t *frame_size)
{
	*frame_size = 0;

	RETURN_VAL_IF_FAIL((header & MP3_FRAME_VERIFY_MASK) == MP3_FRAME_VERIFY_MASK, false);

	unsigned version = MP3_FRAME_GET_MPEG_VERSION(header);
	RETURN_VAL_IF_FAIL(version != MPEG_VERSION_UNDEFINED, false);

	unsigned layer = MP3_FRAME_GET_MPEG_LAYER(header);
	RETURN_VAL_IF_FAIL(layer != MPEG_LAYER_UNDEFINED, false);

	unsigned bitrate_index = MP3_FRAME_GET_BITRATE_IDX(header);
	RETURN_VAL_IF_FAIL((bitrate_index != BITRATE_IDX_FREE), false);
	RETURN_VAL_IF_FAIL((bitrate_index != BITRATE_IDX_BAD), false);

	unsigned sampling_rate_index = MP3_FRAME_GET_SR_IDX(header);
	RETURN_VAL_IF_FAIL((sampling_rate_index != SAMPLE_RATE_IDX_UNDEFINED), false);

	int sampling_rate;
	if (version == MPEG_VERSION_1)
		sampling_rate = kSamplingRateV1[sampling_rate_index];
	else if (version == MPEG_VERSION_2) {
		sampling_rate = kSamplingRateV2[sampling_rate_index];
	} else { // MPEG_VERSION_2_5
		sampling_rate = kSamplingRateV2_5[sampling_rate_index];
	}

	unsigned padding = MP3_FRAME_GET_PADDING(header);

	if (layer == MPEG_LAYER_1) {
		int bitrate = (version == MPEG_VERSION_1)
					  ? kBitrateV1L1[bitrate_index - 1]
					  : kBitrateV2L1[bitrate_index - 1];
		*frame_size = MPEG_LAYER1_FRAME_SIZE(sampling_rate, bitrate, padding);
	} else {
		int bitrate;
		if (version == MPEG_VERSION_1) {
			bitrate = (layer == MPEG_LAYER_2)
					  ? kBitrateV1L2[bitrate_index - 1]
					  : kBitrateV1L3[bitrate_index - 1];
			*frame_size = MPEG1_LAYER2_LAYER3_FRAME_SIZE(sampling_rate, bitrate, padding);
		} else {
			bitrate = kBitrateV2L3[bitrate_index - 1];
			if (layer == MPEG_LAYER_3) {
				*frame_size = MPEG2_LAYER3_FRAME_SIZE(sampling_rate, bitrate, padding);
			} else {
				*frame_size = MPEG_LAYER2_FRAME_SIZE(sampling_rate, bitrate, padding);
			}
		}
	}

	return true;
}

static ssize_t _source_read_at(rbstream_p fp, ssize_t offset, void *data, size_t size)
{
	int retVal = rbs_seek(fp, offset, SEEK_SET);
	RETURN_VAL_IF_FAIL((retVal == OK), SIZE_ZERO);

	return rbs_read(data, 1, size, fp);
}

// Resync to next valid MP3 frame in the file.
static bool mp3_resync(rbstream_p fp, uint32_t match_header, ssize_t *inout_pos, uint32_t *out_header)
{
	medvdbg("[%s] Line %d, match_header %#x, *pos %d\n", __FUNCTION__, __LINE__, match_header, *inout_pos);

	if (*inout_pos == 0) {
		// Skip an optional ID3 header if syncing at the very beginning of the datasource.
		for (;;) {
			uint8_t id3header[MP3_HEAD_ID3_TAG_LEN];
			int retVal = _source_read_at(fp, *inout_pos, id3header, sizeof(id3header));
			RETURN_VAL_IF_FAIL((retVal == (ssize_t) sizeof(id3header)), false);

			if (memcmp("ID3", id3header, 3)) {
				break;
			}
			// Skip the ID3v2 header.
			size_t len = MP3_HEAD_ID3_FRAME_GETSIZE(id3header);
			len += MP3_HEAD_ID3_TAG_LEN;
			*inout_pos += len;
		}
	}

	ssize_t pos = *inout_pos;
	bool valid = false;
	uint8_t buf[FRAME_RESYNC_READ_BYTES];
	ssize_t bytesToRead = FRAME_RESYNC_READ_BYTES;
	ssize_t totalBytesRead = 0;
	ssize_t remainingBytes = 0;
	bool reachEOS = false;
	uint8_t *buf_ptr = buf;

	do {
		if (pos >= *inout_pos + FRAME_RESYNC_MAX_CHECK_BYTES) {
			medvdbg("[%s] resync range < %d\n", __FUNCTION__, FRAME_RESYNC_MAX_CHECK_BYTES);
			break;
		}

		if (remainingBytes < (ssize_t)U32_LEN_IN_BYTES) {
			if (reachEOS) {
				break;
			}

			memcpy(buf, buf_ptr, remainingBytes);
			bytesToRead = FRAME_RESYNC_READ_BYTES - remainingBytes;

			/*
			 * The next read position should start from the end of
			 * the last buffer, and thus should include the remaining
			 * bytes in the buffer.
			 */
			totalBytesRead = _source_read_at(fp, pos + remainingBytes, buf + remainingBytes, bytesToRead);

			if (totalBytesRead <= 0) {
				break;
			}

			reachEOS = (totalBytesRead != bytesToRead);
			remainingBytes += totalBytesRead;
			buf_ptr = buf;
			continue;
		}

		uint32_t header = _u32_at(buf_ptr);

		if (match_header != 0 && (header & MP3_FRAME_HEADER_MASK) != (match_header & MP3_FRAME_HEADER_MASK)) {
			++pos;
			++buf_ptr;
			--remainingBytes;
			continue;
		}

		size_t frame_size;
		if (!_parse_header(header, &frame_size)) {
			++pos;
			++buf_ptr;
			--remainingBytes;
			continue;
		}

		// We found what looks like a valid frame,
		// now find its successors.
		valid = true;
		ssize_t test_pos = pos + frame_size;
		medvdbg("[%s] Line %d, valid frame at pos %#x + framesize %#x = %#x\n", __FUNCTION__, __LINE__, pos, frame_size, test_pos);
		int j;
		for (j = 0; j < FRAME_MATCH_REQUIRED; ++j) {
			uint8_t temp[U32_LEN_IN_BYTES];
			ssize_t retval = _source_read_at(fp, test_pos, temp, sizeof(temp));
			if (retval < (ssize_t) sizeof(temp)) {
				valid = false;
				break;
			}

			uint32_t test_header = _u32_at(temp);

			if ((test_header & MP3_FRAME_HEADER_MASK) != (header & MP3_FRAME_HEADER_MASK)) {
				medvdbg("[%s] Line %d, invalid frame at pos1 %#x\n", __FUNCTION__, __LINE__, test_pos);
				valid = false;
				break;
			}

			size_t test_frame_size;
			if (!_parse_header(test_header, &test_frame_size)) {
				medvdbg("[%s] Line %d, invalid frame at pos2 %#x\n", __FUNCTION__, __LINE__, test_pos);
				valid = false;
				break;
			}

			medvdbg("[%s] Line %d, valid frame at pos %#x + framesize %#x = %#x\n", __FUNCTION__, __LINE__, test_pos, test_frame_size, test_pos + test_frame_size);
			test_pos += test_frame_size;
		}

		if (valid) {
			*inout_pos = pos;

			if (out_header != NULL) {
				*out_header = header;
			}

			medvdbg("[%s] Line %d, find header %#x at pos %d(%#x)\n", __FUNCTION__, __LINE__, header, pos, pos);
		}

		++pos;
		++buf_ptr;
		--remainingBytes;
	} while (!valid);

	return valid;
}

// Initialize the MP3 reader.
bool mp3_init(rbstream_p mFp, ssize_t *offset, uint32_t *header)
{
	// Sync to the first valid frame.
	bool success = mp3_resync(mFp, 0, offset, header);
	RETURN_VAL_IF_FAIL((success == true), false);

	// Policy: Pop out data when *offset updated!
	rbs_seek_ext(mFp, *offset, SEEK_SET);

	size_t frame_size;
	return _parse_header(*header, &frame_size);
}

// Get the next valid MP3 frame.
bool mp3_get_frame(rbstream_p mFp, ssize_t *offset, uint32_t fixed_header, void *buffer, uint32_t *size)
{
	size_t frame_size;

	for (;;) {
		ssize_t n = _source_read_at(mFp, *offset, buffer, U32_LEN_IN_BYTES);
		RETURN_VAL_IF_FAIL((n == U32_LEN_IN_BYTES), false);

		uint32_t header = _u32_at((const uint8_t *)buffer);

		if ((header & MP3_FRAME_HEADER_MASK) == (fixed_header & MP3_FRAME_HEADER_MASK)
			&& _parse_header(header, &frame_size)) {
			break;
		}

		// Lost sync.
		ssize_t pos = *offset;
		if (!mp3_resync(mFp, fixed_header, &pos, NULL /*out_header */)) {
			// Unable to mp3_resync. Signalling end of stream.
			return false;
		}

		*offset = pos;
		// Policy: Pop out data when mCurrentPos updated!
		rbs_seek_ext(mFp, *offset, SEEK_SET);

		// Try again with the new position.
	}

	ssize_t n = _source_read_at(mFp, *offset, buffer, frame_size);
	RETURN_VAL_IF_FAIL((n == (ssize_t) frame_size), false);

	medvdbg("[%s] Line %d, pos %#x, framesize %#x\n", __FUNCTION__, __LINE__, *offset, frame_size);

	*size = frame_size;
	*offset += frame_size;
	// Policy: Pop out data when mCurrentPos updated!
	rbs_seek_ext(mFp, *offset, SEEK_SET);

	return true;
}

bool mp3_check_type(rbstream_p rbsp)
{
	bool result = false;
	uint8_t id3header[MP3_HEAD_ID3_TAG_LEN];

	int retVal = _source_read_at(rbsp, 0, id3header, sizeof(id3header));
	RETURN_VAL_IF_FAIL((retVal == (ssize_t) sizeof(id3header)), false);

	if (memcmp("ID3", id3header, 3) == OK) {
		return true;
	}

	int value = rbs_ctrl(rbsp, OPTION_ALLOW_TO_DEQUEUE, 0);
	ssize_t pos = 0;
	result = mp3_resync(rbsp, 0, &pos, NULL);
	rbs_ctrl(rbsp, OPTION_ALLOW_TO_DEQUEUE, value);

	return result;
}

// Resync to next valid MP3 frame in the file.
static bool aac_resync(rbstream_p fp, ssize_t *inout_pos)
{
	ssize_t pos = *inout_pos;
	bool valid = false;

	uint8_t buf[FRAME_RESYNC_READ_BYTES];
	ssize_t bytesToRead = FRAME_RESYNC_READ_BYTES;
	ssize_t totalBytesRead = 0;
	ssize_t remainingBytes = 0;
	bool reachEOS = false;
	uint8_t *buf_ptr = buf;

	do {
		if (pos >= *inout_pos + FRAME_RESYNC_MAX_CHECK_BYTES) {
			break;
		}

		if (remainingBytes < AAC_ADTS_FRAME_HEADER_LEN) {
			if (reachEOS) {
				break;
			}

			memcpy(buf, buf_ptr, remainingBytes);
			bytesToRead = FRAME_RESYNC_READ_BYTES - remainingBytes;

			/*
			 * The next read position should start from the end of
			 * the last buffer, and thus should include the remaining
			 * bytes in the buffer.
			 */
			totalBytesRead = _source_read_at(fp, pos + remainingBytes, buf + remainingBytes, bytesToRead);
			if (totalBytesRead <= 0) {
				break;
			}

			reachEOS = (totalBytesRead != bytesToRead);
			remainingBytes += totalBytesRead;
			buf_ptr = buf;
			continue;
		}

		if (!AAC_ADTS_SYNC_VERIFY(buf_ptr)) {
			++pos;
			++buf_ptr;
			--remainingBytes;
			continue;
		}

		// We found what looks like a valid frame,
		// now find its successors.
		valid = true;
		int frame_size = AAC_ADTS_FRAME_GETSIZE(buf_ptr);
		ssize_t test_pos = pos + frame_size;
		int j;
		for (j = 0; j < FRAME_MATCH_REQUIRED; ++j) {
			uint8_t temp[AAC_ADTS_FRAME_HEADER_LEN];
			ssize_t retval = _source_read_at(fp, test_pos, temp, sizeof(temp));
			if (retval < (ssize_t) sizeof(temp)) {
				valid = false;
				break;
			}

			if (!AAC_ADTS_SYNC_VERIFY(temp)) {
				valid = false;
				break;
			}

			int test_frame_size = AAC_ADTS_FRAME_GETSIZE(temp);
			test_pos += test_frame_size;
		}

		if (valid) {
			*inout_pos = pos;
		}

		++pos;
		++buf_ptr;
		--remainingBytes;
	} while (!valid);

	return valid;
}

// Initialize the aac reader.
bool aac_init(rbstream_p mFp, ssize_t *offset)
{
	// Sync to the first valid frame.
	bool success = aac_resync(mFp, offset);
	RETURN_VAL_IF_FAIL((success == true), false);

	// Policy: Pop out data when *offset updated!
	rbs_seek_ext(mFp, *offset, SEEK_SET);
	return true;
}

// Get the next valid aac frame.
bool aac_get_frame(rbstream_p mFp, ssize_t *offset, void *buffer, uint32_t *size)
{
	size_t frame_size = 0;
	uint8_t *buf = (uint8_t *) buffer;

	for (;;) {
		ssize_t n = _source_read_at(mFp, *offset, buffer, AAC_ADTS_FRAME_HEADER_LEN);
		RETURN_VAL_IF_FAIL((n == AAC_ADTS_FRAME_HEADER_LEN), false);

		if (AAC_ADTS_SYNC_VERIFY(buf)) {
			frame_size = AAC_ADTS_FRAME_GETSIZE(buf);
			break;
		}

		// Lost sync.
		ssize_t pos = *offset;
		RETURN_VAL_IF_FAIL(aac_resync(mFp, &pos), false);

		*offset = pos;
		rbs_seek_ext(mFp, *offset, SEEK_SET);
		// Try again with the new position.
	}

	ssize_t n = _source_read_at(mFp, *offset, buffer, frame_size);
	RETURN_VAL_IF_FAIL((n == (ssize_t) frame_size), false);

	*size = frame_size;
	*offset += frame_size;
	rbs_seek_ext(mFp, *offset, SEEK_SET);

	return true;
}

bool aac_check_type(rbstream_p rbsp)
{
	bool result = false;
	uint8_t syncword[AAC_ADIF_SYNC_LEN];

	ssize_t rlen = _source_read_at(rbsp, 0, syncword, sizeof(syncword));
	RETURN_VAL_IF_FAIL((rlen == (ssize_t) sizeof(syncword)), false);

	// Don't support ADIF
	RETURN_VAL_IF_FAIL((memcmp(AAC_ADIF_SYNC_DATA, syncword, AAC_ADIF_SYNC_LEN) != OK), false);

	int value = rbs_ctrl(rbsp, OPTION_ALLOW_TO_DEQUEUE, 0);
	ssize_t pos = 0;
	result = aac_resync(rbsp, &pos);
	rbs_ctrl(rbsp, OPTION_ALLOW_TO_DEQUEUE, value);

	return result;
}

#ifdef CONFIG_CODEC_LIBOPUS
// Resync to next valid Opus frame in the file.
static bool opus_resync(rbstream_p fp, ssize_t *inout_pos)
{
	ssize_t pos = *inout_pos;
	bool valid = false;

	uint8_t buf[FRAME_RESYNC_READ_BYTES];
	ssize_t bytesToRead = FRAME_RESYNC_READ_BYTES;
	ssize_t totalBytesRead = 0;
	ssize_t remainingBytes = 0;
	bool reachEOS = false;
	uint8_t *buf_ptr = buf;

	do {
		if (pos >= *inout_pos + FRAME_RESYNC_MAX_CHECK_BYTES) {
			break;
		}

		if (remainingBytes < OPUS_PACKET_HEADER_LEN) {
			if (reachEOS) {
				break;
			}

			memcpy(buf, buf_ptr, remainingBytes);
			bytesToRead = FRAME_RESYNC_READ_BYTES - remainingBytes;

			/*
			 * The next read position should start from the end of
			 * the last buffer, and thus should include the remaining
			 * bytes in the buffer.
			 */
			totalBytesRead = _source_read_at(fp, pos + remainingBytes, buf + remainingBytes, bytesToRead);
			if (totalBytesRead <= 0) {
				break;
			}

			reachEOS = (totalBytesRead != bytesToRead);
			remainingBytes += totalBytesRead;
			buf_ptr = buf;
			continue;
		}

		if (!OPUS_PACKET_SYNC_VERIFY(buf_ptr)) {
			++pos;
			++buf_ptr;
			--remainingBytes;
			continue;
		}

		// We found what looks like a valid frame,
		// now find its successors.
		valid = true;
		int frame_size = OPUS_PACKET_GETSIZE(buf_ptr);
		ssize_t test_pos = pos + frame_size;
		int j;
		for (j = 0; j < FRAME_MATCH_REQUIRED; ++j) {
			uint8_t temp[OPUS_PACKET_HEADER_LEN];
			ssize_t retval = _source_read_at(fp, test_pos, temp, sizeof(temp));
			if (retval < (ssize_t) sizeof(temp)) {
				valid = false;
				break;
			}

			if (!OPUS_PACKET_SYNC_VERIFY(temp)) {
				valid = false;
				break;
			}

			int test_frame_size = OPUS_PACKET_GETSIZE(temp);
			test_pos += test_frame_size;
		}

		if (valid) {
			*inout_pos = pos;
		}

		++pos;
		++buf_ptr;
		--remainingBytes;
	} while (!valid);

	return valid;
}

// Initialize the Opus reader.
bool opus_init(rbstream_p mFp, ssize_t *offset)
{
	// Sync to the first valid frame.
	bool success = opus_resync(mFp, offset);
	RETURN_VAL_IF_FAIL((success == true), false);

	// Policy: Pop out data when *offset updated!
	rbs_seek_ext(mFp, *offset, SEEK_SET);
	return true;
}

// Get the next valid Opus frame.
bool opus_get_frame(rbstream_p mFp, ssize_t *offset, void *buffer, uint32_t *size)
{
	size_t frame_size = 0;
	uint8_t *buf = (uint8_t *) buffer;

	for (;;) {
		ssize_t n = _source_read_at(mFp, *offset, buffer, OPUS_PACKET_HEADER_LEN);
		RETURN_VAL_IF_FAIL((n == OPUS_PACKET_HEADER_LEN), false);

		if (OPUS_PACKET_SYNC_VERIFY(buf)) {
			frame_size = OPUS_PACKET_GETSIZE(buf);
			break;
		}

		// Lost sync.
		ssize_t pos = *offset;
		RETURN_VAL_IF_FAIL(opus_resync(mFp, &pos), false);

		*offset = pos;
		rbs_seek_ext(mFp, *offset, SEEK_SET);
		// Try again with the new position.
	}

	ssize_t n = _source_read_at(mFp, *offset, buffer, frame_size);
	RETURN_VAL_IF_FAIL((n == (ssize_t) frame_size), false);

	*size = frame_size;
	*offset += frame_size;
	rbs_seek_ext(mFp, *offset, SEEK_SET);

	return true;
}

bool opus_check_type(rbstream_p rbsp)
{
	bool result = false;
	int value = rbs_ctrl(rbsp, OPTION_ALLOW_TO_DEQUEUE, 0);
	ssize_t pos = 0;
	result = opus_resync(rbsp, &pos);
	rbs_ctrl(rbsp, OPTION_ALLOW_TO_DEQUEUE, value);

	return result;
}
#endif

int _get_audio_type(rbstream_p rbsp)
{
	if (mp3_check_type(rbsp)) {
		return AUDIO_TYPE_MP3;
	}

	if (aac_check_type(rbsp)) {
		return AUDIO_TYPE_AAC;
	}

#ifdef CONFIG_CODEC_LIBOPUS
	if (opus_check_type(rbsp)) {
		return AUDIO_TYPE_OPUS;
	}
#endif

	return AUDIO_TYPE_UNKNOWN;
}

bool _get_frame(pv_player_p player)
{
	priv_data_p priv = (priv_data_p) player->priv_data;
	assert(priv != NULL);

	switch (player->audio_type) {
	case AUDIO_TYPE_MP3: {
		tPVMP3DecoderExternal *mp3_ext = (tPVMP3DecoderExternal *) player->dec_ext;
		return mp3_get_frame(player->rbsp, &priv->mCurrentPos, priv->mFixedHeader, (void *)mp3_ext->pInputBuffer, (uint32_t *)&mp3_ext->inputBufferCurrentLength);
	}

	case AUDIO_TYPE_AAC: {
		tPVMP4AudioDecoderExternal *aac_ext = (tPVMP4AudioDecoderExternal *) player->dec_ext;
		return aac_get_frame(player->rbsp, &priv->mCurrentPos, (void *)aac_ext->pInputBuffer, (uint32_t *)&aac_ext->inputBufferCurrentLength);
	}

#ifdef CONFIG_CODEC_LIBOPUS
	case AUDIO_TYPE_OPUS: {
		opus_dec_external_t *opus_ext = (opus_dec_external_t *) player->dec_ext;
		return opus_get_frame(player->rbsp, &priv->mCurrentPos, (void *)opus_ext->pInputBuffer, (uint32_t *)&opus_ext->inputBufferCurrentLength);
	}
#endif

	default:
		medwdbg("[%s] unsupported audio type: %d\n", __FUNCTION__, player->audio_type);
		return false;
	}
}

int _init_decoder(pv_player_p player, void *dec_ext)
{
	priv_data_p priv = (priv_data_p) player->priv_data;
	assert(priv != NULL);

	// Init private data
	priv->mCurrentPos = 0;
	priv->mFixedHeader = 0;
	memset(&(priv->pcm), 0, sizeof(pcm_data_t));

	switch (player->audio_type) {
	case AUDIO_TYPE_MP3: {
		player->dec_ext = calloc(1, sizeof(tPVMP3DecoderExternal));
		RETURN_VAL_IF_FAIL((player->dec_ext != NULL), PV_FAILURE);

		player->dec_mem = calloc(1, pvmp3_decoderMemRequirements());
		RETURN_VAL_IF_FAIL((player->dec_mem != NULL), PV_FAILURE);

		*((tPVMP3DecoderExternal *) player->dec_ext) = *((tPVMP3DecoderExternal *) dec_ext);

		pvmp3_resetDecoder(player->dec_mem);
		pvmp3_InitDecoder((tPVMP3DecoderExternal *) player->dec_ext, player->dec_mem);

		bool ret = mp3_init(player->rbsp, &priv->mCurrentPos, &priv->mFixedHeader);
		RETURN_VAL_IF_FAIL((ret == true), PV_FAILURE);
		break;
	}

	case AUDIO_TYPE_AAC: {
		player->dec_ext = calloc(1, sizeof(tPVMP4AudioDecoderExternal));
		RETURN_VAL_IF_FAIL((player->dec_ext != NULL), PV_FAILURE);

		player->dec_mem = calloc(1, PVMP4AudioDecoderGetMemRequirements());
		RETURN_VAL_IF_FAIL((player->dec_mem != NULL), PV_FAILURE);

		*((tPVMP4AudioDecoderExternal *) player->dec_ext) = *((tPVMP4AudioDecoderExternal *) dec_ext);

		PVMP4AudioDecoderResetBuffer(player->dec_mem);
		Int err = PVMP4AudioDecoderInitLibrary((tPVMP4AudioDecoderExternal *) player->dec_ext, player->dec_mem);
		RETURN_VAL_IF_FAIL((err == MP4AUDEC_SUCCESS), PV_FAILURE);

		bool ret = aac_init(player->rbsp, &priv->mCurrentPos);
		RETURN_VAL_IF_FAIL((ret == true), PV_FAILURE);
		break;
	}

#ifdef CONFIG_CODEC_LIBOPUS
	case AUDIO_TYPE_OPUS: {
		player->dec_ext = calloc(1, sizeof(opus_dec_external_t));
		RETURN_VAL_IF_FAIL((player->dec_ext != NULL), PV_FAILURE);

		player->dec_mem = calloc(1, opus_decoderMemRequirements());
		RETURN_VAL_IF_FAIL((player->dec_mem != NULL), PV_FAILURE);

		*((opus_dec_external_t *) player->dec_ext) = *((opus_dec_external_t *) dec_ext);

		opus_resetDecoder(player->dec_mem);
		int err = opus_initDecoder((opus_dec_external_t *) player->dec_ext, player->dec_mem);
		RETURN_VAL_IF_FAIL((err == OPUS_OK), PV_FAILURE);

		priv->mCurrentPos = 0;
		bool ret = opus_init(player->rbsp, &priv->mCurrentPos);
		RETURN_VAL_IF_FAIL((ret == true), PV_FAILURE);
		break;
	}
#endif

	default:
		// Maybe do not need to init, return success.
		return PV_SUCCESS;
	}

	return PV_SUCCESS;
}

int _frame_decoder(pv_player_p player, pcm_data_p pcm)
{
	switch (player->audio_type) {
	case AUDIO_TYPE_MP3: {
		tPVMP3DecoderExternal tmp_ext = *((tPVMP3DecoderExternal *) player->dec_ext);
		tPVMP3DecoderExternal *mp3_ext = &tmp_ext;

		mp3_ext->inputBufferUsedLength = 0;

		ERROR_CODE errorCode = pvmp3_framedecoder(mp3_ext, player->dec_mem);
		medvdbg("[%s] Line %d, pvmp3_framedecoder, errorCode %d\n", __FUNCTION__, __LINE__, errorCode);
		RETURN_VAL_IF_FAIL((errorCode == NO_DECODING_ERROR), PV_FAILURE);

		pcm->length = mp3_ext->outputFrameSize;
		pcm->samples = mp3_ext->pOutputBuffer;
		pcm->channels = mp3_ext->num_channels;
		pcm->samplerate = mp3_ext->samplingRate;
		break;
	}

	case AUDIO_TYPE_AAC: {
		tPVMP4AudioDecoderExternal *aac_ext = (tPVMP4AudioDecoderExternal *) player->dec_ext;

		aac_ext->inputBufferUsedLength = 0;
		aac_ext->remainderBits = 0;

		Int decoderErr = PVMP4AudioDecodeFrame(aac_ext, player->dec_mem);
		medvdbg("[%s] Line %d, PVMP4AudioDecodeFrame, decoderErr %d\n", __FUNCTION__, __LINE__, decoderErr);
		RETURN_VAL_IF_FAIL((decoderErr == MP4AUDEC_SUCCESS), PV_FAILURE);

		pcm->length = aac_ext->frameLength * aac_ext->desiredChannels;
		pcm->samples = aac_ext->pOutputBuffer;
		pcm->channels = aac_ext->desiredChannels;
		pcm->samplerate = aac_ext->samplingRate;
		break;
	}

#ifdef CONFIG_CODEC_LIBOPUS
	case AUDIO_TYPE_OPUS: {
		opus_dec_external_t *opus_ext = (opus_dec_external_t *) player->dec_ext;

		Int err = opus_frameDecode(opus_ext, player->dec_mem);
		medvdbg("[%s] Line %d, opus_frameDecode, err %d\n", __FUNCTION__, __LINE__, err);
		RETURN_VAL_IF_FAIL((err == OPUS_OK), PV_FAILURE);

		pcm->length = opus_ext->outputFrameSize * opus_ext->desiredChannels;
		pcm->samples = opus_ext->pOutputBuffer;
		pcm->channels = opus_ext->desiredChannels;
		pcm->samplerate = opus_ext->desiredSampleRate;
		break;
	}
#endif

	default:
		// No decoding, return failure.
		return PV_FAILURE;
	}

	return PV_SUCCESS;
}

static size_t _input_callback(void *data, rbstream_p rbsp)
{
	pv_player_p player = (pv_player_p) data;
	assert(player != NULL);

	size_t wlen = 0;
	RETURN_VAL_IF_FAIL((player->input_func != NULL), wlen);
	wlen = player->input_func(player->cb_data, player);

	return wlen;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

// check if the given auido type is supportted or not
bool pv_player_check_audio_type(int audio_type)
{
	switch (audio_type) {
	case AUDIO_TYPE_MP3:
	case AUDIO_TYPE_AAC:
		return true;

#ifdef CONFIG_CODEC_LIBOPUS
	case AUDIO_TYPE_OPUS:
		return true;
#endif

	default:
		return false;
	}
}

size_t pv_player_pushdata(pv_player_p player, const void *data, size_t len)
{
	assert(player != NULL);
	assert(data != NULL);

	static pthread_mutex_t s_mutex = PTHREAD_MUTEX_INITIALIZER;

	pthread_mutex_lock(&s_mutex);
	len = rbs_write(data, 1, len, player->rbsp);
	pthread_mutex_unlock(&s_mutex);

	return len;
}

size_t pv_player_dataspace(pv_player_p player)
{
	assert(player != NULL);

	return rb_avail(&player->ringbuffer);
}

bool pv_player_dataspace_is_empty(pv_player_p player)
{
	assert(player != NULL);

	return !rb_used(&player->ringbuffer);
}

int pv_player_get_audio_type(pv_player_p player)
{
	assert(player != NULL);

	if (!pv_player_check_audio_type(player->audio_type)) {
		player->audio_type = _get_audio_type(player->rbsp);
		medvdbg("audio_type %d\n", player->audio_type);
	}

	return player->audio_type;
}

int pv_player_init_decoder(pv_player_p player, int audio_type, void *dec_ext)
{
	assert(player != NULL);

	// User may tell the audio type
	player->audio_type = audio_type;

	// Try to get from stream (in case of given invalid type).
	pv_player_get_audio_type(player);

	return _init_decoder(player, dec_ext);
}

size_t pv_player_get_frames(pv_player_p player, unsigned char *buf, size_t max, unsigned int *sr, unsigned short *ch)
{
	priv_data_p priv = (priv_data_p) player->priv_data;
	pcm_data_p pcm = &priv->pcm;

	// Output size, in bytes
	size_t size = 0;

	// Check 2 bytes aligned, due to 16bit-PCM format.
	assert((max % 2) == 0);

	while (size < max) {
		if (pcm->samples) {
			// Copy remained data to decoder output buffer
			size_t nSamples = (max - size) / BYTES_PER_SAMPLE;
			if (pcm->length  <= nSamples) {
				// Copy all data
				memcpy(buf + size, pcm->samples, pcm->length * BYTES_PER_SAMPLE);
				size += pcm->length * BYTES_PER_SAMPLE;
				// Clear remained data record
				pcm->samples = NULL;
				pcm->length = 0;
				continue;
			} else {
				// Copy part of data required
				memcpy(buf + size, pcm->samples, nSamples * BYTES_PER_SAMPLE);
				size = max;
				// Update remained data record
				pcm->samples += nSamples;
				pcm->length -= nSamples;
				break;
			}
		}

		// Decode more
		if (!_get_frame(player)) {
			// Need to push more data for further decoding.
			// In this case, *size < max.
			medvdbg("there's not enough data, need to push more.\n");
			break;
		}

		if (_frame_decoder(player, pcm) != PV_SUCCESS) {
			// Decoding failed
			meddbg("frame decoding failed!\n");
			break;
		}
	}

	// Output sample rate if desired
	if (sr != NULL) {
		*sr = pcm->samplerate;
	}

	// Output channel num if desired
	if (ch != NULL) {
		*ch = pcm->channels;
	}

	return size;
}


int pv_player_init(pv_player_p player, size_t rbuf_size)
{
	assert(player != NULL);

	priv_data_p priv = (priv_data_p) malloc(sizeof(priv_data_t));
	RETURN_VAL_IF_FAIL((priv != NULL), PV_FAILURE);

	// init private data
	priv->mCurrentPos = 0;
	priv->mFixedHeader = 0;

	// init player data
	player->cb_data = NULL;
	player->input_func = NULL;

	player->dec_ext = NULL;
	player->dec_mem = NULL;
	player->priv_data = priv;

	// init ring-buffer and open it as a stream
	rb_init(&player->ringbuffer, rbuf_size);
	player->rbsp = rbs_open(&player->ringbuffer, _input_callback, (void *)player);
	RETURN_VAL_IF_FAIL((player->rbsp != NULL), PV_FAILURE);

	rbs_ctrl(player->rbsp, OPTION_ALLOW_TO_DEQUEUE, 1);
	return PV_SUCCESS;
}

int pv_player_register_callbacks(pv_player_p player, void *user_data, input_func_f input_func)
{
	player->cb_data = user_data;
	player->input_func = input_func;

	return PV_SUCCESS;
}

int pv_player_finish(pv_player_p player)
{
	assert(player != NULL);

	// close stream
	rbs_close(player->rbsp);
	player->rbsp = NULL;

	// free ring-buffer instance
	rb_free(&player->ringbuffer);

	// free decoder external buffer
	if (player->dec_ext != NULL) {
		free(player->dec_ext);
		player->dec_ext = NULL;
	}

	// free decoder buffer
	if (player->dec_mem != NULL) {
		#ifdef CONFIG_CODEC_LIBOPUS
		opus_uninitDecoder(player->dec_mem);
		#endif
		free(player->dec_mem);
		player->dec_mem = NULL;
	}

	// free private data buffer
	if (player->priv_data != NULL) {
		free(player->priv_data);
		player->priv_data = NULL;
	}

	return PV_SUCCESS;
}

} // namespace media
