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
#if defined(CONFIG_SDIO_GDMA_EN)
#include "driver/dma.h"
#include <driver/hal/hal_dma_types.h>
#endif

#include "driver/sdio_types.h"
#include "sdio_hal.h"
#include "sdio_utils.h"
#include "sdio_slave_driver.h"
#include "sdio.h"

static bk_err_t sdio_slave_add_ongoing_buf_trans_len(sdio_chan_id_t chan_id, chan_direct_t direct, uint32_t trans_len);
static void sdio_send_msg(sdio_msg_t *msg_p);

#if defined(CONFIG_SDIO_V2P0)
static sdio_driver_t s_sdio_driver;
static bool s_sdio_driver_is_init = false;
static beken_queue_t s_sdio_msg_que = NULL;
static beken_thread_t s_sdio_thread = NULL;
#if defined(CONFIG_SDIO_SYNC_TRANS)
static beken_thread_t s_sdio_rx_thread = NULL;
sdio_rx_cb_t sdio_rx_hdl;

#endif
static uint32_t s_sdio_rx_bytes = 0;	//RX:how many bytes Host write at current read transaction
static uint32_t s_sdio_tx_bytes = 0;	//TX:how many bytes Host read
static dma_id_t s_sdio_tx_dma_chan = 2;  /**< SDIO tx dma channel */
static dma_id_t s_sdio_rx_dma_chan = 3;  /**< SDIO rx dma channel */

#define SDIO_RETURN_ON_NOT_INIT() do {\
		if (!s_sdio_driver_is_init) {\
			SDIO_LOGE("driver not init");\
			return BK_ERR_SDIO_NOT_INIT;\
		}\
	} while(0)

#define SDIO_RETURN_CHAN_ID(chan_id) do {\
		if (chan_id >= SDIO_CHAN_MAX_CNT) {\
			SDIO_LOGE("chan id %d", chan_id);\
			return BK_ERR_SDIO_CHAN_ID_INVALID;\
		}\
	} while(0)

#define SDIO_RETURN_CHAN_DIRECT(direct) do {\
		if (direct > SDIO_CHAN_RX) {\
			SDIO_LOGE("direct %d", direct);\
			return BK_ERR_SDIO_DIRECT_INVALID;\
		}\
	} while(0)


#if defined(CONFIG_SDIO_GDMA_EN)
void sdio_slave_dma_tx_finish(dma_id_t dma_id)
{
	sdio_msg_t msg;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	msg.id = SDIO_WRITE_TO_FIFO_FINISH;
	sdio_send_msg(&msg);
}

static bk_err_t sdio_tx_dma_set_src_buf(uint8_t *psrc, uint32_t count)
{
	bk_dma_set_transfer_len(s_sdio_tx_dma_chan, count);
	return bk_dma_set_src_addr(s_sdio_tx_dma_chan, (uint32_t)psrc, (uint32_t)(psrc+count));
	//BK_LOG_ON_ERR(bk_dma_set_transfer_len(s_sdio_tx_dma_chan, count));
}

static bk_err_t sdio_dma_tx_init(void)
{
	dma_config_t dma_config = {0};

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	dma_config.mode = DMA_WORK_MODE_SINGLE;
	dma_config.chan_prio = 0;

	dma_config.src.dev = DMA_DEV_DTCM;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	//dma_config.src.start_addr = (uint32) psrc;
	//dma_config.src.end_addr =  (uint32) psrc + count;
	dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;

	dma_config.dst.dev = DMA_DEV_SDIO;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
	dma_config.dst.addr_inc_en = DMA_ADDR_INC_DISABLE;
#if defined(CONFIG_SOC_BK7256XX)
	dma_config.dst.start_addr = (uint32_t) SDIO_REG0XB_ADDR;
	dma_config.dst.end_addr = (uint32_t) SDIO_REG0XB_ADDR;
#elif defined(CONFIG_SOC_BK7236XX) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX))
	dma_config.dst.start_addr = (uint32_t) SDIO_REG0XF_ADDR;
	dma_config.dst.end_addr = (uint32_t) SDIO_REG0XF_ADDR;
#endif
	s_sdio_tx_dma_chan = bk_dma_alloc(DMA_DEV_SDIO);
	SDIO_LOGI("s_sdio_tx_dma_chan = %d.\n", s_sdio_tx_dma_chan);

	BK_LOG_ON_ERR(bk_dma_init(s_sdio_tx_dma_chan, &dma_config));
	BK_LOG_ON_ERR(bk_dma_register_isr(s_sdio_tx_dma_chan, NULL, sdio_slave_dma_tx_finish));
	BK_LOG_ON_ERR(bk_dma_enable_finish_interrupt(s_sdio_tx_dma_chan));
#if defined(CONFIG_SPE) && (CONFIG_SPE == 1)
	BK_LOG_ON_ERR(bk_dma_set_dest_sec_attr(s_sdio_tx_dma_chan, DMA_ATTR_SEC));
	BK_LOG_ON_ERR(bk_dma_set_src_sec_attr(s_sdio_tx_dma_chan, DMA_ATTR_SEC));
#endif

	SDIO_LOG_DEBUG_FUNCTION_EXIT();	

	return BK_OK;
}

void sdio_slave_dma_rx_finish(dma_id_t dma_id)
{
	sdio_msg_t msg;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	msg.id = SDIO_READ_TO_MEMORY_FINISH;
	sdio_send_msg(&msg);
}

static bk_err_t sdio_rx_dma_set_dst_buf(uint8_t *pdst, uint32_t count)
{
	bk_dma_set_transfer_len(s_sdio_rx_dma_chan, count);
	return bk_dma_set_dest_addr(s_sdio_rx_dma_chan, (uint32_t)pdst, (uint32_t)(pdst+count));
}

static bk_err_t sdio_dma_rx_init()
{
	dma_config_t dma_config = {0};

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();	

	dma_config.mode = DMA_WORK_MODE_SINGLE;
	dma_config.chan_prio = 0;

	dma_config.src.dev = DMA_DEV_SDIO;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.src.addr_inc_en = DMA_ADDR_INC_DISABLE;
#if defined(CONFIG_SOC_BK7256XX)
	dma_config.src.start_addr = (uint32) SDIO_REG0XC_ADDR;
	dma_config.src.end_addr =  (uint32) SDIO_REG0XC_ADDR;
#elif defined(CONFIG_SOC_BK7236XX) || (defined(CONFIG_SOC_BK7239XX))|| (defined(CONFIG_SOC_BK7286XX))
	dma_config.src.start_addr = (uint32) SDIO_REG0X10_ADDR;
	dma_config.src.end_addr =  (uint32) SDIO_REG0X10_ADDR;
#endif
	dma_config.dst.dev = DMA_DEV_DTCM;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
	//dma_config.dst.start_addr = (uint32) pbuf;
	//dma_config.dst.end_addr = (uint32) pbuf + count;
	dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;

	s_sdio_rx_dma_chan = bk_dma_alloc(DMA_DEV_SDIO_RX);
	SDIO_LOGI("s_sdio_rx_dma_chan = %d.\n", s_sdio_rx_dma_chan);

	BK_LOG_ON_ERR(bk_dma_init(s_sdio_rx_dma_chan, &dma_config));
	//BK_LOG_ON_ERR(bk_dma_set_transfer_len(s_sdio_rx_dma_chan, count));
	BK_LOG_ON_ERR(bk_dma_register_isr(s_sdio_rx_dma_chan, NULL, sdio_slave_dma_rx_finish));
	BK_LOG_ON_ERR(bk_dma_enable_finish_interrupt(s_sdio_rx_dma_chan));
#if defined(CONFIG_SPE) && (CONFIG_SPE == 1)
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

#if defined(CONFIG_GPIO_NOTIFY_TRANSACTION_EN)
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
		SDIO_LOGE("set sema");
		return ret;
	}

	return ret;
}

