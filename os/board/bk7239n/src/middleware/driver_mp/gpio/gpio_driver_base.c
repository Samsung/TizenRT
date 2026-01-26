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
#include "icu_driver.h"
#include <os/mem.h>
#include "gpio_hal.h"
#include "gpio_driver_base.h"
#include "gpio_driver.h"
#include <driver/gpio_types.h>
#include <driver/int.h>
#include "amp_lock_api.h"
#if defined(CONFIG_AON_PMU)
#include "aon_pmu_driver.h"
#endif
#if (defined(CONFIG_SYSTEM_CTRL))
#include "sys_driver.h"
#endif
#if defined(CONFIG_GPIO_SIMULATE_UART_WRITE)
#include "bk_misc.h"
#endif
#if defined(CONFIG_GPIO_ANA_WAKEUP_SUPPORT)
#include "sys_driver.h"
#endif
#if defined(CONFIG_USR_GPIO_CFG_EN)
#include "usr_gpio_cfg.h"
#endif


#define GPIO_RETURN_ON_INVALID_ID(id) do {\
				if ((id) >= SOC_GPIO_NUM) {\
					return BK_ERR_GPIO_CHAN_ID;\
				}\
			} while(0)

#define GPIO_RETURN_ON_INVALID_INT_TYPE_MODE(mode) do {\
				if ((mode) >= GPIO_INT_TYPE_MAX) {\
					return BK_ERR_GPIO_INVALID_INT_TYPE;\
				}\
			} while(0)

#define GPIO_RETURN_ON_INVALID_IO_MODE(mode) do {\
				if (((mode)) >= GPIO_IO_INVALID) {\
					return BK_ERR_GPIO_INVALID_MODE;\
				}\
			} while(0)

#define GPIO_RETURN_ON_INVALID_PULL_MODE(mode) do {\
				if (((mode)) >= GPIO_PULL_INVALID) {\
					return BK_ERR_GPIO_INVALID_MODE;\
				}\
			} while(0)


gpio_driver_t s_gpio = {
	.hal.hw = (gpio_hw_t *)GPIO_LL_REG_BASE,
};

gpio_isr_t s_gpio_isr[SOC_GPIO_NUM] = {0};

static bool s_gpio_is_init          = false;

#if defined(CONFIG_GPIO_ANA_WAKEUP_SUPPORT)
#define GPIO_ANA_WAKEUP_MAX (2)
static uint32_t s_wkup_cnt          = 0;
static gpio_wakeup_config_t s_wkup_cfg[GPIO_ANA_WAKEUP_MAX] = {0};
#endif

#if defined(CONFIG_GPIO_WAKEUP_SUPPORT)
#define GPIO_BAK_INT_TYPE_REGS_NUM     ((GPIO_NUM_MAX-1)/16+1)
#define GPIO_BAK_INT_ENABLE_REGS_NUM   ((GPIO_NUM_MAX-1)/32+1)
static uint64_t s_wakeup_id                  = 0;
static uint64_t s_gpio_is_setted_wake_status = 0;
static uint16_t s_gpio_bak_regs[GPIO_NUM_MAX]               = {0};
static uint32_t s_gpio_bak_int_type_regs[GPIO_BAK_INT_TYPE_REGS_NUM]     = {0};
static uint32_t s_gpio_bak_int_enable_regs[GPIO_BAK_INT_ENABLE_REGS_NUM] = {0};

static gpio_id_t s_gpio_wakeup_gpio_id       = SOC_GPIO_NUM;
static bool bk_gpio_get_aon_pmu_deepsleep_flag();
static void gpio_record_wakeup_pin_id(void);

#if defined(CONFIG_GPIO_DYNAMIC_WAKEUP_SUPPORT)
#ifndef CONFIG_GPIO_DYNAMIC_WAKEUP_SOURCE_MAX_CNT
#define CONFIG_GPIO_DYNAMIC_WAKEUP_SOURCE_MAX_CNT (4)
#endif
#define GPIO_WAKE_SOURCE_IDLE_ID                  (SOC_GPIO_NUM)
typedef struct
{
	gpio_id_t id;
	gpio_int_type_t int_type;
	//gpio_isr_t isr;
} gpio_dynamic_wakeup_t;
static gpio_dynamic_wakeup_t s_gpio_dynamic_wakeup_source_map[CONFIG_GPIO_DYNAMIC_WAKEUP_SOURCE_MAX_CNT];
static void gpio_dynamic_wakeup_source_init(void);
#endif

#if defined(CONFIG_GPIO_KPSTAT_SUPPORT)
static uint64_t s_gpio_is_lowpower_keep_status = 0;
static void gpio_lowpower_keep_status_init(void);

#if defined(CONFIG_GPIO_DYNAMIC_KPSTAT_SUPPORT)
#ifndef CONFIG_GPIO_DYNAMIC_KEEP_STATUS_MAX_CNT
#define CONFIG_GPIO_DYNAMIC_KEEP_STATUS_MAX_CNT (4)
#endif
#define GPIO_LOWPOWER_KEEP_STATUS_IDLE_ID (SOC_GPIO_NUM)
typedef struct
{
	gpio_id_t gpio_id;
	gpio_config_t config;
} gpio_dynamic_keep_status_t;
static gpio_dynamic_keep_status_t s_gpio_lowpower_keep_config[CONFIG_GPIO_DYNAMIC_KEEP_STATUS_MAX_CNT];
static void gpio_lowpower_dynamic_keep_status_config(void);
#endif // CONFIG_GPIO_DYNAMIC_KPSTAT_SUPPORT
#endif // CONFIG_GPIO_KPSTAT_SUPPORT
#endif // CONFIG_GPIO_WAKEUP_SUPPORT


