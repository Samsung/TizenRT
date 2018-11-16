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

/****************************************************************************
 * Definitions
 ****************************************************************************/
#define MEDIA_TEST_FILE_PATH "/tmp/record";
#define FILE_DEFAULT_RATE 16000
#define FILE_CUSTOM_RATE 16000
#define DEFAULT_CHANNELS 2
#define CUSTOM_CHANNELS 16
#define NULL 0
#define COUNT 10

/****************************************************************************
 * Global Variables
 ****************************************************************************/
static int MEDIA_FORMAT_TYPE = AUDIO_FORMAT_TYPE_S16_LE;
static const char *file_path = MEDIA_TEST_FILE_PATH;


/**
* @testcase         itc_media_FileOutputDataSource_getChannels_setChannels_p
* @brief            set and get channels of FileOutputDataSource
* @scenario         set and get channels and check its validation
* @apicovered       getChannels,setChannels, open, close
* @precondition     NA
* @postcondition    NA
*/
static void itc_media_FileOutputDataSource_getChannels_setChannels_p(void)
{
	media::stream::FileOutputDataSource source(file_path);

	TC_ASSERT_EQ("open", source.open(), 1);
	TC_ASSERT_EQ_CLEANUP("getChannels", source.getChannels(), DEFAULT_CHANNELS, source.close());
	source.setChannels(CUSTOM_CHANNELS);

	TC_ASSERT_EQ_CLEANUP("getChannels", source.getChannels(), CUSTOM_CHANNELS, source.close());
	TC_ASSERT_EQ("close", source.close(), 1);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_media_FileOutputDataSource_getSampleRate_setSampleRate_p
* @brief            set and get sample rate of FileOutputDataSource
* @scenario         set and get sample rate and check its validation
* @apicovered       getSampleRate,setSampleRate open, close
* @precondition     NA
* @postcondition    NA
*/
static void itc_media_FileOutputDataSource_getSampleRate_setSampleRate_p(void)
{
	media::stream::FileOutputDataSource source(file_path);
	TC_ASSERT_EQ("open", source.open(), 1);
	TC_ASSERT_EQ_CLEANUP("getSampleRate", source.getSampleRate(), FILE_DEFAULT_RATE, source.close());
	source.setSampleRate(FILE_CUSTOM_RATE);
	TC_ASSERT_EQ_CLEANUP("getSampleRate", source.getSampleRate(), FILE_CUSTOM_RATE, source.close());

	TC_ASSERT_EQ("close", source.close(), 1);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_media_FileOutputDataSource_getPcmFormat_setPcmFormat_p
* @brief            set and get pcmFormat of FileOutputDataSource
* @scenario         set and get pcmFormat and check its validation
* @apicovered       setPcmFormat, getPcmFormat, open, close
* @precondition     NA
* @postcondition    NA
*/
static void itc_media_FileOutputDataSource_getPcmFormat_setPcmFormat_p(void)
{
	media::stream::FileOutputDataSource source(file_path);
	TC_ASSERT_EQ("open", source.open(), 1);
	TC_ASSERT_EQ_CLEANUP("getPcmFormat", source.getPcmFormat(), media::AUDIO_FORMAT_TYPE_S16_LE, source.close());
	source.setPcmFormat(media::AUDIO_FORMAT_TYPE_S8);
	TC_ASSERT_EQ_CLEANUP("getPcmFormat", source.getPcmFormat(), media::AUDIO_FORMAT_TYPE_S8, source.close());

	source.setPcmFormat(media::AUDIO_FORMAT_TYPE_S32_LE);
	TC_ASSERT_EQ_CLEANUP("getPcmFormat", source.getPcmFormat(), media::AUDIO_FORMAT_TYPE_S32_LE, source.close());

	TC_ASSERT_EQ("close", source.close(), 1);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_media_FileOutputDataSource_open_close_p
* @brief            open and close FileOutputDataSource
* @scenario         open and close and check its validation
* @apicovered       open, close
* @precondition     NA
* @postcondition    NA
*/
static void itc_media_FileOutputDataSource_open_close_p(void)
{
	media::stream::FileOutputDataSource source(file_path);
	for (int i = 0; i < COUNT; i++) {
		TC_ASSERT_EQ("open", source.open(), 1);
		TC_ASSERT_EQ("close", source.close(), 1);
	}

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_media_FileOutputDataSource_open_n
* @brief            open and close FileOutputDataSource
* @scenario         open multiple times and close and check its validation
* @apicovered       open, close
* @precondition     NA
* @postcondition    NA
*/
static void itc_media_FileOutputDataSource_open_n(void)
{
	media::stream::FileOutputDataSource source(file_path);
	TC_ASSERT_EQ("open", source.open(), 1);
	for (int i = 0; i < COUNT; i++) {
		TC_ASSERT_EQ("open", source.open(), 1);
	}
	TC_ASSERT_EQ("close", source.close(), 1);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_media_FileOutputDataSource_close_n
* @brief            open and close FileOutputDataSource
* @scenario         open and close multiple times and check its validation
* @apicovered       open, close
* @precondition     NA
* @postcondition    NA
*/
static void itc_media_FileOutputDataSource_close_n(void)
{
	media::stream::FileOutputDataSource source(file_path);
	TC_ASSERT_EQ("open", source.open(), 1);
	TC_ASSERT_EQ("close", source.close(), 1);
	for (int i = 0; i < COUNT; i++) {
		TC_ASSERT_EQ("close", source.close(), 0);
	}

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_media_FileOutputDataSource_isPrepare_p
* @brief            open, isPrepare and close for FileOutputDataSource
* @scenario         open, isPrepare and close and check its validation
* @apicovered       open, close, isPrepare
* @precondition     NA
* @postcondition    NA
*/
static void itc_media_FileOutputDataSource_isPrepare_p(void)
{
	media::stream::FileOutputDataSource source(file_path);
	TC_ASSERT_EQ("open", source.open(), 1);
	TC_ASSERT_EQ_CLEANUP("isPrepare", source.isPrepare(), 1, source.close());
	TC_ASSERT_EQ("close", source.close(), 1);

	TC_SUCCESS_RESULT();
}
/**
* @testcase         itc_media_FileOutputDataSource_isPrepare_n
* @brief            open, close and isPrepare for FileOutputDataSource
* @scenario         open, close and isPrepare and  check its validation
* @apicovered       open, close, isPrepare
* @precondition     NA
* @postcondition    NA
*/
static void itc_media_FileOutputDataSource_isPrepare_n(void)
{
	media::stream::FileOutputDataSource source(file_path);
	TC_ASSERT_EQ("open", source.open(), 1);
	TC_ASSERT_EQ("close", source.close(), 1);
	TC_ASSERT_EQ("isPrepare", source.isPrepare(), 0);

	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_media_FileInputDataSource_read_p
* @brief            open, write and close FileOutputDataSource
* @scenario         open, write and close and check its validation
* @apicovered       open, write, close
* @precondition     NA
* @postcondition    NA
*/
static void itc_media_FileOutputDataSource_write_p(void)
{
	media::stream::FileOutputDataSource source(file_path);
	unsigned char buf[] = "dummydata";
	TC_ASSERT_EQ("open", source.open(), 1);
	TC_ASSERT_EQ_CLEANUP("write", source.write(buf, 9), 9, source.close());
	TC_ASSERT_EQ_CLEANUP("write", source.write(buf, 0), 0, source.close());
	TC_ASSERT_EQ_CLEANUP("write", source.write(buf, 10), 10, source.close());
	TC_ASSERT_EQ_CLEANUP("write", source.write(buf, 24), 24, source.close());
	TC_ASSERT_EQ_CLEANUP("write", source.write(buf, 31), 31, source.close());
	TC_ASSERT_EQ("close", source.close(), 1);

	TC_SUCCESS_RESULT();
}
/**
* @testcase         itc_media_FileOutputDataSource_write_n
* @brief            open, write and close FileOutputDataSource
* @scenario         open, write with invalid parameters and close and check its validation
* @apicovered       open, write, close
* @precondition     NA
* @postcondition    NA
*/
static void itc_media_FileOutputDataSource_write_n(void)
{
	media::stream::FileOutputDataSource source(file_path);
	unsigned char buf[] = "dummydata";
	TC_ASSERT_EQ("open", source.open(), 1);
	TC_ASSERT_EQ_CLEANUP("write", source.write(nullptr, 1), EOF, source.close());
	TC_ASSERT_EQ("close", source.close(), 1);

	TC_SUCCESS_RESULT();
}

int itc_media_fileoutputdatasource_main(void)
{
	itc_media_FileOutputDataSource_getChannels_setChannels_p();
	itc_media_FileOutputDataSource_getSampleRate_setSampleRate_p();
	itc_media_FileOutputDataSource_getPcmFormat_setPcmFormat_p();
	itc_media_FileOutputDataSource_open_close_p();
	itc_media_FileOutputDataSource_open_n();
	itc_media_FileOutputDataSource_close_n();
	itc_media_FileOutputDataSource_isPrepare_p();
	itc_media_FileOutputDataSource_isPrepare_n();
	itc_media_FileOutputDataSource_write_p();
	itc_media_FileOutputDataSource_write_n();

	return 0;
}
