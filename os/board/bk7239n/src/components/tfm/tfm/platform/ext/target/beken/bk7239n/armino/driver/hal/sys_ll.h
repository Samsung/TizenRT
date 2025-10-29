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
#include "system_hw.h"

#if CONFIG_TFM_SYS_LL_NSC
#include "tfm_sys_ll_nsc.h"
#else

#ifdef __cplusplus
extern "C" {
#endif

#define SYS_LL_REG_BASE   SOC_SYS_REG_BASE

//This way of setting ana_reg_bit value is only for sys_ctrl, other driver please implement by yourself!!

#if CONFIG_ANA_REG_WRITE_POLL_REG_B
#define SYS_ANALOG_REG_SPI_STATE_REG (SYS_CPU_ANASPI_FREQ_ADDR)
#define SYS_ANALOG_REG_SPI_STATE_POS(idx) (idx + 8)
#else
#define SYS_ANALOG_REG_SPI_STATE_REG (SOC_SYS_REG_BASE + (0x3a << 2))
#define SYS_ANALOG_REG_SPI_STATE_POS(idx) (idx)
#endif

#define SYS_ANA_REG0_ADDR			 (SOC_SYS_REG_BASE + (0x40 << 2))
#define GET_SYS_ANALOG_REG_IDX(addr) ((addr - SYS_ANA_REG0_ADDR) >> 2)

static inline uint32_t sys_ll_get_analog_reg_value(uint32_t addr)
{
	return REG_READ(addr);
}

static inline void sys_ll_set_analog_reg_value(uint32_t addr, uint32_t value)
{
	uint32_t idx;
	idx = GET_SYS_ANALOG_REG_IDX(addr);

	REG_WRITE(addr, value);

	while(REG_READ(SYS_ANALOG_REG_SPI_STATE_REG) & (1 << SYS_ANALOG_REG_SPI_STATE_POS(idx)));
}

static inline void sys_set_ana_reg_bit(uint32_t reg_addr, uint32_t pos, uint32_t mask, uint32_t value)
{
	uint32_t reg_value;
	reg_value = *(volatile uint32_t *)(reg_addr);
	reg_value &= ~(mask << pos);
	reg_value |= ((value & mask) <<pos);
	sys_ll_set_analog_reg_value(reg_addr, reg_value);
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_uart0(uint32_t v)
{
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	r->cksel_uart0 = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_uart0(void) 
{
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	return r->cksel_uart0;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_uart1(uint32_t v)
{
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	r->cksel_uart1 = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_uart1(void) 
{
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	return r->cksel_uart1;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_uart2(uint32_t v)
{
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	r->cksel_uart2 = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_uart2(void) 
{
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	return r->cksel_uart2;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_uart3(uint32_t v)
{
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	r->cksel_uart3 = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_uart3(void) 
{
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	return r->cksel_uart3;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_i2c0(uint32_t v)
{
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	r->cksel_i2c0 = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_i2c0(void) 
{
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	return r->cksel_i2c0;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_i2c1(uint32_t v)
{
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	r->cksel_i2c1 = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_i2c1(void) 
{
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	return r->cksel_i2c1;
}

//reg device_id:
static inline void sys_ll_set_device_id_value(uint32_t v) {
	system_device_id_t *r = (system_device_id_t*)(SOC_SYSTEM_REG_BASE + (0x0 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_device_id_value(void) {
	system_device_id_t *r = (system_device_id_t*)(SOC_SYSTEM_REG_BASE + (0x0 << 2));
	return r->v;
}

static inline uint32_t sys_ll_get_device_id_deviceid(void) {
	system_device_id_t *r = (system_device_id_t*)(SOC_SYSTEM_REG_BASE + (0x0 << 2));
	return r->deviceid;
}

//reg version_id:

static inline void sys_ll_set_version_id_value(uint32_t v) {
	system_version_id_t *r = (system_version_id_t*)(SOC_SYSTEM_REG_BASE + (0x1 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_version_id_value(void) {
	system_version_id_t *r = (system_version_id_t*)(SOC_SYSTEM_REG_BASE + (0x1 << 2));
	return r->v;
}

static inline uint32_t sys_ll_get_version_id_versionid(void) {
	system_version_id_t *r = (system_version_id_t*)(SOC_SYSTEM_REG_BASE + (0x1 << 2));
	return r->versionid;
}

//reg cpu_storage_connect_op_select:

static inline void sys_ll_set_cpu_storage_connect_op_select_value(uint32_t v) {
	system_cpu_storage_connect_op_select_t *r = (system_cpu_storage_connect_op_select_t*)(SOC_SYSTEM_REG_BASE + (0x2 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu_storage_connect_op_select_value(void) {
	system_cpu_storage_connect_op_select_t *r = (system_cpu_storage_connect_op_select_t*)(SOC_SYSTEM_REG_BASE + (0x2 << 2));
	return r->v;
}

static inline void sys_ll_set_cpu_storage_connect_op_select_boot_mode(uint32_t v) {
	system_cpu_storage_connect_op_select_t *r = (system_cpu_storage_connect_op_select_t*)(SOC_SYSTEM_REG_BASE + (0x2 << 2));
	r->boot_mode = v;
}

static inline uint32_t sys_ll_get_cpu_storage_connect_op_select_boot_mode(void) {
	system_cpu_storage_connect_op_select_t *r = (system_cpu_storage_connect_op_select_t*)(SOC_SYSTEM_REG_BASE + (0x2 << 2));
	return r->boot_mode;
}

static inline void sys_ll_set_cpu_storage_connect_op_select_reserved0(uint32_t v) {
	system_cpu_storage_connect_op_select_t *r = (system_cpu_storage_connect_op_select_t*)(SOC_SYSTEM_REG_BASE + (0x2 << 2));
	r->reserved0 = v;
}

static inline uint32_t sys_ll_get_cpu_storage_connect_op_select_reserved0(void) {
	system_cpu_storage_connect_op_select_t *r = (system_cpu_storage_connect_op_select_t*)(SOC_SYSTEM_REG_BASE + (0x2 << 2));
	return r->reserved0;
}

static inline void sys_ll_set_cpu_storage_connect_op_select_rf_switch_manual_en(uint32_t v) {
	system_cpu_storage_connect_op_select_t *r = (system_cpu_storage_connect_op_select_t*)(SOC_SYSTEM_REG_BASE + (0x2 << 2));
	r->rf_switch_manual_en = v;
}

static inline uint32_t sys_ll_get_cpu_storage_connect_op_select_rf_switch_manual_en(void) {
	system_cpu_storage_connect_op_select_t *r = (system_cpu_storage_connect_op_select_t*)(SOC_SYSTEM_REG_BASE + (0x2 << 2));
	return r->rf_switch_manual_en;
}

static inline void sys_ll_set_cpu_storage_connect_op_select_rf_source(uint32_t v) {
	system_cpu_storage_connect_op_select_t *r = (system_cpu_storage_connect_op_select_t*)(SOC_SYSTEM_REG_BASE + (0x2 << 2));
	r->rf_source = v;
}

static inline uint32_t sys_ll_get_cpu_storage_connect_op_select_rf_source(void) {
	system_cpu_storage_connect_op_select_t *r = (system_cpu_storage_connect_op_select_t*)(SOC_SYSTEM_REG_BASE + (0x2 << 2));
	return r->rf_source;
}

static inline void sys_ll_set_cpu_storage_connect_op_select_reserved1(uint32_t v) {
	system_cpu_storage_connect_op_select_t *r = (system_cpu_storage_connect_op_select_t*)(SOC_SYSTEM_REG_BASE + (0x2 << 2));
	r->reserved1 = v;
}

static inline uint32_t sys_ll_get_cpu_storage_connect_op_select_reserved1(void) {
	system_cpu_storage_connect_op_select_t *r = (system_cpu_storage_connect_op_select_t*)(SOC_SYSTEM_REG_BASE + (0x2 << 2));
	return r->reserved1;
}

static inline void sys_ll_set_cpu_storage_connect_op_select_flash_sel(uint32_t v) {
	system_cpu_storage_connect_op_select_t *r = (system_cpu_storage_connect_op_select_t*)(SOC_SYSTEM_REG_BASE + (0x2 << 2));
	r->flash_sel = v;
}

static inline uint32_t sys_ll_get_cpu_storage_connect_op_select_flash_sel(void) {
	system_cpu_storage_connect_op_select_t *r = (system_cpu_storage_connect_op_select_t*)(SOC_SYSTEM_REG_BASE + (0x2 << 2));
	return r->flash_sel;
}

static inline void sys_ll_set_cpu_storage_connect_op_select_fem_bps_txen(uint32_t v) {
	system_cpu_storage_connect_op_select_t *r = (system_cpu_storage_connect_op_select_t*)(SOC_SYSTEM_REG_BASE + (0x2 << 2));
	r->fem_bps_txen = v;
}

static inline uint32_t sys_ll_get_cpu_storage_connect_op_select_fem_bps_txen(void) {
	system_cpu_storage_connect_op_select_t *r = (system_cpu_storage_connect_op_select_t*)(SOC_SYSTEM_REG_BASE + (0x2 << 2));
	return r->fem_bps_txen;
}

static inline void sys_ll_set_cpu_storage_connect_op_select_reserved2(uint32_t v) {
	system_cpu_storage_connect_op_select_t *r = (system_cpu_storage_connect_op_select_t*)(SOC_SYSTEM_REG_BASE + (0x2 << 2));
	r->reserved2 = v;
}

static inline uint32_t sys_ll_get_cpu_storage_connect_op_select_reserved2(void) {
	system_cpu_storage_connect_op_select_t *r = (system_cpu_storage_connect_op_select_t*)(SOC_SYSTEM_REG_BASE + (0x2 << 2));
	return r->reserved2;
}

//reg cpu_current_run_status:

static inline void sys_ll_set_cpu_current_run_status_value(uint32_t v) {
	system_cpu_current_run_status_t *r = (system_cpu_current_run_status_t*)(SOC_SYSTEM_REG_BASE + (0x3 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu_current_run_status_value(void) {
	system_cpu_current_run_status_t *r = (system_cpu_current_run_status_t*)(SOC_SYSTEM_REG_BASE + (0x3 << 2));
	return r->v;
}

static inline uint32_t sys_ll_get_cpu_current_run_status_core0_halted(void) {
	system_cpu_current_run_status_t *r = (system_cpu_current_run_status_t*)(SOC_SYSTEM_REG_BASE + (0x3 << 2));
	return r->core0_halted;
}

static inline void sys_ll_set_cpu_current_run_status_reserved0(uint32_t v) {
	system_cpu_current_run_status_t *r = (system_cpu_current_run_status_t*)(SOC_SYSTEM_REG_BASE + (0x3 << 2));
	r->reserved0 = v;
}

static inline uint32_t sys_ll_get_cpu_current_run_status_reserved0(void) {
	system_cpu_current_run_status_t *r = (system_cpu_current_run_status_t*)(SOC_SYSTEM_REG_BASE + (0x3 << 2));
	return r->reserved0;
}

static inline uint32_t sys_ll_get_cpu_current_run_status_cpu0_sw_reset(void) {
	system_cpu_current_run_status_t *r = (system_cpu_current_run_status_t*)(SOC_SYSTEM_REG_BASE + (0x3 << 2));
	return r->cpu0_sw_reset;
}

static inline void sys_ll_set_cpu_current_run_status_reserved1(uint32_t v) {
	system_cpu_current_run_status_t *r = (system_cpu_current_run_status_t*)(SOC_SYSTEM_REG_BASE + (0x3 << 2));
	r->reserved1 = v;
}

static inline uint32_t sys_ll_get_cpu_current_run_status_reserved1(void) {
	system_cpu_current_run_status_t *r = (system_cpu_current_run_status_t*)(SOC_SYSTEM_REG_BASE + (0x3 << 2));
	return r->reserved1;
}

static inline uint32_t sys_ll_get_cpu_current_run_status_cpu0_pwr_dw_state(void) {
	system_cpu_current_run_status_t *r = (system_cpu_current_run_status_t*)(SOC_SYSTEM_REG_BASE + (0x3 << 2));
	return r->cpu0_pwr_dw_state;
}

static inline void sys_ll_set_cpu_current_run_status_reserved2(uint32_t v) {
	system_cpu_current_run_status_t *r = (system_cpu_current_run_status_t*)(SOC_SYSTEM_REG_BASE + (0x3 << 2));
	r->reserved2 = v;
}

static inline uint32_t sys_ll_get_cpu_current_run_status_reserved2(void) {
	system_cpu_current_run_status_t *r = (system_cpu_current_run_status_t*)(SOC_SYSTEM_REG_BASE + (0x3 << 2));
	return r->reserved2;
}

//reg cpu0_int_halt_clk_op:

static inline void sys_ll_set_cpu0_int_halt_clk_op_value(uint32_t v) {
	system_cpu0_int_halt_clk_op_t *r = (system_cpu0_int_halt_clk_op_t*)(SOC_SYSTEM_REG_BASE + (0x4 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu0_int_halt_clk_op_value(void) {
	system_cpu0_int_halt_clk_op_t *r = (system_cpu0_int_halt_clk_op_t*)(SOC_SYSTEM_REG_BASE + (0x4 << 2));
	return r->v;
}

static inline void sys_ll_set_cpu0_int_halt_clk_op_cpu0_sw_rst(uint32_t v) {
	system_cpu0_int_halt_clk_op_t *r = (system_cpu0_int_halt_clk_op_t*)(SOC_SYSTEM_REG_BASE + (0x4 << 2));
	r->cpu0_sw_rst = v;
}

static inline uint32_t sys_ll_get_cpu0_int_halt_clk_op_cpu0_sw_rst(void) {
	system_cpu0_int_halt_clk_op_t *r = (system_cpu0_int_halt_clk_op_t*)(SOC_SYSTEM_REG_BASE + (0x4 << 2));
	return r->cpu0_sw_rst;
}

static inline void sys_ll_set_cpu0_int_halt_clk_op_cpu0_pwr_dw(uint32_t v) {
	system_cpu0_int_halt_clk_op_t *r = (system_cpu0_int_halt_clk_op_t*)(SOC_SYSTEM_REG_BASE + (0x4 << 2));
	r->cpu0_pwr_dw = v;
}

static inline uint32_t sys_ll_get_cpu0_int_halt_clk_op_cpu0_pwr_dw(void) {
	system_cpu0_int_halt_clk_op_t *r = (system_cpu0_int_halt_clk_op_t*)(SOC_SYSTEM_REG_BASE + (0x4 << 2));
	return r->cpu0_pwr_dw;
}

static inline void sys_ll_set_cpu0_int_halt_clk_op_cpu0_int_mask(uint32_t v) {
	system_cpu0_int_halt_clk_op_t *r = (system_cpu0_int_halt_clk_op_t*)(SOC_SYSTEM_REG_BASE + (0x4 << 2));
	r->cpu0_int_mask = v;
}

static inline uint32_t sys_ll_get_cpu0_int_halt_clk_op_cpu0_int_mask(void) {
	system_cpu0_int_halt_clk_op_t *r = (system_cpu0_int_halt_clk_op_t*)(SOC_SYSTEM_REG_BASE + (0x4 << 2));
	return r->cpu0_int_mask;
}

static inline void sys_ll_set_cpu0_int_halt_clk_op_cpu0_halt(uint32_t v) {
	system_cpu0_int_halt_clk_op_t *r = (system_cpu0_int_halt_clk_op_t*)(SOC_SYSTEM_REG_BASE + (0x4 << 2));
	r->cpu0_halt = v;
}

static inline uint32_t sys_ll_get_cpu0_int_halt_clk_op_cpu0_halt(void) {
	system_cpu0_int_halt_clk_op_t *r = (system_cpu0_int_halt_clk_op_t*)(SOC_SYSTEM_REG_BASE + (0x4 << 2));
	return r->cpu0_halt;
}

static inline void sys_ll_set_cpu0_int_halt_clk_op_cpu0_speed(uint32_t v) {
	system_cpu0_int_halt_clk_op_t *r = (system_cpu0_int_halt_clk_op_t*)(SOC_SYSTEM_REG_BASE + (0x4 << 2));
	r->cpu0_speed = v;
}

static inline uint32_t sys_ll_get_cpu0_int_halt_clk_op_cpu0_speed(void) {
	system_cpu0_int_halt_clk_op_t *r = (system_cpu0_int_halt_clk_op_t*)(SOC_SYSTEM_REG_BASE + (0x4 << 2));
	return r->cpu0_speed;
}

static inline void sys_ll_set_cpu0_int_halt_clk_op_cpu0_rxevt_sel(uint32_t v) {
	system_cpu0_int_halt_clk_op_t *r = (system_cpu0_int_halt_clk_op_t*)(SOC_SYSTEM_REG_BASE + (0x4 << 2));
	r->cpu0_rxevt_sel = v;
}

static inline uint32_t sys_ll_get_cpu0_int_halt_clk_op_cpu0_rxevt_sel(void) {
	system_cpu0_int_halt_clk_op_t *r = (system_cpu0_int_halt_clk_op_t*)(SOC_SYSTEM_REG_BASE + (0x4 << 2));
	return r->cpu0_rxevt_sel;
}

static inline void sys_ll_set_cpu0_int_halt_clk_op_reserved_6_7(uint32_t v) {
	system_cpu0_int_halt_clk_op_t *r = (system_cpu0_int_halt_clk_op_t*)(SOC_SYSTEM_REG_BASE + (0x4 << 2));
	r->reserved_6_7 = v;
}

static inline uint32_t sys_ll_get_cpu0_int_halt_clk_op_reserved_6_7(void) {
	system_cpu0_int_halt_clk_op_t *r = (system_cpu0_int_halt_clk_op_t*)(SOC_SYSTEM_REG_BASE + (0x4 << 2));
	return r->reserved_6_7;
}

static inline uint32_t sys_ll_get_cpu0_int_halt_clk_op_cpu0_offset(void) {
	system_cpu0_int_halt_clk_op_t *r = (system_cpu0_int_halt_clk_op_t*)(SOC_SYSTEM_REG_BASE + (0x4 << 2));
	return r->cpu0_offset;
}

//reg cpu_clk_div_mode1:

static inline void sys_ll_set_cpu_clk_div_mode1_value(uint32_t v) {
	system_cpu_clk_div_mode1_t *r = (system_cpu_clk_div_mode1_t*)(SOC_SYSTEM_REG_BASE + (0x8 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode1_value(void) {
	system_cpu_clk_div_mode1_t *r = (system_cpu_clk_div_mode1_t*)(SOC_SYSTEM_REG_BASE + (0x8 << 2));
	return r->v;
}

static inline void sys_ll_set_cpu_clk_div_mode1_clkdiv_core(uint32_t v) {
	system_cpu_clk_div_mode1_t *r = (system_cpu_clk_div_mode1_t*)(SOC_SYSTEM_REG_BASE + (0x8 << 2));
	r->clkdiv_core = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_core(void) {
	system_cpu_clk_div_mode1_t *r = (system_cpu_clk_div_mode1_t*)(SOC_SYSTEM_REG_BASE + (0x8 << 2));
	return r->clkdiv_core;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_core(uint32_t v) {
	system_cpu_clk_div_mode1_t *r = (system_cpu_clk_div_mode1_t*)(SOC_SYSTEM_REG_BASE + (0x8 << 2));
	r->cksel_core = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_core(void) {
	system_cpu_clk_div_mode1_t *r = (system_cpu_clk_div_mode1_t*)(SOC_SYSTEM_REG_BASE + (0x8 << 2));
	return r->cksel_core;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_pwm0(uint32_t v) {
	system_cpu_clk_div_mode1_t *r = (system_cpu_clk_div_mode1_t*)(SOC_SYSTEM_REG_BASE + (0x8 << 2));
	r->cksel_pwm0 = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_pwm0(void) {
	system_cpu_clk_div_mode1_t *r = (system_cpu_clk_div_mode1_t*)(SOC_SYSTEM_REG_BASE + (0x8 << 2));
	return r->cksel_pwm0;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_tim0(uint32_t v) {
	system_cpu_clk_div_mode1_t *r = (system_cpu_clk_div_mode1_t*)(SOC_SYSTEM_REG_BASE + (0x8 << 2));
	r->cksel_tim0 = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_tim0(void) {
	system_cpu_clk_div_mode1_t *r = (system_cpu_clk_div_mode1_t*)(SOC_SYSTEM_REG_BASE + (0x8 << 2));
	return r->cksel_tim0;
}

static inline void sys_ll_set_cpu_clk_div_mode1_cksel_tim1(uint32_t v) {
	system_cpu_clk_div_mode1_t *r = (system_cpu_clk_div_mode1_t*)(SOC_SYSTEM_REG_BASE + (0x8 << 2));
	r->cksel_tim1 = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_tim1(void) {
	system_cpu_clk_div_mode1_t *r = (system_cpu_clk_div_mode1_t*)(SOC_SYSTEM_REG_BASE + (0x8 << 2));
	return r->cksel_tim1;
}

//reg cpu_clk_div_mode2:

static inline void sys_ll_set_cpu_clk_div_mode2_value(uint32_t v) {
	system_cpu_clk_div_mode2_t *r = (system_cpu_clk_div_mode2_t*)(SOC_SYSTEM_REG_BASE + (0x9 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode2_value(void) {
	system_cpu_clk_div_mode2_t *r = (system_cpu_clk_div_mode2_t*)(SOC_SYSTEM_REG_BASE + (0x9 << 2));
	return r->v;
}

static inline void sys_ll_set_cpu_clk_div_mode2_reserved_0_3(uint32_t v) {
	system_cpu_clk_div_mode2_t *r = (system_cpu_clk_div_mode2_t*)(SOC_SYSTEM_REG_BASE + (0x9 << 2));
	r->reserved_0_3 = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode2_reserved_0_3(void) {
	system_cpu_clk_div_mode2_t *r = (system_cpu_clk_div_mode2_t*)(SOC_SYSTEM_REG_BASE + (0x9 << 2));
	return r->reserved_0_3;
}

static inline void sys_ll_set_cpu_clk_div_mode2_ckdiv_psram(uint32_t v) {
	system_cpu_clk_div_mode2_t *r = (system_cpu_clk_div_mode2_t*)(SOC_SYSTEM_REG_BASE + (0x9 << 2));
	r->ckdiv_psram = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode2_ckdiv_psram(void) {
	system_cpu_clk_div_mode2_t *r = (system_cpu_clk_div_mode2_t*)(SOC_SYSTEM_REG_BASE + (0x9 << 2));
	return r->ckdiv_psram;
}

static inline void sys_ll_set_cpu_clk_div_mode2_cksel_psram(uint32_t v) {
	system_cpu_clk_div_mode2_t *r = (system_cpu_clk_div_mode2_t*)(SOC_SYSTEM_REG_BASE + (0x9 << 2));
	r->cksel_psram = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode2_cksel_psram(void) {
	system_cpu_clk_div_mode2_t *r = (system_cpu_clk_div_mode2_t*)(SOC_SYSTEM_REG_BASE + (0x9 << 2));
	return r->cksel_psram;
}

static inline void sys_ll_set_cpu_clk_div_mode2_ckdiv_sdio(uint32_t v) {
	system_cpu_clk_div_mode2_t *r = (system_cpu_clk_div_mode2_t*)(SOC_SYSTEM_REG_BASE + (0x9 << 2));
	r->ckdiv_sdio = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode2_ckdiv_sdio(void) {
	system_cpu_clk_div_mode2_t *r = (system_cpu_clk_div_mode2_t*)(SOC_SYSTEM_REG_BASE + (0x9 << 2));
	return r->ckdiv_sdio;
}

static inline void sys_ll_set_cpu_clk_div_mode2_cksel_sdio(uint32_t v) {
	system_cpu_clk_div_mode2_t *r = (system_cpu_clk_div_mode2_t*)(SOC_SYSTEM_REG_BASE + (0x9 << 2));
	r->cksel_sdio = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode2_cksel_sdio(void) {
	system_cpu_clk_div_mode2_t *r = (system_cpu_clk_div_mode2_t*)(SOC_SYSTEM_REG_BASE + (0x9 << 2));
	return r->cksel_sdio;
}

static inline void sys_ll_set_cpu_clk_div_mode2_ckdiv_auxs(uint32_t v) {
	system_cpu_clk_div_mode2_t *r = (system_cpu_clk_div_mode2_t*)(SOC_SYSTEM_REG_BASE + (0x9 << 2));
	r->ckdiv_auxs = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode2_ckdiv_auxs(void) {
	system_cpu_clk_div_mode2_t *r = (system_cpu_clk_div_mode2_t*)(SOC_SYSTEM_REG_BASE + (0x9 << 2));
	return r->ckdiv_auxs;
}

static inline void sys_ll_set_cpu_clk_div_mode2_cksel_auxs(uint32_t v) {
	system_cpu_clk_div_mode2_t *r = (system_cpu_clk_div_mode2_t*)(SOC_SYSTEM_REG_BASE + (0x9 << 2));
	r->cksel_auxs = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode2_cksel_auxs(void) {
	system_cpu_clk_div_mode2_t *r = (system_cpu_clk_div_mode2_t*)(SOC_SYSTEM_REG_BASE + (0x9 << 2));
	return r->cksel_auxs;
}

static inline void sys_ll_set_cpu_clk_div_mode2_cksel_flash(uint32_t v) {
	system_cpu_clk_div_mode2_t *r = (system_cpu_clk_div_mode2_t*)(SOC_SYSTEM_REG_BASE + (0x9 << 2));
	r->cksel_flash = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode2_cksel_flash(void) {
	system_cpu_clk_div_mode2_t *r = (system_cpu_clk_div_mode2_t*)(SOC_SYSTEM_REG_BASE + (0x9 << 2));
	return r->cksel_flash;
}

static inline void sys_ll_set_cpu_clk_div_mode2_ckdiv_flash(uint32_t v) {
	system_cpu_clk_div_mode2_t *r = (system_cpu_clk_div_mode2_t*)(SOC_SYSTEM_REG_BASE + (0x9 << 2));
	r->ckdiv_flash = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode2_ckdiv_flash(void) {
	system_cpu_clk_div_mode2_t *r = (system_cpu_clk_div_mode2_t*)(SOC_SYSTEM_REG_BASE + (0x9 << 2));
	return r->ckdiv_flash;
}

static inline void sys_ll_set_cpu_clk_div_mode2_clkdiv_auxs(uint32_t v) {
	system_cpu_clk_div_mode2_t *r = (system_cpu_clk_div_mode2_t*)(SOC_SYSTEM_REG_BASE + (0x9 << 2));
	r->clkdiv_auxs = v;
}

static inline uint32_t sys_ll_get_cpu_clk_div_mode2_clkdiv_auxs(void) {
	system_cpu_clk_div_mode2_t *r = (system_cpu_clk_div_mode2_t*)(SOC_SYSTEM_REG_BASE + (0x9 << 2));
	return r->clkdiv_auxs;
}

//reg cpu_26m_wdt_clk_div:

static inline void sys_ll_set_cpu_26m_wdt_clk_div_value(uint32_t v) {
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYSTEM_REG_BASE + (0xa << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu_26m_wdt_clk_div_value(void) {
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYSTEM_REG_BASE + (0xa << 2));
	return r->v;
}

static inline void sys_ll_set_cpu_26m_wdt_clk_div_ckdiv_xtal(uint32_t v) {
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYSTEM_REG_BASE + (0xa << 2));
	r->ckdiv_xtal = v;
}

static inline uint32_t sys_ll_get_cpu_26m_wdt_clk_div_ckdiv_xtal(void) {
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYSTEM_REG_BASE + (0xa << 2));
	return r->ckdiv_xtal;
}

static inline void sys_ll_set_cpu_26m_wdt_clk_div_ckdiv_wdt(uint32_t v) {
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYSTEM_REG_BASE + (0xa << 2));
	r->ckdiv_wdt = v;
}

static inline uint32_t sys_ll_get_cpu_26m_wdt_clk_div_ckdiv_wdt(void) {
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYSTEM_REG_BASE + (0xa << 2));
	return r->ckdiv_wdt;
}

static inline void sys_ll_set_cpu_26m_wdt_clk_div_clksel_spi0(uint32_t v) {
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYSTEM_REG_BASE + (0xa << 2));
	r->clksel_spi0 = v;
}

static inline uint32_t sys_ll_get_cpu_26m_wdt_clk_div_clksel_spi0(void) {
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYSTEM_REG_BASE + (0xa << 2));
	return r->clksel_spi0;
}

static inline void sys_ll_set_cpu_26m_wdt_clk_div_clksel_spi1(uint32_t v) {
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYSTEM_REG_BASE + (0xa << 2));
	r->clksel_spi1 = v;
}

static inline uint32_t sys_ll_get_cpu_26m_wdt_clk_div_clksel_spi1(void) {
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYSTEM_REG_BASE + (0xa << 2));
	return r->clksel_spi1;
}

static inline void sys_ll_set_cpu_26m_wdt_clk_div_cksel_qspi1(uint32_t v) {
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYSTEM_REG_BASE + (0xa << 2));
	r->cksel_qspi1 = v;
}

static inline uint32_t sys_ll_get_cpu_26m_wdt_clk_div_cksel_qspi1(void) {
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYSTEM_REG_BASE + (0xa << 2));
	return r->cksel_qspi1;
}

static inline void sys_ll_set_cpu_26m_wdt_clk_div_cksel_i2c0(uint32_t v) {
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYSTEM_REG_BASE + (0xa << 2));
	r->cksel_i2c0 = v;
}

static inline uint32_t sys_ll_get_cpu_26m_wdt_clk_div_cksel_i2c0(void) {
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYSTEM_REG_BASE + (0xa << 2));
	return r->cksel_i2c0;
}

static inline void sys_ll_set_cpu_26m_wdt_clk_div_cksel_i2c1(uint32_t v) {
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYSTEM_REG_BASE + (0xa << 2));
	r->cksel_i2c1 = v;
}

static inline uint32_t sys_ll_get_cpu_26m_wdt_clk_div_cksel_i2c1(void) {
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYSTEM_REG_BASE + (0xa << 2));
	return r->cksel_i2c1;
}

static inline void sys_ll_set_cpu_26m_wdt_clk_div_cksel_uart0(uint32_t v) {
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYSTEM_REG_BASE + (0xa << 2));
	r->cksel_uart0 = v;
}

static inline uint32_t sys_ll_get_cpu_26m_wdt_clk_div_cksel_uart0(void) {
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYSTEM_REG_BASE + (0xa << 2));
	return r->cksel_uart0;
}

static inline void sys_ll_set_cpu_26m_wdt_clk_div_cksel_uart1(uint32_t v) {
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYSTEM_REG_BASE + (0xa << 2));
	r->cksel_uart1 = v;
}

static inline uint32_t sys_ll_get_cpu_26m_wdt_clk_div_cksel_uart1(void) {
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYSTEM_REG_BASE + (0xa << 2));
	return r->cksel_uart1;
}

static inline void sys_ll_set_cpu_26m_wdt_clk_div_cksel_uart2(uint32_t v) {
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYSTEM_REG_BASE + (0xa << 2));
	r->cksel_uart2 = v;
}

static inline uint32_t sys_ll_get_cpu_26m_wdt_clk_div_cksel_uart2(void) {
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYSTEM_REG_BASE + (0xa << 2));
	return r->cksel_uart2;
}

static inline void sys_ll_set_cpu_26m_wdt_clk_div_cksel_uart3(uint32_t v) {
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYSTEM_REG_BASE + (0xa << 2));
	r->cksel_uart3 = v;
}

static inline uint32_t sys_ll_get_cpu_26m_wdt_clk_div_cksel_uart3(void) {
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYSTEM_REG_BASE + (0xa << 2));
	return r->cksel_uart3;
}

static inline void sys_ll_set_cpu_26m_wdt_clk_div_ckdiv_qspi1(uint32_t v) {
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYSTEM_REG_BASE + (0xa << 2));
	r->ckdiv_qspi1 = v;
}

static inline uint32_t sys_ll_get_cpu_26m_wdt_clk_div_ckdiv_qspi1(void) {
	system_cpu_26m_wdt_clk_div_t *r = (system_cpu_26m_wdt_clk_div_t*)(SOC_SYSTEM_REG_BASE + (0xa << 2));
	return r->ckdiv_qspi1;
}

//reg cpu_anaspi_freq:

static inline void sys_ll_set_cpu_anaspi_freq_value(uint32_t v) {
	system_cpu_anaspi_freq_t *r = (system_cpu_anaspi_freq_t*)(SOC_SYSTEM_REG_BASE + (0xb << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu_anaspi_freq_value(void) {
	system_cpu_anaspi_freq_t *r = (system_cpu_anaspi_freq_t*)(SOC_SYSTEM_REG_BASE + (0xb << 2));
	return r->v;
}

static inline void sys_ll_set_cpu_anaspi_freq_anaspi_freq(uint32_t v) {
	system_cpu_anaspi_freq_t *r = (system_cpu_anaspi_freq_t*)(SOC_SYSTEM_REG_BASE + (0xb << 2));
	r->anaspi_freq = v;
}

static inline uint32_t sys_ll_get_cpu_anaspi_freq_anaspi_freq(void) {
	system_cpu_anaspi_freq_t *r = (system_cpu_anaspi_freq_t*)(SOC_SYSTEM_REG_BASE + (0xb << 2));
	return r->anaspi_freq;
}

static inline void sys_ll_set_cpu_anaspi_freq_reserved_6_31(uint32_t v) {
	system_cpu_anaspi_freq_t *r = (system_cpu_anaspi_freq_t*)(SOC_SYSTEM_REG_BASE + (0xb << 2));
	r->reserved_6_31 = v;
}

static inline uint32_t sys_ll_get_cpu_anaspi_freq_reserved_6_31(void) {
	system_cpu_anaspi_freq_t *r = (system_cpu_anaspi_freq_t*)(SOC_SYSTEM_REG_BASE + (0xb << 2));
	return r->reserved_6_31;
}

//reg cpu_device_clk_enable:

static inline void sys_ll_set_cpu_device_clk_enable_value(uint32_t v) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_value(void) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	return r->v;
}

static inline void sys_ll_set_cpu_device_clk_enable_i2c0_cken(uint32_t v) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	r->i2c0_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_i2c0_cken(void) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	return r->i2c0_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_spi0_cken(uint32_t v) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	r->spi0_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_spi0_cken(void) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	return r->spi0_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_uart0_cken(uint32_t v) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	r->uart0_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_uart0_cken(void) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	return r->uart0_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_pwm0_cken(uint32_t v) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	r->pwm0_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_pwm0_cken(void) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	return r->pwm0_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_tim0_cken(uint32_t v) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	r->tim0_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_tim0_cken(void) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	return r->tim0_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_sadc_cken(uint32_t v) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	r->sadc_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_sadc_cken(void) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	return r->sadc_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_uart3_cken(uint32_t v) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	r->uart3_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_uart3_cken(void) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	return r->uart3_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_gpio_cken(uint32_t v) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	r->gpio_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_gpio_cken(void) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	return r->gpio_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_i2c1_cken(uint32_t v) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	r->i2c1_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_i2c1_cken(void) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	return r->i2c1_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_spi1_cken(uint32_t v) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	r->spi1_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_spi1_cken(void) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	return r->spi1_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_uart1_cken(uint32_t v) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	r->uart1_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_uart1_cken(void) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	return r->uart1_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_uart2_cken(uint32_t v) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	r->uart2_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_uart2_cken(void) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	return r->uart2_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_tim1_cken(uint32_t v) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	r->tim1_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_tim1_cken(void) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	return r->tim1_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_otp_cken(uint32_t v) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	r->otp_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_otp_cken(void) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	return r->otp_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_i2s_cken(uint32_t v) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	r->i2s_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_i2s_cken(void) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	return r->i2s_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_led_cken(uint32_t v) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	r->led_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_led_cken(void) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	return r->led_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_tamp_cken(uint32_t v) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	r->tamp_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_tamp_cken(void) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	return r->tamp_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_psram_cken(uint32_t v) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	r->psram_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_psram_cken(void) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	return r->psram_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_dplldiv_cken(uint32_t v) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	r->dplldiv_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_dplldiv_cken(void) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	return r->dplldiv_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_qspi1_cken(uint32_t v) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	r->qspi1_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_qspi1_cken(void) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	return r->qspi1_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_sdio_cken(uint32_t v) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	r->sdio_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_sdio_cken(void) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	return r->sdio_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_auxs_cken(uint32_t v) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	r->auxs_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_auxs_cken(void) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	return r->auxs_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_btdm_cken(uint32_t v) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	r->btdm_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_btdm_cken(void) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	return r->btdm_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_xvr_cken(uint32_t v) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	r->xvr_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_xvr_cken(void) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	return r->xvr_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_mac_cken(uint32_t v) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	r->mac_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_mac_cken(void) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	return r->mac_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_phy_cken(uint32_t v) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	r->phy_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_phy_cken(void) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	return r->phy_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_thread_cken(uint32_t v) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	r->thread_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_thread_cken(void) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	return r->thread_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_rf_cken(uint32_t v) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	r->rf_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_rf_cken(void) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	return r->rf_cken;
}

static inline void sys_ll_set_cpu_device_clk_enable_wdt_cken(uint32_t v) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	r->wdt_cken = v;
}

static inline uint32_t sys_ll_get_cpu_device_clk_enable_wdt_cken(void) {
	system_cpu_device_clk_enable_t *r = (system_cpu_device_clk_enable_t*)(SOC_SYSTEM_REG_BASE + (0xc << 2));
	return r->wdt_cken;
}

//reg cpu_device_mem_ctrl2:

static inline void sys_ll_set_cpu_device_mem_ctrl2_value(uint32_t v) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_value(void) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	return r->v;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_uart1_ds(uint32_t v) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	r->uart1_ds = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_uart1_ds(void) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	return r->uart1_ds;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_uart2_ds(uint32_t v) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	r->uart2_ds = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_uart2_ds(void) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	return r->uart2_ds;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_spi1_ds(uint32_t v) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	r->spi1_ds = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_spi1_ds(void) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	return r->spi1_ds;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_sdio_ds(uint32_t v) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	r->sdio_ds = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_sdio_ds(void) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	return r->sdio_ds;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_uart3_ds(uint32_t v) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	r->uart3_ds = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_uart3_ds(void) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	return r->uart3_ds;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_led_ds(uint32_t v) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	r->led_ds = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_led_ds(void) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	return r->led_ds;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_reserved_6_6(uint32_t v) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	r->reserved_6_6 = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_reserved_6_6(void) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	return r->reserved_6_6;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_qspi1_ds(uint32_t v) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	r->qspi1_ds = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_qspi1_ds(void) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	return r->qspi1_ds;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_pram_ds(uint32_t v) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	r->pram_ds = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_pram_ds(void) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	return r->pram_ds;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_reserved0(uint32_t v) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	r->reserved0 = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_reserved0(void) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	return r->reserved0;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_i2s0_ds(uint32_t v) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	r->i2s0_ds = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_i2s0_ds(void) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	return r->i2s0_ds;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_reserved_13_20(uint32_t v) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	r->reserved_13_20 = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_reserved_13_20(void) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	return r->reserved_13_20;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_mac_ds(uint32_t v) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	r->mac_ds = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_mac_ds(void) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	return r->mac_ds;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_phy_ds(uint32_t v) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	r->phy_ds = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_phy_ds(void) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	return r->phy_ds;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_xvr_ds(uint32_t v) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	r->xvr_ds = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_xvr_ds(void) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	return r->xvr_ds;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_la_ds(uint32_t v) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	r->la_ds = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_la_ds(void) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	return r->la_ds;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_flsh_ds(uint32_t v) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	r->flsh_ds = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_flsh_ds(void) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	return r->flsh_ds;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_uart_ds(uint32_t v) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	r->uart_ds = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_uart_ds(void) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	return r->uart_ds;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_spi0_ds(uint32_t v) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	r->spi0_ds = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_spi0_ds(void) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	return r->spi0_ds;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_enc_ds(uint32_t v) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	r->enc_ds = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_enc_ds(void) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	return r->enc_ds;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_dma0_ds(uint32_t v) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	r->dma0_ds = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_dma0_ds(void) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	return r->dma0_ds;
}

static inline void sys_ll_set_cpu_device_mem_ctrl2_reserved1(uint32_t v) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	r->reserved1 = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl2_reserved1(void) {
	system_cpu_device_mem_ctrl2_t *r = (system_cpu_device_mem_ctrl2_t*)(SOC_SYSTEM_REG_BASE + (0xf << 2));
	return r->reserved1;
}

//reg cpu_power_sleep_wakeup:

static inline void sys_ll_set_cpu_power_sleep_wakeup_value(uint32_t v) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_value(void) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	return r->v;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_mem1(uint32_t v) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	r->pwd_mem1 = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_mem1(void) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	return r->pwd_mem1;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_mem2(uint32_t v) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	r->pwd_mem2 = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_mem2(void) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	return r->pwd_mem2;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_mem3(uint32_t v) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	r->pwd_mem3 = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_mem3(void) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	return r->pwd_mem3;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_encp(uint32_t v) {
	return;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_encp(void) {
	return 0; // 7236n do not have encp power domain
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_bakp(uint32_t v) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	r->pwd_bakp = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_bakp(void) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	return r->pwd_bakp;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_btsp(uint32_t v) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	r->pwd_btsp = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_btsp(void) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	return r->pwd_btsp;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_wifp_mac(uint32_t v) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	r->pwd_wifp_mac = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_wifp_mac(void) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	return r->pwd_wifp_mac;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_wifp_phy(uint32_t v) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	r->pwd_wifp_phy = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_wifp_phy(void) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	return r->pwd_wifp_phy;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_mem0(uint32_t v) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	r->pwd_mem0 = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_mem0(void) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	return r->pwd_mem0;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_mem4(uint32_t v) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	r->pwd_mem4 = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_mem4(void) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	return r->pwd_mem4;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_pwd_ofdm(uint32_t v) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	r->pwd_ofdm = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_ofdm(void) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	return r->pwd_ofdm;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_rom_pgen(uint32_t v) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	r->rom_pgen = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_rom_pgen(void) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	return r->rom_pgen;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_sleep_en_need_flash_idle(uint32_t v) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	r->sleep_en_need_flash_idle = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_sleep_en_need_flash_idle(void) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	return r->sleep_en_need_flash_idle;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_sleep_en_need_cpu0_wfi(uint32_t v) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	r->sleep_en_need_cpu0_wfi = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_sleep_en_need_cpu0_wfi(void) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	return r->sleep_en_need_cpu0_wfi;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_sleep_en_global(uint32_t v) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	r->sleep_en_global = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_sleep_en_global(void) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	return r->sleep_en_global;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_sleep_bus_idle_bypass(uint32_t v) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	r->sleep_bus_idle_bypass = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_sleep_bus_idle_bypass(void) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	return r->sleep_bus_idle_bypass;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_bts_soft_wakeup_req(uint32_t v) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	r->bts_soft_wakeup_req = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_bts_soft_wakeup_req(void) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	return r->bts_soft_wakeup_req;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_rom_rd_disable(uint32_t v) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	r->rom_rd_disable = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_rom_rd_disable(void) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	return r->rom_rd_disable;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_otp_rd_disable(uint32_t v) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	r->otp_rd_disable = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_otp_rd_disable(void) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	return r->otp_rd_disable;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_tcm0_pgen(uint32_t v) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	r->tcm0_pgen = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_tcm0_pgen(void) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	return r->tcm0_pgen;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_cpu0_subpwdm_en(uint32_t v) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	r->cpu0_subpwdm_en = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_cpu0_subpwdm_en(void) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	return r->cpu0_subpwdm_en;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_te200_clkgat_bps(uint32_t v) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	r->te200_clkgat_bps = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_te200_clkgat_bps(void) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	return r->te200_clkgat_bps;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_share_mem_clkgating_disable(uint32_t v) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	r->share_mem_clkgating_disable = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_share_mem_clkgating_disable(void) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	return r->share_mem_clkgating_disable;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_cpu0_ticktimer_32k_enable(uint32_t v) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	r->cpu0_ticktimer_32k_enable = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_cpu0_ticktimer_32k_enable(void) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	return r->cpu0_ticktimer_32k_enable;
}

static inline void sys_ll_set_cpu_power_sleep_wakeup_reserved_30_30(uint32_t v) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	r->reserved_30_30 = v;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_reserved_30_30(void) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	return r->reserved_30_30;
}

static inline uint32_t sys_ll_get_cpu_power_sleep_wakeup_busmatrix_busy(void) {
	system_cpu_power_sleep_wakeup_t *r = (system_cpu_power_sleep_wakeup_t*)(SOC_SYSTEM_REG_BASE + (0x10 << 2));
	return r->busmatrix_busy;
}

//reg cpu0_lv_sleep_cfg:

static inline void sys_ll_set_cpu0_lv_sleep_cfg_value(uint32_t v) {
	system_cpu0_lv_sleep_cfg_t *r = (system_cpu0_lv_sleep_cfg_t*)(SOC_SYSTEM_REG_BASE + (0x11 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu0_lv_sleep_cfg_value(void) {
	system_cpu0_lv_sleep_cfg_t *r = (system_cpu0_lv_sleep_cfg_t*)(SOC_SYSTEM_REG_BASE + (0x11 << 2));
	return r->v;
}

static inline void sys_ll_set_cpu0_lv_sleep_cfg_cpu0_cache_ret_en(uint32_t v) {
	system_cpu0_lv_sleep_cfg_t *r = (system_cpu0_lv_sleep_cfg_t*)(SOC_SYSTEM_REG_BASE + (0x11 << 2));
	r->cpu0_cache_ret_en = v;
}

static inline uint32_t sys_ll_get_cpu0_lv_sleep_cfg_cpu0_cache_ret_en(void) {
	system_cpu0_lv_sleep_cfg_t *r = (system_cpu0_lv_sleep_cfg_t*)(SOC_SYSTEM_REG_BASE + (0x11 << 2));
	return r->cpu0_cache_ret_en;
}

static inline void sys_ll_set_cpu0_lv_sleep_cfg_cpu0_cache_sleeppwd_en(uint32_t v) {
	system_cpu0_lv_sleep_cfg_t *r = (system_cpu0_lv_sleep_cfg_t*)(SOC_SYSTEM_REG_BASE + (0x11 << 2));
	r->cpu0_cache_sleeppwd_en = v;
}

static inline uint32_t sys_ll_get_cpu0_lv_sleep_cfg_cpu0_cache_sleeppwd_en(void) {
	system_cpu0_lv_sleep_cfg_t *r = (system_cpu0_lv_sleep_cfg_t*)(SOC_SYSTEM_REG_BASE + (0x11 << 2));
	return r->cpu0_cache_sleeppwd_en;
}

static inline void sys_ll_set_cpu0_lv_sleep_cfg_reserved_2_31(uint32_t v) {
	system_cpu0_lv_sleep_cfg_t *r = (system_cpu0_lv_sleep_cfg_t*)(SOC_SYSTEM_REG_BASE + (0x11 << 2));
	r->reserved_2_31 = v;
}

static inline uint32_t sys_ll_get_cpu0_lv_sleep_cfg_reserved_2_31(void) {
	system_cpu0_lv_sleep_cfg_t *r = (system_cpu0_lv_sleep_cfg_t*)(SOC_SYSTEM_REG_BASE + (0x11 << 2));
	return r->reserved_2_31;
}

//reg cpu_device_mem_ctrl3:

static inline void sys_ll_set_cpu_device_mem_ctrl3_value(uint32_t v) {
	system_cpu_device_mem_ctrl3_t *r = (system_cpu_device_mem_ctrl3_t*)(SOC_SYSTEM_REG_BASE + (0x12 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_value(void) {
	system_cpu_device_mem_ctrl3_t *r = (system_cpu_device_mem_ctrl3_t*)(SOC_SYSTEM_REG_BASE + (0x12 << 2));
	return r->v;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_ram0_ds(uint32_t v) {
	system_cpu_device_mem_ctrl3_t *r = (system_cpu_device_mem_ctrl3_t*)(SOC_SYSTEM_REG_BASE + (0x12 << 2));
	r->ram0_ds = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram0_ds(void) {
	system_cpu_device_mem_ctrl3_t *r = (system_cpu_device_mem_ctrl3_t*)(SOC_SYSTEM_REG_BASE + (0x12 << 2));
	return r->ram0_ds;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_ram1_ds(uint32_t v) {
	system_cpu_device_mem_ctrl3_t *r = (system_cpu_device_mem_ctrl3_t*)(SOC_SYSTEM_REG_BASE + (0x12 << 2));
	r->ram1_ds = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram1_ds(void) {
	system_cpu_device_mem_ctrl3_t *r = (system_cpu_device_mem_ctrl3_t*)(SOC_SYSTEM_REG_BASE + (0x12 << 2));
	return r->ram1_ds;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_ram2_ds(uint32_t v) {
	system_cpu_device_mem_ctrl3_t *r = (system_cpu_device_mem_ctrl3_t*)(SOC_SYSTEM_REG_BASE + (0x12 << 2));
	r->ram2_ds = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram2_ds(void) {
	system_cpu_device_mem_ctrl3_t *r = (system_cpu_device_mem_ctrl3_t*)(SOC_SYSTEM_REG_BASE + (0x12 << 2));
	return r->ram2_ds;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_ram3_ds(uint32_t v) {
	system_cpu_device_mem_ctrl3_t *r = (system_cpu_device_mem_ctrl3_t*)(SOC_SYSTEM_REG_BASE + (0x12 << 2));
	r->ram3_ds = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram3_ds(void) {
	system_cpu_device_mem_ctrl3_t *r = (system_cpu_device_mem_ctrl3_t*)(SOC_SYSTEM_REG_BASE + (0x12 << 2));
	return r->ram3_ds;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_ram4_ds(uint32_t v) {
	system_cpu_device_mem_ctrl3_t *r = (system_cpu_device_mem_ctrl3_t*)(SOC_SYSTEM_REG_BASE + (0x12 << 2));
	r->ram4_ds = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram4_ds(void) {
	system_cpu_device_mem_ctrl3_t *r = (system_cpu_device_mem_ctrl3_t*)(SOC_SYSTEM_REG_BASE + (0x12 << 2));
	return r->ram4_ds;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_reserved0(uint32_t v) {
	system_cpu_device_mem_ctrl3_t *r = (system_cpu_device_mem_ctrl3_t*)(SOC_SYSTEM_REG_BASE + (0x12 << 2));
	r->reserved0 = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_reserved0(void) {
	system_cpu_device_mem_ctrl3_t *r = (system_cpu_device_mem_ctrl3_t*)(SOC_SYSTEM_REG_BASE + (0x12 << 2));
	return r->reserved0;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_ram_config(uint32_t v) {
	system_cpu_device_mem_ctrl3_t *r = (system_cpu_device_mem_ctrl3_t*)(SOC_SYSTEM_REG_BASE + (0x12 << 2));
	r->ram_config = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram_config(void) {
	system_cpu_device_mem_ctrl3_t *r = (system_cpu_device_mem_ctrl3_t*)(SOC_SYSTEM_REG_BASE + (0x12 << 2));
	return r->ram_config;
}

static inline void sys_ll_set_cpu_device_mem_ctrl3_reserved1(uint32_t v) {
	system_cpu_device_mem_ctrl3_t *r = (system_cpu_device_mem_ctrl3_t*)(SOC_SYSTEM_REG_BASE + (0x12 << 2));
	r->reserved1 = v;
}

static inline uint32_t sys_ll_get_cpu_device_mem_ctrl3_reserved1(void) {
	system_cpu_device_mem_ctrl3_t *r = (system_cpu_device_mem_ctrl3_t*)(SOC_SYSTEM_REG_BASE + (0x12 << 2));
	return r->reserved1;
}

//reg cpu0_int_0_31_en:

static inline void sys_ll_set_cpu0_int_0_31_en_value(uint32_t v) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_value(void) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	return r->v;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_dma_nsec_intr_en(uint32_t v) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	r->cpu0_dma_nsec_intr_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_dma_nsec_intr_en(void) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	return r->cpu0_dma_nsec_intr_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_encp_sec_intr_en(uint32_t v) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	r->cpu0_encp_sec_intr_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_encp_sec_intr_en(void) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	return r->cpu0_encp_sec_intr_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_encp_nsec_intr_en(uint32_t v) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	r->cpu0_encp_nsec_intr_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_encp_nsec_intr_en(void) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	return r->cpu0_encp_nsec_intr_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_timer_int_en(uint32_t v) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	r->cpu0_timer_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_timer_int_en(void) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	return r->cpu0_timer_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_uart0_int_en(uint32_t v) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	r->cpu0_uart0_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_uart0_int_en(void) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	return r->cpu0_uart0_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_pwm0_int_en(uint32_t v) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	r->cpu0_pwm0_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_pwm0_int_en(void) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	return r->cpu0_pwm0_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_i2c0_int_en(uint32_t v) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	r->cpu0_i2c0_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_i2c0_int_en(void) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	return r->cpu0_i2c0_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_spi0_int_en(uint32_t v) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	r->cpu0_spi0_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_spi0_int_en(void) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	return r->cpu0_spi0_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_uart3_int_en(uint32_t v) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	r->cpu0_uart3_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_uart3_int_en(void) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	return r->cpu0_uart3_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_sdio_int_en(uint32_t v) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	r->cpu0_sdio_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_sdio_int_en(void) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	return r->cpu0_sdio_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_gdma_int_en(uint32_t v) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	r->cpu0_gdma_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_gdma_int_en(void) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	return r->cpu0_gdma_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_la_int_en(uint32_t v) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	r->cpu0_la_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_la_int_en(void) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	return r->cpu0_la_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_timer1_int_en(uint32_t v) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	r->cpu0_timer1_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_timer1_int_en(void) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	return r->cpu0_timer1_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_i2c1_int_en(uint32_t v) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	r->cpu0_i2c1_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_i2c1_int_en(void) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	return r->cpu0_i2c1_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_uart1_int_en(uint32_t v) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	r->cpu0_uart1_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_uart1_int_en(void) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	return r->cpu0_uart1_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_uart2_int_en(uint32_t v) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	r->cpu0_uart2_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_uart2_int_en(void) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	return r->cpu0_uart2_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_spi1_int_en(uint32_t v) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	r->cpu0_spi1_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_spi1_int_en(void) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	return r->cpu0_spi1_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_led_int_en(uint32_t v) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	r->cpu0_led_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_led_int_en(void) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	return r->cpu0_led_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_resv0_int_en(uint32_t v) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	r->cpu0_resv0_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_resv0_int_en(void) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	return r->cpu0_resv0_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_resv1_int_en(uint32_t v) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	r->cpu0_resv1_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_resv1_int_en(void) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	return r->cpu0_resv1_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_ckmn_int_en(uint32_t v) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	r->cpu0_ckmn_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_ckmn_int_en(void) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	return r->cpu0_ckmn_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_resv2_int_en(uint32_t v) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	r->cpu0_resv2_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_resv2_int_en(void) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	return r->cpu0_resv2_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_resv3_int_en(uint32_t v) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	r->cpu0_resv3_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_resv3_int_en(void) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	return r->cpu0_resv3_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_i2s0_int_en(uint32_t v) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	r->cpu0_i2s0_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_i2s0_int_en(void) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	return r->cpu0_i2s0_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_resv4_int_en(uint32_t v) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	r->cpu0_resv4_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_resv4_int_en(void) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	return r->cpu0_resv4_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_resv5_int_en(uint32_t v) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	r->cpu0_resv5_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_resv5_int_en(void) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	return r->cpu0_resv5_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_resv6_int_en(uint32_t v) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	r->cpu0_resv6_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_resv6_int_en(void) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	return r->cpu0_resv6_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_resv7_int_en(uint32_t v) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	r->cpu0_resv7_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_resv7_int_en(void) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	return r->cpu0_resv7_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_int_mac_wakeup_int_en(uint32_t v) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	r->cpu0_int_mac_wakeup_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_int_mac_wakeup_int_en(void) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	return r->cpu0_int_mac_wakeup_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_hsu_irq_int_en(uint32_t v) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	r->cpu0_hsu_irq_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_hsu_irq_int_en(void) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	return r->cpu0_hsu_irq_int_en;
}

static inline void sys_ll_set_cpu0_int_0_31_en_cpu0_mac_int_gen_int_en(uint32_t v) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	r->cpu0_mac_int_gen_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_mac_int_gen_int_en(void) {
	system_cpu0_int_0_31_en_t *r = (system_cpu0_int_0_31_en_t*)(SOC_SYSTEM_REG_BASE + (0x20 << 2));
	return r->cpu0_mac_int_gen_int_en;
}

//reg cpu0_int_32_63_en:

static inline void sys_ll_set_cpu0_int_32_63_en_value(uint32_t v) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_value(void) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	return r->v;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_mac_int_port_trigger_int_en(uint32_t v) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	r->cpu0_mac_int_port_trigger_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_mac_int_port_trigger_int_en(void) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	return r->cpu0_mac_int_port_trigger_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_mac_int_tx_trigger_int_en(uint32_t v) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	r->cpu0_mac_int_tx_trigger_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_mac_int_tx_trigger_int_en(void) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	return r->cpu0_mac_int_tx_trigger_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_mac_int_rx_trigger_int_en(uint32_t v) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	r->cpu0_mac_int_rx_trigger_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_mac_int_rx_trigger_int_en(void) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	return r->cpu0_mac_int_rx_trigger_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_mac_int_tx_rx_misc_int_en(uint32_t v) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	r->cpu0_mac_int_tx_rx_misc_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_mac_int_tx_rx_misc_int_en(void) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	return r->cpu0_mac_int_tx_rx_misc_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_mac_int_tx_rx_timer_int_en(uint32_t v) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	r->cpu0_mac_int_tx_rx_timer_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_mac_int_tx_rx_timer_int_en(void) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	return r->cpu0_mac_int_tx_rx_timer_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_phy_riu_int_int_en(uint32_t v) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	r->cpu0_phy_riu_int_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_phy_riu_int_int_en(void) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	return r->cpu0_phy_riu_int_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_phy_mbp_int_int_en(uint32_t v) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	r->cpu0_phy_mbp_int_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_phy_mbp_int_int_en(void) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	return r->cpu0_phy_mbp_int_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_bt_irq_int_en(uint32_t v) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	r->cpu0_bt_irq_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_bt_irq_int_en(void) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	return r->cpu0_bt_irq_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_ble_irq_int_en(uint32_t v) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	r->cpu0_ble_irq_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_ble_irq_int_en(void) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	return r->cpu0_ble_irq_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_dm_irq_int_en(uint32_t v) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	r->cpu0_dm_irq_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_dm_irq_int_en(void) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	return r->cpu0_dm_irq_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_qspi0_int_en(uint32_t v) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	r->cpu0_qspi0_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_qspi0_int_en(void) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	return r->cpu0_qspi0_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_resv8_int_en(uint32_t v) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	r->cpu0_resv8_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_resv8_int_en(void) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	return r->cpu0_resv8_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_resv9_int_en(uint32_t v) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	r->cpu0_resv9_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_resv9_int_en(void) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	return r->cpu0_resv9_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_resv10_int_en(uint32_t v) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	r->cpu0_resv10_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_resv10_int_en(void) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	return r->cpu0_resv10_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_resv11_int_en(uint32_t v) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	r->cpu0_resv11_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_resv11_int_en(void) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	return r->cpu0_resv11_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_resv12_int_en(uint32_t v) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	r->cpu0_resv12_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_resv12_int_en(void) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	return r->cpu0_resv12_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_thread_int_en(uint32_t v) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	r->cpu0_thread_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_thread_int_en(void) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	return r->cpu0_thread_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_resv13_int_en(uint32_t v) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	r->cpu0_resv13_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_resv13_int_en(void) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	return r->cpu0_resv13_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_otp_int_en(uint32_t v) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	r->cpu0_otp_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_otp_int_en(void) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	return r->cpu0_otp_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_dpllunlock_int_en(uint32_t v) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	r->cpu0_dpllunlock_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_dpllunlock_int_en(void) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	return r->cpu0_dpllunlock_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_resv14_int_en(uint32_t v) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	r->cpu0_resv14_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_resv14_int_en(void) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	return r->cpu0_resv14_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_resv15_int_en(uint32_t v) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	r->cpu0_resv15_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_resv15_int_en(void) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	return r->cpu0_resv15_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_resv16_int_en(uint32_t v) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	r->cpu0_resv16_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_resv16_int_en(void) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	return r->cpu0_resv16_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_gpio_s_int_en(uint32_t v) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	r->cpu0_gpio_s_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_gpio_s_int_en(void) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	return r->cpu0_gpio_s_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_gpio_ns_int_en(uint32_t v) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	r->cpu0_gpio_ns_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_gpio_ns_int_en(void) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	return r->cpu0_gpio_ns_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_ana_gpio_int_en(uint32_t v) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	r->cpu0_ana_gpio_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_ana_gpio_int_en(void) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	return r->cpu0_ana_gpio_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_ana_rtc_int_en(uint32_t v) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	r->cpu0_ana_rtc_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_ana_rtc_int_en(void) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	return r->cpu0_ana_rtc_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_abnormal_gpio_int_en(uint32_t v) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	r->cpu0_abnormal_gpio_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_abnormal_gpio_int_en(void) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	return r->cpu0_abnormal_gpio_int_en;
}

static inline void sys_ll_set_cpu0_int_32_63_en_cpu0_abnormal_rtc_int_en(uint32_t v) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	r->cpu0_abnormal_rtc_int_en = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_abnormal_rtc_int_en(void) {
	system_cpu0_int_32_63_en_t *r = (system_cpu0_int_32_63_en_t*)(SOC_SYSTEM_REG_BASE + (0x21 << 2));
	return r->cpu0_abnormal_rtc_int_en;
}

//reg cpu0_int_0_31_status:

static inline void sys_ll_set_cpu0_int_0_31_status_value(uint32_t v) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_value(void) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	return r->v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_dma_nsec_intr_st(void) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	return r->cpu0_dma_nsec_intr_st;
}

static inline void sys_ll_set_cpu0_int_0_31_status_cpu0_stcp_sec_intr_st(uint32_t v) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	r->cpu0_stcp_sec_intr_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_stcp_sec_intr_st(void) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	return r->cpu0_stcp_sec_intr_st;
}

static inline void sys_ll_set_cpu0_int_0_31_status_cpu0_stcp_nsec_intr_st(uint32_t v) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	r->cpu0_stcp_nsec_intr_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_stcp_nsec_intr_st(void) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	return r->cpu0_stcp_nsec_intr_st;
}

static inline void sys_ll_set_cpu0_int_0_31_status_cpu0_timer_int_st(uint32_t v) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	r->cpu0_timer_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_timer_int_st(void) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	return r->cpu0_timer_int_st;
}

static inline void sys_ll_set_cpu0_int_0_31_status_cpu0_uart0_int_st(uint32_t v) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	r->cpu0_uart0_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_uart0_int_st(void) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	return r->cpu0_uart0_int_st;
}

static inline void sys_ll_set_cpu0_int_0_31_status_cpu0_pwm0_int_st(uint32_t v) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	r->cpu0_pwm0_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_pwm0_int_st(void) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	return r->cpu0_pwm0_int_st;
}

static inline void sys_ll_set_cpu0_int_0_31_status_cpu0_i2c0_int_st(uint32_t v) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	r->cpu0_i2c0_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_i2c0_int_st(void) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	return r->cpu0_i2c0_int_st;
}

static inline void sys_ll_set_cpu0_int_0_31_status_cpu0_spi0_int_st(uint32_t v) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	r->cpu0_spi0_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_spi0_int_st(void) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	return r->cpu0_spi0_int_st;
}

static inline void sys_ll_set_cpu0_int_0_31_status_cpu0_sadc_int_st(uint32_t v) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	r->cpu0_sadc_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_sadc_int_st(void) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	return r->cpu0_sadc_int_st;
}

static inline void sys_ll_set_cpu0_int_0_31_status_cpu0_uart3_int_st(uint32_t v) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	r->cpu0_uart3_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_uart3_int_st(void) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	return r->cpu0_uart3_int_st;
}

static inline void sys_ll_set_cpu0_int_0_31_status_cpu0_sdio_int_st(uint32_t v) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	r->cpu0_sdio_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_sdio_int_st(void) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	return r->cpu0_sdio_int_st;
}

static inline void sys_ll_set_cpu0_int_0_31_status_cpu0_gdma_int_st(uint32_t v) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	r->cpu0_gdma_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_gdma_int_st(void) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	return r->cpu0_gdma_int_st;
}

static inline void sys_ll_set_cpu0_int_0_31_status_cpu0_la_int_st(uint32_t v) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	r->cpu0_la_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_la_int_st(void) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	return r->cpu0_la_int_st;
}

static inline void sys_ll_set_cpu0_int_0_31_status_cpu0_timer1_int_st(uint32_t v) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	r->cpu0_timer1_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_timer1_int_st(void) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	return r->cpu0_timer1_int_st;
}

static inline void sys_ll_set_cpu0_int_0_31_status_cpu0_i2c1_int_st(uint32_t v) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	r->cpu0_i2c1_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_i2c1_int_st(void) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	return r->cpu0_i2c1_int_st;
}

static inline void sys_ll_set_cpu0_int_0_31_status_cpu0_uart1_int_st(uint32_t v) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	r->cpu0_uart1_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_uart1_int_st(void) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	return r->cpu0_uart1_int_st;
}

static inline void sys_ll_set_cpu0_int_0_31_status_cpu0_uart2_int_st(uint32_t v) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	r->cpu0_uart2_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_uart2_int_st(void) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	return r->cpu0_uart2_int_st;
}

static inline void sys_ll_set_cpu0_int_0_31_status_cpu0_spi1_int_st(uint32_t v) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	r->cpu0_spi1_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_spi1_int_st(void) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	return r->cpu0_spi1_int_st;
}

static inline void sys_ll_set_cpu0_int_0_31_status_cpu0_led_int_st(uint32_t v) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	r->cpu0_led_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_led_int_st(void) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	return r->cpu0_led_int_st;
}

static inline void sys_ll_set_cpu0_int_0_31_status_cpu0_resv0_int_st(uint32_t v) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	r->cpu0_resv0_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_resv0_int_st(void) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	return r->cpu0_resv0_int_st;
}

static inline void sys_ll_set_cpu0_int_0_31_status_cpu0_resv1_int_st(uint32_t v) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	r->cpu0_resv1_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_resv1_int_st(void) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	return r->cpu0_resv1_int_st;
}

static inline void sys_ll_set_cpu0_int_0_31_status_cpu0_ckmn_int_st(uint32_t v) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	r->cpu0_ckmn_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_ckmn_int_st(void) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	return r->cpu0_ckmn_int_st;
}

static inline void sys_ll_set_cpu0_int_0_31_status_cpu0_resv2_int_st(uint32_t v) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	r->cpu0_resv2_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_resv2_int_st(void) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	return r->cpu0_resv2_int_st;
}

static inline void sys_ll_set_cpu0_int_0_31_status_cpu0_resv3_int_st(uint32_t v) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	r->cpu0_resv3_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_resv3_int_st(void) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	return r->cpu0_resv3_int_st;
}

static inline void sys_ll_set_cpu0_int_0_31_status_cpu0_i2s0_int_st(uint32_t v) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	r->cpu0_i2s0_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_i2s0_int_st(void) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	return r->cpu0_i2s0_int_st;
}

static inline void sys_ll_set_cpu0_int_0_31_status_cpu0_resv4_int_st(uint32_t v) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	r->cpu0_resv4_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_resv4_int_st(void) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	return r->cpu0_resv4_int_st;
}

static inline void sys_ll_set_cpu0_int_0_31_status_cpu0_resv5_int_st(uint32_t v) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	r->cpu0_resv5_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_resv5_int_st(void) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	return r->cpu0_resv5_int_st;
}

static inline void sys_ll_set_cpu0_int_0_31_status_cpu0_resv6_int_st(uint32_t v) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	r->cpu0_resv6_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_resv6_int_st(void) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	return r->cpu0_resv6_int_st;
}

static inline void sys_ll_set_cpu0_int_0_31_status_cpu0_resv7_int_st(uint32_t v) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	r->cpu0_resv7_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_resv7_int_st(void) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	return r->cpu0_resv7_int_st;
}

static inline void sys_ll_set_cpu0_int_0_31_status_cpu0_int_mac_wakeup_int_st(uint32_t v) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	r->cpu0_int_mac_wakeup_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_int_mac_wakeup_int_st(void) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	return r->cpu0_int_mac_wakeup_int_st;
}

static inline void sys_ll_set_cpu0_int_0_31_status_cpu0_hsu_irq_int_st(uint32_t v) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	r->cpu0_hsu_irq_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_hsu_irq_int_st(void) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	return r->cpu0_hsu_irq_int_st;
}

static inline void sys_ll_set_cpu0_int_0_31_status_cpu0_mac_int_gst_int_st(uint32_t v) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	r->cpu0_mac_int_gst_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_mac_int_gst_int_st(void) {
	system_cpu0_int_0_31_status_t *r = (system_cpu0_int_0_31_status_t*)(SOC_SYSTEM_REG_BASE + (0x28 << 2));
	return r->cpu0_mac_int_gst_int_st;
}

//reg cpu0_int_32_63_status:

static inline void sys_ll_set_cpu0_int_32_63_status_value(uint32_t v) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_value(void) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	return r->v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_mac_int_port_trigger_int_st(void) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	return r->cpu0_mac_int_port_trigger_int_st;
}

static inline void sys_ll_set_cpu0_int_32_63_status_cpu0_mac_int_tx_trigger_int_st(uint32_t v) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	r->cpu0_mac_int_tx_trigger_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_mac_int_tx_trigger_int_st(void) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	return r->cpu0_mac_int_tx_trigger_int_st;
}

static inline void sys_ll_set_cpu0_int_32_63_status_cpu0_mac_int_rx_trigger_int_st(uint32_t v) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	r->cpu0_mac_int_rx_trigger_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_mac_int_rx_trigger_int_st(void) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	return r->cpu0_mac_int_rx_trigger_int_st;
}

static inline void sys_ll_set_cpu0_int_32_63_status_cpu0_mac_int_tx_rx_misc_int_st(uint32_t v) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	r->cpu0_mac_int_tx_rx_misc_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_mac_int_tx_rx_misc_int_st(void) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	return r->cpu0_mac_int_tx_rx_misc_int_st;
}

static inline void sys_ll_set_cpu0_int_32_63_status_cpu0_mac_int_tx_rx_timer_int_st(uint32_t v) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	r->cpu0_mac_int_tx_rx_timer_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_mac_int_tx_rx_timer_int_st(void) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	return r->cpu0_mac_int_tx_rx_timer_int_st;
}

static inline void sys_ll_set_cpu0_int_32_63_status_cpu0_phy_riu_int_int_st(uint32_t v) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	r->cpu0_phy_riu_int_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_phy_riu_int_int_st(void) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	return r->cpu0_phy_riu_int_int_st;
}

static inline void sys_ll_set_cpu0_int_32_63_status_cpu0_phy_mbp_int_int_st(uint32_t v) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	r->cpu0_phy_mbp_int_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_phy_mbp_int_int_st(void) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	return r->cpu0_phy_mbp_int_int_st;
}

static inline void sys_ll_set_cpu0_int_32_63_status_cpu0_bt_irq_int_st(uint32_t v) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	r->cpu0_bt_irq_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_bt_irq_int_st(void) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	return r->cpu0_bt_irq_int_st;
}

static inline void sys_ll_set_cpu0_int_32_63_status_cpu0_ble_irq_int_st(uint32_t v) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	r->cpu0_ble_irq_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_ble_irq_int_st(void) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	return r->cpu0_ble_irq_int_st;
}

static inline void sys_ll_set_cpu0_int_32_63_status_cpu0_dm_irq_int_st(uint32_t v) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	r->cpu0_dm_irq_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_dm_irq_int_st(void) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	return r->cpu0_dm_irq_int_st;
}

static inline void sys_ll_set_cpu0_int_32_63_status_cpu0_qspi0_int_st(uint32_t v) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	r->cpu0_qspi0_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_qspi0_int_st(void) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	return r->cpu0_qspi0_int_st;
}

static inline void sys_ll_set_cpu0_int_32_63_status_cpu0_resv8_int_st(uint32_t v) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	r->cpu0_resv8_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_resv8_int_st(void) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	return r->cpu0_resv8_int_st;
}

static inline void sys_ll_set_cpu0_int_32_63_status_cpu0_resv9_int_st(uint32_t v) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	r->cpu0_resv9_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_resv9_int_st(void) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	return r->cpu0_resv9_int_st;
}

static inline void sys_ll_set_cpu0_int_32_63_status_cpu0_resv10_int_st(uint32_t v) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	r->cpu0_resv10_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_resv10_int_st(void) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	return r->cpu0_resv10_int_st;
}

static inline void sys_ll_set_cpu0_int_32_63_status_cpu0_resv11_int_st(uint32_t v) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	r->cpu0_resv11_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_resv11_int_st(void) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	return r->cpu0_resv11_int_st;
}

