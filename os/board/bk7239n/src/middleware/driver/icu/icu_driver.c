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

#include <os/os.h>
#include <os/mem.h>
#include "icu_hal.h"
#include "icu_driver.h"
#include <os/os.h>
#include "interrupt_base.h"

icu_driver_t s_icu = {0};

bk_err_t bk_icu_driver_init(void)
{
	os_memset(&s_icu, 0, sizeof(s_icu));
	icu_hal_init(&s_icu.hal);
	icu_enable_global_interrupt();

	return BK_OK;
}

bk_err_t bk_icu_driver_deinit(void)
{
	icu_disable_global_interrupt();
	return BK_OK;
}


/** icu contrl cpu intterrupt api for internal**/

#if (defined(CONFIG_ALIOS))
void bk_icu_deafult_swi(void)
{
	while (1);
}
#endif

bk_err_t icu_enable_global_interrupt(void)
{
	icu_enable_irq();
	icu_enable_fiq();

	return BK_OK;
}

bk_err_t icu_disable_global_interrupt(void)
{
	icu_disable_all_interrupt();
	icu_disable_irq();
	icu_disable_fiq();
	interrupt_deinit();

	return BK_OK;
}




#if (defined(CONFIG_ICU_GPIO))

void icu_set_gpio_perial_mode(gpio_id_t id, uint32_t mode)
{
	icu_hal_set_gpio_perial_mode(&s_icu.hal, id, mode);
}

uint32_t icu_get_gpio_perial_mode(gpio_id_t id)
{
	return icu_hal_get_gpio_perial_mode(&s_icu.hal, id);
}

void icu_uart2_gpio_sel(icu_gpio_uart2_map_mode_t mode)
{

	icu_hal_uart2_gpio_sel(&s_icu.hal, mode);
}

void icu_spi3_gpio_sel(icu_gpio_spi3_map_mode_t mode)
{

	icu_hal_spi3_gpio_sel(&s_icu.hal, mode);
}

void icu_sdio_gpio_sel(icu_gpio_sdio_map_mode_t mode)
{

	icu_hal_sdio_gpio_sel(&s_icu.hal, mode);
}

uint32_t icu_pwms_gpio_sel(uint32 pwm_chan, icu_gpio_pwms_map_mode_t mode)
{
	return icu_hal_pwms_gpio_sel(&s_icu.hal, pwm_chan, mode);
}

void icu_sdio_host_enable(void)
{
	icu_hal_sdio_host_enable(&s_icu.hal);
}

void icu_sdio_dma_enable(void)
{
	icu_hal_sdio_dma_enable(&s_icu.hal);
}
#endif

//global intterrupt APIs
uint32_t icu_get_global_int_status(void)
{
	return icu_hal_get_global_int_status(&s_icu.hal);
}

uint32_t icu_get_irq_int_status(void)
{
	return icu_hal_get_irq_int_status(&s_icu.hal);
}

uint32_t icu_get_fiq_int_status(void)
{
	return icu_hal_get_fiq_int_status(&s_icu.hal);
}

bool icu_is_pwm_has_two_int(void)
{
	return icu_hal_is_pwm_has_two_int(&s_icu.hal);
}

uint32_t icu_get_uart_int_status(void)
{
	return icu_hal_get_uart_int_status(&s_icu.hal);
}

