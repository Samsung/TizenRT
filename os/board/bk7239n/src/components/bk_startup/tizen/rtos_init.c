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

// Notes:
// Place all FreeRTOS-dependent initialization code here

#include <common/bk_include.h>
#include "bk_private/components_init.h"
#include <common/bk_err.h>
#include "bk_arch.h"
#include "mmgmt.h"

#if defined(CONFIG_FREERTOS_RTT_MONITOR)
#include "SEGGER_SYSVIEW.h"
#endif

int rtos_memory_init(void)
{
#if (defined(CONFIG_FULLY_HOSTED) || defined(CONFIG_MEM_MGMT))
	sys_mem_init();
#endif

	return BK_OK;
}

int rtos_init(void)
{
#if defined(CONFIG_FREERTOS_RTT_MONITOR)
	SEGGER_SYSVIEW_Conf();
	SEGGER_SYSVIEW_Start();
#endif

	arch_init();
	rtos_memory_init();

	return BK_OK;
}
