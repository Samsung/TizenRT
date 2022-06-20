/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
#include "osdep_service.h"

#define TIMEOUT_LEAK_DBG	0

struct timer_entry {
	struct list_head list;
	struct timer_list *timer;
#if TIMEOUT_LEAK_DBG
	void (*traced_func)(void *);
	int timeout;
#endif
};

_list timer_table;

static int timer_used_num;
int max_timer_used_num;

void init_timer_wrapper(void)
{
	rtw_init_listhead(&timer_table);
	timer_used_num = 0;
	max_timer_used_num = 0;
}

void deinit_timer_wrapper(void)
{
	_list *plist;
	struct timer_entry *timer_entry;

	if (timer_used_num > 0) {
		DBG_ERR("Need to delete %d timer_entry", timer_used_num);
	}


	unsigned int irq_flags = save_and_cli();

	while (rtw_end_of_queue_search(&timer_table, get_next(&timer_table)) == _FALSE)	{
		plist = get_next(&timer_table);
		timer_entry = LIST_CONTAINOR(plist, struct timer_entry, list);
#if TIMEOUT_LEAK_DBG
		DBG_ERR("timeout function(0x%p) leak. timer=%p", (void *)(timer_entry->traced_func), (void *)(timer_entry->timer));
#endif
		rtw_list_delete(plist);
		rtw_mfree((u8 *) timer_entry, sizeof(struct timer_entry));
	}

	restore_flags(irq_flags);
}

#if TIMEOUT_LEAK_DBG
void write_timeout(struct timer_list *timer, u32 delay_time_ms)
{
	_list *plist;
	struct timer_entry *timer_entry;

	unsigned int irq_flags = save_and_cli();

	plist = get_next(&timer_table);
	while (rtw_end_of_queue_search(&timer_table, plist) == _FALSE)	{
		timer_entry = LIST_CONTAINOR(plist, struct timer_entry, list);
		if (timer_entry->timer == timer || (timer_entry->timer->timer_hdl == timer->timer_hdl)) {
			timer_entry->timeout = delay_time_ms;
			DBG_TRACE("[%d]: SSSet timer=%p ms=%d\n", rtw_get_current_time(), timer_entry->timer, timer_entry->timeout);
			break;
		}
		plist = get_next(plist);
	}

	restore_flags(irq_flags);
}
void dump_timer_wrapper(void)
{
	_list *plist;
	struct timer_entry *timer_entry;
	int count = 0;

	unsigned int irq_flags = save_and_cli();

	plist = get_next(&timer_table);
	while (rtw_end_of_queue_search(&timer_table, plist) == _FALSE)	{
		timer_entry = LIST_CONTAINOR(plist, struct timer_entry, list);
		DBG_ERR("[%d]:timer[%d]: timer=0x%p timeout=%d", rtw_get_current_time(), count++, (void *)(timer_entry->timer), timer_entry->timeout);
		plist = get_next(plist);
	}

	restore_flags(irq_flags);
}
#endif //TIMEOUT_LEAK_DBG

void timer_wrapper(_timerHandle timer_hdl)
{
	_list *plist;
	struct timer_entry *timer_entry = NULL;

	DBG_TRACE("%s is called", __FUNCTION__);

	unsigned int irq_flags = save_and_cli();

	plist = get_next(&timer_table);
	while ((rtw_end_of_queue_search(&timer_table, plist)) == _FALSE) {
		timer_entry = LIST_CONTAINOR(plist, struct timer_entry, list);
		if (timer_entry->timer->timer_hdl == timer_hdl) {
			break;
		}
		plist = get_next(plist);
	}

	restore_flags(irq_flags);

	if (plist == &timer_table) {
		DBG_ERR("Fail to find the timer_entry in timer table");
	} else {
#if TIMEOUT_LEAK_DBG
		timer_entry->timeout = 0;
		DBG_TRACE("[%d]: TTTTimeout timer=%p\n", rtw_get_current_time(), timer_entry->timer);
#endif
		if (timer_entry->timer->function) {
			timer_entry->timer->function((void *) timer_entry->timer->data);
		}
	}
}

void init_timer(struct timer_list *timer)
{
	struct timer_entry *timer_entry;

	if (timer->function == NULL) {
		return;
	}

	if (timer->timer_hdl == NULL) {
		timer->timer_hdl = rtw_timerCreate(
							   (signed const char *)"Timer", 		// Just a text name, not used by the RTOS kernel.
							   TIMER_MAX_DELAY,		// Timer Period, not 0
							   _FALSE,		// Whether timer will auto-load themselves when expires
							   NULL,			// Uniq id used to identify which timer expire..
							   timer_wrapper		// Timer callback
						   );

		if (timer->timer_hdl == NULL) {
			DBG_ERR("Fail to init timer");
		} else {
			timer_entry = (struct timer_entry *) rtw_zmalloc(sizeof(struct timer_entry));

			if (timer_entry == NULL) {
				DBG_ERR("Fail to alloc timer_entry");
				rtw_timerDelete(timer->timer_hdl, TIMER_MAX_DELAY);
				timer->timer_hdl = NULL;
				return;
			}

			timer_entry->timer = timer;
#if TIMEOUT_LEAK_DBG
			timer_entry->traced_func = timer->function;
#endif
			unsigned int irq_flags = save_and_cli();
			rtw_list_insert_head(&timer_entry->list, &timer_table);
			restore_flags(irq_flags);

			timer_used_num ++;
			if (timer_used_num > max_timer_used_num) {
				max_timer_used_num = timer_used_num;
			}
		}
	} else if (rtw_timerIsTimerActive(timer->timer_hdl) == _TRUE) {
		rtw_timerStop(timer->timer_hdl, TIMER_MAX_DELAY);
	}
}

