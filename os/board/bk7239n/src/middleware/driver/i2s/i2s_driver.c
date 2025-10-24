// Copyright 2020-2021 Beken
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
#include "i2s_hal.h"
#include "i2s_driver.h"
#include "sys_driver.h"
#include "clock_driver.h"
#include <soc/soc.h>
#include <driver/i2s_types.h>
#include <os/os.h>
#include <os/mem.h>
#include <driver/int.h>
#include "gpio_driver.h"
#include <driver/gpio.h>
#include <modules/pm.h>
#include <driver/audio_ring_buff.h>


#define I2S_RETURN_ON_NOT_INIT() do {\
		if (!s_i2s_driver_is_init) {\
			return BK_ERR_I2S_NOT_INIT;\
		}\
	} while(0)

#define I2S_RETURN_ON_INVALID_ISR_ID(isr_id) do {\
		if ((isr_id) >= SOC_I2S_ISR_NUM) {\
			return BK_ERR_I2S_ISR_ID;\
		}\
	} while(0)


static bool s_i2s_driver_is_init = false;
static i2s_callback_t s_i2s_isr[SOC_I2S_ISR_NUM] = {NULL};
static i2s_drv_info_t *i2s_drv_info = NULL;

static void i2s_isr(void);
extern void delay(int num);//TODO fix me

bk_err_t bk_i2s_set_samp_rate(i2s_samp_rate_t samp_rate);
bk_err_t bk_i2s_chl_deinit(i2s_channel_id_t chl, i2s_txrx_type_t type);

#if 0

#else
static void i2s_init_gpio(i2s_gpio_group_id_t id)
{
	switch(id)
	{
		case I2S_GPIO_GROUP_0:
			gpio_dev_unmap(GPIO_6);
			gpio_dev_map(GPIO_6, GPIO_DEV_I2S1_CLK);
			gpio_dev_unmap(GPIO_7);
			gpio_dev_map(GPIO_7, GPIO_DEV_I2S1_SYNC);
			gpio_dev_unmap(GPIO_8);
			gpio_dev_map(GPIO_8, GPIO_DEV_I2S1_DIN);
			gpio_dev_unmap(GPIO_9);
			gpio_dev_map(GPIO_9, GPIO_DEV_I2S1_DOUT);
			gpio_dev_unmap(GPIO_28);
			gpio_dev_map(GPIO_28, GPIO_DEV_I2S1_MCLK);
			bk_gpio_disable_output(GPIO_28);
			break;

		case I2S_GPIO_GROUP_1:
			gpio_dev_unmap(GPIO_40);
			gpio_dev_map(GPIO_40, GPIO_DEV_I2S1_CLK);
			bk_gpio_disable_output(GPIO_40);
			gpio_dev_unmap(GPIO_41);
			gpio_dev_map(GPIO_41, GPIO_DEV_I2S1_SYNC);
			bk_gpio_disable_output(GPIO_41);
			gpio_dev_unmap(GPIO_42);
			gpio_dev_map(GPIO_42, GPIO_DEV_I2S1_DIN);
			bk_gpio_disable_output(GPIO_42);
			gpio_dev_unmap(GPIO_43);
			gpio_dev_map(GPIO_43, GPIO_DEV_I2S1_DOUT);
			bk_gpio_disable_output(GPIO_43);
			gpio_dev_unmap(GPIO_28);
			gpio_dev_map(GPIO_28, GPIO_DEV_I2S1_MCLK);
			bk_gpio_disable_output(GPIO_28);
			break;

		default:
			break;
	}
}
#endif

bk_err_t bk_i2s_driver_init(void)
{
	if (s_i2s_driver_is_init)
		return BK_OK;

	//power on
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_AUDP_I2S, PM_POWER_MODULE_STATE_ON);
	//sys_drv_aud_power_en(0);    //temp used

	//select 26M XTAL clock and enable i2s clock
	sys_drv_i2s_select_clock(1);
	//sys_drv_i2s_clock_en(1);
	bk_pm_clock_ctrl(PM_CLK_ID_I2S_1, CLK_PWR_CTRL_PWR_UP);

	//i2s_disckg always on
	sys_drv_i2s_disckg_set(1);

	//set apll clock config
	sys_drv_apll_en(1);
	sys_drv_cb_manu_val_set(0x14);
	sys_drv_ana_reg11_vsel_set(7);

	//enable i2s interrupt
	sys_drv_i2s_int_en(1);

	os_memset(&s_i2s_isr, 0, sizeof(s_i2s_isr));

	//register i2s isr
	i2s_int_config_t int_config_table = {INT_SRC_I2S, i2s_isr};
	bk_int_isr_register(int_config_table.int_src, int_config_table.isr, NULL);
	s_i2s_driver_is_init = true;

	return BK_OK;
}

bk_err_t bk_i2s_driver_deinit(void)
{
	//power down
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_AUDP_I2S, PM_POWER_MODULE_STATE_OFF);
	//sys_drv_i2s_clock_en(0);
	bk_pm_clock_ctrl(PM_CLK_ID_I2S_1, CLK_PWR_CTRL_PWR_DOWN);

	//i2s_disckg not always on
	sys_drv_i2s_disckg_set(0);
	//disable i2s interrupt
	sys_drv_i2s_int_en(0);

	//set apll clock config
	sys_drv_apll_en(0);
	sys_drv_cb_manu_val_set(0x10);
	sys_drv_ana_reg11_vsel_set(5);

	i2s_int_config_t int_config_table = {INT_SRC_I2S, i2s_isr};
	bk_int_isr_unregister(int_config_table.int_src);
	s_i2s_driver_is_init = false;

	return BK_OK;
}

bk_err_t bk_i2s_init(i2s_gpio_group_id_t id, const i2s_config_t *config)
{
	I2S_RETURN_ON_NOT_INIT();
	if (!config)
		return BK_ERR_I2S_PARAM;
#if 0
	/*
	 * GPIO info is setted in GPIO_DEFAULT_DEV_CONFIG and
	 * inited in bk_gpio_driver_init->gpio_hal_default_map_init.
	 * If needs to re-config GPIO, can deal it here.
	 */
#else
	i2s_init_gpio(id);
#endif
	if (i2s_drv_info == NULL) {
		i2s_drv_info = (i2s_drv_info_t *)os_malloc(sizeof(i2s_drv_info_t));
		i2s_drv_info->chl1_cfg = NULL;
		i2s_drv_info->chl2_cfg = NULL;
		i2s_drv_info->chl3_cfg = NULL;
	}

	i2s_drv_info->config.i2s_en = I2S_DISABLE;
	i2s_drv_info->config.role = config->role;
	i2s_drv_info->config.work_mode = config->work_mode;
	i2s_drv_info->config.lrck_invert = config->lrck_invert;
	i2s_drv_info->config.sck_invert = config->sck_invert;
	i2s_drv_info->config.lsb_first_en = config->lsb_first_en;
	i2s_drv_info->config.sync_length = config->sync_length;
	i2s_drv_info->config.data_length = config->data_length;
	i2s_drv_info->config.pcm_dlength = config->pcm_dlength;
	i2s_drv_info->config.sample_ratio = 0;
	i2s_drv_info->config.sample_ratio_h2b = 0;
	i2s_drv_info->config.sck_ratio = 0;
	i2s_drv_info->config.sck_ratio_h4b = 0;
	i2s_drv_info->config.store_mode = config->store_mode;
	i2s_drv_info->config.pcm_chl_num = config->pcm_chl_num;
	i2s_drv_info->config.samp_rate = config->samp_rate;

	/* set parallel_en according to work mode */
	switch (i2s_drv_info->config.work_mode) {
		case I2S_WORK_MODE_I2S:
		case I2S_WORK_MODE_LEFTJUST:
		case I2S_WORK_MODE_RIGHTJUST:
			i2s_drv_info->config.parallel_en = I2S_PARALLEL_ENABLE;
			break;

		case I2S_WORK_MODE_SHORTFAMSYNC:
		case I2S_WORK_MODE_LONGFAMSYNC:
			I2S_LOGE("pcm_chl_num: %d, data_length: %d \n", i2s_drv_info->config.pcm_chl_num, i2s_drv_info->config.data_length);
			if ((i2s_drv_info->config.pcm_chl_num * i2s_drv_info->config.data_length) > 32) {
				i2s_drv_info->config.parallel_en = I2S_PARALLEL_DISABLE;
				I2S_LOGE("disenable parallel \n");
			} else {
				i2s_drv_info->config.parallel_en = I2S_PARALLEL_ENABLE;
			}
			break;

		case I2S_WORK_MODE_NORMAL2BD:
		case I2S_WORK_MODE_DELAY2BD:
			i2s_drv_info->config.parallel_en = I2S_PARALLEL_ENABLE;
			break;

		case I2S_WORK_MODE_RSVD:
			break;

		default:
			break;
	}

	i2s_hal_config(&(i2s_drv_info->config));

	if (bk_i2s_set_samp_rate(i2s_drv_info->config.samp_rate) != BK_OK) {
		I2S_LOGE("config sample rate fail \n");
		return BK_FAIL;
	}

	i2s_drv_info->i2s_enable_state = false;

	return BK_OK;
}

