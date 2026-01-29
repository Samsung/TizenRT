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

#include <stdio.h>
#include <stdint.h>
#include "boot.h"
#include "sdkconfig.h"
#include "reset_reason.h"
#include <os/os.h>
#include "bk_arch.h"
#include "bk_rtos_debug.h"
#include <components/system.h>
#include <os/mem.h>
#include <components/log.h>
#include <common/bk_assert.h>
#include "arch_interrupt.h"
#include "stack_base.h"
#include <driver/wdt.h>
#include "wdt_driver.h"
#include <driver/wdt.h>
#include "bk_aon_wdt.h"
#include "armstar.h"

#include <driver/flash_types.h>
#include <driver/flash.h>


#define MAX_DUMP_SYS_MEM_COUNT       (8)
#define SOC_DTCM_DATA_SIZE           (0x4000)
#define SOC_ITCM_DATA_SIZE           (0x4000)
#define SOC_SRAM_TOTAL_SIZE          0x80000
#define SOC_SRAM_DATA_END            (SOC_SRAM0_DATA_BASE + SOC_SRAM_TOTAL_SIZE)

extern uint32_t RAM_KREGION0_START;
extern uint32_t RAM_KREGION0_SIZE;
extern uint32_t RAM_KREGION2_START;
extern uint32_t RAM_KREGION2_SIZE;

extern uint32_t __psram_data_start__;
extern uint32_t __psram_bss_end__;
extern uint32_t __psram_code_start__;
extern uint32_t __psram_code_end__;


#define KERNEL_HEAP_START_ADDR   (void*)&RAM_KREGION0_START   
#define KERNEL_HEAP_SIZE         (void*)&RAM_KREGION0_SIZE 
#define APP_HEAP_START_ADDR   (void*)&RAM_KREGION2_START   
#define APP_HEAP_SIZE         (void*)&RAM_KREGION2_SIZE 

// typedef struct sys_mem_info
// {
//     uint32_t mem_base_addr;
//     uint32_t mem_size;
// } sys_mem_info_t;

// static unsigned int s_mem_count = 0;
// static sys_mem_info_t s_dump_sys_mem_info[MAX_DUMP_SYS_MEM_COUNT] = {0};

static hook_func s_wifi_dump_func = NULL;
static hook_func s_ble_dump_func = NULL;

volatile unsigned int g_enter_exception = 0;



static void rtos_dump_plat_memory(void) {
#if (!(defined(CONFIG_SOC_BK7236N)) && !(defined(CONFIG_SOC_BK7239XX)))
    // Dump DTCM
    stack_mem_dump((uint32_t)SOC_DTCM_DATA_BASE, (uint32_t)(SOC_DTCM_DATA_BASE + SOC_DTCM_DATA_SIZE));

    // Dump ITCM
    stack_mem_dump((uint32_t)(SOC_ITCM_DATA_BASE + 0x20) , (uint32_t)(SOC_ITCM_DATA_BASE + SOC_ITCM_DATA_SIZE));
#endif

    // Dump All SRAM
    stack_mem_dump((uint32_t)SOC_SRAM0_DATA_BASE, (uint32_t)SOC_SRAM1_DATA_BASE);
    stack_mem_dump((uint32_t)SOC_SRAM1_DATA_BASE, (uint32_t)SOC_SRAM2_DATA_BASE);
    stack_mem_dump((uint32_t)SOC_SRAM2_DATA_BASE, (uint32_t)SOC_SRAM3_DATA_BASE);
    stack_mem_dump((uint32_t)SOC_SRAM3_DATA_BASE, (uint32_t)SOC_SRAM4_DATA_BASE);
#if (CONFIG_SPE == 0)
    stack_mem_dump((uint32_t)SOC_SRAM4_DATA_BASE, (uint32_t)SOC_SRAM_DATA_END - CONFIG_CPU0_SPE_RAM_SIZE);
#else
    stack_mem_dump((uint32_t)SOC_SRAM4_DATA_BASE, (uint32_t)SOC_SRAM_DATA_END);
#endif
    
    stack_mem_dump((uint32_t)KERNEL_HEAP_START_ADDR, (uint32_t)KERNEL_HEAP_START_ADDR + 0xC8000);  //kernel 800k
#ifdef CONFIG_BUILD_PROTECTED
#if !defined(CONFIG_XIP_KERNEL)
    stack_mem_dump((uint32_t)&__psram_code_start__, (uint32_t)&__psram_code_end__);   //psram code
#endif
    stack_mem_dump((uint32_t)&__psram_data_start__, (uint32_t)&__psram_bss_end__);   //psram data and bss
    stack_mem_dump((uint32_t)APP_HEAP_START_ADDR, (uint32_t)APP_HEAP_START_ADDR + 0x100000); // app 1M
#endif
}

