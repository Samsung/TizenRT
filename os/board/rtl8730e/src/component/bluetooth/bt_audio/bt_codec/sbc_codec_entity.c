/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

#include <stdio.h>
#include <string.h>
#include <osif.h>
#include <rtk_bt_common.h>
#include <sbc_codec_entity.h>
#include <bt_audio_debug.h>

static rtk_bt_sbc_codec_t sbc_codec_t = {0};
static sbc_decoder_state_t priv_decode_state = {0};
static sbc_encoder_state_t priv_encode_state = {0};
static bludroid_decoder_state_t bluedroid_decode_state = {0};
static bludroid_encoder_state_t bluedroid_encoder_state = {0};
static sbc_decoder_state_t *psbc_decoder_state_singleton = NULL;
static sbc_encoder_state_t *psbc_encoder_state_singleton = NULL;
static bludroid_decoder_state_t *pbluedroid_decode_state = NULL;
static struct sbc_pcm_data sbc_decoder_free_data[MAX_SBC_DECODER_NUM] = {0};
static struct list_head sbc_pcm_data_list;
static int sbc_pcm_data_used_num;

static void init_sbc_pcm_free_data(void)
{
	int i;

	memset((void *)sbc_decoder_free_data, '\0', sizeof(sbc_decoder_free_data));
	INIT_LIST_HEAD(&sbc_pcm_data_list);

	for (i = 0; i < MAX_SBC_DECODER_NUM; i++) {
		INIT_LIST_HEAD(&sbc_decoder_free_data[i].list);
		list_add_tail(&sbc_decoder_free_data[i].list, &sbc_pcm_data_list);
	}
	sbc_pcm_data_used_num = 0;
}

int16_t *get_sbc_pcm_buffer(void)
{
	struct list_head *phead = &sbc_pcm_data_list;
	struct list_head *plist = NULL;
	int16_t *ppcm_data = NULL;

	if (list_empty(phead)) {
		printf("%s: No enough pcm data buffer, used is %d \r\n", __func__, (int)sbc_pcm_data_used_num);
		return NULL;
	}
	plist = phead->next;
	list_del_init(plist);
	ppcm_data = (int16_t *)((unsigned int)plist + sizeof(struct list_head));
	sbc_pcm_data_used_num ++;

	return ppcm_data;
}

static void release_pcm_buffer(int16_t *pcm_data)
{
	struct list_head *plist;
	plist = (struct list_head *)(((unsigned int)pcm_data) - sizeof(struct list_head));

	list_add_tail(plist, &sbc_pcm_data_list);
	sbc_pcm_data_used_num--;
}

static bool bt_stack_sbc_decoder_init(sbc_decoder_state_t *pstate, sbc_mode_t mode, void *pcontext)
{
	if (psbc_decoder_state_singleton) {
		printf("%s : sbc decoder has already been initialized \r\n", __func__);
		return false;
	}
	OI_STATUS status = OI_STATUS_SUCCESS;
	switch (mode) {
	case SBC_MODE_STANDARD:
		// note: we always request stereo output, even for mono input
		status = OI_CODEC_SBC_DecoderReset(&(bluedroid_decode_state.decoder_context), bluedroid_decode_state.decoder_data, sizeof(bluedroid_decode_state.decoder_data), 2, 2, FALSE);
		break;
	case SBC_MODE_mSBC:
		status = OI_CODEC_mSBC_DecoderReset(&(bluedroid_decode_state.decoder_context), bluedroid_decode_state.decoder_data, sizeof(bluedroid_decode_state.decoder_data));
		break;
	default:
		break;
	}

	if (status != OI_STATUS_SUCCESS) {
		printf("%s : error during reset %d \r\n", __func__, status);
		return false;
	}

	psbc_decoder_state_singleton = pstate;

	bluedroid_decode_state.bytes_in_frame_buffer = 0;
	bluedroid_decode_state.pcm_bytes = 2 * SBC_MAX_CHANNELS * SBC_MAX_BANDS * SBC_MAX_BLOCKS;
	bluedroid_decode_state.h2_sequence_nr = -1;
	bluedroid_decode_state.first_good_frame_found = 0;

	memset(pstate, 0, sizeof(sbc_decoder_state_t));
	pstate->mode = mode;
	pstate->context = pcontext;
	pstate->decoder_state = &bluedroid_decode_state;
	// btstack_sbc_plc_init(&pstate->plc_state);

	return true;
}

