/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/
#ifndef __SBC_CODEC_ENTITY_H__
#define __SBC_CODEC_ENTITY_H__

#include <dlist.h>
#include <oi_status.h>
#include <oi_codec_sbc.h>
#include <sbc_encoder.h>
#include <bt_audio_codec_wrapper.h>
#include <bt_audio_config.h>

#define RTK_A2DP_SINK 0x01
#define SBC_MAX_CHANNELS 2
#define SBC_MAX_BANDS 8
#define SBC_MAX_BLOCKS 16
#define SBC_FS 120          /* SBC Frame Size */
#define SBC_N 512           /* 32ms - Window Length for pattern matching */
#define SBC_M 64            /* 4ms - Template for matching */
#define SBC_LHIST (SBC_N+SBC_FS-1)  /* Length of history buffer required */
#define SBC_RT 36           /* SBC Reconvergence Time (samples) */
#define SBC_OLAL 16         /* OverLap-Add Length (samples) */
#define mSBC_SYNCWORD 0xad
#define SBC_SYNCWORD 0x9c
#define DECODER_DATA_SIZE (SBC_MAX_CHANNELS*SBC_MAX_BLOCKS*SBC_MAX_BANDS * 4 + SBC_CODEC_MIN_FILTER_BUFFERS*SBC_MAX_BANDS*SBC_MAX_CHANNELS * 2)

#if defined(AUDIO_STREAM_MAX_PCM_DATA_NUM) && AUDIO_STREAM_MAX_PCM_DATA_NUM
#define MAX_SBC_DECODER_NUM AUDIO_STREAM_MAX_PCM_DATA_NUM
#else
#define MAX_SBC_DECODER_NUM 60
#endif

typedef enum {
	SBC_MODE_STANDARD = 0,
	SBC_MODE_mSBC
} sbc_mode_t;

typedef enum {
	SBC_ALLOCATION_METHOD_LOUDNESS = 0,
	SBC_ALLOCATION_METHOD_SNR
} sbc_allocation_method_t;

typedef enum {
	SBC_CHANNEL_MODE_MONO = 0,
	SBC_CHANNEL_MODE_DUAL_CHANNEL,
	SBC_CHANNEL_MODE_STEREO,
	SBC_CHANNEL_MODE_JOINT_STEREO
} sbc_channel_mode_t;

struct sbc_pcm_data {
	struct list_head list;
	int16_t pcm_data[SBC_MAX_CHANNELS * SBC_MAX_BANDS * SBC_MAX_BLOCKS];
};

/**
 * @typedef	rtk_bt_sbc_encode_t
 * @brief	structure of sbc encoder init
 */
typedef struct {
	sbc_channel_mode_t		sbc_enc_mode;			/*!< sbc encode mode */
	int						blocks;					/*!< sbc blocks */
	int						subbands;				/*!< sbc subbands */
	sbc_allocation_method_t	alloc_method;			/*!< sbc alloc method */
	int						sample_rate;			/*!< sbc sample rate */
	int						bitpool;				/*!< sbc bitpool */
	sbc_channel_mode_t		channel_mode;			/*!< sbc channel mode */
} rtk_bt_sbc_encode_t;

/**
 * @typedef	rtk_bt_sbc_decode_t
 * @brief	structure of sbc decoder init
 */
typedef struct {
	sbc_channel_mode_t		sbc_dec_mode;			/*!< sbc decode mode */
	uint32_t 				sampling_frequency;		/*!< indicate sample frequence */
	uint8_t 				channel_mode;			/*!< indicate channel mode */
	uint8_t 				block_length;			/*!< indicate block length */
	uint8_t 				subbands;				/*!< indicate subbands */
	uint8_t 				allocation_method;		/*!< indicate allocation method */
	uint8_t 				min_bitpool;			/*!< indicate min bitpool */
	uint8_t 				max_bitpool;			/*!< indicate max bitpool */
} rtk_bt_sbc_decode_t;

/**
 * @typedef	rtk_bt_sbc_codec_t
 * @brief	structure of sbc codec init
 */
typedef struct {
	rtk_bt_sbc_encode_t encoder_t;									/*!< encoder structure */
	rtk_bt_sbc_decode_t decoder_t;									/*!< decoder structure */
} rtk_bt_sbc_codec_t;

typedef struct sbc_plc_state {
	int16_t					hist[SBC_LHIST + SBC_FS + SBC_RT + SBC_OLAL];
	int16_t					bestlag;
	int						nbf;
	int						good_frames_nr;
	int						bad_frames_nr;
	int						frame_count;
	int						max_consecutive_bad_frames_nr;
} sbc_plc_state_t;

typedef struct {
	void					*context;
	void					*decoder_state;
	sbc_plc_state_t			plc_state;
	sbc_mode_t				mode;
	int						good_frames_nr;
	int						bad_frames_nr;
	int						zero_frames_nr;
} sbc_decoder_state_t;

typedef struct {
	void					*encoder_state;
	sbc_mode_t				mode;
} sbc_encoder_state_t;

typedef struct {
	OI_UINT32						bytes_in_frame_buffer;
	OI_CODEC_SBC_DECODER_CONTEXT	decoder_context;
	int16_t							pcm_plc_data[SBC_MAX_CHANNELS * SBC_MAX_BANDS * SBC_MAX_BLOCKS];
	int16_t							pcm_data[SBC_MAX_CHANNELS * SBC_MAX_BANDS * SBC_MAX_BLOCKS];
	uint32_t						pcm_bytes;
	OI_UINT32						decoder_data[(DECODER_DATA_SIZE + 3) / 4];
	int								first_good_frame_found;
	int								h2_sequence_nr;
	uint16_t						msbc_bad_bytes;
} bludroid_decoder_state_t;

typedef struct {
	SBC_ENC_PARAMS					context;
	int								num_data_bytes;
	uint8_t							sbc_packet[1000];
} bludroid_encoder_state_t;

typedef struct {
	uint8_t							fragmentation;
	uint8_t							starting_packet; // of fragmented SBC frame
	uint8_t							last_packet;     // of fragmented SBC frame
	uint8_t							num_frames;
} avdtp_sbc_codec_header_t;

typedef struct {
	uint8_t							syncword;
	uint8_t							sampling_frequency: 2;
	uint8_t							blocks: 2;
	uint8_t							channel_mode: 2;
	uint8_t							allocation_method: 1;
	uint8_t							subbands: 1;
	uint8_t							bitpool;
	uint8_t							crc_check;
} sbc_frame_header_t;

uint16_t rtk_bt_audio_sbc_register(uint32_t type, PAUDIO_CODEC_ENTITY p_entity);

#endif /* __SBC_CODEC_ENTITY_H__ */