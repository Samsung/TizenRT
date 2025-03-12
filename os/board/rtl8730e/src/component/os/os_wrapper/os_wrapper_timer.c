/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ameba.h"
#include "os_wrapper.h"

struct os_wrapper_timer_entry {
	struct list_head list;
	struct os_wrapper_timer_list *timer;
};

static _list os_wrapper_timer_table;
bool os_wrapper_timer_table_init = 0;
static rtos_mutex_t _rtw_timer_mutex = NULL;

int rtos_timer_create_static(rtos_timer_t *pp_handle, const char *p_timer_name, uint32_t timer_id,
							uint32_t interval_ms, uint8_t reload, void (*p_timer_callback)(void *))
{
	return rtos_timer_create(pp_handle, p_timer_name, timer_id, interval_ms, reload, p_timer_callback);
}

int rtos_timer_delete_static(rtos_timer_t p_handle, uint32_t wait_ms)
{
	return rtos_timer_delete(p_handle, wait_ms);
}

void os_wrapper_timer_wrapper(void *timer)
{
	_list *plist;
	struct os_wrapper_timer_entry *timer_entry = NULL;

	rtos_mutex_take(_rtw_timer_mutex, 0xFFFFFFFF);
	plist = get_next(&os_wrapper_timer_table);
	while ((rtw_end_of_queue_search(&os_wrapper_timer_table, plist)) == FALSE) {
		timer_entry = LIST_CONTAINOR(plist, struct os_wrapper_timer_entry, list);
		if (timer_entry->timer == timer) {
			break;
		}
		plist = get_next(plist);
	}
	rtos_mutex_give(_rtw_timer_mutex);

	if (plist == &os_wrapper_timer_table) {
		dbg("find timer_entry fail\n");
		return;
	}

	if (timer_entry->timer->reload) {
		if (work_queue(LPWORK, timer_entry->timer->work_hdl, os_wrapper_timer_wrapper, (void *)(timer_entry->timer), (timer_entry->timer->timeout * TICK_PER_SEC / 1000L)) != OK) {
			dbg("work queue fail\n");
			timer_entry->timer->live = 0;
		}
	} else {
		timer_entry->timer->live = 0;
	}

	if (timer_entry->timer->function) {
		timer_entry->timer->function(timer);
	}
}

int rtos_timer_create(rtos_timer_t *pp_handle, const char *p_timer_name, uint32_t timer_id,
					  uint32_t interval_ms, uint8_t reload, void (*p_timer_callback)(void *))
{
	struct os_wrapper_timer_list *timer;
	struct os_wrapper_timer_entry *timer_entry;

	if (!pp_handle) {
		dbg("pp_handle is NULL\n");
		return FAIL;
	}

	if (*pp_handle) {
		dbg("timer already init\n");
		return FAIL;
	}

	timer = (struct os_wrapper_timer_list *)kmm_zalloc(sizeof(struct os_wrapper_timer_list));
	if (timer == NULL) {
		dbg("alloc os_wrapper_timer_list fail\n");
		return FAIL;
	}

#ifdef CONFIG_DEBUG_MM_HEAPINFO
	DEBUG_SET_CALLER_ADDR(timer);
#endif

	timer->work_hdl = (struct work_s *)kmm_zalloc(sizeof(struct work_s));
	if (timer->work_hdl == NULL) {
		dbg("alloc work_s fail\n");
		kmm_free(timer);
		return FAIL;
	}

#ifdef CONFIG_DEBUG_MM_HEAPINFO
	DEBUG_SET_CALLER_ADDR(timer->work_hdl);
#endif

	timer->timer_hdl = timer;
	memcpy(timer->timer_name, p_timer_name, TMR_NAME_SIZE);
	timer->timer_id = timer_id;
	timer->timeout = interval_ms;
	timer->reload = reload;
	timer->live = 0;
	timer->function = p_timer_callback;

	*pp_handle = timer;

	if(_rtw_timer_mutex == NULL) {
		rtos_mutex_create(&_rtw_timer_mutex);
	}
	if (!os_wrapper_timer_table_init) {
		rtos_mutex_take(_rtw_timer_mutex, 0xFFFFFFFF);
		INIT_LIST_HEAD(&os_wrapper_timer_table);
		rtos_mutex_give(_rtw_timer_mutex);
		os_wrapper_timer_table_init = 1;
	}

	timer_entry = (struct os_wrapper_timer_entry *)kmm_zalloc(sizeof(struct os_wrapper_timer_entry));
	if (timer_entry == NULL) {
		dbg("alloc os_wrapper_timer_entry fail\n");
		kmm_free(timer->work_hdl);
		kmm_free(timer);
		return FAIL;
	}

#ifdef CONFIG_DEBUG_MM_HEAPINFO
	DEBUG_SET_CALLER_ADDR(timer_entry);
#endif

	timer_entry->timer = timer;

	rtos_mutex_take(_rtw_timer_mutex, 0xFFFFFFFF);
	rtw_list_insert_head(&(timer_entry->list), &os_wrapper_timer_table);
	rtos_mutex_give(_rtw_timer_mutex);

	return SUCCESS;
}

