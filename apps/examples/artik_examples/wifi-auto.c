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

/**
 * @file wifi-auto.c
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <shell/tash.h>

#include <artik_module.h>
#include <artik_wifi.h>
#include <artik_network.h>

#define WIFI_SSID NULL
#define WIFI_PASSPHRASE NULL

#define WIFI_SCAN_TIMEOUT       15
#define WIFI_CONNECT_TIMEOUT    30
#define WIFI_DISCONNECT_TIMEOUT 10

static artik_network_dhcp_client_handle g_dhcp_handle;

struct callback_result {
	sem_t sem;
	artik_wifi_connection_info info;
	artik_error error;
};

static void wifi_connect_callback(void *result, void *user_data)
{
	struct callback_result *res = (struct callback_result *)user_data;

	memcpy(&(res->info), (artik_wifi_connection_info *)result,
			sizeof(artik_wifi_connection_info));
	sem_post(&(res->sem));
}

static int wifi_connect(const char *ssid, const char *passphrase)
{
	int ret = 0;
	artik_error err = S_OK;
	struct callback_result conn_res;
	struct timespec timeout;
	artik_wifi_module *wifi = NULL;

	if (!ssid) {
		fprintf(stderr, "SSID is not defined.\n");
		return -1;
	}

	wifi = (artik_wifi_module *)
			artik_request_api_module("wifi");
	if (!wifi) {
		fprintf(stderr, "Failed to request wifi module\n");
		return -1;
	}

	err = wifi->init(ARTIK_WIFI_MODE_STATION);
	if (err != S_OK) {
		fprintf(stderr, "Failed to initialize wifi (%s)\n", error_msg(err));
		ret = -1;
		goto exit;
	}

	sem_init(&(conn_res.sem), 0, 0);

	err = wifi->set_connect_callback(wifi_connect_callback, &(conn_res.sem));
	if (err != S_OK) {
		sem_destroy(&(conn_res.sem));
		fprintf(stderr, "Failed to set connect callback (%)\n", error_msg(err));
		ret = -1;
		goto exit;
	}

	err = wifi->connect(ssid, passphrase, false);
	if (err != S_OK) {
		sem_destroy(&(conn_res.sem));
		wifi->set_connect_callback(NULL, NULL);
		fprintf(stderr, "Failed to start connection to AP (%s)\n",
				error_msg(err));
		ret = -1;
		goto exit;
	}

	clock_gettime(CLOCK_REALTIME, &timeout);
	timeout.tv_sec += WIFI_CONNECT_TIMEOUT;
	if ((sem_timedwait(&(conn_res.sem), &timeout) < 0)
			&& (errno == ETIMEDOUT)) {
		sem_destroy(&(conn_res.sem));
		wifi->set_connect_callback(NULL, NULL);
		fprintf(stderr, "Timed out while waiting for connection\n");
		ret = -1;
		goto exit;
	}

	sem_destroy(&(conn_res.sem));
	wifi->set_connect_callback(NULL, NULL);

	if (!conn_res.info.connected) {
		fprintf(stderr, "Failed to join the network (err=%d)\n",
				conn_res.info.error);
		ret = -1;
		goto exit;
	}

	fprintf(stderr, "Connected to %s\n", ssid);
exit:
	artik_release_api_module(wifi);
	return ret;
}

static int start_dhcp_client(void)
{
	int ret = 0;
	artik_network_module *network = (artik_network_module *)
			artik_request_api_module("network");
	artik_error err = S_OK;

	if (!network) {
		fprintf(stderr, "Failed to request network module\n");
		return -1;
	}

	err = network->dhcp_client_start(&g_dhcp_handle, ARTIK_WIFI);
	if (err != S_OK) {
		fprintf(stderr, "Failed to request DHCP lease (err=%d)\n", ret);
		ret = -1;
		goto exit;
	}

exit:
	artik_release_api_module(network);
	return ret;
}

void StartWifiConnection(void)
{
	if (wifi_connect(WIFI_SSID, WIFI_PASSPHRASE) != 0) {
		return;
	}

	start_dhcp_client();
}
