/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/

#ifndef __OSDEP_SERVICE_H_
#define __OSDEP_SERVICE_H_

/** @addtogroup RTOS
 *  @{
 */

/*************************** OS dep feature enable *******************************/

/******************************************************
 *                    Macros
 ******************************************************/

#if defined(CONFIG_PLATFORM_8721D) || (defined CONFIG_PLATFORM_AMEBAD2) || (defined CONFIG_PLATFORM_AMEBALITE) || (defined CONFIG_PLATFORM_AMEBADPLUS) || (defined CONFIG_PLATFORM_8735B)
#define CONFIG_PLATFORM_AMEBA_X 1
#else
#define CONFIG_PLATFORM_AMEBA_X 0
#endif

#if defined(CONFIG_PLATFORM_AMEBA_X) && (CONFIG_PLATFORM_AMEBA_X == 1)
#define CONFIG_MEM_MONITOR	MEM_MONITOR_SIMPLE
#else
#define CONFIG_MEM_MONITOR	MEM_MONITOR_SIMPLE
#endif

#if defined(CONFIG_PLATFORM_8735B)
#include <platform_conf.h>
#include <basic_types.h>
#if (CONFIG_CMSIS_FREERTOS_EN==1)
#define PLATFORM_FREERTOS 1
#endif
#else
#if defined(CONFIG_PLATFORM_AMEBA_X) && (CONFIG_PLATFORM_AMEBA_X == 1)
#include "platform_autoconf.h"
#endif
#endif

#if defined(PLATFORM_ALIOS)
#include "alios_service.h"
#elif defined(PLATFORM_FREERTOS)
//#include "freertos_service.h"
#elif defined(PLATFORM_ECOS)
#include "ecos/ecos_service.h"
#elif defined(PLATFORM_CMSIS_RTOS)
#include "cmsis_rtos_service.h"
#elif defined(CONFIG_PLATFOMR_CUSTOMER_RTOS)
#include "customer_rtos_service.h"
#endif
#include "tizenrt_service.h"


