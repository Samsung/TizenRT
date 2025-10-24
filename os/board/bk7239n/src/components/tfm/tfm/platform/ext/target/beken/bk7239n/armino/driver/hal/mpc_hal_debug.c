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
#include "mpc_hw.h"
#include "mpc_hal.h"
#include "mpc_ll.h"
#include "driver/mpc.h"

#if CONFIG_MPC_TEST

void mpc_struct_dump(mpc_dev_t mpc_dev)
{
	mpc_hw_t *hw = (mpc_hw_t *)MPC_LL_REG_BASE(mpc_dev);
	SOC_LOGI("base=%x\r\n", (uint32_t)hw);

	SOC_LOGI("  ctrl=0x%x value=0x%x\n", &hw->ctrl, hw->ctrl.v);
	SOC_LOGI("    reserved0:   %x\n", hw->ctrl.reserved0);
	SOC_LOGI("    cfg_sec_rsp: %x\n", hw->ctrl.cfg_sec_rsp);
	SOC_LOGI("    reserved1:   %x\n", hw->ctrl.reserved1);
	SOC_LOGI("    gating_req:  %x\n", hw->ctrl.gating_req);
	SOC_LOGI("    gating_ack:  %x\n", hw->ctrl.gating_ack);
	SOC_LOGI("    auto_inc:    %x\n", hw->ctrl.auto_inc);
	SOC_LOGI("    reserved2:   %x\n", hw->ctrl.reserved2);
	SOC_LOGI("    sec_lock:    %x\n", hw->ctrl.sec_lock);
	SOC_LOGI("\r\n");

	SOC_LOGI("  block_index_max=0x%x value=0x%x\n", &hw->block_index_max, hw->block_index_max);

	SOC_LOGI("  block_size=0x%x value=0x%x\n", &hw->block_size, hw->block_size.v);
	SOC_LOGI("    block_size: %x\n", hw->block_size.block_size);
	SOC_LOGI("    reserved:   %x\n", hw->block_size.reserved);
	SOC_LOGI("\r\n");

	SOC_LOGI("  block_index=0x%x value=0x%x\n", &hw->block_index, hw->block_index);
	SOC_LOGI("  block_lut=0x%x value=0x%x\n", &hw->block_lut, hw->block_lut);
}

#endif

