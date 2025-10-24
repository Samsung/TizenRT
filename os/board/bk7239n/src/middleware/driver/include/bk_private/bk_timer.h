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

#define TIMER_DEV_NAME                "bk_timer"

#define BK_TIMER_FAILURE                (1)
#define BK_TIMER_SUCCESS                (0)

#define TIMER_CMD_MAGIC              (0xe340000)
enum
{
    CMD_TIMER_UNIT_ENABLE = TIMER_CMD_MAGIC + 1,
    CMD_TIMER_UNIT_DISABLE,
    CMD_TIMER_INIT_PARAM,
    CMD_TIMER_INIT_PARAM_US,
    CMD_TIMER_READ_CNT
};

enum
{
    BKTIMER0     = 0,
    BKTIMER1,
    BKTIMER2,
    BKTIMER3,
    BKTIMER4,
    BKTIMER5,
    BKTIMER_COUNT
};

typedef void (*TFUNC)(UINT8);

typedef struct
{
    UINT8 channel;
    UINT8 div;
    UINT32 period;
    TFUNC t_Int_Handler;
} timer_param_t;

//TODO what's the relationship with bk_timer_start/stop???
void bk_timer_init(void);
void bk_timer_exit(void);
void bk_timer_isr(void);

#ifdef __cplusplus
}
#endif
