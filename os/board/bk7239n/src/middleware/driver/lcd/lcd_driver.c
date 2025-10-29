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

#include <stdlib.h>
#include <common/bk_include.h>
#include <os/mem.h>
#include <os/str.h>
#include <math.h>
#include "arch_interrupt.h"
#include "lcd_disp_hal.h"
#include <driver/lcd.h>
#include "gpio_driver.h"
#include <driver/gpio.h>
#include "gpio_map.h"
#include <driver/int.h>
#include "sys_driver.h"
#include <driver/hal/hal_gpio_types.h>
#include <driver/hal/hal_lcd_types.h>
#include <driver/timer.h>
#include <driver/lcd_spi.h>
#include <driver/pwm.h>
#include <driver/gpio.h>
#include "driver/lcd.h"
#include "driver/pwr_clk.h"

#if defined(CONFIG_LCD_QSPI)
#include <driver/lcd_qspi.h>
#include <driver/lcd_qspi_types.h>
#endif

#define TAG "lcd_drv"

#if defined(CONFIG_SOC_BK7256XX)
#define MINOOR_ITCM __attribute__((section(".itcm_sec_code ")))
#else
#define MINOOR_ITCM
#endif

#if defined(CONFIG_SOC_BK7256XX)
#define LCD_BACKLIGHT_PWM_RGB       PWM_ID_1
#define LCD_BACKLIGHT_PWM_QSPI      PWM_ID_2
#define LCD_BACKLIGHT_GPIO_RGB      GPIO_7
#define LCD_BACKLIGHT_GPIO_QSPI     GPIO_8
#endif

#if defined(CONFIG_SOC_BK7236XX)
#define LCD_BACKLIGHT_PWM           PWM_ID_1
#define LCD_BACKLIGHT_GPIO          GPIO_7
#endif

#define IO_FUNCTION_ENABLE(pin, func)   \
	do {                                \
		gpio_dev_unmap(pin);            \
		gpio_dev_map(pin, func);        \
		bk_gpio_enable_output(pin); 	\
		bk_gpio_set_capacity(pin,GPIO_DRIVER_CAPACITY_1);	\
	} while (0)

#define IO_FUNCTION_ENABLE_I8080(pin, func)   \
	do {                                \
		gpio_dev_unmap(pin);            \
		gpio_dev_map(pin, func);        \
		bk_gpio_enable_output(pin);     \
		bk_gpio_set_capacity(pin,GPIO_DRIVER_CAPACITY_3);    \
	} while (0)

//set high impedance
#define IO_FUNCTION_UNMAP(pin)   \
        do {                                \
            gpio_dev_unmap(pin);            \
        } while (0)

#define LCD_RETURN_ON_NOT_INIT() do {\
		if (!s_lcd_driver_is_init) {\
			return BK_ERR_LCD_NOT_INIT;\
		}\
	} while(0)
#define BK_RETURN_ON_NULL(_x) do {\
	if (!(_x)) {\
		BK_LOGE(ERR_TAG, "Null %s\n", __FUNCTION__);\
		return BK_ERR_NULL_PARAM;\
	}\
} while(0)

static bool s_lcd_driver_is_init = false;

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

extern u64 riscv_get_mtimer(void);

typedef struct
{
#if (USE_LCD_REGISTER_CALLBACKS == 1)  //register callback
	lcd_isr_t lcd_8080_frame_start_handler;
	lcd_isr_t lcd_8080_frame_end_handler;
	lcd_isr_t lcd_rgb_frame_end_handler;
	lcd_isr_t lcd_rgb_frame_start_handler;
	lcd_isr_t lcd_rgb_de_handler;
	lcd_isr_t lcd_rgb_frame_interval_handler;
#endif
	const lcd_device_t device;  /**< lcd device config */
} lcd_driver_t;

static lcd_driver_t s_lcd = {0};
static const lcd_device_t **devices_list = NULL;
static uint16_t devices_size = 0;

const lcd_device_t **get_lcd_devices_list(void)
{
	return devices_list;
}

uint32_t get_lcd_devices_num(void)
{
	return devices_size;
}

const lcd_device_t * get_lcd_device_by_name(char * name)
{
	uint32_t i;

	LOGI("%s, devices: %d\n", __func__, devices_size);

	for (i = 0; i < devices_size; i++)
	{
		if (os_strcmp(devices_list[i]->name, name) == 0)
		{
			LOGI("%s, name: %s\n", __func__, devices_list[i]->name);
			return devices_list[i];
		}
	}
	return NULL;
}

const lcd_device_t *get_lcd_device_by_ppi(media_ppi_t ppi)
{
	uint32_t i;

	LOGI("%s, devices: %d\n", __func__, devices_size);

	for (i = 0; i < devices_size; i++)
	{
		if (devices_list[i]->ppi == ppi)
		{
			LOGI("%s, name: %s\n", __func__, devices_list[i]->name);
			return devices_list[i];
		}
	}

	return NULL;
}

const lcd_device_t *get_lcd_device_by_id(lcd_device_id_t id)
{
	uint32_t i;

	for (i = 0; i < devices_size; i++)
	{
		if (devices_list[i]->id == id)
		{
			return devices_list[i];
		}
	}

	return NULL;
}

void bk_lcd_set_devices_list(const lcd_device_t **list, uint16_t size)
{
	devices_list = list;
	devices_size = size;
}


