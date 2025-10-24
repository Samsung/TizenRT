/*
 * Copyright (c) 2017-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <arm_cmse.h>
#include "arch.h"
#include "bitops.h"
#include "fih.h"
#include "tfm_nspm.h"
#include "tfm_api.h"
#include "tfm_arch.h"
#include "tfm_irq_list.h"
#include "psa/service.h"
#include "tfm_core_mem_check.h"
#include "tfm_peripherals_def.h"
#include "tfm_secure_api.h"
#include "tfm_spm_hal.h"
#include "tfm_core_trustzone.h"
#include "spm_func.h"
#include "region_defs.h"
#include "region.h"
#include "spm_partition_defs.h"
#include "psa_manifest/pid.h"
#include "tfm_spm_services.h"
#include "tfm_spm_db_func.inc"

/* Structure to temporarily save iovec parameters from PSA client */
struct iovec_params_t {
    psa_invec in_vec[PSA_MAX_IOVEC];
    size_t in_len;
    psa_outvec out_vec[PSA_MAX_IOVEC];
    size_t out_len;

    psa_outvec *orig_outvec;
};

#define EXC_RETURN_SECURE_FUNCTION 0xFFFFFFFD
#define EXC_RETURN_SECURE_HANDLER  0xFFFFFFF1

#ifndef TFM_LVL
#error TFM_LVL is not defined!
#endif

#ifdef TFM_MULTI_CORE_TOPOLOGY
#error Multi core is not supported by Function mode
#endif

REGION_DECLARE_T(Image$$, TFM_SECURE_STACK, $$ZI$$Base, uint32_t);
REGION_DECLARE_T(Image$$, TFM_SECURE_STACK, $$ZI$$Limit, struct iovec_args_t)[];

static uint32_t *tfm_secure_stack_seal =
    ((uint32_t *)&REGION_NAME(Image$$, TFM_SECURE_STACK, $$ZI$$Limit)[-1]) - 2;

REGION_DECLARE_T(Image$$, ER_INITIAL_PSP_SEAL, $$ZI$$Base, uint32_t)[];

/*
 * Function to seal the psp stacks for Function model of TF-M.
 */
void tfm_spm_seal_psp_stacks(void)
{
    /*
     * The top of TFM_SECURE_STACK is used for iovec parameters, we need to
     * place the seal between iovec parameters and partition stack.
     *
     * Image$$TFM_SECURE_STACK$$ZI$$Limit-> +-------------------------+
     *                                      |                         |
     *                                      | iovec parameters for    |
     *                                      | partition               |
     * (Image$$TFM_SECURE_STACK$$ZI$$Limit -|                         |
     * sizeof(iovec_args_t)) ->             +-------------------------+
     *                                      | Stack Seal              |
     *                                      +-------------------------+
     *                                      |                         |
     *                                      |    Partition stack      |
     *                                      |                         |
     * Image$$TFM_SECURE_STACK$$ZI$$Base->  +-------------------------+
     */
    *(tfm_secure_stack_seal) = TFM_STACK_SEAL_VALUE;
    *(tfm_secure_stack_seal + 1) = TFM_STACK_SEAL_VALUE;

    /*
     * Seal the ER_INITIAL_PSP by writing the seal value to the reserved
     * region.
     */
    uint32_t *arm_lib_stck_seal_base =
        ((uint32_t *)&REGION_NAME(Image$$, ER_INITIAL_PSP_SEAL, $$ZI$$Base)[-1]) - 2;

    *(arm_lib_stck_seal_base) = TFM_STACK_SEAL_VALUE;
    *(arm_lib_stck_seal_base + 1) = TFM_STACK_SEAL_VALUE;
}

/*
 * This is the "Big Lock" on the secure side, to guarantee single entry
 * to SPE
 */
static int32_t tfm_secure_lock;
static int32_t tfm_secure_api_initializing = 1;

static uint32_t *prepare_partition_iovec_ctx(
                             const struct tfm_state_context_t *svc_ctx,
                             const struct tfm_sfn_req_s *desc_ptr,
                             const struct iovec_args_t *iovec_args,
                             uint32_t *dst)
{
    /* XPSR  = as was when called, but make sure it's thread mode */
    *(--dst) = svc_ctx->xpsr & 0xFFFFFE00U;
    /* ReturnAddress = resume veneer in new context */
    *(--dst) = svc_ctx->ra;
    /* LR = sfn address */
    *(--dst) = (uint32_t)desc_ptr->sfn;
    /* R12 = don't care */
    *(--dst) = 0U;

    /* R0-R3 = sfn arguments */
    *(--dst) = iovec_args->out_len;
    *(--dst) = (uint32_t)iovec_args->out_vec;
    *(--dst) = iovec_args->in_len;
    *(--dst) = (uint32_t)iovec_args->in_vec;

    return dst;
}

/**
 * \brief Create a stack frame that sets the execution environment to thread
 *        mode on exception return.
 *
 * \param[in] svc_ctx         The stacked SVC context
 * \param[in] unpriv_handler  The unprivileged IRQ handler to be called
 * \param[in] dst             A pointer where the context is to be created. (the
 *                            pointer is considered to be a stack pointer, and
 *                            the frame is created below it)
 *
 * \return A pointer pointing at the created stack frame.
 */
static int32_t *prepare_partition_irq_ctx(
                             const struct tfm_state_context_t *svc_ctx,
                             sfn_t unpriv_handler,
                             int32_t *dst)
{
    int i;

    /* XPSR  = as was when called, but make sure it's thread mode */
    *(--dst) = svc_ctx->xpsr & 0xFFFFFE00;
    /* ReturnAddress = resume to the privileged handler code, but execute it
     * unprivileged.
     */
    *(--dst) = svc_ctx->ra;
    /* LR = start address */
    *(--dst) = (int32_t)unpriv_handler;

    /* R12, R0-R3 unused arguments */
    for (i = 0; i < 5; ++i) {
        *(--dst) = 0;
    }

    return dst;
}

static void restore_caller_ctx(const struct tfm_state_context_t *svc_ctx,
                               struct tfm_state_context_t *target_ctx)
{
    /* ReturnAddress = resume veneer after second SVC */
    target_ctx->ra = svc_ctx->ra;

    /* R0 = function return value */
    target_ctx->r0 = svc_ctx->r0;

    return;
}

/**
 * \brief Check whether the iovec parameters are valid, and the memory ranges
 *        are in the possession of the calling partition.
 *
 * \param[in]  desc_ptr  The secure function request descriptor
 * \param[out] iovec_ptr The local buffer to store iovec arguments
 *
 * \return Return /ref TFM_SUCCESS if the iovec parameters are valid, error code
 *         otherwise as in /ref tfm_status_e
 */