static inline void sys_ll_set_cpu0_int_32_63_status_cpu0_resv12_int_st(uint32_t v) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	r->cpu0_resv12_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_resv12_int_st(void) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	return r->cpu0_resv12_int_st;
}

static inline void sys_ll_set_cpu0_int_32_63_status_cpu0_thread_int_st(uint32_t v) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	r->cpu0_thread_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_thread_int_st(void) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	return r->cpu0_thread_int_st;
}

static inline void sys_ll_set_cpu0_int_32_63_status_cpu0_resv13_int_st(uint32_t v) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	r->cpu0_resv13_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_resv13_int_st(void) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	return r->cpu0_resv13_int_st;
}

static inline void sys_ll_set_cpu0_int_32_63_status_cpu0_otp_int_st(uint32_t v) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	r->cpu0_otp_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_otp_int_st(void) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	return r->cpu0_otp_int_st;
}

static inline void sys_ll_set_cpu0_int_32_63_status_cpu0_dpllunlock_int_st(uint32_t v) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	r->cpu0_dpllunlock_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_dpllunlock_int_st(void) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	return r->cpu0_dpllunlock_int_st;
}

static inline void sys_ll_set_cpu0_int_32_63_status_cpu0_resv14_int_st(uint32_t v) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	r->cpu0_resv14_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_resv14_int_st(void) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	return r->cpu0_resv14_int_st;
}