bk_err_t bk_i2s_deinit(void)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_deconfig();

	if (i2s_drv_info->chl1_cfg) {
		bk_i2s_chl_deinit(I2S_CHANNEL_1, I2S_TXRX_TYPE_TX);
		bk_i2s_chl_deinit(I2S_CHANNEL_1, I2S_TXRX_TYPE_RX);
	}

	if (i2s_drv_info->chl2_cfg) {
		bk_i2s_chl_deinit(I2S_CHANNEL_2, I2S_TXRX_TYPE_TX);
		bk_i2s_chl_deinit(I2S_CHANNEL_2, I2S_TXRX_TYPE_RX);
	}

	if (i2s_drv_info->chl3_cfg) {
		bk_i2s_chl_deinit(I2S_CHANNEL_3, I2S_TXRX_TYPE_TX);
		bk_i2s_chl_deinit(I2S_CHANNEL_3, I2S_TXRX_TYPE_RX);
	}

	if (i2s_drv_info) {
		os_free(i2s_drv_info);
		i2s_drv_info = NULL;
	}

	return BK_OK;
}

bk_err_t bk_i2s_get_read_ready(uint32_t *read_flag)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_read_ready_get(read_flag);
	return BK_OK;
}

bk_err_t bk_i2s_get_write_ready(uint32_t *write_flag)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_write_ready_get(write_flag);
	return BK_OK;
}

bk_err_t bk_i2s_enable(i2s_en_t en_value)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_en_set(en_value);
	return BK_OK;
}

bk_err_t bk_i2s_int_enable(i2s_isr_id_t int_id, uint32_t value)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_int_set(int_id, value);
	return BK_OK;
}

bk_err_t bk_i2s_set_role(i2s_role_t role)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_role_set(role);
	return BK_OK;
}

bk_err_t bk_i2s_set_work_mode(i2s_work_mode_t work_mode)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_work_mode_set(work_mode);
	return BK_OK;
}

bk_err_t bk_i2s_set_lrck_invert(i2s_lrck_invert_en_t lrckrp)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_lrck_invert_set(lrckrp);
	return BK_OK;
}

bk_err_t bk_i2s_set_sck_invert(i2s_sck_invert_en_t sck_invert)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_sck_invert_set(sck_invert);
	return BK_OK;
}

bk_err_t bk_i2s_set_lsb_first(i2s_lsb_first_en_t lsb_first)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_lsb_first_set(lsb_first);
	return BK_OK;
}

bk_err_t bk_i2s_set_sync_len(uint32_t sync_len)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_sync_len_set(sync_len);
	return BK_OK;
}

bk_err_t bk_i2s_set_data_len(uint32_t data_len)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_data_len_set(data_len);
	return BK_OK;
}

bk_err_t bk_i2s_set_pcm_dlen(uint32_t pcm_dlen)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_pcm_dlen_set(pcm_dlen);
	return BK_OK;
}

bk_err_t bk_i2s_set_store_mode(i2s_lrcom_store_mode_t store_mode)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_store_mode_set(store_mode);
	return BK_OK;
}

bk_err_t bk_i2s_clear_rxfifo(void)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_rxfifo_clear();
	return BK_OK;
}

bk_err_t bk_i2s_clear_txfifo(void)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_txfifo_clear();
	return BK_OK;
}

bk_err_t bk_i2s_clear_txudf_int(i2s_channel_id_t channel_id)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_txudf_int_clear(channel_id);
	return BK_OK;
}

bk_err_t bk_i2s_clear_rxovf_int(i2s_channel_id_t channel_id)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_rxovf_int_clear(channel_id);
	return BK_OK;
}

bk_err_t bk_i2s_set_txint_level(i2s_txint_level_t txint_level)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_txint_level_set(txint_level);
	return BK_OK;
}

bk_err_t bk_i2s_set_rxint_level(i2s_rxint_level_t rxint_level)
{
	I2S_RETURN_ON_NOT_INIT();

	i2s_hal_rxint_level_set(rxint_level);
	return BK_OK;
}

bk_err_t bk_i2s_write_data(uint32_t channel_id, uint32_t *data_buf, uint32_t data_len)
{
	uint32_t i = 0;
	I2S_RETURN_ON_NOT_INIT();

	for (i=0; i<data_len; i++)
		i2s_hal_data_write(channel_id, data_buf[i]);
	return BK_OK;
}

bk_err_t bk_i2s_read_data(uint32_t *data_buf, uint32_t data_len)
{
	uint32_t i = 0;
	I2S_RETURN_ON_NOT_INIT();

	for (i=0; i<data_len; i++)
		i2s_hal_data_read(&data_buf[i]);
	return BK_OK;
}

bk_err_t bk_i2s_get_data_addr(i2s_channel_id_t chl_id, uint32_t *i2s_data_addr)
{
	I2S_RETURN_ON_NOT_INIT();
	switch (chl_id) {
		case I2S_CHANNEL_1:
			*i2s_data_addr = I2S_REG0X3_ADDR;
			break;

		case I2S_CHANNEL_2:
			*i2s_data_addr = I2S_REG0X6_ADDR;
			break;

		case I2S_CHANNEL_3:
			*i2s_data_addr = I2S_REG0X7_ADDR;
			break;

		default:
			return BK_FAIL;
	}

	return BK_OK;
}

bk_err_t bk_i2s_set_samp_rate(i2s_samp_rate_t samp_rate)
{
	//SOC_I2S_APLL_RATE
	uint32_t sample_rate = 0;
	uint32_t smp_ratio = 0;
	uint32_t bit_ratio = 0;
	uint32_t bit_ratio_b = 0;
	uint32_t apll_clk = 0;
	uint32_t data_len = i2s_drv_info->config.data_length;
	uint32_t pcm_chl_num = (uint32_t)i2s_drv_info->config.pcm_chl_num;

	I2S_RETURN_ON_NOT_INIT();

	/* get sample rate number */
	switch (samp_rate) {
		case I2S_SAMP_RATE_8000:
			sample_rate = 8000;
			break;
		case I2S_SAMP_RATE_12000:
			sample_rate = 12000;
			break;
		case I2S_SAMP_RATE_16000:
			sample_rate = 16000;
			break;
		case I2S_SAMP_RATE_24000:
			sample_rate = 24000;
			break;
		case I2S_SAMP_RATE_32000:
			sample_rate = 32000;
			break;
		case I2S_SAMP_RATE_48000:
			sample_rate = 48000;
			break;
		case I2S_SAMP_RATE_96000:
			sample_rate = 96000;
			break;
		case I2S_SAMP_RATE_8018:
			sample_rate = 80182;		//sample_rate * 10
			break;
		case I2S_SAMP_RATE_11025:
			sample_rate = 11025;
			break;
		case I2S_SAMP_RATE_22050:
			sample_rate = 22050;
			break;
		case I2S_SAMP_RATE_44100:
			sample_rate = 44100;
			break;
		case I2S_SAMP_RATE_88200:
			sample_rate = 88200;
			break;
		default:
			break;
	}

	/* get apll clock according to sample rate */
	if (samp_rate == I2S_SAMP_RATE_8000 ||
		samp_rate == I2S_SAMP_RATE_12000 ||
		samp_rate == I2S_SAMP_RATE_16000 ||
		samp_rate == I2S_SAMP_RATE_24000 ||
		samp_rate == I2S_SAMP_RATE_32000 ||
		samp_rate == I2S_SAMP_RATE_48000 ||
		samp_rate == I2S_SAMP_RATE_96000)
	{
		apll_clk = 98304000;
	}
	else if (samp_rate == I2S_SAMP_RATE_8018 ||
		samp_rate == I2S_SAMP_RATE_11025 ||
		samp_rate == I2S_SAMP_RATE_22050 ||
		samp_rate == I2S_SAMP_RATE_44100 ||
		samp_rate == I2S_SAMP_RATE_88200)
	{
		apll_clk = 90316800;
	} else {
		I2S_LOGE("cannot support sample rate \r\n");
		return BK_ERR_I2S_PARAM;
	}

	/* get samp_ratio: Fs = Fsck / 2*(SMPRATIO+1) */
	switch (i2s_drv_info->config.work_mode) {
		case I2S_WORK_MODE_I2S:
		case I2S_WORK_MODE_LEFTJUST:
		case I2S_WORK_MODE_RIGHTJUST:
			smp_ratio = data_len - 1;
			break;

		case I2S_WORK_MODE_SHORTFAMSYNC:
		case I2S_WORK_MODE_LONGFAMSYNC:
			smp_ratio = data_len * pcm_chl_num - 1;
			break;

		case I2S_WORK_MODE_NORMAL2BD:
		case I2S_WORK_MODE_DELAY2BD:
			smp_ratio = data_len - 1;
			break;

		case I2S_WORK_MODE_RSVD:
			break;

		default:
			break;
	}

	/* get bit_ratio: Fsck = Fsys_clk / 2*(BITRATIO) */
	if (sample_rate == 80182) {
		if (i2s_drv_info->config.work_mode == I2S_WORK_MODE_SHORTFAMSYNC || i2s_drv_info->config.work_mode == I2S_WORK_MODE_LONGFAMSYNC) {
			bit_ratio = (apll_clk*10)/((sample_rate*(smp_ratio+1))*2);
			bit_ratio_b = (apll_clk*10)%((sample_rate*(smp_ratio+1))*2);
		} else {
			bit_ratio = (apll_clk*10)/((sample_rate*2*(smp_ratio+1))*2);
			bit_ratio_b = (apll_clk*10)%((sample_rate*2*(smp_ratio+1))*2);
		}
	} else {
		I2S_LOGE("apll_clk: %d, sample_rate: %d, smp_ratio: %d \r\n", apll_clk, sample_rate, smp_ratio);
		if (i2s_drv_info->config.work_mode == I2S_WORK_MODE_SHORTFAMSYNC || i2s_drv_info->config.work_mode == I2S_WORK_MODE_LONGFAMSYNC) {
			bit_ratio = apll_clk/((sample_rate*(smp_ratio+1))*2);
			bit_ratio_b = apll_clk%((sample_rate*(smp_ratio+1))*2);
		} else {
			bit_ratio = apll_clk/((sample_rate*2*(smp_ratio+1))*2);
			bit_ratio_b = apll_clk%((sample_rate*2*(smp_ratio+1))*2);
		}
	}
	I2S_LOGE("bit_ratio: %d, bit_ratio_b: %d \r\n", bit_ratio, bit_ratio_b);
	if (bit_ratio_b != 0) {
		I2S_LOGE("cannot support sample rate \r\n");
		return BK_ERR_I2S_PARAM;
	}

	if (apll_clk == 98304000)
		sys_drv_apll_cal_val_set(0xF1FAA45);
	else if (apll_clk == 90316800)
		sys_drv_apll_cal_val_set(0xDE517A9);
	else
		sys_drv_apll_cal_val_set(0xF1FAA45);
	sys_drv_i2s0_ckdiv_set(3);
	sys_drv_apll_spi_trigger_set(1);
	delay(10);
	sys_drv_apll_spi_trigger_set(0);

	if (smp_ratio > 0x1F) {
		i2s_hal_sample_ratio_set(smp_ratio & 0x1F);
		i2s_hal_sample_ratio_h2b_set((smp_ratio >> 5) & 0x3);
	} else {
		i2s_hal_sample_ratio_set(smp_ratio);
	}
	if (bit_ratio > 0xFF) {
		i2s_hal_sck_ratio_set(bit_ratio & 0xFF);
		i2s_hal_sck_ratio_h4b_set((bit_ratio >> 8) & 0xF);
	} else {
		i2s_hal_sck_ratio_set(bit_ratio);
	}

	return BK_OK;
}

