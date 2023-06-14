/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

#include <stdio.h>
#include <string.h>
#include <osif.h>
#include <rtk_bt_common.h>
#include <cvsd_codec_entity.h>
#include <bt_audio_debug.h>

static rtk_bt_cvsd_codec_t cvsd_codec_t = {0};

static uint16_t cvsd_codec_init(void *p_entity, void *param)
{
	(void)p_entity;
	rtk_bt_cvsd_codec_t *pcvsd_codec_t = (rtk_bt_cvsd_codec_t *)param;

	if (!pcvsd_codec_t) {
		printf("%s : No rtk_bt_cvsd_codec_t configure \r\n", __func__);
		return 1;
	}

	memcpy((void *)&cvsd_codec_t, (void *)pcvsd_codec_t, sizeof(rtk_bt_cvsd_codec_t));

	return 0;
}

static uint16_t cvsd_codec_deinit(void *p_entity)
{
	(void)p_entity;

	memset((void *)&cvsd_codec_t, 0, sizeof(rtk_bt_cvsd_codec_t));

	return 0;
}

static uint16_t cvsd_encoder_process_data(void *p_entity, int16_t *data, uint32_t size, struct enc_codec_buffer *pencoder_buffer, uint8_t *p_frame_num, uint32_t *p_actual_len)
{
	(void)p_entity;

	if (size > CVSD_FRAME_SIZE / 2) {
		printf("%s : inpute size is large than 256 \r\n", __func__);
		return 1;
	}
	pencoder_buffer->pbuffer = (uint8_t *)data;
	pencoder_buffer->frame_num = 1;
	pencoder_buffer->frame_size = size * 2;
	*p_frame_num = 1;
	*p_actual_len = size * 2;

	return 0;
}

static uint16_t cvsd_decoder_process_data(void *p_entity, uint8_t *data, uint32_t size, struct dec_codec_buffer *decode_buffer, uint32_t *ppcm_size, struct audio_param *paudio_param)
{
	(void)p_entity;
	(void)paudio_param;

	if (size > CVSD_FRAME_SIZE) {
		printf("%s : inpute size is large than 256 \r\n", __func__);
		return 1;
	}
	decode_buffer->pbuffer = (int16_t *)data;
	*ppcm_size = size;

	return 0;
}

static uint16_t cvsd_audio_handle_media_data_packet(void *p_entity, uint8_t *packet, uint16_t size, uint32_t *pframe_size, uint8_t *pframe_num, uint8_t *pcodec_header_flag, struct audio_param *paudio_param)
{
	(void)p_entity;
	(void)packet;

	*pframe_size = size;
	*pframe_num = 1;
	*pcodec_header_flag = 0;
	paudio_param->channels = cvsd_codec_t.decoder_t.channel_num;
	paudio_param->channel_allocation = 1;
	paudio_param->rate = cvsd_codec_t.decoder_t.sample_rate;;
	paudio_param->bits = 0;

	return 0;
}

static struct dec_codec_buffer *cvsd_decoder_buffer_get(void *p_entity)
{
	(void)p_entity;
	struct dec_codec_buffer *pdecoder_buffer = NULL;

	pdecoder_buffer = (struct dec_codec_buffer *)osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(struct dec_codec_buffer));
	if (pdecoder_buffer == NULL) {
		printf("%s: allocate pdecoder_buffer fail \r\n", __func__);
		return NULL;
	}
	pdecoder_buffer->pbuffer = NULL;
	pdecoder_buffer->total_size = (uint32_t)CVSD_FRAME_SIZE;
	pdecoder_buffer->actual_write_size = 0;

	return pdecoder_buffer;
}

static void cvsd_free_decode_buffer(void *p_entity, struct dec_codec_buffer *pdecoder_buffer)
{
	(void)p_entity;

	/* free codec buffer */
	osif_mem_free((void *)pdecoder_buffer);
}

static struct enc_codec_buffer *cvsd_encoder_buffer_get(void *p_entity)
{
	(void)p_entity;
	struct enc_codec_buffer *pencoder_buffer = NULL;

	pencoder_buffer = (struct enc_codec_buffer *)osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(struct enc_codec_buffer));
	if (pencoder_buffer == NULL) {
		printf("%s: allocate pencoder_buffer fail \r\n", __func__);
		return NULL;
	}
	pencoder_buffer->pbuffer = NULL;
	pencoder_buffer->frame_num = 0;

	return pencoder_buffer;
}

static void cvsd_free_encode_buffer(void *p_entity, struct enc_codec_buffer *pencodec_buffer)
{
	(void)p_entity;

	osif_mem_free((void *)pencodec_buffer);
}

static int cvsd_decoder_num_samples_per_frame(void *p_entity)
{
	(void)p_entity;

	return CVSD_FRAME_SIZE;
}

/* only support mono */
static int cvsd_decoder_num_channels(void *p_entity)
{
	(void)p_entity;

	return 1;
}

/* bt cvsd is configured at 8K sample rate */
static int cvsd_decoder_sample_rate(void *p_entity)
{
	(void)p_entity;

	return 8000;
}

uint16_t rtk_bt_audio_cvsd_register(uint32_t type, PAUDIO_CODEC_ENTITY p_entity)
{
	uint16_t ret = 1;
	uint32_t lock_flag;

	DBG_BAD("%s:Enter \r\n", __func__);
	if (p_entity == NULL) {
		printf("%s:NULL entity pointer \r\n", __func__);
		return ret;
	}
	lock_flag = osif_lock();
	p_entity->type = type;
	p_entity->stream_in_num = 0;
	p_entity->init = cvsd_codec_init;
	p_entity->deinit = cvsd_codec_deinit;
	p_entity->bt_audio_handle_media_data_packet = cvsd_audio_handle_media_data_packet;
	p_entity->encoding_func = cvsd_encoder_process_data;
	p_entity->decoding_func = cvsd_decoder_process_data;
	p_entity->get_decode_buffer = cvsd_decoder_buffer_get;
	p_entity->free_decode_buffer = cvsd_free_decode_buffer;
	p_entity->get_encode_buffer = cvsd_encoder_buffer_get;
	p_entity->free_encode_buffer = cvsd_free_encode_buffer;
	p_entity->decoder_num_samples_per_frame = cvsd_decoder_num_samples_per_frame;
	p_entity->decoder_num_channels = cvsd_decoder_num_channels;
	p_entity->decoder_sample_rate = cvsd_decoder_sample_rate;
	p_entity->encode_lock = false;
	p_entity->decode_lock = false;
	memset((void *)&p_entity->cvsd, 0, sizeof(p_entity->cvsd));
	osif_unlock(lock_flag);
	ret = 0;

	return ret;
}