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

#ifdef __cplusplus
extern "C" {
#endif

#include <common/bk_err.h>

#define RWNX_ERR_TIMEOUT                (BK_ERR_RWNX_BASE)
#define RWNX_ERR_IOCTL_SEND             (BK_ERR_RWNX_BASE - 1)
#define BK_ERR_RWNX_INIT_IO_QUEUE       (BK_ERR_RWNX_BASE - 2)
#define BK_ERR_RWNX_INIT_IO_TASK        (BK_ERR_RWNX_BASE - 3)
#define BK_ERR_RWNX_INIT_SEMA           (BK_ERR_RWNX_BASE - 4)
#define BK_ERR_RWNX_INIT_KMSG_TASK      (BK_ERR_RWNX_BASE - 5)


#ifdef __cplusplus
}
#endif

