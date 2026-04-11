/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * arch/arm/src/qemu/qemu_boot.c
 *
 * SPDX-License-Identifier: Apache-2.0
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include "up_internal.h"

#ifdef CONFIG_ARM_PSCI
#  include "psci.h"
#endif

#include "qemu_irq.h"
#include "qemu_memorymap.h"
#include "smp.h"
#include "gic.h"
#include "scu.h"

#ifdef CONFIG_DEVICE_TREE
#  include <tinyara/fdt.h>
#endif

#ifdef CONFIG_SCHED_INSTRUMENTATION
#  include <sched/sched.h>
#  include <tinyara/sched_note.h>
#endif

#ifdef CONFIG_ARCH_ARMV7R
#  include <tinyara/init.h>
#endif

#ifdef CONFIG_SYSLOG_RPMSG
#include <tinyara/syslog/syslog_rpmsg.h>
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

#ifdef CONFIG_SYSLOG_RPMSG
static char g_syslog_rpmsg_buf[4096];
#endif

#if defined(CONFIG_SMP) && !defined(CONFIG_ARM_PSCI)
#  error "qemu-virt SMP boot requires CONFIG_ARM_PSCI"
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: arm_boot
 *
 * Description:
 *   Complete boot operations started in arm_head.S
 *
 ****************************************************************************/

void arm_boot(void)
{
#ifdef CONFIG_QEMU_VIRT_PROFILE_RTL8730E
  /* qemu-virt models the rtl8730e execution profile as an A-profile, MMU,
   * PSCI-backed SMP platform. Keep the boot path explicit so later qemu-virt
   * configs can diverge without silently changing this profile.
   */
#endif

#ifdef CONFIG_ARCH_PERF_EVENTS
  /* Perf init */

  up_perf_init(0);
#endif

#ifdef CONFIG_ARCH_ARMV7A_FAMILY
  /* Set the page table for section */

  qemu_setupmappings();
#endif

#ifdef CONFIG_SMP
  /* Enable SMP cache coherency for CPU0 */

  arm_enable_smp(0);
#endif

  arm_fpuconfig();

#ifdef CONFIG_ARM_PSCI
  arm_psci_init("hvc");
#endif

#ifdef CONFIG_DEVICE_TREE
  //TODO: Add Flatten Device Tree support.
  fdt_register((const char *)0x40000000);
#endif

#ifdef USE_EARLYSERIALINIT
  /* Perform early serial initialization if we are going to use the serial
   * driver.
   */
  /* Serial initialization will be done in up_initialize() */
#endif

#ifdef CONFIG_SMP
  smp_init();
#endif
}

#if defined(CONFIG_SMP)
static unsigned long cpu_start[CONFIG_SMP_NCPUS] = {
  0,
#if CONFIG_SMP_NCPUS > 1
  (unsigned long)__cpu1_start,
#endif
#if CONFIG_SMP_NCPUS > 2
  (unsigned long)__cpu2_start,
#endif
#if CONFIG_SMP_NCPUS > 3
  (unsigned long)__cpu3_start,
#endif
};
#endif

#if defined(CONFIG_ARM_PSCI) && defined(CONFIG_SMP)
void smp_init(void)
{
	long xCoreID;
	int err;

#if ( CONFIG_SMP_NCPUS > 1 )
	for (xCoreID = 0; xCoreID < CONFIG_SMP_NCPUS; xCoreID++) {
		if (xCoreID == up_cpu_index()) {
			continue;
		}
		err = psci_cpu_on(xCoreID, cpu_start[xCoreID]);

		DEBUGASSERT(err >= 0);
	}
#endif
}
#endif
