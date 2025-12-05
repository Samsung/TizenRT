// Copyright 2022-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.



#include <common/bk_include.h>

#include "bk_sdio.h"
//#include "sdio.h"
#include "sdio_utils.h"
#include "sdio_slave_driver.h"
#include <os/mem.h>
#include "driver/sdio_types.h"


#if defined(CONFIG_SDIO_SLAVE)

#ifdef CONFIG_SDIO_DEBUG_EN
int32_t sdio_list_dump_info(sdio_node_t *head, sdio_node_t *tail, uint32_t count)
{
	sdio_node_t *cur_p = head;
	sdio_node_t *next_p = NULL;

	//TODO:log out each node info.
	for(uint32_t i = 0; i < count; i++)
	{
		if(cur_p)
		{
			SDIO_LOG_INFO("nd%d=0x%x,next=0x%x len=%d", i, cur_p, cur_p->next, cur_p->len);
			next_p = cur_p->next;
		}

		if(next_p == NULL)
			break;

		cur_p = next_p;
	}

	if(tail && (tail != cur_p))
	{
		SDIO_LOG_ERR("list err:tail=0x%x, len=%d", tail, tail->len);
	}

	return 0;
}
#endif

/**
 * @brief	  Init a sdio single link list which includes the buffers.
 *
 * This API init a sdio single link list which includes the buffers.
 *	 - Malloc buffers
 *	 - Reserved the header for SDIO
 *   - Linked the buffers with single list
 *   - 
 * @param count How many buffers will be malloced.
 * @param size  Each buffer size for data which doesn't include SDIO header,it should be 4 bytes align
 * @param head	Saved the list head
 * @param tail	Saved the list tail
 *
 * @attention
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t sdio_list_init(uint32_t count, uint32_t size, sdio_node_ptr_t *head_p, sdio_node_ptr_t *tail_p)
{
	uint32_t i = 0;
	sdio_node_t *cur_p = NULL, *next_p;
	uint32_t buf_len = 0;

	if(head_p == NULL || tail_p == NULL)
	{
		SDIO_LOG_ERR("list init");		
		return BK_ERR_SDIO_LIST_INIT;
	}

	size = ((size + 3) / 4) * 4;	//align to 4 bytes
	buf_len = sizeof(sdio_node_t) + size;
#ifdef CONFIG_SDIO_DEBUG_EN
	buf_len += SDIO_DEBUG_BUFFER_TAIL_LEN;
#endif
	//first node
	{
		cur_p = *head_p = *tail_p = (sdio_node_t *)os_malloc(buf_len);
		if(*head_p == NULL)
		{
			SDIO_LOG_ERR("mem malloc,len=%d", buf_len);		
			return BK_ERR_SDIO_NO_MEM;
		}

		cur_p->len = 0;
#ifdef CONFIG_SDIO_DEBUG_EN
		//TODO:log info
		*(uint32_t *)(((uint8_t *)cur_p+buf_len-SDIO_DEBUG_BUFFER_TAIL_LEN)) = SDIO_DEBUG_MAGIC_VALUE;
#endif
	}

	for(i = 1; i < count; i++)
	{
		next_p = (sdio_node_t *)os_malloc(buf_len);
		if(next_p == NULL)
		{
			SDIO_LOG_ERR("mem malloc,i=%d,len=%d", i, buf_len);
			return BK_ERR_SDIO_NO_MEM;
		}
		next_p->len = 0;
#ifdef CONFIG_SDIO_DEBUG_EN
		*(uint32_t *)((uint8_t *)(cur_p+buf_len-SDIO_DEBUG_BUFFER_TAIL_LEN)) = SDIO_DEBUG_MAGIC_VALUE;
#endif

		cur_p->next = next_p;
		cur_p = cur_p->next;
	}
	cur_p->next = NULL;
	*tail_p = cur_p;

#ifdef CONFIG_SDIO_DEBUG_EN
	sdio_list_dump_info(*head_p, *tail_p, count);
#endif

	return BK_OK;
}

/**
 * @brief	  Deinit a sdio single link list which includes the buffers.
 *
 * This API deinit a sdio single link list which includes the buffers.
 *	 - Free buffers
 *   - 
 * @param list_p The list will be deinited
 *
 * @attention
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t sdio_list_deinit(sdio_list_t *list_p)
{
	uint32_t count = 0;
	sdio_node_t *cur_p = NULL, *next_p;

	if(list_p)
	{
		cur_p = list_p->head;
		while(cur_p)
		{
			next_p = cur_p->next;
			os_free(cur_p);
			cur_p = next_p;

			count++;
		}

		if(count == list_p->count)
		{
			return BK_OK;
		}
	}

	return BK_FAIL;
}

static bk_err_t sdio_list_check(sdio_node_t *push_head, sdio_node_t *push_tail, uint32_t count)
{
	sdio_node_t *cur_p = push_head;
	uint32_t compute = 1;

	if(push_head == NULL)
	{
		SDIO_LOG_ERR("head null");
		return BK_ERR_SDIO_LIST;
	}

	if(push_tail && push_tail->next != NULL)
	{
		SDIO_LOG_ERR("tail invalid");
		return BK_ERR_SDIO_LIST;
	}

	while(cur_p)
	{
		if(compute == count)
		{
			if(cur_p == push_tail)
			{
				return BK_OK;
			}
			else
			{
				SDIO_LOG_ERR("list is invalid");
				return BK_ERR_SDIO_LIST;
			}	
		}

		cur_p = cur_p->next;
		compute++;
	}

	SDIO_LOG_ERR("list invalid really node cnt=%d, set count=%d", compute-1, count);
	return BK_ERR_SDIO_LIST;
}

/**
 * @brief	  Pop a node from the selected list.
 *
 * This API Pop a node from the selected list.
 *	 - Pop out a node from the selected list
 *   - Update the list info
 *   - 
 * @param list_p Selected pop node list
 * @param node_p Save the pop out node
 *
 * @attention
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t sdio_list_pop_node(sdio_list_t *list_p, sdio_node_ptr_t *node_p)
{
	bk_err_t ret = BK_OK;

	ret = sdio_list_check(list_p->head, list_p->tail, list_p->count);
	if(ret != BK_OK)
	{
		SDIO_LOG_ERR("list fail");
		*node_p = NULL;
		return BK_FAIL;
	}

	if(list_p->count)
	{
		sdio_node_ptr_t tmp_node_p = list_p->head;

		*node_p = tmp_node_p;
		if(list_p->head == list_p->tail)	//last one
		{
			list_p->head = list_p->tail = NULL;
		}
		else
		{
			list_p->head = list_p->head->next;
		}
		tmp_node_p->next = NULL;

		list_p->count--;
	}

#ifdef CONFIG_SDIO_DEBUG_EN
	sdio_list_dump_info(list_p->head, list_p->tail, list_p->count);
#endif
	return ret;
}

/**
 * @brief	  Push a source list to target list.
 *
 * This API Push a source list to target list.
 *	 - Check list is whether valid
 *	 - Link two list together
 *   - Update the list info
 *   - 
 * @param list_p Selected target list
 * @param head_p Source list header node
 * @param tail_p Source list tail node
 * @param count How many nodes of source list
  *
 * @attention
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t sdio_list_push_list(sdio_list_t *list_p, sdio_node_t *head_p, sdio_node_t *tail_p, uint32_t count)
{
	bk_err_t ret = BK_OK;

	ret = sdio_list_check(head_p, tail_p, count);
	if(ret != BK_OK)
	{
		SDIO_LOG_ERR("list fail");
		return ret;
	}

	if(list_p->head == NULL)	//ongoing is null, first in
	{
		list_p->head = head_p;
		list_p->tail = tail_p;
		list_p->count = count;
	}
	else
	{
		ret = sdio_list_check(list_p->head, 
							list_p->tail, 
							list_p->count);
		if(ret != BK_OK)
		{
			SDIO_LOG_ERR("list fail");
			return ret;
		}

		list_p->tail->next = head_p;
		list_p->tail = tail_p;
		list_p->tail->next = NULL;
		list_p->count += count;
	}

	//TODO:Just debug
	ret = sdio_list_check(list_p->head, 
						list_p->tail, 
						list_p->count);
	if(ret != BK_OK)
	{
		SDIO_LOG_ERR("list fail");
		return ret;
	}
	
	return ret;
}

sdio_node_t *sdio_list_pop_list(sdio_node_t *head, uint32_t count)
{
	return 0;
}

uint32_t sdio_list_get_node_count(sdio_list_t *list_p)
{
	return list_p->count;
}

uint32_t sdio_list_compute_total_length(sdio_list_t *list_p)
{
	sdio_node_ptr_t cur_p = list_p->head;
	uint32_t total_length = 0;

	for(uint32_t i = 0; i < list_p->count; i++)
	{
		if(cur_p)
		{
			total_length += cur_p->len;
		}
		cur_p = cur_p->next;
	}

	return total_length;
}



#endif
// EOF

