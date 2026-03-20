/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/*******************************************************************************
* Modifications:
* Copyright 2026 ABOV Semiconductor Co., Ltd.
*
* This file has been modified by ABOV Semiconductor.
*
* Modifications include:
*   - Implementing the interface logic for the ADAM110
*
* Modifications by ABOV Semiconductor are licensed under the BSD-3-Clause license.
* The original portions of this file remain licensed under the Apache License, Version 2.0.
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation and/or
* other materials provided with the distribution.
* 
 * 3. Neither the name of the copyright holder nor the names of its contributors
* may be used to endorse or promote products derived from this software without
* specific prior written permission.
* 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
******************************************************************************/
#ifndef __DRIVERS_AUDIO_ADAM110_H
#define __DRIVERS_AUDIO_ADAM110_H

#include <tinyara/audio/audio.h>
#include <semaphore.h>
#include <queue.h>

#include <tinyara/pm/pm.h>

//#define USE_HOST_RINGBUFFER
#define USE_DIRECT_APB
#if defined(USE_HOST_RINGBUFFER) && defined(USE_DIRECT_APB)
#error "USE_HOST_RINGBUFFER and USE_DIRECT_APB cannot be enabled together"
#endif

#define ADAM110_MIC_GAIN_MAX		255
#define ADAM110_MIC_GAIN_DEFAULT	128
#define ADAM110_DEFAULT_SENSITIVITY	0xAAAA
#define ADAM110_DEFAULT_SAMPLE_SIZE 3840

#define ADMA110_DIS_OFF				0x00
#define ADAM110_EN_ON				0x01

#define ADAM110_RETRY_CNT			1
#define ADAM110_HW_RST_WAIT			(50 * 1000)	/* Almost 30 ~ 50 msec */
#define ADAM110_TXRX_DELAY			20 /* between tx and rx delay 20usec */ 

#define ADAM110_RX_MAX_SIZE			3840
#define ADAM110_SEAMLESS_DATA_BLK	17
#define ADAM110_KEYWORD_DATA_SIZE	64000
#define ADAM110_MODEL_CHUNK_SIZE	256
#define ADAM110_MODEL_RETRY_CNT		100

#define ADAM110_FW_CHUNK_SIZE		128
#define ADAM110_FW_RETRY_CNT		10
#define ADAM110_FW_HEADER_SIZE		16
#define ADAM110_FW_VENDOR_ID		0xAB
#define ADAM110_FW_ERASE_WAITTIME	(2*1000*1000) /* Min 2 sec */
#define ADAM110_FW_UPDATE_WAITTIME  100 /* Min 100us */
#define ADAM110_FW_WRITE_WAITTIME   (4*1000) /* Min 4ms */
#define ADAM110_FW_CHKSUM_WAITTIME  (100*1000) /* Min 100ms */
#define ADAM110_ALIVENESS_CHECK_PERIOD_US	(3*1000*1000) /* 3 sec */
#define ADAM110_FW_BOOT_MODE_OK		0xB004C0DE
#define ADAM110_FW_CHKSUM_FAIL		0xDE
#define ADAM110_FW_DATA_HEADER		2

#define ADAM110_FW_HEADER_VENDOR_OFFSET			0
#define ADAM110_FW_HEADER_HWID_OFFSET			1
#define ADAM110_FW_HEADER_VER_MAJOR_OFFSET		2
#define ADAM110_FW_HEADER_VER_MINOR_OFFSET		3
#define ADAM110_FW_HEADER_VER_PATCH_H_OFFSET	4
#define ADAM110_FW_HEADER_VER_PATCH_L_OFFSET	5
#define ADAM110_FW_HEADER_CHKSUM_H_OFFSET		6
#define ADAM110_FW_HEADER_CHKSUM_L_OFFSET		7

#define ADAM110_FLASH_START_ADDR	0x2000
#define ADAM110_FLASH_END_ADDR		0xFFFF
#define ADAM110_FLASH_TOTAL_SIZE	(ADAM110_FLASH_END_ADDR - ADAM110_FLASH_START_ADDR + 1)

/* Protocol define */
#define PKT_HEADER_SEND				0xAC
#define PKT_HEADER_RECV				0x5C

/* Device Management */
#define DM_RESET					0x00 /* Device Reset */
#define DM_GET_STATE				0x01 /* Get Device State */
#define DM_GET_EVENT				0x02 /* Get Event Data */
#define DM_GET_KEEP_ALIVE			0x03 /* Get Keep alive State */
#define DM_ENTER_LP_MODE			0x04 /* Enter Low Power Mode */

