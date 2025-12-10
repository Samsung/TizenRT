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
#ifndef __MEDIA_SOFTWARE_KEYWORD_DETECTOR_H
#define __MEDIA_SOFTWARE_KEYWORD_DETECTOR_H

#include <functional>

#include <media/MediaRecorder.h>

#include "KeywordDetector.h"

namespace media {
namespace voice {

class SoftwareKeywordDetector : public KeywordDetector
{
public:
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
	bool setKDSensitivity(uint16_t sensitivity) override;
	bool getKDSensitivity(uint16_t *sensitivity) override;

private:
	MediaRecorder mRecorder;
	SpeechResultListener mSpeechResultCallback;
	bool mKeywordDetectStarted;
};

} // namespace voice
} // namespace media

#endif
