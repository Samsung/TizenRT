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
#include "sys_ana_hw.h"
#include "sys_ana_hal.h"

typedef void (*sys_ana_dump_fn_t)(void);
typedef struct {
	uint32_t start;
	uint32_t end;
	sys_ana_dump_fn_t fn;
} sys_ana_reg_fn_map_t;

static void sys_ana_dump_ana_reg0(void)
{
	sys_ana_ana_reg0_t *r = (sys_ana_ana_reg0_t *)(SOC_SYS_ANA_REG_BASE + (0x40 << 2));

	SOC_LOGI("ana_reg0: %8x\r\n", REG_READ(SOC_SYS_ANA_REG_BASE + (0x40 << 2)));
	SOC_LOGI("	vctrl_testen: %8x\r\n", r->vctrl_testen);
	SOC_LOGI("	vctrlsel: %8x\r\n", r->vctrlsel);
	SOC_LOGI("	cp_ioff: %8x\r\n", r->cp_ioff);
	SOC_LOGI("	spisel_unlockl: %8x\r\n", r->spisel_unlockl);
	SOC_LOGI("	spisel_unlockh: %8x\r\n", r->spisel_unlockh);
	SOC_LOGI("	cp: %8x\r\n", r->cp);
	SOC_LOGI("	spideten: %8x\r\n", r->spideten);
	SOC_LOGI("	cben: %8x\r\n", r->cben);
	SOC_LOGI("	hvref: %8x\r\n", r->hvref);
	SOC_LOGI("	lvref: %8x\r\n", r->lvref);
	SOC_LOGI("	rzctrl26m: %8x\r\n", r->rzctrl26m);
	SOC_LOGI("	lpf_rz: %8x\r\n", r->lpf_rz);
	SOC_LOGI("	rpc: %8x\r\n", r->rpc);
	SOC_LOGI("	dpll_tsten: %8x\r\n", r->dpll_tsten);
	SOC_LOGI("	divsel: %8x\r\n", r->divsel);
	SOC_LOGI("	kctrl: %8x\r\n", r->kctrl);
	SOC_LOGI("	vsel_ldo: %8x\r\n", r->vsel_ldo);
	SOC_LOGI("	bp_caldone: %8x\r\n", r->bp_caldone);
	SOC_LOGI("	spi_rst_unlock: %8x\r\n", r->spi_rst_unlock);
}

static void sys_ana_dump_ana_reg1(void)
{
	sys_ana_ana_reg1_t *r = (sys_ana_ana_reg1_t *)(SOC_SYS_ANA_REG_BASE + (0x41 << 2));

	SOC_LOGI("ana_reg1: %8x\r\n", REG_READ(SOC_SYS_ANA_REG_BASE + (0x41 << 2)));
	SOC_LOGI("	nc_0_0: %8x\r\n", r->nc_0_0);
	SOC_LOGI("	bandmanual: %8x\r\n", r->bandmanual);
	SOC_LOGI("	manual_ictrl: %8x\r\n", r->manual_ictrl);
	SOC_LOGI("	enclk: %8x\r\n", r->enclk);
	SOC_LOGI("	closeloop_en: %8x\r\n", r->closeloop_en);
	SOC_LOGI("	nc_13_13: %8x\r\n", r->nc_13_13);
	SOC_LOGI("	spi_rstn: %8x\r\n", r->spi_rstn);
	SOC_LOGI("	osccal_trig: %8x\r\n", r->osccal_trig);
	SOC_LOGI("	manual: %8x\r\n", r->manual);
	SOC_LOGI("	diffl: %8x\r\n", r->diffl);
	SOC_LOGI("	ictrl: %8x\r\n", r->ictrl);
	SOC_LOGI("	cnti: %8x\r\n", r->cnti);
}

