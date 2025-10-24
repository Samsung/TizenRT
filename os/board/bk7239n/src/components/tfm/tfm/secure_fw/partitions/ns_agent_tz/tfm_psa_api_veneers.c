/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdbool.h>
#include <stdio.h>
#include "config_impl.h"
#include "security_defs.h"
#include "svc_num.h"
#include "utilities.h"
#include "tfm_arch.h"
#include "tfm_psa_call_pack.h"
#include "tfm_secure_api.h"

#if CONFIG_TFM_PSA_API_CROSS_CALL == 1
#include "spm_ipc.h"
#include "ffm/psa_api.h"
#endif

/*
 * This is the veneers for FF-M Client APIs. The interfaces are written
 * in assembly, and the reasons:
 *
 * - On the 8.0 version of Armv8-M with security extension, a mandatory
 *   software solution needs to be applied because hardware reentrant
 *   detection was not involved until 8.1. It requires TF-M to take full
 *   control of stack usage to perform reentrant detection. Therefore
 *   TF-M shall implement these veneers in the assembly code.
 *
 * - The non-secure context is pushed in the stack. When SPM API returns,
 *   the pushed non-secure context is popped and overrides the returned
 *   context before returning to NSPE. Therefore it is unnecessary to
 *   explicitly clean up the context.
 */

#if defined(__ICCARM__)

#if CONFIG_TFM_PSA_API_CROSS_CALL == 1

#pragma required = tfm_spm_client_psa_framework_version
#pragma required = tfm_spm_client_psa_version
#pragma required = tfm_spm_client_psa_call
#pragma required = spm_interface_cross_dispatcher
/* Following PSA APIs are only needed by connection-based services */
#if CONFIG_TFM_CONNECTION_BASED_SERVICE_API == 1
#pragma required = tfm_spm_client_psa_connect
#pragma required = tfm_spm_client_psa_close
#endif /* CONFIG_TFM_CONNECTION_BASED_SERVICE_API */

#endif /* CONFIG_TFM_PSA_API_CROSS_CALL == 1 */

#endif

__tfm_psa_secure_gateway_attributes__
uint32_t tfm_psa_framework_version_veneer(void)
{
    __ASM volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                      \n"
#endif

#if !defined(__ARM_ARCH_8_1M_MAIN__)
        "   ldr    r2, [sp]                                   \n"
        "   ldr    r3, ="M2S(STACK_SEAL_PATTERN)"             \n"
        "   cmp    r2, r3                                     \n"
        "   bne    reent_panic1                               \n"
#endif
        "   mrs    r3, control                                \n"
        "   push   {r2, r3}                                   \n"
#if CONFIG_TFM_PSA_API_CROSS_CALL == 1
        "   push   {r0-r4, lr}                                \n"
        "   ldr    r0, =tfm_spm_client_psa_framework_version  \n"
        "   mov    r1, sp                                     \n"
        "   movs   r2, #0                                     \n"
        "   bl     spm_interface_cross_dispatcher             \n"
        "   pop    {r0-r3}                                    \n"
        "   pop    {r2, r3}                                   \n"
        "   mov    lr, r3                                     \n"
#elif CONFIG_TFM_PSA_API_SFN_CALL == 1
        "   push   {r4, lr}                                   \n"
        "   bl     psa_framework_version_sfn                  \n"
        "   pop    {r2, r3}                                   \n"
        "   mov    lr, r3                                     \n"
#else
        "   svc    "M2S(TFM_SVC_PSA_FRAMEWORK_VERSION)"       \n"
#endif
        "   pop    {r2, r3}                                   \n"
        "   msr    control, r3                                \n"
        "   bxns   lr                                         \n"
#if !defined(__ARM_ARCH_8_1M_MAIN__)
        "reent_panic1:                                        \n"
        "   svc    "M2S(TFM_SVC_PSA_PANIC)"                   \n"
        "   b      .                                          \n"
#endif
    );
}

