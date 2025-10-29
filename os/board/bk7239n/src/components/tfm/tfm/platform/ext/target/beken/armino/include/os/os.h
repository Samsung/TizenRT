// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <common/bk_include.h>
#include <common/bk_typedef.h>
#include <common/bk_kernel_err.h>

#pragma once

/**
* @brief os enums defines
* @defgroup os_defines os defines
* @ingroup os
* @{
*/

#define RTOS_TAG "os"  /**< OS log tag*/

#define RTOS_LOGI(...) BK_LOGI(RTOS_TAG, ##__VA_ARGS__)   /**< Output OS Info log */
#define RTOS_LOGW(...) BK_LOGW(RTOS_TAG, ##__VA_ARGS__)   /**< Output OS Warning log */
#define RTOS_LOGE(...) BK_LOGE(RTOS_TAG, ##__VA_ARGS__)   /**< Output OS Error log */
#define RTOS_LOGD(...) BK_LOGD(RTOS_TAG, ##__VA_ARGS__)   /**< Output OS Debug log */


#define RTOS_SUCCESS                       (1)  /**< Return Success */
#define RTOS_FAILURE                       (0)  /**< Return Failure */

#define BEKEN_DEFAULT_WORKER_PRIORITY      (6)  /**< Default Worker Priority */
#define BEKEN_APPLICATION_PRIORITY         (7)  /**< Application Task Priority */

#define NanosecondsPerSecond              1000000000UUL   /**< Nanoseconds Per Second */
#define MicrosecondsPerSecond             1000000UL       /**< Microseconds Per Second */
#define MillisecondsPerSecond             1000            /**< Milliseconds Per Second */


#define BEKEN_NEVER_TIMEOUT                (0xFFFFFFFF)    /**< Never Timeout */
#define BEKEN_WAIT_FOREVER                 (0xFFFFFFFF)    /**< Wait Forever */
#define BEKEN_NO_WAIT                      (0)             /**< No Wait */

#define NANOSECONDS                        1000000UL       /**< Nanoseconds Per Milliseconds */
#define MICROSECONDS                       1000            /**< Microseconds Per Milliseconds */
#define MILLISECONDS                       (1)             /**< One Milliseconds */
#define SECONDS                            (1000)          /**< Milliseconds Per Second */
#define MINUTES                            (60 * SECONDS)  /**< Milliseconds Per MINUTES */
#define HOURS                              (60 * MINUTES)  /**< Milliseconds Per HOURS */
#define DAYS                               (24 * HOURS)    /**< Milliseconds Per DAYS */

#define BEKEN_MAGIC_WORD       (0xBABA7231)                /**< Beken Magic word */


typedef int             bk_err_t;            /**< Return error code */

typedef void           *beken_thread_arg_t;  /**< Thread argument pointer type */
typedef uint8_t         beken_bool_t;        /**< Bool type */
typedef uint32_t        beken_time_t;        /**< Time value in milliseconds */
typedef uint32_t        beken_utc_time_t;    /**< UTC Time in seconds        */
typedef uint64_t        beken_utc_time_ms_t; /**< UTC Time in milliseconds   */
typedef uint32_t        beken_event_flags_t; /**< Event flag type */
typedef void           *beken_semaphore_t;   /**< OS Semaphore handle pointer */
typedef void           *beken_mutex_t;       /**< OS Mutex handle pointer */
typedef void           *beken_thread_t;      /**< OS Thread handle pointer */
typedef void           *beken_queue_t;       /**< OS Queue handle pointer */
typedef void           *beken_event_t;       /**< OS Event handle pointer */

/// timer callback function type with one parameter
typedef void (*timer_handler_t)(void *);
/// timer callback function type with two parameters
typedef void (*timer_2handler_t)(void *Larg, void *Rarg);
/// event callback function type with one parameter
typedef bk_err_t(*event_handler_t)(void *arg);

/// OS Thread entry function type
typedef void (*beken_thread_function_t)(beken_thread_arg_t arg);

/// OS Event wait options
typedef enum {
	WAIT_FOR_ANY_EVENT,  /**< Wait for any event */
	WAIT_FOR_ALL_EVENTS, /**< Wait for all event */
} beken_event_flags_wait_option_t;

/**
* @}
*/


