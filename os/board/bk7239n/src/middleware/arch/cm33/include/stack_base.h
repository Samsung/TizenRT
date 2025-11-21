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

typedef struct {
    union {
        struct {
            long r0;
            long r1;
            long r2;
            long r3;
            long r4;
            long r5;
            long r6;
            long r7;
            long r8;
            long r9;
            long r10;
            long r11;
            long r12;
            long sp;
            long lr;
            long pc;
            long xpsr;
            long msp;
            long psp;
            long primask;
            long basepri;
            long faultmask;
            long control;
            long fpscr;
        };
        long cm33_regs[24];
    };
} SAVED_CONTEXT;

typedef enum {
    ECAUSE_ASSERT = 0,
    ECAUSE_NMI,
    ECAUSE_HARD_FAULT,
    ECAUSE_UNKNOWN = 99,
} EXCEPTION_CAUSE;

typedef void (*hook_func)(void);
typedef void (*dump_writer)(const char *format, ...);

void arch_dump_exception_stack(void);
void stack_mem_dump(uint32_t stack_top, uint32_t stack_bottom);
void stack_mem_dump_internel(dump_writer func, uint32_t stack_top, uint32_t stack_bottom);

void rtos_regist_wifi_dump_hook(hook_func wifi_func);
void rtos_regist_ble_dump_hook(hook_func ble_func);
void rtos_regist_plat_dump_hook(uint32_t reg_base_addr, uint32_t reg_size);
void rtos_dump_plat_sys_mems(void);
