/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * arch/arm/src/amebasmart/arm_cpupause.h
 *
 * Pause and resume the secondary CPUs while the XIP flash is programmed.
 * See arm_cpupause.c for the details.
 *
 ****************************************************************************/

#ifndef __ARCH_ARM_SRC_AMEBASMART_ARM_CPUPAUSE_H
#define __ARCH_ARM_SRC_AMEBASMART_ARM_CPUPAUSE_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdbool.h>

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef CONFIG_SMP

#ifdef __cplusplus
extern "C"
{
#endif

bool up_cpu_pausereq(int cpu);
bool up_is_cpu_paused(int cpu);
int up_cpu_paused(int cpu);
int up_cpu_pause(int cpu);
int up_cpu_resume(int cpu);
void up_cpu_pause_all(void);
void up_cpu_resume_all(void);
void amebasmart_cpupause_initialize(void);

#ifdef __cplusplus
}
#endif

#endif /* CONFIG_SMP */
#endif /* __ARCH_ARM_SRC_AMEBASMART_ARM_CPUPAUSE_H */
