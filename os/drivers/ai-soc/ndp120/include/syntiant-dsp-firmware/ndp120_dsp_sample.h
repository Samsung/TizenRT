/*
 * Copyright (c) 2024 Syntiant Corp.  All rights reserved.
 * Contact at http://www.syntiant.com
 *
 * This software is available to you under a choice of one of two licenses.
 * You may choose to be licensed under the terms of the GNU General Public
 * License (GPL) Version 2, available from the file LICENSE in the main
 * directory of this source tree, or the OpenIB.org BSD license below.  Any
 * code involving Linux software will require selection of the GNU General
 * Public License (GPL) Version 2.
 *
 * OPENIB.ORG BSD LICENSE
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 	** SDK: v112.3.6-Samsung **
*/

#ifndef NDP120_DSP_SAMPLE_H_
#define NDP120_DSP_SAMPLE_H_

#ifndef __KERNEL__
#include <stdint.h>
#endif

enum {
    NDP120_DSP_SAMPLE_MODE_RUN,
    NDP120_DSP_SAMPLE_MODE_BLOCK,
    NDP120_DSP_SAMPLE_MODE_DROP
};

enum {
    NDP120_DSP_SAMPLE_THRESH_ANY,
    NDP120_DSP_SAMPLE_THRESH_HALF,
    NDP120_DSP_SAMPLE_THRESH_FULL
};

enum {
    NDP120_DSP_SAMPLE_TYPE_PCM_AUDIO,
    NDP120_DSP_SAMPLE_TYPE_FUNCTION,
    NDP120_DSP_SAMPLE_TYPE_SENSOR
};

typedef struct {
    uint8_t type;
    uint8_t params;
    uint16_t frame_num;
} ndp120_dsp_sample_header_t;

typedef struct {
    uint8_t  mode; 
    uint8_t  thresh; 
    uint32_t buf_size;
    uint32_t head_idx;
    uint32_t tail_idx;
} ndp120_dsp_sample_buf_state_t;

typedef struct {
    uint8_t src_type;
    uint8_t src_param;
    uint8_t monotonic_cnt;
    uint8_t _dummy0; /* maintain 4-align */
} ndp120_dsp_audio_sample_annotation_t;

typedef struct {
    uint8_t src_param;
    /**< src param of func */
    uint8_t output_slot;
    /**< output slot of func */
    uint16_t length;
    /**< length of func output */
} ndp120_dsp_ibuf_data_t;

#endif
