/*
 * Copyright (c) 2022 Realtek, LLC.
 * All rights reserved.
 *
 * Licensed under the Realtek License, Version 1.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License from Realtek
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef AMEBA_COMPONENT_AUDIO_AUDIO_HAL_RTL8726E_AMEBA_AUDIO_STREAM_CONTROL_H
#define AMEBA_COMPONENT_AUDIO_AUDIO_HAL_RTL8726E_AMEBA_AUDIO_STREAM_CONTROL_H

#include "ameba_audio_stream.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _StreamControl {
	int32_t   board_amp_pin;
	bool      amp_state;
	uint32_t  playback_device;
	uint32_t  capture_usage;
	uint32_t  adc_use_status;
	bool      amic_ref_for_dmic;
	uint32_t  volume_for_dacl;
	uint32_t  volume_for_dacr;
	uint32_t  volume_for_adc[MAX_AD_NUM];
	uint32_t  mic_category_for_adc[MAX_AD_NUM];
	uint32_t  gain_for_micbst[MAX_AMIC_NUM];
} StreamControl;

StreamControl *ameba_audio_get_ctl(void);
int ameba_audio_ctl_set_tx_volume(StreamControl *control, float left, float right);
int ameba_audio_ctl_set_amp_pin(StreamControl *control, uint32_t pin);
int ameba_audio_ctl_get_amp_pin(StreamControl *control);
int ameba_audio_ctl_set_amp_state(StreamControl *control, bool state);
int ameba_audio_ctl_set_tx_mute(StreamControl *control, bool muted);
int ameba_audio_ctl_set_device_category(StreamControl *control, uint32_t device_category);
int ameba_audio_ctl_get_device_category(StreamControl *control);
int ameba_audio_ctl_set_mic_category_for_adc(StreamControl *control, uint32_t adc_num, uint32_t mic_category);
int ameba_audio_ctl_get_mic_category_for_adc(StreamControl *control, uint32_t adc_num);
int ameba_audio_ctl_set_adc_volume(StreamControl *control, uint32_t adc_num, uint32_t volume);
int ameba_audio_ctl_set_tdm_adc_volume(StreamControl *control, uint32_t channels, uint32_t volume);
int ameba_audio_ctl_set_mic_bst_gain(StreamControl *control, uint32_t mic_category, uint32_t gain);
int ameba_audio_ctl_set_mic_usage(StreamControl *control, uint32_t mic_usage);
int ameba_audio_ctl_get_mic_usage(StreamControl *control);
int ameba_audio_ctl_pll_clock_tune(StreamControl *control, uint32_t rate, uint32_t ppm, uint32_t action);

#ifdef __cplusplus
}
#endif

#endif