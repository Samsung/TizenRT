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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <errno.h>
#include <wifi_manager/wifi_manager.h>
#include "wm_test_utils.h"
#include "wm_test_log.h"

#define TAG "[WU]"

/* Supported security method */
static const char *g_wifi_test_auth_method[] = {
#ifdef WT_AUTH_TABLE
#undef WT_AUTH_TABLE
#endif
#define WT_AUTH_TABLE(type, str, desc) str,
#include "wm_test_auth_table.h"
};

static const wifi_manager_ap_crypto_type_e g_crypto_type_table[] = {
#ifdef WT_CRYPTO_TABLE
#undef WT_CRYPTO_TABLE
#endif
#define WT_CRYPTO_TABLE(type, str, desc) type,
#include "wm_test_crypto_table.h"
};

static const wifi_manager_ap_auth_type_e g_auth_type_table[] = {
#ifdef WT_AUTH_TABLE
#undef WT_AUTH_TABLE
#endif
#define WT_AUTH_TABLE(type, str, desc) type,
#include "wm_test_auth_table.h"
};

static const char *g_wifi_test_crypto_method[] = {
#ifdef WT_CRYPTO_TABLE
#undef WT_CRYPTO_TABLE
#endif
#define WT_CRYPTO_TABLE(type, str, desc) str,
#include "wm_test_crypto_table.h"
};

/* queue */
#define WO_QUEUE_SIZE 10
#define WO_QUEUE_LOCK(lock) \
	do {                    \
		sem_wait(lock);     \
	} while (0)
#define WO_QUEUE_UNLOCK(lock) \
	do {                      \
		sem_post(lock);       \
	} while (0)

int wo_is_empty(struct wo_queue *queue)
{
	if (queue->rear == queue->front) {
		return 1;
	}
	return 0;
}

int wo_is_full(struct wo_queue *queue)
{
	int tmp = (queue->front + 1) % WO_QUEUE_SIZE;
	if (tmp == queue->rear) {
		return 1;
	}
	return 0;
}

int wo_add_queue(int conn, struct wo_queue *queue)
{
	WO_QUEUE_LOCK(&queue->lock);
	if (wo_is_full(queue)) {
		WO_QUEUE_UNLOCK(&queue->lock);
		return -1;
	}
	queue->front = (queue->front + 1) % WO_QUEUE_SIZE;
	queue->queue[queue->front] = conn;
	WO_QUEUE_UNLOCK(&queue->lock);

	return 0;
}

int wo_dequeue(int *conn, struct wo_queue *queue)
{
	WO_QUEUE_LOCK(&queue->lock);
	if (wo_is_empty(queue)) {
		WO_QUEUE_UNLOCK(&queue->lock);
		return -1;
	}
	queue->rear = (queue->rear + 1) % WO_QUEUE_SIZE;
	*conn = queue->queue[queue->rear];
	WO_QUEUE_UNLOCK(&queue->lock);

	return 0;
}

struct wo_queue *wo_create_queue(void)
{
	struct wo_queue *queue = (struct wo_queue *)malloc(sizeof(struct wo_queue));
	if (!queue) {
		return NULL;
	}
	int res = sem_init(&queue->lock, 0, 1);
	if (res < 0) {
		WT_LOGE(TAG, "fail to initialize lock %d", errno);
		free(queue);
		return NULL;
	}
	res = sem_init(&queue->signal, 0, 0);
	if (res < 0) {
		WT_LOGE(TAG, "fail to intiailize signal %d", errno);
		sem_destroy(&queue->lock);
		free(queue);
	}
	queue->front = -1;
	queue->rear = -1;

	return queue;
}

void wo_destroy_queue(struct wo_queue *queue)
{
	if (!queue) {
		return;
	}
	sem_destroy(&queue->lock);
	sem_destroy(&queue->signal);

	free(queue);
}

