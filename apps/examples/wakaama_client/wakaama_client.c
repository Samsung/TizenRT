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

#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include "dm_lwm2m.h"
#include "dm_error.h"
#include "dm_connectivity.h"
#include "lwm2mclient.h"
#include "connection.h"
#include <tinyara/wqueue.h>
#include <tinyara/clock.h>
#include <protocols/dhcpc.h>
#include <slsi_wifi/slsi_wifi_api.h>

#ifdef CONFIG_WIFI_MANAGER
#define NET_DEVNAME	CONFIG_WIFIMGR_STA_IFNAME
#else
#define NET_DEVNAME "wl1"
#endif

#define WAKAAMA_CHECK_ARG() \
	do {\
		if (opt >= argc) { \
			app_print_usage(); \
			return 0; \
		} \
	} while (0);


static dm_scan_info_t *appwifiscan_result = NULL;
static dm_scan_info_t *wifi_iterator;

static char *wifi_aps[] = {"hack07", "hack10", "hack13", "hack14", "hack17", "hack18", "hack21", "hack23", "hack24", "hack28", "hack11"};

static uint8_t appscan_iteration;

#define DM_LIFETIME 300
#define DM_SERVERIP "192.168.1.152"
#define DM_SERVERPORT LWM2M_STANDARD_PORT_STR

#define APP_SERVER_IPLEN 20
#define APP_SERVER_PORTLEN 6

extern int dm_lwm2m_change_client_resource(char *buffer);

static int g_wakaama_initialized;

struct wakaama_param_s {
	char serverip[20];
	char serverport[6];
	unsigned int lifetime;
};

typedef struct wakaama_param_s wakaama_param_t;

static wakaama_param_t wakaama_param;

static struct dm_lwm2m_context_s dm_context;
static int g_isconnected;

struct work_s wakaama_wq;

void app_init_param(void)
{
	strncpy(dm_context.server_info.ipAddress, DM_SERVERIP, strlen(DM_SERVERIP));
	strncpy(dm_context.server_info.port, DM_SERVERPORT, strlen(DM_SERVERPORT));
	dm_context.client_info.lifetime = DM_LIFETIME;
}

void app_print_usage(void)
{
	fprintf(stdout, "Usage: wakaamaclient [OPTION]\r\n");
	fprintf(stdout, "Launch a Wakaama LWM2M client.\r\n");
	fprintf(stdout, "Options:\r\n");
	fprintf(stdout, "  -l PORT\tSet the local UDP port of the Client. Default: 56830\r\n");
	fprintf(stdout, "  -s SERVERIP\tSet the IP address of the LWM2M Server to connect to. Default: 192.168.0.4\r\n");
	fprintf(stdout, "  -p PORT\tSet the port of the LWM2M Server to connect to. Default: " LWM2M_STANDARD_PORT_STR "\r\n");
	fprintf(stdout, "  -t TIME\tSet the lifetime of the Client. Default: 300\r\n");
	fprintf(stdout, "  -b\t\tBootstrap requested.\r\n");
	fprintf(stdout, "  -c\t\tResume wakaama from previous state.\r\n");
	fprintf(stdout, "  -g\t\tGet server ip address\r\n");
	fprintf(stdout, "  -h\t\tGet server port\r\n");
	fprintf(stdout, "  -d\t\tDump an object.\r\n");
	fprintf(stdout, "  -r\t\tRead a resource value.\r\n");
#ifdef WITH_TINYDTLS
	fprintf(stdout, "  -i STRING\tSet the device management or bootstrap server PSK identity. If not set use none secure mode\r\n");
	fprintf(stdout, "  -S HEXSTRING\tSet the device management or bootstrap server Pre-Shared-Key. If not set use none secure mode\r\n");
#endif
	fprintf(stdout, "\r\n");
}

static void prv_wakaama_start(void)
{
	if (dm_lwm2m_start_client(&dm_context) != DM_ERROR_NONE) {
		fprintf(stderr, "Error creating wakaama thread\n");
	} else {
		fprintf(stderr, "Successfully created wakaama thread\n");
	}
}


static void change_resource(FAR void *arg)
{
	char argbuffer[20];
	memset(argbuffer, 0x00, 20);
	strcpy(argbuffer, "/3/0/11 7");
	dm_lwm2m_change_client_resource(argbuffer);
}

static void linkUpEvent(void)
{
	g_isconnected = 1;
	if (g_wakaama_initialized == 1) {
		int ret;

		printf("about to restart wakaama_client\n");
		ret = dm_lwm2m_start_client(&dm_context);
		if (ret != DM_ERROR_NONE) {
			fprintf(stderr, "Error creating wakaama thread\n");
		} else {
			fprintf(stderr, "Successfully created wakaama thread\n");
			work_queue(HPWORK, &wakaama_wq, change_resource, NULL, MSEC2TICK(1000));
		}

	}
}

static void linkDownEvent(void)
{
	g_isconnected = 0;
	if (g_wakaama_initialized == 1) {
		printf("about to close wakaama_client\n");
		dm_lwm2m_stop_client();
	}
}

