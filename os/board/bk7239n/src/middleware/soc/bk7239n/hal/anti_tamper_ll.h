// Copyright 2024-2025 Beken
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

#include <driver/hal/hal_gpio_types.h>
#include <soc/soc.h>
#include "hal_port.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ANTI_TAMPER_RX_PORTS {GPIO_2, GPIO_5, GPIO_8, GPIO_12, GPIO_18, GPIO_23}
#define ANTI_TAMPER_TX_PORTS {GPIO_3, GPIO_6, GPIO_9, GPIO_13, GPIO_19, GPIO_22}

#ifdef __cplusplus
}
#endif

