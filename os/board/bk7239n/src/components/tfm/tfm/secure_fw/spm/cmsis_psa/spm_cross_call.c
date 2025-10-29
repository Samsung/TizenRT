/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include "config_impl.h"
#include "compiler_ext_defs.h"
#include "current.h"
#include "spm_ipc.h"
#include "tfm_arch.h"
#include "ffm/backend.h"
#include "ffm/psa_api.h"

typedef psa_status_t (*target_fn_t)(uint32_t a0, uint32_t a1,
                                    uint32_t a2, uint32_t a3);

/* Executes with interrupt unmasked, scheduler locked. */
__used
psa_status_t cross_call_entering_c(uintptr_t fn_addr, uintptr_t frame_addr)
{
    psa_status_t                  status;
    struct partition_t            *caller;
    struct cross_call_abi_frame_t *p_frame =
                                  (struct cross_call_abi_frame_t *)frame_addr;

    /* Indicate ongoing cross call context for return value collecting. */
    caller = GET_CURRENT_COMPONENT();
    caller->ctx_ctrl.cross_frame    = (uint32_t)frame_addr;
    caller->ctx_ctrl.retcode_status = CROSS_RETCODE_EMPTY;

    status = ((target_fn_t)fn_addr)(p_frame->a0, p_frame->a1,
                                    p_frame->a2, p_frame->a3);

    spm_handle_programmer_errors(status);

    return status;
}

/* Executes with interrupt masked. */
__used
void cross_call_exiting_c(psa_status_t status, uintptr_t frame_addr)
{
    struct partition_t            *caller  = GET_CURRENT_COMPONENT();
    struct cross_call_abi_frame_t *p_frame =
                                  (struct cross_call_abi_frame_t *)frame_addr;

    /* Write the status into frame as the default return value. */
    if (caller->ctx_ctrl.retcode_status == CROSS_RETCODE_EMPTY) {
        p_frame->a0 = (uint32_t)status;
    }

    /* Interrupt is masked, PendSV will not happen immediately. */
    if (THRD_EXPECTING_SCHEDULE()) {
        tfm_arch_trigger_pendsv();
    }
}

__used
void spm_interface_cross_dispatcher(uintptr_t fn_addr,
                                    uintptr_t frame_addr,
                                    uint32_t  switch_stack)
{
    if (__get_active_exc_num() != EXC_NUM_THREAD_MODE) {
        /* PSA APIs must be called from Thread mode */
        tfm_core_panic();
    }

    arch_non_preempt_call(fn_addr, frame_addr,
                          switch_stack ? SPM_THREAD_CONTEXT->sp : 0,
                          switch_stack ? SPM_THREAD_CONTEXT->sp_limit : 0);
    /*
     * No need to clear cross_frame and retcode_status because execution here
     * indicates the current thread is running, subsequent interrupts
     * won't update return value to a running context, they only update
     * return value for blocked Partitions.
     */
}
