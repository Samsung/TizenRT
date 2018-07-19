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

/**
 * Turn on the feature to Test.
 */
#define TEST_OPUS

#if defined(TEST_OPUS)
static const char *filePath = "/tmp/record.opus";
#else
static const char *filePath = "/tmp/record";
#endif

class MediaRecorderTest : public MediaRecorderObserverInterface
						, public MediaPlayerObserverInterface
						, public enable_shared_from_this<MediaRecorderTest>
{
public:
	void onRecordStarted(MediaRecorderObserverInterface::Id id)
	{
		std::cout << "onRecordStarted" << std::endl;
	}
	void onRecordPaused(MediaRecorderObserverInterface::Id id)
	{
		std::cout << "onRecordPaused" << std::endl;
	}
	void onRecordFinished(MediaRecorderObserverInterface::Id id)
	{
		std::cout << "onRecordFinished" << std::endl;
		mr.unprepare();
	}
	void onRecordError(MediaRecorderObserverInterface::Id id)
	{
		std::cout << "onRecordError!! please select Stop!!" << std::endl;
	}

	void onPlaybackStarted(MediaPlayerObserverInterface::Id id)
	{
		std::cout << "onPlaybackStarted" << std::endl;
	}

	void onPlaybackFinished(MediaPlayerObserverInterface::Id id)
	{
		std::cout << "onPlaybackFinished" << std::endl;
		isPlaying = false;

		if (mp.stop() == PLAYER_ERROR) {
			std::cout << "Mediaplayer::stop failed" << std::endl;
		}

		if (mp.unprepare() == PLAYER_ERROR) {
			std::cout << "Mediaplayer::unprepare failed" << std::endl;
		}

		if (mp.destroy() == PLAYER_ERROR) {
			std::cout << "Mediaplayer::destroy failed" << std::endl;
		}
	}

	void onPlaybackError(MediaPlayerObserverInterface::Id id)
	{
		std::cout << "onPlaybackError" << std::endl;
	}

	void onPlaybackPaused(MediaPlayerObserverInterface::Id id)
	{
		std::cout << "onPlaybackPaused" << std::endl;
	}

	void start()
	{
		appRunning = true;

		while (appRunning) {
			printRecorderMenu();
			switch (userInput(APP_ON, DELETE_FILE))	{
				case APP_ON: {
					std::cout << "SELECTED APP ON" << std::endl;
					mr.create();
					mr.setObserver(shared_from_this());
					mr.setDataSource(unique_ptr<FileOutputDataSource>(new FileOutputDataSource(2, 16000, AUDIO_FORMAT_TYPE_S16_LE, filePath)));
				}
				break;
				case RECORDER_START: {
					std::cout << "SELECTED RECORDER_START" << std::endl;
					if (isPaused) {
						if (mr.start()) {
							std::cout << "START IN PAUSE STATE SUCCESS" << std::endl;
							isPaused = false;
						}
					} else {
						if (mr.setDuration(2) == RECORDER_OK && mr.prepare() == RECORDER_OK) {
							mr.start();
							std::cout << "START IN NONE-PAUSE STATE SUCCESS" << std::endl;
						} else {
							std::cout << "PREPARE FAILED" << std::endl;
						}
					}
				}
				break;
				case RECORDER_PAUSE: {
					std::cout << "SELECTED RECORDER_PAUSE" << std::endl;
					if (mr.pause()) {
						isPaused = true;
						std::cout << "PAUSE SUCCESS" << std::endl;
					} else {
						std::cout << "PAUSE FAILED" << std::endl;
					}
				}
				break;
				case RECORDER_RESUME: {
					std::cout << "SELECTED RECORDER_RESUME" << std::endl;
					if (mr.start()) {
						isPaused = false;
						std::cout << "RESUME SUCCESS" << std::endl;
					} else {
						std::cout << "RESUME FAILED" << std::endl;
					}
				}
				break;
				case RECORDER_STOP: {
					std::cout << "SELECTED RECORDER_STOP" << std::endl;
					if (mr.stop() == RECORDER_OK) {
						mr.unprepare();
						isPaused = false;
						std::cout << "STOP SUCCESS" << std::endl;
					} else {
						std::cout << "STOP FAILED" << std::endl;
					}
				}
				break;
				case PLAY_DATA: {
					std::cout << "PLAY_DATA" << std::endl;
					play_data();
				}
				break;
				case VOLUME_UP: {
					std::cout << "VOLUME_UP" << std::endl;
					if (mr.setVolume(mr.getVolume() + 1) == RECORDER_ERROR) {
						cout << "setVolume failed" << endl;
					}
					std::cout << "Now, Volume is " << mr.getVolume() << std::endl;
				}
				break;
				case VOLUME_DOWN: {
					std::cout << "VOLUME_DOWN" << std::endl;
					if (mr.setVolume(mr.getVolume() - 1) == RECORDER_ERROR) {
						cout << "setVolume failed" << endl;
					}
					std::cout << "Now, Volume is " << mr.getVolume() << std::endl;
				}
				break;
				case APP_OFF: {
					std::cout << "SELECTED APP_OFF" << std::endl;
					mr.destroy();
					appRunning = false;
				}
				break;
				case DELETE_FILE : {
					if (unlink(filePath) == OK) {
						std::cout << "FILE DELETED" << std::endl;
					} else {
						std::cout << "DELETE FAILED ERRPR " << errno << std::endl;
					}
				}
				break;

			}
		}
	}

	void play_data()
	{
		if (isPlaying) {
			std::cout << "Already in playback" << std::endl;
			return;
		}

		std::cout << "playback " << filePath << std::endl;

		auto source = std::move(unique_ptr<FileInputDataSource>(new FileInputDataSource(filePath)));
		source->setSampleRate(16000);
		source->setChannels(2);
		source->setPcmFormat(AUDIO_FORMAT_TYPE_S16_LE);

		if (mp.create() == PLAYER_ERROR) {
			std::cout << "Mediaplayer::create failed" << std::endl;
			return;
		}

		mp.setObserver(shared_from_this());
		mp.setDataSource(std::move(source));

		if (mp.prepare() == PLAYER_OK) {
			isPlaying = true;
			if (mp.start() == PLAYER_OK) {
				// playback started
				return;
			}

			std::cout << "Mediaplayer::start failed" << std::endl;
			isPlaying = false;

			if (mp.unprepare() == PLAYER_ERROR) {
				std::cout << "Mediaplayer::unprepare failed" << std::endl;
			}
		} else {
			std::cout << "Mediaplayer::prepare failed" << std::endl;
		}

		if (mp.destroy() == PLAYER_ERROR) {
			std::cout << "Mediaplayer::destroy failed" << std::endl;
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

	MediaRecorderTest() : appRunning(false), isPaused(false), isPlaying(false) {}
	~MediaRecorderTest() {}

private:
	bool appRunning;
	bool isPaused;
	MediaRecorder mr;
	bool isPlaying;
	MediaPlayer mp;
};

extern "C"
{
	int mediarecorder_main(int argc, char *argv[])
	{
		up_cxxinitialize();

		auto mediaRecorderTest = make_shared<MediaRecorderTest>();
		mediaRecorderTest->start();

		return 0;
	}
}