int32_t lcd_driver_get_spi_gpio(LCD_SPI_GPIO_TYPE_E gpio_type)
{
    int32_t gpio_value = 0;
    switch(gpio_type)
    {
        case SPI_GPIO_CLK:
            if(s_lcd.device.id == LCD_DEVICE_ST7701S)   //480X480
            {
                #if defined(CONFIG_SOC_BK7256XX)
                gpio_value = GPIO_35;
                #elif defined(CONFIG_SOC_BK7236XX)
                gpio_value = GPIO_0;
                #endif
            }
            else if (s_lcd.device.id == LCD_DEVICE_NT35510) //custom
            {
                gpio_value = GPIO_2;
            }
			else 
            {
#if defined(CONFIG_SOC_BK7256)
				gpio_value = GPIO_9;
#else   //bk7258
				gpio_value = GPIO_0;
#endif
           	}
            break;
        case SPI_GPIO_CSX:
            if(s_lcd.device.id == LCD_DEVICE_ST7701S)
            {
                #if defined(CONFIG_SOC_BK7256XX)
                gpio_value = GPIO_34;
                #elif defined(CONFIG_SOC_BK7236XX)
                gpio_value = GPIO_12;
                #endif
            }
            else if (s_lcd.device.id == LCD_DEVICE_NT35510) //custom
            {
                gpio_value = GPIO_3;
            }
            else
            {
                gpio_value = GPIO_12;
            }
            break;
        case SPI_GPIO_SDA:
            if(s_lcd.device.id == LCD_DEVICE_ST7701S) {
                #if defined(CONFIG_SOC_BK7256XX)
                gpio_value = GPIO_36;
                #elif defined(CONFIG_SOC_BK7236XX)
                gpio_value = GPIO_1;
                #endif
            } else if (s_lcd.device.id == LCD_DEVICE_NT35510) //custom
				gpio_value = GPIO_4;
            else
			{
#if defined(CONFIG_SOC_BK7256)
				gpio_value = GPIO_8;
#else
				gpio_value = GPIO_1;
#endif
			}
            break;
        case SPI_GPIO_RST:
            if(s_lcd.device.id == LCD_DEVICE_ST7701S)
                gpio_value = GPIO_15;
            else
                gpio_value = GPIO_6;
            break;
        default:
            LOGE("%s can't support this gpio type:%d\r\n", __FUNCTION__, __LINE__);
            break;
    }

    return gpio_value;
}

bk_err_t lcd_mcu_gpio_init(void)
{
	LOGI("%s\n", __func__);

	IO_FUNCTION_ENABLE_I8080(LCD_MCU_D0_PIN, LCD_MCU_D0_FUNC);
	IO_FUNCTION_ENABLE_I8080(LCD_MCU_D1_PIN, LCD_MCU_D1_FUNC);
	IO_FUNCTION_ENABLE_I8080(LCD_MCU_D2_PIN, LCD_MCU_D2_FUNC);
	IO_FUNCTION_ENABLE_I8080(LCD_MCU_D3_PIN, LCD_MCU_D3_FUNC);
	IO_FUNCTION_ENABLE_I8080(LCD_MCU_D4_PIN, LCD_MCU_D4_FUNC);
	IO_FUNCTION_ENABLE_I8080(LCD_MCU_D5_PIN, LCD_MCU_D5_FUNC);
	IO_FUNCTION_ENABLE_I8080(LCD_MCU_D6_PIN, LCD_MCU_D6_FUNC);
	IO_FUNCTION_ENABLE_I8080(LCD_MCU_D7_PIN, LCD_MCU_D7_FUNC);
#if defined(CONFIG_SOC_BK7236XX)
	IO_FUNCTION_ENABLE_I8080(LCD_MCU_D8_PIN , LCD_MCU_D8_FUNC );
	IO_FUNCTION_ENABLE_I8080(LCD_MCU_D9_PIN , LCD_MCU_D9_FUNC );
	IO_FUNCTION_ENABLE_I8080(LCD_MCU_D10_PIN, LCD_MCU_D10_FUNC);
	IO_FUNCTION_ENABLE_I8080(LCD_MCU_D11_PIN, LCD_MCU_D11_FUNC);
	IO_FUNCTION_ENABLE_I8080(LCD_MCU_D12_PIN, LCD_MCU_D12_FUNC);
	IO_FUNCTION_ENABLE_I8080(LCD_MCU_D13_PIN, LCD_MCU_D13_FUNC);
	IO_FUNCTION_ENABLE_I8080(LCD_MCU_D14_PIN, LCD_MCU_D14_FUNC);
	IO_FUNCTION_ENABLE_I8080(LCD_MCU_D15_PIN, LCD_MCU_D15_FUNC);
	IO_FUNCTION_ENABLE_I8080(LCD_MCU_D16_PIN, LCD_MCU_D16_FUNC);
	IO_FUNCTION_ENABLE_I8080(LCD_MCU_D17_PIN, LCD_MCU_D17_FUNC);
#endif
	IO_FUNCTION_ENABLE_I8080(LCD_MCU_RDX_PIN, LCD_MCU_RDX_FUNC);
	IO_FUNCTION_ENABLE_I8080(LCD_MCU_WRX_PIN, LCD_MCU_WRX_FUNC);
	IO_FUNCTION_ENABLE_I8080(LCD_MCU_RSX_PIN, LCD_MCU_RSX_FUNC);
	IO_FUNCTION_ENABLE_I8080(LCD_MCU_RESET_PIN, LCD_MCU_RESET_FUNC);
	IO_FUNCTION_ENABLE_I8080(LCD_MCU_CSX_PIN, LCD_MCU_CSX_FUNC);

	return BK_OK;
}


