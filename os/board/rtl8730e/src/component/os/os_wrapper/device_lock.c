/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "os_wrapper.h"
#include "device_lock.h"

//------------------------------------------------------
#define DEVICE_MUTEX_IS_INIT(device)	(mutex_init & (1<<device))
#define DEVICE_MUTEX_SET_INIT(device)	(mutex_init |= (1<<device))
#define DEVICE_MUTEX_CLR_INIT(device)	(mutex_init &= (~(1<<device)))

static u32 mutex_init = 0;
static rtos_mutex_t device_mutex[RT_DEV_LOCK_MAX];

//======================================================
static void device_mutex_init(RT_DEV_LOCK_E device)
{
	if (!DEVICE_MUTEX_IS_INIT(device)) {
#ifdef CONFIG_PLATFORM_TIZENRT_OS
		irqstate_t flags = enter_critical_section();
#else
		_lock lock;
		_irqL irqL;
		rtw_enter_critical(&lock, &irqL);
#endif
		if (!DEVICE_MUTEX_IS_INIT(device)) {
			rtos_mutex_create(&device_mutex[device]);
			DEVICE_MUTEX_SET_INIT(device);
		}
#ifdef CONFIG_PLATFORM_TIZENRT_OS
		leave_critical_section(flags);
#else
		rtw_exit_critical(&lock, &irqL);
#endif
	}
}

//======================================================
void device_mutex_free(RT_DEV_LOCK_E device)
{
	if (DEVICE_MUTEX_IS_INIT(device)) {
#ifdef CONFIG_PLATFORM_TIZENRT_OS
		irqstate_t flags = enter_critical_section();
#else
		_lock lock;
		_irqL irqL;
		rtw_enter_critical(&lock, &irqL);
#endif
		if (DEVICE_MUTEX_IS_INIT(device)) {
			rtos_mutex_delete(device_mutex[device]);
			DEVICE_MUTEX_CLR_INIT(device);
		}
#ifdef CONFIG_PLATFORM_TIZENRT_OS
		leave_critical_section(flags);
#else
		rtw_exit_critical(&lock, &irqL);
#endif
	}
}

//======================================================
void device_mutex_lock(RT_DEV_LOCK_E device)
{
	device_mutex_init(device);
	while (rtos_mutex_take(device_mutex[device], 10000) < 0) {
		printf("device lock timeout: %d\n", (int)device);
	}
}

//======================================================
void device_mutex_unlock(RT_DEV_LOCK_E device)
{
	device_mutex_init(device);
	rtos_mutex_give(device_mutex[device]);
}
