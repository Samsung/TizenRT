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
 * @file wifi-api.c
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <shell/tash.h>

#include <artik_module.h>
#include <artik_wifi.h>
#include <artik_network.h>

#define WIFI_SCAN_TIMEOUT       15
#define WIFI_CONNECT_TIMEOUT    30
#define WIFI_DISCONNECT_TIMEOUT 10

static artik_network_dhcp_client_handle g_dhcp_handle;

static void usage(void);

typedef int (*command_fn)(int argc, char *argv[]);

struct wifi_command {
	const char name[16];
	const char usage[64];
	command_fn fn;
};

struct callback_result {
	sem_t sem;
	artik_wifi_connection_info info;
	artik_error error;
};

static void wifi_scan_callback(void *result, void *user_data)
{
	struct callback_result *res = (struct callback_result *)user_data;

	res->error = *((artik_error *)result);
	sem_post(&(res->sem));
}

static void wifi_connect_callback(void *result, void *user_data)
{
	struct callback_result *res = (struct callback_result *)user_data;

	memcpy(&(res->info), (artik_wifi_connection_info *)result,
	       sizeof(artik_wifi_connection_info));
	sem_post(&(res->sem));
}

static int startsta_command(int argc, char *argv[])
{
	int ret = 0;
	artik_error err = S_OK;
	artik_wifi_module *wifi = (artik_wifi_module *)
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

exit:
	artik_release_api_module(wifi);
	return ret;
}

static int startap_command(int argc, char *argv[])
{
	int ret = 0;
	char *passphrase = NULL;
	artik_error err = S_OK;
	artik_wifi_module *wifi = (artik_wifi_module *)
		artik_request_api_module("wifi");

	if (!wifi) {
		fprintf(stderr, "Failed to request wifi module\n");
		return -1;
	}

	/* Check number of arguments */
	if (argc < 5) {
		fprintf(stderr, "Wrong number of arguments\n");
		usage();
		ret = -1;
		goto exit;
	}

	if (argc == 6)
		passphrase = argv[5];

	err = wifi->init(ARTIK_WIFI_MODE_AP);
	if (err != S_OK) {
		fprintf(stderr, "Failed to initialize wifi (%s)\n", error_msg(err));
		ret = -1;
		goto exit;
	}

	err = wifi->start_ap(argv[3], passphrase, atoi(argv[4]),
			passphrase ? WIFI_ENCRYPTION_WPA2 : WIFI_ENCRYPTION_OPEN);
	if (err != S_OK) {
		fprintf(stderr, "Failed to start Access Point (%s)\n", error_msg(err));
		ret = -1;
		goto exit;
	}

exit:
	artik_release_api_module(wifi);
	return ret;
}

static int scan_command(int argc, char *argv[])
{
	int ret = 0;
	artik_error err = S_OK;
	artik_wifi_ap *list = NULL;
	int count = 0, i = 0;
	struct callback_result scan_res;
	struct timespec timeout;
	artik_wifi_module *wifi = (artik_wifi_module *)
		artik_request_api_module("wifi");

	if (!wifi) {
		fprintf(stderr, "Failed to request wifi module\n");
		return -1;
	}

	err = wifi->set_scan_result_callback(wifi_scan_callback, (void *)&scan_res);
	if (err != S_OK) {
		fprintf(stderr, "Failed to set scan result callback (%s)\n",
			error_msg(err));
		ret = -1;
		goto exit;
	}

	sem_init(&(scan_res.sem), 0, 0);

	err = wifi->scan_request();
	if (err != S_OK) {
		sem_destroy(&(scan_res.sem));
		wifi->set_scan_result_callback(NULL, NULL);
		fprintf(stderr, "Failed to launch scan request (%s)\n", error_msg(err));
		ret = -1;
		goto exit;
	}

	clock_gettime(CLOCK_REALTIME, &timeout);
	timeout.tv_sec += WIFI_SCAN_TIMEOUT;
	if (sem_timedwait(&(scan_res.sem), &timeout) == ETIMEDOUT) {
		sem_destroy(&(scan_res.sem));
		wifi->set_scan_result_callback(NULL, NULL);
		fprintf(stderr, "Timed out while waiting for scan result\n");
		ret = -1;
		goto exit;
	}

	sem_destroy(&(scan_res.sem));
	wifi->set_scan_result_callback(NULL, NULL);

	if (scan_res.error != S_OK) {
		fprintf(stderr, "Failure while scanning the networks\n");
		ret = -1;
		goto exit;
	}

	err = wifi->get_scan_result(&list, &count);
	if (err != S_OK) {
		fprintf(stderr, "Failed to get scan results (%s)\n", error_msg(err));
		ret = -1;
		goto exit;
	}

	for (i = 0; i < count; i++) {
		fprintf(stderr, "%s\t%d\t%d\t%08x\t%s\n", list[i].bssid,
			list[i].frequency, list[i].signal_level,
			list[i].encryption_flags, list[i].name);
	}

	free(list);

exit:
	artik_release_api_module(wifi);
	return ret;
}

