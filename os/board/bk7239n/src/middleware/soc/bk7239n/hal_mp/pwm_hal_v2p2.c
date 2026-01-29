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

#include "pwm_hal_v2p2.h"
#include "pwm_ll.h"
#include "pwm_hw.h"
#include "driver/pwm_types.h"

#define TAG "pwm"

bk_err_t pwm_hal_init(void)
{
	pwm_ll_set_reg2_soft_reset(0);
	for(volatile uint32_t i = 0; i < 100; i++);
	pwm_ll_set_reg2_soft_reset(1);

	return BK_OK;
}

bk_err_t pwm_hal_deinit(void)
{
	return BK_OK;
}

bk_err_t pwm_hal_chan_alloc_io(pwm_chan_t sw_ch, pwm_chan_t tim_ch)
{
	REG_WRITE((PWM_REG10_ADDR + (sw_ch << 2)), tim_ch);

	return BK_OK;
}

bk_err_t pwm_hal_chan_alloc_capture_io(pwm_chan_t sw_ch, pwm_chan_t tim_cap_ch)
{
	REG_WRITE((PWM_REG20_ADDR + (tim_cap_ch << 2)), sw_ch);

	return BK_OK;
}

bk_err_t pwm_hal_set_polarity(pwm_chan_t sw_ch, pwm_polarity_t polarity)
{
	uint32_t group_id = sw_ch / SOC_PWM_CHAN_NUM_PER_GROUP;
	uint32_t group_ch = sw_ch % SOC_PWM_CHAN_NUM_PER_GROUP;

	uint32_t reg_val = REG_READ(PWM_REG46_ADDR + ((0x40 * group_id) << 2));
	reg_val &= ~(PWM_REG46_CH1P_MASK << (PWM_REG46_CH1P_POS + 4 * group_ch));
	reg_val |= (polarity << (PWM_REG46_CH1P_POS + 4 * group_ch));
	REG_WRITE(PWM_REG46_ADDR + ((0x40 * group_id) << 2), reg_val);

	return BK_OK;
}

bk_err_t pwm_hal_set_ocm(pwm_chan_t sw_ch, uint8_t ocm_val)
{
	uint32_t group_id = sw_ch / SOC_PWM_CHAN_NUM_PER_GROUP;
	uint32_t group_ch = sw_ch % SOC_PWM_CHAN_NUM_PER_GROUP;

	uint32_t reg_val = REG_READ(PWM_REG46_ADDR + ((0x40 * group_id) << 2));
	reg_val &= ~(PWM_REG46_OC1M_MASK << (PWM_REG46_OC1M_POS + 3 * group_ch));
	reg_val |= (ocm_val << (PWM_REG46_OC1M_POS + 3 * group_ch));
	REG_WRITE(PWM_REG46_ADDR + ((0x40 * group_id) << 2), reg_val);

	return BK_OK;
}

bk_err_t pwm_hal_set_init_level(pwm_chan_t sw_ch, uint8_t init_level)
{
	uint32_t group_id = sw_ch / SOC_PWM_CHAN_NUM_PER_GROUP;
	uint32_t group_ch = sw_ch % SOC_PWM_CHAN_NUM_PER_GROUP;

	uint32_t reg_val = REG_READ(PWM_REG45_ADDR + ((0x40 * group_id) << 2));
	reg_val &= ~(PWM_REG45_INIT_LEVEL1_MASK << (PWM_REG45_INIT_LEVEL1_POS + group_ch));
	reg_val |= (init_level << (PWM_REG45_INIT_LEVEL1_POS + group_ch));
	REG_WRITE(PWM_REG45_ADDR + ((0x40 * group_id) << 2), reg_val);

	return BK_OK;
}

bk_err_t pwm_hal_set_ocpe(pwm_chan_t sw_ch, uint8_t ocpe_val)
{
	uint32_t group_id = sw_ch / SOC_PWM_CHAN_NUM_PER_GROUP;
	uint32_t group_ch = sw_ch % SOC_PWM_CHAN_NUM_PER_GROUP;

	uint32_t reg_val = REG_READ(PWM_REG40_ADDR + ((0x40 * group_id) << 2));
	reg_val &= ~(PWM_REG40_OC1PE_MASK << (PWM_REG40_OC1PE_POS - group_ch));
	reg_val |= (ocpe_val << (PWM_REG40_OC1PE_POS - group_ch));
	REG_WRITE(PWM_REG40_ADDR + ((0x40 * group_id) << 2), reg_val);

	return BK_OK;
}

bk_err_t pwm_hal_set_arpe(pwm_chan_t sw_ch, uint8_t arpe_val)
{
	uint32_t group_id = sw_ch / SOC_PWM_CHAN_NUM_PER_GROUP;
	uint32_t group_ch = sw_ch % SOC_PWM_CHAN_NUM_PER_GROUP;

	uint32_t reg_val = REG_READ(PWM_REG40_ADDR + ((0x40 * group_id) << 2));
	reg_val &= ~(PWM_REG40_ARPE1_MASK << (PWM_REG40_ARPE1_POS - group_ch));
	reg_val |= (arpe_val << (PWM_REG40_ARPE1_POS - group_ch));
	REG_WRITE(PWM_REG40_ADDR + ((0x40 * group_id) << 2), reg_val);

	return BK_OK;
}

bk_err_t pwm_hal_set_ocmpe(pwm_chan_t sw_ch, uint8_t ocmpe_val)
{
	uint32_t group_id = sw_ch / SOC_PWM_CHAN_NUM_PER_GROUP;
	uint32_t group_ch = sw_ch % SOC_PWM_CHAN_NUM_PER_GROUP;

	uint32_t reg_val = REG_READ(PWM_REG40_ADDR + ((0x40 * group_id) << 2));
	reg_val &= ~(PWM_REG40_OCM1PE_MASK << (PWM_REG40_OCM1PE_POS - group_ch));
	reg_val |= (ocmpe_val << (PWM_REG40_OCM1PE_POS - group_ch));
	REG_WRITE(PWM_REG40_ADDR + ((0x40 * group_id) << 2), reg_val);

	return BK_OK;
}

bk_err_t pwm_hal_set_single_chan_tim_enable(pwm_chan_t sw_ch, uint32_t value)
{
	uint32_t reg_val = REG_READ(PWM_REG4_ADDR);
	reg_val &= ~(PWM_REG4_CEN1_MASK << sw_ch);
	reg_val |= (value << sw_ch);
	REG_WRITE(PWM_REG4_ADDR, reg_val);

	return BK_OK;
}

bk_err_t pwm_hal_set_multi_chan_tim_enable(uint32_t multi_ch, uint32_t value)
{
	uint32_t reg_val = pwm_ll_get_reg4_value();
	for (uint32_t i = 0; i < SOC_PWM_TIM_NUM; i++) {
		if (multi_ch & BIT(i)) {
			reg_val |= BIT(i);
		}
	}
	pwm_ll_set_reg4_value(reg_val);

	return BK_OK;
}

