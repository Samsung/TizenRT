/*
 * Copyright (c) 2017-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "arch.h"
#include "fih.h"
#include "ffm/tfm_boot_data.h"
#include "region.h"
#include "spm_func.h"
#include "tfm_hal_defs.h"
#include "tfm_hal_platform.h"
#include "tfm_hal_isolation.h"
#include "tfm_irq_list.h"
#include "tfm_spm_hal.h"
#include "tfm_spm_log.h"
#include "tfm_version.h"
#include "tfm_plat_otp.h"
#include "tfm_plat_provisioning.h"

/*
 * Avoids the semihosting issue
 * FixMe: describe 'semihosting issue'
 */
#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
__asm("  .global __ARM_use_no_argv\n");
#endif

#ifndef TFM_LVL
#error TFM_LVL is not defined!
#elif (TFM_LVL != 1)
#error Only TFM_LVL 1 is supported for library model!
#endif

REGION_DECLARE(Image$$, ARM_LIB_STACK,  $$ZI$$Base);
REGION_DECLARE(Image$$, ARM_LIB_STACK,  $$ZI$$Limit)[];
REGION_DECLARE(Image$$, ER_INITIAL_PSP,  $$ZI$$Limit)[];

static void configure_ns_code(void)
{
    /* SCB_NS.VTOR points to the Non-secure vector table base address */
    SCB_NS->VTOR = tfm_spm_hal_get_ns_VTOR();

    /* Setups Main stack pointer of the non-secure code */
    uint32_t ns_msp = tfm_spm_hal_get_ns_MSP();

    __TZ_set_MSP_NS(ns_msp);

    /* Get the address of non-secure code entry point to jump there */
    uint32_t entry_ptr = tfm_spm_hal_get_ns_entry_point();

    /* Clears LSB of the function address to indicate the function-call
     * will perform the switch from secure to non-secure
     */
    ns_entry = (nsfptr_t)cmse_nsfptr_create(entry_ptr);
}

static fih_int tfm_core_init(void)
{
    size_t i;
    enum tfm_hal_status_t hal_status = TFM_HAL_ERROR_GENERIC;
    enum tfm_plat_err_t plat_err = TFM_PLAT_ERR_SYSTEM_ERR;
    enum irq_target_state_t irq_target_state = TFM_IRQ_TARGET_STATE_SECURE;
#ifdef TFM_FIH_PROFILE_ON
    fih_int fih_rc = FIH_FAILURE;
#endif

    /*
     * Access to any peripheral should be performed after programming
     * the necessary security components such as PPC/SAU.
     */
#ifdef TFM_FIH_PROFILE_ON
    FIH_CALL(tfm_hal_set_up_static_boundaries, fih_rc);
    if (fih_not_eq(fih_rc, fih_int_encode(TFM_HAL_SUCCESS))) {
        FIH_RET(fih_int_encode(TFM_ERROR_GENERIC));
    }
#else /* TFM_FIH_PROFILE_ON */
    hal_status = tfm_hal_set_up_static_boundaries();
    if (hal_status != TFM_HAL_SUCCESS) {
        return TFM_ERROR_GENERIC;
    }
#endif /* TFM_FIH_PROFILE_ON */

#ifdef TFM_FIH_PROFILE_ON
    FIH_CALL(tfm_hal_platform_init, fih_rc);
    if (fih_not_eq(fih_rc, fih_int_encode(TFM_HAL_SUCCESS))) {
        FIH_RET(fih_int_encode(TFM_ERROR_GENERIC));
    }
#else /* TFM_FIH_PROFILE_ON */
    hal_status = tfm_hal_platform_init();
    if (hal_status != TFM_HAL_SUCCESS) {
        return TFM_ERROR_GENERIC;
    }
#endif /* TFM_FIH_PROFILE_ON */

    plat_err = tfm_plat_otp_init();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
            FIH_RET(fih_int_encode(TFM_ERROR_GENERIC));
    }

    /* Perform provisioning. */
    if (tfm_plat_provisioning_is_required()) {
        plat_err = tfm_plat_provisioning_perform();
        if (plat_err != TFM_PLAT_ERR_SUCCESS) {
            FIH_RET(fih_int_encode(TFM_ERROR_GENERIC));
        }
    } else {
        tfm_plat_provisioning_check_for_dummy_keys();
    }

    /* Configures architecture */
    tfm_arch_config_extensions();

    SPMLOG_INFMSG("\033[1;34m[Sec Thread] Secure image initializing!\033[0m\r\n");

    SPMLOG_DBGMSGVAL("TF-M isolation level is: ", TFM_LVL);

    tfm_core_validate_boot_data();

    configure_ns_code();

    for (i = 0; i < tfm_core_irq_signals_count; ++i) {
        plat_err = tfm_spm_hal_set_secure_irq_priority(
                                          tfm_core_irq_signals[i].irq_line);
        if (plat_err != TFM_PLAT_ERR_SUCCESS) {
            FIH_RET(fih_int_encode(TFM_ERROR_GENERIC));
        }
        irq_target_state = tfm_spm_hal_set_irq_target_state(
                                          tfm_core_irq_signals[i].irq_line,
                                          TFM_IRQ_TARGET_STATE_SECURE);
        if (irq_target_state != TFM_IRQ_TARGET_STATE_SECURE) {
            FIH_RET(fih_int_encode(TFM_ERROR_GENERIC));
        }
    }

    FIH_RET(fih_int_encode(TFM_SUCCESS));
}