static enum tfm_status_e tfm_core_check_sfn_parameters(
                                           const struct tfm_sfn_req_s *desc_ptr,
                                           struct iovec_params_t *iovec_ptr)
{
    struct psa_invec *in_vec = (psa_invec *)desc_ptr->args[0];
    size_t in_len;
    struct psa_outvec *out_vec = (psa_outvec *)desc_ptr->args[2];
    size_t out_len;
    uint32_t i;
    uint32_t privileged_mode = TFM_PARTITION_UNPRIVILEGED_MODE;

    if ((desc_ptr->args[1] < 0) || (desc_ptr->args[3] < 0)) {
        return TFM_ERROR_INVALID_PARAMETER;
    }

    in_len = (size_t)(desc_ptr->args[1]);
    out_len = (size_t)(desc_ptr->args[3]);

    /*
     * Get caller's privileged mode:
     * The privileged mode of NS Secure Service caller will be decided by the
     * tfm_core_has_xxx_access_to_region functions.
     * Secure caller can be only privileged mode because the whole SPE is
     * running under privileged mode
     */
    if (!desc_ptr->ns_caller) {
        privileged_mode = TFM_PARTITION_PRIVILEGED_MODE;
    }

    /* The number of vectors are within range. Extra checks to avoid overflow */
    if ((in_len > PSA_MAX_IOVEC) || (out_len > PSA_MAX_IOVEC) ||
        (in_len + out_len > PSA_MAX_IOVEC)) {
        return TFM_ERROR_INVALID_PARAMETER;
    }

    /* Check whether the caller partition has at write access to the iovec
     * structures themselves. Use the TT instruction for this.
     */
    if (in_len > 0) {
        if ((in_vec == NULL) ||
            (tfm_core_has_write_access_to_region(in_vec,
                            sizeof(psa_invec)*in_len, desc_ptr->ns_caller,
                            privileged_mode) != TFM_SUCCESS)) {
            return TFM_ERROR_INVALID_PARAMETER;
        }
    } else {
        if (in_vec != NULL) {
            return TFM_ERROR_INVALID_PARAMETER;
        }
    }
    if (out_len > 0) {
        if ((out_vec == NULL) ||
            (tfm_core_has_write_access_to_region(out_vec,
                            sizeof(psa_outvec)*out_len, desc_ptr->ns_caller,
                            privileged_mode) != TFM_SUCCESS)) {
            return TFM_ERROR_INVALID_PARAMETER;
        }
    } else {
        if (out_vec != NULL) {
            return TFM_ERROR_INVALID_PARAMETER;
        }
    }

    /* Copy iovec parameters into a local buffer before validating them */
    iovec_ptr->in_len = in_len;
    for (i = 0; i < in_len; ++i) {
        iovec_ptr->in_vec[i].base = in_vec[i].base;
        iovec_ptr->in_vec[i].len = in_vec[i].len;
    }
    iovec_ptr->out_len = out_len;
    for (i = 0; i < out_len; ++i) {
        iovec_ptr->out_vec[i].base = out_vec[i].base;
        iovec_ptr->out_vec[i].len = out_vec[i].len;
    }
    iovec_ptr->orig_outvec = out_vec;

    /* Check whether the caller partition has access to the data inside the
     * iovecs
     */
    for (i = 0; i < in_len; ++i) {
        if (iovec_ptr->in_vec[i].len > 0) {
            if ((iovec_ptr->in_vec[i].base == NULL) ||
                (tfm_core_has_read_access_to_region(iovec_ptr->in_vec[i].base,
                            iovec_ptr->in_vec[i].len, desc_ptr->ns_caller,
                            privileged_mode) != TFM_SUCCESS)) {
                return TFM_ERROR_INVALID_PARAMETER;
            }
        }
    }
    for (i = 0; i < out_len; ++i) {
        if (iovec_ptr->out_vec[i].len > 0) {
            if ((iovec_ptr->out_vec[i].base == NULL) ||
                (tfm_core_has_write_access_to_region(iovec_ptr->out_vec[i].base,
                            iovec_ptr->out_vec[i].len, desc_ptr->ns_caller,
                            privileged_mode) != TFM_SUCCESS)) {
                return TFM_ERROR_INVALID_PARAMETER;
            }
        }
    }

    return TFM_SUCCESS;
}

static void tfm_copy_iovec_parameters(struct iovec_args_t *target,
                                      const struct iovec_args_t *source)
{
    size_t i;

    /* The vectors have been sanity checked already, and since then the
     * interrupts have been kept disabled. So we can be sure that the
     * vectors haven't been tampered with since the check. So it is safe to pass
     * it to the called partition.
     */

    target->in_len = source->in_len;
    for (i = 0; i < source->in_len; ++i) {
        target->in_vec[i].base = source->in_vec[i].base;
        target->in_vec[i].len = source->in_vec[i].len;
    }
    target->out_len = source->out_len;
    for (i = 0; i < source->out_len; ++i) {
        target->out_vec[i].base = source->out_vec[i].base;
        target->out_vec[i].len = source->out_vec[i].len;
    }
}

static void tfm_clear_iovec_parameters(struct iovec_args_t *args)
{
    int i;

    args->in_len = 0;
    for (i = 0; i < PSA_MAX_IOVEC; ++i) {
        args->in_vec[i].base = NULL;
        args->in_vec[i].len = 0;
    }
    args->out_len = 0;
    for (i = 0; i < PSA_MAX_IOVEC; ++i) {
        args->out_vec[i].base = NULL;
        args->out_vec[i].len = 0;
    }
}

/**
 * \brief Check whether the partitions for the secure function call are in a
 *        proper state.
 *
 * \param[in] curr_partition_state    State of the partition to be called
 * \param[in] caller_partition_state  State of the caller partition
 *
 * \return \ref TFM_SUCCESS if the check passes, error otherwise.
 */
static enum tfm_status_e check_partition_state(uint32_t curr_partition_state,
                                               uint32_t caller_partition_state)
{
    if (caller_partition_state != SPM_PARTITION_STATE_RUNNING) {
        /* Calling partition from non-running state (e.g. during handling IRQ)
         * is not allowed.
         */
        return TFM_ERROR_INVALID_EXC_MODE;
    }

    if (curr_partition_state == SPM_PARTITION_STATE_RUNNING ||
        curr_partition_state == SPM_PARTITION_STATE_HANDLING_IRQ ||
        curr_partition_state == SPM_PARTITION_STATE_SUSPENDED ||
        curr_partition_state == SPM_PARTITION_STATE_BLOCKED) {
        /* Active partitions cannot be called! */
        return TFM_ERROR_PARTITION_NON_REENTRANT;
    } else if (curr_partition_state != SPM_PARTITION_STATE_IDLE) {
        /* The partition to be called is not in a proper state */
        return TFM_SECURE_LOCK_FAILED;
    }
    return TFM_SUCCESS;
}

