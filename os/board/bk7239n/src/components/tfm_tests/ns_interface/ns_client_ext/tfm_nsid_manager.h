/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*
 * This is a simple implementaton for reference to manage NSID from NS side.
 * Developers can design the management according to RTOS and usage scenarios.
 */

#ifndef __TFM_NSID_MANAGER_H__
#define __TFM_NSID_MANAGER_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))
#endif

#define TFM_DEFAULT_GID         0x00
#define TFM_DEFAULT_NSID        ((int32_t)-1)
/* Non-secure client ID needs to be negative */
#define TFM_INVALID_NSID_MIN    ((int32_t)0)

/* NSID Manager Error Code */
#define NSID_MGR_ERR_SUCCESS          0x00
#define NSID_MGR_ERR_INVALID_NSID     0x01
#define NSID_MGR_ERR_INVALID_TOKEN    0x02
#define NSID_MGR_ERR_NO_FREE_ENTRY    0x03

/*
 * Current active NSID token needs to be manually save and reload
 * before and after calling NS client ext directly from SVC in NSID test suite.
 */
#ifdef TEST_NS_MANAGE_NSID
extern uint32_t current_active_token;
#endif

/*
 * Initialize the table to map token and nsid.
 * This function should be called before any other NSID manager APIs.
 */
uint8_t nsid_mgr_init(void);

/*
 * Add a new nsid-token map entry to the table.
 * This function should be called once a new token
 * has been successfully assigned by ns_client_ext.
 */
uint8_t nsid_mgr_add_entry(int32_t nsid, uint32_t token);

/*
 * Delete a nsid-token map entry from the table.
 * This function should be called once an existing token
 * has been successfully released by ns_client_ext.
 */
uint8_t nsid_mgr_remove_entry(uint32_t token);

/*
 * Query NSID from the map table with token.
 * This function is to get NSID assigned by RTOS with token of a NS thread.
 */
int32_t nsid_mgr_query_nsid(uint32_t token);

#ifdef __cplusplus
}
#endif

#endif /* __TFM_NSID_MANAGER_H__ */