/* register i2s interrupt */
bk_err_t bk_i2s_register_i2s_isr(i2s_isr_id_t isr_id, i2s_isr_t isr, void *param)
{
	I2S_RETURN_ON_NOT_INIT();
	I2S_RETURN_ON_INVALID_ISR_ID(isr_id);
	uint32_t int_level = rtos_disable_int();
	s_i2s_isr[isr_id].callback = isr;
	s_i2s_isr[isr_id].param = param;
	rtos_enable_int(int_level);

	return BK_OK;
}

void i2s_isr(void)
{
	i2s_int_status_t i2s_status = {0};
	i2s_status.channel_id = I2S_CHANNEL_1;
	i2s_hal_int_status_get(&i2s_status);

	if (i2s_status.tx_udf) {
		i2s_hal_txudf_int_clear(I2S_CHANNEL_1);
		if (s_i2s_isr[I2S_ISR_CHL1_TXUDF].callback) {
			s_i2s_isr[I2S_ISR_CHL1_TXUDF].callback(s_i2s_isr[I2S_ISR_CHL1_TXUDF].param);
		}
	}

	if (i2s_status.rx_ovf) {
		i2s_hal_rxovf_int_clear(I2S_CHANNEL_1);
		if (s_i2s_isr[I2S_ISR_CHL1_RXOVF].callback) {
			s_i2s_isr[I2S_ISR_CHL1_RXOVF].callback(s_i2s_isr[I2S_ISR_CHL1_RXOVF].param);
		}
	}

	if (i2s_status.tx_int) {
		if (s_i2s_isr[I2S_ISR_CHL1_TXINT].callback) {
			s_i2s_isr[I2S_ISR_CHL1_TXINT].callback(s_i2s_isr[I2S_ISR_CHL1_TXINT].param);
		}
	}

	if (i2s_status.rx_int) {
		if (s_i2s_isr[I2S_ISR_CHL1_RXINT].callback) {
			s_i2s_isr[I2S_ISR_CHL1_RXINT].callback(s_i2s_isr[I2S_ISR_CHL1_RXINT].param);
		}
	}

	os_memset(&i2s_status, 0, sizeof(i2s_status));
	i2s_status.channel_id = I2S_CHANNEL_2;
	i2s_hal_int_status_get(&i2s_status);
	if (i2s_status.tx_udf) {
		i2s_hal_txudf_int_clear(I2S_CHANNEL_2);
		if (s_i2s_isr[I2S_ISR_CHL2_TXUDF].callback) {
			s_i2s_isr[I2S_ISR_CHL2_TXUDF].callback(s_i2s_isr[I2S_ISR_CHL2_TXUDF].param);
		}
	}

	if (i2s_status.rx_ovf) {
		i2s_hal_rxovf_int_clear(I2S_CHANNEL_2);
		if (s_i2s_isr[I2S_ISR_CHL2_RXOVF].callback) {
			s_i2s_isr[I2S_ISR_CHL2_RXOVF].callback(s_i2s_isr[I2S_ISR_CHL2_RXOVF].param);
		}
	}

	if (i2s_status.tx_int) {
		if (s_i2s_isr[I2S_ISR_CHL2_TXINT].callback) {
			s_i2s_isr[I2S_ISR_CHL2_TXINT].callback(s_i2s_isr[I2S_ISR_CHL2_TXINT].param);
		}
	}

	if (i2s_status.rx_int) {
		if (s_i2s_isr[I2S_ISR_CHL2_RXINT].callback) {
			s_i2s_isr[I2S_ISR_CHL2_RXINT].callback(s_i2s_isr[I2S_ISR_CHL2_RXINT].param);
		}
	}

	os_memset(&i2s_status, 0, sizeof(i2s_status));
	i2s_status.channel_id = I2S_CHANNEL_3;
	i2s_hal_int_status_get(&i2s_status);
	if (i2s_status.tx_udf) {
		i2s_hal_txudf_int_clear(I2S_CHANNEL_3);
		if (s_i2s_isr[I2S_ISR_CHL3_TXUDF].callback) {
			s_i2s_isr[I2S_ISR_CHL3_TXUDF].callback(s_i2s_isr[I2S_ISR_CHL3_TXUDF].param);
		}
	}

	if (i2s_status.rx_ovf) {
		i2s_hal_rxovf_int_clear(I2S_CHANNEL_3);
		if (s_i2s_isr[I2S_ISR_CHL3_RXOVF].callback) {
			s_i2s_isr[I2S_ISR_CHL3_RXOVF].callback(s_i2s_isr[I2S_ISR_CHL3_RXOVF].param);
		}
	}

	if (i2s_status.tx_int) {
		if (s_i2s_isr[I2S_ISR_CHL3_TXINT].callback) {
			s_i2s_isr[I2S_ISR_CHL3_TXINT].callback(s_i2s_isr[I2S_ISR_CHL3_TXINT].param);
		}
	}

	if (i2s_status.rx_int) {
		if (s_i2s_isr[I2S_ISR_CHL3_RXINT].callback) {
			s_i2s_isr[I2S_ISR_CHL3_RXINT].callback(s_i2s_isr[I2S_ISR_CHL3_RXINT].param);
		}
	}

#if 0
	os_memset(&i2s_status, 0, sizeof(i2s_status));
	i2s_status.channel_id = I2S_CHANNEL_4;
	i2s_hal_int_status_get(&i2s_status);
	if (i2s_status.tx_udf) {
		i2s_hal_txudf_int_clear(I2S_CHANNEL_4);
		if (s_i2s_isr[I2S_ISR_CHL4_TXUDF].callback) {
			s_i2s_isr[I2S_ISR_CHL4_TXUDF].callback(s_i2s_isr[I2S_ISR_CHL4_TXUDF].param);
		}
	}

	if (i2s_status.rx_ovf) {
		i2s_hal_rxovf_int_clear(I2S_CHANNEL_4);
		if (s_i2s_isr[I2S_ISR_CHL4_RXOVF].callback) {
			s_i2s_isr[I2S_ISR_CHL4_RXOVF].callback(s_i2s_isr[I2S_ISR_CHL4_RXOVF].param);
		}
	}

	if (i2s_status.tx_int) {
		if (s_i2s_isr[I2S_ISR_CHL4_TXINT].callback) {
			s_i2s_isr[I2S_ISR_CHL4_TXINT].callback(s_i2s_isr[I2S_ISR_CHL4_TXINT].param);
		}
	}

	if (i2s_status.rx_int) {
		if (s_i2s_isr[I2S_ISR_CHL4_RXINT].callback) {
			s_i2s_isr[I2S_ISR_CHL4_RXINT].callback(s_i2s_isr[I2S_ISR_CHL4_RXINT].param);
		}
	}
#endif
}

/* i2s channel 1 tx dma isr handle */
static void i2s_chl1_tx_dma_finish_isr(void)
{
	//uint32_t size = 0;
	if (i2s_drv_info->chl1_cfg->tx_cfg->data_handle_cb) {
		i2s_drv_info->chl1_cfg->tx_cfg->data_handle_cb((i2s_drv_info->chl1_cfg->tx_cfg->buff_size)/2);
	}
}

/* i2s channel 1 tx dma isr handle */
static void i2s_chl1_rx_dma_finish_isr(void)
{
	if (i2s_drv_info->chl1_cfg->rx_cfg->data_handle_cb) {
		i2s_drv_info->chl1_cfg->rx_cfg->data_handle_cb((i2s_drv_info->chl1_cfg->rx_cfg->buff_size)/2);
	}
}

/* i2s channel 2 tx dma isr handle */
static void i2s_chl2_tx_dma_finish_isr(void)
{
	if (i2s_drv_info->chl2_cfg->tx_cfg->data_handle_cb) {
		i2s_drv_info->chl2_cfg->tx_cfg->data_handle_cb((i2s_drv_info->chl2_cfg->tx_cfg->buff_size)/2);
	}
}

/* i2s channel 2 tx dma isr handle */
static void i2s_chl2_rx_dma_finish_isr(void)
{
	if (i2s_drv_info->chl2_cfg->rx_cfg->data_handle_cb) {
		i2s_drv_info->chl2_cfg->rx_cfg->data_handle_cb((i2s_drv_info->chl2_cfg->rx_cfg->buff_size)/2);
	}
}

