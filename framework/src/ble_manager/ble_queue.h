/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
#pragma once

#include <tinyara/config.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

typedef enum {
	BLE_QUEUE_SUCCESS = 0,
	BLE_QUEUE_FAIL,
	BLE_QUEUE_NOT_INIT,
	BLE_QUEUE_MEM_ALLOC_FAIL,
	BLE_QUEUE_INVALID_ARGS,
	BLE_QUEUE_ALREADY_SET,
	BLE_QUEUE_EMPTY,
	BLE_QUEUE_FULL,
} ble_queue_ret_e;

/* Event Queue Priority
 *
 * 0 : High - general event(noti, connect, disconnect)
 * 1 : Low  - scan result event
 */
typedef enum {
	BLE_QUEUE_EVT_PRI_HIGH = 0,
	BLE_QUEUE_EVT_PRI_LOW,
	BLE_QUEUE_EVT_PRI_MAX,
} ble_event_priority_e;

typedef struct {
	int size;
	volatile int write_index;
	volatile int read_index;
	sem_t countsem;
	sem_t *grp_count;
	void *queue;
	int data_size;
} ble_queue;

typedef void (*queue_event_caller)(int, void *);
typedef struct {
	ble_queue q[BLE_QUEUE_EVT_PRI_MAX];
	sem_t countsem;
	queue_event_caller caller;
	pthread_t thread;
} ble_queue_group;

ble_queue_ret_e ble_queue_init(queue_event_caller caller);
ble_queue_ret_e ble_queue_deinit(void);
ble_queue_ret_e ble_queue_pri_set(int priority, int queue_size, int data_size);
ble_queue_ret_e ble_queue_enque(int priority, void *data);
ble_queue *ble_queue_get_pri_queue(int priority);
