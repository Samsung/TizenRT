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

#include "trng_hal.h"
#include "trng_ll.h"

bk_err_t trng_hal_init(trng_hal_t *hal)
{
	hal->hw = (trng_hw_t *)TRNG_LL_REG_BASE(hal->id);
	trng_ll_init(hal->hw);
	return BK_OK;
}

bk_err_t trng_hal_start_common(trng_hal_t *hal)
{
	trng_ll_enable(hal->hw);
	return BK_OK;
}

bk_err_t trng_hal_stop_common(trng_hal_t *hal)
{
	trng_ll_disable(hal->hw);
	return BK_OK;
}

