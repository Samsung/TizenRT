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

#ifndef _OICQUEUE_H_
#define _OICQUEUE_H_

#include <pthread.h>

#include "things_node.h"

typedef struct queue_s {
	things_node_s *head;
	things_node_s *tail;

	void (*push)(struct queue_s *, void *, int);	// add item to tail
	// get item from head and remove it from queue
	void *(*pop)(struct queue_s *, int *);
	// get item from head but keep it in queue
	void *(*peek)(struct queue_s *, int *);

	int (*length)(struct queue_s *);
	// Remove node
	// size of this queue
	int size;

	pthread_mutex_t q_mutex;	// = PTHREAD_MUTEX_INITIALIZER;
} queue_s;

#endif							// _OICQUEUE_H_
