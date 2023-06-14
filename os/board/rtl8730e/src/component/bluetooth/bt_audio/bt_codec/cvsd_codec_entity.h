/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/
#ifndef __CVSD_CODEC_ENTITY_H__
#define __CVSD_CODEC_ENTITY_H__

#include <dlist.h>
#include <bt_audio_codec_wrapper.h>
#include <bt_audio_config.h>

#define CVSD_FRAME_SIZE 256

/**
 * @typedef	rtk_bt_cvsd_encode_t
 * @brief	structure of cvsd encoder init
 */
typedef struct {
	uint8_t rsvd;								/*1< for future use */
} rtk_bt_cvsd_encode_t;

/**
 * @typedef	rtk_bt_cvsd_decode_t
 * @brief	structure of cvsd decoder init
 */
typedef struct {
	uint8_t 	channel_num;					/*!< indicate channel number */
	int 		sample_rate;					/*!< indicate sample frequence */
	uint8_t 	frame_duration;					/*!< indicate frame duration */
} rtk_bt_cvsd_decode_t;

/**
 * @typedef	rtk_bt_cvsd_codec_t
 * @brief	structure of cvsd codec init
 */
typedef struct {
	rtk_bt_cvsd_encode_t encoder_t;									/*!< encoder structure */
	rtk_bt_cvsd_decode_t decoder_t;									/*!< decoder structure */
} rtk_bt_cvsd_codec_t;

uint16_t rtk_bt_audio_cvsd_register(uint32_t type, PAUDIO_CODEC_ENTITY p_entity);

#endif /* __CVSD_CODEC_ENTITY_H__ */