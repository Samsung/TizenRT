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
#include "fft_hal.h"
#include "fft_driver.h"
#include "sys_driver.h"
#include "clock_driver.h"
#include <driver/fft_types.h>
#include <os/os.h>
#include <os/mem.h>
#include <driver/int.h>
#include <modules/pm.h>


#define FFT_RETURN_ON_NOT_INIT() do {\
		if (!s_fft_driver_is_init) {\
			return BK_ERR_AUD_NOT_INIT;\
		}\
	} while(0)

#define FFT_RETURN_ON_INVALID_ISR_ID(isr_id) do {\
		if ((isr_id) >= SOC_FFT_ISR_NUM) {\
			return BK_ERR_FFT_ISR_ID;\
		}\
	} while(0)


static bool s_fft_driver_is_init = false;
static fft_driver_t driver_fft;

static void fft_isr(void);
extern void delay(int num);//TODO fix me

static __inline uint16_t fft_sat(uint16_t din)
{
	if (din > 32767)
		return 32767;
	if (din < -32767)
		return -32767;
	else
		return (din);
}

bool bk_fft_is_busy(void)
{
	return driver_fft.busy_flag;
}

bk_err_t bk_fft_enable(fft_input_t *fft_conf)
{
	int i;

	fft_hal_fft_config_reset(0);
	fft_hal_fir_config_reset(0);

	if (fft_conf->mode == FFT_WORK_MODE_FFT) {
		fft_hal_fft_int_en(1);
		fft_hal_fft_en(1);
	} else {
		fft_hal_ifft_en(1);
		fft_hal_fft_int_en(1);
		fft_hal_fft_en(1);
	}

	driver_fft.size = fft_conf->size;
	driver_fft.busy_flag = true;

	for (i = 0; i < fft_conf->size; i++) {
		fft_hal_data_write(fft_conf->inbuf[i]);
	}

	fft_hal_start_trigger_set(1);
	//fft_struct_dump();

	return BK_OK;
}

bk_err_t bk_fft_fir_single_enable(fft_fir_input_t *fir_conf)
{
	int i;
	int32 temp_coef = 0;
	int32 temp_data = 0;

	if (fir_conf->fir_len > 256)
		return BK_ERR_FFT_PARAM;

	fft_hal_fft_config_reset(0);
	fft_hal_fir_config_reset(0);

	fft_hal_fir_length_set(fir_conf->fir_len);
	fft_hal_fir_mode_set(fir_conf->mode);
	fft_hal_fir_int_en(1);
	fft_hal_fir_en(1);

	driver_fft.size = fir_conf->fir_len;
	driver_fft.busy_flag = true;
	//FFT_LOGI("source data\r\n");

	if (fir_conf->mode) {
		for (i = 0; i < fir_conf->fir_len + 1; i++) {
			temp_coef = (fir_conf->coef_c1[i] << 16) | fir_conf->coef_c0[i];
			fft_hal_fir_coef_write(temp_coef);
			temp_data = (fir_conf->input_d1[i] << 16) | fir_conf->input_d0[i];
			fft_hal_data_write(temp_data);
			//FFT_LOGI("coef:0x%08x, data:0x%08x\r\n", temp_coef, temp_data);
		}
	} else {
		for (i = 0; i < fir_conf->fir_len + 1; i++) {
			temp_coef = (int32)fir_conf->coef_c0[i];
			fft_hal_fir_coef_write(temp_coef);
			temp_data = (int32)fir_conf->input_d0[i];
			fft_hal_data_write(temp_data);
			//FFT_LOGI("coef:0x%08x, data:0x%08x\r\n", temp_coef, temp_data);
		}
	}

	fft_hal_start_trigger_set(1);

	return BK_OK;
}

bk_err_t bk_fft_driver_init(void)
{
	if (s_fft_driver_is_init)
		return BK_OK;
	//power on
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_AUDP_FFT, PM_POWER_MODULE_STATE_ON);
	//sys_drv_aud_power_en(0);    //temp used

	//fft_disckg always on
	sys_drv_fft_disckg_set(1);

	os_memset(&driver_fft, 0, sizeof(driver_fft));
	//max number of FFT data
	driver_fft.i_out = os_malloc(256 * 2);
	driver_fft.q_out = os_malloc(256 * 2);

	//enable fft interrupt
	sys_drv_cpu_fft_int_en(1);
	//register fft isr
	fft_int_config_t int_config_table = {INT_SRC_FFT, fft_isr};
	bk_int_isr_register(int_config_table.int_src, int_config_table.isr, NULL);
	s_fft_driver_is_init = true;

	return BK_OK;
}

bk_err_t bk_fft_driver_deinit(void)
{
	//power down
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_AUDP_FFT, PM_POWER_MODULE_STATE_OFF);
	//fft_disckg not always on
	sys_drv_fft_disckg_set(0);
	//disable fft interrupt
	sys_drv_cpu_fft_int_en(0);

	os_free(driver_fft.i_out);
	os_free(driver_fft.q_out);
	os_memset(&driver_fft, 0, sizeof(driver_fft));

	fft_int_config_t int_config_table = {INT_SRC_FFT, fft_isr};
	bk_int_isr_unregister(int_config_table.int_src);
	s_fft_driver_is_init = false;

	return BK_OK;
}

bk_err_t bk_fft_output_read(int16_t *i_output, int16_t *q_output, uint32_t size)
{
	os_memcpy(i_output, driver_fft.i_out, size);
	os_memcpy(q_output, driver_fft.q_out, size);

	return BK_OK;
}

void fft_isr(void)
{
	int i;
	uint32_t bit_ext;
	int32 temp = 0;
	int32 temp_out = 0;
	int32 data = 0;
	int16 temp_low = 0, temp_high = 0;
	fft_status_t fft_status = {0};
	fft_hal_status_get(&fft_status);
	//fft_struct_dump();

	if (fft_status.fft_done) {
		bit_ext = (fft_status.bit_ext & 0x00001fff) >> 7;
		FFT_LOGI("bit_ext:0x%x\r\n", bit_ext);

		if (bit_ext & 0x20)
			bit_ext = 64 - bit_ext;
		else
			bit_ext = 0;

		for (i = 0; i < driver_fft.size; i++) {
			temp = fft_hal_data_read();
			temp_out = ((temp << 16) >> 16) << bit_ext;
			temp_low = fft_sat(temp_out);
			temp_out = (temp >> 16) << bit_ext;
			temp_high = fft_sat(temp_out);
			driver_fft.i_out[i]  = temp_low;
			driver_fft.q_out[i]  = temp_high;
			//FFT_LOGI("i:0x%04hx, q:0x%04hx\r\n", driver_fft.i_out[i], driver_fft.q_out[i]);
		}

		driver_fft.busy_flag = false;
		fft_hal_fft_config_reset(0);
		FFT_LOGI("\r\nexit isr \r\n");
	}

	if (fft_status.fir_done) {
		for (i = 0; i < driver_fft.size; i++) {
			data = fft_hal_data_read();
			driver_fft.i_out[i] = (int16)(data & 0x0000ffff);
			driver_fft.q_out[i] = (int16)((data & 0xffff0000) >> 16);
		}
		driver_fft.busy_flag = false;
		fft_hal_fft_config_reset(0);
	}
}

