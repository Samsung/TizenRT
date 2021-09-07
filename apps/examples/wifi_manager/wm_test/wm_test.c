/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License\n");
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
/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <errno.h>
#include <wifi_manager/wifi_manager.h>
#include "wm_test.h"
#include "wm_test_usage.h"
#include "wm_test_log.h"

#define WM_TEST_COUNT 10
#define TAG "[WT]"

typedef int (*parser_func)(struct wt_options *opt, int argc, char *argv[]);

#define WT_TEST_SIGNAL              \
	do {                            \
		sem_post(&g_wm_sem);        \
		WT_LOG(TAG, "send signal"); \
	} while (0)

#define WT_TEST_WAIT                \
	do {                            \
		WT_LOG(TAG, "wait signal"); \
		sem_wait(&g_wm_sem);        \
	} while (0)

#define WT_TEST_FUNC_SIGNAL              \
	do {                                 \
		sem_post(&g_wm_func_sem);        \
		WT_LOG(TAG, "send func signal"); \
	} while (0)

#define WT_TEST_FUNC_WAIT                \
	do {                                 \
		WT_LOG(TAG, "wait func signal"); \
		sem_wait(&g_wm_func_sem);        \
	} while (0)
#define WT_STRESS_MAX_IDX 4
#define WT_STRESS_MIN_IDX 1

typedef enum {
#ifdef WT_MEMBER_POOL
#undef WT_MEMBER_POOL
#endif
#define WT_MEMBER_POOL(type, func, parser, str) type,
#include "wm_test_table.h"
	WT_TYPE_MAX,
	WT_TYPE_ERR = -1
} wt_type_e;

/* Handler */
#ifdef WT_MEMBER_POOL
#undef WT_MEMBER_POOL
#endif
#define WT_MEMBER_POOL(type, func, parser, str) static void func(void *arg);
#include "wm_test_table.h"
extern void wm_run_stress_test(void *arg);
extern void wm_test_on_off(void *arg);

/* Parser */
#ifdef WT_MEMBER_POOL
#undef WT_MEMBER_POOL
#endif
#define WT_MEMBER_POOL(type, func, parser, str) \
	static int parser(struct wt_options *opt, int argc, char *argv[]);
#include "wm_test_table.h"
//static wifi_manager_ap_auth_type_e wt_get_auth_type(const char *method);
//static wifi_manager_ap_crypto_type_e wt_get_crypto_type(const char *method);
static int _wt_parse_commands(struct wt_options *opt, int argc, char *argv[]);

/* Signal */
static void _wt_signal_deinit(void);
static int _wt_signal_init(void);

/* Callbacks */
static void _wt_sta_connected(wifi_manager_result_e);
static void _wt_sta_disconnected(wifi_manager_disconnect_e);
static void _wt_softap_sta_join(void);
static void _wt_softap_sta_leave(void);
static void _wt_scan_done(wifi_manager_scan_info_s **scan_result, wifi_manager_scan_result_e res);

/* Main */
static wt_type_e _wt_get_opt(int argc, char *argv[]);
static void _wt_process(int argc, char *argv[]);

/* Global */
test_func g_func_table[] = {
#ifdef WT_MEMBER_POOL
#undef WT_MEMBER_POOL
#endif
#define WT_MEMBER_POOL(type, func, parser, str) func,
#include "wm_test_table.h"
};

parser_func g_parser_table[] = {
#ifdef WT_MEMBER_POOL
#undef WT_MEMBER_POOL
#endif
#define WT_MEMBER_POOL(type, func, parser, str) parser,
#include "wm_test_table.h"
};

char *g_func_name[] = {
#ifdef WT_MEMBER_POOL
#undef WT_MEMBER_POOL
#endif
#define WT_MEMBER_POOL(type, func, parser, str) str,
#include "wm_test_table.h"
};

static wifi_manager_cb_s g_wifi_callbacks = {
	_wt_sta_connected,
	_wt_sta_disconnected,
	_wt_softap_sta_join,
	_wt_softap_sta_leave,
	_wt_scan_done,
};

