/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 * Copyright (c) 2021, Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include "critical_section.h"
#include "compiler_ext_defs.h"
#include "runtime_defs.h"
#include "spm_ipc.h"
#include "tfm_hal_isolation.h"
#include "tfm_hal_platform.h"
#include "tfm_rpc.h"
#include "ffm/backend.h"
#include "utilities.h"
#include "load/partition_defs.h"
#include "load/service_defs.h"
#include "load/spm_load_api.h"
#include "psa/error.h"

/* Declare the global component list */
struct partition_head_t partition_listhead;

#if CONFIG_TFM_PSA_API_CROSS_CALL == 1

#ifdef TFM_MULTI_CORE_TOPOLOGY
/* TODO: To be checked when RPC design updates. */
static uint8_t spm_stack_local[CONFIG_TFM_SPM_THREAD_STACK_SIZE] __aligned(8);
struct context_ctrl_t spm_thread_context = {
    .sp        = (uint32_t)&spm_stack_local[CONFIG_TFM_SPM_THREAD_STACK_SIZE],
    .sp_limit  = (uint32_t)spm_stack_local,
    .allocated = 0,
    .exc_ret   = 0,
};
struct context_ctrl_t *p_spm_thread_context = &spm_thread_context;
#else
struct context_ctrl_t *p_spm_thread_context;
#endif

#endif

static void prv_process_metadata(struct partition_t *p_pt)
{
    const struct partition_load_info_t *p_pt_ldi;
    const struct service_load_info_t *p_srv_ldi;
    struct context_ctrl_t *ctx_ctrl;
    struct runtime_metadata_t *p_rt_meta;
    service_fn_t *p_sfn_table;
    uint32_t allocate_size;

    p_pt_ldi = p_pt->p_ldinf;
    p_srv_ldi = (struct service_load_info_t *)LOAD_INFO_SERVICE(p_pt_ldi);
    ctx_ctrl = &p_pt->ctx_ctrl;

    /* common runtime metadata */
    allocate_size = sizeof(*p_rt_meta);

    if (!IS_PARTITION_IPC_MODEL(p_pt_ldi)) {
        /* SFN specific metadata - SFN function table */
        allocate_size += sizeof(service_fn_t) * p_pt_ldi->nservices;
    }

    ARCH_CTXCTRL_ALLOCATE_STACK(ctx_ctrl, allocate_size);
    p_rt_meta = (struct runtime_metadata_t *)
                                    ARCH_CTXCTRL_ALLOCATED_PTR(ctx_ctrl);

    p_rt_meta->entry = p_pt_ldi->entry;
    p_rt_meta->n_sfn = 0;
    p_sfn_table = p_rt_meta->sfn_table;

    if (!IS_PARTITION_IPC_MODEL(p_pt_ldi)) {
        /* SFN table. The signal bit of the service is the same index of SFN. */
        for (int i = 0; i < p_pt_ldi->nservices; i++) {
            p_sfn_table[i] = (service_fn_t)p_srv_ldi[i].sfn;
        }

        p_rt_meta->n_sfn = p_pt_ldi->nservices;
    }

    p_pt->p_metadata = (void *)p_rt_meta;
}

/*
 * Send message and wake up the SP who is waiting on message queue, block the
 * current thread and trigger scheduler.
 */
static psa_status_t ipc_messaging(struct service_t *service,
                                  struct conn_handle_t *handle)
{
    struct partition_t *p_owner = NULL;
    psa_signal_t signal = 0;
    struct critical_section_t cs_assert = CRITICAL_SECTION_STATIC_INIT;

    if (!handle || !service || !service->p_ldinf || !service->partition) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    p_owner = service->partition;
    signal = service->p_ldinf->signal;

    CRITICAL_SECTION_ENTER(cs_assert);

    UNI_LIST_INSERT_AFTER(p_owner, handle, p_handles);

    /* Messages put. Update signals */
    p_owner->signals_asserted |= signal;

    if (p_owner->signals_waiting & signal) {
        thrd_wake_up(&p_owner->waitobj,
                     (p_owner->signals_asserted & p_owner->signals_waiting));
        p_owner->signals_waiting &= ~signal;
    }
    CRITICAL_SECTION_LEAVE(cs_assert);

    /*
     * If it is a NS request via RPC, it is unnecessary to block current
     * thread.
     */

    if (!is_tfm_rpc_msg(handle)) {
        thrd_set_wait(&handle->ack_evnt, CURRENT_THREAD);
    }

