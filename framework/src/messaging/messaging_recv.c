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
#include <sys/types.h>
#include <messaging/messaging.h>
#include "messaging_internal.h"

/****************************************************************************
 * private functions
 ****************************************************************************/
static void set_notification(messaging_recv_info_t *data)
{
	int ret;
	struct sigevent notification;

	notification.sigev_notify = SIGEV_SIGNAL;
	notification.sigev_signo = SIGMSG_UNICAST;
	notification.sigev_value.sival_ptr = data;

	ret = mq_notify(data->mqdes, &notification);
	if (ret < 0) {
		printf("message recv fail : set notification fail. %d\n", errno);
		MSG_FREE(data);
		mq_close(data->mqdes);
		return;
	}
}

static void wrapper_callback(int signo, siginfo_t *data)
{
	int ret;
	messaging_recv_info_t *info;
	char msg[CONFIG_MESSAGING_UNICAST_MSGSIZE];
	struct mq_attr attr;

	info = (messaging_recv_info_t *)data->si_value.sival_ptr;
	if (info == NULL) {
		printf("message recv fail : wrong param for wrapper callback.\n");
		return;
	}

	while (1) {
		ret = mq_receive(info->mqdes, (char *)&msg, sizeof(messaging_recv_info_t), 0);
		if (ret < 0) {
			printf("message recv fail : mq_receive fail. %d\n", errno);
			mq_close(info->mqdes);
			MSG_FREE(data);
			return;
		}
		/* Call user callback */
		(*info->user_cb)(NULL, &msg);

		ret = mq_getattr(info->mqdes, &attr);
		if (ret < 0) {
			printf("message recv fail : mq_getattr fail.\n");
			mq_close(info->mqdes);
			MSG_FREE(data);
			return;
		} else if (attr.mq_curmsgs == 0) {
			/* All requests are handled. Register notification again. */
			set_notification(info);
			break;
		}
	}
}

static int set_notify_signal(void)
{
	int ret;
	struct sigaction act;
	act.sa_sigaction = (_sa_sigaction_t)wrapper_callback;
	act.sa_flags = 0;
	(void)sigemptyset(&act.sa_mask);

	ret = sigaddset(&act.sa_mask, SIGMSG_UNICAST);
	if (ret < 0) {
		printf("message recv fail : nonblock recv 1.\n");
		return ERROR;
	}

	ret = sigaction(SIGMSG_UNICAST, &act, NULL);
	if (ret == (int)SIG_ERR) {
		printf("message recv fail : nonblock recv 2.\n");
		return ERROR;
	}
	return OK;
}

static int messaging_recv_nonblock(mqd_t *mqdes, const char *port_name, char *msg, size_t msglen, msg_recv_param_t *param)
{
	int ret;
	messaging_recv_info_t *nonblock_data;

	/* Check there was msg already before setting notification */
	ret = mq_receive(*mqdes, msg, CONFIG_MESSAGING_UNICAST_MSGSIZE, 0);
	if (ret > 0) {
		mq_close(*mqdes);
		mq_unlink(port_name);
		return OK;
	} else if (ret != ERROR) {
		printf("message recv fail : recv fail %d.\n", errno);
		return ERROR;
	}

	/* There was no msg, then set notification. */
	ret = set_notify_signal();
	if (ret != OK) {
		return ERROR;
	}

	nonblock_data = (messaging_recv_info_t *)MSG_ALLOC(sizeof(messaging_recv_info_t));
	if (nonblock_data == NULL) {
		printf("message recv fail : out of memory.\n");
		return ERROR;
	}
	nonblock_data->mqdes = *mqdes;
	nonblock_data->user_cb = param->cb_func;

	set_notification(nonblock_data);
	MSG_FREE(nonblock_data);
	return OK;
}

static int messaging_recv_block(mqd_t *mqdes, const char *port_name, char *msg, size_t msglen)
{
	int ret = OK;

	ret = mq_receive(*mqdes, msg, CONFIG_MESSAGING_UNICAST_MSGSIZE, 0);
	if (ret < 0) {
		printf("message recv fail : recv fail %d.\n", errno);
	}

	mq_close(*mqdes);
	mq_unlink(port_name);
	return ret;
}

static int messaging_recv_internal(const char *port_name, char *msg, size_t msglen, msg_recv_param_t *param)
{
	int ret = OK;
	mqd_t mqdes;
	struct mq_attr internal_attr;

	internal_attr.mq_maxmsg = CONFIG_MESSAGING_UNICAST_MAXMSG;
	internal_attr.mq_msgsize = msglen;
	internal_attr.mq_flags = 0;

	if (param->type == MSG_RECV_BLOCK) {
		mqdes = mq_open(port_name, O_RDONLY | O_CREAT, 0666, &internal_attr);
		if (mqdes == (mqd_t)ERROR) {
			printf("message recv fail : open fail %d.\n", errno);
			return ERROR;
		}
		ret = messaging_recv_block(&mqdes, port_name, msg, msglen);
	} else {
		mqdes = mq_open(port_name, O_RDONLY | O_CREAT | O_NONBLOCK, 0666, &internal_attr);
		if (mqdes == (mqd_t)ERROR) {
			printf("message recv fail : open fail %d.\n", errno);
			return ERROR;
		}
		ret = messaging_recv_nonblock(&mqdes, port_name, msg, msglen, param);
	}
	return ret;
}
/****************************************************************************
 * messaging_recv
 ****************************************************************************/
int messaging_recv(const char *port_name, char *msg, size_t msglen, msg_recv_param_t *param)
{
	int ret = OK;

	if (param->type == MSG_RECV_NONBLOCK && param->cb_func == NULL) {
		printf("message recv fail : invalid param.\n");
		return ERROR;
	}

	ret = messaging_recv_internal(port_name, msg, msglen, param);
	if (ret == ERROR) {
		printf("message recv fail : recv fail.\n");
		return ret;
	}

	return OK;
}
