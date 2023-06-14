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
#include "ameba_audio_stream_control.h"

#include "platform_stdlib.h"
#include "ameba_sport.h"
#include "ameba_audio.h"
#include "ameba_audio_types.h"
#include "ameba_audio_hw_usrcfg.h"
#include "audio_hw_debug.h"
#include "audio_hw_osal_errnos.h"

#include "hardware/audio/audio_hw_types.h"

#define DEVICE_SPEAKER 1
#define MAX_DAC_VOLUME 141

#define IS_AUDIO_DMIC(NUM) (((NUM) == AMEBA_AUDIO_DMIC1) || ((NUM) == AMEBA_AUDIO_DMIC2) \
						|| ((NUM) == AMEBA_AUDIO_DMIC3) || ((NUM) == AMEBA_AUDIO_DMIC4) \
						|| ((NUM) == AMEBA_AUDIO_DMIC5) || ((NUM) == AMEBA_AUDIO_DMIC6) \
						|| ((NUM) == AMEBA_AUDIO_DMIC7) || ((NUM) == AMEBA_AUDIO_DMIC8))

static StreamControl *g_control_instance = NULL;
static _mutex s_stream_ctl_instance_lock;

int ameba_audio_ctl_set_mic_category_for_adc(StreamControl *control, uint32_t adc_num, uint32_t mic_category);

__attribute__((constructor)) static void stream_ctl_instance_lock_init(void)
{
	HAL_AUDIO_INFO("create stream tcl single instance.");
	rtw_mutex_init(&s_stream_ctl_instance_lock);
}

__attribute__((destructor)) static void stream_ctl_instance_lock_free(void)
{
	HAL_AUDIO_INFO("destroy stream tcl single instance.");
	rtw_mutex_free(&s_stream_ctl_instance_lock);
}

StreamControl *ameba_audio_get_ctl(void)
{
	rtw_mutex_get(&s_stream_ctl_instance_lock);
	if (g_control_instance == NULL) {
		g_control_instance = (StreamControl *)calloc(1, sizeof(StreamControl));
		if (!g_control_instance) {
			HAL_AUDIO_ERROR("calloc control fail");
			return NULL;
		}
		g_control_instance->board_amp_pin = AUDIO_HW_AMPLIFIER_PIN;
		g_control_instance->amp_state = false;
		g_control_instance->playback_device = AMEBA_AUDIO_DEVICE_SPEAKER;
		g_control_instance->capture_usage = AMEBA_AUDIO_CAPTURE_USAGE_AMIC;
		g_control_instance->adc_use_status = 0;
		g_control_instance->volume_for_dacl = 0x50;
		g_control_instance->volume_for_dacr = 0x50;
		g_control_instance->volume_for_adc[0] = 0x2f;
		g_control_instance->volume_for_adc[1] = 0x2f;
		g_control_instance->volume_for_adc[2] = 0x2f;
		g_control_instance->volume_for_adc[3] = 0x2f;
		g_control_instance->volume_for_adc[4] = 0x2f;
		g_control_instance->volume_for_adc[5] = 0x2f;
		g_control_instance->volume_for_adc[6] = 0x2f;
		g_control_instance->volume_for_adc[7] = 0x2f;
		g_control_instance->mic_category_for_adc[0] = AMEBA_AUDIO_AMIC1;
		g_control_instance->mic_category_for_adc[1] = AMEBA_AUDIO_AMIC2;
		g_control_instance->mic_category_for_adc[2] = AMEBA_AUDIO_AMIC3;
		g_control_instance->mic_category_for_adc[3] = AMEBA_AUDIO_AMIC4;
		g_control_instance->mic_category_for_adc[4] = AMEBA_AUDIO_AMIC5;
		g_control_instance->mic_category_for_adc[5] = AMEBA_AUDIO_AMIC5;
		g_control_instance->mic_category_for_adc[6] = AMEBA_AUDIO_AMIC5;
		g_control_instance->mic_category_for_adc[7] = AMEBA_AUDIO_AMIC5;
		g_control_instance->gain_for_micbst[0] = MICBST_GAIN_30DB;
		g_control_instance->gain_for_micbst[1] = MICBST_GAIN_30DB;
		g_control_instance->gain_for_micbst[2] = MICBST_GAIN_30DB;
		g_control_instance->gain_for_micbst[3] = MICBST_GAIN_30DB;
		g_control_instance->gain_for_micbst[4] = MICBST_GAIN_30DB;
	}

	rtw_mutex_put(&s_stream_ctl_instance_lock);
	return g_control_instance;
}

