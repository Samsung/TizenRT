// Copyright 2022-2025 Beken
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

// This is a generated file, if you need to modify it, use the script to
// generate and modify all the struct.h, ll.h, reg.h, debug_dump.c files!

#pragma once

#include <soc/soc.h>
#include "hal_port.h"
#include "adc_hw.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ADC_LL_REG_BASE   SOC_SADC_REG_BASE

//reg reg0:
static inline void adc_ll_set_reg0_value(uint32_t v) {
	adc_reg0_t *r = (adc_reg0_t*)(SOC_SADC_REG_BASE + (0x0 << 2));
	r->v = v;
}

static inline uint32_t adc_ll_get_reg0_value(void) {
	adc_reg0_t *r = (adc_reg0_t*)(SOC_SADC_REG_BASE + (0x0 << 2));
	return r->v;
}

static inline uint32_t adc_ll_get_reg0_deviceid(void) {
	adc_reg0_t *r = (adc_reg0_t*)(SOC_SADC_REG_BASE + (0x0 << 2));
	return r->deviceid;
}

//reg reg1:
static inline void adc_ll_set_reg1_value(uint32_t v) {
	adc_reg1_t *r = (adc_reg1_t*)(SOC_SADC_REG_BASE + (0x1 << 2));
	r->v = v;
}

static inline uint32_t adc_ll_get_reg1_value(void) {
	adc_reg1_t *r = (adc_reg1_t*)(SOC_SADC_REG_BASE + (0x1 << 2));
	return r->v;
}

static inline uint32_t adc_ll_get_reg1_versionid(void) {
	adc_reg1_t *r = (adc_reg1_t*)(SOC_SADC_REG_BASE + (0x1 << 2));
	return r->versionid;
}

//reg reg2:

static inline void adc_ll_set_reg2_value(uint32_t v) {
	adc_reg2_t *r = (adc_reg2_t*)(SOC_SADC_REG_BASE + (0x2 << 2));
	r->v = v;
}

static inline uint32_t adc_ll_get_reg2_value(void) {
	adc_reg2_t *r = (adc_reg2_t*)(SOC_SADC_REG_BASE + (0x2 << 2));
	return r->v;
}

static inline void adc_ll_set_reg2_soft_rst(uint32_t v) {
	adc_reg2_t *r = (adc_reg2_t*)(SOC_SADC_REG_BASE + (0x2 << 2));
	r->soft_rst = v;
}

static inline uint32_t adc_ll_get_reg2_soft_rst(void) {
	adc_reg2_t *r = (adc_reg2_t*)(SOC_SADC_REG_BASE + (0x2 << 2));
	return r->soft_rst;
}

static inline void adc_ll_set_reg2_bypass_ckg(uint32_t v) {
	adc_reg2_t *r = (adc_reg2_t*)(SOC_SADC_REG_BASE + (0x2 << 2));
	r->bypass_ckg = v;
}

static inline uint32_t adc_ll_get_reg2_bypass_ckg(void) {
	adc_reg2_t *r = (adc_reg2_t*)(SOC_SADC_REG_BASE + (0x2 << 2));
	return r->bypass_ckg;
}

//reg reg3:

static inline void adc_ll_set_reg3_value(uint32_t v) {
	adc_reg3_t *r = (adc_reg3_t*)(SOC_SADC_REG_BASE + (0x3 << 2));
	r->v = v;
}

static inline uint32_t adc_ll_get_reg3_value(void) {
	adc_reg3_t *r = (adc_reg3_t*)(SOC_SADC_REG_BASE + (0x3 << 2));
	return r->v;
}

static inline uint32_t adc_ll_get_reg3_gbstatus(void) {
	adc_reg3_t *r = (adc_reg3_t*)(SOC_SADC_REG_BASE + (0x3 << 2));
	return r->gbstatus;
}

//reg reg4:

static inline void adc_ll_set_reg4_value(uint32_t v) {
	adc_reg4_t *r = (adc_reg4_t*)(SOC_SADC_REG_BASE + (0x4 << 2));
	r->v = v;
}

