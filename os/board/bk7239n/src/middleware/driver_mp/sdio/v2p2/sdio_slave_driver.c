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

#include <os/mem.h>
#include "string.h"
#include <common/bk_include.h>
#include <driver/int.h>
#include <driver/int_types.h>
#include "sys_driver.h"
#include "bk_uart.h"
#include "bk_gpio.h"
#include <driver/gpio.h>
#include "gpio_driver.h"

#if CONFIG_SDIO_GDMA_EN
#include "driver/dma.h"
#include <driver/hal/hal_dma_types.h>
#endif

#include "driver/sdio_types.h"
#include "sdio_hal.h"
#include "sdio_utils.h"
#include "sdio_slave_driver.h"
static void sdio_send_msg(sdio_msg_t *msg_p);

static sdio_driver_t s_sdio_driver;
static bool s_sdio_driver_is_init = false;
static beken_queue_t s_sdio_msg_que = NULL;
static beken_thread_t s_sdio_thread = NULL;
//volatile uint8_t g_sdio_log_level = SDIO_LOG_ERR_LEVEL;
volatile uint8_t g_sdio_log_level = SDIO_LOG_DEBUG_LEVEL;

#if CONFIG_SDIO_SLAVE_TX_MEMCPY_EN
static beken_thread_t s_sdio_tx_memcpy_thread = NULL;
static beken_semaphore_t s_sdio_memcpy_sema = NULL;

//TX
static uint32_t s_tx_pack_total_len = 0;	//memcpy many payloads to a pack
static uint32_t s_pack_tx_complete_len = 0;	//current pack has TX how many bytes
static uint32_t s_sdio_tx_bytes = 0;	//TX:how many bytes Host read
static dma_id_t s_sdio_tx_dma_chan = DMA_ID_MAX;  /**< SDIO tx dma channel */
#endif
//RX
#if CONFIG_SDIO_SLAVE_RX_MEMCPY_EN
static uint32_t s_rx_pack_total_len = 0;	//how many bytes RX from Host CMD53(write), it will be SDIO_BLK_SIZE aligned
static uint32_t s_pack_rx_left_len = 0; //how many bytes are left be RX
static uint32_t s_rx_last_len = 0;	//drive read data by DMA, set the len to DMA, after DMA read SDIO data to RAM complete, uses to compute whether read all pack finish
static sdio_node_ptr_t s_rx_last_payload = NULL;	//rx data to which payload buffer
static uint32_t s_rx_payload_copied_len = 0;	//drive read data by DMA, set the len to DMA, after DMA read SDIO data to RAM complete, uses to compute whether read all pack finish
static uint32_t s_sdio_rx_bytes = 0;	//RX:how many bytes Host write at current read transaction
static dma_id_t s_sdio_rx_dma_chan = DMA_ID_MAX;  /**< SDIO rx dma channel */

#if (0 == CONFIG_SDIO_4LINES_EN)  // enable RX_WITH_BLK only in 1line mode
#define CONFIG_SDIO_SLAVE_RX_WITH_BLK  1
#endif

#if CONFIG_SDIO_SLAVE_RX_WITH_BLK
static uint32_t s_rx_blk_cnt = 0;	//the block number has been received by slave
#endif
#endif
#define SDIO_RETURN_ON_NOT_INIT() do {\
		if (!s_sdio_driver_is_init) {\
			SDIO_LOG_ERR("driver not init");\
			return BK_ERR_SDIO_NOT_INIT;\
		}\
	} while(0)
#define SDIO_RETURN_CHAN_ID(chan_id) do {\
		if (chan_id >= SDIO_CHAN_MAX_CNT) {\
			SDIO_LOG_ERR("chan id %d", chan_id);\
			return BK_ERR_SDIO_CHAN_ID_INVALID;\
		}\
	} while(0)
#define SDIO_RETURN_CHAN_DIRECT(direct) do {\
		if (direct > SDIO_CHAN_RX) {\
			SDIO_LOG_ERR("direct %d", direct);\
			return BK_ERR_SDIO_DIRECT_INVALID;\
		}\
	} while(0)

#if CONFIG_SDIO_GDMA_EN
static bk_err_t sdio_tx_dma_set_src_buf(uint8_t *psrc, uint32_t count);
static bk_err_t sdio_rx_dma_set_dst_buf(uint8_t *pdst, uint32_t count);
static inline uint32_t sdio_read_fifo_data(void);

//TODO:return value should be check
static void request_cpu_copy_sdio_data_to_payloads(void)
{
	uint32_t i = 0;
	uint32_t tmp_data = 0;
	sdio_sw_pbd_t sw_pbd = {0};
	uint32_t payload_size = 0;
	sdio_node_ptr_t payload_ptr = NULL;
	uint8_t *payload_buf_p = NULL;
	sdio_chan_id_t chan_id = 0;
	uint32_t payload_len = 0;

	while(s_pack_rx_left_len > 0)
	{
		//read payload header
		tmp_data = sdio_read_fifo_data();
		s_pack_rx_left_len -=4;
		//packets decode to all payloads complete
		if((tmp_data == SDIO_PACK_END_MAGIC) || (tmp_data == 0))	//end flag
		{
			//s_pack_rx_left_len -= 4;	//has been read out by previous "get sw pbd"
			//read out all data
			for(i = 0; i < s_pack_rx_left_len; i+=4)
			{
				tmp_data = sdio_read_fifo_data();
				SDIO_LOG_DEBUG("fifo tmp_data=%d", tmp_data);
			}
			SDIO_LOG_DEBUG("pack read complete:fill size=%d", s_pack_rx_left_len);
			s_rx_pack_total_len = 0;
			s_pack_rx_left_len = 0;
			s_rx_last_len = 0;
			s_rx_last_payload = NULL;
			//notify APP
			sdio_msg_t msg;
			msg.id = SDIO_READ_PACK_FINISH;
			sdio_send_msg(&msg);
			return;
		}
		else
		{
			//parse sw pbd
			os_memcpy(&sw_pbd, &tmp_data, sizeof(sdio_sw_pbd_t));
			chan_id = sw_pbd.type;
			payload_len = SDIO_ALIGN_LENGTH(sw_pbd.len);
			SDIO_LOG_ERR("PARSE:tmp_data=0x%x,chan_id=%d,len=%d,s_pack_rx_left_len=%d", tmp_data, chan_id, payload_len, s_pack_rx_left_len);
			BK_ASSERT(chan_id < SDIO_CHAN_MAX_CNT);
			//get payload buffer
			sdio_chan_pop_ongoing_node(chan_id, SDIO_CHAN_RX, &payload_ptr);
			BK_ASSERT(payload_ptr); //hasn't payload buffer
			payload_size = s_sdio_driver.chan[chan_id].chan_buf[SDIO_CHAN_RX].buf_size;
			BK_ASSERT(sw_pbd.len <= payload_size);
			BK_ASSERT(sw_pbd.len <= s_pack_rx_left_len);
			payload_buf_p = (uint8_t *)SDIO_NODE_PTR_TO_DATA_PTR(payload_ptr);
			//sdio_sw_pbd_t has been read out by SDIO,so restore it to payload buffer
			*(sdio_sw_pbd_t *)payload_buf_p = sw_pbd;
			payload_buf_p += sizeof(sdio_sw_pbd_t);	//copy header to payload buffer
			for(i = 0; i < payload_len; i+=4)
			{
				*(uint32_t *)payload_buf_p	= sdio_read_fifo_data();
				SDIO_LOG_ERR("%s line %d: i = %d ", __func__, __LINE__, i);
				payload_buf_p +=4;
			}
			s_pack_rx_left_len -= payload_len;
			//push to finish list
			sdio_chan_push_finish_list(chan_id, SDIO_CHAN_RX, payload_ptr, payload_ptr, 1);
		}
	}
}

__attribute__((section(".iram_sec_code"))) static void request_dma_copy_sdio_data_to_payloads(void)
{
	uint32_t wr_index = 0;
#if CONFIG_SDIO_SLAVE_RX_WITH_BLK
		wr_index = s_sdio_driver.rx_pack.wr_index % SDIO_RX_MEMCPY_PACK_CNT;
		sdio_rx_dma_set_dst_buf((uint8_t *)s_sdio_driver.rx_pack.start_addr[wr_index] + s_rx_blk_cnt * SDIO_BLOCK_SIZE, SDIO_BLOCK_SIZE);
		bk_dma_start(s_sdio_rx_dma_chan);
#else
		if(s_pack_rx_left_len >= SDIO_RX_MEMCPY_PACK_SIZE)
			s_rx_last_len = SDIO_RX_MEMCPY_PACK_SIZE;
		else
			s_rx_last_len = s_pack_rx_left_len;

		BK_ASSERT(s_rx_last_len);
		wr_index = s_sdio_driver.rx_pack.wr_index % SDIO_RX_MEMCPY_PACK_CNT;
		sdio_rx_dma_set_dst_buf((uint8_t *)s_sdio_driver.rx_pack.start_addr[wr_index], s_rx_last_len);
		bk_dma_start(s_sdio_rx_dma_chan);
		s_sdio_driver.rx_pack.wr_index++;
#endif
}

//TODO:return value should be check
__attribute__((section(".iram_sec_code"))) static void request_dma_copy_packet_data_to_sdio(void)
{
	uint32_t rd_index = s_sdio_driver.pack.rd_index % SDIO_MEMCPY_PACK_CNT;
	uint8_t *src_addr = (uint8_t *)s_sdio_driver.pack.start_addr[rd_index];
	uint32_t count = SDIO_BLOCK_SIZE;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	//Avoid doesn't request write, but host start a CMD53 to read data.
	if(s_sdio_driver.pack.rd_index == s_sdio_driver.pack.wr_index)
	{
		return;
	}
	//avoid host doesn't start CMD52, but host CMD53 read twice.
	if(s_tx_pack_total_len == 0)
	{
		return;
	}

	//first?
	if((s_pack_tx_complete_len == 0) &&
		(s_tx_pack_total_len > SDIO_BLOCK_SIZE))
	{
		count = SDIO_DMA_SPLIT_FIRST_SIZE;
	}
	count = SDIO_BYTES_TO_BLOCKS_CNT(s_tx_pack_total_len)*SDIO_BLOCK_SIZE;

	//check it before start DMA
	if(s_pack_tx_complete_len + count > SDIO_BYTES_TO_BLOCKS_CNT(s_tx_pack_total_len)*SDIO_BLOCK_SIZE)
	{
		SDIO_LOG_ERR("count=%d,complete_len=%d,total_len=%d", count, s_pack_tx_complete_len, s_tx_pack_total_len);
		return;
	}
	sdio_tx_dma_set_src_buf(src_addr + s_pack_tx_complete_len, count);
	bk_dma_start(s_sdio_tx_dma_chan);
	s_pack_tx_complete_len += count;
	SDIO_LOG_DEBUG("count=%d,complete_len=%d,total_len=%d", count, s_pack_tx_complete_len, s_tx_pack_total_len);
	//BK_ASSERT(s_pack_tx_complete_len <= SDIO_BYTES_TO_BLOCKS_CNT(s_tx_pack_total_len)*SDIO_BLOCK_SIZE);
}

static bk_err_t sdio_tx_dma_set_src_buf(uint8_t *psrc, uint32_t count)
{
	bk_dma_set_transfer_len(s_sdio_tx_dma_chan, count);
	return bk_dma_set_src_addr(s_sdio_tx_dma_chan, (uint32_t)psrc, (uint32_t)(psrc+count));
}

static bk_err_t sdio_dma_tx_init(void)
{
	dma_config_t dma_config = {0};
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	os_memset(&dma_config, 0, sizeof(dma_config_t));
	dma_config.mode = DMA_WORK_MODE_SINGLE;
	dma_config.chan_prio = 0;
	dma_config.src.dev = DMA_DEV_DTCM;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.dst.dev = DMA_DEV_SDIO;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
	dma_config.dst.addr_inc_en = DMA_ADDR_INC_DISABLE;
#if CONFIG_SOC_BK7236XX || (CONFIG_SOC_BK7239XX) || (CONFIG_SOC_BK7286XX)
	dma_config.dst.start_addr = (uint32_t) SDIO_REG0XF_ADDR;
	dma_config.dst.end_addr = (uint32_t) SDIO_REG0XF_ADDR;
#endif
	if (s_sdio_tx_dma_chan == DMA_ID_MAX)
		s_sdio_tx_dma_chan = bk_dma_alloc(DMA_DEV_SDIO);
	SDIO_LOG_INFO("s_sdio_tx_dma_chan = %d.\n", s_sdio_tx_dma_chan);
	BK_LOG_ON_ERR(bk_dma_init(s_sdio_tx_dma_chan, &dma_config));
#if (CONFIG_SPE)
	BK_LOG_ON_ERR(bk_dma_set_dest_sec_attr(s_sdio_tx_dma_chan, DMA_ATTR_SEC));
	BK_LOG_ON_ERR(bk_dma_set_src_sec_attr(s_sdio_tx_dma_chan, DMA_ATTR_SEC));
#endif
	SDIO_LOG_DEBUG_FUNCTION_EXIT();
	return BK_OK;
}
static inline uint32_t sdio_read_fifo_data(void)
{
	#define READ_FIFO_TIMEOUT (1000)	//us
	extern void delay_us(UINT32 us);
	uint32_t j = 0;
	while(sdio_hal_get_read_ready() == 0)
	{
		j++;
		delay_us(1);
		if(j > READ_FIFO_TIMEOUT)
			//break;    //in case of fifo ready status is not triggered
			BK_ASSERT(0);
	}
	return sdio_hal_slave_read_data();
}
/*
 * Until now, we meet an un-know reason issue about CPU and DMA read SDIO data.
 * If CPU read first word from SDIO FIFO,parse the payload and lets DMA read one payload;
 * after the first payload RX complete, CPU try to read the second payload header from SDIO FIFO,
 * but the SDIO FIFO ready signal is always 0, it means CPU can't read data.
 */
