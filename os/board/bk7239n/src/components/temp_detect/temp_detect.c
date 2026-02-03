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
#include "bk_ps.h"
#include "bk_wifi_private.h"
#include <components/log.h>

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
#include <driver/aon_rtc.h>

#define CFG_USE_TEMPERATURE_DETECT                 1
#define CFG_SUPPORT_SARADC                         1
//TODO - more optimization for temp detect
//1. Keep temperature detection related work here, such as detection task, config etc.
//2. Move temp sensor and adc related work to driver, better implenented temp sensor
//3. Define public temperature API, such as bk_tempsensor_get_temperature()
//   and temp detect module depends on tempsensor!

extern void manual_cal_tmp_pwr_init(uint16_t init_temp, uint16_t init_threshold,
		uint16_t init_dist);

static uint16_t *s_raw_temperature_data = NULL;

//TODO
//Both temp detect and get_temperature api share same raw buffer,
//but we don't protect!
static int tempd_init_temperature_raw_data(void)
{
	if (!s_raw_temperature_data) {
		s_raw_temperature_data = (uint16_t *)os_zalloc(ADC_TEMP_BUFFER_SIZE *
					sizeof(s_raw_temperature_data[0]));
	}

	if (!s_raw_temperature_data) {
		TEMPD_LOGE("oom\r\n");
		return BK_ERR_NO_MEM;
	}

	return BK_OK;
}

//TODO better to put to driver
static void temp_sensor_enable(void)
{

#if (CONFIG_SOC_BK7256XX) || (CONFIG_SOC_BK7236XX) || (CONFIG_SOC_BK7239XX) || (CONFIG_SOC_BK7286XX)
    sys_drv_en_tempdet(1);
#else
    uint32_t param;

    param = BLK_BIT_TEMPRATURE_SENSOR;
    sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_BLK_ENABLE, &param);
#endif
    
}

static void temp_sensor_disable(void)
{
#if (CONFIG_SOC_BK7256XX) || (CONFIG_SOC_BK7236XX) || (CONFIG_SOC_BK7239XX) || (CONFIG_SOC_BK7286XX)
    sys_drv_en_tempdet(0);
#else

    uint32_t param;
    param = BLK_BIT_TEMPRATURE_SENSOR;
    sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_BLK_DISABLE, &param);
#endif
}

#if TEMPD_DISPLAY_RAW_DATA
static void tempd_show_raw_temperature_data(void)
{
	TEMPD_LOGI("raw data ");
	for (int i = 0; i < ADC_TEMP_BUFFER_SIZE; i++) {
		BK_LOG_RAW("%04x ", s_raw_temperature_data[i]);
	}
	BK_LOG_RAW("\n");
}
#else
#define tempd_show_raw_temperature_data()
#endif

static uint16_t tempd_calculate_temperature(void)
{
	tempd_show_raw_temperature_data();

#if (CONFIG_SOC_BK7239XX || CONFIG_SOC_BK7236N)
	uint32_t sum = 0, index, count = 0;

	for (index = 5; index < ADC_TEMP_BUFFER_SIZE; index++) {
		/* 0 is invalid, but saradc may return 0 in power save mode */
		if ((0 != s_raw_temperature_data[index]) && (8192 != s_raw_temperature_data[index])) {
			sum += s_raw_temperature_data[index];
			count++;
		}
	}

	if (count == 0)
		s_raw_temperature_data[0] = 0;
	else {
		sum = sum / count;
		s_raw_temperature_data[0] = sum;
	}
#elif (CONFIG_SOC_BK7236XX)
	uint32_t sum = 0, index, count = 0;

	for (index = 5; index < ADC_TEMP_BUFFER_SIZE; index++) {
		/* 0 is invalid, but saradc may return 0 in power save mode */
		if ((0 != s_raw_temperature_data[index]) && (2048 != s_raw_temperature_data[index])) {
			sum += s_raw_temperature_data[index];
			count++;
		}
	}

	if (count == 0)
		s_raw_temperature_data[0] = 0;
	else {
		sum = sum / count;
		sum = sum / 4;
		s_raw_temperature_data[0] = sum;
	}
#elif (CONFIG_SOC_BK7256XX)
	uint32_t sum = 0, sum1, sum2;

	sum1 = s_raw_temperature_data[1] + s_raw_temperature_data[2];
	sum2 = s_raw_temperature_data[3] + s_raw_temperature_data[4];
	sum = sum1 / 2 + sum2 / 2;
	sum = sum / 2;
	sum = sum / 16;
	s_raw_temperature_data[0] = sum;

#else
	uint32_t sum = 0, sum1, sum2;

	sum1 = s_raw_temperature_data[1] + s_raw_temperature_data[2];
	sum2 = s_raw_temperature_data[3] + s_raw_temperature_data[4];
	sum = sum1 / 2 + sum2 / 2;
	sum = sum / 2;
	sum = sum / 4;
	s_raw_temperature_data[0] = sum;
#endif

	return s_raw_temperature_data[0];
}

