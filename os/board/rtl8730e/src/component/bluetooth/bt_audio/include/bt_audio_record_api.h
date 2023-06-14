/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/
#ifndef __BT_AUDIO_RECORD_API_H__
#define __BT_AUDIO_RECORD_API_H__

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define RTK_BT_AUDIO_MICBST_GAIN_0DB		((u32)0x00000000)
#define RTK_BT_AUDIO_MICBST_GAIN_5DB		((u32)0x00000001)
#define RTK_BT_AUDIO_MICBST_GAIN_10DB		((u32)0x00000002)
#define RTK_BT_AUDIO_MICBST_GAIN_15DB		((u32)0x00000003)
#define RTK_BT_AUDIO_MICBST_GAIN_20DB		((u32)0x00000004)
#define RTK_BT_AUDIO_MICBST_GAIN_25DB		((u32)0x00000005)
#define RTK_BT_AUDIO_MICBST_GAIN_30DB		((u32)0x00000006)
#define RTK_BT_AUDIO_MICBST_GAIN_35DB		((u32)0x00000007)
#define RTK_BT_AUDIO_MICBST_GAIN_40DB		((u32)0x00000008)

/**
 * @typedef   rtk_bt_audio_mic_t
 * @brief     mic configure 
 */
typedef enum {
	/* [amebalite] support AMIC1-AMIC3, DMIC1-DMIC4
	 * [amebasmart] support AMIC1-AMIC5, DMIC1-DMIC8
	 */
	RTK_BT_AUDIO_AMIC1         = 0,
	RTK_BT_AUDIO_AMIC2         = 1,
	RTK_BT_AUDIO_AMIC3         = 2,
	RTK_BT_AUDIO_AMIC4         = 3,
	RTK_BT_AUDIO_AMIC5         = 4,
	RTK_BT_AUDIO_DMIC1         = 5,
	RTK_BT_AUDIO_DMIC2         = 6,
	RTK_BT_AUDIO_DMIC3         = 7,
	RTK_BT_AUDIO_DMIC4         = 8,
	RTK_BT_AUDIO_DMIC5         = 9,
	RTK_BT_AUDIO_DMIC6         = 10,
	RTK_BT_AUDIO_DMIC7         = 11,
	RTK_BT_AUDIO_DMIC8         = 12,
	RTK_BT_AUDIO_MIC_MAX_NUM   = 13,
} rtk_bt_audio_mic_t;

/**
 * @fn void *rtk_bt_audio_record_init(uint32_t channels, uint32_t rate)
 * @brief                       bt audio init
 * @param[in]                   channels: audio channel
 * @param[in]                   rate: audio rate
 * @return    
 *                              - NULL  : Fail
 *                              - record_hdl: Success
 */
void *rtk_bt_audio_record_init(uint32_t channels, uint32_t rate);

/**
 * @fn        uint16_t rtk_bt_audio_record_deinit(void *record_hdl)
 * @brief     bt audio record deinit
 * @param[in] record_hdl: audio record handle
 * @return    none
 */
uint16_t rtk_bt_audio_record_deinit(void *record_hdl);

/**
 * @fn        uint16_t rtk_bt_audio_record_stop(void *record_hdl)
 * @brief     bt audio record stop
 * @param[in] record_hdl: audio record handle
 *                              - 0  : Succeed
 *                              - 1: Fail
 */
uint16_t rtk_bt_audio_record_stop(void *record_hdl);

/**
 * @fn        uint16_t rtk_bt_audio_record_start(void *record_hdl)
 * @brief     bt audio record start
 * @param[in] record_hdl: audio record handle
 * @return    
 *                              - 0  : Succeed
 *                              - 1: Fail
 */
uint16_t rtk_bt_audio_record_start(void *record_hdl);

/**
 * @fn        uint16_t rtk_bt_audio_record_set_sample_rate(void *record_hdl, uint32_t sample_rate)
 * @brief     bt audio set record sample rate
 * @param[in] record_hdl: audio record handle
 * @param[in] sample_rate: sample rate
 * @return    
 *                              - 0  : Succeed
 *                              - 1: Fail
 */
uint16_t rtk_bt_audio_record_set_sample_rate(void *record_hdl, uint32_t sample_rate);

/**
 * @fn        uint32_t rtk_bt_audio_record_get_sample_rate(void *record_hdl)
 * @brief     bt audio get record sample rate
 * @param[in] record_hdl: audio record handle
 * @return    sample rate
 * @return    
 *                              - 0  : Fail
 *                              - Others: Sample rate
 */
uint32_t rtk_bt_audio_record_get_sample_rate(void *record_hdl);

/**
 * @fn        uint16_t rtk_bt_audio_record_set_channel_count(void *record_hdl, uint32_t channel_count)
 * @brief     bt audio set record channel count
 * @param[in] record_hdl: audio record handle
 * @param[in] channel_count: channel count
 * @return    
 *                              - 0  : Succeed
 *                              - 1: Fail
 */
uint16_t rtk_bt_audio_record_set_channel_count(void *record_hdl, uint32_t channel_count);

/**
 * @fn        uint32_t rtk_bt_audio_record_get_channel_count(void *record_hdl)
 * @brief     bt audio get record channel count
 * @param[in] record_hdl: audio record handle
 * @return    
 *                              - 0  : Fail
 *                              - Others: Channel count
 */
uint32_t rtk_bt_audio_record_get_channel_count(void *record_hdl);