static inline void sys_ll_set_cpu0_int_32_63_status_cpu0_resv15_int_st(uint32_t v) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	r->cpu0_resv15_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_resv15_int_st(void) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	return r->cpu0_resv15_int_st;
}

static inline void sys_ll_set_cpu0_int_32_63_status_cpu0_resv16_int_st(uint32_t v) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	r->cpu0_resv16_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_resv16_int_st(void) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	return r->cpu0_resv16_int_st;
}

static inline void sys_ll_set_cpu0_int_32_63_status_cpu0_gpio_s_int_st(uint32_t v) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	r->cpu0_gpio_s_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_gpio_s_int_st(void) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	return r->cpu0_gpio_s_int_st;
}

static inline void sys_ll_set_cpu0_int_32_63_status_cpu0_gpio_ns_int_st(uint32_t v) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	r->cpu0_gpio_ns_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_gpio_ns_int_st(void) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	return r->cpu0_gpio_ns_int_st;
}

static inline void sys_ll_set_cpu0_int_32_63_status_cpu0_ana_gpio_int_st(uint32_t v) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	r->cpu0_ana_gpio_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_ana_gpio_int_st(void) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	return r->cpu0_ana_gpio_int_st;
}

static inline void sys_ll_set_cpu0_int_32_63_status_cpu0_ana_rtc_int_st(uint32_t v) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	r->cpu0_ana_rtc_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_ana_rtc_int_st(void) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	return r->cpu0_ana_rtc_int_st;
}

