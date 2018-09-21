/****************************************************************************
 *
 * Copyright 2018, Cypress Semiconductor Corporation or a subsidiary of
 * Cypress Semiconductor Corporation. All Rights Reserved.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

	.file	"bcm4390x_vectortab.S"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define VENEER_NAME_THUMB2(name) veneer_thumb2_##name
#define VENEER_NAME_ARM(name)    veneer_arm_##name

#if defined(__thumb__) || defined(CONFIG_BCM4390X_USE_EXTMEM)
#define VECTOR_VENEER_NEEDED
#endif

#ifdef VECTOR_VENEER_NEEDED
#define VECTOR_NAME(name)        VENEER_NAME_THUMB2(name)
#endif

/****************************************************************************
 * Public Symbols
 ****************************************************************************/

	.globl		_vector_start

/****************************************************************************
 * Assembly Macros
 ****************************************************************************/

/****************************************************************************
 * Name: _vector_start
 *
 * Description:
 *   Vector initialization block
 ****************************************************************************/

	.section	.vectors, "ax"
	.globl		_vector_start

/* These will be relocated to VECTOR_BASE. */

_vector_start:
#ifdef VECTOR_VENEER_NEEDED
    .arm
    B       VECTOR_NAME(__start)               /* 0x00 - entry point           */
    B       VECTOR_NAME(arm_vectorundefinsn)   /* 0x04 - undefined instruction */
    B       VECTOR_NAME(arm_vectorsvc)         /* 0x08 - software interrupt    */
    B       VECTOR_NAME(arm_vectorprefetch)    /* 0x0c - prefetch abort        */
    B       VECTOR_NAME(arm_vectordata)        /* 0x10 - data abort            */
    B       VECTOR_NAME(arm_vectoraddrexcptn)  /* 0x14 - exception handler     */
    B       VECTOR_NAME(arm_vectorirq)         /* 0x18 - external interrupt    */
    B       VECTOR_NAME(arm_vectorfiq)         /* 0x1c - fast interrupt        */
#else
	B		__start
	B		arm_vectorundefinsn
	B		arm_vectorsvc
	B		arm_vectorprefetch
	B		arm_vectordata
	B		arm_vectoraddrexcptn
	B		arm_vectorirq
	B		arm_vectorfiq
#endif

#ifdef VECTOR_VENEER_NEEDED
    .text
    .section .text.sram, "ax"
    .align 2
    .arm
VECTOR_NAME(__start):
    LDR        pc, =__start
VECTOR_NAME(arm_vectorundefinsn):
    LDR        pc, =arm_vectorundefinsn
VECTOR_NAME(arm_vectorsvc):
    LDR        pc, =arm_vectorsvc
VECTOR_NAME(arm_vectorprefetch):
    LDR        pc, =arm_vectorprefetch
VECTOR_NAME(arm_vectordata):
    LDR        pc, =arm_vectordata
VECTOR_NAME(arm_vectoraddrexcptn):
    LDR        pc, =arm_vectoraddrexcptn
VECTOR_NAME(arm_vectorirq):
    LDR        pc, =arm_vectorirq
VECTOR_NAME(arm_vectorfiq):
    LDR        pc, =arm_vectorfiq
#endif /* VECTOR_VENEER_NEEDED */

	.end
