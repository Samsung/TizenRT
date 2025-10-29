/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __SERVICE_DEFS_H__
#define __SERVICE_DEFS_H__

#include <stdint.h>
#include "config_impl.h"
#include "psa/service.h"

/*
 * Service load data - flags
 * bit 7-0: stateless handle index
 * bit 8: 1 - NS accessible, 0 - NS not accessible
 * bit 9: 1 - stateless, 0 - connection-based
 * bit 10: 1 - strict version policy, 0 - relaxed version policy
 * bit 11: 1 - MM-IOVEC enabled, 0 - MM-IOVEC disabled
 */
#define SERVICE_FLAG_STATELESS_HINDEX_MASK      (0xFF)
#define SERVICE_FLAG_NS_ACCESSIBLE              (1U << 8)
#define SERVICE_FLAG_STATELESS                  (1U << 9)

#define SERVICE_FLAG_VERSION_POLICY_BIT         (1U << 10)
#define SERVICE_VERSION_POLICY_RELAXED          (0U << 10)
#define SERVICE_VERSION_POLICY_STRICT           (1U << 10)
#define SERVICE_FLAG_MM_IOVEC                   (1U << 11)

#define SERVICE_GET_STATELESS_HINDEX(flag)      \
    ((flag) & SERVICE_FLAG_STATELESS_HINDEX_MASK)
#define SERVICE_IS_NS_ACCESSIBLE(flag)          \
    ((flag) & SERVICE_FLAG_NS_ACCESSIBLE)
#define SERVICE_IS_STATELESS(flag)              \
    ((flag) & SERVICE_FLAG_STATELESS)
#define SERVICE_GET_VERSION_POLICY(flag)        \
    ((flag) & SERVICE_FLAG_VERSION_POLICY_BIT)
#define SERVICE_ENABLED_MM_IOVEC(flag)          \
    ((flag) & SERVICE_FLAG_MM_IOVEC)

#define STRID_TO_STRING_PTR(strid)              (const char *)(strid)
#define STRING_PTR_TO_STRID(str)                (uintptr_t)(str)

/* Common service structure type */
struct service_load_info_t {
    uintptr_t       name_strid;         /* String ID for name               */
    uint32_t        sid;                /* Service ID                       */
    uint32_t        flags;              /* Flags                            */
    uint32_t        version;            /* Service version                  */
    uintptr_t       sfn;                /* Secure Function                  */
#if CONFIG_TFM_SPM_BACKEND_IPC == 1
    psa_signal_t    signal;             /* Service signal                   */
#endif
} __attribute__((aligned(4)));

#endif /* __SERVICE_DEFS_H__ */
