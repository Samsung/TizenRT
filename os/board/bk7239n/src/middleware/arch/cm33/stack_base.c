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

#include <string.h>
#include <stdint.h>
#include "boot.h"
#include "arm.h"
#include "bk_arch.h"
#include "bk_uart.h"
#include <components/system.h>
#include <os/os.h>
#include <components/log.h>
#include <common/bk_assert.h>
#include <os/mem.h>
#include <driver/wdt.h>
#include "bk_aon_wdt.h"
#include "bk_wdt.h"
#include "stack_base.h"
#if defined(CONFIG_CM_BACKTRACE)
#include "cm_backtrace.h"
#endif

#define STACK_CALLBACK_BUF_SIZE 32
#define SOC_ITCM_CODE_SIZE      (0x4000)


#if defined(CONFIG_SYS_CPU0)
extern unsigned char __iram_start__;
extern unsigned char __iram_end__;
#endif //#if CONFIG_SYS_CPU0

extern char __etext;

extern uint32_t __CPU0_APP_VIRTUAL_CODE_START;

static inline int addr_is_in_flash_txt(uint32_t addr)
{
    return ((addr > (uint32_t)(SOC_FLASH_DATA_BASE + (uint32_t)&__CPU0_APP_VIRTUAL_CODE_START)) && (addr < (uint32_t)&__etext));
}

static inline int addr_is_in_itcm_txt(uint32_t addr)
{
#if (!(defined(CONFIG_SOC_BK7236N)) && !(defined(CONFIG_SOC_BK7239XX)))
    return ((addr > (uint32_t)SOC_ITCM_DATA_BASE) && (addr < (uint32_t)(SOC_ITCM_DATA_BASE + SOC_ITCM_CODE_SIZE)));
#else
    return 0;
#endif
}

static inline int addr_is_in_iram_txt(uint32_t addr)
{
    if ((addr > (uint32_t)&__iram_start__) && (addr < (uint32_t)&__iram_end__)) {
        return 1;
    }
    return 0;
}

static int code_addr_is_valid(uint32_t addr)
{
    if (addr % 2 == 0) {
        return false;
    }

    return (addr_is_in_flash_txt(addr) || addr_is_in_itcm_txt(addr) || addr_is_in_iram_txt(addr));

    // if (addr_is_in_flash_txt(addr) || addr_is_in_itcm_txt(addr) || addr_is_in_iram_txt(addr)) {
    //     addr -= 1;
    //     #if CONFIG_CM_BACKTRACE
    //     if (!disassembly_ins_is_bl_blx(addr - sizeof(size_t))) {
    //         return false;
    //     }
    //     #endif
    //     return true;
    // }
    // return false;
}

void stack_mem_dump_internel(dump_writer func, uint32_t stack_top, uint32_t stack_bottom)
{
    unsigned char *data = NULL;
    volatile uint32_t value = 0;
    uint32_t cnt = 0;
    uint32_t sp = stack_top;
    uint32_t fp = stack_bottom;

    func(">>>>stack mem dump begin, stack_top=%08x, stack end=%08x\r\n", stack_top, stack_bottom);
    for (;  sp < fp; sp += sizeof(size_t)) {
        value = os_get_word(sp);
        data = ((unsigned char *)&value);

        if ((cnt++ & 0x7) == 0) {
            func("\r\n");
        }
#if defined(CONFIG_WATCHDOG)
        bk_wdt_feed();
#endif

        func("%02x %02x %02x %02x ", data[0], data[1], data[2], data[3]);
    }
    func("\r\n");
    func("<<<<stack mem dump end. stack_top=%08x, stack end=%08x\r\n", stack_top, stack_bottom);
    func("\r\n");
}

// static void dump_to_uart_directly(const char * format, ...)
// {
//     va_list     arg_list;
//     va_start( arg_list, format );

//     shell_assert_out_va(true, (char *)format, arg_list);

//     va_end( arg_list );
// }

void stack_mem_dump(uint32_t stack_top, uint32_t stack_bottom)
{
    if (stack_bottom > stack_top) {
        stack_mem_dump_internel(bk_dump_printf, stack_top, stack_bottom);
    } 
}

/* The stack is grow from bottom to top
 *
 *   |   | <- minimum_addr = (bottom - stack_size)
 *   |   |
 *   |   |
 *   |   | <- top
 *   | . |
 *   | . |
 *   | . | <- bottom, (big address)
 *
 * */
void arch_parse_stack_backtrace(const char *str_type, uint32_t stack_top, uint32_t stack_bottom,
                           uint32_t stack_size, bool thumb_mode)
{
    uint32_t call_stack_buf[STACK_CALLBACK_BUF_SIZE] = {0};
    uint32_t stack_minimum = stack_bottom - stack_size;
    uint32_t pc;
    int call_stack_index = 0;
    uint32_t init_stack_top = stack_top;

    for (; stack_top < stack_bottom && (call_stack_index < STACK_CALLBACK_BUF_SIZE); stack_top += sizeof(size_t)) {
        pc = *((uint32_t *) stack_top);

        if (code_addr_is_valid(pc)) {
            if (pc & 1)
                pc = pc - 1;

            call_stack_buf[call_stack_index] = pc;
            call_stack_index++;
        }
    }

    if (call_stack_index > 0) {
        int index;

        BK_DUMP_OUT("%-16s   [0x%-6x ~ 0x%-6x]   0x%-6x   %-4d   %-8d",
                  str_type, stack_minimum, stack_bottom, init_stack_top, stack_size, init_stack_top < stack_minimum);

        for (index = 0; index < call_stack_index; index++)
            BK_DUMP_OUT("%lx ", call_stack_buf[index]);
        BK_DUMP_OUT("\r\n");
    } else if (init_stack_top < stack_minimum) {
        BK_DUMP_OUT("%-16s   [0x%-6x ~ 0x%-6x]   0x%-6x   %-4d   %-8d   ",
                  str_type, stack_minimum, stack_bottom, init_stack_top, stack_size, init_stack_top < stack_minimum);
    }
}


