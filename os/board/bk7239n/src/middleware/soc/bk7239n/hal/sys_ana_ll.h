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
#include "sys_ana_hw.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SOC_SYS_ANA_REG_BASE SOC_SYS_REG_BASE
#define SYS_ANA_LL_REG_BASE   SOC_SYS_ANA_REG_BASE

//This way of setting ana_reg_bit value is only for sys_ctrl, other driver please implement by yourself!!
#define GET_SYS_ANALOG_REG_IDX(addr) ((addr - SYS_ANA_REG0_ADDR) >> 2)

#if defined(CONFIG_ANA_REG_WRITE_POLL_REG_B)
#define SYS_ANALOG_REG_SPI_STATE_REG (SYS_CPU_ANASPI_FREQ_ADDR)
#define SYS_ANALOG_REG_SPI_STATE_POS(idx) (idx + 8)
#else
#define SYS_ANALOG_REG_SPI_STATE_REG (SOC_SYS_REG_BASE + (0x3a << 2))
#define SYS_ANALOG_REG_SPI_STATE_POS(idx) (idx)
#endif

/* Flash version for early boot (in FLASH) */
__FLASH_BOOT_CODE static inline void sys_ana_set_ana_reg_bit_flash(uint32_t reg_addr, uint32_t pos, uint32_t mask, uint32_t value)
{
	uint32_t reg_value;
	reg_value = *(volatile uint32_t *)(reg_addr);
	reg_value &= ~(mask << pos);
	reg_value |= ((value & mask) <<pos);
	sys_ll_set_analog_reg_value_flash(reg_addr, reg_value);
}

/* Runtime version (in PSRAM) - removed __FLASH_BOOT_CODE */
static inline void sys_ana_set_ana_reg_bit(uint32_t reg_addr, uint32_t pos, uint32_t mask, uint32_t value)
{
	uint32_t reg_value;
	reg_value = *(volatile uint32_t *)(reg_addr);
	reg_value &= ~(mask << pos);
	reg_value |= ((value & mask) <<pos);
	sys_ll_set_analog_reg_value(reg_addr, reg_value);
}

__IRAM_SEC static inline void sys_ana_set_ana_reg_bit_iram(uint32_t reg_addr, uint32_t pos, uint32_t mask, uint32_t value)
{
	uint32_t reg_value;
	reg_value = *(volatile uint32_t *)(reg_addr);
	reg_value &= ~(mask << pos);
	reg_value |= ((value & mask) <<pos);
	sys_ll_set_analog_reg_value_iram(reg_addr, reg_value);
}

//reg ana_reg0:
static inline void sys_ana_ll_set_ana_reg0_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_ANA_REG_BASE + (0x40 << 2)), v);
}

static inline uint32_t sys_ana_ll_get_ana_reg0_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_ANA_REG_BASE + (0x40 << 2));
}

static inline void sys_ana_ll_set_ana_reg0_vctrl_testen(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x40 << 2)), 0, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg0_vctrl_testen(void) {
	sys_ana_ana_reg0_t *r = (sys_ana_ana_reg0_t*)(SOC_SYS_ANA_REG_BASE + (0x40 << 2));
	return r->vctrl_testen;
}

static inline void sys_ana_ll_set_ana_reg0_vctrlsel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x40 << 2)), 1, 0x3, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg0_vctrlsel(void) {
	sys_ana_ana_reg0_t *r = (sys_ana_ana_reg0_t*)(SOC_SYS_ANA_REG_BASE + (0x40 << 2));
	return r->vctrlsel;
}

static inline void sys_ana_ll_set_ana_reg0_cp_ioff(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x40 << 2)), 3, 0x3, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg0_cp_ioff(void) {
	sys_ana_ana_reg0_t *r = (sys_ana_ana_reg0_t*)(SOC_SYS_ANA_REG_BASE + (0x40 << 2));
	return r->cp_ioff;
}

static inline void sys_ana_ll_set_ana_reg0_spisel_unlockl(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x40 << 2)), 5, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg0_spisel_unlockl(void) {
	sys_ana_ana_reg0_t *r = (sys_ana_ana_reg0_t*)(SOC_SYS_ANA_REG_BASE + (0x40 << 2));
	return r->spisel_unlockl;
}

static inline void sys_ana_ll_set_ana_reg0_spisel_unlockh(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x40 << 2)), 6, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg0_spisel_unlockh(void) {
	sys_ana_ana_reg0_t *r = (sys_ana_ana_reg0_t*)(SOC_SYS_ANA_REG_BASE + (0x40 << 2));
	return r->spisel_unlockh;
}

static inline void sys_ana_ll_set_ana_reg0_cp(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x40 << 2)), 7, 0x7, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg0_cp(void) {
	sys_ana_ana_reg0_t *r = (sys_ana_ana_reg0_t*)(SOC_SYS_ANA_REG_BASE + (0x40 << 2));
	return r->cp;
}

static inline void sys_ana_ll_set_ana_reg0_spideten(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x40 << 2)), 10, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg0_spideten(void) {
	sys_ana_ana_reg0_t *r = (sys_ana_ana_reg0_t*)(SOC_SYS_ANA_REG_BASE + (0x40 << 2));
	return r->spideten;
}

__FLASH_BOOT_CODE static inline void sys_ana_ll_set_ana_reg0_cben_flash(uint32_t v) {
	sys_ana_set_ana_reg_bit_flash((SOC_SYS_ANA_REG_BASE + (0x40 << 2)), 11, 0x1, v);
}

static inline void sys_ana_ll_set_ana_reg0_cben(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x40 << 2)), 11, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg0_cben(void) {
	sys_ana_ana_reg0_t *r = (sys_ana_ana_reg0_t*)(SOC_SYS_ANA_REG_BASE + (0x40 << 2));
	return r->cben;
}

static inline void sys_ana_ll_set_ana_reg0_hvref(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x40 << 2)), 12, 0x3, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg0_hvref(void) {
	sys_ana_ana_reg0_t *r = (sys_ana_ana_reg0_t*)(SOC_SYS_ANA_REG_BASE + (0x40 << 2));
	return r->hvref;
}

static inline void sys_ana_ll_set_ana_reg0_lvref(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x40 << 2)), 14, 0x3, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg0_lvref(void) {
	sys_ana_ana_reg0_t *r = (sys_ana_ana_reg0_t*)(SOC_SYS_ANA_REG_BASE + (0x40 << 2));
	return r->lvref;
}

static inline void sys_ana_ll_set_ana_reg0_rzctrl26m(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x40 << 2)), 16, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg0_rzctrl26m(void) {
	sys_ana_ana_reg0_t *r = (sys_ana_ana_reg0_t*)(SOC_SYS_ANA_REG_BASE + (0x40 << 2));
	return r->rzctrl26m;
}

static inline void sys_ana_ll_set_ana_reg0_lpf_rz(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x40 << 2)), 17, 0xf, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg0_lpf_rz(void) {
	sys_ana_ana_reg0_t *r = (sys_ana_ana_reg0_t*)(SOC_SYS_ANA_REG_BASE + (0x40 << 2));
	return r->lpf_rz;
}

static inline void sys_ana_ll_set_ana_reg0_rpc(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x40 << 2)), 21, 0x3, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg0_rpc(void) {
	sys_ana_ana_reg0_t *r = (sys_ana_ana_reg0_t*)(SOC_SYS_ANA_REG_BASE + (0x40 << 2));
	return r->rpc;
}

static inline void sys_ana_ll_set_ana_reg0_dpll_tsten(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x40 << 2)), 23, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg0_dpll_tsten(void) {
	sys_ana_ana_reg0_t *r = (sys_ana_ana_reg0_t*)(SOC_SYS_ANA_REG_BASE + (0x40 << 2));
	return r->dpll_tsten;
}

static inline void sys_ana_ll_set_ana_reg0_divsel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x40 << 2)), 24, 0x3, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg0_divsel(void) {
	sys_ana_ana_reg0_t *r = (sys_ana_ana_reg0_t*)(SOC_SYS_ANA_REG_BASE + (0x40 << 2));
	return r->divsel;
}

static inline void sys_ana_ll_set_ana_reg0_kctrl(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x40 << 2)), 26, 0x3, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg0_kctrl(void) {
	sys_ana_ana_reg0_t *r = (sys_ana_ana_reg0_t*)(SOC_SYS_ANA_REG_BASE + (0x40 << 2));
	return r->kctrl;
}

static inline void sys_ana_ll_set_ana_reg0_vsel_ldo(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x40 << 2)), 28, 0x3, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg0_vsel_ldo(void) {
	sys_ana_ana_reg0_t *r = (sys_ana_ana_reg0_t*)(SOC_SYS_ANA_REG_BASE + (0x40 << 2));
	return r->vsel_ldo;
}

static inline void sys_ana_ll_set_ana_reg0_bp_caldone(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x40 << 2)), 30, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg0_bp_caldone(void) {
	sys_ana_ana_reg0_t *r = (sys_ana_ana_reg0_t*)(SOC_SYS_ANA_REG_BASE + (0x40 << 2));
	return r->bp_caldone;
}

