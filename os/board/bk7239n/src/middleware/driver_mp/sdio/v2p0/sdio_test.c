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

#include "bk_uart.h"

#include "sdio_utils.h"
#include "sdio_slave_driver.h"
#include "sdio_test.h"
#if defined(CONFIG_TRNG_SUPPORT)
#include "driver/trng.h"
#endif
#include <driver/aon_rtc.h>
#include "driver/wdt.h"
#include "bk_wdt.h"


#if defined(CONFIG_SDIO_TEST_EN)

static bool s_sdio_test_inited = false;
static beken_queue_t s_sdio_test_msg_que = NULL;
static beken_thread_t s_test_sdio_thread = NULL;

static volatile uint8_t s_sdio_test_main_case;
static volatile uint8_t s_sdio_test_sub_case;

#define SDIO_CHAN_MAX_CNT 2

//static beken_thread_t s_sdio_test_thread_handle = NULL;

#define SDIO_BUFFER_SIZE 2048

//tx/rx has seperate thread, then it can random tx/rx, on only one thread, tx/rx has to one by one
static beken_thread_t s_tx_thread_handle[SDIO_CHAN_MAX_CNT];
static beken_queue_t s_sdio_ch_x_tx_msg_que[SDIO_CHAN_MAX_CNT];
static volatile uint32_t s_tx_tp_sub_case[SDIO_CHAN_MAX_CNT];

static beken_thread_t s_rx_thread_handle[SDIO_CHAN_MAX_CNT];
static beken_queue_t s_sdio_ch_x_rx_msg_que[SDIO_CHAN_MAX_CNT];
static volatile uint32_t s_rx_tp_sub_case[SDIO_CHAN_MAX_CNT];

static sdio_tp_statisic_t s_tx_tp_statistic[SDIO_CHAN_MAX_CNT];
static sdio_tp_statisic_t s_rx_tp_statistic[SDIO_CHAN_MAX_CNT];
#if defined(CONFIG_TRNG_SUPPORT)
static uint32_t wait_ms = 3000;
static uint32_t rx_wait_ms = 0;
#endif

#if 0
static bk_err_t sdio_test_tx_cb(sdio_node_ptr_t head_p, sdio_node_ptr_t tail_p, uint32_t count)
{
	bk_err_t ret = BK_OK;
	sdio_node_ptr_t cur_p = head_p;
	uint32_t i = count, j = 0;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	//check tx data after finish
	while(cur_p && (i > 0))
	{
		for(j = 0; j < cur_p->len; j += 4)
		{
			SDIO_LOG_INFO("0x%08x ", *(uint32_t *)((uint8_t *)cur_p + (sizeof(sdio_node_t)) + j));
			if(j % 16 == 0)
				SDIO_LOG_INFO("\r\n");
		}

		cur_p = cur_p->next;
		i--;
	}

	//TODO:release buffer
	//WARNING: Application should should use self CHAN_ID and CHAN_DIRECT.
	bk_sdio_chan_push_free_list(SDIO_CHAN_PLATFORM, SDIO_CHAN_TX, head_p, tail_p, count);

	//notify app task finish read

	SDIO_LOG_DEBUG_FUNCTION_EXIT();
	return ret;
}
#else
static bk_err_t sdio_test_tx_cb( sdio_node_ptr_t head_p, sdio_node_ptr_t tail_p, uint32_t count)
{
	bk_err_t ret = BK_OK;
	sdio_chan_id_t chan_id = SDIO_CHAN_PLATFORM;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	if(s_sdio_ch_x_tx_msg_que[chan_id % SDIO_CHAN_MAX_CNT])
	{

		sdio_test_tp_msg_t msg;

		msg.param1 = chan_id % SDIO_CHAN_MAX_CNT;
		msg.param2 = (uint32_t)head_p;
		msg.param3 = (uint32_t)tail_p;
		msg.param4 = count;

		ret = rtos_push_to_queue(&s_sdio_ch_x_tx_msg_que[chan_id % SDIO_CHAN_MAX_CNT], &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret)
		{
			SDIO_LOG_ERR("sdio test tx msg failed\r\n");
		}

	}
	else	//sync:
	{
		//TODO:release buffer to free list mem-pool
		//WARNING: Application should should use self CHAN_ID and CHAN_DIRECT.
		ret = bk_sdio_chan_push_free_list(chan_id, SDIO_CHAN_TX, head_p, tail_p, count);
		if(ret)
		{
			SDIO_LOG_ERR("err release buff");
		}
	}

	SDIO_LOG_DEBUG_FUNCTION_EXIT();

	//notify app task finish write

	return ret;
}
#endif



static bk_err_t sdio_test_tx_init(uint32_t chan_id, uint32_t buf_cnt, uint32_t buf_size)
{
	bk_err_t ret = BK_OK;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	ret = bk_sdio_init_channel(chan_id, SDIO_CHAN_TX, buf_cnt, buf_size);
	if(ret != BK_OK)
	{
		SDIO_LOG_ERR("init channel");
		return ret;
	}
	bk_sdio_register_chan_cb(chan_id, SDIO_CHAN_TX, (sdio_chan_cb_t)sdio_test_tx_cb);

	SDIO_LOG_DEBUG_FUNCTION_EXIT();
	return ret;
}

