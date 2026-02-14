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
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/cpu_driver.h>
#include <tinyara/fs/fs.h>
#include <debug.h>
#include <fcntl.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <sched.h>

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int cpu_ioctl(FAR struct file *filep, int cmd, unsigned long arg);
static int cpu_open(FAR struct file *filep);
static int cpu_close(FAR struct file *filep);
static ssize_t cpu_read(FAR struct file *filep, FAR char *buffer, size_t buflen);
static ssize_t cpu_write(FAR struct file *filep, FAR char *buffer, size_t buflen);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct file_operations g_cpu_fops = {
	.ioctl = cpu_ioctl,
	.open = cpu_open,
	.close = cpu_close,
	.read = cpu_read,
	.write = cpu_write
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int cpu_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	int ret;
	int cpu;
	FAR struct inode *inode;

	DEBUGASSERT(filep != NULL && filep->f_inode != NULL);

	inode = filep->f_inode;

	/* Extract CPU number from device name */
	cpu = inode->i_private;

	switch (cmd) {
	case IOC_CPU_ENABLE:
		/* Call Scheduler API to enable the CPU */
		ret = sched_cpuon(cpu);
		if (ret < 0) {
			smpdbg("ERROR: Failed to enable CPU%d: %d\n", cpu, ret);
			return ret;
		}
		return OK;
	case IOC_CPU_DISABLE:
		/* Call Scheduler API to disable the CPU */
		ret = sched_cpuoff(cpu);
		if (ret < 0) {
			smpdbg("ERROR: Failed to disable CPU%d: %d\n", cpu, ret);
			return ret;
		}
		return OK;
	case IOC_CPU_GET_STATE: {
		cpu_set_t active_cpus = sched_getactivecpu();
		int state;

		if (CPU_ISSET(cpu, &active_cpus)) {
			state = CPU_STATE_RUNNING;
		} else {
			state = CPU_STATE_OFFLINE;
		}

		*(FAR int*)arg = state;
		return OK;
	}
	default:
		return -EINVAL;
	}
}

static int cpu_open(FAR struct file *filep)
{
	DEBUGASSERT(filep != NULL && filep->f_inode != NULL);
	return OK;
}

static int cpu_close(FAR struct file *filep)
{
	DEBUGASSERT(filep != NULL && filep->f_inode != NULL);
	return OK;
}

static ssize_t cpu_read(FAR struct file *filep, FAR char *buffer, size_t buflen)
{
	return OK;
}

static ssize_t cpu_write(FAR struct file *filep, FAR char *buffer, size_t buflen)
{
	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: cpu_driver_init
 *
 * Description:
 *   Initialize the CPU driver by registering device nodes for all secondary
 *   CPUs. This function creates /dev/cpuX device files for each secondary CPU.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   OK (0) on success, negative errno on failure
 *
 ****************************************************************************/

void cpu_driver_init(void)
{
	char devname[16];
	int ret, cpu;
	int ncpus = sched_getcpucount();

	/* Register device nodes for all secondary CPUs */
	for (cpu = 1; cpu < ncpus; cpu++) {
		snprintf(devname, sizeof(devname), "/dev/cpu%d", cpu);

		/* Use CPU number as private data */
		ret = register_driver(devname, &g_cpu_fops, 0666, (FAR void *)cpu);
		if (ret != OK) {
			smpdbg("ERROR: Failed to register %s: %d\n", devname);
			return;
		}

		smpvdbg("CPU driver registered: %s\n", devname);
	}
}
