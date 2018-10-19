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

/**
 * @file dm_connectivity.c
 * @brief DM Connectivity APIs
 */
#include <tinyara/config.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <debug.h>
#include <errno.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ether.h>

#include <dm/dm_error.h>
#include <dm/dm_connectivity.h>

#include <protocols/dhcpc.h>
#include <slsi_wifi/slsi_wifi_api.h>
#include <slsi_wifi/slsi_wifi_utils.h>
#include <arpa/inet.h>

#define DM_GET_INTERFACE_INFO 0
#define DM_GET_IP_ADDR_INFO 1

#define DM_CALLBACK_BUFFER_SIZE 10
#define DM_BSSID_LEN            18
#ifdef CONFIG_WIFI_MANAGER
#define DM_NET_DEVNAME	CONFIG_WIFIMGR_STA_IFNAME
#else
#define DM_NET_DEVNAME "wl1"
#endif

/* Static data structure to hold results of a WiFi scan.
 */
static slsi_scan_info_t *g_wifi_scan_result;
static slsi_scan_info_t *g_wifi_scan_iter;

/* WiFi connection status variable */
static uint8_t g_dm_connected;

static sem_t g_dm_link_evt;

static conn_cb dm_link_up_cb_arr[DM_CALLBACK_BUFFER_SIZE];
static conn_cb dm_link_down_cb_arr[DM_CALLBACK_BUFFER_SIZE];

void dm_cb_register_init(void);
static void dm_cb_linkUpEvent(slsi_reason_t *reason);
static void dm_cb_linkDownEvent(slsi_reason_t *reason);

static void dm_cb_linkUpEvent(slsi_reason_t *reason)
{
	int i;
	dmdbg("linkUpEvent : connected\n");
	sem_wait(&g_dm_link_evt);
	for (i = 0; i < DM_CALLBACK_BUFFER_SIZE; i++) {
		if (dm_link_up_cb_arr[i] == NULL)
			continue;
		dm_link_up_cb_arr[i]();
	}
	sem_post(&g_dm_link_evt);
}

static void dm_cb_linkDownEvent(slsi_reason_t *reason)
{
	int i;
	dmdbg("linkDownEvent : disconnected\n");
	sem_wait(&g_dm_link_evt);
	for (i = 0; i < DM_CALLBACK_BUFFER_SIZE; i++) {
		if (dm_link_down_cb_arr[i] == NULL)
			continue;
		dm_link_down_cb_arr[i]();
	}
	sem_post(&g_dm_link_evt);
}

void dm_cb_register_init()
{
	int i;

	sem_init(&g_dm_link_evt, 0, 1);
	dmdbg("dm registering\n");
	if (WiFiRegisterLinkCallback(&dm_cb_linkUpEvent, &dm_cb_linkDownEvent) != SLSI_STATUS_SUCCESS)
		dmdbg("Failed to register WiFiRegisterLinkCallback\n");
	sem_wait(&g_dm_link_evt);
	for (i = 0; i < DM_CALLBACK_BUFFER_SIZE; i++) {
		dm_link_up_cb_arr[i] = NULL;
		dm_link_down_cb_arr[i] = NULL;
	}
	sem_post(&g_dm_link_evt);
}

int dm_conn_get_rssi(int *rssi)
{
	int8_t ret;
	int8_t val;

	if (rssi == NULL) {
		dmdbg("Null Parameter\n");
		return DM_ERROR_INVALID_PARAMETER;
	}

	if ((ret = WiFiGetRssi(&val)) != SLSI_STATUS_SUCCESS) {
		dmdbg("dm_log : ret[%d]\n", ret);
		return DM_ERROR_NO_DATA;
	}
	*rssi = (int)val;
	return DM_ERROR_NONE;
}