bk_err_t pwm_hal_set_single_chan_enable(pwm_chan_t sw_ch, uint32_t value)
{
	switch(sw_ch) {
	case 0:
		pwm_ll_set_reg46_ch1e(value);
		break;
	case 1:
		pwm_ll_set_reg46_ch3e(value);
		break;
	case 2:
		pwm_ll_set_reg46_ch5e(value);
		break;
	case 3:
		pwm_ll_set_reg86_ch1e(value);
		break;
	case 4:
		pwm_ll_set_reg86_ch3e(value);
		break;
	case 5:
		pwm_ll_set_reg86_ch5e(value);
		break;
	case 6:
		pwm_ll_set_regc6_ch1e(value);
		break;
	case 7:
		pwm_ll_set_regc6_ch3e(value);
		break;
	case 8:
		pwm_ll_set_regc6_ch5e(value);
		break;
	case 9:
		pwm_ll_set_reg106_ch1e(value);
		break;
	case 10:
		pwm_ll_set_reg106_ch3e(value);
		break;
	case 11:
		pwm_ll_set_reg106_ch5e(value);
		break;
	default:
		break;
	}

	return BK_OK;
}

bk_err_t pwm_hal_set_chan_config(pwm_chan_t sw_ch, const pwm_period_duty_config_t *config)
{
	switch(sw_ch) {
	case 0:
		pwm_ll_set_reg4b_tim1_arr(config->period_cycle - 1);
		pwm_ll_set_reg51_ccr1(config->duty_cycle);
		pwm_ll_set_reg52_ccr2(config->duty2_cycle);
		pwm_ll_set_reg53_ccr3(config->duty3_cycle);
		pwm_ll_set_reg4a_psc1(config->psc);
		pwm_ll_set_reg46_ch1e(1);
		break;
	case 1:
		pwm_ll_set_reg4c_tim2_arr(config->period_cycle - 1);
		pwm_ll_set_reg54_ccr4(config->duty_cycle);
		pwm_ll_set_reg55_ccr5(config->duty2_cycle);
		pwm_ll_set_reg56_ccr6(config->duty3_cycle);
		pwm_ll_set_reg4a_psc2(config->psc);
		pwm_ll_set_reg46_ch3e(1);
		break;
	case 2:
		pwm_ll_set_reg4d_tim3_arr(config->period_cycle - 1);
		pwm_ll_set_reg57_ccr7(config->duty_cycle);
		pwm_ll_set_reg58_ccr8(config->duty2_cycle);
		pwm_ll_set_reg59_ccr9(config->duty3_cycle);
		pwm_ll_set_reg4a_psc3(config->psc);
		pwm_ll_set_reg46_ch5e(1);
		break;
	case 3:
		pwm_ll_set_reg8b_tim1_arr(config->period_cycle - 1);
		pwm_ll_set_reg91_ccr1(config->duty_cycle);
		pwm_ll_set_reg92_ccr2(config->duty2_cycle);
		pwm_ll_set_reg93_ccr3(config->duty3_cycle);
		pwm_ll_set_reg8a_psc1(config->psc);
		pwm_ll_set_reg86_ch1e(1);
		break;
	case 4:
		pwm_ll_set_reg8c_tim2_arr(config->period_cycle - 1);
		pwm_ll_set_reg94_ccr4(config->duty_cycle);
		pwm_ll_set_reg95_ccr5(config->duty2_cycle);
		pwm_ll_set_reg96_ccr6(config->duty3_cycle);
		pwm_ll_set_reg8a_psc2(config->psc);
		pwm_ll_set_reg86_ch3e(1);
		break;
	case 5:
		pwm_ll_set_reg8d_tim3_arr(config->period_cycle - 1);
		pwm_ll_set_reg97_ccr7(config->duty_cycle);
		pwm_ll_set_reg98_ccr8(config->duty2_cycle);
		pwm_ll_set_reg99_ccr9(config->duty3_cycle);
		pwm_ll_set_reg8a_psc3(config->psc);
		pwm_ll_set_reg86_ch5e(1);
		break;
	case 6:
		pwm_ll_set_regcb_tim1_arr(config->period_cycle - 1);
		pwm_ll_set_regd1_ccr1(config->duty_cycle);
		pwm_ll_set_regd2_ccr2(config->duty2_cycle);
		pwm_ll_set_regd3_ccr3(config->duty3_cycle);
		pwm_ll_set_regca_psc1(config->psc);
		pwm_ll_set_regc6_ch1e(1);
		break;
	case 7:
		pwm_ll_set_regcc_tim2_arr(config->period_cycle - 1);
		pwm_ll_set_regd4_ccr4(config->duty_cycle);
		pwm_ll_set_regd5_ccr5(config->duty2_cycle);
		pwm_ll_set_regd6_ccr6(config->duty3_cycle);
		pwm_ll_set_regca_psc2(config->psc);
		pwm_ll_set_regc6_ch3e(1);
		break;
	case 8:
		pwm_ll_set_regcd_tim3_arr(config->period_cycle - 1);
		pwm_ll_set_regd7_ccr7(config->duty_cycle);
		pwm_ll_set_regd8_ccr8(config->duty2_cycle);
		pwm_ll_set_regd9_ccr9(config->duty3_cycle);
		pwm_ll_set_regca_psc3(config->psc);
		pwm_ll_set_regc6_ch5e(1);
		break;
	case 9:
		pwm_ll_set_reg10b_tim1_arr(config->period_cycle - 1);
		pwm_ll_set_reg111_ccr1(config->duty_cycle);
		pwm_ll_set_reg112_ccr2(config->duty2_cycle);
		pwm_ll_set_reg113_ccr3(config->duty3_cycle);
		pwm_ll_set_reg10a_psc1(config->psc);
		pwm_ll_set_reg106_ch1e(1);
		break;
	case 10:
		pwm_ll_set_reg10c_tim2_arr(config->period_cycle - 1);
		pwm_ll_set_reg114_ccr4(config->duty_cycle);
		pwm_ll_set_reg115_ccr5(config->duty2_cycle);
		pwm_ll_set_reg116_ccr6(config->duty3_cycle);
		pwm_ll_set_reg10a_psc2(config->psc);
		pwm_ll_set_reg106_ch3e(1);
		break;
	case 11:
		pwm_ll_set_reg10d_tim3_arr(config->period_cycle - 1);
		pwm_ll_set_reg117_ccr7(config->duty_cycle);
		pwm_ll_set_reg118_ccr8(config->duty2_cycle);
		pwm_ll_set_reg119_ccr9(config->duty3_cycle);
		pwm_ll_set_reg10a_psc3(config->psc);
		pwm_ll_set_reg106_ch5e(1);
		break;
	default:
		break;
	}

	return BK_OK;
}