/**
 * \brief Check whether the partitions for the secure function call of irq are
 *        in a proper state.
 *
 * \param[in] called_partition_state    State of the partition to be called
 *
 * \return \ref TFM_SUCCESS if the check passes, error otherwise.
 */
static enum tfm_status_e check_irq_partition_state(
                                                uint32_t called_partition_state)
{
    if (called_partition_state == SPM_PARTITION_STATE_IDLE ||
        called_partition_state == SPM_PARTITION_STATE_RUNNING ||
        called_partition_state == SPM_PARTITION_STATE_HANDLING_IRQ ||
        called_partition_state == SPM_PARTITION_STATE_SUSPENDED ||
        called_partition_state == SPM_PARTITION_STATE_BLOCKED) {
        return TFM_SUCCESS;
    }
    return TFM_SECURE_LOCK_FAILED;
}

/**
 * \brief Calculate the address where the iovec parameters are to be saved for
 *        the called partition.
 *
 * \param[in] partition_idx  The index of the partition to be called.
 *
 * \return The address where the iovec parameters should be saved.
 */
static struct iovec_args_t *get_iovec_args_stack_address(uint32_t partition_idx)
{
    /* Save the iovecs on the common stack. */
    return &REGION_NAME(Image$$, TFM_SECURE_STACK, $$ZI$$Limit)[-1];
}

/**
 * \brief Returns the index of the partition with the given partition ID.
 *
 * \param[in] partition_id     Partition id
 *
 * \return the partition idx if partition_id is valid,
 *         \ref SPM_INVALID_PARTITION_IDX othervise
 */
static uint32_t get_partition_idx(uint32_t partition_id)
{
    uint32_t i;

    if (partition_id == INVALID_PARTITION_ID) {
        return SPM_INVALID_PARTITION_IDX;
    }

    for (i = 0; i < g_spm_partition_db.partition_count; ++i) {
        if (g_spm_partition_db.partitions[i].static_data->partition_id ==
            partition_id) {
            return i;
        }
    }
    return SPM_INVALID_PARTITION_IDX;
}

/**
 * \brief Set the iovec parameters for the partition
 *
 * \param[in] partition_idx  Partition index
 * \param[in] iovec_ptr      The arguments of the secure function
 *
 * \return Error code \ref spm_err_t
 *
 * \note This function doesn't check if partition_idx is valid.
 * \note This function assumes that the iovecs that are passed in iovec_ptr are
 *       valid, and does no sanity check on them at all.
 */
static enum spm_err_t tfm_spm_partition_set_iovec(uint32_t partition_idx,
                                         const struct iovec_params_t *iovec_ptr)
{
    struct spm_partition_runtime_data_t *runtime_data =
            &g_spm_partition_db.partitions[partition_idx].runtime_data;
    size_t i;

    if ((iovec_ptr->in_len < 0) || (iovec_ptr->out_len < 0)) {
        return SPM_ERR_INVALID_PARAMETER;
    }

    runtime_data->iovec_args.in_len = iovec_ptr->in_len;
    for (i = 0U; i < runtime_data->iovec_args.in_len; ++i) {
        runtime_data->iovec_args.in_vec[i].base = iovec_ptr->in_vec[i].base;
        runtime_data->iovec_args.in_vec[i].len = iovec_ptr->in_vec[i].len;
    }
    runtime_data->iovec_args.out_len = iovec_ptr->out_len;
    for (i = 0U; i < runtime_data->iovec_args.out_len; ++i) {
        runtime_data->iovec_args.out_vec[i].base = iovec_ptr->out_vec[i].base;
        runtime_data->iovec_args.out_vec[i].len = iovec_ptr->out_vec[i].len;
    }
    runtime_data->orig_outvec = iovec_ptr->orig_outvec;

    return SPM_ERR_OK;
}

static enum tfm_status_e tfm_start_partition(
                                         const struct tfm_sfn_req_s *desc_ptr,
                                         const struct iovec_params_t *iovec_ptr,
                                         uint32_t excReturn)
{
    register uint32_t partition_idx;
    enum tfm_status_e res;
    uint32_t caller_partition_idx = desc_ptr->caller_part_idx;
    const struct spm_partition_runtime_data_t *curr_part_data;
    const struct spm_partition_runtime_data_t *caller_part_data;
    uint32_t caller_flags;
    uint32_t psp;
    uint32_t partition_psp, partition_psplim;
    uint32_t partition_state;
    uint32_t caller_partition_state;
    struct tfm_state_context_t *svc_ctx;
    uint32_t caller_partition_id;
    int32_t client_id;
    struct iovec_args_t *iovec_args;

    psp = __get_PSP();
    svc_ctx = (struct tfm_state_context_t *)psp;
    caller_flags = tfm_spm_partition_get_flags(caller_partition_idx);

    /* Check partition state consistency */
    if (((caller_flags & SPM_PART_FLAG_APP_ROT) != 0)
        != (!desc_ptr->ns_caller)) {
        /* Partition state inconsistency detected */
        return TFM_SECURE_LOCK_FAILED;
    }

    partition_idx = get_partition_idx(desc_ptr->sp_id);

    curr_part_data = tfm_spm_partition_get_runtime_data(partition_idx);
    caller_part_data = tfm_spm_partition_get_runtime_data(caller_partition_idx);
    partition_state = curr_part_data->partition_state;
    caller_partition_state = caller_part_data->partition_state;
    caller_partition_id = tfm_spm_partition_get_partition_id(
                                                          caller_partition_idx);

    if (!tfm_secure_api_initializing) {
        res = check_partition_state(partition_state, caller_partition_state);
        if (res != TFM_SUCCESS) {
            return res;
        }
    }

    /* Prepare switch to shared secure partition stack */
    /* In case the call is coming from the non-secure world, we save the iovecs
     * on the stop of the stack. Also the stack seal is present below this region.
     * So the memory area, that can actually be used as stack by the partitions
     * starts at a lower address.
     */
    partition_psp = (uint32_t) tfm_secure_stack_seal;
    partition_psplim =
        (uint32_t)&REGION_NAME(Image$$, TFM_SECURE_STACK, $$ZI$$Base);

    /* Store the context for the partition call */
    tfm_spm_partition_set_caller_partition_idx(partition_idx,
                                               caller_partition_idx);
    tfm_spm_partition_store_context(caller_partition_idx, psp, excReturn);

    if ((caller_flags & SPM_PART_FLAG_APP_ROT)) {
        tfm_spm_partition_set_caller_client_id(partition_idx,
                                               caller_partition_id);
    } else {
        client_id = tfm_nspm_get_current_client_id();
        if (client_id >= 0) {
            return TFM_SECURE_LOCK_FAILED;
        }
        tfm_spm_partition_set_caller_client_id(partition_idx, client_id);
    }

    /* In level one, only switch context and return from exception if in
     * handler mode
     */
    if ((desc_ptr->ns_caller) || (tfm_secure_api_initializing)) {
        if (tfm_spm_partition_set_iovec(partition_idx, iovec_ptr) !=
            SPM_ERR_OK) {
            return TFM_ERROR_GENERIC;
        }
        iovec_args = get_iovec_args_stack_address(partition_idx);
        tfm_copy_iovec_parameters(iovec_args, &(curr_part_data->iovec_args));

        /* Prepare the partition context, update stack ptr */
        psp = (uint32_t)prepare_partition_iovec_ctx(svc_ctx, desc_ptr,
                                                    iovec_args,
                                                    (uint32_t *)partition_psp);
        __set_PSP(psp);
        tfm_arch_set_psplim(partition_psplim);
    }

    tfm_spm_partition_set_state(caller_partition_idx,
                                SPM_PARTITION_STATE_BLOCKED);
    tfm_spm_partition_set_state(partition_idx, SPM_PARTITION_STATE_RUNNING);
    tfm_secure_lock++;

    return TFM_SUCCESS;
}

