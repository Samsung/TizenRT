/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/
#ifndef __AAC_CODEC_ENTITY_H__
#define __AAC_CODEC_ENTITY_H__

#include <dlist.h>
#include <bt_audio_codec_wrapper.h>
#include <bt_audio_config.h>

#define AAC_MAX_CHANNEL_COUNT 1
#define AAC_DECODE_MAX_OUT_BUFFER_SIZE 2048 * AAC_MAX_CHANNEL_COUNT
#define AAX_ENCODE_MAX_OUT_BUFFER_SIZE 2048

#if defined(AUDIO_STREAM_MAX_PCM_DATA_NUM) && AUDIO_STREAM_MAX_PCM_DATA_NUM
#define MAX_AAC_DECODER_NUM AUDIO_STREAM_MAX_PCM_DATA_NUM
#else
#define MAX_AAC_DECODER_NUM 20
#endif

/**
 * @typedef   rtk_bt_aac_codec_transport_type
 * @brief     advanced audio codec transport type
 */
typedef enum {
	RTK_AAC_TT_UNKNOWN       = -1,   /**< Unknown format.            */
	RTK_AAC_TT_MP4_RAW       =  0,   /**< "as is" access units (packet based since there is obviously no sync layer) */
	RTK_AAC_TT_MP4_ADIF      =  1,   /**< ADIF bitstream format.     */
	RTK_AAC_TT_MP4_ADTS      =  2,   /**< ADTS bitstream format.     */
	RTK_AAC_TT_MP4_LATM_MCP1 =  6,   /**< Audio Mux Elements with muxConfigPresent = 1 */
	RTK_AAC_TT_MP4_LATM_MCP0 =  7,   /**< Audio Mux Elements with muxConfigPresent = 0, out of band StreamMuxConfig */
	RTK_AAC_TT_MP4_LOAS      =  10,  /**< Audio Sync Stream.         */
	RTK_AAC_TT_DRM           =  12   /**< Digital Radio Mondial (DRM30/DRM+) bitstream format. */
} rtk_bt_aac_codec_transport_type;

/**
 * @typedef   rtk_bt_aac_codec_transport_type
 * @brief     advanced audio codec transport type
 */
typedef enum {
	RTK_AAC_AOT_AAC_LC       = 2,    /**< MPEG-4 AAC LC */
	RTK_AAC_AOT_AAC_LTP      = 4,    /**< MPEG-4 AAC LTP */
	RTK_AAC_AOT_AAC_SCAL     = 6,    /**< MPEG-4 AAC scalable */
	/* Pseudo AOTs */
	RTK_AAC_AOT_MP2_AAC_LC   = 129,  /**< MPEG-2 AAC LC */
} rtk_bt_aac_codec_object_type;

struct aac_pcm_data {
	struct list_head list;
	int16_t aac_data[AAC_DECODE_MAX_OUT_BUFFER_SIZE];
};

/**
 * @typedef	rtk_bt_aac_encode_t
 * @brief	structure of aac encoder init
 */
typedef struct {
	rtk_bt_aac_codec_object_type			object_type;			/*!< aac object type */
	int										sample_freq;			/*!< aac sample frequency */
	int										channels;				/*!< aac channels */
	int										bit_rate;				/*!< aac bit rate */
	uint8_t									vbr;					/*!< aac support for variable bit rate */
} rtk_bt_aac_encode_t;

/**
 * @typedef	rtk_bt_aac_decode_t
 * @brief	structure of aac decoder init
 */
typedef struct {
	rtk_bt_aac_codec_object_type			object_type;			/*!< aac object type */
	int										sample_freq;			/*!< aac sample frequency */
	int										channels;				/*!< aac channels */
	int										bit_rate;				/*!< aac bit rate */
	uint8_t									vbr;					/*!< aac support for variable bit rate */
} rtk_bt_aac_decode_t;

/**
 * @typedef	rtk_bt_aac_codec_t
 * @brief	structure of aac codec init
 */
typedef struct {
	rtk_bt_aac_encode_t encoder_t;									/*!< encoder structure */
	rtk_bt_aac_decode_t decoder_t;									/*!< decoder structure */
} rtk_bt_aac_codec_t;

uint16_t rtk_bt_audio_aac_register(uint32_t type, PAUDIO_CODEC_ENTITY p_entity);

#endif /* __AAC_CODEC_ENTITY_H__ */