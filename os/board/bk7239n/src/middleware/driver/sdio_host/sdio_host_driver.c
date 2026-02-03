// Copyright 2020-2025 Beken
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
#include <os/mem.h>
#include <driver/int.h>
#include <driver/gpio.h>
#include <driver/sdio_host.h>
#include "clock_driver.h"
#include "gpio_driver.h"
#include "gpio_map.h"
#include "pmu.h"
#include "power_driver.h"
#include "sdio_host_driver.h"
#include "sdio_host_hal.h"
#include "sdio_hal.h"
#include "sys_driver.h"
#include "icu_driver.h"
#include "drv_model_pub.h"
#include "bk_misc.h"
#if defined(CONFIG_SDIO_PM_CB_SUPPORT)
#include <modules/pm.h>
#endif

#if ((defined(CONFIG_SDIO_V2P0)) && (defined(CONFIG_SDIO_GDMA_EN)))
#include <driver/dma.h>
#endif

#if (defined(CONFIG_TASK_WDT))
#include "bk_wdt.h"
#endif

#if CONFIG_IO_MATRIX_VER2_0
#include "driver/io_matrix.h"
#include "io_matrix_driver.h"
#endif
#if (defined(CONFIG_SDIO_V2P0))
/*
 * TODO: until now(2022-08-20),we use specific value for timeout.
 *       The right way is read the SDCARD speed mode and SDIO clock frequency, and then set a suit value.
 *       But sometimes, the special SDCARD maybe at busy status(programming data to memory) too much time.
 */
#define SDIO_MAX_TX_WAIT_TIME (512)
#define SDIO_MAX_RX_WAIT_TIME (512)
#define SDIO_CMD_WAIT_TIME    (4)	//one cycle:try 128 counts
#define SDIO_BLOCK_SIZE (0x200)
#define APP_RSP_CMD               63
#define SDIO_GET_WR_STS_MAX_COUNT     (100)
#endif
typedef struct {
	sdio_host_hal_t hal;
	uint32_t int_status;
	beken_queue_t irq_cmd_msg;	//hasn't event in freertos
	bool is_tx_blocked;
	beken_semaphore_t tx_sema;
	beken_semaphore_t rx_sema;
#if defined(CONFIG_SDIO_PM_CB_SUPPORT)
	uint32_t pm_backup[SDIO_PM_BACKUP_REG_NUM];
	uint8_t pm_backup_is_valid;
#endif

#if defined(CONFIG_SDIO_GDMA_EN)
	uint8_t dma_tx_en;
	uint8_t dma_tx_id;
	uint32_t tx_start_addr;
	uint32_t tx_total_len;	//how many bytes data will be write to FIFO:before APP start to write, it sets this value
	uint32_t tx_transfered_len;	//used by driver to record has transfered how many bytes
	uint32_t tx_cur_trans_len;
	uint8_t dma_rx_en;
	uint8_t dma_rx_id;
	uint32_t rx_start_addr;
	uint32_t rx_total_len;
	uint32_t rx_transfered_len;
	uint32_t rx_cur_trans_len;
#endif
} sdio_host_driver_t;

#define SDIO_HOST_RETURN_ON_NOT_INIT() do {\
	if (!s_sdio_host_driver_is_init) {\
		SDIO_HOST_LOGE("sdio host driver not init\r\n");\
		return BK_ERR_SDIO_HOST_NOT_INIT;\
	}\
} while(0)

static sdio_host_driver_t s_sdio_host = {0};
static bool s_sdio_host_driver_is_init = false;
static bool s_sdio_host_data_crc_error = false;
static void sdio_host_isr(void);
void bk_sdio_clock_en(uint32_t enable);

static void sdio_host_init_gpio(void)
{
	const sdio_host_gpio_map_t sdio_host_gpio_map_table[] = SDIO_GPIO_MAP;

	/* set gpio sdio host map: clk,cmd,data0 */
	for (uint32_t i = SDIO_HOST_GPIO_CLK_INDEX; i < SDIO_HOST_GPIO_PIN_NUMBER; i++) {
		gpio_dev_unmap(sdio_host_gpio_map_table[i].gpio_id);
	}


#if defined(CONFIG_SDIO_4LINES_EN)
	gpio_sdio_sel(GPIO_SDIO_MAP_MODE0);
#else
	gpio_sdio_one_line_sel(GPIO_SDIO_MAP_MODE0);
#endif


	/* sdio host clk */
	bk_gpio_pull_up(sdio_host_gpio_map_table[SDIO_HOST_GPIO_CLK_INDEX].gpio_id);
	bk_gpio_set_capacity(sdio_host_gpio_map_table[SDIO_HOST_GPIO_CLK_INDEX].gpio_id, 3);
#if CONFIG_IO_MATRIX_VER2_0
	bk_iomx_set_gpio_func(sdio_host_gpio_map_table[SDIO_HOST_GPIO_CLK_INDEX].gpio_id, FUNC_CODE_SDIO_CLK);
#endif

	/* sdio host cmd */
	bk_gpio_pull_up(sdio_host_gpio_map_table[SDIO_HOST_GPIO_CMD_INDEX].gpio_id);
	bk_gpio_set_capacity(sdio_host_gpio_map_table[SDIO_HOST_GPIO_CMD_INDEX].gpio_id, 3);
#if CONFIG_IO_MATRIX_VER2_0
	bk_iomx_set_gpio_func(sdio_host_gpio_map_table[SDIO_HOST_GPIO_CMD_INDEX].gpio_id, FUNC_CODE_SDIO_CMD);
#endif

	/* sdio host data0 */
	bk_gpio_pull_up(sdio_host_gpio_map_table[SDIO_HOST_GPIO_DATA0_INDEX].gpio_id);
	bk_gpio_set_capacity(sdio_host_gpio_map_table[SDIO_HOST_GPIO_DATA0_INDEX].gpio_id, 3);
#if CONFIG_IO_MATRIX_VER2_0
	bk_iomx_set_gpio_func(sdio_host_gpio_map_table[SDIO_HOST_GPIO_DATA0_INDEX].gpio_id, FUNC_CODE_SDIO_DATA_0);
#endif

#if defined(CONFIG_SDIO_4LINES_EN)
	/* sdio host data1,data2,data3 */
	for (uint32_t i = SDIO_HOST_GPIO_DATA1_INDEX; i < SDIO_HOST_GPIO_PIN_NUMBER; i++) {
		bk_gpio_pull_up(sdio_host_gpio_map_table[i].gpio_id);
		bk_gpio_set_capacity(sdio_host_gpio_map_table[i].gpio_id, 3);
	}

#if CONFIG_IO_MATRIX_VER2_0
	bk_iomx_set_gpio_func(sdio_host_gpio_map_table[SDIO_HOST_GPIO_DATA1_INDEX].gpio_id, FUNC_CODE_SDIO_DATA_1);
	bk_iomx_set_gpio_func(sdio_host_gpio_map_table[SDIO_HOST_GPIO_DATA2_INDEX].gpio_id, FUNC_CODE_SDIO_DATA_2);
	bk_iomx_set_gpio_func(sdio_host_gpio_map_table[SDIO_HOST_GPIO_DATA3_INDEX].gpio_id, FUNC_CODE_SDIO_DATA_3);
#endif // CONFIG_IO_MATRIX_VER2_0
#endif // CONFIG_SDIO_4LINES_EN
}
#endif

