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
#include "aud_hal.h"
#include "sys_driver.h"
#include "clock_driver.h"
#include <os/os.h>
#include <os/mem.h>
#include <driver/int.h>

#include <driver/aud_adc.h>
#include <driver/aud_adc_types.h>



#define TAG "aud_adc_drv"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)


#define AUD_ADC_RETURN_ON_NOT_INIT() do {\
		if (!bk_aud_get_module_init_sta(AUD_MODULE_ADC)) {\
			return BK_ERR_AUD_ADC_NOT_INIT;\
		}\
	} while(0)

#define CONST_DIV_16K		(0x06590000)
#define CONST_DIV_32K		(0x01964000)
#define CONST_DIV_44_1K		(0x049B2368)
#define CONST_DIV_48K		(0x043B5554)


extern void delay(int num);//TODO fix me


bk_err_t bk_aud_adc_init(aud_adc_config_t *adc_config)
{
	bk_err_t ret = BK_OK;
	BK_RETURN_ON_NULL(adc_config);

	if (bk_aud_get_module_init_sta(AUD_MODULE_ADC)) {
		LOGW("aud adc is init already %s\n", __func__);
		ret = BK_FAIL;
		goto fail;
	}

	bk_aud_set_module_init_sta(AUD_MODULE_ADC, true);

	/* audio common driver init */
	if (BK_OK != bk_aud_driver_init()) {
		LOGE("%s, audio driver init fail, line: %d \n", __func__, __LINE__);
		return BK_ERR_AUD_DRV_NOT_INIT;
	}

	/* select audio clock */
	bk_aud_clk_config(adc_config->clk_src);

//	sys_drv_analog_reg12_set(0x81B0E0E0);

	//enable mic1 and mic2
	switch (adc_config->adc_chl) {
		case AUD_ADC_CHL_L:
			sys_drv_aud_mic1_en(1);
			break;

		case AUD_ADC_CHL_R:
			sys_drv_aud_mic2_en(1);
			break;

		case AUD_ADC_CHL_LR:
			sys_drv_aud_mic1_en(1);
			sys_drv_aud_mic2_en(1);
			break;

		default:
			break;
	}

	//enable audio adc power
	sys_drv_aud_aud_en(1);
	//enable audio adc power
	sys_drv_aud_audbias_en(1);
	sys_drv_aud_adcbias_en(1);
	sys_drv_aud_micbias_en(1);

	//reset mic after configuring parameters
	sys_drv_aud_mic_rst_set(1);
	delay(10);
	sys_drv_aud_mic_rst_set(0);

	aud_hal_set_adc_config0_adc_set_gain(adc_config->adc_gain);
	aud_hal_set_adc_config0_adc_sampe_edge(adc_config->adc_samp_edge);

	aud_hal_set_adc_config0_adc_hpf1_bypass(1);
	aud_hal_set_adc_config0_adc_hpf2_bypass(1);

	LOGI("configure mic and adc\r\n");

	if (BK_OK != bk_aud_adc_set_samp_rate(adc_config->samp_rate)) {
		ret = BK_FAIL;
		goto fail;
	}

	return ret;

fail:
	bk_aud_set_module_init_sta(AUD_MODULE_ADC, false);
	return ret;
}

bk_err_t bk_aud_adc_deinit(void)
{
	AUD_ADC_RETURN_ON_NOT_INIT();

	bk_aud_adc_stop();

	//enable mic1 and mic2
	sys_drv_aud_mic1_en(0);
	sys_drv_aud_mic2_en(0);

	//disable audio adc power
	sys_drv_aud_aud_en(0);
	//disable audio adc power
	sys_drv_aud_audbias_en(0);
	sys_drv_aud_adcbias_en(0);
	sys_drv_aud_micbias_en(0);

	//config system registers about audio adc to default value
	aud_hal_set_adc_config0_adc_set_gain(0);
	aud_hal_set_adc_config0_adc_sampe_edge(0);

	aud_hal_set_adc_config0_adc_hpf1_bypass(0);
	aud_hal_set_adc_config0_adc_hpf2_bypass(0);

	bk_aud_adc_set_samp_rate(8000);

	bk_err_t ret = bk_aud_set_module_init_sta(AUD_MODULE_ADC, false);

	bk_aud_driver_deinit();

	return ret;
}

