// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#pragma once

#include <driver/int_types.h>
#include <common/bk_include.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef void (*i2s_isr_t)(void *param);


/**
 * @brief I2S defines
 * @defgroup bk_api_i2s_defs macos
 * @ingroup bk_api_i2s
 * @{
 */

#define BK_ERR_I2S_NOT_INIT			(BK_ERR_I2S_BASE - 1) /**< I2S driver not init */
#define BK_ERR_I2S_PARAM			(BK_ERR_I2S_BASE - 2) /**< I2S parameter invalid */
#define BK_ERR_I2S_ISR_ID			(BK_ERR_I2S_BASE - 3) /**< I2S isr id invalid */

/**
 * @}
 */

/**
 * @brief I2S enum defines
 * @defgroup bk_api_i2s_enum I2S enums
 * @ingroup bk_api_i2s
 * @{
 */
typedef enum {
	I2S_GPIO_GROUP_0 = 0, /**< I2S gpio group 0: GPIO6, GPIO7, GPIO8, GPIO9 */
	I2S_GPIO_GROUP_1,     /**< I2S gpio group 1: GPIO40, GPIO41, GPIO42, GPIO43 */
	I2S_GPIO_GROUP_2,     /**< I2S gpio group 2: GPIO44, GPIO45, GPIO46, GPIO47 */
	I2S_GPIO_GROUP_MAX
} i2s_gpio_group_id_t;


typedef enum {
	I2S_ISR_CHL1_TXUDF = 0, /**< I2S channel1 tx_udf interrupt */
	I2S_ISR_CHL1_RXOVF,	    /**< I2S channel1 rx_ovf interrupt */
	I2S_ISR_CHL1_TXINT,	    /**< I2S channel1 tx interrupt */
	I2S_ISR_CHL1_RXINT,	    /**< I2S channel1 rx interrupt */

	I2S_ISR_CHL2_TXUDF, /**< I2S channel2 tx_udf interrupt */
	I2S_ISR_CHL2_RXOVF, /**< I2S channel2 rx_ovf interrupt */
	I2S_ISR_CHL2_TXINT, /**< I2S channel2 tx interrupt */
	I2S_ISR_CHL2_RXINT, /**< I2S channel2 rx interrupt */

	I2S_ISR_CHL3_TXUDF, /**< I2S channel3 tx_udf interrupt */
	I2S_ISR_CHL3_RXOVF, /**< I2S channel3 rx_ovf interrupt */
	I2S_ISR_CHL3_TXINT, /**< I2S channel3 tx interrupt */
	I2S_ISR_CHL3_RXINT, /**< I2S channel3 rx interrupt */

	I2S_ISR_MAX
} i2s_isr_id_t;

typedef enum {
	I2S_DISABLE = 0, /**< I2S disable */
	I2S_ENABLE,      /**< I2S enable */
	I2S_EN_MAX,
} i2s_en_t;

typedef enum {
	I2S_ROLE_SLAVE = 0, /**< I2S role: slave */
	I2S_ROLE_MASTER,    /**< I2S role: master */
	I2S_ROLE_MAX,
} i2s_role_t;

typedef enum {
	I2S_WORK_MODE_I2S = 0,      /**< I2S work mode: I2S (Philips) */
	I2S_WORK_MODE_LEFTJUST,     /**< I2S work mode: Left Justified */
	I2S_WORK_MODE_RIGHTJUST,    /**< I2S work mode: Right Justified */
	I2S_WORK_MODE_RSVD,         /**< I2S work mode: reserved for future use */
	I2S_WORK_MODE_SHORTFAMSYNC, /**< I2S work mode: Short Frame Sync */
	I2S_WORK_MODE_LONGFAMSYNC,  /**< I2S work mode: Long Frame Sync */
	I2S_WORK_MODE_NORMAL2BD,    /**< I2S work mode: Normal 2B+D */
	I2S_WORK_MODE_DELAY2BD,     /**< I2S work mode: Delay 2B+D */
	I2S_WORK_MODE_MAX,
} i2s_work_mode_t;

typedef enum {
	I2S_LRCK_INVERT_DISABLE = 0, /**< I2S lrck invert disable */
	I2S_LRCK_INVERT_ENABLE,      /**< I2S lrck invert enable */
	I2S_LRCK_INVERT_MAX,
} i2s_lrck_invert_en_t;

