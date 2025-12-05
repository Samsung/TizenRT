/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __TFM_POOLS_H__
#define __TFM_POOLS_H__

#include <stdbool.h>
#include "compiler_ext_defs.h"
#include "lists.h"

/*
 * Pool Instance:
 *  [ Pool Instance ] + N * [ Pool Chunks ]
 */
struct tfm_pool_chunk_t {
    struct tfm_pool_chunk_t *next;        /* Chunk list                     */
    uint8_t data[];                       /* Data indicator                 */
};

struct tfm_pool_instance_t {
    struct tfm_pool_chunk_t *next;        /* Point to the first free node   */
    size_t chunksz;                       /* Chunks size of pool member     */
    size_t chunk_count;                   /* A number of chunks in the pool */
    uint8_t chunks[];                     /* Data indicator                 */
};

/*
 * This will declares a static memory pool variable with chunk memory.
 * Parameters:
 *  name        -   Variable name, will be used when register
 *  chunksz     -   chunk size in bytes
 *  num         -   Number of chunks
 */
#define TFM_POOL_DECLARE(name, chunksz, num)                                \
    static uint8_t name##_pool_buf[((chunksz) +                             \
                                   sizeof(struct tfm_pool_chunk_t)) * (num) \
                                   + sizeof(struct tfm_pool_instance_t)]    \
                                   __aligned(4);                            \
    static struct tfm_pool_instance_t *name =                               \
                            (struct tfm_pool_instance_t *)name##_pool_buf

/* Get the head size of memory pool */
#define POOL_HEAD_SIZE (sizeof(struct tfm_pool_instance_t) +                \
                        sizeof(struct tfm_pool_chunk_t))

/* Get the whole size of memory pool */
#define POOL_BUFFER_SIZE(name)          sizeof(name##_pool_buf)

/**
 * \brief Register a memory pool.
 *
 * \param[in] pool              Pointer to memory pool declared by
 *                              \ref TFM_POOL_DECLARE
 * \param[in] poolsz             Size of the pool buffer.
 * \param[in] chunksz           Size of chunks.
 * \param[in] num               Number of chunks.
 *
 * \retval SPM_SUCCESS          Success.
 * \retval SPM_ERROR_BAD_PARAMETERS Parameters error.
 */
int32_t tfm_pool_init(struct tfm_pool_instance_t *pool, size_t poolsz,
                      size_t chunksz, size_t num);

/**
 * \brief Allocate a memory from pool.
 *
 * \param[in] pool              pool pointer decleared by \ref TFM_POOL_DECLARE
 *
 * \retval buffer pointer       Success.
 * \retval NULL                 Failed.
 */
void *tfm_pool_alloc(struct tfm_pool_instance_t *pool);

/**
 * \brief Free the allocated memory.
 *
 * \param[in] pool              pool pointer decleared by \ref TFM_POOL_DECLARE
 *
 * \param[in] ptr               Buffer pointer want to free.
 */
void tfm_pool_free(struct tfm_pool_instance_t *pool, void *ptr);

/**
 * \brief Checks whether a pointer points to a chunk data in the pool.
 *
 * \param[in] pool              Pointer to memory pool declared by
 *                              \ref TFM_POOL_DECLARE.
 * \param[in] data              The pointer to check.
 *
 * \retval true                 Data is a chunk data in the pool.
 * \retval false                Data is not a chunk data in the pool.
 */
bool is_valid_chunk_data_in_pool(struct tfm_pool_instance_t *pool,
                                 uint8_t *data);

#endif /* __TFM_POOLS_H__ */