static void gpio_isr(void);
bk_err_t bk_gpio_driver_init(void)
{
	//avoid re-init caused some info lost
	if(s_gpio_is_init) {
		GPIO_LOGI("%s:has inited \r\n", __func__);
		return BK_OK;
	}

	gpio_hal_init(&s_gpio.hal);
	/*
	 * @NOTES:
	 * GPIO wakeup system from deepsleep(system will power off at deep sleep status),
	 * the GPIO interrupt status is still keeped as GPIO is at AON power domain.
	 * so bake up the specific gpio id which wakeup system, and then clear
	 * all of the previous GPIO status before init GPIO ID relate function.
	 */
#if defined(CONFIG_GPIO_WAKEUP_SUPPORT)
	gpio_record_wakeup_pin_id();

#if defined(CONFIG_GPIO_KPSTAT_SUPPORT)
	gpio_lowpower_keep_status_init();
#endif

#if defined(CONFIG_GPIO_DYNAMIC_WAKEUP_SUPPORT)
	gpio_dynamic_wakeup_source_init();
#endif
#endif

#if defined(CONFIG_GPIO_DEFAULT_SET_SUPPORT)
	gpio_hal_default_map_init(&s_gpio.hal);
#endif

#if (CONFIG_CPU_CNT > 1) && defined(CONFIG_MAILBOX)
	amp_res_init(AMP_RES_ID_GPIO);
#endif

	//Move ISR to last to avoid other resouce doesn't finish but isr has came.
	//F.E:GPIO wakeup deepsleep.
#if defined(CONFIG_TZ) && ( CONFIG_SPE == 0)
	bk_int_isr_register(INT_SRC_GPIO_NS, gpio_isr, NULL);
#else
	bk_int_isr_register(INT_SRC_GPIO, gpio_isr, NULL);
#endif

	//interrupt to CPU enable
#if (defined(CONFIG_SYSTEM_CTRL))
	sys_drv_int_group2_enable(GPIO_INTERRUPT_CTRL_BIT);
#else
	icu_enable_gpio_interrupt();
#endif

	s_gpio_is_init = true;

	return BK_OK;
}

bk_err_t bk_gpio_driver_deinit(void)
{
	if(!s_gpio_is_init)
	{
		GPIO_LOGI("%s:isn't init \r\n", __func__);
		return BK_OK;
	}

	//interrupt to CPU disable
#if (defined(CONFIG_SYSTEM_CTRL))
	sys_drv_int_group2_disable(GPIO_INTERRUPT_CTRL_BIT);
#else
	icu_disable_gpio_interrupt();
#endif

	s_gpio_is_init = false;

	return BK_OK;
}

void bk_gpio_set_value(gpio_id_t id, uint32_t v)
{
	gpio_hal_set_value(&s_gpio.hal, id, v);
}

uint32_t bk_gpio_get_value(gpio_id_t id)
{
	return gpio_hal_get_value(&s_gpio.hal, id);
}

bk_err_t bk_gpio_enable_output(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	return gpio_hal_output_enable(&s_gpio.hal, gpio_id, 1);
}

bk_err_t bk_gpio_disable_output(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	return gpio_hal_output_enable(&s_gpio.hal, gpio_id, 0);
}

bk_err_t bk_gpio_enable_input(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	return gpio_hal_input_enable(&s_gpio.hal, gpio_id, 1);
}

bk_err_t bk_gpio_disable_input(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	return gpio_hal_input_enable(&s_gpio.hal, gpio_id, 0);
}

bk_err_t bk_gpio_enable_pull(gpio_id_t gpio_id )
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	return gpio_hal_pull_enable(&s_gpio.hal, gpio_id, 1);
}

bk_err_t bk_gpio_disable_pull(gpio_id_t gpio_id )
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	return gpio_hal_pull_enable(&s_gpio.hal, gpio_id, 0);
}

bk_err_t bk_gpio_pull_up(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	gpio_hal_pull_enable(&s_gpio.hal, gpio_id, 1);
	return gpio_hal_pull_up_enable(&s_gpio.hal, gpio_id, 1);
}

bk_err_t bk_gpio_pull_down(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	gpio_hal_pull_enable(&s_gpio.hal, gpio_id, 1);
	return gpio_hal_pull_up_enable(&s_gpio.hal, gpio_id, 0);
}

bk_err_t bk_gpio_set_output_high(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	return gpio_hal_set_output_value(&s_gpio.hal, gpio_id, 1);
}

bk_err_t bk_gpio_set_output_low(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	return gpio_hal_set_output_value(&s_gpio.hal, gpio_id, 0);
}

bool bk_gpio_get_input(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	return gpio_hal_get_input(&s_gpio.hal, gpio_id);
}

//MAX capactiy:3
bool bk_gpio_set_capacity(gpio_id_t gpio_id, uint32 capacity)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	return gpio_hal_set_capacity(&s_gpio.hal, gpio_id, capacity);
}

bk_err_t bk_gpio_set_config(gpio_id_t gpio_id, const gpio_config_t *config)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);
	GPIO_RETURN_ON_INVALID_IO_MODE(config->io_mode);
	GPIO_RETURN_ON_INVALID_PULL_MODE(config->pull_mode);

	return gpio_hal_set_config(&s_gpio.hal, gpio_id, config);
}


/* Enable GPIO  interrupt.
 */
bk_err_t bk_gpio_register_isr(gpio_id_t gpio_id, gpio_isr_t isr)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);
	s_gpio_isr[gpio_id] = isr;

	return BK_OK;
}

//This function just enable the select GPIO can report IRQ to CPU
bk_err_t bk_gpio_enable_interrupt(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);
	//Before enable the interrupt,wait for the internal stability of the chip
	for (volatile int i = 0; i < 1000; i++);    

	return gpio_hal_enable_interrupt(&s_gpio.hal, gpio_id);
}

bk_err_t bk_gpio_disable_interrupt(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	//WARNING:We can't call icu_enable_gpio_interrupt/sys_drv_int_group2_disable in this function
	//If more then one GPIO_ID enable interrupt, here disable the IRQ to CPU, it caused other GPIO ID can't work

	gpio_hal_disable_interrupt(&s_gpio.hal, gpio_id);

	return BK_OK;
}

bk_err_t bk_gpio_clear_interrupt(gpio_id_t gpio_id)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	gpio_hal_clear_chan_interrupt_status(&s_gpio.hal, gpio_id);

	return BK_OK;
}

bk_err_t bk_gpio_set_interrupt_type(gpio_id_t gpio_id, gpio_int_type_t type)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);
	GPIO_RETURN_ON_INVALID_INT_TYPE_MODE(type);

	return gpio_hal_set_int_type(&s_gpio.hal, gpio_id, type);
}

