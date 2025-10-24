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

#define BK_ERR_SCR_ID			(BK_ERR_SCR_BASE - 1)

typedef enum {
	BK_SCR_TYPE_RX = 0,
	BK_SCR_TYPE_TX,
	BK_SCR_TYPE_NONE
}scr_type_t;

typedef enum {
	BK_SCR_GPIO_PAUSE = 0,
	BK_SCR_GPIO_STORE,
	BK_SCR_GPIO_MAX,
} scr_gpio_type;

typedef enum {
	BK_SCR_CTRL_INV_LEVEL = 0,
	BK_SCR_CTRL_INV_ORD,
	BK_SCR_CTRL_PECH2FIFO,
	BK_SCR_CTRL_CLK_STOP,
	BK_SCR_CTRL_CLK_STOP_VAL,
	BK_SCR_CTRL_TX_EN,
	BK_SCR_CTRL_RX_EN,
	BK_SCR_CTRL_TS2FIFO,
	BK_SCR_CTRL_T0T1,
	BK_SCR_CTRL_ATR_START_FLUSH_FIFO,
	BK_SCR_CTRL_TCK_EN,
	BK_SCR_CTRL_GLOBAL_INTR_EN,
	BK_SCR_CTRL_WARM_RST,
	BK_SCR_CTRL_ACT,
	BK_SCR_CTRL_DEACT,
	BK_SCR_CTRL_VCC,
	BK_SCR_CTRL_ACT_FAST,
	BK_SCR_CTRL_ALL,
}scr_ctrl_idx_t;

typedef enum {
	BK_SCR_INTR_TX_FIFO_DONE = 0,
	BK_SCR_INTR_TX_FIFO_EMPTY,
	BK_SCR_INTR_RX_FIFO_FULL,
	BK_SCR_INTR_CLK_STOP_RUN,
	BK_SCR_INTR_TX_DONE,
	BK_SCR_INTR_RX_DONE,
	BK_SCR_INTR_TX_PERR,
	BK_SCR_INTR_RX_PERR,
	BK_SCR_INTR_C2C_FULL,
	BK_SCR_INTR_RX_THD,
	BK_SCR_INTR_ATR_FAIL,
	BK_SCR_INTR_ATR_DONE,
	BK_SCR_INTR_SC_REM,
	BK_SCR_INTR_SC_INS,
	BK_SCR_INTR_SC_ACT,
	BK_SCR_INTR_SC_DEACT,
	BK_SCR_INTR_TX_THD,
	BK_SCR_INTR_TCK_ERR,
	BK_SCR_INTR_ALL,
} scr_intr_idx_t;

typedef struct {
	uint8_t CLA;
	uint8_t INS;
	uint8_t P1;
	uint8_t P2;
	uint8_t P3;
}scr_header_t;

#ifdef __cplusplus
}
#endif


