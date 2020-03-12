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
#ifndef _CUSTOMER_RTOS_SERVICE_H_
#define _CUSTOMER_RTOS_SERVICE_H_

//----- ------------------------------------------------------------------
// Include Files
//----- ------------------------------------------------------------------

#include "dlist.h"

#include <queue.h>
#include <sched.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <threads.h>
#include <tinyara/clock.h>
#include <tinyara/wqueue.h>

#include <arch/irq.h>
#include <string.h>
#include <tinyara/arch.h>
#include <tinyara/irq.h>
#include <tinyara/kmalloc.h>
#include <tinyara/time.h>

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include "wireless.h"
#include "../include/debug.h"

#ifdef CONFIG_USB_HCI
typedef struct urb *PURB;
#endif

// --------------------------------------------
//	Platform dependent type define
// --------------------------------------------
#if defined(CONFIG_PLATFORM_8195A) || defined(CONFIG_PLATFORM_8195BHP) || defined(CONFIG_PLATFORM_8710C)
#include "platform/platform_stdlib.h"
extern VOID RtlUdelayOS(u32 us);
#elif defined(CONFIG_PLATFORM_8711B)
#include "platform/platform_stdlib.h"
#elif defined(CONFIG_PLATFORM_8721D)
#include "platform/platform_stdlib.h"
#elif defined(CONFIG_HARDWARE_8821C)
#include "basic_types.h"
#include "wlan_basic_types.h"
#elif defined(CONFIG_HARDWARE_8188F)
#include "platform/platform_stdlib.h"
#elif defined(CONFIG_HARDWARE_8192E)
#include "platform/platform_stdlib.h"
#elif defined(CONFIG_HARDWARE_8723D)
#include "platform/platform_stdlib.h"
#elif defined(CONFIG_HARDWARE_8188E)
#include "platform/platform_stdlib.h"
#else
// other MCU may use standard library 
#include <string.h>
#endif
#ifndef FIELD_OFFSET
#define FIELD_OFFSET(s, field) ((SSIZE_T) & ((s *)(0))->field)
#endif

// os types
typedef char osdepCHAR;
typedef float osdepFLOAT;
typedef double osdepDOUBLE;
typedef long osdepLONG;
typedef short osdepSHORT;
typedef unsigned long osdepSTACK_TYPE;
typedef long osdepBASE_TYPE;
typedef unsigned long osdepTickType;

// lock and semaphore
typedef void*	            _timerHandle;
typedef void *_sema;
typedef void *_mutex;

typedef void*	            _lock;

typedef void *_queueHandle;
typedef void *_xqueue;
typedef struct timer_list _timer;

typedef int (*task_fun)(int argc, char *argv[]);

#define _SUCCESS 1
#define _FAIL 0
#define VOID void

typedef struct sk_buff _pkt;
typedef unsigned char _buffer;

#ifndef __LIST_H
#warning "DLIST_NOT_DEFINE!!!!!!"
struct list_head {
	struct list_head *next, *prev;
};
#endif

struct __queue {
	struct list_head queue;
	_lock lock;
};

typedef struct __queue _queue;
typedef struct list_head _list;
typedef unsigned long _irqL;

typedef void *_thread_hdl_;
typedef void thread_return;
typedef void *thread_context;

#if !defined(CONFIG_PLATFORM_8710C)
typedef struct {
	volatile int counter;
} atomic_t;
#endif

#define ATOMIC_T atomic_t

struct timer_entry {
	struct list_head list;
	struct timer_list *timer;
#ifdef TIMEOUT_LEAK_DBG
	void (*traced_func)(void *);
	int timeout;
#endif
};

#define TMR_NAME_SIZE 16
struct timer_list_priv {
        struct timer_list *prev, *next;
        _timerHandle timer_hdl;
        struct work_s *work_hdl;
        int timer_id;
        unsigned char timer_name[TMR_NAME_SIZE];
        int live;
        long timevalue;
        void *data;
        void (*function)(void *args);
};

#define HZ TICK_PER_SEC
#define rtw_timer_list timer_list

#define KERNEL_VERSION(a, b, c) (((a) << 16) + ((b) << 8) + (c))
/* emulate a modern version */
#define LINUX_VERSION_CODE KERNEL_VERSION(2, 6, 17)

#define PRIORITIE_OFFSET (120)

#define INIT_LINK_HEAD(_list_)                        \
	do {                                              \
		(_list_)->flink = (_list_)->blink = (_list_); \
	} while (0)

static __inline _list *get_next(_list *list)
{
	return list->next;
}

static __inline _list *get_list_head(_queue *queue)
{
	return (&(queue->queue));
}

#define container_of(ptr, type, member) \
	((type *)((char *)(ptr) - (SIZE_T)(&((type *)0)->member)))
#define TASK_PRORITY_LOW 1
#define TASK_PRORITY_MIDDLE 2
#define TASK_PRORITY_HIGH 3
#define TASK_PRORITY_SUPER 4

