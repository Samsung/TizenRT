/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
//***************************************************************************
// Included Files
//***************************************************************************

#include <tinyara/config.h>
#include <iostream>
#include <functional>

#include <tinyara/init.h>
#include <apps/platform/cxxinitialize.h>
#include <media/FocusManager.h>
#include <media/MediaPlayer.h>
#include <media/FileInputDataSource.h>
#include "BufferInputDataSource.h"
#include <string.h>

using namespace std;
using namespace media;
using namespace media::stream;

//***************************************************************************
// Name: mediaplayer_main
//***************************************************************************/

static const int TEST_PCM = 0;
static const int TEST_MP3 = 1;
static const int TEST_AAC = 2;
static const int TEST_OPUS = 3;
static const int TEST_WAVE = 4;
static const int TEST_BUFFER = 5;

static const int TEST_COMMAND_NUM = 8;

enum test_command_e {
	APP_OFF = 0,
	PLAYER_START = 1,
	PLAYER_PAUSE,
	PLAYER_RESUME,
	PLAYER_STOP,
	GET_MAX_VOLUME,
	VOLUME_UP,
	VOLUME_DOWN
};

class MyMediaPlayer : public MediaPlayerObserverInterface,
					  public FocusChangeListener,
					  public enable_shared_from_this<MyMediaPlayer>
{
public:
	MyMediaPlayer() : volume(0), isSourceSet(false) {};
	virtual ~MyMediaPlayer() = default;
	bool init(int test);
	void doCommand(int command);
	void onPlaybackStarted(MediaPlayer &mediaPlayer) override;
	void onPlaybackFinished(MediaPlayer &mediaPlayer) override;
	void onPlaybackError(MediaPlayer &mediaPlayer, player_error_t error) override;
	void onStartError(MediaPlayer &mediaPlayer, player_error_t error) override;
	void onStopError(MediaPlayer &mediaPlayer, player_error_t error) override;
	void onPauseError(MediaPlayer &mediaPlayer, player_error_t error) override;
	void onPlaybackPaused(MediaPlayer &mediaPlayer) override;
	void onFocusChange(int focusChange) override;

private:
	MediaPlayer mp;
	uint8_t volume;
	std::shared_ptr<FocusRequest> mFocusRequest;
	std::function<std::unique_ptr<InputDataSource>()> makeSource;
	bool isSourceSet;
};

bool MyMediaPlayer::init(int test)
{
	if (mp.create() != PLAYER_OK) {
		cout << "Mediaplayer::create failed" << endl;
		return false;
	}

	switch (test) {
	case TEST_MP3:
		makeSource = []() {
			auto source = std::move(unique_ptr<FileInputDataSource>(new FileInputDataSource("/rom/over_16000.mp3")));
			source->setPcmFormat(AUDIO_FORMAT_TYPE_S16_LE);
			return std::move(source);
		};
		break;
	case TEST_AAC:
		makeSource = []() {
			auto source = std::move(unique_ptr<FileInputDataSource>(new FileInputDataSource("/rom/play.mp4")));
			return std::move(source);
		};
		break;
	case TEST_OPUS:
		makeSource = []() {
			auto source = std::move(unique_ptr<FileInputDataSource>(new FileInputDataSource("/rom/res_16k.opus")));
			return std::move(source);
		};
		break;
	case TEST_WAVE:
		makeSource = []() {
			auto source = std::move(unique_ptr<FileInputDataSource>(new FileInputDataSource("/rom/Audio.wav")));
			return std::move(source);
		};
		break;
	case TEST_BUFFER:
		makeSource = []() {
			auto source = std::move(unique_ptr<BufferInputDataSource>(new BufferInputDataSource()));
			return std::move(source);
		};
		break;
	default:
		makeSource = []() {
			auto source = std::move(unique_ptr<FileInputDataSource>(new FileInputDataSource("/rom/44100.pcm")));
			source->setSampleRate(44100);
			source->setChannels(2);
			source->setPcmFormat(AUDIO_FORMAT_TYPE_S16_LE);
			return std::move(source);
		};
	}
	isSourceSet = false;
	mp.setObserver(shared_from_this());

	mFocusRequest = FocusRequest::Builder().setFocusChangeListener(shared_from_this()).build();
	return true;
}

