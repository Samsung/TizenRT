/*****************************************************************************
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
#include "scsc_mif_abs.h"
#include <pthread.h>
#include <debug_scsc.h>
#include <sys/types.h>
#include "utils_misc.h"

#include "miframman.h"

void miframman_init(struct miframman *ram, void *start_dram, size_t size_pool)
{
	pthread_mutex_init(&ram->lock, NULL);

	ram->num_blocks = size_pool / MIFRAMMAN_BLOCK_SIZE;

	if (ram->num_blocks == 0) {
		SLSI_ERR_NODEV("Pool size < BLOCK_SIZE\n");
		return;
	}

	if (ram->num_blocks > MIFRAMMAN_NUM_BLOCKS) {
		SLSI_ERR_NODEV("Not enough memory\n");
		return;
	}

	memset(ram->bitmap, BLOCK_FREE, sizeof(ram->bitmap));

	ram->start_dram = start_dram;
	ram->size_pool = size_pool;
	ram->free_mem = ram->num_blocks * MIFRAMMAN_BLOCK_SIZE;
}

void *__miframman_alloc(struct miframman *ram, size_t nbytes)
{
	unsigned int index = 0;
	unsigned int available;
	unsigned int i;
	size_t num_blocks;
	void *free_mem = NULL;

	if (!nbytes || nbytes > ram->free_mem) {
		goto end;
	}

	/* Number of blocks required (rounding up) */
	num_blocks = nbytes / MIFRAMMAN_BLOCK_SIZE + ((nbytes % MIFRAMMAN_BLOCK_SIZE) > 0 ? 1 : 0);

	if (num_blocks > ram->num_blocks) {
		goto end;
	}

	pthread_mutex_lock(&ram->lock);
	while (index <= (ram->num_blocks - num_blocks)) {
		available = 0;

		/* Search consecutive blocks */
		for (i = 0; i < num_blocks; i++) {
			if (ram->bitmap[i + index] != BLOCK_FREE) {
				break;
			}
			available++;
		}
		if (available == num_blocks) {
			free_mem = ram->start_dram + MIFRAMMAN_BLOCK_SIZE * index;

			/* Mark the blocks as used */
			ram->bitmap[index++] = BLOCK_BOUND;
			for (i = 1; i < num_blocks; i++) {
				ram->bitmap[index++] = BLOCK_INUSE;
			}

			ram->free_mem -= num_blocks * MIFRAMMAN_BLOCK_SIZE;
			pthread_mutex_unlock(&ram->lock);
			goto exit;
		} else {
			index = index + available + 1;
		}
	}
	pthread_mutex_unlock(&ram->lock);
end:
	SLSI_ERR_NODEV("Not enough memory\n");
	return NULL;
exit:
	return free_mem;
}

#define MIFRAMMAN_ALIGN(mem, align) \
	((void *)((((uintptr_t)(mem) + (align + sizeof(void *))) & \
			(~(uintptr_t)(align - 1)))))

#define MIFRAMMAN_PTR(mem) \
	(*(((void **)((uintptr_t)(mem) & \
			(~(uintptr_t)(sizeof(void *)-1)))) - 1))

bool is_power_of_2(unsigned long n)
{
	return (n != 0 && ((n & (n - 1)) == 0));
}

void *miframman_alloc(struct miframman *ram, size_t nbytes, size_t align)
{
	void *mem, *align_mem;

	if (!is_power_of_2(align) || nbytes == 0) {
		return NULL;
	}

	if (align < sizeof(void *)) {
		align = sizeof(void *);
	}

	mem = __miframman_alloc(ram, nbytes + align + sizeof(void *));
	if (!mem) {
		return NULL;
	}

	align_mem = MIFRAMMAN_ALIGN(mem, align);

	/* Store allocated pointer */
	MIFRAMMAN_PTR(align_mem) = mem;

	return align_mem;
}

void __miframman_free(struct miframman *ram, void *mem)
{
	unsigned int index, num_blocks = 0;

	pthread_mutex_lock(&ram->lock);
	if (ram->start_dram == NULL || !mem) {
		SLSI_ERR_NODEV("Mem is NULL\n");
		pthread_mutex_unlock(&ram->lock);	// unlock before returning.
		return;
	}

	/* Get block index */
	index = (unsigned int)((mem - ram->start_dram)
						   / MIFRAMMAN_BLOCK_SIZE);

	/* Check */
	if (index >= ram->num_blocks) {
		pr_err("%s: Incorrect index %d\n", __func__, index);
		goto end;
	}

	/* Check it is a Boundary block */
	if (ram->bitmap[index] != BLOCK_BOUND) {
		pr_err("%s: Incorrect Block descriptor\n", __func__);
		goto end;
	}

	ram->bitmap[index++] = BLOCK_FREE;
	num_blocks++;
	while (index < ram->num_blocks && ram->bitmap[index] == BLOCK_INUSE) {
		ram->bitmap[index++] = BLOCK_FREE;
		num_blocks++;
	}

	ram->free_mem += num_blocks * MIFRAMMAN_BLOCK_SIZE;
end:
	pthread_mutex_unlock(&ram->lock);
}

void miframman_free(struct miframman *ram, void *mem)
{
	/* Restore allocated pointer */
	if (mem) {
		__miframman_free(ram, MIFRAMMAN_PTR(mem));
	}
}

void miframman_deinit(struct miframman *ram)
{
	/* Mark all the blocks as INUSE to prevent new allocations */
	memset(ram->bitmap, BLOCK_INUSE, sizeof(ram->bitmap));

	ram->num_blocks = 0;
	ram->start_dram = NULL;
	ram->size_pool = 0;
	ram->free_mem = 0;
}
