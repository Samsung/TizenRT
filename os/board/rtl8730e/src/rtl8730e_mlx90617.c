/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdbool.h>
#include <stdio.h>
#include <debug.h>
#include <assert.h>
#include <errno.h>

//#include <tinyara/irq.h>
#include <tinyara/gpio.h>
#include <tinyara/irq.h>
#include <tinyara/sensors/mlx90617.h>
#include "objects.h"
#include "gpio_irq_api.h"
#include "PinNames.h"
#include "gpio_api.h"

#define PIN_LOW 0
#define PIN_HIGH 1

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* i2c config */

/*#if CONFIG_RTL8730E_BOARD_REVISION >= 6
#define MLX90617_I2C_PORT 0
#else
#define MLX90617_I2C_PORT 1
#endif*/

#define MLX90617_I2C_PORT 2


#define MLX90617_SPI_PORT 0
#define MLX90617_SPI_BPW  8 
#define MLX90617_SPI_CS  0 
#define MLX90617_SPI_MODE                 0
#define MLX90617_SPI_FREQ               1000000

#define MLX90617_I2C_FREQ		50000
//#define MLX90617_I2C_FREQ             100000
#define MLX90617_I2C_ADDRLEN		7

#define MLX90617_I2C_ADDR		0x5D
#define MLX90614_N_I2C_ADDR     0x5A  
/*other pin config */
#define MLX90617_GPIO_I2C_PIN PA_29
#define MLX90617_GPIO_RESET_PIN		PA_2
/*
#if CONFIG_RTL8730E_BOARD_REVISION >= 6
#define MLX90617_GPIO_I2C_PIN PA_28
#else
#define MLX90617_GPIO_I2C_PIN PA_2
#endif
*/
/****************************************************************************
 * Private Types
 ****************************************************************************/

struct rtl8730e_mlx90617_info_s {
	struct sensor_config s_config;
	//touch_handler_t handler;
	//gpio_irq_t data_ready;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/*static void rtl8730e_ist415_enable_irq(bool enable);
static void rtl8730e_ist415_irq_attach(touch_handler_t handler, FAR char *arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/
static struct rtl8730e_mlx90617_info_s g_mlx90617info = {
	.s_config = {
		.i2c_config = {
			.frequency = MLX90617_I2C_FREQ,
			.address = MLX90617_I2C_ADDR,
			.addrlen = MLX90617_I2C_ADDRLEN,
		},
		.spi_config = {
                        .bpw = MLX90617_SPI_BPW,
                        .freq = MLX90617_SPI_FREQ,
                        .cs = MLX90617_SPI_CS,
                        .mode = MLX90617_SPI_MODE,
                },
	},
};

//to refactor
//FAR struct i2c_dev_s *i2c;
//struct i2c_config_s configg;
//struct spi_io_config spi_configg;
//FAR struct spi_dev_s *spi;
/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void mlx90617_gpio_reset()
{
	GPIO_WriteBit(MLX90617_GPIO_RESET_PIN, PIN_HIGH);
        DelayMs(500);
        GPIO_WriteBit(MLX90617_GPIO_RESET_PIN, PIN_LOW);
        DelayMs(500);
        GPIO_WriteBit(MLX90617_GPIO_RESET_PIN, PIN_HIGH);
        DelayMs(500);

	return;
}

static void mlx90617_gpio_init()
{
        Pinmux_Config(MLX90617_GPIO_RESET_PIN, PINMUX_FUNCTION_GPIO);
        GPIO_InitTypeDef ResetPin;
        ResetPin.GPIO_Pin = MLX90617_GPIO_RESET_PIN;
        ResetPin.GPIO_PuPd = GPIO_PuPd_NOPULL;
        ResetPin.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_Init(&ResetPin);
}

void rtl8730e_mlx90617_initialize()
{
	struct spi_dev_s *spi = up_spiinitialize(MLX90617_SPI_PORT);
	if (!spi) {
		lldbg("ERROR: Failed to initialize SPI\n");
	}
	FAR struct i2c_dev_s *i2c;
	
	//mlx90617_gpio_init();
	//mlx90617_gpio_reset();
	lldbg("i2c port %d\n", MLX90617_I2C_PORT);
	i2c = up_i2cinitialize(MLX90617_I2C_PORT);
	if (!i2c) {
		lldbg("ERROR: Failed to initialize I2C\n");
	}
	struct i2c_config_s configg = g_mlx90617info.s_config.i2c_config;
	//struct spi_io_config spi_configg;
	SPI_SETMODE(spi, MLX90617_SPI_MODE);
	SPI_SETFREQUENCY(spi, MLX90617_SPI_FREQ);
	SPI_SETBITS(spi, MLX90617_SPI_BPW);

	int ret = 0;
	ret = mlx90617_register("/dev/sensor0", i2c, &(g_mlx90617info.s_config), spi);
	if (ret < 0) {
		lldbg("ERROR: Sensor driver register fail\n");
		return;
	}
	
	lldbg("Sensor driver register success\n");
}
