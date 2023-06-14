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
#ifndef AMEBA_FWK_MEDIA_AUDIO_AUDIOLITE_INTERFACES_AUDIO_AUDIO_CONTROL_H
#define AMEBA_FWK_MEDIA_AUDIO_AUDIOLITE_INTERFACES_AUDIO_AUDIO_CONTROL_H

#include "osal_c/osal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Defines all the audio playback devices.
 *
 * @since 1.0
 * @version 1.0
 */
enum {
	/** play through speaker */
	RTAUDIO_DEVICE_SPEAKER         = 0,
	/** play through headphone*/
	RTAUDIO_DEVICE_HEADPHONE       = 1,
	RTAUDIO_DEVICE_MAX_NUM         = 2,
};

/**
 * @brief Defines all the audio microphone categories.
 *
 * @since 1.0
 * @version 1.0
 */
enum {
	/* [amebalite] support AMIC1-AMIC3, DMIC1-DMIC4
	 * [amebasmart] support AMIC1-AMIC5, DMIC1-DMIC8
	 */
	RTAUDIO_AMIC1         = 0,
	RTAUDIO_AMIC2         = 1,
	RTAUDIO_AMIC3         = 2,
	RTAUDIO_AMIC4         = 3,
	RTAUDIO_AMIC5         = 4,
	RTAUDIO_DMIC1         = 5,
	RTAUDIO_DMIC2         = 6,
	RTAUDIO_DMIC3         = 7,
	RTAUDIO_DMIC4         = 8,
	RTAUDIO_DMIC5         = 9,
	RTAUDIO_DMIC6         = 10,
	RTAUDIO_DMIC7         = 11,
	RTAUDIO_DMIC8         = 12,
	RTAUDIO_MIC_MAX_NUM   = 13,
};

/**
 * @brief Defines the audio capture main usages.
 *
 * @since 1.0
 * @version 1.0
 */
enum {
	RTAUDIO_CAPTURE_USAGE_AMIC         = 0,
	RTAUDIO_CAPTURE_USAGE_DMIC         = 1,
	RTAUDIO_CAPTURE_USAGE_DMIC_REF_AMIC = 2,
	RTAUDIO_CAPTURE_USAGE_LINE_IN      = 3,
	RTAUDIO_CAPTURE_USAGE_MAX_NUM      = 4,
};

/**
 * @brief Defines all the audio mic bst gain values.
 *
 * @since 1.0
 * @version 1.0
 */
enum {
	RTAUDIO_MICBST_GAIN_0DB          = 0,
	RTAUDIO_MICBST_GAIN_5DB          = 1,
	RTAUDIO_MICBST_GAIN_10DB         = 2,
	RTAUDIO_MICBST_GAIN_15DB         = 3,
	RTAUDIO_MICBST_GAIN_20DB         = 4,
	RTAUDIO_MICBST_GAIN_25DB         = 5,
	RTAUDIO_MICBST_GAIN_30DB         = 6,
	RTAUDIO_MICBST_GAIN_35DB         = 7,
	RTAUDIO_MICBST_GAIN_40DB         = 8,
	RTAUDIO_MICBST_GAIN_MAX_NUM      = 9,
};

/**
 * @brief Defines all the audio PLL clock tune options.
 *
 * @since 1.0
 * @version 1.0
 */
enum {
	RTAUDIO_PLL_AUTO    = 0,
	RTAUDIO_PLL_FASTER  = 1,
	RTAUDIO_PLL_SLOWER  = 2,
};

/**
 * @brief Set Hardware Volume of audio dac.
 *
 * @param left_volume volume of left channel, 0.0-1.0.
 * @param right_volume volume of right channel, 0.0-1.0.
 * @return Returns a value listed below: \n
 * int32_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | param not supported.
 * @since 1.0
 * @version 1.0
 */
int32_t RTAudioControl_SetHardwareVolume(float left_volume, float right_volume);

/**
 * @brief Set Amplifier En Pin.
 *
 * @param amp_pin the pin of the amplifier en, you can get your pin value from:
 * fwlib/include/ameba_pinmux.h, for example, if your pin is _PB_7, it's value is "#define _PB_7 (0x27)"
 * which is 39 of uint32_t.So the amp_pin here should set 39.
 * @return Returns a value listed below: \n
 * int32_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | param not supported.
 * @since 1.0
 * @version 1.0
 */
int32_t RTAudioControl_SetAmplifierEnPin(uint32_t amp_pin);

/**
 * @brief Set Amplifier En Pin.
 *
 * @return Returns amp_pin the pin of the amplifier en, value is from:
 *         fwlib/include/ameba_pinmux.h, for example, if your pin is _PB_7, it's value is "#define _PB_7 (0x27)"
 *         which is 39 of uint32_t.
 *         If the value < 0, means get fail.
 * @since 1.0
 * @version 1.0
 */
int32_t RTAudioControl_GetAmplifierEnPin(void);

/**
 * @brief Set Playback Device.
 *
 * @param device_category the device of playback, maybe RTAUDIO_DEVICE_SPEAKER or RTAUDIO_DEVICE_HEADPHONE.
 * @return Returns a value listed below: \n
 * int32_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | param not supported.
 * @since 1.0
 * @version 1.0
 */
int32_t RTAudioControl_SetPlaybackDevice(uint32_t device_category);

/**
 * @brief Get Playback Device.
 *
 * @return Returns the device of playback, maybe RTAUDIO_DEVICE_SPEAKER or RTAUDIO_DEVICE_HEADPHONE.
 *         If the value < 0, means get fail.
 * @since 1.0
 * @version 1.0
 */
