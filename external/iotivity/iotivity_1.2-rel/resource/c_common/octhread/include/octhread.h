/* ****************************************************************
 *
 * Copyright 2014 Samsung Electronics All Rights Reserved.
 *
 *
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

/**
 * @file
 *
 * This file provides APIs related to mutex and semaphores.
 */

#ifndef OC_THREAD_H_
#define OC_THREAD_H_

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

typedef struct oc_mutex_internal *oc_mutex;
typedef struct oc_cond_internal *oc_cond;
typedef struct oc_thread_internal *oc_thread;

/**
 * Enums for oc_cond_wait_for return values.
 */
typedef enum
{
   OC_WAIT_SUCCESS = 0,    /**< Condition Signal. */
   OC_WAIT_INVAL = -1,     /**< Invalid Condition. */
   OC_WAIT_TIMEDOUT = -2   /**< Condition Timed Out. */
} OCWaitResult_t;

typedef enum
{
    OC_THREAD_SUCCESS = 0,
    OC_THREAD_ALLOCATION_FAILURE = 1,
    OC_THREAD_CREATE_FAILURE = 2,
    OC_THREAD_INVALID = 3,
    OC_THREAD_WAIT_FAILURE = 4,
    OC_THREAD_INVALID_PARAMETER = 5,
    OC_THREAD_CANCEL_FAILURE = 6
} OCThreadResult_t;

/**
 * Allocates, and starts a new thread
 *
 * @param[out] t  The thread that will refer to a newly allocated, and started thread
 * @param[in] start_routine The function that will execute in a new thread
 * @param[in] arg The information passed to the start_routine
 * @return OCThreadResult_t An enumeration of possible outcomes
 * @retval OC_THREAD_SUCCESS If a thread was successfully allocated and started.
 * @retval OC_THREAD_ALLOCATION_FAILURE If a thread was unable to be allocated
 * @retval OC_THREAD_CREATE_FAILURE If a thread was unable to be started
 *
 */
#ifndef __TIZENRT__
OCThreadResult_t oc_thread_new(oc_thread *t, void *(*start_routine)(void *), void *arg);
#else
OCThreadResult_t oc_thread_new(oc_thread *t, void *(*start_routine)(void *), void *arg, const char *task_name, int stack_size);
#endif

/**
 * Frees a thread previously allocated with oc_thread_new()
 *
 * @param[in] t The thread to be unallocated
 * @return OCThreadResult_t An enumeration of possible outcomes
 * @retval OC_THREAD_SUCCESS If a thread was successfully unallocated
 * @retval OC_THREAD_INVALID_PARAMETER If param t is NULL
 *
 */
OCThreadResult_t oc_thread_free(oc_thread t);

/**
 * Block until a thread's execution has been completed
 *
 * @param[in] t The thread to be waited on
 * @return OCThreadResult_t An enumeration of possible outcomes
 * @retval OC_THREAD_SUCCESS If the thread successfully completed execution
 * @retval OC_THREAD_WAIT_FAILURE If a problem occured while waiting for execution of the thread to complete
 *
 */
OCThreadResult_t oc_thread_wait(oc_thread t);

#ifdef __TIZEN__
/**
 * Cancel the thread without block
 *
 * @param[in] t The thread to be canceled on
 * @return OCThreadResult_t An enumeration of possible outcomes
 * @retval OC_THREAD_SUCCESS If the thread successfully completed execution
 * @retval OC_THREAD_CANCEL_FAILURE If a problem occured while canceling
 *
 */
OCThreadResult_t oc_thread_cancel(oc_thread t);
#endif

/**
 * Creates new mutex.
 *
 * @return  Reference to newly created mutex, otherwise NULL.
 *
 */
oc_mutex oc_mutex_new(void);

/**
 * Lock the mutex.
 *
 * @param  mutex  The mutex to be locked.
 *
 */
void oc_mutex_lock(oc_mutex mutex);

/**
 * Unlock the mutex.
 *
 * @param  mutex  The mutex to be unlocked.
 *
 */
void oc_mutex_unlock(oc_mutex mutex);

/**
 * Free the mutex.
 *
 * @param  mutex  The mutex to be freed.
 * @return bool to indicate success or failure
 * @retval true if mutex was freed successfully
 * @retval false if mutex parameter is invalid
 *
 */
bool oc_mutex_free(oc_mutex mutex);

/**
 * Creates new condition.
 *
 * @return  Reference to newly created oc_cond, otherwise NULL.
 *
 */
oc_cond oc_cond_new(void);

/**
 * One of threads is woken up if multiple threads are waiting for cond.
 *
 * @param  cond  The condtion to be signaled.
 *
 */
void oc_cond_signal(oc_cond cond);

/**
 * All of threads are woken up if multiple threads are waiting for cond.
 *
 * @param  cond  The condtion to be signaled.
 *
 */
void oc_cond_broadcast(oc_cond cond);

/**
 * Waits until this thread woken up on cond.
 *
 * @param  cond  The condtion to be wait for to signal.
 * @param  mutex  The mutex which is currently locked from calling thread.
 *
 */
void oc_cond_wait(oc_cond cond, oc_mutex mutex);

/**
 * Waits until this thread woken up on cond,
 * but not longer than the interval specified by microseconds.
 * The mutex is unlocked before falling asleep and locked again before resuming.
 * If microseconds is 0, oc_cond_wait_for() acts like oc_cond_wait().
 *
 * @param  cond  The condtion to be wait for to signal.
 * @param  mutex  The mutex which is currently locked from calling thread.
 * @param  microseconds  relative time for waiting, microseconds.
 *
 * @return OC_WAIT_SUCCESS if the condition was signaled,
 *         OC_WAIT_TIMEDOUT if wait period exceeded,
 *         OC_WAIT_INVAL for invalid parameters.
 *
 */
OCWaitResult_t oc_cond_wait_for(oc_cond cond, oc_mutex mutex, uint64_t microseconds);

/**
 * Free the condition.
 *
 * @param  cond  The condition to be freed.
 *
 */
void oc_cond_free(oc_cond cond);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* OC_THREAD_H_ */