bk_err_t pwm_hal_set_group_config(const pwm_period_duty_config_t *config, pwm_chan_t tim_id, uint32_t dead_cycle)
{
	switch (tim_id) {
	case 0:
		pwm_ll_set_reg4a_psc1(config->psc);
		pwm_ll_set_reg4b_tim1_arr(config->period_cycle - 1);
		pwm_ll_set_reg51_ccr1(config->duty_cycle);
		pwm_ll_set_reg52_ccr2(config->duty2_cycle);
		pwm_ll_set_reg5a_dt1(dead_cycle);
		pwm_ll_set_reg46_ch1p(0);
		pwm_ll_set_reg46_ch2p(3);
		pwm_ll_set_reg46_ch1e(1);
		pwm_ll_set_reg46_ch2e(1);
		break;
	case 1:
		pwm_ll_set_reg4a_psc2(config->psc);
		pwm_ll_set_reg4c_tim2_arr(config->period_cycle - 1);
		pwm_ll_set_reg54_ccr4(config->duty_cycle);
		pwm_ll_set_reg55_ccr5(config->duty2_cycle);
		pwm_ll_set_reg5a_dt2(dead_cycle);
		pwm_ll_set_reg46_ch3p(0);
		pwm_ll_set_reg46_ch4p(3);
		pwm_ll_set_reg46_ch3e(1);
		pwm_ll_set_reg46_ch4e(1);
		break;
	case 2:
		pwm_ll_set_reg4a_psc3(config->psc);
		pwm_ll_set_reg4d_tim3_arr(config->period_cycle - 1);
		pwm_ll_set_reg57_ccr7(config->duty_cycle);
		pwm_ll_set_reg58_ccr8(config->duty2_cycle);
		pwm_ll_set_reg5a_dt3(dead_cycle);
		pwm_ll_set_reg46_ch5p(0);
		pwm_ll_set_reg46_ch6p(3);
		pwm_ll_set_reg46_ch5e(1);
		pwm_ll_set_reg46_ch6e(1);
		break;
	case 3:
		pwm_ll_set_reg8a_psc1(config->psc);
		pwm_ll_set_reg8b_tim1_arr(config->period_cycle - 1);
		pwm_ll_set_reg91_ccr1(config->duty_cycle);
		pwm_ll_set_reg92_ccr2(config->duty2_cycle);
		pwm_ll_set_reg9a_dt1(dead_cycle);
		pwm_ll_set_reg86_ch1p(0);
		pwm_ll_set_reg86_ch2p(3);
		pwm_ll_set_reg86_ch1e(1);
		pwm_ll_set_reg86_ch2e(1);
		break;
	case 4:
		pwm_ll_set_reg8a_psc2(config->psc);
		pwm_ll_set_reg8c_tim2_arr(config->period_cycle - 1);
		pwm_ll_set_reg94_ccr4(config->duty_cycle);
		pwm_ll_set_reg95_ccr5(config->duty2_cycle);
		pwm_ll_set_reg9a_dt2(dead_cycle);
		pwm_ll_set_reg86_ch3p(0);
		pwm_ll_set_reg86_ch4p(3);
		pwm_ll_set_reg86_ch3e(1);
		pwm_ll_set_reg86_ch4e(1);
		break;
	case 5:
		pwm_ll_set_reg8a_psc3(config->psc);
		pwm_ll_set_reg8d_tim3_arr(config->period_cycle - 1);
		pwm_ll_set_reg97_ccr7(config->duty_cycle);
		pwm_ll_set_reg98_ccr8(config->duty2_cycle);
		pwm_ll_set_reg9a_dt3(dead_cycle);
		pwm_ll_set_reg86_ch5p(0);
		pwm_ll_set_reg86_ch6p(3);
		pwm_ll_set_reg86_ch5e(1);
		pwm_ll_set_reg86_ch6e(1);
		break;
	case 6:
		pwm_ll_set_regca_psc1(config->psc);
		pwm_ll_set_regcb_tim1_arr(config->period_cycle - 1);
		pwm_ll_set_regd1_ccr1(config->duty_cycle);
		pwm_ll_set_regd2_ccr2(config->duty2_cycle);
		pwm_ll_set_regda_dt1(dead_cycle);
		pwm_ll_set_regc6_ch1p(0);
		pwm_ll_set_regc6_ch2p(3);
		pwm_ll_set_regc6_ch1e(1);
		pwm_ll_set_regc6_ch2e(1);
		break;
	case 7:
		pwm_ll_set_regca_psc2(config->psc);
		pwm_ll_set_regcc_tim2_arr(config->period_cycle - 1);
		pwm_ll_set_regd4_ccr4(config->duty_cycle);
		pwm_ll_set_regd5_ccr5(config->duty2_cycle);
		pwm_ll_set_regda_dt2(dead_cycle);
		pwm_ll_set_regc6_ch3p(0);
		pwm_ll_set_regc6_ch4p(3);
		pwm_ll_set_regc6_ch3e(1);
		pwm_ll_set_regc6_ch4e(1);
		break;
	case 8:
		pwm_ll_set_regca_psc3(config->psc);
		pwm_ll_set_regcd_tim3_arr(config->period_cycle - 1);
		pwm_ll_set_regd7_ccr7(config->duty_cycle);
		pwm_ll_set_regd8_ccr8(config->duty2_cycle);
		pwm_ll_set_regda_dt3(dead_cycle);
		pwm_ll_set_regc6_ch5p(0);
		pwm_ll_set_regc6_ch6p(3);
		pwm_ll_set_regc6_ch5e(1);
		pwm_ll_set_regc6_ch6e(1);
		break;
	case 9:
		pwm_ll_set_reg10a_psc1(config->psc);
		pwm_ll_set_reg10b_tim1_arr(config->period_cycle - 1);
		pwm_ll_set_reg111_ccr1(config->duty_cycle);
		pwm_ll_set_reg112_ccr2(config->duty2_cycle);
		pwm_ll_set_reg11a_dt1(dead_cycle);
		pwm_ll_set_reg106_ch1p(0);
		pwm_ll_set_reg106_ch2p(3);
		pwm_ll_set_reg106_ch1e(1);
		pwm_ll_set_reg106_ch2e(1);
		break;
	case 10:
		pwm_ll_set_reg10a_psc2(config->psc);
		pwm_ll_set_reg10c_tim2_arr(config->period_cycle - 1);
		pwm_ll_set_reg114_ccr4(config->duty_cycle);
		pwm_ll_set_reg115_ccr5(config->duty2_cycle);
		pwm_ll_set_reg11a_dt2(dead_cycle);
		pwm_ll_set_reg106_ch3p(0);
		pwm_ll_set_reg106_ch4p(3);
		pwm_ll_set_reg106_ch3e(1);
		pwm_ll_set_reg106_ch4e(1);
		break;
	case 11:
		pwm_ll_set_reg10a_psc3(config->psc);
		pwm_ll_set_reg10d_tim3_arr(config->period_cycle - 1);
		pwm_ll_set_reg117_ccr7(config->duty_cycle);
		pwm_ll_set_reg118_ccr8(config->duty2_cycle);
		pwm_ll_set_reg11a_dt3(dead_cycle);
		pwm_ll_set_reg106_ch5p(0);
		pwm_ll_set_reg106_ch6p(3);
		pwm_ll_set_reg106_ch5e(1);
		pwm_ll_set_reg106_ch6e(1);
		break;
	default:
		break;
	}

	return BK_OK;
}

