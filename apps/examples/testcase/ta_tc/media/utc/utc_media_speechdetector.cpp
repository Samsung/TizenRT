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
#include <media/voice/SpeechDetectorListenerInterface.h>

#include "tc_common.h"

#define TEST_SAMPLE_RATE 16000
#define TEST_CHANNELS 2

class SpeechDetectorListener : public media::voice::SpeechDetectorListenerInterface, public std::enable_shared_from_this<SpeechDetectorListener>
{
public:
	void onSpeechDetectionListener(media::voice::speech_detect_event_type_e event) override
	{

	}
};

static void utc_media_SpeechDetector_instance_p(void)
{
	media::voice::SpeechDetector *instance1 = media::voice::SpeechDetector::instance();
	media::voice::SpeechDetector *instance2 = media::voice::SpeechDetector::instance();
	TC_ASSERT_EQ("utc_media_SpeechDetector_instance_p", instance1, instance2);
	TC_SUCCESS_RESULT();
	sleep(1);
}

static void utc_media_SpeechDetector_initKeywordDetect_p(void)
{
	media::voice::SpeechDetector *instance = media::voice::SpeechDetector::instance();
	bool ret = instance->initKeywordDetect(TEST_SAMPLE_RATE, TEST_CHANNELS);
	instance->deinitKeywordDetect();
	TC_ASSERT_EQ("utc_media_SpeechDetector_initKeywordDetect_p", ret, true);
	TC_SUCCESS_RESULT();
	sleep(1);
}

static void utc_media_SpeechDetector_initKeywordDetect_n(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	bool ret = instance->initKeywordDetect(0, TEST_CHANNELS);
	TC_ASSERT_EQ("utc_media_SpeechDetector_initKeywordDetect_n", ret, false);
	TC_SUCCESS_RESULT();
	sleep(1);
}

static void utc_media_SpeechDetector_initEndPointDetect_p(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	bool ret = instance->initEndPointDetect(TEST_SAMPLE_RATE, TEST_CHANNELS);
	instance->deinitEndPointDetect();
	TC_ASSERT_EQ("utc_media_SpeechDetector_initEndPointDetect_p", ret, true);
	TC_SUCCESS_RESULT();
	sleep(1);
}

static void utc_media_SpeechDetector_initEndPointDetect_n(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	bool ret = instance->initEndPointDetect(TEST_SAMPLE_RATE, 0);
	instance->deinitEndPointDetect();
	TC_ASSERT_EQ("utc_media_SpeechDetector_initEndPointDetect_n", ret, false);
	TC_SUCCESS_RESULT();
	sleep(1);
}

static void utc_media_SpeechDetector_deinitKeywordDetect_p(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	instance->initKeywordDetect(TEST_SAMPLE_RATE, TEST_CHANNELS);
	bool ret = instance->deinitKeywordDetect();
	TC_ASSERT_EQ("utc_media_SpeechDetector_deinitKeywordDetect_p", ret, true);
	TC_SUCCESS_RESULT();
	sleep(1);
}

static void utc_media_SpeechDetector_deinitKeywordDetect_n(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	bool ret = instance->deinitKeywordDetect();
	TC_ASSERT_EQ("utc_media_SpeechDetector_deinitKeywordDetect_n", ret, false);
	TC_SUCCESS_RESULT();
	sleep(1);
}

static void utc_media_SpeechDetector_deinitEndPointDetect_p(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	instance->initEndPointDetect(TEST_SAMPLE_RATE, TEST_CHANNELS);
	bool ret = instance->deinitEndPointDetect();
	TC_ASSERT_EQ("utc_media_SpeechDetector_deinitEndPointDetect_p", ret, true);
	TC_SUCCESS_RESULT();
	sleep(1);
}

static void utc_media_SpeechDetector_deinitEndPointDetect_n(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	bool ret = instance->deinitEndPointDetect();
	TC_ASSERT_EQ("utc_media_SpeechDetector_deinitEndPointDetect_n", ret, false);
	TC_SUCCESS_RESULT();
	sleep(1);
}

static void utc_media_SpeechDetector_startKeywordDetect_p(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	instance->initKeywordDetect(TEST_SAMPLE_RATE, TEST_CHANNELS);
	bool ret = instance->startKeywordDetect();
	instance->stopKeywordDetect();
	instance->deinitKeywordDetect();
	TC_ASSERT_EQ("utc_media_SpeechDetector_startKeywordDetect_p", ret, true);
	TC_SUCCESS_RESULT();
	sleep(1);
}

