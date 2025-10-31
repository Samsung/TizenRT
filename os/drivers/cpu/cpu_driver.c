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
#include <tinyara/cpu_driver.h>
#include <tinyara/fs/fs.h>
#include <debug.h>
#include <fcntl.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>


/* Static device struct for Secondary cpus */
static struct cpu_dev_s {
    int cpu;
    const struct cpu_arch_ops_s *ops;
} g_cpu_dev[CONFIG_SMP_NCPUS];

static int cpu_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
    FAR struct cpu_dev_s *dev;
    const struct cpu_arch_ops_s *ops;

    DEBUGASSERT(filep != NULL && filep->f_inode != NULL);

    dev = filep->f_inode->i_private;
    if (!dev) {
        return -ENODEV;
    }

    ops = dev->ops;
    if (!ops) {
        return -EINVAL;
    }

    switch (cmd) {
    case IOC_CPU_ENABLE:
                return up_cpu_on(dev->cpu);
    case IOC_CPU_DISABLE:
                /* For testing purpose, the state is set to CPU_SCHED_OFF before calling up_cpu_off
                 * It should be set by the kernel team after migrating scheduled tasks,
                 * Before calling up_cpu_off().
                 */
                cpu_set_state(dev->cpu, CPU_SCHED_OFF);

                return up_cpu_off(dev->cpu);
    case IOC_CPU_HOTPLUG:
                if (!ops->hotplug) {
                    lldbg("ERROR: hotplug operation not supported\n");
                    return -ENOSYS;
                }

		        /* For testing purpose, the state is set to CPU_SCHED_OFF before calling hotplug.
                 * It should be set by the kernel team after migrating scheduled tasks,
                 * Before calling hotplug
                 */
                cpu_set_state(dev->cpu, CPU_SCHED_OFF);

		        return ops->hotplug(dev->cpu);
    case IOC_CPU_GET_STATE: {
                cpu_state_t state = cpu_get_state(dev->cpu);
                if (state < CPU_RUNNING || state > CPU_SCHED_OFF) {
                    lldbg("ERROR: Invalid CPU state %d for CPU%d\n", state, dev->cpu);
                    return -EINVAL;
                }

                *(FAR cpu_state_t *)arg = state;
                return OK;
            }
    default:
                return -EINVAL;
    }

}

static int cpu_open(FAR struct file *filep)
{
	FAR struct inode *inode;
	FAR struct cpu_dev_s *dev;

	DEBUGASSERT(filep != NULL && filep->f_inode != NULL);
	inode = filep->f_inode;

	dev = (FAR struct cpu_dev_s *)inode->i_private;
	DEBUGASSERT(dev);

	return OK;
}

static int cpu_close(FAR struct file *filep)
{
	FAR struct inode *inode;
	FAR struct cpu_dev_s *dev;

	DEBUGASSERT(filep != NULL && filep->f_inode != NULL);
	inode = filep->f_inode;

	dev = (FAR struct cpu_dev_s *)inode->i_private;
	DEBUGASSERT(dev);

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


static const struct file_operations g_cpu_fops = {
    .ioctl = cpu_ioctl,
    .open = cpu_open,
    .close = cpu_close,
    .read = cpu_read,
    .write = cpu_write
};

int cpu_driver_register(int cpu, const struct cpu_arch_ops_s *ops)
{
    char devname[16];

    if (!ops) {
        return -EINVAL;
    }

    if (cpu < 1 || cpu >= CONFIG_SMP_NCPUS) {
        lldbg("[driver] cpu number out of range\n");
        return -ENOTSUP;
    }

    g_cpu_dev[cpu].cpu = cpu;
    g_cpu_dev[cpu].ops = ops;

    snprintf(devname, sizeof(devname), "/dev/cpu%d", cpu);

    return register_driver(devname, &g_cpu_fops, 0666, &g_cpu_dev[cpu]);
}