static void request_cpu_and_dma_copy_sdio_data_to_payloads(void)
{
	sdio_sw_pbd_t sw_pbd = {0};
	uint32_t tmp_data = 0;
	uint32_t payload_size = 0;
	sdio_node_ptr_t payload_ptr = NULL;
	uint8_t *payload_buf_p = NULL;
	sdio_chan_id_t chan_id = 0;
	uint32_t payload_len = 0;
	uint32_t i = 0;
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	//read pack complete
	if(s_pack_rx_left_len == 0)
	{
		s_rx_pack_total_len = 0;
		//s_pack_rx_left_len = 0;
		s_rx_last_len = 0;
		s_rx_last_payload = NULL;
		//notify APP
		sdio_msg_t msg;
		msg.id = SDIO_READ_PACK_FINISH;
		sdio_send_msg(&msg);
		return;
	}
	bk_dma_stop(s_sdio_rx_dma_chan);
	//get sw pbd
	tmp_data = sdio_read_fifo_data();
	s_pack_rx_left_len -= 4;
	if((tmp_data == SDIO_PACK_END_MAGIC) || (tmp_data == 0))	//end flag
	{
		BK_ASSERT(bk_dma_get_remain_len(s_sdio_rx_dma_chan) == 0);
		//s_pack_rx_left_len -= 4;	//has been read out by previous "get sw pbd"
		//read out all data
		for(i = 0; i < s_pack_rx_left_len; i+=4)
		{
			tmp_data = sdio_read_fifo_data();
			SDIO_LOG_DEBUG("fifo tmp_data=%d", tmp_data);
		}
		SDIO_LOG_DEBUG("pack read complete:fill size=%d", s_pack_rx_left_len);
		s_rx_pack_total_len = 0;
		s_pack_rx_left_len = 0;
		s_rx_last_len = 0;
		s_rx_last_payload = NULL;
		//notify APP
		sdio_msg_t msg;
		msg.id = SDIO_READ_PACK_FINISH;
		sdio_send_msg(&msg);
		return;
	}
	//parse sw pbd
	os_memcpy(&sw_pbd, &tmp_data, sizeof(sdio_sw_pbd_t));
	chan_id = sw_pbd.type;
	payload_len = SDIO_ALIGN_LENGTH(sw_pbd.len);
	SDIO_LOG_ERR("PARSE:tmp_data=0x%x,chan_id=%d,len=%d,s_pack_rx_left_len=%d", tmp_data, chan_id, payload_len, s_pack_rx_left_len);
	BK_ASSERT(chan_id < SDIO_CHAN_MAX_CNT);
	//get payload buffer
	sdio_chan_pop_ongoing_node(chan_id, SDIO_CHAN_RX, &payload_ptr);
	BK_ASSERT(payload_ptr);	//hasn't payload buffer
	payload_size = s_sdio_driver.chan[chan_id].chan_buf[SDIO_CHAN_RX].buf_size;
	BK_ASSERT(sw_pbd.len <= payload_size);
	payload_buf_p = (uint8_t *)SDIO_NODE_PTR_TO_DATA_PTR(payload_ptr);
	//sdio_sw_pbd_t has been read out by SDIO,so restore it to payload buffer
	*(sdio_sw_pbd_t *)payload_buf_p = sw_pbd;
	s_rx_last_len = payload_len - sizeof(sdio_sw_pbd_t);
	s_rx_last_payload = payload_ptr;
	sdio_rx_dma_set_dst_buf(payload_buf_p + sizeof(sdio_sw_pbd_t), s_rx_last_len);
	bk_dma_start(s_sdio_rx_dma_chan);
}
#if CONFIG_SDIO_SLAVE_RX_MEMCPY_EN
/*
 * *****************payload format**********************
 * |------------------------------------------------- pack buff1 -------------------------------------------|------------------------------------------------- pack buff2 --------------------------------------|
 * | payload-head(sdio_sw_pbd_t) payload-data | payload-head(sdio_sw_pbd_t) payload-data | payload-head(sdio_sw_pbd_t) payload-data | payload-head(sdio_sw_pbd_t) payload-data |
 * |
 */
__attribute__((section(".iram_sec_code"))) void sdio_slave_dma_rx_finish(dma_id_t dma_id)
{
	uint32_t rd_index = 0;
	uint8_t *tmp_p = NULL;
	uint32_t tmp_data = 0;
	uint32_t left_len = s_rx_last_len;
	sdio_sw_pbd_t sw_pbd = {0};
	sdio_node_ptr_t payload_ptr = NULL;
	uint32_t payload_size = 0;
	uint8_t *payload_buf_p = NULL;
	sdio_chan_id_t chan_id = 0;
	uint32_t payload_len = 0;
#if CONFIG_SDIO_SLAVE_RX_WITH_BLK
	sdio_hal_set_dat_s_rd_mul_blk(1);   //release data0 busy status, and output high
	s_rx_blk_cnt++;
	s_rx_last_len = s_rx_blk_cnt * SDIO_BLOCK_SIZE;
	left_len = s_rx_last_len;
	if((s_rx_last_len >= s_pack_rx_left_len) || (s_rx_last_len >= SDIO_RX_MEMCPY_PACK_SIZE))
	{
#endif    //end of CONFIG_SDIO_SLAVE_RX_WITH_BLK
		BK_ASSERT(s_pack_rx_left_len >= s_rx_last_len);
		//fill temp packet data complete by DMA, get packet buffer pointer
		rd_index = s_sdio_driver.rx_pack.rd_index % SDIO_RX_MEMCPY_PACK_CNT;
		tmp_p = (uint8_t *)s_sdio_driver.rx_pack.start_addr[rd_index];
		s_sdio_driver.rx_pack.rd_index++;	//switch to next packet buffer
		s_pack_rx_left_len -= s_rx_last_len;
		s_rx_last_len = 0;
#if CONFIG_SDIO_SLAVE_RX_WITH_BLK
		s_rx_blk_cnt = 0;
		s_sdio_driver.rx_pack.wr_index++;
#endif    //end of CONFIG_SDIO_SLAVE_RX_WITH_BLK
		//let DMA start at once
		//request dma start to read data to another buffer.(PING-PONG)
		if(s_pack_rx_left_len)
		{
			request_dma_copy_sdio_data_to_payloads();
		}
		//unpack sdio temp packet buffer data to payload
		while(left_len > 0)
		{
			//last payload is over-ride twice buffer
			if(s_rx_payload_copied_len)
			{
				if(s_rx_last_payload)	//last payload doesn't fill data complete
				{
					uint32_t tmp_left_len = SDIO_ALIGN_LENGTH(SDIO_GET_PAYLOAD_LEN(s_rx_last_payload)) - s_rx_payload_copied_len;
					SDIO_LOG_INFO("payload over-ride buf:chan_id=%d,len=%d,s_pack_rx_left_len=%d,tmp_left_len=%d", SDIO_NODE_PAYLOAD_CHAN_ID(s_rx_last_payload), s_rx_payload_copied_len, tmp_left_len);
					os_memcpy((uint8_t *)SDIO_NODE_PTR_TO_DATA_PTR(s_rx_last_payload) + s_rx_payload_copied_len, tmp_p, tmp_left_len);
					tmp_p += tmp_left_len;
					BK_ASSERT(left_len >= tmp_left_len);
					left_len -= tmp_left_len;
					s_rx_payload_copied_len = 0;
					sdio_chan_push_finish_list(SDIO_NODE_PAYLOAD_CHAN_ID(s_rx_last_payload), SDIO_CHAN_RX, s_rx_last_payload, s_rx_last_payload, 1);
					s_rx_last_payload = NULL;
				}
				else	//last time parsed the RX data hasn't payload buffer to save it, and the RX payload is cross two packet buffer
				{
					//jump to next valid payload
					BK_ASSERT(left_len >= s_rx_payload_copied_len);
					tmp_p += s_rx_payload_copied_len;
					left_len -= s_rx_payload_copied_len;
					s_rx_payload_copied_len = 0;
				}
			}
			if(left_len < sizeof(sdio_sw_pbd_t))
			{
				s_rx_pack_total_len = 0;
				s_pack_rx_left_len = 0;
				s_rx_last_len = 0;
				s_rx_last_payload = NULL;
				//notify APP
				sdio_msg_t msg;
				msg.id = SDIO_READ_PACK_FINISH;
				sdio_send_msg(&msg);
				return;
			}
			//read payload header
			tmp_data = *(uint32_t *)tmp_p;
			tmp_p += sizeof(sdio_sw_pbd_t);
			left_len -= sizeof(sdio_sw_pbd_t);
			//unpack packet to payloads complete
			if((tmp_data == SDIO_PACK_END_MAGIC) || (tmp_data == 0))	//end flag
			{
				s_rx_pack_total_len = 0;
				s_pack_rx_left_len = 0;
				s_rx_last_len = 0;
				s_rx_last_payload = NULL;
				//notify APP
				sdio_msg_t msg;
				msg.id = SDIO_READ_PACK_FINISH;
				sdio_send_msg(&msg);
				return;
			}
			else
			{
				//parse sw pbd
				os_memcpy(&sw_pbd, &tmp_data, sizeof(sdio_sw_pbd_t));
				chan_id = sw_pbd.type;
				payload_len = SDIO_ALIGN_LENGTH(sw_pbd.len) - sizeof(sdio_sw_pbd_t);	//head has been read out
				SDIO_LOG_DEBUG("PARSE:tmp_data=0x%x,chan_id=%d,len=%d,s_pack_rx_left_len=%d", tmp_data, chan_id, payload_len, s_pack_rx_left_len);
				BK_ASSERT(chan_id < SDIO_CHAN_MAX_CNT);
				//get payload buffer
				if(sdio_chan_pop_ongoing_node(chan_id, SDIO_CHAN_RX, &payload_ptr) == BK_OK)
				{
					BK_ASSERT(payload_ptr); //hasn't payload buffer
					payload_size = s_sdio_driver.chan[chan_id].chan_buf[SDIO_CHAN_RX].buf_size - sizeof(sdio_sw_pbd_t) - sizeof(sdio_node_t);
					BK_ASSERT(payload_len <= payload_size);
					//sdio_sw_pbd_t has been read out by SDIO,so restore it to payload buffer
					payload_buf_p = (uint8_t *)SDIO_NODE_PTR_TO_DATA_PTR(payload_ptr);
					*(sdio_sw_pbd_t *)payload_buf_p = sw_pbd;
					payload_buf_p += sizeof(sdio_sw_pbd_t); //copy header to payload buffer
					if(payload_len <= left_len)
					{
						os_memcpy(payload_buf_p, tmp_p, payload_len);
						tmp_p += payload_len;
						left_len -= payload_len;
						//push to finish list
						sdio_chan_push_finish_list(chan_id, SDIO_CHAN_RX, payload_ptr, payload_ptr, 1);
					}
					else	//the left_len isn't complete for current payload
					{
						s_rx_payload_copied_len += sizeof(sdio_sw_pbd_t);
						s_rx_last_payload = payload_ptr;
						if (left_len)
						{
							os_memcpy(payload_buf_p, tmp_p, left_len);
							tmp_p += left_len;
						}
						//records current payload ptr, and has copied how many bytes
						s_rx_payload_copied_len += left_len;
						left_len = 0;	//current RX packet unpack complete
					}
				}
				else	//hasn't RX payload buffer
				{
					SDIO_LOG_WARNING("chan_id=%d:hasn't RX %d bytes payload buffer", chan_id, sw_pbd.len);
					if(payload_len <= left_len)	//jump to next payload
					{
						left_len -= payload_len;
						tmp_p += payload_len;
					}
					else	//the left_len isn't complete for current payload, left len jump, and next buffer jump "payload_len - left_len"
					{
						//NOTES:now s_rx_payload_copied_len means next payload jump how many bytes, not mean has copied how many bytes
						s_rx_payload_copied_len = payload_len - left_len;
						tmp_p += left_len;
						left_len = 0;
					}
					s_rx_last_payload = NULL;
				}
			}
		}
		if(s_pack_rx_left_len == 0)
		{
			s_rx_pack_total_len = 0;
			s_rx_last_payload = NULL;
			//notify APP
			sdio_msg_t msg;
			msg.id = SDIO_READ_PACK_FINISH;
			sdio_send_msg(&msg);
			return;
		}
#if CONFIG_SDIO_SLAVE_RX_WITH_BLK
	} else {
		request_dma_copy_sdio_data_to_payloads();
	}
#endif    //end of CONFIG_SDIO_SLAVE_RX_WITH_BLK
}
#else
void sdio_slave_dma_rx_finish(dma_id_t dma_id)
{
	BK_ASSERT(s_pack_rx_left_len >= s_rx_last_len);
	s_pack_rx_left_len -= s_rx_last_len;
	s_rx_last_len = 0;
	//push to finish list
	sdio_chan_id_t chan_id = SDIO_NODE_PAYLOAD_CHAN_ID(s_rx_last_payload);
	sdio_chan_push_finish_list(chan_id, SDIO_CHAN_RX, s_rx_last_payload, s_rx_last_payload, 1);
	//request next payload
	request_cpu_and_dma_copy_sdio_data_to_payloads();
}
#endif
static bk_err_t sdio_rx_dma_set_dst_buf(uint8_t *pdst, uint32_t count)
{
	SDIO_LOG_DEBUG("RX DMA:pdst=0x%x, count=%d", pdst, count);
	bk_dma_set_transfer_len(s_sdio_rx_dma_chan, count);
	return bk_dma_set_dest_addr(s_sdio_rx_dma_chan, (uint32_t)pdst, (uint32_t)(pdst+count));
}
static bk_err_t sdio_dma_rx_init()
{
	dma_config_t dma_config = {0};
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	os_memset(&dma_config, 0, sizeof(dma_config_t));
	dma_config.mode = DMA_WORK_MODE_SINGLE;
	dma_config.chan_prio = 0;
	dma_config.src.dev = DMA_DEV_SDIO_RX;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.src.addr_inc_en = DMA_ADDR_INC_DISABLE;
#if CONFIG_SOC_BK7236XX || (CONFIG_SOC_BK7239XX)|| (CONFIG_SOC_BK7286XX)
	dma_config.src.start_addr = (uint32) SDIO_REG0X10_ADDR;
	dma_config.src.end_addr =  (uint32) SDIO_REG0X10_ADDR;
#endif
	dma_config.dst.dev = DMA_DEV_DTCM;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
	dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;

	if (s_sdio_rx_dma_chan == DMA_ID_MAX)
		s_sdio_rx_dma_chan = bk_dma_alloc(DMA_DEV_SDIO_RX);
	SDIO_LOG_INFO("s_sdio_rx_dma_chan = %d.\n", s_sdio_rx_dma_chan);
	BK_LOG_ON_ERR(bk_dma_init(s_sdio_rx_dma_chan, &dma_config));
	//BK_LOG_ON_ERR(bk_dma_set_transfer_len(s_sdio_rx_dma_chan, count));
	BK_LOG_ON_ERR(bk_dma_register_isr(s_sdio_rx_dma_chan, NULL, sdio_slave_dma_rx_finish));
	BK_LOG_ON_ERR(bk_dma_enable_finish_interrupt(s_sdio_rx_dma_chan));
#if (CONFIG_SPE)
	BK_LOG_ON_ERR(bk_dma_set_dest_sec_attr(s_sdio_rx_dma_chan, DMA_ATTR_SEC));
	BK_LOG_ON_ERR(bk_dma_set_src_sec_attr(s_sdio_rx_dma_chan, DMA_ATTR_SEC));
#endif
	SDIO_LOG_DEBUG_FUNCTION_EXIT();
	return BK_OK;
}
/* It doesn't config the SRC/DST address in APP's buffer */
static bk_err_t sdio_dma_init()
{
	bk_err_t ret = BK_OK;
	//SDIO DMA TX init
	sdio_dma_tx_init();
	//SDIO DMA RX init
	sdio_dma_rx_init();
	return ret;
}
static bk_err_t sdio_dma_deinit()
{
	//TODO:
	bk_err_t ret = BK_OK;
	bk_dma_deinit(s_sdio_tx_dma_chan);
	bk_dma_deinit(s_sdio_rx_dma_chan);
	return ret;
}
#endif
#if CONFIG_GPIO_NOTIFY_TRANSACTION_EN
static uint8_t gpio_to_host_num = GPIO_SDIO_NOTIFY_HOST, gpio_from_host_num = GPIO_SDIO_HOST_WAKEUP_SLAVE;
static volatile uint32_t allow_sleep = 1;	//default:host whether allow ctrl sleep
static uint64_t last_rx_data_time = 0;
static wk_slp_fail_callback wk_host_fail_cb;
static beken_semaphore_t s_sdio_host_ack;
void register_wake_host_fail_func(wk_slp_fail_callback cb)
{
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	wk_host_fail_cb = cb;
	SDIO_LOG_DEBUG_FUNCTION_EXIT();
}
static void sdio_wake_host_fail(void)		//do nothing until now
{
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	if(wk_host_fail_cb)
		wk_host_fail_cb();
	SDIO_LOG_DEBUG_FUNCTION_EXIT();
}
static bk_err_t sdio_slave_tx_recv_host_ack_notification(void)
{
	bk_err_t ret = BK_OK;
	ret = rtos_set_semaphore(&s_sdio_host_ack);
	if(ret != BK_OK)
	{
		SDIO_LOG_ERR("set sema");
		return ret;
	}
	return ret;
}
/**
 * @brief	Pullup GPIO to notify host, slave will send data.
 *
 * This API Pullup GPIO to notify host, slave will send data.
 *	 - Pullup GPIO to notify host
 *
 * @attention 1. This API should be called after SDIO init.
 *
 * @return NONE
 */

