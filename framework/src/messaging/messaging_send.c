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
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <messaging/messaging.h>
#include "messaging_internal.h"

#define MSG_REPLY_PRIO 10
/****************************************************************************
 * private functions
 ****************************************************************************/

static int messaging_sync_recv(const char *port_name, msg_send_param_t *param)
{
	int ret = OK;
	mqd_t sync_mqdes;
	char *sync_portname;
	struct mq_attr internal_attr;

	internal_attr.mq_maxmsg = CONFIG_MESSAGING_UNICAST_MAXMSG;
	internal_attr.mq_msgsize = CONFIG_MESSAGING_UNICAST_MSGSIZE;
	internal_attr.mq_flags = 0;

	/* sender waits the reply with "port_name + MSG_SYNC_PORTNAME_PREFIX". */
	MSG_ASPRINTF(&sync_portname, "%s%s", port_name, MSG_SYNC_PORTNAME_PREFIX);
	if (sync_portname == NULL) {
		printf("message send fail : sync portname allocation fail.\n");
		return ERROR;
	}
	sync_mqdes = mq_open(sync_portname, O_RDONLY | O_CREAT, 0666, &internal_attr);
	if (sync_mqdes == (mqd_t)ERROR) {
		printf("message send fail : sync open fail %d.\n", errno);
		MSG_FREE(sync_portname);
		return ERROR;
	}

	ret = mq_receive(sync_mqdes, (char *)param->sync_msg, param->sync_msglen, 0);
	if (ret != OK) {
		printf("message send fail : sync recv fail %d.\n", errno);
	}

	MSG_FREE(sync_portname);
	mq_close(sync_mqdes);
	mq_unlink(sync_portname);

	return ret;
}

static int messaging_send_internal(const char *port_name, const char *msg, size_t msglen, int prio)
{
	int ret = OK;
	mqd_t mqdes;
	struct mq_attr internal_attr;

	internal_attr.mq_maxmsg = CONFIG_MESSAGING_UNICAST_MAXMSG;
	internal_attr.mq_msgsize = CONFIG_MESSAGING_UNICAST_MSGSIZE;
	internal_attr.mq_flags = 0;

	mqdes = mq_open(port_name, O_WRONLY | O_CREAT, 0666, &internal_attr);
	if (mqdes == (mqd_t)ERROR) {
		printf("message send fail : open fail %d.\n", errno);
		return ERROR;
	}

	ret = mq_send(mqdes, (char *)msg, msglen, prio);
	if (ret != OK) {
		printf("message send fail : send fail %d.\n", errno);
	}

	mq_close(mqdes);
	return ret;
}

/****************************************************************************
 * messaging_send
 ****************************************************************************/
int messaging_send(const char *port_name, const char *msg, size_t msglen, msg_send_param_t *param)
{
	int ret;

	if (param->type != MSG_SEND_ASYNC || (param->type == MSG_SEND_SYNC && param->sync_msg == NULL)) {
		printf("message send fail : invalid param.\n");
		return ERROR;
	}

	ret = messaging_send_internal(port_name, msg, msglen, param->priority);
	if (ret != OK) {
		goto send_fail;
	}

	if (param->type == MSG_SEND_SYNC) {
		ret = messaging_sync_recv(port_name, param);
		if (ret != OK) {
			goto send_fail;
		}
	}
	return OK;

send_fail:
	return ERROR;
}

int messaging_send_reply(const char *port_name, const char *msg, size_t msglen)
{
	int ret = OK;
	char *reply_portname;

	/* sender waits the reply with "port_name + MSG_SYNC_PORTNAME_PREFIX". */
	MSG_ASPRINTF(&reply_portname, "%s%s", port_name, MSG_SYNC_PORTNAME_PREFIX);

	ret = messaging_send_internal(reply_portname, msg, msglen, MSG_REPLY_PRIO);
	if (ret != OK) {
		printf("message reply fail.\n");
	}
	MSG_FREE(reply_portname);
	return ret;
}
