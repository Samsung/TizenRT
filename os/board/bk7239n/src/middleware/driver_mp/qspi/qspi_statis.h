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
#include <driver/hal/hal_qspi_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint32_t qspi_int_cnt;
	uint32_t sw_op_int_cnt;
} qspi_statis_t;

#if defined(CONFIG_QSPI_STATIS)

#define QSPI_STATIS_DEC() qspi_statis_t *qspi_statis = NULL
#define QSPI_STATIS_GET(_statis) (_statis) = qspi_statis_get_statis()
#define QSPI_STATIS_SET(_statis, _v) (_statis) = (_v)
#define QSPI_STATIS_ADD(_statis, _v) (_statis) += (_v);
#define QSPI_STATIS_INC(_statis) QSPI_STATIS_ADD((_statis), 1)
bk_err_t qspi_statis_init(void);
qspi_statis_t* qspi_statis_get_statis(void);
void qspi_statis_dump(void);

#else

#define QSPI_STATIS_DEC()
#define QSPI_STATIS_GET(_statis)
#define QSPI_STATIS_SET(_id, _v)
#define QSPI_STATIS_ADD(_statis, _v)
#define QSPI_STATIS_INC(_statis)
#define qspi_statis_init()
#define qspi_statis_dump()
#define qspi_statis_get_statis()

#endif

#ifdef __cplusplus
}
#endif


