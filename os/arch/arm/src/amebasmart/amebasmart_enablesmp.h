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
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * arch/arm/src/amebasmart/amebasmart_enablesmp.h
 ****************************************************************************/

#ifndef __ARCH_ARM_SRC_AMEBASMART_AMEBASMART_ENABLESMP_H
#define __ARCH_ARM_SRC_AMEBASMART_AMEBASMART_ENABLESMP_H

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifndef __ASSEMBLY__

/****************************************************************************
 * Name: amebasmart_enable_smp
 *
 * Description:
 *   Make the current CPU take part in the SMP cache coherency (the NuttX
 *   arm_enable_smp() without the SCU accesses, which the Cortex-A32 does
 *   not have).
 *
 ****************************************************************************/

#ifdef CONFIG_SMP
void amebasmart_enable_smp(int cpu);
#endif

#endif /* __ASSEMBLY__ */
#endif /* __ARCH_ARM_SRC_AMEBASMART_AMEBASMART_ENABLESMP_H */