__tfm_psa_secure_gateway_attributes__
uint32_t tfm_psa_version_veneer(uint32_t sid)
{
    __ASM volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                      \n"
#endif

#if !defined(__ARM_ARCH_8_1M_MAIN__)
        "   ldr    r2, [sp]                                   \n"
        "   ldr    r3, ="M2S(STACK_SEAL_PATTERN)"             \n"
        "   cmp    r2, r3                                     \n"
        "   bne    reent_panic2                               \n"
#endif
        "   mrs    r3, control                                \n"
        "   push   {r2, r3}                                   \n"
#if CONFIG_TFM_PSA_API_CROSS_CALL == 1
        "   push   {r0-r4, lr}                                \n"
        "   ldr    r0, =tfm_spm_client_psa_version            \n"
        "   mov    r1, sp                                     \n"
        "   movs   r2, #0                                     \n"
        "   bl     spm_interface_cross_dispatcher             \n"
        "   pop    {r0-r3}                                    \n"
        "   pop    {r2, r3}                                   \n"
        "   mov    lr, r3                                     \n"
#elif CONFIG_TFM_PSA_API_SFN_CALL == 1
        "   push   {r4, lr}                                   \n"
        "   bl     psa_version_sfn                            \n"
        "   pop    {r2, r3}                                   \n"
        "   mov    lr, r3                                     \n"
#else
        "   svc    "M2S(TFM_SVC_PSA_VERSION)"                 \n"
#endif
        "   pop    {r2, r3}                                   \n"
        "   msr    control, r3                                \n"
        "   bxns   lr                                         \n"
#if !defined(__ARM_ARCH_8_1M_MAIN__)
        "reent_panic2:                                        \n"
        "   svc    "M2S(TFM_SVC_PSA_PANIC)"                   \n"
        "   b      .                                          \n"
#endif
    );
}

__tfm_psa_secure_gateway_attributes__
psa_status_t tfm_psa_call_veneer(psa_handle_t handle,
                                 uint32_t ctrl_param,
                                 const psa_invec *in_vec,
                                 psa_outvec *out_vec)
{
    __ASM volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                      \n"
#endif

#if !defined(__ARM_ARCH_8_1M_MAIN__)
        "   push   {r2, r3}                                   \n"
        "   ldr    r2, [sp, #8]                               \n"
        "   ldr    r3, ="M2S(STACK_SEAL_PATTERN)"             \n"
        "   cmp    r2, r3                                     \n"
        "   bne    reent_panic4                               \n"
        "   pop    {r2, r3}                                   \n"
#endif
        "   mov    r12, r3                                    \n"
        "   mrs    r3, control                                \n"
        "   push   {r2, r3}                                   \n"
        "   mov    r3, r12                                    \n"
#if CONFIG_TFM_PSA_API_CROSS_CALL == 1
        "   push   {r0-r4, lr}                                \n"
        "   ldr    r0, =tfm_spm_client_psa_call               \n"
        "   mov    r1, sp                                     \n"
        "   movs   r2, #0                                     \n"
        "   bl     spm_interface_cross_dispatcher             \n"
        "   pop    {r0-r3}                                    \n"
        "   pop    {r2, r3}                                   \n"
        "   mov    lr, r3                                     \n"
#elif CONFIG_TFM_PSA_API_SFN_CALL == 1
        "   push   {r4, lr}                                   \n"
        "   bl     psa_call_pack_sfn                          \n"
        "   pop    {r2, r3}                                   \n"
        "   mov    lr, r3                                     \n"
#else
        "   svc    "M2S(TFM_SVC_PSA_CALL)"                    \n"
#endif
        "   pop    {r2, r3}                                   \n"
        "   msr    control, r3                                \n"
        "   bxns   lr                                         \n"
#if !defined(__ARM_ARCH_8_1M_MAIN__)
        "reent_panic4:                                        \n"
        "   svc    "M2S(TFM_SVC_PSA_PANIC)"                   \n"
        "   b      .                                          \n"
#endif
    );
}

