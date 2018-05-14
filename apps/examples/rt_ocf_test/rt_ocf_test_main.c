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
/****************************************************************************
 * apps/examples/rt_ocf_test/rt_ocf_test_main.c
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
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/types.h>
#include <time.h>

#include <tinyara/gpio.h>
#include <fcntl.h>

#include <wifi_manager/wifi_manager.h>

#include <ocf_rep.h>
#include <ocf_types.h>
#include <ocf_resources.h>
#include <ocf_request.h>
#include <ocf_observe.h>

//#define RT_OCF_SSID "Gorani_5G"
//#define RT_OCF_PASS "artikwifi1#"

#define RT_OCF_WIFI_WAIT (5);

static uint8_t g_rt_isWiFiConnected = 0;
uint8_t g_rt_terminate = 0;

static pthread_cond_t g_rt_wifi_condition;
static pthread_mutex_t g_rt_wifi_mutex;

/****************************************************************************
 * Generating SVR dat files
 ****************************************************************************/
static const char TEMP_DOXM_PATH[] = "/mnt/test_svr_doxm_server.dat";
static const char TEMP_CRED_PATH[] = "/mnt/test_svr_cred_server.dat";
static const char TEMP_ACL2_PATH[] = "/mnt/test_svr_acl2_server.dat";
static const char TEMP_PSTAT_PATH[] = "/mnt/test_svr_pstat_server.dat";

static const char TEMP_DOXM_DATA[] = {
	0xa7, 0x64, 0x6f, 0x78, 0x6d, 0x73, 0x81, 0x1b, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x66, 0x6f, 0x78, 0x6d, 0x73, 0x65, 0x6c, 0x1b,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x73, 0x63, 0x74,
	0x1b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x65, 0x6f, 0x77,
	0x6e, 0x65, 0x64, 0xf5, 0x6a, 0x64, 0x65, 0x76, 0x69, 0x63, 0x65, 0x75,
	0x75, 0x69, 0x64, 0x78, 0x24, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33,
	0x33, 0x2d, 0x33, 0x33, 0x33, 0x33, 0x2d, 0x33, 0x33, 0x33, 0x33, 0x2d,
	0x33, 0x33, 0x33, 0x33, 0x2d, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33,
	0x33, 0x33, 0x33, 0x33, 0x33, 0x6c, 0x64, 0x65, 0x76, 0x6f, 0x77, 0x6e,
	0x65, 0x72, 0x75, 0x75, 0x69, 0x64, 0x78, 0x24, 0x34, 0x34, 0x34, 0x34,
	0x34, 0x34, 0x34, 0x34, 0x2d, 0x34, 0x34, 0x34, 0x34, 0x2d, 0x34, 0x34,
	0x34, 0x34, 0x2d, 0x34, 0x34, 0x34, 0x34, 0x2d, 0x34, 0x34, 0x34, 0x34,
	0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x6a, 0x72, 0x6f, 0x77,
	0x6e, 0x65, 0x72, 0x75, 0x75, 0x69, 0x64, 0x78, 0x24, 0x34, 0x34, 0x34,
	0x34, 0x34, 0x34, 0x34, 0x34, 0x2d, 0x34, 0x34, 0x34, 0x34, 0x2d, 0x34,
	0x34, 0x34, 0x34, 0x2d, 0x34, 0x34, 0x34, 0x34, 0x2d, 0x34, 0x34, 0x34,
	0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34
};

