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
#include "SpeechDetectorListenerWorker.h"

#ifndef CONFIG_SPEECH_DETECTOR_LISTENER_STACKSIZE
#define CONFIG_SPEECH_DETECTOR_LISTENER_STACKSIZE 4096
#endif

namespace media {
namespace voice {

SpeechDetectorListenerWorker::SpeechDetectorListenerWorker()
{
	medvdbg("SpeechDetectorListenerWorker::SpeechDetectorListenerWorker()\n");
	mThreadName = "SpeechDetectorListenerWorker";
	mStacksize = CONFIG_SPEECH_DETECTOR_LISTENER_STACKSIZE;
	mPriority = 150;
}
SpeechDetectorListenerWorker::~SpeechDetectorListenerWorker()
{
	medvdbg("SpeechDetectorListenerWorker::~SpeechDetectorListenerWorker()\n");
}

SpeechDetectorListenerWorker& SpeechDetectorListenerWorker::getWorker()
{
	static SpeechDetectorListenerWorker worker;
	return worker;
}

bool SpeechDetectorListenerWorker::processLoop()
{
	return false;
}
} // namespace voice
} // namespace media

