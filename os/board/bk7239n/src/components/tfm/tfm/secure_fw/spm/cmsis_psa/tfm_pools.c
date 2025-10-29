/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include "thread.h"
#include "psa/client.h"
#include "psa/service.h"
#include "internal_errors.h"
#include "cmsis_compiler.h"
#include "utilities.h"
#include "lists.h"
#include "tfm_pools.h"
#include "tfm_memory_utils.h"
#include "tfm_core_utils.h"

int32_t tfm_pool_init(struct tfm_pool_instance_t *pool, size_t poolsz,
                      size_t chunksz, size_t num)
{
    struct tfm_pool_chunk_t *pchunk;
    size_t i;

    if (!pool || num == 0) {
        return SPM_ERROR_BAD_PARAMETERS;
    }

    /* Ensure buffer is large enough */
    if (poolsz != ((chunksz + sizeof(struct tfm_pool_chunk_t)) * num +
        sizeof(struct tfm_pool_instance_t))) {
        return SPM_ERROR_BAD_PARAMETERS;
    }

    /* Buffer should be BSS cleared but clear it again */
    spm_memset(pool, 0, poolsz);

    /* Chain pool chunks */
    UNI_LISI_INIT_NODE(pool, next);

    pchunk = (struct tfm_pool_chunk_t *)pool->chunks;
    for (i = 0; i < num; i++) {
        UNI_LIST_INSERT_AFTER(pool, pchunk, next);
        pchunk = (struct tfm_pool_chunk_t *)&pchunk->data[chunksz];
    }

    /* Prepare instance and insert to pool list */
    pool->chunksz = chunksz;
    pool->chunk_count = num;

    return SPM_SUCCESS;
}

void *tfm_pool_alloc(struct tfm_pool_instance_t *pool)
{
    struct tfm_pool_chunk_t *node;

    if (!pool) {
        return NULL;
    }

    if (UNI_LIST_IS_EMPTY(pool, next)) {
        return NULL;
    }

    node = UNI_LIST_NEXT_NODE(pool, next);
    UNI_LIST_REMOVE_NODE(pool, node, next);

    return &(((struct tfm_pool_chunk_t *)node)->data);
}

void tfm_pool_free(struct tfm_pool_instance_t *pool, void *ptr)
{
    struct tfm_pool_chunk_t *pchunk;

    pchunk = TO_CONTAINER(ptr, struct tfm_pool_chunk_t, data);

    UNI_LIST_INSERT_AFTER(pool, pchunk, next);
}

bool is_valid_chunk_data_in_pool(struct tfm_pool_instance_t *pool,
                                 uint8_t *data)
{
    const uintptr_t chunks_start = (uintptr_t)(pool->chunks);
    const size_t chunks_size = pool->chunksz + sizeof(struct tfm_pool_chunk_t);
    const size_t chunk_count = pool->chunk_count;
    const uintptr_t chunks_end = chunks_start + chunks_size * chunk_count;
    uintptr_t pool_chunk_address = 0;

    /* Check that the message was allocated from the pool. */
    if ((uintptr_t)data < chunks_start || (uintptr_t)data >= chunks_end) {
        return false;
    }

    pool_chunk_address =
        (uint32_t)TO_CONTAINER(data, struct tfm_pool_chunk_t, data);

    /* Make sure that the chunk containing the message is aligned on */
    /* chunk boundary in the pool. */
    if ((pool_chunk_address - chunks_start) % chunks_size != 0) {
        return false;
    }
    return true;
}