static const char TEMP_CRED_DATA[] = {
	0xa2, 0x65, 0x63, 0x72, 0x65, 0x64, 0x73, 0x82, 0xa5, 0x66, 0x63, 0x72,
	0x65, 0x64, 0x69, 0x64, 0x1b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x6b, 0x73, 0x75, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x75, 0x75, 0x69,
	0x64, 0x78, 0x24, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x2d,
	0x31, 0x31, 0x31, 0x31, 0x2d, 0x31, 0x31, 0x31, 0x31, 0x2d, 0x31, 0x31,
	0x31, 0x31, 0x2d, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31,
	0x31, 0x31, 0x31, 0x68, 0x63, 0x72, 0x65, 0x64, 0x74, 0x79, 0x70, 0x65,
	0x1b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x66, 0x70, 0x65,
	0x72, 0x69, 0x6f, 0x64, 0x78, 0x1f, 0x32, 0x30, 0x31, 0x35, 0x30, 0x36,
	0x33, 0x30, 0x54, 0x30, 0x36, 0x30, 0x30, 0x30, 0x30, 0x2f, 0x32, 0x30,
	0x39, 0x39, 0x30, 0x39, 0x32, 0x30, 0x54, 0x32, 0x32, 0x30, 0x30, 0x30,
	0x30, 0x6b, 0x70, 0x72, 0x69, 0x76, 0x61, 0x74, 0x65, 0x64, 0x61, 0x74,
	0x61, 0xa2, 0x68, 0x65, 0x6e, 0x63, 0x6f, 0x64, 0x69, 0x6e, 0x67, 0x74,
	0x6f, 0x69, 0x63, 0x2e, 0x73, 0x65, 0x63, 0x2e, 0x65, 0x6e, 0x63, 0x6f,
	0x64, 0x69, 0x6e, 0x67, 0x2e, 0x72, 0x61, 0x77, 0x64, 0x64, 0x61, 0x74,
	0x61, 0x50, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,
	0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0xa5, 0x66, 0x63, 0x72, 0x65, 0x64,
	0x69, 0x64, 0x1b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x6b,
	0x73, 0x75, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x75, 0x75, 0x69, 0x64, 0x78,
	0x24, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x2d, 0x32, 0x32,
	0x32, 0x32, 0x2d, 0x32, 0x32, 0x32, 0x32, 0x2d, 0x32, 0x32, 0x32, 0x32,
	0x2d, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32,
	0x32, 0x68, 0x63, 0x72, 0x65, 0x64, 0x74, 0x79, 0x70, 0x65, 0x1b, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x66, 0x70, 0x65, 0x72, 0x69,
	0x6f, 0x64, 0x78, 0x1f, 0x32, 0x30, 0x31, 0x35, 0x30, 0x36, 0x33, 0x30,
	0x54, 0x30, 0x36, 0x30, 0x30, 0x30, 0x30, 0x2f, 0x32, 0x30, 0x39, 0x39,
	0x30, 0x39, 0x32, 0x30, 0x54, 0x32, 0x32, 0x30, 0x30, 0x30, 0x30, 0x6b,
	0x70, 0x72, 0x69, 0x76, 0x61, 0x74, 0x65, 0x64, 0x61, 0x74, 0x61, 0xa2,
	0x68, 0x65, 0x6e, 0x63, 0x6f, 0x64, 0x69, 0x6e, 0x67, 0x74, 0x6f, 0x69,
	0x63, 0x2e, 0x73, 0x65, 0x63, 0x2e, 0x65, 0x6e, 0x63, 0x6f, 0x64, 0x69,
	0x6e, 0x67, 0x2e, 0x72, 0x61, 0x77, 0x64, 0x64, 0x61, 0x74, 0x61, 0x50,
	0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,
	0x42, 0x42, 0x42, 0x42, 0x6a, 0x72, 0x6f, 0x77, 0x6e, 0x65, 0x72, 0x75,
	0x75, 0x69, 0x64, 0x78, 0x24, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34,
	0x34, 0x2d, 0x34, 0x34, 0x34, 0x34, 0x2d, 0x34, 0x34, 0x34, 0x34, 0x2d,
	0x34, 0x34, 0x34, 0x34, 0x2d, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34,
	0x34, 0x34, 0x34, 0x34, 0x34
};

