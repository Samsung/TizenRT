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
#include <common/bk_include.h>
#include <driver/aud_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* @brief Overview about this API header
 *
 */
typedef struct {
	aud_adc_samp_rate_t mic_samp_rate;
	aud_dac_samp_rate_t spk_samp_rate;
	uint16_t spk_gain;
} aud_uac_config_t;

/**
 * @brief AUD API
 * @defgroup bk_api_aud AUD API group
 * @{
 */

bk_err_t bk_aud_uac_driver_init(void);
bk_err_t bk_aud_uac_driver_deinit(void);
bk_err_t bk_aud_uac_start_mic(void);
bk_err_t bk_aud_uac_stop_mic(void);
bk_err_t bk_aud_uac_start_spk(void);
bk_err_t bk_aud_uac_stop_spk(void);
bk_err_t bk_aud_uac_register_mic_callback(void *cb);
bk_err_t bk_aud_uac_unregister_mic_callback(void);
bk_err_t bk_aud_uac_register_spk_buff_ptr(void *buff, uint32_t size);
bk_err_t bk_aud_uac_register_spk_callback(void *cb);
bk_err_t bk_aud_uac_unregister_spk_callback(void);
bk_err_t bk_aud_uac_set_mic_samp_rate(uint32_t value);
bk_err_t bk_aud_uac_set_spk_samp_rate(uint32_t value);
bk_err_t bk_aud_uac_set_spk_gain(uint32_t value);
bk_err_t bk_aud_uac_get_spk_gain(uint32_t *value);
bk_err_t bk_aud_uac_register_disconnect_cb(void *callback);
bk_err_t bk_aud_uac_register_connect_cb(void *callback);
bk_err_t bk_aud_uac_check_spk_gain_cfg(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