static void utc_media_SpeechDetector_startKeywordDetect_n(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	bool ret = instance->startKeywordDetect();
	TC_ASSERT_EQ("utc_media_SpeechDetector_startKeywordDetect_n", ret, false);
	TC_SUCCESS_RESULT();
	sleep(1);
}

static void utc_media_SpeechDetector_startEndPointDetect_timeout_p(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	instance->initEndPointDetect(TEST_SAMPLE_RATE, TEST_CHANNELS);
	bool ret = instance->startEndPointDetect(-1);
	instance->stopEndPointDetect();
	instance->deinitEndPointDetect();
	TC_ASSERT_EQ("utc_media_SpeechDetector_startEndPointDetect_timeout_p", ret, true);
	TC_SUCCESS_RESULT();
	sleep(1);
}

static void utc_media_SpeechDetector_startEndPointDetect_timeout_n(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	bool ret = instance->startEndPointDetect(0);
	TC_ASSERT_EQ("utc_media_SpeechDetector_startEndPointDetect_timeout_n", ret, false);
	TC_SUCCESS_RESULT();
	sleep(1);
}

static void utc_media_SpeechDetector_startEndPointDetect_p(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	instance->initEndPointDetect(TEST_SAMPLE_RATE, TEST_CHANNELS);
	bool ret = instance->startEndPointDetect();
	instance->stopEndPointDetect();
	instance->deinitEndPointDetect();
	TC_ASSERT_EQ("utc_media_SpeechDetector_startEndPointDetect_p", ret, true);
	TC_SUCCESS_RESULT();
	sleep(1);
}

static void utc_media_SpeechDetector_startEndPointDetect_n(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	bool ret = instance->startEndPointDetect();
	TC_ASSERT_EQ("utc_media_SpeechDetector_startEndPointDetect_n", ret, false);
	TC_SUCCESS_RESULT();
	sleep(1);
}

static void utc_media_SpeechDetector_detectEndPoint_param_p(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	instance->initEndPointDetect(TEST_SAMPLE_RATE, TEST_CHANNELS);
	instance->startEndPointDetect();
	int size = 2048;
	std::shared_ptr<unsigned char> sample(new unsigned char[size], [](unsigned char *p){ delete[] p; });
	bool ret = instance->detectEndPoint(sample, size);
	instance->stopEndPointDetect();
	instance->deinitEndPointDetect();
	TC_ASSERT_EQ("utc_media_SpeechDetector_detectEndPoint_param_p", ret, true);
	TC_SUCCESS_RESULT();
	sleep(1);
}

static void utc_media_SpeechDetector_detectEndPoint_param_n(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	int size = 2048;
	std::shared_ptr<unsigned char> sample(new unsigned char[size], [](unsigned char *p){ delete[] p; });
	bool ret = instance->detectEndPoint(sample, size);
	TC_ASSERT_EQ("utc_media_SpeechDetector_detectEndPoint_param_n", ret, false);
	TC_SUCCESS_RESULT();
	sleep(1);
}

static void utc_media_SpeechDetector_detectEndPoint_param_n1(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	instance->initEndPointDetect(TEST_SAMPLE_RATE, TEST_CHANNELS);
	instance->startEndPointDetect();
	int size = 2048;
	std::shared_ptr<unsigned char> sample;
	bool ret = instance->detectEndPoint(sample, size);
	instance->stopEndPointDetect();
	instance->deinitEndPointDetect();
	TC_ASSERT_EQ("utc_media_SpeechDetector_detectEndPoint_param_n1", ret, false);
	TC_SUCCESS_RESULT();
	sleep(1);
}

static void utc_media_SpeechDetector_detectEndPoint_p(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	instance->initEndPointDetect(TEST_SAMPLE_RATE, TEST_CHANNELS);
	instance->startEndPointDetect();
	bool ret = instance->detectEndPoint();
	instance->stopEndPointDetect();
	instance->deinitEndPointDetect();
	TC_ASSERT_EQ("utc_media_SpeechDetector_detectEndPoint_p", ret, true);
	TC_SUCCESS_RESULT();
	sleep(1);
}

static void utc_media_SpeechDetector_detectEndPoint_n(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	bool ret = instance->detectEndPoint();
	TC_ASSERT_EQ("utc_media_SpeechDetector_detectEndPoint_n", ret, false);
	TC_SUCCESS_RESULT();
	sleep(1);
}

static void utc_media_SpeechDetector_waitEndPoint_p(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	instance->initEndPointDetect(TEST_SAMPLE_RATE, TEST_CHANNELS);
	instance->startEndPointDetect();
	bool ret = instance->waitEndPoint(-1);
	instance->stopEndPointDetect();
	instance->deinitEndPointDetect();
	TC_ASSERT_EQ("utc_media_SpeechDetector_waitEndPoint", ret, true);
	TC_SUCCESS_RESULT();
	sleep(1);
}

