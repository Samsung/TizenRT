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
#include "qspi_hw.h"
#include "qspi_hal.h"
#include "qspi_ll.h"

#if CFG_HAL_DEBUG_QSPI

void qspi_struct_dump(uint32_t id)
{
	qspi_hw_t *hw = (qspi_hw_t *)QSPI_LL_REG_BASE(id);
	SOC_LOGI("base=%x\r\n", (uint32_t)hw);

}

#endif

