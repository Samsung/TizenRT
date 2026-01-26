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

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "sdkconfig.h"
#include "soc/soc.h"

#define REBOOT_TAG_REQ         (0xAA55AA55)   //4 bytes

#if (defined(CONFIG_SOC_BK7236N)) || (defined(CONFIG_SOC_BK7239XX))
	extern char __persist_mem__, __reboot_tag__;
	#define START_TYPE_ADDR        (SOC_AON_PMU_REG_BASE + (0 << 2))

	#define REBOOT_TAG_ADDR        ((uint32_t)&__reboot_tag__)

	/*REBOOT_TAG_ADDR For CPU0-APP set reset tag in nmi wdt reboot*/
	#define PERSIST_MEMORY_ADDR    ((uint32_t)&__persist_mem__)
#elif (defined(CONFIG_SOC_BK7236XX))
	#define START_TYPE_ADDR        (SOC_AON_PMU_REG_BASE + (3 << 2))

	/*REBOOT_TAG_ADDR For CPU0-APP set reset tag in nmi wdt reboot*/
	#define REBOOT_TAG_ADDR        (0x20003FF8 + SOC_ADDR_OFFSET)
	#define REBOOT_TAG_REQ         (0xAA55AA55)   //4 bytes
	#define PERSIST_MEMORY_ADDR    (0x20003FFC + SOC_ADDR_OFFSET)
#else
	#define     START_TYPE_ADDR        (0x0080a080)
	/* 1. For bk7231n/7236, persist memory lost after power on
	 * 2. For other platform, persist memory lost after interrupt watchdog or power on
	 * */
	#define PERSIST_MEMORY_ADDR (0x0040001c)
#endif


#define     CRASH_ILLEGAL_JUMP_VALUE      0xbedead00
#define     CRASH_UNDEFINED_VALUE         0xbedead01
#define     CRASH_PREFETCH_ABORT_VALUE    0xbedead02
#define     CRASH_DATA_ABORT_VALUE        0xbedead03
#define     CRASH_UNUSED_VALUE            0xbedead04

#define     POWERON_INIT_MEM_TAG          (0xaaaaaaaa)


void show_reset_reason(void);
uint32_t reset_reason_init(void);


void set_reboot_tag(uint32_t tag);
uint32_t get_reboot_tag(void);
void set_nmi_vector(void);

#ifdef __cplusplus
}
#endif