static void gpio_isr(void)
{
	gpio_hal_t *hal = &s_gpio.hal;
	gpio_interrupt_status_t gpio_status;
	int gpio_id;

	gpio_hal_get_interrupt_status(hal, &gpio_status);

	for (gpio_id = 0; gpio_id < SOC_GPIO_NUM; gpio_id++) {
		if (gpio_hal_is_interrupt_triggered(hal, gpio_id, &gpio_status)) {

			//if gpio_id is not within default config, continue
#if defined(CONFIG_USR_GPIO_CFG_EN)
			const gpio_default_map_t default_map[] = GPIO_DEFAULT_DEV_CONFIG;
			int i = 0;
			for(i = 0; i < sizeof(default_map)/sizeof(gpio_default_map_t); i++) {
				if(gpio_id == default_map[i].gpio_id) {
					break;
				}
			}

			if(i == sizeof(default_map)/sizeof(gpio_default_map_t)) {
				continue;
			}
#endif

#if 0	//remove it:wakeup response should be controlled in the APP.
			/* if many gpio wakeup at the same time, it will called many times */
			if(s_gpio_is_setted_wake_status & ((uint64_t)0x1 << gpio_id)) {
				gpio_wakeup_default_isr(gpio_id);
			}
#endif
			if (s_gpio_isr[gpio_id]) {
				GPIO_LOGD("gpio int: index:%d \r\n",gpio_id);
				s_gpio_isr[gpio_id](gpio_id);
			}
			bk_gpio_clear_interrupt(gpio_id);
		}
	}

#if (!defined(CONFIG_SOC_BK7236XX)) && (!defined(CONFIG_SOC_BK7239XX)) && (!defined(CONFIG_SOC_BK7286XX))
	//move it after callback:if isr is caused by level type,
	//clear IRQ status and doesn't disable IRQ, then it causes a new ISR
	gpio_hal_clear_interrupt_status(hal, &gpio_status);
#endif
}
static gpio_ctrl_ldo_t s_gpio_ctrl_ldo_output[] = GPIO_CTRL_LDO_MAP;
bk_err_t bk_gpio_ctrl_external_ldo(gpio_ctrl_ldo_module_e module,gpio_id_t gpio_id,gpio_output_state_e value)
{
	uint32_t i = 0;

	if(value == GPIO_OUTPUT_STATE_HIGH)//output higt
	{
		for(i = 0; i < sizeof(s_gpio_ctrl_ldo_output)/sizeof(gpio_ctrl_ldo_t); i++)
		{
			if(s_gpio_ctrl_ldo_output[i].gpio_id >= 0 && s_gpio_ctrl_ldo_output[i].gpio_id < GPIO_NUM_MAX)
			{
				if(gpio_id == s_gpio_ctrl_ldo_output[i].gpio_id)
				{
					if (s_gpio_ctrl_ldo_output[i].ldo_state == 0) {
						/*gpio dev unmap*/
						BK_LOG_ON_ERR(bk_gpio_disable_output(s_gpio_ctrl_ldo_output[i].gpio_id));
						BK_LOG_ON_ERR(bk_gpio_disable_input(s_gpio_ctrl_ldo_output[i].gpio_id));
						BK_LOG_ON_ERR(bk_gpio_disable_pull(gpio_id));
						BK_LOG_ON_ERR(bk_gpio_disable_interrupt(gpio_id));
						gpio_hal_func_unmap(&s_gpio.hal, gpio_id);

						BK_LOG_ON_ERR(bk_gpio_set_capacity(s_gpio_ctrl_ldo_output[i].gpio_id, 0));
						BK_LOG_ON_ERR(bk_gpio_disable_input(s_gpio_ctrl_ldo_output[i].gpio_id));
						BK_LOG_ON_ERR(bk_gpio_enable_output(s_gpio_ctrl_ldo_output[i].gpio_id));
						BK_LOG_ON_ERR(bk_gpio_set_output_high(s_gpio_ctrl_ldo_output[i].gpio_id));
					}
					s_gpio_ctrl_ldo_output[i].ldo_state |= (0x1 << module);
				}
			}
		}
	}
	else if(value == GPIO_OUTPUT_STATE_LOW)//output low
	{
		for(i = 0; i < sizeof(s_gpio_ctrl_ldo_output)/sizeof(gpio_ctrl_ldo_t); i++)
		{
			if(s_gpio_ctrl_ldo_output[i].gpio_id >= 0 && s_gpio_ctrl_ldo_output[i].gpio_id < GPIO_NUM_MAX)
			{
				if(gpio_id == s_gpio_ctrl_ldo_output[i].gpio_id)
				{
					s_gpio_ctrl_ldo_output[i].ldo_state &= ~(0x1 << module);

					if(s_gpio_ctrl_ldo_output[i].ldo_state == 0x0)
					{
						/*gpio dev unmap*/
						BK_LOG_ON_ERR(bk_gpio_disable_output(s_gpio_ctrl_ldo_output[i].gpio_id));
						BK_LOG_ON_ERR(bk_gpio_disable_input(s_gpio_ctrl_ldo_output[i].gpio_id));
						BK_LOG_ON_ERR(bk_gpio_disable_pull(gpio_id));
						BK_LOG_ON_ERR(bk_gpio_disable_interrupt(gpio_id));
						gpio_hal_func_unmap(&s_gpio.hal, gpio_id);

						BK_LOG_ON_ERR(bk_gpio_set_capacity(s_gpio_ctrl_ldo_output[i].gpio_id, 0));
						BK_LOG_ON_ERR(bk_gpio_disable_input(s_gpio_ctrl_ldo_output[i].gpio_id));
						BK_LOG_ON_ERR(bk_gpio_enable_output(s_gpio_ctrl_ldo_output[i].gpio_id));
						BK_LOG_ON_ERR(bk_gpio_set_output_low(s_gpio_ctrl_ldo_output[i].gpio_id));
					}
				}
			}
		}
	}
	else
	{
	}

	return BK_OK;
}
#if defined(CONFIG_GPIO_WAKEUP_SUPPORT)