static void utc_media_SpeechDetector_waitEndPoint_n(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	bool ret = instance->waitEndPoint(-1);
	TC_ASSERT_EQ("utc_media_SpeechDetector_waitEndPoint", ret, false);
	TC_SUCCESS_RESULT();
	sleep(1);
}

static void utc_media_SpeechDetector_stopKeywordDetect_p(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	instance->initKeywordDetect(TEST_SAMPLE_RATE, TEST_CHANNELS);
	instance->startKeywordDetect();
	bool ret = instance->stopKeywordDetect();
	instance->deinitKeywordDetect();
	TC_ASSERT_EQ("utc_media_SpeechDetector_stopKeywordDetect_p", ret, true);
	TC_SUCCESS_RESULT();
	sleep(1);
}

static void utc_media_SpeechDetector_stopKeywordDetect_n(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	bool ret = instance->stopKeywordDetect();
	TC_ASSERT_EQ("utc_media_SpeechDetector_stopKeywordDetect_n", ret, false);
	TC_SUCCESS_RESULT();
	sleep(1);
}

static void utc_media_SpeechDetector_stopEndPointDetect_p(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	instance->initEndPointDetect(TEST_SAMPLE_RATE, TEST_CHANNELS);
	instance->startEndPointDetect();
	bool ret = instance->stopEndPointDetect();
	instance->deinitEndPointDetect();
	TC_ASSERT_EQ("utc_media_SpeechDetector_stopEndPointDetect_p", ret, true);
	TC_SUCCESS_RESULT();
	sleep(1);
}

static void utc_media_SpeechDetector_stopEndPointDetect_n(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	bool ret = instance->stopEndPointDetect();
	TC_ASSERT_EQ("utc_media_SpeechDetector_stopEndPointDetect_n", ret, false);
	TC_SUCCESS_RESULT();
	sleep(1);
}

static void utc_media_SpeechDetector_addListener_p(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	std::shared_ptr<media::voice::SpeechDetectorListenerInterface> listener = std::make_shared<SpeechDetectorListener>();
	instance->addListener(listener);
	bool ret = instance->removeListener(listener);
	TC_ASSERT_EQ("utc_media_SpeechDetector_addListener_p", ret, true);
	TC_SUCCESS_RESULT();
	sleep(1);
}

static void utc_media_SpeechDetector_removeListener_p(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	std::shared_ptr<media::voice::SpeechDetectorListenerInterface> listener = std::make_shared<SpeechDetectorListener>();
	instance->addListener(listener);
	bool ret = instance->removeListener(listener);
	TC_ASSERT_EQ("utc_media_SpeechDetector_removeListener_p", ret, true);
	TC_SUCCESS_RESULT();
	sleep(1);
}

static void utc_media_SpeechDetector_removeListener_n(void)
{
	auto instance = media::voice::SpeechDetector::instance();
	std::shared_ptr<media::voice::SpeechDetectorListenerInterface> listener;
	bool ret = instance->removeListener(listener);
	TC_ASSERT_EQ("utc_media_SpeechDetector_removeListener_n", ret, false);
	TC_SUCCESS_RESULT();
	sleep(1);
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
	utc_media_SpeechDetector_startEndPointDetect_timeout_p();
	utc_media_SpeechDetector_startEndPointDetect_timeout_n();
	utc_media_SpeechDetector_startEndPointDetect_p();
	utc_media_SpeechDetector_startEndPointDetect_n();
	utc_media_SpeechDetector_detectEndPoint_param_p();
	utc_media_SpeechDetector_detectEndPoint_param_n();
	utc_media_SpeechDetector_detectEndPoint_param_n1();
	utc_media_SpeechDetector_detectEndPoint_p();
	utc_media_SpeechDetector_detectEndPoint_n();
	utc_media_SpeechDetector_waitEndPoint_p();
	utc_media_SpeechDetector_waitEndPoint_n();
	utc_media_SpeechDetector_stopKeywordDetect_p();
	utc_media_SpeechDetector_stopKeywordDetect_n();
	utc_media_SpeechDetector_stopEndPointDetect_p();
	utc_media_SpeechDetector_stopEndPointDetect_n();
	utc_media_SpeechDetector_addListener_p();
	utc_media_SpeechDetector_removeListener_p();
	utc_media_SpeechDetector_removeListener_n();
	return 0;
}