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
#include <fcntl.h>
#include <mqueue.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <queue.h>
#include <sys/types.h>
#include <messaging/messaging.h>
#include "messaging_internal.h"

#define MAX_PORT_NAME_SIZE 64
sq_queue_t g_port_info_list;
struct msg_port_info_s {
	struct msg_port_info_s *flink;
	char name[MAX_PORT_NAME_SIZE];
	mqd_t mqdes;
};
typedef struct msg_port_info_s msg_port_info_t;

/****************************************************************************
 * private functions
 ****************************************************************************/
/****************************************************************************
 * Name : messaging_set_notification
 * 
 * Description:
 *  This function calls mq_notify to set message queue notification.
 ****************************************************************************/
static int messaging_set_notification(msg_recv_info_t *data)
{
	int ret;
	struct sigevent notification;

	notification.sigev_notify = SIGEV_SIGNAL;
	notification.sigev_signo = SIGMSG_UNICAST;
	notification.sigev_value.sival_ptr = data;

	ret = mq_notify(data->mqdes, &notification);
	if (ret < 0) {
		msgdbg("message recv fail : set notification fail. %d\n", errno);
		MSG_FREE(data);
		return ERROR;
	}
	return OK;
}

/****************************************************************************
 * Name : messaging_run_nonblock_callback
 * 
 * Description:
 *  This function is wrapper callback function for waiting until receiving the message.
 ****************************************************************************/
