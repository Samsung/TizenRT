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
 *
 *   Copyright (C) 2014 Gregory Nutt. All rights reserved.
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

#ifndef _INCLUDE_NET_IOB_H
#define _INCLUDE_NET_IOB_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdint.h>
#include <stdbool.h>

#include <tinyara/net/netiob.h>

#ifdef CONFIG_NET_IOB

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration ************************************************************/

/* I/O buffer allocation logic supports a throttle value for the TCP
 * read-ahead buffering to prevent the read-ahead from consuming all
 * available I/O buffers.  This throttle only applies if both TCP write
 * buffering and TCP read-ahead buffering are enabled.
 */

#if !defined(CONFIG_NET_TCP_WRITE_BUFFERS) || !defined(CONFIG_NET_TCP_READAHEAD)
#undef CONFIG_NET_IOB_THROTTLE
#define CONFIG_NET_IOB_THROTTLE 0
#endif

/* The correct way to disable throttling is to the the throttle value to
 * zero.
 */

#if !defined(CONFIG_NET_IOB_THROTTLE)
#define CONFIG_NET_IOB_THROTTLE 0
#endif

/* Some I/O buffers should be allocated */

#if !defined(CONFIG_NET_IOB_NBUFFERS)
#warning CONFIG_NET_IOB_NBUFFERS not defined
#define CONFIG_NET_IOB_NBUFFERS 1
#define CONFIG_NET_IOB_BUFSIZE 1
#define CONFIG_NET_IOB_NCHAINS 1
#endif

#if CONFIG_NET_IOB_NBUFFERS < 1
#error CONFIG_NET_IOB_NBUFFERS is zero
#endif

#if CONFIG_NET_IOB_NBUFFERS <= CONFIG_NET_IOB_THROTTLE
#error CONFIG_NET_IOB_NBUFFERS <= CONFIG_NET_IOB_THROTTLE
#endif

/* IOB helpers */

#define IOB_DATA(p)      (&(p)->io_data[(p)->io_offset])
#define IOB_FREESPACE(p) (CONFIG_NET_IOB_BUFSIZE - (p)->io_len - (p)->io_offset)

#if CONFIG_NET_IOB_NCHAINS > 0
/* Queue helpers */

#define IOB_QINIT(q)   do { (q)->qh_head = 0; (q)->qh_tail = 0; } while (0)
#define IOB_QEMPTY(q)  ((q)->qh_head == NULL)
#endif

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* Represents one I/O buffer.  A packet is contained by one or more I/O
 * buffers in a chain.  The io_pktlen is only valid for the I/O buffer at
 * the head of the chain.
 */

struct netiob_s {
	/* Singly-link list support */

	FAR struct netiob_s *io_flink;

	/* Payload */

#if CONFIG_NET_IOB_BUFSIZE < 256
	uint8_t io_len;				/* Length of the data in the entry */
	uint8_t io_offset;			/* Data begins at this offset */
#else
	uint16_t io_len;			/* Length of the data in the entry */
	uint16_t io_offset;			/* Data begins at this offset */
#endif
	uint16_t io_pktlen;			/* Total length of the packet */

	uint8_t io_data[CONFIG_NET_IOB_BUFSIZE];
};

#if CONFIG_NET_IOB_NCHAINS > 0
/* This container structure supports queuing of I/O buffer chains.  This
 * structure is intended only for internal use by the IOB module.
 */

struct netiob_qentry_s {
	/* Singly-link list support */

	FAR struct netiob_qentry_s *qe_flink;

	/* Payload -- Head of the I/O buffer chain */

	FAR struct netiob_s *qe_head;
};

/* The I/O buffer queue head structure */

struct netiob_queue_s {
	/* Head of the I/O buffer chain list */

	FAR struct netiob_qentry_s *qh_head;
	FAR struct netiob_qentry_s *qh_tail;
};
#endif							/* CONFIG_NET_IOB_NCHAINS > 0 */

/****************************************************************************
 * Global Data
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: netiob_initialize
 *
 * Description:
 *   Set up the I/O buffers for normal operations.
 *
 ****************************************************************************/

