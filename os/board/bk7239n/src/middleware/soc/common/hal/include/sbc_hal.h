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

#include <driver/sbc_types.h>

#ifdef __cplusplus
extern "C" {
#endif


void sbc_decoder_hal_sbc_config(const sbc_config_t *config);void sbc_decoder_hal_sbc_enable(void);

uint32_t sbc_decoder_hal_get_sbc_status_value(void);

void sbc_decoder_hal_sbc_support_msbc(bool enable);

uint32_t sbc_decoder_hal_get_sbc_ctrl_value(void);

void sbc_decoder_hal_sbc_clear_interrupt_status(void);

uint32_t sbc_decoder_hal_get_sbc_interrupt_status(void);

uint32_t sbc_decoder_hal_get_sbc_idle_status(void);

void sbc_decoder_hal_set_res_bytel_value(uint32_t bytel_value);

void sbc_decoder_hal_set_res_bytem_value(uint32_t bytem_value);

void sbc_decoder_hal_set_res_byteh_value(uint32_t byteh_value);

void sbc_decoder_hal_set_sbc_bit_num(uint32_t value);

void sbc_decoder_hal_set_scale_factor(uint32_t value);

void sbc_decoder_hal_set_sbc_level(uint32_t value);

void sbc_decoder_hal_set_sbc_res_bit(uint32_t value);

void sbc_decoder_hal_decode_enable(void);

uint32_t sbc_decoder_hal_get_decode_enable_value(void);

uint32_t sbc_decoder_hal_get_pcm_data(void);

void sbc_decoder_hal_mem_init(void);

void sbc_decoder_hal_soft_reset(void);

#ifdef __cplusplus
}
#endif