//NOTES:temperary codes for BK7236XX debug, we'd better optimize it for other SOC/projects...
static void dump_peri_regs(void) {
#if defined(CONFIG_SOC_BK7236XX)
    stack_mem_dump((uint32_t)SOC_SYS_REG_BASE, (uint32_t)SOC_SYS_REG_BASE + (0x3a*4));
    stack_mem_dump((uint32_t)SOC_AON_PMU_REG_BASE, (uint32_t)SOC_AON_PMU_REG_BASE + (0x7f*4));

#if defined(CONFIG_GENERAL_DMA)
    stack_mem_dump((uint32_t)SOC_GENER_DMA_REG_BASE, (uint32_t)SOC_GENER_DMA_REG_BASE + (0x44*4));
    stack_mem_dump((uint32_t)SOC_GENER_DMA1_REG_BASE, (uint32_t)SOC_GENER_DMA1_REG_BASE + (0x44*4));
#endif
#if defined(CONFIG_MAILBOX)
    stack_mem_dump((uint32_t)SOC_MBOX0_REG_BASE, (uint32_t)SOC_MBOX0_REG_BASE + (0x38*4));
    stack_mem_dump((uint32_t)SOC_MBOX1_REG_BASE, (uint32_t)SOC_MBOX1_REG_BASE + (0x38*4));
#endif
#if defined(CONFIG_AON_RTC)
    stack_mem_dump((uint32_t)SOC_AON_RTC_REG_BASE, (uint32_t)SOC_AON_RTC_REG_BASE + (0x0a*4));
#endif
#if defined(CONFIG_PSRAM)
    stack_mem_dump((uint32_t)SOC_PSRAM_REG_BASE, (uint32_t)SOC_PSRAM_REG_BASE + (0x17*4));
#endif
#endif

}

unsigned int arch_is_enter_exception(void) {
    return g_enter_exception;
}

void arch_set_enter_exception(void) {
    g_enter_exception = 1;
}

void rtos_regist_wifi_dump_hook(hook_func wifi_func)
{
    s_wifi_dump_func = wifi_func;
}

void rtos_regist_ble_dump_hook(hook_func ble_func)
{
    s_ble_dump_func = ble_func;
}

// void rtos_regist_plat_dump_hook(uint32_t mem_base_addr, uint32_t mem_size)
// {
//     if (mem_base_addr >= SOC_SRAM0_DATA_BASE
//         && (mem_base_addr + mem_size) < SOC_SRAM_DATA_END) {
//         return;
//     }

//     for (int i = 0; i < s_mem_count; i++) {
//         if(mem_base_addr == s_dump_sys_mem_info[i].mem_base_addr
//          && mem_size == s_dump_sys_mem_info[i].mem_size) {
//             return;
//          }
//     }

//     if (s_mem_count < MAX_DUMP_SYS_MEM_COUNT) {
//         s_dump_sys_mem_info[s_mem_count].mem_base_addr = mem_base_addr;
//         s_dump_sys_mem_info[s_mem_count].mem_size = mem_size;
//         s_mem_count++;
//     } else {
//         //BK_DUMP_OUT("rtos_regist_plat_dump_hook failed:s_mem_count(%d).\r\n", s_mem_count);
//     }
// }

void rtos_dump_plat_sys_mems(void) {
    dump_peri_regs();    //move before PSRAM, avoid psram not power-on cause it can't dump out

    // for (int i = 0; i < s_mem_count; i++) {
    //     uint32_t begin = s_dump_sys_mem_info[i].mem_base_addr;
    //     uint32_t end = begin + s_dump_sys_mem_info[i].mem_size;
    //     stack_mem_dump(begin, end);
    // }
    rtos_dump_plat_memory();
}


#define CPU_ID     0

static const char * const fault_type[] =
{
    [0]  = NULL,
    [1]  = NULL,
    [2]  = "Watchdog\r\n",
    [3]  = "HardFault\r\n",
    [4]  = "MemFault\r\n",
    [5]  = "BusFault\r\n",
    [6]  = "UsageFault\r\n",
    [7]  = "SecureFault\r\n",
    [8]  = NULL,
    [9]  = NULL,
    [10] = NULL,
    [11] = "SVC\r\n",
    [12] = "DebugFault\r\n",
    [13] = NULL,
    [14] = "PendSV\r\n",
    [15] = "SysTick\r\n",
};

#if (defined(CONFIG_SHELL_ASYNCLOG))
extern void shell_set_log_cpu(u8 req_cpu);
#endif

static void dump_prologue(void)
{
#if defined(CONFIG_FREERTOS_SMP)

    #if (defined(CONFIG_SHELL_ASYNCLOG))
    shell_set_log_cpu(CPU_ID);
    #endif

#else

#if (defined(CONFIG_SYS_CPU0))
    #if (defined(CONFIG_SHELL_ASYNCLOG))
    shell_set_log_cpu(CONFIG_CPU_CNT);
    shell_set_log_cpu(CPU_ID);
    #endif
#elif (defined(CONFIG_SYS_CPU1))
    ipc_send_trap_handle_begin();
#endif

#endif
}

static void dump_epilogue(void)
{

    #if (defined(CONFIG_SHELL_ASYNCLOG))
    shell_set_log_cpu(CONFIG_CPU_CNT);
    #endif

}

static uint32_t bk_dump_primask_save(void)
{
    uint32_t primask;

	/* Return the current value of primask register and set
	 * bit 0 of the primask register to disable interrupts
	 */
	__asm__ __volatile__
	(
		"\tmrs    %0, primask\n"
		"\tcpsid  i\n"
		: "=r"(primask)
		:
		: "memory"
	);
    return primask;
}

