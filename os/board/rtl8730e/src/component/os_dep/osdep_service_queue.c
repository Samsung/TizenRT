/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
// #include <semphr.h>
#include <osdep_service.h>
#include <stdio.h>

void	rtw_init_queue(_queue	*pqueue)
{
	rtw_init_listhead(&(pqueue->queue));
	rtw_spinlock_init(&(pqueue->lock));
}

u32	  rtw_queue_empty(_queue	*pqueue)
{
	return (rtw_is_list_empty(&(pqueue->queue)));
}


u32 rtw_end_of_queue_search(_list *head, _list *plist)
{
	if (head == plist) {
		return _TRUE;
	} else {
		return _FALSE;
	}
}
int rtw_init_xqueue(_xqueue *queue, const char *name, u32 message_size, u32 number_of_messages)
{
	/* To avoid gcc warnings */
	DBG_INFO("\n");
	return 0;
}

int rtw_push_to_xqueue(_xqueue *queue, void *message, u32 timeout_ms)
{
	DBG_INFO("\n");
	return 0;
}

int rtw_pop_from_xqueue(_xqueue *queue, void *message, u32 timeout_ms)
{
	DBG_INFO("\n");
	return 0;
}

int rtw_peek_from_xqueue(_xqueue *queue, void *message, u32 timeout_ms)
{
	DBG_INFO("\n");
	return 0;
}

int rtw_deinit_xqueue(_xqueue *queue)
{
	DBG_INFO("\n");
	return 0;
}


