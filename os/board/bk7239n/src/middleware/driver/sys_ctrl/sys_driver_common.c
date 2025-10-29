// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "sys_driver.h"
#include "sys_driver_common.h"
#if (CONFIG_CPU_CNT > 1)
#include "amp_lock_api.h"
#endif

#if (CONFIG_CPU_CNT > 1)
#define SYSTEM_REG_LOCK_WAIT_TIME_MS  10
#endif

uint32_t sys_amp_res_acquire()
{
#if (CONFIG_CPU_CNT > 1) && defined(CONFIG_MAILBOX)
	uint32_t ret_val = SYS_DRV_FAILURE;
	ret_val = amp_res_acquire(AMP_RES_ID_SYS_REG, SYSTEM_REG_LOCK_WAIT_TIME_MS);
	return ret_val;
#endif
	return SYS_DRV_SUCCESS;
}

uint32_t sys_amp_res_release()
{
#if (CONFIG_CPU_CNT > 1) && defined(CONFIG_MAILBOX)
	uint32_t ret_val = SYS_DRV_FAILURE;
	ret_val = amp_res_release(AMP_RES_ID_SYS_REG);
	return ret_val;
#endif
	return SYS_DRV_SUCCESS;
}

uint32_t sys_amp_res_init()
{
#if (CONFIG_CPU_CNT > 1) && defined(CONFIG_MAILBOX)
	uint32_t ret_val = SYS_DRV_FAILURE;
	ret_val = amp_res_init(AMP_RES_ID_SYS_REG);
	return ret_val;
#endif
	return SYS_DRV_SUCCESS;
}