static void gpio_dump_baked_regs(bool configs, bool int_type_status, bool int_en_status)
{
#if defined(CONFIG_GPIO_WAKEUP_DEBUG)
	gpio_id_t gpio_id = 0;

	GPIO_LOGD("%s[+]\r\n", __func__);

	GPIO_LOGD("is_setted_wake_status h= 0x%x, l=0x%x\r\n", (uint32_t)(s_gpio_is_setted_wake_status>>32), (uint32_t)s_gpio_is_setted_wake_status);

	if(configs)
	{
		for(gpio_id = 0; gpio_id < SOC_GPIO_NUM; gpio_id++)
		{
			GPIO_LOGD("s_gpio_bak_regs[%d]=0x%x\r\n", gpio_id, s_gpio_bak_regs[gpio_id]);
		}
	}

	if(int_type_status)
	{
		for(gpio_id = 0; gpio_id < sizeof(s_gpio_bak_int_type_regs)/sizeof(s_gpio_bak_int_type_regs[0]); gpio_id++)
		{
			GPIO_LOGD("int_type_regs[%d]=0x%x\r\n", gpio_id, s_gpio_bak_int_type_regs[gpio_id]);
		}
	}

	if(int_en_status)
	{
		for(gpio_id = 0; gpio_id < sizeof(s_gpio_bak_int_enable_regs)/sizeof(s_gpio_bak_int_enable_regs[0]); gpio_id++)
		{
			GPIO_LOGD("int_enable_regs[%d]=0x%x\r\n", gpio_id, s_gpio_bak_int_enable_regs[gpio_id]);
		}
	}

	GPIO_LOGD("%s[-]\r\n", __func__);
#endif
}

static void gpio_dump_regs(bool configs, bool int_status)
{
#if defined(CONFIG_GPIO_WAKEUP_DEBUG)
	gpio_id_t gpio_id = 0;

	GPIO_LOGD("%s[+]\r\n", __func__);

	if(configs)
	{
		for(gpio_id = 0; gpio_id < SOC_GPIO_NUM; gpio_id++)
		{
			///gpio_struct_dump(gpio_id);
			GPIO_LOGD("gpio[%d]=0x%x\r\n", gpio_id, *(volatile uint32_t*)(GPIO_LL_REG_BASE + 4*gpio_id));
		}
	}

	//WARNING:BK7256 has this 9 regs, maybe other project doesn't has this 9 REGs
	if(int_status)
	{
		for(gpio_id = 0; gpio_id < 9; gpio_id++)
		{
			///gpio_struct_dump(gpio_id);
			GPIO_LOGD("REG0x%x=0x%x\r\n", (GPIO_LL_REG_BASE + 4*(0x40+gpio_id)), *(volatile uint32_t*)(GPIO_LL_REG_BASE + 4*(0x40+gpio_id)));
		}
	}
	GPIO_LOGD("%s[-]\r\n", __func__);
#endif
}

void gpio_get_interrupt_status(uint32_t *h_status, uint32_t *l_status)
{
	gpio_hal_t *hal = &s_gpio.hal;
	gpio_interrupt_status_t gpio_status;

	gpio_hal_get_interrupt_status(hal, &gpio_status);
	if(h_status)
		*h_status = gpio_status.gpio_32_64_int_status;
	if(l_status)
		*l_status = gpio_status.gpio_0_31_int_status;
}

#if 0	//remove it:wakeup response should be controlled in the APP.
//If APP uses level IRQ type, it should avoid the ISR comes always.
static void gpio_wakeup_default_isr(gpio_id_t gpio_id)
{
	//NOTES:to avoid interrupt comes many times if uses level IRQ type
	//disable IRQ and enable it again in gpio_config_wakeup_function
	bk_gpio_disable_interrupt(gpio_id);
	bk_gpio_clear_interrupt(gpio_id);

	GPIO_LOGD("gpio int: index:%d \r\n", gpio_id);
}
#endif
/* Added fault tolerance for GPIO wakeup.
 * The act of increasing the pull-down voltage to prevent jitter interference.
 */
static void gpio_wakeup_set_pin_voltage_status(gpio_id_t gpio_id, gpio_int_type_t type)
{
	GPIO_LOGD("%s[+]\r\n", __func__);

	switch(type)
	{
		case GPIO_INT_TYPE_LOW_LEVEL:
			bk_gpio_pull_up(gpio_id);
			GPIO_LOGD("%s GPIO_INT_TYPE: LOW_LEVEL Pull_up!\r\n", __func__);
			break;
		case GPIO_INT_TYPE_HIGH_LEVEL:
			bk_gpio_pull_down(gpio_id);
			GPIO_LOGD("%s GPIO_INT_TYPE: HIGH_LEVEL Pull_down!\r\n", __func__);
			break;
		case GPIO_INT_TYPE_RISING_EDGE:
			bk_gpio_pull_down(gpio_id);
			GPIO_LOGD("%s GPIO_INT_TYPE: RISING_EDGE Pull_down!\r\n", __func__);
			break;
		case GPIO_INT_TYPE_FALLING_EDGE:
			bk_gpio_pull_up(gpio_id);
			GPIO_LOGD("%s GPIO_INT_TYPE: FALLING_EDGE Pull_up!\r\n", __func__);
			break;
		case GPIO_INT_TYPE_MAX:
			GPIO_LOGI("%s Please set fill in the mode correctly!\r\n", __func__);
			break;
		default:
			break;
	}

	GPIO_LOGD("%s[-]\r\n", __func__);
}