void netiob_initialize(void);

/****************************************************************************
 * Name: netiob_alloc
 *
 * Description:
 *   Allocate an I/O buffer by taking the buffer at the head of the free list.
 *
 ****************************************************************************/

FAR struct netiob_s *netiob_alloc(bool throttled);

/****************************************************************************
 * Name: netiob_free
 *
 * Description:
 *   Free the I/O buffer at the head of a buffer chain returning it to the
 *   free list.  The link to  the next I/O buffer in the chain is return.
 *
 ****************************************************************************/

FAR struct netiob_s *netiob_free(FAR struct netiob_s *netiob);

/****************************************************************************
 * Name: netiob_free_chain
 *
 * Description:
 *   Free an entire buffer chain, starting at the beginning of the I/O
 *   buffer chain
 *
 ****************************************************************************/

void netiob_free_chain(FAR struct netiob_s *netiob);

/****************************************************************************
 * Name: netiob_add_queue
 *
 * Description:
 *   Add one I/O buffer chain to the end of a queue.  May fail due to lack
 *   of resources.
 *
 ****************************************************************************/

#if CONFIG_NET_IOB_NCHAINS > 0
int netiob_add_queue(FAR struct netiob_s *netiob, FAR struct netiob_queue_s *netiobq);
#endif							/* CONFIG_NET_IOB_NCHAINS > 0 */

/****************************************************************************
 * Name: netiob_tryadd_queue
 *
 * Description:
 *   Add one I/O buffer chain to the end of a queue without waiting for
 *   resources to become free.
 *
 ****************************************************************************/

#if CONFIG_NET_IOB_NCHAINS > 0
int netiob_tryadd_queue(FAR struct netiob_s *netiob, FAR struct netiob_queue_s *netiobq);
#endif							/* CONFIG_NET_IOB_NCHAINS > 0 */

/****************************************************************************
 * Name: netiob_remove_queue
 *
 * Description:
 *   Remove and return one I/O buffer chain from the head of a queue.
 *
 * Returned Value:
 *   Returns a reference to the I/O buffer chain at the head of the queue.
 *
 ****************************************************************************/

#if CONFIG_NET_IOB_NCHAINS > 0
FAR struct netiob_s *netiob_remove_queue(FAR struct netiob_queue_s *netiobq);
#endif							/* CONFIG_NET_IOB_NCHAINS > 0 */

/****************************************************************************
 * Name: netiob_peek_queue
 *
 * Description:
 *   Return a reference to the I/O buffer chain at the head of a queue. This
 *   is similar to netiob_remove_queue except that the I/O buffer chain is in
 *   place at the head of the queue.  The I/O buffer chain may safely be
 *   modified by the caller but must be removed from the queue before it can
 *   be freed.
 *
 * Returned Value:
 *   Returns a reference to the I/O buffer chain at the head of the queue.
 *
 ****************************************************************************/

#if CONFIG_NET_IOB_NCHAINS > 0
FAR struct netiob_s *netiob_peek_queue(FAR struct netiob_queue_s *netiobq);
#endif

/****************************************************************************
 * Name: netiob_free_queue
 *
 * Description:
 *   Free an entire queue of I/O buffer chains.
 *
 ****************************************************************************/

#if CONFIG_NET_IOB_NCHAINS > 0
void netiob_free_queue(FAR struct netiob_queue_s *qhead);
#endif							/* CONFIG_NET_IOB_NCHAINS > 0 */

/****************************************************************************
 * Name: netiob_copyin
 *
 * Description:
 *  Copy data 'len' bytes from a user buffer into the I/O buffer chain,
 *  starting at 'offset', extending the chain as necessary.
 *
 ****************************************************************************/

int netiob_copyin(FAR struct netiob_s *netiob, FAR const uint8_t *src, unsigned int len, unsigned int offset, bool throttled);