static bool bt_stack_sbc_encoder_init(sbc_encoder_state_t *pstate, sbc_mode_t mode, 
						int blocks, int subbands, sbc_allocation_method_t allocation_method, 
						int sample_rate, int bitpool, sbc_channel_mode_t channel_mode) {

	if (psbc_encoder_state_singleton && (psbc_encoder_state_singleton != pstate) ){
		printf("%s : SBC encoder: different sbc decoder state is allready registered \r\n", __func__);
		return false;
	} 
	
	psbc_encoder_state_singleton = pstate;

	if (!psbc_encoder_state_singleton) {
		printf("%s : SBC encoder init: sbc state is NULL \r\n", __func__);
		return false;
	}

	psbc_encoder_state_singleton->mode = mode;

	switch (psbc_encoder_state_singleton->mode){
		case SBC_MODE_STANDARD:
			bluedroid_encoder_state.context.s16NumOfBlocks = blocks;
			bluedroid_encoder_state.context.s16NumOfSubBands = subbands;
			bluedroid_encoder_state.context.s16AllocationMethod = (uint8_t)allocation_method;
			bluedroid_encoder_state.context.s16BitPool = bitpool;
			bluedroid_encoder_state.context.mSBCEnabled = 0;
			bluedroid_encoder_state.context.s16ChannelMode = (uint8_t)channel_mode;
			bluedroid_encoder_state.context.s16NumOfChannels = 2;
			if (bluedroid_encoder_state.context.s16ChannelMode == SBC_MONO){
				bluedroid_encoder_state.context.s16NumOfChannels = 1;
			}
			switch(sample_rate){
				case 16000: bluedroid_encoder_state.context.s16SamplingFreq = SBC_sf16000; break;
				case 32000: bluedroid_encoder_state.context.s16SamplingFreq = SBC_sf32000; break;
				case 44100: bluedroid_encoder_state.context.s16SamplingFreq = SBC_sf44100; break;
				case 48000: bluedroid_encoder_state.context.s16SamplingFreq = SBC_sf48000; break;
				default: bluedroid_encoder_state.context.s16SamplingFreq = 0; break;
			}
			break;
		case SBC_MODE_mSBC:
			bluedroid_encoder_state.context.s16NumOfBlocks    = 15;
			bluedroid_encoder_state.context.s16NumOfSubBands  = 8;
			bluedroid_encoder_state.context.s16AllocationMethod = SBC_LOUDNESS;
			bluedroid_encoder_state.context.s16BitPool   = 26;
			bluedroid_encoder_state.context.s16ChannelMode = SBC_MONO;
			bluedroid_encoder_state.context.s16NumOfChannels = 1;
			bluedroid_encoder_state.context.mSBCEnabled = 1;
			bluedroid_encoder_state.context.s16SamplingFreq = SBC_sf16000;
			break;
		default:
			break;
	}
	bluedroid_encoder_state.context.pu8Packet = bluedroid_encoder_state.sbc_packet;
	psbc_encoder_state_singleton->encoder_state = &bluedroid_encoder_state;
	SBC_ENC_PARAMS *context = &((bludroid_encoder_state_t *)psbc_encoder_state_singleton->encoder_state)->context;
	SBC_Encoder_Init(context);

	return true;
}

static uint16_t sbc_codec_init(void *pentity, void *param)
{
	(void)pentity;
	rtk_bt_sbc_codec_t *psbc_codec_t = (rtk_bt_sbc_codec_t *)param;

	if (!psbc_codec_t) {
		printf("%s : No rtk_bt_sbc_codec_t configure \r\n", __func__);
		return 1;
	}
	/* 3rdparty decoder init */
	if (bt_stack_sbc_decoder_init(&priv_decode_state, (sbc_mode_t)psbc_codec_t->decoder_t.sbc_dec_mode, NULL) == false) {
		printf("%s : bt_stack_sbc_decoder_init error \r\n", __func__);
		return 1;
	}
	pbluedroid_decode_state = (bludroid_decoder_state_t *)priv_decode_state.decoder_state;
	/* initialize decoder pcm buffer */
	init_sbc_pcm_free_data();
	/* 3rdparty encoder init */
	if (bt_stack_sbc_encoder_init(&priv_encode_state, (sbc_mode_t)psbc_codec_t->encoder_t.sbc_enc_mode, 
				psbc_codec_t->encoder_t.blocks, psbc_codec_t->encoder_t.subbands, 
				psbc_codec_t->encoder_t.alloc_method, psbc_codec_t->encoder_t.sample_rate, 
				psbc_codec_t->encoder_t.bitpool,
				psbc_codec_t->encoder_t.channel_mode) == false) {
		printf("%s : bt_stack_sbc_encoder_init error \r\n", __func__);
		return 1;
	}

	memcpy((void *)&sbc_codec_t, (void *)psbc_codec_t, sizeof(rtk_bt_sbc_codec_t));

	return 0;
}