void wt_print_conninfo(wifi_manager_info_s *info)
{
	WT_LOGP(TAG, "==============================================\n");
	if (info->mode == SOFTAP_MODE) {
		WT_LOGP(TAG, "MODE: softap\n");
		WT_LOGP(TAG, "SSID: %s\n", info->ssid);
	} else if (info->mode == STA_MODE) {
		if (info->status == AP_CONNECTED) {
			WT_LOGP(TAG, "MODE: station (connected)\n");
			WT_LOGP(TAG, "SSID: %s\n", info->ssid);
			WT_LOGP(TAG, "rssi: %d\n", info->rssi);
		} else if (info->status == AP_DISCONNECTED) {
			WT_LOGP(TAG, "MODE: station (disconnected)\n");
		}
	} else {
		WT_LOGP(TAG, "STATE: NONE\n");
	}
	WT_LOGP(TAG, "==============================================\n");
}

void wt_print_stats(wifi_manager_stats_s *stats)
{
	WT_LOGP(TAG, "=======================================================================\n");
	WT_LOGP(TAG, "CONN    CONNFAIL    DISCONN    RECONN    SCAN    SOFTAP    JOIN    LEFT\n");
	WT_LOGP(TAG, "%-8d%-12d%-11d%-10d\n", stats->connect, stats->connectfail, stats->disconnect, stats->reconnect);
	WT_LOGP(TAG, "%-8d%-10d%-8d%-8d\n", stats->scan, stats->softap, stats->joined, stats->left);
	WT_LOGP(TAG, "Period %ld:%ld ~ %ld:%ld\n", stats->start.tv_sec, stats->start.tv_usec,
			stats->end.tv_sec, stats->end.tv_usec);
	WT_LOGP(TAG, "retransmit %d\n", stats->tx_retransmit);
	WT_LOGP(TAG, "TX drop %d\n", stats->tx_drop);
	WT_LOGP(TAG, "RX drop %d\n", stats->rx_drop);
	WT_LOGP(TAG, "TX success(count) %d\n", stats->tx_success_cnt);
	WT_LOGP(TAG, "TX success(bytes) %d\n", stats->tx_success_bytes);
	WT_LOGP(TAG, "RX counts %d\n", stats->rx_cnt);
	WT_LOGP(TAG, "RX bytes %d\n", stats->rx_bytes);
	WT_LOGP(TAG, "TX tries %d\n", stats->tx_try);
	WT_LOGP(TAG, "RSSI avg %d\n", stats->rssi_avg);
	WT_LOGP(TAG, "RSSI min %d\n", stats->rssi_min);
	WT_LOGP(TAG, "RSSI max %d\n", stats->rssi_max);
	WT_LOGP(TAG, "BEACON missed(counts) %d\n", stats->beacon_miss_cnt);
	WT_LOGP(TAG, "=======================================================================\n");
}

void wt_print_wifi_ap_profile(wifi_manager_ap_config_s *config, char *title)
{
	WT_LOGP(TAG, "====================================\n");
	if (title) {
		WT_LOGP(TAG, "%s\n", title);
	}
	WT_LOGP(TAG, "------------------------------------\n");
	if (config->ssid_length > 0) {
		WT_LOGP(TAG, "SSID: %s\n", config->ssid);
	}
	if (config->ap_auth_type == WIFI_MANAGER_AUTH_UNKNOWN || config->ap_crypto_type == WIFI_MANAGER_CRYPTO_UNKNOWN) {
		WT_LOGP(TAG, "SECURITY: unknown\n");
	} else {
		char security_type[21] = {
			0,
		};
		strncat(security_type, g_wifi_test_auth_method[config->ap_auth_type],
				strlen(g_wifi_test_auth_method[config->ap_auth_type]));
		wifi_manager_ap_auth_type_e tmp_type = config->ap_auth_type;
		if (tmp_type == WIFI_MANAGER_AUTH_OPEN || tmp_type == WIFI_MANAGER_AUTH_IBSS_OPEN || tmp_type == WIFI_MANAGER_AUTH_WEP_SHARED) {
			WT_LOGP(TAG, "SECURITY: %s\n", security_type);
		} else {
			strncat(security_type, "_", strlen("_") + 1);
			strncat(security_type, g_wifi_test_crypto_method[config->ap_crypto_type],
					strlen(g_wifi_test_crypto_method[config->ap_crypto_type]));
			WT_LOGP(TAG, "SECURITY: %s\n", security_type);
		}
	}
	WT_LOGP(TAG, "====================================\n");
}

