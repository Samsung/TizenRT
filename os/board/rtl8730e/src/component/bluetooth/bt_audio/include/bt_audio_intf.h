/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

#ifndef __BT_AUDIO_INTF_H__
#define __BT_AUDIO_INTF_H__

#include <dlist.h>
#include <basic_types.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @typedef   rtk_bt_audio_role_t
 * @brief     audio role 
 */
typedef enum {
	RTK_BT_AUDIO_ROLE_TRACK = 0x00,						/*!< Audio Track */
	RTK_BT_AUDIO_ROLE_RECORD = 0x01,					/*!< Audio Record */
} rtk_bt_audio_role_t;

/**
 * @typedef	rtk_bt_audio_codec_conf_t
 * @brief	structure of default bt audio configuration
 */
typedef struct {
	uint32_t				codec_index;			/*!< audio codec profile support @ref rtk_bt_audio_codec_t */
	void					*param;					/*!< audio parameter */
} rtk_bt_audio_codec_conf_t;

/**
 * @typedef	rtk_bt_audio_track_t
 * @brief	structure of audio track configuration
 */
typedef struct {
	struct list_head		list;					/*!< list head */
	void					*audio_track_hdl;		/*!< RTAudioTrack pointer */
	uint32_t				channels;				/*!< indicate channels */
	uint32_t				channel_allocation;		/*!< indicate mono or dual mode(1 left channel, 2 right channel, 3 dual mode) */
	uint32_t				rate;					/*!< sample rate */
	uint32_t				bits;					/*!< bit per sample */
	uint32_t				track_num;				/*!< indicate sub index of the same codec */
} rtk_bt_audio_track_t;

/**
 * @typedef	rtk_bt_audio_record_t
 * @brief	structure of audio record configuration
 */
typedef struct {
	struct list_head		list;					/*!< list head */
	void					*audio_record_hdl;		/*!< RTAudioRecord pointer */
	uint32_t				channels;				/*!< indicate channels */
	uint32_t				rate;					/*!< sample rate */
	uint32_t				record_num;				/*!< indicate sub index of the same codec */
} rtk_bt_audio_record_t;

/**  @brief audio stream message definition */
typedef struct {
	uint32_t				type;					/*!< indicate application type */
	rtk_bt_audio_track_t	*track;					/*!< indicate audio track */
	// rtk_bt_audio_record_t	*record;				/*!< indicate audio record */
	void					*entity;				/*!< indicate audio codec entity */
	uint8_t					*data;					/*!< indicate audio stream buffer */
	uint16_t				size;					/*!< indicate audio stream length */
} T_AUDIO_STREAM_MSG;

/**
 * @fn        struct enc_codec_buffer *rtk_bt_audio_data_encode(uint32_t type, void *entity, int16_t *pdata, uint32_t len)
 * @brief     encode pcm data
 * @param[in] type: audio data codec type
 * @param[in] entity: pointer of codec entity
 * @param[in] pdata: pointer of audio data
 * @param[in] len: data length(In Bytes)
 * @return    encoded struct buffer
 */
struct enc_codec_buffer *rtk_bt_audio_data_encode(uint32_t type, void *entity, int16_t *pdata, uint32_t len);

/**
 * @fn        uint16_t rtk_bt_audio_free_encode_buffer(uint32_t type, void *entity, struct enc_codec_buffer *pbuffer)
 * @brief     free encode buffer
 * @param[in] type: audio data codec type
 * @param[in] entity: pointer of codec entity
 * @param[in] pbuffer: encoded struct buffer
 * @return    
 *            - 0  : Succeed
 *            - others: Error code
 */
uint16_t rtk_bt_audio_free_encode_buffer(uint32_t type, void *entity, struct enc_codec_buffer *pbuffer);

/**
 * @fn        uint16_t rtk_bt_audio_decode_data(void *entity, void *pparam, uint8_t *data, uint32_t length, uint32_t *ppcm_data_size, void *paudio_param)
 * @brief     decode data
 * @param[in] entity: pointer of codec entity
 * @param[in] pparam: pointer of param
 * @param[in] data: data
 * @param[in] length: data length
 * @param[in] ppcm_data_size: decoded return pacm data length
 * @param[in] paudio_param: audio parameter
 * @return    
 *            - 0  : Succeed
 *            - others: Error code
 */
uint16_t rtk_bt_audio_decode_data(void *entity, void *pparam, uint8_t *data, uint32_t length, uint32_t *ppcm_data_size, void *paudio_param);

/**
 * @fn        int rtk_bt_audio_record_data_get(uint32_t type, rtk_bt_audio_record_t *record, void *buffer, int size, bool blocking)
 * @brief     read record data after rtk_bt_audio_record_add(RTK_BT_AUDIO_ROLE_RECORD)
 * @param[in] type: audio data codec type
 * @param[in] record: audio record handle
 * @param[in] buffer is the dst buffer of application.
 * @param[in] size is the dst buffer data bytes.
 * @param[in] blocking choice whether to block when read stuck, suggest:true.
 * @return    none
 */
int rtk_bt_audio_record_data_get(uint32_t type, rtk_bt_audio_record_t *record, void *buffer, int size, bool blocking);

