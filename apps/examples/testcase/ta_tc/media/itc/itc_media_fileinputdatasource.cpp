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

/****************************************************************************
 * Definitions
 ****************************************************************************/
#define MEDIA_TEST_FILE_PATH "/mnt/fileinputdatasource.raw";
#define FILE_DEFAULT_RATE 16000
#define MEDIA_FORMAT_TYPE 0
#define COUNT 10

/****************************************************************************
 * Global Variables
 ****************************************************************************/
static const char *file_path = MEDIA_TEST_FILE_PATH;
static const char *test_data = "dummydata";
static int g_flag=0;
static unsigned char *buf;

static void SetUp(void)
{
	FILE *fp;
	fp = fopen(file_path, "w");
	TC_ASSERT_NEQ("fopen", fp, NULL);
	TC_ASSERT_NEQ_CLEANUP("fputs", fputs(test_data, fp), EOF, fclose(fp));
	TC_ASSERT_EQ("fclose", fclose(fp), OK);
	buf = new unsigned char[31];
	g_flag = 1;
}

static void TearDown()
{
	remove(file_path);
	delete(buf);
}

/**
* @testcase         itc_media_FileInputDataSource_getChannels_setChannels_p
* @brief            set and get channels of FileInputDataSource
* @scenario         set and get channels and check its validation
* @apicovered       getChannels,setChannels, open, close
* @precondition     NA
* @postcondition    NA
*/
static void itc_media_FileInputDataSource_getChannels_setChannels_p(void)
{
	media::stream::FileInputDataSource source(file_path);
	TC_ASSERT_EQ("open", source.open(), 1);
	TC_ASSERT_EQ("getChannels", source.getChannels(), 2);
	source.setChannels(16);
	TC_ASSERT_EQ("getChannels", source.getChannels(), 16);
	TC_ASSERT_EQ("close", source.close(), 1);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_media_FileInputDataSource_getSampleRate_setSampleRate_p
* @brief            set and get sample rate of FileInputDataSource
* @scenario         set and get sample rate and check its validation
* @apicovered       getSampleRate,setSampleRate open, close
* @precondition     NA
* @postcondition    NA
*/
static void itc_media_FileInputDataSource_getSampleRate_setSampleRate_p(void)
{
	media::stream::FileInputDataSource source(file_path);
	TC_ASSERT_EQ("open", source.open(), 1);
	TC_ASSERT_EQ("getSampleRate", source.getSampleRate(), FILE_DEFAULT_RATE);
	source.setSampleRate(20000);
	TC_ASSERT_EQ("getSampleRate", source.getSampleRate(), 20000);
	TC_ASSERT_EQ("close", source.close(), 1);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_media_FileInputDataSource_getPcmFormat_setPcmFormat_p
* @brief            set and get pcmFormat of FileInputDataSource
* @scenario         set and get pcmFormat and check its validation
* @apicovered       setPcmFormat, getPcmFormat, open, close
* @precondition     NA
* @postcondition    NA
*/
static void itc_media_FileInputDataSource_getPcmFormat_setPcmFormat_p(void)
{
	media::stream::FileInputDataSource source(file_path);
	TC_ASSERT_EQ("open", source.open(), 1);
	TC_ASSERT_EQ("getPcmFormat", source.getPcmFormat(), MEDIA_FORMAT_TYPE);
	source.setPcmFormat(1);
	TC_ASSERT_EQ("getPcmFormat", source.getPcmFormat(), 1);
	source.setPcmFormat(2);
	TC_ASSERT_EQ("getPcmFormat", source.getPcmFormat(), 2);
	TC_ASSERT_EQ("close", source.close(), 1);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_media_FileInputDataSource_open_close_p
* @brief            open and close FileInputDataSource
* @scenario         open and close and check its validation
* @apicovered       open, close
* @precondition     NA
* @postcondition    NA
*/
static void itc_media_FileInputDataSource_open_close_p(void)
{
	media::stream::FileInputDataSource source(file_path);
	for(int i=0; i<COUNT; i++){
		TC_ASSERT_EQ("open", source.open(), 1);
		TC_ASSERT_EQ("close", source.close(), 1);
	}
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_media_FileInputDataSource_open_n
* @brief            open and close FileInputDataSource
* @scenario         open multiple times and close and check its validation
* @apicovered       open, close
* @precondition     NA
* @postcondition    NA
*/
static void itc_media_FileInputDataSource_open_n(void)
{
	media::stream::FileInputDataSource source(file_path);
	TC_ASSERT_EQ("open", source.open(), 1);
	for(int i=0; i<COUNT; i++){
		TC_ASSERT_EQ("open", source.open(), 0);
	}
	TC_ASSERT_EQ("close", source.close(), 1);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_media_FileInputDataSource_close_n
* @brief            open and close FileInputDataSource
* @scenario         open and close multiple times and check its validation
* @apicovered       open, close
* @precondition     NA
* @postcondition    NA
*/
static void itc_media_FileInputDataSource_close_n(void)
{
	media::stream::FileInputDataSource source(file_path);
	TC_ASSERT_EQ("open", source.open(), 1);
	TC_ASSERT_EQ("close", source.close(), 1);
	for(int i=0; i<COUNT; i++){
		TC_ASSERT_EQ("close", source.close(), 0);
	}
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_media_FileInputDataSource_isPrepare_p
* @brief            open, isPrepare and close for FileInputDataSource
* @scenario         open, isPrepare and close and check its validation
* @apicovered       open, close, isPrepare
* @precondition     NA
* @postcondition    NA
*/
static void itc_media_FileInputDataSource_isPrepare_p(void)
{
	media::stream::FileInputDataSource source(file_path);
	TC_ASSERT_EQ("open", source.open(), 1);
	TC_ASSERT_EQ("isPrepare", source.isPrepare(), 1);
	TC_ASSERT_EQ("close", source.close(), 1);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_media_FileInputDataSource_isPrepare_n
* @brief            open, close and isPrepare for FileInputDataSource
* @scenario         open, close and isPrepare and  check its validation
* @apicovered       open, close, isPrepare
* @precondition     NA
* @postcondition    NA
*/
static void itc_media_FileInputDataSource_isPrepare_n(void)
{
	media::stream::FileInputDataSource source(file_path);
	TC_ASSERT_EQ("open", source.open(), 1);
	TC_ASSERT_EQ("close", source.close(), 1);
	TC_ASSERT_EQ("isPrepare", source.isPrepare(), 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_media_FileInputDataSource_read_p
* @brief            open, readAt and close FileInputDataSource
* @scenario         open, readAt and close and check its validation
* @apicovered       open, close
* @precondition     NA
* @postcondition    NA
*/
static void itc_media_FileInputDataSource_read_p(void)
{
	media::stream::FileInputDataSource source(file_path);
	memset(buf, 0, 31);
	TC_ASSERT_EQ("open", source.open(), 1);
	TC_ASSERT_EQ("read", source.read(buf, 100), strlen(test_data));
	TC_ASSERT_EQ("readAt", source.readAt(1, 0, buf, 100), strlen(test_data + 1));
	TC_ASSERT_EQ("readAt", source.readAt(0, 0, buf, 100), strlen(test_data));
	TC_ASSERT_EQ("close", source.close(), 1);
	TC_SUCCESS_RESULT();
}

/**
* @testcase         itc_media_FileInputDataSource_read_n
* @brief            open, readAt and close FileInputDataSource
* @scenario         open, readAt with invalid parameters and close and check its validation
* @apicovered       open,readAt, close
* @precondition     NA
* @postcondition    NA
*/
static void itc_media_FileInputDataSource_read_n(void)
{
	media::stream::FileInputDataSource source(file_path);
	memset(buf, 0, 31);
	TC_ASSERT_EQ("open", source.open(), 1);
	TC_ASSERT_EQ("readAt", source.readAt(0, 36, buf, 100), -1);
	TC_ASSERT_EQ("readAt", source.readAt(36, 36, buf, 100), -1);
	TC_ASSERT_EQ("readAt", source.readAt(0, strlen(test_data), buf, 100), -1);
	TC_ASSERT_EQ("close", source.close(), 1);
	TC_SUCCESS_RESULT();
}

int itc_media_FileInputDataSource_main(void)
{
	SetUp();
	if(g_flag){
		itc_media_FileInputDataSource_getChannels_setChannels_p();
		itc_media_FileInputDataSource_getSampleRate_setSampleRate_p();
		itc_media_FileInputDataSource_getPcmFormat_setPcmFormat_p();
		itc_media_FileInputDataSource_open_close_p();
		itc_media_FileInputDataSource_open_n();
		itc_media_FileInputDataSource_close_n();
		itc_media_FileInputDataSource_isPrepare_p();
		itc_media_FileInputDataSource_isPrepare_n();
		itc_media_FileInputDataSource_read_p();
		itc_media_FileInputDataSource_read_n();

	}
	TearDown();
	return 0;
}
