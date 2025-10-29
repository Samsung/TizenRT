/*
 * Copyright (c) 2017-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <string.h>
#include "region.h"
#include "spm_ipc.h"
#include "svc_num.h"
#include "tfm_api.h"
#include "tfm_arch.h"
#include "tfm_core_trustzone.h"
#include "tfm_core_utils.h"
#include "tfm_svcalls.h"
#include "utilities.h"
#include "load/spm_load_api.h"
#include "ffm/interrupt.h"
#include "ffm/tfm_boot_data.h"
#include "ffm/psa_api.h"
#include "tfm_hal_isolation.h"
#include "tfm_hal_spm_logdev.h"
#include "load/partition_defs.h"
#include "psa/client.h"
#include "tfm_hal_platform.h"

/* MSP bottom (higher address) */
REGION_DECLARE(Image$$, ARM_LIB_STACK, $$ZI$$Limit);

#ifdef PLATFORM_SVC_HANDLERS
extern int32_t platform_svc_handlers(uint8_t svc_num,
                                     uint32_t *ctx, uint32_t lr);
#endif

static int32_t SVC_Handler_IPC(uint8_t svc_num, uint32_t *ctx,
                               uint32_t lr)
{
    psa_status_t status = PSA_SUCCESS;

    switch (svc_num) {
    case TFM_SVC_PSA_FRAMEWORK_VERSION:
        return tfm_spm_client_psa_framework_version();
    case TFM_SVC_PSA_VERSION:
        return tfm_spm_client_psa_version(ctx[0]);
    case TFM_SVC_PSA_CALL:
        status = tfm_spm_client_psa_call((psa_handle_t)ctx[0], ctx[1],
                                         (const psa_invec *)ctx[2],
                                         (psa_outvec *)ctx[3]);
        break;
    case TFM_SVC_PSA_READ:
        return tfm_spm_partition_psa_read((psa_handle_t)ctx[0], ctx[1],
                                          (void *)ctx[2], (size_t)ctx[3]);
    case TFM_SVC_PSA_SKIP:
        return tfm_spm_partition_psa_skip((psa_handle_t)ctx[0], ctx[1],
                                          (size_t)ctx[2]);
    case TFM_SVC_PSA_WRITE:
        tfm_spm_partition_psa_write((psa_handle_t)ctx[0], ctx[1],
                                    (void *)ctx[2], (size_t)ctx[3]);
        break;
#if CONFIG_TFM_DOORBELL_API == 1
    case TFM_SVC_PSA_NOTIFY:
        tfm_spm_partition_psa_notify((int32_t)ctx[0]);
        break;
    case TFM_SVC_PSA_CLEAR:
        tfm_spm_partition_psa_clear();
        break;
#endif /* CONFIG_TFM_DOORBELL_API == 1 */
    case TFM_SVC_PSA_PANIC:
        tfm_spm_partition_psa_panic();
        break;
    case TFM_SVC_PSA_LIFECYCLE:
        return tfm_spm_get_lifecycle_state();
/* Following svc handlers are only needed by connection-based services */
#if CONFIG_TFM_CONNECTION_BASED_SERVICE_API == 1
    case TFM_SVC_PSA_CONNECT:
        status = tfm_spm_client_psa_connect(ctx[0], ctx[1]);
        break;
    case TFM_SVC_PSA_CLOSE:
        status = tfm_spm_client_psa_close((psa_handle_t)ctx[0]);
        break;
    case TFM_SVC_PSA_SET_RHANDLE:
        tfm_spm_partition_psa_set_rhandle((psa_handle_t)ctx[0], (void *)ctx[1]);
        break;
#endif /* CONFIG_TFM_CONNECTION_BASED_SERVICE_API */
/* These APIs are only used in IPC backend. */
#if CONFIG_TFM_SPM_BACKEND_IPC == 1
    case TFM_SVC_PSA_GET:
        return tfm_spm_partition_psa_get((psa_signal_t)ctx[0],
                                         (psa_msg_t *)ctx[1]);
    case TFM_SVC_PSA_REPLY:
        tfm_spm_partition_psa_reply((psa_handle_t)ctx[0], (psa_status_t)ctx[1]);
        break;
    case TFM_SVC_PSA_WAIT:
        return tfm_spm_partition_psa_wait((psa_signal_t)ctx[0], ctx[1]);
#endif /* CONFIG_TFM_SPM_BACKEND_IPC == 1 */

#if CONFIG_TFM_FLIH_API == 1 || CONFIG_TFM_SLIH_API == 1
    case TFM_SVC_PSA_IRQ_ENABLE:
        tfm_spm_partition_psa_irq_enable((psa_signal_t)ctx[0]);
        break;
    case TFM_SVC_PSA_IRQ_DISABLE:
        return tfm_spm_partition_psa_irq_disable((psa_signal_t)ctx[0]);
/* This API is only used for FLIH. */
#if CONFIG_TFM_FLIH_API == 1
    case TFM_SVC_PSA_RESET_SIGNAL:
        tfm_spm_partition_psa_reset_signal((psa_signal_t)ctx[0]);
        break;
#endif
/* This API is only used for SLIH. */
#if CONFIG_TFM_SLIH_API == 1
    case TFM_SVC_PSA_EOI:
        tfm_spm_partition_psa_eoi((psa_signal_t)ctx[0]);
        break;
#endif
#endif /* CONFIG_TFM_FLIH_API == 1 || CONFIG_TFM_SLIH_API == 1 */

#if TFM_SP_LOG_RAW_ENABLED
    case TFM_SVC_OUTPUT_UNPRIV_STRING:
        return tfm_hal_output_spm_log((const char *)ctx[0], ctx[1]);
#endif
    default:
#ifdef PLATFORM_SVC_HANDLERS
        return (platform_svc_handlers(svc_num, ctx, lr));
#else
        SPMLOG_ERRMSG("Unknown SVC number requested!\r\n");
        return PSA_ERROR_GENERIC_ERROR;
#endif
    }
    __DSB();
    __ISB();
    spm_handle_programmer_errors(status);
    return status;
}

