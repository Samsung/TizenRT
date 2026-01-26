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

#include <string.h>
#include <common/bk_include.h>
#include <os/mem.h>
#include "adc_hal.h"
#include <driver/adc_types.h>
#include "power_driver.h"
#include "clock_driver.h"
#include "bk_sys_ctrl.h"

uint32_t adc_hal_get_temp_code_dft_25degree(void)
{
    return 8505;
}

uint32_t adc_hal_get_temp_code_dft_threshold(void)
{
    return 800;
}

uint32_t adc_hal_get_temp_lsb_per_10degree(void)
{
    return 580;
}

uint32_t adc_hal_get_1Volt_value(void)
{
    return 0x1194;
}

uint32_t adc_hal_get_1Volt_threshold(void)
{
    return 270;
}

uint32_t adc_hal_get_2Volt_value(void)
{
    return 0x23ea;
}

uint32_t adc_hal_get_2Volt_threshold(void)
{
    return 540;
}

#define F2ISR(f, s) ((int32_t)((f > 0) ? ((f) * (1 << (s)) + 0.5) : ((f) * (1 << (s)) - 0.5)))

#define ADC_BITS              (16)
#define SCA_deltad            (24)
#define SCA_cwt               (24)
#define SCA_out               (16)
#define SCA_diff              (SCA_deltad - SCA_cwt)
#define Bit_mu_ito            (5)
#define Bit_mu_d              (5)
#define CWT1                  F2ISR(0.46f, SCA_cwt)
#define SARADC_CALIB_DEBUG    0

typedef struct {
	int32_t deltad;
	int8_t boat1[ADC_BITS];
	int8_t boat2[ADC_BITS];
	int8_t diff[ADC_BITS];
	int32_t cwt[ADC_BITS];
} adc_cali_context_t;

const int32_t cwt_init_val[] = {
	554, 1107, 2215, 4430, 8860, 16398, 30315, 56587, 106320, 194919, 354399, 655638, 1222676, 2250433, 4164187, 7708177
};

static adc_cali_context_t s_adc_cali_context = {0};

static void adc_init_lms(adc_cali_context_t *st)
{
	memset(st, 0, sizeof(adc_cali_context_t));
	st->deltad = F2ISR(0.001f, SCA_deltad);
	memcpy(st->cwt, cwt_init_val, sizeof(int32_t) * ADC_BITS);
}

static void adc_cali_lms(adc_cali_context_t *st, uint16_t *adc_data_p, uint16_t data_cnt, uint16_t loop)
{
	uint8_t *p = NULL;
	uint8_t tp1, tp2, tp3, tp4, m;
	uint16_t cnt;
	int32_t qx1, qx2;
	int32_t deltaq, temp;

	while (loop--) {
		cnt = data_cnt >> 1;
		p = (uint8_t*)adc_data_p;
		while (cnt--) {
			tp1 = *p++;
			tp2 = *p++;
			tp3 = *p++;
			tp4 = *p++;

			qx1 = 0;
			qx2 = 0;
			for (m = 0; m < 8; m++) {
				st->boat1[m] = (tp1 & 1);
				st->boat2[m] = (tp3 & 1);
				st->boat1[m + 8] = (tp2 & 1);
				st->boat2[m + 8] = (tp4 & 1);

				qx1 += st->boat1[m + 8] * st->cwt[m + 8];
				qx2 += st->boat2[m + 8] * st->cwt[m + 8];
				qx1 += st->boat1[m] * st->cwt[m];
				qx2 += st->boat2[m] * st->cwt[m];

				st->diff[m] = st->boat1[m] - st->boat2[m];
				st->diff[m + 8] = st->boat1[m + 8] - st->boat2[m + 8];

				tp1 >>= 1;
				tp2 >>= 1;
				tp3 >>= 1;
				tp4 >>= 1;
			}
			deltaq = (qx1 - qx2) << SCA_diff;
			deltaq -= st->deltad << 1;
			st->deltad += (deltaq >> Bit_mu_d);

			temp = (deltaq >> (Bit_mu_ito + SCA_diff));
			for (m = 0; m < (ADC_BITS - 1); m++) {
				if (st->diff[m] == 1) {
					st->cwt[m] -= temp;
				} else if (st->diff[m] == -1) {
					st->cwt[m] += temp;
				}
			}
			st->cwt[ADC_BITS - 1] = CWT1;
		}
	}
	///////////////////////////////// normalization
	temp = 0;
	for (m = 0; m < ADC_BITS; m++) {
		temp += st->cwt[m];
	}
	temp = ((int32_t)(1 << 30)) / (temp >> 4);  // sca=10
	for (m = 0; m < ADC_BITS; m++) {
		st->cwt[m] = ((int32_t)(st->cwt[m] >> 5) * temp) >> (5 + (SCA_cwt - SCA_out));   // 24-5+10-5-8=16;
	}
   ////////////////////////////////// check data
	if ((st->cwt[0] < 0) || (st->cwt[1] < 0) || (st->cwt[2] < 0)) {
		st->cwt[2] = st->cwt[3] >> 1;
		st->cwt[1] = st->cwt[2] >> 1;
		st->cwt[0] = st->cwt[1] >> 1;
	}
}

