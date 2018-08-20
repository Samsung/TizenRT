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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* tinyara includes */
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <assert.h>
#include <errno.h>
#include <tinyara/clock.h>
#include <tinyara/arch.h>
#include <tinyara/cancelpt.h>
#include <tinyara/kthread.h>
#include <tinyara/semaphore.h>
#include <sys/types.h>

/* lwIP includes. */
#include <net/lwip/stats.h>
#include <net/lwip/opt.h>
#include <net/lwip/debug.h>
#include <net/lwip/def.h>
#include <net/lwip/sys.h>
#include <net/lwip/mem.h>
#include <net/lwip/memp.h>
#include <net/lwip/pbuf.h>
#include <net/lwip/arch/cc.h>

/* TinyAra RTOS implementation of the lwip operating system abstraction */
#include <net/lwip/arch/sys_arch.h>

/* This is the number of threads that can be started with sys_thread_new() */
#define SYS_THREAD_MAX 6

static u16_t s_nextthread = 0;

/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_new
 *---------------------------------------------------------------------------*
 * Description:
 *      Creates a new mailbox
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      int queue_sz            -- Size of elements in the mailbox
 * Outputs:
 *      err_t                   -- ERR_OK if message posted, else ERR_MEM
 *---------------------------------------------------------------------------*/
