/******************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <semaphore.h>
#include <pthread.h>
#include <errno.h>
#include <sched.h>
#include <debug.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <tinyara/arch.h>
#include <tinyara/clock.h>
#include "esp_attr.h"
#include "esp32_queue_api.h"
#include "mq_tryreceive.h"
/************************************************************************
 * Pre-processor Definitions
 ************************************************************************/
#define MAX_QUEUE_INFO 10
#define NAME_LEN 20

/************************************************************************
 * Private Type Declarations
 ************************************************************************/
enum {
	NORMAL = 0,
	MIDDLE,
	HIGH,
} queue_prio_e;

typedef struct {
    bool valid;
    uint32_t mq_item_size;
    mqd_t mqd_fd_send;
    mqd_t mqd_fd_recv;
}queue_info_t;

/************************************************************************
 * Public Variables
 ************************************************************************/
const char *mq_name = "mq_wifi";
static queue_info_t queues_info[MAX_QUEUE_INFO];

/************************************************************************
 * Private Functions
 ************************************************************************/

/************************************************************************
 * Public Functions
 ************************************************************************/
/****************************************************************************
 * Name: calc_abs_time
 *
 * Description:
 *	This function used to calculate wall time based delay ticks.
 *
 * Inputs:
 *	abs_time - output.
 *	delayticks - input, delayed system ticks.
 *
 * Return:
 *	Returns the pionter of the created mqueue, but NULL in a failure.
 *
 ****************************************************************************/

int calc_abs_time(struct timespec *abs_time, int delayticks)
{
    int ret; 
    time_t sec; 
    uint32_t nsec;
    int offset = TICK2MSEC(delayticks);
    sec = offset / MSEC_PER_SEC;
    nsec = (offset - MSEC_PER_SEC * sec) * NSEC_PER_MSEC;

    ret = clock_gettime(CLOCK_REALTIME, abs_time);
    if (ret != 0) { 
        return ERROR;
    }    
    abs_time->tv_sec += sec; 
    abs_time->tv_nsec += nsec;
    if (abs_time->tv_nsec >= NSEC_PER_SEC) {
        abs_time->tv_sec++;
        abs_time->tv_nsec -= NSEC_PER_SEC;
    }    
    return OK;
}


/****************************************************************************
 * Name: queue_create_wrapper
 *
 * Description:
 *	This function is the wrapper of mq_open to establish a connection between a named
 * message queue and the calling task.
 *
 * Inputs:
 *	queue_len - The length of the queue.
 *	item_size - The size of each node of the queue.
 *
 * Return:
 *	Returns the pionter of the created mqueue, but NULL in a failure.
 *
 ****************************************************************************/
void *IRAM_ATTR queue_create_wrapper(uint32_t queue_len, uint32_t item_size)
{
	char name[NAME_LEN];
	bool flag = false;
	uint32_t mq_id = 0;
    int i;
    
	for (i = 0; i < MAX_QUEUE_INFO; i++) {
		if (!queues_info[i].valid) {
			flag = true;
			mq_id = i;
			break;
		}
	}
	if (!flag) {
		return NULL;
	}

	struct mq_attr attr;
	attr.mq_maxmsg = queue_len;
	attr.mq_msgsize = item_size;
	attr.mq_flags = 0;
	snprintf(name, NAME_LEN, "%s%d", mq_name, mq_id);
	/*Invalid param */
	queues_info[mq_id].mqd_fd_send = mq_open(name, O_RDWR | O_CREAT, 0666, &attr);
	if (queues_info[mq_id].mqd_fd_send == (mqd_t)ERROR) {
		return NULL;
	}
	queues_info[mq_id].mqd_fd_recv = mq_open(name, O_RDWR | O_CREAT, 0666, &attr);
	if (queues_info[mq_id].mqd_fd_recv == (mqd_t)ERROR) {
        mq_close(queues_info[mq_id].mqd_fd_send);
		return NULL;
	}
	queues_info[mq_id].valid = true;
	queues_info[mq_id].mq_item_size = item_size;
	return &queues_info[mq_id];
}

/****************************************************************************
 * Name: queue_delete_wrapper
 *
 * Description:
 *	This function is the wrapper of mq_close to close a connection between a named
 * message queue and the calling task.
 *
 * Inputs:
 *	queue - The queue to be closed.
 *
 * Return:
 *
 ****************************************************************************/
