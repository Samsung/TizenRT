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
#include <driver/gpio.h>
#include <driver/gpio_types.h>
#include <driver/io_matrix.h>
#include <driver/io_matrix_types.h>

#include "gpio_adapter.h"
#include "gpio_driver.h"
#include "io_matrix_driver.h"
#include "iomx_ll.h"
#if defined(CONFIG_ANA_GPIO)
#include "ana_gpio_driver.h"
#endif


#define GPIO_REG_DEFAULT_VALUE                    (0x0)
#define GPIO_WAKE_SOURCE_IDLE_ID                  (GPIO_NUM_MAX)
#define GPIO_LOWPOWER_KEEP_STATUS_IDLE_ID         (GPIO_NUM_MAX)


typedef struct
{
	gpio_id_t id;
	gpio_int_type_t int_type;
	//gpio_isr_t isr;
} gpio_dynamic_wakeup_t;

typedef struct
{
	gpio_id_t gpio_id;
	gpio_config_t config;
} gpio_dynamic_keep_status_t;


static bool s_gpio_is_init = false;

#if defined(CONFIG_GPIO_WAKEUP_SUPPORT)
static uint64_t s_gpio_is_setted_wake_status;
static gpio_id_t s_gpio_wakeup_gpio_id = SOC_GPIO_NUM;
#if defined(CONFIG_GPIO_DYNAMIC_WAKEUP_SUPPORT)
static gpio_dynamic_wakeup_t s_gpio_dynamic_wakeup_source_map[CONFIG_GPIO_DYNAMIC_WAKEUP_SOURCE_MAX_CNT];
#endif
#endif

#if defined(CONFIG_GPIO_KPSTAT_SUPPORT)
static uint64_t s_gpio_is_lowpower_keep_status;
#if defined(CONFIG_GPIO_DYNAMIC_KPSTAT_SUPPORT)
static gpio_dynamic_keep_status_t s_gpio_lowpower_keep_config[CONFIG_GPIO_DYNAMIC_KEEP_STATUS_MAX_CNT];
#endif
#endif


// Save GPIO func_code for wakeup (to restore exact pin configuration)
static IOMX_CODE_T s_gpio_func_code_map[SOC_GPIO_NUM] = {FUNC_CODE_INVALID};


#if defined(CONFIG_GPIO_DEFAULT_SET_SUPPORT)
static void gpio_default_map_init(void);
#endif

#if defined(CONFIG_GPIO_WAKEUP_SUPPORT)
static void gpio_wakeup_source_config(void);
static void gpio_record_wakeup_pin_id(void);
#if defined(CONFIG_GPIO_DYNAMIC_WAKEUP_SUPPORT)
static void gpio_dynamic_wakeup_source_init(void);
#endif
#endif

#if defined(CONFIG_GPIO_KPSTAT_SUPPORT)
static void gpio_keep_status_init(void);
#if defined(CONFIG_GPIO_DYNAMIC_KPSTAT_SUPPORT)
static void gpio_keep_status_config(void);
#endif
#endif


bk_err_t bk_gpio_driver_init(void)
{
	//avoid re-init caused some info lost
	if (s_gpio_is_init) {
		GPIO_LOGI("%s:has inited \r\n", __func__);
		return BK_OK;
	}

	bk_iomx_driver_init();

#if defined(CONFIG_GPIO_KPSTAT_SUPPORT)
	gpio_keep_status_init();
#endif

#if defined(CONFIG_GPIO_WAKEUP_SUPPORT)
	gpio_record_wakeup_pin_id();

#if defined(CONFIG_GPIO_DYNAMIC_WAKEUP_SUPPORT)
	gpio_dynamic_wakeup_source_init();
#endif

#if defined(CONFIG_ANA_GPIO)
	ana_gpio_wakeup_init();
#endif
#endif

#if defined(CONFIG_GPIO_DEFAULT_SET_SUPPORT)
	gpio_default_map_init();
#endif

	s_gpio_is_init = true;

#if defined(CONFIG_GPIO_TEST)
		int bk_gpio_register_cli_test_feature(void);
		bk_gpio_register_cli_test_feature();
#endif

	return BK_OK;
}

bk_err_t bk_gpio_driver_deinit(void)
{
	if (!s_gpio_is_init)
	{
		GPIO_LOGI("%s:isn't init \r\n", __func__);
		return BK_OK;
	}

	// bk_iomx_driver_deinit();

	s_gpio_is_init = false;

	return BK_OK;
}

void bk_gpio_set_value(gpio_id_t id, uint32_t v)
{
	bk_iomx_set_value(id, v);
}

uint32_t bk_gpio_get_value(gpio_id_t id)
{
	return bk_iomx_get_value(id);
}

bk_err_t bk_gpio_enable_output(gpio_id_t gpio_id)
{
	return bk_iomx_set_gpio_func(gpio_id, FUNC_CODE_OUTPUT);
}

bk_err_t bk_gpio_disable_output(gpio_id_t gpio_id)
{
	// do nothing
	return BK_OK;
}

bk_err_t bk_gpio_enable_input(gpio_id_t gpio_id)
{
	return bk_iomx_set_gpio_func(gpio_id, FUNC_CODE_INPUT);
}

bk_err_t bk_gpio_disable_input(gpio_id_t gpio_id)
{
	// do nothing
	return BK_OK;
}

bk_err_t bk_gpio_set_high_impedance(gpio_id_t gpio_id)
{
	return bk_iomx_set_gpio_func(gpio_id, FUNC_CODE_HIGH_Z);
}

bk_err_t bk_gpio_enable_pull(gpio_id_t gpio_id )
{
	return bk_iomx_enable_pull(gpio_id);
}

bk_err_t bk_gpio_disable_pull(gpio_id_t gpio_id)
{
	return bk_iomx_disable_pull(gpio_id);
}

