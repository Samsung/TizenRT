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
#include "SoftwareEndPointDetector.h"

namespace media {
namespace voice {

SoftwareEndPointDetector::SoftwareEndPointDetector() :
	mState(nullptr)
{
	sem_init(&mSem, 0, 0);
}

SoftwareEndPointDetector::~SoftwareEndPointDetector()
{
	sem_destroy(&mSem);
}

bool SoftwareEndPointDetector::init(uint32_t samprate, uint8_t channels)
{
	int adjust = 0;

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

bool SoftwareEndPointDetector::waitEndPoint(int timeout)
{
	int ret = -1;

	if (timeout < 0) {
		ret = sem_wait(&mSem);
	} else {
		struct timespec waketime;
		clock_gettime(CLOCK_REALTIME, &waketime);
		waketime.tv_sec += timeout;
		ret = sem_timedwait(&mSem, &waketime);
	}

	return ret == 0 ? true : false;
}

bool SoftwareEndPointDetector::detectEndPoint(short *sample, int numSample)
{
	for (short *ptr = sample; ptr <= sample + numSample - CONFIG_VOICE_SOFTWARE_EPD_FRAMESIZE; ptr += CONFIG_VOICE_SOFTWARE_EPD_FRAMESIZE) {
		int vad = speex_preprocess_run(mState, ptr); // vad : 0 (no speech) or 1 (speech)
		if (vad != 0) {
			return false;
		}
	}

	int semVal;
	medvdbg("#### EPD DETECTED!! ####\n");
	if ((sem_getvalue(&mSem, &semVal) == 0) && (semVal < 0)) {
		sem_post(&mSem);
	}

	return true;
}

} // namespace voice
} // namespace media
