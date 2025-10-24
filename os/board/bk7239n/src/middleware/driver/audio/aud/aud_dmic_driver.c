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
#include "aud_hal.h"
#include "sys_driver.h"
#include "clock_driver.h"
#include <os/os.h>
#include <os/mem.h>
#include <driver/int.h>
#include <driver/gpio.h>
#include "gpio_driver.h"

#include <driver/aud_dmic_types.h>
#include <driver/aud_dmic.h>


#define TAG "aud_dmic_drv"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

#define AUD_DMIC_RETURN_ON_NOT_INIT() do {\
			if (!bk_aud_get_module_init_sta(AUD_MODULE_DMIC)) {\
				return BK_ERR_AUD_DMIC_NOT_INIT;\
			}\
		} while(0)


extern void delay(int num);

bk_err_t bk_aud_dmic_init(aud_dmic_config_t *dmic_config)
{
	BK_RETURN_ON_NULL(dmic_config);

	if (bk_aud_get_module_init_sta(AUD_MODULE_DMIC)) {
		LOGW("[%s]aud dmic is init already \n", __func__);
		goto fail;
	}

	bk_aud_set_module_init_sta(AUD_MODULE_DMIC, true);

	/* audio common driver init */
	if (BK_OK != bk_aud_driver_init()) {
		LOGE("%s, audio driver init fail, line: %d \n", __func__, __LINE__);
		goto fail;
	}

	/* config gpio */
	gpio_dev_unmap(GPIO_8);
	gpio_dev_map(GPIO_8, GPIO_DEV_DMIC1_CLK);
	gpio_dev_unmap(GPIO_9);
	gpio_dev_map(GPIO_9, GPIO_DEV_DMIC1_DAT);

	//reserved channel config

	bk_aud_clk_config(AUD_CLK_APLL);

	if (BK_OK != bk_aud_dmic_set_samp_rate(dmic_config->samp_rate))
		goto fail;

	return BK_OK;
fail:
	bk_aud_set_module_init_sta(AUD_MODULE_DMIC, false);
	return BK_FAIL;
}

bk_err_t bk_aud_dmic_deinit(void)
{
	AUD_DMIC_RETURN_ON_NOT_INIT();

	//config audio adc registers to default value
	bk_aud_dmic_set_samp_rate(0);
	aud_hal_set_audio_config_apll_sel(0);
	aud_hal_set_audio_config_dig_mic_div(0);
	aud_hal_set_audio_config_dmic_cic_sel(0);

	sys_drv_dmic_clk_div_set(0);

	bk_aud_set_module_init_sta(AUD_MODULE_DMIC, false);

	return BK_OK;
}


bk_err_t bk_aud_dmic_set_samp_rate(uint32_t samp_rate)
{
	AUD_DMIC_RETURN_ON_NOT_INIT();

	if (samp_rate != 11025 && samp_rate != 12000 && samp_rate != 16000 && samp_rate != 22050
		&& samp_rate != 24000 && samp_rate != 32000 && samp_rate != 44100 && samp_rate != 48000) {
			LOGE("%s, line: %d, sample rate: %d not support \n", __func__, __LINE__, samp_rate);
	}

	/* get audio clock config */
	if (1 == aud_hal_get_audio_config_apll_sel()) {
		/* config apll */
		sys_drv_apll_en(1);
		//set apll clock config
		if (samp_rate == 11025 || samp_rate == 22050 || samp_rate == 44100) {
			sys_drv_apll_cal_val_set(0x88AF2EC9);
		} else {
			sys_drv_apll_cal_val_set(0x8973CA6F);
		}
		sys_drv_apll_config_set(0xC2A0AE86);
		sys_drv_apll_spi_trigger_set(1);
		delay(10);
		sys_drv_apll_spi_trigger_set(0);
	}

	/* dmic source clock = APLL/2 divide */
	sys_drv_dmic_clk_div_set(1);

	switch(samp_rate)
	{
		case 11025:		//apll: 90.3168MHz  22579.2/(1+1)/64
			aud_hal_set_audio_config_dig_mic_div(1);
			aud_hal_set_audio_config_dmic_cic_sel(0x2);
			break;

		case 12000:		//apll: 98.304MHz  24576/(1+1)/64
			aud_hal_set_audio_config_dig_mic_div(1);
			aud_hal_set_audio_config_dmic_cic_sel(0x2);
			break;

		case 16000:		//apll: 98.304MHz  24576/(23+1)/64
			aud_hal_set_audio_config_dig_mic_div(23);
			aud_hal_set_audio_config_dmic_cic_sel(0x2);
			break;

		case 22050:		//apll: 90.3168MHz  22579.2/(3+1)/64
			aud_hal_set_audio_config_dig_mic_div(3);
			aud_hal_set_audio_config_dmic_cic_sel(0x2);
			break;

		case 24000:		//apll: 98.304MHz  24576/(3+1)/64
			aud_hal_set_audio_config_dig_mic_div(3);
			aud_hal_set_audio_config_dmic_cic_sel(0x2);
			break;

		case 32000:		//apll: 98.304MHz  24576/(11+1)/64
			aud_hal_set_audio_config_dig_mic_div(11);
			aud_hal_set_audio_config_dmic_cic_sel(0x2);
			break;

		case 44100:		//apll: 90.3168MHz  22579.2/‬(7+1)/64
			aud_hal_set_audio_config_dig_mic_div(7);
			aud_hal_set_audio_config_dmic_cic_sel(0x2);
			break;

		case 48000:		//apll: 98.304MHz  24576/(7+1)/64
			aud_hal_set_audio_config_dig_mic_div(7);
			aud_hal_set_audio_config_dmic_cic_sel(0x2);
			break;

		default:
			break;
	}

	return BK_OK;
}

