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

//the max of channel number is 4
#define SOC_I2S_CHANNEL_NUM          3
#define SOC_I2S_ISR_NUM              SOC_I2S_CHANNEL_NUM*4
#define SOC_I2S_APLL_RATE            98304000   //98.304M
#define SOC_I2S_UNIT_NUM             1

#ifdef __cplusplus
}
#endif