static const char TEMP_ACL2_DATA[] = {
	0xa2, 0x67, 0x61, 0x63, 0x6c, 0x69, 0x73, 0x74, 0x32, 0x84, 0xa4, 0x65,
	0x61, 0x63, 0x65, 0x69, 0x64, 0x1b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x01, 0x67, 0x73, 0x75, 0x62, 0x6a, 0x65, 0x63, 0x74, 0xa1, 0x68,
	0x63, 0x6f, 0x6e, 0x6e, 0x74, 0x79, 0x70, 0x65, 0x6a, 0x61, 0x6e, 0x6f,
	0x6e, 0x2d, 0x63, 0x6c, 0x65, 0x61, 0x72, 0x69, 0x72, 0x65, 0x73, 0x6f,
	0x75, 0x72, 0x63, 0x65, 0x73, 0x84, 0xa1, 0x64, 0x68, 0x72, 0x65, 0x66,
	0x68, 0x2f, 0x6f, 0x69, 0x63, 0x2f, 0x72, 0x65, 0x73, 0xa1, 0x64, 0x68,
	0x72, 0x65, 0x66, 0x66, 0x2f, 0x6f, 0x69, 0x63, 0x2f, 0x64, 0xa1, 0x64,
	0x68, 0x72, 0x65, 0x66, 0x66, 0x2f, 0x6f, 0x69, 0x63, 0x2f, 0x70, 0xa1,
	0x64, 0x68, 0x72, 0x65, 0x66, 0x6d, 0x2f, 0x6f, 0x69, 0x63, 0x2f, 0x73,
	0x65, 0x63, 0x2f, 0x64, 0x6f, 0x78, 0x6d, 0x6a, 0x70, 0x65, 0x72, 0x6d,
	0x69, 0x73, 0x73, 0x69, 0x6f, 0x6e, 0x1b, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x02, 0xa4, 0x65, 0x61, 0x63, 0x65, 0x69, 0x64, 0x1b, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x67, 0x73, 0x75, 0x62, 0x6a,
	0x65, 0x63, 0x74, 0xa1, 0x68, 0x63, 0x6f, 0x6e, 0x6e, 0x74, 0x79, 0x70,
	0x65, 0x6a, 0x61, 0x75, 0x74, 0x68, 0x2d, 0x63, 0x72, 0x79, 0x70, 0x74,
	0x69, 0x72, 0x65, 0x73, 0x6f, 0x75, 0x72, 0x63, 0x65, 0x73, 0x81, 0xa1,
	0x64, 0x68, 0x72, 0x65, 0x66, 0x6a, 0x2f, 0x6c, 0x69, 0x67, 0x68, 0x74,
	0x2f, 0x72, 0x65, 0x64, 0x6a, 0x70, 0x65, 0x72, 0x6d, 0x69, 0x73, 0x73,
	0x69, 0x6f, 0x6e, 0x1b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
	0xa4, 0x65, 0x61, 0x63, 0x65, 0x69, 0x64, 0x1b, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x03, 0x67, 0x73, 0x75, 0x62, 0x6a, 0x65, 0x63, 0x74,
	0xa1, 0x64, 0x75, 0x75, 0x69, 0x64, 0x78, 0x24, 0x31, 0x31, 0x31, 0x31,
	0x31, 0x31, 0x31, 0x31, 0x2d, 0x31, 0x31, 0x31, 0x31, 0x2d, 0x31, 0x31,
	0x31, 0x31, 0x2d, 0x31, 0x31, 0x31, 0x31, 0x2d, 0x31, 0x31, 0x31, 0x31,
	0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x69, 0x72, 0x65, 0x73,
	0x6f, 0x75, 0x72, 0x63, 0x65, 0x73, 0x81, 0xa1, 0x64, 0x68, 0x72, 0x65,
	0x66, 0x6b, 0x2f, 0x6c, 0x69, 0x67, 0x68, 0x74, 0x2f, 0x62, 0x6c, 0x75,
	0x65, 0x6a, 0x70, 0x65, 0x72, 0x6d, 0x69, 0x73, 0x73, 0x69, 0x6f, 0x6e,
	0x1b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0xa4, 0x65, 0x61,
	0x63, 0x65, 0x69, 0x64, 0x1b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x04, 0x67, 0x73, 0x75, 0x62, 0x6a, 0x65, 0x63, 0x74, 0xa1, 0x64, 0x75,
	0x75, 0x69, 0x64, 0x78, 0x24, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32,
	0x32, 0x2d, 0x32, 0x32, 0x32, 0x32, 0x2d, 0x32, 0x32, 0x32, 0x32, 0x2d,
	0x32, 0x32, 0x32, 0x32, 0x2d, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32,
	0x32, 0x32, 0x32, 0x32, 0x32, 0x69, 0x72, 0x65, 0x73, 0x6f, 0x75, 0x72,
	0x63, 0x65, 0x73, 0x81, 0xa1, 0x64, 0x68, 0x72, 0x65, 0x66, 0x6b, 0x2f,
	0x6c, 0x69, 0x67, 0x68, 0x74, 0x2f, 0x62, 0x6c, 0x75, 0x65, 0x6a, 0x70,
	0x65, 0x72, 0x6d, 0x69, 0x73, 0x73, 0x69, 0x6f, 0x6e, 0x1b, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x6a, 0x72, 0x6f, 0x77, 0x6e, 0x65,
	0x72, 0x75, 0x75, 0x69, 0x64, 0x78, 0x24, 0x34, 0x34, 0x34, 0x34, 0x34,
	0x34, 0x34, 0x34, 0x2d, 0x34, 0x34, 0x34, 0x34, 0x2d, 0x34, 0x34, 0x34,
	0x34, 0x2d, 0x34, 0x34, 0x34, 0x34, 0x2d, 0x34, 0x34, 0x34, 0x34, 0x34,
	0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34
};

