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

#pragma once

#include <components/log.h>

#define CHIP_SUPPORT_TAG "chip_support"
#define CHIP_SUPPORT_LOGI(...) BK_LOGI(CHIP_SUPPORT_TAG, ##__VA_ARGS__)
#define CHIP_SUPPORT_LOGW(...) BK_LOGW(CHIP_SUPPORT_TAG, ##__VA_ARGS__)
#define CHIP_SUPPORT_LOGE(...) BK_LOGE(CHIP_SUPPORT_TAG, ##__VA_ARGS__)
#define CHIP_SUPPORT_LOGD(...) BK_LOGD(CHIP_SUPPORT_TAG, ##__VA_ARGS__)

/** @brief   This enumeration defines hardware_chip_version.  */
typedef enum
{
	CHIP_VERSION_A = 0,
	CHIP_VERSION_B,
	CHIP_VERSION_C,
	CHIP_VERSION_DEFAULT
}hardware_chip_version_e;

/** @brief   This enumeration defines hardware_chip_id_value.  */
typedef enum
{
	CHIP_ID_A_VALUE = 0x22041020,
	CHIP_ID_B_VALUE ,
	CHIP_ID_C_VALUE = 0x22091022,
	CHIP_ID_DEFAULT_VALUE
}hardware_chip_id_value_e;

bool bk_is_chip_supported(void);

/** 
* @brief  Get chip id version.
* 
* @return current chip_version 
* 	 0:  means chip_version_A.
* 	 1:  means chip_version_B.
* 	 2:  means chip_version_C.
* 	 3:  means default version.
*/
hardware_chip_version_e bk_get_hardware_chip_id_version();