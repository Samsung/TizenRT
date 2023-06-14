/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/
#include <osif.h>
#include <stdio.h>
#include <string.h>
#include <rtk_bt_common.h>
#include <lc3_codec_entity.h>
#include <bt_audio_debug.h>
#include <lc3_codec_main.h>

#define BIT_PER_SAMPLE_DECODE 16
#define BIT_PER_SAMPLE_ENCODE 16

// static int byte_count_max_dec = 800;

struct lc3_pcm_data lc3_decoder_free_data[MAX_LC3_DECODER_NUM] = {0};
static struct list_head lc3_pcm_data_list;
int lc3_pcm_data_used_num;

static struct audio_sample_option le_audio_sample_rate_tab[] = {
	{0x01, 8000}, 
	{0x02, 11000}, 
	{0x03, 16000}, 
	{0x04, 22000}, 
	{0x05, 24000}, 
	{0x06, 32000}, 
	{0x07, 44100}, 
	{0x08, 48000},
	{0x09, 88000},
	{0x0A, 96000},
	{0x0B, 176000},
	{0x0C, 192000},
	{0x0D, 384000},
	{0xFF, 0}
};

static int get_sample_rate(uint8_t sample_index)
{
	uint8_t i = 0;
	int sample_rate = 0;
	/* foreach event table */
	while (1) {
		if (le_audio_sample_rate_tab[i].sample_rate_index == sample_index) {
			sample_rate = le_audio_sample_rate_tab[i].sample_rate;
			break;
		}
		if (le_audio_sample_rate_tab[i].sample_rate_index == 0xFF) {
			printf("%s: cannot find matched sample rate \r\n", __func__);
			break;
		}
		i++;
	}
	
	return sample_rate;
}

static uint32_t get_lea_chnl_num(uint32_t audio_channel_allocation) 
{
    uint32_t channels = 0;
    if (audio_channel_allocation == LC3_AUDIO_LOCATION_MONO) {
        channels = 1;
    }
    else {
        //count_bits_1
        while (audio_channel_allocation) {
            channels ++ ;
            audio_channel_allocation &= (audio_channel_allocation - 1);
        }
    }
    //printf("%s audio_channel_allocation = 0x%x, channels=%d\r\n",__func__,(unsigned int)audio_channel_allocation,channels);
    return channels;
}

static void init_lc3_pcm_free_data(void)
{
	int i;

	memset((void *)lc3_decoder_free_data, '\0', sizeof(lc3_decoder_free_data));
	INIT_LIST_HEAD(&lc3_pcm_data_list);

	for (i = 0; i < MAX_LC3_DECODER_NUM; i++) {
		INIT_LIST_HEAD(&lc3_decoder_free_data[i].list);
		list_add_tail(&lc3_decoder_free_data[i].list, &lc3_pcm_data_list);
	}
	lc3_pcm_data_used_num = 0;
}

int16_t *get_lc3_pcm_buffer(void)
{
	struct list_head *phead = &lc3_pcm_data_list;
	struct list_head *plist = NULL;
	int16_t *plc3_data = NULL;

	if (list_empty(phead)) {
		DBG_BAD("%s: No enough pcm data buffer, used is %d \r\n", __func__, (int)lc3_pcm_data_used_num);
		return NULL;
	}
	plist = phead->next;
	list_del_init(plist);
	plc3_data = (int16_t *)((unsigned int)plist + sizeof(struct list_head));
	lc3_pcm_data_used_num ++;

	return plc3_data;
}

static void release_pcm_buffer(int16_t *pcm_data)
{
	struct list_head *plist;
	plist = (struct list_head *)(((unsigned int)pcm_data) - sizeof(struct list_head));

	list_add_tail(plist, &lc3_pcm_data_list);
	lc3_pcm_data_used_num--;
}