#ifdef __cplusplus
extern "C" {
#endif

/* Define compilor specific symbol */

/*************************** inline functions *******************************/
#if defined ( __ICCARM__ )
#define __inline__                      inline
#define __inline                        inline
#define __inline_definition			//In dialect C99, inline means that a function's definition is provided
//only for inlining, and that there is another definition
//(without inline) somewhere else in the program.
//That means that this program is incomplete, because if
//add isn't inlined (for example, when compiling without optimization),
//then main will have an unresolved reference to that other definition.

// Do not inline function is the function body is defined .c file and this
// function will be called somewhere else, otherwise there is compile error
#elif defined ( __CC_ARM   )
#define __inline__			__inline	//__linine__ is not supported in keil compilor, use __inline instead
#define inline				__inline
#define __inline_definition			// for dialect C99
#elif defined   (  __GNUC__  )
#define __inline__                      inline
#define __inline                        inline
#define __inline_definition	inline
#endif

#include <stdio.h>

//#if defined(CONFIG_PLATFORM_AMEBA_X) && (CONFIG_PLATFORM_AMEBA_X == 0)

#ifndef SUCCESS
#define SUCCESS	0
#endif
#ifndef FAIL
#define FAIL	(-1)
#endif

#ifndef _SUCCESS
#define _SUCCESS	1
#endif
#ifndef _FAIL
#define _FAIL	0
#endif
#ifndef FALSE
#define FALSE   0
#endif

#ifndef false
#define false 0
#endif

#ifndef TRUE
#define TRUE    (!FALSE)
#endif

#ifndef true
#define true (!false)
#endif


#ifndef DBG_8195A
#define DBG_8195A
#endif

#define _TRUE        TRUE
#define _FALSE	     FALSE

//#endif

#define RTW_RX_HANDLED			2
#define RTW_RFRAME_UNAVAIL		3
#define RTW_RFRAME_PKT_UNAVAIL	4
#define RTW_RBUF_UNAVAIL		5
#define RTW_RBUF_PKT_UNAVAIL	6
#define RTW_SDIO_READ_PORT_FAIL	7


#define RTW_MAX_DELAY			0xFFFFFFFF
#define RTW_WAIT_FOREVER		0xFFFFFFFF

#if( configSUPPORT_STATIC_ALLOCATION == 1 )
#ifdef CONFIG_WLAN
#define MAX_MUTEX_BUF_NUM		109
#define MAX_TIMER_BUF_NUM		52
#else
#define MAX_MUTEX_BUF_NUM		5
#define MAX_TIMER_BUF_NUM		5
#endif
#endif


/******************************************************
 *                    Constants
 ******************************************************/
/**
  * @brief  Definitions returned by xTaskGetSchedulerState().
  */

#define OS_SCHEDULER_NOT_STARTED	0
#define OS_SCHEDULER_RUNNING		1
#define OS_SCHEDULER_SUSPENDED		2

/******************************************************
 *                    Structures
 ******************************************************/
struct timer_list {
	struct list_head list;
	_timerHandle 	timer_hdl;
	unsigned long	data;
	void (*function)(void *);
};

/******************************************************
 *                 Type Definitions
 ******************************************************/
typedef thread_return(*thread_func_t)(thread_context context);
typedef void (*TIMER_FUN)(void *context);
typedef int (*event_handler_t)(char *buf, int buf_len, int flags, void *user_data);

#define CONFIG_THREAD_COMM_SEMA
struct task_struct {
	const char *task_name;
	pid_t task;	/* I: workqueue thread */

#ifdef CONFIG_THREAD_COMM_SIGNAL
	const char *name;	/* I: workqueue thread name */
	uint32_t queue_num;		/* total signal num */
	uint32_t cur_queue_num;	/* cur signal num should < queue_num */
#elif defined(CONFIG_THREAD_COMM_SEMA)
	_sema wakeup_sema;    /* for internal use only */
	_sema terminate_sema; /* for internal use only */
#endif
	uint32_t blocked;          /* for internal use only */
	uint32_t callback_running; /* for internal use only */
};

#ifdef CONFIG_THREAD_COMM_SIGNAL
struct work_struct;
typedef void (*work_func_t)(void *context);
struct work_struct {
	_list list;
	uint32_t data;
	work_func_t func;
	void *context;
	struct task_struct *used_wq;
};

struct delayed_work {
	struct work_struct work;
	struct timer_list timer;
};
#endif

/*************************** Memory Management *******************************/
void	*_rtw_vmalloc(uint32_t sz);
void	*_rtw_zvmalloc(uint32_t sz);
void	_rtw_vmfree(uint8_t *pbuf, uint32_t sz);
void	*_rtw_zmalloc(uint32_t sz);
void	*_rtw_malloc(uint32_t sz);
void  *_rtw_calloc(uint32_t nelements, uint32_t elmentSize);
void	_rtw_mfree(uint8_t *pbuf, uint32_t sz);

/**
 * @brief  This function allocates the virtually contiguous memory.
 * @param[in] sz: The size of memory to be allocated.
 * @return	  The pointer to the beginning of the memory
 */
void	*rtw_vmalloc(uint32_t sz);

/**
 * @brief  This function allocates the virtually contiguous memory
 *		   and the values of the memory are setted to 0.
 * @param[in] sz: The size of memory to be allocated.
 * @return	  The pointer to the beginning of the memory
 */
void	*rtw_zvmalloc(uint32_t sz);

/**
 * @brief  This function frees the virtually contiguous memory.
 * @param[in] pbuf: The pointer to the beginning of the memory to be free
 * @param[in] sz: The size of memory allocated.
 * @return	  None
 */
void	rtw_vmfree(uint8_t *pbuf, uint32_t sz);

/**
 * @brief  This function allocates the memory
 *		   and the values of the memory are setted to 0.
 * @param[in] sz: The size of memory to be allocated.
 * @return	  The pointer to the beginning of the memory
 */
void	*rtw_zmalloc(uint32_t sz);

/**
 * @brief  This function allocates the memory.
 * @param[in] sz: The size of memory to be allocated.
 * @return	  The pointer to the beginning of the memory
 */
void	*rtw_malloc(uint32_t sz);

/**
 * @brief  This function allocates the memory, nlements*elementSize bytes in size.
 * @param[in] nelements: The number of elements.
 * @param[in] elementSize: The size of each element
 * @return	  The pointer to the beginning of the memory
 */
void	*rtw_calloc(uint32_t nelements, uint32_t elmentSize);

/**
 * @brief  This function frees the virtually contiguous memory.
 * @param[in] pbuf: The pointer to the beginning of the memory to be free
 * @param[in] sz: The size of memory allocated.
 * @return	  None
 */
void	rtw_mfree(uint8_t *pbuf, uint32_t sz);

#define rtw_free(buf)		rtw_mfree((uint8_t *)buf, 0)

/**
 * @brief  This function allocates a 2 dimensional array memory.
 * @param[in] h: The height of the 2D array.
 * @param[in] w: The width of the 2D array.
 * @param[in] size: The size of the each charactor in array.
 * @return	  the pointer to the beginning of the block
 */
void	*rtw_malloc2d(int h, int w, int size);

/**
 * @brief  This function deallocates the block of memory previously allocated to make it available again.
 * @param[in] pbuf: Pointer to a memory block previously allocated.
 * @param[in] h: The height of the 2D array.
 * @param[in] w: The width of the 2D array.
 * @param[in] size: The size of the each charactor in array.
 * @return	  None
 */
void	rtw_mfree2d(void *pbuf, int h, int w, int size);

/**
 * @brief  This function copies the values of "sz" bytes from the location pointed to by "src"
 *         directly to the memory block pointed to by "des".
 * @param[in] dst: Pointer to the destination array where the content is to be copied, type-casted to a pointer of type void*.
 * @param[in] src: Pointer to the source of data to be copied, type-casted to a pointer of type void*.
 * @param[in] sz: Size of memory to copy.
 * @return	  None
 */
void	rtw_memcpy(void *dst, void *src, uint32_t sz);

/**
 * @brief  This function compares the first "sz" bytes of the block of memory pointed by "dst"
 *		   to the first "sz" bytes pointed by "src".
 * @param[in] dst: Pointer to block of memory to be compared.
 * @param[in] src: pointer to block of memory to compare.
 * @param[in] sz: Size of memory to compare.
 * @return	<0: The first byte that does not match in both memory blocks has a lower value in dst than in src.
 * @return	0: The contents of both memory blocks are equal.
 * @return	<0: The first byte that does not match in both memory blocks has a greater value in dst than in src.
 */
int	rtw_memcmp(void *dst, void *src, uint32_t sz);

/**
 * @brief  This function sets the first "sz" bytes of the block of memory pointed by "pbuf" to the specified "c".
 * @param[in] pbuf: Pointer to the block of memory to fill.
 * @param[in] c: Value to be set.
 * @param[in] sz: Size of memory to be set to the value "c".
 * @return	  None
 */
void	rtw_memset(void *pbuf, int c, uint32_t sz);
/*************************** End Memory Management *******************************/

/*************************** List *******************************/

/**
 * @brief  This function initializes the head of the list.
 * @param[in] list: Pointer to the list to be initialized.
 * @return	  None
 */
void	rtw_init_listhead(_list *list);

/**
 * @brief  This function tests whether a list is empty.
 * @param[in] phead: Pointer to the list to test.
 * @return	  _TRUE/_FALSE
 */
uint32_t	rtw_is_list_empty(_list *phead);

/**
 * @brief  This function adds a new entry after "phead" for the list.
 * @param[in] plist: Pointer to the list to be added.
 * @param[in] phead: List head to add it after.
 * @return	  None
 */
void	rtw_list_insert_head(_list *plist, _list *phead);

/**
 * @brief  This function adds a new entry before "phead" for the list.
 * @param[in] plist: Pointer to the list to be added.
 * @param[in] phead: List head to add it before.
 * @return	  None
 */
void	rtw_list_insert_tail(_list *plist, _list *phead);

/**
 * @brief  This function deletes entry from list and reinitialize it.
 * @param[in] plist: The element to delete from the list.
 * @return	  None
 * @note	  Caller must check if the list is empty before calling rtw_list_delete
 */
void	rtw_list_delete(_list *plist);
/*************************** End List *******************************/


/*************************** Semaphores *******************************/
/**
 * @brief  This function initializes the unnamed semaphore referred to by "sema" to the value "init_val".
 * @param[in] sema: Pointer to the semaphore handle to be initialized.
 * @param[in] init_val: Initial value for semaphore.
 * @return	  None
 */
void	rtw_init_sema(_sema *sema, int init_val);

/**
 * @brief  This function deletes the semaphore.
 * @param[in] sema: The semaphore to be deleted.
 * @return	  None
 */
void	rtw_free_sema(_sema *sema);

/**
 * @brief  This function releases the semaphore.
 *		   This macro must not be used from an ISR.
 * @param[in] sema: The semaphore to be released.
 * @return	  None
 */
void	rtw_up_sema(_sema	*sema);

/**
 * @brief  This function releases the semaphore.
 *		   This macro can be used from an ISR.
 * @param[in] sema: The semaphore to be released.
 * @return	  None
 */
void	rtw_up_sema_from_isr(_sema *sema);

/**
 * @brief  This function acquires the semaphore. If no more tasks are allowed to acquire the semaphore,
 *		   calling this function will put the task to sleep until the semaphore is up.
 * @param[in] sema: The semaphore to be acquired.
 * @return	pdTRUE: The semaphore was obtained.
 * @return	pdFALSE: Obtain the semaphore failed.
 */
uint32_t	rtw_down_sema(_sema *sema);

/**
 * @brief  This function acquires the semaphore. If no more tasks are allowed to acquire the semaphore,
 *		   calling this function will put the task to sleep until the semaphore is up.
 * @param[in] sema: The semaphore to be acquired.
 * @param[in] timeout: The time in ms to wait for the semaphore to become available.
 * @return	pdTRUE: The semaphore was obtained.
 * @return	pdFALSE: Timeout without the semaphore becoming available.
 */
uint32_t	rtw_down_timeout_sema(_sema *sema, uint32_t timeout);
/*************************** End Semaphores *******************************/

/*************************** Mutexes *******************************/
/**
 * @brief  This function implements a mutex semaphore by using the existing queue mechanism.
 * @param[in] pmutex: Pointer to the created mutex semaphore.
 * @return	  None
 */
void	rtw_mutex_init(_mutex *pmutex);

/**
 * @brief  This function deletes the mutex semaphore.
 * @param[in] pmutex: Pointer to the mutex semaphore to be deleted.
 * @return	  None
 */
void	rtw_mutex_free(_mutex *pmutex);

/**
 * @brief  This function releases a mutex semaphore.
 * @param[in] pmutex: Pointer to the mutex semaphore to be released.
 * @return	  None
 */
void	rtw_mutex_put(_mutex *pmutex);

/**
 * @brief  This function obtains a mutex semaphore.
 * @param[in] pmutex: Pointer to the mutex semaphore being taken - obtained when
 *			  the mutex semaphore was created.
 * @return	  None
 */
void	rtw_mutex_get(_mutex *pmutex);

/**
 * @brief  This function obtains a mutex semaphore with a timeout setting.
 * @param[in] pmutex: Pointer to the mutex semaphore being taken - obtained when
 *			  the mutex semaphore was created.
 * @param[in] timeout: The time in ms to wait for the semaphore to become available.
 * @return	0: The semaphore was obtained.
 * @return	-1: Timeout without the semaphore becoming available.
 */
int	rtw_mutex_get_timeout(_mutex *pmutex, uint32_t timeout_ms);
/*************************** End Mutexes *******************************/

/*************************** SchedulerControl *******************************/
/**
 * @brief  This function marks the start of a critical code region.
 * 		   Preemptive context switches cannot occur when in a critical region.
 * @param[in] plock: Pointer to the spin lock semaphore.
 * @param[in] pirqL: Pointer to the IRQ.
 * @return	  None
 * @note: This may alter the stack (depending on the portable implementation)
 * so must be used with care!
 */
void	rtw_enter_critical(_lock *plock, _irqL *pirqL);

/**
 * @brief  This function marks end of a critical code region. Preemptive context
 * switches cannot occur when in a critical region.
 * @param[in] plock: Pointer to the spin lock semaphore.
 * @param[in] pirqL: Pointer to the IRQ.
 * @return	  None
 * @note: This may alter the stack (depending on the portable implementation)
 * so must be used with care!
 */
void	rtw_exit_critical(_lock *plock, _irqL *pirqL);

/**
 * @brief  This function marks the start of a critical code region from isr.
 * @param[in] plock: Pointer to the spin lock semaphore.
 * @param[in] pirqL: Pointer to the IRQ.
 * @return	  None
 */
void	rtw_enter_critical_from_isr(_lock *plock, _irqL *pirqL);

/**
 * @brief  This function marks the end of a critical code region from isr.
 * @param[in] plock: Pointer to the spin lock semaphore.
 * @param[in] pirqL: Pointer to the IRQ.
 * @return	  None
 */
void	rtw_exit_critical_from_isr(_lock *plock, _irqL *pirqL);

/**
 * @brief  This function obtains a spin lock semaphore.
 * @param[in] plock: Pointer to the spin lock semaphore being taken - obtained when
 *			  the mutex semaphore was created.
 * @param[in] pirqL: Pointer to the IRQ.
 * @return	  None
 */
void	rtw_enter_critical_bh(_lock *plock, _irqL *pirqL);

/**
 * @brief  This function releases a spin lock semaphore.
 * @param[in] plock: Pointer to the spin lock semaphore to be released.
 * @param[in] pirqL: Pointer to the IRQ.
 * @return	  None
 */
void	rtw_exit_critical_bh(_lock *plock, _irqL *pirqL);

/**
 * @brief  This function obtains a semaphore.
 * @param[in] pmutex: The handle to the mutex semaphore to be obtained.
 * @param[in] pirqL: Pointer to the IRQ.
 * @return	  None
 */
int		rtw_enter_critical_mutex(_mutex *pmutex, _irqL *pirqL);

/**
 * @brief  This function releases a semaphore.
 * @param[in] pmutex: The handle to the mutex semaphore to be released.
 * @param[in] pirqL: Pointer to the IRQ.
 * @return	  None
 */
void	rtw_exit_critical_mutex(_mutex *pmutex, _irqL *pirqL);

/**
 * @brief  This function will lock cpu. Can be used when xip active and want to do some flash operation.
 * @return	  None
 */
void	rtw_cpu_lock(void);

/**
* @brief  This function unlock cpu.
* @return	  None
*/
void	rtw_cpu_unlock(void);

/*************************** End SchedulerControl *******************************/

/*************************** Semaphores *******************************/

/**
 * @brief  This function implements a spin lock semaphore by using the existing queue mechanism.
 * @param[in] plock: Pointer to the created spin lock semaphore.
 * @return	  None
 */
void	rtw_spinlock_init(_lock *plock);

/**
 * @brief  This function deletes the spin lock semaphore.
 * @param[in] pmutex: Pointer to the spin lock semaphore to be deleted.
 * @return	  None
 */
void	rtw_spinlock_free(_lock *plock);

/**
 * @brief  This function obtains a spin lock semaphore.
 * @param[in] plock: Pointer to the spin lock semaphore being taken - obtained when
 *			  the mutex semaphore was created.
 * @return	  None
 */
void	rtw_spin_lock(_lock *plock);

/**
 * @brief  This function releases a spin lock semaphore.
 * @param[in] plock: Pointer to the spin lock semaphore to be released.
 * @return	  None
 */
void	rtw_spin_unlock(_lock *plock);

/**
 * @brief  This function marks the start of a critical code region and
 *		   obtains a spin lock semaphore.
 * @param[in] plock: Pointer to the spin lock semaphore being taken - obtained when
 *			  the mutex semaphore was created.
 * @param[in] irqL: Pointer to the IRQ.
 * @return	  None
 */
void	rtw_spinlock_irqsave(_lock *plock, _irqL *irqL);

/**
 * @brief  This function releases a spin lock semaphore and
 		   marks the end of a critical code region.
 * @param[in] plock: Pointer to the spin lock semaphore to be released.
 * @param[in] irqL: Pointer to the IRQ.
 * @return	  None
 */
void	rtw_spinunlock_irqsave(_lock *plock, _irqL *irqL);
/*************************** End Semaphores *******************************/

/*************************** Queues *******************************/

/**
 * @brief  This function creates a new queue instance.
 * @param[in] queue: The handle to the newly created queue.
 * @param[in] name: The name of the queue
 * @param[in] message_size: The number of bytes each message in the queue will require.
 * @param[in] number_of_messages: The maximum number of messages that kthe queue can contain.
 * @return	  0: Creating queue success
 * @return	  -1: Creating queue fail
 */
int rtw_init_xqueue(_xqueue *queue, const char *name, uint32_t message_size, uint32_t number_of_messages);

/**
 * @brief  This function posts a message to the back of a queue.
 *		   The message is queued by copy, not by reference.
 * @param[in] queue: The handle to the queue on which the message is to be posted.
 * @param[in] message: The pointer to the message that is to be placed on the queue.
 * @param[in] timeout_ms: The maximum amout of time the task should block waiting for
 			              the space to become available on the queue, should it already be full.
 			              The time is defined in ms.
 * @return	  0: The message was successfully posted.
 * @return	  -1: The message was not posted.
 */
int rtw_push_to_xqueue(_xqueue *queue, void *message, uint32_t timeout_ms);

/**
 * @brief  This function receives a message from a queue.
 *		   The message is recieved by copy so a buffer adequate size must be provided.
 * @param[in] queue: The handle to the queue from which the message is to be received.
 * @param[in] message: The pointer to the buffer into which the received message will be copied.
 * @param[in] timeout_ms: The maximum amout of time the task should block waiting for a message to
 *						  receive should the queue be empty at the time of the call.
 			              The time is defined in ms.
 * @return	  0: A message was successfully received from the queue.
 * @return	  -1: No message was received from the queue.
 */
int rtw_pop_from_xqueue(_xqueue *queue, void *message, uint32_t timeout_ms);

int rtw_peek_from_xqueue(_xqueue *queue, void *message, uint32_t timeout_ms);

/**
 * @brief  Delete a queue - freeing all the memory allocated for storing of messages placed on the queue.
 * @param[in] queue: The handle to the queue to be deleted.
 * @return	  0: The queue was successfully deleted.
 * @return	  -1: The queue was not empty so cannot be deleted.
 */
int rtw_deinit_xqueue(_xqueue *queue);

/**
 * @brief  This function creates a new queue instance.
 * @param[in] pqueue: The handle to the newly created queue.
 * @return	 None
 */
void	rtw_init_queue(_queue	*pqueue);
void	rtw_deinit_queue(_queue	*pqueue);
uint32_t	rtw_is_queue_empty(_queue *pqueue);

/**
 * @brief  This function tests whether the queue is empty.
 * @param[in] pqueue: The handle to the queue to be tested.
 * @return	 None
 */
uint32_t	rtw_queue_empty(_queue	*pqueue);

/**
 * @brief  This function tests whether the "pelement" is at the "queue".
 * @param[in] queue: The pointer to the queue that to be tested.
 * @param[in] pelement: The element that to be tested.
 * @return	 _TRUE/_FALSE
 */
uint32_t	rtw_end_of_queue_search(_list *queue, _list *pelement);
_list *rtw_get_queue_head(_queue	*queue);
/*************************** End Queues *******************************/

/*************************** Time Management *******************************/

/**
 * @brief  Get the count of ticks since the vTaskStartScheduler was called.
 * @return	The count of ticks since the vTaskStartScheduler was called.
 */
uint32_t	rtw_get_current_time(void);

/**
 * @brief  Convert system time to milliseconds.
 * @param[in] systime: The system time to be converted.
 * @return : The milliseconds that converted by the system time.
 */
uint32_t	rtw_systime_to_ms(uint32_t systime);

/**
 * @brief  Convert system time to seconds.
 * @param[in] systime: The system time to be converted.
 * @return : The seconds that converted by the system time.
 */
uint32_t rtw_systime_to_sec(uint32_t systime);

/**
 * @brief  Convert milliseconds to system time.
 * @param[in] systime: The milliseconds to be converted.
 * @return : The system time that converted by the milliseconds.
 */
uint32_t	rtw_ms_to_systime(uint32_t ms);

/**
 * @brief  Convert seconds to system time.
 * @param[in] systime: The seconds to be converted.
 * @return : The system time that converted by the seconds.
 */
uint32_t	rtw_sec_to_systime(uint32_t sec);

/**
 * @brief  Get the passing time from the "start" in milliseconds.
 * @param[in] start: The start time which is in system time format.
 * @return : The passing time from "start" in milliseconds.
 */
int32_t	rtw_get_passing_time_ms(uint32_t start);

/**
 * @brief  Get the interval time from the "start" to "end" in milliseconds.
 * @param[in] start: The start time which is in system time format.
 * @param[in] end: The end time which is in system time format.
 * @return : The interval time from "start" to "end" in milliseconds.
 */
int32_t	rtw_get_time_interval_ms(uint32_t start, uint32_t end);
/*************************** End Time Management *******************************/

/**
 * @brief  This function suspends execution of the calling thread for "ms" milliseconds.
 * @param[in] ms: The time that the function sleep in milliseconds
 * @return	  None
*/
void	rtw_msleep_os(int ms);

/**
 * @brief  This function suspends execution of the calling thread for "us" microseconds.
 * @param[in] ms: The time that the function sleep in microseconds
 * @return	  None
*/
void	rtw_usleep_os(int us);

/**
 * @brief  This function converts the initial portion of the string to integer.
 * @param[in] s: The pointer to the string to be converted.
 * @return	  The converted value.
*/
uint32_t 	rtw_atoi(uint8_t *s);

/**
 * @brief  This function delays a task for the giving time in milliseconds.
 * @param[in] ms: The amount of time, in milliseconds, that the calling task should block.
 * @return	  None
*/
void	rtw_mdelay_os(int ms);

/**
 * @brief  This function delays a task for the giving time in microseconds.
 * @param[in] ms: The amount of time, in microseconds, that the calling task should block.
 * @return	  None
*/
void	rtw_udelay_os(int us);

/**
 * @brief  This function for forcing a context switch.
 * @return	  None
*/
void	rtw_yield_os(void);


uint64_t	rtw_modular64(uint64_t x, uint64_t y);

/**
 * @brief  This function generates random bytes.
 * @param[in] dst: The pointer to the buffer to store the random bytes.
 * @param[in] size: The size of the random bytes.
 * @return	  0
*/
int	rtw_get_random_bytes(void *dst, uint32_t size);

/**
 * @brief  This function gets the available heap size.
 * @return	  The value of the available heap size.
*/
uint32_t	rtw_getFreeHeapSize(void);

void	flush_signals_thread(void);

/**
 * @brief  This function indicates that the WLAN needs to stay on which means cannot go into power saving mode.
 * @return  None
 * @note  Defining configUSE_WAKELOCK_PMU 1 in "FreeRTOSConfig.h" needs to be done before compiling,
 *			or this API won't be effective.
 */
void	rtw_acquire_wakelock(void);

/**
 * @brief  This function indicates that the WLAN does not need to stay on which means can go into power saving mode.
 * @return  None
 * @note  Defining configUSE_WAKELOCK_PMU 1 in "FreeRTOSConfig.h" needs to be done before compiling,
 *			or this API won't be effective.
 */
void	rtw_release_wakelock(void);
void rtw_wakelock_timeout(uint32_t timeout);

/*********************************** Thread related *****************************************/

/**
 * @brief  This function creates a new task and adds it to the list of tasks that are ready to run.
 * @param[in] task:  The task stucture which will store the task related infomation.
 * @param[in] name: A descriptive name for the task.
 * @param[in] stack_size: The size of the task stack specified as the variables the stack can hold.
 * @param[in] priority: The priority at which the task should run.
 * @param[in] func: The task entry function.
 * @param[in] thctx: The pointer that will be used as the parameter for the task being created.
 * @return  pdPASS: The task was successfully created and added to a ready list.
 * @return  other error code defined in the file errors.h.
 * @note  For the task name, please do not use "rtw_little_wifi_mcu_thread", "rtw_check_in_req_state_thread",
 		  "rtw_TDMA_change_state_thread", "xmit_thread", "recv_thread", "rtw_recv_tasklet", "rtw_xmit_tasklet",
 		  "rtw_interrupt_thread", "cmd_thread", "usb_init", "MSC_BULK_CMD" and "MSC_BULK_DATA".
 */
int	rtw_create_task(struct task_struct *task, const char *name, uint32_t  stack_size, uint32_t priority, thread_func_t func, void *thctx);

/**
 * @brief  This function deletes a task.
 * @param[in] task:  The task stucture which will be deleted.
 * @return  None
 */
void rtw_delete_task(struct task_struct *task);

/**
 * @brief  This function wake up a task.
 * @param[in] task:  The task stucture which will be waked up.
 * @return  None
 */
void rtw_wakeup_task(struct task_struct *task);

void rtw_set_priority_task(void *task, uint32_t NewPriority);

int rtw_get_priority_task(void *task);

void rtw_suspend_task(void *task);

void rtw_resume_task(void *task);

void rtw_suspend_task_all(void);

void rtw_resume_task_all(void);


#if 0 //TODO
void	rtw_init_delayed_work(struct delayed_work *dwork, work_func_t func, const char *name);
void	rtw_deinit_delayed_work(struct delayed_work *dwork);
int		rtw_queue_delayed_work(struct workqueue_struct *wq, struct delayed_work *dwork, uint32_t delay, void *context);
BOOLEAN rtw_cancel_delayed_work(struct delayed_work *dwork);
#endif

/**
 * @brief  This function prints the name of the thread in DBG_INFO.
 * @param[in] name:  The name of the thread.
 * @return  None
 */
void	rtw_thread_enter(char *name);

/**
 * @brief  This function exits the calling thread.
 * @return  None
 */
void	rtw_thread_exit(void);

/**
 * @brief  This function gets the scheduler state of the calling thread.
 * @return  OS_SCHEDULER_NOT_STARTED
 * @return  OS_SCHEDULER_RUNNING
 * @return  OS_SCHEDULER_SUSPENDED
 */
uint8_t		rtw_get_scheduler_state(void);

/**
 * @brief  This function creats secure stack for threads.
 * @param[in] secure_stack_size:  The size of the secure stack for this thread, the unit is byte.
 * @return  None
 */
void rtw_create_secure_context(uint32_t secure_stack_size);

/*************************** End Threads *******************************/
#define rtw_warn_on(condition) do {} while (0)


/*************************** Timers *******************************/

/**
 * @brief  This function creates a new software timer instance.
 * @param[in] pcTimerName:  A text name that is assigned to the timer.
 * @param[in] xTimerPeriodInTicks: The timer period which is defined in tick periods.
 * @param[in] uxAutoReload: If uxAutoReload is set to pdTRUE then the timer will
 * expire repeatedly with a frequency set by the xTimerPeriodInTicks parameter.  If
 * uxAutoReload is set to pdFALSE then the timer will be a one-shot timer and
 * enter the dormant state after it expires.
 * @param[in] pvTimerID: An identifier that is assigned to the timer being created.
 * @param[in] pxCallbackFunction: The function to call when the timer expires.
 * @return  If the timer is successfully create then a handle to the newly
 * created timer is returned.  If the timer cannot be created, then 0 is returned.
 */
_timerHandle rtw_timerCreate(const signed char *pcTimerName,
							 osdepTickType xTimerPeriodInTicks,
							 uint32_t uxAutoReload,
							 void *pvTimerID,
							 TIMER_FUN pxCallbackFunction);

/**
 * @brief  This function deletes a timer that was previously created using rtw_timerCreate.
 * @param[in] xTimer:  The handle of the timer being deleted.
 * @param[in] xBlockTime: Specifies th etime, in ticks, that the calling task should be held in the Blocked
 *						  State to wait for the delete command to be successfully sent to the timer command queue,
 *						  should the queue already be full when rtw_timerDelete was called.
 * @return  pdFAIL will be returned if the delete command could not be sent to
 * the timer command queue even after xTicksToWait ticks had passed.  pdPASS will
 * be returned if the command was successfully sent to the timer command queue.
 * When the command is actually processed will depend on the priority of the
 * timer service/daemon task relative to other tasks in the system.
 */
uint32_t rtw_timerDelete(_timerHandle xTimer, osdepTickType xBlockTime);

/**
 * @brief  This function queries a timer to see if it is active or dormant.
 * @param[in] xTimer:  The timer being queried.
 * @return  pdFALSE will be returned if the timer is dormant.  A value other than
 * pdFALSE will be returned if the timer is active.
 * @note  A timer will be dormant if:
 *     1) It has been created but not started, or
 *     2) It is an expired one-shot timer that has not been restarted.
 */
uint32_t rtw_timerIsTimerActive(_timerHandle xTimer);

/**
 * @brief  This function stops a timer that was previously started.
 * @param[in] xTimer:  The handle of the timer being stopped.
 * @param[in] xBlockTime:  Specifies the time, in ticks, that the calling task should
 * be held in the Blocked state to wait for the stop command to be successfully
 * sent to the timer command queue, should the queue already be full when
 * rtw_timerStop() was called.
 * @return  pdFAIL will be returned if the stop command could not be sent to
 * the timer command queue even after xTicksToWait ticks had passed.  pdPASS will
 * be returned if the command was successfully sent to the timer command queue.
 * When the command is actually processed will depend on the priority of the
 * timer service/daemon task relative to other tasks in the system.
 */
uint32_t rtw_timerStop(_timerHandle xTimer, osdepTickType xBlockTime);

/**
 * @brief  This function changes the period of a timer that was previously created.
 * @param[in] xTimer:  The handle of the timer that is having its period changed.
 * @param[in] xNewPeriod:  The new period for xTimer.
 * @param[in] xBlockTime:  Specifies the time, in ticks, that the calling task should
 * be held in the Blocked state to wait for the change period command to be
 * successfully sent to the timer command queue, should the queue already be
 * full when rtw_timerChangePeriod() was called.
 * @return  pdFAIL will be returned if the change period command could not be
 * sent to the timer command queue even after xTicksToWait ticks had passed.
 * pdPASS will be returned if the command was successfully sent to the timer
 * command queue.  When the command is actually processed will depend on the
 * priority of the timer service/daemon task relative to other tasks in the
 * system.
 */
uint32_t rtw_timerChangePeriod(_timerHandle xTimer,
							   osdepTickType xNewPeriod,
							   osdepTickType xBlockTime);

void *rtw_timerGetID(_timerHandle xTimer);

uint32_t  rtw_timerStart(_timerHandle xTimer, osdepTickType xBlockTime);

uint32_t  rtw_timerStartFromISR(_timerHandle xTimer,
								osdepBASE_TYPE *pxHigherPriorityTaskWoken);

uint32_t  rtw_timerStopFromISR(_timerHandle xTimer,
							   osdepBASE_TYPE *pxHigherPriorityTaskWoken);

uint32_t  rtw_timerResetFromISR(_timerHandle xTimer,
								osdepBASE_TYPE *pxHigherPriorityTaskWoken);

uint32_t  rtw_timerChangePeriodFromISR(_timerHandle xTimer,
									   osdepTickType xNewPeriod,
									   osdepBASE_TYPE *pxHigherPriorityTaskWoken);

uint32_t  rtw_timerReset(_timerHandle xTimer,
						 osdepTickType xBlockTime);


/*************************** End Timers *******************************/
#ifdef LIST_CONTAINOR
#undef LIST_CONTAINOR
#endif
#define LIST_CONTAINOR(ptr, type, member) \
	((type *)(void*)((char *)(ptr)-(unsigned int)((char *)&((type *)(void*)ptr)->member - (char *)ptr)))

#ifndef time_after
#define time_after(a,b)	((long)(b) - (long)(a) < 0)
#endif
#ifndef time_before
#define time_before(a,b)	time_after(b,a)
#endif
#ifndef time_after_eq
#define time_after_eq(a,b)	((long)(a) - (long)(b) >= 0)
#endif
#ifndef time_before_eq
#define time_before_eq(a,b)	time_after_eq(b,a)
#endif

#define _RND(sz, r) ((((sz)+((r)-1))/(r))*(r))
#define RND4(x)	(((x >> 2) + (((x & 3) == 0) ?  0: 1)) << 2)

__inline static uint32_t _RND4(uint32_t sz)
{
	uint32_t	val;

	val = ((sz >> 2) + ((sz & 3) ? 1 : 0)) << 2;

	return val;
}

__inline static uint32_t _RND8(uint32_t sz)
{
	uint32_t	val;

	val = ((sz >> 3) + ((sz & 7) ? 1 : 0)) << 3;

	return val;
}

__inline static uint32_t _RND128(uint32_t sz)
{
	uint32_t	val;

	val = ((sz >> 7) + ((sz & 127) ? 1 : 0)) << 7;

	return val;
}

__inline static uint32_t _RND256(uint32_t sz)
{
	uint32_t	val;

	val = ((sz >> 8) + ((sz & 255) ? 1 : 0)) << 8;

	return val;
}

__inline static uint32_t _RND512(uint32_t sz)
{
	uint32_t	val;

	val = ((sz >> 9) + ((sz & 511) ? 1 : 0)) << 9;

	return val;
}

__inline static uint32_t bitshift(uint32_t bitmask)
{
	uint32_t i;

	for (i = 0; i <= 31; i++)
		if (((bitmask >> i) &  0x1) == 1) {
			break;
		}

	return i;
}

#define rtw_min(a, b) ((a > b) ? b : a)
#define rtw_is_range_a_in_b(hi_a, lo_a, hi_b, lo_b) (((hi_a) <= (hi_b)) && ((lo_a) >= (lo_b)))
#define rtw_is_range_overlap(hi_a, lo_a, hi_b, lo_b) (((hi_a) > (lo_b)) && ((lo_a) < (hi_b)))

/* Macros for handling unaligned memory accesses */

#define RTW_GET_BE16(a) ((uint16_t) (((a)[0] << 8) | (a)[1]))
#define RTW_PUT_BE16(a, val)			\
	do {					\
		(a)[0] = ((uint16_t) (val)) >> 8;	\
		(a)[1] = ((uint16_t) (val)) & 0xff;	\
	} while (0)

#define RTW_GET_LE16(a) ((uint16_t) (((a)[1] << 8) | (a)[0]))
#define RTW_PUT_LE16(a, val)			\
	do {					\
		(a)[1] = ((uint16_t) (val)) >> 8;	\
		(a)[0] = ((uint16_t) (val)) & 0xff;	\
	} while (0)

#define RTW_GET_BE24(a) ((((uint32_t) (a)[0]) << 16) | (((uint32_t) (a)[1]) << 8) | \
			 ((uint32_t) (a)[2]))
