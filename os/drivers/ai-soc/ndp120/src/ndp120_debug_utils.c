
/*
 * SYNTIANT CONFIDENTIAL
 * _____________________
 *
 *   Copyright (c) 2018-2024 Syntiant Corporation
 *   All Rights Reserved.
 *
 *  NOTICE:  All information contained herein is, and remains the property of
 *  Syntiant Corporation and its suppliers, if any.  The intellectual and
 *  technical concepts contained herein are proprietary to Syntiant Corporation
 *  and its suppliers and may be covered by U.S. and Foreign Patents, patents in
 *  process, and are protected by trade secret or copyright law.  Dissemination
 *  of this information or reproduction of this material is strictly forbidden
 *  unless prior written permission is obtained from Syntiant Corporation.
	** SDK: v112-3.6-Samsung **
*/


#include <tinyara/kmalloc.h>
#include <tinyara/wqueue.h>
#include <tinyara/pm/pm.h>

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>

#ifdef CONFIG_SYSTEM_INIFILE
#include <sys/socket.h>
#include <arpa/inet.h>
#include <wifi_manager/wifi_manager.h>
#include "../../../apps/include/inifile.h"

#define WIFI_INI_FILE_NAME "/mnt/kernel/audio/wifi.ini"
#define WIFI_CONNECT_ATTEMPTS 8

static char *server = NULL;
static int port = 12345;

static wifi_manager_cb_s g_wifi_callbacks = {NULL, NULL, NULL, NULL, NULL};
static wifi_manager_ap_config_s ap_config;

int wifi_start(char* ssid, char *psk)
{
    int repeat = 0;
    wifi_manager_result_e wres = WIFI_MANAGER_SUCCESS;
    while(repeat++ < 3){
    	wres = wifi_manager_init(&g_wifi_callbacks);
		if (wres != WIFI_MANAGER_SUCCESS) {
			printf("fail to initialize wifi manager, ret: %d\n", wres);
			printf("Will try again in a sec..\n");
			wifi_manager_deinit();
			usleep(1000000);
			continue;
		}
		strncpy(ap_config.ssid, ssid, strlen(ssid) + 1);
		ap_config.ssid_length = strlen(ssid);
		strncpy(ap_config.passphrase, psk, strlen(psk) + 1);
		ap_config.passphrase_length = strlen(psk);
		ap_config.ap_auth_type = WIFI_MANAGER_AUTH_WPA2_PSK;
		ap_config.ap_crypto_type = WIFI_MANAGER_CRYPTO_NONE;
		wres = wifi_manager_connect_ap(&ap_config);
		if (wres != WIFI_MANAGER_SUCCESS) {
			printf("failed to connect to %s, ret: %d\n", ssid, wres);
			printf("Will try again in a sec..\n");
			wifi_manager_deinit();
			usleep(1000000);
			continue;
		} else {
			printf("Connected to %s, ip addr can be checked using ifconfig\n", ssid);
			break;
		}
    }
    return wres;
}

void ndp120_utils_wifi_setup(void)
{
	/* read ini file to set up wifi */
	INIHANDLE handle = inifile_initialize(WIFI_INI_FILE_NAME);
	if (handle) {
		char *ssid;
		char *passwd;
		int i;
		ssid = inifile_read_string(handle, "config", "ssid", "ssid");
		passwd = inifile_read_string(handle, "config", "psk", "psk");

		server = inifile_read_string(handle, "config", "server", "server");
		port = inifile_read_integer(handle, "config", "port", 12345);

		wifi_manager_info_s wifi_info;
		wifi_manager_get_info(&wifi_info);
		if (wifi_info.status != AP_CONNECTED) {
			wifi_start(ssid, passwd);
		}
		inifile_free_string(ssid);
		inifile_free_string(passwd);

		for (i = 0; i < WIFI_CONNECT_ATTEMPTS; i++) {
            wifi_manager_get_info(&wifi_info);
            if (wifi_info.status == AP_CONNECTED) {
                printf("AP connected!\n");
                break;
            }
            usleep(250000);
		}
	}
}

void ndp120_utils_stream_file(const char* filename)
{
	struct sockaddr_in address;
	int s;
	int fd;
	int chunksize = 644;
	int conn_ok = 1;

	if (!server) return;

	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(server);
	address.sin_port = htons(port);

	int socket_send = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(socket_send, (struct sockaddr *)&address, sizeof(address))) {
		printf("Could not connect to server\n");
		conn_ok = 0;
	}

	if (conn_ok) {
		uint8_t *data = (uint8_t *) malloc(chunksize);
		fd = open(filename, O_RDONLY);
		if (!fd) {
			return;
		}

		for (;;) {
			int nbytesread;
			do {
				nbytesread = read(fd, data, chunksize);
			} while (nbytesread < 0 && errno == EINTR);

			if (nbytesread < 0) {
				printf("read failed: %d\n", errno);
				return;
			}

			else if (nbytesread == 0) {
				break;
			}
			s = send(socket_send, data, nbytesread, 0);
			if (s <= 0) {
				printf("Error sending\n");
			}
		}
		close(socket_send);
		close(fd);
	}
}
#else
/* nothing to do if no inifile support */
#endif