/**
 * @brief	Pullup GPIO to notify host, slave will send data.
 *
 * This API Pullup GPIO to notify host, slave will send data.
 *	 - Pullup GPIO to notify host
 *	 - Slave wait "host send CMD52 to get slave will transaction data length".
 *     If 100ms doesn't get CMD52 mean's host timeout.
 *
 * @attention 1. This API should be called after SDIO init.
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t sdio_slave_notify_host(uint32_t timeout)
{
	bk_err_t ret = BK_OK;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	if(WAKEUP_LEVEL)
		bk_gpio_set_output_high(gpio_to_host_num);
	else
		bk_gpio_set_output_low(gpio_to_host_num);

	//wait here
	ret = rtos_get_semaphore(&s_sdio_host_ack, timeout);
	if(ret != BK_OK)
	{
		SDIO_LOGE("get sema");

        //clear wake host
        if(WAKEUP_LEVEL)
            bk_gpio_set_output_low(gpio_to_host_num);
        else
            bk_gpio_set_output_high(gpio_to_host_num);
		sdio_wake_host_fail();

		//TODO: Assert?
		return ret;
	}

	//clear wake host
	if(WAKEUP_LEVEL)
		bk_gpio_set_output_low(gpio_to_host_num);
	else
		bk_gpio_set_output_high(gpio_to_host_num);

	SDIO_LOG_DEBUG_FUNCTION_EXIT();

	return ret;
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
	gpio_dev_unmap(gpio_to_host_num);
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
	SDIO_LOGD("v=%d", allow_slp);
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
		SDIO_LOGD("cur_sts=%d,allow_sleep=%d", cur_sts, allow_sleep);
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
			SDIO_LOGD("wakeup:low to high");
			set_controller_wk_slp_status(0);
			set_last_receive_data_time();	//update wakeup time.

			sdio_slave_response_host_wake_slave_ack();

			bk_gpio_enable_input(gpio_from_host_num);
			bk_gpio_pull_up(gpio_from_host_num);
			bk_gpio_disable_interrupt(gpio_from_host_num);
		}
		else
		{
			SDIO_LOGD("sleep:high to low");
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
				SDIO_LOGE("cur_time=%d,last_time=%d", __func__, bk_get_tick(), last_rx_data_time);
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
		SDIO_LOGD("msg.id=%d,param=%d", msg_p->id, msg_p->param);
	}

	if (s_sdio_msg_que) {
		ret = rtos_push_to_queue(&s_sdio_msg_que, msg_p, BEKEN_NO_WAIT);
		if (kNoErr != ret)
			SDIO_LOGE("sdio send msg failed");
	}
}

static bk_err_t sdio_gpio_init(void)
{
#if defined(CONFIG_SDIO_4LINES_EN)
	return gpio_sdio_sel(GPIO_SDIO_MAP_MODE0);
#else
	return gpio_sdio_one_line_sel(GPIO_SDIO_MAP_MODE0);
#endif
}

static bk_err_t sdio_gpio_deinit(void)
{
	SDIO_LOGE("%s:TODO");
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
#if defined(CONFIG_SOC_BK7256XX)
		if(reg_addr != SDIO_REG0XC_ADDR)	//skip read data out.
#elif defined(CONFIG_SOC_BK7236XX) || (defined(CONFIG_SOC_BK7239XX))|| (defined(CONFIG_SOC_BK7286XX))
		if(reg_addr != SDIO_REG0X10_ADDR)	//skip read data out.
#endif
		{
			SDIO_LOG_DUMP("Reg0x%x=0x%x\r\n", (reg_addr - SDIO_LL_REG_BASE(0))/4, *(volatile uint32_t *)reg_addr);
		}
	}

	SDIO_LOG_DEBUG_FUNCTION_EXIT();
}

#if 0
static void sdio_driver_logout_cmd53_args(sdio_cmd53_arg_t cmd53_arg)
{
	//uint32_t reg_addr = 0;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	SDIO_LOGD("value=0x%x", cmd53_arg.v);

	SDIO_LOGD("count=%d", cmd53_arg.count);
#if defined(CONFIG_SDIO_SW_CHANNEL_EN)
	SDIO_LOGD("channel=%d", cmd53_arg.channel);
	SDIO_LOGD("start_packet=%d", cmd53_arg.start_packet);
	SDIO_LOGD("end_packet=%d", cmd53_arg.end_packet);
	SDIO_LOGD("sw_reserved=%d", cmd53_arg.sw_reserved);
#else
	SDIO_LOGD("addr=0x%08x", cmd53_arg.addr);
#endif
	SDIO_LOGD("block_mode=%d", cmd53_arg.block_mode);
	SDIO_LOGD("rw=%d", cmd53_arg.rw);
	//SDIO_LOGD("func=%d", cmd53_arg.func_num);
	//SDIO_LOGD("count=%d", cmd53_arg.op_mode);

	SDIO_LOG_DEBUG_FUNCTION_EXIT();
}
#endif

#if defined(CONFIG_SDIO_CHANNEL_EN)

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
#if defined(CONFIG_SDIO_BIDIRECT_CHANNEL_EN)
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
	sdio_list_dump_info(head_p, tail_p, count);
	sdio_list_dump_info(chan_buf_p->free_list.head, chan_buf_p->free_list.tail, chan_buf_p->free_list.count);
	sdio_list_push_list(&chan_buf_p->free_list, head_p, tail_p, count);
	sdio_list_dump_info(chan_buf_p->free_list.head, chan_buf_p->free_list.tail, chan_buf_p->free_list.count);

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
#if defined(CONFIG_SDIO_BIDIRECT_CHANNEL_EN)
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
		SDIO_LOGE("null pointer");
		return BK_ERR_SDIO_NULL_POINTER_PARAM;
	}

	int_level = rtos_disable_int();
	chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[buf_index];
	sdio_list_dump_info(chan_buf_p->free_list.head, chan_buf_p->free_list.tail, chan_buf_p->free_list.count);

	if(chan_buf_p->free_list.count)
	{
		sdio_node_ptr_t tmp_node_p;

		ret = sdio_list_pop_node(&chan_buf_p->free_list, &tmp_node_p);
		if(ret != BK_OK)
		{
			SDIO_LOGE("no free buf");
			rtos_enable_int(int_level);
			return BK_ERR_SDIO_NO_BUFFER;
		}
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

	SDIO_LOGD("chan-%d TX/RX=%d get buf:buf=0x%08x,size=%d", chan_id, direct, *node_p, *size_p);
	sdio_list_dump_info(chan_buf_p->free_list.head, chan_buf_p->free_list.tail, chan_buf_p->free_list.count);
	rtos_enable_int(int_level);

	SDIO_LOG_DEBUG_FUNCTION_EXIT();

	return BK_OK;
}

void sdio_chan_push_ongoing_node(
										sdio_chan_id_t chan_id, 
										chan_direct_t direct,
										sdio_node_ptr_t head_p,
										sdio_node_ptr_t tail_p,
										uint32_t count
										)
{
	uint32_t int_level = 0;
#if defined(CONFIG_SDIO_BIDIRECT_CHANNEL_EN)
	uint32_t buf_index = direct;
#else
	uint32_t buf_index = 0;
#endif
	sdio_chan_buf_t *chan_buf_p = NULL;

	//TODO: check chan_id and direct is whether valid

	int_level = rtos_disable_int();
	chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[buf_index];
	sdio_list_dump_info(chan_buf_p->ongoing_list.head, chan_buf_p->ongoing_list.tail, chan_buf_p->ongoing_list.count);
	sdio_list_push_list(&chan_buf_p->ongoing_list, head_p, tail_p, count);
	sdio_list_dump_info(chan_buf_p->ongoing_list.head, chan_buf_p->ongoing_list.tail, chan_buf_p->ongoing_list.count);

	rtos_enable_int(int_level);
}

bk_err_t sdio_chan_pop_ongoing_node(sdio_chan_id_t chan_id, chan_direct_t direct, sdio_node_ptr_t *node_p)
{
	bk_err_t ret = BK_OK;
	uint32_t int_level = 0;
#if defined(CONFIG_SDIO_BIDIRECT_CHANNEL_EN)
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
	sdio_list_dump_info(chan_buf_p->ongoing_list.head, chan_buf_p->ongoing_list.tail, chan_buf_p->ongoing_list.count);
	if(chan_buf_p->ongoing_list.count)
	{
		sdio_node_ptr_t tmp_node_p;
		ret = sdio_list_pop_node(&chan_buf_p->ongoing_list, &tmp_node_p);
		if(ret != BK_OK)
		{
			SDIO_LOGE("no transaction finish node");
			return BK_ERR_SDIO_NO_TRANSACTION_FINISH_NODE;
		}

		*node_p = tmp_node_p;
	}
	else
	{
		rtos_enable_int(int_level);
		return BK_FAIL;
	}

	sdio_list_dump_info(chan_buf_p->ongoing_list.head, chan_buf_p->ongoing_list.tail, chan_buf_p->ongoing_list.count);
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
#if defined(CONFIG_SDIO_BIDIRECT_CHANNEL_EN)
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
	sdio_list_dump_info(chan_buf_p->finish_list.head, chan_buf_p->finish_list.tail, chan_buf_p->finish_list.count);

	sdio_list_push_list(&chan_buf_p->finish_list, head_p, tail_p, count);

	sdio_list_dump_info(chan_buf_p->finish_list.head, chan_buf_p->finish_list.tail, chan_buf_p->finish_list.count);
	rtos_enable_int(int_level);

	SDIO_LOG_DEBUG_FUNCTION_EXIT();

	return BK_OK;
}

bk_err_t sdio_chan_pop_finish_node(sdio_chan_id_t chan_id, chan_direct_t direct, sdio_node_ptr_t *node_p)
{
	bk_err_t ret = BK_OK;
	uint32_t int_level = 0;
#if defined(CONFIG_SDIO_BIDIRECT_CHANNEL_EN)
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

	sdio_list_dump_info(chan_buf_p->finish_list.head, chan_buf_p->finish_list.tail, chan_buf_p->finish_list.count);

	if(chan_buf_p->finish_list.count)
	{
		sdio_node_ptr_t tmp_node_p;
		ret = sdio_list_pop_node(&chan_buf_p->finish_list, &tmp_node_p);
		if(ret != BK_OK)
		{
			SDIO_LOGE("no node");
			return BK_ERR_SDIO_NO_BUFFER;
		}

		*node_p = tmp_node_p;
	}

	sdio_list_dump_info(chan_buf_p->finish_list.head, chan_buf_p->finish_list.tail, chan_buf_p->finish_list.count);
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
#if defined(CONFIG_SDIO_BIDIRECT_CHANNEL_EN)
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

#if defined(CONFIG_SDIO_SYNC_TRANS)

bk_err_t bk_sdio_register_rx_cb(sdio_rx_cb_t cb)
{
	uint32_t int_level = 0;
	int_level = rtos_disable_int();

	sdio_rx_hdl = cb;

	rtos_enable_int(int_level);

	return BK_OK;
}
#endif

//should call sdio_chan_push_free_node after finish use
static bk_err_t sdio_chan_notify_cb(sdio_chan_id_t chan_id, chan_direct_t direct)
{
	bk_err_t ret = BK_OK;
#if defined(CONFIG_SDIO_BIDIRECT_CHANNEL_EN)
	uint32_t buf_index = direct;
#else
	uint32_t buf_index = 0;
#endif
	sdio_chan_buf_t *chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[buf_index];

	if(chan_buf_p->cb)
	{
		chan_buf_p->cb(chan_buf_p->finish_list.head, chan_buf_p->finish_list.tail, chan_buf_p->finish_list.count);
	}

#if 0
	ret = rtos_set_semaphore(&chan_buf_p->semaphore);
	if(ret != BK_OK)
	{
		SDIO_LOGE("set sema");
		return ret;
	}
#endif
	return ret;
}

static bool sdio_chan_direct_is_inited(sdio_chan_id_t chan_id, chan_direct_t direct)
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
#if defined(CONFIG_SDIO_BIDIRECT_CHANNEL_EN)
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
		SDIO_LOGI("chan%d,direct %d inited,buf cnt=%d,buf size=%d",
			          chan_id, direct, chan_buf_p->buf_cnt, chan_buf_p->buf_size);
		goto err_exit;
	}

#if 0	//TODO:check whether needs it.
	if(chan_buf_p->lock_p == NULL)
	{
		ret = rtos_init_mutex(&chan_buf_p->lock_p);
		if (kNoErr != ret) 
		{
			SDIO_LOGE("mutex init");
			goto err_exit;
		}
	}
#endif

	if(count)
		sema_count = count;
	ret = rtos_init_semaphore(&chan_buf_p->semaphore, sema_count);
	if (kNoErr != ret) 
	{
		SDIO_LOGE("semaphore init");
		goto err_exit;
	}

	if(count)	//driver support buffer
	{
		ret = sdio_list_init(count, size, &chan_buf_p->free_list.head, &chan_buf_p->free_list.tail);
		if (BK_OK != ret) 
		{
			rtos_deinit_semaphore(&chan_buf_p->semaphore);
			SDIO_LOGE("list init");
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
	chan_buf_p->transaction_len = 0;

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
#if defined(CONFIG_SDIO_BIDIRECT_CHANNEL_EN)
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
		SDIO_LOGI("chan%d,direct %d isn't inited", chan_id, direct);
		goto err_exit;
	}

	if(chan_buf_p->buf_cnt)	//driver support buffer
	{
		ret = sdio_list_deinit(&chan_buf_p->free_list);
		if (BK_OK != ret) 
		{
			SDIO_LOGE("list deinit");
			goto err_exit;
		}
		chan_buf_p->free_list.head = NULL;

		ret = sdio_list_deinit(&chan_buf_p->ongoing_list);
		if (BK_OK != ret) 
		{
			SDIO_LOGE("list deinit");
			goto err_exit;
		}
		chan_buf_p->ongoing_list.head = NULL;

		ret = sdio_list_deinit(&chan_buf_p->finish_list);
		if (BK_OK != ret) 
		{
			SDIO_LOGE("list deinit");
			goto err_exit;
		}
		chan_buf_p->finish_list.head = NULL;
	}

	if(chan_buf_p->semaphore)
	{
		ret = rtos_deinit_semaphore(&chan_buf_p->semaphore);
		if (kNoErr != ret) 
		{
			SDIO_LOGE("semaphore deinit");
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
			SDIO_LOGE("mutex deinit");
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
	chan_buf_p->transaction_len = 0;

	//add this to avoid modify err_exit involve issue.
	rtos_enable_int(int_level);
	SDIO_LOG_DEBUG_FUNCTION_EXIT();
	return ret;

err_exit:
	rtos_enable_int(int_level);
	return ret;
}

static bk_err_t sdio_slave_get_valid_ongoing_rx_buf_ptr(sdio_chan_id_t chan_id, uint32_t count, uint8_t **buf_p)
{
	bk_err_t ret = BK_OK;
	uint32_t int_level = 0;
	uint32_t left_size = 0;
#if defined(CONFIG_SDIO_BIDIRECT_CHANNEL_EN)
	uint32_t buf_index = SDIO_CHAN_RX;
#else
	uint32_t buf_index = 0;
#endif
	sdio_chan_buf_t *chan_buf_p = NULL;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	SDIO_RETURN_CHAN_ID(chan_id);
	if(s_sdio_driver.chan[chan_id].rx_direct == 0)
	{
		SDIO_LOGD("chan%d RX isn't init", chan_id);
		return BK_ERR_SDIO_CHAN_DIRECT_NOT_INIT;
	}

	int_level = rtos_disable_int();
	chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[buf_index];

	if(chan_buf_p->ongoing_list.head == NULL)
	{
		rtos_enable_int(int_level);
		//should use bk_sdio_slave_sync_read to push buffer
		SDIO_LOGW("chan%d RX hasn't buffer node", chan_id);
		return BK_ERR_SDIO_NO_BUFFER;
	}

	if(count > chan_buf_p->buf_size)
	{
		//TODO:Assert
		SDIO_LOGE("chan%d RX no buf", chan_id);
		return BK_FAIL;
	}

	//*trans_len_p = s_sdio_driver.chan[chan_id].chan_buf[buf_index].transaction_len;
	left_size = chan_buf_p->buf_size - s_sdio_driver.chan[chan_id].chan_buf[buf_index].transaction_len;
	if(left_size >= count)
	{
		*buf_p = (uint8_t *)chan_buf_p->ongoing_list.head + sizeof(sdio_node_t) + s_sdio_driver.chan[chan_id].chan_buf[buf_index].transaction_len;
	}
	else	//current node left size isn't enough, save data to next node
	{
		sdio_node_ptr_t head_p = NULL;

		SDIO_LOGD("chan-%d rx buf next", chan_id);
		//pop out the buffer node from ongoing list
		ret = sdio_chan_pop_ongoing_node(chan_id, SDIO_CHAN_RX, &head_p);
		if((ret == BK_OK) && head_p)
		{
			//push current node to rx finish list
			//here can't confirm this is one packet as maybe host packet size is large then slave node capacity.
			sdio_chan_push_finish_list(chan_id, SDIO_CHAN_RX, head_p, head_p, 1);

			s_sdio_driver.chan[chan_id].chan_buf[buf_index].transaction_len = 0;
			*buf_p = (uint8_t *)head_p + sizeof(sdio_node_t);
		}
	}

	SDIO_LOGD("chan-%d rx buf addr=0x%08x,cnt=%d", chan_id, *buf_p, count);
	rtos_enable_int(int_level);
	SDIO_LOG_DEBUG_FUNCTION_EXIT();

	return ret;
}

static bk_err_t sdio_slave_get_valid_ongoing_tx_buf_ptr(sdio_chan_id_t chan_id, uint32_t len, uint8_t **buf_p)
{
	bk_err_t ret = BK_OK;
	uint32_t int_level = 0;
	uint32_t left_size = 0;
#if defined(CONFIG_SDIO_BIDIRECT_CHANNEL_EN)
	uint32_t buf_index = SDIO_CHAN_TX;
#else
	uint32_t buf_index = 0;
#endif
	sdio_chan_buf_t *chan_buf_p = NULL;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	SDIO_RETURN_CHAN_ID(chan_id);
	if(s_sdio_driver.chan[chan_id].tx_direct == 0)
	{
		SDIO_LOGD("chan%d TX isn't init", chan_id);
		return BK_ERR_SDIO_CHAN_DIRECT_NOT_INIT;
	}

	int_level = rtos_disable_int();
	chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[buf_index];
	if(chan_buf_p->ongoing_list.head == NULL)
	{
		*buf_p = NULL;
		SDIO_LOGI("ongoing finish");
		rtos_enable_int(int_level);
		return BK_FAIL;
	}
	left_size = chan_buf_p->ongoing_list.head->len - s_sdio_driver.chan[chan_id].chan_buf[buf_index].transaction_len;
	if(left_size >= 0)	//current head node tx isn't finish 
	{
		*buf_p = (uint8_t *)chan_buf_p->ongoing_list.head + sizeof(sdio_node_t) + s_sdio_driver.chan[chan_id].chan_buf[buf_index].transaction_len;
	}
	else	//this buffer finish, next buffer
	{
		*buf_p = NULL;
		SDIO_LOGE("TODO:chan%d TX", chan_id);
#if 0
		sdio_node_ptr_t head_p = NULL;
		sdio_msg_t msg;

		//pop out the buffer node from ongoing list
#if defined(CONFIG_SDIO_CHANNEL_EN)
		ret = sdio_chan_pop_ongoing_node(chan_id, SDIO_CHAN_TX, &head_p);
		if(ret != BK_OK)
		{
			SDIO_LOGE("no data to tx");
			rtos_enable_int(int_level);
			return ret;
		}
#endif

		//get next tx node
		s_sdio_driver.chan[chan_id].chan_buf[buf_index].transaction_len = 0;
		*buf_p = (uint8_t *)chan_buf_p->ongoing_list.head + sizeof(sdio_node_t);
#endif
	}	

	SDIO_LOGD("chan-%d tx buf addr=0x%08x,cnt=%d", chan_id, *buf_p, len);

	rtos_enable_int(int_level);

	SDIO_LOG_DEBUG_FUNCTION_EXIT();

	return ret;
}

static bk_err_t sdio_slave_add_ongoing_buf_trans_len(sdio_chan_id_t chan_id, chan_direct_t direct, uint32_t trans_len)
{
	bk_err_t ret = BK_OK;
	uint32_t int_level = 0;
#if defined(CONFIG_SDIO_BIDIRECT_CHANNEL_EN)
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
	chan_buf_p->transaction_len += trans_len;

	//buffer full
	if(chan_buf_p->transaction_len == chan_buf_p->buf_size)
	{
		sdio_node_ptr_t head_p = NULL;

		SDIO_LOGD("chan-%d tx/rx buf next", chan_id);
		//pop out the buffer node from ongoing list
		ret = sdio_chan_pop_ongoing_node(chan_id, direct, &head_p);
		if(ret == BK_OK)
		{
			chan_buf_p->transaction_len = 0;	//set next transaction len to 0
			//push current node to tx/rx finish list
			sdio_chan_push_finish_list(chan_id, direct, head_p, head_p, 1);
		}
		else
		{
			rtos_enable_int(int_level);
			SDIO_LOGE("list err");
			return ret;
		}

#if defined(CONFIG_SDIO_CHANNEL_EN)
		sdio_chan_notify_cb(chan_id, direct);
#endif

		SDIO_LOGI("TODO:maybe need to notify APP without the buffer full");
#if 0
		//notify APP read finish
		ret = rtos_set_semaphore(&chan_buf_p->semaphore);
		if(ret != BK_OK)
		{
			SDIO_LOGE("get sema");
			rtos_enable_int(int_level);
			return BK_FAIL;
		}
#endif

		//remove node from finish list
		sdio_chan_pop_finish_node(chan_id, direct, &head_p);
//		SDIO_LOGE("TODO:APP maybe still wants to use the buffer, so can't release it here");
	}

	rtos_enable_int(int_level);

	SDIO_LOGD("chan_id=%d,tx/rx=%d,tarns_len=%d,total_len=%d", chan_id, direct, trans_len, chan_buf_p->transaction_len);
	SDIO_LOG_DEBUG_FUNCTION_EXIT();

	return ret;
}

#endif

//WARNING:Becard 4bytes align issue
static bk_err_t sdio_slave_rx_fifo_to_buf(uint8_t *tar_addr_p, uint32_t count)
{
	//TODO:DMA not support until now.
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
#if defined(CONFIG_SDIO_GDMA_EN)
	BK_LOG_ON_ERR(bk_dma_set_transfer_len(s_sdio_rx_dma_chan, count));
	sdio_rx_dma_set_dst_buf(tar_addr_p, count);
	SDIO_LOGD("count = %d, tar_addr_p = 0x%x\n", count, tar_addr_p);
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

static bk_err_t sdio_slave_rx(uint32_t count)
{
	uint8_t *tar_addr_p = NULL;

	//TODO: As SW doesn't check header chan_id, so here use default value.
#if defined(CONFIG_SDIO_CHANNEL_EN)
	sdio_chan_id_t chan_id = SDIO_CHAN_PLATFORM;
#endif

	//search current channel
	//TODO:uses CHAN_ID_PLATFORM

	//get current channel rx ptr
#if defined(CONFIG_SDIO_CHANNEL_EN)
	sdio_slave_get_valid_ongoing_rx_buf_ptr(chan_id, count, &tar_addr_p);
#endif

	//save data to rx buffer
	if(tar_addr_p)
	{
		sdio_slave_rx_fifo_to_buf(tar_addr_p, count);
	}
	else
	{
		SDIO_LOGD("rx no buf");
		return BK_FAIL;
	}

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
#if defined(CONFIG_SDIO_SYNC_TRANS)
bk_err_t bk_sdio_slave_sync_read(sdio_chan_id_t chan_id, sdio_node_ptr_t head_p, sdio_node_ptr_t tail_p, uint32_t count, uint32_t timeout)
#else
bk_err_t bk_sdio_slave_sync_read(sdio_chan_id_t chan_id, sdio_node_ptr_t head_p, sdio_node_ptr_t tail_p, uint32_t count)
#endif
{
	bk_err_t ret = BK_OK;
	uint32_t int_level = 0;
#if defined(CONFIG_SDIO_BIDIRECT_CHANNEL_EN)
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
		SDIO_LOGE("push list");
		goto err_exit;
	}
	//add this to avoid modify err_exit involve issue.
	rtos_enable_int(int_level);


	//TODO:wait read finish
//	SDIO_LOGE("TODO:how to notify APP read finish:Sema/Callback");

#if defined(CONFIG_SDIO_SYNC_TRANS)
	ret = rtos_get_semaphore(&chan_buf_p->semaphore, timeout);
	if(ret != BK_OK)
	{
		//SDIO_LOGE("get sema");
		return ret;
	}
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
bk_err_t sdio_slave_async_read(sdio_chan_id_t chan_id, sdio_node_ptr_t head_p, sdio_node_ptr_t tail_p, uint32_t count)
{
	bk_err_t ret = BK_OK;

	return ret;
}

void data_endian_trans(uint8_t* src_addr_p, uint32_t count)
{
    uint32_t index = 0;
    uint8_t  tmp_data = 0;

    SDIO_LOGD("%s:src=0x%x,cnt=%d\n", __func__, src_addr_p, count);
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


//WARNING:Becare 4bytes align issue
static bk_err_t sdio_slave_tx_buf_to_fifo(uint8_t *src_addr_p, uint32_t count)
{
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();


	//TODO:DMA not support until now.
#if defined(CONFIG_SDIO_GDMA_EN)
	BK_LOG_ON_ERR(bk_dma_set_transfer_len(s_sdio_tx_dma_chan, count));
	sdio_tx_dma_set_src_buf(src_addr_p, count);
//	SDIO_LOGE("count = %d, src_addr_p = 0x%x\n", count, src_addr_p);
	return bk_dma_start(s_sdio_tx_dma_chan);
#else	//if not use GDMA, here will block IRQ too much time
	uint32_t i, byte_cnt = count % 4, word_cnt = count/4;
	uint8_t *cur_p = src_addr_p;
	sdio_msg_t msg;

	//read data from buffer to SDIO REG(SDIO FIFO)
	for(i = 0; i < word_cnt; i = i + 4)
	{
		while(sdio_hal_get_write_ready() == 0)
		{
	
		}
	
		sdio_hal_slave_write_data(*(uint32_t *)cur_p);
		cur_p += 4;
	}

	if(byte_cnt)
	{
		while(sdio_hal_get_write_ready() == 0)
		{
		
		}
		//though here write 4 bytes, but the buffer should not use this length if not align 4 bytes
		sdio_hal_slave_write_data(*(uint32_t *)cur_p);
	}

	msg.id = SDIO_WRITE_TO_FIFO_FINISH;
	sdio_send_msg(&msg);

#endif
	SDIO_LOG_DEBUG_FUNCTION_EXIT();

	return BK_OK;
}

static bk_err_t sdio_slave_tx(sdio_chan_id_t chan_id, uint32_t len)
{
	uint8_t *src_addr_p = NULL;

	//get current channel tx ptr
#if defined(CONFIG_SDIO_CHANNEL_EN)
	sdio_slave_get_valid_ongoing_tx_buf_ptr(chan_id, len, &src_addr_p);
#endif

	//save data to tx fifo
	if(src_addr_p)
	{
		sdio_slave_tx_buf_to_fifo(src_addr_p, len);
	}
	else
	{
		SDIO_LOGD("tx no data");
		return BK_FAIL;
	}

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
#if defined(CONFIG_SDIO_SYNC_TRANS)
bk_err_t bk_sdio_slave_sync_write(sdio_chan_id_t chan_id, sdio_node_ptr_t head_p, sdio_node_ptr_t tail_p, uint32_t count, uint32_t timeout)
#else
bk_err_t bk_sdio_slave_sync_write(sdio_chan_id_t chan_id, sdio_node_ptr_t head_p, sdio_node_ptr_t tail_p, uint32_t count)
#endif
{
	bk_err_t ret = BK_OK;
	uint32_t int_level = 0;
#if defined(CONFIG_SDIO_BIDIRECT_CHANNEL_EN)
	uint32_t buf_index = SDIO_CHAN_TX;
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
		SDIO_LOGE("push list fail");
		goto err_exit;
	}

	//Slave set TX length, CMD start
	//be care:maybe the sdio is transaction head node now.
	sdio_hal_slave_set_tx_length(chan_buf_p->ongoing_list.head->len);
	//TODO:wait write finish
//	SDIO_LOGE("TODO:how to notify APP write finish:Sema/Callback");
#if 0
	//TODO:wait write finish
	ret = rtos_get_semaphore(&chan_buf_p->semaphore, BEKEN_WAIT_FOREVER);
	if(ret != BK_OK)
	{
		SDIO_LOGE("get sema");
		goto err_exit;
	}
#endif
	//add this to avoid modify err_exit involve issue.
	rtos_enable_int(int_level);

	//Notify host:slave will send data
#if defined(CONFIG_GPIO_NOTIFY_TRANSACTION_EN)
	sdio_slave_notify_host(100);
#endif

#if defined(CONFIG_SDIO_SYNC_TRANS)
	//TODO:wait write finish
	ret = rtos_get_semaphore(&chan_buf_p->semaphore, timeout);
	if(ret != BK_OK)
	{
		SDIO_LOGE("get tx sema error.");
		return ret;
	}
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

static bk_err_t sdio_slave_tx_left_buffers(void)
{
	sdio_chan_id_t chan_id = 0;
	uint32_t int_level = 0;
	sdio_chan_buf_t *chan_buf_p = NULL;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	int_level = rtos_disable_int();
	for(chan_id = 0; chan_id < SDIO_CHAN_MAX_CNT; chan_id++)
	{
		chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[SDIO_CHAN_TX];
		if(sdio_list_get_node_count(&chan_buf_p->ongoing_list))
		{
			//Slave set TX length, CMD start
			//be care:maybe the sdio is transaction head node now.
			sdio_hal_slave_set_tx_length(chan_buf_p->ongoing_list.head->len);
			//Notify host:slave will send data
#if defined(CONFIG_GPIO_NOTIFY_TRANSACTION_EN)
			sdio_slave_notify_host(0);
#endif
			break;
		}
	}

	rtos_enable_int(int_level);

	SDIO_LOGE("TODO:should support async write");
	SDIO_LOG_DEBUG_FUNCTION_EXIT();

	return BK_OK;
}

static void sdio_thread(void *arg)
{
	bk_err_t ret;
	uint32_t rx_left_bytes = 0;
	uint32_t tx_left_bytes = 0;
	//uint32_t tx_app_left_bytes = 0;	//app cnt;
	sdio_chan_id_t chan_id = SDIO_CHAN_PLATFORM;
	uint32_t packet_len = 0;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	while(1)
	{
		sdio_msg_t msg;
		ret = rtos_pop_from_queue(&s_sdio_msg_que, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == ret)
		{
			SDIO_LOGD("rsp:msg.id=%d,param=%d", msg.id, msg.param);
			switch(msg.id)
			{
				//from SDIO IRQ
				case SDIO_NOTIFY_START_READ:
				{
					sdio_cmd53_arg_t cmd53_arg = (sdio_cmd53_arg_t)sdio_hal_slave_get_cmd_arg0();
					//cmd53_arg = (sdio_cmd53_arg_t)msg.param;
					if(cmd53_arg.block_mode)
						rx_left_bytes = cmd53_arg.count * SDIO_BLOCK_SIZE;
					else
						rx_left_bytes = cmd53_arg.count;

#if defined(CONFIG_SDIO_SW_CHANNEL_EN)
//					chan_id = cmd53_arg.channel;
#else
//					chan_id = SDIO_CHAN_PLATFORM;
#endif
					break;
				}

				//Host to SDIO FIFO finish, so start read data from FIFO to RAM
				//TODO:move it to sdio_slave_isr to improve the throughput
				case SDIO_READ_TO_FIFO_FINISH:
				{
					if(rx_left_bytes >= SDIO_BLOCK_SIZE)
					{
						packet_len = SDIO_BLOCK_SIZE;
					}
					else
						packet_len = rx_left_bytes;

					if(packet_len)
					{
						sdio_slave_rx(packet_len);
					}

					break;
				}

				//SDIO FIFO to RAM read finish,now data is in RAM
				case SDIO_READ_TO_MEMORY_FINISH:
				{
					if(rx_left_bytes >= SDIO_BLOCK_SIZE)
					{
						rx_left_bytes -= SDIO_BLOCK_SIZE;
					}
					else
					{
						rx_left_bytes = 0;	//last block
					}

					//slave can receive data again
					//sdio_hal_slave_rx_clear_host_wait_write_data();

					if(rx_left_bytes != 0)
					{
						//allow host send next block
						sdio_hal_slave_notify_host_next_block();
					}
					//save data from sdio to buffer
					SDIO_LOGE("TODO:chan_id should check");

					sdio_slave_add_ongoing_buf_trans_len(chan_id, SDIO_CHAN_RX, packet_len);
					SDIO_LOGD("read left %d bytes", rx_left_bytes);

					//NOTES:if wants to keep fifo clean, here do reset
					if(rx_left_bytes == 0)
					{
						SDIO_LOGD("TODO:reset fifo to keep clean");
						sdio_hal_fifo_reset();
						sdio_hal_set_sd_data_stop_en(1);
						sdio_hal_slave_cmd_start(1);	//wait to receive next CMD.
#if defined(CONFIG_SDIO_SYNC_TRANS)
						sdio_chan_buf_t *chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[SDIO_CHAN_RX];
						SDIO_LOGI("SDIO_CHAN_RX rtos_set_semaphore chan_id=0x%x.\r\n", chan_id);
						ret = rtos_set_semaphore(&chan_buf_p->semaphore);
						if(ret != BK_OK)
						{
							SDIO_LOGE("set sema");
							return;
						}
#endif
					}
					break;
				}

				//from SDIO IRQ
				case SDIO_NOTIFY_START_WRITE:
				{
					tx_left_bytes = msg.param;
					sdio_hal_set_tx_fifo_empty_int(1);	//start to wait empty and write first block data
					break;
				}

				case SDIO_WRITE_TO_FIFO_FINISH:
				{
					//enable int mask to wait finish
					sdio_hal_set_tx_fifo_empty_int(1);

					if(tx_left_bytes > SDIO_BLOCK_SIZE)
					{
						tx_left_bytes -= SDIO_BLOCK_SIZE;
					}
					else
						tx_left_bytes = 0;	//last block

					sdio_hal_slave_set_blk_size(packet_len);
					
					//start transaction from FIFO to HOST, wait host read finish
					while(sdio_ll_get_sd_slave_wr_finish())
					{
						volatile uint32_t itemp = 0;
						itemp++;
						if(itemp > 0x100000)
						{
							SDIO_LOGE("previous block write timeout");
							break;
						}
					}
					
					sdio_hal_slave_tx_transaction_en();

					//update tx buffer write pointer
					sdio_slave_add_ongoing_buf_trans_len(chan_id, SDIO_CHAN_TX, packet_len);
					SDIO_LOGD("write left %d bytes", tx_left_bytes);

					break;
				}

				case SDIO_WRITE_NOTIFY_FIFO_EMPTY:
				{
					SDIO_LOGI("TODO:TX channel should get from the channel list");
					chan_id = SDIO_CHAN_PLATFORM;
					
					if(tx_left_bytes > SDIO_BLOCK_SIZE)
					{
						packet_len = SDIO_BLOCK_SIZE;
					}
					else
						packet_len = tx_left_bytes;

					//TODO:CHAN ID is fixed. 
					//save data to sdio fifo
					if(packet_len)
					{
						ret = sdio_slave_tx(chan_id, packet_len);
						if(ret)
						{
							SDIO_LOGE("tx fail, reset fifo\n");
							sdio_hal_fifo_reset();
						}
					}
					else	//slave no data need to tx
					{
						sdio_hal_set_tx_fifo_empty_int(0);
						
						//TODO:
//						SDIO_LOGE("TODO:why set to 0?");
						sdio_hal_slave_set_tx_length(0);
						sdio_slave_tx_left_buffers();
						sdio_hal_fifo_reset();
						sdio_hal_set_sd_data_stop_en(1);
						sdio_hal_slave_cmd_start(1);	//wait to receive next CMD.
#if defined(CONFIG_SDIO_SYNC_TRANS)
						sdio_chan_buf_t *chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[SDIO_CHAN_TX];
						SDIO_LOGI("SDIO_CHAN_TX rtos_set_semaphore chan_id=0x%x.\r\n", chan_id);
						ret = rtos_set_semaphore(&chan_buf_p->semaphore);
						if(ret != BK_OK)
						{
							SDIO_LOGE("set sema");
							return;
						}
#endif
					}

					break;
				}

				default:
					break;
			}
		}
	}
}




/*********************************************/
#if defined(CONFIG_SDIO_SYNC_TRANS)

