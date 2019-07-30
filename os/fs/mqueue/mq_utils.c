/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

#include <stdarg.h>
#include <stdio.h>
#include <sched.h>
#include <mqueue.h>
#include <fcntl.h>
#include <errno.h>
#include <mqueue.h>
#include <queue.h>

#include <tinyara/mqueue.h>
#include <tinyara/kmalloc.h>
#include "mqueue/mqueue.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

/****************************************************************************
 * Public Variables
 ****************************************************************************/
extern sq_queue_t g_mq_info_queue;

/****************************************************************************
 * Private Variables
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mq_info_createnode
 *
 * Description:
 *   This function is used to create a new node to be added to the mq_info queue
 *
 * Parameters:
 *   name - Name assigned to the message queue.
 *
 * Return Value:
 *   Returns the node to be added to the queue.
 *
 * Assumptions:
 *
 ****************************************************************************/

static struct mq_info *mq_info_createnode(const char *name)
{
	/* Initializing the message queue structure */

	struct mq_info *info = (struct mq_info *)kmm_zalloc(sizeof(struct mq_info));

	if (info == NULL || strlen(name) > MAX_MQUEUE_PATH) {
		return NULL;
	}

	/* naming the mq_info node */
	strncpy(info->name, name, strlen(name));
	return info;
}

/****************************************************************************
 * Name: mq_info_search
 *
 * Description:
 *   This function searches the mq_info queue and returns the message queue
 *   descriptor if the mqueue with given name exists.
 *
 * Parameters:
 *   name - Name of the mqueue to be searched for
 *
 * Return Value:
 *   Returns mqueue descriptor if mqueue is found else returns NULL.
 *
 * Assumptions:
 *
 ****************************************************************************/

static struct mqueue_inode_s *mq_info_search(const char *name)
{
	/* Search thq queue if any node  matches with the given name */

	struct mq_info *temp = (struct mq_info *)sq_peek(&g_mq_info_queue);

	while (temp != NULL) {
		if (strncmp((const char *)temp->name, (const char *)name, strlen(name)) == 0) {
			return temp->msgq;
		}
		temp = (struct mq_info *)sq_next(temp);
	}
	return NULL;
}

/****************************************************************************
 * Name: mq_open
 *
 * Description:
 *   This function establish a connection between a named message queue and
 *   the calling task.  After a successful call of mq_open(), the task can
 *   reference the message queue using the address returned by the call. The
 *   message queue remains usable until it is closed by a successful call to
 *   mq_close().
 *
 * Parameters:
 *   mq_name - Name of the queue to open
 *   oflags - open flags
 *   Optional parameters.  When the O_CREAT flag is specified, two optional
 *   parameters are expected:
 *
 *     1. mode_t mode (ignored), and
 *     2. struct mq_attr *attr.  The mq_maxmsg attribute
 *        is used at the time that the message queue is
 *        created to determine the maximum number of
 *        messages that may be placed in the message queue.
 *
 * Return Value:
 *   A message queue descriptor or (mqd_t)-1 (ERROR)
 *
 * Assumptions:
 *
 ****************************************************************************/

