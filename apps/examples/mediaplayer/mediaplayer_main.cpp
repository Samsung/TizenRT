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
#include <media/FocusManager.h>
#include <media/MediaPlayer.h>
#include <media/FileInputDataSource.h>
#include <media/HttpInputDataSource.h>
#include "BufferInputDataSource.h"
#include <string.h>
#include <debug.h>
#include <fcntl.h>
#include <dirent.h>
#include <limits.h>

#include "WiFiConnector.h"
#include "CmdView.h"

using namespace std;
using namespace media;
using namespace media::stream;

//***************************************************************************
// Name: mediaplayer_main
//***************************************************************************/

static const int TEST_PCM = 1;
static const int TEST_BUFFER = 2;
static const int TEST_HTTP = 3;

static char TEST_FILE_PATH[128] = "/rom/44100.pcm";
// We don't provide any song's URL, to avoid license issue.
// Please fill a valid URL to `TEST_HTTP_URL` for testing!
static const std::string TEST_HTTP_URL = "";

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
	MyMediaPlayer() : volume(0), isSourceSet(false), testSource(-1) {};
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
	void onAsyncPrepared(MediaPlayer &mediaPlayer, player_error_t error) override;
	void onFocusChange(int focusChange) override;

private:
	MediaPlayer mp;
	uint8_t volume;
	std::shared_ptr<FocusRequest> mFocusRequest;
	std::function<std::unique_ptr<InputDataSource>()> makeSource;
	bool isSourceSet;
	int testSource;
};

bool MyMediaPlayer::init(int test)
{
	if (mp.create() != PLAYER_OK) {
		cout << "Mediaplayer::create failed" << endl;
		return false;
	}

	switch (test) {
	case TEST_PCM:
		makeSource = []() {
			auto source = std::move(unique_ptr<FileInputDataSource>(new FileInputDataSource("/rom/44100.pcm")));
			source->setSampleRate(44100);
			source->setChannels(2);
			source->setPcmFormat(AUDIO_FORMAT_TYPE_S16_LE);
			return std::move(source);
		};
		break;
	case TEST_BUFFER:
		makeSource = []() {
			auto source = std::move(unique_ptr<BufferInputDataSource>(new BufferInputDataSource()));
			return std::move(source);
		};
		break;
	case TEST_HTTP:
		makeSource = []() {
			auto source = std::move(unique_ptr<HttpInputDataSource>(new HttpInputDataSource(TEST_HTTP_URL)));
			return std::move(source);
		};
		break;
	default:
		makeSource = []() {
			auto source = std::move(unique_ptr<FileInputDataSource>(new FileInputDataSource(TEST_FILE_PATH)));
			return std::move(source);
		};
	}
	isSourceSet = false;
	testSource = test;
	mp.setObserver(shared_from_this());

	stream_info_t *info;
	stream_info_create(STREAM_TYPE_MEDIA, &info);
	auto deleter = [](stream_info_t *ptr) { stream_info_destroy(ptr); };
	auto stream_info = std::shared_ptr<stream_info_t>(info, deleter);
	mFocusRequest = FocusRequest::Builder()
						.setStreamInfo(stream_info)
						.setFocusChangeListener(shared_from_this())
						.build();
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
		if (mp.stop() != PLAYER_OK) {
			cout << "Mediaplayer::stop failed" << endl;
		}

		if (mp.unprepare() != PLAYER_OK) {
			cout << "Mediaplayer::unprepare failed" << endl;
		}
		isSourceSet = false;
		focusManager.abandonFocus(mFocusRequest);
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
	mediaPlayer.unprepare();
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

void MyMediaPlayer::onAsyncPrepared(MediaPlayer &mediaPlayer, player_error_t error)
{
	cout << "onAsyncPrepared res " << error << endl;
	if (error == PLAYER_ERROR_NONE) {
		mediaPlayer.start();
	} else {
		mediaPlayer.unprepare();
	}
}

void MyMediaPlayer::onFocusChange(int focusChange)
{
	switch (focusChange) {
	case FOCUS_GAIN:
		if ((testSource == TEST_HTTP) || (testSource == TEST_BUFFER)) {
			if (mp.prepareAsync() != PLAYER_OK) {
				cout << "Mediaplayer::prepareAsync failed" << endl;
			}
			// start player in onAsyncPrepared()
		} else {
			if (mp.prepare() != PLAYER_OK) {
				cout << "Mediaplayer::prepare failed" << endl;
			}
			if (mp.start() != PLAYER_OK) {
				cout << "Mediaplayer::start failed" << endl;
			}
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
	void start()
	{
		while (true) {
			vector<string> sourceList = {"Exit APP", "Test PCM", "Test BUFFER", "Test HTTP"};
			listDirEntries("/rom", sourceList);
			auto test = view.selectSource(sourceList);
			if (test == 0) {
				break;
			} else if (test >= 4) {
				strncpy(TEST_FILE_PATH, sourceList[test].c_str(), sizeof(TEST_FILE_PATH));
			}

			if (!setUp(test)) {
				continue;
			}

			while (true) {
				auto player = view.selectPlayer();
				if (player < 0) {
					break;
				}
				auto command = view.selectCommand();
				mPlayer[player]->doCommand(command);
			}

			tearDown(test);
		}
	}

private:
	bool setUp(const int test)
	{
		if ((test == TEST_HTTP) && (MediaPlayerApp::WiFiConnector::connect() != 0)) {
			return false;
		}

		mPlayer[0] = make_shared<MyMediaPlayer>();
		mPlayer[0]->init(test);
		mPlayer[1] = make_shared<MyMediaPlayer>();
		mPlayer[1]->init(test);
		return true;
	}

	void tearDown(const int test)
	{
		mPlayer[0].reset();
		mPlayer[1].reset();

		if (test == TEST_HTTP) {
			MediaPlayerApp::WiFiConnector::disconnect();
		}
	}

	/* list all files in the directory */
	void listDirEntries(const char *dirpath, vector<string> &sourceList)
	{
		DIR *dirp = opendir(dirpath);
		if (!dirp) {
			cout << "Failed to open directory " << dirpath << "\n";
			return;
		}

		struct dirent *entryp;
		char path[CONFIG_PATH_MAX];

		while ((entryp = readdir(dirp)) != NULL) {
			if ((strcmp(".", entryp->d_name) == 0) || (strcmp("..", entryp->d_name) == 0)) {
				continue;
			}

			snprintf(path, CONFIG_PATH_MAX, "%s/%s", dirpath, entryp->d_name);
			if (DIRENT_ISDIRECTORY(entryp->d_type)) {
				listDirEntries(path, sourceList);
			} else {
				/* this entry is a file, add it to list. */
				sourceList.push_back(path);
			}
		}

		closedir(dirp);
	}

	shared_ptr<MyMediaPlayer> mPlayer[2];
	MediaPlayerApp::CmdView view;
};

extern "C" {
int mediaplayer_main(int argc, char *argv[])
{
	MediaPlayerController mediaPlayerController;
	mediaPlayerController.start();

	return 0;
}
}
