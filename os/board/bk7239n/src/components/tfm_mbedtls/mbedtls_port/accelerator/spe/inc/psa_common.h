/*
 * Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
 * All rights reserved
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */

#ifndef __PSA_COMMON_H__
#define __PSA_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

typedef struct psa_invec {
    const void *base;
    size_t len;
} psa_invec;

typedef struct psa_outvec {
    void *base;
    size_t len;
} psa_outvec;

typedef int32_t psa_status_t;
typedef int32_t psa_handle_t;
typedef struct psa_invec psa_invec;
typedef struct psa_outvec psa_outvec;

#define PSA_FRAMEWORK_VERSION               (0x0100)
#define PSA_VERSION_NONE                    (0)

#define PSA_NULL_HANDLE                     ((psa_handle_t)0)

#define PSA_CONNECTION_ACCEPTED             (0)
#define PSA_DROP_CONNECTION                 (INT32_MIN)
#define PSA_CONNECTION_REFUSED              (INT32_MIN + 1)
#define PSA_CONNECTION_BUSY                 (INT32_MIN + 2)
#define PSA_ERR_NOMSG                       (INT32_MIN + 3)

#if !defined(PSA_SUCCESS)
#define PSA_SUCCESS                         (0)
#endif
/* SPM vendor define */
#define PSA_ERR_OUT_OF_MEMORY               (-128)
#define PSA_ERR_BAD_PARAMETER               (-129)
#define PSA_ERR_NULL_POINTER                (-130)
#define PSA_ERR_GENERIC                     (-131)
#define PSA_ERR_BAD_MSG                     (-132)
#define PSA_ERR_INVALID_HANDLE              (-133)
#define PSA_ERR_MEMORY_DENY                 (-134)
#define PSA_ERR_NOT_SUPPORT                 (-135)
#define PSA_ERR_VECTOR_OVERLAY              (-136)

#define PSA_ERR_INTERNAL_FATAL              (-200)
#define PSA_ERR_GENERIC_HW_FAIL             (-201)

#define PSA_ERR_IGNORE_SIGNLAL              (-300)

#define PSA_MAX_IOVEC                       (4)

#ifdef __cplusplus
}
#endif

#endif /* __PSA_COMMON_H__ */

