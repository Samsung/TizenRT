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
#include <soc/soc.h>

#include "ana_gpio_driver.h"
#include "io_matrix_driver.h"
#include "sys_driver.h"
#include "sys_hal.h"
#include "aon_pmu_hal.h"

gpio_id_t ana_gpio_get_wakeup_pin(void)
{
	gpio_id_t gpio_id = SOC_GPIO_NUM;
	uint32_t gpio_sta = aon_pmu_hal_get_ana_gpio_status();

	for (uint32_t i = 0; i < SOC_GPIO_NUM; i++)
	{
		if (gpio_sta & BIT(i))
		{
			if (gpio_id == SOC_GPIO_NUM) {
				gpio_id = i;
				sys_drv_wakeup_source_gpio_clear();
				break;
			} else {
				// TODO: let user know multiple GPIOs were detected triggering wake-up at the same time
				// BK_ASSERT(0);
			}
		}
	}

	return gpio_id;
}

static void ana_gpio_isr_handler(void)
{
	uint32_t gpio_id = SOC_GPIO_NUM, gpio_idx;
	uint32_t gpio_sta = aon_pmu_hal_get_ana_gpio_status();

	for (uint32_t i = 0; i < SOC_GPIO_NUM; i++)
	{
		if (gpio_sta & BIT(i))
		{
			gpio_id = i;
			ANA_GPIO_LOGD("gpio int: index:%d \r\n", gpio_id);
			iomx_exec_iomx_gpio_isr(gpio_id);
		}
	}

	if (gpio_id == SOC_GPIO_NUM)
	{
		gpio_idx = sys_hal_get_ana_reg11_gpiowk();
		ANA_GPIO_LOGW("unexpected gpio int: idx:%x sta:%x\r\n", gpio_idx, gpio_sta);
	}

	ana_gpio_clear_wakeup_source();
}

bk_err_t ana_gpio_wakeup_init(void)
{
	bk_int_isr_register(INT_SRC_ANA_GPIO, ana_gpio_isr_handler, NULL);
	// bk_int_isr_register(INT_SRC_ABNORMAL_GPIO, ana_gpio_isr_handler, NULL);

	return BK_OK;
}

bk_err_t ana_gpio_wakeup_deinit(void)
{
	bk_int_isr_unregister(INT_SRC_ANA_GPIO);
	// bk_int_isr_unregister(INT_SRC_ABNORMAL_GPIO);

	return BK_OK;
}

bk_err_t ana_gpio_config_wakeup_source(uint64_t gpio_bitmap)
{
	sys_hal_set_ana_reg8_spi_latch1v_iram(1);
	sys_hal_set_ana_reg11_gpiowk((uint32_t)gpio_bitmap);
	sys_hal_set_ana_reg8_spi_latch1v_iram(0);

	return BK_OK;
}

__IRAM_SEC bk_err_t ana_gpio_clear_wakeup_source(void)
{
	sys_hal_set_ana_reg8_spi_latch1v_iram(1);
	sys_hal_set_ana_reg11_gpiowk(0);
	sys_hal_set_ana_reg8_spi_latch1v_iram(0);

	return BK_OK;
}