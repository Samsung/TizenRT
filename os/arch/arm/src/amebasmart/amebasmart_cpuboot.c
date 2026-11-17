/****************************************************************************
 * arch/arm/src/amebasmart/amebasmart_cpuboot.c
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

#include <stdint.h>
#include <assert.h>
#include <debug.h>

#include <tinyara/arch.h>
#include <tinyara/sched.h>
#ifdef CONFIG_SCHED_INSTRUMENTATION
#  include <tinyara/sched_note.h>
#endif
#include <arch/irq.h>

#include "init/init.h"
#include "sched/sched.h"

#include "arm_internal.h"
#include "sctlr.h"
#include "smp.h"
#include "scu.h"
#include "gic.h"
#include "amebasmart_enablesmp.h"
#include "mmu.h"

#ifdef CONFIG_SMP

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: arm_cpu_boot
 *
 * Description:
 *   Continues the C-level initialization started by the assembly language
 *   __cpu[n]_start function (NuttX armv7-a arm_cpuhead.S).  CPU0 brings
 *   CPUn up with up_cpu_on() -> up_cpu_up() (amebasmart_smp.c: power-on
 *   and PSCI CPU_ON at __cpu1_start), reached through sched_cpuon().  Like
 *   the NuttX qemu chip, this function enables the cache coherency, the
 *   FPU and the GIC CPU interface of CPUn, then enters the IDLE task of
 *   CPUn with nx_idle_trampoline().  The system tick stays on CPU0.
 *
 * Input Parameters:
 *   cpu - The CPU index.  This is the same value that would be obtained by
 *      calling up_cpu_index();
 *
 * Returned Value:
 *   Does not return.
 *
 ****************************************************************************/

void arm_cpu_boot(int cpu)
{
  /* Enable SMP cache coherency for the CPU (no SCU on the Cortex-A32) */

  amebasmart_enable_smp(cpu);

  /* Initialize the FPU */

  arm_fpuconfig();

  /* Initialize the Generic Interrupt Controller (GIC) for CPUn (n != 0),
   * set VBAR and enable the interrupts.
   */

  up_irqinitialize();

  /* Then transfer control to the IDLE task */

  nx_idle_trampoline();
}

#endif /* CONFIG_SMP */