static uint16_t sbc_codec_deinit(void *pentity)
{
	(void)pentity;

	memset((void *)&priv_decode_state, 0, sizeof(sbc_decoder_state_t));
	memset((void *)&bluedroid_decode_state, 0, sizeof(bludroid_decoder_state_t));
	psbc_decoder_state_singleton = NULL;
	pbluedroid_decode_state = NULL;
	memset((void *)&sbc_codec_t, 0, sizeof(rtk_bt_sbc_codec_t));

	return 0;
}

#if 0
static void sbc_decoder_process_msbc_data(sbc_decoder_state_t * state, int packet_status_flag, uint8_t * buffer, int size, int16_t *ppcm_data)
{
	bludroid_decoder_state_t * decoder_state = (bludroid_decoder_state_t*)state->decoder_state;
	int input_bytes_to_process = size;
	const unsigned int MSBC_FRAME_SIZE = 60;

	while (input_bytes_to_process > 0){

		// Use PLC to insert missing frames (after first sync found)
		btstack_sbc_decoder_insert_missing_frames(state);

		// fill buffer with new data
		int bytes_missing_for_complete_msbc_frame = MSBC_FRAME_SIZE - decoder_state->bytes_in_frame_buffer;
		int bytes_to_append = btstack_min(input_bytes_to_process, bytes_missing_for_complete_msbc_frame);
		if (bytes_to_append) {
			append_received_sbc_data(decoder_state, buffer, bytes_to_append);
			buffer += bytes_to_append;
			input_bytes_to_process -= bytes_to_append;
		}

		// complete frame in  buffer?
		if (decoder_state->bytes_in_frame_buffer < MSBC_FRAME_SIZE) break;
		
		uint16_t bytes_in_frame_buffer_before_decoding = decoder_state->bytes_in_frame_buffer;
		uint16_t bytes_processed = 0;
		const OI_BYTE *frame_data = decoder_state->frame_buffer;

		// testing only - corrupt frame periodically
		btstack_sbc_decoder_bluedroid_simulate_error(frame_data);

		// assert frame looks like this: 01 x8 AD [rest of frame 56 bytes] 00
		int h2_syncword = 0;
		int h2_sync_pos = find_h2_sync(frame_data, decoder_state->bytes_in_frame_buffer, &h2_syncword);
		if (h2_sync_pos < 0){
			// no sync found, discard all but last 2 bytes
			bytes_processed = decoder_state->bytes_in_frame_buffer - 2;
			memmove(decoder_state->frame_buffer, decoder_state->frame_buffer + bytes_processed, decoder_state->bytes_in_frame_buffer);
			decoder_state->bytes_in_frame_buffer -= bytes_processed;    // == 2
			// don't try PLC without at least a single good frame
			if (decoder_state->first_good_frame_found){
				decoder_state->msbc_bad_bytes += bytes_processed;
			}
			continue;
		}

		decoder_state->h2_sequence_nr = h2_syncword;

		// drop data before it
		bytes_processed = h2_sync_pos - 2;
		if (bytes_processed > 2){
			memmove(decoder_state->frame_buffer, decoder_state->frame_buffer + bytes_processed, decoder_state->bytes_in_frame_buffer);
			decoder_state->bytes_in_frame_buffer -= bytes_processed;
			// don't try PLC without at least a single good frame
			if (decoder_state->first_good_frame_found){
				decoder_state->msbc_bad_bytes += bytes_processed;
			}
			continue;
		}

		int bad_frame = 0;
		int zero_seq_found = find_sequence_of_zeros(frame_data, decoder_state->bytes_in_frame_buffer, 20);

		// after first valid frame, zero sequences count as bad frames
		if (decoder_state->first_good_frame_found){
			bad_frame = zero_seq_found || packet_status_flag;
		} 

		if (bad_frame){
			// stats
			if (zero_seq_found){
				state->zero_frames_nr++;
			} else {
				state->bad_frames_nr++;
			}
	#ifdef LOG_FRAME_STATUS 
			if (zero_seq_found){
				printf("%d : ZERO FRAME\n", decoder_state->h2_sequence_nr);
			} else {
				printf("%d : BAD FRAME\n", decoder_state->h2_sequence_nr);
			}
	#endif
			// retry after dropoing 3 byte sync
			bytes_processed = 3;
			memmove(decoder_state->frame_buffer, decoder_state->frame_buffer + bytes_processed, decoder_state->bytes_in_frame_buffer);
			decoder_state->bytes_in_frame_buffer -= bytes_processed;
			decoder_state->msbc_bad_bytes        += bytes_processed;
			// log_info("Trace bad frame");
			continue;
		}

		// ready to decode frame
		OI_STATUS status = OI_CODEC_SBC_DecodeFrame(&(decoder_state->decoder_context), 
											&frame_data, 
											&(decoder_state->bytes_in_frame_buffer), 
											decoder_state->pcm_plc_data, 
											&(decoder_state->pcm_bytes));

		bytes_processed = bytes_in_frame_buffer_before_decoding - decoder_state->bytes_in_frame_buffer;
		// log_info("Trace decode status %u, processed %u (bad bytes %u), bytes in buffer %u", (int) status, bytes_processed, decoder_state->msbc_bad_bytes, decoder_state->bytes_in_frame_buffer);

		switch(status){
			case 0:
				// synced
				decoder_state->first_good_frame_found = 1;

				// get rid of padding byte, not processed by SBC decoder
				decoder_state->bytes_in_frame_buffer = 0;

				// restart counting bad bytes
				decoder_state->msbc_bad_bytes = 0;

				// feed good frame into PLC history
				btstack_sbc_plc_good_frame(&state->plc_state, decoder_state->pcm_plc_data, decoder_state->pcm_data);

				// deliver PCM data
				state->handle_pcm_data(decoder_state->pcm_data, 
									btstack_sbc_decoder_num_samples_per_frame(state), 
									btstack_sbc_decoder_num_channels(state), 
									btstack_sbc_decoder_sample_rate(state), state->context);

				// stats
				state->good_frames_nr++;
				continue;

			case OI_CODEC_SBC_CHECKSUM_MISMATCH:
				// The next frame is somehow corrupt.
				log_debug("OI_CODEC_SBC_CHECKSUM_MISMATCH");
				// Did the codec consume any bytes?
				if (bytes_processed > 0){
					// Good. Nothing to do.
				} else {
					// Skip the bogus frame by skipping the header.
					bytes_processed = 1;
				}
				break;

			case OI_STATUS_INVALID_PARAMETERS:
				// This caused by corrupt frames.
				// The codec apparently does not recover from this.
				// Re-initialize the codec.
				log_info("SBC decode: invalid parameters: resetting codec");
				if (OI_CODEC_mSBC_DecoderReset(&(bd_decoder_state.decoder_context), bd_decoder_state.decoder_data, sizeof(bd_decoder_state.decoder_data)) != OI_STATUS_SUCCESS){
					log_info("SBC decode: resetting codec failed");
				}
				break;
			default:
				log_info("Frame decode error: %d", status);
				break;
		}

		// on success, while loop was restarted, so all processed bytes have been "bad"
		decoder_state->msbc_bad_bytes += bytes_processed;

		// drop processed bytes from frame buffer
		memmove(decoder_state->frame_buffer, decoder_state->frame_buffer + bytes_processed, decoder_state->bytes_in_frame_buffer);
	}
}
#endif
static void sbc_decoder_process_sbc_data(sbc_decoder_state_t *state, uint8_t *buffer, int size, int16_t *ppcm_data)
{
	bludroid_decoder_state_t *decoder_state = (bludroid_decoder_state_t *)state->decoder_state;
	int input_bytes_to_process = size;
	int keep_decoding = 1;
	int bytes_in_frame_buffer_before_decoding = size;
	const OI_BYTE *frame_data = buffer;
	OI_UINT32 frame_data_len = size;
	OI_STATUS status;
	uint16_t bytes_processed = 0;
	// DBG_BAD("%s : Enter frame size %d \r\n", __func__, size);
	while (keep_decoding) {
		/* Decode the next frame in decoder_state->frame_buffer. */
		bytes_in_frame_buffer_before_decoding -= bytes_processed;
		/* frame_data += bytes_processed;
		frame_data_len -= bytes_processed; */
		status = OI_CODEC_SBC_DecodeFrame(&(decoder_state->decoder_context),
										&frame_data,
										&frame_data_len,
										ppcm_data,
										&(decoder_state->pcm_bytes));
		bytes_processed = bytes_in_frame_buffer_before_decoding - frame_data_len;

		/* Handle decoding result. */
		switch (status) {
		case OI_STATUS_SUCCESS:
		case OI_CODEC_SBC_PARTIAL_DECODE: {
			if (frame_data_len == 0) {
				keep_decoding = 0;
				DBG_BAD(" Decode complete pcm_bytes %d \r\n", decoder_state->pcm_bytes);
				return;
			} else {
				DBG_BAD("%s : decoding something wrong \r\n", __func__);
				return;
			}
		}
		break;
		case OI_CODEC_SBC_NOT_ENOUGH_HEADER_DATA:
		case OI_CODEC_SBC_NOT_ENOUGH_BODY_DATA:
		case OI_CODEC_SBC_NOT_ENOUGH_AUDIO_DATA: {
			if (input_bytes_to_process > 0) {
				/* Should never occur: The SBC codec claims there is not enough bytes in the frame_buffer,
				but the frame_buffer was full. (The frame_buffer is always full before decoding when input_bytes_to_process > 0.)
				Clear frame_buffer. */
				DBG_BAD("%s : frame_buffer too small for frame \r\n", __func__);
				bytes_processed = bytes_in_frame_buffer_before_decoding;
			} else {
				/* Exit decode loop, because there is not enough data in frame_buffer to decode the next frame. */
				keep_decoding = 0;
			}
		}
		break;
		case OI_CODEC_SBC_NO_SYNCWORD: {
			/* This means the entire frame_buffer did not contain the syncword.
			Discard the frame_buffer contents. */
			DBG_BAD("%s : no syncword found \r\n", __func__);
			bytes_processed = bytes_in_frame_buffer_before_decoding;
		}
		break;
		case OI_CODEC_SBC_CHECKSUM_MISMATCH: {
			/* The next frame is somehow corrupt. */
			DBG_BAD("%s : checksum error \r\n");
			/* Did the codec consume any bytes? */
			if (bytes_processed > 0) {
				/* Good. Nothing to do. */
			} else {
				/* Skip the bogus frame by skipping the header. */
				bytes_processed = 1;
			}
		}
		break;
		case OI_STATUS_INVALID_PARAMETERS: {
			/* This caused by corrupt frames.
			The codec apparently does not recover from this.
			Re-initialize the codec. */
			DBG_BAD("%s : invalid parameters: resetting codec \r\n", __func__);
			if (OI_CODEC_SBC_DecoderReset(&(decoder_state->decoder_context), decoder_state->decoder_data, sizeof(decoder_state->decoder_data), 2, 2,
										FALSE) != OI_STATUS_SUCCESS) {
				DBG_BAD("%s : resetting codec failed \r\n", __func__);
			}
		}
		break;
		default: {
			/* Anything else went wrong.
			Skip a few bytes and try again. */
			bytes_processed = 1;
			DBG_BAD("%s : unknown status %d \r\n", __func__, status);
		}
		break;
		}
	}
}

