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

#include <driver/aud_dtmf_types.h>
#include <driver/aud_dtmf.h>

#define TAG "aud_dtmf_drv"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)


#define AUD_DTMF_RETURN_ON_NOT_INIT() do {\
				if (!bk_aud_get_module_init_sta(AUD_MODULE_DTMF)) {\
					return BK_ERR_AUD_DTMF_NOT_INIT;\
				}\
			} while(0)

#define BK_RETURN_ON_NULL(_x) do {\
	if (!(_x)) {\
		BK_LOGE(ERR_TAG, "Null %s\n", __FUNCTION__);\
		return BK_ERR_NULL_PARAM;\
	}\
} while(0)


bk_err_t bk_aud_dtmf_init(aud_dtmf_config_t *dtmf_config)
{
	BK_RETURN_ON_NULL(dtmf_config);

	if (bk_aud_get_module_init_sta(AUD_MODULE_DTMF)) {
		LOGW("aud dtmf is init already %s\n", __func__);
		return BK_OK;
	}

	/* audio common driver init */
	if (BK_OK != bk_aud_driver_init()) {
		LOGE("%s, audio driver init fail, line: %d \n", __func__, __LINE__);
		return BK_ERR_AUD_DRV_NOT_INIT;
	}

	/* select audio clock */
	bk_aud_clk_config(dtmf_config->clk_src);

	/* config dtmf */
	aud_hal_dtmf_config(dtmf_config);

	bk_aud_set_module_init_sta(AUD_MODULE_DTMF, true);

	return BK_OK;
}

bk_err_t bk_aud_dtmf_deinit(void)
{
	AUD_DTMF_RETURN_ON_NOT_INIT();

	aud_dtmf_config_t config;
	os_memset((void *)(&config), 0, sizeof(aud_dtmf_config_t));

	aud_hal_dtmf_config(&config);

	bk_aud_set_module_init_sta(AUD_MODULE_DTMF, false);

	return BK_OK;
}

/* get dtmf fifo port address */
bk_err_t bk_aud_dtmf_get_fifo_addr(uint32_t *dtmf_fifo_addr)
{
	//*dtmf_fifo_addr = AUD_DTMF_FPORT_ADDR;
	aud_hal_dtmf_get_fifo_addr(dtmf_fifo_addr);
	return BK_OK;
}

/* get dtmf fifo data */
bk_err_t bk_aud_dtmf_get_fifo_data(uint32_t *dtmf_data)
{
	AUD_DTMF_RETURN_ON_NOT_INIT();
	*dtmf_data = aud_hal_get_dtmf_fport_value();
	return BK_OK;
}

/* get audio dtmf fifo status */
bk_err_t bk_aud_dtmf_get_status(uint32_t *dtmf_status)
{
	AUD_DTMF_RETURN_ON_NOT_INIT();

	*dtmf_status = aud_hal_get_fifo_status_value();
	return BK_OK;
}

/* start adc to dac test */
bk_err_t bk_aud_dtmf_start_loop_test(void)
{
	AUD_DTMF_RETURN_ON_NOT_INIT();

	aud_hal_set_fifo_config_loop_ton2dac(1);

	return BK_OK;
}

/* stop adc to dac test */
bk_err_t bk_aud_dtmf_stop_loop_test(void)
{
	AUD_DTMF_RETURN_ON_NOT_INIT();

	aud_hal_set_fifo_config_loop_ton2dac(0);

	return BK_OK;
}

/* enable adc interrupt */
bk_err_t bk_aud_dtmf_enable_int(void)
{
	AUD_DTMF_RETURN_ON_NOT_INIT();
//	sys_drv_aud_int_en(0);

	aud_hal_set_fifo_config_dtmf_int_en(1);

//	sys_drv_aud_int_en(1);

	return BK_OK;
}

/* disable adc interrupt */
bk_err_t bk_aud_dtmf_disable_int(void)
{
	AUD_DTMF_RETURN_ON_NOT_INIT();

	aud_hal_set_fifo_config_dtmf_int_en(0);

	return BK_OK;
}

/* enable adc and adc start work */
bk_err_t bk_aud_dtmf_start(void)
{
	AUD_DTMF_RETURN_ON_NOT_INIT();
	aud_hal_set_audio_config_dtmf_enable(1);

	aud_hal_set_dtmf_config1_tone1_enable(1);
	aud_hal_set_dtmf_config2_tone2_enable(1);

	return BK_OK;
}

/* disable adc and adc stop work */
bk_err_t bk_aud_dtmf_stop(void)
{
	AUD_DTMF_RETURN_ON_NOT_INIT();
	aud_hal_set_audio_config_dtmf_enable(0);

	aud_hal_set_dtmf_config1_tone1_enable(0);
	aud_hal_set_dtmf_config2_tone2_enable(0);

	return BK_OK;
}

/* set adcl_wr_threshold */
bk_err_t bk_aud_dtmf_set_dtmf_wr_threshold(uint32_t value)
{
	AUD_DTMF_RETURN_ON_NOT_INIT();

	aud_hal_set_fifo_config_dtmf_wr_threshold(value);

	return BK_OK;
}

/* register audio interrupt */
bk_err_t bk_aud_dtmf_register_isr(aud_isr_t isr)
{
	AUD_DTMF_RETURN_ON_NOT_INIT();
	return bk_aud_register_aud_isr(AUD_ISR_DTMF, isr);
}


