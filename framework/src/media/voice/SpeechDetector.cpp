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
#include <semaphore.h>

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
std::mutex SpeechDetectorImpl::mSpeechDetectorListenerListMutex;

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
	if (mKeywordDetector) {
		meddbg("keyword detector is already init.\n");
		return true;
	}
#ifdef CONFIG_MEDIA_HARDWARE_KD

	int sd_card = INVALID_HW_NUMBER;
	int sd_device = INVALID_HW_NUMBER;

	audio_manager_result_t audioResult = find_stream_in_device_with_process_type(
		AUDIO_DEVICE_PROCESS_TYPE_SPEECH_DETECTOR,
		AUDIO_DEVICE_SPEECH_DETECT_KD,
		&sd_card, &sd_device);

	if (audioResult == AUDIO_MANAGER_SUCCESS) {
		medvdbg("KeywordDetector : card_id : %d device_id %d\n", sd_card, sd_device);
		/* TODO : find AUDIO_DEVICE_PROCESS_TYPE_NONE type card, device id.
				  currently set 0, 0 */
		mKeywordDetector = std::make_shared<HardwareKeywordDetector>(0, 0, sd_card, sd_device);
		if (!mKeywordDetector) {
			meddbg("Memory allocation failed for mKeywordDetector");
			return false;
		}
	} else {
		meddbg("Error: find_stream_in_device_with_process_type failed!, error: %d\n", audioResult);
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
	auto keywordDetector = mKeywordDetector;
	bool result = false;
	sem_t syncSem;
	sem_init(&syncSem, 0, 0);
	if (!sdw.isAlive()) {
		sem_destroy(&syncSem);
		sdw.setKeywordDetector(nullptr);
		mKeywordDetector = nullptr;
		return false;
	}
	sdw.enQueue([keywordDetector, samprate, channels, &result, &syncSem]() {
		result = keywordDetector->init(samprate, channels);
		if (result) {
			keywordDetector->registerKeywordResultCallback(SpeechDetectorImpl::speechResultListener);
		}
		sem_post(&syncSem);
	});
	sem_wait(&syncSem);
	sem_destroy(&syncSem);
	if (!result) {
		sdw.setKeywordDetector(nullptr);
		mKeywordDetector = nullptr;
		meddbg("Speech detector init KD failed");
		return false;
	}
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

	audio_manager_result_t audioResult = find_stream_in_device_with_process_type(
		AUDIO_DEVICE_PROCESS_TYPE_SPEECH_DETECTOR,
		AUDIO_DEVICE_SPEECH_DETECT_EPD,
		&sd_card, &sd_device);

	if (audioResult == AUDIO_MANAGER_SUCCESS) {
		medvdbg("EndPointDetector : card_id : %d device_id %d\n", sd_card, sd_device);
		/* TODO : find AUDIO_DEVICE_PROCESS_TYPE_NONE type card, device id.
				  currently set 0, 0 */
		mEndPointDetector = std::make_shared<HardwareEndPointDetector>(0, 0, sd_card, sd_device);
		if (!mEndPointDetector) {
			meddbg("Memory allocation failed for mEndPointDetector");
			return false;
		}
	} else {
		meddbg("Error: find_stream_in_device_with_process_type failed!, error: %d\n", audioResult);
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
	auto endPointDetector = mEndPointDetector;
	bool result = false;
	sem_t syncSem;
	sem_init(&syncSem, 0, 0);
	if (!sdw.isAlive()) {
		sem_destroy(&syncSem);
		mEndPointDetector = nullptr;
		return false;
	}
	sdw.enQueue([endPointDetector, samprate, channels, &result, &syncSem]() {
		result = endPointDetector->init(samprate, channels);
		if (result) {
			endPointDetector->registerEPDResultListener(SpeechDetectorImpl::speechResultListener);
		}
		sem_post(&syncSem);
	});
	sem_wait(&syncSem);
	sem_destroy(&syncSem);
	if (!result) {
		mEndPointDetector = nullptr;
		meddbg("Speech detector init EPD failed");
		return false;
	}
	medvdbg("Speech detector init EPD successful");
	return true;
}

bool SpeechDetectorImpl::deinitKeywordDetect()
{
	if (!mKeywordDetector) {
		meddbg("Nothing to deinit\n");
		return false;
	}
	SpeechDetectorWorker &sdw = SpeechDetectorWorker::getWorker();
	auto keywordDetector = mKeywordDetector;
	bool result = false;
	sem_t syncSem;
	sem_init(&syncSem, 0, 0);
	if (!sdw.isAlive()) {
		sem_destroy(&syncSem);
		return false;
	}
	sdw.enQueue([keywordDetector, &result, &syncSem]() {
		keywordDetector->deinit();
		result = true;
		sem_post(&syncSem);
	});
	sem_wait(&syncSem);
	sem_destroy(&syncSem);
	if (result) {
		mKeywordDetector = nullptr;
		meddbg("Speech detector deinit KD done");
	}
	return result;
}

bool SpeechDetectorImpl::deinitEndPointDetect()
{
	if (!mEndPointDetector) {
		meddbg("Nothing to deinit\n");
		return false;
	}
	SpeechDetectorWorker &sdw = SpeechDetectorWorker::getWorker();
	auto endPointDetector = mEndPointDetector;
	bool result = false;
	sem_t syncSem;
	sem_init(&syncSem, 0, 0);
	if (!sdw.isAlive()) {
		sem_destroy(&syncSem);
		return false;
	}
	sdw.enQueue([endPointDetector, &result, &syncSem]() {
		endPointDetector->deinit();
		result = true;
		sem_post(&syncSem);
	});
	sem_wait(&syncSem);
	sem_destroy(&syncSem);
	if (result) {
		mEndPointDetector = nullptr;
		medvdbg("Speech detector deinit EPD done");
	}
	return result;
}

bool SpeechDetectorImpl::startKeywordDetect(void)
{
	if (mKeywordDetector == nullptr) {
		meddbg("KeywordDetector is not init\n");
		return false;
	}
	SpeechDetectorWorker &sdw = SpeechDetectorWorker::getWorker();
	auto keywordDetector = mKeywordDetector;
	bool result = false;
	sem_t syncSem;
	sem_init(&syncSem, 0, 0);
	if (!sdw.isAlive()) {
		sem_destroy(&syncSem);
		return false;
	}
	sdw.enQueue([keywordDetector, &result, &syncSem]() {
		result = keywordDetector->startKeywordDetect();
		sem_post(&syncSem);
	});
	sem_wait(&syncSem);
	sem_destroy(&syncSem);
	medvdbg("Speech detector start KD done");
	return result;
}

bool SpeechDetectorImpl::setKDSensitivity(uint16_t sensitivity)
{
	if (mKeywordDetector == nullptr) {
		meddbg("KeywordDetector is not init\n");
		return false;
	}
	SpeechDetectorWorker &sdw = SpeechDetectorWorker::getWorker();
	auto keywordDetector = mKeywordDetector;
	bool result = false;
	sem_t syncSem;
	sem_init(&syncSem, 0, 0);
	if (!sdw.isAlive()) {
		sem_destroy(&syncSem);
		return false;
	}
	sdw.enQueue([keywordDetector, sensitivity, &result, &syncSem]() {
		result = keywordDetector->setKDSensitivity(sensitivity);
		sem_post(&syncSem);
	});
	sem_wait(&syncSem);
	sem_destroy(&syncSem);
	medvdbg("Speech detector set KDSensitivity done");
	return result;
}

bool SpeechDetectorImpl::getKDSensitivity(uint16_t *sensitivity)
{
	if (mKeywordDetector == nullptr || sensitivity == nullptr) {
		meddbg("KeywordDetector is not init or sensitivity is nullptr\n");
		return false;
	}
	SpeechDetectorWorker &sdw = SpeechDetectorWorker::getWorker();
	auto keywordDetector = mKeywordDetector;
	bool result = false;
	sem_t syncSem;
	sem_init(&syncSem, 0, 0);
	if (!sdw.isAlive()) {
		sem_destroy(&syncSem);
		return false;
	}
	sdw.enQueue([keywordDetector, sensitivity, &result, &syncSem]() {
		result = keywordDetector->getKDSensitivity(sensitivity);
		sem_post(&syncSem);
	});
	sem_wait(&syncSem);
	sem_destroy(&syncSem);
	medvdbg("Speech detector get KDSensitivity done");
	return result;
}

bool SpeechDetectorImpl::startEndPointDetect(int timeout)
{
	if (mEndPointDetector == nullptr) {
		meddbg("EndPointDetector is not init\n");
		return false;
	}
	auto endPointDetector = mEndPointDetector;
	bool result = endPointDetector->startEndPointDetect(timeout);
	medvdbg("Speech detector start EPD done with %d sec timeout", timeout);
	return result;
}

bool SpeechDetectorImpl::startEndPointDetect(void)
{
	if (mEndPointDetector == nullptr) {
		meddbg("EndPointDetector is not init\n");
		return false;
	}
	SpeechDetectorWorker &sdw = SpeechDetectorWorker::getWorker();
	auto endPointDetector = mEndPointDetector;
	bool result = false;
	sem_t syncSem;
	sem_init(&syncSem, 0, 0);
	if (!sdw.isAlive()) {
		sem_destroy(&syncSem);
		return false;
	}
	sdw.enQueue([endPointDetector, &result, &syncSem]() {
		result = endPointDetector->startEndPointDetect();
		sem_post(&syncSem);
	});
	sem_wait(&syncSem);
	sem_destroy(&syncSem);
	medvdbg("Speech detector start EPD done");
	return result;
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
	SpeechDetectorWorker &sdw = SpeechDetectorWorker::getWorker();
	auto endPointDetector = mEndPointDetector;
	bool result = false;
	sem_t syncSem;
	sem_init(&syncSem, 0, 0);
	if (!sdw.isAlive()) {
		sem_destroy(&syncSem);
		return false;
	}
	sdw.enQueue([endPointDetector, sample, size, &result, &syncSem]() {
		result = endPointDetector->detectEndPoint(sample, size);
		sem_post(&syncSem);
	});
	sem_wait(&syncSem);
	sem_destroy(&syncSem);
	medvdbg("Speech detector detect EPD done for sample size of %d bytes", size);
	return result;
}

bool SpeechDetectorImpl::detectEndPoint(void)
{
	if (mEndPointDetector == nullptr) {
		meddbg("EndPointDetector is not init\n");
		return false;
	}
	SpeechDetectorWorker &sdw = SpeechDetectorWorker::getWorker();
	auto endPointDetector = mEndPointDetector;
	bool result = false;
	sem_t syncSem;
	sem_init(&syncSem, 0, 0);
	if (!sdw.isAlive()) {
		sem_destroy(&syncSem);
		return false;
	}
	sdw.enQueue([endPointDetector, &result, &syncSem]() {
		result = endPointDetector->detectEndPoint();
		sem_post(&syncSem);
	});
	sem_wait(&syncSem);
	sem_destroy(&syncSem);
	medvdbg("Speech detector detect EPD done");
	return result;
}

bool SpeechDetectorImpl::waitEndPoint(int timeout)
{
	if (mEndPointDetector == nullptr) {
		meddbg("EndPointDetector is not init\n");
		return false;
	}
	auto endPointDetector = mEndPointDetector;
	bool result = endPointDetector->waitEndPoint(timeout);
	medvdbg("Speech detector wait EPD done");
	return result;
}

void SpeechDetectorImpl::addListener(std::shared_ptr<SpeechDetectorListenerInterface> listener)
{
	if (!listener) {
		meddbg("listener is nullptr, cannot add to SpeechDetector\n");
		return;
	}
	meddbg("Adding listener %p to SpeechDetector\n", listener.get());
	std::lock_guard<std::mutex> lock(mSpeechDetectorListenerListMutex);
	mSpeechDetectorListenerList.push_back(listener);
}

bool SpeechDetectorImpl::removeListener(std::shared_ptr<SpeechDetectorListenerInterface> listener)
{
	meddbg("Removing listener %p from SpeechDetector\n", listener.get());
	std::lock_guard<std::mutex> lock(mSpeechDetectorListenerListMutex);
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
	meddbg("Event received in speech detector listener. Event = %d\n", event);
	speech_detect_event_type_e sdEvent = getSpeechDetectEvent(event);

	std::lock_guard<std::mutex> lock(mSpeechDetectorListenerListMutex);
	SpeechDetectorListenerWorker& sdlw = SpeechDetectorListenerWorker::getWorker();
	for (auto &itr : mSpeechDetectorListenerList) {
		/* Copy shared_ptr to increase ref count */
		auto listener = itr;
		if (listener) {
			sdlw.enQueue(&SpeechDetectorListenerInterface::onSpeechDetectionListener, listener, sdEvent);
		} else {
			meddbg("Registered listener is already null\n");
		}
	}
}

bool SpeechDetectorImpl::stopKeywordDetect(void)
{
	if (mKeywordDetector == nullptr) {
		meddbg("KeywordDetector is not init\n");
		return false;
	}
	SpeechDetectorWorker &sdw = SpeechDetectorWorker::getWorker();
	auto keywordDetector = mKeywordDetector;
	bool result = false;
	sem_t syncSem;
	sem_init(&syncSem, 0, 0);
	if (!sdw.isAlive()) {
		sem_destroy(&syncSem);
		return false;
	}
	sdw.enQueue([keywordDetector, &result, &syncSem]() {
		result = keywordDetector->stopKeywordDetect();
		sem_post(&syncSem);
	});
	sem_wait(&syncSem);
	sem_destroy(&syncSem);
	medvdbg("Speech detector stop KD done");
	return result;
}

bool SpeechDetectorImpl::stopEndPointDetect(void)
{
	if (mEndPointDetector == nullptr) {
		meddbg("EndPointDetector is not init\n");
		return false;
	}
	auto endPointDetector = mEndPointDetector;
	bool result = endPointDetector->stopEndPointDetect();
	if (!result) {
		meddbg("Speech detector stop EPD failed");
	}
	medvdbg("Speech detector stop EPD done");
	return result;
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
	meddbg("changed kd model : %d\n", model);
	return true;
}

bool SpeechDetectorImpl::getKeywordBufferSize(uint32_t *bufferSize)
{
	meddbg("[IN] getKeywordBufferSize\n");
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
	meddbg("keyword buffer size: %d\n", *bufferSize);
	return true;
}

bool SpeechDetectorImpl::getKeywordData(uint8_t *buffer)
{
	meddbg("[IN] getKeywordData\n");
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
	meddbg("get keyword data done\n");
	return true;
}

} // namespace voice
} // namespace media