#if CONFIG_SDMADC_TEMP
static bk_err_t tempd_adc_get_raw_data(sdmadc_chan_t adc_chan)
{
	sdmadc_config_t config = {0};

	temp_sensor_enable();

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

	err = bk_sdmadc_read_raw(s_raw_temperature_data, ADC_TEMP_BUFFER_SIZE);

_release_adc:
	temp_sensor_disable();
	bk_sdmadc_deinit();
	bk_sdmadc_driver_deinit();
	return err;
}
#else
static int tempd_adc_get_raw_data(adc_chan_t adc_chan)
{
	adc_config_t config = {0};
	int err = BK_OK;

	BK_RETURN_ON_ERR(bk_adc_acquire());

	temp_sensor_enable();

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
#else
	#error "select sardc hw module version"
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

	err = bk_adc_read_raw(s_raw_temperature_data, ADC_TEMP_BUFFER_SIZE,
				ADC_READ_SEMAPHORE_WAIT_TIME);
	if (BK_OK != err) {
		err = BK_ERR_TEMPD_SAMPLE_NO_DATA;
		goto _release_adc;
	}

_release_adc:
	//TODO check it, do we need to always enable temperature sensor?
	temp_sensor_disable();
    bk_adc_stop();
	bk_adc_deinit(adc_chan);
	bk_adc_release();
	return err;
}
#endif

static int tempd_adc_get_temperature(uint16_t *temperature)
{
	int err;

	err = tempd_adc_get_raw_data(ADC_TEMP_SENSOR_CHANNEL);
	if (BK_OK != err)
		return err;

	*temperature = tempd_calculate_temperature();
	return BK_OK;
}

#if CONFIG_TEMP_DETECT
static temp_detect_config_t s_tempd = {0};

extern void rwnx_cal_do_temp_detect(uint16_t cur_val, uint16_t threshold, uint16_t *last);

int temp_detect_init(uint32_t init_temperature)
{
	TEMPD_LOGD("init temperature %d\r\n", init_temperature);

	BK_RETURN_ON_ERR(tempd_init_temperature_raw_data());
	uint16_t adc_25 = (uint32_t)(init_temperature);
    saradc_set_calibrate_val(&adc_25, SARADC_CALIBRATE_TEMP_CODE25);

	return BK_OK;
}

//TODO by frank
int temp_detect_deinit(void)
{
	// set reg mod & pa to initial value, this must be happened in
	// txevm or rxsens to calibration txpwr or rxsens
	manual_cal_temp_pwr_unint();

	return 0;
}

int temp_detect_stop(void)
{
#if TEMP_DETECT_ONESHOT_TIMER
    if (s_tempd.detect_oneshot_timer.function
        && rtos_is_oneshot_timer_running(&s_tempd.detect_oneshot_timer))
        bk_sensor_send_msg(TMPD_PAUSE_TIMER);
#else
    if (s_tempd.detect_timer.function
		&& rtos_is_timer_running(&s_tempd.detect_timer))
		bk_sensor_send_msg(TMPD_PAUSE_TIMER);
#endif
	return BK_OK;
}

int temp_detect_start(void)
{
#if TEMP_DETECT_ONESHOT_TIMER
	if (s_tempd.detect_oneshot_timer.function &&
		!rtos_is_oneshot_timer_running(&s_tempd.detect_oneshot_timer))
		bk_sensor_send_msg(TMPD_RESTART_TIMER);
#else
	if (s_tempd.detect_timer.function &&
		!rtos_is_timer_running(&s_tempd.detect_timer))
		bk_sensor_send_msg(TMPD_RESTART_TIMER);
#endif
	return BK_OK;
}