static void gpio_config_wakeup_function(void)
{
	uint32_t i = 0;
	uint64_t wakeup_id = 0;
	gpio_wakeup_t gpio_wakeup_map[] = GPIO_STATIC_WAKEUP_SOURCE_MAP;
	gpio_hal_t *hal = &s_gpio.hal;

	GPIO_LOGD("%s[+]\r\n", __func__);

	s_gpio_is_setted_wake_status = 0;
	for(i = 0; i < sizeof(gpio_wakeup_map)/sizeof(gpio_wakeup_t); i++)
	{
		//maybe the PIN is re-used as SECOND_FUNCTION and GPIO,F.E:UART RXD re-uses as wakeup PIN
		//This API is frequently called and prints log in the low-power keepalive
		// gpio_hal_func_unmap(hal, gpio_wakeup_map[i].id);
		gpio_hal_sencond_function_enable(hal, gpio_wakeup_map[i].id, 0);
		gpio_wakeup_set_pin_voltage_status(gpio_wakeup_map[i].id, gpio_wakeup_map[i].int_type);
		bk_gpio_set_interrupt_type(gpio_wakeup_map[i].id, gpio_wakeup_map[i].int_type);
		//bk_gpio_enable_interrupt(gpio_wakeup_map[i].id);
		wakeup_id |= (uint64_t)0x1<<gpio_wakeup_map[i].id;
		s_gpio_is_setted_wake_status |= ((uint64_t)1 << gpio_wakeup_map[i].id);
	}

#if defined(CONFIG_GPIO_DEFAULT_SET_SUPPORT)
	const gpio_default_map_t default_map[] = GPIO_DEFAULT_DEV_CONFIG;
	for(i = 0; i < sizeof(default_map)/sizeof(gpio_default_map_t); i++)
	{
		if(default_map[i].low_power_io_ctrl == GPIO_LOW_POWER_KEEP_INPUT_STATUS)
		{
			s_gpio_is_setted_wake_status |= ((uint64_t)1 << default_map[i].gpio_id);
		}
	}
#endif

#ifdef CONFIG_GPIO_DYNAMIC_WAKEUP_SUPPORT
	for(i = 0; i < CONFIG_GPIO_DYNAMIC_WAKEUP_SOURCE_MAX_CNT; i++)
	{
		if(s_gpio_dynamic_wakeup_source_map[i].id != GPIO_WAKE_SOURCE_IDLE_ID) {
			//maybe the PIN is re-used as SECOND_FUNCTION and GPIO,F.E:UART RXD re-uses as wakeup PIN
			//This API is frequently called and prints log in the low-power keepalive
			// gpio_hal_func_unmap(hal, s_gpio_dynamic_wakeup_source_map[i].id);
			gpio_hal_sencond_function_enable(hal, s_gpio_dynamic_wakeup_source_map[i].id, 0);
			gpio_wakeup_set_pin_voltage_status(s_gpio_dynamic_wakeup_source_map[i].id, s_gpio_dynamic_wakeup_source_map[i].int_type);
			bk_gpio_set_interrupt_type(s_gpio_dynamic_wakeup_source_map[i].id, s_gpio_dynamic_wakeup_source_map[i].int_type);
			//bk_gpio_enable_interrupt(s_gpio_dynamic_wakeup_source_map[i].id);
			wakeup_id |= (uint64_t)0x1<<s_gpio_dynamic_wakeup_source_map[i].id;
			s_gpio_is_setted_wake_status |= ((uint64_t)1 << s_gpio_dynamic_wakeup_source_map[i].id);
		}
	}
#endif

	s_wakeup_id = wakeup_id;
	/*
	 * !!! NOTES NOTES !!!
	 * ASIC switch PIN function to GPIO Input mode(gpio_hal_func_unmap switch from SECOND function to GPIO),
	 * it takes 3 cycles(GPIO uses 32K clock) to be stable.
	 * If in the 3 cycles enable INPUT level/edge check, it will report an error status.
	 * so enable gpio input irq, should wait enough time.
	 */
#if 0
	delay_us(125);	//125 == ((3+1)/32) * 1000 us

	//Move the enable interrupt after dealy 125 us.
	for(i = 0; i < GPIO_NUM_MAX; i++)
	{
		if(wakeup_id & (0x1<<i))
			bk_gpio_enable_interrupt(i);
	}
#endif
	GPIO_LOGD("%s[-]set wake src h=0x%0x, l=0x%0x\r\n", __func__, (uint32_t)(s_gpio_is_setted_wake_status>>32), (uint32_t)s_gpio_is_setted_wake_status) ;
}
/*
To save the 125us delay  in the gpio driver, interrupt enable call before wifi
This api is an internal interface and is not used externally
*/
bk_err_t gpio_enable_interrupt_mult_for_wake(void)
{
	uint32_t i = 0;

	for(i; i < GPIO_NUM_MAX; i++)
	{
		if(s_wakeup_id & (0x1ULL<<i))
			bk_gpio_enable_interrupt(i);
	}

	return BK_OK;
}

static void gpio_config_low_power_wakeup_pin(void)
{
	uint32_t i = 0;

	gpio_wakeup_t gpio_wakeup_map[] = GPIO_STATIC_WAKEUP_SOURCE_MAP;
	for(i = 0; i < sizeof(gpio_wakeup_map)/sizeof(gpio_wakeup_t); i++)
	{
		bk_gpio_disable_output(gpio_wakeup_map[i].id);
		bk_gpio_enable_input(gpio_wakeup_map[i].id);
	}

#ifdef CONFIG_GPIO_DYNAMIC_WAKEUP_SUPPORT
	for(i = 0; i < CONFIG_GPIO_DYNAMIC_WAKEUP_SOURCE_MAX_CNT; i++)
	{
		if(s_gpio_dynamic_wakeup_source_map[i].id != GPIO_WAKE_SOURCE_IDLE_ID) {
			bk_gpio_disable_output(s_gpio_dynamic_wakeup_source_map[i].id);
			bk_gpio_enable_input(s_gpio_dynamic_wakeup_source_map[i].id);
		}
	}
#endif
}

gpio_id_t bk_gpio_get_wakeup_gpio_id()
{
	GPIO_LOGD("GET wakeup gpio_id: %d \r\n", s_gpio_wakeup_gpio_id);
	return s_gpio_wakeup_gpio_id;
}

static void bk_gpio_set_wakeup_gpio_id(gpio_id_t gpio_id)
{
	/* Obatain the First wake source GPIO ID*/
	if (s_gpio_wakeup_gpio_id == SOC_GPIO_NUM) {
		s_gpio_wakeup_gpio_id = gpio_id;
		bk_gpio_disable_interrupt(gpio_id);
	}
	GPIO_LOGI("SET wakeup gpio_id: %d \r\n", s_gpio_wakeup_gpio_id);
}