static inline void sys_ll_set_cpu0_int_32_63_status_cpu0_abnormal_gpio_int_st(uint32_t v) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	r->cpu0_abnormal_gpio_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_abnormal_gpio_int_st(void) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	return r->cpu0_abnormal_gpio_int_st;
}

static inline void sys_ll_set_cpu0_int_32_63_status_cpu0_abnormal_rtc_int_st(uint32_t v) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	r->cpu0_abnormal_rtc_int_st = v;
}

static inline uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_abnormal_rtc_int_st(void) {
	system_cpu0_int_32_63_status_t *r = (system_cpu0_int_32_63_status_t*)(SOC_SYSTEM_REG_BASE + (0x29 << 2));
	return r->cpu0_abnormal_rtc_int_st;
}

//reg gpio_config0:

static inline void sys_ll_set_gpio_config0_value(uint32_t v) {
	system_gpio_config0_t *r = (system_gpio_config0_t*)(SOC_SYSTEM_REG_BASE + (0x30 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_gpio_config0_value(void) {
	system_gpio_config0_t *r = (system_gpio_config0_t*)(SOC_SYSTEM_REG_BASE + (0x30 << 2));
	return r->v;
}

static inline void sys_ll_set_gpio_config0_gpio_config0(uint32_t v) {
	system_gpio_config0_t *r = (system_gpio_config0_t*)(SOC_SYSTEM_REG_BASE + (0x30 << 2));
	r->gpio_config0 = v;
}

static inline uint32_t sys_ll_get_gpio_config0_gpio_config0(void) {
	system_gpio_config0_t *r = (system_gpio_config0_t*)(SOC_SYSTEM_REG_BASE + (0x30 << 2));
	return r->gpio_config0;
}

//reg gpio_config1:

static inline void sys_ll_set_gpio_config1_value(uint32_t v) {
	system_gpio_config1_t *r = (system_gpio_config1_t*)(SOC_SYSTEM_REG_BASE + (0x31 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_gpio_config1_value(void) {
	system_gpio_config1_t *r = (system_gpio_config1_t*)(SOC_SYSTEM_REG_BASE + (0x31 << 2));
	return r->v;
}

static inline void sys_ll_set_gpio_config1_gpio_config1(uint32_t v) {
	system_gpio_config1_t *r = (system_gpio_config1_t*)(SOC_SYSTEM_REG_BASE + (0x31 << 2));
	r->gpio_config1 = v;
}

static inline uint32_t sys_ll_get_gpio_config1_gpio_config1(void) {
	system_gpio_config1_t *r = (system_gpio_config1_t*)(SOC_SYSTEM_REG_BASE + (0x31 << 2));
	return r->gpio_config1;
}

//reg gpio_config2:

static inline void sys_ll_set_gpio_config2_value(uint32_t v) {
	system_gpio_config2_t *r = (system_gpio_config2_t*)(SOC_SYSTEM_REG_BASE + (0x32 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_gpio_config2_value(void) {
	system_gpio_config2_t *r = (system_gpio_config2_t*)(SOC_SYSTEM_REG_BASE + (0x32 << 2));
	return r->v;
}

static inline void sys_ll_set_gpio_config2_gpio_config2(uint32_t v) {
	system_gpio_config2_t *r = (system_gpio_config2_t*)(SOC_SYSTEM_REG_BASE + (0x32 << 2));
	r->gpio_config2 = v;
}

static inline uint32_t sys_ll_get_gpio_config2_gpio_config2(void) {
	system_gpio_config2_t *r = (system_gpio_config2_t*)(SOC_SYSTEM_REG_BASE + (0x32 << 2));
	return r->gpio_config2;
}

//reg dbug_config0:

static inline void sys_ll_set_dbug_config0_value(uint32_t v) {
	system_dbug_config0_t *r = (system_dbug_config0_t*)(SOC_SYSTEM_REG_BASE + (0x38 << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_dbug_config0_value(void) {
	system_dbug_config0_t *r = (system_dbug_config0_t*)(SOC_SYSTEM_REG_BASE + (0x38 << 2));
	return r->v;
}

static inline void sys_ll_set_dbug_config0_dbug_msg(uint32_t v) {
	system_dbug_config0_t *r = (system_dbug_config0_t*)(SOC_SYSTEM_REG_BASE + (0x38 << 2));
	r->dbug_msg = v;
}

static inline uint32_t sys_ll_get_dbug_config0_dbug_msg(void) {
	system_dbug_config0_t *r = (system_dbug_config0_t*)(SOC_SYSTEM_REG_BASE + (0x38 << 2));
	return r->dbug_msg;
}

static inline void sys_ll_set_dbug_config0_dbug_mux(uint32_t v) {
	system_dbug_config0_t *r = (system_dbug_config0_t*)(SOC_SYSTEM_REG_BASE + (0x38 << 2));
	r->dbug_mux = v;
}

static inline uint32_t sys_ll_get_dbug_config0_dbug_mux(void) {
	system_dbug_config0_t *r = (system_dbug_config0_t*)(SOC_SYSTEM_REG_BASE + (0x38 << 2));
	return r->dbug_mux;
}

static inline void sys_ll_set_dbug_config0_reserved_20_31(uint32_t v) {
	system_dbug_config0_t *r = (system_dbug_config0_t*)(SOC_SYSTEM_REG_BASE + (0x38 << 2));
	r->reserved_20_31 = v;
}

static inline uint32_t sys_ll_get_dbug_config0_reserved_20_31(void) {
	system_dbug_config0_t *r = (system_dbug_config0_t*)(SOC_SYSTEM_REG_BASE + (0x38 << 2));
	return r->reserved_20_31;
}

//reg anareg_stat:

static inline void sys_ll_set_anareg_stat_value(uint32_t v) {
	system_anareg_stat_t *r = (system_anareg_stat_t*)(SOC_SYSTEM_REG_BASE + (0x3a << 2));
	r->v = v;
}

static inline uint32_t sys_ll_get_anareg_stat_value(void) {
	system_anareg_stat_t *r = (system_anareg_stat_t*)(SOC_SYSTEM_REG_BASE + (0x3a << 2));
	return r->v;
}

static inline uint32_t sys_ll_get_anareg_stat_anareg_stat(void) {
	system_anareg_stat_t *r = (system_anareg_stat_t*)(SOC_SYSTEM_REG_BASE + (0x3a << 2));
	return r->anareg_stat;
}



//reg ana_reg0:
static inline void sys_ll_set_ana_reg0_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x40 << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg0_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x40 << 2));
}

static inline void sys_ll_set_ana_reg0_nc_0_6(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 0, 0x7f, v);
}

static inline uint32_t sys_ll_get_ana_reg0_nc_0_6(void) {
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	return r->nc_0_6;
}

static inline void sys_ll_set_ana_reg0_cp(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 7, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg0_cp(void) {
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	return r->cp;
}

static inline void sys_ll_set_ana_reg0_spideten(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 10, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg0_spideten(void) {
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	return r->spideten;
}

static inline void sys_ll_set_ana_reg0_cben(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 11, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg0_cben(void) {
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	return r->cben;
}

static inline void sys_ll_set_ana_reg0_hvref(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 12, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg0_hvref(void) {
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	return r->hvref;
}

static inline void sys_ll_set_ana_reg0_lvref(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 14, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg0_lvref(void) {
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	return r->lvref;
}

static inline void sys_ll_set_ana_reg0_rzctrl26m(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 16, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg0_rzctrl26m(void) {
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	return r->rzctrl26m;
}

static inline void sys_ll_set_ana_reg0_lpf_rz(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 17, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg0_lpf_rz(void) {
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	return r->lpf_rz;
}

static inline void sys_ll_set_ana_reg0_rpc(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 21, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg0_rpc(void) {
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	return r->rpc;
}

static inline void sys_ll_set_ana_reg0_dpll_tsten(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 23, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg0_dpll_tsten(void) {
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	return r->dpll_tsten;
}

static inline void sys_ll_set_ana_reg0_divsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 24, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg0_divsel(void) {
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	return r->divsel;
}

static inline void sys_ll_set_ana_reg0_kctrl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 26, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg0_kctrl(void) {
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	return r->kctrl;
}

static inline void sys_ll_set_ana_reg0_vsel_ldo(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 28, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg0_vsel_ldo(void) {
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	return r->vsel_ldo;
}

static inline void sys_ll_set_ana_reg0_bp_caldone(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 30, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg0_bp_caldone(void) {
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	return r->bp_caldone;
}

static inline void sys_ll_set_ana_reg0_nc_31_31(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 31, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg0_nc_31_31(void) {
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	return r->nc_31_31;
}

//reg ana_reg1:

static inline void sys_ll_set_ana_reg1_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x41 << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg1_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x41 << 2));
}

static inline void sys_ll_set_ana_reg1_nc_0_0(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 0, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg1_nc_0_0(void) {
	sys_ana_reg1_t *r = (sys_ana_reg1_t*)(SOC_SYS_REG_BASE + (0x41 << 2));
	return r->nc_0_0;
}

static inline void sys_ll_set_ana_reg1_bandmanual(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 1, 0x7f, v);
}

static inline uint32_t sys_ll_get_ana_reg1_bandmanual(void) {
	sys_ana_reg1_t *r = (sys_ana_reg1_t*)(SOC_SYS_REG_BASE + (0x41 << 2));
	return r->bandmanual;
}

static inline void sys_ll_set_ana_reg1_mode(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 8, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg1_mode(void) {
	sys_ana_reg1_t *r = (sys_ana_reg1_t*)(SOC_SYS_REG_BASE + (0x41 << 2));
	return r->mode;
}

static inline void sys_ll_set_ana_reg1_manual_ictrl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 10, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg1_manual_ictrl(void) {
	sys_ana_reg1_t *r = (sys_ana_reg1_t*)(SOC_SYS_REG_BASE + (0x41 << 2));
	return r->manual_ictrl;
}

static inline void sys_ll_set_ana_reg1_enclk(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 11, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg1_enclk(void) {
	sys_ana_reg1_t *r = (sys_ana_reg1_t*)(SOC_SYS_REG_BASE + (0x41 << 2));
	return r->enclk;
}

static inline void sys_ll_set_ana_reg1_closeloop_en(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 12, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg1_closeloop_en(void) {
	sys_ana_reg1_t *r = (sys_ana_reg1_t*)(SOC_SYS_REG_BASE + (0x41 << 2));
	return r->closeloop_en;
}

static inline void sys_ll_set_ana_reg1_nc_13_13(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 13, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_nc_13_13(void) {
	sys_ana_reg1_t *r = (sys_ana_reg1_t*)(SOC_SYS_REG_BASE + (0x41 << 2));
	return r->nc_13_13;
}

static inline void sys_ll_set_ana_reg1_spi_rstn(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 14, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg1_spi_rstn(void) {
	sys_ana_reg1_t *r = (sys_ana_reg1_t*)(SOC_SYS_REG_BASE + (0x41 << 2));
	return r->spi_rstn;
}

static inline void sys_ll_set_ana_reg1_osccal_trig(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 15, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg1_osccal_trig(void) {
	sys_ana_reg1_t *r = (sys_ana_reg1_t*)(SOC_SYS_REG_BASE + (0x41 << 2));
	return r->osccal_trig;
}

static inline void sys_ll_set_ana_reg1_manual(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 16, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg1_manual(void) {
	sys_ana_reg1_t *r = (sys_ana_reg1_t*)(SOC_SYS_REG_BASE + (0x41 << 2));
	return r->manual;
}

static inline void sys_ll_set_ana_reg1_diffl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 17, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg1_diffl(void) {
	sys_ana_reg1_t *r = (sys_ana_reg1_t*)(SOC_SYS_REG_BASE + (0x41 << 2));
	return r->diffl;
}

static inline void sys_ll_set_ana_reg1_ictrl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 20, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg1_ictrl(void) {
	sys_ana_reg1_t *r = (sys_ana_reg1_t*)(SOC_SYS_REG_BASE + (0x41 << 2));
	return r->ictrl;
}

static inline void sys_ll_set_ana_reg1_cnti(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 23, 0x1ff, v);
}

static inline uint32_t sys_ll_get_ana_reg1_cnti(void) {
	sys_ana_reg1_t *r = (sys_ana_reg1_t*)(SOC_SYS_REG_BASE + (0x41 << 2));
	return r->cnti;
}

//reg ana_reg2:
static inline uint32_t sys_ana_ll_get_reg2_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x42 << 2));
}

static inline void sys_ana_ll_set_reg2_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x42 << 2)), v);
}

static inline void sys_ll_set_ana_reg2_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x42 << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg2_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x42 << 2));
}

static inline void sys_ll_set_ana_reg2_xtalh_ctune(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 0, 0xff, v);
}

static inline uint32_t sys_ll_get_ana_reg2_xtalh_ctune(void) {
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	return r->xtalh_ctune;
}

static inline void sys_ll_set_ana_reg2_gadc_inbufsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 8, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg2_gadc_inbufsel(void) {
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	return r->gadc_inbufsel;
}

static inline void sys_ll_set_ana_reg2_nc_10_10(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 10, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg2_nc_10_10(void) {
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	return r->nc_10_10;
}

static inline void sys_ll_set_ana_reg2_gadc_compisel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 11, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg2_gadc_compisel(void) {
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	return r->gadc_compisel;
}

static inline void sys_ll_set_ana_reg2_gadc_bscalsaw(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 14, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg2_gadc_bscalsaw(void) {
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	return r->gadc_bscalsaw;
}

static inline void sys_ll_set_ana_reg2_gadc_vncalsaw(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 17, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg2_gadc_vncalsaw(void) {
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	return r->gadc_vncalsaw;
}

static inline void sys_ll_set_ana_reg2_gadc_vpcalsaw(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 20, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg2_gadc_vpcalsaw(void) {
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	return r->gadc_vpcalsaw;
}

static inline void sys_ll_set_ana_reg2_gadc_vbg_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 23, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg2_gadc_vbg_sel(void) {
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	return r->gadc_vbg_sel;
}

static inline void sys_ll_set_ana_reg2_gadc_clk_rlten(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 24, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg2_gadc_clk_rlten(void) {
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	return r->gadc_clk_rlten;
}

static inline void sys_ll_set_ana_reg2_gadc_calintsaw_en(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 25, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg2_gadc_calintsaw_en(void) {
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	return r->gadc_calintsaw_en;
}

static inline void sys_ll_set_ana_reg2_gadc_clk_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 26, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg2_gadc_clk_sel(void) {
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	return r->gadc_clk_sel;
}

static inline void sys_ll_set_ana_reg2_gadc_clk_inv(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 27, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg2_gadc_clk_inv(void) {
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	return r->gadc_clk_inv;
}

static inline void sys_ll_set_ana_reg2_gadc_calcap_ch(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 28, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg2_gadc_calcap_ch(void) {
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	return r->gadc_calcap_ch;
}

static inline void sys_ll_set_ana_reg2_inbufen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 30, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg2_inbufen(void) {
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	return r->inbufen;
}

static inline void sys_ll_set_ana_reg2_sar_enspi(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 31, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg2_sar_enspi(void) {
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	return r->sar_enspi;
}

//reg ana_reg3:

static inline void sys_ll_set_ana_reg3_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x43 << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg3_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x43 << 2));
}

static inline void sys_ll_set_ana_reg3_preamp_isel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 0, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg3_preamp_isel(void) {
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	return r->preamp_isel;
}

static inline void sys_ll_set_ana_reg3_refbuff_isel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 3, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg3_refbuff_isel(void) {
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	return r->refbuff_isel;
}

static inline void sys_ll_set_ana_reg3_vref_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 6, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg3_vref_sel(void) {
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	return r->vref_sel;
}

static inline void sys_ll_set_ana_reg3_offset_en(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 7, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg3_offset_en(void) {
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	return r->offset_en;
}

static inline void sys_ll_set_ana_reg3_hpssren(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 8, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg3_hpssren(void) {
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	return r->hpssren;
}

static inline void sys_ll_set_ana_reg3_ck_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 9, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg3_ck_sel(void) {
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	return r->ck_sel;
}

static inline void sys_ll_set_ana_reg3_anabuf_sel_tx(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 10, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg3_anabuf_sel_tx(void) {
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	return r->anabuf_sel_tx;
}

static inline void sys_ll_set_ana_reg3_ampbias_outen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 11, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg3_ampbias_outen(void) {
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	return r->ampbias_outen;
}

static inline void sys_ll_set_ana_reg3_iamp(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 12, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg3_iamp(void) {
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	return r->iamp;
}

static inline void sys_ll_set_ana_reg3_vddren(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 13, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg3_vddren(void) {
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	return r->vddren;
}

static inline void sys_ll_set_ana_reg3_xamp(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 14, 0x3f, v);
}

static inline uint32_t sys_ll_get_ana_reg3_xamp(void) {
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	return r->xamp;
}

static inline void sys_ll_set_ana_reg3_vosel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 20, 0x1f, v);
}

static inline uint32_t sys_ll_get_ana_reg3_vosel(void) {
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	return r->vosel;
}

static inline void sys_ll_set_ana_reg3_en_xtalh_sleep(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 25, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg3_en_xtalh_sleep(void) {
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	return r->en_xtalh_sleep;
}

static inline void sys_ll_set_ana_reg3_digbufb_bpen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 26, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg3_digbufb_bpen(void) {
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	return r->digbufb_bpen;
}

static inline void sys_ll_set_ana_reg3_bufictrl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 27, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg3_bufictrl(void) {
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	return r->bufictrl;
}

static inline void sys_ll_set_ana_reg3_ibias_ctrl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 28, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg3_ibias_ctrl(void) {
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	return r->ibias_ctrl;
}

static inline void sys_ll_set_ana_reg3_icore_ctrl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 30, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg3_icore_ctrl(void) {
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	return r->icore_ctrl;
}

//reg ana_reg4:
static inline uint32_t sys_ana_ll_get_reg4_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x44 << 2));
}

static inline void sys_ana_ll_set_reg4_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x44 << 2)), v);
}

static inline void sys_ll_set_ana_reg4_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x44 << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg4_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x44 << 2));
}

static inline void sys_ll_set_ana_reg4_td_clk_en(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 0, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg4_td_clk_en(void) {
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	return r->td_clk_en;
}

static inline void sys_ll_set_ana_reg4_td_cks(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 1, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg4_td_cks(void) {
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	return r->td_cks;
}

static inline void sys_ll_set_ana_reg4_td_en(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 3, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg4_td_en(void) {
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	return r->td_en;
}

static inline void sys_ll_set_ana_reg4_td_chn(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 4, 0xffff, v);
}

static inline uint32_t sys_ll_get_ana_reg4_td_chn(void) {
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	return r->td_chn;
}

static inline void sys_ll_set_ana_reg4_td_enscm1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 20, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg4_td_enscm1v(void) {
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	return r->td_enscm1v;
}

static inline void sys_ll_set_ana_reg4_tdldo_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 21, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg4_tdldo_sel(void) {
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	return r->tdldo_sel;
}

static inline void sys_ll_set_ana_reg4_tdldo_bypassen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 23, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg4_tdldo_bypassen(void) {
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	return r->tdldo_bypassen;
}

static inline void sys_ll_set_ana_reg4_nc_24_25(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 24, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg4_nc_24_25(void) {
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	return r->nc_24_25;
}

static inline void sys_ll_set_ana_reg4_en_vddflashtest(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 26, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg4_en_vddflashtest(void) {
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	return r->en_vddflashtest;
}

static inline void sys_ll_set_ana_reg4_vddgpio_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 27, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg4_vddgpio_sel(void) {
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	return r->vddgpio_sel;
}

static inline void sys_ll_set_ana_reg4_vpsramsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 28, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg4_vpsramsel(void) {
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	return r->vpsramsel;
}

static inline void sys_ll_set_ana_reg4_swb(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 30, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg4_swb(void) {
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	return r->swb;
}

static inline void sys_ll_set_ana_reg4_en_psramldo(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 31, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg4_en_psramldo(void) {
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	return r->en_psramldo;
}

//reg ana_reg5:
static inline uint32_t sys_ana_ll_get_reg5_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x45 << 2));
}

static inline void sys_ana_ll_set_reg5_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x45 << 2)), v);
}

static inline void sys_ll_set_ana_reg5_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x45 << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg5_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x45 << 2));
}

static inline void sys_ll_set_ana_reg5_anabufsel_rx(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 0, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg5_anabufsel_rx(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->anabufsel_rx;
}

static inline void sys_ll_set_ana_reg5_en_xtall(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 1, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg5_en_xtall(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->en_xtall;
}

static inline void sys_ll_set_ana_reg5_nc_2_2(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 2, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg5_nc_2_2(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->nc_2_2;
}

static inline void sys_ll_set_ana_reg5_nc_3_3(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 3, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg5_nc_3_3(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->nc_3_3;
}

static inline void sys_ll_set_ana_reg5_en_temp(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 4, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg5_en_temp(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->en_temp;
}

static inline void sys_ll_set_ana_reg5_en_dpll(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 5, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg5_en_dpll(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->en_dpll;
}

static inline void sys_ll_set_ana_reg5_en_cb(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 6, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg5_en_cb(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->en_cb;
}

static inline void sys_ll_set_ana_reg5_pmu_test(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 7, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg5_pmu_test(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->pmu_test;
}

static inline void sys_ll_set_ana_reg5_pwd_anabuf_lownoise(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 8, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg5_pwd_anabuf_lownoise(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->pwd_anabuf_lownoise;
}

static inline void sys_ll_set_ana_reg5_cktst_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 9, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg5_cktst_sel(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->cktst_sel;
}

static inline void sys_ll_set_ana_reg5_ck_tst_enable(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 11, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg5_ck_tst_enable(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->ck_tst_enable;
}

static inline void sys_ll_set_ana_reg5_rosc_disable(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 12, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg5_rosc_disable(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->rosc_disable;
}

static inline void sys_ll_set_ana_reg5_spilatchb_rc32k(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 13, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg5_spilatchb_rc32k(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->spilatchb_rc32k;
}

static inline void sys_ll_set_ana_reg5_pwd_rosc_spi(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 14, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg5_pwd_rosc_spi(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->pwd_rosc_spi;
}

static inline void sys_ll_set_ana_reg5_rc32k_refclken(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 15, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg5_rc32k_refclken(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->rc32k_refclken;
}

static inline void sys_ll_set_ana_reg5_itune_xtall(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 16, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg5_itune_xtall(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->itune_xtall;
}

static inline void sys_ll_set_ana_reg5_xtall_ten(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 20, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg5_xtall_ten(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->xtall_ten;
}

static inline void sys_ll_set_ana_reg5_rosc_tsten(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 21, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg5_rosc_tsten(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->rosc_tsten;
}

static inline void sys_ll_set_ana_reg5_bcal_start(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 22, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg5_bcal_start(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->bcal_start;
}

static inline void sys_ll_set_ana_reg5_bcal_en(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 23, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg5_bcal_en(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->bcal_en;
}

static inline void sys_ll_set_ana_reg5_bcal_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 24, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg5_bcal_sel(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->bcal_sel;
}

static inline void sys_ll_set_ana_reg5_vbias(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 27, 0x1f, v);
}

static inline uint32_t sys_ll_get_ana_reg5_vbias(void) {
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	return r->vbias;
}

//reg ana_reg6:

static inline void sys_ll_set_ana_reg6_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x46 << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg6_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x46 << 2));
}

static inline void sys_ll_set_ana_reg6_calib_interval(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x46 << 2)), 0, 0x3ff, v);
}

static inline uint32_t sys_ll_get_ana_reg6_calib_interval(void) {
	sys_ana_reg6_t *r = (sys_ana_reg6_t*)(SOC_SYS_REG_BASE + (0x46 << 2));
	return r->calib_interval;
}

static inline void sys_ll_set_ana_reg6_modify_interval(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x46 << 2)), 10, 0x3f, v);
}

static inline uint32_t sys_ll_get_ana_reg6_modify_interval(void) {
	sys_ana_reg6_t *r = (sys_ana_reg6_t*)(SOC_SYS_REG_BASE + (0x46 << 2));
	return r->modify_interval;
}

static inline void sys_ll_set_ana_reg6_xtal_wakeup_time(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x46 << 2)), 16, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg6_xtal_wakeup_time(void) {
	sys_ana_reg6_t *r = (sys_ana_reg6_t*)(SOC_SYS_REG_BASE + (0x46 << 2));
	return r->xtal_wakeup_time;
}

static inline void sys_ll_set_ana_reg6_spi_trig(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x46 << 2)), 20, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg6_spi_trig(void) {
	sys_ana_reg6_t *r = (sys_ana_reg6_t*)(SOC_SYS_REG_BASE + (0x46 << 2));
	return r->spi_trig;
}

static inline void sys_ll_set_ana_reg6_modifi_auto(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x46 << 2)), 21, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg6_modifi_auto(void) {
	sys_ana_reg6_t *r = (sys_ana_reg6_t*)(SOC_SYS_REG_BASE + (0x46 << 2));
	return r->modifi_auto;
}

static inline void sys_ll_set_ana_reg6_calib_auto(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x46 << 2)), 22, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg6_calib_auto(void) {
	sys_ana_reg6_t *r = (sys_ana_reg6_t*)(SOC_SYS_REG_BASE + (0x46 << 2));
	return r->calib_auto;
}

static inline void sys_ll_set_ana_reg6_cal_mode(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x46 << 2)), 23, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg6_cal_mode(void) {
	sys_ana_reg6_t *r = (sys_ana_reg6_t*)(SOC_SYS_REG_BASE + (0x46 << 2));
	return r->cal_mode;
}

static inline void sys_ll_set_ana_reg6_manu_ena(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x46 << 2)), 24, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg6_manu_ena(void) {
	sys_ana_reg6_t *r = (sys_ana_reg6_t*)(SOC_SYS_REG_BASE + (0x46 << 2));
	return r->manu_ena;
}

static inline void sys_ll_set_ana_reg6_manu_cin(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x46 << 2)), 25, 0x7f, v);
}

static inline uint32_t sys_ll_get_ana_reg6_manu_cin(void) {
	sys_ana_reg6_t *r = (sys_ana_reg6_t*)(SOC_SYS_REG_BASE + (0x46 << 2));
	return r->manu_cin;
}

//reg ana_reg7:

static inline void sys_ll_set_ana_reg7_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x47 << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg7_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x47 << 2));
}

static inline void sys_ll_set_ana_reg7_clk_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x47 << 2)), 0, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg7_clk_sel(void) {
	sys_ana_reg7_t *r = (sys_ana_reg7_t*)(SOC_SYS_REG_BASE + (0x47 << 2));
	return r->clk_sel;
}

static inline void sys_ll_set_ana_reg7_timer_wkrstn(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x47 << 2)), 1, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg7_timer_wkrstn(void) {
	sys_ana_reg7_t *r = (sys_ana_reg7_t*)(SOC_SYS_REG_BASE + (0x47 << 2));
	return r->timer_wkrstn;
}

static inline void sys_ll_set_ana_reg7_dldohp(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x47 << 2)), 2, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg7_dldohp(void) {
	sys_ana_reg7_t *r = (sys_ana_reg7_t*)(SOC_SYS_REG_BASE + (0x47 << 2));
	return r->dldohp;
}

