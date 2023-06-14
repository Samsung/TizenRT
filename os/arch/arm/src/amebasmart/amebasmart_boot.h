/****************************************************************************
 * arch/arm/src/amebasmart/amebasmart_boot.h
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

#ifndef __ARCH_ARM_SRC_AMEBASMART_AMEBASMART_BOOT_H
#define __ARCH_ARM_SRC_AMEBASMART_AMEBASMART_BOOT_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/compiler.h>

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>

#include "arm_internal.h"
#include "chip.h"

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifndef __ASSEMBLY__

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Name: amebasmart_cpu_disable
 *
 * Description:
 *   Called from CPU0 to make sure that all other CPUs are in the disabled
 *   state.  This is a formality because the other CPUs are actually running
 *   then we have probably already crashed.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

#ifdef CONFIG_SMP
void amebasmart_cpu_disable(void);
#else
#  define amebasmart_cpu_disable()
#endif

/****************************************************************************
 * Name: amebasmart_cpu_enable
 *
 * Description:
 *   Called from CPU0 to enable all other CPUs.  The enabled CPUs will start
 *   execution at __cpuN_start and, after very low-level CPU initialization
 *   has been performed, will branch to arm_cpu_boot()
 *   (see arch/arm/src/armv7-a/smp.h)
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

#ifdef CONFIG_SMP
void amebasmart_cpu_enable(void);
#else
#  define amebasmart_cpu_enable()
#endif

/****************************************************************************
 * Name: amebasmart_memory_initialize
 *
 * Description:
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void amebasmart_memory_initialize(void);

/****************************************************************************
 * Name: amebasmart_board_initialize
 *
 * Description:
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

// void amebasmart_board_initialize(void);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __ARCH_ARM_SRC_AMEBASMART_AMEBASMART_BOOT_H */
