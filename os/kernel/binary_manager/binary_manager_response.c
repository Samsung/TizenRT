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

#include <tinyara/binary_manager.h>

#include "binary_manager.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/
void binary_manager_send_response(char *q_name, void *response_msg, int msg_size)
{
	int ret;
	mqd_t mqfd;
	struct mq_attr attr;

	if (q_name == NULL || response_msg == NULL || msg_size < 0) {
		bmdbg("Invalid param\n");
		return;
	}

	attr.mq_maxmsg = BINMGR_MAX_MSG;
	attr.mq_msgsize = msg_size;
	attr.mq_flags = 0;

	mqfd = mq_open(q_name, O_WRONLY | O_CREAT, 0666, &attr);
	if (mqfd == (mqd_t)ERROR) {
		bmdbg("mq_open failed!\n");
		return;
	}

	ret = mq_send(mqfd, (char *)response_msg, msg_size, BINMGR_NORMAL_PRIO);
	if (ret < 0) {
		bmdbg("mq_send failed! %d\n", errno);
		mq_close(mqfd);
		return;
	}

	mq_close(mqfd);
}
