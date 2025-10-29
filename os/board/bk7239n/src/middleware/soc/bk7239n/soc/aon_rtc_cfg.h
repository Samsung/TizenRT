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

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define AON_RTC_TICK_VAL_MAX (0xffffffff)
#define AON_RTC_UPPER_VAL_MAX (0xffffffff)

#define AON_RTC_SOC_REG_ADDRESS_OFFSET (0)

#define AON_RTC_INT_CONFIG_TABLE \
{\
	{AON_RTC_ID_1, INT_SRC_NONE, NULL},\
}

#ifdef __cplusplus
}
#endif