int rtos_timer_delete(rtos_timer_t p_handle, uint32_t wait_ms)
{
	int ret;
	_list *plist;
	struct os_wrapper_timer_list *timer;
	struct os_wrapper_timer_entry *timer_entry;

	if (p_handle == NULL) {
		dbg("p_handle is NULL\n");
		return FAIL;
	}

	timer = (struct os_wrapper_timer_list *)p_handle;
	ret = work_cancel(LPWORK, timer->work_hdl);
	if (ret != OK && ret != -ENOENT) {
		dbg("work cancel fail\n");
		return FAIL;
	}
	rtos_mutex_take(_rtw_timer_mutex, 0xFFFFFFFF);
	plist = get_next(&os_wrapper_timer_table);
	while ((rtw_end_of_queue_search(&os_wrapper_timer_table, plist)) == FALSE) {
		timer_entry = LIST_CONTAINOR(plist, struct os_wrapper_timer_entry, list);
		if (timer_entry->timer == timer) {
			rtw_list_delete(plist);
			kmm_free(timer_entry);
			break;
		}
		plist = get_next(plist);
	}
	rtos_mutex_give(_rtw_timer_mutex);

	if (plist == &os_wrapper_timer_table) {
		dbg("find timer_entry fail\n");
		return FAIL;
	}

	timer->data = NULL;
	timer->function = NULL;
	timer->live = 0;
	timer->reload = 0;
	timer->timeout = 0;
	timer->timer_id = 0;
	memset(timer->timer_name, 0, TMR_NAME_SIZE);
	kmm_free(timer->work_hdl);
	timer->work_hdl = NULL;
	kmm_free(timer);
	timer = NULL;

	return SUCCESS;
}

int rtos_timer_start(rtos_timer_t p_handle, uint32_t wait_ms)
{
	int ret;
	struct os_wrapper_timer_list *timer;

	if (p_handle == NULL) {
		dbg("p_handle is NULL\n");
		return FAIL;
	}

	timer = (struct os_wrapper_timer_list *)p_handle;
	ret = work_queue(LPWORK, timer->work_hdl, os_wrapper_timer_wrapper, (void *)timer, (timer->timeout * TICK_PER_SEC / 1000L));
	if (ret == -EALREADY) {
		if (work_cancel(LPWORK, timer->work_hdl) != OK) {
			dbg("work cancel fail\n");
			return FAIL;
		}

		if (work_queue(LPWORK, timer->work_hdl, os_wrapper_timer_wrapper, (void *)timer, (timer->timeout * TICK_PER_SEC / 1000L)) != OK) {
			dbg("work queue fail\n");
			return FAIL;
		}
	} else if (ret != OK) {
		dbg("work queue fail\n");
		return FAIL;
	}

	timer->live = 1;

	return SUCCESS;
}

int rtos_timer_stop(rtos_timer_t p_handle, uint32_t wait_ms)
{
	struct os_wrapper_timer_list *timer;

	if (p_handle == NULL) {
		dbg("p_handle is NULL\n");
		return FAIL;
	}

	timer = (struct os_wrapper_timer_list *)p_handle;
	if (work_cancel(LPWORK, timer->work_hdl) != OK) {
		dbg("work cancel fail\n");
		return FAIL;
	}

	timer->live = 0;

	return SUCCESS;
}

int rtos_timer_change_period(rtos_timer_t p_handle, uint32_t interval_ms, uint32_t wait_ms)
{
	int ret;
	struct os_wrapper_timer_list *timer;

	if (p_handle == NULL) {
		dbg("p_handle is NULL\n");
		return FAIL;
	}

	timer = (struct os_wrapper_timer_list *)p_handle;
	ret = work_queue(LPWORK, timer->work_hdl, os_wrapper_timer_wrapper, (void *)timer, (interval_ms * TICK_PER_SEC / 1000L));
	if (ret == -EALREADY) {
		if (work_cancel(LPWORK, timer->work_hdl) != OK) {
			dbg("work cancel fail\n");
			return FAIL;
		}

		if (work_queue(LPWORK, timer->work_hdl, os_wrapper_timer_wrapper, (void *)timer, (interval_ms * TICK_PER_SEC / 1000L)) != OK) {
			dbg("work queue fail\n");
			return FAIL;
		}
	} else if (ret != OK) {
		dbg("work queue fail\n");
		return FAIL;
	}

	timer->timeout = interval_ms;
	timer->live = 1;

	return SUCCESS;
}

uint32_t rtos_timer_is_timer_active(rtos_timer_t p_handle)
{
	struct os_wrapper_timer_list *timer;

	if (p_handle == NULL) {
		dbg("p_handle is NULL\n");
		return 0;
	}

	timer = (struct os_wrapper_timer_list *)p_handle;

	return timer->live;
}

uint32_t rtos_timer_get_id(rtos_timer_t p_handle)
{
	struct os_wrapper_timer_list *timer;

	if (p_handle == NULL) {
		dbg("p_handle is NULL\n");
		return 0;
	}

	timer = (struct os_wrapper_timer_list *)p_handle;

	return timer->timer_id;
}

