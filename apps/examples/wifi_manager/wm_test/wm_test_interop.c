/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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

#include <tinyara/config.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <errno.h>
#include <wifi_manager/wifi_manager.h>
#include "wm_test.h"
#include "wm_test_log.h"
#include "wm_test_utils.h"

static sem_t g_wio_sem;

#define TAG "[WTIO]"

#define DELIMITER ","
#define DELI_LEN 1
#define WIFI_INTEROP_BUFSIZE 128
#define WIFI_INTEROP_PATH "/mnt/"
#define WIFI_INTEROP_FILENAME "wm_aplist.conf"
#define WIFI_INTEROP_RESULT "wm_result.conf"

#define WIO_TEST_SIGNAL								\
	do {											\
		sem_post(&g_wio_sem);			\
		WT_LOG(TAG, "send signal");	\
	} while (0)

#define WIO_TEST_WAIT								\
	do {											\
		WT_LOG(TAG, "wait signal");	\
		sem_wait(&g_wio_sem);				\
	} while (0)



typedef enum {
	SSID,
	SECURITY,
	PASSPHRASE
} wio_config_e;

static int g_ap_found_status;
static int g_current_ap_idx;
static int g_connect_status;

static interop_ap_config_list_s g_apconfig_list;


static void _print_wifi_ap_profile(wifi_manager_ap_config_s *config)
{
	if (config == NULL)
		return;

	WT_LOG(TAG, "========================================================");
	WT_LOG(TAG, "SSID[%s] passphrase[%s] auth type:[%d] crypto type: [%d]",
		config->ssid, config->passphrase, config->ap_auth_type, config->ap_crypto_type);
	WT_LOG(TAG, "========================================================");
}

static void _wm_sta_connected(wifi_manager_cb_msg_s msg, void *arg)
{
	WT_LOG(TAG, "connect res(%d)", msg.res);

	if (msg.res == WIFI_MANAGER_SUCCESS) {
		g_connect_status = 1;
	} else {
		g_connect_status = 0;
	}

	WIO_TEST_SIGNAL;
}

static void _wm_sta_disconnected(wifi_manager_cb_msg_s msg, void *arg)
{
	WT_LOG(TAG, "disconnect res(%d)", msg.res);
	g_connect_status = 0;
	WIO_TEST_SIGNAL;
}

static void _wm_scan_done(wifi_manager_cb_msg_s msg, void *arg)
{
	/* Make sure you copy the scan results onto a local data structure.
	 * It will be deleted soon eventually as you exit this function.
	 */
	if (msg.scanlist == NULL || msg.res != WIFI_MANAGER_SUCCESS) {
		WIO_TEST_SIGNAL;
		return;
	}

	wifi_manager_scan_info_s *wifi_scan_iter = msg.scanlist;
	while (wifi_scan_iter != NULL) {
		WT_LOG(TAG, "WiFi AP SSID: %-25s, BSSID: %-20s, Rssi: %d, Auth: %d, Crypto: %d",
				wifi_scan_iter->ssid, wifi_scan_iter->bssid, wifi_scan_iter->rssi,
				wifi_scan_iter->ap_auth_type, wifi_scan_iter->ap_crypto_type);

		interop_ap_info_s *ap_info = (g_apconfig_list.ap_info + g_current_ap_idx);

		//check AP
		if (strncmp(wifi_scan_iter->ssid, ap_info->ap_config.ssid, WIFIMGR_SSID_LEN) == 0) {
			WT_LOG(TAG, "WiFi AP SSID : [%-25s] found", wifi_scan_iter->ssid);
			g_ap_found_status = 1;
			break;
		}

		wifi_scan_iter = wifi_scan_iter->next;
	}

	WIO_TEST_SIGNAL;
}

static wifi_manager_cb_s g_wifi_callbacks = {
	_wm_sta_connected,
	_wm_sta_disconnected,
	NULL, NULL,
	_wm_scan_done,
};

static void _wm_interop_disconnect(void)
{
	WT_ENTER;

	wifi_manager_result_e res = WIFI_MANAGER_SUCCESS;

	/* Disconnect AP */
	res = wifi_manager_disconnect_ap();
	if (res != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "disconnect fail (%d)", res);
		return;
	}
	WIO_TEST_WAIT;

	WT_LEAVE;
}

static void _wm_interop_connect(wifi_manager_ap_config_s *apconfig)
{
	WT_ENTER;
	wifi_manager_result_e res = WIFI_MANAGER_SUCCESS;

	_print_wifi_ap_profile(apconfig);

	res = wifi_manager_connect_ap(apconfig);
	if (res != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "AP connect failed");
		return;
	}
	/* Wait for DHCP connection */
	WT_LOG(TAG, " wait join done");
	WIO_TEST_WAIT;

	WT_LEAVE;
}

