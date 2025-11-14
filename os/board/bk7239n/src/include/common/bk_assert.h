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
#include <components/log.h>
#include <components/system.h>
#include "assert.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BK_ASSERT_TAG "ASSERT"

void bk_reboot(void);

#if (defined(CONFIG_DEBUG_FIRMWARE) || defined(CONFIG_DUMP_ENABLE))
void up_assert(const uint8_t *filename, int lineno);
#define BK_ASSERT_DUMP(func, line)   up_assert(func, line)
#else
#define BK_ASSERT_DUMP(func, line)
#endif

void bk_set_dump_level(uint8_t dump_level);

/* Retrieve dump length through index */
int bk_get_crash_dump_len(int index, uint32_t* len);

/* Retrieve dump content through index */
int bk_get_crash_dump_ptr(int index, char* outBufferToRead, uint32_t buffer_size);

/* get dump count in flash */
int bk_get_crash_count(void);

/* clear crash dump in flash. */
void bk_init_crash_cnt(void);
#define bk_erase_crashlog_flash bk_init_crash_cnt

/* dump to flash storage location. !!note: this api not check parameters valid!! */
void bk_register_where_to_save(uint32_t address, uint32_t size);

/* check if exist dump in flash */
uint32_t bk_is_hardfault_msg_exist(void);

/* check dump to flash feature is enable*/
uint32_t bk_is_dump_to_flash_enable(void);

/* set dump to flash feature enable(1) or disable(0)*/
void bk_set_dump_to_flash_enable(uint32_t flag);

#ifdef CONFIG_STDIO_PRINTF
#define _OS_PRINTF printf
#else
#define _OS_PRINTF os_printf
#endif



#define BK_DUMP_OUT(format, ... )           bk_dump_printf(format, ##__VA_ARGS__)
// #define BK_DUMP_RAW_OUT(buf, len)           shell_assert_raw(true, buf, len)


#if (defined(CONFIG_SWD_DEBUG_MODE))

#define BK_ASSERT(exp) do{                                   \
	if( !(exp) ){                                            \
		BK_DUMP_OUT("(%d)Assert at: %s:%d\r\n", rtos_get_time(), __FUNCTION__, __LINE__);    \
		rtos_disable_int();                                  \
		while(1); }                                          \
}while(0)

#define BK_ASSERT_EX(exp, format, ... )do{                   \
	if( !(exp) ){                                            \
		BK_DUMP_OUT(format, ##__VA_ARGS__);                  \
		BK_DUMP_OUT("(%d)Assert at: %s:%d\r\n", rtos_get_time(), __FUNCTION__, __LINE__);    \
		rtos_disable_int();                                  \
		while(1); }                                          \
}while(0)

#else
#define BK_ASSERT(exp)              ASSERT(exp)  

#define BK_ASSERT_EX(exp, format, ... )                      \
do {                                                         \
    if ( !(exp) ) {                                          \
        rtos_disable_int();                                  \
        BK_DUMP_OUT(format, ##__VA_ARGS__);                  \
        ASSERT(exp);                                         \
        while(1);                                            \
    }                                                        \
} while (0)


#endif // #if (CONFIG_DEBUG_FIRMWARE || CONFIG_DUMP_ENABLE)

#define BK_ASSERT_HALT			BK_DUMP_OUT

#define BK_COREDUMP_DATA __attribute__((section(".coredump_data")))

#ifdef __cplusplus

}

#endif
