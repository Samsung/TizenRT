/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <errno.h>
#include <wifi_manager/wifi_manager.h>
#include "connect_test_utils.h"
#include "connect_test_log.h"

#define TAG "[CU]"

/* Supported security method */
static const char* g_wifi_test_auth_method[] = {
#ifdef CT_AUTH_TABLE
#undef CT_AUTH_TABLE
#endif
#define CT_AUTH_TABLE(type, str, desc) str,
#include "connect_test_auth_table.h"
};

static const char* g_wifi_test_crypto_method[] = {
#ifdef CT_CRYPTO_TABLE
#undef CT_CRYPTO_TABLE
#endif
#define CT_CRYPTO_TABLE(type, str, desc) str,
#include "connect_test_crypto_table.h"
};

static const wifi_manager_ap_crypto_type_e g_crypto_type_table[] = {
#ifdef CT_CRYPTO_TABLE
#undef CT_CRYPTO_TABLE
#endif
#define CT_CRYPTO_TABLE(type, str, desc) type,
#include "connect_test_crypto_table.h"
};

static const wifi_manager_ap_auth_type_e g_auth_type_table[] = {
#ifdef CT_AUTH_TABLE
#undef CT_AUTH_TABLE
#endif
#define CT_AUTH_TABLE(type, str, desc) type,
#include "connect_test_auth_table.h"
};

/* queue */
#define CT_QUEUE_SIZE 10
#define CT_QUEUE_LOCK(lock) \
	do {                    \
		sem_wait(lock);     \
	} while (0)
#define CT_QUEUE_UNLOCK(lock) \
	do {                      \
		sem_post(lock);       \
	} while (0)

int ct_is_empty(struct ct_queue* queue)
{
	if (queue->rear == queue->front) {
		return 1;
	}
	return 0;
}

int ct_is_full(struct ct_queue* queue)
{
	int tmp = (queue->front + 1) % CT_QUEUE_SIZE;
	if (tmp == queue->rear) {		
		return 1;
	}
	return 0;
}

int ct_add_queue(int conn, struct ct_queue* queue)
{
	CT_QUEUE_LOCK(&queue->lock);
	if (ct_is_full(queue)) {
		CT_QUEUE_UNLOCK(&queue->lock);
		return -1;
	}
	queue->front = (queue->front + 1) % CT_QUEUE_SIZE;	
	queue->queue[queue->front] = conn;
	CT_QUEUE_UNLOCK(&queue->lock);
	return 0;
}

int ct_dequeue(int* conn, struct ct_queue* queue)
{
	CT_QUEUE_LOCK(&queue->lock);
	if (ct_is_empty(queue)) {
		CT_QUEUE_UNLOCK(&queue->lock);
		return -1;
	}
	queue->rear = (queue->rear + 1) % CT_QUEUE_SIZE;	
	*conn = queue->queue[queue->rear];
	CT_QUEUE_UNLOCK(&queue->lock);
	return 0;
}

struct ct_queue* ct_create_queue(void)
{
	struct ct_queue* queue = (struct ct_queue*)malloc(sizeof(struct ct_queue));
	if (!queue) {
		return NULL;
	}
	int res = sem_init(&queue->lock, 0, 1);
	if (res < 0) {
		CT_LOGE(TAG, "fail to initialize lock %d", errno);
		free(queue);
		return NULL;
	}
	res = sem_init(&queue->signal, 0, 0);
	if (res < 0) {
		CT_LOGE(TAG, "fail to intiailize signal %d", errno);
		sem_destroy(&queue->lock);
		free(queue);
	}
	queue->front = -1;
	queue->rear = -1;
	return queue;
}

void ct_destroy_queue(struct ct_queue* queue)
{
	if (!queue) {
		return;
	}
	sem_destroy(&queue->lock);
	sem_destroy(&queue->signal);

	free(queue);
}

void ct_print_scanlist(wifi_manager_scan_info_s* slist, const char* ssid, bool* ap_on)
{	
	while (slist != NULL) {
		CT_LOG(TAG, "WiFi AP SSID: %-25s, BSSID: %-20s, Rssi: %d, Auth: %s, Crypto: %s\n",
			slist->ssid, slist->bssid, slist->rssi,
			g_wifi_test_auth_method[slist->ap_auth_type],
			g_wifi_test_crypto_method[slist->ap_crypto_type]);		
		if ((0 == strncmp(slist->ssid, ssid, sizeof(slist->ssid))) && ap_on) {
			*ap_on = true;
			CT_LOG(TAG, "AP_got_ON\n");
		}
		slist = slist->next;
	}
}

