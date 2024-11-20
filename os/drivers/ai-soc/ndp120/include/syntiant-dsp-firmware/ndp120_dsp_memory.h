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


#ifndef NDP120_DSP_MEMORY_H_
#define NDP120_DSP_MEMORY_H_

#ifndef __KERNEL__
#include <stdint.h>
#include <stddef.h>
#endif

typedef enum {
    NDP120_DSP_MEM_TYPE_HEAP = 1,
    NDP120_DSP_MEM_TYPE_DNN_DATA = 2
} ndp120_dsp_mem_type_t;

typedef struct {
    uint32_t size;
    uint8_t pattern[12];
} ndp120_dsp_memory_block_header_t;

/* Round up to multiple of 16 */
#define NDP120_DSP_MEM_ALLOC_TRUE_SIZE(req_size) ((req_size + 15) / 16 * 16 + sizeof(ndp120_dsp_memory_block_header_t)) 

typedef struct {
    uint32_t mem_heap_total;
    uint32_t mem_dnn_data_total;

    uint32_t algo_mem_use_heap;
    uint32_t algo_mem_use_dnn_data;

    uint32_t sample_mem_use_heap;
    uint32_t sample_mem_use_dnn_data;

    uint32_t mem_heap_base;
    uint32_t mem_dnn_data_base;
    uint32_t mem_dnn_data_end;
} ndp120_dsp_memory_state_t;

#endif