#if defined(CONFIG_SDIO_GDMA_EN)
static uint32_t s_dma_transfer_size_check = 0;
static void sdio_dma_tx_start(dma_id_t id)
{
	if(id != s_sdio_host.dma_tx_id)
	{
		SDIO_HOST_LOGE("%s:id=%d,sdio_id=%d\r\n", __func__, id, s_sdio_host.dma_tx_id);
		return;
	}

	if(s_sdio_host.tx_transfered_len >= s_sdio_host.tx_total_len)
	{
		SDIO_HOST_LOGD("%s:id=%d,total_len=%d,transfered_len=%d\r\n", __func__, id, s_sdio_host.tx_total_len, s_sdio_host.tx_transfered_len);
		return;
	}

	uint32_t remain_len = s_sdio_host.tx_total_len - s_sdio_host.tx_transfered_len;
	uint32_t next_trans_len = remain_len < bk_dma_get_transfer_len_max(id) ? remain_len : bk_dma_get_transfer_len_max(id);

	if(next_trans_len)
	{
		//bk_gpio_set_value(32, 2);
		s_sdio_host.tx_cur_trans_len = next_trans_len;
		bk_dma_set_transfer_len(id, next_trans_len);
		bk_dma_set_src_addr(id, (uint32_t)(s_sdio_host.tx_start_addr + s_sdio_host.tx_transfered_len), (uint32_t)((s_sdio_host.tx_start_addr + s_sdio_host.tx_transfered_len)+next_trans_len));
		bk_dma_start(id);
	}
}

void sdio_dma_tx_finish(dma_id_t id)
{
	//static uint32_t s_dma_tx_finish_cnt = 0;

	s_sdio_host.tx_transfered_len += s_sdio_host.tx_cur_trans_len;	//Current round finish, add the transfered len
	//bk_gpio_set_value(32, 0);
	sdio_dma_tx_start(id);

	if(s_sdio_host.tx_total_len <= s_sdio_host.tx_transfered_len)
	{
		//s_dma_tx_finish_cnt++;
		//SDIO_HOST_LOGD("%s:s_dma_tx_finish_cnt=%d\r\n", __func__, s_dma_tx_finish_cnt);
		sdio_host_hal_enable_tx_fifo_empty_mask(&s_sdio_host.hal);
	}
}

static bk_err_t sdio_dma_tx_init(void)
{
	dma_config_t dma_config = {0};

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

	dma_config.dst.start_addr = (uint32_t) SDIO_REG0XF_ADDR;
	dma_config.dst.end_addr = (uint32_t) SDIO_REG0XF_ADDR;
	SDIO_HOST_LOGI("dst start addr=0x%8x\n", (uint32_t) SDIO_REG0XF_ADDR);
	if(s_sdio_host.dma_tx_id != DMA_ID_MAX)
		SDIO_HOST_LOGI("s_sdio_host.dma_tx_id = %d.\n", (dma_id_t)s_sdio_host.dma_tx_id);
	s_sdio_host.dma_tx_id = bk_dma_alloc(DMA_DEV_SDIO);
	SDIO_HOST_LOGI("s_sdio_host.dma_tx_id = %d.\n", (dma_id_t)s_sdio_host.dma_tx_id);
	if(DMA_ID_MAX == (dma_id_t)s_sdio_host.dma_tx_id)
	{
		return BK_ERR_DMA_ID;
	}

	BK_LOG_ON_ERR(bk_dma_init((dma_id_t)s_sdio_host.dma_tx_id, &dma_config));
	//BK_LOG_ON_ERR(bk_dma_set_transfer_len(s_sdio_host.dma_tx_id, count));
	BK_LOG_ON_ERR(bk_dma_register_isr((dma_id_t)s_sdio_host.dma_tx_id, NULL, sdio_dma_tx_finish));
	BK_LOG_ON_ERR(bk_dma_enable_finish_interrupt((dma_id_t)s_sdio_host.dma_tx_id));
#if defined(CONFIG_SPE) && (CONFIG_SPE == 1)
	BK_LOG_ON_ERR(bk_dma_set_dest_sec_attr((dma_id_t)s_sdio_host.dma_tx_id, DMA_ATTR_SEC));
	BK_LOG_ON_ERR(bk_dma_set_src_sec_attr((dma_id_t)s_sdio_host.dma_tx_id, DMA_ATTR_SEC));
#endif

	return BK_OK;
}

static void sdio_dma_rx_start(dma_id_t id)
{
	if(id != s_sdio_host.dma_rx_id)
	{
		SDIO_HOST_LOGE("%s:id=%d,sdio_id=%d\r\n", __func__, id, s_sdio_host.dma_rx_id);
		return;
	}

	if(s_sdio_host.rx_transfered_len >= s_sdio_host.rx_total_len)
	{
		SDIO_HOST_LOGD("%s:id=%d,total_len=%d,transfered_len=%d\r\n", __func__, id, s_sdio_host.rx_total_len, s_sdio_host.rx_transfered_len);
		return;
	}

	uint32_t remain_len = s_sdio_host.rx_total_len - s_sdio_host.rx_transfered_len;
	uint32_t next_trans_len = remain_len < bk_dma_get_transfer_len_max(id) ? remain_len : bk_dma_get_transfer_len_max(id);

	if(next_trans_len)
	{
		s_sdio_host.rx_cur_trans_len = next_trans_len;
		bk_dma_set_transfer_len(id, next_trans_len);
		bk_dma_set_dest_addr(id, (uint32_t)(s_sdio_host.rx_start_addr + s_sdio_host.rx_transfered_len), (uint32_t)((s_sdio_host.rx_start_addr + s_sdio_host.rx_transfered_len)+next_trans_len));
		bk_dma_start(id);
	}
}

void sdio_dma_rx_finish(dma_id_t id)
{
	static uint32_t s_dma_rx_finish_cnt = 0;

	s_sdio_host.rx_transfered_len += s_sdio_host.rx_cur_trans_len;	//Current round finish, add the transfered len
	sdio_dma_rx_start(id);

	if(s_sdio_host.rx_total_len <= s_sdio_host.rx_transfered_len)
	{
		rtos_set_semaphore(&s_sdio_host.rx_sema);
		s_dma_rx_finish_cnt++;
		SDIO_HOST_LOGD("%s:s_dma_rx_finish_cnt=%d\r\n", __func__, s_dma_rx_finish_cnt);
	}
}

static bk_err_t sdio_dma_rx_init()
{
	dma_config_t dma_config = {0};

	dma_config.mode = DMA_WORK_MODE_SINGLE;
	dma_config.chan_prio = 0;

	dma_config.src.dev = DMA_DEV_SDIO_RX;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.src.addr_inc_en = DMA_ADDR_INC_DISABLE;
	dma_config.src.start_addr = (uint32) SDIO_REG0X10_ADDR;
	dma_config.src.end_addr =  (uint32) SDIO_REG0X10_ADDR;
	dma_config.dst.dev = DMA_DEV_DTCM;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
	dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;

	s_sdio_host.dma_rx_id = bk_dma_alloc(DMA_DEV_SDIO_RX);
	SDIO_HOST_LOGI("s_sdio_host.dma_rx_id = %d.\n", s_sdio_host.dma_rx_id);
	if(DMA_ID_MAX == s_sdio_host.dma_rx_id)
	{
		return BK_ERR_DMA_ID;
	}

	BK_LOG_ON_ERR(bk_dma_init(s_sdio_host.dma_rx_id, &dma_config));
	BK_LOG_ON_ERR(bk_dma_register_isr(s_sdio_host.dma_rx_id, NULL, sdio_dma_rx_finish));
	BK_LOG_ON_ERR(bk_dma_enable_finish_interrupt(s_sdio_host.dma_rx_id));
#if defined(CONFIG_SPE) && (CONFIG_SPE == 1)
	BK_LOG_ON_ERR(bk_dma_set_dest_sec_attr(s_sdio_host.dma_rx_id, DMA_ATTR_SEC));
	BK_LOG_ON_ERR(bk_dma_set_src_sec_attr(s_sdio_host.dma_rx_id, DMA_ATTR_SEC));
#endif

	return BK_OK;
}
#endif


