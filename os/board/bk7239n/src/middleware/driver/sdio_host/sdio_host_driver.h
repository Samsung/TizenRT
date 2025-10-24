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

#include <components/log.h>

#define SDIO_HOST_TAG "sdio_host"
#define SDIO_HOST_LOGI(...) BK_LOGI(SDIO_HOST_TAG, ##__VA_ARGS__)
#define SDIO_HOST_LOGW(...) BK_LOGW(SDIO_HOST_TAG, ##__VA_ARGS__)
#define SDIO_HOST_LOGE(...) BK_LOGE(SDIO_HOST_TAG, ##__VA_ARGS__)
#define SDIO_HOST_LOGD(...) BK_LOGD(SDIO_HOST_TAG, ##__VA_ARGS__)

/* standard sd commands */
#define GO_IDLE_STATE             0   /* bc                          */
#define SEND_OP_COND              1   /* bc                          */
#define ALL_SEND_CID              2
#define SEND_RELATIVE_ADDR        3   /* ac   [31:16] RCA        R6  */
#define IO_SEND_OP_COND           5   /* ac                      R4  */
#define SWITCH_FUNC               6
#define SELECT_CARD               7   /* ac   [31:16] RCA        R7  */
#define SEND_IF_COND              8   /* adtc                    R1  */
#define SEND_CSD                  9
#define SEND_STATUS               13
#define READ_SINGLE_BLOCK         17
#define WRITE_BLOCK               24
#define SD_APP_OP_COND            41
#define IO_RW_DIRECT              52  /* ac   [31:0] See below   R5  */
#define IO_RW_EXTENDED            53  /* adtc [31:0] See below   R5  */
#define APP_CMD                   55

