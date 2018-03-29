/****************************************************************************
 *
 * Copyright 2016-2017 Samsung Electronics All Rights Reserved.
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
 * Included Files
 ****************************************************************************/

#include <queue.h>
#include <debug.h>
#include <stddef.h>
#include <stdlib.h>		/* for _free function */
#include <semaphore.h>
#include <tinyara/mm/mm.h>
#include <tinyara/mm/kasan.h>
#include <tinyara/clock.h>
#include <stdbool.h>
#include <arch/irq.h>

#include "kasan.h"

#ifdef CONFIG_KASAN_QRNT

#ifdef BUILD_KERNEL
#error Quarantine can work only in flat build
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/**
 * @brief
 *  Pointer to heap which will be passed to _mm_free function during purging
 *
 */
#define HEAP (&g_mmheap)

/**
 * @brief
 *  Check that quarantine size exceeds max watermark
 *
 * @return
 *  True if exceeds and false otherwise
 *
 */
#define qrnt_exceed() (qrnt_watermark_max < qrnt_size)

/**
 * @brief
 *  Check that quarantine size lower than low watermark
 *
 * @return
 *  True if lower and false otherwise
 *
 */
#define qrnt_low()       (qrnt_size <= qrnt_watermark_low)

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

/**
 * @brief
 *  Represents entry of quarantine
 *
 */
struct qrnt_entry_s {
	/* save this fields for casting with mm_allocnode_s and mm_freenode_s */
	mmsize_t size;		/**< size of memory chunk */
	mmsize_t preceding;	/**< size of preceeding chunk  */

	dq_entry_t queue_entry;	/**< struct for doubly linked list entry */

#ifdef CONFIG_KASAN_QRNT_CALC_AVR
	systime_t tick_add;	/**< tick when chunk was added to quarantine */
#endif
};

/****************************************************************************
 * Type Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/**
 * @brief
 *  Doubly linked list structure for keeping links
 *  to heap and tail.
 *
 */
static dq_queue_t qrnt_queue;

/**
 * @brief
 *  Summary size of all memory chunks in quarantine.
 *
 */
static unsigned int qrnt_size;

/**
 * @brief
 *  Max watermark of quarantine. Calculated as
 *  CONFIG_KASAN_QRNT_MAX percents from heap size.
 *
 */
static unsigned int qrnt_watermark_max;

/**
 * @brief
 *  Low watermark of quarantine. Calculated as
 *  CONFIG_KASAN_QRNT_LOW percents from max watermark.
 *
 */
static unsigned int qrnt_watermark_low;

#ifdef CONFIG_KASAN_QRNT_CALC_AVR
/**
 * @brief
 *  Average time that chunks spent in quarantine
 *
 */
static unsigned long long avr;

/**
 * @brief
 *  Counter for calculating average time
 *
 */
static unsigned long cnt = 1;
#endif

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/**
 * @brief
 *  Poison only quarantine entry struct
 *
 * @param entry pointer to quarantine entry
 *
 * @return
 *  None
 *
 */
static inline inline_function void
qrnt_entry_poison(const struct qrnt_entry_s *entry)
{
	kasan_poison_heap((unsigned long)entry, sizeof(*entry),
			  KASAN_QUARANTINE);
}

/**
 * @brief
 *  Unpoison only quarantine entry struct
 *
 * @param entry pointer to quarantine entry
 *
 * @return
 *  None
 *
 */
static inline inline_function void
qrnt_entry_unpoison(const struct qrnt_entry_s *entry)
{
	kasan_unpoison_heap((unsigned long)entry, sizeof(*entry));
}

/**
 * @brief
 *  Poison quarantine entry and chunk that entry describes
 *
 * @param entry pointer to quarantine entry
 *
 * @return
 *  None
 *
 */
static inline inline_function void
qrnt_entry_chunk_poison(const struct qrnt_entry_s *entry)
{
	kasan_poison_heap((unsigned long)entry, entry->size, KASAN_QUARANTINE);
}

/**
 * @brief
 *  Unpoison quarantine entry and chunk that entry describes
 *
 * @param entry pointer to quarantine entry
 *
 * @return
 *  None
 *
 */
static inline inline_function void
qrnt_entry_chunk_unpoison(const struct qrnt_entry_s *entry)
{
	kasan_unpoison_heap((unsigned long)entry, entry->size);
}

/**
 * @brief
 *  Poison only doubly linked list struct
 *
 * @param entry pointer to doubly linked list entry
 *
 * @return
 *  None
 *
 */
static inline inline_function void
queue_entry_poison(const dq_entry_t *entry)
{
	kasan_poison_heap((unsigned long)entry, sizeof(*entry),
			  KASAN_QUARANTINE);
}

/**
 * @brief
 *  Unpoison only doubly linked list struct
 *
 * @param entry pointer to doubly linked list entry
 *
 * @return
 *  None
 *
 */
static inline inline_function void
queue_entry_unpoison(const dq_entry_t *entry)
{
	kasan_unpoison_heap((unsigned long)entry, sizeof(*entry));
}

/**
 * @brief
 *  Add quarantine entry to the beginning of quaranitne list
 *
 * @param qrnt_entry pointer to quarantine entry
 *
 * @return
 *  OK on success adn ERROR otherwise.
 *
 */