/* i2s channel 3 tx dma isr handle */
static void i2s_chl3_tx_dma_finish_isr(void)
{
	if (i2s_drv_info->chl3_cfg->tx_cfg->data_handle_cb) {
		i2s_drv_info->chl3_cfg->tx_cfg->data_handle_cb((i2s_drv_info->chl3_cfg->tx_cfg->buff_size)/2);
	}
}

/* i2s channel 3 tx dma isr handle */
static void i2s_chl3_rx_dma_finish_isr(void)
{
	if (i2s_drv_info->chl3_cfg->rx_cfg->data_handle_cb) {
		i2s_drv_info->chl3_cfg->rx_cfg->data_handle_cb((i2s_drv_info->chl3_cfg->rx_cfg->buff_size)/2);
	}
}

static bk_err_t i2s_dma_config(dma_id_t dma_id, uint32_t *ring_buff_addr, uint32_t ring_buff_size, uint32_t transfer_len, i2s_channel_id_t chl_id, i2s_txrx_type_t type, i2s_data_handle_cb data_handle_cb)
{
	bk_err_t ret = BK_OK;
	dma_config_t dma_config;
	uint32_t i2s_data_addr;

	dma_config.mode = DMA_WORK_MODE_REPEAT;
	dma_config.chan_prio = 1;
	dma_config.src.width = DMA_DATA_WIDTH_32BITS;
	dma_config.dst.width = DMA_DATA_WIDTH_32BITS;
	dma_config.src.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.src.addr_loop_en = DMA_ADDR_LOOP_ENABLE;
	dma_config.dst.addr_inc_en = DMA_ADDR_INC_ENABLE;
	dma_config.dst.addr_loop_en = DMA_ADDR_LOOP_ENABLE;
	/* get i2s address */
	bk_i2s_get_data_addr(chl_id, &i2s_data_addr);

	if (type == I2S_TXRX_TYPE_RX) {
		switch (chl_id) {
			case I2S_CHANNEL_1:
				dma_config.src.dev = DMA_DEV_I2S_RX;	//DMA_DEV_I2S_RX
				break;
			case I2S_CHANNEL_2:
				dma_config.src.dev = DMA_DEV_I2S_RX_CH1;
				break;
			case I2S_CHANNEL_3:
				dma_config.src.dev = DMA_DEV_I2S_RX_CH2;
				break;
			default:
				break;
		}
		dma_config.dst.dev = DMA_DEV_DTCM;
		dma_config.src.start_addr = i2s_data_addr;
		dma_config.src.end_addr = i2s_data_addr + 4;
		dma_config.dst.start_addr = (uint32_t)ring_buff_addr;
		dma_config.dst.end_addr = (uint32_t)ring_buff_addr + ring_buff_size;
	} else {
		dma_config.src.dev = DMA_DEV_DTCM;
		switch (chl_id) {
			case I2S_CHANNEL_1:
				dma_config.dst.dev = DMA_DEV_I2S;
				break;
			case I2S_CHANNEL_2:
				dma_config.dst.dev = DMA_DEV_I2S_CH1;
				break;
			case I2S_CHANNEL_3:
				dma_config.dst.dev = DMA_DEV_I2S_CH2;
				break;
			default:
				break;
		}
		dma_config.src.start_addr = (uint32_t)ring_buff_addr;
		dma_config.src.end_addr = (uint32_t)ring_buff_addr + ring_buff_size;
		dma_config.dst.start_addr = i2s_data_addr;
		dma_config.dst.end_addr = i2s_data_addr + 4;
	}

	/* init dma channel */
	ret = bk_dma_init(dma_id, &dma_config);
	if (ret != BK_OK) {
		I2S_LOGE("audio adc dma channel init fail \r\n");
		return BK_FAIL;
	}

	/* set dma transfer length */
	bk_dma_set_transfer_len(dma_id, transfer_len);

	if (data_handle_cb) {
		//register isr
		switch (chl_id) {
			case I2S_CHANNEL_1:
				if (type == I2S_TXRX_TYPE_TX)
					bk_dma_register_isr(dma_id, NULL, (void *)i2s_chl1_tx_dma_finish_isr);
				else
					bk_dma_register_isr(dma_id, NULL, (void *)i2s_chl1_rx_dma_finish_isr);
				break;
			case I2S_CHANNEL_2:
				if (type == I2S_TXRX_TYPE_TX)
					bk_dma_register_isr(dma_id, NULL, (void *)i2s_chl2_tx_dma_finish_isr);
				else
					bk_dma_register_isr(dma_id, NULL, (void *)i2s_chl2_rx_dma_finish_isr);
				break;
			case I2S_CHANNEL_3:
				if (type == I2S_TXRX_TYPE_TX)
					bk_dma_register_isr(dma_id, NULL, (void *)i2s_chl3_tx_dma_finish_isr);
				else
					bk_dma_register_isr(dma_id, NULL, (void *)i2s_chl3_rx_dma_finish_isr);
				break;
			default:
				break;
		}

		bk_dma_enable_finish_interrupt(dma_id);
	}

	return BK_OK;
}