static inline void sys_ana_ll_set_ana_reg0_spi_rst_unlock(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x40 << 2)), 31, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg0_spi_rst_unlock(void) {
	sys_ana_ana_reg0_t *r = (sys_ana_ana_reg0_t*)(SOC_SYS_ANA_REG_BASE + (0x40 << 2));
	return r->spi_rst_unlock;
}

//reg ana_reg1:

static inline void sys_ana_ll_set_ana_reg1_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_ANA_REG_BASE + (0x41 << 2)), v);
}

static inline uint32_t sys_ana_ll_get_ana_reg1_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_ANA_REG_BASE + (0x41 << 2));
}

static inline void sys_ana_ll_set_ana_reg1_nc_0_0(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x41 << 2)), 0, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg1_nc_0_0(void) {
	sys_ana_ana_reg1_t *r = (sys_ana_ana_reg1_t*)(SOC_SYS_ANA_REG_BASE + (0x41 << 2));
	return r->nc_0_0;
}

__FLASH_BOOT_CODE static inline void sys_ana_ll_set_ana_reg1_bandmanual_flash(uint32_t v) {
	sys_ana_set_ana_reg_bit_flash((SOC_SYS_ANA_REG_BASE + (0x41 << 2)), 1, 0x7f, v);
}

static inline void sys_ana_ll_set_ana_reg1_bandmanual(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x41 << 2)), 1, 0x7f, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg1_bandmanual(void) {
	sys_ana_ana_reg1_t *r = (sys_ana_ana_reg1_t*)(SOC_SYS_ANA_REG_BASE + (0x41 << 2));
	return r->bandmanual;
}

static inline void sys_ana_ll_set_ana_reg1_mode(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 8, 0x3, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg1_mode(void) {
	sys_ana_ana_reg1_t *r = (sys_ana_ana_reg1_t*)(SOC_SYS_REG_BASE + (0x41 << 2));
	return r->mode;
}

static inline void sys_ana_ll_set_ana_reg1_manual_ictrl(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x41 << 2)), 10, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg1_manual_ictrl(void) {
	sys_ana_ana_reg1_t *r = (sys_ana_ana_reg1_t*)(SOC_SYS_ANA_REG_BASE + (0x41 << 2));
	return r->manual_ictrl;
}

static inline void sys_ana_ll_set_ana_reg1_enclk(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x41 << 2)), 11, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg1_enclk(void) {
	sys_ana_ana_reg1_t *r = (sys_ana_ana_reg1_t*)(SOC_SYS_ANA_REG_BASE + (0x41 << 2));
	return r->enclk;
}

static inline void sys_ana_ll_set_ana_reg1_closeloop_en(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x41 << 2)), 12, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg1_closeloop_en(void) {
	sys_ana_ana_reg1_t *r = (sys_ana_ana_reg1_t*)(SOC_SYS_ANA_REG_BASE + (0x41 << 2));
	return r->closeloop_en;
}

static inline void sys_ana_ll_set_ana_reg1_nc_13_13(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x41 << 2)), 13, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg1_nc_13_13(void) {
	sys_ana_ana_reg1_t *r = (sys_ana_ana_reg1_t*)(SOC_SYS_ANA_REG_BASE + (0x41 << 2));
	return r->nc_13_13;
}

static inline void sys_ana_ll_set_ana_reg1_spi_rstn(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x41 << 2)), 14, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg1_spi_rstn(void) {
	sys_ana_ana_reg1_t *r = (sys_ana_ana_reg1_t*)(SOC_SYS_ANA_REG_BASE + (0x41 << 2));
	return r->spi_rstn;
}

__FLASH_BOOT_CODE static inline void sys_ana_ll_set_ana_reg1_osccal_trig_flash(uint32_t v) {
	sys_ana_set_ana_reg_bit_flash((SOC_SYS_ANA_REG_BASE + (0x41 << 2)), 15, 0x1, v);
}

static inline void sys_ana_ll_set_ana_reg1_osccal_trig(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x41 << 2)), 15, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg1_osccal_trig(void) {
	sys_ana_ana_reg1_t *r = (sys_ana_ana_reg1_t*)(SOC_SYS_ANA_REG_BASE + (0x41 << 2));
	return r->osccal_trig;
}

__FLASH_BOOT_CODE static inline void sys_ana_ll_set_ana_reg1_manual_flash(uint32_t v) {
	sys_ana_set_ana_reg_bit_flash((SOC_SYS_ANA_REG_BASE + (0x41 << 2)), 16, 0x1, v);
}

static inline void sys_ana_ll_set_ana_reg1_manual(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x41 << 2)), 16, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg1_manual(void) {
	sys_ana_ana_reg1_t *r = (sys_ana_ana_reg1_t*)(SOC_SYS_ANA_REG_BASE + (0x41 << 2));
	return r->manual;
}

static inline void sys_ana_ll_set_ana_reg1_diffl(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x41 << 2)), 17, 0x7, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg1_diffl(void) {
	sys_ana_ana_reg1_t *r = (sys_ana_ana_reg1_t*)(SOC_SYS_ANA_REG_BASE + (0x41 << 2));
	return r->diffl;
}

static inline void sys_ana_ll_set_ana_reg1_ictrl(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x41 << 2)), 20, 0x7, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg1_ictrl(void) {
	sys_ana_ana_reg1_t *r = (sys_ana_ana_reg1_t*)(SOC_SYS_ANA_REG_BASE + (0x41 << 2));
	return r->ictrl;
}

static inline void sys_ana_ll_set_ana_reg1_cnti(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x41 << 2)), 23, 0x1ff, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg1_cnti(void) {
	sys_ana_ana_reg1_t *r = (sys_ana_ana_reg1_t*)(SOC_SYS_ANA_REG_BASE + (0x41 << 2));
	return r->cnti;
}

//reg ana_reg2:

static inline void sys_ana_ll_set_ana_reg2_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_ANA_REG_BASE + (0x42 << 2)), v);
}

static inline uint32_t sys_ana_ll_get_ana_reg2_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_ANA_REG_BASE + (0x42 << 2));
}

static inline void sys_ana_ll_set_ana_reg2_xtalh_ctune(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x42 << 2)), 0, 0xff, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg2_xtalh_ctune(void) {
	sys_ana_ana_reg2_t *r = (sys_ana_ana_reg2_t*)(SOC_SYS_ANA_REG_BASE + (0x42 << 2));
	return r->xtalh_ctune;
}

static inline void sys_ana_ll_set_ana_reg2_gadc_inbufsel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x42 << 2)), 8, 0x3, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg2_gadc_inbufsel(void) {
	sys_ana_ana_reg2_t *r = (sys_ana_ana_reg2_t*)(SOC_SYS_ANA_REG_BASE + (0x42 << 2));
	return r->gadc_inbufsel;
}

static inline void sys_ana_ll_set_ana_reg2_bufictrl(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x42 << 2)), 10, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg2_bufictrl(void) {
	sys_ana_ana_reg2_t *r = (sys_ana_ana_reg2_t*)(SOC_SYS_ANA_REG_BASE + (0x42 << 2));
	return r->bufictrl;
}

static inline void sys_ana_ll_set_ana_reg2_gadc_compisel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x42 << 2)), 11, 0x7, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg2_gadc_compisel(void) {
	sys_ana_ana_reg2_t *r = (sys_ana_ana_reg2_t*)(SOC_SYS_ANA_REG_BASE + (0x42 << 2));
	return r->gadc_compisel;
}

static inline void sys_ana_ll_set_ana_reg2_gadc_bscalsaw(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x42 << 2)), 14, 0x7, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg2_gadc_bscalsaw(void) {
	sys_ana_ana_reg2_t *r = (sys_ana_ana_reg2_t*)(SOC_SYS_ANA_REG_BASE + (0x42 << 2));
	return r->gadc_bscalsaw;
}

static inline void sys_ana_ll_set_ana_reg2_gadc_vncalsaw(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x42 << 2)), 17, 0x7, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg2_gadc_vncalsaw(void) {
	sys_ana_ana_reg2_t *r = (sys_ana_ana_reg2_t*)(SOC_SYS_ANA_REG_BASE + (0x42 << 2));
	return r->gadc_vncalsaw;
}

static inline void sys_ana_ll_set_ana_reg2_gadc_vpcalsaw(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x42 << 2)), 20, 0x7, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg2_gadc_vpcalsaw(void) {
	sys_ana_ana_reg2_t *r = (sys_ana_ana_reg2_t*)(SOC_SYS_ANA_REG_BASE + (0x42 << 2));
	return r->gadc_vpcalsaw;
}

static inline void sys_ana_ll_set_ana_reg2_gadc_vbg_sel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x42 << 2)), 23, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg2_gadc_vbg_sel(void) {
	sys_ana_ana_reg2_t *r = (sys_ana_ana_reg2_t*)(SOC_SYS_ANA_REG_BASE + (0x42 << 2));
	return r->gadc_vbg_sel;
}

