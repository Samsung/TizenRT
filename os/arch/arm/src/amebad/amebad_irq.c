/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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
 *
 *   Copyright (C) 2020 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdint.h>
#include <debug.h>

#include <tinyara/irq.h>
#include <tinyara/arch.h>
#include <arch/irq.h>

#ifdef CONFIG_SYSTEM_REBOOT_REASON
#include <tinyara/reboot_reason.h>
#include <arch/reboot_reason.h>
#endif

#include "nvic.h"
#include "ram_vectors.h"
#include "up_arch.h"
#include "up_internal.h"
#include "ameba_soc.h"
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Get a 32-bit version of the default priority */

#define DEFPRIORITY32 \
  (NVIC_SYSH_PRIORITY_DEFAULT << 24 | \
   NVIC_SYSH_PRIORITY_DEFAULT << 16 | \
   NVIC_SYSH_PRIORITY_DEFAULT << 8  | \
   NVIC_SYSH_PRIORITY_DEFAULT)

/* Given the address of a NVIC ENABLE register, this is the offset to
 * the corresponding CLEAR ENABLE register.
 */

#define NVIC_ENA_OFFSET    (0)
#define NVIC_CLRENA_OFFSET (NVIC_IRQ0_31_CLEAR - NVIC_IRQ0_31_ENABLE)

/****************************************************************************
 * Public Data
 ****************************************************************************/

/* g_current_regs[] holds a references to the current interrupt level
 * register storage structure.  If is non-NULL only during interrupt
 * processing.  Access to g_current_regs[] must be through the macro
 * CURRENT_REGS for portability.
 */
volatile uint32_t *current_regs;

/* This is the address of the  exception vector table (determined by the
 * linker script).
 */

//extern uint32_t _vectors[];

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: amebad_dumpnvic
 *
 * Description:
 *   Dump some interesting NVIC registers
 *
 ****************************************************************************/

#if defined(CONFIG_DEBUG_IRQ)
static void amebad_dumpnvic(const char *msg, int irq)
{
	irqstate_t flags;

	flags = irqsave();
	lldbg("NVIC (%s, irq=%d):\n", msg, irq);
	lldbg("  INTCTRL:    %08x VECTAB:  %08x\n", getreg32(NVIC_INTCTRL), getreg32(NVIC_VECTAB));
#if 0
	lldbg("  SYSH ENABLE MEMFAULT: %08x BUSFAULT: %08x USGFAULT: %08x SYSTICK: %08x\n", getreg32(NVIC_SYSHCON_MEMFAULTENA), getreg32(NVIC_SYSHCON_BUSFAULTENA), getreg32(NVIC_SYSHCON_USGFAULTENA), getreg32(NVIC_SYSTICK_CTRL_ENABLE));
#endif
	lldbg("  IRQ ENABLE: %08x %08x %08x\n", getreg32(NVIC_IRQ0_31_ENABLE), getreg32(NVIC_IRQ32_63_ENABLE), getreg32(NVIC_IRQ64_95_ENABLE));
	lldbg("  SYSH_PRIO:  %08x %08x %08x\n", getreg32(NVIC_SYSH4_7_PRIORITY), getreg32(NVIC_SYSH8_11_PRIORITY), getreg32(NVIC_SYSH12_15_PRIORITY));
	lldbg("  IRQ PRIO:   %08x %08x %08x %08x\n", getreg32(NVIC_IRQ0_3_PRIORITY), getreg32(NVIC_IRQ4_7_PRIORITY), getreg32(NVIC_IRQ8_11_PRIORITY), getreg32(NVIC_IRQ12_15_PRIORITY));
	lldbg("              %08x %08x %08x %08x\n", getreg32(NVIC_IRQ16_19_PRIORITY), getreg32(NVIC_IRQ20_23_PRIORITY), getreg32(NVIC_IRQ24_27_PRIORITY), getreg32(NVIC_IRQ28_31_PRIORITY));
	lldbg("              %08x %08x %08x %08x\n", getreg32(NVIC_IRQ32_35_PRIORITY), getreg32(NVIC_IRQ36_39_PRIORITY), getreg32(NVIC_IRQ40_43_PRIORITY), getreg32(NVIC_IRQ44_47_PRIORITY));
	lldbg("              %08x %08x %08x %08x\n", getreg32(NVIC_IRQ48_51_PRIORITY), getreg32(NVIC_IRQ52_55_PRIORITY), getreg32(NVIC_IRQ56_59_PRIORITY), getreg32(NVIC_IRQ60_63_PRIORITY));
	lldbg("              %08x\n", getreg32(NVIC_IRQ64_67_PRIORITY));
	irqrestore(flags);
}
#else
#  define amebad_dumpnvic(msg, irq)
#endif

