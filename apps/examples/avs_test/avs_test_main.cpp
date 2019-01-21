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
#include <fcntl.h>
#include <wifi_manager/wifi_manager.h>
#ifdef CONFIG_NETUTILS_NTPCLIENT
#include <protocols/ntpclient.h>
#include <time.h>
#endif
#include <pthread.h>
#include <iostream>
#include <avs-device-sdk/avs-device-sdk.h>

#ifndef CONFIG_EXAMPLES_AVS_TEST_SSID
#define CONFIG_EXAMPLES_AVS_TEST_SSID "Unknown"
#endif
#ifndef CONFIG_EXAMPLES_AVS_TEST_PASSPHRASE
#define CONFIG_EXAMPLES_AVS_TEST_PASSPHRASE "Unknown"
#endif
#ifndef CONFIG_EXAMPLES_AVS_TEST_AUTHENTICATION
#define CONFIG_EXAMPLES_AVS_TEST_AUTHENTICATION 3
#endif
#ifndef CONFIG_EXAMPLES_AVS_TEST_CRYPTO
#define CONFIG_EXAMPLES_AVS_TEST_CRYPTO 3
#endif

#ifndef CONFIG_AVS_TEST_THREAD_STACKSIZE
#define CONFIG_AVS_TEST_THREAD_STACKSIZE 16384
#endif

extern "C" {

#ifdef CONFIG_NETUTILS_NTPCLIENT

static struct ntpc_server_conn_s server_conn[] = {
	{"0.ubuntu.pool.ntp.org", CONFIG_NETUTILS_NTPCLIENT_DEFAULT_SERVER_PORT},
	{CONFIG_NETUTILS_NTPCLIENT_DEFAULT_SERVER, CONFIG_NETUTILS_NTPCLIENT_DEFAULT_SERVER_PORT}
};

static void ntp_link_error(void)
{
	printf("ntp_link_error() callback is called\n");
}

static bool sync_time_from_ntp(int guarantee_secs)
{
	printf("[%s] guarantee_secs %d\n", __FUNCTION__, guarantee_secs);
	bool time_sync = false;

	if (ntpc_get_status() != NTP_RUNNING) {
		struct timespec pre_tp;
		clock_gettime(CLOCK_REALTIME, &pre_tp);

		struct timespec init_tp;
		init_tp.tv_sec = 0;
		init_tp.tv_nsec = 0;
		clock_settime(CLOCK_REALTIME, &init_tp);

		if (ntpc_start(server_conn, sizeof(server_conn) / sizeof(server_conn[0]), 0, (void *)ntp_link_error) < 0) {
			printf("ntpc_start() failed\n");
		} else {
			printf("ntpc_start() succeed\n");

			while (1) {
				struct timespec sync_tp;
				clock_gettime(CLOCK_REALTIME, &sync_tp);
				if ((init_tp.tv_sec + 1000) < sync_tp.tv_sec) {
					time_sync = true;
					break;
				} else if ((init_tp.tv_sec + guarantee_secs) < sync_tp.tv_sec) {
					break;
				}
				usleep(100000);
			}

			if (time_sync) {
				printf("ntpc_time sync done\n");
			} else {
				printf("ntpc_time sync fail\n");
				clock_settime(CLOCK_REALTIME, &pre_tp);
			}
		}
	} else {
		printf("ntpc already running\n");
	}

	return time_sync;
}
#endif


static bool sgIsWifiConnected = false;
static std::mutex sgWifiMutex;
static std::condition_variable sgWifiCondv;

#define WIFI_CONNECTED_RESETFLAG() sgIsWifiConnected = false

#define WIFI_CONNECTED_CHECKFLAG() sgIsWifiConnected

#define WIFI_CONNECTED_NOTIFY() \
	do {\
		std::lock_guard<std::mutex> lock(sgWifiMutex);\
		sgIsWifiConnected = true;\
		sgWifiCondv.notify_one();\
	} while (0)

#define WIFI_CONNECTED_WAIT() \
	do {\
		std::unique_lock<std::mutex> lock(sgWifiMutex);\
		sgWifiCondv.wait_for(lock, std::chrono::seconds(10), []{return sgIsWifiConnected;});\
	} while (0)

static void wifi_sta_connected(wifi_manager_result_e result) {
	printf("[%s] result %d\n", __FUNCTION__, (int)result);
	WIFI_CONNECTED_NOTIFY();
}

static void wifi_sta_disconnected(wifi_manager_disconnect_e result) {
	printf("[%s] result %d\n", __FUNCTION__, (int)result);
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
		printf("wifi_manager_init failed, ret %d\n", (int)ret);
		return -1;
	}

	wifi_manager_ap_config_s config;
	config.ssid_length = strlen(CONFIG_EXAMPLES_AVS_TEST_SSID);
	config.passphrase_length = strlen(CONFIG_EXAMPLES_AVS_TEST_PASSPHRASE);
	strncpy(config.ssid, CONFIG_EXAMPLES_AVS_TEST_SSID, config.ssid_length + 1);
	strncpy(config.passphrase, CONFIG_EXAMPLES_AVS_TEST_PASSPHRASE, config.passphrase_length + 1);
	config.ap_auth_type = (wifi_manager_ap_auth_type_e)CONFIG_EXAMPLES_AVS_TEST_AUTHENTICATION;
	config.ap_crypto_type = (wifi_manager_ap_crypto_type_e)CONFIG_EXAMPLES_AVS_TEST_CRYPTO;

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

static int wifi_disconnect()
{
	wifi_manager_result_e ret = wifi_manager_deinit();
	if (ret != WIFI_MANAGER_SUCCESS) {
		printf("wifi_manager_deinit failed, ret %d\n", (int)ret);
		return -1;
	}

	return 0;
}

void *avs_test_entry(void *param)
{
	if (wifi_connect() != 0) {
		return NULL;
	}

#ifdef CONFIG_NETUTILS_NTPCLIENT
	if (!sync_time_from_ntp(16)) {
		// time hasn't been synced.
	}
#endif

	int ret = avsSampleAppEntry();
	printf("AVS Device SDK SampleApplication returned %d\n", ret);

#ifdef CONFIG_NETUTILS_NTPCLIENT
#ifndef CONFIG_DISABLE_SIGNALS
	ntpc_stop();
#endif
#endif

	wifi_disconnect();
	return NULL;
}

int avs_test_main(int argc, char **argv)
{
	pthread_attr_t attr;
	struct sched_param sparam;
	sparam.sched_priority = 100;

	pthread_attr_init(&attr);
	pthread_attr_setstacksize(&attr, CONFIG_AVS_TEST_THREAD_STACKSIZE);
	pthread_attr_setschedparam(&attr, &sparam);

	int status;
	pthread_t thread;
	status = pthread_create(&thread, &attr, avs_test_entry, NULL);
	if (status != 0) {
		printf("[%s] pthread_create failed, status=%d\n", __FUNCTION__, status);
		return 0;
	}

	pthread_setname_np(thread, "avs_test_main");
	pthread_join(thread, NULL);
	return 0;
}

}