static inline void sys_ana_ll_set_ana_reg2_gadc_clk_rlten(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x42 << 2)), 24, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg2_gadc_clk_rlten(void) {
	sys_ana_ana_reg2_t *r = (sys_ana_ana_reg2_t*)(SOC_SYS_ANA_REG_BASE + (0x42 << 2));
	return r->gadc_clk_rlten;
}

static inline void sys_ana_ll_set_ana_reg2_gadc_calintsaw_en(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x42 << 2)), 25, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg2_gadc_calintsaw_en(void) {
	sys_ana_ana_reg2_t *r = (sys_ana_ana_reg2_t*)(SOC_SYS_ANA_REG_BASE + (0x42 << 2));
	return r->gadc_calintsaw_en;
}

static inline void sys_ana_ll_set_ana_reg2_gadc_clk_sel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x42 << 2)), 26, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg2_gadc_clk_sel(void) {
	sys_ana_ana_reg2_t *r = (sys_ana_ana_reg2_t*)(SOC_SYS_ANA_REG_BASE + (0x42 << 2));
	return r->gadc_clk_sel;
}

static inline void sys_ana_ll_set_ana_reg2_gadc_clk_inv(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x42 << 2)), 27, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg2_gadc_clk_inv(void) {
	sys_ana_ana_reg2_t *r = (sys_ana_ana_reg2_t*)(SOC_SYS_ANA_REG_BASE + (0x42 << 2));
	return r->gadc_clk_inv;
}

static inline void sys_ana_ll_set_ana_reg2_gadc_calcap_ch(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x42 << 2)), 28, 0x3, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg2_gadc_calcap_ch(void) {
	sys_ana_ana_reg2_t *r = (sys_ana_ana_reg2_t*)(SOC_SYS_ANA_REG_BASE + (0x42 << 2));
	return r->gadc_calcap_ch;
}

static inline void sys_ana_ll_set_ana_reg2_inbufen(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x42 << 2)), 30, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg2_inbufen(void) {
	sys_ana_ana_reg2_t *r = (sys_ana_ana_reg2_t*)(SOC_SYS_ANA_REG_BASE + (0x42 << 2));
	return r->inbufen;
}

static inline void sys_ana_ll_set_ana_reg2_sar_enspi(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x42 << 2)), 31, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg2_sar_enspi(void) {
	sys_ana_ana_reg2_t *r = (sys_ana_ana_reg2_t*)(SOC_SYS_ANA_REG_BASE + (0x42 << 2));
	return r->sar_enspi;
}

//reg ana_reg3:

static inline void sys_ana_ll_set_ana_reg3_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_ANA_REG_BASE + (0x43 << 2)), v);
}

static inline uint32_t sys_ana_ll_get_ana_reg3_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_ANA_REG_BASE + (0x43 << 2));
}

static inline void sys_ana_ll_set_ana_reg3_preamp_isel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x43 << 2)), 0, 0x7, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg3_preamp_isel(void) {
	sys_ana_ana_reg3_t *r = (sys_ana_ana_reg3_t*)(SOC_SYS_ANA_REG_BASE + (0x43 << 2));
	return r->preamp_isel;
}

static inline void sys_ana_ll_set_ana_reg3_refbuff_isel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x43 << 2)), 3, 0x7, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg3_refbuff_isel(void) {
	sys_ana_ana_reg3_t *r = (sys_ana_ana_reg3_t*)(SOC_SYS_ANA_REG_BASE + (0x43 << 2));
	return r->refbuff_isel;
}

static inline void sys_ana_ll_set_ana_reg3_vref_sel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x43 << 2)), 6, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg3_vref_sel(void) {
	sys_ana_ana_reg3_t *r = (sys_ana_ana_reg3_t*)(SOC_SYS_ANA_REG_BASE + (0x43 << 2));
	return r->vref_sel;
}

static inline void sys_ana_ll_set_ana_reg3_offset_en(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x43 << 2)), 7, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg3_offset_en(void) {
	sys_ana_ana_reg3_t *r = (sys_ana_ana_reg3_t*)(SOC_SYS_ANA_REG_BASE + (0x43 << 2));
	return r->offset_en;
}

static inline void sys_ana_ll_set_ana_reg3_hpssren(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x43 << 2)), 8, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg3_hpssren(void) {
	sys_ana_ana_reg3_t *r = (sys_ana_ana_reg3_t*)(SOC_SYS_ANA_REG_BASE + (0x43 << 2));
	return r->hpssren;
}

static inline void sys_ana_ll_set_ana_reg3_ck_sel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x43 << 2)), 9, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg3_ck_sel(void) {
	sys_ana_ana_reg3_t *r = (sys_ana_ana_reg3_t*)(SOC_SYS_ANA_REG_BASE + (0x43 << 2));
	return r->ck_sel;
}

static inline void sys_ana_ll_set_ana_reg3_lppllvddsel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x43 << 2)), 10, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg3_lppllvddsel(void) {
	sys_ana_ana_reg3_t *r = (sys_ana_ana_reg3_t*)(SOC_SYS_ANA_REG_BASE + (0x43 << 2));
	return r->lppllvddsel;
}

static inline void sys_ana_ll_set_ana_reg3_pwd_xtalldo_spi(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x43 << 2)), 11, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg3_pwd_xtalldo_spi(void) {
	sys_ana_ana_reg3_t *r = (sys_ana_ana_reg3_t*)(SOC_SYS_ANA_REG_BASE + (0x43 << 2));
	return r->pwd_xtalldo_spi;
}

static inline void sys_ana_ll_set_ana_reg3_iamp(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x43 << 2)), 12, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg3_iamp(void) {
	sys_ana_ana_reg3_t *r = (sys_ana_ana_reg3_t*)(SOC_SYS_ANA_REG_BASE + (0x43 << 2));
	return r->iamp;
}

static inline void sys_ana_ll_set_ana_reg3_vddren(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x43 << 2)), 13, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg3_vddren(void) {
	sys_ana_ana_reg3_t *r = (sys_ana_ana_reg3_t*)(SOC_SYS_ANA_REG_BASE + (0x43 << 2));
	return r->vddren;
}

static inline void sys_ana_ll_set_ana_reg3_xamp(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x43 << 2)), 14, 0x3f, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg3_xamp(void) {
	sys_ana_ana_reg3_t *r = (sys_ana_ana_reg3_t*)(SOC_SYS_ANA_REG_BASE + (0x43 << 2));
	return r->xamp;
}

static inline void sys_ana_ll_set_ana_reg3_vosel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x43 << 2)), 20, 0x1f, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg3_vosel(void) {
	sys_ana_ana_reg3_t *r = (sys_ana_ana_reg3_t*)(SOC_SYS_ANA_REG_BASE + (0x43 << 2));
	return r->vosel;
}

static inline void sys_ana_ll_set_ana_reg3_en_xtalh_sleep(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x43 << 2)), 25, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg3_en_xtalh_sleep(void) {
	sys_ana_ana_reg3_t *r = (sys_ana_ana_reg3_t*)(SOC_SYS_ANA_REG_BASE + (0x43 << 2));
	return r->en_xtalh_sleep;
}

static inline void sys_ana_ll_set_ana_reg3_digbufb_hpen(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x43 << 2)), 26, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg3_digbufb_hpen(void) {
	sys_ana_ana_reg3_t *r = (sys_ana_ana_reg3_t*)(SOC_SYS_ANA_REG_BASE + (0x43 << 2));
	return r->digbufb_hpen;
}

static inline void sys_ana_ll_set_ana_reg3_bufictrl(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x43 << 2)), 27, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg3_bufictrl(void) {
	sys_ana_ana_reg3_t *r = (sys_ana_ana_reg3_t*)(SOC_SYS_ANA_REG_BASE + (0x43 << 2));
	return r->bufictrl;
}

static inline void sys_ana_ll_set_ana_reg3_ibias_ctrl(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x43 << 2)), 28, 0x3, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg3_ibias_ctrl(void) {
	sys_ana_ana_reg3_t *r = (sys_ana_ana_reg3_t*)(SOC_SYS_ANA_REG_BASE + (0x43 << 2));
	return r->ibias_ctrl;
}

static inline void sys_ana_ll_set_ana_reg3_icore_ctrl(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x43 << 2)), 30, 0x3, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg3_icore_ctrl(void) {
	sys_ana_ana_reg3_t *r = (sys_ana_ana_reg3_t*)(SOC_SYS_ANA_REG_BASE + (0x43 << 2));
	return r->icore_ctrl;
}

//reg ana_reg4:

static inline void sys_ana_ll_set_ana_reg4_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_ANA_REG_BASE + (0x44 << 2)), v);
}

static inline uint32_t sys_ana_ll_get_ana_reg4_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_ANA_REG_BASE + (0x44 << 2));
}

static inline void sys_ana_ll_set_ana_reg4_temp_gsel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x44 << 2)), 0, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg4_temp_gsel(void) {
	sys_ana_ana_reg4_t *r = (sys_ana_ana_reg4_t*)(SOC_SYS_ANA_REG_BASE + (0x44 << 2));
	return r->temp_gsel;
}

