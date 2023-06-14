/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

#include <stdio.h>
#include <string.h>
#include <osif.h>
#include <rtk_bt_common.h>
#include <aac_codec_entity.h>
#include <bt_audio_debug.h>
#include <aacenc_lib.h>
#include <aacdecoder_lib.h>

static struct aac_pcm_data aac_decoder_free_data[MAX_AAC_DECODER_NUM] = {0};
static struct list_head aac_pcm_data_list;
static int aac_pcm_data_used_num;
static rtk_bt_aac_codec_t aac_codec_t = {0};
static HANDLE_AACDECODER paac_decoder_handle = NULL;
static HANDLE_AACENCODER paac_encoder_handle = NULL;
static AACENC_InfoStruct encoder_info = {0};
static uint8_t number_of_layers = 1;
// Value indicating the start of AAC Header Segment
static const char *paac_seg_start_seq = "AAC_STRT";
static uint8_t aac_seg_start_seq_len = sizeof(paac_seg_start_seq);
// Value indicating the end of AAC Header Segment
static const char *paac_seg_end_seq = "AAC_ENDS";
static uint8_t aac_seg_end_seq_len = sizeof(paac_seg_end_seq);
static uint8_t header_length_bytes = 2;
static uint8_t min_data_size = 0;
static uint32_t max_configuration_size = 1024;
static uint8_t encode_out_buffer[AAX_ENCODE_MAX_OUT_BUFFER_SIZE] = {0};

static void init_aac_pcm_free_data(void)
{
	int i;

	memset((void *)aac_decoder_free_data, '\0', sizeof(aac_decoder_free_data));
	INIT_LIST_HEAD(&aac_pcm_data_list);

	for (i = 0; i < MAX_AAC_DECODER_NUM; i++) {
		INIT_LIST_HEAD(&aac_decoder_free_data[i].list);
		list_add_tail(&aac_decoder_free_data[i].list, &aac_pcm_data_list);
	}
	aac_pcm_data_used_num = 0;
}

int16_t *get_aac_pcm_buffer(void)
{
	struct list_head *phead = &aac_pcm_data_list;
	struct list_head *plist = NULL;
	int16_t *ppcm_data = NULL;

	if (list_empty(phead)) {
		printf("%s: No enough pcm data buffer, used is %d \r\n", __func__, (int)aac_pcm_data_used_num);
		return NULL;
	}
	plist = phead->next;
	list_del_init(plist);
	ppcm_data = (int16_t *)((unsigned int)plist + sizeof(struct list_head));
	aac_pcm_data_used_num ++;

	return ppcm_data;
}

static void release_pcm_buffer(int16_t *pcm_data)
{
	struct list_head *plist;
	plist = (struct list_head *)(((unsigned int)pcm_data) - sizeof(struct list_head));

	list_add_tail(plist, &aac_pcm_data_list);
	aac_pcm_data_used_num--;
}

static bool bt_stack_aac_decoder_init(void)
{
	paac_decoder_handle = aacDecoder_Open((TRANSPORT_TYPE)RTK_AAC_TT_MP4_LATM_MCP1, number_of_layers);
	if (!paac_decoder_handle) {
		printf("%s: paac_decoder_handle is NULL \r\n", __func__);
		return false;
	}
	min_data_size = aac_seg_start_seq_len + aac_seg_end_seq_len + header_length_bytes + 2;

	return true;
}

static bool bt_stack_aac_encoder_init(rtk_bt_aac_encode_t *pencoder_t)
{
	// /* init flow */
	// if (AACENC_OK != aacEncOpen(&paac_encoder_handle, 0, 0)) {
	// 	printf("%s: mAacDecoderHandle is NULL \r\n", __func__);
	// 	return false;
	// }

	// if (aacEncoder_SetParam(paac_encoder_handle, AACENC_AOT, pencoder_t->object_type)) {
	// 	printf("%s: AACENC_AOT set failed \r\n", __func__);
	// 	return false;
	// };

	// if (aacEncoder_SetParam(paac_encoder_handle, AACENC_SBR_MODE, 0)) {
	// 	printf("%s: AACENC_SBR_MODE set failed \r\n", __func__);
	// 	return false;
	// };

	// if (aacEncoder_SetParam(paac_encoder_handle, AACENC_SAMPLERATE, pencoder_t->sample_freq)) {
	// 	printf("%s: AACENC_SAMPLERATE set failed \r\n", __func__);
	// 	return false;
	// };

	// if (aacEncoder_SetParam(paac_encoder_handle, AACENC_CHANNELMODE, pencoder_t->channels)) {
	// 	printf("%s: AACENC_CHANNELMODE set failed \r\n", __func__);
	// 	return false;
	// };
	// /* AACENC_CHANNELORDER 1 WAV */
	// if (aacEncoder_SetParam(paac_encoder_handle, AACENC_CHANNELORDER, 1)) {
	// 	printf("%s: AACENC_CHANNELORDER set failed \r\n", __func__);
	// 	return false;
	// };

	// if (aacEncoder_SetParam(paac_encoder_handle, AACENC_BITRATEMODE, 0)) {
	// 	printf("%s: AACENC_BITRATEMODE set failed \r\n", __func__);
	// 	return false;
	// };

	// if (aacEncoder_SetParam(paac_encoder_handle, AACENC_BITRATE, pencoder_t->bit_rate)) {
	// 	printf("%s: AACENC_BITRATE set failed \r\n", __func__);
	// 	return false;
	// };

	// if (aacEncoder_SetParam(paac_encoder_handle, AACENC_TRANSMUX, TT_MP4_ADTS)) {
	// 	printf("%s: AACENC_TRANSMUX set failed \r\n", __func__);
	// 	return false;
	// };
	// /* Call aacEncEncode() with NULL parameters to initialize encoder instance with present parameter set */
	// if (aacEncEncode(paac_encoder_handle, NULL, NULL, NULL, NULL)) {
	// 	printf("%s: init encoder instance failed \r\n", __func__);
	// 	return false;
	// };

	// aacEncInfo(paac_encoder_handle, &encoder_info);

	return true;
}

