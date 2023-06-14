#ifndef AMEBA_COMPONENT_AUDIO_AUDIO_HAL_RTL8726E_AMEBA_AUDIO_STREAM_H
#define AMEBA_COMPONENT_AUDIO_AUDIO_HAL_RTL8726E_AMEBA_AUDIO_STREAM_H

#include "platform_stdlib.h"
#include "ameba_rcc.h"
#include "ameba_sport.h"
#include "ameba_audio.h"
#include "ameba_audio_stream_buffer.h"
#include "hardware/audio/audio_hw_types.h"
#include "osdep_service.h"

#ifdef __cplusplus
extern "C" {
#endif

#define STREAM_OUT     0
#define STREAM_IN      1

#define MAX_AD_NUM     8
#define MAX_AMIC_NUM   5

#define TX_SPORT0_IN_USE    ((uint32_t)0x00000001 << 0)
#define TX_SPORT1_IN_USE    ((uint32_t)0x00000001 << 1)
#define TX_CODEC_IN_USE     ((uint32_t)0x00000001 << 2)
#define TX_CLK_IN_USE       ((uint32_t)0x00000001 << 3)
#define TX_POWER_IN_USE     ((uint32_t)0x00000001 << 4)
#define TX_AUDIO_IP_IN_USE  ((uint32_t)0x00000001 << 5)
#define TX_I2S0_IN_USE      ((uint32_t)0x00000001 << 6)
#define TX_I2S1_IN_USE      ((uint32_t)0x00000001 << 7)
#define TX_ANAL_IN_USE      ((uint32_t)0x00000001 << 8)
#define TX_ANAR_IN_USE      ((uint32_t)0x00000001 << 9)
#define TX_HPO_ANAL_IN_USE  ((uint32_t)0x00000001 << 10)
#define TX_HPO_ANAR_IN_USE  ((uint32_t)0x00000001 << 11)

#define RX_SPORT0_IN_USE    ((uint32_t)0x00000001 << 16)
#define RX_SPORT1_IN_USE    ((uint32_t)0x00000001 << 17)
#define RX_CODEC_IN_USE     ((uint32_t)0x00000001 << 18)
#define RX_CLK_IN_USE       ((uint32_t)0x00000001 << 19)
#define RX_POWER_IN_USE     ((uint32_t)0x00000001 << 20)
#define RX_AUDIO_IP_IN_USE  ((uint32_t)0x00000001 << 21)
#define RX_I2S0_IN_USE      ((uint32_t)0x00000001 << 22)
#define RX_I2S1_IN_USE      ((uint32_t)0x00000001 << 23)

enum {
	AUDIOIP       = 0,
	POWER         = 1,
	SPORT0        = 2,
	SPORT1        = 3,
	CODEC         = 4,
	IIS0          = 5,
	IIS1          = 6,
	LINEOUTLANA   = 7,
	LINEOUTRANA   = 8,
	HPOOUTLANA    = 9,
	HPOOUTRANA    = 10,
};

typedef struct _GdmaCallbackData GdmaCallbackData;

typedef struct _StreamConfig {
	uint32_t channels;
	uint32_t format;
	uint32_t rate;
	uint32_t frame_size;
	uint32_t period_size;
	uint32_t period_count;
	uint32_t mode;
} StreamConfig;

typedef struct _Stream {
	StreamConfig config;
	AudioBuffer *rbuffer;
	GdmaCallbackData *gdma_struct;
	SP_InitTypeDef sp_initstruct;
	I2S_InitTypeDef i2s_initstruct;
	uint32_t sport_dev_num;
	uint32_t direction;
	uint32_t frame_size;
	uint32_t period_count;
	uint32_t period_bytes;
	uint32_t rate;
	uint32_t gdma_cnt;
	uint32_t gdma_irq_cnt;
	_sema sem;
	bool sem_need_post;
	bool start_gdma;
	uint32_t stream_mode;
	struct GDMA_CH_LLI *gdma_ch_lli;
} Stream;

typedef struct _GdmaCallbackData {
	uint32_t gdma_id; //0: dma for fifo0, 1: dma for fio1
	Stream *stream;
	union {
		GDMA_InitTypeDef SpTxGdmaInitStruct;  //Pointer to GDMA_InitTypeDef
		GDMA_InitTypeDef SpRxGdmaInitStruct;  //Pointer to GDMA_InitTypeDef
	} u;
} GdmaCallbackData;

void ameba_audio_periphclock_init(uint32_t sport_num);
void ameba_audio_set_audio_ip_use_status(uint32_t direction, uint32_t audio_ip, bool is_used);
bool ameba_audio_is_audio_ip_in_use(uint32_t audio_ip);
void ameba_audio_reset_audio_ip_status(Stream *stream);

#ifdef __cplusplus
}
#endif

#endif