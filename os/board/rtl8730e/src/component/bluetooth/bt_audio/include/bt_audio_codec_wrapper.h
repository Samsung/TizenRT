/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/
#ifndef __BT_AUDIO_CODEC_WRAPPER_H__
#define __BT_AUDIO_CODEC_WRAPPER_H__

#include <dlist.h>

struct enc_codec_buffer {
	struct list_head			list;
	uint8_t						*pbuffer;
	uint8_t						frame_num;
	uint32_t					frame_size;
};

struct audio_param {
	uint32_t					channels;
	uint32_t					channel_allocation;
	uint32_t					rate;
	uint32_t					bits; 
};

struct dec_codec_buffer {
	struct list_head			list;
	uint32_t					type;
	int16_t						*pbuffer;
	uint32_t					total_size;
	uint32_t					actual_write_size;
	struct audio_param			audio_param_t;
};

/** @brief application priv obj struct*/
struct audio_codec_entity_priv {
	struct list_head			list;
	uint8_t						active_flag;
	void						*mutex;
	bool						encode_lock;
	bool						decode_lock;
	uint32_t					type;
	int							stream_in_num;     //!< indicate application enqueue data/command num.
	union {
		struct {
			uint8_t 	sampling_frequency;				/*!< indicate sample frequence */
			uint8_t 	channel_mode;					/*!< indicate channel mode */
			uint8_t 	block_length;					/*!< indicate block length */
			uint8_t 	subbands;						/*!< indicate subbands */
			uint8_t 	allocation_method;				/*!< indicate allocation method */
			uint8_t 	min_bitpool;					/*!< indicate min bitpool */
			uint8_t 	max_bitpool;					/*!< indicate max bitpool */
		} sbc;
		struct {
			uint8_t 	object_type;					/*!< indicate object type */
			uint16_t 	sampling_frequency;				/*!< indicate sample frequence */
			uint8_t 	channel_number;					/*!< indicate channel number */
			bool 		vbr_supported;					/*!< indicate vbr_supported */
			uint32_t 	bit_rate;						/*!< indicate bit rate */
		} aac;
		struct {
			uint8_t 	channel_mode;					/*!< indicate channel mode */
			int 		sample_rate;					/*!< indicate sample frequence */
			uint8_t 	bitpool;						/*!< indicate min bitpool */
			uint8_t 	allocation_method;				/*!< indicate allocation method */
			uint8_t 	subbands;						/*!< indicate subbands */
			uint8_t 	block_length;					/*!< indicate block length */
		} msbc;
		struct {
			uint8_t 	channel_num;
			int 		sample_rate;
			uint8_t 	frame_duration;
		} cvsd;
		struct {
			uint16_t 	type_exist;
			uint8_t 	frame_duration;
			int 		sample_rate;
			uint8_t 	codec_frame_blocks_per_sdu;
			uint16_t 	octets_per_codec_frame;
			uint32_t 	audio_channel_allocation;
			uint32_t 	channels;
			int 		compress_bytes;
			uint32_t 	presentation_delay;
			void 		*p_lc3_t;
		} lc3;
		struct {
			uint8_t 	info[12];						/*!< vendor codec data */
		} vendor;
	};
	uint16_t					(*init)(void *pentity, void *param);
	uint16_t					(*deinit)(void *pentity);
	uint16_t					(*bt_audio_handle_media_data_packet)(void *pentity, uint8_t *packet, uint16_t size, uint32_t *pframe_size, uint8_t *pframe_num, uint8_t *pcodec_header_flag, struct audio_param *paudio_param);
	uint16_t					(*encoding_func)(void *pentity, int16_t *data, uint32_t size, struct enc_codec_buffer *pencoder_buffer,uint8_t *p_frame_num, uint32_t *p_actual_len);
	uint16_t					(*decoding_func)(void *pentity, uint8_t *data, uint32_t size, struct dec_codec_buffer *pdecoder_buffer, uint32_t *ppcm_size, struct audio_param *paudio_param);
	struct dec_codec_buffer		*(*get_decode_buffer)(void *pentity);
	void						(*free_decode_buffer)(void *pentity, struct dec_codec_buffer *buffer);
	struct enc_codec_buffer		*(*get_encode_buffer)(void *pentity);
	void						(*free_encode_buffer)(void *pentity, struct enc_codec_buffer *buffer);
	int							(*decoder_num_samples_per_frame)(void *pentity);
	int							(*decoder_num_channels)(void *pentity);
	int							(*decoder_sample_rate)(void *pentity);
};

