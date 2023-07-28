/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include <tinyara/config.h>
#include <tinyara/mpu.h>
#include <tinyara/mm/heap_regioninfo.h>

#include "ameba_soc.h"
#include "amebalite_reboot_reason.h"
//#include "psram_reserve.h"

#if defined(CONFIG_EXAMPLE_CM_BACKTRACE) && CONFIG_EXAMPLE_CM_BACKTRACE
#include "cm_backtrace/example_cm_backtrace.h"
#endif

#ifdef CONFIG_ARMV8M_MPU
#include "up_mpuinit.h"
#endif
#ifdef CONFIG_STACK_COLORATION
#include "up_internal.h"
#endif
#if defined ( __ICCARM__ )
#pragma section=".ram_image2.bss"
#pragma section=".ram_image2.nocache.data"
#pragma section=".psram.bss"

SECTION(".data") u8 *__bss_start__ = 0;
SECTION(".data") u8 *__bss_end__ = 0;
SECTION(".data") u8 *__ram_nocache_start__ = 0;
SECTION(".data") u8 *__ram_nocache_end__ = 0;
SECTION(".data") u8 *__psram_bss_start__ = 0;
SECTION(".data") u8 *__psram_bss_end__ = 0;
#endif
static u32 g_mpu_nregion_allocated;
extern int main(void);
extern u32 GlobalDebugEnable;

#ifdef CONFIG_PLATFORM_TIZENRT_OS
extern unsigned int _sidle_stack;
extern unsigned int _sext_heap;
extern unsigned int _sint_heap;
extern unsigned int __StackLimit;
extern unsigned int __PsramStackLimit;
void NS_ENTRY BOOT_IMG3(void);
void app_init_psram(void);

#define IDLE_STACK ((uintptr_t)&_sidle_stack + CONFIG_IDLETHREAD_STACKSIZE - 4)
#define HEAP_BASE  ((uintptr_t)&_sint_heap)
#define HEAP_LIMIT ((uintptr_t)&__StackLimit)
#define PSRAM_HEAP_BASE ((uintptr_t)&_sext_heap)
#define PSRAM_HEAP_LIMIT ((uintptr_t)&__PsramStackLimit)

const uintptr_t g_idle_topstack = IDLE_STACK;

void os_heap_init(void){
	kregionx_start[0] = (void *)HEAP_BASE;
	kregionx_size[0] = (size_t)(HEAP_LIMIT - HEAP_BASE);
#if CONFIG_KMM_REGIONS >= 2
#if CONFIG_KMM_REGIONS == 3
	kregionx_start[1] = (void *)PSRAM_HEAP_BASE;
	kregionx_size[1] = (size_t)kregionx_start[1] + kregionx_size[1] - PSRAM_HEAP_BASE;

	kregionx_start[2] = (void *)kregionx_start[2];
	kregionx_size[2] = (size_t)PSRAM_HEAP_LIMIT - (size_t)kregionx_start[2];
#elif CONFIG_KMM_REGIONS > 3
#error "Need to check here for heap."
#else
	kregionx_start[1] = (void *)PSRAM_HEAP_BASE;
	kregionx_size[1] = (size_t)(PSRAM_HEAP_LIMIT - PSRAM_HEAP_BASE);
#endif
#endif
}
#endif

#define dbg_printf rtw_printf
typedef struct fault_handler_back_trace_s {
    uint32_t msp_top;         /*!< the Top address of the MSP */
    uint32_t msp_limit;       /*!< the Limit address of the MSP */
    uint32_t ps_max_size;     /*!< the maximum stack size of PSP */
    uint32_t *ptxt_range_list;  /*!< point to the array for the list of text code range */
    uint32_t trace_depth;   /*!< the size of the trace buffer */
    uint32_t *ptrace_buf;   /*!< point to the trace buffer */
    uint32_t *poffset_buf;   /*!< point to the buffer for offset from SP in trace */
} fault_handler_back_trace_t, *pfault_handler_back_trace_t;
#define FAULT_BACK_TRACE_BUF_DEPTH      8
uint32_t hard_fault_back_trace_buf[FAULT_BACK_TRACE_BUF_DEPTH];
uint32_t hard_fault_back_trace_sp_offset[FAULT_BACK_TRACE_BUF_DEPTH];
extern u8 __flash_text_end__[];
// extern u8 __ram_image2_text_start__[];
extern u8 __ram_image2_text_end__[];
extern u8 __sram_text_start__[];
extern u8 __sram_text_end__[];
const uint32_t hard_fault_back_trace_txt_range[] = {
    /* code start,              code end */
    0x0001E000,                 0x00034000,             // conde in ITCM ROM
    (uint32_t)&__flash_text_start__,   (uint32_t)&__flash_text_end__,  // code in XIP //FENG no xip for lite, these 2 are in region KM4_BD_PSRAM_NS_NTZ
    // (uint32_t)&__ram_image2_text_start__, (uint32_t)&__ram_image2_text_end__,    // code in PSRAM //FENG these 2 are actually psram text in lite ld
    (uint32_t)&__sram_text_start__,   (uint32_t)&__sram_text_end__,      // code in Internal SRAM

    0xFFFFFFFF                                          // end of list
};
const fault_handler_back_trace_t hard_fault_back_trace = {
    .msp_top = MSP_RAM_HP_NS,
    .msp_limit = MSP_RAM_HP_NS - 4*1024,
    .ps_max_size = (1024*16),
    .ptxt_range_list = (uint32_t *)hard_fault_back_trace_txt_range,
    .trace_depth = FAULT_BACK_TRACE_BUF_DEPTH,
    .ptrace_buf = hard_fault_back_trace_buf,
    .poffset_buf = hard_fault_back_trace_sp_offset
};

fault_handler_back_trace_t *pfault_back_trace = (fault_handler_back_trace_t *)&hard_fault_back_trace;
#if defined(CONFIG_BUILD_SECURE) && (CONFIG_BUILD_SECURE == 1)
fault_handler_back_trace_t *pfault_back_trace_ns = &hard_fault_back_trace;
#endif
#define IN_RANGE(addr, low, high) ((((uint32_t)addr)>=((uint32_t)low))&&(((uint32_t)addr)<((uint32_t)high)))
/**
  \brief        Checks if the given address in the memory range of text code.
  \param [in]   arrd  The address for the cheching.
  \return       true: the given address is located in the text range.
  \return       false: the given address is NOT located in the text range or no text range list.
 */
