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

#include <media/FileInputDataSource.h>
#include "tc_common.h"

static const char dummyfilepath[] = "/mnt/fileinputdatasource.raw";
static const char testData[] = "dummydata";
static unsigned char *buf;

static void SetUp(void)
{
	FILE *fp = fopen(dummyfilepath, "w");
	fputs(testData, fp);
	fclose(fp);

	buf = new unsigned char[21];
}

static void TearDown()
{
	remove(dummyfilepath);

	delete buf;
}

static void utc_media_FileInputDataSource_getChannels_p(void)
{
	media::stream::FileInputDataSource source(dummyfilepath);
	TC_ASSERT_EQ("utc_media_FileInputDataSource_getChannels", source.getChannels(), 2);

	TC_SUCCESS_RESULT();
}

static void utc_media_FileInputDataSource_getSampleRate_p(void)
{
	media::stream::FileInputDataSource source(dummyfilepath);
	TC_ASSERT_EQ("utc_media_FileInputDataSource_getSampleRate", source.getSampleRate(), 16000);

	TC_SUCCESS_RESULT();
}

static void utc_media_FileInputDataSource_getPcmFormat_p(void)
{
	media::stream::FileInputDataSource source(dummyfilepath);

	TC_ASSERT_EQ("utc_media_FileInputDataSource_getPcmFormat", source.getPcmFormat(), media::AUDIO_FORMAT_TYPE_S16_LE);

	TC_SUCCESS_RESULT();
}

static void utc_media_FileInputDataSource_setChannels_p(void)
{
	media::stream::FileInputDataSource source(dummyfilepath);
	source.setChannels(1);

	TC_ASSERT_EQ("utc_media_FileInputDataSource_setChannels", source.getChannels(), 1);

	TC_SUCCESS_RESULT();
}

static void utc_media_FileInputDataSource_setSampleRate_p(void)
{
	media::stream::FileInputDataSource source(dummyfilepath);
	source.setSampleRate(44100);

	TC_ASSERT_EQ("utc_media_FileInputDataSource_setSampleRate", source.getSampleRate(), 44100);

	TC_SUCCESS_RESULT();
}

static void utc_media_FileInputDataSource_setPcmFormat_p(void)
{
	media::stream::FileInputDataSource source(dummyfilepath);
	source.setPcmFormat(media::AUDIO_FORMAT_TYPE_S8);

	TC_ASSERT_EQ("utc_media_FileInputDataSource_setPcmFormat", source.getPcmFormat(), media::AUDIO_FORMAT_TYPE_S8);

	TC_SUCCESS_RESULT();
}

static void utc_media_FileInputDataSource_open_p(void)
{
	media::stream::FileInputDataSource source(dummyfilepath);

	TC_ASSERT_EQ("utc_media_FileInputDataSource_open", source.open(), true);

	source.close();
	TC_SUCCESS_RESULT();
}

static void utc_media_FileInputDataSource_open_n(void)
{
	media::stream::FileInputDataSource source("non-exist-file");

	TC_ASSERT_EQ("utc_media_FileInputDataSource_open", source.open(), false);

	TC_SUCCESS_RESULT();
}

static void utc_media_FileInputDataSource_close_p(void)
{
	media::stream::FileInputDataSource source(dummyfilepath);
	source.open();

	TC_ASSERT_EQ("utc_media_FileInputDataSource_close", source.close(), true);

	TC_SUCCESS_RESULT();
}

static void utc_media_FileInputDataSource_close_n(void)
{
	media::stream::FileInputDataSource source(dummyfilepath);

	TC_ASSERT_EQ("utc_media_FileInputDataSource_close", source.close(), false);

	TC_SUCCESS_RESULT();
}

static void utc_media_FileInputDataSource_isPrepare_p(void)
{
	media::stream::FileInputDataSource source(dummyfilepath);
	source.open();

	TC_ASSERT_EQ("utc_media_FileInputDataSource_isPrepare", source.isPrepare(), true);

	source.close();
	TC_SUCCESS_RESULT();
}

static void utc_media_FileInputDataSource_isPrepare_n(void)
{
	media::stream::FileInputDataSource source(dummyfilepath);

	TC_ASSERT_EQ("utc_media_FileInputDataSource_isPrepare", source.isPrepare(), false);

	TC_SUCCESS_RESULT();
}

static void utc_media_FileInputDataSource_read_p(void)
{
	media::stream::FileInputDataSource source(dummyfilepath);
	memset(buf, 0, 21);
	source.open();

	TC_ASSERT_EQ("utc_media_FileInputDataSource_read", source.read(buf, 100), (ssize_t)strlen(testData));

	source.close();
	TC_SUCCESS_RESULT();
}

static void utc_media_FileInputDataSource_read_n(void)
{
	media::stream::FileInputDataSource source(dummyfilepath);
	memset(buf, 0, 21);

	TC_ASSERT_EQ("utc_media_FileInputDataSource_read", source.read(buf, 100), 0);

	TC_SUCCESS_RESULT();
}

static void utc_media_FileInputDataSource_readAt_p(void)
{
	media::stream::FileInputDataSource source(dummyfilepath);
	memset(buf, 0, 21);
	source.open();

	TC_ASSERT_EQ("utc_media_FileInputDataSource_readAt", source.readAt(1, 0, buf, 100), (int)strlen(testData + 1));

	source.close();
	TC_SUCCESS_RESULT();
}

static void utc_media_FileInputDataSource_readAt_n(void)
{
	media::stream::FileInputDataSource source(dummyfilepath);
	memset(buf, 0, 21);

	TC_ASSERT_LEQ("utc_media_FileInputDataSource_readAt", source.readAt(1, 0, buf, 100), 0);

	TC_SUCCESS_RESULT();
}

int utc_media_FileInputDataSource_main(void)
{
	SetUp();
	utc_media_FileInputDataSource_getChannels_p();
	utc_media_FileInputDataSource_getSampleRate_p();
	utc_media_FileInputDataSource_getPcmFormat_p();
	utc_media_FileInputDataSource_setChannels_p();
	utc_media_FileInputDataSource_setSampleRate_p();
	utc_media_FileInputDataSource_setPcmFormat_p();
	utc_media_FileInputDataSource_open_p();
	utc_media_FileInputDataSource_open_n();
	utc_media_FileInputDataSource_close_p();
	utc_media_FileInputDataSource_close_n();
	utc_media_FileInputDataSource_isPrepare_p();
	utc_media_FileInputDataSource_isPrepare_n();
	utc_media_FileInputDataSource_read_p();
	utc_media_FileInputDataSource_read_n();
	utc_media_FileInputDataSource_readAt_p();
	utc_media_FileInputDataSource_readAt_n();
	TearDown();
	return 0;
}
