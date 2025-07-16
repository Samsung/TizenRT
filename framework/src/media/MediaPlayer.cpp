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

#include <media/MediaPlayer.h>
#include "MediaPlayerImpl.h"
#include "PlayerWorker.h"

namespace media {
MediaPlayer::MediaPlayer() : mPMpImpl(new MediaPlayerImpl(*this))
{
	mId = (uint64_t)this << 32;
	mId = mId | (uint64_t)mPMpImpl.get();
}

player_result_t MediaPlayer::create()
{
	return mPMpImpl->create();
}

player_result_t MediaPlayer::destroy()
{
	return mPMpImpl->destroy();
}

player_result_t MediaPlayer::prepare()
{
	return mPMpImpl->prepare();
}

player_result_t MediaPlayer::prepareAsync()
{
	return mPMpImpl->prepareAsync();
}

player_result_t MediaPlayer::unprepare()
{
	return mPMpImpl->unprepare();
}

player_result_t MediaPlayer::reset()
{
	return mPMpImpl->reset();
}

player_result_t MediaPlayer::start()
{
	return mPMpImpl->start();
}

player_result_t MediaPlayer::stop()
{
	return mPMpImpl->stop();
}

player_result_t MediaPlayer::pause()
{
	return mPMpImpl->pause();
}

player_result_t MediaPlayer::getVolume(uint8_t *vol)
{
	return mPMpImpl->getVolume(vol);
}

player_result_t MediaPlayer::getMaxVolume(uint8_t *vol)
{
	return mPMpImpl->getMaxVolume(vol);
}

player_result_t MediaPlayer::setVolume(uint8_t vol)
{
	return mPMpImpl->setVolume(vol);
}

player_result_t MediaPlayer::setDataSource(std::unique_ptr<stream::InputDataSource> source)
{
	return mPMpImpl->setDataSource(std::move(source));
}

player_result_t MediaPlayer::setObserver(std::shared_ptr<MediaPlayerObserverInterface> observer)
{
	return mPMpImpl->setObserver(observer);
}

bool MediaPlayer::operator==(const MediaPlayer &rhs)
{
	return this->mId == rhs.mId;
}

bool MediaPlayer::isPlaying()
{
	return mPMpImpl->isPlaying();
}

player_result_t MediaPlayer::setStreamInfo(std::shared_ptr<stream_info_t> stream_info)
{
	return mPMpImpl->setStreamInfo(stream_info);
}

player_result_t MediaPlayer::setLooping(bool loop)
{
	return mPMpImpl->setLooping(loop);
}

MediaPlayer::~MediaPlayer()
{
}
} // namespace media

