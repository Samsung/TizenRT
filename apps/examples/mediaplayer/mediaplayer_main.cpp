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
#include <media/MediaPlayer.h>
#include <media/FileInputDataSource.h>

using namespace std;
using namespace media;
using namespace media::stream;

//***************************************************************************
// Name: mediaplayer_main
//***************************************************************************/

class MediaPlayerTest : public MediaPlayerObserverInterface, public enable_shared_from_this<MediaPlayerTest>
{
  public:
	void onPlaybackStarted(Id id) override;
	void onPlaybackFinished(Id id) override;
	void onPlaybackError(Id id) override;

	void start();

	MediaPlayerTest() : volume(0)
	{
		cout << "App start" << endl;
	}
	~MediaPlayerTest() 
	{
		cout << "App terminate" << endl;
	}

  private:
	void printMenu();
	void displayMediaPlayer();
	int userInput(int, int);
	MediaPlayer mp;
	int volume;
	enum test_command_e
	{
		APP_OFF,
		PLAYER_START,
		PLAYER_PAUSE,
		PLAYER_STOP,
		VOLUME_UP,
		VOLUME_DOWN
	};
};

void MediaPlayerTest::onPlaybackStarted(Id id)
{
	cout << "onPlaybackStarted" << endl;
}

void MediaPlayerTest::onPlaybackFinished(Id id)
{
	cout << "onPlaybackFinished" << endl;
}

void MediaPlayerTest::onPlaybackError(Id id)
{
	cout << "onPlaybackError" << endl;
}

void MediaPlayerTest::start(void)
{
	/**
	 * Turn on the feature to Test.
	 */
#define TEST_MP3
#undef TEST_AAC
	
#if defined(TEST_MP3)
	auto source = std::move(unique_ptr<FileInputDataSource>(new FileInputDataSource("/rom/over_16000.mp3")));
	source->setSampleRate(16000);
	source->setChannels(2);
#elif defined(TEST_AAC)
	auto source = std::move(unique_ptr<FileInputDataSource>(new FileInputDataSource("/rom/play.mp4")));
#else
	auto source = std::move(unique_ptr<FileInputDataSource>(new FileInputDataSource("/rom/44100.pcm")));
	source->setSampleRate(44100);
	source->setChannels(2);
#endif

	if (mp.create() == PLAYER_ERROR) {
		cout << "Mediaplayer::create failed" << endl;
	}
	mp.setObserver(shared_from_this());
	mp.setDataSource(std::move(source));

	while (true) {
		printMenu();
		switch (userInput(APP_OFF, VOLUME_DOWN)) {
		case APP_OFF:
			cout << "APP_OFF is selected" << endl;
			if (mp.destroy() == PLAYER_ERROR) {
				cout << "Mediaplayer::destroy failed" << endl;
			}
			return;
		case PLAYER_START:
			cout << "PLAYER_START is selected" << endl;
			if (mp.prepare() == PLAYER_ERROR) {
				cout << "Mediaplayer::prepare failed" << endl;
			}
			if (mp.start() == PLAYER_ERROR) {
				cout << "Mediaplayer::start failed" << endl;
			}
			break;
		case PLAYER_PAUSE:
			cout << "PLAYER_PAUSE is selected" << endl;
			if (mp.pause() == PLAYER_ERROR) {
				cout << "Mediaplayer::pause failed" << endl;
			}
			break;
		case PLAYER_STOP:
			cout << "PLAYER_STOP is selected" << endl;
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
}

void MediaPlayerTest::printMenu()
{
	cout << "====================" << endl;
	cout << " 0. APP_OFF         " << endl;
	cout << " 1. PLAYER_START    " << endl;
	cout << " 2. PLAYER_PAUSE    " << endl;
	cout << " 3. PLAYER_STOP     " << endl;
	cout << " 4. VOLUME_UP       " << endl;
	cout << " 5. VOLUME_DOWN     " << endl;
	cout << "====================" << endl;
}

int MediaPlayerTest::userInput(int min, int max)
{
	assert(min <= max);
	int input = 0;

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

	return input;
}

extern "C" {
int mediaplayer_main(int argc, char *argv[])
{
	up_cxxinitialize();

	auto mediaPlayerTest = make_shared<MediaPlayerTest>();
	mediaPlayerTest->start();
	return 0;
}
}
