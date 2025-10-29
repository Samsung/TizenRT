/*
 * Copyright (c) 2017-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "fih.h"
#include "ffm/tfm_boot_data.h"
#include "compile_check_defs.h"
#include "region.h"
#include "spm_ipc.h"
#include "tfm_hal_isolation.h"
#include "tfm_hal_platform.h"
#include "tfm_spm_hal.h"
#include "tfm_spm_log.h"
#include "tfm_version.h"
#include "tfm_plat_otp.h"
#include "tfm_plat_provisioning.h"
#include "sys_hal.h"
#include "flash_partition.h"
#include "flash_driver.h"
#include "soc/bk7239n/wdt.h"
/*
 * Avoids the semihosting issue
 * FixMe: describe 'semihosting issue'
 */
#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
__asm("  .global __ARM_use_no_argv\n");
#endif

#ifndef TFM_LVL
#error TFM_LVL is not defined!
#elif (TFM_LVL != 1) && (TFM_LVL != 2) && (TFM_LVL != 3)
#error Invalid TFM_LVL value. Only TFM_LVL 1, 2 and 3 are supported in IPC model!
#endif

REGION_DECLARE(Image$$, ARM_LIB_STACK, $$ZI$$Base);

#ifdef  CONFIG_STARTUP_PERFORMANCE
#define STARTUP_PERF(gpio_id) GPIO_UP(gpio_id)
#else
#define STARTUP_PERF(gpio_id) 
#endif

static fih_int tfm_core_init(void)
{
    enum tfm_plat_err_t plat_err = TFM_PLAT_ERR_SYSTEM_ERR;
#ifdef TFM_FIH_PROFILE_ON
    fih_int fih_rc = FIH_FAILURE;
#else
    enum tfm_hal_status_t hal_status = TFM_HAL_ERROR_GENERIC;
#endif

    plat_err = bk_flash_driver_init();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }   

    plat_err = partition_init();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
        FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
    }   

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
    FIH_CALL(tfm_hal_verify_static_boundaries, fih_rc);
    if (fih_not_eq(fih_rc, fih_int_encode(TFM_HAL_SUCCESS))) {
        tfm_core_panic();
    }
#endif

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

#if TFM_PROVISIONING
    /* Perform provisioning. */
    if (tfm_plat_provisioning_is_required()) {
        plat_err = tfm_plat_provisioning_perform();
        if (plat_err != TFM_PLAT_ERR_SUCCESS) {
            FIH_RET(fih_int_encode(TFM_ERROR_GENERIC));
        }
    } else {
        tfm_plat_provisioning_check_for_dummy_keys();
    }
#endif

    /* Configures architecture */
    tfm_arch_config_extensions();

    SPMLOG_INFMSG("[Sec Thread] Secure image initializing!\r\n");

    SPMLOG_DBGMSGVAL("TF-M isolation level is: ", TFM_LVL);

#if (CONFIG_TFM_FP == 2)
    SPMLOG_INFMSG("TF-M FP mode: Hardware\r\n");
#ifdef CONFIG_TFM_LAZY_STACKING
    SPMLOG_INFMSG("Lazy stacking enabled\r\n");
#else
    SPMLOG_INFMSG("Lazy stacking disabled\r\n");
#endif
#endif

    tfm_core_validate_boot_data();

    FIH_RET(fih_int_encode(TFM_SUCCESS));
}

#if CONFIG_TFM_FAST_BOOT_DEBUG
uint32_t s_loop = 1;
#endif

void tfm_enable_swd(void);
int main(void)
{
    STARTUP_PERF(15);
    fih_int fih_rc = FIH_FAILURE;

    close_wdt();
#if CONFIG_BL2_WDT
    update_wdt(CONFIG_BL2_WDT_PERIOD);
#endif
    /* set Main Stack Pointer limit */
    tfm_arch_set_msplim((uint32_t)&REGION_NAME(Image$$, ARM_LIB_STACK, $$ZI$$Base));

#if CONFIG_TFM_FAST_BOOT_DEBUG
    /* For normal reset, bl2 will enable the SWD by default if secure debug is not enabled, 
     * for deep sleep fast boot reset, we need to enable SWD here
     */
    tfm_enable_swd();
    if ( (*(volatile uint32_t *) 0x44000000) & 2) {
    	SPMLOG_INFMSG("fast boot\r\n");
	while(s_loop);
    }
#endif

    // sys_hal_early_init_tfm();
    fih_delay_init();

    FIH_CALL(tfm_core_init, fih_rc);
    if (fih_not_eq(fih_rc, fih_int_encode(TFM_SUCCESS))) {
        tfm_core_panic();
    }

    /* All isolation should have been set up at this point */
    FIH_LABEL_CRITICAL_POINT();

    /* Print the TF-M version */
    SPMLOG_INFMSG("Booting TF-M "VERSION_FULLSTR"\r\n");

    /*
     * Prioritise secure exceptions to avoid NS being able to pre-empt
     * secure SVC or SecureFault. Do it before PSA API initialization.
     */
    tfm_arch_set_secure_exception_priorities();

#if (CONFIG_TFM_FP >= 1)
    tfm_arch_clear_fp_data();
#endif

    tfm_arch_clear_fp_status();

    /* Move to handler mode for further SPM initialization. */
    tfm_core_handler_mode();

    return 0;
}