static bk_err_t sdio_test_tx_cb(sdio_node_ptr_t head_p, sdio_node_ptr_t tail_p, uint32_t count)
{
	bk_err_t ret = BK_OK;
#if 0
	sdio_node_ptr_t cur_p = head_p;
	uint32_t i = count, j = 0;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	//check tx data after finish
	while(cur_p && (i > 0))
	{
		for(j = 0; j < cur_p->len; j += 4)
		{
			SDIO_LOGI("0x%08x ", *(uint32_t *)((uint8_t *)cur_p + (sizeof(sdio_node_t)) + j));
			if(j % 16 == 0)
				SDIO_LOGI("\r\n");
		}

		cur_p = cur_p->next;
		i--;
	}
#endif

	//TODO:release buffer
	//WARNING: Application should should use self CHAN_ID and CHAN_DIRECT.
	bk_sdio_chan_push_free_list(SDIO_CHAN_PLATFORM, SDIO_CHAN_TX, head_p, tail_p, count);

	//notify app task finish read

	SDIO_LOG_DEBUG_FUNCTION_EXIT();
	return ret;
}

static bk_err_t sdio_test_tx_init(uint32_t chan_id, uint32_t buf_cnt, uint32_t buf_size)
{
	bk_err_t ret = BK_OK;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	ret = bk_sdio_init_channel(chan_id, SDIO_CHAN_TX, buf_cnt, buf_size);
	if(ret != BK_OK)
	{
		SDIO_LOGE("init channel");
		return ret;
	}
	bk_sdio_register_chan_cb(chan_id, SDIO_CHAN_TX, (sdio_chan_cb_t)sdio_test_tx_cb);

	SDIO_LOG_DEBUG_FUNCTION_EXIT();
	return ret;
}

