// Copyright 2022-2025 Beken
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
#include "sdkconfig.h"

/* Important notice:
 *
 * Following macros are not public API, they are just for writing FPGA/Chip 
 * debug code only.
 * */

#define GPIO_UP(id) *(volatile uint32_t*) (SOC_AON_GPIO_REG_BASE + ((id) << 2)) = 0x2000002
#define GPIO_DOWN(id) *(volatile uint32_t*) (SOC_AON_GPIO_REG_BASE + ((id) << 2)) = 0x2000000

#define GPIO_UP_DOWN(id) do {\
	volatile uint32_t cnt;\
	for (int i = 0; i < 10; i++) {\
		if ((i % 2) == 0) GPIO_UP((id));\
		else GPIO_DOWN(id);\
		cnt = 1000;\
		while(--cnt > 0);\
	}\
} while(0)

#ifdef  CONFIG_STARTUP_PERFORMANCE
#define STARTUP_PERF(gpio_id) GPIO_UP(gpio_id)
#else
#define STARTUP_PERF(gpio_id) 
#endif

#define REG_SET(r, l, h, v) do { \
	uint32_t reg_v = *(volatile uint32_t*)(r);\
	reg_v &= ~((( 1 << ((h) + 1)) - 1) & (~( (1 << (l)) - 1)));\
	reg_v |= ((v) << (l));\
	*(volatile uint32_t*)(r) = reg_v;\
} while(0)

#define REG_OR(r, l, h, v) do { \
	uint32_t reg_v = *(volatile uint32_t*)(r);\
	reg_v |= ((v) << (l));\
	*(volatile uint32_t*)(r) = reg_v;\
} while(0)

#define SYS_REG_SET(reg_id, l, h, v) REG_SET((SOC_SYSTEM_REG_BASE + ((reg_id) << 2)), (l), (h), (v))
#define SYS_REG_OR(reg_id, l, h, v)  REG_OR((SOC_SYSTEM_REG_BASE + ((reg_id) << 2)), (l), (h), (v))


