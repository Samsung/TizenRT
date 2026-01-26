// Copyright 2022-2025 Beken
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

// This is a generated file, if you need to modify it, use the script to
// generate and modify all the struct.h, ll.h, reg.h, debug_dump.c files!

#pragma once


#ifdef __cplusplus
extern "C" {
#endif


#define IRDA_DEVICE_ID_ADDR (SOC_IRDA_REG_BASE + (0x0 << 2))

#define IRDA_DEVICE_ID_DEVICEID_POS (0)
#define IRDA_DEVICE_ID_DEVICEID_MASK (0xffffffff)

#define IRDA_VERSION_ID_ADDR (SOC_IRDA_REG_BASE + (0x1 << 2))

#define IRDA_VERSION_ID_VERSIONID_POS (0)
#define IRDA_VERSION_ID_VERSIONID_MASK (0xffffffff)

#define IRDA_SOFT_RESET_ADDR (SOC_IRDA_REG_BASE + (0x2 << 2))

#define IRDA_SOFT_RESET_SOFT_RESET_POS (0)
#define IRDA_SOFT_RESET_SOFT_RESET_MASK (0x1)

#define IRDA_SOFT_RESET_CLKG_BYPASS_POS (1)
#define IRDA_SOFT_RESET_CLKG_BYPASS_MASK (0x1)

#define IRDA_SOFT_RESET_RESERVED_BIT_2_31_POS (2)
#define IRDA_SOFT_RESET_RESERVED_BIT_2_31_MASK (0x3fffffff)

#define IRDA_DEV_STATUS_ADDR (SOC_IRDA_REG_BASE + (0x3 << 2))

#define IRDA_DEV_STATUS_DEVSTATUS_POS (0)
#define IRDA_DEV_STATUS_DEVSTATUS_MASK (0xffffffff)

#define IRDA_REG0X4_ADDR (SOC_IRDA_REG_BASE + (0x4 << 2))

#define IRDA_REG0X4_RXENABLE_POS (0)
#define IRDA_REG0X4_RXENABLE_MASK (0x1)

#define IRDA_REG0X4_RX_INITIAL_LEVEL_POS (1)
#define IRDA_REG0X4_RX_INITIAL_LEVEL_MASK (0x1)

#define IRDA_REG0X4_TXENABLE_POS (2)
#define IRDA_REG0X4_TXENABLE_MASK (0x1)

#define IRDA_REG0X4_TX_INITIAL_LEVEL_POS (3)
#define IRDA_REG0X4_TX_INITIAL_LEVEL_MASK (0x1)

#define IRDA_REG0X4_TX_START_POS (4)
#define IRDA_REG0X4_TX_START_MASK (0x1)

#define IRDA_REG0X4_IRDA_PWD_POS (5)
#define IRDA_REG0X4_IRDA_PWD_MASK (0x1)

#define IRDA_REG0X4_RFU7_POS (6)
#define IRDA_REG0X4_RFU7_MASK (0x3)

#define IRDA_REG0X4_CLK_FREQ_IN_POS (8)
#define IRDA_REG0X4_CLK_FREQ_IN_MASK (0x7f)

#define IRDA_REG0X4_RSTN_POS (15)
#define IRDA_REG0X4_RSTN_MASK (0x1)

#define IRDA_REG0X4_TXDATA_NUM_POS (16)
#define IRDA_REG0X4_TXDATA_NUM_MASK (0x3ff)

#define IRDA_REG0X4_RESERVED_BIT_26_31_POS (26)
#define IRDA_REG0X4_RESERVED_BIT_26_31_MASK (0x3f)

#define IRDA_REG0X5_ADDR (SOC_IRDA_REG_BASE + (0x5 << 2))

#define IRDA_REG0X5_TX_FIFO_THRESHOLD_POS (0)
#define IRDA_REG0X5_TX_FIFO_THRESHOLD_MASK (0xff)

#define IRDA_REG0X5_RX_FIFO_THRESHOLD_POS (8)
#define IRDA_REG0X5_RX_FIFO_THRESHOLD_MASK (0xff)

#define IRDA_REG0X5_RX_TIMEOUT_CNT_POS (16)
#define IRDA_REG0X5_RX_TIMEOUT_CNT_MASK (0xffff)

#define IRDA_REG0X6_ADDR (SOC_IRDA_REG_BASE + (0x6 << 2))

#define IRDA_REG0X6_TX_FIFO_COUNT_POS (0)
#define IRDA_REG0X6_TX_FIFO_COUNT_MASK (0xff)

#define IRDA_REG0X6_RX_FIFO_COUNT_POS (8)
#define IRDA_REG0X6_RX_FIFO_COUNT_MASK (0xff)

#define IRDA_REG0X6_TX_FIFO_FULL_POS (16)
#define IRDA_REG0X6_TX_FIFO_FULL_MASK (0x1)

#define IRDA_REG0X6_TX_FIFO_EMPTY_POS (17)
#define IRDA_REG0X6_TX_FIFO_EMPTY_MASK (0x1)

#define IRDA_REG0X6_RX_FIFO_FULL_POS (18)
#define IRDA_REG0X6_RX_FIFO_FULL_MASK (0x1)

#define IRDA_REG0X6_RX_FIFO_EMPTY_POS (19)
#define IRDA_REG0X6_RX_FIFO_EMPTY_MASK (0x1)

#define IRDA_REG0X6_FIFO_WR_READY_POS (20)
#define IRDA_REG0X6_FIFO_WR_READY_MASK (0x1)

#define IRDA_REG0X6_FIFO_RD_READY_POS (21)
#define IRDA_REG0X6_FIFO_RD_READY_MASK (0x1)

#define IRDA_REG0X6_RXDATA_NUM_POS (22)
#define IRDA_REG0X6_RXDATA_NUM_MASK (0x3ff)

#define IRDA_REG0X7_ADDR (SOC_IRDA_REG_BASE + (0x7 << 2))

#define IRDA_REG0X7_FIFO_DATA_RX_POS (0)
#define IRDA_REG0X7_FIFO_DATA_RX_MASK (0xffff)

#define IRDA_REG0X7_FIFO_DATA_TX_POS (16)
#define IRDA_REG0X7_FIFO_DATA_TX_MASK (0xffff)

#define IRDA_REG0X8_ADDR (SOC_IRDA_REG_BASE + (0x8 << 2))

#define IRDA_REG0X8_TX_NEED_WR_MASK_POS (0)
#define IRDA_REG0X8_TX_NEED_WR_MASK_MASK (0x1)

#define IRDA_REG0X8_RX_NEED_RD_MASK_POS (1)
#define IRDA_REG0X8_RX_NEED_RD_MASK_MASK (0x1)

#define IRDA_REG0X8_TX_DONE_MASK_POS (2)
#define IRDA_REG0X8_TX_DONE_MASK_MASK (0x1)

#define IRDA_REG0X8_RX_TIMEOUT_MASK_POS (3)
#define IRDA_REG0X8_RX_TIMEOUT_MASK_MASK (0x1)

#define IRDA_REG0X8_RX_OVERFLOW_STATUS_POS (4)
#define IRDA_REG0X8_RX_OVERFLOW_STATUS_MASK (0x1)

#define IRDA_REG0X8_RFU6_POS (5)
#define IRDA_REG0X8_RFU6_MASK (0x7ffffff)

#define IRDA_REG0X9_ADDR (SOC_IRDA_REG_BASE + (0x9 << 2))

#define IRDA_REG0X9_TX_NEED_WR_STATE_POS (0)
#define IRDA_REG0X9_TX_NEED_WR_STATE_MASK (0x1)

#define IRDA_REG0X9_RX_NEED_RD_STATE_POS (1)
#define IRDA_REG0X9_RX_NEED_RD_STATE_MASK (0x1)

#define IRDA_REG0X9_TX_DONE_STATUS_POS (2)
#define IRDA_REG0X9_TX_DONE_STATUS_MASK (0x1)

#define IRDA_REG0X9_RX_DONE_STATUS_POS (3)
#define IRDA_REG0X9_RX_DONE_STATUS_MASK (0x1)

#define IRDA_REG0X9_RX_OVERFLOW_STATUS_POS (4)
#define IRDA_REG0X9_RX_OVERFLOW_STATUS_MASK (0x1)

#define IRDA_REG0X9_RFU5_POS (5)
#define IRDA_REG0X9_RFU5_MASK (0x7ffffff)

#define IRDA_REG0XA_ADDR (SOC_IRDA_REG_BASE + (0xa << 2))

#define IRDA_REG0XA_CARRIER_PERIOD_POS (0)
#define IRDA_REG0XA_CARRIER_PERIOD_MASK (0xff)

#define IRDA_REG0XA_CARRIER_DUTY_POS (8)
#define IRDA_REG0XA_CARRIER_DUTY_MASK (0xff)

#define IRDA_REG0XA_CARRIER_POLARITY_POS (16)
#define IRDA_REG0XA_CARRIER_POLARITY_MASK (0x1)

#define IRDA_REG0XA_CARRIER_ENABLE_POS (17)
#define IRDA_REG0XA_CARRIER_ENABLE_MASK (0x1)

#define IRDA_REG0XA_RFU4_POS (18)
#define IRDA_REG0XA_RFU4_MASK (0x3)

#define IRDA_REG0XA_RX_START_THR_POS (20)
#define IRDA_REG0XA_RX_START_THR_MASK (0xfff)

#define IRDA_REG0XB_ADDR (SOC_IRDA_REG_BASE + (0xb << 2))

#define IRDA_REG0XB_GLITCH_ENABLE_POS (0)
#define IRDA_REG0XB_GLITCH_ENABLE_MASK (0x1)

#define IRDA_REG0XB_RFU0_POS (1)
#define IRDA_REG0XB_RFU0_MASK (0x1)

#define IRDA_REG0XB_RFU1_POS (2)
#define IRDA_REG0XB_RFU1_MASK (0x1)

#define IRDA_REG0XB_RFU2_POS (3)
#define IRDA_REG0XB_RFU2_MASK (0x1fff)

#define IRDA_REG0XB_GLITCH_THR_POS (16)
#define IRDA_REG0XB_GLITCH_THR_MASK (0xfff)

#define IRDA_REG0XB_RFU3_POS (28)
#define IRDA_REG0XB_RFU3_MASK (0xf)

#ifdef __cplusplus
}
#endif
