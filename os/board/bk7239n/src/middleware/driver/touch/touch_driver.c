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
#include "sys_driver.h"
#include "gpio_map.h"
#include "gpio_driver.h"
#include "touch_driver.h"
#include "aon_pmu_driver.h"
#include <driver/gpio.h>
#include <driver/int.h>
#include <driver/timer.h>
#include <driver/touch_types.h>
#include <os/os.h>

#if defined(CONFIG_TOUCH_PM_SUPPORT)
	#include <modules/pm.h>
#endif

extern void delay(int num);

typedef struct {
	touch_isr_t callback;
	void *param;
} touch_callback_t;


#define TOUCH_RETURN_ON_INVALID_ID(touch_id) do {\
		if ((touch_id) >= SOC_TOUCH_ID_NUM) {\
			return BK_ERR_TOUCH_ID;\
		}\
	} while(0)

uint32_t s_touch_channel = 0;
uint32_t s_touch_wakeup_channel = 0;

static touch_callback_t s_touch_isr[SOC_TOUCH_ID_NUM] = {NULL};

uint8_t digital_led_gpio_map[9] = {GPIO_26, GPIO_8, GPIO_9, GPIO_0, GPIO_1, GPIO_24, GPIO_45, GPIO_44, GPIO_39};

uint16_t disp_value_table[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};


static uint32_t bk_touch_channel_transfer(touch_channel_t touch_id)
{
	uint32_t touch_channel = 0;
	for (touch_channel = 0; touch_channel < 16; touch_channel++)
	{
		touch_id = touch_id / 2;
		if (touch_id == 0 ) {
			break;
		}
	}
	return touch_channel;
}

bk_err_t bk_touch_gpio_init(touch_channel_t touch_id)
{
	uint32_t touch_select = 0;
	touch_select = bk_touch_channel_transfer(touch_id);
	TOUCH_RETURN_ON_INVALID_ID(touch_select);
	switch(touch_id)
	{
		case BK_TOUCH_0:
			gpio_dev_unmap(GPIO_12);
			gpio_dev_map(GPIO_12, GPIO_DEV_TOUCH0);
			bk_gpio_disable_output(GPIO_12);
			break;

		case BK_TOUCH_1:
			gpio_dev_unmap(GPIO_13);
			gpio_dev_map(GPIO_13, GPIO_DEV_TOUCH1);
			bk_gpio_disable_output(GPIO_13);
			break;

		case BK_TOUCH_2:
			gpio_dev_unmap(GPIO_28);
			gpio_dev_map(GPIO_28, GPIO_DEV_TOUCH2);
			bk_gpio_disable_output(GPIO_28);
			break;

		case BK_TOUCH_3:
			gpio_dev_unmap(GPIO_29);
			gpio_dev_map(GPIO_29, GPIO_DEV_TOUCH3);
			bk_gpio_disable_output(GPIO_29);
			break;

		case BK_TOUCH_4:
			gpio_dev_unmap(GPIO_30);
			gpio_dev_map(GPIO_30, GPIO_DEV_TOUCH4);
			bk_gpio_disable_output(GPIO_30);
			break;

		case BK_TOUCH_5:
			gpio_dev_unmap(GPIO_31);
			gpio_dev_map(GPIO_31, GPIO_DEV_TOUCH5);
			bk_gpio_disable_output(GPIO_31);
			break;

		case BK_TOUCH_6:
			gpio_dev_unmap(GPIO_32);
			gpio_dev_map(GPIO_32, GPIO_DEV_TOUCH6);
			bk_gpio_disable_output(GPIO_32);
			break;

		case BK_TOUCH_7:
			gpio_dev_unmap(GPIO_33);
			gpio_dev_map(GPIO_33, GPIO_DEV_TOUCH7);
			bk_gpio_disable_output(GPIO_33);
			break;

		case BK_TOUCH_8:
			gpio_dev_unmap(GPIO_34);
			gpio_dev_map(GPIO_34, GPIO_DEV_TOUCH8);
			bk_gpio_disable_output(GPIO_34);
			break;

		case BK_TOUCH_9:
			gpio_dev_unmap(GPIO_35);
			gpio_dev_map(GPIO_35, GPIO_DEV_TOUCH9);
			bk_gpio_disable_output(GPIO_35);
			break;

		case BK_TOUCH_10:
			gpio_dev_unmap(GPIO_36);
			gpio_dev_map(GPIO_36, GPIO_DEV_TOUCH10);
			bk_gpio_disable_output(GPIO_36);
			break;

		case BK_TOUCH_11:
			gpio_dev_unmap(GPIO_37);
			gpio_dev_map(GPIO_37, GPIO_DEV_TOUCH11);
			bk_gpio_disable_output(GPIO_37);
			break;

		case BK_TOUCH_12:
			gpio_dev_unmap(GPIO_38);
			gpio_dev_map(GPIO_38, GPIO_DEV_TOUCH12);
			bk_gpio_disable_output(GPIO_38);
			break;

		case BK_TOUCH_13:
			gpio_dev_unmap(GPIO_39);
			gpio_dev_map(GPIO_39, GPIO_DEV_TOUCH13);
			bk_gpio_disable_output(GPIO_39);
			break;

		case BK_TOUCH_14:
			gpio_dev_unmap(GPIO_46);
			gpio_dev_map(GPIO_46, GPIO_DEV_TOUCH14);
			bk_gpio_disable_output(GPIO_46);
			break;

		case BK_TOUCH_15:
			gpio_dev_unmap(GPIO_47);
			gpio_dev_map(GPIO_47, GPIO_DEV_TOUCH15);
			bk_gpio_disable_output(GPIO_47);
			break;

		default:
			TOUCH_LOGI("unsupported touch id\r\n");
			break;
	}

	return BK_OK;
}

