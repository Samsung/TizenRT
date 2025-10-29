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

typedef enum {
    CLK_PIN_ANA_ROSC = 0,
    CLK_PIN_ANA_XTAL,
    CLK_PIN_ANA_DCO,
    CLK_PIN_ANA_DPLL,
    CLK_PIN_ANA_INVALID,
} clk_pin_ana_src_t;

typedef enum {
    CLK_PIN_DIG_CLK13M = 0,
    CLK_PIN_DIG_CLK26M,
    CLK_PIN_DIG_XTAL,
    CLK_PIN_DIG_XTAL_DIV,
    CLK_PIN_DIG_LPO_CLK_ROSC,
    CLK_PIN_DIG_LPO_CLK_XTALL,
    CLK_PIN_DIG_LPO_CLK_XTALH,
    CLK_PIN_DIG_CLK_AUXS_DCO,
    CLK_PIN_DIG_CLK_AUXS_APLL,
    CLK_PIN_DIG_CLK_AUXS_CLK_320M,
    CLK_PIN_DIG_CLK_AUXS_CLK_480M,
    CLK_PIN_DIG_INVALID,
} clk_pin_dig_src_t;

typedef enum {
    CLK_PIN_DIV_1 = 0,
    CLK_PIN_DIV_2,
    CLK_PIN_DIV_3,
    CLK_PIN_DIV_4,
    CLK_PIN_DIV_5,
    CLK_PIN_DIV_6,
    CLK_PIN_DIV_7,
    CLK_PIN_DIV_8,
    CLK_PIN_DIV_9,
    CLK_PIN_DIV_10,
    CLK_PIN_DIV_11,
    CLK_PIN_DIV_12,
    CLK_PIN_DIV_13,
    CLK_PIN_DIV_14,
    CLK_PIN_DIV_15,
    CLK_PIN_DIV_16,
    CLK_PIN_DIV_INVALID,
} clk_pin_div_t;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
