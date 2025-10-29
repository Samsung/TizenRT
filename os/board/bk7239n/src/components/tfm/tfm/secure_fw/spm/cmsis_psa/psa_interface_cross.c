/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include "compiler_ext_defs.h"
#include "ffm/psa_api.h"
#include "spm_ipc.h"
#include "svc_num.h"
#include "tfm_psa_call_pack.h"
#include "psa/client.h"
#include "psa/lifecycle.h"
#include "psa/service.h"

#if defined(__ICCARM__)

#pragma required = spm_interface_cross_dispatcher

#endif

/* Grab all functions here in one section to avoid fail in long jump */
__used
__naked
__section(".psa_interface_cross_call")
static uint32_t psa_interface_cross_unified_entry(uint32_t a0)
{
    __asm volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                    \n"
#endif
        "movs   r2, #1                                      \n"
        "bl     spm_interface_cross_dispatcher              \n"
        "pop    {r0-r4, pc}                                 \n"
    );
}

__naked
__section(".psa_interface_cross_call")
uint32_t psa_framework_version_cross(void)
{
    __asm volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                    \n"
#endif
        "push   {r0-r4, lr}                                 \n"
        "ldr    r0, =tfm_spm_client_psa_framework_version   \n"
        "mov    r1, sp                                      \n"
        "b      psa_interface_cross_unified_entry           \n"
    );
}

__naked
__section(".psa_interface_cross_call")
uint32_t psa_version_cross(uint32_t sid)
{
    __asm volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                    \n"
#endif
        "push   {r0-r4, lr}                                 \n"
        "ldr    r0, =tfm_spm_client_psa_version             \n"
        "mov    r1, sp                                      \n"
        "b      psa_interface_cross_unified_entry           \n"
    );
}

__naked
__section(".psa_interface_cross_call")
psa_status_t tfm_psa_call_pack_cross(psa_handle_t handle,
                                     uint32_t ctrl_param,
                                     const psa_invec *in_vec,
                                     psa_outvec *out_vec)
{
    __asm volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                    \n"
#endif
        "push   {r0-r4, lr}                                 \n"
        "ldr    r0, =tfm_spm_client_psa_call                \n"
        "mov    r1, sp                                      \n"
        "b      psa_interface_cross_unified_entry           \n"
    );
}

__naked
__section(".psa_interface_cross_call")
psa_signal_t psa_wait_cross(psa_signal_t signal_mask, uint32_t timeout)
{
    __asm volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                    \n"
#endif
        "push   {r0-r4, lr}                                 \n"
        "ldr    r0, =tfm_spm_partition_psa_wait             \n"
        "mov    r1, sp                                      \n"
        "b      psa_interface_cross_unified_entry           \n"
    );
}

__naked
__section(".psa_interface_cross_call")
psa_status_t psa_get_cross(psa_signal_t signal, psa_msg_t *msg)
{
    __asm volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                    \n"
#endif
        "push   {r0-r4, lr}                                 \n"
        "ldr    r0, =tfm_spm_partition_psa_get              \n"
        "mov    r1, sp                                      \n"
        "b      psa_interface_cross_unified_entry           \n"
    );
}

__naked
__section(".psa_interface_cross_call")
size_t psa_read_cross(psa_handle_t msg_handle, uint32_t invec_idx,
                      void *buffer, size_t num_bytes)
{
    __asm volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                    \n"
#endif
        "push   {r0-r4, lr}                                 \n"
        "ldr    r0, =tfm_spm_partition_psa_read             \n"
        "mov    r1, sp                                      \n"
        "b      psa_interface_cross_unified_entry           \n"
    );
}

__naked
__section(".psa_interface_cross_call")
size_t psa_skip_cross(psa_handle_t msg_handle,
                      uint32_t invec_idx, size_t num_bytes)
{
    __asm volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                    \n"
#endif
        "push   {r0-r4, lr}                                 \n"
        "ldr    r0, =tfm_spm_partition_psa_skip             \n"
        "mov    r1, sp                                      \n"
        "b      psa_interface_cross_unified_entry           \n"
    );
}