int dm_conn_get_channel(int *channel)
{
	int8_t ret;
	int8_t val;

	if (channel == NULL) {
		dmdbg("Null Parameter\n");
		return DM_ERROR_INVALID_PARAMETER;
	}

	if ((ret = WiFiGetChannel(&val)) != SLSI_STATUS_SUCCESS) {
		dmdbg("dm_log : ret[%d]\n", ret);
		return DM_ERROR_NO_DATA;
	}
	*channel = (int)val;
	return DM_ERROR_NONE;
}

int dm_conn_get_tx_power(int *dbm)
{
	int8_t ret;
	uint8_t val;

	if (dbm == NULL) {
		dmdbg("Null Parameter\n");
		return DM_ERROR_INVALID_PARAMETER;
	}

	if ((ret = WiFiGetTxPower(&val)) != SLSI_STATUS_SUCCESS) {
		dmdbg("dm_log : ret[%d]\n", ret);
		return DM_ERROR_NO_DATA;
	}
	*dbm = (int)val;
	return DM_ERROR_NONE;
}

int dm_conn_set_tx_power(const int *dbm)
{
	int8_t ret;
	uint8_t val;

	if (dbm == NULL) {
		dmdbg("Null Parameter\n");
		return DM_ERROR_INVALID_PARAMETER;
	}

	val = (uint8_t)(*dbm);
	if ((ret = WiFiSetTxPower(&val)) != SLSI_STATUS_SUCCESS) {
		dmdbg("dm_log : ret[%d]\n", ret);
		return DM_ERROR_INVALID_PARAMETER;
	}
	return DM_ERROR_NONE;
}

int dm_conn_get_address(char *ipAddr)
{
	if (ipAddr == NULL)
		return DM_ERROR_INVALID_PARAMETER;

	struct ifreq *ifr;
	struct sockaddr_in *sin;
	struct ifconf ifcfg;
	int fd;
	int i;
	int numreqs = 3;
	int num_nic = 0;
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	memset(&ifcfg, 0, sizeof(ifcfg));
	ifcfg.ifc_buf = NULL;
	ifcfg.ifc_len = sizeof(struct ifreq) * numreqs;
	ifcfg.ifc_buf = malloc(ifcfg.ifc_len);
	if (ioctl(fd, SIOCGIFCONF, (unsigned long)&ifcfg) < 0) {
		perror("SIOCGIFCONF ");
		goto DONE;
	}
	num_nic = ifcfg.ifc_len / sizeof(struct ifreq);
	for (i = 0, ifr = ifcfg.ifc_req; i < num_nic; ifr++, i++) {
		dmdbg("%s\t", ifr->ifr_name);
		sin = (struct sockaddr_in *)&ifr->ifr_addr;
		if ((sin->sin_addr.s_addr) == INADDR_LOOPBACK) {
			dmdbg("Loop Back\t");
			continue;
		} else {
			strcpy(ipAddr, inet_ntoa(sin->sin_addr));
			dmdbg("ipAddr : %s\n", ipAddr);
			break;
		}
	}

	return DM_ERROR_NONE;

	DONE: free(ifcfg.ifc_buf);
	return DM_ERROR_NO_DATA;
}

int dm_conn_get_interface(char *interface)
{
	if (interface == NULL) {
		dmdbg("Null Parameter\n");
		return DM_ERROR_INVALID_PARAMETER;
	}

	struct ifreq *ifr;
	struct sockaddr_in *sin;
	struct ifconf ifcfg;
	int fd;
	int i;
	int numreqs = 3;
	int num_nic = 0;
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	memset(&ifcfg, 0, sizeof(ifcfg));
	ifcfg.ifc_buf = NULL;
	ifcfg.ifc_len = sizeof(struct ifreq) * numreqs;
	ifcfg.ifc_buf = malloc(ifcfg.ifc_len);
	if (ioctl(fd, SIOCGIFCONF, (unsigned long)&ifcfg) < 0) {
		perror("SIOCGIFCONF ");
		goto DONE;
	}
	num_nic = ifcfg.ifc_len / sizeof(struct ifreq);

	for (i = 0, ifr = ifcfg.ifc_req; i < num_nic; ifr++, i++) {
		dmdbg("%s\t", ifr->ifr_name);
		sin = (struct sockaddr_in *)&ifr->ifr_addr;
		if ((sin->sin_addr.s_addr) == INADDR_LOOPBACK) {
			dmdbg("Loop Back\t");
			continue;
		} else {
			strncpy(interface, ifr->ifr_name, IF_NAMESIZE);
			dmdbg("Interface : %s\n", interface);
			break;
		}
	}

	return DM_ERROR_NONE;

	DONE: free(ifcfg.ifc_buf);
	return DM_ERROR_NO_DATA;
}