static enum tfm_status_e tfm_start_partition_for_irq_handling(
                                            uint32_t excReturn,
                                            struct tfm_state_context_t *svc_ctx)
{
    uint32_t handler_partition_id = svc_ctx->r0;
    sfn_t unpriv_handler = (sfn_t)svc_ctx->r1;
    uint32_t irq_signal = svc_ctx->r2;
    uint32_t irq_line = svc_ctx->r3;
    enum tfm_status_e res;
    uint32_t psp = __get_PSP();
    uint32_t handler_partition_psp;
    uint32_t handler_partition_state;
    uint32_t interrupted_partition_idx =
            tfm_spm_partition_get_running_partition_idx();
    const struct spm_partition_runtime_data_t *handler_part_data;
    uint32_t handler_partition_idx;

    handler_partition_idx = get_partition_idx(handler_partition_id);
    handler_part_data = tfm_spm_partition_get_runtime_data(
                                                         handler_partition_idx);
    handler_partition_state = handler_part_data->partition_state;

    res = check_irq_partition_state(handler_partition_state);
    if (res != TFM_SUCCESS) {
        return res;
    }

    /* set mask for the partition */
    tfm_spm_partition_set_signal_mask(
                                   handler_partition_idx,
                                   handler_part_data->signal_mask | irq_signal);

    tfm_spm_hal_disable_irq(irq_line);

    /* save the current context of the interrupted partition */
    tfm_spm_partition_push_interrupted_ctx(interrupted_partition_idx);

    handler_partition_psp = psp;

    /* save the current context of the handler partition */
    tfm_spm_partition_push_handler_ctx(handler_partition_idx);

    /* Store caller for the partition */
    tfm_spm_partition_set_caller_partition_idx(handler_partition_idx,
                                               interrupted_partition_idx);

    psp = (uint32_t)prepare_partition_irq_ctx(svc_ctx, unpriv_handler,
                                              (int32_t *)handler_partition_psp);
    __set_PSP(psp);

    tfm_spm_partition_set_state(interrupted_partition_idx,
                                SPM_PARTITION_STATE_SUSPENDED);
    tfm_spm_partition_set_state(handler_partition_idx,
                                SPM_PARTITION_STATE_HANDLING_IRQ);

    return TFM_SUCCESS;
}

static enum tfm_status_e tfm_return_from_partition(uint32_t *excReturn)
{
    uint32_t current_partition_idx =
            tfm_spm_partition_get_running_partition_idx();
    const struct spm_partition_runtime_data_t *curr_part_data, *ret_part_data;
    uint32_t return_partition_idx;
    uint32_t return_partition_flags;
    uint32_t psp = __get_PSP();
    size_t i;
    struct tfm_state_context_t *svc_ctx = (struct tfm_state_context_t *)psp;
    struct iovec_args_t *iovec_args;

    if (current_partition_idx == SPM_INVALID_PARTITION_IDX) {
        return TFM_SECURE_UNLOCK_FAILED;
    }

    curr_part_data = tfm_spm_partition_get_runtime_data(current_partition_idx);
    return_partition_idx = curr_part_data->caller_partition_idx;

    if (return_partition_idx == SPM_INVALID_PARTITION_IDX) {
        return TFM_SECURE_UNLOCK_FAILED;
    }

    ret_part_data = tfm_spm_partition_get_runtime_data(return_partition_idx);

    return_partition_flags = tfm_spm_partition_get_flags(return_partition_idx);

    tfm_secure_lock--;

    if (!(return_partition_flags & SPM_PART_FLAG_APP_ROT) ||
        (tfm_secure_api_initializing)) {
        /* In TFM level 1 context restore is only done when
         * returning to NS or after initialization
         */
        /* Restore caller context */
        restore_caller_ctx(svc_ctx,
            (struct tfm_state_context_t *)ret_part_data->stack_ptr);
        *excReturn = ret_part_data->lr;
        __set_PSP(ret_part_data->stack_ptr);
        REGION_DECLARE_T(Image$$, ER_INITIAL_PSP, $$ZI$$Base, uint32_t)[];
        uint32_t psp_stack_bottom =
            (uint32_t)REGION_NAME(Image$$, ER_INITIAL_PSP, $$ZI$$Base);
        tfm_arch_set_psplim(psp_stack_bottom);

        iovec_args = &REGION_NAME(Image$$, TFM_SECURE_STACK, $$ZI$$Limit)[-1];

        for (i = 0; i < curr_part_data->iovec_args.out_len; ++i) {
            curr_part_data->orig_outvec[i].len = iovec_args->out_vec[i].len;
        }
        tfm_clear_iovec_parameters(iovec_args);
    }

    tfm_spm_partition_cleanup_context(current_partition_idx);

    tfm_spm_partition_set_state(current_partition_idx,
                                SPM_PARTITION_STATE_IDLE);
    tfm_spm_partition_set_state(return_partition_idx,
                                SPM_PARTITION_STATE_RUNNING);

    return TFM_SUCCESS;
}

