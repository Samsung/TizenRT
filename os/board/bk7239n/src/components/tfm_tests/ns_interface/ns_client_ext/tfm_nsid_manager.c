/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_ns_client_ext.h"
#include "tfm_nsid_manager.h"

/* Max number of threads could be customised by RTOS */
#ifndef THREAD_NUM_MAX
#define THREAD_NUM_MAX          10
#endif

/* Map table of token and NSIDs */
struct nsid_token_pair {
    uint32_t token;
    int32_t  nsid;
};

static struct nsid_token_pair test_ns_token_table[THREAD_NUM_MAX];

uint8_t nsid_mgr_init(void)
{
    uint32_t i;

    for (i = 0; i < THREAD_NUM_MAX; i++) {
        test_ns_token_table[i].token = TFM_NS_CLIENT_INVALID_TOKEN;
        test_ns_token_table[i].nsid = TFM_INVALID_NSID_MIN;
    }

    return NSID_MGR_ERR_SUCCESS;
}

uint8_t nsid_mgr_add_entry(int32_t nsid, uint32_t token)
{
    uint32_t i;

    if (nsid >= TFM_INVALID_NSID_MIN) {
        return NSID_MGR_ERR_INVALID_NSID;
    }

    if (token == TFM_NS_CLIENT_INVALID_TOKEN) {
        return NSID_MGR_ERR_INVALID_TOKEN;
    }

    for (i = 0; i < THREAD_NUM_MAX; i++) {
        if (test_ns_token_table[i].token == TFM_NS_CLIENT_INVALID_TOKEN) {
            test_ns_token_table[i].token = token;
            test_ns_token_table[i].nsid = nsid;
            return NSID_MGR_ERR_SUCCESS;
        }
    }

    /* No free entry for new token, return error */
    return NSID_MGR_ERR_NO_FREE_ENTRY;
}

uint8_t nsid_mgr_remove_entry(uint32_t token)
{
    uint32_t i;

    if (token == TFM_NS_CLIENT_INVALID_TOKEN) {
        return NSID_MGR_ERR_INVALID_TOKEN;
    }

    for (i = 0; i < THREAD_NUM_MAX; i++) {
        if (test_ns_token_table[i].token == token) {
            test_ns_token_table[i].token = TFM_NS_CLIENT_INVALID_TOKEN;
            test_ns_token_table[i].nsid = TFM_INVALID_NSID_MIN;
            return NSID_MGR_ERR_SUCCESS;
        }
    }

    /* Token not found in the table, return error */
    return NSID_MGR_ERR_INVALID_TOKEN;
}

int32_t nsid_mgr_query_nsid(uint32_t token)
{
    uint32_t i;

    /* Return invalid NSID if token is invalid */
    if (token == TFM_NS_CLIENT_INVALID_TOKEN) {
        return TFM_INVALID_NSID_MIN;
    }

    for (i = 0; i < THREAD_NUM_MAX; i++) {
        if (test_ns_token_table[i].token == token) {
            return test_ns_token_table[i].nsid;
        }
    }

    /* Token not found in the table, return invalid NSID */
    return TFM_INVALID_NSID_MIN;
}
