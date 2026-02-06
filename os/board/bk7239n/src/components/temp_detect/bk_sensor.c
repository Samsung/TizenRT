#include <common/bk_include.h>
#include <common/bk_err.h>
#if ((CONFIG_SOC_BK7236XX) || (CONFIG_SOC_BK7239XX)) && (CONFIG_OTP)
#include <driver/otp.h>
#elif (CONFIG_SOC_BK7256XX)
#include <driver/efuse.h>
#include "modules/chip_support.h"
#endif
#include <os/os.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "bk_sensor_internal.h"
#include "temp_detect.h"
#include "volt_detect.h"

#include "bk_saradc.h"
#include "sdmadc/sdmadc_driver.h"
#include <driver/rosc_32k.h>
#include <modules/pm.h>

#include "sys_driver.h"

#define TAG "sensor"
#define ADC_SWITCH_DELT                   2

#define TEMPD_MAX_CALLBACK_NUM            2
#define VOLTD_MAX_CALLBACK_NUM            2

#if (CONFIG_TEMP_DETECT || CONFIG_VOLT_DETECT)
typedef struct {
	beken_thread_t    task_handle;
	beken_queue_t     msg_queue;
	beken_mutex_t     lock;

#if (CONFIG_TEMP_DETECT)
	float             temperature;
	bk_sensor_callback temp_callbacks[TEMPD_MAX_CALLBACK_NUM];
#endif

#if (CONFIG_VOLT_DETECT)
	float             voltage;
	bk_sensor_callback volt_callbacks[VOLTD_MAX_CALLBACK_NUM];
#endif
} MCU_SENSOR_INFO;
MCU_SENSOR_INFO g_sensor_info;

static void bk_sensor_main(beken_thread_arg_t data)
{
	int err;

#if (CONFIG_TEMP_DETECT)
	temp_daemon_init();
#endif
#if CONFIG_VOLT_DETECT
	volt_daemon_init();
#endif

	while (1) {
		tempd_msg_t msg;

		err = rtos_pop_from_queue(&g_sensor_info.msg_queue, &msg, BEKEN_WAIT_FOREVER);
		if (kNoErr == err) {
			switch (msg.temp_msg) {
#if (CONFIG_TEMP_DETECT)
			case TMPD_PAUSE_TIMER:
				temp_daemon_stop();
				break;

			case TMPD_RESTART_TIMER:
				temp_daemon_restart();
				break;

			case TMPD_CHANGE_PARAM:
				temp_daemon_change_config();
				break;

			case TMPD_TIMER_EXPIRED:
#if CONFIG_CKMN
				// App is active
				if ((bk_pm_module_sleep_state_get(PM_SLEEP_MODULE_NAME_APP) == 0x0) &&
					(bk_pm_module_sleep_state_get(PM_SLEEP_MODULE_NAME_ROSC) == 0x1)) {
					bk_rosc_32k_ckest_prog(96);
				}
#endif
				temp_daemon_detect_temperature();
				break;
#endif
#if CONFIG_VOLT_DETECT
			case VOLT_PAUSE_TIMER:
				volt_daemon_stop();
				break;

			case VOLT_RESTART_TIMER:
				volt_daemon_restart();
				break;

			case VOLT_TIMER_EXPIRED:
				volt_daemon_polling_handler();
				break;
#endif
			case TMPD_DEINIT:
				goto tempd_exit;

			default:
				break;
			}
		}
	}

tempd_exit:
#if (CONFIG_TEMP_DETECT)
	temp_daemon_deinit();
#endif
#if CONFIG_VOLT_DETECT
	volt_daemon_deinit();
#endif
	rtos_delete_thread(NULL);
}

#endif