static inline void sys_ana_ll_set_ana_reg4_vbg_0v9sel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x44 << 2)), 1, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg4_vbg_0v9sel(void) {
	sys_ana_ana_reg4_t *r = (sys_ana_ana_reg4_t*)(SOC_SYS_ANA_REG_BASE + (0x44 << 2));
	return r->vbg_0v9sel;
}

static inline void sys_ana_ll_set_ana_reg4_ck2xsel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x44 << 2)), 2, 0x7fff, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg4_ck2xsel(void) {
	sys_ana_ana_reg4_t *r = (sys_ana_ana_reg4_t*)(SOC_SYS_ANA_REG_BASE + (0x44 << 2));
	return r->ck2xsel;
}

static inline void sys_ana_ll_set_ana_reg4_nc_17_21(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x44 << 2)), 17, 0x1f, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg4_nc_17_21(void) {
	sys_ana_ana_reg4_t *r = (sys_ana_ana_reg4_t*)(SOC_SYS_ANA_REG_BASE + (0x44 << 2));
	return r->nc_17_21;
}

static inline void sys_ana_ll_set_ana_reg4_dpll_vctrl_tsten(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x44 << 2)), 22, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg4_dpll_vctrl_tsten(void) {
	sys_ana_ana_reg4_t *r = (sys_ana_ana_reg4_t*)(SOC_SYS_ANA_REG_BASE + (0x44 << 2));
	return r->dpll_vctrl_tsten;
}

static inline void sys_ana_ll_set_ana_reg4_spilatchb_rc32k(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x44 << 2)), 23, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg4_spilatchb_rc32k(void) {
	sys_ana_ana_reg4_t *r = (sys_ana_ana_reg4_t*)(SOC_SYS_ANA_REG_BASE + (0x44 << 2));
	return r->spilatchb_rc32k;
}

static inline void sys_ana_ll_set_ana_reg4_rc32k_refclk_en(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x44 << 2)), 24, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg4_rc32k_refclk_en(void) {
	sys_ana_ana_reg4_t *r = (sys_ana_ana_reg4_t*)(SOC_SYS_ANA_REG_BASE + (0x44 << 2));
	return r->rc32k_refclk_en;
}

static inline void sys_ana_ll_set_ana_reg4_en_vddflashtest(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x44 << 2)), 25, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg4_en_vddflashtest(void) {
	sys_ana_ana_reg4_t *r = (sys_ana_ana_reg4_t*)(SOC_SYS_ANA_REG_BASE + (0x44 << 2));
	return r->en_vddflashtest;
}

static inline void sys_ana_ll_set_ana_reg4_vddgpio_sel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x44 << 2)), 26, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg4_vddgpio_sel(void) {
	sys_ana_ana_reg4_t *r = (sys_ana_ana_reg4_t*)(SOC_SYS_ANA_REG_BASE + (0x44 << 2));
	return r->vddgpio_sel;
}

static inline void sys_ana_ll_set_ana_reg4_ck_tst_en(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x44 << 2)), 27, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg4_ck_tst_en(void) {
	sys_ana_ana_reg4_t *r = (sys_ana_ana_reg4_t*)(SOC_SYS_ANA_REG_BASE + (0x44 << 2));
	return r->ck_tst_en;
}

static inline void sys_ana_ll_set_ana_reg4_vpsramsel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x44 << 2)), 28, 0x3, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg4_vpsramsel(void) {
	sys_ana_ana_reg4_t *r = (sys_ana_ana_reg4_t*)(SOC_SYS_ANA_REG_BASE + (0x44 << 2));
	return r->vpsramsel;
}

static inline void sys_ana_ll_set_ana_reg4_swb(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x44 << 2)), 30, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg4_swb(void) {
	sys_ana_ana_reg4_t *r = (sys_ana_ana_reg4_t*)(SOC_SYS_ANA_REG_BASE + (0x44 << 2));
	return r->swb;
}

static inline void sys_ana_ll_set_ana_reg4_en_psramldo(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x44 << 2)), 31, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg4_en_psramldo(void) {
	sys_ana_ana_reg4_t *r = (sys_ana_ana_reg4_t*)(SOC_SYS_ANA_REG_BASE + (0x44 << 2));
	return r->en_psramldo;
}

//reg ana_reg5:

static inline void sys_ana_ll_set_ana_reg5_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_ANA_REG_BASE + (0x45 << 2)), v);
}

static inline uint32_t sys_ana_ll_get_ana_reg5_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_ANA_REG_BASE + (0x45 << 2));
}

__FLASH_BOOT_CODE static inline void sys_ana_ll_set_ana_reg5_pwd_rosc_spi_flash(uint32_t v) {
	sys_ana_set_ana_reg_bit_flash((SOC_SYS_ANA_REG_BASE + (0x45 << 2)), 0, 0x1, v);
}

/* Runtime version (in PSRAM) - removed __FLASH_BOOT_CODE */
static inline void sys_ana_ll_set_ana_reg5_pwd_rosc_spi(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x45 << 2)), 0, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg5_pwd_rosc_spi(void) {
	sys_ana_ana_reg5_t *r = (sys_ana_ana_reg5_t*)(SOC_SYS_ANA_REG_BASE + (0x45 << 2));
	return r->pwd_rosc_spi;
}

static inline void sys_ana_ll_set_ana_reg5_en_xtall(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x45 << 2)), 1, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg5_en_xtall(void) {
	sys_ana_ana_reg5_t *r = (sys_ana_ana_reg5_t*)(SOC_SYS_ANA_REG_BASE + (0x45 << 2));
	return r->en_xtall;
}

static inline void sys_ana_ll_set_ana_reg5_cktst_sel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x45 << 2)), 2, 0x3, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg5_cktst_sel(void) {
	sys_ana_ana_reg5_t *r = (sys_ana_ana_reg5_t*)(SOC_SYS_ANA_REG_BASE + (0x45 << 2));
	return r->cktst_sel;
}

static inline void sys_ana_ll_set_ana_reg5_en_temp(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x45 << 2)), 4, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg5_en_temp(void) {
	sys_ana_ana_reg5_t *r = (sys_ana_ana_reg5_t*)(SOC_SYS_ANA_REG_BASE + (0x45 << 2));
	return r->en_temp;
}

static inline void sys_ana_ll_set_ana_reg5_en_dpll(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x45 << 2)), 5, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg5_en_dpll(void) {
	sys_ana_ana_reg5_t *r = (sys_ana_ana_reg5_t*)(SOC_SYS_ANA_REG_BASE + (0x45 << 2));
	return r->en_dpll;
}

static inline void sys_ana_ll_set_ana_reg5_en_cb(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x45 << 2)), 6, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg5_en_cb(void) {
	sys_ana_ana_reg5_t *r = (sys_ana_ana_reg5_t*)(SOC_SYS_ANA_REG_BASE + (0x45 << 2));
	return r->en_cb;
}

static inline void sys_ana_ll_set_ana_reg5_pmu_test(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x45 << 2)), 7, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg5_pmu_test(void) {
	sys_ana_ana_reg5_t *r = (sys_ana_ana_reg5_t*)(SOC_SYS_ANA_REG_BASE + (0x45 << 2));
	return r->pmu_test;
}

static inline void sys_ana_ll_set_ana_reg5_anabufsel_rx0(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x45 << 2)), 8, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg5_anabufsel_rx0(void) {
	sys_ana_ana_reg5_t *r = (sys_ana_ana_reg5_t*)(SOC_SYS_ANA_REG_BASE + (0x45 << 2));
	return r->anabufsel_rx0;
}

static inline void sys_ana_ll_set_ana_reg5_anabufsel_tx0(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x45 << 2)), 9, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg5_anabufsel_tx0(void) {
	sys_ana_ana_reg5_t *r = (sys_ana_ana_reg5_t*)(SOC_SYS_ANA_REG_BASE + (0x45 << 2));
	return r->anabufsel_tx0;
}

static inline void sys_ana_ll_set_ana_reg5_anabufsel_rx1(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x45 << 2)), 10, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg5_anabufsel_rx1(void) {
	sys_ana_ana_reg5_t *r = (sys_ana_ana_reg5_t*)(SOC_SYS_ANA_REG_BASE + (0x45 << 2));
	return r->anabufsel_rx1;
}

static inline void sys_ana_ll_set_ana_reg5_anabufsel_tx1(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x45 << 2)), 11, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg5_anabufsel_tx1(void) {
	sys_ana_ana_reg5_t *r = (sys_ana_ana_reg5_t*)(SOC_SYS_ANA_REG_BASE + (0x45 << 2));
	return r->anabufsel_tx1;
}

static inline void sys_ana_ll_set_ana_reg5_rosc_disable(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x45 << 2)), 12, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg5_rosc_disable(void) {
	sys_ana_ana_reg5_t *r = (sys_ana_ana_reg5_t*)(SOC_SYS_ANA_REG_BASE + (0x45 << 2));
	return r->rosc_disable;
}