static inline uint32_t adc_ll_get_reg4_value(void) {
	adc_reg4_t *r = (adc_reg4_t*)(SOC_SADC_REG_BASE + (0x4 << 2));
	return r->v;
}

static inline void adc_ll_set_reg4_enable(uint32_t v) {
	adc_reg4_t *r = (adc_reg4_t*)(SOC_SADC_REG_BASE + (0x4 << 2));
	r->enable = v;
}

static inline uint32_t adc_ll_get_reg4_enable(void) {
	adc_reg4_t *r = (adc_reg4_t*)(SOC_SADC_REG_BASE + (0x4 << 2));
	return r->enable;
}

static inline void adc_ll_set_reg4_calib_mode(uint32_t v) {
	adc_reg4_t *r = (adc_reg4_t*)(SOC_SADC_REG_BASE + (0x4 << 2));
	r->calib_mode = v;
}

static inline uint32_t adc_ll_get_reg4_calib_mode(void) {
	adc_reg4_t *r = (adc_reg4_t*)(SOC_SADC_REG_BASE + (0x4 << 2));
	return r->calib_mode;
}

static inline void adc_ll_set_reg4_calib_done(uint32_t v) {
	adc_reg4_t *r = (adc_reg4_t*)(SOC_SADC_REG_BASE + (0x4 << 2));
	r->calib_done = v;
}

static inline uint32_t adc_ll_get_reg4_calib_done(void) {
	adc_reg4_t *r = (adc_reg4_t*)(SOC_SADC_REG_BASE + (0x4 << 2));
	return r->calib_done;
}

static inline void adc_ll_set_reg4_samp_sel(uint32_t v) {
	adc_reg4_t *r = (adc_reg4_t*)(SOC_SADC_REG_BASE + (0x4 << 2));
	r->samp_sel = v;
}

static inline uint32_t adc_ll_get_reg4_samp_sel(void) {
	adc_reg4_t *r = (adc_reg4_t*)(SOC_SADC_REG_BASE + (0x4 << 2));
	return r->samp_sel;
}

static inline void adc_ll_set_reg4_adc_chan(uint32_t v) {
	adc_reg4_t *r = (adc_reg4_t*)(SOC_SADC_REG_BASE + (0x4 << 2));
	r->adc_chan = v;
}

static inline uint32_t adc_ll_get_reg4_adc_chan(void) {
	adc_reg4_t *r = (adc_reg4_t*)(SOC_SADC_REG_BASE + (0x4 << 2));
	return r->adc_chan;
}

static inline void adc_ll_set_reg4_adc_mode(uint32_t v) {
	adc_reg4_t *r = (adc_reg4_t*)(SOC_SADC_REG_BASE + (0x4 << 2));
	r->adc_mode = v;
}

static inline uint32_t adc_ll_get_reg4_adc_mode(void) {
	adc_reg4_t *r = (adc_reg4_t*)(SOC_SADC_REG_BASE + (0x4 << 2));
	return r->adc_mode;
}

static inline void adc_ll_set_reg4_adc_dump_num(uint32_t v) {
	adc_reg4_t *r = (adc_reg4_t*)(SOC_SADC_REG_BASE + (0x4 << 2));
	r->adc_dump_num = v;
}

static inline uint32_t adc_ll_get_reg4_adc_dump_num(void) {
	adc_reg4_t *r = (adc_reg4_t*)(SOC_SADC_REG_BASE + (0x4 << 2));
	return r->adc_dump_num;
}

static inline void adc_ll_set_reg4_adc_sclk_div(uint32_t v) {
	adc_reg4_t *r = (adc_reg4_t*)(SOC_SADC_REG_BASE + (0x4 << 2));
	r->adc_sclk_div = v;
}

static inline uint32_t adc_ll_get_reg4_adc_sclk_div(void) {
	adc_reg4_t *r = (adc_reg4_t*)(SOC_SADC_REG_BASE + (0x4 << 2));
	return r->adc_sclk_div;
}

static inline void adc_ll_set_reg4_adc_sraw(uint32_t v) {
	adc_reg4_t *r = (adc_reg4_t*)(SOC_SADC_REG_BASE + (0x4 << 2));
	r->adc_sraw = v;
}

