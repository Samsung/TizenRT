/******************************************************************
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
#include <tinyara/config.h>
#include <stdio.h>
#include <debug.h>
#include <functional>

#include "SpeechDetector.h"
#include "SoftwareKeywordDetector.h"
#include "SoftwareEndPointDetector.h"
#include "HardwareKeywordDetector.h"
#include "HardwareEndPointDetector.h"

namespace media {
namespace voice {

static SpeechDetector *g_instance = nullptr;

SpeechDetectorInterface *SpeechDetectorInterface::instance()
{
	if (!g_instance) {
		g_instance = new SpeechDetector();
	}

	return g_instance;
}

bool SpeechDetector::initKeywordDetect(uint32_t samprate, uint8_t channels)
{
	// TODO: Should be implemented
	int deviceType = 1;
	const int SUPPORT_HARDWARE_KEYWORD_DETECT = 1;

	if (deviceType & SUPPORT_HARDWARE_KEYWORD_DETECT) {
		mKeywordDetector = std::make_shared<HardwareKeywordDetector>();
	} else {
		mKeywordDetector = std::make_shared<SoftwareKeywordDetector>();
	}

	mKeywordDetector->init(samprate, channels);

	return true;
}

bool SpeechDetector::initEndPointDetect(uint32_t samprate, uint8_t channels)
{
	// TODO: Should be implemented
	int deviceType = 1;
	const int SUPPORT_HARDWARE_ENDPOINT_DETECT = 2;

	if (deviceType & SUPPORT_HARDWARE_ENDPOINT_DETECT) {
		mEndPointDetector = std::make_shared<HardwareEndPointDetector>();
	} else {
		mEndPointDetector = std::make_shared<SoftwareEndPointDetector>();
	}

	mEndPointDetector->init(samprate, channels);

	return true;
}

void SpeechDetector::deinitKeywordDetect()
{
}

void SpeechDetector::deinitEndPointDetect()
{
}

void SpeechDetector::setEndPointDetectedDelegate(OnEndPointDetectedCallback onEndPointDetected)
{
	assert(mEndPointDetector);
	mEndPointDetector->setEndPointDetectedDelegate(onEndPointDetected);
}

bool SpeechDetector::startKeywordDetect(uint32_t timeout)
{
	assert(mKeywordDetector);
	return mKeywordDetector->startKeywordDetect(timeout);
}

bool SpeechDetector::processEPDFrame(short *sample, int numSample)
{
	assert(mEndPointDetector);
	return mEndPointDetector->processEPDFrame(sample, numSample);
}

} // namespace voice
} // namespace media
