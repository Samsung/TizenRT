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

#define SOC_TIMER_UNIT_NUM               1
#define SOC_TIMER_CHAN_NUM_PER_UNIT      6
#define SOC_TIMER_GROUP_NUM              2
#define SOC_TIMER_CHAN_NUM_PER_GROUP     3
#define SOC_TIMER_INTERRUPT_NUM          2

#define TIMER_CLOCK_FREQ_XTAL   40000 // 40M
#define TIMER_CLOCK_FREQ_32K    32    // 32K

#ifdef __cplusplus
}
#endif