static inline uint32_t adc_ll_get_reg4_adc_sraw(void) {
	adc_reg4_t *r = (adc_reg4_t*)(SOC_SADC_REG_BASE + (0x4 << 2));
	return r->adc_sraw;
}

//reg reg5:

static inline void adc_ll_set_reg5_value(uint32_t v) {
	adc_reg5_t *r = (adc_reg5_t*)(SOC_SADC_REG_BASE + (0x5 << 2));
	r->v = v;
}

static inline uint32_t adc_ll_get_reg5_value(void) {
	adc_reg5_t *r = (adc_reg5_t*)(SOC_SADC_REG_BASE + (0x5 << 2));
	return r->v;
}

static inline void adc_ll_set_reg5_co_gain(uint32_t v) {
	adc_reg5_t *r = (adc_reg5_t*)(SOC_SADC_REG_BASE + (0x5 << 2));
	r->co_gain = v;
}

static inline uint32_t adc_ll_get_reg5_co_gain(void) {
	adc_reg5_t *r = (adc_reg5_t*)(SOC_SADC_REG_BASE + (0x5 << 2));
	return r->co_gain;
}

//reg reg6:

static inline void adc_ll_set_reg6_value(uint32_t v) {
	adc_reg6_t *r = (adc_reg6_t*)(SOC_SADC_REG_BASE + (0x6 << 2));
	r->v = v;
}

static inline uint32_t adc_ll_get_reg6_value(void) {
	adc_reg6_t *r = (adc_reg6_t*)(SOC_SADC_REG_BASE + (0x6 << 2));
	return r->v;
}

static inline void adc_ll_set_reg6_co_offset(uint32_t v) {
	adc_reg6_t *r = (adc_reg6_t*)(SOC_SADC_REG_BASE + (0x6 << 2));
	r->co_offset = v;
}

static inline uint32_t adc_ll_get_reg6_co_offset(void) {
	adc_reg6_t *r = (adc_reg6_t*)(SOC_SADC_REG_BASE + (0x6 << 2));
	return r->co_offset;
}

//reg reg7:

static inline void adc_ll_set_reg7_value(uint32_t v) {
	adc_reg7_t *r = (adc_reg7_t*)(SOC_SADC_REG_BASE + (0x7 << 2));
	r->v = v;
}

static inline uint32_t adc_ll_get_reg7_value(void) {
	adc_reg7_t *r = (adc_reg7_t*)(SOC_SADC_REG_BASE + (0x7 << 2));
	return r->v;
}

static inline void adc_ll_set_reg7_cwt_cal00(uint32_t v) {
	adc_reg7_t *r = (adc_reg7_t*)(SOC_SADC_REG_BASE + (0x7 << 2));
	r->cwt_cal00 = v;
}

static inline uint32_t adc_ll_get_reg7_cwt_cal00(void) {
	adc_reg7_t *r = (adc_reg7_t*)(SOC_SADC_REG_BASE + (0x7 << 2));
	return r->cwt_cal00;
}

//reg reg8:

static inline void adc_ll_set_reg8_value(uint32_t v) {
	adc_reg8_t *r = (adc_reg8_t*)(SOC_SADC_REG_BASE + (0x8 << 2));
	r->v = v;
}

static inline uint32_t adc_ll_get_reg8_value(void) {
	adc_reg8_t *r = (adc_reg8_t*)(SOC_SADC_REG_BASE + (0x8 << 2));
	return r->v;
}

static inline void adc_ll_set_reg8_cwt_cal01(uint32_t v) {
	adc_reg8_t *r = (adc_reg8_t*)(SOC_SADC_REG_BASE + (0x8 << 2));
	r->cwt_cal01 = v;
}

static inline uint32_t adc_ll_get_reg8_cwt_cal01(void) {
	adc_reg8_t *r = (adc_reg8_t*)(SOC_SADC_REG_BASE + (0x8 << 2));
	return r->cwt_cal01;
}

//reg reg9:

static inline void adc_ll_set_reg9_value(uint32_t v) {
	adc_reg9_t *r = (adc_reg9_t*)(SOC_SADC_REG_BASE + (0x9 << 2));
	r->v = v;
}

