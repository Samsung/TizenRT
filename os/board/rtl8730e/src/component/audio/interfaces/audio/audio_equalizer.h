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

#ifndef AMEBA_COMPONENT_AUDIO_INTERFACES_AUDIO_AUDIO_EQUALIZER_H
#define AMEBA_COMPONENT_AUDIO_INTERFACES_AUDIO_AUDIO_EQUALIZER_H

#include "audio/audio_type.h"
#include "osal_c/osal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct RTAudioEqualizer;
struct RTEffectModule;

/**
 * @brief Create RTAudioEqualizer instance.
 * @return Returns the instance pointer of RTAudioEqualizer.
 * @since 1.0
 * @version 1.0
 */
struct RTAudioEqualizer *RTAudioEqualizer_Create(void);

/**
 * @brief Release RTAudioEqualizer.
 * @param equalizer is the pointer of struct RTAudioEqualizer.
 * @since 1.0
 * @version 1.0
 */
void RTAudioEqualizer_Destroy(struct RTAudioEqualizer *equalizer);

/**
 * @brief Set RTEffectModule to equalizer.Only works in passthrough mode.
 * @param module is the pointer of struct RTEffectModule.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | the operation is invalid.
 * OSAL_ERR_INVALID_PARAM | the params are invalid.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTAudioEqualizer_SetModule(struct RTAudioEqualizer *equalizer, struct RTEffectModule *module);

/**
 * @brief Init RTAudioEqualizer.
 * @param equalizer is the pointer of struct RTAudioEqualizer.
 * @param priority designed for future use, now please set 0.
 * @param session designed for future use, now please set 0.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | the operation is invalid.
 * OSAL_ERR_INVALID_PARAM | the params are invalid.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTAudioEqualizer_Init(struct RTAudioEqualizer *equalizer, int32_t priority, int32_t session);

/**
 * @brief Set RTAudioEqualizer enable or disable.
 * @param equalizer is the pointer of struct RTAudioEqualizer.
 * @param enabled is the state of struct RTAudioEqualizer, true means enable, false means disable.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | the operation is invalid.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTAudioEqualizer_SetEnabled(struct RTAudioEqualizer *equalizer, bool enabled);

/**
 * @brief Get number of bands RTAudioEqualizer supports.
 * @param equalizer is the pointer of struct RTAudioEqualizer.
 * @param bands is the total bands.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | the operation is invalid.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTAudioEqualizer_SetNumberOfBands(struct RTAudioEqualizer *equalizer, uint32_t bands);

/**
 * @brief Get number of bands RTAudioEqualizer supports.
 * @param equalizer is the pointer of struct RTAudioEqualizer.
 * @return Returns the number of bands supported by the RTAudioEqualizer framework.
 * @since 1.0
 * @version 1.0
 */
int16_t RTAudioEqualizer_GetNumberOfBands(struct RTAudioEqualizer *equalizer);

/**
 * @brief Get band level range RTAudioEqualizer supports.
 * @param equalizer is the pointer of struct RTAudioEqualizer.
 * @return Returns band level range supported by the RTAudioEqualizer framework.The return value
 * contains two int16_t integer, which is decibel * 100, for example (-1500, 1500), means (-15db, 15db).
 * @since 1.0
 * @version 1.0
 */
int16_t *RTAudioEqualizer_GetBandLevelRange(struct RTAudioEqualizer *equalizer);

/**
 * @brief Set RTAudioEqualizer band level.
 * @param equalizer is the pointer of struct RTAudioEqualizer.
 * @param band is the band number, for example, RTAudioEqualizer total supports 5 bands, which band level to set?
 * The band value range is [0, BAND_TATAL_NUM).BAND_TATAL_NUM is got from RTAudioEqualizer_GetNumberOfBands.
 * @param level is the level of band to set. The level value should be delta decibel * 100, and in the range framework
 * supports.See the {@link RTAudioEqualizer_GetBandLevelRange} for information
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | the operation is invalid.
 * OSAL_ERR_INVALID_PARAM | the params are invalid.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTAudioEqualizer_SetBandLevel(struct RTAudioEqualizer *equalizer, uint32_t band, uint32_t level);

/**
 * @brief Get RTAudioEqualizer band level.
 * @param equalizer is the pointer of struct RTAudioEqualizer.
 * @param band is the band number, for example, RTAudioEqualizer total supports 5 bands, which band level to get?
 * The band value range is [0, BAND_TATAL_NUM).BAND_TATAL_NUM is got from RTAudioEqualizer_GetNumberOfBands.
 * @return Returns a value of delta decibel * 100, and in the range framework supports.See the {@link
 * RTAudioEqualizer_GetBandLevelRange} for information
 * @since 1.0
 * @version 1.0
 */
int16_t RTAudioEqualizer_GetBandLevel(struct RTAudioEqualizer *equalizer, uint32_t band);

/**
 * @brief Set RTAudioEqualizer center frequency.
 * @param equalizer is the pointer of struct RTAudioEqualizer.
 * @param band is the band number.
 * @param freq is the new center frequency set to band in hz.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | the operation is invalid.
 * OSAL_ERR_INVALID_PARAM | the params are invalid.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTAudioEqualizer_SetCenterFreq(struct RTAudioEqualizer *equalizer, uint32_t band, uint32_t freq);

/**
 * @brief Get RTAudioEqualizer center frequency.
 * @param equalizer is the pointer of struct RTAudioEqualizer.
 * @param band is the band number, for example, RTAudioEqualizer total supports 5 bands, which band frequency to get?
 * The band value range is [0, BAND_TATAL_NUM).BAND_TATAL_NUM is got from RTAudioEqualizer_GetNumberOfBands.
 * @return Returns a value of center frequency in hz.
 * @since 1.0
 * @version 1.0
 */
int32_t RTAudioEqualizer_GetCenterFreq(struct RTAudioEqualizer *equalizer, uint32_t band);

/**
 * @brief Get RTAudioEqualizer frequency's band.
 * @param equalizer is the pointer of struct RTAudioEqualizer.
 * @param frequency is the frequency value.
 * @return Returns a value of band.
 * @since 1.0
 * @version 1.0
 */
int16_t RTAudioEqualizer_GetBand(struct RTAudioEqualizer *equalizer, uint32_t frequency);

/**
 * @brief Set RTAudioEqualizer Q factor.
 * @param equalizer is the pointer of struct RTAudioEqualizer.
 * @param band is the band number.
 * @param qfactor is the qfactor set to band.
 * @return Returns a value listed below: \n
 * rt_status_t | Description
 * ----------------------| -----------------------
 * OSAL_OK | the operation is successful.
 * OSAL_ERR_INVALID_OPERATION | the operation is invalid.
 * OSAL_ERR_INVALID_PARAM | the params are invalid.
 * @since 1.0
 * @version 1.0
 */
rt_status_t RTAudioEqualizer_SetQfactor(struct RTAudioEqualizer *equalizer, uint32_t band, uint32_t qfactor);

/**
 * @brief Get RTAudioEqualizer Q factor.
 * @param equalizer is the pointer of struct RTAudioEqualizer.
 * @param band is the band number.
 * @return Returns qfactor of the band.
 * @since 1.0
 * @version 1.0
 */
int16_t RTAudioEqualizer_GetQfactor(struct RTAudioEqualizer *equalizer, uint32_t band);

#ifdef __cplusplus
}
#endif

#endif