/**
  ******************************************************************************
  * @file    rtw_queue.h
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

#ifndef __RTW_QUEUE_H_
#define __RTW_QUEUE_H_

#include "dlist.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void	            *_lock;
typedef unsigned long		_irqL;

struct	__queue	{
	struct	list_head	queue;
	_lock			lock;
};

typedef struct	__queue		_queue;
typedef struct	list_head	_list;

/*************************** List *******************************/

/**
 * @brief  This function initializes the head of the list.
 * @param[in] list: Pointer to the list to be initialized.
 * @return	  None
 */
void	rtw_init_listhead(_list *list);

/**
 * @brief  This function tests whether a list is empty.
 * @param[in] phead: Pointer to the list to test.
 * @return	  _TRUE/_FALSE
 */
uint32_t	rtw_is_list_empty(_list *phead);

/**
 * @brief  This function adds a new entry after "phead" for the list.
 * @param[in] plist: Pointer to the list to be added.
 * @param[in] phead: List head to add it after.
 * @return	  None
 */
void	rtw_list_insert_head(_list *plist, _list *phead);

/**
 * @brief  This function adds a new entry before "phead" for the list.
 * @param[in] plist: Pointer to the list to be added.
 * @param[in] phead: List head to add it before.
 * @return	  None
 */
void	rtw_list_insert_tail(_list *plist, _list *phead);

/**
 * @brief  This function deletes entry from list and reinitialize it.
 * @param[in] plist: The element to delete from the list.
 * @return	  None
 * @note	  Caller must check if the list is empty before calling rtw_list_delete
 */
void	rtw_list_delete(_list *plist);
/*************************** End List *******************************/

static __inline _list *get_next(_list	*list)
{
	return list->next;
}

static __inline _list *get_prev(_list	*list)
{
	return list->prev;
}

static __inline _list	*get_list_head(_queue	*queue)
{
	return (&(queue->queue));
}
#ifdef __cplusplus
}
#endif

/*************************** Queue *******************************/

/**
 * @brief  This function creates a new queue instance.
 * @param[in] pqueue: The handle to the newly created queue.
 * @return	 None
 */
void	rtw_init_queue(_queue	*pqueue);

/**
 * @brief  This function tests whether the queue is empty.
 * @param[in] pqueue: The handle to the queue to be tested.
 * @return	 None
 */
uint32_t	rtw_queue_empty(_queue	*pqueue);

/**
 * @brief  This function tests whether the "pelement" is at the "queue".
 * @param[in] queue: The pointer to the queue that to be tested.
 * @param[in] pelement: The element that to be tested.
 * @return	 _TRUE/_FALSE
 */
uint32_t	rtw_end_of_queue_search(_list *queue, _list *pelement);
/*************************** End Queue *******************************/

#endif //__RTW_QUEUE_H_