static inline uint32_t adc_ll_get_reg9_value(void) {
	adc_reg9_t *r = (adc_reg9_t*)(SOC_SADC_REG_BASE + (0x9 << 2));
	return r->v;
}

static inline void adc_ll_set_reg9_cwt_cal02(uint32_t v) {
	adc_reg9_t *r = (adc_reg9_t*)(SOC_SADC_REG_BASE + (0x9 << 2));
	r->cwt_cal02 = v;
}

static inline uint32_t adc_ll_get_reg9_cwt_cal02(void) {
	adc_reg9_t *r = (adc_reg9_t*)(SOC_SADC_REG_BASE + (0x9 << 2));
	return r->cwt_cal02;
}

//reg rega:

static inline void adc_ll_set_rega_value(uint32_t v) {
	adc_rega_t *r = (adc_rega_t*)(SOC_SADC_REG_BASE + (0xa << 2));
	r->v = v;
}

static inline uint32_t adc_ll_get_rega_value(void) {
	adc_rega_t *r = (adc_rega_t*)(SOC_SADC_REG_BASE + (0xa << 2));
	return r->v;
}

static inline void adc_ll_set_rega_cwt_cal03(uint32_t v) {
	adc_rega_t *r = (adc_rega_t*)(SOC_SADC_REG_BASE + (0xa << 2));
	r->cwt_cal03 = v;
}

static inline uint32_t adc_ll_get_rega_cwt_cal03(void) {
	adc_rega_t *r = (adc_rega_t*)(SOC_SADC_REG_BASE + (0xa << 2));
	return r->cwt_cal03;
}

//reg regb:

static inline void adc_ll_set_regb_value(uint32_t v) {
	adc_regb_t *r = (adc_regb_t*)(SOC_SADC_REG_BASE + (0xb << 2));
	r->v = v;
}

static inline uint32_t adc_ll_get_regb_value(void) {
	adc_regb_t *r = (adc_regb_t*)(SOC_SADC_REG_BASE + (0xb << 2));
	return r->v;
}

static inline void adc_ll_set_regb_cwt_cal04(uint32_t v) {
	adc_regb_t *r = (adc_regb_t*)(SOC_SADC_REG_BASE + (0xb << 2));
	r->cwt_cal04 = v;
}

static inline uint32_t adc_ll_get_regb_cwt_cal04(void) {
	adc_regb_t *r = (adc_regb_t*)(SOC_SADC_REG_BASE + (0xb << 2));
	return r->cwt_cal04;
}

//reg regc:

static inline void adc_ll_set_regc_value(uint32_t v) {
	adc_regc_t *r = (adc_regc_t*)(SOC_SADC_REG_BASE + (0xc << 2));
	r->v = v;
}

static inline uint32_t adc_ll_get_regc_value(void) {
	adc_regc_t *r = (adc_regc_t*)(SOC_SADC_REG_BASE + (0xc << 2));
	return r->v;
}

static inline void adc_ll_set_regc_cwt_cal05(uint32_t v) {
	adc_regc_t *r = (adc_regc_t*)(SOC_SADC_REG_BASE + (0xc << 2));
	r->cwt_cal05 = v;
}

static inline uint32_t adc_ll_get_regc_cwt_cal05(void) {
	adc_regc_t *r = (adc_regc_t*)(SOC_SADC_REG_BASE + (0xc << 2));
	return r->cwt_cal05;
}

//reg regd:

static inline void adc_ll_set_regd_value(uint32_t v) {
	adc_regd_t *r = (adc_regd_t*)(SOC_SADC_REG_BASE + (0xd << 2));
	r->v = v;
}

static inline uint32_t adc_ll_get_regd_value(void) {
	adc_regd_t *r = (adc_regd_t*)(SOC_SADC_REG_BASE + (0xd << 2));
	return r->v;
}

static inline void adc_ll_set_regd_cwt_cal06(uint32_t v) {
	adc_regd_t *r = (adc_regd_t*)(SOC_SADC_REG_BASE + (0xd << 2));
	r->cwt_cal06 = v;
}

