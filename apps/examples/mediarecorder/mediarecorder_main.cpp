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

#include <media/MediaRecorder.h>
#include <media/MediaRecorderObserverInterface.h>
#include <media/FileOutputDataSource.h>
#include <iostream>
#include <memory>

constexpr int APP_ON = 0;
constexpr int RECORDER_START = 1;
constexpr int RECORDER_PAUSE = 2;
constexpr int RECORDER_RESUME = 3;
constexpr int RECORDER_STOP = 4;
constexpr int APP_OFF = 5;
constexpr int PLAY_DATA = 6;

using namespace std;
using namespace media;
using namespace media::stream;

class MediaRecorderTest : public MediaRecorderObserverInterface, public enable_shared_from_this<MediaRecorderTest>
{
public:
	void onRecordStarted(Id id)
	{
		std::cout << "onRecordStarted" << std::endl;		
	}
	void onRecordFinished(Id id)
	{
		std::cout << "onRecordFinished" << std::endl;
	}
	void onRecordError(Id id)
	{
		std::cout << "onRecordError" << std::endl;
	}

	void start()
	{
		appRunning = true;
	
		while (appRunning) {
			printRecorderMenu();
			switch (userInput(APP_ON, PLAY_DATA))	{
				case APP_ON: {
					std::cout << "SELECTED APP ON" << std::endl;
					mr.create();
					mr.setObserver(shared_from_this());
					mr.setDataSource(unique_ptr<FileOutputDataSource>(new FileOutputDataSource(2, 16000, PCM_FORMAT_S16_LE, "/ramfs/record")));
				}
				break;
				case RECORDER_START: {
					std::cout << "SELECTED RECORDER_START" << std::endl;
					if (mr.prepare() == RECORDER_OK) {
						mr.start();
					}
					else {
						std::cout << "prepare failed" << std::endl;
					}
				}
				break;
				case RECORDER_PAUSE: {
					std::cout << "SELECTED RECORDER_PAUSE" << std::endl;
					mr.pause();
				}
				break;
				case RECORDER_RESUME: {
					std::cout << "SELECTED RECORDER_RESUME" << std::endl;
					mr.start();
				}
				break;
				case RECORDER_STOP: {
					std::cout << "SELECTED RECORDER_STOP" << std::endl;
					if (mr.stop() == RECORDER_OK) {
						mr.unprepare();
					}
				}
				break;
				case APP_OFF: {
					std::cout << "SELECTED APP_OFF" << std::endl;
						mr.destroy();
						appRunning = false;
				}
				break;
				case PLAY_DATA: {
					std::cout << "PLAY_DATA" << std::endl;
					play_data();
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

		fd = open("/ramfs/record", O_RDONLY);

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
		
		std::cout << "0. RECORDER APP ON" << std::endl;
		std::cout << "1. RECORDER Start" << std::endl;
		std::cout << "2. RECORDER Pause" << std::endl;
		std::cout << "3. RECORDER Resume" << std::endl;
		std::cout << "4. RECORDER Stop" << std::endl;
		std::cout << "5. RECORDER APP OFF" << std::endl;
		std::cout << "6. play Data" << std::endl;

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

	MediaRecorderTest() : p_out(nullptr), appRunning(false) 
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

