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

#include <driver/dma.h>
#include <driver/gpio.h>
#include <driver/int.h>
#include <driver/spi.h>
#include "clock_driver.h"
#include "dma_hal.h"
#include "gpio_driver.h"
#include "icu_driver.h"
#include <common/bk_include.h>
#include <os/mem.h>
#include "power_driver.h"
#include <os/os.h>
#include "spi_driver.h"
#include "spi_hal.h"
#include "spi_statis.h"
#include "spi_config.h"
#include "sys_driver.h"
#include "bk_misc.h"
#if defined(CONFIG_SPE) && (CONFIG_SPE == 1)
#include "security.h"
#endif
#if defined(CONFIG_SPI_PM_CB_SUPPORT)
#include <modules/pm.h>
#endif

#ifdef CONFIG_FREERTOS_SMP
#include "spinlock.h"
static volatile spinlock_t spi_spin_lock = SPIN_LOCK_INIT;
#endif // CONFIG_FREERTOS_SMP

typedef struct {
	spi_hal_t hal;
	uint8_t id_init_bits;
	uint8_t * tx_buf;
	uint32_t tx_size;
	uint8_t * rx_buf;
	uint32_t rx_size;
	volatile uint32_t rx_offset;
	bool is_tx_blocked;
	bool is_sw_tx_finished;
	bool is_rx_blocked;
	beken_semaphore_t tx_sema;
	beken_semaphore_t rx_sema;
	dma_id_t spi_tx_dma_chan;
	dma_id_t spi_rx_dma_chan;
	bool dma_inited;
#if defined(CONFIG_SPI_PM_CB_SUPPORT)
	uint32_t pm_backup[SPI_PM_BACKUP_REG_NUM];
	uint8_t pm_backup_is_valid;
#endif
} spi_driver_t;

typedef struct {
	spi_isr_t callback;
	void *param;
} spi_callback_t;

#define SPI_RETURN_ON_NOT_INIT() do {\
	if (!s_spi_driver_is_init) {\
		SPI_LOGE("SPI driver not init\r\n");\
		return BK_ERR_SPI_NOT_INIT;\
	}\
} while(0)

#define SPI_RETURN_ON_ID_NOT_INIT(id) do {\
	if (!s_spi[id].id_init_bits) {\
		SPI_LOGE("SPI(%d) not init\r\n", id);\
		return BK_ERR_SPI_ID_NOT_INIT;\
	}\
} while(0)

#define SPI_RETURN_ON_INVALID_ID(id) do {\
	if ((id) >= SOC_SPI_UNIT_NUM) {\
		SPI_LOGE("SPI id number(%d) is invalid\r\n", (id));\
		return BK_ERR_SPI_INVALID_ID;\
	}\
} while(0)

#if 0 //defined(CONFIG_SPE)
#define SPI_CHECK_SECURE(id) do {\
	switch (id) {\
	case SPI_ID_0:\
		BK_ASSERT(DEV_IS_SECURE(SPI0) == 1);\
		break;\
	case SPI_ID_1:\
		BK_ASSERT(DEV_IS_SECURE(SPI1) == 1);\
		break;\
	default:\
		break;\
	}\
} while(0)
#else
#define SPI_CHECK_SECURE(id)
#endif