static void gpio_record_wakeup_pin_id(void)
{
	gpio_hal_t *hal = &s_gpio.hal;
	gpio_interrupt_status_t gpio_status;
	gpio_id_t gpio_id;

	if(bk_gpio_get_aon_pmu_deepsleep_flag()) {
		gpio_hal_get_interrupt_status(hal, &gpio_status);
		for (gpio_id = GPIO_0; gpio_id < SOC_GPIO_NUM; gpio_id++) {
			if (gpio_hal_is_interrupt_triggered(hal, gpio_id, &gpio_status)) {
				gpio_hal_clear_chan_interrupt_status(hal, gpio_id);
				bk_gpio_set_wakeup_gpio_id(gpio_id);
				return;
			}
		}
	}
}

static bool bk_gpio_get_aon_pmu_deepsleep_flag()
{
#if defined(CONFIG_AON_PMU)
	return (aon_pmu_drv_reg_get(PMU_REG2) & BIT(BIT_SLEEP_FLAG_DEEP_SLEEP));
#endif
	return false;
}

#ifdef CONFIG_GPIO_DYNAMIC_WAKEUP_SUPPORT
static void gpio_dynamic_wakeup_source_init(void)
{
	uint32_t i = 0;

	GPIO_LOGD("%s[+]gpio wakecnt=%d\r\n", __func__, CONFIG_GPIO_DYNAMIC_WAKEUP_SOURCE_MAX_CNT);
	//search the same id and replace it.
	for(i = 0; i < CONFIG_GPIO_DYNAMIC_WAKEUP_SOURCE_MAX_CNT; i++)
	{
		s_gpio_dynamic_wakeup_source_map[i].id = GPIO_WAKE_SOURCE_IDLE_ID;
	}

	GPIO_LOGD("%s[-]\r\n", __func__);
}

bk_err_t bk_gpio_register_wakeup_source(gpio_id_t gpio_id,
                                                 gpio_int_type_t int_type)
{
	uint32_t i = 0;

	GPIO_RETURN_ON_INVALID_ID(gpio_id);
	GPIO_RETURN_ON_INVALID_INT_TYPE_MODE(int_type);

	//search the same id and replace it.
	for(i = 0; i < CONFIG_GPIO_DYNAMIC_WAKEUP_SOURCE_MAX_CNT; i++)
	{
		if(s_gpio_dynamic_wakeup_source_map[i].id == gpio_id)
		{
			s_gpio_dynamic_wakeup_source_map[i].int_type = int_type;
			//NOTES:If doesn't set int type, exit lowpower, if the ISR has reported by rising/falling type
			//Then restore to level type, entry GPIO ISR caused the int status lost.
			gpio_hal_set_int_type(&s_gpio.hal, gpio_id, int_type);
			//s_gpio_dynamic_wakeup_source_map[i].isr = isr;

			GPIO_LOGD("gpio=%d,int_type=%d replace previous wake src\r\n", gpio_id, int_type);
			return BK_OK;
		}
	}

	//serach the first idle id
	for(i = 0; i < CONFIG_GPIO_DYNAMIC_WAKEUP_SOURCE_MAX_CNT; i++)
	{
		if(s_gpio_dynamic_wakeup_source_map[i].id == GPIO_WAKE_SOURCE_IDLE_ID)
		{
			s_gpio_dynamic_wakeup_source_map[i].id = gpio_id;
			s_gpio_dynamic_wakeup_source_map[i].int_type = int_type;

			//NOTES:If doesn't set int type, exit lowpower, if the ISR has reported by rising/falling type
			//Then restore to level type, entry GPIO ISR caused the int status lost.
			gpio_hal_set_int_type(&s_gpio.hal, gpio_id, int_type);
			//s_gpio_dynamic_wakeup_source_map[i].isr = isr;
			s_gpio_is_setted_wake_status |= ((uint64_t)1 << s_gpio_dynamic_wakeup_source_map[i].id);

			GPIO_LOGD("gpio=%d,int_type=%d register wake src\r\n", gpio_id, int_type);

			return BK_OK;
		}
	}

	GPIO_LOGE("too much(%d) GPIO is setted wake src\r\n", CONFIG_GPIO_DYNAMIC_WAKEUP_SOURCE_MAX_CNT);
	for(i = 0; i < CONFIG_GPIO_DYNAMIC_WAKEUP_SOURCE_MAX_CNT; i++)
	{
		GPIO_LOGE("gpio id:%d is using \r\n", s_gpio_dynamic_wakeup_source_map[i].id);
	}
	return BK_FAIL;
}

