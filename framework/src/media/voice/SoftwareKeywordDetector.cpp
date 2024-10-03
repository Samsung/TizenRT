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
#include "SoftwareKeywordDetector.h"

namespace media {
namespace voice {

bool SoftwareKeywordDetector::init(uint32_t samprate, uint8_t channels)
{
	return true;
}

void SoftwareKeywordDetector::deinit()
{
	
}

bool SoftwareKeywordDetector::startKeywordDetect(void)
{
	return true;
}

bool SoftwareKeywordDetector::stopKeywordDetect(void)
{
	return false;	
}

void SoftwareKeywordDetector::detectKeyword(void)
{
}

bool SoftwareKeywordDetector::isKeywordDetectStarted(void)
{
	return mKeywordDetectStarted;
}

void SoftwareKeywordDetector::registerKeywordResultCallback(SpeechResultListener speechResultCallback)
{
	mSpeechResultCallback = speechResultCallback;
}

bool getKeywordBufferSize(uint32_t *bufferSize)
{
	return false;
}

bool getKeywordData(uint8_t *buffer)
{
	return false;
}

} // namespace voice
} // namespace media
