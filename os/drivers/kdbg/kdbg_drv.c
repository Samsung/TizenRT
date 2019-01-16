/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
#include <string.h>
#include <sys/types.h>
#include <tinyara/sched.h>
#include <tinyara/mm/mm.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/kdbg_drv.h>

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int kdbg_ioctl(FAR struct file *filep, int cmd, unsigned long arg);
static ssize_t kdbg_read(FAR struct file *filep, FAR char *buffer, size_t len);
static ssize_t kdbg_write(FAR struct file *filep, FAR const char *buffer, size_t len);

/****************************************************************************
 * Private Data
 ****************************************************************************/

struct kdbg_dev_s {
	mqd_t mqdes;	   /* A mqueue descriptor */
};

static const struct file_operations kdbg_fops = {
	0,                          /* open */
	0,                          /* close */
	kdbg_read,               /* read */
	kdbg_write,              /* write */
	0,                          /* seek */
	kdbg_ioctl               /* ioctl */
#ifndef CONFIG_DISABLE_POLL
	, 0                         /* poll */
#endif
};
/****************************************************************************
 * Private Functions
 ****************************************************************************/
static ssize_t kdbg_read(FAR struct file *filep, FAR char *buffer, size_t len)
{
	return 0;
}

static ssize_t kdbg_write(FAR struct file *filep, FAR const char *buffer, size_t len)
{
	return 0;
}

/************************************************************************************
 * Name: kdbg_ioctl
 *
 * Description:  The ioctl method for kernel debug.
 *
 ************************************************************************************/
static int kdbg_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	int ret = ERROR;
	struct tcb_s *tcb;
	heapinfo_data_t *data;

	/* Handle built-in ioctl commands */

	switch (cmd) {
	case KDBGIOC_HEAPINFO_ADDSIZE:
		data = (heapinfo_data_t *)arg;
		tcb = sched_gettcb(data->pid);
		if (tcb) {
			tcb->curr_alloc_size += data->size;
			tcb->num_alloc_free++;
			if (tcb->curr_alloc_size > tcb->peak_alloc_size) {
				tcb->peak_alloc_size = tcb->curr_alloc_size;
			}
			ret = OK;
		}
		break;
	case KDBGIOC_HEAPINFO_SUBSIZE:
		data = (heapinfo_data_t *)arg;
		tcb = sched_gettcb(data->pid);
		if (tcb) {
			tcb->curr_alloc_size -= data->size;
			tcb->num_alloc_free--;
			ret = OK;
		}
		break;
	case KDBGIOC_HEAPINFO_EXCLUDE_STACKSIZE:
		tcb = sched_gettcb((int)arg);
		ASSERT(tcb);
		tcb->curr_alloc_size -= arg;
		ret = OK;
		break;
	default:
		break;
	}
	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: kdbg_drv_register
 *
 * Description:
 *   Register kdbg path, KDBG_DRVPATH
 *
 ****************************************************************************/

void kdbg_drv_register(void)
{
	(void)register_driver(KDBG_DRVPATH, &kdbg_fops, 0666, NULL);
}