/**
 * @fn        uint16_t rtk_bt_audio_record_set_parameters(void *record_hdl, const char *strs)
 * @brief     bt audio set record parameters
 * @param[in] record_hdl: audio record handle
 * @param[in] strs: supports AMIC settings like:"ch0_sel_amic=1;ch1_sel_amic=2;ch2_sel_amic=3"
 * The meaning for the string: it means the channel 0 gets data from amic1.
 * channel 1 gets data from amic2, channel 2 gets data from amic3.
 * [amebalite]: 4 channels settings most.
 *              The default setting is :ch0_sel_amic=1;ch1_sel_amic=2;ch2_sel_amic=3;ch3_sel_amic=4.
 * [amebaSmart]: 8 channels settings most.
 *              The default setting is :ch0_sel_amic=1;ch1_sel_amic=2;ch2_sel_amic=3;ch3_sel_amic=4;
 *              ch4_sel_amic=4;ch5_sel_amic=4;ch6_sel_amic=4;ch7_sel_amic=4;
 * strs also supports audio capture mode settings like "cap_mode=no_afe_pure_data".
 * The meaning of the string is to choose "no_afe_pure_data" mode for audio capture.
 * [amebalite] & [amebaSmart]: There are 4 modes, all take the default above amic setting for example,
 *              the data stucture is as follows:
 *              mode(no_afe_pure_data): take RTAudioRecordConfig's channel_count 3 for example:mic1+mic2+mic3;
 *                                     for lite demo borad, mic3 is ref data as the hardware design.
 *              mode(do_afe_only_out): take RTAudioRecordConfig's channel_count 1 for example:out;
 *                                     out means the mic data after denoise.
 *              mode(no_afe_all_data): take RTAudioRecordConfig's channel_count 4 for example:mic1+mic2+mic3+out;
 *                                     for lite demo borad, mic3 is ref data as the hardware design.
 *                                     out is NULL, and for the app to set.
 *              mode(do_afe_all_data): take RTAudioRecordConfig's channel_count 4 for example:mic1+mic2+mic3+out;
 *                                     for lite demo borad, mic3 is ref data as the hardware design.
 *                                     out means the mic data after denoise.
 * If default setting meets your requirements, no need to set again.
 * strs alsa supports to set the channel of reference like:"ref_channel=2"
 * The meaning of the string: it means channel2 is the reference channel.
 * @return    
 *                              - 0  : Succeed
 *                              - 1  : Fail
 */
uint16_t rtk_bt_audio_record_set_parameters(void *record_hdl, const char *strs);

/**
 * @fn        uint16_t rtk_bt_audio_record_set_capture_volume(uint32_t channels, uint32_t volume)
 * @brief     Set Capture volume for channel.this function should be called after rtk_bt_audio_record_init
 * @param[in] channels: the total channels number to set volume, also the channels to capture.
 * @param[in] volume: the value of volume, can be 0x00-0xaf.
 * @return    
 *                              - 0  : Succeed
 *                              - others: Error code
 */
uint16_t rtk_bt_audio_record_set_capture_volume(uint32_t channels, uint32_t volume);

/**
 * @fn        uint16_t rtk_bt_audio_record_set_channel_mic_category(uint32_t channel_num, uint32_t mic_category)
 * @brief     Set Capture Mic type for channel.this function should be called after rtk_bt_audio_record_init
 * @param[in] channel_num: the channel number to set mic type.
 * @param[in] mic_category: the mic type, can be RTAUDIO_AMIC1...RTAUDIO_DMIC8.
 *           [amebalite] support AMIC1-AMIC3, DMIC1-DMIC4
 *           [amebasmart] support AMIC1-AMIC5, DMIC1-DMIC8
 * @return    
 *                              - 0  : Succeed
 *                              - others: Error code
 */
uint16_t rtk_bt_audio_record_set_channel_mic_category(uint32_t channel_num, uint32_t mic_category);

/**
 * @fn        uint16_t rtk_bt_audio_record_set_mic_bst_gain(uint32_t mic_category, uint32_t gain)
 * @brief     Set Micbst Gain.this function should be called after rtk_bt_audio_record_init
 * @param[in] mic_category: the mic type, can be RTAUDIO_AMIC1...RTAUDIO_DMIC8.
 *           [amebalite] support AMIC1-AMIC3, DMIC1-DMIC4
 *           [amebasmart] support AMIC1-AMIC5, DMIC1-DMIC8
 * @param[in] gain.can be RTAUDIO_MICBST_GAIN_0DB-RTAUDIO_MICBST_GAIN_40DB
 * @return    
 *                              - 0  : Succeed
 *                              - others: Error code
 */
uint16_t rtk_bt_audio_record_set_mic_bst_gain(uint32_t mic_category, uint32_t gain);

/**
 * @fn        int rtk_bt_audio_record_read(void *record_hdl, void *buffer, int size, bool blocking)
 * @brief     read record data after rtk_bt_audio_record_init
 * @param[in] record_hdl: audio record handle
 * @param[in] buffer is the dst buffer of application.
 * @param[in] size is the dst buffer data bytes.
 * @param[in] blocking choice whether to block when read stuck, suggest:true.
 * @return    read size
 */
int rtk_bt_audio_record_read(void *record_hdl, void *buffer, int size, bool blocking);

#endif // __BT_AUDIO_RECORD_API_H__