static inline uint32_t adc_ll_get_regd_cwt_cal06(void) {
	adc_regd_t *r = (adc_regd_t*)(SOC_SADC_REG_BASE + (0xd << 2));
	return r->cwt_cal06;
}

//reg rege:

static inline void adc_ll_set_rege_value(uint32_t v) {
	adc_rege_t *r = (adc_rege_t*)(SOC_SADC_REG_BASE + (0xe << 2));
	r->v = v;
}

static inline uint32_t adc_ll_get_rege_value(void) {
	adc_rege_t *r = (adc_rege_t*)(SOC_SADC_REG_BASE + (0xe << 2));
	return r->v;
}

static inline void adc_ll_set_rege_cwt_cal07(uint32_t v) {
	adc_rege_t *r = (adc_rege_t*)(SOC_SADC_REG_BASE + (0xe << 2));
	r->cwt_cal07 = v;
}

static inline uint32_t adc_ll_get_rege_cwt_cal07(void) {
	adc_rege_t *r = (adc_rege_t*)(SOC_SADC_REG_BASE + (0xe << 2));
	return r->cwt_cal07;
}

//reg regf:

static inline void adc_ll_set_regf_value(uint32_t v) {
	adc_regf_t *r = (adc_regf_t*)(SOC_SADC_REG_BASE + (0xf << 2));
	r->v = v;
}

static inline uint32_t adc_ll_get_regf_value(void) {
	adc_regf_t *r = (adc_regf_t*)(SOC_SADC_REG_BASE + (0xf << 2));
	return r->v;
}

static inline void adc_ll_set_regf_cwt_cal08(uint32_t v) {
	adc_regf_t *r = (adc_regf_t*)(SOC_SADC_REG_BASE + (0xf << 2));
	r->cwt_cal08 = v;
}

static inline uint32_t adc_ll_get_regf_cwt_cal08(void) {
	adc_regf_t *r = (adc_regf_t*)(SOC_SADC_REG_BASE + (0xf << 2));
	return r->cwt_cal08;
}

//reg reg10:

static inline void adc_ll_set_reg10_value(uint32_t v) {
	adc_reg10_t *r = (adc_reg10_t*)(SOC_SADC_REG_BASE + (0x10 << 2));
	r->v = v;
}

static inline uint32_t adc_ll_get_reg10_value(void) {
	adc_reg10_t *r = (adc_reg10_t*)(SOC_SADC_REG_BASE + (0x10 << 2));
	return r->v;
}

static inline void adc_ll_set_reg10_cwt_cal09(uint32_t v) {
	adc_reg10_t *r = (adc_reg10_t*)(SOC_SADC_REG_BASE + (0x10 << 2));
	r->cwt_cal09 = v;
}

static inline uint32_t adc_ll_get_reg10_cwt_cal09(void) {
	adc_reg10_t *r = (adc_reg10_t*)(SOC_SADC_REG_BASE + (0x10 << 2));
	return r->cwt_cal09;
}

//reg reg11:

static inline void adc_ll_set_reg11_value(uint32_t v) {
	adc_reg11_t *r = (adc_reg11_t*)(SOC_SADC_REG_BASE + (0x11 << 2));
	r->v = v;
}

static inline uint32_t adc_ll_get_reg11_value(void) {
	adc_reg11_t *r = (adc_reg11_t*)(SOC_SADC_REG_BASE + (0x11 << 2));
	return r->v;
}

static inline void adc_ll_set_reg11_cwt_cal0a(uint32_t v) {
	adc_reg11_t *r = (adc_reg11_t*)(SOC_SADC_REG_BASE + (0x11 << 2));
	r->cwt_cal0a = v;
}

static inline uint32_t adc_ll_get_reg11_cwt_cal0a(void) {
	adc_reg11_t *r = (adc_reg11_t*)(SOC_SADC_REG_BASE + (0x11 << 2));
	return r->cwt_cal0a;
}

//reg reg12:

static inline void adc_ll_set_reg12_value(uint32_t v) {
	adc_reg12_t *r = (adc_reg12_t*)(SOC_SADC_REG_BASE + (0x12 << 2));
	r->v = v;
}