/* 1. power up sdio host
 * 2. set clock
 * 3. enable sdio host interrupt
 * 4. map gpio
 */
static void sdio_host_init_common(void)
{
	/* config sdio host gpio */
	sdio_host_init_gpio();

	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_SDIO, CLK_PWR_CTRL_PWR_UP);
	sys_hal_set_sdio_clk_sel(0); // set sdio source clock as XTAL 26M
	sdio_hal_set_sd_soft_resetn(1);
	sdio_host_hal_write_blk_en(&s_sdio_host.hal, 1);
	sdio_host_hal_set_fifo_send_cnt(&s_sdio_host.hal, 128);

	sys_drv_int_enable(SDIO_INTERRUPT_CTRL_BIT);
#if defined(CONFIG_SDIO_V2P0)
	sdio_host_hal_enable_all_mask(&s_sdio_host.hal);
#endif
}

static void sdio_host_deinit_common(void)
{
	sdio_host_hal_reset_config_to_default(&s_sdio_host.hal);

	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_SDIO, CLK_PWR_CTRL_PWR_DOWN);
	sys_drv_int_disable(SDIO_INTERRUPT_CTRL_BIT);
}

#if (defined(CONFIG_SDIO_PM_CB_SUPPORT))
#define SDIO_PM_CHECK_RESTORE(id) do {\
	if (bk_pm_module_lv_sleep_state_get(PM_DEV_ID_SDIO)) {\
		bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_SDIO, PM_POWER_MODULE_STATE_ON);\
		bk_sdio_restore(0, (void *)id);\
		bk_pm_module_lv_sleep_state_clear(PM_DEV_ID_SDIO); \
	}\
} while(0)

static int bk_sdio_backup(uint64_t sleep_time, void *args)
{
	SDIO_HOST_RETURN_ON_NOT_INIT();
	if (!s_sdio_host.pm_backup_is_valid)
	{
		s_sdio_host.pm_backup_is_valid = 1;
		sdio_hal_backup(&s_sdio_host.hal, &s_sdio_host.pm_backup[0]);
		bk_sdio_clock_en(false);
	}
	return BK_OK;
}

static int bk_sdio_restore(uint64_t sleep_time, void *args)
{
	SDIO_HOST_RETURN_ON_NOT_INIT();
	if (s_sdio_host.pm_backup_is_valid)
	{
		bk_sdio_clock_en(true);
		sdio_hal_restore(&s_sdio_host.hal, &s_sdio_host.pm_backup[0]);
		s_sdio_host.pm_backup_is_valid = 0;
	}
	return BK_OK;
}
#else
#define SDIO_PM_CHECK_RESTORE(id)
#endif

bk_err_t bk_sdio_host_driver_init(void)
{
	bk_err_t ret = BK_OK;
	if (s_sdio_host_driver_is_init) {
		SDIO_HOST_LOGD("bk_sdio_host_driver_init has inited\r\n");
		return BK_OK;
	}

	os_memset(&s_sdio_host, 0, sizeof(s_sdio_host));
#if (defined(CONFIG_SDIO_PM_CB_SUPPORT))
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_SDIO, PM_POWER_MODULE_STATE_ON);
#endif
	sdio_host_hal_init(&s_sdio_host.hal);
#if (defined(CONFIG_SDIO_PM_CB_SUPPORT))
	int id = 0;
	pm_cb_conf_t enter_config = {bk_sdio_backup, (void *)id};
	pm_cb_conf_t exit_config = {bk_sdio_restore, (void *)id};
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_SDIO, PM_POWER_MODULE_STATE_ON);
	bk_pm_sleep_register_cb(PM_MODE_LOW_VOLTAGE, PM_DEV_ID_SDIO, &enter_config, &exit_config);
#endif
	bk_int_isr_register(INT_SRC_SDIO, sdio_host_isr, NULL);

	if (s_sdio_host.irq_cmd_msg == NULL) {
		ret = rtos_init_queue(
								&s_sdio_host.irq_cmd_msg,
								"sdio_host_queue",
								4,
								16
							);
		BK_ASSERT(kNoErr == ret); /* ASSERT VERIFIED */
	}
	if (s_sdio_host.tx_sema == NULL) {
		int ret = rtos_init_semaphore(&(s_sdio_host.tx_sema), 1);
		BK_ASSERT(kNoErr == ret); /* ASSERT VERIFIED */
	}
	s_sdio_host.is_tx_blocked = false;

	if (s_sdio_host.rx_sema == NULL) {
		int ret = rtos_init_semaphore(&(s_sdio_host.rx_sema), 1);
		BK_ASSERT(kNoErr == ret); /* ASSERT VERIFIED */
	}
	s_sdio_host_driver_is_init = true;
	return BK_OK;
}

bk_err_t bk_sdio_host_driver_deinit(void)
{
	bk_err_t ret = BK_OK;
	if (!s_sdio_host_driver_is_init) {
		return BK_OK;
	}
	sdio_host_deinit_common();
#if (defined(CONFIG_SDIO_PM_CB_SUPPORT))
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_SDIO, PM_POWER_MODULE_STATE_OFF);
#endif
	bk_int_isr_unregister(INT_SRC_SDIO);
#if (defined(CONFIG_SDIO_PM_CB_SUPPORT))
	bk_pm_sleep_unregister_cb(PM_MODE_LOW_VOLTAGE, PM_DEV_ID_SDIO, true, true);
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_SDIO, PM_POWER_MODULE_STATE_OFF);
#endif

	if (s_sdio_host.irq_cmd_msg) {
		ret = rtos_deinit_queue(&s_sdio_host.irq_cmd_msg);
		BK_ASSERT(kNoErr == ret); /* ASSERT VERIFIED */

		s_sdio_host.irq_cmd_msg = NULL;
	}

	if (s_sdio_host.tx_sema) {
		int ret = rtos_deinit_semaphore(&(s_sdio_host.tx_sema));
		BK_ASSERT(kNoErr == ret); /* ASSERT VERIFIED */

		s_sdio_host.tx_sema = NULL;
	}
	if (s_sdio_host.rx_sema) {
		int ret = rtos_deinit_semaphore(&(s_sdio_host.rx_sema));
		BK_ASSERT(kNoErr == ret); /* ASSERT VERIFIED */

		s_sdio_host.rx_sema = NULL;
	}

	s_sdio_host_driver_is_init = false;
	return BK_OK;
}

bk_err_t bk_sdio_host_init(const sdio_host_config_t *config)
{
	BK_RETURN_ON_NULL(config);
	SDIO_HOST_RETURN_ON_NOT_INIT();
#if (defined(CONFIG_SDIO_PM_CB_SUPPORT))
	uint32_t id = 0;
	pm_cb_conf_t enter_config = {bk_sdio_backup, (void *)id};
	bk_pm_module_lv_sleep_state_clear(PM_DEV_ID_SDIO);
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_SDIO, PM_POWER_MODULE_STATE_ON);
	bk_pm_sleep_register_cb(PM_MODE_LOW_VOLTAGE, PM_DEV_ID_SDIO, &enter_config, NULL);
#endif

#if CONFIG_SDIO_1V8_EN
	sys_drv_psram_psldo_vset(1, 0); //set vddpsram voltage as 1.8v
	sys_drv_psram_ldo_enable(1);    //vddpsram enable
	sys_hal_set_ana_vddgpio_sel(1); //GPIO vdd select between vio and vddparam
#endif
#if (defined(CONFIG_SDIO_V2P0))
	bk_sdio_host_set_clock_freq(config->clock_freq);
	/* reset sdio host register */
	sdio_host_hal_reset_config_to_default(&s_sdio_host.hal);

	sdio_host_init_common();
