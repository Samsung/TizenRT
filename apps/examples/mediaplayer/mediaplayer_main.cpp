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

#include <tinyara/init.h>
#include <apps/platform/cxxinitialize.h>
#include <media/FocusManager.h>
#include <media/MediaPlayer.h>
#include <media/FileInputDataSource.h>

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

enum test_command_e { APP_OFF, PLAYER_START, PLAYER_PAUSE, PLAYER_RESUME, PLAYER_STOP, VOLUME_UP, VOLUME_DOWN };

class MyMediaPlayer : public MediaPlayerObserverInterface,
					  public FocusChangeListener,
					  public enable_shared_from_this<MyMediaPlayer>
{
public:
	MyMediaPlayer() : volume(0){};
	virtual ~MyMediaPlayer(){};
	bool init(int test);
	void doCommand(int command);
	void onPlaybackStarted(Id id) override;
	void onPlaybackFinished(Id id) override;
	void onPlaybackError(Id id) override;
	void onPlaybackPaused(Id id) override;
	void onFocusChange(int focusChange) override;

private:
	MediaPlayer mp;
	int volume;
	std::shared_ptr<FocusRequest> mFocusRequest;
};

bool MyMediaPlayer::init(int test)
{
	if (mp.create() == PLAYER_ERROR) {
		cout << "Mediaplayer::create failed" << endl;
		return false;
	}
	unique_ptr<FileInputDataSource> source;
	switch (test) {
	case TEST_MP3:
		source = std::move(unique_ptr<FileInputDataSource>(new FileInputDataSource("/rom/over_16000.mp3")));
		source->setSampleRate(16000);
		source->setChannels(2);
		source->setPcmFormat(AUDIO_FORMAT_TYPE_S16_LE);
		break;
	case TEST_AAC:
		source = std::move(unique_ptr<FileInputDataSource>(new FileInputDataSource("/rom/play.mp4")));
		break;
	case TEST_OPUS:
		source = std::move(unique_ptr<FileInputDataSource>(new FileInputDataSource("/rom/res_16k.opus")));
		break;
	default:
		source = std::move(unique_ptr<FileInputDataSource>(new FileInputDataSource("/rom/44100.pcm")));
		source->setSampleRate(44100);
		source->setChannels(2);
		source->setPcmFormat(AUDIO_FORMAT_TYPE_S16_LE);
	}
	mp.setDataSource(std::move(source));
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
		focusManager.requestFocus(mFocusRequest);
		break;
	case PLAYER_PAUSE:
		cout << "PLAYER_PAUSE is selected" << endl;
		if (mp.pause() == PLAYER_ERROR) {
			cout << "Mediaplayer::pause failed" << endl;
		}
		break;
	case PLAYER_RESUME:
		cout << "PLAYER_RESUME is selected" << endl;
		if (mp.start() == PLAYER_ERROR) {
			cout << "Mediaplayer::start failed" << endl;
		}
		break;
	case PLAYER_STOP:
		cout << "PLAYER_STOP is selected" << endl;
		focusManager.abandonFocus(mFocusRequest);
		if (mp.stop() == PLAYER_ERROR) {
			cout << "Mediaplayer::stop failed" << endl;
		}

		if (mp.unprepare() == PLAYER_ERROR) {
			cout << "Mediaplayer::unprepare failed" << endl;
		}
		break;
	case VOLUME_UP:
		cout << "VOLUME_UP is selected" << endl;
		volume = mp.getVolume();
		cout << "Volume was " << volume << endl;
		if (mp.setVolume(volume + 1) == PLAYER_ERROR) {
			cout << "MediaPlayer::setVolume failed" << endl;
		}
		volume = mp.getVolume();
		cout << "Now, Volume is " << volume << endl;
		break;
	case VOLUME_DOWN:
		cout << "VOLUME_UP is selected" << endl;
		volume = mp.getVolume();
		cout << "Volume was " << volume << endl;
		if (mp.setVolume(volume - 1) == PLAYER_ERROR) {
			cout << "MediaPlayer::setVolume failed" << endl;
		}
		volume = mp.getVolume();
		cout << "Now, Volume is " << volume << endl;
		break;
	default:
		break;
	}
}

void MyMediaPlayer::onPlaybackStarted(Id id)
{
	cout << "onPlaybackStarted" << endl;
}

void MyMediaPlayer::onPlaybackFinished(Id id)
{
	cout << "onPlaybackFinished" << endl;
}

void MyMediaPlayer::onPlaybackError(Id id)
{
	cout << "onPlaybackError" << endl;
}

void MyMediaPlayer::onPlaybackPaused(Id id)
{
	cout << "onPlaybackPaused" << endl;
}

void MyMediaPlayer::onFocusChange(int focusChange)
{

	switch (focusChange) {
	case FOCUS_GAIN:
		if (mp.prepare() == PLAYER_ERROR) {
			cout << "Mediaplayer::prepare failed" << endl;
		}
		if (mp.start() == PLAYER_ERROR) {
			cout << "Mediaplayer::start failed" << endl;
		}
		break;
	case FOCUS_LOSS:
		if (mp.pause() == PLAYER_ERROR) {
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
			auto command = selectCommand();
			mPlayer[player]->doCommand(command);
		}

		tearDown();
	}

private:
	void setUp(const int test)
	{
		mPlayer[0] = make_shared<MyMediaPlayer>();
		mPlayer[0]->init(TEST_MP3);
		mPlayer[1] = make_shared<MyMediaPlayer>();
		mPlayer[1]->init(TEST_MP3);
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
					cout << "return input" << endl;
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
		cout << " 5. VOLUME_UP       " << endl;
		cout << " 6. VOLUME_DOWN     " << endl;
		cout << "====================" << endl;
		return userInput(0, 6);
	}
	shared_ptr<MyMediaPlayer> mPlayer[2];
};

extern "C" {
int mediaplayer_main(int argc, char *argv[])
{
	up_cxxinitialize();

	MediaPlayerController mediaPlayerController;
	mediaPlayerController.start(TEST_MP3);
	return 0;
}
}
