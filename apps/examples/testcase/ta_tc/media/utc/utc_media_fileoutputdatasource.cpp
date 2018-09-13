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

#include <media/FileOutputDataSource.h>
#include "tc_common.h"
#include <errno.h>

using namespace std;
using namespace media;
using namespace media::stream;

static unsigned int channels = 2;
static unsigned int sampleRate = 16000;
static audio_format_type_t pcmFormat = AUDIO_FORMAT_TYPE_S16_LE;
static const char *filePath = "/tmp/record";

static void utc_media_FileOutputDataSource_getChannels_p(void)
{
	FileOutputDataSource dataSource(channels, sampleRate, pcmFormat, filePath);
	TC_ASSERT_EQ("utc_media_FileOutputDataSource_getChannels", dataSource.getChannels(), channels);
	TC_SUCCESS_RESULT();
}

static void utc_media_FileOutputDataSource_getSampleRate_p(void)
{
	FileOutputDataSource dataSource(channels, sampleRate, pcmFormat, filePath);
	TC_ASSERT_EQ("utc_media_FileOutputDataSource_getSampleRate", dataSource.getSampleRate(), sampleRate);
	TC_SUCCESS_RESULT();
}

static void utc_media_FileOutputDataSource_getPcmFormat_p(void)
{
	FileOutputDataSource dataSource(channels, sampleRate, pcmFormat, filePath);
	TC_ASSERT_EQ("utc_media_FileOutputDataSource_getPcmFormat", dataSource.getPcmFormat(), pcmFormat);
	TC_SUCCESS_RESULT();
}

static void utc_media_FileOutputDataSource_getAudioType_p(void)
{
	FileOutputDataSource dataSource(channels, sampleRate, pcmFormat, filePath);
	TC_ASSERT_EQ("utc_media_FileOutputDataSource_getAudioType", dataSource.getAudioType(), media::AUDIO_TYPE_INVALID);
	TC_SUCCESS_RESULT();
}

static void utc_media_FileOutputDataSource_setChannels_p(void)
{
	FileOutputDataSource dataSource(filePath);
	unsigned int compare_channels = 3;
	dataSource.setChannels(compare_channels);
	TC_ASSERT_EQ("utc_media_FileOutputDataSource_setChannels", dataSource.getChannels(), compare_channels);
	TC_SUCCESS_RESULT();
}

static void utc_media_FileOutputDataSource_setSampleRate_p(void)
{
	FileOutputDataSource dataSource(filePath);
	unsigned int compare_sampleRate = 32000;
	dataSource.setSampleRate(compare_sampleRate);
	TC_ASSERT_EQ("utc_media_FileOutputDataSource_setSampleRate", dataSource.getSampleRate(), compare_sampleRate);
	TC_SUCCESS_RESULT();
}

static void utc_media_FileOutputDataSource_setPcmFormat_p(void)
{
	FileOutputDataSource dataSource(filePath);
	media::audio_format_type_t compare_pcmFormat = media::AUDIO_FORMAT_TYPE_S8;
	dataSource.setPcmFormat(media::AUDIO_FORMAT_TYPE_S8);
	TC_ASSERT_EQ("utc_media_FileOutputDataSource_setPcmFormat", dataSource.getPcmFormat(), compare_pcmFormat);
	TC_SUCCESS_RESULT();
}

static void utc_media_FileOutputDataSource_setAudioType_p(void)
{
	FileOutputDataSource dataSource(filePath);
	media::audio_type_t compare_audioType = media::AUDIO_TYPE_MP3;
	dataSource.setAudioType(media::AUDIO_TYPE_MP3);
	TC_ASSERT_EQ("utc_media_FileOutputDataSource_setAudioType", dataSource.getAudioType(), compare_audioType);
	TC_SUCCESS_RESULT();
}

static void utc_media_FileOutputDataSource_CopyConstructor_p(void)
{
	FileOutputDataSource dummy(filePath);
	FileOutputDataSource dataSource(dummy);

	TC_ASSERT_EQ("utc_media_FileOutputDataSource_CopyConstructor", dataSource.getChannels(), dummy.getChannels());
	TC_ASSERT_EQ("utc_media_FileOutputDataSource_CopyConstructor", dataSource.getSampleRate(), dummy.getSampleRate());
	TC_ASSERT_EQ("utc_media_FileOutputDataSource_CopyConstructor", dataSource.getPcmFormat(), dummy.getPcmFormat());
	TC_SUCCESS_RESULT();
}