#else
	sdio_host_hal_set_clk_freq(&s_sdio_host.hal, config->clock_freq);
	sdio_host_init_common();

	/* reset sdio host register */
	sdio_host_hal_reset_config_to_default(&s_sdio_host.hal);
#endif

#if defined(CONFIG_SDIO_GDMA_EN)
	//SDIO DMA TX init
	s_sdio_host.dma_tx_en = config->dma_tx_en;
	if(s_sdio_host.dma_tx_en)
	{
		if(sdio_dma_tx_init() != BK_OK)
		{
			SDIO_HOST_LOGI("sdio tx dma enable failed\r\n");
			s_sdio_host.dma_tx_en = 0;
		}

		s_sdio_host.tx_start_addr = 0;
		s_sdio_host.tx_total_len = 0;
		s_sdio_host.tx_transfered_len = 0;
	}

	//SDIO DMA RX init
	s_sdio_host.dma_rx_en = config->dma_rx_en;
	if(s_sdio_host.dma_rx_en)
	{
		if(sdio_dma_rx_init() != BK_OK)
		{
			SDIO_HOST_LOGI("sdio rx dma enable failed\r\n");
			s_sdio_host.dma_rx_en = 0;
		}

		s_sdio_host.rx_start_addr = 0;
		s_sdio_host.rx_total_len = 0;
		s_sdio_host.rx_transfered_len = 0;
	}
#endif

	/* set sdio host bus width */
	sdio_host_hal_set_bus_width(&s_sdio_host.hal, config->bus_width);

	return BK_OK;
}

bk_err_t bk_sdio_host_deinit(void)
{
	SDIO_HOST_RETURN_ON_NOT_INIT();

	sdio_host_deinit_common();

#if defined(CONFIG_SDIO_GDMA_EN)
	if(s_sdio_host.dma_tx_en)
	{
		bk_dma_free(DMA_DEV_SDIO, s_sdio_host.dma_tx_id);
		s_sdio_host.dma_tx_id = DMA_ID_MAX;
		s_sdio_host.dma_tx_en = 0;
	}

	if(s_sdio_host.dma_rx_en)
	{
		bk_dma_free(DMA_DEV_SDIO_RX, s_sdio_host.dma_rx_id);
		s_sdio_host.dma_rx_id = DMA_ID_MAX;
		s_sdio_host.dma_rx_en = 0;
	}
#endif

#if (defined(CONFIG_SDIO_PM_CB_SUPPORT))
	bk_pm_sleep_unregister_cb(PM_MODE_LOW_VOLTAGE, PM_DEV_ID_SDIO, true, true);
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_SDIO, PM_POWER_MODULE_STATE_OFF);
#endif

	return BK_OK;
}

#if defined(CONFIG_SDIO_V2P0)
/*
 * WARNING:
 * Reset the sdio states:TX FIFO, RX FIFO, SDIO state.
 * TX FIFO/RX FIFO effects the clock gate function.
 * Just be called in SDK,APP don't use it.
 * If error called this API maybe cause error clock output to sdcard,
 * because sdcard read/write data and command response bases on clock from BK7256.
 */
void bk_sdio_host_reset_sd_state(void)
{
	SDIO_PM_CHECK_RESTORE(0);
	sdio_host_hal_reset_sd_state(&s_sdio_host.hal);
}

/*
 * WARNING: CLOCK(Enable/Disbale/Gate) API are only called in SDK,not for APP.
 * BK7256 Clock scheme:
 * 1.bk_sdio_clock_en: sdio asic module clock enable
 * 2.bk_sdio_clk_gate_config:
 *   a. enable means the sdio clock always on, not gated;
 *   b. disable means the clock will be controlled by sdio fifo status(read/write).
 * 3.bk_sdio_tx_fifo_clk_gate_config
 *   a. at WRITE status(bk7256 write data to sdio fifo),if the fifo is empty,the clock will be gated
 *      clock doesn't output to sdcard
 * 4.If at READ status, the clock gate will be controlled by sdio asic, SW can't control it.
 *   a. FIFO full or read data finish,the clock will be gated, else clock output to sdcard
 */
void bk_sdio_clock_en(uint32_t enable)
{
	SDIO_PM_CHECK_RESTORE(0);
	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_SDIO, enable);
}

/*
 * WARNING: CLOCK(Enable/Disbale/Gate) API are only called in SDK,not for APP.
 * BK7256 Clock scheme:
 * 1.bk_sdio_clock_en: sdio asic module clock enable
 * 2.bk_sdio_clk_gate_config:
 *   a. enable means the sdio clock always on, not gated;
 *   b. disable means the clock will be controlled by sdio fifo status(read/write).
 * 3.bk_sdio_tx_fifo_clk_gate_config
 *   a. at WRITE status(bk7256 write data to sdio fifo),if the fifo is empty,the clock will be gated
 *      clock doesn't output to sdcard
 * 4.If at READ status, the clock gate will be controlled by sdio asic, SW can't control it.
 *   a. FIFO full or read data finish,the clock will be gated, else clock output to sdcard
 */
void bk_sdio_clk_gate_config(uint32_t enable)
{
	SDIO_PM_CHECK_RESTORE(0);
	sdio_host_hal_set_clock_gate(&s_sdio_host.hal, enable);
}

/*
 * WARNING: CLOCK(Enable/Disbale/Gate) API are only called in SDK,not for APP.
 * BK7256 Clock scheme:
 * 1.bk_sdio_clock_en: sdio asic module clock enable
 * 2.bk_sdio_clk_gate_config:
 *   a. enable means the sdio clock always on, not gated;
 *   b. disable means the clock will be controlled by sdio fifo status(read/write).
 * 3.bk_sdio_tx_fifo_clk_gate_config
 *   a. at WRITE status(bk7256 write data to sdio fifo),if the fifo is empty,the clock will be gated
 *      clock doesn't output to sdcard
 * 4.If at READ status, the clock gate will be controlled by sdio asic, SW can't control it.
 *   a. FIFO full or read data finish,the clock will be gated, else clock output to sdcard
 */
void bk_sdio_tx_fifo_clk_gate_config(uint32_t enable)
{
	SDIO_PM_CHECK_RESTORE(0);
	sdio_hal_host_set_tx_fifo_need_write_mask_cg(&s_sdio_host.hal, enable);
}

bk_err_t bk_sdio_host_set_clock_freq(sdio_host_clock_freq_t clock_freq)
{
	SDIO_HOST_RETURN_ON_NOT_INIT();
	SDIO_PM_CHECK_RESTORE(0);
	sdio_host_hal_set_clk_freq(&s_sdio_host.hal, clock_freq);

	return BK_OK;
}

#else
bk_err_t bk_sdio_host_set_clock_freq(sdio_host_clock_freq_t clock_freq)
{
	SDIO_HOST_RETURN_ON_NOT_INIT();
	SDIO_PM_CHECK_RESTORE(0);

	sdio_host_hal_set_clk_freq(&s_sdio_host.hal, clock_freq);

	return BK_OK;
}
#endif

static void sdio_dump_cmd_info(const sdio_host_cmd_cfg_t *command)
{
#if 0
	SDIO_HOST_LOGI("cmd_index=%d,argument=0x%08x,response=0x%08x\r\n", command->cmd_index, command->argument, command->response );
	SDIO_HOST_LOGI("wait_rsp_timeout=%d, crc_check=%d\r\n", command->wait_rsp_timeout, command->crc_check);
#endif
}