static int qrnt_enq(struct qrnt_entry_s *qrnt_entry)
{
	dq_entry_t *entry;
	dq_entry_t *head;
	irqstate_t flags;

	flags = irqsave();

	qrnt_entry_unpoison(qrnt_entry);

	entry = &qrnt_entry->queue_entry;
	entry->blink = NULL;
	entry->flink = qrnt_queue.head;

	head = qrnt_queue.head;
	if (!head) {
		qrnt_queue.head = entry;
		qrnt_queue.tail = entry;
	} else {
		queue_entry_unpoison(head);

		head->blink = entry;
		qrnt_queue.head = entry;

		queue_entry_poison(head);
	}

	qrnt_size += qrnt_entry->size;

#ifdef CONFIG_KASAN_QRNT_CALC_AVR
	qrnt_entry->tick_add = clock_systimer();
#endif

	qrnt_entry_chunk_poison(qrnt_entry);

	irqrestore(flags);
	return OK;
}

/**
 * @brief
 *  Remove quarantine entry from the end of quaranitne list
 *
 * @param
 *  None
 *
 * @return
 *  Quarantine entry which was removed
 *
 */
static struct qrnt_entry_s *qrnt_deq(void)
{
	dq_entry_t *prev;
	dq_entry_t *tail;
	struct qrnt_entry_s *qrnt_entry;
	irqstate_t flags;

	flags = irqsave();

	tail = qrnt_queue.tail;

	if (!tail) {
		irqrestore(flags);
		return NULL;
	}

	queue_entry_unpoison(tail);
	prev = tail->blink;

	if (!prev) {
		qrnt_queue.head = NULL;
		qrnt_queue.tail = NULL;
	} else {
		queue_entry_unpoison(prev);

		qrnt_queue.tail = prev;
		prev->flink = NULL;

		queue_entry_poison(prev);
	}

	tail->flink = NULL;
	tail->blink = NULL;

	qrnt_entry = container_of(tail, struct qrnt_entry_s, queue_entry);
	qrnt_entry_unpoison(qrnt_entry);

#ifdef CONFIG_KASAN_QRNT_CALC_AVR
	long long diff;

	diff = clock_systimer() - qrnt_entry->tick_add;
	diff -= avr;
	avr += diff / cnt++;
#endif

	qrnt_size -= qrnt_entry->size;
	qrnt_entry_poison(qrnt_entry);

	irqrestore(flags);
	return qrnt_entry;
}

/**
 * @brief
 *  Check addr on locating in quarantine
 *
 * @param addr memory address
 *
 * @return
 *  True if memory chunk already in quarantine and false otherwise
 *
 */
static bool double_free_check(unsigned long addr)
{
	irqstate_t flags;
	unsigned char shadow_val;

	shadow_val = kasan_shadow_val(addr);

	if (shadow_val != KASAN_QUARANTINE)
		return false;

	kasan_report_prologue(addr, "double free", &flags);
	kasan_report_epilogue(&flags);

	return true;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/**
 * @brief
 *  Initialize KASan quarantine queue.
 *
 * @param heapsize size of the heap for watermark calculations
 *
 * @return
 *   None
 *
 */
void kasan_qrnt_init(unsigned long heapsize)
{
	kllvdbg(KASAN_PREFIX "quarantine initialization\n");

	qrnt_watermark_max = heapsize           / 100 *
		CONFIG_KASAN_QRNT_MAX + 1;
	qrnt_watermark_low = qrnt_watermark_max / 100 *
		CONFIG_KASAN_QRNT_LOW + 1;

	kllvdbg(KASAN_PREFIX "heapsize %ld\n", heapsize);
	kllvdbg(KASAN_PREFIX "qrnt_watermark_max %ld\n", qrnt_watermark_max);
	kllvdbg(KASAN_PREFIX "qrnt_watermark_low %ld\n", qrnt_watermark_low);

	dq_init(&qrnt_queue);
	qrnt_size = 0;
}

/**
 * @brief
 *  Enqueue entry to quarantine queue
 *
 * @param entry pointer to memory chunk (behind mm_allocnode_s structure)
 *
 * @return
 *   None
 *
 */
void kasan_qrnt_enq(void *entry)
{
	struct qrnt_entry_s *qrnt_entry;

	if (!entry) {
		kllwdbg(KASAN_PREFIX "NULL entry\n");
		return;
	}

	qrnt_entry = entry - SIZEOF_MM_ALLOCNODE;

	if (double_free_check((unsigned long)qrnt_entry))
		return;

	if (qrnt_enq(qrnt_entry))
		return;

	kasan_qrnt_purge();
}

/**
 * @brief
 *  Dequeue entry from quarantine queue.
 *
 * @param
 *   None
 *
 * @return
 *   Pointer to memory chunk.
 *
 */
void *kasan_qrnt_deq(void)
{
	void *qrnt_entry;

	qrnt_entry = qrnt_deq();

	if (!qrnt_entry) {
		kllwdbg(KASAN_PREFIX "NULL entry\n");
		return NULL;
	}

	return qrnt_entry + SIZEOF_MM_ALLOCNODE;
}

/**
 * @brief
 *   If quarantine size exceeded watermark_max, return memory chunks to
 *   allocator until quarantine size is less than or equal watermark_low.
 *
 * @param
 *   None
 *
 * @return
 *   None
 *
 */
void kasan_qrnt_purge(void)
{
	void *mem;

	if (!qrnt_exceed())
		return;

	kllvdbg(KASAN_PREFIX "purge\n");
	kllvdbg(KASAN_PREFIX "quarantine size before %ld\n", qrnt_size);

	while (!qrnt_low()) {
		mem = kasan_qrnt_deq();

		if (mem)
			_mm_free(HEAP, mem);
	}

	kllvdbg(KASAN_PREFIX "quarantine size after %ld\n", qrnt_size);

#ifdef CONFIG_KASAN_QRNT_CALC_AVR
	kllvdbg(KASAN_PREFIX "average time in quarantine: %llums (~%llus)\n",
		TICK2MSEC(avr), TICK2SEC(avr));
#endif
}

#endif	/* CONFIG_KASAN_QRNT */