////////////////////////////////////////////

volatile uint8_t g_sdio_log_level = SDIO_LOG_DEBUG_LEVEL;

void sdio_set_log_level(uint8_t level)
{
	SDIO_LOG_ERR("%s:%d", __func__, level);
	g_sdio_log_level = level;
}


void sdio_set_tx_delay_time(uint32_t time_ms){
	wait_ms = time_ms;
}

void sdio_set_rx_delay_time(uint32_t time_ms){
	rx_wait_ms = time_ms;
}



extern uint32_t sdio_list_compute_total_length(sdio_list_t *list_p);


static void sdio_test_tx_tp_thread(void *arg)
{
	bk_err_t ret = BK_OK;
	sdio_chan_id_t chan_id = (sdio_chan_id_t)arg;
	uint32_t val_test[SDIO_CHAN_MAX_CNT] = {0};
	struct timeval rtc_start_time = {0, 0};

#if (defined(CONFIG_TASK_WDT))
	bk_task_wdt_stop();
#endif

	while(1)
	{
		sdio_test_tp_msg_t msg;
		sdio_node_ptr_t buf_p = NULL;
		uint32_t len = SDIO_TEST_BUFFER_SIZE * SDIO_TEST_BUFFER_CNT;
		uint32_t size = 0;

		if(s_tx_tp_sub_case[chan_id] == 0)
		{
#if (defined(CONFIG_TRNG_SUPPORT))
			//wait_ms = bk_rand() % 1000;
			//rtos_delay_milliseconds(wait_ms);
#endif

			if(s_tx_tp_statistic[chan_id].end_time)
			{
				sdio_set_log_level(SDIO_LOG_DEBUG_LEVEL);

				SDIO_LOG_ERR("TP:start_t=%u,end_t=%u, period_t=%u, cnt=%d,len=%d", 
							(uint32_t)s_tx_tp_statistic[chan_id].start_time,
							(uint32_t)s_tx_tp_statistic[chan_id].end_time,
							(uint32_t)(s_tx_tp_statistic[chan_id].end_time - s_tx_tp_statistic[chan_id].start_time),
							s_tx_tp_statistic[chan_id].payload_cnt,
							s_tx_tp_statistic[chan_id].total_len);
			}

			s_tx_tp_statistic[chan_id].start_time = 0;
			s_tx_tp_statistic[chan_id].end_time = 0;

			continue;
		}
		else if(s_tx_tp_statistic[chan_id].start_time == 0)	//start, first entry
		{
			sdio_set_log_level(SDIO_LOG_ERR_LEVEL);
			//s_tx_tp_statistic[chan_id].start_time = riscv_get_mtimer();
			bk_rtc_gettimeofday(&rtc_start_time, 0);
			s_tx_tp_statistic[chan_id].start_time = 1000000 * (rtc_start_time.tv_sec) + rtc_start_time.tv_usec;
		}

		//request write buf
		ret = bk_sdio_chan_pop_free_node(chan_id, SDIO_CHAN_TX, &buf_p, &size);
		if((ret == BK_OK) && buf_p)
		{
			s_tx_tp_statistic[chan_id].payload_cnt++;

#if (defined(CONFIG_TRNG_SUPPORT))
			//len = bk_rand() % SDIO_TEST_BUFFER_SIZE;
#endif
			SDIO_LOG_INFO("pop node, len = %d, size = %d\n", len, size);
			len = len < size? len : size;


			uint32_t *ptmp = (uint32_t *)((uint8_t *)buf_p + sizeof(sdio_node_t));

			for (int i = 0; i < len/4; i++) {
				//*ptmp = val_test[chan_id] + 0x10000000 * (chan_id);
				*ptmp = val_test[chan_id] + (0x90000000);
				ptmp++;
			}
			SDIO_LOG_INFO("val_test[%d] = 0x%x\n", chan_id, val_test[chan_id]);
			val_test[chan_id]++;

#if 0
			buf_p->pbd.len = len + sizeof(sdio_sw_pbd_t);	//SDIO ASIC will read this value to SDIO FIFO
			buf_p->sw_pbd.len = len;		//valid data size, exclude sdio_sw_pbd_t
			buf_p->sw_pbd.type = sdio_get_core_id();
			buf_p->sw_pbd.sub_type = chan_id;
#else
			buf_p->len = len;
#endif

#if (defined(CONFIG_TRNG_SUPPORT))
			//wait_ms = bk_rand() % 20;
			rtos_delay_milliseconds(wait_ms);

#endif
			//request read
#if defined(CONFIG_SDIO_SYNC_TRANS)
			ret = bk_sdio_slave_sync_write(chan_id, buf_p, buf_p, 1, BEKEN_WAIT_FOREVER);
#else
			ret = bk_sdio_slave_sync_write(chan_id, buf_p, buf_p, 1);
#endif
			if(ret)
			{
				SDIO_LOG_ERR("write fail");
			}
		}
		else
		{
			SDIO_LOG_ERR("no free buff");
		}
		//wait write finish
		ret = rtos_pop_from_queue(&s_sdio_ch_x_tx_msg_que[chan_id], &msg, SDIO_TEST_TP_MSG_WAIT_TIME);
		if (kNoErr == ret)
		{
			sdio_list_t list;
			//deal APP data
			list.head = (sdio_node_ptr_t)msg.param2;
			list.tail = (sdio_node_ptr_t)msg.param3;
			list.count = msg.param4;
			s_tx_tp_statistic[chan_id].total_len += sdio_list_compute_total_length(&list);

			//release buffer
			ret = bk_sdio_chan_push_free_list(chan_id, SDIO_CHAN_TX, list.head, list.tail, list.count);
		} else {
			SDIO_LOG_ERR("TX Tp rtos_pop_from_queue error\r\n");
		}
	}

	SDIO_LOG_DEBUG_FUNCTION_EXIT();
}



