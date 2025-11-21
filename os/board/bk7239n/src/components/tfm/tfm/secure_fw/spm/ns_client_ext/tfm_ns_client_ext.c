/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include "tfm_arch.h"
#include "tfm_nspm.h"
#include "tfm_ns_client_ext.h"
#include "tfm_ns_ctx.h"

/*
 * Definiton of NS Client Token
 * bit[0:7] - Thread ID
 * bit[8:15] - Group ID
 * bit[16:23] - Context identifier
 * bit[24:31] - Reserved, must be 0
 */
#define MAKE_NS_CLIENT_TOKEN(tid, gid, idx)                         \
                             (uint32_t)((((uint32_t)tid & 0xff)     \
                             | (((uint32_t)gid & 0xff) << 8)        \
                             | (((uint32_t)idx & 0xff) << 16))      \
                             & 0x00ffffff)
#define IS_INVALID_TOKEN(token)             ((token) & 0xff000000)
#define NS_CLIENT_TOKEN_TO_CTX_IDX(token)   (((token) >> 16) & 0xff)
#define NS_CLIENT_TOKEN_TO_GID(token)       (((token) >> 8) & 0xff)
#define NS_CLIENT_TOKEN_TO_TID(token)       ((token) & 0xff)

__tfm_nspm_secure_gateway_attributes__
uint32_t tfm_nsce_init(uint32_t ctx_requested)
{
    if (__get_active_exc_num() == EXC_NUM_THREAD_MODE) {
        /* This veneer should only be called by NS RTOS in handler mode */
        return 0; /* Return 0 if the caller is from thread mode */
    }

    /* Nothing to initialize for now */
    if (ctx_requested == 0) {
        /* 0 means requesting maximum available context */
        return TFM_NS_CONTEXT_MAX;
    } else {
        return ((ctx_requested <= TFM_NS_CONTEXT_MAX) ?
                ctx_requested : TFM_NS_CONTEXT_MAX);
    }
}

__tfm_nspm_secure_gateway_attributes__
uint32_t tfm_nsce_acquire_ctx(uint8_t group_id, uint8_t thread_id)
{
    uint8_t ns_ctx_idx;

    if (__get_active_exc_num() == EXC_NUM_THREAD_MODE) {
        /* This veneer should only be called by NS RTOS in handler mode */
        return TFM_NS_CLIENT_ERR_INVALID_ACCESS;
    }

    /* Try to allocate the non-secure context */
    if (!acquire_ns_ctx(group_id, &ns_ctx_idx)) {
        return TFM_NS_CLIENT_INVALID_TOKEN;
    }

    /* Return the token */
    return MAKE_NS_CLIENT_TOKEN(thread_id, group_id, ns_ctx_idx);
}

__tfm_nspm_secure_gateway_attributes__
uint32_t tfm_nsce_release_ctx(uint32_t token)
{
    /* Free the context slot assigned */
    uint8_t ctx_idx;
    uint8_t gid;
    uint8_t tid;

    if (__get_active_exc_num() == EXC_NUM_THREAD_MODE) {
        /* This veneer should only be called by NS RTOS in handler mode */
        return TFM_NS_CLIENT_ERR_INVALID_ACCESS;
    }

    if (IS_INVALID_TOKEN(token)) {
        return TFM_NS_CLIENT_ERR_INVALID_TOKEN;
    } else {
        /* Extract the context index, group ID and thread ID from the token */
        ctx_idx = NS_CLIENT_TOKEN_TO_CTX_IDX(token);
        gid = NS_CLIENT_TOKEN_TO_GID(token);
        tid = NS_CLIENT_TOKEN_TO_TID(token);
    }

    if (!release_ns_ctx(gid, tid, ctx_idx)) {
        return TFM_NS_CLIENT_ERR_INVALID_TOKEN;
    } else {
        return TFM_NS_CLIENT_ERR_SUCCESS;
    }
}

__tfm_nspm_secure_gateway_attributes__
uint32_t tfm_nsce_load_ctx(uint32_t token, int32_t nsid)
{
    uint8_t ctx_idx;
    uint8_t gid;
    uint8_t tid;

    if (__get_active_exc_num() == EXC_NUM_THREAD_MODE) {
        /* This veneer should only be called by NS RTOS in handler mode */
        return TFM_NS_CLIENT_ERR_INVALID_ACCESS;
    }

    /* Check if NSID is valid */
    if (nsid >= 0) {
        return TFM_NS_CLIENT_ERR_INVALID_NSID;
    }

    if (IS_INVALID_TOKEN(token)) {
        return TFM_NS_CLIENT_ERR_INVALID_TOKEN;
    } else {
        /* Extract the context index, group ID and thread ID from the token */
        ctx_idx = NS_CLIENT_TOKEN_TO_CTX_IDX(token);
        gid = NS_CLIENT_TOKEN_TO_GID(token);
        tid = NS_CLIENT_TOKEN_TO_TID(token);
    }

    if (!load_ns_ctx(gid, tid, nsid, ctx_idx)) {
        return TFM_NS_CLIENT_ERR_INVALID_TOKEN;
    } else {
        return TFM_NS_CLIENT_ERR_SUCCESS;
    }
}

__tfm_nspm_secure_gateway_attributes__
uint32_t tfm_nsce_save_ctx(uint32_t token)
{
    uint8_t ctx_idx;
    uint8_t gid;
    uint8_t tid;

    if (__get_active_exc_num() == EXC_NUM_THREAD_MODE) {
        /* This veneer should only be called by NS RTOS in handler mode */
        return TFM_NS_CLIENT_ERR_INVALID_ACCESS;
    }

    if (IS_INVALID_TOKEN(token)) {
        return TFM_NS_CLIENT_ERR_INVALID_TOKEN;
    } else {
        /* Extract the context index, group ID and thread ID from the token */
        ctx_idx = NS_CLIENT_TOKEN_TO_CTX_IDX(token);
        gid = NS_CLIENT_TOKEN_TO_GID(token);
        tid = NS_CLIENT_TOKEN_TO_TID(token);
    }

    if (!save_ns_ctx(gid, tid, ctx_idx)) {
        return TFM_NS_CLIENT_ERR_INVALID_TOKEN;
    } else {
        return TFM_NS_CLIENT_ERR_SUCCESS;
    }
}