mqd_t mq_open(FAR const char *mq_name, int oflags, ...)
{
	FAR struct mq_info *info;
	FAR struct mqueue_inode_s *msgq;
	va_list ap;
	struct mq_attr *attr;
	mqd_t mqdes;
	mode_t mode;
	int errcode;

	/* Make sure that a non-NULL name is supplied */

	if (!mq_name) {
		errcode = EINVAL;
		goto errout;
	}

	/* Make sure that the check for the existence of the message queue
	 * and the creation of the message queue are atomic with respect to
	 * other processes executing mq_open().  A simple sched_lock() should
	 * be sufficient.
	 */

	sched_lock();

	/* Get the inode for this mqueue.  This should succeed if the message
	 * queue has already been created.
	 */

	msgq = mq_info_search(mq_name);
	if (msgq) {

		if ((oflags & (O_CREAT | O_EXCL)) == (O_CREAT | O_EXCL)) {
			errcode = EEXIST;
			goto errout_with_lock;
		}

		mqdes = mq_descreate(NULL, msgq, oflags);

		if (!mqdes) {
			errcode = ENOMEM;
			goto errout_with_lock;
		}

		info = msgq->info;
		info->crefs++;

	} else {
		/* The mqueue does not exists.  Were we asked to create it? */

		if ((oflags & O_CREAT) == 0) {
			/* The mqueue does not exist and O_CREAT is not set */

			errcode = ENOENT;
			goto errout_with_lock;
		}

		/* Create the mqueue.  First we have to extract the additional
		 * parameters from the variable argument list.
		 */

		va_start(ap, oflags);
		mode = va_arg(ap, mode_t);
		attr = va_arg(ap, FAR struct mq_attr *);
		va_end(ap);

		/* Allocate memory for the new message queue. */

		msgq = (FAR struct mqueue_inode_s *)mq_msgqalloc(mode, attr);
		if (!msgq) {
			errcode = ENOSPC;
			goto errout_with_lock;
		}

		/* Create a message queue descriptor for the TCB */

		mqdes = mq_descreate(NULL, msgq, oflags);
		if (!mqdes) {
			errcode = ENOMEM;
			goto errout_with_msgq;
		}

		/* Create a mqueue node with given name and add it to the
		list of mqueue nodes */

		info = mq_info_createnode(mq_name);

		if (info == NULL) {
			errcode = ENOMEM;
			goto errout_with_mqdes;
		}

		sq_addlast((sq_entry_t *)info, &g_mq_info_queue);

		info->msgq = msgq;
		msgq->info = info;
		info->crefs++;
	}
	sched_unlock();
	return mqdes;

errout_with_mqdes:
	mq_desclose(mqdes);
	mq_msgfree(msgq);
errout_with_msgq:
	mq_msgqfree(msgq);
errout_with_lock:
	sched_unlock();
errout:
	set_errno(errcode);
	return (mqd_t)ERROR;
}

static void mq_info_release(FAR struct mq_info *info)
{
	FAR struct mqueue_inode_s *msgq;

	if (!info) {
		return;
	}

	if (info->crefs > 0) {
		info->crefs--;
	}

	/* If the number of references to the node is zero, then free the mqueue */

	if (info->crefs <= 0 && (info->flags & MQNODEFLAG_DELETED) != 0) {
		msgq = info->msgq;
		mq_msgqfree(msgq);

		/* Remove the mqueue structure from the node */
		info->msgq = NULL;

		sq_rem((sq_entry_t *)info, &g_mq_info_queue);
		kmm_free(info);
	}

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
	FAR struct mqueue_inode_s *msgq;
	FAR struct mq_info *info;

	sched_lock();
	/* Verify the inputs */

	if (mqdes) {

		/* Find the message queue associated with the message descriptor */

		msgq = mqdes->msgq;
		DEBUGASSERT(msgq && msgq->info);

		/* Close/free the message descriptor */

		mq_desclose(mqdes);
		info = msgq->info;
		mq_info_release(info);
	}

	sched_unlock();
	return OK;
}

/************************************************************************
 * Name: mq_unlink
 *
 * Description:
 *   This function removes the message queue named by "mq_name." If one
 *   or more tasks have the message queue open when mq_unlink() is called,
 *   removal of the message queue is postponed until all references to the
 *   message queue have been closed.
 *
 * Parameters:
 *   mq_name - Name of the message queue
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *
 ************************************************************************/

int mq_unlink(FAR const char *mq_name)
{
	FAR struct mq_info *info;
	FAR struct mqueue_inode_s *msgq;
	int errcode;

	sched_lock();

	msgq = mq_info_search(mq_name);
	if (!msgq) {
		/* There is no mqueue in the list that includes in this path */

		errcode = ENOENT;
		goto errout;
	}

	info = msgq->info;
	info->flags |= MQNODEFLAG_DELETED;

	if (info->crefs > 1) {
		/* Some references to the mqueue are still open.
		In our implementation, we will allow unlink only if references to the
		mqueue are zero. Else we return an error code */
		errcode = EBUSY;
		goto errout;
	}

	mq_info_release(info);

	sched_unlock();
	return OK;
errout:
	set_errno(errcode);
	sched_unlock();
	return ERROR;
}
