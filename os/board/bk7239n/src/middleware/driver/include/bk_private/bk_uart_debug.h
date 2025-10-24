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

#include "bk_uart.h"

/*******************************************************************************
* Function Declarations
*******************************************************************************/
UINT32 udebug_has_ctrlc(void);
UINT32 udebug_wait_ctrlc_exit(void);
UINT32 uart_debug_init(void);
extern int bkreg_run_command1(const unsigned char *content, int cnt);
extern void get_device_id(void);
extern void send_device_id(void);
extern void send_chip_id(void);
extern void ate_test_multiple_cpus_init(void);


#ifdef __cplusplus
}
#endif
