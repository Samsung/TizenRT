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
#include "SoftwareEndPointDetector.h"

namespace media {
namespace voice {

bool SoftwareEndPointDetector::init(uint32_t samprate, uint8_t channels)
{
	int adjust = 0;

	mPreviousVAD = 0;
	mVAD = 0;

	/**
	 * Currently, we don't support channel count.
	 */
	mState = speex_preprocess_state_init(CONFIG_VOICE_SOFTWARE_EPD_FRAMESIZE, samprate);

	adjust = 99;
	speex_preprocess_ctl(mState, SPEEX_PREPROCESS_SET_PROB_START, &adjust);
	adjust = 80;
	speex_preprocess_ctl(mState, SPEEX_PREPROCESS_SET_PROB_CONTINUE, &adjust);

	return true;
}

void SoftwareEndPointDetector::deinit()
{
	speex_preprocess_state_destroy(mState);
}

bool SoftwareEndPointDetector::startEndPointDetect(int timeout)
{
	return true;
}

} // namespace voice
} // namespace media
