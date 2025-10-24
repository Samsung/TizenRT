// Copyright 2023-2028 Beken
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

#include <stdint.h>
#include "armstar.h"
#include "cache.h"
#include "mpu.h"
#include "sdkconfig.h"

void sram_dcache_map(void)
{
}

int show_cache_config_info(void)
{
	return 0;
}

__attribute__((section(".iram"))) void flush_dcache(void *va, long size)
{
    if (SCB->CLIDR & SCB_CLIDR_DC_Msk) {
        SCB_CleanInvalidateDCache_by_Addr(va, size);
    }
}

void flush_all_dcache(void)
{
    if (SCB->CLIDR & SCB_CLIDR_DC_Msk) {
        SCB_CleanInvalidateDCache();
    }
}

void enable_dcache(int enable)
{
    if (enable == 0) {
        SCB_DisableDCache();
    } else {
        SCB_EnableDCache();
    }
}

void invalidate_icache(void)
{
    SCB_InvalidateICache();
}
// eof

