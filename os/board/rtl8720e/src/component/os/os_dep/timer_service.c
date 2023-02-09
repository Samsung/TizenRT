/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
#include "osdep_service.h"

#if( configSUPPORT_STATIC_ALLOCATION == 1 )

int max_timer_num = MAX_TIMER_BUF_NUM;
struct timer_buf {
	struct list_head list;
	StaticTimer_t timerbuf;
};

struct timer_buf timer_pool[MAX_TIMER_BUF_NUM];
static struct list_head wrapper_timerbuf_list;
int timerbuf_used_num, timer_dynamic_num;
int max_timerbuf_used_num;
int timerpool_flag = 0;

static __inline__ void *get_timer_from_poll(struct list_head *phead, int *count)
{
	StaticTimer_t *timer;
	struct list_head *plist;

	if (timerpool_flag == 0) {
		return NULL;
	}

	unsigned int irq_flags = save_and_cli();

	if (list_empty(phead)) {
		restore_flags(irq_flags);
		return NULL;
	}

	plist = phead->next;

	list_del_init(plist);

	timer = (StaticTimer_t *)((unsigned int)plist + sizeof(struct list_head));

	*count = *count + 1;

	restore_flags(irq_flags);

	return (unsigned char *)timer;
}

static void release_timer_to_poll(unsigned char *buf, struct list_head *phead, int *count)
{
	struct list_head *plist;
	plist = (struct list_head *)(((unsigned int)buf) - sizeof(struct list_head));
	list_add_tail(plist, phead);

	*count = *count - 1;
}

static u8 rtw_get_timer_dynamic_flag(_timerHandle xTimer)
{
	StaticTimer_t *ptimer = (StaticTimer_t *)xTimer;
	return ptimer->ucDummy8;
}
#endif

void init_timer_pool(void)
{
#if( configSUPPORT_STATIC_ALLOCATION == 1 )
	int i;

	memset(timer_pool, '\0', max_timer_num * sizeof(struct timer_buf));
	INIT_LIST_HEAD(&wrapper_timerbuf_list);

	for (i = 0; i < max_timer_num; i++) {
		INIT_LIST_HEAD(&timer_pool[i].list);
		list_add_tail(&timer_pool[i].list, &wrapper_timerbuf_list);
	}

	timerbuf_used_num = 0;
	max_timerbuf_used_num = 0;
	timer_dynamic_num = 0;

	timerpool_flag = 1;
#endif
}

void deinit_timer_pool(void)
{
#if( configSUPPORT_STATIC_ALLOCATION == 1 )
	memset(timer_pool, '\0', max_timer_num * sizeof(struct timer_buf));

	timerbuf_used_num = 0;
	max_timerbuf_used_num = 0;
	timer_dynamic_num = 0;

	timerpool_flag = 0;
#endif
}

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

	if (timer_used_num > 0) {
		DBG_ERR("Need to delete %d timer_entry", timer_used_num);
	}


	unsigned int irq_flags = save_and_cli();

	while (rtw_end_of_queue_search(&timer_table, get_next(&timer_table)) == _FALSE) {
		plist = get_next(&timer_table);
		rtw_list_delete(plist);
	}

	restore_flags(irq_flags);

}


void timer_wrapper(_timerHandle timer_hdl)
{
	_list *plist;
	struct timer_list *timer_entry = NULL;

	DBG_TRACE("%s is called", __FUNCTION__);

	unsigned int irq_flags = save_and_cli();

	plist = get_next(&timer_table);
	while ((rtw_end_of_queue_search(&timer_table, plist)) == _FALSE) {
		timer_entry = LIST_CONTAINOR(plist, struct timer_list, list);
		if (timer_entry->timer_hdl == timer_hdl) {
			break;
		}
		plist = get_next(plist);
	}

	restore_flags(irq_flags);

	if (plist == &timer_table) {
		DBG_ERR("Fail to find the timer_entry in timer table");
	} else {
		if (timer_entry->function) {
			timer_entry->function((void *) timer_entry->data);
		}
	}
}

