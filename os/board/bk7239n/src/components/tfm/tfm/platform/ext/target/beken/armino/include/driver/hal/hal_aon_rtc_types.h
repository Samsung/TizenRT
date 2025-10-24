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

#include <common/bk_err.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief AON RTC defines
 * @defgroup bk_api_aon_rtc_defs macos
 * @ingroup bk_api_aon_rtc
 * @{
 */

typedef enum
{
	AON_RTC_ID_1 = 0,
#if (SOC_AON_RTC_UNIT_NUM > 1)
	AON_RTC_ID_2,
#endif
	AON_RTC_ID_MAX,
}aon_rtc_id_t;

typedef uint8_t aon_rtc_unit_t; /**< aon rtc uint id */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

