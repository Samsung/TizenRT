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

#define SPI_R_BASE(_id)          (SOC_SPI_REG_BASE + _id * 0x1010000)

#define SPI_R_CTRL(_id)          (SPI_R_BASE(_id) + 4 * 0x04)

#define SPI_F_CLK_DIV            (BIT(8))
#define SPI_F_CLK_DIV_M          (0xFF)
#define SPI_F_CLK_DIV_V          (0xFF)
#define SPI_F_CLK_DIV_S          (8)

#define SPI_R_CFG(_id)           (SPI_R_BASE(_id) + 4 * 0x05)

#define SPI_F_TX_TRANS_LEN       (BIT(8))
#define SPI_F_TX_TRANS_LEN_M     (0xFFF)
#define SPI_F_TX_TRANS_LEN_V     (0xFFF)
#define SPI_F_TX_TRANS_LEN_S     (8)

#define SPI_F_RX_TRANS_LEN       (BIT(20))
#define SPI_F_RX_TRANS_LEN_M     (0xFFF)
#define SPI_F_RX_TRANS_LEN_V     (0xFFF)
#define SPI_F_RX_TRANS_LEN_S     (20)

#define SPI_R_INT_STATUS(_id)    (SPI_R_BASE(_id) + 4 * 0x06)

#define SPI_F_RX_FIOF_RD_READY   (BIT(2))
#define SPI_F_RX_FIOF_RD_READY_M (0x1)
#define SPI_F_RX_FIOF_RD_READY_V (0x1)
#define SPI_F_RX_FIOF_RD_READY_S (1)

#define SPI_R_DATA(_id)          (SPI_R_BASE(_id) + 4 * 0x07)

#ifdef __cplusplus
}
#endif

