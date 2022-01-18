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
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <errno.h>
#include <wifi_manager/wifi_manager.h>
#include <wifi_mbox/wifi_mbox.h>
#include "wifi_mock.h"
#include "tc_common.h"

#define WIFI_PROFILE_PATH "/mnt/"
#define WIFI_PROFILE_FILENAME_INTERNAL "wifi_connected.conf"

#define WEVT_CONNECTED 1
#define WEVT_DISCONNECTED 2
#define WEVT_CONNFAIL 3
#define WEVT_SCAN_SUCCESS 4
#define WEVT_SCAN_FAIL 5
#define WEVT_OK 6
#define WEVT_FAIL 7

static struct wifi_mbox *g_queue = NULL;
static wifi_manager_softap_config_s g_softap_config;
static wifi_manager_ap_config_s g_ap_config;

void wifi_sta_connected(wifi_manager_cb_msg_s msg, void *arg);	  // in station mode, connected to ap
void wifi_sta_disconnected(wifi_manager_cb_msg_s msg, void *arg); // in station mode, disconnected from ap
void wifi_sta_dup_connected(wifi_manager_cb_msg_s msg, void *arg);
void wifi_sta_dup_disconnected(wifi_manager_cb_msg_s msg, void *arg);

void wifi_softap_sta_joined(wifi_manager_cb_msg_s msg, void *arg); // in softap mode, a station joined
void wifi_softap_sta_left(wifi_manager_cb_msg_s msg, void *arg);   // in softap mode, a station left
void wifi_scan_ap_done(wifi_manager_cb_msg_s msg, void *arg);	   // called when scanning ap is done

static wifi_manager_cb_s wifi_callbacks = {
	wifi_sta_connected,
	wifi_sta_disconnected,
	NULL,			   //wifi_softap_sta_joined,
	NULL,			   //wifi_softap_sta_left,
	wifi_scan_ap_done, // this callback function is called when scanning ap is done.
};

#if 0
static wifi_manager_cb_s wifi_null_callbacks = {
	NULL, //wifi_sta_connected,
	NULL, //wifi_sta_disconnected,
	NULL, //wifi_softap_sta_joined,
	NULL, //wifi_softap_sta_left,
	NULL, // in station mode, this callback function is called when scanning ap is done.
};

static wifi_manager_cb_s wifi_dup_callbacks = {
	wifi_sta_dup_connected,
	wifi_sta_dup_disconnected,
	NULL,
	NULL,
	NULL,
};
#endif

void wifi_sta_connected(wifi_manager_cb_msg_s msg, void *arg)
{
  int res = WEVT_CONNECTED;
  if (msg.res != WIFI_MANAGER_SUCCESS) {
    res = WEVT_CONNFAIL;
  }
  WMBOX_SIGNAL(res, g_queue);
}

void wifi_sta_dup_connected(wifi_manager_cb_msg_s msg, void *arg)
{
	return;
}

void wifi_sta_disconnected(wifi_manager_cb_msg_s msg, void *arg)
{
  WMBOX_SIGNAL(WEVT_DISCONNECTED, g_queue);
}

void wifi_sta_dup_disconnected(wifi_manager_cb_msg_s msg, void *arg)
{
	return;
}

void wifi_scan_ap_done(wifi_manager_cb_msg_s msg, void *arg)
{
	/* Make sure you copy the scan results onto a local data structure.
	 * It will be deleted soon eventually as you exit this function.
	 */
	if (msg.res != WIFI_MANAGER_SUCCESS || !msg.scanlist) {
		WMBOX_SIGNAL(WEVT_SCAN_FAIL, g_queue);
		return;
	}
	wifi_manager_scan_info_s *wifi_scan_iter = msg.scanlist;
	while (wifi_scan_iter != NULL) {
		printf("SSID: %-20s, BSSID: %-20s, RSSI: %d, CH: %d, Phy_type: %d\n",
			   wifi_scan_iter->ssid, wifi_scan_iter->bssid, wifi_scan_iter->rssi,
			   wifi_scan_iter->channel, wifi_scan_iter->phy_mode);
		wifi_scan_iter = wifi_scan_iter->next;
	}
	WMBOX_SIGNAL(WEVT_SCAN_SUCCESS, g_queue);
}