static bk_err_t sdio_test_tx_tp_init(uint32_t chan_id, uint32_t buf_cnt, uint32_t buf_size)
{
	bk_err_t ret = BK_OK;
	char str_name[64];

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
#if (defined(CONFIG_TRNG_SUPPORT))
	bk_trng_start();
#endif

	//tx hasn't init thread
	if(s_tx_thread_handle[chan_id] == NULL)
	{
		memset(str_name, 0, sizeof(str_name));
		snprintf(str_name, sizeof(str_name), "%s_ch%d_dir%d", SDIO_TEST_TP_THREAD_NAME, (int32_t)chan_id, SDIO_CHAN_TX);
		ret = rtos_create_thread(
				&s_tx_thread_handle[chan_id],
				SDIO_TEST_TX_TP_PRIORITY+chan_id,
				str_name,
				sdio_test_tx_tp_thread,
				SDIO_TEST_TX_TP_THREAD_STACK_SIZE,
				(beken_thread_arg_t)chan_id
			);
		if (kNoErr != ret)
		{
			SDIO_LOG_ERR("init thread ret=%d", ret);
			return ret;
		}
	}

	//tx notify queue init
	if(s_sdio_ch_x_tx_msg_que[chan_id] == NULL)
	{
		memset(str_name, 0, sizeof(str_name));
		snprintf(str_name, sizeof(str_name), "%s_ch%d", SDIO_TEST_TX_TP_MSG_QUEUE_NAME, (int32_t)chan_id);
		ret = rtos_init_queue(&s_sdio_ch_x_tx_msg_que[chan_id],
								str_name,
								sizeof(sdio_test_tp_msg_t),
								SDIO_TEST_TX_TP_MSG_QUEUE_COUNT
							);
		if (kNoErr != ret)
		{
			SDIO_LOG_ERR("init queue ret=%d", ret);
			return ret;
		}
	}

	ret = sdio_test_tx_init(chan_id, buf_cnt, buf_size);
	if (kNoErr != ret)
	{
		SDIO_LOG_ERR("tx init fail=%d", ret);
		return ret;
	}

	return ret;
}



////////////////////////////////////////////