static const char TEMP_PSTAT_DATA[] = {
	0xa7, 0x63, 0x64, 0x6f, 0x73, 0xa2, 0x61, 0x73, 0x1b, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x03, 0x61, 0x70, 0xf4, 0x64, 0x69, 0x73, 0x6f,
	0x70, 0xf5, 0x62, 0x63, 0x6d, 0x1b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x62, 0x74, 0x6d, 0x1b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x62, 0x6f, 0x6d, 0x1b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x04, 0x62, 0x73, 0x6d, 0x1b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x04, 0x6a, 0x72, 0x6f, 0x77, 0x6e, 0x65, 0x72, 0x75, 0x75, 0x69,
	0x64, 0x78, 0x24, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x2d,
	0x34, 0x34, 0x34, 0x34, 0x2d, 0x34, 0x34, 0x34, 0x34, 0x2d, 0x34, 0x34,
	0x34, 0x34, 0x2d, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34,
	0x34, 0x34, 0x34
};


/*
 * callbacks
 */
static void wm_sta_connected(wifi_manager_result_e);
static void wm_sta_disconnected(void);
static void wm_softap_sta_join(void);
static void wm_softap_sta_leave(void);
static void wm_scan_done(wifi_manager_scan_info_s **scan_result, wifi_manager_scan_result_e res);

static wifi_manager_cb_s wifi_callbacks = {
	wm_sta_connected,
	wm_sta_disconnected,
	wm_softap_sta_join,
	wm_softap_sta_leave,
	wm_scan_done,
};

/* Generating SVR dat files */

/****************************************************************************
 * Wifi Auto Connecting
 ****************************************************************************/

static void wm_sta_connected(wifi_manager_result_e res)
{
	pthread_mutex_lock(&g_rt_wifi_mutex);
	g_rt_isWiFiConnected = 1;
	printf("\n[WiFi] Connected!\n");
	pthread_cond_signal(&g_rt_wifi_condition);
	pthread_mutex_unlock(&g_rt_wifi_mutex);
}

static void wm_sta_disconnected(void)
{
	g_rt_isWiFiConnected = 0;
	printf("\n[WiFi] Disconnected!\n");
}

void wm_softap_sta_join(void)
{
}

void wm_softap_sta_leave(void)
{
}

void wm_scan_done(wifi_manager_scan_info_s **scan_result, wifi_manager_scan_result_e res)
{
}

static FILE *test_doxm_fopen(const char *path, const char *mode)
{
	(void)path;
	return fopen(TEMP_DOXM_PATH, mode);
}

static FILE *test_cred_fopen(const char *path, const char *mode)
{
	(void)path;
	return fopen(TEMP_CRED_PATH, mode);
}

static FILE *test_acl2_fopen(const char *path, const char *mode)
{
	(void)path;
	return fopen(TEMP_ACL2_PATH, mode);
}

static FILE *test_pstat_fopen(const char *path, const char *mode)
{
	(void)path;
	return fopen(TEMP_PSTAT_PATH, mode);
}