BOOLEAN check_in_text_area(uint32_t addr)
{
    uint32_t *range_start;
    uint32_t *range_end;
    uint32_t *ptxt_range_list;

    if ((pfault_back_trace != NULL) && (pfault_back_trace->ptxt_range_list != NULL)) {
        ptxt_range_list = pfault_back_trace->ptxt_range_list;
        range_start = (uint32_t *)(*ptxt_range_list);
        range_end = (uint32_t *)(*(ptxt_range_list + 1));
        while ((uint32_t)range_start != 0xFFFFFFFFUL) {
            if ((uint32_t)range_end > (uint32_t)range_start) {
                if(IN_RANGE(addr, range_start, range_end)) {
                    return TRUE;
                }
            }
            ptxt_range_list += 2;
            range_start = (uint32_t *)(*ptxt_range_list);
            range_end = (uint32_t *)(*(ptxt_range_list + 1));
        }

        return FALSE;
    } else {
        return FALSE;
    }
}

static void store_trace_back_sp_offset (uint32_t idx, uint32_t *pbuf, uint32_t offset)
{
    if (pbuf != NULL) {
        *(pbuf + idx) = offset;
    }
}

#if !defined(CONFIG_BUILD_NONSECURE)

void stack_trace_back(uint32_t msp, uint32_t psp, uint32_t fault_lr, uint32_t trace_ns)
{
    uint32_t stack_top;
    uint32_t stack_lim;
    uint32_t sp;
    uint32_t *sp_ptr;
    uint32_t lr;
    uint32_t pc;
    uint32_t *trace_buffer;
    uint32_t *offset_buffer;
    uint32_t trace_idx = 0;
    uint32_t *sp_offset;
    uint32_t from_int = 0;
    uint32_t i;
    fault_handler_back_trace_t *pback_trace;
    uint32_t trace_loop;
//    uint32_t loop_i;

#if defined(CONFIG_BUILD_SECURE) && (CONFIG_BUILD_SECURE == 1)
    if (trace_ns) {
        pback_trace = pfault_back_trace_ns;
    } else {
        pback_trace = pfault_back_trace;
    }
#else
    pback_trace = pfault_back_trace;
#endif

    if ((pback_trace == NULL) || (pback_trace->ptrace_buf == NULL)) {
        dbg_printf("No Back Trace!\r\n");
        return;
    }

    if (trace_ns && psp != 0) {
        // trace NS stack: for both MS & PS
        trace_loop = 2;
    } else {
        // trace only MS or PS
        trace_loop = 1;
    }

    dbg_printf("\n\r== Back Trace ==\n\r\n\r");
    dbg_printf("msp=0x%08x psp=0x%08x\r\n", msp, psp);

    do {

        trace_buffer = pback_trace->ptrace_buf;
        offset_buffer = pback_trace->poffset_buf;
        _memset((void *)trace_buffer, 0, sizeof(uint32_t) * pback_trace->trace_depth);

        do {
            /* SPSEL, bit [2]
               Stack pointer selection. Indicates which stack pointer the exception frame resides on.
               The possible values of this bit are:
               0 Main stack pointer.
               1 Process stack pointer
            */
            if (!trace_ns) {
                if ((fault_lr & BIT2) != 0) {
                    // use PSP
                    sp = psp;
                    stack_lim = sp;
                    stack_top = sp + pback_trace->ps_max_size;
                    sp_ptr = &psp;
                    dbg_printf("Process stack back trace:\n\r");
                } else {
                    // use MSP
                    stack_top = pback_trace->msp_top;
                    stack_lim = pback_trace->msp_limit;
                    sp = msp;
                    sp_ptr = &msp;
                    dbg_printf("Main stack back trace:\n\r");
                }
            } else {
                if ((trace_loop > 1) || (psp == 0)) {
                    // trace MS
                    stack_top = pback_trace->msp_top;
                    stack_lim = pback_trace->msp_limit;
                    sp = msp;
                    sp_ptr = &msp;
                    dbg_printf("Main stack back trace:\n\r");
                } else {
                    // trace PS
                    sp = psp;
                    stack_lim = sp;
                    stack_top = sp + pback_trace->ps_max_size;
                    sp_ptr = &psp;
                    dbg_printf("Process stack back trace:\n\r");
                }
            }
            dbg_printf("top=0x%08x lim=0x%08x\r\n", stack_top, stack_lim);

            /* DCRS, bit [5]
               Default callee register stacking. Indicates whether the default stacking rules apply, or whether the
               callee registers are already on the stack.
               The possible values of this bit are:
               0 Stacking of the callee saved registers skipped.
               1 Default rules for stacking the callee registers followed
            */
            if ((fault_lr & BIT5) != 0) {
                lr = *((uint32_t *)(sp + 20));
                pc = *((uint32_t *)(sp + 24));
                if ((fault_lr & BIT4) != 0) {
                    //standard stacking format
                    sp += (8 * 4);
                    *sp_ptr += (8 * 4);
                } else {
                    //extend stacking format
                    sp += (26 * 4);
                    *sp_ptr += (26 * 4);
                }

                store_trace_back_sp_offset(trace_idx, offset_buffer, 0);
                trace_buffer[trace_idx++] = pc;
                store_trace_back_sp_offset(trace_idx, offset_buffer, 0);
                trace_buffer[trace_idx++] = lr - 4;

                if (((lr & 0xFFFFFF00) == 0xFFFFFF00) && ((lr & BIT3) == 0)) {
                    // stacked from a hard fault handler
                    fault_lr = lr;
                    from_int = 1;
                } else {
                    from_int = 0;
                }
            }
        } while (from_int != 0);

        sp_offset = (uint32_t*)sp;
        while(IN_RANGE((uint32_t)sp_offset, (uint32_t)stack_lim, (uint32_t)stack_top)){
            // check stack end
            if (*sp_offset & 0x01) {
                if(check_in_text_area(*sp_offset)){
                    store_trace_back_sp_offset(trace_idx, offset_buffer, ((uint32_t)sp_offset));
                    trace_buffer[trace_idx++] = *sp_offset - 4;

                    if(trace_idx >= pback_trace->trace_depth){
                        break;
                    }
                }
            }
            sp_offset++;

            // maximum trace 64K
            if (((uint32_t)sp_offset - sp) > (1024*64)) {
                break;
            }
        }

        for(i=0;i<trace_idx;i++){
            if (offset_buffer == NULL) {
                dbg_printf("%08x \n\r", trace_buffer[i]);
            } else {
                dbg_printf("%08x @ sp = %08x\n\r", trace_buffer[i], offset_buffer[i]);
            }
        }

        dbg_printf("\r\nBacktrace information may not correct! Use this command to get C source level information:\r\narm-none-eabi-addr2line -e ELF_file -a -f ");
        for(i=0;i<trace_idx;i++){
            dbg_printf("%08x ", trace_buffer[i]);
        }
        dbg_printf("\n\r\n\r");
        trace_loop--;
        trace_idx = 0;
    } while (trace_loop > 0);

}

#else   // else of "#if !defined(CONFIG_BUILD_NONSECURE)"