#define RTW_PUT_BE24(a, val)					\
	do {							\
		(a)[0] = (uint8_t) ((((uint32_t) (val)) >> 16) & 0xff);	\
		(a)[1] = (uint8_t) ((((uint32_t) (val)) >> 8) & 0xff);	\
		(a)[2] = (uint8_t) (((uint32_t) (val)) & 0xff);		\
	} while (0)

#define RTW_GET_BE32(a) ((((uint32_t) (a)[0]) << 24) | (((uint32_t) (a)[1]) << 16) | \
			 (((uint32_t) (a)[2]) << 8) | ((uint32_t) (a)[3]))
#define RTW_PUT_BE32(a, val)					\
	do {							\
		(a)[0] = (uint8_t) ((((uint32_t) (val)) >> 24) & 0xff);	\
		(a)[1] = (uint8_t) ((((uint32_t) (val)) >> 16) & 0xff);	\
		(a)[2] = (uint8_t) ((((uint32_t) (val)) >> 8) & 0xff);	\
		(a)[3] = (uint8_t) (((uint32_t) (val)) & 0xff);		\
	} while (0)

#define RTW_GET_LE32(a) ((((uint32_t) (a)[3]) << 24) | (((uint32_t) (a)[2]) << 16) | \
			 (((uint32_t) (a)[1]) << 8) | ((uint32_t) (a)[0]))