static void sys_ana_dump_ana_reg2(void)
{
	sys_ana_ana_reg2_t *r = (sys_ana_ana_reg2_t *)(SOC_SYS_ANA_REG_BASE + (0x42 << 2));

	SOC_LOGI("ana_reg2: %8x\r\n", REG_READ(SOC_SYS_ANA_REG_BASE + (0x42 << 2)));
	SOC_LOGI("	xtalh_ctune: %8x\r\n", r->xtalh_ctune);
	SOC_LOGI("	gadc_inbufsel: %8x\r\n", r->gadc_inbufsel);
	SOC_LOGI("	bufictrl: %8x\r\n", r->bufictrl);
	SOC_LOGI("	gadc_compisel: %8x\r\n", r->gadc_compisel);
	SOC_LOGI("	gadc_bscalsaw: %8x\r\n", r->gadc_bscalsaw);
	SOC_LOGI("	gadc_vncalsaw: %8x\r\n", r->gadc_vncalsaw);
	SOC_LOGI("	gadc_vpcalsaw: %8x\r\n", r->gadc_vpcalsaw);
	SOC_LOGI("	gadc_vbg_sel: %8x\r\n", r->gadc_vbg_sel);
	SOC_LOGI("	gadc_clk_rlten: %8x\r\n", r->gadc_clk_rlten);
	SOC_LOGI("	gadc_calintsaw_en: %8x\r\n", r->gadc_calintsaw_en);
	SOC_LOGI("	gadc_clk_sel: %8x\r\n", r->gadc_clk_sel);
	SOC_LOGI("	gadc_clk_inv: %8x\r\n", r->gadc_clk_inv);
	SOC_LOGI("	gadc_calcap_ch: %8x\r\n", r->gadc_calcap_ch);
	SOC_LOGI("	inbufen: %8x\r\n", r->inbufen);
	SOC_LOGI("	sar_enspi: %8x\r\n", r->sar_enspi);
}

static void sys_ana_dump_ana_reg3(void)
{
	sys_ana_ana_reg3_t *r = (sys_ana_ana_reg3_t *)(SOC_SYS_ANA_REG_BASE + (0x43 << 2));

	SOC_LOGI("ana_reg3: %8x\r\n", REG_READ(SOC_SYS_ANA_REG_BASE + (0x43 << 2)));
	SOC_LOGI("	reserved_bit_0_0: %8x\r\n", r->reserved_bit_0_0);
	SOC_LOGI("	preamp_isel: %8x\r\n", r->preamp_isel);
	SOC_LOGI("	refbuff_isel: %8x\r\n", r->refbuff_isel);
	SOC_LOGI("	vref_sel: %8x\r\n", r->vref_sel);
	SOC_LOGI("	offset_en: %8x\r\n", r->offset_en);
	SOC_LOGI("	hpssren: %8x\r\n", r->hpssren);
	SOC_LOGI("	ck_sel: %8x\r\n", r->ck_sel);
	SOC_LOGI("	lppllvddsel: %8x\r\n", r->lppllvddsel);
	SOC_LOGI("	pwd_xtalldo_spi: %8x\r\n", r->pwd_xtalldo_spi);
	SOC_LOGI("	iamp: %8x\r\n", r->iamp);
	SOC_LOGI("	vddren: %8x\r\n", r->vddren);
	SOC_LOGI("	xamp: %8x\r\n", r->xamp);
	SOC_LOGI("	vosel: %8x\r\n", r->vosel);
	SOC_LOGI("	en_xtalh_sleep: %8x\r\n", r->en_xtalh_sleep);
	SOC_LOGI("	digbufb_hpen: %8x\r\n", r->digbufb_hpen);
	SOC_LOGI("	bufictrl: %8x\r\n", r->bufictrl);
	SOC_LOGI("	ibias_ctrl: %8x\r\n", r->ibias_ctrl);
	SOC_LOGI("	icore_ctrl: %8x\r\n", r->icore_ctrl);
}

