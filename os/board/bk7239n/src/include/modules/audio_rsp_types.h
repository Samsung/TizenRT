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

#include <common/bk_include.h>
#include <driver/aud_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief      Resample Filter Configuration
 */
typedef struct {
    int src_rate;              /**< The sampling rate of the source PCM file (in Hz)*/
    int src_ch;                /**< The number of channel(s) of the source PCM file (Mono=1, Dual=2) */
    int src_bits;              /**< The bit for sample of the source PCM data. 8bits/16bits/24bits/32bits. */
    int dest_rate;             /**< The sampling rate of the destination PCM file (in Hz) */
    int dest_ch;               /**< The number of channel(s) of the destination PCM file (Mono=1, Dual=2) */
    int dest_bits;             /**< The bit for sample of the destination PCM data. */
    int complexity;            /**< Indicates the complexity of the resampling. Range:[0, 10]; 0 indicates the lowest complexity, the highest speed and the lowest accuracy. 10 indicates the highest complexity, the lowest speed and the highest accuracy. recommend: 6 */
    int down_ch_idx;           /**< Indicates the channel that is selected (the right channel or the left channel). This parameter is only valid when the number of channel(s) of the input file has changed from dual to mono. */
} aud_rsp_cfg_t;

#ifdef __cplusplus
}
#endif