static inline void sys_ll_set_ana_reg7_vporsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x47 << 2)), 3, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg7_vporsel(void) {
	sys_ana_reg7_t *r = (sys_ana_reg7_t*)(SOC_SYS_REG_BASE + (0x47 << 2));
	return r->vporsel;
}

static inline void sys_ll_set_ana_reg7_vbspbuf_lp(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x47 << 2)), 4, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg7_vbspbuf_lp(void) {
	sys_ana_reg7_t *r = (sys_ana_reg7_t*)(SOC_SYS_REG_BASE + (0x47 << 2));
	return r->vbspbuf_lp;
}

static inline void sys_ll_set_ana_reg7_iocurlim(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x47 << 2)), 5, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg7_iocurlim(void) {
	sys_ana_reg7_t *r = (sys_ana_reg7_t*)(SOC_SYS_REG_BASE + (0x47 << 2));
	return r->iocurlim;
}

static inline void sys_ll_set_ana_reg7_vanaldosel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x47 << 2)), 6, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg7_vanaldosel(void) {
	sys_ana_reg7_t *r = (sys_ana_reg7_t*)(SOC_SYS_REG_BASE + (0x47 << 2));
	return r->vanaldosel;
}
static inline void sys_ll_set_ana_reg7_aldohp(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x47 << 2)), 10, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg7_aldohp(void) {
	sys_ana_reg7_t *r = (sys_ana_reg7_t*)(SOC_SYS_REG_BASE + (0x47 << 2));
	return r->aldohp;
}