/* Following veneers are only needed by connection-based services */
#if CONFIG_TFM_CONNECTION_BASED_SERVICE_API == 1

__tfm_psa_secure_gateway_attributes__
psa_handle_t tfm_psa_connect_veneer(uint32_t sid, uint32_t version)
{
    __ASM volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                      \n"
#endif

#if !defined(__ARM_ARCH_8_1M_MAIN__)
        "   ldr    r2, [sp]                                   \n"
        "   ldr    r3, ="M2S(STACK_SEAL_PATTERN)"             \n"
        "   cmp    r2, r3                                     \n"
        "   bne    reent_panic3                               \n"
#endif
        "   mrs    r3, control                                \n"
        "   push   {r2, r3}                                   \n"
        "   mov    r3, r12                                    \n"
#if CONFIG_TFM_PSA_API_CROSS_CALL == 1
        "   push   {r0-r4, lr}                                \n"
        "   ldr    r0, =tfm_spm_client_psa_connect            \n"
        "   mov    r1, sp                                     \n"
        "   movs   r2, #0                                     \n"
        "   bl     spm_interface_cross_dispatcher             \n"
        "   pop    {r0-r3}                                    \n"
        "   pop    {r2, r3}                                   \n"
        "   mov    lr, r3                                     \n"
#elif CONFIG_TFM_PSA_API_SFN_CALL == 1
        "   push   {r4, lr}                                   \n"
        "   bl     psa_connect_sfn                            \n"
        "   pop    {r2, r3}                                   \n"
        "   mov    lr, r3                                     \n"
#else
        "   svc    "M2S(TFM_SVC_PSA_CONNECT)"                 \n"
#endif
        "   pop    {r2, r3}                                   \n"
        "   msr    control, r3                                \n"
        "   bxns   lr                                         \n"
#if !defined(__ARM_ARCH_8_1M_MAIN__)
        "reent_panic3:                                        \n"
        "   svc    "M2S(TFM_SVC_PSA_PANIC)"                   \n"
        "   b      .                                          \n"
#endif
    );
}

__tfm_psa_secure_gateway_attributes__
void tfm_psa_close_veneer(psa_handle_t handle)
{
    __ASM volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                      \n"
#endif

#if !defined(__ARM_ARCH_8_1M_MAIN__)
        "   ldr    r2, [sp]                                   \n"
        "   ldr    r3, ="M2S(STACK_SEAL_PATTERN)"             \n"
        "   cmp    r2, r3                                     \n"
        "   bne    reent_panic5                               \n"
#endif
        "   mrs    r3, control                                \n"
        "   push   {r2, r3}                                   \n"
#if CONFIG_TFM_PSA_API_CROSS_CALL == 1
        "   push   {r0-r4, lr}                                \n"
        "   ldr    r0, =tfm_spm_client_psa_close              \n"
        "   mov    r1, sp                                     \n"
        "   movs   r2, #0                                     \n"
        "   bl     spm_interface_cross_dispatcher             \n"
        "   pop    {r0-r3}                                    \n"
        "   pop    {r2, r3}                                   \n"
        "   mov    lr, r3                                     \n"
#elif CONFIG_TFM_PSA_API_SFN_CALL == 1
        "   push   {r4, lr}                                   \n"
        "   bl     psa_close_sfn                              \n"
        "   pop    {r2, r3}                                   \n"
        "   mov    lr, r3                                     \n"
#else
        "   svc    "M2S(TFM_SVC_PSA_CLOSE)"                   \n"
#endif
        "   pop    {r2, r3}                                   \n"
        "   msr    control, r3                                \n"
        "   bxns   lr                                         \n"
#if !defined(__ARM_ARCH_8_1M_MAIN__)
        "reent_panic5:                                        \n"
        "   svc    "M2S(TFM_SVC_PSA_PANIC)"                   \n"
        "   b      .                                          \n"
#endif
    );
}

#endif /* CONFIG_TFM_CONNECTION_BASED_SERVICE_API */
