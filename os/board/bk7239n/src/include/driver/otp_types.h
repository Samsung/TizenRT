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
#include "_otp.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum{
    OTP_BUSY = 0x1,
    OTP_ERROR = 0x2,
    OTP_WARNING = 0x4,
    OTP_WRONG = 0x8,
    OTP_FORBID = 0x10,
} otp_status_t;

#ifdef __cplusplus
}
#endif