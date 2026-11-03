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
#include "amebasmart_smp.h"
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
 *   __cpu[n]_start function (NuttX armv7-a arm_cpuhead.S).  CPU0 starts
 *   CPUn with up_cpu_start() (amebasmart_smp.c: power-on and PSCI CPU_ON
 *   at __cpu1_start).  Like the NuttX qemu chip, this function enables the
 *   cache coherency, the FPU and the GIC CPU interface of CPUn, then enters
 *   the IDLE task of CPUn with nx_idle_trampoline().  The system tick stays
 *   on CPU0.
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

/****************************************************************************
 * Name: up_cpu_start
 *
 * Description:
 *   Start CPUn (called by nx_smp_start() on CPU0).  This overrides the weak
 *   SGI version of the NuttX armv7-a arm_cpustart.c, like the NuttX qemu
 *   chip does in qemu_boot.c.  CPU1 is powered off at cold boot: power it
 *   on and start it at __cpu1_start (NuttX armv7-a arm_cpuhead.S) with the
 *   PSCI CPU_ON of the secure firmware (amebasmart_smp.c).  __cpu1_start
 *   continues with arm_cpu_boot() above.
 *
 *   It lives in this file and not in amebasmart_smp.c because this object
 *   is always linked (arm_cpuhead.S calls arm_cpu_boot()); a strong
 *   definition in an archive member that nothing else pulls in would lose
 *   against the weak one of arm_cpustart.o.
 *
 * Input Parameters:
 *   cpu - The index of the CPU being started (1 .. CONFIG_SMP_NCPUS-1)
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

#ifdef CONFIG_AMEBASMART_TRUSTZONE
int up_cpu_start(int cpu)
{
  int ret;

  DEBUGASSERT(cpu > 0 && cpu < CONFIG_SMP_NCPUS && cpu != this_cpu());

#ifdef CONFIG_SCHED_INSTRUMENTATION
  /* Notify of the start event */

  sched_note_cpu_start(this_task(), cpu);
#endif

  ret = cpu_core_poweron(cpu);
  if (ret < 0)
    {
      smplldbg("Failed to power on CPU%d: %d\n", cpu, ret);
      return ret;
    }

  ret = vPortSecondaryBoot(cpu);
  if (ret < 0)
    {
      smplldbg("Failed to boot CPU%d: %d\n", cpu, ret);
    }

  return ret;
}
#endif /* CONFIG_AMEBASMART_TRUSTZONE */

#endif /* CONFIG_SMP */
