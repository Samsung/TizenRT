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

#include <driver/int.h>
#include <os/os.h>
#include <os/mem.h>
#include <os/str.h>
#include <driver/gpio.h>
#include <driver/gpio_types.h>
#include <gpio_map.h>
#include "gpio_driver.h"
#include <driver/i2c.h>
#include <driver/media_types.h>
#include <driver/tp.h>
#include <driver/tp_types.h>
#include "driver/drv_tp.h"
#include <common/bk_compiler.h>


#define TAG "tp_drv"
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)


// power definition.
#ifndef TP_POWER_CTRL_EN
	#define TP_POWER_CTRL_EN (0)
#endif
#ifndef TP_POWER_CTRL_ACTIVE_LEVEL
	#define TP_POWER_CTRL_ACTIVE_LEVEL (1)
#endif
#ifndef TP_POWER_CTRL_GPIO_ID
	#define TP_POWER_CTRL_GPIO_ID (GPIO_38)
#endif

// pin definition.
#ifndef TP_RST_GPIO_ID
	#define TP_RST_GPIO_ID (GPIO_9)
#endif
#ifndef TP_INT_GPIO_ID
	#define TP_INT_GPIO_ID (GPIO_6)
#endif

// i2c parameters.
#define TP_I2C_INIT_INNER (1)
#define TP_I2C_ID (1)
#define TP_I2C_TIMEOUT (2000)  // 2s

// task parameters.
#define TP_THREAD_PRIORITY   (4)
#define TP_THREAD_STACK_SIZE (1024)


static beken_semaphore_t tp_sema = NULL;
static beken_thread_t tp_thread_handle = NULL;

static const tp_sensor_config_t *current_sensor = NULL;
static tp_device_t tp_device = {0};
volatile static uint32_t tp_driver_init_flag = false;
static const tp_sensor_config_t **tp_sensor_devices_list;
static uint16_t tp_sensor_devices_size;

static const tp_i2c_callback_t tp_i2c_cb =
{
	tp_i2c_read_uint8,
	tp_i2c_write_uint8,
	tp_i2c_read_uint16,
	tp_i2c_write_uint16,
};

const tp_sensor_config_t **get_tp_sensor_devices_list(void)
{
    return tp_sensor_devices_list;
}

uint32_t get_tp_sensor_devices_num(void)
{
    return tp_sensor_devices_size;
}

const tp_sensor_config_t *tp_get_sensor_config_interface_by_id(tp_sensor_id_t id)
{
    uint8_t i;

	for (i = 0; i < tp_sensor_devices_size; i++)
	{
		if (tp_sensor_devices_list[i]->id == id)
		{
			return tp_sensor_devices_list[i];
		}
	}

	return NULL;
}

const tp_sensor_config_t *tp_get_sensor_auto_detect(const tp_i2c_callback_t *cb)
{
    uint8_t i;

	if(NULL == cb)
	{
		LOGE("%s, pointer is null!\r\n", __func__);
		return NULL;
	}

	for (i = 0; i < tp_sensor_devices_size; i++)
	{
		if (NULL != tp_sensor_devices_list[i]->detect)
		{
			if (true == tp_sensor_devices_list[i]->detect(cb))
			{
				return tp_sensor_devices_list[i];
			}
		}
	}

	return NULL;
}

void bk_tp_set_sensor_devices_list(const tp_sensor_config_t **list, uint16_t size)
{
    tp_sensor_devices_list = list;
    tp_sensor_devices_size = size;

}

int tp_i2c_read_uint8(uint8_t addr, uint8_t reg, uint8_t *buff, uint16_t len)
{
	if(NULL == buff)
	{
		LOGE("%s, pointer is null!\r\n", __func__);
		return BK_FAIL;
	}

	if(0 == len)
	{
		LOGE("%s, len is 0!\r\n", __func__);
		return BK_FAIL;
	}

	i2c_mem_param_t mem_param = {0};

	mem_param.dev_addr = addr;
	mem_param.mem_addr = reg;
	mem_param.mem_addr_size = I2C_MEM_ADDR_SIZE_8BIT;
	mem_param.data = buff;
	mem_param.data_size = len;
	mem_param.timeout_ms = TP_I2C_TIMEOUT;

#if defined(CONFIG_SIM_I2C_HW_BOARD_V3)
	return bk_i2c_memory_read_v2(TP_I2C_ID, &mem_param);
#else
	return bk_i2c_memory_read(TP_I2C_ID, &mem_param);
#endif
}