#if ((CONFIG_SOC_BK7236XX) || (CONFIG_SOC_BK7239XX)) && (CONFIG_OTP)
static bk_err_t bk_sensor_load_adc_cali_value(void)
{
    bk_err_t result;

    uint16_t vol_values[3];
    uint16_t temp_values[2];
    uint8_t data[72];
    result = bk_otp_ahb_read(OTP_GADC_CALIBRATION, data, sizeof(data));
    if (result != BK_OK) {
        //BK_LOGW(TAG, "read otp calibration data failed\r\n");
        goto LOAD_SDMADC;
    }
    memcpy(&vol_values[0], &data[64], sizeof(uint16_t));
    memcpy(&vol_values[1], &data[66], sizeof(uint16_t));
    memcpy(&vol_values[2], &data[68], sizeof(uint16_t));
    if (vol_values[2] == 0) {
        BK_LOGW(TAG, "uncali saradc_ext_low value:[%x]\r\n", vol_values[2]);
        goto LOAD_SDMADC;
    }
    //BK_LOGI(TAG, "saradc ext_low value:[%x]\r\n", vol_values[2]);
    saradc_set_calibrate_val(&vol_values[2], SARADC_CALIBRATE_EXT_LOW);

    if ((result != BK_OK) || (vol_values[0] == 0) || (vol_values[1] == 0)) {
        BK_LOGW(TAG, "uncali saradc value:[%x %x]\r\n", vol_values[0], vol_values[1]);
        goto LOAD_SDMADC;
    }

    //BK_LOGI(TAG, "saradc low value:[%x]\r\n", vol_values[0]);
    //BK_LOGI(TAG, "saradc high value:[%x]\r\n", vol_values[1]);
    saradc_set_calibrate_val(&vol_values[0], SARADC_CALIBRATE_LOW);
    saradc_set_calibrate_val(&vol_values[1], SARADC_CALIBRATE_HIGH);

LOAD_SDMADC:
#if CONFIG_SDMADC
    result = bk_otp_apb_read(OTP_SDMADC_CALIBRATION, (uint8_t *)&vol_values[0], sizeof(vol_values));
    if ((result != BK_OK) || (vol_values[0] == 0) || (vol_values[1] == 0)) {
        //BK_LOGW(TAG, "uncali sdmadc value:[%x %x]\r\n", vol_values[0], vol_values[1]);
        goto LOAD_TEMP;
    }

    //BK_LOGI(TAG, "sdmadc low value:[%x]\r\n", vol_values[0]);
    //BK_LOGI(TAG, "sdmadc high value:[%x]\r\n", vol_values[1]);
    bk_sdmadc_set_calibrate_val(vol_values[0], SARADC_CALIBRATE_LOW);
    bk_sdmadc_set_calibrate_val(vol_values[1], SARADC_CALIBRATE_HIGH);

LOAD_TEMP:
#endif

    result = bk_otp_ahb_read(OTP_GADC_TEMPERATURE, (uint8_t *)&temp_values, sizeof(temp_values));
    if ((result != BK_OK) || (temp_values[0] == 0) || (0xFFFF == temp_values[0]) || (temp_values[1] == 0) || (0xFFFF == temp_values[1])) {
        //BK_LOGW(TAG, "uncali temp value:[%x]\r\n", temp_values[0]);
        goto FAILURE;
    }

#if CONFIG_TEMPERATURE_HIGH_VOLT
    //BK_LOGI(TAG, "saradc temp value:[%x]\r\n", temp_values[0]);
    saradc_set_calibrate_val(&temp_values[0], SARADC_CALIBRATE_TEMP_CODE25);
    sys_drv_set_temp_mode(true);
#else
    //BK_LOGI(TAG, "saradc temp value:[%x]\r\n", temp_values[1]);
    saradc_set_calibrate_val(&temp_values[1], SARADC_CALIBRATE_TEMP_CODE25);
    sys_drv_set_temp_mode(false);
#endif

    return BK_OK;

FAILURE:
    return BK_FAIL;
}
#else
static bk_err_t bk_sensor_load_adc_cali_value(void)
{
    return BK_FAIL;
}
#endif

bool temp_detect_is_init(void)
{
#if (CONFIG_TEMP_DETECT || CONFIG_VOLT_DETECT)
	return !!((g_sensor_info.task_handle) && (g_sensor_info.msg_queue));
#else
	return false;
#endif
}

bk_err_t bk_sensor_init(void)
{
	bk_err_t ret = BK_OK;

	bk_sensor_load_adc_cali_value();

#if (CONFIG_TEMP_DETECT || CONFIG_VOLT_DETECT)
	if (NULL == g_sensor_info.lock)
	{
		ret = rtos_init_mutex(&g_sensor_info.lock);
#if (CONFIG_VOLT_DETECT)
		g_sensor_info.voltage    = NAN;
#endif
#if (CONFIG_TEMP_DETECT)
		g_sensor_info.temperature = NAN;
#endif
	}
#endif

	return ret;
}

