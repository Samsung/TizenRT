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
#include <debug.h>
#include <sys/types.h>
#include <unistd.h>

#include "HardwareEndPointDetector.h"
#include "../audio/audio_manager.h"

namespace media {
namespace voice {

HardwareEndPointDetector::HardwareEndPointDetector(int card, int device)
	: mCard(card)
	, mDevice(device)
{

}

bool HardwareEndPointDetector::init(uint32_t samprate, uint8_t channels)
{
	audio_manager_result_t result;

	result = register_stream_in_device_process_type(
		mCard, mDevice,
		AUDIO_DEVICE_PROCESS_TYPE_SPEECH_DETECTOR,
		AUDIO_DEVICE_SPEECH_DETECT_EPD);

	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("Error: register_stream_in_device_process_type(%d, %d) failed!\n", mCard, mDevice);
		return false;
	}

	result = register_stream_in_device_process_handler(
		mCard, mDevice,
		AUDIO_DEVICE_PROCESS_TYPE_SPEECH_DETECTOR);

	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("Error: register_stream_in_device_process_handler(%d, %d) failed!\n", mCard, mDevice);
		return false;
	}

	result = start_stream_in_device_process(mCard, mDevice);
	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("Error: start_stream_in_device_process(%d, %d) failed!\n", mCard, mDevice);
		return false;
	}

	pthread_t epd_thread;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setstacksize(&attr, 1024);
	int ret = pthread_create(&epd_thread, &attr, static_cast<pthread_startroutine_t>(HardwareEndPointDetector::endPointDetectThread), this);
	if (ret != 0) {
		meddbg("Fail to create worker thread, return value : %d\n", ret);
		return false;
	}
	pthread_setname_np(epd_thread, "epd_thread");

	void *thread_ret = 0;

	medvdbg("### pthread join for EPD\n");
	pthread_join(epd_thread, &thread_ret);

	medvdbg("### pthread return for EPD: %d\n", thread_ret);

	return true;
}

void HardwareEndPointDetector::deinit()
{
	audio_manager_result_t result;

	result = unregister_stream_in_device_process(mCard, mDevice);

	if (result != AUDIO_MANAGER_SUCCESS) {
		meddbg("Error: unregister_stream_in_device_process(%d, %d) failed!\n", mCard, mDevice);
	}
}

void *HardwareEndPointDetector::endPointDetectThread(void *param)
{
	HardwareEndPointDetector *detector = (HardwareEndPointDetector *)param;

	while (true) {
		if (detector->processEPDFrame(nullptr, 0)) {
			medvdbg("#### EPD DETECTED!! ####\n");
			break;
		}
		usleep(30 * 1000);
	}

	unregister_stream_in_device_process(detector->mCard, detector->mDevice);

	return NULL;
}

bool HardwareEndPointDetector::processEPDFrame(short *sample, int numSample)
{
	audio_manager_result_t result;
	uint16_t msgId;

	result = get_device_process_handler_message(mCard, mDevice, &msgId);

	if (result == AUDIO_MANAGER_SUCCESS) {
		if (msgId == AUDIO_DEVICE_SPEECH_DETECT_EPD) {
			stop_stream_in_device_process(mCard, mDevice);
			return true;
		}
	} else if (result == AUDIO_MANAGER_INVALID_DEVICE) {
		meddbg("Error: device doesn't support it!!!\n");
		return false;
	}

	return false;
}

} // namespace voice
} // namespace media
