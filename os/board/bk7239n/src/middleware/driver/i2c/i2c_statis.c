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

#include <os/mem.h>
#include "i2c_hw.h"
#include "i2c_statis.h"

#if defined(CONFIG_I2C_STATIS)

#define TAG "i2c_statis"

static i2c_statis_t s_i2c_statis[SOC_I2C_UNIT_NUM] = {0};

bk_err_t i2c_statis_init(void)
{
	os_memset(&s_i2c_statis, 0, sizeof(s_i2c_statis));
	return BK_OK;
}

#endif