void adc_hal_set_cwt_calib(uint16_t *adc_data, uint16_t data_num)
{
	adc_init_lms(&s_adc_cali_context); // initial
	adc_cali_lms(&s_adc_cali_context, adc_data, data_num, 2); // adc lms calibration process

#if SARADC_CALIB_DEBUG
	for (int k = 0; k < 16; k++) {
		BK_LOG_RAW("%9.6f  %d\n", (double)s_adc_cali_context.cwt[k] / (1 << SCA_out), s_adc_cali_context.cwt[k]);
	}
#endif

	adc_hal_set_cwt((uint32_t *)s_adc_cali_context.cwt);
}

void adc_hal_set_cwt(uint32_t *adc_data)
{
	if (adc_data == NULL) {
		return;
	}
	for (int k = 0; k < ADC_CWT_COEF_NUM; k++) {
		s_adc_cali_context.cwt[k] = adc_data[k];
	}

	adc_ll_set_reg7_cwt_cal00(s_adc_cali_context.cwt[0]);
	adc_ll_set_reg8_cwt_cal01(s_adc_cali_context.cwt[1]);
	adc_ll_set_reg9_cwt_cal02(s_adc_cali_context.cwt[2]);
	adc_ll_set_rega_cwt_cal03(s_adc_cali_context.cwt[3]);
	adc_ll_set_regb_cwt_cal04(s_adc_cali_context.cwt[4]);
	adc_ll_set_regc_cwt_cal05(s_adc_cali_context.cwt[5]);
	adc_ll_set_regd_cwt_cal06(s_adc_cali_context.cwt[6]);
	adc_ll_set_rege_cwt_cal07(s_adc_cali_context.cwt[7]);
	adc_ll_set_regf_cwt_cal08(s_adc_cali_context.cwt[8]);
	adc_ll_set_reg10_cwt_cal09(s_adc_cali_context.cwt[9]);
	adc_ll_set_reg11_cwt_cal0a(s_adc_cali_context.cwt[10]);
	adc_ll_set_reg12_cwt_cal0b(s_adc_cali_context.cwt[11]);
	adc_ll_set_reg13_cwt_cal0c(s_adc_cali_context.cwt[12]);
	adc_ll_set_reg14_cwt_cal0d(s_adc_cali_context.cwt[13]);
	adc_ll_set_reg15_cwt_cal0e(s_adc_cali_context.cwt[14]);
	adc_ll_set_reg16_cwt_cal0f(s_adc_cali_context.cwt[15]);

	adc_ll_set_reg5_co_gain(0x0);
	adc_ll_set_reg6_co_offset(0x0);
	adc_ll_set_reg4_adc_sraw(0);
	adc_ll_set_reg4_calib_mode(0);
	adc_ll_set_reg4_calib_done(1);
}

