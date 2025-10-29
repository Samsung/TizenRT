/*
 * Copyright (c) 2020-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "fih.h"
#include "tfm_hal_defs.h"
#include "tfm_hal_platform.h"

#ifdef TFM_FIH_PROFILE_ON
fih_int FIH_SUCCESS = FIH_INT_INIT(FIH_POSITIVE_VALUE);
fih_int FIH_FAILURE = FIH_INT_INIT(FIH_NEGATIVE_VALUE);
#endif

#ifdef FIH_ENABLE_CFI
fih_int _fih_cfi_ctr = FIH_INT_INIT(0);

fih_int fih_cfi_get_and_increment(uint8_t cnt)
{
    fih_int saved_ctr = _fih_cfi_ctr;

    if (fih_int_decode(_fih_cfi_ctr) < 0) {
        FIH_PANIC;
    }

    /* Overflow */
    if (fih_int_decode(_fih_cfi_ctr) > (fih_int_decode(_fih_cfi_ctr) + cnt)) {
        FIH_PANIC;
    }

    _fih_cfi_ctr = fih_int_encode(fih_int_decode(_fih_cfi_ctr) + cnt);

    fih_int_validate(_fih_cfi_ctr);
    fih_int_validate(saved_ctr);

    return saved_ctr;
}

void fih_cfi_validate(fih_int saved)
{
    volatile int32_t rc = FIH_FALSE;

    rc = fih_eq(saved, _fih_cfi_ctr);
    if (rc != FIH_TRUE) {
        FIH_PANIC;
    }
}

void fih_cfi_decrement(void)
{
    if (fih_int_decode(_fih_cfi_ctr) < 1) {
        FIH_PANIC;
    }

    _fih_cfi_ctr = fih_int_encode(fih_int_decode(_fih_cfi_ctr) - 1);

    fih_int_validate(_fih_cfi_ctr);
}
#endif /* FIH_ENABLE_CFI */

#ifdef FIH_ENABLE_GLOBAL_FAIL
/* Global failure loop for bootloader code. Uses attribute used to prevent
 * compiler removing due to non-standard calling procedure. Multiple loop jumps
 * used to make unlooping difficult.
 */
__attribute__((used))
__attribute__((noinline))
void fih_panic_loop(void)
{
    FIH_LABEL("FAILURE_LOOP");
    __asm volatile ("b fih_panic_loop");
    __asm volatile ("b fih_panic_loop");
    __asm volatile ("b fih_panic_loop");
    __asm volatile ("b fih_panic_loop");
    __asm volatile ("b fih_panic_loop");
    __asm volatile ("b fih_panic_loop");
    __asm volatile ("b fih_panic_loop");
    __asm volatile ("b fih_panic_loop");
    __asm volatile ("b fih_panic_loop");
}
#endif /* FIH_ENABLE_GLOBAL_FAIL */

#ifdef FIH_ENABLE_DELAY
void fih_delay_init(void)
{
    volatile int32_t ret = TFM_HAL_ERROR_GENERIC;

    ret = tfm_hal_random_init();
    if (ret != TFM_HAL_SUCCESS) {
        FIH_PANIC;
    }
}

uint8_t fih_delay_random(void)
{
    volatile int32_t ret = TFM_HAL_ERROR_GENERIC;
    uint8_t rand_value = 0xFF;

    ret = tfm_hal_random_generate(&rand_value, sizeof(rand_value));
    if (ret != TFM_HAL_SUCCESS) {
        FIH_PANIC;
    }

    return rand_value;
}
#endif /* FIH_ENABLE_DELAY */