static bk_err_t lcd_rgb_gpio_init(void)
{
	LOGI("%s\n", __func__);
#if defined(CONFIG_SOC_BK7236XX)
	IO_FUNCTION_ENABLE(LCD_RGB_R0_PIN, LCD_RGB_R0_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_R1_PIN, LCD_RGB_R1_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_R2_PIN, LCD_RGB_R2_FUNC);
#endif
	IO_FUNCTION_ENABLE(LCD_RGB_R3_PIN, LCD_RGB_R3_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_R4_PIN, LCD_RGB_R4_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_R5_PIN, LCD_RGB_R5_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_R6_PIN, LCD_RGB_R6_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_R7_PIN, LCD_RGB_R7_FUNC);
	
#if defined(CONFIG_SOC_BK7236XX)
	IO_FUNCTION_ENABLE(LCD_RGB_G0_PIN, LCD_RGB_G0_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_G1_PIN, LCD_RGB_G1_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_G2_PIN, LCD_RGB_G2_FUNC);
#endif
	IO_FUNCTION_ENABLE(LCD_RGB_G3_PIN, LCD_RGB_G3_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_G4_PIN, LCD_RGB_G4_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_G5_PIN, LCD_RGB_G5_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_G6_PIN, LCD_RGB_G6_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_G7_PIN, LCD_RGB_G7_FUNC);

#if defined(CONFIG_SOC_BK7236XX)
	IO_FUNCTION_ENABLE(LCD_RGB_B0_PIN, LCD_RGB_B0_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_B1_PIN, LCD_RGB_B1_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_B2_PIN, LCD_RGB_B2_FUNC);
	
#endif
	IO_FUNCTION_ENABLE(LCD_RGB_B3_PIN, LCD_RGB_B3_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_B4_PIN, LCD_RGB_B4_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_B5_PIN, LCD_RGB_B5_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_B6_PIN, LCD_RGB_B6_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_B7_PIN, LCD_RGB_B7_FUNC);

	IO_FUNCTION_ENABLE(LCD_RGB_CLK_PIN, LCD_RGB_CLK_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_DISP_PIN, LCD_RGB_DISP_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_HSYNC_PIN, LCD_RGB_HSYNC_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_VSYNC_PIN, LCD_RGB_VSYNC_FUNC);
	IO_FUNCTION_ENABLE(LCD_RGB_DE_PIN, LCD_RGB_DE_FUNC);

	return BK_OK;
}

bk_err_t bk_lcd_rgb_io_deinit(void)
{
    IO_FUNCTION_UNMAP(LCD_SPI_CLK_GPIO);
    IO_FUNCTION_UNMAP(LCD_SPI_CSX_GPIO);
    IO_FUNCTION_UNMAP(LCD_SPI_SDA_GPIO);
    IO_FUNCTION_UNMAP(LCD_SPI_RST);
    return BK_OK;
}


bk_err_t lcd_driver_backlight_open(void)
{
#if (defined(CONFIG_SOC_BK7256XX))
#if defined(CONFIG_PWM)
    BK_LOG_ON_ERR(bk_pwm_driver_init());
    pwm_init_config_t config = {0};
    config.period_cycle = 100;
    config.duty_cycle = 100;
    config.psc = 25;
    if (s_lcd.device.type == LCD_TYPE_QSPI) {
        BK_LOG_ON_ERR(bk_pwm_init(LCD_BACKLIGHT_PWM_QSPI, &config));
        BK_LOG_ON_ERR(bk_pwm_start(LCD_BACKLIGHT_PWM_QSPI));
    } else {
        BK_LOG_ON_ERR(bk_pwm_init(LCD_BACKLIGHT_PWM_RGB, &config));
        BK_LOG_ON_ERR(bk_pwm_start(LCD_BACKLIGHT_PWM_RGB));
    }
#else
    if (s_lcd.device.type == LCD_TYPE_QSPI) {
        gpio_dev_unmap(LCD_BACKLIGHT_GPIO_QSPI);
        BK_LOG_ON_ERR(bk_gpio_enable_output(LCD_BACKLIGHT_GPIO_QSPI));
        BK_LOG_ON_ERR(bk_gpio_pull_up(LCD_BACKLIGHT_GPIO_QSPI));
        bk_gpio_set_output_high(LCD_BACKLIGHT_GPIO_QSPI);
    } else {
        gpio_dev_unmap(LCD_BACKLIGHT_GPIO_RGB);
        BK_LOG_ON_ERR(bk_gpio_enable_output(LCD_BACKLIGHT_GPIO_RGB));
        BK_LOG_ON_ERR(bk_gpio_pull_up(LCD_BACKLIGHT_GPIO_RGB));
        bk_gpio_set_output_high(LCD_BACKLIGHT_GPIO_RGB);
    }
#endif
#elif (defined(CONFIG_SOC_BK7236XX))
#if defined(CONFIG_PWM)
    BK_LOG_ON_ERR(bk_pwm_driver_init());
    pwm_init_config_t config = {0};
    config.period_cycle = 100;
    config.duty_cycle = 100;
    config.psc = 25;
    BK_LOG_ON_ERR(bk_pwm_init(LCD_BACKLIGHT_PWM, &config));
    BK_LOG_ON_ERR(bk_pwm_start(LCD_BACKLIGHT_PWM));
#else
    gpio_dev_unmap(LCD_BACKLIGHT_GPIO);
    BK_LOG_ON_ERR(bk_gpio_enable_output(LCD_BACKLIGHT_GPIO));
    BK_LOG_ON_ERR(bk_gpio_pull_up(LCD_BACKLIGHT_GPIO));
    bk_gpio_set_output_high(LCD_BACKLIGHT_GPIO);
#endif
#endif

    return BK_OK;
}