#define REG_GPIO_P18                      *((volatile unsigned long *) (0x44830000+0x52*4))
static inline void sdio_slave_set_notify_host(void)
{
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	if(WAKEUP_LEVEL)
		bk_gpio_set_output_high(gpio_to_host_num);
	else
		bk_gpio_set_output_low(gpio_to_host_num);

	SDIO_LOG_DEBUG_FUNCTION_EXIT();
}
/**
 * @brief	Pulldown GPIO to notify host, slave no needs to send data.
 *
 * This API Pulldown GPIO to notify host, slave no needs to send data.
 *	 - Pullup GPIO to clear notify host
 *
 * @attention 1. This API should be called after SDIO init.
 *
 * @return NONE
 */
static inline void sdio_slave_clear_notify_host(void)
{
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	//clear wake host
	if(WAKEUP_LEVEL)
		bk_gpio_set_output_low(gpio_to_host_num);
	else
		bk_gpio_set_output_high(gpio_to_host_num);

	SDIO_LOG_DEBUG_FUNCTION_EXIT();
}
static void sdio_gpio_notify_to_host_init(uint8_t gpio_num)
{
	int ret = 0;
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	if (s_sdio_host_ack == NULL) {
		ret = rtos_init_semaphore(&(s_sdio_host_ack), 1);
		BK_ASSERT(kNoErr == ret);
	}
	gpio_to_host_num = gpio_num;
	gpio_dev_unprotect_unmap(gpio_to_host_num);
	bk_gpio_enable_output(gpio_to_host_num);	//output
	bk_gpio_set_output_low(gpio_to_host_num);	//to low, means allow peer side enter sleep.
	SDIO_LOG_DEBUG_FUNCTION_EXIT();
}
static void sdio_gpio_notify_to_host_deinit(uint8_t gpio_num)
{
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	gpio_dev_unmap(gpio_num);
	rtos_deinit_semaphore(&(s_sdio_host_ack));
	SDIO_LOG_DEBUG_FUNCTION_EXIT();
}
static void sdio_gpio_notify_from_host_init(uint8_t gpio_num)
{
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	gpio_from_host_num = gpio_num;
	bk_gpio_enable_input(gpio_from_host_num);
	bk_gpio_pull_down(gpio_from_host_num);
	//maybe host pull up the gpio before controller finish bootup.
	if(get_controller_wk_slp_status())
	{
		bk_gpio_enable_interrupt(gpio_from_host_num);	//default check high level
	}
	else
	{
		bk_gpio_disable_interrupt(gpio_from_host_num);	//maybe host pull up the gpio before controller finish bootup.
	}
	SDIO_LOG_DEBUG_FUNCTION_EXIT();
}
static void sdio_gpio_notify_from_host_deinit(uint8_t gpio_num)
{
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	gpio_dev_unmap(gpio_num);
	bk_gpio_disable_interrupt(gpio_num);	//default check high level
	bk_gpio_register_isr_ex(gpio_num, (gpio_isr_t)NULL, NULL);
	SDIO_LOG_DEBUG_FUNCTION_EXIT();
}
/**
 * @brief	Just update software allow sleep status, used to check gpio irq is
 *          whether sync with software status.
 *
 * This API ....
 *	 - 1:means allow sleep, 0 means host vote bk7256 don't go to sleep.
 *
 * @attention 1. This API should be called after SDIO init.
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
static void set_controller_wk_slp_status(uint32_t allow_slp)
{
	SDIO_LOG_DEBUG("v=%d", allow_slp);
	allow_sleep = allow_slp;
}
/**
 * @brief	Get host whether allow slave goto sleep
 *
 * This API ....
 *
 * @attention 1. This API should be called after SDIO init.
 *
 * @return
 *	  - 1:means allow sleep, 0 means host vote bk7256 don't go to sleep.
 *	  -
 */
uint32_t get_controller_wk_slp_status(void)
{
	uint32_t cur_sts = bk_gpio_get_input(gpio_from_host_num);
	SDIO_LOG_DEBUG_FUNCTION_ENTRY("cur_sts=%d,allow_sleep=%d", cur_sts, allow_sleep);
	if(cur_sts)		// !bk_gpio_get_input: it's optimized by compiler? the value maybe equal=0x100
		cur_sts = 0;
	else
		cur_sts = 1;
	if(allow_sleep != cur_sts)
	{
		SDIO_LOG_DEBUG("cur_sts=%d,allow_sleep=%d", cur_sts, allow_sleep);
		allow_sleep = cur_sts;
	}
	return (allow_sleep);
}
static void sdio_slave_response_host_wake_slave_ack(void)
{
	//do nothing now, no need to ack host
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
}
static void sdio_slave_response_host_sleep_slave_ack(void)
{
	//do nothing now, no need to ack host
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
}
/**
 * @brief	Host pull up/down GPIO to wakeup/clear wakeup slave.
 *          slave get the GPIO interrupt and run this handler.
 *
 * This API ....
 *
 * @attention 1. This API should be called after SDIO init.
 *
 * @return
 *	  -
 *	  -
 */
void wk_slp_controller_handler(void)
{
	uint32_t v27=bk_gpio_get_input(gpio_from_host_num);
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	{
		if(v27)	//now is high:check to low
		{
			SDIO_LOG_DEBUG("wakeup:low to high");
			set_controller_wk_slp_status(0);
			set_last_receive_data_time();	//update wakeup time.
			sdio_slave_response_host_wake_slave_ack();
			bk_gpio_enable_input(gpio_from_host_num);
			bk_gpio_pull_up(gpio_from_host_num);
			bk_gpio_disable_interrupt(gpio_from_host_num);
		}
		else
		{
			SDIO_LOG_DEBUG("sleep:high to low");
			set_controller_wk_slp_status(1);
			//set_last_receive_data_time();	//update wakeup time.
			sdio_slave_response_host_sleep_slave_ack();
			bk_gpio_enable_input(gpio_from_host_num);
			bk_gpio_pull_down(gpio_from_host_num);
			bk_gpio_enable_interrupt(gpio_from_host_num);
		}
	}
	SDIO_LOG_DEBUG_FUNCTION_EXIT();
}
void set_last_receive_data_time(void)
{
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	last_rx_data_time = bk_get_tick();
}
uint64_t get_last_receive_data_time(void)
{
	SDIO_LOG_DEBUG("last_time=%d", last_rx_data_time);
	return last_rx_data_time;
}
uint64_t check_wakeup_contorller_validity_timeout(void)
{
	uint8_t timeout;
	//LOG_D(GWS, "%s[+],last_time=%d", __func__, last_rx_data_time);
	if(get_controller_wk_slp_status() == 0)
	{
		timeout = (bk_get_tick() > (last_rx_data_time + FORBID_SLEEP_TIMEOUT) ? 1 : 0);
		if(timeout)
		{
			#define PRINT_INTERAL_TIME 10
			static uint64_t last_print_time = 0;
			if(bk_get_tick() - last_print_time > PRINT_INTERAL_TIME)
				SDIO_LOG_ERR("cur_time=%d,last_time=%d", __func__, bk_get_tick(), last_rx_data_time);
			last_print_time = bk_get_tick();
		}
	}
	return (bk_get_tick() - last_rx_data_time);
}
/**
 * @brief	wake,allow sleep by gpio feature resources init
 *
 * This API ....
 *
 * @param to_host: the gpio num which is used to wake up host or allow host sleep.
 * @param from_host: the gpio num which is used to wake up controller or allow controller sleep from host.
 *
 *
 * @attention 1. This API should be called after SDIO init.
 *
 * @return
 *	  -
 *	  -
 */

void sdio_gpio_notify_transaction_init(uint8_t to_host, uint8_t from_host)
{
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	sdio_gpio_notify_to_host_init(to_host);
	//sdio_gpio_notify_from_host_init(from_host);
	//bk_gpio_register_isr(from_host, (gpio_isr_t)wk_slp_controller_handler);
	SDIO_LOG_DEBUG_FUNCTION_EXIT();
}
void sdio_gpio_notify_transaction_deinit(uint8_t to_host, uint8_t from_host)
{
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	sdio_gpio_notify_to_host_deinit(to_host);
	sdio_gpio_notify_from_host_deinit(from_host);
	SDIO_LOG_DEBUG_FUNCTION_EXIT();
}
#endif	//CONFIG_GPIO_NOTIFY_TRANSACTION_EN
static void sdio_send_msg(sdio_msg_t *msg_p)
{
	bk_err_t ret;
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	if(msg_p)
	{
		SDIO_LOG_DEBUG("msg.id=%d,param=%d", msg_p->id, msg_p->param);
	}
	if (s_sdio_msg_que) {
		ret = rtos_push_to_queue(&s_sdio_msg_que, msg_p, BEKEN_NO_WAIT);
		if (kNoErr != ret)
			SDIO_LOG_ERR("sdio send msg failed");
	}
}
#define REG_GPIO_P14                     *((volatile unsigned long *) (0x44830000+0x4e*4))
#define REG_GPIO_P15                     *((volatile unsigned long *) (0x44830000+0x4f*4))
#define REG_GPIO_P16                     *((volatile unsigned long *) (0x44830000+0x50*4))
#define REG_GPIO_P17                     *((volatile unsigned long *) (0x44830000+0x51*4))
#define REG_GPIO_P18                     *((volatile unsigned long *) (0x44830000+0x52*4))
#define REG_GPIO_P19                     *((volatile unsigned long *) (0x44830000+0x53*4))