#define RTW_PUT_LE32(a, val)					\
	do {							\
		(a)[3] = (uint8_t) ((((uint32_t) (val)) >> 24) & 0xff);	\
		(a)[2] = (uint8_t) ((((uint32_t) (val)) >> 16) & 0xff);	\
		(a)[1] = (uint8_t) ((((uint32_t) (val)) >> 8) & 0xff);	\
		(a)[0] = (uint8_t) (((uint32_t) (val)) & 0xff);		\
	} while (0)

#define RTW_GET_BE64(a) ((((uint64_t) (a)[0]) << 56) | (((uint64_t) (a)[1]) << 48) | \
			 (((uint64_t) (a)[2]) << 40) | (((uint64_t) (a)[3]) << 32) | \
			 (((uint64_t) (a)[4]) << 24) | (((uint64_t) (a)[5]) << 16) | \
			 (((uint64_t) (a)[6]) << 8) | ((uint64_t) (a)[7]))
#define RTW_PUT_BE64(a, val)				\
	do {						\
		(a)[0] = (uint8_t) (((uint64_t) (val)) >> 56);	\
		(a)[1] = (uint8_t) (((uint64_t) (val)) >> 48);	\
		(a)[2] = (uint8_t) (((uint64_t) (val)) >> 40);	\
		(a)[3] = (uint8_t) (((uint64_t) (val)) >> 32);	\
		(a)[4] = (uint8_t) (((uint64_t) (val)) >> 24);	\
		(a)[5] = (uint8_t) (((uint64_t) (val)) >> 16);	\
		(a)[6] = (uint8_t) (((uint64_t) (val)) >> 8);	\
		(a)[7] = (uint8_t) (((uint64_t) (val)) & 0xff);	\
	} while (0)