int dm_conn_register_linkup_cb(conn_cb cb)
{
	int i;
	int flag;

	if (cb == NULL) {
		dmdbg("Null Parameter\n");
		return DM_ERROR_INVALID_PARAMETER;
	}

	sem_wait(&g_dm_link_evt);
	flag = 0;
	for (i = 0; i < DM_CALLBACK_BUFFER_SIZE; i++) {
		if (dm_link_up_cb_arr[i] == cb) {
			flag = 1;
			break;
		}
	}
	sem_post(&g_dm_link_evt);

	if (flag == 1) {
		dmdbg("Already Registered Link Up Callback\n");
		return DM_ERROR_NONE;
	}

	sem_wait(&g_dm_link_evt);
	flag = 0;
	for (i = 0; i < DM_CALLBACK_BUFFER_SIZE; i++) {
		if (dm_link_up_cb_arr[i] == NULL) {
			flag = 1;
			dm_link_up_cb_arr[i] = cb;
			break;
		}
	}
	sem_post(&g_dm_link_evt);

	if (flag == 0) {
		dmdbg("Link Up Callback Buffer Overflow.\n");
		return DM_ERROR_INVALID_PARAMETER;
	}

	dmdbg("Link Up Callback Registered Successfully\n");
	return DM_ERROR_NONE;
}

int dm_conn_register_linkdown_cb(conn_cb cb)
{
	int i;
	int flag;

	if (cb == NULL) {
		dmdbg("Null Parameter\n");
		return DM_ERROR_INVALID_PARAMETER;
	}

	sem_wait(&g_dm_link_evt);
	flag = 0;
	for (i = 0; i < DM_CALLBACK_BUFFER_SIZE; i++) {
		if (dm_link_down_cb_arr[i] == cb) {
			flag = 1;
			break;
		}
	}
	sem_post(&g_dm_link_evt);

	if (flag == 1) {
		dmdbg("Already Registered Link Down Callback\n");
		return DM_ERROR_NONE;
	}

	sem_wait(&g_dm_link_evt);
	flag = 0;
	for (i = 0; i < DM_CALLBACK_BUFFER_SIZE; i++) {
		if (dm_link_down_cb_arr[i] == NULL) {
			flag = 1;
			dm_link_down_cb_arr[i] = cb;
			break;
		}
	}
	sem_post(&g_dm_link_evt);

	if (flag == 0) {
		dmdbg("Link Down Callback Buffer Overflow.\n");
		return DM_ERROR_INVALID_PARAMETER;
	}

	dmdbg("Link Down Callback Registered Successfully\n");
	return DM_ERROR_NONE;
}

int dm_conn_unregister_linkup_cb(conn_cb cb)
{
	int i;

	if (cb == NULL) {
		dmdbg("Null Parameter\n");
		return DM_ERROR_INVALID_PARAMETER;
	}

	sem_wait(&g_dm_link_evt);
	for (i = 0; i < DM_CALLBACK_BUFFER_SIZE; i++) {
		if (dm_link_up_cb_arr[i] == cb)
			dm_link_up_cb_arr[i] = NULL;
	}
	sem_post(&g_dm_link_evt);

	dmdbg("Link Up Callback UnRegistered Successfully\n");
	return DM_ERROR_NONE;
}

