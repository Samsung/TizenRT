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

#include <media/MediaUtils.h>
#include <debug.h>
#include <errno.h>
#include <tinyara/config.h>
#include <cstring>

#ifdef CONFIG_CONTAINER_MPEG2TS
#include "../demux/mpeg2ts/TSDemuxer.h"
#endif

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
static const std::string MP2T_MIME_TYPE = "video/MP2T";

// Sample Rate(in Hz) tables
static const int sampleRateTable[3][3] = {
	{44100, 48000, 32000},
	{22050, 24000, 16000},
	{11025, 12000, 8000},
};
// Bit Rate (in kbps) tables
// V1 - MPEG 1, V2 - MPEG 2 and MPEG 2.5
// L1 - Layer 1, L2 - Layer 2, L3 - Layer 3
static const int bitRateTable[3][3][14] = {
	{
		{32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448}, /* MPEG 1, Layer 1 */
		{32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384},	/* MPEG 1, Layer 2 */
		{32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320},	 /* MPEG 1, Layer 3 */
	},
	{
		{32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256}, /* MPEG 2, Layer 1 */
		{8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160},	  /* MPEG 2, Layer 2 */
		{8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160},	  /* MPEG 2, Layer 3 */
	},
	{
		{32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256}, /* MPEG 2.5, Layer 1 */
		{8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160},	  /* MPEG 2.5, Layer 2 */
		{8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160},	  /* MPEG 2.5, Layer 3 */
	},
};

// MPEG version
#define MPEG_1 3
#define MPEG_2 2
#define MPEG_UNDEFINED 1
#define MPEG_2_5 0

// MPEG layer
#define LAYER_1 3
#define LAYER_2 2
#define LAYER_3 1
#define LAYER_UNDEFINED 0

// Bitrate index
#define BITRATE_FREE 0x0 // Variable Bit Rate
#define BITRATE_BAD 0xf  // Not allow

// Sample Rate index
#define SAMPLE_RATE_UNDEFINED 0x3

// aau size = frame samples * (1 / sample rate) * bitrate / 8 + padding
//            = frame samples * bitrate / 8 / sample rate + padding
// Number of frame samples is constant value as below table:
//        MPEG1  MPEG2(LSF)  MPEG2.5(LSF)
// Layer1  384     384         384
// Layer2  1152    1152        1152
// Layer3  1152    576         576
#define MPEG_LAYER1_AAU_SIZE(sr, br, pad) (384 * ((br)*1000) / 8 / (sr) + ((pad)*4))
#define MPEG_LAYER2_AAU_SIZE(sr, br, pad) (1152 * ((br)*1000) / 8 / (sr) + (pad))
#define MPEG1_LAYER3_AAU_SIZE(sr, br, pad) (1152 * ((br)*1000) / 8 / (sr) + (pad))
#define MPEG2_LAYER3_AAU_SIZE(sr, br, pad) (576 * ((br)*1000) / 8 / (sr) + (pad))

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
	} else if (extension.compare("ts") == 0) {
		medvdbg("audio type : ts\n");
		return AUDIO_TYPE_MP2T;
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
	} else if (mimeType.find(MP2T_MIME_TYPE) != std::string::npos) {
		audioType = AUDIO_TYPE_MP2T;
	} else {
		meddbg("Unsupported mime type: %s\n", mimeType.c_str());
		audioType = AUDIO_TYPE_UNKNOWN;
	}

	return audioType;
}