//extern Rltk_wlan_t	rltk_wlan_info[];

void mod_timer(struct timer_list *timer, u32 delay_time_ms)
{
	struct timer_entry *timer_entry;

	//if(!rltk_wlan_info[0].enable){
	//DBG_TRACE("netif is DOWN");
	//return;
	//}

	if (timer->timer_hdl == NULL) {
		timer->timer_hdl = rtw_timerCreate(
							   (signed const char *)"Timer", 		// Just a text name, not used by the RTOS kernel.
							   TIMER_MAX_DELAY,		// Timer Period, not 0
							   _FALSE,		// Whether timer will auto-load themselves when expires
							   NULL,			// Uniq id used to identify which timer expire..
							   timer_wrapper		// Timer callback
						   );

		if (timer->timer_hdl == NULL) {
			DBG_ERR("Fail to init timer");
		} else {
			timer_entry = (struct timer_entry *) rtw_zmalloc(sizeof(struct timer_entry));

			if (timer_entry == NULL) {
				DBG_ERR("Fail to alloc timer_entry");
				rtw_timerDelete(timer->timer_hdl, TIMER_MAX_DELAY);
				timer->timer_hdl = NULL;
				return;
			}

			timer_entry->timer = timer;
#if TIMEOUT_LEAK_DBG
			timer_entry->traced_func = timer->function;
#endif
			unsigned int irq_flags = save_and_cli();
			rtw_list_insert_head(&timer_entry->list, &timer_table);
			restore_flags(irq_flags);

			timer_used_num ++;
			if (timer_used_num > max_timer_used_num) {
				max_timer_used_num = timer_used_num;
			}
		}
	} else if (rtw_timerIsTimerActive(timer->timer_hdl) == _TRUE) {
		rtw_timerStop(timer->timer_hdl, TIMER_MAX_DELAY);
	}

	//Set Timer period
	if (timer->timer_hdl != NULL)
		if (rtw_timerChangePeriod(timer->timer_hdl, rtw_ms_to_systime(delay_time_ms), TIMER_MAX_DELAY) == _FAIL) {
			DBG_ERR("Fail to set timer period");
		}
#if TIMEOUT_LEAK_DBG
	write_timeout(timer, delay_time_ms);
#endif
}

int timer_pending(const struct timer_list *timer)
{
	if (timer->timer_hdl != NULL) {
		return 1;
	} else {
		return 0;
	}
}

void  cancel_timer_ex(struct timer_list *timer)
{
	_list *plist;
	struct timer_entry *timer_entry = NULL;

	if (timer->timer_hdl == NULL) {
		return;
	}

	unsigned int irq_flags = save_and_cli();

	plist = get_next(&timer_table);
	while ((rtw_end_of_queue_search(&timer_table, plist)) == _FALSE) {
		timer_entry = LIST_CONTAINOR(plist, struct timer_entry, list);
		if (timer_entry->timer->timer_hdl == timer->timer_hdl) {
			break;
		}
		plist = get_next(plist);
	}

	restore_flags(irq_flags);

	if (plist == &timer_table) {
		DBG_TRACE("Fail to find the timer_entry(%08X) in timer table", timer->timer_hdl);
	} else {
		rtw_timerStop(timer->timer_hdl, TIMER_MAX_DELAY);
#if TIMEOUT_LEAK_DBG
		if (timer_entry) {
			timer_entry->timeout = 0;
		}
		DBG_TRACE("[%d]: CCCCancel timer=%p\n", rtw_get_current_time(), timer);
#endif
	}
}

void  del_timer_sync(struct timer_list *timer)
{
	_list *plist;
	struct timer_entry *timer_entry;

	if (timer->timer_hdl == NULL) {
		return;
	}

	unsigned int irq_flags = save_and_cli();

	plist = get_next(&timer_table);
	while ((rtw_end_of_queue_search(&timer_table, plist)) == _FALSE) {
		timer_entry = LIST_CONTAINOR(plist, struct timer_entry, list);
		if (timer_entry->timer->timer_hdl == timer->timer_hdl) {
			rtw_list_delete(plist);
			rtw_mfree((u8 *) timer_entry, sizeof(struct timer_entry));
			break;
		}
		plist = get_next(plist);
	}

	restore_flags(irq_flags);

	if (plist == &timer_table) {
		DBG_TRACE("Fail to find the timer_entry in timer table");
	} else {
#if TIMEOUT_LEAK_DBG
		DBG_ERR("DDDelete timer=%p\n", timer);
#endif
		rtw_timerDelete(timer->timer_hdl, TIMER_MAX_DELAY);
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
	init_timer(ptimer);
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



