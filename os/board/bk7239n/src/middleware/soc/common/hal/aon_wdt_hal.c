// Copyright 2021-2022 Beken
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

#include "aon_wdt_hal.h"

uint32_t aon_wdt_hal_get_period(void)
{
	return aon_wdt_ll_get_wd_period();
}

bk_err_t aon_wdt_hal_set_period(uint32_t timeout)
{
	aon_wdt_ll_set_period(timeout);
	return BK_OK;
}

bk_err_t aon_wdt_hal_stop_aon_wdt(void)
{
	aon_wdt_ll_set_period(0);
	return BK_OK;
}