static sem_t g_wm_sem = SEM_INITIALIZER(0);
static sem_t g_wm_func_sem = SEM_INITIALIZER(0);
static int g_mode = 0; // check program is running

// if _wt_scan_done is for WT_TYPE_SJOIN then it doesn't need to print
// scan list. So if g_scan_join is 1 then it doesn't print scan list
static int g_scan_join = 0;
static int g_scanned_result = 0;
static char g_scanned_ssid[WIFIMGR_SSID_LEN + 1] = {
	0,
};
static wifi_manager_ap_auth_type_e g_scanned_auth_type = WIFI_MANAGER_AUTH_OPEN;
static wifi_manager_ap_crypto_type_e g_scanned_crypto_type = WIFI_MANAGER_CRYPTO_NONE;


int _wt_get_scanned_list(wifi_manager_scan_info_s *slist, char *ssid,
						 wifi_manager_ap_auth_type_e *atype,
						 wifi_manager_ap_crypto_type_e *ctype)
{
	int ssid_len = strlen(ssid);
	while (slist) {
		if (strncmp(ssid, slist->ssid, ssid_len + 1) == 0) {
			*atype = slist->ap_auth_type;
			*ctype = slist->ap_crypto_type;
			return 0;
		}
		slist = slist->next;
	}
	return -1;
}

int _wt_signal_init(void)
{
	if (g_mode != 0) {
		WT_LOGE(TAG, "Program is already running");
		return -1;
	}
	g_mode = 1;
	return 0;
}

void _wt_signal_deinit(void)
{
	g_mode = 0;
}

/*  callback */
void _wt_sta_connected(wifi_manager_result_e res)
{
	WT_LOG(TAG, "-->res(%d)", res);
	WT_TEST_SIGNAL;
}

void _wt_sta_disconnected(wifi_manager_disconnect_e disconn)
{
	WT_LOG(TAG, "-->");
	WT_TEST_SIGNAL;
}

void _wt_softap_sta_join(void)
{
	WT_LOG(TAG, "-->");
	WT_TEST_SIGNAL;
}

void _wt_softap_sta_leave(void)
{
	WT_LOG(TAG, "-->");
	WT_TEST_SIGNAL;
}

void _wt_scan_done(wifi_manager_scan_info_s **scan_result, wifi_manager_scan_result_e res)
{
	WT_LOG(TAG, "-->");
	/* Make sure you copy the scan results onto a local data structure.
	 * It will be deleted soon eventually as you exit this function.
	 */
	if (scan_result == NULL) {
		WT_TEST_SIGNAL;
		return;
	}
	if (g_scan_join == 0) {
		wt_print_scanlist(*scan_result);
	} else {
		// request type is WT_TYPE_SJOIN. so it doesn't print scan list
		// and pass scan list result to _wt_scan_connect;
		g_scanned_result = _wt_get_scanned_list(*scan_result,
												g_scanned_ssid,
												&g_scanned_auth_type,
												&g_scanned_crypto_type);
	}
	WT_TEST_SIGNAL;
}

/* Control Functions */
void _wt_start(void *arg)
{
	WT_ENTER;
	wifi_manager_result_e res = WIFI_MANAGER_SUCCESS;

	res = wifi_manager_init(&g_wifi_callbacks);
	if (res != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "wifi_manager_init fail");
	}
	WT_LEAVE;
}

void _wt_stop(void *arg)
{
	WT_ENTER;
	wifi_manager_result_e res = wifi_manager_deinit();
	if (res != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "WiFi Manager failed to stop");
	}
	WT_LEAVE;
}

