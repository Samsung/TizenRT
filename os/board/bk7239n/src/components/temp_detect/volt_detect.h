// Copyright 2020-2025 Beken
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

#include "bk_sensor_internal.h"

#define ADC_VOLT_SENSER_CHANNEL                 0

/* volt daemon API, should only invoke in sensor daemon task */
void volt_daemon_init(void);
void volt_daemon_deinit(void);
void volt_daemon_polling_handler(void);
int volt_daemon_restart(void);
int volt_daemon_stop(void);

