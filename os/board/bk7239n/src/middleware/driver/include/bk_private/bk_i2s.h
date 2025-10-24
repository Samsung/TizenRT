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

#define I2S_FAILURE                (1)
#define I2S_SUCCESS                (0)

#define I2S_DEV_NAME                "i2s"

#define I2S_CMD_MAGIC              (0xe280000)
enum
{
	I2S_CMD_UNIT_ENABLE = I2S_CMD_MAGIC + 1,
	I2S_CMD_SET_MSTEN,
	I2S_CMD_SELECT_MODE,
	I2S_CMD_SET_LRCK,
	I2S_CMD_SET_SCK_INV,
	I2S_CMD_SET_SCK_LSB,
	I2S_CMD_SET_SCK_SYNCLEN,
	I2S_CMD_SET_PCM_DLEN,	
	I2S_CMD_SET_FREQ_DATAWIDTH,
    I2S_CMD_RXINT_EN,
    I2S_CMD_TXINT_EN,
    I2S_CMD_RXOVR_EN,
    I2S_CMD_TXOVR_EN,
    I2S_CMD_RXFIFO_CLR_EN,
    I2S_CMD_TXFIFO_CLR_EN,
    I2S_CMD_RXINT_MODE,
    I2S_CMD_TXINT_MODE,
    I2S_CMD_GET_BUSY,
    I2S_CMD_ENABLE_INTERRUPT,
    I2S_CMD_DISABLE_INTERRUPT,
    I2S_CMD_MASTER_ENABLE,
    I2S_CMD_SLAVE_ENABLE,
    I2S_CMD_DISABLE_I2S,
    I2S_CMD_DMA_MASTER_ENABLE,
    I2S_CMD_DMA_ISR,
};


#define I2S_BIT_DEBUG
#ifdef I2S_BIT_DEBUG
#define bit_dbg(fmt, ...)   BK_LOG_RAW(fmt, ##__VA_ARGS__)
#else
#define bit_dbg(fmt, ...)
#endif

#define TX_FINISH_FLAG              (1 << 31)
#define I2S_SA
enum
{
    I2S_MODE_I2S = 0,
    I2S_MODE_LEFT_JUST = 1,
    I2S_MODE_RIGHT_JUST = 2,
    I2S_MODE_SHORT_SYNC = 4,
    I2S_MODE_LONG_SYNC = 5,
    I2S_MODE_NORMAL_2B_D = 6,
    I2S_MODE_DELAY_2B_D = 7
};
struct i2s_message
{
	UINT32 *send_buf;
	UINT32 send_len;
		
	UINT32 *recv_buf;
	UINT32 recv_len;
};

typedef struct
{
    UINT32 *p_tx_buf;
    UINT32 *p_rx_buf;
    UINT32 trans_done;
	volatile UINT32 tx_remain_data_cnt;
    volatile UINT32 rx_remain_data_cnt;
} i2s_trans_t;

typedef struct
{
    UINT8 rx_level;
    UINT8 tx_level;
} i2s_level_t;

typedef struct
{
    UINT32 freq;
    UINT32 datawidth;
} i2s_rate_t;

/*******************************************************************************
* Function Declarations
*******************************************************************************/
void i2s_init(int register_isr);
void i2s_exit(void);
void i2s_isr(void);
UINT8 is_i2s_active(void);

#ifdef __cplusplus
}
#endif
