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
#include <errno.h>
#include <stdlib.h>

#include <tinyara/kmalloc.h>
#include <tinyara/mminfo.h>
#include <tinyara/mm/mm.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>

#if defined(CONFIG_DEBUG_MM_HEAPINFO) && defined(CONFIG_MM_KERNEL_HEAP)
extern struct mm_heap_s g_kmmheap[CONFIG_KMM_NHEAPS];
#endif
/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int mminfo_ioctl(FAR struct file *filep, int cmd, unsigned long arg);
static ssize_t mminfo_read(FAR struct file *filep, FAR char *buffer, size_t len);
static ssize_t mminfo_write(FAR struct file *filep, FAR const char *buffer, size_t len);

/****************************************************************************
 * Private Data
 ****************************************************************************/
static const struct file_operations mminfo_fops = {
	0,                          /* open */
	0,                          /* close */
	mminfo_read,               /* read */
	mminfo_write,              /* write */
	0,                          /* seek */
	mminfo_ioctl               /* ioctl */
#ifndef CONFIG_DISABLE_POLL
	, 0                         /* poll */
#endif
};
/****************************************************************************
 * Private Functions
 ****************************************************************************/
static ssize_t mminfo_read(FAR struct file *filep, FAR char *buffer, size_t len)
{
	return 0;
}

static ssize_t mminfo_write(FAR struct file *filep, FAR const char *buffer, size_t len)
{
	return 0;
}

/************************************************************************************
 * Name: mminfo_ioctl
 *
 * Description: The ioctl method for mminfo.
 *
 ************************************************************************************/
static int mminfo_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	int ret = -ENOSYS;
	struct mallinfo mem;
	struct mallinfo *retmem;
#if defined(CONFIG_DEBUG_MM_HEAPINFO)
	heapinfo_option_t *option;
	struct mm_heap_s *heap = NULL;
#endif

	/* Handle built-in ioctl commands */

	switch (cmd) {
	case MMINFOIOC_HEAP:
		retmem = (struct mallinfo *)arg;
#ifdef CONFIG_MM_KERNEL_HEAP
#ifdef CONFIG_CAN_PASS_STRUCTS
		mem = kmm_mallinfo();
#else
		(void)kmm_mallinfo(&mem);
#endif
#else
#ifdef CONFIG_CAN_PASS_STRUCTS
		mem = kumm_mallinfo();
#else
		(void)kumm_mallinfo(&mem);
#endif
#endif
		*retmem = mem;
		ret = OK;
		break;
	case MMINFOIOC_PARSE:
#if defined(CONFIG_DEBUG_MM_HEAPINFO)
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
			return -EINVAL;
		}
		if (option->mode == HEAPINFO_INIT_PEAK) {
			heap->peak_alloc_size = 0;
			return OK;
		}
		heapinfo_parse_heap(heap, option->mode, option->pid);
		ret = OK;
		break;
#endif
	default:
		mdbg("Not supported\n");
		break;
	}
	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mminfo_register
 *
 * Description:
 *   Register mminfo driver path, MMINFO_DRVPATH
 *
 ****************************************************************************/

void mminfo_register(void)
{
	(void)register_driver(MMINFO_DRVPATH, &mminfo_fops, 0666, NULL);
}
