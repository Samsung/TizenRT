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

#include "hal_config.h"
#include "adc_hw.h"
#include "adc_hal.h"

typedef void (*adc_dump_fn_t)(void);
typedef struct {
	uint32_t start;
	uint32_t end;
	adc_dump_fn_t fn;
} adc_reg_fn_map_t;

static void adc_dump_reg0(void)
{
	SOC_LOGI("reg0: %8x\r\n", REG_READ(SOC_SADC_REG_BASE + (0x0 << 2)));
}

static void adc_dump_reg1(void)
{
	SOC_LOGI("reg1: %8x\r\n", REG_READ(SOC_SADC_REG_BASE + (0x1 << 2)));
}

static void adc_dump_reg2(void)
{
	adc_reg2_t *r = (adc_reg2_t *)(SOC_SADC_REG_BASE + (0x2 << 2));

	SOC_LOGI("reg2: %8x\r\n", REG_READ(SOC_SADC_REG_BASE + (0x2 << 2)));
	SOC_LOGI("	soft_rst: %8x\r\n", r->soft_rst);
	SOC_LOGI("	bypass_ckg: %8x\r\n", r->bypass_ckg);
	SOC_LOGI("	reserved_bit_2_31: %8x\r\n", r->reserved_bit_2_31);
}

static void adc_dump_reg3(void)
{
	SOC_LOGI("reg3: %8x\r\n", REG_READ(SOC_SADC_REG_BASE + (0x3 << 2)));
}

static void adc_dump_reg4(void)
{
	adc_reg4_t *r = (adc_reg4_t *)(SOC_SADC_REG_BASE + (0x4 << 2));

	SOC_LOGI("reg4: %8x\r\n", REG_READ(SOC_SADC_REG_BASE + (0x4 << 2)));
	SOC_LOGI("	enable: %8x\r\n", r->enable);
	SOC_LOGI("	calib_mode: %8x\r\n", r->calib_mode);
	SOC_LOGI("	calib_done: %8x\r\n", r->calib_done);
	SOC_LOGI("	samp_sel: %8x\r\n", r->samp_sel);
	SOC_LOGI("	adc_chan: %8x\r\n", r->adc_chan);
	SOC_LOGI("	adc_mode: %8x\r\n", r->adc_mode);
	SOC_LOGI("	adc_dump_num: %8x\r\n", r->adc_dump_num);
	SOC_LOGI("	adc_sclk_div: %8x\r\n", r->adc_sclk_div);
	SOC_LOGI("	adc_sraw: %8x\r\n", r->adc_sraw);
	SOC_LOGI("	reserved_bit_17_31: %8x\r\n", r->reserved_bit_17_31);
}

static void adc_dump_reg5(void)
{
	adc_reg5_t *r = (adc_reg5_t *)(SOC_SADC_REG_BASE + (0x5 << 2));

	SOC_LOGI("reg5: %8x\r\n", REG_READ(SOC_SADC_REG_BASE + (0x5 << 2)));
	SOC_LOGI("	co_gain: %8x\r\n", r->co_gain);
	SOC_LOGI("	reserved_bit_8_31: %8x\r\n", r->reserved_bit_8_31);
}

static void adc_dump_reg6(void)
{
	adc_reg6_t *r = (adc_reg6_t *)(SOC_SADC_REG_BASE + (0x6 << 2));

	SOC_LOGI("reg6: %8x\r\n", REG_READ(SOC_SADC_REG_BASE + (0x6 << 2)));
	SOC_LOGI("	co_offset: %8x\r\n", r->co_offset);
	SOC_LOGI("	reserved_bit_16_31: %8x\r\n", r->reserved_bit_16_31);
}