#define RTW_GET_LE64(a) ((((uint64_t) (a)[7]) << 56) | (((uint64_t) (a)[6]) << 48) | \
			 (((uint64_t) (a)[5]) << 40) | (((uint64_t) (a)[4]) << 32) | \
			 (((uint64_t) (a)[3]) << 24) | (((uint64_t) (a)[2]) << 16) | \
			 (((uint64_t) (a)[1]) << 8) | ((uint64_t) (a)[0]))

struct osdep_service_ops {
	void *(*rtw_vmalloc)(uint32_t sz);
	void *(*rtw_zvmalloc)(uint32_t sz);
	void (*rtw_vmfree)(uint8_t *pbuf, uint32_t sz);
	void *(*rtw_malloc)(uint32_t sz);
	void *(*rtw_zmalloc)(uint32_t sz);
	void (*rtw_mfree)(uint8_t *pbuf, uint32_t sz);
	void (*rtw_memcpy)(void *dst, void *src, uint32_t sz);
	int (*rtw_memcmp)(void *dst, void *src, uint32_t sz);
	void (*rtw_memset)(void *pbuf, int c, uint32_t sz);
	void (*rtw_init_sema)(_sema *sema, int init_val);
	void (*rtw_free_sema)(_sema *sema);
	void (*rtw_up_sema)(_sema *sema);
	void (*rtw_up_sema_from_isr)(_sema *sema);
	uint32_t(*rtw_down_timeout_sema)(_sema *sema, uint32_t timeout);
	void (*rtw_mutex_init)(_mutex *pmutex);
	void (*rtw_mutex_free)(_mutex *pmutex);
	void (*rtw_mutex_get)(_mutex *pmutex);
	int (*rtw_mutex_get_timeout)(_mutex *pmutex, uint32_t timeout_ms);
	void (*rtw_mutex_put)(_mutex *pmutex);
	void (*rtw_enter_critical)(_lock *plock, _irqL *pirqL);
	void (*rtw_exit_critical)(_lock *plock, _irqL *pirqL);
	void (*rtw_enter_critical_from_isr)(_lock *plock, _irqL *pirqL);
	void (*rtw_exit_critical_from_isr)(_lock *plock, _irqL *pirqL);
	void (*rtw_enter_critical_bh)(_lock *plock, _irqL *pirqL);
	void (*rtw_exit_critical_bh)(_lock *plock, _irqL *pirqL);
	int (*rtw_enter_critical_mutex)(_mutex *pmutex, _irqL *pirqL);
	void (*rtw_exit_critical_mutex)(_mutex *pmutex, _irqL *pirqL);
	void (*rtw_cpu_lock)(void);
	void (*rtw_cpu_unlock)(void);
	void (*rtw_spinlock_init)(_lock *plock);
	void (*rtw_spinlock_free)(_lock *plock);
	void (*rtw_spin_lock)(_lock *plock);
	void (*rtw_spin_unlock)(_lock *plock);
	void (*rtw_spinlock_irqsave)(_lock *plock, _irqL *irqL);
	void (*rtw_spinunlock_irqsave)(_lock *plock, _irqL *irqL);
	int (*rtw_init_xqueue)(_xqueue *queue, const char *name, uint32_t message_size, uint32_t number_of_messages);
	int (*rtw_push_to_xqueue)(_xqueue *queue, void *message, uint32_t timeout_ms);
	int (*rtw_pop_from_xqueue)(_xqueue *queue, void *message, uint32_t timeout_ms);
	int (*rtw_peek_from_xqueue)(_xqueue *queue, void *message, uint32_t timeout_ms);
	int (*rtw_deinit_xqueue)(_xqueue *queue);
	uint32_t(*rtw_get_current_time)(void);
	uint32_t(*rtw_systime_to_ms)(uint32_t systime);
	uint32_t(*rtw_systime_to_sec)(uint32_t systime);
	uint32_t(*rtw_ms_to_systime)(uint32_t ms);
	uint32_t(*rtw_sec_to_systime)(uint32_t sec);
	void (*rtw_msleep_os)(int ms);
	void (*rtw_usleep_os)(int us);
	void (*rtw_mdelay_os)(int ms);
	void (*rtw_udelay_os)(int us);
	void (*rtw_yield_os)(void);
	void (*ATOMIC_SET)(ATOMIC_T *v, int i);
	int (*ATOMIC_READ)(ATOMIC_T *v);
	void (*ATOMIC_ADD)(ATOMIC_T *v, int i);
	void (*ATOMIC_SUB)(ATOMIC_T *v, int i);
	void (*ATOMIC_INC)(ATOMIC_T *v);
	void (*ATOMIC_DEC)(ATOMIC_T *v);
	int (*ATOMIC_ADD_RETURN)(ATOMIC_T *v, int i);
	int (*ATOMIC_SUB_RETURN)(ATOMIC_T *v, int i);
	int (*ATOMIC_INC_RETURN)(ATOMIC_T *v);
	int (*ATOMIC_DEC_RETURN)(ATOMIC_T *v);
	uint64_t(*rtw_modular64)(uint64_t x, uint64_t y);
	int (*rtw_get_random_bytes)(void *dst, uint32_t size);
	uint32_t(*rtw_getFreeHeapSize)(void);
	int (*rtw_create_task)(struct task_struct *task, const char *name, uint32_t stack_size, uint32_t priority, thread_func_t func, void *thctx);
	void (*rtw_delete_task)(struct task_struct *task);
	void (*rtw_wakeup_task)(struct task_struct *task);
	void (*rtw_set_priority_task)(void *task, uint32_t NewPriority);
	int (*rtw_get_priority_task)(void *task);
	void (*rtw_suspend_task)(void *task);
	void (*rtw_resume_task)(void *task);
	void (*rtw_suspend_task_all)(void);
	void (*rtw_resume_task_all)(void);

#if 0	//TODO
	void (*rtw_init_delayed_work)(struct delayed_work *dwork, work_func_t func, const char *name);
	void (*rtw_deinit_delayed_work)(struct delayed_work *dwork);
	int (*rtw_queue_delayed_work)(struct workqueue_struct *wq, struct delayed_work *dwork, unsigned long delay, void *context);
	BOOLEAN(*rtw_cancel_delayed_work)(struct delayed_work *dwork);
#endif
	void (*rtw_thread_enter)(char *name);
	void (*rtw_thread_exit)(void);
	_timerHandle(*rtw_timerCreate)(const signed char *pcTimerName,
								   osdepTickType xTimerPeriodInTicks,
								   uint32_t uxAutoReload,
								   void *pvTimerID,
								   TIMER_FUN pxCallbackFunction);
	uint32_t(*rtw_timerDelete)(_timerHandle xTimer,
							   osdepTickType xBlockTime);
	uint32_t(*rtw_timerIsTimerActive)(_timerHandle xTimer);
	uint32_t(*rtw_timerStop)(_timerHandle xTimer,
							 osdepTickType xBlockTime);
	uint32_t(*rtw_timerChangePeriod)(_timerHandle xTimer,
									 osdepTickType xNewPeriod,
									 osdepTickType xBlockTime);
	void *(*rtw_timerGetID)(_timerHandle xTimer);
	uint32_t(*rtw_timerStart)(_timerHandle xTimer,
							  osdepTickType xBlockTime);
	uint32_t(*rtw_timerStartFromISR)(_timerHandle xTimer,
									 osdepBASE_TYPE *pxHigherPriorityTaskWoken);

