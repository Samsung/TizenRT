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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <apps/platform/cxxinitialize.h>

#include <media/MediaRecorder.h>
#include <media/MediaRecorderObserverInterface.h>
#include <media/BufferOutputDataSource.h>

#include <media/voice/SpeechDetector.h>

/****************************************************************************
 * speech_detector_test_main
 ****************************************************************************/

static void run() {
	media::MediaRecorder recorder;
	bool bRet;

	printf("Hello, Speech Detector Test!!\n");

	media::recorder_result_t mret = recorder.create();
	if (mret == media::RECORDER_OK) {
		printf("[MR] create succeeded.\n");
	} else {
		printf("[MR] create failed.\n");
	}

	mret = recorder.setDataSource(std::unique_ptr<media::stream::BufferOutputDataSource>(new media::stream::BufferOutputDataSource()));
	if (mret == media::RECORDER_OK) {
		printf("[MR] setDataSource succeeded.\n");
	} else {
		printf("[MR] setDataSource failed.\n");
	}

	auto detector = media::voice::SpeechDetector::instance();

	bRet = detector->initKeywordDetect(16000, 1);
	if (bRet) {
		printf("[SD] initKeywordDetect succeeded.\n");
	} else {
		printf("[SD] initKeywordDetect failed.\n");
	}

	mret = recorder.setDuration(3);
	if (mret == media::RECORDER_OK) {
		printf("[MR] setDuration succeeded.\n");
	} else {
		printf("[MR] setDuration failed.\n");
	}
	
	mret = recorder.prepare();
	if (mret == media::RECORDER_OK) {
		printf("[MR] prepare succeeded.\n");
	} else {
		printf("[MR] prepare failed.\n");
	}

	mret = recorder.start();
	if (mret == media::RECORDER_OK) {
		printf("[MR] start succeeded.\n");
	} else {
		printf("[MR] start failed.\n");
	}

	bRet = detector->startKeywordDetect(0);
	if (bRet) {
		printf("[SD] startKeywordDetect succeeded.\n");

		detector->deinitKeywordDetect();

		bRet = detector->initEndPointDetect(16000, 1);
		if (bRet) {
			printf("[SD] initEndPointDetect succeeded.\n");
		} else {
			printf("[SD] initEndPointDetect failed.\n");
		}

		detector->deinitEndPointDetect();
	} else {
		printf("[SD] startKeywordDetect failed.\n");
	}

	printf("[SD] Done\n");
}

extern "C" {
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int speech_detector_test_main(int argc, char *argv[])
#endif
{
	up_cxxinitialize();

	run();

	return 0;
}
}