bk_err_t sdio_gpio_init(void)
{
	SDIO_LOG_INFO("sdio_gpio_init.\r\n");
	REG_GPIO_P14 = 0x3E000330;
	REG_GPIO_P15 = 0x3f000330;
	REG_GPIO_P16 = 0x40000330;
	REG_GPIO_P17 = 0x41000330;
#if CONFIG_SDIO_4LINES_EN
	REG_GPIO_P18 = 0x42000330;
	REG_GPIO_P19 = 0x43000330;
#endif
	return BK_OK;
}

bk_err_t sdio_gpio_deinit(void)
{
	SDIO_LOG_INFO("sdio_gpio_deinit.\r\n");
	REG_GPIO_P14 = 0x0;
	REG_GPIO_P15 = 0x0;
	REG_GPIO_P16 = 0x0;
	REG_GPIO_P17 = 0x0;
#if CONFIG_SDIO_4LINES_EN
	REG_GPIO_P18 = 0x0;
	REG_GPIO_P19 = 0x0;
#endif

	return BK_OK;
}
/**
 * @brief	sdio set controller clock.
 *
 * This API set the sdio clock as slave device.
 *	 -  1.The sdio slave default clock is setted as 26M.
 *      2.The transfer data clock on the lines is decided by host side,
 *        not this clock which is used for sdio-controller.
 *
 * @attention
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_sdio_set_clk(SDIO_CLK_SEL_T sel, SDIO_CLK_DIV_T div)
{
	uint32_t int_level = 0;
	int_level = rtos_disable_int();
	sys_driver_set_sdio_clk_sel((uint32_t)sel);
	sys_driver_set_sdio_clk_div((uint32_t)div);
	rtos_enable_int(int_level);
	return BK_OK;
}
/**
 * @brief	  Log out the SDIO REGs value without the "Data Read Reg(REG0xC)".
 *
 * This API log out SDIO REGs value without the "Data Read Reg(REG0xC)".
 *
 * @attention 1. This API doesn't log out the value of "Data Read Reg(REG0xC)",
 *               because if read REG0xC will cause FIFO data out.
 *
 * @return
 *	  - None.
 */
void bk_sdio_driver_dump_sdio_regs(void)
{
	uint32_t reg_addr = 0;
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	for(reg_addr = SDIO_LL_REG_BASE(0); reg_addr <= SDIO_LL_REG_END; reg_addr += 4)
	{
		if(reg_addr != SDIO_REG0X10_ADDR)	//skip read data out.
		{
			SDIO_LOG_DUMP("Reg0x%x=0x%x\r\n", (reg_addr - SDIO_LL_REG_BASE(0))/4, *(volatile uint32_t *)reg_addr);
		}
	}
	SDIO_LOG_DEBUG_FUNCTION_EXIT();
}

