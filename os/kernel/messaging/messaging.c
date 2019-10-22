/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
/****************************************************************************
 * kernel/messaging/messaging.c
 *
 *   Copyright (C) 2007-2009, 2011, 2014 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <debug.h>
#include <errno.h>
#include <string.h>
#include <queue.h>
#include <semaphore.h>
#include <tinyara/kmalloc.h>
#include <tinyara/mm/mm.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define MSG_SENDER_UNDEFINED (-1)

#define MSG_MAX_PORT_NAME 64

#define MSG_RECV_EXIST   0
#define MSG_RECV_NOEXIST 1

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/
struct msg_port_node_s {
	struct msg_port_node_s *flink;
	char port_name[MSG_MAX_PORT_NAME];
	pid_t sender_pid;
	int nreceiver;
	sem_t port_sem;
	sq_queue_t recv_node_list;
};
typedef struct msg_port_node_s msg_port_node_t;

struct msg_recv_node_s {
	struct msg_recv_node_s *flink;
	pid_t pid;
	int prio;
};
typedef struct msg_recv_node_s msg_recv_node_t;

static sem_t port_list_sem;

/****************************************************************************
 * Public Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables
 ****************************************************************************/
static sq_queue_t g_port_node_list;
static int curr_recv_cnt;;
/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int messaging_append_receiver(pid_t pid, int prio, sq_queue_t *queue)
{
	msg_recv_node_t *recv_node;
	msg_recv_node_t *prev_node;
	msg_recv_node_t *next_node;
	recv_node = (msg_recv_node_t *)kmm_malloc(sizeof(msg_recv_node_t));
	if (recv_node == NULL) {
		msgdbg("[Messaging] fail to save receiver info : out of memory.\n");
		return ERROR;
	}
	recv_node->pid = pid;
	recv_node->prio = prio;

	/* Append recv node by decreasing order. */
	next_node = (msg_recv_node_t *)sq_peek(queue);
	prev_node = next_node;
	if (next_node == NULL) {
		/* The queue is empty, so add new recv node. */
		sq_addlast((FAR sq_entry_t *)recv_node, queue);
		return OK;
	}

	while (next_node != NULL) {
		if (next_node->prio > recv_node->prio) {
			prev_node = next_node;
			next_node = (msg_recv_node_t *)sq_next(next_node);
			continue;
		}
		break;
	}

	if (next_node == prev_node) {
		/* In case recv_node's priority higher than queue->head */
		sq_addfirst((FAR sq_entry_t *)recv_node, (FAR sq_queue_t *)queue);
	} else if (next_node == NULL) {
		/* In case recv_node's priority lower than queue->tail */
		sq_addlast((FAR sq_entry_t *)recv_node, (FAR sq_queue_t *)queue);
	} else {
		/* In case recv_node's priority higher than prev_node and lower than next_node */
		sq_addafter((FAR sq_entry_t *)prev_node, (FAR sq_entry_t *)recv_node, (FAR sq_queue_t *)queue);
	}

	return OK;
}

static int messaging_check_recv_exist(int recv_pid, sq_queue_t *queue)
{
	msg_recv_node_t *recv_node;
	recv_node = (msg_recv_node_t *)sq_peek(queue);
	while (recv_node != NULL) {
		if (recv_node->pid == recv_pid) {
			return MSG_RECV_EXIST;
		}
		recv_node = (msg_recv_node_t *)sq_next(recv_node);
	}

	return MSG_RECV_NOEXIST;
}
/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: messaging_save_receiver
 *
 * Description:
 *   Save(append) receiver information to the message port list.
 *
 * Parameters:
 *   port_name - A message port name
 *   recv_pid  - A pid of receiver
 *   recv_prio - A priority of receiver
 *
 * Return Value:
 *   OK on success, ERROR on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/
int messaging_save_receiver(char *port_name, pid_t recv_pid, int recv_prio)
{
	int ret;
	msg_port_node_t *port_node;

	port_node = (msg_port_node_t *)sq_peek(&g_port_node_list);
	while (port_node != NULL) {
		if (strncmp(port_node->port_name, port_name, strlen(port_name) + 1) == 0) {
			ret = messaging_check_recv_exist(recv_pid, &port_node->recv_node_list);
			if (ret == MSG_RECV_EXIST) {
				return OK;
			}
			port_node->nreceiver++;
			/* There was already same port node in the list, append recv node to this list. */
			sem_wait(&port_node->port_sem);
			ret = messaging_append_receiver(recv_pid, recv_prio, &port_node->recv_node_list);
			sem_post(&port_node->port_sem);
			return ret;
		}
		port_node = (msg_port_node_t *)sq_next(port_node);
	}

	/* Create new port node which has this port name */
	port_node = (msg_port_node_t *)kmm_malloc(sizeof(msg_port_node_t));
	if (port_node == NULL) {
		msgdbg("[Messaging] fail to save receiver info : out of memory.\n");
		return ERROR;
	}

	/* Fill the port node information except sender_pid. */
	strncpy(port_node->port_name, port_name, strlen(port_name) + 1);
	port_node->sender_pid = MSG_SENDER_UNDEFINED;
	port_node->nreceiver = 1;
	sem_init(&port_node->port_sem, 0, 1);
	sq_init(&port_node->recv_node_list);
	sem_wait(&port_list_sem);
	sq_addlast((FAR sq_entry_t *)port_node, &g_port_node_list);
	sem_post(&port_list_sem);

	/* Append recv node to new created port node. */
	sem_wait(&port_node->port_sem);
	ret = messaging_append_receiver(recv_pid, recv_prio, &port_node->recv_node_list);
	sem_post(&port_node->port_sem);

	return ret;
}

