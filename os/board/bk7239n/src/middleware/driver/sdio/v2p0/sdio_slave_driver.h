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



#ifndef _SDIO_SLAVE_DRIVER_H_
#define _SDIO_SLAVE_DRIVER_H_

#include <components/log.h>
#include <common/bk_err.h>
#include "bk_list.h"
#include "sdio_utils.h"


//LOG's
typedef enum
{
	SDIO_LOG_DEBUG_LEVEL = 0,
	SDIO_LOG_INFO_LEVEL,
	SDIO_LOG_WARNING_LEVEL,
	SDIO_LOG_ERR_LEVEL
}SDIO_LOG_LEVEL_T;

#define SDIO_LOG_OUTPUT_LEVEL (SDIO_LOG_DEBUG_LEVEL)

#define SDIO_LOG_ERR(...) do {if(SDIO_LOG_OUTPUT_LEVEL <= SDIO_LOG_ERR_LEVEL) { \
	BK_LOG_RAW("SDIO ERR(%s:%d) ", __FUNCTION__, __LINE__); \
	BK_LOG_RAW(__VA_ARGS__); \
	BK_LOG_RAW("\r\n"); \
}}while(0)
#define SDIO_LOG_WARNING(...) do {if(SDIO_LOG_OUTPUT_LEVEL <= SDIO_LOG_WARNING_LEVEL) { \
	BK_LOG_RAW("[SDIO WARN]"); \
	BK_LOG_RAW(__VA_ARGS__); \
	BK_LOG_RAW("\r\n"); \
}}while(0)
#define SDIO_LOG_INFO(...) do {if(SDIO_LOG_OUTPUT_LEVEL <= SDIO_LOG_INFO_LEVEL) { \
	BK_LOG_RAW("[SDIO INFO]"); \
	BK_LOG_RAW(__VA_ARGS__); \
	BK_LOG_RAW("\r\n"); \
}}while(0)
#define SDIO_LOG_DEBUG(...) do {if(SDIO_LOG_OUTPUT_LEVEL <= SDIO_LOG_DEBUG_LEVEL) { \
	BK_LOG_RAW("[SDIO DBG]"); \
	BK_LOG_RAW(__VA_ARGS__); \
	BK_LOG_RAW("\r\n"); \
}}while(0)
#define SDIO_LOG_DUMP(...) do {if(SDIO_LOG_OUTPUT_LEVEL <= SDIO_LOG_DEBUG_LEVEL) { \
		BK_LOG_RAW(__VA_ARGS__); \
	}}while(0)

#define SDIO_LOG_DEBUG_FUNCTION_ENTRY(...) do {if(SDIO_LOG_OUTPUT_LEVEL <= SDIO_LOG_DEBUG_LEVEL) { \
	BK_LOG_RAW("SDIO DBG(%s:Entry)\r\n", __FUNCTION__); \
}}while(0)
#define SDIO_LOG_DEBUG_FUNCTION_EXIT(...) do {if(SDIO_LOG_OUTPUT_LEVEL <= SDIO_LOG_DEBUG_LEVEL) { \
	BK_LOG_RAW("SDIO DBG(%s:Exit)\r\n", __FUNCTION__); \
}}while(0)


#ifdef CONFIG_SDIO_DEBUG_EN
#define SDIO_DMA_DEBUG
#define SDIO_DEBUG
#endif


#define SDIO_THREAD_PRIORITY (2)
#define SDIO_THREAD_NAME "sdio_thread"
#define SDIO_THREAD_STACK_SIZE (0x4<<10)	//Bytes

#if defined(CONFIG_SDIO_SYNC_TRANS)
#define SDIO_RX_THREAD_PRIORITY (5)
#define SDIO_RX_THREAD_NAME "sdio_rx_thread"
#define SDIO_RX_THREAD_STACK_SIZE (0x4<<10)	//Bytes
#endif

