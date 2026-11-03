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
 * arch/arm/src/amebasmart/amebasmart_smp.h
 ****************************************************************************/

#ifndef __ARCH_ARM_SRC_AMEBASMART_AMEBASMART_SMP_H
#define __ARCH_ARM_SRC_AMEBASMART_AMEBASMART_SMP_H

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifndef __ASSEMBLY__

/* CPU1 power and PSCI start of the Realtek code (amebasmart_smp.c) */

int cpu_core_poweron(int cpu);
int vPortSecondaryBoot(int cpu);

#endif /* __ASSEMBLY__ */
#endif /* __ARCH_ARM_SRC_AMEBASMART_AMEBASMART_SMP_H */
