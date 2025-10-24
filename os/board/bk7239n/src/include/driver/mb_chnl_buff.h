// Copyright 2020-2022 Beken
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

#ifndef _mb_chnl_buff_h_
#define _mb_chnl_buff_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <common/bk_typedef.h>
#include <common/bk_err.h>
#include <driver/mailbox_channel.h>

#if (CONFIG_CPU_CNT > 1)

#define MB_CHNL_BUFF_LEN		128

typedef union
{
	struct
	{
		mb_chnl_hdr_t	chnl_hdr;
		void *	data_buff;
		u16		data_len;
	};

	mb_chnl_cmd_t	mb_cmd;
	mb_chnl_ack_t	mb_ack;
} mb_buff_hdr_t;

/**
 * @brief	  get the tx buffer address in swap region for data copy.
 *
 * This API get the memory address to copy data into it.
 *
 * @chnl_id     mailbox logical channel ID.
 *
 * @attention   if one channel uses data copy (via swap region) for communication instead of data pointer, 
 *              it should firtly map it in chni_id_map[].
 *
 * @return
 *  - NULL: if the channel ID is not valid or not mapped in chni_id_map[].
 *  - address of tx buffer in swap region if success.
 */
void *   mb_chnl_get_tx_buff(u8 chnl_id);

/**
 * @brief	  get the rx buffer address in swap region for data copy.
 *
 * This API get the memory address to copy data from it.
 *
 * @chnl_id     mailbox logical channel ID.
 *
 * @attention   if one channel uses data copy (via swap region) for communication instead of data pointer, 
 *              it should firtly map it in chni_id_map[].
 *
 * @return
 *  - NULL: if the channel ID is not valid or not mapped in chni_id_map[].
 *  - address of rx buffer in swap region if success.
 */
void *   mb_chnl_get_rx_buff(u8 chnl_id);

/**
 * @brief	  get the size of tx/rx buffer.
 *
 * @param 
 *   - void
 *
 * @attention 
 *
 * @return
 *  - the configured tx/rx buffer size (in bytes) for every channel.
 */
u32      mb_chnl_get_buff_len(void);

#endif

#ifdef __cplusplus
}
#endif

#endif /* _mb_chnl_buff_h_ */

