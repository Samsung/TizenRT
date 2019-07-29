/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

#include "WiFiConnector.h"

#include <stdio.h>
#include <string.h>
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

namespace MediaPlayerApp
{
bool WiFiConnector::sg_wifiIsConnected = false;
std::mutex WiFiConnector::sg_wifiMutex;
std::condition_variable WiFiConnector::sg_wifiCondv;
std::chrono::seconds WiFiConnector::WAIT_WIFI_TIMEOUT(5);

int WiFiConnector::connect(void)
{
	static wifi_manager_cb_s wifi_callbacks = {
		[](wifi_manager_result_e result) {
			/* wifi_sta_connected */
			printf("result %d\n", (int)result);
			WIFI_CONNECTED_NOTIFY();
		},
		[](wifi_manager_disconnect_e result) {
			/* wifi_sta_disconnected */
			printf("result %d\n", (int)result);
		},
		NULL,
		NULL,
		NULL,
	};

	wifi_manager_result_e ret = wifi_manager_init(&wifi_callbacks);
	if (ret != WIFI_MANAGER_SUCCESS) {
		printf("wifi_manager_init failed, ret %d\n", (int)ret);
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

	printf("wifi_manager_connect_ap...\n");
	ret = wifi_manager_connect_ap(&config);
	if (ret != WIFI_MANAGER_SUCCESS) {
		printf("wifi_manager_connect_ap failed, ret %d\n", (int)ret);
		wifi_manager_deinit();
		return -1;
	}

	printf("WIFI_CONNECTED_WAIT...\n");
	WIFI_CONNECTED_WAIT();
	if (!WIFI_CONNECTED_CHECKFLAG()) {
		printf("wifi connect failed, timeout!\n");
		wifi_manager_deinit();
		return -1;
	}

	return 0;
}

int WiFiConnector::disconnect(void)
{
	wifi_manager_result_e ret = wifi_manager_deinit();
	if (ret != WIFI_MANAGER_SUCCESS) {
		printf("wifi_manager_deinit failed, ret %d\n", (int)ret);
		return -1;
	}

	return 0;
}
}