static void decoder_process_data(sbc_decoder_state_t *pstate, int packet_status_flag, uint8_t *buffer, int size, int16_t *ppcm_data, uint32_t *ppcm_size)
{
	(void)packet_status_flag;
	bludroid_decoder_state_t *pdecoder_state = (bludroid_decoder_state_t *)pstate->decoder_state;

	if (pstate->mode == SBC_MODE_mSBC) {
		// sbc_decoder_process_msbc_data(pstate, packet_status_flag, buffer, size, ppcm_data);
	} else {
		sbc_decoder_process_sbc_data(pstate, buffer, size, ppcm_data);
		*ppcm_size = pdecoder_state->pcm_bytes;
	}
}

static uint16_t sbc_decoder_process_data(void *pentity, uint8_t *data, uint32_t size, struct dec_codec_buffer *decode_buffer, uint32_t *ppcm_size, struct audio_param *paudio_param)
{
	(void)pentity;
	(void)paudio_param;

	decoder_process_data(&priv_decode_state, 0, data, (int)size, decode_buffer->pbuffer, ppcm_size);

	return 0;
}

// static int sbc_encoder_num_audio_frames(void){
// 	SBC_ENC_PARAMS * context = &((bludroid_encoder_state_t *)psbc_encoder_state_singleton->encoder_state)->context;
// 	return context->s16NumOfSubBands * context->s16NumOfBlocks;
// }

