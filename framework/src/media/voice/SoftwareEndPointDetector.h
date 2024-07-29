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
#ifndef __MEDIA_SOFTWARE_ENDPOINT_DETECTOR_H
#define __MEDIA_SOFTWARE_ENDPOINT_DETECTOR_H

#include <tinyara/config.h>

#ifndef CONFIG_VOICE_SOFTWARE_EPD_FRAMESIZE
#define CONFIG_VOICE_SOFTWARE_EPD_FRAMESIZE 256
#endif

#include <functional>

#include <media/MediaRecorder.h>

#include "EndPointDetector.h"

#include "aifw/AIInferenceHandler.h"
#include "aifw/AIModelService.h"
#include "../StreamBuffer.h"
namespace media {
namespace voice {

class SoftwareEndPointDetector : public EndPointDetector
{
public:
	SoftwareEndPointDetector();
	~SoftwareEndPointDetector();
	bool init(uint32_t samprate, uint8_t channels) override;
	void deinit() override;
	bool startEndPointDetect(void) override;
	bool stopEndPointDetect(void) override;
	bool detectEndPoint(std::shared_ptr<unsigned char> sample, int size) override;
	bool waitEndPoint(int timeout) override;
	void registerEPDResultListener(EPDResultListener epdResultCallback) override;
	static void epd_inferenceResultListener(AIFW_RESULT res, void *values, uint16_t count);

private:
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
	static EPDResultListener mEPDResultCallback;

	uint16_t getAudioBufferSize(void);
};

} // namespace voice
} // namespace media

#endif
