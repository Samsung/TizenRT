/*
 * Copyright (c) 2017-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_SECURE_API_H__
#define __TFM_SECURE_API_H__

#ifndef TFM_MULTI_CORE_TOPOLOGY
#include <arm_cmse.h>
#endif
#include "tfm_arch.h"
#include "tfm_api.h"
#include "utilities.h"
#include "tfm_boot_status.h"
#include "psa/service.h"
#ifdef TFM_PSA_API
#include "svc_num.h"
#else
#include "tfm_core_svc.h"
#endif /* TFM_PSA_API */

#ifndef TFM_MULTI_CORE_TOPOLOGY
#ifndef TFM_PSA_API
/*!
 * \def __tfm_secure_gateway_attributes__
 *
 * \brief Attributes for secure gateway functions
 */
#if defined(__GNUC__) && !defined(__ARMCC_VERSION)
/*
 * GNUARM requires noclone attribute to protect gateway function symbol from
 * being renamed and cloned
 */
#define __tfm_secure_gateway_attributes__ \
        __attribute__((cmse_nonsecure_entry, noclone, section("SFN")))
#else
#define __tfm_secure_gateway_attributes__ \
        __attribute__((cmse_nonsecure_entry, section("SFN")))
#endif /* __GNUC__ && !__ARMCC_VERSION */
#endif /* !TFM_PSA_API */

#ifdef TFM_PSA_API
/*!
 * \def __tfm_psa_secure_gateway_attributes__
 *
 * \brief Attributes for psa api secure gateway functions
 */
#if defined(__GNUC__) && !defined(__ARMCC_VERSION)
/*
 * GNUARM requires noclone attribute to protect gateway function symbol from
 * being renamed and cloned
 */
#define __tfm_psa_secure_gateway_attributes__ \
        __attribute__((cmse_nonsecure_entry, noclone, naked, section("SFN")))

#define __tfm_psa_secure_gateway_no_naked_attributes__ \
        __attribute__((cmse_nonsecure_entry, noclone, section("SFN")))
#else
#define __tfm_psa_secure_gateway_attributes__ \
        __attribute__((cmse_nonsecure_entry, naked, section("SFN")))
#endif /* __GNUC__ && !__ARMCC_VERSION */
#endif /* TFM_PSA_API */
#endif /* TFM_MULTI_CORE_TOPOLOGY */

/* Hide specific errors if not debugging */
#ifdef TFM_CORE_DEBUG
#define TFM_ERROR_STATUS(status) (status)
#else
#define TFM_ERROR_STATUS(status) (TFM_PARTITION_BUSY)
#endif

#ifndef TFM_LVL
#error TFM_LVL is not defined!
#endif

#ifndef TFM_PSA_API
extern void tfm_secure_api_error_handler(void);

typedef int32_t(*sfn_t)(int32_t, int32_t, int32_t, int32_t);

struct tfm_sfn_req_s {
    uint32_t sp_id;
    sfn_t sfn;
    int32_t *args;
    uint32_t caller_part_idx;
    bool ns_caller;
};

enum tfm_status_e tfm_core_get_caller_client_id(int32_t *caller_client_id);

int32_t tfm_core_sfn_request(const struct tfm_sfn_req_s *desc_ptr);

int32_t tfm_spm_sfn_request_thread_mode(struct tfm_sfn_req_s *desc_ptr);

#define TFM_CORE_IOVEC_SFN_REQUEST(id, is_ns, fn, a, b, c, d)        \
        return tfm_core_partition_request(id, is_ns, fn,             \
                (int32_t)a, (int32_t)b, (int32_t)c, (int32_t)d)

int32_t tfm_core_partition_request(uint32_t id, bool is_ns, void *fn,
            int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);

__attribute__ ((always_inline)) __STATIC_INLINE
bool tfm_core_is_ns_client(void)
{
    /*
     * This preprocessor condition checks if a version of GCC smaller than
     * 7.3.1 is being used to compile the code.
     * These versions are affected by a bug on the cmse_nonsecure_caller
     * intrinsic which returns incorrect results.
     * Please check Bug 85203 on GCC Bugzilla for more information.
     */
#if defined(__GNUC__) && !defined(__ARMCC_VERSION) && \
    (__GNUC__ < 7 || \
     (__GNUC__ == 7 && (__GNUC_MINOR__ < 3 || \
                       (__GNUC_MINOR__ == 3 && __GNUC_PATCHLEVEL__ < 1))))
    /*
     * Use the fact that, if called from Non-Secure, the LSB of the return
     * address is set to 0.
     */
    return !(
          (uintptr_t)__builtin_extract_return_addr(__builtin_return_address(0U))
          & 0x1);
#else
    /*
     * Convert the result of cmse_nonsecure_caller from an int to a bool
     * to prevent using an int in the tfm_sfn_req_s structure.
     */
    return (cmse_nonsecure_caller() != 0) ? true : false;
#endif /* Check for GCC compiler version smaller than 7.3.1 */
}
#endif

#endif /* __TFM_SECURE_API_H__ */