static uint32_t s_sdio_cmd_index = 0;
bk_err_t bk_sdio_host_send_command(const sdio_host_cmd_cfg_t *command)
{
	BK_RETURN_ON_NULL(command);
	SDIO_HOST_RETURN_ON_NOT_INIT();
	SDIO_PM_CHECK_RESTORE(0);

	sdio_dump_cmd_info(command);
	s_sdio_cmd_index = command->cmd_index;

	//default:need crc check, if no need crc check, please set it in command.
	//sdio_host_hal_set_cmd_crc_check(&s_sdio_host.hal, true);
	BK_RETURN_ON_ERR(sdio_host_hal_init_commad(&s_sdio_host.hal, command));
	sdio_host_hal_start_send_command(&s_sdio_host.hal);
	return BK_OK;
}

bk_err_t bk_sdio_host_wait_cmd_response(uint32_t cmd_index)
{
#if defined(CONFIG_SDIO_V2P0)
	bk_err_t ret = BK_OK;
	uint32_t msg = 0;
#endif

	SDIO_HOST_RETURN_ON_NOT_INIT();
	SDIO_PM_CHECK_RESTORE(0);

	uint32_t int_status = sdio_host_hal_get_interrupt_status(&s_sdio_host.hal);

#if defined(CONFIG_SDIO_V2P0)
	{
		//TODO:Timeout and CRC fail should use different case
		//FreeRTOS doesn't use EVENT,so use msg queue.
		ret = rtos_pop_from_queue(&s_sdio_host.irq_cmd_msg, &msg, SDIO_CMD_WAIT_TIME);
		if(ret)
		{
			SDIO_HOST_LOGD("sdio wait slave CMD%d timeout, int_status=0x%x, ret=%d\r\n", cmd_index, int_status, ret);
			return BK_ERR_SDIO_HOST_CMD_RSP_TIMEOUT;
		}
	}
#else
	/* wait until cmd response */
	BK_WHILE (!sdio_host_hal_is_cmd_rsp_interrupt_triggered(&s_sdio_host.hal, int_status)) {
		int_status = sdio_host_hal_get_interrupt_status(&s_sdio_host.hal);
	}

	/* clear command response int status */
	sdio_host_hal_clear_cmd_rsp_interrupt_status(&s_sdio_host.hal, int_status);

	if (sdio_host_hal_is_cmd_rsp_timeout_interrupt_triggered(&s_sdio_host.hal, int_status)) {
		if (cmd_index != SEND_OP_COND) {
			SDIO_HOST_LOGW("sdio wait slave CMD%d timeout\r\n", cmd_index);
			return BK_ERR_SDIO_HOST_CMD_RSP_TIMEOUT;
		}
	}

	if (sdio_host_hal_is_cmd_rsp_crc_fail_interrupt_triggered(&s_sdio_host.hal, int_status)) {
		if ((cmd_index != SD_APP_OP_COND) &&
			(cmd_index != ALL_SEND_CID) &&
			(cmd_index != SEND_CSD) &&
			(cmd_index != SEND_OP_COND)) {
			SDIO_HOST_LOGW("sdio receive slave CMD%d crc fail\r\n", cmd_index);
			return BK_ERR_SDIO_HOST_CMD_RSP_CRC_FAIL;
		}
	}
#endif
	return BK_OK;
}

uint32_t bk_sdio_host_get_cmd_rsp_argument(sdio_host_response_t argument_index)
{
	SDIO_HOST_RETURN_ON_NOT_INIT();
	SDIO_PM_CHECK_RESTORE(0);

	return sdio_host_hal_get_cmd_rsp_argument(&s_sdio_host.hal, argument_index);
}

#if (defined(CONFIG_SDIO_V2P0))
bk_err_t bk_sdio_host_config_data(const sdio_host_data_config_t *data_config)
{
	BK_RETURN_ON_NULL(data_config);
	SDIO_HOST_RETURN_ON_NOT_INIT();
	SDIO_PM_CHECK_RESTORE(0);

	if (data_config->data_dir == SDIO_HOST_DATA_DIR_RD) {
		/* 1) set data timer
		 * 2) clear int status(REG9)
		 * 3) set rx fifo threshold
		 * 4) reset read fifo , reset sdcard command and data state
		 * 5) set data register
 		 */
		//TODO:Interrupt status should only be cleared in ISR
		//sdio_host_hal_clear_interrupt_status(&s_sdio_host.hal);
		sdio_host_hal_set_rx_fifo_threshold(&s_sdio_host.hal, 0x1);

		sdio_host_hal_set_read_multi_block_data(&s_sdio_host.hal, data_config->data_block_size);
		sdio_host_hal_enable_rx_end_mask(&s_sdio_host.hal);
		//TODO:disable it or set a big value of RX threshhold?
		//sdio_host_hal_enable_rx_need_read_mask(&s_sdio_host.hal);
		sdio_host_hal_start_receive_data(&s_sdio_host.hal);

		sdio_host_hal_set_data_timeout(&s_sdio_host.hal, data_config->data_timeout);
	} else {
		//TODO:Interrupt status should only be cleared in ISR
 		//sdio_host_hal_clear_interrupt_status(&s_sdio_host.hal);
		sdio_host_hal_set_tx_fifo_threshold(&s_sdio_host.hal, 0x1);
		sdio_host_hal_set_write_multi_block_data(&s_sdio_host.hal, data_config->data_block_size);
	}
	return BK_OK;
}
#else
bk_err_t bk_sdio_host_config_data(const sdio_host_data_config_t *data_config)
{
	BK_RETURN_ON_NULL(data_config);
	SDIO_HOST_RETURN_ON_NOT_INIT();
	SDIO_PM_CHECK_RESTORE(0);

	if (data_config->data_dir == SDIO_HOST_DATA_DIR_RD) {
		/* 1) set data timer
		 * 2) clear int status(REG9)
		 * 3) set rx fifo threshold
		 * 4) reset read fifo , reset sdcard command and data state
		 * 5) set data register
 		 */
 		sdio_host_hal_clear_interrupt_status(&s_sdio_host.hal);
		sdio_host_hal_set_rx_fifo_threshold(&s_sdio_host.hal, 0x1);
		sdio_host_hal_set_read_data(&s_sdio_host.hal, data_config->data_block_size);
		sdio_host_hal_start_receive_data(&s_sdio_host.hal);
	} else {
 		sdio_host_hal_clear_interrupt_status(&s_sdio_host.hal);
		sdio_host_hal_set_tx_fifo_threshold(&s_sdio_host.hal, 0x1);
		sdio_host_hal_set_write_data(&s_sdio_host.hal, data_config->data_block_size);
		//sdio_host_hal_start_send_data(&s_sdio_host.hal);
	}
	sdio_host_hal_set_data_timeout(&s_sdio_host.hal, data_config->data_timeout);
	return BK_OK;
}

#endif
bk_err_t bk_sdio_host_set_data_timeout(uint32_t timeout)
{
	SDIO_HOST_RETURN_ON_NOT_INIT();
	SDIO_PM_CHECK_RESTORE(0);

	sdio_host_hal_set_data_timeout(&s_sdio_host.hal, timeout);
	return BK_OK;
}

