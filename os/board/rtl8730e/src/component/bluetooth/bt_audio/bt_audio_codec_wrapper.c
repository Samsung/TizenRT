/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/
#include <bt_api_config.h>
#include <stdio.h>
#include <osif.h>
#include <bt_audio_codec_wrapper.h>
#include <bt_audio_debug.h>
#include <rtk_bt_common.h>
#include <sbc_codec_entity.h>
#include <aac_codec_entity.h>
#include <lc3_codec_entity.h>
#include <cvsd_codec_entity.h>

/* bt_audio_handle_media_data_packet should be invoked before decode */
uint16_t bt_audio_handle_media_data_packet(PAUDIO_CODEC_ENTITY pentity, 
										uint8_t *packet, 
										uint16_t size, 
										uint32_t *pframe_size, 
										uint8_t *pframe_num, 
										uint8_t *pcodec_header_flag, struct audio_param *paudio_param)
{
	if (!pentity) {
		printf("[BT_AUDIO] Find match codec entity fail \r\n");
		return RTK_BT_AUDIO_FAIL;
	}
	if (!pentity->active_flag) {
		printf("[BT_AUDIO] codec entity is not active \r\n");
		return RTK_BT_AUDIO_FAIL;
	}
	osif_mutex_take(pentity->mutex,0);
	if (pentity->bt_audio_handle_media_data_packet((void *)pentity, packet, size, pframe_size, pframe_num, pcodec_header_flag, paudio_param)) {
		printf("[BT_AUDIO] param media data packet fail ! \r\n");
		osif_mutex_give(pentity->mutex);
		return RTK_BT_AUDIO_FAIL;
	}
	osif_mutex_give(pentity->mutex);

	return RTK_BT_AUDIO_OK;
}

uint16_t bt_audio_decode_process_data(PAUDIO_CODEC_ENTITY pentity, 
									struct dec_codec_buffer *pdecoder_buffer, 
									uint8_t *data, 
									uint32_t size,
									uint32_t *ppcm_size, struct audio_param *paudio_param)
{
	if (!pentity) {
		printf("[BT_AUDIO] Find match codec entity fail \r\n");
		return RTK_BT_AUDIO_FAIL;
	}
	if (!pentity->active_flag) {
		printf("[BT_AUDIO] codec entity is not active \r\n");
		return RTK_BT_AUDIO_FAIL;
	}
	osif_mutex_take(pentity->mutex,0);
	if (pentity->decoding_func((void *)pentity, data, size, pdecoder_buffer, ppcm_size, paudio_param)) {
		printf("[BT_AUDIO] decode fail ! \r\n");
		osif_mutex_give(pentity->mutex);
		return RTK_BT_AUDIO_FAIL;
	}
	osif_mutex_give(pentity->mutex);

	return RTK_BT_AUDIO_OK;
}

struct dec_codec_buffer *bt_audio_get_decode_buffer(PAUDIO_CODEC_ENTITY pentity)
{
	struct dec_codec_buffer *pbuffer = NULL;

	if (!pentity) {
		printf("[BT_AUDIO] Find match codec entity fail \r\n");
		return NULL;
	}
	if (!pentity->active_flag) {
		printf("[BT_AUDIO] codec entity is not active \r\n");
		return NULL;
	}
	osif_mutex_take(pentity->mutex,0);
	pbuffer = pentity->get_decode_buffer((void *)pentity);
	if (!pbuffer) {
		printf("[BT_AUDIO] get decode buffer fail ! \r\n");
		osif_mutex_give(pentity->mutex);
		return NULL;
	}
	pentity->decode_lock = true;
	osif_mutex_give(pentity->mutex);

	return pbuffer;
}

uint16_t bt_audio_free_decode_buffer(PAUDIO_CODEC_ENTITY pentity, struct dec_codec_buffer *buffer)
{
	if (!pentity) {
		printf("[BT_AUDIO] Find match codec entity fail \r\n");
		return RTK_BT_AUDIO_FAIL;
	}
	osif_mutex_take(pentity->mutex,0);
	pentity->free_decode_buffer((void *)pentity, buffer);
	pentity->decode_lock = false;
	osif_mutex_give(pentity->mutex);

	return RTK_BT_AUDIO_OK;
}