static void create_security_data_files(void)
{
	int fd = open(TEMP_DOXM_PATH, O_WRONLY | O_CREAT, 0644);
	if (fd > 0) {
		write(fd, TEMP_DOXM_DATA, sizeof(TEMP_DOXM_DATA));
		close(fd);
	}

	fd = open(TEMP_CRED_PATH, O_WRONLY | O_CREAT, 0644);
	if (fd > 0) {
		write(fd, TEMP_CRED_DATA, sizeof(TEMP_CRED_DATA));
		close(fd);
	}

	fd = open(TEMP_ACL2_PATH, O_WRONLY | O_CREAT, 0644);
	if (fd > 0) {
		write(fd, TEMP_ACL2_DATA, sizeof(TEMP_ACL2_DATA));
		close(fd);
	}

	fd = open(TEMP_PSTAT_PATH, O_WRONLY | O_CREAT, 0644);
	if (fd > 0) {
		write(fd, TEMP_PSTAT_DATA, sizeof(TEMP_PSTAT_DATA));
		close(fd);
	}
}

static void remove_security_data_files(void)
{
	unlink(TEMP_DOXM_PATH);
	unlink(TEMP_CRED_PATH);
	unlink(TEMP_ACL2_PATH);
	unlink(TEMP_PSTAT_PATH);
}


#define TAG "SIMPLE_SERVER"

typedef struct {
	const char *uri;
	ocf_resource_s resource;
	bool value;
	ocf_request_cb get;
	ocf_request_cb put;
	ocf_request_cb post;
	ocf_request_cb delete;
} ocf_item;

ocf_item blue_led = {.uri = "/light/blue" };
ocf_item red_led = {.uri = "/light/red" };

static ocf_rep_encoder_s create_item_representation(ocf_item *item)
{
	ocf_rep_encoder_s rep = ocf_rep_encoder_init(OCF_REP_MAP);
	if (!rep) {
		printf("[%s]Can not make rep!! \n", __func__);
		return NULL;
	}

	ocf_rep_add_bool_to_map(rep, "value", item->value);
	return rep;
}

static void notifiy_item_state(ocf_item *item)
{
	ocf_rep_encoder_s rep = create_item_representation(item);
	ocf_observe_notify(item->uri, rep);
	ocf_rep_encoder_release(rep);
	printf("[%s]Item Value Changed!!!!\n", __func__);
}

static ocf_result_t apply_interface_to_representation(ocf_resource_s resource, ocf_rep_encoder_s rep)
{
	if (rep) {
		ocf_res_add_if_rt_rep(rep, resource);
		return OCF_OK;
	}
	return OCF_ERROR;
}

static void blue_led_get_handler_func(ocf_request_s request, ocf_rep_decoder_s data)
{
	printf("Blue LED Receive GET Method\n");
	ocf_item *item = &blue_led;

	ocf_rep_encoder_s rep = create_item_representation(item);
	ocf_result_t ret = apply_interface_to_representation(item->resource, rep);

	if (ret == OCF_OK) {
		ocf_response_send(request, rep, OCF_RESPONSE_CONTENT);
		ocf_rep_encoder_release(rep);
	} else {
		ocf_response_send(request, NULL, OCF_RESPONSE_RESOURCE_NOT_FOUND);
	}
}

static void red_led_get_handler_func(ocf_request_s request, ocf_rep_decoder_s data)
{
	printf("Red LED Receive GET Method\n");
	ocf_item *item = &red_led;

	ocf_rep_encoder_s rep = create_item_representation(item);
	ocf_result_t ret = apply_interface_to_representation(item->resource, rep);
	if (ret == OCF_OK) {
		ocf_response_send(request, rep, OCF_RESPONSE_CONTENT);
		ocf_rep_encoder_release(rep);
	} else {
		ocf_response_send(request, NULL, OCF_RESPONSE_RESOURCE_NOT_FOUND);
	}
}

static void put_handler_func(ocf_request_s request, ocf_rep_decoder_s data)
{
	printf("Put request is received\n");
	ocf_response_send(request, NULL, OCF_RESPONSE_METHOD_NOT_ALLOWED);
}