bk_err_t bk_sensor_start(void)
{
	bk_err_t ret = BK_OK;

#if (CONFIG_TEMP_DETECT || CONFIG_VOLT_DETECT)
	if ((!g_sensor_info.task_handle) && (!g_sensor_info.msg_queue)) {

		ret = rtos_init_queue(&g_sensor_info.msg_queue,
				"sensor_q",
				sizeof(tempd_msg_t),
				TEMPD_QUEUE_LEN);
		if (BK_OK != ret) {
			TEMPD_LOGE("ceate Q failed(%d)\r\n", ret);
			return ret;
		}

		ret = rtos_create_thread(&g_sensor_info.task_handle,
			TEMPD_TASK_PRIO,
			"bk_sensor",
			(beken_thread_function_t)bk_sensor_main,
			TEMPD_TASK_STACK_SIZE,
			(beken_thread_arg_t)NULL);
		if (BK_OK != ret) {
			rtos_deinit_queue(&g_sensor_info.msg_queue);
			g_sensor_info.msg_queue = NULL;
			TEMPD_LOGE("create task failed(%d)\r\n", ret);
			return ret;
		}
	}
#endif

	return ret;
}

bk_err_t bk_sensor_deinit(void)
{
	bk_err_t ret = BK_OK;

#if (CONFIG_TEMP_DETECT || CONFIG_VOLT_DETECT)
	if (NULL != g_sensor_info.lock)
	{
		ret = rtos_deinit_mutex(&g_sensor_info.lock);
		g_sensor_info.lock       = NULL;
#if (CONFIG_VOLT_DETECT)
		g_sensor_info.voltage    = NAN;
#endif
#if (CONFIG_TEMP_DETECT)
		g_sensor_info.temperature = NAN;
#endif
	}

	if (NULL != g_sensor_info.msg_queue) {
		rtos_deinit_queue(&g_sensor_info.msg_queue);
		g_sensor_info.msg_queue = NULL;
	}

	if (NULL != g_sensor_info.task_handle) {
		rtos_delete_thread(g_sensor_info.task_handle);
		g_sensor_info.task_handle = NULL;
	}
#endif
	return ret;
}

int bk_sensor_send_msg(uint32_t msg_type)
{
	bk_err_t ret = BK_FAIL;

	TEMPD_LOGD("send msg(%d)\n", msg_type);
#if (CONFIG_TEMP_DETECT || CONFIG_VOLT_DETECT)
	if (g_sensor_info.msg_queue) {
		tempd_msg_t msg;
		msg.temp_msg = msg_type;

		ret = rtos_push_to_queue(&g_sensor_info.msg_queue, &msg, BEKEN_NO_WAIT);
		if (kNoErr != ret)
			TEMPD_LOGE("send msg failed ret:%x,m_t:%x\r\n", ret, msg_type);
	}
#endif

	return (int)ret;
}

#if (CONFIG_TEMP_DETECT)
bk_err_t bk_sensor_set_current_temperature(float temperature)
{
	if (NULL == g_sensor_info.lock)
	{
		return BK_ERR_NOT_INIT;
	}

	rtos_lock_mutex(&g_sensor_info.lock);
	g_sensor_info.temperature = temperature;
	rtos_unlock_mutex(&g_sensor_info.lock);

	return BK_OK;
}

bk_err_t bk_sensor_get_current_temperature(float *temperature)
{
	if (NULL == g_sensor_info.lock)
	{
		return BK_ERR_NOT_INIT;
	}

	if (isnan(g_sensor_info.temperature))
	{
		return BK_ERR_TRY_AGAIN;
	}

	if (NULL == temperature)
	{
		return BK_ERR_PARAM;
	}

	if(rtos_local_irq_disabled() || rtos_is_in_interrupt_context())
	{
		*temperature = g_sensor_info.temperature;
	}
	else
	{
		rtos_lock_mutex(&g_sensor_info.lock);
		*temperature = g_sensor_info.temperature;
		rtos_unlock_mutex(&g_sensor_info.lock);
	}

	return BK_OK;
}

bk_err_t bk_sensor_register_temperature_callback(bk_sensor_callback callback)
{
	int exist_index = 0;
	int empty_index = -1;

	rtos_lock_mutex(&g_sensor_info.lock);
	for (; exist_index < TEMPD_MAX_CALLBACK_NUM; exist_index++) {
		if (callback == g_sensor_info.temp_callbacks[exist_index]) {
			break;
		} else if (NULL != g_sensor_info.temp_callbacks[exist_index]) {
			continue;
		} else if (empty_index < 0) {
			empty_index = exist_index;
		}
	}
	rtos_unlock_mutex(&g_sensor_info.lock);

	if (exist_index < TEMPD_MAX_CALLBACK_NUM) {
		return BK_OK;
	} else if (empty_index < 0) {
		return BK_ERR_IS_FALL; //typo for full
	}

	g_sensor_info.temp_callbacks[empty_index] = callback;
	return BK_OK;
}