#if defined(CONFIG_SDIO_V2P0)
#if defined(CONFIG_SDIO_GDMA_EN)
static bk_err_t sdio_dma_write_fifo(const uint8_t *write_data, uint32_t data_size)
{
	bk_err_t error_state = BK_FAIL;

	rtos_get_semaphore(&s_sdio_host.tx_sema, 0);	//assume last time tx_sema finish.
	s_sdio_host.tx_total_len = data_size;
	s_sdio_host.tx_transfered_len = 0;
	s_sdio_host.tx_start_addr = (uint32_t)write_data;

	s_dma_transfer_size_check = 0;

	//dma start
	sdio_dma_tx_start(s_sdio_host.dma_tx_id);

	//sdio start
	bk_sdio_tx_fifo_clk_gate_config(1);
	sdio_host_hal_start_send_data(&s_sdio_host.hal);

#if 0
	//wait SDIO last one or two block finish(DMA copy data to SDIO FIFO,the last one or two blocks are still ongoing on SDIO wires.)
	error_state = rtos_get_semaphore(&(s_sdio_host.tx_sema), SDIO_MAX_TX_WAIT_TIME);
	if(error_state)
	{
		s_sdio_host.is_tx_blocked = false;
		SDIO_HOST_LOGE("TODO:sdio write data timeout,write_data=0x%x,data_size=%d\r\n", write_data, data_size);

		//special abnormal case:If the write_data pointer is error,which cause DMA can't copy data from the source, then it cause DMA err.
		if(s_sdio_host.tx_total_len != s_sdio_host.tx_transfered_len)
		{
			bk_dma_stop(s_sdio_host.dma_tx_id);
		}
	}
#endif
	//maybe DMA finish, but SDIO doesn't finish
	//Default:after DMA finish ISR,there are 512+16 bytes still in the SDIO FIFO.so wait fifo transmit finish here
	error_state = rtos_get_semaphore(&(s_sdio_host.tx_sema), SDIO_MAX_TX_WAIT_TIME);
	if(error_state)
	{
		SDIO_HOST_LOGE("sdio write data timeout,write_data=0x%x,data_size=%d, tx_transfered_len=%d\r\n", write_data, data_size, s_sdio_host.tx_transfered_len);
	}

	return error_state;
}
#endif

static bk_err_t sdio_host_cpu_write_fifo(const uint8_t *write_data, uint32_t data_size)
{
	BK_RETURN_ON_NULL(write_data);
	SDIO_HOST_RETURN_ON_NOT_INIT();
	SDIO_PM_CHECK_RESTORE(0);

	sdio_host_hal_t *hal = &s_sdio_host.hal;
	uint32_t index = 0;
#if (defined(CONFIG_SDIO_DEBUG_SUPPORT))
	uint32 data_tmp = 0;
#endif
	bk_err_t error_state = BK_OK;
	uint32_t i = 0;

	while ((index < data_size)) {
		//confirm can write data to fifo(fifo isn't at busy/full status)
		while((sdio_host_hal_is_tx_fifo_write_ready(hal)) == 0)
		{
			i++;
			if(i % 0x400000 == 0)
				SDIO_HOST_LOGE("FIFO can't write i=0x%08x", i);

			//avoid dead in while
			if(i == 0x1000000 * 8) {
				SDIO_HOST_LOGE("FIFO write fail,the write data is invalid");
				error_state = BK_ERR_SDIO_HOST_DATA_TIMEOUT;
				break;
			}
		}

//NOTES:SDIO V1P0:write data should be reverted sequence by software, and read data should revert by ASIC of "SD_BYTE_SEL"
//or the endian isn't match with windows system.
//From BK7258XX, the write data sequence is reverted by ASIC hardware,without any config of "SD_BYTE_SEL"

#if (defined(CONFIG_SDIO_DEBUG_SUPPORT))
		data_tmp = *(uint32_t *)&write_data[index];
#endif
		sdio_host_hal_write_fifo(hal, *(uint32_t *)&write_data[index]);


		index += 4;

#if (defined(CONFIG_SDIO_DEBUG_SUPPORT))
		if(index < 16)
		{
			SDIO_HOST_LOGD("data_tmp=0x%08x", data_tmp);
		}
		if(index == 16)
		{
			SDIO_HOST_LOGD("0x%08x\r\n", data_tmp);
		}
#endif
		//first block finish, enable tx fifo clock gate and then start write data to sdio wires(sdcard)
		if((index == SDIO_BLOCK_SIZE))
		{
			bk_sdio_tx_fifo_clk_gate_config(1);
			sdio_host_hal_start_send_data(&s_sdio_host.hal);
		}

		//one block write to sdcard fifo finish
		if((index % SDIO_BLOCK_SIZE == 0) || (index == data_size))	//maybe isn't a full block(though sdcard always use full block)
		{
			/*
			 * wait write end int which means the data has sent to sdcard and get the sdcard's response.
			 * empty mask just mean host FIFO is empty but can't indicate that sdcard has dealed data finish.
			 * write status fail will not set semaphore yet, so it cause here timeout, here set it as tx fail.
			 */
			s_sdio_host.is_tx_blocked = true;
			error_state = rtos_get_semaphore(&(s_sdio_host.tx_sema), SDIO_MAX_TX_WAIT_TIME);
			if(error_state)
			{
				s_sdio_host.is_tx_blocked = false;
				SDIO_HOST_LOGE("TODO:sdio tx data fail:index=%d!\r\n", index);
			}
		}
	}

	return error_state;
}

bk_err_t bk_sdio_host_write_fifo(const uint8_t *write_data, uint32_t data_size)
{
#if defined(CONFIG_SDIO_GDMA_EN)
	if(s_sdio_host.dma_tx_en)
		return sdio_dma_write_fifo(write_data, data_size);
	else
		return sdio_host_cpu_write_fifo(write_data, data_size);
#else
	return sdio_host_cpu_write_fifo(write_data, data_size);
#endif
}

/*
 * Internal API only for SD-CARD Driver.
 * SDIO has pre-read feature, it cause SD-CARD driver gets one more sema if the read block
 * isn't continious addr with previous block address.
 * So add one special API for SD-CARD to discard the previous rx data sema.
 */
void bk_sdio_host_discard_previous_receive_data_sema(void)
{
	SDIO_PM_CHECK_RESTORE(0);

	rtos_get_semaphore(&(s_sdio_host.rx_sema), 0);
}

bk_err_t bk_sdio_host_wait_receive_data(void)
{
	SDIO_HOST_RETURN_ON_NOT_INIT();
	SDIO_PM_CHECK_RESTORE(0);
	bk_err_t error_state = BK_FAIL;

	//CRC err will not set semaphore yet, so it cause here timeout, here set it as rx fail.
	error_state = rtos_get_semaphore(&(s_sdio_host.rx_sema), SDIO_MAX_RX_WAIT_TIME);
	if(error_state != BK_OK)
	{
		SDIO_HOST_LOGI("rx fail\r\n");
	}

	if(s_sdio_host_data_crc_error == true) {
		error_state = BK_ERR_SDIO_HOST_DATA_CRC_FAIL;
		s_sdio_host_data_crc_error = false;
		SDIO_HOST_LOGI("func %s, line %d, return crc error.\r\n", __func__, __LINE__);
	}



	return error_state;
}
#else
bk_err_t bk_sdio_host_write_fifo(const uint8_t *write_data, uint32_t data_size)
{
	BK_RETURN_ON_NULL(write_data);
	SDIO_HOST_RETURN_ON_NOT_INIT();
	SDIO_PM_CHECK_RESTORE(0);

	sdio_host_hal_t *hal = &s_sdio_host.hal;
	uint32_t index = 0;
	uint32 data_tmp = 0;
	uint32_t int_level = 0;
	bk_err_t error_state = BK_OK;

	while ((sdio_host_hal_is_tx_fifo_write_ready(hal)) && (index < data_size)) {
		data_tmp = ((write_data[index] << 24) | (write_data[index + 1] << 16) | (write_data[index + 2] << 8) | write_data[index + 3]);
		index += 4;
		sdio_host_hal_write_fifo(hal, data_tmp);
	}

	int_level = rtos_enter_critical();
	sdio_host_hal_enable_tx_fifo_empty_mask(hal);
	sdio_host_hal_start_send_data(&s_sdio_host.hal);
	s_sdio_host.is_tx_blocked = true;
	rtos_exit_critical(int_level);

	rtos_get_semaphore(&(s_sdio_host.tx_sema), BEKEN_NEVER_TIMEOUT);

	int_level = rtos_enter_critical();
	sdio_host_hal_disable_tx_fifo_empty_mask(hal);
	if (sdio_host_hal_is_data_crc_fail_int_triggered(hal, s_sdio_host.int_status)) {
		error_state = BK_ERR_SDIO_HOST_DATA_CRC_FAIL;
	}
	if (sdio_host_hal_is_data_timeout_int_triggered(hal, s_sdio_host.int_status)) {
		error_state = BK_ERR_SDIO_HOST_DATA_TIMEOUT;
	}
	rtos_exit_critical(int_level);

	return error_state;
}