static bk_err_t sdio_test_rx_cb(sdio_node_ptr_t head_p, sdio_node_ptr_t tail_p, uint32_t count)
{
	bk_err_t ret = BK_OK;

//	static uint32_t rx_val_test[SDIO_CHAN_MAX_CNT] = {0};
//	int print_len = 2048;
	sdio_chan_id_t chan_id = SDIO_CHAN_PLATFORM;

	sdio_node_ptr_t cur_p = head_p;
//	uint32_t j = 0;

	//not print received data by default
#if 1
	SDIO_LOGI("core_id=0x%x, payload cur_p=0x%08x \r\n", chan_id, cur_p);
	// for(j = 0; j < print_len; j += sizeof(uint32_t))
	// {
	// 	SDIO_LOGI("0x%08x ", *(uint32_t *)((uint8_t *)cur_p + (sizeof(sdio_node_t)) + j));
	// 	if(j && (j % 16 == 0))
	// 		SDIO_LOGI("\r\n");
	// }
#else
	//not print received data by default
	for (j = 4; j < print_len - 4; j += sizeof(uint32_t)) {
		if(*(uint32_t *)((uint8_t *)cur_p + (sizeof(sdio_node_t)) + j) != (rx_val_test[chan_id] + 0x10000000 * (chan_id))) {
			SDIO_LOGI("rx_tp_cb error, cur_p_word[%d] = 0x%x, correct value is 0x%x.\n" , j, *(uint32_t *)((uint8_t *)cur_p + (sizeof(sdio_node_t)) + j), (rx_val_test[chan_id] + 0x10000000 * (chan_id)) );
		}
	}
	rx_val_test[chan_id]++;
#endif
	//TODO:release buffer to free list mem-pool
	//WARNING: Application should should use self CHAN_ID and CHAN_DIRECT.
	ret = bk_sdio_chan_push_free_list(SDIO_CHAN_PLATFORM, SDIO_CHAN_RX, head_p, tail_p, count);

	SDIO_LOG_DEBUG_FUNCTION_EXIT();
	return ret;
}