int tp_i2c_write_uint8(uint8_t addr, uint8_t reg, uint8_t *buff, uint16_t len)
{
	if(NULL == buff)
	{
		LOGE("%s, pointer is null!\r\n", __func__);
		return BK_FAIL;
	}

	if(0 == len)
	{
		LOGE("%s, len is 0!\r\n", __func__);
		return BK_FAIL;
	}
	
	i2c_mem_param_t mem_param = {0};

	mem_param.dev_addr = addr;
	mem_param.mem_addr = reg;
	mem_param.mem_addr_size = I2C_MEM_ADDR_SIZE_8BIT;
	mem_param.data = buff;
	mem_param.data_size = len;
	mem_param.timeout_ms = TP_I2C_TIMEOUT;

#if defined(CONFIG_SIM_I2C_HW_BOARD_V3)
	return bk_i2c_memory_write_v2(TP_I2C_ID, &mem_param);
#else
	return bk_i2c_memory_write(TP_I2C_ID, &mem_param);
#endif
}

int tp_i2c_read_uint16(uint8_t addr, uint16_t reg, uint8_t *buff, uint16_t len)
{
	if(NULL == buff)
	{
		LOGE("%s, pointer is null!\r\n", __func__);
		return BK_FAIL;
	}

	if(0 == len)
	{
		LOGE("%s, len is 0!\r\n", __func__);
		return BK_FAIL;
	}

	i2c_mem_param_t mem_param = {0};

	mem_param.dev_addr = addr;
	mem_param.mem_addr = reg;
	mem_param.mem_addr_size = I2C_MEM_ADDR_SIZE_16BIT;
	mem_param.data = buff;
	mem_param.data_size = len;
	mem_param.timeout_ms = TP_I2C_TIMEOUT;

#if defined(CONFIG_SIM_I2C_HW_BOARD_V3)
	return bk_i2c_memory_read_v2(TP_I2C_ID, &mem_param);
#else
	return bk_i2c_memory_read(TP_I2C_ID, &mem_param);
#endif
}

int tp_i2c_write_uint16(uint8_t addr, uint16_t reg, uint8_t *buff, uint16_t len)
{
	if(NULL == buff)
	{
		LOGE("%s, pointer is null!\r\n", __func__);
		return BK_FAIL;
	}

	if(0 == len)
	{
		LOGE("%s, len is 0!\r\n", __func__);
		return BK_FAIL;
	}
	
	i2c_mem_param_t mem_param = {0};

	mem_param.dev_addr = addr;
	mem_param.mem_addr = reg;
	mem_param.mem_addr_size = I2C_MEM_ADDR_SIZE_16BIT;
	mem_param.data = buff;
	mem_param.data_size = len;
	mem_param.timeout_ms = TP_I2C_TIMEOUT;

#if defined(CONFIG_SIM_I2C_HW_BOARD_V3)
	return bk_i2c_memory_write_v2(TP_I2C_ID, &mem_param);
#else
	return bk_i2c_memory_write(TP_I2C_ID, &mem_param);
#endif
}

#if (TP_POWER_CTRL_EN > 0)
// tp power control initialization.
bk_err_t bk_tp_power_ctrl_init(const tp_config_t *config)
{
	if(NULL == config)
	{
		LOGE("%s, pointer is null!\r\n", __func__);
		return BK_FAIL;
	}

	gpio_config_t mode = {0};
	gpio_id_t power_ctr_id = TP_POWER_CTRL_GPIO_ID;

	// INT GPIO - output low
	BK_LOG_ON_ERR(gpio_dev_unmap(power_ctr_id));
	mode.io_mode = GPIO_OUTPUT_ENABLE;
	mode.pull_mode = GPIO_PULL_DISABLE;
	BK_LOG_ON_ERR(bk_gpio_set_config(power_ctr_id, &mode));
	#if (TP_POWER_CTRL_ACTIVE_LEVEL > 0)
		BK_LOG_ON_ERR(bk_gpio_set_output_high(power_ctr_id));
	#else
		BK_LOG_ON_ERR(bk_gpio_set_output_low(power_ctr_id));
	#endif
	// this delay time maybe can optimization.
	rtos_delay_milliseconds(20);

	return BK_OK;
}
#endif

