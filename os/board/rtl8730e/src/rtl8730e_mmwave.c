#include <tinyara/config.h>
#include <debug.h>
#include "objects.h"
#include "PinNames.h"
#include "gpio_api.h"
#include <tinyara/mmwave/mmwave.h>

#ifndef CONFIG_MMWAVE_SENSOR_SPI_PORT
#define CONFIG_MMWAVE_SENSOR_SPI_PORT 0
#endif
#define GPIO_PIN_RESET PA_4
#define GPIO_PIN_BUSY PA_15

/****************************************************************************
 * Private Types
 ****************************************************************************/
struct rtl8730e_mmwave_info_s {
	struct mmwave_config_s sensor_config;
};

static int rtl8730e_mmwave_gpio_reset(void);
static int rtl8730e_mmwave_gpio_busy_status(void);

struct rtl8730e_mmwave_info_s g_rtl8730_config_dev_s = {
	.sensor_config = {
		.reset = rtl8730e_mmwave_gpio_reset,
		.busy_status = rtl8730e_mmwave_gpio_busy_status,
		NULL,
		NULL}};

static int rtl8730e_mmwave_gpio_reset(void)
{
	GPIO_WriteBit(GPIO_PIN_RESET, 1);
	DelayMs(10);
	GPIO_WriteBit(GPIO_PIN_RESET, 0);
	DelayMs(10);
	GPIO_WriteBit(GPIO_PIN_RESET, 1);
	DelayMs(50);
	return OK;
}

static int rtl8730e_mmwave_gpio_busy_status(void)
{
	return GPIO_ReadDataBit(GPIO_PIN_BUSY);
}

static void gpio_pins_init(void)
{
	GPIO_InitTypeDef ResetPin;
	GPIO_InitTypeDef BusyPin;

	Pinmux_Config(GPIO_PIN_RESET, PINMUX_FUNCTION_GPIO);

	ResetPin.GPIO_Pin = GPIO_PIN_RESET;
	ResetPin.GPIO_PuPd = GPIO_PuPd_NOPULL;
	ResetPin.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(&ResetPin);

	Pinmux_Config(GPIO_PIN_BUSY, PINMUX_FUNCTION_GPIO);
	BusyPin.GPIO_Pin = GPIO_PIN_BUSY;
	BusyPin.GPIO_PuPd = GPIO_PuPd_UP;
	BusyPin.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(&BusyPin);
}

/****************************************************************************
 * Name:  rtl8730e_mmwave_initialize
 *
 ****************************************************************************/

void rtl8730e_mmwave_initialize(void)
{
	gpio_pins_init();
	FAR struct spi_dev_s *spi = up_spiinitialize(CONFIG_MMWAVE_SENSOR_SPI_PORT);
	if (mmwave_initialize(spi, &g_rtl8730_config_dev_s.sensor_config) < 0) {
		lldbg("ERROR: mmWave driver register fail\n");
	} else {
		lldbg("mmWave driver register success\n");
	}
}