/****************************************************************************
 * Name: amebad_nmi, amebad_pendsv, amebad_dbgmonitor,
 *       amebad_pendsv, amebad_reserved
 *
 * Description:
 *   Handlers for various execptions.  None are handled and all are fatal
 *   error conditions.  The only advantage these provided over the default
 *   unexpected interrupt handler is that they provide a diagnostic output.
 *
 ****************************************************************************/

#ifdef CONFIG_DEBUG
static int amebad_nmi(int irq, FAR void *context, FAR void *arg)
{
  (void)irqsave();
  dbg("PANIC!!! NMI received\n");
#ifdef CONFIG_SYSTEM_REBOOT_REASON
  up_reboot_reason_write(REBOOT_SYSTEM_PREFETCHABORT);
#endif
  PANIC();
  return 0;
}

static int amebad_pendsv(int irq, FAR void *context, FAR void *arg)
{
  (void)irqsave();
  dbg("PANIC!!! PendSV received\n");
#ifdef CONFIG_SYSTEM_REBOOT_REASON
  up_reboot_reason_write(REBOOT_SYSTEM_PREFETCHABORT);
#endif
  PANIC();
  return 0;
}

static int amebad_dbgmonitor(int irq, FAR void *context, FAR void *arg)
{
  (void)irqsave();
  dbg("PANIC!!! Debug Monitor received\n");
#ifdef CONFIG_SYSTEM_REBOOT_REASON
  up_reboot_reason_write(REBOOT_SYSTEM_PREFETCHABORT);
#endif
  PANIC();
  return 0;
}

static int amebad_reserved(int irq, FAR void *context, FAR void *arg)
{
  (void)irqsave();
  dbg("PANIC!!! Reserved interrupt\n");
#ifdef CONFIG_SYSTEM_REBOOT_REASON
  up_reboot_reason_write(REBOOT_SYSTEM_PREFETCHABORT);
#endif
  PANIC();
  return 0;
}
#endif

/****************************************************************************
 * Name: amebad_prioritize_syscall
 *
 * Description:
 *   Set the priority of an exception.  This function may be needed
 *   internally even if support for prioritized interrupts is not enabled.
 *
 ****************************************************************************/

#ifdef CONFIG_ARMV8M_USEBASEPRI
static inline void amebad_prioritize_syscall(int priority)
{
  uint32_t regval;

  /* SVCALL is system handler 11 */

  regval  = getreg32(NVIC_SYSH8_11_PRIORITY);
  regval &= ~NVIC_SYSH_PRIORITY_PR11_MASK;
  regval |= (priority << NVIC_SYSH_PRIORITY_PR11_SHIFT);
  putreg32(regval, NVIC_SYSH8_11_PRIORITY);
}
#endif

/****************************************************************************
 * Name: amebad_irqinfo
 *
 * Description:
 *   Given an IRQ number, provide the register and bit setting to enable or
 *   disable the irq.
 *
 ****************************************************************************/