void stack_trace_back(uint32_t msp, uint32_t psp, uint32_t fault_lr, uint32_t trace_ns)
{
    uint32_t stack_top;
    uint32_t stack_lim;
    uint32_t sp;
    uint32_t *sp_ptr;
    uint32_t lr;
    uint32_t pc;
    uint32_t *trace_buffer;
    uint32_t *offset_buffer;
    uint32_t trace_idx = 0;
    uint32_t *sp_offset;
    uint32_t from_int = 0;
    uint32_t i;
    fault_handler_back_trace_t *pback_trace;

    pback_trace = pfault_back_trace;

    if ((pback_trace == NULL) || (pback_trace->ptrace_buf == NULL)) {
        dbg_printf("No Back Trace!\r\n");
        return;
    }

    trace_buffer = pback_trace->ptrace_buf;
    offset_buffer = pback_trace->poffset_buf;
    _memset((void *)trace_buffer, 0, sizeof(uint32_t) * pback_trace->trace_depth);

    do {
        /* SPSEL, bit [2]
           Stack pointer selection. Indicates which stack pointer the exception frame resides on.
           The possible values of this bit are:
           0 Main stack pointer.
           1 Process stack pointer
        */
        if ((fault_lr & BIT2) != 0) {
            // use PSP
            sp = psp;
            stack_lim = sp;
            stack_top = sp + pback_trace->ps_max_size;
            sp_ptr = &psp;
        } else {
            // use MSP
            stack_top = pback_trace->msp_top;
            stack_lim = pback_trace->msp_limit;
            sp = msp;
            sp_ptr = &msp;
        }

        /* DCRS, bit [5]
           Default callee register stacking. Indicates whether the default stacking rules apply, or whether the
           callee registers are already on the stack.
           The possible values of this bit are:
           0 Stacking of the callee saved registers skipped.
           1 Default rules for stacking the callee registers followed
        */
        if ((fault_lr & BIT5) != 0) {
            lr = *((uint32_t *)(sp + 20));
            pc = *((uint32_t *)(sp + 24));
            if ((fault_lr & BIT4) != 0) {
                //standard stacking format
                sp += (8 * 4);
                *sp_ptr += (8 * 4);
            } else {
                //extend stacking format
                sp += (26 * 4);
                *sp_ptr += (26 * 4);
            }

            store_trace_back_sp_offset(trace_idx, offset_buffer, 0);
            trace_buffer[trace_idx++] = pc;
            store_trace_back_sp_offset(trace_idx, offset_buffer, 0);
            trace_buffer[trace_idx++] = lr - 4;

            if (((lr & 0xFFFFFF00) == 0xFFFFFF00) && ((lr & BIT3) == 0)) {
                // stacked from a hard fault handler
                fault_lr = lr;
                from_int = 1;
            } else {
                from_int = 0;
            }
        }
    } while (from_int != 0);

    sp_offset = (uint32_t*)sp;
    while(IN_RANGE((uint32_t)sp_offset, (uint32_t)stack_lim, (uint32_t)stack_top)){
        // check stack end
        if (*sp_offset & 0x01) {
            if(check_in_text_area(*sp_offset)){
                store_trace_back_sp_offset(trace_idx, offset_buffer, ((uint32_t)sp_offset));
                trace_buffer[trace_idx++] = *sp_offset - 4;

                if(trace_idx >= pback_trace->trace_depth){
                    break;
                }
            }
        }
        sp_offset++;

        // maximum trace 64K
        if (((uint32_t)sp_offset - sp) > (1024*64)) {
            break;
        }
    }

    dbg_printf("\n\r== Back Trace ==\n\r\n\r");
    dbg_printf("msp=0x%08x psp=0x%08x top=0x%08x lim=0x%08x\r\n", msp, psp, stack_top, stack_lim);
    for(i=0;i<trace_idx;i++){
        if (offset_buffer == NULL) {
            dbg_printf("%08x \n\r", trace_buffer[i]);
        } else {
            dbg_printf("%08x @ sp = %08x\n\r", trace_buffer[i], offset_buffer[i]);
        }
    }

    dbg_printf("\r\nBacktrace information may not correct! Use this command to get C source level information:\r\narm-none-eabi-addr2line -e ELF_file -a -f ");
    for(i=0;i<trace_idx;i++){
        dbg_printf("%08x ", trace_buffer[i]);
    }
    dbg_printf("\n\r\n\r");
}
#endif      // end of else of "#if !defined(CONFIG_BUILD_NONSECURE)"

void parse_cfsr(uint32_t cfsr)
{
    dbg_printf("SCB Configurable Fault Status Reg = 0x%08x\r\n", cfsr);
    if (cfsr & (0xFFFF << 16)) {
        dbg_printf("\r\nUsage Fault Status: \r\n");
        if (cfsr & SCB_CFSR_DIVBYZERO_Msk) {
            dbg_printf("Divide by zero UsageFault\r\n");
        }

        if (cfsr & SCB_CFSR_UNALIGNED_Msk) {
            dbg_printf("Unaligned access UsageFault\r\n");
        }

        if (cfsr & SCB_CFSR_STKOF_Msk) {
            dbg_printf("Stack overflow UsageFault\r\n");
        }

        if (cfsr & SCB_CFSR_NOCP_Msk) {
            dbg_printf("No coprocessor UsageFault. The processor does not support coprocessor instructions.\r\n");
        }

        if (cfsr & SCB_CFSR_INVPC_Msk) {
            dbg_printf("Invalid PC load UsageFault, caused by an invalid PC load by EXC_RETURN.\r\n");
        }

        if (cfsr & SCB_CFSR_INVSTATE_Msk) {
            dbg_printf("Invalid state UsageFault, the processor has attempted to execute an instruction that makes illegal use of the EPSR\r\n");
        }

        if (cfsr & SCB_CFSR_UNDEFINSTR_Msk) {
            dbg_printf("Undefined instruction UsageFault\r\n");
        }
    }

    if (cfsr & (0xFF << 8)) {
        dbg_printf("\r\nBus Fault Status: \r\n");
        if (cfsr & SCB_CFSR_BFARVALID_Msk) {
            dbg_printf("BusFault Address Reg = 0x%08x\r\n", SCB->BFAR);
        } else {
            dbg_printf("BusFault Address Reg is invalid(Asyn. BusFault)\r\n");
        }

        if (cfsr & SCB_CFSR_LSPERR_Msk) {
            dbg_printf("BusFault occurred during floating-point lazy state preservation\r\n");
        }

        if (cfsr & SCB_CFSR_STKERR_Msk) {
            dbg_printf("BusFault on stacking for exception entry\r\n");
        }

        if (cfsr & SCB_CFSR_UNSTKERR_Msk) {
            dbg_printf("BusFault on unstacking for a return from exception\r\n");
        }

        if (cfsr & SCB_CFSR_IMPRECISERR_Msk) {
            /* This is an asynchronous fault. Therefore, if it is detected when the priority of the current
               process is higher than the BusFault priority, the BusFault becomes pending and becomes active
               only when the processor returns from all higher priority processes.
               If a precise fault occurs before the processor enters the handler for the imprecise BusFault,
               the handler detects both IMPRECISERR set to 1 and one of the precise fault status bits set to 1.*/
            dbg_printf("Imprecise data bus error:\r\n");
            dbg_printf("a data bus error has occurred, but the return address in the stack frame is not related to the instruction that caused the error.\r\n");
        }

        if (cfsr & SCB_CFSR_PRECISERR_Msk) {
            dbg_printf("Precise data bus error: BFAR = 0x%08x\r\n", SCB->BFAR);
        }

        if (cfsr & SCB_CFSR_IBUSERR_Msk) {
            dbg_printf("Instruction bus error\r\n");
        }
    }

    if (cfsr & 0xFF) {
        dbg_printf("\r\nMemManage Fault Status: \r\n");
        if (cfsr & SCB_CFSR_MMARVALID_Msk) {
            dbg_printf("MemManage Fault Address Reg = 0x%08x\r\n", SCB->MMFAR);
        } else {
            dbg_printf("MemManage Fault Address Reg is invalid\r\n");
        }

        if (cfsr & SCB_CFSR_MLSPERR_Msk) {
            dbg_printf("MemManage fault occurred during floating-point lazy state preservation\r\n");
        }

        if (cfsr & SCB_CFSR_MSTKERR_Msk) {
            dbg_printf("MemManage fault on stacking for exception entry\r\n");
        }

        if (cfsr & SCB_CFSR_MUNSTKERR_Msk) {
            dbg_printf("MemManage fault on unstacking for a return from exception\r\n");
        }

        if (cfsr & SCB_CFSR_DACCVIOL_Msk) {
            dbg_printf("Data access violation: the processor attempted a load or store at a location that does not permit the operation.\r\n");
        }

        if (cfsr & SCB_CFSR_IACCVIOL_Msk) {
            dbg_printf("Instruction access violation: the processor attempted an instruction fetch from a location that does not permit execution.\r\n");
        }
    }

}