int32_t RTAudioControl_GetPlaybackDevice(void);

/**
 * @brief Set Capture Mic type for channel.this function should be called after RTAudioRecord_Start
 *        and before RTAudioRecord_Read, otherwise it will not work.
 *
 * @param channel_num the channel number to set mic type.
 * @param mic_category the mic type, can be RTAUDIO_AMIC1...RTAUDIO_DMIC8.
 *       [amebalite] support AMIC1-AMIC3, DMIC1-DMIC4
 *       [amebasmart] support AMIC1-AMIC5, DMIC1-DMIC8
 * @return Returns a value listed below: \n
 * int32_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | param not supported.
 * @since 1.0
 * @version 1.0
 */
int32_t RTAudioControl_SetChannelMicCategory(uint32_t channel_num, uint32_t mic_category);

/**
 * @brief Get Mic Category.
 * @param channel_num the channel number to get mic type.
 * @return Returns the mic type, can be RTAUDIO_AMIC1...RTAUDIO_DMIC8.
 *         [amebalite] support AMIC1-AMIC3, DMIC1-DMIC4
 *         [amebasmart] support AMIC1-AMIC5, DMIC1-DMIC8
 *         If the value < 0, means get fail.
 * @since 1.0
 * @version 1.0
 */
int32_t RTAudioControl_GetChannelMicCategory(uint32_t channel_num);

/**
 * @brief Set Capture volume for channel.this function should be called after RTAudioRecord_Start
 *        and before RTAudioRecord_Read, otherwise it will not work.
 *
 * @param channel_num the channel number to set volume.
 * @param volume the value of volume, can be 0x00-0xaf.
  *          This parameter can be -17.625dB~48dB in 0.375dB step
  *          0x00 -17.625dB
  *          0xaf 48dB
 * @return Returns a value listed below: \n
 * int32_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | param not supported.
 * @since 1.0
 * @version 1.0
 */
int32_t RTAudioControl_SetCaptureChannelVolume(uint32_t channel_num, uint32_t volume);

/**
 * @brief Set Capture volume for channel.this function should be called after RTAudioRecord_Start
 *        and before RTAudioRecord_Read, otherwise it will not work.
 *
 * @param channels the total channels number to set volume, also the channels to capture.
 * @param volume the value of volume, can be 0x00-0xaf.
  *          This parameter can be -17.625dB~48dB in 0.375dB step
  *          0x00 -17.625dB
  *          0xaf 48dB
 * @return Returns a value listed below: \n
 * int32_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | param not supported.
 * @since 1.0
 * @version 1.0
 */
int32_t RTAudioControl_SetCaptureVolume(uint32_t channels, uint32_t volume);

/**
 * @brief Set Micbst Gain.this function should be called after RTAudioRecord_Start
 *        and before RTAudioRecord_Read, otherwise it will not work.
 *
 * @param mic_category the mic type, can be RTAUDIO_AMIC1...RTAUDIO_DMIC8.
 *       [amebalite] support AMIC1-AMIC3, DMIC1-DMIC4
 *       [amebasmart] support AMIC1-AMIC5, DMIC1-DMIC8
 * @param gain can be RTAUDIO_MICBST_GAIN_0DB-RTAUDIO_MICBST_GAIN_40DB
 * @return Returns a value listed below: \n
 * int32_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | param not supported.
 * @since 1.0
 * @version 1.0
 */
int32_t RTAudioControl_SetMicBstGain(uint32_t mic_category, uint32_t gain);

/**
 * @brief Set mic usage.
 *
 * @param mic_usage RTAUDIO_CAPTURE_USAGE_AMIC, or RTAUDIO_CAPTURE_USAGE_DMIC, if user's
 *        microphone data is recorded from amic, then set RTAUDIO_CAPTURE_USAGE_AMIC, if
 *        user's microphone data is recorded from dmic, then set RTAUDIO_CAPTURE_USAGE_DMIC.
 *        The reference data is always recorded from amic, even if the usage is
 *        RTAUDIO_CAPTURE_USAGE_DMIC, the default setting is RTAUDIO_CAPTURE_USAGE_AMIC.
 * @return Returns a value listed below: \n
 * int32_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | param not supported.
 * @since 1.0
 * @version 1.0
 */
int32_t RTAudioControl_SetMicUsage(uint32_t mic_usage);

/**
 * @brief Get mic usage.
 *
 * @param Returns RTAUDIO_CAPTURE_USAGE_AMIC, or RTAUDIO_CAPTURE_USAGE_DMIC, if user's
 *        microphone data is recorded from amic, then set RTAUDIO_CAPTURE_USAGE_AMIC, if
 *        user's microphone data is recorded from dmic, then set RTAUDIO_CAPTURE_USAGE_DMIC.
 *        If mic data is from dmic, the reference data is recorded from amic, then set
 *        RTAUDIO_CAPTURE_USAGE_DMIC_REF_AMIC, the default setting is RTAUDIO_CAPTURE_USAGE_AMIC.
 *        if the return value<0, then get fail.
 * @since 1.0
 * @version 1.0
 */
int32_t RTAudioControl_GetMicUsage(void);

/**
 * @brief Adjust PLL clk .
 *
 * @param rate sample rate of current stream
 * @param ppm ~1.55ppm per FOF step
 * @param action can be RTAUDIO_PLL_AUTO RTAUDIO_PLL_FASTER RTAUDIO_PLL_SLOWER
 * @since 1.0
 * @version 1.0
 */
int32_t RTAudioControl_AdjustPLLClock(uint32_t rate, uint32_t ppm, uint32_t action);

#ifdef __cplusplus
}
#endif


#endif