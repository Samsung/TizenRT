/******************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

/************************************************************************
 * Included Files
 ************************************************************************/

#include <sys/types.h>
#include <stdbool.h>
#include <errno.h>
#include <mqueue.h>
#include <debug.h>
#include <tinyara/arch.h>
#include <tinyara/cancelpt.h>
#include "mq_tryreceive.h"

/************************************************************************
 * Pre-processor Definitions
 ************************************************************************/

/************************************************************************
 * Private Type Declarations
 ************************************************************************/

/************************************************************************
 * Public Variables
 ************************************************************************/

/************************************************************************
 * Private Variables
 ************************************************************************/

/************************************************************************
 * Private Functions
 ************************************************************************/

/************************************************************************
 * Public Functions
 ************************************************************************/

/****************************************************************************
 * Name: mq_receive_isr
 *
 * Description:
 *   The logic is same to mq_waitreceive, but used in an interrupt handler.
 *
 * Parameters:
 *   mqdes - Message queue descriptor
 *
 * Return Value:
 *   On success, a reference to the received message.  If the message queue is empty,
 *   NULL will be returned.
 *
 * Assumptions:
 * - The caller has provided all validity checking of the input parameters
 *   using mq_verifyreceive.
 * - Interrupts should be disabled throughout this call.  This is necessary
 *   because messages can be sent from interrupt level processing.
 *
 ****************************************************************************/

static FAR struct mqueue_msg_s *mq_receive_isr(mqd_t mqdes)
{
	FAR struct mqueue_inode_s *msgq;
	FAR struct mqueue_msg_s *rcvmsg;

	if (enter_cancellation_point()) {
		/* If there is a pending cancellation, then do not perform
		 * the wait. Exit now with ECANCELED.
		 */
		set_errno(ECANCELED);
		leave_cancellation_point();
		return NULL;
	}

	/* Get a pointer to the message queue */

	msgq = mqdes->msgq;

	/* Get the message from the head of the queue */

	rcvmsg = (FAR struct mqueue_msg_s *)sq_remfirst(&msgq->msglist);

	/* If we got message, then decrement the number of messages in
	 * the queue while we are still in the critical section
	 */

	if (rcvmsg) {
		msgq->nmsgs--;
	}

	leave_cancellation_point();
	return rcvmsg;
}

/************************************************************************
 * Name: mq_tryreceive_isr
 *
 * Description:
 *   This function receives the oldest of the highest priority messages
 *   from the message queue specified by "mqdes", in an interrupt handle.
 *   If the size of the buffer in bytes (msglen) is less than the "mq_msgsize"
 *   attribute of the message queue, mq_receive will return an error.
 *   Otherwise, the selected message is removed from the queue and copied to "msg."
 *
 * Parameters:
 *   mqdes - Message Queue Descriptor
 *   msg - Buffer to receive the message
 *   msglen - Size of the buffer in bytes
 *   prio - If not NULL, the location to store message priority.
 *
 * Return Value:
 *   On success, the length of the selected message in bytes is returned.
 *   On failure, -1 (ERROR) is returned and the errno is set appropriately:
 *
 *   EAGAIN   The queue was empty, and the O_NONBLOCK flag was set
 *            for the message queue description referred to by 'mqdes'.
 *   EPERM    Message queue opened not opened for reading.
 *   EMSGSIZE 'msglen' was less than the maxmsgsize attribute of the
 *            message queue.
 *   EINTR    The call was interrupted by a signal handler.
 *   EINVAL   Invalid 'msg' or 'mqdes'
 *
 * Assumptions:
 *
 ************************************************************************/

ssize_t mq_tryreceive_isr(mqd_t mqdes, FAR char *msg, size_t msglen, FAR int *prio)
{
	FAR struct mqueue_msg_s *mqmsg;
	irqstate_t saved_state;
	ssize_t ret = ERROR;

	ASSERT(up_interrupt_context() != false);

	/* Verify the input parameters and, in case of an error, set
	 * errno appropriately.
	 */

	/* mq_receive() is a cancellation point */
	(void)enter_cancellation_point();

	if (mq_verifyreceive(mqdes, msg, msglen) != OK) {
		leave_cancellation_point();
		return ERROR;
	}

	/* Get the next message from the message queue.  We will disable
	 * pre-emption until we have completed the message received.  This
	 * is not too bad because if the receipt takes a long time, it will
	 * be because we are blocked waiting for a message and pre-emption
	 * will be re-enabled while we are blocked
	 */

	sched_lock();

	/* Furthermore, mq_waitreceive() expects to have interrupts disabled
	 * because messages can be sent from interrupt level.
	 */

	saved_state = irqsave();

	/* Get the message from the message queue */

	mqmsg = mq_receive_isr(mqdes);
	irqrestore(saved_state);

	/* Check if we got a message from the message queue.  We might
	 * not have a message if:
	 *
	 * - The message queue is empty and O_NONBLOCK is set in the mqdes
	 * - The wait was interrupted by a signal
	 */

	if (mqmsg) {
		ret = mq_doreceive(mqdes, mqmsg, msg, prio);
	}

	sched_unlock();
	leave_cancellation_point();
	return ret;
}
