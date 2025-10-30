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

#define WDT_R_BASE                  (SOC_WDT_REG_BASE)

#define WDT_R_CTRL                  (WDT_R_BASE + 0x4 * 4)

#define WDT_F_PERIOD                (BIT(0))
#define WDT_F_PERIOD_M              (0xffff)
#define WDT_F_PERIOD_V              (0xffff)
#define WDT_F_PERIOD_S              (0)

#define WDT_F_KEY                   (BIT(16))
#define WDT_F_KEY_M                 (0xff)
#define WDT_F_KEY_V                 (0xff)
#define WDT_F_KEY_S                 (16)

#define WDT_V_KEY_1ST               (0x5A)
#define WDT_V_KEY_2ND               (0xA5)
#define WDT_V_PERIOD_DEFAULT_VALUE  (0x4000)

#ifdef __cplusplus
}
#endif
