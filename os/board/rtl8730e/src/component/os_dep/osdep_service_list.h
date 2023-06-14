/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/

#ifndef __OSDEP_SERVICE_LIST_H_
#define __OSDEP_SERVICE_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

// struct	__queue	{
// 	struct	list_head	queue;
// 	_lock			lock;
// };

// typedef struct	__queue		_queue;
// typedef struct	list_head	_list;

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

// static __inline _list *get_next(_list	*list)
// {
// 	return list->next;
// }

static __inline _list *get_prev(_list	*list)
{
	return list->prev;
}

// static __inline _list	*get_list_head(_queue	*queue)
// {
// 	return (&(queue->queue));
// }
#ifdef __cplusplus
}
#endif
#endif