static uint16_t aac_codec_init(void *p_entity, void *param)
{
	(void)p_entity;
	rtk_bt_aac_codec_t *paac_codec_t = (rtk_bt_aac_codec_t *)param;

	if (!paac_codec_t) {
		printf("%s : No rtk_bt_aac_codec_t configure \r\n", __func__);
		return 1;
	}
	/* initialize decoder pcm buffer */
	init_aac_pcm_free_data();
	/* 3rdparty decoder init */
	if (bt_stack_aac_decoder_init() == false) {
		printf("%s : bt_stack_aac_decoder_init error \r\n", __func__);
		return 1;
	}
	/* 3rdparty encoder init */
	if (bt_stack_aac_encoder_init(&paac_codec_t->encoder_t) == false) {
		printf("%s : bt_stack_aac_encoder_init error \r\n", __func__);
		return 1;
	}

	memcpy((void *)&aac_codec_t, (void *)paac_codec_t, sizeof(rtk_bt_aac_codec_t));

	return 0;
}

static uint16_t aac_codec_deinit(void *p_entity)
{
	(void)p_entity;
	/* deinit encoder */
	aacEncClose(&paac_encoder_handle);
	paac_encoder_handle = NULL;
	/* deinit decoder */
	aacDecoder_Close(paac_decoder_handle);
	paac_decoder_handle = NULL;
	min_data_size = 0;

	memset((void *)&aac_codec_t, 0, sizeof(rtk_bt_aac_codec_t));

	return 0;
}

static unsigned int getHeaderSize(unsigned char *data, unsigned int size)
{
	int32_t result = 0;
	uint32_t header_length = 0;

	if (size < min_data_size) {
		return 0;
	}

	result = memcmp(data, paac_seg_start_seq, aac_seg_start_seq_len);
	if (result) {
		return 0;
	}
	data += aac_seg_start_seq_len;
	size -= aac_seg_start_seq_len;

	header_length = (data[0] << 8 | data[1]) & 0xFFFF;
	data += header_length_bytes;
	size -= header_length_bytes;

	if (header_length + aac_seg_end_seq_len > size) {
		return 0;
	}

	data += header_length;
	size -= header_length;
	result = memcmp(data, paac_seg_end_seq, aac_seg_end_seq_len);
	if (result) {
		return 0;
	}

	return (header_length > max_configuration_size) ? max_configuration_size: header_length;
}