/* get dmic fifo port address */
bk_err_t bk_aud_dmic_get_fifo_addr(uint32_t *dmic_fifo_addr)
{
	*dmic_fifo_addr = AUD_DMIC_FPORT_ADDR;
	return BK_OK;
}

/* get dtmf fifo data */
bk_err_t bk_aud_dmic_get_fifo_data(uint32_t *dmic_data)
{
	AUD_DMIC_RETURN_ON_NOT_INIT();
	*dmic_data = aud_hal_get_dmic_fport_value();
	return BK_OK;
}

/* get audio adc fifo and agc status */
bk_err_t bk_aud_dmic_get_status(uint32_t *dmic_status)
{
	AUD_DMIC_RETURN_ON_NOT_INIT();
	*dmic_status = aud_hal_get_fifo_status_value();
	return BK_OK;
}

/* start adc to dac test */
bk_err_t bk_aud_dmic_start_loop_test(void)
{
	AUD_DMIC_RETURN_ON_NOT_INIT();
	aud_hal_set_fifo_config_loop_dmic2dac(1);
	return BK_OK;
}

/* stop adc to dac test */
bk_err_t bk_aud_dmic_stop_loop_test(void)
{
	AUD_DMIC_RETURN_ON_NOT_INIT();
	aud_hal_set_fifo_config_loop_dmic2dac(0);
	return BK_OK;
}

/* enable adc interrupt */
bk_err_t bk_aud_dmic_enable_int(void)
{
	AUD_DMIC_RETURN_ON_NOT_INIT();
//	sys_drv_aud_int_en(0);
	aud_hal_set_fifo_config_dmic_int_en(1);
//	sys_drv_aud_int_en(1);

	return BK_OK;
}

/* disable adc interrupt */
bk_err_t bk_aud_dmic_disable_int(void)
{
	AUD_DMIC_RETURN_ON_NOT_INIT();

	aud_hal_set_fifo_config_dmic_int_en(0);

	return BK_OK;
}

/* enable adc and adc start work */
bk_err_t bk_aud_dmic_start(void)
{
	AUD_DMIC_RETURN_ON_NOT_INIT();
	aud_hal_set_audio_config_dmic_enable(1);
	return BK_OK;
}

/* disable adc and adc stop work */
bk_err_t bk_aud_dmic_stop(void)
{
	AUD_DMIC_RETURN_ON_NOT_INIT();
	aud_hal_set_audio_config_dmic_enable(0);
	return BK_OK;
}

/* set adcl_wr_threshold */
bk_err_t bk_aud_dmic_set_dmic_wr_threshold(uint32_t value)
{
	AUD_DMIC_RETURN_ON_NOT_INIT();
	aud_hal_set_fifo_config_dmic_wr_threshold(value);
	return BK_OK;
}

/* register audio interrupt */
bk_err_t bk_aud_dmic_register_isr(aud_isr_t isr)
{
	AUD_DMIC_RETURN_ON_NOT_INIT();
	return bk_aud_register_aud_isr(AUD_ISR_DMIC, isr);
}