bk_err_t bk_aud_adc_set_samp_rate(uint32_t samp_rate)
{
	AUD_ADC_RETURN_ON_NOT_INIT();

#if defined(CONFIG_SOC_BK7236XX)
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
#endif

	/* disable dac handset bit again, to make sure this bit unset */
	aud_hal_set_extend_cfg_adc_fracmod_manual(0x0);

	switch(samp_rate)
	{
		case 8000:
			aud_hal_set_audio_config_samp_rate_adc(0x0);
			break;

		case 16000:
			aud_hal_set_audio_config_samp_rate_adc(0x1);
			break;

		case 44100:
			aud_hal_set_audio_config_samp_rate_adc(0x2);
			break;

		case 48000:
			aud_hal_set_audio_config_samp_rate_adc(0x3);
			break;

		case 11025:
			aud_hal_set_extend_cfg_adc_fracmod_manual(0x1);
			aud_hal_set_adc_fracmod_value(CONST_DIV_44_1K);
			aud_hal_set_audio_config_samp_rate_adc(0x2);
			break;

		case 12000:
			aud_hal_set_extend_cfg_adc_fracmod_manual(0x1);
			aud_hal_set_adc_fracmod_value(CONST_DIV_48K);
			aud_hal_set_audio_config_samp_rate_adc(0x3);
			break;

		case 22050:
			aud_hal_set_extend_cfg_adc_fracmod_manual(0x1);
			aud_hal_set_adc_fracmod_value(CONST_DIV_44_1K >> 1);
			aud_hal_set_audio_config_samp_rate_adc(0x2);
			break;

		case 24000:
			aud_hal_set_extend_cfg_adc_fracmod_manual(0x1);
			aud_hal_set_adc_fracmod_value(CONST_DIV_48K >> 1);
			aud_hal_set_audio_config_samp_rate_adc(0x3);
			break;

		case 32000:
			aud_hal_set_extend_cfg_adc_fracmod_manual(0x1);
			aud_hal_set_adc_fracmod_value(CONST_DIV_32K);
			aud_hal_set_audio_config_samp_rate_adc(0x3);
			break;

		default:
			LOGE("%s, not support sample rate: %d, line: %d \n", __func__, samp_rate, __LINE__);
			return BK_FAIL;
			break;
	}

	return BK_OK;
}

bk_err_t bk_aud_adc_set_gain(uint32_t value)
{
	AUD_ADC_RETURN_ON_NOT_INIT();
	aud_hal_set_adc_config0_adc_set_gain(value);
	return BK_OK;
}

bk_err_t bk_aud_adc_set_chl(aud_adc_chl_t chl)
{
	AUD_ADC_RETURN_ON_NOT_INIT();

	switch (chl) {
		case AUD_ADC_CHL_LR:
//			sys_drv_analog_reg14_set(0x40038002);
//			sys_drv_analog_reg15_set(0x40038002);
			sys_drv_aud_mic1_en(1);
			sys_drv_aud_mic2_en(1);
			break;

		case AUD_ADC_CHL_L:
//			sys_drv_analog_reg14_set(0x40038002);
//			sys_drv_analog_reg15_set(0x0);
			sys_drv_aud_mic1_en(1);
			break;

		case AUD_ADC_CHL_R:
//			sys_drv_analog_reg14_set(0x0);
//			sys_drv_analog_reg15_set(0x40038002);
			sys_drv_aud_mic2_en(1);
			break;

		default:
			break;
	}

	return BK_OK;
}

