#include <tinyara/config.h>
#include <tinyara/cpu_driver.h>
#include <tinyara/fs/fs.h>
#include <debug.h>
#include <fcntl.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>


/* Static device struct for cpu1 */
static struct cpu_dev_s {
    int cpu;
    const struct cpu_arch_ops_s *ops;
} g_cpu1_dev;

static int cpu_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
    FAR struct cpu_dev_s *dev = filep->f_inode->i_private;
    const struct cpu_arch_ops_s *ops = dev->ops;
    int ret;

    switch (cmd) {
    case IOC_CPU_ENABLE:
                // cpu_enable() now handles state and calling ops->enable()
                return cpu_enable(1);
    case IOC_CPU_DISABLE:
                // cpu_disable() now handles state and calling ops->disable()
                return cpu_disable(1);
    case IOC_CPU_HOTPLUG:
                ret = cpu_set_state(dev->cpu, CPU_HOTPLUG);
                if (ret != OK) {
                        return ret;
                }
                return ops->hotplug(dev->cpu);
    case IOC_CPU_WAKEUP:
                ret = cpu_set_state(dev->cpu, CPU_WAKEUP);
                if (ret != OK) {
                        return ret;
                }
                return ops->wakeup(dev->cpu);
    case IOC_CPU_TEARDOWN:
                ret = cpu_set_state(dev->cpu, CPU_TEARDOWN);
                if (ret != OK) {
                        return ret;
                }
                return ops->teardown(dev->cpu);
    case IOC_CPU_GET_STATE: {
                cpu_state_t state = cpu_get_state(dev->cpu);
                if (ret != OK) {
                    return ret;
                }
                *(FAR cpu_state_t *)arg = state;
                return OK;
            }
    case IOC_CPU_SET_STATE: {
                cpu_state_t new_state = *(FAR cpu_state_t *)arg;
                return cpu_set_state(dev->cpu, new_state);
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

    if (cpu != 1) {
        lldbg("[driver] only cpu1 supported\n");
        return -ENOTSUP;
    }

    g_cpu1_dev.cpu = cpu;
    g_cpu1_dev.ops = ops;

    snprintf(devname, sizeof(devname), "/dev/cpu%d", cpu);

    return register_driver(devname, &g_cpu_fops, 0666, &g_cpu1_dev);
}
