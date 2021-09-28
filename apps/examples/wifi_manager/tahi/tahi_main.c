/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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
/****************************************************************************
 * examples/wifi_manager/tahi/tahi_main.c
 *
 *   Copyright (C) 2008, 2011-2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <sys/types.h>
#include <semaphore.h>
#include <wifi_manager/wifi_manager.h>
#include <netutils/netlib.h>

/****************************************************************************
 * TAHI Test
 ****************************************************************************/

#define TAHI_IF_NAME "wlan0"
#define TAHI_SSID ""
#define TAHI_PASS ""

static sem_t sem;

struct options {
	char *ssid;
	char *password;
	wifi_manager_ap_auth_type_e    auth_type;
	wifi_manager_ap_crypto_type_e  crypto_type;
};

extern void nd6_cache_debug_print(void);

/*
 * Callback
 */
static void wm_sta_connected(wifi_manager_cb_msg_s msg, void *arg)
{
	printf(" T%d --> %s res(%d)\n", getpid(), __FUNCTION__, msg.res);
	sem_post(&sem);
}

static void wm_sta_disconnected((wifi_manager_cb_msg_s msg, void *arg)
{
	sleep(2);
	printf(" T%d --> %s\n", getpid(), __FUNCTION__);
}

static void wm_softap_sta_join(wifi_manager_cb_msg_s msg, void *arg)
{
	printf(" T%d --> %s\n", getpid(), __FUNCTION__);
}

static void wm_softap_sta_leave(wifi_manager_cb_msg_s msg, void *arg)
{
	printf(" T%d --> %s\n", getpid(), __FUNCTION__);
}

static void wm_scan_done(wifi_manager_cb_msg_s msg, void *arg)
{
	printf(" T%d --> %s\n", getpid(), __FUNCTION__);
}

static wifi_manager_cb_s wifi_callbacks = {
	wm_sta_connected,
	wm_sta_disconnected,
	wm_softap_sta_join,
	wm_softap_sta_leave,
	wm_scan_done,
};

static void _print_wifi_ap_profile(wifi_manager_ap_config_s *config, char *title)
{
	printf("====================================\n");
	if (title) {
		printf("[TAHI] %s\n", title);
	}
	printf("------------------------------------\n");
	printf("[TAHI] SSID: %s\n", config->ssid);
	if (config->ap_auth_type == WIFI_MANAGER_AUTH_UNKNOWN
		|| config->ap_crypto_type == WIFI_MANAGER_CRYPTO_UNKNOWN) {
		printf("[TAHI] SECURITY: unknown\n");
	} else {
		printf("[TAHI] SECURITY: %d CRYPTO: %d\n", config->ap_auth_type, config->ap_crypto_type);
	}
	printf("====================================\n");
}

static int _wm_connect(struct options *ap_info)
{
	wifi_manager_ap_config_s apconfig;
	strncpy(apconfig.ssid, ap_info->ssid, WIFIMGR_SSID_LEN);
	apconfig.ssid_length = strlen(ap_info->ssid);
	apconfig.ssid[WIFIMGR_SSID_LEN] = '\0';
	apconfig.ap_auth_type = ap_info->auth_type;
	apconfig.ap_crypto_type = ap_info->crypto_type;
	if (ap_info->auth_type != WIFI_MANAGER_AUTH_OPEN || ap_info->crypto_type != WIFI_MANAGER_CRYPTO_NONE) {
		strncpy(apconfig.passphrase, ap_info->password, WIFIMGR_PASSPHRASE_LEN);
		apconfig.passphrase[WIFIMGR_PASSPHRASE_LEN] = '\0';
		apconfig.passphrase_length = strlen(ap_info->password);
	} else {
		apconfig.passphrase[0] = '\0';
		apconfig.passphrase_length = 0;
	}

	_print_wifi_ap_profile(&apconfig, "Connecting AP Info");

	wifi_manager_result_e res = wifi_manager_connect_ap(&apconfig);
	if (res != WIFI_MANAGER_SUCCESS) {
		printf("[TAHI] AP connect failed\n");
		return -1;
	}
	/* Wait for DHCP connection */
	printf("[TAHI] wait join done\n");
	sem_wait(&sem);

	return 0;
}

static int g_tahi_init = 0;

static void print_help(void)
{
	printf("\n");
	printf("Usage: tahi [OPTIONS]\n");

	printf("\n");
	printf("And [OPTIONS] include:\n");
	printf("  1) <type>\n");
	printf("     <type> set IPv6 address style\n");
	printf("        0 : EUI-64 (default, you don't need to set value)\n");
	printf("        1 : Stable Privacy\n");
	printf("     example 1): tahi 1\n");
	printf("     example 2): tahi 0\n");
	printf("  2) log\n");
	printf("     Show nd6 cache data\n");
	printf("\n");
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int tahi_main(int argc, char *argv[])
#endif
{
	int ret;
	int ipv6_type = 0;
	wifi_manager_result_e res = WIFI_MANAGER_SUCCESS;

	printf("[TAHI] IPv6 Test App\n");

	if (argc != 2) {
		print_help();
		return 0;
	} else if (strncmp(argv[1], "log", 4) == 0) {
		/*  it's available if build mode is flat */
		nd6_cache_debug_print();
		return 0;
	} else if (g_tahi_init > 0) {
		printf("already initialized\n");
		print_help();
		return 0;
	}

	printf("[TAHI] Start to connect WiFi\n");
	sem_init(&sem, 0, 1);

	usleep(100 * 1000);

	res = wifi_manager_init(&wifi_callbacks);
	if (res != WIFI_MANAGER_SUCCESS) {
		printf("[TAHI] wifi_manager_init fail\n");
	}

	usleep(100 * 1000);

	printf("[TAHI] Disable Power mode\n");
	if (wifi_manager_set_powermode(WIFI_MANAGER_POWERMODE_DISABLE)
		!= WIFI_MANAGER_SUCCESS) {
		printf("[TAHI] wifi manager set power mode fail\n");
		return -1;
	}

	if (argv[1][0] == '1') {
		printf("[TAHI] Stable Privacy\n");
		ipv6_type = 1;
	} else if (argv[1][0] == '0') {
		printf("[TAHI] EUI-64\n");
		ipv6_type = 0;
	} else {
		print_help();
		return -1;
	}
	ret = netlib_set_ipv6type(TAHI_IF_NAME, ipv6_type);
	if (ret != 0) {
		printf("[TAHI] Set ipv6 type fail\n");
	}

	struct options opt = {
		.auth_type = WIFI_MANAGER_AUTH_WPA2_PSK,
		.crypto_type = WIFI_MANAGER_CRYPTO_AES,
		.ssid = TAHI_SSID,
		.password = TAHI_PASS
	};

	if (_wm_connect(&opt) != 0) {
		printf("[TAHI] Fail to connect WiFi\n");
		return 0;
	}
	printf("[TAHI] WiFi Connected\n");

	g_tahi_init = 1;

	return 0;
}
