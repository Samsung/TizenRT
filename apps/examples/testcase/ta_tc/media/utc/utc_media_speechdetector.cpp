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

#include <unistd.h>
#include <media/voice/SpeechDetector.h>

#include "tc_common.h"

#define TEST_SAMPLE_RATE 16000
#define TEST_CHANNELS 2

static void utc_media_SpeechDetector_instance_p(void)
{
	auto p1 = media::voice::SpeechDetector::instance();
	auto p2 = media::voice::SpeechDetector::instance();
	TC_ASSERT_EQ("utc_media_SpeechDetector_instance", p1, p2);
	TC_SUCCESS_RESULT();
}

static void utc_media_SpeechDetector_initKeywordDetect_p(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	bool ret = instance->initKeywordDetect(TEST_SAMPLE_RATE, TEST_CHANNELS);
	TC_ASSERT_EQ("utc_media_SpeechDetector_initKeywordDetect", ret, true);
	TC_SUCCESS_RESULT();
	instance->deinitKeywordDetect();
}

static void utc_media_SpeechDetector_initKeywordDetect_n(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	bool ret = instance->initKeywordDetect(0, TEST_CHANNELS);
	TC_ASSERT_EQ("utc_media_SpeechDetector_initKeywordDetect", ret, false);
	TC_SUCCESS_RESULT();
}

static void utc_media_SpeechDetector_initEndPointDetect_p(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	bool ret = instance->initEndPointDetect(TEST_SAMPLE_RATE, TEST_CHANNELS);
	TC_ASSERT_EQ("utc_media_SpeechDetector_initEndPointDetect", ret, true);
	TC_SUCCESS_RESULT();
	instance->deinitEndPointDetect();
}

static void utc_media_SpeechDetector_initEndPointDetect_n(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	bool ret = instance->initEndPointDetect(0, TEST_CHANNELS);
	TC_ASSERT_EQ("utc_media_SpeechDetector_initEndPointDetect", ret, false);
	TC_SUCCESS_RESULT();
}

static void utc_media_SpeechDetector_deinitKeywordDetect_p(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	instance->initKeywordDetect(TEST_SAMPLE_RATE, TEST_CHANNELS);
	bool ret = instance->deinitKeywordDetect();
	TC_ASSERT_EQ("utc_media_SpeechDetector_deinitKeywordDetect", ret, true);
	TC_SUCCESS_RESULT();
}

static void utc_media_SpeechDetector_deinitKeywordDetect_n(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	bool ret = instance->deinitKeywordDetect();
	TC_ASSERT_EQ("utc_media_SpeechDetector_deinitKeywordDetect", ret, false);
	TC_SUCCESS_RESULT();
}

static void utc_media_SpeechDetector_deinitEndPointDetect_p(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	instance->initEndPointDetect(TEST_SAMPLE_RATE, TEST_CHANNELS);
	bool ret = instance->deinitEndPointDetect();
	TC_ASSERT_EQ("utc_media_SpeechDetector_deinitEndPointDetect", ret, true);
	TC_SUCCESS_RESULT();
}

static void utc_media_SpeechDetector_deinitEndPointDetect_n(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	bool ret = instance->deinitEndPointDetect();
	TC_ASSERT_EQ("utc_media_SpeechDetector_deinitEndPointDetect", ret, false);
	TC_SUCCESS_RESULT();
}

static void utc_media_SpeechDetector_startKeywordDetect_p(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	instance->initKeywordDetect(TEST_SAMPLE_RATE, TEST_CHANNELS);
	printf("Do not speak keyword\n");
	instance->startKeywordDetect(0);
	printf("Now, speak keyword\n");
	bool ret = instance->startKeywordDetect(-1);
	instance->deinitKeywordDetect();
	TC_ASSERT_EQ("utc_media_SpeechDetector_startKeywordDetect", ret, true);

	TC_SUCCESS_RESULT();
}

static void utc_media_SpeechDetector_startKeywordDetect_n(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	bool ret = instance->startKeywordDetect(0);
	TC_ASSERT_EQ("utc_media_SpeechDetector_startKeywordDetect", ret, false);
	TC_SUCCESS_RESULT();
}

static void utc_media_SpeechDetector_startEndPointDetect_p(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	instance->initKeywordDetect(TEST_SAMPLE_RATE, TEST_CHANNELS);
	instance->initEndPointDetect(TEST_SAMPLE_RATE, TEST_CHANNELS);
	printf("Now, speak keyword\n");
	instance->startKeywordDetect(-1);
	printf("Do not stop to speak\n");
	instance->startEndPointDetect(0);
	printf("Now, stop speaking\n");
	bool ret = instance->startEndPointDetect(-1);
	instance->deinitKeywordDetect();
	instance->deinitEndPointDetect();
	TC_ASSERT_EQ("utc_media_SpeechDetector_startEndPointDetect", ret, true);

	TC_SUCCESS_RESULT();
}

