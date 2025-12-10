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
#include <pthread.h>
#include <tinyara/audio/audio.h>
#include <sys/types.h>
#include <debug.h>
#include <unistd.h>
#include <sched.h>
#include <pthread.h>

#include "HardwareKeywordDetector.h"
#include "../audio/audio_manager.h"

namespace media {
namespace voice {

HardwareKeywordDetector::HardwareKeywordDetector(int normal_card, int normal_device, int sd_card, int sd_device) :
	mNormalCard(normal_card),
	mNormalDevice(normal_device),
	mSdCard(sd_card),
	mSdDevice(sd_device),
	mSpeechResultCallback(nullptr),
	mKeywordDetectStarted(false)
{
	medvdbg("HardwareKeywordDetector constructor");
}

bool HardwareKeywordDetector::init(uint32_t samprate, uint8_t channels)
{
	audio_manager_result_t result;

	result = change_stream_in_device(mSdCard, mSdDevice);
	if (result != AUDIO_MANAGER_SUCCESS && result != AUDIO_MANAGER_DEVICE_ALREADY_IN_USE) {
		meddbg("change_stream_in_device failed: %d\n", result);
		return false;
	}

	result = register_stream_in_device_process_type(
		mSdCard, mSdDevice,
		AUDIO_DEVICE_PROCESS_TYPE_SPEECH_DETECTOR,
		AUDIO_DEVICE_SPEECH_DETECT_KD);

	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("Error: register_stream_in_device_process_type(%d, %d) failed!, error: %d\n", mSdCard, mSdDevice, result);
		return false;
	}

	result = register_stream_in_device_process_type(
		mSdCard, mSdDevice,
		AUDIO_DEVICE_PROCESS_TYPE_SPEECH_DETECTOR,
		AUDIO_DEVICE_SPEECH_DETECT_LOCAL);

	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("Error: register_stream_in_device_process_type(%d, %d) failed!, error: %d\n", mSdCard, mSdDevice, result);
		return false;
	}

	result = register_stream_in_device_process_handler(
		mSdCard, mSdDevice,
		AUDIO_DEVICE_PROCESS_TYPE_SPEECH_DETECTOR);

	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("Error: register_stream_in_device_process_handler(%d, %d) failed!, error: %d\n", mSdCard, mSdDevice, result);
		return false;
	}

	medvdbg("Hardware KD intialization successful");
	return true;
}

void HardwareKeywordDetector::deinit()
{
	audio_manager_result_t result;

	result = unregister_stream_in_device_process(mSdCard, mSdDevice);
	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("Error: unregister_stream_in_device_process(%d, %d) failed!, error: %d\n", mSdCard, mSdDevice, result);
	}

	result = change_stream_in_device(mNormalCard, mNormalDevice);
	if (result != AUDIO_MANAGER_SUCCESS && result != AUDIO_MANAGER_DEVICE_ALREADY_IN_USE) {
		meddbg("change_stream_in_device failed: %d\n", result);
	}
	medvdbg("Hardware KD deinit done");
}

bool HardwareKeywordDetector::startKeywordDetect(void)
{
	audio_manager_result_t result;

	medvdbg("startKeywordDetect for %d %d\n", mSdCard, mSdDevice);

	/* ToDo: If wake up & local command need to be distinguished, Other type also need to be started here */
	result = start_stream_in_device_process_type(mSdCard, mSdDevice, AUDIO_DEVICE_SPEECH_DETECT_KD);

	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("Error: start_stream_in_device_process(%d, %d) failed!, error: %d\n", mSdCard, mSdDevice, result);
		return false;
	}
	mKeywordDetectStarted = true;
	medvdbg("Hardware KD start successful");
	return true;
}

bool HardwareKeywordDetector::stopKeywordDetect(void)
{
	audio_manager_result_t result;
	medvdbg("stopKeywordDetect for %d %d\n", mSdCard, mSdDevice);
	result = stop_stream_in_device_process_type(mSdCard, mSdDevice, AUDIO_DEVICE_SPEECH_DETECT_KD);
	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("Error: stop_stream_in_device_process_type(%d, %d) failed!, error: %d\n", mSdCard, mSdDevice, result);
		return false;
	}
	mKeywordDetectStarted = false;
	medvdbg("Hardware KD stop successful");
	return true;
}

void HardwareKeywordDetector::detectKeyword(void)
{
	uint16_t msgId;
	audio_manager_result_t result = get_device_process_handler_message(mSdCard, mSdDevice, &msgId);

	if (result == AUDIO_MANAGER_SUCCESS) {
		/* TODO Local Local Command value will be received here */
		if (msgId == AUDIO_DEVICE_SPEECH_DETECT_KD) {
			medvdbg("#### KD DETECTED!! ####\n");
			mSpeechResultCallback((audio_device_process_unit_subtype_e)msgId);
			mKeywordDetectStarted = false;
		}
	} else if (result == AUDIO_MANAGER_INVALID_DEVICE) {
		meddbg("Error: device doesn't support it!!!\n");
		mKeywordDetectStarted = false;
	}
}

bool HardwareKeywordDetector::isKeywordDetectStarted(void)
{
	return mKeywordDetectStarted;
}

void HardwareKeywordDetector::registerKeywordResultCallback(SpeechResultListener speechResultCallback)
{
	mSpeechResultCallback = speechResultCallback;
}

bool HardwareKeywordDetector::changeKeywordModel(uint8_t model)
{
	audio_manager_result_t result = set_keyword_model(model);
	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("change keyword model failed : %d\n", result);
		return false;
	}
	return true;
}

bool HardwareKeywordDetector::getKeywordBufferSize(uint32_t *bufferSize)
{
	if (bufferSize == NULL) {
		meddbg("Error: parameter cannot be NULL\n");
		return false;
	}
	audio_manager_result_t result = get_keyword_buffer_size(bufferSize);
	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("Error: audio manager return not OK, return value: %d\n", result);
		return false;
	}
	return true;
}

bool HardwareKeywordDetector::getKeywordData(uint8_t *buffer)
{
	if (buffer == NULL) {
		meddbg("Error: parameter cannot be NULL\n");
		return false;
	}
	audio_manager_result_t result = get_keyword_data(buffer);
	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("Error: audio manager return not OK, return value: %d\n", result);
		return false;
	}
	return true;
}

bool HardwareKeywordDetector::setKDSensitivity(uint16_t sensitivity)
{
	if (sensitivity > 1000) {
		meddbg("Invalid Sensitivity : %d\n", sensitivity);
		return false;
	}
	audio_manager_result_t result = set_kd_sensitivity(sensitivity);
	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("Error: audio manager return not OK, return value: %d\n", result);
		return false;
	}
	return true;
}

bool HardwareKeywordDetector::getKDSensitivity(uint16_t *sensitivity)
{
	audio_manager_result_t result = get_kd_sensitivity(sensitivity);
	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("Error: audio manager return not OK, return value: %d\n", result);
		return false;
	}
	return true;
}

} // namespace voice
} // namespace media
