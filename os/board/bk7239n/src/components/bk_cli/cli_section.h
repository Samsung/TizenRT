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

#include "sdkconfig.h"

#define CLI_SECTION(x) __attribute__((used, section(x)))
#define CLI_CMD_LABLE  CLI_SECTION(".cli_cmdtabl")

extern const int _cli_cmds_start __attribute__((weak));
extern const int _cli_cmds_end __attribute__((weak));

#if 0// (!((defined(CONFIG_DEBUG_FIRMWARE)) && (!defined(CONFIG_CLI_CODE_SIZE_OPTIMIZE_ENABLE))))
#undef CLI_CMD_EXPORT
#define CLI_CMD_EXPORT
#endif

#if defined(CONFIG_ARCH_CLI_SUPPORT)
#define ARCH_CLI_CMD_EXPORT CLI_CMD_LABLE
#else
#define ARCH_CLI_CMD_EXPORT 
#endif

#if defined(CONFIG_COMPONENT_CLI_SUPPORT)
#define COMPONENTS_CLI_CMD_EXPORT CLI_CMD_LABLE
#else
#define COMPONENTS_CLI_CMD_EXPORT
#endif

#if defined(CONFIG_DRV_CLI_SUPPORT)
#define DRV_CLI_CMD_EXPORT CLI_CMD_LABLE
#else
#define DRV_CLI_CMD_EXPORT
#endif

// eof

