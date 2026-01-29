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
#include "qspi_hw.h"
#include "qspi_statis.h"

#if defined(CONFIG_QSPI_STATIS)

#define TAG "qspi_statis"

static qspi_statis_t s_qspi_statis = {0};

bk_err_t qspi_statis_init(void)
{
	os_memset(&s_qspi_statis, 0, sizeof(s_qspi_statis));
	return BK_OK;
}

qspi_statis_t* qspi_statis_get_statis(void)
{
	return &(s_qspi_statis);
}

void qspi_statis_dump(void)
{
	BK_LOGI(TAG, "dump qspi statis:\r\n");
	BK_LOGI(TAG, "qspi_int_cnt    %d\r\n", s_qspi_statis.qspi_int_cnt);
	BK_LOGI(TAG, "sw_op_int_cnt:  %d\r\n", s_qspi_statis.sw_op_int_cnt);
}

#endif