bk_err_t lcd_driver_backlight_set(uint8_t percent)
{
#if defined(CONFIG_PWM)
    pwm_period_duty_config_t config = {0};

    if (percent > 100)
    {
        percent  = 100;
    }

    config.period_cycle = 100;
    config.duty_cycle = percent;

#if defined(CONFIG_SOC_BK7256XX)
    if (s_lcd.device.type == LCD_TYPE_QSPI) {
        bk_pwm_set_period_duty(LCD_BACKLIGHT_PWM_QSPI, &config);
    } else {
        bk_pwm_set_period_duty(LCD_BACKLIGHT_PWM_RGB, &config);
    }
#elif defined(CONFIG_SOC_BK7236XX)
    bk_pwm_set_period_duty(LCD_BACKLIGHT_PWM, &config);
#endif
#endif

    return BK_OK;
}

bk_err_t lcd_driver_backlight_close(void)
{
#if (defined(CONFIG_SOC_BK7256XX))
#if defined(CONFIG_PWM)
    if (s_lcd.device.type == LCD_TYPE_QSPI) {
        BK_LOG_ON_ERR(bk_pwm_stop(LCD_BACKLIGHT_PWM_QSPI));
        BK_LOG_ON_ERR(bk_pwm_deinit(LCD_BACKLIGHT_PWM_QSPI));
    } else {
        BK_LOG_ON_ERR(bk_pwm_stop(LCD_BACKLIGHT_PWM_RGB));
        BK_LOG_ON_ERR(bk_pwm_deinit(LCD_BACKLIGHT_PWM_RGB));
    }
#else
    if (s_lcd.device.type == LCD_TYPE_QSPI) {
        BK_LOG_ON_ERR(bk_gpio_pull_down(LCD_BACKLIGHT_GPIO_QSPI));
        bk_gpio_set_output_low(LCD_BACKLIGHT_GPIO_QSPI);
    } else {
        BK_LOG_ON_ERR(bk_gpio_pull_down(LCD_BACKLIGHT_GPIO_RGB));
        bk_gpio_set_output_low(LCD_BACKLIGHT_GPIO_RGB);
    }
#endif
#elif (defined(CONFIG_SOC_BK7236XX))
#if defined(CONFIG_PWM)
    BK_LOG_ON_ERR(bk_pwm_stop(LCD_BACKLIGHT_PWM));
    BK_LOG_ON_ERR(bk_pwm_deinit(LCD_BACKLIGHT_PWM));
#else
    BK_LOG_ON_ERR(bk_gpio_pull_down(LCD_BACKLIGHT_GPIO));
    bk_gpio_set_output_low(LCD_BACKLIGHT_GPIO);
#endif
#endif

    return BK_OK;
}


#if (USE_LCD_REGISTER_CALLBACKS == 1)
#if (defined(CONFIG_SOC_BK7236XX))
__attribute__((section(".iram"))) void lcd_isr();
#else
__attribute__((section(".itcm_sec_code"))) void lcd_isr();
#endif

bk_err_t  bk_lcd_isr_register(lcd_int_type_t int_type, lcd_isr_t isr)
{
	if (int_type == I8080_OUTPUT_SOF)
	{
		s_lcd.lcd_8080_frame_start_handler = isr;
	}
	if (int_type == I8080_OUTPUT_EOF)
	{
		s_lcd.lcd_8080_frame_end_handler = isr;
	}
	if (int_type == RGB_OUTPUT_SOF)
	{
		s_lcd.lcd_rgb_frame_start_handler = isr;
	}
	if (int_type == RGB_OUTPUT_EOF)
	{
		s_lcd.lcd_rgb_frame_end_handler = isr;
	}
	if (int_type == DE_INT)
	{
		s_lcd.lcd_rgb_de_handler = isr;
	}
	if (int_type == FRAME_INTERVAL_INT)
	{
		s_lcd.lcd_rgb_frame_interval_handler = isr;
	}
	return BK_OK;
}


#if (defined(CONFIG_SOC_BK7236XX))
__attribute__((section(".iram"))) void lcd_isr()
#else
__attribute__((section(".itcm_sec_code"))) void lcd_isr()
#endif
{
	uint32_t int_status = lcd_hal_int_status_get();

	if (int_status & RGB_OUTPUT_SOF)
	{
		if (s_lcd.lcd_rgb_frame_start_handler)
		{
			s_lcd.lcd_rgb_frame_start_handler();
		}
		lcd_hal_int_status_clear(RGB_OUTPUT_SOF);
	}
	if (int_status & RGB_OUTPUT_EOF)
	{
		if (s_lcd.lcd_rgb_frame_end_handler)
		{
			s_lcd.lcd_rgb_frame_end_handler();
		}
		lcd_hal_int_status_clear(RGB_OUTPUT_EOF);
	}

	if (int_status & DE_INT)
	{
		if (s_lcd.lcd_rgb_de_handler)
		{
			s_lcd.lcd_rgb_de_handler();
		}
		//lcd_hal_soft_reset();
		lcd_hal_int_status_clear(DE_INT);
	}
	if (int_status & FRAME_INTERVAL_INT)
	{
		if (s_lcd.lcd_8080_frame_end_handler)
		{
			s_lcd.lcd_rgb_frame_interval_handler();
		}
		lcd_hal_int_status_clear(FRAME_INTERVAL_INT);
	}

	if (int_status & I8080_OUTPUT_SOF)
	{
		if (s_lcd.lcd_8080_frame_start_handler)
		{
			s_lcd.lcd_8080_frame_start_handler();
		}
		lcd_hal_int_status_clear(I8080_OUTPUT_SOF);
	}

	if (int_status & I8080_OUTPUT_EOF)
	{
		if (s_lcd.lcd_8080_frame_end_handler)
		{
			s_lcd.lcd_8080_frame_end_handler();
		}
		lcd_hal_int_status_clear(I8080_OUTPUT_EOF);
	}
}
#else
bk_err_t bk_lcd_isr_register(lcd_isr_t lcd_isr)
{
	bk_int_isr_register(INT_SRC_LCD, lcd_isr, NULL);
	return BK_OK;
}
#endif