void IRAM_ATTR queue_delete_wrapper(void *queue)
{
	queue_info_t *queue_info = NULL;

	if (queue) {
		queue_info = (queue_info_t *)queue;
		if (queue_info->mqd_fd_send != (mqd_t)ERROR) {
			mq_close(queue_info->mqd_fd_send);
			queue_info->mqd_fd_send = NULL;
		}
		if (queue_info->mqd_fd_recv != (mqd_t)ERROR) {
			mq_close(queue_info->mqd_fd_recv);
			queue_info->mqd_fd_recv = NULL;
		}
		queue_info->mq_item_size = 0;
		queue_info->valid = false;
	}
	return;
}

int32_t IRAM_ATTR queue_send_wrapper(void *queue, void *item, uint32_t block_time_tick)
{
	int32_t ret;
	queue_info_t *queue_info = NULL;
	struct timespec abstime;

	if (!queue || !item) {
		return pdFAIL;
	}
	queue_info = (queue_info_t *)queue;
	if (queue_info->mqd_fd_send != (mqd_t)ERROR) {
		if (block_time_tick == 0xFFFFFFFF) {
			ret = mq_send(queue_info->mqd_fd_send, (char *)item, queue_info->mq_item_size, NORMAL);
			if (ret == ERROR) {
				return pdFAIL;
			}
		}
		else {
			calc_abs_time(&abstime, block_time_tick);
            ret = mq_timedsend(queue_info->mqd_fd_send, (char *)item, queue_info->mq_item_size, NORMAL, &abstime);
			if (ret == ERROR) {
				return pdFAIL;
			}
		}
	} else {
		return pdFAIL;
	}
	return pdPASS;;
}

/****************************************************************************
 * Name: queue_send_from_isr_wrapper
 *
 * Description:
 *	This function is the wrapper of mq_send to adds the specificied message (msg) to
 * the message queue (mqdes).
 *
 * Inputs:
 *	queue - The queue to send messages.
 *	item - The message to be send.
 *	hptw - useless in TizenRT, but needed in the declaration of esp32 wifi os adapter.
 * Return:
 *	Returns pdPASS if success, pdFAIL if failure.
 *
 ****************************************************************************/
int32_t IRAM_ATTR queue_send_from_isr_wrapper(void *queue, void *item, void *hptw)
{
	int32_t ret;
	queue_info_t *queue_info = NULL;

	if (!queue || !item) {
		return pdFAIL;
	}
	queue_info = (queue_info_t *)queue;
	if (queue_info->mqd_fd_send != (mqd_t)ERROR) {
		ret = mq_send(queue_info->mqd_fd_send, (char *)item, queue_info->mq_item_size, NORMAL);
		if (ret == ERROR) {
			return pdFAIL;
		}
		return pdPASS;
	}
	return pdFAIL;
}

/****************************************************************************
 * Name: queue_send_to_back_wrapper
 *
 * Description:
 *	This function is the wrapper of queue_send_wrapper to adds the specificied message (msg) to
 * the message queue (mqdes) end.
 *
 * Inputs:
 *	queue - The queue to send messages.
 *	item - The message to be send.
 *	block_time_tick - the time (in Ticks) to wait until a timeout is decleared
 * Return:
 *	Returns pdPASS if success, pdFAIL if failure.
 *
 ****************************************************************************/
int32_t IRAM_ATTR queue_send_to_back_wrapper(void *queue, void *item, uint32_t block_time_tick)
{
	if (!queue || !item) {
		return pdFAIL;
	}
	return queue_send_wrapper(queue, item, block_time_tick);
}

/****************************************************************************
 * Name: queue_send_to_front_wrapper
 *
 * Description:
 *	This function is the wrapper of mq_timedsend to adds the specificied message (msg) to
 * the message queue (mqdes) head.
 *
 * Inputs:
 *	queue - The queue to send messages.
 *	item - The message to be send.
 *	block_time_tick - the time (in Ticks) to wait until a timeout is decleared
 * Return:
 *	Returns pdPASS if success, pdFAIL if failure.
 *
 ****************************************************************************/
