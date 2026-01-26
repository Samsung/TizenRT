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
#include "bk_arm_arch.h"
#include "bk_misc.h"
#include <os/mem.h>
#include "bk_drv_model.h"
#include "bk_sys_ctrl.h"
#include "bk_saradc.h"
#include "bk_uart.h"
#include "sys_rtos.h"
#include <os/os.h>
#include <common/bk_kernel_err.h>
#include "bk_phy.h"
#include "temp_detect.h"
#include "volt_detect.h"
#include "bk_ps.h"
#include "bk_wifi_private.h"
#include <components/log.h>
#include <components/ate.h>

#if CONFIG_SDMADC_TEMP
#include <driver/sdmadc.h>
#else
#include <driver/adc.h>
#endif

#include "drv_model.h"
#include "sys_driver.h"

#if CONFIG_FLASH_ORIGIN_API
#include "flash.h"
#endif

#include <modules/pm.h>
#include "components/sensor.h"

#define VOLT_FIFO_MAX    3

typedef struct {
	uint16_t last_detect_val;
	uint16_t detect_interval;
	uint16_t detect_threshold;
	uint16_t inital_data;
	uint32_t detect_cnt;
#if TEMP_DETECT_ONESHOT_TIMER
    beken2_timer_t detect_oneshot_timer;
#else
	beken_timer_t detect_timer;
#endif
	float         volt_last;
	float         volt_fifo[VOLT_FIFO_MAX]; //for smoothing
} volt_detect_config_t;

volt_detect_config_t s_voltd;
static uint16_t *s_raw_voltage_data = NULL;

static int _volt_detect_init_adc_buffer(void)
{
	if (!s_raw_voltage_data) {
		s_raw_voltage_data = (uint16_t *)os_zalloc(ADC_TEMP_BUFFER_SIZE *
					sizeof(s_raw_voltage_data[0]));

		if (!s_raw_voltage_data) {
			TEMPD_LOGE("oom\r\n");
			return BK_ERR_NO_MEM;
		}
	}

	return BK_OK;
}

#if CONFIG_SDMADC_TEMP
static int _volt_detect_get_adc_data(sdmadc_chan_t adc_chan, uint16_t *adc_buf)
{
	sdmadc_config_t config = {0};
	int err = BK_OK;

	err = bk_sdmadc_driver_init();
	if (BK_OK != err)
		goto _release_adc;

	err = bk_sdmadc_init();
	if (BK_OK != err)
		goto _release_adc;

	config.samp_mode = 0x0;
	config.samp_numb = 0x0;
	config.samp_chan = adc_chan;
	config.comp_bpss = 0x1;
	config.cic2_bpss = 0x1;
	config.cic2_gain = 0x2d;
	config.int_enable = 0x8;
	config.cali_offset = 0x0;
	config.cali_gains  = 0x1000;

	err = bk_sdmadc_set_cfg(&config);
	if (BK_OK != err)
		goto _release_adc;

	err = bk_sdmadc_read_raw(adc_buf, ADC_TEMP_BUFFER_SIZE);

_release_adc:
	bk_sdmadc_deinit();
	bk_sdmadc_driver_deinit();

	return err;
}
#else
static int _volt_detect_get_adc_data(adc_chan_t adc_chan, uint16_t *adc_buf)
{
	adc_config_t config = {0};
	int err = BK_OK;

	BK_RETURN_ON_ERR(bk_adc_acquire());

	err = bk_adc_init(adc_chan);
	if (BK_OK != err)
		goto _release_adc;

	config.chan = adc_chan;
	config.adc_mode = ADC_CONTINUOUS_MODE;
	config.clk = TEMP_DETEC_ADC_CLK;

#if CONFIG_SARADC_V1P2
	config.src_clk = ADC_SCLK_XTAL;
#elif CONFIG_SARADC_V1P1
	config.src_clk = ADC_SCLK_XTAL_26M;
#endif

	config.saturate_mode = ADC_TEMP_SATURATE_MODE;
	config.sample_rate = TEMP_DETEC_ADC_SAMPLE_RATE;
	config.steady_ctrl= TEMP_DETEC_ADC_STEADY_CTRL;
	config.adc_filter = 0;

	err = bk_adc_set_config(&config);
	if (BK_OK != err)
		goto _release_adc;

	err = bk_adc_enable_bypass_clalibration();
	if (BK_OK != err)
		goto _release_adc;

	err = bk_adc_start();
	if (BK_OK != err)
		goto _release_adc;

	err = bk_adc_read_raw(adc_buf, ADC_TEMP_BUFFER_SIZE,
				ADC_READ_SEMAPHORE_WAIT_TIME);
	if (BK_OK != err) {
		err = BK_ERR_TEMPD_SAMPLE_NO_DATA;
		goto _release_adc;
	}

_release_adc:
    bk_adc_stop();
	bk_adc_deinit(adc_chan);
	bk_adc_release();
	return err;
}
#endif