static bk_err_t sdio_test_tx_sub_case(SDIO_TEST_MSG_T *msg_p)
{
	bk_err_t ret = BK_OK;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();



	switch(msg_p->sub_case)
	{
		//chan id, buf_cnt, buf_size
		case SDIO_TEST_TX_INIT:
		{
			sdio_chan_id_t id  = msg_p->param1;
			uint32_t buf_size = msg_p->param2;
			uint32_t buf_cnt = msg_p->param3;
		
			ret = sdio_test_tx_init(id, buf_cnt, buf_size);
			break;
		}

		//len, value
		case SDIO_TEST_TX_SINGLE_PACKET:
		{
			sdio_node_ptr_t buf_p = NULL;
			uint32_t len = msg_p->param1;
			uint32_t value = (uint32_t)msg_p->param2;
			uint32_t size;
			
			ret = bk_sdio_chan_pop_free_node(SDIO_CHAN_PLATFORM, SDIO_CHAN_TX, &buf_p, &size);
			if((ret == BK_OK) && buf_p)
			{
				//fill data
				if(len > size)
				{
					len = size;
					SDIO_LOG_ERR("max packet size=%d, len=%d", size, len);
				}

if(value == 12345678) {

#if 0
					for (int i = 0; i < len; i++) {
						//buf_p[i]
						*((uint8_t *)(buf_p + sizeof(sdio_node_t) + i)) = i & 0xff;
					}

#else
					memset((uint8_t *)buf_p + sizeof(sdio_node_t), 0, len);

					uint8_t *send_data = (uint8_t *)os_zalloc(len);
					if (send_data == NULL) {
						SDIO_LOG_INFO("send buffer malloc failed\r\n");
						break;
					}
					for (int i = 0; i < len; i++) {
						send_data[i] = i & 0xff;
					}
					memcpy((uint8_t *)buf_p + sizeof(sdio_node_t), send_data, len);
					os_free(send_data);
#endif

} else {
				memset((uint8_t *)buf_p + sizeof(sdio_node_t), value, len);
}


				buf_p->len = len;
				
				//request write
#if defined(CONFIG_SDIO_SYNC_TRANS)
				ret = bk_sdio_slave_sync_write(SDIO_CHAN_PLATFORM, buf_p, buf_p, 1, BEKEN_WAIT_FOREVER);
#else
				ret = bk_sdio_slave_sync_write(SDIO_CHAN_PLATFORM, buf_p, buf_p, 1);
#endif
			}
			else
			{
				SDIO_LOG_ERR("no free buff");
				ret = BK_FAIL;
			}

			break;
		}
	
		case SDIO_TEST_TX_MANY_SINGLE_PACKETS:
		{
			break;
		}
	
		case SDIO_TEST_TX_MULTI_PACKETS:
		{
			break;
		}

		//chan id, buf_cnt, buf_size
		case SDIO_TEST_TX_TP_INIT:
		{
			sdio_chan_id_t chan_id  = msg_p->param1;
			uint32_t buf_size = msg_p->param2;
			uint32_t buf_cnt = msg_p->param3;

			ret = sdio_test_tx_tp_init(chan_id, buf_cnt, buf_size);
			break;
		}

		//len, value
		case SDIO_TEST_TX_TP_START:
		{
			sdio_chan_id_t chan_id  = msg_p->param1 % SDIO_CHAN_MAX_CNT;
			wait_ms = msg_p->param2;
			s_tx_tp_sub_case[chan_id] = 1;
			break;
		}

		case SDIO_TEST_TX_TP_STOP:
		{
			sdio_chan_id_t chan_id  = msg_p->param1 % SDIO_CHAN_MAX_CNT;
			struct timeval rtc_end_time = {0, 0};
			bk_rtc_gettimeofday(&rtc_end_time, 0);
			s_tx_tp_statistic[chan_id].end_time = 1000000 * (rtc_end_time.tv_sec) + rtc_end_time.tv_usec;

			s_tx_tp_sub_case[chan_id] = 0;
			break;
		}

#if defined(CONFIG_SDIO_SYNC_TRANS)
		case SDIO_TEST_TX_SINGLE_BUF:
		{
			extern bk_err_t bk_sdio_slave_tx(char* buf_p, uint32_t len, uint32_t timeout_ms);

			uint32_t timeout = 20000;
			uint32_t buf_len = msg_p->param1;
			uint8_t *send_data = (uint8_t *)os_zalloc(buf_len);
			if (send_data == NULL) {
				SDIO_LOG_ERR("send buffer malloc failed\r\n");
				return ret;
			}
			for (int i = 0; i < buf_len; i++) {
				send_data[i] = i & 0xff;
			}
			bk_sdio_slave_tx((char* )send_data, buf_len, timeout);
			if (send_data) {
				os_free(send_data);
			}
			send_data = NULL;
			SDIO_LOG_INFO("sdio send raw data,  data_len=%d\n", buf_len);

			break;
		}
#endif

		default:
			break;
	}

	SDIO_LOG_DEBUG_FUNCTION_EXIT();

	return ret;
}


#if 0
static bk_err_t sdio_test_rx_cb(sdio_node_ptr_t head_p, sdio_node_ptr_t tail_p, uint32_t count)
{
	bk_err_t ret = BK_OK;
	sdio_node_ptr_t cur_p = head_p;
	uint32_t i = count, j = 0;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	//deal data
	while(cur_p && (i > 0))
	{
		for(j = 0; j < cur_p->len; j += 4)
		{
			SDIO_LOG_INFO("0x%08x ", *(uint32_t *)((uint8_t *)cur_p + (sizeof(sdio_node_t)) + j));
			if(j % 16 == 0)
				SDIO_LOG_INFO("\r\n");
		}

		cur_p = cur_p->next;
		i--;
	}

	//TODO:release buffer to free list mem-pool
	//WARNING: Application should should use self CHAN_ID and CHAN_DIRECT.
	ret = bk_sdio_chan_push_free_list(SDIO_CHAN_PLATFORM, SDIO_CHAN_RX, head_p, tail_p, count);

	SDIO_LOG_DEBUG_FUNCTION_EXIT();
	return ret;
}

#else

