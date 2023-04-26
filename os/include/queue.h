/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
 * include/queue.h
 *
 *   Copyright (C) 2007-2009 Gregory Nutt. All rights reserved.
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
/**
 * @defgroup QUEUE_LIBC QUEUE
 * @brief Provides APIs for Queue
 * @ingroup KERNEL
 *
 * @{
 */

/// @file queue.h
/// @brief Queue APIs

#ifndef __INCLUDE_QUEUE_H
#define __INCLUDE_QUEUE_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <sys/types.h>
#include <stddef.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define sq_init(q) do { (q)->head = NULL; (q)->tail = NULL; } while (0)
#define dq_init(q) do { (q)->head = NULL; (q)->tail = NULL; } while (0)

#define sq_move(q1,q2) \
  do \
    { \
      (q2)->head = (q1)->head; \
      (q2)->tail = (q1)->tail; \
      (q1)->head = NULL; \
      (q1)->tail = NULL; \
    } \
  while (0)

#define dq_move(q1,q2) \
  do \
    { \
      (q2)->head = (q1)->head; \
      (q2)->tail = (q1)->tail; \
      (q1)->head = NULL; \
      (q1)->tail = NULL; \
    } \
  while (0)

#define sq_next(p) ((p)->flink)
#define dq_next(p) ((p)->flink)
#define dq_prev(p) ((p)->blink)

#define sq_empty(q) ((q)->head == NULL)
#define dq_empty(q) ((q)->head == NULL)

#define sq_peek(q)  ((q)->head)
#define dq_peek(q)  ((q)->head)

#define dq_tail(q)  ((q)->tail)
#define sq_tail(q)  ((q)->tail)
/****************************************************************************
 * Global Type Declarations
 ****************************************************************************/

/**
 * @brief structure for singly-linked queue
 */
struct sq_entry_s {
	FAR struct sq_entry_s *flink;
};
typedef struct sq_entry_s sq_entry_t;

/**
 * @brief structure for double-linked queue
 */
struct dq_entry_s {
	FAR struct dq_entry_s *flink;
	FAR struct dq_entry_s *blink;
};
typedef struct dq_entry_s dq_entry_t;

/**
 * @brief structure for header queue
 */
struct sq_queue_s {
	FAR sq_entry_t *head;
	FAR sq_entry_t *tail;
};
typedef struct sq_queue_s sq_queue_t;

/**
 * @brief structure for header queue
 */
struct dq_queue_s {
	FAR dq_entry_t *head;
	FAR dq_entry_t *tail;
};
typedef struct dq_queue_s dq_queue_t;

/****************************************************************************
 * Global Function Prototypes
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/**
 * @brief places the 'node' at the head of the 'queue'
 * @details @b #include <queue.h>
 * @param[in] node node to be added
 * @param[in] queue queue
 * @return none
 * @since TizenRT v1.0
 */
void sq_addfirst(FAR sq_entry_t *node, FAR sq_queue_t *queue);
/**
 * @brief adds 'node' at the beginning of 'queue'
 * @details @b #include <queue.h>
 * @param[in] node node to be added
 * @param[in] queue queue
 * @return none
 * @since TizenRT v1.0
 */
void dq_addfirst(FAR dq_entry_t *node, FAR dq_queue_t *queue);
/**
 * @brief places the 'node' at the tail of the 'queue'
 * @details @b #include <queue.h>
 * @param[in] node node to be added
 * @param[in] queue queue
 * @return none
 * @since TizenRT v1.0
 */
void sq_addlast(FAR sq_entry_t *node, FAR sq_queue_t *queue);
/**
 * @brief adds 'node' to the end of 'queue'
 * @details @b #include <queue.h>
 * @param[in] node node to be added
 * @param[in] queue queue
 * @return none
 * @since TizenRT v1.0
 */
void dq_addlast(FAR dq_entry_t *node, FAR dq_queue_t *queue);
/**
 * @brief adds 'node' fter 'prev' in the 'queue.'
 * @details @b #include <queue.h>
 * @param[in] node node to be added
 * @param[in] queue queue
 * @return none
 * @since TizenRT v1.0
 */
void sq_addafter(FAR sq_entry_t *prev, FAR sq_entry_t *node, FAR sq_queue_t *queue);
/**
 * @brief adds 'node' after 'prev' in the 'queue.'
 * @details @b #include <queue.h>
 * @param[in] prev prev node
 * @param[in] node node to be added
 * @param[in] queue queue
 * @return none
 * @since TizenRT v1.0
 */
void dq_addafter(FAR dq_entry_t *prev, FAR dq_entry_t *node, FAR dq_queue_t *queue);

/* Combine queues */

void sq_cat(FAR sq_queue_t *queue1, FAR sq_queue_t *queue2);
void dq_cat(FAR dq_queue_t *queue1, FAR dq_queue_t *queue2);

/**
 * @brief adds 'node' before 'next' in 'queue'
 * @details @b #include <queue.h>
 * @param[in] next next node
 * @param[in] node node to be added
 * @param[in] queue queue
 * @return none
 * @since TizenRT v1.0
 */
void dq_addbefore(FAR dq_entry_t *next, FAR dq_entry_t *node, FAR dq_queue_t *queue);

/**
 * @brief  removes the entry following 'node
 * @details @b #include <queue.h>
 * @param[in] node node to be removed
 * @param[in] queue queue
 * @return a reference to the removed entry
 * @since TizenRT v1.0
 */
FAR sq_entry_t *sq_remafter(FAR sq_entry_t *node, FAR sq_queue_t *queue);
/**
 * @brief  removes a 'node' for 'queue.'
 * @details @b #include <queue.h>
 * @param[in] node node to be removed
 * @param[in] queue queue
 * @return void
 * @since TizenRT v1.0
 */
void sq_rem(FAR sq_entry_t *node, FAR sq_queue_t *queue);
/**
 * @brief  removes 'node' from 'queue'
 * @details @b #include <queue.h>
 * @param[in] node node to be removed
 * @param[in] queue queue
 * @return void
 * @since TizenRT v1.0
 */
void dq_rem(FAR dq_entry_t *node, FAR dq_queue_t *queue);
/**
 * @brief  Removes the last entry in a singly-linked queue.
 * @details @b #include <queue.h>
 * @param[in] queue singly-linked queue
 * @return node to be removed
 * @since TizenRT v1.0
 */
FAR sq_entry_t *sq_remlast(FAR sq_queue_t *queue);
/**
 * @brief  removes the last entry from 'queue'
 * @details @b #include <queue.h>
 * @param[in] queue singly-linked queue
 * @return node to be removed
 * @since TizenRT v1.0
 */
FAR dq_entry_t *dq_remlast(FAR dq_queue_t *queue);
/**
 * @brief  removes the first entry from 'queue'
 * @details @b #include <queue.h>
 * @param[in] queue singly-linked queue
 * @return node to be removed
 * @since TizenRT v1.0
 */
FAR sq_entry_t *sq_remfirst(FAR sq_queue_t *queue);
/**
 * @brief  removes 'node' from the head of 'queue'
 * @details @b #include <queue.h>
 * @param[in] queue singly-linked queue
 * @return node to be removed
 * @since TizenRT v1.0
 */
FAR dq_entry_t *dq_remfirst(FAR dq_queue_t *queue);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* __INCLUDE_QUEUE_H_ */
/**
 * @}
 */
