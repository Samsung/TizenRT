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

#include <common/bk_include.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BK_ERR_WAKEUP_DRIVER_NOT_INIT         (BK_ERR_WAKEUP_BASE - 1) /**< WAKEUP driver not init */
#define BK_ERR_WAKEUP_INVALID_SOURCE_TYPE     (BK_ERR_WAKEUP_BASE - 2) /**< WAKEUP invalid source type */


typedef int (*wakeup_cb)(void *args);
typedef struct {
    wakeup_cb cb;
    void *args;
} wakeup_cb_conf_t;


/**
 * @}
 */

#ifdef __cplusplus
}
#endif