/**
 * @fn        rtk_bt_audio_track_t *rtk_bt_audio_track_add(uint32_t type, float left_volume, float right_volume, uint32_t channels, uint32_t rate, uint32_t bits)
 * @brief     add one audio track(Audio mix support multiple stream/ Passthrough only one)
 * @param[in] type: audio data codec type
 * @param[in] left_volume: left init volume
 * @param[in] right_volume: right init volume
 * @param[in] channels: channels
 * @param[in] rate: sample rate
 * @param[in] bits: bits
 * @return    
 *            - NULL  : Fail
 *            - others: Track Handle
 */
rtk_bt_audio_track_t *rtk_bt_audio_track_add(uint32_t type, float left_volume, float right_volume, uint32_t channels, uint32_t rate, uint32_t bits);

/**
 * @fn        rtk_bt_audio_record_t *rtk_bt_audio_record_add(uint32_t type, uint32_t channels, uint32_t rate)
 * @brief     add one audio record(Audio mix support multiple stream/ Passthrough only one)
 * @param[in] type: audio data codec type
 * @param[in] channels: channels
 * @param[in] rate: sample rate
 * @return    
 *            - NULL  : Fail
 *            - others: Record Handle
 */
rtk_bt_audio_record_t *rtk_bt_audio_record_add(uint32_t type, uint32_t channels, uint32_t rate);

/**
 * @fn        uint16_t rtk_bt_audio_track_del(uint32_t type, rtk_bt_audio_track_t *ptrack)
 * @brief     delete one audio track
 * @param[in] type: audio data codec type
 * @param[in] ptrack: track handle
 * @return    
 *            - 1  : Fail
 *            - 0: Success
 */
uint16_t rtk_bt_audio_track_del(uint32_t type, rtk_bt_audio_track_t *ptrack);

/**
 * @fn        uint16_t rtk_bt_audio_track_del_all(void)
 * @brief     delete all audio track
 * @return    
 *            - 1  : Fail
 *            - 0: Success
 */
uint16_t rtk_bt_audio_track_del_all(void);

/**
 * @fn        uint16_t rtk_bt_audio_record_del(uint32_t type, rtk_bt_audio_record_t *precord)
 * @brief     delete one audio record
 * @param[in] type: audio data codec type
 * @param[in] precord: record handle
 * @return    
 *            - 1  : Fail
 *            - 0: Success
 */
uint16_t rtk_bt_audio_record_del(uint32_t type, rtk_bt_audio_record_t *precord);

/**
 * @fn        uint16_t rtk_bt_audio_record_del_all(void)
 * @brief     delete all audio record
 * @return    
 *            - 1  : Fail
 *            - 0: Success
 */
uint16_t rtk_bt_audio_record_del_all(void);

/**
 * @fn        void *rtk_bt_audio_codec_add(rtk_bt_audio_codec_conf_t *paudio_codec_conf)
 * @brief     add one audio codec
 * @param[in] paudio_codec_conf: audio codec configuration
 * @return    
 *            - NULL  : Fail
 *            - others: Codec entity
 */
void *rtk_bt_audio_codec_add(rtk_bt_audio_codec_conf_t *paudio_codec_conf);

/**
 * @fn        uint16_t rtk_bt_audio_codec_remove(uint32_t type, void *pentity)
 * @brief     remove one audio codec
 * @param[in] type: audio data codec type
 * @param[in] pentity: audio codec entity
 * @return    
 *            - NULL  : Fail
 *            - others: Codec entity
 */
uint16_t rtk_bt_audio_codec_remove(uint32_t type, void *pentity);

/**
 * @fn        uint16_t rtk_bt_audio_codec_remove_all(void)
 * @brief     remove all audio codec
 * @return    
 *            - 1  : Fail
 *            - 0: Success
 */
uint16_t rtk_bt_audio_codec_remove_all(void);

/**
 * @fn        uint16_t rtk_bt_audio_recvd_data_in(uint32_t type, rtk_bt_audio_track_t *track, void *entity, uint8_t *pdata, uint32_t len)
 * @brief     enqueue received audio data
 * @param[in] type: audio data codec type
 * @param[in] track: track handle
 * @param[in] entity: entity handle
 * @param[in] pdata: pointer of audio data
 * @param[in] len: data length
 * @return    
 *            - 0  : Succeed
 *            - others: Error code
 */
uint16_t rtk_bt_audio_recvd_data_in(uint32_t type, rtk_bt_audio_track_t *track, void *entity, uint8_t *pdata, uint32_t len);

/**
 * @fn        uint16_t rtk_bt_audio_init(void)
 * @brief     bt audio framework init
 * @return    
 *            - 0  : Succeed
 *            - others: Error code
 */
uint16_t rtk_bt_audio_init(void);

/**
 * @fn        uint16_t rtk_bt_audio_deinit(void)
 * @brief     bt audio deinit
 * @return    
 *            - 0  : Succeed
 *            - others: Error code
 */
uint16_t rtk_bt_audio_deinit(void);

#ifdef __cplusplus
}
#endif

#endif /* __BT_AUDIO_INTF_H__ */
