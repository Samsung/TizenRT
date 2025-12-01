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
#ifndef __MEDIA_HARDWARE_KEYWORD_DETECTOR_H
#define __MEDIA_HARDWARE_KEYWORD_DETECTOR_H

#include <functional>

#include <media/MediaRecorder.h>

#include "KeywordDetector.h"

#include "aifw/AIInferenceHandler.h"
#include "aifw/AIModelService.h"
#include "../StreamBuffer.h"

#define KEYWORD_INTERRUPT_MASK_KD		0
#define KEYWORD_INTERRUPT_MASK_LOCAL	1

namespace media {
namespace voice {

class HardwareKeywordDetector : public KeywordDetector
{
public:
	HardwareKeywordDetector(int normal_card, int normal_device, int sd_card, int sd_device);
	bool init(uint32_t samprate, uint8_t channels) override;
	void deinit() override;
	bool startKeywordDetect(void) override;
	bool stopKeywordDetect(void) override;
	void detectKeyword(void) override;
	bool isKeywordDetectStarted(void) override;
	void registerKeywordResultCallback(SpeechResultListener speechResultCallback) override;
	bool getKeywordBufferSize(uint32_t *bufferSize) override;
	bool getKeywordData(uint8_t *buffer) override;
	bool changeKeywordModel(uint8_t model) override;

private:
	/* AUDIO_DEVICE_PROCESS_TYPE_NONE card, device id */
	int mNormalCard;
	int mNormalDevice;
	/* AUDIO_DEVICE_PROCESS_TYPE_SPEECH_DETECTOR card, device id */
	int mSdCard;
	int mSdDevice;
	SpeechResultListener mSpeechResultCallback;
	bool mKeywordDetectStarted;
	bool mIsLocalCommandEnabled;

	/* AIFW and audio buffer for Keyword Verifier of local command */
	std::shared_ptr<aifw::AIInferenceHandler> mAIInferenceHandler;
	std::shared_ptr<aifw::AIModelService> mAIModelService;
	std::shared_ptr<media::stream::StreamBuffer> mAudioDataBuffer;
	unsigned char *mPCMData;
	/**
	 * @brief Size of mAudioBuffer in bytes. Size is calculated based on the number of bytes required for inference &
	 * the number of bytes coming from the recorder in detectEndPoint() API.
	*/
	int16_t mAudioBufferSize;
	/**
	 * @brief Number of bytes required for inference.
	*/
	uint16_t mInferenceInputSize;

	uint16_t getAudioBufferSize(void);
};

} // namespace voice
} // namespace media

#endif