void UsageFault_Handler_ram(void)
{
    __ASM volatile(
        "PUSH {R4-R11}\n\t"
        "MRS R0, MSP\n\t"
        "MRS R1, PSP\n\t"
        "MOV R2, LR\n\t" /* second parameter is LR current value */
        "MOV R3, #1\n\t"
        "SUB.W	R4, R0, #128\n\t"
        "MSR MSP, R4\n\t"
        "LDR R4,=hard_fault_handler_c\n\t"
        "BX R4\n\t"
    );
}

void BusFault_Handler_ram(void)
{
    __ASM volatile(
        "PUSH {R4-R11}\n\t"
        "MRS R0, MSP\n\t"
        "MRS R1, PSP\n\t"
        "MOV R2, LR\n\t" /* second parameter is LR current value */
        "MOV R3, #2\n\t"
        "SUB.W	R4, R0, #128\n\t"
        "MSR MSP, R4\n\t"
        "LDR R4,=hard_fault_handler_c\n\t"
        "BX R4\n\t"
    );
}

void MemManage_Handler_ram(void)
{
    __ASM volatile(
        "PUSH {R4-R11}\n\t"
        "MRS R0, MSP\n\t"
        "MRS R1, PSP\n\t"
        "MOV R2, LR\n\t" /* second parameter is LR current value */
        "MOV R3, #3\n\t"
        "SUB.W	R4, R0, #128\n\t"
        "MSR MSP, R4\n\t"
        "LDR R4,=hard_fault_handler_c\n\t"
        "BX R4\n\t"
    );
}

void SecureFault_Handler_ram(void)
{
    __ASM volatile(
        "PUSH {R4-R11}\n\t"
        "MRS R0, MSP\n\t"
        "MRS R1, PSP\n\t"
        "MOV R2, LR\n\t" /* second parameter is LR current value */
        "MOV R3, #4\n\t"
        "SUB.W	R4, R0, #128\n\t"
        "MSR MSP, R4\n\t"
        "LDR R4,=hard_fault_handler_c\n\t"
        "BX R4\n\t"
    );
}

void HardFault_Handler_ram(void)
{
	__ASM volatile(
        "PUSH {R4-R11}\n\t"
        "MRS R0, MSP\n\t"
        "MRS R1, PSP\n\t"
        "MOV R2, LR\n\t" /* second parameter is current LR value */
        "MOV R3, #0\n\t"
        "SUB.W	R4, R0, #128\n\t"
        "MSR MSP, R4\n\t"   // Move MSP to upper to for we can dump current stack contents without chage contents
        "LDR R4,=hard_fault_handler_c\n\t"
        "BX R4\n\t"
    );
}

/**
 *  @brief The default hard fault interrupt handler.
 *         It will print out the stack contents and the fault status registers.
 *
 *  @returns    void
 */