static uint8_t *sbc_encoder_sbc_buffer(void){
	SBC_ENC_PARAMS * context = &((bludroid_encoder_state_t *)psbc_encoder_state_singleton->encoder_state)->context;
	return context->pu8Packet;
}

static uint16_t sbc_encoder_sbc_buffer_length(void){
	SBC_ENC_PARAMS * context = &((bludroid_encoder_state_t *)psbc_encoder_state_singleton->encoder_state)->context;
	return context->u16PacketLength;
}

static int max_input_mono_mode_pcm_size_in_bytes = SBC_MAX_NUM_FRAME * SBC_MAX_NUM_OF_BLOCKS * 1 * SBC_MAX_NUM_OF_SUBBANDS * 2;
static int16_t pcm_frame_mono[SBC_MAX_NUM_FRAME * SBC_MAX_NUM_OF_BLOCKS * 1 * SBC_MAX_NUM_OF_SUBBANDS] = {0};

static uint16_t sbc_encode_process_mono_data(int16_t *data, uint32_t size, struct enc_codec_buffer *pencoder_buffer,uint8_t *p_frame_num, uint32_t *p_actual_len)
{
	if (size > (uint32_t)max_input_mono_mode_pcm_size_in_bytes) {
		printf("%s : input length max is %d in bytes\r\n", __func__, (int)max_input_mono_mode_pcm_size_in_bytes);
		return 1;
	}
	memset((void *)pcm_frame_mono, 0, max_input_mono_mode_pcm_size_in_bytes);
	memcpy((void *)pcm_frame_mono, (void *)data, size);
	if (!psbc_encoder_state_singleton){
		printf("%s : SBC encoder: sbc state is NULL, call btstack_sbc_encoder_init to initialize it \r\n", __func__);
		return 1;
	}
	SBC_ENC_PARAMS * context = &((bludroid_encoder_state_t *)psbc_encoder_state_singleton->encoder_state)->context;
	context->ps16PcmBuffer = pcm_frame_mono;
	if (context->mSBCEnabled){
		context->pu8Packet[0] = 0xad;
	}
	SBC_Encoder(context);
	*p_actual_len = (uint32_t)sbc_encoder_sbc_buffer_length(); 
	*p_frame_num = (uint8_t)bluedroid_encoder_state.context.u8NumPacketToEncode;
	pencoder_buffer->pbuffer = sbc_encoder_sbc_buffer();
	pencoder_buffer->frame_num = *p_frame_num;
	pencoder_buffer->frame_size = *p_actual_len;

	return 0;
}