__IRAM_SEC void hal_calib_apply(void)
{
	adc_ll_set_reg2_soft_rst(0);
	adc_ll_set_reg2_soft_rst(1);
	adc_ll_set_reg4_value(0x12112);
	adc_ll_set_reg5_co_gain(0x0);
	adc_ll_set_reg6_co_offset(0x0);

	adc_ll_set_reg7_cwt_cal00(s_adc_cali_context.cwt[0]);
	adc_ll_set_reg8_cwt_cal01(s_adc_cali_context.cwt[1]);
	adc_ll_set_reg9_cwt_cal02(s_adc_cali_context.cwt[2]);
	adc_ll_set_rega_cwt_cal03(s_adc_cali_context.cwt[3]);
	adc_ll_set_regb_cwt_cal04(s_adc_cali_context.cwt[4]);
	adc_ll_set_regc_cwt_cal05(s_adc_cali_context.cwt[5]);
	adc_ll_set_regd_cwt_cal06(s_adc_cali_context.cwt[6]);
	adc_ll_set_rege_cwt_cal07(s_adc_cali_context.cwt[7]);
	adc_ll_set_regf_cwt_cal08(s_adc_cali_context.cwt[8]);
	adc_ll_set_reg10_cwt_cal09(s_adc_cali_context.cwt[9]);
	adc_ll_set_reg11_cwt_cal0a(s_adc_cali_context.cwt[10]);
	adc_ll_set_reg12_cwt_cal0b(s_adc_cali_context.cwt[11]);
	adc_ll_set_reg13_cwt_cal0c(s_adc_cali_context.cwt[12]);
	adc_ll_set_reg14_cwt_cal0d(s_adc_cali_context.cwt[13]);
	adc_ll_set_reg15_cwt_cal0e(s_adc_cali_context.cwt[14]);
	adc_ll_set_reg16_cwt_cal0f(s_adc_cali_context.cwt[15]);
	adc_ll_set_reg17_adc_int_en(0x0);
	adc_ll_set_reg17_adc_int_level(0x10);
	adc_ll_set_reg17_adc_int_en(1);

	adc_ll_set_reg4_calib_mode(0);
	adc_ll_set_reg4_calib_done(1);
	adc_ll_set_reg4_samp_sel(0);
	adc_ll_set_reg4_adc_mode(1);
	adc_ll_set_reg4_adc_dump_num(5);
	adc_ll_set_reg4_adc_sraw(0);
	adc_ll_set_reg5_co_gain(0xff);
	adc_ll_set_reg6_co_offset(0x1);

}

void adc_hal_calib_init(void)
{
	/* ana_reg0x2 */
	uint32_t reg_val = sys_ana_ll_get_reg2_value();
	reg_val |= (BIT(25) | BIT(30) | BIT(31));
	reg_val |= (0x3 << 28);
	sys_ana_ll_set_reg2_value(reg_val);

	/* ana_reg0x4 */
	//sys_ana_ll_set_reg4_value(0x9FC9A7F0);

	/* ana_reg0x5 */
	reg_val = sys_ana_ll_get_reg5_value();
	reg_val |= (BIT(12) | BIT(14));
	reg_val &= ~BIT(1);
	sys_ana_ll_set_reg5_value(reg_val);

	adc_ll_set_reg2_soft_rst(0);
	for(uint32_t i = 0; i < 10000; i++);
	adc_ll_set_reg2_soft_rst(1);
	adc_ll_set_reg4_value(0x12112);
	adc_ll_set_reg5_co_gain(0x0);
	adc_ll_set_reg6_co_offset(0x0);
	adc_ll_set_reg7_cwt_cal00(0x0);
	adc_ll_set_reg8_cwt_cal01(0x0);
	adc_ll_set_reg9_cwt_cal02(0x0);
	adc_ll_set_rega_cwt_cal03(0x0);
	adc_ll_set_regb_cwt_cal04(0x0);
	adc_ll_set_regc_cwt_cal05(0x0);
	adc_ll_set_regd_cwt_cal06(0x0);
	adc_ll_set_rege_cwt_cal07(0x0);
	adc_ll_set_regf_cwt_cal08(0x0);
	adc_ll_set_reg10_cwt_cal09(0x0);
	adc_ll_set_reg11_cwt_cal0a(0x0);
	adc_ll_set_reg12_cwt_cal0b(0x0);
	adc_ll_set_reg13_cwt_cal0c(0x0);
	adc_ll_set_reg14_cwt_cal0d(0x0);
	adc_ll_set_reg15_cwt_cal0e(0x0);
	adc_ll_set_reg16_cwt_cal0f(0x0);
	adc_ll_set_reg17_adc_int_en(0x0);
	adc_ll_set_reg17_adc_int_level(0x10);
}

bk_err_t adc_hal_init(adc_hal_t *hal)
{
	adc_ll_set_reg2_soft_rst(0);
	for(uint32_t i = 0; i < 10000; i++);
	adc_ll_set_reg2_soft_rst(1);

	adc_ll_set_reg17_adc_int_en(1);

	adc_ll_set_reg4_calib_mode(0);
	adc_ll_set_reg4_calib_done(1);
	adc_ll_set_reg4_samp_sel(0);
	adc_ll_set_reg4_adc_mode(1);
	adc_ll_set_reg4_adc_dump_num(5);
	adc_ll_set_reg4_adc_sraw(0);
	adc_ll_set_reg5_co_gain(0xff);
	adc_ll_set_reg6_co_offset(0x1);

	return BK_OK;
}

