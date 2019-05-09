/* ****************************************************************
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

#include "MediaUtils.h"
#include <debug.h>
#include <errno.h>

namespace media {
namespace utils {

#ifdef __GNUC__
#define POPCOUNT(x) __builtin_popcount(x)
#else
#define POPCOUNT(x) popcount(x)
#endif

// Mine-Type for audio stream
static const std::string AAC_MIME_TYPE = "audio/aac";
static const std::string AACP_MIME_TYPE = "audio/aacp";
static const std::string MPEG_MIME_TYPE = "audio/mpeg";
static const std::string MP4_MIME_TYPE = "audio/mp4";
static const std::string OPUS_MIME_TYPE = "audio/opus";

void toLowerString(std::string &str)
{
	for (char& c : str) {
		if ('A' <= c && c <= 'Z') {
			c += ('a' - 'A');
		}
	}
}

void toUpperString(std::string &str)
{
	for (char& c : str) {
		if ('a' <= c && c <= 'z') {
			c -= ('a' - 'A');
		}
	}
}

audio_type_t getAudioTypeFromPath(std::string datapath)
{
	std::string basename = datapath.substr(datapath.find_last_of("/") + 1);
	std::string extension;

	if (basename.find(".") == std::string::npos) {
		extension = "";
	} else {
		extension = basename.substr(basename.find_last_of(".") + 1);
	}

	toLowerString(extension);

	if (extension.compare("mp3") == 0) {
		medvdbg("audio type : mp3\n");
		return AUDIO_TYPE_MP3;
	} else if ((extension.compare("aac") == 0) || (extension.compare("mp4") == 0)) {
		medvdbg("audio type : aac\n");
		return AUDIO_TYPE_AAC;
	} else if ((extension.compare("opus") == 0)) {
		medvdbg("audio type : opus\n");
		return AUDIO_TYPE_OPUS;
	} else if ((extension.compare("flac") == 0)) {
		medvdbg("audio type : flac\n");
		return AUDIO_TYPE_FLAC;
	} else if ((extension.compare("") == 0) || (extension.compare("pcm") == 0) || (extension.compare("raw") == 0)) {
		medvdbg("audio type : pcm\n");
		return AUDIO_TYPE_PCM;
	} else if (extension.compare(AUDIO_EXT_TYPE_WAV) == 0) {
		medvdbg("audio type : wav\n");
		return AUDIO_TYPE_WAVE;
	} else {
		medvdbg("audio type : unknown\n");
		return AUDIO_TYPE_INVALID;
	}
}

audio_type_t getAudioTypeFromMimeType(std::string &mimeType)
{
	audio_type_t audioType;

	if (mimeType.empty()) {
		medwdbg("empty mime type!\n");
		audioType = AUDIO_TYPE_UNKNOWN;
	} else if ((mimeType.find(AAC_MIME_TYPE) != std::string::npos) ||
			(mimeType.find(AACP_MIME_TYPE) != std::string::npos)) {
		audioType = AUDIO_TYPE_AAC;
	} else if (mimeType.find(MPEG_MIME_TYPE) != std::string::npos) {
		audioType = AUDIO_TYPE_MP3;
	} else if (mimeType.find(MP4_MIME_TYPE) != std::string::npos) {
		audioType = AUDIO_TYPE_AAC;
	} else if (mimeType.find(OPUS_MIME_TYPE) != std::string::npos) {
		audioType = AUDIO_TYPE_OPUS;
	} else {
		meddbg("Unsupported mime type: %s\n", mimeType.c_str());
		audioType = AUDIO_TYPE_UNKNOWN;
	}

	return audioType;
}

bool mp3_header_parsing(unsigned char *header, unsigned int *channel, unsigned int *sampleRate)
{
/**
*mp3_header is AAAAAAAA AAABBCCD EEEEFFGH IIJJKLMM
*A - Frame sync
*B - MPEG Audio version
*...
*F - Sampling rate frequency
*.
*I - Channel Mode
*...
*/
	unsigned char bit;
	unsigned int mpegVersion;

	bit = header[1];
	bit >>= 3;
	bit &= (unsigned char)0x03;
	/* we need B information so Shift header[1] three times to the right, and & 0x03 */
	switch (bit) {
	case 0:
		mpegVersion = 2;
		break;
	case 2:
		mpegVersion = 1;
		break;
	case 3:
		mpegVersion = 0;
		break;
	default:
		medvdbg("Not Supported Format mpeg version : %u\n", mpegVersion);
		return false;
	}

	bit = header[2];
	bit >>= 2;
	bit &= 0x03;
	/* we need F information so Shift header[1] two times to the right, and & 0x03 */
	switch (bit) {
	case 0:
		*sampleRate = AUDIO_SAMPLE_RATE_44100;
		break;
	case 1:
		*sampleRate = AUDIO_SAMPLE_RATE_48000;
		break;
	case 2:
		*sampleRate = AUDIO_SAMPLE_RATE_32000;
		break;
	default:
		medvdbg("Not Supported Format sample rate : %u\n", sampleRate);
		return false;
	}

	*sampleRate >>= mpegVersion;

	bit = header[3];
	bit >>= 6;
	/* we need I information so Shift header[3] six times to the right */
	if (bit <= 2) {
		*channel = 2;
	} else {
		*channel = 1;
	}
	return true;
}