int ameba_audio_ctl_set_tx_volume(StreamControl *control, float left, float right)
{
	if (control == NULL) {
		HAL_AUDIO_ERROR("ops, %s fail, control null", __func__);
		return HAL_OSAL_ERR_INVALID_OPERATION;
	}

	uint32_t l = left * MAX_DAC_VOLUME;
	uint32_t r = right * MAX_DAC_VOLUME;
	if (l != r) {
		HAL_AUDIO_VERBOSE("left & right volume is not same");
	}

	if (control->volume_for_dacl != l) {
		HAL_AUDIO_INFO("set dacl volume to 0x%lx", l);
		control->volume_for_dacl = l;
		if (ameba_audio_is_audio_ip_in_use(CODEC)) {
			AUDIO_CODEC_SetDACVolume(DAC_L, l);
		}
	}

	if (control->volume_for_dacr != r) {
		HAL_AUDIO_INFO("set dacr volume to 0x%lx", r);
		control->volume_for_dacr = r;
		if (ameba_audio_is_audio_ip_in_use(CODEC)) {
			AUDIO_CODEC_SetDACVolume(DAC_R, r);
		}
	}

	return HAL_OSAL_OK;
}

int ameba_audio_ctl_set_tx_mute(StreamControl *control, bool muted)
{
	if (control == NULL) {
		HAL_AUDIO_ERROR("ops, %s fail, control null", __func__);
		return HAL_OSAL_ERR_INVALID_OPERATION;
	}

	if (muted) {
		HAL_AUDIO_INFO("dac mute.");
		AUDIO_CODEC_SetDACVolume(DAC_L, 0);
		AUDIO_CODEC_SetDACVolume(DAC_R, 0);
		//max time wait: 175/44100ms
		vTaskDelay(4 / portTICK_RATE_MS);
		AUDIO_CODEC_SetDACMute(DAC_L, MUTE);
		AUDIO_CODEC_SetDACMute(DAC_R, MUTE);
	} else {
		HAL_AUDIO_INFO("dac unmute.");
		AUDIO_CODEC_SetDACMute(DAC_L, UNMUTE);
		AUDIO_CODEC_SetDACMute(DAC_R, UNMUTE);
		//no need to wait, because, we set volume here, and then data render
		//to hardware, and hardware will itself fade in.
		AUDIO_CODEC_SetDACVolume(DAC_L, control->volume_for_dacl);
		AUDIO_CODEC_SetDACVolume(DAC_R, control->volume_for_dacr);
	}

	return HAL_OSAL_OK;
}

int ameba_audio_ctl_set_amp_pin(StreamControl *control, uint32_t pin)
{
	if (control == NULL) {
		HAL_AUDIO_ERROR("ops, %s fail, control null", __func__);
		return HAL_OSAL_ERR_INVALID_OPERATION;
	}

	if (control->board_amp_pin != (int32_t)pin) {
		HAL_AUDIO_INFO("set amp pin from %" PRId32 " to %" PRIu32 "", control->board_amp_pin, pin);
		control->board_amp_pin = pin;
	}

	return HAL_OSAL_OK;
}

int ameba_audio_ctl_get_amp_pin(StreamControl *control)
{
	if (control == NULL) {
		HAL_AUDIO_ERROR("ops, %s fail, control null", __func__);
		return HAL_OSAL_ERR_INVALID_OPERATION;
	}
	return control->board_amp_pin;
}

int ameba_audio_ctl_set_amp_state(StreamControl *control, bool state)
{
	if (control == NULL) {
		HAL_AUDIO_ERROR("ops, %s fail, control null", __func__);
		return HAL_OSAL_ERR_INVALID_OPERATION;
	}

	control->amp_state = state;
	return HAL_OSAL_OK;
}

int ameba_audio_ctl_set_device_category(StreamControl *control, uint32_t device_category)
{
	if (control == NULL) {
		HAL_AUDIO_ERROR("ops, %s fail, control null", __func__);
		return HAL_OSAL_ERR_INVALID_OPERATION;
	}
	control->playback_device = device_category;
	return HAL_OSAL_OK;
}

