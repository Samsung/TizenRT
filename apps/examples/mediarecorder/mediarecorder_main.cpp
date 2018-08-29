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

#include <tinyara/config.h>
#include <tinyara/init.h>
#include <apps/platform/cxxinitialize.h>
#include <tinyalsa/tinyalsa.h>
#include <fcntl.h>
#include <errno.h>
#include <media/MediaRecorder.h>
#include <media/MediaRecorderObserverInterface.h>
#include <media/MediaPlayer.h>
#include <media/MediaPlayerObserverInterface.h>
#include <media/FileOutputDataSource.h>
#include <media/BufferOutputDataSource.h>
#include <media/FileInputDataSource.h>
#include <iostream>
#include <memory>

constexpr int APP_ON = 1;
constexpr int RECORDER_START = 2;
constexpr int RECORDER_PAUSE = 3;
constexpr int RECORDER_RESUME = 4;
constexpr int RECORDER_STOP = 5;
constexpr int VOLUME_UP = 6;
constexpr int VOLUME_DOWN = 7;
constexpr int PLAY_DATA = 8;
constexpr int DELETE_FILE = 9;
constexpr int APP_OFF = 0;

using namespace std;
using namespace media;
using namespace media::stream;

static const int TEST_PCM = 0;
static const int TEST_OPUS = 1;
static const int TEST_BUFFER = 2;

static const char *filePath = "";

