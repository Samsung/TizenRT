/**************************************************************************************************
*
* Copyright 2020 Samsung Electronics All Rights Reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing,
* software distributed under the License is distributed on an
* "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
* either express or implied. See the License for the specific
* language governing permissions and limitations under the License.
*
***************************************************************************************************/

#include <stdint.h>

#include "flash_api.h"
#include "device_lock.h"

typedef struct {
	void (*flash_erase)(uint32_t);
	int (*flash_read)(uint32_t, uint32_t, uint8_t *);
	int (*flash_write)(uint32_t, uint32_t, uint8_t *);
	void (*device_lock)(uint32_t);
	void (*device_unlock)(uint32_t);
	int (*get_random_bytes)(void *, uint32_t);
} nsfunc_ops_s;

nsfunc_ops_s ns_func;

/* Helper Function */
static void ns_flash_erase(uint32_t address)
{
	flash_t flash;

	device_mutex_lock(RT_DEV_LOCK_FLASH);
	flash_erase_sector(&flash, address);
	device_mutex_unlock(RT_DEV_LOCK_FLASH);
}

static int ns_flash_read(uint32_t address, uint32_t len, uint8_t *data)
{
	int ret;
	flash_t flash;

	device_mutex_lock(RT_DEV_LOCK_FLASH);
	ret = flash_stream_read(&flash, address, len, data);
	device_mutex_unlock(RT_DEV_LOCK_FLASH);

	return ret;
}

static int ns_flash_write(uint32_t address, uint32_t len, uint8_t *data)
{
	int ret;
	flash_t flash;

	device_mutex_lock(RT_DEV_LOCK_FLASH);
	ret = flash_stream_write(&flash, address, len, data);
	device_mutex_unlock(RT_DEV_LOCK_FLASH);

	return ret;
}

extern int rtw_get_random_bytes(void *dst, u32 size);
void *rtl_set_ns_func(void)
{
	ns_func.flash_erase = ns_flash_erase;
	ns_func.flash_read = ns_flash_read;
	ns_func.flash_write = ns_flash_write;
	ns_func.device_lock = device_mutex_lock;
	ns_func.device_unlock = device_mutex_unlock;
	ns_func.get_random_bytes = rtw_get_random_bytes;

	return (void *)&ns_func;
}