static int amebad_irqinfo(int irq, uintptr_t *regaddr, uint32_t *bit,
                         uintptr_t offset)
{
  int n;

  DEBUGASSERT(irq >= AMEBAD_IRQ_NMI && irq < NR_IRQS);

  /* Check for external interrupt */

  if (irq >= AMEBAD_IRQ_FIRST)
    {
      n        = irq - AMEBAD_IRQ_FIRST;
      *regaddr = NVIC_IRQ_ENABLE(n) + offset;
      *bit     = (uint32_t)1 << (n & 0x1f);
    }

  /* Handle processor exceptions.  Only a few can be disabled */

  else
    {
      *regaddr = NVIC_SYSHCON;
      if (irq == AMEBAD_IRQ_MEMFAULT)
        {
          *bit = NVIC_SYSHCON_MEMFAULTENA;
        }
      else if (irq == AMEBAD_IRQ_BUSFAULT)
        {
          *bit = NVIC_SYSHCON_BUSFAULTENA;
        }
      else if (irq == AMEBAD_IRQ_USAGEFAULT)
        {
          *bit = NVIC_SYSHCON_USGFAULTENA;
        }
      else if (irq == AMEBAD_IRQ_SYSTICK)
        {
          *regaddr = NVIC_SYSTICK_CTRL;
          *bit = NVIC_SYSTICK_CTRL_ENABLE;
        }
      else
        {
          return ERROR; /* Invalid or unsupported exception */
        }
    }

	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_irqinitialize
 ****************************************************************************/

void up_irqinitialize(void)
{

#ifdef CONFIG_ARCH_RAMVECTORS
  /* If CONFIG_ARCH_RAMVECTORS is defined, then we are using a RAM-based
   * vector table that requires special initialization.
   */

  up_ramvec_initialize();
#endif


  /* currents_regs is non-NULL only while processing an interrupt */

  current_regs = NULL;

  /* Attach the SVCall and Hard Fault exception handlers.  The SVCall
   * exception is used for performing context switches; The Hard Fault
   * must also be caught because a SVCall may show up as a Hard Fault
   * under certain conditions.
   */

  irq_attach(AMEBAD_IRQ_SVCALL, up_svcall, NULL);
  irq_attach(AMEBAD_IRQ_HARDFAULT, up_hardfault, NULL);

  /* Set the priority of the SVCall interrupt */

#ifdef CONFIG_ARCH_IRQPRIO
  up_prioritize_irq(AMEBAD_IRQ_PENDSV, NVIC_SYSH_PRIORITY_MIN);
#endif
#ifdef CONFIG_ARMV8M_USEBASEPRI
#ifdef CONFIG_ARCH_IRQPRIO
  up_prioritize_irq(AMEBAD_IRQ_SVCALL,NVIC_SYSH_SVCALL_PRIORITY);
#else
  amebad_prioritize_syscall(NVIC_SYSH_SVCALL_PRIORITY);
#endif
#endif
  /* Attach all other processor exceptions (except reset and sys tick) */

#ifdef CONFIG_ARMV8M_MPU
  irq_attach(AMEBAD_IRQ_MEMFAULT, up_memfault, NULL);
#endif
  irq_attach(AMEBAD_IRQ_BUSFAULT, up_busfault, NULL);
  irq_attach(AMEBAD_IRQ_USAGEFAULT, up_usagefault, NULL);

#ifdef CONFIG_DEBUG
  irq_attach(AMEBAD_IRQ_NMI, amebad_nmi, NULL);
  irq_attach(AMEBAD_IRQ_PENDSV, amebad_pendsv, NULL);
  irq_attach(AMEBAD_IRQ_DBGMONITOR, amebad_dbgmonitor, NULL);
  irq_attach(AMEBAD_IRQ_RESERVED, amebad_reserved, NULL);
#endif

  amebad_dumpnvic("initial", NR_IRQS);

#ifndef CONFIG_SUPPRESS_INTERRUPTS

  /* And finally, enable interrupts */

  irqenable();
#endif
}

/****************************************************************************
 * Name: up_disable_irq
 *
 * Description:
 *   Disable the IRQ specified by 'irq'
 *
 ****************************************************************************/

void up_disable_irq(int irq)
{
#if 1
	  irq -= AMEBAD_IRQ_FIRST;
	  __NVIC_DisableIRQ(irq);
#else
  uintptr_t regaddr;
  uint32_t regval;
  uint32_t bit;
  if (amebad_irqinfo(irq, &regaddr, &bit, NVIC_CLRENA_OFFSET) == 0)
    {
      /* Modify the appropriate bit in the register to disable the interrupt.
       * For normal interrupts, we need to set the bit in the associated
       * Interrupt Clear Enable register.  For other exceptions, we need to
       * clear the bit in the System Handler Control and State Register.
       */

      if (irq >= AMEBAD_IRQ_FIRST)
        {
          putreg32(bit, regaddr);
        }
      else
        {
          regval  = getreg32(regaddr);
          regval &= ~bit;
          putreg32(regval, regaddr);
        }
    }
#endif
	amebad_dumpnvic("disable", irq);
}

/****************************************************************************
 * Name: up_enable_irq
 *
 * Description:
 *   Enable the IRQ specified by 'irq'
 *
 ****************************************************************************/

void up_enable_irq(int irq)
{
#if 1
	irq -= AMEBAD_IRQ_FIRST;
	__NVIC_EnableIRQ(irq);
#else
	uintptr_t regaddr;
	uint32_t regval;
	uint32_t bit;
	if (amebad_irqinfo(irq, &regaddr, &bit, NVIC_ENA_OFFSET) == 0)
	{
		/* Modify the appropriate bit in the register to enable the interrupt.
		* For normal interrupts, we need to set the bit in the associated
		* Interrupt Set Enable register.  For other exceptions, we need to
		* set the bit in the System Handler Control and State Register.
		*/
		if (irq >= AMEBAD_IRQ_FIRST)
		{
			putreg32(bit, regaddr);
		}
		else
		{
			regval  = getreg32(regaddr);
			regval |= bit;
			putreg32(regval, regaddr);
		}
	}
#endif
	amebad_dumpnvic("enable", irq);
}

/****************************************************************************
 * Name: up_ack_irq
 *
 * Description:
 *   Acknowledge the IRQ
 *
 ****************************************************************************/

void up_ack_irq(int irq)
{
}

/****************************************************************************
 * Name: up_prioritize_irq
 *
 * Description:
 *   Set the priority of an IRQ.
 *
 *   Since this API is not supported on all architectures, it should be
 *   avoided in common implementations where possible.
 *
 ****************************************************************************/

#ifdef CONFIG_ARCH_IRQPRIO
int up_prioritize_irq(int irq, int priority)
{

  DEBUGASSERT(irq >= AMEBAD_IRQ_MEMFAULT && irq < NR_IRQS &&
              (unsigned)priority <= NVIC_SYSH_PRIORITY_MIN);
#if 1
  irq -= AMEBAD_IRQ_FIRST;
  priority = priority >> (8U - __NVIC_PRIO_BITS);
  __NVIC_SetPriority(irq, priority); //use CMSIS for now
#else
  uint32_t regaddr;
  uint32_t regval;
  int shift;
  if (irq < AMEBAD_IRQ_FIRST)
    {
      /* NVIC_SYSH_PRIORITY() maps {0..15} to one of three priority
       * registers (0-3 are invalid)
       */

      regaddr = NVIC_SYSH_PRIORITY(irq);
      irq    -= 4;
    }
  else
    {
      /* NVIC_IRQ_PRIORITY() maps {0..} to one of many priority registers */

      irq    -= AMEBAD_IRQ_FIRST;
      regaddr = NVIC_IRQ_PRIORITY(irq);
    }

  regval      = getreg32(regaddr);
  shift       = ((irq & 3) << 3);
  regval     &= ~(0xff << shift);
  regval     |= (priority << shift);
  putreg32(regval, regaddr);
#endif
  amebad_dumpnvic("prioritize", irq);
  return OK;
}
#endif