bk_err_t bk_i2s_chl_init(i2s_channel_id_t chl, i2s_txrx_type_t type, uint32_t buff_size, i2s_data_handle_cb data_handle_cb, RingBufferContext **rb)
{
	bk_err_t ret = BK_OK;
	bk_err_t err = BK_OK;

	I2S_RETURN_ON_NOT_INIT();

	switch (chl) {
		case I2S_CHANNEL_1:
				if (i2s_drv_info->chl1_cfg == NULL) {
					i2s_drv_info->chl1_cfg = (i2s_chl_cfg_t *)os_malloc(sizeof(i2s_chl_cfg_t));
					if (i2s_drv_info->chl1_cfg == NULL) {
						I2S_LOGE("malloc chl1_cfg fail \n");
						err = BK_FAIL;
						return err;
					}
					i2s_drv_info->chl1_cfg->tx_cfg = NULL;
					i2s_drv_info->chl1_cfg->rx_cfg = NULL;
				}

				if (type == I2S_TXRX_TYPE_TX) {
					//TODO
					//free tx_cfg
					//if (i2s_drv_info->chl1_cfg->tx_cfg == NULL) {
					i2s_drv_info->chl1_cfg->tx_cfg = (i2s_txrx_cfg_t *)os_malloc(sizeof(i2s_txrx_cfg_t));
					if (i2s_drv_info->chl1_cfg->tx_cfg == NULL) {
						I2S_LOGE("malloc tx_cfg fail \n");
						err = BK_FAIL;
						return err;
					} else {
						i2s_drv_info->chl1_cfg->tx_cfg->buff_addr = NULL;
						i2s_drv_info->chl1_cfg->tx_cfg->buff_size = 0;
						i2s_drv_info->chl1_cfg->tx_cfg->rb = NULL;
						i2s_drv_info->chl1_cfg->tx_cfg->dma_id = DMA_ID_MAX;
						i2s_drv_info->chl1_cfg->tx_cfg->state = I2S_TXRX_STATE_NULL;
						i2s_drv_info->chl1_cfg->tx_cfg->data_handle_cb = NULL;
					}
					//}
					i2s_drv_info->chl1_cfg->tx_cfg->buff_size = buff_size;
					i2s_drv_info->chl1_cfg->tx_cfg->buff_addr = (uint8_t *)os_malloc(buff_size);
					if (i2s_drv_info->chl1_cfg->tx_cfg->buff_addr == NULL) {
						I2S_LOGE("malloc buff_addr fail \n");
						err = BK_FAIL;
						return err;
					}
					i2s_drv_info->chl1_cfg->tx_cfg->rb = (RingBufferContext *)os_malloc(sizeof(RingBufferContext));
					if (i2s_drv_info->chl1_cfg->tx_cfg->rb == NULL) {
						I2S_LOGE("malloc rb fail \n");
						err = BK_FAIL;
						return err;
					}
					*rb = i2s_drv_info->chl1_cfg->tx_cfg->rb;

					/* init dma driver */
					ret = bk_dma_driver_init();
					if (ret != BK_OK) {
						I2S_LOGE("dma driver init failed\r\n");
						err = BK_FAIL;
						return err;
					}

					/* allocate free DMA channel */
					i2s_drv_info->chl1_cfg->tx_cfg->dma_id = bk_dma_alloc(DMA_DEV_I2S);
					if ((i2s_drv_info->chl1_cfg->tx_cfg->dma_id < DMA_ID_0) || (i2s_drv_info->chl1_cfg->tx_cfg->dma_id >= DMA_ID_MAX)) {
						I2S_LOGE("malloc adc dma fail \r\n");
						err = BK_FAIL;
						return err;
					}

					ret = i2s_dma_config(i2s_drv_info->chl1_cfg->tx_cfg->dma_id, (uint32_t *)i2s_drv_info->chl1_cfg->tx_cfg->buff_addr, buff_size, buff_size/2, chl, type, data_handle_cb);
					if (ret != BK_OK) {
						I2S_LOGE("dma config fail \r\n");
						err = BK_FAIL;
						return err;
					}
					//I2S_LOGE("dma_id: %d, tx_buff_addr: %p, buff_size: %d, tx_rb: %d \n", i2s_drv_info->chl1_cfg->tx_cfg->dma_id, i2s_drv_info->chl1_cfg->tx_cfg->buff_addr, buff_size, i2s_drv_info->chl1_cfg->tx_cfg->rb);
					ring_buffer_init(i2s_drv_info->chl1_cfg->tx_cfg->rb, i2s_drv_info->chl1_cfg->tx_cfg->buff_addr, buff_size, i2s_drv_info->chl1_cfg->tx_cfg->dma_id, RB_DMA_TYPE_READ);
					i2s_drv_info->chl1_cfg->tx_cfg->data_handle_cb = data_handle_cb;
					i2s_drv_info->chl1_cfg->tx_cfg->state = I2S_TXRX_STATE_IDLE;
				} else {
					//TODO
					//free tx_cfg
					i2s_drv_info->chl1_cfg->rx_cfg = (i2s_txrx_cfg_t *)os_malloc(sizeof(i2s_txrx_cfg_t));
					if (i2s_drv_info->chl1_cfg->rx_cfg == NULL) {
						I2S_LOGE("malloc rx_cfg fail \n");
						err = BK_FAIL;
						return err;
					} else {
						i2s_drv_info->chl1_cfg->rx_cfg->buff_addr = NULL;
						i2s_drv_info->chl1_cfg->rx_cfg->buff_size = 0;
						i2s_drv_info->chl1_cfg->rx_cfg->rb = NULL;
						i2s_drv_info->chl1_cfg->rx_cfg->dma_id = DMA_ID_MAX;
						i2s_drv_info->chl1_cfg->rx_cfg->state = I2S_TXRX_STATE_NULL;
						i2s_drv_info->chl1_cfg->rx_cfg->data_handle_cb = NULL;
					}

					i2s_drv_info->chl1_cfg->rx_cfg->buff_size = buff_size;
					i2s_drv_info->chl1_cfg->rx_cfg->buff_addr = (uint8_t *)os_malloc(buff_size);
					if (i2s_drv_info->chl1_cfg->rx_cfg->buff_addr == NULL) {
						I2S_LOGE("malloc buff_addr fail \n");
						err = BK_FAIL;
						return err;
					}
					i2s_drv_info->chl1_cfg->rx_cfg->rb = (RingBufferContext *)os_malloc(sizeof(RingBufferContext));
					if (i2s_drv_info->chl1_cfg->rx_cfg->rb == NULL) {
						I2S_LOGE("malloc rb fail \n");
						err = BK_FAIL;
						return err;
					}
					*rb = i2s_drv_info->chl1_cfg->rx_cfg->rb;

					/* init dma driver */
					ret = bk_dma_driver_init();
					if (ret != BK_OK) {
						I2S_LOGE("dma driver init failed\r\n");
						err = BK_FAIL;
						return err;
					}

					/* allocate free DMA channel */
					i2s_drv_info->chl1_cfg->rx_cfg->dma_id = bk_dma_alloc(DMA_DEV_I2S);
					if ((i2s_drv_info->chl1_cfg->rx_cfg->dma_id < DMA_ID_0) || (i2s_drv_info->chl1_cfg->rx_cfg->dma_id >= DMA_ID_MAX)) {
						I2S_LOGE("malloc adc dma fail \r\n");
						err = BK_FAIL;
						return err;
					}

					ret = i2s_dma_config(i2s_drv_info->chl1_cfg->rx_cfg->dma_id, (uint32_t *)i2s_drv_info->chl1_cfg->rx_cfg->buff_addr, buff_size, buff_size/2, chl, type, data_handle_cb);
					if (ret != BK_OK) {
						I2S_LOGE("dma config fail \r\n");
						err = BK_FAIL;
						return err;
					}

					ring_buffer_init(i2s_drv_info->chl1_cfg->rx_cfg->rb, i2s_drv_info->chl1_cfg->rx_cfg->buff_addr, buff_size, i2s_drv_info->chl1_cfg->rx_cfg->dma_id, RB_DMA_TYPE_WRITE);
					i2s_drv_info->chl1_cfg->rx_cfg->data_handle_cb = data_handle_cb;
					i2s_drv_info->chl1_cfg->rx_cfg->state = I2S_TXRX_STATE_IDLE;
				}
			break;

		case I2S_CHANNEL_2:
				if (i2s_drv_info->chl2_cfg == NULL) {
					i2s_drv_info->chl2_cfg = (i2s_chl_cfg_t *)os_malloc(sizeof(i2s_chl_cfg_t));
					if (i2s_drv_info->chl2_cfg == NULL) {
						I2S_LOGE("malloc chl2_cfg fail \n");
						err = BK_FAIL;
						return err;
					}
					i2s_drv_info->chl2_cfg->tx_cfg = NULL;
					i2s_drv_info->chl2_cfg->rx_cfg = NULL;
				}
				if (type == I2S_TXRX_TYPE_TX) {
					//TODO
					//free tx_cfg
					//if (i2s_drv_info->chl1_cfg->tx_cfg == NULL) {
					i2s_drv_info->chl2_cfg->tx_cfg = (i2s_txrx_cfg_t *)os_malloc(sizeof(i2s_txrx_cfg_t));
					if (i2s_drv_info->chl2_cfg->tx_cfg == NULL) {
						I2S_LOGE("malloc tx_cfg fail \n");
						err = BK_FAIL;
						return err;
					} else {
						i2s_drv_info->chl2_cfg->tx_cfg->buff_addr = NULL;
						i2s_drv_info->chl2_cfg->tx_cfg->buff_size = 0;
						i2s_drv_info->chl2_cfg->tx_cfg->rb = NULL;
						i2s_drv_info->chl2_cfg->tx_cfg->dma_id = DMA_ID_MAX;
						i2s_drv_info->chl2_cfg->tx_cfg->state = I2S_TXRX_STATE_NULL;
						i2s_drv_info->chl2_cfg->tx_cfg->data_handle_cb = NULL;
					}

					//}
					i2s_drv_info->chl2_cfg->tx_cfg->buff_size = buff_size;
					i2s_drv_info->chl2_cfg->tx_cfg->buff_addr = (uint8_t *)os_malloc(buff_size);
					if (i2s_drv_info->chl2_cfg->tx_cfg->buff_addr == NULL) {
						I2S_LOGE("malloc buff_addr fail \n");
						err = BK_FAIL;
						return err;
					}
					i2s_drv_info->chl2_cfg->tx_cfg->rb = (RingBufferContext *)os_malloc(sizeof(RingBufferContext));
					if (i2s_drv_info->chl2_cfg->tx_cfg->rb == NULL) {
						I2S_LOGE("malloc rb fail \n");
						err = BK_FAIL;
						return err;
					}
					*rb = i2s_drv_info->chl2_cfg->tx_cfg->rb;

					/* init dma driver */
					ret = bk_dma_driver_init();
					if (ret != BK_OK) {
						I2S_LOGE("dma driver init failed\r\n");
						err = BK_FAIL;
						return err;
					}

					/* allocate free DMA channel */
					i2s_drv_info->chl2_cfg->tx_cfg->dma_id = bk_dma_alloc(DMA_DEV_I2S);
					if ((i2s_drv_info->chl2_cfg->tx_cfg->dma_id < DMA_ID_0) || (i2s_drv_info->chl2_cfg->tx_cfg->dma_id >= DMA_ID_MAX)) {
						I2S_LOGE("malloc adc dma fail \r\n");
						err = BK_FAIL;
						return err;
					}

					ret = i2s_dma_config(i2s_drv_info->chl2_cfg->tx_cfg->dma_id, (uint32_t *)i2s_drv_info->chl2_cfg->tx_cfg->buff_addr, buff_size, buff_size/2, chl, type, data_handle_cb);
					if (ret != BK_OK) {
						I2S_LOGE("dma config fail \r\n");
						err = BK_FAIL;
						return err;
					}

					ring_buffer_init(i2s_drv_info->chl2_cfg->tx_cfg->rb, i2s_drv_info->chl2_cfg->tx_cfg->buff_addr, buff_size, i2s_drv_info->chl2_cfg->tx_cfg->dma_id, RB_DMA_TYPE_READ);
					i2s_drv_info->chl2_cfg->tx_cfg->data_handle_cb = data_handle_cb;
					i2s_drv_info->chl2_cfg->tx_cfg->state = I2S_TXRX_STATE_IDLE;
				} else {
					//TODO
					//free tx_cfg
					i2s_drv_info->chl2_cfg->rx_cfg = (i2s_txrx_cfg_t *)os_malloc(sizeof(i2s_txrx_cfg_t));
					if (i2s_drv_info->chl2_cfg->rx_cfg == NULL) {
						I2S_LOGE("malloc rx_cfg fail \n");
						err = BK_FAIL;
						return err;
					} else {
						i2s_drv_info->chl2_cfg->rx_cfg->buff_addr = NULL;
						i2s_drv_info->chl2_cfg->rx_cfg->buff_size = 0;
						i2s_drv_info->chl2_cfg->rx_cfg->rb = NULL;
						i2s_drv_info->chl2_cfg->rx_cfg->dma_id = DMA_ID_MAX;
						i2s_drv_info->chl2_cfg->rx_cfg->state = I2S_TXRX_STATE_NULL;
						i2s_drv_info->chl2_cfg->rx_cfg->data_handle_cb = NULL;
					}

					i2s_drv_info->chl2_cfg->rx_cfg->buff_size = buff_size;
					i2s_drv_info->chl2_cfg->rx_cfg->buff_addr = (uint8_t *)os_malloc(buff_size);
					if (i2s_drv_info->chl2_cfg->rx_cfg->buff_addr == NULL) {
						I2S_LOGE("malloc buff_addr fail \n");
						err = BK_FAIL;
						return err;
					}
					i2s_drv_info->chl2_cfg->rx_cfg->rb = (RingBufferContext *)os_malloc(sizeof(RingBufferContext));
					if (i2s_drv_info->chl2_cfg->rx_cfg->rb == NULL) {
						I2S_LOGE("malloc rb fail \n");
						err = BK_FAIL;
						return err;
					}
					*rb = i2s_drv_info->chl2_cfg->rx_cfg->rb;

					/* init dma driver */
					ret = bk_dma_driver_init();
					if (ret != BK_OK) {
						I2S_LOGE("dma driver init failed\r\n");
						err = BK_FAIL;
						return err;
					}

					/* allocate free DMA channel */
					i2s_drv_info->chl2_cfg->rx_cfg->dma_id = bk_dma_alloc(DMA_DEV_I2S);
					if ((i2s_drv_info->chl2_cfg->rx_cfg->dma_id < DMA_ID_0) || (i2s_drv_info->chl2_cfg->rx_cfg->dma_id >= DMA_ID_MAX)) {
						I2S_LOGE("malloc adc dma fail \r\n");
						err = BK_FAIL;
						return err;
					}

					ret = i2s_dma_config(i2s_drv_info->chl2_cfg->rx_cfg->dma_id, (uint32_t *)i2s_drv_info->chl2_cfg->rx_cfg->buff_addr, buff_size, buff_size/2, chl, type, data_handle_cb);
					if (ret != BK_OK) {
						I2S_LOGE("dma config fail \r\n");
						err = BK_FAIL;
						return err;
					}

					ring_buffer_init(i2s_drv_info->chl2_cfg->rx_cfg->rb, i2s_drv_info->chl2_cfg->rx_cfg->buff_addr, buff_size, i2s_drv_info->chl2_cfg->rx_cfg->dma_id, RB_DMA_TYPE_WRITE);
					i2s_drv_info->chl2_cfg->rx_cfg->data_handle_cb = data_handle_cb;
					i2s_drv_info->chl2_cfg->rx_cfg->state = I2S_TXRX_STATE_IDLE;
				}
			break;

		case I2S_CHANNEL_3:
				if (i2s_drv_info->chl3_cfg == NULL) {
					i2s_drv_info->chl3_cfg = (i2s_chl_cfg_t *)os_malloc(sizeof(i2s_chl_cfg_t));
					if (i2s_drv_info->chl3_cfg == NULL) {
						I2S_LOGE("malloc chl1_cfg fail \n");
						err = BK_FAIL;
						return err;
					}
					i2s_drv_info->chl3_cfg->tx_cfg = NULL;
					i2s_drv_info->chl3_cfg->rx_cfg = NULL;
				}
				if (type == I2S_TXRX_TYPE_TX) {
					//TODO
					//free tx_cfg
					//if (i2s_drv_info->chl1_cfg->tx_cfg == NULL) {
					i2s_drv_info->chl3_cfg->tx_cfg = (i2s_txrx_cfg_t *)os_malloc(sizeof(i2s_txrx_cfg_t));
					if (i2s_drv_info->chl3_cfg->tx_cfg == NULL) {
						I2S_LOGE("malloc tx_cfg fail \n");
						err = BK_FAIL;
						return err;
					} else {
						i2s_drv_info->chl3_cfg->tx_cfg->buff_addr = NULL;
						i2s_drv_info->chl3_cfg->tx_cfg->buff_size = 0;
						i2s_drv_info->chl3_cfg->tx_cfg->rb = NULL;
						i2s_drv_info->chl3_cfg->tx_cfg->dma_id = DMA_ID_MAX;
						i2s_drv_info->chl3_cfg->tx_cfg->state = I2S_TXRX_STATE_NULL;
						i2s_drv_info->chl3_cfg->tx_cfg->data_handle_cb = NULL;
					}

					//}
					i2s_drv_info->chl3_cfg->tx_cfg->buff_size = buff_size;
					i2s_drv_info->chl3_cfg->tx_cfg->buff_addr = (uint8_t *)os_malloc(buff_size);
					if (i2s_drv_info->chl3_cfg->tx_cfg->buff_addr == NULL) {
						I2S_LOGE("malloc buff_addr fail \n");
						err = BK_FAIL;
						return err;
					}
					i2s_drv_info->chl3_cfg->tx_cfg->rb = (RingBufferContext *)os_malloc(sizeof(RingBufferContext));
					if (i2s_drv_info->chl3_cfg->tx_cfg->rb == NULL) {
						I2S_LOGE("malloc rb fail \n");
						err = BK_FAIL;
						return err;
					}
					*rb = i2s_drv_info->chl3_cfg->tx_cfg->rb;

					/* init dma driver */
					ret = bk_dma_driver_init();
					if (ret != BK_OK) {
						I2S_LOGE("dma driver init failed\r\n");
						err = BK_FAIL;
						return err;
					}

					/* allocate free DMA channel */
					i2s_drv_info->chl3_cfg->tx_cfg->dma_id = bk_dma_alloc(DMA_DEV_I2S);
					if ((i2s_drv_info->chl3_cfg->tx_cfg->dma_id < DMA_ID_0) || (i2s_drv_info->chl3_cfg->tx_cfg->dma_id >= DMA_ID_MAX)) {
						I2S_LOGE("malloc adc dma fail \r\n");
						err = BK_FAIL;
						return err;
					}

					ret = i2s_dma_config(i2s_drv_info->chl3_cfg->tx_cfg->dma_id, (uint32_t *)i2s_drv_info->chl3_cfg->tx_cfg->buff_addr, buff_size, buff_size/2, chl, type, data_handle_cb);
					if (ret != BK_OK) {
						I2S_LOGE("dma config fail \r\n");
						err = BK_FAIL;
						return err;
					}

					ring_buffer_init(i2s_drv_info->chl3_cfg->tx_cfg->rb, i2s_drv_info->chl3_cfg->tx_cfg->buff_addr, buff_size, i2s_drv_info->chl3_cfg->tx_cfg->dma_id, RB_DMA_TYPE_READ);
					i2s_drv_info->chl3_cfg->tx_cfg->data_handle_cb = data_handle_cb;
					i2s_drv_info->chl3_cfg->tx_cfg->state = I2S_TXRX_STATE_IDLE;
				} else {
					//TODO
					//free tx_cfg
					i2s_drv_info->chl3_cfg->rx_cfg = (i2s_txrx_cfg_t *)os_malloc(sizeof(i2s_txrx_cfg_t));
					if (i2s_drv_info->chl3_cfg->rx_cfg == NULL) {
						I2S_LOGE("malloc rx_cfg fail \n");
						err = BK_FAIL;
						return err;
					} else {
						i2s_drv_info->chl3_cfg->rx_cfg->buff_addr = NULL;
						i2s_drv_info->chl3_cfg->rx_cfg->buff_size = 0;
						i2s_drv_info->chl3_cfg->rx_cfg->rb = NULL;
						i2s_drv_info->chl3_cfg->rx_cfg->dma_id = DMA_ID_MAX;
						i2s_drv_info->chl3_cfg->rx_cfg->state = I2S_TXRX_STATE_NULL;
						i2s_drv_info->chl3_cfg->rx_cfg->data_handle_cb = NULL;
					}

					i2s_drv_info->chl3_cfg->rx_cfg->buff_size = buff_size;
					i2s_drv_info->chl3_cfg->rx_cfg->buff_addr = (uint8_t *)os_malloc(buff_size);
					if (i2s_drv_info->chl3_cfg->rx_cfg->buff_addr == NULL) {
						I2S_LOGE("malloc buff_addr fail \n");
						err = BK_FAIL;
						return err;
					}
					i2s_drv_info->chl3_cfg->rx_cfg->rb = (RingBufferContext *)os_malloc(sizeof(RingBufferContext));
					if (i2s_drv_info->chl3_cfg->rx_cfg->rb == NULL) {
						I2S_LOGE("malloc rb fail \n");
						err = BK_FAIL;
						return err;
					}
					*rb = i2s_drv_info->chl3_cfg->rx_cfg->rb;

					/* init dma driver */
					ret = bk_dma_driver_init();
					if (ret != BK_OK) {
						I2S_LOGE("dma driver init failed\r\n");
						err = BK_FAIL;
						return err;
					}

					/* allocate free DMA channel */
					i2s_drv_info->chl3_cfg->rx_cfg->dma_id = bk_dma_alloc(DMA_DEV_I2S);
					if ((i2s_drv_info->chl3_cfg->rx_cfg->dma_id < DMA_ID_0) || (i2s_drv_info->chl3_cfg->rx_cfg->dma_id >= DMA_ID_MAX)) {
						I2S_LOGE("malloc adc dma fail \r\n");
						err = BK_FAIL;
						return err;
					}

					ret = i2s_dma_config(i2s_drv_info->chl3_cfg->rx_cfg->dma_id, (uint32_t *)i2s_drv_info->chl3_cfg->rx_cfg->buff_addr, buff_size, buff_size/2, chl, type, data_handle_cb);
					if (ret != BK_OK) {
						I2S_LOGE("dma config fail \r\n");
						err = BK_FAIL;
						return err;
					}

					ring_buffer_init(i2s_drv_info->chl3_cfg->rx_cfg->rb, i2s_drv_info->chl3_cfg->rx_cfg->buff_addr, buff_size, i2s_drv_info->chl3_cfg->rx_cfg->dma_id, RB_DMA_TYPE_WRITE);
					i2s_drv_info->chl3_cfg->rx_cfg->data_handle_cb = data_handle_cb;
					i2s_drv_info->chl3_cfg->rx_cfg->state = I2S_TXRX_STATE_IDLE;
				}
			break;

		default:
			break;
	}

	return BK_OK;
}

