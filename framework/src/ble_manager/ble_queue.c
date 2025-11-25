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
#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "ble_queue.h"

#define BLE_EVT_TASK_STACK_SIZE 4096
#define BLE_QUEUE_CHECK do { if (g_q_grp == NULL) {return BLE_QUEUE_NOT_INIT;}} while(0)

static ble_queue_group *g_q_grp = NULL;
static int g_run_process = 0;

static void *_ble_evt_handler(void)
{
	int priority;
	int err_no;
	ble_queue *q;

	while (g_run_process) {
		if (sem_wait(&g_q_grp->countsem) < 0) {
			err_no = get_errno();
			if (err_no == EINTR) {
				continue;
			} else {
				printf("[BLEQUEUE] Event Handler stopped (errno = %d)\n", err_no);
				g_run_process = 0;
				break;
			}
		}
		for (priority = 0; priority < BLE_QUEUE_EVT_PRI_MAX; priority++) {
			q = &g_q_grp->q[priority];
			if (q->queue == NULL) {
				continue;
			}
			if (sem_trywait(&q->countsem) < 0) {
				err_no = get_errno();
				if (err_no == EDEADLK || err_no == EINVAL) {
					g_run_process = 0;
					break;
				}
			} else {
				if (q->read_index != q->write_index) {
					/* This Priority Queue is not Empty */
					if (g_q_grp->caller) {
						g_q_grp->caller(priority, q->queue + (q->data_size * q->read_index));
					}
					q->read_index = (q->read_index + 1) % q->size;
				}
				break;
			}
		}
	}

	return 0;
}

ble_queue_ret_e ble_queue_init(queue_event_caller caller)
{
	if (caller == NULL) {
		return BLE_QUEUE_INVALID_ARGS;
	}

	if (g_q_grp == NULL) {
		g_q_grp = (ble_queue_group *)calloc(1, sizeof(ble_queue_group));
		if (g_q_grp == NULL) {
			return BLE_QUEUE_MEM_ALLOC_FAIL;
		}
		g_q_grp->caller = caller;
		g_run_process = 1;

		int ret;
		pthread_attr_t attr;
		/* Initialize the attribute variable */
		if ((ret = pthread_attr_init(&attr)) != 0) {
			printf("[BLEQUEUE] pthread_attr_init failed, status=%d\n", __func__, ret);
		}

		/* set a stacksize */
		if ((ret = pthread_attr_setstacksize(&attr, BLE_EVT_TASK_STACK_SIZE)) != 0) {
			printf("[BLEQUEUE] pthread_attr_setstacksize failed, status=%d\n", __func__, ret);
		}

		sem_init(&g_q_grp->countsem, 0, 0);
		ret = pthread_create(&g_q_grp->thread, &attr, (pthread_startroutine_t)_ble_evt_handler, NULL);
		if (ret < 0) {
			printf("[BLEQUEUE] create task fail\n");
			sem_destroy(&g_q_grp->countsem);
			g_q_grp->caller = NULL;
			g_run_process = 0;
			free(g_q_grp);
			g_q_grp = NULL;
			return BLE_QUEUE_FAIL;
		}
		pthread_setname_np(g_q_grp->thread, "ble_evt_handler");
	}
	return BLE_QUEUE_SUCCESS;
}

ble_queue_ret_e ble_queue_deinit(void)
{
	BLE_QUEUE_CHECK;

	int i;
	ble_queue_ret_e ret = BLE_QUEUE_SUCCESS;
	if (g_q_grp != NULL) {
		g_q_grp->caller = NULL;
		g_run_process = 0;
		sem_post(&g_q_grp->countsem);
		pthread_join(g_q_grp->thread, NULL);
		sem_destroy(&g_q_grp->countsem);
		for (i = 0; i < BLE_QUEUE_EVT_PRI_MAX; i++) {
			ble_queue *q = &g_q_grp->q[i];
			if (q->queue != NULL) {
				free(q->queue);
				q->queue = NULL;
			}
		}
		free(g_q_grp);
		g_q_grp = NULL;
	}
	return ret;
}

ble_queue_ret_e ble_queue_pri_set(int priority, int queue_size, int data_size) {
	BLE_QUEUE_CHECK;

	if (priority < 0 || priority >= BLE_QUEUE_EVT_PRI_MAX) {
		return BLE_QUEUE_INVALID_ARGS;
	}

	ble_queue *q = &g_q_grp->q[priority];

	if (q->queue == NULL) {
		void *temp = (void *)malloc(data_size * queue_size);
		if (temp == NULL) {
			printf("[BLEQUEUE] fail to set priority queue[%d]\n", priority);
			return BLE_QUEUE_MEM_ALLOC_FAIL;
		}
		sem_init(&q->countsem, 0, 0);
		q->grp_count = &g_q_grp->countsem;
		q->read_index = 0;
		q->write_index = 0;
		q->data_size = data_size;
		q->size = queue_size;
		q->queue = temp;

		return BLE_QUEUE_SUCCESS;
	} 

	return BLE_QUEUE_ALREADY_SET;
}

ble_queue_ret_e ble_queue_enque(int priority, void *data)
{
	BLE_QUEUE_CHECK;

	ble_queue *q = &g_q_grp->q[priority];

	if (q->queue == NULL) {
		return BLE_QUEUE_NOT_INIT;
	}

	int write_index_next = (q->write_index + 1) % q->size;
	if (write_index_next == q->read_index) {
		/* 
		Queue is Full
		- This functions is related to interrupt callbacks.
		  If any logs are printed in this line, they come up very fast and cannot check other logs.
		*/
		return BLE_QUEUE_FULL;
	}

	memcpy(q->queue + (q->data_size * q->write_index), data, q->data_size);
	q->write_index = write_index_next;

	sem_post(&q->countsem);
	sem_post(q->grp_count);

	return BLE_QUEUE_SUCCESS;
}

ble_queue *ble_queue_get_pri_queue(int priority)
{
	if (g_q_grp == NULL) {
		return NULL;
	}

	return &g_q_grp->q[priority];
}