int bk_sensor_unregister_temperature_callback(bk_sensor_callback callback)
{
	int index = 0;

	rtos_lock_mutex(&g_sensor_info.lock);
	for (; index < TEMPD_MAX_CALLBACK_NUM; index++) {
		if (callback == g_sensor_info.temp_callbacks[index]) {
			g_sensor_info.temp_callbacks[index] = NULL;
			break;
		}
	}
	rtos_unlock_mutex(&g_sensor_info.lock);

	if (index < TEMPD_MAX_CALLBACK_NUM) {
		return BK_OK;
	}

	return BK_ERR_NOT_FOUND;
}

bk_err_t bk_sensor_traversal_temperature_callback(uint16_t adc_code, float temp_code, float temp_last)
{
	int index = 0;

	rtos_lock_mutex(&g_sensor_info.lock);
	for (; index < TEMPD_MAX_CALLBACK_NUM; index++) {
		if (NULL == g_sensor_info.temp_callbacks[index]) {
			continue;
		}
		g_sensor_info.temp_callbacks[index](adc_code, temp_code, temp_last);
	}
	rtos_unlock_mutex(&g_sensor_info.lock);

	return BK_OK;
}
#endif

#if (CONFIG_VOLT_DETECT)
bk_err_t bk_sensor_set_current_voltage(float voltage)
{
	if (NULL == g_sensor_info.lock)
	{
		return BK_ERR_NOT_INIT;
	}

	rtos_lock_mutex(&g_sensor_info.lock);
	g_sensor_info.voltage = voltage;
	rtos_unlock_mutex(&g_sensor_info.lock);

	return BK_OK;
}

bk_err_t bk_sensor_get_current_voltage(float *voltage)
{
	if (NULL == g_sensor_info.lock)
	{
		return BK_ERR_NOT_INIT;
	}

	if (isnan(g_sensor_info.voltage))
	{
		return BK_ERR_TRY_AGAIN;
	}

	if (NULL == voltage)
	{
		return BK_ERR_PARAM;
	}

	rtos_lock_mutex(&g_sensor_info.lock);
	*voltage = g_sensor_info.voltage;
	rtos_unlock_mutex(&g_sensor_info.lock);

	return BK_OK;
}

bk_err_t bk_sensor_register_voltage_callback(bk_sensor_callback callback)
{
	int exist_index = 0;
	int empty_index = -1;

	rtos_lock_mutex(&g_sensor_info.lock);
	for (; exist_index < VOLTD_MAX_CALLBACK_NUM; exist_index++) {
		if (callback == g_sensor_info.volt_callbacks[exist_index]) {
			break;
		} else if (NULL != g_sensor_info.volt_callbacks[exist_index]) {
			continue;
		} else if (empty_index < 0) {
			empty_index = exist_index;
		}
	}
	rtos_unlock_mutex(&g_sensor_info.lock);

	if (exist_index < VOLTD_MAX_CALLBACK_NUM) {
		return BK_OK;
	} else if (empty_index < 0) {
		return BK_ERR_IS_FALL; //typo for full
	}

	g_sensor_info.volt_callbacks[empty_index] = callback;
	return BK_OK;
}

bk_err_t bk_sensor_unregister_voltage_callback(bk_sensor_callback callback)
{
	int index = 0;

	rtos_lock_mutex(&g_sensor_info.lock);
	for (; index < VOLTD_MAX_CALLBACK_NUM; index++) {
		if (callback == g_sensor_info.volt_callbacks[index]) {
			g_sensor_info.volt_callbacks[index] = NULL;
			break;
		}
	}
	rtos_unlock_mutex(&g_sensor_info.lock);

	if (index < VOLTD_MAX_CALLBACK_NUM) {
		return BK_OK;
	}

	return BK_ERR_NOT_FOUND;
}

bk_err_t bk_sensor_traversal_voltage_callback(uint16_t adc_code, float volt_code, float volt_last)
{
	int index = 0;

	rtos_lock_mutex(&g_sensor_info.lock);
	for (; index < VOLTD_MAX_CALLBACK_NUM; index++) {
		if (NULL == g_sensor_info.volt_callbacks[index]) {
			continue;
		}
		g_sensor_info.volt_callbacks[index](adc_code, volt_code, volt_last);
	}
	rtos_unlock_mutex(&g_sensor_info.lock);

	return BK_OK;
}
#endif