bk_err_t bk_i2s_chl_deinit(i2s_channel_id_t chl, i2s_txrx_type_t type)
{
	switch (chl) {
		case I2S_CHANNEL_1:
			if (type == I2S_TXRX_TYPE_TX) {
				if (i2s_drv_info->chl1_cfg && i2s_drv_info->chl1_cfg->tx_cfg) {
					/* stop data transfer */
					bk_dma_stop(i2s_drv_info->chl1_cfg->tx_cfg->dma_id);
					ring_buffer_clear(i2s_drv_info->chl1_cfg->tx_cfg->rb);
					bk_dma_deinit(i2s_drv_info->chl1_cfg->tx_cfg->dma_id);
					bk_dma_free(DMA_DEV_I2S, i2s_drv_info->chl1_cfg->tx_cfg->dma_id);

					if (i2s_drv_info->chl1_cfg->tx_cfg->rb)
						os_free(i2s_drv_info->chl1_cfg->tx_cfg->rb);
					i2s_drv_info->chl1_cfg->tx_cfg->rb = NULL;
					if (i2s_drv_info->chl1_cfg->tx_cfg->buff_addr)
						os_free(i2s_drv_info->chl1_cfg->tx_cfg->buff_addr);
					i2s_drv_info->chl1_cfg->tx_cfg->buff_addr = NULL;
					i2s_drv_info->chl1_cfg->tx_cfg->buff_size = 0;
					i2s_drv_info->chl1_cfg->tx_cfg->dma_id = DMA_ID_MAX;
					i2s_drv_info->chl1_cfg->tx_cfg->state = I2S_TXRX_STATE_NULL;
					i2s_drv_info->chl1_cfg->tx_cfg->data_handle_cb = NULL;
					if (i2s_drv_info->chl1_cfg->tx_cfg)
						os_free(i2s_drv_info->chl1_cfg->tx_cfg);
					i2s_drv_info->chl1_cfg->tx_cfg = NULL;
				}
			} else {
				if (i2s_drv_info->chl1_cfg && i2s_drv_info->chl1_cfg->rx_cfg) {
					/* stop data transfer */
					bk_dma_stop(i2s_drv_info->chl1_cfg->rx_cfg->dma_id);
					bk_dma_deinit(i2s_drv_info->chl1_cfg->rx_cfg->dma_id);
					bk_dma_free(DMA_DEV_I2S, i2s_drv_info->chl1_cfg->rx_cfg->dma_id);

					ring_buffer_clear(i2s_drv_info->chl1_cfg->rx_cfg->rb);
					if (i2s_drv_info->chl1_cfg->rx_cfg->rb)
						os_free(i2s_drv_info->chl1_cfg->rx_cfg->rb);
					i2s_drv_info->chl1_cfg->rx_cfg->rb = NULL;
					if (i2s_drv_info->chl1_cfg->rx_cfg->buff_addr)
						os_free(i2s_drv_info->chl1_cfg->rx_cfg->buff_addr);
					i2s_drv_info->chl1_cfg->rx_cfg->buff_addr = NULL;
					i2s_drv_info->chl1_cfg->rx_cfg->buff_size = 0;
					i2s_drv_info->chl1_cfg->rx_cfg->dma_id = DMA_ID_MAX;
					i2s_drv_info->chl1_cfg->rx_cfg->state = I2S_TXRX_STATE_NULL;
					i2s_drv_info->chl1_cfg->rx_cfg->data_handle_cb = NULL;
					if (i2s_drv_info->chl1_cfg->rx_cfg)
						os_free(i2s_drv_info->chl1_cfg->rx_cfg);
					i2s_drv_info->chl1_cfg->rx_cfg = NULL;
				}
			}
			if (i2s_drv_info->chl1_cfg->tx_cfg == NULL && i2s_drv_info->chl1_cfg->rx_cfg == NULL) {
				if (i2s_drv_info->chl1_cfg) {
					os_free(i2s_drv_info->chl1_cfg);
					i2s_drv_info->chl1_cfg = NULL;
				}
			}
			break;

		case I2S_CHANNEL_2:
			if (type == I2S_TXRX_TYPE_TX) {
				if (i2s_drv_info->chl2_cfg && i2s_drv_info->chl2_cfg->tx_cfg) {
					/* stop data transfer */
					bk_dma_stop(i2s_drv_info->chl2_cfg->tx_cfg->dma_id);
					bk_dma_deinit(i2s_drv_info->chl2_cfg->tx_cfg->dma_id);
					bk_dma_free(DMA_DEV_I2S, i2s_drv_info->chl2_cfg->tx_cfg->dma_id);

					ring_buffer_clear(i2s_drv_info->chl2_cfg->tx_cfg->rb);
					if (i2s_drv_info->chl2_cfg->tx_cfg->rb)
						os_free(i2s_drv_info->chl2_cfg->tx_cfg->rb);
					i2s_drv_info->chl2_cfg->tx_cfg->rb = NULL;
					if (i2s_drv_info->chl2_cfg->tx_cfg->buff_addr)
						os_free(i2s_drv_info->chl2_cfg->tx_cfg->buff_addr);
					i2s_drv_info->chl2_cfg->tx_cfg->buff_addr = NULL;
					i2s_drv_info->chl2_cfg->tx_cfg->buff_size = 0;
					i2s_drv_info->chl2_cfg->tx_cfg->dma_id = DMA_ID_MAX;
					i2s_drv_info->chl2_cfg->tx_cfg->state = I2S_TXRX_STATE_NULL;
					i2s_drv_info->chl2_cfg->tx_cfg->data_handle_cb = NULL;
					if (i2s_drv_info->chl2_cfg->tx_cfg)
						os_free(i2s_drv_info->chl2_cfg->tx_cfg);
					i2s_drv_info->chl2_cfg->tx_cfg = NULL;
				}
			} else {
				if (i2s_drv_info->chl2_cfg && i2s_drv_info->chl2_cfg->rx_cfg) {
					/* stop data transfer */
					bk_dma_stop(i2s_drv_info->chl2_cfg->rx_cfg->dma_id);
					bk_dma_deinit(i2s_drv_info->chl2_cfg->rx_cfg->dma_id);
					bk_dma_free(DMA_DEV_I2S, i2s_drv_info->chl2_cfg->rx_cfg->dma_id);

					ring_buffer_clear(i2s_drv_info->chl2_cfg->rx_cfg->rb);
					if (i2s_drv_info->chl2_cfg->rx_cfg->rb)
						os_free(i2s_drv_info->chl2_cfg->rx_cfg->rb);
					i2s_drv_info->chl2_cfg->rx_cfg->rb = NULL;
					if (i2s_drv_info->chl2_cfg->rx_cfg->buff_addr)
						os_free(i2s_drv_info->chl2_cfg->rx_cfg->buff_addr);
					i2s_drv_info->chl2_cfg->rx_cfg->buff_addr = NULL;
					i2s_drv_info->chl2_cfg->rx_cfg->buff_size = 0;
					i2s_drv_info->chl2_cfg->rx_cfg->dma_id = DMA_ID_MAX;
					i2s_drv_info->chl2_cfg->rx_cfg->state = I2S_TXRX_STATE_NULL;
					i2s_drv_info->chl2_cfg->rx_cfg->data_handle_cb = NULL;
					if (i2s_drv_info->chl2_cfg->rx_cfg)
						os_free(i2s_drv_info->chl2_cfg->rx_cfg);
					i2s_drv_info->chl2_cfg->rx_cfg = NULL;
				}
			}
			if (i2s_drv_info->chl2_cfg->tx_cfg == NULL && i2s_drv_info->chl2_cfg->rx_cfg == NULL) {
				if (i2s_drv_info->chl2_cfg)
					os_free(i2s_drv_info->chl2_cfg);
				i2s_drv_info->chl2_cfg = NULL;
			}
			break;

		case I2S_CHANNEL_3:
			if (type == I2S_TXRX_TYPE_TX) {
				if (i2s_drv_info->chl3_cfg && i2s_drv_info->chl3_cfg->tx_cfg) {
					/* stop data transfer */
					bk_dma_stop(i2s_drv_info->chl3_cfg->tx_cfg->dma_id);
					bk_dma_deinit(i2s_drv_info->chl3_cfg->tx_cfg->dma_id);
					bk_dma_free(DMA_DEV_I2S, i2s_drv_info->chl3_cfg->tx_cfg->dma_id);

					ring_buffer_clear(i2s_drv_info->chl3_cfg->tx_cfg->rb);
					if (i2s_drv_info->chl3_cfg->tx_cfg->rb)
						os_free(i2s_drv_info->chl3_cfg->tx_cfg->rb);
					i2s_drv_info->chl3_cfg->tx_cfg->rb = NULL;
					if (i2s_drv_info->chl3_cfg->tx_cfg->buff_addr)
						os_free(i2s_drv_info->chl3_cfg->tx_cfg->buff_addr);
					i2s_drv_info->chl3_cfg->tx_cfg->buff_addr = NULL;
					i2s_drv_info->chl3_cfg->tx_cfg->buff_size = 0;
					i2s_drv_info->chl3_cfg->tx_cfg->dma_id = DMA_ID_MAX;
					i2s_drv_info->chl3_cfg->tx_cfg->state = I2S_TXRX_STATE_NULL;
					i2s_drv_info->chl3_cfg->tx_cfg->data_handle_cb = NULL;
					if (i2s_drv_info->chl3_cfg->tx_cfg)
						os_free(i2s_drv_info->chl3_cfg->tx_cfg);
					i2s_drv_info->chl3_cfg->tx_cfg = NULL;
				}
			} else {
				if (i2s_drv_info->chl3_cfg && i2s_drv_info->chl3_cfg->rx_cfg) {
					/* stop data transfer */
					bk_dma_stop(i2s_drv_info->chl3_cfg->rx_cfg->dma_id);
					bk_dma_deinit(i2s_drv_info->chl3_cfg->rx_cfg->dma_id);
					bk_dma_free(DMA_DEV_I2S, i2s_drv_info->chl3_cfg->rx_cfg->dma_id);

					ring_buffer_clear(i2s_drv_info->chl3_cfg->rx_cfg->rb);
					if (i2s_drv_info->chl3_cfg->rx_cfg->rb)
						os_free(i2s_drv_info->chl3_cfg->rx_cfg->rb);
					i2s_drv_info->chl3_cfg->rx_cfg->rb = NULL;
					if (i2s_drv_info->chl3_cfg->rx_cfg->buff_addr)
						os_free(i2s_drv_info->chl3_cfg->rx_cfg->buff_addr);
					i2s_drv_info->chl3_cfg->rx_cfg->buff_addr = NULL;
					i2s_drv_info->chl3_cfg->rx_cfg->buff_size = 0;
					i2s_drv_info->chl3_cfg->rx_cfg->dma_id = DMA_ID_MAX;
					i2s_drv_info->chl3_cfg->rx_cfg->state = I2S_TXRX_STATE_NULL;
					i2s_drv_info->chl3_cfg->rx_cfg->data_handle_cb = NULL;
					if (i2s_drv_info->chl3_cfg->rx_cfg)
						os_free(i2s_drv_info->chl3_cfg->rx_cfg);
					i2s_drv_info->chl3_cfg->rx_cfg = NULL;
				}
			}
			if (i2s_drv_info->chl3_cfg->tx_cfg == NULL && i2s_drv_info->chl3_cfg->rx_cfg == NULL) {
				if (i2s_drv_info->chl3_cfg)
					os_free(i2s_drv_info->chl3_cfg);
				i2s_drv_info->chl3_cfg = NULL;
			}
			break;

		default:
			break;
	}

	return BK_OK;
}