typedef enum {
	I2S_SCK_INVERT_DISABLE = 0, /**< I2S sck invert disable */
	I2S_SCK_INVERT_ENABLE,      /**< I2S sck invert enable */
	I2S_SCK_INVERT_MAX,
} i2s_sck_invert_en_t;

typedef enum {
	I2S_LSB_FIRST_DISABLE = 0, /**< I2S lsb first disable */
	I2S_LSB_FIRST_ENABLE,      /**< I2S lsb first enable */
	I2S_LSB_FIRST_EN_MAX,
} i2s_lsb_first_en_t;

typedef enum {
	I2S_PARALLEL_DISABLE = 0, /**< I2S parallel disable */
	I2S_PARALLEL_ENABLE,      /**< I2S parallel enable */
	I2S_PARALLEL_EN_MAX,
} i2s_parallel_en_t;

typedef enum {
	I2S_LRCOM_STORE_LRLR = 0, /**< I2S lrcom store mode: L->R->L->R */
	I2S_LRCOM_STORE_16R16L,   /**< I2S lrcom store mode: {16bitR,16bitL} */
	I2S_LRCOM_STORE_MODE_MAX,
} i2s_lrcom_store_mode_t;

typedef enum {
	I2S_TXINT_LEVEL_1 = 0, /**< I2S tx_int level: 1 */
	I2S_TXINT_LEVEL_8,     /**< I2S tx_int level: 8 */
	I2S_TXINT_LEVEL_16,    /**< I2S tx_int level: 16 */
	I2S_TXINT_LEVEL_24,    /**< I2S tx_int level: 24 */
	I2S_TXINT_LEVEL_MAX,
} i2s_txint_level_t;

typedef enum {
	I2S_RXINT_LEVEL_1 = 0, /**< I2S rx_int level: 1 */
	I2S_RXINT_LEVEL_8,     /**< I2S rx_int level: 8 */
	I2S_RXINT_LEVEL_16,    /**< I2S rx_int level: 16 */
	I2S_RXINT_LEVEL_24,    /**< I2S rx_int level: 24 */
	I2S_RXINT_LEVEL_MAX,
} i2s_rxint_level_t;

typedef enum {
	I2S_INT_DISABLE = 0, /**< I2S interrupt disable */
	I2S_INT_ENABLE,      /**< I2S interrupt enable */
	I2S_INT_EN_MAX,
} i2s_int_en_t;

typedef enum {
	I2S_CHANNEL_1 = 0, /**< I2S channel: 1 */
	I2S_CHANNEL_2,     /**< I2S channel: 2 */
	I2S_CHANNEL_3,     /**< I2S channel: 3 */
	I2S_CHANNEL_MAX,
} i2s_channel_id_t;

typedef enum {
	I2S_DATA_WIDTH_8 = 8,   /**< I2S data width: 8 */
	I2S_DATA_WIDTH_16 = 16, /**< I2S data width: 16 */
	I2S_DATA_WIDTH_24 = 24, /**< I2S data width: 24 */
	I2S_DATA_WIDTH_32 = 32, /**< I2S data width: 32 */
} i2s_data_width_t;

typedef enum {
	I2S_SAMP_RATE_8000 = 0, /**< I2S sample rate: 8K */
	I2S_SAMP_RATE_12000,    /**< I2S sample rate: 12K */
	I2S_SAMP_RATE_16000,    /**< I2S sample rate: 16K */
	I2S_SAMP_RATE_24000,    /**< I2S sample rate: 24K */
	I2S_SAMP_RATE_32000,    /**< I2S sample rate: 32K */
	I2S_SAMP_RATE_48000,    /**< I2S sample rate: 48K */
	I2S_SAMP_RATE_96000,    /**< I2S sample rate: 96K */
	I2S_SAMP_RATE_8018,     /**< I2S sample rate: 8.0182K */
	I2S_SAMP_RATE_11025,    /**< I2S sample rate: 11.025K */
	I2S_SAMP_RATE_22050,    /**< I2S sample rate: 22.050K */
	I2S_SAMP_RATE_44100,    /**< I2S sample rate: 44.1K */
	I2S_SAMP_RATE_88200,    /**< I2S sample rate: 88.2K */
	I2S_SAMP_RATE_MAX
} i2s_samp_rate_t;

