/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 * Copyright (c) 2017-2022 Arm Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mcuboot_config/mcuboot_config.h"
#include <assert.h>
#include "target.h"
#include "tfm_hal_device_header.h"
#include "mbedtls/memory_buffer_alloc.h"
#include "bootutil/security_cnt.h"
#include "bootutil/bootutil_log.h"
#include "bootutil/image.h"
#include "bootutil/bootutil.h"
#include "bootutil/boot_record.h"
#include "bootutil/fault_injection_hardening.h"
#include "flash_map_backend/flash_map_backend.h"
#include "boot_hal.h"
#include "uart_stdout.h"
#include "tfm_plat_otp.h"
#include "tfm_plat_provisioning.h"
#include "sdkconfig.h"
#include "partitions_gen.h"
#include "flash_partition.h"
#include "aon_pmu_hal.h"
#include "driver/flash.h"
#include <common/bk_err.h>
#include <stdint.h>
#include "soc/bk7239n/soc_debug.h"
#include "soc/bk7239n/wdt.h"

#ifdef TEST_BL2
#include "mcuboot_suites.h"
#endif /* TEST_BL2 */
#include "sys_hal.h"

/* Avoids the semihosting issue */
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
__asm("  .global __ARM_use_no_argv\n");
#endif

#ifdef MCUBOOT_ENCRYPT_RSA
#define BL2_MBEDTLS_MEM_BUF_LEN 0x3000
#else
#define BL2_MBEDTLS_MEM_BUF_LEN 0x2000
#endif

#define HDR_SZ                  0x1000
/* Static buffer to be used by mbedtls for memory allocation */
static uint8_t mbedtls_mem_buf[BL2_MBEDTLS_MEM_BUF_LEN];

/*================FUNCTION DECLARATION SECTION START========*/
extern void sys_hal_dpll_cpu_flash_time_early_init(void);
extern bk_err_t sys_hal_ctrl_vdddig_h_vol(uint32_t vol_value);
extern void sys_hal_switch_freq(uint32_t cksel_core, uint32_t ckdiv_core, uint32_t ckdiv_cpu0);
extern void close_wdt(void);
extern int flash_map_init(void);
extern void enable_dcache(int enable);
/*================FUNCTION DECLARATION SECTION END========*/

extern void flash_set_excute_enable(int enable);
extern int bl2_get_active_slot_without_signing(void);

static void do_boot(struct boot_rsp *rsp)
{
    struct boot_arm_vector_table *vt;
    uintptr_t flash_base;
    int rc;

    /* The beginning of the image is the ARM vector table, containing
     * the initial stack pointer address and the reset vector
     * consecutively. Manually set the stack pointer and jump into the
     * reset vector
     */
    rc = flash_device_base(rsp->br_flash_dev_id, &flash_base);
    assert(rc == 0);

    if (rsp->br_hdr->ih_flags & IMAGE_F_RAM_LOAD) {
       /* The image has been copied to SRAM, find the vector table
        * at the load address instead of image's address in flash
        */
        vt = (struct boot_arm_vector_table *)(rsp->br_hdr->ih_load_addr +
                                         rsp->br_hdr->ih_hdr_size);
    } else {
        /* Using the flash address as not executing in SRAM */
#if CONFIG_OTA_OVERWRITE || CONFIG_DIRECT_XIP
    extern uint32_t get_flash_map_offset(uint32_t index);
    uint32_t primary_off  = get_flash_map_offset(0);
#if CONFIG_CPU_CRC_EN
    vt = (struct boot_arm_vector_table *)(flash_base + 
                    FLASH_PHY2VIRTUAL_CODE_START(primary_off +
                                         rsp->br_hdr->ih_hdr_size / 32 * 34));
#else
    vt = (struct boot_arm_vector_table *)(flash_base + 
                    FLASH_PHY2VIRTUAL_CODE_START(primary_off + rsp->br_hdr->ih_hdr_size));
#endif
#else
    vt = (struct boot_arm_vector_table *)(flash_base + 
                    FLASH_PHY2VIRTUAL_CODE_START(rsp->br_image_off +
                                                    rsp->br_hdr->ih_hdr_size));
#endif
    }

#if MCUBOOT_LOG_LEVEL > MCUBOOT_LOG_LEVEL_OFF || TEST_BL2
#if CONFIG_BL2_CLEAN_UART_ON_EXIT
    stdio_uninit();
#endif
#endif

    /* This function never returns, because it calls the secure application
     * Reset_Handler(). 
     */
    boot_platform_quit(vt);
}

static void do_boot_without_signing(void)
{
    struct boot_arm_vector_table *vt;
    uint8_t active_slot = bl2_get_active_slot_without_signing();
    if(active_slot == UINT32_MAX)
    {
        update_wdt(0);
        while(1);
    }

    if (active_slot == 0) {
        BOOT_LOG_FORCE("\nJumping to the first slot");
    }
    else {
        BOOT_LOG_FORCE("\nJumping to the second slot");

    }
    flash_set_excute_enable(active_slot);

    extern uint32_t get_flash_map_offset(uint32_t index);
    uint32_t primary_off  = get_flash_map_offset(0);
    BOOT_LOG_FORCE("primary_off: 0x%x\r\n", primary_off);
    vt = (struct boot_arm_vector_table *)(FLASH_BASE_ADDRESS + (primary_off + HDR_SZ));
    BOOT_LOG_FORCE("vt: 0x%p\r\n", vt);
    
    boot_platform_quit(vt);
}

// Set CONFIG_DOWNLOAD_LOG to 1 to enable printf in download to debug
#define CONFIG_DOWNLOAD_LOG 1

