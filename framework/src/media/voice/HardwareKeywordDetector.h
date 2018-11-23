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

#include "KeywordDetector.h"

namespace media {
namespace voice {

class HardwareKeywordDetector : public KeywordDetector
{
public:
	HardwareKeywordDetector(int normal_card, int normal_device, int sd_card, int sd_device);
	bool init(uint32_t samprate, uint8_t channels) override;
	void deinit() override;
	bool startKeywordDetect(int timeout) override;

private:
	/* AUDIO_DEVICE_PROCESS_TYPE_NONE card, device id */
	int mNormalCard;
	int mNormalDevice;
	/* AUDIO_DEVICE_PROCESS_TYPE_SPEECH_DETECTOR card, device id */
	int mSdCard;
	int mSdDevice;
};

} // namespace voice
} // namespace media

#endif
