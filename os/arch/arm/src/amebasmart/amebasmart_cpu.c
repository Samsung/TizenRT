/****************************************************************************
 * arch/arm/src/amebasmart/amebasmart_cpu.c
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

#include <tinyara/arch.h>
#include <tinyara/cpu_state.h>
#include <arch/irq.h>

#include "up_internal.h"
#include "sctlr.h"
#include "smp.h"
#include "scu.h"
#include "gic.h"
#include "mmu.h"
#include "amebasmart_cpu.h"
#include "amebasmart_boot.h"
#include "barriers.h"

#include <tinyara/fs/fs.h>
#include <fcntl.h>
#include <errno.h>
#include <debug.h>



#ifdef CONFIG_SMP

/****************************************************************************
 * Private Types
 ****************************************************************************/


/****************************************************************************
 * Private Data
 ****************************************************************************/


/****************************************************************************
 * Public Data
 ****************************************************************************/

/* Symbols defined via the linker script */


/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/


/****************************************************************************
 * Name: cpu__disable
 *
 * Description:
 *   Called from CPU0 to make sure that all other CPUs are in the disabled
 *   state.  This is a formality because the other CPUs are actually running
 *   then we have probably already crashed.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static int cpu__disable(int cpu_id)
{
	smp_full_powerdown_cpu();
	lldbg("CPU%d: disable\n", cpu_id);
	return 0;
}


static int cpu__enable(int cpu_id)
{
	vPortSecondaryStart();
	lldbg("CPU%d: enable\n", cpu_id);
	return 0;
}

static int cpu__hotplug(int cpu_id)
{
	up_cpu_hotplug(cpu_id);
	lldbg("CPU%d: hotplug\n", cpu_id);
	return 0;
}

static int cpu__wakeup(int cpu_id)
{
	lldbg("CPU%d: wakeup\n", cpu_id);
	return 0;
}

static int cpu__teardown(int cpu_id)
{
        lldbg("CPU%d: teardown\n", cpu_id);
        return 0;
}


const struct cpu_arch_ops_s g_cpu_arch_ops = {
    .enable                  = cpu__enable,
    .disable                 = cpu__disable,
    .hotplug                 = cpu__hotplug,
    .wakeup		     = cpu__wakeup,
    .teardown		     = cpu__teardown,
};


const struct cpu_arch_ops_s * up_cpuinitialize(void)
{
    lldbg("[cpu hal] initialized\n");
    return &g_cpu_arch_ops;
}



#endif /* CONFIG_SMP */
