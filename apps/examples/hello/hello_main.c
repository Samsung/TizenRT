/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
 * examples/hello/hello_main.c
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
#include <wifi_manager/wifi_manager.h>
/****************************************************************************
 * hello_main
 ****************************************************************************/
#define WT_TEST_SIGNAL							\
	do {										\
		sem_post(&g_wm_sem);					\
	} while (0)

#define WT_TEST_WAIT							\
	do {										\
		sem_wait(&g_wm_sem);					\
	} while (0)

#define TR_SSID "rt tahi"

static sem_t g_wm_sem = SEM_INITIALIZER(0);

void _wt_print_scanlist(wifi_manager_scan_info_s *slist)
{
	while (slist != NULL) {
		printf("WiFi AP SSID: %-25s, BSSID: %-20s, Rssi: %d, Auth: %d, Crypto: %d\n",
			   slist->ssid, slist->bssid, slist->rssi,
			   slist->ap_auth_type,
			   slist->ap_crypto_type);
		slist = slist->next;
	}
}

void _wt_scan_done(wifi_manager_scan_info_s **scan_result, wifi_manager_scan_result_e res)
{
	printf( "-->scan_result %p res %d\n", scan_result, res);
	/* Make sure you copy the scan results onto a local data structure.
	 * It will be deleted soon eventually as you exit this function.
	 */
	if (scan_result == NULL) {
		WT_TEST_SIGNAL;
		return;
	}
	_wt_print_scanlist(*scan_result);
	WT_TEST_SIGNAL;
}

static wifi_manager_cb_s g_wifi_callbacks = {
	NULL, NULL, NULL, NULL,
	_wt_scan_done,
};

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int hello_main(int argc, char *argv[])
#endif
{
	printf("Hello, World!!\n");
	if (argc == 2) {
		printf("tmp wi-fi test\n");
		wifi_manager_result_e wres = wifi_manager_init(&g_wifi_callbacks);
		if (wres != WIFI_MANAGER_SUCCESS) {
			printf("init fail\n");
			return -1;
		}
		wifi_manager_ap_config_s config;
		memset(&config, 0, sizeof(config));
		config.ssid_length = strlen(TR_SSID);
		strncpy(config.ssid, TR_SSID, config.ssid_length + 1);
		wres = wifi_manager_scan_specific_ap(&config);
		if (wres != WIFI_MANAGER_SUCCESS) {
			printf("scan fail\n");
			return -1;
		}
		WT_TEST_WAIT;
	}
	return 0;
}