int ameba_audio_ctl_get_device_category(StreamControl *control)
{
	if (control == NULL) {
		HAL_AUDIO_ERROR("ops, %s fail, control null", __func__);
		return HAL_OSAL_ERR_INVALID_OPERATION;
	}
	return control->playback_device;
}

int ameba_audio_ctl_set_mic_usage(StreamControl *control, uint32_t mic_usage)
{
	if (control == NULL) {
		HAL_AUDIO_ERROR("ops, %s fail, control null", __func__);
		return HAL_OSAL_ERR_INVALID_OPERATION;
	}

	control->capture_usage = mic_usage;
	switch (mic_usage) {
	case AMEBA_AUDIO_CAPTURE_USAGE_AMIC:
		control->mic_category_for_adc[0] = AMEBA_AUDIO_AMIC1;
		control->mic_category_for_adc[1] = AMEBA_AUDIO_AMIC2;
		control->mic_category_for_adc[2] = AMEBA_AUDIO_AMIC3;
		control->mic_category_for_adc[3] = AMEBA_AUDIO_AMIC4;
		control->mic_category_for_adc[4] = AMEBA_AUDIO_AMIC5;
		control->mic_category_for_adc[5] = AMEBA_AUDIO_AMIC5;
		control->mic_category_for_adc[6] = AMEBA_AUDIO_AMIC5;
		control->mic_category_for_adc[7] = AMEBA_AUDIO_AMIC5;
		break;
	case AMEBA_AUDIO_CAPTURE_USAGE_DMIC:
		control->mic_category_for_adc[0] = AMEBA_AUDIO_DMIC1;
		control->mic_category_for_adc[1] = AMEBA_AUDIO_DMIC2;
		control->mic_category_for_adc[2] = AMEBA_AUDIO_DMIC3;
		control->mic_category_for_adc[3] = AMEBA_AUDIO_DMIC4;
		control->mic_category_for_adc[4] = AMEBA_AUDIO_DMIC5;
		control->mic_category_for_adc[5] = AMEBA_AUDIO_DMIC6;
		control->mic_category_for_adc[6] = AMEBA_AUDIO_DMIC7;
		control->mic_category_for_adc[7] = AMEBA_AUDIO_DMIC8;
		break;
	case AMEBA_AUDIO_CAPTURE_USAGE_DMIC_REF_AMIC:
		control->mic_category_for_adc[0] = AMEBA_AUDIO_DMIC1;
		control->mic_category_for_adc[1] = AMEBA_AUDIO_DMIC2;
		control->mic_category_for_adc[2] = AMEBA_AUDIO_AMIC5;
		control->mic_category_for_adc[3] = AMEBA_AUDIO_AMIC5;
		control->mic_category_for_adc[4] = AMEBA_AUDIO_AMIC5;
		control->mic_category_for_adc[5] = AMEBA_AUDIO_AMIC5;
		control->mic_category_for_adc[6] = AMEBA_AUDIO_AMIC5;
		control->mic_category_for_adc[7] = AMEBA_AUDIO_AMIC5;
		break;
	default:
		HAL_AUDIO_ERROR("unsupported mic usage");
		break;
	}

	control->adc_use_status = 0;

	return HAL_OSAL_OK;
}

int ameba_audio_ctl_get_mic_usage(StreamControl *control)
{
	if (control == NULL) {
		HAL_AUDIO_ERROR("ops, %s fail, control null", __func__);
		return HAL_OSAL_ERR_INVALID_OPERATION;
	}
	return control->capture_usage;
}

int ameba_audio_ctl_set_mic_category_for_adc(StreamControl *control, uint32_t adc_num, uint32_t mic_category)
{
	if (control == NULL) {
		HAL_AUDIO_ERROR("ops, %s fail, control null", __func__);
		return HAL_OSAL_ERR_INVALID_OPERATION;
	}

	if (adc_num >= MAX_AD_NUM) {
		HAL_AUDIO_ERROR("ops, adc_num(%lu) out of range", adc_num);
		return HAL_OSAL_ERR_INVALID_OPERATION;
	}

	control->adc_use_status |= ((uint32_t)0x00000001 << adc_num);

	if (control->mic_category_for_adc[adc_num] != mic_category) {
		HAL_AUDIO_INFO("ADC: %" PRIu32 " use mic: %" PRIu32 "", adc_num, mic_category);
		control->mic_category_for_adc[adc_num] = mic_category;
	}

	return HAL_OSAL_OK;
}

