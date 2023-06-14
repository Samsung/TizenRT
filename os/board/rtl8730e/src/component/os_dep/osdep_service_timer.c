/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
// #include <timers.h>
#include <osdep_service.h>
#include <stdio.h>

extern void timer_wrapper(_timerHandle timer_hdl);
struct _rtw_timer_entry {
	struct list_head list;
	struct timer_list_priv *timer;
};
static _list _rtw_timer_table;
static _mutex _rtw_timer_mutex = NULL;

_timerHandle rtw_timerCreate(const signed char *pcTimerName,
							 u32 xTimerPeriodInTicks,
							 u32 uxAutoReload,
							 void *pvTimerID,
							 TIMER_FUN pxCallbackFunction)
{
	struct timer_list_priv *timer = (struct timer_list_priv *)kmm_zalloc(sizeof(struct timer_list_priv));
	if (timer == NULL) {
		DBG_ERR("Fail to alloc priv\n");
		return NULL;
	}
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	DEBUG_SET_CALLER_ADDR(timer);
#endif
	timer->work_hdl = (struct work_s *)kmm_zalloc(sizeof(struct work_s));
	if (timer->work_hdl == NULL) {
		DBG_ERR("Fail to alloc timer->work_hdl\n");
		kmm_free(timer);
		return NULL;
	}
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	DEBUG_SET_CALLER_ADDR(timer->work_hdl);
#endif
	timer->live = 0;
	timer->timevalue = xTimerPeriodInTicks;
	timer->data = pvTimerID;
	timer->function = pxCallbackFunction;

	if(_rtw_timer_mutex == NULL) {
		unsigned int irq_flags = save_and_cli();
		if(_rtw_timer_mutex == NULL) {
			rtw_mutex_init(&_rtw_timer_mutex);
			INIT_LIST_HEAD(&_rtw_timer_table);
		}
		restore_flags(irq_flags);
	}

	struct _rtw_timer_entry *timer_entry;
	timer_entry = (struct _rtw_timer_entry *)kmm_zalloc(sizeof(struct _rtw_timer_entry));
	if (timer_entry == NULL) {
		kmm_free(timer->work_hdl);
		kmm_free(timer);
		return NULL;
	}
#ifdef CONFIG_DEBUG_MM_HEAPINFO
	DEBUG_SET_CALLER_ADDR(timer_entry);
#endif
	timer_entry->timer = timer;

	rtw_mutex_get(&_rtw_timer_mutex);
	list_add(&(timer_entry->list), &_rtw_timer_table);
	rtw_mutex_put(&_rtw_timer_mutex);

	return (_timerHandle) timer;
}

u32 rtw_timerDelete(_timerHandle xTimer,
					u32 xBlockTime)
{
	struct timer_list_priv *timer = (struct timer_list_priv *)xTimer;

	_list *plist;
	struct _rtw_timer_entry *timer_entry;

	int ret = work_cancel(LPWORK, timer->work_hdl);
	if (ret != OK && ret != -ENOENT) {
		DBG_ERR(" failed! ret = %d\n", ret);
		return _FAIL;
	}

	rtw_mutex_get(&_rtw_timer_mutex);
	plist = get_next(&_rtw_timer_table);
	while ((rtw_end_of_queue_search(&_rtw_timer_table, plist)) == _FALSE) {
		timer_entry = LIST_CONTAINOR(plist, struct _rtw_timer_entry, list);
		if (timer_entry->timer == timer) {
			list_del_init(plist);
			kmm_free(timer_entry);
			break;
		}
		plist = get_next(plist);
	}
	rtw_mutex_put(&_rtw_timer_mutex);
	if (plist == &_rtw_timer_table) {
		return _FAIL;
	}

	timer->data = NULL;
	timer->timer_hdl = NULL;
	timer->timevalue = 0;
	timer->live = 0;
	kmm_free(timer->work_hdl);
	kmm_free(timer);
	return _SUCCESS;
}

u32 rtw_timerIsTimerActive(_timerHandle xTimer)
{
	struct timer_list_priv *timer = (struct timer_list_priv *)xTimer;

	return timer->live;
}

u32  rtw_timerStop(_timerHandle xTimer,
				   u32 xBlockTime)
{
	struct timer_list_priv *timer = (struct timer_list_priv *)xTimer;

	int ret = work_cancel(LPWORK, timer->work_hdl);
	if (ret != OK && ret != -ENOENT) {
		DBG_ERR(" failed! ret = %d\n", ret);
		return _FAIL;
	}

	timer->timevalue = 0;
	timer->live = 0;
	return _SUCCESS;
}

void rtw_timer_wrapper(void *timer)
{
	_list *plist;
	struct _rtw_timer_entry *timer_entry = NULL;

	rtw_mutex_get(&_rtw_timer_mutex);
	plist = get_next(&_rtw_timer_table);
	while ((rtw_end_of_queue_search(&_rtw_timer_table, plist)) == _FALSE) {
		timer_entry = LIST_CONTAINOR(plist, struct _rtw_timer_entry, list);
		if (timer_entry->timer == timer) {
			break;
		}
		plist = get_next(plist);
	}
	rtw_mutex_put(&_rtw_timer_mutex);

	if (plist == &_rtw_timer_table) {
		return;
	}
	timer_entry->timer->live = 0;

	if (timer_entry->timer->function) {
		timer_entry->timer->function(timer);
	}
}

u32  rtw_timerChangePeriod(_timerHandle xTimer,
						   u32 xNewPeriod,
						   u32 xBlockTime)
{
	int ret;
	struct timer_list_priv *timer = (struct timer_list_priv *)xTimer;
	ret = work_queue(LPWORK, timer->work_hdl, rtw_timer_wrapper, (void *)(timer), xNewPeriod);
	if (ret == -EALREADY) {
		if (work_cancel(LPWORK, timer->work_hdl) != OK) {
			DBG_ERR("Failed!\n");
			return _FAIL;
		}
		if (work_queue(LPWORK, timer->work_hdl, rtw_timer_wrapper, (void *)(timer), xNewPeriod)) {
			DBG_ERR("Failed!\n");
			return _FAIL;
		}
	}
	timer->live = 1;

	return _SUCCESS;
}



