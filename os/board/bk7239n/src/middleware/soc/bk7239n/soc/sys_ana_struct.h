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

#ifdef __cplusplus
extern "C" {
#endif


typedef volatile union {
	struct {
		uint32_t vctrl_testen             :  1; /**<bit[0 : 0]  */
		uint32_t vctrlsel                 :  2; /**<bit[1 : 2]  */
		uint32_t cp_ioff                  :  2; /**<bit[3 : 4]  */
		uint32_t spisel_unlockl           :  1; /**<bit[5 : 5]  */
		uint32_t spisel_unlockh           :  1; /**<bit[6 : 6]  */
		uint32_t cp                       :  3; /**<bit[7 : 9] */
		uint32_t spideten                 :  1; /**<bit[10 : 10] */
		uint32_t cben                     :  1; /**<bit[11 : 11] */
		uint32_t hvref                    :  2; /**<bit[12 : 13] */
		uint32_t lvref                    :  2; /**<bit[14 : 15] */
		uint32_t rzctrl26m                :  1; /**<bit[16 : 16] */
		uint32_t lpf_rz                   :  4; /**<bit[17 : 20] */
		uint32_t rpc                      :  2; /**<bit[21 : 22] */
		uint32_t dpll_tsten               :  1; /**<bit[23 : 23] */
		uint32_t divsel                   :  2; /**<bit[24 : 25] */
		uint32_t kctrl                    :  2; /**<bit[26 : 27] */
		uint32_t vsel_ldo                 :  2; /**<bit[28 : 29] */
		uint32_t bp_caldone               :  1; /**<bit[30 : 30] */
		uint32_t spi_rst_unlock           :  1; /**<bit[31 : 31]  */
	};
	uint32_t v;
} sys_ana_ana_reg0_t;


typedef volatile union {
	struct {
		uint32_t nc_0_0                   :  1; /**<bit[0 : 0] */
		uint32_t bandmanual               :  7; /**<bit[1 : 7]  */
		uint32_t mode                     :  2; /**<bit[8 : 9] */
		uint32_t manual_ictrl             :  1; /**<bit[10 : 10] */
		uint32_t enclk                    :  1; /**<bit[11 : 11] */
		uint32_t closeloop_en             :  1; /**<bit[12 : 12] */
		uint32_t nc_13_13                 :  1; /**<bit[13 : 13] */
		uint32_t spi_rstn                 :  1; /**<bit[14 : 14] */
		uint32_t osccal_trig              :  1; /**<bit[15 : 15] */
		uint32_t manual                   :  1; /**<bit[16 : 16] */
		uint32_t diffl                    :  3; /**<bit[17 : 19] */
		uint32_t ictrl                    :  3; /**<bit[20 : 22] */
		uint32_t cnti                     :  9; /**<bit[23 : 31] */
	};
	uint32_t v;
} sys_ana_ana_reg1_t;


typedef volatile union {
	struct {
		uint32_t xtalh_ctune              :  8; /**<bit[0 : 7] */
		uint32_t gadc_inbufsel            :  2; /**<bit[8 : 9] */
		uint32_t bufictrl                 :  1; /**<bit[10 : 10]  */
		uint32_t gadc_compisel            :  3; /**<bit[11 : 13] */
		uint32_t gadc_bscalsaw            :  3; /**<bit[14 : 16] */
		uint32_t gadc_vncalsaw            :  3; /**<bit[17 : 19] */
		uint32_t gadc_vpcalsaw            :  3; /**<bit[20 : 22] */
		uint32_t gadc_vbg_sel             :  1; /**<bit[23 : 23] */
		uint32_t gadc_clk_rlten           :  1; /**<bit[24 : 24] */
		uint32_t gadc_calintsaw_en        :  1; /**<bit[25 : 25] */
		uint32_t gadc_clk_sel             :  1; /**<bit[26 : 26] */
		uint32_t gadc_clk_inv             :  1; /**<bit[27 : 27] */
		uint32_t gadc_calcap_ch           :  2; /**<bit[28 : 29] */
		uint32_t inbufen                  :  1; /**<bit[30 : 30] */
		uint32_t sar_enspi                :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} sys_ana_ana_reg2_t;


typedef volatile union {
	struct {
		uint32_t reserved_bit_0_0         :  1; /**<bit[0 : 0] */
		uint32_t preamp_isel              :  2; /**<bit[1 : 2] */
		uint32_t refbuff_isel             :  3; /**<bit[3 : 5] */
		uint32_t vref_sel                 :  1; /**<bit[6 : 6] */
		uint32_t offset_en                :  1; /**<bit[7 : 7] */
		uint32_t hpssren                  :  1; /**<bit[8 : 8] */
		uint32_t ck_sel                   :  1; /**<bit[9 : 9] */
		uint32_t lppllvddsel              :  1; /**<bit[10 : 10]  */
		uint32_t pwd_xtalldo_spi          :  1; /**<bit[11 : 11]  */
		uint32_t iamp                     :  1; /**<bit[12 : 12] */
		uint32_t vddren                   :  1; /**<bit[13 : 13] */
		uint32_t xamp                     :  6; /**<bit[14 : 19] */
		uint32_t vosel                    :  5; /**<bit[20 : 24] */
		uint32_t en_xtalh_sleep           :  1; /**<bit[25 : 25] */
		uint32_t digbufb_hpen             :  1; /**<bit[26 : 26] */
		uint32_t bufictrl                 :  1; /**<bit[27 : 27] */
		uint32_t ibias_ctrl               :  2; /**<bit[28 : 29] */
		uint32_t icore_ctrl               :  2; /**<bit[30 : 31] */
	};
	uint32_t v;
} sys_ana_ana_reg3_t;


typedef volatile union {
	struct {
		uint32_t temp_gsel                :  1; /**<bit[0 : 0]  */
		uint32_t vbg_0v9sel               :  1; /**<bit[1 : 1]  */
		uint32_t nc_2_15                  : 14; /**<bit[2 : 15]  */
		uint32_t ck2xsel                  :  1; /**<bit[16 : 16]  */
		uint32_t nc_17_21                 :  5; /**<bit[17 : 21]  */
		uint32_t dpll_vctrl_tsten         :  1; /**<bit[22 : 22]  */
		uint32_t spilatchb_rc32k          :  1; /**<bit[23 : 23]  */
		uint32_t rc32k_refclk_en          :  1; /**<bit[24 : 24]  */
		uint32_t en_vddflashtest          :  1; /**<bit[25 : 25] */
		uint32_t vddgpio_sel              :  1; /**<bit[26 : 26] */
		uint32_t ck_tst_en                :  1; /**<bit[27 : 27]  */
		uint32_t vpsramsel                :  2; /**<bit[28 : 29] */
		uint32_t swb                      :  1; /**<bit[30 : 30] */
		uint32_t en_psramldo              :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} sys_ana_ana_reg4_t;


typedef volatile union {
	struct {
		uint32_t pwd_rosc_spi             :  1; /**<bit[0 : 0]  */
		uint32_t en_xtall                 :  1; /**<bit[1 : 1] */
		uint32_t cktst_sel                :  2; /**<bit[2 : 3]  */
		uint32_t en_temp                  :  1; /**<bit[4 : 4] */
		uint32_t en_dpll                  :  1; /**<bit[5 : 5] */
		uint32_t en_cb                    :  1; /**<bit[6 : 6] */
		uint32_t pmu_test                 :  1; /**<bit[7 : 7] */
		uint32_t anabufsel_rx0            :  1; /**<bit[8 : 8]  */
		uint32_t anabufsel_tx0            :  1; /**<bit[9 : 9]  */
		uint32_t anabufsel_rx1            :  1; /**<bit[10 : 10]  */
		uint32_t anabufsel_tx1            :  1; /**<bit[11 : 11]  */
		uint32_t rosc_disable             :  1; /**<bit[12 : 12] */
		uint32_t ldo2xvsel                :  3; /**<bit[13 : 15]  */
		uint32_t itune_xtall              :  4; /**<bit[16 : 19] */
		uint32_t xtall_ten                :  1; /**<bit[20 : 20] */
		uint32_t rosc_tsten               :  1; /**<bit[21 : 21] */
		uint32_t bcal_start               :  1; /**<bit[22 : 22] */
		uint32_t bcal_en                  :  1; /**<bit[23 : 23] */
		uint32_t bcal_sel                 :  3; /**<bit[24 : 26] */
		uint32_t vbias                    :  5; /**<bit[27 : 31] */
	};
	uint32_t v;
} sys_ana_ana_reg5_t;


typedef volatile union {
	struct {
		uint32_t calib_interval           : 10; /**<bit[0 : 9] */
		uint32_t modify_interval          :  6; /**<bit[10 : 15] */
		uint32_t xtal_wakeup_time         :  4; /**<bit[16 : 19] */
		uint32_t spi_trig                 :  1; /**<bit[20 : 20] */
		uint32_t modifi_auto              :  1; /**<bit[21 : 21] */
		uint32_t calib_auto               :  1; /**<bit[22 : 22] */
		uint32_t cal_mode                 :  1; /**<bit[23 : 23] */
		uint32_t manu_ena                 :  1; /**<bit[24 : 24] */
		uint32_t manu_cin                 :  7; /**<bit[25 : 31] */
	};
	uint32_t v;
} sys_ana_ana_reg6_t;


typedef volatile union {
	struct {
		uint32_t clk_sel                  :  1; /**<bit[0 : 0] */
		uint32_t timer_wkrstn             :  1; /**<bit[1 : 1] */
		uint32_t dldohp                   :  1; /**<bit[2 : 2] */
		uint32_t vporsel                  :  1; /**<bit[3 : 3] */
		uint32_t vbspbuf_lp               :  1; /**<bit[4 : 4] */
		uint32_t iocurlim                 :  1; /**<bit[5 : 5] */
		uint32_t vanaldosel               :  4; /**<bit[6 : 9] */
		uint32_t aldohp                   :  1; /**<bit[10 : 10] */
		uint32_t anacurlim                :  1; /**<bit[11 : 11] */
		uint32_t envrefh1v                :  1; /**<bit[12 : 12] */
		uint32_t violdosel                :  3; /**<bit[13 : 15] */
		uint32_t vbatdetsel               :  2; /**<bit[16 : 17] */
		uint32_t en_compe                 :  1; /**<bit[18 : 18]  */
		uint32_t spi_pwd_regpow           :  1; /**<bit[19 : 19] */
		uint32_t bypassen                 :  1; /**<bit[20 : 20] */
		uint32_t ioldolp                  :  1; /**<bit[21 : 21] */
		uint32_t bgcal                    :  6; /**<bit[22 : 27] */
		uint32_t vbgcalmode               :  1; /**<bit[28 : 28] */
		uint32_t vbgcalstart              :  1; /**<bit[29 : 29] */
		uint32_t pwd_bgcal                :  1; /**<bit[30 : 30] */
		uint32_t spi_envbg                :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} sys_ana_ana_reg7_t;


typedef volatile union {
	struct {
		uint32_t asoft_stc                :  4; /**<bit[0 : 3] */
		uint32_t rst_timerwks1v           :  1; /**<bit[4 : 4] */
		uint32_t dldo_czsel               :  3; /**<bit[5 : 7] */
		uint32_t digcurlim                :  1; /**<bit[8 : 8] */
		uint32_t spi_latch1v              :  1; /**<bit[9 : 9] */
		uint32_t dldo_rzsel               :  2; /**<bit[10 : 11] */
		uint32_t lvsleep_wkrstn           :  1; /**<bit[12 : 12] */
		uint32_t gpiowk_rstn              :  1; /**<bit[13 : 13] */
		uint32_t rtcwk_rstn               :  1; /**<bit[14 : 14] */
		uint32_t ensfsdd                  :  1; /**<bit[15 : 15] */
		uint32_t vcorehsel                :  4; /**<bit[16 : 19] */
		uint32_t vcorelsel                :  3; /**<bit[20 : 22] */
		uint32_t vlden                    :  1; /**<bit[23 : 23] */
		uint32_t rst_gpiowks              :  1; /**<bit[24 : 24]  */
		uint32_t aldo_rzsel               :  2; /**<bit[25 : 26] */
		uint32_t aldo_czsel               :  3; /**<bit[27 : 29] */
		uint32_t vtrxspisel               :  2; /**<bit[30 : 31] */
	};
	uint32_t v;
} sys_ana_ana_reg8_t;


typedef volatile union {
	struct {
		uint32_t azcd_manu                :  6; /**<bit[0 : 5] */
		uint32_t zcdswvs                  :  3; /**<bit[6 : 8] */
		uint32_t zcdsrefsel               :  3; /**<bit[9 : 11] */
		uint32_t enzcdcalib               :  1; /**<bit[12 : 12] */
		uint32_t enzcddy                  :  1; /**<bit[13 : 13] */
		uint32_t zcdmsel                  :  1; /**<bit[14 : 14] */
		uint32_t spi_timerwken            :  1; /**<bit[15 : 15] */
		uint32_t spi_byp32pwd             :  1; /**<bit[16 : 16] */
		uint32_t sd                       :  1; /**<bit[17 : 17] */
		uint32_t iburstsel                :  2; /**<bit[18 : 19] */
		uint32_t ckfs                     :  2; /**<bit[20 : 21] */
		uint32_t ckintsel                 :  1; /**<bit[22 : 22] */
		uint32_t osccaltrig               :  1; /**<bit[23 : 23] */
		uint32_t mroscsel                 :  1; /**<bit[24 : 24] */
		uint32_t mrosci_cal               :  3; /**<bit[25 : 27] */
		uint32_t mrosccap_cal             :  4; /**<bit[28 : 31] */
	};
	uint32_t v;
} sys_ana_ana_reg9_t;


typedef volatile union {
	struct {
		uint32_t sfsr                     :  4; /**<bit[0 : 3] */
		uint32_t ensfsaa                  :  1; /**<bit[4 : 4] */
		uint32_t apfms                    :  5; /**<bit[5 : 9] */
		uint32_t atmpo_sel                :  2; /**<bit[10 : 11] */
		uint32_t ampoen                   :  1; /**<bit[12 : 12] */
		uint32_t vbg_rstrtc_en            :  1; /**<bit[13 : 13]  */
		uint32_t avea_sel                 :  2; /**<bit[14 : 15] */
		uint32_t aforcepfm                :  1; /**<bit[16 : 16] */
		uint32_t acls                     :  3; /**<bit[17 : 19] */
		uint32_t aswrsten                 :  1; /**<bit[20 : 20] */
		uint32_t aripc                    :  3; /**<bit[21 : 23] */
		uint32_t arampc                   :  4; /**<bit[24 : 27] */
		uint32_t arampcen                 :  1; /**<bit[28 : 28] */
		uint32_t aenburst                 :  1; /**<bit[29 : 29] */
		uint32_t apfmen                   :  1; /**<bit[30 : 30] */
		uint32_t aldosel                  :  1; /**<bit[31 : 31] */
	};
	uint32_t v;
} sys_ana_ana_reg10_t;


typedef volatile union {
	struct {
		uint32_t gpiowk                   : 24; /**<bit[0 : 23] */
		uint32_t rtcsel                   :  4; /**<bit[24 : 27] */
		uint32_t timersel                 :  4; /**<bit[28 : 31] */
	};
	uint32_t v;
} sys_ana_ana_reg11_t;


typedef volatile union {
	struct {
		uint32_t timersel                 : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} sys_ana_ana_reg12_t;


typedef volatile union {
	struct {
		uint32_t rtcsel                   : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} sys_ana_ana_reg13_t;

typedef volatile struct {
	volatile sys_ana_ana_reg0_t ana_reg0;
	volatile sys_ana_ana_reg1_t ana_reg1;
	volatile sys_ana_ana_reg2_t ana_reg2;
	volatile sys_ana_ana_reg3_t ana_reg3;
	volatile sys_ana_ana_reg4_t ana_reg4;
	volatile sys_ana_ana_reg5_t ana_reg5;
	volatile sys_ana_ana_reg6_t ana_reg6;
	volatile sys_ana_ana_reg7_t ana_reg7;
	volatile sys_ana_ana_reg8_t ana_reg8;
	volatile sys_ana_ana_reg9_t ana_reg9;
	volatile sys_ana_ana_reg10_t ana_reg10;
	volatile sys_ana_ana_reg11_t ana_reg11;
	volatile sys_ana_ana_reg12_t ana_reg12;
	volatile sys_ana_ana_reg13_t ana_reg13;
} sys_ana_hw_t;

#ifdef __cplusplus
}
#endif
