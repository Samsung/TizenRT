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
		if (option->mode == HEAPINFO_DUMP_HEAP) {
			heapinfo_dump_heap(heap);
			return OK;
		}
		if (option->mode == HEAPINFO_INIT_PEAK) {
			heap->peak_alloc_size = 0;
			return OK;
		}
#if CONFIG_KMM_NHEAPS > 1
		if (option->heap_type == HEAPINFO_HEAP_TYPE_KERNEL) {
			int heap_idx;
			for (heap_idx = HEAP_START_IDX; heap_idx <= HEAP_END_IDX; heap_idx++) {
				printf("\n [HEAP %d]\n", heap_idx);
				heapinfo_parse_heap(&heap[heap_idx], option->mode, option->pid);
			}
		} else
#endif
		{
			heapinfo_parse_heap(heap, option->mode, option->pid);
		}
		ret = OK;
		break;
#endif
#ifdef CONFIG_APP_BINARY_SEPARATION
	case MMINFOIOC_MNG_ALLOCFAIL:
		/* There is a single heap for user. So start and end indexes of heap are always 0. */
		mm_manage_alloc_fail(BASE_HEAP, 0, 0, ((struct mm_alloc_fail_s *)arg)->size, USER_HEAP
#if defined(CONFIG_DEBUG_MM_HEAPINFO)
				, ((struct mm_alloc_fail_s *)arg)->caller
#endif
				);
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