int dm_conn_unregister_linkdown_cb(conn_cb cb)
{
	int i;

	if (cb == NULL) {
		dmdbg("Null Parameter\n");
		return DM_ERROR_INVALID_PARAMETER;
	}

	sem_wait(&g_dm_link_evt);
	for (i = 0; i < DM_CALLBACK_BUFFER_SIZE; i++) {
		if (dm_link_down_cb_arr[i] == cb)
			dm_link_down_cb_arr[i] = NULL;
	}
	sem_post(&g_dm_link_evt);

	dmdbg("Link Down Callback UnRegistered Successfully\n");
	return DM_ERROR_NONE;
}

static int8_t prv_dm_conn_scan_result(slsi_reason_t *reason)
{
	if (reason->reason_code != 0) {
		dmdbg("Failed to scan : reason %d, locally generated : %d\n", \
			  reason->reason_code, reason->locally_generated);
		g_wifi_scan_result = NULL;
	} else {
		WiFiFreeScanResults(&g_wifi_scan_result);
		WiFiGetScanResults(&g_wifi_scan_result);
		WiFiNetworkJoin((uint8_t *)CONFIG_DM_AP_SSID, strlen(CONFIG_DM_AP_SSID), \
						NULL, (slsi_security_config_t *)getSecurityConfig(CONFIG_DM_AP_SECURITY, \
								CONFIG_DM_AP_PASS, SLSI_WIFI_STATION_IF));
	}
	printf("scanned result\n");
	return SLSI_STATUS_SUCCESS;
}

int dm_conn_get_scan_result(dm_scan_info_t **result)
{
	dm_scan_info_t *curr_record;
	dm_scan_info_t *prev_record = NULL;
	if (g_wifi_scan_result == NULL) {
		return DM_ERROR_NO_DATA;
	}
	/* Initialize pointer */
	g_wifi_scan_iter = g_wifi_scan_result;

	do {
		curr_record = (dm_scan_info_t *)malloc(sizeof(dm_scan_info_t));
		if (curr_record == NULL) {
			dm_conn_free_scan_result(result);
			return DM_ERROR_UNKNOWN;
		}
		if (!prev_record) {
			*result = curr_record;
		} else {
			prev_record->next = curr_record;
		}

		memset(curr_record->ssid, 0x00, SLSI_SSID_LEN + 1);
		memset(curr_record->bssid, 0x00, DM_BSSID_LEN);
		curr_record->rssi = g_wifi_scan_iter->rssi;
		strncpy(curr_record->ssid, (char *)g_wifi_scan_iter->ssid, strlen((const  char*)g_wifi_scan_iter->ssid));
		strncpy(curr_record->bssid, (char *)g_wifi_scan_iter->bssid, strlen((const  char*)g_wifi_scan_iter->bssid));
		prev_record = curr_record;
		g_wifi_scan_iter = g_wifi_scan_iter->next;
	} while (g_wifi_scan_iter != NULL);

	curr_record->next = NULL;

	return DM_ERROR_NONE;
}

int dm_conn_free_scan_result(dm_scan_info_t **result)
{
	dm_scan_info_t *curr_record;
	dm_scan_info_t *next_record;

	curr_record = *result;
	while (curr_record != NULL) {
		next_record = curr_record->next;
		free(curr_record);
		curr_record = next_record;
	}

	*result = NULL;
	return DM_ERROR_NONE;
}


int dm_conn_wifi_scan(void)
{
	if (WiFiScanNetwork() == SLSI_STATUS_SUCCESS) {
		WiFiFreeScanResults(&g_wifi_scan_result);
		WiFiGetScanResults(&g_wifi_scan_result);
		WiFiNetworkJoin((uint8_t *)CONFIG_DM_AP_SSID, strlen(CONFIG_DM_AP_SSID), \
						NULL, (slsi_security_config_t *)getSecurityConfig(CONFIG_DM_AP_SECURITY, \
								CONFIG_DM_AP_PASS, SLSI_WIFI_STATION_IF));
		return DM_ERROR_NONE;
	}
	return DM_ERROR_UNKNOWN;
}