uint32_t pwm_hal_get_ccmr_chan_polarity(pwm_chan_t chan)
{
	uint32_t input_polarity = 0;

	switch(chan) {
	case 0:
		input_polarity = pwm_ll_get_reg46_ch1p();
		break;
	case 1:
		input_polarity = pwm_ll_get_reg46_ch3p();
		break;
	case 2:
		input_polarity = pwm_ll_get_reg46_ch5p();
		break;
	case 3:
		input_polarity = pwm_ll_get_reg86_ch1p();
		break;
	case 4:
		input_polarity = pwm_ll_get_reg86_ch3p();
		break;
	case 5:
		input_polarity = pwm_ll_get_reg86_ch5p();
		break;
	case 6:
		input_polarity = pwm_ll_get_regc6_ch1p();
		break;
	case 7:
		input_polarity = pwm_ll_get_regc6_ch3p();
		break;
	case 8:
		input_polarity = pwm_ll_get_regc6_ch5p();
		break;
	case 9:
		input_polarity = pwm_ll_get_reg106_ch1p();
		break;
	case 10:
		input_polarity = pwm_ll_get_reg106_ch3p();
		break;
	case 11:
		input_polarity = pwm_ll_get_reg106_ch5p();
		break;
	default:
		break;
	}

	if (input_polarity == 0) {
		return PWM_CAPTURE_POS;
	} else if (input_polarity == 1) {
		return PWM_CAPTURE_NEG;
	} else {
		return PWM_CAPTURE_EDGE;
	}
}

bk_err_t pwm_hal_init_capture(pwm_chan_t chan, pwm_capture_edge_t edge)
{
	uint32_t input_polarity = 0;
	if (edge == PWM_CAPTURE_POS) {
		input_polarity = 0;
	} else if (edge == PWM_CAPTURE_NEG) {
		input_polarity = 1;
	} else {
		input_polarity = 2;
	}

	switch(chan) {
	case 0:
		pwm_ll_set_reg4a_psc1(0x0);
		pwm_ll_set_reg4b_tim1_arr(0xffffffff);
		pwm_ll_set_reg40_urs1(1);
		pwm_ll_set_reg46_ch1p(input_polarity);
		pwm_ll_set_reg46_ch1e(1);
		pwm_ll_set_reg46_tim1ccm(1); // CAPTURE mode
		pwm_ll_set_reg42_sms1(0x5);  // clear timer every capture
		pwm_ll_set_reg42_ts1(0x0);   // select pwm_i[0]
		break;
	case 1:
		pwm_ll_set_reg4a_psc2(0x0);
		pwm_ll_set_reg4c_tim2_arr(0xffffffff);
		pwm_ll_set_reg40_urs2(1);
		pwm_ll_set_reg46_ch3p(input_polarity);
		pwm_ll_set_reg46_ch3e(1);
		pwm_ll_set_reg46_tim2ccm(1); // CAPTURE mode
		pwm_ll_set_reg42_sms2(0x5);  // clear timer every capture
		pwm_ll_set_reg42_ts2(0x0);   // select pwm_i[2]
		break;
	case 2:
		pwm_ll_set_reg4a_psc3(0x0);
		pwm_ll_set_reg4d_tim3_arr(0xffffffff);
		pwm_ll_set_reg40_urs3(1);
		pwm_ll_set_reg46_ch5p(input_polarity);
		pwm_ll_set_reg46_ch5e(1);
		pwm_ll_set_reg46_tim3ccm(1); // CAPTURE mode
		pwm_ll_set_reg42_sms3(0x5);  // clear timer every capture
		pwm_ll_set_reg42_ts3(0x0);   // select pwm_i[4]
		break;
	case 3:
		pwm_ll_set_reg8a_psc1(0x0);
		pwm_ll_set_reg8b_tim1_arr(0xffffffff);
		pwm_ll_set_reg80_urs1(1);
		pwm_ll_set_reg86_ch1p(input_polarity);
		pwm_ll_set_reg86_ch1e(1);
		pwm_ll_set_reg86_tim1ccm(1); // CAPTURE mode
		pwm_ll_set_reg82_sms1(0x5);  // clear timer every capture
		pwm_ll_set_reg82_ts1(0x0);   // select pwm_i[0]
		break;
	case 4:
		pwm_ll_set_reg8a_psc2(0x0);
		pwm_ll_set_reg8c_tim2_arr(0xffffffff);
		pwm_ll_set_reg80_urs2(1);
		pwm_ll_set_reg86_ch3p(input_polarity);
		pwm_ll_set_reg86_ch3e(1);
		pwm_ll_set_reg86_tim2ccm(1); // CAPTURE mode
		pwm_ll_set_reg82_sms2(0x5);  // clear timer every capture
		pwm_ll_set_reg82_ts2(0x0);   // select pwm_i[2]
		break;
	case 5:
		pwm_ll_set_reg8a_psc3(0x0);
		pwm_ll_set_reg8d_tim3_arr(0xffffffff);
		pwm_ll_set_reg80_urs3(1);
		pwm_ll_set_reg86_ch5p(input_polarity);
		pwm_ll_set_reg86_ch5e(1);
		pwm_ll_set_reg86_tim3ccm(1); // CAPTURE mode
		pwm_ll_set_reg82_sms3(0x5);  // clear timer every capture
		pwm_ll_set_reg82_ts3(0x0);   // select pwm_i[4]
		break;
	case 6:
		pwm_ll_set_regca_psc1(0x0);
		pwm_ll_set_regcb_tim1_arr(0xffffffff);
		pwm_ll_set_regc0_urs1(1);
		pwm_ll_set_regc6_ch1p(input_polarity);
		pwm_ll_set_regc6_ch1e(1);
		pwm_ll_set_regc6_tim1ccm(1); // CAPTURE mode
		pwm_ll_set_regc2_sms1(0x5);  // clear timer every capture
		pwm_ll_set_regc2_ts1(0x0);   // select pwm_i[0]
		break;
	case 7:
		pwm_ll_set_regca_psc2(0x0);
		pwm_ll_set_regcc_tim2_arr(0xffffffff);
		pwm_ll_set_regc0_urs2(1);
		pwm_ll_set_regc6_ch3p(input_polarity);
		pwm_ll_set_regc6_ch3e(1);
		pwm_ll_set_regc6_tim2ccm(1); // CAPTURE mode
		pwm_ll_set_regc2_sms2(0x5);  // clear timer every capture
		pwm_ll_set_regc2_ts2(0x0);   // select pwm_i[2]
		break;
	case 8:
		pwm_ll_set_regca_psc3(0x0);
		pwm_ll_set_regcd_tim3_arr(0xffffffff);
		pwm_ll_set_regc0_urs3(1);
		pwm_ll_set_regc6_ch5p(input_polarity);
		pwm_ll_set_regc6_ch5e(1);
		pwm_ll_set_regc6_tim3ccm(1); // CAPTURE mode
		pwm_ll_set_regc2_sms3(0x5);  // clear timer every capture
		pwm_ll_set_regc2_ts3(0x0);   // select pwm_i[4]
		break;
	case 9:
		pwm_ll_set_reg10a_psc1(0x0);
		pwm_ll_set_reg10b_tim1_arr(0xffffffff);
		pwm_ll_set_reg100_urs1(1);
		pwm_ll_set_reg106_ch1p(input_polarity);
		pwm_ll_set_reg106_ch1e(1);
		pwm_ll_set_reg106_tim1ccm(1); // CAPTURE mode
		pwm_ll_set_reg102_sms1(0x5);  // clear timer every capture
		pwm_ll_set_reg102_ts1(0x0);   // select pwm_i[0]
		break;
	case 10:
		pwm_ll_set_reg10a_psc2(0x0);
		pwm_ll_set_reg10c_tim2_arr(0xffffffff);
		pwm_ll_set_reg100_urs2(1);
		pwm_ll_set_reg106_ch3p(input_polarity);
		pwm_ll_set_reg106_ch3e(1);
		pwm_ll_set_reg106_tim2ccm(1); // CAPTURE mode
		pwm_ll_set_reg102_sms2(0x5);  // clear timer every capture
		pwm_ll_set_reg102_ts2(0x0);   // select pwm_i[2]
		break;
	case 11:
		pwm_ll_set_reg10a_psc3(0x0);
		pwm_ll_set_reg10d_tim3_arr(0xffffffff);
		pwm_ll_set_reg100_urs3(1);
		pwm_ll_set_reg106_ch5p(input_polarity);
		pwm_ll_set_reg106_ch5e(1);
		pwm_ll_set_reg106_tim3ccm(1); // CAPTURE mode
		pwm_ll_set_reg102_sms3(0x5);  // clear timer every capture
		pwm_ll_set_reg102_ts3(0x0);   // select pwm_i[4]
		break;
	default:
		break;
	}

	return BK_OK;
}

