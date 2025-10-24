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
#include <components/usb.h>
#include <components/usb_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* @brief Overview about this API header
 *
 */

/* uac 1.0 version */
typedef enum {
	AUD_UAC_DATA_FORMAT_TYPE_UNDEFINED = 0,
	AUD_UAC_DATA_FORMAT_TYPE_PCM,
	AUD_UAC_DATA_FORMAT_TYPE_PCM8,
	AUD_UAC_DATA_FORMAT_TYPE_IEEE_FLOAT,
	AUD_UAC_DATA_FORMAT_TYPE_ALAW,
	AUD_UAC_DATA_FORMAT_TYPE_MULAW,
	AUD_UAC_DATA_FORMAT_TYPE_MAX
} aud_uac_data_format_type_t;

typedef struct {
    aud_uac_data_format_type_t mic_format_tag;
    uint32_t mic_samp_rate;
} aud_uac_mic_config_t;

typedef struct {
    aud_uac_data_format_type_t spk_format_tag;
    uint32_t spk_samp_rate;
    uint16_t spk_volume;
} aud_uac_spk_config_t;

typedef struct {
    aud_uac_mic_config_t mic_config;
    aud_uac_spk_config_t spk_config;
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

bk_err_t bk_aud_uac_set_param(aud_uac_config_t *config);
bk_err_t bk_aud_uac_mic_set_param(aud_uac_mic_config_t *config);
bk_err_t bk_aud_uac_spk_set_param(aud_uac_spk_config_t *config);
bk_err_t bk_aud_uac_register_transfer_buffer_ops(void *cb);
bk_err_t bk_aud_uac_register_disconnect_cb(void *callback);
bk_err_t bk_aud_uac_register_connect_cb(void *callback);
bk_err_t bk_aud_uac_set_spk_gain(uint32_t value);
bk_err_t bk_aud_uac_get_spk_gain(uint32_t *value);
bk_err_t bk_aud_uac_ctrl_spk_mute(uint32_t value);
bk_err_t bk_aud_uac_check_spk_gain_cfg(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