	uint32_t(*rtw_timerStopFromISR)(_timerHandle xTimer,
									osdepBASE_TYPE *pxHigherPriorityTaskWoken);

	uint32_t(*rtw_timerResetFromISR)(_timerHandle xTimer,
									 osdepBASE_TYPE *pxHigherPriorityTaskWoken);

	uint32_t(*rtw_timerChangePeriodFromISR)(_timerHandle xTimer,
											osdepTickType xNewPeriod,
											osdepBASE_TYPE *pxHigherPriorityTaskWoken);

	uint32_t(*rtw_timerReset)(_timerHandle xTimer,
							  osdepTickType xBlockTime);

	void (*rtw_acquire_wakelock)(void);
	void (*rtw_release_wakelock)(void);
	void (*rtw_wakelock_timeout)(uint32_t timeoutMs);
	uint8_t(*rtw_get_scheduler_state)(void);
	void (*rtw_create_secure_context)(uint32_t secure_stack_size);
	void *(*rtw_get_current_TaskHandle)(void);
#if( configSUPPORT_STATIC_ALLOCATION == 1 )
	void (*rtw_init_sema_static)(_sema *sema, int init_val, StaticSemaphore_t *xSemaphoreBuffer);
	void (*rtw_mutex_init_static)(_mutex *pmutex, StaticSemaphore_t *xSemaphoreBuffer);
	void (*rtw_spinlock_init_static)(_lock *plock, StaticSemaphore_t *xSemaphoreBuffer);
#endif
};

