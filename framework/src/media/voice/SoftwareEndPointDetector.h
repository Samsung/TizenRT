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

#ifndef CONFIG_VOICE_SOFTWARE_EPD
#error "To use S/W EPD, Please enable the External/Software EndPoint Detector(Fixed Float) Support."
#endif

#ifndef CONFIG_VOICE_SOFTWARE_EPD_FRAMESIZE
#define CONFIG_VOICE_SOFTWARE_EPD_FRAMESIZE 256
#endif

#include <functional>

#include <media/MediaRecorder.h>

#include "EndPointDetector.h"
#include <speex/speex_preprocess.h>

namespace media {
namespace voice {

class SoftwareEndPointDetector : public EndPointDetector
{
public:
	bool init(uint32_t samprate, uint8_t channels) override;
	void deinit() override;
	bool startEndPointDetect(int timeout) override;

private:
	SpeexPreprocessState *mState;
	int mPreviousVAD;
	int mVAD;
};

} // namespace voice
} // namespace media

#endif