bk_err_t bk_gpio_pull_up(gpio_id_t gpio_id)
{
	return bk_iomx_pull_up(gpio_id);
}

bk_err_t bk_gpio_pull_down(gpio_id_t gpio_id)
{
	return bk_iomx_pull_down(gpio_id);
}

bk_err_t bk_gpio_set_output_high(gpio_id_t gpio_id)
{
	return bk_iomx_output_high(gpio_id);
}

bk_err_t bk_gpio_set_output_low(gpio_id_t gpio_id)
{
	return bk_iomx_output_low(gpio_id);
}

bool bk_gpio_get_input(gpio_id_t gpio_id)
{
	return bk_iomx_get_input(gpio_id);
}

bool bk_gpio_get_output(gpio_id_t gpio_id)
{
	return bk_iomx_get_output(gpio_id);
}


//MAX capactiy:3
bool bk_gpio_set_capacity(gpio_id_t gpio_id, uint32 capacity)
{
	return bk_iomx_set_capacity(gpio_id, capacity);
}

bk_err_t bk_gpio_set_config(gpio_id_t gpio_id, const gpio_config_t *config)
{
	switch (config->io_mode) {
	case GPIO_OUTPUT_ENABLE:
		bk_iomx_set_gpio_func(gpio_id, FUNC_CODE_OUTPUT);
		break;

	case GPIO_INPUT_ENABLE:
		bk_iomx_set_gpio_func(gpio_id, FUNC_CODE_INPUT);
		break;

	case GPIO_IO_DISABLE:
		bk_iomx_set_gpio_func(gpio_id, FUNC_CODE_HIGH_Z);
		break;

	default:
		break;
	}

	switch (config->pull_mode) {
	case GPIO_PULL_DISABLE:
		bk_iomx_disable_pull(gpio_id);
		break;

	case GPIO_PULL_DOWN_EN:
		bk_iomx_pull_down(gpio_id);
		break;

	case GPIO_PULL_UP_EN:
		bk_iomx_pull_up(gpio_id);
		break;

	default:
		break;
	}

	return BK_OK;
}

/* Enable GPIO  interrupt.
*/
bk_err_t bk_gpio_register_isr(gpio_id_t gpio_id, gpio_isr_t isr)
{
	return bk_iomx_register_isr(gpio_id, isr);
}

bk_err_t bk_gpio_register_isr_ex(gpio_id_t gpio_id, gpio_isr_priv_t isr, void *priv)
{
	return bk_iomx_register_isr_ex(gpio_id, isr, priv);
}

//This function just enable the select GPIO can report IRQ to CPU
bk_err_t bk_gpio_enable_interrupt(gpio_id_t gpio_id)
{
	return bk_iomx_enable_interrupt(gpio_id);
}

bk_err_t bk_gpio_disable_interrupt(gpio_id_t gpio_id)
{
	return bk_iomx_disable_interrupt(gpio_id);
}

bk_err_t bk_gpio_clear_interrupt(gpio_id_t gpio_id)
{
	return bk_iomx_clear_interrupt(gpio_id);
}

bk_err_t bk_gpio_set_interrupt_type(gpio_id_t gpio_id, gpio_int_type_t type)
{
	return bk_iomx_set_interrupt_type(gpio_id, type);
}