typedef enum {
	I2S_TXRX_TYPE_TX = 0,
	I2S_TXRX_TYPE_RX,
	I2S_TXRX_TYPE_MAX
} i2s_txrx_type_t;

/**
 * @}
 */

/**
 * @brief I2S struct defines
 * @defgroup bk_api_i2s_structs structs in I2S
 * @ingroup bk_api_i2s
 * @{
 */

typedef struct {
	icu_int_src_t int_src;
	int_group_isr_t isr;
} i2s_int_config_t;

typedef int (*i2s_data_handle_cb)(uint32_t size);

typedef struct {
	i2s_role_t role;
	i2s_work_mode_t work_mode;
	i2s_lrck_invert_en_t lrck_invert;
	i2s_sck_invert_en_t sck_invert;
	i2s_lsb_first_en_t lsb_first_en;
	uint32_t sync_length;
	uint32_t data_length;
	uint32_t pcm_dlength;
	i2s_lrcom_store_mode_t store_mode;
	i2s_samp_rate_t samp_rate;
	uint8_t pcm_chl_num;					//the channel number need to transfer in pcm mode
} i2s_config_t;

#define DEFAULT_I2S_CONFIG() {                         \
        .role = I2S_ROLE_MASTER,                       \
        .work_mode = I2S_WORK_MODE_I2S,                \
        .lrck_invert = I2S_LRCK_INVERT_DISABLE,        \
        .sck_invert = I2S_SCK_INVERT_DISABLE,          \
        .lsb_first_en = I2S_LSB_FIRST_DISABLE,         \
        .sync_length = 0,                              \
        .data_length = 16,                             \
        .pcm_dlength = 0,                              \
        .store_mode = I2S_LRCOM_STORE_LRLR,            \
        .samp_rate = I2S_SAMP_RATE_8000,               \
        .pcm_chl_num = 2,                              \
    }

#define DEFAULT_PCM_CONFIG() {                         \
        .role = I2S_ROLE_MASTER,                       \
        .work_mode = I2S_WORK_MODE_SHORTFAMSYNC,        \
        .lrck_invert = I2S_LRCK_INVERT_DISABLE,        \
        .sck_invert = I2S_SCK_INVERT_DISABLE,          \
        .lsb_first_en = I2S_LSB_FIRST_DISABLE,         \
        .sync_length = 1,                              \
        .data_length = 16,                             \
        .pcm_dlength = 0,                              \
        .store_mode = I2S_LRCOM_STORE_LRLR,            \
        .samp_rate = I2S_SAMP_RATE_8000,               \
        .pcm_chl_num = 1,                              \
    }

#define DEFAULT_TDM_CONFIG() {                         \
        .role = I2S_ROLE_MASTER,                       \
        .work_mode = I2S_WORK_MODE_SHORTFAMSYNC,        \
        .lrck_invert = I2S_LRCK_INVERT_DISABLE,        \
        .sck_invert = I2S_SCK_INVERT_DISABLE,          \
        .lsb_first_en = I2S_LSB_FIRST_DISABLE,         \
        .sync_length = 1,                              \
        .data_length = 32,                             \
        .pcm_dlength = 0,                              \
        .store_mode = I2S_LRCOM_STORE_LRLR,            \
        .samp_rate = I2S_SAMP_RATE_32000,               \
        .pcm_chl_num = 3,                              \
    }

#define DEFAULT_2BD_CONFIG() {                         \
        .role = I2S_ROLE_MASTER,                       \
        .work_mode = I2S_WORK_MODE_NORMAL2BD,           \
        .lrck_invert = I2S_LRCK_INVERT_DISABLE,        \
        .sck_invert = I2S_SCK_INVERT_DISABLE,          \
        .lsb_first_en = I2S_LSB_FIRST_DISABLE,         \
        .sync_length = 0,                              \
        .data_length = 16,                             \
        .pcm_dlength = 0,                              \
        .store_mode = I2S_LRCOM_STORE_LRLR,            \
        .samp_rate = I2S_SAMP_RATE_8000,               \
        .pcm_chl_num = 1,                              \
    }

/**
 * @}
 */


#ifdef __cplusplus
}
#endif
