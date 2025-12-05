/*
 * Copyright (c) 2019-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "target_cfg.h"
#include "region.h"
#include "cmsis.h"
#include "boot_hal.h"
#include "Driver_Flash.h"
#include "flash_layout.h"
#include "region_defs.h"
#include "uart_stdout.h"
#ifdef CRYPTO_HW_ACCELERATOR
#include "crypto_hw.h"
#include "fih.h"
#endif /* CRYPTO_HW_ACCELERATOR */

/* Flash device name must be specified by target */
extern ARM_DRIVER_FLASH FLASH_DEV_NAME;

REGION_DECLARE(Image$$, ER_DATA, $$Base)[];
REGION_DECLARE(Image$$, ARM_LIB_HEAP, $$ZI$$Limit)[];
#if defined(__ARM_ARCH_8M_MAIN__) || defined(__ARM_ARCH_8M_BASE__) \
 || defined(__ARM_ARCH_8_1M_MAIN__)
REGION_DECLARE(Image$$, ARM_LIB_STACK, $$ZI$$Base);
#endif /* defined(__ARM_ARCH_8M_MAIN__) || defined(__ARM_ARCH_8M_BASE__) \
       || defined(__ARM_ARCH_8_1M_MAIN__) */

uint32_t platform_code_is_bl1_2 = 0;

static const uint32_t bl1_1_data_start = BL1_1_DATA_START;
static const uint32_t bl1_1_data_limit = BL1_1_DATA_START + BL1_1_DATA_SIZE;
static const uint32_t bl1_2_data_start = BL1_2_DATA_START;
static const uint32_t bl1_2_data_limit = BL1_2_DATA_START + BL1_2_DATA_SIZE;

/* Erase both sections at the end of BL1_2 */
__WEAK __attribute__((naked)) void boot_clear_ram_area(void)
{
    __ASM volatile(
#if !defined(__ICCARM__)
        ".syntax unified                             \n"
#endif
        /* If platform_code_is_bl1_2 isn't set, don't clear anything to allow
         * code-sharing to work correctly.
         */
        "ldr     r0, %0                              \n"
        "cmp     r0, #0                              \n"
        "beq     Clear_done_1                        \n"
        /* Clear the entire data section */
        "movs    r0, #0                              \n"
        "ldr     r1, %1                              \n"
        "ldr     r2, %2                              \n"
        "subs    r2, r2, r1                          \n"
        "Loop_1:                                     \n"
        "subs    r2, #4                              \n"
        "blt     Clear_done_1                        \n"
        "str     r0, [r1, r2]                        \n"
        "b       Loop_1                              \n"
        "Clear_done_1:                               \n"
        "nop \n"
         :
         : "m" (platform_code_is_bl1_2),
           "m" (bl1_1_data_start), "m" (bl1_1_data_limit)
         : "r0" , "r1" , "r2" , "memory"
    );

    __ASM volatile(
#if !defined(__ICCARM__)
        ".syntax unified                             \n"
#endif
        /* If platform_code_is_bl1_2 isn't set, don't clear anything to allow
         * code-sharing to work correctly.
         */
        "ldr     r0, %0                              \n"
        "cmp     r0, #0                              \n"
        "beq     Clear_done_2                        \n"
        /* Clear the entire data section */
        "movs    r0, #0                              \n"
        "ldr     r1, %1                              \n"
        "ldr     r2, %2                              \n"
        "subs    r2, r2, r1                          \n"
        "Loop_2:                                     \n"
        "subs    r2, #4                              \n"
        "blt     Clear_done_2                        \n"
        "str     r0, [r1, r2]                        \n"
        "b       Loop_2                              \n"
        "Clear_done_2:                               \n"
        "bx lr                                       \n"
         :
         : "m" (platform_code_is_bl1_2),
           "m" (bl1_2_data_start), "m" (bl1_2_data_limit)
         : "r0" , "r1" , "r2" , "memory"
    );
}

/*!
 * \brief Chain-loading the next image in the boot sequence.
 *
 * This function calls the Reset_Handler of the next image in the boot sequence,
 * usually it is the secure firmware. Before passing the execution to next image
 * there is conditional rule to remove the secrets from the memory. This must be
 * done if the following conditions are satisfied:
 *  - Memory is shared between SW components at different stages of the trusted
 *    boot process.
 *  - There are secrets in the memory: KDF parameter, symmetric key,
 *    manufacturer sensitive code/data, etc.
 */