#if CONFIG_SDIO_CHANNEL_EN
/**
 * @brief	  Push the data use finish link list to free list.
 *
 * This API Push the data use finish link list to free list.
 *	 - Link the link list to free list.
 *   -
 *
 * @param chan_id	The selected chan id.
 * @param direct	The channel used for TX data or RX data
 * @param head_p    The list head of which node will be to free list
 * @param tail_p    The list tail of which data will be to free list
 * @param count     How many nodes will be to free list
 *
 * @attention
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_sdio_chan_push_free_list(
										sdio_chan_id_t chan_id,
										chan_direct_t direct,
										sdio_node_ptr_t head_p,
										sdio_node_ptr_t tail_p,
										uint32_t count
										)
{
	bk_err_t ret = BK_OK;
	uint32_t int_level = 0;
#if CONFIG_SDIO_BIDIRECT_CHANNEL_EN
	uint32_t buf_index = direct;
#else
	uint32_t buf_index = 0;
#endif
	sdio_chan_buf_t *chan_buf_p = NULL;
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	SDIO_RETURN_CHAN_ID(chan_id);
	SDIO_RETURN_CHAN_DIRECT(direct);
	int_level = rtos_disable_int();
	chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[buf_index];
#ifdef CONFIG_SDIO_DEBUG_EN
	sdio_list_dump_info(head_p, tail_p, count);
	sdio_list_dump_info(chan_buf_p->free_list.head, chan_buf_p->free_list.tail, chan_buf_p->free_list.count);
#endif
	sdio_list_push_list(&chan_buf_p->free_list, head_p, tail_p, count);
#ifdef CONFIG_SDIO_DEBUG_EN
	sdio_list_dump_info(chan_buf_p->free_list.head, chan_buf_p->free_list.tail, chan_buf_p->free_list.count);
#endif
	rtos_enable_int(int_level);
	SDIO_LOG_DEBUG_FUNCTION_EXIT();
	return ret;
}
/**
 * @brief	  Pop a free node from the selected channel and direct.
 *
 * This API Pop a free node from the selected channel and direct.
 *	 - Get a free node from the selected channel and direct.
 *   - Update the free node list to next node.
 *
 * @param chan_id	The selected chan id.
 * @param direct	The channel used for TX data or RX data
 * @param node_p    Saved the getted free buffer node.
 * @param size_p Saved the getted free buffer node capacity(How many bytes data can be saved).
 *
 * @attention
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_sdio_chan_pop_free_node(sdio_chan_id_t chan_id, chan_direct_t direct, sdio_node_ptr_t *node_p, uint32_t *size_p)
{
	bk_err_t ret = BK_OK;
	uint32_t int_level = 0;
#if CONFIG_SDIO_BIDIRECT_CHANNEL_EN
	uint32_t buf_index = direct;
#else
	uint32_t buf_index = 0;
#endif
	sdio_chan_buf_t *chan_buf_p = NULL;
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	SDIO_RETURN_CHAN_ID(chan_id);
	SDIO_RETURN_CHAN_DIRECT(direct);
	if(node_p == NULL)
	{
		SDIO_LOG_ERR("null pointer");
		return BK_ERR_SDIO_NULL_POINTER_PARAM;
	}
	int_level = rtos_disable_int();
	chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[buf_index];
#ifdef CONFIG_SDIO_DEBUG_EN
	sdio_list_dump_info(chan_buf_p->free_list.head, chan_buf_p->free_list.tail, chan_buf_p->free_list.count);
#endif
	if(chan_buf_p->free_list.count)
	{
		sdio_node_ptr_t tmp_node_p;
		ret = sdio_list_pop_node(&chan_buf_p->free_list, &tmp_node_p);
		if(ret != BK_OK)
		{
			SDIO_LOG_ERR("no free buf");
			rtos_enable_int(int_level);
			return BK_ERR_SDIO_NO_BUFFER;
		}
		SDIO_FILL_PAYLOAD_CHAN_ID(tmp_node_p, chan_id);
		*node_p = tmp_node_p;
		*size_p = chan_buf_p->buf_size;
	}
	else
	{
		*size_p = 0;
		*node_p = NULL;
		SDIO_LOG_WARNING("no free node");
		rtos_enable_int(int_level);
		return BK_ERR_SDIO_NO_BUFFER;
	}
	SDIO_LOG_DEBUG("chan-%d TX/RX=%d get buf:buf=0x%08x,size=%d", chan_id, direct, *node_p, *size_p);
#ifdef CONFIG_SDIO_DEBUG_EN
	sdio_list_dump_info(chan_buf_p->free_list.head, chan_buf_p->free_list.tail, chan_buf_p->free_list.count);
#endif
	rtos_enable_int(int_level);
	SDIO_LOG_DEBUG_FUNCTION_EXIT();
	return BK_OK;
}
#if CONFIG_SDIO_SLAVE_RX_FLOW_CONTROL
//value should be less then 255
static inline void sdio_set_rx_free_payloads_cnt(sdio_chan_id_t chan_id, chan_direct_t direct, uint32_t value)
{
	uint32_t reg_v = sdio_hal_slave_get_arg2_val();
	//here only care WIFI throughput channel.
	if((chan_id == 1) && (direct == SDIO_CHAN_RX))
	{
		reg_v = (reg_v & (~0xff)) | (value & (0xff));	//updates bytes 0
		sdio_hal_slave_set_arg2_val(value);
	}
}
static inline void sdio_reset_rx_free_payload_cnt()
{
	sdio_set_rx_free_payloads_cnt(1, SDIO_CHAN_RX, 10);
}
#endif
void sdio_chan_push_ongoing_node(
										sdio_chan_id_t chan_id,
										chan_direct_t direct,
										sdio_node_ptr_t head_p,
										sdio_node_ptr_t tail_p,
										uint32_t count
										)
{
	uint32_t int_level = 0;
#if CONFIG_SDIO_BIDIRECT_CHANNEL_EN
	uint32_t buf_index = direct;
#else
	uint32_t buf_index = 0;
#endif
	sdio_chan_buf_t *chan_buf_p = NULL;
	//TODO: check chan_id and direct is whether valid
	int_level = rtos_disable_int();
	chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[buf_index];
#ifdef CONFIG_SDIO_DEBUG_EN
	sdio_list_dump_info(chan_buf_p->ongoing_list.head, chan_buf_p->ongoing_list.tail, chan_buf_p->ongoing_list.count);
#endif
	sdio_list_push_list(&chan_buf_p->ongoing_list, head_p, tail_p, count);
#ifdef CONFIG_SDIO_DEBUG_EN
	sdio_list_dump_info(chan_buf_p->ongoing_list.head, chan_buf_p->ongoing_list.tail, chan_buf_p->ongoing_list.count);
#endif
	rtos_enable_int(int_level);
}
bk_err_t sdio_chan_pop_ongoing_node(sdio_chan_id_t chan_id, chan_direct_t direct, sdio_node_ptr_t *node_p)
{
	bk_err_t ret = BK_OK;
	uint32_t int_level = 0;
#if CONFIG_SDIO_BIDIRECT_CHANNEL_EN
	uint32_t buf_index = direct;
#else
	uint32_t buf_index = 0;
#endif
	sdio_chan_buf_t *chan_buf_p = NULL;
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	SDIO_RETURN_CHAN_ID(chan_id);
	SDIO_RETURN_CHAN_DIRECT(direct);
	int_level = rtos_disable_int();
	chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[buf_index];
#ifdef CONFIG_SDIO_DEBUG_EN
	sdio_list_dump_info(chan_buf_p->ongoing_list.head, chan_buf_p->ongoing_list.tail, chan_buf_p->ongoing_list.count);
#endif
	if(chan_buf_p->ongoing_list.count)
	{
		sdio_node_ptr_t tmp_node_p;
#if CONFIG_SDIO_SLAVE_RX_FLOW_CONTROL
		sdio_set_rx_free_payloads_cnt(chan_id, direct, chan_buf_p->ongoing_list.count - 1);
#endif
		ret = sdio_list_pop_node(&chan_buf_p->ongoing_list, &tmp_node_p);
		if(ret != BK_OK)
		{
			SDIO_LOG_ERR("no transaction finish node");
			return BK_ERR_SDIO_NO_TRANSACTION_FINISH_NODE;
		}
		*node_p = tmp_node_p;
	}
	else
	{
		rtos_enable_int(int_level);
		return BK_FAIL;
	}
#ifdef CONFIG_SDIO_DEBUG_EN
	sdio_list_dump_info(chan_buf_p->ongoing_list.head, chan_buf_p->ongoing_list.tail, chan_buf_p->ongoing_list.count);
#endif
	rtos_enable_int(int_level);
	SDIO_LOG_DEBUG_FUNCTION_EXIT();
	return BK_OK;
}
bk_err_t sdio_chan_push_finish_list(
										sdio_chan_id_t chan_id,
										chan_direct_t direct,
										sdio_node_ptr_t head_p,
										sdio_node_ptr_t tail_p,
										uint32_t count
										)
{
	uint32_t int_level = 0;
#if CONFIG_SDIO_BIDIRECT_CHANNEL_EN
	uint32_t buf_index = direct;
#else
	uint32_t buf_index = 0;
#endif
	sdio_chan_buf_t *chan_buf_p = NULL;
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	SDIO_RETURN_CHAN_ID(chan_id);
	SDIO_RETURN_CHAN_DIRECT(direct);
	int_level = rtos_disable_int();
	chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[buf_index];
#ifdef CONFIG_SDIO_DEBUG_EN
	sdio_list_dump_info(chan_buf_p->finish_list.head, chan_buf_p->finish_list.tail, chan_buf_p->finish_list.count);
#endif
	sdio_list_push_list(&chan_buf_p->finish_list, head_p, tail_p, count);
#ifdef CONFIG_SDIO_DEBUG_EN
	sdio_list_dump_info(chan_buf_p->finish_list.head, chan_buf_p->finish_list.tail, chan_buf_p->finish_list.count);
#endif
	rtos_enable_int(int_level);
	SDIO_LOG_DEBUG_FUNCTION_EXIT();
	return BK_OK;
}
bk_err_t sdio_chan_pop_finish_node(sdio_chan_id_t chan_id, chan_direct_t direct, sdio_node_ptr_t *node_p)
{
	bk_err_t ret = BK_OK;
	uint32_t int_level = 0;
#if CONFIG_SDIO_BIDIRECT_CHANNEL_EN
	uint32_t buf_index = direct;
#else
	uint32_t buf_index = 0;
#endif
	sdio_chan_buf_t *chan_buf_p = NULL;
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	SDIO_RETURN_CHAN_ID(chan_id);
	SDIO_RETURN_CHAN_DIRECT(direct);
	int_level = rtos_disable_int();
	chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[buf_index];
#ifdef CONFIG_SDIO_DEBUG_EN
	sdio_list_dump_info(chan_buf_p->finish_list.head, chan_buf_p->finish_list.tail, chan_buf_p->finish_list.count);
#endif
	if(chan_buf_p->finish_list.count)
	{
		sdio_node_ptr_t tmp_node_p;
		ret = sdio_list_pop_node(&chan_buf_p->finish_list, &tmp_node_p);
		if(ret != BK_OK)
		{
			SDIO_LOG_ERR("no node");
			return BK_ERR_SDIO_NO_BUFFER;
		}
		*node_p = tmp_node_p;
	}
#ifdef CONFIG_SDIO_DEBUG_EN
	sdio_list_dump_info(chan_buf_p->finish_list.head, chan_buf_p->finish_list.tail, chan_buf_p->finish_list.count);
#endif
	rtos_enable_int(int_level);
	SDIO_LOG_DEBUG_FUNCTION_EXIT();
	return BK_OK;
}
/**
 * @brief	  Register the clalback function for sdio selected channel and direct.
 *
 * This API register the clalback function for sdio selected channel and direct.
 *	 - Save the callback in channel.
 *   - When tx/rx data finish, it will call this callback function.
 *
 * @param chan_id	The selected chan id.
 * @param direct	The channel used for TX data or RX data
 * @param cb        Registered callback function.
 *
 * @attention
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_sdio_register_chan_cb(sdio_chan_id_t chan_id, chan_direct_t direct, sdio_chan_cb_t cb)
{
	uint32_t int_level = 0;
#if CONFIG_SDIO_BIDIRECT_CHANNEL_EN
	uint32_t buf_index = direct;
#else
	uint32_t buf_index = 0;
#endif
	sdio_chan_buf_t *chan_buf_p = NULL;
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	SDIO_RETURN_CHAN_ID(chan_id);
	SDIO_RETURN_CHAN_DIRECT(direct);
	int_level = rtos_disable_int();
	chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[buf_index];
	chan_buf_p->cb = cb;
	rtos_enable_int(int_level);
	SDIO_LOG_DEBUG_FUNCTION_EXIT();
	return BK_OK;
}
//should call sdio_chan_push_free_node after finish use
static bk_err_t sdio_chan_notify_cb(sdio_chan_id_t chan_id, chan_direct_t direct)
{
	bk_err_t ret = BK_OK;
#if CONFIG_SDIO_BIDIRECT_CHANNEL_EN
	uint32_t buf_index = direct;
#else
	uint32_t buf_index = 0;
#endif
	sdio_chan_buf_t *chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[buf_index];
	if(chan_buf_p->cb && chan_buf_p->finish_list.count)
	{
		uint32_t int_level = rtos_disable_int();
		sdio_list_t cb_list;
		os_memcpy(&cb_list, &chan_buf_p->finish_list, sizeof(sdio_list_t));
		//finish list should clear, avoid next time finish list err
		chan_buf_p->finish_list.head = NULL;
		chan_buf_p->finish_list.tail = NULL;
		chan_buf_p->finish_list.count = 0;
		rtos_enable_int(int_level);
		chan_buf_p->cb(chan_id,cb_list.head, cb_list.tail, cb_list.count);
	}
#if 0
	ret = rtos_set_semaphore(&chan_buf_p->semaphore);
	if(ret != BK_OK)
	{
		SDIO_LOG_ERR("set sema");
		return ret;
	}
#endif
	return ret;
}
static inline bool sdio_chan_direct_is_inited(sdio_chan_id_t chan_id, chan_direct_t direct)
{
	if((direct == SDIO_CHAN_TX) && (s_sdio_driver.chan[chan_id].tx_direct == 1))
		return true;
	else if((direct == SDIO_CHAN_RX) && (s_sdio_driver.chan[chan_id].rx_direct == 1))
		return true;
	else
		return false;
}
/**
 * @brief	  Init a sdio channel.
 *
 * This API init a sdio channel.
 *	 - Init a single list for buffers if count != 0
 *   - Create a semaphore to do notification for read/write data
 *   - Create a mutex lock to protect concurrence operate link list
 *
 * @param chan_id	The selected chan id.
 * @param direct	The channel used for TX data or RX data
 * @param count How many buffers will be malloced by channel.
 *              If count == 0 means the buffer malloced by APP.
 * @param size  Each buffer size for data which doesn't include SDIO header,it should be 4 bytes align
 *
 * @attention The count value is re-used as APPLICATION supports buffer or DRIVER supports buffer.
 *            Though if count == 0, the APPLICATION(caller) should set the size value as driver knows
 *            how many bytes can be saved in the buffer which APPLICATION supported.
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_sdio_init_channel(sdio_chan_id_t chan_id, chan_direct_t direct, uint32_t count, uint32_t size)
{
	bk_err_t ret = BK_OK;
	uint32_t int_level = 0;
	uint32_t sema_count = SDIO_CHAN_SEMAPHORE_COUNT;
#if CONFIG_SDIO_BIDIRECT_CHANNEL_EN
	uint32_t buf_index = direct;
#else
	uint32_t buf_index = 0;
#endif
	sdio_chan_buf_t *chan_buf_p = NULL;
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	SDIO_RETURN_CHAN_ID(chan_id);
	SDIO_RETURN_CHAN_DIRECT(direct);
	int_level = rtos_disable_int();
	chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[buf_index];
	if(sdio_chan_direct_is_inited(chan_id, direct))
	{
		SDIO_LOG_INFO("chan%d,direct %d inited,buf cnt=%d,buf size=%d",
			          chan_id, direct, chan_buf_p->buf_cnt, chan_buf_p->buf_size);
		goto err_exit;
	}
#if 0	//TODO:check whether needs it.
	if(chan_buf_p->lock_p == NULL)
	{
		ret = rtos_init_mutex(&chan_buf_p->lock_p);
		if (kNoErr != ret)
		{
			SDIO_LOG_ERR("mutex init");
			goto err_exit;
		}
	}
#endif
	if(count)
		sema_count = count;
	ret = rtos_init_semaphore(&chan_buf_p->semaphore, sema_count);
	if (kNoErr != ret)
	{
		SDIO_LOG_ERR("semaphore init");
		goto err_exit;
	}
	SDIO_LOG_DEBUG("sema_count=0x%x, chan_id=0x%x, buf_index = 0x%x .\r\n", sema_count, chan_id, buf_index);
	if(count)	//driver support buffer
	{
		ret = sdio_list_init(count, size, &chan_buf_p->free_list.head, &chan_buf_p->free_list.tail);
		if (BK_OK != ret)
		{
			rtos_deinit_semaphore(&chan_buf_p->semaphore);
			SDIO_LOG_ERR("list init");
			goto err_exit;
		}
	}
	//save channel info
	s_sdio_driver.chan[chan_id].chan_id = chan_id;
	if(direct == SDIO_CHAN_TX)
		s_sdio_driver.chan[chan_id].tx_direct = 1;
	else
		s_sdio_driver.chan[chan_id].rx_direct = 1;
	//save buf info
	chan_buf_p->buf_cnt = count;
	chan_buf_p->buf_size = size;
	chan_buf_p->free_list.count = count;
	//chan_buf_p->transaction_len = 0;
	//add this to avoid modify err_exit involve issue.
	rtos_enable_int(int_level);
	SDIO_LOG_DEBUG_FUNCTION_EXIT();
	return ret;
err_exit:
	rtos_enable_int(int_level);
	return ret;
}
/**
 * @brief	  Deinit a sdio channel.
 *
 * This API deinit a sdio channel.
 *	 - Init a single list for buffers
 *   - Create a mutex lock to protect concurrence operate link list
 *
 * @param chan_id	The selected chan id.
 * @param direct	The channel used for TX data or RX data
 *
 * @attention
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_sdio_deinit_channel(sdio_chan_id_t chan_id, chan_direct_t direct)
{
	bk_err_t ret = BK_OK;
	uint32_t int_level = 0;
#if CONFIG_SDIO_BIDIRECT_CHANNEL_EN
	uint32_t buf_index = direct;
#else
	uint32_t buf_index = 0;
#endif
	sdio_chan_buf_t *chan_buf_p = NULL;
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	SDIO_RETURN_CHAN_ID(chan_id);
	SDIO_RETURN_CHAN_DIRECT(direct);
	int_level = rtos_disable_int();
	chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[buf_index];
	if(sdio_chan_direct_is_inited(chan_id, direct) == false)
	{
		SDIO_LOG_INFO("chan%d,direct %d isn't inited", chan_id, direct);
		goto err_exit;
	}
	if(chan_buf_p->buf_cnt)	//driver support buffer
	{
		ret = sdio_list_deinit(&chan_buf_p->free_list);
		if (BK_OK != ret)
		{
			SDIO_LOG_ERR("list deinit");
			goto err_exit;
		}
		chan_buf_p->free_list.head = NULL;
		ret = sdio_list_deinit(&chan_buf_p->ongoing_list);
		if (BK_OK != ret)
		{
			SDIO_LOG_ERR("list deinit");
			goto err_exit;
		}
		chan_buf_p->ongoing_list.head = NULL;
		ret = sdio_list_deinit(&chan_buf_p->finish_list);
		if (BK_OK != ret)
		{
			SDIO_LOG_ERR("list deinit");
			goto err_exit;
		}
		chan_buf_p->finish_list.head = NULL;
	}
	if(chan_buf_p->semaphore)
	{
		ret = rtos_deinit_semaphore(&chan_buf_p->semaphore);
		if (kNoErr != ret)
		{
			SDIO_LOG_ERR("semaphore deinit");
			goto err_exit;
		}
		chan_buf_p->semaphore = NULL;
	}
#if 0	//TODO:check whether needs it.
	if(chan_buf_p->lock_p == NULL)
	{
		ret = rtos_deinit_mutex(&chan_buf_p->lock_p);
		if (kNoErr != ret)
		{
			SDIO_LOG_ERR("mutex deinit");
			goto err_exit;
		}
	}
#endif
	//save channel info
	s_sdio_driver.chan[chan_id].chan_id = SDIO_CHAN_MAX_CNT;
	if(direct == SDIO_CHAN_TX)
		s_sdio_driver.chan[chan_id].tx_direct = 0;
	else
		s_sdio_driver.chan[chan_id].rx_direct = 0;
	//save buf info
	chan_buf_p->buf_cnt = 0;
	chan_buf_p->buf_size = 0;
	chan_buf_p->free_list.count = 0;
	//chan_buf_p->transaction_len = 0;
	//add this to avoid modify err_exit involve issue.
	rtos_enable_int(int_level);
	SDIO_LOG_DEBUG_FUNCTION_EXIT();
	return ret;
err_exit:
	rtos_enable_int(int_level);
	return ret;
}
#endif
//WARNING:Becare 4bytes align issue
static bk_err_t sdio_slave_rx_fifo_to_buf(uint8_t *tar_addr_p, uint32_t count)
{
	//TODO:DMA not support until now.
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
#if CONFIG_SDIO_GDMA_EN
	BK_LOG_ON_ERR(bk_dma_set_transfer_len(s_sdio_rx_dma_chan, count));
	sdio_rx_dma_set_dst_buf(tar_addr_p, count);
	SDIO_LOG_DEBUG("count = %d, tar_addr_p = 0x%x\n", count, tar_addr_p);
	SDIO_LOG_DEBUG_FUNCTION_EXIT();
	return bk_dma_start(s_sdio_rx_dma_chan);
#else	//if not use GDMA, here will block IRQ too much time
	uint32_t i, byte_cnt = count % 4, word_cnt = count/4;
	uint8_t *cur_p = tar_addr_p;
	sdio_msg_t msg;
	//read data from SDIO REG to buffer
	for(i = 0; i < word_cnt; i = i + 4)
	{
		while(sdio_hal_get_read_ready() == 0)
		{
		}
		*(uint32_t *)cur_p = sdio_hal_slave_read_data();
		cur_p += 4;
	}
	if(byte_cnt)
	{
		while(sdio_hal_get_read_ready() == 0)
		{
		}
		//though here write 4 bytes, but the buffer should not use this length if not align 4 bytes
		*(uint32_t *)cur_p = sdio_hal_slave_read_data();
	}
	msg.id = SDIO_READ_TO_MEMORY_FINISH;
	sdio_send_msg(&msg);
#endif
	SDIO_LOG_DEBUG_FUNCTION_EXIT();
	return BK_OK;
}
/**
 * @brief	 SDIO sync read data from peer side(From Host)
 *
 * This API SDIO sync read data from peer side(From Host).
 *	 - Push the read link list to rxing list, after finish read, it will run out, or blocked by sema.
 *	 -
 * @param chan_id	The selected chan id.
 * @param head_p    The list head of which data will be save read data from peer side
 * @param tail_p    The list tail of which data will be save read data from peer side
 *                  If there is only one node, the head_p == tail_p
 * @param count     How many nodes will be used to save data.
 *
 * @attention 1.
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_sdio_slave_sync_read(sdio_chan_id_t chan_id, sdio_node_ptr_t head_p, sdio_node_ptr_t tail_p, uint32_t count)
{
	bk_err_t ret = BK_OK;
	uint32_t int_level = 0;
#if CONFIG_SDIO_BIDIRECT_CHANNEL_EN
	uint32_t buf_index = SDIO_CHAN_RX;
#else
	uint32_t buf_index = 0;
#endif
	sdio_chan_buf_t *chan_buf_p = NULL;
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	SDIO_RETURN_CHAN_ID(chan_id);
	int_level = rtos_disable_int();
	chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[buf_index];
	ret = sdio_list_push_list(&chan_buf_p->ongoing_list, head_p, tail_p, count);

	if(ret != BK_OK)
	{
		SDIO_LOG_ERR("push list");
		goto err_exit;
	}
#if CONFIG_SDIO_SLAVE_RX_FLOW_CONTROL
	sdio_set_rx_free_payloads_cnt(chan_id, buf_index, chan_buf_p->ongoing_list.count);
#endif
	//add this to avoid modify err_exit involve issue.
	rtos_enable_int(int_level);
	//TODO:wait read finish
//	SDIO_LOG_ERR("TODO:how to notify APP read finish:Sema/Callback");
	SDIO_LOG_DEBUG_FUNCTION_EXIT();
	return ret;
err_exit:
	rtos_enable_int(int_level);
	return ret;
}
/**
 * @brief
 *
 * This API .
 *	 -
 *	 -
 *
 * @attention 1.
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t sdio_slave_async_read(sdio_chan_id_t chan_id, sdio_node_ptr_t head_p, sdio_node_ptr_t tail_p, uint32_t count)
{
	bk_err_t ret = BK_OK;
	return ret;
}
void data_endian_trans(uint8_t* src_addr_p, uint32_t count)
{
    uint32_t index = 0;
    uint8_t  tmp_data = 0;
    SDIO_LOG_DEBUG("%s:src=0x%x,cnt=%d\n", __func__, src_addr_p, count);
    //stack_mem_dump(src_addr_p - 8, src_addr_p + 1024);
    while(index < count)
    {
        tmp_data = src_addr_p[index];
        src_addr_p[index] = src_addr_p[index + 3];
        src_addr_p[index + 3] = tmp_data;
        tmp_data = src_addr_p[index + 1];
        src_addr_p[index + 1] =  src_addr_p[index + 2];
        src_addr_p[index + 2] = tmp_data;
        index += 4;
    }
}
//TODO:whether MSG too much?event is better?
static inline bk_err_t sdio_slave_tx_request(sdio_chan_id_t chan_id)
{
	sdio_msg_t msg = {0};
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	msg.id = SDIO_APP_REQUEST_WRITE;
	msg.param = chan_id;
	sdio_send_msg(&msg);
	return BK_OK;
}
/**
 * @brief	 SDIO sync write data to peer side(To Host)
 *
 * This API SDIO sync write data to peer side(To Host) .
 *	 - Push the write link list to write list, after finish write, it will run out, or blocked by sema.
 *	 -
 * @param chan_id	The selected chan id.
 * @param head_p    The list head of which data will be write
 * @param tail_p    The list tail of which data will be write
 * @param count     How many nodes will be write
 *
 * @attention 1.
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_sdio_slave_sync_write(sdio_chan_id_t chan_id, sdio_node_ptr_t head_p, sdio_node_ptr_t tail_p, uint32_t count)
{
	bk_err_t ret = BK_OK;
	uint32_t int_level = 0;
#if CONFIG_SDIO_BIDIRECT_CHANNEL_EN
	uint32_t buf_index = SDIO_CHAN_TX;
#else
	uint32_t buf_index = 0;
#endif
	sdio_chan_buf_t *chan_buf_p = NULL;
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	SDIO_RETURN_CHAN_ID(chan_id);
	chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[buf_index];
	int_level = rtos_disable_int();
	ret = sdio_list_push_list(&chan_buf_p->ongoing_list, head_p, tail_p, count);
	if(ret != BK_OK)
	{
		SDIO_LOG_ERR("push list fail");
		goto err_exit;
	}
	rtos_enable_int(int_level);
#if CONFIG_SDIO_SLAVE_TX_MEMCPY_EN
	rtos_set_semaphore(&s_sdio_memcpy_sema);
#endif
	SDIO_LOG_DEBUG_FUNCTION_EXIT();
	return ret;
err_exit:
	rtos_enable_int(int_level);
	return ret;
}
/**
 * @brief
 *
 * This API .
 *	 -
 *	 -
 *
 * @attention 1.
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t sdio_slave_async_write(sdio_chan_id_t chan_id, sdio_node_ptr_t head_p, sdio_node_ptr_t tail_p, uint32_t count)
{
	bk_err_t ret = BK_OK;
	return ret;
}

static uint32_t write_blk_cnt = 0;
static volatile uint32_t s_record_msg = 0;	//just for debug

static void sdio_thread(void *arg)
{
	bk_err_t ret;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
#if CONFIG_SDIO_CMD_AUTO_RSP_EN
	sdio_hal_cmd_sst_sel_en(0);
#endif

	while(1)
	{
		sdio_msg_t msg;
		ret = rtos_pop_from_queue(&s_sdio_msg_que, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == ret)
		{
#if CONFIG_SDIO_DEBUG_EN
			s_record_msg = msg.id;
#endif
			SDIO_LOG_DEBUG("rsp:msg.id=%d,param=%d", msg.id, msg.param);
			switch(msg.id)
			{
				//from SDIO IRQ
				case SDIO_NOTIFY_START_READ:
				{
					sdio_cmd53_arg_t cmd53_arg = (sdio_cmd53_arg_t)sdio_hal_slave_get_cmd_arg0();
					BK_ASSERT(s_rx_pack_total_len == 0);	//confirm previous complete.
					if(cmd53_arg.block_mode)
						s_pack_rx_left_len = s_rx_pack_total_len = cmd53_arg.count * SDIO_BLOCK_SIZE;
					else
						s_pack_rx_left_len = s_rx_pack_total_len = cmd53_arg.count;

					if(s_tx_pack_total_len - s_pack_tx_complete_len)
						SDIO_LOG_DEBUG("SDIO_NOTIFY_START_READ CMD53 tx_left_bytes is %d.\r\n", s_tx_pack_total_len - s_pack_tx_complete_len);
#if CONFIG_SDIO_SLAVE_RX_MEMCPY_EN
					request_dma_copy_sdio_data_to_payloads();
#else
					request_cpu_and_dma_copy_sdio_data_to_payloads();
#endif
					break;
				}

				case SDIO_READ_PACK_FINISH:
				{
					for(sdio_chan_id_t chan_id = 0; chan_id < SDIO_CHAN_MAX_CNT; chan_id++)
					{
						sdio_chan_notify_cb(chan_id, SDIO_CHAN_RX);
					}
					sdio_hal_fifo_reset();
					sdio_hal_cmd_sst_sel_en(0);//debug
					sdio_hal_slave_cmd_start(1);	//wait to receive next CMD.
					break;
				}
				case SDIO_APP_REQUEST_WRITE:
				{
					if((write_blk_cnt == 0) && //prev TX complete
						(s_sdio_driver.pack.rd_index < s_sdio_driver.pack.wr_index))	//confirm pack node needs to TX
					{
						uint32_t rd_index = s_sdio_driver.pack.rd_index % SDIO_MEMCPY_PACK_CNT;
						s_tx_pack_total_len = (uint32_t)s_sdio_driver.pack.end_addr[rd_index] - (uint32_t)s_sdio_driver.pack.start_addr[rd_index];
						s_pack_tx_complete_len = 0;
#if CONFIG_SDIO_PACK_END_FLAG
						uint32_t fill_size = SDIO_BLOCK_SIZE - (s_tx_pack_total_len%SDIO_BLOCK_SIZE);
						pack_tail_t tail = {SDIO_PACK_END_MAGIC, s_tx_pack_total_len};
						if(fill_size)	//has enough buffer to adds pack tail
						{
							if(fill_size >= sizeof(tail))
								os_memcpy(s_sdio_driver.pack.end_addr[rd_index], &tail, sizeof(tail));
							else
								os_memcpy(s_sdio_driver.pack.end_addr[rd_index], &tail, 4);	//4 bytes align, assume space is enough
						}
#endif
						//Slave set TX length
						write_blk_cnt = SDIO_BYTES_TO_BLOCKS_CNT(s_tx_pack_total_len);
						sdio_hal_slave_set_tx_length(write_blk_cnt);	//set to SDIO register is block cnt;

#if CONFIG_GPIO_NOTIFY_TRANSACTION_EN
						sdio_slave_set_notify_host();	//Notify host:slave will send data
#endif
						SDIO_LOG_DEBUG("Req TX:tx_pack_len=%d,block_cnt=%d", s_tx_pack_total_len, write_blk_cnt);
					}
					else	//after TX complete, will try to request write again
					{
						SDIO_LOG_DEBUG("Reject Write:total=%d,complete=%d,rd=%d,wr=%d\n", s_tx_pack_total_len, s_pack_tx_complete_len, s_sdio_driver.pack.rd_index, s_sdio_driver.pack.wr_index);
					}
					break;
				}
				//from SDIO IRQ:Host CMD53 read,Slave can transfer data now
				case SDIO_NOTIFY_START_WRITE:
				{
					sdio_hal_slave_set_tx_length(0);    //clear slave tx length as 0
					if(write_blk_cnt != (msg.param / SDIO_BLOCK_SIZE))
						SDIO_LOG_ERR("Request TX size=%d, Host RX size=%d", write_blk_cnt, (msg.param / SDIO_BLOCK_SIZE));
					sdio_hal_slave_tx_transaction_en();
					request_dma_copy_packet_data_to_sdio();
					break;
				}
				//TODO:do it in bottom-half
				case SDIO_WRITE_BLK_FINISH:
				{
					write_blk_cnt--;
					//current pack tx complete
					if(write_blk_cnt == 0)
					{
						//really TX complete by SDIO to Host
						s_sdio_driver.pack.end_addr[s_sdio_driver.pack.rd_index%SDIO_MEMCPY_PACK_CNT] = s_sdio_driver.pack.start_addr[s_sdio_driver.pack.rd_index%SDIO_MEMCPY_PACK_CNT];
						s_sdio_driver.pack.rd_index++;
						s_tx_pack_total_len = 0;
						s_pack_tx_complete_len = 0;
						for(sdio_chan_id_t chan_id = 0; chan_id < SDIO_CHAN_MAX_CNT; chan_id++)
						{
							sdio_chan_notify_cb(chan_id, SDIO_CHAN_TX);
						}
						//previous request doesn't satisfy at once, now needs to require it
						if(SDIO_MEMCPY_PACK_USED_LEN(s_sdio_driver.pack.wr_index % SDIO_MEMCPY_PACK_CNT))
						{
							rtos_set_semaphore(&s_sdio_memcpy_sema);
						}
					}
#ifndef CONFIG_SDIO_CMD_AUTO_RSP_EN  //ASIC BUG fixed: one packet send all data
					else if(write_blk_cnt == 1)	//last block
					{
						uint32_t last_word = *(uint32_t *)((uint32_t)s_sdio_driver.pack.start_addr[s_sdio_driver.pack.rd_index%SDIO_MEMCPY_PACK_CNT] +
											SDIO_BYTES_TO_BLOCKS_CNT(s_tx_pack_total_len) * SDIO_BLOCK_SIZE - 4);
						//wait previous DMA writes all RAM data to SDIO FIFO
						while(bk_dma_get_remain_len(s_sdio_tx_dma_chan))
						{
							SDIO_LOG_DEBUG("last dma copy not complete:len=%d", bk_dma_get_remain_len(s_sdio_tx_dma_chan));
						}
						//fill the last 4 bytes to SDIO FIFO
						sdio_hal_slave_write_data(last_word);
					}
					else	//request TX next block
					{
						request_dma_copy_packet_data_to_sdio();
					}
#endif
					break;
				}
				default:
					break;
			}
		}
	}
}
#if CONFIG_SDIO_SLAVE_TX_MEMCPY_EN
static void sdio_slave_tx_memcpy_thread(void *arg)
{
	bk_err_t ret = BK_OK;
	sdio_chan_id_t chan_id = 0;
	sdio_node_ptr_t cur_node_p = NULL;
	uint32_t cur_pack_left_size = 0;
	uint32_t wr_index = 0;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	while(1)
	{
		ret = rtos_get_semaphore(&s_sdio_memcpy_sema, BEKEN_WAIT_FOREVER);
		if (kNoErr == ret)
		{
			//notify_tx = false;
			for(chan_id = 0; chan_id < SDIO_CHAN_MAX_CNT; chan_id++)
			{
				while(sdio_chan_pop_ongoing_node(chan_id, SDIO_CHAN_TX, &cur_node_p) == BK_OK)	//Current channel hasn't memcpy not complete
				{
					uint32_t payload_len = SDIO_GET_PAYLOAD_LEN(cur_node_p);
					wr_index = s_sdio_driver.pack.wr_index % SDIO_MEMCPY_PACK_CNT;
					cur_pack_left_size = SDIO_MEMCPY_PACK_SIZE - SDIO_MEMCPY_PACK_USED_LEN(wr_index);
					//Current payload len, Current pack left len
					if(cur_pack_left_size < SDIO_ALIGN_LENGTH(payload_len))	//Current pack hasn't enough buffer
					{
						uint32_t delay_ms_cnt = 0;
						SDIO_LOG_DEBUG("Current pack full:left_size=%d,req len=%d,wr_pack_index=%d", cur_pack_left_size, payload_len, s_sdio_driver.pack.wr_index);
						//request sdio_thread to do transactions
						sdio_slave_tx_request(0);
						//try to switch next pack buffer
						s_sdio_driver.pack.wr_index += 1;
						while(s_sdio_driver.pack.wr_index - s_sdio_driver.pack.rd_index >= SDIO_MEMCPY_PACK_CNT)
						{
							rtos_delay_milliseconds(1);	//TODO:here can be suspend by a yet
							delay_ms_cnt++;
							if((delay_ms_cnt % 200) == 0) {
								SDIO_LOG_ERR("TX timeout: tx len is %d, notify host again.\r\n", s_tx_pack_total_len);
								sdio_hal_slave_set_tx_length(SDIO_BYTES_TO_BLOCKS_CNT(s_tx_pack_total_len));
#if 0 //CONFIG_SDIO_CMD_AUTO_RSP_EN
								sdio_hal_cccr_intx_en(1);		// SDIO IRQ Notify host:slave will send data
#endif

#if CONFIG_GPIO_NOTIFY_TRANSACTION_EN
								sdio_slave_set_notify_host();
								sdio_slave_clear_notify_host();
#endif
							}
							if(delay_ms_cnt > SDIO_TX_PACK_FULL_TIMEOUT) {
								BK_ASSERT(0);
							}
						}
						//switch to new pack
						wr_index = s_sdio_driver.pack.wr_index % SDIO_MEMCPY_PACK_CNT;
						s_sdio_driver.pack.end_addr[wr_index] = s_sdio_driver.pack.start_addr[wr_index];
					}
					//copy payload data to pack buffer
					os_memcpy(s_sdio_driver.pack.end_addr[wr_index], (void *)SDIO_NODE_PTR_TO_DATA_PTR(cur_node_p), SDIO_ALIGN_LENGTH(payload_len));
					s_sdio_driver.pack.end_addr[wr_index] = (uint32_t *)((uint32_t)s_sdio_driver.pack.end_addr[wr_index] + SDIO_ALIGN_LENGTH(payload_len));
					sdio_chan_push_finish_list(chan_id, SDIO_CHAN_TX, cur_node_p, cur_node_p, 1);
					//notify_tx = true;
				}
			}
			//request sdio_thread to do transactions
			if(SDIO_MEMCPY_PACK_USED_LEN(s_sdio_driver.pack.wr_index%SDIO_MEMCPY_PACK_CNT))
			{
				//if host hasn't start to read before here, then no need to switch wr_index
				if(s_sdio_driver.pack.rd_index == s_sdio_driver.pack.wr_index)
					s_sdio_driver.pack.wr_index += 1;	//last pack notify to TX,though maybe not full
				sdio_slave_tx_request(0);
			}
			SDIO_LOG_DEBUG("rd_index=%d,wr_index=%d", s_sdio_driver.pack.rd_index, s_sdio_driver.pack.wr_index);
		}
	}
}
#endif
static uint32_t s_wr_blk_left_cnt = 0;
#if CONFIG_SDIO_DEBUG_EN
static volatile uint32_t s_record_cmd_index;
static volatile uint32_t s_record_cmd_arg;
static volatile uint32_t s_record_int_status;
#endif
__attribute__((section(".iram_sec_code"))) static void sdio_slave_isr(void)
{
	uint32_t int_status = sdio_hal_get_int_status();
	uint32_t cmd_index = (int_status >> 14) & (0x3f);

	//Slave response end int
	if(sdio_hal_slave_get_cmd_response_end_int()) //TODO:uses int_status to reduce function caller
	{
		sdio_hal_slave_clear_cmd_response_end_int(); //CMD_S_RES_END_INT
		SDIO_LOG_DEBUG("CMD%d Respond end", cmd_index);
		switch (cmd_index)
		{
			case SDIO_CMD_INDEX_52:
			{
				sdio_hal_slave_cmd_start(1);
				break;
			}
			case SDIO_CMD_INDEX_53:
			{
				sdio_cmd53_arg_t cmd53_arg;
				uint32_t cmd53_arg_value = 0;

				cmd53_arg_value = sdio_hal_slave_get_cmd_arg0();
				cmd53_arg.v = cmd53_arg_value;
#if CONFIG_SDIO_DEBUG
				s_record_cmd_arg = cmd53_arg_value;
#endif
				if(cmd53_arg.rw)	//host write, slave read
				{
					//notify sdio task to read data
					if(cmd53_arg.block_mode)
					{
						s_sdio_rx_bytes = SDIO_BLOCK_SIZE * cmd53_arg.count;
						if(cmd53_arg.count)
							SDIO_LOG_DEBUG("Host write:%d blocks(x 512bytes)", cmd53_arg.count);
						else
							SDIO_LOG_DEBUG("Host write:multi-blocks till stop");
					}
					else
					{
						s_sdio_rx_bytes = cmd53_arg.count;
						SDIO_LOG_DEBUG("Host write:%d bytes", cmd53_arg.count);
					}
					sdio_msg_t msg;
					msg.id = SDIO_NOTIFY_START_READ;
					msg.param =  (uint32_t)s_sdio_rx_bytes;
					sdio_send_msg(&msg);
				}
				else	//host read, slave write
				{
					//notify sdio task to read data
					if(cmd53_arg.block_mode)
					{
						s_sdio_tx_bytes = SDIO_BLOCK_SIZE * cmd53_arg.count;
						if(cmd53_arg.count)
						{
							s_wr_blk_left_cnt = cmd53_arg.count;
							SDIO_LOG_DEBUG("Host read:%d blocks(x 512bytes)", cmd53_arg.count);
						}
						else
							SDIO_LOG_DEBUG("Host read:multi-blocks till stop");
					}
					else
					{
						s_sdio_tx_bytes = cmd53_arg.count;
						SDIO_LOG_DEBUG("Host read:%d bytes", cmd53_arg.count);
					}
					sdio_msg_t msg;
					msg.id = SDIO_NOTIFY_START_WRITE;
					msg.param = s_sdio_tx_bytes;
					sdio_send_msg(&msg);
				}
				break;
			}
			default:
			{
				SDIO_LOG_ERR("cmd_index = 0x%x.\r\n", cmd_index);
				sdio_hal_slave_cmd_start(1);	//wait to receive next CMD.
				break;
			}
		}
#if CONFIG_SDIO_DEBUG_EN
		s_record_cmd_index = cmd_index;
#endif
		//TODO:need to check whether return.
		return;
	}
#if CONFIG_SDIO_DEBUG_EN
	s_record_int_status = int_status;
#endif
	//slave rx each block int
	if(sdio_hal_slave_get_data_rec_end_int())
	{
		//sdio_msg_t    msg;
		//msg.id = SDIO_READ_BLK_FINISH;
		//sdio_send_msg(&msg);
		SDIO_LOG_DEBUG("====rx blk end====\r\n");
		sdio_hal_slave_clear_data_rec_end_int();
	}
	//slave rx full or finish data int
	if(sdio_hal_slave_get_read_int_status())
	{
		SDIO_LOG_DEBUG("TODO:===disable===ISR to Task:Read");
#if 0
		sdio_msg_t	msg;
		msg.id = SDIO_READ_TO_FIFO_FINISH;
		sdio_send_msg(&msg);
#endif
		//clear self int status
		sdio_hal_slave_clear_read_int_status();
	}
	//slave tx fifo empty int
	if(sdio_hal_get_tx_fifo_empty_int_status())
	{
		//disable int mask
		sdio_hal_set_tx_fifo_empty_int(0);
		//clear self int status
		sdio_hal_clear_tx_fifo_empty_int_status();
	}
	//slave tx write data end int
	if(sdio_hal_slave_get_wr_end_int())
	{
		sdio_msg_t	msg;
		sdio_hal_slave_clear_wr_end_int();
		//SDIO_LOG_ERR("tx blk end\r\n");
#if 1	//move to top-half, quickly enable CMD START, and then can response host next CMDx
		s_wr_blk_left_cnt--;
		if(s_wr_blk_left_cnt == 0)	//last block
		{
#if CONFIG_GPIO_NOTIFY_TRANSACTION_EN
			sdio_slave_clear_notify_host();
#endif
			//force reset current TX
			{
				sdio_hal_set_tx_fifo_empty_int(0);
				//TODO:
				sdio_hal_fifo_reset();
				sdio_hal_cmd_sst_sel_en(0);
				sdio_hal_slave_cmd_start(1);	//wait to receive next CMD.
			}
		}
#endif
		msg.id = SDIO_WRITE_BLK_FINISH;
		sdio_send_msg(&msg);
	}
	//TODO:whether notify task?
	if(sdio_hal_slave_get_wr_blk_int_status())
	{
		sdio_hal_slave_clear_wr_blk_int_status();
		SDIO_LOG_INFO("TODO:wr block end int status");
	}
	//TODO:whether notify task?
	if(sdio_hal_slave_get_rd_blk_int_status())
	{
		sdio_hal_slave_clear_rd_blk_int_status();
		SDIO_LOG_INFO("TODO:rd block end int status\n");
	}
}
void sdio_set_log_level(uint8_t level)
{
	SDIO_LOG_DEBUG("%s:%d", __func__, level);
	g_sdio_log_level = level;
}
#if CONFIG_SDIO_DEBUG
void bk_sdio_driver_dump_info(void)
{
	SDIO_LOG_DUMP("cmd=%d,arg=0x%x,int_sts=%d,s_record_msg=%d", s_record_cmd_index, s_record_cmd_arg, s_record_int_status, s_record_msg);
	bk_sdio_driver_dump_sdio_regs();
}
#endif
static bk_err_t sdio_sw_init(void)
{
	bk_err_t ret = BK_OK;
	//confirm to 0 as maybe crash value by other app.
	memset(&s_sdio_driver, 0, sizeof(s_sdio_driver));
	ret = rtos_init_queue(
							&s_sdio_msg_que,
							SDIO_MSG_QUEUE_NAME,
							sizeof(sdio_msg_t),
							SDIO_MSG_QUEUE_COUNT
						);
	if (kNoErr != ret)
	{
		SDIO_LOG_ERR("init queue ret=%d", ret);
		goto err_exit;
	}
	ret = rtos_create_thread(
								&s_sdio_thread,
								SDIO_THREAD_PRIORITY,
								SDIO_THREAD_NAME,
								sdio_thread,
								SDIO_THREAD_STACK_SIZE,
								NULL
							);
	if (kNoErr != ret)
	{
		SDIO_LOG_ERR("init thread ret=%d", ret);
		goto err_exit;
	}
#if CONFIG_SDIO_SLAVE_TX_MEMCPY_EN
	/* if (s_sdio_memcpy_sema == NULL) */
	{
		int ret = rtos_init_semaphore(&s_sdio_memcpy_sema, 1);
		if (kNoErr != ret)
		{
			SDIO_LOG_ERR("init sema ret=%d", ret);
			goto err_exit;
		}
	}
	ret = rtos_create_thread(
								&s_sdio_tx_memcpy_thread,
								SDIO_MEMCPY_THREAD_PRIORITY,
								SDIO_MEMCPY_THREAD_NAME,
								sdio_slave_tx_memcpy_thread,
								SDIO_MEMCPY_THREAD_STACK_SIZE,
								NULL
							);
	if (kNoErr != ret)
	{
		SDIO_LOG_ERR("init thread ret=%d", ret);
		goto err_exit;
	}
	for(uint32_t i = 0; i < SDIO_MEMCPY_PACK_CNT; i++)
	{
		uint32_t *temp_ptr = os_zalloc(SDIO_MEMCPY_PACK_SIZE);
		if(temp_ptr == NULL)
		{
			SDIO_LOG_ERR("os_malloc fail, i = %d", i);
			goto err_exit;
		}

		s_sdio_driver.pack.start_addr[i] = temp_ptr;
		s_sdio_driver.pack.end_addr[i] = temp_ptr;
	}
