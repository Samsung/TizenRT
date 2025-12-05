/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __TFM_NS_CTX_H__
#define __TFM_NS_CTX_H__

#include <stdint.h>
#include <stdbool.h>

/* Supported maximum context for NS. Only support single context for now. */
#define TFM_NS_CONTEXT_MAX                  1

#define TFM_NS_CONTEXT_MAX_TID              0xFF

/* Non-secure context structure */
struct tfm_ns_ctx_t {
    int32_t nsid;       /* Non-secure Client ID, must be < 0 */
    uint8_t gid;        /* Group ID. Threads in same group share one context */
    uint8_t tid;        /* Thread ID. Used to identify threads in same group */
    uint8_t ref_cnt;    /* The number of threads sharing this context */
};

/* Initialize the non-secure context */
bool init_ns_ctx(void);

/*
 * Acquire the non-secure context for a non-secure client thread
 * gid: The group ID of the thread. The threads in one group share one context.
 * idx: Output buffer to retrieve the index of the allocated context.
 * Return: bool type to indicate success of the context allocation.
 */
bool acquire_ns_ctx(uint8_t gid, uint8_t *idx);

/*
 * Release the non-secure context for a non-secure client thread
 * gid: The group ID of the thread. The threads in one group share one context.
 * tid: The thread ID.
 * idx: The context index for that thread.
 * Return: bool type to indicate success of the context release.
 */
bool release_ns_ctx(uint8_t gid, uint8_t tid, uint8_t idx);

/*
 * Load the context for the non-secure client thread
 * gid: The group ID of the thread. The threads in one group share one context.
 * tid: The thread ID.
 * nsid: The non-secure client ID for that thread.
 * idx: The context index for that thread.
 * Return: bool type to indicate success of the context load.
 */
bool load_ns_ctx(uint8_t gid, uint8_t tid, int32_t nsid, uint8_t idx);

/*
 * Save the context for the non-secure client thread
 * gid: The group ID of the thread. The threads in one group share one context.
 * tid: The thread ID.
 * idx: The context index for that thread.
 * Return: bool type to indicate success of the context save.
 */
bool save_ns_ctx(uint8_t gid, uint8_t tid, uint8_t idx);

/*
 * Return non-secure client ID in the active non-secure context.
 * Return invalid NSID if no context is active.
 */
int32_t get_nsid_from_active_ns_ctx(void);

#endif  /* __TFM_NS_CTX_H__ */
