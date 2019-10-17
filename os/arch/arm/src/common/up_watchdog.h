/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

#ifndef __ARCH_ARM_SRC_ARM_WATCHDOG_H
#define __ARCH_ARM_SRC_ARM_WATCHDOG_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Name: up_watchdog_disable
 *
 * Description:
 *   Disable board specific watchdog
 *
 * Cautions:
 *   This can be only used if we cannot use driver structure like assert.
 *
 ****************************************************************************/
void up_watchdog_disable(void);

#endif							/* __ARCH_ARM_SRC_ARM_WATCHDOG_H */