#define os_printf bk_printf    /**< OS printf, will be replace with RTOS_LOGI later */
#define os_null_printf bk_null_printf  /**< OS drop print string */
#define rtos_init_semaphore_adv rtos_init_semaphore_ex   /**< To be replace with rtos_init_semaphore_ex */
#define rtos_get_sema_count  rtos_get_semaphore_count    /**< To be replace with rtos_get_semaphore_count */

/**
* @brief os struct defines
* @defgroup os_structs structs in os
* @ingroup os
* @{
*/

/// OS timer handle struct type
typedef struct {
	void           *handle;    /**< OS timer handle pointer */
	timer_handler_t function;  /**< OS timer handle callback function */
	void           *arg;       /**< OS timer handle callback argument */
} beken_timer_t;

/// OS worker thread handle struct type
typedef struct {
	beken_thread_t thread;       /**< OS thread handle */
	beken_queue_t  event_queue;  /**< OS event queue */
} beken_worker_thread_t;

/// OS timer event struct type
typedef struct {
	event_handler_t        function; /**< OS event callback function */
	void                  *arg;      /**< OS event callback argument */
	beken_timer_t           timer;   /**< OS timer handle */
	beken_worker_thread_t  *thread;  /**< OS work thread handle */
} beken_timed_event_t;

/// OS timer handle struct type
typedef struct {
	void           *handle;      /**< OS timer handle pointer */
	timer_2handler_t function;   /**< OS timer handle callback function */
	void           *left_arg;    /**< OS timer handle callback first argument */
	void           *right_arg;   /**< OS timer handle callback second argument */
	uint32_t        beken_magic; /**< OS timer magic word */
} beken2_timer_t;
/**
* @}
*/



/**
 * @brief os API
 * @defgroup os_apis os API group
 * @ingroup os
 * @{
 */


/** @brief Enter a critical session, all interrupts are disabled
  *
  * @return    none
  */
void rtos_enter_critical(void);

/** @brief Exit a critical session, all interrupts are enabled
  *
  * @return    none
  */
void rtos_exit_critical(void);


/** @brief   Get system time value in milliseconds
  *
  * @param   time_ptr     : the pointer of time value in milliseconds
  *
  * @return  kNoErr        : on success.
  * @return  kGeneralErr   : if an error occurred
  */
bk_err_t beken_time_get_time(beken_time_t *time_ptr);



/** @defgroup BEKEN_RTOS_Thread _BK_ RTOS Thread Management Functions
 *  @brief Provide thread creation, delete, suspend, resume, and other RTOS management API
 *  @verbatim
 *   _BK_ thread priority table
 *
 * +----------+-----------------+
 * | Priority |      Thread     |
 * |----------|-----------------|
 * |     0    |      _BK_       |   Highest priority
 * |     1    |     Network     |
 * |     2    |                 |
 * |     3    | Network worker  |
 * |     4    |                 |
 * |     5    | Default Library |
 * |          | Default worker  |
 * |     6    |                 |
 * |     7    |   Application   |
 * |     8    |                 |
 * |     9    |      Idle       |   Lowest priority
 * +----------+-----------------+
 *  @endverbatim
 * @{
 */


/** @brief Creates and starts a new thread
  *
  * @param thread     : Pointer to variable that will receive the thread handle (can be null)
  * @param priority   : A priority number.
  * @param name       : a text name for the thread (can be null)
  * @param function   : the main thread function
  * @param stack_size : stack size for this thread
  * @param arg        : argument which will be passed to thread function
  *
  * @return    kNoErr          : on success.
  * @return    kGeneralErr     : if an error occurred
  */
bk_err_t rtos_create_thread(beken_thread_t *thread, uint8_t priority, const char *name, beken_thread_function_t function, uint32_t stack_size, beken_thread_arg_t arg);

/** @brief   Deletes a terminated thread
  *
  * @param   thread     : the handle of the thread to delete, , NULL is the current thread
  *
  * @return  kNoErr        : on success.
  * @return  kGeneralErr   : if an error occurred
  */
bk_err_t rtos_delete_thread(beken_thread_t *thread);

/** @brief   Creates a worker thread
 *
 * Creates a worker thread
 * A worker thread is a thread in whose context timed and asynchronous events
 * execute.
 *
 * @param worker_thread    : a pointer to the worker thread to be created
 * @param priority         : thread priority
 * @param stack_size       : thread's stack size in number of bytes
 * @param event_queue_size : number of events can be pushed into the queue
 *
 * @return    kNoErr        : on success.
 * @return    kGeneralErr   : if an error occurred
 */
