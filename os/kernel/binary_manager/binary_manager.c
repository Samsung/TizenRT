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
#include <stdio.h>
#include <mqueue.h>
#include <errno.h>
#include <debug.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <tinyara/binary_manager.h>

#include "binary_manager.h"

/****************************************************************************
 * Private Definitions
 ****************************************************************************/
/* Index of 'kernel' data in binary table, bin_table. */
#define KERNEL_IDX               0

/* Kernel version (not implemented, it will be modified) */
#ifdef CONFIG_VERSION_STRING
#define KERNEL_VER               CONFIG_VERSION_STRING
#else
#define KERNEL_VER               "2.0"
#endif

/* Binary table, the first data [0] is for kernel. */
static binmgr_bininfo_t bin_table[BINARY_COUNT];
static uint32_t g_bin_count;
static mqd_t g_binmgr_mq_fd;

/****************************************************************************
 * Public Functions
 ****************************************************************************/
#ifdef CONFIG_BINMGR_RECOVERY
/* Get a mqfd of binary manager for binary recovery */
mqd_t binary_manager_get_mqfd(void)
{
	return g_binmgr_mq_fd;
}
#endif

/* Get the number of user binaries */
uint32_t binary_manager_get_binary_count(void)
{
	return g_bin_count;
}

/* Get a row of binary table with bin_idx */
binmgr_bininfo_t *binary_manager_get_binary_data(uint32_t bin_idx)
{
	return &bin_table[bin_idx];
}

/* Register partitions : kernel, loadparam, user binaries */
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
		BIN_STATE(g_bin_count) = BINARY_INACTIVE;
		BIN_PARTNUM(g_bin_count, 0) = part_num;
		BIN_PARTSIZE(g_bin_count, 0) = part_size;
		strncpy(BIN_NAME(g_bin_count), name, BIN_NAME_MAX);
		sq_init(&BIN_CBLIST(g_bin_count));
		bmvdbg("[USER%d : 1] %s size %d num %d\n", g_bin_count, BIN_NAME(g_bin_count), BIN_PARTSIZE(g_bin_count, 0), BIN_PARTNUM(g_bin_count, 0));
	}
}

/* Update binary state to BINARY_RUNNING state */
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
 * Main Function
 ****************************************************************************/
int binary_manager(int argc, char *argv[])
{
	int ret;
	int nbytes;
#if !defined(CONFIG_DISABLE_SIGNALS)
	sigset_t sigset;
#endif
	char type_str[1];
	char data_str[1];
	char *loading_data[LOADTHD_ARGC + 1];
	binmgr_request_t request_msg;

	struct mq_attr attr;
	attr.mq_maxmsg = BINMGR_MAX_MSG;
	attr.mq_msgsize = sizeof(binmgr_request_t);
	attr.mq_flags = 0;

	ASSERT(BIN_STATE(KERNEL_IDX) != BINARY_UNREGISTERED && g_bin_count > 0);

	bmvdbg("Binary Manager STARTED\n");

	/* Unset all signals except for SIGKILL */
	sigfillset(&sigset);
	sigdelset(&sigset, SIGKILL);
	(void)sigprocmask(SIG_SETMASK, &sigset, NULL);

	/* Create binary manager message queue */
	g_binmgr_mq_fd = mq_open(BINMGR_REQUEST_MQ, O_RDWR | O_CREAT, 0666, &attr);
	if (g_binmgr_mq_fd == (mqd_t)ERROR) {
		bmdbg("Failed to open message queue\n");
		return 0;
	}

	/* Execute loading thread for load all binaries */
	loading_data[0] = itoa(LOADCMD_LOAD_ALL, type_str, 10);
	loading_data[1] = NULL;

	ret = binary_manager_loading(loading_data);
	if (ret <= 0) {
		bmdbg("Failed to create loading thread\n");
		goto binary_manager_exit;
	} else {
		bmvdbg("Loading thread with pid %d will load binaries!\n", ret);
	}

	while (1) {
		bmvdbg("Wait for message\n");

		nbytes = mq_receive(g_binmgr_mq_fd, (char *)&request_msg, sizeof(binmgr_request_t), NULL);
		if (nbytes <= 0) {
			bmdbg("receive ERROR %d, errno %d, retry!\n", nbytes, errno);
			continue;
		}

		bmvdbg("Recevied Request msg : cmd = %d\n", request_msg);
		switch (request_msg.cmd) {
#ifdef CONFIG_BINMGR_RECOVERY
		case BINMGR_FAULT:
			binary_manager_recovery(request_msg.requester_pid);
			break;
#endif
		case BINMGR_GET_INFO:
			binary_manager_get_info_with_name(request_msg.requester_pid, (char *)request_msg.data.bin_name);
			break;
		case BINMGR_GET_INFO_ALL:
			binary_manager_get_info_all(request_msg.requester_pid);
			break;
		case BINMGR_UPDATE:
			memset(type_str, 0, 1);
			memset(data_str, 0, 1);
			loading_data[0] = itoa(LOADCMD_UPDATE, type_str, 10);
			loading_data[1] = (char *)request_msg.data.bin_name;
			loading_data[2] = NULL;
			binary_manager_loading(loading_data);
			break;
		case BINMGR_NOTIFY_STARTED:
			binary_manager_update_running_state(request_msg.requester_pid);
			break;
		case BINMGR_REGISTER_STATECB:
			binary_manager_register_statecb(request_msg.requester_pid, (binmgr_cb_t *)request_msg.data.cb_info);
			break;
		case BINMGR_UNREGISTER_STATECB:
			binary_manager_unregister_statecb(request_msg.requester_pid);
			break;
		default:
			break;
		}
	}

binary_manager_exit:
	bmvdbg("Recovery Manager EXITED\n");

	mq_close(g_binmgr_mq_fd);
	mq_unlink(BINMGR_REQUEST_MQ);

	return 0;
}
