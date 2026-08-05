/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
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

#include <tinyara/config.h>

#include <stdint.h>
#include <assert.h>
#include <debug.h>

#include <tinyara/mm/mm.h>

#include "mm_node.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mm_uaf_area
 *
 * Description:
 *   Locate the part of the free chunk 'node' which carries the poison
 *   pattern. It is the data area of the chunk, excluding the free node
 *   bookkeeping fields (flink/blink and the free debug info) which the
 *   allocator itself keeps using while the chunk sits in the free list.
 *
 *   Those fields are skipped by MM_ALIGN_UP(SIZEOF_MM_FREENODE), the same
 *   granule alignment the allocator applies to every other offset. Since a
 *   node address is always a multiple of sizeof(uint32_t), that keeps the
 *   pattern accessible with aligned 32-bit reads and writes.
 *
 *   The size is clamped to CONFIG_DEBUG_MM_UAF_POISON_SIZE and truncated to
 *   whole words, so nothing is ever written past the end of the chunk.
 *
 * Returned Value:
 *   The number of poisoned words, 0 if the chunk is too small to hold any.
 *   On a non-zero return, *poison points to the first poisoned word.
 *
 ****************************************************************************/

static size_t mm_uaf_area(struct mm_freenode_s *node, uint32_t **poison)
{
	size_t offset = MM_ALIGN_UP(SIZEOF_MM_FREENODE);
	size_t nbytes;

	if (node->size <= offset) {
		/* The chunk is all bookkeeping, there is no room for the pattern */
		return 0;
	}

	nbytes = node->size - offset;
	if (nbytes > CONFIG_DEBUG_MM_UAF_POISON_SIZE) {
		nbytes = CONFIG_DEBUG_MM_UAF_POISON_SIZE;
	}

	*poison = (uint32_t *)((char *)node + offset);

	return nbytes / sizeof(uint32_t);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mm_uaf_poison
 *
 * Description:
 *   Fill the data area of a chunk which is about to enter the free list with
 *   the poison pattern. Called for every chunk which becomes free, so that
 *   mm_uaf_verify() has a known content to compare against.
 *
 *   NOTES:
 *     (1) node->size must already be set to its final value.
 *     (2) the caller must hold the MM semaphore.
 *
 ****************************************************************************/

void mm_uaf_poison(struct mm_freenode_s *node)
{
	uint32_t *poison;
	size_t nwords;
	size_t i;

	nwords = mm_uaf_area(node, &poison);
	for (i = 0; i < nwords; i++) {
		poison[i] = MM_UAF_PATTERN;
	}
}

/****************************************************************************
 * Name: mm_uaf_verify
 *
 * Description:
 *   Check that the poison pattern written by mm_uaf_poison() is still intact
 *   in the chunk which is about to be handed out. Any difference means that
 *   the chunk was written while it was free, which is a use-after-free by
 *   whoever owned it before.
 *
 *   NOTES:
 *     (1) must be called before the chunk is removed from the free list and
 *         split, so that the poisoned area is still the one which was
 *         poisoned when the chunk was freed.
 *     (2) the caller must hold the MM semaphore.
 *
 ****************************************************************************/

void mm_uaf_verify(struct mm_freenode_s *node)
{
	uint32_t *poison;
	size_t nwords;
	size_t i;

	nwords = mm_uaf_area(node, &poison);
	for (i = 0; i < nwords; i++) {
		if (poison[i] != MM_UAF_PATTERN) {
			break;
		}
	}

	if (i >= nwords) {
		/* The pattern is intact, nothing wrote into the chunk while free */
		return;
	}

	heap_dbg("#########################################################################################\n");
	mfdbg("ERROR: Use after free detected.\n");
	mm_dump_node((struct mm_allocnode_s *)node, "FREE NODE");
	mm_dump_heap_region((uint32_t)node, (uint32_t)node + node->size);

#ifdef CONFIG_DEBUG_MM_UAF_PANIC
	PANIC();
#endif
}