bk_err_t pwm_hal_set_uie(pwm_chan_t tim_id, uint32_t value)
{
	switch (tim_id) {
	case 0:
		pwm_ll_set_reg43_uie1(value);
		break;
	case 1:
		pwm_ll_set_reg43_uie2(value);
		break;
	case 2:
		pwm_ll_set_reg43_uie3(value);
		break;
	case 3:
		pwm_ll_set_reg83_uie1(value);
		break;
	case 4:
		pwm_ll_set_reg83_uie2(value);
		break;
	case 5:
		pwm_ll_set_reg83_uie3(value);
		break;
	case 6:
		pwm_ll_set_regc3_uie1(value);
		break;
	case 7:
		pwm_ll_set_regc3_uie2(value);
		break;
	case 8:
		pwm_ll_set_regc3_uie3(value);
		break;
	case 9:
		pwm_ll_set_reg103_uie1(value);
		break;
	case 10:
		pwm_ll_set_reg103_uie2(value);
		break;
	case 11:
		pwm_ll_set_reg103_uie3(value);
		break;
	default:
		break;
	}

	return BK_OK;
}

bk_err_t pwm_hal_set_cc1ie(pwm_chan_t tim_id, uint32_t value)
{
	switch (tim_id) {
	case 0:
		pwm_ll_set_reg43_cc1ie(value);
		break;
	case 1:
		pwm_ll_set_reg43_cc4ie(value);
		break;
	case 2:
		pwm_ll_set_reg43_cc7ie(value);
		break;
	case 3:
		pwm_ll_set_reg83_cc1ie(value);
		break;
	case 4:
		pwm_ll_set_reg83_cc4ie(value);
		break;
	case 5:
		pwm_ll_set_reg83_cc7ie(value);
		break;
	case 6:
		pwm_ll_set_regc3_cc1ie(value);
		break;
	case 7:
		pwm_ll_set_regc3_cc4ie(value);
		break;
	case 8:
		pwm_ll_set_regc3_cc7ie(value);
		break;
	case 9:
		pwm_ll_set_reg103_cc1ie(value);
		break;
	case 10:
		pwm_ll_set_reg103_cc4ie(value);
		break;
	case 11:
		pwm_ll_set_reg103_cc7ie(value);
		break;
	default:
		break;
	}

	return BK_OK;
}

uint32_t pwm_hal_get_ccr1_shadow(pwm_chan_t tim_id)
{
	switch (tim_id) {
	case 0:
		return pwm_ll_get_reg61_ccr1_shad();
	case 1:
		return pwm_ll_get_reg64_ccr4_shad();
	case 2:
		return pwm_ll_get_reg67_ccr7_shad();
	case 3:
		return pwm_ll_get_rega1_ccr1_shad();
	case 4:
		return pwm_ll_get_rega4_ccr4_shad();
	case 5:
		return pwm_ll_get_rega7_ccr7_shad();
	case 6:
		return pwm_ll_get_rege1_ccr1_shad();
	case 7:
		return pwm_ll_get_rege4_ccr4_shad();
	case 8:
		return pwm_ll_get_rege7_ccr7_shad();
	case 9:
		return pwm_ll_get_reg121_ccr1_shad();
	case 10:
		return pwm_ll_get_reg124_ccr4_shad();
	case 11:
		return pwm_ll_get_reg127_ccr7_shad();
	default:
		return 0;
	}

	return 0;
}

uint32_t pwm_hal_get_ccr2_shadow(pwm_chan_t tim_id)
{
	switch (tim_id) {
	case 0:
		return pwm_ll_get_reg62_ccr2_shad();
	case 1:
		return pwm_ll_get_reg65_ccr5_shad();
	case 2:
		return pwm_ll_get_reg68_ccr8_shad();
	case 3:
		return pwm_ll_get_rega2_ccr2_shad();
	case 4:
		return pwm_ll_get_rega5_ccr5_shad();
	case 5:
		return pwm_ll_get_rega8_ccr8_shad();
	case 6:
		return pwm_ll_get_rege2_ccr2_shad();
	case 7:
		return pwm_ll_get_rege5_ccr5_shad();
	case 8:
		return pwm_ll_get_rege8_ccr8_shad();
	case 9:
		return pwm_ll_get_reg122_ccr2_shad();
	case 10:
		return pwm_ll_get_reg125_ccr5_shad();
	case 11:
		return pwm_ll_get_reg128_ccr8_shad();
	default:
		return 0;
	}

	return 0;
}