void hard_fault_handler_c(uint32_t mstack[], uint32_t pstack[], uint32_t lr_value, uint32_t fault_id)
{
    uint32_t ret, i, src, *stack;
    uint32_t xpsr=__get_xPSR();
    uint32_t CFSR, HFSR, DFSR, MMFAR, BFAR, AFSR;

    enum { r0=0, r1, r2, r3, r12, lr, pc, psr};
    enum { r4=0, r5, r6, r7, r8, r9, r10, r11};

    /*
        0xE000ED28 CFSR Configurable Fault Status Register
        0xE000ED28 MMFSR MemManage Fault Status Register
        0xE000ED29 BFSR BusFault Status Register
        0xE000ED2A UFSR UsageFault Status Register
        0xE000ED2C HFSR HardFault Status Register
        0xE000ED30 DFSR Debug Fault Status Register
        0xE000ED34 MMFAR MemManage Fault Address Register
        0xE000ED38 BFAR BusFault Address Register
        0xE000ED3C AFSR Auxiliary Fault Status Register

        0xE002ED28 CFSR_NS Configurable Fault Status Register (NS)
        0xE002ED28 MMFSR_NS MemManage Fault Status Register (NS)
        0xE002ED29 BFSR_NS BusFault Status Register (NS)
        0xE002ED2A UFSR_NS UsageFault Status Register (NS)
        0xE002ED2C HFSR_NS HardFault Status Register (NS)
        0xE002ED30 DFSR_NS Debug Fault Status Register (NS)
        0xE002ED34 MMFAR_NS MemManage Fault Address Register (NS)
        0xE002ED38 BFAR_NS BusFault Address Register (NS)
        0xE002ED3C AFSR_NS Auxiliary Fault Status Register (NS
    */
    CFSR  = SCB->CFSR;
    HFSR  = SCB->HFSR;
    DFSR  = SCB->DFSR;
    MMFAR = SCB->MMFAR;
    BFAR  = SCB->BFAR;
    AFSR  = SCB->AFSR;


    if ((lr_value & 0x04) == 0) {
        stack = mstack + 8;     // since we pushed R4 ~ R11 in MSP
    } else {
        stack = pstack;
    }



    if (1) {
#if !defined (CONFIG_BUILD_NONSECURE)
        dbg_printf("\r\nS-Domain Fault Handler: msp=0x%08x psp=0x%08x lr=0x%08x fault_id=%u\r\n",
                    (uint32_t)mstack, (uint32_t)pstack, lr_value, fault_id);
#else
        dbg_printf("\r\nNS-Domain Fault Handler: msp=0x%08x psp=0x%08x lr=0x%08x fault_id=%u\r\n",
                    (uint32_t)mstack, (uint32_t)pstack, lr_value, fault_id);
#endif
    }

    if (fault_id == 0) {
        dbg_printf("\r\nHard Fault: \r\n");
        dbg_printf("HardFault Status Reg = 0x%08x\r\n", HFSR);
        if (HFSR & SCB_HFSR_FORCED_Msk) {
            // forced HardFault
            /* When this bit is set to 1, the HardFault handler must read the other
               fault status registers to find the cause of the fault. */
            dbg_printf("Forced HardFault\r\n");
            dbg_printf("SCB Configurable Fault Status Reg = 0x%08x\r\n", CFSR);
        }

        if (HFSR & SCB_HFSR_VECTTBL_Msk) {
            // Indicates a BusFault on a vector table read during exception processing
            /* When this bit is set to 1, the PC value stacked for the exception return points to
               the instruction that was preempted by the exception.*/
            dbg_printf("BusFault on vector table read\r\n");
        }
    } else if (fault_id == 1) {
        dbg_printf("\r\nUsage Fault: \r\n");
    } else if (fault_id == 2) {
        dbg_printf("\r\nBus Fault: \r\n");
    } else if (fault_id == 3) {
        dbg_printf("\r\nMemManage Fault: \r\n");
    } else if (fault_id == 4) {
        dbg_printf("\r\nSecurity Fault: \r\n");
    } else {
        dbg_printf("\r\nDefault Hard Fault: \r\n");
    }
    parse_cfsr(CFSR);

    if ((lr_value & BIT0) == 0) {
        dbg_printf("\r\nNS-domain exception ");
    } else {
        dbg_printf("\r\nS-domain exception ");
    }

    if ((lr_value & BIT3) == 0) {
        dbg_printf("from Handler mode, ");
    } else {
        dbg_printf("from Thread mode, ");
    }

    if ((lr_value & BIT4) == 0) {
        dbg_printf("Extended Stack frame on");
    } else {
        dbg_printf("Standard Stack frame on");
    }

    if ((lr_value & BIT6) == 0) {
        dbg_printf(" NS-");
#if !defined (CONFIG_BUILD_NONSECURE)
        if ((lr_value & 0x04) == 0) {
            stack = (uint32_t *)(__TZ_get_MSP_NS());
        } else {
            stack = (uint32_t *)(__TZ_get_PSP_NS());
        }
#endif
    } else {
        dbg_printf(" S-");
    }

    if ((lr_value & BIT2) == 0) {
        dbg_printf("MSP\r\n");
    } else {
        dbg_printf("PSP\r\n");
    }

    if ((lr_value & BIT5) == 0) {
        dbg_printf("Registers Stacking Skiped\r\n\r\n");
    } else {
        dbg_printf("Registers Saved to stack\r\n\r\n");
    }

    dbg_printf("Stacked: \r\n");
    dbg_printf("R0  = 0x%08x\r\n", stack[r0]);
    dbg_printf("R1  = 0x%08x\r\n", stack[r1]);
    dbg_printf("R2  = 0x%08x\r\n", stack[r2]);
    dbg_printf("R3  = 0x%08x\r\n", stack[r3]);

    // we stacked R4 ~ R11 in the exception handler
    dbg_printf("R4  = 0x%08x\r\n", mstack[r4]);
    dbg_printf("R5  = 0x%08x\r\n", mstack[r5]);
    dbg_printf("R6  = 0x%08x\r\n", mstack[r6]);
    dbg_printf("R7  = 0x%08x\r\n", mstack[r7]);
    dbg_printf("R8  = 0x%08x\r\n", mstack[r8]);
    dbg_printf("R9  = 0x%08x\r\n", mstack[r9]);
    dbg_printf("R10 = 0x%08x\r\n", mstack[r10]);
    dbg_printf("R11 = 0x%08x\r\n", mstack[r11]);
    mstack = (uint32_t *)((uint32_t)mstack + 0x20); // recover stack pointer for we pushed R4 ~ R11

    dbg_printf("R12 = 0x%08x\r\n", stack[r12]);
    dbg_printf("LR  = 0x%08x\r\n", stack[lr]);
    dbg_printf("PC  = 0x%08x\r\n", stack[pc]);
    dbg_printf("PSR = 0x%08x\r\n", stack[psr]);

    dbg_printf("\r\nCurrent: \r\n");
    dbg_printf("LR   = 0x%08x\r\n", lr_value);
    dbg_printf("MSP  = 0x%08x\r\n", mstack);
    dbg_printf("PSP  = 0x%08x\r\n", pstack);
    dbg_printf("xPSR = 0x%08x\r\n", xpsr);

#if !defined (CONFIG_BUILD_NONSECURE)
    dbg_printf("CFSR  = 0x%08x\r\n", CFSR);
    dbg_printf("HFSR  = 0x%08x\r\n", HFSR);
    dbg_printf("DFSR  = 0x%08x\r\n", DFSR);
    dbg_printf("MMFAR = 0x%08x\r\n", MMFAR);
    dbg_printf("BFAR  = 0x%08x\r\n", BFAR);
    dbg_printf("AFSR  = 0x%08x\r\n", AFSR);
#else
    dbg_printf("CFSR_NS  = 0x%08x\r\n", CFSR );
    dbg_printf("HFSR_NS  = 0x%08x\r\n", HFSR);
    dbg_printf("DFSR_NS  = 0x%08x\r\n", DFSR);
    dbg_printf("MMFAR_NS = 0x%08x\r\n", MMFAR);
    dbg_printf("BFAR_NS  = 0x%08x\r\n", BFAR);
    dbg_printf("AFSR_NS  = 0x%08x\r\n", AFSR);
#endif
    ret = __get_PRIMASK();
    dbg_printf("PriMask = 0x%08x\r\n", ret);
//    ret = __get_BASEPRI();
//    dbg_printf("BasePri 0x%08x\r\n", ret);
    dbg_printf("SVC priority: 0x%02x\r\n", HAL_READ8(0xE000ED1F, 0));
    dbg_printf("PendSVC priority: 0x%02x\r\n", HAL_READ8(0xE000ED22, 0));
    dbg_printf("Systick priority: 0x%02x\r\n", HAL_READ8(0xE000ED23, 0));

    dbg_printf ("\r\nMSP Data:\r\n");
    src = (uint32_t)mstack;
    for(i = 0; i < 64; i+=4, src+=16) {
        if (*(u32 *)(src) == 0xDEADBEEF) {
            break;
        }
        dbg_printf("%08X:    %08X", src, *(u32 *)(src));
        dbg_printf("    %08X", *(u32 *)(src+4));
        dbg_printf("    %08X", *(u32 *)(src+8));
        dbg_printf("    %08X\r\n", *(u32 *)(src+12));
    }

    src = (uint32_t)pstack;
    if (src != 0) {
        dbg_printf ("\r\nPSP Data:\r\n");
        for(i = 0; i < 64; i+=4, src+=16) {
            dbg_printf("%08X:    %08X", src, *(u32 *)(src));
            dbg_printf("    %08X", *(u32 *)(src+4));
            dbg_printf("    %08X", *(u32 *)(src+8));
            dbg_printf("    %08X\r\n", *(u32 *)(src+12));
        }
    }

#if !defined (CONFIG_BUILD_NONSECURE)
#if  (__ARM_FEATURE_CMSE == 3U)
    if (((SCB->AIRCR & SCB_AIRCR_BFHFNMINS_Msk) == 0) || (fault_id == 4)) {
        // BusFault, HardFault & NMI handler are in secure state or in SecureFault
        uint32_t CFSR_NS, HFSR_NS, DFSR_NS, MMFAR_NS,BFAR_NS, AFSR_NS;
        uint32_t MSP_NS, PSP_NS;

        CFSR_NS  = SCB_NS->CFSR;
        HFSR_NS  = SCB_NS->HFSR;
        DFSR_NS  = SCB_NS->DFSR;
        MMFAR_NS = SCB_NS->MMFAR;
        BFAR_NS  = SCB_NS->BFAR;
        AFSR_NS  = SCB_NS->AFSR;

        MSP_NS = __TZ_get_MSP_NS();
        PSP_NS = __TZ_get_PSP_NS();
        dbg_printf("\r\n == NS Dump ==\r\n", CFSR_NS );
        dbg_printf("CFSR_NS  = 0x%08x\r\n", CFSR_NS );
        dbg_printf("HFSR_NS  = 0x%08x\r\n", HFSR_NS);
        dbg_printf("DFSR_NS  = 0x%08x\r\n", DFSR_NS);
        dbg_printf("MMFAR_NS = 0x%08x\r\n", MMFAR_NS);
        dbg_printf("BFAR_NS  = 0x%08x\r\n", BFAR_NS);
        dbg_printf("AFSR_NS  = 0x%08x\r\n", AFSR_NS);

        dbg_printf("MSP_NS   = 0x%08x\r\n", MSP_NS);
        dbg_printf("PSP_NS   = 0x%08x\r\n", PSP_NS);

        dbg_printf("NS HardFault Status Reg = 0x%08x\r\n", HFSR_NS);
        if (HFSR_NS & SCB_HFSR_FORCED_Msk) {
            // forced HardFault
            /* When this bit is set to 1, the HardFault handler must read the other
               fault status registers to find the cause of the fault. */
            dbg_printf("Forced HardFault\r\n");
            dbg_printf("SCB Configurable Fault Status Reg = 0x%08x\r\n", HFSR_NS);
        }

        if (HFSR_NS & SCB_HFSR_VECTTBL_Msk) {
            // Indicates a BusFault on a vector table read during exception processing
            /* When this bit is set to 1, the PC value stacked for the exception return points to
               the instruction that was preempted by the exception.*/
            dbg_printf("NS BusFault on vector table read\r\n");
        }

        parse_cfsr(CFSR_NS);

        if (MSP_NS != 0) {
            dbg_printf ("\r\nNS MSP Data:\r\n");
            src = (uint32_t)MSP_NS;
            for(i = 0; i < 64; i+=4, src+=16) {
                dbg_printf("%08X:    %08X", src, *(u32 *)(src));
                dbg_printf("    %08X", *(u32 *)(src+4));
                dbg_printf("    %08X", *(u32 *)(src+8));
                dbg_printf("    %08X\r\n", *(u32 *)(src+12));
            }
        }

        if (PSP_NS != 0) {
            dbg_printf ("\r\nNS PSP Data:\r\n");
            src = (uint32_t)PSP_NS;
            for(i = 0; i < 64; i+=4, src+=16) {
                dbg_printf("%08X:    %08X", src, *(u32 *)(src));
                dbg_printf("    %08X", *(u32 *)(src+4));
                dbg_printf("    %08X", *(u32 *)(src+8));
                dbg_printf("    %08X\r\n", *(u32 *)(src+12));
            }
        }
    }
#endif
    if ((lr_value & BIT6) == 0) {
        stack_trace_back((__TZ_get_MSP_NS()), (__TZ_get_PSP_NS()), lr_value, 1);
    } else {
        stack_trace_back((uint32_t)mstack, (uint32_t)pstack, lr_value, 0);
    }
#else
    stack_trace_back((uint32_t)mstack, (uint32_t)pstack, lr_value, 0);
#endif

    if(CoreDebug->DHCSR & 0x01) {
        // Debugger Connected
        __BKPT (0);     // Halt program here
    }
    while (1);
}