int wakaamaclient_main(int argc, char *argv[])
{
	int opt;
	bool bootstrap_requested = false;

	if (argc == 1) {
		return 0;
	}

	if (!g_wakaama_initialized) {
		app_init_param();
		printf("trying connect\n");
		if (dm_conn_wifi_connect(linkUpEvent, linkDownEvent) != DM_ERROR_NONE) {
			return -1;
		}
		sleep(10);
		printf("trying dhcp\n");
		if (dm_conn_dhcp_init() != DM_ERROR_NONE) {
			return -1;
		}
	}
	opt = 1;

	while (opt < argc) {
		if (argv[opt] == NULL || argv[opt][0] != '-' || argv[opt][2] != 0) {
			app_print_usage();
			return 0;
		}
		switch (argv[opt][1]) {
		case 'b':
			opt++;
			WAKAAMA_CHECK_ARG();
			bootstrap_requested = true;
			strncpy(dm_context.server_info.ipAddress, \
					argv[opt], strlen(argv[opt]));
			break;
		case 't':
			opt++;
			WAKAAMA_CHECK_ARG();
			if (sscanf(argv[opt], "%d", &(dm_context.client_info.lifetime)) != 1) {
				app_print_usage();
				return 0;
			}
			break;

		case 's':
			opt++;
			WAKAAMA_CHECK_ARG();
			strncpy(dm_context.server_info.ipAddress, \
					argv[opt], strlen(argv[opt]));
			break;

		case 'c':
			prv_wakaama_start();
			return 0;
#ifdef WITH_TINYDTLS
		case 'i':
			opt++;
			WAKAAMA_CHECK_ARG();
			break;
		case 'S':
			opt++;
			WAKAAMA_CHECK_ARG();
			break;
#endif
		case 'n': {
			char argbuffer[20];
			opt++;
			WAKAAMA_CHECK_ARG();

			memset(argbuffer, 0x00, 20);
			strncpy(argbuffer, argv[opt], strlen(argv[opt]));
			opt++;
			strcat(argbuffer, " ");
			strncat(argbuffer, argv[opt], strlen(argv[opt]));
			dm_lwm2m_change_client_resource(argbuffer);
		}
		return 0;
		case 'l':
			opt++;
			WAKAAMA_CHECK_ARG();
			break;
		case 'p':
			opt++;
			WAKAAMA_CHECK_ARG();
			strncpy(wakaama_param.serverport, argv[opt], strlen(argv[opt]));
			break;
		case 'g': {
			char ipAddress[IPADDRLEN_MAX];
			int ret;

			WAKAAMA_CHECK_ARG();
			ret = dm_lwm2m_get_server_address(ipAddress);
			if (ret == DM_ERROR_NONE) {
				printf("Server ip address is %s\n", ipAddress);
			} else {
				printf("Error reading server ip address\n");
			}
			return 0;
		}
		case 'h': {
			char server_port[PORTLEN];
			int ret;

			WAKAAMA_CHECK_ARG();
			ret = dm_lwm2m_get_server_port(server_port);
			if (ret != DM_ERROR_NONE) {
				printf("Server port is %s\n", server_port);
			} else {
				printf("Error reading server port\n");
			}
			return 0;
		}
		case 'd': {
			char argbuffer[20];
			opt++;
			WAKAAMA_CHECK_ARG();
			printf("Read all resource of an object instance %s\n", argv[opt]);
			memset(argbuffer, 0x00, 20);
			strncpy(argbuffer, argv[opt], strlen(argv[opt]));
			if (dm_lwm2m_display_client_resource(argbuffer) != DM_ERROR_NONE) {
				printf("object dump did not work\n");
			}
			return 0;
		}
		case 'r':
			WAKAAMA_CHECK_ARG();
			printf("Read a resource value\n");
			return 0;

		case 'f': {
			int i;
			int wifi_count;
			while (1) {
				wifi_count = 0;
				if (dm_conn_wifi_scan() == DM_ERROR_NONE) {
					printf("DM scan network PASS\n");
				} else {
					printf("Failed on wifi scan\n");
				}
				sleep(10);
				if (dm_conn_get_scan_result(&appwifiscan_result) ==  DM_ERROR_NONE) {
					wifi_iterator = appwifiscan_result;
					while (wifi_iterator != NULL) {
						for (i = 0; i < sizeof(wifi_aps) / sizeof(char *); i++) {
							if (!strncmp(wifi_iterator->ssid, wifi_aps[i], strlen(wifi_aps[i]))) {
								printf("AP:%d:%s:%s:%d\n", appscan_iteration,
									   wifi_iterator->ssid,
									   wifi_iterator->bssid,
									   wifi_iterator->rssi);
							}
						}
						wifi_count++;
						wifi_iterator = wifi_iterator->next;
					}
				} else {
					printf("DM_ERROR_NO_DATA\n");
				}
				printf("AP:%d:totalAP#:%d\n", appscan_iteration, wifi_count);
				dm_conn_free_scan_result(&appwifiscan_result);
				appscan_iteration++;
			}
		}
		return 0;

		case 'x':
			dm_lwm2m_stop_client();
			return 0;


		default:
			app_print_usage();
			return 0;
		}
		opt += 1;
	}
	dm_context.server_info.isBootstrap = bootstrap_requested;
	prv_wakaama_start();
	g_wakaama_initialized = 1;
	return 0;
}