static uint16_t _volt_detect_calculate_voltage(uint16_t *raw_voltage_data)
{
#if (CONFIG_SOC_BK7231N) || (CONFIG_SOC_BK7236A) || (CONFIG_SOC_BK7236XX) || (CONFIG_SOC_BK7239XX) || (CONFIG_SOC_BK7286XX) || (CONFIG_SOC_BK7236XX)
	uint32_t sum = 0, index, count = 0;

	for (index = 5; index < ADC_TEMP_BUFFER_SIZE; index++) {
		/* 0 is invalid, but saradc may return 0 in power save mode */
		if ((0 != raw_voltage_data[index]) && (2048 != raw_voltage_data[index])) {
			#if CONFIG_SDMADC_TEMP
			sum += (int16_t)raw_voltage_data[index] + CFG_SDMADC_OFFSET; //offset half of 2^16 to be positive
			#else
			sum += raw_voltage_data[index];
			#endif
			count++;
		}
	}

	if (count == 0)
		raw_voltage_data[0] = 0;
	else {
		sum = sum / count;
		raw_voltage_data[0] = sum;
	}
#elif (CONFIG_SOC_BK7256XX)
	uint32_t sum = 0, sum1, sum2;

	sum1 = raw_voltage_data[1] + raw_voltage_data[2];
	sum2 = raw_voltage_data[3] + raw_voltage_data[4];
	sum = sum1 / 2 + sum2 / 2;
	sum = sum / 2;
	raw_voltage_data[0] = sum;

#else
	uint32_t sum = 0, sum1, sum2;

	sum1 = raw_voltage_data[1] + raw_voltage_data[2];
	sum2 = raw_voltage_data[3] + raw_voltage_data[4];
	sum = sum1 / 2 + sum2 / 2;
	sum = sum / 2;
	raw_voltage_data[0] = sum;
#endif

	return raw_voltage_data[0];
}

#if CONFIG_VOLT_DETECT
#if TEMP_DETECT_ONESHOT_TIMER
__IRAM_SEC static void _volt_detect_oneshot_timer_handler(void *data1, void *data2)
{
	int result;

    result = bk_sensor_send_msg(VOLT_TIMER_EXPIRED);
    if (result != BK_OK) {
		result = bk_sensor_send_msg(VOLT_RESTART_TIMER);
	    if (result != BK_OK) {
			volt_daemon_restart();
	    }
    }
}
#else
static void _volt_detect_timer_handler(void *data)
{
	bk_sensor_send_msg(VOLT_TIMER_EXPIRED);
}
#endif

static float volt_detect_calc_voltage(uint16_t adc_code)
{
	float volt_code;
	int index_valid = 0;
	int index;

	//1. calc volt with adc
	volt_code = bk_adc_data_calculate(adc_code, ADC_VOLT_SENSER_CHANNEL);
	//2. update FIFO
	for (index = 0; index < VOLT_FIFO_MAX - 1; index++) {
		s_voltd.volt_fifo[index] = s_voltd.volt_fifo[index + 1];
		if (s_voltd.volt_fifo[index] < 1.0) {
			index_valid = index + 1;
		}
	}
	s_voltd.volt_fifo[index] = volt_code;
	//3. average volt with FIFO
	for (index = index_valid; index < VOLT_FIFO_MAX - 1; index++) {
		volt_code += s_voltd.volt_fifo[index];
	}
	volt_code /= (VOLT_FIFO_MAX - index_valid);

    return volt_code;
}

__IRAM_SEC int volt_daemon_restart(void)
{
	int err;

#if TEMP_DETECT_ONESHOT_TIMER
    err = rtos_oneshot_reload_timer(&s_voltd.detect_oneshot_timer);
#else
    err = rtos_reload_timer(&s_voltd.detect_timer);
#endif
	TEMPD_LOGD("restart err=%d\n", err);

	return err;
}

int volt_daemon_stop(void)
{
	int err;

#if TEMP_DETECT_ONESHOT_TIMER
    err = rtos_stop_oneshot_timer(&s_voltd.detect_oneshot_timer);
#else
    err = rtos_stop_timer(&s_voltd.detect_timer);
#endif

	TEMPD_LOGD("stop err=%d\n", err);

	return err;
}

