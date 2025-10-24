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

#ifdef __cplusplus
extern "C" {
#endif

#define EFUSE_R_BASE         (SOC_EFUSE_REG_BASE)

#define EFUSE_R_CTRL         (EFUSE_R_BASE)

#define EFUSE_F_ADDR         (BIT(8))
#define EFUSE_F_ADDR_M       (0x1F)
#define EFUSE_F_ADDR_V       (0x1F)
#define EFUSE_F_ADDR_S       (8)

#define EFUSE_F_WR_DATA      (BIT(16))
#define EFUSE_F_WR_DATA_M    (0xFF)
#define EFUSE_F_WR_DATA_V    (0xFF)
#define EFUSE_F_WR_DATA_S    (16)

#define EFUSE_R_OPTR         (EFUSE_R_BASE + 0x4 * 1)

#define EFUSE_F_RD_DATA      (BIT(0))
#define EFUSE_F_RD_DATA_M    (0xFF)
#define EFUSE_F_RD_DATA_V    (0xFF)
#define EFUSE_F_RD_DATA_S    (0)

#ifdef __cplusplus
}
#endif