#define TIMER_MAX_DELAY 0xFFFFFFFF

#ifndef PRIORITIE_OFFSET
#define PRIORITIE_OFFSET (0)
#endif

void save_and_cli(void);
void restore_flags(void);
void cli(void);

#ifndef mdelay
#define mdelay(t) usleep(t * 1000)
#endif

#ifndef udelay
#define udelay(t) usleep(t)
#endif

//----- ------------------------------------------------------------------
// Common Definition
//----- ------------------------------------------------------------------

#define __init
#define __exit
#define __devinit
#define __devexit

#define KERN_ERR
#define KERN_INFO
#define KERN_NOTICE

#undef GFP_KERNEL
#define GFP_KERNEL 1
#define GFP_ATOMIC 1

#define SET_MODULE_OWNER(some_struct) \
	do {                              \
	} while (0)
#define SET_NETDEV_DEV(dev, obj) \
	do {                         \
	} while (0)
#define register_netdev(dev) (0)
#define unregister_netdev(dev) \
	do {                       \
	} while (0)
#define netif_queue_stopped(dev) (0)
#define netif_wake_queue(dev) \
	do {                      \
	} while (0)
#define printk nvdbg

#define DBG_ERR(fmt, args...) ndbg("\n\r[%s] " fmt, __FUNCTION__, ##args)
#ifdef WLAN_INTF_DBG
#define DBG_TRACE(fmt, args...) nvdbg("\n\r[%s] " fmt, __FUNCTION__, ##args)
#define DBG_INFO(fmt, args...) nvdbg("\n\r[%s] " fmt, __FUNCTION__, ##args)
#else
#define DBG_TRACE(fmt, args...)
#define DBG_INFO(fmt, args...)
#endif
#define HALT()   \
	do {         \
		cli();   \
		for (;;) \
			;    \
	} while (0)
#undef ASSERT
#define ASSERT(x)                                                              \
	do {                                                                       \
		if ((x) == 0) {                                                        \
			nvdbg("\n\rAssert(" #x ") failed on line %d in file %s", __LINE__, \
				  __FILE__);                                                   \
			HALT();                                                            \
		}                                                                      \
	} while (0)

#undef DBG_ASSERT
#define DBG_ASSERT(x, msg)                                                    \
	do {                                                                      \
		if ((x) == 0)                                                         \
			nvdbg("\n\r%s, Assert(" #x ") failed on line %d in file %s", msg, \
				  __LINE__, __FILE__);                                        \
	} while (0)

//----- ------------------------------------------------------------------
// Atomic Operation
//----- ------------------------------------------------------------------

/*
 * atomic_read - read atomic variable
 * @v: pointer of type atomic_t
 *
 * Atomically reads the value of @v.  Note that the guaranteed
 * useful range of an atomic_t is only 24 bits.
 */
#undef atomic_read
#define atomic_read(v) ((v)->counter)

/*
 * atomic_set - set atomic variable
 * @v: pointer of type atomic_t
 * @i: required value
 *
 * Atomically sets the value of @v to @i.  Note that the guaranteed
 * useful range of an atomic_t is only 24 bits.
 */
#undef atomic_set
#define atomic_set(v, i) ((v)->counter = (i))

/****************************Work
 * Queue******************************************************/
/**
 * @ingroup workqueue
 * Initialize a work.
 *
 */
#ifdef CONFIG_THREAD_COMM_SIGNAL
typedef struct work_s _workitem;
typedef struct work_s work_struct;
#define INIT_WORK(_work, _func)         \
	do {                                \
		INIT_LINK_HEAD(&((_work)->dq)); \
		(_work)->worker = (_func);      \
		(_work)->arg = (FAR void *)(0); \
		(_work)->qtime = 0;             \
		(_work)->delay = 0;             \
	} while (0)

static inline void _init_workitem(_workitem *pwork, void *pfunc, void *cntx)
{
	INIT_WORK(pwork, pfunc);
}

inline static void _set_workitem(_workitem *pwork)
{
	work_queue(HPWORK, pwork, pwork->worker, pwork->arg, pwork->delay);
}

inline static void _cancel_workitem_sync(_workitem *pwork)
{
	work_cancel(HPWORK, pwork);
}
#endif
/****************************Work Queue
 * End*************************************************/

extern void rtw_init_listhead(_list *list);
extern u32 rtw_is_list_empty(_list *phead);
extern void rtw_list_insert_head(_list *plist, _list *phead);
extern void rtw_list_insert_tail(_list *plist, _list *phead);
extern void rtw_list_delete(_list *plist);

#if (defined CONFIG_PLATFORM_8711B) || (defined CONFIG_PLATFORM_8721D)
extern u32 random_seed;
#endif

#endif /* _CUSTOMER_RTOS_SERVICE_H_ */