#if defined(__ICCARM__)
#pragma required = boot_clear_ram_area
#endif

__WEAK __attribute__((naked)) void boot_jump_to_next_image(uint32_t reset_handler_addr)
{
    __ASM volatile(
#if !defined(__ICCARM__)
        ".syntax unified                 \n"
#endif
        "mov     r7, r0                  \n"
        "bl      boot_clear_ram_area     \n" /* Clear RAM before jump */
        "movs    r0, #0                  \n" /* Clear registers: R0-R12, */
        "mov     r1, r0                  \n" /* except R7 */
        "mov     r2, r0                  \n"
        "mov     r3, r0                  \n"
        "mov     r4, r0                  \n"
        "mov     r5, r0                  \n"
        "mov     r6, r0                  \n"
        "mov     r8, r0                  \n"
        "mov     r9, r0                  \n"
        "mov     r10, r0                 \n"
        "mov     r11, r0                 \n"
        "mov     r12, r0                 \n"
        "mov     lr,  r0                 \n"
        "bx      r7                      \n" /* Jump to Reset_handler */
    );
}

/* bootloader platform-specific hw initialization */
__WEAK int32_t boot_platform_init(void)
{
    int32_t result;

#if defined(__ARM_ARCH_8M_MAIN__) || defined(__ARM_ARCH_8M_BASE__) \
 || defined(__ARM_ARCH_8_1M_MAIN__)
    /* Initialize stack limit register */
    uint32_t msp_stack_bottom =
            (uint32_t)&REGION_NAME(Image$$, ARM_LIB_STACK, $$ZI$$Base);

    __set_MSPLIM(msp_stack_bottom);
#endif /* defined(__ARM_ARCH_8M_MAIN__) || defined(__ARM_ARCH_8M_BASE__) \
       || defined(__ARM_ARCH_8_1M_MAIN__) */

#if defined(TFM_BL1_LOGGING) || defined(TEST_BL1_1) || defined(TEST_BL1_2)
    stdio_init();
#endif /* defined(TFM_BL1_LOGGING) || defined(TEST_BL1_1) || defined(TEST_BL1_2) */

    result = FLASH_DEV_NAME.Initialize(NULL);
    if (result != ARM_DRIVER_OK) {
        return 1;
    }

    return 0;
}

__WEAK int32_t boot_platform_post_init(void)
{
#ifdef CRYPTO_HW_ACCELERATOR
    int32_t result;

    result = crypto_hw_accelerator_init();
    if (result) {
        return 1;
    }

    (void)fih_delay_init();
#endif /* CRYPTO_HW_ACCELERATOR */

    return 0;
}

__WEAK void boot_platform_quit(struct boot_arm_vector_table *vt)
{
    /* Clang at O0, stores variables on the stack with SP relative addressing.
     * When manually set the SP then the place of reset vector is lost.
     * Static variables are stored in 'data' or 'bss' section, change of SP has
     * no effect on them.
     */
    static struct boot_arm_vector_table *vt_cpy;
    int32_t result;

#ifdef CRYPTO_HW_ACCELERATOR

    result = crypto_hw_accelerator_finish();
    if (result) {
        //while (1);
    }
#endif /* CRYPTO_HW_ACCELERATOR */

    result = FLASH_DEV_NAME.Uninitialize();
    if (result != ARM_DRIVER_OK) {
        //while (1){}
    }

#if defined(TFM_BL1_LOGGING) || defined(TEST_BL1_1) || defined(TEST_BL1_2)
    stdio_uninit();
#endif /* defined(TFM_BL1_LOGGING) || defined(TEST_BL1_1) || defined(TEST_BL1_2) */

    vt_cpy = vt;
#if defined(__ARM_ARCH_8M_MAIN__) || defined(__ARM_ARCH_8M_BASE__) \
 || defined(__ARM_ARCH_8_1M_MAIN__)
    /* Restore the Main Stack Pointer Limit register's reset value
     * before passing execution to runtime firmware to make the
     * bootloader transparent to it.
     */
    __set_MSPLIM(0);
#endif /* defined(__ARM_ARCH_8M_MAIN__) || defined(__ARM_ARCH_8M_BASE__) \
       || defined(__ARM_ARCH_8_1M_MAIN__) */

    __set_MSP(vt_cpy->msp);
    __DSB();
    __ISB();

    boot_jump_to_next_image(vt_cpy->reset);
}