static bool private_lc3_codec_init(PAUDIO_CODEC_ENTITY p_entity, rtk_bt_le_audio_lc3_cfg_t *p_le_audio_codec_cfg)
{
	if (!p_le_audio_codec_cfg) {
		DBG_BAD("%s : lc3 decoder param is NULL \r\n", __func__);
		return false;
	}

	p_entity->lc3.sample_rate = get_sample_rate(p_le_audio_codec_cfg->sample_frequency);
	if (!p_entity->lc3.sample_rate) {
		DBG_BAD("%s : lc3 sample rate invalid \r\n", __func__);
		return false;
	}
	p_entity->lc3.frame_duration = p_le_audio_codec_cfg->frame_duration;
	p_entity->lc3.codec_frame_blocks_per_sdu = p_le_audio_codec_cfg->codec_frame_blocks_per_sdu;
	p_entity->lc3.audio_channel_allocation = p_le_audio_codec_cfg->audio_channel_allocation;
	p_entity->lc3.channels = get_lea_chnl_num(p_entity->lc3.audio_channel_allocation);
	p_entity->lc3.compress_bytes = p_le_audio_codec_cfg->compress_bytes;
	p_entity->lc3.p_lc3_t = audio_lc3_codec_cfg(p_entity->lc3.sample_rate, p_entity->lc3.frame_duration, get_lea_chnl_num(p_entity->lc3.audio_channel_allocation), BIT_PER_SAMPLE_DECODE, BIT_PER_SAMPLE_ENCODE, p_entity->lc3.compress_bytes);
	if (!p_entity->lc3.p_lc3_t) {
		printf("[BT AUDIO] audio_lc3_codec_cfg fail \r\n");
		goto fail;
	}

	return _SUCCESS;

fail:
	p_entity->lc3.sample_rate = 0;
	p_entity->lc3.frame_duration = 0;
	p_entity->lc3.channels = 0;
	p_entity->lc3.codec_frame_blocks_per_sdu = 0;
	p_entity->lc3.audio_channel_allocation = 0;
	p_entity->lc3.compress_bytes = 0;

	return _FAIL;
}

static void private_lc3_codec_deinit(PAUDIO_CODEC_ENTITY p_entity)
{
	lc3_decoder_reset(p_entity->lc3.p_lc3_t);
	p_entity->lc3.frame_duration = 0;
	p_entity->lc3.sample_rate = 0;
	p_entity->lc3.frame_duration = 0;
	p_entity->lc3.channels = 0;
	p_entity->lc3.codec_frame_blocks_per_sdu = 0;
	p_entity->lc3.audio_channel_allocation = 0;
	p_entity->lc3.compress_bytes = 0;
}

uint16_t lc3_codec_init(void *p_entity, void *param)
{
	/* 3rdparty codec init */
	if (private_lc3_codec_init((PAUDIO_CODEC_ENTITY)p_entity, (rtk_bt_le_audio_lc3_cfg_t *)param) == false) {
		DBG_BAD("%s : lc3_decoder_init errir \r\n", __func__);
		return RTK_BT_AUDIO_FAIL;
	}
	/* initialize decoder pcm buffer */
	init_lc3_pcm_free_data();

	return RTK_BT_AUDIO_OK;
}

uint16_t lc3_codec_deinit(void *p_entity)
{
	private_lc3_codec_deinit((PAUDIO_CODEC_ENTITY)p_entity);

	return RTK_BT_AUDIO_OK;
}

uint16_t lc3_decoder_process_data(void *p_entity, uint8_t *data, uint32_t size, struct dec_codec_buffer *decode_buffer, uint32_t *ppcm_size, struct audio_param *paudio_param)
{
	(void)paudio_param;
	PAUDIO_CODEC_ENTITY entity = (PAUDIO_CODEC_ENTITY)p_entity;

	audio_lc3_decode(entity->lc3.p_lc3_t, data, size, decode_buffer->pbuffer, ppcm_size);

	return RTK_BT_AUDIO_OK;
}

uint16_t lc3_encoder_process_data(void *p_entity, int16_t *data, uint32_t size, struct enc_codec_buffer *pencoder_buffer, uint8_t *p_frame_num, uint32_t *p_actual_len)
{
	PAUDIO_CODEC_ENTITY entity = (PAUDIO_CODEC_ENTITY)p_entity;

	if (audio_lc3_encode(entity->lc3.p_lc3_t, data, size, &pencoder_buffer->pbuffer, p_frame_num, p_actual_len)) {
		return RTK_BT_AUDIO_FAIL;
	}
	else {
		pencoder_buffer->frame_num = *p_frame_num;
		pencoder_buffer->frame_size = *p_actual_len;
	}

	return RTK_BT_AUDIO_OK;
}

