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

#include "bk_list.h"

#define SDIO_DEV_NAME       "sdio"

#define SDIO_FAILURE        ((UINT32)-1)
#define SDIO_SUCCESS        (0)

#define BLOCK_LEN           512

#define H2S_RD_SYNC         (1)
#define H2S_WR_SYNC         (2)
#define H2S_RD_SPECIAL      (5)

#define S2H_WR_SPECIAL      (6)
#define S2H_RD_SYNC         (3)
#define S2H_WR_SYNC         (4)

#define SDIO_DUMMY_BUFF_ADDR             (0x100)
#define SDIO_DUMMY_LENGTH                (0x100)

#define SDIO_CMD_GET_FREE_NODE           (0xBB)
#define SDIO_CMD_REG_RX_CALLBACK         (0xBC)
#define SDIO_CMD_PUSH_FREE_NODE          (0xBD)
#define SDIO_CMD_GET_CNT_FREE_NODE       (0xBE)
#define SDIO_CMD_CLEAR_TX_VALID          (0xC0)
#define SDIO_CMD_SET_TX_VALID            (0xC1)
#define SDIO_CMD_PEEK_H2S_COUNT          (0xC2)
#define SDIO_CMD_PEEK_S2H_COUNT          (0xC3)
#define SDIO_CMD_SET_TX_LEN              (0xC4)

typedef void (*SDIO_FUNC)(void *Lparam, void *Rparam);

typedef struct _sdio_mem_node_
{
    LIST_HEADER_T node_list;

    UINT8 *orig_addr;

    UINT8 *addr;
    UINT32 length;

    SDIO_FUNC callback;
    void *Lparam;
    void *Rparam;
    int  ac;
} SDIO_NODE_T, *SDIO_NODE_PTR;

struct get_free_node_param {
	UINT32 size;
	UINT8 tid;
};

extern int sdio_debug_level;

/*******************************************************************************
* Function Declarations
*******************************************************************************/
void sdio_init(void);
void sdio_exit(void);

#ifdef __cplusplus
}
#endif
