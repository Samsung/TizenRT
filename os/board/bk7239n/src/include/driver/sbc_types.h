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

#pragma once

#include <driver/int_types.h>
#include <common/bk_include.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef void (*sbc_decoder_isr_t)(void *param);

#define SBC_SYNCWORD					0x9C		/**< SBC synchronize word */
#define MSBC_SYNCWORD					0xAD		/**< MSBC synchronize word */
#define SBC_CHANNEL_MODE_MONO			0x00		/**< SBC channel mode : MONO */
#define SBC_CHANNEL_MODE_DUAL			0x01		/**< SBC channel mode : Dual Channels */
#define SBC_CHANNEL_MODE_STEREO			0x02		/**< SBC channel mode : Stereo */
#define SBC_CHANNEL_MODE_JOINT_STEREO	0x03		/**< SBC channel mode : Joint Stereo */

#define SBC_DECODER_ERRORS						BK_ERR_SBC_BASE	
#define SBC_DECODER_ERROR_BUFFER_OVERFLOW		BK_ERR_SBC_BASE - 1		/**< buffer overflow       */
#define SBC_DECODER_ERROR_SYNC_INCORRECT		BK_ERR_SBC_BASE	- 2		/**< synchronize incorrect */
#define SBC_DECODER_ERROR_BITPOOL_OUT_BOUNDS	BK_ERR_SBC_BASE	- 3		/**< bitpool out of bounds */
#define SBC_DECODER_ERROR_STREAM_EMPTY			BK_ERR_SBC_BASE	- 4		/**< stream empty          */
#define SBC_DECODER_ERROR_OK					0x0						/**< no error              */


typedef enum {
	SBC_SAMPLE_RATE_16000 = 0,		/**< SBC sampling frequency : 16.0KHz */
	SBC_SAMPLE_RATE_32000,			/**< SBC sampling frequency : 32.0KHz */
	SBC_SAMPLE_RATE_44100,			/**< SBC sampling frequency : 44.1KHz */
	SBC_SAMPLE_RATE_48000,			/**< SBC sampling frequency : 48.0KHz */
	SBC_SAMPLE_RATE_MAX,
} sbc_sample_rates_t;

typedef enum {
	SBC_CHANNEL_NUM_ONE = 0,		/**< SBC channel number : 1 channel */
	SBC_CHANNEL_NUM_TWO,			/**< SBC channel number : 2 channels */
	SBC_CHANNEL_MODE_INVALID,
} sbc_channel_num_t;

typedef enum {
	SBC_SUBBAND_NUMBER_4 = 0,		/**< SBC subbands number 4 */
	SBC_SUBBAND_NUMBER_8,			/**< SBC subbands number 8 */
	SBC_SUBBAND_MAX,
} sbc_subband_number_t;

typedef enum {
	SBC_DECODE_OUTPUT_SINGLE = 0,		/**< SBC decoder output struct:{16'd0, pcm} */
	SBC_DECODE_OUTPUT_DOUBLE,			/**< SBC decoder output struct:{pcm_chn2, pcm_chn1} */
	SBC_DECODE_OUTPUT_INVALID,
} sbc_chn_comb_t;

typedef enum {
	SBC_BLOCK_NUMBER_4 = 0,			/**< SBC blocks number 4 */
	SBC_BLOCK_NUMBER_8,				/**< SBC blocks number 8 */
	SBC_BLOCK_NUMBER_12,			/**< SBC blocks number 12 */
	SBC_BLOCK_NUMBER_16,			/**< SBC blocks number 16 */
	SBC_BLOCK_NUMBER_MAX,
} sbc_blocks_number_t;

typedef enum {
	SBC_ALLOCATION_METHOD_LOUDNESS = 0,		/**< SBC allocation method : Loudness */
	SBC_ALLOCATION_METHOD_SNR,				/**< SBC allocation method : SNR */
	SBC_ALLOCATION_METHOD_INVALID,
} sbc_allocation_method_t;

/**
 * @brief SBC decoder context
 */
typedef struct {
	int8_t blocks;                    /**< block number */
	int8_t subbands;                  /**< subbands number */
	uint8_t join;					   /**< bit number x set means joint stereo has been used in subband x */
	uint8_t bitpool;                   /**< indicate the size of the bit allocation pool that has been used for encoding the stream */

	int8_t channel_mode;              /**< channel mode */
	int8_t sample_rate_index;         /**< sample rate index, 0:16000, 1:32000, 2:44100, 3:48000 */
	int8_t allocation_method;         /**< allocation method */
	int8_t reserved8;                 /**< dummy, reserved for byte align */

	int8_t bits[2][8];                /**< calculate result by bit allocation */

	int8_t scale_factor[2][8];        /**< only the lower 4 bits of every element are to be used */

	int32_t mem[2][8];                /**< Memory used as bit need and levels */

}sbccommoncontext_t;

/**
 * @brief SBC decoder context
 */
typedef struct {
    sbccommoncontext_t frame;

    int8_t   channel_number;            /**< channels number */
    uint8_t  pcm_length;                /**< PCM length */
    uint16_t sample_rate;               /**< sample rate */

    int32_t  pcm_sample[2][128];        /**< PCM frame buffer */

    int32_t  vfifo[2][170];             /**< FIFO V for subbands synthesis calculation. */ 

    int32_t  offset[2][16];

}sbcdecodercontext_t;

typedef struct {
	sbc_channel_num_t channel_num;		/**< channels number */
	sbc_subband_number_t subbands;		/**< subband number */
	sbc_chn_comb_t chn_comb;			/**< pcm output struct */
	sbc_blocks_number_t blocks;			/**< block number */
} sbc_config_t;


#ifdef __cplusplus
}
#endif