static void adc_dump_reg7(void)
{
	adc_reg7_t *r = (adc_reg7_t *)(SOC_SADC_REG_BASE + (0x7 << 2));

	SOC_LOGI("reg7: %8x\r\n", REG_READ(SOC_SADC_REG_BASE + (0x7 << 2)));
	SOC_LOGI("	cwt_cal00: %8x\r\n", r->cwt_cal00);
	SOC_LOGI("	reserved_bit_24_31: %8x\r\n", r->reserved_bit_24_31);
}

static void adc_dump_reg8(void)
{
	adc_reg8_t *r = (adc_reg8_t *)(SOC_SADC_REG_BASE + (0x8 << 2));

	SOC_LOGI("reg8: %8x\r\n", REG_READ(SOC_SADC_REG_BASE + (0x8 << 2)));
	SOC_LOGI("	cwt_cal01: %8x\r\n", r->cwt_cal01);
	SOC_LOGI("	reserved_bit_24_31: %8x\r\n", r->reserved_bit_24_31);
}

static void adc_dump_reg9(void)
{
	adc_reg9_t *r = (adc_reg9_t *)(SOC_SADC_REG_BASE + (0x9 << 2));

	SOC_LOGI("reg9: %8x\r\n", REG_READ(SOC_SADC_REG_BASE + (0x9 << 2)));
	SOC_LOGI("	cwt_cal02: %8x\r\n", r->cwt_cal02);
	SOC_LOGI("	reserved_bit_24_31: %8x\r\n", r->reserved_bit_24_31);
}

static void adc_dump_rega(void)
{
	adc_rega_t *r = (adc_rega_t *)(SOC_SADC_REG_BASE + (0xa << 2));

	SOC_LOGI("rega: %8x\r\n", REG_READ(SOC_SADC_REG_BASE + (0xa << 2)));
	SOC_LOGI("	cwt_cal03: %8x\r\n", r->cwt_cal03);
	SOC_LOGI("	reserved_bit_24_31: %8x\r\n", r->reserved_bit_24_31);
}

static void adc_dump_regb(void)
{
	adc_regb_t *r = (adc_regb_t *)(SOC_SADC_REG_BASE + (0xb << 2));

	SOC_LOGI("regb: %8x\r\n", REG_READ(SOC_SADC_REG_BASE + (0xb << 2)));
	SOC_LOGI("	cwt_cal04: %8x\r\n", r->cwt_cal04);
	SOC_LOGI("	reserved_bit_24_31: %8x\r\n", r->reserved_bit_24_31);
}

static void adc_dump_regc(void)
{
	adc_regc_t *r = (adc_regc_t *)(SOC_SADC_REG_BASE + (0xc << 2));

	SOC_LOGI("regc: %8x\r\n", REG_READ(SOC_SADC_REG_BASE + (0xc << 2)));
	SOC_LOGI("	cwt_cal05: %8x\r\n", r->cwt_cal05);
	SOC_LOGI("	reserved_bit_24_31: %8x\r\n", r->reserved_bit_24_31);
}

static void adc_dump_regd(void)
{
	adc_regd_t *r = (adc_regd_t *)(SOC_SADC_REG_BASE + (0xd << 2));

	SOC_LOGI("regd: %8x\r\n", REG_READ(SOC_SADC_REG_BASE + (0xd << 2)));
	SOC_LOGI("	cwt_cal06: %8x\r\n", r->cwt_cal06);
	SOC_LOGI("	reserved_bit_24_31: %8x\r\n", r->reserved_bit_24_31);
}

static void adc_dump_rege(void)
{
	adc_rege_t *r = (adc_rege_t *)(SOC_SADC_REG_BASE + (0xe << 2));

	SOC_LOGI("rege: %8x\r\n", REG_READ(SOC_SADC_REG_BASE + (0xe << 2)));
	SOC_LOGI("	cwt_cal07: %8x\r\n", r->cwt_cal07);
	SOC_LOGI("	reserved_bit_24_31: %8x\r\n", r->reserved_bit_24_31);
}