uint32_t pwm_hal_get_ccr2(pwm_chan_t tim_id)
{
	switch (tim_id) {
	case 0:
		return pwm_ll_get_reg52_ccr2();
	case 1:
		return pwm_ll_get_reg55_ccr5();
	case 2:
		return pwm_ll_get_reg58_ccr8();
	case 3:
		return pwm_ll_get_reg92_ccr2();
	case 4:
		return pwm_ll_get_reg95_ccr5();
	case 5:
		return pwm_ll_get_reg98_ccr8();
	case 6:
		return pwm_ll_get_regd2_ccr2();
	case 7:
		return pwm_ll_get_regd5_ccr5();
	case 8:
		return pwm_ll_get_regd8_ccr8();
	case 9:
		return pwm_ll_get_reg112_ccr2();
	case 10:
		return pwm_ll_get_reg115_ccr5();
	case 11:
		return pwm_ll_get_reg118_ccr8();
	default:
		return 0;
	}

	return 0;
}

uint32_t pwm_hal_get_tim_arr(pwm_chan_t tim_id)
{
	switch (tim_id) {
	case 0:
		return pwm_ll_get_reg4b_tim1_arr();
	case 1:
		return pwm_ll_get_reg4c_tim2_arr();
	case 2:
		return pwm_ll_get_reg4d_tim3_arr();
	case 3:
		return pwm_ll_get_reg8b_tim1_arr();
	case 4:
		return pwm_ll_get_reg8c_tim2_arr();
	case 5:
		return pwm_ll_get_reg8d_tim3_arr();
	case 6:
		return pwm_ll_get_regcb_tim1_arr();
	case 7:
		return pwm_ll_get_regcc_tim2_arr();
	case 8:
		return pwm_ll_get_regcd_tim3_arr();
	case 9:
		return pwm_ll_get_reg10b_tim1_arr();
	case 10:
		return pwm_ll_get_reg10c_tim2_arr();
	case 11:
		return pwm_ll_get_reg10d_tim3_arr();
	default:
		return 0;
	}

	return 0;
}

uint32_t pwm_hal_get_tim_input_level(pwm_chan_t tim_id)
{
	switch (tim_id) {
	case 0:
	case 1:
	case 2:
		return REG_READ(PWM_REG44_ADDR + ((0x40 * 0) << 2));
	case 3:
	case 4:
	case 5:
		return REG_READ(PWM_REG44_ADDR + ((0x40 * 1) << 2));
	case 6:
	case 7:
	case 8:
		return REG_READ(PWM_REG44_ADDR + ((0x40 * 2) << 2));
	default:
		return 0;
	}

	return 0;
}

uint32_t pwm_hal_get_capture_int_type(pwm_chan_t tim_id)
{
	switch (tim_id) {
	case 0:
		return pwm_ll_get_reg44_ch1eif();
	case 1:
		return pwm_ll_get_reg44_ch3eif();
	case 2:
		return pwm_ll_get_reg44_ch5eif();
	case 3:
		return pwm_ll_get_reg84_ch1eif();
	case 4:
		return pwm_ll_get_reg84_ch3eif();
	case 5:
		return pwm_ll_get_reg84_ch5eif();
	case 6:
		return pwm_ll_get_regc4_ch1eif();
	case 7:
		return pwm_ll_get_regc4_ch3eif();
	case 8:
		return pwm_ll_get_regc4_ch5eif();
	case 9:
		return pwm_ll_get_reg104_ch1eif();
	case 10:
		return pwm_ll_get_reg104_ch3eif();
	case 11:
		return pwm_ll_get_reg104_ch5eif();
	default:
		return 0;
	}

	return 0;
}

bk_err_t pwm_hal_set_fade_scale(pwm_chan_t tim_id, uint32_t value)
{
	switch (tim_id) {
	case 0:
		pwm_ll_set_reg6b_tim1_fade_scale(value);
		break;
	case 1:
		pwm_ll_set_reg6c_tim2_fade_scale(value);
		break;
	case 2:
		pwm_ll_set_reg6d_tim3_fade_scale(value);
		break;
	case 3:
		pwm_ll_set_regab_tim1_fade_scale(value);
		break;
	case 4:
		pwm_ll_set_regac_tim2_fade_scale(value);
		break;
	case 5:
		pwm_ll_set_regad_tim3_fade_scale(value);
		break;
	case 6:
		pwm_ll_set_regeb_tim1_fade_scale(value);
		break;
	case 7:
		pwm_ll_set_regec_tim2_fade_scale(value);
		break;
	case 8:
		pwm_ll_set_reged_tim3_fade_scale(value);
		break;
	case 9:
		pwm_ll_set_reg12b_tim1_fade_scale(value);
		break;
	case 10:
		pwm_ll_set_reg12c_tim2_fade_scale(value);
		break;
	case 11:
		pwm_ll_set_reg12d_tim3_fade_scale(value);
		break;
	default:
		break;
	}

	return BK_OK;
}

uint32_t pwm_hal_get_fade_scale(pwm_chan_t tim_id)
{
	switch (tim_id) {
	case 0:
		return pwm_ll_get_reg6b_tim1_fade_scale();
	case 1:
		return pwm_ll_get_reg6c_tim2_fade_scale();
	case 2:
		return pwm_ll_get_reg6d_tim3_fade_scale();
	case 3:
		return pwm_ll_get_regab_tim1_fade_scale();
	case 4:
		return pwm_ll_get_regac_tim2_fade_scale();
	case 5:
		return pwm_ll_get_regad_tim3_fade_scale();
	case 6:
		return pwm_ll_get_regeb_tim1_fade_scale();
	case 7:
		return pwm_ll_get_regec_tim2_fade_scale();
	case 8:
		return pwm_ll_get_reged_tim3_fade_scale();
	case 9:
		return pwm_ll_get_reg12b_tim1_fade_scale();
	case 10:
		return pwm_ll_get_reg12c_tim2_fade_scale();
	case 11:
		return pwm_ll_get_reg12d_tim3_fade_scale();
	default:
		return 0;
	}
}

bk_err_t pwm_hal_set_fade_num(pwm_chan_t tim_id, uint32_t value)
{
	switch (tim_id) {
	case 0:
		pwm_ll_set_reg6b_tim1_fade_num(value);
		break;
	case 1:
		pwm_ll_set_reg6c_tim2_fade_num(value);
		break;
	case 2:
		pwm_ll_set_reg6d_tim3_fade_num(value);
		break;
	case 3:
		pwm_ll_set_regab_tim1_fade_num(value);
		break;
	case 4:
		pwm_ll_set_regac_tim2_fade_num(value);
		break;
	case 5:
		pwm_ll_set_regad_tim3_fade_num(value);
		break;
	case 6:
		pwm_ll_set_regeb_tim1_fade_num(value);
		break;
	case 7:
		pwm_ll_set_regec_tim2_fade_num(value);
		break;
	case 8:
		pwm_ll_set_reged_tim3_fade_num(value);
		break;
	case 9:
		pwm_ll_set_reg12b_tim1_fade_num(value);
		break;
	case 10:
		pwm_ll_set_reg12c_tim2_fade_num(value);
		break;
	case 11:
		pwm_ll_set_reg12d_tim3_fade_num(value);
		break;
	default:
		break;
	}

	return BK_OK;
}