#define SDIO_MSG_QUEUE_NAME "sdio_queue"
#define SDIO_MSG_QUEUE_COUNT (8)		//Temp value,exact value should be tested out.
#define SDIO_CHAN_SEMAPHORE_COUNT (8)	//default semaphore count if APP supports buffer

#define SDIO_CMD_INDEX_52	(52)
#define SDIO_CMD_INDEX_53	(53)

#define SDIO_BLOCK_SIZE (0x200)		//512 bytes per round
#define SDIO_BUFF_SIZE  (0x800)		//2048 bytes per round


//#define SDIO_IS_CHAN_SUPPORT_BUF (0)	//const value, dont change it

typedef enum
{
	SDIO_CHAN_TX = 0,
	SDIO_CHAN_RX,
}chan_direct_t;

typedef enum
{
#ifdef CONFIG_SDIO_CHANNEL_EN
	SDIO_CHAN_PLATFORM,
	SDIO_CHAN_WIFI,
	SDIO_CHAN_MAX_CNT
#else
	SDIO_CHAN_PLATFORM,
	SDIO_CHAN_MAX_CNT
#endif
}sdio_chan_id_t;

typedef enum
{
	//CTRL
	SDIO_CHANNEL_INIT,
	
	//READ
	SDIO_NOTIFY_START_READ,			//CMD53:ISR to Task start to read data to buffer
	SDIO_READ_TO_FIFO_FINISH,		//sdio block finish,from host to SDIO FIFO
	SDIO_READ_TO_MEMORY_FINISH,		//DMA finish,from SDIO FIFO to RAM
	SDIO_MSG_SYNC_READ,
	SDIO_MSG_ASYNC_READ,
	
	//WRITE
	SDIO_NOTIFY_START_WRITE,		//CMD53:ISR to Task, start to write data from RAM buffer to SDIO FIFO
	SDIO_WRITE_NOTIFY_FIFO_EMPTY,			//sdio fifo is empty, can write data to SDIO FIFO
	SDIO_WRITE_TO_FIFO_FINISH,		//DMA finish,from RAM to SDIO FIFO
	
	SDIO_WRITE_NODE_FINISH,
	SDIO_MSG_SYNC_WRITE,
	SDIO_MSG_ASYNC_WRITE

}sdio_msg_id_t;

typedef union
{
	struct
	{
		uint32_t stop : 8;	//1:write, 0:read
		uint32_t reserve : 24;			//block/byte mode, how many blocks/bytes
	};

	uint32_t v;
}sdio_cmd52_func_arg0_t;

typedef union
{
	struct
	{
		uint32_t count : 9;			//block/byte mode, how many blocks/bytes
#if defined(CONFIG_SDIO_SW_CHANNEL_EN)				//BK7256 ASIC not use this bits, SW use it as self-define function
		uint32_t channel : 3;		//for BK7256 slave channel id
		uint32_t start_packet : 1;	//Host write:after this CMD53,the data is a new packet start
		uint32_t end_packet : 1;	//Host write:after this CMD53,the data is a packet end
		uint32_t sw_reserved : 12;
#else
		uint32_t addr : 17;			//SDIO protocal
#endif
		uint32_t op_mode : 1;		//1:multi-byte read/write increase address, 0:fixed address;BK7256 not use this bits
		uint32_t block_mode : 1;	//1:block mode, 0:byte mode
		uint32_t func_num : 3;		//BK7256 always 1
		uint32_t rw : 1;			//1:host write, 0:host read
	};

	uint32_t v;
}sdio_cmd53_arg_t;

typedef bk_err_t (*sdio_chan_cb_t)(sdio_node_ptr_t head_p, sdio_node_ptr_t tail_p, uint32_t count);

typedef bk_err_t (*sdio_rx_cb_t)(char* buf_p, uint32_t len);

bk_err_t bk_sdio_register_rx_cb(sdio_rx_cb_t cb);



