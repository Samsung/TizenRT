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

#include <components/log.h>

//TOOD: Move the following code to a higher-level implementation
typedef struct lock {
#if defined(CONFIG_SPIN_LOCK)
    spinlock_t spin_lock;
#endif
    beken_mutex_t mutex;
    bool is_rtos_disable;
    uint32_t int_level;
} bk_lock_t;

bk_lock_t* lock_init();
void lock_deinit(bk_lock_t* bk_lock);
void lock(bk_lock_t* bk_lock);
void unlock(bk_lock_t* bk_lock);
//TODO end

#define OTP_TAG "otp"
#define OTP_LOGI(...) BK_LOGI(OTP_TAG, ##__VA_ARGS__)
#define OTP_LOGW(...) BK_LOGW(OTP_TAG, ##__VA_ARGS__)
#define OTP_LOGE(...) BK_LOGE(OTP_TAG, ##__VA_ARGS__)
#define OTP_LOGD(...) BK_LOGD(OTP_TAG, ##__VA_ARGS__)
#define OTP_LOGV(...) BK_LOGV(OTP_TAG, ##__VA_ARGS__)