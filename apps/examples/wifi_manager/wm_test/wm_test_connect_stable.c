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
#include <sys/types.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <queue.h>
#include <semaphore.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <wifi_manager/wifi_manager.h>
#include <stress_tool/st_perf.h>
#include "wm_test.h"
#include "wm_test_network.h"
#include "wm_test_utils.h"
#include "wm_test_log.h"

#ifndef CONFIG_ENABLE_CURL
void wm_test_connect_stable(void *arg)
{
	printf("Turn on curl to test this\n");
}
#else

#define REQ_URL "www.google.com"
#define CHUNK_SIZE 512
//#define PRINT_RESPONSE
#define TAG "[WT]"

struct message {
	char *buf;
	int buf_size;
	int received;
};

static void wm_cb_sta_connected(wifi_manager_cb_msg_s msg, void *arg);
static void wm_cb_sta_disconnected(wifi_manager_cb_msg_s msg, void *arg);
static void wm_cb_scan_done(wifi_manager_cb_msg_s msg, void *arg);

static wifi_manager_ap_config_s g_apconfig;
static struct wo_queue *g_wo_queue = NULL;
static int g_terminate = 0;
static wifi_manager_scan_config_s g_scanconfig;
static wifi_manager_cb_s g_wifi_callbacks = {
	wm_cb_sta_connected,
	wm_cb_sta_disconnected,
	NULL,
	NULL,
	wm_cb_scan_done,
};
static uint32_t g_conn_fail = 0;
static uint32_t g_http_fail = 0;
static uint32_t g_conn_success = 0;
static uint32_t g_http_success = 0;
static uint32_t g_total_conn_trial = 0;
static uint32_t g_total_http_trial = 0;
static uint32_t g_scan_fail = 0;
static uint32_t g_scan_success = 0;
static uint32_t g_total_scan_trial = 0;
static uint32_t g_disconn = 0;

static void _print_stats(void)
{
	WT_LOG(TAG, "conn (%d/%d/%d) http (%d/%d/%d) scan (%d/%d/%d) disconn(%d)\n",
		   g_conn_success, g_conn_fail, g_total_conn_trial,
		   g_http_success, g_http_fail, g_total_http_trial,
		   g_scan_success, g_scan_fail, g_total_scan_trial,
		   g_disconn);
}

void wm_cb_sta_connected(wifi_manager_cb_msg_s msg, void *arg)
{
	WT_LOG(TAG, "--> res(%d)", msg.res);
	WT_LOG(TAG, "bssid %02x:%02x:%02x:%02x:%02x:%02x",
		   msg.bssid[0], msg.bssid[1],
		   msg.bssid[2], msg.bssid[3],
		   msg.bssid[4], msg.bssid[5]);
	int conn = 0;
	if (WIFI_MANAGER_SUCCESS == msg.res) {
		conn = WO_CONN_SUCCESS;
	} else {
		conn = WO_CONN_FAIL;
	}
	WO_TEST_SIGNAL(conn, g_wo_queue);
}

void wm_cb_sta_disconnected(wifi_manager_cb_msg_s msg, void *arg)
{
	WT_LOG(TAG, "--> res(%d) reason %d", msg.res, msg.reason);
	g_disconn++;
	g_terminate = 0;
}

void wm_cb_scan_done(wifi_manager_cb_msg_s msg, void *arg)
{
	WT_LOG(TAG, "--> res(%d)", msg.res);
	/* Make sure you copy the scan results onto a local data structure.
	 * It will be deleted soon eventually as you exit this function.
	 */
	if (msg.res != WIFI_MANAGER_SUCCESS || msg.scanlist == NULL) {
		WO_TEST_SIGNAL(WO_CONN_FAIL, g_wo_queue);
		return;
	}
	wt_print_scanlist(msg.scanlist);
	WO_TEST_SIGNAL(WO_CONN_SUCCESS, g_wo_queue);
}

static size_t curl_callback(void *contents, size_t size, size_t nmemb,
							void *userp)
{
	struct message *msg = (struct message *)userp;
#ifdef PRINT_RESPONSE
	if (msg->received + (size * nmemb) > msg->buf_size) {
		/*  realloc buf */
		int block_size = ((msg->received + (size * nmemb)) / CHUNK_SIZE) + 1;
		msg->buf = (char *)realloc(msg->buf, block_size * CHUNK_SIZE);
		if (!msg->buf) {
			WT_LOGE(TAG, "fail to alloc %s\n", strerror(errno));
			assert(0);
		}
		msg->buf_size = block_size * CHUNK_SIZE;
	}
	memcpy(msg->buf + msg->received, contents, (size * nmemb));
#endif
	msg->received += (size * nmemb);

	return size * nmemb;
}

