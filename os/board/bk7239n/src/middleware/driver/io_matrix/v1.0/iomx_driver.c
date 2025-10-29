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
#include "iomx_driver.h"
#include "iomx_hal.h"
#include "gpio_hal.h"

#define IOMX_RETURN_ON_GPIO_NOT_SUPPORT(gpio_id) do {\
		if ((gpio_id) > SOC_GPIO_MAX_ID) {\
			return BK_ERR_IO_MATRIX_INVALID_GPIO;\
		}\
	} while(0)

#define IOMX_RETURN_INVALID_ON_GPIO_NOT_SUPPORT(gpio_id) do {\
		if ((gpio_id) > SOC_GPIO_MAX_ID) {\
			return FUNC_CODE_INVALID;\
		}\
	} while(0)
        
#if defined(CONFIG_GPIO_WAKEUP_SUPPORT)
static uint32_t s_iomx_bak_regs[SOC_GPIO_TOTAL_CNT/4];
#endif

bk_err_t bk_iomx_driver_init(void)
{
        iomx_hal_init();

	return BK_OK;
}

uint32_t bk_iomx_get_gpio_func_code(uint32_t gpio_id)
{
        IOMX_RETURN_INVALID_ON_GPIO_NOT_SUPPORT(gpio_id);

        uint32_t val;
        uint32_t offset = gpio_id % IOMUX_CFG_GPIO_CNT;
        uint32_t reg_id = gpio_id / IOMUX_CFG_GPIO_CNT;

        val = iomx_hal_get_cfg_value(reg_id);

	return ((val >> (offset * IOMUX_CFG_GPIO_FIELD_BITS)) & IOMUX_FUNC_CODE_MASK);
}

bk_err_t bk_iomx_set_gpio_func(uint32_t gpio_id, IOMX_CODE_T func_code)
{
        IOMX_RETURN_ON_GPIO_NOT_SUPPORT(gpio_id);

        uint32_t val;
        uint32_t offset = gpio_id % IOMUX_CFG_GPIO_CNT;
        uint32_t reg_id = gpio_id / IOMUX_CFG_GPIO_CNT;

        val = iomx_hal_get_cfg_value(reg_id);
        val &= ~(IOMUX_FUNC_CODE_MASK << (offset * IOMUX_CFG_GPIO_FIELD_BITS));
        val |= ((func_code) & IOMUX_FUNC_CODE_MASK) << (offset * IOMUX_CFG_GPIO_FIELD_BITS);

        iomx_hal_set_cfg_value(reg_id, val);

	return BK_OK;
}

bk_err_t bk_iomx_driver_deinit(void)
{
        iomx_hal_deinit();
	return BK_OK;
}

#if defined(CONFIG_GPIO_WAKEUP_SUPPORT)
bk_err_t iomx_enter_low_power(void)
{
	iomx_hal_bak_configs(&s_iomx_bak_regs[0], sizeof(s_iomx_bak_regs)/sizeof(s_iomx_bak_regs[0]));
	return BK_OK;
}

bk_err_t iomx_exit_low_power(void)
{
	gpio_hal_switch_to_low_power_status(0);
	iomx_hal_restore_configs(&s_iomx_bak_regs[0], sizeof(s_iomx_bak_regs)/sizeof(s_iomx_bak_regs[0]));
	return BK_OK;
}
#endif
// eof