bk_err_t rtos_create_worker_thread(beken_worker_thread_t *worker_thread, uint8_t priority, uint32_t stack_size, uint32_t event_queue_size);


/** @brief   Deletes a worker thread
 *
 * @param worker_thread : a pointer to the worker thread to be created
 *
 * @return    kNoErr : on success.
 * @return    kGeneralErr   : if an error occurred
 */
bk_err_t rtos_delete_worker_thread(beken_worker_thread_t *worker_thread);


/** @brief    Suspend a thread
  *
  * @param    thread     : the handle of the thread to suspend, NULL is the current thread
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error occurred
  */
void rtos_suspend_thread(beken_thread_t *thread);



/** @brief    Suspend all other thread
  *
  * @param    none
  *
  * @return   none
  */
void rtos_suspend_all_thread(void);


/** @brief    resume a thread
  *
  * @param    thread     : the handle of the thread to resume, NULL is the current thread
  *
  * @return   none
  */
void rtos_resume_thread(beken_thread_t* thread);


/** @brief    Rresume all other thread
  *
  * @param    none
  *
  * @return   none
  */
long rtos_resume_all_thread(void);


/** @brief    Sleeps until another thread has terminated
  *
  * @Details  Causes the current thread to sleep until the specified other thread
  *           has terminated. If the processor is heavily loaded with higher priority
  *           tasks, this thread may not wake until significantly after the thread termination.
  *
  * @param    thread : the handle of the other thread which will terminate
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error occurred
  */
bk_err_t rtos_thread_join(beken_thread_t *thread);


/** @brief    Forcibly wakes another thread
  *
  * @Details  Causes the specified thread to wake from suspension. This will usually
  *           cause an error or timeout in that thread, since the task it was waiting on
  *           is not complete.
  *
  * @param    thread : the handle of the other thread which will be woken
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error occurred
  */
bk_err_t rtos_thread_force_awake(beken_thread_t *thread);


/** @brief    Checks if a thread is the current thread
  *
  * @Details  Checks if a specified thread is the currently running thread
  *
  * @param    thread : the handle of the other thread against which the current thread
  *                    will be compared
  *
  * @return   true   : specified thread is the current thread
  * @return   false  : specified thread is not currently running
  */
bool rtos_is_current_thread(beken_thread_t *thread);

/** @brief    Get current thread handler
  *
  * @return   Current RTOS thread handler
  */
beken_thread_t *rtos_get_current_thread(void);

/** @brief    Suspend current thread for a specific time
  *
  * @param    seconds : A time interval (Unit: seconds)
  *
  * @return   None.
  */
void rtos_thread_sleep(uint32_t seconds);

/** @brief    Suspend current thread for a specific time
 *
 * @param     milliseconds : A time interval (Unit: millisecond)
 *
 * @return    None.
 */
void rtos_thread_msleep(uint32_t milliseconds);

/** @brief    Suspend current thread for a specific time
 *
 * @param     num_ms : A time interval (Unit: millisecond)
 *
 * @return    kNoErr.
 */
bk_err_t rtos_delay_milliseconds(uint32_t num_ms);


/** @brief    Print Thread status into buffer
  *
  * @param    buffer, point to buffer to store thread status
  * @param    length, length of the buffer
  *
  * @return   none
  */
bk_err_t rtos_print_thread_status(char *buffer, int length);

/**
  * @}
  */

/** @defgroup BEKEN_RTOS_SEM _BK_ RTOS Semaphore Functions
  * @brief Provide management APIs for semaphore such as init,set,get and dinit.
  * @{
  */

/** @brief    Initialises a counting semaphore and set count to 0
  *
  * @param    semaphore : a pointer to the semaphore handle to be initialised
  * @param    max_count  : the max count number of this semaphore
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error occurred
  */
bk_err_t rtos_init_semaphore(beken_semaphore_t *semaphore, int max_count);

/** @brief    Initialises a counting semaphore and set count to init count
  *
  * @param    semaphore : a pointer to the semaphore handle to be initialised
  * @param    max_count  : the max count number of this semaphore
  * @param    init_count  : the init count number of this semaphore
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error occurred
  */
bk_err_t rtos_init_semaphore_ex(beken_semaphore_t *semaphore, int max_count, int init_count);