static void _wm_interop_scan(wifi_manager_ap_config_s *ap_info)
{
	WT_ENTER;
	wifi_manager_result_e res = WIFI_MANAGER_SUCCESS;
	wifi_manager_scan_config_s sconfig;

	sconfig.channel = 0;
	sconfig.ssid_length = strlen(ap_info->ssid);
	strncpy(sconfig.ssid, ap_info->ssid, sconfig.ssid_length + 1);

	res = wifi_manager_scan_ap(&sconfig);

	if (res != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "scan Fail");
		return;
	}
	WIO_TEST_WAIT; // wait the scan result

	WT_LEAVE;
}

static int _wm_interop_store_file(char *buf, unsigned int buf_size, char *file_name)
{
	FILE *fp;

	WT_ENTER;

	fp = fopen(file_name, "a+");

	if (!fp) {
		WT_LOGE(TAG, "file open error(%d)", errno);
		return -1;
	}

	int ret = fputs(buf, fp);
	if (ret < 0) {
		WT_LOGE(TAG, "file write error(%d)", errno);
		fclose(fp);
		return -1;
	}

	fclose(fp);

	WT_LEAVE;
	return 0;
}

static int _wm_interop_save_ap_config(struct wt_options *ap_info)
{
	WT_ENTER;

	char buf[WIFI_INTEROP_BUFSIZE];
	int ret = -1;
	char *file_name = NULL;

	if (ap_info == NULL) {
		WT_LOGE(TAG, "AP Info is NULL");
		return -1;
	}

	snprintf(buf, WIFI_INTEROP_BUFSIZE, "%s" DELIMITER "%s" DELIMITER "%s" DELIMITER "\n",
			ap_info->ssid, ap_info->security, ap_info->password);


	file_name = WIFI_INTEROP_PATH WIFI_INTEROP_FILENAME;

	ret = _wm_interop_store_file(buf, WIFI_INTEROP_BUFSIZE, file_name);
	if (ret < 0) {
		WT_LOGE(TAG, "store file failed");
		return -2;
	}

	WT_LEAVE;
	return 0;
}

void  wm_interop_add_ap_config(void *arg)
{
	WT_ENTER;

	struct wt_options *ap_info = (struct wt_options *)arg;
	int ret = -1;

	WT_LOG(TAG, "ssid[%s] security[%s] password[%s]",
			ap_info->ssid, ap_info->security, ap_info->password);

	ret = _wm_interop_save_ap_config(ap_info);
	if (ret < 0) {
		WT_LOGE(TAG, "Save AP configuration failed");
		return;
	}

	WT_LEAVE;
}

static void _build_ap_config(int idx, char *token, interop_ap_info_s *ap_info)
{
	switch (idx) {
	case SSID:
		memset(ap_info->ap_config.ssid, 0, WIFIMGR_SSID_LEN + 1);
		ap_info->ap_config.ssid_length = strlen(token);
		memcpy(ap_info->ap_config.ssid, token, ap_info->ap_config.ssid_length);

		break;
	case SECURITY:
		memcpy(ap_info->security, token, MAX_INTEROP_SECURITY_LEN);
		ap_info->ap_config.ap_auth_type = wt_get_auth_type(token);
		if (ap_info->ap_config.ap_auth_type == WIFI_MANAGER_AUTH_UNKNOWN) {
			WT_LOGE(TAG, "Unknown Auth type");
			break;
		}

		if (ap_info->ap_config.ap_auth_type == WIFI_MANAGER_AUTH_OPEN
			 || ap_info->ap_config.ap_auth_type == WIFI_MANAGER_AUTH_IBSS_OPEN) {

			ap_info->ap_config.ap_crypto_type = WIFI_MANAGER_CRYPTO_NONE;
			memset(ap_info->ap_config.passphrase, 0, WIFIMGR_PASSPHRASE_LEN + 1);
			strcpy(ap_info->ap_config.passphrase, "");
			ap_info->ap_config.passphrase_length = 0;
			break;
		}

		ap_info->ap_config.ap_crypto_type = wt_get_crypto_type(token);
		if (ap_info->ap_config.ap_crypto_type == WIFI_MANAGER_CRYPTO_UNKNOWN) {
			WT_LOGE(TAG, "Unknown Crypto Type");
			break;
		}

		break;

	case PASSPHRASE:
		memset(ap_info->ap_config.passphrase, 0, WIFIMGR_PASSPHRASE_LEN + 1);
		ap_info->ap_config.passphrase_length = strlen(token);
		memcpy(ap_info->ap_config.passphrase, token, ap_info->ap_config.passphrase_length);

		if (ap_info->ap_config.ap_auth_type == WIFI_MANAGER_AUTH_WEP_SHARED) {
			if ((strlen(token) == 13) || (strlen(token) == 26))
				ap_info->ap_config.ap_crypto_type = WIFI_MANAGER_CRYPTO_WEP_128;
			else if ((strlen(token) == 5) || (strlen(token) == 10))
				ap_info->ap_config.ap_crypto_type = WIFI_MANAGER_CRYPTO_WEP_64;
			else
				WT_LOGE(TAG, "Unknown Crypto Type");
		}

		break;
	default:
		WT_LOGE(TAG, "unknown");
		break;
	}
}