void _wt_softap_start(void *arg)
{
	WT_ENTER;
	wifi_manager_result_e res = WIFI_MANAGER_SUCCESS;
	struct wt_options *ap_info = (struct wt_options *)arg;
	if (strlen(ap_info->ssid) > WIFIMGR_SSID_LEN || strlen(ap_info->password) > WIFIMGR_PASSPHRASE_LEN) {
		WT_LOGE(TAG, "Param Error");
		WT_LEAVE;
		return;
	}
	wifi_manager_softap_config_s ap_config;
	strncpy(ap_config.ssid, ap_info->ssid, WIFIMGR_SSID_LEN - 1);
	ap_config.ssid[WIFIMGR_SSID_LEN - 1] = '\0';
	strncpy(ap_config.passphrase, ap_info->password, WIFIMGR_PASSPHRASE_LEN - 1);
	ap_config.passphrase[WIFIMGR_PASSPHRASE_LEN - 1] = '\0';
	ap_config.channel = 1;

	wt_print_wifi_softap_profile(&ap_config, "AP INFO");

	res = wifi_manager_set_mode(SOFTAP_MODE, &ap_config);
	if (res != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "Run SoftAP Fail");
	}
	WT_LEAVE;
}

void _wt_sta_start(void *arg)
{
	WT_ENTER;
	wifi_manager_result_e res = WIFI_MANAGER_SUCCESS;

	res = wifi_manager_set_mode(STA_MODE, NULL);
	if (res != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "Set STA mode Fail");
		return;
	}
	WT_LOG(TAG, "Start STA mode");
	WT_LEAVE;
}

void _wt_connect(void *arg)
{
	WT_ENTER;
	struct wt_options *ap_info = (struct wt_options *)arg;
	wifi_manager_ap_config_s apconfig;
	strncpy(apconfig.ssid, ap_info->ssid, WIFIMGR_SSID_LEN);
	apconfig.ssid_length = strlen(ap_info->ssid);
	apconfig.ssid[WIFIMGR_SSID_LEN] = '\0';
	apconfig.ap_auth_type = ap_info->auth_type;
	if (ap_info->auth_type != WIFI_MANAGER_AUTH_OPEN) {
		strncpy(apconfig.passphrase, ap_info->password, WIFIMGR_PASSPHRASE_LEN);
		apconfig.passphrase[WIFIMGR_PASSPHRASE_LEN] = '\0';
		apconfig.passphrase_length = strlen(ap_info->password);
		apconfig.ap_crypto_type = ap_info->crypto_type;
	} else {
		apconfig.passphrase[0] = '\0';
		apconfig.passphrase_length = 0;
		apconfig.ap_crypto_type = ap_info->crypto_type;
	}

	wt_print_wifi_ap_profile(&apconfig, "Connecting AP Info");

	wifi_manager_result_e res = wifi_manager_connect_ap(&apconfig);
	if (res != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "AP connect failed");
		return;
	}
	/* Wait for DHCP connection */
	WT_LOG(TAG, "wait join done");
	WT_TEST_WAIT;
	WT_LEAVE;
}

void _wt_scan_connect(void *arg)
{
	WT_ENTER;
	WT_LOG(TAG, "--> scan_connect\n");
	g_scan_join = 1;
	struct wt_options *ap_info = (struct wt_options *)arg;
	wifi_manager_scan_config_s sconfig;
	sconfig.channel = 0;
	sconfig.ssid_length = strlen(ap_info->ssid);
	strncpy(sconfig.ssid, ap_info->ssid, sconfig.ssid_length + 1);
	strncpy(g_scanned_ssid, ap_info->ssid, sconfig.ssid_length + 1);

	wifi_manager_result_e res = wifi_manager_scan_ap(&sconfig);

	if (res != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "request scan ssid (%s) fail %d\n", sconfig.ssid, res);
		return;
	}
	WT_TEST_WAIT;

	g_scan_join = 0;
	if (g_scanned_result != 0) {
		WT_LOGE(TAG, "scan ssid fail\n");
		return;
	}

	wifi_manager_ap_config_s apconfig;
	memset(&apconfig, 0, sizeof(wifi_manager_ap_config_s));
	strncpy(apconfig.ssid, ap_info->ssid, WIFIMGR_SSID_LEN);
	apconfig.ssid_length = strlen(ap_info->ssid);
	apconfig.ap_auth_type = g_scanned_auth_type;
	if (ap_info->auth_type != WIFI_MANAGER_AUTH_OPEN) {
		strncpy(apconfig.passphrase, ap_info->password, WIFIMGR_PASSPHRASE_LEN);
		apconfig.passphrase_length = strlen(ap_info->password);
		apconfig.ap_crypto_type = g_scanned_crypto_type;
	} else {
		apconfig.passphrase[0] = '\0';
		apconfig.passphrase_length = 0;
		apconfig.ap_crypto_type = ap_info->crypto_type;
	}

	wt_print_wifi_ap_profile(&apconfig, "Scan connect AP Info");

	res = wifi_manager_connect_ap(&apconfig);
	if (res != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "AP connect failed %d\n", res);
		return;
	}
	/* Wait for DHCP connection */
	WT_TEST_WAIT;
	WT_LOG(TAG, "<-- scan_connect\n");
	WT_LEAVE;
}