static void bk_dump_primask_restore(uint32_t flags)
{
    /* If bit 0 of the primask is 0, then we need to restore
	 * interrupts.
	 */

	__asm__ __volatile__
	(
		"\ttst    %0, #1\n"
		"\tbne.n  1f\n"
		"\tcpsie  i\n"
		"1:\n"
		:
		: "r"(flags)
		: "memory"
	);
}


/**
 * this function will show registers of CPU
 *
 * @param mcause
 * @param context
 */
extern volatile uint32_t *current_regs;
static void arch_dump_cpu_registers(uint32_t mcause, SAVED_CONTEXT *context, uint32_t is_irq_assert) 
{
    BK_DUMP_OUT("CPU%d Current regs:\r\n", CPU_ID);

    if (current_regs && !is_irq_assert) {
       // context of task.
        BK_DUMP_OUT("0 r0 x 0x%lx\r\n", current_regs[REG_R0]);
        BK_DUMP_OUT("1 r1 x 0x%lx\r\n", current_regs[REG_R1]);
        BK_DUMP_OUT("2 r2 x 0x%lx\r\n", current_regs[REG_R2]);
        BK_DUMP_OUT("3 r3 x 0x%lx\r\n", current_regs[REG_R3]);
        BK_DUMP_OUT("4 r4 x 0x%lx\r\n", current_regs[REG_R4]);
        BK_DUMP_OUT("5 r5 x 0x%lx\r\n", current_regs[REG_R5]);
        BK_DUMP_OUT("6 r6 x 0x%lx\r\n", current_regs[REG_R6]);
        BK_DUMP_OUT("7 r7 x 0x%lx\r\n", current_regs[REG_R7]);
        BK_DUMP_OUT("8 r8 x 0x%lx\r\n", current_regs[REG_R8]);
        BK_DUMP_OUT("9 r9 x 0x%lx\r\n", current_regs[REG_R9]);
        BK_DUMP_OUT("10 r10 x 0x%lx\r\n", current_regs[REG_R10]);
        BK_DUMP_OUT("11 r11 x 0x%lx\r\n", current_regs[REG_R11]);
        BK_DUMP_OUT("12 r12 x 0x%lx\r\n", current_regs[REG_R12]);
        BK_DUMP_OUT("14 sp x 0x%lx\r\n", current_regs[REG_R13]);
        BK_DUMP_OUT("15 lr x 0x%lx\r\n", current_regs[REG_R14]);
        BK_DUMP_OUT("16 pc x 0x%lx\r\n", current_regs[REG_R15]);
        BK_DUMP_OUT("17 xpsr x 0x%lx\r\n", current_regs[REG_XPSR]);
        BK_DUMP_OUT("18 msp x 0x%lx\r\n", __get_MSP());
        BK_DUMP_OUT("19 psp x 0x%lx\r\n", __get_PSP());
        BK_DUMP_OUT("20 primask x 0x%lx\r\n", __get_PRIMASK());
        BK_DUMP_OUT("21 basepri x 0x%lx\r\n", current_regs[REG_BASEPRI]);
        // BK_DUMP_OUT("22 faultmask x 0x%lx\r\n", __get_FAULTMASK());
        BK_DUMP_OUT("23 fpscr x 0x%lx\r\n", __get_FPSCR());

        // context of ISR.
        mcause = __get_xPSR();
        BK_DUMP_OUT("30 CPU%d xPSR x 0x%lx\r\n", CPU_ID, mcause);
        BK_DUMP_OUT("31 LR x 0x%lx\r\n", current_regs[REG_R14]);       // exception LR.
        BK_DUMP_OUT("32 control x 0x%lx\r\n", __get_CONTROL());

        BK_DUMP_OUT("40 MMFAR x 0x%lx\r\n", SCB->MMFAR);
        BK_DUMP_OUT("41 BFAR x 0x%lx\r\n", SCB->BFAR);
        BK_DUMP_OUT("42 CFSR x 0x%lx\r\n", SCB->CFSR);
        BK_DUMP_OUT("43 HFSR x 0x%lx\r\n", SCB->HFSR);

        mcause = mcause & 0x1FF;

        if ( (mcause <= 0x0F) && (fault_type[mcause] != NULL) )
        {
            BK_DUMP_OUT((char *)fault_type[mcause]);
        }
    } else {
        // context of task.
        BK_DUMP_OUT("0 r0 x 0x%lx\r\n", context->r0);
        BK_DUMP_OUT("1 r1 x 0x%lx\r\n", context->r1);
        BK_DUMP_OUT("2 r2 x 0x%lx\r\n", context->r2);
        BK_DUMP_OUT("3 r3 x 0x%lx\r\n", context->r3);
        BK_DUMP_OUT("4 r4 x 0x%lx\r\n", context->r4);
        BK_DUMP_OUT("5 r5 x 0x%lx\r\n", context->r5);
        BK_DUMP_OUT("6 r6 x 0x%lx\r\n", context->r6);
        BK_DUMP_OUT("7 r7 x 0x%lx\r\n", context->r7);
        BK_DUMP_OUT("8 r8 x 0x%lx\r\n", context->r8);
        BK_DUMP_OUT("9 r9 x 0x%lx\r\n", context->r9);
        BK_DUMP_OUT("10 r10 x 0x%lx\r\n", context->r10);
        BK_DUMP_OUT("11 r11 x 0x%lx\r\n", context->r11);
        BK_DUMP_OUT("12 r12 x 0x%lx\r\n", context->r12);
        BK_DUMP_OUT("14 sp x 0x%lx\r\n", context->sp);
        BK_DUMP_OUT("15 lr x 0x%lx\r\n", context->lr);
        BK_DUMP_OUT("16 pc x 0x%lx\r\n", context->pc);
        BK_DUMP_OUT("17 xpsr x 0x%lx\r\n", context->xpsr);
        BK_DUMP_OUT("18 msp x 0x%lx\r\n", context->msp);
        BK_DUMP_OUT("19 psp x 0x%lx\r\n", context->psp);
        BK_DUMP_OUT("20 primask x 0x%lx\r\n", context->primask);
        BK_DUMP_OUT("21 basepri x 0x%lx\r\n", context->basepri);
        BK_DUMP_OUT("22 faultmask x 0x%lx\r\n", context->faultmask);
        BK_DUMP_OUT("23 fpscr x 0x%lx\r\n", context->fpscr);

        // context of ISR.
        mcause = __get_xPSR();
        BK_DUMP_OUT("30 CPU%d xPSR x 0x%lx\r\n", CPU_ID, mcause);
        BK_DUMP_OUT("31 LR x 0x%lx\r\n", context->control);       // exception LR.
        BK_DUMP_OUT("32 control x 0x%lx\r\n", __get_CONTROL());

        BK_DUMP_OUT("40 MMFAR x 0x%lx\r\n", SCB->MMFAR);
        BK_DUMP_OUT("41 BFAR x 0x%lx\r\n", SCB->BFAR);
        BK_DUMP_OUT("42 CFSR x 0x%lx\r\n", SCB->CFSR);
        BK_DUMP_OUT("43 HFSR x 0x%lx\r\n", SCB->HFSR);

        mcause = mcause & 0x1FF;

        if ( (mcause <= 0x0F) && (fault_type[mcause] != NULL) )
        {
            BK_DUMP_OUT((char *)fault_type[mcause]);
        }
    }
}