void app_section_init(void)
{
#if defined ( __ICCARM__ )
	__bss_start__               = (u8 *)__section_begin(".ram_image2.bss");
	__bss_end__                 = (u8 *)__section_end(".ram_image2.bss");
	__ram_nocache_start__       = (u8 *)__section_begin(".ram_image2.nocache.data");
	__ram_nocache_end__         = (u8 *)__section_end(".ram_image2.nocache.data");
	__ram_nocache_end__ = (u8 *)(((((u32)__ram_nocache_end__ - 1) >> 5) + 1) << 5); //32-byte aligned
#endif
}

u32 app_mpu_nocache_check(u32 mem_addr)
{
	mpu_region_config mpu_cfg;


	mpu_cfg.region_base = (uint32_t)__ram_nocache_start__;
	mpu_cfg.region_size = __ram_nocache_end__ - __ram_nocache_start__;

	if ((mem_addr >= mpu_cfg.region_base) && (mem_addr < (mpu_cfg.region_base + mpu_cfg.region_size))) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/*KM4 have 4 secure mpu entrys & 8 nonsecure mpu entrys*/
u32 app_mpu_nocache_init(void)
{
	mpu_region_config mpu_cfg;
	u32 mpu_entry = 0;

	mpu_cfg.region_base = (uint32_t)__ram_nocache_start__;
	mpu_cfg.region_size = __ram_nocache_end__ - __ram_nocache_start__;
	mpu_cfg.xn = MPU_EXEC_ALLOW;
	mpu_cfg.ap = MPU_UN_PRIV_RW;
	mpu_cfg.sh = MPU_NON_SHAREABLE;
	mpu_cfg.attr_idx = MPU_MEM_ATTR_IDX_NC;
	if (mpu_cfg.region_size >= 32) {
		mpu_entry = mpu_entry_alloc();
		mpu_region_cfg(mpu_entry, &mpu_cfg);
	}

	/* close rom_ns cache */
	mpu_entry = mpu_entry_alloc();
	mpu_cfg.region_base = 0x1E000;
	mpu_cfg.region_size = 0x45000 - 0x1E000;
	mpu_cfg.xn = MPU_EXEC_ALLOW;
	mpu_cfg.ap = MPU_UN_PRIV_RW;
	mpu_cfg.sh = MPU_NON_SHAREABLE;
	mpu_cfg.attr_idx = MPU_MEM_ATTR_IDX_NC;
	mpu_region_cfg(mpu_entry, &mpu_cfg);

	g_mpu_nregion_allocated = mpu_entry + 1;
	return 0;
}

void app_mpu_s_nocache_init(void)
{
#ifdef CONFIG_AMEBAD_TRUSTZONE
	mpu_s_no_cache_init();
#endif
}

#ifdef CONFIG_AMEBALITE_TRUSTZONE
void app_hardfualt_s_prehanlder(uint32_t fault_id)
{
	//write reboot reason, TrustZone watchdog
	BKUP_Write(BKUP_REG1, REBOOT_SYSTEM_TZWD_RESET);
}
#endif

#if defined (__GNUC__)
/* Add This for C++ support to avoid compile error */
void _init(void) {}
#endif

void app_fault_handler(uint32_t mstack[], uint32_t pstack[], uint32_t lr_value, uint32_t fault_id)
{
#if defined(CONFIG_EXAMPLE_CM_BACKTRACE) && CONFIG_EXAMPLE_CM_BACKTRACE
	UNUSED(fault_id);

	DBG_8195A("\r\nHard Fault Patch (Non-secure)\r\n");

	u8 IsPstack = 0;

	/* EXC_RETURN.S, 1: Registers have been pushed to a Secure stack
	                 0: Registers have been pushed to a Non-Secure stack */
	if (lr_value & BIT_EXCRETURN_S) {				//Taken from S
		DBG_8195A("\nException taken from Secure to Non-secure.\nSecure stack is used to store context."
				  "It can not be dumped from non-secure side for security reason!!!\n");

		while (1);
	} else {										//Taken from NS
		if (lr_value & BIT_EXCRETURN_MODE) {		//Thread Mode
			if (lr_value & BIT_EXCRETURN_SPSEL) {	//PSP
				IsPstack = 1;
			}
		}
	}

	cm_backtrace_fault(IsPstack ? (uint32_t)pstack : (uint32_t)mstack, lr_value);
	while (1);
#else
	UNUSED(mstack);
	UNUSED(pstack);
	UNUSED(lr_value);
	UNUSED(fault_id);
#endif
}

void INT_HardFault_Patch_C(uint32_t mstack[], uint32_t pstack[], uint32_t lr_value, uint32_t fault_id)
{
	u8 IsPstack = 0;

	DiagPrintf("\r\nHard Fault Patch (Non-secure)\r\n");

	/* EXC_RETURN.S, 1: original is Secure, 0: original is Non-secure */
	if (lr_value & BIT(6)) {					//Taken from S
		DiagPrintf("\nException taken from Secure to Non-secure.\nSecure stack is used to store context."
			"It can not be dumped from non-secure side for security reason!!!\n");

		while(1);
	} else {									//Taken from NS
		if (lr_value & BIT(3)) {				//Thread mode
			if (lr_value & BIT(2)) {			//PSP
				IsPstack = 1;
			}
		}
	}

#if defined(CONFIG_EXAMPLE_CM_BACKTRACE) && CONFIG_EXAMPLE_CM_BACKTRACE
	cm_backtrace_fault(IsPstack ? pstack : mstack, lr_value);
	while(1);
#else

	if(IsPstack)
		mstack = pstack;

	INT_HardFault_C(mstack, pstack, lr_value, fault_id);
#endif

}

VOID
INT_HardFault_Patch(void)
{
	__ASM volatile(
		"MRS R0, MSP\n\t"
		"MRS R1, PSP\n\t"
		"MOV R2, LR\n\t" /* second parameter is LR current value */
		"MOV R3, #0\n\t"
		"SUB.W	R4, R0, #128\n\t"
		"MSR MSP, R4\n\t"   // Move MSP to upper to for we can dump current stack contents without chage contents
		"LDR R4,=INT_HardFault_Patch_C\n\t"
		"BX R4\n\t"
	);
}

VOID
INT_UsageFault_Patch(void)
{
	__ASM volatile(
		"MRS R0, MSP\n\t"
		"MRS R1, PSP\n\t"
		"MOV R2, LR\n\t" /* second parameter is LR current value */
		"MOV R3, #1\n\t"
		"SUB.W	R4, R0, #128\n\t"
		"MSR MSP, R4\n\t"   // Move MSP to upper to for we can dump current stack contents without chage contents
		"LDR R4,=INT_HardFault_Patch_C\n\t"
		"BX R4\n\t"
	);
}

VOID
INT_BusFault_Patch(void)
{
	__ASM volatile(
		"MRS R0, MSP\n\t"
		"MRS R1, PSP\n\t"
		"MOV R2, LR\n\t" /* second parameter is LR current value */
		"MOV R3, #2\n\t"
		"SUB.W	R4, R0, #128\n\t"
		"MSR MSP, R4\n\t"   // Move MSP to upper to for we can dump current stack contents without chage contents
		"LDR R4,=INT_HardFault_Patch_C\n\t"
		"BX R4\n\t"
	);
}

VOID
INT_MemFault_Patch(void)
{
	__ASM volatile(
		"MRS R0, MSP\n\t"
		"MRS R1, PSP\n\t"
		"MOV R2, LR\n\t" /* second parameter is LR current value */
		"MOV R3, #3\n\t"
		"SUB.W	R4, R0, #128\n\t"
		"MSR MSP, R4\n\t"   // Move MSP to upper to for we can dump current stack contents without chage contents
		"LDR R4,=INT_HardFault_Patch_C\n\t"
		"BX R4\n\t"
	);
}

#ifdef CONFIG_PLATFORM_TIZENRT_OS
void exception_common(void);
#endif
VOID VectorTableOverride(VOID)
{
#if 1
	NewVectorTable[3] = (HAL_VECTOR_FUN)INT_HardFault_Patch;
	NewVectorTable[4] = (HAL_VECTOR_FUN)INT_MemFault_Patch;
	NewVectorTable[5] = (HAL_VECTOR_FUN)INT_BusFault_Patch;
	NewVectorTable[6] = (HAL_VECTOR_FUN)INT_UsageFault_Patch;
#endif
#ifdef CONFIG_PLATFORM_TIZENRT_OS
	int i;
	for(i=3;i<MAX_VECTOR_TABLE_NUM;i++)
		NewVectorTable[i] = exception_common;
	//NewVectorTable[AMEBALITE_IRQ_HARDFAULT] = (HAL_VECTOR_FUN)HardFault_Handler_ram;
	//NewVectorTable[AMEBALITE_IRQ_MEMFAULT] = (HAL_VECTOR_FUN)MemManage_Handler_ram;
	//NewVectorTable[AMEBALITE_IRQ_BUSFAULT] = (HAL_VECTOR_FUN)BusFault_Handler_ram;
	//NewVectorTable[AMEBALITE_IRQ_USAGEFAULT] = (HAL_VECTOR_FUN)UsageFault_Handler_ram;
	NewVectorTable[7] = (HAL_VECTOR_FUN)SecureFault_Handler_ram;
	SCB->VTOR = (uint32_t)NewVectorTable;
#endif
}

void app_psram_suspend(void)
{
	RRAM_TypeDef *rram = RRAM_DEV;

	rram->PSRAM_TYPE = ChipInfo_PSRAMType() >> 4;
}

u32 app_psram_resume(u32 expected_idle_time, void *param)
{
	/* To avoid gcc warnings */
	(void) expected_idle_time;
	(void) param;

	u32 temp;

	/*dummy read to make psram exit half sleep mode*/
	temp = HAL_READ32(PSRAM_BASE, 0);

	/*wait psram exit half sleep mode*/
	DelayUs(100);
	DCache_Invalidate((u32)PSRAM_BASE, 32);

	/*Try use temp to avoid warning */
	if (temp != 0) {
		return TRUE;
	} else {
		return FALSE;
	}
}

// The Main App entry point
void app_start(void)
{
	/* enable non-secure cache */
	Cache_Enable(ENABLE);

	irq_table_init(MSP_RAM_HP_NS); /* NS Vector table init */
	VectorTableOverride();
	DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "VTOR: %x, VTOR_NS:%x\n", SCB->VTOR, SCB_NS->VTOR);

	FaultPatch_register(app_fault_handler);

	/* lp platform has three master(KR4, OTPC, SIC) and two master port
	  * OTPC and KR4 share one master port, and SIC occupy another master port
	  * OTPC is selected by default and then KR4 is selected after OTPC autoload finished.
	  * KR4 boot is not conflict with SIC because they do not share with a common port.
	*/
	if (SYSCFG_TRP_TestMode()) {
		if (SYSCFG_TRP_OTPBYP()) {
			DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "Bypass OTP autoload\r\n");
		} else {
			DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "In Test mode: 0x%x\r\n", SYSCFG_TRP_ICFG());
		}
	}

	DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "KM4 APP START \n");

	cmse_address_info_t cmse_address_info = cmse_TT((void *)app_start);
	DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "IMG2 SECURE STATE: %d\n", cmse_address_info.flags.secure);

	app_section_init();
	_memset((void *) __bss_start__, 0, (__bss_end__ - __bss_start__));

