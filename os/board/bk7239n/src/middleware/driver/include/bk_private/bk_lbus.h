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

#include <common/sys_config.h>

#if (defined(CONFIG_SOC_BK7271))
#define LBUS_BASE_ADDR                        (0x06000000)

#define LBUS_CONF0_REG                        (LBUS_BASE_ADDR + 0x180)
#define DSP_DOWNLOAD_ENABLE                      (1 << 0)
#define BT_DOWNLOAD_ENABLE                       (1 << 1)
#define DSP_RESET_ENABLE                         (1 << 2)
#define BT_RESET_ENABLE                          (1 << 3)

#define LBUS_CONF1_REG                        (LBUS_BASE_ADDR + 0x184)
#endif

#ifdef __cplusplus
}
#endif