static void dump_context(uint32_t lr, uint32_t msp)
{
    SAVED_CONTEXT regs;

    regs.r4 = ((uint32_t *)msp)[-8];
    regs.r5 = ((uint32_t *)msp)[-7];
    regs.r6 = ((uint32_t *)msp)[-6];
    regs.r7 = ((uint32_t *)msp)[-5];
    regs.r8 = ((uint32_t *)msp)[-4];
    regs.r9 = ((uint32_t *)msp)[-3];
    regs.r10 = ((uint32_t *)msp)[-2];
    regs.r11 = ((uint32_t *)msp)[-1];
    regs.control = lr;       // exception LR.

    regs.msp = __get_MSP();
    regs.psp = __get_PSP();

    regs.primask = __get_PRIMASK();
    regs.basepri = __get_BASEPRI();
    regs.faultmask = __get_FAULTMASK();

    regs.fpscr = __get_FPSCR();

    uint32_t stack_pointer = msp;
    uint32_t stack_adj = 8 * sizeof(uint32_t);

    if(lr & (1UL << 2))  //[get sp] if the bit[2] val is 1, reps psp, otherwise  is msp;
    {
        stack_pointer = __get_PSP();
    }

    if((lr & (1UL << 4)) == 0)   //if the bit[4] val is 1, reps the FPU is forbidden,otherwise is using the FPU;
    {
        stack_adj += 18 * sizeof(uint32_t);  // 18 FPU regs.
    }

    regs.r0 = ((uint32_t *)stack_pointer)[0];
    regs.r1 = ((uint32_t *)stack_pointer)[1];
    regs.r2 = ((uint32_t *)stack_pointer)[2];
    regs.r3 = ((uint32_t *)stack_pointer)[3];
    regs.r12 = ((uint32_t *)stack_pointer)[4];
    regs.lr = ((uint32_t *)stack_pointer)[5];
    regs.pc = ((uint32_t *)stack_pointer)[6];
    regs.xpsr = ((uint32_t *)stack_pointer)[7];

    if(regs.xpsr & (1UL << 9))  //if val is 1.need align two word .when using FPU.
    {
        stack_adj += 1 * sizeof(uint32_t);
    }
    regs.sp = stack_pointer + stack_adj;

    rtos_disable_int();
    bk_wdt_feed();
#if (defined(CONFIG_INT_AON_WDT))
    bk_int_aon_wdt_feed();
#endif
    bk_set_printf_sync(true);
    dump_prologue();
    arch_dump_cpu_registers(0, &regs, 0);
}

static void rtos_dump_system(uint32_t msp, uint32_t psp)
{

    // BK_LOG_FLUSH();
    // bk_set_printf_sync(true);

    BK_DUMP_OUT("***********************************************************************************************\r\n");
    BK_DUMP_OUT("***********************************user except handler begin***********************************\r\n");
    BK_DUMP_OUT("***********************************************************************************************\r\n");
    
#if defined(CONFIG_WATCHDOG)
    bk_wdt_feed();
#if (defined(CONFIG_INT_AON_WDT))
    bk_int_aon_wdt_feed();
#endif
#endif

    if(NULL != s_wifi_dump_func) {
        s_wifi_dump_func();
    }

    if(NULL != s_ble_dump_func) {
        s_ble_dump_func();
    }
 
    rtos_dump_plat_sys_mems();
    
    BK_DUMP_OUT("***********************************************************************************************\r\n");
    BK_DUMP_OUT("************************************user except handler end************************************\r\n");
    BK_DUMP_OUT("***********************************************************************************************\r\n");
    

}

