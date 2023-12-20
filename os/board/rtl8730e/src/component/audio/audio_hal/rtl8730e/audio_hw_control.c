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
volatile uint32_t g_hw_ctl_create = 0;

static int32_t AmebaSetHardwareVolume(struct AudioHwControl *control, float left_volume, float right_volume)
{
	(void) control;
	return ameba_audio_ctl_set_tx_volume(ameba_audio_get_ctl(), left_volume, right_volume);
}

static int32_t AmebaGetHardwareVolume(struct AudioHwControl *control, float *left_volume, float *right_volume)
{
	(void) control;
	return ameba_audio_ctl_get_tx_volume(ameba_audio_get_ctl(), left_volume, right_volume);
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

static int32_t AmebaSetAmplifierMute(struct AudioHwControl *control, bool mute)
{
	(void) control;
	return ameba_audio_ctl_set_amp_state(ameba_audio_get_ctl(), !mute);
}

static bool AmebaGetAmplifierMute(struct AudioHwControl *control)
{
	(void) control;
	return !ameba_audio_ctl_get_amp_state(ameba_audio_get_ctl());
}

static int32_t AmebaSetPlaybackMute(struct AudioHwControl *control, bool mute)
{
	(void) control;
	return ameba_audio_ctl_set_tx_mute(ameba_audio_get_ctl(), mute, true, true);
}

static bool AmebaGetPlaybackMute(struct AudioHwControl *control)
{
	(void) control;
	return ameba_audio_ctl_get_tx_mute(ameba_audio_get_ctl());
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

static int32_t AmebaGetCaptureChannelVolume(struct AudioHwControl *control, uint32_t channel_num)
{
	(void) control;
	return ameba_audio_ctl_get_adc_volume(ameba_audio_get_ctl(), channel_num);
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

static int32_t AmebaGetMicBstGain(struct AudioHwControl *control, uint32_t mic_category)
{
	(void) control;
	return ameba_audio_ctl_get_mic_bst_gain(ameba_audio_get_ctl(), mic_category);
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

static float AmebaAdjustPLLClock(struct AudioHwControl *control, uint32_t rate, float ppm, uint32_t action)
{
	(void) control;
	return ameba_audio_ctl_pll_clock_tune(ameba_audio_get_ctl(), rate, ppm, action);
}

static int32_t AmebaSetRecordMute(struct AudioHwControl *control, uint32_t channel, bool mute)
{
	(void) control;
	return ameba_audio_ctl_set_adc_mute(ameba_audio_get_ctl(), channel, mute);
}

static bool AmebaGetRecordMute(struct AudioHwControl *control, uint32_t channel)
{
	(void) control;
	return ameba_audio_ctl_get_adc_mute(ameba_audio_get_ctl(), channel);
}

struct AudioHwControl *GetAudioHwControl(void)
{

	uint32_t expected_value = 0;
	uint32_t new_value = 1;

	if (AudioHALAtomicCompareAddSwap(&g_hw_ctl_create, &expected_value, &new_value)) {
		if (s_hw_ctl_instance == NULL) {
			s_hw_ctl_instance = (struct AudioHwControl *)rtw_zmalloc(sizeof(struct AudioHwControl));
			if (!s_hw_ctl_instance) {
				return NULL;
			}

			s_hw_ctl_instance->SetHardwareVolume = AmebaSetHardwareVolume;
			s_hw_ctl_instance->GetHardwareVolume = AmebaGetHardwareVolume;
			s_hw_ctl_instance->SetAmplifierEnPin = AmebaSetAmplifierEnPin;
			s_hw_ctl_instance->GetAmplifierEnPin = AmebaGetAmplifierEnPin;
			s_hw_ctl_instance->SetAmplifierMute = AmebaSetAmplifierMute;
			s_hw_ctl_instance->GetAmplifierMute = AmebaGetAmplifierMute;
			s_hw_ctl_instance->SetPlaybackMute = AmebaSetPlaybackMute;
			s_hw_ctl_instance->GetPlaybackMute = AmebaGetPlaybackMute;
			s_hw_ctl_instance->SetPlaybackDevice = AmebaSetPlaybackDevice;
			s_hw_ctl_instance->GetPlaybackDevice = AmebaGetPlaybackDevice;
			s_hw_ctl_instance->SetChannelMicCategory = AmebaSetChannelMicCategory;
			s_hw_ctl_instance->GetChannelMicCategory = AmebaGetChannelMicCategory;
			s_hw_ctl_instance->SetCaptureChannelVolume = AmebaSetCaptureChannelVolume;
			s_hw_ctl_instance->GetCaptureChannelVolume = AmebaGetCaptureChannelVolume;
			s_hw_ctl_instance->SetCaptureVolume = AmebaSetCaptureVolume;
			s_hw_ctl_instance->SetMicBstGain = AmebaSetMicBstGain;
			s_hw_ctl_instance->GetMicBstGain = AmebaGetMicBstGain;
			s_hw_ctl_instance->SetMicUsage = AmebaSetMicUsage;
			s_hw_ctl_instance->GetMicUsage = AmebaGetMicUsage;
			s_hw_ctl_instance->AdjustPLLClock = AmebaAdjustPLLClock;
			s_hw_ctl_instance->SetRecordMute = AmebaSetRecordMute;
			s_hw_ctl_instance->GetRecordMute = AmebaGetRecordMute;
		}
	}

	return s_hw_ctl_instance;
}

void DestroyAudioHwControl(struct AudioHwControl *control)
{
	uint32_t expected_value = 1;
	uint32_t new_value = 0;

	if (AudioHALAtomicCompareAddSwap(&g_hw_ctl_create, &expected_value, &new_value)) {
		ameba_audio_destroy_ctl();
		if (control != NULL) {
			rtw_free(control);
		}
	}
}