// tp gpio initialization and including sensor address select through controling gpio level.
bk_err_t bk_tp_gpio_init(const tp_config_t *config)
{
	if(NULL == config)
	{
		LOGE("%s, pointer is null!\r\n", __func__);
		return BK_FAIL;
	}

	gpio_config_t mode = {0};
	gpio_id_t rst_id = TP_RST_GPIO_ID;
	gpio_id_t int_id = TP_INT_GPIO_ID;

	// INT GPIO - output high
	BK_LOG_ON_ERR(gpio_dev_unmap(int_id));
	mode.io_mode = GPIO_OUTPUT_ENABLE;
	mode.pull_mode = GPIO_PULL_DISABLE;
	BK_LOG_ON_ERR(bk_gpio_set_config(int_id, &mode));
	BK_LOG_ON_ERR(bk_gpio_set_output_high(int_id));	

	// RESET GPIO - output low
	BK_LOG_ON_ERR(gpio_dev_unmap(rst_id));
	mode.io_mode = GPIO_OUTPUT_ENABLE;
	mode.pull_mode = GPIO_PULL_DISABLE;
	BK_LOG_ON_ERR(bk_gpio_set_config(rst_id, &mode));
	BK_LOG_ON_ERR(bk_gpio_set_output_low(rst_id));	

	// this delay time maybe can optimization.
	#if defined(CONFIG_TP_HY4633)
		rtos_delay_milliseconds(220);
	#elif defined(CONFIG_TP_FT6336)
		rtos_delay_milliseconds(20);
	#else
		rtos_delay_milliseconds(2);
	#endif
	
    BK_LOG_ON_ERR(bk_gpio_set_output_high(rst_id));
	
	return BK_OK;
}

// tp i2c communication initialization.
bk_err_t bk_tp_i2c_init(const tp_config_t *config)
{
	if(NULL == config)
	{
		LOGE("%s, pointer is null!\r\n", __func__);
		return BK_FAIL;
	}

	i2c_config_t i2c_config = {0};

	i2c_config.baud_rate = I2C_BAUD_RATE_100KHZ;
	i2c_config.addr_mode = I2C_ADDR_MODE_7BIT;
	if (BK_OK != bk_i2c_init(TP_I2C_ID, &i2c_config))
	{
		LOGE("%s, I2C%d init fail!\r\n", TP_I2C_ID);
		return BK_FAIL;
	}

	return BK_OK;
}

// tp interrupt service routine.
static void tp_int_gpio_isr(gpio_id_t id, void *priv)
{
	// LOGI("tp int isr index:%d\n", id);

	if (false != tp_driver_init_flag)
	{
		bk_gpio_disable_interrupt(TP_INT_GPIO_ID);
		rtos_set_semaphore(&tp_sema);
	}
}

__bk_weak void bk_tp_read_info_callback(tp_data_t *tp_data)
{
#if 0
	static uint32_t hisTimeStamp = 0;

	// write tp data to queue
	if (tp_data->event == TP_EVENT_TYPE_DOWN)
	{
		hisTimeStamp = tp_data->timestamp;
		drv_tp_write(tp_data->x_coordinate, tp_data->y_coordinate, 1);
	}
	else if (tp_data->event == TP_EVENT_TYPE_MOVE)
	{
		if ( (tp_data->timestamp - hisTimeStamp) >= 5 )
		{
			hisTimeStamp = tp_data->timestamp;
			drv_tp_write(tp_data->x_coordinate, tp_data->y_coordinate, 1);
		}
	}
	else if (tp_data->event == TP_EVENT_TYPE_UP)
	{
		drv_tp_write(tp_data->x_coordinate, tp_data->y_coordinate, 0);
	}
#endif
}