void _wt_disconnect(void *arg)
{
	WT_ENTER;
	wifi_manager_result_e res = WIFI_MANAGER_SUCCESS;
	/* Disconnect AP */
	res = wifi_manager_disconnect_ap();
	if (res != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "disconnect fail (%d)", res);
		return;
	}
	WT_TEST_WAIT;
	WT_LEAVE;
	;
}

void _wt_cancel(void *arg)
{
	WT_ENTER;
	wifi_manager_result_e res = WIFI_MANAGER_SUCCESS;
	/* Disconnect AP */
	res = wifi_manager_disconnect_ap();
	if (res != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "disconnect fail (%d)", res);
		return;
	}
	WT_LEAVE;
}

void _wt_set_info(void *arg)
{
	WT_ENTER;
	struct wt_options *ap_info = (struct wt_options *)arg;
	wifi_manager_ap_config_s apconfig;
	strncpy(apconfig.ssid, ap_info->ssid, WIFIMGR_SSID_LEN);
	apconfig.ssid_length = strlen(ap_info->ssid);
	apconfig.ssid[WIFIMGR_SSID_LEN] = '\0';
	apconfig.ap_auth_type = ap_info->auth_type;
	if (apconfig.ap_auth_type != WIFI_MANAGER_AUTH_OPEN) {
		strncpy(apconfig.passphrase, ap_info->password, WIFIMGR_PASSPHRASE_LEN);
		apconfig.passphrase[WIFIMGR_PASSPHRASE_LEN] = '\0';
		apconfig.passphrase_length = strlen(ap_info->password);

	} else {
		strcpy(apconfig.passphrase, "");
		apconfig.passphrase_length = 0;
	}

	apconfig.ap_crypto_type = ap_info->crypto_type;

	wt_print_wifi_ap_profile(&apconfig, "Set AP Info");

	wifi_manager_result_e res = wifi_manager_save_config(&apconfig);
	if (res != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "Save AP configuration failed");
		return;
	}
	WT_LEAVE;
}

void _wt_get_info(void *arg)
{
	WT_ENTER;
	wifi_manager_ap_config_s apconfig;
	wifi_manager_result_e res = wifi_manager_get_config(&apconfig);
	if (res != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "Get AP configuration failed");
		return;
	}
	wt_print_wifi_ap_profile(&apconfig, "Stored Wi-Fi Infomation");
	WT_LEAVE;
}

void _wt_reset_info(void *arg)
{
	WT_ENTER;
	wifi_manager_result_e res = wifi_manager_remove_config();
	if (res != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "Get AP configuration failed");
		return;
	}
	WT_LEAVE;
}

