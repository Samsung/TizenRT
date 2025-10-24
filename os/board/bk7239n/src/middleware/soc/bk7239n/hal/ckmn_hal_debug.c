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

#include "hal_config.h"
#include "ckmn_hw.h"
#include "ckmn_hal.h"

#if CFG_HAL_DEBUG_CKMN

void ckmn_struct_dump(void)
{
	ckmn_hw_t *ckmn = (ckmn_hw_t *)CKMN_LL_REG_BASE;
	SOC_LOGI("device_id: %8x\r\n", ckmn->deviceid);
	SOC_LOGI("versionid: %8x\r\n", ckmn->versionid);
	SOC_LOGI("global_ctrl: %8x\r\n", ckmn->global_ctrl.v);
	SOC_LOGI("rc32k_count: %8x\r\n", ckmn->rc32k_count.v);
	SOC_LOGI("rc32k_ctrl: %8x\r\n", ckmn->rc32k_ctrl.v);
	SOC_LOGI("rc26m_count: %8x\r\n", ckmn->rc26m_count.v);
	SOC_LOGI("corr_cfg: %8x\r\n", ckmn->corr_cfg.v);
	SOC_LOGI("intr: %8x\r\n", ckmn->intr.v);
}
#endif
