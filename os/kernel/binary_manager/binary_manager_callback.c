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
#include <debug.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <queue.h>
#include <sys/types.h>

#include <tinyara/mm/mm.h>
#include <tinyara/sched.h>
#include <tinyara/binary_manager.h>

#include "binary_manager.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: binary_manager_clear_bin_statecb
 *
 * Description:
 *   This function clears all registered callbacks in binary.
 *
 ****************************************************************************/
void binary_manager_clear_bin_statecb(int bin_idx)
{
	statecb_node_t *cb_node;

	cb_node = (statecb_node_t *)sq_peek(&BIN_CBLIST(bin_idx));
	while (cb_node != NULL) {
		bmvdbg("Clear callback info, bin idx %d\n", bin_idx);
		sq_rem((FAR sq_entry_t *)cb_node, &BIN_CBLIST(bin_idx));
		kmm_free(cb_node);
		cb_node = (statecb_node_t *)sq_next(cb_node);
	}
}

/****************************************************************************
 * Name: binary_manager_unregister_statecb
 *
 * Description:
 *   This function clears registered callback for the changes of binary state.
 *
 ****************************************************************************/
void binary_manager_unregister_statecb(int pid)
{
	int bin_idx;
	struct tcb_s *tcb;
	statecb_node_t *cb_node;
	binmgr_statecb_response_t response_msg;
	char q_name[BIN_PRIVMQ_LEN];

	if (pid < 0) {
		bmdbg("Invalid parameter\n");
		response_msg.result = BINMGR_INVALID_PARAM;
		goto send_result;
	}

	tcb = sched_gettcb(pid);

	if (tcb == NULL || tcb->group == NULL || tcb->group->tg_binidx < 0) {
		bmdbg("Failed to get pid %d binary info\n", pid);
		response_msg.result = BINMGR_INVALID_PARAM;
		goto send_result;
	}

	response_msg.result = BINMGR_NOT_FOUND;

	bin_idx = tcb->group->tg_binidx;
	if (bin_idx < 0) {
		bmdbg("binary %d is not registered\n", tcb->group->tg_binidx);
		goto send_result;
	}

	cb_node = (statecb_node_t *)sq_peek(&BIN_CBLIST(bin_idx));
	while (cb_node != NULL) {
		if (cb_node->pid == pid) {
			bmvdbg("Clear callback info, pid %d\n", pid);
			sq_rem((FAR sq_entry_t *)cb_node, &BIN_CBLIST(bin_idx));
			response_msg.result = BINMGR_OK;
			kmm_free(cb_node);
			break;
		}
		cb_node = (statecb_node_t *)sq_next(cb_node);
	}

send_result:
	snprintf(q_name, BIN_PRIVMQ_LEN, "%s%d", BINMGR_RESPONSE_MQ_PREFIX, pid);
	binary_manager_send_response(q_name, &response_msg, sizeof(binmgr_statecb_response_t));
}

/****************************************************************************
 * Name: binary_manager_register_statecb
 *
 * Description:
 *   This function registers callback for the changes of binary state.
 *
 ****************************************************************************/
void binary_manager_register_statecb(int pid, binmgr_cb_t *cb_info)
{
	int bin_idx;
	struct tcb_s *tcb;
	statecb_node_t *cb_node;
	char q_name[BIN_PRIVMQ_LEN];
	binmgr_statecb_response_t response_msg;

	if (pid < 0 || cb_info == NULL) {
		bmdbg("Invalid parameter\n");
		response_msg.result = BINMGR_INVALID_PARAM;
		goto send_result;
	}

	tcb = sched_gettcb(pid);
	if (tcb == NULL || tcb->group == NULL || tcb->group->tg_binidx < 0) {
		bmdbg("Failed to get pid %d binary info\n", pid);
		response_msg.result = BINMGR_INVALID_PARAM;
		goto send_result;
	}

	bin_idx = tcb->group->tg_binidx;
	if (bin_idx < 0) {
		bmdbg("binary %d is not registered\n", tcb->group->tg_binidx);
		response_msg.result = BINMGR_NOT_FOUND;
		goto send_result;
	}

	/* Check whether a callback is registered. */
	cb_node = (statecb_node_t *)sq_peek(&BIN_CBLIST(bin_idx));
	while (cb_node != NULL) {
		if (cb_node->pid == pid) {
			bmdbg("callback is already registered, pid %d\n", pid);
			response_msg.result = BINMGR_ALREADY_REGISTERED;
			goto send_result;
		}
		cb_node = (statecb_node_t *)sq_next(cb_node);
	}

	/* If not, allocate new callback node and copy data */
	cb_node = (statecb_node_t *)kmm_malloc(sizeof(statecb_node_t));
	if (cb_node == NULL) {
		bmdbg("Out of Memory\n");
		response_msg.result = BINMGR_OUT_OF_MEMORY;
		goto send_result;
	}
	cb_node->pid = pid;
	cb_node->cb_info = cb_info;
	bmvdbg("callback is registered pid %d, cb %p\n", cb_node->pid, cb_node->cb_info);

	/* Add callback node to a callback list of binary */
	sq_addlast((FAR sq_entry_t *)cb_node, &BIN_CBLIST(bin_idx));
	response_msg.result = OK;

send_result:
	snprintf(q_name, BIN_PRIVMQ_LEN, "%s%d", BINMGR_RESPONSE_MQ_PREFIX, pid);
	binary_manager_send_response(q_name, &response_msg, sizeof(binmgr_statecb_response_t));
}