#define CHECK_TASK_WDT_INTERRUPT (0x13)

uint32_t     g_wdt_handler_lr;

static void user_except_handler(uint32_t reset_reason, SAVED_CONTEXT *regs)
{
    if (0 == g_enter_exception) {
        // Make sure the interrupt is disable
        // uint32_t int_level = rtos_disable_int();

        /* Handled Trap */
        g_enter_exception = 1;

        // if it is a task WDT assert!
        if((regs->xpsr & 0x1FF) == CHECK_TASK_WDT_INTERRUPT)  // it can be used for any interrupts if LR is saved at entrance of ISR.
        {
            if(g_wdt_handler_lr & (1UL << 2))
            {
                uint32_t stack_pointer = __get_PSP();
                uint32_t stack_adj = 8 * sizeof(uint32_t);

                if((g_wdt_handler_lr & (1UL << 4)) == 0)
                {
                    stack_adj += 18 * sizeof(uint32_t);  // 18 FPU regs.
                }

                regs->r0 = ((uint32_t *)stack_pointer)[0];
                regs->r1 = ((uint32_t *)stack_pointer)[1];
                regs->r2 = ((uint32_t *)stack_pointer)[2];
                regs->r3 = ((uint32_t *)stack_pointer)[3];
                regs->r12 = ((uint32_t *)stack_pointer)[4];
                regs->lr = ((uint32_t *)stack_pointer)[5];
                regs->pc = ((uint32_t *)stack_pointer)[6];
                regs->xpsr = ((uint32_t *)stack_pointer)[7];

                if(regs->xpsr & (1UL << 9))
                {
                    stack_adj += 1 * sizeof(uint32_t);
                }
                regs->sp = stack_pointer + stack_adj;
            }
        }

        bk_set_printf_sync(true);
        // dump_prologue();
        arch_dump_cpu_registers(ECAUSE_ASSERT, regs,0);

        rtos_dump_system(0, regs->sp);
        // dump_epilogue();

        bk_reboot_ex(reset_reason);

        while(g_enter_exception);

        // rtos_enable_int(int_level);
    } else {
        dump_epilogue();

        bk_wdt_force_reboot();
    }

}

///1. Save to stack is better
///2. Some regs already saved in stack
static void store_cpu_regs(uint32_t mcause, SAVED_CONTEXT *regs) {
    regs->r0 = __get_R0();
    regs->r1 = __get_R1();
    regs->r2 = __get_R2();
    regs->r3 = __get_R3();
    regs->r4 = __get_R4();
    regs->r5 = __get_R5();
    regs->r6 = __get_R6();
    regs->r7 = __get_R7();
    regs->r8 = __get_R8();
    regs->r9 = __get_R9();
    regs->r10 = __get_R10();
    regs->r11 = __get_R11();
    regs->r12 = __get_R12();
    regs->sp = __get_SP();
    regs->lr = __get_LR();
    regs->pc = __get_PC();

    regs->xpsr = __get_xPSR();
    regs->msp = __get_MSP();
    regs->psp = __get_PSP();

    regs->primask = __get_PRIMASK();
    regs->basepri = __get_BASEPRI();
    regs->faultmask = __get_FAULTMASK();

    regs->control = __get_CONTROL();
    regs->fpscr = __get_FPSCR();
}

extern void bk_set_jtag_mode(uint32_t cpu_id, uint32_t group_id);
extern volatile const uint8_t build_version[];


void bk_system_dump(const char *file, const int line) 
{
    uint32_t int_level = bk_dump_primask_save();
    SAVED_CONTEXT regs = {0};
    store_cpu_regs(ECAUSE_ASSERT, &regs);
    
#if defined(CONFIG_WATCHDOG)
    bk_wdt_feed();
#if (defined(CONFIG_INT_AON_WDT))
    bk_int_aon_wdt_feed();
#endif
#endif

    BK_DUMP_OUT("(%d)Assert at: %s:%d\r\n", rtos_get_time(), file, line);
    // BK_DUMP_OUT("build time => %s !\r\n", build_version);

    user_except_handler(RESET_SOURCE_CRASH_ASSERT, &regs);

    bk_set_jtag_mode(CPU_ID, 0);
    bk_dump_primask_restore(int_level);
}

void user_except_handler_ex(uint32_t reset_reason, uint32_t lr, uint32_t sp)
{

    dump_context(lr, sp);

    // BK_DUMP_OUT("build time => %s !\r\n", build_version);

    if (0 == g_enter_exception) { //prevent secondary exception.

        // Make sure the interrupt is disable
        uint32_t int_level = rtos_disable_int();

        /* Handled Trap */
        g_enter_exception = 1;

        rtos_dump_system(sp, __get_PSP());

        dump_epilogue();

        bk_reboot_ex(reset_reason);

        while(g_enter_exception);

        rtos_enable_int(int_level);
    } else {

        bk_misc_set_reset_reason(reset_reason);
        bk_wdt_force_reboot();
    }

}

