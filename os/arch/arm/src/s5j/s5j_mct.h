/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * arch/arm/src/s5j/s5j_mct.h
 *
 *   Copyright (C) 2013-2014 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/
#ifndef __ARCH_ARM_SRC_S5J_S5J_MCT_H__
#define __ARCH_ARM_SRC_S5J_S5J_MCT_H__

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stddef.h>
#include <sys/types.h>
#include <string.h>
#include <arch/chip/chip_types.h>

/****************************************************************************
 * Private Types
 ****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	MCT_G0 = 0x200,
	MCT_G1 = 0x210,
	MCT_G2 = 0x220,
	MCT_G3 = 0x230,
	MCT_L0 = 0x300,
	MCT_L1 = 0x400,
	MCT_L2 = 0x500,
	MCT_L3 = 0x600,
} MCT_CHANNEL;

typedef enum {
	LINT_NONE = 0,
	LINT_INT = 1,
	LINT_FRC = 2,
	LINT_FRC_INT = 3,
} MCT_LINT;

typedef enum {
	eDIVIDER_1 = 0,
	eDIVIDER_2,
	eDIVIDER_4,
	eDIVIDER_8,
	eDIVIDER_16,
	eDIVIDER_MAX,
} eMCT_DIVIDER;

/****************************************************************************
 * Public Functions
 ****************************************************************************/
void s5j_mct_local_int_ctrl(MCT_CHANNEL mct_id, MCT_LINT flag);
u64 s5j_mct_global_get_comp_cnt(MCT_CHANNEL mct_id);
u32 s5j_mct_global_get_auto_incr(MCT_CHANNEL mct_id);
void s5j_mct_global_set_auto_incr(MCT_CHANNEL mct_id, u32 count);
void s5j_mct_local_set_int_cnt_auto(MCT_CHANNEL mct_id, u32 count);
void s5j_mct_local_set_int_cnt_manual(MCT_CHANNEL mct_id, u32 count);
void s5j_mct_local_set_tick_cnt(MCT_CHANNEL mct_id, u32 count);
u32 s5j_mct_local_get_int_cnt(MCT_CHANNEL mct_id);
u32 s5j_mct_local_get_tick_cnt(MCT_CHANNEL mct_id);
void s5j_mct_local_set_frc_cnt(MCT_CHANNEL mct_id, u32 count);
u32 s5j_mct_local_get_frc_cnt(MCT_CHANNEL mct_id);
bool s5j_mct_ispending(MCT_CHANNEL mct_id);
void s5j_mct_global_start_frc(void);
void s5j_mct_global_stop_frc(void);
void s5j_mct_global_set_frc_cnt(cycle_t count);
void s5j_mct_clk_rate(u32 prescaler, eMCT_DIVIDER divider);
void s5j_mct_local_set_interval(MCT_CHANNEL mct_id);
void s5j_mct_local_clear_interval(MCT_CHANNEL mct_id);
void s5j_mct_local_start_int(MCT_CHANNEL mct_id);
void s5j_mct_local_stop_int(MCT_CHANNEL mct_id);
void s5j_mct_local_start_timer(MCT_CHANNEL mct_id);
void s5j_mct_local_stop_timer(MCT_CHANNEL mct_id);
void s5j_mct_local_start_frc(MCT_CHANNEL mct_id);
void s5j_mct_local_stop_frc(MCT_CHANNEL mct_id);
void s5j_mct_global_enable_int(MCT_CHANNEL mct_id);
void s5j_mct_global_disable_int(MCT_CHANNEL mct_id);
void s5j_mct_global_set_comp_cnt(MCT_CHANNEL mct_id, u64 count);
cycle_t s5j_mct_global_get_frc_cnt(void);
void s5j_mct_global_disable_comp(MCT_CHANNEL mct_id);
void s5j_mct_global_enable_comp(MCT_CHANNEL mct_id);
void s5j_mct_reset(MCT_CHANNEL mct_id);
void s5j_mct_clear_pending(MCT_CHANNEL mct_id);

#ifdef __cplusplus
}
#endif
#endif /* __ARCH_ARM_SRC_S5J_S5J_MCT_H__ */
