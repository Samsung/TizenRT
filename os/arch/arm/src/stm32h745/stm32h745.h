/************************************************************************************
 * arch/arm/src/stm32h745/stm32h745.h
 *
 *   Copyright (C) 2015 Sebastien Lorquet. All rights reserved.
 *   Author: Sebastien Lorquet <sebastien@lorquet.fr>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ************************************************************************************/

#ifndef __ARCH_ARM_SRC_STM32H745_H
#define __ARCH_ARM_SRC_STM32H745_H

extern int  stm32h745_shared_memory_init(uint32_t hsem_id);
extern int  stm32h745_haltick_init(void);
extern void __ramfunc__ stm32h745_wwdginitialize(FAR const char *devpath);
extern void stm32h745_tim4_init(void);
extern int  stm32h745_switch_boot_address(uint32_t address);
extern void stm32h745_irq_clear_pending_all(void);

/* Definition of HSEM ID */
#define HSEM_ID_0  (0U)
#define HSEM_ID_1  (1U)
#define HSEM_ID_2  (2U)
#define HSEM_ID_3  (3U)
#define HSEM_ID_4  (4U)
#define HSEM_ID_5  (5U)
#define HSEM_ID_6  (6U)
#define HSEM_ID_7  (7U)
#define HSEM_ID_8  (8U)
#define HSEM_ID_9  (9U)
#define HSEM_ID_10 (10U)
#define HSEM_ID_11 (11U)
#define HSEM_ID_12 (12U)
#define HSEM_ID_13 (13U)
#define HSEM_ID_14 (14U)
#define HSEM_ID_15 (15U)
#define HSEM_ID_16 (16U)
#define HSEM_ID_17 (17U)
#define HSEM_ID_18 (18U)
#define HSEM_ID_19 (19U)
#define HSEM_ID_20 (20U)
#define HSEM_ID_21 (21U)
#define HSEM_ID_22 (22U)
#define HSEM_ID_23 (23U)
#define HSEM_ID_24 (24U)
#define HSEM_ID_25 (25U)
#define HSEM_ID_26 (26U)
#define HSEM_ID_27 (27U)
#define HSEM_ID_28 (28U)
#define HSEM_ID_29 (29U)
#define HSEM_ID_30 (30U)
#define HSEM_ID_31 (31U)
#define HSEM_ID_MAX HSEM_ID_31

#endif /* __ARCH_ARM_SRC_STM32H745_H */