/** @brief    Set (post/put/increment) a semaphore
  *
  * @param    semaphore : a pointer to the semaphore handle to be set
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error occurred
  */
bk_err_t rtos_set_semaphore(beken_semaphore_t *semaphore);


/** @brief    Get (wait/decrement) a semaphore
  *
  * @Details  Attempts to get (wait/decrement) a semaphore. If semaphore is at zero already,
  *           then the calling thread will be suspended until another thread sets the
  *           semaphore with @ref rtos_set_semaphore
  *
  * @param    semaphore : a pointer to the semaphore handle
  * @param    timeout_ms: the number of milliseconds to wait before returning
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error occurred
  */
bk_err_t rtos_get_semaphore(beken_semaphore_t *semaphore, uint32_t timeout_ms);


int rtos_get_semaphore_count(beken_semaphore_t *semaphore);


/** @brief    De-initialise a semaphore
  *
  * @Details  Deletes a semaphore created with @ref rtos_init_semaphore
  *
  * @param    semaphore : a pointer to the semaphore handle
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error occurred
  */
bk_err_t rtos_deinit_semaphore(beken_semaphore_t *semaphore);
/**
  * @}
  */

/** @defgroup BEKEN_RTOS_MUTEX _BK_ RTOS Mutex Functions
  * @brief Provide management APIs for Mutex such as init,lock,unlock and dinit.
  * @{
  */

/** @brief    Initialises a mutex
  *
  * @Details  A mutex is different to a semaphore in that a thread that already holds
  *           the lock on the mutex can request the lock again (nested) without causing
  *           it to be suspended.
  *
  * @param    mutex : a pointer to the mutex handle to be initialised
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error occurred
  */
bk_err_t rtos_init_mutex(beken_mutex_t *mutex);

/** @brief    Obtains the lock on a mutex
  *
  * @Details  Attempts to obtain the lock on a mutex. If the lock is already held
  *           by another thead, the calling thread will not be suspended until the mutex
  *           lock is released by the other thread.
  *
  * @param    mutex : a pointer to the mutex handle to be locked
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error occurred
  */
bk_err_t rtos_trylock_mutex(beken_mutex_t *mutex);

/** @brief    Obtains the lock on a mutex
  *
  * @Details  Attempts to obtain the lock on a mutex. If the lock is already held
  *           by another thead, the calling thread will be suspended until the mutex
  *           lock is released by the other thread.
  *
  * @param    mutex : a pointer to the mutex handle to be locked
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error occurred
  */
bk_err_t rtos_lock_mutex(beken_mutex_t *mutex);


/** @brief    Releases the lock on a mutex
  *
  * @Details  Releases a currently held lock on a mutex. If another thread
  *           is waiting on the mutex lock, then it will be resumed.
  *
  * @param    mutex : a pointer to the mutex handle to be unlocked
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error occurred
  */
bk_err_t rtos_unlock_mutex(beken_mutex_t *mutex);


/** @brief    De-initialise a mutex
  *
  * @Details  Deletes a mutex created with @ref rtos_init_mutex
  *
  * @param    mutex : a pointer to the mutex handle
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error occurred
  */
bk_err_t rtos_deinit_mutex(beken_mutex_t *mutex);
/**
  * @}
  */

/** @defgroup BEKEN_RTOS_QUEUE _BK_ RTOS FIFO Queue Functions
  * @brief Provide management APIs for FIFO such as init,push,pop and dinit.
  * @{
  */

/** @brief    Initialises a FIFO queue
  *
  * @param    queue : a pointer to the queue handle to be initialised
  * @param    name  : a text string name for the queue (NULL is allowed)
  * @param    message_size : size in bytes of objects that will be held in the queue
  * @param    number_of_messages : depth of the queue - i.e. max number of objects in the queue
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error occurred
  */
bk_err_t rtos_init_queue(beken_queue_t *queue, const char *name, uint32_t message_size, uint32_t number_of_messages);


/** @brief    Pushes an object onto a queue
  *
  * @param    queue : a pointer to the queue handle
  * @param    message : the object to be added to the queue. Size is assumed to be
  *                  the size specified in @ref rtos_init_queue
  * @param    timeout_ms: the number of milliseconds to wait before returning
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error or timeout occurred
  */
bk_err_t rtos_push_to_queue(beken_queue_t *queue, void *message, uint32_t timeout_ms);