int buffer_header_parsing(unsigned char *buffer, unsigned int bufferSize, media::audio_type_t audioType, unsigned int *channel, unsigned int *sampleRate, unsigned int *samplePerFrame, media::audio_format_type_t *pcmFormat)
{
	unsigned int headPoint = 0;
	unsigned char *header = NULL;
	bool isHeader = false;
	switch (audioType) {
	case AUDIO_TYPE_MP3:
		while (headPoint < bufferSize) {
			if ((((buffer[headPoint] & 0xFF) << 8) | (buffer[headPoint + 1] & 0xF0)) == 0xFFF0) {
				if (mp3_header_parsing(&buffer[headPoint], channel, sampleRate, samplePerFrame) == true) {
					isHeader = true;
					break;
				}
			}

			headPoint++;
		}

		if (!isHeader) {
			return -1;
		}
		break;
	case AUDIO_TYPE_AAC:
		if (AAC_HEADER_LENGTH > bufferSize) {
			medvdbg("no header\n");
			return -1;
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
				return -1;
			}
			memcpy(header, buffer + headPoint, MP3_HEADER_LENGTH);
			if (!aac_header_parsing(header, channel, sampleRate)) {
				free(header);
				return -1;
			}
		} else {
			medvdbg("no header\n");
			return -1;
		}
		break;
	case AUDIO_TYPE_WAVE:
		if (WAVE_HEADER_LENGTH <= bufferSize) {
			header = (unsigned char *)malloc(sizeof(unsigned char) * (WAVE_HEADER_LENGTH + 1));
			if (header == NULL) {
				medvdbg("malloc failed error\n");
				return -1;
			}
			memcpy(header, buffer, WAVE_HEADER_LENGTH);
			if (!wave_header_parsing(header, channel, sampleRate, pcmFormat)) {
				free(header);
				return -1;
			}
		} else {
			medvdbg("no header\n");
			return -1;
		}
		break;
	default:
		medvdbg("does not support header parsing\n");
		return -1;
	}

	if (header != NULL) {
		free(header);
	}

	return headPoint;
}