#ifdef CONFIG_AMEBALITE_TRUSTZONE
	BOOT_IMG3();
#endif
	SystemCoreClockUpdate();
	DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "KM4 CPU CLK: %d Hz \n", SystemCoreClock);

	if (SYSCFG_CHIPType_Get() != CHIP_TYPE_FPGA) {

		if (((BOOT_Reason()) & AON_BIT_RSTF_DSLP) == FALSE) {

			OSC131K_Calibration(30000); /* PPM=30000=3% *//* 7.5ms */
		}
		OSC4M_Init();
		OSC4M_Calibration(30000);
		XTAL_PDCK();
	}

	/* configure FreeRTOS interrupt and heap region */
	os_heap_init();
#ifndef CONFIG_PLATFORM_TIZENRT_OS
	__NVIC_SetVector(SVCall_IRQn, (u32)(VOID *)vPortSVCHandler);
	__NVIC_SetVector(PendSV_IRQn, (u32)(VOID *)xPortPendSVHandler);
	__NVIC_SetVector(SysTick_IRQn, (u32)(VOID *)xPortSysTickHandler);

#if defined (__GNUC__)
	extern void __libc_init_array(void);
	/* Add This for C++ support */
	__libc_init_array();
#endif
#endif
	// force SP align to 8 byte not 4 byte (initial SP is 4 byte align)
	__asm(
		"mov r0, sp\n"
		"bic r0, r0, #7\n"
		"mov sp, r0\n"
	);

	mpu_init();
	app_mpu_nocache_init();
	
	flash_layout_init();