void volt_daemon_polling_handler(void)
{
	uint16_t volt_adc = 0;
	int result = BK_OK;
	float cur_vol = 0.0;

    bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_SARADC, 0,0);
	s_voltd.detect_cnt++;
	volt_daemon_stop();

	result = _volt_detect_init_adc_buffer();
	if (BK_OK == result) {
		result = _volt_detect_get_adc_data(ADC_VOLT_SENSER_CHANNEL, s_raw_voltage_data);
	}
	if (BK_OK != result) {
		TEMPD_LOGW("vdetect failed(%d), retry\n", result);
		volt_daemon_restart();
        bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_SARADC, 1,0);

		return; //TODO is that correct?
	}

    volt_adc = _volt_detect_calculate_voltage(s_raw_voltage_data);
	TEMPD_LOGD("cnt=%d, interval=%d, last=%d, cur=%d, thr=%d\r\n",
		s_voltd.detect_cnt, s_voltd.detect_interval, s_voltd.last_detect_val,
		volt_adc, s_voltd.detect_threshold);

	cur_vol = volt_detect_calc_voltage(volt_adc);
	bk_sensor_traversal_voltage_callback(volt_adc, cur_vol, s_voltd.volt_last);
	bk_sensor_set_current_voltage(cur_vol);
	s_voltd.volt_last = cur_vol;

	volt_daemon_restart();

    bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_SARADC, 1,0);
}

void volt_daemon_init(void)
{
	int err;

	s_voltd.last_detect_val = 0;
	s_voltd.inital_data = 0;
	s_voltd.detect_threshold = 0;
	if (ate_is_enabled()) {
		s_voltd.detect_interval = ADC_TMEP_DETECT_INTERVAL_INIT;
	} else {
		s_voltd.detect_interval = ADC_TMEP_DETECT_INTERVAL;
	}
	s_voltd.detect_cnt = 0;

	bk_sensor_send_msg(VOLT_TIMER_EXPIRED);

#if TEMP_DETECT_ONESHOT_TIMER
    err = rtos_init_oneshot_timer(&s_voltd.detect_oneshot_timer,
						  s_voltd.detect_interval * 1000,
						  _volt_detect_oneshot_timer_handler,
						  (void *)0,
						  (void *)0);
    BK_ASSERT(kNoErr == err);
    err = rtos_start_oneshot_timer(&s_voltd.detect_oneshot_timer);
#else
	err = rtos_init_timer(&s_voltd.detect_timer,
					  s_voltd.detect_interval * 1000,
					  _volt_detect_timer_handler,
					  (void *)0);
    BK_ASSERT(kNoErr == err);
    err = rtos_start_timer(&s_voltd.detect_timer);
#endif
	BK_ASSERT(kNoErr == err);
}

void volt_daemon_deinit(void)
{
	int err;

#if TEMP_DETECT_ONESHOT_TIMER
    err = rtos_deinit_oneshot_timer(&s_voltd.detect_oneshot_timer);
#else
    err = rtos_deinit_timer(&s_voltd.detect_timer);
#endif
	BK_ASSERT(kNoErr == err);
}

int volt_detect_start(void)
{
#if TEMP_DETECT_ONESHOT_TIMER
	if (s_voltd.detect_oneshot_timer.function &&
		!rtos_is_oneshot_timer_running(&s_voltd.detect_oneshot_timer))
		bk_sensor_send_msg(VOLT_RESTART_TIMER);
#else
	if (s_voltd.detect_timer.function &&
		!rtos_is_timer_running(&s_voltd.detect_timer))
		bk_sensor_send_msg(VOLT_RESTART_TIMER);
#endif
	return BK_OK;
}

int volt_detect_stop(void)
{
#if TEMP_DETECT_ONESHOT_TIMER
    if (s_voltd.detect_oneshot_timer.function
        && rtos_is_oneshot_timer_running(&s_voltd.detect_oneshot_timer))
        bk_sensor_send_msg(VOLT_PAUSE_TIMER);
#else
    if (s_voltd.detect_timer.function
		&& rtos_is_timer_running(&s_voltd.detect_timer))
		bk_sensor_send_msg(VOLT_PAUSE_TIMER);
#endif
	return BK_OK;
}

#endif

#if (CONFIG_SOC_BK7256XX) ||(CONFIG_SOC_BK7236A) || (CONFIG_SOC_BK7231N) || (CONFIG_SOC_BK7236XX) || (CONFIG_SOC_BK7239XX) || (CONFIG_SOC_BK7286XX) || (CONFIG_SOC_BK7286XX)
int volt_single_get_current_voltage(UINT32 *volt_value)
{
    int result;
    int retry_count = 3;

	if (!volt_value)
		return BK_ERR_NULL_PARAM;

	BK_RETURN_ON_ERR(_volt_detect_init_adc_buffer());

    *volt_value = 0;

    for (; retry_count > 0; retry_count--) {
		result = _volt_detect_get_adc_data(ADC_VOLT_SENSER_CHANNEL, s_raw_voltage_data);
		if (BK_OK != result) {
			TEMPD_LOGW("get volt_single failed(%d), retry\n", result);
            continue;
		}

        *volt_value = _volt_detect_calculate_voltage(s_raw_voltage_data);
        if ((ADC_TEMP_VAL_MIN < *volt_value)/* && (*volt_value < ADC_TEMP_VAL_MAX)*/) {
            break;
        }
        result = BK_ERR_TRY_AGAIN;
    }

    return result;
}
#endif