static void _set_softap_config(wifi_manager_softap_config_s *config, char *ssid, char *passphrase, int channel)
{
	strncpy(config->ssid, ssid, strlen(ssid) + 1);
  strncpy(config->passphrase, passphrase, strlen(passphrase) + 1);
	config->channel = channel;
}

static void _set_ap_config(wifi_manager_ap_config_s *config,
                           char *ssid,
                           char *passphrase,
                           wifi_manager_ap_auth_type_e auth,
                           wifi_manager_ap_crypto_type_e crypto)
{
  config->ssid_length = strlen(ssid);
	config->passphrase_length = strlen(passphrase);
	strncpy(config->ssid, ssid, config->ssid_length + 1);
	strncpy(config->passphrase, passphrase,
			config->passphrase_length + 1);
	config->ap_auth_type = auth;
	config->ap_crypto_type = crypto;
}

static void _print_ap_config(wifi_manager_ap_config_s *config)
{
	printf("AP config: %s(%d), %s(%d), %d %d\n",
		   config->ssid, config->ssid_length,
		   config->passphrase, config->passphrase_length,
		   config->ap_auth_type, config->ap_crypto_type);
}

#if 0
static void _print_stats(wifi_manager_stats_s *stats)
{
	printf("=======================================================================\n");
	printf("CONN    CONNFAIL    DISCONN    RECONN    SCAN    SOFTAP    JOIN    LEFT\n");
	printf("%-8d%-12d%-11d%-10d", stats->connect, stats->connectfail, stats->disconnect, stats->reconnect);
	printf("%-8d%-10d%-8d%-8d\n", stats->scan, stats->softap, stats->joined, stats->left);
	printf("=======================================================================\n");
}
#endif

static void _print_softap_config(wifi_manager_softap_config_s *apconfig)
{
	printf("SoftAP config: %s(%d), %s(%d), %d\n", apconfig->ssid,
		   strlen(apconfig->ssid),
		   apconfig->passphrase,
		   strlen(apconfig->passphrase),
		   apconfig->channel);
}

static void utc_wifimanager_init_n(void)
{
	TC_ASSERT_EQ("wifi_manager_init", wifi_manager_init(NULL), WIFI_MANAGER_INVALID_ARGS);
	TC_SUCCESS_RESULT();
}

