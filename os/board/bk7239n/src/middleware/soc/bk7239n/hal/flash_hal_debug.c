// Copyright 2020-2021 Beken
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

#include "hal_config.h"
#include "flash_hal.h"
#include "flash_hw.h"
#include "flash_ll.h"

#if CFG_HAL_DEBUG_FLASH

void flash_struct_dump(void)
{
	flash_hw_t *hw = (flash_hw_t *)FLASH_LL_REG_BASE(0);
	SOC_LOGI("base=%x\r\n", (uint32_t)hw);

	/* REG_0x0 */
	SOC_LOGI("  dev_id=0x%x value=0x%x\n", &hw->dev_id, hw->dev_id);

	/* REG_0x1 */
	SOC_LOGI("  dev_version=0x%x value=0x%x\n", &hw->dev_version, hw->dev_version);

	/* REG_0x2 */
	SOC_LOGI("  global_ctrl=0x%x value=0x%x\n", &hw->global_ctrl, hw->global_ctrl.v);
	SOC_LOGI("    soft_reset:      %x\n", hw->global_ctrl.soft_reset);
	SOC_LOGI("    clk_gate_bypass: %x\n", hw->global_ctrl.clk_gate_bypass);
	SOC_LOGI("    reserved:        %x\n", hw->global_ctrl.reserved);
	SOC_LOGI("\r\n");

	/* REG_0x3 */
	SOC_LOGI("  global_status=0x%x value=0x%x\n", &hw->global_status, hw->global_status);
	SOC_LOGI("\r\n");

	/* REG_0x4 */
	SOC_LOGI("  op_ctrl=0x%x value=0x%x\n", &hw->op_ctrl, hw->op_ctrl.v);
	SOC_LOGI("    reserved: %x\n", hw->op_ctrl.reserved);
	SOC_LOGI("    op_sw:    %x\n", hw->op_ctrl.op_sw);
	SOC_LOGI("    wp_value: %x\n", hw->op_ctrl.wp_value);
	SOC_LOGI("    busy_sw:  %x\n", hw->op_ctrl.busy_sw);
	SOC_LOGI("\r\n");

	/* REG_0x5, data to be written from software to flash */
	SOC_LOGI("  data_sw_flash=0x%x value=0x%x\n", &hw->data_sw_flash, hw->data_sw_flash);

	/* REG_0x6, data read from flash to software */
	SOC_LOGI("  data_flash_sw=0x%x value=0x%x\n", &hw->data_flash_sw, hw->data_flash_sw);
	SOC_LOGI("\r\n");

	/* REG_0x7 */
	SOC_LOGI("  cmd_cfg=0x%x value=0x%x\n", &hw->cmd_cfg, hw->cmd_cfg.v);
	SOC_LOGI("    wrsr_cmd_reg: %x\n", hw->cmd_cfg.wrsr_cmd_reg);
	SOC_LOGI("    rdsr_cmd_reg: %x\n", hw->cmd_cfg.rdsr_cmd_reg);
	SOC_LOGI("    wrsr_cmd_sel: %x\n", hw->cmd_cfg.wrsr_cmd_sel);
	SOC_LOGI("    rdsr_cmd_sel: %x\n", hw->cmd_cfg.rdsr_cmd_sel);
	SOC_LOGI("    reserved:     %x\n", hw->cmd_cfg.reserved);
	SOC_LOGI("\r\n");

	/* REG_0x8 */
	SOC_LOGI("  rd_flash_id=0x%x value=0x%x\n", &hw->rd_flash_id, hw->rd_flash_id);
	SOC_LOGI("\r\n");

	/* REG_0x9 */
	SOC_LOGI("  state=0x%x value=0x%x\n", &hw->state, hw->state.v);
	SOC_LOGI("    status_reg:  %x\n", hw->state.status_reg);
	SOC_LOGI("    crc_err_num: %x\n", hw->state.crc_err_num);
	SOC_LOGI("    byte_sel_wr: %x\n", hw->state.byte_sel_wr);
	SOC_LOGI("    byte_sel_rd: %x\n", hw->state.byte_sel_rd);
	SOC_LOGI("    m_value:     %x\n", hw->state.m_value);
	SOC_LOGI("    pw_write:    %x\n", hw->state.pw_write);
	SOC_LOGI("    otp_sel:     %x\n", hw->state.otp_sel);
	SOC_LOGI("\r\n");

	/* REG_0xA */
	SOC_LOGI("  config=0x%x value=0x%x\n", &hw->config, hw->config.v);
	SOC_LOGI("    clk_cfg:        %x\n", hw->config.clk_cfg);
	SOC_LOGI("    mode_sel:       %x\n", hw->config.mode_sel);
	SOC_LOGI("    cpu_data_wr_en: %x\n", hw->config.cpu_data_wr_en);
	SOC_LOGI("    wrsr_data:      %x\n", hw->config.wrsr_data);
	SOC_LOGI("    crc_en:         %x\n", hw->config.crc_en);
	SOC_LOGI("    allff_reg:      %x\n", hw->config.allff_reg);
	SOC_LOGI("    fclk_gate_xaes: %x\n", hw->config.fclk_gate_xaes);
	SOC_LOGI("    reserved:       %x\n", hw->config.reserved);
	SOC_LOGI("\r\n");

	/* REG_0xB */
	SOC_LOGI("  ps_ctrl=0x%x value=0x%x\n", &hw->ps_ctrl, hw->ps_ctrl.v);
	SOC_LOGI("    tres1_trdp_delay_cnt: %x\n", hw->ps_ctrl.tres1_trdp_delay_cnt);
	SOC_LOGI("    tdp_trdpdd_delay_cnt: %x\n", hw->ps_ctrl.tdp_trdpdd_delay_cnt);
	SOC_LOGI("    dpd_fbd:              %x\n", hw->ps_ctrl.dpd_fbd);
	SOC_LOGI("    prefetch_version:     %x\n", hw->ps_ctrl.prefetch_version);
	SOC_LOGI("    reserved:             %x\n", hw->ps_ctrl.reserved);
	SOC_LOGI("    dpd_status:           %x\n", hw->ps_ctrl.dpd_status);
	SOC_LOGI("\r\n");

	/* REG_0xC */
	SOC_LOGI("  page_ctrl=0x%x value=0x%x\n", &hw->page_ctrl, hw->page_ctrl.v);
	SOC_LOGI("    mem_data:     %x\n", hw->page_ctrl.mem_data);
	SOC_LOGI("    reserved:     %x\n", hw->page_ctrl.reserved);
	SOC_LOGI("    mem_addr_clr: %x\n", hw->page_ctrl.mem_addr_clr);
	SOC_LOGI("\r\n");

	/* REG_0xd ~ REG_0x14 */
	for(uint32_t i = 0; i < SOC_SEC_ADDR_NUM; i++) {
		SOC_LOGI("  sec_addr%d_start=0x%x value=0x%x\n", i, &hw->sec_addr[i].sec_addr_start, hw->sec_addr[i].sec_addr_start.v);
		SOC_LOGI("    flash_sec_start_addr%d: %x\n", i, hw->sec_addr[i].sec_addr_start.flash_sec_start_addr);
		SOC_LOGI("    flash_sec_addr%d_en:    %x\n", i, hw->sec_addr[i].sec_addr_start.flash_sec_addr_en);
		SOC_LOGI("    reserved:               %x\n", hw->sec_addr[i].sec_addr_start.reserved);
		SOC_LOGI("\r\n");

		SOC_LOGI("  sec_addr%d_end=0x%x value=0x%x\n", i, &hw->sec_addr[i].sec_addr_end, hw->sec_addr[i].sec_addr_end.v);
		SOC_LOGI("    flash_sec_end_addr%d: %x\n", i, hw->sec_addr[i].sec_addr_end.flash_sec_end_addr);
		SOC_LOGI("    reserved:             %x\n", hw->sec_addr[i].sec_addr_end.reserved);
		SOC_LOGI("\r\n");
	}

	/* REG_0x15 */
	SOC_LOGI("  op_cmd=0x%x value=0x%x\n", &hw->op_cmd, hw->op_cmd.v);
	SOC_LOGI("    addr_sw_reg: %x\n", hw->op_cmd.addr_sw_reg);
	SOC_LOGI("    op_type_sw:  %x\n", hw->op_cmd.op_type_sw);
	SOC_LOGI("    reserved:    %x\n", hw->op_cmd.reserved);
	SOC_LOGI("\r\n");
}

#endif