uint16_t le_audio_handle_media_data_packet(void *p_entity, uint8_t *packet, uint16_t size, uint32_t *pframe_size, uint8_t *pframe_num, uint8_t *pcodec_header_flag, struct audio_param *paudio_param)
{
	(void)packet;
	PAUDIO_CODEC_ENTITY entity = (PAUDIO_CODEC_ENTITY)p_entity;

	*pframe_size = size;
	*pframe_num = entity->lc3.codec_frame_blocks_per_sdu;
	*pcodec_header_flag = 0;
	paudio_param->channels = get_lea_chnl_num(entity->lc3.audio_channel_allocation);
	paudio_param->channel_allocation = entity->lc3.audio_channel_allocation;
	paudio_param->rate = entity->lc3.sample_rate;
	paudio_param->bits = 16;

	return RTK_BT_AUDIO_OK;
}

struct dec_codec_buffer *lc3_decoder_buffer_get(void *p_entity)
{
	(void)p_entity;
	int16_t *ppcm_buffer = NULL;
	struct dec_codec_buffer *pdecoder_buffer = NULL;

	ppcm_buffer = get_lc3_pcm_buffer();
	if (ppcm_buffer == NULL) {
		DBG_BAD("%s: allocate pcm_buffer fial \r\n", __func__);
		return NULL;
	}
	pdecoder_buffer = (struct dec_codec_buffer *)osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(struct dec_codec_buffer));
	if (pdecoder_buffer == NULL) {
		DBG_BAD("%s: allocate pbuffer fial \r\n", __func__);
		release_pcm_buffer(ppcm_buffer);
		return NULL;
	}
	pdecoder_buffer->pbuffer = ppcm_buffer;
	pdecoder_buffer->total_size = (uint32_t)(LC3_MAX_PCM_SIZE);
	pdecoder_buffer->actual_write_size = 0;

	return pdecoder_buffer;
}

void lc3_free_decode_buffer(void *p_entity, struct dec_codec_buffer *pdecodec_buffer)
{
	(void)p_entity;
	/* release pcm buffer */
	release_pcm_buffer(pdecodec_buffer->pbuffer);
	/* free codec buffer */
	osif_mem_free((void *)pdecodec_buffer);
}

struct enc_codec_buffer *lc3_encoder_buffer_get(void *p_entity)
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

void lc3_free_encode_buffer(void *p_entity, struct enc_codec_buffer *pencodec_buffer)
{
	(void)p_entity;

	osif_mem_free((void *)pencodec_buffer);
}

int lc3_decoder_num_samples_per_frame(void *p_entity)
{
	PAUDIO_CODEC_ENTITY entity = (PAUDIO_CODEC_ENTITY)p_entity;

	return entity->lc3.codec_frame_blocks_per_sdu;
}

int lc3_decoder_num_channels(void *p_entity)
{
	PAUDIO_CODEC_ENTITY entity = (PAUDIO_CODEC_ENTITY)p_entity;

	return entity->lc3.channels;
}

int lc3_decoder_sample_rate(void *p_entity)
{
	PAUDIO_CODEC_ENTITY entity = (PAUDIO_CODEC_ENTITY)p_entity;

	return entity->lc3.sample_rate;
}

uint16_t rtk_bt_audio_lc3_register(uint32_t type, PAUDIO_CODEC_ENTITY p_entity)
{
	uint16_t ret = RTK_BT_AUDIO_FAIL;
	uint32_t lock_flag;

	DBG_BAD("%s:Enter \r\n", __func__);
	if (p_entity == NULL) {
		DBG_BAD("%s:NULL entity pointer \r\n");
		return ret;
	}
	lock_flag = osif_lock();
	p_entity->type = type;
	p_entity->stream_in_num = 0;
	p_entity->init = lc3_codec_init;
	p_entity->deinit = lc3_codec_deinit;
	p_entity->bt_audio_handle_media_data_packet = le_audio_handle_media_data_packet;
	p_entity->encoding_func = lc3_encoder_process_data;
	p_entity->decoding_func = lc3_decoder_process_data;
	p_entity->get_decode_buffer = lc3_decoder_buffer_get;
	p_entity->free_decode_buffer = lc3_free_decode_buffer;
	p_entity->get_encode_buffer = lc3_encoder_buffer_get;
	p_entity->free_encode_buffer = lc3_free_encode_buffer;
	p_entity->decoder_num_samples_per_frame = lc3_decoder_num_samples_per_frame;
	p_entity->decoder_num_channels = lc3_decoder_num_channels;
	p_entity->decoder_sample_rate = lc3_decoder_sample_rate;
	p_entity->encode_lock = false;
	p_entity->decode_lock = false;
	memset((void *)&p_entity->lc3, 0, sizeof(p_entity->lc3));
	osif_unlock(lock_flag);
	ret = RTK_BT_AUDIO_OK;

	return ret;
}