static int connect_command(int argc, char *argv[])
{
	int ret = 0;
	artik_error err = S_OK;
	struct callback_result conn_res;
	struct timespec timeout;
	bool persistent = false;
	artik_wifi_module *wifi = (artik_wifi_module *)
		artik_request_api_module("wifi");

	if (!wifi) {
		fprintf(stderr, "Failed to request wifi module\n");
		return -1;
	}

	/* Check number of arguments */
	if (argc < 5) {
		fprintf(stderr, "Wrong number of arguments\n");
		usage();
		ret = -1;
		goto exit;
	}

	if ((argc == 6) && !strncmp(argv[5], "persistent", strlen("persistent")))
		persistent = true;

	sem_init(&(conn_res.sem), 0, 0);

	err = wifi->set_connect_callback(wifi_connect_callback, &(conn_res.sem));
	if (err != S_OK) {
		sem_destroy(&(conn_res.sem));
		fprintf(stderr, "Failed to set connect callback (%)\n", error_msg(err));
		ret = -1;
		goto exit;
	}

	err = wifi->connect(argv[3], argv[4], persistent);
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

	fprintf(stderr, "Connected to %s\n", argv[3]);

exit:
	artik_release_api_module(wifi);
	return ret;
}

static int disconnect_command(int argc, char *argv[])
{
	int ret = 0;
	artik_error err = S_OK;
	struct callback_result conn_res;
	struct timespec timeout;
	artik_wifi_module *wifi = (artik_wifi_module *)
		artik_request_api_module("wifi");

	if (!wifi) {
		fprintf(stderr, "Failed to request wifi module\n");
		return -1;
	}

	sem_init(&(conn_res.sem), 0, 0);

	err = wifi->set_connect_callback(wifi_connect_callback, &(conn_res.sem));
	if (err != S_OK) {
		sem_destroy(&(conn_res.sem));
		fprintf(stderr, "Failed to set connect callback (%s)\n",
			error_msg(err));
		ret = -1;
		goto exit;
	}

	err = wifi->disconnect();
	if (err != S_OK) {
		sem_destroy(&(conn_res.sem));
		wifi->set_connect_callback(NULL, NULL);
		fprintf(stderr, "Failed to disconnect from AP (%s)\n", error_msg(err));
		ret = -1;
		goto exit;
	}

	clock_gettime(CLOCK_REALTIME, &timeout);
	timeout.tv_sec += WIFI_DISCONNECT_TIMEOUT;
	if ((sem_timedwait(&(conn_res.sem), &timeout) < 0)
	    && (errno == ETIMEDOUT)) {
		sem_destroy(&(conn_res.sem));
		wifi->set_connect_callback(NULL, NULL);
		fprintf(stderr, "Timed out while waiting for disconnection\n");
		ret = -1;
		goto exit;
	}

	sem_destroy(&(conn_res.sem));
	wifi->set_connect_callback(NULL, NULL);

	if (conn_res.info.connected) {
		fprintf(stderr, "Failed to disconnect from the network (err=%d)\n",
			conn_res.info.error);
		ret = -1;
		goto exit;
	}

exit:
	artik_release_api_module(wifi);
	return ret;
}

static int stop_command(int argc, char *argv[])
{
	int ret = 0;
	artik_error err = S_OK;
	artik_wifi_module *wifi = (artik_wifi_module *)
		artik_request_api_module("wifi");

	if (!wifi) {
		fprintf(stderr, "Failed to request wifi module\n");
		return -1;
	}

	err = wifi->deinit();
	if (err != S_OK) {
		fprintf(stderr, "Failed to deinitialize wifi (%s)\n", error_msg(err));
		ret = -1;
		goto exit;
	}

exit:
	artik_release_api_module(wifi);
	return ret;
}

static int dhcp_command(int argc, char *argv[])
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

static const struct wifi_command commands[] = {
	{ "startsta", "", startsta_command },
	{ "startap", "<ssid> <channel> [<passphrase>]", startap_command },
	{ "scan", "", scan_command },
	{ "connect", "<ssid> <passphrase> [persistent]", connect_command },
	{ "disconnect", "", disconnect_command },
	{ "stop", "", stop_command },
	{ "dhcp", "", dhcp_command },
	{ "", "", NULL }
};

static void usage(void)
{
	const struct wifi_command *cmd = commands;

	fprintf(stderr, "usage:\n");

	while (cmd->fn) {
		fprintf(stderr, "\twifi %s %s\n", cmd->name, cmd->usage);
		cmd++;
	}
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int wifictl_main(int argc, char *argv[])
#endif
{
	const struct wifi_command *cmd = commands;

	if (argc < 2) {
		usage();
		return -1;
	}

	while (cmd->fn) {
		if (!strcmp(argv[1], cmd->name)) {
			if (cmd->fn) {
				char tname[32];

				snprintf(tname, 32, "%s_command", cmd->name);
				task_create(tname, SCHED_PRIORITY_DEFAULT, 8192, cmd->fn, argv);
			}
			break;
		}

		cmd++;
	}

	if (!cmd->fn) {
		fprintf(stderr, "Unknown command\n");
		usage();
		return -1;
	}

	return 0;
}