bool aac_header_parsing(unsigned char *header, unsigned int *channel, unsigned int *sampleRate)
{
/**
*aac_header is AAAAAAAA AAAABCCD EEFFFFGH HHIJKLMM MMMMMMMM MMMOOOOO OOOOOOPP
*A - syncword 0xFFF, all bits must be 1
*....
*F - Sampling rate frequency
*..
*H - Channel Mode
*......
*/
	unsigned char bit;
	bit = header[2];
	bit >>= 2;
	bit &= 0x0F;
	/* we need F information so Shift header[2] two times to the right, and & 0x0F */
	switch (bit) {
	case 0:
		*sampleRate = AUDIO_SAMPLE_RATE_96000;
		break;
	case 1:
		*sampleRate = AUDIO_SAMPLE_RATE_88200;
		break;
	case 2:
		*sampleRate = AUDIO_SAMPLE_RATE_64000;
		break;
	case 3:
		*sampleRate = AUDIO_SAMPLE_RATE_48000;
		break;
	case 4:
		*sampleRate = AUDIO_SAMPLE_RATE_44100;
		break;
	case 5:
		*sampleRate = AUDIO_SAMPLE_RATE_32000;
		break;
	case 6:
		*sampleRate = AUDIO_SAMPLE_RATE_24000;
		break;
	case 7:
		*sampleRate = AUDIO_SAMPLE_RATE_22050;
		break;
	case 8:
		*sampleRate = AUDIO_SAMPLE_RATE_16000;
		break;
	case 9:
		*sampleRate = AUDIO_SAMPLE_RATE_12000;
		break;
	case 10:
		*sampleRate = AUDIO_SAMPLE_RATE_11025;
		break;
	case 11:
		*sampleRate = AUDIO_SAMPLE_RATE_8000;
		break;
	case 12:
		*sampleRate = AUDIO_SAMPLE_RATE_7350;
		break;
	default:
		meddbg("Not Supported Format sample rate : %u\n", sampleRate);
		return false;
	}
	bit = header[2];
	bit <<= 2;
	bit |= (header[3] >> 6);
	bit &= 0x07;
	/* we need H information so Shift header[3] six times to the right, and & 0x07 */
	if (bit <= 0 || bit >= 8) {
		meddbg("Invalid value bit : %d\n", bit);
		return false;
	}

	if (bit == 7) {
		*channel = 8;
	} else {
		*channel = (unsigned int)bit;
	}
	return true;
}

