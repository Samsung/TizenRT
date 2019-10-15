/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
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
 ******************************************************************************/


#include "osdep_service.h"
#include "device_lock.h"

#define DEVICE_MUTEX_IS_INIT(device) (mutex_init & (1 << device))
#define DEVICE_MUTEX_SET_INIT(device) (mutex_init |= (1 << device))
#define DEVICE_MUTEX_CLR_INIT(device) (mutex_init &= (~(1 << device)))

static u32 mutex_init = 0;
static _mutex device_mutex[RT_DEV_LOCK_MAX];

static void device_mutex_init(RT_DEV_LOCK_E device)
{
	if (!DEVICE_MUTEX_IS_INIT(device)) {
		_lock lock;
		_irqL irqL;
		rtw_enter_critical(&lock, &irqL);
		if (!DEVICE_MUTEX_IS_INIT(device)) {
			rtw_mutex_init(&device_mutex[device]);
			DEVICE_MUTEX_SET_INIT(device);
		}
		rtw_exit_critical(&lock, &irqL);
	}
}

void device_mutex_free(RT_DEV_LOCK_E device)
{
	if (DEVICE_MUTEX_IS_INIT(device)) {
		_lock lock;
		_irqL irqL;
		rtw_enter_critical(&lock, &irqL);
		if (DEVICE_MUTEX_IS_INIT(device)) {
			rtw_mutex_free(&device_mutex[device]);
			DEVICE_MUTEX_CLR_INIT(device);
		}
		rtw_exit_critical(&lock, &irqL);
	}
}

void device_mutex_lock(RT_DEV_LOCK_E device)
{
	device_mutex_init(device);
	while (rtw_mutex_get_timeout(&device_mutex[device], 10000) < 0)
		printf("device lock timeout: %d\n", (int)device);
}

void device_mutex_unlock(RT_DEV_LOCK_E device)
{
	device_mutex_init(device);
	rtw_mutex_put(&device_mutex[device]);
}