static enum tfm_status_e tfm_return_from_partition_irq_handling(
                                                            uint32_t *excReturn)
{
    uint32_t handler_partition_idx =
            tfm_spm_partition_get_running_partition_idx();
    const struct spm_partition_runtime_data_t *handler_part_data;
    uint32_t interrupted_partition_idx;
    uint32_t psp = __get_PSP();
    struct tfm_state_context_t *svc_ctx = (struct tfm_state_context_t *)psp;

    if (handler_partition_idx == SPM_INVALID_PARTITION_IDX) {
        return TFM_SECURE_UNLOCK_FAILED;
    }

    handler_part_data = tfm_spm_partition_get_runtime_data(
                                                         handler_partition_idx);
    interrupted_partition_idx = handler_part_data->caller_partition_idx;

    if (interrupted_partition_idx == SPM_INVALID_PARTITION_IDX) {
        return TFM_SECURE_UNLOCK_FAILED;
    }

    /* For level 1, modify PSP, so that the SVC stack frame disappears,
     * and return to the privileged handler using the stack frame still on the
     * MSP stack.
     */
    *excReturn = svc_ctx->ra;
    psp += sizeof(struct tfm_state_context_t);

    tfm_spm_partition_pop_handler_ctx(handler_partition_idx);
    tfm_spm_partition_pop_interrupted_ctx(interrupted_partition_idx);

    __set_PSP(psp);

    return TFM_SUCCESS;
}

static enum tfm_status_e tfm_check_sfn_req_integrity(
                                           const struct tfm_sfn_req_s *desc_ptr)
{
    if ((desc_ptr == NULL) ||
        (desc_ptr->sp_id == 0) ||
        (desc_ptr->sfn == NULL)) {
        /* invalid parameter */
        return TFM_ERROR_INVALID_PARAMETER;
    }
    return TFM_SUCCESS;
}

static enum tfm_status_e tfm_core_check_sfn_req_rules(
        const struct tfm_sfn_req_s *desc_ptr)
{
    /* Check partition idx validity */
    if (desc_ptr->caller_part_idx == SPM_INVALID_PARTITION_IDX) {
        return TFM_ERROR_NO_ACTIVE_PARTITION;
    }

    if ((desc_ptr->ns_caller) && (tfm_secure_lock != 0)) {
        /* Secure domain is already locked!
         * This should only happen if caller is secure partition!
         */
        /* This scenario is a potential security breach.
         * Error is handled in caller.
         */
        return TFM_ERROR_SECURE_DOMAIN_LOCKED;
    }

    if (tfm_secure_api_initializing) {
        int32_t id =
            tfm_spm_partition_get_partition_id(desc_ptr->caller_part_idx);

        if ((id != TFM_SP_CORE_ID) || (tfm_secure_lock != 0)) {
            /* Invalid request during system initialization */
            SPMLOG_ERRMSG("Invalid service request during initialization!\r\n");
            return TFM_ERROR_NOT_INITIALIZED;
        }
    }

    return TFM_SUCCESS;
}

uint32_t tfm_spm_partition_get_flags(uint32_t partition_idx)
{
    return g_spm_partition_db.partitions[partition_idx].static_data->
           partition_flags;
}

uint32_t tfm_spm_partition_get_partition_id(uint32_t partition_idx)
{
    return g_spm_partition_db.partitions[partition_idx].static_data->
           partition_id;
}

uint32_t tfm_spm_partition_get_privileged_mode(uint32_t partition_flags)
{
    if (partition_flags & SPM_PART_FLAG_PSA_ROT) {
        return TFM_PARTITION_PRIVILEGED_MODE;
    } else {
        return TFM_PARTITION_UNPRIVILEGED_MODE;
    }
}

bool tfm_is_partition_privileged(uint32_t partition_idx)
{
    uint32_t flags = tfm_spm_partition_get_flags(partition_idx);

    return tfm_spm_partition_get_privileged_mode(flags) ==
           TFM_PARTITION_PRIVILEGED_MODE;
}

void tfm_spm_secure_api_init_done(void)
{
    tfm_secure_api_initializing = 0;
}

static enum tfm_status_e tfm_spm_sfn_request_handler(
                             struct tfm_sfn_req_s *desc_ptr, uint32_t excReturn)
{
    enum tfm_status_e res;
    struct iovec_params_t iovecs = {0};

    res = tfm_check_sfn_req_integrity(desc_ptr);
    if (res != TFM_SUCCESS) {
        SPMLOG_ERRMSG("Invalid service request!\r\n");
        tfm_secure_api_error_handler();
    }

    desc_ptr->caller_part_idx = tfm_spm_partition_get_running_partition_idx();

    res = tfm_core_check_sfn_parameters(desc_ptr, &iovecs);
    if (res != TFM_SUCCESS) {
        /* The sanity check of iovecs failed. */
        tfm_secure_api_error_handler();
    }

    res = tfm_core_check_sfn_req_rules(desc_ptr);
    if (res != TFM_SUCCESS) {
        /* FixMe: error compartmentalization TBD */
        tfm_spm_partition_set_state(
            desc_ptr->caller_part_idx, SPM_PARTITION_STATE_CLOSED);
        SPMLOG_ERRMSG("Unauthorized service request!\r\n");
        tfm_secure_api_error_handler();
    }

    res = tfm_start_partition(desc_ptr, &iovecs, excReturn);
    if (res != TFM_SUCCESS) {
        /* FixMe: consider possible fault scenarios */
        SPMLOG_ERRMSG("Failed to process service request!\r\n");
        tfm_secure_api_error_handler();
    }

    return res;
}

int32_t tfm_spm_sfn_request_thread_mode(struct tfm_sfn_req_s *desc_ptr)
{
    enum tfm_status_e res;
    int32_t *args;
    int32_t retVal;
    struct iovec_params_t iovecs;

    res = tfm_core_check_sfn_parameters(desc_ptr, &iovecs);
    if (res != TFM_SUCCESS) {
        /* The sanity check of iovecs failed. */
        return (int32_t)res;
    }

    /* No excReturn value is needed as no exception handling is used */
    res = tfm_spm_sfn_request_handler(desc_ptr, 0);

    if (res != TFM_SUCCESS) {
        tfm_secure_api_error_handler();
    }

    /* Secure partition to secure partition call in TFM level 1 */
    args = desc_ptr->args;
    retVal = desc_ptr->sfn(args[0], args[1], args[2], args[3]);

    /* return handler should restore original exc_return value... */
    res = tfm_return_from_partition(NULL);
    if (res == TFM_SUCCESS) {
        /* If unlock successful, pass SS return value to caller */
        return retVal;
    } else {
        /* Unlock errors indicate ctx database corruption or unknown
         * anomalies. Halt execution
         */
        SPMLOG_ERRMSG("Secure API error during unlock!\r\n");
        tfm_secure_api_error_handler();
    }
    return (int32_t)res;
}