#ifdef __cplusplus
}
#endif

/*************************** ATOMIC Integer *******************************/

/**
 * @brief  This function atomically sets the value of the variable.
 * @param[in] v: Pointer of type atomic_t that to be set value.
 * @param[in] i: Required value.
 * @return	  None
 * @note    The guaranteed useful range of an atomic_t is only 24 bits.
*/
void 	ATOMIC_SET(ATOMIC_T *v, int i);

/**
 * @brief  This function atomically reads the value of the variable.
 * @param[in] v: Pointer of type atomic_t that to be read.
 * @return	  The value of the variable.
 * @note	The guaranteed useful range of an atomic_t is only 24 bits.
*/
int		ATOMIC_READ(ATOMIC_T *v);

/**
 * @brief  This function adds "i" to the contained "v".
 * @param[in] v: Pointer of type atomic_t.
 * @param[in] i: value to add.
 * @return	  None
*/
void 	ATOMIC_ADD(ATOMIC_T *v, int i);

/**
 * @brief  This function subtracts "i" from th econtained "v".
 * @param[in] v: Pointer of type atomic_t.
 * @param[in] i: value to subtract.
 * @return	  None
*/
void 	ATOMIC_SUB(ATOMIC_T *v, int i);

/**
 * @brief  This function adds 1 to the contained "v".
 * @param[in] v: Pointer of type atomic_t.
 * @return	  None
*/
void 	ATOMIC_INC(ATOMIC_T *v);