static void sys_ana_dump_ana_reg4(void)
{
	sys_ana_ana_reg4_t *r = (sys_ana_ana_reg4_t *)(SOC_SYS_ANA_REG_BASE + (0x44 << 2));

	SOC_LOGI("ana_reg4: %8x\r\n", REG_READ(SOC_SYS_ANA_REG_BASE + (0x44 << 2)));
	SOC_LOGI("	temp_gsel: %8x\r\n", r->temp_gsel);
	SOC_LOGI("	vbg_0v9sel: %8x\r\n", r->vbg_0v9sel);
	SOC_LOGI("	nc_2_15: %8x\r\n", r->nc_2_15);
	SOC_LOGI("	ck2xsel: %8x\r\n", r->ck2xsel);
	SOC_LOGI("	nc_17_21: %8x\r\n", r->nc_17_21);
	SOC_LOGI("	dpll_vctrl_tsten: %8x\r\n", r->dpll_vctrl_tsten);
	SOC_LOGI("	spilatchb_rc32k: %8x\r\n", r->spilatchb_rc32k);
	SOC_LOGI("	rc32k_refclk_en: %8x\r\n", r->rc32k_refclk_en);
	SOC_LOGI("	en_vddflashtest: %8x\r\n", r->en_vddflashtest);
	SOC_LOGI("	vddgpio_sel: %8x\r\n", r->vddgpio_sel);
	SOC_LOGI("	ck_tst_en: %8x\r\n", r->ck_tst_en);
	SOC_LOGI("	vpsramsel: %8x\r\n", r->vpsramsel);
	SOC_LOGI("	swb: %8x\r\n", r->swb);
	SOC_LOGI("	en_psramldo: %8x\r\n", r->en_psramldo);
}

static void sys_ana_dump_ana_reg5(void)
{
	sys_ana_ana_reg5_t *r = (sys_ana_ana_reg5_t *)(SOC_SYS_ANA_REG_BASE + (0x45 << 2));

	SOC_LOGI("ana_reg5: %8x\r\n", REG_READ(SOC_SYS_ANA_REG_BASE + (0x45 << 2)));
	SOC_LOGI("	pwd_rosc_spi: %8x\r\n", r->pwd_rosc_spi);
	SOC_LOGI("	en_xtall: %8x\r\n", r->en_xtall);
	SOC_LOGI("	reserved_bit_2_2: %8x\r\n", r->reserved_bit_2_2);
	SOC_LOGI("	cktst_sel: %8x\r\n", r->cktst_sel);
	SOC_LOGI("	en_temp: %8x\r\n", r->en_temp);
	SOC_LOGI("	en_dpll: %8x\r\n", r->en_dpll);
	SOC_LOGI("	en_cb: %8x\r\n", r->en_cb);
	SOC_LOGI("	pmu_test: %8x\r\n", r->pmu_test);
	SOC_LOGI("	anabufsel_rx0: %8x\r\n", r->anabufsel_rx0);
	SOC_LOGI("	anabufsel_tx0: %8x\r\n", r->anabufsel_tx0);
	SOC_LOGI("	anabufsel_rx1: %8x\r\n", r->anabufsel_rx1);
	SOC_LOGI("	anabufsel_tx1: %8x\r\n", r->anabufsel_tx1);
	SOC_LOGI("	rosc_disable: %8x\r\n", r->rosc_disable);
	SOC_LOGI("	ldo2xvsel: %8x\r\n", r->ldo2xvsel);
	SOC_LOGI("	itune_xtall: %8x\r\n", r->itune_xtall);
	SOC_LOGI("	xtall_ten: %8x\r\n", r->xtall_ten);
	SOC_LOGI("	rosc_tsten: %8x\r\n", r->rosc_tsten);
	SOC_LOGI("	bcal_start: %8x\r\n", r->bcal_start);
	SOC_LOGI("	bcal_en: %8x\r\n", r->bcal_en);
	SOC_LOGI("	bcal_sel: %8x\r\n", r->bcal_sel);
	SOC_LOGI("	vbias: %8x\r\n", r->vbias);
}