static inline void sys_ll_set_ana_reg7_anacurlim(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x47 << 2)), 11, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg7_anacurlim(void) {
	sys_ana_reg7_t *r = (sys_ana_reg7_t*)(SOC_SYS_REG_BASE + (0x47 << 2));
	return r->anacurlim;
}

static inline void sys_ll_set_ana_reg7_envrefh1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x47 << 2)), 12, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg7_envrefh1v(void) {
	sys_ana_reg7_t *r = (sys_ana_reg7_t*)(SOC_SYS_REG_BASE + (0x47 << 2));
	return r->envrefh1v;
}

static inline void sys_ll_set_ana_reg7_violdosel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x47 << 2)), 13, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg7_violdosel(void) {
	sys_ana_reg7_t *r = (sys_ana_reg7_t*)(SOC_SYS_REG_BASE + (0x47 << 2));
	return r->violdosel;
}

static inline void sys_ll_set_ana_reg7_vbatdetsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x47 << 2)), 16, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg7_vbatdetsel(void) {
	sys_ana_reg7_t *r = (sys_ana_reg7_t*)(SOC_SYS_REG_BASE + (0x47 << 2));
	return r->vbatdetsel;
}

static inline void sys_ll_set_ana_reg7_nc_18_18(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x47 << 2)), 18, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg7_nc_18_18(void) {
	sys_ana_reg7_t *r = (sys_ana_reg7_t*)(SOC_SYS_REG_BASE + (0x47 << 2));
	return r->nc_18_18;
}

