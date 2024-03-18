/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
// #include <semphr.h>
#include <osdep_service.h>
#include <stdio.h>
#include <fcntl.h>
#include "mqueue.h"
#include "../kernel/mqueue/mqueue.h"

void	rtw_init_queue(_queue	*pqueue)
{
	rtw_init_listhead(&(pqueue->queue));
	rtw_spinlock_init(&(pqueue->lock));
}

u32	  rtw_queue_empty(_queue	*pqueue)
{
	return (rtw_is_list_empty(&(pqueue->queue)));
}


u32 rtw_end_of_queue_search(_list *head, _list *plist)
{
	if (head == plist) {
		return _TRUE;
	} else {
		return _FALSE;
	}
}
int rtw_init_xqueue(_xqueue *queue, const char *name, u32 message_size, u32 number_of_messages)
{
	mqd_t pmqd;
	/* The following character array of size 9 is used to store pointer address in hex represented as ascii.
	   As address has 32-bit integer value, the maximum string length in hex is 8 with 1 null character.*/
	char mq_name[9];
	struct mq_attr attr;
	int ret;
	if (!queue) {
		dbg("queue is NULL\n");
		return FAIL;
	}
	if (*queue) {
		dbg("msg queue already init\n");
		return FAIL;
	}

	attr.mq_maxmsg = number_of_messages * 5;
	attr.mq_msgsize = message_size > MQ_MAX_BYTES? MQ_MAX_BYTES : message_size;

	itoa((int) queue, mq_name, 16);
	pmqd = mq_open((const char *) mq_name, O_RDWR | O_CREAT, 0666, &attr);
	if (pmqd == (mqd_t) ERROR) {
		ret = get_errno();
		dbg("mq open fail: %d\n", ret);
		return FAIL;
	}

	*queue = pmqd;
	return SUCCESS;
}

int rtw_push_to_xqueue(_xqueue *queue, void *message, u32 timeout_ms)
{
	int prio = MQ_PRIO_MAX;
	int ret;
	if (!queue) {
		dbg("queue is NULL\n");
		return FAIL;
	}

	if (timeout_ms != 0xFFFFFFFF) {
		struct timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += timeout_ms / 1000;
		ts.tv_nsec += (timeout_ms % 1000) * 1000 * 1000;
		if (ts.tv_nsec >= NSEC_PER_SEC) {
			ts.tv_sec ++;
			ts.tv_nsec -= NSEC_PER_SEC;
		}
		if (mq_timedsend((mqd_t) *queue, message, ((mqd_t) *queue)->msgq->maxmsgsize, prio, &ts) != OK) {
			ret = get_errno();
			dbg("mq time send fail: %d\n", ret);
			return FAIL;
		}
	} else {
		if (mq_send((mqd_t) *queue, message, ((mqd_t) *queue)->msgq->maxmsgsize, prio) != OK) {
			ret = get_errno();
			dbg("mq send fail: %d\n", ret);
			return FAIL;
		}
	}
	return SUCCESS;
}

int rtw_pop_from_xqueue(_xqueue *queue, void *message, u32 timeout_ms)
{
	int prio;
	int ret;
	if (!queue) {
		dbg("queue is NULL\n");
		return FAIL;
	}

	if (timeout_ms != 0xFFFFFFFF) {
		struct timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);
		ts.tv_sec += timeout_ms / 1000;
		ts.tv_nsec += (timeout_ms % 1000) * 1000 * 1000;
		if (ts.tv_nsec >= NSEC_PER_SEC) {
			ts.tv_sec ++;
			ts.tv_nsec -= NSEC_PER_SEC;
		}
		if (mq_timedreceive((mqd_t) *queue, message, ((mqd_t) *queue)->msgq->maxmsgsize, &prio, &ts) == ERROR) {
			ret = get_errno();
			if (ETIMEDOUT != ret)
			{
				dbg("mq time receive fail errno: %d\n", ret);
			}
			return FAIL;
		}
	} else {
		if (mq_receive((mqd_t) *queue, message, ((mqd_t)*queue)->msgq->maxmsgsize, &prio) == ERROR) {
			ret = get_errno();
			dbg("mq receive fail: %d\n", ret);
			return FAIL;
		}
	}
	//printf("success\n");
	return SUCCESS;
}

int rtw_peek_from_xqueue(_xqueue *queue, void *message, u32 timeout_ms)
{
	if (!queue) {
		dbg("queue is NULL\n");
		return FAIL;
	}

	if (message == NULL) {
		dbg("message is NULL\n");
		return FAIL;
	}

	message = ((mqd_t) *queue)->msgq->nmsgs;

	return SUCCESS;
}

int rtw_deinit_xqueue(_xqueue *queue)
{
	int ret;

	if (!queue) {
		dbg("queue is NULL\n");
		return FAIL;
	}

	if(mq_close((mqd_t) *queue) != OK) {
		ret = get_errno();
		dbg("mq 0x%x close fail: %d\n", *queue, ret);
		return FAIL;
	}

	return SUCCESS;
}


