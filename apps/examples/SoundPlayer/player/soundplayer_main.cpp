/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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
#include <sys/stat.h>
#include <tinyara/init.h>
#include <media/FocusManager.h>
#include <media/MediaPlayer.h>
#include <media/FileInputDataSource.h>
#include <media/HttpInputDataSource.h>
#include <media/MediaUtils.h>
#include <audio/SoundManager.h>
#include "../controller/PlayerController.h"
#include <string.h>
#include <debug.h>
#include <fcntl.h>
#include <dirent.h>
#include <limits.h>
#include <vector>

using namespace std;
using namespace media;
using namespace media::stream;

#define DEFAULT_CONTENTS_PATH "/mnt"
#define DEFAULT_SAMPLERATE_TYPE AUDIO_SAMPLE_RATE_24000
#define DEFAULT_FORMAT_TYPE AUDIO_FORMAT_TYPE_S16_LE
#define DEFAULT_CHANNEL_NUM 1 //mono
#define DEFAULT_VOLUME 7
//#define PLAYBACK_REPEAT 1

//***************************************************************************
// class : SoundPlayer
//***************************************************************************/

class SoundPlayer : public MediaPlayerObserverInterface,
					public FocusChangeListener,
					public enable_shared_from_this<SoundPlayer>
{
public:
	SoundPlayer() : mPlayerId(-1), mNumContents(0), mPlayIndex(-1), mHasFocus(false), mPaused(false), mStopped(false),
					mIsPlaying(false), mTrackFinished(false), mSampleRate(DEFAULT_SAMPLERATE_TYPE), mVolume(DEFAULT_VOLUME), mLooping(false), mFocusState(FOCUS_NONE), mIsHttpUrl(false) {};
	~SoundPlayer() {};
	bool init(int argc, char *argv[]);
	player_result_t startPlayback(void);
	bool checkTrackFinished(void);
	void handleError(player_error_t error);
	void onPlaybackFinished(MediaPlayer &mediaPlayer) override;
	void onPlaybackError(MediaPlayer &mediaPlayer, player_error_t error) override;
	void onPauseError(MediaPlayer &mediaPlayer, player_error_t error) override;
	void onAsyncPrepared(MediaPlayer &mediaPlayer, player_error_t error) override;
	void onFocusChange(int focusChange) override;

private:
	MediaPlayer mp;
	int mPlayerId;
	shared_ptr<FocusRequest> mFocusRequest;
	vector<string> mList;
	unsigned int mNumContents;
	unsigned int mPlayIndex;
	bool mHasFocus;
	bool mPaused;
	bool mStopped;
	bool mIsPlaying;
	bool mTrackFinished;
	unsigned int mSampleRate;
	uint8_t mVolume;
	bool mLooping;
	focus_state_t mFocusState;
	bool mIsHttpUrl;
	void loadContents(const char *path);
};

void SoundPlayer::onPauseError(MediaPlayer &mediaPlayer, player_error_t error)
{
	handleError(error);
}

void SoundPlayer::onPlaybackFinished(MediaPlayer &mediaPlayer)
{
	printf("onPlaybackFinished playback index : %d player : %x\n", mPlayIndex, &mp);
	mPlayIndex++;
	mIsPlaying = false;
	mPaused = false;
	mStopped = true;
	if (mPlayIndex == mNumContents) {
#ifndef PLAYBACK_REPEAT
		mTrackFinished = true;
		printf("All Track played, Destroy Player\n");
		mp.unprepare();
		mp.destroy();
		auto &focusManager = FocusManager::getFocusManager();
		focusManager.abandonFocus(mFocusRequest);
		mHasFocus = false;
		return;
#else
		mPlayIndex = 0;
#endif
	}
	if (mHasFocus) {
		printf("wait 3s until play next contents\n");
		mediaPlayer.unprepare();
		sleep(3);
		startPlayback();
	}
}

/* Error case, terminate playback */
void SoundPlayer::onPlaybackError(MediaPlayer &mediaPlayer, player_error_t error)
{
	handleError(error);
}

void SoundPlayer::handleError(player_error_t error)
{
	printf("%s error : %d player : %x\n", __func__, error, &mp);
	if (mHasFocus) {
		auto &focusManager = FocusManager::getFocusManager();
		focusManager.abandonFocus(mFocusRequest);
		mHasFocus = false;
	}
	mp.unprepare();
	mp.destroy();
	mTrackFinished = true;
}

void SoundPlayer::onAsyncPrepared(MediaPlayer &mediaPlayer, player_error_t error)
{
	printf("onAsyncPrepared error : %d\n", error);
	player_result_t res;
	if (error == PLAYER_ERROR_NONE) {
		res = mediaPlayer.start();
		if (res != PLAYER_OK) {
			printf("player start failed res : %d\n", res);
			handleError((player_error_t)res);
		} else {
			printf("Playback started player : %x\n", &mp);
			mPaused = false;
			mStopped = false;
			mIsPlaying = true;
		}
	} else {
		mediaPlayer.unprepare();
	}
}