static void arch_dump_cpu_registers_securt_fault(uint32_t mcause, SAVED_CONTEXT *context) {

    BK_DUMP_OUT("CPU%d Current regs:\r\n", CPU_ID);

    // context of task.
    BK_DUMP_OUT("0 r0 x 0x%lx\r\n", context->r0);
    BK_DUMP_OUT("1 r1 x 0x%lx\r\n", context->r1);
    BK_DUMP_OUT("2 r2 x 0x%lx\r\n", context->r2);
    BK_DUMP_OUT("3 r3 x 0x%lx\r\n", context->r3);
    BK_DUMP_OUT("4 r4 x 0x%lx\r\n", context->r4);
    BK_DUMP_OUT("5 r5 x 0x%lx\r\n", context->r5);
    BK_DUMP_OUT("6 r6 x 0x%lx\r\n", context->r6);
    BK_DUMP_OUT("7 r7 x 0x%lx\r\n", context->r7);
    BK_DUMP_OUT("8 r8 x 0x%lx\r\n", context->r8);
    BK_DUMP_OUT("9 r9 x 0x%lx\r\n", context->r9);
    BK_DUMP_OUT("10 r10 x 0x%lx\r\n", context->r10);
    BK_DUMP_OUT("11 r11 x 0x%lx\r\n", context->r11);
    BK_DUMP_OUT("12 r12 x 0x%lx\r\n", context->r12);
    BK_DUMP_OUT("14 sp x 0x%lx\r\n", context->sp);
    BK_DUMP_OUT("15 lr x 0x%lx\r\n", context->lr);
    BK_DUMP_OUT("16 pc x 0x%lx\r\n", context->pc);
    BK_DUMP_OUT("17 xpsr x 0x%lx\r\n", context->xpsr);
    BK_DUMP_OUT("18 msp x 0x%lx\r\n", context->msp);
    BK_DUMP_OUT("19 psp x 0x%lx\r\n", context->psp);
    BK_DUMP_OUT("20 primask x 0x%lx\r\n", context->primask);
    BK_DUMP_OUT("21 basepri x 0x%lx\r\n", context->basepri);
    BK_DUMP_OUT("22 faultmask x 0x%lx\r\n", context->faultmask);
    BK_DUMP_OUT("23 fpscr x 0x%lx\r\n", context->fpscr);

    // context of ISR.
    mcause = context->control;   // __get_xPSR();
    BK_DUMP_OUT("30 CPU%d xPSR x 0x%lx\r\n", CPU_ID, mcause);
    BK_DUMP_OUT("31 LR x 0x%lx\r\n", context->control);       // exception LR.
    BK_DUMP_OUT("32 control x 0x%lx\r\n", __get_CONTROL());

    BK_DUMP_OUT("40 MMFAR x 0x%lx\r\n", SCB->MMFAR);
    BK_DUMP_OUT("41 BFAR x 0x%lx\r\n", SCB->BFAR);
    BK_DUMP_OUT("42 CFSR x 0x%lx\r\n", SCB->CFSR);
    BK_DUMP_OUT("43 HFSR x 0x%lx\r\n", SCB->HFSR);

    mcause = mcause & 0x1FF;

    if ( (mcause <= 0x0F) && (fault_type[mcause] != NULL) )
    {
        BK_DUMP_OUT((char *)fault_type[mcause]);
    }
}


#if defined(CONFIG_TFM_S_TO_NS_DUMP_ENABLE)
//#include "tfm_aes_gcm_nsc.h"
#define FRAME_BUF_LEN    (64)

struct tfm_exception_info_t {
    uint32_t EXC_RETURN;        /* EXC_RETURN value in LR. */
    uint32_t MSP;               /* (Secure) MSP. */
    uint32_t PSP;               /* (Secure) PSP. */
    uint32_t CONTROL;               /* (Secure) CONTROL. */
    uint32_t MSP_NS;            /* (Non Secure) MSP. */
    uint32_t PSP_NS;            /* (Non Secure) PSP. */
    uint32_t CONTROL_NS;            /* (Non Secure) CONTROL. */
    uint32_t *EXC_FRAME;        /* Exception frame on stack. */
    uint32_t EXC_FRAME_COPY[8]; /* Copy of the basic exception frame. */
    uint32_t EXT_SAVED_FRAME[8]; /* Copy of exception frame r4-r11. */
    uint32_t xPSR;              /* Program Status Registers. */
    uint32_t SP;

    uint32_t CFSR;              /* Configurable Fault Status Register. */
    uint32_t HFSR;              /* Hard Fault Status Register. */
    uint32_t BFAR;              /* Bus Fault address register. */
    uint32_t BFARVALID;         /* Whether BFAR contains a valid address. */
    uint32_t MMFAR;             /* MemManage Fault address register. */
    uint32_t MMARVALID;         /* Whether MMFAR contains a valid address. */
    uint32_t SFSR;              /* SecureFault Status Register. */
    uint32_t SFAR;              /* SecureFault Address Register. */
    uint32_t SFARVALID;         /* Whether SFAR contains a valid address. */
    uint32_t *SE_EXC_FRAME;        /* Exception frame on Secure stack. */
    uint32_t SE_EXC_FRAME_BUF[FRAME_BUF_LEN];         /* the basic psp exception frame. */
};