wifi_manager_ap_auth_type_e wt_get_auth_type(const char *method)
{
	int list_size = sizeof(g_wifi_test_auth_method) / sizeof(g_wifi_test_auth_method[0]);
	char *result[3];
	char *next_ptr;
	char data[20];
	strncpy(data, method, 20);
	result[0] = strtok_r(data, "_", &next_ptr);
	result[1] = strtok_r(NULL, "_", &next_ptr);
	result[2] = strtok_r(NULL, "_", &next_ptr);

	for (int i = 0; i < list_size; i++) {
		if ((strncmp(method, g_wifi_test_auth_method[i], strlen(method) + 1) == 0) || (result[0] && (strncmp(result[0], g_wifi_test_auth_method[i], strlen(method) + 1) == 0))) {
			if (result[2] != NULL) {
				if (strcmp(result[2], "ent") == 0) {
					return WIFI_MANAGER_AUTH_UNKNOWN;
				}
			}
			return g_auth_type_table[i];
		}
	}
	return WIFI_MANAGER_AUTH_UNKNOWN;
}

wifi_manager_ap_crypto_type_e wt_get_crypto_type(const char *method)
{
	char data[20];
	char *result[2];
	char *next_ptr;
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

void wt_print_wifi_softap_profile(wifi_manager_softap_config_s *config, char *title)
{
	WT_LOGP(TAG, "====================================\n");
	if (title) {
		WT_LOGP(TAG, "%s\n", title);
	}
	WT_LOGP(TAG, "------------------------------------\n");
	WT_LOGP(TAG, "SSID: %s\n", config->ssid);
	WT_LOGP(TAG, "channel: %d\n", config->channel);
	WT_LOGP(TAG, "====================================\n");
}

void wt_print_scanlist(wifi_manager_scan_info_s *slist)
{
	while (slist != NULL) {
		WT_LOGP(TAG, "WiFi AP SSID: %-25s, BSSID: %-20s, Rssi: %d, Auth: %s, Crypto: %s\n",
				slist->ssid, slist->bssid, slist->rssi,
				g_wifi_test_auth_method[slist->ap_auth_type],
				g_wifi_test_crypto_method[slist->ap_crypto_type]);
		slist = slist->next;
	}
}

void print_ap_config_list(interop_ap_config_list_s *ap_config_list)
{
	interop_ap_info_s *ap_info = NULL;

	WT_LOG(TAG, "AP config from file ");
	WT_LOG(TAG, "=============================================================");
	for (int i = 0; i < ap_config_list->ap_count; i++) {
		ap_info = ap_config_list->ap_info + i;
		WT_LOG(TAG, "ssid[%s] passphrase[%s] auth_type[%d], crypto_type[%d]",
			   ap_info->ap_config.ssid, ap_info->ap_config.passphrase,
			   ap_info->ap_config.ap_auth_type, ap_info->ap_config.ap_crypto_type);
	}
	WT_LOG(TAG, "==============================================================");
}

void wm_get_apinfo(wifi_manager_ap_config_s *apconfig,
				   char *ssid,
				   char *pwd,
				   wifi_manager_ap_auth_type_e auth,
				   wifi_manager_ap_crypto_type_e crypto)
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

void wm_get_softapinfo(wifi_manager_softap_config_s *ap_config,
					   char *softap_ssid,
					   char *softap_password,
					   int softap_channel)
{
	strncpy(ap_config->ssid, softap_ssid, strlen(softap_ssid) + 1);
	strncpy(ap_config->passphrase, softap_password, strlen(softap_password) + 1);
	ap_config->channel = softap_channel;
}
