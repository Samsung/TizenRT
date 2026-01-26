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
#include <os/os.h>
#include <os/mem.h>
#include <driver/io_matrix.h>
#include <driver/io_matrix_types.h>
#include <driver/gpio.h>
#include <driver/int.h>
#include "io_matrix_driver.h"
#include "iomx_hal.h"

#if (defined(CONFIG_SYSTEM_CTRL))
#include "sys_driver.h"
#endif

static bool s_iomx_is_init = false;
static struct {
	gpio_isr_t legacy_handler;
	gpio_isr_priv_t priv_handler;
	void *priv;
}s_iomx_gpio_isr[SOC_GPIO_NUM] = {NULL};

static uint32_t s_iomx_gpio_baked_regs[SOC_GPIO_NUM] = {0};

#define IOMX_RETURN_ON_INVALID_ID(id) do {\
			if ((id) >= SOC_GPIO_NUM) {\
				return BK_ERR_GPIO_INVALID_ID;\
			}\
		} while(0)

#define IOMX_RETURN_ON_INVALID_INT_TYPE_MODE(mode) do {\
			if ((mode) >= GPIO_INT_TYPE_MAX) {\
				return BK_ERR_GPIO_INVALID_INT_TYPE;\
			}\
		} while(0)

#define IOMX_RETURN_ON_INVALID_IO_MODE(mode) do {\
			if (((mode)) >= GPIO_IO_INVALID) {\
				return BK_ERR_GPIO_INVALID_MODE;\
			}\
		} while(0)

#define IOMX_RETURN_ON_INVALID_PULL_MODE(mode) do {\
			if (((mode)) >= GPIO_PULL_INVALID) {\
				return BK_ERR_GPIO_INVALID_MODE;\
			}\
		} while(0)

#define IOMX_RETURN_ON_INVALID_FUNC_CODE(code) do {\
			if ((code) >= FUNC_CODE_INVALID) {\
				return BK_ERR_IO_MATRIX_INVALID_FUNC_CODE;\
			}\
		} while(0)

static void iomx_gpio_isr(int irq, void *context, void *arg);
bk_err_t bk_iomx_driver_init(void)
{
	if (s_iomx_is_init)
		return BK_OK;

	iomx_hal_init();

#if (CONFIG_CPU_CNT > 1) && defined(CONFIG_MAILBOX)
	amp_res_init(AMP_RES_ID_GPIO);
#endif

	//Move ISR to last to avoid other resouce doesn't finish but isr has came.
	//F.E:GPIO wakeup deepsleep.
#if defined(CONFIG_TZ) && ( CONFIG_SPE == 0)
	bk_int_isr_register(INT_SRC_GPIO_NS, iomx_gpio_isr, NULL);
#else
	bk_int_isr_register(INT_SRC_GPIO, iomx_gpio_isr, NULL);
#endif

	//interrupt to CPU enable
#if (defined(CONFIG_SYSTEM_CTRL))
	#if defined(CONFIG_TZ) && ( CONFIG_SPE == 0)
		sys_drv_int_group2_enable(GPIO_NS_INTERRUPT_CTRL_BIT);  // Enable GPIO_NS interrupt for TF-M NS mode
	#else
		sys_drv_int_group2_enable(GPIO_INTERRUPT_CTRL_BIT);
	#endif
#else
	icu_enable_gpio_interrupt();
#endif

	s_iomx_is_init = true;

	return BK_OK;
}

bk_err_t bk_iomx_driver_deinit(void)
{
	if (!s_iomx_is_init)
		return BK_OK;

	//interrupt to CPU disable
#if (defined(CONFIG_SYSTEM_CTRL))
	#if defined(CONFIG_TZ) && ( CONFIG_SPE == 0)
		sys_drv_int_group2_disable(GPIO_NS_INTERRUPT_CTRL_BIT);  // Disable GPIO_NS interrupt for TF-M NS mode
	#else
		sys_drv_int_group2_disable(GPIO_INTERRUPT_CTRL_BIT);
	#endif
#else
	icu_disable_gpio_interrupt();
#endif

	iomx_hal_deinit();

	s_iomx_is_init = false;

	return BK_OK;
}

