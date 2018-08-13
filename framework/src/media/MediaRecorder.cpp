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
MediaRecorder::MediaRecorder() : mPMrImpl(new MediaRecorderImpl(*this))
{
}

recorder_error_t MediaRecorder::create()
{
	return mPMrImpl->create();
}

recorder_error_t MediaRecorder::destroy()
{
	return mPMrImpl->destroy();
}

recorder_error_t MediaRecorder::prepare()
{
	return mPMrImpl->prepare();
}

recorder_error_t MediaRecorder::unprepare()
{
	return mPMrImpl->unprepare();
}

recorder_error_t MediaRecorder::start()
{
	return mPMrImpl->start();
}

recorder_error_t MediaRecorder::stop()
{
	return mPMrImpl->stop();
}

recorder_error_t MediaRecorder::pause()
{
	return mPMrImpl->pause();
}

recorder_error_t MediaRecorder::getVolume(uint8_t *vol)
{
	return mPMrImpl->getVolume(vol);
}

recorder_error_t MediaRecorder::setVolume(uint8_t vol)
{
	return mPMrImpl->setVolume(vol);
}

recorder_error_t MediaRecorder::setDataSource(std::unique_ptr<stream::OutputDataSource> dataSource)
{
	return mPMrImpl->setDataSource(std::move(dataSource));
}

recorder_error_t MediaRecorder::setObserver(std::shared_ptr<MediaRecorderObserverInterface> observer)
{
	return mPMrImpl->setObserver(observer);
}

recorder_error_t MediaRecorder::setDuration(int second)
{
	return mPMrImpl->setDuration(second);
}

MediaRecorder::~MediaRecorder()
{
}
} // namespace media
