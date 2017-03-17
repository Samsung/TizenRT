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

#include <apps/netutils/dhcpc.h>

#include <apps/netutils/wifi/slsi_wifi_api.h>

#define NET_DEVNAME "wl1"

#define DM_LIFETIME 300
#define DM_SERVERIP "192.168.1.152"
#define DM_SERVERPORT LWM2M_STANDARD_PORT_STR

#define APP_SERVER_IPLEN 20
#define APP_SERVER_PORTLEN 6

extern int dm_lwm2m_change_client_resource(char *buffer);

static int g_wakaama_initialized;

struct wakaama_param_s {
	unsigned char serverip[20];
	unsigned char serverport[6];
	unsigned int lifetime;
};

typedef struct wakaama_param_s wakaama_param_t;

static wakaama_param_t wakaama_param;

static struct dm_lwm2m_context_s dm_context;
static int isConnected = 0;

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
	int ret;
	ret = dm_lwm2m_start_client(&dm_context);
	if (ret != DM_ERROR_NONE) {
		fprintf(stderr, "Error creating wakaama thread\n");
	} else {
		fprintf(stderr, "Successfully created wakaama thread\n");
	}
}

static int app_dhcp_main(void)
{
	uint32_t timeleft = 15000;
	struct dhcpc_state state;
	void * dhcp_handle;
	int ret;

	dhcp_handle = dhcpc_open(NET_DEVNAME);
	ret = dhcpc_request(dhcp_handle, &state);
	if (ret != OK) {
			dhcpc_close(dhcp_handle);
			return -1;
	}

	netlib_set_ipv4addr(NET_DEVNAME, &state.ipaddr);
	netlib_set_ipv4netmask(NET_DEVNAME, &state.netmask);
	netlib_set_dripv4addr(NET_DEVNAME, &state.default_router);

	printf("IP address : %s ----\n", inet_ntoa(state.ipaddr));
	return 1;
}


static int change_resource(FAR void *arg)
{
	char argbuffer[20];
	memset(argbuffer, 0x00, 20);
	strcpy(argbuffer, "/3/0/11 7");
	dm_lwm2m_change_client_resource(argbuffer);
}

static conn_cb linkUpEvent()
{
	isConnected = 1;
	if (g_wakaama_initialized == 1) {
		int resource_val;
		int ret;

		printf("about to restart wakaama_client\n");
		resource_val = 7;
		ret = dm_lwm2m_start_client(&dm_context);
		if (ret != DM_ERROR_NONE) {
			fprintf(stderr, "Error creating wakaama thread\n");
		} else {
			fprintf(stderr, "Successfully created wakaama thread\n");
			work_queue(HPWORK, &wakaama_wq, change_resource, NULL, MSEC2TICK(1000));
		}

	}
}

static conn_cb linkDownEvent()
{
	isConnected = 0;
	if (g_wakaama_initialized == 1) {
		int resource_val;
		printf("about to close wakaama_client\n");
		dm_lwm2m_stop_client();
	}
}

static int wifiAutoConnectInit()
{
	int8_t ret;
	uint8_t result;

	isConnected = 0;
	dm_conn_register_linkup_cb(linkUpEvent);
	dm_conn_register_linkdown_cb(linkDownEvent);

	if (WifiIsConnected(&result, NULL) != SLSI_STATUS_SUCCESS) {
		printf("failed to WifiIsConnected\n");
		return;
	}

	if (result > 0) {
		printf("Wi-Fi status - Connected : %d\n", result);
		isConnected = 1;
		return 1;
	}

	ret = WiFiStart(SLSI_WIFI_STATION_IF, NULL);
	if (ret == SLSI_STATUS_SUCCESS) {
		printf("[AutoConnect]STA mode started\n");
		ret = WiFiNetworkJoin(CONFIG_DM_AP_SSID, strlen(CONFIG_DM_AP_SSID), NULL, get_security_config(CONFIG_DM_AP_SECURITY, CONFIG_DM_AP_PASS));
		sleep(1);
		if (ret == SLSI_STATUS_SUCCESS) {
			printf("[AutoConnect]Start doJoin with SSID %s\n", CONFIG_DM_AP_SSID);
			return 1;
		} else {
			printf("[AutoConnect]Failed to join the network.[%d]\n", ret);
		}
		return -1;
	} else {
		printf("[AutoConnect]Failed to start STA mode\n");
	}
	return -1;
}

