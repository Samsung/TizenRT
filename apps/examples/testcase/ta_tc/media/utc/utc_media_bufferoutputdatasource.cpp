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

static void utc_media_BufferOutputDataSource_getChannels_p(void)
{
	BufferOutputDataSource dataSource(channels, sampleRate, pcmFormat);
	TC_ASSERT_EQ("utc_media_BufferOutputDataSource_getChannels", dataSource.getChannels(), channels);
	TC_SUCCESS_RESULT();
}

static void utc_media_BufferOutputDataSource_getSampleRate_p(void)
{
	BufferOutputDataSource dataSource(channels, sampleRate, pcmFormat);
	TC_ASSERT_EQ("utc_media_BufferOutputDataSource_getSampleRate", dataSource.getSampleRate(), sampleRate);
	TC_SUCCESS_RESULT();
}

static void utc_media_BufferOutputDataSource_getPcmFormat_p(void)
{
	BufferOutputDataSource dataSource(channels, sampleRate, pcmFormat);
	TC_ASSERT_EQ("utc_media_BufferOutputDataSource_getPcmFormat", dataSource.getPcmFormat(), pcmFormat);
	TC_SUCCESS_RESULT();
}

static void utc_media_BufferOutputDataSource_getAudioType_p(void)
{
	BufferOutputDataSource dataSource(channels, sampleRate, pcmFormat);
	TC_ASSERT_EQ("utc_media_BufferOutputDataSource_getAudioType", dataSource.getAudioType(), media::AUDIO_TYPE_INVALID);
	TC_SUCCESS_RESULT();
}

static void utc_media_BufferOutputDataSource_setChannels_p(void)
{
	BufferOutputDataSource dataSource;
	unsigned int compare_channels = 3;
	dataSource.setChannels(compare_channels);
	TC_ASSERT_EQ("utc_media_BufferOutputDataSource_setChannels", dataSource.getChannels(), compare_channels);
	TC_SUCCESS_RESULT();
}

static void utc_media_BufferOutputDataSource_setSampleRate_p(void)
{
	BufferOutputDataSource dataSource;
	unsigned int compare_sampleRate = 32000;
	dataSource.setSampleRate(compare_sampleRate);
	TC_ASSERT_EQ("utc_media_BufferOutputDataSource_setSampleRate", dataSource.getSampleRate(), compare_sampleRate);
	TC_SUCCESS_RESULT();
}

static void utc_media_BufferOutputDataSource_setPcmFormat_p(void)
{
	BufferOutputDataSource dataSource;
	media::audio_format_type_t compare_pcmFormat = media::AUDIO_FORMAT_TYPE_S8;
	dataSource.setPcmFormat(media::AUDIO_FORMAT_TYPE_S8);
	TC_ASSERT_EQ("utc_media_BufferOutputDataSource_setPcmFormat", dataSource.getPcmFormat(), compare_pcmFormat);
	TC_SUCCESS_RESULT();
}

static void utc_media_BufferOutputDataSource_setAudioType_p(void)
{
	BufferOutputDataSource dataSource;
	media::audio_type_t compare_audioType = media::AUDIO_TYPE_MP3;
	dataSource.setAudioType(media::AUDIO_TYPE_MP3);
	TC_ASSERT_EQ("utc_media_BufferOutputDataSource_setAudioType", dataSource.getAudioType(), compare_audioType);
	TC_SUCCESS_RESULT();
}

static void utc_media_BufferOutputDataSource_CopyConstructor_p(void)
{
	BufferOutputDataSource dummy;
	BufferOutputDataSource dataSource(dummy);

	TC_ASSERT_EQ("utc_media_BufferOutputDataSource_CopyConstructor", dataSource.getChannels(), dummy.getChannels());
	TC_ASSERT_EQ("utc_media_BufferOutputDataSource_CopyConstructor", dataSource.getSampleRate(), dummy.getSampleRate());
	TC_ASSERT_EQ("utc_media_BufferOutputDataSource_CopyConstructor", dataSource.getPcmFormat(), dummy.getPcmFormat());
	TC_SUCCESS_RESULT();
}

