// Copyright 2020-2021 Beken
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

#include "boot.h"
#include "boot_asm_macro.h"
#include "arm.h"

.equ	ARM_PSR_THUMB,		0x20
.equ	ARM_PSR_INT_DIS,    0xC0
.equ	ARM_PSR_FIQ_DIS,    0x40
.equ	ARM_PSR_IRQ_DIS,    0x80
.equ	ARM_PSR_MODE_MASK,	0x1F
.equ	ARM_PSR_MODE_USR,	0x10
.equ	ARM_PSR_MODE_FIQ,	0x11
.equ	ARM_PSR_MODE_IRQ,	0x12
.equ	ARM_PSR_MODE_SVC,	0x13
.equ	ARM_PSR_MODE_ABT,	0x17
.equ	ARM_PSR_MODE_UND,	0x1B
.equ	ARM_PSR_MODE_SYS,	0x1F

.globl boot_swi
.globl do_swi
.globl intc_irq
.globl intc_fiq
.globl irq_handler
.globl fiq_handler
.globl do_irq
.globl do_fiq

.align	5
do_swi:
	LDMFD   SP!, {R0-R1}

.align	5
boot_swi:
	STMFD  sp!,{r0-r1}
	LDR    R1, =0x400008
	LDR    r0, [R1]
	BX     r0

.align	5
irq_handler:
	STMFD  sp!,{r0-r1}
	LDR    R1, =0x400000
	LDR    r0, [R1]
	BX     r0

.align	5
fiq_handler:
	STMFD  sp!,{r0-r1}
	LDR    R1, =0x400004
	LDR    r0, [R1]
	BX     r0

.align	5
do_irq:
	LDMFD   SP!, {R0-R1}
	STMDB sp!, {r0-r12, lr}
	LDR R0, =intc_irq
	MOV LR, PC
	BX R0
	LDMIA sp!, {r0-r12, lr}
	subs pc, lr, #0x04

.align	5
do_fiq:
	LDMFD   SP!, {R0-R1}
	STMDB sp!, {r0-r12, lr}
	LDR R0, =intc_fiq
	MOV LR, PC
	BX R0
	LDMIA sp!, {r0-r12, lr}
	subs pc, lr, #0x04


.globl arch_enable_int
.type arch_enable_int, %function
arch_enable_int:
	MRS r0, cpsr
	BIC r0, r0, #0x80
	MSR cpsr_c, r0
	BX LR