static inline uint32_t adc_ll_get_reg12_value(void) {
	adc_reg12_t *r = (adc_reg12_t*)(SOC_SADC_REG_BASE + (0x12 << 2));
	return r->v;
}

static inline void adc_ll_set_reg12_cwt_cal0b(uint32_t v) {
	adc_reg12_t *r = (adc_reg12_t*)(SOC_SADC_REG_BASE + (0x12 << 2));
	r->cwt_cal0b = v;
}

static inline uint32_t adc_ll_get_reg12_cwt_cal0b(void) {
	adc_reg12_t *r = (adc_reg12_t*)(SOC_SADC_REG_BASE + (0x12 << 2));
	return r->cwt_cal0b;
}

//reg reg13:

static inline void adc_ll_set_reg13_value(uint32_t v) {
	adc_reg13_t *r = (adc_reg13_t*)(SOC_SADC_REG_BASE + (0x13 << 2));
	r->v = v;
}

static inline uint32_t adc_ll_get_reg13_value(void) {
	adc_reg13_t *r = (adc_reg13_t*)(SOC_SADC_REG_BASE + (0x13 << 2));
	return r->v;
}

static inline void adc_ll_set_reg13_cwt_cal0c(uint32_t v) {
	adc_reg13_t *r = (adc_reg13_t*)(SOC_SADC_REG_BASE + (0x13 << 2));
	r->cwt_cal0c = v;
}

static inline uint32_t adc_ll_get_reg13_cwt_cal0c(void) {
	adc_reg13_t *r = (adc_reg13_t*)(SOC_SADC_REG_BASE + (0x13 << 2));
	return r->cwt_cal0c;
}

//reg reg14:

static inline void adc_ll_set_reg14_value(uint32_t v) {
	adc_reg14_t *r = (adc_reg14_t*)(SOC_SADC_REG_BASE + (0x14 << 2));
	r->v = v;
}

static inline uint32_t adc_ll_get_reg14_value(void) {
	adc_reg14_t *r = (adc_reg14_t*)(SOC_SADC_REG_BASE + (0x14 << 2));
	return r->v;
}

static inline void adc_ll_set_reg14_cwt_cal0d(uint32_t v) {
	adc_reg14_t *r = (adc_reg14_t*)(SOC_SADC_REG_BASE + (0x14 << 2));
	r->cwt_cal0d = v;
}

static inline uint32_t adc_ll_get_reg14_cwt_cal0d(void) {
	adc_reg14_t *r = (adc_reg14_t*)(SOC_SADC_REG_BASE + (0x14 << 2));
	return r->cwt_cal0d;
}

//reg reg15:

static inline void adc_ll_set_reg15_value(uint32_t v) {
	adc_reg15_t *r = (adc_reg15_t*)(SOC_SADC_REG_BASE + (0x15 << 2));
	r->v = v;
}

static inline uint32_t adc_ll_get_reg15_value(void) {
	adc_reg15_t *r = (adc_reg15_t*)(SOC_SADC_REG_BASE + (0x15 << 2));
	return r->v;
}

static inline void adc_ll_set_reg15_cwt_cal0e(uint32_t v) {
	adc_reg15_t *r = (adc_reg15_t*)(SOC_SADC_REG_BASE + (0x15 << 2));
	r->cwt_cal0e = v;
}

static inline uint32_t adc_ll_get_reg15_cwt_cal0e(void) {
	adc_reg15_t *r = (adc_reg15_t*)(SOC_SADC_REG_BASE + (0x15 << 2));
	return r->cwt_cal0e;
}

//reg reg16:

static inline void adc_ll_set_reg16_value(uint32_t v) {
	adc_reg16_t *r = (adc_reg16_t*)(SOC_SADC_REG_BASE + (0x16 << 2));
	r->v = v;
}

static inline uint32_t adc_ll_get_reg16_value(void) {
	adc_reg16_t *r = (adc_reg16_t*)(SOC_SADC_REG_BASE + (0x16 << 2));
	return r->v;
}