static struct tfm_exception_info_t tfm_exception_info;

static void exception_frame_printf (uint32_t *ptr_buff , uint32_t buflen)
{
    if(!ptr_buff)
    {
	return ;
    }

    for (int i = 0; i <buflen; i += 8){
        for (int j=0; j<8; j++){
         if (i+j < buflen){
            BK_DUMP_OUT("%08X ", ptr_buff[i+j]);
        }else{
             BK_DUMP_OUT("   ");}}
        BK_DUMP_OUT(" ");
        BK_DUMP_OUT("\n");
    }
}

static void NS_handle_securt_fault(uint32_t reset_reason, struct tfm_exception_info_t *ctx)
{
    // BK_DUMP_OUT("build time => %s !\r\n", build_version);
    
    SAVED_CONTEXT regs;

    uint32_t lr ;
    uint32_t msp;
    uint32_t psp;

    lr  = ctx->EXC_RETURN;
    msp = ctx->MSP_NS;
    psp = ctx->PSP_NS;
  
    /* fill saved context */
    regs.control = lr;       // exception LR.
    regs.msp = msp;
    regs.psp = psp;

    regs.primask = __get_PRIMASK();
    regs.basepri = __get_BASEPRI();
    regs.faultmask = __get_FAULTMASK();

    regs.fpscr = __get_FPSCR();

    uint32_t stack_pointer = msp;

    if(ctx->CONTROL_NS & (1UL << 1)) //CONTROL_NS.SPSEL
    {
        stack_pointer = psp;
    }

    uint32_t stack_adj = 8 * sizeof(uint32_t);

    if((lr & (1UL << 4)) == 0)   //FType
    {
        stack_adj += 18 * sizeof(uint32_t);  // 18 FPU regs.
    }

    regs.r0 = ((uint32_t *)stack_pointer)[0];
    regs.r1 = ((uint32_t *)stack_pointer)[1];
    regs.r2 = ((uint32_t *)stack_pointer)[2];
    regs.r3 = ((uint32_t *)stack_pointer)[3];
    regs.r12 = ((uint32_t *)stack_pointer)[4];
    regs.lr = ((uint32_t *)stack_pointer)[5];
    regs.pc = ((uint32_t *)stack_pointer)[6];
    regs.xpsr = ((uint32_t *)stack_pointer)[7];

    regs.r4 = ctx->EXT_SAVED_FRAME[0];
    regs.r5 = ctx->EXT_SAVED_FRAME[1];
    regs.r6 = ctx->EXT_SAVED_FRAME[2];
    regs.r7 = ctx->EXT_SAVED_FRAME[3];
    regs.r8 = ctx->EXT_SAVED_FRAME[4];
    regs.r9 = ctx->EXT_SAVED_FRAME[5];
    regs.r10 = ctx->EXT_SAVED_FRAME[6];
    regs.r11 = ctx->EXT_SAVED_FRAME[7];
    
    regs.sp = stack_pointer + stack_adj;

#if defined(CONFIG_WATCHDOG)
    bk_wdt_feed();
#if (defined(CONFIG_INT_AON_WDT))
    bk_int_aon_wdt_feed();
#endif
#endif
    // bk_set_printf_sync(true);
    // dump_prologue();
    arch_dump_cpu_registers_securt_fault(0, &regs);

    if (0 == g_enter_exception) {

        // Make sure the interrupt is disable
        // uint32_t int_level = rtos_disable_int();

        /* Handled Trap */
        g_enter_exception = 1;

        rtos_dump_system(msp, psp);


        bk_reboot_ex(reset_reason);

        while(g_enter_exception);

        // rtos_enable_int(int_level);
    } else {

        bk_misc_set_reset_reason(reset_reason);
        bk_wdt_force_reboot();
    }
}

