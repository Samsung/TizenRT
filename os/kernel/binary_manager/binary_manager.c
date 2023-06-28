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
#ifdef CONFIG_BOARDCTL_RESET
#include <sys/boardctl.h>
#endif
#include <tinyara/reboot_reason.h>
#ifdef CONFIG_BINMGR_RECOVERY
#include <tinyara/kthread.h>
#endif

#include "sched/sched.h"
#include "binary_manager.h"

#if CONFIG_MQ_MAXMSGSIZE < (CONFIG_NAME_MAX + 64)
/* The largest size of messages is (CONFIG_NAME_MAX + 64) for communication with binary manager */
#error "CONFIG_MQ_MAXMSGSIZE should be greater than (CONFIG_NAME_MAX + 64)"
#endif

#ifdef CONFIG_BINMGR_UPDATE
#ifndef CONFIG_BOARDCTL_RESET
/* CONFIG_BOARDCTL_RESET should be enabled for kernel binary update */
#error "CONFIG_BOARDCTL_RESET should be enabled for kernel binary update"
#endif
#endif


/****************************************************************************
 * Private Definitions
 ****************************************************************************/
static mqd_t g_binmgr_mq_fd;

#ifdef CONFIG_BINMGR_RECOVERY
/****************************************************************************
 * Name: binary_manager_get_mqfd
 *
 * Description:
 *	 This function gets a mqfd of binary manager for request.
 *
 ****************************************************************************/
mqd_t binary_manager_get_mqfd(void)
{
	return g_binmgr_mq_fd;
}
#endif

/****************************************************************************
 * Name: binary_manager_reset_board
 *
 * Description:
 *	 This function resets the board.
 *
 ****************************************************************************/
void binary_manager_reset_board(int reboot_reason)
{
#ifdef CONFIG_SYSTEM_REBOOT_REASON
	WRITE_REBOOT_REASON(reboot_reason);
#endif
#ifdef CONFIG_BOARDCTL_RESET
	boardctl(BOARDIOC_RESET, EXIT_SUCCESS);
#else
	(void)irqsave();
	sched_lock();
	for (;;) {
		lldbg("\nASSERT!! Push the reset button!\n");
		up_mdelay(300000);  // Print the message every 5 min
	}
#endif
}

/****************************************************************************
 * Main Function
 ****************************************************************************/
/****************************************************************************
 * Name: binary_manager
 *
 * Description:
 *   This thread is a main thread for binary management.
 *   It receives messages and handles them according to request type.
 *
 ****************************************************************************/
