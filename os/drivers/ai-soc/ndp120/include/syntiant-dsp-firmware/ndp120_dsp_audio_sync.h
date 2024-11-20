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


#ifndef NDP120_DSP_AUDIO_SYNC_H_
#define NDP120_DSP_AUDIO_SYNC_H_

#ifndef __KERNEL__
#include <stdint.h>
#endif

typedef enum {
 NDP120_DSP_AUDIO_CHAN_AUD0 = 0,
 NDP120_DSP_AUDIO_CHAN_AUD1 = 1,
 NDP120_DSP_AUDIO_CHAN_AUD2 = 2
} ndp120_dsp_audio_chan_t;

typedef enum {
 NDP120_DSP_AUDIO_SYNC_MODE_OFF = 0,
 NDP120_DSP_AUDIO_SYNC_MODE_ALIGN = 1,
 NDP120_DSP_AUDIO_SYNC_MODE_FULL = 2
} ndp120_dsp_audio_sync_mode_t;

typedef struct {
    uint32_t mode;
    int started;
    uint32_t ref_chan;
    uint32_t adj_chan;
    int sample_count_offset;
    int32_t chan_delay_ticks; /* positive = ref before adj, negative = adj before ref */
} ndp120_dsp_audio_sync_config_t;
#endif