bk_err_t bk_i2s_start(void)
{
	uint32_t temp_data = 0xF0F0F0F0;

	/* first enable i2s */
	bk_i2s_enable(I2S_ENABLE);

	/* chl1_tx */
	if (i2s_drv_info->chl1_cfg &&
		i2s_drv_info->chl1_cfg->tx_cfg &&
		(i2s_drv_info->chl1_cfg->tx_cfg->state == I2S_TXRX_STATE_IDLE || i2s_drv_info->chl1_cfg->tx_cfg->state == I2S_TXRX_STATE_STOP))
	{
		bk_dma_start(i2s_drv_info->chl1_cfg->tx_cfg->dma_id);
		i2s_drv_info->chl1_cfg->tx_cfg->state = I2S_TXRX_STATE_START;
	}

	/* chl1_rx */
	if (i2s_drv_info->chl1_cfg &&
		i2s_drv_info->chl1_cfg->rx_cfg &&
		(i2s_drv_info->chl1_cfg->rx_cfg->state == I2S_TXRX_STATE_IDLE || i2s_drv_info->chl1_cfg->rx_cfg->state == I2S_TXRX_STATE_STOP))
	{
		bk_dma_start(i2s_drv_info->chl1_cfg->rx_cfg->dma_id);
		i2s_drv_info->chl1_cfg->rx_cfg->state = I2S_TXRX_STATE_START;
	}

	/* chl2_tx */
	if (i2s_drv_info->chl2_cfg &&
		i2s_drv_info->chl2_cfg->tx_cfg &&
		(i2s_drv_info->chl2_cfg->tx_cfg->state == I2S_TXRX_STATE_IDLE || i2s_drv_info->chl2_cfg->tx_cfg->state == I2S_TXRX_STATE_STOP))
	{
		bk_dma_start(i2s_drv_info->chl2_cfg->tx_cfg->dma_id);
		i2s_drv_info->chl2_cfg->tx_cfg->state = I2S_TXRX_STATE_START;
	}

	/* chl2_rx */
	if (i2s_drv_info->chl2_cfg &&
		i2s_drv_info->chl2_cfg->rx_cfg &&
		(i2s_drv_info->chl2_cfg->rx_cfg->state == I2S_TXRX_STATE_IDLE || i2s_drv_info->chl2_cfg->rx_cfg->state == I2S_TXRX_STATE_STOP))
	{
		bk_dma_start(i2s_drv_info->chl2_cfg->rx_cfg->dma_id);
		i2s_drv_info->chl2_cfg->rx_cfg->state = I2S_TXRX_STATE_START;
	}

	/* chl3_tx */
	if (i2s_drv_info->chl3_cfg &&
		i2s_drv_info->chl3_cfg->tx_cfg &&
		(i2s_drv_info->chl3_cfg->tx_cfg->state == I2S_TXRX_STATE_IDLE || i2s_drv_info->chl3_cfg->tx_cfg->state == I2S_TXRX_STATE_STOP))
	{
		bk_dma_start(i2s_drv_info->chl3_cfg->tx_cfg->dma_id);
		i2s_drv_info->chl3_cfg->tx_cfg->state = I2S_TXRX_STATE_START;
	}

	/* chl3_rx */
	if (i2s_drv_info->chl3_cfg &&
		i2s_drv_info->chl3_cfg->rx_cfg &&
		(i2s_drv_info->chl3_cfg->rx_cfg->state == I2S_TXRX_STATE_IDLE || i2s_drv_info->chl3_cfg->rx_cfg->state == I2S_TXRX_STATE_STOP))
	{
		bk_dma_start(i2s_drv_info->chl3_cfg->rx_cfg->dma_id);
		i2s_drv_info->chl3_cfg->rx_cfg->state = I2S_TXRX_STATE_START;
	}

	switch (i2s_drv_info->config.work_mode) {
		case I2S_WORK_MODE_I2S:
		case I2S_WORK_MODE_LEFTJUST:
		case I2S_WORK_MODE_RIGHTJUST:
			bk_i2s_write_data(2, &temp_data, 1);
			bk_i2s_write_data(3, &temp_data, 1);
			break;

		case I2S_WORK_MODE_SHORTFAMSYNC:
		case I2S_WORK_MODE_LONGFAMSYNC:
			if (i2s_drv_info->config.parallel_en == I2S_PARALLEL_ENABLE) {
				bk_i2s_write_data(2, &temp_data, 1);
				bk_i2s_write_data(3, &temp_data, 1);
			} else {
				if (i2s_drv_info->chl2_cfg != NULL) {
					bk_i2s_write_data(2, &temp_data, 1);
				}
				if (i2s_drv_info->chl3_cfg != NULL) {
					bk_i2s_write_data(3, &temp_data, 1);
				}
			}
			break;

		case I2S_WORK_MODE_NORMAL2BD:
		case I2S_WORK_MODE_DELAY2BD:
			i2s_drv_info->config.parallel_en = I2S_PARALLEL_ENABLE;
			bk_i2s_write_data(2, &temp_data, 1);
			bk_i2s_write_data(3, &temp_data, 1);
			break;

		case I2S_WORK_MODE_RSVD:
			break;

		default:
			break;
	}

	return BK_OK;
}

