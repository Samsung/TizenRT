/******************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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
#include <stdio.h>
#include <memory>
#include <vector>

#include <media/voice/SpeechDetector.h>
#include <media/voice/SpeechDetectorListenerInterface.h>
#include "EndPointDetector.h"
#include "KeywordDetector.h"
#include "../audio/audio_manager.h"

namespace media {
namespace voice {

class SpeechDetectorImpl : public SpeechDetector
{
public:
	SpeechDetectorImpl();
	~SpeechDetectorImpl();
	bool initKeywordDetect(uint32_t samprate, uint8_t channels) override;
	bool initEndPointDetect(uint32_t samprate, uint8_t channels) override;
	bool deinitKeywordDetect() override;
	bool deinitEndPointDetect() override;
	bool startKeywordDetect(void) override;
	bool setKDSensitivity(uint16_t sensitivity) override;
	bool getKDSensitivity(uint16_t *sensitivity) override;
	bool startEndPointDetect(int timeout) override;
	bool startEndPointDetect(void) override;
	bool detectEndPoint(std::shared_ptr<unsigned char> sample, int size) override;
	bool detectEndPoint(void) override;
	bool waitEndPoint(int timeout) override;
	bool stopKeywordDetect(void) override;
	void addListener(std::shared_ptr<SpeechDetectorListenerInterface> listener) override;
	bool removeListener(std::shared_ptr<SpeechDetectorListenerInterface> listener) override;
	bool stopEndPointDetect(void) override;
	bool getKeywordBufferSize(uint32_t *bufferSize) override;
	bool getKeywordData(uint8_t *buffer) override;
	bool changeKeywordModel(uint8_t model) override;
	static void speechResultListener(audio_device_process_unit_subtype_e event);

private:
	static speech_detect_event_type_e getSpeechDetectEvent(audio_device_process_unit_subtype_e event);
	void resetKeywordDetectorPtr(void);
	void resetEndPointDetectorPtr(void);

	std::shared_ptr<KeywordDetector> mKeywordDetector;
	std::shared_ptr<EndPointDetector> mEndPointDetector;
	/* @todo: check static keyword usage */
	static std::vector<std::shared_ptr<SpeechDetectorListenerInterface>> mSpeechDetectorListenerList;
};

} // namespace voice
} // namespace media