#endif
#if CONFIG_SDIO_SLAVE_RX_MEMCPY_EN
	for(uint32_t i = 0; i < SDIO_RX_MEMCPY_PACK_CNT; i++)
	{
		s_sdio_driver.rx_pack.end_addr[i] = s_sdio_driver.rx_pack.start_addr[i] = os_malloc(SDIO_MEMCPY_PACK_SIZE);
		if(s_sdio_driver.rx_pack.start_addr[i] == NULL)
		{
			SDIO_LOG_ERR("os_malloc fail, i = %d", i);
			goto err_exit;
		}
	}
#endif
	SDIO_LOG_DEBUG("sizeof(s_sdio_driver)=%d", sizeof(s_sdio_driver));
	//return, or err_exit release resources caused bug.
	return ret;
err_exit:
	SDIO_LOG_ERR("exit");
	if(s_sdio_msg_que)
	{
		rtos_deinit_queue(&s_sdio_msg_que);
	}
	if(s_sdio_thread)
	{
		rtos_delete_thread(&s_sdio_thread);
	}
#if CONFIG_SDIO_SLAVE_TX_MEMCPY_EN
	if(s_sdio_memcpy_sema)
	{
		rtos_deinit_semaphore(&s_sdio_memcpy_sema);
		s_sdio_memcpy_sema = NULL;
	}
	if(s_sdio_tx_memcpy_thread)
	{
		rtos_delete_thread(&s_sdio_tx_memcpy_thread);
	}
	for(uint32_t i = 0; i < SDIO_MEMCPY_PACK_CNT; i++)
	{
		if(s_sdio_driver.pack.start_addr[i])
		{
			os_free(s_sdio_driver.pack.start_addr[i]);
			s_sdio_driver.pack.end_addr[i] = s_sdio_driver.pack.start_addr[i] = NULL;
		}
	}