    handle->status = TFM_HANDLE_STATUS_ACTIVE;

    return PSA_SUCCESS;
}

static psa_status_t ipc_replying(struct conn_handle_t *handle, int32_t status)
{
    if (is_tfm_rpc_msg(handle)) {
        tfm_rpc_client_call_reply(handle, status);
    } else {
        thrd_wake_up(&handle->ack_evnt, status);
    }

    /*
     * 'psa_reply' exists in IPC model only and returns 'void'. Return
     * 'PSA_SUCCESS' here always since SPM does not forward the status
     * to the caller.
     */
    return PSA_SUCCESS;
}

extern void sprt_main(void);

/* Parameters are treated as assuredly */
static void ipc_comp_init_assuredly(struct partition_t *p_pt,
                                    uint32_t service_setting)
{
    const struct partition_load_info_t *p_pldi = p_pt->p_ldinf;

#if CONFIG_TFM_DOORBELL_API == 1
    p_pt->signals_allowed |= PSA_DOORBELL;
#endif /* CONFIG_TFM_DOORBELL_API == 1 */

    p_pt->signals_allowed |= service_setting;

    THRD_SYNC_INIT(&p_pt->waitobj);
    UNI_LISI_INIT_NODE(p_pt, p_handles);

    ARCH_CTXCTRL_INIT(&p_pt->ctx_ctrl,
                      LOAD_ALLOCED_STACK_ADDR(p_pldi),
                      p_pldi->stack_size);

    prv_process_metadata(p_pt);

    THRD_INIT(&p_pt->thrd, &p_pt->ctx_ctrl,
              TO_THREAD_PRIORITY(PARTITION_PRIORITY(p_pldi->flags)));

#if (CONFIG_TFM_PSA_API_CROSS_CALL == 1) && !defined(TFM_MULTI_CORE_TOPOLOGY)
    if (p_pldi->pid == TFM_SP_NON_SECURE_ID) {
        SPM_THREAD_CONTEXT = &p_pt->ctx_ctrl;
    }
#endif

	p_pt->thrd.name = p_pt->p_ldinf->name; // todo wangzhilei
    thrd_start(&p_pt->thrd,
               POSITION_TO_ENTRY(sprt_main, thrd_fn_t),
               THRD_GENERAL_EXIT);
}

static uint32_t ipc_system_run(void)
{
    uint32_t control;
    struct partition_t *p_cur_pt;

#if CONFIG_TFM_PSA_API_THREAD_CALL == 1
    TFM_CORE_ASSERT(SPM_THREAD_CONTEXT);
#endif

    control = thrd_start_scheduler(&CURRENT_THREAD);

    p_cur_pt = TO_CONTAINER(CURRENT_THREAD->p_context_ctrl,
                            struct partition_t, ctx_ctrl);

    if (tfm_hal_update_boundaries(p_cur_pt->p_ldinf, p_cur_pt->p_boundaries)
            != TFM_HAL_SUCCESS) {
        tfm_core_panic();
    }

    return control;
}

static psa_signal_t ipc_wait(struct partition_t *p_pt, psa_signal_t signal_mask)
{
    struct critical_section_t cs_assert = CRITICAL_SECTION_STATIC_INIT;
    psa_signal_t ret_signal;

    /*
     * 'ipc_wait()' sets the waiting signal mask for partition, and
     * blocks the partition thread state to wait for signals.
     * These changes should be inside the ciritical section to avoid
     * 'signal_waiting' or the thread state to be changed by interrupts
     * while this function is reading or writing values.
     */
    CRITICAL_SECTION_ENTER(cs_assert);

    ret_signal = p_pt->signals_asserted & signal_mask;
    if (ret_signal == 0) {
        p_pt->signals_waiting = signal_mask;
        thrd_set_wait(&p_pt->waitobj, CURRENT_THREAD);
    }
    CRITICAL_SECTION_LEAVE(cs_assert);

    return ret_signal;
}

static void ipc_wake_up(struct partition_t *p_pt)
{
    thrd_wake_up(&p_pt->waitobj,
                 p_pt->signals_asserted & p_pt->signals_waiting);
    p_pt->signals_waiting = 0;
}

const struct backend_ops_t backend_instance = {
    .comp_init_assuredly = ipc_comp_init_assuredly,
    .system_run          = ipc_system_run,
    .messaging           = ipc_messaging,
    .replying            = ipc_replying,
    .wait                = ipc_wait,
    .wake_up             = ipc_wake_up,
};