IOMX_CODE_T contert_gpio_dev_to_iomx_code(gpio_dev_t dev)
{
	IOMX_CODE_T func_code = FUNC_CODE_INVALID;

	switch (dev)
	{
	case GPIO_DEV_BT_ACTIVE:
		func_code = FUNC_CODE_BT_ACTIVE;
		break;
	case GPIO_DEV_BT_ANT0:
		func_code = FUNC_CODE_BT_ANT_0;
		break;
	case GPIO_DEV_BT_ANT1:
		func_code = FUNC_CODE_BT_ANT_1;
		break;
	case GPIO_DEV_BT_ANT2:
		func_code = FUNC_CODE_BT_ANT_2;
		break;
	case GPIO_DEV_BT_ANT3:
		func_code = FUNC_CODE_BT_ANT_3;
		break;
	case GPIO_DEV_BT_PRIORITY:
		func_code = FUNC_CODE_BT_PRIORITY;
		break;
	case GPIO_DEV_CLK_AUXS:
		func_code = FUNC_CODE_CLK_AUXS;
		break;
	case GPIO_DEV_CLK_XTAL:
		func_code = FUNC_CODE_CLK_XTAL;
		break;
	case GPIO_DEV_CLK_XTAL_DIV:
		func_code = FUNC_CODE_CLK_XTAL_DIV;
		break;
	case GPIO_DEV_DEBUG0:
		func_code = FUNC_CODE_DEBUG_0;
		break;
	case GPIO_DEV_DEBUG1:
		func_code = FUNC_CODE_DEBUG_1;
		break;
	case GPIO_DEV_DEBUG2:
		func_code = FUNC_CODE_DEBUG_2;
		break;
	case GPIO_DEV_DEBUG3:
		func_code = FUNC_CODE_DEBUG_3;
		break;
	case GPIO_DEV_DEBUG4:
		func_code = FUNC_CODE_DEBUG_4;
		break;
	case GPIO_DEV_DEBUG5:
		func_code = FUNC_CODE_DEBUG_5;
		break;
	case GPIO_DEV_DEBUG6:
		func_code = FUNC_CODE_DEBUG_6;
		break;
	case GPIO_DEV_DEBUG7:
		func_code = FUNC_CODE_DEBUG_7;
		break;
	case GPIO_DEV_DEBUG8:
		func_code = FUNC_CODE_DEBUG_8;
		break;
	case GPIO_DEV_DEBUG9:
		func_code = FUNC_CODE_DEBUG_9;
		break;
	case GPIO_DEV_DEBUG10:
		func_code = FUNC_CODE_DEBUG_10;
		break;
	case GPIO_DEV_DEBUG11:
		func_code = FUNC_CODE_DEBUG_11;
		break;
	case GPIO_DEV_DEBUG12:
		func_code = FUNC_CODE_DEBUG_12;
		break;
	case GPIO_DEV_DEBUG13:
		func_code = FUNC_CODE_DEBUG_13;
		break;
	case GPIO_DEV_DEBUG14:
		func_code = FUNC_CODE_DEBUG_14;
		break;
	case GPIO_DEV_DEBUG15:
		func_code = FUNC_CODE_DEBUG_15;
		break;
	case GPIO_DEV_FEM_LNA_EN: // ?
		func_code = FUNC_CODE_DEM_LNA_EN;
		break;
	case GPIO_DEV_I2C0_SCL:
		func_code = FUNC_CODE_I2C0_SCL;
		break;
	case GPIO_DEV_I2C0_SDA:
		func_code = FUNC_CODE_I2C0_SDA;
		break;
	case GPIO_DEV_I2C1_SCL:
		func_code = FUNC_CODE_I2C1_SCL;
		break;
	case GPIO_DEV_I2C1_SDA:
		func_code = FUNC_CODE_I2C1_SDA;
		break;
	case GPIO_DEV_I2S1_MCLK:
		func_code = FUNC_CODE_I2S_MCLK;
		break;
	case GPIO_DEV_I2S1_DIN:
		func_code = FUNC_CODE_I2S0_DIN;
		break;
	case GPIO_DEV_I2S1_DOUT:
		func_code = FUNC_CODE_I2S0_DOUT;
		break;
	case GPIO_DEV_I2S1_CLK:
		func_code = FUNC_CODE_I2S0_SCK;
		break;
	case GPIO_DEV_I2S1_SYNC:
		func_code = FUNC_CODE_I2S0_SYNC;
		break;
	case GPIO_DEV_LEDC:
		func_code = FUNC_CODE_LEDC;
		break;
	case GPIO_DEV_LPO_CLK:
		func_code = FUNC_CODE_LPO_CLK;
		break;
	case GPIO_DEV_PWM0:
		func_code = FUNC_CODE_PWM0_0;
		break;
	case GPIO_DEV_PWM1:
		func_code = FUNC_CODE_PWM0_1;
		break;
	case GPIO_DEV_PWM2:
		func_code = FUNC_CODE_PWM0_2;
		break;
	case GPIO_DEV_PWM3:
		func_code = FUNC_CODE_PWM0_3;
		break;
	case GPIO_DEV_PWM4:
		func_code = FUNC_CODE_PWM0_4;
		break;
	case GPIO_DEV_PWM5:
		func_code = FUNC_CODE_PWM0_5;
		break;
	case GPIO_DEV_PWM6:
		func_code = FUNC_CODE_PWM0_6;
		break;
	case GPIO_DEV_PWM7:
		func_code = FUNC_CODE_PWM0_7;
		break;
	case GPIO_DEV_PWM8:
		func_code = FUNC_CODE_PWM0_8;
		break;
	case GPIO_DEV_PWM9:
		func_code = FUNC_CODE_PWM0_9;
		break;
	case GPIO_DEV_PWM10:
		func_code = FUNC_CODE_PWM0_10;
		break;
	case GPIO_DEV_PWM11:
		func_code = FUNC_CODE_PWM0_11;
		break;
	case GPIO_DEV_QSPI0_CSN:
		func_code = FUNC_CODE_QSPI0_CS;
		break;
	case GPIO_DEV_QSPI0_IO0:
		func_code = FUNC_CODE_QSPI0_DAT_0;
		break;
	case GPIO_DEV_QSPI0_IO1:
		func_code = FUNC_CODE_QSPI0_DAT_1;
		break;
	case GPIO_DEV_QSPI0_IO2:
		func_code = FUNC_CODE_QSPI0_DAT_2;
		break;
	case GPIO_DEV_QSPI0_IO3:
		func_code = FUNC_CODE_QSPI0_DAT_3;
		break;
	case GPIO_DEV_QSPI0_CLK:
		func_code = FUNC_CODE_QSPI0_SCK;
		break;
	case GPIO_DEV_SDIO_HOST_CLK:
		func_code = FUNC_CODE_SDIO_CLK;
		break;
	case GPIO_DEV_SDIO_HOST_CMD:
		func_code = FUNC_CODE_SDIO_CMD;
		break;
	case GPIO_DEV_SDIO_HOST_DATA0:
		func_code = FUNC_CODE_SDIO_DATA_0;
		break;
	case GPIO_DEV_SDIO_HOST_DATA1:
		func_code = FUNC_CODE_SDIO_DATA_1;
		break;
	case GPIO_DEV_SDIO_HOST_DATA2:
		func_code = FUNC_CODE_SDIO_DATA_2;
		break;
	case GPIO_DEV_SDIO_HOST_DATA3:
		func_code = FUNC_CODE_SDIO_DATA_3;
		break;
	case GPIO_DEV_SPI0_MISO:
		func_code = FUNC_CODE_SPI0_MISO;
		break;
	case GPIO_DEV_SPI0_MOSI:
		func_code = FUNC_CODE_SPI0_MOSI;
		break;
	case GPIO_DEV_SPI0_CSN:
		func_code = FUNC_CODE_SPI0_NSS;
		break;
	case GPIO_DEV_SPI0_SCK:
		func_code = FUNC_CODE_SPI0_SCK;
		break;
	case GPIO_DEV_SPI1_MISO:
		func_code = FUNC_CODE_SPI1_MISO;
		break;
	case GPIO_DEV_SPI1_MOSI:
		func_code = FUNC_CODE_SPI1_MOSI;
		break;
	case GPIO_DEV_SPI1_CSN:
		func_code = FUNC_CODE_SPI1_NSS;
		break;
	case GPIO_DEV_SPI1_SCK:
		func_code = FUNC_CODE_SPI1_SCK;
		break;
	case GPIO_DEV_SW_CLK:
		func_code = FUNC_CODE_SWCLK;
		break;
	case GPIO_DEV_SWD_IO:
		func_code = FUNC_CODE_SWDIO;
		break;
	case GPIO_DEV_TAMP_RX_I:
		func_code = FUNC_CODE_TAMP_RX;
		break;
	case GPIO_DEV_TAMP_TX_O:
		func_code = FUNC_CODE_TAMP_TX;
		break;
	case GPIO_DEV_UART1_CTS:
		func_code = FUNC_CODE_UART0_CTS;
		break;
	case GPIO_DEV_UART1_RTS:
		func_code = FUNC_CODE_UART0_RTS;
		break;
	case GPIO_DEV_UART1_RXD:
		func_code = FUNC_CODE_UART0_RXD;
		break;
	case GPIO_DEV_UART1_TXD:
		func_code = FUNC_CODE_UART0_TXD;
		break;
	case GPIO_DEV_UART2_RXD:
		func_code = FUNC_CODE_UART1_RXD;
		break;
	case GPIO_DEV_UART2_TXD:
		func_code = FUNC_CODE_UART1_TXD;
		break;
	case GPIO_DEV_UART3_RXD:
		func_code = FUNC_CODE_UART2_RXD;
		break;
	case GPIO_DEV_UART3_TXD:
		func_code = FUNC_CODE_UART2_TXD;
		break;
	case GPIO_DEV_UART4_CTS:
		func_code = FUNC_CODE_UART3_CTS;
		break;
	case GPIO_DEV_UART4_RTS:
		func_code = FUNC_CODE_UART3_RTS;
		break;
	case GPIO_DEV_UART4_RXD:
		func_code = FUNC_CODE_UART3_RXD;
		break;
	case GPIO_DEV_UART4_TXD:
		func_code = FUNC_CODE_UART3_TXD;
		break;
	case GPIO_DEV_WIFI_ACTIVE:
		func_code = FUNC_CODE_WIFI_ACTIVE;
		break;
	case GPIO_DEV_WIFI_TX_EN:
		func_code = FUNC_CODE_WIFI_RX_EN;
		break;
	case GPIO_DEV_WIFI_RX_EN:
		func_code = FUNC_CODE_WIFI_TX_EN;
		break;

	default:
		break;
	}

	return func_code;
}