void bk_security_donmain_notifies_non_security_domain_to_dump(uint32_t *reg)
{
    struct tfm_exception_info_t *ctx = &tfm_exception_info;

    bk_wdt_feed();
#if (defined(CONFIG_INT_AON_WDT))
    bk_int_aon_wdt_feed();
#endif

    BK_DUMP_OUT("Here is some context for the exception:\r\n");
    BK_DUMP_OUT("EXC_RETURN (LR): 0x%x\r\n", ctx->EXC_RETURN);

    BK_DUMP_OUT("xPSR: 0x%x\r\n", ctx->xPSR);
    BK_DUMP_OUT("MSP: 0x%x\r\n", ctx->MSP);
    BK_DUMP_OUT("PSP: 0x%x\r\n", ctx->PSP);
    BK_DUMP_OUT("MSP_NS: 0x%x\r\n", ctx->MSP_NS);
    BK_DUMP_OUT("PSP_NS: 0x%x\r\n", ctx->PSP_NS);
    BK_DUMP_OUT("CONTROL: 0x%x\r\n", ctx->CONTROL);
    BK_DUMP_OUT("CONTROL_NS: 0x%x\r\n", ctx->CONTROL_NS);
    BK_DUMP_OUT("Exception frame at:0x%x \r\n", (uint32_t)ctx->EXC_FRAME);

    BK_DUMP_OUT("0 R0: 0x%x\r\n", ctx->EXC_FRAME_COPY[0]);
    BK_DUMP_OUT("1 R1: 0x%x\r\n", ctx->EXC_FRAME_COPY[1]);
    BK_DUMP_OUT("2 R2: 0x%x\r\n", ctx->EXC_FRAME_COPY[2]);
    BK_DUMP_OUT("3 R3: 0x%x\r\n", ctx->EXC_FRAME_COPY[3]);
    BK_DUMP_OUT("4 R4: 0x%x\r\n", ctx->EXT_SAVED_FRAME[0]);
    BK_DUMP_OUT("5 R5: 0x%x\r\n", ctx->EXT_SAVED_FRAME[1]);
    BK_DUMP_OUT("6 R6: 0x%x\r\n", ctx->EXT_SAVED_FRAME[2]);
    BK_DUMP_OUT("7 R7: 0x%x\r\n", ctx->EXT_SAVED_FRAME[3]);
    BK_DUMP_OUT("8 R8: 0x%x\r\n", ctx->EXT_SAVED_FRAME[4]);
    BK_DUMP_OUT("9 R9: 0x%x\r\n", ctx->EXT_SAVED_FRAME[5]);
    BK_DUMP_OUT("10 R10: 0x%x\r\n", ctx->EXT_SAVED_FRAME[6]);
    BK_DUMP_OUT("11 R11: 0x%x\r\n", ctx->EXT_SAVED_FRAME[7]);
    BK_DUMP_OUT("12 R12: 0x%x\r\n", ctx->EXC_FRAME_COPY[4]);
    BK_DUMP_OUT("13 LR: 0x%x\r\n", ctx->EXC_FRAME_COPY[5]);
    BK_DUMP_OUT("14 PC: 0x%x\r\n", ctx->EXC_FRAME_COPY[6]);
    BK_DUMP_OUT("15 xPSR: 0x%x\r\n", ctx->EXC_FRAME_COPY[7]);
    BK_DUMP_OUT("CFSR: 0x%x\r\n", ctx->CFSR);
    BK_DUMP_OUT("BFSR: 0x%x\r\n", (ctx->CFSR & (0xFFUL << 8)) >> 8);
    BK_DUMP_OUT("BFAR: 0x%x\r\n", ctx->BFAR);
    BK_DUMP_OUT("HFSR: 0x%x\r\n", ctx->HFSR);
    BK_DUMP_OUT("SFAR: 0x%x\r\n", ctx->SFAR);
    BK_DUMP_OUT("SFSR: 0x%x\r\n", ctx->SFSR);
    BK_DUMP_OUT("SFARVALID: 0x%x\r\n", ctx->SFARVALID);

    uint32_t buflen = (sizeof(ctx->SE_EXC_FRAME_BUF) >>2);

    BK_DUMP_OUT(">>>>SE stack mem dump begin, stack_top=%08x, stack end=%08x\r\n", ctx->SE_EXC_FRAME_BUF, &(ctx->SE_EXC_FRAME_BUF[FRAME_BUF_LEN -1]));
    exception_frame_printf((uint32_t*)(ctx->SE_EXC_FRAME_BUF), buflen);
    BK_DUMP_OUT("<<<<SE stack mem dump end. stack_top=%08x, stack end=%08x\r\n", ctx->SE_EXC_FRAME_BUF, &(ctx->SE_EXC_FRAME_BUF[FRAME_BUF_LEN -1]));

    NS_handle_securt_fault(RESET_SOURCE_SECURE_FAULT, ctx);
}

void bk_security_to_nosecurity_dump_register_callback(void)
{
    uint32_t callback_address = (uint32_t)(&bk_security_donmain_notifies_non_security_domain_to_dump);
    psa_register_dump_callback(callback_address, (uint32_t)&tfm_exception_info);
}
#endif

/* if mpu enable, accessing itcm zero pointer violates the mpu rule, please refer to mpu_cfg
 * if mpu disable, null pointer/zero pointer maybe is a software fault. So bk_null_trap_handler
 *    can catch issue;
 */
#if defined(CONFIG_MPU)
        /*mpu_region about itcm will catch null pointer, for zero address is out of mpu setting*/
#else
__attribute__((used, section(".null_trap_handler"))) \
void bk_null_trap_handler(void)
{
    __asm volatile(
                "nop\n\t"
                "nop\n\t"
                "nop\n\t"
                "nop\n\t"
                "nop\n\t"
                "nop\n\t"
                "nop\n\t"
                "nop\n\t"
                "nop\n\t"
                "nop\n\t"
        );
        BK_ASSERT(0);
}
#endif


void bk_dump_entry(uint32_t is_irq_assert)
{
    // uint32_t primask = bk_dump_primask_save();
    SAVED_CONTEXT regs = {0};
    store_cpu_regs(ECAUSE_ASSERT, &regs);
  
    arch_dump_cpu_registers(ECAUSE_ASSERT, &regs ,is_irq_assert);

    rtos_dump_system(0,0);  

    // bk_dump_primask_restore(primask);

    // bk_reboot();
}