/****************************************************************************
 * Name: messaging_read_list
 *
 * Description:
 *   Check that how many receivers are waiting in the list.
 *
 * Parameters:
 *   port_name - A message port name
 *
 * Return Value:
 *   Return the number of receivers who wait the port on success.
 *   Return ERROR on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/
int messaging_read_list(char *port_name, int *recv_arr, int *total_cnt)
{
	int recv_idx;
	msg_port_node_t *port_node;
	msg_recv_node_t *recv_node;
	int recv_cnt;

	port_node = (msg_port_node_t *)sq_peek(&g_port_node_list);
	while (port_node != NULL) {
		if (strncmp(port_node->port_name, port_name, strlen(port_name) + 1) == 0) {
			if (port_node->nreceiver > 0) {
				*total_cnt = port_node->nreceiver;

				recv_node = (msg_recv_node_t *)sq_peek(&port_node->recv_node_list);
				if (recv_node == NULL) {
					curr_recv_cnt = 0;
					msgdbg("[Messaging] fail to read receivers list.\n");
					return ERROR;
				}

				/* Ignore already read information. */
				for (recv_idx = 0; recv_idx < curr_recv_cnt; recv_idx++) {
					recv_node = (msg_recv_node_t *)sq_next(recv_node);
				}

				/* Read receivers' information. */
				for (recv_idx = 0; recv_idx < CONFIG_MESSAGING_RECV_LIST_SIZE; recv_idx++) {
					recv_arr[recv_idx] = recv_node->pid;
					curr_recv_cnt++;
					recv_node = (msg_recv_node_t *)sq_next(recv_node);
					if (recv_node == NULL) {
						recv_cnt = curr_recv_cnt;
						curr_recv_cnt = 0;
						return recv_cnt;
					}
				}
			}
			return curr_recv_cnt;
		}
		port_node = (msg_port_node_t *)sq_next(port_node);
	}

	return ERROR;
}

/****************************************************************************
 * Name: messaging_remove_recv_node
 *
 * Description:
 *   Remove the recv_node which this task made
 *
 * Parameters:
 *   recv_node_list - A list which appended to port_node
 *
 * Return Value:
 *   OK on success, ERROR on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/
static int messaging_remove_recv_node(sq_queue_t *recv_node_list)
{
	msg_recv_node_t *recv_node;
	pid_t my_pid = getpid();

	recv_node = (msg_recv_node_t *)sq_peek(recv_node_list);
	while (recv_node != NULL) {
		if (recv_node->pid == my_pid) {
			sq_rem((sq_entry_t *)recv_node, recv_node_list);
			kmm_free(recv_node);
			return OK;
		}
		recv_node = (msg_recv_node_t *)sq_next(recv_node);
	}
	return ERROR;
}

/****************************************************************************
 * Name: messaging_remove_list
 *
 * Description:
 *   Remove the receivers' list
 *
 * Parameters:
 *   port_name - A message port name
 *
 * Return Value:
 *   OK on success, ERROR on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/
int messaging_remove_list(char *port_name)
{
	int ret = ERROR;
	msg_port_node_t *port_node;

	port_node = (msg_port_node_t *)sq_peek(&g_port_node_list);
	while (port_node != NULL) {
		if (strncmp(port_node->port_name, port_name, strlen(port_name) + 1) == 0) {
			/* Remove the whole recv node which attached to this port node. */
			sem_wait(&port_node->port_sem);
			ret = messaging_remove_recv_node(&port_node->recv_node_list);
			if (ret == OK) {
				port_node->nreceiver--;
			}
			sem_post(&port_node->port_sem);

			if (port_node->nreceiver == 0) {
				sem_wait(&port_list_sem);
				(void)sq_rem((FAR sq_entry_t *)port_node, &g_port_node_list);
				sem_post(&port_list_sem);
				kmm_free(port_node);
			}
			ret = OK;
			break;
		}
		port_node = (msg_port_node_t *)sq_next(port_node);
	}

	if (port_node == NULL) {
		/* If port_node is NULL, there is no information for removing. */
		ret = OK;
	}

	return ret;
}

void messaging_initialize(void)
{
	/* Initialize a sempahore for port list */

	sem_init(&port_list_sem, 0, 1);
}
