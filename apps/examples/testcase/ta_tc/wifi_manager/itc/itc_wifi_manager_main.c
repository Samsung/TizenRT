/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <errno.h>
#include <wifi_manager/wifi_manager.h>
#include "tc_common.h"

extern sem_t tc_sem;
extern int working_tc;

static pthread_mutex_t g_wifi_manager_test_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_wifi_manager_test_cond;

#define SSID_MAX_LEN 32
#define PASSPHRASE_MAX_LEN 64
#define WIFITEST_SIGNAL\
	do {\
		pthread_mutex_lock(&g_wifi_manager_test_mutex);\
		pthread_cond_signal(&g_wifi_manager_test_cond);\
		pthread_mutex_unlock(&g_wifi_manager_test_mutex);\
	} while (0)

#define WIFITEST_WAIT\
	do {\
		pthread_mutex_lock(&g_wifi_manager_test_mutex);\
		pthread_cond_wait(&g_wifi_manager_test_cond, &g_wifi_manager_test_mutex);\
		pthread_mutex_unlock(&g_wifi_manager_test_mutex);\
	} while (0)

static void wifi_sta_connected_cb(void); // in station mode, connected to ap
static void wifi_sta_disconnected_cb(void); // in station mode, disconnected from ap
static void wifi_scan_ap_done_cb(wifi_manager_scan_info_s **scan_info, wifi_manager_scan_result_e res); // called when scanning ap is done

static wifi_manager_cb_s wifi_callbacks = {
	wifi_sta_connected_cb,
	wifi_sta_disconnected_cb,
	NULL,
	NULL,
	wifi_scan_ap_done_cb, // this callback function is called when scanning ap is done.
};

static void wifi_sta_connected_cb(void)
{
	printf("wifi_sta_connected: send signal!!! \n");
	WIFITEST_SIGNAL;
}

static void wifi_sta_disconnected_cb(void)
{
	printf("wifi_sta_disconnected: send signal!!! \n");
	WIFITEST_SIGNAL;
}

static void wifi_scan_ap_done_cb(wifi_manager_scan_info_s **scan_info, wifi_manager_scan_result_e res)
{
/******************************************************************
 * Make sure you copy the scan results onto a local data structure.
 * It will be deleted soon eventually as you exit this function.
******************************************************************/
	if (res == WIFI_SCAN_FAIL) {
		printf("WiFi scan failed\n");
		return;
	}
	wifi_manager_scan_info_s *wifi_scan_iter = *scan_info;
	while (wifi_scan_iter != NULL) {
		printf("SSID: %-20s, BSSID: %-20s, RSSI: %d, CH: %d, Phy_type: %d\n", \
				wifi_scan_iter->ssid, wifi_scan_iter->bssid, wifi_scan_iter->rssi, \
				wifi_scan_iter->channel, wifi_scan_iter->phy_mode);
		wifi_scan_iter = wifi_scan_iter->next;
	}
}

static int wifi_test_signal_init(void)
{
	int res = pthread_mutex_init(&g_wifi_manager_test_mutex, NULL);
	if (res != 0) {
		printf("wifi test signal init fail(%d) (%d)\n", res, errno);
		return -1;
	}
	res = pthread_cond_init(&g_wifi_manager_test_cond, NULL);
	if (res != 0) {
		printf("wifi test condition init fail(%d) (%d)\n", res, errno);
		res = pthread_mutex_destroy(&g_wifi_manager_test_mutex);
		if (res != 0) {
			printf("wifi test signal init fail(%d) (%d)\n", res, errno);
		}
		return -1;
	}
	return 0;
}

static void wifi_test_signal_deinit(void)
{
	int res = pthread_mutex_destroy(&g_wifi_manager_test_mutex);
	if (res != 0) {
		printf("wifi test signal deinit fail(%d) (%d)\n", res, errno);
	}
	res = pthread_cond_destroy(&g_wifi_manager_test_cond);
	if (res != 0) {
		printf("wifi test signal deinit fail(%d) (%d)\n", res, errno);
	}
}

/*********************************************************************
 * Test Cases
*********************************************************************/

/**
 * @testcase         itc_wifi_manager_init_deinit_p
 * @brief            Initialize  and de-initalization of wifi manager
 * @scenario
 * @apicovered       wifi_manager_init and wifi_manager_deinit
 * @precondition     none
 * @postcondition    none
 */

