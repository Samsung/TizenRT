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

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <stdint.h>

#define BT_SBC_SYNCWORD                    0x9C        /**< SBC synchronize word             */
#define BT_MSBC_SYNCWORD                   0xAD        /**< MSBC synchronize word            */
#define BT_SBC_SAMPLE_RATE_16000           0x0         /**< SBC sampling frequency : 16.0KHz */ //define conflict
#define BT_SBC_SAMPLE_RATE_32000           0x1         /**< SBC sampling frequency : 32.0KHz */
#define BT_SBC_SAMPLE_RATE_44100           0x2         /**< SBC sampling frequency : 44.1KHz */
#define BT_SBC_SAMPLE_RATE_48000           0x3         /**< SBC sampling frequency : 48.0KHz */
#define BT_SBC_BLOCKS_4                    0x0         /**< SBC blocks number 4              */
#define BT_SBC_BLOCKS_8                    0x1         /**< SBC blocks number 8              */
#define BT_SBC_BLOCKS_12                   0x2         /**< SBC blocks number 12             */
#define BT_SBC_BLOCKS_16                   0x3         /**< SBC blocks number 16             */
#define BT_SBC_CH_MODE_MONO                0x0         /**< SBC channel mode : MONO          */
#define BT_SBC_CH_MODE_DUAL_CHANNEL        0x1         /**< SBC channel mode : Dual Channels */
#define BT_SBC_CH_MODE_STEREO              0x2         /**< SBC channel mode : Stereo        */
#define BT_SBC_CH_MODE_JOINT_STEREO        0x3         /**< SBC channel mode : Joint Stereo  */
#define BT_SBC_ALLOCATION_METHOD_LOUDNESS  0x0         /**< SBC allocation method : Loudness */ //define conflict
#define BT_SBC_ALLOCATION_METHOD_SNR       0x1         /**< SBC allocation method : SNR      */
#define BT_SBC_SUBBANDS_4                  0x0         /**< SBC subbands number 4            */
#define BT_SBC_SUBBANDS_8                  0x1         /**< SBC subbands number 8            */
#define BT_SBC_HEADER_MAX_SZIE              13         /**< SBC header max size 13 in bytes = (8+2+2+2+1+1+8+8+8*1+2*8*4)/8 */
#define BT_SBC_MAX_FRAME_SIZE              513         /**< SBC max frame size 513 in bytes = 4+(4*8*2)/8+(8+16*250)/8      */

#define SCALE_PROTO4_TBL                (15)
#define SCALE_ANA4_TBL                  (17)
#define SCALE_PROTO8_TBL                (16)
#define SCALE_ANA8_TBL                  (17)
#define SCALE_SPROTO4_TBL               (12)
#define SCALE_SPROTO8_TBL               (14)
#define SCALE_NPROTO4_TBL               (11)
#define SCALE_NPROTO8_TBL               (11)
#define SCALE4_STAGE1_BITS              (15)
#define SCALE4_STAGE2_BITS              (15)
#define SCALE8_STAGE1_BITS              (15)
#define SCALE8_STAGE2_BITS              (15)
#define SCALE4_STAGED1_BITS             (15)
#define SCALE4_STAGED2_BITS             (16)
#define SCALE8_STAGED1_BITS             (15)
#define SCALE8_STAGED2_BITS             (16)

#define ASR(val, bits)                  ((int32_t)(val)) >> (bits)
#define SP4(val)                        ASR(val, SCALE_PROTO4_TBL)
#define SA4(val)                        ASR(val, SCALE_ANA4_TBL)
#define SP8(val)                        ASR(val, SCALE_PROTO8_TBL)
#define SA8(val)                        ASR(val, SCALE_ANA8_TBL)
#define SS4(val)                        ASR(val, SCALE_SPROTO4_TBL)
#define SS8(val)                        ASR(val, SCALE_SPROTO8_TBL)
#define SN4(val)                        ASR(val, SCALE_NPROTO4_TBL)
#define SN8(val)                        ASR(val, SCALE_NPROTO8_TBL)
#define SCALE4_STAGE1(src)              ASR(src, SCALE4_STAGE1_BITS)
#define SCALE4_STAGE2(src)              ASR(src, SCALE4_STAGE2_BITS)
#define SCALE8_STAGE1(src)              ASR(src, SCALE8_STAGE1_BITS)
#define SCALE8_STAGE2(src)              ASR(src, SCALE8_STAGE2_BITS)
#define SCALE4_STAGED1(src)             ASR(src, SCALE4_STAGED1_BITS)
#define SCALE4_STAGED2(src)             ASR(src, SCALE4_STAGED2_BITS)
#define SCALE8_STAGED1(src)             ASR(src, SCALE8_STAGED1_BITS)
#define SCALE8_STAGED2(src)             ASR(src, SCALE8_STAGED2_BITS)

#ifndef inline
#define inline __inline
#endif//inline

/**
 * @brief SBC decoder context
 */