uint16_t bt_audio_encode_process_data(PAUDIO_CODEC_ENTITY pentity, 
									struct enc_codec_buffer *pencoder_buffer, 
									int16_t *data, 
									uint32_t size,
									uint8_t *p_frame_num,
									uint32_t *p_actual_len)
{
	if (!pentity) {
		printf("[BT_AUDIO] Find match codec entity fail \r\n");
		return RTK_BT_AUDIO_FAIL;
	}
	if (!pentity->active_flag) {
		printf("[BT_AUDIO] codec entity is not active \r\n");
		return RTK_BT_AUDIO_FAIL;
	}
	osif_mutex_take(pentity->mutex,0);
	if (pentity->encoding_func((void *)pentity, data, size, pencoder_buffer, p_frame_num, p_actual_len)) {
		printf("[BT_AUDIO] decode fail ! \r\n");
		osif_mutex_give(pentity->mutex);
		return RTK_BT_AUDIO_FAIL;
	}
	osif_mutex_give(pentity->mutex);

	return RTK_BT_AUDIO_OK;
}

struct enc_codec_buffer *bt_audio_get_encode_buffer(PAUDIO_CODEC_ENTITY pentity)
{
	struct enc_codec_buffer *pbuffer = NULL;

	if (!pentity) {
		printf("[BT_AUDIO] Find match codec entity fail \r\n");
		return NULL;
	}
	if (!pentity->active_flag) {
		printf("[BT_AUDIO] codec entity is not active \r\n");
		return NULL;
	}
	osif_mutex_take(pentity->mutex,0);
	pbuffer = pentity->get_encode_buffer((void *)pentity);
	if (pbuffer == NULL) {
		printf("[BT_AUDIO] get encode buffer fail ! \r\n");
		osif_mutex_give(pentity->mutex);
		return NULL;
	}
	pentity->encode_lock = true;
	osif_mutex_give(pentity->mutex);

	return pbuffer;
}

uint16_t bt_audio_free_encode_buffer(PAUDIO_CODEC_ENTITY pentity, struct enc_codec_buffer *buffer)
{
	if (!pentity) {
		printf("[BT_AUDIO] Find match codec entity fail \r\n");
		return RTK_BT_AUDIO_FAIL;
	}
	osif_mutex_take(pentity->mutex,0);
	pentity->free_encode_buffer((void *)pentity, buffer);
	pentity->encode_lock = false;
	osif_mutex_give(pentity->mutex);

	return RTK_BT_AUDIO_OK;
}

uint16_t bt_codec_decoder_num_samples_per_frame(PAUDIO_CODEC_ENTITY pentity)
{
	int sample_num_per_frame = 0;

	if (!pentity) {
		printf("[BT_AUDIO] Find match codec entity fail \r\n");
		return RTK_BT_AUDIO_GET_VALUE_FAIL;
	}
	if (!pentity->active_flag) {
		printf("[BT_AUDIO] codec entity is not active \r\n");
		return RTK_BT_AUDIO_FAIL;
	}
	osif_mutex_take(pentity->mutex,0);
	sample_num_per_frame = pentity->decoder_num_samples_per_frame((void *)pentity);
	osif_mutex_give(pentity->mutex);

	return sample_num_per_frame;
}

uint16_t bt_codec_decoder_num_channels(PAUDIO_CODEC_ENTITY pentity)
{
	int channels_num = 0;

	if (!pentity) {
		printf("[BT_AUDIO] Find match codec entity fail \r\n");
		return RTK_BT_AUDIO_GET_VALUE_FAIL;
	}
	if (!pentity->active_flag) {
		printf("[BT_AUDIO] codec entity is not active \r\n");
		return RTK_BT_AUDIO_FAIL;
	}
	osif_mutex_take(pentity->mutex,0);
	channels_num = pentity->decoder_num_channels((void *)pentity);
	osif_mutex_give(pentity->mutex);

	return channels_num;
}

int bt_codec_decoder_sample_rate(PAUDIO_CODEC_ENTITY pentity)
{
	int decoder_sample_rate = 0;

	if (!pentity) {
		printf("[BT_AUDIO] Find match codec entity fail \r\n");
		return RTK_BT_AUDIO_GET_VALUE_FAIL;
	}
	if (!pentity->active_flag) {
		printf("[BT_AUDIO] codec entity is not active \r\n");
		return RTK_BT_AUDIO_FAIL;
	}
	osif_mutex_take(pentity->mutex,0);
	decoder_sample_rate = pentity->decoder_sample_rate((void *)pentity);
	osif_mutex_give(pentity->mutex);

	return decoder_sample_rate;
}