// tp interrupt initialization.
bk_err_t bk_tp_int_init(const tp_config_t *config)
{
	if(NULL == config)
	{
		LOGE("%s, pointer is null!\r\n", __func__);
		return BK_FAIL;
	}

	gpio_config_t mode = {0};
	gpio_id_t int_id = TP_INT_GPIO_ID;
	gpio_int_type_t int_type = 0;

	BK_LOG_ON_ERR(gpio_dev_unmap(int_id));
	mode.io_mode = GPIO_INPUT_ENABLE;
	mode.pull_mode = GPIO_PULL_DISABLE;
	BK_LOG_ON_ERR(bk_gpio_set_config(int_id, &mode));

	if (TP_INT_TYPE_RISING_EDGE == config->int_type)
	{
		int_type = GPIO_INT_TYPE_RISING_EDGE;
	}
	else if (TP_INT_TYPE_FALLING_EDGE == config->int_type)
	{
		int_type = GPIO_INT_TYPE_FALLING_EDGE;
	}
	else if (TP_INT_TYPE_LOW_LEVEL == config->int_type)
	{
		int_type = GPIO_INT_TYPE_LOW_LEVEL;
	}	
	else if (TP_INT_TYPE_HIGH_LEVEL == config->int_type)
	{
		int_type = GPIO_INT_TYPE_HIGH_LEVEL;
	}	
    BK_LOG_ON_ERR(bk_gpio_register_isr_ex(int_id, tp_int_gpio_isr, NULL));
    BK_LOG_ON_ERR(bk_gpio_set_interrupt_type(int_id, int_type));
    BK_LOG_ON_ERR(bk_gpio_enable_interrupt(int_id));

	return BK_OK;
}

bk_err_t bk_tp_int_deinit(void)
{
	gpio_id_t int_id = TP_INT_GPIO_ID;

	BK_LOG_ON_ERR(bk_gpio_disable_interrupt(int_id));

	return BK_OK;
}

void tp_process_task(beken_thread_arg_t arg)
{
	int ret;
	tp_data_t tp_data[TP_SUPPORT_MAX_NUM];

	while (1)
	{
		ret = rtos_get_semaphore(&tp_sema, BEKEN_NEVER_TIMEOUT);
		if(kNoErr != ret)
		{
			LOGE("%s, get semaphore fail!\r\n", __func__);
		}

		os_memset(tp_data, 0x00, sizeof(tp_data));
		if (NULL != current_sensor->read_tp_info)
		{
			if (BK_OK != current_sensor->read_tp_info(&tp_i2c_cb, TP_SUPPORT_MAX_NUM, (void *)tp_data))
			{
				LOGE("%s get tp info fail!\r\n", __func__);
			}
			else
			{
				for (uint8_t i=0; i<TP_SUPPORT_MAX_NUM; i++)
				{
					if ((TP_EVENT_TYPE_DOWN == tp_data[i].event) || (TP_EVENT_TYPE_UP == tp_data[i].event) || (TP_EVENT_TYPE_MOVE == tp_data[i].event))
					{
						LOGD("event=%d, track_id=%d, x=%d, y=%d, s=%d, timestamp=%u.\r\n", 
									tp_data[i].event,
									tp_data[i].track_id,
									tp_data[i].x_coordinate,
									tp_data[i].y_coordinate,
									tp_data[i].width,
									tp_data[i].timestamp);
					}

					bk_tp_read_info_callback(&tp_data[i]);
				}
			}
		}

		BK_LOG_ON_ERR(bk_gpio_enable_interrupt(TP_INT_GPIO_ID));
	}
}