static uint16_t aac_decoder_process_data(void *p_entity, uint8_t *data, uint32_t size, struct dec_codec_buffer *decode_buffer, uint32_t *ppcm_size, struct audio_param *paudio_param)
{
	(void)p_entity;
	AAC_DECODER_ERROR mErrorCode = AAC_DEC_OK;
	unsigned int headerSize = 0;
	uint32_t bytes_valid = size;
	uint32_t buffer_size = size;
	// uint8_t decoded_num = 0;

	if (!paac_decoder_handle) {
		printf("%s : Decoder has not been inited \r\n", __func__);
		return 0;
	}
	if (size > 1024) {
		printf("%s : Decoder max pcm size si 1024 \r\n", __func__);
		return 0;
	}
	printf("length is %d \r\n", size);
	headerSize = getHeaderSize(data, size);
	if (headerSize != 0) {
		printf("has header \r\n");
		data += aac_seg_start_seq_len + header_length_bytes;
		size -= aac_seg_start_seq_len + header_length_bytes;
		aacDecoder_ConfigRaw(paac_decoder_handle, &data, &headerSize);
		data += headerSize + aac_seg_end_seq_len;
		size -= headerSize + aac_seg_end_seq_len;
	}
	while (bytes_valid > 0) {
		CStreamInfo *p_si = NULL;
		mErrorCode = aacDecoder_Fill(paac_decoder_handle, &data, &buffer_size, &bytes_valid);
		if (mErrorCode != AAC_DEC_OK) {
			printf("%s : accDecoder_Fill fail ! \r\n", __func__);
			return 1;
		}	
		/* do decode until return no AAC_DEC_NOT_ENOUGH_BITS */
		while (true) {
			mErrorCode =
				aacDecoder_DecodeFrame(paac_decoder_handle, decode_buffer->pbuffer, decode_buffer->total_size /*size in number of short, not bytes*/, 0);
			if (mErrorCode == AAC_DEC_NOT_ENOUGH_BITS) {
				/* input aac data is not enough, waiting for more data input */
				printf("Done \r\n");
				break;
			}
			if (mErrorCode != AAC_DEC_OK) {
				printf("%s : decode error %d ! \r\n", __func__, mErrorCode);
				memset((void *)decode_buffer->pbuffer, 0, decode_buffer->total_size * 2);
				*ppcm_size = 0;
				break;
			}
			if (mErrorCode == AAC_DEC_OK) {
				// if (decoded_num ++ < 1) {
					p_si = aacDecoder_GetStreamInfo(paac_decoder_handle);
					if (!p_si || p_si->sampleRate <= 0) {
						printf("%s : get stream info error ! \r\n", __func__);
						memset((void *)decode_buffer->pbuffer, 0, decode_buffer->total_size * 2);
						*ppcm_size = 0;
						break;
					}
					paudio_param->channels = p_si->numChannels;
					if (paudio_param->channels == 1) {
						/* only left channel */
						paudio_param->channel_allocation = 1;
					}
					paudio_param->rate = p_si->sampleRate;
					paudio_param->bits = 0;
					*ppcm_size = p_si->frameSize * p_si->numChannels;
				// } else {
				// 	printf("%s : Only allow one aac packet ! \r\n", __func__);
				// 	memset((void *)decode_buffer->pbuffer, 0, decode_buffer->total_size * 2);
				// 	*ppcm_size = 0;
				// 	break;
				// }
			}
		} 
	}

	return 0;
}

static uint16_t aac_encoder_process_data(void *p_entity, int16_t *data, uint32_t size, struct enc_codec_buffer *pencoder_buffer, uint8_t *p_frame_num, uint32_t *p_actual_len)
{
	(void)p_entity;
	uint16_t ret = 1;
	uint8_t *paudio_data = (uint8_t *)data;
	void *pout_buffer = (void *)encode_out_buffer;
	uint32_t audio_size = size;
	int buffer_ids = IN_AUDIO_DATA;
	int in_sl_size = sizeof(int16_t);
	int buff_size = size;
	int out_buffer_lds = OUT_BITSTREAM_DATA;
	int out_buffer_size = AAX_ENCODE_MAX_OUT_BUFFER_SIZE;
	int out_sl_size = sizeof(uint8_t);

	if (!paac_encoder_handle) {
		printf("%s : Decoder has not been inited \r\n", __func__);
		return 0;
	}

	AACENC_InArgs inargs = {0};
	AACENC_OutArgs outargs = {0};
	AACENC_BufDesc inBufDesc = {0};
	AACENC_BufDesc outBufDesc = {0};

	while (audio_size > 0) {
		memset(&inargs, 0, sizeof(inargs));
		inargs.numInSamples = audio_size / sizeof(int16_t);
		inargs.numAncBytes = 0;

		inBufDesc.numBufs = 1;
		inBufDesc.bufs = (void **)&paudio_data;
		inBufDesc.bufferIdentifiers = &buffer_ids;
		inBufDesc.bufSizes = &buff_size;
		inBufDesc.bufElSizes = &in_sl_size;

		outBufDesc.numBufs = 1;
		outBufDesc.bufs = (void **)&pout_buffer;
		outBufDesc.bufferIdentifiers = &out_buffer_lds;
		outBufDesc.bufSizes = &out_buffer_size;
		outBufDesc.bufElSizes = &out_sl_size;

		AACENC_OutArgs outargs = {};
		aacEncEncode(paac_encoder_handle, &inBufDesc, &outBufDesc, &inargs, &outargs);
		
		if (outargs.numOutBytes == 0) {
			*p_frame_num = 0;
			*p_actual_len = 0;
			printf("%s : This pcm frame encode has no aac output \r\n", __func__);
			break;
		} else {
			uint32_t audioConsumed = outargs.numInSamples * sizeof(int16_t);
			if (audio_size != audioConsumed) {
				*p_frame_num = 0;
				*p_actual_len = 0;
				printf("%s : Not support multiple encode \r\n", __func__);
				break;
			}
			if (outargs.numOutBytes > AAX_ENCODE_MAX_OUT_BUFFER_SIZE) {
				printf("%s :Encoder buffer size (%d) is not enough, required (%d) \r\n",
																		__func__,
																		AAX_ENCODE_MAX_OUT_BUFFER_SIZE,
																		outargs.numOutBytes);
				*p_frame_num = 0;
				*p_actual_len = 0;
			} else {
				*p_frame_num = 1;
				*p_actual_len = outargs.numOutBytes;
				pencoder_buffer->pbuffer = encode_out_buffer;
				pencoder_buffer->frame_size = *p_actual_len;
				pencoder_buffer->frame_num = *p_frame_num;
			}
		}
		// check whether there is only metadata input which is redundant
		/* TODO */
	}

	return ret;
}

