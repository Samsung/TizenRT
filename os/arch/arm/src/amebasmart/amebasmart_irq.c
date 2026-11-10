/****************************************************************************
 * arch/arm/src/amebasmart/amebasmart_irq.c
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

#include <assert.h>

#include <tinyara/arch.h>

#include "arm_internal.h"
#include "arm_cpupause.h"
#include "amebasmart_enablesmp.h"
#include "sctlr.h"
#include "gic.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Size of the interrupt stack allocation */

#define INTSTACK_ALLOC (CONFIG_SMP_NCPUS * INTSTACK_SIZE)

/****************************************************************************
 * Public Data
 ****************************************************************************/

#include "section_config.h"
SRAM_BD_BSS_SECTION
volatile uint32_t *g_current_regs[CONFIG_SMP_NCPUS];

#if defined(CONFIG_SMP) && CONFIG_ARCH_INTERRUPTSTACK > 7
/* In the SMP configuration, we will need custom IRQ and FIQ stacks.
 * These definitions provide the aligned stack allocations.
 */

uint64_t g_irqstack_alloc[INTSTACK_ALLOC >> 3];

/* These are arrays that point to the top of each interrupt stack */

uintptr_t g_irqstack_top[CONFIG_SMP_NCPUS] =
{
  (uintptr_t)g_irqstack_alloc + INTSTACK_SIZE,
#if CONFIG_SMP_NCPUS > 1
  (uintptr_t)g_irqstack_alloc + (2 * INTSTACK_SIZE),
#endif
#if CONFIG_SMP_NCPUS > 2
  (uintptr_t)g_irqstack_alloc + (3 * INTSTACK_SIZE),
#endif
#if CONFIG_SMP_NCPUS > 3
  (uintptr_t)g_irqstack_alloc + (4 * INTSTACK_SIZE)
#endif
};

#if defined(CONFIG_ARMV7A_DECODEFIQ)

uint64_t g_fiqstack_alloc[INTSTACK_ALLOC >> 3];

uintptr_t g_fiqstack_top[CONFIG_SMP_NCPUS] =
{
  (uintptr_t)g_fiqstack_alloc + INTSTACK_SIZE,
#if CONFIG_SMP_NCPUS > 1
  (uintptr_t)g_fiqstack_alloc + 2 * INTSTACK_SIZE,
#endif
#if CONFIG_SMP_NCPUS > 2
  (uintptr_t)g_fiqstack_alloc + 3 * INTSTACK_SIZE,
#endif
#if CONFIG_SMP_NCPUS > 3
  (uintptr_t)g_fiqstack_alloc + 4 * INTSTACK_SIZE
#endif
};

#endif

#endif

/* Symbols defined via the linker script */

extern uint8_t _vector_start[]; /* Beginning of vector block */
extern uint8_t _vector_end[];   /* End+1 of vector block */

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_irqinitialize
 *
 * Description:
 *   This function is called by up_initialize() during the bring-up of the
 *   system.  It is the responsibility of this function to but the interrupt
 *   subsystem into the working and ready state.
 *
 ****************************************************************************/

void up_irqinitialize(void)
{
  /* The following operations need to be atomic, but since this function is
   * called early in the initialization sequence, we expect to have exclusive
   * access to the GIC.
   */

  /* Initialize the Generic Interrupt Controller (GIC).  The distributor is
   * initialized once by CPU0; CPUn only initializes its CPU interface when
   * arm_cpu_boot() calls this function (as in the NuttX qemu chip).
   */

  if (up_cpu_index() == 0)
    {
      arm_gic0_initialize();  /* Initialization unique to CPU0 */

#ifdef CONFIG_SMP
      /* Attach the CPU pause handler used while the XIP flash is
       * programmed.  The handler is attached to all CPUs at once.
       */

      amebasmart_cpupause_initialize();
#endif
    }

  arm_gic_initialize();   /* Initialization common to all CPUs */

#ifdef CONFIG_ARCH_LOWVECTORS
  /* If CONFIG_ARCH_LOWVECTORS is defined, then the vectors located at the
   * beginning of the .text region must appear at address at the address
   * specified in the VBAR.  There are two ways to accomplish this:
   *
   *   1. By explicitly mapping the beginning of .text region with a page
   *      table entry so that the virtual address zero maps to the beginning
   *      of the .text region.  VBAR == 0x0000:0000.
   *
   *   2. Set the Cortex-A5 VBAR register so that the vector table address
   *      is moved to a location other than 0x0000:0000.
   *
   *  The second method is used by this logic.
   */

  /* Set the VBAR register to the address of the vector table */

  DEBUGASSERT((((uintptr_t)_vector_start) & ~VBAR_MASK) == 0);
  cp15_wrvbar((uint32_t)_vector_start);
#endif /* CONFIG_ARCH_LOWVECTORS */

  /* Enable the generic timer event stream for this CPU: every WFE in
   * the kernel (spin_lock_notrace wait, CPU pause stage-5 wait) then
   * wakes within a bounded time even if an SEV is lost.  Runs after
   * the GIC and VBAR are set up and attaches the pause SGI handler.
   */

  amebasmart_enable_evtstrm((int)up_cpu_index());

#ifndef CONFIG_SUPPRESS_INTERRUPTS

  /* And finally, enable interrupts */

  up_irq_enable();
#endif
}

/****************************************************************************
 * Name: up_get_intstackbase
 *
 * Description:
 *   Return a pointer to the "alloc" the correct interrupt stack allocation
 *   for the given CPU (NuttX 12.13 interface; the old TizenRT had
 *   arm_intstack_top() and arm_intstack_alloc() for the current CPU).
 *
 ****************************************************************************/

#if defined(CONFIG_SMP) && CONFIG_ARCH_INTERRUPTSTACK > 7
uintptr_t up_get_intstackbase(int cpu)
{
  return g_irqstack_top[cpu] - INTSTACK_SIZE;
}
#endif