static void wifiAutoConnectDeInit()
{
	isConnected = 0;
	dm_conn_unregister_linkup_cb(linkUpEvent);
}

static void wifiAutoConnect()
{
	int ret;
	if ((ret = wifiAutoConnectInit()) == 1) {
		int waitTime = 10;
		while (waitTime--) {
			if (isConnected == 1) {
				printf("[AutoConnect]WiFi Connected!\n");
				int dhcp_ret = app_dhcp_main();
				if (dhcp_ret == -1) {
					printf("Timeout fail to get ip address\n");
					return dhcp_ret;
				} else if (dhcp_ret == 0) {
					printf("No network interface\n");
					return dhcp_ret;
				}
				printf("DHCP done\n");
				break;
			}
			sleep(1);
		}
		if (waitTime <= 0) {
			printf("[AutoConnect]WiFi is not working. Test Canceled\n");
			return 0;
		}
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
		wifiAutoConnect();
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
			if (opt >= argc) {
				app_print_usage();
				return 0;
			}
			bootstrap_requested = true;
			strncpy(dm_context.server_info.ipAddress, \
					argv[opt], strlen(argv[opt]));
			break;
		case 't':
			opt++;
			if (opt >= argc) {
				app_print_usage();
				return 0;
			}
			if (1 != sscanf(argv[opt], "%d", \
							&(dm_context.client_info.lifetime))) {
				app_print_usage();
				return 0;
			}
			break;

		case 's':
			opt++;
			if (opt >= argc) {
				app_print_usage();
				return 0;
			}
			strncpy(dm_context.server_info.ipAddress, \
					argv[opt], strlen(argv[opt]));
			break;

		case 'c':
			prv_wakaama_start();
			return 0;
#ifdef WITH_TINYDTLS
		case 'i':
			opt++;
			if (opt >= argc) {
				app_print_usage();
				return 0;
			}
			break;
		case 'S':
			opt++;
			if (opt >= argc) {
				app_print_usage();
				return 0;
			}
			break;
#endif
		case 'n': {
			char argbuffer[20];
			int val = 7;
			opt++;
			if (opt >= argc) {
				app_print_usage();
				return 0;
			}

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
			if (opt >= argc) {
				app_print_usage();
				return 0;
			}
			break;
		case 'p':
			opt++;
			if (opt >= argc) {
				app_print_usage();
				return 0;
			}
			strncpy(wakaama_param.serverport, argv[opt], strlen(argv[opt]));
			break;
		case 'g': {
			char ipAddress[IPADDRLEN_MAX];
			int ret;

			if (opt >= argc) {
				app_print_usage();
				return 0;
			}
			ret = dm_lwm2m_get_server_address(ipAddress);
			if (ret == DM_ERROR_NONE) {
				printf("Server ip address is %s\n", ipAddress);
			} else {
				printf("Error reading server ip address\n");
			}
			return 0;
		}
		case 'h': {
			char *server_port[PORTLEN];
			int ret;

			if (opt >= argc) {
				app_print_usage();
				return 0;
			}
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
			if (opt >= argc) {
				app_print_usage();
				return 0;
			}
			printf("Read all resource of an object instance %s\n", argv[opt]);
			memset(argbuffer, 0x00, 20);
			strncpy(argbuffer, argv[opt], strlen(argv[opt]));
			if (dm_lwm2m_display_client_resource(argbuffer) != DM_ERROR_NONE) {
				printf("object dump did not work\n");
			}
			return 0;
		}
		case 'r':
			if (opt >= argc) {
				app_print_usage();
				return 0;
			}
			printf("Read a resource value\n");
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