bk_err_t test_sdio_rx_cb(char* buf_p, uint32_t len)
{
	SDIO_LOGI("============func %s, len =%d.\r\n", __func__, len);
	return BK_OK;
}


static bk_err_t sdio_test_rx_init(uint32_t chan_id, uint32_t buf_cnt, uint32_t buf_size)
{
	bk_err_t ret = BK_OK;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	ret = bk_sdio_init_channel(chan_id, SDIO_CHAN_RX, buf_cnt, buf_size);
	if(ret != BK_OK)
	{
		SDIO_LOGE("init channel");
		return ret;
	}

	ret = bk_sdio_register_chan_cb(chan_id, SDIO_CHAN_RX, (sdio_chan_cb_t)sdio_test_rx_cb);
	
	//bk_sdio_register_rx_cb((sdio_rx_cb_t) test_sdio_rx_cb);


	SDIO_LOG_DEBUG_FUNCTION_EXIT();
	return ret;
}

bk_err_t bk_sdio_slave_tx(char* buf_p, uint32_t len, uint32_t timeout_ms)
{
	int ret = 0;
	sdio_node_ptr_t node_buf_p = NULL;
	uint32_t size = 0;
//	uint32_t chan_id = SDIO_CHAN_PLATFORM;
//	uint32_t buf_index = SDIO_CHAN_TX;
//
//	sdio_chan_buf_t *chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[buf_index];
	
	ret = bk_sdio_chan_pop_free_node(SDIO_CHAN_PLATFORM, SDIO_CHAN_TX, &node_buf_p, &size);
	if((ret == BK_OK) && node_buf_p)
	{
		//fill data
		if(len > size)
		{
			len = size;
			SDIO_LOGE("max packet size=%d, len=%d", size, len);
		}

		memcpy((uint8_t *)node_buf_p + sizeof(sdio_node_t), buf_p, len);
		node_buf_p->len = len;
		
		//request write
		ret = bk_sdio_slave_sync_write(SDIO_CHAN_PLATFORM, node_buf_p, node_buf_p, 1, BEKEN_WAIT_FOREVER);
	}
	else
	{
		SDIO_LOGE("no free buff");
		ret = BK_FAIL;
	}

	return BK_OK;
}

