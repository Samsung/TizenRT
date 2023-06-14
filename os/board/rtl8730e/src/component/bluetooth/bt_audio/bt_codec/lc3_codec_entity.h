/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/
#ifndef __LC3_CODEC_ENTITY_H__
#define __LC3_CODEC_ENTITY_H__

#include <dlist.h>
#include <bt_audio_codec_wrapper.h>
#include <bt_audio_config.h>

#define LC3_MAX_PCM_SIZE 2 * 480 * 32 / 8 //channels * MaxFrameSize * bit_per_sample_encode(max = 32)/8 bytes

#if defined(AUDIO_STREAM_MAX_PCM_DATA_NUM) && AUDIO_STREAM_MAX_PCM_DATA_NUM
#define MAX_LC3_DECODER_NUM AUDIO_STREAM_MAX_PCM_DATA_NUM
#else
#define MAX_LC3_DECODER_NUM 40
#endif

#define LC3_AUDIO_LOCATION_MONO                0x00000000
#define LC3_AUDIO_LOCATION_FL                  0x00000001
#define LC3_AUDIO_LOCATION_FR                  0x00000002

struct lc3_pcm_data {
	struct list_head list;
	int16_t pcm_data[LC3_MAX_PCM_SIZE / 2];
};

struct audio_sample_option {
	uint8_t sample_rate_index;
	int sample_rate;
};

/**
 * @typedef	rtk_bt_le_audio_lc3_cfg_t
 * @brief	structure of le audio configuration
 */
typedef struct {
	uint8_t  frame_duration;
	uint8_t  sample_frequency;
	uint8_t  codec_frame_blocks_per_sdu;
	uint16_t octets_per_codec_frame;
	uint32_t audio_channel_allocation;
	int      compress_bytes;
} rtk_bt_le_audio_lc3_cfg_t;

uint16_t rtk_bt_audio_lc3_register(uint32_t type, PAUDIO_CODEC_ENTITY p_entity);

#endif /* __LC3_CODEC_ENTITY_H__ */