int32_t tfm_spm_check_buffer_access(uint32_t  partition_idx,
                                    void     *start_addr,
                                    size_t    len,
                                    uint32_t  alignment)
{
    uintptr_t start_addr_value = (uintptr_t)start_addr;
    uintptr_t end_addr_value = (uintptr_t)start_addr + len;
    uintptr_t alignment_mask;

    alignment_mask = (((uintptr_t)1) << alignment) - 1;

    /* Check pointer alignment and protect against overflow and zero len */
    if (!(start_addr_value & alignment_mask) &&
        (end_addr_value > start_addr_value)) {
        /* Check that the range is in S_DATA */
        if ((start_addr_value >= S_DATA_START) &&
            (end_addr_value <= (S_DATA_START + S_DATA_SIZE))) {
            return TFM_SUCCESS;
        } else {
            return TFM_ERROR_NOT_IN_RANGE;
        }
    }

    return TFM_ERROR_INVALID_PARAMETER;
}

static void tfm_spm_partition_requests_thread(struct tfm_sfn_req_s *desc_ptr,
                                              uint32_t exc_return,
                                              uint32_t is_return,
                                              uintptr_t msp)
{
    enum tfm_status_e res;
    uint32_t exc_ret;

    if (!is_return) {
        res = tfm_spm_sfn_request_handler(desc_ptr, exc_return);
        exc_ret = EXC_RETURN_SECURE_FUNCTION;
    } else {
        res = tfm_return_from_partition(&exc_return);
        exc_ret = exc_return;
    }
    /* Reset MSP at top of stack and do TFM_SVC_SFN_COMPLETION */
    tfm_sfn_completion(res, exc_ret, msp);
}

/* This SVC handler is called if veneer is running in thread mode */
void tfm_spm_partition_request_return_handler(
        const uint32_t *svc_ctx, uint32_t exc_return, uint32_t *msp)
{
    if (!(exc_return & EXC_RETURN_STACK_PROCESS)) {
        /* Service request SVC called with MSP active.
         * Either invalid configuration for Thread mode or SVC called
         * from Handler mode, which is not supported.
         * FixMe: error severity TBD
         */
        SPMLOG_ERRMSG("Service request SVC called with MSP active!\r\n");
        tfm_secure_api_error_handler();
    }

    /* Setup a context on the stack to trigger exception return */
    struct tfm_state_context_t ctx = {0};

    ctx.r0 = svc_ctx ? svc_ctx[0] : (uintptr_t) NULL;
    ctx.r1 = exc_return;
    ctx.r2 = svc_ctx ? 0 : 1;
    ctx.r3 = (uintptr_t) msp;
    ctx.xpsr = XPSR_T32;
    ctx.ra = (uint32_t) tfm_spm_partition_requests_thread & ~0x1UL;

    __set_MSP((uint32_t)&ctx);

    tfm_arch_trigger_exc_return(EXC_RETURN_THREAD_S_MSP);
}

void tfm_spm_partition_completion_handler(enum tfm_status_e res, uint32_t exc_return, uint32_t *msp)
{
    if (res != TFM_SUCCESS) {
        tfm_secure_api_error_handler();
    }

    uint32_t msp_stack_val = (uint32_t)msp + sizeof(struct tfm_state_context_t);

    /* Equivalent to a call to __set_MSP() and then tfm_arch_trigger_exc_return
     * with the exc_return value received as parameter in the handler
     */
    __ASM volatile (
        "MSR msp, %0\n"
        "MOV R0, %1\n"
        "BX R0"
        : : "r" (msp_stack_val), "r" (exc_return) : );
}

/* This SVC handler is called, if a thread mode execution environment is to
 * be set up, to run an unprivileged IRQ handler
 */
uint32_t tfm_spm_depriv_req_handler(uint32_t *svc_args, uint32_t excReturn)
{
    struct tfm_state_context_t *svc_ctx =
                                        (struct tfm_state_context_t *)svc_args;

    enum tfm_status_e res;

    if (excReturn & EXC_RETURN_STACK_PROCESS) {
        /* FixMe: error severity TBD */
        SPMLOG_ERRMSG("Partition request SVC called with PSP active!\r\n");
        tfm_secure_api_error_handler();
    }

    res = tfm_start_partition_for_irq_handling(excReturn, svc_ctx);
    if (res != TFM_SUCCESS) {
        /* The partition is in an invalid state (UNINIT or CLOSED), so none of
         * its code can be run
         */
        /* FixMe: For now this case is handled with TF-M panic, however it would
         * be possible to skip the execution of the interrupt handler, and
         * resume the execution of the interrupted code.
         */
        tfm_secure_api_error_handler();
    }
    return EXC_RETURN_SECURE_FUNCTION;
}

/* This SVC handler is called if a deprivileged IRQ handler was executed, and
 * the execution environment is to be set back for the privileged handler mode
 */
uint32_t tfm_spm_depriv_return_handler(uint32_t *irq_svc_args, uint32_t lr)
{
    enum tfm_status_e res;
    struct tfm_state_context_t *irq_svc_ctx;

    /* Take into account the sealed stack*/
    irq_svc_args += 2;

    irq_svc_ctx = (struct tfm_state_context_t *)irq_svc_args;

    if (!(lr & EXC_RETURN_STACK_PROCESS)) {
        /* Partition request SVC called with MSP active.
         * FixMe: error severity TBD
         */
        SPMLOG_ERRMSG("Partition request SVC called with MSP active!\r\n");
        tfm_secure_api_error_handler();
    }

    res = tfm_return_from_partition_irq_handling(&lr);
    if (res != TFM_SUCCESS) {
        /* Unlock errors indicate ctx database corruption or unknown anomalies
         * Halt execution
         */
        SPMLOG_ERRMSG("Secure API error during unlock!\r\n");
        tfm_secure_api_error_handler();
    }

    irq_svc_ctx->ra = lr;

    return EXC_RETURN_SECURE_HANDLER;
}

/* FIXME: get_irq_line_for_signal is also implemented in the ipc folder. */
/**
 * \brief Return the IRQ line number associated with a signal
 *
 * \param[in] partition_id    The ID of the partition in which we look for the
 *                            signal
 * \param[in] signal          The signal we do the query for
 *
 * \retval >=0     The IRQ line number associated with a signal in the partition
 * \retval <0      error
 */
static int32_t get_irq_line_for_signal(int32_t partition_id,
                                       psa_signal_t signal)
{
    size_t i;

    if (!IS_ONLY_ONE_BIT_IN_UINT32(signal)) {
        return -1;
    }

    for (i = 0; i < tfm_core_irq_signals_count; ++i) {
        if (tfm_core_irq_signals[i].partition_id == partition_id &&
            tfm_core_irq_signals[i].signal_value == signal) {
            return tfm_core_irq_signals[i].irq_line;
        }
    }
    return -1;
}

