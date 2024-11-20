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


#ifndef NDP120_MEM_H
#define NDP120_MEM_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __KERNEL__
#include <stddef.h>
#include <stdint.h>
#endif

#ifdef X86_64
extern uint64_t __pseudo_heap_start__;
extern uint64_t __pseudo_heap_end__;
extern uint64_t __pseudo_heap_len__;
#define NUM_PAGES 5
#define HEAP_BEGIN ((uint8_t *)__pseudo_heap_start__)
#define HEAP_END ((uint8_t *)__pseudo_heap_end__)
#else
extern const uint32_t __pseudo_heap_start__;
extern const uint32_t __pseudo_heap_end__;
extern const uint32_t __pseudo_heap_len__;
#define HEAP_BEGIN ((uint8_t *)&__pseudo_heap_start__)
#define HEAP_END ((uint8_t *)&__pseudo_heap_end__)
#endif

#define ALIGNED_FOUR_BYTE(x) (((x) + 3) / 4 * 4)
#define ALIGNED_EIGHT_BYTE(x) (((x) + 7) / 8 * 8)
#define HEAP_SIZE __pseudo_heap_len__
#define MEM_TOTAL() HEAP_SIZE;

#ifdef X86_64
void mem_clean(void);
void mmap_init(void);
void mmap_clean(void);
#endif
size_t mem_avail(void);
void*  mem_alloc(size_t size);
void   mem_reset(void **prev_heap);

#ifdef __cplusplus
}
#endif
#endif

