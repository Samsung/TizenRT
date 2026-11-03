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
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * arch/arm/src/amebasmart/arm_cpupause.c
 *
 * Pause and resume the secondary CPUs while the XIP flash is programmed.
 *
 * The RTL8730E executes the kernel in place from the external flash
 * (XIP).  While one CPU writes or erases the flash, no other CPU may
 * fetch instructions from it.  The Realtek flash driver calls
 * up_cpu_pause_all() before touching the flash and up_cpu_resume_all()
 * when it is done; the paused CPUs spin in WFE inside this file, which
 * the linker script keeps in PSRAM (see the *arm_cpupause.o rules in
 * rlx8730e_img2.ld).
 *
 * This is a port of the armv7-a CPU pause support that TizenRT carried
 * before the NuttX 12.13.0 sync dropped it.  The protocol is unchanged:
 *
 * 1. The up_cpu_pause() implementation on CPUn locks both g_cpu_wait[m]
 *    and g_cpu_paused[m].  CPUn then waits spinning on g_cpu_paused[m].
 * 2. CPUm receives the pause SGI; its handler (1) locks g_cpu_resumed[m],
 *    (2) unlocks g_cpu_paused[m] and (3) locks g_cpu_wait[m].  The second
 *    unblocks CPUn and the third blocks CPUm in the interrupt handler.
 *
 * When CPUm resumes, CPUn unlocks g_cpu_wait[m] and the interrupt handler
 * on CPUm continues.  CPUm must, of course, also then unlock g_cpu_wait[m]
 * so that it will be ready for the next pause operation.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdint.h>
#include <assert.h>

#include <tinyara/arch.h>
#include <tinyara/sched.h>
#include <tinyara/spinlock.h>

#include "arm_cpupause.h"
#include "gic.h"

#ifdef CONFIG_SMP

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* The pause request is delivered with SGI4.  SGI1 .. SGI3 are taken by
 * GIC_SMP_CPUSTART, GIC_SMP_CALL and GIC_SMP_SCHED (gic.h); the old
 * armv7-a implementation used SGI2 which now belongs to GIC_SMP_CALL.
 */

#define GIC_SMP_PAUSE  GIC_IRQ_SGI4

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* These spinlocks are used in the SMP configuration in order to implement
 * up_cpu_pause().  See the file comment block above for the protocol.
 */

static volatile spinlock_t g_cpu_wait[CONFIG_SMP_NCPUS];
static volatile spinlock_t g_cpu_paused[CONFIG_SMP_NCPUS];
static volatile spinlock_t g_cpu_resumed[CONFIG_SMP_NCPUS];

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_cpu_pausereq
 *
 * Description:
 *   Return true if a pause request is pending for this CPU.
 *
 * Input Parameters:
 *   cpu - The index of the CPU to be queried
 *
 * Returned Value:
 *   true   = a pause request is pending.
 *   false = no pasue request is pending.
 *
 ****************************************************************************/

bool up_cpu_pausereq(int cpu)
{
  return spin_is_locked(&g_cpu_paused[cpu]);
}

/****************************************************************************
 * Name: up_is_cpu_paused
 *
 * Description:
 *   Return true if this CPU is in paused state.
 *
 * Input Parameters:
 *   cpu - The index of the CPU to be queried
 *
 * Returned Value:
 *   true   = cpu is paused.
 *   false = cpu is not paused.
 *
 ****************************************************************************/

bool up_is_cpu_paused(int cpu)
{
  return spin_is_locked(&g_cpu_resumed[cpu]);
}


/****************************************************************************
 * Name: up_cpu_paused
 *
 * Description:
 *   Handle a pause request from another CPU.  This is called from the
 *   pause SGI handler on the CPU being paused and performs the following
 *   operations:
 *
 *   1. It marks the CPU paused (g_cpu_resumed),
 *   2. It releases the requesting CPU (g_cpu_paused), then
 *   3. It waits in WFE on g_cpu_wait until up_cpu_resume() releases it.
 *
 *   The whole wait runs from PSRAM (see the file comment block), so the
 *   requesting CPU can program the XIP flash in the meantime.
 *
 * Input Parameters:
 *   cpu - The index of the CPU to be paused
 *
 * Returned Value:
 *   On success, OK is returned.  Otherwise, a negated errno value indicating
 *   the nature of the failure is returned.
 *
 ****************************************************************************/

int up_cpu_paused(int cpu)
{
  /* Ensure the CPU has been resumed to avoid causing a deadlock */

  spin_lock(&g_cpu_resumed[cpu]);

  /* Release the g_cpu_paused spinlock to synchronize with the
   * requesting CPU.
   */

  spin_unlock(&g_cpu_paused[cpu]);

  /* Wait for the spinlock to be released.  The requesting CPU will release
   * the spinlock when the CPU is resumed.  spin_lock() waits in WFE, which
   * also sets the CA32 STANDBYWFE status the flash driver waits for.
   */

  spin_lock(&g_cpu_wait[cpu]);

  spin_unlock(&g_cpu_wait[cpu]);
  spin_unlock(&g_cpu_resumed[cpu]);

  return OK;
}

