/* ****************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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

#include <tinyara/config.h>
#include <debug.h>
#include "SpeechDetectorWorker.h"

#ifndef CONFIG_SPEECH_DETECTOR_STACKSIZE
#define CONFIG_SPEECH_DETECTOR_STACKSIZE 4096
#endif

namespace media {
namespace voice {

SpeechDetectorWorker::SpeechDetectorWorker()
{
	medvdbg("SpeechDetectorWorker::SpeechDetectorWorker()\n");
	mThreadName = "SpeechDetectorWorker";
	mStacksize = CONFIG_SPEECH_DETECTOR_STACKSIZE;
	mKeywordDetector = nullptr;
	mPriority = 130;
}
SpeechDetectorWorker::~SpeechDetectorWorker()
{
	medvdbg("SpeechDetectorWorker::~SpeechDetectorWorker()\n");
}

SpeechDetectorWorker& SpeechDetectorWorker::getWorker()
{
	static SpeechDetectorWorker worker;
	return worker;
}

void SpeechDetectorWorker::setKeywordDetector(std::shared_ptr<KeywordDetector> keywordDetector)
{
	mKeywordDetector = keywordDetector;
}

bool SpeechDetectorWorker::processLoop()
{
	if (mKeywordDetector && mKeywordDetector->isKeywordDetectStarted()) {
		mKeywordDetector->detectKeyword();
		return true;
	}
	return false;
}
} // namespace voice
} // namespace media