#if TEMP_DETECT_ONESHOT_TIMER
__IRAM_SEC static void temp_detect_oneshot_timer_handler(void *data1, void *data2)
{
	bk_sensor_send_msg(TMPD_TIMER_EXPIRED);
	bk_sensor_send_msg(TMPD_RESTART_TIMER);
}
#else
static void temp_detect_timer_handler(void *data)
{
	bk_sensor_send_msg(TMPD_TIMER_EXPIRED);
}
#endif

void temp_daemon_stop(void)
{
	int err;

#if TEMP_DETECT_ONESHOT_TIMER
    err = rtos_stop_oneshot_timer(&s_tempd.detect_oneshot_timer);
#else
    err = rtos_stop_timer(&s_tempd.detect_timer);
#endif

	BK_ASSERT(kNoErr == err);
	TEMPD_LOGD("stop\n");
}

void temp_daemon_restart(void)
{
	int err;

#if TEMP_DETECT_ONESHOT_TIMER
    err = rtos_oneshot_reload_timer(&s_tempd.detect_oneshot_timer);
#else
    err = rtos_reload_timer(&s_tempd.detect_timer);
#endif
	BK_ASSERT(kNoErr == err);
	TEMPD_LOGD("restart\n");
}

void temp_daemon_change_config(void)
{
	temp_detect_change_configuration(ADC_TMEP_DETECT_INTERVAL,
		s_tempd.detect_threshold, ADC_TMEP_DIST_INTIAL_VAL);
}

static bool tempd_need_change_detect_interval(uint32_t detect_cnt)
{
	uint32_t detect_change_cnt = ADC_TMEP_DETECT_INTERVAL_CHANGE/
					ADC_TMEP_DETECT_INTERVAL_INIT;

	return (detect_cnt == detect_change_cnt);
}

void temp_daemon_detect_temperature(void)
{
	uint16_t adc_code = 0;
	int err = BK_OK;
    bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_SARADC, 0,0);
	s_tempd.detect_cnt++;
	temp_daemon_stop();

	err = tempd_adc_get_temperature(&adc_code);
	if (BK_OK != err) {
		TEMPD_LOGW("tdetect failed(%d), retry\n", err);
		temp_daemon_restart();
        bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_SARADC, 1,0);
		return; //TODO is that correct?
	}

	TEMPD_LOGD("cnt=%d, interval=%d, last=%d, cur=%d, thr=%d\r\n",
		s_tempd.detect_cnt, s_tempd.detect_interval, (int)s_tempd.temp_last,
		adc_code, s_tempd.detect_threshold);

	float temp_code = bk_adc_data_calculate(adc_code, ADC_TEMP_SENSOR_CHANNEL);
	bk_sensor_traversal_temperature_callback(adc_code, temp_code, s_tempd.temp_last);
	bk_sensor_set_current_temperature(temp_code);
	s_tempd.temp_last = temp_code;

	if (tempd_need_change_detect_interval(s_tempd.detect_cnt))
		bk_sensor_send_msg(TMPD_CHANGE_PARAM);
	else
		temp_daemon_restart();

    bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_SARADC, 1,0);
}

void temp_daemon_init(void)
{
	int err;

	s_tempd.detect_threshold = ADC_TMEP_LSB_PER_10DEGREE * ADC_TMEP_10DEGREE_PER_DBPWR;
	s_tempd.detect_interval = ADC_TMEP_DETECT_INTERVAL_INIT;
	s_tempd.detect_cnt = 0;

	TEMPD_LOGD("xtal inital:%d\r\n", sys_drv_analog_get_xtalh_ctune());

	bk_sensor_send_msg(TMPD_TIMER_EXPIRED);
#if TEMP_DETECT_ONESHOT_TIMER
    err = rtos_init_oneshot_timer(&s_tempd.detect_oneshot_timer,
						  s_tempd.detect_interval * 1000,
						  temp_detect_oneshot_timer_handler,
						  (void *)0,
						  (void *)0);
    BK_ASSERT(kNoErr == err);
    err = rtos_start_oneshot_timer(&s_tempd.detect_oneshot_timer);
#else
	err = rtos_init_timer(&s_tempd.detect_timer,
					  s_tempd.detect_interval * 1000,
					  temp_detect_timer_handler,
					  (void *)0);
    BK_ASSERT(kNoErr == err);
    err = rtos_start_timer(&s_tempd.detect_timer);
#endif
	BK_ASSERT(kNoErr == err);
}

