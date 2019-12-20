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
#include <string.h>
#include <queue.h>
#include <tinyara/sched.h>
#include <tinyara/binary_manager.h>

#include "sched/sched.h"
#include "binary_manager/binary_manager.h"

/****************************************************************************
 * Private Definitions
 ****************************************************************************/
/* Kernel version (not implemented, it will be modified) */
#ifdef CONFIG_VERSION_STRING
#define KERNEL_VER               CONFIG_VERSION_STRING
#else
#define KERNEL_VER               "2.0"
#endif

/* Binary table, the first data [0] is for kernel. */
static binmgr_bininfo_t bin_table[BINARY_COUNT];
static uint32_t g_bin_count;

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: binary_manager_get_binary_count
 *
 * Description:
 *	 This function gets the number of user binaries.
 *
 ****************************************************************************/
uint32_t binary_manager_get_binary_count(void)
{
	return g_bin_count;
}

/****************************************************************************
 * Name: binary_manager_get_binary_data
 *
 * Description:
 *	 This function gets a row of binary table with bin_idx.
 *
 ****************************************************************************/
binmgr_bininfo_t *binary_manager_get_binary_data(uint32_t bin_idx)
{
	return &bin_table[bin_idx];
}

/****************************************************************************
 * Name: binary_manager_register_partition
 *
 * Description:
 *	 This function registers partitions of kernel or user binaries.
 *
 ****************************************************************************/
void binary_manager_register_partition(int part_num, int part_type, char *name, int part_size)
{
	int bin_idx;

	if (part_num < 0 || part_size <= 0 || part_type < 0 || part_type >= BINMGR_PART_MAX) {
		bmdbg("ERROR: Invalid part info : num %d, type %d size %d\n", part_num, part_type, part_size);
		return;
	}

	/* Check partition type and register it */
	if (part_type == BINMGR_PART_KERNEL) {
		if (BIN_PARTSIZE(KERNEL_IDX, 0) > 0) {
			/* Already registered first kernel partition, register it as second partition. */
			BIN_PARTSIZE(KERNEL_IDX, 1) = part_size;
			BIN_PARTNUM(KERNEL_IDX, 1) = part_num;
		} else {
			BIN_USEIDX(KERNEL_IDX) = 0;
			BIN_STATE(KERNEL_IDX) = BINARY_RUNNING;
			BIN_PARTNUM(KERNEL_IDX, 0) = part_num;
			BIN_PARTSIZE(KERNEL_IDX, 0) = part_size;
			strncpy(BIN_VER(KERNEL_IDX), KERNEL_VER, KERNEL_VER_MAX);
			strncpy(BIN_KERNEL_VER(KERNEL_IDX), KERNEL_VER, KERNEL_VER_MAX);
			strncpy(BIN_NAME(KERNEL_IDX), "kernel", BIN_NAME_MAX);
		}
		bmvdbg("[KERNEL] part num %d size %d\n", part_num, part_size);
	} else {
		/* Else, It is user binary partition. First, Find the partition in the list. */
		for (bin_idx = 1; bin_idx <= g_bin_count; bin_idx++) {
			/* Found in the list, then register it as second partition */
			if (!strncmp(BIN_NAME(bin_idx), name, strlen(name) + 1)) {
				BIN_PARTNUM(bin_idx, 1) = part_num;
				BIN_PARTSIZE(bin_idx, 1) = part_size;
				bmvdbg("[USER%d : 2] %s size %d num %d\n", bin_idx, BIN_NAME(bin_idx), BIN_PARTSIZE(bin_idx, 1), BIN_PARTNUM(bin_idx, 1));
				return;
			}
		}
		/* No, Register it as a new user partition */
		g_bin_count++;
		BIN_ID(g_bin_count) = -1;
		BIN_RTCOUNT(g_bin_count) = 0;
		BIN_STATE(g_bin_count) = BINARY_INACTIVE;
		BIN_PARTNUM(g_bin_count, 0) = part_num;
		BIN_PARTSIZE(g_bin_count, 0) = part_size;
		strncpy(BIN_NAME(g_bin_count), name, BIN_NAME_MAX);
		sq_init(&BIN_CBLIST(g_bin_count));
		bmvdbg("[USER%d : 1] %s size %d num %d\n", g_bin_count, BIN_NAME(g_bin_count), BIN_PARTSIZE(g_bin_count, 0), BIN_PARTNUM(g_bin_count, 0));
	}
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

	if (bin_id <= 0) {
		bmdbg("Invalid parameter: bin id %d\n", bin_id);
		return;
	}

	bin_idx = binary_manager_get_index_with_binid(bin_id);
	if (bin_idx < 0) {
		bmdbg("Failed to get index of binary %d\n", bin_id);
		return;
	}

	BIN_STATE(bin_idx) = BINARY_RUNNING;
	bmvdbg("binary '%s' state is changed, state = %d.\n", BIN_NAME(bin_idx), BIN_STATE(bin_idx));

	/* Notify that binary is started. */
	binary_manager_notify_state_changed(bin_idx, BINARY_STARTED);
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
	struct tcb_s *rtcb;
	struct tcb_s *next;

	rtcb = this_task();
	next = rtcb->bin_flink;
	tcb->bin_blink = rtcb;
	tcb->bin_flink = next;
	if (next) next->bin_blink = tcb;
	rtcb->bin_flink = tcb;
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
	struct tcb_s *prev;
	struct tcb_s *next;

	prev = tcb->bin_blink;
	next = tcb->bin_flink;
	if (prev) prev->bin_flink = next;
	if (next) next->bin_blink = prev;
}