uint32_t pwm_hal_get_fade_num(pwm_chan_t tim_id)
{
	switch (tim_id) {
	case 0:
		return pwm_ll_get_reg6b_tim1_fade_num();
	case 1:
		return pwm_ll_get_reg6c_tim2_fade_num();
	case 2:
		return pwm_ll_get_reg6d_tim3_fade_num();
	case 3:
		return pwm_ll_get_regab_tim1_fade_num();
	case 4:
		return pwm_ll_get_regac_tim2_fade_num();
	case 5:
		return pwm_ll_get_regad_tim3_fade_num();
	case 6:
		return pwm_ll_get_regeb_tim1_fade_num();
	case 7:
		return pwm_ll_get_regec_tim2_fade_num();
	case 8:
		return pwm_ll_get_reged_tim3_fade_num();
	case 9:
		return pwm_ll_get_reg12b_tim1_fade_num();
	case 10:
		return pwm_ll_get_reg12c_tim2_fade_num();
	case 11:
		return pwm_ll_get_reg12d_tim3_fade_num();
	default:
		return 0;
	}
}

bk_err_t pwm_hal_set_fade_intv_cycle(pwm_chan_t tim_id, uint32_t value)
{
	switch (tim_id) {
	case 0:
		pwm_ll_set_reg6b_tim1_fade_intval_cyc(value);
		break;
	case 1:
		pwm_ll_set_reg6c_tim2_fade_intval_cyc(value);
		break;
	case 2:
		pwm_ll_set_reg6d_tim3_fade_intval_cyc(value);
		break;
	case 3:
		pwm_ll_set_regab_tim1_fade_intval_cyc(value);
		break;
	case 4:
		pwm_ll_set_regac_tim2_fade_intval_cyc(value);
		break;
	case 5:
		pwm_ll_set_regad_tim3_fade_intval_cyc(value);
		break;
	case 6:
		pwm_ll_set_regeb_tim1_fade_intval_cyc(value);
		break;
	case 7:
		pwm_ll_set_regec_tim2_fade_intval_cyc(value);
		break;
	case 8:
		pwm_ll_set_reged_tim3_fade_intval_cyc(value);
		break;
	case 9:
		pwm_ll_set_reg12b_tim1_fade_intval_cyc(value);
		break;
	case 10:
		pwm_ll_set_reg12c_tim2_fade_intval_cyc(value);
		break;
	case 11:
		pwm_ll_set_reg12d_tim3_fade_intval_cyc(value);
		break;
	default:
		break;
	}

	return BK_OK;
}

bk_err_t pwm_hal_set_fade_inc_dec(pwm_chan_t tim_id, uint32_t value)
{
	switch (tim_id) {
	case 0:
		pwm_ll_set_reg6b_tim1_fade_inc_dec(value);
		break;
	case 1:
		pwm_ll_set_reg6c_tim2_fade_inc_dec(value);
		break;
	case 2:
		pwm_ll_set_reg6d_tim3_fade_inc_dec(value);
		break;
	case 3:
		pwm_ll_set_regab_tim1_fade_inc_dec(value);
		break;
	case 4:
		pwm_ll_set_regac_tim2_fade_inc_dec(value);
		break;
	case 5:
		pwm_ll_set_regad_tim3_fade_inc_dec(value);
		break;
	case 6:
		pwm_ll_set_regeb_tim1_fade_inc_dec(value);
		break;
	case 7:
		pwm_ll_set_regec_tim2_fade_inc_dec(value);
		break;
	case 8:
		pwm_ll_set_reged_tim3_fade_inc_dec(value);
		break;
	case 9:
		pwm_ll_set_reg12b_tim1_fade_inc_dec(value);
		break;
	case 10:
		pwm_ll_set_reg12c_tim2_fade_inc_dec(value);
		break;
	case 11:
		pwm_ll_set_reg12d_tim3_fade_inc_dec(value);
		break;
	default:
		break;
	}

	return BK_OK;
}

bk_err_t pwm_hal_set_fade_enable(pwm_chan_t tim_id, uint32_t value)
{
	switch (tim_id) {
	case 0:
		pwm_ll_set_reg6b_tim1_duty_fading_en(value);
		break;
	case 1:
		pwm_ll_set_reg6c_tim2_duty_fading_en(value);
		break;
	case 2:
		pwm_ll_set_reg6d_tim3_duty_fading_en(value);
		break;
	case 3:
		pwm_ll_set_regab_tim1_duty_fading_en(value);
		break;
	case 4:
		pwm_ll_set_regac_tim2_duty_fading_en(value);
		break;
	case 5:
		pwm_ll_set_regad_tim3_duty_fading_en(value);
		break;
	case 6:
		pwm_ll_set_regeb_tim1_duty_fading_en(value);
		break;
	case 7:
		pwm_ll_set_regec_tim2_duty_fading_en(value);
		break;
	case 8:
		pwm_ll_set_reged_tim3_duty_fading_en(value);
		break;
	case 9:
		pwm_ll_set_reg12b_tim1_duty_fading_en(value);
		break;
	case 10:
		pwm_ll_set_reg12c_tim2_duty_fading_en(value);
		break;
	case 11:
		pwm_ll_set_reg12d_tim3_duty_fading_en(value);
		break;
	default:
		break;
	}

	return BK_OK;
}

bk_err_t pwm_hal_get_pwm_interrupt_status(uint32_t *int_status, uint32_t group_num)
{
	uint32_t int_status_tmp = 0;

	for (uint32_t index = 0; index < group_num; index++) {
		int_status_tmp = ((REG_READ(PWM_REG44_ADDR + ((0x40 * index) << 2))) & 0xffff);
		int_status[index] = 0;
		int_status[index] = int_status_tmp;
	}
	return BK_OK;
}

bk_err_t pwm_hal_clear_interrupt_status(uint32_t *int_status, uint32_t group_num)
{
	for (uint32_t index = 0; index < group_num; index++) {
		if (int_status[index]) {
			REG_WRITE(PWM_REG44_ADDR + ((0x40 * index) << 2), int_status[index]);
		}
	}

	return BK_OK;
}

bool pwm_hal_is_uif_triggered(pwm_chan_t tim_id, uint32_t *status, uint32_t group_num)
{
	uint32_t int_status_tmp = 0;
	uint32_t uif_int_status = 0;

	for (uint32_t index = 0; index < group_num; index++) {
		int_status_tmp = ((status[index] >> 9) & 0x7);
		uif_int_status |= (int_status_tmp << (3 * index));
	}
	return ((uif_int_status & BIT(tim_id)) ? true : false);
}

