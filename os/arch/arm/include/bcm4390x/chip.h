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

#ifndef __ARCH_ARM_INCLUDE_BCM4390X_CHIP_H
#define __ARCH_ARM_INCLUDE_BCM4390X_CHIP_H

/*****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#ifndef __ASSEMBLY__

#include <stdint.h>

#endif
/****************************************************************************************
 *  * Public Types
 *****************************************************************************************/

#ifndef __ASSEMBLY__

/****************************************************************************************
 *  * Public Data
 ****************************************************************************************/

#define EXTERN extern
#ifdef __cplusplus
#undef EXTERN
#define EXTERN extern "C"
extern "C" {
#endif

/****************************************************************************************
 *  * Public Function Prototypes
 *****************************************************************************************/

EXTERN uint32_t clock_systimer_proxy(void);
EXTERN void clock_systimer_set(uint32_t ticks);

EXTERN int pwm_devinit(void);

EXTERN void host_platform_bus_buffer_freed_nuttx(int tx_pool);

EXTERN int bcm4390x_audio_initialize(void);

#ifdef CONFIG_AUDIO_LOOPBACK_TEST
EXTERN int bcm4390x_audio_loopback_initialize(void);
#endif							/* CONFIG_AUDIO_LOOPBACK_TEST */

#undef EXTERN
#ifdef __cplusplus
}
#endif
#endif
#endif							/* __ARCH_ARM_INCLUDE_BCM4390X_CHIP_H */
