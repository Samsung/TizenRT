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

#ifndef __MM_MM_HEAP_MM_NODE_H
#define __MM_MM_HEAP_MM_NODE_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <assert.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define REMOVE_NODE_FROM_LIST(node)				\
	do {							\
		DEBUGASSERT((node)->blink);			\
		(node)->blink->flink = (node)->flink;		\
		if ((node)->flink) {				\
			(node)->flink->blink = (node)->blink;	\
		}						\
	} while (0)

/****************************************************************************
 * Public Functions
 ****************************************************************************/

struct mm_heap_s; // Forward declaration
void mm_free_delaylist(FAR struct mm_heap_s *heap);
int mm_delayed_free_worker(int argc, char *argv[]);

#endif /* __MM_MM_HEAP_MM_NODE_H */