extern uint32_t sys_is_enable_fast_boot(void);
extern uint32_t sys_is_running_from_deep_sleep(void);
#if CONFIG_BL2_UPDATE_WITH_PC
extern uint8_t bl_get_boot_flag_value(void);
#endif
extern  int bl_operate_bootloader_version(void);
extern uint32_t flash_get_excute_enable();
extern void flash_set_excute_enable(int enable);

static void deep_sleep_reset(void)
{
    struct boot_arm_vector_table *vt;

    if (sys_is_running_from_deep_sleep() && sys_is_enable_fast_boot()) {
        BOOT_LOG_INF("deep sleep fastboot");
        extern uint32_t get_flash_map_offset(uint32_t index);
        uint32_t phy_offset  = get_flash_map_offset(0);
        uint32_t virtual_off = FLASH_PHY2VIRTUAL(phy_offset);
        vt = (struct boot_arm_vector_table *)(FLASH_DEVICE_BASE +
                    FLASH_CEIL_ALIGN(virtual_off + HDR_SZ, CPU_VECTOR_ALIGN_SZ));
#if CONFIG_DIRECT_XIP
        uint32_t candidate_slot = 0;
        uint32_t reg = aon_pmu_ll_get_r7b();
        aon_pmu_ll_set_r0(reg);
        candidate_slot = !!(reg & BIT(2));
        extern void flash_set_excute_enable(int enable);
        flash_set_excute_enable(candidate_slot);
#endif
        boot_platform_quit(vt);
    }
}

int main(void)
{
    struct boot_rsp rsp;
    fih_int fih_rc = FIH_FAILURE;
    enum tfm_plat_err_t plat_err;

    bk_efuse_init();
#if CONFIG_BL2_SECURE_DEBUG
    extern void hal_secure_debug(void);
    hal_secure_debug(); 
#endif

#if CONFIG_DOWNLOAD_LOG
    stdio_init();
#endif

    BOOT_LOG_FORCE(" main 0x%p \r\n", main);
    /* Perform platform specific initialization */
    if (boot_platform_init() != 0) {
        BOOT_LOG_ERR("Platform init failed");
        FIH_PANIC;
    }

#if CONFIG_BL2_UPDATE_WITH_PC
    bl_get_boot_flag_value();
#endif

#if CONFIG_BL2_DOWNLOAD
    if (efuse_is_secure_download_enabled()) {
        flash_switch_to_line_mode_two();
        void legacy_boot_main(void);
        legacy_boot_main();
        flash_restore_line_mode();
    }
#endif
#if CONFIG_BL2_UPDATE_WITH_PC
    bl_operate_bootloader_version();
#endif
#if CONFIG_BL2_WDT
    update_wdt(CONFIG_BL2_WDT_PERIOD);
#endif

    /* Initialise the mbedtls static memory allocator so that mbedtls allocates
     * memory from the provided static buffer instead of from the heap.
     */
    mbedtls_memory_buffer_alloc_init(mbedtls_mem_buf, BL2_MBEDTLS_MEM_BUF_LEN);

#if !CONFIG_DOWNLOAD_LOG
#if MCUBOOT_LOG_LEVEL > MCUBOOT_LOG_LEVEL_OFF || TEST_BL2
    stdio_init();
#endif
#endif
    BOOT_LOG_FORCE("Starting bootloader\r\n");

    partition_init();
    flash_map_init();
    deep_sleep_reset();

	BOOT_LOG_FORCE("Init0\r\n");
    dump_partition();
    dump_efuse();
    plat_err = tfm_plat_otp_init();
    if (plat_err != TFM_PLAT_ERR_SUCCESS) {
            BOOT_LOG_ERR("OTP system initialization failed");
            FIH_PANIC;
    }

#if TFM_PROVISIONING
    if (tfm_plat_provisioning_is_required()) {
        plat_err = tfm_plat_provisioning_perform();
        if (plat_err != TFM_PLAT_ERR_SUCCESS) {
            BOOT_LOG_ERR("Provisioning failed");
            FIH_PANIC;
        }
    } else {
        tfm_plat_provisioning_check_for_dummy_keys();
    }
#endif

    FIH_CALL(boot_nv_security_counter_init, fih_rc);
    if (fih_not_eq(fih_rc, FIH_SUCCESS)) {
        BOOT_LOG_ERR("Error while initializing the security counter");
        FIH_PANIC;
    }

    /* Perform platform specific post-initialization */
    if (boot_platform_post_init() != 0) {
        BOOT_LOG_ERR("Platform post init failed");
        FIH_PANIC;
    }

#ifdef TEST_BL2
    (void)run_mcuboot_testsuite();
#endif /* TEST_BL2 */

    update_wdt(CONFIG_BL2_WDT_PERIOD);

#if defined(CONFIG_BINARY_SIGNING) && (CONFIG_BINARY_SIGNING == 1)
    FIH_CALL(boot_go, fih_rc, &rsp);
    if (fih_not_eq(fih_rc, FIH_SUCCESS)) {
        BOOT_LOG_ERR("Unable to find bootable image");
        FIH_PANIC;
    }

    BOOT_LOG_FORCE("tfm_addr_offset:[0x%x]\n", rsp.br_image_off);
	if(boot_active_slot_get() == 0)
	{
		BOOT_LOG_FORCE("\nJumping to the first slot");
	}
	else
	{
		BOOT_LOG_FORCE("\nJumping to the second slot");
	}

    do_boot(&rsp);
#else
    do_boot_without_signing();
#endif

    BOOT_LOG_ERR("Never should get here");
    FIH_PANIC;
}
// eof

