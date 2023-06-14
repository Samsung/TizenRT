/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
#include <osdep_service.h>
#include <stdio.h>

void rtw_init_listhead(_list *list)
{
	INIT_LIST_HEAD(list);
}

/*
For the following list_xxx operations,
caller must guarantee the atomic context.
Otherwise, there will be racing condition.
*/
u32 rtw_is_list_empty(_list *phead)
{
	if (list_empty(phead)) {
		return _TRUE;
	}

	return _FALSE;
}

void rtw_list_insert_head(_list *plist, _list *phead)
{
	list_add(plist, phead);
}

void rtw_list_insert_tail(_list *plist, _list *phead)
{
	list_add_tail(plist, phead);
}

/*

Caller must check if the list is empty before calling rtw_list_delete

*/
void rtw_list_delete(_list *plist)
{
	list_del_init(plist);
}

