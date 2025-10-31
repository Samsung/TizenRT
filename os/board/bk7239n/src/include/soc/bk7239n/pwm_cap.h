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

#define SOC_PWM_UNIT_NUM               1
#define SOC_PWM_CHAN_NUM_PER_UNIT      12
#define SOC_PWM_CHAN_NUM_PER_GROUP     3
#define SOC_PWM_GROUP_NUM              4
#define SOC_PWM_CHAN_NUM_MAX           (SOC_PWM_UNIT_NUM * SOC_PWM_CHAN_NUM_PER_UNIT)
#define SOC_PWM_TIM_NUM                12

#define PWM_CLOCK_SRC_XTAL   40000000 // 40M
#define PWM_CLOCK_SRC_32K    32000    // 32K

#ifdef __cplusplus
}
#endif
