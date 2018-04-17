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

#include <media/MediaRecorder.h>
#include "MediaRecorderImpl.h"
#include "RecorderWorker.h"

namespace media {
MediaRecorder::MediaRecorder() : mPMrImpl(new MediaRecorderImpl())
{
}

recorder_result_t MediaRecorder::create()
{
	return mPMrImpl->create();
}

recorder_result_t MediaRecorder::destroy() // sync call
{
	return mPMrImpl->destroy();
}

recorder_result_t MediaRecorder::prepare()
{
	return mPMrImpl->prepare();
}

recorder_result_t MediaRecorder::unprepare()
{
	return mPMrImpl->unprepare();
}

recorder_result_t MediaRecorder::start()
{
	return mPMrImpl->start();
}

recorder_result_t MediaRecorder::stop()
{
	return mPMrImpl->stop();
}

recorder_result_t MediaRecorder::pause()
{
	return mPMrImpl->pause();
}

int MediaRecorder::getVolume()
{
	return mPMrImpl->getVolume();
}

void MediaRecorder::setVolume(int vol)
{
	mPMrImpl->setVolume(vol);
}

void MediaRecorder::setDataSource(std::unique_ptr<stream::OutputDataSource> dataSource)
{
	mPMrImpl->setDataSource(std::move(dataSource));
}

void MediaRecorder::setObserver(std::shared_ptr<MediaRecorderObserverInterface> observer)
{
	mPMrImpl->setObserver(observer);
}

MediaRecorder::~MediaRecorder()
{
}
} // namespace media
