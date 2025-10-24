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

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief rosc /extern32k  ppm statistic
 */
bk_err_t bk_rosc_ppm_statistic_start(timer_id_t timer_id, uint32_t timer_interval, uint32_t count);

bk_err_t bk_rosc_ppm_statistics_get(int32 *rosc_ppm);

#ifdef __cplusplus
}
#endif