static inline void sys_ana_ll_set_ana_reg5_ldo2xvsel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x45 << 2)), 13, 0x7, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg5_ldo2xvsel(void) {
	sys_ana_ana_reg5_t *r = (sys_ana_ana_reg5_t*)(SOC_SYS_ANA_REG_BASE + (0x45 << 2));
	return r->ldo2xvsel;
}

static inline void sys_ana_ll_set_ana_reg5_itune_xtall(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x45 << 2)), 16, 0xf, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg5_itune_xtall(void) {
	sys_ana_ana_reg5_t *r = (sys_ana_ana_reg5_t*)(SOC_SYS_ANA_REG_BASE + (0x45 << 2));
	return r->itune_xtall;
}

static inline void sys_ana_ll_set_ana_reg5_xtall_ten(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x45 << 2)), 20, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg5_xtall_ten(void) {
	sys_ana_ana_reg5_t *r = (sys_ana_ana_reg5_t*)(SOC_SYS_ANA_REG_BASE + (0x45 << 2));
	return r->xtall_ten;
}

static inline void sys_ana_ll_set_ana_reg5_rosc_tsten(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x45 << 2)), 21, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg5_rosc_tsten(void) {
	sys_ana_ana_reg5_t *r = (sys_ana_ana_reg5_t*)(SOC_SYS_ANA_REG_BASE + (0x45 << 2));
	return r->rosc_tsten;
}

static inline void sys_ana_ll_set_ana_reg5_bcal_start(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x45 << 2)), 22, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg5_bcal_start(void) {
	sys_ana_ana_reg5_t *r = (sys_ana_ana_reg5_t*)(SOC_SYS_ANA_REG_BASE + (0x45 << 2));
	return r->bcal_start;
}

static inline void sys_ana_ll_set_ana_reg5_bcal_en(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x45 << 2)), 23, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg5_bcal_en(void) {
	sys_ana_ana_reg5_t *r = (sys_ana_ana_reg5_t*)(SOC_SYS_ANA_REG_BASE + (0x45 << 2));
	return r->bcal_en;
}

static inline void sys_ana_ll_set_ana_reg5_bcal_sel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x45 << 2)), 24, 0x7, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg5_bcal_sel(void) {
	sys_ana_ana_reg5_t *r = (sys_ana_ana_reg5_t*)(SOC_SYS_ANA_REG_BASE + (0x45 << 2));
	return r->bcal_sel;
}

static inline void sys_ana_ll_set_ana_reg5_vbias(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x45 << 2)), 27, 0x1f, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg5_vbias(void) {
	sys_ana_ana_reg5_t *r = (sys_ana_ana_reg5_t*)(SOC_SYS_ANA_REG_BASE + (0x45 << 2));
	return r->vbias;
}

//reg ana_reg6:

static inline void sys_ana_ll_set_ana_reg6_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_ANA_REG_BASE + (0x46 << 2)), v);
}

static inline uint32_t sys_ana_ll_get_ana_reg6_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_ANA_REG_BASE + (0x46 << 2));
}

static inline void sys_ana_ll_set_ana_reg6_calib_interval(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x46 << 2)), 0, 0x3ff, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg6_calib_interval(void) {
	sys_ana_ana_reg6_t *r = (sys_ana_ana_reg6_t*)(SOC_SYS_ANA_REG_BASE + (0x46 << 2));
	return r->calib_interval;
}

static inline void sys_ana_ll_set_ana_reg6_modify_interval(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x46 << 2)), 10, 0x3f, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg6_modify_interval(void) {
	sys_ana_ana_reg6_t *r = (sys_ana_ana_reg6_t*)(SOC_SYS_ANA_REG_BASE + (0x46 << 2));
	return r->modify_interval;
}

static inline void sys_ana_ll_set_ana_reg6_xtal_wakeup_time(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x46 << 2)), 16, 0xf, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg6_xtal_wakeup_time(void) {
	sys_ana_ana_reg6_t *r = (sys_ana_ana_reg6_t*)(SOC_SYS_ANA_REG_BASE + (0x46 << 2));
	return r->xtal_wakeup_time;
}

static inline void sys_ana_ll_set_ana_reg6_spi_trig(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x46 << 2)), 20, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg6_spi_trig(void) {
	sys_ana_ana_reg6_t *r = (sys_ana_ana_reg6_t*)(SOC_SYS_ANA_REG_BASE + (0x46 << 2));
	return r->spi_trig;
}

static inline void sys_ana_ll_set_ana_reg6_modifi_auto(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x46 << 2)), 21, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg6_modifi_auto(void) {
	sys_ana_ana_reg6_t *r = (sys_ana_ana_reg6_t*)(SOC_SYS_ANA_REG_BASE + (0x46 << 2));
	return r->modifi_auto;
}

static inline void sys_ana_ll_set_ana_reg6_calib_auto(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x46 << 2)), 22, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg6_calib_auto(void) {
	sys_ana_ana_reg6_t *r = (sys_ana_ana_reg6_t*)(SOC_SYS_ANA_REG_BASE + (0x46 << 2));
	return r->calib_auto;
}

static inline void sys_ana_ll_set_ana_reg6_cal_mode(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x46 << 2)), 23, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg6_cal_mode(void) {
	sys_ana_ana_reg6_t *r = (sys_ana_ana_reg6_t*)(SOC_SYS_ANA_REG_BASE + (0x46 << 2));
	return r->cal_mode;
}

static inline void sys_ana_ll_set_ana_reg6_manu_ena(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x46 << 2)), 24, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg6_manu_ena(void) {
	sys_ana_ana_reg6_t *r = (sys_ana_ana_reg6_t*)(SOC_SYS_ANA_REG_BASE + (0x46 << 2));
	return r->manu_ena;
}

static inline void sys_ana_ll_set_ana_reg6_manu_cin(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x46 << 2)), 25, 0x7f, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg6_manu_cin(void) {
	sys_ana_ana_reg6_t *r = (sys_ana_ana_reg6_t*)(SOC_SYS_ANA_REG_BASE + (0x46 << 2));
	return r->manu_cin;
}

//reg ana_reg7:

static inline void sys_ana_ll_set_ana_reg7_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_ANA_REG_BASE + (0x47 << 2)), v);
}

static inline uint32_t sys_ana_ll_get_ana_reg7_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_ANA_REG_BASE + (0x47 << 2));
}

static inline void sys_ana_ll_set_ana_reg7_clk_sel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x47 << 2)), 0, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg7_clk_sel(void) {
	sys_ana_ana_reg7_t *r = (sys_ana_ana_reg7_t*)(SOC_SYS_ANA_REG_BASE + (0x47 << 2));
	return r->clk_sel;
}

static inline void sys_ana_ll_set_ana_reg7_timer_wkrstn(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x47 << 2)), 1, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg7_timer_wkrstn(void) {
	sys_ana_ana_reg7_t *r = (sys_ana_ana_reg7_t*)(SOC_SYS_ANA_REG_BASE + (0x47 << 2));
	return r->timer_wkrstn;
}

static inline void sys_ana_ll_set_ana_reg7_dldohp(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x47 << 2)), 2, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg7_dldohp(void) {
	sys_ana_ana_reg7_t *r = (sys_ana_ana_reg7_t*)(SOC_SYS_ANA_REG_BASE + (0x47 << 2));
	return r->dldohp;
}

__FLASH_BOOT_CODE static inline void sys_ana_ll_set_ana_reg7_vporsel_flash(uint32_t v) {
	sys_ana_set_ana_reg_bit_flash((SOC_SYS_ANA_REG_BASE + (0x47 << 2)), 3, 0x1, v);
}

static inline void sys_ana_ll_set_ana_reg7_vporsel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x47 << 2)), 3, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg7_vporsel(void) {
	sys_ana_ana_reg7_t *r = (sys_ana_ana_reg7_t*)(SOC_SYS_ANA_REG_BASE + (0x47 << 2));
	return r->vporsel;
}

static inline void sys_ana_ll_set_ana_reg7_vbspbuf_lp(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x47 << 2)), 4, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg7_vbspbuf_lp(void) {
	sys_ana_ana_reg7_t *r = (sys_ana_ana_reg7_t*)(SOC_SYS_ANA_REG_BASE + (0x47 << 2));
	return r->vbspbuf_lp;
}

static inline void sys_ana_ll_set_ana_reg7_iocurlim(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x47 << 2)), 5, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg7_iocurlim(void) {
	sys_ana_ana_reg7_t *r = (sys_ana_ana_reg7_t*)(SOC_SYS_ANA_REG_BASE + (0x47 << 2));
	return r->iocurlim;
}

static inline void sys_ana_ll_set_ana_reg7_vanaldosel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x47 << 2)), 6, 0xf, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg7_vanaldosel(void) {
	sys_ana_ana_reg7_t *r = (sys_ana_ana_reg7_t*)(SOC_SYS_ANA_REG_BASE + (0x47 << 2));
	return r->vanaldosel;
}