bk_err_t bk_i2s_stop(void)
{
	/* chl1_tx */
	if (i2s_drv_info->chl1_cfg &&
		i2s_drv_info->chl1_cfg->tx_cfg &&
		i2s_drv_info->chl1_cfg->tx_cfg->state == I2S_TXRX_STATE_START)
	{
		bk_dma_stop(i2s_drv_info->chl1_cfg->tx_cfg->dma_id);
		i2s_drv_info->chl1_cfg->tx_cfg->state = I2S_TXRX_STATE_STOP;
	}

	/* chl1_rx */
	if (i2s_drv_info->chl1_cfg &&
		i2s_drv_info->chl1_cfg->rx_cfg &&
		i2s_drv_info->chl1_cfg->rx_cfg->state == I2S_TXRX_STATE_START)
	{
		bk_dma_stop(i2s_drv_info->chl1_cfg->rx_cfg->dma_id);
		i2s_drv_info->chl1_cfg->rx_cfg->state = I2S_TXRX_STATE_STOP;
	}

	/* chl2_tx */
	if (i2s_drv_info->chl2_cfg &&
		i2s_drv_info->chl2_cfg->tx_cfg &&
		i2s_drv_info->chl2_cfg->tx_cfg->state == I2S_TXRX_STATE_START)
	{
		bk_dma_stop(i2s_drv_info->chl2_cfg->tx_cfg->dma_id);
		i2s_drv_info->chl2_cfg->tx_cfg->state = I2S_TXRX_STATE_STOP;
	}

	/* chl2_rx */
	if (i2s_drv_info->chl2_cfg &&
		i2s_drv_info->chl2_cfg->rx_cfg &&
		i2s_drv_info->chl2_cfg->rx_cfg->state == I2S_TXRX_STATE_START)
	{
		bk_dma_stop(i2s_drv_info->chl2_cfg->rx_cfg->dma_id);
		i2s_drv_info->chl2_cfg->rx_cfg->state = I2S_TXRX_STATE_STOP;
	}

	/* chl3_tx */
	if (i2s_drv_info->chl3_cfg &&
		i2s_drv_info->chl3_cfg->tx_cfg &&
		i2s_drv_info->chl3_cfg->tx_cfg->state == I2S_TXRX_STATE_START)
	{
		bk_dma_stop(i2s_drv_info->chl3_cfg->tx_cfg->dma_id);
		i2s_drv_info->chl3_cfg->tx_cfg->state = I2S_TXRX_STATE_STOP;
	}

	/* chl3_rx */
	if (i2s_drv_info->chl3_cfg &&
		i2s_drv_info->chl3_cfg->rx_cfg &&
		i2s_drv_info->chl3_cfg->rx_cfg->state == I2S_TXRX_STATE_START)
	{
		bk_dma_stop(i2s_drv_info->chl3_cfg->rx_cfg->dma_id);
		i2s_drv_info->chl3_cfg->rx_cfg->state = I2S_TXRX_STATE_STOP;
	}

	bk_i2s_enable(I2S_DISABLE);

	return BK_OK;
}