#ifndef CONFIG_XIP_FLASH
	app_psram_suspend();
#endif

#ifdef CONFIG_AMEBALITE_TRUSTZONE
	Secure_VectorTableOverride(app_hardfualt_s_prehanlder);
#endif

#ifdef CONFIG_PLATFORM_TIZENRT_OS
#ifdef CONFIG_ARMV8M_MPU
	/* Initialize number of mpu regions for board specific purpose */
	mpu_set_nregion_board_specific(g_mpu_nregion_allocated);

	up_mpuinitialize();
#endif

#ifdef CONFIG_STACK_COLORATION
	/* Set the IDLE stack to the coloration value and jump into os_start() */

	go_os_start((FAR void *)g_idle_topstack - CONFIG_IDLETHREAD_STACKSIZE, CONFIG_IDLETHREAD_STACKSIZE);
#else
	/* Call os_start() */

	os_start();

	/* Shoulnd't get here */

	for (;;) ;
#endif
#else
	main(); /* project/xxxx/src/main.c */
#endif
}

IMAGE2_VALID_PATTEN_SECTION
const u8 RAM_IMG2_VALID_PATTEN[20] = {
	'R', 'T', 'K', 'W', 'i', 'n', 0x0, 0xff,
	(FW_VERSION & 0xff), ((FW_VERSION >> 8) & 0xff),
	(FW_SUBVERSION & 0xff), ((FW_SUBVERSION >> 8) & 0xff),
	(FW_CHIP_ID & 0xff), ((FW_CHIP_ID >> 8) & 0xff),
	(FW_CHIP_VER),
	(FW_BUS_TYPE),
	(FW_INFO_RSV1),
	(FW_INFO_RSV2),
	(FW_INFO_RSV3),
	(FW_INFO_RSV4)
};
extern VOID SOCPS_WakeFromPG_KM4(VOID);
IMAGE2_ENTRY_SECTION
RAM_START_FUNCTION Img2EntryFun0 = {
	app_start,
	SOCPS_WakeFromPG_KM4,
	(u32)NewVectorTable
};