// Convert func_code to peripheral group for wakeup
// Only support SPI, I2C, UART and GPIO
static peripheral_group_t convert_iomx_code_to_peripheral_group(IOMX_CODE_T func_code)
{
	peripheral_group_t group = PERIPHERAL_GROUP_NONE;

	switch (func_code)
	{
	// I2C0 group
	case FUNC_CODE_I2C0_SCL:
	case FUNC_CODE_I2C0_SDA:
		group = PERIPHERAL_GROUP_I2C0;
		break;

	// I2C1 group
	case FUNC_CODE_I2C1_SCL:
	case FUNC_CODE_I2C1_SDA:
		group = PERIPHERAL_GROUP_I2C1;
		break;

	// SPI0 group
	case FUNC_CODE_SPI0_MISO:
	case FUNC_CODE_SPI0_MOSI:
	case FUNC_CODE_SPI0_NSS:
	case FUNC_CODE_SPI0_SCK:
		group = PERIPHERAL_GROUP_SPI0;
		break;

	// SPI1 group
	case FUNC_CODE_SPI1_MISO:
	case FUNC_CODE_SPI1_MOSI:
	case FUNC_CODE_SPI1_NSS:
	case FUNC_CODE_SPI1_SCK:
		group = PERIPHERAL_GROUP_SPI1;
		break;

	// UART0 group (UART1 in hardware)
	case FUNC_CODE_UART0_CTS:
	case FUNC_CODE_UART0_RTS:
	case FUNC_CODE_UART0_RXD:
	case FUNC_CODE_UART0_TXD:
		group = PERIPHERAL_GROUP_UART0;
		break;

	// UART1 group (UART2 in hardware)
	case FUNC_CODE_UART1_RXD:
	case FUNC_CODE_UART1_TXD:
		group = PERIPHERAL_GROUP_UART1;
		break;

	// UART2 group (UART3 in hardware)
	case FUNC_CODE_UART2_RXD:
	case FUNC_CODE_UART2_TXD:
		group = PERIPHERAL_GROUP_UART2;
		break;

	// UART3 group (UART4 in hardware)
	case FUNC_CODE_UART3_CTS:
	case FUNC_CODE_UART3_RTS:
	case FUNC_CODE_UART3_RXD:
	case FUNC_CODE_UART3_TXD:
		group = PERIPHERAL_GROUP_UART3;
		break;

	// GPIO mode
	case FUNC_CODE_INPUT:
		group = PERIPHERAL_GROUP_GPIO;
		break;

	default:
		// Other func codes not supported for wakeup
		group = PERIPHERAL_GROUP_NONE;
		break;
	}

	return group;
}

// Save GPIO func_code mapping before entering low power
static void gpio_save_peripheral_mapping(void)
{
	gpio_id_t gpio_id;
	IOMX_CODE_T func_code;

	for (gpio_id = GPIO_0; gpio_id < SOC_GPIO_NUM; gpio_id++)
	{
		func_code = bk_iomx_get_gpio_func_code(gpio_id);
		s_gpio_func_code_map[gpio_id] = func_code;
		GPIO_LOGD("GPIO[%d] func_code=0x%x\r\n", gpio_id, func_code);
	}
}

