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
#include <debug.h>
#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <messaging/messaging.h>
#include "messaging_internal.h"

/****************************************************************************
 * functions
 ****************************************************************************/
/****************************************************************************
 * Name : messaging_handle_data
 * 
 * Description:
 *  This function saves or reads receiver data from the list.
 *
 * Return Value:
 *  For MSG_HANDLE_SAVE : On success, 0 (OK) is returned. On failure, -1 (ERROR) is returned.
 *  FOR MSG_HANDLE_READ : On success, the number of receivers who waits is returned.
 *			  On failure, -1 (ERROR) is returned.
 ****************************************************************************/
int messaging_handle_data(msg_handle_type_t type, const char *port_name, int *data_arr, int *recv_cnt)
{
	int ret;
	pid_t pid = getpid();
	struct sched_param param;
	ret = sched_getparam(pid, &param);
	if (ret != OK) {
		msgdbg("[Messaging] fail : getting sched param error.\n");
		return ERROR;
	}

	if (type == MSG_HANDLE_SAVE) {
		ret = prctl(PR_MSG_SAVE, port_name, pid, param.sched_priority);
	} else if (type == MSG_HANDLE_READ) {
		ret = prctl(PR_MSG_READ, port_name, data_arr, recv_cnt);
	} else if (type == MSG_HANDLE_REMOVE) {
		ret = prctl(PR_MSG_REMOVE, port_name);
	} else {
		msgdbg("[Messaging] fail : invalid type for messaging_handle_data, type : %d.\n", type);
		ret = ERROR;
	}

	return ret;
}
/****************************************************************************
 * Name : messaging_set_notify_signal
 * 
 * Description:
 *  This function registers the wrapper callback for notification.
 *
 * Return Value:
 *  On success, 0 (OK) is returned.; On failure, -1 (ERROR) is returned.
 ****************************************************************************/
int messaging_set_notify_signal(int signo, _sa_sigaction_t func)
{
	int ret;
	struct sigaction act;
	act.sa_sigaction = func;
	act.sa_flags = 0;
	(void)sigemptyset(&act.sa_mask);

	ret = sigaddset(&act.sa_mask, signo);
	if (ret < 0) {
		msgdbg("[Messaging] set noti-signal fail : sigaddset error.\n");
		return ERROR;
	}

	ret = sigaction(signo, &act, NULL);
	if (ret == (int)SIG_ERR) {
		msgdbg("[Messaging] set noti-signal fail : sigaction error.\n");
		return ERROR;
	}
	return OK;
}
/****************************************************************************
 * Name : messaging_set_notification
 * 
 * Description:
 *  This function calls mq_notify to set message queue notification.
 ****************************************************************************/
int messaging_set_notification(int signo, msg_recv_info_t *data)
{
	int ret;
	struct sigevent notification;

	notification.sigev_notify = SIGEV_SIGNAL;
	notification.sigev_signo = signo;
	notification.sigev_value.sival_ptr = data;

	ret = mq_notify(data->mqdes, &notification);
	if (ret < 0) {
		msgdbg("[Messaging] set notification fail. errno %d\n", errno);
		MSG_FREE(data);
		return ERROR;
	}
	return OK;
}