static void sys_ana_dump_ana_reg6(void)
{
	sys_ana_ana_reg6_t *r = (sys_ana_ana_reg6_t *)(SOC_SYS_ANA_REG_BASE + (0x46 << 2));

	SOC_LOGI("ana_reg6: %8x\r\n", REG_READ(SOC_SYS_ANA_REG_BASE + (0x46 << 2)));
	SOC_LOGI("	calib_interval: %8x\r\n", r->calib_interval);
	SOC_LOGI("	modify_interval: %8x\r\n", r->modify_interval);
	SOC_LOGI("	xtal_wakeup_time: %8x\r\n", r->xtal_wakeup_time);
	SOC_LOGI("	spi_trig: %8x\r\n", r->spi_trig);
	SOC_LOGI("	modifi_auto: %8x\r\n", r->modifi_auto);
	SOC_LOGI("	calib_auto: %8x\r\n", r->calib_auto);
	SOC_LOGI("	cal_mode: %8x\r\n", r->cal_mode);
	SOC_LOGI("	manu_ena: %8x\r\n", r->manu_ena);
	SOC_LOGI("	manu_cin: %8x\r\n", r->manu_cin);
}

static void sys_ana_dump_ana_reg7(void)
{
	sys_ana_ana_reg7_t *r = (sys_ana_ana_reg7_t *)(SOC_SYS_ANA_REG_BASE + (0x47 << 2));

	SOC_LOGI("ana_reg7: %8x\r\n", REG_READ(SOC_SYS_ANA_REG_BASE + (0x47 << 2)));
	SOC_LOGI("	clk_sel: %8x\r\n", r->clk_sel);
	SOC_LOGI("	timer_wkrstn: %8x\r\n", r->timer_wkrstn);
	SOC_LOGI("	dldohp: %8x\r\n", r->dldohp);
	SOC_LOGI("	vporsel: %8x\r\n", r->vporsel);
	SOC_LOGI("	vbspbuf_lp: %8x\r\n", r->vbspbuf_lp);
	SOC_LOGI("	iocurlim: %8x\r\n", r->iocurlim);
	SOC_LOGI("	vanaldosel: %8x\r\n", r->vanaldosel);
	SOC_LOGI("	aldohp: %8x\r\n", r->aldohp);
	SOC_LOGI("	anacurlim: %8x\r\n", r->anacurlim);
	SOC_LOGI("	envrefh1v: %8x\r\n", r->envrefh1v);
	SOC_LOGI("	violdosel: %8x\r\n", r->violdosel);
	SOC_LOGI("	vbatdetsel: %8x\r\n", r->vbatdetsel);
	SOC_LOGI("	en_compe: %8x\r\n", r->en_compe);
	SOC_LOGI("	spi_pwd_regpow: %8x\r\n", r->spi_pwd_regpow);
	SOC_LOGI("	bypassen: %8x\r\n", r->bypassen);
	SOC_LOGI("	ioldolp: %8x\r\n", r->ioldolp);
	SOC_LOGI("	bgcal: %8x\r\n", r->bgcal);
	SOC_LOGI("	vbgcalmode: %8x\r\n", r->vbgcalmode);
	SOC_LOGI("	vbgcalstart: %8x\r\n", r->vbgcalstart);
	SOC_LOGI("	pwd_bgcal: %8x\r\n", r->pwd_bgcal);
	SOC_LOGI("	spi_envbg: %8x\r\n", r->spi_envbg);
}

