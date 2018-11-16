/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include <media/BufferOutputDataSource.h>
#include "tc_common.h"
#include <errno.h>

using namespace std;
using namespace media;
using namespace media::stream;

static unsigned int channels = 2;
static unsigned int sampleRate = 16000;
static audio_format_type_t pcmFormat = AUDIO_FORMAT_TYPE_S16_LE;

static void itc_media_BufferOutputDataSource_getChannels_setChannels_p(void)
{
	unsigned int compare_channels = 3;
	BufferOutputDataSource dataSource(channels, sampleRate, pcmFormat);
	TC_ASSERT_EQ("getChannels", dataSource.getChannels(), 2);
	dataSource.setChannels(compare_channels);
	TC_ASSERT_EQ("getChannels", dataSource.getChannels(), compare_channels);
	dataSource.setChannels(0);
	TC_ASSERT_EQ("getChannels", dataSource.getChannels(), 0);
	TC_SUCCESS_RESULT();
}

static void itc_media_BufferOutputDataSource_getSampleRate_setSampleRate_p(void)
{
	audio_sample_rate_t sample_rate[] = { AUDIO_SAMPLE_RATE_7350,
		AUDIO_SAMPLE_RATE_8000,
		AUDIO_SAMPLE_RATE_11025,
		AUDIO_SAMPLE_RATE_12000,
		AUDIO_SAMPLE_RATE_16000,
		AUDIO_SAMPLE_RATE_22050,
		AUDIO_SAMPLE_RATE_24000,
		AUDIO_SAMPLE_RATE_32000,
		AUDIO_SAMPLE_RATE_44100,
		AUDIO_SAMPLE_RATE_48000,
		AUDIO_SAMPLE_RATE_64000,
		AUDIO_SAMPLE_RATE_88200,
		AUDIO_SAMPLE_RATE_96000
	};
	int enum_size = sizeof(sample_rate) / sizeof(sample_rate[0]);
	int enum_counter = 0;

	BufferOutputDataSource dataSource(channels, sampleRate, pcmFormat);
	TC_ASSERT_EQ("getSampleRate", dataSource.getSampleRate(), sampleRate);

	for (enum_counter = 0; enum_counter < enum_size; enum_counter++) {
		dataSource.setSampleRate(sample_rate[enum_counter]);
		TC_ASSERT_EQ("getSampleRate", dataSource.getSampleRate(), sample_rate[enum_counter]);
	}
	TC_SUCCESS_RESULT();
}

static void itc_media_BufferOutputDataSource_getPcmFormat_setPcmFormat_p(void)
{
	audio_format_type_t audio_type[] = {
		AUDIO_FORMAT_TYPE_S8,
		AUDIO_FORMAT_TYPE_S16_LE,
		AUDIO_FORMAT_TYPE_S32_LE
	};
	int enum_size = sizeof(audio_type) / sizeof(audio_type[0]);
	int enum_counter = 0;

	BufferOutputDataSource dataSource(channels, sampleRate, pcmFormat);
	TC_ASSERT_EQ("getPcmFormat", dataSource.getPcmFormat(), media::AUDIO_FORMAT_TYPE_S16_LE);

	for (enum_counter = 0; enum_counter < enum_size; enum_counter++) {
		dataSource.setPcmFormat(audio_type[enum_counter]);
		TC_ASSERT_EQ("getPcmFormat", dataSource.getPcmFormat(), audio_type[enum_counter]);
	}

	TC_SUCCESS_RESULT();
}

static void itc_media_BufferOutputDataSource_getAudioType_setAudioType_p(void)
{
	audio_type_t  audio_type[] = {
		AUDIO_TYPE_INVALID,
		AUDIO_TYPE_UNKNOWN,
		AUDIO_TYPE_MP3,
		AUDIO_TYPE_AAC,
		AUDIO_TYPE_PCM,
		AUDIO_TYPE_OPUS,
		AUDIO_TYPE_FLAC,
		AUDIO_TYPE_WAVE
	};
	int enum_size = sizeof(audio_type) / sizeof(audio_type[0]);
	int enum_counter = 0;

	BufferOutputDataSource dataSource(channels, sampleRate, pcmFormat);
	TC_ASSERT_EQ("getAudioType", dataSource.getAudioType(), media::AUDIO_TYPE_INVALID);

	for (enum_counter = 0; enum_counter < enum_size; enum_counter++) {
		dataSource.setAudioType(audio_type[enum_counter]);
		TC_ASSERT_EQ("getAudioType", dataSource.getAudioType(), audio_type[enum_counter]);
	}

	TC_SUCCESS_RESULT();
}

static void itc_media_BufferOutputDataSource_reopen_p(void)
{
	BufferOutputDataSource dataSource;

	TC_ASSERT_EQ("itc_media_BufferOutputDataSource_open", dataSource.open(), true);
	TC_ASSERT_EQ("itc_media_BufferOutputDataSource_open", dataSource.open(), true);
	dataSource.close();

	TC_SUCCESS_RESULT();
}

int itc_media_bufferoutputdatasource_main(void)
{
	itc_media_BufferOutputDataSource_getChannels_setChannels_p();
	itc_media_BufferOutputDataSource_getSampleRate_setSampleRate_p();
	itc_media_BufferOutputDataSource_getPcmFormat_setPcmFormat_p();
	itc_media_BufferOutputDataSource_getAudioType_setAudioType_p();

	itc_media_BufferOutputDataSource_reopen_p();
	return 0;
}
