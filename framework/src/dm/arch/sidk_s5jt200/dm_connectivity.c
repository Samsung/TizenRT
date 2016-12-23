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

#include <apps/netutils/wifi/slsi_wifi_api.h>
#include <arpa/inet.h>

#define DM_GET_INTERFACE_INFO 0
#define DM_GET_IP_ADDR_INFO 1

#define DM_CALLBACK_BUFFER_SIZE 10

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
	if (ioctl(fd, SIOCGIFCONF, (void *)&ifcfg) < 0) {
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
	if (ioctl(fd, SIOCGIFCONF, (void *)&ifcfg) < 0) {
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