bool wave_header_parsing(unsigned char *header, unsigned int *channel, unsigned int *sampleRate, audio_format_type_t *pcmFormat)
{
/**
*wave header is
*Chunk ID    (4byte) / .... (4byte) /            .... (4byte)                /
*.....       (4byte) / .... (4byte) / .. (2byte)   / NumChannels     (2byte) /
*sample Rate (4byte) / .... (4byte) / .. (2byte)   / Bits Per sample (2byte) /
*.....       (4byte) / .... (4byte)
*/
	unsigned short bitPerSample;
	*channel = header[23];
	*channel <<= 8;
	*channel |= header[22];

	*sampleRate = header[27];
	*sampleRate <<= 8;
	*sampleRate |= header[26];
	*sampleRate <<= 8;
	*sampleRate |= header[25];
	*sampleRate <<= 8;
	*sampleRate |= header[24];

	bitPerSample = header[35];
	bitPerSample <<= 8;
	bitPerSample |= header[34];
	/* wave header is Little Endian, so read from right byte */
	switch (bitPerSample) {
	case 8:
		*pcmFormat = AUDIO_FORMAT_TYPE_S8;
		break;
	case 16:
		*pcmFormat = AUDIO_FORMAT_TYPE_S16_LE;
		break;
	case 32:
		*pcmFormat = AUDIO_FORMAT_TYPE_S32_LE;
		break;
	default:
		meddbg("Not Supported Format bit/sample : %u\n", bitPerSample);
		return false;
	}
	return true;
}

bool header_parsing(FILE *fp, audio_type_t audioType, unsigned int *channel, unsigned int *sampleRate, audio_format_type_t *pcmFormat)
{
	unsigned char *header;
	unsigned char tag[2];
	bool isHeader;
	int ret;

	switch (audioType) {
	case AUDIO_TYPE_MP3:
		isHeader = false;
		while (fread(tag, sizeof(unsigned char), 2, fp) == 2) {
			/* 12 bits for MP3 Sync Word(the beginning of the frame) */
			if ((tag[0] == 0xFF) && ((tag[1] & 0xF0) == 0xF0)) {
				isHeader = true;
				break;
			} else {
				/* If isn't the header information, go back 1byte and then check 2bytes again */
				ret = fseek(fp, -1, SEEK_CUR);
				if (ret != OK) {
					meddbg("file seek failed errno : %d\n", errno);
					return false;
				}
			}
		}
		if (isHeader) {
			header = (unsigned char *)malloc(sizeof(unsigned char) * (MP3_HEADER_LENGTH + 1));

			if (header == NULL) {
				meddbg("malloc failed error\n");
				return false;
			}

			ret = fseek(fp, -2, SEEK_CUR);
			if (ret != OK) {
				meddbg("file seek failed errno : %d\n", errno);
				free(header);
				return false;
			}

			ret = fread(header, sizeof(unsigned char), MP3_HEADER_LENGTH, fp);
			if (ret != MP3_HEADER_LENGTH) {
				meddbg("read failed ret : %d\n", ret);
				free(header);
				return false;
			}

			if (!mp3_header_parsing(header, channel, sampleRate)) {
				meddbg("Header parsing failed\n");
				free(header);
				return false;
			}
		} else {
			medvdbg("no header\n");
			return false;
		}
		break;
	case AUDIO_TYPE_AAC:
		isHeader = false;
		while (fread(tag, sizeof(unsigned char), 1, fp) > 0) {
			if (tag[0] == 0xFF) {
				isHeader = true;
				break;
			}
		}
		if (isHeader) {
			header = (unsigned char *)malloc(sizeof(unsigned char) * (AAC_HEADER_LENGTH + 1));
			if (header == NULL) {
				medvdbg("malloc failed error\n");
				return false;
			}

			if (fseek(fp, -1, SEEK_CUR) != 0) {
				meddbg("file seek failed error\n");
				free(header);
				return false;
			}

			if ((fread(header, sizeof(unsigned char), AAC_HEADER_LENGTH, fp) != AAC_HEADER_LENGTH) || !aac_header_parsing(header, channel, sampleRate)) {
				free(header);
				return false;
			}
		} else {
			medvdbg("no header\n");
			return false;
		}
		break;
	case AUDIO_TYPE_WAVE:
		header = (unsigned char *)malloc(sizeof(unsigned char) * (WAVE_HEADER_LENGTH + 1));
		if (header == NULL) {
			medvdbg("malloc failed error\n");
			return false;
		}

		if ((fread(header, sizeof(unsigned char), WAVE_HEADER_LENGTH, fp) != WAVE_HEADER_LENGTH) || !wave_header_parsing(header, channel, sampleRate, pcmFormat)) {
			free(header);
			return false;
		}
		break;
	default:
		medvdbg("does not support header parsing\n");
		return false;
	}

	if (header != NULL) {
		free(header);
	}

	if (fseek(fp, 0, SEEK_SET) != 0) {
		meddbg("file seek failed error\n");
		return false;
	}
	return true;
}