static int max_input_dual_mode_pcm_size_in_bytes = SBC_MAX_NUM_FRAME * SBC_MAX_NUM_OF_BLOCKS * SBC_MAX_NUM_OF_CHANNELS * SBC_MAX_NUM_OF_SUBBANDS * 2;
static int16_t pcm_frame_dual[SBC_MAX_NUM_FRAME * SBC_MAX_NUM_OF_BLOCKS * SBC_MAX_NUM_OF_CHANNELS * SBC_MAX_NUM_OF_SUBBANDS] = {0};

static uint16_t sbc_encode_process_dual_data(int16_t *data, uint32_t size, struct enc_codec_buffer *pencoder_buffer,uint8_t *p_frame_num, uint32_t *p_actual_len)
{
	if (size > (uint32_t)max_input_dual_mode_pcm_size_in_bytes) {
		printf("%s : input length max is %d in bytes \r\n", __func__, (int)max_input_dual_mode_pcm_size_in_bytes);
		return 1;
	}
	memset((void *)pcm_frame_dual, 0, max_input_dual_mode_pcm_size_in_bytes);
	memcpy((void *)pcm_frame_dual, (void *)data, size);
	if (!psbc_encoder_state_singleton){
		printf("%s : SBC encoder: sbc state is NULL, call btstack_sbc_encoder_init to initialize it \r\n", __func__);
		return 1;
	}
	SBC_ENC_PARAMS * context = &((bludroid_encoder_state_t *)psbc_encoder_state_singleton->encoder_state)->context;
	context->ps16PcmBuffer = pcm_frame_dual;
	if (context->mSBCEnabled) {
		context->pu8Packet[0] = 0xad;
	}
	SBC_Encoder(context);
	*p_actual_len = (uint32_t)sbc_encoder_sbc_buffer_length(); 
	*p_frame_num = (uint8_t)bluedroid_encoder_state.context.u8NumPacketToEncode;
	pencoder_buffer->pbuffer = sbc_encoder_sbc_buffer();
	pencoder_buffer->frame_num = *p_frame_num;
	pencoder_buffer->frame_size = *p_actual_len;

	return 0;
}

static uint16_t sbc_encoder_process_data(void *pentity, int16_t *data, uint32_t size, struct enc_codec_buffer *pencoder_buffer,uint8_t *p_frame_num, uint32_t *p_actual_len)
{
	(void)pentity;
	uint16_t ret = 1;

	if (bluedroid_encoder_state.context.s16ChannelMode == SBC_CHANNEL_MODE_MONO) {
		ret = sbc_encode_process_mono_data(data, size, pencoder_buffer, p_frame_num, p_actual_len);
	} else {
		ret = sbc_encode_process_dual_data(data, size, pencoder_buffer, p_frame_num, p_actual_len);
	}

	return ret;
}

static int get_bit16(uint16_t bitmap, int position)
{
	return (bitmap >> position) & 1;
}

static uint8_t read_sbc_header(uint8_t *packet, int size, int *offset, avdtp_sbc_codec_header_t *sbc_header)
{
	int sbc_header_len = 12; // without crc
	int pos = *offset;

	if (packet[pos] == 0x9c) {
		DBG_BAD("%s: No Sbc Header \r\n", __func__);
		return 0;
	}

	if (size - pos < sbc_header_len) {
		printf("%s: Not enough data to read SBC header, expected %d, received %d\n", __func__, sbc_header_len, size - pos);
		return 0;
	}
	sbc_header->fragmentation = get_bit16(packet[pos], 7);
	sbc_header->starting_packet = get_bit16(packet[pos], 6);
	sbc_header->last_packet = get_bit16(packet[pos], 5);
	sbc_header->num_frames = packet[pos] & 0x0f;
	pos++;
	*offset = pos;

	DBG_BAD("%s: fragmentation %d, starting_packet %d, last_packet %d, num_frames %d \r\n", __func__, sbc_header->fragmentation, 
																										sbc_header->starting_packet,
																										sbc_header->last_packet,
																										sbc_header->num_frames);

	return 1;
}

