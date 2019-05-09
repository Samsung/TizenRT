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
#include <media/HttpInputDataSource.h>
#include "BufferInputDataSource.h"
#include <string.h>
#include <debug.h>
#include <fcntl.h>
#include <dirent.h>
#include <limits.h>
#include <wifi_manager/wifi_manager.h>

#ifndef CONFIG_EXAMPLES_MEDIAPLAYER_SSID
#define CONFIG_EXAMPLES_MEDIAPLAYER_SSID "NULL"
#endif
#ifndef CONFIG_EXAMPLES_MEDIAPLAYER_PASSPHRASE
#define CONFIG_EXAMPLES_MEDIAPLAYER_PASSPHRASE "NULL"
#endif
#ifndef CONFIG_EXAMPLES_MEDIAPLAYER_AUTHENTICATION
#define CONFIG_EXAMPLES_MEDIAPLAYER_AUTHENTICATION 0
#endif
#ifndef CONFIG_EXAMPLES_MEDIAPLAYER_CRYPTO
#define CONFIG_EXAMPLES_MEDIAPLAYER_CRYPTO 0
#endif

using namespace std;
using namespace media;
using namespace media::stream;

//***************************************************************************
// Name: mediaplayer_main
//***************************************************************************/

static const int TEST_PCM = 0;
static const int TEST_BUFFER = 1;
static const int TEST_HTTP = 2;

static char TEST_FILE_PATH[128] = "/rom/44100.pcm";
// We don't provide any song's URL, to avoid license issue.
// Please fill a valid URL to `TEST_HTTP_URL` for testing!
static const std::string TEST_HTTP_URL = "";

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

extern "C" {
static int wifi_connect();
static int wifi_disconnect();
static int list_dir_entries(const char *dirpath, char **filelist, int max);
}

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

void MyMediaPlayer::onAsyncPrepared(MediaPlayer &mediaPlayer, player_error_t error)
{
	cout << "onAsyncPrepared res" << endl;
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
		if (testSource == TEST_HTTP) {
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
			auto test = selectSource();
			if (test < 0) {
				break;
			}

			if (!setUp(test)) {
				continue;
			}

			while (true) {
				auto player = selectPlayer();
				if (player < 0) {
					break;
				}
				cout << "PLAYER " << (char)('A' + player) << " is selected" << endl;
				auto command = selectCommand();
				mPlayer[player]->doCommand(command);
			}

			tearDown(test);
		}
	}