int binary_manager(int argc, char *argv[])
{
	int nbytes;
#if !defined(CONFIG_DISABLE_SIGNALS)
	sigset_t sigset;
#endif
	struct mq_attr attr;
	binmgr_request_t request_msg;
	bool is_found_kpart = true;
	bool is_found_bootparam = true;
#ifdef CONFIG_APP_BINARY_SEPARATION
	int ret;
	bool is_found_ubin = true;
#endif

#ifdef CONFIG_USE_BP
	if (binary_manager_update_bpinfo() != BINMGR_OK) {
		is_found_bootparam = false;
		goto errout_with_nobinary;
	}
#endif

	if (binary_manager_get_kcount() <= 0 || !binary_manager_scan_kbin()) {
		is_found_kpart = false;
		goto errout_with_nobinary;
	}

#ifdef CONFIG_APP_BINARY_SEPARATION
	if (binary_manager_get_ucount() <= 0) {
		is_found_ubin = false;
		goto errout_with_nobinary;
	}
#endif

	bmvdbg("Binary Manager STARTED\n");

	/* Unset all signals except for SIGKILL */
	sigfillset(&sigset);
	sigdelset(&sigset, SIGKILL);
	(void)sigprocmask(SIG_SETMASK, &sigset, NULL);

#ifdef CONFIG_BINMGR_RECOVERY
	ret = kernel_thread(FAULTMSGSENDER_NAME, FAULTMSGSENDER_PRIORITY, FAULTMSGSENDER_STACKSIZE, binary_manager_faultmsg_sender, NULL);
	if (ret > 0) {
		bmvdbg("Launch fault msg sender thread with pid %d\n", ret);
		binary_manager_set_faultmsg_sender(ret);
	} else {
		bmdbg("Fail to launch fault msg sender, EXIT!\n");
		return 0;
	}
#endif
	attr.mq_maxmsg = BINMGR_MAX_MSG;
	attr.mq_msgsize = sizeof(binmgr_request_t);
	attr.mq_flags = 0;

	/* Create binary manager message queue */
	g_binmgr_mq_fd = mq_open(BINMGR_REQUEST_MQ, O_RDWR | O_CREAT, 0666, &attr);
	if (g_binmgr_mq_fd == (mqd_t)ERROR) {
		bmdbg("Fail to open mq, errno %d. EXIT!\n", errno);
		return 0;
	}

#ifdef CONFIG_APP_BINARY_SEPARATION
	/* Execute loading thread for load all binaries */
	ret = binary_manager_execute_loader(LOADCMD_LOAD_ALL, 0);
	if (ret != OK) {
		bmdbg("Fail to create loading thread. EXIT!\n");
		mq_close(g_binmgr_mq_fd);
		mq_unlink(BINMGR_REQUEST_MQ);
		return 0;
	}
#endif

	while (1) {
		bmvdbg("Wait for message\n");
		nbytes = mq_receive(g_binmgr_mq_fd, (char *)&request_msg, sizeof(binmgr_request_t), NULL);
		if (nbytes <= 0) {
			bmdbg("Fail to receive mq, ret %d, errno %d. retry!\n", nbytes, errno);
			continue;
		}

		bmvdbg("Received Request msg : cmd = %d\n", request_msg);
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
		case BINMGR_GET_DOWNLOAD_PATH:
			binary_manager_get_inactive_path(request_msg.requester_pid, request_msg.data.bin_name);
			break;
		case BINMGR_GET_CURRENT_PATH:
			binary_manager_get_active_path(request_msg.requester_pid, request_msg.data.bin_name);
			break;
#ifdef CONFIG_USE_BP
		case BINMGR_SETBP:
			binary_manager_update_bootparam(request_msg.requester_pid, request_msg.data.type);
			break;
#endif
		case BINMGR_UPDATE:
#ifdef CONFIG_APP_BINARY_SEPARATION
			binary_manager_execute_loader(LOADCMD_UPDATE, 0);
#else
			binary_manager_update_kernel_binary();
#endif
			break;
#ifdef CONFIG_APP_BINARY_SEPARATION
		case BINMGR_GET_STATE:
			binary_manager_get_state_with_name(request_msg.requester_pid, (char *)request_msg.data.bin_name);
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
#endif
		default:
			bmvdbg("Invalid cmd = %d\n", request_msg.cmd);
			break;
		}
	}

	return 0;
errout_with_nobinary:
	while (1) {
		printf("=============== !!ERROR!! ============== \n");
		if (!is_found_bootparam) {
			printf("ERROR!! Not found user partitions because parsing a partition list is failed.\n");
			printf("Please check whether the partition 'bootparam' exists in CONFIG_FLASH_PART_TYPE with 8K size.\n");
		}
#ifdef CONFIG_APP_BINARY_SEPARATION
		if (!is_found_ubin) {
			printf("ERROR!! Not found user partitions because parsing a partition list is failed.\n");
			printf("Please check logs from configure_mtd_partitions and whether the partition 'bin' exists in CONFIG_FLASH_PART_TYPE.\n");
		}
#endif
		if (!is_found_kpart) {
			printf("ERROR!! Not found kernel partitions because parsing a partition list is failed.\n");
			printf("Please check logs from configure_mtd_partitions and whether the partition 'kernel' exists in CONFIG_FLASH_PART_TYPE.\n");
		}
		printf("Enable CONFIG_DEBUG_BINMGR_ERROR if you want to know exact reason.\n\n");
		sleep(10);
	}
}
