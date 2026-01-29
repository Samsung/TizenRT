// Copyright 2023-2025 Beken
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
//#include <modules/pm.h>

#include <driver/aud_dac_types.h>
#include <driver/aud_dac.h>


#define TAG "aud_dac_drv"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)


#define AUD_DAC_RETURN_ON_NOT_INIT() do {\
				if (!bk_aud_get_module_init_sta(AUD_MODULE_DAC)) {\
					return BK_ERR_AUD_DAC_NOT_INIT;\
				}\
			} while(0)

#define CONST_DIV_16K		(0x06590000)
#define CONST_DIV_32K		(0x01964000)
#define CONST_DIV_44_1K		(0x049B2368)
#define CONST_DIV_48K		(0x043B5554)


extern void delay(int num);
bk_err_t bk_aud_dac_dacl_disable_int(void);
bk_err_t bk_aud_dac_dacr_disable_int(void);

bk_err_t bk_aud_dac_init(aud_dac_config_t *dac_config)
{
	bk_err_t ret = BK_OK;
	BK_RETURN_ON_NULL(dac_config);

	if (bk_aud_get_module_init_sta(AUD_MODULE_DAC)) {
		LOGW("aud dac is init already %s\n", __func__);
		return BK_OK;
	}

	bk_aud_set_module_init_sta(AUD_MODULE_DAC, true);

	/* audio common driver init */
	if (BK_OK != bk_aud_driver_init()) {
		LOGE("%s, audio driver init fail, line: %d \n", __func__, __LINE__);
		ret = BK_ERR_AUD_DRV_NOT_INIT;
		goto fail;
	}

	/* select audio clock */
	bk_aud_clk_config(dac_config->clk_src);

	/*active dac*/
	sys_drv_aud_dac_bias_en(1);
	sys_drv_aud_dac_drv_en(1);
	sys_drv_aud_dac_dcoc_en(1);
	sys_drv_aud_dac_idac_en(1);

	//enable dacl and dacr
	switch (dac_config->dac_chl) {
		case AUD_DAC_CHL_L:
			sys_drv_aud_dacr_en(0);
			sys_drv_aud_dacl_en(1);
			break;

		case AUD_DAC_CHL_R:
			sys_drv_aud_dacr_en(1);
			sys_drv_aud_dacl_en(0);
			break;

		case AUD_DAC_CHL_LR:
			sys_drv_aud_dacr_en(1);
			sys_drv_aud_dacl_en(1);
			break;

		default:
			break;
	}

	//set dac work mode
	if (dac_config->work_mode == AUD_DAC_WORK_MODE_SIGNAL_END) {
		sys_drv_aud_diffen_en(0);
	} else if (dac_config->work_mode == AUD_DAC_WORK_MODE_DIFFEN) {
		sys_drv_aud_diffen_en(1);
	} else {
		LOGE("%s, audio dac work mode fail, line: %d \n", __func__, __LINE__);
		ret = BK_FAIL;
		goto fail;
	}

	aud_hal_set_dac_config0_dac_set_gain(dac_config->dac_gain);
	aud_hal_set_dac_config0_dac_clk_invert(dac_config->dac_clk_invert);

	aud_hal_set_dac_config0_dac_hpf1_bypass(1);
	aud_hal_set_dac_config0_dac_hpf2_bypass(1);

#if defined(CONFIG_SOC_BK7256XX)
	aud_hal_set_fifo_config_dacl_rd_threshold(4);
#endif


	if (BK_OK != bk_aud_dac_set_samp_rate(dac_config->samp_rate)) {
		ret = BK_FAIL;
		goto fail;
	}

	return ret;

fail:
	bk_aud_set_module_init_sta(AUD_MODULE_DAC, false);
	return ret;
}

bk_err_t bk_aud_dac_deinit(void)
{
	AUD_DAC_RETURN_ON_NOT_INIT();

	bk_aud_dac_stop();

	/* disable int */
	bk_aud_dac_dacl_disable_int();
	bk_aud_dac_dacr_disable_int();

	//enable dacl and dacr
	sys_drv_aud_dacr_en(0);
	sys_drv_aud_dacl_en(0);

	/*active dac*/
	sys_drv_aud_dac_bias_en(0);
	sys_drv_aud_dac_drv_en(0);
	sys_drv_aud_dac_dcoc_en(0);
	sys_drv_aud_dac_idac_en(0);

	sys_drv_aud_diffen_en(1);

	aud_hal_set_dac_config0_dac_set_gain(0);
	aud_hal_set_dac_config0_dac_clk_invert(0);

	aud_hal_set_dac_config0_dac_hpf1_bypass(0);
	aud_hal_set_dac_config0_dac_hpf2_bypass(0);

	bk_aud_dac_set_samp_rate(8000);

	bk_err_t ret = bk_aud_set_module_init_sta(AUD_MODULE_DAC, false);

	bk_aud_driver_deinit();

	return ret;
}