static bk_err_t sdio_test_rx_cb( sdio_node_ptr_t head_p, sdio_node_ptr_t tail_p, uint32_t count)
{
	bk_err_t ret = BK_OK;
	static uint32_t rx_val_test[SDIO_CHAN_MAX_CNT] = {0};
	int print_len = 2048;
	sdio_chan_id_t chan_id = SDIO_CHAN_PLATFORM;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	//async: notify APP rx thread, default usage method
	if(s_sdio_ch_x_rx_msg_que[chan_id % SDIO_CHAN_MAX_CNT])
	{

		//dump received data to check data validation
		sdio_node_ptr_t cur_p = head_p;
		uint32_t i = count, j = 0;
		while(cur_p && (i > 0))
		{
//not print received data by default
#if 0
			SDIO_LOG_INFO("core_id=0x%x, payload cur_p=0x%08x \r\n", chan_id, cur_p);
			for(j = 0; j < print_len; j += sizeof(uint32_t))
			{
				SDIO_LOG_INFO("0x%08x ", *(uint32_t *)((uint8_t *)cur_p + (sizeof(sdio_node_t)) + j));
				if(j && (j % 16 == 0))
					SDIO_LOG_INFO("\r\n");
			}
#else
			for (j = 4; j < print_len - 4; j += sizeof(uint32_t)) {
				if(*(uint32_t *)((uint8_t *)cur_p + (sizeof(sdio_node_t)) + j) != (rx_val_test[chan_id] + 0x10000000 * (chan_id))) {
					SDIO_LOG_INFO("rx_tp_cb error, cur_p_word[%d] = 0x%x, correct value is 0x%x.\n" , j, *(uint32_t *)((uint8_t *)cur_p + (sizeof(sdio_node_t)) + j), (rx_val_test[chan_id] + 0x10000000 * (chan_id)) );
				}

			}
#endif

			rx_val_test[chan_id]++;

			cur_p = (sdio_node_ptr_t)cur_p->next;
			i--;
		}
		/////////////////

		sdio_test_tp_msg_t msg;

		msg.param1 = chan_id % SDIO_CHAN_MAX_CNT;
		msg.param2 = (uint32_t)head_p;
		msg.param3 = (uint32_t)tail_p;
		msg.param4 = count;

		ret = rtos_push_to_queue(&s_sdio_ch_x_rx_msg_que[chan_id % SDIO_CHAN_MAX_CNT], &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret)
		{
			SDIO_LOG_ERR("sdio test rx msg failed\r\n");
		}

	}
	else	//sync:
	{

		sdio_node_ptr_t cur_p = head_p;
		uint32_t i = count, j = 0;

		//deal data
		while(cur_p && (i > 0))
		{
			SDIO_LOG_INFO("sync: chan_id=0x%x, payload cur_p=0x%08x \r\n", chan_id, cur_p);

			for(j = 0; j < print_len; j += sizeof(uint32_t))
			{

				SDIO_LOG_INFO("0x%08x ", *(uint32_t *)((uint8_t *)cur_p + (sizeof(sdio_node_t)) + j));

				if(j && (j % 16 == 0))
					SDIO_LOG_INFO("\r\n");
			}

#if 0
			extern uint32_t looptest_flag;
			if (looptest_flag == 1) {
				for (j = 0; j < (cur_p->sw_pbd.len - 4); j++) {
					if( *((uint8_t *)cur_p + (sizeof(sdio_node_t)) + j) != ((j + 0x84) % 256)) {
						SDIO_LOG_INFO("loopback error, cur_p_byte[%d] = 0x%x, correct value is 0x%x.\n" , j, *((uint8_t *)cur_p + (sizeof(sdio_node_t)) + j), ((j + 0x80) % 256) );
					}
				}
			}
#endif

			SDIO_LOG_INFO("\r\n");

			cur_p = (sdio_node_ptr_t)cur_p->next;
			i--;
		}

		//TODO:release buffer to free list mem-pool
		//WARNING: Application should should use self CHAN_ID and CHAN_DIRECT.
		ret = bk_sdio_chan_push_free_list(chan_id, SDIO_CHAN_RX, head_p, tail_p, count);

	}

	SDIO_LOG_DEBUG_FUNCTION_EXIT();

	return ret;
}

#endif


static bk_err_t sdio_test_rx_init(uint32_t chan_id, uint32_t buf_cnt, uint32_t buf_size)
{
	bk_err_t ret = BK_OK;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	ret = bk_sdio_init_channel(chan_id, SDIO_CHAN_RX, buf_cnt, buf_size);
	if(ret != BK_OK)
	{
		SDIO_LOG_ERR("init channel");
		return ret;
	}

	ret = bk_sdio_register_chan_cb(chan_id, SDIO_CHAN_RX, (sdio_chan_cb_t)sdio_test_rx_cb);

	SDIO_LOG_DEBUG_FUNCTION_EXIT();
	return ret;
}

///////////////////////////////////////////////////////////



