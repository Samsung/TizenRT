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
#include "pwm_hw.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PWM_LL_REG_BASE   SOC_PWM_REG_BASE

//reg reg0:

static inline void pwm_ll_set_reg0_value(uint32_t v) {
	pwm_reg0_t *r = (pwm_reg0_t*)(SOC_PWM_REG_BASE + (0x0 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg0_value(void) {
	pwm_reg0_t *r = (pwm_reg0_t*)(SOC_PWM_REG_BASE + (0x0 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg0_deviceid(void) {
	pwm_reg0_t *r = (pwm_reg0_t*)(SOC_PWM_REG_BASE + (0x0 << 2));
	return r->deviceid;
}

//reg reg1:

static inline void pwm_ll_set_reg1_value(uint32_t v) {
	pwm_reg1_t *r = (pwm_reg1_t*)(SOC_PWM_REG_BASE + (0x1 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg1_value(void) {
	pwm_reg1_t *r = (pwm_reg1_t*)(SOC_PWM_REG_BASE + (0x1 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg1_versionid(void) {
	pwm_reg1_t *r = (pwm_reg1_t*)(SOC_PWM_REG_BASE + (0x1 << 2));
	return r->versionid;
}

//reg reg2:

static inline void pwm_ll_set_reg2_value(uint32_t v) {
	pwm_reg2_t *r = (pwm_reg2_t*)(SOC_PWM_REG_BASE + (0x2 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg2_value(void) {
	pwm_reg2_t *r = (pwm_reg2_t*)(SOC_PWM_REG_BASE + (0x2 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg2_soft_reset(uint32_t v) {
	pwm_reg2_t *r = (pwm_reg2_t*)(SOC_PWM_REG_BASE + (0x2 << 2));
	r->soft_reset = v;
}

static inline uint32_t pwm_ll_get_reg2_soft_reset(void) {
	pwm_reg2_t *r = (pwm_reg2_t*)(SOC_PWM_REG_BASE + (0x2 << 2));
	return r->soft_reset;
}

static inline void pwm_ll_set_reg2_bps_clkgate(uint32_t v) {
	pwm_reg2_t *r = (pwm_reg2_t*)(SOC_PWM_REG_BASE + (0x2 << 2));
	r->bps_clkgate = v;
}

static inline uint32_t pwm_ll_get_reg2_bps_clkgate(void) {
	pwm_reg2_t *r = (pwm_reg2_t*)(SOC_PWM_REG_BASE + (0x2 << 2));
	return r->bps_clkgate;
}

//reg reg3:

static inline void pwm_ll_set_reg3_value(uint32_t v) {
	pwm_reg3_t *r = (pwm_reg3_t*)(SOC_PWM_REG_BASE + (0x3 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg3_value(void) {
	pwm_reg3_t *r = (pwm_reg3_t*)(SOC_PWM_REG_BASE + (0x3 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg3_pwm_o(void) {
	pwm_reg3_t *r = (pwm_reg3_t*)(SOC_PWM_REG_BASE + (0x3 << 2));
	return r->pwm_o;
}

static inline void pwm_ll_set_reg3_reserved_12_31(uint32_t v) {
	pwm_reg3_t *r = (pwm_reg3_t*)(SOC_PWM_REG_BASE + (0x3 << 2));
	r->reserved_12_31 = v;
}

static inline uint32_t pwm_ll_get_reg3_reserved_12_31(void) {
	pwm_reg3_t *r = (pwm_reg3_t*)(SOC_PWM_REG_BASE + (0x3 << 2));
	return r->reserved_12_31;
}

//reg reg4:

static inline void pwm_ll_set_reg4_value(uint32_t v) {
	pwm_reg4_t *r = (pwm_reg4_t*)(SOC_PWM_REG_BASE + (0x4 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg4_value(void) {
	pwm_reg4_t *r = (pwm_reg4_t*)(SOC_PWM_REG_BASE + (0x4 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg4_cen1(uint32_t v) {
	pwm_reg4_t *r = (pwm_reg4_t*)(SOC_PWM_REG_BASE + (0x4 << 2));
	r->cen1 = v;
}

static inline uint32_t pwm_ll_get_reg4_cen1(void) {
	pwm_reg4_t *r = (pwm_reg4_t*)(SOC_PWM_REG_BASE + (0x4 << 2));
	return r->cen1;
}

static inline void pwm_ll_set_reg4_cen2(uint32_t v) {
	pwm_reg4_t *r = (pwm_reg4_t*)(SOC_PWM_REG_BASE + (0x4 << 2));
	r->cen2 = v;
}

static inline uint32_t pwm_ll_get_reg4_cen2(void) {
	pwm_reg4_t *r = (pwm_reg4_t*)(SOC_PWM_REG_BASE + (0x4 << 2));
	return r->cen2;
}

static inline void pwm_ll_set_reg4_cen3(uint32_t v) {
	pwm_reg4_t *r = (pwm_reg4_t*)(SOC_PWM_REG_BASE + (0x4 << 2));
	r->cen3 = v;
}

static inline uint32_t pwm_ll_get_reg4_cen3(void) {
	pwm_reg4_t *r = (pwm_reg4_t*)(SOC_PWM_REG_BASE + (0x4 << 2));
	return r->cen3;
}

static inline void pwm_ll_set_reg4_cen4(uint32_t v) {
	pwm_reg4_t *r = (pwm_reg4_t*)(SOC_PWM_REG_BASE + (0x4 << 2));
	r->cen4 = v;
}

static inline uint32_t pwm_ll_get_reg4_cen4(void) {
	pwm_reg4_t *r = (pwm_reg4_t*)(SOC_PWM_REG_BASE + (0x4 << 2));
	return r->cen4;
}

static inline void pwm_ll_set_reg4_cen5(uint32_t v) {
	pwm_reg4_t *r = (pwm_reg4_t*)(SOC_PWM_REG_BASE + (0x4 << 2));
	r->cen5 = v;
}

static inline uint32_t pwm_ll_get_reg4_cen5(void) {
	pwm_reg4_t *r = (pwm_reg4_t*)(SOC_PWM_REG_BASE + (0x4 << 2));
	return r->cen5;
}

static inline void pwm_ll_set_reg4_cen6(uint32_t v) {
	pwm_reg4_t *r = (pwm_reg4_t*)(SOC_PWM_REG_BASE + (0x4 << 2));
	r->cen6 = v;
}

static inline uint32_t pwm_ll_get_reg4_cen6(void) {
	pwm_reg4_t *r = (pwm_reg4_t*)(SOC_PWM_REG_BASE + (0x4 << 2));
	return r->cen6;
}

static inline void pwm_ll_set_reg4_cen7(uint32_t v) {
	pwm_reg4_t *r = (pwm_reg4_t*)(SOC_PWM_REG_BASE + (0x4 << 2));
	r->cen7 = v;
}

static inline uint32_t pwm_ll_get_reg4_cen7(void) {
	pwm_reg4_t *r = (pwm_reg4_t*)(SOC_PWM_REG_BASE + (0x4 << 2));
	return r->cen7;
}

static inline void pwm_ll_set_reg4_cen8(uint32_t v) {
	pwm_reg4_t *r = (pwm_reg4_t*)(SOC_PWM_REG_BASE + (0x4 << 2));
	r->cen8 = v;
}

static inline uint32_t pwm_ll_get_reg4_cen8(void) {
	pwm_reg4_t *r = (pwm_reg4_t*)(SOC_PWM_REG_BASE + (0x4 << 2));
	return r->cen8;
}

static inline void pwm_ll_set_reg4_cen9(uint32_t v) {
	pwm_reg4_t *r = (pwm_reg4_t*)(SOC_PWM_REG_BASE + (0x4 << 2));
	r->cen9 = v;
}

static inline uint32_t pwm_ll_get_reg4_cen9(void) {
	pwm_reg4_t *r = (pwm_reg4_t*)(SOC_PWM_REG_BASE + (0x4 << 2));
	return r->cen9;
}

static inline void pwm_ll_set_reg4_cen10(uint32_t v) {
	pwm_reg4_t *r = (pwm_reg4_t*)(SOC_PWM_REG_BASE + (0x4 << 2));
	r->cen10 = v;
}

static inline uint32_t pwm_ll_get_reg4_cen10(void) {
	pwm_reg4_t *r = (pwm_reg4_t*)(SOC_PWM_REG_BASE + (0x4 << 2));
	return r->cen10;
}

static inline void pwm_ll_set_reg4_cen11(uint32_t v) {
	pwm_reg4_t *r = (pwm_reg4_t*)(SOC_PWM_REG_BASE + (0x4 << 2));
	r->cen11 = v;
}

static inline uint32_t pwm_ll_get_reg4_cen11(void) {
	pwm_reg4_t *r = (pwm_reg4_t*)(SOC_PWM_REG_BASE + (0x4 << 2));
	return r->cen11;
}

static inline void pwm_ll_set_reg4_cen12(uint32_t v) {
	pwm_reg4_t *r = (pwm_reg4_t*)(SOC_PWM_REG_BASE + (0x4 << 2));
	r->cen12 = v;
}

static inline uint32_t pwm_ll_get_reg4_cen12(void) {
	pwm_reg4_t *r = (pwm_reg4_t*)(SOC_PWM_REG_BASE + (0x4 << 2));
	return r->cen12;
}

static inline void pwm_ll_set_reg4_sync_all(uint32_t v) {
	pwm_reg4_t *r = (pwm_reg4_t*)(SOC_PWM_REG_BASE + (0x4 << 2));
	r->sync_all = v;
}

static inline void pwm_ll_set_reg4_sync_all_en(uint32_t v) {
	pwm_reg4_t *r = (pwm_reg4_t*)(SOC_PWM_REG_BASE + (0x4 << 2));
	r->sync_all_en = v;
}

static inline uint32_t pwm_ll_get_reg4_sync_all_en(void) {
	pwm_reg4_t *r = (pwm_reg4_t*)(SOC_PWM_REG_BASE + (0x4 << 2));
	return r->sync_all_en;
}

static inline void pwm_ll_set_reg4_reserved_14_31(uint32_t v) {
	pwm_reg4_t *r = (pwm_reg4_t*)(SOC_PWM_REG_BASE + (0x4 << 2));
	r->reserved_14_31 = v;
}

static inline uint32_t pwm_ll_get_reg4_reserved_14_31(void) {
	pwm_reg4_t *r = (pwm_reg4_t*)(SOC_PWM_REG_BASE + (0x4 << 2));
	return r->reserved_14_31;
}

//reg reg10:

static inline void pwm_ll_set_reg10_value(uint32_t v) {
	pwm_reg10_t *r = (pwm_reg10_t*)(SOC_PWM_REG_BASE + (0x10 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg10_value(void) {
	pwm_reg10_t *r = (pwm_reg10_t*)(SOC_PWM_REG_BASE + (0x10 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg10_pwm_o_0_allocate(uint32_t v) {
	pwm_reg10_t *r = (pwm_reg10_t*)(SOC_PWM_REG_BASE + (0x10 << 2));
	r->pwm_o_0_allocate = v;
}

static inline uint32_t pwm_ll_get_reg10_pwm_o_0_allocate(void) {
	pwm_reg10_t *r = (pwm_reg10_t*)(SOC_PWM_REG_BASE + (0x10 << 2));
	return r->pwm_o_0_allocate;
}

//reg reg11:

static inline void pwm_ll_set_reg11_value(uint32_t v) {
	pwm_reg11_t *r = (pwm_reg11_t*)(SOC_PWM_REG_BASE + (0x11 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg11_value(void) {
	pwm_reg11_t *r = (pwm_reg11_t*)(SOC_PWM_REG_BASE + (0x11 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg11_pwm_o_1_allocate(uint32_t v) {
	pwm_reg11_t *r = (pwm_reg11_t*)(SOC_PWM_REG_BASE + (0x11 << 2));
	r->pwm_o_1_allocate = v;
}

static inline uint32_t pwm_ll_get_reg11_pwm_o_1_allocate(void) {
	pwm_reg11_t *r = (pwm_reg11_t*)(SOC_PWM_REG_BASE + (0x11 << 2));
	return r->pwm_o_1_allocate;
}

//reg reg12:

static inline void pwm_ll_set_reg12_value(uint32_t v) {
	pwm_reg12_t *r = (pwm_reg12_t*)(SOC_PWM_REG_BASE + (0x12 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg12_value(void) {
	pwm_reg12_t *r = (pwm_reg12_t*)(SOC_PWM_REG_BASE + (0x12 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg12_pwm_o_2_allocate(uint32_t v) {
	pwm_reg12_t *r = (pwm_reg12_t*)(SOC_PWM_REG_BASE + (0x12 << 2));
	r->pwm_o_2_allocate = v;
}

static inline uint32_t pwm_ll_get_reg12_pwm_o_2_allocate(void) {
	pwm_reg12_t *r = (pwm_reg12_t*)(SOC_PWM_REG_BASE + (0x12 << 2));
	return r->pwm_o_2_allocate;
}

//reg reg13:

static inline void pwm_ll_set_reg13_value(uint32_t v) {
	pwm_reg13_t *r = (pwm_reg13_t*)(SOC_PWM_REG_BASE + (0x13 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg13_value(void) {
	pwm_reg13_t *r = (pwm_reg13_t*)(SOC_PWM_REG_BASE + (0x13 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg13_pwm_o_3_allocate(uint32_t v) {
	pwm_reg13_t *r = (pwm_reg13_t*)(SOC_PWM_REG_BASE + (0x13 << 2));
	r->pwm_o_3_allocate = v;
}

static inline uint32_t pwm_ll_get_reg13_pwm_o_3_allocate(void) {
	pwm_reg13_t *r = (pwm_reg13_t*)(SOC_PWM_REG_BASE + (0x13 << 2));
	return r->pwm_o_3_allocate;
}

//reg reg14:

static inline void pwm_ll_set_reg14_value(uint32_t v) {
	pwm_reg14_t *r = (pwm_reg14_t*)(SOC_PWM_REG_BASE + (0x14 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg14_value(void) {
	pwm_reg14_t *r = (pwm_reg14_t*)(SOC_PWM_REG_BASE + (0x14 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg14_pwm_o_4_allocate(uint32_t v) {
	pwm_reg14_t *r = (pwm_reg14_t*)(SOC_PWM_REG_BASE + (0x14 << 2));
	r->pwm_o_4_allocate = v;
}

static inline uint32_t pwm_ll_get_reg14_pwm_o_4_allocate(void) {
	pwm_reg14_t *r = (pwm_reg14_t*)(SOC_PWM_REG_BASE + (0x14 << 2));
	return r->pwm_o_4_allocate;
}

//reg reg15:

static inline void pwm_ll_set_reg15_value(uint32_t v) {
	pwm_reg15_t *r = (pwm_reg15_t*)(SOC_PWM_REG_BASE + (0x15 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg15_value(void) {
	pwm_reg15_t *r = (pwm_reg15_t*)(SOC_PWM_REG_BASE + (0x15 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg15_pwm_o_5_allocate(uint32_t v) {
	pwm_reg15_t *r = (pwm_reg15_t*)(SOC_PWM_REG_BASE + (0x15 << 2));
	r->pwm_o_5_allocate = v;
}

static inline uint32_t pwm_ll_get_reg15_pwm_o_5_allocate(void) {
	pwm_reg15_t *r = (pwm_reg15_t*)(SOC_PWM_REG_BASE + (0x15 << 2));
	return r->pwm_o_5_allocate;
}

//reg reg16:

static inline void pwm_ll_set_reg16_value(uint32_t v) {
	pwm_reg16_t *r = (pwm_reg16_t*)(SOC_PWM_REG_BASE + (0x16 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg16_value(void) {
	pwm_reg16_t *r = (pwm_reg16_t*)(SOC_PWM_REG_BASE + (0x16 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg16_pwm_o_6_allocate(uint32_t v) {
	pwm_reg16_t *r = (pwm_reg16_t*)(SOC_PWM_REG_BASE + (0x16 << 2));
	r->pwm_o_6_allocate = v;
}

static inline uint32_t pwm_ll_get_reg16_pwm_o_6_allocate(void) {
	pwm_reg16_t *r = (pwm_reg16_t*)(SOC_PWM_REG_BASE + (0x16 << 2));
	return r->pwm_o_6_allocate;
}

//reg reg17:

static inline void pwm_ll_set_reg17_value(uint32_t v) {
	pwm_reg17_t *r = (pwm_reg17_t*)(SOC_PWM_REG_BASE + (0x17 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg17_value(void) {
	pwm_reg17_t *r = (pwm_reg17_t*)(SOC_PWM_REG_BASE + (0x17 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg17_pwm_o_7_allocate(uint32_t v) {
	pwm_reg17_t *r = (pwm_reg17_t*)(SOC_PWM_REG_BASE + (0x17 << 2));
	r->pwm_o_7_allocate = v;
}

static inline uint32_t pwm_ll_get_reg17_pwm_o_7_allocate(void) {
	pwm_reg17_t *r = (pwm_reg17_t*)(SOC_PWM_REG_BASE + (0x17 << 2));
	return r->pwm_o_7_allocate;
}

//reg reg18:

static inline void pwm_ll_set_reg18_value(uint32_t v) {
	pwm_reg18_t *r = (pwm_reg18_t*)(SOC_PWM_REG_BASE + (0x18 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg18_value(void) {
	pwm_reg18_t *r = (pwm_reg18_t*)(SOC_PWM_REG_BASE + (0x18 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg18_pwm_o_8_allocate(uint32_t v) {
	pwm_reg18_t *r = (pwm_reg18_t*)(SOC_PWM_REG_BASE + (0x18 << 2));
	r->pwm_o_8_allocate = v;
}

static inline uint32_t pwm_ll_get_reg18_pwm_o_8_allocate(void) {
	pwm_reg18_t *r = (pwm_reg18_t*)(SOC_PWM_REG_BASE + (0x18 << 2));
	return r->pwm_o_8_allocate;
}

//reg reg19:

static inline void pwm_ll_set_reg19_value(uint32_t v) {
	pwm_reg19_t *r = (pwm_reg19_t*)(SOC_PWM_REG_BASE + (0x19 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg19_value(void) {
	pwm_reg19_t *r = (pwm_reg19_t*)(SOC_PWM_REG_BASE + (0x19 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg19_pwm_o_9_allocate(uint32_t v) {
	pwm_reg19_t *r = (pwm_reg19_t*)(SOC_PWM_REG_BASE + (0x19 << 2));
	r->pwm_o_9_allocate = v;
}

static inline uint32_t pwm_ll_get_reg19_pwm_o_9_allocate(void) {
	pwm_reg19_t *r = (pwm_reg19_t*)(SOC_PWM_REG_BASE + (0x19 << 2));
	return r->pwm_o_9_allocate;
}

//reg reg1a:

static inline void pwm_ll_set_reg1a_value(uint32_t v) {
	pwm_reg1a_t *r = (pwm_reg1a_t*)(SOC_PWM_REG_BASE + (0x1a << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg1a_value(void) {
	pwm_reg1a_t *r = (pwm_reg1a_t*)(SOC_PWM_REG_BASE + (0x1a << 2));
	return r->v;
}

static inline void pwm_ll_set_reg1a_pwm_o_10_allocate(uint32_t v) {
	pwm_reg1a_t *r = (pwm_reg1a_t*)(SOC_PWM_REG_BASE + (0x1a << 2));
	r->pwm_o_10_allocate = v;
}

static inline uint32_t pwm_ll_get_reg1a_pwm_o_10_allocate(void) {
	pwm_reg1a_t *r = (pwm_reg1a_t*)(SOC_PWM_REG_BASE + (0x1a << 2));
	return r->pwm_o_10_allocate;
}

//reg reg1b:

static inline void pwm_ll_set_reg1b_value(uint32_t v) {
	pwm_reg1b_t *r = (pwm_reg1b_t*)(SOC_PWM_REG_BASE + (0x1b << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg1b_value(void) {
	pwm_reg1b_t *r = (pwm_reg1b_t*)(SOC_PWM_REG_BASE + (0x1b << 2));
	return r->v;
}

static inline void pwm_ll_set_reg1b_pwm_o_11_allocate(uint32_t v) {
	pwm_reg1b_t *r = (pwm_reg1b_t*)(SOC_PWM_REG_BASE + (0x1b << 2));
	r->pwm_o_11_allocate = v;
}

static inline uint32_t pwm_ll_get_reg1b_pwm_o_11_allocate(void) {
	pwm_reg1b_t *r = (pwm_reg1b_t*)(SOC_PWM_REG_BASE + (0x1b << 2));
	return r->pwm_o_11_allocate;
}

//reg reg20:

static inline void pwm_ll_set_reg20_value(uint32_t v) {
	pwm_reg20_t *r = (pwm_reg20_t*)(SOC_PWM_REG_BASE + (0x20 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg20_value(void) {
	pwm_reg20_t *r = (pwm_reg20_t*)(SOC_PWM_REG_BASE + (0x20 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg20_pwm_i_0_allocate(uint32_t v) {
	pwm_reg20_t *r = (pwm_reg20_t*)(SOC_PWM_REG_BASE + (0x20 << 2));
	r->pwm_i_0_allocate = v;
}

static inline uint32_t pwm_ll_get_reg20_pwm_i_0_allocate(void) {
	pwm_reg20_t *r = (pwm_reg20_t*)(SOC_PWM_REG_BASE + (0x20 << 2));
	return r->pwm_i_0_allocate;
}

//reg reg21:

static inline void pwm_ll_set_reg21_value(uint32_t v) {
	pwm_reg21_t *r = (pwm_reg21_t*)(SOC_PWM_REG_BASE + (0x21 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg21_value(void) {
	pwm_reg21_t *r = (pwm_reg21_t*)(SOC_PWM_REG_BASE + (0x21 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg21_pwm_i_1_allocate(uint32_t v) {
	pwm_reg21_t *r = (pwm_reg21_t*)(SOC_PWM_REG_BASE + (0x21 << 2));
	r->pwm_i_1_allocate = v;
}

static inline uint32_t pwm_ll_get_reg21_pwm_i_1_allocate(void) {
	pwm_reg21_t *r = (pwm_reg21_t*)(SOC_PWM_REG_BASE + (0x21 << 2));
	return r->pwm_i_1_allocate;
}

//reg reg22:

static inline void pwm_ll_set_reg22_value(uint32_t v) {
	pwm_reg22_t *r = (pwm_reg22_t*)(SOC_PWM_REG_BASE + (0x22 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg22_value(void) {
	pwm_reg22_t *r = (pwm_reg22_t*)(SOC_PWM_REG_BASE + (0x22 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg22_pwm_i_2_allocate(uint32_t v) {
	pwm_reg22_t *r = (pwm_reg22_t*)(SOC_PWM_REG_BASE + (0x22 << 2));
	r->pwm_i_2_allocate = v;
}

static inline uint32_t pwm_ll_get_reg22_pwm_i_2_allocate(void) {
	pwm_reg22_t *r = (pwm_reg22_t*)(SOC_PWM_REG_BASE + (0x22 << 2));
	return r->pwm_i_2_allocate;
}

//reg reg23:

static inline void pwm_ll_set_reg23_value(uint32_t v) {
	pwm_reg23_t *r = (pwm_reg23_t*)(SOC_PWM_REG_BASE + (0x23 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg23_value(void) {
	pwm_reg23_t *r = (pwm_reg23_t*)(SOC_PWM_REG_BASE + (0x23 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg23_pwm_i_3_allocate(uint32_t v) {
	pwm_reg23_t *r = (pwm_reg23_t*)(SOC_PWM_REG_BASE + (0x23 << 2));
	r->pwm_i_3_allocate = v;
}

static inline uint32_t pwm_ll_get_reg23_pwm_i_3_allocate(void) {
	pwm_reg23_t *r = (pwm_reg23_t*)(SOC_PWM_REG_BASE + (0x23 << 2));
	return r->pwm_i_3_allocate;
}

//reg reg24:

static inline void pwm_ll_set_reg24_value(uint32_t v) {
	pwm_reg24_t *r = (pwm_reg24_t*)(SOC_PWM_REG_BASE + (0x24 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg24_value(void) {
	pwm_reg24_t *r = (pwm_reg24_t*)(SOC_PWM_REG_BASE + (0x24 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg24_pwm_i_4_allocate(uint32_t v) {
	pwm_reg24_t *r = (pwm_reg24_t*)(SOC_PWM_REG_BASE + (0x24 << 2));
	r->pwm_i_4_allocate = v;
}

static inline uint32_t pwm_ll_get_reg24_pwm_i_4_allocate(void) {
	pwm_reg24_t *r = (pwm_reg24_t*)(SOC_PWM_REG_BASE + (0x24 << 2));
	return r->pwm_i_4_allocate;
}

//reg reg25:

static inline void pwm_ll_set_reg25_value(uint32_t v) {
	pwm_reg25_t *r = (pwm_reg25_t*)(SOC_PWM_REG_BASE + (0x25 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg25_value(void) {
	pwm_reg25_t *r = (pwm_reg25_t*)(SOC_PWM_REG_BASE + (0x25 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg25_pwm_i_5_allocate(uint32_t v) {
	pwm_reg25_t *r = (pwm_reg25_t*)(SOC_PWM_REG_BASE + (0x25 << 2));
	r->pwm_i_5_allocate = v;
}

static inline uint32_t pwm_ll_get_reg25_pwm_i_5_allocate(void) {
	pwm_reg25_t *r = (pwm_reg25_t*)(SOC_PWM_REG_BASE + (0x25 << 2));
	return r->pwm_i_5_allocate;
}

//reg reg26:

static inline void pwm_ll_set_reg26_value(uint32_t v) {
	pwm_reg26_t *r = (pwm_reg26_t*)(SOC_PWM_REG_BASE + (0x26 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg26_value(void) {
	pwm_reg26_t *r = (pwm_reg26_t*)(SOC_PWM_REG_BASE + (0x26 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg26_pwm_i_6_allocate(uint32_t v) {
	pwm_reg26_t *r = (pwm_reg26_t*)(SOC_PWM_REG_BASE + (0x26 << 2));
	r->pwm_i_6_allocate = v;
}

static inline uint32_t pwm_ll_get_reg26_pwm_i_6_allocate(void) {
	pwm_reg26_t *r = (pwm_reg26_t*)(SOC_PWM_REG_BASE + (0x26 << 2));
	return r->pwm_i_6_allocate;
}

//reg reg27:

static inline void pwm_ll_set_reg27_value(uint32_t v) {
	pwm_reg27_t *r = (pwm_reg27_t*)(SOC_PWM_REG_BASE + (0x27 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg27_value(void) {
	pwm_reg27_t *r = (pwm_reg27_t*)(SOC_PWM_REG_BASE + (0x27 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg27_pwm_i_7_allocate(uint32_t v) {
	pwm_reg27_t *r = (pwm_reg27_t*)(SOC_PWM_REG_BASE + (0x27 << 2));
	r->pwm_i_7_allocate = v;
}

static inline uint32_t pwm_ll_get_reg27_pwm_i_7_allocate(void) {
	pwm_reg27_t *r = (pwm_reg27_t*)(SOC_PWM_REG_BASE + (0x27 << 2));
	return r->pwm_i_7_allocate;
}

//reg reg28:

static inline void pwm_ll_set_reg28_value(uint32_t v) {
	pwm_reg28_t *r = (pwm_reg28_t*)(SOC_PWM_REG_BASE + (0x28 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg28_value(void) {
	pwm_reg28_t *r = (pwm_reg28_t*)(SOC_PWM_REG_BASE + (0x28 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg28_pwm_i_8_allocate(uint32_t v) {
	pwm_reg28_t *r = (pwm_reg28_t*)(SOC_PWM_REG_BASE + (0x28 << 2));
	r->pwm_i_8_allocate = v;
}

static inline uint32_t pwm_ll_get_reg28_pwm_i_8_allocate(void) {
	pwm_reg28_t *r = (pwm_reg28_t*)(SOC_PWM_REG_BASE + (0x28 << 2));
	return r->pwm_i_8_allocate;
}

//reg reg29:

static inline void pwm_ll_set_reg29_value(uint32_t v) {
	pwm_reg29_t *r = (pwm_reg29_t*)(SOC_PWM_REG_BASE + (0x29 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg29_value(void) {
	pwm_reg29_t *r = (pwm_reg29_t*)(SOC_PWM_REG_BASE + (0x29 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg29_pwm_i_9_allocate(uint32_t v) {
	pwm_reg29_t *r = (pwm_reg29_t*)(SOC_PWM_REG_BASE + (0x29 << 2));
	r->pwm_i_9_allocate = v;
}

static inline uint32_t pwm_ll_get_reg29_pwm_i_9_allocate(void) {
	pwm_reg29_t *r = (pwm_reg29_t*)(SOC_PWM_REG_BASE + (0x29 << 2));
	return r->pwm_i_9_allocate;
}

//reg reg2a:

static inline void pwm_ll_set_reg2a_value(uint32_t v) {
	pwm_reg2a_t *r = (pwm_reg2a_t*)(SOC_PWM_REG_BASE + (0x2a << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg2a_value(void) {
	pwm_reg2a_t *r = (pwm_reg2a_t*)(SOC_PWM_REG_BASE + (0x2a << 2));
	return r->v;
}

static inline void pwm_ll_set_reg2a_pwm_i_10_allocate(uint32_t v) {
	pwm_reg2a_t *r = (pwm_reg2a_t*)(SOC_PWM_REG_BASE + (0x2a << 2));
	r->pwm_i_10_allocate = v;
}

static inline uint32_t pwm_ll_get_reg2a_pwm_i_10_allocate(void) {
	pwm_reg2a_t *r = (pwm_reg2a_t*)(SOC_PWM_REG_BASE + (0x2a << 2));
	return r->pwm_i_10_allocate;
}

//reg reg2b:

static inline void pwm_ll_set_reg2b_value(uint32_t v) {
	pwm_reg2b_t *r = (pwm_reg2b_t*)(SOC_PWM_REG_BASE + (0x2b << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg2b_value(void) {
	pwm_reg2b_t *r = (pwm_reg2b_t*)(SOC_PWM_REG_BASE + (0x2b << 2));
	return r->v;
}

static inline void pwm_ll_set_reg2b_pwm_i_11_allocate(uint32_t v) {
	pwm_reg2b_t *r = (pwm_reg2b_t*)(SOC_PWM_REG_BASE + (0x2b << 2));
	r->pwm_i_11_allocate = v;
}

static inline uint32_t pwm_ll_get_reg2b_pwm_i_11_allocate(void) {
	pwm_reg2b_t *r = (pwm_reg2b_t*)(SOC_PWM_REG_BASE + (0x2b << 2));
	return r->pwm_i_11_allocate;
}

//reg reg2c:

static inline void pwm_ll_set_reg2c_value(uint32_t v) {
	pwm_reg2c_t *r = (pwm_reg2c_t*)(SOC_PWM_REG_BASE + (0x2c << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg2c_value(void) {
	pwm_reg2c_t *r = (pwm_reg2c_t*)(SOC_PWM_REG_BASE + (0x2c << 2));
	return r->v;
}

static inline void pwm_ll_set_reg2c_pwm_i_12_allocate(uint32_t v) {
	pwm_reg2c_t *r = (pwm_reg2c_t*)(SOC_PWM_REG_BASE + (0x2c << 2));
	r->pwm_i_12_allocate = v;
}

static inline uint32_t pwm_ll_get_reg2c_pwm_i_12_allocate(void) {
	pwm_reg2c_t *r = (pwm_reg2c_t*)(SOC_PWM_REG_BASE + (0x2c << 2));
	return r->pwm_i_12_allocate;
}

//reg reg2d:

static inline void pwm_ll_set_reg2d_value(uint32_t v) {
	pwm_reg2d_t *r = (pwm_reg2d_t*)(SOC_PWM_REG_BASE + (0x2d << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg2d_value(void) {
	pwm_reg2d_t *r = (pwm_reg2d_t*)(SOC_PWM_REG_BASE + (0x2d << 2));
	return r->v;
}

static inline void pwm_ll_set_reg2d_pwm_i_13_allocate(uint32_t v) {
	pwm_reg2d_t *r = (pwm_reg2d_t*)(SOC_PWM_REG_BASE + (0x2d << 2));
	r->pwm_i_13_allocate = v;
}

static inline uint32_t pwm_ll_get_reg2d_pwm_i_13_allocate(void) {
	pwm_reg2d_t *r = (pwm_reg2d_t*)(SOC_PWM_REG_BASE + (0x2d << 2));
	return r->pwm_i_13_allocate;
}

//reg reg2e:

static inline void pwm_ll_set_reg2e_value(uint32_t v) {
	pwm_reg2e_t *r = (pwm_reg2e_t*)(SOC_PWM_REG_BASE + (0x2e << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg2e_value(void) {
	pwm_reg2e_t *r = (pwm_reg2e_t*)(SOC_PWM_REG_BASE + (0x2e << 2));
	return r->v;
}

static inline void pwm_ll_set_reg2e_pwm_i_14_allocate(uint32_t v) {
	pwm_reg2e_t *r = (pwm_reg2e_t*)(SOC_PWM_REG_BASE + (0x2e << 2));
	r->pwm_i_14_allocate = v;
}

static inline uint32_t pwm_ll_get_reg2e_pwm_i_14_allocate(void) {
	pwm_reg2e_t *r = (pwm_reg2e_t*)(SOC_PWM_REG_BASE + (0x2e << 2));
	return r->pwm_i_14_allocate;
}

//reg reg2f:

static inline void pwm_ll_set_reg2f_value(uint32_t v) {
	pwm_reg2f_t *r = (pwm_reg2f_t*)(SOC_PWM_REG_BASE + (0x2f << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg2f_value(void) {
	pwm_reg2f_t *r = (pwm_reg2f_t*)(SOC_PWM_REG_BASE + (0x2f << 2));
	return r->v;
}

static inline void pwm_ll_set_reg2f_pwm_i_15_allocate(uint32_t v) {
	pwm_reg2f_t *r = (pwm_reg2f_t*)(SOC_PWM_REG_BASE + (0x2f << 2));
	r->pwm_i_15_allocate = v;
}

static inline uint32_t pwm_ll_get_reg2f_pwm_i_15_allocate(void) {
	pwm_reg2f_t *r = (pwm_reg2f_t*)(SOC_PWM_REG_BASE + (0x2f << 2));
	return r->pwm_i_15_allocate;
}

//reg reg30:

static inline void pwm_ll_set_reg30_value(uint32_t v) {
	pwm_reg30_t *r = (pwm_reg30_t*)(SOC_PWM_REG_BASE + (0x30 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg30_value(void) {
	pwm_reg30_t *r = (pwm_reg30_t*)(SOC_PWM_REG_BASE + (0x30 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg30_pwm_i_16_allocate(uint32_t v) {
	pwm_reg30_t *r = (pwm_reg30_t*)(SOC_PWM_REG_BASE + (0x30 << 2));
	r->pwm_i_16_allocate = v;
}

static inline uint32_t pwm_ll_get_reg30_pwm_i_16_allocate(void) {
	pwm_reg30_t *r = (pwm_reg30_t*)(SOC_PWM_REG_BASE + (0x30 << 2));
	return r->pwm_i_16_allocate;
}

//reg reg31:

static inline void pwm_ll_set_reg31_value(uint32_t v) {
	pwm_reg31_t *r = (pwm_reg31_t*)(SOC_PWM_REG_BASE + (0x31 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg31_value(void) {
	pwm_reg31_t *r = (pwm_reg31_t*)(SOC_PWM_REG_BASE + (0x31 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg31_pwm_i_17_allocate(uint32_t v) {
	pwm_reg31_t *r = (pwm_reg31_t*)(SOC_PWM_REG_BASE + (0x31 << 2));
	r->pwm_i_17_allocate = v;
}

static inline uint32_t pwm_ll_get_reg31_pwm_i_17_allocate(void) {
	pwm_reg31_t *r = (pwm_reg31_t*)(SOC_PWM_REG_BASE + (0x31 << 2));
	return r->pwm_i_17_allocate;
}

//reg reg32:

static inline void pwm_ll_set_reg32_value(uint32_t v) {
	pwm_reg32_t *r = (pwm_reg32_t*)(SOC_PWM_REG_BASE + (0x32 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg32_value(void) {
	pwm_reg32_t *r = (pwm_reg32_t*)(SOC_PWM_REG_BASE + (0x32 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg32_pwm_i_18_allocate(uint32_t v) {
	pwm_reg32_t *r = (pwm_reg32_t*)(SOC_PWM_REG_BASE + (0x32 << 2));
	r->pwm_i_18_allocate = v;
}

static inline uint32_t pwm_ll_get_reg32_pwm_i_18_allocate(void) {
	pwm_reg32_t *r = (pwm_reg32_t*)(SOC_PWM_REG_BASE + (0x32 << 2));
	return r->pwm_i_18_allocate;
}

//reg reg33:

static inline void pwm_ll_set_reg33_value(uint32_t v) {
	pwm_reg33_t *r = (pwm_reg33_t*)(SOC_PWM_REG_BASE + (0x33 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg33_value(void) {
	pwm_reg33_t *r = (pwm_reg33_t*)(SOC_PWM_REG_BASE + (0x33 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg33_pwm_i_19_allocate(uint32_t v) {
	pwm_reg33_t *r = (pwm_reg33_t*)(SOC_PWM_REG_BASE + (0x33 << 2));
	r->pwm_i_19_allocate = v;
}

static inline uint32_t pwm_ll_get_reg33_pwm_i_19_allocate(void) {
	pwm_reg33_t *r = (pwm_reg33_t*)(SOC_PWM_REG_BASE + (0x33 << 2));
	return r->pwm_i_19_allocate;
}

//reg reg34:

static inline void pwm_ll_set_reg34_value(uint32_t v) {
	pwm_reg34_t *r = (pwm_reg34_t*)(SOC_PWM_REG_BASE + (0x34 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg34_value(void) {
	pwm_reg34_t *r = (pwm_reg34_t*)(SOC_PWM_REG_BASE + (0x34 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg34_pwm_i_20_allocate(uint32_t v) {
	pwm_reg34_t *r = (pwm_reg34_t*)(SOC_PWM_REG_BASE + (0x34 << 2));
	r->pwm_i_20_allocate = v;
}

static inline uint32_t pwm_ll_get_reg34_pwm_i_20_allocate(void) {
	pwm_reg34_t *r = (pwm_reg34_t*)(SOC_PWM_REG_BASE + (0x34 << 2));
	return r->pwm_i_20_allocate;
}

//reg reg35:

static inline void pwm_ll_set_reg35_value(uint32_t v) {
	pwm_reg35_t *r = (pwm_reg35_t*)(SOC_PWM_REG_BASE + (0x35 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg35_value(void) {
	pwm_reg35_t *r = (pwm_reg35_t*)(SOC_PWM_REG_BASE + (0x35 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg35_pwm_i_21_allocate(uint32_t v) {
	pwm_reg35_t *r = (pwm_reg35_t*)(SOC_PWM_REG_BASE + (0x35 << 2));
	r->pwm_i_21_allocate = v;
}

static inline uint32_t pwm_ll_get_reg35_pwm_i_21_allocate(void) {
	pwm_reg35_t *r = (pwm_reg35_t*)(SOC_PWM_REG_BASE + (0x35 << 2));
	return r->pwm_i_21_allocate;
}

//reg reg36:

static inline void pwm_ll_set_reg36_value(uint32_t v) {
	pwm_reg36_t *r = (pwm_reg36_t*)(SOC_PWM_REG_BASE + (0x36 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg36_value(void) {
	pwm_reg36_t *r = (pwm_reg36_t*)(SOC_PWM_REG_BASE + (0x36 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg36_pwm_i_22_allocate(uint32_t v) {
	pwm_reg36_t *r = (pwm_reg36_t*)(SOC_PWM_REG_BASE + (0x36 << 2));
	r->pwm_i_22_allocate = v;
}

static inline uint32_t pwm_ll_get_reg36_pwm_i_22_allocate(void) {
	pwm_reg36_t *r = (pwm_reg36_t*)(SOC_PWM_REG_BASE + (0x36 << 2));
	return r->pwm_i_22_allocate;
}

//reg reg37:

static inline void pwm_ll_set_reg37_value(uint32_t v) {
	pwm_reg37_t *r = (pwm_reg37_t*)(SOC_PWM_REG_BASE + (0x37 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg37_value(void) {
	pwm_reg37_t *r = (pwm_reg37_t*)(SOC_PWM_REG_BASE + (0x37 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg37_pwm_i_23_allocate(uint32_t v) {
	pwm_reg37_t *r = (pwm_reg37_t*)(SOC_PWM_REG_BASE + (0x37 << 2));
	r->pwm_i_23_allocate = v;
}

static inline uint32_t pwm_ll_get_reg37_pwm_i_23_allocate(void) {
	pwm_reg37_t *r = (pwm_reg37_t*)(SOC_PWM_REG_BASE + (0x37 << 2));
	return r->pwm_i_23_allocate;
}

//reg reg40:

static inline void pwm_ll_set_reg40_value(uint32_t v) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg40_value(void) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg40_ocm3pe(uint32_t v) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	r->ocm3pe = v;
}

static inline uint32_t pwm_ll_get_reg40_ocm3pe(void) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	return r->ocm3pe;
}

static inline void pwm_ll_set_reg40_ocm2pe(uint32_t v) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	r->ocm2pe = v;
}

static inline uint32_t pwm_ll_get_reg40_ocm2pe(void) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	return r->ocm2pe;
}

static inline void pwm_ll_set_reg40_ocm1pe(uint32_t v) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	r->ocm1pe = v;
}

static inline uint32_t pwm_ll_get_reg40_ocm1pe(void) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	return r->ocm1pe;
}

static inline void pwm_ll_set_reg40_arpe3(uint32_t v) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	r->arpe3 = v;
}

static inline uint32_t pwm_ll_get_reg40_arpe3(void) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	return r->arpe3;
}

static inline void pwm_ll_set_reg40_arpe2(uint32_t v) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	r->arpe2 = v;
}

static inline uint32_t pwm_ll_get_reg40_arpe2(void) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	return r->arpe2;
}

static inline void pwm_ll_set_reg40_arpe1(uint32_t v) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	r->arpe1 = v;
}

static inline uint32_t pwm_ll_get_reg40_arpe1(void) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	return r->arpe1;
}

static inline void pwm_ll_set_reg40_oc3pe(uint32_t v) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	r->oc3pe = v;
}

static inline uint32_t pwm_ll_get_reg40_oc3pe(void) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	return r->oc3pe;
}

static inline void pwm_ll_set_reg40_oc2pe(uint32_t v) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	r->oc2pe = v;
}

static inline uint32_t pwm_ll_get_reg40_oc2pe(void) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	return r->oc2pe;
}

static inline void pwm_ll_set_reg40_oc1pe(uint32_t v) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	r->oc1pe = v;
}

static inline uint32_t pwm_ll_get_reg40_oc1pe(void) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	return r->oc1pe;
}

static inline void pwm_ll_set_reg40_reserved_9_17(uint32_t v) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	r->reserved_9_17 = v;
}

static inline uint32_t pwm_ll_get_reg40_reserved_9_17(void) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	return r->reserved_9_17;
}

static inline void pwm_ll_set_reg40_cms_tim3(uint32_t v) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	r->cms_tim3 = v;
}

static inline uint32_t pwm_ll_get_reg40_cms_tim3(void) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	return r->cms_tim3;
}

static inline void pwm_ll_set_reg40_cms_tim2(uint32_t v) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	r->cms_tim2 = v;
}

static inline uint32_t pwm_ll_get_reg40_cms_tim2(void) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	return r->cms_tim2;
}

static inline void pwm_ll_set_reg40_cms_tim1(uint32_t v) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	r->cms_tim1 = v;
}

static inline uint32_t pwm_ll_get_reg40_cms_tim1(void) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	return r->cms_tim1;
}

static inline void pwm_ll_set_reg40_reserved_24_25(uint32_t v) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	r->reserved_24_25 = v;
}

static inline uint32_t pwm_ll_get_reg40_reserved_24_25(void) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	return r->reserved_24_25;
}

static inline void pwm_ll_set_reg40_urs3(uint32_t v) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	r->urs3 = v;
}

static inline uint32_t pwm_ll_get_reg40_urs3(void) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	return r->urs3;
}

static inline void pwm_ll_set_reg40_urs2(uint32_t v) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	r->urs2 = v;
}

static inline uint32_t pwm_ll_get_reg40_urs2(void) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	return r->urs2;
}

static inline void pwm_ll_set_reg40_urs1(uint32_t v) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	r->urs1 = v;
}

static inline uint32_t pwm_ll_get_reg40_urs1(void) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	return r->urs1;
}

static inline void pwm_ll_set_reg40_dir_tim3(uint32_t v) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	r->dir_tim3 = v;
}

static inline uint32_t pwm_ll_get_reg40_dir_tim3(void) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	return r->dir_tim3;
}

static inline void pwm_ll_set_reg40_dir_tim2(uint32_t v) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	r->dir_tim2 = v;
}

static inline uint32_t pwm_ll_get_reg40_dir_tim2(void) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	return r->dir_tim2;
}

static inline void pwm_ll_set_reg40_dir_tim1(uint32_t v) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	r->dir_tim1 = v;
}

static inline uint32_t pwm_ll_get_reg40_dir_tim1(void) {
	pwm_reg40_t *r = (pwm_reg40_t*)(SOC_PWM_REG_BASE + (0x40 << 2));
	return r->dir_tim1;
}

//reg reg41:

static inline void pwm_ll_set_reg41_value(uint32_t v) {
	pwm_reg41_t *r = (pwm_reg41_t*)(SOC_PWM_REG_BASE + (0x41 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg41_value(void) {
	pwm_reg41_t *r = (pwm_reg41_t*)(SOC_PWM_REG_BASE + (0x41 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg41_reserved_0_7(uint32_t v) {
	pwm_reg41_t *r = (pwm_reg41_t*)(SOC_PWM_REG_BASE + (0x41 << 2));
	r->reserved_0_7 = v;
}

static inline uint32_t pwm_ll_get_reg41_reserved_0_7(void) {
	pwm_reg41_t *r = (pwm_reg41_t*)(SOC_PWM_REG_BASE + (0x41 << 2));
	return r->reserved_0_7;
}

static inline void pwm_ll_set_reg41_com_mode(uint32_t v) {
	pwm_reg41_t *r = (pwm_reg41_t*)(SOC_PWM_REG_BASE + (0x41 << 2));
	r->com_mode = v;
}

static inline uint32_t pwm_ll_get_reg41_com_mode(void) {
	pwm_reg41_t *r = (pwm_reg41_t*)(SOC_PWM_REG_BASE + (0x41 << 2));
	return r->com_mode;
}

static inline void pwm_ll_set_reg41_ccpc(uint32_t v) {
	pwm_reg41_t *r = (pwm_reg41_t*)(SOC_PWM_REG_BASE + (0x41 << 2));
	r->ccpc = v;
}

static inline uint32_t pwm_ll_get_reg41_ccpc(void) {
	pwm_reg41_t *r = (pwm_reg41_t*)(SOC_PWM_REG_BASE + (0x41 << 2));
	return r->ccpc;
}

static inline void pwm_ll_set_reg41_trios3(uint32_t v) {
	pwm_reg41_t *r = (pwm_reg41_t*)(SOC_PWM_REG_BASE + (0x41 << 2));
	r->trios3 = v;
}

static inline uint32_t pwm_ll_get_reg41_trios3(void) {
	pwm_reg41_t *r = (pwm_reg41_t*)(SOC_PWM_REG_BASE + (0x41 << 2));
	return r->trios3;
}

static inline void pwm_ll_set_reg41_adcs3(uint32_t v) {
	pwm_reg41_t *r = (pwm_reg41_t*)(SOC_PWM_REG_BASE + (0x41 << 2));
	r->adcs3 = v;
}

static inline uint32_t pwm_ll_get_reg41_adcs3(void) {
	pwm_reg41_t *r = (pwm_reg41_t*)(SOC_PWM_REG_BASE + (0x41 << 2));
	return r->adcs3;
}

static inline void pwm_ll_set_reg41_reserved_16_16(uint32_t v) {
	pwm_reg41_t *r = (pwm_reg41_t*)(SOC_PWM_REG_BASE + (0x41 << 2));
	r->reserved_16_16 = v;
}

static inline uint32_t pwm_ll_get_reg41_reserved_16_16(void) {
	pwm_reg41_t *r = (pwm_reg41_t*)(SOC_PWM_REG_BASE + (0x41 << 2));
	return r->reserved_16_16;
}

static inline void pwm_ll_set_reg41_ccus(uint32_t v) {
	pwm_reg41_t *r = (pwm_reg41_t*)(SOC_PWM_REG_BASE + (0x41 << 2));
	r->ccus = v;
}

static inline uint32_t pwm_ll_get_reg41_ccus(void) {
	pwm_reg41_t *r = (pwm_reg41_t*)(SOC_PWM_REG_BASE + (0x41 << 2));
	return r->ccus;
}

static inline void pwm_ll_set_reg41_trios2(uint32_t v) {
	pwm_reg41_t *r = (pwm_reg41_t*)(SOC_PWM_REG_BASE + (0x41 << 2));
	r->trios2 = v;
}

static inline uint32_t pwm_ll_get_reg41_trios2(void) {
	pwm_reg41_t *r = (pwm_reg41_t*)(SOC_PWM_REG_BASE + (0x41 << 2));
	return r->trios2;
}

static inline void pwm_ll_set_reg41_adcs2(uint32_t v) {
	pwm_reg41_t *r = (pwm_reg41_t*)(SOC_PWM_REG_BASE + (0x41 << 2));
	r->adcs2 = v;
}

static inline uint32_t pwm_ll_get_reg41_adcs2(void) {
	pwm_reg41_t *r = (pwm_reg41_t*)(SOC_PWM_REG_BASE + (0x41 << 2));
	return r->adcs2;
}

static inline void pwm_ll_set_reg41_reserved_24_25(uint32_t v) {
	pwm_reg41_t *r = (pwm_reg41_t*)(SOC_PWM_REG_BASE + (0x41 << 2));
	r->reserved_24_25 = v;
}

static inline uint32_t pwm_ll_get_reg41_reserved_24_25(void) {
	pwm_reg41_t *r = (pwm_reg41_t*)(SOC_PWM_REG_BASE + (0x41 << 2));
	return r->reserved_24_25;
}

static inline void pwm_ll_set_reg41_trios1(uint32_t v) {
	pwm_reg41_t *r = (pwm_reg41_t*)(SOC_PWM_REG_BASE + (0x41 << 2));
	r->trios1 = v;
}

static inline uint32_t pwm_ll_get_reg41_trios1(void) {
	pwm_reg41_t *r = (pwm_reg41_t*)(SOC_PWM_REG_BASE + (0x41 << 2));
	return r->trios1;
}

static inline void pwm_ll_set_reg41_adcs1(uint32_t v) {
	pwm_reg41_t *r = (pwm_reg41_t*)(SOC_PWM_REG_BASE + (0x41 << 2));
	r->adcs1 = v;
}

static inline uint32_t pwm_ll_get_reg41_adcs1(void) {
	pwm_reg41_t *r = (pwm_reg41_t*)(SOC_PWM_REG_BASE + (0x41 << 2));
	return r->adcs1;
}

//reg reg42:

static inline void pwm_ll_set_reg42_value(uint32_t v) {
	pwm_reg42_t *r = (pwm_reg42_t*)(SOC_PWM_REG_BASE + (0x42 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg42_value(void) {
	pwm_reg42_t *r = (pwm_reg42_t*)(SOC_PWM_REG_BASE + (0x42 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg42_sms1(uint32_t v) {
	pwm_reg42_t *r = (pwm_reg42_t*)(SOC_PWM_REG_BASE + (0x42 << 2));
	r->sms1 = v;
}

static inline uint32_t pwm_ll_get_reg42_sms1(void) {
	pwm_reg42_t *r = (pwm_reg42_t*)(SOC_PWM_REG_BASE + (0x42 << 2));
	return r->sms1;
}

static inline void pwm_ll_set_reg42_sms2(uint32_t v) {
	pwm_reg42_t *r = (pwm_reg42_t*)(SOC_PWM_REG_BASE + (0x42 << 2));
	r->sms2 = v;
}

static inline uint32_t pwm_ll_get_reg42_sms2(void) {
	pwm_reg42_t *r = (pwm_reg42_t*)(SOC_PWM_REG_BASE + (0x42 << 2));
	return r->sms2;
}

static inline void pwm_ll_set_reg42_sms3(uint32_t v) {
	pwm_reg42_t *r = (pwm_reg42_t*)(SOC_PWM_REG_BASE + (0x42 << 2));
	r->sms3 = v;
}

static inline uint32_t pwm_ll_get_reg42_sms3(void) {
	pwm_reg42_t *r = (pwm_reg42_t*)(SOC_PWM_REG_BASE + (0x42 << 2));
	return r->sms3;
}

static inline void pwm_ll_set_reg42_ts1(uint32_t v) {
	pwm_reg42_t *r = (pwm_reg42_t*)(SOC_PWM_REG_BASE + (0x42 << 2));
	r->ts1 = v;
}

static inline uint32_t pwm_ll_get_reg42_ts1(void) {
	pwm_reg42_t *r = (pwm_reg42_t*)(SOC_PWM_REG_BASE + (0x42 << 2));
	return r->ts1;
}

static inline void pwm_ll_set_reg42_reserved_15_15(uint32_t v) {
	pwm_reg42_t *r = (pwm_reg42_t*)(SOC_PWM_REG_BASE + (0x42 << 2));
	r->reserved_15_15 = v;
}

static inline uint32_t pwm_ll_get_reg42_reserved_15_15(void) {
	pwm_reg42_t *r = (pwm_reg42_t*)(SOC_PWM_REG_BASE + (0x42 << 2));
	return r->reserved_15_15;
}

static inline void pwm_ll_set_reg42_ts2(uint32_t v) {
	pwm_reg42_t *r = (pwm_reg42_t*)(SOC_PWM_REG_BASE + (0x42 << 2));
	r->ts2 = v;
}

static inline uint32_t pwm_ll_get_reg42_ts2(void) {
	pwm_reg42_t *r = (pwm_reg42_t*)(SOC_PWM_REG_BASE + (0x42 << 2));
	return r->ts2;
}

static inline void pwm_ll_set_reg42_ts3(uint32_t v) {
	pwm_reg42_t *r = (pwm_reg42_t*)(SOC_PWM_REG_BASE + (0x42 << 2));
	r->ts3 = v;
}

static inline uint32_t pwm_ll_get_reg42_ts3(void) {
	pwm_reg42_t *r = (pwm_reg42_t*)(SOC_PWM_REG_BASE + (0x42 << 2));
	return r->ts3;
}

static inline void pwm_ll_set_reg42_reserved_22_31(uint32_t v) {
	pwm_reg42_t *r = (pwm_reg42_t*)(SOC_PWM_REG_BASE + (0x42 << 2));
	r->reserved_22_31 = v;
}

static inline uint32_t pwm_ll_get_reg42_reserved_22_31(void) {
	pwm_reg42_t *r = (pwm_reg42_t*)(SOC_PWM_REG_BASE + (0x42 << 2));
	return r->reserved_22_31;
}

//reg reg43:

static inline void pwm_ll_set_reg43_value(uint32_t v) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg43_value(void) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg43_cc1ie(uint32_t v) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	r->cc1ie = v;
}

static inline uint32_t pwm_ll_get_reg43_cc1ie(void) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	return r->cc1ie;
}

static inline void pwm_ll_set_reg43_cc2ie(uint32_t v) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	r->cc2ie = v;
}

static inline uint32_t pwm_ll_get_reg43_cc2ie(void) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	return r->cc2ie;
}

static inline void pwm_ll_set_reg43_cc3ie(uint32_t v) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	r->cc3ie = v;
}

static inline uint32_t pwm_ll_get_reg43_cc3ie(void) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	return r->cc3ie;
}

static inline void pwm_ll_set_reg43_cc4ie(uint32_t v) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	r->cc4ie = v;
}

static inline uint32_t pwm_ll_get_reg43_cc4ie(void) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	return r->cc4ie;
}

static inline void pwm_ll_set_reg43_cc5ie(uint32_t v) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	r->cc5ie = v;
}

static inline uint32_t pwm_ll_get_reg43_cc5ie(void) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	return r->cc5ie;
}

static inline void pwm_ll_set_reg43_cc6ie(uint32_t v) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	r->cc6ie = v;
}

static inline uint32_t pwm_ll_get_reg43_cc6ie(void) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	return r->cc6ie;
}

static inline void pwm_ll_set_reg43_cc7ie(uint32_t v) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	r->cc7ie = v;
}

static inline uint32_t pwm_ll_get_reg43_cc7ie(void) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	return r->cc7ie;
}

static inline void pwm_ll_set_reg43_cc8ie(uint32_t v) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	r->cc8ie = v;
}

static inline uint32_t pwm_ll_get_reg43_cc8ie(void) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	return r->cc8ie;
}

static inline void pwm_ll_set_reg43_cc9ie(uint32_t v) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	r->cc9ie = v;
}

static inline uint32_t pwm_ll_get_reg43_cc9ie(void) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	return r->cc9ie;
}

static inline void pwm_ll_set_reg43_uie1(uint32_t v) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	r->uie1 = v;
}

static inline uint32_t pwm_ll_get_reg43_uie1(void) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	return r->uie1;
}

static inline void pwm_ll_set_reg43_uie2(uint32_t v) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	r->uie2 = v;
}

static inline uint32_t pwm_ll_get_reg43_uie2(void) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	return r->uie2;
}

static inline void pwm_ll_set_reg43_uie3(uint32_t v) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	r->uie3 = v;
}

static inline uint32_t pwm_ll_get_reg43_uie3(void) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	return r->uie3;
}

static inline void pwm_ll_set_reg43_tie1(uint32_t v) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	r->tie1 = v;
}

static inline uint32_t pwm_ll_get_reg43_tie1(void) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	return r->tie1;
}

static inline void pwm_ll_set_reg43_tie2(uint32_t v) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	r->tie2 = v;
}

static inline uint32_t pwm_ll_get_reg43_tie2(void) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	return r->tie2;
}

static inline void pwm_ll_set_reg43_tie3(uint32_t v) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	r->tie3 = v;
}

static inline uint32_t pwm_ll_get_reg43_tie3(void) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	return r->tie3;
}

static inline void pwm_ll_set_reg43_comie(uint32_t v) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	r->comie = v;
}

static inline uint32_t pwm_ll_get_reg43_comie(void) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	return r->comie;
}

static inline void pwm_ll_set_reg43_reserved_16_19(uint32_t v) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	r->reserved_16_19 = v;
}

static inline uint32_t pwm_ll_get_reg43_reserved_16_19(void) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	return r->reserved_16_19;
}

static inline void pwm_ll_set_reg43_df_tim1(uint32_t v) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	r->df_tim1 = v;
}

static inline uint32_t pwm_ll_get_reg43_df_tim1(void) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	return r->df_tim1;
}

static inline void pwm_ll_set_reg43_df_tim2(uint32_t v) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	r->df_tim2 = v;
}

static inline uint32_t pwm_ll_get_reg43_df_tim2(void) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	return r->df_tim2;
}

static inline void pwm_ll_set_reg43_df_tim3(uint32_t v) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	r->df_tim3 = v;
}

static inline uint32_t pwm_ll_get_reg43_df_tim3(void) {
	pwm_reg43_t *r = (pwm_reg43_t*)(SOC_PWM_REG_BASE + (0x43 << 2));
	return r->df_tim3;
}

//reg reg44:

static inline void pwm_ll_set_reg44_value(uint32_t v) {
	pwm_reg44_t *r = (pwm_reg44_t*)(SOC_PWM_REG_BASE + (0x44 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg44_value(void) {
	pwm_reg44_t *r = (pwm_reg44_t*)(SOC_PWM_REG_BASE + (0x44 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg44_cc1if(uint32_t v) {
	pwm_reg44_t *r = (pwm_reg44_t*)(SOC_PWM_REG_BASE + (0x44 << 2));
	r->cc1if = v;
}

static inline void pwm_ll_set_reg44_cc2if(uint32_t v) {
	pwm_reg44_t *r = (pwm_reg44_t*)(SOC_PWM_REG_BASE + (0x44 << 2));
	r->cc2if = v;
}

static inline void pwm_ll_set_reg44_cc3if(uint32_t v) {
	pwm_reg44_t *r = (pwm_reg44_t*)(SOC_PWM_REG_BASE + (0x44 << 2));
	r->cc3if = v;
}

static inline void pwm_ll_set_reg44_cc4if(uint32_t v) {
	pwm_reg44_t *r = (pwm_reg44_t*)(SOC_PWM_REG_BASE + (0x44 << 2));
	r->cc4if = v;
}

static inline void pwm_ll_set_reg44_cc5if(uint32_t v) {
	pwm_reg44_t *r = (pwm_reg44_t*)(SOC_PWM_REG_BASE + (0x44 << 2));
	r->cc5if = v;
}

static inline void pwm_ll_set_reg44_cc6if(uint32_t v) {
	pwm_reg44_t *r = (pwm_reg44_t*)(SOC_PWM_REG_BASE + (0x44 << 2));
	r->cc6if = v;
}

static inline void pwm_ll_set_reg44_cc7if(uint32_t v) {
	pwm_reg44_t *r = (pwm_reg44_t*)(SOC_PWM_REG_BASE + (0x44 << 2));
	r->cc7if = v;
}

static inline void pwm_ll_set_reg44_cc8if(uint32_t v) {
	pwm_reg44_t *r = (pwm_reg44_t*)(SOC_PWM_REG_BASE + (0x44 << 2));
	r->cc8if = v;
}

static inline void pwm_ll_set_reg44_cc9if(uint32_t v) {
	pwm_reg44_t *r = (pwm_reg44_t*)(SOC_PWM_REG_BASE + (0x44 << 2));
	r->cc9if = v;
}

static inline void pwm_ll_set_reg44_uif1(uint32_t v) {
	pwm_reg44_t *r = (pwm_reg44_t*)(SOC_PWM_REG_BASE + (0x44 << 2));
	r->uif1 = v;
}

static inline void pwm_ll_set_reg44_uif2(uint32_t v) {
	pwm_reg44_t *r = (pwm_reg44_t*)(SOC_PWM_REG_BASE + (0x44 << 2));
	r->uif2 = v;
}

static inline void pwm_ll_set_reg44_uif3(uint32_t v) {
	pwm_reg44_t *r = (pwm_reg44_t*)(SOC_PWM_REG_BASE + (0x44 << 2));
	r->uif3 = v;
}

static inline void pwm_ll_set_reg44_tif1(uint32_t v) {
	pwm_reg44_t *r = (pwm_reg44_t*)(SOC_PWM_REG_BASE + (0x44 << 2));
	r->tif1 = v;
}

static inline void pwm_ll_set_reg44_tif2(uint32_t v) {
	pwm_reg44_t *r = (pwm_reg44_t*)(SOC_PWM_REG_BASE + (0x44 << 2));
	r->tif2 = v;
}

static inline void pwm_ll_set_reg44_tif3(uint32_t v) {
	pwm_reg44_t *r = (pwm_reg44_t*)(SOC_PWM_REG_BASE + (0x44 << 2));
	r->tif3 = v;
}

static inline void pwm_ll_set_reg44_comif(uint32_t v) {
	pwm_reg44_t *r = (pwm_reg44_t*)(SOC_PWM_REG_BASE + (0x44 << 2));
	r->comif = v;
}

static inline void pwm_ll_set_reg44_ch1eif(uint32_t v) {
	pwm_reg44_t *r = (pwm_reg44_t*)(SOC_PWM_REG_BASE + (0x44 << 2));
	r->ch1eif = v;
}

static inline uint32_t pwm_ll_get_reg44_ch1eif(void) {
	pwm_reg44_t *r = (pwm_reg44_t*)(SOC_PWM_REG_BASE + (0x44 << 2));
	return r->ch1eif;
}

static inline void pwm_ll_set_reg44_ch2eif(uint32_t v) {
	pwm_reg44_t *r = (pwm_reg44_t*)(SOC_PWM_REG_BASE + (0x44 << 2));
	r->ch2eif = v;
}

static inline uint32_t pwm_ll_get_reg44_ch2eif(void) {
	pwm_reg44_t *r = (pwm_reg44_t*)(SOC_PWM_REG_BASE + (0x44 << 2));
	return r->ch2eif;
}

static inline void pwm_ll_set_reg44_ch3eif(uint32_t v) {
	pwm_reg44_t *r = (pwm_reg44_t*)(SOC_PWM_REG_BASE + (0x44 << 2));
	r->ch3eif = v;
}

static inline uint32_t pwm_ll_get_reg44_ch3eif(void) {
	pwm_reg44_t *r = (pwm_reg44_t*)(SOC_PWM_REG_BASE + (0x44 << 2));
	return r->ch3eif;
}

static inline void pwm_ll_set_reg44_ch4eif(uint32_t v) {
	pwm_reg44_t *r = (pwm_reg44_t*)(SOC_PWM_REG_BASE + (0x44 << 2));
	r->ch4eif = v;
}

static inline uint32_t pwm_ll_get_reg44_ch4eif(void) {
	pwm_reg44_t *r = (pwm_reg44_t*)(SOC_PWM_REG_BASE + (0x44 << 2));
	return r->ch4eif;
}

static inline void pwm_ll_set_reg44_ch5eif(uint32_t v) {
	pwm_reg44_t *r = (pwm_reg44_t*)(SOC_PWM_REG_BASE + (0x44 << 2));
	r->ch5eif = v;
}

static inline uint32_t pwm_ll_get_reg44_ch5eif(void) {
	pwm_reg44_t *r = (pwm_reg44_t*)(SOC_PWM_REG_BASE + (0x44 << 2));
	return r->ch5eif;
}

static inline void pwm_ll_set_reg44_ch6eif(uint32_t v) {
	pwm_reg44_t *r = (pwm_reg44_t*)(SOC_PWM_REG_BASE + (0x44 << 2));
	r->ch6eif = v;
}

static inline uint32_t pwm_ll_get_reg44_ch6eif(void) {
	pwm_reg44_t *r = (pwm_reg44_t*)(SOC_PWM_REG_BASE + (0x44 << 2));
	return r->ch6eif;
}

static inline void pwm_ll_set_reg44_reserved_22_31(uint32_t v) {
	pwm_reg44_t *r = (pwm_reg44_t*)(SOC_PWM_REG_BASE + (0x44 << 2));
	r->reserved_22_31 = v;
}

static inline uint32_t pwm_ll_get_reg44_reserved_22_31(void) {
	pwm_reg44_t *r = (pwm_reg44_t*)(SOC_PWM_REG_BASE + (0x44 << 2));
	return r->reserved_22_31;
}

//reg reg45:

static inline void pwm_ll_set_reg45_value(uint32_t v) {
	pwm_reg45_t *r = (pwm_reg45_t*)(SOC_PWM_REG_BASE + (0x45 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg45_value(void) {
	pwm_reg45_t *r = (pwm_reg45_t*)(SOC_PWM_REG_BASE + (0x45 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg45_reserved_0_8(uint32_t v) {
	pwm_reg45_t *r = (pwm_reg45_t*)(SOC_PWM_REG_BASE + (0x45 << 2));
	r->reserved_0_8 = v;
}

static inline uint32_t pwm_ll_get_reg45_reserved_0_8(void) {
	pwm_reg45_t *r = (pwm_reg45_t*)(SOC_PWM_REG_BASE + (0x45 << 2));
	return r->reserved_0_8;
}

static inline void pwm_ll_set_reg45_ug1(uint32_t v) {
	pwm_reg45_t *r = (pwm_reg45_t*)(SOC_PWM_REG_BASE + (0x45 << 2));
	r->ug1 = v;
}

static inline void pwm_ll_set_reg45_ug2(uint32_t v) {
	pwm_reg45_t *r = (pwm_reg45_t*)(SOC_PWM_REG_BASE + (0x45 << 2));
	r->ug2 = v;
}

static inline void pwm_ll_set_reg45_ug3(uint32_t v) {
	pwm_reg45_t *r = (pwm_reg45_t*)(SOC_PWM_REG_BASE + (0x45 << 2));
	r->ug3 = v;
}

static inline void pwm_ll_set_reg45_tg1(uint32_t v) {
	pwm_reg45_t *r = (pwm_reg45_t*)(SOC_PWM_REG_BASE + (0x45 << 2));
	r->tg1 = v;
}

static inline void pwm_ll_set_reg45_tg2(uint32_t v) {
	pwm_reg45_t *r = (pwm_reg45_t*)(SOC_PWM_REG_BASE + (0x45 << 2));
	r->tg2 = v;
}

static inline void pwm_ll_set_reg45_tg3(uint32_t v) {
	pwm_reg45_t *r = (pwm_reg45_t*)(SOC_PWM_REG_BASE + (0x45 << 2));
	r->tg3 = v;
}

static inline void pwm_ll_set_reg45_comg(uint32_t v) {
	pwm_reg45_t *r = (pwm_reg45_t*)(SOC_PWM_REG_BASE + (0x45 << 2));
	r->comg = v;
}

static inline void pwm_ll_set_reg45_dtm1(uint32_t v) {
	pwm_reg45_t *r = (pwm_reg45_t*)(SOC_PWM_REG_BASE + (0x45 << 2));
	r->dtm1 = v;
}

static inline uint32_t pwm_ll_get_reg45_dtm1(void) {
	pwm_reg45_t *r = (pwm_reg45_t*)(SOC_PWM_REG_BASE + (0x45 << 2));
	return r->dtm1;
}

static inline void pwm_ll_set_reg45_dtm2(uint32_t v) {
	pwm_reg45_t *r = (pwm_reg45_t*)(SOC_PWM_REG_BASE + (0x45 << 2));
	r->dtm2 = v;
}

static inline uint32_t pwm_ll_get_reg45_dtm2(void) {
	pwm_reg45_t *r = (pwm_reg45_t*)(SOC_PWM_REG_BASE + (0x45 << 2));
	return r->dtm2;
}

static inline void pwm_ll_set_reg45_dtm3(uint32_t v) {
	pwm_reg45_t *r = (pwm_reg45_t*)(SOC_PWM_REG_BASE + (0x45 << 2));
	r->dtm3 = v;
}

static inline uint32_t pwm_ll_get_reg45_dtm3(void) {
	pwm_reg45_t *r = (pwm_reg45_t*)(SOC_PWM_REG_BASE + (0x45 << 2));
	return r->dtm3;
}

static inline void pwm_ll_set_reg45_init_level1(uint32_t v) {
	pwm_reg45_t *r = (pwm_reg45_t*)(SOC_PWM_REG_BASE + (0x45 << 2));
	r->init_level1 = v;
}

static inline uint32_t pwm_ll_get_reg45_init_level1(void) {
	pwm_reg45_t *r = (pwm_reg45_t*)(SOC_PWM_REG_BASE + (0x45 << 2));
	return r->init_level1;
}

static inline void pwm_ll_set_reg45_init_level2(uint32_t v) {
	pwm_reg45_t *r = (pwm_reg45_t*)(SOC_PWM_REG_BASE + (0x45 << 2));
	r->init_level2 = v;
}

static inline uint32_t pwm_ll_get_reg45_init_level2(void) {
	pwm_reg45_t *r = (pwm_reg45_t*)(SOC_PWM_REG_BASE + (0x45 << 2));
	return r->init_level2;
}

static inline void pwm_ll_set_reg45_init_level3(uint32_t v) {
	pwm_reg45_t *r = (pwm_reg45_t*)(SOC_PWM_REG_BASE + (0x45 << 2));
	r->init_level3 = v;
}

static inline uint32_t pwm_ll_get_reg45_init_level3(void) {
	pwm_reg45_t *r = (pwm_reg45_t*)(SOC_PWM_REG_BASE + (0x45 << 2));
	return r->init_level3;
}

static inline void pwm_ll_set_reg45_reserved_25_31(uint32_t v) {
	pwm_reg45_t *r = (pwm_reg45_t*)(SOC_PWM_REG_BASE + (0x45 << 2));
	r->reserved_25_31 = v;
}

static inline uint32_t pwm_ll_get_reg45_reserved_25_31(void) {
	pwm_reg45_t *r = (pwm_reg45_t*)(SOC_PWM_REG_BASE + (0x45 << 2));
	return r->reserved_25_31;
}

//reg reg46:

static inline void pwm_ll_set_reg46_value(uint32_t v) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg46_value(void) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg46_ch1p(uint32_t v) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	r->ch1p = v;
}

static inline uint32_t pwm_ll_get_reg46_ch1p(void) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	return r->ch1p;
}

static inline void pwm_ll_set_reg46_ch2p(uint32_t v) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	r->ch2p = v;
}

static inline uint32_t pwm_ll_get_reg46_ch2p(void) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	return r->ch2p;
}

static inline void pwm_ll_set_reg46_ch3p(uint32_t v) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	r->ch3p = v;
}

static inline uint32_t pwm_ll_get_reg46_ch3p(void) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	return r->ch3p;
}

static inline void pwm_ll_set_reg46_ch4p(uint32_t v) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	r->ch4p = v;
}

static inline uint32_t pwm_ll_get_reg46_ch4p(void) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	return r->ch4p;
}

static inline void pwm_ll_set_reg46_ch5p(uint32_t v) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	r->ch5p = v;
}

static inline uint32_t pwm_ll_get_reg46_ch5p(void) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	return r->ch5p;
}

static inline void pwm_ll_set_reg46_ch6p(uint32_t v) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	r->ch6p = v;
}

static inline uint32_t pwm_ll_get_reg46_ch6p(void) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	return r->ch6p;
}

static inline void pwm_ll_set_reg46_ch1e(uint32_t v) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	r->ch1e = v;
}

static inline uint32_t pwm_ll_get_reg46_ch1e(void) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	return r->ch1e;
}

static inline void pwm_ll_set_reg46_ch2e(uint32_t v) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	r->ch2e = v;
}

static inline uint32_t pwm_ll_get_reg46_ch2e(void) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	return r->ch2e;
}

static inline void pwm_ll_set_reg46_ch3e(uint32_t v) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	r->ch3e = v;
}

static inline uint32_t pwm_ll_get_reg46_ch3e(void) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	return r->ch3e;
}

static inline void pwm_ll_set_reg46_ch4e(uint32_t v) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	r->ch4e = v;
}

static inline uint32_t pwm_ll_get_reg46_ch4e(void) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	return r->ch4e;
}

static inline void pwm_ll_set_reg46_ch5e(uint32_t v) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	r->ch5e = v;
}

static inline uint32_t pwm_ll_get_reg46_ch5e(void) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	return r->ch5e;
}

static inline void pwm_ll_set_reg46_ch6e(uint32_t v) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	r->ch6e = v;
}

static inline uint32_t pwm_ll_get_reg46_ch6e(void) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	return r->ch6e;
}

static inline void pwm_ll_set_reg46_tim1ccm(uint32_t v) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	r->tim1ccm = v;
}

static inline uint32_t pwm_ll_get_reg46_tim1ccm(void) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	return r->tim1ccm;
}

static inline void pwm_ll_set_reg46_tim2ccm(uint32_t v) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	r->tim2ccm = v;
}

static inline uint32_t pwm_ll_get_reg46_tim2ccm(void) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	return r->tim2ccm;
}

static inline void pwm_ll_set_reg46_tim3ccm(uint32_t v) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	r->tim3ccm = v;
}

static inline uint32_t pwm_ll_get_reg46_tim3ccm(void) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	return r->tim3ccm;
}

static inline void pwm_ll_set_reg46_oc1m(uint32_t v) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	r->oc1m = v;
}

static inline uint32_t pwm_ll_get_reg46_oc1m(void) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	return r->oc1m;
}

static inline void pwm_ll_set_reg46_oc2m(uint32_t v) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	r->oc2m = v;
}

static inline uint32_t pwm_ll_get_reg46_oc2m(void) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	return r->oc2m;
}

static inline void pwm_ll_set_reg46_oc3m(uint32_t v) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	r->oc3m = v;
}

static inline uint32_t pwm_ll_get_reg46_oc3m(void) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	return r->oc3m;
}

static inline void pwm_ll_set_reg46_reserved_30_31(uint32_t v) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	r->reserved_30_31 = v;
}

static inline uint32_t pwm_ll_get_reg46_reserved_30_31(void) {
	pwm_reg46_t *r = (pwm_reg46_t*)(SOC_PWM_REG_BASE + (0x46 << 2));
	return r->reserved_30_31;
}

//reg reg47:

static inline void pwm_ll_set_reg47_value(uint32_t v) {
	pwm_reg47_t *r = (pwm_reg47_t*)(SOC_PWM_REG_BASE + (0x47 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg47_value(void) {
	pwm_reg47_t *r = (pwm_reg47_t*)(SOC_PWM_REG_BASE + (0x47 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg47_tim1_cnt(void) {
	pwm_reg47_t *r = (pwm_reg47_t*)(SOC_PWM_REG_BASE + (0x47 << 2));
	return r->tim1_cnt;
}

//reg reg48:

static inline void pwm_ll_set_reg48_value(uint32_t v) {
	pwm_reg48_t *r = (pwm_reg48_t*)(SOC_PWM_REG_BASE + (0x48 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg48_value(void) {
	pwm_reg48_t *r = (pwm_reg48_t*)(SOC_PWM_REG_BASE + (0x48 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg48_tim2_cnt(void) {
	pwm_reg48_t *r = (pwm_reg48_t*)(SOC_PWM_REG_BASE + (0x48 << 2));
	return r->tim2_cnt;
}

//reg reg49:

static inline void pwm_ll_set_reg49_value(uint32_t v) {
	pwm_reg49_t *r = (pwm_reg49_t*)(SOC_PWM_REG_BASE + (0x49 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg49_value(void) {
	pwm_reg49_t *r = (pwm_reg49_t*)(SOC_PWM_REG_BASE + (0x49 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg49_tim3_cnt(void) {
	pwm_reg49_t *r = (pwm_reg49_t*)(SOC_PWM_REG_BASE + (0x49 << 2));
	return r->tim3_cnt;
}

//reg reg4a:

static inline void pwm_ll_set_reg4a_value(uint32_t v) {
	pwm_reg4a_t *r = (pwm_reg4a_t*)(SOC_PWM_REG_BASE + (0x4a << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg4a_value(void) {
	pwm_reg4a_t *r = (pwm_reg4a_t*)(SOC_PWM_REG_BASE + (0x4a << 2));
	return r->v;
}

static inline void pwm_ll_set_reg4a_psc1(uint32_t v) {
	pwm_reg4a_t *r = (pwm_reg4a_t*)(SOC_PWM_REG_BASE + (0x4a << 2));
	r->psc1 = v;
}

static inline uint32_t pwm_ll_get_reg4a_psc1(void) {
	pwm_reg4a_t *r = (pwm_reg4a_t*)(SOC_PWM_REG_BASE + (0x4a << 2));
	return r->psc1;
}

static inline void pwm_ll_set_reg4a_psc2(uint32_t v) {
	pwm_reg4a_t *r = (pwm_reg4a_t*)(SOC_PWM_REG_BASE + (0x4a << 2));
	r->psc2 = v;
}

static inline uint32_t pwm_ll_get_reg4a_psc2(void) {
	pwm_reg4a_t *r = (pwm_reg4a_t*)(SOC_PWM_REG_BASE + (0x4a << 2));
	return r->psc2;
}

static inline void pwm_ll_set_reg4a_psc3(uint32_t v) {
	pwm_reg4a_t *r = (pwm_reg4a_t*)(SOC_PWM_REG_BASE + (0x4a << 2));
	r->psc3 = v;
}

static inline uint32_t pwm_ll_get_reg4a_psc3(void) {
	pwm_reg4a_t *r = (pwm_reg4a_t*)(SOC_PWM_REG_BASE + (0x4a << 2));
	return r->psc3;
}

static inline void pwm_ll_set_reg4a_reserved_24_31(uint32_t v) {
	pwm_reg4a_t *r = (pwm_reg4a_t*)(SOC_PWM_REG_BASE + (0x4a << 2));
	r->reserved_24_31 = v;
}

static inline uint32_t pwm_ll_get_reg4a_reserved_24_31(void) {
	pwm_reg4a_t *r = (pwm_reg4a_t*)(SOC_PWM_REG_BASE + (0x4a << 2));
	return r->reserved_24_31;
}

//reg reg4b:

static inline void pwm_ll_set_reg4b_value(uint32_t v) {
	pwm_reg4b_t *r = (pwm_reg4b_t*)(SOC_PWM_REG_BASE + (0x4b << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg4b_value(void) {
	pwm_reg4b_t *r = (pwm_reg4b_t*)(SOC_PWM_REG_BASE + (0x4b << 2));
	return r->v;
}

static inline void pwm_ll_set_reg4b_tim1_arr(uint32_t v) {
	pwm_reg4b_t *r = (pwm_reg4b_t*)(SOC_PWM_REG_BASE + (0x4b << 2));
	r->tim1_arr = v;
}

static inline uint32_t pwm_ll_get_reg4b_tim1_arr(void) {
	pwm_reg4b_t *r = (pwm_reg4b_t*)(SOC_PWM_REG_BASE + (0x4b << 2));
	return r->tim1_arr;
}

//reg reg4c:

static inline void pwm_ll_set_reg4c_value(uint32_t v) {
	pwm_reg4c_t *r = (pwm_reg4c_t*)(SOC_PWM_REG_BASE + (0x4c << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg4c_value(void) {
	pwm_reg4c_t *r = (pwm_reg4c_t*)(SOC_PWM_REG_BASE + (0x4c << 2));
	return r->v;
}

static inline void pwm_ll_set_reg4c_tim2_arr(uint32_t v) {
	pwm_reg4c_t *r = (pwm_reg4c_t*)(SOC_PWM_REG_BASE + (0x4c << 2));
	r->tim2_arr = v;
}

static inline uint32_t pwm_ll_get_reg4c_tim2_arr(void) {
	pwm_reg4c_t *r = (pwm_reg4c_t*)(SOC_PWM_REG_BASE + (0x4c << 2));
	return r->tim2_arr;
}

//reg reg4d:

static inline void pwm_ll_set_reg4d_value(uint32_t v) {
	pwm_reg4d_t *r = (pwm_reg4d_t*)(SOC_PWM_REG_BASE + (0x4d << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg4d_value(void) {
	pwm_reg4d_t *r = (pwm_reg4d_t*)(SOC_PWM_REG_BASE + (0x4d << 2));
	return r->v;
}

static inline void pwm_ll_set_reg4d_tim3_arr(uint32_t v) {
	pwm_reg4d_t *r = (pwm_reg4d_t*)(SOC_PWM_REG_BASE + (0x4d << 2));
	r->tim3_arr = v;
}

static inline uint32_t pwm_ll_get_reg4d_tim3_arr(void) {
	pwm_reg4d_t *r = (pwm_reg4d_t*)(SOC_PWM_REG_BASE + (0x4d << 2));
	return r->tim3_arr;
}

//reg reg4e:

static inline void pwm_ll_set_reg4e_value(uint32_t v) {
	pwm_reg4e_t *r = (pwm_reg4e_t*)(SOC_PWM_REG_BASE + (0x4e << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg4e_value(void) {
	pwm_reg4e_t *r = (pwm_reg4e_t*)(SOC_PWM_REG_BASE + (0x4e << 2));
	return r->v;
}

static inline void pwm_ll_set_reg4e_tim1_rcr(uint32_t v) {
	pwm_reg4e_t *r = (pwm_reg4e_t*)(SOC_PWM_REG_BASE + (0x4e << 2));
	r->tim1_rcr = v;
}

static inline uint32_t pwm_ll_get_reg4e_tim1_rcr(void) {
	pwm_reg4e_t *r = (pwm_reg4e_t*)(SOC_PWM_REG_BASE + (0x4e << 2));
	return r->tim1_rcr;
}

//reg reg4f:

static inline void pwm_ll_set_reg4f_value(uint32_t v) {
	pwm_reg4f_t *r = (pwm_reg4f_t*)(SOC_PWM_REG_BASE + (0x4f << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg4f_value(void) {
	pwm_reg4f_t *r = (pwm_reg4f_t*)(SOC_PWM_REG_BASE + (0x4f << 2));
	return r->v;
}

static inline void pwm_ll_set_reg4f_tim2_rcr(uint32_t v) {
	pwm_reg4f_t *r = (pwm_reg4f_t*)(SOC_PWM_REG_BASE + (0x4f << 2));
	r->tim2_rcr = v;
}

static inline uint32_t pwm_ll_get_reg4f_tim2_rcr(void) {
	pwm_reg4f_t *r = (pwm_reg4f_t*)(SOC_PWM_REG_BASE + (0x4f << 2));
	return r->tim2_rcr;
}

//reg reg50:

static inline void pwm_ll_set_reg50_value(uint32_t v) {
	pwm_reg50_t *r = (pwm_reg50_t*)(SOC_PWM_REG_BASE + (0x50 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg50_value(void) {
	pwm_reg50_t *r = (pwm_reg50_t*)(SOC_PWM_REG_BASE + (0x50 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg50_tim3_rcr(uint32_t v) {
	pwm_reg50_t *r = (pwm_reg50_t*)(SOC_PWM_REG_BASE + (0x50 << 2));
	r->tim3_rcr = v;
}

static inline uint32_t pwm_ll_get_reg50_tim3_rcr(void) {
	pwm_reg50_t *r = (pwm_reg50_t*)(SOC_PWM_REG_BASE + (0x50 << 2));
	return r->tim3_rcr;
}

//reg reg51:

static inline void pwm_ll_set_reg51_value(uint32_t v) {
	pwm_reg51_t *r = (pwm_reg51_t*)(SOC_PWM_REG_BASE + (0x51 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg51_value(void) {
	pwm_reg51_t *r = (pwm_reg51_t*)(SOC_PWM_REG_BASE + (0x51 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg51_ccr1(uint32_t v) {
	pwm_reg51_t *r = (pwm_reg51_t*)(SOC_PWM_REG_BASE + (0x51 << 2));
	r->ccr1 = v;
}

static inline uint32_t pwm_ll_get_reg51_ccr1(void) {
	pwm_reg51_t *r = (pwm_reg51_t*)(SOC_PWM_REG_BASE + (0x51 << 2));
	return r->ccr1;
}

//reg reg52:

static inline void pwm_ll_set_reg52_value(uint32_t v) {
	pwm_reg52_t *r = (pwm_reg52_t*)(SOC_PWM_REG_BASE + (0x52 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg52_value(void) {
	pwm_reg52_t *r = (pwm_reg52_t*)(SOC_PWM_REG_BASE + (0x52 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg52_ccr2(uint32_t v) {
	pwm_reg52_t *r = (pwm_reg52_t*)(SOC_PWM_REG_BASE + (0x52 << 2));
	r->ccr2 = v;
}

static inline uint32_t pwm_ll_get_reg52_ccr2(void) {
	pwm_reg52_t *r = (pwm_reg52_t*)(SOC_PWM_REG_BASE + (0x52 << 2));
	return r->ccr2;
}

//reg reg53:

static inline void pwm_ll_set_reg53_value(uint32_t v) {
	pwm_reg53_t *r = (pwm_reg53_t*)(SOC_PWM_REG_BASE + (0x53 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg53_value(void) {
	pwm_reg53_t *r = (pwm_reg53_t*)(SOC_PWM_REG_BASE + (0x53 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg53_ccr3(uint32_t v) {
	pwm_reg53_t *r = (pwm_reg53_t*)(SOC_PWM_REG_BASE + (0x53 << 2));
	r->ccr3 = v;
}

static inline uint32_t pwm_ll_get_reg53_ccr3(void) {
	pwm_reg53_t *r = (pwm_reg53_t*)(SOC_PWM_REG_BASE + (0x53 << 2));
	return r->ccr3;
}

//reg reg54:

static inline void pwm_ll_set_reg54_value(uint32_t v) {
	pwm_reg54_t *r = (pwm_reg54_t*)(SOC_PWM_REG_BASE + (0x54 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg54_value(void) {
	pwm_reg54_t *r = (pwm_reg54_t*)(SOC_PWM_REG_BASE + (0x54 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg54_ccr4(uint32_t v) {
	pwm_reg54_t *r = (pwm_reg54_t*)(SOC_PWM_REG_BASE + (0x54 << 2));
	r->ccr4 = v;
}

static inline uint32_t pwm_ll_get_reg54_ccr4(void) {
	pwm_reg54_t *r = (pwm_reg54_t*)(SOC_PWM_REG_BASE + (0x54 << 2));
	return r->ccr4;
}

//reg reg55:

static inline void pwm_ll_set_reg55_value(uint32_t v) {
	pwm_reg55_t *r = (pwm_reg55_t*)(SOC_PWM_REG_BASE + (0x55 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg55_value(void) {
	pwm_reg55_t *r = (pwm_reg55_t*)(SOC_PWM_REG_BASE + (0x55 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg55_ccr5(uint32_t v) {
	pwm_reg55_t *r = (pwm_reg55_t*)(SOC_PWM_REG_BASE + (0x55 << 2));
	r->ccr5 = v;
}

static inline uint32_t pwm_ll_get_reg55_ccr5(void) {
	pwm_reg55_t *r = (pwm_reg55_t*)(SOC_PWM_REG_BASE + (0x55 << 2));
	return r->ccr5;
}

//reg reg56:

static inline void pwm_ll_set_reg56_value(uint32_t v) {
	pwm_reg56_t *r = (pwm_reg56_t*)(SOC_PWM_REG_BASE + (0x56 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg56_value(void) {
	pwm_reg56_t *r = (pwm_reg56_t*)(SOC_PWM_REG_BASE + (0x56 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg56_ccr6(uint32_t v) {
	pwm_reg56_t *r = (pwm_reg56_t*)(SOC_PWM_REG_BASE + (0x56 << 2));
	r->ccr6 = v;
}

static inline uint32_t pwm_ll_get_reg56_ccr6(void) {
	pwm_reg56_t *r = (pwm_reg56_t*)(SOC_PWM_REG_BASE + (0x56 << 2));
	return r->ccr6;
}

//reg reg57:

static inline void pwm_ll_set_reg57_value(uint32_t v) {
	pwm_reg57_t *r = (pwm_reg57_t*)(SOC_PWM_REG_BASE + (0x57 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg57_value(void) {
	pwm_reg57_t *r = (pwm_reg57_t*)(SOC_PWM_REG_BASE + (0x57 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg57_ccr7(uint32_t v) {
	pwm_reg57_t *r = (pwm_reg57_t*)(SOC_PWM_REG_BASE + (0x57 << 2));
	r->ccr7 = v;
}

static inline uint32_t pwm_ll_get_reg57_ccr7(void) {
	pwm_reg57_t *r = (pwm_reg57_t*)(SOC_PWM_REG_BASE + (0x57 << 2));
	return r->ccr7;
}

//reg reg58:

static inline void pwm_ll_set_reg58_value(uint32_t v) {
	pwm_reg58_t *r = (pwm_reg58_t*)(SOC_PWM_REG_BASE + (0x58 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg58_value(void) {
	pwm_reg58_t *r = (pwm_reg58_t*)(SOC_PWM_REG_BASE + (0x58 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg58_ccr8(uint32_t v) {
	pwm_reg58_t *r = (pwm_reg58_t*)(SOC_PWM_REG_BASE + (0x58 << 2));
	r->ccr8 = v;
}

static inline uint32_t pwm_ll_get_reg58_ccr8(void) {
	pwm_reg58_t *r = (pwm_reg58_t*)(SOC_PWM_REG_BASE + (0x58 << 2));
	return r->ccr8;
}

//reg reg59:

static inline void pwm_ll_set_reg59_value(uint32_t v) {
	pwm_reg59_t *r = (pwm_reg59_t*)(SOC_PWM_REG_BASE + (0x59 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg59_value(void) {
	pwm_reg59_t *r = (pwm_reg59_t*)(SOC_PWM_REG_BASE + (0x59 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg59_ccr9(uint32_t v) {
	pwm_reg59_t *r = (pwm_reg59_t*)(SOC_PWM_REG_BASE + (0x59 << 2));
	r->ccr9 = v;
}

static inline uint32_t pwm_ll_get_reg59_ccr9(void) {
	pwm_reg59_t *r = (pwm_reg59_t*)(SOC_PWM_REG_BASE + (0x59 << 2));
	return r->ccr9;
}

//reg reg5a:

static inline void pwm_ll_set_reg5a_value(uint32_t v) {
	pwm_reg5a_t *r = (pwm_reg5a_t*)(SOC_PWM_REG_BASE + (0x5a << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg5a_value(void) {
	pwm_reg5a_t *r = (pwm_reg5a_t*)(SOC_PWM_REG_BASE + (0x5a << 2));
	return r->v;
}

static inline void pwm_ll_set_reg5a_dt1(uint32_t v) {
	pwm_reg5a_t *r = (pwm_reg5a_t*)(SOC_PWM_REG_BASE + (0x5a << 2));
	r->dt1 = v;
}

static inline uint32_t pwm_ll_get_reg5a_dt1(void) {
	pwm_reg5a_t *r = (pwm_reg5a_t*)(SOC_PWM_REG_BASE + (0x5a << 2));
	return r->dt1;
}

static inline void pwm_ll_set_reg5a_dt2(uint32_t v) {
	pwm_reg5a_t *r = (pwm_reg5a_t*)(SOC_PWM_REG_BASE + (0x5a << 2));
	r->dt2 = v;
}

static inline uint32_t pwm_ll_get_reg5a_dt2(void) {
	pwm_reg5a_t *r = (pwm_reg5a_t*)(SOC_PWM_REG_BASE + (0x5a << 2));
	return r->dt2;
}

static inline void pwm_ll_set_reg5a_dt3(uint32_t v) {
	pwm_reg5a_t *r = (pwm_reg5a_t*)(SOC_PWM_REG_BASE + (0x5a << 2));
	r->dt3 = v;
}

static inline uint32_t pwm_ll_get_reg5a_dt3(void) {
	pwm_reg5a_t *r = (pwm_reg5a_t*)(SOC_PWM_REG_BASE + (0x5a << 2));
	return r->dt3;
}

static inline void pwm_ll_set_reg5a_reserved_30_31(uint32_t v) {
	pwm_reg5a_t *r = (pwm_reg5a_t*)(SOC_PWM_REG_BASE + (0x5a << 2));
	r->reserved_30_31 = v;
}

static inline uint32_t pwm_ll_get_reg5a_reserved_30_31(void) {
	pwm_reg5a_t *r = (pwm_reg5a_t*)(SOC_PWM_REG_BASE + (0x5a << 2));
	return r->reserved_30_31;
}

//reg reg5b:

static inline void pwm_ll_set_reg5b_value(uint32_t v) {
	pwm_reg5b_t *r = (pwm_reg5b_t*)(SOC_PWM_REG_BASE + (0x5b << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg5b_value(void) {
	pwm_reg5b_t *r = (pwm_reg5b_t*)(SOC_PWM_REG_BASE + (0x5b << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg5b_tim1_arr_shad(void) {
	pwm_reg5b_t *r = (pwm_reg5b_t*)(SOC_PWM_REG_BASE + (0x5b << 2));
	return r->tim1_arr_shad;
}

//reg reg5c:

static inline void pwm_ll_set_reg5c_value(uint32_t v) {
	pwm_reg5c_t *r = (pwm_reg5c_t*)(SOC_PWM_REG_BASE + (0x5c << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg5c_value(void) {
	pwm_reg5c_t *r = (pwm_reg5c_t*)(SOC_PWM_REG_BASE + (0x5c << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg5c_tim2_arr_shad(void) {
	pwm_reg5c_t *r = (pwm_reg5c_t*)(SOC_PWM_REG_BASE + (0x5c << 2));
	return r->tim2_arr_shad;
}

//reg reg5d:

static inline void pwm_ll_set_reg5d_value(uint32_t v) {
	pwm_reg5d_t *r = (pwm_reg5d_t*)(SOC_PWM_REG_BASE + (0x5d << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg5d_value(void) {
	pwm_reg5d_t *r = (pwm_reg5d_t*)(SOC_PWM_REG_BASE + (0x5d << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg5d_tim3_arr_shad(void) {
	pwm_reg5d_t *r = (pwm_reg5d_t*)(SOC_PWM_REG_BASE + (0x5d << 2));
	return r->tim3_arr_shad;
}

//reg reg5e:

static inline void pwm_ll_set_reg5e_value(uint32_t v) {
	pwm_reg5e_t *r = (pwm_reg5e_t*)(SOC_PWM_REG_BASE + (0x5e << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg5e_value(void) {
	pwm_reg5e_t *r = (pwm_reg5e_t*)(SOC_PWM_REG_BASE + (0x5e << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg5e_tim1_rcr_shad(void) {
	pwm_reg5e_t *r = (pwm_reg5e_t*)(SOC_PWM_REG_BASE + (0x5e << 2));
	return r->tim1_rcr_shad;
}

//reg reg5f:

static inline void pwm_ll_set_reg5f_value(uint32_t v) {
	pwm_reg5f_t *r = (pwm_reg5f_t*)(SOC_PWM_REG_BASE + (0x5f << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg5f_value(void) {
	pwm_reg5f_t *r = (pwm_reg5f_t*)(SOC_PWM_REG_BASE + (0x5f << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg5f_tim2_rcr_shad(void) {
	pwm_reg5f_t *r = (pwm_reg5f_t*)(SOC_PWM_REG_BASE + (0x5f << 2));
	return r->tim2_rcr_shad;
}

//reg reg60:

static inline void pwm_ll_set_reg60_value(uint32_t v) {
	pwm_reg60_t *r = (pwm_reg60_t*)(SOC_PWM_REG_BASE + (0x60 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg60_value(void) {
	pwm_reg60_t *r = (pwm_reg60_t*)(SOC_PWM_REG_BASE + (0x60 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg60_tim3_rcr_shad(void) {
	pwm_reg60_t *r = (pwm_reg60_t*)(SOC_PWM_REG_BASE + (0x60 << 2));
	return r->tim3_rcr_shad;
}

//reg reg61:

static inline void pwm_ll_set_reg61_value(uint32_t v) {
	pwm_reg61_t *r = (pwm_reg61_t*)(SOC_PWM_REG_BASE + (0x61 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg61_value(void) {
	pwm_reg61_t *r = (pwm_reg61_t*)(SOC_PWM_REG_BASE + (0x61 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg61_ccr1_shad(void) {
	pwm_reg61_t *r = (pwm_reg61_t*)(SOC_PWM_REG_BASE + (0x61 << 2));
	return r->ccr1_shad;
}

//reg reg62:

static inline void pwm_ll_set_reg62_value(uint32_t v) {
	pwm_reg62_t *r = (pwm_reg62_t*)(SOC_PWM_REG_BASE + (0x62 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg62_value(void) {
	pwm_reg62_t *r = (pwm_reg62_t*)(SOC_PWM_REG_BASE + (0x62 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg62_ccr2_shad(void) {
	pwm_reg62_t *r = (pwm_reg62_t*)(SOC_PWM_REG_BASE + (0x62 << 2));
	return r->ccr2_shad;
}

//reg reg63:

static inline void pwm_ll_set_reg63_value(uint32_t v) {
	pwm_reg63_t *r = (pwm_reg63_t*)(SOC_PWM_REG_BASE + (0x63 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg63_value(void) {
	pwm_reg63_t *r = (pwm_reg63_t*)(SOC_PWM_REG_BASE + (0x63 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg63_ccr3_shad(void) {
	pwm_reg63_t *r = (pwm_reg63_t*)(SOC_PWM_REG_BASE + (0x63 << 2));
	return r->ccr3_shad;
}

//reg reg64:

static inline void pwm_ll_set_reg64_value(uint32_t v) {
	pwm_reg64_t *r = (pwm_reg64_t*)(SOC_PWM_REG_BASE + (0x64 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg64_value(void) {
	pwm_reg64_t *r = (pwm_reg64_t*)(SOC_PWM_REG_BASE + (0x64 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg64_ccr4_shad(void) {
	pwm_reg64_t *r = (pwm_reg64_t*)(SOC_PWM_REG_BASE + (0x64 << 2));
	return r->ccr4_shad;
}

//reg reg65:

static inline void pwm_ll_set_reg65_value(uint32_t v) {
	pwm_reg65_t *r = (pwm_reg65_t*)(SOC_PWM_REG_BASE + (0x65 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg65_value(void) {
	pwm_reg65_t *r = (pwm_reg65_t*)(SOC_PWM_REG_BASE + (0x65 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg65_ccr5_shad(void) {
	pwm_reg65_t *r = (pwm_reg65_t*)(SOC_PWM_REG_BASE + (0x65 << 2));
	return r->ccr5_shad;
}

//reg reg66:

static inline void pwm_ll_set_reg66_value(uint32_t v) {
	pwm_reg66_t *r = (pwm_reg66_t*)(SOC_PWM_REG_BASE + (0x66 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg66_value(void) {
	pwm_reg66_t *r = (pwm_reg66_t*)(SOC_PWM_REG_BASE + (0x66 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg66_ccr6_shad(void) {
	pwm_reg66_t *r = (pwm_reg66_t*)(SOC_PWM_REG_BASE + (0x66 << 2));
	return r->ccr6_shad;
}

//reg reg67:

static inline void pwm_ll_set_reg67_value(uint32_t v) {
	pwm_reg67_t *r = (pwm_reg67_t*)(SOC_PWM_REG_BASE + (0x67 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg67_value(void) {
	pwm_reg67_t *r = (pwm_reg67_t*)(SOC_PWM_REG_BASE + (0x67 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg67_ccr7_shad(void) {
	pwm_reg67_t *r = (pwm_reg67_t*)(SOC_PWM_REG_BASE + (0x67 << 2));
	return r->ccr7_shad;
}

//reg reg68:

static inline void pwm_ll_set_reg68_value(uint32_t v) {
	pwm_reg68_t *r = (pwm_reg68_t*)(SOC_PWM_REG_BASE + (0x68 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg68_value(void) {
	pwm_reg68_t *r = (pwm_reg68_t*)(SOC_PWM_REG_BASE + (0x68 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg68_ccr8_shad(void) {
	pwm_reg68_t *r = (pwm_reg68_t*)(SOC_PWM_REG_BASE + (0x68 << 2));
	return r->ccr8_shad;
}

//reg reg69:

static inline void pwm_ll_set_reg69_value(uint32_t v) {
	pwm_reg69_t *r = (pwm_reg69_t*)(SOC_PWM_REG_BASE + (0x69 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg69_value(void) {
	pwm_reg69_t *r = (pwm_reg69_t*)(SOC_PWM_REG_BASE + (0x69 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg69_ccr9_shad(void) {
	pwm_reg69_t *r = (pwm_reg69_t*)(SOC_PWM_REG_BASE + (0x69 << 2));
	return r->ccr9_shad;
}

//reg reg6a:

static inline void pwm_ll_set_reg6a_value(uint32_t v) {
	pwm_reg6a_t *r = (pwm_reg6a_t*)(SOC_PWM_REG_BASE + (0x6a << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg6a_value(void) {
	pwm_reg6a_t *r = (pwm_reg6a_t*)(SOC_PWM_REG_BASE + (0x6a << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg6a_ch6e_shad(void) {
	pwm_reg6a_t *r = (pwm_reg6a_t*)(SOC_PWM_REG_BASE + (0x6a << 2));
	return r->ch6e_shad;
}

static inline uint32_t pwm_ll_get_reg6a_ch5e_shad(void) {
	pwm_reg6a_t *r = (pwm_reg6a_t*)(SOC_PWM_REG_BASE + (0x6a << 2));
	return r->ch5e_shad;
}

static inline uint32_t pwm_ll_get_reg6a_ch4e_shad(void) {
	pwm_reg6a_t *r = (pwm_reg6a_t*)(SOC_PWM_REG_BASE + (0x6a << 2));
	return r->ch4e_shad;
}

static inline uint32_t pwm_ll_get_reg6a_ch3e_shad(void) {
	pwm_reg6a_t *r = (pwm_reg6a_t*)(SOC_PWM_REG_BASE + (0x6a << 2));
	return r->ch3e_shad;
}

static inline uint32_t pwm_ll_get_reg6a_ch2e_shad(void) {
	pwm_reg6a_t *r = (pwm_reg6a_t*)(SOC_PWM_REG_BASE + (0x6a << 2));
	return r->ch2e_shad;
}

static inline uint32_t pwm_ll_get_reg6a_ch1e_shad(void) {
	pwm_reg6a_t *r = (pwm_reg6a_t*)(SOC_PWM_REG_BASE + (0x6a << 2));
	return r->ch1e_shad;
}

static inline uint32_t pwm_ll_get_reg6a_oc1m_shad(void) {
	pwm_reg6a_t *r = (pwm_reg6a_t*)(SOC_PWM_REG_BASE + (0x6a << 2));
	return r->oc1m_shad;
}

static inline uint32_t pwm_ll_get_reg6a_oc2m_shad(void) {
	pwm_reg6a_t *r = (pwm_reg6a_t*)(SOC_PWM_REG_BASE + (0x6a << 2));
	return r->oc2m_shad;
}

static inline uint32_t pwm_ll_get_reg6a_oc3m_shad(void) {
	pwm_reg6a_t *r = (pwm_reg6a_t*)(SOC_PWM_REG_BASE + (0x6a << 2));
	return r->oc3m_shad;
}

static inline uint32_t pwm_ll_get_reg6a_reserved_15_31(void) {
	pwm_reg6a_t *r = (pwm_reg6a_t*)(SOC_PWM_REG_BASE + (0x6a << 2));
	return r->reserved_15_31;
}

//reg reg6b:

static inline void pwm_ll_set_reg6b_value(uint32_t v) {
	pwm_reg6b_t *r = (pwm_reg6b_t*)(SOC_PWM_REG_BASE + (0x6b << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg6b_value(void) {
	pwm_reg6b_t *r = (pwm_reg6b_t*)(SOC_PWM_REG_BASE + (0x6b << 2));
	return r->v;
}

static inline void pwm_ll_set_reg6b_tim1_fade_num(uint32_t v) {
	pwm_reg6b_t *r = (pwm_reg6b_t*)(SOC_PWM_REG_BASE + (0x6b << 2));
	r->tim1_fade_num = v;
}

static inline uint32_t pwm_ll_get_reg6b_tim1_fade_num(void) {
	pwm_reg6b_t *r = (pwm_reg6b_t*)(SOC_PWM_REG_BASE + (0x6b << 2));
	return r->tim1_fade_num;
}

static inline void pwm_ll_set_reg6b_tim1_fade_intval_cyc(uint32_t v) {
	pwm_reg6b_t *r = (pwm_reg6b_t*)(SOC_PWM_REG_BASE + (0x6b << 2));
	r->tim1_fade_intval_cyc = v;
}

static inline uint32_t pwm_ll_get_reg6b_tim1_fade_intval_cyc(void) {
	pwm_reg6b_t *r = (pwm_reg6b_t*)(SOC_PWM_REG_BASE + (0x6b << 2));
	return r->tim1_fade_intval_cyc;
}

static inline void pwm_ll_set_reg6b_tim1_fade_scale(uint32_t v) {
	pwm_reg6b_t *r = (pwm_reg6b_t*)(SOC_PWM_REG_BASE + (0x6b << 2));
	r->tim1_fade_scale = v;
}

static inline uint32_t pwm_ll_get_reg6b_tim1_fade_scale(void) {
	pwm_reg6b_t *r = (pwm_reg6b_t*)(SOC_PWM_REG_BASE + (0x6b << 2));
	return r->tim1_fade_scale;
}

static inline void pwm_ll_set_reg6b_tim1_fade_end_if(uint32_t v) {
	pwm_reg6b_t *r = (pwm_reg6b_t*)(SOC_PWM_REG_BASE + (0x6b << 2));
	r->tim1_fade_end_if = v;
}

static inline uint32_t pwm_ll_get_reg6b_tim1_fade_end_if(void) {
	pwm_reg6b_t *r = (pwm_reg6b_t*)(SOC_PWM_REG_BASE + (0x6b << 2));
	return r->tim1_fade_end_if;
}

static inline void pwm_ll_set_reg6b_tim1_fade_end_int_en(uint32_t v) {
	pwm_reg6b_t *r = (pwm_reg6b_t*)(SOC_PWM_REG_BASE + (0x6b << 2));
	r->tim1_fade_end_int_en = v;
}

static inline uint32_t pwm_ll_get_reg6b_tim1_fade_end_int_en(void) {
	pwm_reg6b_t *r = (pwm_reg6b_t*)(SOC_PWM_REG_BASE + (0x6b << 2));
	return r->tim1_fade_end_int_en;
}

static inline void pwm_ll_set_reg6b_tim1_fade_inc_dec(uint32_t v) {
	pwm_reg6b_t *r = (pwm_reg6b_t*)(SOC_PWM_REG_BASE + (0x6b << 2));
	r->tim1_fade_inc_dec = v;
}

static inline uint32_t pwm_ll_get_reg6b_tim1_fade_inc_dec(void) {
	pwm_reg6b_t *r = (pwm_reg6b_t*)(SOC_PWM_REG_BASE + (0x6b << 2));
	return r->tim1_fade_inc_dec;
}

static inline void pwm_ll_set_reg6b_tim1_duty_fading_en(uint32_t v) {
	pwm_reg6b_t *r = (pwm_reg6b_t*)(SOC_PWM_REG_BASE + (0x6b << 2));
	r->tim1_duty_fading_en = v;
}

static inline uint32_t pwm_ll_get_reg6b_tim1_duty_fading_en(void) {
	pwm_reg6b_t *r = (pwm_reg6b_t*)(SOC_PWM_REG_BASE + (0x6b << 2));
	return r->tim1_duty_fading_en;
}

//reg reg6c:

static inline void pwm_ll_set_reg6c_value(uint32_t v) {
	pwm_reg6c_t *r = (pwm_reg6c_t*)(SOC_PWM_REG_BASE + (0x6c << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg6c_value(void) {
	pwm_reg6c_t *r = (pwm_reg6c_t*)(SOC_PWM_REG_BASE + (0x6c << 2));
	return r->v;
}

static inline void pwm_ll_set_reg6c_tim2_fade_num(uint32_t v) {
	pwm_reg6c_t *r = (pwm_reg6c_t*)(SOC_PWM_REG_BASE + (0x6c << 2));
	r->tim2_fade_num = v;
}

static inline uint32_t pwm_ll_get_reg6c_tim2_fade_num(void) {
	pwm_reg6c_t *r = (pwm_reg6c_t*)(SOC_PWM_REG_BASE + (0x6c << 2));
	return r->tim2_fade_num;
}

static inline void pwm_ll_set_reg6c_tim2_fade_intval_cyc(uint32_t v) {
	pwm_reg6c_t *r = (pwm_reg6c_t*)(SOC_PWM_REG_BASE + (0x6c << 2));
	r->tim2_fade_intval_cyc = v;
}

static inline uint32_t pwm_ll_get_reg6c_tim2_fade_intval_cyc(void) {
	pwm_reg6c_t *r = (pwm_reg6c_t*)(SOC_PWM_REG_BASE + (0x6c << 2));
	return r->tim2_fade_intval_cyc;
}

static inline void pwm_ll_set_reg6c_tim2_fade_scale(uint32_t v) {
	pwm_reg6c_t *r = (pwm_reg6c_t*)(SOC_PWM_REG_BASE + (0x6c << 2));
	r->tim2_fade_scale = v;
}

static inline uint32_t pwm_ll_get_reg6c_tim2_fade_scale(void) {
	pwm_reg6c_t *r = (pwm_reg6c_t*)(SOC_PWM_REG_BASE + (0x6c << 2));
	return r->tim2_fade_scale;
}

static inline void pwm_ll_set_reg6c_tim2_fade_end_if(uint32_t v) {
	pwm_reg6c_t *r = (pwm_reg6c_t*)(SOC_PWM_REG_BASE + (0x6c << 2));
	r->tim2_fade_end_if = v;
}

static inline uint32_t pwm_ll_get_reg6c_tim2_fade_end_if(void) {
	pwm_reg6c_t *r = (pwm_reg6c_t*)(SOC_PWM_REG_BASE + (0x6c << 2));
	return r->tim2_fade_end_if;
}

static inline void pwm_ll_set_reg6c_tim2_fade_end_int_en(uint32_t v) {
	pwm_reg6c_t *r = (pwm_reg6c_t*)(SOC_PWM_REG_BASE + (0x6c << 2));
	r->tim2_fade_end_int_en = v;
}

static inline uint32_t pwm_ll_get_reg6c_tim2_fade_end_int_en(void) {
	pwm_reg6c_t *r = (pwm_reg6c_t*)(SOC_PWM_REG_BASE + (0x6c << 2));
	return r->tim2_fade_end_int_en;
}

static inline void pwm_ll_set_reg6c_tim2_fade_inc_dec(uint32_t v) {
	pwm_reg6c_t *r = (pwm_reg6c_t*)(SOC_PWM_REG_BASE + (0x6c << 2));
	r->tim2_fade_inc_dec = v;
}

static inline uint32_t pwm_ll_get_reg6c_tim2_fade_inc_dec(void) {
	pwm_reg6c_t *r = (pwm_reg6c_t*)(SOC_PWM_REG_BASE + (0x6c << 2));
	return r->tim2_fade_inc_dec;
}

static inline void pwm_ll_set_reg6c_tim2_duty_fading_en(uint32_t v) {
	pwm_reg6c_t *r = (pwm_reg6c_t*)(SOC_PWM_REG_BASE + (0x6c << 2));
	r->tim2_duty_fading_en = v;
}

static inline uint32_t pwm_ll_get_reg6c_tim2_duty_fading_en(void) {
	pwm_reg6c_t *r = (pwm_reg6c_t*)(SOC_PWM_REG_BASE + (0x6c << 2));
	return r->tim2_duty_fading_en;
}

//reg reg6d:

static inline void pwm_ll_set_reg6d_value(uint32_t v) {
	pwm_reg6d_t *r = (pwm_reg6d_t*)(SOC_PWM_REG_BASE + (0x6d << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg6d_value(void) {
	pwm_reg6d_t *r = (pwm_reg6d_t*)(SOC_PWM_REG_BASE + (0x6d << 2));
	return r->v;
}

static inline void pwm_ll_set_reg6d_tim3_fade_num(uint32_t v) {
	pwm_reg6d_t *r = (pwm_reg6d_t*)(SOC_PWM_REG_BASE + (0x6d << 2));
	r->tim3_fade_num = v;
}

static inline uint32_t pwm_ll_get_reg6d_tim3_fade_num(void) {
	pwm_reg6d_t *r = (pwm_reg6d_t*)(SOC_PWM_REG_BASE + (0x6d << 2));
	return r->tim3_fade_num;
}

static inline void pwm_ll_set_reg6d_tim3_fade_intval_cyc(uint32_t v) {
	pwm_reg6d_t *r = (pwm_reg6d_t*)(SOC_PWM_REG_BASE + (0x6d << 2));
	r->tim3_fade_intval_cyc = v;
}

static inline uint32_t pwm_ll_get_reg6d_tim3_fade_intval_cyc(void) {
	pwm_reg6d_t *r = (pwm_reg6d_t*)(SOC_PWM_REG_BASE + (0x6d << 2));
	return r->tim3_fade_intval_cyc;
}

static inline void pwm_ll_set_reg6d_tim3_fade_scale(uint32_t v) {
	pwm_reg6d_t *r = (pwm_reg6d_t*)(SOC_PWM_REG_BASE + (0x6d << 2));
	r->tim3_fade_scale = v;
}

static inline uint32_t pwm_ll_get_reg6d_tim3_fade_scale(void) {
	pwm_reg6d_t *r = (pwm_reg6d_t*)(SOC_PWM_REG_BASE + (0x6d << 2));
	return r->tim3_fade_scale;
}

static inline void pwm_ll_set_reg6d_tim3_fade_end_if(uint32_t v) {
	pwm_reg6d_t *r = (pwm_reg6d_t*)(SOC_PWM_REG_BASE + (0x6d << 2));
	r->tim3_fade_end_if = v;
}

static inline uint32_t pwm_ll_get_reg6d_tim3_fade_end_if(void) {
	pwm_reg6d_t *r = (pwm_reg6d_t*)(SOC_PWM_REG_BASE + (0x6d << 2));
	return r->tim3_fade_end_if;
}

static inline void pwm_ll_set_reg6d_tim3_fade_end_int_en(uint32_t v) {
	pwm_reg6d_t *r = (pwm_reg6d_t*)(SOC_PWM_REG_BASE + (0x6d << 2));
	r->tim3_fade_end_int_en = v;
}

static inline uint32_t pwm_ll_get_reg6d_tim3_fade_end_int_en(void) {
	pwm_reg6d_t *r = (pwm_reg6d_t*)(SOC_PWM_REG_BASE + (0x6d << 2));
	return r->tim3_fade_end_int_en;
}

static inline void pwm_ll_set_reg6d_tim3_fade_inc_dec(uint32_t v) {
	pwm_reg6d_t *r = (pwm_reg6d_t*)(SOC_PWM_REG_BASE + (0x6d << 2));
	r->tim3_fade_inc_dec = v;
}

static inline uint32_t pwm_ll_get_reg6d_tim3_fade_inc_dec(void) {
	pwm_reg6d_t *r = (pwm_reg6d_t*)(SOC_PWM_REG_BASE + (0x6d << 2));
	return r->tim3_fade_inc_dec;
}

static inline void pwm_ll_set_reg6d_tim3_duty_fading_en(uint32_t v) {
	pwm_reg6d_t *r = (pwm_reg6d_t*)(SOC_PWM_REG_BASE + (0x6d << 2));
	r->tim3_duty_fading_en = v;
}

static inline uint32_t pwm_ll_get_reg6d_tim3_duty_fading_en(void) {
	pwm_reg6d_t *r = (pwm_reg6d_t*)(SOC_PWM_REG_BASE + (0x6d << 2));
	return r->tim3_duty_fading_en;
}

//reg reg80:

static inline void pwm_ll_set_reg80_value(uint32_t v) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg80_value(void) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg80_ocm3pe(uint32_t v) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	r->ocm3pe = v;
}

static inline uint32_t pwm_ll_get_reg80_ocm3pe(void) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	return r->ocm3pe;
}

static inline void pwm_ll_set_reg80_ocm2pe(uint32_t v) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	r->ocm2pe = v;
}

static inline uint32_t pwm_ll_get_reg80_ocm2pe(void) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	return r->ocm2pe;
}

static inline void pwm_ll_set_reg80_ocm1pe(uint32_t v) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	r->ocm1pe = v;
}

static inline uint32_t pwm_ll_get_reg80_ocm1pe(void) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	return r->ocm1pe;
}

static inline void pwm_ll_set_reg80_arpe3(uint32_t v) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	r->arpe3 = v;
}

static inline uint32_t pwm_ll_get_reg80_arpe3(void) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	return r->arpe3;
}

static inline void pwm_ll_set_reg80_arpe2(uint32_t v) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	r->arpe2 = v;
}

static inline uint32_t pwm_ll_get_reg80_arpe2(void) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	return r->arpe2;
}

static inline void pwm_ll_set_reg80_arpe1(uint32_t v) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	r->arpe1 = v;
}

static inline uint32_t pwm_ll_get_reg80_arpe1(void) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	return r->arpe1;
}

static inline void pwm_ll_set_reg80_oc3pe(uint32_t v) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	r->oc3pe = v;
}

static inline uint32_t pwm_ll_get_reg80_oc3pe(void) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	return r->oc3pe;
}

static inline void pwm_ll_set_reg80_oc2pe(uint32_t v) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	r->oc2pe = v;
}

static inline uint32_t pwm_ll_get_reg80_oc2pe(void) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	return r->oc2pe;
}

static inline void pwm_ll_set_reg80_oc1pe(uint32_t v) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	r->oc1pe = v;
}

static inline uint32_t pwm_ll_get_reg80_oc1pe(void) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	return r->oc1pe;
}

static inline void pwm_ll_set_reg80_reserved_9_17(uint32_t v) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	r->reserved_9_17 = v;
}

static inline uint32_t pwm_ll_get_reg80_reserved_9_17(void) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	return r->reserved_9_17;
}

static inline void pwm_ll_set_reg80_cms_tim3(uint32_t v) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	r->cms_tim3 = v;
}

static inline uint32_t pwm_ll_get_reg80_cms_tim3(void) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	return r->cms_tim3;
}

static inline void pwm_ll_set_reg80_cms_tim2(uint32_t v) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	r->cms_tim2 = v;
}

static inline uint32_t pwm_ll_get_reg80_cms_tim2(void) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	return r->cms_tim2;
}

static inline void pwm_ll_set_reg80_cms_tim1(uint32_t v) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	r->cms_tim1 = v;
}

static inline uint32_t pwm_ll_get_reg80_cms_tim1(void) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	return r->cms_tim1;
}

static inline void pwm_ll_set_reg80_reserved_24_25(uint32_t v) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	r->reserved_24_25 = v;
}

static inline uint32_t pwm_ll_get_reg80_reserved_24_25(void) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	return r->reserved_24_25;
}

static inline void pwm_ll_set_reg80_urs3(uint32_t v) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	r->urs3 = v;
}

static inline uint32_t pwm_ll_get_reg80_urs3(void) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	return r->urs3;
}

static inline void pwm_ll_set_reg80_urs2(uint32_t v) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	r->urs2 = v;
}

static inline uint32_t pwm_ll_get_reg80_urs2(void) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	return r->urs2;
}

static inline void pwm_ll_set_reg80_urs1(uint32_t v) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	r->urs1 = v;
}

static inline uint32_t pwm_ll_get_reg80_urs1(void) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	return r->urs1;
}

static inline void pwm_ll_set_reg80_dir_tim3(uint32_t v) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	r->dir_tim3 = v;
}

static inline uint32_t pwm_ll_get_reg80_dir_tim3(void) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	return r->dir_tim3;
}

static inline void pwm_ll_set_reg80_dir_tim2(uint32_t v) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	r->dir_tim2 = v;
}

static inline uint32_t pwm_ll_get_reg80_dir_tim2(void) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	return r->dir_tim2;
}

static inline void pwm_ll_set_reg80_dir_tim1(uint32_t v) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	r->dir_tim1 = v;
}

static inline uint32_t pwm_ll_get_reg80_dir_tim1(void) {
	pwm_reg80_t *r = (pwm_reg80_t*)(SOC_PWM_REG_BASE + (0x80 << 2));
	return r->dir_tim1;
}

//reg reg81:

static inline void pwm_ll_set_reg81_value(uint32_t v) {
	pwm_reg81_t *r = (pwm_reg81_t*)(SOC_PWM_REG_BASE + (0x81 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg81_value(void) {
	pwm_reg81_t *r = (pwm_reg81_t*)(SOC_PWM_REG_BASE + (0x81 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg81_reserved_0_7(uint32_t v) {
	pwm_reg81_t *r = (pwm_reg81_t*)(SOC_PWM_REG_BASE + (0x81 << 2));
	r->reserved_0_7 = v;
}

static inline uint32_t pwm_ll_get_reg81_reserved_0_7(void) {
	pwm_reg81_t *r = (pwm_reg81_t*)(SOC_PWM_REG_BASE + (0x81 << 2));
	return r->reserved_0_7;
}

static inline void pwm_ll_set_reg81_com_mode(uint32_t v) {
	pwm_reg81_t *r = (pwm_reg81_t*)(SOC_PWM_REG_BASE + (0x81 << 2));
	r->com_mode = v;
}

static inline uint32_t pwm_ll_get_reg81_com_mode(void) {
	pwm_reg81_t *r = (pwm_reg81_t*)(SOC_PWM_REG_BASE + (0x81 << 2));
	return r->com_mode;
}

static inline void pwm_ll_set_reg81_ccpc(uint32_t v) {
	pwm_reg81_t *r = (pwm_reg81_t*)(SOC_PWM_REG_BASE + (0x81 << 2));
	r->ccpc = v;
}

static inline uint32_t pwm_ll_get_reg81_ccpc(void) {
	pwm_reg81_t *r = (pwm_reg81_t*)(SOC_PWM_REG_BASE + (0x81 << 2));
	return r->ccpc;
}

static inline void pwm_ll_set_reg81_trios3(uint32_t v) {
	pwm_reg81_t *r = (pwm_reg81_t*)(SOC_PWM_REG_BASE + (0x81 << 2));
	r->trios3 = v;
}

static inline uint32_t pwm_ll_get_reg81_trios3(void) {
	pwm_reg81_t *r = (pwm_reg81_t*)(SOC_PWM_REG_BASE + (0x81 << 2));
	return r->trios3;
}

static inline void pwm_ll_set_reg81_adcs3(uint32_t v) {
	pwm_reg81_t *r = (pwm_reg81_t*)(SOC_PWM_REG_BASE + (0x81 << 2));
	r->adcs3 = v;
}

static inline uint32_t pwm_ll_get_reg81_adcs3(void) {
	pwm_reg81_t *r = (pwm_reg81_t*)(SOC_PWM_REG_BASE + (0x81 << 2));
	return r->adcs3;
}

static inline void pwm_ll_set_reg81_reserved_16_16(uint32_t v) {
	pwm_reg81_t *r = (pwm_reg81_t*)(SOC_PWM_REG_BASE + (0x81 << 2));
	r->reserved_16_16 = v;
}

static inline uint32_t pwm_ll_get_reg81_reserved_16_16(void) {
	pwm_reg81_t *r = (pwm_reg81_t*)(SOC_PWM_REG_BASE + (0x81 << 2));
	return r->reserved_16_16;
}

static inline void pwm_ll_set_reg81_ccus(uint32_t v) {
	pwm_reg81_t *r = (pwm_reg81_t*)(SOC_PWM_REG_BASE + (0x81 << 2));
	r->ccus = v;
}

static inline uint32_t pwm_ll_get_reg81_ccus(void) {
	pwm_reg81_t *r = (pwm_reg81_t*)(SOC_PWM_REG_BASE + (0x81 << 2));
	return r->ccus;
}

static inline void pwm_ll_set_reg81_trios2(uint32_t v) {
	pwm_reg81_t *r = (pwm_reg81_t*)(SOC_PWM_REG_BASE + (0x81 << 2));
	r->trios2 = v;
}

static inline uint32_t pwm_ll_get_reg81_trios2(void) {
	pwm_reg81_t *r = (pwm_reg81_t*)(SOC_PWM_REG_BASE + (0x81 << 2));
	return r->trios2;
}

static inline void pwm_ll_set_reg81_adcs2(uint32_t v) {
	pwm_reg81_t *r = (pwm_reg81_t*)(SOC_PWM_REG_BASE + (0x81 << 2));
	r->adcs2 = v;
}

static inline uint32_t pwm_ll_get_reg81_adcs2(void) {
	pwm_reg81_t *r = (pwm_reg81_t*)(SOC_PWM_REG_BASE + (0x81 << 2));
	return r->adcs2;
}

static inline void pwm_ll_set_reg81_reserved_24_25(uint32_t v) {
	pwm_reg81_t *r = (pwm_reg81_t*)(SOC_PWM_REG_BASE + (0x81 << 2));
	r->reserved_24_25 = v;
}

static inline uint32_t pwm_ll_get_reg81_reserved_24_25(void) {
	pwm_reg81_t *r = (pwm_reg81_t*)(SOC_PWM_REG_BASE + (0x81 << 2));
	return r->reserved_24_25;
}

static inline void pwm_ll_set_reg81_trios1(uint32_t v) {
	pwm_reg81_t *r = (pwm_reg81_t*)(SOC_PWM_REG_BASE + (0x81 << 2));
	r->trios1 = v;
}

static inline uint32_t pwm_ll_get_reg81_trios1(void) {
	pwm_reg81_t *r = (pwm_reg81_t*)(SOC_PWM_REG_BASE + (0x81 << 2));
	return r->trios1;
}

static inline void pwm_ll_set_reg81_adcs1(uint32_t v) {
	pwm_reg81_t *r = (pwm_reg81_t*)(SOC_PWM_REG_BASE + (0x81 << 2));
	r->adcs1 = v;
}

static inline uint32_t pwm_ll_get_reg81_adcs1(void) {
	pwm_reg81_t *r = (pwm_reg81_t*)(SOC_PWM_REG_BASE + (0x81 << 2));
	return r->adcs1;
}

//reg reg82:

static inline void pwm_ll_set_reg82_value(uint32_t v) {
	pwm_reg82_t *r = (pwm_reg82_t*)(SOC_PWM_REG_BASE + (0x82 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg82_value(void) {
	pwm_reg82_t *r = (pwm_reg82_t*)(SOC_PWM_REG_BASE + (0x82 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg82_sms1(uint32_t v) {
	pwm_reg82_t *r = (pwm_reg82_t*)(SOC_PWM_REG_BASE + (0x82 << 2));
	r->sms1 = v;
}

static inline uint32_t pwm_ll_get_reg82_sms1(void) {
	pwm_reg82_t *r = (pwm_reg82_t*)(SOC_PWM_REG_BASE + (0x82 << 2));
	return r->sms1;
}

static inline void pwm_ll_set_reg82_sms2(uint32_t v) {
	pwm_reg82_t *r = (pwm_reg82_t*)(SOC_PWM_REG_BASE + (0x82 << 2));
	r->sms2 = v;
}

static inline uint32_t pwm_ll_get_reg82_sms2(void) {
	pwm_reg82_t *r = (pwm_reg82_t*)(SOC_PWM_REG_BASE + (0x82 << 2));
	return r->sms2;
}

static inline void pwm_ll_set_reg82_sms3(uint32_t v) {
	pwm_reg82_t *r = (pwm_reg82_t*)(SOC_PWM_REG_BASE + (0x82 << 2));
	r->sms3 = v;
}

static inline uint32_t pwm_ll_get_reg82_sms3(void) {
	pwm_reg82_t *r = (pwm_reg82_t*)(SOC_PWM_REG_BASE + (0x82 << 2));
	return r->sms3;
}

static inline void pwm_ll_set_reg82_ts1(uint32_t v) {
	pwm_reg82_t *r = (pwm_reg82_t*)(SOC_PWM_REG_BASE + (0x82 << 2));
	r->ts1 = v;
}

static inline uint32_t pwm_ll_get_reg82_ts1(void) {
	pwm_reg82_t *r = (pwm_reg82_t*)(SOC_PWM_REG_BASE + (0x82 << 2));
	return r->ts1;
}

static inline void pwm_ll_set_reg82_reserved_15_15(uint32_t v) {
	pwm_reg82_t *r = (pwm_reg82_t*)(SOC_PWM_REG_BASE + (0x82 << 2));
	r->reserved_15_15 = v;
}

static inline uint32_t pwm_ll_get_reg82_reserved_15_15(void) {
	pwm_reg82_t *r = (pwm_reg82_t*)(SOC_PWM_REG_BASE + (0x82 << 2));
	return r->reserved_15_15;
}

static inline void pwm_ll_set_reg82_ts2(uint32_t v) {
	pwm_reg82_t *r = (pwm_reg82_t*)(SOC_PWM_REG_BASE + (0x82 << 2));
	r->ts2 = v;
}

static inline uint32_t pwm_ll_get_reg82_ts2(void) {
	pwm_reg82_t *r = (pwm_reg82_t*)(SOC_PWM_REG_BASE + (0x82 << 2));
	return r->ts2;
}

static inline void pwm_ll_set_reg82_ts3(uint32_t v) {
	pwm_reg82_t *r = (pwm_reg82_t*)(SOC_PWM_REG_BASE + (0x82 << 2));
	r->ts3 = v;
}

static inline uint32_t pwm_ll_get_reg82_ts3(void) {
	pwm_reg82_t *r = (pwm_reg82_t*)(SOC_PWM_REG_BASE + (0x82 << 2));
	return r->ts3;
}

static inline void pwm_ll_set_reg82_reserved_22_31(uint32_t v) {
	pwm_reg82_t *r = (pwm_reg82_t*)(SOC_PWM_REG_BASE + (0x82 << 2));
	r->reserved_22_31 = v;
}

static inline uint32_t pwm_ll_get_reg82_reserved_22_31(void) {
	pwm_reg82_t *r = (pwm_reg82_t*)(SOC_PWM_REG_BASE + (0x82 << 2));
	return r->reserved_22_31;
}

//reg reg83:

static inline void pwm_ll_set_reg83_value(uint32_t v) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg83_value(void) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg83_cc1ie(uint32_t v) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	r->cc1ie = v;
}

static inline uint32_t pwm_ll_get_reg83_cc1ie(void) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	return r->cc1ie;
}

static inline void pwm_ll_set_reg83_cc2ie(uint32_t v) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	r->cc2ie = v;
}

static inline uint32_t pwm_ll_get_reg83_cc2ie(void) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	return r->cc2ie;
}

static inline void pwm_ll_set_reg83_cc3ie(uint32_t v) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	r->cc3ie = v;
}

static inline uint32_t pwm_ll_get_reg83_cc3ie(void) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	return r->cc3ie;
}

static inline void pwm_ll_set_reg83_cc4ie(uint32_t v) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	r->cc4ie = v;
}

static inline uint32_t pwm_ll_get_reg83_cc4ie(void) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	return r->cc4ie;
}

static inline void pwm_ll_set_reg83_cc5ie(uint32_t v) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	r->cc5ie = v;
}

static inline uint32_t pwm_ll_get_reg83_cc5ie(void) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	return r->cc5ie;
}

static inline void pwm_ll_set_reg83_cc6ie(uint32_t v) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	r->cc6ie = v;
}

static inline uint32_t pwm_ll_get_reg83_cc6ie(void) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	return r->cc6ie;
}

static inline void pwm_ll_set_reg83_cc7ie(uint32_t v) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	r->cc7ie = v;
}

static inline uint32_t pwm_ll_get_reg83_cc7ie(void) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	return r->cc7ie;
}

static inline void pwm_ll_set_reg83_cc8ie(uint32_t v) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	r->cc8ie = v;
}

static inline uint32_t pwm_ll_get_reg83_cc8ie(void) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	return r->cc8ie;
}

static inline void pwm_ll_set_reg83_cc9ie(uint32_t v) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	r->cc9ie = v;
}

static inline uint32_t pwm_ll_get_reg83_cc9ie(void) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	return r->cc9ie;
}

static inline void pwm_ll_set_reg83_uie1(uint32_t v) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	r->uie1 = v;
}

static inline uint32_t pwm_ll_get_reg83_uie1(void) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	return r->uie1;
}

static inline void pwm_ll_set_reg83_uie2(uint32_t v) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	r->uie2 = v;
}

static inline uint32_t pwm_ll_get_reg83_uie2(void) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	return r->uie2;
}

static inline void pwm_ll_set_reg83_uie3(uint32_t v) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	r->uie3 = v;
}

static inline uint32_t pwm_ll_get_reg83_uie3(void) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	return r->uie3;
}

static inline void pwm_ll_set_reg83_tie1(uint32_t v) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	r->tie1 = v;
}

static inline uint32_t pwm_ll_get_reg83_tie1(void) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	return r->tie1;
}

static inline void pwm_ll_set_reg83_tie2(uint32_t v) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	r->tie2 = v;
}

static inline uint32_t pwm_ll_get_reg83_tie2(void) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	return r->tie2;
}

static inline void pwm_ll_set_reg83_tie3(uint32_t v) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	r->tie3 = v;
}

static inline uint32_t pwm_ll_get_reg83_tie3(void) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	return r->tie3;
}

static inline void pwm_ll_set_reg83_comie(uint32_t v) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	r->comie = v;
}

static inline uint32_t pwm_ll_get_reg83_comie(void) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	return r->comie;
}

static inline void pwm_ll_set_reg83_reserved_16_19(uint32_t v) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	r->reserved_16_19 = v;
}

static inline uint32_t pwm_ll_get_reg83_reserved_16_19(void) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	return r->reserved_16_19;
}

static inline void pwm_ll_set_reg83_df_tim1(uint32_t v) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	r->df_tim1 = v;
}

static inline uint32_t pwm_ll_get_reg83_df_tim1(void) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	return r->df_tim1;
}

static inline void pwm_ll_set_reg83_df_tim2(uint32_t v) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	r->df_tim2 = v;
}

static inline uint32_t pwm_ll_get_reg83_df_tim2(void) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	return r->df_tim2;
}

static inline void pwm_ll_set_reg83_df_tim3(uint32_t v) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	r->df_tim3 = v;
}

static inline uint32_t pwm_ll_get_reg83_df_tim3(void) {
	pwm_reg83_t *r = (pwm_reg83_t*)(SOC_PWM_REG_BASE + (0x83 << 2));
	return r->df_tim3;
}

//reg reg84:

static inline void pwm_ll_set_reg84_value(uint32_t v) {
	pwm_reg84_t *r = (pwm_reg84_t*)(SOC_PWM_REG_BASE + (0x84 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg84_value(void) {
	pwm_reg84_t *r = (pwm_reg84_t*)(SOC_PWM_REG_BASE + (0x84 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg84_cc1if(uint32_t v) {
	pwm_reg84_t *r = (pwm_reg84_t*)(SOC_PWM_REG_BASE + (0x84 << 2));
	r->cc1if = v;
}

static inline void pwm_ll_set_reg84_cc2if(uint32_t v) {
	pwm_reg84_t *r = (pwm_reg84_t*)(SOC_PWM_REG_BASE + (0x84 << 2));
	r->cc2if = v;
}

static inline void pwm_ll_set_reg84_cc3if(uint32_t v) {
	pwm_reg84_t *r = (pwm_reg84_t*)(SOC_PWM_REG_BASE + (0x84 << 2));
	r->cc3if = v;
}

static inline void pwm_ll_set_reg84_cc4if(uint32_t v) {
	pwm_reg84_t *r = (pwm_reg84_t*)(SOC_PWM_REG_BASE + (0x84 << 2));
	r->cc4if = v;
}

static inline void pwm_ll_set_reg84_cc5if(uint32_t v) {
	pwm_reg84_t *r = (pwm_reg84_t*)(SOC_PWM_REG_BASE + (0x84 << 2));
	r->cc5if = v;
}

static inline void pwm_ll_set_reg84_cc6if(uint32_t v) {
	pwm_reg84_t *r = (pwm_reg84_t*)(SOC_PWM_REG_BASE + (0x84 << 2));
	r->cc6if = v;
}

static inline void pwm_ll_set_reg84_cc7if(uint32_t v) {
	pwm_reg84_t *r = (pwm_reg84_t*)(SOC_PWM_REG_BASE + (0x84 << 2));
	r->cc7if = v;
}

static inline void pwm_ll_set_reg84_cc8if(uint32_t v) {
	pwm_reg84_t *r = (pwm_reg84_t*)(SOC_PWM_REG_BASE + (0x84 << 2));
	r->cc8if = v;
}

static inline void pwm_ll_set_reg84_cc9if(uint32_t v) {
	pwm_reg84_t *r = (pwm_reg84_t*)(SOC_PWM_REG_BASE + (0x84 << 2));
	r->cc9if = v;
}

static inline void pwm_ll_set_reg84_uif1(uint32_t v) {
	pwm_reg84_t *r = (pwm_reg84_t*)(SOC_PWM_REG_BASE + (0x84 << 2));
	r->uif1 = v;
}

static inline void pwm_ll_set_reg84_uif2(uint32_t v) {
	pwm_reg84_t *r = (pwm_reg84_t*)(SOC_PWM_REG_BASE + (0x84 << 2));
	r->uif2 = v;
}

static inline void pwm_ll_set_reg84_uif3(uint32_t v) {
	pwm_reg84_t *r = (pwm_reg84_t*)(SOC_PWM_REG_BASE + (0x84 << 2));
	r->uif3 = v;
}

static inline void pwm_ll_set_reg84_tif1(uint32_t v) {
	pwm_reg84_t *r = (pwm_reg84_t*)(SOC_PWM_REG_BASE + (0x84 << 2));
	r->tif1 = v;
}

static inline void pwm_ll_set_reg84_tif2(uint32_t v) {
	pwm_reg84_t *r = (pwm_reg84_t*)(SOC_PWM_REG_BASE + (0x84 << 2));
	r->tif2 = v;
}

static inline void pwm_ll_set_reg84_tif3(uint32_t v) {
	pwm_reg84_t *r = (pwm_reg84_t*)(SOC_PWM_REG_BASE + (0x84 << 2));
	r->tif3 = v;
}

static inline void pwm_ll_set_reg84_comif(uint32_t v) {
	pwm_reg84_t *r = (pwm_reg84_t*)(SOC_PWM_REG_BASE + (0x84 << 2));
	r->comif = v;
}

static inline void pwm_ll_set_reg84_ch1eif(uint32_t v) {
	pwm_reg84_t *r = (pwm_reg84_t*)(SOC_PWM_REG_BASE + (0x84 << 2));
	r->ch1eif = v;
}

static inline uint32_t pwm_ll_get_reg84_ch1eif(void) {
	pwm_reg84_t *r = (pwm_reg84_t*)(SOC_PWM_REG_BASE + (0x84 << 2));
	return r->ch1eif;
}

static inline void pwm_ll_set_reg84_ch2eif(uint32_t v) {
	pwm_reg84_t *r = (pwm_reg84_t*)(SOC_PWM_REG_BASE + (0x84 << 2));
	r->ch2eif = v;
}

static inline uint32_t pwm_ll_get_reg84_ch2eif(void) {
	pwm_reg84_t *r = (pwm_reg84_t*)(SOC_PWM_REG_BASE + (0x84 << 2));
	return r->ch2eif;
}

static inline void pwm_ll_set_reg84_ch3eif(uint32_t v) {
	pwm_reg84_t *r = (pwm_reg84_t*)(SOC_PWM_REG_BASE + (0x84 << 2));
	r->ch3eif = v;
}

static inline uint32_t pwm_ll_get_reg84_ch3eif(void) {
	pwm_reg84_t *r = (pwm_reg84_t*)(SOC_PWM_REG_BASE + (0x84 << 2));
	return r->ch3eif;
}

static inline void pwm_ll_set_reg84_ch4eif(uint32_t v) {
	pwm_reg84_t *r = (pwm_reg84_t*)(SOC_PWM_REG_BASE + (0x84 << 2));
	r->ch4eif = v;
}

static inline uint32_t pwm_ll_get_reg84_ch4eif(void) {
	pwm_reg84_t *r = (pwm_reg84_t*)(SOC_PWM_REG_BASE + (0x84 << 2));
	return r->ch4eif;
}

static inline void pwm_ll_set_reg84_ch5eif(uint32_t v) {
	pwm_reg84_t *r = (pwm_reg84_t*)(SOC_PWM_REG_BASE + (0x84 << 2));
	r->ch5eif = v;
}

static inline uint32_t pwm_ll_get_reg84_ch5eif(void) {
	pwm_reg84_t *r = (pwm_reg84_t*)(SOC_PWM_REG_BASE + (0x84 << 2));
	return r->ch5eif;
}

static inline void pwm_ll_set_reg84_ch6eif(uint32_t v) {
	pwm_reg84_t *r = (pwm_reg84_t*)(SOC_PWM_REG_BASE + (0x84 << 2));
	r->ch6eif = v;
}

static inline uint32_t pwm_ll_get_reg84_ch6eif(void) {
	pwm_reg84_t *r = (pwm_reg84_t*)(SOC_PWM_REG_BASE + (0x84 << 2));
	return r->ch6eif;
}

static inline void pwm_ll_set_reg84_reserved_22_31(uint32_t v) {
	pwm_reg84_t *r = (pwm_reg84_t*)(SOC_PWM_REG_BASE + (0x84 << 2));
	r->reserved_22_31 = v;
}

static inline uint32_t pwm_ll_get_reg84_reserved_22_31(void) {
	pwm_reg84_t *r = (pwm_reg84_t*)(SOC_PWM_REG_BASE + (0x84 << 2));
	return r->reserved_22_31;
}

//reg reg85:

static inline void pwm_ll_set_reg85_value(uint32_t v) {
	pwm_reg85_t *r = (pwm_reg85_t*)(SOC_PWM_REG_BASE + (0x85 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg85_value(void) {
	pwm_reg85_t *r = (pwm_reg85_t*)(SOC_PWM_REG_BASE + (0x85 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg85_reserved_0_8(uint32_t v) {
	pwm_reg85_t *r = (pwm_reg85_t*)(SOC_PWM_REG_BASE + (0x85 << 2));
	r->reserved_0_8 = v;
}

static inline uint32_t pwm_ll_get_reg85_reserved_0_8(void) {
	pwm_reg85_t *r = (pwm_reg85_t*)(SOC_PWM_REG_BASE + (0x85 << 2));
	return r->reserved_0_8;
}

static inline void pwm_ll_set_reg85_ug1(uint32_t v) {
	pwm_reg85_t *r = (pwm_reg85_t*)(SOC_PWM_REG_BASE + (0x85 << 2));
	r->ug1 = v;
}

static inline void pwm_ll_set_reg85_ug2(uint32_t v) {
	pwm_reg85_t *r = (pwm_reg85_t*)(SOC_PWM_REG_BASE + (0x85 << 2));
	r->ug2 = v;
}

static inline void pwm_ll_set_reg85_ug3(uint32_t v) {
	pwm_reg85_t *r = (pwm_reg85_t*)(SOC_PWM_REG_BASE + (0x85 << 2));
	r->ug3 = v;
}

static inline void pwm_ll_set_reg85_tg1(uint32_t v) {
	pwm_reg85_t *r = (pwm_reg85_t*)(SOC_PWM_REG_BASE + (0x85 << 2));
	r->tg1 = v;
}

static inline void pwm_ll_set_reg85_tg2(uint32_t v) {
	pwm_reg85_t *r = (pwm_reg85_t*)(SOC_PWM_REG_BASE + (0x85 << 2));
	r->tg2 = v;
}

static inline void pwm_ll_set_reg85_tg3(uint32_t v) {
	pwm_reg85_t *r = (pwm_reg85_t*)(SOC_PWM_REG_BASE + (0x85 << 2));
	r->tg3 = v;
}

static inline void pwm_ll_set_reg85_comg(uint32_t v) {
	pwm_reg85_t *r = (pwm_reg85_t*)(SOC_PWM_REG_BASE + (0x85 << 2));
	r->comg = v;
}

static inline void pwm_ll_set_reg85_dtm1(uint32_t v) {
	pwm_reg85_t *r = (pwm_reg85_t*)(SOC_PWM_REG_BASE + (0x85 << 2));
	r->dtm1 = v;
}

static inline uint32_t pwm_ll_get_reg85_dtm1(void) {
	pwm_reg85_t *r = (pwm_reg85_t*)(SOC_PWM_REG_BASE + (0x85 << 2));
	return r->dtm1;
}

static inline void pwm_ll_set_reg85_dtm2(uint32_t v) {
	pwm_reg85_t *r = (pwm_reg85_t*)(SOC_PWM_REG_BASE + (0x85 << 2));
	r->dtm2 = v;
}

static inline uint32_t pwm_ll_get_reg85_dtm2(void) {
	pwm_reg85_t *r = (pwm_reg85_t*)(SOC_PWM_REG_BASE + (0x85 << 2));
	return r->dtm2;
}

static inline void pwm_ll_set_reg85_dtm3(uint32_t v) {
	pwm_reg85_t *r = (pwm_reg85_t*)(SOC_PWM_REG_BASE + (0x85 << 2));
	r->dtm3 = v;
}

static inline uint32_t pwm_ll_get_reg85_dtm3(void) {
	pwm_reg85_t *r = (pwm_reg85_t*)(SOC_PWM_REG_BASE + (0x85 << 2));
	return r->dtm3;
}

static inline void pwm_ll_set_reg85_init_level1(uint32_t v) {
	pwm_reg85_t *r = (pwm_reg85_t*)(SOC_PWM_REG_BASE + (0x85 << 2));
	r->init_level1 = v;
}

static inline uint32_t pwm_ll_get_reg85_init_level1(void) {
	pwm_reg85_t *r = (pwm_reg85_t*)(SOC_PWM_REG_BASE + (0x85 << 2));
	return r->init_level1;
}

static inline void pwm_ll_set_reg85_init_level2(uint32_t v) {
	pwm_reg85_t *r = (pwm_reg85_t*)(SOC_PWM_REG_BASE + (0x85 << 2));
	r->init_level2 = v;
}

static inline uint32_t pwm_ll_get_reg85_init_level2(void) {
	pwm_reg85_t *r = (pwm_reg85_t*)(SOC_PWM_REG_BASE + (0x85 << 2));
	return r->init_level2;
}

static inline void pwm_ll_set_reg85_init_level3(uint32_t v) {
	pwm_reg85_t *r = (pwm_reg85_t*)(SOC_PWM_REG_BASE + (0x85 << 2));
	r->init_level3 = v;
}

static inline uint32_t pwm_ll_get_reg85_init_level3(void) {
	pwm_reg85_t *r = (pwm_reg85_t*)(SOC_PWM_REG_BASE + (0x85 << 2));
	return r->init_level3;
}

static inline void pwm_ll_set_reg85_reserved_25_31(uint32_t v) {
	pwm_reg85_t *r = (pwm_reg85_t*)(SOC_PWM_REG_BASE + (0x85 << 2));
	r->reserved_25_31 = v;
}

static inline uint32_t pwm_ll_get_reg85_reserved_25_31(void) {
	pwm_reg85_t *r = (pwm_reg85_t*)(SOC_PWM_REG_BASE + (0x85 << 2));
	return r->reserved_25_31;
}

//reg reg86:

static inline void pwm_ll_set_reg86_value(uint32_t v) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg86_value(void) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg86_ch1p(uint32_t v) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	r->ch1p = v;
}

static inline uint32_t pwm_ll_get_reg86_ch1p(void) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	return r->ch1p;
}

static inline void pwm_ll_set_reg86_ch2p(uint32_t v) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	r->ch2p = v;
}

static inline uint32_t pwm_ll_get_reg86_ch2p(void) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	return r->ch2p;
}

static inline void pwm_ll_set_reg86_ch3p(uint32_t v) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	r->ch3p = v;
}

static inline uint32_t pwm_ll_get_reg86_ch3p(void) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	return r->ch3p;
}

static inline void pwm_ll_set_reg86_ch4p(uint32_t v) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	r->ch4p = v;
}

static inline uint32_t pwm_ll_get_reg86_ch4p(void) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	return r->ch4p;
}

static inline void pwm_ll_set_reg86_ch5p(uint32_t v) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	r->ch5p = v;
}

static inline uint32_t pwm_ll_get_reg86_ch5p(void) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	return r->ch5p;
}

static inline void pwm_ll_set_reg86_ch6p(uint32_t v) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	r->ch6p = v;
}

static inline uint32_t pwm_ll_get_reg86_ch6p(void) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	return r->ch6p;
}

static inline void pwm_ll_set_reg86_ch1e(uint32_t v) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	r->ch1e = v;
}

static inline uint32_t pwm_ll_get_reg86_ch1e(void) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	return r->ch1e;
}

static inline void pwm_ll_set_reg86_ch2e(uint32_t v) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	r->ch2e = v;
}

static inline uint32_t pwm_ll_get_reg86_ch2e(void) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	return r->ch2e;
}

static inline void pwm_ll_set_reg86_ch3e(uint32_t v) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	r->ch3e = v;
}

static inline uint32_t pwm_ll_get_reg86_ch3e(void) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	return r->ch3e;
}

static inline void pwm_ll_set_reg86_ch4e(uint32_t v) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	r->ch4e = v;
}

static inline uint32_t pwm_ll_get_reg86_ch4e(void) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	return r->ch4e;
}

static inline void pwm_ll_set_reg86_ch5e(uint32_t v) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	r->ch5e = v;
}

static inline uint32_t pwm_ll_get_reg86_ch5e(void) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	return r->ch5e;
}

static inline void pwm_ll_set_reg86_ch6e(uint32_t v) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	r->ch6e = v;
}

static inline uint32_t pwm_ll_get_reg86_ch6e(void) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	return r->ch6e;
}

static inline void pwm_ll_set_reg86_tim1ccm(uint32_t v) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	r->tim1ccm = v;
}

static inline uint32_t pwm_ll_get_reg86_tim1ccm(void) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	return r->tim1ccm;
}

static inline void pwm_ll_set_reg86_tim2ccm(uint32_t v) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	r->tim2ccm = v;
}

static inline uint32_t pwm_ll_get_reg86_tim2ccm(void) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	return r->tim2ccm;
}

static inline void pwm_ll_set_reg86_tim3ccm(uint32_t v) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	r->tim3ccm = v;
}

static inline uint32_t pwm_ll_get_reg86_tim3ccm(void) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	return r->tim3ccm;
}

static inline void pwm_ll_set_reg86_oc1m(uint32_t v) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	r->oc1m = v;
}

static inline uint32_t pwm_ll_get_reg86_oc1m(void) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	return r->oc1m;
}

static inline void pwm_ll_set_reg86_oc2m(uint32_t v) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	r->oc2m = v;
}

static inline uint32_t pwm_ll_get_reg86_oc2m(void) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	return r->oc2m;
}

static inline void pwm_ll_set_reg86_oc3m(uint32_t v) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	r->oc3m = v;
}

static inline uint32_t pwm_ll_get_reg86_oc3m(void) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	return r->oc3m;
}

static inline void pwm_ll_set_reg86_reserved_30_31(uint32_t v) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	r->reserved_30_31 = v;
}

static inline uint32_t pwm_ll_get_reg86_reserved_30_31(void) {
	pwm_reg86_t *r = (pwm_reg86_t*)(SOC_PWM_REG_BASE + (0x86 << 2));
	return r->reserved_30_31;
}

//reg reg87:

static inline void pwm_ll_set_reg87_value(uint32_t v) {
	pwm_reg87_t *r = (pwm_reg87_t*)(SOC_PWM_REG_BASE + (0x87 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg87_value(void) {
	pwm_reg87_t *r = (pwm_reg87_t*)(SOC_PWM_REG_BASE + (0x87 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg87_tim1_cnt(void) {
	pwm_reg87_t *r = (pwm_reg87_t*)(SOC_PWM_REG_BASE + (0x87 << 2));
	return r->tim1_cnt;
}

//reg reg88:

static inline void pwm_ll_set_reg88_value(uint32_t v) {
	pwm_reg88_t *r = (pwm_reg88_t*)(SOC_PWM_REG_BASE + (0x88 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg88_value(void) {
	pwm_reg88_t *r = (pwm_reg88_t*)(SOC_PWM_REG_BASE + (0x88 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg88_tim2_cnt(void) {
	pwm_reg88_t *r = (pwm_reg88_t*)(SOC_PWM_REG_BASE + (0x88 << 2));
	return r->tim2_cnt;
}

//reg reg89:

static inline void pwm_ll_set_reg89_value(uint32_t v) {
	pwm_reg89_t *r = (pwm_reg89_t*)(SOC_PWM_REG_BASE + (0x89 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg89_value(void) {
	pwm_reg89_t *r = (pwm_reg89_t*)(SOC_PWM_REG_BASE + (0x89 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg89_tim3_cnt(void) {
	pwm_reg89_t *r = (pwm_reg89_t*)(SOC_PWM_REG_BASE + (0x89 << 2));
	return r->tim3_cnt;
}

//reg reg8a:

static inline void pwm_ll_set_reg8a_value(uint32_t v) {
	pwm_reg8a_t *r = (pwm_reg8a_t*)(SOC_PWM_REG_BASE + (0x8a << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg8a_value(void) {
	pwm_reg8a_t *r = (pwm_reg8a_t*)(SOC_PWM_REG_BASE + (0x8a << 2));
	return r->v;
}

static inline void pwm_ll_set_reg8a_psc1(uint32_t v) {
	pwm_reg8a_t *r = (pwm_reg8a_t*)(SOC_PWM_REG_BASE + (0x8a << 2));
	r->psc1 = v;
}

static inline uint32_t pwm_ll_get_reg8a_psc1(void) {
	pwm_reg8a_t *r = (pwm_reg8a_t*)(SOC_PWM_REG_BASE + (0x8a << 2));
	return r->psc1;
}

static inline void pwm_ll_set_reg8a_psc2(uint32_t v) {
	pwm_reg8a_t *r = (pwm_reg8a_t*)(SOC_PWM_REG_BASE + (0x8a << 2));
	r->psc2 = v;
}

static inline uint32_t pwm_ll_get_reg8a_psc2(void) {
	pwm_reg8a_t *r = (pwm_reg8a_t*)(SOC_PWM_REG_BASE + (0x8a << 2));
	return r->psc2;
}

static inline void pwm_ll_set_reg8a_psc3(uint32_t v) {
	pwm_reg8a_t *r = (pwm_reg8a_t*)(SOC_PWM_REG_BASE + (0x8a << 2));
	r->psc3 = v;
}

static inline uint32_t pwm_ll_get_reg8a_psc3(void) {
	pwm_reg8a_t *r = (pwm_reg8a_t*)(SOC_PWM_REG_BASE + (0x8a << 2));
	return r->psc3;
}

static inline void pwm_ll_set_reg8a_reserved_24_31(uint32_t v) {
	pwm_reg8a_t *r = (pwm_reg8a_t*)(SOC_PWM_REG_BASE + (0x8a << 2));
	r->reserved_24_31 = v;
}

static inline uint32_t pwm_ll_get_reg8a_reserved_24_31(void) {
	pwm_reg8a_t *r = (pwm_reg8a_t*)(SOC_PWM_REG_BASE + (0x8a << 2));
	return r->reserved_24_31;
}

//reg reg8b:

static inline void pwm_ll_set_reg8b_value(uint32_t v) {
	pwm_reg8b_t *r = (pwm_reg8b_t*)(SOC_PWM_REG_BASE + (0x8b << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg8b_value(void) {
	pwm_reg8b_t *r = (pwm_reg8b_t*)(SOC_PWM_REG_BASE + (0x8b << 2));
	return r->v;
}

static inline void pwm_ll_set_reg8b_tim1_arr(uint32_t v) {
	pwm_reg8b_t *r = (pwm_reg8b_t*)(SOC_PWM_REG_BASE + (0x8b << 2));
	r->tim1_arr = v;
}

static inline uint32_t pwm_ll_get_reg8b_tim1_arr(void) {
	pwm_reg8b_t *r = (pwm_reg8b_t*)(SOC_PWM_REG_BASE + (0x8b << 2));
	return r->tim1_arr;
}

//reg reg8c:

static inline void pwm_ll_set_reg8c_value(uint32_t v) {
	pwm_reg8c_t *r = (pwm_reg8c_t*)(SOC_PWM_REG_BASE + (0x8c << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg8c_value(void) {
	pwm_reg8c_t *r = (pwm_reg8c_t*)(SOC_PWM_REG_BASE + (0x8c << 2));
	return r->v;
}

static inline void pwm_ll_set_reg8c_tim2_arr(uint32_t v) {
	pwm_reg8c_t *r = (pwm_reg8c_t*)(SOC_PWM_REG_BASE + (0x8c << 2));
	r->tim2_arr = v;
}

static inline uint32_t pwm_ll_get_reg8c_tim2_arr(void) {
	pwm_reg8c_t *r = (pwm_reg8c_t*)(SOC_PWM_REG_BASE + (0x8c << 2));
	return r->tim2_arr;
}

//reg reg8d:

static inline void pwm_ll_set_reg8d_value(uint32_t v) {
	pwm_reg8d_t *r = (pwm_reg8d_t*)(SOC_PWM_REG_BASE + (0x8d << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg8d_value(void) {
	pwm_reg8d_t *r = (pwm_reg8d_t*)(SOC_PWM_REG_BASE + (0x8d << 2));
	return r->v;
}

static inline void pwm_ll_set_reg8d_tim3_arr(uint32_t v) {
	pwm_reg8d_t *r = (pwm_reg8d_t*)(SOC_PWM_REG_BASE + (0x8d << 2));
	r->tim3_arr = v;
}

static inline uint32_t pwm_ll_get_reg8d_tim3_arr(void) {
	pwm_reg8d_t *r = (pwm_reg8d_t*)(SOC_PWM_REG_BASE + (0x8d << 2));
	return r->tim3_arr;
}

//reg reg8e:

static inline void pwm_ll_set_reg8e_value(uint32_t v) {
	pwm_reg8e_t *r = (pwm_reg8e_t*)(SOC_PWM_REG_BASE + (0x8e << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg8e_value(void) {
	pwm_reg8e_t *r = (pwm_reg8e_t*)(SOC_PWM_REG_BASE + (0x8e << 2));
	return r->v;
}

static inline void pwm_ll_set_reg8e_tim1_rcr(uint32_t v) {
	pwm_reg8e_t *r = (pwm_reg8e_t*)(SOC_PWM_REG_BASE + (0x8e << 2));
	r->tim1_rcr = v;
}

static inline uint32_t pwm_ll_get_reg8e_tim1_rcr(void) {
	pwm_reg8e_t *r = (pwm_reg8e_t*)(SOC_PWM_REG_BASE + (0x8e << 2));
	return r->tim1_rcr;
}

//reg reg8f:

static inline void pwm_ll_set_reg8f_value(uint32_t v) {
	pwm_reg8f_t *r = (pwm_reg8f_t*)(SOC_PWM_REG_BASE + (0x8f << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg8f_value(void) {
	pwm_reg8f_t *r = (pwm_reg8f_t*)(SOC_PWM_REG_BASE + (0x8f << 2));
	return r->v;
}

static inline void pwm_ll_set_reg8f_tim2_rcr(uint32_t v) {
	pwm_reg8f_t *r = (pwm_reg8f_t*)(SOC_PWM_REG_BASE + (0x8f << 2));
	r->tim2_rcr = v;
}

static inline uint32_t pwm_ll_get_reg8f_tim2_rcr(void) {
	pwm_reg8f_t *r = (pwm_reg8f_t*)(SOC_PWM_REG_BASE + (0x8f << 2));
	return r->tim2_rcr;
}

//reg reg90:

static inline void pwm_ll_set_reg90_value(uint32_t v) {
	pwm_reg90_t *r = (pwm_reg90_t*)(SOC_PWM_REG_BASE + (0x90 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg90_value(void) {
	pwm_reg90_t *r = (pwm_reg90_t*)(SOC_PWM_REG_BASE + (0x90 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg90_tim3_rcr(uint32_t v) {
	pwm_reg90_t *r = (pwm_reg90_t*)(SOC_PWM_REG_BASE + (0x90 << 2));
	r->tim3_rcr = v;
}

static inline uint32_t pwm_ll_get_reg90_tim3_rcr(void) {
	pwm_reg90_t *r = (pwm_reg90_t*)(SOC_PWM_REG_BASE + (0x90 << 2));
	return r->tim3_rcr;
}

//reg reg91:

static inline void pwm_ll_set_reg91_value(uint32_t v) {
	pwm_reg91_t *r = (pwm_reg91_t*)(SOC_PWM_REG_BASE + (0x91 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg91_value(void) {
	pwm_reg91_t *r = (pwm_reg91_t*)(SOC_PWM_REG_BASE + (0x91 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg91_ccr1(uint32_t v) {
	pwm_reg91_t *r = (pwm_reg91_t*)(SOC_PWM_REG_BASE + (0x91 << 2));
	r->ccr1 = v;
}

static inline uint32_t pwm_ll_get_reg91_ccr1(void) {
	pwm_reg91_t *r = (pwm_reg91_t*)(SOC_PWM_REG_BASE + (0x91 << 2));
	return r->ccr1;
}

//reg reg92:

static inline void pwm_ll_set_reg92_value(uint32_t v) {
	pwm_reg92_t *r = (pwm_reg92_t*)(SOC_PWM_REG_BASE + (0x92 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg92_value(void) {
	pwm_reg92_t *r = (pwm_reg92_t*)(SOC_PWM_REG_BASE + (0x92 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg92_ccr2(uint32_t v) {
	pwm_reg92_t *r = (pwm_reg92_t*)(SOC_PWM_REG_BASE + (0x92 << 2));
	r->ccr2 = v;
}

static inline uint32_t pwm_ll_get_reg92_ccr2(void) {
	pwm_reg92_t *r = (pwm_reg92_t*)(SOC_PWM_REG_BASE + (0x92 << 2));
	return r->ccr2;
}

//reg reg93:

static inline void pwm_ll_set_reg93_value(uint32_t v) {
	pwm_reg93_t *r = (pwm_reg93_t*)(SOC_PWM_REG_BASE + (0x93 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg93_value(void) {
	pwm_reg93_t *r = (pwm_reg93_t*)(SOC_PWM_REG_BASE + (0x93 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg93_ccr3(uint32_t v) {
	pwm_reg93_t *r = (pwm_reg93_t*)(SOC_PWM_REG_BASE + (0x93 << 2));
	r->ccr3 = v;
}

static inline uint32_t pwm_ll_get_reg93_ccr3(void) {
	pwm_reg93_t *r = (pwm_reg93_t*)(SOC_PWM_REG_BASE + (0x93 << 2));
	return r->ccr3;
}

//reg reg94:

static inline void pwm_ll_set_reg94_value(uint32_t v) {
	pwm_reg94_t *r = (pwm_reg94_t*)(SOC_PWM_REG_BASE + (0x94 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg94_value(void) {
	pwm_reg94_t *r = (pwm_reg94_t*)(SOC_PWM_REG_BASE + (0x94 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg94_ccr4(uint32_t v) {
	pwm_reg94_t *r = (pwm_reg94_t*)(SOC_PWM_REG_BASE + (0x94 << 2));
	r->ccr4 = v;
}

static inline uint32_t pwm_ll_get_reg94_ccr4(void) {
	pwm_reg94_t *r = (pwm_reg94_t*)(SOC_PWM_REG_BASE + (0x94 << 2));
	return r->ccr4;
}

//reg reg95:

static inline void pwm_ll_set_reg95_value(uint32_t v) {
	pwm_reg95_t *r = (pwm_reg95_t*)(SOC_PWM_REG_BASE + (0x95 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg95_value(void) {
	pwm_reg95_t *r = (pwm_reg95_t*)(SOC_PWM_REG_BASE + (0x95 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg95_ccr5(uint32_t v) {
	pwm_reg95_t *r = (pwm_reg95_t*)(SOC_PWM_REG_BASE + (0x95 << 2));
	r->ccr5 = v;
}

static inline uint32_t pwm_ll_get_reg95_ccr5(void) {
	pwm_reg95_t *r = (pwm_reg95_t*)(SOC_PWM_REG_BASE + (0x95 << 2));
	return r->ccr5;
}

//reg reg96:

static inline void pwm_ll_set_reg96_value(uint32_t v) {
	pwm_reg96_t *r = (pwm_reg96_t*)(SOC_PWM_REG_BASE + (0x96 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg96_value(void) {
	pwm_reg96_t *r = (pwm_reg96_t*)(SOC_PWM_REG_BASE + (0x96 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg96_ccr6(uint32_t v) {
	pwm_reg96_t *r = (pwm_reg96_t*)(SOC_PWM_REG_BASE + (0x96 << 2));
	r->ccr6 = v;
}

static inline uint32_t pwm_ll_get_reg96_ccr6(void) {
	pwm_reg96_t *r = (pwm_reg96_t*)(SOC_PWM_REG_BASE + (0x96 << 2));
	return r->ccr6;
}

//reg reg97:

static inline void pwm_ll_set_reg97_value(uint32_t v) {
	pwm_reg97_t *r = (pwm_reg97_t*)(SOC_PWM_REG_BASE + (0x97 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg97_value(void) {
	pwm_reg97_t *r = (pwm_reg97_t*)(SOC_PWM_REG_BASE + (0x97 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg97_ccr7(uint32_t v) {
	pwm_reg97_t *r = (pwm_reg97_t*)(SOC_PWM_REG_BASE + (0x97 << 2));
	r->ccr7 = v;
}

static inline uint32_t pwm_ll_get_reg97_ccr7(void) {
	pwm_reg97_t *r = (pwm_reg97_t*)(SOC_PWM_REG_BASE + (0x97 << 2));
	return r->ccr7;
}

//reg reg98:

static inline void pwm_ll_set_reg98_value(uint32_t v) {
	pwm_reg98_t *r = (pwm_reg98_t*)(SOC_PWM_REG_BASE + (0x98 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg98_value(void) {
	pwm_reg98_t *r = (pwm_reg98_t*)(SOC_PWM_REG_BASE + (0x98 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg98_ccr8(uint32_t v) {
	pwm_reg98_t *r = (pwm_reg98_t*)(SOC_PWM_REG_BASE + (0x98 << 2));
	r->ccr8 = v;
}

static inline uint32_t pwm_ll_get_reg98_ccr8(void) {
	pwm_reg98_t *r = (pwm_reg98_t*)(SOC_PWM_REG_BASE + (0x98 << 2));
	return r->ccr8;
}

//reg reg99:

static inline void pwm_ll_set_reg99_value(uint32_t v) {
	pwm_reg99_t *r = (pwm_reg99_t*)(SOC_PWM_REG_BASE + (0x99 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg99_value(void) {
	pwm_reg99_t *r = (pwm_reg99_t*)(SOC_PWM_REG_BASE + (0x99 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg99_ccr9(uint32_t v) {
	pwm_reg99_t *r = (pwm_reg99_t*)(SOC_PWM_REG_BASE + (0x99 << 2));
	r->ccr9 = v;
}

static inline uint32_t pwm_ll_get_reg99_ccr9(void) {
	pwm_reg99_t *r = (pwm_reg99_t*)(SOC_PWM_REG_BASE + (0x99 << 2));
	return r->ccr9;
}

//reg reg9a:

static inline void pwm_ll_set_reg9a_value(uint32_t v) {
	pwm_reg9a_t *r = (pwm_reg9a_t*)(SOC_PWM_REG_BASE + (0x9a << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg9a_value(void) {
	pwm_reg9a_t *r = (pwm_reg9a_t*)(SOC_PWM_REG_BASE + (0x9a << 2));
	return r->v;
}

static inline void pwm_ll_set_reg9a_dt1(uint32_t v) {
	pwm_reg9a_t *r = (pwm_reg9a_t*)(SOC_PWM_REG_BASE + (0x9a << 2));
	r->dt1 = v;
}

static inline uint32_t pwm_ll_get_reg9a_dt1(void) {
	pwm_reg9a_t *r = (pwm_reg9a_t*)(SOC_PWM_REG_BASE + (0x9a << 2));
	return r->dt1;
}

static inline void pwm_ll_set_reg9a_dt2(uint32_t v) {
	pwm_reg9a_t *r = (pwm_reg9a_t*)(SOC_PWM_REG_BASE + (0x9a << 2));
	r->dt2 = v;
}

static inline uint32_t pwm_ll_get_reg9a_dt2(void) {
	pwm_reg9a_t *r = (pwm_reg9a_t*)(SOC_PWM_REG_BASE + (0x9a << 2));
	return r->dt2;
}

static inline void pwm_ll_set_reg9a_dt3(uint32_t v) {
	pwm_reg9a_t *r = (pwm_reg9a_t*)(SOC_PWM_REG_BASE + (0x9a << 2));
	r->dt3 = v;
}

static inline uint32_t pwm_ll_get_reg9a_dt3(void) {
	pwm_reg9a_t *r = (pwm_reg9a_t*)(SOC_PWM_REG_BASE + (0x9a << 2));
	return r->dt3;
}

static inline void pwm_ll_set_reg9a_reserved_30_31(uint32_t v) {
	pwm_reg9a_t *r = (pwm_reg9a_t*)(SOC_PWM_REG_BASE + (0x9a << 2));
	r->reserved_30_31 = v;
}

static inline uint32_t pwm_ll_get_reg9a_reserved_30_31(void) {
	pwm_reg9a_t *r = (pwm_reg9a_t*)(SOC_PWM_REG_BASE + (0x9a << 2));
	return r->reserved_30_31;
}

//reg reg9b:

static inline void pwm_ll_set_reg9b_value(uint32_t v) {
	pwm_reg9b_t *r = (pwm_reg9b_t*)(SOC_PWM_REG_BASE + (0x9b << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg9b_value(void) {
	pwm_reg9b_t *r = (pwm_reg9b_t*)(SOC_PWM_REG_BASE + (0x9b << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg9b_tim1_arr_shad(void) {
	pwm_reg9b_t *r = (pwm_reg9b_t*)(SOC_PWM_REG_BASE + (0x9b << 2));
	return r->tim1_arr_shad;
}

//reg reg9c:

static inline void pwm_ll_set_reg9c_value(uint32_t v) {
	pwm_reg9c_t *r = (pwm_reg9c_t*)(SOC_PWM_REG_BASE + (0x9c << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg9c_value(void) {
	pwm_reg9c_t *r = (pwm_reg9c_t*)(SOC_PWM_REG_BASE + (0x9c << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg9c_tim2_arr_shad(void) {
	pwm_reg9c_t *r = (pwm_reg9c_t*)(SOC_PWM_REG_BASE + (0x9c << 2));
	return r->tim2_arr_shad;
}

//reg reg9d:

static inline void pwm_ll_set_reg9d_value(uint32_t v) {
	pwm_reg9d_t *r = (pwm_reg9d_t*)(SOC_PWM_REG_BASE + (0x9d << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg9d_value(void) {
	pwm_reg9d_t *r = (pwm_reg9d_t*)(SOC_PWM_REG_BASE + (0x9d << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg9d_tim3_arr_shad(void) {
	pwm_reg9d_t *r = (pwm_reg9d_t*)(SOC_PWM_REG_BASE + (0x9d << 2));
	return r->tim3_arr_shad;
}

//reg reg9e:

static inline void pwm_ll_set_reg9e_value(uint32_t v) {
	pwm_reg9e_t *r = (pwm_reg9e_t*)(SOC_PWM_REG_BASE + (0x9e << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg9e_value(void) {
	pwm_reg9e_t *r = (pwm_reg9e_t*)(SOC_PWM_REG_BASE + (0x9e << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg9e_tim1_rcr_shad(void) {
	pwm_reg9e_t *r = (pwm_reg9e_t*)(SOC_PWM_REG_BASE + (0x9e << 2));
	return r->tim1_rcr_shad;
}

//reg reg9f:

static inline void pwm_ll_set_reg9f_value(uint32_t v) {
	pwm_reg9f_t *r = (pwm_reg9f_t*)(SOC_PWM_REG_BASE + (0x9f << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg9f_value(void) {
	pwm_reg9f_t *r = (pwm_reg9f_t*)(SOC_PWM_REG_BASE + (0x9f << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg9f_tim2_rcr_shad(void) {
	pwm_reg9f_t *r = (pwm_reg9f_t*)(SOC_PWM_REG_BASE + (0x9f << 2));
	return r->tim2_rcr_shad;
}

//reg rega0:

static inline void pwm_ll_set_rega0_value(uint32_t v) {
	pwm_rega0_t *r = (pwm_rega0_t*)(SOC_PWM_REG_BASE + (0xa0 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_rega0_value(void) {
	pwm_rega0_t *r = (pwm_rega0_t*)(SOC_PWM_REG_BASE + (0xa0 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_rega0_tim3_rcr_shad(void) {
	pwm_rega0_t *r = (pwm_rega0_t*)(SOC_PWM_REG_BASE + (0xa0 << 2));
	return r->tim3_rcr_shad;
}

//reg rega1:

static inline void pwm_ll_set_rega1_value(uint32_t v) {
	pwm_rega1_t *r = (pwm_rega1_t*)(SOC_PWM_REG_BASE + (0xa1 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_rega1_value(void) {
	pwm_rega1_t *r = (pwm_rega1_t*)(SOC_PWM_REG_BASE + (0xa1 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_rega1_ccr1_shad(void) {
	pwm_rega1_t *r = (pwm_rega1_t*)(SOC_PWM_REG_BASE + (0xa1 << 2));
	return r->ccr1_shad;
}

//reg rega2:

static inline void pwm_ll_set_rega2_value(uint32_t v) {
	pwm_rega2_t *r = (pwm_rega2_t*)(SOC_PWM_REG_BASE + (0xa2 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_rega2_value(void) {
	pwm_rega2_t *r = (pwm_rega2_t*)(SOC_PWM_REG_BASE + (0xa2 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_rega2_ccr2_shad(void) {
	pwm_rega2_t *r = (pwm_rega2_t*)(SOC_PWM_REG_BASE + (0xa2 << 2));
	return r->ccr2_shad;
}

//reg rega3:

static inline void pwm_ll_set_rega3_value(uint32_t v) {
	pwm_rega3_t *r = (pwm_rega3_t*)(SOC_PWM_REG_BASE + (0xa3 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_rega3_value(void) {
	pwm_rega3_t *r = (pwm_rega3_t*)(SOC_PWM_REG_BASE + (0xa3 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_rega3_ccr3_shad(void) {
	pwm_rega3_t *r = (pwm_rega3_t*)(SOC_PWM_REG_BASE + (0xa3 << 2));
	return r->ccr3_shad;
}

//reg rega4:

static inline void pwm_ll_set_rega4_value(uint32_t v) {
	pwm_rega4_t *r = (pwm_rega4_t*)(SOC_PWM_REG_BASE + (0xa4 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_rega4_value(void) {
	pwm_rega4_t *r = (pwm_rega4_t*)(SOC_PWM_REG_BASE + (0xa4 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_rega4_ccr4_shad(void) {
	pwm_rega4_t *r = (pwm_rega4_t*)(SOC_PWM_REG_BASE + (0xa4 << 2));
	return r->ccr4_shad;
}

//reg rega5:

static inline void pwm_ll_set_rega5_value(uint32_t v) {
	pwm_rega5_t *r = (pwm_rega5_t*)(SOC_PWM_REG_BASE + (0xa5 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_rega5_value(void) {
	pwm_rega5_t *r = (pwm_rega5_t*)(SOC_PWM_REG_BASE + (0xa5 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_rega5_ccr5_shad(void) {
	pwm_rega5_t *r = (pwm_rega5_t*)(SOC_PWM_REG_BASE + (0xa5 << 2));
	return r->ccr5_shad;
}

//reg rega6:

static inline void pwm_ll_set_rega6_value(uint32_t v) {
	pwm_rega6_t *r = (pwm_rega6_t*)(SOC_PWM_REG_BASE + (0xa6 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_rega6_value(void) {
	pwm_rega6_t *r = (pwm_rega6_t*)(SOC_PWM_REG_BASE + (0xa6 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_rega6_ccr6_shad(void) {
	pwm_rega6_t *r = (pwm_rega6_t*)(SOC_PWM_REG_BASE + (0xa6 << 2));
	return r->ccr6_shad;
}

//reg rega7:

static inline void pwm_ll_set_rega7_value(uint32_t v) {
	pwm_rega7_t *r = (pwm_rega7_t*)(SOC_PWM_REG_BASE + (0xa7 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_rega7_value(void) {
	pwm_rega7_t *r = (pwm_rega7_t*)(SOC_PWM_REG_BASE + (0xa7 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_rega7_ccr7_shad(void) {
	pwm_rega7_t *r = (pwm_rega7_t*)(SOC_PWM_REG_BASE + (0xa7 << 2));
	return r->ccr7_shad;
}

//reg rega8:

static inline void pwm_ll_set_rega8_value(uint32_t v) {
	pwm_rega8_t *r = (pwm_rega8_t*)(SOC_PWM_REG_BASE + (0xa8 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_rega8_value(void) {
	pwm_rega8_t *r = (pwm_rega8_t*)(SOC_PWM_REG_BASE + (0xa8 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_rega8_ccr8_shad(void) {
	pwm_rega8_t *r = (pwm_rega8_t*)(SOC_PWM_REG_BASE + (0xa8 << 2));
	return r->ccr8_shad;
}

//reg rega9:

static inline void pwm_ll_set_rega9_value(uint32_t v) {
	pwm_rega9_t *r = (pwm_rega9_t*)(SOC_PWM_REG_BASE + (0xa9 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_rega9_value(void) {
	pwm_rega9_t *r = (pwm_rega9_t*)(SOC_PWM_REG_BASE + (0xa9 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_rega9_ccr9_shad(void) {
	pwm_rega9_t *r = (pwm_rega9_t*)(SOC_PWM_REG_BASE + (0xa9 << 2));
	return r->ccr9_shad;
}

//reg regaa:

static inline void pwm_ll_set_regaa_value(uint32_t v) {
	pwm_regaa_t *r = (pwm_regaa_t*)(SOC_PWM_REG_BASE + (0xaa << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regaa_value(void) {
	pwm_regaa_t *r = (pwm_regaa_t*)(SOC_PWM_REG_BASE + (0xaa << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_regaa_ch6e_shad(void) {
	pwm_regaa_t *r = (pwm_regaa_t*)(SOC_PWM_REG_BASE + (0xaa << 2));
	return r->ch6e_shad;
}

static inline uint32_t pwm_ll_get_regaa_ch5e_shad(void) {
	pwm_regaa_t *r = (pwm_regaa_t*)(SOC_PWM_REG_BASE + (0xaa << 2));
	return r->ch5e_shad;
}

static inline uint32_t pwm_ll_get_regaa_ch4e_shad(void) {
	pwm_regaa_t *r = (pwm_regaa_t*)(SOC_PWM_REG_BASE + (0xaa << 2));
	return r->ch4e_shad;
}

static inline uint32_t pwm_ll_get_regaa_ch3e_shad(void) {
	pwm_regaa_t *r = (pwm_regaa_t*)(SOC_PWM_REG_BASE + (0xaa << 2));
	return r->ch3e_shad;
}

static inline uint32_t pwm_ll_get_regaa_ch2e_shad(void) {
	pwm_regaa_t *r = (pwm_regaa_t*)(SOC_PWM_REG_BASE + (0xaa << 2));
	return r->ch2e_shad;
}

static inline uint32_t pwm_ll_get_regaa_ch1e_shad(void) {
	pwm_regaa_t *r = (pwm_regaa_t*)(SOC_PWM_REG_BASE + (0xaa << 2));
	return r->ch1e_shad;
}

static inline uint32_t pwm_ll_get_regaa_oc1m_shad(void) {
	pwm_regaa_t *r = (pwm_regaa_t*)(SOC_PWM_REG_BASE + (0xaa << 2));
	return r->oc1m_shad;
}

static inline uint32_t pwm_ll_get_regaa_oc2m_shad(void) {
	pwm_regaa_t *r = (pwm_regaa_t*)(SOC_PWM_REG_BASE + (0xaa << 2));
	return r->oc2m_shad;
}

static inline uint32_t pwm_ll_get_regaa_oc3m_shad(void) {
	pwm_regaa_t *r = (pwm_regaa_t*)(SOC_PWM_REG_BASE + (0xaa << 2));
	return r->oc3m_shad;
}

static inline uint32_t pwm_ll_get_regaa_reserved_15_31(void) {
	pwm_regaa_t *r = (pwm_regaa_t*)(SOC_PWM_REG_BASE + (0xaa << 2));
	return r->reserved_15_31;
}

//reg regab:

static inline void pwm_ll_set_regab_value(uint32_t v) {
	pwm_regab_t *r = (pwm_regab_t*)(SOC_PWM_REG_BASE + (0xab << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regab_value(void) {
	pwm_regab_t *r = (pwm_regab_t*)(SOC_PWM_REG_BASE + (0xab << 2));
	return r->v;
}

static inline void pwm_ll_set_regab_tim1_fade_num(uint32_t v) {
	pwm_regab_t *r = (pwm_regab_t*)(SOC_PWM_REG_BASE + (0xab << 2));
	r->tim1_fade_num = v;
}

static inline uint32_t pwm_ll_get_regab_tim1_fade_num(void) {
	pwm_regab_t *r = (pwm_regab_t*)(SOC_PWM_REG_BASE + (0xab << 2));
	return r->tim1_fade_num;
}

static inline void pwm_ll_set_regab_tim1_fade_intval_cyc(uint32_t v) {
	pwm_regab_t *r = (pwm_regab_t*)(SOC_PWM_REG_BASE + (0xab << 2));
	r->tim1_fade_intval_cyc = v;
}

static inline uint32_t pwm_ll_get_regab_tim1_fade_intval_cyc(void) {
	pwm_regab_t *r = (pwm_regab_t*)(SOC_PWM_REG_BASE + (0xab << 2));
	return r->tim1_fade_intval_cyc;
}

static inline void pwm_ll_set_regab_tim1_fade_scale(uint32_t v) {
	pwm_regab_t *r = (pwm_regab_t*)(SOC_PWM_REG_BASE + (0xab << 2));
	r->tim1_fade_scale = v;
}

static inline uint32_t pwm_ll_get_regab_tim1_fade_scale(void) {
	pwm_regab_t *r = (pwm_regab_t*)(SOC_PWM_REG_BASE + (0xab << 2));
	return r->tim1_fade_scale;
}

static inline void pwm_ll_set_regab_tim1_fade_end_if(uint32_t v) {
	pwm_regab_t *r = (pwm_regab_t*)(SOC_PWM_REG_BASE + (0xab << 2));
	r->tim1_fade_end_if = v;
}

static inline uint32_t pwm_ll_get_regab_tim1_fade_end_if(void) {
	pwm_regab_t *r = (pwm_regab_t*)(SOC_PWM_REG_BASE + (0xab << 2));
	return r->tim1_fade_end_if;
}

static inline void pwm_ll_set_regab_tim1_fade_end_int_en(uint32_t v) {
	pwm_regab_t *r = (pwm_regab_t*)(SOC_PWM_REG_BASE + (0xab << 2));
	r->tim1_fade_end_int_en = v;
}

static inline uint32_t pwm_ll_get_regab_tim1_fade_end_int_en(void) {
	pwm_regab_t *r = (pwm_regab_t*)(SOC_PWM_REG_BASE + (0xab << 2));
	return r->tim1_fade_end_int_en;
}

static inline void pwm_ll_set_regab_tim1_fade_inc_dec(uint32_t v) {
	pwm_regab_t *r = (pwm_regab_t*)(SOC_PWM_REG_BASE + (0xab << 2));
	r->tim1_fade_inc_dec = v;
}

static inline uint32_t pwm_ll_get_regab_tim1_fade_inc_dec(void) {
	pwm_regab_t *r = (pwm_regab_t*)(SOC_PWM_REG_BASE + (0xab << 2));
	return r->tim1_fade_inc_dec;
}

static inline void pwm_ll_set_regab_tim1_duty_fading_en(uint32_t v) {
	pwm_regab_t *r = (pwm_regab_t*)(SOC_PWM_REG_BASE + (0xab << 2));
	r->tim1_duty_fading_en = v;
}

static inline uint32_t pwm_ll_get_regab_tim1_duty_fading_en(void) {
	pwm_regab_t *r = (pwm_regab_t*)(SOC_PWM_REG_BASE + (0xab << 2));
	return r->tim1_duty_fading_en;
}

//reg regac:

static inline void pwm_ll_set_regac_value(uint32_t v) {
	pwm_regac_t *r = (pwm_regac_t*)(SOC_PWM_REG_BASE + (0xac << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regac_value(void) {
	pwm_regac_t *r = (pwm_regac_t*)(SOC_PWM_REG_BASE + (0xac << 2));
	return r->v;
}

static inline void pwm_ll_set_regac_tim2_fade_num(uint32_t v) {
	pwm_regac_t *r = (pwm_regac_t*)(SOC_PWM_REG_BASE + (0xac << 2));
	r->tim2_fade_num = v;
}

static inline uint32_t pwm_ll_get_regac_tim2_fade_num(void) {
	pwm_regac_t *r = (pwm_regac_t*)(SOC_PWM_REG_BASE + (0xac << 2));
	return r->tim2_fade_num;
}

static inline void pwm_ll_set_regac_tim2_fade_intval_cyc(uint32_t v) {
	pwm_regac_t *r = (pwm_regac_t*)(SOC_PWM_REG_BASE + (0xac << 2));
	r->tim2_fade_intval_cyc = v;
}

static inline uint32_t pwm_ll_get_regac_tim2_fade_intval_cyc(void) {
	pwm_regac_t *r = (pwm_regac_t*)(SOC_PWM_REG_BASE + (0xac << 2));
	return r->tim2_fade_intval_cyc;
}

static inline void pwm_ll_set_regac_tim2_fade_scale(uint32_t v) {
	pwm_regac_t *r = (pwm_regac_t*)(SOC_PWM_REG_BASE + (0xac << 2));
	r->tim2_fade_scale = v;
}

static inline uint32_t pwm_ll_get_regac_tim2_fade_scale(void) {
	pwm_regac_t *r = (pwm_regac_t*)(SOC_PWM_REG_BASE + (0xac << 2));
	return r->tim2_fade_scale;
}

static inline void pwm_ll_set_regac_tim2_fade_end_if(uint32_t v) {
	pwm_regac_t *r = (pwm_regac_t*)(SOC_PWM_REG_BASE + (0xac << 2));
	r->tim2_fade_end_if = v;
}

static inline uint32_t pwm_ll_get_regac_tim2_fade_end_if(void) {
	pwm_regac_t *r = (pwm_regac_t*)(SOC_PWM_REG_BASE + (0xac << 2));
	return r->tim2_fade_end_if;
}

static inline void pwm_ll_set_regac_tim2_fade_end_int_en(uint32_t v) {
	pwm_regac_t *r = (pwm_regac_t*)(SOC_PWM_REG_BASE + (0xac << 2));
	r->tim2_fade_end_int_en = v;
}

static inline uint32_t pwm_ll_get_regac_tim2_fade_end_int_en(void) {
	pwm_regac_t *r = (pwm_regac_t*)(SOC_PWM_REG_BASE + (0xac << 2));
	return r->tim2_fade_end_int_en;
}

static inline void pwm_ll_set_regac_tim2_fade_inc_dec(uint32_t v) {
	pwm_regac_t *r = (pwm_regac_t*)(SOC_PWM_REG_BASE + (0xac << 2));
	r->tim2_fade_inc_dec = v;
}

static inline uint32_t pwm_ll_get_regac_tim2_fade_inc_dec(void) {
	pwm_regac_t *r = (pwm_regac_t*)(SOC_PWM_REG_BASE + (0xac << 2));
	return r->tim2_fade_inc_dec;
}

static inline void pwm_ll_set_regac_tim2_duty_fading_en(uint32_t v) {
	pwm_regac_t *r = (pwm_regac_t*)(SOC_PWM_REG_BASE + (0xac << 2));
	r->tim2_duty_fading_en = v;
}

static inline uint32_t pwm_ll_get_regac_tim2_duty_fading_en(void) {
	pwm_regac_t *r = (pwm_regac_t*)(SOC_PWM_REG_BASE + (0xac << 2));
	return r->tim2_duty_fading_en;
}

//reg regad:

static inline void pwm_ll_set_regad_value(uint32_t v) {
	pwm_regad_t *r = (pwm_regad_t*)(SOC_PWM_REG_BASE + (0xad << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regad_value(void) {
	pwm_regad_t *r = (pwm_regad_t*)(SOC_PWM_REG_BASE + (0xad << 2));
	return r->v;
}

static inline void pwm_ll_set_regad_tim3_fade_num(uint32_t v) {
	pwm_regad_t *r = (pwm_regad_t*)(SOC_PWM_REG_BASE + (0xad << 2));
	r->tim3_fade_num = v;
}

static inline uint32_t pwm_ll_get_regad_tim3_fade_num(void) {
	pwm_regad_t *r = (pwm_regad_t*)(SOC_PWM_REG_BASE + (0xad << 2));
	return r->tim3_fade_num;
}

static inline void pwm_ll_set_regad_tim3_fade_intval_cyc(uint32_t v) {
	pwm_regad_t *r = (pwm_regad_t*)(SOC_PWM_REG_BASE + (0xad << 2));
	r->tim3_fade_intval_cyc = v;
}

static inline uint32_t pwm_ll_get_regad_tim3_fade_intval_cyc(void) {
	pwm_regad_t *r = (pwm_regad_t*)(SOC_PWM_REG_BASE + (0xad << 2));
	return r->tim3_fade_intval_cyc;
}

static inline void pwm_ll_set_regad_tim3_fade_scale(uint32_t v) {
	pwm_regad_t *r = (pwm_regad_t*)(SOC_PWM_REG_BASE + (0xad << 2));
	r->tim3_fade_scale = v;
}

static inline uint32_t pwm_ll_get_regad_tim3_fade_scale(void) {
	pwm_regad_t *r = (pwm_regad_t*)(SOC_PWM_REG_BASE + (0xad << 2));
	return r->tim3_fade_scale;
}

static inline void pwm_ll_set_regad_tim3_fade_end_if(uint32_t v) {
	pwm_regad_t *r = (pwm_regad_t*)(SOC_PWM_REG_BASE + (0xad << 2));
	r->tim3_fade_end_if = v;
}

static inline uint32_t pwm_ll_get_regad_tim3_fade_end_if(void) {
	pwm_regad_t *r = (pwm_regad_t*)(SOC_PWM_REG_BASE + (0xad << 2));
	return r->tim3_fade_end_if;
}

static inline void pwm_ll_set_regad_tim3_fade_end_int_en(uint32_t v) {
	pwm_regad_t *r = (pwm_regad_t*)(SOC_PWM_REG_BASE + (0xad << 2));
	r->tim3_fade_end_int_en = v;
}

static inline uint32_t pwm_ll_get_regad_tim3_fade_end_int_en(void) {
	pwm_regad_t *r = (pwm_regad_t*)(SOC_PWM_REG_BASE + (0xad << 2));
	return r->tim3_fade_end_int_en;
}

static inline void pwm_ll_set_regad_tim3_fade_inc_dec(uint32_t v) {
	pwm_regad_t *r = (pwm_regad_t*)(SOC_PWM_REG_BASE + (0xad << 2));
	r->tim3_fade_inc_dec = v;
}

static inline uint32_t pwm_ll_get_regad_tim3_fade_inc_dec(void) {
	pwm_regad_t *r = (pwm_regad_t*)(SOC_PWM_REG_BASE + (0xad << 2));
	return r->tim3_fade_inc_dec;
}

static inline void pwm_ll_set_regad_tim3_duty_fading_en(uint32_t v) {
	pwm_regad_t *r = (pwm_regad_t*)(SOC_PWM_REG_BASE + (0xad << 2));
	r->tim3_duty_fading_en = v;
}

static inline uint32_t pwm_ll_get_regad_tim3_duty_fading_en(void) {
	pwm_regad_t *r = (pwm_regad_t*)(SOC_PWM_REG_BASE + (0xad << 2));
	return r->tim3_duty_fading_en;
}

//reg regc0:

static inline void pwm_ll_set_regc0_value(uint32_t v) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regc0_value(void) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	return r->v;
}

static inline void pwm_ll_set_regc0_ocm3pe(uint32_t v) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	r->ocm3pe = v;
}

static inline uint32_t pwm_ll_get_regc0_ocm3pe(void) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	return r->ocm3pe;
}

static inline void pwm_ll_set_regc0_ocm2pe(uint32_t v) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	r->ocm2pe = v;
}

static inline uint32_t pwm_ll_get_regc0_ocm2pe(void) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	return r->ocm2pe;
}

static inline void pwm_ll_set_regc0_ocm1pe(uint32_t v) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	r->ocm1pe = v;
}

static inline uint32_t pwm_ll_get_regc0_ocm1pe(void) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	return r->ocm1pe;
}

static inline void pwm_ll_set_regc0_arpe3(uint32_t v) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	r->arpe3 = v;
}

static inline uint32_t pwm_ll_get_regc0_arpe3(void) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	return r->arpe3;
}

static inline void pwm_ll_set_regc0_arpe2(uint32_t v) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	r->arpe2 = v;
}

static inline uint32_t pwm_ll_get_regc0_arpe2(void) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	return r->arpe2;
}

static inline void pwm_ll_set_regc0_arpe1(uint32_t v) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	r->arpe1 = v;
}

static inline uint32_t pwm_ll_get_regc0_arpe1(void) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	return r->arpe1;
}

static inline void pwm_ll_set_regc0_oc3pe(uint32_t v) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	r->oc3pe = v;
}

static inline uint32_t pwm_ll_get_regc0_oc3pe(void) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	return r->oc3pe;
}

static inline void pwm_ll_set_regc0_oc2pe(uint32_t v) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	r->oc2pe = v;
}

static inline uint32_t pwm_ll_get_regc0_oc2pe(void) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	return r->oc2pe;
}

static inline void pwm_ll_set_regc0_oc1pe(uint32_t v) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	r->oc1pe = v;
}

static inline uint32_t pwm_ll_get_regc0_oc1pe(void) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	return r->oc1pe;
}

static inline void pwm_ll_set_regc0_reserved_9_17(uint32_t v) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	r->reserved_9_17 = v;
}

static inline uint32_t pwm_ll_get_regc0_reserved_9_17(void) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	return r->reserved_9_17;
}

static inline void pwm_ll_set_regc0_cms_tim3(uint32_t v) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	r->cms_tim3 = v;
}

static inline uint32_t pwm_ll_get_regc0_cms_tim3(void) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	return r->cms_tim3;
}

static inline void pwm_ll_set_regc0_cms_tim2(uint32_t v) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	r->cms_tim2 = v;
}

static inline uint32_t pwm_ll_get_regc0_cms_tim2(void) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	return r->cms_tim2;
}

static inline void pwm_ll_set_regc0_cms_tim1(uint32_t v) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	r->cms_tim1 = v;
}

static inline uint32_t pwm_ll_get_regc0_cms_tim1(void) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	return r->cms_tim1;
}

static inline void pwm_ll_set_regc0_reserved_24_25(uint32_t v) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	r->reserved_24_25 = v;
}

static inline uint32_t pwm_ll_get_regc0_reserved_24_25(void) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	return r->reserved_24_25;
}

static inline void pwm_ll_set_regc0_urs3(uint32_t v) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	r->urs3 = v;
}

static inline uint32_t pwm_ll_get_regc0_urs3(void) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	return r->urs3;
}

static inline void pwm_ll_set_regc0_urs2(uint32_t v) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	r->urs2 = v;
}

static inline uint32_t pwm_ll_get_regc0_urs2(void) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	return r->urs2;
}

static inline void pwm_ll_set_regc0_urs1(uint32_t v) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	r->urs1 = v;
}

static inline uint32_t pwm_ll_get_regc0_urs1(void) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	return r->urs1;
}

static inline void pwm_ll_set_regc0_dir_tim3(uint32_t v) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	r->dir_tim3 = v;
}

static inline uint32_t pwm_ll_get_regc0_dir_tim3(void) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	return r->dir_tim3;
}

static inline void pwm_ll_set_regc0_dir_tim2(uint32_t v) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	r->dir_tim2 = v;
}

static inline uint32_t pwm_ll_get_regc0_dir_tim2(void) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	return r->dir_tim2;
}

static inline void pwm_ll_set_regc0_dir_tim1(uint32_t v) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	r->dir_tim1 = v;
}

static inline uint32_t pwm_ll_get_regc0_dir_tim1(void) {
	pwm_regc0_t *r = (pwm_regc0_t*)(SOC_PWM_REG_BASE + (0xc0 << 2));
	return r->dir_tim1;
}

//reg regc1:

static inline void pwm_ll_set_regc1_value(uint32_t v) {
	pwm_regc1_t *r = (pwm_regc1_t*)(SOC_PWM_REG_BASE + (0xc1 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regc1_value(void) {
	pwm_regc1_t *r = (pwm_regc1_t*)(SOC_PWM_REG_BASE + (0xc1 << 2));
	return r->v;
}

static inline void pwm_ll_set_regc1_reserved_0_7(uint32_t v) {
	pwm_regc1_t *r = (pwm_regc1_t*)(SOC_PWM_REG_BASE + (0xc1 << 2));
	r->reserved_0_7 = v;
}

static inline uint32_t pwm_ll_get_regc1_reserved_0_7(void) {
	pwm_regc1_t *r = (pwm_regc1_t*)(SOC_PWM_REG_BASE + (0xc1 << 2));
	return r->reserved_0_7;
}

static inline void pwm_ll_set_regc1_com_mode(uint32_t v) {
	pwm_regc1_t *r = (pwm_regc1_t*)(SOC_PWM_REG_BASE + (0xc1 << 2));
	r->com_mode = v;
}

static inline uint32_t pwm_ll_get_regc1_com_mode(void) {
	pwm_regc1_t *r = (pwm_regc1_t*)(SOC_PWM_REG_BASE + (0xc1 << 2));
	return r->com_mode;
}

static inline void pwm_ll_set_regc1_ccpc(uint32_t v) {
	pwm_regc1_t *r = (pwm_regc1_t*)(SOC_PWM_REG_BASE + (0xc1 << 2));
	r->ccpc = v;
}

static inline uint32_t pwm_ll_get_regc1_ccpc(void) {
	pwm_regc1_t *r = (pwm_regc1_t*)(SOC_PWM_REG_BASE + (0xc1 << 2));
	return r->ccpc;
}

static inline void pwm_ll_set_regc1_trios3(uint32_t v) {
	pwm_regc1_t *r = (pwm_regc1_t*)(SOC_PWM_REG_BASE + (0xc1 << 2));
	r->trios3 = v;
}

static inline uint32_t pwm_ll_get_regc1_trios3(void) {
	pwm_regc1_t *r = (pwm_regc1_t*)(SOC_PWM_REG_BASE + (0xc1 << 2));
	return r->trios3;
}

static inline void pwm_ll_set_regc1_adcs3(uint32_t v) {
	pwm_regc1_t *r = (pwm_regc1_t*)(SOC_PWM_REG_BASE + (0xc1 << 2));
	r->adcs3 = v;
}

static inline uint32_t pwm_ll_get_regc1_adcs3(void) {
	pwm_regc1_t *r = (pwm_regc1_t*)(SOC_PWM_REG_BASE + (0xc1 << 2));
	return r->adcs3;
}

static inline void pwm_ll_set_regc1_reserved_16_16(uint32_t v) {
	pwm_regc1_t *r = (pwm_regc1_t*)(SOC_PWM_REG_BASE + (0xc1 << 2));
	r->reserved_16_16 = v;
}

static inline uint32_t pwm_ll_get_regc1_reserved_16_16(void) {
	pwm_regc1_t *r = (pwm_regc1_t*)(SOC_PWM_REG_BASE + (0xc1 << 2));
	return r->reserved_16_16;
}

static inline void pwm_ll_set_regc1_ccus(uint32_t v) {
	pwm_regc1_t *r = (pwm_regc1_t*)(SOC_PWM_REG_BASE + (0xc1 << 2));
	r->ccus = v;
}

static inline uint32_t pwm_ll_get_regc1_ccus(void) {
	pwm_regc1_t *r = (pwm_regc1_t*)(SOC_PWM_REG_BASE + (0xc1 << 2));
	return r->ccus;
}

static inline void pwm_ll_set_regc1_trios2(uint32_t v) {
	pwm_regc1_t *r = (pwm_regc1_t*)(SOC_PWM_REG_BASE + (0xc1 << 2));
	r->trios2 = v;
}

static inline uint32_t pwm_ll_get_regc1_trios2(void) {
	pwm_regc1_t *r = (pwm_regc1_t*)(SOC_PWM_REG_BASE + (0xc1 << 2));
	return r->trios2;
}

static inline void pwm_ll_set_regc1_adcs2(uint32_t v) {
	pwm_regc1_t *r = (pwm_regc1_t*)(SOC_PWM_REG_BASE + (0xc1 << 2));
	r->adcs2 = v;
}

static inline uint32_t pwm_ll_get_regc1_adcs2(void) {
	pwm_regc1_t *r = (pwm_regc1_t*)(SOC_PWM_REG_BASE + (0xc1 << 2));
	return r->adcs2;
}

static inline void pwm_ll_set_regc1_reserved_24_25(uint32_t v) {
	pwm_regc1_t *r = (pwm_regc1_t*)(SOC_PWM_REG_BASE + (0xc1 << 2));
	r->reserved_24_25 = v;
}

static inline uint32_t pwm_ll_get_regc1_reserved_24_25(void) {
	pwm_regc1_t *r = (pwm_regc1_t*)(SOC_PWM_REG_BASE + (0xc1 << 2));
	return r->reserved_24_25;
}

static inline void pwm_ll_set_regc1_trios1(uint32_t v) {
	pwm_regc1_t *r = (pwm_regc1_t*)(SOC_PWM_REG_BASE + (0xc1 << 2));
	r->trios1 = v;
}

static inline uint32_t pwm_ll_get_regc1_trios1(void) {
	pwm_regc1_t *r = (pwm_regc1_t*)(SOC_PWM_REG_BASE + (0xc1 << 2));
	return r->trios1;
}

static inline void pwm_ll_set_regc1_adcs1(uint32_t v) {
	pwm_regc1_t *r = (pwm_regc1_t*)(SOC_PWM_REG_BASE + (0xc1 << 2));
	r->adcs1 = v;
}

static inline uint32_t pwm_ll_get_regc1_adcs1(void) {
	pwm_regc1_t *r = (pwm_regc1_t*)(SOC_PWM_REG_BASE + (0xc1 << 2));
	return r->adcs1;
}

//reg regc2:

static inline void pwm_ll_set_regc2_value(uint32_t v) {
	pwm_regc2_t *r = (pwm_regc2_t*)(SOC_PWM_REG_BASE + (0xc2 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regc2_value(void) {
	pwm_regc2_t *r = (pwm_regc2_t*)(SOC_PWM_REG_BASE + (0xc2 << 2));
	return r->v;
}

static inline void pwm_ll_set_regc2_sms1(uint32_t v) {
	pwm_regc2_t *r = (pwm_regc2_t*)(SOC_PWM_REG_BASE + (0xc2 << 2));
	r->sms1 = v;
}

static inline uint32_t pwm_ll_get_regc2_sms1(void) {
	pwm_regc2_t *r = (pwm_regc2_t*)(SOC_PWM_REG_BASE + (0xc2 << 2));
	return r->sms1;
}

static inline void pwm_ll_set_regc2_sms2(uint32_t v) {
	pwm_regc2_t *r = (pwm_regc2_t*)(SOC_PWM_REG_BASE + (0xc2 << 2));
	r->sms2 = v;
}

static inline uint32_t pwm_ll_get_regc2_sms2(void) {
	pwm_regc2_t *r = (pwm_regc2_t*)(SOC_PWM_REG_BASE + (0xc2 << 2));
	return r->sms2;
}

static inline void pwm_ll_set_regc2_sms3(uint32_t v) {
	pwm_regc2_t *r = (pwm_regc2_t*)(SOC_PWM_REG_BASE + (0xc2 << 2));
	r->sms3 = v;
}

static inline uint32_t pwm_ll_get_regc2_sms3(void) {
	pwm_regc2_t *r = (pwm_regc2_t*)(SOC_PWM_REG_BASE + (0xc2 << 2));
	return r->sms3;
}

static inline void pwm_ll_set_regc2_ts1(uint32_t v) {
	pwm_regc2_t *r = (pwm_regc2_t*)(SOC_PWM_REG_BASE + (0xc2 << 2));
	r->ts1 = v;
}

static inline uint32_t pwm_ll_get_regc2_ts1(void) {
	pwm_regc2_t *r = (pwm_regc2_t*)(SOC_PWM_REG_BASE + (0xc2 << 2));
	return r->ts1;
}

static inline void pwm_ll_set_regc2_reserved_15_15(uint32_t v) {
	pwm_regc2_t *r = (pwm_regc2_t*)(SOC_PWM_REG_BASE + (0xc2 << 2));
	r->reserved_15_15 = v;
}

static inline uint32_t pwm_ll_get_regc2_reserved_15_15(void) {
	pwm_regc2_t *r = (pwm_regc2_t*)(SOC_PWM_REG_BASE + (0xc2 << 2));
	return r->reserved_15_15;
}

static inline void pwm_ll_set_regc2_ts2(uint32_t v) {
	pwm_regc2_t *r = (pwm_regc2_t*)(SOC_PWM_REG_BASE + (0xc2 << 2));
	r->ts2 = v;
}

static inline uint32_t pwm_ll_get_regc2_ts2(void) {
	pwm_regc2_t *r = (pwm_regc2_t*)(SOC_PWM_REG_BASE + (0xc2 << 2));
	return r->ts2;
}

static inline void pwm_ll_set_regc2_ts3(uint32_t v) {
	pwm_regc2_t *r = (pwm_regc2_t*)(SOC_PWM_REG_BASE + (0xc2 << 2));
	r->ts3 = v;
}

static inline uint32_t pwm_ll_get_regc2_ts3(void) {
	pwm_regc2_t *r = (pwm_regc2_t*)(SOC_PWM_REG_BASE + (0xc2 << 2));
	return r->ts3;
}

static inline void pwm_ll_set_regc2_reserved_22_31(uint32_t v) {
	pwm_regc2_t *r = (pwm_regc2_t*)(SOC_PWM_REG_BASE + (0xc2 << 2));
	r->reserved_22_31 = v;
}

static inline uint32_t pwm_ll_get_regc2_reserved_22_31(void) {
	pwm_regc2_t *r = (pwm_regc2_t*)(SOC_PWM_REG_BASE + (0xc2 << 2));
	return r->reserved_22_31;
}

//reg regc3:

static inline void pwm_ll_set_regc3_value(uint32_t v) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regc3_value(void) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	return r->v;
}

static inline void pwm_ll_set_regc3_cc1ie(uint32_t v) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	r->cc1ie = v;
}

static inline uint32_t pwm_ll_get_regc3_cc1ie(void) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	return r->cc1ie;
}

static inline void pwm_ll_set_regc3_cc2ie(uint32_t v) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	r->cc2ie = v;
}

static inline uint32_t pwm_ll_get_regc3_cc2ie(void) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	return r->cc2ie;
}

static inline void pwm_ll_set_regc3_cc3ie(uint32_t v) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	r->cc3ie = v;
}

static inline uint32_t pwm_ll_get_regc3_cc3ie(void) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	return r->cc3ie;
}

static inline void pwm_ll_set_regc3_cc4ie(uint32_t v) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	r->cc4ie = v;
}

static inline uint32_t pwm_ll_get_regc3_cc4ie(void) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	return r->cc4ie;
}

static inline void pwm_ll_set_regc3_cc5ie(uint32_t v) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	r->cc5ie = v;
}

static inline uint32_t pwm_ll_get_regc3_cc5ie(void) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	return r->cc5ie;
}

static inline void pwm_ll_set_regc3_cc6ie(uint32_t v) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	r->cc6ie = v;
}

static inline uint32_t pwm_ll_get_regc3_cc6ie(void) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	return r->cc6ie;
}

static inline void pwm_ll_set_regc3_cc7ie(uint32_t v) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	r->cc7ie = v;
}

static inline uint32_t pwm_ll_get_regc3_cc7ie(void) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	return r->cc7ie;
}

static inline void pwm_ll_set_regc3_cc8ie(uint32_t v) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	r->cc8ie = v;
}

static inline uint32_t pwm_ll_get_regc3_cc8ie(void) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	return r->cc8ie;
}

static inline void pwm_ll_set_regc3_cc9ie(uint32_t v) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	r->cc9ie = v;
}

static inline uint32_t pwm_ll_get_regc3_cc9ie(void) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	return r->cc9ie;
}

static inline void pwm_ll_set_regc3_uie1(uint32_t v) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	r->uie1 = v;
}

static inline uint32_t pwm_ll_get_regc3_uie1(void) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	return r->uie1;
}

static inline void pwm_ll_set_regc3_uie2(uint32_t v) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	r->uie2 = v;
}

static inline uint32_t pwm_ll_get_regc3_uie2(void) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	return r->uie2;
}

static inline void pwm_ll_set_regc3_uie3(uint32_t v) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	r->uie3 = v;
}

static inline uint32_t pwm_ll_get_regc3_uie3(void) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	return r->uie3;
}

static inline void pwm_ll_set_regc3_tie1(uint32_t v) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	r->tie1 = v;
}

static inline uint32_t pwm_ll_get_regc3_tie1(void) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	return r->tie1;
}

static inline void pwm_ll_set_regc3_tie2(uint32_t v) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	r->tie2 = v;
}

static inline uint32_t pwm_ll_get_regc3_tie2(void) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	return r->tie2;
}

static inline void pwm_ll_set_regc3_tie3(uint32_t v) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	r->tie3 = v;
}

static inline uint32_t pwm_ll_get_regc3_tie3(void) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	return r->tie3;
}

static inline void pwm_ll_set_regc3_comie(uint32_t v) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	r->comie = v;
}

static inline uint32_t pwm_ll_get_regc3_comie(void) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	return r->comie;
}

static inline void pwm_ll_set_regc3_reserved_16_19(uint32_t v) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	r->reserved_16_19 = v;
}

static inline uint32_t pwm_ll_get_regc3_reserved_16_19(void) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	return r->reserved_16_19;
}

static inline void pwm_ll_set_regc3_df_tim1(uint32_t v) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	r->df_tim1 = v;
}

static inline uint32_t pwm_ll_get_regc3_df_tim1(void) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	return r->df_tim1;
}

static inline void pwm_ll_set_regc3_df_tim2(uint32_t v) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	r->df_tim2 = v;
}

static inline uint32_t pwm_ll_get_regc3_df_tim2(void) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	return r->df_tim2;
}

static inline void pwm_ll_set_regc3_df_tim3(uint32_t v) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	r->df_tim3 = v;
}

static inline uint32_t pwm_ll_get_regc3_df_tim3(void) {
	pwm_regc3_t *r = (pwm_regc3_t*)(SOC_PWM_REG_BASE + (0xc3 << 2));
	return r->df_tim3;
}

//reg regc4:

static inline void pwm_ll_set_regc4_value(uint32_t v) {
	pwm_regc4_t *r = (pwm_regc4_t*)(SOC_PWM_REG_BASE + (0xc4 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regc4_value(void) {
	pwm_regc4_t *r = (pwm_regc4_t*)(SOC_PWM_REG_BASE + (0xc4 << 2));
	return r->v;
}

static inline void pwm_ll_set_regc4_cc1if(uint32_t v) {
	pwm_regc4_t *r = (pwm_regc4_t*)(SOC_PWM_REG_BASE + (0xc4 << 2));
	r->cc1if = v;
}

static inline void pwm_ll_set_regc4_cc2if(uint32_t v) {
	pwm_regc4_t *r = (pwm_regc4_t*)(SOC_PWM_REG_BASE + (0xc4 << 2));
	r->cc2if = v;
}

static inline void pwm_ll_set_regc4_cc3if(uint32_t v) {
	pwm_regc4_t *r = (pwm_regc4_t*)(SOC_PWM_REG_BASE + (0xc4 << 2));
	r->cc3if = v;
}

static inline void pwm_ll_set_regc4_cc4if(uint32_t v) {
	pwm_regc4_t *r = (pwm_regc4_t*)(SOC_PWM_REG_BASE + (0xc4 << 2));
	r->cc4if = v;
}

static inline void pwm_ll_set_regc4_cc5if(uint32_t v) {
	pwm_regc4_t *r = (pwm_regc4_t*)(SOC_PWM_REG_BASE + (0xc4 << 2));
	r->cc5if = v;
}

static inline void pwm_ll_set_regc4_cc6if(uint32_t v) {
	pwm_regc4_t *r = (pwm_regc4_t*)(SOC_PWM_REG_BASE + (0xc4 << 2));
	r->cc6if = v;
}

static inline void pwm_ll_set_regc4_cc7if(uint32_t v) {
	pwm_regc4_t *r = (pwm_regc4_t*)(SOC_PWM_REG_BASE + (0xc4 << 2));
	r->cc7if = v;
}

static inline void pwm_ll_set_regc4_cc8if(uint32_t v) {
	pwm_regc4_t *r = (pwm_regc4_t*)(SOC_PWM_REG_BASE + (0xc4 << 2));
	r->cc8if = v;
}

static inline void pwm_ll_set_regc4_cc9if(uint32_t v) {
	pwm_regc4_t *r = (pwm_regc4_t*)(SOC_PWM_REG_BASE + (0xc4 << 2));
	r->cc9if = v;
}

static inline void pwm_ll_set_regc4_uif1(uint32_t v) {
	pwm_regc4_t *r = (pwm_regc4_t*)(SOC_PWM_REG_BASE + (0xc4 << 2));
	r->uif1 = v;
}

static inline void pwm_ll_set_regc4_uif2(uint32_t v) {
	pwm_regc4_t *r = (pwm_regc4_t*)(SOC_PWM_REG_BASE + (0xc4 << 2));
	r->uif2 = v;
}

static inline void pwm_ll_set_regc4_uif3(uint32_t v) {
	pwm_regc4_t *r = (pwm_regc4_t*)(SOC_PWM_REG_BASE + (0xc4 << 2));
	r->uif3 = v;
}

static inline void pwm_ll_set_regc4_tif1(uint32_t v) {
	pwm_regc4_t *r = (pwm_regc4_t*)(SOC_PWM_REG_BASE + (0xc4 << 2));
	r->tif1 = v;
}

static inline void pwm_ll_set_regc4_tif2(uint32_t v) {
	pwm_regc4_t *r = (pwm_regc4_t*)(SOC_PWM_REG_BASE + (0xc4 << 2));
	r->tif2 = v;
}

static inline void pwm_ll_set_regc4_tif3(uint32_t v) {
	pwm_regc4_t *r = (pwm_regc4_t*)(SOC_PWM_REG_BASE + (0xc4 << 2));
	r->tif3 = v;
}

static inline void pwm_ll_set_regc4_comif(uint32_t v) {
	pwm_regc4_t *r = (pwm_regc4_t*)(SOC_PWM_REG_BASE + (0xc4 << 2));
	r->comif = v;
}

static inline void pwm_ll_set_regc4_ch1eif(uint32_t v) {
	pwm_regc4_t *r = (pwm_regc4_t*)(SOC_PWM_REG_BASE + (0xc4 << 2));
	r->ch1eif = v;
}

static inline uint32_t pwm_ll_get_regc4_ch1eif(void) {
	pwm_regc4_t *r = (pwm_regc4_t*)(SOC_PWM_REG_BASE + (0xc4 << 2));
	return r->ch1eif;
}

static inline void pwm_ll_set_regc4_ch2eif(uint32_t v) {
	pwm_regc4_t *r = (pwm_regc4_t*)(SOC_PWM_REG_BASE + (0xc4 << 2));
	r->ch2eif = v;
}

static inline uint32_t pwm_ll_get_regc4_ch2eif(void) {
	pwm_regc4_t *r = (pwm_regc4_t*)(SOC_PWM_REG_BASE + (0xc4 << 2));
	return r->ch2eif;
}

static inline void pwm_ll_set_regc4_ch3eif(uint32_t v) {
	pwm_regc4_t *r = (pwm_regc4_t*)(SOC_PWM_REG_BASE + (0xc4 << 2));
	r->ch3eif = v;
}

static inline uint32_t pwm_ll_get_regc4_ch3eif(void) {
	pwm_regc4_t *r = (pwm_regc4_t*)(SOC_PWM_REG_BASE + (0xc4 << 2));
	return r->ch3eif;
}

static inline void pwm_ll_set_regc4_ch4eif(uint32_t v) {
	pwm_regc4_t *r = (pwm_regc4_t*)(SOC_PWM_REG_BASE + (0xc4 << 2));
	r->ch4eif = v;
}

static inline uint32_t pwm_ll_get_regc4_ch4eif(void) {
	pwm_regc4_t *r = (pwm_regc4_t*)(SOC_PWM_REG_BASE + (0xc4 << 2));
	return r->ch4eif;
}

static inline void pwm_ll_set_regc4_ch5eif(uint32_t v) {
	pwm_regc4_t *r = (pwm_regc4_t*)(SOC_PWM_REG_BASE + (0xc4 << 2));
	r->ch5eif = v;
}

static inline uint32_t pwm_ll_get_regc4_ch5eif(void) {
	pwm_regc4_t *r = (pwm_regc4_t*)(SOC_PWM_REG_BASE + (0xc4 << 2));
	return r->ch5eif;
}

static inline void pwm_ll_set_regc4_ch6eif(uint32_t v) {
	pwm_regc4_t *r = (pwm_regc4_t*)(SOC_PWM_REG_BASE + (0xc4 << 2));
	r->ch6eif = v;
}

static inline uint32_t pwm_ll_get_regc4_ch6eif(void) {
	pwm_regc4_t *r = (pwm_regc4_t*)(SOC_PWM_REG_BASE + (0xc4 << 2));
	return r->ch6eif;
}

static inline void pwm_ll_set_regc4_reserved_22_31(uint32_t v) {
	pwm_regc4_t *r = (pwm_regc4_t*)(SOC_PWM_REG_BASE + (0xc4 << 2));
	r->reserved_22_31 = v;
}

static inline uint32_t pwm_ll_get_regc4_reserved_22_31(void) {
	pwm_regc4_t *r = (pwm_regc4_t*)(SOC_PWM_REG_BASE + (0xc4 << 2));
	return r->reserved_22_31;
}

//reg regc5:

static inline void pwm_ll_set_regc5_value(uint32_t v) {
	pwm_regc5_t *r = (pwm_regc5_t*)(SOC_PWM_REG_BASE + (0xc5 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regc5_value(void) {
	pwm_regc5_t *r = (pwm_regc5_t*)(SOC_PWM_REG_BASE + (0xc5 << 2));
	return r->v;
}

static inline void pwm_ll_set_regc5_reserved_0_8(uint32_t v) {
	pwm_regc5_t *r = (pwm_regc5_t*)(SOC_PWM_REG_BASE + (0xc5 << 2));
	r->reserved_0_8 = v;
}

static inline uint32_t pwm_ll_get_regc5_reserved_0_8(void) {
	pwm_regc5_t *r = (pwm_regc5_t*)(SOC_PWM_REG_BASE + (0xc5 << 2));
	return r->reserved_0_8;
}

static inline void pwm_ll_set_regc5_ug1(uint32_t v) {
	pwm_regc5_t *r = (pwm_regc5_t*)(SOC_PWM_REG_BASE + (0xc5 << 2));
	r->ug1 = v;
}

static inline void pwm_ll_set_regc5_ug2(uint32_t v) {
	pwm_regc5_t *r = (pwm_regc5_t*)(SOC_PWM_REG_BASE + (0xc5 << 2));
	r->ug2 = v;
}

static inline void pwm_ll_set_regc5_ug3(uint32_t v) {
	pwm_regc5_t *r = (pwm_regc5_t*)(SOC_PWM_REG_BASE + (0xc5 << 2));
	r->ug3 = v;
}

static inline void pwm_ll_set_regc5_tg1(uint32_t v) {
	pwm_regc5_t *r = (pwm_regc5_t*)(SOC_PWM_REG_BASE + (0xc5 << 2));
	r->tg1 = v;
}

static inline void pwm_ll_set_regc5_tg2(uint32_t v) {
	pwm_regc5_t *r = (pwm_regc5_t*)(SOC_PWM_REG_BASE + (0xc5 << 2));
	r->tg2 = v;
}

static inline void pwm_ll_set_regc5_tg3(uint32_t v) {
	pwm_regc5_t *r = (pwm_regc5_t*)(SOC_PWM_REG_BASE + (0xc5 << 2));
	r->tg3 = v;
}

static inline void pwm_ll_set_regc5_comg(uint32_t v) {
	pwm_regc5_t *r = (pwm_regc5_t*)(SOC_PWM_REG_BASE + (0xc5 << 2));
	r->comg = v;
}

static inline void pwm_ll_set_regc5_dtm1(uint32_t v) {
	pwm_regc5_t *r = (pwm_regc5_t*)(SOC_PWM_REG_BASE + (0xc5 << 2));
	r->dtm1 = v;
}

static inline uint32_t pwm_ll_get_regc5_dtm1(void) {
	pwm_regc5_t *r = (pwm_regc5_t*)(SOC_PWM_REG_BASE + (0xc5 << 2));
	return r->dtm1;
}

static inline void pwm_ll_set_regc5_dtm2(uint32_t v) {
	pwm_regc5_t *r = (pwm_regc5_t*)(SOC_PWM_REG_BASE + (0xc5 << 2));
	r->dtm2 = v;
}

static inline uint32_t pwm_ll_get_regc5_dtm2(void) {
	pwm_regc5_t *r = (pwm_regc5_t*)(SOC_PWM_REG_BASE + (0xc5 << 2));
	return r->dtm2;
}

static inline void pwm_ll_set_regc5_dtm3(uint32_t v) {
	pwm_regc5_t *r = (pwm_regc5_t*)(SOC_PWM_REG_BASE + (0xc5 << 2));
	r->dtm3 = v;
}

static inline uint32_t pwm_ll_get_regc5_dtm3(void) {
	pwm_regc5_t *r = (pwm_regc5_t*)(SOC_PWM_REG_BASE + (0xc5 << 2));
	return r->dtm3;
}

static inline void pwm_ll_set_regc5_init_level1(uint32_t v) {
	pwm_regc5_t *r = (pwm_regc5_t*)(SOC_PWM_REG_BASE + (0xc5 << 2));
	r->init_level1 = v;
}

static inline uint32_t pwm_ll_get_regc5_init_level1(void) {
	pwm_regc5_t *r = (pwm_regc5_t*)(SOC_PWM_REG_BASE + (0xc5 << 2));
	return r->init_level1;
}

static inline void pwm_ll_set_regc5_init_level2(uint32_t v) {
	pwm_regc5_t *r = (pwm_regc5_t*)(SOC_PWM_REG_BASE + (0xc5 << 2));
	r->init_level2 = v;
}

static inline uint32_t pwm_ll_get_regc5_init_level2(void) {
	pwm_regc5_t *r = (pwm_regc5_t*)(SOC_PWM_REG_BASE + (0xc5 << 2));
	return r->init_level2;
}

static inline void pwm_ll_set_regc5_init_level3(uint32_t v) {
	pwm_regc5_t *r = (pwm_regc5_t*)(SOC_PWM_REG_BASE + (0xc5 << 2));
	r->init_level3 = v;
}

static inline uint32_t pwm_ll_get_regc5_init_level3(void) {
	pwm_regc5_t *r = (pwm_regc5_t*)(SOC_PWM_REG_BASE + (0xc5 << 2));
	return r->init_level3;
}

static inline void pwm_ll_set_regc5_reserved_25_31(uint32_t v) {
	pwm_regc5_t *r = (pwm_regc5_t*)(SOC_PWM_REG_BASE + (0xc5 << 2));
	r->reserved_25_31 = v;
}

static inline uint32_t pwm_ll_get_regc5_reserved_25_31(void) {
	pwm_regc5_t *r = (pwm_regc5_t*)(SOC_PWM_REG_BASE + (0xc5 << 2));
	return r->reserved_25_31;
}

//reg regc6:

static inline void pwm_ll_set_regc6_value(uint32_t v) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regc6_value(void) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	return r->v;
}

static inline void pwm_ll_set_regc6_ch1p(uint32_t v) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	r->ch1p = v;
}

static inline uint32_t pwm_ll_get_regc6_ch1p(void) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	return r->ch1p;
}

static inline void pwm_ll_set_regc6_ch2p(uint32_t v) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	r->ch2p = v;
}

static inline uint32_t pwm_ll_get_regc6_ch2p(void) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	return r->ch2p;
}

static inline void pwm_ll_set_regc6_ch3p(uint32_t v) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	r->ch3p = v;
}

static inline uint32_t pwm_ll_get_regc6_ch3p(void) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	return r->ch3p;
}

static inline void pwm_ll_set_regc6_ch4p(uint32_t v) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	r->ch4p = v;
}

static inline uint32_t pwm_ll_get_regc6_ch4p(void) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	return r->ch4p;
}

static inline void pwm_ll_set_regc6_ch5p(uint32_t v) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	r->ch5p = v;
}

static inline uint32_t pwm_ll_get_regc6_ch5p(void) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	return r->ch5p;
}

static inline void pwm_ll_set_regc6_ch6p(uint32_t v) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	r->ch6p = v;
}

static inline uint32_t pwm_ll_get_regc6_ch6p(void) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	return r->ch6p;
}

static inline void pwm_ll_set_regc6_ch1e(uint32_t v) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	r->ch1e = v;
}

static inline uint32_t pwm_ll_get_regc6_ch1e(void) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	return r->ch1e;
}

static inline void pwm_ll_set_regc6_ch2e(uint32_t v) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	r->ch2e = v;
}

static inline uint32_t pwm_ll_get_regc6_ch2e(void) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	return r->ch2e;
}

static inline void pwm_ll_set_regc6_ch3e(uint32_t v) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	r->ch3e = v;
}

static inline uint32_t pwm_ll_get_regc6_ch3e(void) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	return r->ch3e;
}

static inline void pwm_ll_set_regc6_ch4e(uint32_t v) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	r->ch4e = v;
}

static inline uint32_t pwm_ll_get_regc6_ch4e(void) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	return r->ch4e;
}

static inline void pwm_ll_set_regc6_ch5e(uint32_t v) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	r->ch5e = v;
}

static inline uint32_t pwm_ll_get_regc6_ch5e(void) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	return r->ch5e;
}

static inline void pwm_ll_set_regc6_ch6e(uint32_t v) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	r->ch6e = v;
}

static inline uint32_t pwm_ll_get_regc6_ch6e(void) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	return r->ch6e;
}

static inline void pwm_ll_set_regc6_tim1ccm(uint32_t v) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	r->tim1ccm = v;
}

static inline uint32_t pwm_ll_get_regc6_tim1ccm(void) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	return r->tim1ccm;
}

static inline void pwm_ll_set_regc6_tim2ccm(uint32_t v) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	r->tim2ccm = v;
}

static inline uint32_t pwm_ll_get_regc6_tim2ccm(void) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	return r->tim2ccm;
}

static inline void pwm_ll_set_regc6_tim3ccm(uint32_t v) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	r->tim3ccm = v;
}

static inline uint32_t pwm_ll_get_regc6_tim3ccm(void) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	return r->tim3ccm;
}

static inline void pwm_ll_set_regc6_oc1m(uint32_t v) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	r->oc1m = v;
}

static inline uint32_t pwm_ll_get_regc6_oc1m(void) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	return r->oc1m;
}

static inline void pwm_ll_set_regc6_oc2m(uint32_t v) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	r->oc2m = v;
}

static inline uint32_t pwm_ll_get_regc6_oc2m(void) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	return r->oc2m;
}

static inline void pwm_ll_set_regc6_oc3m(uint32_t v) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	r->oc3m = v;
}

static inline uint32_t pwm_ll_get_regc6_oc3m(void) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	return r->oc3m;
}

static inline void pwm_ll_set_regc6_reserved_30_31(uint32_t v) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	r->reserved_30_31 = v;
}

static inline uint32_t pwm_ll_get_regc6_reserved_30_31(void) {
	pwm_regc6_t *r = (pwm_regc6_t*)(SOC_PWM_REG_BASE + (0xc6 << 2));
	return r->reserved_30_31;
}

//reg regc7:

static inline void pwm_ll_set_regc7_value(uint32_t v) {
	pwm_regc7_t *r = (pwm_regc7_t*)(SOC_PWM_REG_BASE + (0xc7 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regc7_value(void) {
	pwm_regc7_t *r = (pwm_regc7_t*)(SOC_PWM_REG_BASE + (0xc7 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_regc7_tim1_cnt(void) {
	pwm_regc7_t *r = (pwm_regc7_t*)(SOC_PWM_REG_BASE + (0xc7 << 2));
	return r->tim1_cnt;
}

//reg regc8:

static inline void pwm_ll_set_regc8_value(uint32_t v) {
	pwm_regc8_t *r = (pwm_regc8_t*)(SOC_PWM_REG_BASE + (0xc8 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regc8_value(void) {
	pwm_regc8_t *r = (pwm_regc8_t*)(SOC_PWM_REG_BASE + (0xc8 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_regc8_tim2_cnt(void) {
	pwm_regc8_t *r = (pwm_regc8_t*)(SOC_PWM_REG_BASE + (0xc8 << 2));
	return r->tim2_cnt;
}

//reg regc9:

static inline void pwm_ll_set_regc9_value(uint32_t v) {
	pwm_regc9_t *r = (pwm_regc9_t*)(SOC_PWM_REG_BASE + (0xc9 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regc9_value(void) {
	pwm_regc9_t *r = (pwm_regc9_t*)(SOC_PWM_REG_BASE + (0xc9 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_regc9_tim3_cnt(void) {
	pwm_regc9_t *r = (pwm_regc9_t*)(SOC_PWM_REG_BASE + (0xc9 << 2));
	return r->tim3_cnt;
}

//reg regca:

static inline void pwm_ll_set_regca_value(uint32_t v) {
	pwm_regca_t *r = (pwm_regca_t*)(SOC_PWM_REG_BASE + (0xca << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regca_value(void) {
	pwm_regca_t *r = (pwm_regca_t*)(SOC_PWM_REG_BASE + (0xca << 2));
	return r->v;
}

static inline void pwm_ll_set_regca_psc1(uint32_t v) {
	pwm_regca_t *r = (pwm_regca_t*)(SOC_PWM_REG_BASE + (0xca << 2));
	r->psc1 = v;
}

static inline uint32_t pwm_ll_get_regca_psc1(void) {
	pwm_regca_t *r = (pwm_regca_t*)(SOC_PWM_REG_BASE + (0xca << 2));
	return r->psc1;
}

static inline void pwm_ll_set_regca_psc2(uint32_t v) {
	pwm_regca_t *r = (pwm_regca_t*)(SOC_PWM_REG_BASE + (0xca << 2));
	r->psc2 = v;
}

static inline uint32_t pwm_ll_get_regca_psc2(void) {
	pwm_regca_t *r = (pwm_regca_t*)(SOC_PWM_REG_BASE + (0xca << 2));
	return r->psc2;
}

static inline void pwm_ll_set_regca_psc3(uint32_t v) {
	pwm_regca_t *r = (pwm_regca_t*)(SOC_PWM_REG_BASE + (0xca << 2));
	r->psc3 = v;
}

static inline uint32_t pwm_ll_get_regca_psc3(void) {
	pwm_regca_t *r = (pwm_regca_t*)(SOC_PWM_REG_BASE + (0xca << 2));
	return r->psc3;
}

static inline void pwm_ll_set_regca_reserved_24_31(uint32_t v) {
	pwm_regca_t *r = (pwm_regca_t*)(SOC_PWM_REG_BASE + (0xca << 2));
	r->reserved_24_31 = v;
}

static inline uint32_t pwm_ll_get_regca_reserved_24_31(void) {
	pwm_regca_t *r = (pwm_regca_t*)(SOC_PWM_REG_BASE + (0xca << 2));
	return r->reserved_24_31;
}

//reg regcb:

static inline void pwm_ll_set_regcb_value(uint32_t v) {
	pwm_regcb_t *r = (pwm_regcb_t*)(SOC_PWM_REG_BASE + (0xcb << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regcb_value(void) {
	pwm_regcb_t *r = (pwm_regcb_t*)(SOC_PWM_REG_BASE + (0xcb << 2));
	return r->v;
}

static inline void pwm_ll_set_regcb_tim1_arr(uint32_t v) {
	pwm_regcb_t *r = (pwm_regcb_t*)(SOC_PWM_REG_BASE + (0xcb << 2));
	r->tim1_arr = v;
}

static inline uint32_t pwm_ll_get_regcb_tim1_arr(void) {
	pwm_regcb_t *r = (pwm_regcb_t*)(SOC_PWM_REG_BASE + (0xcb << 2));
	return r->tim1_arr;
}

//reg regcc:

static inline void pwm_ll_set_regcc_value(uint32_t v) {
	pwm_regcc_t *r = (pwm_regcc_t*)(SOC_PWM_REG_BASE + (0xcc << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regcc_value(void) {
	pwm_regcc_t *r = (pwm_regcc_t*)(SOC_PWM_REG_BASE + (0xcc << 2));
	return r->v;
}

static inline void pwm_ll_set_regcc_tim2_arr(uint32_t v) {
	pwm_regcc_t *r = (pwm_regcc_t*)(SOC_PWM_REG_BASE + (0xcc << 2));
	r->tim2_arr = v;
}

static inline uint32_t pwm_ll_get_regcc_tim2_arr(void) {
	pwm_regcc_t *r = (pwm_regcc_t*)(SOC_PWM_REG_BASE + (0xcc << 2));
	return r->tim2_arr;
}

//reg regcd:

static inline void pwm_ll_set_regcd_value(uint32_t v) {
	pwm_regcd_t *r = (pwm_regcd_t*)(SOC_PWM_REG_BASE + (0xcd << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regcd_value(void) {
	pwm_regcd_t *r = (pwm_regcd_t*)(SOC_PWM_REG_BASE + (0xcd << 2));
	return r->v;
}

static inline void pwm_ll_set_regcd_tim3_arr(uint32_t v) {
	pwm_regcd_t *r = (pwm_regcd_t*)(SOC_PWM_REG_BASE + (0xcd << 2));
	r->tim3_arr = v;
}

static inline uint32_t pwm_ll_get_regcd_tim3_arr(void) {
	pwm_regcd_t *r = (pwm_regcd_t*)(SOC_PWM_REG_BASE + (0xcd << 2));
	return r->tim3_arr;
}

//reg regce:

static inline void pwm_ll_set_regce_value(uint32_t v) {
	pwm_regce_t *r = (pwm_regce_t*)(SOC_PWM_REG_BASE + (0xce << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regce_value(void) {
	pwm_regce_t *r = (pwm_regce_t*)(SOC_PWM_REG_BASE + (0xce << 2));
	return r->v;
}

static inline void pwm_ll_set_regce_tim1_rcr(uint32_t v) {
	pwm_regce_t *r = (pwm_regce_t*)(SOC_PWM_REG_BASE + (0xce << 2));
	r->tim1_rcr = v;
}

static inline uint32_t pwm_ll_get_regce_tim1_rcr(void) {
	pwm_regce_t *r = (pwm_regce_t*)(SOC_PWM_REG_BASE + (0xce << 2));
	return r->tim1_rcr;
}

//reg regcf:

static inline void pwm_ll_set_regcf_value(uint32_t v) {
	pwm_regcf_t *r = (pwm_regcf_t*)(SOC_PWM_REG_BASE + (0xcf << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regcf_value(void) {
	pwm_regcf_t *r = (pwm_regcf_t*)(SOC_PWM_REG_BASE + (0xcf << 2));
	return r->v;
}

static inline void pwm_ll_set_regcf_tim2_rcr(uint32_t v) {
	pwm_regcf_t *r = (pwm_regcf_t*)(SOC_PWM_REG_BASE + (0xcf << 2));
	r->tim2_rcr = v;
}

static inline uint32_t pwm_ll_get_regcf_tim2_rcr(void) {
	pwm_regcf_t *r = (pwm_regcf_t*)(SOC_PWM_REG_BASE + (0xcf << 2));
	return r->tim2_rcr;
}

//reg regd0:

static inline void pwm_ll_set_regd0_value(uint32_t v) {
	pwm_regd0_t *r = (pwm_regd0_t*)(SOC_PWM_REG_BASE + (0xd0 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regd0_value(void) {
	pwm_regd0_t *r = (pwm_regd0_t*)(SOC_PWM_REG_BASE + (0xd0 << 2));
	return r->v;
}

static inline void pwm_ll_set_regd0_tim3_rcr(uint32_t v) {
	pwm_regd0_t *r = (pwm_regd0_t*)(SOC_PWM_REG_BASE + (0xd0 << 2));
	r->tim3_rcr = v;
}

static inline uint32_t pwm_ll_get_regd0_tim3_rcr(void) {
	pwm_regd0_t *r = (pwm_regd0_t*)(SOC_PWM_REG_BASE + (0xd0 << 2));
	return r->tim3_rcr;
}

//reg regd1:

static inline void pwm_ll_set_regd1_value(uint32_t v) {
	pwm_regd1_t *r = (pwm_regd1_t*)(SOC_PWM_REG_BASE + (0xd1 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regd1_value(void) {
	pwm_regd1_t *r = (pwm_regd1_t*)(SOC_PWM_REG_BASE + (0xd1 << 2));
	return r->v;
}

static inline void pwm_ll_set_regd1_ccr1(uint32_t v) {
	pwm_regd1_t *r = (pwm_regd1_t*)(SOC_PWM_REG_BASE + (0xd1 << 2));
	r->ccr1 = v;
}

static inline uint32_t pwm_ll_get_regd1_ccr1(void) {
	pwm_regd1_t *r = (pwm_regd1_t*)(SOC_PWM_REG_BASE + (0xd1 << 2));
	return r->ccr1;
}

//reg regd2:

static inline void pwm_ll_set_regd2_value(uint32_t v) {
	pwm_regd2_t *r = (pwm_regd2_t*)(SOC_PWM_REG_BASE + (0xd2 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regd2_value(void) {
	pwm_regd2_t *r = (pwm_regd2_t*)(SOC_PWM_REG_BASE + (0xd2 << 2));
	return r->v;
}

static inline void pwm_ll_set_regd2_ccr2(uint32_t v) {
	pwm_regd2_t *r = (pwm_regd2_t*)(SOC_PWM_REG_BASE + (0xd2 << 2));
	r->ccr2 = v;
}

static inline uint32_t pwm_ll_get_regd2_ccr2(void) {
	pwm_regd2_t *r = (pwm_regd2_t*)(SOC_PWM_REG_BASE + (0xd2 << 2));
	return r->ccr2;
}

//reg regd3:

static inline void pwm_ll_set_regd3_value(uint32_t v) {
	pwm_regd3_t *r = (pwm_regd3_t*)(SOC_PWM_REG_BASE + (0xd3 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regd3_value(void) {
	pwm_regd3_t *r = (pwm_regd3_t*)(SOC_PWM_REG_BASE + (0xd3 << 2));
	return r->v;
}

static inline void pwm_ll_set_regd3_ccr3(uint32_t v) {
	pwm_regd3_t *r = (pwm_regd3_t*)(SOC_PWM_REG_BASE + (0xd3 << 2));
	r->ccr3 = v;
}

static inline uint32_t pwm_ll_get_regd3_ccr3(void) {
	pwm_regd3_t *r = (pwm_regd3_t*)(SOC_PWM_REG_BASE + (0xd3 << 2));
	return r->ccr3;
}

//reg regd4:

static inline void pwm_ll_set_regd4_value(uint32_t v) {
	pwm_regd4_t *r = (pwm_regd4_t*)(SOC_PWM_REG_BASE + (0xd4 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regd4_value(void) {
	pwm_regd4_t *r = (pwm_regd4_t*)(SOC_PWM_REG_BASE + (0xd4 << 2));
	return r->v;
}

static inline void pwm_ll_set_regd4_ccr4(uint32_t v) {
	pwm_regd4_t *r = (pwm_regd4_t*)(SOC_PWM_REG_BASE + (0xd4 << 2));
	r->ccr4 = v;
}

static inline uint32_t pwm_ll_get_regd4_ccr4(void) {
	pwm_regd4_t *r = (pwm_regd4_t*)(SOC_PWM_REG_BASE + (0xd4 << 2));
	return r->ccr4;
}

//reg regd5:

static inline void pwm_ll_set_regd5_value(uint32_t v) {
	pwm_regd5_t *r = (pwm_regd5_t*)(SOC_PWM_REG_BASE + (0xd5 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regd5_value(void) {
	pwm_regd5_t *r = (pwm_regd5_t*)(SOC_PWM_REG_BASE + (0xd5 << 2));
	return r->v;
}

static inline void pwm_ll_set_regd5_ccr5(uint32_t v) {
	pwm_regd5_t *r = (pwm_regd5_t*)(SOC_PWM_REG_BASE + (0xd5 << 2));
	r->ccr5 = v;
}

static inline uint32_t pwm_ll_get_regd5_ccr5(void) {
	pwm_regd5_t *r = (pwm_regd5_t*)(SOC_PWM_REG_BASE + (0xd5 << 2));
	return r->ccr5;
}

//reg regd6:

static inline void pwm_ll_set_regd6_value(uint32_t v) {
	pwm_regd6_t *r = (pwm_regd6_t*)(SOC_PWM_REG_BASE + (0xd6 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regd6_value(void) {
	pwm_regd6_t *r = (pwm_regd6_t*)(SOC_PWM_REG_BASE + (0xd6 << 2));
	return r->v;
}

static inline void pwm_ll_set_regd6_ccr6(uint32_t v) {
	pwm_regd6_t *r = (pwm_regd6_t*)(SOC_PWM_REG_BASE + (0xd6 << 2));
	r->ccr6 = v;
}

static inline uint32_t pwm_ll_get_regd6_ccr6(void) {
	pwm_regd6_t *r = (pwm_regd6_t*)(SOC_PWM_REG_BASE + (0xd6 << 2));
	return r->ccr6;
}

//reg regd7:

static inline void pwm_ll_set_regd7_value(uint32_t v) {
	pwm_regd7_t *r = (pwm_regd7_t*)(SOC_PWM_REG_BASE + (0xd7 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regd7_value(void) {
	pwm_regd7_t *r = (pwm_regd7_t*)(SOC_PWM_REG_BASE + (0xd7 << 2));
	return r->v;
}

static inline void pwm_ll_set_regd7_ccr7(uint32_t v) {
	pwm_regd7_t *r = (pwm_regd7_t*)(SOC_PWM_REG_BASE + (0xd7 << 2));
	r->ccr7 = v;
}

static inline uint32_t pwm_ll_get_regd7_ccr7(void) {
	pwm_regd7_t *r = (pwm_regd7_t*)(SOC_PWM_REG_BASE + (0xd7 << 2));
	return r->ccr7;
}

//reg regd8:

static inline void pwm_ll_set_regd8_value(uint32_t v) {
	pwm_regd8_t *r = (pwm_regd8_t*)(SOC_PWM_REG_BASE + (0xd8 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regd8_value(void) {
	pwm_regd8_t *r = (pwm_regd8_t*)(SOC_PWM_REG_BASE + (0xd8 << 2));
	return r->v;
}

static inline void pwm_ll_set_regd8_ccr8(uint32_t v) {
	pwm_regd8_t *r = (pwm_regd8_t*)(SOC_PWM_REG_BASE + (0xd8 << 2));
	r->ccr8 = v;
}

static inline uint32_t pwm_ll_get_regd8_ccr8(void) {
	pwm_regd8_t *r = (pwm_regd8_t*)(SOC_PWM_REG_BASE + (0xd8 << 2));
	return r->ccr8;
}

//reg regd9:

static inline void pwm_ll_set_regd9_value(uint32_t v) {
	pwm_regd9_t *r = (pwm_regd9_t*)(SOC_PWM_REG_BASE + (0xd9 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regd9_value(void) {
	pwm_regd9_t *r = (pwm_regd9_t*)(SOC_PWM_REG_BASE + (0xd9 << 2));
	return r->v;
}

static inline void pwm_ll_set_regd9_ccr9(uint32_t v) {
	pwm_regd9_t *r = (pwm_regd9_t*)(SOC_PWM_REG_BASE + (0xd9 << 2));
	r->ccr9 = v;
}

static inline uint32_t pwm_ll_get_regd9_ccr9(void) {
	pwm_regd9_t *r = (pwm_regd9_t*)(SOC_PWM_REG_BASE + (0xd9 << 2));
	return r->ccr9;
}

//reg regda:

static inline void pwm_ll_set_regda_value(uint32_t v) {
	pwm_regda_t *r = (pwm_regda_t*)(SOC_PWM_REG_BASE + (0xda << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regda_value(void) {
	pwm_regda_t *r = (pwm_regda_t*)(SOC_PWM_REG_BASE + (0xda << 2));
	return r->v;
}

static inline void pwm_ll_set_regda_dt1(uint32_t v) {
	pwm_regda_t *r = (pwm_regda_t*)(SOC_PWM_REG_BASE + (0xda << 2));
	r->dt1 = v;
}

static inline uint32_t pwm_ll_get_regda_dt1(void) {
	pwm_regda_t *r = (pwm_regda_t*)(SOC_PWM_REG_BASE + (0xda << 2));
	return r->dt1;
}

static inline void pwm_ll_set_regda_dt2(uint32_t v) {
	pwm_regda_t *r = (pwm_regda_t*)(SOC_PWM_REG_BASE + (0xda << 2));
	r->dt2 = v;
}

static inline uint32_t pwm_ll_get_regda_dt2(void) {
	pwm_regda_t *r = (pwm_regda_t*)(SOC_PWM_REG_BASE + (0xda << 2));
	return r->dt2;
}

static inline void pwm_ll_set_regda_dt3(uint32_t v) {
	pwm_regda_t *r = (pwm_regda_t*)(SOC_PWM_REG_BASE + (0xda << 2));
	r->dt3 = v;
}

static inline uint32_t pwm_ll_get_regda_dt3(void) {
	pwm_regda_t *r = (pwm_regda_t*)(SOC_PWM_REG_BASE + (0xda << 2));
	return r->dt3;
}

static inline void pwm_ll_set_regda_reserved_30_31(uint32_t v) {
	pwm_regda_t *r = (pwm_regda_t*)(SOC_PWM_REG_BASE + (0xda << 2));
	r->reserved_30_31 = v;
}

static inline uint32_t pwm_ll_get_regda_reserved_30_31(void) {
	pwm_regda_t *r = (pwm_regda_t*)(SOC_PWM_REG_BASE + (0xda << 2));
	return r->reserved_30_31;
}

//reg regdb:

static inline void pwm_ll_set_regdb_value(uint32_t v) {
	pwm_regdb_t *r = (pwm_regdb_t*)(SOC_PWM_REG_BASE + (0xdb << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regdb_value(void) {
	pwm_regdb_t *r = (pwm_regdb_t*)(SOC_PWM_REG_BASE + (0xdb << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_regdb_tim1_arr_shad(void) {
	pwm_regdb_t *r = (pwm_regdb_t*)(SOC_PWM_REG_BASE + (0xdb << 2));
	return r->tim1_arr_shad;
}

//reg regdc:

static inline void pwm_ll_set_regdc_value(uint32_t v) {
	pwm_regdc_t *r = (pwm_regdc_t*)(SOC_PWM_REG_BASE + (0xdc << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regdc_value(void) {
	pwm_regdc_t *r = (pwm_regdc_t*)(SOC_PWM_REG_BASE + (0xdc << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_regdc_tim2_arr_shad(void) {
	pwm_regdc_t *r = (pwm_regdc_t*)(SOC_PWM_REG_BASE + (0xdc << 2));
	return r->tim2_arr_shad;
}

//reg regdd:

static inline void pwm_ll_set_regdd_value(uint32_t v) {
	pwm_regdd_t *r = (pwm_regdd_t*)(SOC_PWM_REG_BASE + (0xdd << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regdd_value(void) {
	pwm_regdd_t *r = (pwm_regdd_t*)(SOC_PWM_REG_BASE + (0xdd << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_regdd_tim3_arr_shad(void) {
	pwm_regdd_t *r = (pwm_regdd_t*)(SOC_PWM_REG_BASE + (0xdd << 2));
	return r->tim3_arr_shad;
}

//reg regde:

static inline void pwm_ll_set_regde_value(uint32_t v) {
	pwm_regde_t *r = (pwm_regde_t*)(SOC_PWM_REG_BASE + (0xde << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regde_value(void) {
	pwm_regde_t *r = (pwm_regde_t*)(SOC_PWM_REG_BASE + (0xde << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_regde_tim1_rcr_shad(void) {
	pwm_regde_t *r = (pwm_regde_t*)(SOC_PWM_REG_BASE + (0xde << 2));
	return r->tim1_rcr_shad;
}

//reg regdf:

static inline void pwm_ll_set_regdf_value(uint32_t v) {
	pwm_regdf_t *r = (pwm_regdf_t*)(SOC_PWM_REG_BASE + (0xdf << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regdf_value(void) {
	pwm_regdf_t *r = (pwm_regdf_t*)(SOC_PWM_REG_BASE + (0xdf << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_regdf_tim2_rcr_shad(void) {
	pwm_regdf_t *r = (pwm_regdf_t*)(SOC_PWM_REG_BASE + (0xdf << 2));
	return r->tim2_rcr_shad;
}

//reg rege0:

static inline void pwm_ll_set_rege0_value(uint32_t v) {
	pwm_rege0_t *r = (pwm_rege0_t*)(SOC_PWM_REG_BASE + (0xe0 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_rege0_value(void) {
	pwm_rege0_t *r = (pwm_rege0_t*)(SOC_PWM_REG_BASE + (0xe0 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_rege0_tim3_rcr_shad(void) {
	pwm_rege0_t *r = (pwm_rege0_t*)(SOC_PWM_REG_BASE + (0xe0 << 2));
	return r->tim3_rcr_shad;
}

//reg rege1:

static inline void pwm_ll_set_rege1_value(uint32_t v) {
	pwm_rege1_t *r = (pwm_rege1_t*)(SOC_PWM_REG_BASE + (0xe1 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_rege1_value(void) {
	pwm_rege1_t *r = (pwm_rege1_t*)(SOC_PWM_REG_BASE + (0xe1 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_rege1_ccr1_shad(void) {
	pwm_rege1_t *r = (pwm_rege1_t*)(SOC_PWM_REG_BASE + (0xe1 << 2));
	return r->ccr1_shad;
}

//reg rege2:

static inline void pwm_ll_set_rege2_value(uint32_t v) {
	pwm_rege2_t *r = (pwm_rege2_t*)(SOC_PWM_REG_BASE + (0xe2 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_rege2_value(void) {
	pwm_rege2_t *r = (pwm_rege2_t*)(SOC_PWM_REG_BASE + (0xe2 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_rege2_ccr2_shad(void) {
	pwm_rege2_t *r = (pwm_rege2_t*)(SOC_PWM_REG_BASE + (0xe2 << 2));
	return r->ccr2_shad;
}

//reg rege3:

static inline void pwm_ll_set_rege3_value(uint32_t v) {
	pwm_rege3_t *r = (pwm_rege3_t*)(SOC_PWM_REG_BASE + (0xe3 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_rege3_value(void) {
	pwm_rege3_t *r = (pwm_rege3_t*)(SOC_PWM_REG_BASE + (0xe3 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_rege3_ccr3_shad(void) {
	pwm_rege3_t *r = (pwm_rege3_t*)(SOC_PWM_REG_BASE + (0xe3 << 2));
	return r->ccr3_shad;
}

//reg rege4:

static inline void pwm_ll_set_rege4_value(uint32_t v) {
	pwm_rege4_t *r = (pwm_rege4_t*)(SOC_PWM_REG_BASE + (0xe4 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_rege4_value(void) {
	pwm_rege4_t *r = (pwm_rege4_t*)(SOC_PWM_REG_BASE + (0xe4 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_rege4_ccr4_shad(void) {
	pwm_rege4_t *r = (pwm_rege4_t*)(SOC_PWM_REG_BASE + (0xe4 << 2));
	return r->ccr4_shad;
}

//reg rege5:

static inline void pwm_ll_set_rege5_value(uint32_t v) {
	pwm_rege5_t *r = (pwm_rege5_t*)(SOC_PWM_REG_BASE + (0xe5 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_rege5_value(void) {
	pwm_rege5_t *r = (pwm_rege5_t*)(SOC_PWM_REG_BASE + (0xe5 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_rege5_ccr5_shad(void) {
	pwm_rege5_t *r = (pwm_rege5_t*)(SOC_PWM_REG_BASE + (0xe5 << 2));
	return r->ccr5_shad;
}

//reg rege6:

static inline void pwm_ll_set_rege6_value(uint32_t v) {
	pwm_rege6_t *r = (pwm_rege6_t*)(SOC_PWM_REG_BASE + (0xe6 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_rege6_value(void) {
	pwm_rege6_t *r = (pwm_rege6_t*)(SOC_PWM_REG_BASE + (0xe6 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_rege6_ccr6_shad(void) {
	pwm_rege6_t *r = (pwm_rege6_t*)(SOC_PWM_REG_BASE + (0xe6 << 2));
	return r->ccr6_shad;
}

//reg rege7:

static inline void pwm_ll_set_rege7_value(uint32_t v) {
	pwm_rege7_t *r = (pwm_rege7_t*)(SOC_PWM_REG_BASE + (0xe7 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_rege7_value(void) {
	pwm_rege7_t *r = (pwm_rege7_t*)(SOC_PWM_REG_BASE + (0xe7 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_rege7_ccr7_shad(void) {
	pwm_rege7_t *r = (pwm_rege7_t*)(SOC_PWM_REG_BASE + (0xe7 << 2));
	return r->ccr7_shad;
}

//reg rege8:

static inline void pwm_ll_set_rege8_value(uint32_t v) {
	pwm_rege8_t *r = (pwm_rege8_t*)(SOC_PWM_REG_BASE + (0xe8 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_rege8_value(void) {
	pwm_rege8_t *r = (pwm_rege8_t*)(SOC_PWM_REG_BASE + (0xe8 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_rege8_ccr8_shad(void) {
	pwm_rege8_t *r = (pwm_rege8_t*)(SOC_PWM_REG_BASE + (0xe8 << 2));
	return r->ccr8_shad;
}

//reg rege9:

static inline void pwm_ll_set_rege9_value(uint32_t v) {
	pwm_rege9_t *r = (pwm_rege9_t*)(SOC_PWM_REG_BASE + (0xe9 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_rege9_value(void) {
	pwm_rege9_t *r = (pwm_rege9_t*)(SOC_PWM_REG_BASE + (0xe9 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_rege9_ccr9_shad(void) {
	pwm_rege9_t *r = (pwm_rege9_t*)(SOC_PWM_REG_BASE + (0xe9 << 2));
	return r->ccr9_shad;
}

//reg regea:

static inline void pwm_ll_set_regea_value(uint32_t v) {
	pwm_regea_t *r = (pwm_regea_t*)(SOC_PWM_REG_BASE + (0xea << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regea_value(void) {
	pwm_regea_t *r = (pwm_regea_t*)(SOC_PWM_REG_BASE + (0xea << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_regea_ch6e_shad(void) {
	pwm_regea_t *r = (pwm_regea_t*)(SOC_PWM_REG_BASE + (0xea << 2));
	return r->ch6e_shad;
}

static inline uint32_t pwm_ll_get_regea_ch5e_shad(void) {
	pwm_regea_t *r = (pwm_regea_t*)(SOC_PWM_REG_BASE + (0xea << 2));
	return r->ch5e_shad;
}

static inline uint32_t pwm_ll_get_regea_ch4e_shad(void) {
	pwm_regea_t *r = (pwm_regea_t*)(SOC_PWM_REG_BASE + (0xea << 2));
	return r->ch4e_shad;
}

static inline uint32_t pwm_ll_get_regea_ch3e_shad(void) {
	pwm_regea_t *r = (pwm_regea_t*)(SOC_PWM_REG_BASE + (0xea << 2));
	return r->ch3e_shad;
}

static inline uint32_t pwm_ll_get_regea_ch2e_shad(void) {
	pwm_regea_t *r = (pwm_regea_t*)(SOC_PWM_REG_BASE + (0xea << 2));
	return r->ch2e_shad;
}

static inline uint32_t pwm_ll_get_regea_ch1e_shad(void) {
	pwm_regea_t *r = (pwm_regea_t*)(SOC_PWM_REG_BASE + (0xea << 2));
	return r->ch1e_shad;
}

static inline uint32_t pwm_ll_get_regea_oc1m_shad(void) {
	pwm_regea_t *r = (pwm_regea_t*)(SOC_PWM_REG_BASE + (0xea << 2));
	return r->oc1m_shad;
}

static inline uint32_t pwm_ll_get_regea_oc2m_shad(void) {
	pwm_regea_t *r = (pwm_regea_t*)(SOC_PWM_REG_BASE + (0xea << 2));
	return r->oc2m_shad;
}

static inline uint32_t pwm_ll_get_regea_oc3m_shad(void) {
	pwm_regea_t *r = (pwm_regea_t*)(SOC_PWM_REG_BASE + (0xea << 2));
	return r->oc3m_shad;
}

static inline uint32_t pwm_ll_get_regea_reserved_15_31(void) {
	pwm_regea_t *r = (pwm_regea_t*)(SOC_PWM_REG_BASE + (0xea << 2));
	return r->reserved_15_31;
}

//reg regeb:

static inline void pwm_ll_set_regeb_value(uint32_t v) {
	pwm_regeb_t *r = (pwm_regeb_t*)(SOC_PWM_REG_BASE + (0xeb << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regeb_value(void) {
	pwm_regeb_t *r = (pwm_regeb_t*)(SOC_PWM_REG_BASE + (0xeb << 2));
	return r->v;
}

static inline void pwm_ll_set_regeb_tim1_fade_num(uint32_t v) {
	pwm_regeb_t *r = (pwm_regeb_t*)(SOC_PWM_REG_BASE + (0xeb << 2));
	r->tim1_fade_num = v;
}

static inline uint32_t pwm_ll_get_regeb_tim1_fade_num(void) {
	pwm_regeb_t *r = (pwm_regeb_t*)(SOC_PWM_REG_BASE + (0xeb << 2));
	return r->tim1_fade_num;
}

static inline void pwm_ll_set_regeb_tim1_fade_intval_cyc(uint32_t v) {
	pwm_regeb_t *r = (pwm_regeb_t*)(SOC_PWM_REG_BASE + (0xeb << 2));
	r->tim1_fade_intval_cyc = v;
}

static inline uint32_t pwm_ll_get_regeb_tim1_fade_intval_cyc(void) {
	pwm_regeb_t *r = (pwm_regeb_t*)(SOC_PWM_REG_BASE + (0xeb << 2));
	return r->tim1_fade_intval_cyc;
}

static inline void pwm_ll_set_regeb_tim1_fade_scale(uint32_t v) {
	pwm_regeb_t *r = (pwm_regeb_t*)(SOC_PWM_REG_BASE + (0xeb << 2));
	r->tim1_fade_scale = v;
}

static inline uint32_t pwm_ll_get_regeb_tim1_fade_scale(void) {
	pwm_regeb_t *r = (pwm_regeb_t*)(SOC_PWM_REG_BASE + (0xeb << 2));
	return r->tim1_fade_scale;
}

static inline void pwm_ll_set_regeb_tim1_fade_end_if(uint32_t v) {
	pwm_regeb_t *r = (pwm_regeb_t*)(SOC_PWM_REG_BASE + (0xeb << 2));
	r->tim1_fade_end_if = v;
}

static inline uint32_t pwm_ll_get_regeb_tim1_fade_end_if(void) {
	pwm_regeb_t *r = (pwm_regeb_t*)(SOC_PWM_REG_BASE + (0xeb << 2));
	return r->tim1_fade_end_if;
}

static inline void pwm_ll_set_regeb_tim1_fade_end_int_en(uint32_t v) {
	pwm_regeb_t *r = (pwm_regeb_t*)(SOC_PWM_REG_BASE + (0xeb << 2));
	r->tim1_fade_end_int_en = v;
}

static inline uint32_t pwm_ll_get_regeb_tim1_fade_end_int_en(void) {
	pwm_regeb_t *r = (pwm_regeb_t*)(SOC_PWM_REG_BASE + (0xeb << 2));
	return r->tim1_fade_end_int_en;
}

static inline void pwm_ll_set_regeb_tim1_fade_inc_dec(uint32_t v) {
	pwm_regeb_t *r = (pwm_regeb_t*)(SOC_PWM_REG_BASE + (0xeb << 2));
	r->tim1_fade_inc_dec = v;
}

static inline uint32_t pwm_ll_get_regeb_tim1_fade_inc_dec(void) {
	pwm_regeb_t *r = (pwm_regeb_t*)(SOC_PWM_REG_BASE + (0xeb << 2));
	return r->tim1_fade_inc_dec;
}

static inline void pwm_ll_set_regeb_tim1_duty_fading_en(uint32_t v) {
	pwm_regeb_t *r = (pwm_regeb_t*)(SOC_PWM_REG_BASE + (0xeb << 2));
	r->tim1_duty_fading_en = v;
}

static inline uint32_t pwm_ll_get_regeb_tim1_duty_fading_en(void) {
	pwm_regeb_t *r = (pwm_regeb_t*)(SOC_PWM_REG_BASE + (0xeb << 2));
	return r->tim1_duty_fading_en;
}

//reg regec:

static inline void pwm_ll_set_regec_value(uint32_t v) {
	pwm_regec_t *r = (pwm_regec_t*)(SOC_PWM_REG_BASE + (0xec << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_regec_value(void) {
	pwm_regec_t *r = (pwm_regec_t*)(SOC_PWM_REG_BASE + (0xec << 2));
	return r->v;
}

static inline void pwm_ll_set_regec_tim2_fade_num(uint32_t v) {
	pwm_regec_t *r = (pwm_regec_t*)(SOC_PWM_REG_BASE + (0xec << 2));
	r->tim2_fade_num = v;
}

static inline uint32_t pwm_ll_get_regec_tim2_fade_num(void) {
	pwm_regec_t *r = (pwm_regec_t*)(SOC_PWM_REG_BASE + (0xec << 2));
	return r->tim2_fade_num;
}

static inline void pwm_ll_set_regec_tim2_fade_intval_cyc(uint32_t v) {
	pwm_regec_t *r = (pwm_regec_t*)(SOC_PWM_REG_BASE + (0xec << 2));
	r->tim2_fade_intval_cyc = v;
}

static inline uint32_t pwm_ll_get_regec_tim2_fade_intval_cyc(void) {
	pwm_regec_t *r = (pwm_regec_t*)(SOC_PWM_REG_BASE + (0xec << 2));
	return r->tim2_fade_intval_cyc;
}

static inline void pwm_ll_set_regec_tim2_fade_scale(uint32_t v) {
	pwm_regec_t *r = (pwm_regec_t*)(SOC_PWM_REG_BASE + (0xec << 2));
	r->tim2_fade_scale = v;
}

static inline uint32_t pwm_ll_get_regec_tim2_fade_scale(void) {
	pwm_regec_t *r = (pwm_regec_t*)(SOC_PWM_REG_BASE + (0xec << 2));
	return r->tim2_fade_scale;
}

static inline void pwm_ll_set_regec_tim2_fade_end_if(uint32_t v) {
	pwm_regec_t *r = (pwm_regec_t*)(SOC_PWM_REG_BASE + (0xec << 2));
	r->tim2_fade_end_if = v;
}

static inline uint32_t pwm_ll_get_regec_tim2_fade_end_if(void) {
	pwm_regec_t *r = (pwm_regec_t*)(SOC_PWM_REG_BASE + (0xec << 2));
	return r->tim2_fade_end_if;
}

static inline void pwm_ll_set_regec_tim2_fade_end_int_en(uint32_t v) {
	pwm_regec_t *r = (pwm_regec_t*)(SOC_PWM_REG_BASE + (0xec << 2));
	r->tim2_fade_end_int_en = v;
}

static inline uint32_t pwm_ll_get_regec_tim2_fade_end_int_en(void) {
	pwm_regec_t *r = (pwm_regec_t*)(SOC_PWM_REG_BASE + (0xec << 2));
	return r->tim2_fade_end_int_en;
}

static inline void pwm_ll_set_regec_tim2_fade_inc_dec(uint32_t v) {
	pwm_regec_t *r = (pwm_regec_t*)(SOC_PWM_REG_BASE + (0xec << 2));
	r->tim2_fade_inc_dec = v;
}

static inline uint32_t pwm_ll_get_regec_tim2_fade_inc_dec(void) {
	pwm_regec_t *r = (pwm_regec_t*)(SOC_PWM_REG_BASE + (0xec << 2));
	return r->tim2_fade_inc_dec;
}

static inline void pwm_ll_set_regec_tim2_duty_fading_en(uint32_t v) {
	pwm_regec_t *r = (pwm_regec_t*)(SOC_PWM_REG_BASE + (0xec << 2));
	r->tim2_duty_fading_en = v;
}

static inline uint32_t pwm_ll_get_regec_tim2_duty_fading_en(void) {
	pwm_regec_t *r = (pwm_regec_t*)(SOC_PWM_REG_BASE + (0xec << 2));
	return r->tim2_duty_fading_en;
}

//reg reged:

static inline void pwm_ll_set_reged_value(uint32_t v) {
	pwm_reged_t *r = (pwm_reged_t*)(SOC_PWM_REG_BASE + (0xed << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reged_value(void) {
	pwm_reged_t *r = (pwm_reged_t*)(SOC_PWM_REG_BASE + (0xed << 2));
	return r->v;
}

static inline void pwm_ll_set_reged_tim3_fade_num(uint32_t v) {
	pwm_reged_t *r = (pwm_reged_t*)(SOC_PWM_REG_BASE + (0xed << 2));
	r->tim3_fade_num = v;
}

static inline uint32_t pwm_ll_get_reged_tim3_fade_num(void) {
	pwm_reged_t *r = (pwm_reged_t*)(SOC_PWM_REG_BASE + (0xed << 2));
	return r->tim3_fade_num;
}

static inline void pwm_ll_set_reged_tim3_fade_intval_cyc(uint32_t v) {
	pwm_reged_t *r = (pwm_reged_t*)(SOC_PWM_REG_BASE + (0xed << 2));
	r->tim3_fade_intval_cyc = v;
}

static inline uint32_t pwm_ll_get_reged_tim3_fade_intval_cyc(void) {
	pwm_reged_t *r = (pwm_reged_t*)(SOC_PWM_REG_BASE + (0xed << 2));
	return r->tim3_fade_intval_cyc;
}

static inline void pwm_ll_set_reged_tim3_fade_scale(uint32_t v) {
	pwm_reged_t *r = (pwm_reged_t*)(SOC_PWM_REG_BASE + (0xed << 2));
	r->tim3_fade_scale = v;
}

static inline uint32_t pwm_ll_get_reged_tim3_fade_scale(void) {
	pwm_reged_t *r = (pwm_reged_t*)(SOC_PWM_REG_BASE + (0xed << 2));
	return r->tim3_fade_scale;
}

static inline void pwm_ll_set_reged_tim3_fade_end_if(uint32_t v) {
	pwm_reged_t *r = (pwm_reged_t*)(SOC_PWM_REG_BASE + (0xed << 2));
	r->tim3_fade_end_if = v;
}

static inline uint32_t pwm_ll_get_reged_tim3_fade_end_if(void) {
	pwm_reged_t *r = (pwm_reged_t*)(SOC_PWM_REG_BASE + (0xed << 2));
	return r->tim3_fade_end_if;
}

static inline void pwm_ll_set_reged_tim3_fade_end_int_en(uint32_t v) {
	pwm_reged_t *r = (pwm_reged_t*)(SOC_PWM_REG_BASE + (0xed << 2));
	r->tim3_fade_end_int_en = v;
}

static inline uint32_t pwm_ll_get_reged_tim3_fade_end_int_en(void) {
	pwm_reged_t *r = (pwm_reged_t*)(SOC_PWM_REG_BASE + (0xed << 2));
	return r->tim3_fade_end_int_en;
}

static inline void pwm_ll_set_reged_tim3_fade_inc_dec(uint32_t v) {
	pwm_reged_t *r = (pwm_reged_t*)(SOC_PWM_REG_BASE + (0xed << 2));
	r->tim3_fade_inc_dec = v;
}

static inline uint32_t pwm_ll_get_reged_tim3_fade_inc_dec(void) {
	pwm_reged_t *r = (pwm_reged_t*)(SOC_PWM_REG_BASE + (0xed << 2));
	return r->tim3_fade_inc_dec;
}

static inline void pwm_ll_set_reged_tim3_duty_fading_en(uint32_t v) {
	pwm_reged_t *r = (pwm_reged_t*)(SOC_PWM_REG_BASE + (0xed << 2));
	r->tim3_duty_fading_en = v;
}

static inline uint32_t pwm_ll_get_reged_tim3_duty_fading_en(void) {
	pwm_reged_t *r = (pwm_reged_t*)(SOC_PWM_REG_BASE + (0xed << 2));
	return r->tim3_duty_fading_en;
}

//reg reg100:

static inline void pwm_ll_set_reg100_value(uint32_t v) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg100_value(void) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg100_ocm3pe(uint32_t v) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	r->ocm3pe = v;
}

static inline uint32_t pwm_ll_get_reg100_ocm3pe(void) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	return r->ocm3pe;
}

static inline void pwm_ll_set_reg100_ocm2pe(uint32_t v) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	r->ocm2pe = v;
}

static inline uint32_t pwm_ll_get_reg100_ocm2pe(void) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	return r->ocm2pe;
}

static inline void pwm_ll_set_reg100_ocm1pe(uint32_t v) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	r->ocm1pe = v;
}

static inline uint32_t pwm_ll_get_reg100_ocm1pe(void) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	return r->ocm1pe;
}

static inline void pwm_ll_set_reg100_arpe3(uint32_t v) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	r->arpe3 = v;
}

static inline uint32_t pwm_ll_get_reg100_arpe3(void) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	return r->arpe3;
}

static inline void pwm_ll_set_reg100_arpe2(uint32_t v) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	r->arpe2 = v;
}

static inline uint32_t pwm_ll_get_reg100_arpe2(void) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	return r->arpe2;
}

static inline void pwm_ll_set_reg100_arpe1(uint32_t v) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	r->arpe1 = v;
}

static inline uint32_t pwm_ll_get_reg100_arpe1(void) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	return r->arpe1;
}

static inline void pwm_ll_set_reg100_oc3pe(uint32_t v) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	r->oc3pe = v;
}

static inline uint32_t pwm_ll_get_reg100_oc3pe(void) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	return r->oc3pe;
}

static inline void pwm_ll_set_reg100_oc2pe(uint32_t v) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	r->oc2pe = v;
}

static inline uint32_t pwm_ll_get_reg100_oc2pe(void) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	return r->oc2pe;
}

static inline void pwm_ll_set_reg100_oc1pe(uint32_t v) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	r->oc1pe = v;
}

static inline uint32_t pwm_ll_get_reg100_oc1pe(void) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	return r->oc1pe;
}

static inline void pwm_ll_set_reg100_reserved_9_17(uint32_t v) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	r->reserved_9_17 = v;
}

static inline uint32_t pwm_ll_get_reg100_reserved_9_17(void) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	return r->reserved_9_17;
}

static inline void pwm_ll_set_reg100_cms_tim3(uint32_t v) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	r->cms_tim3 = v;
}

static inline uint32_t pwm_ll_get_reg100_cms_tim3(void) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	return r->cms_tim3;
}

static inline void pwm_ll_set_reg100_cms_tim2(uint32_t v) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	r->cms_tim2 = v;
}

static inline uint32_t pwm_ll_get_reg100_cms_tim2(void) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	return r->cms_tim2;
}

static inline void pwm_ll_set_reg100_cms_tim1(uint32_t v) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	r->cms_tim1 = v;
}

static inline uint32_t pwm_ll_get_reg100_cms_tim1(void) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	return r->cms_tim1;
}

static inline void pwm_ll_set_reg100_reserved_24_25(uint32_t v) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	r->reserved_24_25 = v;
}

static inline uint32_t pwm_ll_get_reg100_reserved_24_25(void) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	return r->reserved_24_25;
}

static inline void pwm_ll_set_reg100_urs3(uint32_t v) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	r->urs3 = v;
}

static inline uint32_t pwm_ll_get_reg100_urs3(void) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	return r->urs3;
}

static inline void pwm_ll_set_reg100_urs2(uint32_t v) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	r->urs2 = v;
}

static inline uint32_t pwm_ll_get_reg100_urs2(void) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	return r->urs2;
}

static inline void pwm_ll_set_reg100_urs1(uint32_t v) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	r->urs1 = v;
}

static inline uint32_t pwm_ll_get_reg100_urs1(void) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	return r->urs1;
}

static inline void pwm_ll_set_reg100_dir_tim3(uint32_t v) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	r->dir_tim3 = v;
}

static inline uint32_t pwm_ll_get_reg100_dir_tim3(void) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	return r->dir_tim3;
}

static inline void pwm_ll_set_reg100_dir_tim2(uint32_t v) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	r->dir_tim2 = v;
}

static inline uint32_t pwm_ll_get_reg100_dir_tim2(void) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	return r->dir_tim2;
}

static inline void pwm_ll_set_reg100_dir_tim1(uint32_t v) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	r->dir_tim1 = v;
}

static inline uint32_t pwm_ll_get_reg100_dir_tim1(void) {
	pwm_reg100_t *r = (pwm_reg100_t*)(SOC_PWM_REG_BASE + (0x100 << 2));
	return r->dir_tim1;
}

//reg reg101:

static inline void pwm_ll_set_reg101_value(uint32_t v) {
	pwm_reg101_t *r = (pwm_reg101_t*)(SOC_PWM_REG_BASE + (0x101 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg101_value(void) {
	pwm_reg101_t *r = (pwm_reg101_t*)(SOC_PWM_REG_BASE + (0x101 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg101_reserved_0_7(uint32_t v) {
	pwm_reg101_t *r = (pwm_reg101_t*)(SOC_PWM_REG_BASE + (0x101 << 2));
	r->reserved_0_7 = v;
}

static inline uint32_t pwm_ll_get_reg101_reserved_0_7(void) {
	pwm_reg101_t *r = (pwm_reg101_t*)(SOC_PWM_REG_BASE + (0x101 << 2));
	return r->reserved_0_7;
}

static inline void pwm_ll_set_reg101_com_mode(uint32_t v) {
	pwm_reg101_t *r = (pwm_reg101_t*)(SOC_PWM_REG_BASE + (0x101 << 2));
	r->com_mode = v;
}

static inline uint32_t pwm_ll_get_reg101_com_mode(void) {
	pwm_reg101_t *r = (pwm_reg101_t*)(SOC_PWM_REG_BASE + (0x101 << 2));
	return r->com_mode;
}

static inline void pwm_ll_set_reg101_ccpc(uint32_t v) {
	pwm_reg101_t *r = (pwm_reg101_t*)(SOC_PWM_REG_BASE + (0x101 << 2));
	r->ccpc = v;
}

static inline uint32_t pwm_ll_get_reg101_ccpc(void) {
	pwm_reg101_t *r = (pwm_reg101_t*)(SOC_PWM_REG_BASE + (0x101 << 2));
	return r->ccpc;
}

static inline void pwm_ll_set_reg101_trios3(uint32_t v) {
	pwm_reg101_t *r = (pwm_reg101_t*)(SOC_PWM_REG_BASE + (0x101 << 2));
	r->trios3 = v;
}

static inline uint32_t pwm_ll_get_reg101_trios3(void) {
	pwm_reg101_t *r = (pwm_reg101_t*)(SOC_PWM_REG_BASE + (0x101 << 2));
	return r->trios3;
}

static inline void pwm_ll_set_reg101_adcs3(uint32_t v) {
	pwm_reg101_t *r = (pwm_reg101_t*)(SOC_PWM_REG_BASE + (0x101 << 2));
	r->adcs3 = v;
}

static inline uint32_t pwm_ll_get_reg101_adcs3(void) {
	pwm_reg101_t *r = (pwm_reg101_t*)(SOC_PWM_REG_BASE + (0x101 << 2));
	return r->adcs3;
}

static inline void pwm_ll_set_reg101_reserved_16_16(uint32_t v) {
	pwm_reg101_t *r = (pwm_reg101_t*)(SOC_PWM_REG_BASE + (0x101 << 2));
	r->reserved_16_16 = v;
}

static inline uint32_t pwm_ll_get_reg101_reserved_16_16(void) {
	pwm_reg101_t *r = (pwm_reg101_t*)(SOC_PWM_REG_BASE + (0x101 << 2));
	return r->reserved_16_16;
}

static inline void pwm_ll_set_reg101_ccus(uint32_t v) {
	pwm_reg101_t *r = (pwm_reg101_t*)(SOC_PWM_REG_BASE + (0x101 << 2));
	r->ccus = v;
}

static inline uint32_t pwm_ll_get_reg101_ccus(void) {
	pwm_reg101_t *r = (pwm_reg101_t*)(SOC_PWM_REG_BASE + (0x101 << 2));
	return r->ccus;
}

static inline void pwm_ll_set_reg101_trios2(uint32_t v) {
	pwm_reg101_t *r = (pwm_reg101_t*)(SOC_PWM_REG_BASE + (0x101 << 2));
	r->trios2 = v;
}

static inline uint32_t pwm_ll_get_reg101_trios2(void) {
	pwm_reg101_t *r = (pwm_reg101_t*)(SOC_PWM_REG_BASE + (0x101 << 2));
	return r->trios2;
}

static inline void pwm_ll_set_reg101_adcs2(uint32_t v) {
	pwm_reg101_t *r = (pwm_reg101_t*)(SOC_PWM_REG_BASE + (0x101 << 2));
	r->adcs2 = v;
}

static inline uint32_t pwm_ll_get_reg101_adcs2(void) {
	pwm_reg101_t *r = (pwm_reg101_t*)(SOC_PWM_REG_BASE + (0x101 << 2));
	return r->adcs2;
}

static inline void pwm_ll_set_reg101_reserved_24_25(uint32_t v) {
	pwm_reg101_t *r = (pwm_reg101_t*)(SOC_PWM_REG_BASE + (0x101 << 2));
	r->reserved_24_25 = v;
}

static inline uint32_t pwm_ll_get_reg101_reserved_24_25(void) {
	pwm_reg101_t *r = (pwm_reg101_t*)(SOC_PWM_REG_BASE + (0x101 << 2));
	return r->reserved_24_25;
}

static inline void pwm_ll_set_reg101_trios1(uint32_t v) {
	pwm_reg101_t *r = (pwm_reg101_t*)(SOC_PWM_REG_BASE + (0x101 << 2));
	r->trios1 = v;
}

static inline uint32_t pwm_ll_get_reg101_trios1(void) {
	pwm_reg101_t *r = (pwm_reg101_t*)(SOC_PWM_REG_BASE + (0x101 << 2));
	return r->trios1;
}

static inline void pwm_ll_set_reg101_adcs1(uint32_t v) {
	pwm_reg101_t *r = (pwm_reg101_t*)(SOC_PWM_REG_BASE + (0x101 << 2));
	r->adcs1 = v;
}

static inline uint32_t pwm_ll_get_reg101_adcs1(void) {
	pwm_reg101_t *r = (pwm_reg101_t*)(SOC_PWM_REG_BASE + (0x101 << 2));
	return r->adcs1;
}

//reg reg102:

static inline void pwm_ll_set_reg102_value(uint32_t v) {
	pwm_reg102_t *r = (pwm_reg102_t*)(SOC_PWM_REG_BASE + (0x102 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg102_value(void) {
	pwm_reg102_t *r = (pwm_reg102_t*)(SOC_PWM_REG_BASE + (0x102 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg102_sms1(uint32_t v) {
	pwm_reg102_t *r = (pwm_reg102_t*)(SOC_PWM_REG_BASE + (0x102 << 2));
	r->sms1 = v;
}

static inline uint32_t pwm_ll_get_reg102_sms1(void) {
	pwm_reg102_t *r = (pwm_reg102_t*)(SOC_PWM_REG_BASE + (0x102 << 2));
	return r->sms1;
}

static inline void pwm_ll_set_reg102_sms2(uint32_t v) {
	pwm_reg102_t *r = (pwm_reg102_t*)(SOC_PWM_REG_BASE + (0x102 << 2));
	r->sms2 = v;
}

static inline uint32_t pwm_ll_get_reg102_sms2(void) {
	pwm_reg102_t *r = (pwm_reg102_t*)(SOC_PWM_REG_BASE + (0x102 << 2));
	return r->sms2;
}

static inline void pwm_ll_set_reg102_sms3(uint32_t v) {
	pwm_reg102_t *r = (pwm_reg102_t*)(SOC_PWM_REG_BASE + (0x102 << 2));
	r->sms3 = v;
}

static inline uint32_t pwm_ll_get_reg102_sms3(void) {
	pwm_reg102_t *r = (pwm_reg102_t*)(SOC_PWM_REG_BASE + (0x102 << 2));
	return r->sms3;
}

static inline void pwm_ll_set_reg102_ts1(uint32_t v) {
	pwm_reg102_t *r = (pwm_reg102_t*)(SOC_PWM_REG_BASE + (0x102 << 2));
	r->ts1 = v;
}

static inline uint32_t pwm_ll_get_reg102_ts1(void) {
	pwm_reg102_t *r = (pwm_reg102_t*)(SOC_PWM_REG_BASE + (0x102 << 2));
	return r->ts1;
}

static inline void pwm_ll_set_reg102_reserved_15_15(uint32_t v) {
	pwm_reg102_t *r = (pwm_reg102_t*)(SOC_PWM_REG_BASE + (0x102 << 2));
	r->reserved_15_15 = v;
}

static inline uint32_t pwm_ll_get_reg102_reserved_15_15(void) {
	pwm_reg102_t *r = (pwm_reg102_t*)(SOC_PWM_REG_BASE + (0x102 << 2));
	return r->reserved_15_15;
}

static inline void pwm_ll_set_reg102_ts2(uint32_t v) {
	pwm_reg102_t *r = (pwm_reg102_t*)(SOC_PWM_REG_BASE + (0x102 << 2));
	r->ts2 = v;
}

static inline uint32_t pwm_ll_get_reg102_ts2(void) {
	pwm_reg102_t *r = (pwm_reg102_t*)(SOC_PWM_REG_BASE + (0x102 << 2));
	return r->ts2;
}

static inline void pwm_ll_set_reg102_ts3(uint32_t v) {
	pwm_reg102_t *r = (pwm_reg102_t*)(SOC_PWM_REG_BASE + (0x102 << 2));
	r->ts3 = v;
}

static inline uint32_t pwm_ll_get_reg102_ts3(void) {
	pwm_reg102_t *r = (pwm_reg102_t*)(SOC_PWM_REG_BASE + (0x102 << 2));
	return r->ts3;
}

static inline void pwm_ll_set_reg102_reserved_22_31(uint32_t v) {
	pwm_reg102_t *r = (pwm_reg102_t*)(SOC_PWM_REG_BASE + (0x102 << 2));
	r->reserved_22_31 = v;
}

static inline uint32_t pwm_ll_get_reg102_reserved_22_31(void) {
	pwm_reg102_t *r = (pwm_reg102_t*)(SOC_PWM_REG_BASE + (0x102 << 2));
	return r->reserved_22_31;
}

//reg reg103:

static inline void pwm_ll_set_reg103_value(uint32_t v) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg103_value(void) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg103_cc1ie(uint32_t v) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	r->cc1ie = v;
}

static inline uint32_t pwm_ll_get_reg103_cc1ie(void) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	return r->cc1ie;
}

static inline void pwm_ll_set_reg103_cc2ie(uint32_t v) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	r->cc2ie = v;
}

static inline uint32_t pwm_ll_get_reg103_cc2ie(void) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	return r->cc2ie;
}

static inline void pwm_ll_set_reg103_cc3ie(uint32_t v) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	r->cc3ie = v;
}

static inline uint32_t pwm_ll_get_reg103_cc3ie(void) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	return r->cc3ie;
}

static inline void pwm_ll_set_reg103_cc4ie(uint32_t v) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	r->cc4ie = v;
}

static inline uint32_t pwm_ll_get_reg103_cc4ie(void) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	return r->cc4ie;
}

static inline void pwm_ll_set_reg103_cc5ie(uint32_t v) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	r->cc5ie = v;
}

static inline uint32_t pwm_ll_get_reg103_cc5ie(void) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	return r->cc5ie;
}

static inline void pwm_ll_set_reg103_cc6ie(uint32_t v) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	r->cc6ie = v;
}

static inline uint32_t pwm_ll_get_reg103_cc6ie(void) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	return r->cc6ie;
}

static inline void pwm_ll_set_reg103_cc7ie(uint32_t v) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	r->cc7ie = v;
}

static inline uint32_t pwm_ll_get_reg103_cc7ie(void) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	return r->cc7ie;
}

static inline void pwm_ll_set_reg103_cc8ie(uint32_t v) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	r->cc8ie = v;
}

static inline uint32_t pwm_ll_get_reg103_cc8ie(void) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	return r->cc8ie;
}

static inline void pwm_ll_set_reg103_cc9ie(uint32_t v) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	r->cc9ie = v;
}

static inline uint32_t pwm_ll_get_reg103_cc9ie(void) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	return r->cc9ie;
}

static inline void pwm_ll_set_reg103_uie1(uint32_t v) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	r->uie1 = v;
}

static inline uint32_t pwm_ll_get_reg103_uie1(void) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	return r->uie1;
}

static inline void pwm_ll_set_reg103_uie2(uint32_t v) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	r->uie2 = v;
}

static inline uint32_t pwm_ll_get_reg103_uie2(void) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	return r->uie2;
}

static inline void pwm_ll_set_reg103_uie3(uint32_t v) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	r->uie3 = v;
}

static inline uint32_t pwm_ll_get_reg103_uie3(void) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	return r->uie3;
}

static inline void pwm_ll_set_reg103_tie1(uint32_t v) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	r->tie1 = v;
}

static inline uint32_t pwm_ll_get_reg103_tie1(void) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	return r->tie1;
}

static inline void pwm_ll_set_reg103_tie2(uint32_t v) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	r->tie2 = v;
}

static inline uint32_t pwm_ll_get_reg103_tie2(void) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	return r->tie2;
}

static inline void pwm_ll_set_reg103_tie3(uint32_t v) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	r->tie3 = v;
}

static inline uint32_t pwm_ll_get_reg103_tie3(void) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	return r->tie3;
}

static inline void pwm_ll_set_reg103_comie(uint32_t v) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	r->comie = v;
}

static inline uint32_t pwm_ll_get_reg103_comie(void) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	return r->comie;
}

static inline void pwm_ll_set_reg103_reserved_16_19(uint32_t v) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	r->reserved_16_19 = v;
}

static inline uint32_t pwm_ll_get_reg103_reserved_16_19(void) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	return r->reserved_16_19;
}

static inline void pwm_ll_set_reg103_df_tim1(uint32_t v) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	r->df_tim1 = v;
}

static inline uint32_t pwm_ll_get_reg103_df_tim1(void) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	return r->df_tim1;
}

static inline void pwm_ll_set_reg103_df_tim2(uint32_t v) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	r->df_tim2 = v;
}

static inline uint32_t pwm_ll_get_reg103_df_tim2(void) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	return r->df_tim2;
}

static inline void pwm_ll_set_reg103_df_tim3(uint32_t v) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	r->df_tim3 = v;
}

static inline uint32_t pwm_ll_get_reg103_df_tim3(void) {
	pwm_reg103_t *r = (pwm_reg103_t*)(SOC_PWM_REG_BASE + (0x103 << 2));
	return r->df_tim3;
}

//reg reg104:

static inline void pwm_ll_set_reg104_value(uint32_t v) {
	pwm_reg104_t *r = (pwm_reg104_t*)(SOC_PWM_REG_BASE + (0x104 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg104_value(void) {
	pwm_reg104_t *r = (pwm_reg104_t*)(SOC_PWM_REG_BASE + (0x104 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg104_cc1if(uint32_t v) {
	pwm_reg104_t *r = (pwm_reg104_t*)(SOC_PWM_REG_BASE + (0x104 << 2));
	r->cc1if = v;
}

static inline void pwm_ll_set_reg104_cc2if(uint32_t v) {
	pwm_reg104_t *r = (pwm_reg104_t*)(SOC_PWM_REG_BASE + (0x104 << 2));
	r->cc2if = v;
}

static inline void pwm_ll_set_reg104_cc3if(uint32_t v) {
	pwm_reg104_t *r = (pwm_reg104_t*)(SOC_PWM_REG_BASE + (0x104 << 2));
	r->cc3if = v;
}

static inline void pwm_ll_set_reg104_cc4if(uint32_t v) {
	pwm_reg104_t *r = (pwm_reg104_t*)(SOC_PWM_REG_BASE + (0x104 << 2));
	r->cc4if = v;
}

static inline void pwm_ll_set_reg104_cc5if(uint32_t v) {
	pwm_reg104_t *r = (pwm_reg104_t*)(SOC_PWM_REG_BASE + (0x104 << 2));
	r->cc5if = v;
}

static inline void pwm_ll_set_reg104_cc6if(uint32_t v) {
	pwm_reg104_t *r = (pwm_reg104_t*)(SOC_PWM_REG_BASE + (0x104 << 2));
	r->cc6if = v;
}

static inline void pwm_ll_set_reg104_cc7if(uint32_t v) {
	pwm_reg104_t *r = (pwm_reg104_t*)(SOC_PWM_REG_BASE + (0x104 << 2));
	r->cc7if = v;
}

static inline void pwm_ll_set_reg104_cc8if(uint32_t v) {
	pwm_reg104_t *r = (pwm_reg104_t*)(SOC_PWM_REG_BASE + (0x104 << 2));
	r->cc8if = v;
}

static inline void pwm_ll_set_reg104_cc9if(uint32_t v) {
	pwm_reg104_t *r = (pwm_reg104_t*)(SOC_PWM_REG_BASE + (0x104 << 2));
	r->cc9if = v;
}

static inline void pwm_ll_set_reg104_uif1(uint32_t v) {
	pwm_reg104_t *r = (pwm_reg104_t*)(SOC_PWM_REG_BASE + (0x104 << 2));
	r->uif1 = v;
}

static inline void pwm_ll_set_reg104_uif2(uint32_t v) {
	pwm_reg104_t *r = (pwm_reg104_t*)(SOC_PWM_REG_BASE + (0x104 << 2));
	r->uif2 = v;
}

static inline void pwm_ll_set_reg104_uif3(uint32_t v) {
	pwm_reg104_t *r = (pwm_reg104_t*)(SOC_PWM_REG_BASE + (0x104 << 2));
	r->uif3 = v;
}

static inline void pwm_ll_set_reg104_tif1(uint32_t v) {
	pwm_reg104_t *r = (pwm_reg104_t*)(SOC_PWM_REG_BASE + (0x104 << 2));
	r->tif1 = v;
}

static inline void pwm_ll_set_reg104_tif2(uint32_t v) {
	pwm_reg104_t *r = (pwm_reg104_t*)(SOC_PWM_REG_BASE + (0x104 << 2));
	r->tif2 = v;
}

static inline void pwm_ll_set_reg104_tif3(uint32_t v) {
	pwm_reg104_t *r = (pwm_reg104_t*)(SOC_PWM_REG_BASE + (0x104 << 2));
	r->tif3 = v;
}

static inline void pwm_ll_set_reg104_comif(uint32_t v) {
	pwm_reg104_t *r = (pwm_reg104_t*)(SOC_PWM_REG_BASE + (0x104 << 2));
	r->comif = v;
}

static inline void pwm_ll_set_reg104_ch1eif(uint32_t v) {
	pwm_reg104_t *r = (pwm_reg104_t*)(SOC_PWM_REG_BASE + (0x104 << 2));
	r->ch1eif = v;
}

static inline uint32_t pwm_ll_get_reg104_ch1eif(void) {
	pwm_reg104_t *r = (pwm_reg104_t*)(SOC_PWM_REG_BASE + (0x104 << 2));
	return r->ch1eif;
}

static inline void pwm_ll_set_reg104_ch2eif(uint32_t v) {
	pwm_reg104_t *r = (pwm_reg104_t*)(SOC_PWM_REG_BASE + (0x104 << 2));
	r->ch2eif = v;
}

static inline uint32_t pwm_ll_get_reg104_ch2eif(void) {
	pwm_reg104_t *r = (pwm_reg104_t*)(SOC_PWM_REG_BASE + (0x104 << 2));
	return r->ch2eif;
}

static inline void pwm_ll_set_reg104_ch3eif(uint32_t v) {
	pwm_reg104_t *r = (pwm_reg104_t*)(SOC_PWM_REG_BASE + (0x104 << 2));
	r->ch3eif = v;
}

static inline uint32_t pwm_ll_get_reg104_ch3eif(void) {
	pwm_reg104_t *r = (pwm_reg104_t*)(SOC_PWM_REG_BASE + (0x104 << 2));
	return r->ch3eif;
}

static inline void pwm_ll_set_reg104_ch4eif(uint32_t v) {
	pwm_reg104_t *r = (pwm_reg104_t*)(SOC_PWM_REG_BASE + (0x104 << 2));
	r->ch4eif = v;
}

static inline uint32_t pwm_ll_get_reg104_ch4eif(void) {
	pwm_reg104_t *r = (pwm_reg104_t*)(SOC_PWM_REG_BASE + (0x104 << 2));
	return r->ch4eif;
}

static inline void pwm_ll_set_reg104_ch5eif(uint32_t v) {
	pwm_reg104_t *r = (pwm_reg104_t*)(SOC_PWM_REG_BASE + (0x104 << 2));
	r->ch5eif = v;
}

static inline uint32_t pwm_ll_get_reg104_ch5eif(void) {
	pwm_reg104_t *r = (pwm_reg104_t*)(SOC_PWM_REG_BASE + (0x104 << 2));
	return r->ch5eif;
}

static inline void pwm_ll_set_reg104_ch6eif(uint32_t v) {
	pwm_reg104_t *r = (pwm_reg104_t*)(SOC_PWM_REG_BASE + (0x104 << 2));
	r->ch6eif = v;
}

static inline uint32_t pwm_ll_get_reg104_ch6eif(void) {
	pwm_reg104_t *r = (pwm_reg104_t*)(SOC_PWM_REG_BASE + (0x104 << 2));
	return r->ch6eif;
}

static inline void pwm_ll_set_reg104_reserved_22_31(uint32_t v) {
	pwm_reg104_t *r = (pwm_reg104_t*)(SOC_PWM_REG_BASE + (0x104 << 2));
	r->reserved_22_31 = v;
}

static inline uint32_t pwm_ll_get_reg104_reserved_22_31(void) {
	pwm_reg104_t *r = (pwm_reg104_t*)(SOC_PWM_REG_BASE + (0x104 << 2));
	return r->reserved_22_31;
}

//reg reg105:

static inline void pwm_ll_set_reg105_value(uint32_t v) {
	pwm_reg105_t *r = (pwm_reg105_t*)(SOC_PWM_REG_BASE + (0x105 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg105_value(void) {
	pwm_reg105_t *r = (pwm_reg105_t*)(SOC_PWM_REG_BASE + (0x105 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg105_reserved_0_8(uint32_t v) {
	pwm_reg105_t *r = (pwm_reg105_t*)(SOC_PWM_REG_BASE + (0x105 << 2));
	r->reserved_0_8 = v;
}

static inline uint32_t pwm_ll_get_reg105_reserved_0_8(void) {
	pwm_reg105_t *r = (pwm_reg105_t*)(SOC_PWM_REG_BASE + (0x105 << 2));
	return r->reserved_0_8;
}

static inline void pwm_ll_set_reg105_ug1(uint32_t v) {
	pwm_reg105_t *r = (pwm_reg105_t*)(SOC_PWM_REG_BASE + (0x105 << 2));
	r->ug1 = v;
}

static inline void pwm_ll_set_reg105_ug2(uint32_t v) {
	pwm_reg105_t *r = (pwm_reg105_t*)(SOC_PWM_REG_BASE + (0x105 << 2));
	r->ug2 = v;
}

static inline void pwm_ll_set_reg105_ug3(uint32_t v) {
	pwm_reg105_t *r = (pwm_reg105_t*)(SOC_PWM_REG_BASE + (0x105 << 2));
	r->ug3 = v;
}

static inline void pwm_ll_set_reg105_tg1(uint32_t v) {
	pwm_reg105_t *r = (pwm_reg105_t*)(SOC_PWM_REG_BASE + (0x105 << 2));
	r->tg1 = v;
}

static inline void pwm_ll_set_reg105_tg2(uint32_t v) {
	pwm_reg105_t *r = (pwm_reg105_t*)(SOC_PWM_REG_BASE + (0x105 << 2));
	r->tg2 = v;
}

static inline void pwm_ll_set_reg105_tg3(uint32_t v) {
	pwm_reg105_t *r = (pwm_reg105_t*)(SOC_PWM_REG_BASE + (0x105 << 2));
	r->tg3 = v;
}

static inline void pwm_ll_set_reg105_comg(uint32_t v) {
	pwm_reg105_t *r = (pwm_reg105_t*)(SOC_PWM_REG_BASE + (0x105 << 2));
	r->comg = v;
}

static inline void pwm_ll_set_reg105_dtm1(uint32_t v) {
	pwm_reg105_t *r = (pwm_reg105_t*)(SOC_PWM_REG_BASE + (0x105 << 2));
	r->dtm1 = v;
}

static inline uint32_t pwm_ll_get_reg105_dtm1(void) {
	pwm_reg105_t *r = (pwm_reg105_t*)(SOC_PWM_REG_BASE + (0x105 << 2));
	return r->dtm1;
}

static inline void pwm_ll_set_reg105_dtm2(uint32_t v) {
	pwm_reg105_t *r = (pwm_reg105_t*)(SOC_PWM_REG_BASE + (0x105 << 2));
	r->dtm2 = v;
}

static inline uint32_t pwm_ll_get_reg105_dtm2(void) {
	pwm_reg105_t *r = (pwm_reg105_t*)(SOC_PWM_REG_BASE + (0x105 << 2));
	return r->dtm2;
}

static inline void pwm_ll_set_reg105_dtm3(uint32_t v) {
	pwm_reg105_t *r = (pwm_reg105_t*)(SOC_PWM_REG_BASE + (0x105 << 2));
	r->dtm3 = v;
}

static inline uint32_t pwm_ll_get_reg105_dtm3(void) {
	pwm_reg105_t *r = (pwm_reg105_t*)(SOC_PWM_REG_BASE + (0x105 << 2));
	return r->dtm3;
}

static inline void pwm_ll_set_reg105_init_level1(uint32_t v) {
	pwm_reg105_t *r = (pwm_reg105_t*)(SOC_PWM_REG_BASE + (0x105 << 2));
	r->init_level1 = v;
}

static inline uint32_t pwm_ll_get_reg105_init_level1(void) {
	pwm_reg105_t *r = (pwm_reg105_t*)(SOC_PWM_REG_BASE + (0x105 << 2));
	return r->init_level1;
}

static inline void pwm_ll_set_reg105_init_level2(uint32_t v) {
	pwm_reg105_t *r = (pwm_reg105_t*)(SOC_PWM_REG_BASE + (0x105 << 2));
	r->init_level2 = v;
}

static inline uint32_t pwm_ll_get_reg105_init_level2(void) {
	pwm_reg105_t *r = (pwm_reg105_t*)(SOC_PWM_REG_BASE + (0x105 << 2));
	return r->init_level2;
}

static inline void pwm_ll_set_reg105_init_level3(uint32_t v) {
	pwm_reg105_t *r = (pwm_reg105_t*)(SOC_PWM_REG_BASE + (0x105 << 2));
	r->init_level3 = v;
}

static inline uint32_t pwm_ll_get_reg105_init_level3(void) {
	pwm_reg105_t *r = (pwm_reg105_t*)(SOC_PWM_REG_BASE + (0x105 << 2));
	return r->init_level3;
}

static inline void pwm_ll_set_reg105_reserved_25_31(uint32_t v) {
	pwm_reg105_t *r = (pwm_reg105_t*)(SOC_PWM_REG_BASE + (0x105 << 2));
	r->reserved_25_31 = v;
}

static inline uint32_t pwm_ll_get_reg105_reserved_25_31(void) {
	pwm_reg105_t *r = (pwm_reg105_t*)(SOC_PWM_REG_BASE + (0x105 << 2));
	return r->reserved_25_31;
}

//reg reg106:

static inline void pwm_ll_set_reg106_value(uint32_t v) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg106_value(void) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg106_ch1p(uint32_t v) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	r->ch1p = v;
}

static inline uint32_t pwm_ll_get_reg106_ch1p(void) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	return r->ch1p;
}

static inline void pwm_ll_set_reg106_ch2p(uint32_t v) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	r->ch2p = v;
}

static inline uint32_t pwm_ll_get_reg106_ch2p(void) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	return r->ch2p;
}

static inline void pwm_ll_set_reg106_ch3p(uint32_t v) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	r->ch3p = v;
}

static inline uint32_t pwm_ll_get_reg106_ch3p(void) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	return r->ch3p;
}

static inline void pwm_ll_set_reg106_ch4p(uint32_t v) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	r->ch4p = v;
}

static inline uint32_t pwm_ll_get_reg106_ch4p(void) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	return r->ch4p;
}

static inline void pwm_ll_set_reg106_ch5p(uint32_t v) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	r->ch5p = v;
}

static inline uint32_t pwm_ll_get_reg106_ch5p(void) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	return r->ch5p;
}

static inline void pwm_ll_set_reg106_ch6p(uint32_t v) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	r->ch6p = v;
}

static inline uint32_t pwm_ll_get_reg106_ch6p(void) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	return r->ch6p;
}

static inline void pwm_ll_set_reg106_ch1e(uint32_t v) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	r->ch1e = v;
}

static inline uint32_t pwm_ll_get_reg106_ch1e(void) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	return r->ch1e;
}

static inline void pwm_ll_set_reg106_ch2e(uint32_t v) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	r->ch2e = v;
}

static inline uint32_t pwm_ll_get_reg106_ch2e(void) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	return r->ch2e;
}

static inline void pwm_ll_set_reg106_ch3e(uint32_t v) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	r->ch3e = v;
}

static inline uint32_t pwm_ll_get_reg106_ch3e(void) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	return r->ch3e;
}

static inline void pwm_ll_set_reg106_ch4e(uint32_t v) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	r->ch4e = v;
}

static inline uint32_t pwm_ll_get_reg106_ch4e(void) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	return r->ch4e;
}

static inline void pwm_ll_set_reg106_ch5e(uint32_t v) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	r->ch5e = v;
}

static inline uint32_t pwm_ll_get_reg106_ch5e(void) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	return r->ch5e;
}

static inline void pwm_ll_set_reg106_ch6e(uint32_t v) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	r->ch6e = v;
}

static inline uint32_t pwm_ll_get_reg106_ch6e(void) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	return r->ch6e;
}

static inline void pwm_ll_set_reg106_tim1ccm(uint32_t v) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	r->tim1ccm = v;
}

static inline uint32_t pwm_ll_get_reg106_tim1ccm(void) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	return r->tim1ccm;
}

static inline void pwm_ll_set_reg106_tim2ccm(uint32_t v) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	r->tim2ccm = v;
}

static inline uint32_t pwm_ll_get_reg106_tim2ccm(void) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	return r->tim2ccm;
}

static inline void pwm_ll_set_reg106_tim3ccm(uint32_t v) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	r->tim3ccm = v;
}

static inline uint32_t pwm_ll_get_reg106_tim3ccm(void) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	return r->tim3ccm;
}

static inline void pwm_ll_set_reg106_oc1m(uint32_t v) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	r->oc1m = v;
}

static inline uint32_t pwm_ll_get_reg106_oc1m(void) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	return r->oc1m;
}

static inline void pwm_ll_set_reg106_oc2m(uint32_t v) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	r->oc2m = v;
}

static inline uint32_t pwm_ll_get_reg106_oc2m(void) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	return r->oc2m;
}

static inline void pwm_ll_set_reg106_oc3m(uint32_t v) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	r->oc3m = v;
}

static inline uint32_t pwm_ll_get_reg106_oc3m(void) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	return r->oc3m;
}

static inline void pwm_ll_set_reg106_reserved_30_31(uint32_t v) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	r->reserved_30_31 = v;
}

static inline uint32_t pwm_ll_get_reg106_reserved_30_31(void) {
	pwm_reg106_t *r = (pwm_reg106_t*)(SOC_PWM_REG_BASE + (0x106 << 2));
	return r->reserved_30_31;
}

//reg reg107:

static inline void pwm_ll_set_reg107_value(uint32_t v) {
	pwm_reg107_t *r = (pwm_reg107_t*)(SOC_PWM_REG_BASE + (0x107 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg107_value(void) {
	pwm_reg107_t *r = (pwm_reg107_t*)(SOC_PWM_REG_BASE + (0x107 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg107_tim1_cnt(void) {
	pwm_reg107_t *r = (pwm_reg107_t*)(SOC_PWM_REG_BASE + (0x107 << 2));
	return r->tim1_cnt;
}

//reg reg108:

static inline void pwm_ll_set_reg108_value(uint32_t v) {
	pwm_reg108_t *r = (pwm_reg108_t*)(SOC_PWM_REG_BASE + (0x108 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg108_value(void) {
	pwm_reg108_t *r = (pwm_reg108_t*)(SOC_PWM_REG_BASE + (0x108 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg108_tim2_cnt(void) {
	pwm_reg108_t *r = (pwm_reg108_t*)(SOC_PWM_REG_BASE + (0x108 << 2));
	return r->tim2_cnt;
}

//reg reg109:

static inline void pwm_ll_set_reg109_value(uint32_t v) {
	pwm_reg109_t *r = (pwm_reg109_t*)(SOC_PWM_REG_BASE + (0x109 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg109_value(void) {
	pwm_reg109_t *r = (pwm_reg109_t*)(SOC_PWM_REG_BASE + (0x109 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg109_tim3_cnt(void) {
	pwm_reg109_t *r = (pwm_reg109_t*)(SOC_PWM_REG_BASE + (0x109 << 2));
	return r->tim3_cnt;
}

//reg reg10a:

static inline void pwm_ll_set_reg10a_value(uint32_t v) {
	pwm_reg10a_t *r = (pwm_reg10a_t*)(SOC_PWM_REG_BASE + (0x10a << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg10a_value(void) {
	pwm_reg10a_t *r = (pwm_reg10a_t*)(SOC_PWM_REG_BASE + (0x10a << 2));
	return r->v;
}

static inline void pwm_ll_set_reg10a_psc1(uint32_t v) {
	pwm_reg10a_t *r = (pwm_reg10a_t*)(SOC_PWM_REG_BASE + (0x10a << 2));
	r->psc1 = v;
}

static inline uint32_t pwm_ll_get_reg10a_psc1(void) {
	pwm_reg10a_t *r = (pwm_reg10a_t*)(SOC_PWM_REG_BASE + (0x10a << 2));
	return r->psc1;
}

static inline void pwm_ll_set_reg10a_psc2(uint32_t v) {
	pwm_reg10a_t *r = (pwm_reg10a_t*)(SOC_PWM_REG_BASE + (0x10a << 2));
	r->psc2 = v;
}

static inline uint32_t pwm_ll_get_reg10a_psc2(void) {
	pwm_reg10a_t *r = (pwm_reg10a_t*)(SOC_PWM_REG_BASE + (0x10a << 2));
	return r->psc2;
}

static inline void pwm_ll_set_reg10a_psc3(uint32_t v) {
	pwm_reg10a_t *r = (pwm_reg10a_t*)(SOC_PWM_REG_BASE + (0x10a << 2));
	r->psc3 = v;
}

static inline uint32_t pwm_ll_get_reg10a_psc3(void) {
	pwm_reg10a_t *r = (pwm_reg10a_t*)(SOC_PWM_REG_BASE + (0x10a << 2));
	return r->psc3;
}

static inline void pwm_ll_set_reg10a_reserved_24_31(uint32_t v) {
	pwm_reg10a_t *r = (pwm_reg10a_t*)(SOC_PWM_REG_BASE + (0x10a << 2));
	r->reserved_24_31 = v;
}

static inline uint32_t pwm_ll_get_reg10a_reserved_24_31(void) {
	pwm_reg10a_t *r = (pwm_reg10a_t*)(SOC_PWM_REG_BASE + (0x10a << 2));
	return r->reserved_24_31;
}

//reg reg10b:

static inline void pwm_ll_set_reg10b_value(uint32_t v) {
	pwm_reg10b_t *r = (pwm_reg10b_t*)(SOC_PWM_REG_BASE + (0x10b << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg10b_value(void) {
	pwm_reg10b_t *r = (pwm_reg10b_t*)(SOC_PWM_REG_BASE + (0x10b << 2));
	return r->v;
}

static inline void pwm_ll_set_reg10b_tim1_arr(uint32_t v) {
	pwm_reg10b_t *r = (pwm_reg10b_t*)(SOC_PWM_REG_BASE + (0x10b << 2));
	r->tim1_arr = v;
}

static inline uint32_t pwm_ll_get_reg10b_tim1_arr(void) {
	pwm_reg10b_t *r = (pwm_reg10b_t*)(SOC_PWM_REG_BASE + (0x10b << 2));
	return r->tim1_arr;
}

//reg reg10c:

static inline void pwm_ll_set_reg10c_value(uint32_t v) {
	pwm_reg10c_t *r = (pwm_reg10c_t*)(SOC_PWM_REG_BASE + (0x10c << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg10c_value(void) {
	pwm_reg10c_t *r = (pwm_reg10c_t*)(SOC_PWM_REG_BASE + (0x10c << 2));
	return r->v;
}

static inline void pwm_ll_set_reg10c_tim2_arr(uint32_t v) {
	pwm_reg10c_t *r = (pwm_reg10c_t*)(SOC_PWM_REG_BASE + (0x10c << 2));
	r->tim2_arr = v;
}

static inline uint32_t pwm_ll_get_reg10c_tim2_arr(void) {
	pwm_reg10c_t *r = (pwm_reg10c_t*)(SOC_PWM_REG_BASE + (0x10c << 2));
	return r->tim2_arr;
}

//reg reg10d:

static inline void pwm_ll_set_reg10d_value(uint32_t v) {
	pwm_reg10d_t *r = (pwm_reg10d_t*)(SOC_PWM_REG_BASE + (0x10d << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg10d_value(void) {
	pwm_reg10d_t *r = (pwm_reg10d_t*)(SOC_PWM_REG_BASE + (0x10d << 2));
	return r->v;
}

static inline void pwm_ll_set_reg10d_tim3_arr(uint32_t v) {
	pwm_reg10d_t *r = (pwm_reg10d_t*)(SOC_PWM_REG_BASE + (0x10d << 2));
	r->tim3_arr = v;
}

static inline uint32_t pwm_ll_get_reg10d_tim3_arr(void) {
	pwm_reg10d_t *r = (pwm_reg10d_t*)(SOC_PWM_REG_BASE + (0x10d << 2));
	return r->tim3_arr;
}

//reg reg10e:

static inline void pwm_ll_set_reg10e_value(uint32_t v) {
	pwm_reg10e_t *r = (pwm_reg10e_t*)(SOC_PWM_REG_BASE + (0x10e << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg10e_value(void) {
	pwm_reg10e_t *r = (pwm_reg10e_t*)(SOC_PWM_REG_BASE + (0x10e << 2));
	return r->v;
}

static inline void pwm_ll_set_reg10e_tim1_rcr(uint32_t v) {
	pwm_reg10e_t *r = (pwm_reg10e_t*)(SOC_PWM_REG_BASE + (0x10e << 2));
	r->tim1_rcr = v;
}

static inline uint32_t pwm_ll_get_reg10e_tim1_rcr(void) {
	pwm_reg10e_t *r = (pwm_reg10e_t*)(SOC_PWM_REG_BASE + (0x10e << 2));
	return r->tim1_rcr;
}

//reg reg10f:

static inline void pwm_ll_set_reg10f_value(uint32_t v) {
	pwm_reg10f_t *r = (pwm_reg10f_t*)(SOC_PWM_REG_BASE + (0x10f << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg10f_value(void) {
	pwm_reg10f_t *r = (pwm_reg10f_t*)(SOC_PWM_REG_BASE + (0x10f << 2));
	return r->v;
}

static inline void pwm_ll_set_reg10f_tim2_rcr(uint32_t v) {
	pwm_reg10f_t *r = (pwm_reg10f_t*)(SOC_PWM_REG_BASE + (0x10f << 2));
	r->tim2_rcr = v;
}

static inline uint32_t pwm_ll_get_reg10f_tim2_rcr(void) {
	pwm_reg10f_t *r = (pwm_reg10f_t*)(SOC_PWM_REG_BASE + (0x10f << 2));
	return r->tim2_rcr;
}

//reg reg110:

static inline void pwm_ll_set_reg110_value(uint32_t v) {
	pwm_reg110_t *r = (pwm_reg110_t*)(SOC_PWM_REG_BASE + (0x110 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg110_value(void) {
	pwm_reg110_t *r = (pwm_reg110_t*)(SOC_PWM_REG_BASE + (0x110 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg110_tim3_rcr(uint32_t v) {
	pwm_reg110_t *r = (pwm_reg110_t*)(SOC_PWM_REG_BASE + (0x110 << 2));
	r->tim3_rcr = v;
}

static inline uint32_t pwm_ll_get_reg110_tim3_rcr(void) {
	pwm_reg110_t *r = (pwm_reg110_t*)(SOC_PWM_REG_BASE + (0x110 << 2));
	return r->tim3_rcr;
}

//reg reg111:

static inline void pwm_ll_set_reg111_value(uint32_t v) {
	pwm_reg111_t *r = (pwm_reg111_t*)(SOC_PWM_REG_BASE + (0x111 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg111_value(void) {
	pwm_reg111_t *r = (pwm_reg111_t*)(SOC_PWM_REG_BASE + (0x111 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg111_ccr1(uint32_t v) {
	pwm_reg111_t *r = (pwm_reg111_t*)(SOC_PWM_REG_BASE + (0x111 << 2));
	r->ccr1 = v;
}

static inline uint32_t pwm_ll_get_reg111_ccr1(void) {
	pwm_reg111_t *r = (pwm_reg111_t*)(SOC_PWM_REG_BASE + (0x111 << 2));
	return r->ccr1;
}

//reg reg112:

static inline void pwm_ll_set_reg112_value(uint32_t v) {
	pwm_reg112_t *r = (pwm_reg112_t*)(SOC_PWM_REG_BASE + (0x112 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg112_value(void) {
	pwm_reg112_t *r = (pwm_reg112_t*)(SOC_PWM_REG_BASE + (0x112 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg112_ccr2(uint32_t v) {
	pwm_reg112_t *r = (pwm_reg112_t*)(SOC_PWM_REG_BASE + (0x112 << 2));
	r->ccr2 = v;
}

static inline uint32_t pwm_ll_get_reg112_ccr2(void) {
	pwm_reg112_t *r = (pwm_reg112_t*)(SOC_PWM_REG_BASE + (0x112 << 2));
	return r->ccr2;
}

//reg reg113:

static inline void pwm_ll_set_reg113_value(uint32_t v) {
	pwm_reg113_t *r = (pwm_reg113_t*)(SOC_PWM_REG_BASE + (0x113 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg113_value(void) {
	pwm_reg113_t *r = (pwm_reg113_t*)(SOC_PWM_REG_BASE + (0x113 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg113_ccr3(uint32_t v) {
	pwm_reg113_t *r = (pwm_reg113_t*)(SOC_PWM_REG_BASE + (0x113 << 2));
	r->ccr3 = v;
}

static inline uint32_t pwm_ll_get_reg113_ccr3(void) {
	pwm_reg113_t *r = (pwm_reg113_t*)(SOC_PWM_REG_BASE + (0x113 << 2));
	return r->ccr3;
}

//reg reg114:

static inline void pwm_ll_set_reg114_value(uint32_t v) {
	pwm_reg114_t *r = (pwm_reg114_t*)(SOC_PWM_REG_BASE + (0x114 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg114_value(void) {
	pwm_reg114_t *r = (pwm_reg114_t*)(SOC_PWM_REG_BASE + (0x114 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg114_ccr4(uint32_t v) {
	pwm_reg114_t *r = (pwm_reg114_t*)(SOC_PWM_REG_BASE + (0x114 << 2));
	r->ccr4 = v;
}

static inline uint32_t pwm_ll_get_reg114_ccr4(void) {
	pwm_reg114_t *r = (pwm_reg114_t*)(SOC_PWM_REG_BASE + (0x114 << 2));
	return r->ccr4;
}

//reg reg115:

static inline void pwm_ll_set_reg115_value(uint32_t v) {
	pwm_reg115_t *r = (pwm_reg115_t*)(SOC_PWM_REG_BASE + (0x115 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg115_value(void) {
	pwm_reg115_t *r = (pwm_reg115_t*)(SOC_PWM_REG_BASE + (0x115 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg115_ccr5(uint32_t v) {
	pwm_reg115_t *r = (pwm_reg115_t*)(SOC_PWM_REG_BASE + (0x115 << 2));
	r->ccr5 = v;
}

static inline uint32_t pwm_ll_get_reg115_ccr5(void) {
	pwm_reg115_t *r = (pwm_reg115_t*)(SOC_PWM_REG_BASE + (0x115 << 2));
	return r->ccr5;
}

//reg reg116:

static inline void pwm_ll_set_reg116_value(uint32_t v) {
	pwm_reg116_t *r = (pwm_reg116_t*)(SOC_PWM_REG_BASE + (0x116 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg116_value(void) {
	pwm_reg116_t *r = (pwm_reg116_t*)(SOC_PWM_REG_BASE + (0x116 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg116_ccr6(uint32_t v) {
	pwm_reg116_t *r = (pwm_reg116_t*)(SOC_PWM_REG_BASE + (0x116 << 2));
	r->ccr6 = v;
}

static inline uint32_t pwm_ll_get_reg116_ccr6(void) {
	pwm_reg116_t *r = (pwm_reg116_t*)(SOC_PWM_REG_BASE + (0x116 << 2));
	return r->ccr6;
}

//reg reg117:

static inline void pwm_ll_set_reg117_value(uint32_t v) {
	pwm_reg117_t *r = (pwm_reg117_t*)(SOC_PWM_REG_BASE + (0x117 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg117_value(void) {
	pwm_reg117_t *r = (pwm_reg117_t*)(SOC_PWM_REG_BASE + (0x117 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg117_ccr7(uint32_t v) {
	pwm_reg117_t *r = (pwm_reg117_t*)(SOC_PWM_REG_BASE + (0x117 << 2));
	r->ccr7 = v;
}

static inline uint32_t pwm_ll_get_reg117_ccr7(void) {
	pwm_reg117_t *r = (pwm_reg117_t*)(SOC_PWM_REG_BASE + (0x117 << 2));
	return r->ccr7;
}

//reg reg118:

static inline void pwm_ll_set_reg118_value(uint32_t v) {
	pwm_reg118_t *r = (pwm_reg118_t*)(SOC_PWM_REG_BASE + (0x118 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg118_value(void) {
	pwm_reg118_t *r = (pwm_reg118_t*)(SOC_PWM_REG_BASE + (0x118 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg118_ccr8(uint32_t v) {
	pwm_reg118_t *r = (pwm_reg118_t*)(SOC_PWM_REG_BASE + (0x118 << 2));
	r->ccr8 = v;
}

static inline uint32_t pwm_ll_get_reg118_ccr8(void) {
	pwm_reg118_t *r = (pwm_reg118_t*)(SOC_PWM_REG_BASE + (0x118 << 2));
	return r->ccr8;
}

//reg reg119:

static inline void pwm_ll_set_reg119_value(uint32_t v) {
	pwm_reg119_t *r = (pwm_reg119_t*)(SOC_PWM_REG_BASE + (0x119 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg119_value(void) {
	pwm_reg119_t *r = (pwm_reg119_t*)(SOC_PWM_REG_BASE + (0x119 << 2));
	return r->v;
}

static inline void pwm_ll_set_reg119_ccr9(uint32_t v) {
	pwm_reg119_t *r = (pwm_reg119_t*)(SOC_PWM_REG_BASE + (0x119 << 2));
	r->ccr9 = v;
}

static inline uint32_t pwm_ll_get_reg119_ccr9(void) {
	pwm_reg119_t *r = (pwm_reg119_t*)(SOC_PWM_REG_BASE + (0x119 << 2));
	return r->ccr9;
}

//reg reg11a:

static inline void pwm_ll_set_reg11a_value(uint32_t v) {
	pwm_reg11a_t *r = (pwm_reg11a_t*)(SOC_PWM_REG_BASE + (0x11a << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg11a_value(void) {
	pwm_reg11a_t *r = (pwm_reg11a_t*)(SOC_PWM_REG_BASE + (0x11a << 2));
	return r->v;
}

static inline void pwm_ll_set_reg11a_dt1(uint32_t v) {
	pwm_reg11a_t *r = (pwm_reg11a_t*)(SOC_PWM_REG_BASE + (0x11a << 2));
	r->dt1 = v;
}

static inline uint32_t pwm_ll_get_reg11a_dt1(void) {
	pwm_reg11a_t *r = (pwm_reg11a_t*)(SOC_PWM_REG_BASE + (0x11a << 2));
	return r->dt1;
}

static inline void pwm_ll_set_reg11a_dt2(uint32_t v) {
	pwm_reg11a_t *r = (pwm_reg11a_t*)(SOC_PWM_REG_BASE + (0x11a << 2));
	r->dt2 = v;
}

static inline uint32_t pwm_ll_get_reg11a_dt2(void) {
	pwm_reg11a_t *r = (pwm_reg11a_t*)(SOC_PWM_REG_BASE + (0x11a << 2));
	return r->dt2;
}

static inline void pwm_ll_set_reg11a_dt3(uint32_t v) {
	pwm_reg11a_t *r = (pwm_reg11a_t*)(SOC_PWM_REG_BASE + (0x11a << 2));
	r->dt3 = v;
}

static inline uint32_t pwm_ll_get_reg11a_dt3(void) {
	pwm_reg11a_t *r = (pwm_reg11a_t*)(SOC_PWM_REG_BASE + (0x11a << 2));
	return r->dt3;
}

static inline void pwm_ll_set_reg11a_reserved_30_31(uint32_t v) {
	pwm_reg11a_t *r = (pwm_reg11a_t*)(SOC_PWM_REG_BASE + (0x11a << 2));
	r->reserved_30_31 = v;
}

static inline uint32_t pwm_ll_get_reg11a_reserved_30_31(void) {
	pwm_reg11a_t *r = (pwm_reg11a_t*)(SOC_PWM_REG_BASE + (0x11a << 2));
	return r->reserved_30_31;
}

//reg reg11b:

static inline void pwm_ll_set_reg11b_value(uint32_t v) {
	pwm_reg11b_t *r = (pwm_reg11b_t*)(SOC_PWM_REG_BASE + (0x11b << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg11b_value(void) {
	pwm_reg11b_t *r = (pwm_reg11b_t*)(SOC_PWM_REG_BASE + (0x11b << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg11b_tim1_arr_shad(void) {
	pwm_reg11b_t *r = (pwm_reg11b_t*)(SOC_PWM_REG_BASE + (0x11b << 2));
	return r->tim1_arr_shad;
}

//reg reg11c:

static inline void pwm_ll_set_reg11c_value(uint32_t v) {
	pwm_reg11c_t *r = (pwm_reg11c_t*)(SOC_PWM_REG_BASE + (0x11c << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg11c_value(void) {
	pwm_reg11c_t *r = (pwm_reg11c_t*)(SOC_PWM_REG_BASE + (0x11c << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg11c_tim2_arr_shad(void) {
	pwm_reg11c_t *r = (pwm_reg11c_t*)(SOC_PWM_REG_BASE + (0x11c << 2));
	return r->tim2_arr_shad;
}

//reg reg11d:

static inline void pwm_ll_set_reg11d_value(uint32_t v) {
	pwm_reg11d_t *r = (pwm_reg11d_t*)(SOC_PWM_REG_BASE + (0x11d << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg11d_value(void) {
	pwm_reg11d_t *r = (pwm_reg11d_t*)(SOC_PWM_REG_BASE + (0x11d << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg11d_tim3_arr_shad(void) {
	pwm_reg11d_t *r = (pwm_reg11d_t*)(SOC_PWM_REG_BASE + (0x11d << 2));
	return r->tim3_arr_shad;
}

//reg reg11e:

static inline void pwm_ll_set_reg11e_value(uint32_t v) {
	pwm_reg11e_t *r = (pwm_reg11e_t*)(SOC_PWM_REG_BASE + (0x11e << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg11e_value(void) {
	pwm_reg11e_t *r = (pwm_reg11e_t*)(SOC_PWM_REG_BASE + (0x11e << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg11e_tim1_rcr_shad(void) {
	pwm_reg11e_t *r = (pwm_reg11e_t*)(SOC_PWM_REG_BASE + (0x11e << 2));
	return r->tim1_rcr_shad;
}

//reg reg11f:

static inline void pwm_ll_set_reg11f_value(uint32_t v) {
	pwm_reg11f_t *r = (pwm_reg11f_t*)(SOC_PWM_REG_BASE + (0x11f << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg11f_value(void) {
	pwm_reg11f_t *r = (pwm_reg11f_t*)(SOC_PWM_REG_BASE + (0x11f << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg11f_tim2_rcr_shad(void) {
	pwm_reg11f_t *r = (pwm_reg11f_t*)(SOC_PWM_REG_BASE + (0x11f << 2));
	return r->tim2_rcr_shad;
}

//reg reg120:

static inline void pwm_ll_set_reg120_value(uint32_t v) {
	pwm_reg120_t *r = (pwm_reg120_t*)(SOC_PWM_REG_BASE + (0x120 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg120_value(void) {
	pwm_reg120_t *r = (pwm_reg120_t*)(SOC_PWM_REG_BASE + (0x120 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg120_tim3_rcr_shad(void) {
	pwm_reg120_t *r = (pwm_reg120_t*)(SOC_PWM_REG_BASE + (0x120 << 2));
	return r->tim3_rcr_shad;
}

//reg reg121:

static inline void pwm_ll_set_reg121_value(uint32_t v) {
	pwm_reg121_t *r = (pwm_reg121_t*)(SOC_PWM_REG_BASE + (0x121 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg121_value(void) {
	pwm_reg121_t *r = (pwm_reg121_t*)(SOC_PWM_REG_BASE + (0x121 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg121_ccr1_shad(void) {
	pwm_reg121_t *r = (pwm_reg121_t*)(SOC_PWM_REG_BASE + (0x121 << 2));
	return r->ccr1_shad;
}

//reg reg122:

static inline void pwm_ll_set_reg122_value(uint32_t v) {
	pwm_reg122_t *r = (pwm_reg122_t*)(SOC_PWM_REG_BASE + (0x122 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg122_value(void) {
	pwm_reg122_t *r = (pwm_reg122_t*)(SOC_PWM_REG_BASE + (0x122 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg122_ccr2_shad(void) {
	pwm_reg122_t *r = (pwm_reg122_t*)(SOC_PWM_REG_BASE + (0x122 << 2));
	return r->ccr2_shad;
}

//reg reg123:

static inline void pwm_ll_set_reg123_value(uint32_t v) {
	pwm_reg123_t *r = (pwm_reg123_t*)(SOC_PWM_REG_BASE + (0x123 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg123_value(void) {
	pwm_reg123_t *r = (pwm_reg123_t*)(SOC_PWM_REG_BASE + (0x123 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg123_ccr3_shad(void) {
	pwm_reg123_t *r = (pwm_reg123_t*)(SOC_PWM_REG_BASE + (0x123 << 2));
	return r->ccr3_shad;
}

//reg reg124:

static inline void pwm_ll_set_reg124_value(uint32_t v) {
	pwm_reg124_t *r = (pwm_reg124_t*)(SOC_PWM_REG_BASE + (0x124 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg124_value(void) {
	pwm_reg124_t *r = (pwm_reg124_t*)(SOC_PWM_REG_BASE + (0x124 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg124_ccr4_shad(void) {
	pwm_reg124_t *r = (pwm_reg124_t*)(SOC_PWM_REG_BASE + (0x124 << 2));
	return r->ccr4_shad;
}

//reg reg125:

static inline void pwm_ll_set_reg125_value(uint32_t v) {
	pwm_reg125_t *r = (pwm_reg125_t*)(SOC_PWM_REG_BASE + (0x125 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg125_value(void) {
	pwm_reg125_t *r = (pwm_reg125_t*)(SOC_PWM_REG_BASE + (0x125 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg125_ccr5_shad(void) {
	pwm_reg125_t *r = (pwm_reg125_t*)(SOC_PWM_REG_BASE + (0x125 << 2));
	return r->ccr5_shad;
}

//reg reg126:

static inline void pwm_ll_set_reg126_value(uint32_t v) {
	pwm_reg126_t *r = (pwm_reg126_t*)(SOC_PWM_REG_BASE + (0x126 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg126_value(void) {
	pwm_reg126_t *r = (pwm_reg126_t*)(SOC_PWM_REG_BASE + (0x126 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg126_ccr6_shad(void) {
	pwm_reg126_t *r = (pwm_reg126_t*)(SOC_PWM_REG_BASE + (0x126 << 2));
	return r->ccr6_shad;
}

//reg reg127:

static inline void pwm_ll_set_reg127_value(uint32_t v) {
	pwm_reg127_t *r = (pwm_reg127_t*)(SOC_PWM_REG_BASE + (0x127 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg127_value(void) {
	pwm_reg127_t *r = (pwm_reg127_t*)(SOC_PWM_REG_BASE + (0x127 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg127_ccr7_shad(void) {
	pwm_reg127_t *r = (pwm_reg127_t*)(SOC_PWM_REG_BASE + (0x127 << 2));
	return r->ccr7_shad;
}

//reg reg128:

static inline void pwm_ll_set_reg128_value(uint32_t v) {
	pwm_reg128_t *r = (pwm_reg128_t*)(SOC_PWM_REG_BASE + (0x128 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg128_value(void) {
	pwm_reg128_t *r = (pwm_reg128_t*)(SOC_PWM_REG_BASE + (0x128 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg128_ccr8_shad(void) {
	pwm_reg128_t *r = (pwm_reg128_t*)(SOC_PWM_REG_BASE + (0x128 << 2));
	return r->ccr8_shad;
}

//reg reg129:

static inline void pwm_ll_set_reg129_value(uint32_t v) {
	pwm_reg129_t *r = (pwm_reg129_t*)(SOC_PWM_REG_BASE + (0x129 << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg129_value(void) {
	pwm_reg129_t *r = (pwm_reg129_t*)(SOC_PWM_REG_BASE + (0x129 << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg129_ccr9_shad(void) {
	pwm_reg129_t *r = (pwm_reg129_t*)(SOC_PWM_REG_BASE + (0x129 << 2));
	return r->ccr9_shad;
}

//reg reg12a:

static inline void pwm_ll_set_reg12a_value(uint32_t v) {
	pwm_reg12a_t *r = (pwm_reg12a_t*)(SOC_PWM_REG_BASE + (0x12a << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg12a_value(void) {
	pwm_reg12a_t *r = (pwm_reg12a_t*)(SOC_PWM_REG_BASE + (0x12a << 2));
	return r->v;
}

static inline uint32_t pwm_ll_get_reg12a_ch6e_shad(void) {
	pwm_reg12a_t *r = (pwm_reg12a_t*)(SOC_PWM_REG_BASE + (0x12a << 2));
	return r->ch6e_shad;
}

static inline uint32_t pwm_ll_get_reg12a_ch5e_shad(void) {
	pwm_reg12a_t *r = (pwm_reg12a_t*)(SOC_PWM_REG_BASE + (0x12a << 2));
	return r->ch5e_shad;
}

static inline uint32_t pwm_ll_get_reg12a_ch4e_shad(void) {
	pwm_reg12a_t *r = (pwm_reg12a_t*)(SOC_PWM_REG_BASE + (0x12a << 2));
	return r->ch4e_shad;
}

static inline uint32_t pwm_ll_get_reg12a_ch3e_shad(void) {
	pwm_reg12a_t *r = (pwm_reg12a_t*)(SOC_PWM_REG_BASE + (0x12a << 2));
	return r->ch3e_shad;
}

static inline uint32_t pwm_ll_get_reg12a_ch2e_shad(void) {
	pwm_reg12a_t *r = (pwm_reg12a_t*)(SOC_PWM_REG_BASE + (0x12a << 2));
	return r->ch2e_shad;
}

static inline uint32_t pwm_ll_get_reg12a_ch1e_shad(void) {
	pwm_reg12a_t *r = (pwm_reg12a_t*)(SOC_PWM_REG_BASE + (0x12a << 2));
	return r->ch1e_shad;
}

static inline uint32_t pwm_ll_get_reg12a_oc1m_shad(void) {
	pwm_reg12a_t *r = (pwm_reg12a_t*)(SOC_PWM_REG_BASE + (0x12a << 2));
	return r->oc1m_shad;
}

static inline uint32_t pwm_ll_get_reg12a_oc2m_shad(void) {
	pwm_reg12a_t *r = (pwm_reg12a_t*)(SOC_PWM_REG_BASE + (0x12a << 2));
	return r->oc2m_shad;
}

static inline uint32_t pwm_ll_get_reg12a_oc3m_shad(void) {
	pwm_reg12a_t *r = (pwm_reg12a_t*)(SOC_PWM_REG_BASE + (0x12a << 2));
	return r->oc3m_shad;
}

static inline uint32_t pwm_ll_get_reg12a_reserved_15_31(void) {
	pwm_reg12a_t *r = (pwm_reg12a_t*)(SOC_PWM_REG_BASE + (0x12a << 2));
	return r->reserved_15_31;
}

//reg reg12b:

static inline void pwm_ll_set_reg12b_value(uint32_t v) {
	pwm_reg12b_t *r = (pwm_reg12b_t*)(SOC_PWM_REG_BASE + (0x12b << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg12b_value(void) {
	pwm_reg12b_t *r = (pwm_reg12b_t*)(SOC_PWM_REG_BASE + (0x12b << 2));
	return r->v;
}

static inline void pwm_ll_set_reg12b_tim1_fade_num(uint32_t v) {
	pwm_reg12b_t *r = (pwm_reg12b_t*)(SOC_PWM_REG_BASE + (0x12b << 2));
	r->tim1_fade_num = v;
}

static inline uint32_t pwm_ll_get_reg12b_tim1_fade_num(void) {
	pwm_reg12b_t *r = (pwm_reg12b_t*)(SOC_PWM_REG_BASE + (0x12b << 2));
	return r->tim1_fade_num;
}

static inline void pwm_ll_set_reg12b_tim1_fade_intval_cyc(uint32_t v) {
	pwm_reg12b_t *r = (pwm_reg12b_t*)(SOC_PWM_REG_BASE + (0x12b << 2));
	r->tim1_fade_intval_cyc = v;
}

static inline uint32_t pwm_ll_get_reg12b_tim1_fade_intval_cyc(void) {
	pwm_reg12b_t *r = (pwm_reg12b_t*)(SOC_PWM_REG_BASE + (0x12b << 2));
	return r->tim1_fade_intval_cyc;
}

static inline void pwm_ll_set_reg12b_tim1_fade_scale(uint32_t v) {
	pwm_reg12b_t *r = (pwm_reg12b_t*)(SOC_PWM_REG_BASE + (0x12b << 2));
	r->tim1_fade_scale = v;
}

static inline uint32_t pwm_ll_get_reg12b_tim1_fade_scale(void) {
	pwm_reg12b_t *r = (pwm_reg12b_t*)(SOC_PWM_REG_BASE + (0x12b << 2));
	return r->tim1_fade_scale;
}

static inline void pwm_ll_set_reg12b_tim1_fade_end_if(uint32_t v) {
	pwm_reg12b_t *r = (pwm_reg12b_t*)(SOC_PWM_REG_BASE + (0x12b << 2));
	r->tim1_fade_end_if = v;
}

static inline uint32_t pwm_ll_get_reg12b_tim1_fade_end_if(void) {
	pwm_reg12b_t *r = (pwm_reg12b_t*)(SOC_PWM_REG_BASE + (0x12b << 2));
	return r->tim1_fade_end_if;
}

static inline void pwm_ll_set_reg12b_tim1_fade_end_int_en(uint32_t v) {
	pwm_reg12b_t *r = (pwm_reg12b_t*)(SOC_PWM_REG_BASE + (0x12b << 2));
	r->tim1_fade_end_int_en = v;
}

static inline uint32_t pwm_ll_get_reg12b_tim1_fade_end_int_en(void) {
	pwm_reg12b_t *r = (pwm_reg12b_t*)(SOC_PWM_REG_BASE + (0x12b << 2));
	return r->tim1_fade_end_int_en;
}

static inline void pwm_ll_set_reg12b_tim1_fade_inc_dec(uint32_t v) {
	pwm_reg12b_t *r = (pwm_reg12b_t*)(SOC_PWM_REG_BASE + (0x12b << 2));
	r->tim1_fade_inc_dec = v;
}

static inline uint32_t pwm_ll_get_reg12b_tim1_fade_inc_dec(void) {
	pwm_reg12b_t *r = (pwm_reg12b_t*)(SOC_PWM_REG_BASE + (0x12b << 2));
	return r->tim1_fade_inc_dec;
}

static inline void pwm_ll_set_reg12b_tim1_duty_fading_en(uint32_t v) {
	pwm_reg12b_t *r = (pwm_reg12b_t*)(SOC_PWM_REG_BASE + (0x12b << 2));
	r->tim1_duty_fading_en = v;
}

static inline uint32_t pwm_ll_get_reg12b_tim1_duty_fading_en(void) {
	pwm_reg12b_t *r = (pwm_reg12b_t*)(SOC_PWM_REG_BASE + (0x12b << 2));
	return r->tim1_duty_fading_en;
}

//reg reg12c:

static inline void pwm_ll_set_reg12c_value(uint32_t v) {
	pwm_reg12c_t *r = (pwm_reg12c_t*)(SOC_PWM_REG_BASE + (0x12c << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg12c_value(void) {
	pwm_reg12c_t *r = (pwm_reg12c_t*)(SOC_PWM_REG_BASE + (0x12c << 2));
	return r->v;
}

static inline void pwm_ll_set_reg12c_tim2_fade_num(uint32_t v) {
	pwm_reg12c_t *r = (pwm_reg12c_t*)(SOC_PWM_REG_BASE + (0x12c << 2));
	r->tim2_fade_num = v;
}

static inline uint32_t pwm_ll_get_reg12c_tim2_fade_num(void) {
	pwm_reg12c_t *r = (pwm_reg12c_t*)(SOC_PWM_REG_BASE + (0x12c << 2));
	return r->tim2_fade_num;
}

static inline void pwm_ll_set_reg12c_tim2_fade_intval_cyc(uint32_t v) {
	pwm_reg12c_t *r = (pwm_reg12c_t*)(SOC_PWM_REG_BASE + (0x12c << 2));
	r->tim2_fade_intval_cyc = v;
}

static inline uint32_t pwm_ll_get_reg12c_tim2_fade_intval_cyc(void) {
	pwm_reg12c_t *r = (pwm_reg12c_t*)(SOC_PWM_REG_BASE + (0x12c << 2));
	return r->tim2_fade_intval_cyc;
}

static inline void pwm_ll_set_reg12c_tim2_fade_scale(uint32_t v) {
	pwm_reg12c_t *r = (pwm_reg12c_t*)(SOC_PWM_REG_BASE + (0x12c << 2));
	r->tim2_fade_scale = v;
}

static inline uint32_t pwm_ll_get_reg12c_tim2_fade_scale(void) {
	pwm_reg12c_t *r = (pwm_reg12c_t*)(SOC_PWM_REG_BASE + (0x12c << 2));
	return r->tim2_fade_scale;
}

static inline void pwm_ll_set_reg12c_tim2_fade_end_if(uint32_t v) {
	pwm_reg12c_t *r = (pwm_reg12c_t*)(SOC_PWM_REG_BASE + (0x12c << 2));
	r->tim2_fade_end_if = v;
}

static inline uint32_t pwm_ll_get_reg12c_tim2_fade_end_if(void) {
	pwm_reg12c_t *r = (pwm_reg12c_t*)(SOC_PWM_REG_BASE + (0x12c << 2));
	return r->tim2_fade_end_if;
}

static inline void pwm_ll_set_reg12c_tim2_fade_end_int_en(uint32_t v) {
	pwm_reg12c_t *r = (pwm_reg12c_t*)(SOC_PWM_REG_BASE + (0x12c << 2));
	r->tim2_fade_end_int_en = v;
}

static inline uint32_t pwm_ll_get_reg12c_tim2_fade_end_int_en(void) {
	pwm_reg12c_t *r = (pwm_reg12c_t*)(SOC_PWM_REG_BASE + (0x12c << 2));
	return r->tim2_fade_end_int_en;
}

static inline void pwm_ll_set_reg12c_tim2_fade_inc_dec(uint32_t v) {
	pwm_reg12c_t *r = (pwm_reg12c_t*)(SOC_PWM_REG_BASE + (0x12c << 2));
	r->tim2_fade_inc_dec = v;
}

static inline uint32_t pwm_ll_get_reg12c_tim2_fade_inc_dec(void) {
	pwm_reg12c_t *r = (pwm_reg12c_t*)(SOC_PWM_REG_BASE + (0x12c << 2));
	return r->tim2_fade_inc_dec;
}

static inline void pwm_ll_set_reg12c_tim2_duty_fading_en(uint32_t v) {
	pwm_reg12c_t *r = (pwm_reg12c_t*)(SOC_PWM_REG_BASE + (0x12c << 2));
	r->tim2_duty_fading_en = v;
}

static inline uint32_t pwm_ll_get_reg12c_tim2_duty_fading_en(void) {
	pwm_reg12c_t *r = (pwm_reg12c_t*)(SOC_PWM_REG_BASE + (0x12c << 2));
	return r->tim2_duty_fading_en;
}

//reg reg12d:

static inline void pwm_ll_set_reg12d_value(uint32_t v) {
	pwm_reg12d_t *r = (pwm_reg12d_t*)(SOC_PWM_REG_BASE + (0x12d << 2));
	r->v = v;
}

static inline uint32_t pwm_ll_get_reg12d_value(void) {
	pwm_reg12d_t *r = (pwm_reg12d_t*)(SOC_PWM_REG_BASE + (0x12d << 2));
	return r->v;
}

static inline void pwm_ll_set_reg12d_tim3_fade_num(uint32_t v) {
	pwm_reg12d_t *r = (pwm_reg12d_t*)(SOC_PWM_REG_BASE + (0x12d << 2));
	r->tim3_fade_num = v;
}

static inline uint32_t pwm_ll_get_reg12d_tim3_fade_num(void) {
	pwm_reg12d_t *r = (pwm_reg12d_t*)(SOC_PWM_REG_BASE + (0x12d << 2));
	return r->tim3_fade_num;
}

static inline void pwm_ll_set_reg12d_tim3_fade_intval_cyc(uint32_t v) {
	pwm_reg12d_t *r = (pwm_reg12d_t*)(SOC_PWM_REG_BASE + (0x12d << 2));
	r->tim3_fade_intval_cyc = v;
}

static inline uint32_t pwm_ll_get_reg12d_tim3_fade_intval_cyc(void) {
	pwm_reg12d_t *r = (pwm_reg12d_t*)(SOC_PWM_REG_BASE + (0x12d << 2));
	return r->tim3_fade_intval_cyc;
}

static inline void pwm_ll_set_reg12d_tim3_fade_scale(uint32_t v) {
	pwm_reg12d_t *r = (pwm_reg12d_t*)(SOC_PWM_REG_BASE + (0x12d << 2));
	r->tim3_fade_scale = v;
}

static inline uint32_t pwm_ll_get_reg12d_tim3_fade_scale(void) {
	pwm_reg12d_t *r = (pwm_reg12d_t*)(SOC_PWM_REG_BASE + (0x12d << 2));
	return r->tim3_fade_scale;
}

static inline void pwm_ll_set_reg12d_tim3_fade_end_if(uint32_t v) {
	pwm_reg12d_t *r = (pwm_reg12d_t*)(SOC_PWM_REG_BASE + (0x12d << 2));
	r->tim3_fade_end_if = v;
}

static inline uint32_t pwm_ll_get_reg12d_tim3_fade_end_if(void) {
	pwm_reg12d_t *r = (pwm_reg12d_t*)(SOC_PWM_REG_BASE + (0x12d << 2));
	return r->tim3_fade_end_if;
}

static inline void pwm_ll_set_reg12d_tim3_fade_end_int_en(uint32_t v) {
	pwm_reg12d_t *r = (pwm_reg12d_t*)(SOC_PWM_REG_BASE + (0x12d << 2));
	r->tim3_fade_end_int_en = v;
}

static inline uint32_t pwm_ll_get_reg12d_tim3_fade_end_int_en(void) {
	pwm_reg12d_t *r = (pwm_reg12d_t*)(SOC_PWM_REG_BASE + (0x12d << 2));
	return r->tim3_fade_end_int_en;
}

static inline void pwm_ll_set_reg12d_tim3_fade_inc_dec(uint32_t v) {
	pwm_reg12d_t *r = (pwm_reg12d_t*)(SOC_PWM_REG_BASE + (0x12d << 2));
	r->tim3_fade_inc_dec = v;
}

static inline uint32_t pwm_ll_get_reg12d_tim3_fade_inc_dec(void) {
	pwm_reg12d_t *r = (pwm_reg12d_t*)(SOC_PWM_REG_BASE + (0x12d << 2));
	return r->tim3_fade_inc_dec;
}

static inline void pwm_ll_set_reg12d_tim3_duty_fading_en(uint32_t v) {
	pwm_reg12d_t *r = (pwm_reg12d_t*)(SOC_PWM_REG_BASE + (0x12d << 2));
	r->tim3_duty_fading_en = v;
}

static inline uint32_t pwm_ll_get_reg12d_tim3_duty_fading_en(void) {
	pwm_reg12d_t *r = (pwm_reg12d_t*)(SOC_PWM_REG_BASE + (0x12d << 2));
	return r->tim3_duty_fading_en;
}

#ifdef __cplusplus
}
#endif
