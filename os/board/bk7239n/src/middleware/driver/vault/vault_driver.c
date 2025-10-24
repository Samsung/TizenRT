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
#include <driver/vault_types.h>
// power control
#include <modules/pm.h>
#include "vault_hal.h"
#include "vault_driver.h"

#define MODULE_PWR_ON_MASK 0x3

/*----------------------------------------------------------------------------
 * Local variables
 */
typedef struct {
	vault_hal_t hal;
	uint8_t     power_status;
} vault_driver_t;

static vault_driver_t s_vault = {0};

bk_err_t bk_vault_driver_init(security_module_name_t module_name)
{
	vault_hal_init(&s_vault.hal);

	if(s_vault.power_status > 0x0)
	{
		s_vault.power_status |= 0x1 << module_name;
		return BK_OK;
	}

	s_vault.power_status |= 0x1 << module_name;

	bk_pm_clock_ctrl(PM_CLK_ID_OTP, PM_CLK_CTRL_PWR_UP);
	bk_pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_ENCP,PM_POWER_MODULE_STATE_ON);
	vault_hal_start_common(&s_vault.hal);
	rtos_delay_milliseconds(5);  //Wait for OTP to stabilize

	return BK_OK;
}

bk_err_t bk_vault_driver_deinit(security_module_name_t module_name)
{
	switch (module_name)
		{
			case MODULE_OTP:
				s_vault.power_status &= ~(0x1 << MODULE_OTP);
				break;
				
			case MODULE_SECURITYIP:
				s_vault.power_status &= ~(0x1 << MODULE_SECURITYIP);
				break;

			default:
				return BK_FAIL;
				break;
		}

	if((s_vault.power_status & MODULE_PWR_ON_MASK) == 0x0)
	{
		vault_hal_stop_common(&s_vault.hal);
		bk_pm_clock_ctrl(PM_CLK_ID_OTP, PM_CLK_CTRL_PWR_DOWN);
		bk_pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_ENCP,PM_POWER_MODULE_STATE_OFF);
	}

	return BK_OK;
}