bool pwm_hal_is_cc1if_triggered(pwm_chan_t tim_id, uint32_t *status, uint32_t group_num)
{
	uint32_t int_status_tmp = 0;
	uint32_t cc1if_int_status = 0;

	for (uint32_t index = 0; index < group_num; index++) {
		int_status_tmp = status[index] & 0xffff;
		cc1if_int_status |= ((int_status_tmp >> 0) & 0x1) << (0 + 3 * index);
		cc1if_int_status |= ((int_status_tmp >> 3) & 0x1) << (1 + 3 * index);
		cc1if_int_status |= ((int_status_tmp >> 6) & 0x1) << (2 + 3 * index);
	}
	return ((cc1if_int_status & BIT(tim_id)) ? true : false);
}

void pwm_hal_set_sync_all_enable(bool is_enable)
{
	pwm_ll_set_reg4_sync_all_en(is_enable);
}

void pwm_hal_sync_all_shadow(void)
{
	pwm_ll_set_reg4_sync_all(1);
}

#if CONFIG_PWM_PM_CB_SUPPORT
void pwm_hal_pm_backup(uint32_t chan, uint32_t *pm_backup, uint32_t reg_count)
{
	if (reg_count < PWM_PM_BACKUP_REG_NUM) {
		return;
	}

	uint32_t group_id = chan / SOC_PWM_CHAN_NUM_PER_GROUP;

	pm_backup[0] = pwm_ll_get_reg2_value();
	pm_backup[1] = pwm_ll_get_reg4_value();
	pm_backup[2] = REG_READ(SOC_PWM_REG_BASE + ((0x10 + chan) << 2));
	pm_backup[3] = REG_READ(SOC_PWM_REG_BASE + ((0x43 + (group_id * 0x40)) << 2));
	pm_backup[4] = REG_READ(SOC_PWM_REG_BASE + ((0x45 + (group_id * 0x40)) << 2));
	pm_backup[5] = REG_READ(SOC_PWM_REG_BASE + ((0x46 + (group_id * 0x40)) << 2));
	pm_backup[6] = REG_READ(SOC_PWM_REG_BASE + ((0x4a + (group_id * 0x40)) << 2));

	pm_backup[7] = REG_READ(SOC_PWM_REG_BASE + ((0x4b + (group_id * 0x40)) << 2));
	pm_backup[8] = REG_READ(SOC_PWM_REG_BASE + ((0x51 + (group_id * 0x40)) << 2));
	pm_backup[9] = REG_READ(SOC_PWM_REG_BASE + ((0x52 + (group_id * 0x40)) << 2));
	pm_backup[10] = REG_READ(SOC_PWM_REG_BASE + ((0x53 + (group_id * 0x40)) << 2));

	pm_backup[11] = REG_READ(SOC_PWM_REG_BASE + ((0x4c + (group_id * 0x40)) << 2));
	pm_backup[12] = REG_READ(SOC_PWM_REG_BASE + ((0x54 + (group_id * 0x40)) << 2));
	pm_backup[13] = REG_READ(SOC_PWM_REG_BASE + ((0x55 + (group_id * 0x40)) << 2));
	pm_backup[14] = REG_READ(SOC_PWM_REG_BASE + ((0x56 + (group_id * 0x40)) << 2));

	pm_backup[15] = REG_READ(SOC_PWM_REG_BASE + ((0x4d + (group_id * 0x40)) << 2));
	pm_backup[16] = REG_READ(SOC_PWM_REG_BASE + ((0x57 + (group_id * 0x40)) << 2));
	pm_backup[17] = REG_READ(SOC_PWM_REG_BASE + ((0x58 + (group_id * 0x40)) << 2));
	pm_backup[18] = REG_READ(SOC_PWM_REG_BASE + ((0x59 + (group_id * 0x40)) << 2));

	pm_backup[19] = REG_READ(SOC_PWM_REG_BASE + ((0x5a + (group_id * 0x40)) << 2));
}

void pwm_hal_pm_restore(uint32_t chan, uint32_t *pm_backup, uint32_t reg_count)
{
	if (reg_count < PWM_PM_BACKUP_REG_NUM) {
		return;
	}

	uint32_t group_id = chan / SOC_PWM_CHAN_NUM_PER_GROUP;

	pwm_ll_set_reg2_value(pm_backup[0]);
	pwm_ll_set_reg4_value(pm_backup[1]);
	REG_WRITE(SOC_PWM_REG_BASE + ((0x10 + chan) << 2), pm_backup[2]);
	REG_WRITE(SOC_PWM_REG_BASE + ((0x43 + (group_id * 0x40)) << 2), pm_backup[3]);
	REG_WRITE(SOC_PWM_REG_BASE + ((0x45 + (group_id * 0x40)) << 2), pm_backup[4]);
	REG_WRITE(SOC_PWM_REG_BASE + ((0x46 + (group_id * 0x40)) << 2), pm_backup[5]);
	REG_WRITE(SOC_PWM_REG_BASE + ((0x4a + (group_id * 0x40)) << 2), pm_backup[6]);

	REG_WRITE(SOC_PWM_REG_BASE + ((0x4b + (group_id * 0x40)) << 2), pm_backup[7]);
	REG_WRITE(SOC_PWM_REG_BASE + ((0x51 + (group_id * 0x40)) << 2), pm_backup[8]);
	REG_WRITE(SOC_PWM_REG_BASE + ((0x52 + (group_id * 0x40)) << 2), pm_backup[9]);
	REG_WRITE(SOC_PWM_REG_BASE + ((0x53 + (group_id * 0x40)) << 2), pm_backup[10]);

	REG_WRITE(SOC_PWM_REG_BASE + ((0x4c + (group_id * 0x40)) << 2), pm_backup[11]);
	REG_WRITE(SOC_PWM_REG_BASE + ((0x54 + (group_id * 0x40)) << 2), pm_backup[12]);
	REG_WRITE(SOC_PWM_REG_BASE + ((0x55 + (group_id * 0x40)) << 2), pm_backup[13]);
	REG_WRITE(SOC_PWM_REG_BASE + ((0x56 + (group_id * 0x40)) << 2), pm_backup[14]);

	REG_WRITE(SOC_PWM_REG_BASE + ((0x4d + (group_id * 0x40)) << 2), pm_backup[15]);
	REG_WRITE(SOC_PWM_REG_BASE + ((0x57 + (group_id * 0x40)) << 2), pm_backup[16]);
	REG_WRITE(SOC_PWM_REG_BASE + ((0x58 + (group_id * 0x40)) << 2), pm_backup[17]);
	REG_WRITE(SOC_PWM_REG_BASE + ((0x59 + (group_id * 0x40)) << 2), pm_backup[18]);

	REG_WRITE(SOC_PWM_REG_BASE + ((0x5a + (group_id * 0x40)) << 2), pm_backup[19]);
}
#endif