static inline void adc_ll_set_reg16_cwt_cal0f(uint32_t v) {
	adc_reg16_t *r = (adc_reg16_t*)(SOC_SADC_REG_BASE + (0x16 << 2));
	r->cwt_cal0f = v;
}

static inline uint32_t adc_ll_get_reg16_cwt_cal0f(void) {
	adc_reg16_t *r = (adc_reg16_t*)(SOC_SADC_REG_BASE + (0x16 << 2));
	return r->cwt_cal0f;
}

//reg reg17:

static inline void adc_ll_set_reg17_value(uint32_t v) {
	adc_reg17_t *r = (adc_reg17_t*)(SOC_SADC_REG_BASE + (0x17 << 2));
	r->v = v;
}

static inline uint32_t adc_ll_get_reg17_value(void) {
	adc_reg17_t *r = (adc_reg17_t*)(SOC_SADC_REG_BASE + (0x17 << 2));
	return r->v;
}

static inline void adc_ll_set_reg17_adc_int_en(uint32_t v) {
	adc_reg17_t *r = (adc_reg17_t*)(SOC_SADC_REG_BASE + (0x17 << 2));
	r->adc_int_en = v;
}

static inline uint32_t adc_ll_get_reg17_adc_int_en(void) {
	adc_reg17_t *r = (adc_reg17_t*)(SOC_SADC_REG_BASE + (0x17 << 2));
	return r->adc_int_en;
}

static inline void adc_ll_set_reg17_adc_int_level(uint32_t v) {
	adc_reg17_t *r = (adc_reg17_t*)(SOC_SADC_REG_BASE + (0x17 << 2));
	r->adc_int_level = v;
}

static inline uint32_t adc_ll_get_reg17_adc_int_level(void) {
	adc_reg17_t *r = (adc_reg17_t*)(SOC_SADC_REG_BASE + (0x17 << 2));
	return r->adc_int_level;
}

//reg reg18:

static inline void adc_ll_set_reg18_value(uint32_t v) {
	adc_reg18_t *r = (adc_reg18_t*)(SOC_SADC_REG_BASE + (0x18 << 2));
	r->v = v;
}

static inline uint32_t adc_ll_get_reg18_value(void) {
	adc_reg18_t *r = (adc_reg18_t*)(SOC_SADC_REG_BASE + (0x18 << 2));
	return r->v;
}

static inline uint32_t adc_ll_get_reg18_fifo_rdata(void) {
	adc_reg18_t *r = (adc_reg18_t*)(SOC_SADC_REG_BASE + (0x18 << 2));
	return r->fifo_rdata;
}

//reg reg19:

static inline void adc_ll_set_reg19_value(uint32_t v) {
	adc_reg19_t *r = (adc_reg19_t*)(SOC_SADC_REG_BASE + (0x19 << 2));
	r->v = v;
}

static inline uint32_t adc_ll_get_reg19_value(void) {
	adc_reg19_t *r = (adc_reg19_t*)(SOC_SADC_REG_BASE + (0x19 << 2));
	return r->v;
}

static inline uint32_t adc_ll_get_reg19_fifo_full(void) {
	adc_reg19_t *r = (adc_reg19_t*)(SOC_SADC_REG_BASE + (0x19 << 2));
	return r->fifo_full;
}

static inline void adc_ll_set_reg19_fifo_empty(uint32_t v) {
	adc_reg19_t *r = (adc_reg19_t*)(SOC_SADC_REG_BASE + (0x19 << 2));
	r->fifo_empty = v;
}

static inline uint32_t adc_ll_get_reg19_fifo_empty(void) {
	adc_reg19_t *r = (adc_reg19_t*)(SOC_SADC_REG_BASE + (0x19 << 2));
	return r->fifo_empty;
}

static inline void adc_ll_set_reg19_int_state(uint32_t v) {
	adc_reg19_t *r = (adc_reg19_t*)(SOC_SADC_REG_BASE + (0x19 << 2));
	r->int_state = v;
}

static inline uint32_t adc_ll_get_reg19_int_state(void) {
	adc_reg19_t *r = (adc_reg19_t*)(SOC_SADC_REG_BASE + (0x19 << 2));
	return r->int_state;
}
#ifdef __cplusplus
}
#endif