int32_t IRAM_ATTR queue_send_to_front_wrapper(void *queue, void *item, uint32_t block_time_tick)
{
	int32_t ret;
	queue_info_t *queue_info = NULL;
	struct timespec abstime;

	if (!queue || !item) {
		return pdFAIL;
	}
	queue_info = (queue_info_t *)queue;
	if (queue_info->mqd_fd_send != (mqd_t)ERROR) {
		if (block_time_tick == 0xFFFFFFFF) {
			ret = mq_send(queue_info->mqd_fd_send, (char *)item, queue_info->mq_item_size, NORMAL);
			if (ret == ERROR) {
				return pdFAIL;
			}
		} else {
            calc_abs_time(&abstime, block_time_tick);
			ret = mq_timedsend(queue_info->mqd_fd_send, (char *)item, queue_info->mq_item_size, HIGH, &abstime);
			if (ret == ERROR) {
				return pdFAIL;
			}
		}
	} else {
		return pdFAIL;
	}
	return pdPASS;
}

/****************************************************************************
 * Name: queue_recv_wrapper
 *
 * Description:
 *	This function is the wrapper of mq_timedreceive to receives the oldest of the highest priority
 *messages from the message queue.
 *
 * Inputs:
 *	queue - The queue to receive messages.
 *	item - The received message.
 *	block_time_tick - the time (in Ticks) to wait until a timeout is decleared
 * Return:
 *	Returns pdPASS if success, pdFAIL if failure.
 *
 ****************************************************************************/
int32_t IRAM_ATTR queue_recv_wrapper(void *queue, void *item, uint32_t block_time_tick)
{
	queue_info_t *queue_info = NULL;
	size_t msglen = 0;
	int prio = 0;
	int32_t ret;
	struct timespec abstime;

	if (!queue || !item) {
		return pdFAIL;
	}
    queue_info = (queue_info_t *)queue;
	if (queue_info->mqd_fd_recv != (mqd_t)ERROR) {
		msglen = queue_info->mq_item_size;
		if (block_time_tick == 0xFFFFFFFF) {
			ret = mq_receive(queue_info->mqd_fd_recv, (char *)item, msglen, &prio);
			if (ret == ERROR) {
				return pdFAIL;
			}
		} else {
            calc_abs_time(&abstime, block_time_tick); 
            ret = mq_timedreceive(queue_info->mqd_fd_recv, (char *)item, msglen, &prio, &abstime);
			if (ret == ERROR) {
				return pdFAIL;
			}
		}
	} else {
		return pdFAIL;
	}
	return pdPASS;
}

/****************************************************************************
 * Name: queue_recv_from_isr_wrapper
 *
 * Description:
 *	This function is the wrapper of mq_timedreceive to receives the oldest of the highest priority
 *messages from the message queue in an interrupt handle.
 *
 * Inputs:
 *	queue - The queue to receive messages.
 *	item - The received message.
 *	hptw - useless in TizenRT, but needed in the declaration of esp32 wifi os adapter.
 * Return:
 *	Returns pdPASS if success, pdFAIL if failure.
 *
 ****************************************************************************/
int32_t IRAM_ATTR queue_recv_from_isr_wrapper(void *queue, void *item, int32_t *const hptw)
{
	queue_info_t *queue_info = NULL;
	size_t msglen = 0;
	int prio = 0;
	int32_t ret;

	if (!queue || !item) {
		return pdFAIL;
	}
	queue_info = (queue_info_t *)queue;
	if (queue_info->mqd_fd_recv != (mqd_t)ERROR) {
		msglen = queue_info->mq_item_size;
		ret = mq_tryreceive_isr(queue_info->mqd_fd_recv, (char *)item, msglen, &prio);
		if (ret == ERROR) {
			return pdFAIL;
		}
		return pdPASS;
	}
	return pdFAIL;
}

/****************************************************************************
 * Name: queue_msg_waiting_wrapper
 *
 * Description:
 *	This function gets the message count of message queue.
 *
 * Inputs:
 *	queue - The queue to send messages.
 * Return:
 *	Returns the count of messages in the queue if success,  returns pdFAIL if failure.
 *
 ****************************************************************************/
uint32_t IRAM_ATTR queue_msg_waiting_wrapper(void *queue)
{
	queue_info_t *queue_info = NULL;
	if (queue) {
		queue_info = (queue_info_t *)queue;
		if ((queue_info->mqd_fd_send != (mqd_t)ERROR) && (queue_info->mqd_fd_send->msgq != NULL)) {
			return queue_info->mqd_fd_send->msgq->nmsgs;
		}
	}
	return pdFAIL;
}