bk_err_t bk_sdio_host_wait_receive_data(void)
{
	SDIO_HOST_RETURN_ON_NOT_INIT();
	SDIO_PM_CHECK_RESTORE(0);

	bk_err_t error_state = BK_FAIL;
	uint32_t int_status = 0;
	sdio_host_hal_t *hal = &s_sdio_host.hal;

	/* wait for recv data interrupt triggered */
	do {
		int_status = sdio_host_hal_get_interrupt_status(hal);
		sdio_host_hal_clear_data_interrupt_status(hal, int_status);
		SDIO_HOST_LOGI("int_status:%x\r\n", int_status);
	} while (!sdio_host_hal_is_recv_data_interrupt_triggered(hal, int_status));

	if (sdio_host_hal_is_data_recv_end_int_triggered(hal, int_status)) {
		if (sdio_host_hal_is_data_crc_fail_int_triggered(hal, int_status)) {
			error_state = BK_ERR_SDIO_HOST_DATA_CRC_FAIL;
		} else {
			error_state = BK_OK;
		}
	} else {
		if (sdio_host_hal_is_data_timeout_int_triggered(hal, int_status)) {
			error_state = BK_ERR_SDIO_HOST_DATA_TIMEOUT;
		}
	}

	return error_state;
}
#endif

void bk_sdio_host_start_read(void)
{
	SDIO_PM_CHECK_RESTORE(0);

	sdio_host_hal_start_receive_data(&s_sdio_host.hal);
}

#if defined(CONFIG_SDIO_GDMA_EN)
static bk_err_t sdio_dma_read_fifo(uint8_t *read_data, uint32_t data_size)
{
	bk_err_t error_state = BK_FAIL;

	s_sdio_host.rx_total_len = data_size;
	s_sdio_host.rx_transfered_len = 0;
	s_sdio_host.rx_start_addr = (uint32_t)read_data;

	sdio_dma_rx_start((dma_id_t)s_sdio_host.dma_rx_id);

	//wait finish
	error_state = rtos_get_semaphore(&(s_sdio_host.rx_sema), SDIO_MAX_RX_WAIT_TIME * 10);
	if(error_state)
	{
		SDIO_HOST_LOGE("read data fail,request cnt=%d\r\n", data_size);
	}

	return error_state;
}
#endif

bk_err_t bk_sdio_host_read_fifo(uint32_t *save_v_p)
{
#define SDIO_READ_FIFO_TIMEOUT (400)	//us, one word transfer in 13M clock nearly 10us with single wire
	uint32_t i = 0;
	SDIO_PM_CHECK_RESTORE(0);

	while (!sdio_host_hal_is_rx_fifo_read_ready(&s_sdio_host.hal))
	{
		i++;
		delay_us(1);
		if(i == SDIO_READ_FIFO_TIMEOUT) {
			SDIO_HOST_LOGE("FIFO read fail,the return data is invalid\r\n");
			return BK_ERR_SDIO_HOST_READ_DATA_FAIL;
		}
	}

	*save_v_p = sdio_host_hal_read_fifo(&s_sdio_host.hal);

	return BK_OK;
}

static bk_err_t sdio_host_cpu_read_blks_fifo(uint8_t *data, uint32_t blk_cnt)
{
	bk_err_t error_state = BK_OK;
	uint32_t read_data = 0, index = 0;
	uint32_t data_size = blk_cnt * 512;

	//read data from SDIO to buffer
	while (bk_sdio_host_wait_receive_data() == BK_OK) {
		do {
			/* Read data from SDIO Rx fifo */
			error_state = bk_sdio_host_read_fifo(&read_data);
			if(error_state == BK_OK)
			{
				data[index++] = read_data & 0xff;
				data[index++] = (read_data >> 8) & 0xff;
				data[index++] = (read_data >> 16) & 0xff;
				data[index++] = (read_data >> 24) & 0xff;
				//SDIO_HOST_LOGD("read_data:%x, index:%d\r\n", read_data, index);
			}
			else
			{
				SDIO_HOST_LOGW("Read FIFO fail,index=%d\r\n", index);
				break;
			}
		} while ((index % SDIO_BLOCK_SIZE) != 0);

		if (index >= data_size) {
			SDIO_HOST_LOGD("rx data finish bytes:%d\r\n", index);
			break;
		}

//maybe some files in SDCARD and read it takes too much time, then cause WDT timeout
#if (defined(CONFIG_TASK_WDT))
		bk_task_wdt_feed();
#endif
	}

	//CRC fail or some of block timeout and so on
	if (index != data_size)
	{
		error_state = BK_ERR_SDIO_HOST_DATA_CRC_FAIL;
		SDIO_HOST_LOGE("func %s, read data fail,rx real cnt=%d,request cnt=%d, error_state=%d\r\n", __func__, index, data_size, error_state);

	}

	return error_state;
}

bk_err_t bk_sdio_host_read_blks_fifo(uint8_t *read_data, uint32_t blk_cnt)
{
#if defined(CONFIG_SDIO_GDMA_EN)
	if(s_sdio_host.dma_rx_en)
	{
		uint32_t data_size = blk_cnt * 512;
		return sdio_dma_read_fifo(read_data, data_size);
	}
	else
		return sdio_host_cpu_read_blks_fifo(read_data, blk_cnt);
#else
	return sdio_host_cpu_read_blks_fifo(read_data, blk_cnt);
#endif
}

