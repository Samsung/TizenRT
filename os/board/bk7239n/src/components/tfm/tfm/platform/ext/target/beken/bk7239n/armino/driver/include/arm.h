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

#include <common/sys_config.h>

#define BOOT_ARM_MODE_MASK                    0x1F
#define BOOT_ARM_MODE_USR                     0x10
#define BOOT_ARM_MODE_FIQ                     0x11
#define BOOT_ARM_MODE_IRQ                     0x12
#define BOOT_ARM_MODE_SVC                     0x13
#define BOOT_ARM_MODE_ABT                     0x17
#define BOOT_ARM_MODE_UND                     0x1B
#define BOOT_ARM_MODE_SYS                     0x1F

#define ARM_FIQ_IRQ_MASK                      0xC0
#define ARM_IRQ_MASK                          0x80

#ifdef __cplusplus
}
#endif