static int read_frame_header(uint8_t *packet, int size, int *offset, sbc_frame_header_t *sbc_frame_header)
{
	int sbc_frame_header_len = 4; // without join RFA and scale factors
	int pos = *offset;

	if (size - pos < sbc_frame_header_len) {
		printf("%s: Not enough data to read SBC frame header, expected %d, received %d\n", __func__, sbc_frame_header_len, size - pos);
		return 1;
	}
	sbc_frame_header->syncword = packet[pos];
	sbc_frame_header->sampling_frequency = (packet[pos + 1] >> 6) & 3;
	sbc_frame_header->blocks = (packet[pos + 1] >> 4) & 3;
	sbc_frame_header->channel_mode = (packet[pos + 1] >> 2) & 3;
	sbc_frame_header->allocation_method = (packet[pos + 1] >> 1) & 1;
	sbc_frame_header->subbands = packet[pos + 1] & 1;
	sbc_frame_header->bitpool = packet[pos + 2];
	sbc_frame_header->crc_check = packet[pos + 3];
	pos += 4;
	*offset = pos;

	DBG_BAD("%s: syncword %d, sampling_frequency %d, blocks %d, channel_mode %d, allocation_method %d, subbands %d, bitpool %d, crc_check %d \r\n", 
																										__func__, sbc_frame_header->syncword, 
																										sbc_frame_header->sampling_frequency,
																										sbc_frame_header->blocks,
																										sbc_frame_header->channel_mode,
																										sbc_frame_header->allocation_method,
																										sbc_frame_header->subbands,
																										sbc_frame_header->bitpool,
																										sbc_frame_header->crc_check);

	return 0;
}

static unsigned int caculate_sbc_frame_size(sbc_frame_header_t *psbc_frame_header, struct audio_param *paudio_param)
{
	unsigned int frame_size = 0;
	unsigned int subbands = 0;
	unsigned int channels = 0;
	unsigned int blocks = 0;
	unsigned int bitpool = 0;

	/* param blocks*/
	switch (psbc_frame_header->blocks) {
	case 0: {
		blocks = 4;
	}
	break;
	case 1: {
		blocks = 8;
	}
	break;
	case 2: {
		blocks = 12;
	}
	break;
	case 3: {
		blocks = 16;
	}
	break;
	default:
		break;
	}
	/* param channels*/
	if (psbc_frame_header->channel_mode == 0) {
		channels = 1;
	} else {
		channels = 2;
	}
	/* param subbands*/
	if (psbc_frame_header->subbands == 0) {
		subbands = 4;
	} else {
		subbands = 8;
	}
	/* param bitpool*/
	bitpool = psbc_frame_header->bitpool;
	/* stereo */
	if (psbc_frame_header->channel_mode == 0x2) {
		frame_size = 4 + (4 * subbands * channels)/8;
		float temp = (float)(blocks * bitpool)/8;
		frame_size += (int)(temp + 0.5);
	} else if (psbc_frame_header->channel_mode == 0x3) { /* joint_stereo */
		frame_size = 4 + (4 * subbands * channels)/8;
		float temp = (float)(subbands + blocks * bitpool)/8;
		frame_size += (int)(temp + 0.5);
	} else { /* mono and dual channel */
		frame_size = 4 + (4 * subbands * channels)/8;
		float temp = (float)(blocks * channels * bitpool)/8;
		frame_size += (int)(temp + 0.5);
	}

	/* P audio parameter */
	paudio_param->channels = channels;
	if (channels == 1) {
		paudio_param->channel_allocation = 1;
	} else {
		paudio_param->channel_allocation = 3;
	}
	if (psbc_frame_header->sampling_frequency == 0) {
		paudio_param->rate = 16000;
	} else if (psbc_frame_header->sampling_frequency == 1) {
		paudio_param->rate = 32000;
	} else if (psbc_frame_header->sampling_frequency == 2) {
		paudio_param->rate = 44100;
	} else if (psbc_frame_header->sampling_frequency == 3) {
		paudio_param->rate = 48000;
	}

	return frame_size;
}

