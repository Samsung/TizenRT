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

#include <components/log.h>
#include <driver/gpio_types.h>
#include <driver/hal/hal_clock_types.h>

typedef struct {
	gpio_clk_pin_src_t clk_src;
	struct {
		gpio_id_t gpio_id;
		clk_pin_ana_src_t ana_src;
	} ana_map;
	struct {
		gpio_id_t gpio_id;
		gpio_dev_t gpio_dev;
		clk_pin_dig_src_t dig_src;
	} dig_map;
} gpio_clk_pin_map_t;


#define GPIO_CLK_PIN_GPIO_NEED_RECOVER 0