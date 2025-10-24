// Copyright 2022-2023 Beken
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


#ifndef _WAV_HEADER_H_
#define _WAV_HEADER_H_

#include <common/bk_include.h>

#ifdef __cplusplus
extern "C"
{
#endif


/**
* brief     RIFF chunk
*/
typedef struct {
    int8_t chunk_id[4];              /*!<chunk id;"RIFF"*/
    uint32_t chunk_size;             /*!<file length - 8 */
    int8_t format[4];                /*!<"WAVE" */
} riff_chunk_t;

/**
* brief     FMT chunk
*/
typedef struct {
    int8_t fmt_id[4];                /*!<chunk id;"fmt " */
    uint32_t fmt_size;               /*!<Size of this fmt block (Not include ID and Size);16 or 18 or 40 bytes. */
    uint16_t audio_format;           /*!<format;0X01:linear PCM */
    uint16_t num_of_channels;        /*!<Number of channel;1: 1 channel;2: 2 channels; */
    uint32_t samplerate;             /*!<sample rate;0X1F40 = 8Khz */
    uint32_t byterate;               /*!<Byte rate; */
    uint16_t block_align;            /*!<align with byte; */
    uint16_t bits_per_sample;        /*!<Bit lenght per sample point */
} fmt_chunk_t;

/**
* brief     Data chunk
*/
typedef struct {
    int8_t data_id[4];               /*!<data id;"data" */
    uint32_t data_size;              /*!<Size of data block(Not include ID and Size) */
} data_chunk_t;

/**
* brief     WAV header struct
*/
typedef struct {
    riff_chunk_t riff;               /*!<riff */
    fmt_chunk_t fmt;                 /*!<fmt */
    data_chunk_t data;               /*!<data */
} wav_header_t;

#define DEFAULT_WAV_HEAD_CONFIG() {                         \
        .riff = {                                      \
                       .chunk_id = "RIFF",             \
                       .chunk_size = 0,                \
                       .format = "WAVE ",              \
                    },                                 \
        .fmt =  {                                      \
                       .fmt_id = "fmt ",               \
                       .fmt_size = 16,                 \
                       .audio_format = 1,              \
                       .num_of_channels = 1,           \
                       .samplerate = 8000,             \
                       .byterate = 16000,              \
                       .block_align = 2,               \
                       .bits_per_sample = 16,          \
                    },                                 \
        .data = {                                      \
                       .data_id = "data",              \
                       .data_size = 0,                 \
                    },                                 \
    }


/**
* brief     WAV information struct
*/
typedef struct {
    uint16_t channels;                  /*!<Number of channel;1: 1 channel;2: 2 channels; */
    uint32_t data_size;                 /*!<size of data */
    uint32_t samplerate;                /*!<sample rate */
    uint16_t bits;                      /*!<bit length 16bit,24bit,32bit */ 
} wav_info_t;


/**
* @brief      Check whether this stream is WAV or not
*
* @param[in]      in_data: The input stream data(wav head) of WAV stream
* @param[in]      len: The length of input stream data, wav head is 44 bytes
*
* @return
*                 - BK_OK :check pass
*                 - BK_FAIL:check fail
*/
bk_err_t wav_check_type(uint8_t *in_data, uint32_t len);

/**
* @brief      Initialize the header information of WAV stream
*
* @param[in]      wavhead: The header information of WAV stream
* @param[in]      sample_rate: The sample rate of WAV stream
* @param[in]      bits: The bit width of WAV stream
* @param[in]      channels: The number channel(s) of WAV stream
*
* @return
*                 - BK_OK
*                 - BK_FAIL
*/
bk_err_t wav_head_init(wav_header_t *wavhead, uint32_t sample_rate, uint32_t bits, uint16_t channels);

/**
* @brief      Parse the header of WAV stream
*
* @param[in]      wavhead: The input stream data of WAV stream header
* @param[in]      len: The length of input stream data
* 
* @param[out]     info: The header information of stream data
*
* @return
*                 - BK_OK
*                 - BK_FAIL
*/
bk_err_t wav_head_parse(wav_header_t *wavhead, wav_info_t *info);

/**
* @brief      Set audio header data size of WAV stream
*
* @param[in]      wavhead: The header information of WAV stream
* @param[in]      dataSize: The size of WAV stream
*
* @return
*                 - BK_OK
*                 - BK_FAIL
*/
bk_err_t wav_set_head_size(wav_header_t *wavhead, uint32_t dataSize);

#ifdef __cplusplus
}
#endif

#endif
