// Copyright 2025-2025 Beken
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
#include <common/bk_err.h>

typedef struct
{
    uint32_t event;
    uint32_t param1;
	uint32_t param2;
    uint32_t param3;
} pm_ap_core_msg_t;
typedef enum
{
   PM_DEMO_ENTER_LOW_VOLTAGE = 0,
   PM_DEMO_ENTER_DEEP_SLEEP,
   PM_DEMO_CALLBACK_MSG_HANDLE
}pm_demo_sleep_mode_e;
/**
 * @brief send message to pm demo thread for enter deepsleep or low voltage
 *
 * @attention
 * - This API is to send message to pm demo thread for enter deepsleep or low voltage.
 *
 * @param
 * -msg:message info
 * @return
 * - BK_OK: succeed
 * - others: other errors.
 */
bk_err_t bk_pm_demo_send_msg(pm_ap_core_msg_t *msg);
/**
 * @brief start pm demo thread
 *
 * @attention
 * - This API is start pm demo thread.
 *
 * @param
 * -void
 * @return
 * - BK_OK: succeed
 * - others: other errors.
 */
bk_err_t pm_demo_thread_main(void);