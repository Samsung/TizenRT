// Copyright 2020-2021 Beken
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
#include <os/mem.h>
#include <driver/efuse.h>
#include "efuse_driver.h"
#include "efuse_hal.h"
#include "sys_driver.h"

typedef struct {
	efuse_hal_t hal;
} efuse_driver_t;

#define EFUSE_ADDR_MAX_VALUE    EFUSE_F_ADDR_V

#define EFUSE_RETURN_ON_DRIVER_NOT_INIT() do {\
		if (!s_efuse_driver_is_init) {\
			return BK_ERR_EFUSE_DRIVER_NOT_INIT;\
		}\
	} while(0)

#define EFUSE_RETURN_ON_ADDR_OUT_OF_RANGE(addr) do {\
		if ((addr) > EFUSE_ADDR_MAX_VALUE) {\
			return BK_ERR_EFUSE_ADDR_OUT_OF_RANGE;\
		}\
	} while(0)

static efuse_driver_t s_efuse = {0};
static bool s_efuse_driver_is_init = false;

static void efuse_deinit_common(void)
{
	efuse_hal_disable(&s_efuse.hal);
	efuse_hal_reset_config_to_default(&s_efuse.hal);
}

bk_err_t bk_efuse_driver_init(void)
{
	if (s_efuse_driver_is_init) {
		return BK_OK;
	}

	os_memset(&s_efuse, 0, sizeof(s_efuse));
	efuse_hal_init(&s_efuse.hal);
	s_efuse_driver_is_init = true;
#if (CONFIG_SYSTEM_CTRL)
	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_EFUSE, CLK_PWR_CTRL_PWR_UP);
#endif

	return BK_OK;
}

bk_err_t bk_efuse_driver_deinit(void)
{
	if (!s_efuse_driver_is_init) {
		return BK_OK;
	}

#if (CONFIG_SYSTEM_CTRL)
	sys_drv_dev_clk_pwr_up(CLK_PWR_ID_EFUSE, CLK_PWR_CTRL_PWR_DOWN);
#endif
	efuse_deinit_common();
	s_efuse_driver_is_init = false;

	return BK_OK;
}

bk_err_t bk_efuse_write_byte(uint8_t addr, uint8_t data)
{
	EFUSE_RETURN_ON_DRIVER_NOT_INIT();
	EFUSE_RETURN_ON_ADDR_OUT_OF_RANGE(addr);

	return efuse_hal_write(&s_efuse.hal, addr, data);
}

bk_err_t bk_efuse_read_byte(uint8_t addr, uint8_t *data)
{
	int ret;
	EFUSE_RETURN_ON_DRIVER_NOT_INIT();
	EFUSE_RETURN_ON_ADDR_OUT_OF_RANGE(addr);

	ret = efuse_hal_read(&s_efuse.hal, addr, data);
	if(ret != BK_OK){
		return BK_ERR_EFUSE_READ_FAIL;
	}
	return BK_OK;
}

