// Copyright 2022-2023 Beken
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



#ifndef _SDIO_UTIL_H_
#define _SDIO_UTIL_H_

#define SDIO_DEBUG_BUFFER_TAIL_LEN (4)
#define SDIO_DEBUG_MAGIC_VALUE (0xA0A05050)

typedef struct _sdio_node_t
{
	struct _sdio_node_t *next;
	uint32_t len;
	uint32_t priv; 
	uint32_t rsvd;
}sdio_node_t, *sdio_node_ptr_t;

typedef struct _sdio_list_t
{
	sdio_node_ptr_t head;
	sdio_node_ptr_t tail;
	uint32_t count;
}sdio_list_t;

int32_t sdio_list_dump_info(sdio_node_t *head, sdio_node_t *tail, uint32_t count);

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
bk_err_t sdio_list_init(uint32_t count, uint32_t size, sdio_node_ptr_t *head_p, sdio_node_ptr_t *tail_p);

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
bk_err_t sdio_list_deinit(sdio_list_t *list_p);

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
bk_err_t sdio_list_pop_node(sdio_list_t *list_p, sdio_node_ptr_t *node_p);

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
bk_err_t sdio_list_push_list(sdio_list_t *list_p, sdio_node_t *head_p, sdio_node_t *tail_p, uint32_t count);

sdio_node_t *sdio_list_pop_list(sdio_node_t *head, uint32_t count);

uint32_t sdio_list_get_node_count(sdio_list_t *list_p);

#endif // _SDIO_UTIL_H_