void _wt_scan(void *arg)
{
	WT_ENTER;
	wifi_manager_result_e res = WIFI_MANAGER_SUCCESS;

	struct wt_options *ap_info = (struct wt_options *)arg;
	if (ap_info->scan_specific) {
		wifi_manager_scan_config_s config;
		memset(&config, 0, sizeof(config));
		if (ap_info->ssid) {
			config.ssid_length = strlen(ap_info->ssid);
			strncpy(config.ssid, ap_info->ssid, config.ssid_length + 1);
		}
		if (ap_info->channel > 0) {
			config.channel = ap_info->channel;
		}
		res = wifi_manager_scan_ap(&config);
	} else {
		res = wifi_manager_scan_ap(NULL);
	}
	if (res != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "scan Fail");
		return;
	}
	WT_TEST_WAIT; // wait the scan result
	WT_LEAVE;
}

void _wt_get_stats(void *arg)
{
	WT_ENTER;
	wifi_manager_stats_s stats;
	wifi_manager_result_e res = wifi_manager_get_stats(&stats);
	if (res != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "Get WiFi Manager stats failed");
	} else {
		wt_print_stats(&stats);
	}
	WT_LEAVE;
}

void _wt_get_conn_info(void *arg)
{
	WT_ENTER;
	wifi_manager_info_s info;
	wifi_manager_result_e res = wifi_manager_get_info(&info);
	if (res != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "fail to get connection info(%d)", res);
		return;
	}
	wt_print_conninfo(&info);

	WT_LEAVE;
}

void _wt_set_power(void *arg)
{
	WT_ENTER;
	struct wt_options *opt = (struct wt_options *)arg;
	wifi_manager_powermode_e pmode = WIFI_MANAGER_POWERMODE_ENABLE;
	if (opt->power_mode == 0) {
		pmode = WIFI_MANAGER_POWERMODE_DISABLE;
	}
	wifi_manager_result_e res = wifi_manager_set_powermode(pmode);
	if (res != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "fail to set power mode\n");
		return;
	}
	WT_LOG(TAG, "set power mode %d\n", pmode);
	WT_LEAVE;
}

void _wt_stress_test(void *arg)
{
	wm_run_stress_test(arg);
}

void _wt_onoff_test(void *arg)
{
	wm_test_on_off(arg);
}

wt_type_e _wt_get_opt(int argc, char *argv[])
{
	int idx = 0;
	if (argc < 3) {
		return WT_TYPE_ERR;
	}

	for (idx = 0; idx < WT_TYPE_MAX; idx++) {
		if (strcmp(argv[2], g_func_name[idx]) == 0) {
			return (wt_type_e)idx;
		}
	}
	return WT_TYPE_ERR;
}

int _wt_parse_none(struct wt_options *opt, int argc, char *argv[])
{
	if (argc != 3) {
		return -1;
	}
	return 0;
}

int _wt_parse_softap(struct wt_options *opt, int argc, char *argv[])
{
	if (argc < 5) {
		return -1;
	}
	/* wpa2 aes is a default security mode. */
	opt->ssid = argv[3];
	opt->password = argv[4];
	return 0;
}

int _wt_parse_join(struct wt_options *opt, int argc, char *argv[])
{
	if (argc < 5) {
		return -1;
	}
	opt->ssid = argv[3];
	opt->auth_type = wt_get_auth_type(argv[4]);
	if (opt->auth_type == WIFI_MANAGER_AUTH_OPEN || opt->auth_type == WIFI_MANAGER_AUTH_IBSS_OPEN) {
		// case: open mode
		opt->password = "";
		opt->crypto_type = WIFI_MANAGER_CRYPTO_NONE;
		return 0;
	}

	if (argc == 5) {
		// case: unspecified security mode
		opt->auth_type = WIFI_MANAGER_AUTH_UNKNOWN;
		opt->crypto_type = WIFI_MANAGER_CRYPTO_UNKNOWN;
		opt->password = argv[4];
		return 0;
	}

	// case: security mode + password
	if (opt->auth_type == WIFI_MANAGER_AUTH_UNKNOWN) {
		return -1;
	}

	if (opt->auth_type == WIFI_MANAGER_AUTH_WEP_SHARED) {
		if ((strlen(argv[5]) == 13) || (strlen(argv[5]) == 26)) {
			opt->crypto_type = WIFI_MANAGER_CRYPTO_WEP_128;
		} else if ((strlen(argv[5]) == 5) || (strlen(argv[5]) == 10)) {
			opt->crypto_type = WIFI_MANAGER_CRYPTO_WEP_64;
		} else {
			return -1;
		}
	} else {
		opt->crypto_type = wt_get_crypto_type(argv[4]);
		if (opt->crypto_type == WIFI_MANAGER_CRYPTO_UNKNOWN) {
			return -1;
		}
	}
	opt->password = argv[5];
	return 0;
}