static uint16_t sbc_audio_handle_media_data_packet(void *pentity, uint8_t *packet, uint16_t size, uint32_t *pframe_size, uint8_t *pframe_num, uint8_t *pcodec_header_flag, struct audio_param *paudio_param)
{
	(void)pentity;
	avdtp_sbc_codec_header_t sbc_header = {0};
	sbc_frame_header_t sbc_frame_header = {0};
	uint32_t sbc_frame_size = 0;
	int pos = 0;

	/* decode sbc header */
	*pcodec_header_flag = read_sbc_header(packet, size, &pos, &sbc_header);

	/* decode sbc frame header */
	if (read_frame_header(packet, size - pos, &pos, &sbc_frame_header)) {
		printf("%s: read sbc frame header fail \r\n", __func__);
		return 1;
	}

	/* calculate sbc frame size */
	sbc_frame_size = caculate_sbc_frame_size(&sbc_frame_header, paudio_param);

	if (*pcodec_header_flag) {
		/* cause sbc_frame_size maybe float */
		*pframe_size = (uint32_t)(((size - 1) / sbc_header.num_frames)) > sbc_frame_size ? (uint32_t)(((size - 1) / sbc_header.num_frames)) : sbc_frame_size;
		*pframe_num = sbc_header.num_frames;
	} else {
		*pframe_size = sbc_frame_size;
		*pframe_num = 1;
	}

	return 0;
}

static struct dec_codec_buffer *sbc_decoder_buffer_get(void *pentity)
{
	(void)pentity;
	int16_t *ppcm_buffer = NULL;
	struct dec_codec_buffer *pdecoder_buffer = NULL;

	ppcm_buffer = get_sbc_pcm_buffer();
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
	pdecoder_buffer->total_size = (uint32_t)(SBC_MAX_CHANNELS * SBC_MAX_BANDS * SBC_MAX_BLOCKS);
	pdecoder_buffer->actual_write_size = 0;

	return pdecoder_buffer;
}

static void sbc_free_decode_buffer(void *pentity, struct dec_codec_buffer *pdecoder_buffer)
{
	(void)pentity;

	/* release pcm buffer */
	release_pcm_buffer(pdecoder_buffer->pbuffer);
	/* free codec buffer */
	osif_mem_free((void *)pdecoder_buffer);
}

static struct enc_codec_buffer *sbc_encoder_buffer_get(void *pentity)
{
	(void)pentity;
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

static void sbc_free_encode_buffer(void *pentity, struct enc_codec_buffer *pencodec_buffer)
{
	(void)pentity;

	osif_mem_free((void *)pencodec_buffer);
}

static int sbc_decoder_num_samples_per_frame(void *pentity)
{
	(void)pentity;

	if (!psbc_decoder_state_singleton) {
		printf("%s: sbc entity has not been inited \r\n", __func__);
		return 0xFF;
	}
	bludroid_decoder_state_t * decoder_state = (bludroid_decoder_state_t *) psbc_decoder_state_singleton->decoder_state;
	return decoder_state->decoder_context.common.frameInfo.nrof_blocks * decoder_state->decoder_context.common.frameInfo.nrof_subbands;
}

static int sbc_decoder_num_channels(void *pentity)
{
	(void)pentity;

	if (!psbc_decoder_state_singleton) {
		printf("%s: sbc entity has not been inited \r\n", __func__);
		return 0xFF;
	}
	bludroid_decoder_state_t * decoder_state = (bludroid_decoder_state_t *) psbc_decoder_state_singleton->decoder_state;
	return decoder_state->decoder_context.common.frameInfo.nrof_channels;
}

static int sbc_decoder_sample_rate(void *pentity)
{
	(void)pentity;

	if (!psbc_decoder_state_singleton) {
		printf("%s: sbc entity has not been inited \r\n", __func__);
		return 0xFF;
	}
	bludroid_decoder_state_t * decoder_state = (bludroid_decoder_state_t *) psbc_decoder_state_singleton->decoder_state;
	return decoder_state->decoder_context.common.frameInfo.frequency;
}

uint16_t rtk_bt_audio_sbc_register(uint32_t type, PAUDIO_CODEC_ENTITY p_entity)
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
	p_entity->init = sbc_codec_init;
	p_entity->deinit = sbc_codec_deinit;
	p_entity->bt_audio_handle_media_data_packet = sbc_audio_handle_media_data_packet;
	p_entity->encoding_func = sbc_encoder_process_data;
	p_entity->decoding_func = sbc_decoder_process_data;
	p_entity->get_decode_buffer = sbc_decoder_buffer_get;
	p_entity->free_decode_buffer = sbc_free_decode_buffer;
	p_entity->get_encode_buffer = sbc_encoder_buffer_get;
	p_entity->free_encode_buffer = sbc_free_encode_buffer;
	p_entity->decoder_num_samples_per_frame = sbc_decoder_num_samples_per_frame;
	p_entity->decoder_num_channels = sbc_decoder_num_channels;
	p_entity->decoder_sample_rate = sbc_decoder_sample_rate;
	p_entity->encode_lock = false;
	p_entity->decode_lock = false;
	memset((void *)&p_entity->sbc, 0, sizeof(p_entity->sbc));
	osif_unlock(lock_flag);
	ret = 0;

	return ret;
}