static int _get_http_msg(void)
{
	CURL *curl;
	CURLcode res;
	struct message msg = {NULL, CHUNK_SIZE, 0};
	msg.buf = (char *)malloc(CHUNK_SIZE);
	if (!msg.buf) {
		WT_LOGE(TAG, "malloc fail %s\n", strerror(0));
		assert(0);
	}

	/* In windows, this will init the winsock stuff */
	curl_global_init(CURL_GLOBAL_ALL);

	/* get a curl handle */
	curl = curl_easy_init();
	if (curl) {
		/* First set the URL that is about to receive our POST. This URL can
just as well be a https:// URL if that is what should receive the
data. */
		curl_easy_setopt(curl, CURLOPT_URL, REQ_URL);
		struct curl_slist *list = NULL;
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
		curl_easy_setopt(curl, CURLOPT_HEADER, 1); // get response header?
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&msg);

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
					curl_easy_strerror(res));
		}
		/*  print http response */
		printf("=================================\n");
		printf("Response\n");
		printf("----------------------------------\n");
#ifdef PRINT_RESPONSE
		printf("%s\n\n", msg.buf);
#else
		printf("total received %d\n", msg.received);
#endif
		/* always cleanup */
		curl_easy_cleanup(curl);
		curl_slist_free_all(list);
	}
	curl_global_cleanup();

	free(msg.buf);
	return 0;
}

/*  every 1 seconds, it requests http GET message */
static int _run_http_transmit(void)
{
	while (g_terminate) {
		_print_stats();
		g_total_http_trial++;
		int res = _get_http_msg();
		if (res != 0) {
			g_http_fail++;
			WT_LOGE(TAG, "get http message fail\n");
		} else {
			g_http_success++;
		}
		sleep(1);
	}
	WT_LOG(TAG, "AP disconnected\n");
	return 0;
}

static int _connect_ap(void)
{
	wifi_manager_result_e wres = WIFI_MANAGER_SUCCESS;
	int conn = WO_CONN_FAIL;
	do {
		_print_stats();
		g_total_scan_trial++;
		if ((wres = wifi_manager_scan_ap(&g_scanconfig)) != WIFI_MANAGER_SUCCESS) {
			WT_LOGE(TAG, "scan fail %d\n", wres);
			g_scan_fail++;
			continue;
		}
		WT_LOG(TAG, "wait scan event\n");
		WO_TEST_WAIT(conn, g_wo_queue);
		if (conn != WO_CONN_SUCCESS) {
			WT_LOGE(TAG, "get scan event fail %d\n", wres);
			g_scan_fail++;
			continue;
		}
		WT_LOG(TAG, "scan done\n");
		g_scan_success++;

		g_total_conn_trial++;
		if ((wres = wifi_manager_connect_ap(&g_apconfig)) != WIFI_MANAGER_SUCCESS) {
			WT_LOGE(TAG, "connect fail %d\n", wres);
			g_conn_fail++;
			continue;
		}
		WT_LOG(TAG, "connection success\n");
		WO_TEST_WAIT(conn, g_wo_queue);
		if (conn != WO_CONN_SUCCESS) {
			WT_LOGE(TAG, "get connection event fail %d\n", conn);
			g_conn_fail++;
			continue;
		}
		WT_LOG(TAG, "get connection event\n");
		g_conn_success++;
		break;
	} while (1);

	return 0;
}

static void _run_procedure(void)
{
	wifi_manager_result_e wres = WIFI_MANAGER_SUCCESS;
	if ((wres = wifi_manager_init(&g_wifi_callbacks)) != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "init fail %d\n", wres);
		assert(0);
	}
	do {
		int res = _connect_ap();
		if (res != 0) {
			WT_LOGE(TAG, "connection fail %d\n", res);
			assert(0);
		}
		g_terminate = 1;

		res = _run_http_transmit();
		if (res != 0) {
			WT_LOGE(TAG, "connection fail %d\n", res);
			assert(0);
		}
	} while (1);
	if ((wres = wifi_manager_deinit()) != WIFI_MANAGER_SUCCESS) {
		WT_LOGE(TAG, "deint fail %d\n", wres);
		assert(0);
	}
}

void wm_test_connect_stable(void *arg)
{
	if (!arg) {
		WT_LOGE(TAG, "invalid param\n");
		return;
	}
	struct wt_options *opt = (struct wt_options *)arg;
	wm_get_apinfo(&g_apconfig, opt->ssid, opt->password, opt->auth_type, opt->crypto_type);
	wt_print_wifi_ap_profile(&g_apconfig, "Connection Stable AP setting");
	g_scanconfig.ssid_length = strlen(opt->ssid);
	strncpy(g_scanconfig.ssid, opt->ssid, g_scanconfig.ssid_length);
	g_scanconfig.channel = 0;

	g_wo_queue = wo_create_queue();

	_run_procedure();

	wo_destroy_queue(g_wo_queue);
}
#endif