/** @brief    Pushes an object to front of the queue
  *
  * @param    queue : a pointer to the queue handle
  * @param    message : the object to be added to the queue. Size is assumed to be
  *                  the size specified in @ref rtos_init_queue
  * @param    timeout_ms: the number of milliseconds to wait before returning
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error or timeout occurred
  */
bk_err_t rtos_push_to_queue_front(beken_queue_t *queue, void *message, uint32_t timeout_ms);


/** @brief    Pops an object off a queue
  *
  * @param    queue : a pointer to the queue handle
  * @param    message : pointer to a buffer that will receive the object being
  *                     popped off the queue. Size is assumed to be
  *                     the size specified in @ref rtos_init_queue , hence
  *                     you must ensure the buffer is long enough or memory
  *                     corruption will result
  * @param    timeout_ms: the number of milliseconds to wait before returning
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error or timeout occurred
  */
bk_err_t rtos_pop_from_queue(beken_queue_t *queue, void *message, uint32_t timeout_ms);


/** @brief    De-initialise a queue created with @ref rtos_init_queue
  *
  * @param    queue : a pointer to the queue handle
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error occurred
  */
bk_err_t rtos_deinit_queue(beken_queue_t *queue);


/** @brief    Check if a queue is empty
  *
  * @param    queue : a pointer to the queue handle
  *
  * @return   true  : queue is empty.
  * @return   false : queue is not empty.
  */
bool rtos_is_queue_empty(beken_queue_t *queue);


/** @brief    Check if a queue is full
  *
  * @param    queue : a pointer to the queue handle
  *
  * @return   true  : queue is empty.
  * @return   false : queue is not empty.
  */
bool rtos_is_queue_full(beken_queue_t *queue);




/** @defgroup BEKEN_RTOS_EVENT _BK_ RTOS Event Functions
  * @{
  */

/**
  * @brief    Sends an asynchronous event to the associated worker thread
  *
  * @param worker_thread :the worker thread in which context the callback should execute from
  * @param function      : the callback function to be called from the worker thread
  * @param arg           : the argument to be passed to the callback function
  *
  * @return    kNoErr        : on success.
  * @return    kGeneralErr   : if an error occurred
  */
bk_err_t rtos_send_asynchronous_event(beken_worker_thread_t *worker_thread, event_handler_t function, void *arg);

/** Requests a function be called at a regular interval
 *
 * This function registers a function that will be called at a regular
 * interval. Since this is based on the RTOS time-slice scheduling, the
 * accuracy is not high, and is affected by processor load.
 *
 * @param event_object  : pointer to a event handle which will be initialised
 * @param worker_thread : pointer to the worker thread in whose context the
 *                        callback function runs on
 * @param function      : the callback function that is to be called regularly
 * @param time_ms       : the time period between function calls in milliseconds
 * @param arg           : an argument that will be supplied to the function when
 *                        it is called
 *
 * @return    kNoErr        : on success.
 * @return    kGeneralErr   : if an error occurred
 */
bk_err_t rtos_register_timed_event(beken_timed_event_t *event_object, beken_worker_thread_t *worker_thread, event_handler_t function, uint32_t time_ms, void *arg);


/** Removes a request for a regular function execution
 *
 * This function de-registers a function that has previously been set-up
 * with @ref rtos_register_timed_event.
 *
 * @param event_object : the event handle used with @ref rtos_register_timed_event
 *
 * @return    kNoErr        : on success.
 * @return    kGeneralErr   : if an error occurred
 */
bk_err_t rtos_deregister_timed_event(beken_timed_event_t *event_object);



/** @defgroup BEKEN_RTOS_TIMER _BK_ RTOS Timer Functions
  * @brief Provide management APIs for timer such as init,start,stop,reload and dinit.
  * @{
  */

/**
  * @brief    Gets time in miiliseconds since RTOS start
  *
  * @note:    Since this is only 32 bits, it will roll over every 49 days, 17 hours.
  *
  * @returns  Time in milliseconds since RTOS started.
  */
uint32_t rtos_get_time(void);

/** @defgroup BEKEN_RTOS_TIMER _BK_ RTOS Timer Functions
  * @brief Provide management APIs for timer such as init,start,stop,reload and dinit.
  * @{
  */

/**
  * @brief    Gets time in miiliseconds since RTOS start
  *
  * @note:    Since this is only 32 bits, it will roll over every 49 days, 17 hours.
  *
  * @returns  Time in milliseconds since RTOS started.
  */
