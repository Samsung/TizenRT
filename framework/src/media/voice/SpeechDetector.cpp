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
#include <algorithm>

#include "SoftwareKeywordDetector.h"
#include "SoftwareEndPointDetector.h"
#include "HardwareKeywordDetector.h"
#include "HardwareEndPointDetector.h"
#include "SpeechDetectorListenerWorker.h"
#include "SpeechDetectorWorker.h"
#include "SpeechDetectorImpl.h"
#include "../audio/audio_manager.h"

#include <media/voice/SpeechDetector.h>

#define INVALID_HW_NUMBER -1

namespace media {
namespace voice {

vector<shared_ptr<SpeechDetectorListenerInterface>> SpeechDetectorImpl::mSpeechDetectorListenerList;

SpeechDetector *SpeechDetector::instance()
{
	static SpeechDetectorImpl inst;
	return &inst;
}

SpeechDetectorImpl::SpeechDetectorImpl()
{
}

SpeechDetectorImpl::~SpeechDetectorImpl()
{
	SpeechDetectorWorker& sdw = SpeechDetectorWorker::getWorker();
	sdw.stopWorker();
	SpeechDetectorListenerWorker& sdlw = SpeechDetectorListenerWorker::getWorker();
	sdlw.stopWorker();
}

bool SpeechDetectorImpl::initKeywordDetect(uint32_t samprate, uint8_t channels)
{
	if (samprate == 0 || channels == 0) {
		meddbg("%s[line : %d] fail : invalid parameter. samprate : %u, channels : %u\n", __func__, __LINE__, samprate, channels);
		return false;
	}
#ifdef CONFIG_MEDIA_HARDWARE_KD

	int sd_card = INVALID_HW_NUMBER;
	int sd_device = INVALID_HW_NUMBER;

	audio_manager_result_t result = find_stream_in_device_with_process_type(
		AUDIO_DEVICE_PROCESS_TYPE_SPEECH_DETECTOR,
		AUDIO_DEVICE_SPEECH_DETECT_KD,
		&sd_card, &sd_device);

	if (result == AUDIO_MANAGER_SUCCESS) {
		medvdbg("KeywordDetector : card_id : %d device_id %d\n", sd_card, sd_device);
		/* TODO : find AUDIO_DEVICE_PROCESS_TYPE_NONE type card, device id.
				  currently set 0, 0 */
		mKeywordDetector = std::make_shared<HardwareKeywordDetector>(0, 0, sd_card, sd_device);
		if (!mKeywordDetector) {
			meddbg("Memory allocation failed for mKeywordDetector");
			return false;
		}
	} else {
		meddbg("Error: find_stream_in_device_with_process_type failed!, error: %d\n", result);
		return false;
	}
#elif defined(CONFIG_MEDIA_SOFTWARE_KD)
	medvdbg("Not found H/W keyword detector. Use Software\n");
	mKeywordDetector = std::make_shared<SoftwareKeywordDetector>();
	if (!mKeywordDetector) {
		meddbg("Memory allocation failed for mKeywordDetector");
		return false;
	}
#endif
	SpeechDetectorWorker& sdw = SpeechDetectorWorker::getWorker();
	sdw.startWorker();
	sdw.setKeywordDetector(mKeywordDetector);

	SpeechDetectorListenerWorker& sdlw = SpeechDetectorListenerWorker::getWorker();
	sdlw.startWorker();

	sdw.enQueue(&KeywordDetector::init, mKeywordDetector, samprate, channels);
	sdw.enQueue(&KeywordDetector::registerKeywordResultCallback, mKeywordDetector, speechResultListener);

	medvdbg("Speech detector init KD successful");
	return true;
}

bool SpeechDetectorImpl::initEndPointDetect(uint32_t samprate, uint8_t channels)
{
	if (samprate == 0 || channels == 0) {
		meddbg("%s[line : %d] fail : invalid parameter. samprate : %u, channels : %u\n", __func__, __LINE__, samprate, channels);
		return false;
	}
#ifdef CONFIG_MEDIA_HARDWARE_EPD
	int sd_card = INVALID_HW_NUMBER;
	int sd_device = INVALID_HW_NUMBER;

	audio_manager_result_t result = find_stream_in_device_with_process_type(
		AUDIO_DEVICE_PROCESS_TYPE_SPEECH_DETECTOR,
		AUDIO_DEVICE_SPEECH_DETECT_EPD,
		&sd_card, &sd_device);

	if (result == AUDIO_MANAGER_SUCCESS) {
		medvdbg("EndPointDetector : card_id : %d device_id %d\n", sd_card, sd_device);
		/* TODO : find AUDIO_DEVICE_PROCESS_TYPE_NONE type card, device id.
				  currently set 0, 0 */
		mEndPointDetector = std::make_shared<HardwareEndPointDetector>(0, 0, sd_card, sd_device);
		if (!mEndPointDetector) {
			meddbg("Memory allocation failed for mEndPointDetector");
			return false;
		}
	} else {
		meddbg("Error: find_stream_in_device_with_process_type failed!, error: %d\n", result);
		return false;
	}
#elif defined(CONFIG_MEDIA_SOFTWARE_EPD)
	medvdbg("Not found H/W end point detector. Use Software\n");
	mEndPointDetector = std::make_shared<SoftwareEndPointDetector>();
	if (!mEndPointDetector) {
		meddbg("Memory allocation failed for mEndPointDetector");
		return false;
	}
#endif
	SpeechDetectorWorker& sdw = SpeechDetectorWorker::getWorker();
	sdw.startWorker();

	SpeechDetectorListenerWorker& sdlw = SpeechDetectorListenerWorker::getWorker();
	sdlw.startWorker();

	sdw.enQueue(&EndPointDetector::init, mEndPointDetector, samprate, channels);
	sdw.enQueue(&EndPointDetector::registerEPDResultListener, mEndPointDetector, speechResultListener);

	medvdbg("Speech detector init EPD successful");
	return true;
}

bool SpeechDetectorImpl::deinitKeywordDetect()
{
	if (mKeywordDetector) {
		SpeechDetectorWorker &sdw = SpeechDetectorWorker::getWorker();
		sdw.enQueue(&KeywordDetector::deinit, mKeywordDetector);
		sdw.enQueue(&SpeechDetectorImpl::resetKeywordDetectorPtr, this);
		medvdbg("Speech detector deinit KD done");
		return true;
	} else {
		meddbg("Nothing to deinit\n");
		return false;
	}
}

bool SpeechDetectorImpl::deinitEndPointDetect()
{
	if (mEndPointDetector) {
		SpeechDetectorWorker &sdw = SpeechDetectorWorker::getWorker();
		sdw.enQueue(&EndPointDetector::deinit, mEndPointDetector);
		sdw.enQueue(&SpeechDetectorImpl::resetEndPointDetectorPtr, this);
		medvdbg("Speech detector deinit EPD done");
		return true;
	} else {
		meddbg("Nothing to deinit\n");
		return false;
	}
}

bool SpeechDetectorImpl::startKeywordDetect(void)
{
	if (mKeywordDetector == nullptr) {
		meddbg("KeywordDetector is not init\n");
		return false;
	}

	SpeechDetectorWorker &sdw = SpeechDetectorWorker::getWorker();
	sdw.enQueue(&KeywordDetector::startKeywordDetect, mKeywordDetector);

	medvdbg("Speech detector start KD done");
	return true;
}

bool SpeechDetectorImpl::setKDSensitivity(uint16_t sensitivity){
	if (mKeywordDetector == nullptr) {
		meddbg("KeywordDetector is not init\n");
		return false;
	}

	SpeechDetectorWorker &sdw = SpeechDetectorWorker::getWorker();
	sdw.enQueue(&KeywordDetector::setKDSensitivity, mKeywordDetector, sensitivity);

	medvdbg("Speech detector set KDSensitivity done");
	return true;
}

bool SpeechDetectorImpl::getKDSensitivity(uint16_t *sensitivity){
	if (mKeywordDetector == nullptr) {
		meddbg("KeywordDetector is not init\n");
		return false;
	}

	SpeechDetectorWorker &sdw = SpeechDetectorWorker::getWorker();
	sdw.enQueue(&KeywordDetector::getKDSensitivity, mKeywordDetector, sensitivity);

	medvdbg("Speech detector get KDSensitivity done");
	return true;
}

bool SpeechDetectorImpl::startEndPointDetect(int timeout)
{
	if (mEndPointDetector == nullptr) {
		meddbg("EndPointDetector is not init\n");
		return false;
	}

	SpeechDetectorWorker &sdw = SpeechDetectorWorker::getWorker();
	sdw.enQueue([this, timeout]() { mEndPointDetector->startEndPointDetect(timeout); });

	medvdbg("Speech detector start EPD done with %d sec timeout", timeout);
	return true;
}

bool SpeechDetectorImpl::startEndPointDetect(void)
{
	if (mEndPointDetector == nullptr) {
		meddbg("EndPointDetector is not init\n");
		return false;
	}

	SpeechDetectorWorker &sdw = SpeechDetectorWorker::getWorker();
	sdw.enQueue([this]() { mEndPointDetector->startEndPointDetect(); });

	medvdbg("Speech detector start EPD done");
	return true;
}

bool SpeechDetectorImpl::detectEndPoint(std::shared_ptr<unsigned char> sample, int size)
{
	if (mEndPointDetector == nullptr) {
		meddbg("EndPointDetector is not init\n");
		return false;
	}

	if (sample == nullptr) {
		meddbg("parameter sample is nullptr\n");
		return false;
	}

	if (mEndPointDetector->getEPDState() == EPD_STATE_IN_PROGRESS) {
		SpeechDetectorWorker &sdw = SpeechDetectorWorker::getWorker();
		sdw.enQueue([this, sample, size]() { mEndPointDetector->detectEndPoint(sample, size); });
	}

	medvdbg("Speech detector detect EPD done for sample size of %d bytes", size);
	return true;
}

bool SpeechDetectorImpl::detectEndPoint(void)
{
	if (mEndPointDetector == nullptr) {
		meddbg("EndPointDetector is not init\n");
		return false;
	}

	if (mEndPointDetector->getEPDState() == EPD_STATE_IN_PROGRESS) {
		SpeechDetectorWorker &sdw = SpeechDetectorWorker::getWorker();
		sdw.enQueue([this]() { mEndPointDetector->detectEndPoint(); });
	}

	medvdbg("Speech detector detect EPD done");
	return true;
}

bool SpeechDetectorImpl::waitEndPoint(int timeout)
{
	if (mEndPointDetector == nullptr) {
		meddbg("EndPointDetector is not init\n");
		return false;
	}

	SpeechDetectorWorker &sdw = SpeechDetectorWorker::getWorker();
	sdw.enQueue(&EndPointDetector::waitEndPoint, mEndPointDetector, timeout);

	medvdbg("Speech detector wait EPD done");
	return true;
}

void SpeechDetectorImpl::addListener(std::shared_ptr<SpeechDetectorListenerInterface> listener)
{
	mSpeechDetectorListenerList.push_back(listener);
}

bool SpeechDetectorImpl::removeListener(std::shared_ptr<SpeechDetectorListenerInterface> listener)
{
	auto itr = std::find(mSpeechDetectorListenerList.begin(), mSpeechDetectorListenerList.end(), listener);
	if (itr == mSpeechDetectorListenerList.end()) {
		meddbg("listener is not found\n");
		return false;
	}
	mSpeechDetectorListenerList.erase(itr);
	return true;
}

speech_detect_event_type_e SpeechDetectorImpl::getSpeechDetectEvent(audio_device_process_unit_subtype_e event)
{
	switch (event) {
	case AUDIO_DEVICE_SPEECH_DETECT_NONE:
		return SPEECH_DETECT_NONE;
	case AUDIO_DEVICE_SPEECH_DETECT_EPD:
		return SPEECH_DETECT_EPD;
	case AUDIO_DEVICE_SPEECH_DETECT_SPD:
		return SPEECH_DETECT_SPD;
	case AUDIO_DEVICE_SPEECH_DETECT_KD:
		return SPEECH_DETECT_KD;
	case AUDIO_DEVICE_SPEECH_DETECT_LOCAL:
		return SPEECH_DETECT_LOCAL;
	case AUDIO_DEVICE_SPEECH_DETECT_LOCAL0:
		return SPEECH_DETECT_LOCAL0;
	case AUDIO_DEVICE_SPEECH_DETECT_LOCAL1:
		return SPEECH_DETECT_LOCAL1;
	case AUDIO_DEVICE_SPEECH_DETECT_LOCAL2:
		return SPEECH_DETECT_LOCAL2;
	case AUDIO_DEVICE_SPEECH_DETECT_LOCAL3:
		return SPEECH_DETECT_LOCAL3;
	case AUDIO_DEVICE_SPEECH_DETECT_LOCAL4:
		return SPEECH_DETECT_LOCAL4;
	case AUDIO_DEVICE_SPEECH_DETECT_LOCAL5:
		return SPEECH_DETECT_LOCAL5;
	case AUDIO_DEVICE_SPEECH_DETECT_LOCAL6:
		return SPEECH_DETECT_LOCAL6;
	case AUDIO_DEVICE_SPEECH_DETECT_LOCAL7:
		return SPEECH_DETECT_LOCAL7;
	default:
		return SPEECH_DETECT_NONE;
	}
}

void SpeechDetectorImpl::speechResultListener(audio_device_process_unit_subtype_e event)
{
	medvdbg("Event received in speech detector listener. Event = %d\n", event);
	speech_detect_event_type_e sdEvent = getSpeechDetectEvent(event);
	SpeechDetectorListenerWorker& sdlw = SpeechDetectorListenerWorker::getWorker();
	for (auto &itr : mSpeechDetectorListenerList) {
		sdlw.enQueue(&SpeechDetectorListenerInterface::onSpeechDetectionListener, itr, sdEvent);
	}
}

bool SpeechDetectorImpl::stopKeywordDetect(void)
{
	if (mKeywordDetector == nullptr) {
		meddbg("KeywordDetector is not init\n");
		return false;
	}

	SpeechDetectorWorker &sdw = SpeechDetectorWorker::getWorker();
	sdw.enQueue(&KeywordDetector::stopKeywordDetect, mKeywordDetector);

	medvdbg("Speech detector stop KD done");
	return true;
}

bool SpeechDetectorImpl::stopEndPointDetect(void)
{
	if (mEndPointDetector == nullptr) {
		meddbg("EndPointDetector is not init\n");
		return false;
	}
	SpeechDetectorWorker &sdw = SpeechDetectorWorker::getWorker();
	sdw.clearQueue();
	SpeechDetectorListenerWorker &sdlw = SpeechDetectorListenerWorker::getWorker();
	sdlw.clearQueue();
	bool result = mEndPointDetector->stopEndPointDetect();
	if (!result) {
		meddbg("Speech detector stop EPD failed");
	}
	medvdbg("Speech detector stop EPD done");
	return true;
}

void SpeechDetectorImpl::resetKeywordDetectorPtr(void)
{
	mKeywordDetector = nullptr;
}

void SpeechDetectorImpl::resetEndPointDetectorPtr(void)
{
	mEndPointDetector = nullptr;
}

bool SpeechDetectorImpl::changeKeywordModel(uint8_t model)
{
	if (mKeywordDetector == nullptr) {
		meddbg("keyword detector is not init\n");
		return false;
	}
	if (mKeywordDetector->changeKeywordModel(model) == false) {
		meddbg("model change failed\n");
		return false;
	}
	medvdbg("changed kd model : %d\n", model);
	return true;
}

bool SpeechDetectorImpl::getKeywordBufferSize(uint32_t *bufferSize)
{
	if (mKeywordDetector == nullptr) {
		meddbg("keyword detector is not init\n");
		return false;
	}
	if (bufferSize == NULL) {
		meddbg("invalid parameter\n");
		return false;
	}
	if (mKeywordDetector->getKeywordBufferSize(bufferSize) == false) {
		meddbg("keyword buffer size fetch operation failed\n");
		return false;
	}
	medvdbg("keyword buffer size: %d\n", *bufferSize);
	return true;
}

bool SpeechDetectorImpl::getKeywordData(uint8_t *buffer)
{
	if (mKeywordDetector == nullptr) {
		meddbg("keyword detector is not init\n");
		return false;
	}
	if (buffer == NULL) {
		meddbg("invalid parameter\n");
		return false;
	}
	if (mKeywordDetector->getKeywordData(buffer) == false) {
		meddbg("keyword buffer fetch operation failed\n");
		return false;
	}
	return true;
}

} // namespace voice
} // namespace media