static void adc_dump_regf(void)
{
	adc_regf_t *r = (adc_regf_t *)(SOC_SADC_REG_BASE + (0xf << 2));

	SOC_LOGI("regf: %8x\r\n", REG_READ(SOC_SADC_REG_BASE + (0xf << 2)));
	SOC_LOGI("	cwt_cal08: %8x\r\n", r->cwt_cal08);
	SOC_LOGI("	reserved_bit_24_31: %8x\r\n", r->reserved_bit_24_31);
}

static void adc_dump_reg10(void)
{
	adc_reg10_t *r = (adc_reg10_t *)(SOC_SADC_REG_BASE + (0x10 << 2));

	SOC_LOGI("reg10: %8x\r\n", REG_READ(SOC_SADC_REG_BASE + (0x10 << 2)));
	SOC_LOGI("	cwt_cal09: %8x\r\n", r->cwt_cal09);
	SOC_LOGI("	reserved_bit_24_31: %8x\r\n", r->reserved_bit_24_31);
}

static void adc_dump_reg11(void)
{
	adc_reg11_t *r = (adc_reg11_t *)(SOC_SADC_REG_BASE + (0x11 << 2));

	SOC_LOGI("reg11: %8x\r\n", REG_READ(SOC_SADC_REG_BASE + (0x11 << 2)));
	SOC_LOGI("	cwt_cal0a: %8x\r\n", r->cwt_cal0a);
	SOC_LOGI("	reserved_bit_24_31: %8x\r\n", r->reserved_bit_24_31);
}

static void adc_dump_reg12(void)
{
	adc_reg12_t *r = (adc_reg12_t *)(SOC_SADC_REG_BASE + (0x12 << 2));

	SOC_LOGI("reg12: %8x\r\n", REG_READ(SOC_SADC_REG_BASE + (0x12 << 2)));
	SOC_LOGI("	cwt_cal0b: %8x\r\n", r->cwt_cal0b);
	SOC_LOGI("	reserved_bit_24_31: %8x\r\n", r->reserved_bit_24_31);
}

static void adc_dump_reg13(void)
{
	adc_reg13_t *r = (adc_reg13_t *)(SOC_SADC_REG_BASE + (0x13 << 2));

	SOC_LOGI("reg13: %8x\r\n", REG_READ(SOC_SADC_REG_BASE + (0x13 << 2)));
	SOC_LOGI("	cwt_cal0c: %8x\r\n", r->cwt_cal0c);
	SOC_LOGI("	reserved_bit_24_31: %8x\r\n", r->reserved_bit_24_31);
}

static void adc_dump_reg14(void)
{
	adc_reg14_t *r = (adc_reg14_t *)(SOC_SADC_REG_BASE + (0x14 << 2));

	SOC_LOGI("reg14: %8x\r\n", REG_READ(SOC_SADC_REG_BASE + (0x14 << 2)));
	SOC_LOGI("	cwt_cal0d: %8x\r\n", r->cwt_cal0d);
	SOC_LOGI("	reserved_bit_24_31: %8x\r\n", r->reserved_bit_24_31);
}

static void adc_dump_reg15(void)
{
	adc_reg15_t *r = (adc_reg15_t *)(SOC_SADC_REG_BASE + (0x15 << 2));

	SOC_LOGI("reg15: %8x\r\n", REG_READ(SOC_SADC_REG_BASE + (0x15 << 2)));
	SOC_LOGI("	cwt_cal0e: %8x\r\n", r->cwt_cal0e);
	SOC_LOGI("	reserved_bit_24_31: %8x\r\n", r->reserved_bit_24_31);
}

static void adc_dump_reg16(void)
{
	adc_reg16_t *r = (adc_reg16_t *)(SOC_SADC_REG_BASE + (0x16 << 2));

	SOC_LOGI("reg16: %8x\r\n", REG_READ(SOC_SADC_REG_BASE + (0x16 << 2)));
	SOC_LOGI("	cwt_cal0f: %8x\r\n", r->cwt_cal0f);
	SOC_LOGI("	reserved_bit_24_31: %8x\r\n", r->reserved_bit_24_31);
}