__naked
__section(".psa_interface_cross_call")
void psa_write_cross(psa_handle_t msg_handle, uint32_t outvec_idx,
                     const void *buffer, size_t num_bytes)
{
    __asm volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                    \n"
#endif
        "push   {r0-r4, lr}                                 \n"
        "ldr    r0, =tfm_spm_partition_psa_write            \n"
        "mov    r1, sp                                      \n"
        "b      psa_interface_cross_unified_entry           \n"
    );
}

__naked
__section(".psa_interface_cross_call")
void psa_reply_cross(psa_handle_t msg_handle, psa_status_t status)
{
    __asm volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                    \n"
#endif
        "push   {r0-r4, lr}                                 \n"
        "ldr    r0, =tfm_spm_partition_psa_reply            \n"
        "mov    r1, sp                                      \n"
        "b      psa_interface_cross_unified_entry           \n"
    );
}

#if CONFIG_TFM_DOORBELL_API == 1
__naked
__section(".psa_interface_cross_call")
void psa_notify_cross(int32_t partition_id)
{
    __asm volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                    \n"
#endif
        "push   {r0-r4, lr}                                 \n"
        "ldr    r0, =tfm_spm_partition_psa_notify           \n"
        "mov    r1, sp                                      \n"
        "b      psa_interface_cross_unified_entry           \n"
    );
}

__naked
__section(".psa_interface_cross_call")
void psa_clear_cross(void)
{
    __asm volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                    \n"
#endif
        "push   {r0-r4, lr}                                 \n"
        "ldr    r0, =tfm_spm_partition_psa_clear            \n"
        "mov    r1, sp                                      \n"
        "b      psa_interface_cross_unified_entry           \n"
    );
}
#endif /* CONFIG_TFM_DOORBELL_API == 1 */

__naked
__section(".psa_interface_cross_call")
void psa_panic_cross(void)
{
    __asm volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                    \n"
#endif
        "push   {r0-r4, lr}                                 \n"
        "ldr    r0, =tfm_spm_partition_psa_panic            \n"
        "mov    r1, sp                                      \n"
        "b      psa_interface_cross_unified_entry           \n"
    );
}

__naked
__section(".psa_interface_cross_call")
uint32_t psa_rot_lifecycle_state_cross(void)
{
    __asm volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                    \n"
#endif
        "push   {r0-r4, lr}                                 \n"
        "ldr    r0, =tfm_spm_get_lifecycle_state            \n"
        "mov    r1, sp                                      \n"
        "b      psa_interface_cross_unified_entry           \n"
    );
}

/* Following PSA APIs are only needed by connection-based services */
#if CONFIG_TFM_CONNECTION_BASED_SERVICE_API == 1

__naked
__section(".psa_interface_cross_call")
psa_handle_t psa_connect_cross(uint32_t sid, uint32_t version)
{
    __asm volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                    \n"
#endif
        "push   {r0-r4, lr}                                 \n"
        "ldr    r0, =tfm_spm_client_psa_connect             \n"
        "mov    r1, sp                                      \n"
        "b      psa_interface_cross_unified_entry           \n"
    );
}

__naked
__section(".psa_interface_cross_call")
void psa_close_cross(psa_handle_t handle)
{
    __asm volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                    \n"
#endif
        "push   {r0-r4, lr}                                 \n"
        "ldr    r0, =tfm_spm_client_psa_close               \n"
        "mov    r1, sp                                      \n"
        "b      psa_interface_cross_unified_entry           \n"
    );
}

__naked
__section(".psa_interface_cross_call")
void psa_set_rhandle_cross(psa_handle_t msg_handle, void *rhandle)
{
    __asm volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                    \n"
#endif
        "push   {r0-r4, lr}                                 \n"
        "ldr    r0, =tfm_spm_partition_psa_set_rhandle      \n"
        "mov    r1, sp                                      \n"
        "b      psa_interface_cross_unified_entry           \n"
    );
}

#endif /* CONFIG_TFM_CONNECTION_BASED_SERVICE_API */

