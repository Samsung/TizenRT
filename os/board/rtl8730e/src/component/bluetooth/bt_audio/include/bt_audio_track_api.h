/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/
#ifndef __BT_AUDIO_TRACK_API_H__
#define __BT_AUDIO_TRACK_API_H__

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @fn void *rtk_bt_audio_track_init(uint32_t channels, 
                                uint32_t rate, 
                                uint32_t bits, 
                                uint32_t period_size, 
                                uint32_t period_count)
 * @brief                       bt audio init
 * @param[in]                   channels: audio channel
 * @param[in]                   rate: audio rate
 * @param[in]                   bits: audio bits
 * @param[in]                   period_size: period size
 * @param[in]                   period_count: period count
 * @return    
 *                              - NULL  : Fail
 *                              - track_hdl: Success
 */
void *rtk_bt_audio_track_init(uint32_t channels, 
                               uint32_t rate, 
                               uint32_t bits, 
                               uint32_t period_size, 
                               uint32_t period_count);

/**
 * @fn        uint16_t rtk_bt_audio_track_deinit(void *track_hdl)
 * @brief     bt audio track deinit
.* @param[in] track_hdl: audio track handle
 * @return    
 *                              - 0  : Succeed
 *                              - 1: Fail
 */
uint16_t rtk_bt_audio_track_deinit(void *track_hdl);

/**
 * @fn        uint16_t rtk_bt_audio_track_stop(void *track_hdl)
 * @brief     bt audio track stop
 * @param[in] track_hdl: audio track handle
 * @return    
 *                              - 0  : Succeed
 *                              - 1: Fail
 */
uint16_t rtk_bt_audio_track_stop(void *track_hdl);

/**
 * @fn        uint16_t rtk_bt_audio_track_pause(void *track_hdl)
 * @brief     bt audio track pause
 * @param[in] track_hdl: audio track handle
 * @return    
 *                              - 0  : Succeed
 *                              - 1: Fail
 */
uint16_t rtk_bt_audio_track_pause(void *track_hdl);

/**
 * @fn        uint16_t rtk_bt_audio_track_resume(void *track_hdl)
 * @brief     bt audio track resume
 * @param[in] track_hdl: audio track handle
 * @return    
 *                              - 0  : Succeed
 *                              - 1: Fail
 */
uint16_t rtk_bt_audio_track_resume(void *track_hdl);

/**
 * @fn        uint16_t rtk_bt_audio_track_play(void *track_hdl, void *buffer, uint16_t size)
 * @brief     bt audio track playback
 * @param[in] track_hdl: audio track handle
 * @param[in] buffer: pointer of audio data
 * @param[in] size: size of audio data
 * @return    
 *                              - 0  : Succeed
 *                              - 1: Fail
 */
uint16_t rtk_bt_audio_track_play(void *track_hdl, void *buffer, uint16_t size);

/**
 * @fn        int16_t rtk_bt_audio_track_set_volume(float left_volume, float right_volume)
 * @brief     set audio track volume
 * @param[in] left_volume: left audio volume
 * @param[in] right_volume: right audio volume
 * @return    
 *            - 0  : Succeed
 *            - others: Error code
 */
uint16_t rtk_bt_audio_track_set_volume(float left_volume, float right_volume);

/**
 * @fn        float rtk_bt_audio_track_get_volume(void)
 * @brief     get audio track volume (Not ready)
 * @return    - audio volume
 */
float rtk_bt_audio_track_get_volume(void);

/**
 * @fn        uint16_t rtk_bt_audio_track_start(void *track_hdl, float left_volume, float right_volume)
 * @brief     audio track start
 * @param[in] track_hdl: audio track handle
 * @param[in] left_volume: left volume value
 * @param[in] right_volume: right volume value
 * @return    
 *                              - 0  : Succeed
 *                              - 1: Fail
 */
uint16_t rtk_bt_audio_track_start(void *track_hdl, float left_volume, float right_volume);

/**
 * @fn        void rtk_bt_audio_track_set_mute(uint8_t muted)
 * @brief     audio track configure mute(Not ready)
 * @param[in] muted: mute flag
 * @return    none
 */
void rtk_bt_audio_track_set_mute(uint8_t muted);

/**
 * @fn        uint8_t rtk_bt_audio_track_get_muted(void)
 * @brief     audio track get mute(Not ready)
 * @return    mute status
 */
uint8_t rtk_bt_audio_track_get_muted(void);

/**
 * @fn        uint16_t rtk_bt_audio_track_set_sample_rate(void *track_hdl, uint32_t sample_rate)
 * @brief     audio track set sample rate
 * @param[in] track_hdl: audio track handle
 * @param[in] sample_rate: sample rate
 * @return    
 *                              - 0  : Succeed
 *                              - 1: Fail
 */
uint16_t rtk_bt_audio_track_set_sample_rate(void *track_hdl, uint32_t sample_rate);

/**
 * @fn        uint32_t rtk_bt_audio_track_get_sample_rate(void *track_hdl)
 * @brief     audio track get sample rate
 * @param[in] track_hdl: audio track handle
 * @return    
 *                              - 0  : Fail
 *                              - Others: Samplerate
 */
uint32_t rtk_bt_audio_track_get_sample_rate(void *track_hdl);

/**
 * @fn        uint16_t rtk_bt_audio_track_set_channel_count(void *track_hdl, uint32_t channel_count)
 * @brief     audio track set channel count
 * @param[in] track_hdl: audio track handle
 * @param[in] channel_count: channel count
 * @return    
 *                              - 0  : Succeed
 *                              - 1: Fail
 */
uint16_t rtk_bt_audio_track_set_channel_count(void *track_hdl, uint32_t channel_count);

/**
 * @fn        uint32_t rtk_bt_audio_track_get_channel_count(void *track_hdl)
 * @brief     audio track get channel count
 * @param[in] track_hdl: audio track handle
 * @return    
 *                              - 0  : Fail
 *                              - Others: Channel count
 */
uint32_t rtk_bt_audio_track_get_channel_count(void *track_hdl);

/**
 * @fn        long rtk_bt_audio_track_get_buffer_size(void)
 * @brief     audio track get buffer size(Not ready)
 * @return    buffer size
 */
long rtk_bt_audio_track_get_buffer_size(void);

#endif // __BT_AUDIO_TRACK_API_H__