void SoundPlayer::onFocusChange(int focusChange)
{
	player_result_t res;
	printf("onFousChange player : %x focus : %d isPlaying : %d mPaused : %d\n", &mp, focusChange, mIsPlaying, mPaused);
	/* TODO TRANSIENT option is not ready but will be supported soon */
	switch (focusChange) {
	case FOCUS_GAIN:
	case FOCUS_GAIN_TRANSIENT:
	case FOCUS_GAIN_TRANSIENT_MAY_DUCK:
		mHasFocus = true;
		if (mIsPlaying) {
			return;
		}
		if (mPaused) {
			printf("it was paused, just start playback now\n");
			res = mp.start();
			if (res != PLAYER_OK) {
				printf("player start failed res : %d\n", res);
				handleError((player_error_t)res);
			} else {
				printf("Playback started player : %x\n", &mp);
				mPaused = false;
				mStopped = false;
				mIsPlaying = true;
			}
		} else {
			res = startPlayback();
			if (res != PLAYER_OK) {
				printf("startPlayback failed res : %d\n", res);
			}
		}
		break;
	case FOCUS_LOSS:
		mHasFocus = false;
		if (!mIsPlaying) {
			return;
		}
		res = mp.stop();
		if (res != PLAYER_OK) {
			printf("Player stop failed res : %d\n", res);
			handleError((player_error_t)res);
		} else {
			printf("Playback stopped player : %x\n", &mp);
			mStopped = true;
			mIsPlaying = false;
			mPaused = false;
			mp.unprepare();
		}
		break;
	case FOCUS_LOSS_TRANSIENT:
		mHasFocus = false;
		if (!mIsPlaying) {
			return;
		}
		res = mp.pause(); // it will be played again
		if (res != PLAYER_OK) {
			printf("Player pause failed res : %d\n", res);
			handleError((player_error_t)res);
		} else {
			printf(" Playback paused player : %x\n", &mp);
			mStopped = false;
			mPaused = true;
			mIsPlaying = false;
		}
		break;
	default:
		break;
	}
}

bool SoundPlayer::init(int argc, char *argv[])
{
	int ret;
	char *path = argv[1];
	mIsHttpUrl = (strncmp(path, "http://", 7) == 0 || strncmp(path, "https://", 8) == 0);
	if (mIsHttpUrl) {
		string s = path;
		mList.push_back(s);
	} else {
		struct stat st;
		ret = stat(path, &st);
		if (ret != OK) {
			printf("invalid path : %s\n", path);
			return false;
		}
		if (S_ISDIR(st.st_mode)) {
			loadContents(path);
		} else {
			string s = path;
			mList.push_back(s);
		}
	}

	mNumContents = mList.size();
	if (mNumContents > 0) {
		mPlayIndex = 0;
	}
	printf("Show Track List!! mNumContents : %d mPlayIndex : %d\n", mNumContents, mPlayIndex);
	for (int i = 0; i != (int)mList.size(); i++) {
		printf("path : %s\n", mList.at(i).c_str());
	}

	player_result_t res = mp.create();
	if (res != PLAYER_OK) {
		printf("MediaPlayer create failed res : %d\n", res);
		return false;
	}
	mPlayerId = PlayerController::getInstance().registerPlayer(&mp, (stream_policy_t)(atoi(argv[4])));
	printf("Registered player with ID: %d\n", mPlayerId);
	mp.setObserver(shared_from_this());

	mVolume = atoi(argv[2]);
	stream_info_t *info;
	stream_info_create((stream_policy_t)(atoi(argv[4])), &info);
	auto deleter = [](stream_info_t *ptr) { stream_info_destroy(ptr); };
	auto stream_info = std::shared_ptr<stream_info_t>(info, deleter);
	mFocusRequest = FocusRequest::Builder()
						.setStreamInfo(stream_info)
						.setFocusChangeListener(shared_from_this())
						.build();
	mp.setStreamInfo(stream_info);

	mSampleRate = atoi(argv[3]);
	mTrackFinished = false;

	mFocusState = (focus_state_t)atoi(argv[5]);

	if (argc > 6 && strcmp(argv[6], "1") == 0) {
		mLooping = true;
	}

	auto &focusManager = FocusManager::getFocusManager();
	printf("mp : %x request focus!!\n", &mp);
	ret = focusManager.requestFocus(mFocusRequest, mFocusState);
	if (ret != FOCUS_REQUEST_SUCCESS) {
		printf("Focus request failed. ret: %d\n", ret);
		return false;
	};

	return true;
}