uint32_t tfm_core_svc_handler(uint32_t *msp, uint32_t exc_return,
                              uint32_t *psp)
{
    uint8_t svc_number = TFM_SVC_PSA_FRAMEWORK_VERSION;
    uint32_t *svc_args = msp;

    if ((exc_return & EXC_RETURN_MODE) && (exc_return & EXC_RETURN_SPSEL)) {
        /* Use PSP when both EXC_RETURN.MODE and EXC_RETURN.SPSEL are set */
        svc_args = psp;
    }

    /*
     * Stack contains:
     * r0, r1, r2, r3, r12, r14 (lr), the return address and xPSR
     * First argument (r0) is svc_args[0]
     */
    if (is_return_secure_stack(exc_return)) {
        /* SV called directly from secure context. Check instruction for
         * svc_number
         */
        svc_number = ((uint8_t *)svc_args[6])[-2];
    } else {
        /* Secure SV executing with NS return.
         * NS cannot directly trigger S SVC so this should not happen. This is
         * an unrecoverable error.
         */
        tfm_core_panic();
    }

    if (!(exc_return & EXC_RETURN_MODE)
                                  != (svc_number > TFM_SVC_THREAD_NUMBER_END)) {
        tfm_core_panic();
    }

    switch (svc_number) {
    case TFM_SVC_SPM_INIT:
        exc_return = tfm_spm_init();
        /* The following call does not return */
        tfm_arch_free_msp_and_exc_ret(
            (uint32_t)&REGION_NAME(Image$$, ARM_LIB_STACK, $$ZI$$Limit),
            exc_return);
        break;
    case TFM_SVC_GET_BOOT_DATA:
        tfm_core_get_boot_data_handler(svc_args);
        break;
#if TFM_LVL != 1
    case TFM_SVC_PREPARE_DEPRIV_FLIH:
        exc_return = tfm_flih_prepare_depriv_flih(
                                            (struct partition_t *)svc_args[0],
                                            (uintptr_t)svc_args[1]);
        break;
    case TFM_SVC_FLIH_FUNC_RETURN:
        exc_return = tfm_flih_return_to_isr(svc_args[0],
                                            (struct context_flih_ret_t *)msp);
        break;
#endif
    default:
        if (((uint32_t)&REGION_NAME(Image$$, ARM_LIB_STACK, $$ZI$$Limit)
                                     - (uint32_t)msp) > TFM_STACK_SEALED_SIZE) {
            /* The Main Stack has contents, not calling from Partition thread */
            tfm_core_panic();
        }
        svc_args[0] = SVC_Handler_IPC(svc_number, svc_args, exc_return);

        if (THRD_EXPECTING_SCHEDULE()) {
            tfm_arch_trigger_pendsv();
        }

        break;
    }

    return exc_return;
}

__attribute__ ((naked)) void tfm_core_handler_mode(void)
{
    __ASM volatile("SVC %0           \n"
                   "BX LR            \n"
                   : : "I" (TFM_SVC_SPM_INIT));
}

void tfm_access_violation_handler(void)
{
    tfm_hal_system_halt();
}