static void utc_media_FileOutputDataSource_EqualOperator_p(void)
{
	FileOutputDataSource dummy(filePath);
	FileOutputDataSource dataSource = dummy;

	TC_ASSERT_EQ("utc_media_FileOutputDataSource_EqualOperator", dataSource.getChannels(), dummy.getChannels());
	TC_ASSERT_EQ("utc_media_FileOutputDataSource_EqualOperator", dataSource.getSampleRate(), dummy.getSampleRate());
	TC_ASSERT_EQ("utc_media_FileOutputDataSource_EqualOperator", dataSource.getPcmFormat(), dummy.getPcmFormat());
	TC_SUCCESS_RESULT();
}

static void utc_media_FileOutputDataSource_open_p(void)
{
	FileOutputDataSource dataSource(filePath);

	TC_ASSERT_EQ("utc_media_FileOutputDataSource_open", dataSource.open(), true);
	dataSource.close();

	TC_SUCCESS_RESULT();
}

static void utc_media_FileOutputDataSource_open_n(void)
{
	FileOutputDataSource dataSource("non-exist-file");

	TC_ASSERT_EQ("utc_media_FileOutputDataSource_open", dataSource.open(), false);

	TC_SUCCESS_RESULT();
}

static void utc_media_FileOutputDataSource_close_p(void)
{
	FileOutputDataSource dataSource(filePath);

	dataSource.open();
	TC_ASSERT_EQ("utc_media_FileOutputDataSource_close", dataSource.close(), true);

	TC_SUCCESS_RESULT();
}

static void utc_media_FileOutputDataSource_close_n(void)
{
	FileOutputDataSource dataSource(filePath);

	TC_ASSERT_EQ("utc_media_FileOutputDataSource_close", dataSource.close(), false);
	TC_SUCCESS_RESULT();
}

static void utc_media_FileOutputDataSource_isPrepare_p(void)
{
	FileOutputDataSource dataSource(filePath);
	dataSource.open();
	TC_ASSERT_EQ("utc_media_FileOutputDataSource_isPrepare", dataSource.isPrepare(), true);
	dataSource.close();
	TC_SUCCESS_RESULT();
}

static void utc_media_FileOutputDataSource_isPrepare_n(void)
{
	FileOutputDataSource dataSource(filePath);
	TC_ASSERT_EQ("utc_media_FileOutputDataSource_isPrepare", dataSource.isPrepare(), false);
	TC_SUCCESS_RESULT();
}

static void utc_media_FileOutputDataSource_write_p(void)
{
	FileOutputDataSource dataSource(filePath);
	unsigned char dummy[] = "dummy";
	ssize_t dummySize = 6;

	dataSource.open();
	TC_ASSERT_EQ("utc_media_FileOutputDataSource_write", dataSource.write(dummy, dummySize), dummySize);
	dataSource.close();

	TC_SUCCESS_RESULT();
}

static void utc_media_FileOutputDataSource_write_n(void)
{
	FileOutputDataSource dataSource(filePath);

	dataSource.open();
	TC_ASSERT_EQ("utc_media_FileOutputDataSource_write", dataSource.write(nullptr, 1), EOF);
	dataSource.close();

	TC_SUCCESS_RESULT();
}

int utc_media_fileoutputdatasource_main(void)
{
	utc_media_FileOutputDataSource_getChannels_p();
	utc_media_FileOutputDataSource_getSampleRate_p();
	utc_media_FileOutputDataSource_getPcmFormat_p();
	utc_media_FileOutputDataSource_getAudioType_p();

	utc_media_FileOutputDataSource_setChannels_p();
	utc_media_FileOutputDataSource_setSampleRate_p();
	utc_media_FileOutputDataSource_setPcmFormat_p();
	utc_media_FileOutputDataSource_setAudioType_p();

	utc_media_FileOutputDataSource_CopyConstructor_p();
	utc_media_FileOutputDataSource_EqualOperator_p();

	utc_media_FileOutputDataSource_open_p();
	utc_media_FileOutputDataSource_open_n();

	utc_media_FileOutputDataSource_close_p();
	utc_media_FileOutputDataSource_close_n();

	utc_media_FileOutputDataSource_isPrepare_p();
	utc_media_FileOutputDataSource_isPrepare_n();

	utc_media_FileOutputDataSource_write_p();
	utc_media_FileOutputDataSource_write_n();

	return 0;
}