typedef struct _SbcCommonContext
{
    int8_t   blocks;                    /**< block number       */
    int8_t   subbands;                  /**< subbands number    */
    uint8_t  join;                      /**< bit number x set means joint stereo has been used in sub-band x */
    uint8_t  bitpool;                   /**< indicate the size of the bit allocation pool that has been used for encoding the stream */

    int8_t   channel_mode;              /**< channel mode       */
    int8_t   sample_rate_index;         /**< sample rate index, 0:16000, 1:32000, 2:44100, 3:48000 */
    int8_t   allocation_method;         /**< allocation method  */
    int8_t   reserved8;                 /**< dummy, reserved for byte align */

    int8_t   bits[2][8];                /**< calculate result by bit allocation. */

    int8_t   scale_factor[2][8];        /**< only the lower 4 bits of every element are to be used */

    int32_t  mem[2][8];                 /**< Memory used as bit need and levels */

} SbcCommonContext;

/**
 * @brief SBC frame header context
 */
typedef struct _SbcFrameHeader
{
#if defined(__BIG_ENDIAN__)
    //big endianness
    uint32_t crc_check          : 8;
    uint32_t bitpool            : 8;
    uint32_t subband_mode       : 1;
    uint32_t allocation_method  : 1;
    uint32_t channel_mode       : 2;
    uint32_t block_mode         : 2;
    uint32_t sample_rate_index  : 2;
    uint32_t syncword           : 8;
#else
    //little endianness
    uint32_t syncword           : 8;
    uint32_t subband_mode       : 1;
    uint32_t allocation_method  : 1;
    uint32_t channel_mode       : 2;
    uint32_t block_mode         : 2;
    uint32_t sample_rate_index  : 2;
    uint32_t bitpool            : 8;
    uint32_t crc_check          : 8;
#endif
} SbcFrameHeader;



/**
 * @brief SBC encoder context
 */
typedef struct _SbcEncoderContext
{
    SbcCommonContext frame;
    SbcFrameHeader   header;

    int8_t   num_channels;              /**< channels number    */
    uint8_t  pcm_length;                /**< PCM length         */
    uint16_t sample_rate;               /**< sample rate        */

    int32_t  sb_sample_f[2][16][8];     /**< subband sample     */

    uint8_t  reserved;
    uint8_t  frame_index;
    uint8_t  frame_id[2];
    uint8_t  stream[512];               /**< encoded buffer     */

    int32_t  position[2];
    int32_t  xfifo[2][160];

} SbcEncoderContext;


/**
 * SBC encoder error code
 */
typedef enum _SBC_ENCODER_ERROR_CODE
{
    SBC_ENCODER_ERRORS = -128,
    SBC_ENCODER_ERROR_INVALID_SAMPLE_RATE,      /**< invalid sample rate   */
    SBC_ENCODER_ERROR_INVALID_CHANNLES,         /**< invalid channels      */
    SBC_ENCODER_ERROR_INVALID_CTRL_CMD,         /**< invalid ctrl cmd      */
    SBC_ENCODER_ERROR_INVALID_CTRL_ARG,         /**< invalid ctrl arg      */
    SBC_ENCODER_ERROR_BITPOOL_OUT_BOUNDS,       /**< bitpool out of bounds */

    SBC_ENCODER_ERROR_OK = 0,                   /**< no error              */

} SBC_ENCODER_ERROR_CODE;

typedef enum _SBC_ENCODER_CTRL_CMD
{
    SBC_ENCODER_CTRL_CMD_SET_ALLOCATION_METHOD,    /**< arg: 0:loundness, 1:SNR                       */
    SBC_ENCODER_CTRL_CMD_SET_BITPOOL,              /**< arg: 2-250                                    */
    SBC_ENCODER_CTRL_CMD_SET_BLOCK_MODE,           /**< arg: 0:4, 1:8, 2:12, 3:16                     */
    SBC_ENCODER_CTRL_CMD_SET_CHANNEL_MODE,         /**< arg: 0:MONO, 1:DUAL, 2:STEREO, 3:JOINT STEREO */
    SBC_ENCODER_CTRL_CMD_SET_SAMPLE_RATE_INDEX,    /**< arg: 0:16000, 1:32000, 2:44100, 3:48000       */
    SBC_ENCODER_CTRL_CMD_SET_SUBBAND_MODE,         /**< arg: 0:4, 1:8                                 */
    SBC_ENCODER_CTRL_CMD_SET_MSBC_ENCODE_MODE,     /**< arg: NULL                                     */

    SBC_ENCODER_CTRL_CMD_GET_ALLOCATION_METHOD,    /**< get allcation method                          */
    SBC_ENCODER_CTRL_CMD_GET_BITPOOL,              /**< get bitpool value                             */
    SBC_ENCODER_CTRL_CMD_GET_BLOCK_MODE,           /**< get block mode                                */
    SBC_ENCODER_CTRL_CMD_GET_CHANNEL_MODE,         /**< get channel mode                              */
    SBC_ENCODER_CTRL_CMD_GET_SAMPLE_RATE_INDEX,    /**< get sample rate index                         */
    SBC_ENCODER_CTRL_CMD_GET_SUBBAND_MODE,         /**< get sunband mode                              */

} SBC_ENCODER_CTRL_CMD;


#ifdef __cplusplus
}
#endif//__cplusplus