static inline void sys_ll_set_ana_reg7_spi_pwd_regpow(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x47 << 2)), 19, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg7_spi_pwd_regpow(void) {
	sys_ana_reg7_t *r = (sys_ana_reg7_t*)(SOC_SYS_REG_BASE + (0x47 << 2));
	return r->spi_pwd_regpow;
}

static inline void sys_ll_set_ana_reg7_bypassen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x47 << 2)), 20, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg7_bypassen(void) {
	sys_ana_reg7_t *r = (sys_ana_reg7_t*)(SOC_SYS_REG_BASE + (0x47 << 2));
	return r->bypassen;
}

static inline void sys_ll_set_ana_reg7_ioldolp(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x47 << 2)), 21, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg7_ioldolp(void) {
	sys_ana_reg7_t *r = (sys_ana_reg7_t*)(SOC_SYS_REG_BASE + (0x47 << 2));
	return r->ioldolp;
}

static inline void sys_ll_set_ana_reg7_bgcal(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x47 << 2)), 22, 0x3f, v);
}

static inline uint32_t sys_ll_get_ana_reg7_bgcal(void) {
	sys_ana_reg7_t *r = (sys_ana_reg7_t*)(SOC_SYS_REG_BASE + (0x47 << 2));
	return r->bgcal;
}