#if CONFIG_TFM_FLIH_API == 1 || CONFIG_TFM_SLIH_API == 1
__naked
__section(".psa_interface_cross_call")
void psa_irq_enable_cross(psa_signal_t irq_signal)
{
    __asm volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                    \n"
#endif
        "push   {r0-r4, lr}                                 \n"
        "ldr    r0, =tfm_spm_partition_psa_irq_enable       \n"
        "mov    r1, sp                                      \n"
        "b      psa_interface_cross_unified_entry           \n"
    );
}

__naked
__section(".psa_interface_cross_call")
psa_irq_status_t psa_irq_disable_cross(psa_signal_t irq_signal)
{
    __asm volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                    \n"
#endif
        "push   {r0-r4, lr}                                 \n"
        "ldr    r0, =tfm_spm_partition_psa_irq_disable      \n"
        "mov    r1, sp                                      \n"
        "b      psa_interface_cross_unified_entry           \n"
    );
}

/* This API is only used for FLIH. */
#if CONFIG_TFM_FLIH_API == 1
__naked
__section(".psa_interface_cross_call")
void psa_reset_signal_cross(psa_signal_t irq_signal)
{
    __asm volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                    \n"
#endif
        "push   {r0-r4, lr}                                 \n"
        "ldr    r0, =tfm_spm_partition_psa_reset_signal     \n"
        "mov    r1, sp                                      \n"
        "b      psa_interface_cross_unified_entry           \n"
    );
}
#endif /* CONFIG_TFM_FLIH_API == 1 */

/* This API is only used for SLIH. */
#if CONFIG_TFM_SLIH_API == 1
__naked
__section(".psa_interface_cross_call")
void psa_eoi_cross(psa_signal_t irq_signal)
{
    __asm volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                    \n"
#endif
        "push   {r0-r4, lr}                                 \n"
        "ldr    r0, =tfm_spm_partition_psa_eoi              \n"
        "mov    r1, sp                                      \n"
        "b      psa_interface_cross_unified_entry           \n"
    );
}
#endif /* CONFIG_TFM_SLIH_API */
#endif /* CONFIG_TFM_FLIH_API == 1 || CONFIG_TFM_SLIH_API == 1 */

#if PSA_FRAMEWORK_HAS_MM_IOVEC

__naked
__section(".psa_interface_cross_call")
const void *psa_map_invec_cross(psa_handle_t msg_handle, uint32_t invec_idx)
{
    __asm volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                    \n"
#endif
        "push   {r0-r4, lr}                                 \n"
        "ldr    r0, =tfm_spm_partition_psa_map_invec        \n"
        "mov    r1, sp                                      \n"
        "b      psa_interface_cross_unified_entry           \n"
    );
}

__naked
__section(".psa_interface_cross_call")
void psa_unmap_invec_cross(psa_handle_t msg_handle, uint32_t invec_idx)
{
    __asm volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                    \n"
#endif
        "push   {r0-r4, lr}                                 \n"
        "ldr    r0, =tfm_spm_partition_psa_unmap_invec      \n"
        "mov    r1, sp                                      \n"
        "b      psa_interface_cross_unified_entry           \n"
    );
}

__naked
__section(".psa_interface_cross_call")
void *psa_map_outvec_cross(psa_handle_t msg_handle, uint32_t outvec_idx)
{
    __asm volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                    \n"
#endif
        "push   {r0-r4, lr}                                 \n"
        "ldr    r0, =tfm_spm_partition_psa_map_outvec       \n"
        "mov    r1, sp                                      \n"
        "b      psa_interface_cross_unified_entry           \n"
    );
}

__naked
__section(".psa_interface_cross_call")
void psa_unmap_outvec_cross(psa_handle_t msg_handle, uint32_t outvec_idx,
                            size_t len)
{
    __asm volatile(
#if !defined(__ICCARM__)
        ".syntax unified                                    \n"
#endif
        "push   {r0-r4, lr}                                 \n"
        "ldr    r0, =tfm_spm_partition_psa_unmap_outvec     \n"
        "mov    r1, sp                                      \n"
        "b      psa_interface_cross_unified_entry           \n"
    );
}

#endif /* PSA_FRAMEWORK_HAS_MM_IOVEC */