static void sys_ana_dump_ana_reg8(void)
{
	sys_ana_ana_reg8_t *r = (sys_ana_ana_reg8_t *)(SOC_SYS_ANA_REG_BASE + (0x48 << 2));

	SOC_LOGI("ana_reg8: %8x\r\n", REG_READ(SOC_SYS_ANA_REG_BASE + (0x48 << 2)));
	SOC_LOGI("	asoft_stc: %8x\r\n", r->asoft_stc);
	SOC_LOGI("	rst_timerwks1v: %8x\r\n", r->rst_timerwks1v);
	SOC_LOGI("	dldo_czsel: %8x\r\n", r->dldo_czsel);
	SOC_LOGI("	digcurlim: %8x\r\n", r->digcurlim);
	SOC_LOGI("	spi_latch1v: %8x\r\n", r->spi_latch1v);
	SOC_LOGI("	dldo_rzsel: %8x\r\n", r->dldo_rzsel);
	SOC_LOGI("	lvsleep_wkrstn: %8x\r\n", r->lvsleep_wkrstn);
	SOC_LOGI("	gpiowk_rstn: %8x\r\n", r->gpiowk_rstn);
	SOC_LOGI("	rtcwk_rstn: %8x\r\n", r->rtcwk_rstn);
	SOC_LOGI("	ensfsdd: %8x\r\n", r->ensfsdd);
	SOC_LOGI("	vcorehsel: %8x\r\n", r->vcorehsel);
	SOC_LOGI("	vcorelsel: %8x\r\n", r->vcorelsel);
	SOC_LOGI("	vlden: %8x\r\n", r->vlden);
	SOC_LOGI("	rst_gpiowks: %8x\r\n", r->rst_gpiowks);
	SOC_LOGI("	aldo_rzsel: %8x\r\n", r->aldo_rzsel);
	SOC_LOGI("	aldo_czsel: %8x\r\n", r->aldo_czsel);
	SOC_LOGI("	vtrxspisel: %8x\r\n", r->vtrxspisel);
}

static void sys_ana_dump_ana_reg9(void)
{
	sys_ana_ana_reg9_t *r = (sys_ana_ana_reg9_t *)(SOC_SYS_ANA_REG_BASE + (0x49 << 2));

	SOC_LOGI("ana_reg9: %8x\r\n", REG_READ(SOC_SYS_ANA_REG_BASE + (0x49 << 2)));
	SOC_LOGI("	azcd_manu: %8x\r\n", r->azcd_manu);
	SOC_LOGI("	zcdswvs: %8x\r\n", r->zcdswvs);
	SOC_LOGI("	zcdsrefsel: %8x\r\n", r->zcdsrefsel);
	SOC_LOGI("	enzcdcalib: %8x\r\n", r->enzcdcalib);
	SOC_LOGI("	enzcddy: %8x\r\n", r->enzcddy);
	SOC_LOGI("	zcdmsel: %8x\r\n", r->zcdmsel);
	SOC_LOGI("	spi_timerwken: %8x\r\n", r->spi_timerwken);
	SOC_LOGI("	spi_byp32pwd: %8x\r\n", r->spi_byp32pwd);
	SOC_LOGI("	sd: %8x\r\n", r->sd);
	SOC_LOGI("	iburstsel: %8x\r\n", r->iburstsel);
	SOC_LOGI("	ckfs: %8x\r\n", r->ckfs);
	SOC_LOGI("	ckintsel: %8x\r\n", r->ckintsel);
	SOC_LOGI("	osccaltrig: %8x\r\n", r->osccaltrig);
	SOC_LOGI("	mroscsel: %8x\r\n", r->mroscsel);
	SOC_LOGI("	mrosci_cal: %8x\r\n", r->mrosci_cal);
	SOC_LOGI("	mrosccap_cal: %8x\r\n", r->mrosccap_cal);
}