/**
 * @brief  This function subtracts 1 from th econtained "v".
 * @param[in] v: Pointer of type atomic_t.
 * @return	  None
*/
void 	ATOMIC_DEC(ATOMIC_T *v);

/**
 * @brief  This function adds "i" to the contained "v" and returns the result.
 * @param[in] v: Pointer of type atomic_t.
 * @param[in] i: value to add.
 * @return	  None
*/
int 	ATOMIC_ADD_RETURN(ATOMIC_T *v, int i);

/**
 * @brief  This function subtracts "i" from th econtained "v" and returns the result.
 * @param[in] v: Pointer of type atomic_t.
 * @param[in] i: value to subtract.
 * @return	  None
*/
int 	ATOMIC_SUB_RETURN(ATOMIC_T *v, int i);

/**
 * @brief  This function adds 1 to the contained "v" and returns the result.
 * @param[in] v: Pointer of type atomic_t.
 * @return	  None
*/
int 	ATOMIC_INC_RETURN(ATOMIC_T *v);

/**
 * @brief  This function subtracts 1 from th econtained "v" and returns the result.
 * @param[in] v: Pointer of type atomic_t.
 * @return	  None
*/
int 	ATOMIC_DEC_RETURN(ATOMIC_T *v);

/**
 * @brief  This function subtracts 1 from th econtained "v" and test if the result equals 0.
 * @param[in] v: Pointer of type atomic_t.
 * @return	  0: The result after subtracting 1 is 0
 * @return	 -1: The result after subtracting 1 is not 0
*/
int ATOMIC_DEC_AND_TEST(ATOMIC_T *v);
/*************************** End ATOMIC *******************************/

/*\@}*/

void init_mutex_pool(void);

#endif	//#ifndef __OSDEP_SERVICE_H_