bool mp3_header_parsing(unsigned char *buffer, unsigned int *channel, unsigned int *sampleRate, unsigned int *samplePerFrame)
{
	unsigned int header;
	unsigned int bitrate;
	unsigned int aau_size;
	unsigned int version_index;
	unsigned int layer_index;

	unsigned char version;
	unsigned char layer;
	unsigned char bitrate_index;
	unsigned char sample_rate_index;
	unsigned char padding;
	unsigned char channel_mode;

	header = buffer[0] << 24 | buffer[1] << 16 | buffer[2] << 8 | buffer[3];

	version = (header >> 19) & 0x3;
	switch (version) {
	case MPEG_1:
		version_index = 0;
		break;
	case MPEG_2:
		version_index = 1;
		break;
	case MPEG_2_5:
		version_index = 2;
		break;
	default:
		return false;
	}

	layer = (header >> 17) & 0x3;
	switch (layer) {
	case LAYER_1:
		layer_index = 0;
		break;
	case LAYER_2:
		layer_index = 1;
		break;
	case LAYER_3:
		layer_index = 2;
		break;
	default:
		return false;
	}

	bitrate_index = (header >> 12) & 0xf;
	if ((bitrate_index == BITRATE_FREE) || bitrate_index == BITRATE_BAD) {
		return false;
	}

	sample_rate_index = (header >> 10) & 0x3;
	if (sample_rate_index == SAMPLE_RATE_UNDEFINED) {
		return false;
	}
	padding = (header >> 9) & 0x1;

	bitrate = bitRateTable[version_index][layer_index][bitrate_index - 1];
	*sampleRate = sampleRateTable[version_index][sample_rate_index];

	channel_mode = (header >> 8) & 0x3;
	if (channel_mode <= 2) {
		*channel = 2;
	} else {
		*channel = 1;
	}

	switch (layer) {
	case LAYER_1:
		aau_size = MPEG_LAYER1_AAU_SIZE(*sampleRate, bitrate, padding);
		break;
	case LAYER_2:
		aau_size = MPEG_LAYER2_AAU_SIZE(*sampleRate, bitrate, padding);
		break;
	case LAYER_3:
		if (version == MPEG_1) {
			aau_size = MPEG1_LAYER3_AAU_SIZE(*sampleRate, bitrate, padding);
		} else {
			aau_size = MPEG2_LAYER3_AAU_SIZE(*sampleRate, bitrate, padding);
		}
		break;
	}

	if (aau_size > 0) {
		*samplePerFrame = aau_size;
	}

	if (buffer[0] == buffer[aau_size] && buffer[1] == buffer[aau_size + 1]) {
		meddbg("aau_size : %d version : %d bitrate : %d samplerate : %d", aau_size, version, bitrate, *sampleRate);
		return true;
	}

	return false;
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

#ifdef CONFIG_CONTAINER_MPEG2TS
int ts_parsing(unsigned char *buffer, unsigned int bufferSize, audio_type_t *audioType, unsigned int *channel, unsigned int *sampleRate, audio_format_type_t *pcmFormat)
{
	// create temporary ts parser
	auto tsDemuxer = media::TSDemuxer::create();
	if (!tsDemuxer) {
		meddbg("TSDemuxer::create failed\n");
		return false;
	}

	// push the given (ts) data into demux buffer
	size_t ret = tsDemuxer->pushData(buffer, bufferSize);
	if (ret < bufferSize) {
		medwdbg("TSDemuxer accept part of data %u/%u\n", ret, bufferSize);
	}

	// pre parse to get PSI
	if (tsDemuxer->prepare() < 0) {
		meddbg("TSDemuxer parse failed\n");
		return false;
	}

	// get programs in ts, usually we select the 1th one as default.
	std::vector<unsigned short> programs;
	tsDemuxer->getPrograms(programs);
	medvdbg("There's %lu programs in the given transport stream\n", programs.size());
	if (programs.empty()) {
		meddbg("TSDemuxer didn't find any program! Failed!\n");
		return false;
	}

	// get audio type (from PMT component stream type field)
	*audioType = tsDemuxer->getAudioType((void *)&programs[0]);

	// get ES data (we expect the given ts data is enough to form a PES packet)
	unsigned char audioES[64]; // 64 bytes should be enough for header parsing
	ssize_t audioESLen = tsDemuxer->pullData(audioES, sizeof(audioES), (void *)&programs[0]);
	if (audioESLen < 0) {
		meddbg("TSDemuxer get ES data failed!\n");
		return false;
	}

	if (pcmFormat) {
		// Asign default value (same in DataSource)
		*pcmFormat = AUDIO_FORMAT_TYPE_S16_LE;
	}

	unsigned int samplePerFrame = 0;

	return buffer_header_parsing(audioES, (unsigned int)audioESLen, *audioType, channel, sampleRate, &samplePerFrame, pcmFormat);
}
#endif

bool file_header_parsing(FILE *fp, audio_type_t audioType, unsigned int *channel, unsigned int *sampleRate, audio_format_type_t *pcmFormat)
{
	unsigned char *header = NULL;
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

			unsigned int samplePerFrame = 0;
			if (mp3_header_parsing(header, channel, sampleRate, &samplePerFrame) == -1) {
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
				meddbg("malloc failed error\n");
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
			meddbg("no header\n");
			return false;
		}
		break;
	case AUDIO_TYPE_WAVE:
		header = (unsigned char *)malloc(sizeof(unsigned char) * (WAVE_HEADER_LENGTH + 1));
		if (header == NULL) {
			meddbg("malloc failed error\n");
			return false;
		}

		if ((fread(header, sizeof(unsigned char), WAVE_HEADER_LENGTH, fp) != WAVE_HEADER_LENGTH) || !wave_header_parsing(header, channel, sampleRate, pcmFormat)) {
			free(header);
			return false;
		}
		break;
#ifdef CONFIG_CONTAINER_MPEG2TS
	case AUDIO_TYPE_MP2T: {
		// has container, demux and parse stream data to get audio type
		size_t bufferSize = CONFIG_DATASOURCE_PREPARSE_BUFFER_SIZE;
		header = (unsigned char *)malloc(sizeof(unsigned char) * bufferSize);
		if (!header) {
			meddbg("run out of memory! size %u\n", bufferSize);
			return false;
		}

		size_t readSize = fread(header, sizeof(unsigned char), bufferSize, fp);
		if (readSize != bufferSize) {
			free(header);
			meddbg("can not read enough data for preparsing! read:%u\n", readSize);
			return false;
		}

		if (!ts_parsing(header, readSize, &audioType, channel, sampleRate, pcmFormat)) {
			free(header);
			meddbg("stream_parsing failed, can not get audio codec type!\n");
			return false;
		}
	} break;
#endif
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

	memcpy(header->headerRiff, "RIFF", 4);

	header->riffSize = fileSize - 8;

	memcpy(header->headerWave, "WAVE", 4);
	memcpy(header->headerFmt, "fmt ", 4);

	header->fmtSize = 16;
	header->format = 1;
	header->channels = channel;
	header->sampleRate = sampleRate;
	header->byteRate = byteRate;
	header->blockAlign = blockAlign;
	header->bitPerSample = bitPerSample;

	memcpy(header->headerData, "data", 4);

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