uint32_t beken_ms_per_tick(void);

/**
  * @brief     Initialize a RTOS timer
  *
  * @note      Timer does not start running until @ref beken_start_timer is called
  *
  * @param     timer    : a pointer to the timer handle to be initialised
  * @param     time_ms  : Timer period in milliseconds
  * @param     function : the callback handler function that is called each time the
  *                       timer expires
  * @param     arg      : an argument that will be passed to the callback function
  *
  * @return    kNoErr        : on success.
  * @return    kGeneralErr   : if an error occurred
  */
bk_err_t rtos_init_timer(beken_timer_t *timer, uint32_t time_ms, timer_handler_t function, void *arg);
bk_err_t rtos_init_oneshot_timer(beken2_timer_t *timer,
								 uint32_t time_ms,
								 timer_2handler_t function,
								 void *larg,
								 void *rarg);
bk_err_t rtos_deinit_oneshot_timer(beken2_timer_t *timer);
bk_err_t rtos_stop_oneshot_timer(beken2_timer_t *timer);
bool rtos_is_oneshot_timer_running(beken2_timer_t *timer);
bk_err_t rtos_start_oneshot_timer(beken2_timer_t *timer);
bool rtos_is_oneshot_timer_init(beken2_timer_t *timer);
bk_err_t rtos_oneshot_reload_timer(beken2_timer_t *timer);
bk_err_t rtos_change_period(beken_timer_t *timer, uint32_t time_ms);
bk_err_t rtos_oneshot_reload_timer_ex(beken2_timer_t *timer,
									  uint32_t time_ms,
									  timer_2handler_t function,
									  void *larg,
									  void *rarg);

/** @brief    Starts a RTOS timer running
  *
  * @note     Timer must have been previously initialised with @ref rtos_init_timer
  *
  * @param    timer    : a pointer to the timer handle to start
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error occurred
  */
bk_err_t rtos_start_timer(beken_timer_t *timer);


/** @brief    Stops a running RTOS timer
  *
  * @note     Timer must have been previously started with @ref rtos_init_timer
  *
  * @param    timer    : a pointer to the timer handle to stop
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error occurred
  */
bk_err_t rtos_stop_timer(beken_timer_t *timer);


/** @brief    Reloads a RTOS timer that has expired
  *
  * @note     This is usually called in the timer callback handler, to
  *           reschedule the timer for the next period.
  *
  * @param    timer    : a pointer to the timer handle to reload
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error occurred
  */
bk_err_t rtos_reload_timer(beken_timer_t *timer);


/** @brief    De-initialise a RTOS timer
  *
  * @note     Deletes a RTOS timer created with @ref rtos_init_timer
  *
  * @param    timer : a pointer to the RTOS timer handle
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error occurred
  */
bk_err_t rtos_deinit_timer(beken_timer_t *timer);

/** @brief    Check if an RTOS timer is init
  *
  * @param    timer : a pointer to the RTOS timer handle
  *
  * @return   true        : if init.
  * @return   false       : if not init
  */
bool rtos_is_timer_init(beken_timer_t *timer);

/** @brief    Check if an RTOS timer is running
  *
  * @param    timer : a pointer to the RTOS timer handle
  *
  * @return   true        : if running.
  * @return   false       : if not running
  */
bool rtos_is_timer_running(beken_timer_t *timer);

uint32_t rtos_get_timer_expiry_time(beken_timer_t *timer);
uint32_t rtos_get_next_expire_time();
uint32_t rtos_get_current_timer_count(void);


void rtos_start_scheduler(void);
bool rtos_is_scheduler_started(void);

uint32_t rtos_get_cpsr(void);

char* rtos_get_name(void);
char* rtos_get_version(void);

size_t rtos_get_total_heap_size(void);
size_t rtos_get_free_heap_size(void);
size_t rtos_get_minimum_free_heap_size(void);
size_t rtos_get_psram_total_heap_size(void);
size_t rtos_get_psram_free_heap_size(void);
size_t rtos_get_psram_minimum_free_heap_size(void);

uint32_t rtos_get_tick_count(void);

uint32_t rtos_before_sleep(void);
void rtos_after_sleep(uint32_t int_level);
bool rtos_is_in_interrupt_context(void);
void rtos_wait_for_interrupt(void);

void rtos_shutdown(void);


