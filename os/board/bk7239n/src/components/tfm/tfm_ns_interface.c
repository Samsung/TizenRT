/*
 * Copyright (c) 2017-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*
 * An example to implement tfm_ns_interface_dispatch() in NS RTOS to integrate
 * TF-M interface on Armv8-M TrustZone based platforms.
 *
 * In this example, NS OS calls mutex in tfm_ns_interface_dispatch() to
 * synchronize multiple NS client calls.
 * NS OS pseudo code in this example is not based on any specific RTOS.
 *
 * Please note that this example cannot be built directly.
 */

#include <stdint.h>

/* Include NS RTOS specific mutex declarations */
#include <os/os.h>
#include "tfm_ns_interface.h"
#include "components/log.h"
#include <driver/flash.h>
#include "armstar.h"

/* Static ns lock handle - encapsulated within module */
static beken_mutex_t ns_lock_handle = NULL;
static bool ns_lock_initialized = false;

void ns_interface_delay(INT32 num)
{
	volatile INT32 i, j;

	for (i = 0; i < num; i ++) {
		for (j = 0; j < 100; j ++)
			;
	}
}

/* Initialize the ns lock */
int32_t ns_interface_lock_init(void)
{
    if (ns_lock_initialized) {
        return BK_OK;
    }

    /* NS RTOS specific mutex creation/initialization */
    rtos_init_mutex(&ns_lock_handle);
    if (ns_lock_handle) {
        ns_lock_initialized = true;
        return BK_OK;
    }

    return BK_FAIL;
}

int32_t tfm_ns_interface_lock(void)
{
    uint32_t timeout_count = 0;
    const uint32_t max_timeout = 1000000; // 1 second timeout at 1MHz
    
    if (!ns_lock_initialized) {
        BK_LOGE("TFM", "NS interface lock not initialized\n");
        return BK_ERR_NOT_INIT;
    }
    
    while (rtos_lock_mutex(&ns_lock_handle) != BK_OK) {
        timeout_count++;
        if (timeout_count >= max_timeout) {
            BK_LOGE("TFM", "Failed to acquire NS interface lock after timeout\n");
            return BK_ERR_TIMEOUT;
        }
        ns_interface_delay(1); // Small delay to prevent busy waiting
    }
    return BK_OK;
}

int32_t tfm_ns_interface_unlock(void)
{
    if (!ns_lock_initialized) {
        BK_LOGE("TFM", "NS interface lock not initialized\n");
        return BK_ERR_NOT_INIT;
    }
    
    return rtos_unlock_mutex(&ns_lock_handle);
}

int32_t tfm_ns_interface_dispatch(veneer_fn fn,
                                  uint32_t arg0, uint32_t arg1,
                                  uint32_t arg2, uint32_t arg3)
{
    int32_t result;
    int32_t status = bk_flash_mutex_lock();

    /* TF-M request protected by NS lock. */
    int32_t lock_result = tfm_ns_interface_lock();
    if (lock_result != BK_OK) {
        bk_flash_mutex_unlock(status);
        return lock_result;
    }
    
    result = fn(arg0, arg1, arg2, arg3);
    SCB_CleanInvalidateDCache();

    /*
     * Whether to check/handle lock release return code depends on NS RTOS
     * specific implementation and usage scenario.
     */
    tfm_ns_interface_unlock();

    bk_flash_mutex_unlock(status);

    return result;
}