static inline void sys_ana_ll_set_ana_reg7_aldohp(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x47 << 2)), 10, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg7_aldohp(void) {
	sys_ana_ana_reg7_t *r = (sys_ana_ana_reg7_t*)(SOC_SYS_ANA_REG_BASE + (0x47 << 2));
	return r->aldohp;
}

static inline void sys_ana_ll_set_ana_reg7_anacurlim(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x47 << 2)), 11, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg7_anacurlim(void) {
	sys_ana_ana_reg7_t *r = (sys_ana_ana_reg7_t*)(SOC_SYS_ANA_REG_BASE + (0x47 << 2));
	return r->anacurlim;
}

static inline void sys_ana_ll_set_ana_reg7_envrefh1v(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x47 << 2)), 12, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg7_envrefh1v(void) {
	sys_ana_ana_reg7_t *r = (sys_ana_ana_reg7_t*)(SOC_SYS_ANA_REG_BASE + (0x47 << 2));
	return r->envrefh1v;
}

static inline void sys_ana_ll_set_ana_reg7_violdosel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x47 << 2)), 13, 0x7, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg7_violdosel(void) {
	sys_ana_ana_reg7_t *r = (sys_ana_ana_reg7_t*)(SOC_SYS_ANA_REG_BASE + (0x47 << 2));
	return r->violdosel;
}

static inline void sys_ana_ll_set_ana_reg7_vbatdetsel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x47 << 2)), 16, 0x3, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg7_vbatdetsel(void) {
	sys_ana_ana_reg7_t *r = (sys_ana_ana_reg7_t*)(SOC_SYS_ANA_REG_BASE + (0x47 << 2));
	return r->vbatdetsel;
}

static inline void sys_ana_ll_set_ana_reg7_en_compe(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x47 << 2)), 18, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg7_en_compe(void) {
	sys_ana_ana_reg7_t *r = (sys_ana_ana_reg7_t*)(SOC_SYS_ANA_REG_BASE + (0x47 << 2));
	return r->en_compe;
}

static inline void sys_ana_ll_set_ana_reg7_spi_pwd_regpow(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x47 << 2)), 19, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg7_spi_pwd_regpow(void) {
	sys_ana_ana_reg7_t *r = (sys_ana_ana_reg7_t*)(SOC_SYS_ANA_REG_BASE + (0x47 << 2));
	return r->spi_pwd_regpow;
}

static inline void sys_ana_ll_set_ana_reg7_bypassen(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x47 << 2)), 20, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg7_bypassen(void) {
	sys_ana_ana_reg7_t *r = (sys_ana_ana_reg7_t*)(SOC_SYS_ANA_REG_BASE + (0x47 << 2));
	return r->bypassen;
}

static inline void sys_ana_ll_set_ana_reg7_ioldolp(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x47 << 2)), 21, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg7_ioldolp(void) {
	sys_ana_ana_reg7_t *r = (sys_ana_ana_reg7_t*)(SOC_SYS_ANA_REG_BASE + (0x47 << 2));
	return r->ioldolp;
}

static inline void sys_ana_ll_set_ana_reg7_bgcal(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x47 << 2)), 22, 0x3f, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg7_bgcal(void) {
	sys_ana_ana_reg7_t *r = (sys_ana_ana_reg7_t*)(SOC_SYS_ANA_REG_BASE + (0x47 << 2));
	return r->bgcal;
}

__FLASH_BOOT_CODE static inline void sys_ana_ll_set_ana_reg7_vbgcalmode_flash(uint32_t v) {
	sys_ana_set_ana_reg_bit_flash((SOC_SYS_ANA_REG_BASE + (0x47 << 2)), 28, 0x1, v);
}

static inline void sys_ana_ll_set_ana_reg7_vbgcalmode(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x47 << 2)), 28, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg7_vbgcalmode(void) {
	sys_ana_ana_reg7_t *r = (sys_ana_ana_reg7_t*)(SOC_SYS_ANA_REG_BASE + (0x47 << 2));
	return r->vbgcalmode;
}

__FLASH_BOOT_CODE static inline void sys_ana_ll_set_ana_reg7_vbgcalstart_flash(uint32_t v) {
	sys_ana_set_ana_reg_bit_flash((SOC_SYS_ANA_REG_BASE + (0x47 << 2)), 29, 0x1, v);
}

static inline void sys_ana_ll_set_ana_reg7_vbgcalstart(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x47 << 2)), 29, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg7_vbgcalstart(void) {
	sys_ana_ana_reg7_t *r = (sys_ana_ana_reg7_t*)(SOC_SYS_ANA_REG_BASE + (0x47 << 2));
	return r->vbgcalstart;
}

__FLASH_BOOT_CODE static inline void sys_ana_ll_set_ana_reg7_pwd_bgcal_flash(uint32_t v) {
	sys_ana_set_ana_reg_bit_flash((SOC_SYS_ANA_REG_BASE + (0x47 << 2)), 30, 0x1, v);
}

static inline void sys_ana_ll_set_ana_reg7_pwd_bgcal(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x47 << 2)), 30, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg7_pwd_bgcal(void) {
	sys_ana_ana_reg7_t *r = (sys_ana_ana_reg7_t*)(SOC_SYS_ANA_REG_BASE + (0x47 << 2));
	return r->pwd_bgcal;
}

static inline void sys_ana_ll_set_ana_reg7_spi_envbg(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x47 << 2)), 31, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg7_spi_envbg(void) {
	sys_ana_ana_reg7_t *r = (sys_ana_ana_reg7_t*)(SOC_SYS_ANA_REG_BASE + (0x47 << 2));
	return r->spi_envbg;
}

//reg ana_reg8:

static inline void sys_ana_ll_set_ana_reg8_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_ANA_REG_BASE + (0x48 << 2)), v);
}

static inline uint32_t sys_ana_ll_get_ana_reg8_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_ANA_REG_BASE + (0x48 << 2));
}

static inline void sys_ana_ll_set_ana_reg8_asoft_stc(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x48 << 2)), 0, 0xf, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg8_asoft_stc(void) {
	sys_ana_ana_reg8_t *r = (sys_ana_ana_reg8_t*)(SOC_SYS_ANA_REG_BASE + (0x48 << 2));
	return r->asoft_stc;
}

static inline void sys_ana_ll_set_ana_reg8_rst_timerwks1v(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x48 << 2)), 4, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg8_rst_timerwks1v(void) {
	sys_ana_ana_reg8_t *r = (sys_ana_ana_reg8_t*)(SOC_SYS_ANA_REG_BASE + (0x48 << 2));
	return r->rst_timerwks1v;
}

static inline void sys_ana_ll_set_ana_reg8_dldo_czsel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x48 << 2)), 5, 0x7, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg8_dldo_czsel(void) {
	sys_ana_ana_reg8_t *r = (sys_ana_ana_reg8_t*)(SOC_SYS_ANA_REG_BASE + (0x48 << 2));
	return r->dldo_czsel;
}

static inline void sys_ana_ll_set_ana_reg8_digcurlim(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x48 << 2)), 8, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg8_digcurlim(void) {
	sys_ana_ana_reg8_t *r = (sys_ana_ana_reg8_t*)(SOC_SYS_ANA_REG_BASE + (0x48 << 2));
	return r->digcurlim;
}

/* Declaration for _flash version - defined in sys_hal.c */
__FLASH_BOOT_CODE void sys_ana_ll_set_ana_reg8_spi_latch1v_flash(uint32_t v);

/* Runtime version (in PSRAM) - removed __FLASH_BOOT_CODE */
static inline void sys_ana_ll_set_ana_reg8_spi_latch1v(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x48 << 2)), 9, 0x1, v);
}

__IRAM_SEC static inline void sys_ana_ll_set_ana_reg8_spi_latch1v_iram(uint32_t v) {
	sys_ana_set_ana_reg_bit_iram((SOC_SYS_ANA_REG_BASE + (0x48 << 2)), 9, 0x1, v);
}
static inline uint32_t sys_ana_ll_get_ana_reg8_spi_latch1v(void) {
	sys_ana_ana_reg8_t *r = (sys_ana_ana_reg8_t*)(SOC_SYS_ANA_REG_BASE + (0x48 << 2));
	return r->spi_latch1v;
}

static inline void sys_ana_ll_set_ana_reg8_dldo_rzsel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x48 << 2)), 10, 0x3, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg8_dldo_rzsel(void) {
	sys_ana_ana_reg8_t *r = (sys_ana_ana_reg8_t*)(SOC_SYS_ANA_REG_BASE + (0x48 << 2));
	return r->dldo_rzsel;
}

static inline void sys_ana_ll_set_ana_reg8_lvsleep_wkrstn(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x48 << 2)), 12, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg8_lvsleep_wkrstn(void) {
	sys_ana_ana_reg8_t *r = (sys_ana_ana_reg8_t*)(SOC_SYS_ANA_REG_BASE + (0x48 << 2));
	return r->lvsleep_wkrstn;
}

