/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "cmsis_compiler.h"
#include "tfm_ns_client_ext.h"
#include "nsid_svc_handler.h"

/**
 * \brief Macro to encode an svc instruction
 *
 */
#define SVC(code) __ASM volatile("svc %0" : : "I" (code))

/**
 * \brief Numbers associated to each SVC available
 *
 * \details Start from 1 as 0 is reserved by RTX
 */
enum tfm_svc_num {
    SVC_INVALID = 0,
    /* SVC API for Services */
    SVC_NS_CLIENT_EXT_INIT,
    SVC_NS_CLIENT_ACQUIRE_CTX,
    SVC_NS_CLIENT_RELEASE_CTX,
    SVC_NS_CLIENT_LOAD_CTX,
    SVC_NS_CLIENT_SAVE_CTX,
    /* add all the new entries above this line */
    SVC_TFM_MAX,
};

/* number of user SVC functions */
#define USER_SVC_COUNT ((uint32_t)SVC_TFM_MAX - 1)

/**
 * \brief Modified table template for user defined SVC functions
 *
 * \details RTX has a weak definition of osRtxUserSVC, which
 *          is overridden here
 */
#if (defined(__ARMCC_VERSION) && (__ARMCC_VERSION == 6110004))
/* Workaround needed for a bug in Armclang 6.11, more details at:
 * http://www.keil.com/support/docs/4089.htm
 */
__attribute__((section(".gnu.linkonce")))
#endif
extern void * const osRtxUserSVC[1+USER_SVC_COUNT];
       void * const osRtxUserSVC[1+USER_SVC_COUNT] = {
    (void *)USER_SVC_COUNT,
    /* SVC API for Services */
    (void *)tfm_nsce_init,
    (void *)tfm_nsce_acquire_ctx,
    (void *)tfm_nsce_release_ctx,
    (void *)tfm_nsce_load_ctx,
    (void *)tfm_nsce_save_ctx,
};

__attribute__ ((naked))
uint32_t nsid_ext_init_svc(uint32_t ctx_requested)
{
    SVC(SVC_NS_CLIENT_EXT_INIT);
    __ASM volatile("BX LR");
}

__attribute__ ((naked))
uint32_t nsid_acquire_ctx_svc(uint8_t group_id, uint8_t thread_id)
{
    SVC(SVC_NS_CLIENT_ACQUIRE_CTX);
    __ASM volatile("BX LR");
}

__attribute__ ((naked))
uint32_t nsid_release_ctx_svc(uint32_t token)
{
    SVC(SVC_NS_CLIENT_RELEASE_CTX);
    __ASM volatile("BX LR");
}

__attribute__ ((naked))
uint32_t nsid_load_ctx_svc(uint32_t token, int32_t nsid)
{
    SVC(SVC_NS_CLIENT_LOAD_CTX);
    __ASM volatile("BX LR");
}

__attribute__ ((naked))
uint32_t nsid_save_ctx_svc(uint32_t token)
{
    SVC(SVC_NS_CLIENT_SAVE_CTX);
    __ASM volatile("BX LR");
}
