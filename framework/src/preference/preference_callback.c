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
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>
#include <errno.h>
#include <fcntl.h>
	 
#include <tinyara/preference.h>

void preference_signal_cb(int signo)
{
	int nbytes;
	mqd_t mqfd;
	struct mq_attr attr;
	preference_callback_t data;
	char q_name[PREFERENCE_CBMQ_LEN];

	if (signo != SIG_PREFERENCE) {
		prefdbg("Invalid signal %d\n", signo);
		return;
	}

	memset(&data, 0, sizeof(preference_callback_t));

	snprintf(q_name, PREFERENCE_CBMQ_LEN, "%s%d", PREFERENCE_CBMSG_MQ, getpid());
	mqfd = mq_open(q_name, O_RDONLY, 0666);
	if (mqfd == (mqd_t)ERROR) {
		prefdbg("Failed to open mq, errno %d\n", errno);
		return;
	}

	while (1) {
		mq_getattr(mqfd, &attr);
		if (attr.mq_curmsgs <= 0) {
			break;
		}
		nbytes = mq_receive(mqfd, (char *)&data, sizeof(preference_callback_t), NULL);
		if (nbytes == sizeof(preference_callback_t) && data.cb_func != NULL) {
			/* Execute callback function */
			data.cb_func(data.key, data.cb_data);
		} else {
			prefdbg("receive ERROR, ret %d errno %d\n", nbytes, errno);
		}
	}
	mq_close(mqfd);
	mq_unlink(q_name);
}