static inline void sys_ana_ll_set_ana_reg8_gpiowk_rstn(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x48 << 2)), 13, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg8_gpiowk_rstn(void) {
	sys_ana_ana_reg8_t *r = (sys_ana_ana_reg8_t*)(SOC_SYS_ANA_REG_BASE + (0x48 << 2));
	return r->gpiowk_rstn;
}

static inline void sys_ana_ll_set_ana_reg8_rtcwk_rstn(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x48 << 2)), 14, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg8_rtcwk_rstn(void) {
	sys_ana_ana_reg8_t *r = (sys_ana_ana_reg8_t*)(SOC_SYS_ANA_REG_BASE + (0x48 << 2));
	return r->rtcwk_rstn;
}

static inline void sys_ana_ll_set_ana_reg8_ensfsdd(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x48 << 2)), 15, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg8_ensfsdd(void) {
	sys_ana_ana_reg8_t *r = (sys_ana_ana_reg8_t*)(SOC_SYS_ANA_REG_BASE + (0x48 << 2));
	return r->ensfsdd;
}

static inline void sys_ana_ll_set_ana_reg8_vcorehsel(uint32_t v) {
	sys_ana_set_ana_reg_bit_iram((SOC_SYS_ANA_REG_BASE + (0x48 << 2)), 16, 0xf, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg8_vcorehsel(void) {
	sys_ana_ana_reg8_t *r = (sys_ana_ana_reg8_t*)(SOC_SYS_ANA_REG_BASE + (0x48 << 2));
	return r->vcorehsel;
}

static inline void sys_ana_ll_set_ana_reg8_vcorelsel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x48 << 2)), 20, 0x7, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg8_vcorelsel(void) {
	sys_ana_ana_reg8_t *r = (sys_ana_ana_reg8_t*)(SOC_SYS_ANA_REG_BASE + (0x48 << 2));
	return r->vcorelsel;
}

static inline void sys_ana_ll_set_ana_reg8_vlden(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x48 << 2)), 23, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg8_vlden(void) {
	sys_ana_ana_reg8_t *r = (sys_ana_ana_reg8_t*)(SOC_SYS_ANA_REG_BASE + (0x48 << 2));
	return r->vlden;
}

static inline void sys_ana_ll_set_ana_reg8_rst_gpiowks(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x48 << 2)), 24, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg8_rst_gpiowks(void) {
	sys_ana_ana_reg8_t *r = (sys_ana_ana_reg8_t*)(SOC_SYS_ANA_REG_BASE + (0x48 << 2));
	return r->rst_gpiowks;
}

static inline void sys_ana_ll_set_ana_reg8_aldo_rzsel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x48 << 2)), 25, 0x3, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg8_aldo_rzsel(void) {
	sys_ana_ana_reg8_t *r = (sys_ana_ana_reg8_t*)(SOC_SYS_ANA_REG_BASE + (0x48 << 2));
	return r->aldo_rzsel;
}

static inline void sys_ana_ll_set_ana_reg8_aldo_czsel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x48 << 2)), 27, 0x7, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg8_aldo_czsel(void) {
	sys_ana_ana_reg8_t *r = (sys_ana_ana_reg8_t*)(SOC_SYS_ANA_REG_BASE + (0x48 << 2));
	return r->aldo_czsel;
}

static inline void sys_ana_ll_set_ana_reg8_vtrxspisel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x48 << 2)), 30, 0x3, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg8_vtrxspisel(void) {
	sys_ana_ana_reg8_t *r = (sys_ana_ana_reg8_t*)(SOC_SYS_ANA_REG_BASE + (0x48 << 2));
	return r->vtrxspisel;
}

//reg ana_reg9:

static inline void sys_ana_ll_set_ana_reg9_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_ANA_REG_BASE + (0x49 << 2)), v);
}

static inline uint32_t sys_ana_ll_get_ana_reg9_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_ANA_REG_BASE + (0x49 << 2));
}

static inline void sys_ana_ll_set_ana_reg9_azcd_manu(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x49 << 2)), 0, 0x3f, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg9_azcd_manu(void) {
	sys_ana_ana_reg9_t *r = (sys_ana_ana_reg9_t*)(SOC_SYS_ANA_REG_BASE + (0x49 << 2));
	return r->azcd_manu;
}

static inline void sys_ana_ll_set_ana_reg9_zcdswvs(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x49 << 2)), 6, 0x7, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg9_zcdswvs(void) {
	sys_ana_ana_reg9_t *r = (sys_ana_ana_reg9_t*)(SOC_SYS_ANA_REG_BASE + (0x49 << 2));
	return r->zcdswvs;
}

static inline void sys_ana_ll_set_ana_reg9_zcdsrefsel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x49 << 2)), 9, 0x7, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg9_zcdsrefsel(void) {
	sys_ana_ana_reg9_t *r = (sys_ana_ana_reg9_t*)(SOC_SYS_ANA_REG_BASE + (0x49 << 2));
	return r->zcdsrefsel;
}

static inline void sys_ana_ll_set_ana_reg9_enzcdcalib(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x49 << 2)), 12, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg9_enzcdcalib(void) {
	sys_ana_ana_reg9_t *r = (sys_ana_ana_reg9_t*)(SOC_SYS_ANA_REG_BASE + (0x49 << 2));
	return r->enzcdcalib;
}

static inline void sys_ana_ll_set_ana_reg9_enzcddy(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x49 << 2)), 13, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg9_enzcddy(void) {
	sys_ana_ana_reg9_t *r = (sys_ana_ana_reg9_t*)(SOC_SYS_ANA_REG_BASE + (0x49 << 2));
	return r->enzcddy;
}

static inline void sys_ana_ll_set_ana_reg9_zcdmsel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x49 << 2)), 14, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg9_zcdmsel(void) {
	sys_ana_ana_reg9_t *r = (sys_ana_ana_reg9_t*)(SOC_SYS_ANA_REG_BASE + (0x49 << 2));
	return r->zcdmsel;
}

static inline void sys_ana_ll_set_ana_reg9_spi_timerwken(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x49 << 2)), 15, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg9_spi_timerwken(void) {
	sys_ana_ana_reg9_t *r = (sys_ana_ana_reg9_t*)(SOC_SYS_ANA_REG_BASE + (0x49 << 2));
	return r->spi_timerwken;
}

static inline void sys_ana_ll_set_ana_reg9_spi_byp32pwd(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x49 << 2)), 16, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg9_spi_byp32pwd(void) {
	sys_ana_ana_reg9_t *r = (sys_ana_ana_reg9_t*)(SOC_SYS_ANA_REG_BASE + (0x49 << 2));
	return r->spi_byp32pwd;
}

static inline void sys_ana_ll_set_ana_reg9_sd(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x49 << 2)), 17, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg9_sd(void) {
	sys_ana_ana_reg9_t *r = (sys_ana_ana_reg9_t*)(SOC_SYS_ANA_REG_BASE + (0x49 << 2));
	return r->sd;
}

static inline void sys_ana_ll_set_ana_reg9_iburstsel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x49 << 2)), 18, 0x3, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg9_iburstsel(void) {
	sys_ana_ana_reg9_t *r = (sys_ana_ana_reg9_t*)(SOC_SYS_ANA_REG_BASE + (0x49 << 2));
	return r->iburstsel;
}

static inline void sys_ana_ll_set_ana_reg9_ckfs(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x49 << 2)), 20, 0x3, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg9_ckfs(void) {
	sys_ana_ana_reg9_t *r = (sys_ana_ana_reg9_t*)(SOC_SYS_ANA_REG_BASE + (0x49 << 2));
	return r->ckfs;
}

static inline void sys_ana_ll_set_ana_reg9_ckintsel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x49 << 2)), 22, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg9_ckintsel(void) {
	sys_ana_ana_reg9_t *r = (sys_ana_ana_reg9_t*)(SOC_SYS_ANA_REG_BASE + (0x49 << 2));
	return r->ckintsel;
}

static inline void sys_ana_ll_set_ana_reg9_osccaltrig(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x49 << 2)), 23, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg9_osccaltrig(void) {
	sys_ana_ana_reg9_t *r = (sys_ana_ana_reg9_t*)(SOC_SYS_ANA_REG_BASE + (0x49 << 2));
	return r->osccaltrig;
}

static inline void sys_ana_ll_set_ana_reg9_mroscsel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x49 << 2)), 24, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg9_mroscsel(void) {
	sys_ana_ana_reg9_t *r = (sys_ana_ana_reg9_t*)(SOC_SYS_ANA_REG_BASE + (0x49 << 2));
	return r->mroscsel;
}

static inline void sys_ana_ll_set_ana_reg9_mrosci_cal(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x49 << 2)), 25, 0x7, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg9_mrosci_cal(void) {
	sys_ana_ana_reg9_t *r = (sys_ana_ana_reg9_t*)(SOC_SYS_ANA_REG_BASE + (0x49 << 2));
	return r->mrosci_cal;
}

