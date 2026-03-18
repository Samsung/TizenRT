// Copyright 2022-2024 Beken
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

//This is a generated file, don't modify it!

#pragma once


#include "security.h"
#define KB(size)                                      ((size) << 10)
#define MB(size)                                      ((size) << 20)

#define CONFIG_TFM_S_JUMP_TO_TFM_NS                   0
#define CONFIG_TFM_S_JUMP_TO_CPU0_APP                 1
#define CPU_VECTOR_ALIGN_SZ                           512
#define CONFIG_BL1_CONTROL_PHY_PARTITION_OFFSET       0x0
#define CONFIG_BL1_CONTROL_PHY_PARTITION_SIZE         0x1000

#define CONFIG_BOOT_FLAG_PHY_PARTITION_OFFSET         0x1000
#define CONFIG_BOOT_FLAG_PHY_PARTITION_SIZE           0x1000

#define CONFIG_PARTITION_PHY_PARTITION_OFFSET         0x2000
#define CONFIG_PARTITION_PHY_PARTITION_SIZE           0x1000

#define CONFIG_PRIMARY_MANIFEST_PHY_PARTITION_OFFSET  0x3000
#define CONFIG_PRIMARY_MANIFEST_PHY_PARTITION_SIZE    0x1000

#define CONFIG_BL2_PHY_PARTITION_OFFSET               0x4000
#define CONFIG_BL2_PHY_PARTITION_SIZE                 0x13000
#define CONFIG_BL2_PHY_CODE_START                     0x4000
#define CONFIG_BL2_VIRTUAL_PARTITION_SIZE             0x13000
#define CONFIG_BL2_VIRTUAL_CODE_START                 0x4000
#define CONFIG_BL2_VIRTUAL_CODE_SIZE                  0x13000

#define CONFIG_SYS_ITS_PHY_PARTITION_OFFSET           0x17000
#define CONFIG_SYS_ITS_PHY_PARTITION_SIZE             0x2000

#define CONFIG_SYS_PS_PHY_PARTITION_OFFSET            0x19000
#define CONFIG_SYS_PS_PHY_PARTITION_SIZE              0x2000

#define CONFIG_SS_PHY_PARTITION_OFFSET                0x1b000
#define CONFIG_SS_PHY_PARTITION_SIZE                  0x85000

#define CONFIG_RF_PHY_PARTITION_OFFSET                0xff6000
#define CONFIG_RF_PHY_PARTITION_SIZE                  0x1000

#define CONFIG_NET_PHY_PARTITION_OFFSET               0xff7000
#define CONFIG_NET_PHY_PARTITION_SIZE                 0x1000

#define CONFIG_EASYFLASH_PHY_PARTITION_OFFSET         0xff8000
#define CONFIG_EASYFLASH_PHY_PARTITION_SIZE           0x6000

#define CONFIG_BOOTPARAM_PHY_PARTITION_OFFSET         0xffe000
#define CONFIG_BOOTPARAM_PHY_PARTITION_SIZE           0x2000


#define PARTITION_MAP { \
    {"bl1_control", CONFIG_BL1_CONTROL_PHY_PARTITION_OFFSET, CONFIG_BL1_CONTROL_PHY_PARTITION_SIZE}, \
    {"boot_flag", CONFIG_BOOT_FLAG_PHY_PARTITION_OFFSET, CONFIG_BOOT_FLAG_PHY_PARTITION_SIZE}, \
    {"partition", CONFIG_PARTITION_PHY_PARTITION_OFFSET, CONFIG_PARTITION_PHY_PARTITION_SIZE}, \
    {"primary_manifest", CONFIG_PRIMARY_MANIFEST_PHY_PARTITION_OFFSET, CONFIG_PRIMARY_MANIFEST_PHY_PARTITION_SIZE}, \
    {"bl2", CONFIG_BL2_PHY_PARTITION_OFFSET, CONFIG_BL2_PHY_PARTITION_SIZE}, \
    {"sys_its", CONFIG_SYS_ITS_PHY_PARTITION_OFFSET, CONFIG_SYS_ITS_PHY_PARTITION_SIZE}, \
    {"sys_ps", CONFIG_SYS_PS_PHY_PARTITION_OFFSET, CONFIG_SYS_PS_PHY_PARTITION_SIZE}, \
    {"ss", CONFIG_SS_PHY_PARTITION_OFFSET, CONFIG_SS_PHY_PARTITION_SIZE}, \
    {"rf", CONFIG_RF_PHY_PARTITION_OFFSET, CONFIG_RF_PHY_PARTITION_SIZE}, \
    {"net", CONFIG_NET_PHY_PARTITION_OFFSET, CONFIG_NET_PHY_PARTITION_SIZE}, \
    {"easyflash", CONFIG_EASYFLASH_PHY_PARTITION_OFFSET, CONFIG_EASYFLASH_PHY_PARTITION_SIZE}, \
    {"bootparam", CONFIG_BOOTPARAM_PHY_PARTITION_OFFSET, CONFIG_BOOTPARAM_PHY_PARTITION_SIZE}, \
}