bk_err_t bk_aud_dac_set_samp_rate(uint32_t samp_rate)
{
	AUD_DAC_RETURN_ON_NOT_INIT();

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
	aud_hal_set_extend_cfg_dac_fracmod_manual(0x0);

	if (samp_rate == 8000) {
		aud_hal_set_audio_config_samp_rate_dac(0x0);
	} else if (samp_rate == 110250) {
		aud_hal_set_extend_cfg_dac_fracmod_manual(0x1);
		aud_hal_set_dac_fracmod_value(CONST_DIV_44_1K << 2);
		aud_hal_set_audio_config_samp_rate_dac(0x2);
	} else if (samp_rate == 12000) {
		aud_hal_set_extend_cfg_dac_fracmod_manual(0x1);
		aud_hal_set_dac_fracmod_value(CONST_DIV_48K << 2);
		aud_hal_set_audio_config_samp_rate_dac(0x3);
	} else if (samp_rate == 16000) {
		aud_hal_set_audio_config_samp_rate_dac(0x1);
	} else if (samp_rate == 22050) {
		aud_hal_set_extend_cfg_dac_fracmod_manual(0x1);
		aud_hal_set_dac_fracmod_value(CONST_DIV_44_1K << 1);
		aud_hal_set_audio_config_samp_rate_dac(0x2);
	} else if (samp_rate == 24000) {
		aud_hal_set_extend_cfg_dac_fracmod_manual(0x1);
		aud_hal_set_dac_fracmod_value(CONST_DIV_48K << 1);
		aud_hal_set_audio_config_samp_rate_dac(0x3);
	} else if (samp_rate == 32000) {
		aud_hal_set_extend_cfg_dac_fracmod_manual(0x1);
		aud_hal_set_dac_fracmod_value(CONST_DIV_16K >> 1);
		aud_hal_set_audio_config_samp_rate_dac(0x1);
	} else if (samp_rate == 44100) {
		aud_hal_set_audio_config_samp_rate_dac(0x2);
	} else if (samp_rate == 48000) {
		aud_hal_set_audio_config_samp_rate_dac(0x3);
	} else {
		LOGE("%s, not support sample rate: %d, line: %d \n", __func__, samp_rate, __LINE__);
		return BK_FAIL;
	}

	return BK_OK;
}

bk_err_t bk_aud_dac_set_gain(uint32_t value)
{
	AUD_DAC_RETURN_ON_NOT_INIT();
	aud_hal_set_dac_config0_dac_set_gain(value);
	return BK_OK;
}

bk_err_t bk_aud_dac_mute(void)
{
	AUD_DAC_RETURN_ON_NOT_INIT();
	//aud_hal_set_dac_config0_dac_set_gain(value);
	return BK_OK;
}

bk_err_t bk_aud_dac_unmute(void)
{
	AUD_DAC_RETURN_ON_NOT_INIT();
	//aud_hal_set_dac_config0_dac_set_gain(value);
	return BK_OK;
}

bk_err_t bk_aud_dac_set_chl(aud_dac_chl_t dac_chl)
{
	AUD_DAC_RETURN_ON_NOT_INIT();
	switch (dac_chl) {
		case AUD_DAC_CHL_L:
			sys_drv_aud_dacr_en(0);
			sys_drv_aud_dacl_en(1);
			break;

		case AUD_DAC_CHL_R:
			sys_drv_aud_dacr_en(1);
			sys_drv_aud_dacl_en(0);
			break;

		case AUD_DAC_CHL_LR:
			sys_drv_aud_dacr_en(1);
			sys_drv_aud_dacl_en(1);
			break;

		default:
			LOGE("%s, not support dac channel: %d, line: %d \n", __func__, dac_chl, __LINE__);
			return BK_FAIL;
			break;
	}

	return BK_OK;
}

