;/*
; * Copyright (c) 2009-2021 ARM Limited
; *
; * Licensed under the Apache License, Version 2.0 (the "License");
; * you may not use this file except in compliance with the License.
; * You may obtain a copy of the License at
; *
; *     http://www.apache.org/licenses/LICENSE-2.0
; *
; * Unless required by applicable law or agreed to in writing, software
; * distributed under the License is distributed on an "AS IS" BASIS,
; * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; * See the License for the specific language governing permissions and
; * limitations under the License.
; *
; *
; * This file is derivative of CMSIS V5.00 startup_ARMCM33.S
; */

#include "tfm_plat_config.h"
#include "sdkconfig.h"

    .syntax    unified
    .arch      armv8-m.main
    .globl     bfhfnmi_handler
    .globl     NMI_Handler
    .globl     HardFault_Handler
    .globl     MemManage_Handler
    .globl     BusFault_Handler
    .globl     UsageFault_Handler
    .globl     SecureFault_Handler
    .globl     UART1_Handler
    .globl     GPIO_Handler

    .section .vectors, "a"
    .align 2
    .globl    __Vectors
__Vectors:
    .long    Image$$ARM_LIB_STACK$$ZI$$Limit   /* Top of Stack */

    /* Core interrupts */
    .long    Reset_Handler                  /* Reset Handler */
    .long    NMI_Handler                    /* NMI Handler */
    .long    HardFault_Handler              /* Hard Fault Handler */
    .long    MemManage_Handler              /* MPU Fault Handler */
    .long    BusFault_Handler               /* Bus Fault Handler */
    .long    UsageFault_Handler             /* Usage Fault Handler */
    .long    SecureFault_Handler            /* Secure Fault Handler */
    .long    0                              /* Reserved */
    .long    0                              /* Reserved */
    .long    0                              /* Reserved */
    .long    SVC_Handler                    /* SVCall Handler */
    .long    DebugMon_Handler               /* Debug Monitor Handler */
    .long    0                              /* Reserved */
    .long    PendSV_Handler                 /* PendSV Handler */
    .long    SysTick_Handler                /* SysTick Handler */

    /* External interrupts */
    .long    DMA_NS_Handler                 /* 0: DMA NS Handler */
    .long    ENCP_S_Handler                 /* 1: Encrypt enginee S Handler */
    .long    ENCP_NS_Handler                /* 2: Encrypt enginee NS Handler */
    .long    TFM_TIMER0_Handler             /* 3: TFM Timer Handler */
    .long    UART0_Handler                  /* 4: UART 0 Handler */
    .long    0                              /* 5 */
    .long    0                              /* 6 */
    .long    0                              /* 7 */
    .long    0                              /* 8 */
    .long    0                              /* 9 */
    .long    0                              /* 10 */
    .long    0                              /* 11 */
    .long    0                              /* 12 */
    .long    0                              /* 13 */
    .long    0                              /* 14 */
    .long    UART1_Handler                  /* 15 */
    .rept    21                             /* 16~36 */
    .long    0
    .endr
    .long    GPIO_Handler                   /* 37 */
#if CONFIG_TFM_TAKE_FOR_BL2
    .rept    10                             /* bootrom will check the magic word:asc ii:bk7236*/
    .long    0
    .endr
    .long    0x32374b42
    .long    0x00003633
#endif
    .long    0
    .long    0
    .long    0
    .long    0
    .long    0
    .long    GPIO_Handler

    .size    __Vectors, . - __Vectors

    .section .fix.reset_entry,"ax"
    .thumb
    .thumb_func
    .align    2
    .globl   Reset_Handler
    .type    Reset_Handler, %function
Reset_Handler:

set_msp_limit:
    ldr     r0, =Image$$ARM_LIB_STACK$$ZI$$Base
    MSR     msplim, r0

/*  Firstly it copies data from read only memory to RAM. There are two schemes
 *  to copy. One can copy more than one sections. Another can only copy
 *  one section.  The former scheme needs more instructions and read-only
 *  data to implement than the latter.
 *  Macro __STARTUP_COPY_MULTIPLE is used to choose between two schemes.  */

#ifdef __STARTUP_COPY_MULTIPLE
/*  Multiple sections scheme.
 *
 *  Between symbol address __copy_table_start__ and __copy_table_end__,
 *  there are array of triplets, each of which specify:
 *    offset 0: LMA of start of a section to copy from
 *    offset 4: VMA of start of a section to copy to
 *    offset 8: size of the section to copy. Must be multiply of 4
 *
 *  All addresses must be aligned to 4 bytes boundary.
 */
    ldr    r4, =__copy_table_start__
    ldr    r5, =__copy_table_end__