static void messaging_run_nonblock_callback(int signo, siginfo_t *data)
{
	int ret;
	ssize_t size;
	msg_recv_info_t *recv_info;
	char msg[CONFIG_MESSAGING_UNICAST_MSGSIZE - (MSG_HEADER_SIZE)];
	struct mq_attr attr;
	char *recv_data;
	pid_t sender_pid;
	int msg_type;

	if (data == NULL) {
		msgdbg("message recv fail : wrong param for wrapper callback(data).\n");
		return;
	}

	recv_info = (msg_recv_info_t *)data->si_value.sival_ptr;
	if (recv_info == NULL) {
		msgdbg("message recv fail : wrong param for wrapper callback(info).\n");
		return;
	}

	recv_data = (char *)MSG_ALLOC(CONFIG_MESSAGING_UNICAST_MSGSIZE);
	if (recv_data == NULL) {
		msgdbg("message recv fail : out of memory for including header.\n");
		return;
	}

	while (1) {
		size = mq_receive(recv_info->mqdes, recv_data, sizeof(msg_recv_info_t), 0);
		if (size != sizeof(msg_recv_info_t)) {
			msgdbg("message recv fail : mq_receive fail. %d\n", errno);
			mq_close(recv_info->mqdes);
			MSG_FREE(recv_data);
			MSG_FREE(data);
			return;
		}
		memcpy(&sender_pid, recv_data, sizeof(pid_t));
		memcpy(&msg_type, recv_data + sizeof(pid_t), sizeof(int));
		memcpy(msg, recv_data + MSG_HEADER_SIZE, CONFIG_MESSAGING_UNICAST_MSGSIZE - sizeof(pid_t));
		/* Call user callback */
		(*recv_info->user_cb)(msg_type, sender_pid, recv_info->cb_data, &msg);
		MSG_FREE(recv_data);

		ret = mq_getattr(recv_info->mqdes, &attr);
		if (ret < 0) {
			msgdbg("message recv fail : mq_getattr fail.\n");
			mq_close(recv_info->mqdes);
			MSG_FREE(data);
			return;
		} else if (attr.mq_curmsgs == 0) {
			/* All requests are handled. Register notification again. */
			messaging_set_notification(recv_info);
			break;
		}
	}
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
static int messaging_set_notify_signal(void)
{
	int ret;
	struct sigaction act;
	act.sa_sigaction = (_sa_sigaction_t)messaging_run_nonblock_callback;
	act.sa_flags = 0;
	(void)sigemptyset(&act.sa_mask);

	ret = sigaddset(&act.sa_mask, SIGMSG_UNICAST);
	if (ret < 0) {
		msgdbg("message recv fail : sigaddset error.\n");
		return ERROR;
	}

	ret = sigaction(SIGMSG_UNICAST, &act, NULL);
	if (ret == (int)SIG_ERR) {
		msgdbg("message recv fail : sigaction error.\n");
		return ERROR;
	}
	return OK;
}

/****************************************************************************
 * Name : messaging_recv_nonblock
 * 
 * Description:
 *  This function is for non-blocking mq_receive.
 * If there is a message in the queue, this function calls the callback with msg.
 * If there is no message, then calls the notification function to register callback. 
 *
 * Input Parameters:
 *  mqdes     : Message queue descriptor
 *  port_name : The message port name to receive
 *  msg       : The message buffer to receive the message
 *  msglen    : The length of message to receive
 *  param     :
 *	type    : MSG_RECV_TYPE_BLOCK or MSG_RECV_TYPE_NONBLOCK\n
 *	cb_func : user callback function for nonblocking receive
 * 
 * Return Value:
 *  On success, 0 (OK) is returned.; On failure, -1 (ERROR) is returned.
 *  Sender pid is passed to callback function as 3rd argument.
 ****************************************************************************/
static int messaging_recv_nonblock(mqd_t mqdes, const char *port_name, msg_data_t *data, msg_recv_param_t *param)
{
	int ret;
	ssize_t recv_size_chk;
	msg_recv_info_t *nonblock_data;
	msg_port_info_t *port_info;
	int recv_size;
	char *recv_data;
	pid_t sender_pid;

	recv_size = data->msglen + MSG_HEADER_SIZE;
	recv_data = MSG_ALLOC(recv_size);
	if (recv_data == NULL) {
		msgdbg("message recv fail : out of memory for including header.\n");
		goto cleanup_mq;
	}
	/* Check there was msg already before setting notification */
	while (1) {
		recv_size_chk = mq_receive(mqdes, recv_data, recv_size, 0);
		if (recv_size_chk > 0 && recv_size_chk <= recv_size) {
			memcpy(&sender_pid, recv_data, sizeof(pid_t));
			memcpy(&data->msg_type, recv_data + sizeof(pid_t), sizeof(int));
			memcpy(data->msg, recv_data + MSG_HEADER_SIZE, data->msglen);
			(*param->cb_func)(data->msg_type, sender_pid, param->cb_data, data->msg);
		} else if (recv_size_chk == ERROR && errno == EAGAIN) {
			msgdbg("message recv : the queue was empty, and the NONBLOCK flag was set.\n");
			break;
		} else {
			msgdbg("message recv fail : recv fail errno %d, recv_size %d.\n", errno, recv_size_chk);
			MSG_FREE(recv_data);
			goto cleanup_mq;
		}
	}
	MSG_FREE(recv_data);

	/* Update the messag port info list. */
	port_info = (msg_port_info_t *)MSG_ALLOC(sizeof(msg_port_info_t));
	if (port_info == NULL) {
		msgdbg("message recv fail : out of memory for msg_port_info.\n");
		goto cleanup_mq;
	}
	port_info->mqdes = mqdes;
	strncpy(port_info->name, port_name, strlen(port_name) + 1);
	sq_addlast((FAR sq_entry_t *)port_info, &g_port_info_list);

	/* There was no msg, then set notification. */
	ret = messaging_set_notify_signal();
	if (ret != OK) {
		msgdbg("message recv fail : set notification error.\n");
		goto cleanup_all;
	}

	nonblock_data = (msg_recv_info_t *)MSG_ALLOC(sizeof(msg_recv_info_t));
	if (nonblock_data == NULL) {
		msgdbg("message recv fail : out of memory.\n");
		goto cleanup_all;
	}
	nonblock_data->mqdes = mqdes;
	nonblock_data->user_cb = param->cb_func;
	nonblock_data->cb_data = param->cb_data;

	ret = messaging_set_notification(nonblock_data);
	if (ret != OK) {
		goto cleanup_all;
	}
	MSG_FREE(nonblock_data);
	return OK;

cleanup_all:
	sq_rem((FAR sq_entry_t *)port_info, &g_port_info_list);
	MSG_FREE(port_info);
cleanup_mq:
	mq_close(mqdes);
	mq_unlink(port_name);
	return ERROR;
}

/****************************************************************************
 * Name : messaging_recv_block
 * 
 * Description:
 *  This function calls mq_receive with block mode.
 *
 * Input Parameters:
 *  mqdes     : Message queue descriptor
 *  port_name : The message port name to receive
 *  msg       : The message buffer to receive the message
 *  msglen    : The length of message to receive
 * 
 * Return Value:
 *  On success, sender pid (>=0) is returned.; On failure, -1 (ERROR) is returned.
 ****************************************************************************/
static int messaging_recv_block(mqd_t mqdes, const char *port_name, msg_data_t *data)
{
	int ret = OK;
	int recv_size;
	char *recv_data;
	pid_t sender_pid;

	recv_size = MSG_HEADER_SIZE + data->msglen;
	recv_data = MSG_ALLOC(recv_size);
	if (recv_data == NULL) {
		msgdbg("message recv fail : out of memory for including header.\n");
		ret = ERROR;
		goto cleanup_return;
	}

	ret = mq_receive(mqdes, recv_data, CONFIG_MESSAGING_UNICAST_MSGSIZE, 0);
	if (ret < 0) {
		msgdbg("message recv fail : recv fail %d.\n", errno);
		ret = ERROR;
		goto cleanup_return;
	}
	memcpy(&sender_pid, recv_data, sizeof(pid_t));
	ret = (int)sender_pid;
	memcpy(&data->msg_type, recv_data + sizeof(pid_t), sizeof(int));
	memcpy(data->msg, recv_data + MSG_HEADER_SIZE, data->msglen);

cleanup_return:
	MSG_FREE(recv_data);
	mq_close(mqdes);
	mq_unlink(port_name);
	return ret;
}
/****************************************************************************
 * Name : messaging_recv_internal
 * 
 * Description:
 *  This function calls receive function based on receive type.
 *
 * Input Parameters:
 *  port_name : The message port name to receive
 *  param :
 *	msg     : The message buffer to receive the message
 *	msglen  : The length of message to receive
 *	type    : MSG_RECV_TYPE_BLOCK or MSG_RECV_TYPE_NONBLOCK\n
 *	cb_func : user callback function for nonblocking receive
 * 
 * Return Value:
 *  On success, 0 (OK) is returned.; On failure, -1 (ERROR) is returned.
 ****************************************************************************/
static int messaging_recv_internal(const char *port_name, msg_data_t *data, msg_recv_param_t *param)
{
	int ret = OK;
	mqd_t mqdes;
	struct mq_attr internal_attr;

	internal_attr.mq_maxmsg = CONFIG_MESSAGING_UNICAST_MAXMSG;
	internal_attr.mq_msgsize = data->msglen + MSG_HEADER_SIZE;
	internal_attr.mq_flags = 0;

	if (param->type == MSG_RECV_TYPE_BLOCK) {
		mqdes = mq_open(port_name, O_RDONLY | O_CREAT, 0666, &internal_attr);
		if (mqdes == (mqd_t)ERROR) {
			msgdbg("message recv fail : open fail %d.\n", errno);
			return ERROR;
		}
		ret = messaging_recv_block(mqdes, port_name, data);
		if (ret >= 0) {
			data->sender_pid = ret;
			ret = OK;
		}
	} else {
		if (param->cb_func == NULL) {
			msgdbg("message recv fail : invalid parameter.\n");
			return ERROR;
		}
		mqdes = mq_open(port_name, O_RDONLY | O_CREAT | O_NONBLOCK, 0666, &internal_attr);
		if (mqdes == (mqd_t)ERROR) {
			msgdbg("message recv fail : open fail %d.\n", errno);
			return ERROR;
		}
		ret = messaging_recv_nonblock(mqdes, port_name, data, param);
	}
	return ret;
}
/****************************************************************************
 * Name : messaging_recv
 *
 * Description :
 *  Wait to receive unicast message from specified message port
 * 
 * Input Parameters:
 *  port_name : The message port name to receive
 *  data      :
 *	msg    : The message buffer to receive the message\n
 *	msglen : The length of message to receive\n
 *  param
 *	type    : MSG_RECV_BLOCK or MSG_RECV_NONBLOCK\n
 *	cb_func : user callback function for nonblocking receive\n
 *	cb_data : user callback data for nonblocking receive\n
 *
 * Return Value:
 *  On success, 0 (OK) is returned.; On failure, -1 (ERROR) is returned.
 ****************************************************************************/
int messaging_recv(const char *port_name, msg_data_t *data, msg_recv_param_t *param)
{
	int ret = OK;

	if ((param->type < MSG_RECV_TYPE_BLOCK) || (param->type > MSG_RECV_TYPE_NONBLOCK) || (param->type == MSG_RECV_TYPE_NONBLOCK && param->cb_func == NULL)) {
		msgdbg("message recv fail : invalid param.\n");
		return ERROR;
	}

	ret = messaging_recv_internal(port_name, data, param);
	if (ret == ERROR) {
		msgdbg("message recv fail : recv fail.\n");
		return ret;
	}

	return OK;
}
/****************************************************************************
 * Name : messaging_unregister
 * 
 * Description:
 *  Unregister the messaging port when set non-block receive.
 *
 * Input Parameters:
 *	port_name : The message port name to unregister\n
 *		This API should be called when calling messaging_recv with non-block type.\n
 *		If this API is not called, memory leak can happen.
 * 
 * Return Value:
 *  On success, 0 (OK) is returned.; On failure, -1 (ERROR) is returned.
 ****************************************************************************/
int messaging_unregister(const char *port_name)
{
	int ret;
	msg_port_info_t *port_info;

	if (port_name == NULL) {
		msgdbg("message unregister fail : invalid param.\n");
		return ERROR;
	}

	port_info = (msg_port_info_t *)sq_peek(&g_port_info_list);
	while (port_info != NULL) {
		if (strncmp(port_info->name, port_name, strlen(port_name) + 1) == 0) {
			mq_close(port_info->mqdes);
			sq_rem((FAR sq_entry_t *)port_info, &g_port_info_list);
			MSG_FREE(port_info);
			break;
		}
		port_info = (msg_port_info_t *)sq_next(port_info);
	}
	
	ret = mq_unlink(port_name);
	if (ret != OK && errno != ENOENT) {
		msgdbg("message unregister fail : unlink error %d.\n", errno);
		return ERROR;
	}
	return OK;
}
