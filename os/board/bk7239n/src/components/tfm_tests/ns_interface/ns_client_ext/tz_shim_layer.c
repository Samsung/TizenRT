/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*
 * This is a simple example implementaton for reference
 * to make TZ APIs co-work with TF-M NSCE module.
 *
 * Developers can implement according to different RTOS and usage scenarios.
 */

#include "tz_context.h"

#include "tfm_ns_client_ext.h"
#include "tfm_nsid_manager.h"

#ifdef TFM_NS_MANAGE_NSID
#define NSID_MGR_THREAD_ID_MAX       0xFF
#define NSID_MGR_GROUP_ID_DEFAULT    0X00
/*
 * 0 is reserved for thread ID in NSID manager to avoid token being set as 0.
 * TZ_MemoryID_t is used to record the token value.
 * Scheduler of the RTOS would be disabled if TZ_MemoryID_t is 0.
 */
static uint8_t nsid_mgr_thread_id = 1;
#endif

#ifdef TEST_NS_MANAGE_NSID
uint32_t current_active_token;
#endif

/*
 * TF-M shim layer of the CMSIS TZ RTOS thread context management API
 */

/*
 * Initialize token-nsid map table in tfm nsid manager
 * Return execution status (1: success, 0: error)
 */
uint32_t TZ_InitContextSystem_S(void)
{
#ifdef TFM_NS_MANAGE_NSID
    /* Initialize the ns client ext */
    if (tfm_nsce_init(0) == 0) {
        return 0U;    /* Error */
    }

    /* Initialize the nsid manager */
    if (nsid_mgr_init() == NSID_MGR_ERR_SUCCESS) {
#ifdef TEST_NS_MANAGE_NSID
        current_active_token = TFM_NS_CLIENT_INVALID_TOKEN;
#endif
        return 1U;    /* Success */
    } else {
        return 0U;    /* Error */
    }
#else /* TFM_NS_MANAGE_NSID */
    return 1U;    /* Success */
#endif /* TFM_NS_MANAGE_NSID */
}

/*
 * Allocate context memory from Secure side
 * Param: TZ_ModuleId_t (NSID if TFM_NS_MANAGE_NSID is enabled)
 * Return token if TFM_NS_MANAGE_NSID is enabled
 * Return 0 if no memory available or internal error
 */
TZ_MemoryId_t TZ_AllocModuleContext_S(TZ_ModuleId_t module)
{
#ifdef TFM_NS_MANAGE_NSID
    int32_t nsid;
    uint32_t token;

    /* TZ_ModuleID_t is used to record NSID */
    nsid = (int32_t)module;

    /* New thread ID not available, return error */
    if (nsid_mgr_thread_id >= NSID_MGR_THREAD_ID_MAX) {
        return 0U;    /* Error */
    }

    token = tfm_nsce_acquire_ctx(NSID_MGR_GROUP_ID_DEFAULT, nsid_mgr_thread_id++);

    if (nsid_mgr_add_entry(nsid, token) == NSID_MGR_ERR_SUCCESS) {
        return token;    /* Success: return token as TZ_MemoryId_t */
    } else {
        return 0U;    /* Error */
    }
#else /* TFM_NS_MANAGE_NSID */
    return 1U;    /* Success */
#endif /* TFM_NS_MANAGE_NSID */
}

/*
 * Free context memory that was previously allocated with TZ_AllocModuleContext_S
 * Param: TZ_MemoryId_t (token if TFM_NS_MANAGE_NSID is enabled)
 * Return execution status (1: success, 0: error)
 */
uint32_t TZ_FreeModuleContext_S(TZ_MemoryId_t id)
{
#ifdef TFM_NS_MANAGE_NSID
    uint32_t token;

    /* TZ_MemoryId_t is used to record token */
    token = (uint32_t)id;

    if (nsid_mgr_remove_entry(token) != NSID_MGR_ERR_SUCCESS) {
        return 0U;    /* Error */
    }

    if (tfm_nsce_release_ctx(token) == TFM_NS_CLIENT_ERR_SUCCESS) {
#ifdef TEST_NS_MANAGE_NSID
        if (current_active_token != TFM_NS_CLIENT_INVALID_TOKEN) {
            current_active_token = TFM_NS_CLIENT_INVALID_TOKEN;
        }
#endif
        return 1U;    /* Success */
    } else {
        return 0U;    /* Error */
    }
#else /* TFM_NS_MANAGE_NSID */
    return 1U;    /* Success */
#endif /* TFM_NS_MANAGE_NSID */
}

/*
 * Load secure context (called on RTOS thread context switch)
 * Param: TZ_MemoryId_t (token if TFM_NS_MANAGE_NSID is enabled)
 * Return execution status (1: success, 0: error)
 */
uint32_t TZ_LoadContext_S(TZ_MemoryId_t id)
{
#ifdef TFM_NS_MANAGE_NSID
    int32_t nsid;
    uint32_t token;

    /* TZ_MemoryId_t is used to record token */
    token = (uint32_t)id;

    nsid = nsid_mgr_query_nsid(token);

    if (nsid >= TFM_INVALID_NSID_MIN) {
        return 0U;    /* Error */
    }

    if (tfm_nsce_load_ctx(token, nsid) == TFM_NS_CLIENT_ERR_SUCCESS) {
#ifdef TEST_NS_MANAGE_NSID
        current_active_token = token;
#endif
        return 1U;    /* Success */
    } else {
        return 0U;    /* Error */
    }
#else /* TFM_NS_MANAGE_NSID */
    return 1U;    /* Success */
#endif /* TFM_NS_MANAGE_NSID */
}

/*
 * Store secure context (called on RTOS thread context switch)
 * Param: TZ_MemoryId_t (token if TFM_NS_MANAGE_NSID is enabled)
 * Return execution status (1: success, 0: error)
 */
uint32_t TZ_StoreContext_S(TZ_MemoryId_t id)
{
#ifdef TFM_NS_MANAGE_NSID
    uint32_t token;

    /* TZ_MemoryId_t is used to record token */
    token = (uint32_t)id;

    if (tfm_nsce_save_ctx(token) == TFM_NS_CLIENT_ERR_SUCCESS) {
#ifdef TEST_NS_MANAGE_NSID
        if (current_active_token != TFM_NS_CLIENT_INVALID_TOKEN) {
            current_active_token = TFM_NS_CLIENT_INVALID_TOKEN;
        }
#endif
        return 1U;    /* Success */
    } else {
        return 0U;    /* Error */
    }
#else /* TFM_NS_MANAGE_NSID */
    return 1U;    /* Success */
#endif /* TFM_NS_MANAGE_NSID */
}
