/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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
 * Included Files
 ****************************************************************************/

 #include <tinyara/config.h>

#include <stdint.h>
#include <assert.h>
#include <debug.h>
#include <errno.h>

#include <tinyara/arch.h>
#include <tinyara/sched.h>
#include <tinyara/sched_note.h>

#include "up_internal.h"
#include "cp15_cacheops.h"
#include "gic.h"
#include "sched/sched.h"
#include "barriers.h"
#include "smp.h"
#include "arm.h"


#ifdef CONFIG_CPU_HOTPLUG

/****************************************************************************
 * Name: up_cpu_stop
 *
 * Description:
 *   Stop and power down a secondary CPU core. This function performs a
 *   complete CPU shutdown sequence by first hot-plugging the CPU (making it
 *   enter halt state) and then powering it down completely. The CPU will be
 *   removed from the SMP system and can be restarted later with up_cpu_start().
 *
 * Input Parameters:
 *   cpu - The index of the CPU to stop (must be > 0 and < CONFIG_SMP_NCPUS)
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno value on failure.
 *
 * Assumptions:
 *   - Called from a different CPU than the target
 *   - Target CPU is currently running and can be safely stopped
 *   - Scheduler is in a state that allows CPU hot-plug operations
 *
 ****************************************************************************/

int up_cpu_stop(int cpu)
{
    int ret;

    svdbg("Disabling CPU%d\n", cpu);

    DEBUGASSERT(cpu > 0 && cpu < CONFIG_SMP_NCPUS && cpu != this_cpu());

    /* send SGI3 to make core enter hotplug state */
    ret = up_cpu_hotplug(cpu);
    if (ret != OK) {
        svdbg("Failed to hotplug secondary core CPU%d", cpu);
        return ret;
    }

    svdbg("Secondary core hotplug complete CPU%d\n", cpu);

    /* Power down the core completely */
    ret = up_cpu_off(cpu);
    if (ret < 0) {
        svdbg("Failed to powerdown secondary core CPU%d\n", cpu);
        return ret;
    }

    svdbg("Secondary core powerdown complete CPU%d\n", cpu);

    return OK;
}

#endif /* CONFIG_CPU_HOTPLUG */