// Get peripheral group that caused wakeup by GPIO ID
static peripheral_group_t gpio_get_wakeup_peripheral_group(gpio_id_t gpio_id)
{
	if (gpio_id >= SOC_GPIO_NUM)
	{
		return PERIPHERAL_GROUP_NONE;
	}

	IOMX_CODE_T func_code = s_gpio_func_code_map[gpio_id];
	return convert_iomx_code_to_peripheral_group(func_code);
}

// Get peripheral group that caused wakeup (automatically get wakeup GPIO ID)
peripheral_group_t bk_gpio_get_wake_source_sub_type(void)
{

	gpio_id_t wakeup_gpio_id = ana_gpio_get_wakeup_pin();
	
	// Check if wakeup GPIO ID is valid
	if (wakeup_gpio_id >= SOC_GPIO_NUM)
	{
		return PERIPHERAL_GROUP_NONE;
	}
	
	return gpio_get_wakeup_peripheral_group(wakeup_gpio_id);

}

static void gpio_low_power_config(void);
bk_err_t gpio_enter_low_power(void *param)
{
	GPIO_LOGD("%s[+]\r\n", __func__);

	iomx_dump_regs(true, true, true);

	iomx_backup_gpio_configs();

	iomx_dump_baked_regs();

	// Save GPIO to peripheral device mapping before entering low power
	gpio_save_peripheral_mapping();

	//NOTES:force disable all int to avoid config gpio caused error isr
	iomx_disable_all_interrupts();

	// setup gpio configs for sleep
#if defined(CONFIG_GPIO_DYNAMIC_KPSTAT_SUPPORT)
	gpio_keep_status_config();
#endif

#if defined(CONFIG_GPIO_WAKEUP_SUPPORT)
	gpio_wakeup_source_config();
#endif

	gpio_low_power_config();

	iomx_dump_regs(true, true, true);

	GPIO_LOGD("%s[-]\r\n", __func__);

	return BK_OK;
}

__IRAM_SEC bk_err_t gpio_exit_low_power(void *param)
{
	GPIO_LOGD("%s[+]\r\n", __func__);

	iomx_dump_regs(true, true, true);
	iomx_dump_baked_regs();

	iomx_disable_all_interrupts();

#if defined(CONFIG_GPIO_WAKEUP_SUPPORT)
#if defined(CONFIG_ANA_GPIO)
	// another workaround fix for unexpected gpio interrupt
	ana_gpio_clear_wakeup_source();
#endif
#endif

	iomx_restore_gpio_configs();

	iomx_dump_regs(true, true, true);
	iomx_dump_baked_regs();

	GPIO_LOGD("%s[-]\r\n", __func__);

	return BK_OK;
}

__IRAM_SEC bk_err_t gpio_hal_switch_to_low_power_status(uint64_t skip_io)
{
	for (gpio_id_t i = GPIO_0; i < GPIO_NUM_MAX; i++)
	{
		if (skip_io & (0x1ULL << i))
			continue;
		bk_iomx_set_value(i, GPIO_REG_DEFAULT_VALUE);
	}

	return BK_OK;
}

static void gpio_low_power_config(void)
{
	uint64_t skip_io = 0;
	
#if defined(CONFIG_GPIO_WAKEUP_SUPPORT)
	skip_io |= s_gpio_is_setted_wake_status;
#endif

#if defined(CONFIG_GPIO_KPSTAT_SUPPORT)
	skip_io |= s_gpio_is_lowpower_keep_status;
#endif

	gpio_hal_switch_to_low_power_status(skip_io);
}

#if defined(CONFIG_GPIO_WAKEUP_SUPPORT)
gpio_id_t bk_gpio_get_wakeup_gpio_id(void)
{
	return s_gpio_wakeup_gpio_id;
}

static void gpio_record_wakeup_pin_id(void)
{
#if defined(CONFIG_ANA_GPIO)
	s_gpio_wakeup_gpio_id = ana_gpio_get_wakeup_pin();
#endif
}

bk_err_t gpio_enable_interrupt_mult_for_wake(void)
{
#if defined(CONFIG_ANA_GPIO)
	ana_gpio_config_wakeup_source(s_gpio_is_setted_wake_status);
#endif

	return iomx_enable_multi_interrupts(s_gpio_is_setted_wake_status);
}

static void gpio_set_wakeup_config(gpio_id_t gpio_id, gpio_int_type_t int_type)
{
	// setup gpio as input mode
	bk_iomx_set_value(gpio_id, GPIO_REG_DEFAULT_VALUE);
	bk_iomx_set_gpio_func(gpio_id, FUNC_CODE_INPUT);

	switch(int_type)
	{
		case GPIO_INT_TYPE_LOW_LEVEL:
		case GPIO_INT_TYPE_FALLING_EDGE:
			bk_gpio_pull_up(gpio_id);
			GPIO_LOGD("%s GPIO %d Pull_up!\r\n", __func__, gpio_id);
			break;
		case GPIO_INT_TYPE_HIGH_LEVEL:
		case GPIO_INT_TYPE_RISING_EDGE:
			bk_gpio_pull_down(gpio_id);
			GPIO_LOGD("%s GPIO %d Pull_down!\r\n", __func__, gpio_id);
			break;
		default:
			GPIO_LOGI("%s Please set fill in the mode correctly!\r\n", __func__);
			break;
	}

	bk_gpio_set_interrupt_type(gpio_id, int_type);
}