private:
	bool setUp(const int test)
	{
		if ((test == TEST_HTTP) && (wifi_connect() != 0)) {
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
			wifi_disconnect();
		}
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

	int selectSource()
	{
		char *filelist[32] = { 0, };
		int filenum = list_dir_entries("/rom", filelist, 32);

		cout << "====================" << endl;
		cout << " 0. Exit APP        " << endl;
		cout << " 1. Test PCM        " << endl;
		cout << " 2. Test BUFFER     " << endl;
		cout << " 3. Test HTTP       " << endl;
		int offset = 4;
		int i;
		for (i = 0; i < filenum; i++) {
			cout << " " << offset + i << ". " << filelist[i] << endl;
		}
		cout << "====================" << endl;

		int select = userInput(0, offset + filenum - 1);
		if (select >= offset) {
			strncpy(TEST_FILE_PATH, filelist[select - offset], sizeof(TEST_FILE_PATH));
		}
		for (i = 0; i < filenum; i++) {
			free(filelist[i]);
		}
		return select - 1;
	}

	int selectPlayer()
	{
		cout << "====================" << endl;
		cout << " 0. Select Source   " << endl;
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

static bool sg_wifiIsConnected = false;
static std::mutex sg_wifiMutex;
static std::condition_variable sg_wifiCondv;
static const std::chrono::seconds WAIT_WIFI_TIMEOUT(5);

#define WIFI_CONNECTED_RESETFLAG() sg_wifiIsConnected = false

#define WIFI_CONNECTED_CHECKFLAG() sg_wifiIsConnected

#define WIFI_CONNECTED_NOTIFY() \
	do {\
		std::lock_guard<std::mutex> lock(sg_wifiMutex);\
		sg_wifiIsConnected = true;\
		sg_wifiCondv.notify_one();\
	} while (0)

#define WIFI_CONNECTED_WAIT() \
	do {\
		std::unique_lock<std::mutex> lock(sg_wifiMutex);\
		sg_wifiCondv.wait_for(lock, WAIT_WIFI_TIMEOUT, []{return sg_wifiIsConnected;});\
	} while (0)

static void wifi_sta_connected(wifi_manager_result_e result) {
	meddbg("result %d\n", (int)result);
	WIFI_CONNECTED_NOTIFY();
}

static void wifi_sta_disconnected(wifi_manager_disconnect_e result) {
	meddbg("result %d\n", (int)result);
}

static int wifi_connect()
{
	static wifi_manager_cb_s wifi_callbacks = {
		wifi_sta_connected,
		wifi_sta_disconnected,
		NULL,
		NULL,
		NULL,
	};

	wifi_manager_result_e ret = wifi_manager_init(&wifi_callbacks);
	if (ret != WIFI_MANAGER_SUCCESS) {
		meddbg("wifi_manager_init failed, ret %d\n", (int)ret);
		return -1;
	}

	wifi_manager_ap_config_s config;
	config.ssid_length = strlen(CONFIG_EXAMPLES_MEDIAPLAYER_SSID);
	config.passphrase_length = strlen(CONFIG_EXAMPLES_MEDIAPLAYER_PASSPHRASE);
	strncpy(config.ssid, CONFIG_EXAMPLES_MEDIAPLAYER_SSID, config.ssid_length + 1);
	strncpy(config.passphrase, CONFIG_EXAMPLES_MEDIAPLAYER_PASSPHRASE, config.passphrase_length + 1);
	config.ap_auth_type = (wifi_manager_ap_auth_type_e)CONFIG_EXAMPLES_MEDIAPLAYER_AUTHENTICATION;
	config.ap_crypto_type = (wifi_manager_ap_crypto_type_e)CONFIG_EXAMPLES_MEDIAPLAYER_CRYPTO;

	WIFI_CONNECTED_RESETFLAG();

	meddbg("wifi_manager_connect_ap...\n");
	ret = wifi_manager_connect_ap(&config);
	if (ret != WIFI_MANAGER_SUCCESS) {
		meddbg("wifi_manager_connect_ap failed, ret %d\n", (int)ret);
		wifi_manager_deinit();
		return -1;
	}

	meddbg("WIFI_CONNECTED_WAIT...\n");
	WIFI_CONNECTED_WAIT();
	if (!WIFI_CONNECTED_CHECKFLAG()) {
		meddbg("wifi connect failed, timeout!\n");
		wifi_manager_deinit();
		return -1;
	}

	return 0;
}

static int wifi_disconnect()
{
	wifi_manager_result_e ret = wifi_manager_deinit();
	if (ret != WIFI_MANAGER_SUCCESS) {
		meddbg("wifi_manager_deinit failed, ret %d\n", (int)ret);
		return -1;
	}

	return 0;
}

/* list all files in the directory */
static int list_dir_entries(const char *dirpath, char **filelist, int max)
{
	if (!filelist) {
		meddbg("Invalid list param!\n", dirpath);
		return -1;
	}

	DIR *dirp = opendir(dirpath);
	if (!dirp) {
		meddbg("Failed to open directory %s\n", dirpath);
		return -1;
	}

	int num = 0;
	struct dirent *entryp;
	char path[CONFIG_PATH_MAX];

	while ((entryp = readdir(dirp)) != NULL) {
		snprintf(path, CONFIG_PATH_MAX, "%s/%s", dirpath, entryp->d_name);
		if (!DIRENT_ISDIRECTORY(entryp->d_type)) {
			/* this entry is a file, add it to list. */
			if (num >= max) {
				meddbg("Too many files, list is full.\n");
				break;
			}
			filelist[num++] = strdup(path);
		} else {
			if (!strcmp(".", entryp->d_name) || !strcmp("..", entryp->d_name)) {
				continue;
			}
			int ret = list_dir_entries(path, &filelist[num], max - num);
			if (ret <= 0) {
				continue;
			}
			num += ret;
		}
	}

	closedir(dirp);
	return num;
}


int mediaplayer_main(int argc, char *argv[])
{
	up_cxxinitialize();

	MediaPlayerController mediaPlayerController;
	mediaPlayerController.start();

	return 0;
}
}