bk_err_t bk_gpio_unregister_wakeup_source(gpio_id_t gpio_id)
{
	uint32_t i = 0;

	GPIO_RETURN_ON_INVALID_ID(gpio_id);

	/* search the same id and replace it.*/
	for(i = 0; i < CONFIG_GPIO_DYNAMIC_WAKEUP_SOURCE_MAX_CNT; i++)
	{
		if(s_gpio_dynamic_wakeup_source_map[i].id == gpio_id)
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
#endif

#if defined(CONFIG_GPIO_KPSTAT_SUPPORT)
static void gpio_lowpower_keep_status_init(void)
{
	uint32_t i = 0;

	//has configured in default map with static mode
#if defined(CONFIG_GPIO_DEFAULT_SET_SUPPORT)
	const gpio_default_map_t default_map[] = GPIO_DEFAULT_DEV_CONFIG;

	for(i = 0; i < sizeof(default_map)/sizeof(gpio_default_map_t); i++)
	{
		//uses equal to avoid some guy maybe write other value
		if((default_map[i].low_power_io_ctrl == GPIO_LOW_POWER_KEEP_INPUT_STATUS) ||
			(default_map[i].low_power_io_ctrl == GPIO_LOW_POWER_KEEP_OUTPUT_STATUS))
		{
			s_gpio_is_lowpower_keep_status |= ((uint64_t)0x1 << default_map[i].gpio_id);
		}
	}
#endif

#if defined(CONFIG_GPIO_DYNAMIC_KPSTAT_SUPPORT)
	GPIO_LOGD("%s[+]gpio wakecnt=%d\r\n", __func__, CONFIG_GPIO_DYNAMIC_KEEP_STATUS_MAX_CNT);
	for(i = 0; i < CONFIG_GPIO_DYNAMIC_KEEP_STATUS_MAX_CNT; i++)
	{
		s_gpio_lowpower_keep_config[i].gpio_id = GPIO_LOWPOWER_KEEP_STATUS_IDLE_ID;
	}
#endif

	GPIO_LOGD("%s[-]\r\n", __func__);
}

#if defined(CONFIG_GPIO_DYNAMIC_KPSTAT_SUPPORT)
static void gpio_lowpower_keep_io_mode(gpio_id_t gpio_id, gpio_io_mode_t io_mode,
														gpio_pull_mode_t pull_mode)
{
	switch(io_mode)
	{
		case GPIO_OUTPUT_ENABLE:
		{
			if(pull_mode == GPIO_PULL_UP_EN) {
				bk_gpio_disable_input(gpio_id);
				bk_gpio_enable_output(gpio_id);
				bk_gpio_enable_pull(gpio_id);
				bk_gpio_pull_up(gpio_id);
			} else if(pull_mode == GPIO_PULL_DOWN_EN){
				bk_gpio_disable_input(gpio_id);
				bk_gpio_enable_output(gpio_id);
				bk_gpio_enable_pull(gpio_id);
				bk_gpio_pull_down(gpio_id);
			} else
				GPIO_LOGI("%s PLEASE set right pull_mode\r\n", __func__);

			break;
		}
		case GPIO_INPUT_ENABLE:
		{
			if(pull_mode == GPIO_PULL_UP_EN) {
				bk_gpio_disable_output(gpio_id);
				bk_gpio_enable_input(gpio_id);
				bk_gpio_enable_pull(gpio_id);
				bk_gpio_pull_up(gpio_id);
			} else if(pull_mode == GPIO_PULL_DOWN_EN){
				bk_gpio_disable_output(gpio_id);
				bk_gpio_enable_input(gpio_id);
				bk_gpio_enable_pull(gpio_id);
				bk_gpio_pull_down(gpio_id);
			} else
				GPIO_LOGI("%s PLEASE set right pull_mode\r\n", __func__);

			break;
		}

		default:
		{
			bk_gpio_disable_input(gpio_id);
			bk_gpio_disable_output(gpio_id);
			bk_gpio_disable_pull(gpio_id);
			GPIO_LOGI("%s PLEASE set right io_mode\r\n", __func__);
			break;
		}
	}
}

static void gpio_lowpower_dynamic_keep_status_config(void)
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
			gpio_hal_set_config(&s_gpio.hal, gpio_id, &config);
			gpio_lowpower_keep_io_mode(gpio_id, config.io_mode, config.pull_mode);
		}
	}
}

