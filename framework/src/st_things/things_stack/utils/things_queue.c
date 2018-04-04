/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "things_queue.h"
#include "logging/things_logger.h"
#include "utils/things_malloc.h"

//#include "oicrequest.h"

#define TAG "[things_queue]"

/**
 * Push an item into queue, if this is the first item,
 * both queue->head and queue->tail will point to it,
 * otherwise the oldtail->next and tail will point to it.
 */
void enQueue(queue_s *queue, void *item, int req)
{

	if (queue == NULL || item == NULL) {
		return;
	}

	pthread_mutex_lock(&(queue->q_mutex));
	// Create a new node
	things_node_s *n = (things_node_s *) things_malloc(sizeof(things_node_s));
	if (NULL == n) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, THINGS_MEMORY_ERROR);
		return;
	}
	n->item = item;
	n->req = req;
	n->next = NULL;

	if (queue->head == NULL) {	// no head
		queue->head = n;
	} else {
		queue->tail->next = n;
	}
	queue->tail = n;
	queue->size++;

	pthread_mutex_unlock(&(queue->q_mutex));
}

/**
 * Return and remove the first item.
 */
void *deQueue(queue_s *queue, int *req)
{

	if (queue == NULL || queue->head == NULL) {
		return NULL;
	}

	pthread_mutex_lock(&(queue->q_mutex));

	// get the first item
	things_node_s *head = queue->head;

	void *item = head->item;
	*req = head->req;
	// move head pointer to next node, decrease size
	queue->head = head->next;
	queue->size--;
	// free the memory of original head
	things_free(head);

	pthread_mutex_unlock(&(queue->q_mutex));

	return item;
}

/**
 * Return but not remove the first item.
 */
void *peek(queue_s *queue, int *req)
{

	pthread_mutex_lock(&(queue->q_mutex));

	things_node_s *head = queue->head;
	*req = head->req;

	pthread_mutex_unlock(&(queue->q_mutex));

	return head->item;
}

/**
 * Return but not remove the first item.
 */
int getLength(queue_s *queue)
{

	pthread_mutex_lock(&(queue->q_mutex));

	int length = queue->size;

	pthread_mutex_unlock(&(queue->q_mutex));

	return length;
}