bk_err_t bk_aud_dac_write(uint32_t pcm_value)
{
	AUD_DAC_RETURN_ON_NOT_INIT();
	aud_hal_set_dac_fport_value(pcm_value);

	return BK_OK;
}

bk_err_t bk_aud_dac_set_dacl_read_threshold(uint16_t dacl_throld)
{
	AUD_DAC_RETURN_ON_NOT_INIT();
	aud_hal_set_fifo_config_dacl_rd_threshold(dacl_throld);

	return BK_OK;
}

bk_err_t bk_aud_dac_set_dacr_read_threshold(uint16_t dacr_throld)
{
	AUD_DAC_RETURN_ON_NOT_INIT();
	aud_hal_set_fifo_config_dacr_rd_threshold(dacr_throld);

	return BK_OK;
}

bk_err_t bk_aud_dac_dacl_enable_int(void)
{
	AUD_DAC_RETURN_ON_NOT_INIT();
//	sys_drv_aud_int_en(1);
	aud_hal_set_fifo_config_dacl_int_en(1);

	return BK_OK;
}

bk_err_t bk_aud_dac_dacr_enable_int(void)
{
	AUD_DAC_RETURN_ON_NOT_INIT();
//	sys_drv_aud_int_en(1);
	aud_hal_set_fifo_config_dacr_int_en(1);

	return BK_OK;
}

bk_err_t bk_aud_dac_dacl_disable_int(void)
{
	AUD_DAC_RETURN_ON_NOT_INIT();
	aud_hal_set_fifo_config_dacl_int_en(0);

	return BK_OK;
}

bk_err_t bk_aud_dac_dacr_disable_int(void)
{
	AUD_DAC_RETURN_ON_NOT_INIT();
	aud_hal_set_fifo_config_dacr_int_en(0);

	return BK_OK;
}

/* get dac fifo port address */
bk_err_t bk_aud_dac_get_fifo_addr(uint32_t *dac_fifo_addr)
{
	aud_hal_dac_get_fifo_addr(dac_fifo_addr);
	return BK_OK;
}

bk_err_t bk_aud_dac_get_status(uint32_t *dac_status)
{
	AUD_DAC_RETURN_ON_NOT_INIT();

	*dac_status = aud_hal_get_fifo_status_value();

	return BK_OK;
}

bk_err_t bk_aud_dac_start(void)
{
	AUD_DAC_RETURN_ON_NOT_INIT();
	aud_hal_set_audio_config_dac_enable(1);

	return BK_OK;
}

bk_err_t bk_aud_dac_stop(void)
{
	AUD_DAC_RETURN_ON_NOT_INIT();
	aud_hal_set_audio_config_dac_enable(0);

	return BK_OK;
}

bk_err_t bk_aud_dac_eq_config(aud_dac_eq_config_t *config)
{
	AUD_DAC_RETURN_ON_NOT_INIT();
	BK_RETURN_ON_NULL(config);

	aud_hal_dac_filt_config(config);
	aud_hal_set_extend_cfg_filt_enable(1);

	return BK_OK;
}

bk_err_t bk_aud_dac_eq_deconfig(void)
{
	AUD_DAC_RETURN_ON_NOT_INIT();

	aud_dac_eq_config_t config;
	aud_hal_set_extend_cfg_filt_enable(0);
	os_memset((void *)&config, 0, sizeof(aud_dac_eq_config_t));
	aud_hal_dac_filt_config(&config);

	return BK_OK;
}

/* register audio interrupt */
bk_err_t bk_aud_dac_register_isr(aud_isr_id_t isr_id, aud_isr_t isr)
{
	AUD_DAC_RETURN_ON_NOT_INIT();

	return bk_aud_register_aud_isr(isr_id, isr);
}

/* set audio dac analog gain */
bk_err_t bk_aud_dac_set_ana_gain(uint8_t value)
{
	AUD_DAC_RETURN_ON_NOT_INIT();

	return sys_drv_aud_dacg_set(value);
}

/* get audio dac analog gain */
bk_err_t bk_aud_dac_get_ana_gain(uint32_t *gain)
{
	AUD_DAC_RETURN_ON_NOT_INIT();

	*gain = sys_drv_aud_dacg_get();
	return BK_OK;
}

/* set bypass audio dac dwa */
bk_err_t bk_aud_dac_set_dwa_bypass(uint8_t value)
{
	AUD_DAC_RETURN_ON_NOT_INIT();

	return sys_drv_aud_dac_bypass_dwa_en(value);
}