int _wt_parse_stress(struct wt_options *opt, int argc, char *argv[])
{
	if (argc < 4) {
		return -1;
	}

	opt->stress_tc_idx = atoi(argv[3]);
	if (opt->stress_tc_idx > WT_STRESS_MAX_IDX || opt->stress_tc_idx < WT_STRESS_MIN_IDX) {
		return -2;
	}
	if (opt->stress_tc_idx == 1) {
		// TC index is 1
		if (argc != 7 && argc != 6) {
			return -1;
		}
	} else {
		if (argc != 10 && argc != 9) {
			return -1;
		}
	}

	opt->ssid = argv[4];
	opt->auth_type = wt_get_auth_type(argv[5]);
	if (opt->auth_type == WIFI_MANAGER_AUTH_OPEN || opt->auth_type == WIFI_MANAGER_AUTH_IBSS_OPEN) {
		// case: open mode
		opt->password = "";
		opt->crypto_type = WIFI_MANAGER_CRYPTO_NONE;
		return 0;
	}

	if (argc == 6) {
		// case: unspecified security mode
		opt->auth_type = WIFI_MANAGER_AUTH_UNKNOWN;
		opt->crypto_type = WIFI_MANAGER_CRYPTO_UNKNOWN;
		opt->password = argv[5];
		return 0;
	}

	// case: security mode + password
	if (opt->auth_type == WIFI_MANAGER_AUTH_UNKNOWN) {
		return -1;
	}

	if (opt->auth_type == WIFI_MANAGER_AUTH_WEP_SHARED) {
		if (strlen(argv[6]) == 13) {
			opt->crypto_type = WIFI_MANAGER_CRYPTO_WEP_128;
		} else if (strlen(argv[6]) == 5) {
			opt->crypto_type = WIFI_MANAGER_CRYPTO_WEP_64;
		} else {
			return -1;
		}
	} else {
		opt->crypto_type = wt_get_crypto_type(argv[5]);
		if (opt->crypto_type == WIFI_MANAGER_CRYPTO_UNKNOWN) {
			return -1;
		}
	}
	opt->password = argv[6];

	int softap_index = 7;
	if (opt->auth_type == WIFI_MANAGER_AUTH_OPEN) {
		softap_index = 6;
	}
	if (opt->stress_tc_idx != 1) {
		/* wpa2 aes is a default security mode. */
		opt->softap_ssid = argv[softap_index++];
		opt->softap_password = argv[softap_index++];
		opt->softap_channel = atoi(argv[softap_index]);
	}

	return 0;
}

int _wt_parse_set(struct wt_options *opt, int argc, char *argv[])
{
	if (argc < 5) {
		return -1;
	}
	opt->ssid = argv[3];
	opt->auth_type = wt_get_auth_type(argv[4]);
	if (opt->auth_type == WIFI_MANAGER_AUTH_UNKNOWN) {
		return -1;
	}
	if (opt->auth_type == WIFI_MANAGER_AUTH_OPEN || opt->auth_type == WIFI_MANAGER_AUTH_IBSS_OPEN) {
		opt->crypto_type = WIFI_MANAGER_CRYPTO_NONE;
		opt->password = "";
		return 0;
	}

	if (opt->auth_type == WIFI_MANAGER_AUTH_WEP_SHARED) {
		if ((strlen(argv[5]) == 13) || (strlen(argv[5]) == 26)) {
			opt->crypto_type = WIFI_MANAGER_CRYPTO_WEP_128;
		} else if ((strlen(argv[5]) == 5) || (strlen(argv[5]) == 10)) {
			opt->crypto_type = WIFI_MANAGER_CRYPTO_WEP_64;
		} else {
			return -1;
		}
	} else {
		opt->crypto_type = wt_get_crypto_type(argv[4]);
		if (opt->crypto_type == WIFI_MANAGER_CRYPTO_UNKNOWN) {
			return -1;
		}
	}
	opt->password = argv[5];
	return 0;
}

