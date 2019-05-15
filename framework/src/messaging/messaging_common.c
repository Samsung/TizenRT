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
 *  For MSG_INFO_SAVE : On success, 0 (OK) is returned. On failure, -1 (ERROR) is returned.
 *  FOR MSG_INFO_READ : On success, the number of receivers who waits is returned.
 *			  On failure, -1 (ERROR) is returned.
 ****************************************************************************/
int messaging_handle_data(msg_info_type_t type, const char *port_name, int *data_arr, int *recv_cnt)
{
	int ret;
	pid_t pid;
	struct sched_param param;

	if (type == MSG_INFO_SAVE) {
		pid = getpid();
		if (pid < 0) {
			msgdbg("[Messaging] fail : getting pid error.\n");
			return ERROR;
		}
		ret = sched_getparam(pid, &param);
		if (ret != OK) {
			msgdbg("[Messaging] fail : getting sched param error.\n");
			return ERROR;
		}
		ret = prctl(PR_MSG_SAVE, port_name, pid, param.sched_priority);
	} else if (type == MSG_INFO_READ) {
		ret = prctl(PR_MSG_READ, port_name, data_arr, recv_cnt);
	} else if (type == MSG_INFO_REMOVE) {
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
/****************************************************************************
 * Name : messaging_run_callback
 * 
 * Description:
 *  This function is wrapper callback function for waiting until receiving the message.
 ****************************************************************************/
void messaging_run_callback(int signo, siginfo_t *data)
{
	int ret;
	ssize_t size;
	msg_recv_info_t *recv_info;
	struct mq_attr attr;
	char *recv_packet;
	int msg_type;

	if (data == NULL) {
		msgdbg("[Messaging] recv fail : wrong param for wrapper callback(data).\n");
		return;
	}

	/* recv_info is passed through signal. It has mqdes, buflen and callback information. */
	recv_info = (msg_recv_info_t *)data->si_value.sival_ptr;
	if (recv_info == NULL) {
		msgdbg("[Messaging] recv fail : wrong param for wrapper callback(info).\n");
		return;
	}

	ret = mq_getattr(recv_info->mqdes, &attr);
	if (ret < 0) {
		MSG_FREE(recv_info);
		msgdbg("[Messaging] recv fail : get attribute fail.\n");
		return;
	}

	/* recv_packet is used for receiving message including header. */
	recv_packet = (char *)MSG_ALLOC(attr.mq_msgsize);
	if (recv_packet == NULL) {
		MSG_FREE(recv_info);
		msgdbg("[Messaging] recv fail : out of memory for including header.\n");
		return;
	}

	while (1) {
		size = mq_receive(recv_info->mqdes, (char *)recv_packet, attr.mq_msgsize, 0);
		if (size < 0) {
			msgdbg("[Messaging] recv fail : mq_receive fail, errno %d\n", errno);
			mq_close(recv_info->mqdes);
			MSG_FREE(recv_packet);
			MSG_FREE(recv_info);
			return;
		}

		/* Parsing the received data to user message buffer. */
		memcpy(&(recv_info->msg->sender_pid), recv_packet, sizeof(pid_t));
		memcpy(&msg_type, recv_packet + sizeof(pid_t), sizeof(int));
		memcpy(recv_info->msg->buf, recv_packet + MSG_HEADER_SIZE, recv_info->msg->buflen);

		/* Call user callback */
		(*recv_info->user_cb)(msg_type, recv_info->msg, recv_info->cb_data);
		MSG_FREE(recv_packet);
		if (msg_type == MSG_SEND_REPLY) {
			/* This is only for async-reply msg. In this case, callback registration is removed after one-time use. */
			ret = mq_close(recv_info->mqdes);
			if (ret != OK) {
				msgdbg("[Messaging] Mq_close fail for async-reply.\n");
			}
			ret = mq_unlink(recv_info->port_name);
			if (ret != OK) {
				msgdbg("[Messaging] Unlink fail for async-reply, errno %d\n", errno);
			}
			MSG_FREE(recv_info);
			return;
		}

		ret = mq_getattr(recv_info->mqdes, &attr);
		if (ret < 0) {
			msgdbg("[Messaging] recv fail : mq_getattr fail, mqdes is not correct.\n");
			mq_close(recv_info->mqdes);
			MSG_FREE(recv_info);
			MSG_FREE(data);
			return;
		} else if (attr.mq_curmsgs == 0) {
			/* All requests are handled. Register notification again. */
			messaging_set_notification(SIGMSG_MESSAGING, recv_info);
			break;
		}
	}
}