static void utc_media_SpeechDetector_startEndPointDetect_n(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	bool ret = instance->startEndPointDetect(0);
	TC_ASSERT_EQ("utc_media_SpeechDetector_startEndPointDetect", ret, false);
	TC_SUCCESS_RESULT();
}

static void *thread_waitEndPoint(void *arg)
{
	size_t size = 8196;
	short *noise = (short *)malloc(size * sizeof(short));
	short *silence = (short *)calloc(size, sizeof(short));
	auto instance = media::voice::SpeechDetector::instance();
	sleep(1);
	instance->detectEndPoint(noise, size);
	while (instance->detectEndPoint(silence, size) == false) {
		// do nothing
	}

	free(noise);
	free(silence);
	return NULL;
}

static void utc_media_SpeechDetector_detectEndPoint_p(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	pthread_t pid;
	instance->initEndPointDetect(TEST_SAMPLE_RATE, TEST_CHANNELS);
	pthread_create(&pid, NULL, thread_waitEndPoint, NULL);
	bool ret = instance->waitEndPoint(-1);
	pthread_join(pid, NULL);
	instance->deinitEndPointDetect();
	TC_ASSERT_EQ("utc_media_SpeechDetector_detectEndPoint", ret, true);

	TC_SUCCESS_RESULT();
}

static void utc_media_SpeechDetector_detectEndPoint_n(void)
{
	/* detectEndPoint without init */
	{
		auto instance = media::voice::SpeechDetector::instance();
		bool ret = instance->detectEndPoint(nullptr, 0);
		TC_ASSERT_EQ("utc_media_SpeechDetector_waitEndPoint", ret, false);
	}

	/* invalid param */
	{
		auto instance = media::voice::SpeechDetector::instance();
		instance->initEndPointDetect(TEST_SAMPLE_RATE, TEST_CHANNELS);
		bool ret = instance->detectEndPoint(nullptr, 0);
		instance->deinitEndPointDetect();
		TC_ASSERT_EQ("utc_media_SpeechDetector_detectEndPoint", ret, false);
	}

	TC_SUCCESS_RESULT();
}

static void utc_media_SpeechDetector_waitEndPoint_p(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	pthread_t pid;
	instance->initEndPointDetect(TEST_SAMPLE_RATE, TEST_CHANNELS);
	pthread_create(&pid, NULL, thread_waitEndPoint, NULL);
	bool ret = instance->waitEndPoint(-1);
	pthread_join(pid, NULL);
	instance->deinitEndPointDetect();
	TC_ASSERT_EQ("utc_media_SpeechDetector_waitEndPoint", ret, true);

	TC_SUCCESS_RESULT();
}

static void utc_media_SpeechDetector_waitEndPoint_n(void)
{
	/* waitEndPoint without init */
	{
		auto instance = media::voice::SpeechDetector::instance();
		bool ret = instance->waitEndPoint(0);
		TC_ASSERT_EQ("utc_media_SpeechDetector_waitEndPoint", ret, false);
	}

	/* waitEndPoint, but timeout */
	{
		auto instance = media::voice::SpeechDetector::instance();
		instance->initEndPointDetect(TEST_SAMPLE_RATE, TEST_CHANNELS);
		bool ret = instance->waitEndPoint(0);
		instance->deinitEndPointDetect();
		TC_ASSERT_EQ("utc_media_SpeechDetector_waitEndPoint", ret, false);
	}

	TC_SUCCESS_RESULT();
}

int utc_media_SpeechDetector_main(void)
{
	utc_media_SpeechDetector_instance_p();
	utc_media_SpeechDetector_initKeywordDetect_p();
	utc_media_SpeechDetector_initKeywordDetect_n();
	utc_media_SpeechDetector_initEndPointDetect_p();
	utc_media_SpeechDetector_initEndPointDetect_n();
	utc_media_SpeechDetector_deinitKeywordDetect_p();
	utc_media_SpeechDetector_deinitKeywordDetect_n();
	utc_media_SpeechDetector_deinitEndPointDetect_p();
	utc_media_SpeechDetector_deinitEndPointDetect_n();
	utc_media_SpeechDetector_startKeywordDetect_p();
	utc_media_SpeechDetector_startKeywordDetect_n();
	utc_media_SpeechDetector_startEndPointDetect_p();
	utc_media_SpeechDetector_startEndPointDetect_n();
	utc_media_SpeechDetector_detectEndPoint_p();
	utc_media_SpeechDetector_detectEndPoint_n();
	utc_media_SpeechDetector_waitEndPoint_p();
	utc_media_SpeechDetector_waitEndPoint_n();
	return 0;
}
