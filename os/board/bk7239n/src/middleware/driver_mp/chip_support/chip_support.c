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
#include <os/mem.h>
#include <modules/chip_support.h>

#include "legacy/sys_ctrl_pub.h"
#include "sys_driver.h"
#include "bk_misc.h"
#include "aon_pmu_hal.h"


#if defined(CONFIG_CHIP_SUPPORT)
/* If we don't want to check chip ID, the application can extern s_chip_id_check,
 * and set flag CHIP_ID_CHECK_SKIP. Generally speaking, we should NOT use it.
 *
 */
#include "chip_support_map.h"

#define AON_PMU_CHIP_ID_SHIFT (16)

typedef struct {
	uint32_t chip_id;
	uint32_t chip_id_mask;
	uint32_t dev_id;
	uint32_t dev_id_mask;
	uint32_t software_chip_id;
	uint32_t software_chip_id_mask;
} chip_info_t;

static uint32_t bk_get_software_chip_id_from_aon_pmu()
{
	uint32_t aon_pmu_software_chip_id;

	aon_pmu_software_chip_id = AON_PMU_SOFTWARE_CHIP_ID >> AON_PMU_CHIP_ID_SHIFT;

	return aon_pmu_software_chip_id;
}

static int bk_get_chip_info(chip_info_t *chip_info)
{
	chip_info->chip_id = sys_drv_get_chip_id();
	chip_info->dev_id = sys_drv_get_device_id();
	chip_info->software_chip_id = bk_get_software_chip_id_from_aon_pmu();
	return BK_OK;
}

bool bk_is_chip_supported(void)
{
	chip_info_t supported_chips[] = SUPPORTED_CHIPS;
	chip_info_t chip_info;
	uint32_t chip_id_mask;
	uint32_t dev_id_mask;
	uint32_t software_chip_id_mask;

	if (bk_get_chip_info(&chip_info) != BK_OK) {
		CHIP_SUPPORT_LOGE("Failed to read device id info, abort!!\n");
		return false;
	}

	CHIP_SUPPORT_LOGD("get chip id=%x device id=%x software_chip id=%x\n",
						  chip_info.chip_id,
						  chip_info.dev_id,
						  chip_info.software_chip_id);

	for (int i = 0; i < sizeof(supported_chips)/sizeof(chip_info_t); i++) {
		chip_id_mask = supported_chips[i].chip_id_mask;
		dev_id_mask = supported_chips[i].dev_id_mask;
		software_chip_id_mask = supported_chips[i].software_chip_id_mask;

		if ( (supported_chips[i].chip_id & chip_id_mask) !=
			(chip_info.chip_id & chip_id_mask) ) {
			CHIP_SUPPORT_LOGE("Current chip_id(%x)unsupport this hardware(%x).\n",
					  supported_chips[i].chip_id,chip_info.chip_id);
			continue;
		}

		if ( (supported_chips[i].dev_id & dev_id_mask) !=
			(chip_info.dev_id & dev_id_mask) ) {
			CHIP_SUPPORT_LOGE("Current dev_id(%x) unsupport this hardware(%x).\n",
					  supported_chips[i].dev_id,chip_info.dev_id);
			continue;
		}

		if ( (supported_chips[i].software_chip_id & software_chip_id_mask) <=
			(chip_info.software_chip_id & software_chip_id_mask) ) {
			CHIP_SUPPORT_LOGE("Current software unsupport this hardware (%x <= %x),please updata software.\n",
					  supported_chips[i].software_chip_id,chip_info.software_chip_id);
			continue;
		}

		CHIP_SUPPORT_LOGD("SUPPORTED chip id=%x device id=%x software_chip id=%x\n",
						  chip_info.chip_id,
						  chip_info.dev_id,
						  chip_info.software_chip_id);
		return true;
	}

	return false;
}

#endif //CONFIG_CHIP_SUPPORT

#include "sys_types.h"
extern uint32_t aon_pmu_hal_reg_get(pmu_reg_e reg);

hardware_chip_version_e bk_get_hardware_chip_id_version()
{
	uint32_t hardware_chip_id;
	hardware_chip_version_e chip_version;

	hardware_chip_id = aon_pmu_hal_reg_get(PMU_REG0x7c);
	//CHIP_SUPPORT_LOGI("hardware_chip_id value is 0x%x",hardware_chip_id);
	switch(hardware_chip_id)
	{
		case CHIP_ID_A_VALUE:
			chip_version = CHIP_VERSION_A;
			break;
		case CHIP_ID_B_VALUE:
			chip_version = CHIP_VERSION_B;
			break;
		case CHIP_ID_C_VALUE:
			chip_version = CHIP_VERSION_C;
			break;
		case CHIP_ID_D_VALUE:
			chip_version = CHIP_VERSION_D;
			break;
		default:
			chip_version = CHIP_VERSION_DEFAULT;
	}

	return chip_version;
}

bk_err_t bk_soc_info_get(soc_info_t* soc_info)
{
	soc_info->chip_id    = sys_drv_get_chip_id();
	soc_info->version_id = aon_pmu_hal_get_chipid();

	return BK_OK;
}