/* Audio Control */
#define AUD_I2S_SET_CONFIG			0x20 /* Set I2S Audio Configuration */
#define AUD_I2S_SET_STATE			0x21 /* Set Audio Output Enable / Disable */
#define AUD_I2S_AEC_CTRL			0x22 /* Set AEC On/Off */
#define AUD_PDM_SET_GAIN			0x30 /* Set PDM Gain */
#define AUD_GET_SEAMLESS_DATA		0x40 /* Get seamless audio data */
#define AUD_GET_PREP_DATA			0x41 /* Get NPU preprocessed audio data */
#define AUD_GET_MIC_DATA			0x42 /* */
#define AUD_GET_I2S_DATA			0x43 /* */
#define AUD_SET_DEBUG_MODE			0x44 /* Audio Debug Mode Enable/Disable */
#define AUD_INT_EN					0x45 /* Audio Interrupt Enable/Disable */

/* AI Control */
#define AC_UPDATE_MODEL				0x60 /* Set AI Model Update Mode */
#define AC_UPDATE_CHK_XMIT			0x61 /* Check AI Model Data Transmit */
#define AC_UPDATE_RSLT				0x62 /* Get AI Model Update Result */
#define AC_THD_ADJ					0x63 /* Set AI Model Threshold Adjust */
#define AC_PARA_TUNING				0x64 /* Set AI Model Parameter Tuning */
#define AC_GET_MODEL_NAME			0x65 /* Get AI model name */
#define AC_INT_EN					0x66 /* AI model Interrupt Enable/Disable */

/* Firmware */
#define FW_GET_VER					0xE0 /* Get FW Version */
#define FW_ENTER_BOOT_MODE			0xE1 /* Enter BootMode */
#define FW_CHK_BOOT_MODE			0xE2 /* Boot Status Read */
#define FW_ERASE					0xE3 /* Flash Erase */
#define FW_UPDATE					0xE4 /* Firmware write start */
#define FW_GET_UPDATE_DATA			0xE5 /* Firmware data packet */
#define FW_UPDATE_RSLT_CAL			0xE6 /* Firmware checksum calculation execute */
#define FW_UPDATE_RSLT				0xE7 /* Firmware checksum read */

/* Error code */
#define RSLT_SUCCESS				0x00 /* Success */
#define RSLT_ERROR					0x01 /* General Error */
#define RSLT_BAD_PARAM				0x02 /* Bad Oaraneters Error */
#define RSLT_BUF_FULL				0x03 /* Error caused by Buffer Fall */
#define RSLT_CHKSUM_ERR				0x04 /* Checksum Error */
#define RSLT_SYNC_ERR				0x05 /* Sync Byte Error */
#define RSLT_INTERNAL_ERR			0x06 /* Internal Error */

#ifdef USE_HOST_RINGBUFFER
#define ADAM110_PCM_SAMPLE_RATE			16000 //16k
#define ADAM110_PCM_BYTES_PER_SAMPLE	2     // 16bit
#define ADAM110_PCM_SECONDS				4     // 4sec
#define ADAM110_PCM_RING_SIZE_4SEC		(ADAM110_PCM_SAMPLE_RATE * ADAM110_PCM_BYTES_PER_SAMPLE * ADAM110_PCM_SECONDS)
#endif

typedef struct {
	uint8_t header;
	uint8_t op; 	
	uint8_t parm1;
	uint8_t parm2;
	uint8_t parm3;
	uint8_t parm4;
	uint8_t checksum;
} t_proto_pkt;

/* I2S Control bit */
typedef enum {
	I2S_DATA_WIDTH_8 = 0,		/* 8bit */
	I2S_DATA_WIDTH_16,			/* 16bit */
	I2S_DATA_WIDTH_24,			/* 24bit */
	I2S_DATA_WIDTH_32,			/* 32bit */
	I2S_DATA_WIDTH_MAX
} i2s_data_width_t;

typedef enum {
	I2S_FRAME_SIZE_8 = 0,		/* 8bit */
	I2S_FRAME_SIZE_16,			/* 16bit */
	I2S_FRAME_SIZE_24,			/* 24bit */
	I2S_FRAME_SIZE_32,			/* 32bit */
	I2S_FRAME_SIZE_MAX
} i2s_frame_size_t;

typedef enum {
	I2S_SAMPLE_RATE_16K = 0,	/* 16KHz */
	I2S_SAMPLE_RATE_48K,		/* 48KHz */
	I2S_SAMPLE_RATE_MAX
} i2s_sample_rate_t;