bk_err_t adc_hal_deinit(adc_hal_t *hal)
{
	adc_ll_set_reg4_value(0);

	return BK_OK;
}

bk_err_t adc_hal_set_clk(adc_hal_t *hal, adc_src_clk_t src_clk, uint32_t adc_clk)
{
	uint32_t clk_src = 0;
	uint32_t pre_div = 0;

	if (src_clk == ADC_SCLK_XTAL) {
		sys_hal_set_cksel_sadc(1);
		clk_src = SOC_ADC_XTAL_CLK;
	} else if (src_clk == ADC_SCLK_DPLL) {
		return BK_ERR_ADC_DPLL_NOT_SUPPORTED;
	}

	if (clk_src && (adc_clk > (clk_src / 2))) {
		pre_div = 0;
	} else if (clk_src && adc_clk) {
		/* adc_clk = clk_src / (adc_sclk_div * 2) */
		pre_div = clk_src / (2 * adc_clk);
	}

	adc_ll_set_reg4_adc_sclk_div(pre_div);

	return BK_OK;
}

#if SARADC_AUTOTEST
bk_err_t adc_hal_set_div(adc_hal_t *hal, uint32_t div)
{
	uint32_t pre_div = div;
	sys_hal_set_cksel_sadc(0);
	adc_ll_set_reg4_adc_sclk_div(hal->hw, pre_div);
	return BK_OK;
}
#endif

bk_err_t adc_hal_set_mode(adc_hal_t *hal, adc_mode_t adc_mode)
{
	if (adc_mode == ADC_CONTINUOUS_MODE) {
		adc_ll_set_reg4_adc_mode(1);
	} else if (adc_mode == ADC_SINGLE_STEP_MODE) {
		adc_ll_set_reg4_adc_mode(0);
	}

	return BK_OK;
}

bk_err_t adc_hal_set_saturate_mode(adc_hal_t *hal, adc_saturate_mode_t mode)
{
	return BK_OK;
}

bk_err_t adc_hal_start_commom(adc_hal_t *hal)
{
	adc_ll_set_reg4_enable(1);

	return BK_OK;
}

bk_err_t adc_hal_stop_commom(adc_hal_t *hal)
{
	adc_ll_set_reg4_enable(0);

	return BK_OK;
}

uint16_t adc_hal_get_single_step_adc_data(adc_hal_t *hal)
{
	return adc_ll_get_reg18_fifo_rdata();
}

#if defined(CONFIG_SARADC_V1P2)
bk_err_t adc_hal_set_vol_div(adc_chan_t adc_chan, adc_vol_div_t vol_div)
{
	return BK_OK;
}

adc_vol_div_t adc_hal_get_vol_div(adc_chan_t adc_chan)
{
	return ADC_VOL_DIV_1;
}
#endif

void adc_hal_clear_int_status(adc_hal_t *hal)
{
	adc_ll_set_reg19_int_state(1);
}

bool adc_hal_is_fifo_empty(adc_hal_t *hal)
{
	return !!(adc_ll_get_reg19_fifo_empty());
}

uint32_t adc_hal_get_adc_data(adc_hal_t *hal)
{
	return adc_ll_get_reg18_fifo_rdata();
}

void adc_hal_sel_channel(adc_hal_t *hal, adc_chan_t chan)
{
	adc_ll_set_reg4_adc_chan(chan);
}

bool adc_hal_check_adc_enable(adc_hal_t *hal)
{
	return !!(adc_ll_get_reg4_enable());
}

void adc_hal_set_fifo_threshold(adc_hal_t *hal, uint32_t cnt)
{
	adc_ll_set_reg17_adc_int_level(cnt);
}

uint32_t adc_hal_get_mode(adc_hal_t *hal)
{
	uint32_t adc_mode = adc_ll_get_reg4_adc_mode();

	if (adc_mode == 1) {
		return ADC_CONTINUOUS_MODE;
	} else if (adc_mode == 0) {
		return ADC_SINGLE_STEP_MODE;
	} else {
		return ADC_CONTINUOUS_MODE;
	}
}