#endif
#if CONFIG_SDIO_SLAVE_RX_MEMCPY_EN
	for(uint32_t i = 0; i < SDIO_RX_MEMCPY_PACK_CNT; i++)
	{
		if(s_sdio_driver.rx_pack.start_addr[i])
		{
			os_free(s_sdio_driver.rx_pack.start_addr[i]);
			s_sdio_driver.rx_pack.end_addr[i] = s_sdio_driver.rx_pack.start_addr[i] = NULL;
		}
	}
#endif
	return ret;
}
/*
 * Just only clear global variables.
 * Doesn't clear queue/sema(s_sdio_msg_que, s_sdio_memcpy_sema):
 */
static bk_err_t sdio_slave_sw_reset()
{
	s_sdio_driver.pack.wr_index = s_sdio_driver.pack.rd_index = 0;
	s_sdio_driver.rx_pack.wr_index = s_sdio_driver.rx_pack.rd_index = 0;
#if CONFIG_SDIO_SLAVE_TX_MEMCPY_EN
	//TX
	s_tx_pack_total_len = 0;
	s_pack_tx_complete_len = 0;
	s_sdio_tx_bytes = 0;
	s_wr_blk_left_cnt = 0;
	write_blk_cnt = 0;
#endif
	//RX
#if CONFIG_SDIO_SLAVE_RX_MEMCPY_EN
	/*
	 * Special case:
	 * Maybe one paylod copy data from temp buffer , crosses twice MEMORY-COPY buffer,
	 * but it can't comlete copy all of the data because Host stop transfer data.
	 * So needs to push the special payload buffer to ongoing list again.
	 * NOTES:It doesn't disable int:because it only modifies value in DMA ISR and here.
	 */
	if(s_rx_last_payload)
	{
		sdio_chan_id_t chan_id = SDIO_NODE_PAYLOAD_CHAN_ID(s_rx_last_payload);
		sdio_chan_push_ongoing_node(chan_id, SDIO_CHAN_RX, s_rx_last_payload, s_rx_last_payload, 1);
		s_rx_last_payload = NULL;
	}
	s_rx_pack_total_len = 0;
	s_pack_rx_left_len = 0;
	s_rx_last_len = 0;
	s_rx_payload_copied_len = 0;
	s_sdio_rx_bytes = 0;
#endif
	return BK_OK;
}
/*
 * When enter low power, it should clear/save some status.
 * NOTES:Until now, only support low-voltage mode.
 *       so it only clears software status, because low-voltage will
 *       power-down SDIO, DMA...
 */