/** @brief typedef struct audio_application_entity_priv */
typedef struct audio_codec_entity_priv AUDIO_CODEC_ENTITY, *PAUDIO_CODEC_ENTITY;

/**
 * @fn uint16_t bt_audio_handle_media_data_packet(PAUDIO_CODEC_ENTITY pentity,
                                           uint8_t *packet, 
                                           uint16_t size, 
                                           uint32_t *pframe_size, 
                                           uint8_t *pframe_num, 
                                           bool *psbc_header_flag, struct audio_param *paudio_param)
 * @brief                                  bt audio handle received media data
 * @param[in]                              pentity:codec entity
 * @param[in]                              packet: pointer of data
 * @param[in]                              size: packet length
 * @param[in]                              pframe_size: resulet of frame size
 * @param[in]                              pframe_num: resulet of frame number
 * @param[in]                              pcodec_header_flag: codec header flag
 * @param[in]                              paudio_param: audio parameter
 * @return    
 *                              - 0  : Succeed
 *                              - others: Error code
 */
uint16_t bt_audio_handle_media_data_packet(PAUDIO_CODEC_ENTITY pentity,
                                           uint8_t *packet, 
                                           uint16_t size, 
                                           uint32_t *pframe_size, 
                                           uint8_t *pframe_num, 
                                           uint8_t *pcodec_header_flag, struct audio_param *paudio_param);

/**
 * @fn        uint16_t bt_audio_decode_process_data(PAUDIO_CODEC_ENTITY pentity, struct dec_codec_buffer *pdecoder_buffer, uint8_t *data, uint32_t size, uint32_t *ppcm_size, struct audio_param *paudio_param)
 * @brief     decode audio data to pcm 
 * @param[in] pentity:codec entity
 * @param[in] pdecoder_buffer: indicate the allocate memory
 * @param[in] data: poniter to the data buffer
 * @param[in] size: indicate data size
 * @param[in] ppcm_size: indicate decoded pcm data length
 * @param[in] paudio_param: audio parameter
 * @return    
 *                              - 0  : Succeed
 *                              - others: Error code
 */
uint16_t bt_audio_decode_process_data(PAUDIO_CODEC_ENTITY pentity, struct dec_codec_buffer *pdecoder_buffer, uint8_t *data, uint32_t size, uint32_t *ppcm_size, struct audio_param *paudio_param);

/**
 * @fn        struct dec_codec_buffer *bt_audio_get_decode_buffer(PAUDIO_CODEC_ENTITY pentity)
 * @brief     get decode buffer from specific application memory management
 * @param[in] pentity:codec entity
 * @return buffer: point to buffer , NULL: allocate fail
 */
struct dec_codec_buffer *bt_audio_get_decode_buffer(PAUDIO_CODEC_ENTITY pentity);

/**
 * @fn        uint16_t bt_audio_free_decode_buffer(PAUDIO_CODEC_ENTITY pentity, struct dec_codec_buffer *buffer)
 * @brief     memory free decode buffer
 * @param[in] pentity:codec entity
 * @param[in] buffer: indicate the allocate memory
 * @return    
 *                              - 0  : Succeed
 *                              - others: Error code
 */
uint16_t bt_audio_free_decode_buffer(PAUDIO_CODEC_ENTITY pentity, struct dec_codec_buffer *buffer);