static void utc_wifimanager_init_p(void)
{
	TC_ASSERT_EQ("wifi_manager_init", wifi_manager_init(&wifi_callbacks), WIFI_MANAGER_SUCCESS);
	TC_ASSERT_EQ("wifi_manager deinit", wifi_manager_deinit(), WIFI_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_wifimanager_set_mode_n(void)
{
	TC_ASSERT_EQ("wifi_manager_init", wifi_manager_init(&wifi_callbacks), WIFI_MANAGER_SUCCESS);
	TC_ASSERT_EQ("wifi_manager_set_mode", wifi_manager_set_mode(SOFTAP_MODE, NULL), WIFI_MANAGER_INVALID_ARGS);
	TC_ASSERT_EQ("wifi_manager_set_mode", wifi_manager_set_mode(WIFI_NONE, &g_softap_config), WIFI_MANAGER_INVALID_ARGS);
	TC_ASSERT_EQ("wifi_manager deinit", wifi_manager_deinit(), WIFI_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_wifimanager_set_mode_p(void)
{
	TC_ASSERT_EQ("wifi_manager_init", wifi_manager_init(&wifi_callbacks), WIFI_MANAGER_SUCCESS);
	TC_ASSERT_EQ("wifi_manager_set_mode", wifi_manager_set_mode(SOFTAP_MODE, &g_softap_config), WIFI_MANAGER_SUCCESS);
	TC_ASSERT_EQ("wifi_manager_set_mode", wifi_manager_set_mode(STA_MODE, NULL), WIFI_MANAGER_SUCCESS);
	TC_ASSERT_EQ("wifi_manager deinit", wifi_manager_deinit(), WIFI_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_wifimanager_get_mode_n(void)
{
	TC_ASSERT_EQ("wifi_manager_init", wifi_manager_init(&wifi_callbacks), WIFI_MANAGER_SUCCESS);
	TC_ASSERT_EQ("wifi_manager_get_info", wifi_manager_get_info(NULL), WIFI_MANAGER_INVALID_ARGS);
	TC_ASSERT_EQ("wifi_manager deinit", wifi_manager_deinit(), WIFI_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_wifimanager_get_mode_p(void)
{
	wifi_manager_info_s info;
	TC_ASSERT_EQ("wifi_manager_init", wifi_manager_init(&wifi_callbacks), WIFI_MANAGER_SUCCESS);
	TC_ASSERT_EQ("wifi_manager_get_info", wifi_manager_get_info(&info), WIFI_MANAGER_SUCCESS);
	TC_ASSERT_EQ("wifi_manager_get_info", info.mode, STA_MODE);
	TC_ASSERT_EQ("wifi_manager_set_mode", wifi_manager_set_mode(SOFTAP_MODE, &g_softap_config), WIFI_MANAGER_SUCCESS);
	TC_ASSERT_EQ("wifi_manager_get_info", wifi_manager_get_info(&info), WIFI_MANAGER_SUCCESS);
	TC_ASSERT_EQ("wifi_manager_get_info", info.mode, SOFTAP_MODE);
	TC_ASSERT_EQ("wifi_manager_get_mode", strncmp(info.ssid, g_softap_config.ssid, strlen(g_softap_config.ssid) + 1), 0);
	TC_ASSERT_EQ("wifi_manager deinit", wifi_manager_deinit(), WIFI_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

/*  description: connect ap in softap mode */
static void utc_wifimanager_connect_ap_n(void)
{
	TC_ASSERT_EQ("wifi_manager_init", wifi_manager_init(&wifi_callbacks), WIFI_MANAGER_SUCCESS);
	TC_ASSERT_EQ("wifi_manager_set_mode", wifi_manager_set_mode(SOFTAP_MODE, &g_softap_config), WIFI_MANAGER_SUCCESS);
	/* current wifi mode is softap, then this try will fail */
	TC_ASSERT_EQ("wifi_manager_connect_ap", wifi_manager_connect_ap(&g_ap_config), WIFI_MANAGER_FAIL);
	TC_ASSERT_EQ("wifi_manager deinit", wifi_manager_deinit(), WIFI_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_wifimanager_connect_ap_p(void)
{
	TC_ASSERT_EQ("wifi_manager_init", wifi_manager_init(&wifi_callbacks), WIFI_MANAGER_SUCCESS);
	TC_ASSERT_EQ("wifi_manager_connect_ap", wifi_manager_connect_ap(&g_ap_config), WIFI_MANAGER_SUCCESS);
	CONTROL_VVDRIVER(VWIFI_CMD_GEN_EVT, LWNL_EVT_STA_CONNECTED, 0, 100);
	int conn_res = WEVT_FAIL;
	WMBOX_WAIT(conn_res, g_queue);
	TC_ASSERT_EQ("wifi_manager_connect_ap cb", conn_res, WEVT_CONNECTED);
	CONTROL_VVDRIVER(VWIFI_CMD_GEN_EVT_FUNC, LWNL_EVT_STA_DISCONNECTED, 3, 0);
	TC_ASSERT_EQ("wifi_manager deinit", wifi_manager_deinit(), WIFI_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

/*  description: disconnect ap in connected state */
static void utc_wifimanager_disconnect_ap_p(void)
{
	int conn_res = WEVT_OK;
  TC_ASSERT_EQ("wifi_manager_init", wifi_manager_init(&wifi_callbacks), WIFI_MANAGER_SUCCESS);
  TC_ASSERT_EQ("wifi_manager_connect_ap", wifi_manager_connect_ap(&g_ap_config), WIFI_MANAGER_SUCCESS);
	CONTROL_VVDRIVER(VWIFI_CMD_GEN_EVT, LWNL_EVT_STA_CONNECTED, 0, 100);
	WMBOX_WAIT(conn_res, g_queue);
  TC_ASSERT_EQ("wifi_manager_connect_ap cb", conn_res, WEVT_CONNECTED);
	TC_ASSERT_EQ("wifi_manager_disconnect_ap", wifi_manager_disconnect_ap(), WIFI_MANAGER_SUCCESS);
  CONTROL_VVDRIVER(VWIFI_CMD_GEN_EVT, LWNL_EVT_STA_DISCONNECTED, 0, 100);
	WMBOX_WAIT(conn_res, g_queue);
	TC_ASSERT_EQ("wifi_manager_disconnect_ap evt", conn_res, WEVT_DISCONNECTED);
  TC_ASSERT_EQ("wifi_manager deinit", wifi_manager_deinit(), WIFI_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

/* second disconnection trial will get failed */
static void utc_wifimanager_disconnect_ap_n(void)
{
	int conn_res = WEVT_OK;
	TC_ASSERT_EQ("wifi_manager_init", wifi_manager_init(&wifi_callbacks), WIFI_MANAGER_SUCCESS);
	TC_ASSERT_EQ("wifi_manager_connect_ap", wifi_manager_connect_ap(&g_ap_config), WIFI_MANAGER_SUCCESS);
	CONTROL_VVDRIVER(VWIFI_CMD_GEN_EVT, LWNL_EVT_STA_CONNECTED, 0, 1000);
	WMBOX_WAIT(conn_res, g_queue);
	TC_ASSERT_EQ("wifi_manager_connect_ap cb", conn_res, WEVT_CONNECTED);
	TC_ASSERT_EQ("wifi_manager_disconnect_ap", wifi_manager_disconnect_ap(), WIFI_MANAGER_SUCCESS);
	CONTROL_VVDRIVER(VWIFI_CMD_GEN_EVT, LWNL_EVT_STA_DISCONNECTED, 0, 1000);
	WMBOX_WAIT(conn_res, g_queue);
	TC_ASSERT_EQ("wifi_manager_disconnect_ap evt", conn_res, WEVT_DISCONNECTED);
  TC_ASSERT_EQ("wifi_manager_disconnect_ap", wifi_manager_disconnect_ap(), WIFI_MANAGER_FAIL);
  TC_ASSERT_EQ("wifi_manager deinit", wifi_manager_deinit(), WIFI_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_wifimanager_deinit_p(void)
{
  TC_ASSERT_EQ("wifi_manager_init", wifi_manager_init(&wifi_callbacks), WIFI_MANAGER_SUCCESS);
	TC_ASSERT_EQ("wifi_manager deinit", wifi_manager_deinit(), WIFI_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

/*  description: deinit wifi in unintialized state */
static void utc_wifimanager_deinit_n(void)
{
	TC_ASSERT_EQ("wifi_manager_deinit", wifi_manager_deinit(), WIFI_MANAGER_FAIL);
	TC_SUCCESS_RESULT();
}

// To Do: it's advanced testcase. wifi manager needs additional code to support this
#if 0
static void utc_wifimanager_scan_ap_n(void)
{
	TC_ASSERT_EQ("wifi_manager_init", wifi_manager_init(&wifi_null_callbacks), WIFI_MANAGER_SUCCESS);
	TC_ASSERT_EQ("wifi_manager_scan_ap", wifi_manager_scan_ap(NULL), WIFI_MANAGER_FAIL);
  TC_ASSERT_EQ("wifi_manager deinit", wifi_manager_deinit(), WIFI_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_wifimanager_scan_specific_ap_n(void)
{
	// deprecated
	TC_SUCCESS_RESULT();
}

static void utc_wifimanager_scan_ap_p(void)
{
	TC_ASSERT_EQ("wifi_manager_init", wifi_manager_init(&wifi_callbacks), WIFI_MANAGER_SUCCESS);
	TC_ASSERT_EQ("wifi_manager_scan_ap", wifi_manager_scan_ap(NULL), WIFI_MANAGER_SUCCESS);
  TC_ASSERT_EQ("wifi_manager deinit", wifi_manager_deinit(), WIFI_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_wifimanager_scan_specific_ap_p(void)
{
	// deprecated
	TC_SUCCESS_RESULT();
}

static void utc_wifimanager_get_stats_n(void)
{
	TC_ASSERT_EQ("wifi_manager_init", wifi_manager_init(&wifi_callbacks), WIFI_MANAGER_SUCCESS);
	TC_ASSERT_EQ("wifi_manager_get_stats_n", wifi_manager_get_stats(NULL), WIFI_MANAGER_INVALID_ARGS);
  TC_ASSERT_EQ("wifi_manager deinit", wifi_manager_deinit(), WIFI_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_wifimanager_get_stats_p(void)
{
	wifi_manager_stats_s stats;
	TC_ASSERT_EQ("wifi_manager_init", wifi_manager_init(&wifi_callbacks), WIFI_MANAGER_SUCCESS);
	TC_ASSERT_EQ("wifi_manager_get_stats_p", wifi_manager_get_stats(&stats), WIFI_MANAGER_SUCCESS);
	_print_stats(&stats);
	TC_ASSERT_EQ("wifi_manager deinit", wifi_manager_deinit(), WIFI_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

/*  register cb when wifi manager is not initialized */
static void utc_wifimanager_register_cb_n(void)
{
	TC_ASSERT_EQ("wifi_manager_register_cb_n", wifi_manager_register_cb(&wifi_dup_callbacks), WIFI_MANAGER_DEINITIALIZED);
	TC_SUCCESS_RESULT();
}

static void utc_wifimanager_register_cb_p(void)
{
	TC_ASSERT_EQ("wifi_manager_init", wifi_manager_init(&wifi_callbacks), WIFI_MANAGER_SUCCESS);
	TC_ASSERT_EQ("wifi_manager_register_cb_p", wifi_manager_register_cb(&wifi_dup_callbacks), WIFI_MANAGER_SUCCESS);
	TC_ASSERT_EQ("wifi_manager deinit", wifi_manager_deinit(), WIFI_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_wifimanager_unregister_cb_n(void)
{
	TC_ASSERT_EQ("wifi_manager_init", wifi_manager_init(&wifi_callbacks), WIFI_MANAGER_SUCCESS);
	TC_ASSERT_EQ("wifi_manager_unregister_cb_n", wifi_manager_unregister_cb(&wifi_dup_callbacks), WIFI_MANAGER_FAIL);
	TC_ASSERT_EQ("wifi_manager deinit", wifi_manager_deinit(), WIFI_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_wifimanager_unregister_cb_p(void)
{
	TC_ASSERT_EQ("wifi_manager_init", wifi_manager_init(&wifi_callbacks), WIFI_MANAGER_SUCCESS);
	TC_ASSERT_EQ("wifi_manager_unregister_cb_p", wifi_manager_register_cb(&wifi_dup_callbacks), WIFI_MANAGER_SUCCESS);
	TC_ASSERT_EQ("wifi_manager_unregister_cb_p", wifi_manager_unregister_cb(&wifi_dup_callbacks), WIFI_MANAGER_SUCCESS);
	TC_ASSERT_EQ("wifi_manager deinit", wifi_manager_deinit(), WIFI_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}
#endif

#ifdef CONFIG_WIFI_MANAGER_SAVE_CONFIG
static void utc_wifimanager_save_config_n(void)
{
	wifi_manager_result_e ret = WIFI_MANAGER_FAIL;
	ret = wifi_manager_save_config(NULL);

	TC_ASSERT_EQ("wifi_manager_save_config", ret, WIFI_MANAGER_INVALID_ARGS);
	TC_SUCCESS_RESULT();
}

static void utc_wifimanager_save_config_p(void)
{
	TC_ASSERT_EQ("wifi_manager_save_config", wifi_manager_save_config(&g_ap_config), WIFI_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_wifimanager_get_config_p(void)
{
	wifi_manager_result_e ret = WIFI_MANAGER_FAIL;
	int res = 0;
	wifi_manager_ap_config_s config;
	ret = wifi_manager_get_config(&config);
	if (ret == WIFI_MANAGER_SUCCESS) {
		_print_ap_config(&config);
		res = strncmp(config.ssid, g_ssid, strlen(config.ssid));
		if (res < 0) {
			ret = WIFI_MANAGER_FAIL;
		}
		res = strncmp(config.passphrase, g_passphrase, strlen(config.passphrase));
		if (res < 0) {
			ret = WIFI_MANAGER_FAIL;
		}
	}
	TC_ASSERT_EQ("wifi_manager_get_config", ret, WIFI_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_wifimanager_get_config_n(void)
{
	wifi_manager_result_e ret = WIFI_MANAGER_FAIL;
	int res = 0;
	wifi_manager_ap_config_s config;
	ret = wifi_manager_get_config(&config);
	if (ret == WIFI_MANAGER_SUCCESS) {
		_print_ap_config(&config);
		res = strncmp(config.ssid, g_ssid, strlen(config.ssid));
		if (res < 0) {
			ret = WIFI_MANAGER_FAIL;
		}
		res = strncmp(config.passphrase, g_passphrase, strlen(config.passphrase));
		if (res < 0) {
			ret = WIFI_MANAGER_FAIL;
		}
	}
	TC_ASSERT_EQ("wifi_manager_get_config", ret, WIFI_MANAGER_FAIL);
	TC_SUCCESS_RESULT();
}

static void utc_wifimanager_remove_config_n(void)
{
	wifi_manager_result_e ret = WIFI_MANAGER_FAIL;
	ret = wifi_manager_remove_config();
	TC_ASSERT_EQ("wifi_manager_remove_config", ret, WIFI_MANAGER_FAIL);
	TC_SUCCESS_RESULT();
}

static void utc_wifimanager_remove_config_p(void)
{
	wifi_manager_result_e ret = WIFI_MANAGER_FAIL;
	ret = wifi_manager_remove_config();
	TC_ASSERT_EQ("wifi_manager_remove_config", ret, WIFI_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}

static void utc_wifimanager_get_connected_config_n(void)
{
	wifi_manager_result_e ret = WIFI_MANAGER_FAIL;
	wifi_manager_ap_config_s apconfig;

	unlink(WIFI_PROFILE_PATH WIFI_PROFILE_FILENAME_INTERNAL);

	ret = wifi_manager_get_connected_config(&apconfig);
	if (ret == WIFI_MANAGER_SUCCESS) {
		_print_ap_config(&apconfig);
	}
	TC_ASSERT_EQ("wifi_manager_get_connected_config_n", ret, WIFI_MANAGER_FAIL);
	TC_SUCCESS_RESULT();
}

static void utc_wifimanager_get_connected_config_p(void)
{
	wifi_manager_result_e ret = WIFI_MANAGER_FAIL;
	wifi_manager_ap_config_s apconfig;
	ret = wifi_manager_get_connected_config(&apconfig);
	if (ret == WIFI_MANAGER_SUCCESS) {
		_print_ap_config(&apconfig);
	}
	TC_ASSERT_EQ("wifi_manager_get_connected_config_p", ret, WIFI_MANAGER_SUCCESS);
	TC_SUCCESS_RESULT();
}
#endif //CONFIG_WIFI_MANAGER_SAVE_CONFIG

int wifi_manager_utc(int argc, FAR char *argv[])
{
	if (testcase_state_handler(TC_START, "WiFiManager UTC") == ERROR) {
		return ERROR;
	}

	utc_wifimanager_init_n();
	utc_wifimanager_init_p();

  utc_wifimanager_set_mode_n();
	utc_wifimanager_set_mode_p();

	utc_wifimanager_get_mode_n();
	utc_wifimanager_get_mode_p(); // set softap mode inside this function

	/* shoud define ap config first in utc_wifimanager_connect_ap_n()
	 * and utc_wifimanager_connect_ap_p() before trying these two tests
	 * Otherwise, you will get failed.
	 */
	utc_wifimanager_connect_ap_n(); // try to connect to ap in softap mode
	utc_wifimanager_connect_ap_p(); // change to station mode and try to connect to ap

	utc_wifimanager_disconnect_ap_p();
	utc_wifimanager_disconnect_ap_n(); //  Should be run after positive tc, that is, the second disconnect gets failed.

	utc_wifimanager_deinit_p();
	utc_wifimanager_deinit_n(); // Should be run after positive tc, that is, the second deinit gets failed.

#if 0 // todo wifi manager need additional code to support this
	utc_wifimanager_scan_ap_n(); // Get failed becasue there is no callback hander for scan results
	utc_wifimanager_scan_specific_ap_n();
	utc_wifimanager_scan_ap_p(); // Reinitialized wifi manager with the callback hander for scan results
	utc_wifimanager_scan_specific_ap_p();

	utc_wifimanager_get_stats_n();
	utc_wifimanager_get_stats_p();

  // register api is not managed from TizenRT 3.1
	utc_wifimanager_register_cb_n(); //not initialized yet
	utc_wifimanager_register_cb_p();
	utc_wifimanager_unregister_cb_n(); // nothing to unregister
	utc_wifimanager_unregister_cb_p();
#endif

#ifdef CONFIG_WIFI_MANAGER_SAVE_CONFIG
	utc_wifimanager_get_connected_config_n();
	utc_wifimanager_get_connected_config_p();
	utc_wifimanager_save_config_n();
	utc_wifimanager_get_config_n();
	utc_wifimanager_remove_config_n();
	utc_wifimanager_save_config_p(); // save correct wifi config
	utc_wifimanager_get_config_p();
	utc_wifimanager_remove_config_p();
#endif

	(void)testcase_state_handler(TC_END, "WiFiManager UTC");

	return 0;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int utc_wifimanager_main(int argc, char *argv[])
#endif
{
	g_queue = wifi_mbox_init();
	_set_ap_config(&g_ap_config, CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_UTC_SSID,
				   CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_UTC_PASSPHRASE,
				   CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_UTC_AUTHENTICATION,
				   CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_UTC_CRYPTO);
  _print_ap_config(&g_ap_config);

  _set_softap_config(&g_softap_config, CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_UTC_SOFTAP_SSID,
					   CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_UTC_SOFTAP_PASSPHRASE,
					   CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_UTC_SOFTAP_CHANNEL);
  _print_softap_config(&g_softap_config);

	int ssid_len = strlen(CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_UTC_SSID);
	int passphrase_len = strlen(CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_UTC_PASSPHRASE);
	int softap_ssid_len = strlen(CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_UTC_SOFTAP_SSID);
	int softap_passphrase_len = strlen(CONFIG_EXAMPLES_TESTCASE_WIFI_MANAGER_UTC_SOFTAP_PASSPHRASE);
	if ((ssid_len > 31) || (softap_ssid_len > 31) || (passphrase_len > 63) || (softap_passphrase_len > 63)) {
		printf("AP or SoftAP configuration fails: too long ssid or passphrase\n");
		printf("Make sure that length of SSID < 32 and length of passphrase < 64\n");
		return -1;
	}

	wifi_manager_utc(argc, argv);

	wifi_mbox_deinit(g_queue);

	return 0;
}