bk_err_t bk_tp_driver_init(tp_config_t *config)
{
	if(NULL == config)
	{
		LOGE("%s, pointer is null!\r\n", __func__);
		return BK_FAIL;
	}

	if ( false != tp_driver_init_flag)
	{
		LOGE("%s, reinit is denied!\r\n", __func__);
		return BK_FAIL;
	}

	int ret = BK_OK;
	tp_sensor_user_config_t sensor_user_config = {0};

	LOGI("%s, ppi=%d, int_type=%d, refresh_rate=%d, tp_num=%d.\r\n", __func__, config->ppi, config->int_type, config->refresh_rate, config->tp_num);

	#if (TP_POWER_CTRL_EN > 0)
		// power control initialization.
		if (BK_OK != bk_tp_power_ctrl_init((const tp_config_t *)config))
		{
			LOGE("%s, power ctrl init fail!\r\n", __func__);
			return BK_FAIL;
		}
	#endif

	// gpio initialization.
	if (BK_OK != bk_tp_gpio_init((const tp_config_t *)config))
	{
		LOGE("%s, gpio init fail!\r\n", __func__);
		return BK_FAIL;
	}

	#if (TP_I2C_INIT_INNER > 0)
	// i2c initialization.
	if (BK_OK != bk_tp_i2c_init((const tp_config_t *)config))
	{
		LOGE("%s, i2c init fail!\r\n", __func__);
		return BK_FAIL;
	}
	#endif

	rtos_delay_milliseconds(10);

	ret = rtos_init_semaphore_ex(&tp_sema, 1, 0);
	if(kNoErr != ret)
	{
		LOGE("%s semaphore init fail!\r\n", __func__);
		return ret;
	}

	ret = rtos_create_thread(&tp_thread_handle, TP_THREAD_PRIORITY, "tp", (beken_thread_function_t)tp_process_task, TP_THREAD_STACK_SIZE, (beken_thread_arg_t)0); 	
	if(kNoErr != ret)
	{
		LOGE("%s create thread fail!\r\n", __func__);
		goto error;
	}

	if (BK_OK != bk_tp_int_init((const tp_config_t *)config))
	{
		LOGE("%s tp int init fail!\r\n", __func__);
		return BK_FAIL;
	}

	#if defined(CONFIG_TP_HY4633)
		rtos_delay_milliseconds(1000);
	#elif defined(CONFIG_TP_FT6336)
		rtos_delay_milliseconds(300);
	#else
		rtos_delay_milliseconds(100);
	#endif

	current_sensor = tp_get_sensor_auto_detect(&tp_i2c_cb);
	if (NULL == current_sensor)
	{
		LOGE("%s no tp sensor found!\r\n", __func__);
		return BK_FAIL;
	}

	if (NULL != current_sensor->init)
	{
		sensor_user_config.x_size = (config->ppi >> 16) & 0xFFFF;
		sensor_user_config.y_size = config->ppi & 0xFFFF;
		sensor_user_config.int_type = config->int_type;
		sensor_user_config.refresh_rate = config->refresh_rate;
		sensor_user_config.tp_num = config->tp_num;

		if (BK_OK != current_sensor->init(&tp_i2c_cb, &sensor_user_config))
		{
			LOGE("%s sensor init fail!\r\n", __func__);
			return BK_FAIL;
		}
	}

	// device parameters.
	tp_device.name = current_sensor->name;
	tp_device.id = current_sensor->id;
	tp_device.ppi = config->ppi;
	tp_device.int_type = config->int_type;
	tp_device.refresh_rate = config->refresh_rate;
	tp_device.tp_num = config->tp_num;
	LOGI("%s, name=%s, id=%d, ppi=%d, x_size=%d, y_size=%d, int_type=%d, refresh_rate=%d, tp_num=%d.\r\n", __func__, tp_device.name, tp_device.id, tp_device.ppi, sensor_user_config.x_size, sensor_user_config.y_size, tp_device.int_type, tp_device.refresh_rate, tp_device.tp_num);

	// initilization finish.
	tp_driver_init_flag = true;

	return ret;

error:
	if (NULL != tp_sema)
	{
		if (kNoErr != rtos_deinit_semaphore(&tp_sema))
		{
			LOGE("%s deinit semaaphore fail!\r\n", __func__);
		}
		tp_sema = NULL;
	}

	return ret;
}

bk_err_t bk_tp_driver_deinit(void)
{
	int ret = BK_OK;

	if (NULL != tp_sema)
	{
		if (kNoErr != rtos_deinit_semaphore(&tp_sema))
		{
			LOGE("%s deinit semaphore fail!\r\n", __func__);
			ret = BK_FAIL;
		}
		tp_sema = NULL;
	}

	if (NULL != tp_thread_handle)
	{
		if (kNoErr != rtos_delete_thread(&tp_thread_handle))
		{
			LOGE("%s delete thread fail!\r\n", __func__);
			ret = BK_FAIL;
		}
		tp_thread_handle = NULL;
	}

	bk_tp_int_deinit();

	current_sensor = NULL;
	os_memset(&tp_device, 0x00, sizeof(tp_device_t));

	tp_driver_init_flag = false;

	return ret;
}

tp_device_t *bk_tp_get_device(void)
{
	return (&tp_device);
}