bk_err_t bk_touch_enable(touch_channel_t touch_id)
{
	uint32_t touch_select = 0;
	touch_select = bk_touch_channel_transfer(touch_id);
	TOUCH_RETURN_ON_INVALID_ID(touch_select);
	bk_int_isr_register(INT_SRC_TOUCH, touch_isr, NULL);
	aon_pmu_drv_touch_select(touch_select);

	sys_drv_touch_power_down(0);

#if (defined(CONFIG_TOUCH_INTERNAL_SERIAL_CAP))
	sys_drv_touch_serial_cap_enable();
#else
	sys_drv_touch_serial_cap_disable();
#endif

	return BK_OK;
}

bk_err_t bk_touch_disable(void)
{
	sys_drv_touch_power_down(1);

	return BK_OK;
}

bk_err_t bk_touch_config(const touch_config_t *touch_config)
{
	sys_drv_touch_sensitivity_level_set(touch_config->sensitivity_level);
	sys_drv_touch_detect_threshold_set(touch_config->detect_threshold);
	sys_drv_touch_detect_range_set(touch_config->detect_range);

	return BK_OK;
}

bk_err_t bk_touch_calib_enable(uint32_t enable)
{
	if(enable) {
		sys_drv_touch_calib_enable(1);
	} else {
		sys_drv_touch_calib_enable(0);
	}

	return BK_OK;
}

bk_err_t bk_touch_calibration_start(void)
{
	bk_touch_calib_enable(0);
	delay(100);
	bk_touch_calib_enable(1);
	delay(200);

	return BK_OK;
}

bk_err_t bk_touch_adc_mode_enable(uint32_t enable)
{
    if(enable) {
        sys_drv_touch_adc_mode_enable(1);
    } else {
        sys_drv_touch_adc_mode_enable(0);
    }

    return BK_OK;
}

bk_err_t bk_touch_scan_mode_enable(uint32_t enable)
{
	if(enable) {
		sys_drv_touch_scan_mode_enable(1);
	} else {
		sys_drv_touch_scan_mode_enable(0);
	}

	return BK_OK;
}

bk_err_t bk_touch_manul_mode_enable(uint32_t calib_value)
{
	sys_drv_touch_manul_mode_calib_value_set(calib_value);
	sys_drv_touch_manul_mode_enable(1);
	delay(100);

	return BK_OK;
}

bk_err_t bk_touch_manul_mode_disable(void)
{
	sys_drv_touch_manul_mode_enable(0);

	return BK_OK;
}

bk_err_t bk_touch_scan_mode_multi_channl_set(touch_channel_t touch_id)
{
	sys_drv_touch_scan_mode_chann_set(touch_id);

	return BK_OK;
}

bk_err_t bk_touch_int_enable(touch_channel_t touch_id, uint32_t enable)
{
	if(enable) {
		sys_drv_touch_int_enable(1);
		aon_pmu_drv_touch_int_enable(touch_id);
	} else {
		aon_pmu_drv_touch_int_disable(touch_id);
		sys_drv_touch_int_enable(0);
	}

	return BK_OK;
}