bool header_parsing(unsigned char *buffer, unsigned int bufferSize, audio_type_t audioType, unsigned int *channel, unsigned int *sampleRate, audio_format_type_t *pcmFormat)
{
	unsigned int headPoint;
	unsigned char *header;
	bool isHeader;
	switch (audioType) {
	case AUDIO_TYPE_MP3:
		if (MP3_HEADER_LENGTH > bufferSize) {
			medvdbg("no header\n");
			return false;
		}
		isHeader = false;
		headPoint = 0;
		while (headPoint < bufferSize) {
			/* 12 bits for MP3 Sync Word(the beginning of the frame) */
			if ((buffer[headPoint]) == 0xFF && ((buffer[headPoint + 1] & 0xF0) == 0xF0)) {
				isHeader = true;
				break;
			}
			headPoint++;
		}
		if (isHeader && MP3_HEADER_LENGTH <= bufferSize - headPoint) {
			header = (unsigned char *)malloc(sizeof(unsigned char) * (MP3_HEADER_LENGTH + 1));
			if (header == NULL) {
				medvdbg("malloc failed error\n");
				return false;
			}
			memcpy(header, buffer + headPoint, MP3_HEADER_LENGTH);
			if (!mp3_header_parsing(header, channel, sampleRate)) {
				free(header);
				return false;
			}
		} else {
			medvdbg("no header\n");
			return false;
		}
		break;
	case AUDIO_TYPE_AAC:
		if (AAC_HEADER_LENGTH > bufferSize) {
			medvdbg("no header\n");
			return false;
		}
		isHeader = false;
		headPoint = 0;
		while (headPoint < bufferSize) {
			if (buffer[headPoint] == 0xFF) {
				isHeader = true;
				break;
			}
			headPoint++;
		}
		if (isHeader && AAC_HEADER_LENGTH <= bufferSize - headPoint) {
			header = (unsigned char *)malloc(sizeof(unsigned char) * (AAC_HEADER_LENGTH + 1));
			if (header == NULL) {
				medvdbg("malloc failed error\n");
				return false;
			}
			memcpy(header, buffer + headPoint, MP3_HEADER_LENGTH);
			if (!aac_header_parsing(header, channel, sampleRate)) {
				free(header);
				return false;
			}
		} else {
			medvdbg("no header\n");
			return false;
		}
		break;
	case AUDIO_TYPE_WAVE:
		if (WAVE_HEADER_LENGTH <= bufferSize) {
			header = (unsigned char *)malloc(sizeof(unsigned char) * (WAVE_HEADER_LENGTH + 1));
			if (header == NULL) {
				medvdbg("malloc failed error\n");
				return false;
			}
			memcpy(header, buffer, WAVE_HEADER_LENGTH);
			if (!wave_header_parsing(header, channel, sampleRate, pcmFormat)) {
				free(header);
				return false;
			}
		} else {
			medvdbg("no header\n");
			return false;
		}
		break;
	default:
		medvdbg("does not support header parsing\n");
		return false;
	}

	if (header != NULL) {
		free(header);
	}
	return true;
}

struct wav_header_s {
	char headerRiff[4]; //"RIFF"
	uint32_t riffSize;
	char headerWave[4]; //"wave"
	char headerFmt[4]; //"fmt "
	uint32_t fmtSize; //16 for pcm
	uint16_t format; //1 for pcm
	uint16_t channels;
	uint32_t sampleRate;
	uint32_t byteRate;
	uint16_t blockAlign;
	uint16_t bitPerSample;
	char headerData[4]; //"data"
	uint32_t dataSize;
};

bool createWavHeader(FILE *fp)
{
	struct wav_header_s *header;
	header = (struct wav_header_s *)malloc(sizeof(struct wav_header_s));
	if (!header) {
		meddbg("fail to malloc buffer\n");
		return false;
	}

	memset(header, 0xff, WAVE_HEADER_LENGTH);
	int ret;
	ret = fwrite(header, sizeof(unsigned char), WAVE_HEADER_LENGTH, fp);
	if (ret != WAVE_HEADER_LENGTH) {
		meddbg("file write failed error %d\n", errno);
		free(header);
		return false;
	}
	if (fseek(fp, WAVE_HEADER_LENGTH, SEEK_SET) != 0) {
		meddbg("file seek failed error\n");
		free(header);
		return false;
	}

	free(header);
	return true;
}

