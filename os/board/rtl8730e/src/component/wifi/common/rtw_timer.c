/**
  ******************************************************************************
  * @file    rtw_timer.c
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

#include <basic_types.h>
#include <os_wrapper.h>
#include <rtw_timer.h>
#include <rtw_queue.h>
#include "log.h"
#include "diag.h"
_list timer_table;

static int timer_used_num;
int max_timer_used_num;
static const char *TAG = "TIMER";

void init_timer_wrapper(void)
{
	rtw_init_listhead(&timer_table);
	timer_used_num = 0;
	max_timer_used_num = 0;
}

void deinit_timer_wrapper(void)
{
	_list *plist;

	if (timer_used_num > 0) {
		RTK_LOGI(TAG, "del timer entry %d\n", timer_used_num);
	}

#if defined(CONFIG_PLATFORM_TIZENRT_OS) && defined(ARM_CORE_CA32)
	irqstate_t flags = tizenrt_critical_enter();
#else
	rtos_critical_enter();
#endif //#if defined(CONFIG_PLATFORM_TIZENRT_OS) && defined(ARM_CORE_CA32)

	while (rtw_end_of_queue_search(&timer_table, get_next(&timer_table)) == _FALSE) {
		plist = get_next(&timer_table);
		rtw_list_delete(plist);
	}

#if defined(CONFIG_PLATFORM_TIZENRT_OS) && defined(ARM_CORE_CA32)
	tizenrt_critical_exit(flags);
#else
	rtos_critical_exit();
#endif //#if defined(CONFIG_PLATFORM_TIZENRT_OS) && defined(ARM_CORE_CA32)
}

void timer_wrapper(rtos_timer_t timer_hdl)
{
	_list *plist;
	struct timer_list *timer_entry = NULL;

#if defined(CONFIG_PLATFORM_TIZENRT_OS) && defined(ARM_CORE_CA32)
	irqstate_t flags = tizenrt_critical_enter();
#else
	rtos_critical_enter();
#endif //#if defined(CONFIG_PLATFORM_TIZENRT_OS) && defined(ARM_CORE_CA32)

	plist = get_next(&timer_table);
	while ((rtw_end_of_queue_search(&timer_table, plist)) == _FALSE) {
		timer_entry = LIST_CONTAINOR(plist, struct timer_list, list);
		if (timer_entry->timer_hdl == timer_hdl) {
			break;
		}
		plist = get_next(plist);
	}

#if defined(CONFIG_PLATFORM_TIZENRT_OS) && defined(ARM_CORE_CA32)
	tizenrt_critical_exit(flags);
#else
	rtos_critical_exit();
#endif //#if defined(CONFIG_PLATFORM_TIZENRT_OS) && defined(ARM_CORE_CA32)

	if (plist == &timer_table) {
		RTK_LOGE(TAG, "Fail to find the timer_entry in timer table.\n");
	} else {
		if (timer_entry->function) {
			timer_entry->function((void *) timer_entry->data);
		}
	}
}

void init_timer(struct timer_list *timer, const char *name)
{
	if (timer->function == NULL) {
		return;
	}

	if (timer->timer_hdl == NULL) {
		rtos_timer_create_static(&timer->timer_hdl,
								 (const char *)name,	// Just a text name, not used by the RTOS kernel.
								 (uint32_t)NULL,	// Uniq id used to identify which timer expire..
								 RTOS_MAX_DELAY, // Timer Period, not 0
								 _FALSE, // Whether timer will auto-load themselves when expires
								 timer_wrapper); // Timer callback

		if (timer->timer_hdl == NULL) {
			RTK_LOGE(TAG, "Fail to init timer.\n");
		} else {

#if defined(CONFIG_PLATFORM_TIZENRT_OS) && defined(ARM_CORE_CA32)
			irqstate_t flags = tizenrt_critical_enter();
#else
			rtos_critical_enter();
#endif //#if defined(CONFIG_PLATFORM_TIZENRT_OS) && defined(ARM_CORE_CA32)

			rtw_list_insert_head(&timer->list, &timer_table);

#if defined(CONFIG_PLATFORM_TIZENRT_OS) && defined(ARM_CORE_CA32)
			tizenrt_critical_exit(flags);
#else
			rtos_critical_exit();
#endif //#if defined(CONFIG_PLATFORM_TIZENRT_OS) && defined(ARM_CORE_CA32)

			timer_used_num ++;
			if (timer_used_num > max_timer_used_num) {
				max_timer_used_num = timer_used_num;
			}
		}
	} else if (rtos_timer_is_timer_active(timer->timer_hdl) == TRUE) {
		rtos_timer_stop(timer->timer_hdl, RTOS_TIMER_MAX_DELAY);
	}
}

void mod_timer(struct timer_list *timer, u32 delay_time_ms)
{
	if (timer->timer_hdl == NULL) {
		RTK_LOGS(TAG, "ModTimer: not init.\n");
	} else if (rtos_timer_is_timer_active(timer->timer_hdl) == TRUE) {
		rtos_timer_stop(timer->timer_hdl, RTOS_TIMER_MAX_DELAY);
	}

	//Set Timer period
	if (timer->timer_hdl != NULL)
		if (rtos_timer_change_period(timer->timer_hdl, delay_time_ms, RTOS_TIMER_MAX_DELAY) == FAIL) {
			RTK_LOGS(TAG, "ModTimer fail\n");
		}
}

void  cancel_timer_ex(struct timer_list *timer)
{
	_list *plist;
	struct timer_list *timer_entry = NULL;

	if (timer->timer_hdl == NULL) {
		return;
	}

#if defined(CONFIG_PLATFORM_TIZENRT_OS) && defined(ARM_CORE_CA32)
	irqstate_t flags = tizenrt_critical_enter();
#else
	rtos_critical_enter();
#endif //#if defined(CONFIG_PLATFORM_TIZENRT_OS) && defined(ARM_CORE_CA32)

	plist = get_next(&timer_table);
	while ((rtw_end_of_queue_search(&timer_table, plist)) == _FALSE) {
		timer_entry = LIST_CONTAINOR(plist, struct timer_list, list);
		if (timer_entry->timer_hdl == timer->timer_hdl) {
			break;
		}
		plist = get_next(plist);
	}

#if defined(CONFIG_PLATFORM_TIZENRT_OS) && defined(ARM_CORE_CA32)
	tizenrt_critical_exit(flags);
#else
	rtos_critical_exit();
#endif //#if defined(CONFIG_PLATFORM_TIZENRT_OS) && defined(ARM_CORE_CA32)

	if (plist == &timer_table) {
		RTK_LOGS(TAG, "CancelTimer Fail(%x)\n", (unsigned int)timer->timer_hdl);
	} else {
		rtos_timer_stop(timer->timer_hdl, RTOS_TIMER_MAX_DELAY);
	}
}

void  del_timer_sync(struct timer_list *timer)
{
	_list *plist;
	struct timer_list *timer_entry;

	if (timer->timer_hdl == NULL) {
		return;
	}

#if defined(CONFIG_PLATFORM_TIZENRT_OS) && defined(ARM_CORE_CA32)
	irqstate_t flags = tizenrt_critical_enter();
#else
	rtos_critical_enter();
#endif //#if defined(CONFIG_PLATFORM_TIZENRT_OS) && defined(ARM_CORE_CA32)

	plist = get_next(&timer_table);
	while ((rtw_end_of_queue_search(&timer_table, plist)) == _FALSE) {
		timer_entry = LIST_CONTAINOR(plist, struct timer_list, list);
		if (timer_entry->timer_hdl == timer->timer_hdl) {
			rtw_list_delete(plist);
			break;
		}
		plist = get_next(plist);
	}

#if defined(CONFIG_PLATFORM_TIZENRT_OS) && defined(ARM_CORE_CA32)
	tizenrt_critical_exit(flags);
#else
	rtos_critical_exit();
#endif //#if defined(CONFIG_PLATFORM_TIZENRT_OS) && defined(ARM_CORE_CA32)

	if (plist == &timer_table) {
		RTK_LOGS(TAG, "DelTimer Fail\n");
	} else {
		rtos_timer_delete_static(timer->timer_hdl, RTOS_TIMER_MAX_DELAY);
		timer->timer_hdl = NULL;
		timer_used_num --;
	}
}

void rtw_init_timer(_timer *ptimer, void *adapter, TIMER_FUN pfunc, void *cntx, const char *name)
{
	/* To avoid gcc warnings */
	(void) adapter;
	(void) name;

	ptimer->function = pfunc;
	ptimer->data = (u32) cntx;
	init_timer(ptimer, name);
}

void rtw_set_timer(_timer *ptimer, u32 delay_time_ms)
{
	mod_timer(ptimer, delay_time_ms);
}

u8 rtw_cancel_timer(_timer *ptimer)
{
	cancel_timer_ex(ptimer);
	return 0;
}

void rtw_del_timer(_timer *ptimer)
{
	del_timer_sync(ptimer);
}
