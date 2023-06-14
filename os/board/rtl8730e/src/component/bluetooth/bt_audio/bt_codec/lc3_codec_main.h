/******************************************************************************
 *
 * Copyright(c) 2020 - 2021 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
#ifndef _LC3_CODEC_MAIN_H_
#define _LC3_CODEC_MAIN_H_

typedef struct {
	void 		*decoder;
	void 		*encoder;
	void 		*config;
	void 		*points;
	int16_t 	pcm_buffer[1920]; //LC3_MAX_PCM_SIZE/2 = 
	uint8_t 	*p_encode_out_buffer;
	int 		FrameSize;
	int 		channels;
	int 		bit_per_sample_decode;
	int 		bit_per_sample_encode;
	int 		compress_bytes;
} lc3_codec_t;

void *audio_lc3_codec_cfg(int sample_rate, uint8_t frame_duration, uint8_t channel_num, uint8_t bits_per_audio_sample_dec_, uint8_t bits_per_audio_sample_enc_, int compress_bytes);
uint16_t lc3_decoder_reset(void *p_lc3_t);
void audio_lc3_decode(void *p_lc3_t, uint8_t *data, uint32_t size, int16_t *ppcm_buffer, uint32_t *ppcm_size);
uint16_t audio_lc3_encode(void *p_lc3_t, int16_t *data, uint32_t size, uint8_t **ppout_buffer, uint8_t *p_frame_num, uint32_t *p_actual_len);

#endif /* _LC3_CODEC_MAIN_H_ */