void tfm_spm_enable_irq_handler(uint32_t *svc_args)
{
    struct tfm_state_context_t *svc_ctx =
                                        (struct tfm_state_context_t *)svc_args;
    psa_signal_t irq_signal = svc_ctx->r0;
    uint32_t running_partition_idx =
                      tfm_spm_partition_get_running_partition_idx();
    uint32_t running_partition_id =
                      tfm_spm_partition_get_partition_id(running_partition_idx);
    int32_t irq_line;

    irq_line = get_irq_line_for_signal(running_partition_id, irq_signal);

    if (irq_line < 0) {
        /* FixMe: error severity TBD */
        tfm_secure_api_error_handler();
    }

    tfm_spm_hal_enable_irq(irq_line);
}

void tfm_spm_disable_irq_handler(uint32_t *svc_args)
{
    struct tfm_state_context_t *svc_ctx =
                                        (struct tfm_state_context_t *)svc_args;
    psa_signal_t irq_signal = svc_ctx->r0;
    uint32_t running_partition_idx =
                      tfm_spm_partition_get_running_partition_idx();
    uint32_t running_partition_id =
                      tfm_spm_partition_get_partition_id(running_partition_idx);
    int32_t irq_line;

    irq_line = get_irq_line_for_signal(running_partition_id, irq_signal);

    if (irq_line < 0) {
        /* FixMe: error severity TBD */
        tfm_secure_api_error_handler();
    }

    tfm_spm_hal_disable_irq(irq_line);
}

void tfm_spm_psa_wait(uint32_t *svc_args)
{
    /* Look for partition that is ready for run */
    struct tfm_state_context_t *svc_ctx =
                                        (struct tfm_state_context_t *)svc_args;
    uint32_t running_partition_idx;
    const struct spm_partition_runtime_data_t *curr_part_data;

    psa_signal_t signal_mask = svc_ctx->r0;
    uint32_t timeout = svc_ctx->r1;

    /*
     * Timeout[30:0] are reserved for future use.
     * SPM must ignore the value of RES.
     */
    timeout &= PSA_TIMEOUT_MASK;

    running_partition_idx = tfm_spm_partition_get_running_partition_idx();
    curr_part_data = tfm_spm_partition_get_runtime_data(running_partition_idx);

    if (timeout == PSA_BLOCK) {
        /* FIXME: Scheduling is not available in library model, and busy wait is
         * also not possible as this code is running in SVC context, and it
         * cannot be pre-empted by interrupts. So do nothing here for now
         */
        (void) signal_mask;
    }

    svc_ctx->r0 = curr_part_data->signal_mask;
}

void tfm_spm_psa_eoi(uint32_t *svc_args)
{
    struct tfm_state_context_t *svc_ctx =
                                        (struct tfm_state_context_t *)svc_args;
    psa_signal_t irq_signal = svc_ctx->r0;
    uint32_t signal_mask;
    uint32_t running_partition_idx;
    uint32_t running_partition_id;
    const struct spm_partition_runtime_data_t *curr_part_data;
    int32_t irq_line;

    running_partition_idx = tfm_spm_partition_get_running_partition_idx();
    running_partition_id =
                      tfm_spm_partition_get_partition_id(running_partition_idx);
    curr_part_data = tfm_spm_partition_get_runtime_data(running_partition_idx);

    irq_line = get_irq_line_for_signal(running_partition_id, irq_signal);

    if (irq_line < 0) {
        /* FixMe: error severity TBD */
        tfm_secure_api_error_handler();
    }

    tfm_spm_hal_clear_pending_irq(irq_line);
    tfm_spm_hal_enable_irq(irq_line);

    signal_mask = curr_part_data->signal_mask & ~irq_signal;
    tfm_spm_partition_set_signal_mask(running_partition_idx, signal_mask);
}

/*
 * This function is called when a secure partition causes an error.
 * In case of an error in the error handling, a non-zero value have to be
 * returned.
 */
static void tfm_spm_partition_err_handler(const uint32_t idx, int32_t errcode)
{
    (void)errcode;

    tfm_spm_partition_set_state(idx, SPM_PARTITION_STATE_CLOSED);
}

fih_int tfm_spm_partition_init(void)
{
    struct spm_partition_desc_t *part;
    struct tfm_sfn_req_s desc;
    int32_t args[4] = {0};
    fih_int fail_cnt = FIH_INT_INIT(0);
    uint32_t idx;
    bool privileged;
    const struct platform_data_t **platform_data_p;
#ifdef TFM_FIH_PROFILE_ON
    fih_int fih_rc = FIH_FAILURE;
#endif

    /* Call the init function for each partition */
    for (idx = 0; idx < g_spm_partition_db.partition_count; ++idx) {
        part = &g_spm_partition_db.partitions[idx];
        platform_data_p = part->platform_data_list;
        if (platform_data_p != NULL) {
            while ((*platform_data_p) != NULL) {
                if (tfm_is_partition_privileged(idx)) {
                    privileged = true;
                } else {
                    privileged = false;
                }
#ifdef TFM_FIH_PROFILE_ON
                FIH_CALL(tfm_spm_hal_configure_default_isolation, fih_rc,
                         privileged, *platform_data_p);
                if (fih_not_eq(fih_rc, fih_int_encode(TFM_PLAT_ERR_SUCCESS))) {
                    fail_cnt = fih_int_encode(fih_int_decode(fail_cnt) + 1);
                }
#else /* TFM_FIH_PROFILE_ON */
                if (tfm_spm_hal_configure_default_isolation(privileged,
                            *platform_data_p) != TFM_PLAT_ERR_SUCCESS) {
                    fail_cnt++;
                }
#endif /* TFM_FIH_PROFILE_ON */
                ++platform_data_p;
            }
        }
        if (part->static_data->partition_init == NULL) {
            tfm_spm_partition_set_state(idx, SPM_PARTITION_STATE_IDLE);
            tfm_spm_partition_set_caller_partition_idx(idx,
                                                    SPM_INVALID_PARTITION_IDX);
        } else {
            int32_t res;

            desc.args = args;
            desc.ns_caller = false;
            desc.sfn = (sfn_t)part->static_data->partition_init;
            desc.sp_id = part->static_data->partition_id;
            res = tfm_core_sfn_request(&desc);
            if (res == TFM_SUCCESS) {
                tfm_spm_partition_set_state(idx, SPM_PARTITION_STATE_IDLE);
            } else {
                tfm_spm_partition_err_handler(idx, res);
                fail_cnt = fih_int_encode(fih_int_decode(fail_cnt) + 1);
            }
        }
    }

    tfm_spm_secure_api_init_done();

    fih_int_validate(fail_cnt);
    if (fih_eq(fail_cnt, fih_int_encode(0))) {
        FIH_RET(fih_int_encode(SPM_ERR_OK));
    }

    FIH_RET(fih_int_encode(SPM_ERR_PARTITION_NOT_AVAILABLE));
}

