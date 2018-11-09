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

#include "SoftwareKeywordDetector.h"
#include "SoftwareEndPointDetector.h"
#include "HardwareKeywordDetector.h"
#include "HardwareEndPointDetector.h"

#include "../audio/audio_manager.h"

#include <media/voice/SpeechDetector.h>

namespace media {
namespace voice {

class SpeechDetectorImpl : public SpeechDetector
{
public:
	SpeechDetectorImpl() = default;
	bool initKeywordDetect(uint32_t samprate, uint8_t channels) override;
	bool initEndPointDetect(uint32_t samprate, uint8_t channels) override;
	bool deinitKeywordDetect() override;
	bool deinitEndPointDetect() override;
	bool startKeywordDetect(int timeout) override;
	bool startEndPointDetect(int timeout) override;

private:
	std::shared_ptr<KeywordDetector> mKeywordDetector;
	std::shared_ptr<EndPointDetector> mEndPointDetector;
};

SpeechDetector *SpeechDetector::instance()
{
	static SpeechDetectorImpl inst;
	return &inst;
}

bool SpeechDetectorImpl::initKeywordDetect(uint32_t samprate, uint8_t channels)
{
	if (samprate == 0 || channels == 0) {
		meddbg("%s[line : %d] fail : invalid parameter. samprate : %u, channels : %u\n", __func__, __LINE__, samprate, channels);
		return false;
	}

	int card = -1;
	int device = -1;

	audio_manager_result_t result = find_stream_in_device_with_process_type(
		AUDIO_DEVICE_PROCESS_TYPE_SPEECH_DETECTOR,
		AUDIO_DEVICE_SPEECH_DETECT_KD,
		&card, &device);

	if (result == AUDIO_MANAGER_SUCCESS) {
		medvdbg("KeywordDetector : card_id : %d device_id %d\n", card, device);
		mKeywordDetector = std::make_shared<HardwareKeywordDetector>(card, device);

		result = change_stream_in_device(card, device);
		if (result != AUDIO_MANAGER_SUCCESS && result != AUDIO_MANAGER_DEVICE_ALREADY_IN_USE) {
			meddbg("change_stream_in_device failed: %d\n", result);
			mKeywordDetector = nullptr;
			return false;
		}
	} else {
		medvdbg("Not found H/W speech detector. Use Software\n");
		mKeywordDetector = std::make_shared<SoftwareKeywordDetector>();
	}

	return mKeywordDetector->init(samprate, channels);
}

bool SpeechDetectorImpl::initEndPointDetect(uint32_t samprate, uint8_t channels)
{
	if (samprate == 0 || channels == 0) {
		meddbg("%s[line : %d] fail : invalid parameter. samprate : %u, channels : %u\n", __func__, __LINE__, samprate, channels);
		return false;
	}

	int card = -1;
	int device = -1;

	audio_manager_result_t result = find_stream_in_device_with_process_type(
		AUDIO_DEVICE_PROCESS_TYPE_SPEECH_DETECTOR,
		AUDIO_DEVICE_SPEECH_DETECT_EPD,
		&card, &device);

	if (result == AUDIO_MANAGER_SUCCESS) {
		medvdbg("EndPointDetector : card_id : %d device_id %d\n", card, device);
		mEndPointDetector = std::make_shared<HardwareEndPointDetector>(card, device);

		result = change_stream_in_device(card, device);
		if (result != AUDIO_MANAGER_SUCCESS && result != AUDIO_MANAGER_DEVICE_ALREADY_IN_USE) {
			meddbg("change_stream_in_device failed: %d\n", result);
			mEndPointDetector = nullptr;
			return false;
		}
	} else {
		mEndPointDetector = std::make_shared<SoftwareEndPointDetector>();
	}

	return mEndPointDetector->init(samprate, channels);
}

bool SpeechDetectorImpl::deinitKeywordDetect()
{
	if (mKeywordDetector) {
		mKeywordDetector->deinit();
		mKeywordDetector = nullptr;
		return true;
	} else {
		meddbg("Nothing to deinit\n");
		return false;
	}
}

bool SpeechDetectorImpl::deinitEndPointDetect()
{
	if (mEndPointDetector) {
		mEndPointDetector->deinit();
		mEndPointDetector = nullptr;
		return true;
	} else {
		meddbg("Nothing to deinit\n");
		return false;
	}
}

bool SpeechDetectorImpl::startKeywordDetect(int timeout)
{
	if (mKeywordDetector == nullptr) {
		meddbg("KeywordDetector is not init\n");
		return false;
	}

	return mKeywordDetector->startKeywordDetect(timeout);
}

bool SpeechDetectorImpl::startEndPointDetect(int timeout)
{
	if (mEndPointDetector == nullptr) {
		meddbg("EndPointDetector is not init\n");
		return false;
	}

	return mEndPointDetector->startEndPointDetect(timeout);
}

} // namespace voice
} // namespace media