bk_err_t bk_lcd_driver_deinit(void)
{
	if (!s_lcd_driver_is_init) {
		LOGE("%s, lcd already deinit. \n", __func__);
		return BK_OK;
	}
	bk_int_isr_unregister(INT_SRC_LCD);
	if (sys_drv_lcd_close() != 0)
	{
		LOGE("lcd system deinit reg config error \r\n");
		return BK_FAIL;
	}
	s_lcd_driver_is_init = false;
	return BK_OK;
}

//bk_err_t bk_lcd_8080_int_enable(bool is_sof_en, bool is_eof_en)
//{
//	lcd_hal_8080_int_enable(is_sof_en, is_eof_en);
//	return BK_OK;
//}
//
//bk_err_t  bk_lcd_rgb_int_enable(bool is_sof_en, bool is_eof_en)
//{
//	lcd_hal_rgb_int_enable(is_sof_en, is_eof_en);
//	return BK_OK;
//}


bk_err_t bk_lcd_driver_init(lcd_clk_t clk)
{
	bk_err_t ret = BK_OK;
	if (s_lcd_driver_is_init) {
		LOGE("%s already init. \n", __func__);
		return BK_OK;
	}

	lcd_hal_soft_reset();
	sys_drv_int_enable(LCD_INTERRUPT_CTRL_BIT);

	switch (clk)
	{
#if defined(CONFIG_SOC_BK7256XX)
#if 0
		case LCD_320M:
			ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_0, DISP_DIV_H_0, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_160M:
			ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_1, DISP_DIV_H_0, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_106M:
			ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_0, DISP_DIV_H_1, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_120M:
			ret = sys_drv_lcd_set(DISP_CLK_120M, DISP_DIV_L_0, DISP_DIV_H_0, DSIP_DISCLK_ALWAYS_ON);
			break;
#endif
		case LCD_64M:
			ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_0, DISP_DIV_H_2, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_60M:
			ret = sys_drv_lcd_set(DISP_CLK_120M, DISP_DIV_L_1, DISP_DIV_H_0, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_80M:
			ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_1, DISP_DIV_H_1, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_54M:
			ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_1, DISP_DIV_H_2, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_45M:
			ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_1, DISP_DIV_H_3, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_40M:
			ret = sys_drv_lcd_set(DISP_CLK_120M, DISP_DIV_L_0, DISP_DIV_H_1, DSIP_DISCLK_ALWAYS_ON);
			//ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_1, DISP_DIV_H_3, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_35M:
			ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_0, DISP_DIV_H_4, DSIP_DISCLK_ALWAYS_ON);
			//ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_1, DISP_DIV_H_3, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_32M:
			ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_1, DISP_DIV_H_4, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_30M:
				ret = sys_drv_lcd_set(DISP_CLK_120M, DISP_DIV_L_1, DISP_DIV_H_1, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_26M:
			ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_1, DISP_DIV_H_5, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_24M:
			ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_0, DISP_DIV_H_6, DSIP_DISCLK_ALWAYS_ON);
		break;
		case LCD_22M:
			ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_1, DISP_DIV_H_6, DSIP_DISCLK_ALWAYS_ON);
		break;
		case LCD_20M:
			ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_1, DISP_DIV_H_7, DSIP_DISCLK_ALWAYS_ON);
			//ret = sys_drv_lcd_set(DISP_CLK_120M, DISP_DIV_L_1, DISP_DIV_H_2, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_17M:
			ret = sys_drv_lcd_set(DISP_CLK_120M, DISP_DIV_L_0, DISP_DIV_H_3, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_15M:
			ret = sys_drv_lcd_set(DISP_CLK_120M, DISP_DIV_L_1, DISP_DIV_H_3, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_12M:
			ret = sys_drv_lcd_set(DISP_CLK_120M, DISP_DIV_L_1, DISP_DIV_H_4, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_10M:
			ret = sys_drv_lcd_set(DISP_CLK_120M, DISP_DIV_L_1, DISP_DIV_H_5, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_9M:
			ret = sys_drv_lcd_set(DISP_CLK_120M, DISP_DIV_L_0, DISP_DIV_H_6, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_8M:
			ret = sys_drv_lcd_set(DISP_CLK_120M, DISP_DIV_L_0, DISP_DIV_H_7, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_7M:
			ret = sys_drv_lcd_set(DISP_CLK_120M, DISP_DIV_L_1, DISP_DIV_H_7, DSIP_DISCLK_ALWAYS_ON);
			break;
#else
		case LCD_80M:
			ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_1, DISP_DIV_H_0, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_54M:
			ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_0, DISP_DIV_H_1, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_60M:
			ret = sys_drv_lcd_set(DISP_CLK_120M, DISP_DIV_L_0, DISP_DIV_H_0, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_32M:
			ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_0, DISP_DIV_H_2, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_30M:
			ret = sys_drv_lcd_set(DISP_CLK_120M, DISP_DIV_L_1, DISP_DIV_H_0, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_40M:
			ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_1, DISP_DIV_H_1, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_26M:
			ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_1, DISP_DIV_H_2, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_22M:
			ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_1, DISP_DIV_H_3, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_20M:
			ret = sys_drv_lcd_set(DISP_CLK_120M, DISP_DIV_L_0, DISP_DIV_H_1, DSIP_DISCLK_ALWAYS_ON);
			//ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_1, DISP_DIV_H_3, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_17M:
			ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_0, DISP_DIV_H_4, DSIP_DISCLK_ALWAYS_ON);
			//ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_1, DISP_DIV_H_3, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_15M:
				ret = sys_drv_lcd_set(DISP_CLK_120M, DISP_DIV_L_1, DISP_DIV_H_1, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_12M:
			ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_0, DISP_DIV_H_6, DSIP_DISCLK_ALWAYS_ON);
		break;
		case LCD_10M:
			ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_1, DISP_DIV_H_7, DSIP_DISCLK_ALWAYS_ON);
			//ret = sys_drv_lcd_set(DISP_CLK_120M, DISP_DIV_L_1, DISP_DIV_H_2, DSIP_DISCLK_ALWAYS_ON);
			break;
		case LCD_8M:
			ret = sys_drv_lcd_set(DISP_CLK_120M, DISP_DIV_L_0, DISP_DIV_H_3, DSIP_DISCLK_ALWAYS_ON);
			break;
#endif
		default:
			ret = sys_drv_lcd_set(DISP_CLK_320M, DISP_DIV_L_0, DISP_DIV_H_4, DSIP_DISCLK_ALWAYS_ON);
			break;
	}
#if (USE_LCD_REGISTER_CALLBACKS == 1)
	bk_int_isr_register(INT_SRC_LCD, lcd_isr, NULL);
#endif
	rtos_delay_milliseconds(1);

	lcd_hal_soft_reset();
	s_lcd_driver_is_init = true;
	return ret;
}
/**
 * @brief This API config lcd display x size and y size

 * @param
 *     - width lcd display width
 *     - height lcd display height
 *
 * attention 1. int the next version, the width and height deside the transfer number of lcd display.
 *              will config with another two register x offset and y offset
 *
 * attention 2. in this sdk version width/height only set once in 8080_init,if you want set twice,should
                set bk_lcd_8080_display_enable(0)
 */
bk_err_t bk_lcd_pixel_config(uint16_t x_pixel, uint16_t y_pixel)
{
	LCD_RETURN_ON_NOT_INIT();

	lcd_hal_pixel_config(x_pixel, y_pixel);
	return BK_OK;
}


bk_err_t bk_lcd_8080_send_cmd(uint8_t param_count, uint32_t command, uint32_t *param)
{
	LCD_RETURN_ON_NOT_INIT();

	lcd_hal_8080_cmd_send(param_count, command, param);
	return BK_OK;
}




bk_err_t bk_lcd_input_pixel_hf_reverse(bool hf_reverse)
{
	lcd_hal_set_pixel_reverse(hf_reverse);
	return BK_OK;
}

/*
 * rgb input RGB565, rgb888. yuv format
 */
bk_err_t bk_lcd_set_yuv_mode(pixel_format_t input_data_format)
{
	lcd_hal_set_yuv_mode(input_data_format);
	return BK_OK;
}


pixel_format_t bk_lcd_get_yuv_mode(void)
{
	uint32_t yuv_mode = lcd_hal_get_display_yuv_sel();
	uint32_t reverse = lcd_hal_get_pixel_reverse();
	pixel_format_t output_data_format = PIXEL_FMT_UNKNOW;

	if (reverse == 0)
	{
		if (yuv_mode == 0)
		{
			output_data_format = PIXEL_FMT_RGB565_LE;
		}
		else if (yuv_mode == 1)
		{
			output_data_format = PIXEL_FMT_YUYV;
		}
		else if (yuv_mode == 2)
		{
			output_data_format = PIXEL_FMT_UYVY;
		}
		else if (yuv_mode == 3)
		{
			output_data_format = PIXEL_FMT_YYUV;
		}
		else if (yuv_mode == 4)
		{
			output_data_format = PIXEL_FMT_UVYY;
		}
		else if (yuv_mode == 5)
		{
			output_data_format = PIXEL_FMT_VUYY;
		}
	}
	else if (reverse == 1)
	{
		if (yuv_mode == 0)
		{
			output_data_format = PIXEL_FMT_RGB565;
		}
	}

	return output_data_format;
}

bk_err_t bk_lcd_set_partical_display(bool en, uint16_t partial_clum_l, uint16_t partial_clum_r, uint16_t partial_line_l, uint16_t partial_line_r)
{
	LCD_RETURN_ON_NOT_INIT();
	lcd_hal_set_partical_display(en, partial_clum_l, partial_clum_r, partial_line_l, partial_line_r);
	return BK_OK;
}



bk_err_t bk_lcd_rgb_init(const lcd_device_t *device)
{
	BK_RETURN_ON_NULL(device);

	const lcd_rgb_t *rgb = device->rgb;
	uint16_t x = ppi_to_pixel_x(device->ppi);  //lcd size x
	uint16_t y = ppi_to_pixel_y(device->ppi);  //lcd size y
	LOGI("%s\n", __func__);

	lcd_hal_rgb_display_sel(1);  //RGB display enable, and select rgb module
	lcd_hal_set_sync_low(rgb->hsync_pulse_width, rgb->vsync_pulse_width);
	lcd_hal_rgb_int_enable(0, 1);

	lcd_hal_rgb_sync_config(rgb->hsync_back_porch,
	                        rgb->hsync_front_porch,
	                        rgb->vsync_back_porch,
	                        rgb->vsync_front_porch);

	lcd_hal_set_rgb_clk_rev_edge(rgb->data_out_clk_edge);//output data is in clk doen edge or up adge

	lcd_hal_disconti_mode(DISCONTINUE_MODE);

	bk_lcd_pixel_config(x, y); //image xpixel ypixel
//	bk_lcd_set_yuv_mode(device->fmt);
	lcd_hal_set_data_fifo_thrd(DATA_FIFO_WR_THRD, DATA_FIFO_RD_THRD);
	return BK_OK;
}

bk_err_t bk_lcd_8080_init(const lcd_device_t *device)
{
	BK_RETURN_ON_NULL(device);

	uint16_t x = ppi_to_pixel_x(device->ppi);
	uint16_t y = ppi_to_pixel_y(device->ppi);
	LOGI("%s\n", __func__);
	lcd_hal_rgb_display_sel(0); //25bit - rgb_on = 0 select 8080 mode
	lcd_hal_disconti_mode(DISCONTINUE_MODE);
	lcd_hal_8080_verify_1ms_count(VERIFY_1MS_COUNT);
	lcd_hal_8080_set_tik(TIK_CNT);
	lcd_hal_set_data_fifo_thrd(DATA_FIFO_WR_THRD, DATA_FIFO_RD_THRD);
	lcd_hal_8080_set_fifo_data_thrd(CMD_FIFO_WR_THRD, CMD_FIFO_RD_THRD);
	lcd_hal_pixel_config(x, y);
	lcd_hal_8080_display_enable(1);
	lcd_hal_8080_int_enable(0, 1); //set eof int enable
//	bk_lcd_set_yuv_mode(config->fmt);
	return BK_OK;
}

bk_err_t bk_lcd_8080_deinit(void)
{
	LCD_RETURN_ON_NOT_INIT();
	lcd_hal_8080_int_enable(0, 0);
	lcd_hal_8080_display_enable(0);
	lcd_hal_8080_start_transfer(0);
	return BK_OK;
}

bk_err_t bk_lcd_rgb_deinit(void)
{
	LCD_RETURN_ON_NOT_INIT();
	lcd_hal_rgb_int_enable(0, 0);
	lcd_hal_rgb_display_en(0);
	lcd_hal_rgb_display_sel(0);

	return BK_OK;
}

bk_err_t bk_lcd_8080_start_transfer(bool start)
{
	LCD_RETURN_ON_NOT_INIT();
	lcd_hal_8080_start_transfer(start);
	return BK_OK;
}

//to del
bk_err_t bk_lcd_rgb_display_en(bool en)
{
	lcd_hal_rgb_display_en(en);
	return BK_OK;
}

bk_err_t lcd_driver_display_disable(void)
{
	lcd_type_t type;
	type = s_lcd.device.type;

	if ((type == LCD_TYPE_RGB565) || (type == LCD_TYPE_RGB))
	{
		lcd_hal_rgb_display_en(0);
	} else if (type == LCD_TYPE_MCU8080) {
        lcd_hal_8080_cmd_param_count(0);
        lcd_hal_8080_start_transfer(0);
    }

    return BK_OK;
}

bk_err_t lcd_driver_display_enable(void)
{
	lcd_type_t type;
	type = s_lcd.device.type;
	if ((type == LCD_TYPE_RGB565) || (type == LCD_TYPE_RGB))
	{
		lcd_hal_rgb_display_en(1);
	}
	else if (type == LCD_TYPE_MCU8080)
	{
		if(s_lcd.device.id == LCD_DEVICE_NT35510_MCU)
		{
			lcd_hal_8080_start_transfer(0);
			if(s_lcd.device.mcu->set_display_area)
				s_lcd.device.mcu->set_display_area(0, 479, 0, 799);
			if(s_lcd.device.mcu->start_transform)
				s_lcd.device.mcu->start_transform();
		}
		else if(s_lcd.device.id == LCD_DEVICE_ST7789V)
		{
			lcd_hal_8080_start_transfer(0);
			if(s_lcd.device.mcu->set_display_area)
				s_lcd.device.mcu->set_display_area(0+35, 169+35, 0, 319); // special process
			if(s_lcd.device.mcu->start_transform)
				s_lcd.device.mcu->start_transform();
		}
		lcd_hal_8080_start_transfer(1);
		lcd_hal_8080_cmd_param_count(1);
		lcd_hal_8080_write_cmd(0x2c);
	}

	return BK_OK;
}

bk_err_t lcd_driver_display_continue(void)
{
	lcd_type_t type;
	type = s_lcd.device.type;
	if (type == LCD_TYPE_RGB565)
	{
	}
	if (type == LCD_TYPE_MCU8080)
	{
		lcd_hal_8080_start_transfer(1);
		if(s_lcd.device.id != LCD_DEVICE_NT35510_MCU)
		{
			lcd_hal_8080_write_cmd(0x3c);
		}
		else
		{
			lcd_hal_8080_start_transfer(0);
			if(s_lcd.device.mcu->continue_transform)
				s_lcd.device.mcu->continue_transform();

			lcd_hal_8080_cmd_param_count(1);
			lcd_hal_8080_write_cmd(0x2c);
		}
	}
	return BK_OK;
}

bk_err_t lcd_driver_set_display_base_addr(uint32_t disp_base_addr)
{
	lcd_hal_set_display_read_base_addr(disp_base_addr);

	return BK_OK;
}

void lcd_driver_ppi_set(uint16_t width, uint16_t height)
{
	uint16_t x = ppi_to_pixel_x(s_lcd.device.ppi);
	uint16_t y = ppi_to_pixel_y(s_lcd.device.ppi);

	//bk_lcd_set_partical_display(0, 0, 0, 0, 0);
	lcd_hal_pixel_config(width, height);
	uint16_t start_x = 1;
	uint16_t start_y = 1;
	uint16_t end_x = x;
	uint16_t end_y = y;

	if (x < width || y < height)
	{
		if (x < width)
		{
			start_x = (width - x) / 2 + 1;
			end_x = start_x + x - 1;
		}
		if (y < height)
		{
			start_y = (height - y) / 2 + 1;
			end_y = start_y + y - 1;
		}

		LOGD("%s, offset %d, %d, %d, %d\n", __func__, start_x, end_x, start_y, end_y);
		bk_lcd_set_partical_display(1, start_x, end_x, start_y, end_y);
	}
    else if (x == width && y == height)
    {
        bk_lcd_set_partical_display(0, 0, 0, 0, 0);
    }
	else
	{
        bk_lcd_set_partical_display(0, 0, 0, 0, 0);
	}
}

bk_err_t lcd_ldo_power_enable(uint8_t enable)
{
#if (defined(CONFIG_LCD_POWER_GPIO_CTRL))

	if (enable)
	{
#if (LCD_LDO_CTRL_ACTIVE_LEVEL)
		bk_gpio_ctrl_external_ldo(GPIO_CTRL_LDO_MODULE_LCD, LCD_LDO_CTRL_GPIO, GPIO_OUTPUT_STATE_HIGH);
#else
		bk_gpio_ctrl_external_ldo(GPIO_CTRL_LDO_MODULE_LCD, LCD_LDO_CTRL_GPIO, GPIO_OUTPUT_STATE_LOW);
#endif
	}
	else
	{

#if (LCD_LDO_CTRL_ACTIVE_LEVEL)
		bk_gpio_ctrl_external_ldo(GPIO_CTRL_LDO_MODULE_LCD, LCD_LDO_CTRL_GPIO, GPIO_OUTPUT_STATE_LOW);
#else
		bk_gpio_ctrl_external_ldo(GPIO_CTRL_LDO_MODULE_LCD, LCD_LDO_CTRL_GPIO, GPIO_OUTPUT_STATE_HIGH);
#endif
	}
#endif

	return BK_OK;
}

bk_err_t lcd_driver_init(const lcd_device_t *device)
{
	BK_RETURN_ON_NULL(device);

	int ret = BK_OK;
	LOGI("%s  \n", __func__);

	/// LCD module power
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_VIDP_LCD, PM_POWER_MODULE_STATE_ON);
	bk_pm_clock_ctrl(PM_CLK_ID_DISP, CLK_PWR_CTRL_PWR_UP);
#if defined(CONFIG_SOC_BK7256XX)
	lcd_ldo_power_enable(1);
	bk_pm_module_vote_cpu_freq(PM_DEV_ID_DISP, PM_CPU_FRQ_320M);
#else
    bk_pm_module_vote_ctrl_external_ldo(GPIO_CTRL_LDO_MODULE_LCD, LCD_LDO_CTRL_GPIO, GPIO_OUTPUT_STATE_HIGH);
	bk_pm_module_vote_cpu_freq(PM_DEV_ID_DISP, PM_CPU_FRQ_480M);
#endif
	os_memset(&s_lcd, 0, sizeof(s_lcd));
	os_memcpy((void*)&s_lcd.device, device, sizeof(lcd_device_t));

	if ((device->type == LCD_TYPE_RGB565) || (device->type == LCD_TYPE_RGB))
	{
		bk_lcd_driver_init(device->rgb->clk);
		lcd_rgb_gpio_init();
		bk_lcd_rgb_init(device);
		lcd_hal_rgb_set_in_out_format(device->src_fmt, device->out_fmt);
//		lcd_hal_int_enable(DE_INT);

//		lcd_hal_int_enable(FRAME_INTERVAL_INT);
//		lcd_hal_frame_interval_config(1, VSYNC_UNIT, 2);
	}
	else if (device->type == LCD_TYPE_MCU8080)
	{
		bk_lcd_driver_init(device->mcu->clk);
		lcd_mcu_gpio_init();
		bk_lcd_8080_init(device);
		lcd_hal_mcu_set_in_out_format(device->src_fmt, device->out_fmt);
	}
#if defined(CONFIG_LCD_QSPI)
	else if (device->type == LCD_TYPE_QSPI)
	{
        bk_lcd_qspi_disp_open(LCD_QSPI_ID, device);
    }
#endif

	uint64_t before, after;
#if defined(CONFIG_ARCH_RISCV)
	before = riscv_get_mtimer();
#else
	before = 0;
#endif

	if (device->init)
	{
		device->init();
	}
#if defined(CONFIG_ARCH_RISCV)
	after = riscv_get_mtimer();
#else
	after = 0;
#endif
	LOGI("lcd init time: %lu\n", (after - before) / 26000);

	return ret;
}

bk_err_t lcd_driver_deinit(void)
{
	bk_err_t ret = BK_OK;

	if (s_lcd.device.lcd_off)
	{
		s_lcd.device.lcd_off();
	}
	/* LCD controller */
	if (s_lcd.device.type == LCD_TYPE_RGB565)
	{
		ret = bk_lcd_rgb_deinit();
		if (ret != 0)
		{
			LOGE("lcd system deinit reg config error \r\n");
			ret = BK_FAIL;
			goto out;
		}
	}
	else if (s_lcd.device.type == LCD_TYPE_MCU8080)
	{
		ret = bk_lcd_8080_deinit();
		if (ret != BK_OK)
		{
			LOGE("lcd system deinit reg config error \r\n");
			ret = BK_FAIL;
			goto out;
		}
	}
#if defined(CONFIG_LCD_QSPI)
    else if (s_lcd.device.type == LCD_TYPE_QSPI)
    {
        bk_lcd_qspi_disp_close(LCD_QSPI_ID);
    }
#endif

	out:
	bk_lcd_driver_deinit();
    bk_lcd_rgb_io_deinit();
	bk_pm_clock_ctrl(PM_CLK_ID_DISP, CLK_PWR_CTRL_PWR_DOWN);
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_VIDP_LCD, PM_POWER_MODULE_STATE_OFF);
	bk_pm_module_vote_cpu_freq(PM_DEV_ID_DISP, PM_CPU_FRQ_DEFAULT);
#if defined(CONFIG_SOC_BK7256XX)
	lcd_ldo_power_enable(0);
#else
    bk_pm_module_vote_ctrl_external_ldo(GPIO_CTRL_LDO_MODULE_LCD, LCD_LDO_CTRL_GPIO, GPIO_OUTPUT_STATE_LOW);
#endif
	return ret;
}


