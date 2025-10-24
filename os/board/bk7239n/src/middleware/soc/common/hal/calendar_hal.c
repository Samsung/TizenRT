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

#include "calendar_hal.h"
#include "calendar_ll.h"
#include <driver/hal/hal_calendar_types.h>

bk_err_t calendar_hal_init(calendar_hal_t *hal)
{
	hal->hw = (calendar_hw_t *)CALENDAR_LL_REG_BASE(hal->id);
	calendar_ll_init(hal->hw);
	calendar_ll_enable(hal->hw);
	return BK_OK;
}

