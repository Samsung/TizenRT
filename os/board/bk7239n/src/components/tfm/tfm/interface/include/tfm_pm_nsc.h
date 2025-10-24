// Copyright 2024-2025 Beken
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

#include <soc/soc.h>
#include "hal_port.h"
#include "system_hw.h"

#ifdef __cplusplus
extern "C" {
#endif

int psa_pm_otp_ahb_backup(uint64_t sleep_time, void *args);
int psa_pm_otp_ahb_restore(uint64_t sleep_time, void *args);
int psa_pm_otp_apb_backup(uint64_t sleep_time, void *args);
int psa_pm_otp_apb_restore(uint64_t sleep_time, void *args);
int psa_pm_shanhai_backup(uint64_t sleep_time, void *args);
int psa_pm_shanhai_restore(uint64_t sleep_time, void *args);
int psa_pm_uart1_backup(uint64_t sleep_time, void *args);
int psa_pm_uart1_restore(uint64_t sleep_time, void *args);

#ifdef __cplusplus
}
#endif
