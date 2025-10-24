/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <inttypes.h>
#include "fih.h"
#include "utilities.h"
#include "tfm_hal_platform.h"

void tfm_core_panic(void)
{
#if CONFIG_PANIC_DEAD_LOOP
	volatile uint32_t panic_loop = 1;
	printf("core panic\r\n");
	while(panic_loop);
#else
    fih_delay();

#ifdef CONFIG_TFM_HALT_ON_CORE_PANIC

    /*
     * Halt instead of reboot to retain the backtrace that triggered
     * the fault and thereby make it easier to debug.
     */
    tfm_hal_system_halt();

#ifdef TFM_FIH_PROFILE_ON
    fih_delay();

    tfm_hal_system_halt();
#endif

#else /* CONFIG_TFM_HALT_ON_CORE_PANIC */
    /*
     * FixMe: In the first stage, the SPM will restart the entire system when a
     * programmer error is detected in either the SPE or NSPE.
     * In the next stage, the specified error codes are also sent to any NSPE
     * management firmware. The NSPE management firmware can then decide to pass
     * those error codes back to the calling task or to use its own
     * functionality for terminating an execution context.
     */
    tfm_hal_system_reset();

#ifdef TFM_FIH_PROFILE_ON
    fih_delay();

    tfm_hal_system_reset();
#endif

#endif /* CONFIG_TFM_HALT_ON_CORE_PANIC */
#endif
}
