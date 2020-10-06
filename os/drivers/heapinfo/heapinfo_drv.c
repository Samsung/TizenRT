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
#include <debug.h>
#include <tinyara/heapinfo_drv.h>
#include <tinyara/mm/mm.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>

#ifdef CONFIG_MM_KERNEL_HEAP
extern struct mm_heap_s g_kmmheap[CONFIG_KMM_NHEAPS];
#endif
/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int heapinfo_ioctl(FAR struct file *filep, int cmd, unsigned long arg);
static ssize_t heapinfo_read(FAR struct file *filep, FAR char *buffer, size_t len);
static ssize_t heapinfo_write(FAR struct file *filep, FAR const char *buffer, size_t len);

/****************************************************************************
 * Private Data
 ****************************************************************************/

struct heapinfo_dev_s {
	mqd_t mqdes;	   /* A mqueue descriptor */
};

static const struct file_operations heapinfo_fops = {
	0,                          /* open */
	0,                          /* close */
	heapinfo_read,               /* read */
	heapinfo_write,              /* write */
	0,                          /* seek */
	heapinfo_ioctl               /* ioctl */
#ifndef CONFIG_DISABLE_POLL
	, 0                         /* poll */
#endif
};
/****************************************************************************
 * Private Functions
 ****************************************************************************/
static ssize_t heapinfo_read(FAR struct file *filep, FAR char *buffer, size_t len)
{
	return 0;
}

static ssize_t heapinfo_write(FAR struct file *filep, FAR const char *buffer, size_t len)
{
	return 0;
}

/************************************************************************************
 * Name: heapinfo_ioctl
 *
 * Description: The ioctl method for heapinfo.
 *
 ************************************************************************************/
static int heapinfo_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	int ret = ERROR;
	struct mm_heap_s *heap = NULL;
	heapinfo_option_t *option;

	/* Handle built-in ioctl commands */

	switch (cmd) {
	case HEAPINFOIOC_PARSE:
		option = (heapinfo_option_t *)arg;
		switch (option->heap_type) {
		case HEAPINFO_HEAP_TYPE_KERNEL:
			heap = g_kmmheap;
			break;
#ifdef CONFIG_APP_BINARY_SEPARATION
		case HEAPINFO_HEAP_TYPE_BINARY:
			heap = mm_get_app_heap_with_name(option->app_name);
			break;
#endif
		default:
			break;
		}
		if (heap == NULL) {
			return ERROR;
		}
		if (option->mode == HEAPINFO_INIT_PEAK) {
			heap->peak_alloc_size = 0;
			return OK;
		}
		heapinfo_parse_heap(heap, option->mode, option->pid);
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
 * Name: heapinfo_drv_register
 *
 * Description:
 *   Register heapinfo driver path, HEAPINFO_DRVPATH
 *
 ****************************************************************************/

void heapinfo_drv_register(void)
{
	(void)register_driver(HEAPINFO_DRVPATH, &heapinfo_fops, 0666, NULL);
}
