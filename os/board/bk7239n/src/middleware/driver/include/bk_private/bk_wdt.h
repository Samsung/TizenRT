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

#define WDT_DEV_NAME                "wdt"

void bk_int_wdt_feed(void);
void bk_task_wdt_start(void);
__attribute__((section(".itcm_sec_code")))void bk_task_wdt_stop(void);
void bk_task_wdt_feed(void);
void bk_task_wdt_timeout_check(void);
void bk_wdt_feed_handle(void);

#ifdef __cplusplus
}
#endif