//one channel one thread
static void sdio_test_rx_tp_thread(void *arg)
{
	bk_err_t ret = BK_OK;
	sdio_chan_id_t chan_id = (sdio_chan_id_t)arg%SDIO_CHAN_MAX_CNT;
	struct timeval rtc_start_time = {0, 0};

#if (defined(CONFIG_TASK_WDT))
	bk_task_wdt_stop();
#endif

	while(1)
	{
		sdio_test_tp_msg_t msg;
		sdio_node_ptr_t buf_p = NULL;
		uint32_t len = SDIO_TEST_BUFFER_SIZE * SDIO_TEST_BUFFER_CNT ;
#if 0
		uint32_t value = 0;
#endif
		uint32_t size = 0;

#if (defined(CONFIG_TRNG_SUPPORT))
		//rx_wait_ms = bk_rand() % 1000;
#endif
		//rtos_delay_milliseconds(rx_wait_ms);

		if(s_rx_tp_sub_case[chan_id] == 0)	//stop status
		{
			if(s_rx_tp_statistic[chan_id].end_time)
			{
				sdio_set_log_level(SDIO_LOG_DEBUG_LEVEL);

				SDIO_LOG_ERR("TP:start_t=%u,end_t=%u,period_t=%u,cnt=%d,len=%d", 
							(uint32_t)s_rx_tp_statistic[chan_id].start_time,
							(uint32_t)s_rx_tp_statistic[chan_id].end_time,
							(uint32_t)(s_rx_tp_statistic[chan_id].end_time - s_rx_tp_statistic[chan_id].start_time),
							s_rx_tp_statistic[chan_id].payload_cnt,
							s_rx_tp_statistic[chan_id].total_len);
			}
			s_rx_tp_statistic[chan_id].start_time = 0;
			s_rx_tp_statistic[chan_id].end_time = 0;

			continue;
		}
		else if(s_rx_tp_statistic[chan_id].start_time == 0)	//start, first entry
		{
			sdio_set_log_level(SDIO_LOG_ERR_LEVEL);

			bk_rtc_gettimeofday(&rtc_start_time, 0);
			s_rx_tp_statistic[chan_id].start_time = 1000000 * (rtc_start_time.tv_sec) + rtc_start_time.tv_usec;
		}

		//request read buf
		ret = bk_sdio_chan_pop_free_node(chan_id, SDIO_CHAN_RX, &buf_p, &size);
		if((ret == BK_OK) && buf_p)
		{
			s_rx_tp_statistic[chan_id].payload_cnt++;

			len = len < size? len : size;
#if 0
			//fill dirty data
			if(value == 12345678)
			{
				for(uint32_t i = 0; i < len; i++)
					*(uint8_t *)((uint8_t *)buf_p + sizeof(sdio_node_t) + i) = i & 0xff;
			}
			else
			{
				memset((uint8_t *)buf_p + sizeof(sdio_node_t), value, len);
			}
#endif

#if 0
			buf_p->pbd.len = len + sizeof(sdio_sw_pbd_t);	//SDIO ASIC will read this value to SDIO FIFO
			buf_p->sw_pbd.len = len;		//valid data size, exclude sdio_sw_pbd_t
#else
			buf_p->len = len;
#endif


			//request read

#if defined(CONFIG_SDIO_SYNC_TRANS)
			ret = bk_sdio_slave_sync_read(chan_id, buf_p, buf_p, 1, BEKEN_WAIT_FOREVER);
#else
			ret = bk_sdio_slave_sync_read(chan_id, buf_p, buf_p, 1);
#endif
		}
		else
		{
#if (defined(CONFIG_TRNG_SUPPORT))
			//rx_wait_ms = bk_rand() % 1000;
#endif
			//SDIO_LOG_DEBUG("rx tp no free node, all nodes poped.");
			//SDIO_LOG_ERR("rx tp no free node, all nodes poped.");

			//rtos_delay_milliseconds(rx_wait_ms);
		}

		//wait read finish
#if (defined(CONFIG_TRNG_SUPPORT))
		//rx_wait_ms = bk_rand() % 10;
#endif
		ret = rtos_pop_from_queue(&s_sdio_ch_x_rx_msg_que[chan_id], &msg, SDIO_TEST_TP_MSG_WAIT_TIME);
		if (kNoErr == ret)
		{
			sdio_list_t list;
			//deal APP data
			list.head = (sdio_node_ptr_t)msg.param2;
			list.tail = (sdio_node_ptr_t)msg.param3;
			list.count = msg.param4;
			s_rx_tp_statistic[chan_id].total_len += sdio_list_compute_total_length(&list);

			//release buffer
			ret = bk_sdio_chan_push_free_list(chan_id, SDIO_CHAN_RX, list.head, list.tail, list.count);
		} else {
			//SDIO_LOG_ERR("RX rtos_pop_from_queue error\r\n");
		}
	}

	SDIO_LOG_DEBUG_FUNCTION_EXIT();
}




static bk_err_t sdio_test_rx_tp_init(uint32_t chan_id, uint32_t buf_cnt, uint32_t buf_size)
{
	bk_err_t ret = BK_OK;
	char str_name[64];

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	//rx hasn't init thread
	if(s_rx_thread_handle[chan_id] == NULL)
	{
		memset(str_name, 0, sizeof(str_name));
		snprintf(str_name, sizeof(str_name), "%s_ch%d_dir%d", SDIO_TEST_TP_THREAD_NAME, (int32_t)chan_id, SDIO_CHAN_RX);
		ret = rtos_create_thread(
				&s_rx_thread_handle[chan_id],
				SDIO_TEST_RX_TP_PRIORITY+chan_id,
				str_name,
				sdio_test_rx_tp_thread,
				SDIO_TEST_RX_TP_THREAD_STACK_SIZE,
				(beken_thread_arg_t)chan_id
			);
		if (kNoErr != ret)
		{
			SDIO_LOG_ERR("init thread ret=%d", ret);
			return ret;
		}
	}

	//rx notify queue init
	if(s_sdio_ch_x_rx_msg_que[chan_id] == NULL)
	{
		memset(str_name, 0, sizeof(str_name));
		snprintf(str_name, sizeof(str_name), "%s_ch%d", SDIO_TEST_RX_TP_MSG_QUEUE_NAME, (int32_t)chan_id);
		ret = rtos_init_queue(&s_sdio_ch_x_rx_msg_que[chan_id],
								str_name,
								sizeof(sdio_test_tp_msg_t),
								SDIO_TEST_RX_TP_MSG_QUEUE_COUNT
							);
		if (kNoErr != ret)
		{
			SDIO_LOG_ERR("init queue ret=%d", ret);
			return ret;
		}
	}

	ret = sdio_test_rx_init(chan_id, buf_cnt, buf_size);
	if (kNoErr != ret)
	{
		SDIO_LOG_ERR("rx init fail=%d", ret);
		return ret;
	}

	return ret;
}