#if (defined(CONFIG_SDIO_V2P0))
static void sdio_host_isr(void)
{
	sdio_host_hal_t *hal = &s_sdio_host.hal;
	uint32_t int_status = sdio_host_hal_get_interrupt_status(&s_sdio_host.hal);
	uint32_t cmd_index = sdio_host_hal_get_cmd_index_interrupt_status(&s_sdio_host.hal, int_status);
	s_sdio_host.int_status = int_status;

	//sdio_host_hal_clear_data_interrupt_status(hal, int_status);

	//TODO:WARNING:sdio_host_hal_is_data_crc_fail_int_triggered should check
	SDIO_HOST_LOGD("sdio isr, cmd_index=%d,int_status:%x\r\n", cmd_index, int_status);

	//CMD:RESP, NO RESP, TIMEOUT
	if(sdio_host_hal_is_cmd_rsp_interrupt_triggered(hal, int_status))
	{
		//CMD END(Some CMD has no response, so has not CRC)
		if(sdio_host_hal_is_cmd_end_interrupt_triggered(hal, int_status))
		{
			uint32_t cmd_rsp_ok = true;
			if(sdio_host_hal_is_cmd_rsp_crc_ok_interrupt_triggered(hal, int_status))	//CRC OK
			{

			}
			else if(sdio_host_hal_is_cmd_rsp_crc_fail_interrupt_triggered(hal, int_status))	//CRC Fail
			{
				//SDIO Host driver no need to care about SDCARD CMD Index,SDCARD driver needs to set
				//the speific CMD Index whether needs CRC check.
				SDIO_HOST_LOGW("sdio receive CMD%d crc fail\r\n", cmd_index);
				cmd_rsp_ok = false;
			}
			else	//some CMD has no response,so has no CRC check.
			{

			}

			if (/*s_sdio_host.is_cmd_blocked &&*/ cmd_rsp_ok) {
				if(rtos_push_to_queue(&s_sdio_host.irq_cmd_msg, &int_status, 0))
				{
					SDIO_HOST_LOGE("sdio push cmd msg fail\r\n");
				}
			}
		}
		else if(sdio_host_hal_is_cmd_rsp_timeout_interrupt_triggered(hal, int_status))	//timeout
		{
			if ((cmd_index != SEND_OP_COND) && (s_sdio_cmd_index != 6)) {
				SDIO_HOST_LOGD("isr sdio wait CMD RSP timeout, int_status=0x%x,cmd_index=%d,s_sdio_cmd_index=%d\r\n", int_status,cmd_index,s_sdio_cmd_index);
			}
		}

		//clear cmd int status
		sdio_host_hal_clear_cmd_rsp_interrupt_status(hal, int_status);
	}
	//TX(host write) DATA
#if defined(CONFIG_SDIO_GDMA_EN)
	else if(sdio_host_hal_is_data_write_end_int_triggered(hal, int_status) ||
		sdio_host_hal_is_fifo_empty_int_triggered(hal, int_status) /* ||
		sdio_host_hal_is_tx_fifo_need_write_int_triggered(hal, int_status) */)
#else
	else if(sdio_host_hal_is_data_write_end_int_triggered(hal, int_status))
#endif
	{
#if defined(CONFIG_SDIO_GDMA_EN)
		if(sdio_host_hal_is_fifo_empty_int_triggered(hal, int_status))
		{
			sdio_host_hal_disable_tx_fifo_empty_mask(hal);
			rtos_set_semaphore(&s_sdio_host.tx_sema);
		}
#endif
		//TODO:check write status
		if (sdio_host_hal_get_wr_status(&s_sdio_host.hal) != 2) {
			for (int sts_index = 0; sts_index < SDIO_GET_WR_STS_MAX_COUNT; sts_index++ ) {
				if (sdio_host_hal_get_wr_status(&s_sdio_host.hal) == 2) {
					SDIO_HOST_LOGD("wr_status ok, i = %d.\r\n", sts_index);
					break;
				}
				if(sts_index == (SDIO_GET_WR_STS_MAX_COUNT - 1) ) {
					SDIO_HOST_LOGE("write data error, wr_status = 0x%x!!!\r\n", sdio_host_hal_get_wr_status(&s_sdio_host.hal));
				}
			}
		}

		{
			SDIO_HOST_LOGD("write blk end\r\n");
#if defined(CONFIG_SDIO_GDMA_EN)
#if 0
			if(s_sdio_host.dma_tx_en)	//WARNING:DMA enable: SDIO,DMA,Software at a-sync status,maybe software loses end isr.
			{
				s_dma_transfer_size_check += 512;	//can't use this value to check whether tx finish:because if CPU-INT is disable or CPU response too slow, then it lost ISR handler.
				if(s_sdio_host.tx_total_len <= s_sdio_host.tx_transfered_len)	//after DMA finish and then set sema to avoid too much sema clear.
				{
					rtos_set_semaphore(&s_sdio_host.tx_sema);
				}
			}
			else
				rtos_set_semaphore(&s_sdio_host.tx_sema);
#endif
#else
			rtos_set_semaphore(&s_sdio_host.tx_sema);
#endif
		}
#if 0	//(defined(CONFIG_SDIO_HOST_CLR_WRITE_INT))
		bk_sdio_tx_fifo_clk_gate_config(1);
		sdio_host_hal_clear_write_data_interrupt_status(hal, int_status);
		sdio_host_hal_clear_write_data_interrupt_status(hal, int_status);
		sdio_host_hal_clear_write_data_interrupt_status(hal, int_status);
		bk_sdio_tx_fifo_clk_gate_config(0);//clr 3 times to wait sd_card a cycle;
#else
		sdio_host_hal_clear_write_data_interrupt_status(hal, int_status);
#endif
	}
	//RX(host read) DATA
	else if(sdio_host_hal_is_data_recv_end_int_triggered(hal, int_status) ||
		/* sdio_host_hal_is_data_recv_need_read_int_triggered(hal, int_status) ||
		sdio_host_hal_is_data_recv_overflow_int_triggered(hal, int_status) || */
		sdio_host_hal_is_data_timeout_int_triggered(hal, int_status))
	{
		if(sdio_host_hal_is_data_crc_ok_int_triggered(hal, int_status)) //CRC OK
		{
#if defined(CONFIG_SDIO_GDMA_EN)
			if(s_sdio_host.dma_rx_en)
				SDIO_HOST_LOGD("DMA read blk end\r\n");	//do nothing
			else
				rtos_set_semaphore(&s_sdio_host.rx_sema);			
#else
			rtos_set_semaphore(&s_sdio_host.rx_sema);
#endif
			sdio_host_hal_clear_read_data_interrupt_status(hal, int_status);
		}
		else if(sdio_host_hal_is_data_crc_fail_int_triggered(hal, int_status))	//CRC Fail
		{
			//multi-block read:the sdio has pre-read feature(after read finish, the sdio still reads one block data),
			//so it always read more data then SW requires,
			//but the data is invalid,so reset the sd state if CRC fail.

			bk_sdio_host_reset_sd_state();

			//TODO:If the data is really CRC fail, should notify APP the data received is error.
			SDIO_HOST_LOGE("TODO:read data crc error!!!\r\n");
#if 1	//just not set sema cause rx data timeout, which cause rx fail.
			s_sdio_host_data_crc_error = true;
			rtos_set_semaphore(&s_sdio_host.rx_sema);
#endif
		}
		else if(sdio_host_hal_is_data_timeout_int_triggered(hal, int_status))	//timeout
		{
			SDIO_HOST_LOGE("TODO:read data timeout error!!!\r\n");
			sdio_host_hal_clear_read_data_timeout_interrupt_status(hal, int_status);
		}
		/*
		 * !!!NOTES!!! SDIO pre-read feature time-sequence issue!!!
		 * If read data finish,SW will clear FIFO(bk_sdio_host_reset_sd_state) when stop read,
		 * but maybe the ASIC have pre-read one block data finished but SDIO ISR doesn't response.
		 * After FIFO reset, ISR response found the CRC isn't OK and isn't ERROR,
		 * which cause ISR status isn't cleared, so the SDIO ISR will entry and exit forever
		 * cause system abnormall(watchdog timeout reset)
		 */
		else
		{
			SDIO_HOST_LOGE("read data status err:int_status=0x%08x!!!\r\n", int_status);
			sdio_host_hal_clear_read_data_interrupt_status(hal, int_status);
		}
	}
	else {
		SDIO_HOST_LOGD("sdio isr no deal:cmd_index=%d,int_status=%x\r\n", cmd_index, int_status);
	}
}

#else
static void sdio_host_isr(void)
{
	sdio_host_hal_t *hal = &s_sdio_host.hal;
	uint32_t int_status = sdio_host_hal_get_interrupt_status(hal);
	s_sdio_host.int_status = int_status;

	SDIO_HOST_LOGD("enter sdio_host isr, int_status:%x\r\n", int_status);
	sdio_host_hal_clear_data_interrupt_status(hal, int_status);

	if (sdio_host_hal_is_data_write_end_int_triggered(hal, int_status) ||
		sdio_host_hal_is_fifo_empty_int_triggered(hal, int_status) ||
		sdio_host_hal_is_data_crc_fail_int_triggered(hal, int_status) ||
		sdio_host_hal_is_data_timeout_int_triggered(hal, int_status)) {
		/* need clear tx fifo empty mask, otherwise sdio_isr will triggered forever */
		sdio_host_hal_disable_tx_fifo_empty_mask(hal);
		if (s_sdio_host.is_tx_blocked) {
			rtos_set_semaphore(&s_sdio_host.tx_sema);
			s_sdio_host.is_tx_blocked = false;
		}
	}
}
#endif