bk_err_t bk_touch_get_int_status(void)
{
	uint32_t  int_status = 0;
	int_status = aon_pmu_drv_get_touch_int_status();

	return int_status;
}

bk_err_t bk_touch_clear_int(touch_channel_t touch_id)
{
	uint32_t touch_select = 0;
	touch_select = bk_touch_channel_transfer(touch_id);
	TOUCH_RETURN_ON_INVALID_ID(touch_select);
	aon_pmu_drv_clear_touch_int(touch_id);

	return BK_OK;
}

uint32_t bk_touch_get_calib_value(void)
{
	uint32_t calib_value = 0;
	calib_value = aon_pmu_drv_get_cap_cal();

	return calib_value;
}

uint32_t bk_touch_get_touch_status(void)
{
	uint32_t touch_status = 0;
	touch_status = aon_pmu_drv_get_touch_state();

	return touch_status;
}

bk_err_t bk_touch_digital_tube_init(void)
{
	uint8_t i = 0;
	for (i = 0; i < 9; i++) {
		gpio_dev_unmap(digital_led_gpio_map[i]);
		bk_gpio_enable_output(digital_led_gpio_map[i]);
		bk_gpio_set_output_high(digital_led_gpio_map[i]);
	}

	return BK_OK;
}

void bk_touch_digital_tube_display(uint8_t disp_value)
{
	if (0 <= disp_value && disp_value <= 9) {
		bk_gpio_set_output_high(digital_led_gpio_map[8]);
		bk_gpio_set_output_low(digital_led_gpio_map[7]);
	
		for (uint8_t j = 0; j < 7; j++) {
			if (((disp_value_table[0] >> j) & 0x01) == 0) {
				bk_gpio_set_output_low(digital_led_gpio_map[j]);
			} else {
				bk_gpio_set_output_high(digital_led_gpio_map[j]);
			}
		}
		rtos_delay_milliseconds(10);
	
		bk_gpio_set_output_high(digital_led_gpio_map[7]);
		bk_gpio_set_output_low(digital_led_gpio_map[8]);
		for (uint8_t k = 0; k < 7; k++) {
			if (((disp_value_table[disp_value] >> k) & 0x01) == 0) {
				bk_gpio_set_output_low(digital_led_gpio_map[k]);
			} else {
				bk_gpio_set_output_high(digital_led_gpio_map[k]);
			}
		}
		rtos_delay_milliseconds(10);

	} else if (10 <= disp_value && disp_value < 16) {
		bk_gpio_set_output_high(digital_led_gpio_map[8]);
		bk_gpio_set_output_low(digital_led_gpio_map[7]);
	
		for (uint8_t i = 0; i < 7; i++) {
			if (((disp_value_table[1] >> i) & 0x01) == 0) {
				bk_gpio_set_output_low(digital_led_gpio_map[i]);
			} else {
				bk_gpio_set_output_high(digital_led_gpio_map[i]);
			}
		}
		rtos_delay_milliseconds(10);
	
		bk_gpio_set_output_high(digital_led_gpio_map[7]);
		bk_gpio_set_output_low(digital_led_gpio_map[8]);
	
		for (uint8_t j = 0; j < 7; j++) {
			if (((disp_value_table[disp_value % 10] >> j) & 0x01) == 0) {
				bk_gpio_set_output_low(digital_led_gpio_map[j]);
			} else {
				bk_gpio_set_output_high(digital_led_gpio_map[j]);
			}
		}
		rtos_delay_milliseconds(10);

	} else {
		TOUCH_LOGI("Invalid touch channel!\r\n");
		return;
	}

}

#if (defined(CONFIG_TOUCH_PM_SUPPORT))
void bk_touch_wakeup_channel_set(touch_channel_t channel)
{
	s_touch_wakeup_channel = channel;
}

uint32_t bk_touch_wakeup_channel_get(void)
{
	return s_touch_wakeup_channel;
}