static void _wifi_interop_parse_ap_config(char * buf, interop_ap_info_s *ap_info)
{
	char *token = NULL;
	char *ap_buf = NULL;
	int idx = 0;

	int len = strlen(buf);

	//remove the new line character '\n' from buf a,b,c,\n
	ap_buf = strndup(buf, len-1);

	token = strtok(ap_buf, DELIMITER);

	while (token != NULL) {
		if (idx > 2) {
			WT_LOGE(TAG, "invalid data");
			break;
		}

		_build_ap_config(idx, token, ap_info);
		idx++;

		token = strtok(NULL, DELIMITER);
	}

	free(ap_buf);
}

int wifi_interop_read_file(interop_ap_config_list_s *ap_config_list, char *file_path)
{
	FILE *fp;
	char read_buf[WIFI_INTEROP_BUFSIZE] = {0,};
	int ap_idx = 0;

	WT_ENTER;

	memset(read_buf, 0 , WIFI_INTEROP_BUFSIZE);

	fp = fopen(file_path, "r");
	if (!fp) {
		WT_LOGE(TAG, "file open error(%d)", errno);
		return -1;
	}

	fseek(fp, 0, SEEK_SET);

	while (fgets(read_buf, WIFI_INTEROP_BUFSIZE, fp)) {
		interop_ap_info_s *ap_info = ap_config_list->ap_info + ap_idx;
		_wifi_interop_parse_ap_config(read_buf, ap_info);
		ap_idx++;

		if (ap_idx == MAX_INTEROP_AP_CONFIG_COUNT) {
			WT_LOG(TAG, "MAX_INTEROP_AP_CONFIG_COUNT limit reached");
			break;
		}
	}

	ap_config_list->ap_count = ap_idx;

	WT_LEAVE;
	fclose(fp);
	return 0;
}

static int _wm_interop_read_config(interop_ap_config_list_s *ap_config_list, char *path)
{
	int ret = 0;

	WT_ENTER;

	ret = wifi_interop_read_file(ap_config_list, path);
	if (ret < 0) {
		WT_LOGE(TAG, "failed to open file");

		return -1;
	}

	print_ap_config_list(ap_config_list);

	WT_LEAVE;

	return 0;
}

void wm_interop_display_ap_config(void *arg)
{
	WT_ENTER;
	int res = -1;
	struct wt_options *interop_info = (struct wt_options *)arg;

	interop_ap_config_list_s ap_config_list;
	res = _wm_interop_read_config(&ap_config_list, interop_info->path);
	if (res < 0) {
		WT_LOGE(TAG, "read config failed");
		return;
	}

	WT_LEAVE;
}

static void _summary_result_on_console(void)
{
	int ap_count = 0;
	interop_ap_info_s *ap_info = NULL;

	ap_count = g_apconfig_list.ap_count;

	WT_LOG(TAG, "Summary of Interoperability Test");
	WT_LOG(TAG, "====================================================================");
	WT_LOG(TAG, "SSID                              SECURITY       SCAN       CONNECT");

	for (int i = 0; i < ap_count; i++) {
		ap_info = g_apconfig_list.ap_info + i;
		WT_LOG(TAG, "%-33s %-15s %-11d %-7d", ap_info->ap_config.ssid, ap_info->security,
				ap_info->scan_result, ap_info->connect_result);
	}
	WT_LOG(TAG, "====================================================================");
}