int ameba_audio_ctl_get_mic_category_for_adc(StreamControl *control, uint32_t adc_num)
{
	if (control == NULL) {
		HAL_AUDIO_ERROR("ops, %s fail, control null", __func__);
		return HAL_OSAL_ERR_INVALID_OPERATION;
	}

	return control->mic_category_for_adc[adc_num];
}

int ameba_audio_ctl_set_adc_volume(StreamControl *control, uint32_t adc_num, uint32_t volume)
{
	if (control == NULL) {
		HAL_AUDIO_ERROR("ops, %s fail, control null", __func__);
		return HAL_OSAL_ERR_INVALID_OPERATION;
	}

	if (adc_num >= MAX_AD_NUM) {
		HAL_AUDIO_ERROR("ops, adc_num(%lu) out of range", adc_num);
		return HAL_OSAL_ERR_INVALID_OPERATION;
	}

	control->volume_for_adc[adc_num] = volume;

	if (ameba_audio_is_audio_ip_in_use(CODEC)) {
		AUDIO_CODEC_SetADCVolume(adc_num, volume);
	}

	return HAL_OSAL_OK;
}

int ameba_audio_ctl_set_tdm_adc_volume(StreamControl *control, uint32_t channels, uint32_t volume)
{
	if (control == NULL) {
		HAL_AUDIO_ERROR("ops, %s fail, control null", __func__);
		return HAL_OSAL_ERR_INVALID_OPERATION;
	}

	if (channels > MAX_AD_NUM) {
		HAL_AUDIO_ERROR("ops, channels(%lu) out of range", channels);
		return HAL_OSAL_ERR_INVALID_OPERATION;
	}

	uint32_t i = 0;
	for (; i < channels; i++) {
		control->volume_for_adc[i] = volume;
		if (ameba_audio_is_audio_ip_in_use(CODEC)) {
			AUDIO_CODEC_SetADCVolume(i, volume);
		}
	}

	return HAL_OSAL_OK;
}

int ameba_audio_ctl_set_mic_bst_gain(StreamControl *control, uint32_t mic_category, uint32_t gain)
{
	uint32_t amic_num;
	if (control == NULL) {
		HAL_AUDIO_ERROR("ops, %s fail, control null", __func__);
		return HAL_OSAL_ERR_INVALID_OPERATION;
	}

	switch (mic_category) {
	case AMEBA_AUDIO_AMIC1:
		amic_num = AMIC1;
		break;
	case AMEBA_AUDIO_AMIC2:
		amic_num = AMIC2;
		break;
	case AMEBA_AUDIO_AMIC3:
		amic_num = AMIC3;
		break;
	case AMEBA_AUDIO_AMIC4:
		amic_num = AMIC4;
		break;
	case AMEBA_AUDIO_AMIC5:
		amic_num = AMIC5;
		break;
	default:
		HAL_AUDIO_ERROR("mic category not supported:%" PRIu32 "", mic_category);
		return HAL_OSAL_ERR_INVALID_OPERATION;
	}

	HAL_AUDIO_INFO("ameba_audio_ctl_set_mic_bst_gain mic: %" PRIu32 ", gain: %" PRIu32 "",
				   amic_num, gain);
	if (control->gain_for_micbst[amic_num - 1] != gain) {
		if (ameba_audio_is_audio_ip_in_use(CODEC)) {
			AUDIO_CODEC_SetMicBstGain(amic_num, gain);
		}
		control->gain_for_micbst[amic_num - 1] = gain;
	}

	return HAL_OSAL_OK;

}

int ameba_audio_ctl_pll_clock_tune(StreamControl *control, uint32_t rate, uint32_t ppm, uint32_t action)
{
	if (control == NULL) {
		HAL_AUDIO_ERROR("ops, %s fail, control null", __func__);
		return HAL_OSAL_ERR_INVALID_OPERATION;
	}

	if (rate % 8000 == 0) {
		PLL_I2S_98P304M_ClkTune(ppm, action);
	} else {
		PLL_I2S_45P158M_ClkTune(ppm, action);
	}

	return HAL_OSAL_OK;
}