static inline void sys_ll_set_ana_reg7_vbgcalmode(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x47 << 2)), 28, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg7_vbgcalmode(void) {
	sys_ana_reg7_t *r = (sys_ana_reg7_t*)(SOC_SYS_REG_BASE + (0x47 << 2));
	return r->vbgcalmode;
}

static inline void sys_ll_set_ana_reg7_vbgcalstart(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x47 << 2)), 29, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg7_vbgcalstart(void) {
	sys_ana_reg7_t *r = (sys_ana_reg7_t*)(SOC_SYS_REG_BASE + (0x47 << 2));
	return r->vbgcalstart;
}

static inline void sys_ll_set_ana_reg7_pwd_bgcal(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x47 << 2)), 30, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg7_pwd_bgcal(void) {
	sys_ana_reg7_t *r = (sys_ana_reg7_t*)(SOC_SYS_REG_BASE + (0x47 << 2));
	return r->pwd_bgcal;
}

static inline void sys_ll_set_ana_reg7_spi_envbg(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x47 << 2)), 31, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg7_spi_envbg(void) {
	sys_ana_reg7_t *r = (sys_ana_reg7_t*)(SOC_SYS_REG_BASE + (0x47 << 2));
	return r->spi_envbg;
}

//reg ana_reg8:

static inline void sys_ll_set_ana_reg8_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x48 << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg8_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x48 << 2));
}

static inline void sys_ll_set_ana_reg8_asoft_stc(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 0, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg8_asoft_stc(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->asoft_stc;
}

static inline void sys_ll_set_ana_reg8_rst_wks1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 4, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg8_rst_wks1v(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->rst_wks1v;
}

static inline void sys_ll_set_ana_reg8_dldo_czsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 5, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg8_dldo_czsel(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->dldo_czsel;
}

static inline void sys_ll_set_ana_reg8_digcurlim(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 8, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg8_digcurlim(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->digcurlim;
}

static inline void sys_ll_set_ana_reg8_spi_latch1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 9, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg8_spi_latch1v(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->spi_latch1v;
}

static inline void sys_ll_set_ana_reg8_dldo_rzsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 10, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg8_dldo_rzsel(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->dldo_rzsel;
}

static inline void sys_ll_set_ana_reg8_lvsleep_wkrst(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 12, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg8_lvsleep_wkrst(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->lvsleep_wkrst;
}

static inline void sys_ll_set_ana_reg8_gpiowk_rstn(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 13, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg8_gpiowk_rstn(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->gpiowk_rstn;
}

static inline void sys_ll_set_ana_reg8_rtcwk_rstn(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 14, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg8_rtcwk_rstn(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->rtcwk_rstn;
}

static inline void sys_ll_set_ana_reg8_ensfsdd(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 15, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg8_ensfsdd(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->ensfsdd;
}

static inline void sys_ll_set_ana_reg8_vcorehsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 16, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg8_vcorehsel(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->vcorehsel;
}

static inline void sys_ll_set_ana_reg8_vcorelsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 20, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg8_vcorelsel(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->vcorelsel;
}

static inline void sys_ll_set_ana_reg8_vlden(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 23, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg8_vlden(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->vlden;
}

static inline void sys_ll_set_ana_reg8_nc_24_24(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 24, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg8_nc_24_24(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->nc_24_24;
}

static inline void sys_ll_set_ana_reg8_aldo_rzsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 25, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg8_aldo_rzsel(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->aldo_rzsel;
}

static inline void sys_ll_set_ana_reg8_aldo_czsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 27, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg8_aldo_czsel(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->aldo_czsel;
}

static inline void sys_ll_set_ana_reg8_vtrxspisel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 30, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg8_vtrxspisel(void) {
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	return r->vtrxspisel;
}

//reg ana_reg9:
static inline uint32_t sys_ana_ll_get_reg9_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x49 << 2));
}

static inline void sys_ana_ll_set_reg9_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x49 << 2)), v);
}

static inline void sys_ll_set_ana_reg9_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x49 << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg9_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x49 << 2));
}

static inline void sys_ll_set_ana_reg9_azcd_manu(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 0, 0x3f, v);
}

static inline uint32_t sys_ll_get_ana_reg9_azcd_manu(void) {
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	return r->azcd_manu;
}

static inline void sys_ll_set_ana_reg9_zcdswvs(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 6, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg9_zcdswvs(void) {
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	return r->zcdswvs;
}

static inline void sys_ll_set_ana_reg9_zcdsrefsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 9, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg9_zcdsrefsel(void) {
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	return r->zcdsrefsel;
}

static inline void sys_ll_set_ana_reg9_enzcdcalib(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 12, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg9_enzcdcalib(void) {
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	return r->enzcdcalib;
}

static inline void sys_ll_set_ana_reg9_enzcddy(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 13, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg9_enzcddy(void) {
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	return r->enzcddy;
}

static inline void sys_ll_set_ana_reg9_zcdmsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 14, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg9_zcdmsel(void) {
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	return r->zcdmsel;
}

static inline void sys_ll_set_ana_reg9_spi_timerwken(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 15, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg9_spi_timerwken(void) {
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	return r->spi_timerwken;
}

static inline void sys_ll_set_ana_reg9_spi_byp32pwd(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 16, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg9_spi_byp32pwd(void) {
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	return r->spi_byp32pwd;
}

static inline void sys_ll_set_ana_reg9_sd(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 17, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg9_sd(void) {
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	return r->sd;
}

static inline void sys_ll_set_ana_reg9_lburstsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 18, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg9_lburstsel(void) {
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	return r->lburstsel;
}

static inline void sys_ll_set_ana_reg9_ckfs(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 20, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg9_ckfs(void) {
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	return r->ckfs;
}

static inline void sys_ll_set_ana_reg9_ckintsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 22, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg9_ckintsel(void) {
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	return r->ckintsel;
}

static inline void sys_ll_set_ana_reg9_osccaltrig(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 23, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg9_osccaltrig(void) {
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	return r->osccaltrig;
}

static inline void sys_ll_set_ana_reg9_mroscsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 24, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg9_mroscsel(void) {
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	return r->mroscsel;
}

static inline void sys_ll_set_ana_reg9_mrosci_cal(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 25, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg9_mrosci_cal(void) {
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	return r->mrosci_cal;
}

static inline void sys_ll_set_ana_reg9_mrosccap_cal(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 28, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg9_mrosccap_cal(void) {
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	return r->mrosccap_cal;
}

//reg ana_reg10:
static inline uint32_t sys_ana_ll_get_reg10_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x4a << 2));
}

static inline void sys_ana_ll_set_reg10_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x4a << 2)), v);
}

static inline void sys_ll_set_ana_reg10_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x4a << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg10_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x4a << 2));
}

static inline void sys_ll_set_ana_reg10_sfsr(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 0, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg10_sfsr(void) {
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	return r->sfsr;
}

static inline void sys_ll_set_ana_reg10_ensfsaa(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 4, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg10_ensfsaa(void) {
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	return r->ensfsaa;
}

static inline void sys_ll_set_ana_reg10_apfms(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 5, 0x1f, v);
}

static inline uint32_t sys_ll_get_ana_reg10_apfms(void) {
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	return r->apfms;
}

static inline void sys_ll_set_ana_reg10_atmpo_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 10, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg10_atmpo_sel(void) {
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	return r->atmpo_sel;
}

static inline void sys_ll_set_ana_reg10_ampoen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 12, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg10_ampoen(void) {
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	return r->ampoen;
}

static inline void sys_ll_set_ana_reg10_nc_13_13(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 13, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg10_nc_13_13(void) {
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	return r->nc_13_13;
}

static inline void sys_ll_set_ana_reg10_avea_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 14, 0x3, v);
}

static inline uint32_t sys_ll_get_ana_reg10_avea_sel(void) {
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	return r->avea_sel;
}

static inline void sys_ll_set_ana_reg10_aforcepfm(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 16, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg10_aforcepfm(void) {
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	return r->aforcepfm;
}

static inline void sys_ll_set_ana_reg10_acls(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 17, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg10_acls(void) {
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	return r->acls;
}

static inline void sys_ll_set_ana_reg10_aswrsten(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 20, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg10_aswrsten(void) {
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	return r->aswrsten;
}

static inline void sys_ll_set_ana_reg10_aripc(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 21, 0x7, v);
}

static inline uint32_t sys_ll_get_ana_reg10_aripc(void) {
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	return r->aripc;
}

static inline void sys_ll_set_ana_reg10_arampc(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 24, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg10_arampc(void) {
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	return r->arampc;
}

static inline void sys_ll_set_ana_reg10_arampcen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 28, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg10_arampcen(void) {
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	return r->arampcen;
}

static inline void sys_ll_set_ana_reg10_aenburst(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 29, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg10_aenburst(void) {
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	return r->aenburst;
}

static inline void sys_ll_set_ana_reg10_apfmen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 30, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg10_apfmen(void) {
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	return r->apfmen;
}

static inline void sys_ll_set_ana_reg10_aldosel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 31, 0x1, v);
}

static inline uint32_t sys_ll_get_ana_reg10_aldosel(void) {
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	return r->aldosel;
}

//reg ana_reg11:

static inline void sys_ll_set_ana_reg11_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x4b << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg11_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x4b << 2));
}

static inline void sys_ll_set_ana_reg11_gpiowk(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 0, 0xffffff, v);
}

static inline uint32_t sys_ll_get_ana_reg11_gpiowk(void) {
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	return r->gpiowk;
}

static inline void sys_ll_set_ana_reg11_rtcsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 24, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg11_rtcsel(void) {
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	return r->rtcsel;
}

static inline void sys_ll_set_ana_reg11_timersel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 28, 0xf, v);
}

static inline uint32_t sys_ll_get_ana_reg11_timersel(void) {
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	return r->timersel;
}

//reg ana_reg12:

static inline void sys_ll_set_ana_reg12_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x4c << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg12_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x4c << 2));
}

static inline void sys_ll_set_ana_reg12_timersel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4c << 2)), 0, 0xffffffff, v);
}

static inline uint32_t sys_ll_get_ana_reg12_timersel(void) {
	sys_ana_reg12_t *r = (sys_ana_reg12_t*)(SOC_SYS_REG_BASE + (0x4c << 2));
	return r->timersel;
}

//reg ana_reg13:

static inline void sys_ll_set_ana_reg13_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x4d << 2)), v);
}

static inline uint32_t sys_ll_get_ana_reg13_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x4d << 2));
}

static inline void sys_ll_set_ana_reg13_rtcsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4d << 2)), 0, 0xffffffff, v);
}

static inline uint32_t sys_ll_get_ana_reg13_rtcsel(void) {
	sys_ana_reg13_t *r = (sys_ana_reg13_t*)(SOC_SYS_REG_BASE + (0x4d << 2));
	return r->rtcsel;
}

#ifdef __cplusplus
}
#endif

#endif // CONFIG_TFM_SYS_LL_NSC
