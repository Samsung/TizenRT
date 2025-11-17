/**
  ******************************************************************************
  * @file    rtw_misc.h
  * @author
  * @version
  * @date
  * @brief
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */


#ifndef __RTW_MISC_H_
#define __RTW_MISC_H_


/* defined by Realtek, for offseting wlan task priority */
#define PRIORITIE_OFFSET								( 4 )

// -------------------- from osdep_service_xxx.h --------------------
#define rtw_warn_on(condition) do {} while (0)
#define netif_queue_stopped(dev)	(0)
#define netif_wake_queue(dev)		do { } while (0)

#define TASK_PRORITY_LOW  				1
#define TASK_PRORITY_MIDDLE   			2
#define TASK_PRORITY_HIGH    			3
#define TASK_PRORITY_SUPER    			4
typedef void			    thread_return;
typedef void			    *thread_context;
typedef thread_return(*thread_func_t)(thread_context context);
#define CONFIG_THREAD_COMM_SEMA
struct task_struct {
	const char *task_name;
	rtos_task_t task;	/* I: workqueue thread */
#ifdef CONFIG_THREAD_COMM_SIGNAL
	const char *name;	/* I: workqueue thread name */
	uint32_t queue_num;		/* total signal num */
	uint32_t cur_queue_num;	/* cur signal num should < queue_num */
#elif defined(CONFIG_THREAD_COMM_SEMA)
	rtos_sema_t wakeup_sema;    /* for internal use only */
	rtos_sema_t terminate_sema; /* for internal use only */
#endif
	uint32_t blocked;          /* for internal use only */
	uint32_t callback_running; /* for internal use only */
};

// ------------------------------------------------------------------
#if defined(CONFIG_PLATFORM_TIZENRT_OS) && defined(ARM_CORE_CA32)
#define HALT()				do { tizenrt_critical_enter(); for(;;);} while(0)
#else
#define HALT()				do { rtos_critical_enter(); for(;;);} while(0)
#endif //#if defined(CONFIG_PLATFORM_TIZENRT_OS) && defined(ARM_CORE_CA32)
#undef ASSERT
#define ASSERT(x)			do { \
						if((x) == 0){\
							DiagPrintf("\n\rAssert(" #x ") failed on line %d in file %s\n", __LINE__, __FILE__); \
						HALT();}\
					} while(0)

//#define container_of(p,t,n) (t*)((p)-&(((t*)0)->n))
#if !defined(container_of)
#define container_of(ptr, type, member) \
			((type *)(void*)((char *)(ptr)-(SIZE_T)(&((type *)0)->member)))
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef FIELD_OFFSET
#define FIELD_OFFSET(s,field)	((SSIZE_T)&((s*)(0))->field)
#endif
#define rtw_min(a, b) ((a > b) ? b : a)


__inline static u16 _htons(u16 n)
{
	return ((n & 0xff) << 8) | ((n & 0xff00) >> 8);
}

__inline static u16 _ntohs(u16 n)
{
	return _htons(n);
}

__inline static u32 _htonl(u32 n)
{
	return ((n & 0xff) << 24) |
		   ((n & 0xff00) << 8) |
		   ((n & 0xff0000UL) >> 8) |
		   ((n & 0xff000000UL) >> 24);
}


#ifdef __cplusplus
}
#endif

#endif
