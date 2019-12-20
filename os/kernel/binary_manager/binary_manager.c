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

#include "sched/sched.h"
#include "binary_manager.h"

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

	ASSERT(BIN_STATE(KERNEL_IDX) != BINARY_UNREGISTERED && binary_manager_get_binary_count() > 0);

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
		bmdbg("Fail to launch fault msg sender\n");
		return 0;
	}
#endif

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