void init_timer(struct timer_list *timer)
{
	if (timer->function == NULL) {
		return;
	}

	if (timer->timer_hdl == NULL) {
#if ( configSUPPORT_STATIC_ALLOCATION == 1 )
		StaticTimer_t *timerbuf;

		timerbuf = (StaticTimer_t *)get_timer_from_poll(&wrapper_timerbuf_list, &timerbuf_used_num);

		if (timerbuf == NULL) {
			if (timerpool_flag) {
				timer_dynamic_num++;
				printf("static timer is not avaliable. timerbuf_used_num: %d\n", timerbuf_used_num);
			}
			goto exit1;
		} else {
			memset(timerbuf, '\0', sizeof(*timerbuf));
			timer->timer_hdl = xTimerCreateStatic(
								   (const char *)"Timer", 		// Just a text name, not used by the RTOS kernel.
								   TIMER_MAX_DELAY,		// Timer Period, not 0
								   _FALSE,		// Whether timer will auto-load themselves when expires
								   NULL,			// Uniq id used to identify which timer expire..
								   (TimerCallbackFunction_t)timer_wrapper,		// Timer callback
								   timerbuf		// The buffer that will hold the software timer structure.
							   );

		}

		if (timerbuf_used_num > max_timerbuf_used_num) {
			max_timerbuf_used_num = timerbuf_used_num;
		}

		goto exit2;
exit1:
#endif
		timer->timer_hdl = rtw_timerCreate(
							   (signed const char *)"Timer", 		// Just a text name, not used by the RTOS kernel.
							   TIMER_MAX_DELAY,		// Timer Period, not 0
							   _FALSE,		// Whether timer will auto-load themselves when expires
							   NULL,			// Uniq id used to identify which timer expire..
							   timer_wrapper		// Timer callback
						   );
#if ( configSUPPORT_STATIC_ALLOCATION == 1 )
exit2:
#endif
		if (timer->timer_hdl == NULL) {
			DBG_ERR("Fail to init timer");
		} else {
			unsigned int irq_flags = save_and_cli();
			rtw_list_insert_head(&timer->list, &timer_table);
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

void mod_timer(struct timer_list *timer, u32 delay_time_ms)
{
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
			unsigned int irq_flags = save_and_cli();
			rtw_list_insert_head(&timer->list, &timer_table);
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
	if (timer->timer_hdl != NULL){
		if (rtw_timerChangePeriod(timer->timer_hdl, rtw_ms_to_systime(delay_time_ms), TIMER_MAX_DELAY) == _FAIL) {
			DBG_ERR("Fail to set timer period");
		}
	}
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
	struct timer_list *timer_entry = NULL;

	if (timer->timer_hdl == NULL) {
		return;
	}

	unsigned int irq_flags = save_and_cli();

	plist = get_next(&timer_table);
	while ((rtw_end_of_queue_search(&timer_table, plist)) == _FALSE) {
		timer_entry = LIST_CONTAINOR(plist, struct timer_list, list);
		if (timer_entry->timer_hdl == timer->timer_hdl) {
			break;
		}
		plist = get_next(plist);
	}

	restore_flags(irq_flags);

	if (plist == &timer_table) {
		DBG_TRACE("Fail to find the timer_entry(%08X) in timer table", timer->timer_hdl);
	} else {
		rtw_timerStop(timer->timer_hdl, TIMER_MAX_DELAY);
	}
}

void  del_timer_sync(struct timer_list *timer)
{
	_list *plist;
	struct timer_list *timer_entry;

	if (timer->timer_hdl == NULL) {
		return;
	}
#if ( configSUPPORT_STATIC_ALLOCATION == 1 )
	if (rtw_get_timer_dynamic_flag(timer->timer_hdl)) {
		unsigned int irq_flags = save_and_cli();
		release_timer_to_poll((unsigned char *)timer->timer_hdl, &wrapper_timerbuf_list, &timerbuf_used_num);
		restore_flags(irq_flags);
	} else {
		if (timerpool_flag) {
			timer_dynamic_num--;
		}
	}
#endif

	unsigned int irq_flags = save_and_cli();

	plist = get_next(&timer_table);
	while ((rtw_end_of_queue_search(&timer_table, plist)) == _FALSE) {
		timer_entry = LIST_CONTAINOR(plist, struct timer_list, list);
		if (timer_entry->timer_hdl == timer->timer_hdl) {
			rtw_list_delete(plist);
			break;
		}
		plist = get_next(plist);
	}

	restore_flags(irq_flags);

	if (plist == &timer_table) {
		DBG_ERR("Fail to find the timer_entry in timer table");
	} else {
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