static void itc_wifi_manager_init_deinit_p(void)
{
	wifi_manager_result_e ret = WIFI_MANAGER_FAIL;
	ret = wifi_manager_init(&wifi_callbacks);
	TC_ASSERT_EQ("wifi_manager_init_p", ret, WIFI_MANAGER_SUCCESS);

	ret = wifi_manager_deinit();
	TC_ASSERT_EQ("wifi_manager_deinit_p", ret, WIFI_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         itc_wifi_manager_set_mode_p
 * @brief            Initialize  and de-initalization of wifi manager
 * @scenario
 * @apicovered
 * @precondition     wifi_manager_init
 * @postcondition    wifi_manager_deinit
 */

static void itc_wifi_manager_set_mode_p(void)
{
	wifi_manager_result_e ret = WIFI_MANAGER_FAIL;

	ret = wifi_manager_init(&wifi_callbacks);
	TC_ASSERT_EQ("wifi_manager_init", ret, WIFI_MANAGER_SUCCESS);

	wifi_manager_softap_config_s ap_config;

	strncpy(ap_config.ssid, CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_ITC_SOFTAP_SSID,
			strlen(CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_ITC_SOFTAP_SSID) + 1);

	ap_config.channel = CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_ITC_SOFTAP_CHANNEL;
	strncpy(ap_config.passphrase, CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_ITC_SOFTAP_PASSPHRASE,
			strlen(CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_ITC_SOFTAP_PASSPHRASE) + 1);

	printf("SoftAP config: %s(%d), %s(%d), %d\n", ap_config.ssid, strlen(CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_ITC_SOFTAP_SSID),
			ap_config.passphrase, strlen(CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_ITC_SOFTAP_PASSPHRASE), ap_config.channel);

	ret = wifi_manager_set_mode(SOFTAP_MODE, &ap_config);
	TC_ASSERT_EQ("wifi_manager_set_mode_p", ret, WIFI_MANAGER_SUCCESS);

	ret = wifi_manager_set_mode(STA_MODE, NULL);
	TC_ASSERT_EQ("wifi_manager_set_mode_p", ret, WIFI_MANAGER_SUCCESS);

	ret = wifi_manager_deinit();
	TC_ASSERT_EQ("wifi_manager_deinit", ret, WIFI_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         itc_wifi_manager_get_mode_p
 * @brief            Initialize  and de-initalization of wifi manager
 * @scenario
 * @apicovered
 * @precondition     wifi_manager_init
 * @postcondition    wifi_manager_deinit
 */

static void itc_wifi_manager_get_mode_p(void)
{
	wifi_manager_result_e ret = WIFI_MANAGER_FAIL;
	wifi_manager_info_s info;
	int ret_cmp = 0;
	int i;

	ret = wifi_manager_init(&wifi_callbacks);
	TC_ASSERT_EQ("wifi_manager_init", ret, WIFI_MANAGER_SUCCESS);

	ret = wifi_manager_get_info(&info);
	TC_ASSERT_EQ("wifi_manager_get_info", ret, WIFI_MANAGER_SUCCESS);
	TC_ASSERT_EQ("wifi_manager_get_info", info.mode, STA_MODE);

	wifi_manager_softap_config_s ap_config;
	strncpy(ap_config.ssid, CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_ITC_SOFTAP_SSID,
			strlen(CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_ITC_SOFTAP_SSID) + 1);

	ap_config.channel = CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_ITC_SOFTAP_CHANNEL;
	strncpy(ap_config.passphrase, CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_ITC_SOFTAP_PASSPHRASE,
			strlen(CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_ITC_SOFTAP_PASSPHRASE) + 1);

	ret = wifi_manager_set_mode(SOFTAP_MODE, &ap_config);
	TC_ASSERT_EQ("wifi_manager_set_mode", ret, WIFI_MANAGER_SUCCESS);

	ret = wifi_manager_get_info(&info);
	TC_ASSERT_EQ("wifi_manager_get_info", ret, WIFI_MANAGER_SUCCESS);
	TC_ASSERT_EQ("wifi_manager_get_info", info.mode, SOFTAP_MODE);

	printf("\nMAC Address: ");

	for (i = 0; i < 5; i++) {
		printf("%x:", info.mac_address[i]);
	}
	printf("%x\n", info.mac_address[5]);

	ret_cmp = strncmp(info.ssid, ap_config.ssid, strlen(CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_ITC_SOFTAP_SSID));
	TC_ASSERT_EQ("string cmp between SSID ", ret_cmp, 0);

	ret = wifi_manager_deinit();
	TC_ASSERT_EQ("wifi_manager_deinit", ret, WIFI_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

/**
 * @testcase         itc_wifi_manager_connect_disconnect_ap_p
 * @brief            Initialize  and de-initalization of wifi manager
 * @scenario
 * @apicovered
 * @precondition     wifi_manager_init
 * @postcondition    wifi_manager_deinit
 */

static void itc_wifi_manager_connect_disconnect_ap_p(void)
{
	wifi_manager_result_e ret = WIFI_MANAGER_FAIL;
	ret = wifi_manager_init(&wifi_callbacks);
	TC_ASSERT_EQ("wifi_manager_init", ret, WIFI_MANAGER_SUCCESS);

	ret = wifi_manager_set_mode(STA_MODE, NULL);
	TC_ASSERT_EQ("wifi_manager_set_mode", ret, WIFI_MANAGER_SUCCESS);

	wifi_manager_ap_config_s config;
	config.ssid_length = strlen(CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_ITC_SSID);
	config.passphrase_length = strlen(CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_ITC_PASSPHRASE);
	strncpy(config.ssid, CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_ITC_SSID, config.ssid_length + 1);
	strncpy(config.passphrase, CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_ITC_PASSPHRASE, config.passphrase_length + 1);
	config.ap_auth_type = (wifi_manager_ap_auth_type_e)CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_ITC_AUTHENTICATION;
	config.ap_crypto_type = (wifi_manager_ap_crypto_type_e)CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_ITC_CRYPTO;
	printf("AP config: %s(%d), %s(%d), %d %d\n", config.ssid, config.ssid_length, config.passphrase, \
			config.passphrase_length, config.ap_auth_type, config.ap_crypto_type);

	ret = wifi_manager_connect_ap(&config);
	TC_ASSERT_EQ("wifi_manager_connect_ap", ret, WIFI_MANAGER_SUCCESS);
	WIFITEST_WAIT;
	sleep(5);
	ret = wifi_manager_disconnect_ap();
	WIFITEST_WAIT;
	TC_ASSERT_EQ("wifi_manager_disconnect_ap", ret, WIFI_MANAGER_SUCCESS);

	ret = wifi_manager_deinit();
	TC_ASSERT_EQ("wifi_manager_deinit", ret, WIFI_MANAGER_SUCCESS);

	TC_SUCCESS_RESULT();
}

/**
 * @testcase         itc_wifi_manager_scan_ap_p
 * @brief            Initialize  and de-initalization of wifi manager
 * @scenario
 * @apicovered
 * @precondition     wifi_manager_init
 * @postcondition    wifi_manager_deinit
 */

static void itc_wifi_manager_scan_ap_p(void)
{
	wifi_manager_result_e ret = WIFI_MANAGER_FAIL;

	ret = wifi_manager_init(&wifi_callbacks);
	TC_ASSERT_EQ("wifi_manager_init", ret, WIFI_MANAGER_SUCCESS);

	ret = wifi_manager_scan_ap();
	TC_ASSERT_EQ("wifi_manager_scan_ap", ret, WIFI_MANAGER_SUCCESS);
	sleep(5);
	ret = wifi_manager_deinit();
	TC_ASSERT_EQ("wifi_manager_deinit", ret, WIFI_MANAGER_SUCCESS);

	TC_SUCCESS_RESULT();
}

/*******************************************************************************************************************/

int wifi_manager_itc(int argc, FAR char *argv[])
{
	sem_wait(&tc_sem);
	working_tc++;

	total_pass = 0;
	total_fail = 0;

	printf("=== TINYARA WIFI_MANAGER ITC START! ===\n");

	itc_wifi_manager_init_deinit_p();

	itc_wifi_manager_set_mode_p();

	itc_wifi_manager_get_mode_p(); // set softap mode inside this function

	itc_wifi_manager_connect_disconnect_ap_p(); // change to station mode and try to connect to ap

	itc_wifi_manager_scan_ap_p(); // Reinitialized wifi manager with the callback hander for scan results

	printf("\n=== TINYARA WIFI_MANAGER ITC COMPLETE ===\n");
	printf("\t\tTotal pass : %d\n\t\tTotal fail : %d\n", total_pass, total_fail);

	working_tc--;
	sem_post(&tc_sem);

	return 0;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int itc_wifi_manager_main(int argc, char *argv[])
#endif
{
	int ssid_len = strlen(CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_ITC_SSID);
	int passphrase_len = strlen(CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_ITC_PASSPHRASE);
	int softap_ssid_len = strlen(CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_ITC_SOFTAP_SSID);
	int softap_passphrase_len = strlen(CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_ITC_SOFTAP_PASSPHRASE);

	if ((ssid_len >= SSID_MAX_LEN) || (softap_ssid_len >= SSID_MAX_LEN) || (passphrase_len >= PASSPHRASE_MAX_LEN) || (softap_passphrase_len >= PASSPHRASE_MAX_LEN)) {
		printf("AP or SoftAP configuration fails: too long ssid or passphrase\n");
		printf("Make sure that length of SSID < SSID_MAX_LEN and length of passphrase < PASSPHRASE_MAX_LEN\n");
		return -1;
	}

	int res = wifi_test_signal_init();
	if (res < 0) {
		return -1;
	}

	wifi_manager_itc(argc, argv);

	wifi_test_signal_deinit();
	return 0;
}