bool writeWavHeader(FILE *fp, unsigned int channel, unsigned int sampleRate, audio_format_type_t pcmFormat, unsigned int fileSize)
{
/**
*wave header is
*Chunk ID 'RIFF' (4byte) / Chunk Size (4byte) / Fomat 'WAVE' (4byte) /
*Chunk ID 'fmt ' (4byte) / Chunk Size (4byte) / Audio Format (2byte) / NumChannels     (2byte) /
*sample Rate     (4byte) / Byte Rate  (4byte) / Block Align  (2byte) / Bits Per sample (2byte) /
*Chunk ID 'data' (4byte) / Chunk Size (4byte)
*/
	if (fseek(fp, 0, SEEK_SET) != 0) {
		meddbg("file seek failed error\n");
		return false;
	}

	uint32_t byteRate = 0;
	uint16_t bitPerSample = 0;
	uint16_t blockAlign = 0;

	switch (pcmFormat) {
	case AUDIO_FORMAT_TYPE_S16_LE:
		bitPerSample = 16;
		break;
	case AUDIO_FORMAT_TYPE_S32_LE:
		bitPerSample = 32;
		break;
	default:
		meddbg("does not support audio format.\n");
		return false;
	}

	blockAlign = channel * (bitPerSample >> 3);
	byteRate = sampleRate * blockAlign;

	struct wav_header_s *header;
	header = (struct wav_header_s *)malloc(sizeof(struct wav_header_s));
	if (header == NULL) {
		meddbg("malloc failed error\n");
		return false;
	}

	strncpy(header->headerRiff, "RIFF", 4);

	header->riffSize = fileSize - 8;

	strncpy(header->headerWave, "WAVE", 4);
	strncpy(header->headerFmt, "fmt ", 4);

	header->fmtSize = 16;
	header->format = 1;
	header->channels = channel;
	header->sampleRate = sampleRate;
	header->byteRate = byteRate;
	header->blockAlign = blockAlign;
	header->bitPerSample = bitPerSample;

	strncpy(header->headerData, "data", 4);

	header->dataSize = fileSize - WAVE_HEADER_LENGTH;

	int ret = 0;
	ret = fwrite(header, sizeof(unsigned char), WAVE_HEADER_LENGTH, fp);

	if (ret != WAVE_HEADER_LENGTH) {
		meddbg("file write failed error %d\n", errno);
		free(header);
		return false;
	}

	free(header);
	return true;
}

#ifndef __GNUC__
static int32_t popcount(uint32_t x)
{
	x -= (x >> 1) & 0x55555555;
	x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
	x = (x + (x >> 4)) & 0x0F0F0F0F;
	x += x >> 8;
	x += x >> 16;
	return x & 0x0000003F;
}
#endif

unsigned int splitChannel(unsigned int layout, const signed short *stream, unsigned int frames, unsigned int channels, ...)
{
	uint32_t ret = 0;
	if (stream == NULL) {
		meddbg("invalid audio stream!\n");
		return ret;
	}

	uint32_t spf = POPCOUNT(layout); // samples per frame
	uint32_t mask, i, j;
	const int16_t *sdata;
	int16_t *buffer;

	va_list ap;
	va_start(ap, channels);

	for (i = 0; i < channels; i++) {
		mask = va_arg(ap, uint32_t);
		buffer = va_arg(ap, int16_t *);

		// Check params validation
		if (POPCOUNT(mask) != 1) {
			meddbg("specified channel must be a single channel! i:%u, mask:0x%x\n", i, mask);
			continue;
		}

		if ((layout & mask) == 0) {
			meddbg("specified channel does not exist! layout: 0x%x, i:%u, mask:0x%x\n", layout, i, mask);
			continue;
		}

		if (buffer == NULL) {
			meddbg("invalid output buffer! i:%u, mask:0x%x\n", i, mask);
			continue;
		}

		sdata = stream + POPCOUNT(layout & (mask - 1));
		for (j = 0; j < frames; j++) {
			*buffer++ = *sdata;
			sdata += spf;
		}

		ret |= mask;
	}

	va_end(ap);
	return ret;
}
} // namespace util
} // namespace media
