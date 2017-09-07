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
#include <apps/shell/tash.h>
#include <wifi_manager/wifi_manager.h>
#include "../../../tc_common.h"

extern sem_t tc_sem;
extern int working_tc;

static void wifi_sta_connected(void);		// in station mode, connected to ap
static void wifi_sta_disconnected(void);	// in station mode, disconnected from ap
static void wifi_softap_sta_joined(void);	// in softap mode, a station joined
static void wifi_softap_sta_left(void);		// in softap mode, a station left
static void wifi_sta_scan_ap_done(wifi_manager_scan_info_s **scan_info, wifi_manager_scan_result_e res); // in station mode, scanning ap is done

static wifi_manager_cb_s wifi_callbacks = {
	NULL,	//wifi_sta_connected,
	NULL,	//wifi_sta_disconnected,
	NULL,	//wifi_softap_sta_joined,
	NULL,	//wifi_softap_sta_left,
	wifi_sta_scan_ap_done,	// in station mode, this callback function is called when scanning ap is done.
};

static wifi_manager_cb_s wifi_null_callbacks = {
	NULL,	//wifi_sta_connected,
	NULL,	//wifi_sta_disconnected,
	NULL,	//wifi_softap_sta_joined,
	NULL,	//wifi_softap_sta_left,
	NULL,	// in station mode, this callback function is called when scanning ap is done.
};

static void wifi_sta_scan_ap_done(wifi_manager_scan_info_s **scan_info, wifi_manager_scan_result_e res)
{
	printf("[pkes] T%d --> %s\n", getpid(), __FUNCTION__);
	/* Make sure you copy the scan results onto a local data structure.
	 * It will be deleted soon eventually as you exit this function.
	 */
	if (res == WIFI_SCAN_FAIL) {
		printf("WiFi scan failed\n");
		return;
	}
	wifi_manager_scan_info_s *wifi_scan_iter = *scan_info;
	while (wifi_scan_iter != NULL) {
		printf("WiFi AP SSID: %-20s, WiFi AP BSSID: %-20s, WiFi Rssi: %d\n", wifi_scan_iter->ssid, wifi_scan_iter->bssid, wifi_scan_iter->rssi);
		wifi_scan_iter = wifi_scan_iter->next;
	}
}

static void utc_wifi_manager_init_n(void)
{
	wifi_manager_result_e ret = WIFI_MANAGER_FAIL;

	ret = wifi_manager_init(NULL);

	TC_ASSERT_EQ("wifi_manager_init", ret, WIFI_MANAGER_FAIL);
	TC_SUCCESS_RESULT();
}