bk_err_t bk_iomx_gpio_isr_attach(void)
{
#if defined(CONFIG_TZ) && ( CONFIG_SPE == 0)
	bk_int_isr_register(INT_SRC_GPIO_NS, iomx_gpio_isr, NULL);
#else
	bk_int_isr_register(INT_SRC_GPIO, iomx_gpio_isr, NULL);
#endif
	return BK_OK;
}

uint32_t bk_iomx_get_gpio_func_code(uint32_t gpio_id)
{
	IOMX_RETURN_ON_INVALID_ID(gpio_id);

	return iomx_hal_get_func_code(gpio_id);
}

bk_err_t bk_iomx_set_gpio_func(uint32_t gpio_id, IOMX_CODE_T func_code)
{
	IOMX_RETURN_ON_INVALID_ID(gpio_id);
	IOMX_RETURN_ON_INVALID_FUNC_CODE(func_code);

	return iomx_hal_set_func_code(gpio_id, func_code);
}

void bk_iomx_set_value(gpio_id_t id, uint32_t v)
{
	IOMX_RETURN_ON_INVALID_ID(id);
	iomx_hal_set_value(id, v);
}

uint32_t bk_iomx_get_value(gpio_id_t id)
{
	IOMX_RETURN_ON_INVALID_ID(id);
	return iomx_hal_get_value(id);
}

bk_err_t bk_iomx_enable_pull(gpio_id_t gpio_id)
{
	IOMX_RETURN_ON_INVALID_ID(gpio_id);

	return iomx_hal_pull_enable(gpio_id, 1);
}

bk_err_t bk_iomx_disable_pull(gpio_id_t gpio_id)
{
	IOMX_RETURN_ON_INVALID_ID(gpio_id);

	return iomx_hal_pull_enable(gpio_id, 0);
}

bk_err_t bk_iomx_pull_up(gpio_id_t gpio_id)
{
	IOMX_RETURN_ON_INVALID_ID(gpio_id);

	iomx_hal_pull_enable(gpio_id, 1);
	return iomx_hal_pull_up_enable(gpio_id, 1);
}

bk_err_t bk_iomx_pull_down(gpio_id_t gpio_id)
{
	IOMX_RETURN_ON_INVALID_ID(gpio_id);

	iomx_hal_pull_enable(gpio_id, 1);
	return iomx_hal_pull_up_enable(gpio_id, 0);
}

bk_err_t bk_iomx_output_high(gpio_id_t gpio_id)
{
	IOMX_RETURN_ON_INVALID_ID(gpio_id);

	return iomx_hal_set_output_value(gpio_id, 1);
}

bk_err_t bk_iomx_output_low(gpio_id_t gpio_id)
{
	IOMX_RETURN_ON_INVALID_ID(gpio_id);

	return iomx_hal_set_output_value(gpio_id, 0);
}

bool bk_iomx_get_input(gpio_id_t gpio_id)
{
	IOMX_RETURN_ON_INVALID_ID(gpio_id);

	return iomx_hal_get_input(gpio_id);
}

bool bk_iomx_get_output(gpio_id_t gpio_id)
{
	IOMX_RETURN_ON_INVALID_ID(gpio_id);

	return iomx_hal_get_output(gpio_id);
}

//MAX capactiy:3
bk_err_t bk_iomx_set_capacity(gpio_id_t gpio_id, uint32 capacity)
{
	IOMX_RETURN_ON_INVALID_ID(gpio_id);

	return iomx_hal_set_capacity(gpio_id, capacity);
}