bk_err_t bk_sdio_slave_rx(char* buf_p, uint32_t len, uint32_t timeout_ms)
{
	int ret = 0;
	sdio_node_ptr_t node_buf_p = NULL;
	uint32_t size = 0;
//	uint32_t chan_id = SDIO_CHAN_PLATFORM;
//	uint32_t buf_index = SDIO_CHAN_RX;
//	sdio_chan_buf_t *chan_buf_p = &s_sdio_driver.chan[chan_id].chan_buf[buf_index];
	
	ret = bk_sdio_chan_pop_free_node(SDIO_CHAN_PLATFORM, SDIO_CHAN_RX, &node_buf_p, &size);
	if((ret == BK_OK) && buf_p)
	{
		len = len < size? len : size;

		node_buf_p->len = len;
		
		//request read
		ret = bk_sdio_slave_sync_read(SDIO_CHAN_PLATFORM, node_buf_p, node_buf_p, 1, timeout_ms);
	}
	else
	{
		rtos_delay_milliseconds(10);
		//SDIO_LOGE("no free buff");
		ret = BK_FAIL;
	}

	//memcpy(buf_p, (uint8_t *)node_buf_p + sizeof(sdio_node_t), len);

	//slave rx callback
	if(ret == 0) {
		sdio_rx_hdl((char *)node_buf_p + sizeof(sdio_node_t), len);
	}

	return BK_OK;
}