err_t sys_mbox_new(sys_mbox_t *mbox, int queue_sz)
{
	err_t err = ERR_OK;
	mbox->is_valid = 1;
#if LWIP_STATS
	mbox->id = lwip_stats.sys.mbox.used + 1;
#endif
	mbox->queue_size = queue_sz;
	mbox->wait_send = 0;
	mbox->wait_fetch = 0;
	mbox->front = mbox->rear = 0;
	sys_sem_new(&(mbox->mail), 0);
	sys_sem_new(&(mbox->mutex), 1);

#if SYS_STATS
	SYS_STATS_INC_USED(mbox);
#endif							/* SYS_STATS */

	LWIP_DEBUGF(SYS_DEBUG, ("Succesfully Created MBOX with id %d", mbox->id));
	return err;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_free
 *---------------------------------------------------------------------------*
 * Description:
 *      Deallocates a mailbox. If there are messages still present in the
 *      mailbox when the mailbox is deallocated, it is an indication of a
 *      programming error in lwIP and the developer should be notified.
 * Inputs:
 *      sys_mbox_t *mbox         -- Handle of mailbox
 *---------------------------------------------------------------------------*/
void sys_mbox_free(sys_mbox_t *mbox)
{
	if (mbox != SYS_MBOX_NULL) {

		LWIP_DEBUGF(SYS_DEBUG, ("Deleting MBOX with id %d", mbox->id));

		mbox->is_valid = 0;
		mbox->id = 0;
		mbox->queue_size = 0;
		mbox->wait_send = 0;
		mbox->wait_fetch = 0;
		sys_sem_free(&(mbox->mail));
		sys_sem_free(&(mbox->mutex));

		LWIP_DEBUGF(SYS_DEBUG, ("Succesfully deleted MBOX with id %d", mbox->id));
#if SYS_STATS
		SYS_STATS_DEC(mbox.used);
#endif							/* SYS_STATS */
	}

	return;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_post (Blocking Call)
 *---------------------------------------------------------------------------*
 * Description:
 *      Post the "msg" to the mailbox.
 * Inputs:
 *      sys_mbox_t mbox        -- Handle of mailbox
 *      void *msg              -- Pointer to data to post
 *---------------------------------------------------------------------------*/
void sys_mbox_post(sys_mbox_t *mbox, void *msg)
{
	u8_t first_msg = 0;
	u32_t tmp = 0;
	u32_t status = OK;
	status = sys_arch_sem_wait(&(mbox->mutex), 0);
	if (status == SYS_ARCH_CANCELED) {
		return;
	}
	LWIP_DEBUGF(SYS_DEBUG, ("mbox %p msg %p\n", (void *)mbox, (void *)msg));
	/* Wait while the queue is full */
	tmp = (mbox->rear + 1) % mbox->queue_size;
	if (tmp == mbox->front) {
		LWIP_DEBUGF(SYS_DEBUG, ("Queue Full, Wait until gets free\n"));
	}
	while (tmp == mbox->front) {
		mbox->wait_send++;
		sys_sem_signal(&(mbox->mutex));
		sys_arch_sem_wait(&(mbox->mail), 0);
		status = sys_arch_sem_wait(&(mbox->mutex), 0);
		mbox->wait_send--;
		if (status == SYS_ARCH_CANCELED) {
			return;
		}
	}

	if (mbox->rear == mbox->front) {
		first_msg = 1;
	} else {
		first_msg = 0;
	}

	mbox->rear = tmp;
	mbox->msgs[mbox->rear] = msg;
	LWIP_DEBUGF(SYS_DEBUG, ("Post SUCCESS\n"));

	/* If msg was posted to an empty queue, Release semaphore for
	   some fetch api blocked on this sem due to Empty queue. */
	if (first_msg && mbox->wait_fetch) {
		sys_sem_signal(&(mbox->mail));
	}

	sys_sem_signal(&(mbox->mutex));
	return;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_trypost
 *---------------------------------------------------------------------------*
 * Description:
 *      Try to post the "msg" to the mailbox.  Returns immediately with
 *      error if cannot.
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      void *msg               -- Pointer to data to post
 * Outputs:
 *      err_t                   -- ERR_OK if message posted, else ERR_MEM
 *                                  if not.
 *---------------------------------------------------------------------------*/
err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
	err_t err = ERR_OK;
	u8_t first_msg = 0;
	u32_t tmp = 0;
	sys_arch_sem_wait(&(mbox->mutex), 0);

	LWIP_DEBUGF(SYS_DEBUG, ("mbox %p msg %p\n", (void *)mbox, (void *)msg));
	/* Check if the queue is full */
	tmp = (mbox->rear + 1) % mbox->queue_size;
	if (tmp == mbox->front) {
		LWIP_DEBUGF(SYS_DEBUG, ("Queue Full, returning error\n"));
		err = ERR_MEM;
		goto errout_with_mutex;
	}

	if (mbox->rear == mbox->front) {
		first_msg = 1;
	} else {
		first_msg = 0;
	}

	mbox->rear = tmp;
	mbox->msgs[mbox->rear] = msg;
	LWIP_DEBUGF(SYS_DEBUG, ("Post SUCCESS\n"));

	/* If msg was posted to an empty queue, Release semaphore for
	   some fetch api blocked on this sem due to Empty queue. */
	if (first_msg && mbox->wait_fetch) {
		sys_sem_signal(&(mbox->mail));
	}

errout_with_mutex:
	sys_sem_signal(&(mbox->mutex));

	return err;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_mbox_fetch
 *---------------------------------------------------------------------------*
 * Description:
 *      Blocks the thread until a message arrives in the mailbox, but does
 *      not block the thread longer than "timeout" milliseconds (similar to
 *      the sys_arch_sem_wait() function). The "msg" argument is a result
 *      parameter that is set by the function (i.e., by doing "*msg =
 *      ptr"). The "msg" parameter maybe NULL to indicate that the message
 *      should be dropped.
 *
 *      The return values are the same as for the sys_arch_sem_wait() function:
 *      Number of milliseconds spent waiting or SYS_ARCH_TIMEOUT if there was a
 *      timeout.
 *
 *      Note that a function with a similar name, sys_mbox_fetch(), is
 *      implemented by lwIP.
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      void **msg              -- Pointer to pointer to msg received
 *      u32_t timeout           -- Number of milliseconds until timeout
 * Outputs:
 *      u32_t                   -- SYS_ARCH_CANCELED if the operation canceled,
 *				   SYS_ARCH_TIMEOUT if timeout, else number
 *                                 of milliseconds until received.
 *---------------------------------------------------------------------------*/
u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout)
{
	u32_t time = 0;
	u32_t status = OK;

	/* The mutex lock is quick so we don't bother with the timeout
	   stuff here. */

	status = sys_arch_sem_wait(&(mbox->mutex), 0);
	if (status == SYS_ARCH_CANCELED) {
		return SYS_ARCH_CANCELED;
	}

	/* wait while the queue is empty */
	while (mbox->front == mbox->rear) {
		mbox->wait_fetch++;
		sys_sem_signal(&(mbox->mutex));

		/* We block while waiting for a mail to arrive in the mailbox. We
		   must be prepared to timeout. */
		if (timeout != 0) {

			if (timeout < MSEC_PER_TICK)
				timeout = MSEC_PER_TICK;

			time = sys_arch_sem_wait(&(mbox->mail), timeout);

			if (time == SYS_ARCH_TIMEOUT) {
				sys_arch_sem_wait(&(mbox->mutex), 0);
				mbox->wait_fetch--;
				sys_sem_signal(&(mbox->mutex));
				return SYS_ARCH_TIMEOUT;
			}
		} else {
			sys_arch_sem_wait(&(mbox->mail), 0);
		}

		status = sys_arch_sem_wait(&(mbox->mutex), 0);
		mbox->wait_fetch--;
		if (status == SYS_ARCH_CANCELED) {
			return SYS_ARCH_CANCELED;
		}
	}

	mbox->front = (mbox->front + 1) % mbox->queue_size;
	if (msg != NULL) {
		*msg = mbox->msgs[mbox->front];
		LWIP_DEBUGF(SYS_DEBUG, (" mbox %p msg %p\n", (void *)mbox, *msg));
	} else {
		LWIP_DEBUGF(SYS_DEBUG, (" mbox %p, null msg\n", (void *)mbox));
	}

	/* We just fetched a msg, Release semaphore for
	   some post api blocked on this sem due to queue full. */
	if (mbox->wait_send) {
		sys_sem_signal(&(mbox->mail));
	}

	sys_sem_signal(&(mbox->mutex));

	return time;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_mbox_tryfetch
 *---------------------------------------------------------------------------*
 * Description:
 *      Similar to sys_arch_mbox_fetch, but if message is not ready
 *      immediately, we'll return with SYS_MBOX_EMPTY.  On success, 0 is
 *      returned.
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      void **msg              -- Pointer to pointer to msg received
 * Outputs:
 *      u32_t                   -- SYS_MBOX_EMPTY if no messages.  Otherwise,
 *                                  return ERR_OK.
 *---------------------------------------------------------------------------*/
u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
	u32_t err = ERR_OK;
	/* The mutex lock is quick so we don't bother with the timeout
	   stuff here. */
	sys_arch_sem_wait(&(mbox->mutex), 0);

	/* check if the queue is empty */
	if (mbox->front == mbox->rear) {
		LWIP_DEBUGF(SYS_DEBUG, ("SYS_MBOX_EMPTY , returning\n"));
		err = SYS_MBOX_EMPTY;
		goto errout_with_mutex;
	}

	mbox->front = (mbox->front + 1) % mbox->queue_size;
	if (msg != NULL) {
		LWIP_DEBUGF(SYS_DEBUG, ("mbox %p msg %p\n", (void *)mbox, *msg));
		*msg = mbox->msgs[mbox->front];
	} else {
		LWIP_DEBUGF(SYS_DEBUG, ("mbox %p, null msg\n", (void *)mbox));
	}

	/* We just fetched a msg, Release semaphore for
	   some post api blocked on this sem due to queue full. */
	if (mbox->wait_send) {
		sys_sem_signal(&(mbox->mail));
	}

errout_with_mutex:
	sys_sem_signal(&(mbox->mutex));
	return err;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_valid
 *---------------------------------------------------------------------------*
 * Description:
 *      Validate whether the mbox still exists and functioning or not.
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 * Outputs:
 *      int                     -- 1 if mbox is valid and functioning
 *                                                                      otherwise 0
 *---------------------------------------------------------------------------*/
int sys_mbox_valid(sys_mbox_t *mbox)
{
	LWIP_DEBUGF(SYS_DEBUG, ("mbox->id = %d ", mbox->id));
	if (mbox->is_valid == 1) {
		LWIP_DEBUGF(SYS_DEBUG, ("Mbox (%d) Valid", mbox->id));
		return 1;
	} else {
		LWIP_DEBUGF(SYS_DEBUG, ("Mbox (%d) Invalid", mbox->id));
		return 0;
	}
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_set_invalid
 *---------------------------------------------------------------------------*
 * Description:
 *      sets mboox invalid
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 * Outputs:
 *---------------------------------------------------------------------------*/
void sys_mbox_set_invalid(sys_mbox_t *mbox)
{
	mbox->is_valid = 0;
	return;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_sem_new
 *---------------------------------------------------------------------------*
 * Description:
 *      Creates and returns a new semaphore. The "ucCount" argument specifies
 *      the initial state of the semaphore.
 *      NOTE: Currently this routine only creates counts of 1 or 0
 * Inputs:
 *      sys_sem_t sem         -- Handle of semaphore
 *      u8_t count            -- Initial count of semaphore
 * Outputs:
 *      err_t                 -- ERR_OK if semaphore created
 *---------------------------------------------------------------------------*/
err_t sys_sem_new(sys_sem_t *sem, u8_t count)
{
	int status = -1;

	status = sem_init(sem, 0, count);
	if (status != OK) {
#if SYS_STATS
		SYS_STATS_INC(sem.err);
#endif
		return ERR_MEM;
	}
#if SYS_STATS
	SYS_STATS_INC_USED(sem);
#endif							/* SYS_STATS */

	sem_setprotocol(sem, SEM_PRIO_NONE);

	return ERR_OK;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_sem_wait
 *---------------------------------------------------------------------------*
 * Description:
 *      Blocks the thread while waiting for the semaphore to be
 *      signaled. If the "timeout" argument is non-zero, the thread should
 *      only be blocked for the specified time (measured in
 *      milliseconds).
 *
 *      If the timeout argument is non-zero, the return value is the number of
 *      milliseconds spent waiting for the semaphore to be signaled. If the
 *      semaphore wasn't signaled within the specified time, the return value is
 *      SYS_ARCH_TIMEOUT. If the thread didn't have to wait for the semaphore
 *      (i.e., it was already signaled), the function may return zero.
 *
 *      Notice that lwIP implements a function with a similar name,
 *      sys_sem_wait(), that uses the sys_arch_sem_wait() function.
 * Inputs:
 *      sys_sem_t sem           -- Semaphore to wait on
 *      u32_t timeout           -- Number of milliseconds until timeout
 * Outputs:
 *      u32_t                   -- SYS_ARCH_CANCELED if the operation canceled,
 *				   SYS_ARCH_TIMEOUT if timeout, else time elapsed.
 *---------------------------------------------------------------------------*/
u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
	clock_t start = clock_systimer();
	int status = OK;
	int remaining_time;

	if (timeout == 0) {

		while (sem_wait(sem) != OK) {
			/* Handle the special case where the semaphore wait was
			 * awakened by the receipt of a signal.
			 * Restart If signal is EINTR else crash on Assert as we can't receive ETIMEDOUT
			 */
			status = get_errno();
			if (status == ECANCELED) {
				return SYS_ARCH_CANCELED;
			}
			LWIP_ASSERT("status == EINTR", status == EINTR);
		}
		status = OK;
	} else {
		remaining_time = timeout;
		while ((status = sem_tickwait(sem, clock_systimer(), MSEC2TICK(remaining_time))) != OK) {
			/* Handle the special case where the semaphore wait was
			 * awakened by the receipt of a signal.
			 * Restart If signal is EINTR else break if ETIMEDOUT
			 */
			if (status == -ECANCELED) {
				return SYS_ARCH_CANCELED;
			} else if (status == -ETIMEDOUT) {
				return SYS_ARCH_TIMEOUT;
			} else {
				/* calculate remaining timeout */
				remaining_time -= TICK2MSEC(clock_systimer() - start);
				if (remaining_time < MSEC_PER_TICK) {
					return SYS_ARCH_TIMEOUT;
				}
			}
		}

	}

	clock_t end = clock_systimer();
	return TICK2MSEC(end - start);
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_sem_signal
 *---------------------------------------------------------------------------*
 * Description:
 *      Signals (releases) a semaphore
 * Inputs:
 *      sys_sem_t sem           -- Semaphore to signal
 *---------------------------------------------------------------------------*/
void sys_sem_signal(sys_sem_t *sem)
{
	int status = -1;
	status = sem_post(sem);
	if (status) {
		LWIP_DEBUGF(SYS_DEBUG, ("sem_post error"));
	}
	return;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_sem_free
 *---------------------------------------------------------------------------*
 * Description:
 *      Deallocates a semaphore
 * Inputs:
 *      sys_sem_t sem           -- Semaphore to free
 *---------------------------------------------------------------------------*/
void sys_sem_free(sys_sem_t *sem)
{
	int status = -1;
	status = sem_destroy(sem);
	if (status) {
		LWIP_DEBUGF(SYS_DEBUG, ("sem_destroy error --> Could not destroy the semaphore"));
	} else {
#if SYS_STATS
		SYS_STATS_DEC(sem.used);
#endif							/* SYS_STATS */
	}
	return;
}

/*-----------------------------------------------------------------------------------*/
int sys_sem_valid(sys_sem_t *sem)
{
	if (sem == SYS_SEM_NULL) {
		return 0;
	} else {
		return 1;
	}
}

/*-----------------------------------------------------------------------------------*/
void sys_sem_set_invalid(sys_sem_t *sem)
{
	sem = SYS_SEM_NULL;
	return;
}

/*-----------------------------------------------------------------------------------*/
// Initialize sys arch
void sys_init(void)
{
	// keep track of how many threads have been created
	s_nextthread = 0;
	return;
}

/*-----------------------------------------------------------------------------------*/

/* Mutexes*/
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
#if LWIP_COMPAT_MUTEX == 0
/* Create a new mutex*/
err_t sys_mutex_new(sys_mutex_t *mutex)
{
	int status = 0;

	if (NULL == mutex) {
		mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	}
	if (mutex == NULL) {
#if SYS_STATS
		SYS_STATS_INC(mutex.err);
#endif							/* SYS_STATS */
		return ERR_MEM;
	}
	status = pthread_mutex_init(mutex, NULL);
	if (status) {
		return ERR_MEM;
	}
#if SYS_STATS
	SYS_STATS_INC_USED(mutex);
#endif							/* SYS_STATS */

	return ERR_OK;
}

/*-----------------------------------------------------------------------------------*/
/* Deallocate a mutex*/
void sys_mutex_free(sys_mutex_t *mutex)
{
	int status = 0;
	status = pthread_mutex_destroy(mutex);
	if (status) {
		LWIP_DEBUGF(SYS_DEBUG, ("pthread_mutex_destroy error --> Could not free the mutex"));
	} else {
#if SYS_STATS
		SYS_STATS_DEC(mutex.used);
#endif							/* SYS_STATS */
	}
	return;
}

/*-----------------------------------------------------------------------------------*/
/* Lock a mutex*/
void sys_mutex_lock(sys_mutex_t *mutex)
{
	int status = 0;
	status = pthread_mutex_lock(mutex);
	if (status) {
		LWIP_DEBUGF(SYS_DEBUG, ("\n pthread_mutex_lock error --> Could not lock the mutex \n"));
	}
	return;
}

/*-----------------------------------------------------------------------------------*/
/* Unlock a mutex*/
void sys_mutex_unlock(sys_mutex_t *mutex)
{
	int status = 0;
	status = pthread_mutex_unlock(mutex);
	if (status) {
		LWIP_DEBUGF(SYS_DEBUG, ("sys_mutex_unlock: Could not unlock the mutex"));
	}
}
#endif							/*LWIP_COMPAT_MUTEX */
/*-----------------------------------------------------------------------------------*/

u32_t sys_now(void)
{
	return TICK2MSEC(clock_systimer());
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_thread_new
 *---------------------------------------------------------------------------*
 * Description:
 *      Starts a new user thread with priority "prio" that will begin its
 *      execution in the function "thread()". The "arg" argument will be
 *      passed as an argument to the thread() function. The id of the new
 *      thread is returned. Both the id and the priority are system
 *      dependent.
 * Inputs:
 *      char *name                -- Name of thread
 *      void (*thread)(void *arg) -- Pointer to function to run.
 *      void *arg                 -- Argument passed into function
 *      int stacksize             -- Required stack amount in bytes
 *      int priority              -- Thread priority
 * Outputs:
 *      sys_thread_t              -- Pointer to thread handle.
 *---------------------------------------------------------------------------*/
sys_thread_t sys_thread_new(const char *name, lwip_thread_fn entry_function, void *arg, int stacksize, int priority)
{

	if (s_nextthread < SYS_THREAD_MAX) {
		sys_thread_t new_thread;
		new_thread = task_create(name, priority, stacksize, (main_t) entry_function, (char *const *)NULL);
		if (new_thread < 0) {
			int errval = errno;
			LWIP_DEBUGF(SYS_DEBUG, ("Failed to create new_thread: %d", errval));
			return -errval;
		} else {
			LWIP_DEBUGF(SYS_DEBUG, ("Created New Thread with pid %d", new_thread));
			s_nextthread++;
			return new_thread;
		}
	}
	return -1;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_kernel_thread_new
 *---------------------------------------------------------------------------*
 * Description:
 *      Starts a new kernel thread with priority "prio" that will begin its
 *      execution in the function "thread()". The "arg" argument will be
 *      passed as an argument to the thread() function. The id of the new
 *      thread is returned. Both the id and the priority are system
 *      dependent.
 * Inputs:
 *      char *name                -- Name of thread
 *      void (*thread)(void *arg) -- Pointer to function to run.
 *      void *arg                 -- Argument passed into function
 *      int stacksize             -- Required stack amount in bytes
 *      int priority              -- Thread priority
 * Outputs:
 *      sys_thread_t              -- Pointer to thread handle.
 *---------------------------------------------------------------------------*/
sys_thread_t sys_kernel_thread_new(const char *name, lwip_thread_fn entry_function, void *arg, int stacksize, int priority)
{

	if (s_nextthread < SYS_THREAD_MAX) {
		sys_thread_t new_thread;
		new_thread = kernel_thread(name, priority, stacksize, (main_t) entry_function, (char *const *)NULL);
		if (new_thread < 0) {
			int errval = errno;
			LWIP_DEBUGF(SYS_DEBUG, ("Failed to create new_thread: %d", errval));
			return -errval;
		} else {
			LWIP_DEBUGF(SYS_DEBUG, ("Created New Thread with pid %d", new_thread));
			s_nextthread++;
			return new_thread;
		}
	}
	return -1;
}

sys_prot_t sys_arch_protect(void)
{
	sched_lock();
	return (sys_prot_t) 1;
}

void sys_arch_unprotect(sys_prot_t p)
{
	sched_unlock();
	return;
}
