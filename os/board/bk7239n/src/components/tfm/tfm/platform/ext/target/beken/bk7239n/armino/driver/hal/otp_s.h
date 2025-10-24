// Copyright 2022-2023 Beken
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

#include "_otp.h"
#include "otp_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

void otp_sleep();
int otp_active();

#define OTP_ACTIVE() do {\
	if(otp_active() == -1){ \
		otp_sleep(); \
		return BK_FAIL; \
	}\
} while(0)
	
bk_err_t otp_read(uint8_t map_id, uint32_t item, uint8_t* buf, uint32_t size);

bk_err_t otp_update(uint8_t map_id, uint32_t item, uint8_t* buf, uint32_t size);

#ifdef __cplusplus
}
#endif