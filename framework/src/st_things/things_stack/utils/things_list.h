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

#ifndef _THINGS_LIST_H_
#define _THINGS_LIST_H_

#include <sys/types.h>

#include "octypes.h"
#include "things_node.h"

typedef int (*key_compare)(void *src_key, const void *dest_key);

typedef struct list_s {
	things_node_s *head;
	things_node_s *tail;

	// size of this list
	int size;

	// begin
	things_node_s *(*begin)(struct list_s *);
	// pop
	void *(*pop)(struct list_s *p_list);
	// end
	things_node_s *(*end)(struct list_s *);
	// size
	int (*list_size)(struct list_s *);
	// front
	things_node_s *(*front)(struct list_s *);
	// insert
	void (*insert)(struct list_s *, void *);
	// next
	// Node*       (*get_next)             (oic_node_s*);
	// erase
	void *(*erase)(struct list_s *, void *);
	void *(*erase_by_key)(struct list_s *, key_compare, const void *key);
	// clear
	void (*clear)(struct list_s *);
	// find
	things_node_s *(*find_by_key)(struct list_s *, key_compare, const void *key);
	pthread_mutex_t q_mutex;	// = PTHREAD_MUTEX_INITIALIZER;
} list_s;

//-----------------------------------------------------------------------------
// Function prototype
//-----------------------------------------------------------------------------
/**
 * Create and initiate a Queue
 */
list_s *create_list(void);

void terminate_list(list_s *list);

#endif							// _OICLIST_H_
