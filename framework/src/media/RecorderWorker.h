/* ****************************************************************
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

#ifndef __MEDIA_RECORDERWORKER_H
#define __MEDIA_RECORDERWORKER_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include <functional>
#include <iostream>
#include <fstream>

#include <tinyalsa/tinyalsa.h>
#include <media/MediaRecorder.h>
#include "MediaWorker.h"
#include "MediaRecorderImpl.h"
#include "audio/audio_manager.h"

using namespace std;

namespace media {
class RecorderWorker : public MediaWorker
{
public:
	static RecorderWorker& getWorker();

	std::shared_ptr<MediaRecorderImpl> getCurrentRecorder();
	void setCurrentRecorder(std::shared_ptr<MediaRecorderImpl> mr);

private:
	RecorderWorker();
	virtual ~RecorderWorker();
	bool processLoop() override;

private:
	std::shared_ptr<MediaRecorderImpl> mCurRecorder;
};
} // namespace media

#endif
