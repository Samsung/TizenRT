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
MediaPlayer::MediaPlayer() : mPMpImpl(new MediaPlayerImpl())
{
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

player_result_t MediaPlayer::unprepare()
{
	return mPMpImpl->unprepare();
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

int MediaPlayer::getVolume()
{
	return mPMpImpl->getVolume();
}

player_result_t MediaPlayer::setVolume(int vol)
{
	return mPMpImpl->setVolume(vol);
}

void MediaPlayer::setDataSource(std::unique_ptr<stream::InputDataSource> source)
{
	mPMpImpl->setDataSource(std::move(source));
}

void MediaPlayer::setObserver(std::shared_ptr<MediaPlayerObserverInterface> observer)
{
	mPMpImpl->setObserver(observer);
}

player_state_t MediaPlayer::getState()
{
	return mPMpImpl->getState();
}

player_result_t MediaPlayer::seekTo(int msec)
{
	return mPMpImpl->seekTo(msec);
}

MediaPlayer::~MediaPlayer()
{
}
} // namespace media
