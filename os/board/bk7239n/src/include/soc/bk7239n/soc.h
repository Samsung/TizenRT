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

#include <sdkconfig.h>
#include <soc/soc_port.h>
#include "soc_cap.h"
#include "reg_base.h"
#include "soc_debug.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SOC_ICU_REG_BASE      (0xdead0add)
#define SOC_CALENDAR_REG_BASE (0xdead0add + 0x55 * 4)

#define SOC_PSRAM_DATA_ADDR_SEC     (0x60000000)
#define SOC_PSRAM_DATA_ADDR_NONSEC  (0x70000000)

#define REG_WRITE(_r, _v) ({\
		(*(volatile uint32_t *)(_r)) = (_v);\
	})

#define REG_READ(_r) ({\
		(*(volatile uint32_t *)(_r));\
	})

#define REG_GET_BIT(_r, _b)  ({\
		(*(volatile uint32_t*)(_r) & (_b));\
	})

#define REG_SET_BIT(_r, _b)  ({\
		(*(volatile uint32_t*)(_r) |= (_b));\
	})

#define REG_CLR_BIT(_r, _b)  ({\
		(*(volatile uint32_t*)(_r) &= ~(_b));\
	})

#define REG_SET_BITS(_r, _b, _m) ({\
		(*(volatile uint32_t*)(_r) = (*(volatile uint32_t*)(_r) & ~(_m)) | ((_b) & (_m)));\
	})

#define REG_GET_FIELD(_r, _f) ({\
		((REG_READ(_r) >> (_f##_S)) & (_f##_V));\
	})

#define REG_SET_FIELD(_r, _f, _v) ({\
		(REG_WRITE((_r),((REG_READ(_r) & ~((_f##_V) << (_f##_S)))|(((_v) & (_f##_V))<<(_f##_S)))));\
	})

#define REG_MCHAN_GET_FIELD(_ch, _r, _f) ({\
		((REG_READ(_r) >> (_f##_MS(_ch))) & (_f##_V));\
	})

#define REG_MCHAN_SET_FIELD(_ch, _r, _f, _v) ({\
		(REG_WRITE((_r), ((REG_READ(_r) & ~((_f##_V) << (_f##_MS(_ch))))|(((_v) & (_f##_V))<<(_f##_MS(_ch))))));\
	})

#define VALUE_GET_FIELD(_r, _f) (((_r) >> (_f##_S)) & (_f))

#define VALUE_GET_FIELD2(_r, _f) (((_r) & (_f))>> (_f##_S))

#define VALUE_SET_FIELD(_r, _f, _v) ((_r)=(((_r) & ~((_f) << (_f##_S)))|((_v)<<(_f##_S))))

#define VALUE_SET_FIELD2(_r, _f, _v) ((_r)=(((_r) & ~(_f))|((_v)<<(_f##_S))))

#define FIELD_TO_VALUE(_f, _v) (((_v)&(_f))<<_f##_S)

#define FIELD_TO_VALUE2(_f, _v) (((_v)<<_f##_S) & (_f))

#ifndef BIT
#define BIT(i) (1<<(i))
#endif

#ifndef BIT64
#define BIT64(i)                  (1LL << (i))
#endif

#if (defined(CONFIG_RELEASE_IRAM))
#define __IRAM_SEC
#else
#define __IRAM_SEC __attribute__((section(".iram")))
#define __IRAM2 __attribute__((section(".iram2")))
#endif

#if defined(CONFIG_XIP_KERNEL) && (CONFIG_XIP_KERNEL == 1)
#define __FLASH_BOOT_CODE
#else
#define __FLASH_BOOT_CODE __attribute__((section(".flash_boot_code")))
#endif

#ifdef CONFIG_BUILD_PROTECTED
#define __PSRAM_DATA __attribute__((section(".psram.data")))
#define __PSRAM_BSS __attribute__((section(".psram.bss")))
#else
#define __PSRAM_DATA 
#define __PSRAM_BSS 
#endif

#ifdef __cplusplus
}
#endif