bk_err_t bk_gpio_register_lowpower_keep_status(gpio_id_t gpio_id,
                                                 const gpio_config_t *config)
{
	GPIO_RETURN_ON_INVALID_ID(gpio_id);
	GPIO_RETURN_ON_INVALID_IO_MODE(config->io_mode);
	GPIO_RETURN_ON_INVALID_PULL_MODE(config->pull_mode);

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

			GPIO_LOGD("gpio=%d io_mode=%d pull_mode=%d func_mode=%d\r\n",
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
	GPIO_RETURN_ON_INVALID_ID(gpio_id);

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
			gpio_hal_set_config(&s_gpio.hal, gpio_id, &config);
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
#endif // CONFIG_GPIO_DYNAMIC_KPSTAT_SUPPORT
#endif // CONFIG_GPIO_KPSTAT_SUPPORT

__IRAM_SEC bk_err_t gpio_enter_low_power(void *param)
{
	uint32_t int_cfg[2] = {0, 0};
	uint64_t skip_io = 0;

	GPIO_LOGD("%s[+]\r\n", __func__);

	gpio_dump_regs(true, true);

	gpio_hal_bak_configs(&s_gpio_bak_regs[0], GPIO_NUM_MAX);
	gpio_dump_baked_regs(true, false, false);
	gpio_hal_bak_int_type_configs(&s_gpio_bak_int_type_regs[0], sizeof(s_gpio_bak_int_type_regs)/sizeof(s_gpio_bak_int_type_regs[0]));
	gpio_hal_bak_int_enable_configs(&s_gpio_bak_int_enable_regs[0], sizeof(s_gpio_bak_int_enable_regs)/sizeof(s_gpio_bak_int_enable_regs[0]));

	gpio_dump_baked_regs(false, true, true);

	//NOTES:force disable all int to avoid config gpio caused error isr
	gpio_hal_restore_int_enable_configs(int_cfg, sizeof(int_cfg)/sizeof(int_cfg[0]));

#if 1	//TODO:
	if((uint32_t)param != 0x534b4950)	//just debug:magic value == "SKIP"
	{
		//NOTES:not called in "gpio_config_wakeup_function" as it needs 125us to stable if config GPIO
		gpio_config_low_power_wakeup_pin();
		GPIO_LOGD("exit switch to low power tatus\r\n");
		gpio_dump_regs(true, false);
	}
#endif

#if defined(CONFIG_GPIO_KPSTAT_SUPPORT)
	skip_io |= s_gpio_is_lowpower_keep_status;
#if defined(CONFIG_GPIO_DYNAMIC_KPSTAT_SUPPORT)
	gpio_lowpower_dynamic_keep_status_config();
#endif
#endif

	gpio_config_wakeup_function();

	skip_io |= s_gpio_is_setted_wake_status;

	gpio_hal_switch_to_low_power_status(skip_io);
	gpio_dump_regs(false, true);

	GPIO_LOGD("%s[-]\r\n", __func__);

	return BK_OK;
}

__IRAM_SEC bk_err_t gpio_exit_low_power(void *param)
{
	gpio_hal_t *hal = &s_gpio.hal;
	gpio_interrupt_status_t gpio_status;
	uint32_t int_cfg[2] = {0, 0};

	GPIO_LOGD("%s[+]\r\n", __func__);

	gpio_dump_regs(true, true);
	gpio_dump_baked_regs(true, true, true);

	gpio_hal_get_interrupt_status(hal, &gpio_status);

#if 1	//TODO:
	if((uint32_t)param != 0x534b4950)	//just debug:magic value == "SKIP"
	{
		gpio_hal_restore_int_enable_configs(int_cfg, sizeof(int_cfg)/sizeof(int_cfg[0]));
		gpio_hal_restore_configs(&s_gpio_bak_regs[0], GPIO_NUM_MAX);
		gpio_hal_restore_int_type_configs(&s_gpio_bak_int_type_regs[0], sizeof(s_gpio_bak_int_type_regs)/sizeof(s_gpio_bak_int_type_regs[0]));
		gpio_hal_restore_int_enable_configs(&s_gpio_bak_int_enable_regs[0], sizeof(s_gpio_bak_int_enable_regs)/sizeof(s_gpio_bak_int_enable_regs[0]));
	}
#endif

	gpio_dump_regs(true, true);
	gpio_dump_baked_regs(true, true, true);

	GPIO_LOGD("%s[-]\r\n", __func__);

	return BK_OK;
}

#else
bk_err_t bk_gpio_reg_save(uint32_t*  gpio_cfg)
{
    return gpio_hal_reg_save(gpio_cfg);
}

bk_err_t bk_gpio_reg_restore(uint32_t*  gpio_cfg)
{
    return gpio_hal_reg_restore(gpio_cfg);
}

bk_err_t bk_gpio_wakeup_enable(int64_t index, uint64_t type_l, uint64_t type_h)
{
    return gpio_hal_wakeup_enable( index, type_l, type_h);
}

bk_err_t bk_gpio_wakeup_interrupt_clear()
{
    return gpio_hal_wakeup_interrupt_clear();
}
#endif

#if defined(CONFIG_GPIO_SIMULATE_UART_WRITE)
/**
 * @brief	  Uses specifies GPIO to simulate UART write data
 *
 * This API Uses specifies GPIO to simulate UART write data:
 *	 - Uses CPU poll wait to do delay, so it blocks CPU.
 *	 - The caller should confirm the specifies GPIO is not used by other APP.
 *
 * @param *buff  Which buffers will be write with GPIO.
 * @param len    How many bytes data will be wrote.
 * @param gpio_id  Which GPIO will be simulated as UART write data.
 * @param div    Baud rate == 1Mbps/(1+div)
 *
 * @attention 1. As this function just simulate uart write, it blocks the CPU,
 *               so please don't write too much data.
 *
 * @return
 */
void gpio_simulate_uart_write(unsigned char *buff, uint32_t len, gpio_id_t gpio_id, uint32_t div)
{
	volatile unsigned char c, n;
	UINT32 param;
	uint32_t div_cnt = div+1;

	BK_LOG_ON_ERR(bk_gpio_disable_input(gpio_id));
	BK_LOG_ON_ERR(bk_gpio_enable_output(gpio_id));

	bk_gpio_set_output_high(gpio_id);
	delay_us(div_cnt);

	while (len--) {
		//in while loop, to avoid disable IRQ too much time, release it if finish one byte.
		GLOBAL_INT_DECLARATION();
		GLOBAL_INT_DISABLE();

		//UART start bit
		bk_gpio_set_output_low(gpio_id);
		delay_us(div_cnt);

		//char value
		c = *buff++;
		n = 8;
		while (n--) {
			param = c & 0x01;
			if (param) {
				bk_gpio_set_output_high(gpio_id);
			} else {
				bk_gpio_set_output_low(gpio_id);
			}

			delay_us(div_cnt);
			c >>= 1;
		}

		//UART stop bit
		bk_gpio_set_output_high(gpio_id);
		delay_us(div_cnt);

		GLOBAL_INT_RESTORE();
	}
}
#endif

#if defined(CONFIG_GPIO_ANA_WAKEUP_SUPPORT)
#define GPIO_ANA_WKUP16 16
static int gpio_ana_enter_cb(uint64_t sleep_time, void *args)
{
	uint32_t gpio_id;
	gpio_int_type_t int_type = GPIO_INT_TYPE_MAX;

	for (uint32_t i = 0; i < MIN(s_wkup_cnt, GPIO_ANA_WAKEUP_MAX); i++) {
		gpio_id = s_wkup_cfg[i].id;
		int_type = (uint32_t)s_wkup_cfg[i].int_type;
		sys_drv_gpio_ana_wakeup_enable(i, gpio_id, int_type);
	}

	// special treatment for gpio 16
	gpio_dev_unmap(GPIO_ANA_WKUP16);
	if (int_type == GPIO_INT_TYPE_LOW_LEVEL) {
		gpio_config_t cfg = {GPIO_IO_DISABLE, GPIO_PULL_DISABLE, GPIO_SECOND_FUNC_DISABLE};
		bk_gpio_set_config(GPIO_ANA_WKUP16, &cfg);
	} else if (int_type == GPIO_INT_TYPE_HIGH_LEVEL) {
		gpio_config_t cfg = {GPIO_INPUT_ENABLE, GPIO_PULL_UP_EN, GPIO_SECOND_FUNC_DISABLE};
		bk_gpio_set_config(GPIO_ANA_WKUP16, &cfg);
	}

	return 0;
}

bk_err_t bk_gpio_ana_register_wakeup_source(gpio_id_t gpio_id, gpio_int_type_t int_type)
{
	uint32_t next_idx;
	pm_cb_conf_t enter_conf;

	GPIO_RETURN_ON_INVALID_ID(gpio_id);
	GPIO_RETURN_ON_INVALID_INT_TYPE_MODE(int_type);

	if (gpio_id > GPIO_15 || int_type > GPIO_INT_TYPE_HIGH_LEVEL) {
		GPIO_LOGE("gpio ana wakeup source not support id: %d type: %d\r\n", gpio_id, int_type);
		return BK_ERR_ANA_GPIO_TYPE_NOT_SUPPORT;
	}

	next_idx = (s_wkup_cnt++) % GPIO_ANA_WAKEUP_MAX;
	s_wkup_cfg[next_idx].id = gpio_id;
	s_wkup_cfg[next_idx].int_type = int_type;
	GPIO_LOGI("regist wakeup source gpio id: %d type: %d\r\n", gpio_id, int_type);

	enter_conf.cb = gpio_ana_enter_cb;
	enter_conf.args = NULL;

	return bk_pm_sleep_register_cb(PM_MODE_SUPER_DEEP_SLEEP, PM_DEV_ID_GPIO, &enter_conf, NULL);
}
#endif

#if defined(CONFIG_GPIO_DUMP_MAP_DEV_DEBUG)
bk_err_t gpio_dump_map_dev_cfg(void)
{
	return gpio_hal_dump_map_dev_cfg(&s_gpio.hal);
}
#endif