/****************************************************************************
 * Name: netiob_trycopyin
 *
 * Description:
 *  Copy data 'len' bytes from a user buffer into the I/O buffer chain,
 *  starting at 'offset', extending the chain as necessary BUT without
 *  waiting if buffers are not available.
 *
 ****************************************************************************/

int netiob_trycopyin(FAR struct netiob_s *netiob, FAR const uint8_t *src, unsigned int len, unsigned int offset, bool throttled);

/****************************************************************************
 * Name: netiob_copyout
 *
 * Description:
 *  Copy data 'len' bytes of data into the user buffer starting at 'offset'
 *  in the I/O buffer, returning that actual number of bytes copied out.
 *
 ****************************************************************************/

int netiob_copyout(FAR uint8_t *dest, FAR const struct netiob_s *netiob, unsigned int len, unsigned int offset);

/****************************************************************************
 * Name: netiob_clone
 *
 * Description:
 *   Duplicate (and pack) the data in netiob1 in netiob2.  netiob2 must be empty.
 *
 ****************************************************************************/

int netiob_clone(FAR struct netiob_s *netiob1, FAR struct netiob_s *netiob2, bool throttled);

/****************************************************************************
 * Name: netiob_concat
 *
 * Description:
 *   Concatenate netiob_s chain netiob2 to netiob1.
 *
 ****************************************************************************/

void netiob_concat(FAR struct netiob_s *netiob1, FAR struct netiob_s *netiob2);

/****************************************************************************
 * Name: netiob_trimhead
 *
 * Description:
 *   Remove bytes from the beginning of an I/O chain.  Emptied I/O buffers
 *   are freed and, hence, the beginning of the chain may change.
 *
 ****************************************************************************/

FAR struct netiob_s *netiob_trimhead(FAR struct netiob_s *netiob, unsigned int trimlen);

/****************************************************************************
 * Name: netiob_trimhead_queue
 *
 * Description:
 *   Remove bytes from the beginning of an I/O chain at the head of the
 *   queue.  Emptied I/O buffers are freed and, hence, the head of the
 *   queue may change.
 *
 *   This function is just a wrapper around netiob_trimhead() that assures that
 *   the netiob at the head of queue is modified with the trimming operations.
 *
 * Returned Value:
 *   The new netiob at the head of the queue is returned.
 *
 ****************************************************************************/

#if CONFIG_NET_IOB_NCHAINS > 0
FAR struct netiob_s *netiob_trimhead_queue(FAR struct netiob_queue_s *qhead, unsigned int trimlen);
#endif

/****************************************************************************
 * Name: netiob_trimtail
 *
 * Description:
 *   Remove bytes from the end of an I/O chain.  Emptied I/O buffers are
 *   freed NULL will be returned in the special case where the entry I/O
 *   buffer chain is freed.
 *
 ****************************************************************************/

FAR struct netiob_s *netiob_trimtail(FAR struct netiob_s *netiob, unsigned int trimlen);

/****************************************************************************
 * Name: netiob_pack
 *
 * Description:
 *   Pack all data in the I/O buffer chain so that the data offset is zero
 *   and all but the final buffer in the chain are filled.  Any emptied
 *   buffers at the end of the chain are freed.
 *
 ****************************************************************************/

FAR struct netiob_s *netiob_pack(FAR struct netiob_s *netiob);

/****************************************************************************
 * Name: netiob_contig
 *
 * Description:
 *   Ensure that there is 'len' bytes of contiguous space at the beginning
 *   of the I/O buffer chain starting at 'netiob'.
 *
 ****************************************************************************/

int netiob_contig(FAR struct netiob_s *netiob, unsigned int len);

/****************************************************************************
 * Function: netiob_dump
 *
 * Description:
 *   Dump the contents of a I/O buffer chain
 *
 ****************************************************************************/

#ifdef CONFIG_DEBUG
void netiob_dump(FAR const char *msg, FAR struct netiob_s *netiob, unsigned int len, unsigned int offset);
#else
#define netiob_dump(wrb)
#endif

#endif							/* CONFIG_NET_IOB */
#endif							/* _INCLUDE_NET_IOB_H */