#define SPI_SET_PIN(id) do {\
	gpio_dev_unmap(SPI##id##_LL_CSN_PIN);\
	gpio_dev_unmap(SPI##id##_LL_SCK_PIN);\
	gpio_dev_unmap(SPI##id##_LL_MOSI_PIN);\
	gpio_dev_unmap(SPI##id##_LL_MISO_PIN);\
	gpio_dev_map(SPI##id##_LL_CSN_PIN, GPIO_DEV_SPI##id##_CSN);\
	gpio_dev_map(SPI##id##_LL_SCK_PIN, GPIO_DEV_SPI##id##_SCK);\
	gpio_dev_map(SPI##id##_LL_MOSI_PIN, GPIO_DEV_SPI##id##_MOSI);\
	gpio_dev_map(SPI##id##_LL_MISO_PIN, GPIO_DEV_SPI##id##_MISO);\
	bk_gpio_pull_up(SPI##id##_LL_CSN_PIN);\
	bk_gpio_pull_up(SPI##id##_LL_SCK_PIN);\
	bk_gpio_set_capacity(SPI##id##_LL_CSN_PIN, 0);\
	bk_gpio_set_capacity(SPI##id##_LL_SCK_PIN, 1);\
	bk_gpio_set_capacity(SPI##id##_LL_MOSI_PIN, 0);\
	bk_gpio_set_capacity(SPI##id##_LL_MISO_PIN, 0);\
} while(0)

static spi_driver_t s_spi[SOC_SPI_UNIT_NUM] = {
	{
		.hal.hw = (spi_hw_t *)(SOC_SPI_REG_BASE),
	},
#if (SOC_SPI_UNIT_NUM > 1)
	{
		.hal.hw = (spi_hw_t *)(SOC_SPI1_REG_BASE),
	}
#endif
};
static bool s_spi_driver_is_init = false;
static volatile spi_id_t s_current_spi_dma_wr_id;
static volatile spi_id_t s_current_spi_dma_rd_id;
static spi_callback_t s_spi_rx_isr[SOC_SPI_UNIT_NUM] = {NULL};
static spi_callback_t s_spi_tx_finish_isr[SOC_SPI_UNIT_NUM] = {NULL};
static spi_callback_t s_spi_rx_finish_isr[SOC_SPI_UNIT_NUM] = {NULL};

static void spi_isr(void);
#if (SOC_SPI_UNIT_NUM > 1)
static void spi2_isr(void);
#endif
#if (SOC_SPI_UNIT_NUM > 2)
static void spi3_isr(void);
#endif

static inline uint32_t spi_enter_critical()
{
       uint32_t flags = rtos_disable_int();

#ifdef CONFIG_FREERTOS_SMP
       spin_lock(&spi_spin_lock);
#endif // CONFIG_FREERTOS_SMP

       return flags;
}

static inline void spi_exit_critical(uint32_t flags)
{
#ifdef CONFIG_FREERTOS_SMP
       spin_unlock(&spi_spin_lock);
#endif // CONFIG_FREERTOS_SMP

       rtos_enable_int(flags);
}

static void spi_init_gpio(spi_id_t id)
{
	switch (id) {
	case SPI_ID_0:
		SPI_SET_PIN(0);
		break;
#if (SOC_SPI_UNIT_NUM > 1)
	case SPI_ID_1:
		SPI_SET_PIN(1);
		break;
#endif
#if (SOC_SPI_UNIT_NUM > 2)
	case SPI_ID_2:
		SPI_SET_PIN(2);
		break;
#endif
	default:
		break;
	}
#if (!defined(CONFIG_SYSTEM_CTRL))
	gpio_spi_sel(GPIO_SPI_MAP_MODE1);
#endif
}

bk_err_t bk_spi_set_role(spi_id_t id, spi_role_t role)
{
	if(role == SPI_ROLE_SLAVE) {
		spi_hal_set_role_slave(&s_spi[id].hal);
	} else {
		spi_hal_set_role_master(&s_spi[id].hal);
	}
	return BK_OK;
}

bk_err_t bk_spi_clear_tx_fifo(spi_id_t id)
{
	spi_hal_clear_tx_fifo(&s_spi[id].hal);
	return BK_OK;
}

bk_err_t bk_spi_clear_rx_fifo(spi_id_t id)
{
	spi_hal_clear_rx_fifo(&s_spi[id].hal);
	return BK_OK;
}

#if (defined(CONFIG_SYSTEM_CTRL))
static void spi_clock_enable(spi_id_t id)
{
	switch(id)
	{
		case SPI_ID_0:
			sys_drv_dev_clk_pwr_up(CLK_PWR_ID_SPI_1, CLK_PWR_CTRL_PWR_UP);
			break;
#if (SOC_SPI_UNIT_NUM > 1)
		case SPI_ID_1:
			sys_drv_dev_clk_pwr_up(CLK_PWR_ID_SPI_2, CLK_PWR_CTRL_PWR_UP);
			break;
#endif
		default:
			break;
	}
}

static void spi_clock_disable(spi_id_t id)
{
#if defined(CONFIG_SOC_BK7256XX)
	//disable apll clock, and restore the clock config
	sys_drv_apll_en(false);
	sys_drv_cb_manu_val_set(SPI_DIS_APLL_MANU_VAL);
	sys_drv_ana_reg11_vsel_set(SPI_DIS_APLL_VSEL_VAL);
#endif

	switch(id)
	{
		case SPI_ID_0:
			sys_drv_dev_clk_pwr_up(CLK_PWR_ID_SPI_1, CLK_PWR_CTRL_PWR_DOWN);
			break;
#if (SOC_SPI_UNIT_NUM > 1)
		case SPI_ID_1:
			sys_drv_dev_clk_pwr_up(CLK_PWR_ID_SPI_2, CLK_PWR_CTRL_PWR_DOWN);
			break;
#endif
		default:
			break;
	}
}

static void spi_interrupt_enable(spi_id_t id)
{
	switch(id)
	{
		case SPI_ID_0:
			sys_drv_int_enable(SPI_INTERRUPT_CTRL_BIT);
			break;
#if (SOC_SPI_UNIT_NUM > 1)
		case SPI_ID_1:
			sys_drv_int_enable(SPI1_INTERRUPT_CTRL_BIT);
			break;
#endif
		default:
			break;
	}
}

static void spi_interrupt_disable(spi_id_t id)
{
	switch(id)
	{
		case SPI_ID_0:
			sys_drv_int_disable(SPI_INTERRUPT_CTRL_BIT);
			break;
#if (SOC_SPI_UNIT_NUM > 1)
		case SPI_ID_1:
			sys_drv_int_disable(SPI1_INTERRUPT_CTRL_BIT);
			break;
#endif
		default:
			break;
	}
}
#endif

/* 1. power up spi
 * 2. set clk
 * 3. set gpio as spi
 * 4. icu enable interrupt
 */
static bk_err_t spi_id_init_common(spi_id_t id)
{
	int ret = 0;

#if (defined(CONFIG_SYSTEM_CTRL))
	spi_clock_enable(id);
	spi_interrupt_enable(id);
#else
	power_up_spi(id);
	clk_set_spi_clk_26m(id);
	icu_enable_spi_interrupt(id);
#endif
	spi_init_gpio(id);

	if (s_spi[id].tx_sema == NULL) {
		ret = rtos_init_semaphore(&(s_spi[id].tx_sema), 1);
		BK_ASSERT(kNoErr == ret); /* ASSERT VERIFIED */
	}
	if (s_spi[id].rx_sema == NULL) {
		ret = rtos_init_semaphore(&(s_spi[id].rx_sema), 1);
		BK_ASSERT(kNoErr == ret); /* ASSERT VERIFIED */
	}
	s_spi[id].is_tx_blocked = false;
	s_spi[id].is_sw_tx_finished = true;
	s_spi[id].is_rx_blocked = false;
	s_spi[id].id_init_bits |= BIT(id);

	return ret;
}

static void spi_id_deinit_common(spi_id_t id)
{
	spi_hal_stop_common(&s_spi[id].hal);

#if (defined(CONFIG_SYSTEM_CTRL))
	spi_clock_disable(id);
	spi_interrupt_disable(id);
#else
	icu_disable_spi_interrupt(id);
	power_down_spi(id);
#endif
	if(s_spi[id].tx_sema){
		rtos_deinit_semaphore(&(s_spi[id].tx_sema));
	}

	if(s_spi[id].rx_sema){
		rtos_deinit_semaphore(&(s_spi[id].rx_sema));
	}
	s_spi[id].id_init_bits &= ~BIT(id);
}

static void spi_id_write_bytes_common(spi_id_t id)
{
	uint16_t *data = (uint16_t *)s_spi[id].tx_buf;
	for (int i = 0; i < s_spi[id].tx_size; i++) {
		BK_WHILE (!spi_hal_is_tx_fifo_wr_ready(&s_spi[id].hal));
		if(s_spi[id].hal.hw->ctrl.bit_width == SPI_BIT_WIDTH_8BITS){
			spi_hal_write_byte(&s_spi[id].hal, s_spi[id].tx_buf[i]);
		} else {
			spi_hal_write_byte(&s_spi[id].hal, data[i]);
		}
	}
}

static uint32_t spi_id_read_bytes_common(spi_id_t id)
{
	uint16_t data = 0;
	uint32_t offset = s_spi[id].rx_offset;
	uint16_t *data_buf = (uint16_t *)s_spi[id].rx_buf;

	while (spi_hal_read_byte(&s_spi[id].hal, &data) == BK_OK) {
		if ((s_spi[id].rx_buf) && (offset < s_spi[id].rx_size)) {
			if(s_spi[id].hal.hw->ctrl.bit_width == SPI_BIT_WIDTH_8BITS){
				s_spi[id].rx_buf[offset++] = (uint8_t)(data&0xFF);
			} else {
				data_buf[offset++] = data;
			}
		}
	}
	s_spi[id].rx_offset = offset;
	SPI_LOGD("spi offset:%d\r\n", s_spi[id].rx_offset);
	return offset;
}

#if defined(CONFIG_SPI_DMA)

static void spi_dma_tx_finish_handler(dma_id_t id)
{
	SPI_LOGD("[%s] spi_id:%d\r\n", __func__, s_current_spi_dma_wr_id);

	if (s_spi_tx_finish_isr[s_current_spi_dma_wr_id].callback){
		s_spi_tx_finish_isr[s_current_spi_dma_wr_id].callback(s_current_spi_dma_wr_id,s_spi_tx_finish_isr[s_current_spi_dma_wr_id].param);
	}
	if (s_spi[s_current_spi_dma_wr_id].is_tx_blocked) {
		rtos_set_semaphore(&s_spi[s_current_spi_dma_wr_id].tx_sema);
		s_spi[s_current_spi_dma_wr_id].is_tx_blocked = false;
	}
}

static void spi_dma_rx_finish_handler(dma_id_t id)
{
	SPI_LOGD("[%s] spi_id:%d\r\n", __func__, s_current_spi_dma_rd_id);
	if (s_spi_rx_finish_isr[s_current_spi_dma_rd_id].callback){
		s_spi_rx_finish_isr[s_current_spi_dma_rd_id].callback(s_current_spi_dma_rd_id,s_spi_rx_finish_isr[s_current_spi_dma_rd_id].param);
	}
	if (s_spi[s_current_spi_dma_rd_id].is_rx_blocked) {
		rtos_set_semaphore(&s_spi[s_current_spi_dma_rd_id].rx_sema);
		s_spi[s_current_spi_dma_rd_id].is_rx_blocked = false;
	}
}

static void spi_dma_tx_init(spi_id_t id, dma_id_t spi_tx_dma_chan, dma_data_width_t spi_tx_dma_width)
{
	dma_config_t dma_config = {0};
	spi_int_config_t int_cfg_table[] = SPI_INT_CONFIG_TABLE;

	dma_config.mode = DMA_WORK_MODE_SINGLE;
	dma_config.chan_prio = 0;
	dma_config.src.dev = DMA_DEV_DTCM;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.src.addr_loop_en = DMA_ADDR_LOOP_DISABLE;
	dma_config.dst.width = spi_tx_dma_width;
	dma_config.dst.start_addr = SPI_R_DATA(id);
	dma_config.dst.dev = int_cfg_table[id].dma_dev;

	BK_LOG_ON_ERR(bk_dma_init(spi_tx_dma_chan, &dma_config));
	BK_LOG_ON_ERR(bk_dma_register_isr(spi_tx_dma_chan, NULL, spi_dma_tx_finish_handler));
	BK_LOG_ON_ERR(bk_dma_enable_finish_interrupt(spi_tx_dma_chan));
#if defined(CONFIG_SPE) && (CONFIG_SPE == 1) && defined(CONFIG_GDMA_HW_V2PX)
	BK_LOG_ON_ERR(bk_dma_set_dest_sec_attr(spi_tx_dma_chan, DMA_ATTR_SEC));
	BK_LOG_ON_ERR(bk_dma_set_src_sec_attr(spi_tx_dma_chan, DMA_ATTR_SEC));
#endif
}

static void spi_dma_rx_init(spi_id_t id, dma_id_t spi_rx_dma_chan, dma_data_width_t spi_rx_dma_width)
{
	dma_config_t dma_config = {0};
	spi_int_config_t int_cfg_table[] = SPI_RX_INT_CONFIG_TABLE;

	dma_config.mode = DMA_WORK_MODE_SINGLE;
	dma_config.chan_prio = 0;
	dma_config.src.dev = int_cfg_table[id].dma_dev;
	dma_config.src.width = spi_rx_dma_width;
	dma_config.src.start_addr = SPI_R_DATA(id);
	dma_config.dst.dev = DMA_DEV_DTCM;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
	dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.dst.addr_loop_en = DMA_ADDR_LOOP_DISABLE;

	BK_LOG_ON_ERR(bk_dma_init(spi_rx_dma_chan, &dma_config));
	BK_LOG_ON_ERR(bk_dma_register_isr(spi_rx_dma_chan, NULL, spi_dma_rx_finish_handler));
	BK_LOG_ON_ERR(bk_dma_enable_finish_interrupt(spi_rx_dma_chan));
#if defined(CONFIG_SPE) && (CONFIG_SPE == 1) && defined(CONFIG_GDMA_HW_V2PX)
	BK_LOG_ON_ERR(bk_dma_set_dest_sec_attr(spi_rx_dma_chan, DMA_ATTR_SEC));
	BK_LOG_ON_ERR(bk_dma_set_src_sec_attr(spi_rx_dma_chan, DMA_ATTR_SEC));
#endif
}

#endif /* CONFIG_SPI_DMA */

#if (defined(CONFIG_SPI_PM_CB_SUPPORT))
#define SPI_PM_CHECK_RESTORE(id) do {\
	if ((id == SPI_ID_1) && bk_pm_module_lv_sleep_state_get(PM_DEV_ID_SPI_2)) {\
		bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_SPI1, PM_POWER_MODULE_STATE_ON);\
		bk_spi_restore(0, (void *)id);\
		bk_pm_module_lv_sleep_state_clear(PM_DEV_ID_SPI_2); \
	}\
} while(0)

static int bk_spi_backup(uint64_t sleep_time, void *args)
{
	spi_id_t id = (spi_id_t)args;
	SPI_RETURN_ON_NOT_INIT();
	// SPI_RETURN_ON_ID_NOT_INIT(id);
	if (!s_spi[id].pm_backup_is_valid)
	{
		s_spi[id].pm_backup_is_valid = 1;
		spi_hal_backup(&s_spi[id].hal, &s_spi[id].pm_backup[0]);
		spi_clock_disable(id);
	}
	return BK_OK;
}
static int bk_spi_restore(uint64_t sleep_time, void *args)
{
	spi_id_t id = (spi_id_t)args;
	SPI_RETURN_ON_NOT_INIT();
	//SPI_RETURN_ON_ID_NOT_INIT(id);
	if (s_spi[id].pm_backup_is_valid)
	{
		spi_clock_enable(id);
		spi_hal_restore(&s_spi[id].hal, &s_spi[id].pm_backup[0]);
		s_spi[id].pm_backup_is_valid = 0;
	}
	return BK_OK;
}
#else
#define SPI_PM_CHECK_RESTORE(id)
#endif

bk_err_t bk_spi_driver_init(void)
{
	if (s_spi_driver_is_init) {
		return BK_OK;
	}

	spi_int_config_t int_config_table[] = SPI_INT_CONFIG_TABLE;
	os_memset(&s_spi_rx_isr, 0, sizeof(s_spi_rx_isr));
	os_memset(&s_spi_tx_finish_isr, 0, sizeof(s_spi_tx_finish_isr));
	for (int id = SPI_ID_0; id < SPI_ID_MAX; id++) {
#if (defined(CONFIG_SPI_PM_CB_SUPPORT))
		if (id == SPI_ID_0) {
			//SPI_ID_0 of BK7236 is in AON domain
		} else if (id == SPI_ID_1) {
			bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_SPI1, PM_POWER_MODULE_STATE_ON);
		}
#endif
		spi_int_config_t *cur_int_cfg = &int_config_table[id];
		bk_int_isr_register(cur_int_cfg->int_src, cur_int_cfg->isr, NULL);
		s_spi[id].hal.id = id;
		spi_hal_reset_config_to_default(&s_spi[id].hal);
	}
	spi_statis_init();
	s_spi_driver_is_init = true;

	return BK_OK;
}

bk_err_t bk_spi_driver_deinit(void)
{
	if (!s_spi_driver_is_init) {
		return BK_OK;
	}
	spi_int_config_t int_cfg_table[] = SPI_INT_CONFIG_TABLE;
	for (int id = SPI_ID_0; id < SPI_ID_MAX; id++) {
		spi_id_deinit_common(id);
		bk_int_isr_unregister(int_cfg_table[id].int_src);
#if (defined(CONFIG_SPI_PM_CB_SUPPORT))
		if (id == SPI_ID_0) {
			//SPI_ID_0 of BK7236 is in AON domain
		} else if (id == SPI_ID_1) {
			bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_SPI1, PM_POWER_MODULE_STATE_OFF);
		}
#endif
	}

	s_spi_driver_is_init = false;

	return BK_OK;
}

bk_err_t bk_spi_init(spi_id_t id, const spi_config_t *config)
{
	BK_RETURN_ON_NULL(config);
	SPI_RETURN_ON_NOT_INIT();
	SPI_RETURN_ON_INVALID_ID(id);
	SPI_CHECK_SECURE(id);

#if (defined(CONFIG_SPI_PM_CB_SUPPORT))
	pm_cb_conf_t enter_config = {bk_spi_backup, (void *)id};
	if (id == SPI_ID_1) {
		bk_pm_module_lv_sleep_state_clear(PM_DEV_ID_SPI_2);
		bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_SPI1, PM_POWER_MODULE_STATE_ON);
		bk_pm_sleep_register_cb(PM_MODE_LOW_VOLTAGE, PM_DEV_ID_SPI_2, &enter_config, NULL);
	}
#endif
	spi_id_init_common(id);
	spi_hal_init(&s_spi[id].hal);
	spi_hal_configure(&s_spi[id].hal, config);
	spi_hal_start_common(&s_spi[id].hal);

#if (defined(CONFIG_SPI_DMA))
	if (!s_spi[id].dma_inited) {
		s_spi[id].spi_tx_dma_chan = bk_dma_alloc(DMA_DEV_GSPI0 + id * 2);
		s_spi[id].spi_rx_dma_chan = bk_dma_alloc(DMA_DEV_GSPI0_RX + id * 2);

		s_spi[id].dma_inited = true;
	}

	if (config->dma_mode) {
#if (!defined(CONFIG_SYSTEM_CTRL))
		gpio_spi_sel(GPIO_SPI_MAP_MODE0);
#endif
		spi_dma_tx_init(id, s_spi[id].spi_tx_dma_chan, config->spi_tx_dma_width);
		spi_dma_rx_init(id, s_spi[id].spi_rx_dma_chan, config->spi_rx_dma_width);
	}
#endif

	return BK_OK;
}

bk_err_t bk_spi_deinit(spi_id_t id)
{
	SPI_RETURN_ON_NOT_INIT();
	SPI_RETURN_ON_INVALID_ID(id);

#if defined(CONFIG_SPI_DMA)
    if (s_spi[id].dma_inited) {
        if (s_spi[id].spi_tx_dma_chan != DMA_ID_MAX) {
            BK_LOG_ON_ERR(bk_dma_free(DMA_DEV_GSPI0 + id * 2, s_spi[id].spi_tx_dma_chan));
            s_spi[id].spi_tx_dma_chan = DMA_ID_MAX;
        }
        if (s_spi[id].spi_rx_dma_chan != DMA_ID_MAX) {
            BK_LOG_ON_ERR(bk_dma_free(DMA_DEV_GSPI0_RX + id * 2, s_spi[id].spi_rx_dma_chan));
            s_spi[id].spi_rx_dma_chan = DMA_ID_MAX;
        }
        s_spi[id].dma_inited = false;
    }
#endif

	spi_id_deinit_common(id);
#if (defined(CONFIG_SPI_PM_CB_SUPPORT))
	if (id == SPI_ID_1) {
		bk_pm_sleep_unregister_cb(PM_MODE_LOW_VOLTAGE, PM_DEV_ID_SPI_2, true, true);
		bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BAKP_SPI1, PM_POWER_MODULE_STATE_OFF);
	}
#endif

	return BK_OK;
}

bk_err_t bk_spi_set_mode(spi_id_t id, spi_mode_t mode)
{
	SPI_RETURN_ON_NOT_INIT();
	SPI_RETURN_ON_INVALID_ID(id);
	SPI_PM_CHECK_RESTORE(id);

	spi_hal_t *hal = &s_spi[id].hal;
	switch (mode) {
	case SPI_POL_MODE_0:
		spi_hal_set_cpol(hal, SPI_POLARITY_LOW);
		spi_hal_set_cpha(hal, SPI_PHASE_1ST_EDGE);
		break;
	case SPI_POL_MODE_1:
		spi_hal_set_cpol(hal, SPI_POLARITY_LOW);
		spi_hal_set_cpha(hal, SPI_PHASE_2ND_EDGE);
		break;
	case SPI_POL_MODE_2:
		spi_hal_set_cpol(hal, SPI_POLARITY_HIGH);
		spi_hal_set_cpha(hal, SPI_PHASE_1ST_EDGE);
		break;
	case SPI_POL_MODE_3:
	default:
		spi_hal_set_cpol(hal, SPI_POLARITY_HIGH);
		spi_hal_set_cpha(hal, SPI_PHASE_2ND_EDGE);
		break;
	}
	return BK_OK;
}

bk_err_t bk_spi_set_bit_width(spi_id_t id, spi_bit_width_t bit_width)
{
	SPI_RETURN_ON_NOT_INIT();
	SPI_RETURN_ON_INVALID_ID(id);
	SPI_PM_CHECK_RESTORE(id);
	spi_hal_set_bit_width(&s_spi[id].hal, bit_width);
	return BK_OK;
}

bk_err_t bk_spi_set_wire_mode(spi_id_t id, spi_wire_mode_t wire_mode)
{
	SPI_RETURN_ON_NOT_INIT();
	SPI_RETURN_ON_INVALID_ID(id);
	SPI_PM_CHECK_RESTORE(id);
	spi_hal_set_wire_mode(&s_spi[id].hal, wire_mode);
	return BK_OK;
}

bk_err_t bk_spi_set_baud_rate(spi_id_t id, uint32_t baud_rate)
{
	SPI_RETURN_ON_NOT_INIT();
	SPI_RETURN_ON_INVALID_ID(id);
	SPI_PM_CHECK_RESTORE(id);
	spi_hal_set_baud_rate(&s_spi[id].hal, baud_rate);
	return BK_OK;
}

bk_err_t bk_spi_set_bit_order(spi_id_t id, spi_bit_order_t bit_order)
{
	SPI_RETURN_ON_NOT_INIT();
	SPI_RETURN_ON_INVALID_ID(id);
	SPI_PM_CHECK_RESTORE(id);
	spi_hal_set_first_bit(&s_spi[id].hal, bit_order);
	return BK_OK;
}

bk_err_t bk_spi_register_rx_isr(spi_id_t id, spi_isr_t isr, void *param)
{
	SPI_RETURN_ON_INVALID_ID(id);
	SPI_PM_CHECK_RESTORE(id);
	uint32_t int_level = spi_enter_critical();
	s_spi_rx_isr[id].callback = isr;
	s_spi_rx_isr[id].param = param;
	spi_exit_critical(int_level);
	return BK_OK;
}

bk_err_t bk_spi_register_rx_finish_isr(spi_id_t id, spi_isr_t isr, void *param)
{
	SPI_RETURN_ON_INVALID_ID(id);
	SPI_PM_CHECK_RESTORE(id);
	uint32_t int_level = spi_enter_critical();
	s_spi_rx_finish_isr[id].callback = isr;
	s_spi_rx_finish_isr[id].param = param;
	spi_exit_critical(int_level);
	return BK_OK;
}


bk_err_t bk_spi_register_tx_finish_isr(spi_id_t id, spi_isr_t isr, void *param)
{
	SPI_RETURN_ON_INVALID_ID(id);
	SPI_PM_CHECK_RESTORE(id);
	uint32_t int_level = spi_enter_critical();
	s_spi_tx_finish_isr[id].callback = isr;
	s_spi_tx_finish_isr[id].param = param;
	spi_exit_critical(int_level);
	return BK_OK;
}

bk_err_t bk_spi_unregister_rx_isr(spi_id_t id)
{
	SPI_RETURN_ON_INVALID_ID(id);
	SPI_PM_CHECK_RESTORE(id);
	uint32_t int_level = spi_enter_critical();
	s_spi_rx_isr[id].callback = NULL;
	s_spi_rx_isr[id].param = NULL;
	spi_exit_critical(int_level);
	return BK_OK;
}

bk_err_t bk_spi_unregister_rx_finish_isr(spi_id_t id)
{
	SPI_RETURN_ON_INVALID_ID(id);
	SPI_PM_CHECK_RESTORE(id);
	uint32_t int_level = spi_enter_critical();
	s_spi_rx_finish_isr[id].callback = NULL;
	s_spi_rx_finish_isr[id].param = NULL;
	spi_exit_critical(int_level);
	return BK_OK;
}


bk_err_t bk_spi_unregister_tx_finish_isr(spi_id_t id)
{
	SPI_RETURN_ON_INVALID_ID(id);
	SPI_PM_CHECK_RESTORE(id);
	uint32_t int_level = spi_enter_critical();
	s_spi_tx_finish_isr[id].callback = NULL;
	s_spi_tx_finish_isr[id].param = NULL;
	spi_exit_critical(int_level);
	return BK_OK;
}


bk_err_t bk_spi_write_bytes(spi_id_t id, const void *data, uint32_t size)
{
	SPI_RETURN_ON_NOT_INIT();
	SPI_RETURN_ON_INVALID_ID(id);
	SPI_RETURN_ON_ID_NOT_INIT(id);
	BK_RETURN_ON_NULL(data);
	SPI_PM_CHECK_RESTORE(id);

	uint32_t int_level = spi_enter_critical();
	s_spi[id].tx_buf = (uint8_t *)data;
	if (s_spi[id].hal.hw->ctrl.bit_width == SPI_BIT_WIDTH_8BITS) {
		s_spi[id].tx_size = size;
	} else {
		s_spi[id].tx_size = size>>1;
	}
	s_spi[id].is_sw_tx_finished = false;
	s_spi[id].is_tx_blocked = true;
	s_spi[id].rx_size = size;
	s_spi[id].rx_offset = 0;
	spi_hal_clear_tx_fifo(&s_spi[id].hal);
	spi_hal_set_tx_trans_len(&s_spi[id].hal, s_spi[id].tx_size);
#if defined(CONFIG_SPI_SUPPORT_TX_FIFO_WR_READY)
	spi_hal_disable_tx_fifo_int(&s_spi[id].hal);
#else
	spi_hal_enable_tx_fifo_int(&s_spi[id].hal);
#endif
	spi_hal_enable_tx(&s_spi[id].hal);
	spi_exit_critical(int_level);

#if defined(CONFIG_SPI_SUPPORT_TX_FIFO_WR_READY)
	s_spi[id].is_sw_tx_finished = true;
	spi_id_write_bytes_common(id); /* to solve slave write */
#endif

	rtos_get_semaphore(&s_spi[id].tx_sema, BEKEN_NEVER_TIMEOUT);

	int_level = spi_enter_critical();
	spi_hal_disable_tx_fifo_int(&s_spi[id].hal);
	spi_hal_disable_tx(&s_spi[id].hal);
	spi_exit_critical(int_level);

	return BK_OK;
}

bk_err_t bk_spi_read_bytes(spi_id_t id, void *data, uint32_t size)
{
	SPI_RETURN_ON_NOT_INIT();
	SPI_RETURN_ON_INVALID_ID(id);
	SPI_RETURN_ON_ID_NOT_INIT(id);
	BK_RETURN_ON_NULL(data);
	SPI_PM_CHECK_RESTORE(id);

	uint32_t int_level = spi_enter_critical();
	if (s_spi[id].hal.hw->ctrl.bit_width == SPI_BIT_WIDTH_8BITS) {
		s_spi[id].rx_size = size;
	} else {
		s_spi[id].rx_size = size>>1;
	}
	s_spi[id].rx_buf = (uint8_t *)data;
	s_spi[id].rx_offset = 0;
	s_spi[id].is_rx_blocked = true;
	spi_hal_set_rx_trans_len(&s_spi[id].hal, s_spi[id].rx_size);
	spi_hal_clear_rx_fifo(&s_spi[id].hal);
	spi_hal_enable_rx_fifo_int(&s_spi[id].hal);
	spi_hal_enable_rx_finish_int(&s_spi[id].hal);
	spi_hal_enable_rx(&s_spi[id].hal);
#if !defined(CONFIG_SPI_SUPPORT_TX_FIFO_WR_READY)
	/* special for bk7251, bk7251 need enable tx fifo int, otherwize spi_isr will not triggered */
	spi_hal_enable_tx_fifo_int(&s_spi[id].hal);
#endif
	spi_exit_critical(int_level);

	rtos_get_semaphore(&(s_spi[id].rx_sema), BEKEN_NEVER_TIMEOUT);

	int_level = spi_enter_critical();
	spi_hal_disable_rx(&s_spi[id].hal);
	spi_hal_disable_tx_fifo_int(&s_spi[id].hal);
	spi_hal_disable_rx_fifo_int(&s_spi[id].hal);
	s_spi[id].rx_size = 0;
	s_spi[id].rx_offset = 0;
	s_spi[id].rx_buf = NULL;
	spi_exit_critical(int_level);

	return BK_OK;
}

bk_err_t bk_spi_transmit(spi_id_t id, const void *tx_data, uint32_t tx_size, void *rx_data, uint32_t rx_size)
{
	SPI_RETURN_ON_INVALID_ID(id);
	SPI_RETURN_ON_ID_NOT_INIT(id);
	SPI_PM_CHECK_RESTORE(id);

	if (tx_size && tx_data) {
		BK_LOG_ON_ERR(bk_spi_write_bytes(id, tx_data, tx_size));
	}

	if (rx_size && rx_data) {
		BK_LOG_ON_ERR(bk_spi_read_bytes(id, rx_data, rx_size));
	}

	return BK_OK;
}


bk_err_t bk_spi_clr_tx(spi_id_t id)
{
    spi_hal_disable_tx_fifo_int(&s_spi[id].hal);
    spi_hal_disable_tx(&s_spi[id].hal);
    return BK_OK;
}

bk_err_t bk_spi_clr_rx(spi_id_t id)
{
    spi_hal_disable_rx(&s_spi[id].hal);
    spi_hal_disable_tx_fifo_int(&s_spi[id].hal);
    spi_hal_disable_rx_fifo_int(&s_spi[id].hal);
    s_spi[id].rx_size = 0;
    s_spi[id].rx_offset = 0;
    s_spi[id].rx_buf = NULL;
    return BK_OK;
}


bk_err_t bk_spi_write_bytes_async(spi_id_t id, const void *data, uint32_t size)
{
	SPI_RETURN_ON_NOT_INIT();
	SPI_RETURN_ON_INVALID_ID(id);
	SPI_RETURN_ON_ID_NOT_INIT(id);
	BK_RETURN_ON_NULL(data);
	SPI_PM_CHECK_RESTORE(id);

	uint32_t int_level = spi_enter_critical();
	s_spi[id].tx_buf = (uint8_t *)data;
	s_spi[id].tx_size = size;
	s_spi[id].is_sw_tx_finished = false;
	s_spi[id].is_tx_blocked = false;
	s_spi[id].rx_size = size;
	s_spi[id].rx_offset = 0;
	spi_hal_clear_tx_fifo(&s_spi[id].hal);
	spi_hal_set_tx_trans_len(&s_spi[id].hal, size);
#if defined(CONFIG_SPI_SUPPORT_TX_FIFO_WR_READY)
	spi_hal_disable_tx_fifo_int(&s_spi[id].hal);
#else
	spi_hal_enable_tx_fifo_int(&s_spi[id].hal);
#endif
	spi_hal_enable_tx(&s_spi[id].hal);
	spi_exit_critical(int_level);

#if defined(CONFIG_SPI_SUPPORT_TX_FIFO_WR_READY)
	s_spi[id].is_sw_tx_finished = true;
	spi_id_write_bytes_common(id); /* to solve slave write */
#endif

	return BK_OK;
}

bk_err_t bk_spi_read_bytes_async(spi_id_t id, void *data, uint32_t size)
{
	SPI_RETURN_ON_NOT_INIT();
	SPI_RETURN_ON_INVALID_ID(id);
	SPI_RETURN_ON_ID_NOT_INIT(id);
	BK_RETURN_ON_NULL(data);

	uint32_t int_level = spi_enter_critical();
	s_spi[id].rx_size = size;
	s_spi[id].rx_buf = (uint8_t *)data;
	s_spi[id].rx_offset = 0;
	s_spi[id].is_rx_blocked = false;
	spi_hal_set_rx_trans_len(&s_spi[id].hal, size);
	spi_hal_clear_rx_fifo(&s_spi[id].hal);
	spi_hal_enable_rx_fifo_int(&s_spi[id].hal);
	spi_hal_enable_rx_finish_int(&s_spi[id].hal);
	spi_hal_enable_rx(&s_spi[id].hal);
#if !defined(CONFIG_SPI_SUPPORT_TX_FIFO_WR_READY)
	/* special for bk7251, bk7251 need enable tx fifo int, otherwize spi_isr will not triggered */
	spi_hal_enable_tx_fifo_int(&s_spi[id].hal);
#endif
	spi_exit_critical(int_level);

	return BK_OK;
}


#if defined(CONFIG_SPI_DMA)

static bk_err_t spi_duplex_tx_rx_enable(spi_id_t id)
{
	bk_dma_start(s_spi[id].spi_tx_dma_chan);
	bk_dma_start(s_spi[id].spi_rx_dma_chan);
	spi_hal_enable_tx(&s_spi[id].hal);
	spi_hal_enable_rx(&s_spi[id].hal);
	return BK_OK;
}

bk_err_t bk_spi_dma_duplex_init(spi_id_t id)
{
	spi_hal_duplex_config(&s_spi[id].hal);
	bk_dma_disable_src_addr_loop(s_spi[id].spi_tx_dma_chan);
	bk_dma_disable_dest_addr_loop(s_spi[id].spi_rx_dma_chan);
	return BK_OK;
}

bk_err_t bk_spi_dma_duplex_deinit(spi_id_t id)
{
	spi_hal_duplex_release(&s_spi[id].hal);
	bk_dma_enable_src_addr_loop(s_spi[id].spi_tx_dma_chan);
	bk_dma_enable_dest_addr_loop(s_spi[id].spi_rx_dma_chan);
	return BK_OK;
}

bk_err_t bk_spi_dma_duplex_xfer(spi_id_t id, const void *tx_data, uint32_t tx_size, void *rx_data, uint32_t rx_size)
{
	SPI_RETURN_ON_NOT_INIT();
	SPI_RETURN_ON_ID_NOT_INIT(id);
	SPI_RETURN_ON_INVALID_ID(id);

	if (tx_size != rx_size) {
		SPI_LOGW("tx and rx size must equal.\r\n");
		return BK_ERR_SPI_DUPLEX_SIZE_NOT_EQUAL;
	}

	uint32_t len = rx_size > 0 ? rx_size : tx_size;
	uint32_t offset = 0;
	uint32_t int_level = 0;
	s_current_spi_dma_wr_id = id;
	s_current_spi_dma_rd_id = id;

	while(len > 0) {
		int_level = spi_enter_critical();
		if(rx_data) {
			s_spi[id].is_rx_blocked = true;
			spi_hal_clear_rx_fifo(&s_spi[id].hal);
			spi_hal_set_rx_trans_len(&s_spi[id].hal, rx_size);
			bk_dma_set_dest_start_addr(s_spi[id].spi_rx_dma_chan,((uint32_t)rx_data + offset));
			bk_dma_set_transfer_len(s_spi[id].spi_rx_dma_chan,rx_size);
		}

		if(tx_data) {
			s_spi[id].is_tx_blocked = true;
			spi_hal_clear_tx_fifo(&s_spi[id].hal);
			spi_hal_set_tx_trans_len(&s_spi[id].hal, tx_size);
			bk_dma_set_src_start_addr(s_spi[id].spi_tx_dma_chan,((uint32_t)tx_data + offset));
			bk_dma_set_transfer_len(s_spi[id].spi_tx_dma_chan,tx_size);
		}

		if(tx_data) {
			bk_dma_start(s_spi[id].spi_tx_dma_chan);
		}
		if(rx_data) {
			bk_dma_start(s_spi[id].spi_rx_dma_chan);
		}

		if(tx_data) {
			spi_hal_enable_tx(&s_spi[id].hal);
		}
		if(rx_data) {
			spi_hal_enable_rx(&s_spi[id].hal);
		}
		spi_exit_critical(int_level);

		if(tx_data) {
			rtos_get_semaphore(&s_spi[id].tx_sema, BEKEN_NEVER_TIMEOUT);
		}
		if(rx_data) {
			rtos_get_semaphore(&s_spi[id].rx_sema, BEKEN_NEVER_TIMEOUT);
		}

		int_level = spi_enter_critical();
		extern uint32_t dma_wait_to_idle(dma_id_t id);
		if(tx_data) {
			spi_hal_disable_tx(&s_spi[id].hal);
			spi_hal_disable_tx_fifo_int(&s_spi[id].hal);
			dma_wait_to_idle(s_spi[id].spi_tx_dma_chan);
		}
		if(rx_data) {
			spi_hal_disable_rx(&s_spi[id].hal);
			spi_hal_disable_rx_fifo_int(&s_spi[id].hal);
			dma_wait_to_idle(s_spi[id].spi_rx_dma_chan);
		}
		spi_exit_critical(int_level);

		len = rx_size > 0 ? (len-rx_size) : (len-tx_size);
		offset += rx_size > 0 ? (rx_size) : (tx_size);
	}

	return BK_OK;
}

bk_err_t bk_spi_dma_write_bytes(spi_id_t id, const void *data, uint32_t size)
{
	BK_RETURN_ON_NULL(data);
	SPI_RETURN_ON_INVALID_ID(id);
	SPI_RETURN_ON_ID_NOT_INIT(id);

	int32_t left_len = size;
	uint32_t tx_len = 0;
	uint32_t buf_offset = 0;
	while(left_len > 0) {
		tx_len = (left_len < SPI_MAX_LENGTH)? left_len : SPI_MAX_LENGTH;
		SPI_LOGD("tx_len = 0x%x, left_len=0x%x\r\n", tx_len, left_len);

		uint32_t int_level = spi_enter_critical();
		s_spi[id].is_tx_blocked = true;
		s_current_spi_dma_wr_id = id;
		spi_hal_clear_tx_fifo(&s_spi[id].hal);
		//set spi trans_len as 0, to increase max trans_len from 4096(spi max length) to 65536(dma max length).
		spi_hal_set_tx_trans_len(&s_spi[id].hal, 0);
		spi_hal_enable_tx(&s_spi[id].hal);
		spi_exit_critical(int_level);
		bk_dma_write(s_spi[id].spi_tx_dma_chan, (uint8_t *)data + buf_offset, tx_len);
		rtos_get_semaphore(&s_spi[id].tx_sema, BEKEN_NEVER_TIMEOUT);
		int_level = spi_enter_critical();
		//wait spi last fifo data transfer finish
		spi_hal_enable_tx_fifo_int(&s_spi[id].hal);
		for (int i = 0; i <= 500; i++) {
			delay_us(1);
			SPI_LOGD("index = %d, id=%d, tx_fifo_int_status = %d\n", i, id, spi_hal_is_tx_fifo_int_triggered(&s_spi[id].hal));
			if(spi_hal_is_tx_fifo_int_triggered(&s_spi[id].hal)) {
				delay_us(1);
				break;
			}
			if(i == 500)
				SPI_LOGE("wait tx fifo empty timeout.\n");
		}
		spi_hal_disable_tx_fifo_int(&s_spi[id].hal);
		spi_hal_clear_tx_fifo_int_status(&s_spi[id].hal);
		spi_hal_disable_tx(&s_spi[id].hal);
		bk_dma_stop(s_spi[id].spi_tx_dma_chan);
		spi_exit_critical(int_level);
		left_len -= tx_len;
		buf_offset += tx_len;
	}

	return BK_OK;
}

bk_err_t bk_spi_dma_read_bytes(spi_id_t id, void *data, uint32_t size)
{
	SPI_RETURN_ON_INVALID_ID(id);
	SPI_RETURN_ON_ID_NOT_INIT(id);
	BK_RETURN_ON_NULL(data);

	int32_t left_len = size;
	uint32_t rx_len = 0;
	uint32_t buf_offset = 0;

	while(left_len > 0) {
		rx_len = (left_len < SPI_MAX_LENGTH)? left_len : SPI_MAX_LENGTH;
		uint32_t int_level = spi_enter_critical();
		s_current_spi_dma_rd_id = id;
		s_spi[id].is_rx_blocked = true;

		//set spi trans_len as 0, to increase max trans_len from 4096(spi max length) to 65536(dma max length).
		spi_hal_set_rx_trans_len(&s_spi[id].hal, 0);
		spi_hal_clear_rx_fifo(&s_spi[id].hal);
		spi_hal_disable_rx_fifo_int(&s_spi[id].hal);
		spi_hal_disable_rx_overflow_int(&s_spi[id].hal);
		spi_hal_enable_rx(&s_spi[id].hal);
		spi_exit_critical(int_level);
		bk_dma_read(s_spi[id].spi_rx_dma_chan, (uint8_t *)data + buf_offset, rx_len);

		rtos_get_semaphore(&s_spi[id].rx_sema, BEKEN_NEVER_TIMEOUT);

		int_level = spi_enter_critical();
		spi_hal_disable_rx(&s_spi[id].hal);
		bk_dma_stop(s_spi[id].spi_rx_dma_chan);
		spi_exit_critical(int_level);

		left_len -= rx_len;
		buf_offset += rx_len;
	}

	return BK_OK;
}

bk_err_t bk_spi_dma_transmit(spi_id_t id, const void *tx_data, uint32_t tx_size, void *rx_data, uint32_t rx_size)
{
	SPI_RETURN_ON_INVALID_ID(id);
	SPI_RETURN_ON_ID_NOT_INIT(id);

	if (tx_size && tx_data) {
		BK_LOG_ON_ERR(bk_spi_dma_write_bytes(id, tx_data, tx_size));
	}

	if (rx_size && rx_data) {
		BK_LOG_ON_ERR(bk_spi_dma_read_bytes(id, rx_data, rx_size));
	}

	return BK_OK;
}

#endif

static void spi_isr_common(spi_id_t id)
{
	spi_hal_t *hal = &s_spi[id].hal;
	uint16_t rd_data = 0;
	uint32_t int_status = 0;
	uint32_t rd_offset = s_spi[id].rx_offset;
	uint16_t *data = (uint16_t *)s_spi[id].rx_buf;
	SPI_STATIS_DEC();
	SPI_STATIS_GET(spi_statis, id);

	int_status = spi_hal_get_interrupt_status(hal);
	spi_hal_clear_interrupt_status(hal, int_status);

	SPI_LOGD("int_status:%x\r\n", int_status);

	if (spi_hal_is_rx_fifo_int_triggered_with_status(hal, int_status)) {
		SPI_STATIS_INC(spi_statis->rx_fifo_isr_cnt);
		SPI_LOGD("rx fifo int triggered\r\n");
		spi_id_read_bytes_common(id);
		if (s_spi_rx_isr[id].callback) {
			s_spi_rx_isr[id].callback(id, s_spi_rx_isr[id].param);
		}
	}

	if (spi_hal_is_rx_finish_int_triggered(hal, int_status)) {
		SPI_STATIS_INC(spi_statis->rx_finish_isr_cnt);
		SPI_LOGD("rx fifo finish int triggered\r\n");
		if (s_spi[id].rx_size && s_spi[id].rx_buf) {
			while (spi_hal_read_byte(hal, &rd_data) == BK_OK) {
				if (rd_offset < s_spi[id].rx_size) {
					if(s_spi[id].hal.hw->ctrl.bit_width == SPI_BIT_WIDTH_8BITS){
						s_spi[id].rx_buf[rd_offset++] = (uint8_t)(rd_data&0xFF);
					} else {
						data[rd_offset++] = rd_data;
					}
				}
			}
			s_spi[id].rx_offset = rd_offset;
		}
		bk_spi_clr_rx(id);
		if (s_spi_rx_finish_isr[id].callback){
			s_spi_rx_finish_isr[id].callback(s_current_spi_dma_rd_id,s_spi_rx_finish_isr[id].param);
		}
		if (s_spi[id].is_rx_blocked && (0 != s_spi[id].rx_offset)) {
			rtos_set_semaphore(&s_spi[id].rx_sema);
			s_spi[id].is_rx_blocked = false;
		}
	}

	if (spi_hal_is_rx_overflow_int_triggered(hal, int_status)) {
		SPI_STATIS_INC(spi_statis->rx_overflow_isr_cnt);
		SPI_LOGW("rx overflow int triggered\r\n");
	}

	if (spi_hal_is_tx_fifo_int_triggered_with_status(hal, int_status)) {
		SPI_STATIS_INC(spi_statis->tx_fifo_isr_cnt);
		SPI_LOGD("tx fifo int triggered\r\n");
#if !defined(CONFIG_SPI_SUPPORT_TX_FIFO_WR_READY)
		if ((!s_spi[id].is_sw_tx_finished) &&
			s_spi[id].tx_size &&
			s_spi[id].tx_buf) {
			spi_id_write_bytes_common(id);
			s_spi[id].is_sw_tx_finished = true;
		}
		for (int i = 0; (i < s_spi[id].rx_size) && s_spi[id].rx_buf; i++) {
			/* bk7251 need spi_hal_write_byte when read byte,
			 * bk7251 master read data will not work without this operation */
			if (spi_hal_is_master(hal)) {
				spi_hal_write_byte(hal, 0xff);
			}
			if (spi_hal_read_byte(hal, &rd_data) == BK_OK) {
				SPI_LOGD("tx fifo int read byte\r\n");
				if (rd_offset < s_spi[id].rx_size) {
					s_spi[id].rx_buf[rd_offset++] = rd_data;
				}
			} else {
				break;
			}
		}
		s_spi[id].rx_offset = rd_offset;
		if (s_spi[id].is_sw_tx_finished) {
			spi_hal_disable_tx_fifo_int(hal);
		}
#endif
	}

	if (spi_hal_is_tx_finish_int_triggered(hal, int_status) &&
		s_spi[id].is_sw_tx_finished) {
		SPI_STATIS_INC(spi_statis->tx_finish_isr_cnt);
		SPI_LOGD("tx finish int triggered\r\n");
		if (spi_hal_is_master(hal)) {
			if (s_spi[id].is_tx_blocked) {
				rtos_set_semaphore(&s_spi[id].tx_sema);
				s_spi[id].is_tx_blocked = false;
			}
			if (s_spi_tx_finish_isr[id].callback) {
				s_spi_tx_finish_isr[id].callback(id, s_spi_tx_finish_isr[id].param);
			}
			bk_spi_clr_tx(id);
		} else {
			SPI_LOGW("tx finish int triggered, but current mode is spi_slave\r\n");
		}
	}

	if (spi_hal_is_tx_underflow_int_triggered(hal, int_status)) {
		SPI_STATIS_INC(spi_statis->tx_underflow_isr_cnt);
		SPI_LOGW("tx underflow int triggered\r\n");
	}

	if (spi_hal_is_slave_release_int_triggered(hal, int_status)) {
		SPI_STATIS_INC(spi_statis->slave_release_isr_cnt);
		SPI_LOGD("slave cs up int triggered\r\n");
		if (spi_hal_is_slave(hal)) {
			if (s_spi[id].is_tx_blocked) {
				rtos_set_semaphore(&s_spi[id].tx_sema);
				s_spi[id].is_tx_blocked = false;
			}
			if (s_spi_tx_finish_isr[id].callback) {
				s_spi_tx_finish_isr[id].callback(id, s_spi_tx_finish_isr[id].param);
			}
		}
		if (s_spi[id].is_rx_blocked) {
			rtos_set_semaphore(&s_spi[id].rx_sema);
			s_spi[id].is_rx_blocked = false;
		}
	}
}

static void spi_isr(void)
{
	SPI_STATIS_DEC();
	SPI_STATIS_GET(spi_statis, SPI_ID_0);
	SPI_STATIS_INC(spi_statis->spi_isr_cnt);
	spi_isr_common(SPI_ID_0);
}

#if (SOC_SPI_UNIT_NUM > 1)

static void spi2_isr(void)
{
	SPI_STATIS_DEC();
	SPI_STATIS_GET(spi_statis, SPI_ID_1);
	SPI_STATIS_INC(spi_statis->spi_isr_cnt);
	spi_isr_common(SPI_ID_1);
}

#endif

#if (SOC_SPI_UNIT_NUM > 2)

static void spi3_isr(void)
{
	SPI_STATIS_DEC();
	SPI_STATIS_GET(spi_statis, SPI_ID_2);
	SPI_STATIS_INC(spi_statis->spi_isr_cnt);
	spi_isr_common(SPI_ID_2);
}

#endif

