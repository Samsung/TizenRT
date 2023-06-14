/*
 * Copyright (c) 2021 Realtek, LLC.
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
#include "audio_hw_debug.h"
#include "audio_hw_osal_errnos.h"

#include "hardware/audio/audio_hw_control.h"

static struct AudioHwControl *s_hw_ctl_instance = NULL;
static _mutex s_hw_ctl_instance_lock;

static int32_t AmebaSetHardwareVolume(struct AudioHwControl *control, float left_volume, float right_volume)
{
	(void) control;
	return ameba_audio_ctl_set_tx_volume(ameba_audio_get_ctl(), left_volume, right_volume);
}

static int32_t AmebaSetAmplifierEnPin(struct AudioHwControl *control, uint32_t amp_pin)
{
	(void) control;
	return ameba_audio_ctl_set_amp_pin(ameba_audio_get_ctl(), amp_pin);
}

static int32_t AmebaGetAmplifierEnPin(struct AudioHwControl *control)
{
	(void) control;
	return ameba_audio_ctl_get_amp_pin(ameba_audio_get_ctl());
}

static int32_t AmebaSetPlaybackDevice(struct AudioHwControl *control, uint32_t device_category)
{
	(void) control;
	return ameba_audio_ctl_set_device_category(ameba_audio_get_ctl(), device_category);
}

static int32_t AmebaGetPlaybackDevice(struct AudioHwControl *control)
{
	(void) control;
	return ameba_audio_ctl_get_device_category(ameba_audio_get_ctl());
}

static int32_t AmebaSetChannelMicCategory(struct AudioHwControl *control, uint32_t channel_num, uint32_t mic_category)
{
	(void) control;
	return ameba_audio_ctl_set_mic_category_for_adc(ameba_audio_get_ctl(), channel_num, mic_category);
}

static int32_t AmebaGetChannelMicCategory(struct AudioHwControl *control, uint32_t channel_num)
{
	(void) control;
	return ameba_audio_ctl_get_mic_category_for_adc(ameba_audio_get_ctl(), channel_num);
}

static int32_t AmebaSetCaptureChannelVolume(struct AudioHwControl *control, uint32_t channel_num, uint32_t volume)
{
	(void) control;
	return ameba_audio_ctl_set_adc_volume(ameba_audio_get_ctl(), channel_num, volume);
}

static int32_t AmebaSetCaptureVolume(struct AudioHwControl *control, uint32_t channels, uint32_t volume)
{
	(void) control;
	return ameba_audio_ctl_set_tdm_adc_volume(ameba_audio_get_ctl(), channels, volume);
}

static int32_t AmebaSetMicBstGain(struct AudioHwControl *control, uint32_t mic_category, uint32_t gain)
{
	(void) control;
	return ameba_audio_ctl_set_mic_bst_gain(ameba_audio_get_ctl(), mic_category, gain);
}

static int32_t AmebaSetMicUsage(struct AudioHwControl *control, uint32_t mic_usage)
{
	(void) control;
	return ameba_audio_ctl_set_mic_usage(ameba_audio_get_ctl(), mic_usage);
}

static int32_t AmebaGetMicUsage(struct AudioHwControl *control)
{
	(void) control;
	return ameba_audio_ctl_get_mic_usage(ameba_audio_get_ctl());
}

static int32_t AmebaAdjustPLLClock(struct AudioHwControl *control, uint32_t rate, uint32_t ppm, uint32_t action)
{
	(void) control;
	return ameba_audio_ctl_pll_clock_tune(ameba_audio_get_ctl(), rate, ppm, action);
}

__attribute__((constructor)) static void hw_ctl_instance_lock_init(void)
{
	HAL_AUDIO_INFO("create hw tcl single instance.");
	rtw_mutex_init(&s_hw_ctl_instance_lock);
}

__attribute__((destructor)) static void hw_ctl_instance_lock_free(void)
{
	HAL_AUDIO_INFO("destroy hw tcl single instance.");
	rtw_mutex_free(&s_hw_ctl_instance_lock);
}

struct AudioHwControl *GetAudioHwControl(void)
{
	rtw_mutex_get(&s_hw_ctl_instance_lock);

	if (s_hw_ctl_instance == NULL) {
		s_hw_ctl_instance = (struct AudioHwControl *)rtw_zmalloc(sizeof(struct AudioHwControl));
		if (!s_hw_ctl_instance) {
			return NULL;
		}

		s_hw_ctl_instance->SetHardwareVolume = AmebaSetHardwareVolume;
		s_hw_ctl_instance->SetAmplifierEnPin = AmebaSetAmplifierEnPin;
		s_hw_ctl_instance->GetAmplifierEnPin = AmebaGetAmplifierEnPin;
		s_hw_ctl_instance->SetPlaybackDevice = AmebaSetPlaybackDevice;
		s_hw_ctl_instance->GetPlaybackDevice = AmebaGetPlaybackDevice;
		s_hw_ctl_instance->SetChannelMicCategory = AmebaSetChannelMicCategory;
		s_hw_ctl_instance->GetChannelMicCategory = AmebaGetChannelMicCategory;
		s_hw_ctl_instance->SetCaptureChannelVolume = AmebaSetCaptureChannelVolume;
		s_hw_ctl_instance->SetCaptureVolume = AmebaSetCaptureVolume;
		s_hw_ctl_instance->SetMicBstGain = AmebaSetMicBstGain;
		s_hw_ctl_instance->SetMicUsage = AmebaSetMicUsage;
		s_hw_ctl_instance->GetMicUsage = AmebaGetMicUsage;
		s_hw_ctl_instance->AdjustPLLClock = AmebaAdjustPLLClock;
	}

	rtw_mutex_put(&s_hw_ctl_instance_lock);
	return s_hw_ctl_instance;
}

void DestroyAudioHwControl(struct AudioHwControl *control)
{
	if (control != NULL) {
		HAL_AUDIO_VERBOSE("free control");
		rtw_free(control);
	}
}