typedef enum {
	I2S_STREAM_TYPE_PB = 0,		/* Playback */
	I2S_STREAM_TYPE_CAP,		/* Capture */
	I2S_STREAM_TYPE_MAX
} i2s_stream_type_t;

/* AI Model */
typedef enum {
	AI_MODEL_HIBIXBY	= 1,	/* Hi-Bixby */
	AI_MODEL_BIXBY,				/* Bixby */
	AI_MODEL_ALEAX,				/* Alexa */
	AI_MODEL_1,					/* Model 1 reserved */
	AI_MODEL_2,					/* Model 2 reserved */
	AI_MODEL_3,					/* Model 3 reserved */
	AI_MODEL_4,					/* Model 3 reserved */
	AI_MODEL_SEAMLESS,			/* Seamless audio data ready */
	AI_MODEL_AUDIO,				/* RT audio data ready */
	AI_MODEL_MAX
} ai_model_t;

/* AI Data */
typedef enum {
	AI_DATA_TYPE_SEAMLESS = 1,		/* Keyword Detect Seamless data */
	AI_DATA_TYPE_AUDIO = 	2,		/* Real Audio data */
	AI_DATA_TYPE_MAX
} ai_data_type_t;

typedef enum {
	AI_MODEL_THD_LOW = 0,
	AI_MODEL_THD_MID,
	AI_MODEL_THD_HIGH,
	AI_MODEL_THD_MAX
} ai_model_thd_t;

#ifdef CONFIG_AUDIO_SPEECH_DETECT_FEATURES
enum speech_state_e {
	ADAM110_SPI_SPEECH_STATE_NONE = 0,
#ifdef CONFIG_AUDIO_KEYWORD_DETECT
	ADAM110_SPI_SPEECH_STATE_KD = 2,
#endif
};
typedef enum speech_state_e speech_state_t;
#endif

struct adam110_dev_s {
	struct audio_lowerhalf_s dev;	/* Audio lower half (this device) */
	struct adam110_lower_s *lower;
	mqd_t mq;					/* Message queue for receiving messages */
	char mqname[16];			/* Our message queue name */
#ifndef CONFIG_AUDIO_EXCLUDE_GAIN
	uint16_t mic_gain;
#endif
	bool mute;
	struct sq_queue_s pendq;
	sem_t devsem;
	bool running;
	bool recording;
	bool reserved;
	bool init_done;

	FAR struct spi_dev_s *spi;

#ifdef CONFIG_AUDIO_KEYWORD_DETECT
	uint32_t keyword_bytes;
	uint32_t keyword_bytes_left;
	bool kd_enabled;
	uint8_t *keyword_buffer;
#endif

	uint32_t sample_size;
	volatile bool fw_loaded;
	uint16_t sensitivity;
	uint32_t total_size;

#ifdef CONFIG_PM
	struct pm_domain_s *pm_domain;
#endif

#ifdef CONFIG_AUDIO_SPEECH_DETECT_FEATURES
	volatile speech_state_t speech_state;
#ifdef CONFIG_AUDIO_KEYWORD_DETECT
	volatile bool keyword_detect;
#endif
#endif

#ifdef USE_HOST_RINGBUFFER
    /* Host PCM Ring Buffer */
    uint8_t  *pcm_ring;
    uint32_t  pcm_ring_size;
    uint32_t  pcm_w;
    uint32_t  pcm_r;
    uint32_t  pcm_fill;

    /* overflow debug stats */
    uint32_t pcm_ovf_count;
    uint32_t pcm_drop_bytes_total;
#endif
#if defined(USE_HOST_RINGBUFFER) || defined(USE_DIRECT_APB)
    /* apb wait queue */
    struct sq_queue_s pcm_apb_waitq;

    /* protection */
    sem_t pcmsem;
#endif
};

static struct work_s adam110_work; /* workqueen handle */
static char s_rxsmlbuf[ADAM110_RX_MAX_SIZE*ADAM110_SEAMLESS_DATA_BLK]; /* seamless pre-buffer */
static uint8_t s_temp_chunk[ADAM110_RX_MAX_SIZE+1]; /* 3840 + 1(checksum) */

static const char *mcu_package = "/res/kernel/audio/mcu_fw";
static const char *dsp_package = "/res/kernel/audio/dsp_fw";

#endif				/* __DRIVERS_AUDIO_ADAM110_H */
