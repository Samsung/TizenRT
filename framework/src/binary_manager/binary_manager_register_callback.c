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

/***************************************************************************
 * Included Files
 ***************************************************************************/
#include <stdio.h>
#include <debug.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <mqueue.h>

#include <tinyara/binary_manager.h>

#include "binary_manager_internal.h"

static int binary_manager_send_callback_response(binmgr_statecb_response_t *response_msg)
{
	int ret;
	mqd_t mqfd;

	if (response_msg == NULL) {
		bmdbg("Invalid param\n");
		return ERROR;
	}

	mqfd = mq_open(BINMGR_CBMSG_MQ, O_WRONLY, 0666, 0);
	if (mqfd == (mqd_t)ERROR) {
		bmdbg("mq_open failed!\n");
		return ERROR;
	}

	ret = mq_send(mqfd, (char *)response_msg, sizeof(binmgr_statecb_response_t), BINMGR_NORMAL_PRIO);
	if (ret < 0) {
		bmdbg("mq_send failed! %d\n", errno);
		mq_close(mqfd);
		return ERROR;
	}
	mq_close(mqfd);

	return OK;
}

static void binary_manager_signal_cb(int signo)
{
	int ret;
	int nbytes;
	mqd_t mqfd;
	struct mq_attr attr;
	binmgr_cb_t *callback;
	binmgr_cb_msg_t state_data;
	char q_name[BIN_PRIVMQ_LEN];
	binmgr_statecb_response_t response_msg;

	if (signo != SIGBM_STATE) {
		bmdbg("Invalid signal %d\n", signo);
		return;
	}

	memset(&state_data, 0, sizeof(binmgr_cb_msg_t));

	snprintf(q_name, BIN_NAME_MAX, "%s%d", BINMGR_CBMSG_MQ, getpid());
	mqfd = mq_open(q_name, O_RDONLY, 0666);
	if (mqfd == (mqd_t)ERROR) {
		bmdbg("Failed to open mq, errno %d\n", errno);
		return;
	}

	while (1) {
		mq_getattr(mqfd, &attr);
		if (attr.mq_curmsgs <= 0) {
			break;
		}
		nbytes = mq_receive(mqfd, (char *)&state_data, sizeof(binmgr_cb_msg_t), NULL);
		if (nbytes == sizeof(binmgr_cb_msg_t)) {
			callback = (binmgr_cb_t *)state_data.callback;
			/* Execute callback function */
			callback->func(state_data.bin_name, state_data.state, callback->data);
			if (state_data.need_response) {
				response_msg.result = BINMGR_OK;
				ret = binary_manager_send_callback_response(&response_msg);
				if (ret < 0) {
					bmdbg("Send callback response ERROR\n");
					continue;
				}
			}
		} else {
			bmdbg("receive ERROR, ret %d errno %d\n", nbytes, errno);
			break;
		}
	}
	mq_close(mqfd);
}

int binary_manager_register_state_changed_callback(binmgr_statecb_t handler, void *cb_data)
{
	int ret;
	struct sigaction act;
	binmgr_cb_t *callback;
	binmgr_request_t request_msg;
	binmgr_statecb_response_t response_msg;

	if (handler == NULL) {
		bmdbg("Handler is NULL\n");
		return BINMGR_INVALID_PARAM;
	}

	callback = (binmgr_cb_t *)malloc(sizeof(binmgr_cb_t));
	if (callback == NULL) {
		return BINMGR_OUT_OF_MEMORY;
	}
	callback->func = handler;
	callback->data = cb_data;

	/* Register signal handler */
	act.sa_handler = (_sa_handler_t)binary_manager_signal_cb;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGBM_STATE);

	ret = sigaction(SIGBM_STATE, &act, NULL);
	if (ret == (int)SIG_ERR) {
		bmdbg("Failed to set signal handler\n");
		free(callback);
		return BINMGR_COMMUNICATION_FAIL;
	}

	memset(&request_msg, 0, sizeof(binmgr_request_t));

	ret = binary_manager_set_request(&request_msg, BINMGR_REGISTER_STATECB, callback);
	if (ret < 0) {
		return BINMGR_COMMUNICATION_FAIL;
	}

	ret = binary_manager_send_request(&request_msg);
	if (ret < 0) {
		bmdbg("Failed to send request msg %d\n", ret);
		goto errout_with_cleanup;
	}

	ret = binary_manager_receive_response(&response_msg, sizeof(binmgr_statecb_response_t));
	if (ret == BINMGR_OK) {
		return response_msg.result;
	}
	bmdbg("Failed to receive response msg %d\n", ret);

errout_with_cleanup:
	act.sa_handler = SIG_DFL;
	(void)sigaction(SIGBM_STATE, &act, NULL);

	free(callback);

	return BINMGR_COMMUNICATION_FAIL;
}

int binary_manager_unregister_state_changed_callback(void)
{
	int ret;
	struct sigaction act;
	binmgr_request_t request_msg;
	binmgr_statecb_response_t response_msg;

	memset(&request_msg, 0, sizeof(binmgr_request_t));

	ret = binary_manager_set_request(&request_msg, BINMGR_UNREGISTER_STATECB, NULL);
	if (ret < 0) {
		return BINMGR_COMMUNICATION_FAIL;
	}

	ret = binary_manager_send_request(&request_msg);
	if (ret < 0) {
		bmdbg("Failed to send request msg %d\n", ret);
		return BINMGR_COMMUNICATION_FAIL;
	}

	ret = binary_manager_receive_response(&response_msg, sizeof(binmgr_statecb_response_t));
	if (ret < 0) {
		bmdbg("Failed to receive response msg %d\n", ret);
		return BINMGR_COMMUNICATION_FAIL;
	}

	if (response_msg.result == BINMGR_OK) {
		act.sa_handler = SIG_DFL;
		(void)sigaction(SIGBM_STATE, &act, NULL);
		bmvdbg("Free registered callback %p\n", response_msg.cb_info);
		free(response_msg.cb_info);
	}

	return response_msg.result;
}