bk_err_t bk_aud_adc_set_mic_mode(aud_mic_id_t mic_id, aud_adc_mode_t mode)
{
	AUD_ADC_RETURN_ON_NOT_INIT();

	uint32_t intf_value = 0;

	if (mode == AUD_ADC_MODE_SIGNAL_END) {
		intf_value = 1;
	} else {
		intf_value = 0;
	}

	switch (mic_id) {
		case AUD_MIC_MIC1:
			sys_drv_aud_mic1_single_en(intf_value);
			break;

		case AUD_MIC_MIC2:
			sys_drv_aud_mic2_single_en(intf_value);
			break;

		case AUD_MIC_BOTH:
			sys_drv_aud_mic1_single_en(intf_value);
			sys_drv_aud_mic2_single_en(intf_value);
			break;

		default:
			break;
	}

	return BK_OK;
}

/* get adc fifo port address */
bk_err_t bk_aud_adc_get_fifo_addr(uint32_t *adc_fifo_addr)
{
	aud_hal_adc_get_fifo_addr(adc_fifo_addr);
	return BK_OK;
}

/* get adc fifo data */
bk_err_t bk_aud_adc_get_fifo_data(uint32_t *adc_data)
{
	AUD_ADC_RETURN_ON_NOT_INIT();
	*adc_data = aud_hal_get_adc_fport_value();
	return BK_OK;
}

/* get audio adc fifo and agc status */
bk_err_t bk_aud_adc_get_status(uint32_t *adc_status)
{
	AUD_ADC_RETURN_ON_NOT_INIT();

	*adc_status = aud_hal_get_fifo_status_value();
	return BK_OK;
}

/* start adc to dac test */
bk_err_t bk_aud_adc_start_loop_test(void)
{
	AUD_ADC_RETURN_ON_NOT_INIT();
	aud_hal_set_fifo_config_loop_adc2dac(1);

	return BK_OK;
}

/* stop adc to dac test */
bk_err_t bk_aud_adc_stop_loop_test(void)
{
	AUD_ADC_RETURN_ON_NOT_INIT();
	aud_hal_set_fifo_config_loop_adc2dac(0);
	return BK_OK;
}

/* enable adc interrupt */
bk_err_t bk_aud_adc_enable_int(void)
{
	AUD_ADC_RETURN_ON_NOT_INIT();
//	sys_drv_aud_int_en(0);

//	aud_hal_adc_int_enable();
	aud_hal_set_fifo_config_adcl_int_en(1);

//	sys_drv_aud_int_en(1);

	return BK_OK;
}

/* disable adc interrupt */
bk_err_t bk_aud_adc_disable_int(void)
{
	AUD_ADC_RETURN_ON_NOT_INIT();
//	aud_hal_adc_int_disable();
	aud_hal_set_fifo_config_adcl_int_en(0);

	return BK_OK;
}

/* enable adc and adc start work */
bk_err_t bk_aud_adc_start(void)
{
	AUD_ADC_RETURN_ON_NOT_INIT();
	aud_hal_set_audio_config_adc_enable(1);
	aud_hal_set_audio_config_line_enable(1);

	return BK_OK;
}

/* disable adc and adc stop work */
bk_err_t bk_aud_adc_stop(void)
{
	AUD_ADC_RETURN_ON_NOT_INIT();
	aud_hal_set_audio_config_adc_enable(0);
	aud_hal_set_audio_config_line_enable(0);

	return BK_OK;
}

/* set adcl_wr_threshold */
bk_err_t bk_aud_adc_set_adcl_wr_threshold(uint32_t value)
{
	AUD_ADC_RETURN_ON_NOT_INIT();
	aud_hal_set_fifo_config_adcl_wr_threshold(value);

	return BK_OK;
}

/* register audio interrupt */
bk_err_t bk_aud_adc_register_isr(aud_isr_t isr)
{
	AUD_ADC_RETURN_ON_NOT_INIT();
	return bk_aud_register_aud_isr(AUD_ISR_ADCL, isr);
}

/* hpf config */
bk_err_t bk_aud_adc_hpf_config(aud_adc_hpf_config_t *config)
{
	AUD_ADC_RETURN_ON_NOT_INIT();
	return aud_hal_adc_hpf_config(config);
}

/* agc config */
bk_err_t bk_aud_adc_agc_config(aud_adc_agc_config_t *config)
{
	AUD_ADC_RETURN_ON_NOT_INIT();
	return aud_hal_adc_agc_config(config);
}

