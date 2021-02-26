/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <crc32.h>
#ifdef CONFIG_APP_BINARY_SEPARATION
#include <queue.h>
#include <stdint.h>
#ifdef CONFIG_VERSION_STRING
#include <stdlib.h>
#endif
#include <tinyara/sched.h>
#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
#include <tinyara/binfmt/binfmt.h>
#endif
#endif
#include <tinyara/mm/mm.h>
#include <tinyara/binary_manager.h>

#ifdef CONFIG_APP_BINARY_SEPARATION
#include "sched/sched.h"
#endif
#include "binary_manager/binary_manager.h"

/****************************************************************************
 * Private Definitions
 ****************************************************************************/
#ifdef CONFIG_APP_BINARY_SEPARATION
/* Kernel version (not implemented, it will be modified) */
#ifdef CONFIG_VERSION_STRING
#define KERNEL_VER               atof(CONFIG_VERSION_STRING)
#else
#define KERNEL_VER               3.0
#endif

/* Table for User binaries */
/* Binary table, the first data [0] is for Common Library. */
static binmgr_uinfo_t bin_table[USER_BIN_COUNT + 1];
static uint32_t g_bin_count;
#endif

/* Data for Kernel partitions */
static binmgr_kinfo_t kernel_info;

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int binary_manager_verify_kernel_binary(char *path, kernel_binary_header_t *header_data)
{
	int fd;
	int ret;
	uint32_t crc_value = 0;
	uint8_t *crc_buffer;
	uint32_t crc_bufsize;

	memset(header_data, 0, sizeof(kernel_binary_header_t));
	crc_buffer = NULL;

	fd = open(path, O_RDONLY);
	if (fd < 0) {
		bmdbg("Failed to open %s: %d, errno %d\n", path, ret, errno);
		return ERROR;
	}

	/* Read the binary header */
	ret = read(fd, (FAR uint8_t *)header_data, sizeof(kernel_binary_header_t));
	if (ret != sizeof(kernel_binary_header_t)) {
		bmdbg("Failed to read %s: %d, errno %d\n", path, ret, errno);
		goto errout_with_fd;
	}

	/* Verify header data */
	if (header_data->header_size == 0 || header_data->binary_size == 0 \
		|| header_data->version < KERNEL_BIN_VER_MIN || header_data->version > KERNEL_BIN_VER_MAX) {
		bmdbg("Invalid header data : headersize %u, version %u, binary size %u\n", header_data->header_size, header_data->version, header_data->binary_size);
		goto errout_with_fd;
	}

	crc_bufsize = header_data->binary_size;
	crc_buffer = (uint8_t *)kmm_malloc(crc_bufsize);
	if (!crc_buffer) {
		bmdbg("Failed to allocate buffer for checking crc, size %u\n", crc_bufsize);
		goto errout_with_fd;
	}

	/* Calculate checksum and Verify it */
	crc_value = crc32part((uint8_t *)header_data + CHECKSUM_SIZE, header_data->header_size, crc_value);
	ret = read(fd, (void *)crc_buffer, crc_bufsize);
	if (ret < 0 || ret != crc_bufsize) {
		bmdbg("Failed to read : %d, errno %d\n", ret, errno);
		goto errout_with_fd;
	}
	crc_value = crc32part(crc_buffer, crc_bufsize, crc_value);

	if (crc_value != header_data->crc_hash) {
		bmdbg("Failed to crc check : %u != %u\n", crc_value, header_data->crc_hash);
		goto errout_with_fd;
	}
	kmm_free(crc_buffer);
	bmvdbg("Binary header : %u %u %u %u \n", header_data->header_size, header_data->binary_size, header_data->version, header_data->secure_header_size);
	close(fd);

	return OK;

errout_with_fd:
	close(fd);
	if (crc_buffer) {
		kmm_free(crc_buffer);
	}
	return ERROR;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: partitions_get_kcount
 *
 * Description:
 *	 This function gets the number of registered partitions for kernel.
 *
 ****************************************************************************/
uint32_t binary_manager_get_kcount(void)
{
	return kernel_info.part_count;
}

/****************************************************************************	
 * Name: binary_manager_get_kdata	
 *	
 * Description:	
 *	 This function gets a kernel data.	
 *	
 ****************************************************************************/	
binmgr_kinfo_t *binary_manager_get_kdata(void)	
{	
	return &kernel_info;	
}

void binary_manager_register_kpart(int part_num, int part_size)
{
	if (part_num < 0 || part_size <= 0 || kernel_info.part_count >= KERNEL_BIN_COUNT) {
		bmdbg("ERROR: Invalid part info : num %d, size %d\n", part_num, part_size);
		return;
	}

	kernel_info.part_size[kernel_info.part_count] = part_size;
	kernel_info.part_num[kernel_info.part_count] = part_num;

	bmvdbg("[KERNEL %d] part num %d size %d\n", kernel_info.part_count, part_num, part_size);

	kernel_info.part_count++;
}

/****************************************************************************
 * Name: binary_manager_scan_kbin
 *
 * Description:
 *	 This function scans kernel binaries and update information.
 *
 ****************************************************************************/
bool binary_manager_scan_kbin(void)
{
	int ret;
	int part_idx;
	uint32_t latest_ver;
	kernel_binary_header_t header_data;
	char filepath[BINARY_PATH_LEN];

	latest_ver = 0;

	/* Load the binaries with high priority directly */
	for (part_idx = 0; part_idx < kernel_info.part_count; part_idx++) {
		snprintf(filepath, BINARY_PATH_LEN, BINMGR_DEVNAME_FMT, kernel_info.part_num[part_idx]);
		ret = binary_manager_verify_kernel_binary(filepath, &header_data);
		if (ret == OK && latest_ver < header_data.version) {
			/* Update latest version and inuse index */
			kernel_info.version = header_data.version;
			kernel_info.inuse_idx = part_idx;
			latest_ver = header_data.version;
		}
	}
	bmvdbg("Latest version [%u] %u\n", kernel_info.inuse_idx, kernel_info.version);

	/* Found valid binary */
	if (kernel_info.version != 0) {
		return true;
	}
	return false;
}

#ifdef CONFIG_APP_BINARY_SEPARATION
/****************************************************************************
 * Name: binary_manager_get_ucount
 *
 * Description:
 *	 This function gets the number of user binaries.
 *
 ****************************************************************************/
uint32_t binary_manager_get_ucount(void)
{
	return g_bin_count;
}

/****************************************************************************
 * Name: binary_manager_get_udata
 *
 * Description:
 *	 This function gets a row of binary table with bin_idx.
 *
 ****************************************************************************/
binmgr_uinfo_t *binary_manager_get_udata(uint32_t bin_idx)
{
	return &bin_table[bin_idx];
}

/****************************************************************************
 * Name: binary_manager_register_ubin
 *
 * Description:
 *	 This function registers user binaries.
 *
 ****************************************************************************/
int binary_manager_register_ubin(char *name, uint32_t version, uint8_t load_priority)
{
	if (name == NULL || g_bin_count >= USER_BIN_COUNT) {
		bmdbg("ERROR: Invalid parameter\n");
		return ERROR;
	}

	/* Initialize binary table and register binary name, version and priority */
	g_bin_count++;
	BIN_ID(g_bin_count) = -1;
	BIN_FILECNT(g_bin_count) = 1;
	BIN_RTLIST(g_bin_count) = NULL;
	BIN_NRTLIST(g_bin_count) = NULL;
	BIN_STATE(g_bin_count) = BINARY_INACTIVE;
	BIN_VER(g_bin_count, 0) = version;
	BIN_LOAD_PRIORITY(g_bin_count, 0) = load_priority;
	strncpy(BIN_NAME(g_bin_count), name, BIN_NAME_MAX - 1);
	BIN_NAME(g_bin_count)[BIN_NAME_MAX - 1] = '\0';
	sq_init(&BIN_CBLIST(g_bin_count));

	bmvdbg("[USER %d] %s\n", g_bin_count, BIN_NAME(g_bin_count));

	return g_bin_count;
}

/****************************************************************************
 * Name: binary_manager_update_running_state
 *
 * Description:
 *	 This function update binary state to BINARY_RUNNING state.
 *   And notify other binaries changed state.
 *
 ****************************************************************************/
void binary_manager_update_running_state(int bin_id)
{
	int bin_idx;
	struct tcb_s *tcb;

	tcb = sched_gettcb(bin_id);
	if (tcb && tcb->group && tcb->group->tg_binidx > 0) {
		bin_idx = tcb->group->tg_binidx;
		BIN_STATE(bin_idx) = BINARY_RUNNING;
		bmvdbg("binary '%s' state is changed, state = %d.\n", BIN_NAME(bin_idx), BIN_STATE(bin_idx));

		/* Notify that binary is started. */
		binary_manager_notify_state_changed(bin_idx, BINARY_STARTED);
	}
}

/****************************************************************************
 * Name: binary_manager_add_binlist
 *
 * Description:
 *	 This function adds tcb to binary list.
 *
 ****************************************************************************/
void binary_manager_add_binlist(FAR struct tcb_s *tcb)
{
	int bin_idx;

	if (!tcb) {
		bmdbg("ERROR: tcb parameter is NULL\n");
		return;
	}

	if (!tcb->group) {
		/* It is possible that the task / pthread has already exited at the time of calling this function.
		 * In this case, the group member will be NULL and will cause an exception if we try to dereference
		 * the pointer. So, perform a null check here. If group is null, it is ok to not add the tcb to our
		 * list because, task has already exited.
		 */
		bmdbg("Failed to add pid %d to binlist. This task has already exited and group is NULL\n", tcb->pid);
		return;
	}

	bin_idx = tcb->group->tg_binidx;

	/* A binary index, bin_idx is greater than 0 only if tcb is a thread of user binary.
	 * Add tcb to the binary list for fault recovery in case of threads of user binary.
	 * Otherwise, it is unnecessary for idle or kernel threads. */
	if (bin_idx > 0) {
		/* Add a tcb to a head of list */
		if (tcb->sched_priority > BM_PRIORITY_MAX) {
			if (BIN_RTLIST(bin_idx)) {
				tcb->bin_flink = BIN_RTLIST(bin_idx);
				BIN_RTLIST(bin_idx)->bin_blink = tcb;
			}
			BIN_RTLIST(bin_idx) = tcb;
		} else {
			if (BIN_NRTLIST(bin_idx)) {
				tcb->bin_flink = BIN_NRTLIST(bin_idx);
				BIN_NRTLIST(bin_idx)->bin_blink = tcb;
			}
			BIN_NRTLIST(bin_idx) = tcb;
		}
	}
}

/****************************************************************************
 * Name: binary_manager_remove_binlist
 *
 * Description:
 *	 This function removes tcb from binary list.
 *
 ****************************************************************************/
void binary_manager_remove_binlist(FAR struct tcb_s *tcb)
{
	int bin_idx;
	struct tcb_s *prev;
	struct tcb_s *next;

	bin_idx = tcb->group->tg_binidx;
	if (bin_idx > 0) {
		/* Remove a tcb from the thread list of binary */
		prev = tcb->bin_blink;
		next = tcb->bin_flink;
		if (!prev) {
			if (tcb->sched_priority > BM_PRIORITY_MAX) {
				BIN_RTLIST(bin_idx) = next;
			} else {
				BIN_NRTLIST(bin_idx) = next;
			}
		} else {
			prev->bin_flink = next;
		}
		if (next) next->bin_blink = prev;
	}
}

/****************************************************************************
 * Name: binary_manager_clear_bindata
 *
 * Description:
 *	 This function clears data of binary with bin_idx in a binary table.
 *
 ****************************************************************************/
void binary_manager_clear_bindata(int bin_idx)
{
	struct tcb_s *tcb;

	if (bin_idx < 0 || bin_idx > USER_BIN_COUNT) {
		return;
	}

#ifdef CONFIG_OPTIMIZE_APP_RELOAD_TIME
	tcb = sched_gettcb(BIN_ID(bin_idx));
	/* If 'reload' flag is true, a binary manager uses loading data 'bininfo' when loading the binary later.
	 * So clear BIN_LOADINFO which points loading data if flag is false.
	 */
	if (tcb && ((struct task_tcb_s *)tcb)->bininfo && !((struct task_tcb_s *)tcb)->bininfo->reload) {
		BIN_LOADINFO(bin_idx) = NULL;
	}
#endif /* CONFIG_OPTIMIZE_APP_RELOAD_TIME */
	BIN_STATE(bin_idx) = BINARY_INACTIVE;
	BIN_ID(bin_idx) = -1;
	BIN_RTLIST(bin_idx) = NULL;
	BIN_NRTLIST(bin_idx) = NULL;
	/* Clear registered callbacks of binary */
	binary_manager_clear_bin_statecb(bin_idx);
}
#endif