int dm_conn_wifi_connect(conn_cb linkUpEvent, conn_cb linkDownEvent)
{
	uint8_t ret;
	uint8_t result;

	if (g_dm_connected == 1) {
		dmdbg("Already, Init!\n");
		return DM_ERROR_NONE;
	}

	dm_conn_register_linkup_cb(linkUpEvent);
	dm_conn_register_linkdown_cb(linkDownEvent);

	g_dm_connected = 0;
	ret = WiFiIsConnected(&result, NULL);
	if (ret == SLSI_STATUS_SUCCESS) {
		dmdbg("Wi-Fi status - Already Connected : %d\n", result);
		g_dm_connected = 1;
		return 1;
	} else if (ret != SLSI_STATUS_NOT_STARTED)
		return DM_ERROR_UNKNOWN;


	ret = WiFiStart(SLSI_WIFI_STATION_IF, NULL);
	if (ret == SLSI_STATUS_SUCCESS) {
		dmdbg("STA mode started\n");
		ret = WiFiNetworkJoin((uint8_t *)CONFIG_DM_AP_SSID, strlen(CONFIG_DM_AP_SSID), \
							  NULL, (slsi_security_config_t *)getSecurityConfig(CONFIG_DM_AP_SECURITY, CONFIG_DM_AP_PASS, SLSI_WIFI_STATION_IF));
		sleep(1);
		if (ret == SLSI_STATUS_SUCCESS) {
			dmdbg("Start doJoin with SSID %s\n", CONFIG_DM_AP_SSID);
			return DM_ERROR_NONE;
		}
		return DM_ERROR_UNKNOWN;
	}
	return DM_ERROR_UNKNOWN;
}

int dm_conn_wifi_disconnect(void)
{
	g_dm_connected = 0;
	return DM_ERROR_NONE;
}

static int prv_dm_conn_dhcp(void)
{
	int ret;
	struct in_addr ip_check;

	ret = dhcp_client_start(DM_NET_DEVNAME);
	if (ret != OK) {
		return -1;
	}

	ret = netlib_get_ipv4addr(DM_NET_DEVNAME, &ip_check);
	if (ret != OK) {
		return -1;
	}
	printf("IP address get %s ----\n", inet_ntoa(ip_check));
	return 1;

}

int dm_conn_dhcp_init(void)
{
	uint8_t wifi_status;
	int8_t ret;

	if (g_dm_connected == -1) {
		dmdbg("First, Do Conn Init!\n");
		return DM_ERROR_UNKNOWN;
	}

	if (g_dm_connected == 0) {
		int waitTime = 10;
		while (waitTime--) {
			ret = WiFiIsConnected(&wifi_status, NULL);
			dmdbg("retcode connect %d\n", ret);
			if (ret != SLSI_STATUS_SUCCESS) {
				dmdbg("Failed to WiFiIsConnected\n");
				return DM_ERROR_UNKNOWN;
			}

			if (wifi_status > 0) {
				g_dm_connected = 1;
				dmdbg("WiFi Connected!\n");
				int dhcp_ret = prv_dm_conn_dhcp();
				if (dhcp_ret == -1) {
					dmdbg("Timeout fail to get ip address\n");
					return DM_ERROR_TIMED_OUT;
				} else if (dhcp_ret == 0) {
					dmdbg("No network interface\n");
					return DM_ERROR_NOT_SUPPORTED;
				}
				dmdbg("DHCP done\n");
				break;
			}
			sleep(1);
		}
		if (waitTime <= 0) {
			dmdbg("WiFi is not working.\n");
			return DM_ERROR_UNKNOWN;
		}
	}

	return DM_ERROR_NONE;
}
