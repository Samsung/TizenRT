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
#include <driver/io_matrix.h>
#include "gpio_driver.h"
#include "gpio_adapter.h"


#define GPIO_REG_DEFAULT_VALUE      0x0
#define GPIO_RETURN_ON_INVALID_PERIAL_MODE(mode, mode_max) do {\
				if ((mode) >= (mode_max)) {\
					return BK_ERR_GPIO_SET_INVALID_FUNC_MODE;\
				}\
			} while(0)


bk_err_t gpio_dev_map(gpio_id_t gpio_id, gpio_dev_t dev)
{
	// Temp use for peri dev, remove later!
	gpio_dev_unprotect_map(gpio_id, dev);

	return BK_OK;
}

bk_err_t gpio_dev_unmap(gpio_id_t gpio_id)
{
	// Temp use for peri dev, remove later!
	gpio_dev_unprotect_unmap(gpio_id);

	return BK_OK;
}

/* Here doesn't check the GPIO id is whether used by another CPU-CORE, but checked current CPU-CORE */
bk_err_t gpio_dev_unprotect_map(gpio_id_t gpio_id, gpio_dev_t dev)
{
	//GPIO_LOGI("%s:id=%d, dev=%d\r\n", __func__, gpio_id, dev);

	IOMX_CODE_T code = contert_gpio_dev_to_iomx_code(dev);

	bk_iomx_set_value(gpio_id, GPIO_REG_DEFAULT_VALUE);

	return bk_iomx_set_gpio_func(gpio_id, code);
}

/* Here doesn't check the GPIO id is whether used by another CPU-CORE */
bk_err_t gpio_dev_unprotect_unmap(gpio_id_t gpio_id)
{
	bk_iomx_set_value(gpio_id, GPIO_REG_DEFAULT_VALUE);

	return BK_OK;
}

bk_err_t gpio_i2c1_sel(gpio_i2c1_map_mode_t mode)
{
	return BK_OK;
}

bk_err_t gpio_i2s_sel(gpio_i2s_map_mode_t mode)
{
	return BK_OK;
}

bk_err_t gpio_spi_sel(gpio_spi1_map_mode_t mode)
{
	return BK_OK;
}

bk_err_t gpio_sdio_sel(gpio_sdio_map_mode_t mode)
{
	return BK_OK;
}

bk_err_t gpio_sdio_one_line_sel(gpio_sdio_map_mode_t mode)
{
	return BK_OK;
}


bk_err_t gpio_jtag_sel(gpio_jtag_map_group_t group_id)
{
	bk_err_t ret = BK_OK;
	gpio_dev_unprotect_unmap(GPIO_20);
	gpio_dev_unprotect_unmap(GPIO_21);

	#if CONFIG_SPE
	gpio_dev_unprotect_unmap(GPIO_0);
	gpio_dev_unprotect_unmap(GPIO_1);
	#endif

	if (group_id == GPIO_JTAG_MAP_GROUP0) {
		ret = gpio_dev_unprotect_map(GPIO_20, GPIO_DEV_JTAG_TCK);
		ret = gpio_dev_unprotect_map(GPIO_21, GPIO_DEV_JTAG_TMS);
	} else if (group_id == GPIO_JTAG_MAP_GROUP1) {
		ret = gpio_dev_unprotect_map(GPIO_0, GPIO_DEV_JTAG_TCK);
		ret = gpio_dev_unprotect_map(GPIO_1, GPIO_DEV_JTAG_TMS);
	} else {
		// IOMX_LOGI("Unsupported group id(%d).\r\n", group_id);
		return BK_FAIL;
	}

	return ret;
}

bk_err_t gpio_scr_sel(gpio_scr_map_group_t mode)
{
	return BK_OK;
}