typedef struct
{
	//
	uint32_t buf_cnt;
	uint32_t buf_size;

	//free list
	sdio_list_t free_list;
	
	//txing/rxing list
	sdio_list_t ongoing_list;
	//As SDIO each cycle read/write <= 512Bytes, transaction_len record has read/write how many bytes data addr in buffer
	uint32_t transaction_len;

	//tx/rx finish list
	sdio_list_t finish_list;

	//notify app:callback
	sdio_chan_cb_t cb;
	void *semaphore;
	void *lock_p;
}sdio_chan_buf_t;

typedef struct
{
	uint32 tx_direct:1;		//1:enable tx
	uint32 rx_direct:1;
	uint32 chan_id:6;
	uint32 misc_reserve:24;

#if defined(CONFIG_SDIO_BIDIRECT_CHANNEL_EN)
	sdio_chan_buf_t chan_buf[2];	//index 0:tx & index 1:rx
#else
	sdio_chan_buf_t chan_buf[1];	//tx or rx
#endif
}sdio_chan_t;

typedef struct
{
	sdio_chan_t chan[SDIO_CHAN_MAX_CNT];
	void *lock_p;
}sdio_driver_t;

typedef struct {
	uint32_t id;
	uint32_t param;
} sdio_msg_t;

//#define SDIO_MEM_DEBUG

#ifdef SDIO_DEBUG
#define SDIO_TAG "sdio"
#define SDIO_LOGI(...)      BK_LOGI(SDIO_TAG, ##__VA_ARGS__)
#define SDIO_LOGW(...)      BK_LOGW(SDIO_TAG, ##__VA_ARGS__)
#define SDIO_LOGE(...)      BK_LOGE(SDIO_TAG, ##__VA_ARGS__)
#define SDIO_LOGD(...)      BK_LOGD(SDIO_TAG, ##__VA_ARGS__)
#else
#define SDIO_LOGI(...)      os_null_printf
#define SDIO_LOGW(...)      os_null_printf
#define STATIC              static
#endif

//#define SDIO_PREALLOC

#ifdef SDIO_DEBUG
#define MALLOC_MAGIC_LEN             (8)
#else
#define MALLOC_MAGIC_LEN             (0)
#endif

#define MALLOC_MAGIC_BYTE0           (0xAA)
#define MALLOC_MAGIC_BYTE1           (0xBB)

/*CMD BUFFER space: 64bytes*/
#define MAX_CONTENT_COUNT            (15)



#if defined(CONFIG_GPIO_NOTIFY_TRANSACTION_EN)

#define WAKEUP_LEVEL (1)
#define ALLOW_SLEEP_LEVEL (0)
#define FORBID_SLEEP_TIMEOUT (2000)
#define WAIT_HOST_ACK_TIMEOUT (100)
#define GPIO_SDIO_HOST_WAKEUP_SLAVE (GPIO_27)	// == gpio_wk_receive_num
#if defined(CONFIG_SDIO_RK3568)
#define GPIO_SDIO_NOTIFY_HOST (GPIO_0)	// == gpio_wk_send_num
#else
#define GPIO_SDIO_NOTIFY_HOST (GPIO_38)
#endif

typedef void (*wk_slp_fail_callback)(void);

void register_wake_host_fail_func(wk_slp_fail_callback cb);

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
bk_err_t sdio_slave_notify_host(uint32_t timeout);

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
uint32_t get_controller_wk_slp_status(void);

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
void wk_slp_controller_handler(void);

void set_last_receive_data_time(void);

uint64_t get_last_receive_data_time(void);

uint64_t check_wakeup_contorller_validity_timeout(void);

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
void sdio_gpio_notify_transaction_init(uint8_t to_host, uint8_t from_host);

void sdio_gpio_notify_transaction_deinit(uint8_t to_host, uint8_t from_host);

#endif	//CONFIG_GPIO_NOTIFY_TRANSACTION_EN