static int gpio_read(int port)
{
	char buf[4];
	char devpath[16];
	int ret;
	int fd;

	snprintf(devpath, 16, "/dev/gpio%d", port);
	fd = open(devpath, O_RDWR);
	if (fd < 0) {
		printf("ERROR: open failed: %d\n", fd);
		return -1;
	}

	ret = ioctl(fd, GPIOIOC_SET_DIRECTION, GPIO_DIRECTION_IN);
	if (ret < 0) {
		printf("ERROR: ioctl failed: %d=n", ret);
		return -1;
	}

	ret = read(fd, buf, sizeof(buf));
	if (ret < 0) {
		printf("ERROR: read failed: %d=n", ret);
		return -1;
	}

	close(fd);

	return buf[0] == '1';
}

static void gpio_write(int port, int value)
{
	char buf[4];
	char devpath[16];
	int fd;
	int ret;

	snprintf(devpath, 16, "/dev/gpio%d", port);
	fd = open(devpath, O_RDWR);
	if (fd < 0) {
		printf("ERROR: open failed: %d\n", fd);
		return;
	}

	ret = ioctl(fd, GPIOIOC_SET_DIRECTION, GPIO_DIRECTION_OUT);
	if (ret < 0) {
		printf("ERROR: ioctl failed: %d\n", fd);
		return;
	}

	ret = write(fd, buf, snprintf(buf, sizeof(buf), "%d", ! !value));
	if (fd < 0) {
		printf("ERROR: write failed: %d\n", fd);
		return;
	}

	close(fd);
}

static void blue_led_post_handler_func(ocf_request_s request, ocf_rep_decoder_s data)
{
	printf("Blue LED receives POST Method\n");
	ocf_item *item = &blue_led;

	if (!request) {
		printf("[%s]request is null\n", __func__);
		return;
	}

	bool value = 0;
	if (OCF_OK == ocf_rep_get_bool_from_map(data, "value", &value)) {
		printf("[%s]value : %d\n", __func__, value);
		item->value = value;

		ocf_rep_encoder_s rep = create_item_representation(item);
		if (rep) {
			ocf_request_query_set_s *queries = ocf_request_get_queries(request);
			if (queries->if_query && strcmp(OIC_IF_BASELINE_VALUE, queries->if_query) == 0) {
				ocf_res_add_if_rt_rep(rep, item->resource);
			}
			ocf_response_send(request, rep, OCF_RESPONSE_CHANGED);
			ocf_rep_encoder_release(rep);

			notifiy_item_state(&blue_led);
			return;
		}
	}
	ocf_response_send(request, NULL, OCF_RESPONSE_NOT_ACCEPTABLE);
}

static void red_led_post_handler_func(ocf_request_s request, ocf_rep_decoder_s data)
{
	printf("Red LED receives POST Method\n");
	ocf_item *item = &red_led;

	if (!request) {
		printf("[%s]request is null\n", __func__);
		return;
	}

	bool value = 0;
	if (OCF_OK == ocf_rep_get_bool_from_map(data, "value", &value)) {
		printf("[%s]value : %d\n", __func__, value);
		item->value = value;

		ocf_rep_encoder_s rep = create_item_representation(item);
		if (rep) {
			ocf_request_query_set_s *queries = ocf_request_get_queries(request);
			if (queries->if_query && strcmp(OIC_IF_BASELINE_VALUE, queries->if_query) == 0) {
				ocf_res_add_if_rt_rep(rep, item->resource);
			}
			ocf_response_send(request, rep, OCF_RESPONSE_CHANGED);
			ocf_rep_encoder_release(rep);

			notifiy_item_state(&red_led);
			return;
		}
	}
	ocf_response_send(request, NULL, OCF_RESPONSE_NOT_ACCEPTABLE);
}

static void delete_handler_func(ocf_request_s request, ocf_rep_decoder_s data)
{
	printf("Delete request is received\n");
	ocf_response_send(request, NULL, OCF_RESPONSE_METHOD_NOT_ALLOWED);
}