static void sys_ana_dump_ana_reg10(void)
{
	sys_ana_ana_reg10_t *r = (sys_ana_ana_reg10_t *)(SOC_SYS_ANA_REG_BASE + (0x4a << 2));

	SOC_LOGI("ana_reg10: %8x\r\n", REG_READ(SOC_SYS_ANA_REG_BASE + (0x4a << 2)));
	SOC_LOGI("	sfsr: %8x\r\n", r->sfsr);
	SOC_LOGI("	ensfsaa: %8x\r\n", r->ensfsaa);
	SOC_LOGI("	apfms: %8x\r\n", r->apfms);
	SOC_LOGI("	atmpo_sel: %8x\r\n", r->atmpo_sel);
	SOC_LOGI("	ampoen: %8x\r\n", r->ampoen);
	SOC_LOGI("	vbg_rstrtc_en: %8x\r\n", r->vbg_rstrtc_en);
	SOC_LOGI("	avea_sel: %8x\r\n", r->avea_sel);
	SOC_LOGI("	aforcepfm: %8x\r\n", r->aforcepfm);
	SOC_LOGI("	acls: %8x\r\n", r->acls);
	SOC_LOGI("	aswrsten: %8x\r\n", r->aswrsten);
	SOC_LOGI("	aripc: %8x\r\n", r->aripc);
	SOC_LOGI("	arampc: %8x\r\n", r->arampc);
	SOC_LOGI("	arampcen: %8x\r\n", r->arampcen);
	SOC_LOGI("	aenburst: %8x\r\n", r->aenburst);
	SOC_LOGI("	apfmen: %8x\r\n", r->apfmen);
	SOC_LOGI("	aldosel: %8x\r\n", r->aldosel);
}

static void sys_ana_dump_ana_reg11(void)
{
	sys_ana_ana_reg11_t *r = (sys_ana_ana_reg11_t *)(SOC_SYS_ANA_REG_BASE + (0x4b << 2));

	SOC_LOGI("ana_reg11: %8x\r\n", REG_READ(SOC_SYS_ANA_REG_BASE + (0x4b << 2)));
	SOC_LOGI("	gpiowk: %8x\r\n", r->gpiowk);
	SOC_LOGI("	rtcsel: %8x\r\n", r->rtcsel);
	SOC_LOGI("	timersel: %8x\r\n", r->timersel);
}

static void sys_ana_dump_ana_reg12(void)
{
	SOC_LOGI("ana_reg12: %8x\r\n", REG_READ(SOC_SYS_ANA_REG_BASE + (0x4c << 2)));
}

static void sys_ana_dump_ana_reg13(void)
{
	SOC_LOGI("ana_reg13: %8x\r\n", REG_READ(SOC_SYS_ANA_REG_BASE + (0x4d << 2)));
}

static sys_ana_reg_fn_map_t s_fn[] =
{
	{0x40, 0x40, sys_ana_dump_ana_reg0},
	{0x41, 0x41, sys_ana_dump_ana_reg1},
	{0x42, 0x42, sys_ana_dump_ana_reg2},
	{0x43, 0x43, sys_ana_dump_ana_reg3},
	{0x44, 0x44, sys_ana_dump_ana_reg4},
	{0x45, 0x45, sys_ana_dump_ana_reg5},
	{0x46, 0x46, sys_ana_dump_ana_reg6},
	{0x47, 0x47, sys_ana_dump_ana_reg7},
	{0x48, 0x48, sys_ana_dump_ana_reg8},
	{0x49, 0x49, sys_ana_dump_ana_reg9},
	{0x4a, 0x4a, sys_ana_dump_ana_reg10},
	{0x4b, 0x4b, sys_ana_dump_ana_reg11},
	{0x4c, 0x4c, sys_ana_dump_ana_reg12},
	{0x4d, 0x4d, sys_ana_dump_ana_reg13},
	{-1, -1, 0}
};

void sys_ana_struct_dump(uint32_t start, uint32_t end)
{
	uint32_t dump_fn_cnt = sizeof(s_fn)/sizeof(s_fn[0]) - 1;

	for (uint32_t idx = 0; idx < dump_fn_cnt; idx++) {
		if ((start <= s_fn[idx].start) && (end >= s_fn[idx].end)) {
			s_fn[idx].fn();
		}
	}
}