static void adc_dump_reg17(void)
{
	adc_reg17_t *r = (adc_reg17_t *)(SOC_SADC_REG_BASE + (0x17 << 2));

	SOC_LOGI("reg17: %8x\r\n", REG_READ(SOC_SADC_REG_BASE + (0x17 << 2)));
	SOC_LOGI("	adc_int_en: %8x\r\n", r->adc_int_en);
	SOC_LOGI("	adc_int_level: %8x\r\n", r->adc_int_level);
	SOC_LOGI("	reserved_bit_7_31: %8x\r\n", r->reserved_bit_7_31);
}

static void adc_dump_reg18(void)
{
	adc_reg18_t *r = (adc_reg18_t *)(SOC_SADC_REG_BASE + (0x18 << 2));

	SOC_LOGI("reg18: %8x\r\n", REG_READ(SOC_SADC_REG_BASE + (0x18 << 2)));
	SOC_LOGI("	fifo_rdata: %8x\r\n", r->fifo_rdata);
	SOC_LOGI("	reserved_bit_16_31: %8x\r\n", r->reserved_bit_16_31);
}

static void adc_dump_reg19(void)
{
	adc_reg19_t *r = (adc_reg19_t *)(SOC_SADC_REG_BASE + (0x19 << 2));

	SOC_LOGI("reg19: %8x\r\n", REG_READ(SOC_SADC_REG_BASE + (0x19 << 2)));
	SOC_LOGI("	fifo_full: %8x\r\n", r->fifo_full);
	SOC_LOGI("	fifo_empty: %8x\r\n", r->fifo_empty);
	SOC_LOGI("	int_state: %8x\r\n", r->int_state);
	SOC_LOGI("	reserved_bit_3_31: %8x\r\n", r->reserved_bit_3_31);
}

static adc_reg_fn_map_t s_fn[] =
{
	{0x0, 0x0, adc_dump_reg0},
	{0x1, 0x1, adc_dump_reg1},
	{0x2, 0x2, adc_dump_reg2},
	{0x3, 0x3, adc_dump_reg3},
	{0x4, 0x4, adc_dump_reg4},
	{0x5, 0x5, adc_dump_reg5},
	{0x6, 0x6, adc_dump_reg6},
	{0x7, 0x7, adc_dump_reg7},
	{0x8, 0x8, adc_dump_reg8},
	{0x9, 0x9, adc_dump_reg9},
	{0xa, 0xa, adc_dump_rega},
	{0xb, 0xb, adc_dump_regb},
	{0xc, 0xc, adc_dump_regc},
	{0xd, 0xd, adc_dump_regd},
	{0xe, 0xe, adc_dump_rege},
	{0xf, 0xf, adc_dump_regf},
	{0x10, 0x10, adc_dump_reg10},
	{0x11, 0x11, adc_dump_reg11},
	{0x12, 0x12, adc_dump_reg12},
	{0x13, 0x13, adc_dump_reg13},
	{0x14, 0x14, adc_dump_reg14},
	{0x15, 0x15, adc_dump_reg15},
	{0x16, 0x16, adc_dump_reg16},
	{0x17, 0x17, adc_dump_reg17},
	{0x18, 0x18, adc_dump_reg18},
	{0x19, 0x19, adc_dump_reg19},
	{-1, -1, 0}
};

void adc_struct_dump(uint32_t start, uint32_t end)
{
	uint32_t dump_fn_cnt = sizeof(s_fn)/sizeof(s_fn[0]) - 1;

	for (uint32_t idx = 0; idx < dump_fn_cnt; idx++) {
		if ((start <= s_fn[idx].start) && (end >= s_fn[idx].end)) {
			s_fn[idx].fn();
		}
	}
}