/****************************************************************************
 * Name: binary_manager_send_statecb_msg
 *
 * Description:
 *   This function sends messages and signals to tasks/pthreads which registered callback.
 *
 ****************************************************************************/
int binary_manager_send_statecb_msg(int recv_binidx, char *bin_name, uint8_t state, bool need_response)
{
	int ret;
	int send_count;
	mqd_t send_mq;
	mqd_t recv_mq = NULL;
	struct mq_attr attr;
	statecb_node_t *cb_node;
	char q_name[BIN_PRIVMQ_LEN];
	binmgr_cb_msg_t state_data;
	binmgr_statecb_response_t response_msg;

	attr.mq_maxmsg = BINMGR_MAX_MSG;
	attr.mq_flags = 0;

	if (need_response) {
		attr.mq_msgsize = sizeof(binmgr_statecb_response_t);
		recv_mq = mq_open(BINMGR_CBMSG_MQ, O_RDONLY | O_CREAT, 0666, &attr);
		if (recv_mq == (mqd_t)ERROR) {
			bmdbg("Failed to open recv mq, errno %d\n", errno);
			return ERROR;
		}
	}

	memset(&state_data, 0, sizeof(binmgr_cb_msg_t));

	state_data.need_response = need_response;
	state_data.state = state;
	strncpy(state_data.bin_name, bin_name, BIN_NAME_MAX);

	send_count = 0;
	cb_node = (statecb_node_t *)sq_peek(&BIN_CBLIST(recv_binidx));
	attr.mq_msgsize = sizeof(binmgr_cb_msg_t);

	while (cb_node != NULL) {
		/* Create message queue to receive notification messages */
		snprintf(q_name, BIN_NAME_MAX, "%s%d", BINMGR_CBMSG_MQ, cb_node->pid);
		send_mq = mq_open(q_name, O_WRONLY | O_CREAT, 0666, &attr);
		if (send_mq == (mqd_t)ERROR) {
			bmdbg("Failed to open mq '%s', errno %d\n", q_name, errno);
			goto errout;
		}
		state_data.callback = cb_node->cb_info;
		
		/* Send callback message */
		ret = mq_send(send_mq, (char *)&state_data, sizeof(binmgr_cb_msg_t), BINMGR_NORMAL_PRIO);
		mq_close(send_mq);
		if (ret == OK) {
			ret = kill(cb_node->pid, SIGBM_STATE);
			if (ret == OK) {
				send_count++;
			} else {
				bmdbg("Failed to send signal, errno %d\n", errno);
				mq_unlink(q_name);
				goto errout;
			}
		} else {
			bmdbg("Failed to send mq %s\n", q_name);
			mq_unlink(q_name);
			goto errout;
		}
		cb_node = (statecb_node_t *)sq_next(cb_node);
	}

	if (need_response) {
		bmvdbg("Wait response count %d !\n", send_count);
		while (send_count > 0) {
			ret = mq_receive(recv_mq, (char *)&response_msg, sizeof(binmgr_statecb_response_t), NULL);
			if (ret == sizeof(binmgr_statecb_response_t)) {
				send_count--;
				bmvdbg("Receive message, %d response remained\n", send_count);
			} else if (ret < 0 && errno == EINTR) {
				continue;
			} else {
				bmdbg("Failed to receive response. errno %d\n", errno);
				goto errout;
			}
		}
	}

	return OK;

errout:
	if (need_response) {
		mq_close(recv_mq);
		mq_unlink(BINMGR_CBMSG_MQ);
	}

	return ERROR;
}

/****************************************************************************
 * Name: binary_manager_notify_state_changed
 *
 * Description:
 *   This function sends callback message to other binaries if registered callbacks exist.
 *
 ****************************************************************************/
void binary_manager_notify_state_changed(int bin_idx, uint8_t state)
{
	int ret;
	int count;
	int fail_count;
	int send_bin_idx;

	if (bin_idx <= 0 || state >= BINARY_STATE_MAX) {
		bmdbg("Invalid parameter: bin idx %d, state %d\n", bin_idx, state);
		return;
	}

	count = binary_manager_get_ucount();
	fail_count = 0;

	for (send_bin_idx = 1; send_bin_idx < count + 1; send_bin_idx++) {
		if (send_bin_idx != bin_idx && !sq_empty(&BIN_CBLIST(send_bin_idx))) {
			/* Notify the change of binary state if any callback is registered. */
			ret = binary_manager_send_statecb_msg(send_bin_idx, BIN_NAME(bin_idx), state, false);
			if (ret < 0) {
				fail_count++;
			}
		}
	}

	if (fail_count > 0) {
		bmdbg("Invalid parameter: bin idx %d, state %d\n", bin_idx, state);
	}
}
