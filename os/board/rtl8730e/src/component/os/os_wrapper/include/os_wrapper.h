/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __OS_WRAPPER_H__
#define __OS_WRAPPER_H__

/**
 * @brief  Necessary headers
*/
#include "platform_autoconf.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "rtw_queue.h"
#include "rtw_timer.h"
#include "rtw_atomic.h"
/* Added for os_wrapper_misc.c */
#include "basic_types.h"
#include <ameba_vector.h>
#include <ameba.h>
/**
 * @brief  Common header file
*/
#ifdef __cplusplus
extern "C" {
#endif
/////////////////////////////////////////////////////////////TizenRT service header start
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

#include "../include/debug.h"

#ifdef CONFIG_USB_HCI
typedef struct urb *PURB;
#endif

#if (defined CONFIG_GSPI_HCI || defined CONFIG_SDIO_HCI) || defined(CONFIG_LX_HCI)
/* For SPI interface transfer and us delay implementation */
#if !defined(CONFIG_PLATFORM_8195A) && !defined(CONFIG_PLATFORM_8711B) && !defined(CONFIG_PLATFORM_8721D)  && !defined(CONFIG_PLATFORM_8195BHP) && !defined(CONFIG_PLATFORM_8710C)
#include <rtwlan_bsp.h>
#endif
#endif

// --------------------------------------------
//  Platform dependent type define
// --------------------------------------------
#ifndef FIELD_OFFSET
#define FIELD_OFFSET(s, field)	((SSIZE_T)&((s*)(0))->field)
#endif

typedef void *_sema;
typedef void *_mutex;
typedef void *_lock;
typedef void *_queueHandle;
typedef void *_xqueue;

typedef struct sk_buff _pkt;
typedef unsigned char _buffer;
typedef unsigned int systime;

#if !defined(__LIST_H) && !defined(__LIST_H__)
#warning "DLIST_NOT_DEFINE!!!!!!"
struct list_head {
	struct list_head *next, *prev;
};
#endif

typedef void *_thread_hdl_;
typedef void thread_return;
typedef void *thread_context;

#define HZ TICK_PER_SEC

#define   KERNEL_VERSION(a, b, c) (((a) << 16) + ((b) << 8) + (c))
/* emulate a modern version */
#define LINUX_VERSION_CODE KERNEL_VERSION(2, 6, 17)

#ifndef LIST_CONTAINOR
#define LIST_CONTAINOR(ptr, type, member) \
	((type *)(void*)((char *)(ptr)-(SIZE_T)((char *)&((type *)(void*)ptr)->member - (char *)ptr)))
#endif //#ifndef LIST_CONTAINOR
//#define container_of(p,t,n) (t*)((p)-&(((t*)0)->n))
#define container_of(ptr, type, member) \
			((type *)((char *)(ptr)-(SIZE_T)(&((type *)0)->member)))
#define TASK_PRORITY_LOW			1
#define TASK_PRORITY_MIDDLE			2
#define TASK_PRORITY_HIGH			3
#define TASK_PRORITY_SUPER			4

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
#define GFP_KERNEL			1
#define GFP_ATOMIC			1

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

/*
 *      These inlines deal with timer wrapping correctly. You are
 *      strongly encouraged to use them
 *      1. Because people otherwise forget
 *      2. Because if the timer wrap changes in future you wont have to
 *         alter your driver code.
 *
 * time_after(a,b) returns true if the time a is after time b.
 *
 * Do this with "<0" and ">=0" to only test the sign of the result. A
 * good compiler would generate better code (and a really good compiler
 * wouldn't care). Gcc is currently neither.
 */
#define time_after(a, b)	((long)(b) - (long)(a) < 0)
#define time_before(a, b)	time_after(b, a)

#define time_after_eq(a, b)	((long)(a) - (long)(b) >= 0)
#define time_before_eq(a, b)	time_after_eq(b, a)

extern int rtw_if_wifi_thread(char *name);

int rtw_printf(const char *format,...);
int rtw_printf_info(const char *format,...);

#if (defined CONFIG_PLATFORM_8711B) || (defined CONFIG_PLATFORM_8721D)
extern u32 random_seed;
#endif

#define configSUPPORT_STATIC_ALLOCATION					0

/////////////////////////////////////////////////////////////TizenRT service header end

#include <tinyara/sched.h>

#include "os_wrapper_critical.h"
#include "os_wrapper_memory.h"
#include "os_wrapper_mutex.h"
#include "os_wrapper_queue.h"
#include "os_wrapper_semaphore.h"
#include "os_wrapper_task.h"
#include "os_wrapper_time.h"
#include "os_wrapper_timer.h"
#include "os_wrapper_static_functions.h"

#ifdef __cplusplus
}
#endif

/**
 * @brief  General macro definition
 */
#define RTOS_MAX_DELAY    				0xFFFFFFFFUL
#define RTOS_MAX_TIMEOUT    			0xFFFFFFFFUL

#endif //#ifndef __OS_WRAPPER_H__
