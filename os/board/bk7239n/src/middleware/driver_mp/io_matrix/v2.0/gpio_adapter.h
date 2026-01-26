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

#include <driver/io_matrix_types.h>
#include <driver/gpio_types.h>

#ifdef __cplusplus
extern "C" {
#endif


// Peripheral group type for wakeup
typedef enum {
	PERIPHERAL_GROUP_NONE = 0,
	PERIPHERAL_GROUP_GPIO,
	PERIPHERAL_GROUP_I2C0,
	PERIPHERAL_GROUP_I2C1,
	PERIPHERAL_GROUP_SPI0,
	PERIPHERAL_GROUP_SPI1,
	PERIPHERAL_GROUP_UART0,  // UART1 in hardware
	PERIPHERAL_GROUP_UART1,  // UART2 in hardware
	PERIPHERAL_GROUP_UART2,  // UART3 in hardware
	PERIPHERAL_GROUP_UART3,  // UART4 in hardware
} peripheral_group_t;

// Get peripheral group that caused wakeup (automatically get wakeup GPIO ID)
peripheral_group_t bk_gpio_get_wake_source_sub_type(void);

#ifdef __cplusplus
}
#endif