bk_err_t sdio_enter_low_power(void *param_p)
{
	return sdio_slave_sw_reset();
}
static bk_err_t sdio_slave_hw_init(void)
{
	bk_err_t ret = BK_OK;

	sdio_hal_set_sd_soft_resetn(1);
	sdio_hal_set_host_slave_mode(SDIO_SLAVE_MODE);
	sdio_hal_slave_set_samp_sel(1);
	sdio_hal_rx_set_sd_byte_sel(1);

	//clock
	sys_hal_set_sdio_clk_en(true);
	sys_hal_set_sdio_clk_sel((uint32_t)SDIO_CLK_XTL);
	sys_hal_set_sdio_clk_div((uint32_t)SDIO_CLK_DIV_1);

#if CONFIG_SDIO_4LINES_EN
	sdio_hal_set_sd_data_bus(1);
#endif

#if CONFIG_SDIO_1V8_EN
	sys_drv_psram_psldo_vset(1, 0); //set vddpsram voltage as 1.8v
	sys_drv_psram_ldo_enable(1);    //vddpsram enable
	sys_hal_set_ana_vddgpio_sel(1); //GPIO vdd select between vio and vddparam
#endif

	//pin
	ret = sdio_gpio_init();
	if(ret != BK_OK)
	{
		SDIO_LOG_ERR("gpio init");
		return ret;
	}

#if CONFIG_GPIO_NOTIFY_TRANSACTION_EN
	sdio_gpio_notify_transaction_init(GPIO_SDIO_NOTIFY_HOST, GPIO_SDIO_HOST_WAKEUP_SLAVE);
#endif
	//dma has to enable DMA to TX/RX data to increase throughput
	BK_ASSERT(CONFIG_SDIO_GDMA_EN);
#if CONFIG_SDIO_GDMA_EN
	ret = sdio_dma_init();
	if(ret != BK_OK)
	{
		SDIO_LOG_ERR("dma init");
		return ret;
	}
#endif
	//interrupt
	sdio_hal_slave_set_cmd_res_end_int(1);
	sdio_hal_slave_set_write_end_int(1);
	sdio_hal_slave_set_read_end_int(1);
	sdio_hal_slave_set_data_rec_end_int(1);
	sdio_hal_slave_set_write_mul_sel(1);
	sdio_hal_slave_set_write_block_int_mask(1);
#if CONFIG_SDIO_SLAVE_RX_WITH_BLK
	sdio_hal_slave_set_read_mul_sel(0);    //disable asic auto release data0 busy
#else
	sdio_hal_slave_set_read_mul_sel(1);
#endif
#if CONFIG_SDIO_CMD_AUTO_RSP_EN
	sdio_hal_cmd_sst_sel_en(1);
	sdio_hal_dat1_int_en(1);
	sdio_hal_cccr_s4mi_en(1);
#endif
	bk_int_isr_register(INT_SRC_SDIO, sdio_slave_isr, NULL);
//TODO:SMP how to deal?
#if	((CONFIG_CPU_CNT > 1) && CONFIG_SYS_CPU1)
	sys_driver_set_cpu1_sdio_int_en(true);
#else
	sys_driver_set_cpu0_sdio_int_en(true);
#endif
	//prevent response invalid cmd
	sdio_hal_set_sd_cmd_crc_check(1);
	sdio_hal_set_cmd_keep_det(1);  //Write only reg
	sdio_hal_fifo_reset();
	//CMD start:wait Host CMD
	sdio_hal_slave_cmd_start(1);

	return ret;
}
/*
 * when enter low-power(low-voltage) mode, the SDIO Hardware registers will be cleared.
 * So exit from low-power, it should restore SDIO registers and relate info.
 */

bk_err_t sdio_slave_hw_reinit(void)
{
	write_blk_cnt = 0;
	#if CONFIG_SDIO_SLAVE_RX_FLOW_CONTROL
	sdio_reset_rx_free_payload_cnt();
	#endif
	bk_sdio_loopcheck_init();
	return sdio_slave_hw_init();
}

/*
 * Sometimes, Host communicates with Slave failed, then host will notify Slave that it can't work.
 * After that, Slave can reset SDIO hardware and software status.
 */
bk_err_t sdio_reset(void)
{
	for(uint32_t i = 0; i < 8; i++)
		SDIO_LOG_ERR("WWWWWAR:enter func %s.WWWWWAR\r\n", __func__);
	uint32_t int_level = 0;
	int_level = rtos_disable_int();
	sdio_slave_sw_reset();
	//force clear queue/sema?
	if(rtos_is_queue_empty(&s_sdio_msg_que) == 0)	//just debug
	{
		SDIO_LOG_ERR("WWWWWAR:Queue not Empty.WWWWWAR\r\n");
		rtos_reset_queue(&s_sdio_msg_que);	//maybe at the reset time,APP has many writes request msg.
	}
	rtos_get_semaphore(&s_sdio_memcpy_sema, 0);	//force read out the semaphore, maybe lost APP TX request sema, then it can't TX data at once.
	bk_dma_stop(s_sdio_rx_dma_chan);
	bk_dma_stop(s_sdio_tx_dma_chan);
	//clear TX info if not complete
#if CONFIG_GPIO_NOTIFY_TRANSACTION_EN
	sdio_slave_clear_notify_host();
#endif
	sdio_hal_slave_set_tx_length(0);
	//NOTES:if TX not complete:the finished TX buffer maybe not notify to APP at once.
	sdio_hal_fifo_reset();
	sdio_hal_slave_cmd_start(1);
	rtos_enable_int(int_level);
	return 0;
}
/**
 * @brief	  Init the sdio driver as slave device.
 *
 * This API init the sdio driver as slave device.
 *	 - Init SDIO slave SW: Thread, Semaphore, Event.
 *	 - Init SDIO slave HW: GPIO,CLOCK,DMA,IRQ ...
 *
 * @attention 1. This API should be called before any other SDIO SLAVE APIs.
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_sdio_slave_driver_init(void)
{
	bk_err_t ret = BK_OK;
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	if(s_sdio_driver_is_init)
	{
		SDIO_LOG_INFO("has inited");
		return BK_OK;
	}
	ret = sdio_sw_init();
	if(ret != BK_OK)
	{
		SDIO_LOG_ERR("sw init fail");
		goto err_exit;
	}
	ret = sdio_slave_hw_init();
	if(ret != BK_OK)
	{
		SDIO_LOG_ERR("sw init fail");
		goto err_exit;
	}
	s_sdio_driver_is_init = true;

	bk_sdio_driver_dump_sdio_regs();
	bk_sdio_loopcheck_init();
	SDIO_LOG_DEBUG_FUNCTION_EXIT();

	return ret;
err_exit:
	return ret;
}
static bk_err_t sdio_sw_deinit(void)
{
	bk_err_t ret = BK_OK;
	if(s_sdio_msg_que)
	{
		rtos_deinit_queue(&s_sdio_msg_que);
		if (BK_OK != ret)
		{
			SDIO_LOG_ERR("deinit queue ret=%d", ret);
			return ret;
		}
		s_sdio_msg_que = NULL;
	}
	if(s_sdio_thread)
	{
		ret = rtos_delete_thread(&s_sdio_thread);
		if (BK_OK != ret)
		{
			SDIO_LOG_ERR("delete thread ret=%d", ret);
			return ret;
		}
		s_sdio_msg_que = NULL;
	}
	return BK_OK;
}
static bk_err_t sdio_hw_deinit(void)
{
	bk_err_t ret = BK_OK;
	sdio_hal_set_host_slave_mode(SDIO_SLAVE_MODE);
	//clock
	sys_hal_set_sdio_clk_en(false);
	sys_hal_set_sdio_clk_sel((uint32_t)SDIO_CLK_XTL);
	sys_hal_set_sdio_clk_div((uint32_t)SDIO_CLK_DIV_1);
#if 0	//has config in gpio_map.h
	//pin
	ret = sdio_gpio_deinit();
	if(ret != BK_OK)
	{
		SDIO_LOG_ERR("gpio deinit");
		return ret;
	}
#endif
	//TODO:
#if CONFIG_GPIO_NOTIFY_TRANSACTION_EN
	sdio_gpio_notify_transaction_deinit(GPIO_SDIO_NOTIFY_HOST, GPIO_SDIO_HOST_WAKEUP_SLAVE);
#endif
	//dma
#if CONFIG_SDIO_GDMA_EN
	ret = sdio_dma_deinit();
	if(ret != BK_OK)
	{
		SDIO_LOG_ERR("dma deinit");
		return ret;
	}
#endif
	//interrupt
	bk_int_isr_unregister(INT_SRC_SDIO);
#if	((CONFIG_CPU_CNT > 1) && CONFIG_SYS_CPU1)
	sys_driver_set_cpu1_sdio_int_en(false);
#else
	sys_driver_set_cpu0_sdio_int_en(false);
#endif
	return ret;
}
/**
 * @brief	  Deinit the sdio driver as slave device.
 *
 * This API deinit the sdio driver as slave device.
 *	 - Deinit SDIO slave HW: GPIO,DMA,IRQ ...
 *	 - Deinit SDIO slave SW: Thread, Queue...
 *
 * @attention 1. This API should be called after any other SDIO SLAVE APIs.
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t sdio_slave_driver_deinit(void)
{
	bk_err_t ret = BK_OK;
	uint32_t int_level = 0;
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	int_level = rtos_disable_int();
	ret = sdio_sw_deinit();
	if (BK_OK != ret)
	{
		SDIO_LOG_ERR("sw deinit ret=%d", ret);
		rtos_enable_int(int_level);
		return ret;
	}
	ret = sdio_hw_deinit();
	if (BK_OK != ret)
	{
		SDIO_LOG_ERR("hw deinit ret=%d", ret);
		rtos_enable_int(int_level);
		return ret;
	}
	bk_sdio_driver_dump_sdio_regs();
	SDIO_LOG_DEBUG_FUNCTION_EXIT();
	rtos_enable_int(int_level);
	return ret;
}