/**
 * @brief	  Init the sdio driver as slave device.
 *
 * This API init the sdio driver as slave device.
 *	 - Init SDIO slave HW: GPIO,CLOCK,DMA,IRQ ...
 *	 - Init SDIO slave SW: Thread, Semaphore, Event.
 *
 * @attention 1. This API should be called before any other SDIO SLAVE APIs.
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_sdio_slave_driver_init(void);

/**
 * @brief	  Init a sdio channel.
 *
 * This API init a sdio channel.
 *	 - Init a single list for buffers
 *   - Create a mutex lock to protect concurrence operate link list
 * @param chan_id	The selected chan id.
 * @param direct	The channel used for TX data or RX data
 * @param count How many buffers will be malloced.
 * @param size  Each buffer size for data which doesn't include SDIO header,it should be 4 bytes align
 *
 * @attention
 *
 * @return
 *	  - BK_OK: succeed
 *	  - others: other errors.
 */
bk_err_t bk_sdio_init_channel(sdio_chan_id_t chan_id, chan_direct_t direct, uint32_t count, uint32_t size);

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
bk_err_t bk_sdio_deinit_channel(sdio_chan_id_t chan_id, chan_direct_t direct);

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
bk_err_t bk_sdio_register_chan_cb(sdio_chan_id_t chan_id, chan_direct_t direct, sdio_chan_cb_t cb);

#if defined(CONFIG_SDIO_SYNC_TRANS)
bk_err_t bk_sdio_slave_sync_read(sdio_chan_id_t chan_id, sdio_node_ptr_t head_p, sdio_node_ptr_t tail_p, uint32_t count, uint32_t timeout);
#else
bk_err_t bk_sdio_slave_sync_read(sdio_chan_id_t chan_id, sdio_node_ptr_t head_p, sdio_node_ptr_t tail_p, uint32_t count);
#endif
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
bk_err_t sdio_slave_async_read(sdio_chan_id_t chan_id, sdio_node_ptr_t head_p, sdio_node_ptr_t tail_p, uint32_t count);

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
#if defined(CONFIG_SDIO_SYNC_TRANS)
bk_err_t bk_sdio_slave_sync_write(sdio_chan_id_t chan_id, sdio_node_ptr_t head_p, sdio_node_ptr_t tail_p, uint32_t count, uint32_t timeout);
#else
bk_err_t bk_sdio_slave_sync_write(sdio_chan_id_t chan_id, sdio_node_ptr_t head_p, sdio_node_ptr_t tail_p, uint32_t count);
#endif

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
bk_err_t sdio_slave_async_write(sdio_chan_id_t chan_id, sdio_node_ptr_t head_p, sdio_node_ptr_t tail_p, uint32_t count);

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
										);

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
bk_err_t bk_sdio_chan_pop_free_node(sdio_chan_id_t chan_id, chan_direct_t direct, sdio_node_ptr_t *node_p, uint32_t *size_p);

void sdio_chan_push_ongoing_node(
										sdio_chan_id_t chan_id, 
										chan_direct_t direct,
										sdio_node_ptr_t head_p,
										sdio_node_ptr_t tail_p,
										uint32_t count
										);

bk_err_t sdio_chan_pop_ongoing_node(sdio_chan_id_t chan_id, chan_direct_t direct, sdio_node_ptr_t *head_p);

bk_err_t sdio_chan_push_finish_list(
										sdio_chan_id_t chan_id, 
										chan_direct_t direct,
										sdio_node_ptr_t head_p,
										sdio_node_ptr_t tail_p,
										uint32_t count
										);

bk_err_t sdio_chan_pop_finish_node(sdio_chan_id_t chan_id, chan_direct_t direct, sdio_node_ptr_t *node_p);

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
void bk_sdio_driver_dump_sdio_regs(void);

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
bk_err_t sdio_slave_driver_deinit(void);
bk_err_t bk_sdio_slave_tx(char* buf_p, uint32_t len, uint32_t timeout_ms);

#endif // _SDIO_H_

