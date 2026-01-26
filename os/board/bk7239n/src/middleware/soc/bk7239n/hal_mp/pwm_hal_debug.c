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
#include "pwm_hw.h"
#include "pwm_hal.h"

#if CFG_HAL_DEBUG_PWM
typedef void (*pwm_dump_fn_t)(void);
typedef struct {
	uint32_t start;
	uint32_t end;
	pwm_dump_fn_t fn;
} pwm_reg_fn_map_t;

static void pwm_dump_reg0(void)
{
	SOC_LOGI("reg0: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x0 << 2)));
}

static void pwm_dump_reg1(void)
{
	SOC_LOGI("reg1: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x1 << 2)));
}

static void pwm_dump_reg2(void)
{
	pwm_reg2_t *r = (pwm_reg2_t *)(SOC_PWM_REG_BASE + (0x2 << 2));

	SOC_LOGI("reg2: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x2 << 2)));
	SOC_LOGI("	soft_reset: %8x\r\n", r->soft_reset);
	SOC_LOGI("	bps_clkgate: %8x\r\n", r->bps_clkgate);
	SOC_LOGI("	reserved_bit_2_31: %8x\r\n", r->reserved_bit_2_31);
}

static void pwm_dump_reg3(void)
{
	pwm_reg3_t *r = (pwm_reg3_t *)(SOC_PWM_REG_BASE + (0x3 << 2));

	SOC_LOGI("reg3: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x3 << 2)));
	SOC_LOGI("	pwm_o: %8x\r\n", r->pwm_o);
	SOC_LOGI("	reserved_12_31: %8x\r\n", r->reserved_12_31);
}

static void pwm_dump_reg4(void)
{
	pwm_reg4_t *r = (pwm_reg4_t *)(SOC_PWM_REG_BASE + (0x4 << 2));

	SOC_LOGI("reg4: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x4 << 2)));
	SOC_LOGI("	cen1: %8x\r\n", r->cen1);
	SOC_LOGI("	cen2: %8x\r\n", r->cen2);
	SOC_LOGI("	cen3: %8x\r\n", r->cen3);
	SOC_LOGI("	cen4: %8x\r\n", r->cen4);
	SOC_LOGI("	cen5: %8x\r\n", r->cen5);
	SOC_LOGI("	cen6: %8x\r\n", r->cen6);
	SOC_LOGI("	cen7: %8x\r\n", r->cen7);
	SOC_LOGI("	cen8: %8x\r\n", r->cen8);
	SOC_LOGI("	cen9: %8x\r\n", r->cen9);
	SOC_LOGI("	cen10: %8x\r\n", r->cen10);
	SOC_LOGI("	cen11: %8x\r\n", r->cen11);
	SOC_LOGI("	cen12: %8x\r\n", r->cen12);
	SOC_LOGI("	sync_all: %8x\r\n", r->sync_all);
	SOC_LOGI("	sync_all_en: %8x\r\n", r->sync_all_en);
	SOC_LOGI("	reserved_14_31: %8x\r\n", r->reserved_14_31);
}

static void pwm_dump_rsv_5_f(void)
{
	for (uint32_t idx = 0; idx < 11; idx++) {
		SOC_LOGI("rsv_5_f: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + ((0x5 + idx) << 2)));
	}
}

static void pwm_dump_reg10(void)
{
	SOC_LOGI("reg10: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x10 << 2)));
}

static void pwm_dump_reg11(void)
{
	SOC_LOGI("reg11: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x11 << 2)));
}

static void pwm_dump_reg12(void)
{
	SOC_LOGI("reg12: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x12 << 2)));
}

static void pwm_dump_reg13(void)
{
	SOC_LOGI("reg13: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x13 << 2)));
}

static void pwm_dump_reg14(void)
{
	SOC_LOGI("reg14: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x14 << 2)));
}

static void pwm_dump_reg15(void)
{
	SOC_LOGI("reg15: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x15 << 2)));
}

static void pwm_dump_reg16(void)
{
	SOC_LOGI("reg16: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x16 << 2)));
}

static void pwm_dump_reg17(void)
{
	SOC_LOGI("reg17: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x17 << 2)));
}

static void pwm_dump_reg18(void)
{
	SOC_LOGI("reg18: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x18 << 2)));
}

static void pwm_dump_reg19(void)
{
	SOC_LOGI("reg19: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x19 << 2)));
}

static void pwm_dump_reg1a(void)
{
	SOC_LOGI("reg1a: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x1a << 2)));
}

static void pwm_dump_reg1b(void)
{
	SOC_LOGI("reg1b: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x1b << 2)));
}

static void pwm_dump_rsv_1c_1f(void)
{
	for (uint32_t idx = 0; idx < 4; idx++) {
		SOC_LOGI("rsv_1c_1f: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + ((0x1c + idx) << 2)));
	}
}

static void pwm_dump_reg20(void)
{
	SOC_LOGI("reg20: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x20 << 2)));
}

static void pwm_dump_reg21(void)
{
	SOC_LOGI("reg21: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x21 << 2)));
}

static void pwm_dump_reg22(void)
{
	SOC_LOGI("reg22: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x22 << 2)));
}

static void pwm_dump_reg23(void)
{
	SOC_LOGI("reg23: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x23 << 2)));
}

static void pwm_dump_reg24(void)
{
	SOC_LOGI("reg24: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x24 << 2)));
}

static void pwm_dump_reg25(void)
{
	SOC_LOGI("reg25: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x25 << 2)));
}

static void pwm_dump_reg26(void)
{
	SOC_LOGI("reg26: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x26 << 2)));
}

static void pwm_dump_reg27(void)
{
	SOC_LOGI("reg27: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x27 << 2)));
}

static void pwm_dump_reg28(void)
{
	SOC_LOGI("reg28: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x28 << 2)));
}

static void pwm_dump_reg29(void)
{
	SOC_LOGI("reg29: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x29 << 2)));
}

static void pwm_dump_reg2a(void)
{
	SOC_LOGI("reg2a: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x2a << 2)));
}

static void pwm_dump_reg2b(void)
{
	SOC_LOGI("reg2b: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x2b << 2)));
}

static void pwm_dump_reg2c(void)
{
	SOC_LOGI("reg2c: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x2c << 2)));
}

static void pwm_dump_reg2d(void)
{
	SOC_LOGI("reg2d: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x2d << 2)));
}

static void pwm_dump_reg2e(void)
{
	SOC_LOGI("reg2e: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x2e << 2)));
}

static void pwm_dump_reg2f(void)
{
	SOC_LOGI("reg2f: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x2f << 2)));
}

static void pwm_dump_reg30(void)
{
	SOC_LOGI("reg30: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x30 << 2)));
}

static void pwm_dump_reg31(void)
{
	SOC_LOGI("reg31: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x31 << 2)));
}

static void pwm_dump_reg32(void)
{
	SOC_LOGI("reg32: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x32 << 2)));
}

static void pwm_dump_reg33(void)
{
	SOC_LOGI("reg33: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x33 << 2)));
}

static void pwm_dump_reg34(void)
{
	SOC_LOGI("reg34: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x34 << 2)));
}

static void pwm_dump_reg35(void)
{
	SOC_LOGI("reg35: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x35 << 2)));
}

static void pwm_dump_reg36(void)
{
	SOC_LOGI("reg36: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x36 << 2)));
}

static void pwm_dump_reg37(void)
{
	SOC_LOGI("reg37: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x37 << 2)));
}

static void pwm_dump_rsv_38_3f(void)
{
	for (uint32_t idx = 0; idx < 8; idx++) {
		SOC_LOGI("rsv_38_3f: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + ((0x38 + idx) << 2)));
	}
}

static void pwm_dump_reg40(void)
{
	pwm_reg40_t *r = (pwm_reg40_t *)(SOC_PWM_REG_BASE + (0x40 << 2));

	SOC_LOGI("reg40: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x40 << 2)));
	SOC_LOGI("	ocm3pe: %8x\r\n", r->ocm3pe);
	SOC_LOGI("	ocm2pe: %8x\r\n", r->ocm2pe);
	SOC_LOGI("	ocm1pe: %8x\r\n", r->ocm1pe);
	SOC_LOGI("	arpe3: %8x\r\n", r->arpe3);
	SOC_LOGI("	arpe2: %8x\r\n", r->arpe2);
	SOC_LOGI("	arpe1: %8x\r\n", r->arpe1);
	SOC_LOGI("	oc3pe: %8x\r\n", r->oc3pe);
	SOC_LOGI("	oc2pe: %8x\r\n", r->oc2pe);
	SOC_LOGI("	oc1pe: %8x\r\n", r->oc1pe);
	SOC_LOGI("	reserved_9_17: %8x\r\n", r->reserved_9_17);
	SOC_LOGI("	cms_tim3: %8x\r\n", r->cms_tim3);
	SOC_LOGI("	cms_tim2: %8x\r\n", r->cms_tim2);
	SOC_LOGI("	cms_tim1: %8x\r\n", r->cms_tim1);
	SOC_LOGI("	reserved_24_25: %8x\r\n", r->reserved_24_25);
	SOC_LOGI("	urs3: %8x\r\n", r->urs3);
	SOC_LOGI("	urs2: %8x\r\n", r->urs2);
	SOC_LOGI("	urs1: %8x\r\n", r->urs1);
	SOC_LOGI("	dir_tim3: %8x\r\n", r->dir_tim3);
	SOC_LOGI("	dir_tim2: %8x\r\n", r->dir_tim2);
	SOC_LOGI("	dir_tim1: %8x\r\n", r->dir_tim1);
}

static void pwm_dump_reg41(void)
{
	pwm_reg41_t *r = (pwm_reg41_t *)(SOC_PWM_REG_BASE + (0x41 << 2));

	SOC_LOGI("reg41: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x41 << 2)));
	SOC_LOGI("	reserved_0_7: %8x\r\n", r->reserved_0_7);
	SOC_LOGI("	com_mode: %8x\r\n", r->com_mode);
	SOC_LOGI("	ccpc: %8x\r\n", r->ccpc);
	SOC_LOGI("	trios3: %8x\r\n", r->trios3);
	SOC_LOGI("	adcs3: %8x\r\n", r->adcs3);
	SOC_LOGI("	reserved_16_16: %8x\r\n", r->reserved_16_16);
	SOC_LOGI("	ccus: %8x\r\n", r->ccus);
	SOC_LOGI("	trios2: %8x\r\n", r->trios2);
	SOC_LOGI("	adcs2: %8x\r\n", r->adcs2);
	SOC_LOGI("	reserved_24_25: %8x\r\n", r->reserved_24_25);
	SOC_LOGI("	trios1: %8x\r\n", r->trios1);
	SOC_LOGI("	adcs1: %8x\r\n", r->adcs1);
}

static void pwm_dump_reg42(void)
{
	pwm_reg42_t *r = (pwm_reg42_t *)(SOC_PWM_REG_BASE + (0x42 << 2));

	SOC_LOGI("reg42: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x42 << 2)));
	SOC_LOGI("	sms1: %8x\r\n", r->sms1);
	SOC_LOGI("	sms2: %8x\r\n", r->sms2);
	SOC_LOGI("	sms3: %8x\r\n", r->sms3);
	SOC_LOGI("	ts1: %8x\r\n", r->ts1);
	SOC_LOGI("	reserved_15_15: %8x\r\n", r->reserved_15_15);
	SOC_LOGI("	ts2: %8x\r\n", r->ts2);
	SOC_LOGI("	ts3: %8x\r\n", r->ts3);
	SOC_LOGI("	reserved_22_31: %8x\r\n", r->reserved_22_31);
}

static void pwm_dump_reg43(void)
{
	pwm_reg43_t *r = (pwm_reg43_t *)(SOC_PWM_REG_BASE + (0x43 << 2));

	SOC_LOGI("reg43: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x43 << 2)));
	SOC_LOGI("	cc1ie: %8x\r\n", r->cc1ie);
	SOC_LOGI("	cc2ie: %8x\r\n", r->cc2ie);
	SOC_LOGI("	cc3ie: %8x\r\n", r->cc3ie);
	SOC_LOGI("	cc4ie: %8x\r\n", r->cc4ie);
	SOC_LOGI("	cc5ie: %8x\r\n", r->cc5ie);
	SOC_LOGI("	cc6ie: %8x\r\n", r->cc6ie);
	SOC_LOGI("	cc7ie: %8x\r\n", r->cc7ie);
	SOC_LOGI("	cc8ie: %8x\r\n", r->cc8ie);
	SOC_LOGI("	cc9ie: %8x\r\n", r->cc9ie);
	SOC_LOGI("	uie1: %8x\r\n", r->uie1);
	SOC_LOGI("	uie2: %8x\r\n", r->uie2);
	SOC_LOGI("	uie3: %8x\r\n", r->uie3);
	SOC_LOGI("	tie1: %8x\r\n", r->tie1);
	SOC_LOGI("	tie2: %8x\r\n", r->tie2);
	SOC_LOGI("	tie3: %8x\r\n", r->tie3);
	SOC_LOGI("	comie: %8x\r\n", r->comie);
	SOC_LOGI("	reserved_16_19: %8x\r\n", r->reserved_16_19);
	SOC_LOGI("	df_tim1: %8x\r\n", r->df_tim1);
	SOC_LOGI("	df_tim2: %8x\r\n", r->df_tim2);
	SOC_LOGI("	df_tim3: %8x\r\n", r->df_tim3);
}

static void pwm_dump_reg44(void)
{
	pwm_reg44_t *r = (pwm_reg44_t *)(SOC_PWM_REG_BASE + (0x44 << 2));

	SOC_LOGI("reg44: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x44 << 2)));
	SOC_LOGI("	cc1if: %8x\r\n", r->cc1if);
	SOC_LOGI("	cc2if: %8x\r\n", r->cc2if);
	SOC_LOGI("	cc3if: %8x\r\n", r->cc3if);
	SOC_LOGI("	cc4if: %8x\r\n", r->cc4if);
	SOC_LOGI("	cc5if: %8x\r\n", r->cc5if);
	SOC_LOGI("	cc6if: %8x\r\n", r->cc6if);
	SOC_LOGI("	cc7if: %8x\r\n", r->cc7if);
	SOC_LOGI("	cc8if: %8x\r\n", r->cc8if);
	SOC_LOGI("	cc9if: %8x\r\n", r->cc9if);
	SOC_LOGI("	uif1: %8x\r\n", r->uif1);
	SOC_LOGI("	uif2: %8x\r\n", r->uif2);
	SOC_LOGI("	uif3: %8x\r\n", r->uif3);
	SOC_LOGI("	tif1: %8x\r\n", r->tif1);
	SOC_LOGI("	tif2: %8x\r\n", r->tif2);
	SOC_LOGI("	tif3: %8x\r\n", r->tif3);
	SOC_LOGI("	comif: %8x\r\n", r->comif);
	SOC_LOGI("	reserved_22_31: %8x\r\n", r->reserved_22_31);
}

static void pwm_dump_reg45(void)
{
	pwm_reg45_t *r = (pwm_reg45_t *)(SOC_PWM_REG_BASE + (0x45 << 2));

	SOC_LOGI("reg45: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x45 << 2)));
	SOC_LOGI("	reserved_0_8: %8x\r\n", r->reserved_0_8);
	SOC_LOGI("	ug1: %8x\r\n", r->ug1);
	SOC_LOGI("	ug2: %8x\r\n", r->ug2);
	SOC_LOGI("	ug3: %8x\r\n", r->ug3);
	SOC_LOGI("	tg1: %8x\r\n", r->tg1);
	SOC_LOGI("	tg2: %8x\r\n", r->tg2);
	SOC_LOGI("	tg3: %8x\r\n", r->tg3);
	SOC_LOGI("	comg: %8x\r\n", r->comg);
	SOC_LOGI("	dtm1: %8x\r\n", r->dtm1);
	SOC_LOGI("	dtm2: %8x\r\n", r->dtm2);
	SOC_LOGI("	dtm3: %8x\r\n", r->dtm3);
	SOC_LOGI("	init_level1: %8x\r\n", r->init_level1);
	SOC_LOGI("	init_level2: %8x\r\n", r->init_level2);
	SOC_LOGI("	init_level3: %8x\r\n", r->init_level3);
	SOC_LOGI("	reserved_25_31: %8x\r\n", r->reserved_25_31);
}

static void pwm_dump_reg46(void)
{
	pwm_reg46_t *r = (pwm_reg46_t *)(SOC_PWM_REG_BASE + (0x46 << 2));

	SOC_LOGI("reg46: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x46 << 2)));
	SOC_LOGI("	ch1p: %8x\r\n", r->ch1p);
	SOC_LOGI("	ch2p: %8x\r\n", r->ch2p);
	SOC_LOGI("	ch3p: %8x\r\n", r->ch3p);
	SOC_LOGI("	ch4p: %8x\r\n", r->ch4p);
	SOC_LOGI("	ch5p: %8x\r\n", r->ch5p);
	SOC_LOGI("	ch6p: %8x\r\n", r->ch6p);
	SOC_LOGI("	ch1e: %8x\r\n", r->ch1e);
	SOC_LOGI("	ch2e: %8x\r\n", r->ch2e);
	SOC_LOGI("	ch3e: %8x\r\n", r->ch3e);
	SOC_LOGI("	ch4e: %8x\r\n", r->ch4e);
	SOC_LOGI("	ch5e: %8x\r\n", r->ch5e);
	SOC_LOGI("	ch6e: %8x\r\n", r->ch6e);
	SOC_LOGI("	tim1ccm: %8x\r\n", r->tim1ccm);
	SOC_LOGI("	tim2ccm: %8x\r\n", r->tim2ccm);
	SOC_LOGI("	tim3ccm: %8x\r\n", r->tim3ccm);
	SOC_LOGI("	oc1m: %8x\r\n", r->oc1m);
	SOC_LOGI("	oc2m: %8x\r\n", r->oc2m);
	SOC_LOGI("	oc3m: %8x\r\n", r->oc3m);
	SOC_LOGI("	reserved_30_31: %8x\r\n", r->reserved_30_31);
}

static void pwm_dump_reg47(void)
{
	SOC_LOGI("reg47: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x47 << 2)));
}

static void pwm_dump_reg48(void)
{
	SOC_LOGI("reg48: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x48 << 2)));
}

static void pwm_dump_reg49(void)
{
	SOC_LOGI("reg49: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x49 << 2)));
}

static void pwm_dump_reg4a(void)
{
	pwm_reg4a_t *r = (pwm_reg4a_t *)(SOC_PWM_REG_BASE + (0x4a << 2));

	SOC_LOGI("reg4a: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x4a << 2)));
	SOC_LOGI("	psc1: %8x\r\n", r->psc1);
	SOC_LOGI("	psc2: %8x\r\n", r->psc2);
	SOC_LOGI("	psc3: %8x\r\n", r->psc3);
	SOC_LOGI("	reserved_24_31: %8x\r\n", r->reserved_24_31);
}

static void pwm_dump_reg4b(void)
{
	SOC_LOGI("reg4b: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x4b << 2)));
}

static void pwm_dump_reg4c(void)
{
	SOC_LOGI("reg4c: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x4c << 2)));
}

static void pwm_dump_reg4d(void)
{
	SOC_LOGI("reg4d: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x4d << 2)));
}

static void pwm_dump_reg4e(void)
{
	SOC_LOGI("reg4e: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x4e << 2)));
}

static void pwm_dump_reg4f(void)
{
	SOC_LOGI("reg4f: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x4f << 2)));
}

static void pwm_dump_reg50(void)
{
	SOC_LOGI("reg50: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x50 << 2)));
}

static void pwm_dump_reg51(void)
{
	SOC_LOGI("reg51: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x51 << 2)));
}

static void pwm_dump_reg52(void)
{
	SOC_LOGI("reg52: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x52 << 2)));
}

static void pwm_dump_reg53(void)
{
	SOC_LOGI("reg53: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x53 << 2)));
}

static void pwm_dump_reg54(void)
{
	SOC_LOGI("reg54: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x54 << 2)));
}

static void pwm_dump_reg55(void)
{
	SOC_LOGI("reg55: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x55 << 2)));
}

static void pwm_dump_reg56(void)
{
	SOC_LOGI("reg56: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x56 << 2)));
}

static void pwm_dump_reg57(void)
{
	SOC_LOGI("reg57: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x57 << 2)));
}

static void pwm_dump_reg58(void)
{
	SOC_LOGI("reg58: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x58 << 2)));
}

static void pwm_dump_reg59(void)
{
	SOC_LOGI("reg59: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x59 << 2)));
}

static void pwm_dump_reg5a(void)
{
	pwm_reg5a_t *r = (pwm_reg5a_t *)(SOC_PWM_REG_BASE + (0x5a << 2));

	SOC_LOGI("reg5a: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x5a << 2)));
	SOC_LOGI("	dt1: %8x\r\n", r->dt1);
	SOC_LOGI("	dt2: %8x\r\n", r->dt2);
	SOC_LOGI("	dt3: %8x\r\n", r->dt3);
	SOC_LOGI("	reserved_30_31: %8x\r\n", r->reserved_30_31);
}

static void pwm_dump_reg5b(void)
{
	SOC_LOGI("reg5b: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x5b << 2)));
}

static void pwm_dump_reg5c(void)
{
	SOC_LOGI("reg5c: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x5c << 2)));
}

static void pwm_dump_reg5d(void)
{
	SOC_LOGI("reg5d: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x5d << 2)));
}

static void pwm_dump_reg5e(void)
{
	SOC_LOGI("reg5e: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x5e << 2)));
}

static void pwm_dump_reg5f(void)
{
	SOC_LOGI("reg5f: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x5f << 2)));
}

static void pwm_dump_reg60(void)
{
	SOC_LOGI("reg60: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x60 << 2)));
}

static void pwm_dump_reg61(void)
{
	SOC_LOGI("reg61: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x61 << 2)));
}

static void pwm_dump_reg62(void)
{
	SOC_LOGI("reg62: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x62 << 2)));
}

static void pwm_dump_reg63(void)
{
	SOC_LOGI("reg63: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x63 << 2)));
}

static void pwm_dump_reg64(void)
{
	SOC_LOGI("reg64: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x64 << 2)));
}

static void pwm_dump_reg65(void)
{
	SOC_LOGI("reg65: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x65 << 2)));
}

static void pwm_dump_reg66(void)
{
	SOC_LOGI("reg66: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x66 << 2)));
}

static void pwm_dump_reg67(void)
{
	SOC_LOGI("reg67: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x67 << 2)));
}

static void pwm_dump_reg68(void)
{
	SOC_LOGI("reg68: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x68 << 2)));
}

static void pwm_dump_reg69(void)
{
	SOC_LOGI("reg69: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x69 << 2)));
}

static void pwm_dump_reg6a(void)
{
	pwm_reg6a_t *r = (pwm_reg6a_t *)(SOC_PWM_REG_BASE + (0x6a << 2));

	SOC_LOGI("reg6a: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x6a << 2)));
	SOC_LOGI("	ch6e_shad: %8x\r\n", r->ch6e_shad);
	SOC_LOGI("	ch5e_shad: %8x\r\n", r->ch5e_shad);
	SOC_LOGI("	ch4e_shad: %8x\r\n", r->ch4e_shad);
	SOC_LOGI("	ch3e_shad: %8x\r\n", r->ch3e_shad);
	SOC_LOGI("	ch2e_shad: %8x\r\n", r->ch2e_shad);
	SOC_LOGI("	ch1e_shad: %8x\r\n", r->ch1e_shad);
	SOC_LOGI("	oc1m_shad: %8x\r\n", r->oc1m_shad);
	SOC_LOGI("	oc2m_shad: %8x\r\n", r->oc2m_shad);
	SOC_LOGI("	oc3m_shad: %8x\r\n", r->oc3m_shad);
	SOC_LOGI("	reserved_15_31: %8x\r\n", r->reserved_15_31);
}

static void pwm_dump_rsv_6b_7f(void)
{
	for (uint32_t idx = 0; idx < 21; idx++) {
		SOC_LOGI("rsv_6b_7f: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + ((0x6b + idx) << 2)));
	}
}

static void pwm_dump_reg80(void)
{
	pwm_reg80_t *r = (pwm_reg80_t *)(SOC_PWM_REG_BASE + (0x80 << 2));

	SOC_LOGI("reg80: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x80 << 2)));
	SOC_LOGI("	ocm3pe: %8x\r\n", r->ocm3pe);
	SOC_LOGI("	ocm2pe: %8x\r\n", r->ocm2pe);
	SOC_LOGI("	ocm1pe: %8x\r\n", r->ocm1pe);
	SOC_LOGI("	arpe3: %8x\r\n", r->arpe3);
	SOC_LOGI("	arpe2: %8x\r\n", r->arpe2);
	SOC_LOGI("	arpe1: %8x\r\n", r->arpe1);
	SOC_LOGI("	oc3pe: %8x\r\n", r->oc3pe);
	SOC_LOGI("	oc2pe: %8x\r\n", r->oc2pe);
	SOC_LOGI("	oc1pe: %8x\r\n", r->oc1pe);
	SOC_LOGI("	reserved_9_17: %8x\r\n", r->reserved_9_17);
	SOC_LOGI("	cms_tim3: %8x\r\n", r->cms_tim3);
	SOC_LOGI("	cms_tim2: %8x\r\n", r->cms_tim2);
	SOC_LOGI("	cms_tim1: %8x\r\n", r->cms_tim1);
	SOC_LOGI("	reserved_24_25: %8x\r\n", r->reserved_24_25);
	SOC_LOGI("	urs3: %8x\r\n", r->urs3);
	SOC_LOGI("	urs2: %8x\r\n", r->urs2);
	SOC_LOGI("	urs1: %8x\r\n", r->urs1);
	SOC_LOGI("	dir_tim3: %8x\r\n", r->dir_tim3);
	SOC_LOGI("	dir_tim2: %8x\r\n", r->dir_tim2);
	SOC_LOGI("	dir_tim1: %8x\r\n", r->dir_tim1);
}

static void pwm_dump_reg81(void)
{
	pwm_reg81_t *r = (pwm_reg81_t *)(SOC_PWM_REG_BASE + (0x81 << 2));

	SOC_LOGI("reg81: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x81 << 2)));
	SOC_LOGI("	reserved_0_7: %8x\r\n", r->reserved_0_7);
	SOC_LOGI("	com_mode: %8x\r\n", r->com_mode);
	SOC_LOGI("	ccpc: %8x\r\n", r->ccpc);
	SOC_LOGI("	trios3: %8x\r\n", r->trios3);
	SOC_LOGI("	adcs3: %8x\r\n", r->adcs3);
	SOC_LOGI("	reserved_16_16: %8x\r\n", r->reserved_16_16);
	SOC_LOGI("	ccus: %8x\r\n", r->ccus);
	SOC_LOGI("	trios2: %8x\r\n", r->trios2);
	SOC_LOGI("	adcs2: %8x\r\n", r->adcs2);
	SOC_LOGI("	reserved_24_25: %8x\r\n", r->reserved_24_25);
	SOC_LOGI("	trios1: %8x\r\n", r->trios1);
	SOC_LOGI("	adcs1: %8x\r\n", r->adcs1);
}

static void pwm_dump_reg82(void)
{
	pwm_reg82_t *r = (pwm_reg82_t *)(SOC_PWM_REG_BASE + (0x82 << 2));

	SOC_LOGI("reg82: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x82 << 2)));
	SOC_LOGI("	sms1: %8x\r\n", r->sms1);
	SOC_LOGI("	sms2: %8x\r\n", r->sms2);
	SOC_LOGI("	sms3: %8x\r\n", r->sms3);
	SOC_LOGI("	ts1: %8x\r\n", r->ts1);
	SOC_LOGI("	reserved_15_15: %8x\r\n", r->reserved_15_15);
	SOC_LOGI("	ts2: %8x\r\n", r->ts2);
	SOC_LOGI("	ts3: %8x\r\n", r->ts3);
	SOC_LOGI("	reserved_22_31: %8x\r\n", r->reserved_22_31);
}

static void pwm_dump_reg83(void)
{
	pwm_reg83_t *r = (pwm_reg83_t *)(SOC_PWM_REG_BASE + (0x83 << 2));

	SOC_LOGI("reg83: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x83 << 2)));
	SOC_LOGI("	cc1ie: %8x\r\n", r->cc1ie);
	SOC_LOGI("	cc2ie: %8x\r\n", r->cc2ie);
	SOC_LOGI("	cc3ie: %8x\r\n", r->cc3ie);
	SOC_LOGI("	cc4ie: %8x\r\n", r->cc4ie);
	SOC_LOGI("	cc5ie: %8x\r\n", r->cc5ie);
	SOC_LOGI("	cc6ie: %8x\r\n", r->cc6ie);
	SOC_LOGI("	cc7ie: %8x\r\n", r->cc7ie);
	SOC_LOGI("	cc8ie: %8x\r\n", r->cc8ie);
	SOC_LOGI("	cc9ie: %8x\r\n", r->cc9ie);
	SOC_LOGI("	uie1: %8x\r\n", r->uie1);
	SOC_LOGI("	uie2: %8x\r\n", r->uie2);
	SOC_LOGI("	uie3: %8x\r\n", r->uie3);
	SOC_LOGI("	tie1: %8x\r\n", r->tie1);
	SOC_LOGI("	tie2: %8x\r\n", r->tie2);
	SOC_LOGI("	tie3: %8x\r\n", r->tie3);
	SOC_LOGI("	comie: %8x\r\n", r->comie);
	SOC_LOGI("	reserved_16_19: %8x\r\n", r->reserved_16_19);
	SOC_LOGI("	df_tim1: %8x\r\n", r->df_tim1);
	SOC_LOGI("	df_tim2: %8x\r\n", r->df_tim2);
	SOC_LOGI("	df_tim3: %8x\r\n", r->df_tim3);
}

static void pwm_dump_reg84(void)
{
	pwm_reg84_t *r = (pwm_reg84_t *)(SOC_PWM_REG_BASE + (0x84 << 2));

	SOC_LOGI("reg84: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x84 << 2)));
	SOC_LOGI("	cc1if: %8x\r\n", r->cc1if);
	SOC_LOGI("	cc2if: %8x\r\n", r->cc2if);
	SOC_LOGI("	cc3if: %8x\r\n", r->cc3if);
	SOC_LOGI("	cc4if: %8x\r\n", r->cc4if);
	SOC_LOGI("	cc5if: %8x\r\n", r->cc5if);
	SOC_LOGI("	cc6if: %8x\r\n", r->cc6if);
	SOC_LOGI("	cc7if: %8x\r\n", r->cc7if);
	SOC_LOGI("	cc8if: %8x\r\n", r->cc8if);
	SOC_LOGI("	cc9if: %8x\r\n", r->cc9if);
	SOC_LOGI("	uif1: %8x\r\n", r->uif1);
	SOC_LOGI("	uif2: %8x\r\n", r->uif2);
	SOC_LOGI("	uif3: %8x\r\n", r->uif3);
	SOC_LOGI("	tif1: %8x\r\n", r->tif1);
	SOC_LOGI("	tif2: %8x\r\n", r->tif2);
	SOC_LOGI("	tif3: %8x\r\n", r->tif3);
	SOC_LOGI("	comif: %8x\r\n", r->comif);
	SOC_LOGI("	reserved_22_31: %8x\r\n", r->reserved_22_31);
}

static void pwm_dump_reg85(void)
{
	pwm_reg85_t *r = (pwm_reg85_t *)(SOC_PWM_REG_BASE + (0x85 << 2));

	SOC_LOGI("reg85: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x85 << 2)));
	SOC_LOGI("	reserved_0_8: %8x\r\n", r->reserved_0_8);
	SOC_LOGI("	ug1: %8x\r\n", r->ug1);
	SOC_LOGI("	ug2: %8x\r\n", r->ug2);
	SOC_LOGI("	ug3: %8x\r\n", r->ug3);
	SOC_LOGI("	tg1: %8x\r\n", r->tg1);
	SOC_LOGI("	tg2: %8x\r\n", r->tg2);
	SOC_LOGI("	tg3: %8x\r\n", r->tg3);
	SOC_LOGI("	comg: %8x\r\n", r->comg);
	SOC_LOGI("	dtm1: %8x\r\n", r->dtm1);
	SOC_LOGI("	dtm2: %8x\r\n", r->dtm2);
	SOC_LOGI("	dtm3: %8x\r\n", r->dtm3);
	SOC_LOGI("	init_level1: %8x\r\n", r->init_level1);
	SOC_LOGI("	init_level2: %8x\r\n", r->init_level2);
	SOC_LOGI("	init_level3: %8x\r\n", r->init_level3);
	SOC_LOGI("	reserved_25_31: %8x\r\n", r->reserved_25_31);
}

static void pwm_dump_reg86(void)
{
	pwm_reg86_t *r = (pwm_reg86_t *)(SOC_PWM_REG_BASE + (0x86 << 2));

	SOC_LOGI("reg86: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x86 << 2)));
	SOC_LOGI("	ch1p: %8x\r\n", r->ch1p);
	SOC_LOGI("	ch2p: %8x\r\n", r->ch2p);
	SOC_LOGI("	ch3p: %8x\r\n", r->ch3p);
	SOC_LOGI("	ch4p: %8x\r\n", r->ch4p);
	SOC_LOGI("	ch5p: %8x\r\n", r->ch5p);
	SOC_LOGI("	ch6p: %8x\r\n", r->ch6p);
	SOC_LOGI("	ch1e: %8x\r\n", r->ch1e);
	SOC_LOGI("	ch2e: %8x\r\n", r->ch2e);
	SOC_LOGI("	ch3e: %8x\r\n", r->ch3e);
	SOC_LOGI("	ch4e: %8x\r\n", r->ch4e);
	SOC_LOGI("	ch5e: %8x\r\n", r->ch5e);
	SOC_LOGI("	ch6e: %8x\r\n", r->ch6e);
	SOC_LOGI("	tim1ccm: %8x\r\n", r->tim1ccm);
	SOC_LOGI("	tim2ccm: %8x\r\n", r->tim2ccm);
	SOC_LOGI("	tim3ccm: %8x\r\n", r->tim3ccm);
	SOC_LOGI("	oc1m: %8x\r\n", r->oc1m);
	SOC_LOGI("	oc2m: %8x\r\n", r->oc2m);
	SOC_LOGI("	oc3m: %8x\r\n", r->oc3m);
	SOC_LOGI("	reserved_30_31: %8x\r\n", r->reserved_30_31);
}

static void pwm_dump_reg87(void)
{
	SOC_LOGI("reg87: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x87 << 2)));
}

static void pwm_dump_reg88(void)
{
	SOC_LOGI("reg88: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x88 << 2)));
}

static void pwm_dump_reg89(void)
{
	SOC_LOGI("reg89: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x89 << 2)));
}

static void pwm_dump_reg8a(void)
{
	pwm_reg8a_t *r = (pwm_reg8a_t *)(SOC_PWM_REG_BASE + (0x8a << 2));

	SOC_LOGI("reg8a: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x8a << 2)));
	SOC_LOGI("	psc1: %8x\r\n", r->psc1);
	SOC_LOGI("	psc2: %8x\r\n", r->psc2);
	SOC_LOGI("	psc3: %8x\r\n", r->psc3);
	SOC_LOGI("	reserved_24_31: %8x\r\n", r->reserved_24_31);
}

static void pwm_dump_reg8b(void)
{
	SOC_LOGI("reg8b: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x8b << 2)));
}

static void pwm_dump_reg8c(void)
{
	SOC_LOGI("reg8c: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x8c << 2)));
}

static void pwm_dump_reg8d(void)
{
	SOC_LOGI("reg8d: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x8d << 2)));
}

static void pwm_dump_reg8e(void)
{
	SOC_LOGI("reg8e: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x8e << 2)));
}

static void pwm_dump_reg8f(void)
{
	SOC_LOGI("reg8f: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x8f << 2)));
}

static void pwm_dump_reg90(void)
{
	SOC_LOGI("reg90: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x90 << 2)));
}

static void pwm_dump_reg91(void)
{
	SOC_LOGI("reg91: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x91 << 2)));
}

static void pwm_dump_reg92(void)
{
	SOC_LOGI("reg92: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x92 << 2)));
}

static void pwm_dump_reg93(void)
{
	SOC_LOGI("reg93: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x93 << 2)));
}

static void pwm_dump_reg94(void)
{
	SOC_LOGI("reg94: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x94 << 2)));
}

static void pwm_dump_reg95(void)
{
	SOC_LOGI("reg95: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x95 << 2)));
}

static void pwm_dump_reg96(void)
{
	SOC_LOGI("reg96: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x96 << 2)));
}

static void pwm_dump_reg97(void)
{
	SOC_LOGI("reg97: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x97 << 2)));
}

static void pwm_dump_reg98(void)
{
	SOC_LOGI("reg98: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x98 << 2)));
}

static void pwm_dump_reg99(void)
{
	SOC_LOGI("reg99: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x99 << 2)));
}

static void pwm_dump_reg9a(void)
{
	pwm_reg9a_t *r = (pwm_reg9a_t *)(SOC_PWM_REG_BASE + (0x9a << 2));

	SOC_LOGI("reg9a: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x9a << 2)));
	SOC_LOGI("	dt1: %8x\r\n", r->dt1);
	SOC_LOGI("	dt2: %8x\r\n", r->dt2);
	SOC_LOGI("	dt3: %8x\r\n", r->dt3);
	SOC_LOGI("	reserved_30_31: %8x\r\n", r->reserved_30_31);
}

static void pwm_dump_reg9b(void)
{
	SOC_LOGI("reg9b: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x9b << 2)));
}

static void pwm_dump_reg9c(void)
{
	SOC_LOGI("reg9c: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x9c << 2)));
}

static void pwm_dump_reg9d(void)
{
	SOC_LOGI("reg9d: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x9d << 2)));
}

static void pwm_dump_reg9e(void)
{
	SOC_LOGI("reg9e: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x9e << 2)));
}

static void pwm_dump_reg9f(void)
{
	SOC_LOGI("reg9f: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x9f << 2)));
}

static void pwm_dump_rega0(void)
{
	SOC_LOGI("rega0: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xa0 << 2)));
}

static void pwm_dump_rega1(void)
{
	SOC_LOGI("rega1: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xa1 << 2)));
}

static void pwm_dump_rega2(void)
{
	SOC_LOGI("rega2: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xa2 << 2)));
}

static void pwm_dump_rega3(void)
{
	SOC_LOGI("rega3: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xa3 << 2)));
}

static void pwm_dump_rega4(void)
{
	SOC_LOGI("rega4: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xa4 << 2)));
}

static void pwm_dump_rega5(void)
{
	SOC_LOGI("rega5: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xa5 << 2)));
}

static void pwm_dump_rega6(void)
{
	SOC_LOGI("rega6: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xa6 << 2)));
}

static void pwm_dump_rega7(void)
{
	SOC_LOGI("rega7: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xa7 << 2)));
}

static void pwm_dump_rega8(void)
{
	SOC_LOGI("rega8: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xa8 << 2)));
}

static void pwm_dump_rega9(void)
{
	SOC_LOGI("rega9: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xa9 << 2)));
}

static void pwm_dump_regaa(void)
{
	pwm_regaa_t *r = (pwm_regaa_t *)(SOC_PWM_REG_BASE + (0xaa << 2));

	SOC_LOGI("regaa: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xaa << 2)));
	SOC_LOGI("	ch6e_shad: %8x\r\n", r->ch6e_shad);
	SOC_LOGI("	ch5e_shad: %8x\r\n", r->ch5e_shad);
	SOC_LOGI("	ch4e_shad: %8x\r\n", r->ch4e_shad);
	SOC_LOGI("	ch3e_shad: %8x\r\n", r->ch3e_shad);
	SOC_LOGI("	ch2e_shad: %8x\r\n", r->ch2e_shad);
	SOC_LOGI("	ch1e_shad: %8x\r\n", r->ch1e_shad);
	SOC_LOGI("	oc1m_shad: %8x\r\n", r->oc1m_shad);
	SOC_LOGI("	oc2m_shad: %8x\r\n", r->oc2m_shad);
	SOC_LOGI("	oc3m_shad: %8x\r\n", r->oc3m_shad);
	SOC_LOGI("	reserved_15_31: %8x\r\n", r->reserved_15_31);
}

static void pwm_dump_rsv_ab_bf(void)
{
	for (uint32_t idx = 0; idx < 21; idx++) {
		SOC_LOGI("rsv_ab_bf: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + ((0xab + idx) << 2)));
	}
}

static void pwm_dump_regc0(void)
{
	pwm_regc0_t *r = (pwm_regc0_t *)(SOC_PWM_REG_BASE + (0xc0 << 2));

	SOC_LOGI("regc0: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xc0 << 2)));
	SOC_LOGI("	ocm3pe: %8x\r\n", r->ocm3pe);
	SOC_LOGI("	ocm2pe: %8x\r\n", r->ocm2pe);
	SOC_LOGI("	ocm1pe: %8x\r\n", r->ocm1pe);
	SOC_LOGI("	arpe3: %8x\r\n", r->arpe3);
	SOC_LOGI("	arpe2: %8x\r\n", r->arpe2);
	SOC_LOGI("	arpe1: %8x\r\n", r->arpe1);
	SOC_LOGI("	oc3pe: %8x\r\n", r->oc3pe);
	SOC_LOGI("	oc2pe: %8x\r\n", r->oc2pe);
	SOC_LOGI("	oc1pe: %8x\r\n", r->oc1pe);
	SOC_LOGI("	reserved_9_17: %8x\r\n", r->reserved_9_17);
	SOC_LOGI("	cms_tim3: %8x\r\n", r->cms_tim3);
	SOC_LOGI("	cms_tim2: %8x\r\n", r->cms_tim2);
	SOC_LOGI("	cms_tim1: %8x\r\n", r->cms_tim1);
	SOC_LOGI("	reserved_24_25: %8x\r\n", r->reserved_24_25);
	SOC_LOGI("	urs3: %8x\r\n", r->urs3);
	SOC_LOGI("	urs2: %8x\r\n", r->urs2);
	SOC_LOGI("	urs1: %8x\r\n", r->urs1);
	SOC_LOGI("	dir_tim3: %8x\r\n", r->dir_tim3);
	SOC_LOGI("	dir_tim2: %8x\r\n", r->dir_tim2);
	SOC_LOGI("	dir_tim1: %8x\r\n", r->dir_tim1);
}

static void pwm_dump_regc1(void)
{
	pwm_regc1_t *r = (pwm_regc1_t *)(SOC_PWM_REG_BASE + (0xc1 << 2));

	SOC_LOGI("regc1: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xc1 << 2)));
	SOC_LOGI("	reserved_0_7: %8x\r\n", r->reserved_0_7);
	SOC_LOGI("	com_mode: %8x\r\n", r->com_mode);
	SOC_LOGI("	ccpc: %8x\r\n", r->ccpc);
	SOC_LOGI("	trios3: %8x\r\n", r->trios3);
	SOC_LOGI("	adcs3: %8x\r\n", r->adcs3);
	SOC_LOGI("	reserved_16_16: %8x\r\n", r->reserved_16_16);
	SOC_LOGI("	ccus: %8x\r\n", r->ccus);
	SOC_LOGI("	trios2: %8x\r\n", r->trios2);
	SOC_LOGI("	adcs2: %8x\r\n", r->adcs2);
	SOC_LOGI("	reserved_24_25: %8x\r\n", r->reserved_24_25);
	SOC_LOGI("	trios1: %8x\r\n", r->trios1);
	SOC_LOGI("	adcs1: %8x\r\n", r->adcs1);
}

static void pwm_dump_regc2(void)
{
	pwm_regc2_t *r = (pwm_regc2_t *)(SOC_PWM_REG_BASE + (0xc2 << 2));

	SOC_LOGI("regc2: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xc2 << 2)));
	SOC_LOGI("	sms1: %8x\r\n", r->sms1);
	SOC_LOGI("	sms2: %8x\r\n", r->sms2);
	SOC_LOGI("	sms3: %8x\r\n", r->sms3);
	SOC_LOGI("	ts1: %8x\r\n", r->ts1);
	SOC_LOGI("	reserved_15_15: %8x\r\n", r->reserved_15_15);
	SOC_LOGI("	ts2: %8x\r\n", r->ts2);
	SOC_LOGI("	ts3: %8x\r\n", r->ts3);
	SOC_LOGI("	reserved_22_31: %8x\r\n", r->reserved_22_31);
}

static void pwm_dump_regc3(void)
{
	pwm_regc3_t *r = (pwm_regc3_t *)(SOC_PWM_REG_BASE + (0xc3 << 2));

	SOC_LOGI("regc3: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xc3 << 2)));
	SOC_LOGI("	cc1ie: %8x\r\n", r->cc1ie);
	SOC_LOGI("	cc2ie: %8x\r\n", r->cc2ie);
	SOC_LOGI("	cc3ie: %8x\r\n", r->cc3ie);
	SOC_LOGI("	cc4ie: %8x\r\n", r->cc4ie);
	SOC_LOGI("	cc5ie: %8x\r\n", r->cc5ie);
	SOC_LOGI("	cc6ie: %8x\r\n", r->cc6ie);
	SOC_LOGI("	cc7ie: %8x\r\n", r->cc7ie);
	SOC_LOGI("	cc8ie: %8x\r\n", r->cc8ie);
	SOC_LOGI("	cc9ie: %8x\r\n", r->cc9ie);
	SOC_LOGI("	uie1: %8x\r\n", r->uie1);
	SOC_LOGI("	uie2: %8x\r\n", r->uie2);
	SOC_LOGI("	uie3: %8x\r\n", r->uie3);
	SOC_LOGI("	tie1: %8x\r\n", r->tie1);
	SOC_LOGI("	tie2: %8x\r\n", r->tie2);
	SOC_LOGI("	tie3: %8x\r\n", r->tie3);
	SOC_LOGI("	comie: %8x\r\n", r->comie);
	SOC_LOGI("	reserved_16_19: %8x\r\n", r->reserved_16_19);
	SOC_LOGI("	df_tim1: %8x\r\n", r->df_tim1);
	SOC_LOGI("	df_tim2: %8x\r\n", r->df_tim2);
	SOC_LOGI("	df_tim3: %8x\r\n", r->df_tim3);
}

static void pwm_dump_regc4(void)
{
	pwm_regc4_t *r = (pwm_regc4_t *)(SOC_PWM_REG_BASE + (0xc4 << 2));

	SOC_LOGI("regc4: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xc4 << 2)));
	SOC_LOGI("	cc1if: %8x\r\n", r->cc1if);
	SOC_LOGI("	cc2if: %8x\r\n", r->cc2if);
	SOC_LOGI("	cc3if: %8x\r\n", r->cc3if);
	SOC_LOGI("	cc4if: %8x\r\n", r->cc4if);
	SOC_LOGI("	cc5if: %8x\r\n", r->cc5if);
	SOC_LOGI("	cc6if: %8x\r\n", r->cc6if);
	SOC_LOGI("	cc7if: %8x\r\n", r->cc7if);
	SOC_LOGI("	cc8if: %8x\r\n", r->cc8if);
	SOC_LOGI("	cc9if: %8x\r\n", r->cc9if);
	SOC_LOGI("	uif1: %8x\r\n", r->uif1);
	SOC_LOGI("	uif2: %8x\r\n", r->uif2);
	SOC_LOGI("	uif3: %8x\r\n", r->uif3);
	SOC_LOGI("	tif1: %8x\r\n", r->tif1);
	SOC_LOGI("	tif2: %8x\r\n", r->tif2);
	SOC_LOGI("	tif3: %8x\r\n", r->tif3);
	SOC_LOGI("	comif: %8x\r\n", r->comif);
	SOC_LOGI("	reserved_22_31: %8x\r\n", r->reserved_22_31);
}

static void pwm_dump_regc5(void)
{
	pwm_regc5_t *r = (pwm_regc5_t *)(SOC_PWM_REG_BASE + (0xc5 << 2));

	SOC_LOGI("regc5: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xc5 << 2)));
	SOC_LOGI("	reserved_0_8: %8x\r\n", r->reserved_0_8);
	SOC_LOGI("	ug1: %8x\r\n", r->ug1);
	SOC_LOGI("	ug2: %8x\r\n", r->ug2);
	SOC_LOGI("	ug3: %8x\r\n", r->ug3);
	SOC_LOGI("	tg1: %8x\r\n", r->tg1);
	SOC_LOGI("	tg2: %8x\r\n", r->tg2);
	SOC_LOGI("	tg3: %8x\r\n", r->tg3);
	SOC_LOGI("	comg: %8x\r\n", r->comg);
	SOC_LOGI("	dtm1: %8x\r\n", r->dtm1);
	SOC_LOGI("	dtm2: %8x\r\n", r->dtm2);
	SOC_LOGI("	dtm3: %8x\r\n", r->dtm3);
	SOC_LOGI("	init_level1: %8x\r\n", r->init_level1);
	SOC_LOGI("	init_level2: %8x\r\n", r->init_level2);
	SOC_LOGI("	init_level3: %8x\r\n", r->init_level3);
	SOC_LOGI("	reserved_25_31: %8x\r\n", r->reserved_25_31);
}

static void pwm_dump_regc6(void)
{
	pwm_regc6_t *r = (pwm_regc6_t *)(SOC_PWM_REG_BASE + (0xc6 << 2));

	SOC_LOGI("regc6: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xc6 << 2)));
	SOC_LOGI("	ch1p: %8x\r\n", r->ch1p);
	SOC_LOGI("	ch2p: %8x\r\n", r->ch2p);
	SOC_LOGI("	ch3p: %8x\r\n", r->ch3p);
	SOC_LOGI("	ch4p: %8x\r\n", r->ch4p);
	SOC_LOGI("	ch5p: %8x\r\n", r->ch5p);
	SOC_LOGI("	ch6p: %8x\r\n", r->ch6p);
	SOC_LOGI("	ch1e: %8x\r\n", r->ch1e);
	SOC_LOGI("	ch2e: %8x\r\n", r->ch2e);
	SOC_LOGI("	ch3e: %8x\r\n", r->ch3e);
	SOC_LOGI("	ch4e: %8x\r\n", r->ch4e);
	SOC_LOGI("	ch5e: %8x\r\n", r->ch5e);
	SOC_LOGI("	ch6e: %8x\r\n", r->ch6e);
	SOC_LOGI("	tim1ccm: %8x\r\n", r->tim1ccm);
	SOC_LOGI("	tim2ccm: %8x\r\n", r->tim2ccm);
	SOC_LOGI("	tim3ccm: %8x\r\n", r->tim3ccm);
	SOC_LOGI("	oc1m: %8x\r\n", r->oc1m);
	SOC_LOGI("	oc2m: %8x\r\n", r->oc2m);
	SOC_LOGI("	oc3m: %8x\r\n", r->oc3m);
	SOC_LOGI("	reserved_30_31: %8x\r\n", r->reserved_30_31);
}

static void pwm_dump_regc7(void)
{
	SOC_LOGI("regc7: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xc7 << 2)));
}

static void pwm_dump_regc8(void)
{
	SOC_LOGI("regc8: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xc8 << 2)));
}

static void pwm_dump_regc9(void)
{
	SOC_LOGI("regc9: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xc9 << 2)));
}

static void pwm_dump_regca(void)
{
	pwm_regca_t *r = (pwm_regca_t *)(SOC_PWM_REG_BASE + (0xca << 2));

	SOC_LOGI("regca: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xca << 2)));
	SOC_LOGI("	psc1: %8x\r\n", r->psc1);
	SOC_LOGI("	psc2: %8x\r\n", r->psc2);
	SOC_LOGI("	psc3: %8x\r\n", r->psc3);
	SOC_LOGI("	reserved_24_31: %8x\r\n", r->reserved_24_31);
}

static void pwm_dump_regcb(void)
{
	SOC_LOGI("regcb: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xcb << 2)));
}

static void pwm_dump_regcc(void)
{
	SOC_LOGI("regcc: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xcc << 2)));
}

static void pwm_dump_regcd(void)
{
	SOC_LOGI("regcd: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xcd << 2)));
}

static void pwm_dump_regce(void)
{
	SOC_LOGI("regce: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xce << 2)));
}

static void pwm_dump_regcf(void)
{
	SOC_LOGI("regcf: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xcf << 2)));
}

static void pwm_dump_regd0(void)
{
	SOC_LOGI("regd0: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xd0 << 2)));
}

static void pwm_dump_regd1(void)
{
	SOC_LOGI("regd1: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xd1 << 2)));
}

static void pwm_dump_regd2(void)
{
	SOC_LOGI("regd2: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xd2 << 2)));
}

static void pwm_dump_regd3(void)
{
	SOC_LOGI("regd3: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xd3 << 2)));
}

static void pwm_dump_regd4(void)
{
	SOC_LOGI("regd4: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xd4 << 2)));
}

static void pwm_dump_regd5(void)
{
	SOC_LOGI("regd5: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xd5 << 2)));
}

static void pwm_dump_regd6(void)
{
	SOC_LOGI("regd6: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xd6 << 2)));
}

static void pwm_dump_regd7(void)
{
	SOC_LOGI("regd7: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xd7 << 2)));
}

static void pwm_dump_regd8(void)
{
	SOC_LOGI("regd8: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xd8 << 2)));
}

static void pwm_dump_regd9(void)
{
	SOC_LOGI("regd9: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xd9 << 2)));
}

static void pwm_dump_regda(void)
{
	pwm_regda_t *r = (pwm_regda_t *)(SOC_PWM_REG_BASE + (0xda << 2));

	SOC_LOGI("regda: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xda << 2)));
	SOC_LOGI("	dt1: %8x\r\n", r->dt1);
	SOC_LOGI("	dt2: %8x\r\n", r->dt2);
	SOC_LOGI("	dt3: %8x\r\n", r->dt3);
	SOC_LOGI("	reserved_30_31: %8x\r\n", r->reserved_30_31);
}

static void pwm_dump_regdb(void)
{
	SOC_LOGI("regdb: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xdb << 2)));
}

static void pwm_dump_regdc(void)
{
	SOC_LOGI("regdc: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xdc << 2)));
}

static void pwm_dump_regdd(void)
{
	SOC_LOGI("regdd: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xdd << 2)));
}

static void pwm_dump_regde(void)
{
	SOC_LOGI("regde: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xde << 2)));
}

static void pwm_dump_regdf(void)
{
	SOC_LOGI("regdf: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xdf << 2)));
}

static void pwm_dump_rege0(void)
{
	SOC_LOGI("rege0: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xe0 << 2)));
}

static void pwm_dump_rege1(void)
{
	SOC_LOGI("rege1: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xe1 << 2)));
}

static void pwm_dump_rege2(void)
{
	SOC_LOGI("rege2: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xe2 << 2)));
}

static void pwm_dump_rege3(void)
{
	SOC_LOGI("rege3: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xe3 << 2)));
}

static void pwm_dump_rege4(void)
{
	SOC_LOGI("rege4: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xe4 << 2)));
}

static void pwm_dump_rege5(void)
{
	SOC_LOGI("rege5: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xe5 << 2)));
}

static void pwm_dump_rege6(void)
{
	SOC_LOGI("rege6: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xe6 << 2)));
}

static void pwm_dump_rege7(void)
{
	SOC_LOGI("rege7: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xe7 << 2)));
}

static void pwm_dump_rege8(void)
{
	SOC_LOGI("rege8: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xe8 << 2)));
}

static void pwm_dump_rege9(void)
{
	SOC_LOGI("rege9: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xe9 << 2)));
}

static void pwm_dump_regea(void)
{
	pwm_regea_t *r = (pwm_regea_t *)(SOC_PWM_REG_BASE + (0xea << 2));

	SOC_LOGI("regea: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0xea << 2)));
	SOC_LOGI("	ch6e_shad: %8x\r\n", r->ch6e_shad);
	SOC_LOGI("	ch5e_shad: %8x\r\n", r->ch5e_shad);
	SOC_LOGI("	ch4e_shad: %8x\r\n", r->ch4e_shad);
	SOC_LOGI("	ch3e_shad: %8x\r\n", r->ch3e_shad);
	SOC_LOGI("	ch2e_shad: %8x\r\n", r->ch2e_shad);
	SOC_LOGI("	ch1e_shad: %8x\r\n", r->ch1e_shad);
	SOC_LOGI("	oc1m_shad: %8x\r\n", r->oc1m_shad);
	SOC_LOGI("	oc2m_shad: %8x\r\n", r->oc2m_shad);
	SOC_LOGI("	oc3m_shad: %8x\r\n", r->oc3m_shad);
	SOC_LOGI("	reserved_15_31: %8x\r\n", r->reserved_15_31);
}

static void pwm_dump_rsv_eb_ff(void)
{
	for (uint32_t idx = 0; idx < 21; idx++) {
		SOC_LOGI("rsv_eb_ff: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + ((0xeb + idx) << 2)));
	}
}

static void pwm_dump_reg100(void)
{
	pwm_reg100_t *r = (pwm_reg100_t *)(SOC_PWM_REG_BASE + (0x100 << 2));

	SOC_LOGI("reg100: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x100 << 2)));
	SOC_LOGI("	ocm3pe: %8x\r\n", r->ocm3pe);
	SOC_LOGI("	ocm2pe: %8x\r\n", r->ocm2pe);
	SOC_LOGI("	ocm1pe: %8x\r\n", r->ocm1pe);
	SOC_LOGI("	arpe3: %8x\r\n", r->arpe3);
	SOC_LOGI("	arpe2: %8x\r\n", r->arpe2);
	SOC_LOGI("	arpe1: %8x\r\n", r->arpe1);
	SOC_LOGI("	oc3pe: %8x\r\n", r->oc3pe);
	SOC_LOGI("	oc2pe: %8x\r\n", r->oc2pe);
	SOC_LOGI("	oc1pe: %8x\r\n", r->oc1pe);
	SOC_LOGI("	reserved_9_17: %8x\r\n", r->reserved_9_17);
	SOC_LOGI("	cms_tim3: %8x\r\n", r->cms_tim3);
	SOC_LOGI("	cms_tim2: %8x\r\n", r->cms_tim2);
	SOC_LOGI("	cms_tim1: %8x\r\n", r->cms_tim1);
	SOC_LOGI("	reserved_24_25: %8x\r\n", r->reserved_24_25);
	SOC_LOGI("	urs3: %8x\r\n", r->urs3);
	SOC_LOGI("	urs2: %8x\r\n", r->urs2);
	SOC_LOGI("	urs1: %8x\r\n", r->urs1);
	SOC_LOGI("	dir_tim3: %8x\r\n", r->dir_tim3);
	SOC_LOGI("	dir_tim2: %8x\r\n", r->dir_tim2);
	SOC_LOGI("	dir_tim1: %8x\r\n", r->dir_tim1);
}

static void pwm_dump_reg101(void)
{
	pwm_reg101_t *r = (pwm_reg101_t *)(SOC_PWM_REG_BASE + (0x101 << 2));

	SOC_LOGI("reg101: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x101 << 2)));
	SOC_LOGI("	reserved_0_7: %8x\r\n", r->reserved_0_7);
	SOC_LOGI("	com_mode: %8x\r\n", r->com_mode);
	SOC_LOGI("	ccpc: %8x\r\n", r->ccpc);
	SOC_LOGI("	trios3: %8x\r\n", r->trios3);
	SOC_LOGI("	adcs3: %8x\r\n", r->adcs3);
	SOC_LOGI("	reserved_16_16: %8x\r\n", r->reserved_16_16);
	SOC_LOGI("	ccus: %8x\r\n", r->ccus);
	SOC_LOGI("	trios2: %8x\r\n", r->trios2);
	SOC_LOGI("	adcs2: %8x\r\n", r->adcs2);
	SOC_LOGI("	reserved_24_25: %8x\r\n", r->reserved_24_25);
	SOC_LOGI("	trios1: %8x\r\n", r->trios1);
	SOC_LOGI("	adcs1: %8x\r\n", r->adcs1);
}

static void pwm_dump_reg102(void)
{
	pwm_reg102_t *r = (pwm_reg102_t *)(SOC_PWM_REG_BASE + (0x102 << 2));

	SOC_LOGI("reg102: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x102 << 2)));
	SOC_LOGI("	sms1: %8x\r\n", r->sms1);
	SOC_LOGI("	sms2: %8x\r\n", r->sms2);
	SOC_LOGI("	sms3: %8x\r\n", r->sms3);
	SOC_LOGI("	ts1: %8x\r\n", r->ts1);
	SOC_LOGI("	reserved_15_15: %8x\r\n", r->reserved_15_15);
	SOC_LOGI("	ts2: %8x\r\n", r->ts2);
	SOC_LOGI("	ts3: %8x\r\n", r->ts3);
	SOC_LOGI("	reserved_22_31: %8x\r\n", r->reserved_22_31);
}

static void pwm_dump_reg103(void)
{
	pwm_reg103_t *r = (pwm_reg103_t *)(SOC_PWM_REG_BASE + (0x103 << 2));

	SOC_LOGI("reg103: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x103 << 2)));
	SOC_LOGI("	cc1ie: %8x\r\n", r->cc1ie);
	SOC_LOGI("	cc2ie: %8x\r\n", r->cc2ie);
	SOC_LOGI("	cc3ie: %8x\r\n", r->cc3ie);
	SOC_LOGI("	cc4ie: %8x\r\n", r->cc4ie);
	SOC_LOGI("	cc5ie: %8x\r\n", r->cc5ie);
	SOC_LOGI("	cc6ie: %8x\r\n", r->cc6ie);
	SOC_LOGI("	cc7ie: %8x\r\n", r->cc7ie);
	SOC_LOGI("	cc8ie: %8x\r\n", r->cc8ie);
	SOC_LOGI("	cc9ie: %8x\r\n", r->cc9ie);
	SOC_LOGI("	uie1: %8x\r\n", r->uie1);
	SOC_LOGI("	uie2: %8x\r\n", r->uie2);
	SOC_LOGI("	uie3: %8x\r\n", r->uie3);
	SOC_LOGI("	tie1: %8x\r\n", r->tie1);
	SOC_LOGI("	tie2: %8x\r\n", r->tie2);
	SOC_LOGI("	tie3: %8x\r\n", r->tie3);
	SOC_LOGI("	comie: %8x\r\n", r->comie);
	SOC_LOGI("	reserved_16_19: %8x\r\n", r->reserved_16_19);
	SOC_LOGI("	df_tim1: %8x\r\n", r->df_tim1);
	SOC_LOGI("	df_tim2: %8x\r\n", r->df_tim2);
	SOC_LOGI("	df_tim3: %8x\r\n", r->df_tim3);
}

static void pwm_dump_reg104(void)
{
	pwm_reg104_t *r = (pwm_reg104_t *)(SOC_PWM_REG_BASE + (0x104 << 2));

	SOC_LOGI("reg104: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x104 << 2)));
	SOC_LOGI("	cc1if: %8x\r\n", r->cc1if);
	SOC_LOGI("	cc2if: %8x\r\n", r->cc2if);
	SOC_LOGI("	cc3if: %8x\r\n", r->cc3if);
	SOC_LOGI("	cc4if: %8x\r\n", r->cc4if);
	SOC_LOGI("	cc5if: %8x\r\n", r->cc5if);
	SOC_LOGI("	cc6if: %8x\r\n", r->cc6if);
	SOC_LOGI("	cc7if: %8x\r\n", r->cc7if);
	SOC_LOGI("	cc8if: %8x\r\n", r->cc8if);
	SOC_LOGI("	cc9if: %8x\r\n", r->cc9if);
	SOC_LOGI("	uif1: %8x\r\n", r->uif1);
	SOC_LOGI("	uif2: %8x\r\n", r->uif2);
	SOC_LOGI("	uif3: %8x\r\n", r->uif3);
	SOC_LOGI("	tif1: %8x\r\n", r->tif1);
	SOC_LOGI("	tif2: %8x\r\n", r->tif2);
	SOC_LOGI("	tif3: %8x\r\n", r->tif3);
	SOC_LOGI("	comif: %8x\r\n", r->comif);
	SOC_LOGI("	reserved_22_31: %8x\r\n", r->reserved_22_31);
}

static void pwm_dump_reg105(void)
{
	pwm_reg105_t *r = (pwm_reg105_t *)(SOC_PWM_REG_BASE + (0x105 << 2));

	SOC_LOGI("reg105: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x105 << 2)));
	SOC_LOGI("	reserved_0_8: %8x\r\n", r->reserved_0_8);
	SOC_LOGI("	ug1: %8x\r\n", r->ug1);
	SOC_LOGI("	ug2: %8x\r\n", r->ug2);
	SOC_LOGI("	ug3: %8x\r\n", r->ug3);
	SOC_LOGI("	tg1: %8x\r\n", r->tg1);
	SOC_LOGI("	tg2: %8x\r\n", r->tg2);
	SOC_LOGI("	tg3: %8x\r\n", r->tg3);
	SOC_LOGI("	comg: %8x\r\n", r->comg);
	SOC_LOGI("	dtm1: %8x\r\n", r->dtm1);
	SOC_LOGI("	dtm2: %8x\r\n", r->dtm2);
	SOC_LOGI("	dtm3: %8x\r\n", r->dtm3);
	SOC_LOGI("	init_level1: %8x\r\n", r->init_level1);
	SOC_LOGI("	init_level2: %8x\r\n", r->init_level2);
	SOC_LOGI("	init_level3: %8x\r\n", r->init_level3);
	SOC_LOGI("	reserved_25_31: %8x\r\n", r->reserved_25_31);
}

static void pwm_dump_reg106(void)
{
	pwm_reg106_t *r = (pwm_reg106_t *)(SOC_PWM_REG_BASE + (0x106 << 2));

	SOC_LOGI("reg106: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x106 << 2)));
	SOC_LOGI("	ch1p: %8x\r\n", r->ch1p);
	SOC_LOGI("	ch2p: %8x\r\n", r->ch2p);
	SOC_LOGI("	ch3p: %8x\r\n", r->ch3p);
	SOC_LOGI("	ch4p: %8x\r\n", r->ch4p);
	SOC_LOGI("	ch5p: %8x\r\n", r->ch5p);
	SOC_LOGI("	ch6p: %8x\r\n", r->ch6p);
	SOC_LOGI("	ch1e: %8x\r\n", r->ch1e);
	SOC_LOGI("	ch2e: %8x\r\n", r->ch2e);
	SOC_LOGI("	ch3e: %8x\r\n", r->ch3e);
	SOC_LOGI("	ch4e: %8x\r\n", r->ch4e);
	SOC_LOGI("	ch5e: %8x\r\n", r->ch5e);
	SOC_LOGI("	ch6e: %8x\r\n", r->ch6e);
	SOC_LOGI("	tim1ccm: %8x\r\n", r->tim1ccm);
	SOC_LOGI("	tim2ccm: %8x\r\n", r->tim2ccm);
	SOC_LOGI("	tim3ccm: %8x\r\n", r->tim3ccm);
	SOC_LOGI("	oc1m: %8x\r\n", r->oc1m);
	SOC_LOGI("	oc2m: %8x\r\n", r->oc2m);
	SOC_LOGI("	oc3m: %8x\r\n", r->oc3m);
	SOC_LOGI("	reserved_30_31: %8x\r\n", r->reserved_30_31);
}

static void pwm_dump_reg107(void)
{
	SOC_LOGI("reg107: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x107 << 2)));
}

static void pwm_dump_reg108(void)
{
	SOC_LOGI("reg108: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x108 << 2)));
}

static void pwm_dump_reg109(void)
{
	SOC_LOGI("reg109: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x109 << 2)));
}

static void pwm_dump_reg10a(void)
{
	pwm_reg10a_t *r = (pwm_reg10a_t *)(SOC_PWM_REG_BASE + (0x10a << 2));

	SOC_LOGI("reg10a: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x10a << 2)));
	SOC_LOGI("	psc1: %8x\r\n", r->psc1);
	SOC_LOGI("	psc2: %8x\r\n", r->psc2);
	SOC_LOGI("	psc3: %8x\r\n", r->psc3);
	SOC_LOGI("	reserved_24_31: %8x\r\n", r->reserved_24_31);
}

static void pwm_dump_reg10b(void)
{
	SOC_LOGI("reg10b: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x10b << 2)));
}

static void pwm_dump_reg10c(void)
{
	SOC_LOGI("reg10c: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x10c << 2)));
}

static void pwm_dump_reg10d(void)
{
	SOC_LOGI("reg10d: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x10d << 2)));
}

static void pwm_dump_reg10e(void)
{
	SOC_LOGI("reg10e: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x10e << 2)));
}

static void pwm_dump_reg10f(void)
{
	SOC_LOGI("reg10f: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x10f << 2)));
}

static void pwm_dump_reg110(void)
{
	SOC_LOGI("reg110: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x110 << 2)));
}

static void pwm_dump_reg111(void)
{
	SOC_LOGI("reg111: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x111 << 2)));
}

static void pwm_dump_reg112(void)
{
	SOC_LOGI("reg112: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x112 << 2)));
}

static void pwm_dump_reg113(void)
{
	SOC_LOGI("reg113: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x113 << 2)));
}

static void pwm_dump_reg114(void)
{
	SOC_LOGI("reg114: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x114 << 2)));
}

static void pwm_dump_reg115(void)
{
	SOC_LOGI("reg115: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x115 << 2)));
}

static void pwm_dump_reg116(void)
{
	SOC_LOGI("reg116: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x116 << 2)));
}

static void pwm_dump_reg117(void)
{
	SOC_LOGI("reg117: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x117 << 2)));
}

static void pwm_dump_reg118(void)
{
	SOC_LOGI("reg118: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x118 << 2)));
}

static void pwm_dump_reg119(void)
{
	SOC_LOGI("reg119: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x119 << 2)));
}

static void pwm_dump_reg11a(void)
{
	pwm_reg11a_t *r = (pwm_reg11a_t *)(SOC_PWM_REG_BASE + (0x11a << 2));

	SOC_LOGI("reg11a: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x11a << 2)));
	SOC_LOGI("	dt1: %8x\r\n", r->dt1);
	SOC_LOGI("	dt2: %8x\r\n", r->dt2);
	SOC_LOGI("	dt3: %8x\r\n", r->dt3);
	SOC_LOGI("	reserved_30_31: %8x\r\n", r->reserved_30_31);
}

static void pwm_dump_reg11b(void)
{
	SOC_LOGI("reg11b: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x11b << 2)));
}

static void pwm_dump_reg11c(void)
{
	SOC_LOGI("reg11c: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x11c << 2)));
}

static void pwm_dump_reg11d(void)
{
	SOC_LOGI("reg11d: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x11d << 2)));
}

static void pwm_dump_reg11e(void)
{
	SOC_LOGI("reg11e: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x11e << 2)));
}

static void pwm_dump_reg11f(void)
{
	SOC_LOGI("reg11f: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x11f << 2)));
}

static void pwm_dump_reg120(void)
{
	SOC_LOGI("reg120: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x120 << 2)));
}

static void pwm_dump_reg121(void)
{
	SOC_LOGI("reg121: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x121 << 2)));
}

static void pwm_dump_reg122(void)
{
	SOC_LOGI("reg122: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x122 << 2)));
}

static void pwm_dump_reg123(void)
{
	SOC_LOGI("reg123: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x123 << 2)));
}

static void pwm_dump_reg124(void)
{
	SOC_LOGI("reg124: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x124 << 2)));
}

static void pwm_dump_reg125(void)
{
	SOC_LOGI("reg125: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x125 << 2)));
}

static void pwm_dump_reg126(void)
{
	SOC_LOGI("reg126: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x126 << 2)));
}

static void pwm_dump_reg127(void)
{
	SOC_LOGI("reg127: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x127 << 2)));
}

static void pwm_dump_reg128(void)
{
	SOC_LOGI("reg128: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x128 << 2)));
}

static void pwm_dump_reg129(void)
{
	SOC_LOGI("reg129: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x129 << 2)));
}

static void pwm_dump_reg12a(void)
{
	pwm_reg12a_t *r = (pwm_reg12a_t *)(SOC_PWM_REG_BASE + (0x12a << 2));

	SOC_LOGI("reg12a: %8x\r\n", REG_READ(SOC_PWM_REG_BASE + (0x12a << 2)));
	SOC_LOGI("	ch6e_shad: %8x\r\n", r->ch6e_shad);
	SOC_LOGI("	ch5e_shad: %8x\r\n", r->ch5e_shad);
	SOC_LOGI("	ch4e_shad: %8x\r\n", r->ch4e_shad);
	SOC_LOGI("	ch3e_shad: %8x\r\n", r->ch3e_shad);
	SOC_LOGI("	ch2e_shad: %8x\r\n", r->ch2e_shad);
	SOC_LOGI("	ch1e_shad: %8x\r\n", r->ch1e_shad);
	SOC_LOGI("	oc1m_shad: %8x\r\n", r->oc1m_shad);
	SOC_LOGI("	oc2m_shad: %8x\r\n", r->oc2m_shad);
	SOC_LOGI("	oc3m_shad: %8x\r\n", r->oc3m_shad);
	SOC_LOGI("	reserved_15_31: %8x\r\n", r->reserved_15_31);
}

static pwm_reg_fn_map_t s_fn[] =
{
	{0x0, 0x0, pwm_dump_reg0},
	{0x1, 0x1, pwm_dump_reg1},
	{0x2, 0x2, pwm_dump_reg2},
	{0x3, 0x3, pwm_dump_reg3},
	{0x4, 0x4, pwm_dump_reg4},
	{0x5, 0x10, pwm_dump_rsv_5_f},
	{0x10, 0x10, pwm_dump_reg10},
	{0x11, 0x11, pwm_dump_reg11},
	{0x12, 0x12, pwm_dump_reg12},
	{0x13, 0x13, pwm_dump_reg13},
	{0x14, 0x14, pwm_dump_reg14},
	{0x15, 0x15, pwm_dump_reg15},
	{0x16, 0x16, pwm_dump_reg16},
	{0x17, 0x17, pwm_dump_reg17},
	{0x18, 0x18, pwm_dump_reg18},
	{0x19, 0x19, pwm_dump_reg19},
	{0x1a, 0x1a, pwm_dump_reg1a},
	{0x1b, 0x1b, pwm_dump_reg1b},
	{0x1c, 0x20, pwm_dump_rsv_1c_1f},
	{0x20, 0x20, pwm_dump_reg20},
	{0x21, 0x21, pwm_dump_reg21},
	{0x22, 0x22, pwm_dump_reg22},
	{0x23, 0x23, pwm_dump_reg23},
	{0x24, 0x24, pwm_dump_reg24},
	{0x25, 0x25, pwm_dump_reg25},
	{0x26, 0x26, pwm_dump_reg26},
	{0x27, 0x27, pwm_dump_reg27},
	{0x28, 0x28, pwm_dump_reg28},
	{0x29, 0x29, pwm_dump_reg29},
	{0x2a, 0x2a, pwm_dump_reg2a},
	{0x2b, 0x2b, pwm_dump_reg2b},
	{0x2c, 0x2c, pwm_dump_reg2c},
	{0x2d, 0x2d, pwm_dump_reg2d},
	{0x2e, 0x2e, pwm_dump_reg2e},
	{0x2f, 0x2f, pwm_dump_reg2f},
	{0x30, 0x30, pwm_dump_reg30},
	{0x31, 0x31, pwm_dump_reg31},
	{0x32, 0x32, pwm_dump_reg32},
	{0x33, 0x33, pwm_dump_reg33},
	{0x34, 0x34, pwm_dump_reg34},
	{0x35, 0x35, pwm_dump_reg35},
	{0x36, 0x36, pwm_dump_reg36},
	{0x37, 0x37, pwm_dump_reg37},
	{0x38, 0x40, pwm_dump_rsv_38_3f},
	{0x40, 0x40, pwm_dump_reg40},
	{0x41, 0x41, pwm_dump_reg41},
	{0x42, 0x42, pwm_dump_reg42},
	{0x43, 0x43, pwm_dump_reg43},
	{0x44, 0x44, pwm_dump_reg44},
	{0x45, 0x45, pwm_dump_reg45},
	{0x46, 0x46, pwm_dump_reg46},
	{0x47, 0x47, pwm_dump_reg47},
	{0x48, 0x48, pwm_dump_reg48},
	{0x49, 0x49, pwm_dump_reg49},
	{0x4a, 0x4a, pwm_dump_reg4a},
	{0x4b, 0x4b, pwm_dump_reg4b},
	{0x4c, 0x4c, pwm_dump_reg4c},
	{0x4d, 0x4d, pwm_dump_reg4d},
	{0x4e, 0x4e, pwm_dump_reg4e},
	{0x4f, 0x4f, pwm_dump_reg4f},
	{0x50, 0x50, pwm_dump_reg50},
	{0x51, 0x51, pwm_dump_reg51},
	{0x52, 0x52, pwm_dump_reg52},
	{0x53, 0x53, pwm_dump_reg53},
	{0x54, 0x54, pwm_dump_reg54},
	{0x55, 0x55, pwm_dump_reg55},
	{0x56, 0x56, pwm_dump_reg56},
	{0x57, 0x57, pwm_dump_reg57},
	{0x58, 0x58, pwm_dump_reg58},
	{0x59, 0x59, pwm_dump_reg59},
	{0x5a, 0x5a, pwm_dump_reg5a},
	{0x5b, 0x5b, pwm_dump_reg5b},
	{0x5c, 0x5c, pwm_dump_reg5c},
	{0x5d, 0x5d, pwm_dump_reg5d},
	{0x5e, 0x5e, pwm_dump_reg5e},
	{0x5f, 0x5f, pwm_dump_reg5f},
	{0x60, 0x60, pwm_dump_reg60},
	{0x61, 0x61, pwm_dump_reg61},
	{0x62, 0x62, pwm_dump_reg62},
	{0x63, 0x63, pwm_dump_reg63},
	{0x64, 0x64, pwm_dump_reg64},
	{0x65, 0x65, pwm_dump_reg65},
	{0x66, 0x66, pwm_dump_reg66},
	{0x67, 0x67, pwm_dump_reg67},
	{0x68, 0x68, pwm_dump_reg68},
	{0x69, 0x69, pwm_dump_reg69},
	{0x6a, 0x6a, pwm_dump_reg6a},
	{0x6b, 0x80, pwm_dump_rsv_6b_7f},
	{0x80, 0x80, pwm_dump_reg80},
	{0x81, 0x81, pwm_dump_reg81},
	{0x82, 0x82, pwm_dump_reg82},
	{0x83, 0x83, pwm_dump_reg83},
	{0x84, 0x84, pwm_dump_reg84},
	{0x85, 0x85, pwm_dump_reg85},
	{0x86, 0x86, pwm_dump_reg86},
	{0x87, 0x87, pwm_dump_reg87},
	{0x88, 0x88, pwm_dump_reg88},
	{0x89, 0x89, pwm_dump_reg89},
	{0x8a, 0x8a, pwm_dump_reg8a},
	{0x8b, 0x8b, pwm_dump_reg8b},
	{0x8c, 0x8c, pwm_dump_reg8c},
	{0x8d, 0x8d, pwm_dump_reg8d},
	{0x8e, 0x8e, pwm_dump_reg8e},
	{0x8f, 0x8f, pwm_dump_reg8f},
	{0x90, 0x90, pwm_dump_reg90},
	{0x91, 0x91, pwm_dump_reg91},
	{0x92, 0x92, pwm_dump_reg92},
	{0x93, 0x93, pwm_dump_reg93},
	{0x94, 0x94, pwm_dump_reg94},
	{0x95, 0x95, pwm_dump_reg95},
	{0x96, 0x96, pwm_dump_reg96},
	{0x97, 0x97, pwm_dump_reg97},
	{0x98, 0x98, pwm_dump_reg98},
	{0x99, 0x99, pwm_dump_reg99},
	{0x9a, 0x9a, pwm_dump_reg9a},
	{0x9b, 0x9b, pwm_dump_reg9b},
	{0x9c, 0x9c, pwm_dump_reg9c},
	{0x9d, 0x9d, pwm_dump_reg9d},
	{0x9e, 0x9e, pwm_dump_reg9e},
	{0x9f, 0x9f, pwm_dump_reg9f},
	{0xa0, 0xa0, pwm_dump_rega0},
	{0xa1, 0xa1, pwm_dump_rega1},
	{0xa2, 0xa2, pwm_dump_rega2},
	{0xa3, 0xa3, pwm_dump_rega3},
	{0xa4, 0xa4, pwm_dump_rega4},
	{0xa5, 0xa5, pwm_dump_rega5},
	{0xa6, 0xa6, pwm_dump_rega6},
	{0xa7, 0xa7, pwm_dump_rega7},
	{0xa8, 0xa8, pwm_dump_rega8},
	{0xa9, 0xa9, pwm_dump_rega9},
	{0xaa, 0xaa, pwm_dump_regaa},
	{0xab, 0xc0, pwm_dump_rsv_ab_bf},
	{0xc0, 0xc0, pwm_dump_regc0},
	{0xc1, 0xc1, pwm_dump_regc1},
	{0xc2, 0xc2, pwm_dump_regc2},
	{0xc3, 0xc3, pwm_dump_regc3},
	{0xc4, 0xc4, pwm_dump_regc4},
	{0xc5, 0xc5, pwm_dump_regc5},
	{0xc6, 0xc6, pwm_dump_regc6},
	{0xc7, 0xc7, pwm_dump_regc7},
	{0xc8, 0xc8, pwm_dump_regc8},
	{0xc9, 0xc9, pwm_dump_regc9},
	{0xca, 0xca, pwm_dump_regca},
	{0xcb, 0xcb, pwm_dump_regcb},
	{0xcc, 0xcc, pwm_dump_regcc},
	{0xcd, 0xcd, pwm_dump_regcd},
	{0xce, 0xce, pwm_dump_regce},
	{0xcf, 0xcf, pwm_dump_regcf},
	{0xd0, 0xd0, pwm_dump_regd0},
	{0xd1, 0xd1, pwm_dump_regd1},
	{0xd2, 0xd2, pwm_dump_regd2},
	{0xd3, 0xd3, pwm_dump_regd3},
	{0xd4, 0xd4, pwm_dump_regd4},
	{0xd5, 0xd5, pwm_dump_regd5},
	{0xd6, 0xd6, pwm_dump_regd6},
	{0xd7, 0xd7, pwm_dump_regd7},
	{0xd8, 0xd8, pwm_dump_regd8},
	{0xd9, 0xd9, pwm_dump_regd9},
	{0xda, 0xda, pwm_dump_regda},
	{0xdb, 0xdb, pwm_dump_regdb},
	{0xdc, 0xdc, pwm_dump_regdc},
	{0xdd, 0xdd, pwm_dump_regdd},
	{0xde, 0xde, pwm_dump_regde},
	{0xdf, 0xdf, pwm_dump_regdf},
	{0xe0, 0xe0, pwm_dump_rege0},
	{0xe1, 0xe1, pwm_dump_rege1},
	{0xe2, 0xe2, pwm_dump_rege2},
	{0xe3, 0xe3, pwm_dump_rege3},
	{0xe4, 0xe4, pwm_dump_rege4},
	{0xe5, 0xe5, pwm_dump_rege5},
	{0xe6, 0xe6, pwm_dump_rege6},
	{0xe7, 0xe7, pwm_dump_rege7},
	{0xe8, 0xe8, pwm_dump_rege8},
	{0xe9, 0xe9, pwm_dump_rege9},
	{0xea, 0xea, pwm_dump_regea},
	{0xeb, 0x100, pwm_dump_rsv_eb_ff},
	{0x100, 0x100, pwm_dump_reg100},
	{0x101, 0x101, pwm_dump_reg101},
	{0x102, 0x102, pwm_dump_reg102},
	{0x103, 0x103, pwm_dump_reg103},
	{0x104, 0x104, pwm_dump_reg104},
	{0x105, 0x105, pwm_dump_reg105},
	{0x106, 0x106, pwm_dump_reg106},
	{0x107, 0x107, pwm_dump_reg107},
	{0x108, 0x108, pwm_dump_reg108},
	{0x109, 0x109, pwm_dump_reg109},
	{0x10a, 0x10a, pwm_dump_reg10a},
	{0x10b, 0x10b, pwm_dump_reg10b},
	{0x10c, 0x10c, pwm_dump_reg10c},
	{0x10d, 0x10d, pwm_dump_reg10d},
	{0x10e, 0x10e, pwm_dump_reg10e},
	{0x10f, 0x10f, pwm_dump_reg10f},
	{0x110, 0x110, pwm_dump_reg110},
	{0x111, 0x111, pwm_dump_reg111},
	{0x112, 0x112, pwm_dump_reg112},
	{0x113, 0x113, pwm_dump_reg113},
	{0x114, 0x114, pwm_dump_reg114},
	{0x115, 0x115, pwm_dump_reg115},
	{0x116, 0x116, pwm_dump_reg116},
	{0x117, 0x117, pwm_dump_reg117},
	{0x118, 0x118, pwm_dump_reg118},
	{0x119, 0x119, pwm_dump_reg119},
	{0x11a, 0x11a, pwm_dump_reg11a},
	{0x11b, 0x11b, pwm_dump_reg11b},
	{0x11c, 0x11c, pwm_dump_reg11c},
	{0x11d, 0x11d, pwm_dump_reg11d},
	{0x11e, 0x11e, pwm_dump_reg11e},
	{0x11f, 0x11f, pwm_dump_reg11f},
	{0x120, 0x120, pwm_dump_reg120},
	{0x121, 0x121, pwm_dump_reg121},
	{0x122, 0x122, pwm_dump_reg122},
	{0x123, 0x123, pwm_dump_reg123},
	{0x124, 0x124, pwm_dump_reg124},
	{0x125, 0x125, pwm_dump_reg125},
	{0x126, 0x126, pwm_dump_reg126},
	{0x127, 0x127, pwm_dump_reg127},
	{0x128, 0x128, pwm_dump_reg128},
	{0x129, 0x129, pwm_dump_reg129},
	{0x12a, 0x12a, pwm_dump_reg12a},
	{-1, -1, 0}
};

static void pwm_struct_dump_index(uint32_t start, uint32_t end)
{
	uint32_t dump_fn_cnt = sizeof(s_fn)/sizeof(s_fn[0]) - 1;

	for (uint32_t idx = 0; idx < dump_fn_cnt; idx++) {
		if ((start <= s_fn[idx].start) && (end >= s_fn[idx].end)) {
			s_fn[idx].fn();
		}
	}
}

void pwm_struct_dump(void)
{
	pwm_struct_dump_index(0, 219);
}
#endif
