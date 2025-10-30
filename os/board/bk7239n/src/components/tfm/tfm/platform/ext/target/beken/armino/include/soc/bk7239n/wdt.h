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

#include "reg_base.h"
#include "soc_debug.h"

#ifdef __cplusplus
extern "C" {
#endif

void set_wdt_clkdiv(void);
void update_wdt(uint32_t val);
void close_wdt(void);

#ifdef __cplusplus
}
#endif