/* Enable GPIO  interrupt.
*/
bk_err_t bk_iomx_register_isr(gpio_id_t gpio_id, gpio_isr_t isr)
{
	IOMX_RETURN_ON_INVALID_ID(gpio_id);

	s_iomx_gpio_isr[gpio_id].legacy_handler = isr;
	s_iomx_gpio_isr[gpio_id].priv_handler = NULL;
	s_iomx_gpio_isr[gpio_id].priv = NULL;

	return BK_OK;
}

/* Enable GPIO  interrupt.
*/
bk_err_t bk_iomx_register_isr_ex(gpio_id_t gpio_id, gpio_isr_priv_t isr, void *priv)
{
	IOMX_RETURN_ON_INVALID_ID(gpio_id);

	s_iomx_gpio_isr[gpio_id].legacy_handler = NULL;
	s_iomx_gpio_isr[gpio_id].priv_handler = isr;
	s_iomx_gpio_isr[gpio_id].priv = priv;

	return BK_OK;
}

//This function just enable the select GPIO can report IRQ to CPU
bk_err_t bk_iomx_enable_interrupt(gpio_id_t gpio_id)
{
	IOMX_RETURN_ON_INVALID_ID(gpio_id);

	//Before enable the interrupt,wait for the internal stability of the chip
	for (volatile int i = 0; i < 1000; i++);

	return iomx_hal_enable_interrupt(gpio_id);
}

bk_err_t bk_iomx_disable_interrupt(gpio_id_t gpio_id)
{
	IOMX_RETURN_ON_INVALID_ID(gpio_id);

	//WARNING:We can't call icu_enable_gpio_interrupt/sys_drv_int_group2_disable in this function
	//If more then one GPIO_ID enable interrupt, here disable the IRQ to CPU, it caused other GPIO ID can't work
	iomx_hal_disable_interrupt(gpio_id);

	return BK_OK;
}

bk_err_t bk_iomx_clear_interrupt(gpio_id_t gpio_id)
{
	IOMX_RETURN_ON_INVALID_ID(gpio_id);

	iomx_hal_clear_chan_interrupt_status(gpio_id);

	return BK_OK;
}

bk_err_t bk_iomx_set_interrupt_type(gpio_id_t gpio_id, gpio_int_type_t type)
{
	IOMX_RETURN_ON_INVALID_ID(gpio_id);
	IOMX_RETURN_ON_INVALID_INT_TYPE_MODE(type);

	return iomx_hal_set_int_type(gpio_id, type);
}

static void iomx_gpio_isr(int irq, void *context, void *arg)
{
	iomx_gpio_interrupt_status_t gpio_status;
	int gpio_id;

	iomx_hal_get_interrupt_status(&gpio_status);

	for (gpio_id = 0; gpio_id < SOC_GPIO_NUM; gpio_id++) {
		if (iomx_hal_is_interrupt_triggered(gpio_id, &gpio_status)) {
			if (s_iomx_gpio_isr[gpio_id].priv_handler) {
				IOMX_LOGD("gpio int: index:%d \r\n",gpio_id);
				s_iomx_gpio_isr[gpio_id].priv_handler(gpio_id, s_iomx_gpio_isr[gpio_id].priv);
			} else if (s_iomx_gpio_isr[gpio_id].legacy_handler) {
				IOMX_LOGD("gpio int: index:%d \r\n",gpio_id);
				s_iomx_gpio_isr[gpio_id].legacy_handler(gpio_id);
			}
			bk_iomx_clear_interrupt(gpio_id);
		}
	}
}

bk_err_t iomx_enable_multi_interrupts(uint64_t gpio_idx)
{
	return iomx_hal_enable_multi_interrupts(gpio_idx);
}

bk_err_t iomx_disable_all_interrupts(void)
{
	return iomx_hal_disable_all_interrupts();
}

bk_err_t iomx_get_interrupt_overview(void)
{
	return iomx_hal_get_interrupt_overview();
}