class MediaRecorderTest : public MediaRecorderObserverInterface,
						  public MediaPlayerObserverInterface,
						  public enable_shared_from_this<MediaRecorderTest>
{
public:
	void onRecordStarted(MediaRecorder& mediaRecorder)
	{
		std::cout << "onRecordStarted" << std::endl;
	}

	void onRecordPaused(MediaRecorder& mediaRecorder)
	{
		std::cout << "onRecordPaused" << std::endl;
	}

	void onRecordFinished(MediaRecorder& mediaRecorder)
	{
		std::cout << "onRecordFinished" << std::endl;
		mediaRecorder.unprepare();
	}

	void onRecordStartError(MediaRecorder& mediaRecorder, recorder_error_t errCode)
	{
		std::cout << "onRecordStartError!! errCode : " << errCode << std::endl;
	}

	void onRecordPauseError(MediaRecorder& mediaRecorder, recorder_error_t errCode)
	{
		std::cout << "onRecordPauseError!! errCode : " << errCode << std::endl;
	}

	void onRecordStopError(MediaRecorder& mediaRecorder, recorder_error_t errCode)
	{
		std::cout << "onRecordStopError!! errCode : " << errCode << std::endl;
	}

	void onRecordBufferDataReached(MediaRecorder& mediaRecorder, unsigned char *data, size_t size)
	{
		std::cout << "onRecordBufferDataReached, data size : " << size << std::endl;
	}

	void onPlaybackStarted(MediaPlayer &mediaPlayer)
	{
		std::cout << "onPlaybackStarted" << std::endl;
		mIsPlaying = true;
	}

	void onPlaybackFinished(MediaPlayer &mediaPlayer)
	{
		std::cout << "onPlaybackFinished" << std::endl;
		mIsPlaying = false;

		if (mMp.unprepare() != PLAYER_OK) {
			std::cout << "Mediaplayer::unprepare failed" << std::endl;
		}

		if (mMp.destroy() != PLAYER_OK) {
			std::cout << "Mediaplayer::destroy failed" << std::endl;
		}
	}

	void onPlaybackError(MediaPlayer &mediaPlayer, player_error_t error)
	{
		std::cout << "onPlaybackError" << std::endl;
	}

	void onStartError(MediaPlayer &mediaPlayer, player_error_t error)
	{
		std::cout << "onStartError" << std::endl;
	}

	void onStopError(MediaPlayer &mediaPlayer, player_error_t error)
	{
		std::cout << "onStopError" << std::endl;
	}

	void onPauseError(MediaPlayer &mediaPlayer, player_error_t error)
	{
		std::cout << "onPauseError" << std::endl;
	}

	void onPlaybackPaused(MediaPlayer &mediaPlayer)
	{
		std::cout << "onPlaybackPaused" << std::endl;
	}

	void start(const int test)
	{
		uint8_t volume;
		mAppRunning = true;

		while (mAppRunning) {
			printRecorderMenu();
			switch (userInput(APP_ON, DELETE_FILE)) {
			case APP_ON:
				std::cout << "SELECTED APP ON" << std::endl;
				mMr.create();
				mMr.setObserver(shared_from_this());
				break;
			case RECORDER_START:
				std::cout << "SELECTED RECORDER_START" << std::endl;
				if (mIsPaused) {
					if (mMr.start()) {
						std::cout << "START IN PAUSE STATE SUCCESS" << std::endl;
						mIsPaused = false;
					}
				} else {
					if (test == TEST_PCM) {
						filePath = "/tmp/record.pcm";
						mMr.setDataSource(unique_ptr<FileOutputDataSource>(
							new FileOutputDataSource(2, 16000, AUDIO_FORMAT_TYPE_S16_LE, filePath)));
					} else if (test == TEST_OPUS) {
						filePath = "/tmp/record.opus";
						mMr.setDataSource(unique_ptr<FileOutputDataSource>(
							new FileOutputDataSource(2, 16000, AUDIO_FORMAT_TYPE_S16_LE, filePath)));
					} else if (test == TEST_BUFFER) {
						filePath = "";
						mMr.setDataSource(unique_ptr<BufferOutputDataSource>(new BufferOutputDataSource()));
					}

					if (mMr.setDuration(3) == RECORDER_ERROR_NONE && mMr.prepare() == RECORDER_ERROR_NONE) {
						mMr.start();
						std::cout << "START IN NONE-PAUSE STATE SUCCESS" << std::endl;
					} else {
						std::cout << "PREPARE FAILED" << std::endl;
					}
				}
				break;
			case RECORDER_PAUSE:
				std::cout << "SELECTED RECORDER_PAUSE" << std::endl;
				if (mMr.pause()) {
					mIsPaused = true;
					std::cout << "PAUSE SUCCESS" << std::endl;
				} else {
					std::cout << "PAUSE FAILED" << std::endl;
				}
				break;
			case RECORDER_RESUME:
				std::cout << "SELECTED RECORDER_RESUME" << std::endl;
				if (mMr.start()) {
					mIsPaused = false;
					std::cout << "RESUME SUCCESS" << std::endl;
				} else {
					std::cout << "RESUME FAILED" << std::endl;
				}
				break;
			case RECORDER_STOP:
				std::cout << "SELECTED RECORDER_STOP" << std::endl;
				if (mMr.stop() == RECORDER_ERROR_NONE) {
					mIsPaused = false;
					std::cout << "STOP SUCCESS" << std::endl;
				} else {
					std::cout << "STOP FAILED" << std::endl;
				}
				break;
			case PLAY_DATA:
				std::cout << "PLAY_DATA" << std::endl;
				play_data();
				break;
			case VOLUME_UP:
				cout << "VOLUME_UP is selected" << endl;
				if (mMr.getVolume(&volume) != RECORDER_ERROR_NONE) {
					cout << "MediaRecorder::getVolume failed" << endl;
				} else {
					cout << "Volume was " << (int)volume << endl;
				}
				if (mMr.setVolume(volume + 1) != RECORDER_ERROR_NONE) {
					cout << "MediaRecorder::setVolume failed" << endl;
				}
				if (mMr.getVolume(&volume) != RECORDER_ERROR_NONE) {
					cout << "MediaRecorder::getVolume failed" << endl;
				} else {
					cout << "Now, Volume is " << (int)volume << endl;
				}
				break;
			case VOLUME_DOWN:
				cout << "VOLUME_DOWN is selected" << endl;
				if (mMr.getVolume(&volume) != RECORDER_ERROR_NONE) {
					cout << "MediaRecorder::getVolume failed" << endl;
				} else {
					cout << "Volume was " << (int)volume << endl;
				}
				if (mMr.setVolume(volume - 1) != RECORDER_ERROR_NONE) {
					cout << "MediaRecorder::setVolume failed" << endl;
				}
				if (mMr.getVolume(&volume) != RECORDER_ERROR_NONE) {
					cout << "MediaRecorder::getVolume failed" << endl;
				} else {
					cout << "Now, Volume is " << (int)volume << endl;
				}
				break;
			case APP_OFF:
				std::cout << "SELECTED APP_OFF" << std::endl;
				mMr.destroy();
				mAppRunning = false;
				break;
			case DELETE_FILE:
				if (unlink(filePath) == OK) {
					std::cout << "FILE DELETED" << std::endl;
				} else {
					std::cout << "DELETE FAILED ERRPR " << errno << std::endl;
				}
				break;
			}
		}
	}

	void play_data()
	{
		if (mIsPlaying) {
			std::cout << "Already in playback" << std::endl;
			return;
		}

		std::cout << "playback " << filePath << std::endl;

		auto source = std::move(unique_ptr<FileInputDataSource>(new FileInputDataSource(filePath)));
		source->setSampleRate(16000);
		source->setChannels(2);
		source->setPcmFormat(AUDIO_FORMAT_TYPE_S16_LE);

		if (mMp.create() != PLAYER_OK) {
			std::cout << "Mediaplayer::create failed" << std::endl;
			return;
		}

		if (mMp.setObserver(shared_from_this()) != PLAYER_OK) {
			std::cout << "Mediaplayer::setObserver failed" << std::endl;
			return;
		}

		if (mMp.setDataSource(std::move(source)) != PLAYER_OK) {
			std::cout << "Mediaplayer::setDataSource failed" << std::endl;
			return;
		}

		if (mMp.prepare() != PLAYER_OK) {
			std::cout << "Mediaplayer::prepare failed" << std::endl;
			return;
		}

		if (mMp.start() != PLAYER_OK) {
			std::cout << "Mediaplayer::start failed" << std::endl;
			if (mMp.unprepare() != PLAYER_OK) {
				std::cout << "Mediaplayer::unprepare failed" << std::endl;
			}
			return;
		}
	}

	void printRecorderMenu()
	{
		std::cout << "========================================" << std::endl;
		std::cout << "1. RECORDER APP ON" << std::endl;
		std::cout << "2. RECORDER Start" << std::endl;
		std::cout << "3. RECORDER Pause" << std::endl;
		std::cout << "4. RECORDER Resume" << std::endl;
		std::cout << "5. RECORDER Stop" << std::endl;
		std::cout << "6. RECORDER Volume Up" << std::endl;
		std::cout << "7. RECORDER Volume Down" << std::endl;
		std::cout << "8. play Data" << std::endl;
		std::cout << "9. Delete file" << std::endl;
		std::cout << "0. RECORDER APP OFF" << std::endl;
		std::cout << "========================================" << std::endl;
	}

	int userInput(int min, int max)
	{
		assert(min <= max);
		int input = 0;

		std::cin >> input;
		std::cout << std::endl;

		if (!std::cin.fail()) {
			if (min <= input && input <= max) {
				std::cout << "return input" << std::endl;
				return input;
			}
		}

		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		std::cout << "Invalid Input, please try again" << std::endl;

		return input;
	}

	MediaRecorderTest() : mAppRunning(false), mIsPaused(false), mIsPlaying(false)
	{
	}
	~MediaRecorderTest()
	{
	}

private:
	bool mAppRunning;
	bool mIsPaused;
	MediaRecorder mMr;
	bool mIsPlaying;
	MediaPlayer mMp;
};

extern "C" {
int mediarecorder_main(int argc, char *argv[])
{
	up_cxxinitialize();

	auto mediaRecorderTest = make_shared<MediaRecorderTest>();
	mediaRecorderTest->start(TEST_PCM);

	return 0;
}
}