void tfm_spm_partition_push_interrupted_ctx(uint32_t partition_idx)
{
    struct spm_partition_runtime_data_t *runtime_data =
        &g_spm_partition_db.partitions[partition_idx].runtime_data;
    struct interrupted_ctx_stack_frame_t *stack_frame =
        (struct interrupted_ctx_stack_frame_t *)runtime_data->ctx_stack_ptr;

    stack_frame->partition_state = runtime_data->partition_state;

    runtime_data->ctx_stack_ptr +=
        sizeof(struct interrupted_ctx_stack_frame_t) / sizeof(uint32_t);
}

void tfm_spm_partition_pop_interrupted_ctx(uint32_t partition_idx)
{
    struct spm_partition_runtime_data_t *runtime_data =
        &g_spm_partition_db.partitions[partition_idx].runtime_data;
    struct interrupted_ctx_stack_frame_t *stack_frame;

    runtime_data->ctx_stack_ptr -=
        sizeof(struct interrupted_ctx_stack_frame_t) / sizeof(uint32_t);

    stack_frame = (struct interrupted_ctx_stack_frame_t *)
                      runtime_data->ctx_stack_ptr;
    tfm_spm_partition_set_state(partition_idx, stack_frame->partition_state);
    stack_frame->partition_state = 0;
}

void tfm_spm_partition_push_handler_ctx(uint32_t partition_idx)
{
    struct spm_partition_runtime_data_t *runtime_data =
        &g_spm_partition_db.partitions[partition_idx].runtime_data;
    struct handler_ctx_stack_frame_t *stack_frame =
        (struct handler_ctx_stack_frame_t *)
            runtime_data->ctx_stack_ptr;

    stack_frame->partition_state = runtime_data->partition_state;
    stack_frame->caller_partition_idx = runtime_data->caller_partition_idx;

    runtime_data->ctx_stack_ptr +=
        sizeof(struct handler_ctx_stack_frame_t) / sizeof(uint32_t);
}

void tfm_spm_partition_pop_handler_ctx(uint32_t partition_idx)
{
    struct spm_partition_runtime_data_t *runtime_data =
        &g_spm_partition_db.partitions[partition_idx].runtime_data;
    struct handler_ctx_stack_frame_t *stack_frame;

    runtime_data->ctx_stack_ptr -=
        sizeof(struct handler_ctx_stack_frame_t) / sizeof(uint32_t);

    stack_frame = (struct handler_ctx_stack_frame_t *)
                      runtime_data->ctx_stack_ptr;

    tfm_spm_partition_set_state(partition_idx, stack_frame->partition_state);
    stack_frame->partition_state = 0;
    tfm_spm_partition_set_caller_partition_idx(
        partition_idx, stack_frame->caller_partition_idx);
    stack_frame->caller_partition_idx = 0;
}

void tfm_spm_partition_store_context(uint32_t partition_idx,
        uint32_t stack_ptr, uint32_t lr)
{
    g_spm_partition_db.partitions[partition_idx].
            runtime_data.stack_ptr = stack_ptr;
    g_spm_partition_db.partitions[partition_idx].
            runtime_data.lr = lr;
}

const struct spm_partition_runtime_data_t *
            tfm_spm_partition_get_runtime_data(uint32_t partition_idx)
{
    return &(g_spm_partition_db.partitions[partition_idx].runtime_data);
}

void tfm_spm_partition_set_state(uint32_t partition_idx, uint32_t state)
{
    g_spm_partition_db.partitions[partition_idx].runtime_data.partition_state =
            state;
    if (state == SPM_PARTITION_STATE_RUNNING ||
        state == SPM_PARTITION_STATE_HANDLING_IRQ) {
        g_spm_partition_db.running_partition_idx = partition_idx;
    }
}

void tfm_spm_partition_set_caller_partition_idx(uint32_t partition_idx,
                                                uint32_t caller_partition_idx)
{
    g_spm_partition_db.partitions[partition_idx].runtime_data.
            caller_partition_idx = caller_partition_idx;
}

void tfm_spm_partition_set_signal_mask(uint32_t partition_idx,
                                       uint32_t signal_mask)
{
    g_spm_partition_db.partitions[partition_idx].runtime_data.
            signal_mask = signal_mask;
}

void tfm_spm_partition_set_caller_client_id(uint32_t partition_idx,
                                            int32_t caller_client_id)
{
    g_spm_partition_db.partitions[partition_idx].runtime_data.
            caller_client_id = caller_client_id;
}

uint32_t tfm_spm_partition_get_running_partition_idx(void)
{
    return g_spm_partition_db.running_partition_idx;
}

void tfm_spm_partition_cleanup_context(uint32_t partition_idx)
{
    struct spm_partition_desc_t *partition =
            &(g_spm_partition_db.partitions[partition_idx]);
    int32_t i;

    partition->runtime_data.caller_partition_idx = SPM_INVALID_PARTITION_IDX;
    partition->runtime_data.iovec_args.in_len = 0;
    for (i = 0; i < PSA_MAX_IOVEC; ++i) {
        partition->runtime_data.iovec_args.in_vec[i].base = 0;
        partition->runtime_data.iovec_args.in_vec[i].len = 0;
    }
    partition->runtime_data.iovec_args.out_len = 0;
    for (i = 0; i < PSA_MAX_IOVEC; ++i) {
        partition->runtime_data.iovec_args.out_vec[i].base = 0;
        partition->runtime_data.iovec_args.out_vec[i].len = 0;
    }
    partition->runtime_data.orig_outvec = 0;
}

enum spm_err_t tfm_spm_db_init(void)
{
    uint32_t i;

    /* This function initialises partition db */

    /* For the non secure Execution environment */
    tfm_nspm_ctx_init();

    for (i = 0; i < g_spm_partition_db.partition_count; i++) {
        g_spm_partition_db.partitions[i].runtime_data.partition_state =
            SPM_PARTITION_STATE_UNINIT;
        g_spm_partition_db.partitions[i].runtime_data.caller_partition_idx =
            SPM_INVALID_PARTITION_IDX;
        g_spm_partition_db.partitions[i].runtime_data.caller_client_id =
            TFM_INVALID_CLIENT_ID;
        g_spm_partition_db.partitions[i].runtime_data.ctx_stack_ptr =
            ctx_stack_list[i];
        g_spm_partition_db.partitions[i].static_data = &static_data_list[i];
        g_spm_partition_db.partitions[i].platform_data_list =
                                                     platform_data_list_list[i];
    }
    g_spm_partition_db.is_init = 1;

    return SPM_ERR_OK;
}