bk_err_t iomx_exec_iomx_gpio_isr(gpio_id_t gpio_id)
{
	if (s_iomx_gpio_isr[gpio_id].priv_handler) {
		IOMX_LOGD("gpio int: index:%d \r\n",gpio_id);
		s_iomx_gpio_isr[gpio_id].priv_handler(gpio_id, s_iomx_gpio_isr[gpio_id].priv);
		return BK_OK;
	} else if (s_iomx_gpio_isr[gpio_id].legacy_handler) {
		IOMX_LOGD("gpio int: index:%d \r\n",gpio_id);
		s_iomx_gpio_isr[gpio_id].legacy_handler(gpio_id);
		return BK_OK;
	}
	
	return BK_FAIL;
}

bk_err_t iomx_backup_gpio_configs(void)
{
	return iomx_hal_bakup_configs(s_iomx_gpio_baked_regs);
}

bk_err_t iomx_restore_gpio_configs(void)
{
	return iomx_hal_restore_configs(s_iomx_gpio_baked_regs);
}

void iomx_dump_baked_regs(void)
{
#if defined(CONFIG_GPIO_WAKEUP_DEBUG)
	gpio_id_t gpio_id = 0;

	GPIO_LOGD("%s[+]\r\n", __func__);

	// GPIO_LOGD("is_setted_wake_status h= 0x%x, l=0x%x\r\n", (uint32_t)(s_gpio_is_setted_wake_status>>32), (uint32_t)s_gpio_is_setted_wake_status);

	for(gpio_id = 0; gpio_id < SOC_GPIO_NUM; gpio_id++)
	{
		GPIO_LOGD("s_gpio_bak_regs[%d]=0x%x\r\n", gpio_id, s_iomx_gpio_baked_regs[gpio_id]);
	}

	GPIO_LOGD("%s[-]\r\n", __func__);
#endif
}

void iomx_dump_regs(bool config, bool overview, bool atpg)
{
#if defined(CONFIG_GPIO_WAKEUP_DEBUG)
	gpio_id_t gpio_id;

	GPIO_LOGD("%s[+]\r\n", __func__);

	if (config)
	{
		for(gpio_id = GPIO_0; gpio_id < SOC_GPIO_NUM; gpio_id++)
		{
			///gpio_struct_dump(gpio_id);
			GPIO_LOGD("gpio[%d]=0x%x\r\n", gpio_id, *(volatile uint32_t*)(GPIO_LL_REG_BASE + 4*gpio_id));
		}
	}

	if (overview)
	{
		GPIO_LOGD("REG0x%x=0x%x\r\n", IOMX_GPIO_INTSTA_ADDR, REG_READ(IOMX_GPIO_INTSTA_ADDR));
		GPIO_LOGD("REG0x%x=0x%x\r\n", IOMX_GPIO_INPUT_ADDR,  REG_READ(IOMX_GPIO_INPUT_ADDR));
		GPIO_LOGD("REG0x%x=0x%x\r\n", IOMX_GPIO_OUTPUT_ADDR, REG_READ(IOMX_GPIO_OUTPUT_ADDR));
	}

	if (atpg)
	{
		GPIO_LOGD("REG0x%x=0x%x\r\n", IOMX_GPIO_FUNC_O_ATPG_ADDR,   REG_READ(IOMX_GPIO_FUNC_O_ATPG_ADDR));
		GPIO_LOGD("REG0x%x=0x%x\r\n", IOMX_GPIO_FUNC_IE_ATPG_ADDR,  REG_READ(IOMX_GPIO_FUNC_IE_ATPG_ADDR));
		GPIO_LOGD("REG0x%x=0x%x\r\n", IOMX_GPIO_FUNC_OEN_ATPG_ADDR, REG_READ(IOMX_GPIO_FUNC_OEN_ATPG_ADDR));
	}

	GPIO_LOGD("%s[-]\r\n", __func__);
#endif
}

// eof