static void create_ocf_item_resource(ocf_item *item)
{
	item->resource = ocf_res_new_resource(item->uri);
	ocf_res_set_discoverable(item->resource, true);
	ocf_res_set_observable(item->resource, true);
	ocf_res_set_interface(item->resource, OIC_IF_BASELINE);
	ocf_res_set_default_interface(item->resource, OIC_IF_BASELINE);
	const char *item_resource_types[1] = { "core.light" };
	ocf_res_set_resource_types(item->resource, item_resource_types, 1);
	ocf_res_set_secure(item->resource, true);
	ocf_res_set_resource_protocol(item->resource, OCF_COAP | OCF_COAPS);
	ocf_res_set_request_handler(item->resource, OCF_GET, item->get);
	ocf_res_set_request_handler(item->resource, OCF_PUT, item->put);
	ocf_res_set_request_handler(item->resource, OCF_POST, item->post);
	ocf_res_set_request_handler(item->resource, OCF_DELETE, item->delete);
	ocf_res_register_resource(item->resource);
}

/* Set PS handler */
static ocf_persistent_storage_handler_s temp_doxm_handler = { test_doxm_fopen, fread, fwrite, fclose };
static ocf_persistent_storage_handler_s temp_cred_handler = { test_cred_fopen, fread, fwrite, fclose };
static ocf_persistent_storage_handler_s temp_acl2_handler = { test_acl2_fopen, fread, fwrite, fclose };
static ocf_persistent_storage_handler_s temp_pstat_handler = { test_pstat_fopen, fread, fwrite, fclose };

static void *gpio_thread(void *data)
{
	int r_led = 45;
	int b_led = 49;
	int r_btn = 42;
	int b_btn = 44;
	int r_read = 0;
	int b_read = 0;

	int r_click = 0;
	int b_click = 0;

	while (!g_rt_terminate) {
		r_read = gpio_read(r_btn);
		b_read = gpio_read(b_btn);

		if (r_read == 0) {
			if (r_click == 0) {
				red_led.value = !(red_led.value);
				notifiy_item_state(&red_led);
				r_click = 1;
			}
		} else if (r_read == 1) {
			r_click = 0;
		}

		if (b_read == 0) {
			if (b_click == 0) {
				blue_led.value = !(blue_led.value);
				notifiy_item_state(&blue_led);
				b_click = 1;
			}
		} else if (b_read == 1) {
			b_click = 0;
		}

		if (red_led.value == false) {
			gpio_write(r_led, 0);
		} else {
			gpio_write(r_led, 1);
		}

		if (blue_led.value == false) {
			gpio_write(b_led, 0);
		} else {
			gpio_write(b_led, 1);
		}

		up_mdelay(100);
	}

	return NULL;
}

//static wifi_manager_cb_s wifi_callbacks = {
//  WiFi_Link_Up,
//  WiFi_Link_Down,
//  NULL,
//  NULL,
//  NULL,
//};

static void get_wait_timespec(struct timespec *ts)
{
	struct timeval tp;
	gettimeofday(&tp, NULL);
	ts->tv_sec = tp.tv_sec;
	ts->tv_nsec = tp.tv_usec * 1000;
	ts->tv_sec += RT_OCF_WIFI_WAIT;
}

static int wait_condition(pthread_cond_t *cond)
{
	pthread_mutex_lock(&g_rt_wifi_mutex);
	struct timespec ts;
	get_wait_timespec(&ts);
	int ret = pthread_cond_timedwait(cond, &g_rt_wifi_mutex, &ts);
	pthread_mutex_unlock(&g_rt_wifi_mutex);
	return ret;
}

int WiFi_Auto_Conn(void)
{
	wifi_manager_result_e res = WIFI_MANAGER_FAIL;

	res = wifi_manager_init(&wifi_callbacks);
	if (res != WIFI_MANAGER_SUCCESS) {
		printf(" wifi_manager_init fail\n");
		return -1;
	}

	res = wifi_manager_set_mode(STA_MODE, NULL);
	if (res != WIFI_MANAGER_SUCCESS) {
		printf(" Set AP mode Fail (%d)\n", res);
		return -1;
	}

	wifi_manager_ap_config_s apconfig;
//	strncpy(apconfig.ssid, RT_OCF_SSID, 33);
//	apconfig.ssid_length = strlen(RT_OCF_SSID);
//	strncpy(apconfig.passphrase, RT_OCF_PASS, 64);
//	apconfig.passphrase_length = strlen(RT_OCF_SSID);
	apconfig.ap_auth_type = WIFI_MANAGER_AUTH_WPA2_PSK;
	apconfig.ap_crypto_type = WIFI_MANAGER_CRYPTO_AES;

	res = wifi_manager_connect_ap(&apconfig);
	if (res != WIFI_MANAGER_SUCCESS) {
		printf(" AP connect failed\n");
		return -1;
	}
	wait_condition(&g_rt_wifi_condition);

	wifi_manager_info_s info;
	res = wifi_manager_get_info(&info);
	if (res != WIFI_MANAGER_SUCCESS) {
		printf("Get Wifi Info Fail\n");
		return -1;
	}

	if (info.status == AP_CONNECTED) {
		printf("[WiFi] MODE: station (connected)\n");
		printf("[WiFi] IP: %s\n", info.ip4_address);
		printf("[WiFi] SSID: %s\n", info.ssid);
		printf("[WiFi] RSSI: %d\n", info.rssi);
	} else if (info.status == AP_DISCONNECTED) {
		printf("[WiFi] MODE: station (disconnected)\n");
	}

	return 1;
}