/****************************************************************************
 * Name: up_cpu_pause
 *
 * Description:
 *   Pause task execution on the CPU.  The CPU is resumed with
 *   up_cpu_resume().
 *
 *   This function is called when the logic executing on one CPU needs the
 *   other CPU to stop fetching from the XIP flash.
 *
 * Input Parameters:
 *   cpu - The index of the CPU to be stopped
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

int up_cpu_pause(int cpu)
{
  DEBUGASSERT(cpu >= 0 && cpu < CONFIG_SMP_NCPUS && cpu != this_cpu());

  /* If a pause request is already pending or if the cpu is already in
   * paused state, then dont send the pause
   * request again.
   */

  if (up_cpu_pausereq(cpu) || up_is_cpu_paused(cpu))
    {
      return OK;
    }

  /* Take the both spinlocks.  The g_cpu_wait spinlock will prevent the SGI
   * handler from returning until up_cpu_resume() is called; g_cpu_paused
   * is a handshake that will prefent this function from returning until
   * the CPU is actually paused.
   * Note that we might spin before getting g_cpu_wait, this just means that
   * the other CPU still hasn't finished responding to the previous resume
   * request.
   */

  DEBUGASSERT(!spin_is_locked(&g_cpu_paused[cpu]));

  spin_lock(&g_cpu_wait[cpu]);
  spin_lock(&g_cpu_paused[cpu]);

  /* Execute the pause SGI */

  arm_cpu_sgi(GIC_SMP_PAUSE, (1 << cpu));

  /* Wait for the other CPU to unlock g_cpu_paused meaning that
   * it is fully paused and ready for up_cpu_resume();
   */

  spin_lock(&g_cpu_paused[cpu]);
  spin_unlock(&g_cpu_paused[cpu]);

  /* On successful return g_cpu_wait will be locked, the other CPU will be
   * spinning on g_cpu_wait and will not continue until g_cpu_resume() is
   * called.  g_cpu_paused will be unlocked in any case.
   */

  return OK;
}

/****************************************************************************
 * Name: up_cpu_resume
 *
 * Description:
 *   Restart the cpu after it was paused via up_cpu_pause().
 *
 * Input Parameters:
 *   cpu - The index of the CPU being re-started.
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

int up_cpu_resume(int cpu)
{
  DEBUGASSERT(cpu >= 0 && cpu < CONFIG_SMP_NCPUS && cpu != this_cpu());

  /* Resume the CPU only if it has been paused earlier */

  if (up_cpu_pausereq(cpu) || up_is_cpu_paused(cpu))
    {
      /* Release the spinlock.  Releasing the spinlock will cause the SGI
       * handler on 'cpu' to continue and return from interrupt.
       */

      DEBUGASSERT(spin_is_locked(&g_cpu_wait[cpu]) &&
                  !spin_is_locked(&g_cpu_paused[cpu]));

      spin_unlock(&g_cpu_wait[cpu]);

      /* Ensure the CPU has been resumed to avoid causing a deadlock */

      spin_lock(&g_cpu_resumed[cpu]);

      spin_unlock(&g_cpu_resumed[cpu]);
    }

  return OK;
}

/****************************************************************************
 * Name: up_cpu_pause_all
 *
 * Description:
 *   pause all the CPUs other than the current cpu. Internally, it calls
 *   up_cpu_pause() api to stop all the CPU's.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void up_cpu_pause_all(void)
{
  int me = this_cpu();
  for (int cpu = 0; cpu < CONFIG_SMP_NCPUS; cpu++)
    {
      if (cpu != me)
        {
          /* Pause the CPU */

          up_cpu_pause(cpu);
        }
    }
}

/****************************************************************************
 * Name: up_cpu_resume_all
 *
 * Description:
 *   Resume all the CPUs which were paused earlier.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void up_cpu_resume_all(void)
{
  int me = this_cpu();
  for (int cpu = 0; cpu < CONFIG_SMP_NCPUS; cpu++)
    {
      if (cpu != me)
        {
          /* Resume the CPU */

          up_cpu_resume(cpu);
        }
    }
}

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: arm_pause_handler
 *
 * Description:
 *   This is the handler for the pause SGI.  The requesting CPU waits for
 *   this CPU to pause, so the pause is performed directly here:
 *   up_cpu_paused() does not return until up_cpu_resume() runs on the
 *   requesting CPU.
 *
 * Input Parameters:
 *   Standard interrupt handling
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

static int arm_pause_handler(int irq, FAR void *context, FAR void *arg)
{
  int cpu = this_cpu();

  /* Check for false alarms.  If the pause event has already been processed
   * then g_cpu_paused[cpu] will not be locked.
   */

  if (up_cpu_pausereq(cpu))
    {
      up_cpu_paused(cpu);
    }

  return OK;
}

/****************************************************************************
 * Public Functions (continued)
 ****************************************************************************/

/****************************************************************************
 * Name: amebasmart_cpupause_initialize
 *
 * Description:
 *   Attach the pause SGI handler.  Called once from up_irqinitialize()
 *   on CPU0 before the secondary CPUs are started.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void amebasmart_cpupause_initialize(void)
{
  DEBUGVERIFY(irq_attach(GIC_SMP_PAUSE, arm_pause_handler, NULL));
}

#endif /* CONFIG_SMP */