static void sdio_rx_thread(void *arg)
{
	SDIO_LOG_DEBUG_FUNCTION_ENTRY();
	char buf_p[SDIO_BUFF_SIZE + 16] = {0};
	uint32_t timeout = 20;

	while(1)
	{
		bk_sdio_slave_rx(buf_p, SDIO_BUFF_SIZE, timeout);
	}

	SDIO_LOG_DEBUG_FUNCTION_EXIT();
}

static bk_err_t sdio_rx_thread_init(void)
{
	bk_err_t ret = BK_OK;

	ret = rtos_create_thread(
								&s_sdio_rx_thread,
								SDIO_RX_THREAD_PRIORITY,
								SDIO_RX_THREAD_NAME,
								sdio_rx_thread,
								SDIO_RX_THREAD_STACK_SIZE,
								NULL
							);
	if (kNoErr != ret)
	{
		SDIO_LOGE("init rx thread ret=%d", ret);
		goto err_exit;
	}

	//return, or err_exit release resources caused bug.
	return ret;

err_exit:
	if(s_sdio_rx_thread)
	{
		rtos_delete_thread(&s_sdio_rx_thread);
	}

	return ret;
}

#endif


static bk_err_t sdio_sw_init(void)
{
	bk_err_t ret = BK_OK;

	ret = rtos_init_queue(
							&s_sdio_msg_que,
							SDIO_MSG_QUEUE_NAME,
							sizeof(sdio_msg_t),
							SDIO_MSG_QUEUE_COUNT
						);
	if (kNoErr != ret)
	{
		SDIO_LOGE("init queue ret=%d", ret);
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
		SDIO_LOGE("init thread ret=%d", ret);
		goto err_exit;
	}

	//confirm to 0 as maybe crash value by other app.
	memset(&s_sdio_driver, 0, sizeof(s_sdio_driver));
	SDIO_LOGD("sizeof(s_sdio_driver)=%d", sizeof(s_sdio_driver));

	//return, or err_exit release resources caused bug.
	return ret;

err_exit:
	if(s_sdio_msg_que)
	{
		rtos_deinit_queue(&s_sdio_msg_que);
	}

	if(s_sdio_thread)
	{
		rtos_delete_thread(&s_sdio_thread);
	}

	return ret;
}