/****************************************************************************
 * rt_ocf_test_main
 ****************************************************************************/
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int rt_ocf_test_main(int argc, char *argv[])
#endif
{
	const char *device_types[1] = { "oic.d.light" };
	char buf[20] = { 0, };
	uint16_t udp_normal_port_v4;
	uint16_t udp_secure_port_v4;
	pthread_t led_thread;
	pthread_attr_t led_thread_attr;
	int r;
	g_rt_terminate = 0;

	pthread_mutex_init(&g_rt_wifi_mutex, NULL);
	pthread_cond_init(&g_rt_wifi_condition, NULL);

	printf("Hello IOT_RT_Sample_Server\n");

//  if (WiFi_Auto_Conn() < 0) {
//      printf("WiFi Fail!!\n");
//      return -1;
//  }

	create_security_data_files();
	ocf_sec_register_ps_handler(&temp_doxm_handler, &temp_pstat_handler, &temp_cred_handler, &temp_acl2_handler);

	ocf_init(OCF_SERVER, "Samsung", OCF_SH_100);

	ocf_core_add_oic_d_type(device_types, 1);

	blue_led.get = blue_led_get_handler_func;
	blue_led.put = put_handler_func;
	blue_led.post = blue_led_post_handler_func;
	blue_led.delete = delete_handler_func;
	blue_led.value = false;

	red_led.get = red_led_get_handler_func;
	red_led.put = put_handler_func;
	red_led.post = red_led_post_handler_func;
	red_led.delete = delete_handler_func;
	red_led.value = false;

	create_ocf_item_resource(&blue_led);
	create_ocf_item_resource(&red_led);

	rt_transport_get_local_ipv4(buf, 20);

	rt_udp_get_normal_port_v4(&udp_normal_port_v4);
	rt_udp_get_secure_port_v4(&udp_secure_port_v4);

	printf("My IP : %s\n", buf);
	printf("udp_normal_port_v4: %u\nudp_secure_port_v4: %u\n", udp_normal_port_v4, udp_secure_port_v4);

	if ((r = pthread_attr_init(&led_thread_attr))) {
		printf("%s: pthread_attr_init failed, status=%d", __func__, r);
		return -1;
	}
	if ((r = pthread_attr_setstacksize(&led_thread_attr, 2048))) {
		printf("%s: pthread_attr_setstacksize failed, status=%d", __func__, r);
		return -1;
	}
	if ((r = pthread_create(&led_thread, &led_thread_attr, gpio_thread, NULL))) {
		printf("%s: pthred_create failed, status=%d", __func__, r);
		return -1;
	}
	if ((r = pthread_setname_np(led_thread, "gpio_thread"))) {
		printf("%s: pthread_setname_np failed, status=%d", __func__, r);
		return -1;
	}

	while (!g_rt_terminate) {
		sleep(1);
	};

	pthread_join(led_thread, NULL);
	ocf_terminate();
	remove_security_data_files();
	g_rt_terminate = 0;
	pthread_cond_destroy(&g_rt_wifi_condition);
	pthread_mutex_destroy(&g_rt_wifi_mutex);

	printf("\n===============================\n");
	printf("Terminate RT_OCF_Sample_Server\n");
	printf("===============================\n");

	return 0;
}
