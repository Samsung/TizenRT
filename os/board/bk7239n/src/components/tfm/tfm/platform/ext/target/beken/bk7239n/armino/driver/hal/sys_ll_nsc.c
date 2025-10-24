// Copyright 2022-2023 Beken
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

#include <soc/soc.h>
#include "hal_port.h"
#include "system_hw.h"
#include "tfm_secure_api.h"
#include "driver/prro.h"

#define PRRO_SYSTEM_SECURE_DECLARATION() uint32_t secure_status

#define PRRO_SET_SYSTEM_SECURE() do { \
	bk_prro_driver_init(); \
	secure_status = bk_prro_get_secure(PRRO_DEV_AHB_SYSTEM); \
	bk_prro_set_secure(PRRO_DEV_AHB_SYSTEM, PRRO_SECURE); \
} while(0)

#define PRRO_SET_SYSTEM_NON_SECURE() do { \
	bk_prro_set_secure(PRRO_DEV_AHB_SYSTEM, secure_status); \
} while(0)

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_analog_reg_value(uint32_t addr)
{
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	uint32_t tmp = REG_READ(addr);
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_analog_reg_value(uint32_t addr, uint32_t value)
{
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	uint32_t idx;
	idx = GET_SYS_ANALOG_REG_IDX(addr);

	REG_WRITE(addr, value);

	while(REG_READ(SYS_ANALOG_REG_SPI_STATE_REG) & (1 << SYS_ANALOG_REG_SPI_STATE_POS(idx)));
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_set_ana_reg_bit(uint32_t reg_addr, uint32_t pos, uint32_t mask, uint32_t value)
{
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	uint32_t reg_value;
	reg_value = *(volatile uint32_t *)(reg_addr);
	reg_value &= ~(mask << pos);
	reg_value |= ((value & mask) <<pos);
	sys_ll_set_analog_reg_value(reg_addr, reg_value);
	PRRO_SET_SYSTEM_NON_SECURE();
}

//reg device_id:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_device_id_value(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_device_id_t *r = (sys_device_id_t*)(SOC_SYS_REG_BASE + (0x0 << 2));
	r->v = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_device_id_value(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_device_id_t *r = (sys_device_id_t*)(SOC_SYS_REG_BASE + (0x0 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_device_id_deviceid(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_device_id_t *r = (sys_device_id_t*)(SOC_SYS_REG_BASE + (0x0 << 2));
	uint32_t tmp = r->deviceid;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg version_id:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_version_id_value(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_version_id_t *r = (sys_version_id_t*)(SOC_SYS_REG_BASE + (0x1 << 2));
	r->v = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_version_id_value(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_version_id_t *r = (sys_version_id_t*)(SOC_SYS_REG_BASE + (0x1 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_version_id_versionid(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_version_id_t *r = (sys_version_id_t*)(SOC_SYS_REG_BASE + (0x1 << 2));
	uint32_t tmp = r->versionid;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg cpu_storage_connect_op_select:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_storage_connect_op_select_value(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_storage_connect_op_select_t *r = (sys_cpu_storage_connect_op_select_t*)(SOC_SYS_REG_BASE + (0x2 << 2));
	r->v = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_storage_connect_op_select_value(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_storage_connect_op_select_t *r = (sys_cpu_storage_connect_op_select_t*)(SOC_SYS_REG_BASE + (0x2 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_storage_connect_op_select_boot_mode(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_storage_connect_op_select_t *r = (sys_cpu_storage_connect_op_select_t*)(SOC_SYS_REG_BASE + (0x2 << 2));
	r->boot_mode = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_storage_connect_op_select_boot_mode(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_storage_connect_op_select_t *r = (sys_cpu_storage_connect_op_select_t*)(SOC_SYS_REG_BASE + (0x2 << 2));
	uint32_t tmp = r->boot_mode;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_storage_connect_op_select_jtag_core_sel(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_storage_connect_op_select_t *r = (sys_cpu_storage_connect_op_select_t*)(SOC_SYS_REG_BASE + (0x2 << 2));
	r->jtag_core_sel = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_storage_connect_op_select_jtag_core_sel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_storage_connect_op_select_t *r = (sys_cpu_storage_connect_op_select_t*)(SOC_SYS_REG_BASE + (0x2 << 2));
	uint32_t tmp = r->jtag_core_sel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_storage_connect_op_select_flash_sel(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_storage_connect_op_select_t *r = (sys_cpu_storage_connect_op_select_t*)(SOC_SYS_REG_BASE + (0x2 << 2));
	r->flash_sel = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_storage_connect_op_select_flash_sel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_storage_connect_op_select_t *r = (sys_cpu_storage_connect_op_select_t*)(SOC_SYS_REG_BASE + (0x2 << 2));
	uint32_t tmp = r->flash_sel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg cpu_current_run_status:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_current_run_status_value(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_current_run_status_t *r = (sys_cpu_current_run_status_t*)(SOC_SYS_REG_BASE + (0x3 << 2));
	r->v = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_current_run_status_value(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_current_run_status_t *r = (sys_cpu_current_run_status_t*)(SOC_SYS_REG_BASE + (0x3 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_current_run_status_core0_halted(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_current_run_status_t *r = (sys_cpu_current_run_status_t*)(SOC_SYS_REG_BASE + (0x3 << 2));
	uint32_t tmp = r->core0_halted;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_current_run_status_core1_halted(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_current_run_status_t *r = (sys_cpu_current_run_status_t*)(SOC_SYS_REG_BASE + (0x3 << 2));
	uint32_t tmp = r->core1_halted;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_current_run_status_cpu0_sw_reset(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_current_run_status_t *r = (sys_cpu_current_run_status_t*)(SOC_SYS_REG_BASE + (0x3 << 2));
	uint32_t tmp = r->cpu0_sw_reset;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_current_run_status_cpu1_sw_reset(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_current_run_status_t *r = (sys_cpu_current_run_status_t*)(SOC_SYS_REG_BASE + (0x3 << 2));
	uint32_t tmp = r->cpu1_sw_reset;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_current_run_status_cpu0_pwr_dw_state(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_current_run_status_t *r = (sys_cpu_current_run_status_t*)(SOC_SYS_REG_BASE + (0x3 << 2));
	uint32_t tmp = r->cpu0_pwr_dw_state;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_current_run_status_cpu1_pwr_dw_state(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_current_run_status_t *r = (sys_cpu_current_run_status_t*)(SOC_SYS_REG_BASE + (0x3 << 2));
	uint32_t tmp = r->cpu1_pwr_dw_state;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg cpu0_int_halt_clk_op:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_halt_clk_op_value(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_halt_clk_op_t *r = (sys_cpu0_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x4 << 2));
	r->v = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_halt_clk_op_value(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_halt_clk_op_t *r = (sys_cpu0_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x4 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_halt_clk_op_cpu0_sw_rst(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_halt_clk_op_t *r = (sys_cpu0_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x4 << 2));
	r->cpu0_sw_rst = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_halt_clk_op_cpu0_sw_rst(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_halt_clk_op_t *r = (sys_cpu0_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x4 << 2));
	uint32_t tmp = r->cpu0_sw_rst;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_halt_clk_op_cpu0_pwr_dw(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_halt_clk_op_t *r = (sys_cpu0_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x4 << 2));
	r->cpu0_pwr_dw = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_halt_clk_op_cpu0_pwr_dw(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_halt_clk_op_t *r = (sys_cpu0_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x4 << 2));
	uint32_t tmp = r->cpu0_pwr_dw;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_halt_clk_op_cpu0_int_mask(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_halt_clk_op_t *r = (sys_cpu0_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x4 << 2));
	r->cpu0_int_mask = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_halt_clk_op_cpu0_int_mask(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_halt_clk_op_t *r = (sys_cpu0_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x4 << 2));
	uint32_t tmp = r->cpu0_int_mask;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_halt_clk_op_cpu0_halt(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_halt_clk_op_t *r = (sys_cpu0_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x4 << 2));
	r->cpu0_halt = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_halt_clk_op_cpu0_halt(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_halt_clk_op_t *r = (sys_cpu0_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x4 << 2));
	uint32_t tmp = r->cpu0_halt;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_halt_clk_op_reserved_4_7(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_halt_clk_op_t *r = (sys_cpu0_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x4 << 2));
	r->reserved_4_7 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_halt_clk_op_reserved_4_7(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_halt_clk_op_t *r = (sys_cpu0_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x4 << 2));
	uint32_t tmp = r->reserved_4_7;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_halt_clk_op_cpu0_offset(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_halt_clk_op_t *r = (sys_cpu0_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x4 << 2));
	uint32_t tmp = r->cpu0_offset;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg cpu1_int_halt_clk_op:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_halt_clk_op_value(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_halt_clk_op_t *r = (sys_cpu1_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x5 << 2));
	r->v = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_halt_clk_op_value(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_halt_clk_op_t *r = (sys_cpu1_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x5 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_halt_clk_op_cpu1_sw_rst(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_halt_clk_op_t *r = (sys_cpu1_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x5 << 2));
	r->cpu1_sw_rst = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_halt_clk_op_cpu1_sw_rst(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_halt_clk_op_t *r = (sys_cpu1_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x5 << 2));
	uint32_t tmp = r->cpu1_sw_rst;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_halt_clk_op_cpu1_pwr_dw(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_halt_clk_op_t *r = (sys_cpu1_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x5 << 2));
	r->cpu1_pwr_dw = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_halt_clk_op_cpu1_pwr_dw(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_halt_clk_op_t *r = (sys_cpu1_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x5 << 2));
	uint32_t tmp = r->cpu1_pwr_dw;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_halt_clk_op_cpu1_int_mask(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_halt_clk_op_t *r = (sys_cpu1_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x5 << 2));
	r->cpu1_int_mask = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_halt_clk_op_cpu1_int_mask(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_halt_clk_op_t *r = (sys_cpu1_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x5 << 2));
	uint32_t tmp = r->cpu1_int_mask;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_halt_clk_op_cpu1_halt(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_halt_clk_op_t *r = (sys_cpu1_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x5 << 2));
	r->cpu1_halt = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_halt_clk_op_cpu1_halt(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_halt_clk_op_t *r = (sys_cpu1_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x5 << 2));
	uint32_t tmp = r->cpu1_halt;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_halt_clk_op_cpu0_bus_clk_2div(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_halt_clk_op_t *r = (sys_cpu1_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x5 << 2));
	r->cpu0_bus_clk_2div = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_halt_clk_op_cpu0_bus_clk_2div(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_halt_clk_op_t *r = (sys_cpu1_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x5 << 2));
	uint32_t tmp = r->cpu0_bus_clk_2div;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_halt_clk_op_reserved_5_7(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_halt_clk_op_t *r = (sys_cpu1_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x5 << 2));
	r->reserved_5_7 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_halt_clk_op_reserved_5_7(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_halt_clk_op_t *r = (sys_cpu1_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x5 << 2));
	uint32_t tmp = r->reserved_5_7;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_halt_clk_op_cpu1_offset(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_halt_clk_op_t *r = (sys_cpu1_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x5 << 2));
	r->cpu1_offset = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_halt_clk_op_cpu1_offset(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_halt_clk_op_t *r = (sys_cpu1_int_halt_clk_op_t*)(SOC_SYS_REG_BASE + (0x5 << 2));
	uint32_t tmp = r->cpu1_offset;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg reserved_reg0x6:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_reserved_reg0x6_value(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_reserved_reg0x6_t *r = (sys_reserved_reg0x6_t*)(SOC_SYS_REG_BASE + (0x6 << 2));
	r->v = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_reserved_reg0x6_value(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_reserved_reg0x6_t *r = (sys_reserved_reg0x6_t*)(SOC_SYS_REG_BASE + (0x6 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_reserved_reg0x6_reserved_0_31(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_reserved_reg0x6_t *r = (sys_reserved_reg0x6_t*)(SOC_SYS_REG_BASE + (0x6 << 2));
	r->reserved_0_31 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_reserved_reg0x6_reserved_0_31(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_reserved_reg0x6_t *r = (sys_reserved_reg0x6_t*)(SOC_SYS_REG_BASE + (0x6 << 2));
	uint32_t tmp = r->reserved_0_31;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg cpu_clk_div_mode1:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode1_value(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->v = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode1_value(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode1_clkdiv_core(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->clkdiv_core = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_core(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	uint32_t tmp = r->clkdiv_core;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode1_cksel_core(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->cksel_core = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_core(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	uint32_t tmp = r->cksel_core;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode1_clkdiv_bus(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->clkdiv_bus = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_bus(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	uint32_t tmp = r->clkdiv_bus;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode1_clkdiv_uart0(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->clkdiv_uart0 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_uart0(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	uint32_t tmp = r->clkdiv_uart0;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode1_clksel_uart0(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->clksel_uart0 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode1_clksel_uart0(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	uint32_t tmp = r->clksel_uart0;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode1_clkdiv_uart1(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->clkdiv_uart1 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_uart1(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	uint32_t tmp = r->clkdiv_uart1;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode1_cksel_uart1(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->cksel_uart1 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_uart1(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	uint32_t tmp = r->cksel_uart1;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode1_clkdiv_uart2(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->clkdiv_uart2 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_uart2(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	uint32_t tmp = r->clkdiv_uart2;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode1_cksel_uart2(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->cksel_uart2 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_uart2(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	uint32_t tmp = r->cksel_uart2;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode1_cksel_sadc(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->cksel_sadc = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_sadc(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	uint32_t tmp = r->cksel_sadc;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode1_cksel_pwm0(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->cksel_pwm0 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_pwm0(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	uint32_t tmp = r->cksel_pwm0;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode1_cksel_pwm1(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->cksel_pwm1 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_pwm1(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	uint32_t tmp = r->cksel_pwm1;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode1_cksel_tim0(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->cksel_tim0 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_tim0(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	uint32_t tmp = r->cksel_tim0;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode1_cksel_tim1(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->cksel_tim1 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_tim1(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	uint32_t tmp = r->cksel_tim1;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode1_cksel_tim2(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->cksel_tim2 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_tim2(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	uint32_t tmp = r->cksel_tim2;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode1_cksel_i2s(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->cksel_i2s = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_i2s(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	uint32_t tmp = r->cksel_i2s;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode1_cksel_aud(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->cksel_aud = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_aud(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	uint32_t tmp = r->cksel_aud;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode1_clkdiv_jpeg(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->clkdiv_jpeg = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_jpeg(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	uint32_t tmp = r->clkdiv_jpeg;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode1_cksel_jpeg(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->cksel_jpeg = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode1_cksel_jpeg(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	uint32_t tmp = r->cksel_jpeg;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode1_clkdiv_disp_l(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	r->clkdiv_disp_l = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode1_clkdiv_disp_l(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode1_t *r = (sys_cpu_clk_div_mode1_t*)(SOC_SYS_REG_BASE + (0x8 << 2));
	uint32_t tmp = r->clkdiv_disp_l;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg cpu_clk_div_mode2:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode2_value(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	r->v = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode2_value(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode2_clkdiv_disp_h(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	r->clkdiv_disp_h = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode2_clkdiv_disp_h(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	uint32_t tmp = r->clkdiv_disp_h;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode2_cksel_disp(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	r->cksel_disp = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode2_cksel_disp(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	uint32_t tmp = r->cksel_disp;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode2_ckdiv_psram(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	r->ckdiv_psram = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode2_ckdiv_psram(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	uint32_t tmp = r->ckdiv_psram;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode2_cksel_psram(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	r->cksel_psram = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode2_cksel_psram(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	uint32_t tmp = r->cksel_psram;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode2_ckdiv_qspi0(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	r->ckdiv_qspi0 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode2_ckdiv_qspi0(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	uint32_t tmp = r->ckdiv_qspi0;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode2_cksel_qspi0(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	r->cksel_qspi0 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode2_cksel_qspi0(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	uint32_t tmp = r->cksel_qspi0;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode2_reserved_11_13(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	r->reserved_11_13 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode2_reserved_11_13(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	uint32_t tmp = r->reserved_11_13;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode2_ckdiv_sdio(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	r->ckdiv_sdio = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode2_ckdiv_sdio(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	uint32_t tmp = r->ckdiv_sdio;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode2_cksel_sdio(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	r->cksel_sdio = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode2_cksel_sdio(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	uint32_t tmp = r->cksel_sdio;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode2_ckdiv_auxs(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	r->ckdiv_auxs = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode2_ckdiv_auxs(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	uint32_t tmp = r->ckdiv_auxs;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode2_cksel_auxs(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	r->cksel_auxs = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode2_cksel_auxs(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	uint32_t tmp = r->cksel_auxs;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode2_cksel_flash(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	r->cksel_flash = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode2_cksel_flash(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	uint32_t tmp = r->cksel_flash;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode2_ckdiv_flash(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	r->ckdiv_flash = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode2_ckdiv_flash(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	uint32_t tmp = r->ckdiv_flash;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_clk_div_mode2_ckdiv_i2s0(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	r->ckdiv_i2s0 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_clk_div_mode2_ckdiv_i2s0(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_clk_div_mode2_t *r = (sys_cpu_clk_div_mode2_t*)(SOC_SYS_REG_BASE + (0x9 << 2));
	uint32_t tmp = r->ckdiv_i2s0;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg cpu_26m_wdt_clk_div:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_26m_wdt_clk_div_value(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_26m_wdt_clk_div_t *r = (sys_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	r->v = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_26m_wdt_clk_div_value(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_26m_wdt_clk_div_t *r = (sys_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	uint32_t tmp = r->v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_26m_wdt_clk_div_ckdiv_26m(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_26m_wdt_clk_div_t *r = (sys_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	r->ckdiv_26m = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_26m_wdt_clk_div_ckdiv_26m(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_26m_wdt_clk_div_t *r = (sys_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	uint32_t tmp = r->ckdiv_26m;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_26m_wdt_clk_div_ckdiv_wdt(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_26m_wdt_clk_div_t *r = (sys_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	r->ckdiv_wdt = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_26m_wdt_clk_div_ckdiv_wdt(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_26m_wdt_clk_div_t *r = (sys_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	uint32_t tmp = r->ckdiv_wdt;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_26m_wdt_clk_div_clksel_spi0(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_26m_wdt_clk_div_t *r = (sys_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	r->clksel_spi0 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_26m_wdt_clk_div_clksel_spi0(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_26m_wdt_clk_div_t *r = (sys_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	uint32_t tmp = r->clksel_spi0;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_26m_wdt_clk_div_clksel_spi1(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_26m_wdt_clk_div_t *r = (sys_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	r->clksel_spi1 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_26m_wdt_clk_div_clksel_spi1(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_26m_wdt_clk_div_t *r = (sys_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	uint32_t tmp = r->clksel_spi1;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_26m_wdt_clk_div_clksel_qspi1(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_26m_wdt_clk_div_t *r = (sys_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	r->cksel_qspi1 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_26m_wdt_clk_div_clksel_qspi1(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_26m_wdt_clk_div_t *r = (sys_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	uint32_t tmp = r->cksel_qspi1;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_26m_wdt_clk_div_clkdiv_qspi1(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_26m_wdt_clk_div_t *r = (sys_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	r->ckdiv_qspi1 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_26m_wdt_clk_div_clkdiv_qspi1(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_26m_wdt_clk_div_t *r = (sys_cpu_26m_wdt_clk_div_t*)(SOC_SYS_REG_BASE + (0xa << 2));
	uint32_t tmp = r->ckdiv_qspi1;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg cpu_anaspi_freq:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_anaspi_freq_value(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_anaspi_freq_t *r = (sys_cpu_anaspi_freq_t*)(SOC_SYS_REG_BASE + (0xb << 2));
	r->v = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_anaspi_freq_value(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_anaspi_freq_t *r = (sys_cpu_anaspi_freq_t*)(SOC_SYS_REG_BASE + (0xb << 2));
	uint32_t tmp = r->v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_anaspi_freq_anaspi_freq(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_anaspi_freq_t *r = (sys_cpu_anaspi_freq_t*)(SOC_SYS_REG_BASE + (0xb << 2));
	r->anaspi_freq = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_anaspi_freq_anaspi_freq(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_anaspi_freq_t *r = (sys_cpu_anaspi_freq_t*)(SOC_SYS_REG_BASE + (0xb << 2));
	uint32_t tmp = r->anaspi_freq;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg cpu_device_clk_enable:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_clk_enable_value(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->v = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_clk_enable_value(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	uint32_t tmp = r->v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_clk_enable_i2c0_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->i2c0_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_clk_enable_i2c0_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	uint32_t tmp = r->i2c0_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_clk_enable_spi0_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->spi0_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_clk_enable_spi0_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	uint32_t tmp = r->spi0_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_clk_enable_uart0_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->uart0_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_clk_enable_uart0_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	uint32_t tmp = r->uart0_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_clk_enable_pwm0_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->pwm0_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_clk_enable_pwm0_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	uint32_t tmp = r->pwm0_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_clk_enable_tim0_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->tim0_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_clk_enable_tim0_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	uint32_t tmp = r->tim0_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_clk_enable_sadc_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->sadc_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_clk_enable_sadc_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	uint32_t tmp = r->sadc_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_clk_enable_irda_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->irda_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_clk_enable_irda_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	uint32_t tmp = r->irda_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_clk_enable_efuse_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->efuse_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_clk_enable_efuse_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	uint32_t tmp = r->efuse_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_clk_enable_i2c1_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->i2c1_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_clk_enable_i2c1_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	uint32_t tmp = r->i2c1_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_clk_enable_spi1_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->spi1_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_clk_enable_spi1_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	uint32_t tmp = r->spi1_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_clk_enable_uart1_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->uart1_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_clk_enable_uart1_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	uint32_t tmp = r->uart1_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_clk_enable_uart2_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->uart2_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_clk_enable_uart2_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	uint32_t tmp = r->uart2_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_clk_enable_pwm1_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->pwm1_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_clk_enable_pwm1_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	uint32_t tmp = r->pwm1_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_clk_enable_tim1_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->tim1_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_clk_enable_tim1_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	uint32_t tmp = r->tim1_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_clk_enable_tim2_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->tim2_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_clk_enable_tim2_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	uint32_t tmp = r->tim2_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_clk_enable_otp_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->otp_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_clk_enable_otp_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	uint32_t tmp = r->otp_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_clk_enable_i2s_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->i2s_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_clk_enable_i2s_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	uint32_t tmp = r->i2s_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_clk_enable_usb_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->usb_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_clk_enable_usb_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	uint32_t tmp = r->usb_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_clk_enable_can_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->can_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_clk_enable_can_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	uint32_t tmp = r->can_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_clk_enable_psram_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->psram_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_clk_enable_psram_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	uint32_t tmp = r->psram_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_clk_enable_qspi0_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->qspi0_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_clk_enable_qspi0_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	uint32_t tmp = r->qspi0_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_clk_enable_qspi1_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->qspi1_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_clk_enable_qspi1_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	uint32_t tmp = r->qspi1_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_clk_enable_sdio_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->sdio_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_clk_enable_sdio_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	uint32_t tmp = r->sdio_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_clk_enable_auxs_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->auxs_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_clk_enable_auxs_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	uint32_t tmp = r->auxs_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_clk_enable_btdm_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->btdm_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_clk_enable_btdm_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	uint32_t tmp = r->btdm_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_clk_enable_xvr_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->xvr_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_clk_enable_xvr_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	uint32_t tmp = r->xvr_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_clk_enable_mac_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->mac_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_clk_enable_mac_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	uint32_t tmp = r->mac_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_clk_enable_phy_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->phy_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_clk_enable_phy_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	uint32_t tmp = r->phy_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_clk_enable_jpeg_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->jpeg_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_clk_enable_jpeg_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	uint32_t tmp = r->jpeg_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_clk_enable_disp_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->disp_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_clk_enable_disp_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	uint32_t tmp = r->disp_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_clk_enable_aud_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->aud_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_clk_enable_aud_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	uint32_t tmp = r->aud_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_clk_enable_wdt_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	r->wdt_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_clk_enable_wdt_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_clk_enable_t *r = (sys_cpu_device_clk_enable_t*)(SOC_SYS_REG_BASE + (0xc << 2));
	uint32_t tmp = r->wdt_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg reserver_reg0xd:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_reserver_reg0xd_value(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_reserver_reg0xd_t *r = (sys_reserver_reg0xd_t*)(SOC_SYS_REG_BASE + (0xd << 2));
	r->v = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_reserver_reg0xd_value(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_reserver_reg0xd_t *r = (sys_reserver_reg0xd_t*)(SOC_SYS_REG_BASE + (0xd << 2));
	uint32_t tmp = r->v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_reserver_reg0xd_h264_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_reserver_reg0xd_t *r = (sys_reserver_reg0xd_t*)(SOC_SYS_REG_BASE + (0xd << 2));
	r->h264_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_reserver_reg0xd_h264_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_reserver_reg0xd_t *r = (sys_reserver_reg0xd_t*)(SOC_SYS_REG_BASE + (0xd << 2));
	uint32_t tmp = r->h264_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_reserver_reg0xd_i2s1_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_reserver_reg0xd_t *r = (sys_reserver_reg0xd_t*)(SOC_SYS_REG_BASE + (0xd << 2));
	r->i2s1_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_reserver_reg0xd_i2s1_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_reserver_reg0xd_t *r = (sys_reserver_reg0xd_t*)(SOC_SYS_REG_BASE + (0xd << 2));
	uint32_t tmp = r->i2s1_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_reserver_reg0xd_i2s2_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_reserver_reg0xd_t *r = (sys_reserver_reg0xd_t*)(SOC_SYS_REG_BASE + (0xd << 2));
	r->i2s2_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_reserver_reg0xd_i2s2_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_reserver_reg0xd_t *r = (sys_reserver_reg0xd_t*)(SOC_SYS_REG_BASE + (0xd << 2));
	uint32_t tmp = r->i2s2_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_reserver_reg0xd_yuv_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_reserver_reg0xd_t *r = (sys_reserver_reg0xd_t*)(SOC_SYS_REG_BASE + (0xd << 2));
	r->yuv_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_reserver_reg0xd_yuv_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_reserver_reg0xd_t *r = (sys_reserver_reg0xd_t*)(SOC_SYS_REG_BASE + (0xd << 2));
	uint32_t tmp = r->yuv_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_reserver_reg0xd_slcd_cken(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_reserver_reg0xd_t *r = (sys_reserver_reg0xd_t*)(SOC_SYS_REG_BASE + (0xd << 2));
	r->slcd_cken = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_reserver_reg0xd_slcd_cken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_reserver_reg0xd_t *r = (sys_reserver_reg0xd_t*)(SOC_SYS_REG_BASE + (0xd << 2));
	uint32_t tmp = r->slcd_cken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_reserver_reg0xd_reserved_5_31(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_reserver_reg0xd_t *r = (sys_reserver_reg0xd_t*)(SOC_SYS_REG_BASE + (0xd << 2));
	r->reserved_5_31 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_reserver_reg0xd_reserved_5_31(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_reserver_reg0xd_t *r = (sys_reserver_reg0xd_t*)(SOC_SYS_REG_BASE + (0xd << 2));
	uint32_t tmp = r->reserved_5_31;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg cpu_device_mem_ctrl1:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl1_value(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->v = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl1_value(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	uint32_t tmp = r->v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl1_uart1_mem_sd(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->uart1_mem_sd = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl1_uart1_mem_sd(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	uint32_t tmp = r->uart1_mem_sd;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl1_uart2(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->uart2 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl1_uart2(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	uint32_t tmp = r->uart2;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl1_spi1(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->spi1 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl1_spi1(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	uint32_t tmp = r->spi1;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl1_sdio(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->sdio = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl1_sdio(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	uint32_t tmp = r->sdio;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl1_usb(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->usb = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl1_usb(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	uint32_t tmp = r->usb;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl1_can(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->can = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl1_can(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	uint32_t tmp = r->can;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl1_qspi0(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->qspi0 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl1_qspi0(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	uint32_t tmp = r->qspi0;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl1_qspi1(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->qspi1 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl1_qspi1(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	uint32_t tmp = r->qspi1;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl1_pram(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->pram = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl1_pram(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	uint32_t tmp = r->pram;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl1_fft(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->fft = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl1_fft(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	uint32_t tmp = r->fft;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl1_abc(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->abc = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl1_abc(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	uint32_t tmp = r->abc;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl1_aud(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->aud = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl1_aud(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	uint32_t tmp = r->aud;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl1_i2s0(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->i2s0 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl1_i2s0(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	uint32_t tmp = r->i2s0;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl1_i2s1(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->i2s1 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl1_i2s1(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	uint32_t tmp = r->i2s1;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl1_i2s2(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->i2s2 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl1_i2s2(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	uint32_t tmp = r->i2s2;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl1_jpge(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->jpge = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl1_jpge(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	uint32_t tmp = r->jpge;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl1_yuv(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->yuv = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl1_yuv(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	uint32_t tmp = r->yuv;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl1_jpgd(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->jpgd = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl1_jpgd(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	uint32_t tmp = r->jpgd;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl1_disp(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->disp = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl1_disp(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	uint32_t tmp = r->disp;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl1_dmad(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->dmad = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl1_dmad(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	uint32_t tmp = r->dmad;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl1_h26f(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->h26f = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl1_h26f(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	uint32_t tmp = r->h26f;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl1_mac(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->mac = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl1_mac(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	uint32_t tmp = r->mac;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl1_phy(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->phy = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl1_phy(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	uint32_t tmp = r->phy;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl1_xvr(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->xvr = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl1_xvr(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	uint32_t tmp = r->xvr;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl1_irda(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->irda = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl1_irda(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	uint32_t tmp = r->irda;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl1_la(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->la = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl1_la(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	uint32_t tmp = r->la;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl1_flsh(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->flsh = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl1_flsh(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	uint32_t tmp = r->flsh;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl1_uart(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->uart = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl1_uart(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	uint32_t tmp = r->uart;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl1_spi0(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->spi0 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl1_spi0(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	uint32_t tmp = r->spi0;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl1_enc(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->enc = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl1_enc(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	uint32_t tmp = r->enc;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl1_dma0(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->dma0 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl1_dma0(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	uint32_t tmp = r->dma0;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl1_dma1(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	r->dma1 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl1_dma1(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl1_t *r = (sys_cpu_device_mem_ctrl1_t*)(SOC_SYS_REG_BASE + (0xe << 2));
	uint32_t tmp = r->dma1;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg cpu_device_mem_ctrl2:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl2_value(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->v = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl2_value(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	uint32_t tmp = r->v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl2_uart1_mem_ds(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->uart1_mem_ds = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl2_uart1_mem_ds(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	uint32_t tmp = r->uart1_mem_ds;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl2_uart2(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->uart2 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl2_uart2(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	uint32_t tmp = r->uart2;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl2_spi1(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->spi1 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl2_spi1(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	uint32_t tmp = r->spi1;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl2_sdio(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->sdio = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl2_sdio(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	uint32_t tmp = r->sdio;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl2_usb(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->usb = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl2_usb(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	uint32_t tmp = r->usb;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl2_can(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->can = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl2_can(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	uint32_t tmp = r->can;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl2_qspi0(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->qspi0 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl2_qspi0(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	uint32_t tmp = r->qspi0;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl2_qspi1(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->qspi1 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl2_qspi1(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	uint32_t tmp = r->qspi1;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl2_pram(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->pram = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl2_pram(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	uint32_t tmp = r->pram;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl2_fft(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->fft = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl2_fft(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	uint32_t tmp = r->fft;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl2_abc(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->abc = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl2_abc(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	uint32_t tmp = r->abc;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl2_aud(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->aud = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl2_aud(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	uint32_t tmp = r->aud;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl2_i2s0(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->i2s0 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl2_i2s0(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	uint32_t tmp = r->i2s0;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl2_i2s1(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->i2s1 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl2_i2s1(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	uint32_t tmp = r->i2s1;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl2_i2s2(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->i2s2 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl2_i2s2(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	uint32_t tmp = r->i2s2;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl2_jpge(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->jpge = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl2_jpge(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	uint32_t tmp = r->jpge;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl2_yuv(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->yuv = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl2_yuv(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	uint32_t tmp = r->yuv;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl2_jpgd(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->jpgd = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl2_jpgd(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	uint32_t tmp = r->jpgd;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl2_disp(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->disp = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl2_disp(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	uint32_t tmp = r->disp;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl2_dmad(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->dmad = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl2_dmad(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	uint32_t tmp = r->dmad;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl2_h26f(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->h26f = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl2_h26f(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	uint32_t tmp = r->h26f;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl2_mac(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->mac = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl2_mac(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	uint32_t tmp = r->mac;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl2_phy(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->phy = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl2_phy(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	uint32_t tmp = r->phy;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl2_xvr(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->xvr = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl2_xvr(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	uint32_t tmp = r->xvr;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl2_irda(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->irda = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl2_irda(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	uint32_t tmp = r->irda;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl2_la(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->la = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl2_la(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	uint32_t tmp = r->la;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl2_flsh(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->flsh = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl2_flsh(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	uint32_t tmp = r->flsh;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl2_uart(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->uart = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl2_uart(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	uint32_t tmp = r->uart;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl2_spi0(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->spi0 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl2_spi0(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	uint32_t tmp = r->spi0;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl2_enc(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->enc = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl2_enc(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	uint32_t tmp = r->enc;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl2_dma0(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->dma0 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl2_dma0(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	uint32_t tmp = r->dma0;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl2_dma1(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	r->dma1 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl2_dma1(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl2_t *r = (sys_cpu_device_mem_ctrl2_t*)(SOC_SYS_REG_BASE + (0xf << 2));
	uint32_t tmp = r->dma1;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg cpu_power_sleep_wakeup:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_power_sleep_wakeup_value(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->v = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_power_sleep_wakeup_value(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_power_sleep_wakeup_pwd_mem1(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->pwd_mem1 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_mem1(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	uint32_t tmp = r->pwd_mem1;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_power_sleep_wakeup_pwd_mem2(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->pwd_mem2 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_mem2(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	uint32_t tmp = r->pwd_mem2;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_power_sleep_wakeup_pwd_mem3(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->pwd_mem3 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_mem3(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	uint32_t tmp = r->pwd_mem3;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_power_sleep_wakeup_pwd_encp(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->pwd_encp = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_encp(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	uint32_t tmp = r->pwd_encp;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_power_sleep_wakeup_pwd_bakp(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->pwd_bakp = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_bakp(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	uint32_t tmp = r->pwd_bakp;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_power_sleep_wakeup_pwd_ahbp(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->pwd_ahbp = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_ahbp(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	uint32_t tmp = r->pwd_ahbp;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_power_sleep_wakeup_pwd_audp(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->pwd_audp = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_audp(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	uint32_t tmp = r->pwd_audp;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_power_sleep_wakeup_pwd_vidp(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->pwd_vidp = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_vidp(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	uint32_t tmp = r->pwd_vidp;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_power_sleep_wakeup_pwd_btsp(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->pwd_btsp = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_btsp(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	uint32_t tmp = r->pwd_btsp;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_power_sleep_wakeup_pwd_wifp_mac(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->pwd_wifp_mac = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_wifp_mac(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	uint32_t tmp = r->pwd_wifp_mac;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_power_sleep_wakeup_pwd_wifp_phy(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->pwd_wifp_phy = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_wifp_phy(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	uint32_t tmp = r->pwd_wifp_phy;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_power_sleep_wakeup_pwd_mem0(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->pwd_mem0 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_mem0(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	uint32_t tmp = r->pwd_mem0;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_power_sleep_wakeup_pwd_mem4(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->pwd_mem4 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_mem4(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	uint32_t tmp = r->pwd_mem4;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_power_sleep_wakeup_pwd_ofdm(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->pwd_ofdm = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_power_sleep_wakeup_pwd_ofdm(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	uint32_t tmp = r->pwd_ofdm;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_power_sleep_wakeup_tcm0_pgen(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->tcm0_pgen = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_power_sleep_wakeup_tcm0_pgen(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	uint32_t tmp = r->tcm0_pgen;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_power_sleep_wakeup_rom_pgen(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->rom_pgen = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_power_sleep_wakeup_rom_pgen(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	uint32_t tmp = r->rom_pgen;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_power_sleep_wakeup_sleep_en_need_flash_idle(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->sleep_en_need_flash_idle = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_power_sleep_wakeup_sleep_en_need_flash_idle(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	uint32_t tmp = r->sleep_en_need_flash_idle;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_power_sleep_wakeup_sleep_en_need_cpu1_wfi(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->sleep_en_need_cpu1_wfi = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_power_sleep_wakeup_sleep_en_need_cpu1_wfi(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	uint32_t tmp = r->sleep_en_need_cpu1_wfi;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_power_sleep_wakeup_sleep_en_need_cpu0_wfi(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->sleep_en_need_cpu0_wfi = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_power_sleep_wakeup_sleep_en_need_cpu0_wfi(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	uint32_t tmp = r->sleep_en_need_cpu0_wfi;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_power_sleep_wakeup_sleep_en_global(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->sleep_en_global = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_power_sleep_wakeup_sleep_en_global(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	uint32_t tmp = r->sleep_en_global;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_power_sleep_wakeup_sleep_bus_idle_bypass(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->sleep_bus_idle_bypass = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_power_sleep_wakeup_sleep_bus_idle_bypass(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	uint32_t tmp = r->sleep_bus_idle_bypass;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_power_sleep_wakeup_bts_wakeup_platform_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->bts_wakeup_platform_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_power_sleep_wakeup_bts_wakeup_platform_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	uint32_t tmp = r->bts_wakeup_platform_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_power_sleep_wakeup_bts_soft_wakeup_req(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->bts_soft_wakeup_req = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_power_sleep_wakeup_bts_soft_wakeup_req(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	uint32_t tmp = r->bts_soft_wakeup_req;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_power_sleep_wakeup_rom_rd_disable(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->rom_rd_disable = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_power_sleep_wakeup_rom_rd_disable(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	uint32_t tmp = r->rom_rd_disable;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_power_sleep_wakeup_otp_rd_disable(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->otp_rd_disable = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_power_sleep_wakeup_otp_rd_disable(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	uint32_t tmp = r->otp_rd_disable;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_power_sleep_wakeup_tcm1_pgen(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->tcm1_pgen = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_power_sleep_wakeup_tcm1_pgen(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	uint32_t tmp = r->tcm1_pgen;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_power_sleep_wakeup_cpu0_subpwdm_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->cpu0_subpwdm_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_power_sleep_wakeup_cpu0_subpwdm_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	uint32_t tmp = r->cpu0_subpwdm_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_power_sleep_wakeup_cpu1_subpwdm_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->cpu1_subpwdm_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_power_sleep_wakeup_cpu1_subpwdm_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	uint32_t tmp = r->cpu1_subpwdm_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_power_sleep_wakeup_share_mem_clkgating_disable(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->share_mem_clkgating_disable = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_power_sleep_wakeup_share_mem_clkgating_disable(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	uint32_t tmp = r->share_mem_clkgating_disable;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_power_sleep_wakeup_cpu0_ticktimer_32k_enable(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->cpu0_ticktimer_32k_enable = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_power_sleep_wakeup_cpu0_ticktimer_32k_enable(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	uint32_t tmp = r->cpu0_ticktimer_32k_enable;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_power_sleep_wakeup_cpu1_ticktimer_32k_enable(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	r->cpu1_ticktimer_32k_enable = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_power_sleep_wakeup_cpu1_ticktimer_32k_enable(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_power_sleep_wakeup_t *r = (sys_cpu_power_sleep_wakeup_t*)(SOC_SYS_REG_BASE + (0x10 << 2));
	uint32_t tmp = r->cpu1_ticktimer_32k_enable;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg reserver_reg0x11:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_reserver_reg0x11_value(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_reserver_reg0x11_t *r = (sys_reserver_reg0x11_t*)(SOC_SYS_REG_BASE + (0x11 << 2));
	r->v = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_reserver_reg0x11_value(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_reserver_reg0x11_t *r = (sys_reserver_reg0x11_t*)(SOC_SYS_REG_BASE + (0x11 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_reserver_reg0x11_reserved_0_31(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_reserver_reg0x11_t *r = (sys_reserver_reg0x11_t*)(SOC_SYS_REG_BASE + (0x11 << 2));
	r->reserved_0_31 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_reserver_reg0x11_reserved_0_31(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_reserver_reg0x11_t *r = (sys_reserver_reg0x11_t*)(SOC_SYS_REG_BASE + (0x11 << 2));
	uint32_t tmp = r->reserved_0_31;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg cpu_device_mem_ctrl3:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl3_value(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->v = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl3_value(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl3_ram0_mem_ds_deep_sleep(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->ram0_mem_ds_deep_sleep = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram0_mem_ds_deep_sleep(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	uint32_t tmp = r->ram0_mem_ds_deep_sleep;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl3_ram1_deep_sleep(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->ram1_deep_sleep = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram1_deep_sleep(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	uint32_t tmp = r->ram1_deep_sleep;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl3_ram2_deep_sleep(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->ram2_deep_sleep = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram2_deep_sleep(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	uint32_t tmp = r->ram2_deep_sleep;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl3_ram3_deep_sleep(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->ram3_deep_sleep = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram3_deep_sleep(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	uint32_t tmp = r->ram3_deep_sleep;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl3_ram4_deep_sleep(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->ram4_deep_sleep = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram4_deep_sleep(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	uint32_t tmp = r->ram4_deep_sleep;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl3_cpu0_icache_itag_deep_sleep(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->cpu0_icache_itag_deep_sleep = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu0_icache_itag_deep_sleep(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	uint32_t tmp = r->cpu0_icache_itag_deep_sleep;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl3_cpu0_dcache_dtag_deep_sleep(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->cpu0_dcache_dtag_deep_sleep = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu0_dcache_dtag_deep_sleep(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	uint32_t tmp = r->cpu0_dcache_dtag_deep_sleep;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl3_cpu1_icache_itag_deep_sleep(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->cpu1_icache_itag_deep_sleep = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu1_icache_itag_deep_sleep(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	uint32_t tmp = r->cpu1_icache_itag_deep_sleep;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl3_cpu1_dcache_dtag_deep_sleep(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->cpu1_dcache_dtag_deep_sleep = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu1_dcache_dtag_deep_sleep(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	uint32_t tmp = r->cpu1_dcache_dtag_deep_sleep;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl3_cpu0_itcm_deep_sleep(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->cpu0_itcm_deep_sleep = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu0_itcm_deep_sleep(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	uint32_t tmp = r->cpu0_itcm_deep_sleep;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl3_cpu0_dtcm_deep_sleep(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->cpu0_dtcm_deep_sleep = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu0_dtcm_deep_sleep(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	uint32_t tmp = r->cpu0_dtcm_deep_sleep;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl3_cpu1_itcm_deep_sleep(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->cpu1_itcm_deep_sleep = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu1_itcm_deep_sleep(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	uint32_t tmp = r->cpu1_itcm_deep_sleep;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl3_cpu1_dtcm_deep_sleep(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->cpu1_dtcm_deep_sleep = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu1_dtcm_deep_sleep(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	uint32_t tmp = r->cpu1_dtcm_deep_sleep;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl3_rott_deep_sleep(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->rott_deep_sleep = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl3_rott_deep_sleep(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	uint32_t tmp = r->rott_deep_sleep;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl3_reserved0(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->reserved0 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl3_reserved0(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	uint32_t tmp = r->reserved0;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl3_reserved1(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->reserved1 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl3_reserved1(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	uint32_t tmp = r->reserved1;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl3_ram0_mem_sd_shutdown(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->ram0_mem_sd_shutdown = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram0_mem_sd_shutdown(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	uint32_t tmp = r->ram0_mem_sd_shutdown;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl3_ram1_shutdown(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->ram1_shutdown = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram1_shutdown(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	uint32_t tmp = r->ram1_shutdown;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl3_ram2_shutdown(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->ram2_shutdown = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram2_shutdown(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	uint32_t tmp = r->ram2_shutdown;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl3_ram3_shutdown(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->ram3_shutdown = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram3_shutdown(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	uint32_t tmp = r->ram3_shutdown;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl3_ram4_shutdown(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->ram4_shutdown = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl3_ram4_shutdown(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	uint32_t tmp = r->ram4_shutdown;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl3_cpu0_icache_itag_shutdown(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->cpu0_icache_itag_shutdown = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu0_icache_itag_shutdown(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	uint32_t tmp = r->cpu0_icache_itag_shutdown;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl3_cpu0_dcache_dtag_shutdown(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->cpu0_dcache_dtag_shutdown = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu0_dcache_dtag_shutdown(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	uint32_t tmp = r->cpu0_dcache_dtag_shutdown;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl3_cpu1_icache_itag_shutdown(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->cpu1_icache_itag_shutdown = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu1_icache_itag_shutdown(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	uint32_t tmp = r->cpu1_icache_itag_shutdown;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl3_cpu1_dcache_dtag_shutdown(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->cpu1_dcache_dtag_shutdown = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu1_dcache_dtag_shutdown(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	uint32_t tmp = r->cpu1_dcache_dtag_shutdown;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl3_cpu0_itcm_shutdown(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->cpu0_itcm_shutdown = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu0_itcm_shutdown(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	uint32_t tmp = r->cpu0_itcm_shutdown;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl3_cpu0_dtcm_shutdown(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->cpu0_dtcm_shutdown = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu0_dtcm_shutdown(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	uint32_t tmp = r->cpu0_dtcm_shutdown;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl3_cpu1_itcm_shutdown(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->cpu1_itcm_shutdown = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu1_itcm_shutdown(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	uint32_t tmp = r->cpu1_itcm_shutdown;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl3_cpu1_dtcm_shutdown(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->cpu1_dtcm_shutdown = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl3_cpu1_dtcm_shutdown(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	uint32_t tmp = r->cpu1_dtcm_shutdown;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl3_rott_shutdown(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->rott_shutdown = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl3_rott_shutdown(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	uint32_t tmp = r->rott_shutdown;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl3_reserved2(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->reserved2 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl3_reserved2(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	uint32_t tmp = r->reserved2;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu_device_mem_ctrl3_reserved3(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	r->reserved3 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu_device_mem_ctrl3_reserved3(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu_device_mem_ctrl3_t *r = (sys_cpu_device_mem_ctrl3_t*)(SOC_SYS_REG_BASE + (0x12 << 2));
	uint32_t tmp = r->reserved3;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg cpu0_int_0_31_en:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_0_31_en_value(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->v = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_en_value(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_0_31_en_cpu0_dma0_nsec_intr_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_dma0_nsec_intr_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_dma0_nsec_intr_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	uint32_t tmp = r->cpu0_dma0_nsec_intr_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_0_31_en_cpu0_encp_sec_intr_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_encp_sec_intr_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_encp_sec_intr_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	uint32_t tmp = r->cpu0_encp_sec_intr_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_0_31_en_cpu0_encp_nsec_intr_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_encp_nsec_intr_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_encp_nsec_intr_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	uint32_t tmp = r->cpu0_encp_nsec_intr_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_0_31_en_cpu0_timer0_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_timer0_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_timer0_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	uint32_t tmp = r->cpu0_timer0_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_0_31_en_cpu0_uart_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_uart_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_uart_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	uint32_t tmp = r->cpu0_uart_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_0_31_en_cpu0_pwm0_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_pwm0_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_pwm0_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	uint32_t tmp = r->cpu0_pwm0_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_0_31_en_cpu0_i2c0_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_i2c0_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_i2c0_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	uint32_t tmp = r->cpu0_i2c0_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_0_31_en_cpu0_spi0_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_spi0_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_spi0_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	uint32_t tmp = r->cpu0_spi0_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_0_31_en_cpu0_sadc_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_sadc_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_sadc_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	uint32_t tmp = r->cpu0_sadc_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_0_31_en_cpu0_irda_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_irda_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_irda_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	uint32_t tmp = r->cpu0_irda_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_0_31_en_cpu0_sdio_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_sdio_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_sdio_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	uint32_t tmp = r->cpu0_sdio_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_0_31_en_cpu0_gdma_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_gdma_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_gdma_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	uint32_t tmp = r->cpu0_gdma_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_0_31_en_cpu0_la_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_la_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_la_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	uint32_t tmp = r->cpu0_la_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_0_31_en_cpu0_timer1_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_timer1_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_timer1_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	uint32_t tmp = r->cpu0_timer1_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_0_31_en_cpu0_i2c1_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_i2c1_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_i2c1_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	uint32_t tmp = r->cpu0_i2c1_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_0_31_en_cpu0_uart1_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_uart1_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_uart1_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	uint32_t tmp = r->cpu0_uart1_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_0_31_en_cpu0_uart2_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_uart2_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_uart2_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	uint32_t tmp = r->cpu0_uart2_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_0_31_en_cpu0_spi1_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_spi1_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_spi1_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	uint32_t tmp = r->cpu0_spi1_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_0_31_en_cpu0_can_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_can_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_can_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	uint32_t tmp = r->cpu0_can_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_0_31_en_cpu0_usb_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_usb_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_usb_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	uint32_t tmp = r->cpu0_usb_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_0_31_en_cpu0_qspi0_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_qspi0_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_qspi0_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	uint32_t tmp = r->cpu0_qspi0_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_0_31_en_cpu0_fft_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_fft_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_fft_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	uint32_t tmp = r->cpu0_fft_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_0_31_en_cpu0_sbc_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_sbc_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_sbc_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	uint32_t tmp = r->cpu0_sbc_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_0_31_en_cpu0_aud_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_aud_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_aud_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	uint32_t tmp = r->cpu0_aud_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_0_31_en_cpu0_i2s0_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_i2s0_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_i2s0_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	uint32_t tmp = r->cpu0_i2s0_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_0_31_en_cpu0_jpegenc_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_jpegenc_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_jpegenc_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	uint32_t tmp = r->cpu0_jpegenc_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_0_31_en_cpu0_jpegdec_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_jpegdec_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_jpegdec_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	uint32_t tmp = r->cpu0_jpegdec_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_0_31_en_cpu0_lcd_display_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_lcd_display_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_lcd_display_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	uint32_t tmp = r->cpu0_lcd_display_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_0_31_en_cpu0_dma2d_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_dma2d_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_dma2d_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	uint32_t tmp = r->cpu0_dma2d_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_0_31_en_cpu0_wifi_int_phy_mpb_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_wifi_int_phy_mpb_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_wifi_int_phy_mpb_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	uint32_t tmp = r->cpu0_wifi_int_phy_mpb_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_0_31_en_cpu0_wifi_int_phy_riu_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_wifi_int_phy_riu_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_wifi_int_phy_riu_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	uint32_t tmp = r->cpu0_wifi_int_phy_riu_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_0_31_en_cpu0_wifi_mac_int_tx_rx_timer_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	r->cpu0_wifi_mac_int_tx_rx_timer_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_en_cpu0_wifi_mac_int_tx_rx_timer_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_en_t *r = (sys_cpu0_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x20 << 2));
	uint32_t tmp = r->cpu0_wifi_mac_int_tx_rx_timer_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg cpu0_int_32_63_en:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_32_63_en_value(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->v = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_en_value(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_tx_rx_misc_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_wifi_mac_int_tx_rx_misc_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_mac_int_tx_rx_misc_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	uint32_t tmp = r->cpu0_wifi_mac_int_tx_rx_misc_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_rx_trigger_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_wifi_mac_int_rx_trigger_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_mac_int_rx_trigger_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	uint32_t tmp = r->cpu0_wifi_mac_int_rx_trigger_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_tx_trigger_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_wifi_mac_int_tx_trigger_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_mac_int_tx_trigger_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	uint32_t tmp = r->cpu0_wifi_mac_int_tx_trigger_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_prot_trigger_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_wifi_mac_int_prot_trigger_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_mac_int_prot_trigger_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	uint32_t tmp = r->cpu0_wifi_mac_int_prot_trigger_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_mac_int_gen_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_wifi_mac_int_gen_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_mac_int_gen_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	uint32_t tmp = r->cpu0_wifi_mac_int_gen_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_hsu_irq_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_wifi_hsu_irq_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_hsu_irq_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	uint32_t tmp = r->cpu0_wifi_hsu_irq_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_32_63_en_cpu0_wifi_int_mac_wakeup_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_wifi_int_mac_wakeup_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_wifi_int_mac_wakeup_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	uint32_t tmp = r->cpu0_wifi_int_mac_wakeup_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_32_63_en_cpu0_dm_irq_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_dm_irq_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_dm_irq_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	uint32_t tmp = r->cpu0_dm_irq_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_32_63_en_cpu0_ble_irq_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_ble_irq_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_ble_irq_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	uint32_t tmp = r->cpu0_ble_irq_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_32_63_en_cpu0_bt_irq_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_bt_irq_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_bt_irq_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	uint32_t tmp = r->cpu0_bt_irq_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_32_63_en_cpu0_qspi1_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_qspi1_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_qspi1_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	uint32_t tmp = r->cpu0_qspi1_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_32_63_en_cpu0_pwm1_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_pwm1_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_pwm1_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	uint32_t tmp = r->cpu0_pwm1_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_32_63_en_cpu0_i2s1_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_i2s1_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_i2s1_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	uint32_t tmp = r->cpu0_i2s1_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_32_63_en_cpu0_i2s2_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_i2s2_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_i2s2_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	uint32_t tmp = r->cpu0_i2s2_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_32_63_en_cpu0_h264_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_h264_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_h264_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	uint32_t tmp = r->cpu0_h264_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_32_63_en_cpu0_sdmadc_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_sdmadc_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_sdmadc_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	uint32_t tmp = r->cpu0_sdmadc_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_32_63_en_cpu0_mbox0_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_mbox0_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_mbox0_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	uint32_t tmp = r->cpu0_mbox0_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_32_63_en_cpu0_mbox1_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_mbox1_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_mbox1_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	uint32_t tmp = r->cpu0_mbox1_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_32_63_en_cpu0_bmc64_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_bmc64_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_bmc64_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	uint32_t tmp = r->cpu0_bmc64_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_32_63_en_cpu0_dpll_unlock_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_dpll_unlock_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_dpll_unlock_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	uint32_t tmp = r->cpu0_dpll_unlock_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_32_63_en_cpu0_touched_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_touched_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_touched_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	uint32_t tmp = r->cpu0_touched_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_32_63_en_cpu0_usbplug_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_usbplug_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_usbplug_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	uint32_t tmp = r->cpu0_usbplug_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_32_63_en_cpu0_rtc_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_rtc_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_rtc_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	uint32_t tmp = r->cpu0_rtc_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_32_63_en_cpu0_gpio_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_gpio_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_gpio_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	uint32_t tmp = r->cpu0_gpio_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_32_63_en_cpu0_dma1_sec_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_dma1_sec_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_dma1_sec_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	uint32_t tmp = r->cpu0_dma1_sec_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_32_63_en_cpu0_dma1_nsec_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_dma1_nsec_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_dma1_nsec_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	uint32_t tmp = r->cpu0_dma1_nsec_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_32_63_en_cpu0_yuvb_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_yuvb_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_yuvb_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	uint32_t tmp = r->cpu0_yuvb_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_32_63_en_cpu0_rott_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->cpu0_rott_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_en_cpu0_rott_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	uint32_t tmp = r->cpu0_rott_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_32_63_en_reserved0(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	r->reserved0 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_en_reserved0(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_en_t *r = (sys_cpu0_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x21 << 2));
	uint32_t tmp = r->reserved0;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg cpu1_int_0_31_en:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_0_31_en_value(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->v = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_en_value(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_0_31_en_cpu1_dma0_nsec_intr_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_dma0_nsec_intr_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_dma0_nsec_intr_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	uint32_t tmp = r->cpu1_dma0_nsec_intr_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_0_31_en_cpu1_encp_sec_intr_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_encp_sec_intr_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_encp_sec_intr_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	uint32_t tmp = r->cpu1_encp_sec_intr_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_0_31_en_cpu1_encp_nsec_intr_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_encp_nsec_intr_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_encp_nsec_intr_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	uint32_t tmp = r->cpu1_encp_nsec_intr_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_0_31_en_cpu1_timer0_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_timer0_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_timer0_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	uint32_t tmp = r->cpu1_timer0_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_0_31_en_cpu1_uart_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_uart_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_uart_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	uint32_t tmp = r->cpu1_uart_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_0_31_en_cpu1_pwm0_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_pwm0_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_pwm0_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	uint32_t tmp = r->cpu1_pwm0_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_0_31_en_cpu1_i2c0_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_i2c0_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_i2c0_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	uint32_t tmp = r->cpu1_i2c0_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_0_31_en_cpu1_spi0_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_spi0_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_spi0_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	uint32_t tmp = r->cpu1_spi0_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_0_31_en_cpu1_sadc_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_sadc_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_sadc_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	uint32_t tmp = r->cpu1_sadc_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_0_31_en_cpu1_irda_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_irda_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_irda_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	uint32_t tmp = r->cpu1_irda_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_0_31_en_cpu1_sdio_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_sdio_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_sdio_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	uint32_t tmp = r->cpu1_sdio_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_0_31_en_cpu1_gdma_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_gdma_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_gdma_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	uint32_t tmp = r->cpu1_gdma_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_0_31_en_cpu1_la_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_la_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_la_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	uint32_t tmp = r->cpu1_la_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_0_31_en_cpu1_timer1_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_timer1_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_timer1_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	uint32_t tmp = r->cpu1_timer1_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_0_31_en_cpu1_i2c1_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_i2c1_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_i2c1_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	uint32_t tmp = r->cpu1_i2c1_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_0_31_en_cpu1_uart1_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_uart1_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_uart1_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	uint32_t tmp = r->cpu1_uart1_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_0_31_en_cpu1_uart2_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_uart2_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_uart2_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	uint32_t tmp = r->cpu1_uart2_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_0_31_en_cpu1_spi1_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_spi1_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_spi1_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	uint32_t tmp = r->cpu1_spi1_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_0_31_en_cpu1_can_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_can_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_can_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	uint32_t tmp = r->cpu1_can_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_0_31_en_cpu1_usb_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_usb_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_usb_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	uint32_t tmp = r->cpu1_usb_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_0_31_en_cpu1_qspi0_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_qspi0_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_qspi0_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	uint32_t tmp = r->cpu1_qspi0_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_0_31_en_cpu1_fft_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_fft_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_fft_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	uint32_t tmp = r->cpu1_fft_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_0_31_en_cpu1_sbc_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_sbc_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_sbc_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	uint32_t tmp = r->cpu1_sbc_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_0_31_en_cpu1_aud_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_aud_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_aud_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	uint32_t tmp = r->cpu1_aud_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_0_31_en_cpu1_i2s0_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_i2s0_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_i2s0_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	uint32_t tmp = r->cpu1_i2s0_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_0_31_en_cpu1_jpegenc_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_jpegenc_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_jpegenc_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	uint32_t tmp = r->cpu1_jpegenc_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_0_31_en_cpu1_jpegdec_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_jpegdec_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_jpegdec_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	uint32_t tmp = r->cpu1_jpegdec_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_0_31_en_cpu1_lcd_display_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_lcd_display_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_lcd_display_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	uint32_t tmp = r->cpu1_lcd_display_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_0_31_en_cpu1_dma2d_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_dma2d_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_dma2d_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	uint32_t tmp = r->cpu1_dma2d_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_0_31_en_cpu1_wifi_int_phy_mpb_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_wifi_int_phy_mpb_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_wifi_int_phy_mpb_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	uint32_t tmp = r->cpu1_wifi_int_phy_mpb_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_0_31_en_cpu1_wifi_int_phy_riu_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_wifi_int_phy_riu_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_wifi_int_phy_riu_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	uint32_t tmp = r->cpu1_wifi_int_phy_riu_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_0_31_en_cpu1_wifi_mac_int_tx_rx_timer_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	r->cpu1_wifi_mac_int_tx_rx_timer_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_en_cpu1_wifi_mac_int_tx_rx_timer_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_en_t *r = (sys_cpu1_int_0_31_en_t*)(SOC_SYS_REG_BASE + (0x22 << 2));
	uint32_t tmp = r->cpu1_wifi_mac_int_tx_rx_timer_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg cpu1_int_32_63_en:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_32_63_en_value(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->v = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_en_value(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_mac_int_tx_rx_misc_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_wifi_mac_int_tx_rx_misc_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_mac_int_tx_rx_misc_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	uint32_t tmp = r->cpu1_wifi_mac_int_tx_rx_misc_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_mac_int_rx_trigger_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_wifi_mac_int_rx_trigger_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_mac_int_rx_trigger_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	uint32_t tmp = r->cpu1_wifi_mac_int_rx_trigger_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_mac_int_tx_trigger_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_wifi_mac_int_tx_trigger_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_mac_int_tx_trigger_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	uint32_t tmp = r->cpu1_wifi_mac_int_tx_trigger_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_mac_int_prot_trigger_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_wifi_mac_int_prot_trigger_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_mac_int_prot_trigger_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	uint32_t tmp = r->cpu1_wifi_mac_int_prot_trigger_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_mac_int_gen_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_wifi_mac_int_gen_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_mac_int_gen_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	uint32_t tmp = r->cpu1_wifi_mac_int_gen_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_hsu_irq_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_wifi_hsu_irq_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_hsu_irq_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	uint32_t tmp = r->cpu1_wifi_hsu_irq_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_32_63_en_cpu1_wifi_int_mac_wakeup_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_wifi_int_mac_wakeup_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_wifi_int_mac_wakeup_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	uint32_t tmp = r->cpu1_wifi_int_mac_wakeup_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_32_63_en_cpu1_dm_irq_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_dm_irq_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_dm_irq_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	uint32_t tmp = r->cpu1_dm_irq_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_32_63_en_cpu1_ble_irq_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_ble_irq_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_ble_irq_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	uint32_t tmp = r->cpu1_ble_irq_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_32_63_en_cpu1_bt_irq_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_bt_irq_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_bt_irq_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	uint32_t tmp = r->cpu1_bt_irq_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_32_63_en_cpu1_qspi1_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_qspi1_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_qspi1_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	uint32_t tmp = r->cpu1_qspi1_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_32_63_en_cpu1_pwm1_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_pwm1_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_pwm1_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	uint32_t tmp = r->cpu1_pwm1_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_32_63_en_cpu1_i2s1_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_i2s1_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_i2s1_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	uint32_t tmp = r->cpu1_i2s1_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_32_63_en_cpu1_i2s2_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_i2s2_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_i2s2_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	uint32_t tmp = r->cpu1_i2s2_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_32_63_en_cpu1_h264_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_h264_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_h264_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	uint32_t tmp = r->cpu1_h264_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_32_63_en_cpu1_sdmadc_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_sdmadc_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_sdmadc_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	uint32_t tmp = r->cpu1_sdmadc_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_32_63_en_cpu1_mbox0_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_mbox0_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_mbox0_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	uint32_t tmp = r->cpu1_mbox0_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_32_63_en_cpu1_mbox1_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_mbox1_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_mbox1_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	uint32_t tmp = r->cpu1_mbox1_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_32_63_en_cpu1_bmc64_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_bmc64_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_bmc64_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	uint32_t tmp = r->cpu1_bmc64_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_32_63_en_cpu1_dpll_unlock_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_dpll_unlock_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_dpll_unlock_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	uint32_t tmp = r->cpu1_dpll_unlock_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_32_63_en_cpu1_touched_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_touched_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_touched_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	uint32_t tmp = r->cpu1_touched_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_32_63_en_cpu1_usbplug_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_usbplug_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_usbplug_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	uint32_t tmp = r->cpu1_usbplug_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_32_63_en_cpu1_rtc_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_rtc_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_rtc_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	uint32_t tmp = r->cpu1_rtc_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_32_63_en_cpu1_gpio_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_gpio_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_gpio_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	uint32_t tmp = r->cpu1_gpio_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_32_63_en_cpu1_dma1_sec_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_dma1_sec_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_dma1_sec_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	uint32_t tmp = r->cpu1_dma1_sec_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_32_63_en_cpu1_dma1_nsec_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_dma1_nsec_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_dma1_nsec_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	uint32_t tmp = r->cpu1_dma1_nsec_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_32_63_en_cpu1_yuvb_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_yuvb_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_yuvb_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	uint32_t tmp = r->cpu1_yuvb_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_32_63_en_cpu1_rott_int_en(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->cpu1_rott_int_en = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_en_cpu1_rott_int_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	uint32_t tmp = r->cpu1_rott_int_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_32_63_en_reserved0(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	r->reserved0 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_en_reserved0(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_en_t *r = (sys_cpu1_int_32_63_en_t*)(SOC_SYS_REG_BASE + (0x23 << 2));
	uint32_t tmp = r->reserved0;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg cpu0_int_0_31_status:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_0_31_status_value(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	r->v = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_status_value(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_dma0_nsec_intr_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	uint32_t tmp = r->cpu0_dma0_nsec_intr_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_encp_sec_intr_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	uint32_t tmp = r->cpu0_encp_sec_intr_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_encp_nsec_intr_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	uint32_t tmp = r->cpu0_encp_nsec_intr_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_timer0_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	uint32_t tmp = r->cpu0_timer0_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_uart_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	uint32_t tmp = r->cpu0_uart_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_pwm0_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	uint32_t tmp = r->cpu0_pwm0_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_i2c0_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	uint32_t tmp = r->cpu0_i2c0_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_spi0_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	uint32_t tmp = r->cpu0_spi0_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_sadc_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	uint32_t tmp = r->cpu0_sadc_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_irda_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	uint32_t tmp = r->cpu0_irda_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_sdio_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	uint32_t tmp = r->cpu0_sdio_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_gdma_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	uint32_t tmp = r->cpu0_gdma_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_la_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	uint32_t tmp = r->cpu0_la_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_timer1_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	uint32_t tmp = r->cpu0_timer1_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_i2c1_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	uint32_t tmp = r->cpu0_i2c1_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_uart1_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	uint32_t tmp = r->cpu0_uart1_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_uart2_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	uint32_t tmp = r->cpu0_uart2_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_spi1_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	uint32_t tmp = r->cpu0_spi1_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_can_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	uint32_t tmp = r->cpu0_can_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_usb_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	uint32_t tmp = r->cpu0_usb_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_qspi0_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	uint32_t tmp = r->cpu0_qspi0_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_fft_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	uint32_t tmp = r->cpu0_fft_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_sbc_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	uint32_t tmp = r->cpu0_sbc_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_aud_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	uint32_t tmp = r->cpu0_aud_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_i2s0_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	uint32_t tmp = r->cpu0_i2s0_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_jpegenc_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	uint32_t tmp = r->cpu0_jpegenc_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_jpegdec_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	uint32_t tmp = r->cpu0_jpegdec_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_lcd_display_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	uint32_t tmp = r->cpu0_lcd_display_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_dma2d_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	uint32_t tmp = r->cpu0_dma2d_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_wifi_int_phy_mpb_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	uint32_t tmp = r->cpu0_wifi_int_phy_mpb_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_wifi_int_phy_riu_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	uint32_t tmp = r->cpu0_wifi_int_phy_riu_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_0_31_status_cpu0_wifi_mac_int_tx_rx_timer_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_0_31_status_t *r = (sys_cpu0_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x28 << 2));
	uint32_t tmp = r->cpu0_wifi_mac_int_tx_rx_timer_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg cpu0_int_32_63_status:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu0_int_32_63_status_value(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	r->v = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_status_value(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_mac_int_tx_rx_misc_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	uint32_t tmp = r->cpu0_wifi_mac_int_tx_rx_misc_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_mac_int_rx_trigger_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	uint32_t tmp = r->cpu0_wifi_mac_int_rx_trigger_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_mac_int_tx_trigger_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	uint32_t tmp = r->cpu0_wifi_mac_int_tx_trigger_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_mac_int_prot_trigger_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	uint32_t tmp = r->cpu0_wifi_mac_int_prot_trigger_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_mac_int_gen_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	uint32_t tmp = r->cpu0_wifi_mac_int_gen_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_hsu_irq_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	uint32_t tmp = r->cpu0_wifi_hsu_irq_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_wifi_int_mac_wakeup_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	uint32_t tmp = r->cpu0_wifi_int_mac_wakeup_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_dm_irq_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	uint32_t tmp = r->cpu0_dm_irq_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_ble_irq_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	uint32_t tmp = r->cpu0_ble_irq_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_bt_irq_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	uint32_t tmp = r->cpu0_bt_irq_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_qspi1_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	uint32_t tmp = r->cpu0_qspi1_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_pwm1_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	uint32_t tmp = r->cpu0_pwm1_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_i2s1_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	uint32_t tmp = r->cpu0_i2s1_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_i2s2_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	uint32_t tmp = r->cpu0_i2s2_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_h264_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	uint32_t tmp = r->cpu0_h264_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_sdmadc_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	uint32_t tmp = r->cpu0_sdmadc_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_mbox0_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	uint32_t tmp = r->cpu0_mbox0_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_mbox1_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	uint32_t tmp = r->cpu0_mbox1_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_bmc64_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	uint32_t tmp = r->cpu0_bmc64_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_dpll_unlock_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	uint32_t tmp = r->cpu0_dpll_unlock_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_touched_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	uint32_t tmp = r->cpu0_touched_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_usbplug_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	uint32_t tmp = r->cpu0_usbplug_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_rtc_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	uint32_t tmp = r->cpu0_rtc_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_gpio_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	uint32_t tmp = r->cpu0_gpio_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_dma1_sec_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	uint32_t tmp = r->cpu0_dma1_sec_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_dma1_nsec_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	uint32_t tmp = r->cpu0_dma1_nsec_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_yuvb_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	uint32_t tmp = r->cpu0_yuvb_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_status_cpu0_rott_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	uint32_t tmp = r->cpu0_rott_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu0_int_32_63_status_reserved0(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu0_int_32_63_status_t *r = (sys_cpu0_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x29 << 2));
	uint32_t tmp = r->reserved0;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg cpu1_int_0_31_status:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_0_31_status_value(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	r->v = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_status_value(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	uint32_t tmp = r->v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_dma0_nsec_intr_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	uint32_t tmp = r->cpu1_dma0_nsec_intr_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_encp_sec_intr_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	uint32_t tmp = r->cpu1_encp_sec_intr_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_encp_nsec_intr_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	uint32_t tmp = r->cpu1_encp_nsec_intr_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_timer0_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	uint32_t tmp = r->cpu1_timer0_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_uart_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	uint32_t tmp = r->cpu1_uart_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_pwm0_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	uint32_t tmp = r->cpu1_pwm0_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_i2c0_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	uint32_t tmp = r->cpu1_i2c0_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_spi0_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	uint32_t tmp = r->cpu1_spi0_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_sadc_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	uint32_t tmp = r->cpu1_sadc_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_irda_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	uint32_t tmp = r->cpu1_irda_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_sdio_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	uint32_t tmp = r->cpu1_sdio_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_gdma_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	uint32_t tmp = r->cpu1_gdma_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_la_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	uint32_t tmp = r->cpu1_la_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_timer1_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	uint32_t tmp = r->cpu1_timer1_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_i2c1_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	uint32_t tmp = r->cpu1_i2c1_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_uart1_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	uint32_t tmp = r->cpu1_uart1_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_uart2_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	uint32_t tmp = r->cpu1_uart2_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_spi1_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	uint32_t tmp = r->cpu1_spi1_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_can_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	uint32_t tmp = r->cpu1_can_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_usb_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	uint32_t tmp = r->cpu1_usb_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_qspi0_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	uint32_t tmp = r->cpu1_qspi0_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_fft_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	uint32_t tmp = r->cpu1_fft_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_sbc_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	uint32_t tmp = r->cpu1_sbc_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_aud_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	uint32_t tmp = r->cpu1_aud_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_i2s0_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	uint32_t tmp = r->cpu1_i2s0_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_jpegenc_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	uint32_t tmp = r->cpu1_jpegenc_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_jpegdec_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	uint32_t tmp = r->cpu1_jpegdec_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_lcd_display_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	uint32_t tmp = r->cpu1_lcd_display_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_dma2d_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	uint32_t tmp = r->cpu1_dma2d_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_wifi_int_phy_mpb_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	uint32_t tmp = r->cpu1_wifi_int_phy_mpb_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_wifi_int_phy_riu_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	uint32_t tmp = r->cpu1_wifi_int_phy_riu_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_0_31_status_cpu1_wifi_mac_int_tx_rx_timer_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_0_31_status_t *r = (sys_cpu1_int_0_31_status_t*)(SOC_SYS_REG_BASE + (0x2a << 2));
	uint32_t tmp = r->cpu1_wifi_mac_int_tx_rx_timer_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg cpu1_int_32_63_status:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_cpu1_int_32_63_status_value(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	r->v = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_status_value(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	uint32_t tmp = r->v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_mac_int_tx_rx_misc_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	uint32_t tmp = r->cpu1_wifi_mac_int_tx_rx_misc_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_mac_int_rx_trigger_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	uint32_t tmp = r->cpu1_wifi_mac_int_rx_trigger_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_mac_int_tx_trigger_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	uint32_t tmp = r->cpu1_wifi_mac_int_tx_trigger_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_mac_int_prot_trigger_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	uint32_t tmp = r->cpu1_wifi_mac_int_prot_trigger_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_mac_int_gen_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	uint32_t tmp = r->cpu1_wifi_mac_int_gen_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_hsu_irq_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	uint32_t tmp = r->cpu1_wifi_hsu_irq_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_wifi_int_mac_wakeup_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	uint32_t tmp = r->cpu1_wifi_int_mac_wakeup_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_dm_irq_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	uint32_t tmp = r->cpu1_dm_irq_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_ble_irq_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	uint32_t tmp = r->cpu1_ble_irq_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_bt_irq_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	uint32_t tmp = r->cpu1_bt_irq_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_qspi1_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	uint32_t tmp = r->cpu1_qspi1_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_pwm1_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	uint32_t tmp = r->cpu1_pwm1_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_i2s1_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	uint32_t tmp = r->cpu1_i2s1_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_i2s2_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	uint32_t tmp = r->cpu1_i2s2_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_h264_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	uint32_t tmp = r->cpu1_h264_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_sdmadc_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	uint32_t tmp = r->cpu1_sdmadc_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_mbox0_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	uint32_t tmp = r->cpu1_mbox0_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_mbox1_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	uint32_t tmp = r->cpu1_mbox1_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_bmc64_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	uint32_t tmp = r->cpu1_bmc64_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_dpll_unlock_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	uint32_t tmp = r->cpu1_dpll_unlock_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_touched_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	uint32_t tmp = r->cpu1_touched_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_usbplug_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	uint32_t tmp = r->cpu1_usbplug_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_rtc_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	uint32_t tmp = r->cpu1_rtc_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_gpio_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	uint32_t tmp = r->cpu1_gpio_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_dma1_sec_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	uint32_t tmp = r->cpu1_dma1_sec_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_dma1_nsec_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	uint32_t tmp = r->cpu1_dma1_nsec_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_yuvb_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	uint32_t tmp = r->cpu1_yuvb_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_status_cpu1_rott_int_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	uint32_t tmp = r->cpu1_rott_int_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_cpu1_int_32_63_status_reserved0(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_cpu1_int_32_63_status_t *r = (sys_cpu1_int_32_63_status_t*)(SOC_SYS_REG_BASE + (0x2b << 2));
	uint32_t tmp = r->reserved0;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg gpio_config0:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_gpio_config0_value(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_gpio_config0_t *r = (sys_gpio_config0_t*)(SOC_SYS_REG_BASE + (0x30 << 2));
	r->v = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_gpio_config0_value(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_gpio_config0_t *r = (sys_gpio_config0_t*)(SOC_SYS_REG_BASE + (0x30 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_gpio_config0_gpio_config0(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_gpio_config0_t *r = (sys_gpio_config0_t*)(SOC_SYS_REG_BASE + (0x30 << 2));
	r->gpio_config0 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_gpio_config0_gpio_config0(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_gpio_config0_t *r = (sys_gpio_config0_t*)(SOC_SYS_REG_BASE + (0x30 << 2));
	uint32_t tmp = r->gpio_config0;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg gpio_config1:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_gpio_config1_value(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_gpio_config1_t *r = (sys_gpio_config1_t*)(SOC_SYS_REG_BASE + (0x31 << 2));
	r->v = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_gpio_config1_value(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_gpio_config1_t *r = (sys_gpio_config1_t*)(SOC_SYS_REG_BASE + (0x31 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_gpio_config1_gpio_config1(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_gpio_config1_t *r = (sys_gpio_config1_t*)(SOC_SYS_REG_BASE + (0x31 << 2));
	r->gpio_config1 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_gpio_config1_gpio_config1(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_gpio_config1_t *r = (sys_gpio_config1_t*)(SOC_SYS_REG_BASE + (0x31 << 2));
	uint32_t tmp = r->gpio_config1;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg gpio_config2:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_gpio_config2_value(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_gpio_config2_t *r = (sys_gpio_config2_t*)(SOC_SYS_REG_BASE + (0x32 << 2));
	r->v = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_gpio_config2_value(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_gpio_config2_t *r = (sys_gpio_config2_t*)(SOC_SYS_REG_BASE + (0x32 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_gpio_config2_gpio_config2(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_gpio_config2_t *r = (sys_gpio_config2_t*)(SOC_SYS_REG_BASE + (0x32 << 2));
	r->gpio_config2 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_gpio_config2_gpio_config2(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_gpio_config2_t *r = (sys_gpio_config2_t*)(SOC_SYS_REG_BASE + (0x32 << 2));
	uint32_t tmp = r->gpio_config2;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg gpio_config3:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_gpio_config3_value(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_gpio_config3_t *r = (sys_gpio_config3_t*)(SOC_SYS_REG_BASE + (0x33 << 2));
	r->v = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_gpio_config3_value(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_gpio_config3_t *r = (sys_gpio_config3_t*)(SOC_SYS_REG_BASE + (0x33 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_gpio_config3_gpio_config3(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_gpio_config3_t *r = (sys_gpio_config3_t*)(SOC_SYS_REG_BASE + (0x33 << 2));
	r->gpio_config3 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_gpio_config3_gpio_config3(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_gpio_config3_t *r = (sys_gpio_config3_t*)(SOC_SYS_REG_BASE + (0x33 << 2));
	uint32_t tmp = r->gpio_config3;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg gpio_config4:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_gpio_config4_value(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_gpio_config4_t *r = (sys_gpio_config4_t*)(SOC_SYS_REG_BASE + (0x34 << 2));
	r->v = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_gpio_config4_value(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_gpio_config4_t *r = (sys_gpio_config4_t*)(SOC_SYS_REG_BASE + (0x34 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_gpio_config4_gpio_config4(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_gpio_config4_t *r = (sys_gpio_config4_t*)(SOC_SYS_REG_BASE + (0x34 << 2));
	r->gpio_config4 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_gpio_config4_gpio_config4(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_gpio_config4_t *r = (sys_gpio_config4_t*)(SOC_SYS_REG_BASE + (0x34 << 2));
	uint32_t tmp = r->gpio_config4;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg gpio_config5:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_gpio_config5_value(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_gpio_config5_t *r = (sys_gpio_config5_t*)(SOC_SYS_REG_BASE + (0x35 << 2));
	r->v = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_gpio_config5_value(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_gpio_config5_t *r = (sys_gpio_config5_t*)(SOC_SYS_REG_BASE + (0x35 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_gpio_config5_gpio_config5(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_gpio_config5_t *r = (sys_gpio_config5_t*)(SOC_SYS_REG_BASE + (0x35 << 2));
	r->gpio_config5 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_gpio_config5_gpio_config5(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_gpio_config5_t *r = (sys_gpio_config5_t*)(SOC_SYS_REG_BASE + (0x35 << 2));
	uint32_t tmp = r->gpio_config5;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg sys_debug_config0:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_sys_debug_config0_value(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_sys_debug_config0_t *r = (sys_sys_debug_config0_t*)(SOC_SYS_REG_BASE + (0x38 << 2));
	r->v = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_sys_debug_config0_value(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_sys_debug_config0_t *r = (sys_sys_debug_config0_t*)(SOC_SYS_REG_BASE + (0x38 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_sys_debug_config0_dbug_config0(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_sys_debug_config0_t *r = (sys_sys_debug_config0_t*)(SOC_SYS_REG_BASE + (0x38 << 2));
	r->dbug_config0 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_sys_debug_config0_dbug_config0(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_sys_debug_config0_t *r = (sys_sys_debug_config0_t*)(SOC_SYS_REG_BASE + (0x38 << 2));
	uint32_t tmp = r->dbug_config0;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg sys_debug_config1:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_sys_debug_config1_value(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_sys_debug_config1_t *r = (sys_sys_debug_config1_t*)(SOC_SYS_REG_BASE + (0x39 << 2));
	r->v = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_sys_debug_config1_value(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_sys_debug_config1_t *r = (sys_sys_debug_config1_t*)(SOC_SYS_REG_BASE + (0x39 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_sys_debug_config1_dbug_config1(uint32_t v) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_sys_debug_config1_t *r = (sys_sys_debug_config1_t*)(SOC_SYS_REG_BASE + (0x39 << 2));
	r->dbug_config1 = v;
	PRRO_SET_SYSTEM_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_sys_debug_config1_dbug_config1(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_sys_debug_config1_t *r = (sys_sys_debug_config1_t*)(SOC_SYS_REG_BASE + (0x39 << 2));
	uint32_t tmp = r->dbug_config1;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg ana_reg0:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg0_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x40 << 2)), v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg0_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x40 << 2));
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg0_dpll_tsten(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 0, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg0_dpll_tsten(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	uint32_t tmp = r->dpll_tsten;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg0_cp(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 1, 0x7, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg0_cp(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	uint32_t tmp = r->cp;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg0_spideten(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 4, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg0_spideten(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	uint32_t tmp = r->spideten;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg0_hvref(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 5, 0x3, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg0_hvref(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	uint32_t tmp = r->hvref;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg0_lvref(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 7, 0x3, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg0_lvref(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	uint32_t tmp = r->lvref;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg0_rzctrl26m(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 9, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg0_rzctrl26m(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	uint32_t tmp = r->rzctrl26m;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg0_looprzctrl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 10, 0xf, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg0_looprzctrl(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	uint32_t tmp = r->looprzctrl;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg0_rpc(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 14, 0x3, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg0_rpc(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	uint32_t tmp = r->rpc;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg0_openloop_en(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 16, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg0_openloop_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	uint32_t tmp = r->openloop_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg0_cksel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 17, 0x3, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg0_cksel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	uint32_t tmp = r->cksel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg0_spitrig(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 19, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg0_spitrig(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	uint32_t tmp = r->spitrig;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg0_band0(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 20, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg0_band0(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	uint32_t tmp = r->band0;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg0_band1(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 21, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg0_band1(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	uint32_t tmp = r->band1;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg0_band(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 22, 0x7, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg0_band(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	uint32_t tmp = r->band;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg0_bandmanual(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 25, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg0_bandmanual(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	uint32_t tmp = r->bandmanual;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg0_dsptrig(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 26, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg0_dsptrig(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	uint32_t tmp = r->dsptrig;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg0_lpen_dpll(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 27, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg0_lpen_dpll(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	uint32_t tmp = r->lpen_dpll;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg0_nc_28_30(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 28, 0x7, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg0_nc_28_30(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	uint32_t tmp = r->nc_28_30;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg0_vctrl_dpllldo(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x40 << 2)), 31, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg0_vctrl_dpllldo(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg0_t *r = (sys_ana_reg0_t*)(SOC_SYS_REG_BASE + (0x40 << 2));
	uint32_t tmp = r->vctrl_dpllldo;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg ana_reg1:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg1_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x41 << 2)), v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg1_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x41 << 2));
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg1_nc3(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 0, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg1_nc2(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 1, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg1_msw(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 2, 0x1ff, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg1_ictrl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 11, 0x7, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg1_osc_trig(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 14, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg1_osccal_trig(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 15, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg1_cnti(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 16, 0x1ff, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg1_spi_rst(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 25, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg1_amsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 26, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg1_divctrl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 27, 0x7, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg1_nc1(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 30, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg1_nc0(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x41 << 2)), 31, 0x1, v);
}

//reg ana_reg2:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg2_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x42 << 2)), v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg2_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x42 << 2));
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg2_xtalh_ctune(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 0, 0xff, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg2_xtalh_ctune(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	uint32_t tmp = r->xtalh_ctune;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg2_force_26mpll(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 8, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg2_force_26mpll(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	uint32_t tmp = r->force_26mpll;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg2_gadc_cmp_ictrl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 9, 0x3, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg2_gadc_cmp_ictrl(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	uint32_t tmp = r->gadc_cmp_ictrl;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg2_gadc_inbuf_ictrl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 11, 0x3, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg2_gadc_inbuf_ictrl(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	uint32_t tmp = r->gadc_inbuf_ictrl;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg2_gadc_refbuf_ictrl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 13, 0x3, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg2_gadc_refbuf_ictrl(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	uint32_t tmp = r->gadc_refbuf_ictrl;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg2_gadc_nobuf_enable(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 15, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg2_gadc_nobuf_enable(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	uint32_t tmp = r->gadc_nobuf_enable;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg2_vref_scale(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 16, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg2_vref_scale(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	uint32_t tmp = r->vref_scale;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg2_scal_en(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 17, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg2_scal_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	uint32_t tmp = r->scal_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg2_gadc_capcal_en(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 18, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg2_gadc_capcal_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	uint32_t tmp = r->gadc_capcal_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg2_gadc_capcal(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 19, 0x3f, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg2_gadc_capcal(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	uint32_t tmp = r->gadc_capcal;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg2_sp_nt_ctrl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x42 << 2)), 25, 0x7f, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg2_sp_nt_ctrl(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg2_t *r = (sys_ana_reg2_t*)(SOC_SYS_REG_BASE + (0x42 << 2));
	uint32_t tmp = r->sp_nt_ctrl;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg ana_reg3:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg3_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x43 << 2)), v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg3_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x43 << 2));
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg3_usbpen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 0, 0xf, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg3_usbnen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 4, 0xf, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg3_hpssren(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 8, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg3_hpssren(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	uint32_t tmp = r->hpssren;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg3_ck_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 9, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg3_ck_sel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	uint32_t tmp = r->ck_sel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg3_anabuf_sel_rx(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 10, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg3_anabuf_sel_rx(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	uint32_t tmp = r->anabuf_sel_rx;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg3_pwd_xtalldo(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 11, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg3_pwd_xtalldo(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	uint32_t tmp = r->pwd_xtalldo;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg3_iamp(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 12, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg3_iamp(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	uint32_t tmp = r->iamp;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg3_vddren(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 13, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg3_vddren(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	uint32_t tmp = r->vddren;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg3_xamp(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 14, 0x3f, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg3_xamp(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	uint32_t tmp = r->xamp;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg3_vosel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 20, 0x1f, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg3_vosel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	uint32_t tmp = r->vosel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg3_en_xtalh_sleep(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 25, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg3_en_xtalh_sleep(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	uint32_t tmp = r->en_xtalh_sleep;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg3_xtal40_en(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 26, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg3_xtal40_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	uint32_t tmp = r->xtal40_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg3_bufictrl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 27, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg3_bufictrl(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	uint32_t tmp = r->bufictrl;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg3_ibias_ctrl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 28, 0x3, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg3_ibias_ctrl(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	uint32_t tmp = r->ibias_ctrl;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg3_icore_ctrl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x43 << 2)), 30, 0x3, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg3_icore_ctrl(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg3_t *r = (sys_ana_reg3_t*)(SOC_SYS_REG_BASE + (0x43 << 2));
	uint32_t tmp = r->icore_ctrl;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg ana_reg4:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg4_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x44 << 2)), v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg4_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x44 << 2));
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg4_anabuf_sel_tx(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 0, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg4_anabuf_sel_tx(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	uint32_t tmp = r->anabuf_sel_tx;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg4_trng_tsten(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 1, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg4_trng_tsten(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	uint32_t tmp = r->trng_tsten;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg4_itune_ref(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 2, 0x7, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg4_itune_ref(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	uint32_t tmp = r->itune_ref;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg4_itune_opa(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 5, 0x7, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg4_itune_opa(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	uint32_t tmp = r->itune_opa;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg4_itune_cmp(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 8, 0x7, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg4_itune_cmp(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	uint32_t tmp = r->itune_cmp;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg4_rnooise_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 11, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg4_rnooise_sel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	uint32_t tmp = r->rnooise_sel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg4_fslow_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 12, 0x7, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg4_fslow_sel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	uint32_t tmp = r->fslow_sel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg4_ffast_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 15, 0xf, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg4_ffast_sel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	uint32_t tmp = r->ffast_sel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg4_trng_tstck_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 19, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg4_trng_tstck_sel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	uint32_t tmp = r->trng_tstck_sel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg4_cktst_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 20, 0x3, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg4_cktst_sel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	uint32_t tmp = r->cktst_sel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg4_ck_tst_enbale(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 22, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg4_ck_tst_enbale(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	uint32_t tmp = r->ck_tst_enbale;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg4_sw_bias(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 23, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg4_sw_bias(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	uint32_t tmp = r->sw_bias;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg4_crb(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 24, 0xf, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg4_crb(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	uint32_t tmp = r->crb;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg4_port_enablel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x44 << 2)), 28, 0xf, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg4_port_enablel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg4_t *r = (sys_ana_reg4_t*)(SOC_SYS_REG_BASE + (0x44 << 2));
	uint32_t tmp = r->port_enablel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg ana_reg5:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg5_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x45 << 2)), v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg5_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x45 << 2));
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg5_en_usb(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 0, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg5_en_usb(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	uint32_t tmp = r->en_usb;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg5_en_xtall(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 1, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg5_en_xtall(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	uint32_t tmp = r->en_xtall;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg5_en_dco(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 2, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg5_en_dco(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	uint32_t tmp = r->en_dco;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg5_en_ram(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 3, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg5_en_ram(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	uint32_t tmp = r->en_ram;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg5_en_temp(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 4, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg5_en_temp(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	uint32_t tmp = r->en_temp;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg5_en_dpll(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 5, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg5_en_dpll(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	uint32_t tmp = r->en_dpll;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg5_en_cb(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 6, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg5_en_cb(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	uint32_t tmp = r->en_cb;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg5_en_lcd(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 7, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg5_en_lcd(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	uint32_t tmp = r->en_lcd;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg5_trxspi_ctrl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 8, 0x3, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg5_trxspi_ctrl(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	uint32_t tmp = r->trxspi_ctrl;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg5_adc_div(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 10, 0x3, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg5_adc_div(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	uint32_t tmp = r->adc_div;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg5_usb_speed(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 12, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg5_usb_speed(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	uint32_t tmp = r->usb_speed;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg5_spideepsleep(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 13, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg5_spideepsleep(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	uint32_t tmp = r->spideepsleep;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg5_vsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 14, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg5_vsel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	uint32_t tmp = r->vsel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg5_swb(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 15, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg5_swb(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	uint32_t tmp = r->swb;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg5_itune_xtall(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 16, 0xf, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg5_itune_xtall(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	uint32_t tmp = r->itune_xtall;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg5_xtall_ten(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 20, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg5_xtall_ten(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	uint32_t tmp = r->xtall_ten;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg5_rosc_tsten(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 21, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg5_rosc_tsten(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	uint32_t tmp = r->rosc_tsten;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg5_bcal_start(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 22, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg5_bcal_start(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	uint32_t tmp = r->bcal_start;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg5_bcal_en(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 23, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg5_bcal_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	uint32_t tmp = r->bcal_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg5_bcal_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 24, 0x7, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg5_bcal_sel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	uint32_t tmp = r->bcal_sel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg5_vbias(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x45 << 2)), 27, 0x1f, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg5_vbias(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg5_t *r = (sys_ana_reg5_t*)(SOC_SYS_REG_BASE + (0x45 << 2));
	uint32_t tmp = r->vbias;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg ana_reg6:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg6_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x46 << 2)), v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg6_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x46 << 2));
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg6_calib_interval(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x46 << 2)), 0, 0x3ff, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg6_calib_interval(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg6_t *r = (sys_ana_reg6_t*)(SOC_SYS_REG_BASE + (0x46 << 2));
	uint32_t tmp = r->calib_interval;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg6_modify_interval(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x46 << 2)), 10, 0x3f, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg6_modify_interval(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg6_t *r = (sys_ana_reg6_t*)(SOC_SYS_REG_BASE + (0x46 << 2));
	uint32_t tmp = r->modify_interval;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg6_xtal_wakeup_time(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x46 << 2)), 16, 0xf, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg6_xtal_wakeup_time(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg6_t *r = (sys_ana_reg6_t*)(SOC_SYS_REG_BASE + (0x46 << 2));
	uint32_t tmp = r->xtal_wakeup_time;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg6_spi_trig(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x46 << 2)), 20, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg6_spi_trig(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg6_t *r = (sys_ana_reg6_t*)(SOC_SYS_REG_BASE + (0x46 << 2));
	uint32_t tmp = r->spi_trig;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg6_modifi_auto(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x46 << 2)), 21, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg6_modifi_auto(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg6_t *r = (sys_ana_reg6_t*)(SOC_SYS_REG_BASE + (0x46 << 2));
	uint32_t tmp = r->modifi_auto;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg6_calib_auto(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x46 << 2)), 22, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg6_calib_auto(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg6_t *r = (sys_ana_reg6_t*)(SOC_SYS_REG_BASE + (0x46 << 2));
	uint32_t tmp = r->calib_auto;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg6_cal_mode(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x46 << 2)), 23, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg6_cal_mode(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg6_t *r = (sys_ana_reg6_t*)(SOC_SYS_REG_BASE + (0x46 << 2));
	uint32_t tmp = r->cal_mode;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg6_manu_ena(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x46 << 2)), 24, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg6_manu_ena(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg6_t *r = (sys_ana_reg6_t*)(SOC_SYS_REG_BASE + (0x46 << 2));
	uint32_t tmp = r->manu_ena;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg6_manu_cin(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x46 << 2)), 25, 0x7f, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg6_manu_cin(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg6_t *r = (sys_ana_reg6_t*)(SOC_SYS_REG_BASE + (0x46 << 2));
	uint32_t tmp = r->manu_cin;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg ana_reg7:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg7_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x47 << 2)), v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg7_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x47 << 2));
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg7_port_enablel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x47 << 2)), 0, 0xffffffff, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg7_port_enablel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg7_t *r = (sys_ana_reg7_t*)(SOC_SYS_REG_BASE + (0x47 << 2));
	uint32_t tmp = r->port_enablel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg ana_reg8:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg8_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x48 << 2)), v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg8_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x48 << 2));
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg8_ioldo_lp(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 0, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg8_ioldo_lp(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	uint32_t tmp = r->ioldo_lp;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg8_coreldo_hp(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 1, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg8_coreldo_hp(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	uint32_t tmp = r->coreldo_hp;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg8_dldohp(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 2, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg8_dldohp(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	uint32_t tmp = r->dldohp;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg8_t_vanaldosel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 3, 0x7, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg8_t_vanaldosel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	uint32_t tmp = r->t_vanaldosel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg8_r_vanaldosel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 6, 0x7, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg8_r_vanaldosel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	uint32_t tmp = r->r_vanaldosel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg8_en_trsw(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 9, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg8_en_trsw(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	uint32_t tmp = r->en_trsw;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg8_aldohp(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 10, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg8_aldohp(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	uint32_t tmp = r->aldohp;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg8_anacurlim(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 11, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg8_anacurlim(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	uint32_t tmp = r->anacurlim;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg8_violdosel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 12, 0x7, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg8_violdosel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	uint32_t tmp = r->violdosel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg8_iocurlim(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 15, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg8_iocurlim(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	uint32_t tmp = r->iocurlim;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg8_valoldosel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 16, 0x7, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg8_valoldosel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	uint32_t tmp = r->valoldosel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg8_alopowsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 19, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg8_alopowsel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	uint32_t tmp = r->alopowsel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg8_en_fast_aloldo(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 20, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg8_en_fast_aloldo(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	uint32_t tmp = r->en_fast_aloldo;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg8_aloldohp(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 21, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg8_aloldohp(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	uint32_t tmp = r->aloldohp;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg8_bgcal(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 22, 0x3f, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg8_bgcal(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	uint32_t tmp = r->bgcal;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg8_vbgcalmode(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 28, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg8_vbgcalmode(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	uint32_t tmp = r->vbgcalmode;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg8_vbgcalstart(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 29, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg8_vbgcalstart(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	uint32_t tmp = r->vbgcalstart;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg8_pwd_bgcal(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 30, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg8_pwd_bgcal(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	uint32_t tmp = r->pwd_bgcal;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg8_spi_envbg(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x48 << 2)), 31, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg8_spi_envbg(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg8_t *r = (sys_ana_reg8_t*)(SOC_SYS_REG_BASE + (0x48 << 2));
	uint32_t tmp = r->spi_envbg;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg ana_reg9:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg9_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x49 << 2)), v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg9_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x49 << 2));
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg9_wkgpiosel1(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 0, 0xf, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg9_wkgpiosel1(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	uint32_t tmp = r->wkgpiosel1;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg9_rst_wks1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 4, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg9_rst_wks1v(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	uint32_t tmp = r->rst_wks1v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg9_wkgpiosel2(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 5, 0xf, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg9_wkgpiosel2(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	uint32_t tmp = r->wkgpiosel2;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg9_spi_latch1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 9, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg9_spi_latch1v(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	uint32_t tmp = r->spi_latch1v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg9_digcurlim(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 10, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg9_digcurlim(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	uint32_t tmp = r->digcurlim;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg9_pupres_enb1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 11, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg9_pupres_enb1v(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	uint32_t tmp = r->pupres_enb1v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg9_pdnres_en1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 12, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg9_pdnres_en1v(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	uint32_t tmp = r->pdnres_en1v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg9_d_veasel1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 13, 0x3, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg9_d_veasel1v(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	uint32_t tmp = r->d_veasel1v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg9_ensfsdd(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 15, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg9_ensfsdd(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	uint32_t tmp = r->ensfsdd;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg9_vcorehsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 16, 0xf, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg9_vcorehsel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	uint32_t tmp = r->vcorehsel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg9_vcorelsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 20, 0x7, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg9_vcorelsel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	uint32_t tmp = r->vcorelsel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg9_vlden(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 23, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg9_vlden(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	uint32_t tmp = r->vlden;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg9_en_fast_coreldo(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 24, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg9_en_fast_coreldo(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	uint32_t tmp = r->en_fast_coreldo;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg9_pwdcoreldo(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 25, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg9_pwdcoreldo(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	uint32_t tmp = r->pwdcoreldo;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg9_vdighsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 26, 0x7, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg9_vdighsel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	uint32_t tmp = r->vdighsel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg9_vdigsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 29, 0x3, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg9_vdigsel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	uint32_t tmp = r->vdigsel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg9_vdd12lden(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x49 << 2)), 31, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg9_vdd12lden(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg9_t *r = (sys_ana_reg9_t*)(SOC_SYS_REG_BASE + (0x49 << 2));
	uint32_t tmp = r->vdd12lden;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg ana_reg10:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg10_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x4a << 2)), v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg10_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x4a << 2));
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg10_pasoft_st(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 0, 0xf, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg10_pasoft_st(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	uint32_t tmp = r->pasoft_st;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg10_azcdcnt_manu(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 4, 0x7, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg10_azcdcnt_manu(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	uint32_t tmp = r->azcdcnt_manu;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg10_timer_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 7, 0xf, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg10_timer_sel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	uint32_t tmp = r->timer_sel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg10_vpabucksel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 11, 0xf, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg10_vpabucksel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	uint32_t tmp = r->vpabucksel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg10_spi_timerwken(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 15, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg10_spi_timerwken(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	uint32_t tmp = r->spi_timerwken;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg10_nc_16_16(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 16, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg10_nc_16_16(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	uint32_t tmp = r->nc_16_16;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg10_sd(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 17, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg10_sd(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	uint32_t tmp = r->sd;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg10_ioldosel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 18, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg10_ioldosel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	uint32_t tmp = r->ioldosel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg10_iobyapssen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 19, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg10_iobyapssen(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	uint32_t tmp = r->iobyapssen;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg10_ckfs(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 20, 0x3, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg10_ckfs(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	uint32_t tmp = r->ckfs;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg10_ckintsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 22, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg10_ckintsel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	uint32_t tmp = r->ckintsel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg10_osccaltrig(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 23, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg10_osccaltrig(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	uint32_t tmp = r->osccaltrig;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg10_mroscsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 24, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg10_mroscsel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	uint32_t tmp = r->mroscsel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg10_mrosci_cal(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 25, 0x7, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg10_mrosci_cal(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	uint32_t tmp = r->mrosci_cal;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg10_mrosccap_cal(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4a << 2)), 28, 0xf, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg10_mrosccap_cal(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg10_t *r = (sys_ana_reg10_t*)(SOC_SYS_REG_BASE + (0x4a << 2));
	uint32_t tmp = r->mrosccap_cal;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg ana_reg11:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg11_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x4b << 2)), v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg11_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x4b << 2));
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg11_sfsr(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 0, 0xf, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg11_sfsr(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	uint32_t tmp = r->sfsr;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg11_ensfsaa(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 4, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg11_ensfsaa(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	uint32_t tmp = r->ensfsaa;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg11_apfms(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 5, 0x1f, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg11_apfms(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	uint32_t tmp = r->apfms;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg11_atmpo_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 10, 0x3, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg11_atmpo_sel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	uint32_t tmp = r->atmpo_sel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg11_ampoen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 12, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg11_ampoen(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	uint32_t tmp = r->ampoen;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg11_enpowa(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 13, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg11_enpowa(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	uint32_t tmp = r->enpowa;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg11_avea_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 14, 0x3, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg11_avea_sel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	uint32_t tmp = r->avea_sel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg11_aforcepfm(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 16, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg11_aforcepfm(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	uint32_t tmp = r->aforcepfm;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg11_acls(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 17, 0x7, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg11_acls(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	uint32_t tmp = r->acls;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg11_aswrsten(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 20, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg11_aswrsten(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	uint32_t tmp = r->aswrsten;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg11_aripc(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 21, 0x7, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg11_aripc(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	uint32_t tmp = r->aripc;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg11_arampc(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 24, 0xf, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg11_arampc(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	uint32_t tmp = r->arampc;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg11_arampcen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 28, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg11_arampcen(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	uint32_t tmp = r->arampcen;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg11_aenburst(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 29, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg11_aenburst(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	uint32_t tmp = r->aenburst;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg11_apfmen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 30, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg11_apfmen(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	uint32_t tmp = r->apfmen;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg11_aldosel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4b << 2)), 31, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg11_aldosel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg11_t *r = (sys_ana_reg11_t*)(SOC_SYS_REG_BASE + (0x4b << 2));
	uint32_t tmp = r->aldosel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg ana_reg12:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg12_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x4c << 2)), v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg12_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x4c << 2));
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg12_buckd_softst(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4c << 2)), 0, 0xf, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg12_buckd_softst(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg12_t *r = (sys_ana_reg12_t*)(SOC_SYS_REG_BASE + (0x4c << 2));
	uint32_t tmp = r->buckd_softst;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg12_dzcdcnt_manu(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4c << 2)), 4, 0x7, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg12_dzcdcnt_manu(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg12_t *r = (sys_ana_reg12_t*)(SOC_SYS_REG_BASE + (0x4c << 2));
	uint32_t tmp = r->dzcdcnt_manu;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg12_clk_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4c << 2)), 7, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg12_clk_sel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg12_t *r = (sys_ana_reg12_t*)(SOC_SYS_REG_BASE + (0x4c << 2));
	uint32_t tmp = r->clk_sel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg12_dpfms(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4c << 2)), 8, 0x1f, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg12_dpfms(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg12_t *r = (sys_ana_reg12_t*)(SOC_SYS_REG_BASE + (0x4c << 2));
	uint32_t tmp = r->dpfms;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg12_dtmpo_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4c << 2)), 13, 0x3, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg12_dtmpo_sel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg12_t *r = (sys_ana_reg12_t*)(SOC_SYS_REG_BASE + (0x4c << 2));
	uint32_t tmp = r->dtmpo_sel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg12_dmpoen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4c << 2)), 15, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg12_dmpoen(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg12_t *r = (sys_ana_reg12_t*)(SOC_SYS_REG_BASE + (0x4c << 2));
	uint32_t tmp = r->dmpoen;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg12_dforcepfm(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4c << 2)), 16, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg12_dforcepfm(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg12_t *r = (sys_ana_reg12_t*)(SOC_SYS_REG_BASE + (0x4c << 2));
	uint32_t tmp = r->dforcepfm;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg12_dcls(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4c << 2)), 17, 0x7, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg12_dcls(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg12_t *r = (sys_ana_reg12_t*)(SOC_SYS_REG_BASE + (0x4c << 2));
	uint32_t tmp = r->dcls;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg12_dswrsten(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4c << 2)), 20, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg12_dswrsten(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg12_t *r = (sys_ana_reg12_t*)(SOC_SYS_REG_BASE + (0x4c << 2));
	uint32_t tmp = r->dswrsten;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg12_dripc(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4c << 2)), 21, 0x7, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg12_dripc(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg12_t *r = (sys_ana_reg12_t*)(SOC_SYS_REG_BASE + (0x4c << 2));
	uint32_t tmp = r->dripc;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg12_drampc(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4c << 2)), 24, 0xf, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg12_drampc(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg12_t *r = (sys_ana_reg12_t*)(SOC_SYS_REG_BASE + (0x4c << 2));
	uint32_t tmp = r->drampc;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg12_drampcen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4c << 2)), 28, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg12_drampcen(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg12_t *r = (sys_ana_reg12_t*)(SOC_SYS_REG_BASE + (0x4c << 2));
	uint32_t tmp = r->drampcen;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg12_denburst(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4c << 2)), 29, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg12_denburst(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg12_t *r = (sys_ana_reg12_t*)(SOC_SYS_REG_BASE + (0x4c << 2));
	uint32_t tmp = r->denburst;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg12_dpfmen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4c << 2)), 30, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg12_dpfmen(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg12_t *r = (sys_ana_reg12_t*)(SOC_SYS_REG_BASE + (0x4c << 2));
	uint32_t tmp = r->dpfmen;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg12_dldosel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4c << 2)), 31, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg12_dldosel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg12_t *r = (sys_ana_reg12_t*)(SOC_SYS_REG_BASE + (0x4c << 2));
	uint32_t tmp = r->dldosel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg ana_reg13:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg13_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x4d << 2)), v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg13_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x4d << 2));
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg13_pwdovp1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4d << 2)), 0, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg13_pwdovp1v(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg13_t *r = (sys_ana_reg13_t*)(SOC_SYS_REG_BASE + (0x4d << 2));
	uint32_t tmp = r->pwdovp1v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg13_asoft_stc(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4d << 2)), 1, 0xf, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg13_asoft_stc(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg13_t *r = (sys_ana_reg13_t*)(SOC_SYS_REG_BASE + (0x4d << 2));
	uint32_t tmp = r->asoft_stc;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg13_volen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4d << 2)), 5, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg13_volen(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg13_t *r = (sys_ana_reg13_t*)(SOC_SYS_REG_BASE + (0x4d << 2));
	uint32_t tmp = r->volen;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg13_dpfms(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4d << 2)), 6, 0x1f, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg13_dpfms(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg13_t *r = (sys_ana_reg13_t*)(SOC_SYS_REG_BASE + (0x4d << 2));
	uint32_t tmp = r->dpfms;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg13_dtmpo_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4d << 2)), 11, 0x3, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg13_dtmpo_sel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg13_t *r = (sys_ana_reg13_t*)(SOC_SYS_REG_BASE + (0x4d << 2));
	uint32_t tmp = r->dtmpo_sel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg13_dmpoen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4d << 2)), 13, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg13_dmpoen(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg13_t *r = (sys_ana_reg13_t*)(SOC_SYS_REG_BASE + (0x4d << 2));
	uint32_t tmp = r->dmpoen;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg13_pavea_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4d << 2)), 14, 0x3, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg13_pavea_sel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg13_t *r = (sys_ana_reg13_t*)(SOC_SYS_REG_BASE + (0x4d << 2));
	uint32_t tmp = r->pavea_sel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg13_dforcepfm(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4d << 2)), 16, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg13_dforcepfm(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg13_t *r = (sys_ana_reg13_t*)(SOC_SYS_REG_BASE + (0x4d << 2));
	uint32_t tmp = r->dforcepfm;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg13_dcls(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4d << 2)), 17, 0x7, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg13_dcls(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg13_t *r = (sys_ana_reg13_t*)(SOC_SYS_REG_BASE + (0x4d << 2));
	uint32_t tmp = r->dcls;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg13_dswrsten(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4d << 2)), 20, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg13_dswrsten(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg13_t *r = (sys_ana_reg13_t*)(SOC_SYS_REG_BASE + (0x4d << 2));
	uint32_t tmp = r->dswrsten;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg13_dripc(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4d << 2)), 21, 0x7, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg13_dripc(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg13_t *r = (sys_ana_reg13_t*)(SOC_SYS_REG_BASE + (0x4d << 2));
	uint32_t tmp = r->dripc;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg13_drampc(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4d << 2)), 24, 0xf, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg13_drampc(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg13_t *r = (sys_ana_reg13_t*)(SOC_SYS_REG_BASE + (0x4d << 2));
	uint32_t tmp = r->drampc;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg13_drampcen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4d << 2)), 28, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg13_drampcen(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg13_t *r = (sys_ana_reg13_t*)(SOC_SYS_REG_BASE + (0x4d << 2));
	uint32_t tmp = r->drampcen;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg13_paenburst(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4d << 2)), 29, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg13_paenburst(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg13_t *r = (sys_ana_reg13_t*)(SOC_SYS_REG_BASE + (0x4d << 2));
	uint32_t tmp = r->paenburst;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg13_papfmen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4d << 2)), 30, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg13_papfmen(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg13_t *r = (sys_ana_reg13_t*)(SOC_SYS_REG_BASE + (0x4d << 2));
	uint32_t tmp = r->papfmen;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg13_enbuckpa(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4d << 2)), 31, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg13_enbuckpa(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg13_t *r = (sys_ana_reg13_t*)(SOC_SYS_REG_BASE + (0x4d << 2));
	uint32_t tmp = r->enbuckpa;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg ana_reg14:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg14_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x4e << 2)), v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg14_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x4e << 2));
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg14_chs(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4e << 2)), 0, 0xffff, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg14_en_lpmod(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4e << 2)), 16, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg14_cal_vth(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4e << 2)), 17, 0x7, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg14_crg(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4e << 2)), 20, 0x3, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg14_vrefs(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4e << 2)), 22, 0xf, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg14_gain_s(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4e << 2)), 26, 0xf, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg14_td_latch1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4e << 2)), 30, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg14_pwd_td(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4e << 2)), 31, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg14_en_seri_cap(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4e << 2)), 14, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg14_sel_seri_cap(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4e << 2)), 13, 0x1, v);
}

//reg ana_reg15:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg15_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x4f << 2)), v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg15_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x4f << 2));
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg15_cal_number1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4f << 2)), 0, 0xf, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg15_cal_period1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4f << 2)), 4, 0x1ff, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg15_test_number1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4f << 2)), 13, 0xf, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg15_test_period1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4f << 2)), 17, 0xf, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg15_chs(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4f << 2)), 8, 0xffff, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg15_nc_21_21(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4f << 2)), 21, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg15_chs_sel_cal1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4f << 2)), 22, 0xf, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg15_cal_done_clr1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4f << 2)), 26, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg15_en_cal_force1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4f << 2)), 27, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg15_en_cal_auto1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4f << 2)), 28, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg15_en_scm(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4f << 2)), 29, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg15_en_adcmod(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4f << 2)), 30, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg15_enfsr1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x4f << 2)), 31, 0x1, v);
}

//reg ana_reg16:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg16_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x50 << 2)), v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg16_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x50 << 2));
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg16_int_en(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x50 << 2)), 0, 0x3ff, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg16_int_en16(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x50 << 2)), 10, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg16_nc_11_15(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x50 << 2)), 11, 0x1f, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg16_ckadc_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x50 << 2)), 16, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg16_int_clr_sel1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x50 << 2)), 17, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg16_ctrl_ck2d(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x50 << 2)), 18, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg16_ctrl_seri_cap(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x50 << 2)), 19, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg16_en_testcmp1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x50 << 2)), 20, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg16_en_man_wr1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x50 << 2)), 21, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg16_en_manmod1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x50 << 2)), 22, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg16_cap_calspi1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x50 << 2)), 23, 0x1ff, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg16_cal_period1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x50 << 2)), 23, 0x1ff, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg16_cal_number1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x50 << 2)), 19, 0xf, v);
}

//reg ana_reg17:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg17_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x51 << 2)), v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg17_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x51 << 2));
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg17_int_clr(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x51 << 2)), 0, 0x3ff, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg17_int_clr16(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x51 << 2)), 10, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg17_nc_11_11(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x51 << 2)), 11, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg17_int_clr_cal(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x51 << 2)), 12, 0x3ff, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg17_int_en_cal(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x51 << 2)), 22, 0x3ff, v);
}

//reg ana_reg18:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg18_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x52 << 2)), v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg18_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x52 << 2));
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg18_iselaud(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x52 << 2)), 0, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg18_iselaud(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg18_t *r = (sys_ana_reg18_t*)(SOC_SYS_REG_BASE + (0x52 << 2));
	uint32_t tmp = r->iselaud;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg18_audck_rlcen1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x52 << 2)), 1, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg18_audck_rlcen1v(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg18_t *r = (sys_ana_reg18_t*)(SOC_SYS_REG_BASE + (0x52 << 2));
	uint32_t tmp = r->audck_rlcen1v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg18_lchckinven1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x52 << 2)), 2, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg18_lchckinven1v(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg18_t *r = (sys_ana_reg18_t*)(SOC_SYS_REG_BASE + (0x52 << 2));
	uint32_t tmp = r->lchckinven1v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg18_enaudbias(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x52 << 2)), 3, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg18_enaudbias(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg18_t *r = (sys_ana_reg18_t*)(SOC_SYS_REG_BASE + (0x52 << 2));
	uint32_t tmp = r->enaudbias;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg18_enadcbias(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x52 << 2)), 4, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg18_enadcbias(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg18_t *r = (sys_ana_reg18_t*)(SOC_SYS_REG_BASE + (0x52 << 2));
	uint32_t tmp = r->enadcbias;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg18_enmicbias(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x52 << 2)), 5, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg18_enmicbias(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg18_t *r = (sys_ana_reg18_t*)(SOC_SYS_REG_BASE + (0x52 << 2));
	uint32_t tmp = r->enmicbias;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg18_adcckinven1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x52 << 2)), 6, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg18_adcckinven1v(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg18_t *r = (sys_ana_reg18_t*)(SOC_SYS_REG_BASE + (0x52 << 2));
	uint32_t tmp = r->adcckinven1v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg18_dacfb2st0v9(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x52 << 2)), 7, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg18_dacfb2st0v9(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg18_t *r = (sys_ana_reg18_t*)(SOC_SYS_REG_BASE + (0x52 << 2));
	uint32_t tmp = r->dacfb2st0v9;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg18_nc1(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x52 << 2)), 8, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg18_nc1(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg18_t *r = (sys_ana_reg18_t*)(SOC_SYS_REG_BASE + (0x52 << 2));
	uint32_t tmp = r->nc1;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg18_micbias_trm(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x52 << 2)), 9, 0x3, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg18_micbias_trm(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg18_t *r = (sys_ana_reg18_t*)(SOC_SYS_REG_BASE + (0x52 << 2));
	uint32_t tmp = r->micbias_trm;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg18_micbias_voc(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x52 << 2)), 11, 0x1f, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg18_micbias_voc(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg18_t *r = (sys_ana_reg18_t*)(SOC_SYS_REG_BASE + (0x52 << 2));
	uint32_t tmp = r->micbias_voc;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg18_vrefsel1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x52 << 2)), 16, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg18_vrefsel1v(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg18_t *r = (sys_ana_reg18_t*)(SOC_SYS_REG_BASE + (0x52 << 2));
	uint32_t tmp = r->vrefsel1v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg18_capswspi(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x52 << 2)), 17, 0x1f, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg18_capswspi(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg18_t *r = (sys_ana_reg18_t*)(SOC_SYS_REG_BASE + (0x52 << 2));
	uint32_t tmp = r->capswspi;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg18_adref_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x52 << 2)), 22, 0x3, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg18_adref_sel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg18_t *r = (sys_ana_reg18_t*)(SOC_SYS_REG_BASE + (0x52 << 2));
	uint32_t tmp = r->adref_sel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg18_nc0(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x52 << 2)), 24, 0x3, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg18_nc0(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg18_t *r = (sys_ana_reg18_t*)(SOC_SYS_REG_BASE + (0x52 << 2));
	uint32_t tmp = r->nc0;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg18_spi_dacckpssel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x52 << 2)), 31, 0x1, v);
}

//reg ana_reg19:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg19_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x53 << 2)), v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg19_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x53 << 2));
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg19_isel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 0, 0x3, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg19_isel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	uint32_t tmp = r->isel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg19_micirsel1(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 2, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg19_micirsel1(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	uint32_t tmp = r->micirsel1;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg19_micdacit(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 3, 0x3, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg19_micdacit(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	uint32_t tmp = r->micdacit;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg19_micdacih(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 5, 0xff, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg19_micdacih(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	uint32_t tmp = r->micdacih;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg19_micsingleen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 13, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg19_micsingleen(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	uint32_t tmp = r->micsingleen;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg19_dccompen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 14, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg19_dccompen(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	uint32_t tmp = r->dccompen;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg19_micgain(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 15, 0xf, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg19_micgain(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	uint32_t tmp = r->micgain;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg19_micdacen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 19, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg19_micdacen(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	uint32_t tmp = r->micdacen;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg19_stg2lsen1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 20, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg19_stg2lsen1v(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	uint32_t tmp = r->stg2lsen1v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg19_openloopcal1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 21, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg19_openloopcal1v(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	uint32_t tmp = r->openloopcal1v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg19_callatch(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 22, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg19_callatch(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	uint32_t tmp = r->callatch;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg19_vcmsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 23, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg19_vcmsel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	uint32_t tmp = r->vcmsel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg19_dwamode(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 24, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg19_dwamode(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	uint32_t tmp = r->dwamode;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg19_r2ren(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 25, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg19_r2ren(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	uint32_t tmp = r->r2ren;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg19_nc_26_27(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 26, 0x3, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg19_nc_26_27(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	uint32_t tmp = r->nc_26_27;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg19_micen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 28, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg19_micen(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	uint32_t tmp = r->micen;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg19_rst(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 29, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg19_rst(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	uint32_t tmp = r->rst;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg19_bpdwa1v(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 30, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg19_bpdwa1v(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	uint32_t tmp = r->bpdwa1v;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg19_hcen1stg(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x53 << 2)), 31, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg19_hcen1stg(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg19_t *r = (sys_ana_reg19_t*)(SOC_SYS_REG_BASE + (0x53 << 2));
	uint32_t tmp = r->hcen1stg;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg ana_reg20:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg20_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x54 << 2)), v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg20_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x54 << 2));
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg20_hpdac(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 0, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg20_hpdac(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	uint32_t tmp = r->hpdac;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg20_calcon_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 1, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg20_calcon_sel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	uint32_t tmp = r->calcon_sel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg20_oscdac(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 2, 0x3, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg20_oscdac(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	uint32_t tmp = r->oscdac;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg20_ocendac(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 4, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg20_ocendac(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	uint32_t tmp = r->ocendac;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg20_vcmsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 5, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg20_vcmsel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	uint32_t tmp = r->vcmsel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg20_adjdacref(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 6, 0x1f, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg20_adjdacref(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	uint32_t tmp = r->adjdacref;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg20_dcochg(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 11, 0x3, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg20_dcochg(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	uint32_t tmp = r->dcochg;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg20_diffen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 13, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg20_diffen(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	uint32_t tmp = r->diffen;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg20_endaccal(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 14, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg20_endaccal(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	uint32_t tmp = r->endaccal;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg20_nc2(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 15, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg20_nc2(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	uint32_t tmp = r->nc2;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg20_lendcoc(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 16, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg20_lendcoc(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	uint32_t tmp = r->lendcoc;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg20_nc1(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 17, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg20_nc1(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	uint32_t tmp = r->nc1;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg20_lenvcmd(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 18, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg20_lenvcmd(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	uint32_t tmp = r->lenvcmd;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg20_dacdrven(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 19, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg20_dacdrven(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	uint32_t tmp = r->dacdrven;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg20_nc0(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 20, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg20_nc0(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	uint32_t tmp = r->nc0;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg20_daclen(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 21, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg20_daclen(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	uint32_t tmp = r->daclen;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg20_dacg(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 22, 0xf, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg20_dacg(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	uint32_t tmp = r->dacg;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg20_dacmute(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 26, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg20_dacmute(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	uint32_t tmp = r->dacmute;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg20_dacdwamode_sel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 27, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg20_dacdwamode_sel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	uint32_t tmp = r->dacdwamode_sel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg20_dacsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x54 << 2)), 28, 0xf, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg20_dacsel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg20_t *r = (sys_ana_reg20_t*)(SOC_SYS_REG_BASE + (0x54 << 2));
	uint32_t tmp = r->dacsel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg ana_reg21:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg21_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x55 << 2)), v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg21_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x55 << 2));
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg21_lmdcin(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x55 << 2)), 0, 0xff, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg21_lmdcin(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg21_t *r = (sys_ana_reg21_t*)(SOC_SYS_REG_BASE + (0x55 << 2));
	uint32_t tmp = r->lmdcin;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg21_nc1(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x55 << 2)), 8, 0xff, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg21_nc1(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg21_t *r = (sys_ana_reg21_t*)(SOC_SYS_REG_BASE + (0x55 << 2));
	uint32_t tmp = r->nc1;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg21_spirst_ovc(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x55 << 2)), 16, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg21_spirst_ovc(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg21_t *r = (sys_ana_reg21_t*)(SOC_SYS_REG_BASE + (0x55 << 2));
	uint32_t tmp = r->spirst_ovc;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg21_nc0(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x55 << 2)), 17, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg21_nc0(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg21_t *r = (sys_ana_reg21_t*)(SOC_SYS_REG_BASE + (0x55 << 2));
	uint32_t tmp = r->nc0;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg21_enidacl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x55 << 2)), 18, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg21_enidacl(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg21_t *r = (sys_ana_reg21_t*)(SOC_SYS_REG_BASE + (0x55 << 2));
	uint32_t tmp = r->enidacl;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg21_dac3rdhc0v9(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x55 << 2)), 19, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg21_dac3rdhc0v9(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg21_t *r = (sys_ana_reg21_t*)(SOC_SYS_REG_BASE + (0x55 << 2));
	uint32_t tmp = r->dac3rdhc0v9;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg21_hc2s(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x55 << 2)), 20, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg21_hc2s(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg21_t *r = (sys_ana_reg21_t*)(SOC_SYS_REG_BASE + (0x55 << 2));
	uint32_t tmp = r->hc2s;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg21_rfb_ctrl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x55 << 2)), 21, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg21_rfb_ctrl(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg21_t *r = (sys_ana_reg21_t*)(SOC_SYS_REG_BASE + (0x55 << 2));
	uint32_t tmp = r->rfb_ctrl;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg21_vcmsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x55 << 2)), 22, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg21_vcmsel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg21_t *r = (sys_ana_reg21_t*)(SOC_SYS_REG_BASE + (0x55 << 2));
	uint32_t tmp = r->vcmsel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg21_enbs(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x55 << 2)), 23, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg21_enbs(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg21_t *r = (sys_ana_reg21_t*)(SOC_SYS_REG_BASE + (0x55 << 2));
	uint32_t tmp = r->enbs;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg21_calck_sel0v9(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x55 << 2)), 24, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg21_calck_sel0v9(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg21_t *r = (sys_ana_reg21_t*)(SOC_SYS_REG_BASE + (0x55 << 2));
	uint32_t tmp = r->calck_sel0v9;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg21_bpdwa0v9(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x55 << 2)), 25, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg21_bpdwa0v9(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg21_t *r = (sys_ana_reg21_t*)(SOC_SYS_REG_BASE + (0x55 << 2));
	uint32_t tmp = r->bpdwa0v9;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg21_looprst0v9(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x55 << 2)), 26, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg21_looprst0v9(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg21_t *r = (sys_ana_reg21_t*)(SOC_SYS_REG_BASE + (0x55 << 2));
	uint32_t tmp = r->looprst0v9;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg21_oct0v9(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x55 << 2)), 27, 0x3, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg21_oct0v9(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg21_t *r = (sys_ana_reg21_t*)(SOC_SYS_REG_BASE + (0x55 << 2));
	uint32_t tmp = r->oct0v9;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg21_sout0v9(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x55 << 2)), 29, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg21_sout0v9(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg21_t *r = (sys_ana_reg21_t*)(SOC_SYS_REG_BASE + (0x55 << 2));
	uint32_t tmp = r->sout0v9;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg21_hc0v9(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x55 << 2)), 30, 0x3, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg21_hc0v9(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg21_t *r = (sys_ana_reg21_t*)(SOC_SYS_REG_BASE + (0x55 << 2));
	uint32_t tmp = r->hc0v9;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg ana_reg22:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg22_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x56 << 2)), v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg22_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x56 << 2));
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg22_ictrl_dsppll(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x56 << 2)), 0, 0xf, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg22_ictrl_dsppll(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg22_t *r = (sys_ana_reg22_t*)(SOC_SYS_REG_BASE + (0x56 << 2));
	uint32_t tmp = r->ictrl_dsppll;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg22_nc_4_18(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x56 << 2)), 4, 0x7fff, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg22_nc_4_18(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg22_t *r = (sys_ana_reg22_t*)(SOC_SYS_REG_BASE + (0x56 << 2));
	uint32_t tmp = r->nc_4_18;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg22_mode(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x56 << 2)), 19, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg22_mode(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg22_t *r = (sys_ana_reg22_t*)(SOC_SYS_REG_BASE + (0x56 << 2));
	uint32_t tmp = r->mode;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg22_iamsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x56 << 2)), 20, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg22_iamsel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg22_t *r = (sys_ana_reg22_t*)(SOC_SYS_REG_BASE + (0x56 << 2));
	uint32_t tmp = r->iamsel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg22_hvref(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x56 << 2)), 21, 0x3, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg22_hvref(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg22_t *r = (sys_ana_reg22_t*)(SOC_SYS_REG_BASE + (0x56 << 2));
	uint32_t tmp = r->hvref;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg22_lvref(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x56 << 2)), 23, 0x3, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg22_lvref(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg22_t *r = (sys_ana_reg22_t*)(SOC_SYS_REG_BASE + (0x56 << 2));
	uint32_t tmp = r->lvref;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg22_nc_25_31(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x56 << 2)), 25, 0x7f, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg22_nc_25_31(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg22_t *r = (sys_ana_reg22_t*)(SOC_SYS_REG_BASE + (0x56 << 2));
	uint32_t tmp = r->nc_25_31;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

//reg ana_reg23:

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg23_value(uint32_t v) {
	sys_ll_set_analog_reg_value((SOC_SYS_REG_BASE + (0x57 << 2)), v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg23_value(void) {
	return sys_ll_get_analog_reg_value(SOC_SYS_REG_BASE + (0x57 << 2));
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg23_camsel(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x57 << 2)), 0, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg23_camsel(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg23_t *r = (sys_ana_reg23_t*)(SOC_SYS_REG_BASE + (0x57 << 2));
	uint32_t tmp = r->camsel;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg23_msw(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x57 << 2)), 1, 0x1ff, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg23_msw(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg23_t *r = (sys_ana_reg23_t*)(SOC_SYS_REG_BASE + (0x57 << 2));
	uint32_t tmp = r->msw;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg23_tstcken_dpll(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x57 << 2)), 10, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg23_tstcken_dpll(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg23_t *r = (sys_ana_reg23_t*)(SOC_SYS_REG_BASE + (0x57 << 2));
	uint32_t tmp = r->tstcken_dpll;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg23_osccal_trig(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x57 << 2)), 11, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg23_osccal_trig(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg23_t *r = (sys_ana_reg23_t*)(SOC_SYS_REG_BASE + (0x57 << 2));
	uint32_t tmp = r->osccal_trig;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg23_cnti(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x57 << 2)), 12, 0x1ff, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg23_cnti(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg23_t *r = (sys_ana_reg23_t*)(SOC_SYS_REG_BASE + (0x57 << 2));
	uint32_t tmp = r->cnti;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg23_nc_21_21(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x57 << 2)), 21, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg23_nc_21_21(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg23_t *r = (sys_ana_reg23_t*)(SOC_SYS_REG_BASE + (0x57 << 2));
	uint32_t tmp = r->nc_21_21;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg23_spi_rst(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x57 << 2)), 22, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg23_spi_rst(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg23_t *r = (sys_ana_reg23_t*)(SOC_SYS_REG_BASE + (0x57 << 2));
	uint32_t tmp = r->spi_rst;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg23_closeloop_en(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x57 << 2)), 23, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg23_closeloop_en(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg23_t *r = (sys_ana_reg23_t*)(SOC_SYS_REG_BASE + (0x57 << 2));
	uint32_t tmp = r->closeloop_en;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg23_caltime(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x57 << 2)), 24, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg23_caltime(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg23_t *r = (sys_ana_reg23_t*)(SOC_SYS_REG_BASE + (0x57 << 2));
	uint32_t tmp = r->caltime;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg23_lpfrz(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x57 << 2)), 25, 0x3, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg23_lpfrz(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg23_t *r = (sys_ana_reg23_t*)(SOC_SYS_REG_BASE + (0x57 << 2));
	uint32_t tmp = r->lpfrz;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg23_icp(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x57 << 2)), 27, 0xf, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg23_icp(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg23_t *r = (sys_ana_reg23_t*)(SOC_SYS_REG_BASE + (0x57 << 2));
	uint32_t tmp = r->icp;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void sys_ll_set_ana_reg23_cp2ctrl(uint32_t v) {
	sys_set_ana_reg_bit((SOC_SYS_REG_BASE + (0x57 << 2)), 31, 0x1, v);
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t sys_ll_get_ana_reg23_cp2ctrl(void) {
	PRRO_SYSTEM_SECURE_DECLARATION();
	PRRO_SET_SYSTEM_SECURE();
	sys_ana_reg23_t *r = (sys_ana_reg23_t*)(SOC_SYS_REG_BASE + (0x57 << 2));
	uint32_t tmp = r->cp2ctrl;
	PRRO_SET_SYSTEM_NON_SECURE();
	return tmp;
}
