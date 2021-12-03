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
#pragma once
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <semaphore.h>

#define WO_CONN_FAIL 1
#define WO_CONN_SUCCESS 2
#define WT_SEM_TRY_WAIT_US 10000

#define WM_SEM_POST(sem)                \
	do {                                \
		while (sem_post(&sem) != 0) {   \
			usleep(WT_SEM_TRY_WAIT_US); \
		}                               \
	} while (0)

#define WM_SEM_WAIT(sem)                             \
	do {                                             \
		while (sem_wait(&sem) != 0) {                \
			if (errno == EINTR) {                    \
				continue;                            \
			}                                        \
			WT_LOGE(TAG, "sem wait fail %d", errno); \
		}                                            \
	} while (0)

#define WO_TEST_SIGNAL(conn, queue)                                 \
	do {                                                            \
		WT_LOG(TAG, "send signal\t %s:%d", __FUNCTION__, __LINE__); \
		int ssres = wo_add_queue(conn, queue);                      \
		if (ssres != 0) {                                           \
			assert(0);                                              \
		}                                                           \
		WM_SEM_POST(queue->signal);                                 \
	} while (0)

#define WO_TEST_WAIT(conn, queue)                                   \
	do {                                                            \
		WT_LOG(TAG, "wait signal\t %s:%d", __FUNCTION__, __LINE__); \
		WM_SEM_WAIT(queue->signal);                                 \
		int swres = wo_dequeue(&conn, queue);                       \
		if (swres != 0) {                                           \
			assert(0);                                              \
		}                                                           \
	} while (0)

struct wo_queue {
	sem_t lock;
	sem_t signal;
	int queue[10];
	int front;
	int rear;
};

#define MAX_INTEROP_SECURITY_LEN 10
#ifndef CONFIG_EXAMPLES_WIFIMANAGER_AP_LIST_ITEMS_COUNT
#define MAX_INTEROP_AP_CONFIG_COUNT 10
#else
#define MAX_INTEROP_AP_CONFIG_COUNT CONFIG_EXAMPLES_WIFIMANAGER_AP_LIST_ITEMS_COUNT
#endif

typedef struct {
	wifi_manager_ap_config_s ap_config;
	char security[MAX_INTEROP_SECURITY_LEN];
	int scan_result;
	int connect_result;
} interop_ap_info_s;

typedef struct {
	int ap_count;
	interop_ap_info_s ap_info[MAX_INTEROP_AP_CONFIG_COUNT];
} interop_ap_config_list_s;

int wo_add_queue(int conn, struct wo_queue *queue);
int wo_dequeue(int *conn, struct wo_queue *queue);
struct wo_queue *wo_create_queue(void);
void wo_destroy_queue(struct wo_queue *queue);

void wt_print_conninfo(wifi_manager_info_s *info);
void wt_print_stats(wifi_manager_stats_s *stats);
wifi_manager_ap_auth_type_e wt_get_auth_type(const char *method);
wifi_manager_ap_crypto_type_e wt_get_crypto_type(const char *method);
void wt_print_wifi_softap_profile(wifi_manager_softap_config_s *config, char *title);
void wt_print_stats(wifi_manager_stats_s *stats);
void wt_print_scanlist(wifi_manager_scan_info_s *slist);
void wt_print_wifi_ap_profile(wifi_manager_ap_config_s *config, char *title);
wifi_manager_ap_auth_type_e wt_get_autbh_type(const char *method);
wifi_manager_ap_crypto_type_e wt_get_crypto_type(const char *method);
void print_ap_config_list(interop_ap_config_list_s *ap_config_list);
void wm_get_apinfo(wifi_manager_ap_config_s *apconfig,
				   char *ssid,
				   char *pwd,
				   wifi_manager_ap_auth_type_e auth,
				   wifi_manager_ap_crypto_type_e crypto);
void wm_get_softapinfo(wifi_manager_softap_config_s *ap_config,
					   char *softap_ssid,
					   char *softap_password,
					   int softap_channel);