static void sdio_slave_isr(void)
{
	uint32_t int_status = sdio_hal_get_int_status();
	uint8_t cmd_index = (int_status >> 14) & (0x3f);
	//uint32_t cmd_arg0 = 0;
	//uint32_t cmd_arg1 = 0;

	SDIO_LOGD("%s[+]: int_status=0x%x",__func__, int_status);

	//Temp debug
	//bk_sdio_driver_dump_sdio_regs();

	//Slave response end int
	if(sdio_hal_slave_get_cmd_response_end_int())
	{
		switch (cmd_index)
		{
			case SDIO_CMD_INDEX_52:
			{
				//SDIO_LOGD("reset fifo to keep clean");
				//sdio_hal_fifo_reset();
				sdio_hal_slave_cmd_start(1);	//wait to receive next CMD.
#if 0
				uint32_t cmd52_arg0 = sdio_hal_slave_get_func_reg_value();

				//host write func reg0 to 1, notify slave stop multi-blk infinite transfer.
				if(((sdio_cmd52_func_arg0_t) cmd52_arg0).stop == 1)
				{
					//clear stop status
					sdio_hal_slave_clear_stop();
				}

				//host read:get slave tx length
				{
					//do nothing, just wait next cmd:cmd53

#if defined(CONFIG_GPIO_NOTIFY_TRANSACTION_EN)
					//notify Slave that Host has response TX.
					sdio_slave_tx_recv_host_ack_notification();
#endif
				}
#endif

				break;
			}
			
			case SDIO_CMD_INDEX_53:
			{
				sdio_cmd53_arg_t cmd53_arg;
				uint32_t cmd53_arg_value = 0;
				cmd53_arg.v = cmd53_arg_value = sdio_hal_slave_get_cmd_arg0();

				SDIO_LOGD("cmd53_arg_value=0x%x", cmd53_arg_value);
				//sdio_driver_logout_cmd53_args(cmd53_arg);

				if(cmd53_arg.rw)	//host write, slave read
				{
					//notify sdio task to read data
					if(cmd53_arg.block_mode)
					{
						s_sdio_rx_bytes = SDIO_BLOCK_SIZE * cmd53_arg.count;
						if(cmd53_arg.count)
							SDIO_LOGD("Host write:%d blocks(x 512bytes)", cmd53_arg.count);
						else
							SDIO_LOGD("Host write:multi-blocks till stop");
					}
					else
					{
						s_sdio_rx_bytes = cmd53_arg.count;
						SDIO_LOGD("Host write:%d bytes", cmd53_arg.count);
					}

					sdio_msg_t msg;

					msg.id = SDIO_NOTIFY_START_READ;
					msg.param =  (uint32_t)s_sdio_rx_bytes;
					sdio_send_msg(&msg);
				}
				else	//host read, slave write
				{
				//BK7256 DEBUG
				#if defined(CONFIG_GPIO_NOTIFY_TRANSACTION_EN)
					//notify Slave that Host has response TX.
					sdio_slave_tx_recv_host_ack_notification();
				#endif
					//notify sdio task to read data
					if(cmd53_arg.block_mode)
					{
						s_sdio_tx_bytes = SDIO_BLOCK_SIZE * cmd53_arg.count;
						if(cmd53_arg.count)
							SDIO_LOGD("Host read:%d blocks(x 512bytes)", cmd53_arg.count);
						else
							SDIO_LOGD("Host read:multi-blocks till stop");
					}
					else
					{
						s_sdio_tx_bytes = cmd53_arg.count;
						SDIO_LOGD("Host read:%d bytes", cmd53_arg.count);
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
				SDIO_LOGE("doesn't support CMD%d", cmd_index);
				sdio_hal_slave_cmd_start(1);	//wait to receive next CMD.
				break;
			}
		}

		sdio_hal_slave_clear_cmd_response_end_int();	//CMD_S_RES_END_INT
		SDIO_LOGD("CMD%d Respond end", cmd_index);

		//TODO:need to check whether return.
		return;
	}

	//slave rx full or finish data int
	if(sdio_hal_slave_get_read_int_status())
	{
		sdio_msg_t	msg;

		SDIO_LOGD("ISR to Task:Read");
		msg.id = SDIO_READ_TO_FIFO_FINISH;
		sdio_send_msg(&msg);

		//clear self int status
		sdio_hal_slave_clear_read_int_status();
	}

	//slave tx fifo empty int
	if(sdio_hal_get_tx_fifo_empty_int_status())
	{
		sdio_msg_t	msg;

		SDIO_LOGD("ISR to Task:write");
		msg.id = SDIO_WRITE_NOTIFY_FIFO_EMPTY;
		sdio_send_msg(&msg);

		//disable int mask
		sdio_hal_set_tx_fifo_empty_int(0);

		//clear self int status
		sdio_hal_clear_tx_fifo_empty_int_status();
	}

	//slave tx write data end int
	if(sdio_hal_slave_get_wr_end_int())
	{
//		SDIO_LOGE("TODO:write end int whether similar with empty");
		sdio_hal_slave_clear_wr_end_int();
	}

//TODO:
#if 0
	if(SDIO_REG0X9_ADDR & (1<<4)){
		SDIO_REG0X9_ADDR  = (1<<4);	//SD_DATA_WR_END_INT
		SDIO_LOGD("write end");
		s_write_end = 1;
	}

	if(SDIO_REG0X9_ADDR & (1<<3)){
		SDIO_REG0X9_ADDR  = (1<<3);	//TX_FIFO_NEED_WRITE
	}

	if(SDIO_REG0X9_ADDR & (1<<7)){
		SDIO_REG0X9_ADDR  = (1<<7);	//TX_FIFO_NEED_WRITE
	}	

	if(SDIO_REG0X9_ADDR & (1<<9)){	//TX_FIFO_EMPT
		SDIO_REG0XA_ADDR  &= ~(1<<9);	//TX_FIFO_EMPT_MASK
	}	
#endif

}

static bk_err_t sdio_slave_hw_init(void)
{
	bk_err_t ret = BK_OK;

	sdio_hal_set_host_slave_mode(SDIO_SLAVE_MODE);

	sdio_hal_slave_set_samp_sel(1);
	sdio_hal_rx_set_sd_byte_sel(1);

	//clock
	sys_hal_set_sdio_clk_en(true);
	sys_hal_set_sdio_clk_sel((uint32_t)SDIO_CLK_XTL);
	sys_hal_set_sdio_clk_div((uint32_t)SDIO_CLK_DIV_1);

#if defined(CONFIG_SDIO_4LINES_EN)
	sdio_hal_set_sd_data_bus(1);
#endif

#if defined(CONFIG_SDIO_1V8_EN)
	sys_drv_psram_psldo_vset(1, 3);        //set vddpsram voltage as 1.8v
	sys_drv_psram_ldo_enable(1);           //vddpsram enable
	sys_hal_set_ana_reg10_vddgpio_sel(1);  //GPIO vdd select between vio and vddparam
#endif

	//pin
	ret = sdio_gpio_init();
	if(ret != BK_OK)
	{
		SDIO_LOGE("gpio init");
		return ret;
	}
#if defined(CONFIG_GPIO_NOTIFY_TRANSACTION_EN)
	sdio_gpio_notify_transaction_init(GPIO_SDIO_NOTIFY_HOST, GPIO_SDIO_HOST_WAKEUP_SLAVE);
#endif

	//dma
#if defined(CONFIG_SDIO_GDMA_EN)
	ret = sdio_dma_init();
	if(ret != BK_OK)
	{
		SDIO_LOGE("dma init");
		return ret;
	}
#endif

	//interrupt
	sdio_hal_slave_set_cmd_res_end_int(1);
	sdio_hal_slave_set_write_end_int(1);
	sdio_hal_slave_set_read_end_int(1);

	bk_int_isr_register(INT_SRC_SDIO, sdio_slave_isr, NULL);
#if	((CONFIG_CPU_CNT > 1) && defined(CONFIG_SYS_CPU1))
	sys_driver_set_cpu1_sdio_int_en(true);
#else
	sys_driver_set_cpu0_sdio_int_en(true);
#endif

	//CMD start:wait Host CMD
	sdio_hal_slave_cmd_start(1);

	return ret;
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
	//uint32_t int_level = 0;

	SDIO_LOG_DEBUG_FUNCTION_ENTRY();

	//int_level = rtos_disable_int();

	if(s_sdio_driver_is_init) 
	{
		//rtos_enable_int(int_level);
		SDIO_LOGI("has inited");
		return BK_OK;
	}

	ret = sdio_sw_init();
	if(ret != BK_OK)
	{
		SDIO_LOGE("sw init fail");
		goto err_exit;
	}

	ret = sdio_slave_hw_init();
	if(ret != BK_OK)
	{
		SDIO_LOGE("sw init fail");
		goto err_exit;
	}

	s_sdio_driver_is_init = true;

	bk_sdio_driver_dump_sdio_regs();

	SDIO_LOG_DEBUG_FUNCTION_EXIT();

#if defined(CONFIG_SDIO_SYNC_TRANS)
	sdio_test_tx_init(SDIO_CHAN_PLATFORM, 6, SDIO_BUFF_SIZE);
	sdio_test_rx_init(SDIO_CHAN_PLATFORM, 6, SDIO_BUFF_SIZE);
	sdio_rx_thread_init();

#endif

	//add this to avoid modify err_exit involve issue.
	//rtos_enable_int(int_level);
	return ret;

err_exit:
	//rtos_enable_int(int_level);
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
			SDIO_LOGE("deinit queue ret=%d", ret);
			return ret;
		}

		s_sdio_msg_que = NULL;
	}

	if(s_sdio_thread)
	{
		ret = rtos_delete_thread(&s_sdio_thread);
		if (BK_OK != ret)
		{
			SDIO_LOGE("delete thread ret=%d", ret);
			return ret;
		}

		s_sdio_msg_que = NULL;
	}

	return BK_OK;
}

static bk_err_t sdio_hw_deinit(void)
{
	bk_err_t ret = BK_OK;

	sdio_hal_set_host_slave_mode(SDIO_HOST_MODE);

	//clock
	sys_hal_set_sdio_clk_en(false);
	sys_hal_set_sdio_clk_sel((uint32_t)SDIO_CLK_XTL);
	sys_hal_set_sdio_clk_div((uint32_t)SDIO_CLK_DIV_1);

	//pin
	ret = sdio_gpio_deinit();
	if(ret != BK_OK)
	{
		SDIO_LOGE("gpio deinit");
		return ret;
	}

	//TODO:
#if defined(CONFIG_GPIO_NOTIFY_TRANSACTION_EN)
	sdio_gpio_notify_transaction_deinit(GPIO_SDIO_NOTIFY_HOST, GPIO_SDIO_HOST_WAKEUP_SLAVE);
#endif

	//dma
#if defined(CONFIG_SDIO_GDMA_EN)
	ret = sdio_dma_deinit();
	if(ret != BK_OK)
	{
		SDIO_LOGE("dma deinit");
		return ret;
	}
#endif

	//interrupt
	bk_int_isr_unregister(INT_SRC_SDIO);
#if	((CONFIG_CPU_CNT > 1) && defined(CONFIG_SYS_CPU1))
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
		SDIO_LOGE("sw deinit ret=%d", ret);
		rtos_enable_int(int_level);
		return ret;
	}

	ret = sdio_hw_deinit();
	if (BK_OK != ret)
	{
		SDIO_LOGE("hw deinit ret=%d", ret);
		rtos_enable_int(int_level);
		return ret;
	}

	bk_sdio_driver_dump_sdio_regs();

	SDIO_LOG_DEBUG_FUNCTION_EXIT();

	rtos_enable_int(int_level);
	return ret;
}

#endif

