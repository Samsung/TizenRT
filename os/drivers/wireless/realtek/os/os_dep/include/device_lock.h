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


#ifndef _DEVICE_LOCK_H_
#define _DEVICE_LOCK_H_

enum _RT_DEV_LOCK_E {
	RT_DEV_LOCK_EFUSE = 0,
	RT_DEV_LOCK_FLASH = 1,
	RT_DEV_LOCK_CRYPTO = 2,
	RT_DEV_LOCK_PTA = 3,
	RT_DEV_LOCK_WLAN = 4,
	RT_DEV_LOCK_MAX = 5
};
typedef uint32_t RT_DEV_LOCK_E;

void device_mutex_lock(RT_DEV_LOCK_E device);
void device_mutex_unlock(RT_DEV_LOCK_E device);

#endif //_DEVICE_LOCK_H_
