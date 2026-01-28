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
#ifndef __MEDIA_KEYWORD_DETECTOR_H
#define __MEDIA_KEYWORD_DETECTOR_H

#include <functional>

#include <media/voice/SpeechDetector.h>
#include "../audio/audio_manager.h"

namespace media {
namespace voice {

typedef void (*SpeechResultListener)(audio_device_process_unit_subtype_e event);

class KeywordDetector
{
public:
	virtual bool init(uint32_t samprate, uint8_t channels) = 0;
	virtual void deinit() = 0;
	virtual bool startKeywordDetect(void) = 0;
	virtual bool stopKeywordDetect(void) = 0;
	virtual void detectKeyword(void) = 0;
	virtual bool setKDSensitivity(uint16_t sensitivity) = 0;
	virtual bool getKDSensitivity(uint16_t *sensitivity) = 0;
	virtual bool isKeywordDetectStarted(void) = 0;
	virtual void registerKeywordResultCallback(SpeechResultListener speechResultCallback) = 0;
	virtual bool getKeywordBufferSize(uint32_t *bufferSize) = 0;
	virtual bool getKeywordData(uint8_t *buffer) = 0;
	virtual bool changeKeywordModel(uint8_t model) = 0;
};

} // namespace voice
} // namespace media

#endif