///////////////////////////////////////////////////////////

static bk_err_t sdio_test_rx_sub_case(SDIO_TEST_MSG_T *msg_p)
{
	bk_err_t ret = BK_OK;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	switch(msg_p->sub_case)
	{
		//chan id, buf_cnt, buf_size
		case SDIO_TEST_RX_INIT:
		{
			sdio_chan_id_t id  = msg_p->param1;
			uint32_t buf_size = msg_p->param2;
			uint32_t buf_cnt = msg_p->param3;
		
			ret = sdio_test_rx_init(id, buf_cnt, buf_size);
			break;
		}
		
		//len, value
		case SDIO_TEST_RX_SINGLE_PACKET:
		{
			sdio_node_ptr_t buf_p = NULL;
			uint32_t len = msg_p->param1;
			uint8_t value = (uint8_t)msg_p->param2;
			uint32_t size = 0;

			ret = bk_sdio_chan_pop_free_node(SDIO_CHAN_PLATFORM, SDIO_CHAN_RX, &buf_p, &size);
			if((ret == BK_OK) && buf_p)
			{
				len = len < size? len : size;
				//fill dirty data
				memset((uint8_t *)buf_p + sizeof(sdio_node_t), value, len);
				buf_p->len = len;
				
				//request read
#if defined(CONFIG_SDIO_SYNC_TRANS)
				ret = bk_sdio_slave_sync_read(SDIO_CHAN_PLATFORM, buf_p, buf_p, 1, BEKEN_WAIT_FOREVER);
#else
				ret = bk_sdio_slave_sync_read(SDIO_CHAN_PLATFORM, buf_p, buf_p, 1);
#endif
			}
			else
			{
				SDIO_LOG_ERR("no free buff");
				ret = BK_FAIL;
			}

			break;
		}
	
		case SDIO_TEST_RX_MANY_SINGLE_PACKETS:
		{
			break;
		}
	
		case SDIO_TEST_RX_MULTI_PACKETS:
		{
			break;
		}
		

		//chan id, buf_cnt, buf_size
		case SDIO_TEST_RX_TP_INIT:
		{
			sdio_chan_id_t chan_id  = msg_p->param1;
			uint32_t buf_size = msg_p->param2;
			uint32_t buf_cnt = msg_p->param3;
			
			ret = sdio_test_rx_tp_init(chan_id, buf_cnt, buf_size);
			break;
		}

		//len, value
		case SDIO_TEST_RX_TP_START:
		{
			sdio_chan_id_t chan_id  = msg_p->param1 % SDIO_CHAN_MAX_CNT;
			rx_wait_ms = msg_p->param2;
			s_rx_tp_sub_case[chan_id] = 1;
			break;
		}

		case SDIO_TEST_RX_TP_STOP:
		{
			sdio_chan_id_t chan_id  = msg_p->param1 % SDIO_CHAN_MAX_CNT;
			struct timeval rtc_end_time = {0, 0};
			bk_rtc_gettimeofday(&rtc_end_time, 0);
			s_rx_tp_statistic[chan_id].end_time = 1000000 * (rtc_end_time.tv_sec) + rtc_end_time.tv_usec;
			s_rx_tp_sub_case[chan_id] = 0;
			break;
		}

#if defined(CONFIG_SDIO_SYNC_TRANS)
		case SDIO_TEST_RX_SINGLE_BUF:
		{
			extern bk_err_t bk_sdio_slave_rx(char* buf_p, uint32_t len, uint32_t timeout_ms);

			uint32_t timeout = 20000;
			uint32_t buf_len = msg_p->param1;
			uint8_t *recv_data = (uint8_t *)os_malloc(buf_len);
			if (recv_data == NULL) {
				SDIO_LOG_ERR("recv buffer malloc failed\r\n");
				return ret;
			}
			os_memset(recv_data, 0xff, buf_len);
			bk_sdio_slave_rx((char* )recv_data, buf_len, timeout);
			SDIO_LOG_INFO("sdio dma recv, data_len=%d\n",  buf_len);
			for (int i = 0; i < buf_len; i++) {
				SDIO_LOG_INFO("recv_buffer[%d]=0x%x\n", i, recv_data[i]);
			}
			if (recv_data) {
				os_free(recv_data);
			}
			recv_data = NULL;


			break;
		}
#endif
		default:
			break;
	}

	SDIO_LOG_DEBUG_FUNCTION_EXIT();
	return ret;
}

static bk_err_t sdio_test_send_msg(SDIO_TEST_MSG_T *msg_p)
{
	bk_err_t ret = BK_FAIL;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	if (s_sdio_test_msg_que)
	{
		ret = rtos_push_to_queue(&s_sdio_test_msg_que, msg_p, BEKEN_NO_WAIT);
		if (kNoErr != ret)
			SDIO_LOG_ERR("sdio test send msg failed\r\n");
	}

	SDIO_LOG_DEBUG_FUNCTION_EXIT();
	return ret;
}

