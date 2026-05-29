/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include "ameba.h"
#include "os_wrapper.h"
#include <fcntl.h>
#include "../kernel/mqueue/mqueue.h"

int rtos_queue_create(rtos_queue_t *pp_handle, uint32_t msg_num, uint32_t msg_size)
{
	mqd_t pmqd;
	/* The following character array of size 9 is used to store pointer address in hex represented as ascii.
	   As address has 32-bit integer value, the maximum string length in hex is 8 with 1 null character.*/
	char mq_name[9];
	struct mq_attr attr;
	int ret;

	if (!pp_handle) {
		dbg("pp_handle is NULL\n");
		return FAIL;
	}

	if (*pp_handle) {
		dbg("msg queue already init\n");
		return FAIL;
	}

	attr.mq_maxmsg = msg_num;
	attr.mq_msgsize = msg_size > MQ_MAX_BYTES? MQ_MAX_BYTES : msg_size;

	itoa((int) pp_handle, mq_name, 16);

	pmqd = mq_open((const char *) mq_name, O_RDWR | O_CREAT, 0666, &attr);
	if (pmqd == (mqd_t) ERROR) {
		ret = get_errno();
		dbg("mq open fail: %d\n", ret);
		return FAIL;
	}

	*pp_handle = pmqd;

	return SUCCESS;
}

int rtos_queue_delete(rtos_queue_t p_handle)
{
	/* TIZEN_RT_NOT_REQUIRED */
	dbg("%s not implemented\n", __FUNCTION__);
	return FAIL;
	/* Below implementation is based on osif */
	// int ret;

	// if (!p_handle) {
	// 	dbg("p_handle is NULL\n");
	// 	return FAIL;
	// }

	// if (mq_close((mqd_t) p_handle) != OK) {
	// 	ret = get_errno();
	// 	dbg("mq 0x%x close fail: %d\n", p_handle, ret);
	// 	return FAIL;
	// }

	// return SUCCESS;
}

uint32_t rtos_queue_message_waiting(rtos_queue_t p_handle)
{
	/* TIZEN_RT_NOT_REQUIRED */
	dbg("%s not implemented\n", __FUNCTION__);
	return FAIL;
	/* Below implementation is based on osif */
	// if (!p_handle) {
	// 	dbg("p_handle is NULL\n");
	// 	return FAIL;
	// }

	// if (p_msg_num == NULL) {
	// 	dbg("p_msg_num is NULL\n");
	// 	return FAIL;
	// }

	// *p_msg_num = ((mqd_t) p_handle)->msgq->nmsgs;
}

int rtos_queue_send(rtos_queue_t p_handle, void *p_msg, uint32_t wait_ms)
{
	/* Below implementation is based on osif */
	int prio = MQ_PRIO_MAX;
	int ret;

	if (!p_handle) {
		dbg("p_handle is NULL\n");
		return FAIL;
	}

	if (wait_ms != RTOS_MAX_TIMEOUT && rtos_critical_is_in_interrupt() == false) {
		struct timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += wait_ms / 1000;
		ts.tv_nsec += (wait_ms % 1000) * 1000 * 1000;
		if (ts.tv_nsec >= NSEC_PER_SEC) {
			ts.tv_sec ++;
			ts.tv_nsec -= NSEC_PER_SEC;
		}
		if (mq_timedsend((mqd_t) p_handle, p_msg, ((mqd_t) p_handle)->msgq->maxmsgsize, prio, &ts) != OK) {
			ret = get_errno();
			dbg("mq time send fail: %d\n", ret);
			return FAIL;
		}
	} else {
		if (mq_send((mqd_t) p_handle, p_msg, ((mqd_t) p_handle)->msgq->maxmsgsize, prio) != OK) {
			ret = get_errno();
			dbg("mq send fail: %d\n", ret);
			return FAIL;
		}
	}

	return _SUCCESS;
}

int rtos_queue_send_to_front(rtos_queue_t p_handle, void *p_msg, uint32_t wait_ms)
{
	/* TIZEN_RT_NOT_REQUIRED */
	dbg("%s not implemented\n", __FUNCTION__);
	return FAIL;
}

int rtos_queue_receive(rtos_queue_t p_handle, void *p_msg, uint32_t wait_ms)
{
	int prio;
	int ret;

	if (!p_handle) {
		dbg("p_handle is NULL\n");
		return FAIL;
	}

	if (rtos_critical_is_in_interrupt() == true) {
		DEBUGASSERT(wait_ms == RTOS_TIMEOUT_NONE);

		irqstate_t flags = tizenrt_critical_enter();
		FAR struct mqueue_msg_s *mqmsg = mq_waitreceive((mqd_t)p_handle);
		tizenrt_critical_exit(flags);
		mq_doreceive((mqd_t)p_handle, mqmsg, p_msg, &prio);
		return SUCCESS;
	}

	if (wait_ms != RTOS_MAX_TIMEOUT) {
		struct timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += wait_ms / 1000;
		ts.tv_nsec += (wait_ms % 1000) * 1000 * 1000;
		if (ts.tv_nsec >= NSEC_PER_SEC) {
			ts.tv_sec ++;
			ts.tv_nsec -= NSEC_PER_SEC;
		}
		if (mq_timedreceive((mqd_t) p_handle, p_msg, ((mqd_t) p_handle)->msgq->maxmsgsize, &prio, &ts) == ERROR) {
			ret = get_errno();
			if (ETIMEDOUT != ret) {
				dbg("mq time receive fail errno: %d\n", ret);
			}
			return FAIL;
		}
	} else {
		if (mq_receive((mqd_t) p_handle, p_msg, ((mqd_t) p_handle)->msgq->maxmsgsize, &prio) == ERROR) {
			ret = get_errno();
			dbg("mq receive fail: %d\n", ret);
			return FAIL;
		}
	}

	return SUCCESS;
}

int rtos_queue_peek(rtos_queue_t p_handle, void *p_msg, uint32_t wait_ms)
{
	/* TIZEN_RT_NOT_REQUIRED */
	dbg("%s not implemented\n", __FUNCTION__);
	return FAIL;
}
