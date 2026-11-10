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
 * arch/arm/src/amebasmart/amebasmart_enablesmp.c
 *
 * Based on arch/arm/src/armv7-a/arm_scu.c of NuttX 12.13.0.
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

#include <stdint.h>
#include <debug.h>

#include <arch/barriers.h>
#include <arch/irq.h>
#include <sched/sched.h>

#include "arm_internal.h"
#include "cp15_cacheops.h"
#include "sctlr.h"
#include "amebasmart_enablesmp.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: amebasmart_enable_evtstrm
 *
 * Description:
 *   Enable the ARMv7 generic timer event stream on the current CPU.
 *
 *   spin_lock_notrace() and the stage-5 wait of the XIP flash pause
 *   protocol park in WFE and rely on the UP_SEV() of the lock holder to
 *   wake up.  On the RTL8730E CA32 cluster that single SEV can be lost
 *   when it arrives while the peer core is entering its clock-gated WFE
 *   state, and without an event stream the parked core is then never
 *   woken again: the kernel hangs inside a contended spin_lock() (seen
 *   with the ostest spinlock test) and a pending pause SGI is never
 *   delivered (seen as "CPUPAUSE TIMEOUT ... stage_tgt=0 wfe=0x2").
 *   ARMv7 designs WFE loops to be made safe by the event stream; this
 *   enables it.  The counter runs at GENERICTIMERFREQ (50 MHz); with
 *   EVNTIS = 0 the counter is divided by 2^10, producing an event every
 *   ~20us, so a lost wakeup becomes a bounded delay.
 *
 *   The event stream control depends on the exception level the kernel
 *   executes at (the AP boots through the Realtek EL3/EL2 firmware):
 *   at EL2 (HYP mode) it is enabled by CNTHCTL.EVNTEN (op1 = 4, bit 8)
 *   and at EL1/EL3 by CNTKCTL.EVNTEN (op1 = 0, bit 2).  The kernel
 *   programs the CNTV_* registers with the op1 = 0 encodings from the
 *   system tick already, so those accesses are proven legal at the
 *   current level; the mode check only selects which control bit to
 *   set.  Call after up_irqinitialize() has set VBAR so that an
 *   unexpected access fault is reported through the abort handlers
 *   instead of running uninstalled vectors.
 *
 *   The control registers are per-CPU, so no critical section is
 *   needed.  Called once per CPU, with logging, from up_irqinitialize().
 */

#define EVTSTRM_MODE_HYP   0x1a   /* CPSR.M in HYP mode (AArch32 EL2)   */

void amebasmart_enable_evtstrm(int cpu)
{
	uint32_t cpsr;
	uint32_t regval;
	uint32_t check;

	__asm__ volatile("mrs %0, cpsr\n\t"
					 : "=r"(cpsr) : : "memory");

	if ((cpsr & 0x1f) == EVTSTRM_MODE_HYP)
	{
		/* EL2: CNTHCTL.EVNTEN(8) = 1, EVNTDIR(9) = 0, EVNTIS(10) = 0 */

		__asm__ volatile("mrc p15, 4, %0, c14, c1, 0\n\t"
						 : "=r"(regval) : : "memory");

		regval &= ~((1 << 10) | (1 << 9));
		regval |= (1 << 8);

		__asm__ volatile("mcr p15, 4, %0, c14, c1, 0\n\t"
						 : : "r"(regval) : "memory");
	}
	else
	{
		/* EL1/EL3: CNTKCTL.EVNTEN(2) = 1, EVNTDIR(3) = 0,
		 * EVNTIS(7:4) = 0 (divide by 2^10)
		 */

		__asm__ volatile("mrc p15, 0, %0, c14, c1, 0\n\t"
						 : "=r"(regval) : : "memory");

		regval &= ~(0xf0);   /* clear EVNTIS(7:4) and EVNTDIR(3) */
		regval |= (1 << 2);

		__asm__ volatile("mcr p15, 0, %0, c14, c1, 0\n\t"
						 : : "r"(regval) : "memory");
	}

	__asm__ volatile("isb\n\t"
					 : : : "memory");

	/* Read back and report so the boot log proves the event stream is
	 * enabled on every CPU.
	 */

	if ((cpsr & 0x1f) == EVTSTRM_MODE_HYP)
	{
		__asm__ volatile("mrc p15, 4, %0, c14, c1, 0\n\t"
						 : "=r"(check) : : "memory");
	}
	else
	{
		__asm__ volatile("mrc p15, 0, %0, c14, c1, 0\n\t"
						 : "=r"(check) : : "memory");
	}

	lowsyslog("[CPU%d] evtstrm: mode=0x%x reg=0x%08x\n",
			  cpu, (unsigned int)(cpsr & 0x1f), (unsigned int)check);
}

/****************************************************************************
 * Name: amebasmart_enable_smp
 *
 * Description:
 *   Make the current CPU take part in the SMP cache coherency.  This is
 *   arm_enable_smp() of the NuttX armv7-a code without the SCU accesses:
 *   the Cortex-A32 has no memory-mapped SCU, and on AmebaSmart the SCU
 *   offsets of arm_enable_smp() fall into the reserved start of the GIC-400
 *   (CHIP_MPCORE_VBASE).  The old TizenRT arm_scu.c skipped them for the
 *   Cortex-A32 as well.  ACTLR.SMP is set as by the Realtek start-up code.
 *
 * Assumption:
 *   Called early in the CPU start-up.  No special critical sections are
 *   needed if only CPU-private registers are modified.
 *
 ****************************************************************************/

void amebasmart_enable_smp(int cpu)
{
	uint32_t regval;

	/* Handle actions unique to CPU0 which comes up first */

	if (cpu == 0) {
		/* Invalidate CPU0 L1 data cache so that is will we be reloaded from
		 * coherent L2.
		 */

		cp15_invalidate_dcache_all();
		UP_DSB();

		/* Initialize done, kick other cpus which waiting on __start */

		UP_SEV();
	}

	/* Actions for other CPUs */

	else {
		/* Take part in the SMP cache coherency first.  Without ACTLR.SMP
		 * the reads below do not snoop the dirty L1 of CPU0, so the
		 * current_task() wait would spin on a stale null value forever.
		 * The Realtek secondary start-up used to set ACTLR.SMP in the
		 * assembly entry; the NuttX arm_cpuhead.S does not.
		 */

		regval  = CP15_GET(ACTLR);
		regval |= ACTLR_SMP;
		CP15_SET(ACTLR, regval);
		UP_DSB();

		/* NOTE: no L1 invalidate here.  The data cache of the secondary
		 * CPU has never been enabled, so there is nothing stale to
		 * invalidate; moreover cp15_dcache_op_level() does not return
		 * on the real RTL8730E hardware in this state (ACTLR.SMP set,
		 * SCTLR.C clear), which wedged the CPU1 boot.
		 */

		/* We need to confirm that current_task has been initialized. */

		while (!current_task(this_cpu()));

		/* Init idle task to percpu reg */

		up_update_task(current_task(cpu));
	}

	/* Enable the data cache, set the SMP mode with ACTLR.SMP=1 (already
	 * set above for the secondary CPUs).
	 */

	if (cpu == 0) {
		regval  = CP15_GET(ACTLR);
		regval |= ACTLR_SMP;
		CP15_SET(ACTLR, regval);
	}

	regval  = CP15_GET(SCTLR);
	regval |= SCTLR_C | SCTLR_I | SCTLR_M;
	CP15_SET(SCTLR, regval);
}
