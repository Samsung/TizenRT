/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
 *  fs/mqueue/mq_close.c
 *
 *   Copyright (C) 2007, 2009, 2013-2014 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sched.h>
#include <mqueue.h>
#include <assert.h>
#include <debug.h>
#include <stdio.h>
#include <errno.h>

#include <tinyara/kmalloc.h>
#include <tinyara/sched.h>
#include <tinyara/mqueue.h>
#include <tinyara/arch.h>

#include "inode/inode.h"
#include "mqueue/mqueue.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/************************************************************************
 * Name: mq_desc_in_grouplist
 *
 * Description:
 *   This function checks if a message queue descriptor is present in the
 *   calling task's group list of mq des.
 *
 * Parameters:
 *   mqdes - Message queue descriptor
 *
 * Return Value:
 *   OK - if mqdes is present in the calling task group list of mqdes
 *   EBADF - if mqdes is not present in the calling task group of mqdes
 *
 ************************************************************************/

int mq_desc_in_grouplist(mqd_t mqdes)
{
	int ret = -EBADF;
    	mqd_t mqdes_ptr;

	/* If we are in irq hanlder, then we must NOT perform this check.
	 * Because in irq handler, we will not be able to find which task or
	 * task's group opened the current mqdes */
	if (up_interrupt_context()) {
		return OK;
	}

	FAR struct task_group_s *group = sched_self()->group;

	DEBUGASSERT(mqdes != NULL && group != NULL);

    	sched_lock();
    	mqdes_ptr = (mqd_t)sq_peek(&group->tg_msgdesq);
    	while (mqdes_ptr) {
        	if (mqdes_ptr == mqdes) {
            		ret = OK;
            		break;
        	}
        	mqdes_ptr = (mqd_t)sq_next(mqdes_ptr);
    	}
    	sched_unlock();

	return ret;
}

/****************************************************************************
 * Name: mq_close_group
 *
 * Description:
 *   This function is used to indicate that all threads in the group are
 *   finished with the specified message queue mqdes.  The mq_close_group()
 *   deallocates any system resources allocated by the system for use by
 *   this task for its message queue.
 *
 * Parameters:
 *   mqdes - Message queue descriptor.
 *   group - Group that has the open descriptor.
 *
 * Return Value:
 *   0 (OK) if the message queue is closed successfully,
 *   otherwise, -1 (ERROR).
 *
 ****************************************************************************/

int mq_close_group(mqd_t mqdes, FAR struct task_group_s *group)
{
	int ret = OK;
	FAR struct mqueue_inode_s *msgq;
	FAR struct inode *inode;

	DEBUGASSERT(mqdes != NULL && group != NULL);

	/* Verify the inputs */

	if (mqdes) {
		sched_lock();

		/* If there is no mqdes in one's group, skip to desclose and inode release. */
		if (mq_desc_in_grouplist(mqdes) == OK) {

			/* Find the message queue associated with the message descriptor */

			msgq = mqdes->msgq;
			DEBUGASSERT(msgq && msgq->inode);

			/* Close/free the message descriptor */

			mq_desclose_group(mqdes, group);

			/* Get the inode from the message queue structure */

			inode = msgq->inode;
			DEBUGASSERT(inode->u.i_mqueue == msgq);

			/* Decrement the reference count on the inode, possibly freeing it */

			mq_inode_release(inode);
		} else {
			dbg("ERROR: mqdes(%x) is not in this thread's group.\n", mqdes);
			ret = ERROR;
		}
		sched_unlock();
	}

	return ret;
}

/****************************************************************************
 * Name: mq_close
 *
 * Description:
 *   This function is used to indicate that the calling task is finished
 *   with the specified message queue mqdes.  The mq_close() deallocates
 *   any system resources allocated by the system for use by this task for
 *   its message queue.
 *
 *   If the calling task has attached a notification to the message queue
 *   via this mqdes, this attachment will be removed and the message queue
 *   is available for another process to attach a notification.
 *
 * Parameters:
 *   mqdes - Message queue descriptor.
 *
 * Return Value:
 *   0 (OK) if the message queue is closed successfully,
 *   otherwise, -1 (ERROR).
 *
 * Assumptions:
 * - The behavior of a task that is blocked on either a mq_send() or
 *   mq_receive() is undefined when mq_close() is called.
 * - The results of using this message queue descriptor after a successful
 *   return from mq_close() is undefined.
 *
 ****************************************************************************/

int mq_close(mqd_t mqdes)
{
	FAR struct tcb_s *rtcb;
	int ret;

	rtcb = (FAR struct tcb_s *)sched_self();
	DEBUGASSERT(mqdes != NULL && rtcb != NULL && rtcb->group != NULL);

	/* Lock the scheduler to prevent any asynchrounous task delete operation
	* (unlikely).
	*/

	sched_lock();
	ret = mq_close_group(mqdes, rtcb->group);
	sched_unlock();

	return ret;
}

/****************************************************************************
 * Name: mq_inode_release
 *
 * Description:
 *   Release a reference count on a message queue inode.
 *
 * Parameters:
 *   inode - The message queue inode
 *
 * Return Value:
 *   None
 *
 ****************************************************************************/

void mq_inode_release(FAR struct inode *inode)
{
	/* Decrement the reference count on the inode */

	inode_semtake();
	if (inode->i_crefs > 0) {
		inode->i_crefs--;
	}

	/* If the message queue was previously unlinked and the reference count
	 * has decremented to zero, then release the message queue and delete
	 * the inode now.
	 */

	if (inode->i_crefs <= 0 && (inode->i_flags & FSNODEFLAG_DELETED) != 0) {
		FAR struct mqueue_inode_s *msgq = inode->u.i_mqueue;
		DEBUGASSERT(msgq);

		/* Free the message queue (and any messages left in it) */

		mq_msgqfree(msgq);
		inode->u.i_mqueue = NULL;

		/* Release and free the inode container */

		inode_semgive();
		inode_free(inode->i_child);
		kmm_free(inode);
		return;
	}

	inode_semgive();
}