static void gpio_wakeup_source_config(void)
{
	uint32_t i = 0;

	GPIO_LOGD("%s[+]\r\n", __func__);

	s_gpio_is_setted_wake_status = 0;

	gpio_wakeup_t gpio_wakeup_map[] = GPIO_STATIC_WAKEUP_SOURCE_MAP;
	for (i = 0; i < sizeof(gpio_wakeup_map)/sizeof(gpio_wakeup_t); i++)
	{
		gpio_set_wakeup_config(gpio_wakeup_map[i].id, gpio_wakeup_map[i].int_type);
		s_gpio_is_setted_wake_status |= ((uint64_t)1 << gpio_wakeup_map[i].id);
	}

#if defined(CONFIG_GPIO_DYNAMIC_WAKEUP_SUPPORT)
	for (i = 0; i < CONFIG_GPIO_DYNAMIC_WAKEUP_SOURCE_MAX_CNT; i++)
	{
		if (s_gpio_dynamic_wakeup_source_map[i].id != GPIO_WAKE_SOURCE_IDLE_ID) {
			//maybe the PIN is re-used as SECOND_FUNCTION and GPIO,F.E:UART RXD re-uses as wakeup PIN
			gpio_set_wakeup_config(s_gpio_dynamic_wakeup_source_map[i].id, s_gpio_dynamic_wakeup_source_map[i].int_type);
			s_gpio_is_setted_wake_status |= ((uint64_t)1 << s_gpio_dynamic_wakeup_source_map[i].id);
		}
	}
#endif

	GPIO_LOGD("%s[-]set wake src h=0x%0x, l=0x%0x\r\n", __func__, (uint32_t)(s_gpio_is_setted_wake_status>>32), (uint32_t)s_gpio_is_setted_wake_status);
}

#if defined(CONFIG_GPIO_DYNAMIC_WAKEUP_SUPPORT)
bk_err_t bk_gpio_register_wakeup_source(gpio_id_t gpio_id, gpio_int_type_t int_type)
{
	uint32_t i = 0;

	// GPIO_RETURN_ON_INVALID_ID(gpio_id);
	// GPIO_RETURN_ON_INVALID_INT_TYPE_MODE(int_type);

	//search the same id and replace it.
	for (i = 0; i < CONFIG_GPIO_DYNAMIC_WAKEUP_SOURCE_MAX_CNT; i++)
	{
		if (s_gpio_dynamic_wakeup_source_map[i].id == gpio_id)
		{
			s_gpio_dynamic_wakeup_source_map[i].int_type = int_type;

			//NOTES:If doesn't set int type, exit lowpower, if the ISR has reported by rising/falling type
			//Then restore to level type, entry GPIO ISR caused the int status lost.
			bk_gpio_set_interrupt_type(gpio_id, int_type);
			//s_gpio_dynamic_wakeup_source_map[i].isr = isr;

			GPIO_LOGD("gpio=%d,int_type=%d replace previous wake src\r\n", gpio_id, int_type);
			return BK_OK;
		}
	}

	//serach the first idle id
	for (i = 0; i < CONFIG_GPIO_DYNAMIC_WAKEUP_SOURCE_MAX_CNT; i++)
	{
		if (s_gpio_dynamic_wakeup_source_map[i].id == GPIO_WAKE_SOURCE_IDLE_ID)
		{
			s_gpio_dynamic_wakeup_source_map[i].id = gpio_id;
			s_gpio_dynamic_wakeup_source_map[i].int_type = int_type;

			//NOTES:If doesn't set int type, exit lowpower, if the ISR has reported by rising/falling type
			//Then restore to level type, entry GPIO ISR caused the int status lost.
			bk_gpio_set_interrupt_type(gpio_id, int_type);
			//s_gpio_dynamic_wakeup_source_map[i].isr = isr;
			s_gpio_is_setted_wake_status |= ((uint64_t)1 << s_gpio_dynamic_wakeup_source_map[i].id);

			GPIO_LOGD("gpio=%d,int_type=%d register wake src\r\n", gpio_id, int_type);

			return BK_OK;
		}
	}

	GPIO_LOGE("too much(%d) GPIO is setted wake src\r\n", CONFIG_GPIO_DYNAMIC_WAKEUP_SOURCE_MAX_CNT);
	for (i = 0; i < CONFIG_GPIO_DYNAMIC_WAKEUP_SOURCE_MAX_CNT; i++)
	{
		GPIO_LOGE("gpio id:%d is using \r\n", s_gpio_dynamic_wakeup_source_map[i].id);
	}
	return BK_FAIL;
}

bk_err_t bk_gpio_unregister_wakeup_source(gpio_id_t gpio_id)
{
	uint32_t i = 0;

	// GPIO_RETURN_ON_INVALID_ID(gpio_id);

	/* search the same id and replace it.*/
	for (i = 0; i < CONFIG_GPIO_DYNAMIC_WAKEUP_SOURCE_MAX_CNT; i++)
	{
		if (s_gpio_dynamic_wakeup_source_map[i].id == gpio_id)
		{
			s_gpio_dynamic_wakeup_source_map[i].id = GPIO_WAKE_SOURCE_IDLE_ID;
			s_gpio_dynamic_wakeup_source_map[i].int_type = GPIO_INT_TYPE_MAX;
			//s_gpio_dynamic_wakeup_source_map[i].isr = NULL;
			s_gpio_is_setted_wake_status &= ~(((uint64_t)1 << s_gpio_dynamic_wakeup_source_map[i].id));

			/* Clear the hardware status during deregister */
			bk_gpio_disable_input(gpio_id);
			bk_gpio_disable_interrupt(gpio_id);

			GPIO_LOGD("%s[-]gpioid=%d\r\n", __func__, gpio_id);

			return BK_OK;
		}
	}

	GPIO_LOGE("gpio id:%d is not using \r\n", gpio_id);
	return BK_FAIL;
}