uint16_t bt_audio_register_codec(uint32_t type, void* param, PAUDIO_CODEC_ENTITY pentity)
{
	uint16_t err = RTK_BT_AUDIO_FAIL;

	if (!pentity) {
		printf("[BT_AUDIO] pentity is empty \r\n");
		return err;
	}
	switch (type) {
#if defined(CONFIG_BT_AUDIO_CODEC_SBC) && CONFIG_BT_AUDIO_CODEC_SBC
	case RTK_BT_AUDIO_CODEC_SBC:
		rtk_bt_audio_sbc_register(RTK_BT_AUDIO_CODEC_SBC, pentity);
		if (pentity->init(pentity, param)) {
			printf("[BT_AUDIO] init sbc codec fail \r\n");
			break;
		} else {
			if (false == osif_mutex_create(&pentity->mutex)) {
				printf("[BT AUDIO] mutex create fail \r\n");
				break;
			}
			pentity->active_flag = 1;
			err = RTK_BT_AUDIO_OK;
		}
		break;
#endif
#if defined(CONFIG_BT_AUDIO_CODEC_AAC) && CONFIG_BT_AUDIO_CODEC_AAC
	case RTK_BT_AUDIO_CODEC_AAC:
		// rtk_bt_audio_aac_register(RTK_BT_AUDIO_CODEC_AAC, pentity);
		// if (pentity->init(pentity, param)) {
		// 	printf("[BT_AUDIO] init aac codec fail \r\n");
		// 	break;
		// } else {
		// if (false == osif_mutex_create(&pentity->mutex)) {
		// 	printf("[BT AUDIO] mutex create fail \r\n");
		// 	break;
		// }
		// 	pentity->active_flag = 1;
		// 	err = RTK_BT_AUDIO_OK;
		// }
		break;
#endif
#if defined(CONFIG_BT_AUDIO_CODEC_LC3) && CONFIG_BT_AUDIO_CODEC_LC3
	case RTK_BT_AUDIO_CODEC_LC3:
		rtk_bt_audio_lc3_register(RTK_BT_AUDIO_CODEC_LC3, pentity);
		if (pentity->init(pentity, param)) {
			printf("[BT_AUDIO] init lc3 codec fail \r\n");
			break;
		} else {
			if (false == osif_mutex_create(&pentity->mutex)) {
				printf("[BT AUDIO] mutex create fail \r\n");
				break;
			}
			pentity->active_flag = 1;
			err = RTK_BT_AUDIO_OK;
		}
		break;
#endif
#if defined(CONFIG_BT_AUDIO_CODEC_CVSD) && CONFIG_BT_AUDIO_CODEC_CVSD
	case RTK_BT_AUDIO_CODEC_CVSD:
		rtk_bt_audio_cvsd_register(RTK_BT_AUDIO_CODEC_CVSD, pentity);
		if (pentity->init(pentity, param)) {
			printf("[BT_AUDIO] init cvsd codec fail \r\n");
			break;
		} else {
			if (false == osif_mutex_create(&pentity->mutex)) {
				printf("[BT AUDIO] mutex create fail \r\n");
				break;
			}
			pentity->active_flag = 1;
			err = RTK_BT_AUDIO_OK;
		}
		break;
#endif	
	default:
		printf("[BT_AUDIO] unknown codec type \r\n");
		break;
	}

	return err;
}

uint16_t bt_audio_unregister_codec(uint32_t type, PAUDIO_CODEC_ENTITY pentity)
{
	(void)type;
	if (!pentity) {
		printf("[BT_AUDIO] pentity is empty \r\n");
		return RTK_BT_AUDIO_FAIL;
	}
	if (!pentity->active_flag) {
		printf("[BT_AUDIO] codec entity is not active \r\n");
		return RTK_BT_AUDIO_FAIL;
	}
	pentity->active_flag = 0;
	/* wait for decode buffer and encode buffer free */
	while (pentity->decode_lock || pentity->encode_lock) {
		osif_delay(5);
	}
	osif_mutex_take(pentity->mutex,0);
	pentity->deinit(pentity);
	osif_mutex_give(pentity->mutex);
	osif_mutex_delete(pentity->mutex);

	return RTK_BT_AUDIO_OK;
}