player_result_t SoundPlayer::startPlayback(void)
{
	player_result_t res = PLAYER_OK;
	string s = mList.at(mPlayIndex);
	printf("startPlayback... playIndex : %d path : %s\n", mPlayIndex, s.c_str());
	unique_ptr<InputDataSource> source;
	if (mIsHttpUrl) {
		source = std::move(unique_ptr<HttpInputDataSource>(new HttpInputDataSource((const string)s)));
	} else {
		source = std::move(unique_ptr<FileInputDataSource>(new FileInputDataSource((const string)s)));
	}
	source->setSampleRate(mSampleRate);
	source->setChannels(DEFAULT_CHANNEL_NUM);
	source->setPcmFormat(DEFAULT_FORMAT_TYPE);
	res = mp.setDataSource(std::move(source));
	if (res != PLAYER_OK) {
		handleError((player_error_t)res);
		printf("set Data source failed. res : %d\n", res);
		return res;
	}

	if (mLooping) {
		mp.setLooping(true);
		printf("Looping is set to true\n");
	}
	res = mp.prepare();
	if (res != PLAYER_OK) {
		handleError((player_error_t)res);
		printf("prepare failed res : %d\n", res);
		return res;
	}
	uint8_t curVolume = 0;
	mp.getVolume(&curVolume);
	printf("Current volume : %d new Volume : %d\n", curVolume, mVolume);
	if (curVolume != mVolume) {
		mp.setVolume(mVolume);
	}
	res = mp.start();
	if (res != PLAYER_OK) {
		printf("start failed res : %d\n", res);
		handleError((player_error_t)res);
		return res;
	} else {
		printf("Playback started player : %x\n", &mp);
		mPaused = false;
		mStopped = false;
		mIsPlaying = true;
	}

	return res;
}

/* list all files in the directory */
void SoundPlayer::loadContents(const char *dirpath)
{
	DIR *dirp = opendir(dirpath);
	if (!dirp) {
		printf("Failed to open directory %s\n", dirpath);
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
			loadContents(path);
		} else {
			/* this entry is a file, add it to list. */
			string s = path;
			audio_type_t type = utils::getAudioTypeFromPath(s);
			if (type != AUDIO_TYPE_INVALID) {
				mList.push_back(s);
			}
		}
	}
	closedir(dirp);
}

bool SoundPlayer::checkTrackFinished(void)
{
	return mTrackFinished;
}

extern "C"
{
/*
 This is guide to use MediaPlayer with focus request.
 As MediaPlayer is updated and now without focus request, MediaPlayer can't use audio device!!!

 Steps to follow to play an audio data are as follows:

 1) Application to create a listener to receive onFocusChange callback. Application needs to implement FocusChangeListener interface.
 2) Sample source code to create a FocusRequest object and requestFocus is as follows:
	stream_info_t *info;
	stream_info_create(STREAM_TYPE_BIXBY, &info); //refer to stream_policy_e for various stream types
	auto deleter = [](stream_info_t *ptr) { stream_info_destroy(ptr); };
	auto stream_info = std::shared_ptr<stream_info_t>(info, deleter);
	mFocusRequest = FocusRequest::Builder()
						.setStreamInfo(stream_info)
						.setFocusChangeListener(mFocusObserver)
						.build();
	mp.setStreamInfo(stream_info);
	auto &focusManager = FocusManager::getFocusManager();
	focusManager.requestFocus(mFocusRequest);
3)  Application should handle onFocusChange for FOCUS_LOSS during playback. Application must pause/stop playaback when FOCUS_LOSS / FOCUS_LOSS_TRANSIENT recevied
4)  When playback finsihes, application must call abandonFocus to release focus.
5)  Mandatory condition, requestFocus must be done and onFocusChange with FOCUS_GAIN options received before calling mediaplayer.prepare
6)  Sequence of MediaPlayer API calls are as follows:
	mp.create();
	mp.setObserver(mMediaPlayerObserverInterface);
	mp.setStreamInfo(stream_info);
	mp.setDataSource(source);
	mp.prepare();
	mp.start();
	mp.stop();/mp.pause();
	mp.unprepare();
	mp.destroy();
*/
int soundplayer_main(int argc, char *argv[])
{
	auto player = std::shared_ptr<SoundPlayer>(new SoundPlayer());
	printf("cur SoundPlayer : %x\n", &player);

	if (argc != 7 && argc != 6) {
		printf("invalid input\n");
		printf("Usage : soundplayer [contents path] [volume] [sample rate] [stream policy] [focus state] [looping]\n");
		printf("looping argument is optional\n");
		return -1;
	}
	if (!player->init(argc, argv)) {
		return -1;
	}

	while (!player->checkTrackFinished()) {
		sleep(1);
	}
	printf("Terminate Application : %x\n", &player);

	return 0;
}
}
