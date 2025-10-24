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

#include "reg_base.h"

#define JPEG_SRAM_ADDRESS (SOC_SRAM4_DATA_BASE)
#define YUV_BUFFER_BASE_ADDR   (0x60000000UL)
#define JPEG_PSRAM_BUFFER_SIZE (1024 * 40)
#define H264_PSRAM_BUFFER_SIZE (1024 * 80)