static int _summary_result_on_file(void)
{
	char buf[WIFI_INTEROP_BUFSIZE];
	int ap_count = 0;
	interop_ap_info_s *ap_info = NULL;
	int ret = -1;
	FILE *fp;

	WT_ENTER;

	fp = fopen(WIFI_INTEROP_PATH WIFI_INTEROP_RESULT, "w+");

	if (!fp) {
		WT_LOGE(TAG, "file open error(%d)", errno);
		return -1;
	}

	memset(buf, 0, WIFI_INTEROP_BUFSIZE);
	snprintf(buf, WIFI_INTEROP_BUFSIZE, "Summary of Interoperability Test \n");
	ret = fputs(buf, fp);
	if (ret < 0) {
		WT_LOGE(TAG, "file write error(%d)", errno);
		fclose(fp);
		return -1;
	}

	memset(buf, 0, WIFI_INTEROP_BUFSIZE);
	snprintf(buf, WIFI_INTEROP_BUFSIZE, "====================================================================\n");
	ret = fputs(buf, fp);
	if (ret < 0) {
		WT_LOGE(TAG, "file write error(%d)", errno);
		fclose(fp);
		return -1;
	}

	memset(buf, 0, WIFI_INTEROP_BUFSIZE);
	snprintf(buf, WIFI_INTEROP_BUFSIZE, "SSID                              SECURITY       SCAN       CONNECT \n");
	ret = fputs(buf, fp);
	if (ret < 0) {
		WT_LOGE(TAG, "file write error(%d)", errno);
		fclose(fp);
		return -1;
	}


	ap_count = g_apconfig_list.ap_count;
	for (int i = 0; i < ap_count; i++) {
		ap_info = g_apconfig_list.ap_info + i;

		memset(buf, 0, WIFI_INTEROP_BUFSIZE);
		snprintf(buf, WIFI_INTEROP_BUFSIZE, "%-33s %-15s %-11d %-7d\n",
				ap_info->ap_config.ssid, ap_info->security,
				ap_info->scan_result, ap_info->connect_result);
		ret = fputs(buf, fp);
		if (ret < 0) {
			WT_LOGE(TAG, "file write error(%d)", errno);
			fclose(fp);
			return -1;
		}
	}

	memset(buf, 0, WIFI_INTEROP_BUFSIZE);
	snprintf(buf, WIFI_INTEROP_BUFSIZE, "====================================================================\n");
	ret = fputs(buf, fp);
	if (ret < 0) {
		WT_LOGE(TAG, "file write error(%d)", errno);
		fclose(fp);
		return -1;
	}

	fclose(fp);
	WT_LEAVE;
	return 0;
}

static int _wm_interop_init(void)
{
	WT_ENTER;

	wifi_manager_result_e res = WIFI_MANAGER_SUCCESS;

	res = wifi_manager_init(&g_wifi_callbacks);
	if (res != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "Failed to init wifi_manager");
		return 1 ;
	}

	WT_LEAVE;
	return 0;

}

static void _wm_interop_deinit(void)
{
	WT_ENTER;

	wifi_manager_result_e res = wifi_manager_deinit();
	if (res != WIFI_MANAGER_SUCCESS)
		WT_LOGE(TAG, "WiFi Manager failed to stop");

	WT_LEAVE;
}

static void _wm_interop_run_test(void *arg)
{
	WT_ENTER;

	int ap_count = 0;
	wifi_manager_result_e res = WIFI_MANAGER_SUCCESS;
	interop_ap_info_s *ap_info = NULL;
	struct wt_options *interop_info = (struct wt_options *)arg;

	memset(&g_apconfig_list, 0, sizeof(g_apconfig_list));
	res = _wm_interop_read_config(&g_apconfig_list, interop_info->path);
	if (res < 0) {
		WT_LOGE(TAG, "read config failed");
		return ;
	}

	ap_count = g_apconfig_list.ap_count;

	for (int i = 0; i < ap_count; i++) {
		ap_info = g_apconfig_list.ap_info + i;

		//scan AP
		WT_LOG(TAG, "Starting scan AP [%s]", ap_info->ap_config.ssid);
		g_current_ap_idx = i;
		g_ap_found_status = 0;
		g_connect_status = 0;

		_wm_interop_scan(&(ap_info->ap_config));

		//update scan result
		ap_info->scan_result = g_ap_found_status;

		if (!g_ap_found_status) {
			WT_LOGE(TAG, "failed to get AP in scan SSID: %s", ap_info->ap_config.ssid);
			continue;
		}

		//connect
		WT_LOG(TAG, "Starting Connect AP [%s]", ap_info->ap_config.ssid);
		_wm_interop_connect(&(ap_info->ap_config));

		//update connect status
		ap_info->connect_result = g_connect_status;

		if (g_connect_status) {
			WT_LOG(TAG, "AP connected  SSID: %s", ap_info->ap_config.ssid);

			_wm_interop_disconnect();
		}
	}

	_summary_result_on_console();

	_summary_result_on_file();

	WT_LEAVE;
}

static void _wm_run_interop(void *arg)
{
	WT_ENTER;

	int ret = -1;

	ret = _wm_interop_init();
	if (ret) {
		WT_LOGE(TAG, "Failed to run interop test ");
		return;
	}

	_wm_interop_run_test(arg);

	_wm_interop_deinit();

	WT_LEAVE;
}

void wm_test_interop(void *arg)
{
	WT_ENTER;
	int res = sem_init(&g_wio_sem, 0, 0);
	if (res < 0)
		WT_LOGE(TAG, "fail to initialize semaphore %d", errno);

	_wm_run_interop(arg);

	sem_destroy(&g_wio_sem);
	WT_LEAVE;
}

