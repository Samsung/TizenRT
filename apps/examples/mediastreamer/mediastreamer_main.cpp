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
#include <fcntl.h>
#include <wifi_manager/wifi_manager.h>

#include <media/MediaPlayer.h>
#include <media/MediaRecorder.h>
#include <media/MediaPlayerObserverInterface.h>
#include <media/MediaRecorderObserverInterface.h>
#include <media/SocketOutputDataSource.h>
#include <iostream>
#include <memory>

#define SAY_OK_OR_ERROR(condition) if (ret == (condition)) { std::cout << "Ok" << std::endl; } else { std::cout << "Error" << std::endl; }
#define TEST_SERVER_IP_ADDR "192.168.1.215"
#define TEST_SERVER_PORT 9098

constexpr int APP_ON = 0;
constexpr int RECORDER_START = 1;
constexpr int RECORDER_PAUSE = 2;
constexpr int RECORDER_RESUME = 3;
constexpr int RECORDER_STOP = 4;
constexpr int APP_OFF = 5;

using namespace std;
using namespace media;
using namespace media::stream;

static std::string g_ipAddr = "";
static uint32_t g_port = 0;

static void wifi_sta_connected(wifi_manager_result_e result) {
	std::cout << __FUNCTION__ << std::endl;
}

static void wifi_sta_disconnected(wifi_manager_disconnect_e disconn) {
	std::cout << __FUNCTION__ << std::endl;
}

static wifi_manager_cb_s wifi_callbacks = {
	wifi_sta_connected,
	wifi_sta_disconnected,
	NULL,
	NULL,
	NULL,
};

class MediaStreamerTest : public MediaRecorderObserverInterface, public enable_shared_from_this<MediaStreamerTest>
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
			switch (userInput(APP_ON, APP_OFF))	{
				case APP_ON: {
					std::cout << "SELECTED APP ON" << std::endl;
					mr.create();
					mr.setObserver(shared_from_this());
					mr.setDataSource(unique_ptr<SocketOutputDataSource>(new SocketOutputDataSource(2, 16000, AUDIO_FORMAT_TYPE_S16_LE, g_ipAddr, 9098)));
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
			}
		}
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

	MediaStreamerTest() : appRunning(false) 
	{
		wifi_manager_result_e ret = WIFI_MANAGER_FAIL;
		wifi_manager_ap_config_s config;

		std::cout << "wifi_manager_init()...";
		ret = wifi_manager_init(&wifi_callbacks);
		SAY_OK_OR_ERROR(WIFI_MANAGER_SUCCESS);

		config.ssid_length = strlen(CONFIG_EXAMPLES_MEDIASTREAMER_SSID);
		config.passphrase_length = strlen(CONFIG_EXAMPLES_MEDIASTREAMER_PASSPHRASE);
		strncpy(config.ssid, CONFIG_EXAMPLES_MEDIASTREAMER_SSID, config.ssid_length + 1);
		strncpy(config.passphrase, CONFIG_EXAMPLES_MEDIASTREAMER_PASSPHRASE, config.passphrase_length + 1);
		config.ap_auth_type = (wifi_manager_ap_auth_type_e)CONFIG_EXAMPLES_MEDIASTREAMER_AUTHENTICATION;
		config.ap_crypto_type = (wifi_manager_ap_crypto_type_e)CONFIG_EXAMPLES_MEDIASTREAMER_CRYPTO;

		std::cout << "AP config: "
			<< config.ssid << "(" << config.ssid_length << ") "
			<< config.passphrase << "(" << config.passphrase_length << ") "
			<< config.ap_auth_type << ", " << config.ap_crypto_type << std::endl;

		/* current wifi mode is station, then this try will succeed */
		std::cout << "wifi_manager_connect_ap()...";
		ret = wifi_manager_connect_ap(&config);
		SAY_OK_OR_ERROR(WIFI_MANAGER_SUCCESS);
	}

	~MediaStreamerTest() {
		wifi_manager_result_e ret = WIFI_MANAGER_FAIL;

		std::cout << "wifi_manager_deinit()...";
		ret = wifi_manager_deinit();
		SAY_OK_OR_ERROR(WIFI_MANAGER_SUCCESS);
	}

private:
	bool appRunning;
	MediaRecorder mr;
	MediaPlayer mp;
};

extern "C"
{
	int mediastreamer_main(int argc, char *argv[])
	{
		/**
		 * Need to sleep for WiFi initialized.
		 */
		int i = 0;
		for(i = 0; i < 3; i++) {
			printf("Wait (%d/3) sec...\n", i + 1);
			sleep(1);
		}

		up_cxxinitialize();

		g_ipAddr = TEST_SERVER_IP_ADDR;
		g_port = TEST_SERVER_PORT;

		auto mediaStreamerTest = make_shared<MediaStreamerTest>();
		mediaStreamerTest->start();

		return 0;
	}
}