static void utc_media_BufferOutputDataSource_EqualOperator_p(void)
{
	BufferOutputDataSource dummy;
	BufferOutputDataSource dataSource = dummy;

	TC_ASSERT_EQ("utc_media_BufferOutputDataSource_EqualOperator", dataSource.getChannels(), dummy.getChannels());
	TC_ASSERT_EQ("utc_media_BufferOutputDataSource_EqualOperator", dataSource.getSampleRate(), dummy.getSampleRate());
	TC_ASSERT_EQ("utc_media_BufferOutputDataSource_EqualOperator", dataSource.getPcmFormat(), dummy.getPcmFormat());
	TC_SUCCESS_RESULT();
}

static void utc_media_BufferOutputDataSource_open_p(void)
{
	BufferOutputDataSource dataSource;

	TC_ASSERT_EQ("utc_media_BufferOutputDataSource_open", dataSource.open(), true);
	dataSource.close();

	TC_SUCCESS_RESULT();
}

static void utc_media_BufferOutputDataSource_close_p(void)
{
	BufferOutputDataSource dataSource;

	TC_ASSERT_EQ("utc_media_BufferOutputDataSource_close", dataSource.close(), true);

	TC_SUCCESS_RESULT();
}

static void utc_media_BufferOutputDataSource_isPrepare_p(void)
{
	BufferOutputDataSource dataSource;

	dataSource.open();
	TC_ASSERT_EQ("utc_media_BufferOutputDataSource_isPrepare", dataSource.isPrepare(), true);
	dataSource.close();
	TC_SUCCESS_RESULT();
}

static void utc_media_BufferOutputDataSource_isPrepare_n(void)
{
	BufferOutputDataSource dataSource;
	TC_ASSERT_EQ("utc_media_BufferOutputDataSource_isPrepare", dataSource.isPrepare(), false);
	TC_SUCCESS_RESULT();
}

static void utc_media_BufferOutputDataSource_write_p(void)
{
	BufferOutputDataSource dataSource;
	unsigned char dummy[] = "dummy";
	ssize_t dummySize = 6;
	int count = 5;
	int i;
	dataSource.open();
	for (i = 0; i < count; i++) {
		TC_ASSERT_EQ("utc_media_BufferOutputDataSource_write", dataSource.write(dummy, (size_t)dummySize), dummySize);
	}
	dataSource.close();

	TC_SUCCESS_RESULT();
}

static void utc_media_BufferOutputDataSource_write_n(void)
{
	BufferOutputDataSource dataSource;

	dataSource.open();
	TC_ASSERT_EQ("utc_media_BufferOutputDataSource_write", dataSource.write(nullptr, 1), EOF);
	dataSource.close();

	TC_SUCCESS_RESULT();
}

int utc_media_bufferoutputdatasource_main(void)
{
	utc_media_BufferOutputDataSource_getChannels_p();
	utc_media_BufferOutputDataSource_getSampleRate_p();
	utc_media_BufferOutputDataSource_getPcmFormat_p();
	utc_media_BufferOutputDataSource_getAudioType_p();

	utc_media_BufferOutputDataSource_setChannels_p();
	utc_media_BufferOutputDataSource_setSampleRate_p();
	utc_media_BufferOutputDataSource_setPcmFormat_p();
	utc_media_BufferOutputDataSource_setAudioType_p();

	utc_media_BufferOutputDataSource_CopyConstructor_p();
	utc_media_BufferOutputDataSource_EqualOperator_p();

	utc_media_BufferOutputDataSource_open_p();

	utc_media_BufferOutputDataSource_close_p();

	utc_media_BufferOutputDataSource_isPrepare_p();
	utc_media_BufferOutputDataSource_isPrepare_n();

	utc_media_BufferOutputDataSource_write_p();
	utc_media_BufferOutputDataSource_write_n();

	return 0;
}