wifi_manager_ap_auth_type_e ct_get_auth_type(const char* method)
{
	int list_size = sizeof(g_wifi_test_auth_method) / sizeof(g_wifi_test_auth_method[0]);
	char* result[3];
	char* next_ptr;
	char data[20];
	strncpy(data, method, 20);
	result[0] = strtok_r(data, "_", &next_ptr);
	result[1] = strtok_r(NULL, "_", &next_ptr);
	result[2] = strtok_r(NULL, "_", &next_ptr);

	for (int i = 0; i < list_size; i++) {
		if ((strncmp(method, g_wifi_test_auth_method[i], strlen(method) + 1) == 0) || (result[0] && (strncmp(result[0], g_wifi_test_auth_method[i], strlen(method) + 1) == 0))) {
			if (result[2] != NULL) {				
				if (0 == strncmp(result[2], "ent", strlen(result[2])+1)) {
					return WIFI_MANAGER_AUTH_UNKNOWN;
				}
			}
			return g_auth_type_table[i];
		}
	}
	return WIFI_MANAGER_AUTH_UNKNOWN;
}

wifi_manager_ap_crypto_type_e ct_get_crypto_type(const char* method)
{
	char data[20];
	char* result[2];
	char* next_ptr;
	int list_size = sizeof(g_wifi_test_crypto_method) / sizeof(g_wifi_test_crypto_method[0]);

	strncpy(data, method, 20);
	result[0] = strtok_r(data, "_", &next_ptr);
	result[1] = next_ptr;

	for (int i = 0; i < list_size; i++) {
		if (strncmp(result[1], g_wifi_test_crypto_method[i], strlen(result[1]) + 1) == 0) {
			return g_crypto_type_table[i];
		}
	}
	return WIFI_MANAGER_CRYPTO_UNKNOWN;
}

void ct_get_apinfo(wifi_manager_ap_config_s* apconfig, char* ssid, char* pwd,
	wifi_manager_ap_auth_type_e auth, wifi_manager_ap_crypto_type_e crypto)
{
	strncpy(apconfig->ssid, ssid, strlen(ssid) + 1);
	apconfig->ssid_length = strlen(ssid);
	apconfig->ap_auth_type = auth;
	apconfig->ap_crypto_type = WIFI_MANAGER_CRYPTO_NONE;
	apconfig->passphrase_length = 0;
	if (auth != WIFI_MANAGER_AUTH_OPEN) {
		strncpy(apconfig->passphrase, pwd, strlen(pwd) + 1);
		apconfig->passphrase_length = strlen(pwd);
		apconfig->ap_crypto_type = crypto;
	}
}

void ct_get_softapinfo(wifi_manager_softap_config_s* ap_config, char* softap_ssid,
	char* softap_password, int softap_channel)
{
	strncpy(ap_config->ssid, softap_ssid, strlen(softap_ssid) + 1);
	strncpy(ap_config->passphrase, softap_password, strlen(softap_password) + 1);
	ap_config->channel = softap_channel;
}

int connect_AP(wifi_manager_ap_config_s apconfig, struct ct_queue* ct_queue, unsigned int time_interval)
{
	int conn = 0;
	wifi_manager_result_e wres = WIFI_MANAGER_SUCCESS;
	CT_LOG(TAG, "connect AP");
	int retry_connect = MAX_AP_CON_RETRY;

	while (retry_connect) {
		wres = wifi_manager_connect_ap(&apconfig);
		if (wres != WIFI_MANAGER_SUCCESS) {
			CT_LOGE(TAG, "connect AP fail %d", wres);
			CT_LOG(TAG, "add %d_ms sleep, gives time to driver to stabilise", time_interval);			
			usleep(time_interval);
		}
		else {
			break;
		}
		retry_connect--;
	}
	if (wres != WIFI_MANAGER_SUCCESS) {
		return -1;
	}

	/*  wait connect event */
	CT_LOG(TAG, "wait connect success event");	
	CT_TEST_WAIT(conn, ct_queue);
	return 0;
}

int get_device_mac(char* mac_buf, int len)
{
	char mac_address[6];
	if (netlib_getmacaddr("wlan0", mac_address) != 0) {
		CT_LOGE(TAG, "MAC Get Error");
		return -1;
	}
	snprintf((char*)mac_buf, len, "%02X%02X%02X%02X%02X%02X",
		mac_address[0], mac_address[1], mac_address[2],
		mac_address[3], mac_address[4], mac_address[5]);
	CT_LOG(TAG, "Device ID: %s", mac_buf);
	return 0;
}

int get_device_ip(char* ipv4_buf, int len)
{
	char ipv4_address[4];
	if (netlib_get_ipv4addr("wlan0", ipv4_address) != 0) {
		CT_LOGE(TAG, "IPv4 Get Error");
		return -1;
	}
	snprintf((char*)ipv4_buf, len, "%d.%d.%d.%d", ipv4_address[0], ipv4_address[1],
		ipv4_address[2], ipv4_address[3]);
	CT_LOG(TAG, "Device ip %s", ipv4_buf);
	return 0;
}