static void gpio_dynamic_wakeup_source_init(void)
{
	uint32_t i = 0;

	GPIO_LOGD("%s[+]gpio wakecnt=%d\r\n", __func__, CONFIG_GPIO_DYNAMIC_WAKEUP_SOURCE_MAX_CNT);
	//search the same id and replace it.
	for (i = 0; i < CONFIG_GPIO_DYNAMIC_WAKEUP_SOURCE_MAX_CNT; i++)
	{
		s_gpio_dynamic_wakeup_source_map[i].id = GPIO_WAKE_SOURCE_IDLE_ID;
	}

	GPIO_LOGD("%s[-]\r\n", __func__);
}
#else
bk_err_t bk_gpio_register_wakeup_source(gpio_id_t gpio_id,
                                                 gpio_int_type_t int_type)
{
	return BK_OK;
}

bk_err_t bk_gpio_unregister_wakeup_source(gpio_id_t gpio_id)
{
	return BK_OK;
}
#endif
#endif

#if defined(CONFIG_GPIO_KPSTAT_SUPPORT)
static void gpio_keep_status_init(void)
{
	//has configured in default map with static mode
#if defined(CONFIG_GPIO_DEFAULT_SET_SUPPORT)
	const gpio_default_map_t default_map[] = GPIO_DEFAULT_DEV_CONFIG;

	for (uint32_t i = 0; i < sizeof(default_map)/sizeof(gpio_default_map_t); i++)
	{
		//uses equal to avoid some guy maybe write other value
		if ((default_map[i].low_power_io_ctrl == GPIO_LOW_POWER_KEEP_INPUT_STATUS) ||
			(default_map[i].low_power_io_ctrl == GPIO_LOW_POWER_KEEP_OUTPUT_STATUS))
		{
			s_gpio_is_lowpower_keep_status |= ((uint64_t)0x1 << default_map[i].gpio_id);
		}
	}
#endif

#if defined(CONFIG_GPIO_DYNAMIC_KPSTAT_SUPPORT)
	GPIO_LOGD("%s[+]gpio wakecnt=%d\r\n", __func__, CONFIG_GPIO_DYNAMIC_KEEP_STATUS_MAX_CNT);
	for (uint32_t i = 0; i < CONFIG_GPIO_DYNAMIC_KEEP_STATUS_MAX_CNT; i++)
	{
		s_gpio_lowpower_keep_config[i].gpio_id = GPIO_LOWPOWER_KEEP_STATUS_IDLE_ID;
	}
#endif

	GPIO_LOGD("%s[-]\r\n", __func__);
}

#if defined(CONFIG_GPIO_DYNAMIC_KPSTAT_SUPPORT)
static void gpio_keep_status_config(void)
{
	uint32_t index;
	gpio_id_t gpio_id;
	gpio_config_t config;

	config.io_mode = GPIO_IO_DISABLE;
	config.pull_mode = GPIO_PULL_DISABLE;
	config.func_mode = GPIO_SECOND_FUNC_DISABLE;

	for(index = 0; index < CONFIG_GPIO_DYNAMIC_KEEP_STATUS_MAX_CNT; index++)
	{
		if(s_gpio_is_lowpower_keep_status & ((uint64_t)1 << s_gpio_lowpower_keep_config[index].gpio_id)) {
			gpio_id = s_gpio_lowpower_keep_config[index].gpio_id;
			config.io_mode = s_gpio_lowpower_keep_config[index].config.io_mode;
			config.pull_mode = s_gpio_lowpower_keep_config[index].config.pull_mode;
			config.func_mode = s_gpio_lowpower_keep_config[index].config.func_mode;
			bk_gpio_set_config(gpio_id, &config);
			os_printf("set config %d %d %d %x\r\n", config.io_mode, config.pull_mode, config.func_mode, bk_gpio_get_value(gpio_id));
		}
	}
}

bk_err_t bk_gpio_register_lowpower_keep_status(gpio_id_t gpio_id,
                                                 const gpio_config_t *config)
{
	uint32_t i = 0;
	GPIO_LOGI("[+]gpio=%d io_mode=%d pull_mode=%d func_mode=%d\r\n",
		gpio_id, config->io_mode, config->pull_mode, config->func_mode);

#if defined(CONFIG_GPIO_RETENTION_SUPPORT)
	if (config->io_mode == GPIO_OUTPUT_ENABLE && config->pull_mode == GPIO_PULL_UP_EN)
	{
		gpio_retention_map_set(gpio_id, GPIO_OUTPUT_STATE_HIGH);
	}
	else if (config->io_mode == GPIO_OUTPUT_ENABLE && config->pull_mode == GPIO_PULL_DOWN_EN)
	{
		gpio_retention_map_set(gpio_id, GPIO_OUTPUT_STATE_LOW);
	}
#endif

	//search the same id and replace it.
	for(i = 0; i < CONFIG_GPIO_DYNAMIC_KEEP_STATUS_MAX_CNT; i++)
	{
		if(s_gpio_lowpower_keep_config[i].gpio_id == gpio_id) {
			s_gpio_lowpower_keep_config[i].config.io_mode = config->io_mode;
			s_gpio_lowpower_keep_config[i].config.pull_mode = config->pull_mode;
			s_gpio_lowpower_keep_config[i].config.func_mode = config->func_mode;
			s_gpio_is_lowpower_keep_status |= ((uint64_t)1 << gpio_id);

			GPIO_LOGD("gpio=%d io_mode=%d pull_mode=%d func_mode=%d\r\n",
				gpio_id, config->io_mode, config->pull_mode, config->func_mode);
			return BK_OK;
		}
	}

	//serach the first idle id
	for(i = 0; i < CONFIG_GPIO_DYNAMIC_KEEP_STATUS_MAX_CNT; i++)
	{
		if(s_gpio_lowpower_keep_config[i].gpio_id == GPIO_LOWPOWER_KEEP_STATUS_IDLE_ID) {
			s_gpio_lowpower_keep_config[i].gpio_id = gpio_id;
			s_gpio_lowpower_keep_config[i].config.io_mode = config->io_mode;
			s_gpio_lowpower_keep_config[i].config.pull_mode = config->pull_mode;
			s_gpio_lowpower_keep_config[i].config.func_mode = config->func_mode;
			s_gpio_is_lowpower_keep_status |= ((uint64_t)1 << gpio_id);

			GPIO_LOGI("gpio=%d io_mode=%d pull_mode=%d func_mode=%d\r\n",
				gpio_id, config->io_mode, config->pull_mode, config->func_mode);

			return BK_OK;
		}
	}

	GPIO_LOGE("too much(%d) GPIO is setted keep status\r\n", CONFIG_GPIO_DYNAMIC_KEEP_STATUS_MAX_CNT);
	for(i = 0; i < CONFIG_GPIO_DYNAMIC_KEEP_STATUS_MAX_CNT; i++)
	{
		GPIO_LOGE("gpio id:%d is using \r\n", s_gpio_lowpower_keep_config[i].gpio_id);
	}
	return BK_FAIL;
}