void temp_daemon_deinit(void)
{
	int err;

#if TEMP_DETECT_ONESHOT_TIMER
    err = rtos_deinit_oneshot_timer(&s_tempd.detect_oneshot_timer);
#else
    err = rtos_deinit_timer(&s_tempd.detect_timer);
#endif
	BK_ASSERT(kNoErr == err);
}

void temp_detect_change_configuration(uint32_t interval, uint32_t threshold, uint32_t dist)
{
	bk_err_t err;

	if (interval == 0)
		interval = ADC_TMEP_DETECT_INTERVAL;

	if (threshold == 0)
		threshold = ADC_TMEP_LSB_PER_10DEGREE * ADC_TMEP_10DEGREE_PER_DBPWR;

	if (dist == 0)
		dist = ADC_TMEP_DIST_INTIAL_VAL;

	TEMPD_LOGD("change config, interval=%d, threshold=%d, dist=%d\r\n",
		interval, threshold, dist);

	if (s_tempd.detect_interval != interval) {
		s_tempd.detect_interval = interval;
#if TEMP_DETECT_ONESHOT_TIMER
        if (s_tempd.detect_oneshot_timer.function) {
    		err = rtos_deinit_oneshot_timer(&s_tempd.detect_oneshot_timer);
    		BK_ASSERT(kNoErr == err);
    	}
        err = rtos_init_oneshot_timer(&s_tempd.detect_oneshot_timer,
                                  s_tempd.detect_interval * 1000,
                                  temp_detect_oneshot_timer_handler,
                                  (void *)0,
                                  (void *)0);
        BK_ASSERT(kNoErr == err);
        err = rtos_start_oneshot_timer(&s_tempd.detect_oneshot_timer);
#else
		if (s_tempd.detect_timer.function) {
			err = rtos_deinit_timer(&s_tempd.detect_timer);
			BK_ASSERT(kNoErr == err);
		}

		err = rtos_init_timer(&s_tempd.detect_timer,
							  s_tempd.detect_interval * 1000,
							  temp_detect_timer_handler,
							  (void *)0);
		BK_ASSERT(kNoErr == err);

		err = rtos_start_timer(&s_tempd.detect_timer);
#endif
		BK_ASSERT(kNoErr == err);
	}
}
#else
int temp_detect_init(uint32_t init_temperature)
{
    return 0;
}

int temp_detect_deinit(void)
{
    return 0;
}

void temp_detect_change_configuration(uint32_t interval, uint32_t threshold, uint32_t dist)
{
    return;
}
#endif  // CONFIG_TEMP_DETECT

//TODO why only check the temperature range here?
//why NOT in temp_detect? Frank check it!!!
static bool tempd_is_temperature_valid(uint16_t temperature)
{
	return ((temperature > ADC_TEMP_VAL_MIN) && (temperature < ADC_TEMP_VAL_MAX));
}

int temp_detect_get_temperature(uint32_t *temperature)
{
	uint16_t cur_temperature = 0;
	int err = BK_OK;

	if (!temperature)
		return BK_ERR_NULL_PARAM;

	BK_RETURN_ON_ERR(tempd_init_temperature_raw_data());

	for (int i = 0; i < TEMPD_MAX_RETRY_NUM; i++) {
		err = tempd_adc_get_temperature(&cur_temperature);
		if (BK_OK != err) {
			TEMPD_LOGW("get temperature failed(%d), retry\n", err);
			continue;
		}

		if (tempd_is_temperature_valid(cur_temperature))
			break;
		err = BK_ERR_TRY_AGAIN;
	}

	*temperature = cur_temperature;
	TEMPD_LOGD("get temperature %d\n", cur_temperature);
	return err;
}
