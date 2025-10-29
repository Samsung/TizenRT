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

#include <stdio.h>
#include <soc/soc.h>
#include <driver/hal/hal_adc_types.h>
#include <driver/hal/hal_gpio_types.h>

#define MAP_INVALID_ITEM   0xFF
#define INVALID	MAP_INVALID_ITEM

#define ADC_DEV_MAP  \
{\
	{0, INVALID, INVALID},\
	{ADC_1, GPIO_2, GPIO_DEV_ADC1},\
	{ADC_2, GPIO_3, GPIO_DEV_ADC2},\
	{ADC_3, GPIO_4, GPIO_DEV_ADC3},\
	{ADC_4, GPIO_5, GPIO_DEV_ADC4},\
	{ADC_5, GPIO_6, GPIO_DEV_ADC5},\
	{ADC_6, GPIO_7, GPIO_DEV_ADC6},\
	{ADC_7, INVALID, INVALID},\
	{ADC_8, INVALID, INVALID},\
	{ADC_9, INVALID, INVALID},\
	{ADC_10, GPIO_8, GPIO_DEV_ADC10},\
	{ADC_11, INVALID, INVALID},\
	{ADC_12, INVALID, INVALID},\
	{ADC_13, INVALID, INVALID},\
	{ADC_14, GPIO_12, GPIO_DEV_ADC14},\
	{ADC_15, GPIO_13, GPIO_DEV_ADC15},\
}

#ifdef __cplusplus
}
#endif