static inline void sys_ana_ll_set_ana_reg9_mrosccap_cal(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x49 << 2)), 28, 0xf, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg9_mrosccap_cal(void) {
	sys_ana_ana_reg9_t *r = (sys_ana_ana_reg9_t*)(SOC_SYS_ANA_REG_BASE + (0x49 << 2));
	return r->mrosccap_cal;
}

//reg ana_reg10:

static inline void sys_ana_ll_set_ana_reg10_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_ANA_REG_BASE + (0x4a << 2)), v);
}

static inline uint32_t sys_ana_ll_get_ana_reg10_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_ANA_REG_BASE + (0x4a << 2));
}

static inline void sys_ana_ll_set_ana_reg10_sfsr(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x4a << 2)), 0, 0xf, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg10_sfsr(void) {
	sys_ana_ana_reg10_t *r = (sys_ana_ana_reg10_t*)(SOC_SYS_ANA_REG_BASE + (0x4a << 2));
	return r->sfsr;
}

static inline void sys_ana_ll_set_ana_reg10_ensfsaa(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x4a << 2)), 4, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg10_ensfsaa(void) {
	sys_ana_ana_reg10_t *r = (sys_ana_ana_reg10_t*)(SOC_SYS_ANA_REG_BASE + (0x4a << 2));
	return r->ensfsaa;
}

static inline void sys_ana_ll_set_ana_reg10_apfms(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x4a << 2)), 5, 0x1f, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg10_apfms(void) {
	sys_ana_ana_reg10_t *r = (sys_ana_ana_reg10_t*)(SOC_SYS_ANA_REG_BASE + (0x4a << 2));
	return r->apfms;
}

static inline void sys_ana_ll_set_ana_reg10_atmpo_sel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x4a << 2)), 10, 0x3, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg10_atmpo_sel(void) {
	sys_ana_ana_reg10_t *r = (sys_ana_ana_reg10_t*)(SOC_SYS_ANA_REG_BASE + (0x4a << 2));
	return r->atmpo_sel;
}

static inline void sys_ana_ll_set_ana_reg10_ampoen(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x4a << 2)), 12, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg10_ampoen(void) {
	sys_ana_ana_reg10_t *r = (sys_ana_ana_reg10_t*)(SOC_SYS_ANA_REG_BASE + (0x4a << 2));
	return r->ampoen;
}

__FLASH_BOOT_CODE static inline void sys_ana_ll_set_ana_reg10_vbg_rstrtc_en_flash(uint32_t v) {
	sys_ana_set_ana_reg_bit_flash((SOC_SYS_ANA_REG_BASE + (0x4a << 2)), 13, 0x1, v);
}

static inline void sys_ana_ll_set_ana_reg10_vbg_rstrtc_en(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x4a << 2)), 13, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg10_vbg_rstrtc_en(void) {
	sys_ana_ana_reg10_t *r = (sys_ana_ana_reg10_t*)(SOC_SYS_ANA_REG_BASE + (0x4a << 2));
	return r->vbg_rstrtc_en;
}

static inline void sys_ana_ll_set_ana_reg10_avea_sel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x4a << 2)), 14, 0x3, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg10_avea_sel(void) {
	sys_ana_ana_reg10_t *r = (sys_ana_ana_reg10_t*)(SOC_SYS_ANA_REG_BASE + (0x4a << 2));
	return r->avea_sel;
}

static inline void sys_ana_ll_set_ana_reg10_aforcepfm(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x4a << 2)), 16, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg10_aforcepfm(void) {
	sys_ana_ana_reg10_t *r = (sys_ana_ana_reg10_t*)(SOC_SYS_ANA_REG_BASE + (0x4a << 2));
	return r->aforcepfm;
}

static inline void sys_ana_ll_set_ana_reg10_acls(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x4a << 2)), 17, 0x7, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg10_acls(void) {
	sys_ana_ana_reg10_t *r = (sys_ana_ana_reg10_t*)(SOC_SYS_ANA_REG_BASE + (0x4a << 2));
	return r->acls;
}

static inline void sys_ana_ll_set_ana_reg10_aswrsten(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x4a << 2)), 20, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg10_aswrsten(void) {
	sys_ana_ana_reg10_t *r = (sys_ana_ana_reg10_t*)(SOC_SYS_ANA_REG_BASE + (0x4a << 2));
	return r->aswrsten;
}

static inline void sys_ana_ll_set_ana_reg10_aripc(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x4a << 2)), 21, 0x7, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg10_aripc(void) {
	sys_ana_ana_reg10_t *r = (sys_ana_ana_reg10_t*)(SOC_SYS_ANA_REG_BASE + (0x4a << 2));
	return r->aripc;
}

static inline void sys_ana_ll_set_ana_reg10_arampc(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x4a << 2)), 24, 0xf, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg10_arampc(void) {
	sys_ana_ana_reg10_t *r = (sys_ana_ana_reg10_t*)(SOC_SYS_ANA_REG_BASE + (0x4a << 2));
	return r->arampc;
}

static inline void sys_ana_ll_set_ana_reg10_arampcen(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x4a << 2)), 28, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg10_arampcen(void) {
	sys_ana_ana_reg10_t *r = (sys_ana_ana_reg10_t*)(SOC_SYS_ANA_REG_BASE + (0x4a << 2));
	return r->arampcen;
}

static inline void sys_ana_ll_set_ana_reg10_aenburst(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x4a << 2)), 29, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg10_aenburst(void) {
	sys_ana_ana_reg10_t *r = (sys_ana_ana_reg10_t*)(SOC_SYS_ANA_REG_BASE + (0x4a << 2));
	return r->aenburst;
}

static inline void sys_ana_ll_set_ana_reg10_apfmen(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x4a << 2)), 30, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg10_apfmen(void) {
	sys_ana_ana_reg10_t *r = (sys_ana_ana_reg10_t*)(SOC_SYS_ANA_REG_BASE + (0x4a << 2));
	return r->apfmen;
}

static inline void sys_ana_ll_set_ana_reg10_aldosel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x4a << 2)), 31, 0x1, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg10_aldosel(void) {
	sys_ana_ana_reg10_t *r = (sys_ana_ana_reg10_t*)(SOC_SYS_ANA_REG_BASE + (0x4a << 2));
	return r->aldosel;
}

//reg ana_reg11:

static inline void sys_ana_ll_set_ana_reg11_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_ANA_REG_BASE + (0x4b << 2)), v);
}

static inline uint32_t sys_ana_ll_get_ana_reg11_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_ANA_REG_BASE + (0x4b << 2));
}

static inline void sys_ana_ll_set_ana_reg11_gpiowk(uint32_t v) {
	sys_ana_set_ana_reg_bit_iram((SOC_SYS_ANA_REG_BASE + (0x4b << 2)), 0, 0xffffff, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg11_gpiowk(void) {
	sys_ana_ana_reg11_t *r = (sys_ana_ana_reg11_t*)(SOC_SYS_ANA_REG_BASE + (0x4b << 2));
	return r->gpiowk;
}

static inline void sys_ana_ll_set_ana_reg11_rtcsel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x4b << 2)), 24, 0xf, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg11_rtcsel(void) {
	sys_ana_ana_reg11_t *r = (sys_ana_ana_reg11_t*)(SOC_SYS_ANA_REG_BASE + (0x4b << 2));
	return r->rtcsel;
}

static inline void sys_ana_ll_set_ana_reg11_timersel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x4b << 2)), 28, 0xf, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg11_timersel(void) {
	sys_ana_ana_reg11_t *r = (sys_ana_ana_reg11_t*)(SOC_SYS_ANA_REG_BASE + (0x4b << 2));
	return r->timersel;
}

//reg ana_reg12:

static inline void sys_ana_ll_set_ana_reg12_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_ANA_REG_BASE + (0x4c << 2)), v);
}

static inline uint32_t sys_ana_ll_get_ana_reg12_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_ANA_REG_BASE + (0x4c << 2));
}

static inline void sys_ana_ll_set_ana_reg12_timersel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x4c << 2)), 0, 0xffffffff, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg12_timersel(void) {
	sys_ana_ana_reg12_t *r = (sys_ana_ana_reg12_t*)(SOC_SYS_ANA_REG_BASE + (0x4c << 2));
	return r->timersel;
}

//reg ana_reg13:

static inline void sys_ana_ll_set_ana_reg13_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_ANA_REG_BASE + (0x4d << 2)), v);
}

static inline uint32_t sys_ana_ll_get_ana_reg13_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_ANA_REG_BASE + (0x4d << 2));
}

static inline void sys_ana_ll_set_ana_reg13_rtcsel(uint32_t v) {
	sys_ana_set_ana_reg_bit((SOC_SYS_ANA_REG_BASE + (0x4d << 2)), 0, 0xffffffff, v);
}

static inline uint32_t sys_ana_ll_get_ana_reg13_rtcsel(void) {
	sys_ana_ana_reg13_t *r = (sys_ana_ana_reg13_t*)(SOC_SYS_ANA_REG_BASE + (0x4d << 2));
	return r->rtcsel;
}
#ifdef __cplusplus
}
#endif