int _wt_parse_scan(struct wt_options *opt, int argc, char *argv[])
{
	if (argc == 5) {
		opt->scan_specific = 1;
		if (strncmp("ssid", argv[3], strlen("ssid") + 1) == 0) {
			if (strlen(argv[4]) > WIFIMGR_SSID_LEN) {
				WT_LOGE(TAG, "ssid length is too long\n", strlen(argv[4]));
				return -1;
			}
			opt->ssid = argv[4];
		} else if (strncmp("ch", argv[3], strlen("ch") + 1) == 0) {
			opt->channel = atoi(argv[4]);
		} else {
			return -1;
		}
		return 0;
	} else if (argc == 6) {
		opt->scan_specific = 1;
		if (strncmp("both", argv[3], strlen("both") + 1) == 0) {
			opt->ssid = argv[4];
			opt->channel = atoi(argv[5]);
			return 0;
		} else {
			return -1;
		}
	} else if (argc == 3) {
		// full scan
		opt->scan_specific = 0;
		return 0;
	}
	return -1;
}

int _wt_parse_auto(struct wt_options *opt, int argc, char *argv[])
{
	if (argc < 7) {
		return -1;
	}
	opt->softap_ssid = argv[3];
	opt->softap_password = argv[4];
	opt->ssid = argv[5];
	opt->auth_type = wt_get_auth_type(argv[6]);
	if (opt->auth_type == WIFI_MANAGER_AUTH_UNKNOWN) {
		return -1;
	}
	if (opt->auth_type == WIFI_MANAGER_AUTH_OPEN) {
		return 0;
	}
	opt->crypto_type = wt_get_crypto_type(argv[6]);
	opt->password = argv[7];
	return 0;
}

int _wt_parse_power(struct wt_options *opt, int argc, char *argv[])
{
	if (argc != 4) {
		return -1;
	} else if (!strncmp(argv[3], "on", 3)) {
		opt->power_mode = 1;
		return 0;
	} else if (!strncmp(argv[3], "off", 4)) {
		opt->power_mode = 0;
		return 0;
	}
	return -1;
}

int _wt_parse_commands(struct wt_options *opt, int argc, char *argv[])
{
	int ret = 0;
	wt_type_e options = _wt_get_opt(argc, argv);
	if (options == WT_TYPE_ERR) {
		return -1;
	}

	opt->func = g_func_table[options];
	if (opt->func == NULL) {
		return -1;
	}

	if (g_parser_table[options]) {
		ret = (g_parser_table[options])(opt, argc, argv);
	}

	return ret;
}

void _wt_process(int argc, char *argv[])
{
	struct wt_options opt;
	memset(&opt, 0, sizeof(struct wt_options));
	int res = _wt_parse_commands(&opt, argc, argv);
	if (res < 0) {
		WT_LOGE(TAG, "%s", WT_USAGE);
		goto exit;
	}
	opt.func((void *)&opt);
exit:
	WT_TEST_FUNC_SIGNAL;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int wm_test_main(int argc, char *argv[])
#endif
{
	WT_LOG(TAG, "wifi manager test!!");
	int res = _wt_signal_init();
	if (res < 0) {
		return -1;
	}
	task_create("wifi test sample", 100, 1024 * 10, (main_t)_wt_process, argv);

	WT_TEST_FUNC_WAIT;

	_wt_signal_deinit();

	return 0;
}
