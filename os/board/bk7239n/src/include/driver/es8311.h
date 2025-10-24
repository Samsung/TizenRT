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
//

#pragma once

#ifndef _ES8311_H
#define _ES8311_H

#include <driver/es8311_types.h>
#include <components/log.h>
#ifdef __cplusplus
extern "C" {
#endif


/*
 * @brief Initialize ES8311 codec chip
 *
 * @return
 *      - BK_OK
 *      - BK_FAIL
 */
bk_err_t es8311_codec_init(void);

/**
 * @brief Deinitialize ES8311 codec chip
 *
 * @return
 *     - BK_OK
 *     - BK_FAIL
 */
bk_err_t es8311_codec_deinit(void);

/**
 * @brief Configure ES8311 DAC mute or not. Basically you can use this function to mute the output or unmute
 *
 * @param enable
 *     - 1 enable
 *     - 0 disable
 *
 * @return
 *     - BK_FAIL Parameter error
 *     - BK_OK   Success
 */
bk_err_t es8311_set_voice_mute(bool enable);

/**
 * @brief  Set voice volume
 *
 * @param volume:  voice volume (0~100)
 *
 * @return
 *     - BK_OK
 *     - BK_FAIL
 */
bk_err_t es8311_codec_set_voice_volume(int volume);

/**
 * @brief Get voice volume
 *
 * @param[out] *volume:  voice volume (0~100)
 *
 * @return
 *     - BK_OK
 *     - BK_FAIL
 */
bk_err_t es8311_codec_get_voice_volume(int *volume);

/**
 * @brief Configure ES8311 I2S work mode
 *
 * @param mode:   ES8388 I2S work mode
 *
 * @return
 *     - BK_OK
 *     - BK_FAIL
 */
bk_err_t es8311_config_i2s_mode(i2s_work_mode_t mode);

/**
 * @brief Configure ES8311 data sample bits
 *
 * @param bits:  bit number of per sample
 *
 * @return
 *     - BK_OK
 *     - BK_FAIL
 */
bk_err_t es8311_set_bits_per_sample(i2s_data_width_t bits);

/**
 * @brief  Start ES8311 codec chip
 *
 * This API config adc codec
 *
 * @param cfg
 *
 * @return
 *     - BK_OK
 *     - BK_FAIL
 */
bk_err_t es8311_codec_start(es8311_codec_config_t *cfg);

/**
 * @brief  Stop ES8311 codec chip
 *
 * @return
 *     - BK_OK
 *     - BK_FAIL
 */
bk_err_t es8311_stop(void);

/**
 * @brief Get ES8311 DAC mute status
 *
 * @return
 *     - BK_FAIL
 *     - BK_OK
 */
bk_err_t es8311_get_voice_mute(int *mute);

/**
 * @brief Set ES8311 mic gain
 *
 * @param gain_db of mic gain
 *
 * @return
 *     - BK_FAIL Parameter error
 *     - BK_OK   Success
 */
bk_err_t es8311_set_mic_gain(es8311_mic_gain_t gain_db);

/**
 * @brief Print all ES8311 registers
 *
 * @return
 *     - void
 */
void es8311_read_all(void);

/**
 * @brief Write ES8311 register
 *
 * @param reg_addr: es8311 register address
 * @param data: value
 *
 *
 * @return
 *     - BK_OK
 *     - BK_FAIL
 */
bk_err_t es8311_write_reg(uint8_t reg_addr, uint8_t data);

/**
 * @brief Write ES8311 register
 *
 * @param reg_addr: es8311 register address
 *
 *
 * @return the register value
 */
int es8311_read_reg(uint8_t reg_addr);

#ifdef __cplusplus
}
#endif

#endif
