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
#include <tinyara/config.h>
#include "SoftwareEndPointDetector.h"
#include "EPDInferenceHandler.h"

namespace media {
namespace voice {

EPDResultListener SoftwareEndPointDetector::mEPDResultCallback;

SoftwareEndPointDetector::SoftwareEndPointDetector() : mAIInferenceHandler(nullptr), mAIModelService(nullptr), mAudioDataBuffer(nullptr), mPCMData(nullptr), mAudioBufferSize(-1), mInferenceInputSize(0)
{
	medvdbg("SoftwareEndPointDetector constructor");
	mEPDState = EPD_STATE_IDLE;
}

SoftwareEndPointDetector::~SoftwareEndPointDetector()
{
	medvdbg("SoftwareEndPointDetector destructor");
}

bool SoftwareEndPointDetector::init(uint32_t samprate, uint8_t channels)
{
	mAIInferenceHandler = std::make_shared<EPDInferenceHandler>(epd_inferenceResultListener);
	if (!mAIInferenceHandler) {
		meddbg("Memory allocation failed for mAIInferenceHandler");
		return false;
	}
	mAIModelService = std::make_shared<aifw::AIModelService>([](void){}, mAIInferenceHandler);
	if (!mAIModelService) {
		mAIInferenceHandler = nullptr;
		meddbg("Memory allocation failed for mAIModelService");
		return false;
	}
	AIFW_RESULT res = mAIModelService->prepare();
	if (res != AIFW_OK) {
		mAIInferenceHandler = nullptr;
		mAIModelService = nullptr;
		meddbg("AI model service prepare api failed, error: %d", res);
		return false;
	}
	AIModelAttribute modelAttribute = std::static_pointer_cast<EPDInferenceHandler>(mAIInferenceHandler)->getModelAttribute();
	mInferenceInputSize = modelAttribute.rawDataCount * sizeof(float);
	mPCMData = new unsigned char[mInferenceInputSize];
	if (!mPCMData) {
		meddbg("Out of memory!! AI FW push data buffer Memory allocation failed\n");
		return false;
	}

	mAudioBufferSize = getAudioBufferSize();
	if (mAudioBufferSize == -1) {
		meddbg("Incoming bytes from recorder are less than bytes used by model per inference cycle\n");
		/* @todo this error is not propogated till appplication due to async calls. Same is applicable at other error handling */
		return false;
	}
	mAudioDataBuffer = media::stream::StreamBuffer::Builder()
					.setBufferSize((size_t)mAudioBufferSize)
					.setThreshold((size_t)mAudioBufferSize)
					.build();
	if (!mAudioDataBuffer) {
		meddbg("Out of memory!! Audio data buffer Memory allocation failed\n");
		return false;
	}
	medvdbg("Software EPD initialization successful");
	return true;
}

void SoftwareEndPointDetector::deinit()
{
	if (mPCMData) {
		delete[] mPCMData;
		mPCMData = nullptr;
	}
	mAudioDataBuffer = nullptr;
	medvdbg("Software EPD deinit done");
}

bool SoftwareEndPointDetector::startEndPointDetect(void)
{
	if (mEPDState != EPD_STATE_IDLE && mEPDState != EPD_STATE_STOP) {
		medwdbg("Software EPD already in progress");
		return false;
	}
	AIFW_RESULT res = mAIModelService->start();
	if (res != AIFW_OK) {
		meddbg("Service start failed. ret: %d", res);
		return false;
	}
	mEPDState = EPD_STATE_IN_PROGRESS;
	medvdbg("Software EPD start successful");
	return true;
}

bool SoftwareEndPointDetector::stopEndPointDetect(void)
{
	if (mEPDState != EPD_STATE_IN_PROGRESS) {
		medwdbg("Software EPD state not in progress");
		return false;
	}

	mEPDState = EPD_STATE_STOP;
	AIFW_RESULT res = mAIModelService->stop();
	if (res != AIFW_OK) {
		meddbg("Service stop failed. ret: %d", res);
		return false;
	}

	medvdbg("Software EPD stop successful");
	return true;
}

bool SoftwareEndPointDetector::waitEndPoint(int timeout)
{
	return false;
}

/* @todo add state checking code */
bool SoftwareEndPointDetector::detectEndPoint(std::shared_ptr<unsigned char> sample, int size)
{
	if (mEPDState != EPD_STATE_IN_PROGRESS) {
		medwdbg("Software EPD state not in progress");
		return false;
	}

	mAudioDataBuffer->write(sample.get(), size);
	while (mAudioDataBuffer->sizeOfData() >= mInferenceInputSize && mEPDState == EPD_STATE_IN_PROGRESS) {
		mAudioDataBuffer->read(mPCMData, mInferenceInputSize);
		AIFW_RESULT res = mAIModelService->pushData(mPCMData, mInferenceInputSize / sizeof(short));
		if (res != AIFW_OK) {
			meddbg("push data operation failed. ret: %d", res);
		}
		medvdbg("push data operation OK");
	}
	medvdbg("Detect EPD done for sample size of %d bytes", size);
	return true;
}

void SoftwareEndPointDetector::registerEPDResultListener(EPDResultListener epdResultCallback)
{
	mEPDResultCallback = epdResultCallback;
}

void SoftwareEndPointDetector::epd_inferenceResultListener(AIFW_RESULT res, void *values, uint16_t count)
{
	if (res != AIFW_OK) {
		meddbg("Inference failed for this cycle, error: %d", res);
		return;
	}
	float *resultBuf = (float *)values;
	medvdbg("EPD inference result[0] = %f, EPD inference result[1] = %f\n", resultBuf[0], resultBuf[1]);
	if (resultBuf[0] < resultBuf[1]) {
		if (resultBuf[1] == 1) {
			meddbg("SPD is Detected\n");
			mEPDResultCallback(AUDIO_DEVICE_SPEECH_DETECT_SPD);
		} else {
			meddbg("Endpoint is Detected\n");
			mEPDResultCallback(AUDIO_DEVICE_SPEECH_DETECT_EPD);
		}
	} else {
		medvdbg("Endpoint is not detected\n");
		mEPDResultCallback(AUDIO_DEVICE_SPEECH_DETECT_NONE);
	}
}

uint16_t SoftwareEndPointDetector::getAudioBufferSize(void)
{
	uint16_t bufferSize;
	/** @todo CONFIG_HANDLER_STREAM_BUFFER_SIZE is used temporarily instead of get_input_card_buffer_size() to avoid buffer
	 *  overflow in case of 4096 bytes of data received.
	*/
	uint16_t incomingBytes = CONFIG_HANDLER_STREAM_BUFFER_SIZE;
	medvdbg("incomingBytes = %d\n", incomingBytes);
	if (incomingBytes % mInferenceInputSize == 0) {
		bufferSize = incomingBytes;
	} else if (incomingBytes > mInferenceInputSize) {
		uint16_t remainder = incomingBytes % mInferenceInputSize;
		uint16_t i = 1;
		do {
			i++;
		} while (remainder * i < mInferenceInputSize);
		bufferSize = incomingBytes + (i - 1) * remainder;
	} else {
		meddbg("Data received less than required size. audio data size from device: %d, model input size: %d", incomingBytes, mInferenceInputSize);
		bufferSize = -1;
	}
	medvdbg("Calculated audio buffer size is %d bytes", bufferSize);
	return bufferSize;
}

} // namespace voice
} // namespace media