static void utc_wifi_manager_init_p(void)
{
	wifi_manager_result_e ret = WIFI_MANAGER_FAIL;

	ret = wifi_manager_init(&wifi_callbacks);

	TC_ASSERT_EQ("wifi_manager_init", ret, WIFI_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_wifi_manager_set_mode_n(void)
{
	wifi_manager_result_e ret = WIFI_MANAGER_FAIL;

	ret = wifi_manager_set_mode(SOFTAP_MODE, NULL);

	TC_ASSERT_EQ("wifi_manager_set_mode", ret, WIFI_MANAGER_FAIL);

	wifi_manager_softap_config_s ap_config = {"PKES", 1, "1111122222"};

	ret = wifi_manager_set_mode(WIFI_NONE, &ap_config);

	TC_ASSERT_EQ("wifi_manager_set_mode", ret, WIFI_MANAGER_INVALID_ARGS);
	TC_SUCCESS_RESULT();
}

static void utc_wifi_manager_set_mode_p(void)
{
	wifi_manager_result_e ret = WIFI_MANAGER_FAIL;

	wifi_manager_softap_config_s ap_config = {"PKES", 1, "1111122222"};

	ret = wifi_manager_set_mode(SOFTAP_MODE, &ap_config);

	TC_ASSERT_EQ("wifi_manager_set_mode", ret, WIFI_MANAGER_SUCCESS);

	ret = wifi_manager_set_mode(STA_MODE, NULL);

	TC_ASSERT_EQ("wifi_manager_set_mode", ret, WIFI_MANAGER_SUCCESS);

	TC_SUCCESS_RESULT();
}

static void utc_wifi_manager_get_mode_n(void)
{
	wifi_manager_result_e ret = WIFI_MANAGER_FAIL;
	wifi_manager_info_s *info = NULL;

	ret = wifi_manager_get_info(info);

	TC_ASSERT_EQ("wifi_manager_get_mode", ret, WIFI_MANAGER_FAIL);
	TC_SUCCESS_RESULT();
}

static void utc_wifi_manager_get_mode_p(void)
{
	wifi_manager_result_e ret = WIFI_MANAGER_FAIL;
	wifi_manager_info_s info;
	int ret_cmp = 0;

	ret = wifi_manager_get_info(&info);

	TC_ASSERT_EQ("wifi_manager_get_mode", ret, WIFI_MANAGER_SUCCESS);
	TC_ASSERT_EQ("wifi_manager_get_mode", info.mode, STA_MODE);

	wifi_manager_softap_config_s ap_config = {"PKES", 1, "1111122222"};
	ret = wifi_manager_set_mode(SOFTAP_MODE, &ap_config);

	TC_ASSERT_EQ("wifi_manager_get_mode", ret, WIFI_MANAGER_SUCCESS);

	ret = wifi_manager_get_info(&info);

	TC_ASSERT_EQ("wifi_manager_get_mode", ret, WIFI_MANAGER_SUCCESS);
	TC_ASSERT_EQ("wifi_manager_get_mode", info.mode, SOFTAP_MODE);
	ret_cmp = strncmp(info.ssid, ap_config.ssid, sizeof(ap_config.ssid));
	TC_ASSERT_EQ("wifi_manager_get_mode", ret_cmp, 0);

	TC_SUCCESS_RESULT();
}


static void utc_wifi_manager_connect_ap_n(void)
{
	wifi_manager_result_e ret = WIFI_MANAGER_FAIL;

	/* Connect to AP
	 * This negative tc function will fail
	 * so that you do not need to change the ap configuration
	 */
	wifi_manager_ap_config_s config = {\
		"NETGEAR99", 9,\
		"artikwifi", 9, \
		WIFI_MANAGER_AUTH_WPA2_PSK, \
		WIFI_MANAGER_CRYPTO_AES
	};

	/* current wifi mode is softap, then this try will fail */
	ret = wifi_manager_connect_ap(&config);

	TC_ASSERT_EQ("wifi_manager_connect_ap", ret, WIFI_MANAGER_FAIL);
	TC_SUCCESS_RESULT();
}

static void utc_wifi_manager_connect_ap_p(void)
{
	wifi_manager_result_e ret = WIFI_MANAGER_FAIL;

	/* change to station mode */
	ret = wifi_manager_set_mode(STA_MODE, NULL);
	TC_ASSERT_EQ("wifi_manager_connect_ap", ret, WIFI_MANAGER_SUCCESS);

	/* Connect to AP
	 * You need to change configuration of ap which you actually try to connect to.
	 * Otherwise, this function will fail.
	 */
	wifi_manager_ap_config_s config = {\
		"NETGEAR99", 9,\
		"artikwifi", 9, \
		WIFI_MANAGER_AUTH_WPA2_PSK, \
		WIFI_MANAGER_CRYPTO_AES
	};

	/* current wifi mode is station, then this try will succeed */
	ret = wifi_manager_connect_ap(&config);

	TC_ASSERT_EQ("wifi_manager_connect_ap", ret, WIFI_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_wifi_utils_disconnect_ap_p(void)
{
	wifi_manager_result_e ret = WIFI_MANAGER_FAIL;

	ret = wifi_utils_disconnect_ap();

	TC_ASSERT_EQ("wifi_manager_deconnect_ap", ret, WIFI_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

/* second disconnection trial will get failed */
static void utc_wifi_utils_disconnect_ap_n(void)
{
	wifi_manager_result_e ret = WIFI_MANAGER_FAIL;

	ret = wifi_utils_disconnect_ap();

	TC_ASSERT_EQ("wifi_manager_deconnect_ap", ret, WIFI_MANAGER_FAIL);
	TC_SUCCESS_RESULT();
}

static void utc_wifi_manager_deinit_p(void)
{
	wifi_manager_result_e ret = WIFI_MANAGER_FAIL;

	ret = wifi_manager_deinit();

	TC_ASSERT_EQ("wifi_manager_deinit", ret, WIFI_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

/* Initialize wifi manager without callback for scan results
 * this leads to failure.
 */
static void utc_wifi_manager_deinit_n(void)
{
	wifi_manager_result_e ret = WIFI_MANAGER_FAIL;

	ret = wifi_manager_deinit();

	TC_ASSERT_EQ("wifi_manager_deinit", ret, WIFI_MANAGER_DEINITIALIZED);
	TC_SUCCESS_RESULT();
}

static void utc_wifi_manager_scan_ap_n(void)
{
	wifi_manager_result_e ret = WIFI_MANAGER_FAIL;

	ret = wifi_manager_init(&wifi_null_callbacks);
	TC_ASSERT_EQ("wifi_manager_scan_ap", ret, WIFI_MANAGER_SUCCESS);

	ret = wifi_manager_scan_ap();
	TC_ASSERT_EQ("wifi_manager_scan_ap", ret, WIFI_MANAGER_INVALID_ARGS);

	TC_SUCCESS_RESULT();
}

static void utc_wifi_manager_scan_ap_p(void)
{
	wifi_manager_result_e ret = WIFI_MANAGER_FAIL;

	ret = wifi_manager_deinit();
	TC_ASSERT_EQ("wifi_manager_scan_ap", ret, WIFI_MANAGER_SUCCESS);

	ret = wifi_manager_init(&wifi_callbacks);
	TC_ASSERT_EQ("wifi_manager_scan_ap", ret, WIFI_MANAGER_SUCCESS);

	ret = wifi_manager_scan_ap();
	TC_ASSERT_EQ("wifi_manager_scan_ap", ret, WIFI_MANAGER_SUCCESS);

	TC_SUCCESS_RESULT();
}

int wifi_manager_utc(int argc, FAR char *argv[])
{
	sem_wait(&tc_sem);
	working_tc++;

	total_pass = 0;
	total_fail = 0;

	printf("=== TINYARA WIFI_MANAGER TC START! ===\n");

	utc_wifi_manager_init_n();
	utc_wifi_manager_init_p();

	utc_wifi_manager_set_mode_n();
	utc_wifi_manager_set_mode_p();

	utc_wifi_manager_get_mode_n();
	utc_wifi_manager_get_mode_p();	// set softap mode inside this function

	/* shoud define ap config first in utc_wifi_manager_connect_ap_n()
	 * and utc_wifi_manager_connect_ap_p() before trying these two tests
	 * Otherwise, you will get failed.
	 */
	utc_wifi_manager_connect_ap_n();	// try to connect to ap in softap mode
	utc_wifi_manager_connect_ap_p();	// change to station mode and try to connect to ap

	/* wait for 10 seconds to acquire ip address from the connected ap.
	 * Even though ip address is not assigned, this unit test may pass if joing ap is successful.
	 */
	sleep(10);

	utc_wifi_utils_disconnect_ap_p();
	utc_wifi_utils_disconnect_ap_n();	//  Should be run after positive tc, that is, the second disconnect gets failed.

	utc_wifi_manager_deinit_p();
	utc_wifi_manager_deinit_n(); // Should be run after positive tc, that is, the second deinit gets failed.

	utc_wifi_manager_scan_ap_n(); // Get failed becasue there is no callback hander for scan results
	utc_wifi_manager_scan_ap_p(); // Reinitialized wifi manager with the callback hander for scan results

	printf("\n=== TINYARA WIFI_MANAGER TC COMPLETE ===\n");
	printf("\t\tTotal pass : %d\n\t\tTotal fail : %d\n", total_pass, total_fail);

	working_tc--;
	sem_post(&tc_sem);

	return 0;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int utc_wifi_manager_main(int argc, char *argv[])
#endif
{
#ifdef CONFIG_TASH
	int ret = 0;
	ret = tash_cmd_install("wifi_mgr_utc", wifi_manager_utc, TASH_EXECMD_SYNC);
	if (ret != 0) {
		printf("wifi_mgr_utc registration is failed!\n");
	}
#else
	wifi_manager_utc(argc, argv);
#endif

	return 0;
}