void MyMediaPlayer::doCommand(int command)
{
	auto &focusManager = FocusManager::getFocusManager();
	switch (command) {
	case PLAYER_START:
		cout << "PLAYER_START is selected" << endl;
		if (isSourceSet == false) {
			mp.setDataSource(makeSource());
			isSourceSet = true;
		}
		focusManager.requestFocus(mFocusRequest);
		break;
	case PLAYER_PAUSE:
		cout << "PLAYER_PAUSE is selected" << endl;
		if (mp.pause() != PLAYER_OK) {
			cout << "Mediaplayer::pause failed" << endl;
		}
		break;
	case PLAYER_RESUME:
		cout << "PLAYER_RESUME is selected" << endl;
		if (mp.start() != PLAYER_OK) {
			cout << "Mediaplayer::start failed" << endl;
		}
		break;
	case PLAYER_STOP:
		cout << "PLAYER_STOP is selected" << endl;
		focusManager.abandonFocus(mFocusRequest);
		if (mp.stop() != PLAYER_OK) {
			cout << "Mediaplayer::stop failed" << endl;
		}

		if (mp.unprepare() != PLAYER_OK) {
			cout << "Mediaplayer::unprepare failed" << endl;
		}
		isSourceSet = false;
		break;
	case GET_MAX_VOLUME:
		cout << "GET_MAX_VOLUME is selected" << endl;
		if (mp.getMaxVolume(&volume) != PLAYER_OK) {
			cout << "MediaPlayer::getMaxVolume failed" << endl;
		} else {
			cout << "Max Volume is " << (int)volume << endl;
		}
		break;
	case VOLUME_UP:
		uint8_t max_volume;
		cout << "VOLUME_UP is selected" << endl;
		if (mp.getMaxVolume(&max_volume) != PLAYER_OK) {
			cout << "MediaPlayer::getMaxVolume failed" << endl;
		}
		if (mp.getVolume(&volume) != PLAYER_OK) {
			cout << "MediaPlayer::getVolume failed" << endl;
		} else {
			cout << "Volume was " << (int)volume << endl;
		}
		if (volume < max_volume) {
			volume++;
		}
		if (mp.setVolume(volume) != PLAYER_OK) {
			cout << "MediaPlayer::setVolume failed" << endl;
		}
		if (mp.getVolume(&volume) != PLAYER_OK) {
			cout << "MediaPlayer::getVolume failed" << endl;
		} else {
			cout << "Now, Volume is " << (int)volume << endl;
		}
		break;
	case VOLUME_DOWN:
		cout << "VOLUME_UP is selected" << endl;
		if (mp.getVolume(&volume) != PLAYER_OK) {
			cout << "MediaPlayer::getVolume failed" << endl;
		} else {
			cout << "Volume was " << (int)volume << endl;
		}
		if (volume > 0) {
			volume--;
		}
		if (mp.setVolume(volume) != PLAYER_OK) {
			cout << "MediaPlayer::setVolume failed" << endl;
		}
		if (mp.getVolume(&volume) != PLAYER_OK) {
			cout << "MediaPlayer::getVolume failed" << endl;
		} else {
			cout << "Now, Volume is " << (int)volume << endl;
		}
		break;
	default:
		break;
	}
}

void MyMediaPlayer::onPlaybackStarted(MediaPlayer &mediaPlayer)
{
	cout << "onPlaybackStarted" << endl;
}

void MyMediaPlayer::onPlaybackFinished(MediaPlayer &mediaPlayer)
{
	cout << "onPlaybackFinished" << endl;
	this->mp.unprepare();
}

void MyMediaPlayer::onPlaybackError(MediaPlayer &mediaPlayer, player_error_t error)
{
	cout << "onPlaybackError" << endl;
}

void MyMediaPlayer::onStartError(MediaPlayer &mediaPlayer, player_error_t error)
{
	cout << "onStartError" << endl;
}

void MyMediaPlayer::onPauseError(MediaPlayer &mediaPlayer, player_error_t error)
{
	cout << "onPauseError" << endl;
}

void MyMediaPlayer::onStopError(MediaPlayer &mediaPlayer, player_error_t error)
{
	cout << "onStopError" << endl;
}

void MyMediaPlayer::onPlaybackPaused(MediaPlayer &mediaPlayer)
{
	cout << "onPlaybackPaused" << endl;
}

void MyMediaPlayer::onFocusChange(int focusChange)
{

	switch (focusChange) {
	case FOCUS_GAIN:
		if (mp.prepare() != PLAYER_OK) {
			cout << "Mediaplayer::prepare failed" << endl;
		}
		if (mp.start() != PLAYER_OK) {
			cout << "Mediaplayer::start failed" << endl;
		}
		break;
	case FOCUS_LOSS:
		if (mp.pause() != PLAYER_OK) {
			cout << "Mediaplayer::pause failed" << endl;
		}
		break;
	default:
		break;
	}
}

class MediaPlayerController
{
public:
	void start(const int test)
	{
		setUp(test);

		while (true) {
			auto player = selectPlayer();
			if (player < 0) {
				break;
			}
			cout << "PLAYER " << (char)('A' + player) << " is selected" << endl;
			auto command = selectCommand();
			mPlayer[player]->doCommand(command);
		}

		tearDown();
	}

private:
	void setUp(const int test)
	{
		mPlayer[0] = make_shared<MyMediaPlayer>();
		mPlayer[0]->init(test);
		mPlayer[1] = make_shared<MyMediaPlayer>();
		mPlayer[1]->init(test);
	}

	void tearDown()
	{
		mPlayer[0].reset();
		mPlayer[1].reset();
	}

	int userInput(int min, int max)
	{
		assert(min <= max);
		int input;

		while (true) {
			cin >> input;
			cout << endl;

			if (!cin.fail()) {
				if (min <= input && input <= max) {
					cout << "return input " << input << endl;
					return input;
				}
			}

			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			cout << "Invalid Input, please try again" << endl;
		}
	}

	int selectPlayer()
	{
		cout << "====================" << endl;
		cout << " 0. Exit APP        " << endl;
		cout << " 1. Select PLAYER A " << endl;
		cout << " 2. Select PLAYER B " << endl;
		cout << "====================" << endl;
		return userInput(0, 2) - 1;
	}

	int selectCommand()
	{
		cout << "====================" << endl;
		cout << " 0. Do Nothing      " << endl;
		cout << " 1. PLAYER_START    " << endl;
		cout << " 2. PLAYER_PAUSE    " << endl;
		cout << " 3. PLAYER_RESUME   " << endl;
		cout << " 4. PLAYER_STOP     " << endl;
		cout << " 5. GET_MAX_VOLUME  " << endl;
		cout << " 6. VOLUME_UP       " << endl;
		cout << " 7. VOLUME_DOWN     " << endl;
		cout << "====================" << endl;
		return userInput(0, TEST_COMMAND_NUM - 1);
	}
	shared_ptr<MyMediaPlayer> mPlayer[2];
};

extern "C" {
int mediaplayer_main(int argc, char *argv[])
{
	up_cxxinitialize();

	MediaPlayerController mediaPlayerController;
	mediaPlayerController.start(TEST_BUFFER);
	return 0;
}
}