/**
 * @fn        uint16_t bt_audio_encode_process_data(PAUDIO_CODEC_ENTITY pentity, struct enc_codec_buffer *pencoder_buffer, int16_t *data, uint32_t size, uint8_t *p_frame_num, uint32_t *p_actual_len)
 * @brief     encode audio data to pcm 
 * @param[in] pentity:codec entity
 * @param[in] pencoder_buffer: indicate the allocate memory
 * @param[in] data: poniter to the data buffer
 * @param[in] size: indicate data size(In bytes)
 * @param[in] p_frame_num: frame number pointer
 * @param[in] p_actual_len: encoded length
 * @return    
 *                              - 0  : Succeed
 *                              - others: Error code
 */
uint16_t bt_audio_encode_process_data(PAUDIO_CODEC_ENTITY pentity, struct enc_codec_buffer *pencoder_buffer, int16_t *data, uint32_t size, uint8_t *p_frame_num, uint32_t *p_actual_len);

/**
 * @fn        struct enc_codec_buffer *bt_audio_get_encode_buffer(PAUDIO_CODEC_ENTITY pentity)
 * @brief     get encode buffer from specific application memory management
 * @param[in] pentity:codec entity
 * @return buffer: point to buffer , NULL: allocate fail
 */
struct enc_codec_buffer *bt_audio_get_encode_buffer(PAUDIO_CODEC_ENTITY pentity);

/**
 * @fn        uint16_t uint16_t bt_audio_free_encode_buffer(PAUDIO_CODEC_ENTITY pentity, struct enc_codec_buffer *buffer)
 * @brief     memory free encode buffer
 * @param[in] pentity:codec entity
 * @param[in] buffer: indicate the allocate memory
 * @return    
 *                              - 0  : Succeed
 *                              - others: Error code
 */
uint16_t bt_audio_free_encode_buffer(PAUDIO_CODEC_ENTITY pentity, struct enc_codec_buffer *buffer);

/**
 * @fn        uint16_t bt_codec_decoder_num_samples_per_frame(PAUDIO_CODEC_ENTITY pentity)
 * @brief     get decoder num samples per frame media packet
 * @param[in] pentity:codec entity
 * @return 0xFF: fail others: number samplers
 */
uint16_t bt_codec_decoder_num_samples_per_frame(PAUDIO_CODEC_ENTITY pentity);

/**
 * @fn        uint16_t bt_codec_decoder_num_channels(PAUDIO_CODEC_ENTITY pentity)
 * @brief     get decoder channel numbers
 * @param[in] pentity:codec entity
 * @return 0xFF: fail others: number samplers
 */
uint16_t bt_codec_decoder_num_channels(PAUDIO_CODEC_ENTITY pentity);

/**
 * @fn        int bt_codec_decoder_sample_rate(PAUDIO_CODEC_ENTITY pentity)
 * @brief     get decoder sample rate
 * @param[in] pentity:codec entity
 * @return 0xFF: fail others: number samplers
 */
int bt_codec_decoder_sample_rate(PAUDIO_CODEC_ENTITY pentity);

/**
 * @fn        uint16_t bt_audio_register_codec(uint32_t type, void* param)
 * @brief     register codec
 * @param[in] type: indicate codec type
 * @param[in] param: pointer of parameter
 * @param[in] pentity:codec entity
 * @return    
 *                              - 0  : Succeed
 *                              - others: Error code
 */
uint16_t bt_audio_register_codec(uint32_t type, void* param, PAUDIO_CODEC_ENTITY pentity);

/**
 * @fn        uint16_t bt_audio_unregister_codec(uint32_t type)
 * @brief     unregister codec
 * @param[in] type: indicate codec type
 * @param[in] pentity:codec entity
 * @return    
 *                              - 0  : Succeed
 *                              - others: Error code
 */
uint16_t bt_audio_unregister_codec(uint32_t type, PAUDIO_CODEC_ENTITY pentity);

#endif // __BT_AUDIO_CODEC_WRAPPER_H__
