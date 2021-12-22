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
#pragma once
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <semaphore.h>

#define CT_CONN_FAIL 1
#define CT_CONN_SUCCESS 2
#define CT_SEM_TRY_WAIT_US 10000
#define MAX_AP_CON_RETRY 5

#define CT_SEM_POST(sem)                \
	do {                                \
		while (sem_post(&sem) != 0) {   \
			usleep(CT_SEM_TRY_WAIT_US); \
		}                               \
	} while (0)

#define CT_SEM_WAIT(sem)                             \
	do {                                             \
		while (sem_wait(&sem) != 0) {                \
			if (errno == EINTR) {                    \
				continue;                            \
			}                                        \
			CT_LOGE(TAG, "sem wait fail %d", errno); \
		}                                            \
	} while (0)

#define CT_TEST_SIGNAL(conn, queue)                                 \
	do {                                                            \
		CT_LOG(TAG, "send signal\t %s:%d", __FUNCTION__, __LINE__); \
		int ssres = ct_add_queue(conn, queue);                      \
		if (ssres != 0) {                                           \			
			assert(0);                                              \
		}                                                           \
		CT_SEM_POST(queue->signal);                                 \
	} while (0)

#define CT_TEST_WAIT(conn, queue)                                   \
	do {                                                            \
		CT_LOG(TAG, "wait signal\t %s:%d", __FUNCTION__, __LINE__); \
		CT_SEM_WAIT(queue->signal);                                 \
		int swres = ct_dequeue(&conn, queue);                       \
		if (swres != 0) {                                           \			
			assert(0);                                              \
		}                                                           \
	} while (0)

struct ct_queue {
	sem_t lock;
	sem_t signal;
	int queue[10];
	int front;
	int rear;	
};

int ct_add_queue(int conn, struct ct_queue* queue);
int ct_dequeue(int* conn, struct ct_queue* queue);
struct ct_queue* ct_create_queue(void);
void ct_destroy_queue(struct ct_queue* queue);
void ct_print_scanlist(wifi_manager_scan_info_s* slist, const char* ssid, bool* ap_on);
void ct_get_apinfo(wifi_manager_ap_config_s* apconfig, char* ssid, char* pwd,
	wifi_manager_ap_auth_type_e auth, wifi_manager_ap_crypto_type_e crypto);
void ct_get_softapinfo(wifi_manager_softap_config_s* ap_config, char* softap_ssid,
	char* softap_password, int softap_channel);
int connect_AP(wifi_manager_ap_config_s apconfig, struct ct_queue* ct_queue, unsigned int time_delay);
int getmac(char* mac_buf, int len);