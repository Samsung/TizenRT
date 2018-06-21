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
#include <media/FileOutputDataSource.h>
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

static const char *filePath = "/ramfs/record";

class MediaRecorderTest : public MediaRecorderObserverInterface, public enable_shared_from_this<MediaRecorderTest>
{
public:
	void onRecordStarted(Id id)
	{
		std::cout << "onRecordStarted" << std::endl;		
	}
	void onRecordPaused(Id id)
	{
		std::cout << "onRecordPaused" << std::endl;
	}
	void onRecordFinished(Id id)
	{
		std::cout << "onRecordFinished" << std::endl;
		mr.unprepare();
	}
	void onRecordError(Id id)
	{
		std::cout << "onRecordError!! please select Stop!!" << std::endl;

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
		int fd;
		char *buffer;
		int num_read;
		unsigned int size;

		p_out = pcm_open(0, 0, PCM_OUT, &pcmConfig);

		if (pcm_get_file_descriptor(p_out) < 0) {
			printf("pcm open fail\n%s\n", pcm_get_error(p_out));
			return;
		}

		size = pcm_frames_to_bytes(p_out, pcm_get_buffer_size(p_out));
		buffer = (char *)malloc(size);

		fd = open(filePath, O_RDONLY);

		if (fd < 0) {
			printf("file open fail : %d\n", fd);
			free(buffer);
			buffer = NULL;
			pcm_close(p_out);
			p_out = NULL;
			return;
		}

		printf("fd = %d\n", fd);

		printf("Playback start!!\n");
		do {
			num_read = read(fd, buffer, size);
			printf("num_read =%d\n", num_read);
			if (num_read > 0) {
				pcm_writei(p_out, buffer, pcm_bytes_to_frames(p_out, num_read));
			}
		} while (num_read > 0);

		if (buffer != NULL) {
			free(buffer);
			buffer = NULL;
		}
		if (fd > 0) {
			close(fd);
		}

		pcm_close(p_out);
		
		printf("Playback done.\n");
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

	MediaRecorderTest() : p_out(nullptr), appRunning(false), isPaused(false) 
	{
		pcmConfig.channels = 2;
		pcmConfig.rate = 16000;
		pcmConfig.format = PCM_FORMAT_S16_LE;
		pcmConfig.period_size = 1024;
		pcmConfig.period_count = 2;
	}
	~MediaRecorderTest() {}

private:
	struct pcm_config pcmConfig;
	struct pcm *p_out;
	bool appRunning;
	bool isPaused;
	MediaRecorder mr;
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