__attribute__((naked))
int main(void)
{
    __ASM volatile(
#if !defined(__ICCARM__)
        ".syntax unified                               \n"
#endif
        "msr     msp, %0                               \n"
        "msr     psp, %1                               \n"
        "mrs     r0, control                           \n"
        "movs    r1, #2                                \n"
        "orrs    r0, r0, r1                            \n" /* Switch to PSP */
        "msr     control, r0                           \n"
        "bl      c_main                                \n"
        :
        : "r" (REGION_NAME(Image$$, ARM_LIB_STACK, $$ZI$$Limit)),
          "r" (REGION_NAME(Image$$, ER_INITIAL_PSP, $$ZI$$Limit))
        : "r0", "memory"
    );
}

int c_main(void)
{
    enum spm_err_t spm_err = SPM_ERR_GENERIC_ERR;
    fih_int fih_rc = FIH_FAILURE;

    /* set Main Stack Pointer limit */
    tfm_arch_init_secure_msp((uint32_t)&REGION_NAME(Image$$,
                                                    ARM_LIB_STACK,
                                                    $$ZI$$Base));

    /* Seal the PSP stacks viz ARM_LIB_STACK and TFM_SECURE_STACK */
    tfm_spm_seal_psp_stacks();

    fih_delay_init();

    FIH_CALL(tfm_core_init, fih_rc);
    if (fih_not_eq(fih_rc, fih_int_encode(TFM_SUCCESS))) {
        tfm_core_panic();
    }

    /* All isolation should have been set up at this point */
    FIH_LABEL_CRITICAL_POINT();

    /* Print the TF-M version */
    SPMLOG_INFMSG("\033[1;34mBooting TF-M "VERSION_FULLSTR"\033[0m\r\n");

    spm_err = tfm_spm_db_init();
    if (spm_err != SPM_ERR_OK) {
        tfm_core_panic();
    }

    tfm_spm_partition_set_state(TFM_SP_CORE_ID, SPM_PARTITION_STATE_RUNNING);

    REGION_DECLARE(Image$$, ER_INITIAL_PSP, $$ZI$$Base)[];
    uint32_t psp_stack_bottom =
                      (uint32_t)REGION_NAME(Image$$, ER_INITIAL_PSP, $$ZI$$Base);

    tfm_arch_set_psplim(psp_stack_bottom);

    FIH_CALL(tfm_spm_partition_init, fih_rc);
    if (fih_not_eq(fih_rc, fih_int_encode(SPM_ERR_OK))) {
        /* Certain systems might refuse to boot altogether if partitions fail
         * to initialize. This is a placeholder for such an error handler
         */
    }

    /*
     * Prioritise secure exceptions to avoid NS being able to pre-empt
     * secure SVC or SecureFault. Do it before PSA API initialization.
     */
    tfm_arch_set_secure_exception_priorities();

    /* We close the TFM_SP_CORE_ID partition, because its only purpose is
     * to be able to pass the state checks for the tests started from secure.
     */
    tfm_spm_partition_set_state(TFM_SP_CORE_ID, SPM_PARTITION_STATE_CLOSED);
    tfm_spm_partition_set_state(TFM_SP_NON_SECURE_ID,
                                SPM_PARTITION_STATE_RUNNING);

#ifdef TFM_FIH_PROFILE_ON
    FIH_CALL(tfm_hal_verify_static_boundaries, fih_rc);
    if (fih_not_eq(fih_rc, fih_int_encode(TFM_HAL_SUCCESS))) {
        tfm_core_panic();
    }
#endif

#ifdef TFM_CORE_DEBUG
    /* Jumps to non-secure code */
    SPMLOG_DBGMSG("\033[1;34mJumping to non-secure code...\033[0m\r\n");
#endif

    jump_to_ns_code();

    return 0;
}