bk_err_t bk_gpio_unregister_lowpower_keep_status(gpio_id_t gpio_id)
{
	gpio_config_t config;
	uint32_t i = 0;

	config.io_mode = GPIO_IO_DISABLE;
	config.pull_mode = GPIO_PULL_DISABLE;
	config.func_mode = GPIO_SECOND_FUNC_DISABLE;

	/* search the same id and replace it.*/
	for(i = 0; i < CONFIG_GPIO_DYNAMIC_KEEP_STATUS_MAX_CNT; i++)
	{
		if(s_gpio_lowpower_keep_config[i].gpio_id == gpio_id) {
			s_gpio_lowpower_keep_config[i].gpio_id = GPIO_LOWPOWER_KEEP_STATUS_IDLE_ID;
			s_gpio_is_lowpower_keep_status &= ~(((uint64_t)1 << gpio_id));
			bk_gpio_set_config(gpio_id, &config);
			s_gpio_lowpower_keep_config[i].config.io_mode = config.io_mode;
			s_gpio_lowpower_keep_config[i].config.pull_mode = config.pull_mode;
			s_gpio_lowpower_keep_config[i].config.func_mode = config.func_mode;

			GPIO_LOGD("%s[-]gpioid=%d\r\n", __func__, gpio_id);

			return BK_OK;
		}
	}

	GPIO_LOGE("gpio id:%d is not using \r\n", gpio_id);
	return BK_FAIL;
}
#endif
#endif

#if defined(CONFIG_GPIO_DEFAULT_SET_SUPPORT)
static void gpio_default_map_init(void)
{
	const gpio_default_map_t default_map[] = GPIO_DEFAULT_DEV_CONFIG;
	gpio_id_t gpio_id;
	IOMX_CODE_T code;

	for (int i = 0; i < sizeof(default_map)/sizeof(gpio_default_map_t); i++)
	{
		gpio_id = default_map[i].gpio_id;

		if (default_map[i].gpio_skip == GPIO_INIT_SKIP){
			GPIO_LOGD("skipping gpio_id : %d", gpio_id);
			continue;
		}

		bk_iomx_disable_pull(gpio_id);
		bk_iomx_disable_interrupt(gpio_id);

		GPIO_LOGD("gpio_id: %d, second_func_en:%d, second_func_dev %d, low_power_io_ctrl:%d", gpio_id,
					default_map[i].second_func_en, default_map[i].second_func_dev,
					default_map[i].low_power_io_ctrl);

		GPIO_LOGD("int_en: %d, int_type:%d \r\n",
				default_map[i].int_en, default_map[i].int_type);

		//function mode
		if (default_map[i].second_func_en) {
			code = contert_gpio_dev_to_iomx_code(default_map[i].second_func_dev);
			bk_iomx_set_gpio_func(gpio_id, code);
		}

		//low power
		if (default_map[i].low_power_io_ctrl == GPIO_LOW_POWER_KEEP_OUTPUT_STATUS)
			bk_iomx_set_gpio_func(gpio_id, FUNC_CODE_OUTPUT);
		else if (default_map[i].low_power_io_ctrl == GPIO_LOW_POWER_KEEP_INPUT_STATUS)
			bk_iomx_set_gpio_func(gpio_id, FUNC_CODE_INPUT);

		//io mode
		switch(default_map[i].io_mode)
		{
			case GPIO_IO_DISABLE:
				bk_iomx_set_gpio_func(gpio_id, FUNC_CODE_HIGH_Z);
				break;
			case GPIO_OUTPUT_ENABLE:
				//NOTES:special combine use it with pull mode
				if (default_map[i].pull_mode == GPIO_PULL_DOWN_EN)
					bk_iomx_output_low(gpio_id);
				if (default_map[i].pull_mode == GPIO_PULL_UP_EN)
					bk_iomx_output_high(gpio_id);
				break;
			case GPIO_INPUT_ENABLE:
				bk_iomx_set_gpio_func(gpio_id, FUNC_CODE_INPUT);
				break;
			default:
				break;
		}

		//pull mode
		switch(default_map[i].pull_mode)
		{
			case GPIO_PULL_DISABLE:
				bk_iomx_disable_pull(gpio_id);
				break;
			case GPIO_PULL_DOWN_EN:
				bk_iomx_pull_down(gpio_id);
				break;
			case GPIO_PULL_UP_EN:
				bk_iomx_pull_up(gpio_id);
				break;
			default:
				break;
		}

		//interrupt
		if (default_map[i].int_en) {
			bk_iomx_set_interrupt_type(gpio_id, default_map[i].int_type);

			for (volatile int j = 0; j < 1000; j++);    //Before enable the interrupt,wait for the internal stability of the chip
			bk_iomx_enable_interrupt(gpio_id);
		}

		bk_iomx_clear_interrupt(gpio_id);

		//driver_capacity
		bk_iomx_set_capacity(gpio_id, default_map[i].driver_capacity);
	}
}
#endif