static uint16_t aac_audio_handle_media_data_packet(void *p_entity, uint8_t *packet, uint16_t size, uint32_t *pframe_size, uint8_t *pframe_num, uint8_t *pcodec_header_flag, struct audio_param *paudio_param)
{
	(void)p_entity;
	(void)packet;

	*pframe_size = size;
	*pframe_num = 1;
	*pcodec_header_flag = 0;
	paudio_param->channels = 2;
	paudio_param->channel_allocation = 0;
	paudio_param->rate = 0;
	paudio_param->bits = 0;

	return 0;
}

static struct dec_codec_buffer *aac_decoder_buffer_get(void *p_entity)
{
	(void)p_entity;
	int16_t *ppcm_buffer = NULL;
	struct dec_codec_buffer *pdecoder_buffer = NULL;

	ppcm_buffer = get_aac_pcm_buffer();
	if (ppcm_buffer == NULL) {
		printf("%s: allocate pcm_buffer fail \r\n", __func__);
		return NULL;
	}
	pdecoder_buffer = (struct dec_codec_buffer *)osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(struct dec_codec_buffer));
	if (pdecoder_buffer == NULL) {
		printf("%s: allocate pdecoder_buffer fail \r\n", __func__);
		release_pcm_buffer(ppcm_buffer);
		return NULL;
	}
	pdecoder_buffer->pbuffer = ppcm_buffer;
	pdecoder_buffer->total_size = (uint32_t)AAC_DECODE_MAX_OUT_BUFFER_SIZE;
	pdecoder_buffer->actual_write_size = 0;

	return pdecoder_buffer;
}

static void aac_free_decode_buffer(void *p_entity, struct dec_codec_buffer *pdecoder_buffer)
{
	(void)p_entity;

	/* release pcm buffer */
	release_pcm_buffer(pdecoder_buffer->pbuffer);
	/* free codec buffer */
	osif_mem_free((void *)pdecoder_buffer);
}

static struct enc_codec_buffer *aac_encoder_buffer_get(void *p_entity)
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

static void aac_free_encode_buffer(void *p_entity, struct enc_codec_buffer *pencodec_buffer)
{
	(void)p_entity;

	osif_mem_free((void *)pencodec_buffer);
}

static int aac_decoder_num_samples_per_frame(void *p_entity)
{
	(void)p_entity;

	return 1;
}

static int aac_decoder_num_channels(void *p_entity)
{
	(void)p_entity;

	return aac_codec_t.decoder_t.channels;
}

static int aac_decoder_sample_rate(void *p_entity)
{
	(void)p_entity;

	return aac_codec_t.decoder_t.sample_freq;
}

uint16_t rtk_bt_audio_aac_register(uint32_t type, PAUDIO_CODEC_ENTITY p_entity)
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
	p_entity->init = aac_codec_init;
	p_entity->deinit = aac_codec_deinit;
	p_entity->bt_audio_handle_media_data_packet = aac_audio_handle_media_data_packet;
	p_entity->encoding_func = aac_encoder_process_data;
	p_entity->decoding_func = aac_decoder_process_data;
	p_entity->get_decode_buffer = aac_decoder_buffer_get;
	p_entity->free_decode_buffer = aac_free_decode_buffer;
	p_entity->get_encode_buffer = aac_encoder_buffer_get;
	p_entity->free_encode_buffer = aac_free_encode_buffer;
	p_entity->decoder_num_samples_per_frame = aac_decoder_num_samples_per_frame;
	p_entity->decoder_num_channels = aac_decoder_num_channels;
	p_entity->decoder_sample_rate = aac_decoder_sample_rate;
	p_entity->encode_lock = false;
	p_entity->decode_lock = false;
	memset((void *)&p_entity->aac, 0, sizeof(p_entity->aac));
	osif_unlock(lock_flag);
	ret = 0;

	return ret;
}