.L_loop0:
    cmp    r4, r5
    bge    .L_loop0_done
    ldr    r1, [r4]
    ldr    r2, [r4, #4]
    ldr    r3, [r4, #8]

.L_loop0_0:
    subs    r3, #4
    ittt    ge
    ldrge    r0, [r1, r3]
    strge    r0, [r2, r3]
    bge    .L_loop0_0

    adds    r4, #12
    b    .L_loop0

.L_loop0_done:
#else
/*  Single section scheme.
 *
 *  The ranges of copy from/to are specified by following symbols
 *    __etext: LMA of start of the section to copy from. Usually end of text
 *    __data_start__: VMA of start of the section to copy to
 *    __data_end__: VMA of end of the section to copy to
 *
 *  All addresses must be aligned to 4 bytes boundary.
 */
    ldr    r1, =__etext
    ldr    r2, =__data_start__
    ldr    r3, =__data_end__

.L_loop1:
    cmp    r2, r3
    ittt    lt
    ldrlt    r0, [r1], #4
    strlt    r0, [r2], #4
    blt    .L_loop1
#endif /*__STARTUP_COPY_MULTIPLE */

/*  This part of work usually is done in C library startup code. Otherwise,
 *  define this macro to enable it in this startup.
 *
 *  There are two schemes too. One can clear multiple BSS sections. Another
 *  can only clear one section. The former is more size expensive than the
 *  latter.
 *
 *  Define macro __STARTUP_CLEAR_BSS_MULTIPLE to choose the former.
 *  Otherwise efine macro __STARTUP_CLEAR_BSS to choose the later.
 */
#ifdef __STARTUP_CLEAR_BSS_MULTIPLE
/*  Multiple sections scheme.
 *
 *  Between symbol address __copy_table_start__ and __copy_table_end__,
 *  there are array of tuples specifying:
 *    offset 0: Start of a BSS section
 *    offset 4: Size of this BSS section. Must be multiply of 4
 */
    ldr    r3, =__zero_table_start__
    ldr    r4, =__zero_table_end__

.L_loop2:
    cmp    r3, r4
    bge    .L_loop2_done
    ldr    r1, [r3]
    ldr    r2, [r3, #4]
    movs    r0, 0

.L_loop2_0:
    subs    r2, #4
    itt    ge
    strge    r0, [r1, r2]
    bge    .L_loop2_0

    adds    r3, #8
    b    .L_loop2
.L_loop2_done:
#elif defined (__STARTUP_CLEAR_BSS)
/*  Single BSS section scheme.
 *
 *  The BSS section is specified by following symbols
 *    __bss_start__: start of the BSS section.
 *    __bss_end__: end of the BSS section.
 *
 *  Both addresses must be aligned to 4 bytes boundary.
 */
    ldr    r1, =__bss_start__
    ldr    r2, =__bss_end__

    movs    r0, 0
.L_loop3:
    cmp    r1, r2
    itt    lt
    strlt    r0, [r1], #4
    blt    .L_loop3
#endif /* __STARTUP_CLEAR_BSS_MULTIPLE || __STARTUP_CLEAR_BSS */

    cpsid   i              /* Disable IRQs */
    bl    SystemInit

#ifndef __START
#define __START _start
#endif
    bl    __START

    .pool
    .size    Reset_Handler, . - Reset_Handler


/*  Macro to define default handlers. */
    .macro    def_irq_handler    handler_name
    .align    1
    .thumb_func
    .weak    \handler_name
    \handler_name:
    bl       bfhfnmi_handler
    b        \handler_name
    .endm

    /* Core interrupts */
    def_irq_handler      SVC_Handler
    def_irq_handler      DebugMon_Handler
    def_irq_handler      PendSV_Handler
    def_irq_handler      SysTick_Handler

    /* External interrupts */
    def_irq_handler     DMA_NS_Handler                /* 0: DMA NS Handler */
    def_irq_handler     ENCP_S_Handler                 /* 1: Encrypt enginee S Handler */
    def_irq_handler     ENCP_NS_Handler                /* 2: Encrypt enginee NS Handler */
    def_irq_handler     TFM_TIMER0_Handler             /* 3: TFM Timer Handler */
    def_irq_handler     UART0_Handler                  /* 15: TIMER 0 Handler */

    .end