/**
 * @brief	Set the sdio test case.
 *
 * This API set the sdio driver test case.
 *	 - It will tested with the selected case.
 *	 - Every case param has itself mean.
 *
 * @attention 1. This API should be called after sdio test inited.
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_sdio_set_test_case(
							uint8_t main_case,
							uint8_t sub_case,
							uint32_t param1,
							uint32_t param2,
							uint32_t param3,
							uint32_t param4
							)
{
	SDIO_TEST_MSG_T msg;

	if(main_case >= SDIO_TEST_MAX_MAIN_CASE)
	{
		SDIO_LOG_ERR("main case id %d is big then %d", main_case, SDIO_TEST_MAX_MAIN_CASE);
		return BK_FAIL;
	}

	msg.main_case = main_case;
	msg.sub_case = sub_case;
	msg.param1 = param1;
	msg.param2 = param2;
	msg.param3 = param3;
	msg.param4 = param4;

	return sdio_test_send_msg(&msg);
}

static void sdio_test_thread(void *arg)
{
	bk_err_t ret = BK_OK;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	while(1)
	{
		SDIO_TEST_MSG_T msg;

		ret = rtos_pop_from_queue(&s_sdio_test_msg_que, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == ret)
		{
			//
			switch(msg.main_case)
			{
				case SDIO_TEST_TX:
				{
					if(msg.sub_case >= SDIO_TEST_TX_MAX_SUB_CASE)
					{
						SDIO_LOG_ERR("sub case id %d is big then %d", msg.sub_case, SDIO_TEST_TX_MAX_SUB_CASE);
						break;
					}
					
					sdio_test_tx_sub_case(&msg);
					break;
				}

				case SDIO_TEST_RX:
				{
					if(msg.sub_case >= SDIO_TEST_RX_MAX_SUB_CASE)
					{
						SDIO_LOG_ERR("sub case id %d is big then %d", msg.sub_case, SDIO_TEST_RX_MAX_SUB_CASE);
						break;
					}
					
					sdio_test_rx_sub_case(&msg);
					break;
				}

				default:
					break;
			}
		}
	}

	SDIO_LOG_DEBUG_FUNCTION_EXIT();

	//TODO: Assert
}

/**
 * @brief	Init the sdio driver test program.
 *
 * This API init the sdio driver test program.
 *	 - It will create a task do selected test case.
 *	 - It can select test case by command line.
 *
 * @attention 1. This API should be called after sdio inited finish.
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_sdio_test_init(void)
{
	bk_err_t ret = BK_OK;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	if(s_sdio_test_inited)
	{
		SDIO_LOG_INFO("has inited");
		return ret;
	}

	ret = rtos_init_queue(
							&s_sdio_test_msg_que,
							SDIO_TEST_MSG_QUEUE_NAME,
							sizeof(SDIO_TEST_MSG_T),
							SDIO_TEST_MSG_QUEUE_COUNT
						);
	if (kNoErr != ret)
	{
		SDIO_LOG_ERR("init queue ret=%d", ret);
		goto err_exit;
	}

	ret = rtos_create_thread(
			&s_test_sdio_thread,
			SDIO_TEST_PRIORITY,
			SDIO_TEST_THREAD_NAME,
			sdio_test_thread,
			SDIO_TEST_THREAD_STACK_SIZE,
			NULL
		);
	if (kNoErr != ret)
	{
		SDIO_LOG_ERR("init thread ret=%d", ret);
		goto err_exit;
	}

	s_sdio_test_inited = true;

	SDIO_LOG_DEBUG_FUNCTION_EXIT();

	//return, or err_exit release resources caused bug.
	return ret;

err_exit:
	if(s_sdio_test_msg_que)
	{
		rtos_deinit_queue(&s_sdio_test_msg_que);
		s_sdio_test_msg_que = NULL;
	}

	if(s_test_sdio_thread)
	{
		rtos_delete_thread(&s_test_sdio_thread);
		s_test_sdio_thread = NULL;
	}

	return ret;
}

/**
 * @brief	Deinit the sdio driver test program.
 *
 * This API init the sdio driver test program.
 *	 - It will delete the test thread and queue.
 *
 * @attention 1. This API should be called after sdio inited finish.
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_sdio_test_deinit(void)
{
	bk_err_t ret = BK_OK;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	if(s_sdio_test_msg_que)
	{
		ret = rtos_deinit_queue(&s_sdio_test_msg_que);
		if (BK_OK != ret)
		{
			SDIO_LOG_ERR("deinit queue ret=%d", ret);
			return ret;
		}

		s_sdio_test_msg_que = NULL;
	}

	if(s_test_sdio_thread)
	{
		ret = rtos_delete_thread(&s_test_sdio_thread);
		if (BK_OK != ret)
		{
			SDIO_LOG_ERR("delete thread ret=%d", ret);
			return ret;
		}

		s_test_sdio_thread = NULL;
	}

	SDIO_LOG_DEBUG_FUNCTION_EXIT();

	return BK_OK;
}

#endif //CONFIG_SDIO_TEST_EN