static int touch_pm_enter_cb(uint64_t sleep_time_ms, void *args)
{
	uint32_t cap_out = 0;
	touch_config_t touch_config;

	bk_touch_gpio_init(s_touch_wakeup_channel);
	bk_touch_scan_mode_enable(0);
	bk_touch_enable(s_touch_wakeup_channel);

	touch_config.sensitivity_level = TOUCH_SENSITIVITY_LEVLE_1;
	touch_config.detect_threshold = TOUCH_DETECT_THRESHOLD_6;
	touch_config.detect_range = TOUCH_DETECT_RANGE_8PF;
	bk_touch_config(&touch_config);

	bk_touch_calibration_start();
	cap_out = bk_touch_get_calib_value();
	TOUCH_LOGI("cap_out0 = %x\r\n", cap_out);
	if (cap_out >= 0x1F0) {
		touch_config.detect_range = TOUCH_DETECT_RANGE_12PF;
		bk_touch_config(&touch_config);
		bk_touch_calibration_start();
		cap_out = bk_touch_get_calib_value();
		TOUCH_LOGI("cap_out1 = %x\r\n", cap_out);
		if (cap_out >= 0x1F0) {
			touch_config.detect_range = TOUCH_DETECT_RANGE_19PF;
			bk_touch_config(&touch_config);
			bk_touch_calibration_start();
			cap_out = bk_touch_get_calib_value();
			TOUCH_LOGI("cap_out2 = %x\r\n", cap_out);
			if (cap_out >= 0x1F0) {
				touch_config.detect_range = TOUCH_DETECT_RANGE_27PF;
				bk_touch_config(&touch_config);
				bk_touch_calibration_start();
				cap_out = bk_touch_get_calib_value();
				TOUCH_LOGI("cap_out3 = %x\r\n", cap_out);
				if (cap_out >= 0x1F0) {
					TOUCH_LOGE("Calibration value is out of the detect range, the channel cannot be used, please select the other channel!\r\n");
					return BK_FAIL;
				}
			}
		}
	}
	bk_touch_int_enable(s_touch_wakeup_channel, 1);

	return BK_OK;
}

bk_err_t bk_touch_pm_init(void)
{
	bk_err_t ret = BK_OK;

	pm_cb_conf_t enter_config_touch = {touch_pm_enter_cb, NULL};
	ret = bk_pm_sleep_register_cb(PM_MODE_DEEP_SLEEP, PM_DEV_ID_TOUCH, &enter_config_touch, NULL);
	if (ret != BK_OK) {
		TOUCH_LOGE("register touch pm deep sleep cb fail!\r\n");
	}

	ret = bk_pm_sleep_register_cb(PM_MODE_LOW_VOLTAGE, PM_DEV_ID_TOUCH, &enter_config_touch, NULL);
	if (ret != BK_OK) {
		TOUCH_LOGE("register touch pm low voltage cb fail!\r\n");
	}

	return ret;

}
#endif

bk_err_t bk_touch_register_touch_isr(touch_channel_t touch_id, touch_isr_t isr, void *param)
{
	uint32_t touch_channel = 0;
	touch_channel = bk_touch_channel_transfer(touch_id);
	TOUCH_RETURN_ON_INVALID_ID(touch_channel);
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	s_touch_isr[touch_channel].callback = isr;
	s_touch_isr[touch_channel].param = param;
	GLOBAL_INT_RESTORE();

	return BK_OK;
}

static void touch_timer_isr(timer_id_t chan)
{
	uint32_t touch_status = 0;
	touch_status = bk_touch_get_touch_status();
	if (!(touch_status & (1 << s_touch_channel))) {
		bk_touch_int_enable(1 << s_touch_channel, 1);
		bk_timer_stop(chan);
	}
}

void touch_isr(void)
{
	int ret = 0;
	uint32_t int_status = 0;
	uint32_t touch_id = 0;
	int_status = bk_touch_get_int_status();

	for (touch_id = 0; touch_id < SOC_TOUCH_ID_NUM; touch_id++)
	{
		if (int_status & (1 << touch_id)) {
			TOUCH_LOGI("Touch[%d] has been selected!\r\n", touch_id);
			s_touch_channel = touch_id;
			bk_touch_clear_int(1 << touch_id);
			bk_touch_int_enable(1 << touch_id, 0);
			if (s_touch_isr[touch_id].callback) {
				s_touch_isr[touch_id].callback(s_touch_isr[touch_id].param);
			}

			ret = bk_timer_start(TIMER_ID0, 200, touch_timer_isr);
			if (ret != BK_OK) {
				TOUCH_LOGW("Timer start failed\r\n");
			}
			break;
		}
	}
}



