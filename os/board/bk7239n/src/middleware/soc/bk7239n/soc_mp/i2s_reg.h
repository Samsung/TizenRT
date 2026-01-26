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

#define I2S_LL_REG_BASE                   (SOC_I2S_REG_BASE)

#define I2S_DEVICE_ID_ADDR                (I2S_LL_REG_BASE  + (0x0 << 2))

#define I2S_VERSION_ID_ADDR               (I2S_LL_REG_BASE  + (0x1 << 2))

#define I2S_CLK_RST_ADDR                  (I2S_LL_REG_BASE  + (0x2 << 2))
#define I2S_SOFT_RESET_POS                (0)
#define I2S_SOFT_RESET_MASK               (0x1)
#define I2S_CLKG_BYPASS_POS               (1)
#define I2S_CLKG_BYPASS_MASK              (0x1)

#define I2S_DEV_STATUS_ADDR               (I2S_LL_REG_BASE  + (0x3 << 2))

#define I2S_PCM_CONF_ADDR                 (I2S_LL_REG_BASE  + (0x4 << 2))
#define I2S_BITRATIO_POS                  (0)
#define I2S_BITRATIO_MASK                 (0xFF)
#define I2S_SMPRATIO_POS                  (8)
#define I2S_SMPRATIO_MASK                 (0x1F)
#define I2S_PCM_DLEN_POS                  (13)
#define I2S_PCM_DLEN_MASK                 (0x7)
#define I2S_DATALEN_POS                   (16)
#define I2S_DATALEN_MASK                  (0x1F)
#define I2S_SYNCLEN_POS                   (21)
#define I2S_SYNCLEN_MASK                  (0x7)
#define I2S_LSBFIRST_POS                  (24)
#define I2S_LSBFIRST_MASK                 (0x1)
#define I2S_SCLKINV_POS                   (25)
#define I2S_SCLKINV_MASK                  (0x1)
#define I2S_LRCKRP_POS                    (26)
#define I2S_LRCKRP_MASK                   (0x1)
#define I2S_MODESEL_POS                   (27)
#define I2S_MODESEL_MASK                  (0x7)
#define I2S_MSTEN_POS                     (30)
#define I2S_MSTEN_MASK                    (0x1)
#define I2S_I2SPCMEN_POS                  (31)
#define I2S_I2SPCMEN_MASK                 (0x1)

#define I2S_PCM_CN_ADDR                   (I2S_LL_REG_BASE  + (0x5 << 2))
#define I2S_RXINT_EN_POS                  (0)
#define I2S_RXINT_EN_MASK                 (0x1)
#define I2S_TXINT_EN_POS                  (1)
#define I2S_TXINT_EN_MASK                 (0x1)
#define I2S_RXOVF_EN_POS                  (2)
#define I2S_RXOVF_EN_MASK                 (0x1)
#define I2S_TXUDF_EN_POS                  (3)
#define I2S_TXUDF_EN_MASK                 (0x1)
#define I2S_RXINT_LEVEL_POS               (4)
#define I2S_RXINT_LEVEL_MASK              (0x3)
#define I2S_TXINT_LEVEL_POS               (6)
#define I2S_TXINT_LEVEL_MASK              (0x3)
#define I2S_TXFIFO_CLR_POS                (8)
#define I2S_TXFIFO_CLR_MASK               (0x1)
#define I2S_RXFIFO_CLR_POS                (9)
#define I2S_RXFIFO_CLR_MASK               (0x1)
#define I2S_SMPRATIO_H2B_POS              (10)
#define I2S_SMPRATIO_H2B_MASK             (0x3)
#define I2S_BITRATIO_H4B_POS              (12)
#define I2S_BITRATIO_H4B_MASK             (0xF)
#define I2S_LRCOM_STORE_POS               (16)
#define I2S_LRCOM_STORE_MASK              (0x1)
#define I2S_PARALLEL_EN_POS               (17)
#define I2S_PARALLEL_EN_MASK              (0x1)

#define I2S_PCM_STAT_ADDR                 (I2S_LL_REG_BASE  + (0x6 << 2))
#define I2S_RXINT_POS                     (0)
#define I2S_RXINT_MASK                    (0x1)
#define I2S_TXINT_POS                     (1)
#define I2S_TXINT_MASK                    (0x1)
#define I2S_RXOVF_POS                     (2)
#define I2S_RXOVF_MASK                    (0x1)
#define I2S_TXUDF_POS                     (3)
#define I2S_TXUDF_MASK                    (0x1)
#define I2S_RXFIFO_RD_READY_POS           (4)
#define I2S_RXFIFO_RD_READY_MASK          (0x1)
#define I2S_TXFIFO_WR_READY_POS           (5)
#define I2S_TXFIFO_WR_READY_MASK          (0x1)

#define I2S_PCM_DAT_ADDR                  (I2S_LL_REG_BASE  + (0x7 << 2))

#define I2S_PCM_CN_LT2_ADDR               (I2S_LL_REG_BASE  + (0x8 << 2))
#define I2S_RX2INT_EN_POS                 (0)
#define I2S_RX2INT_EN_MASK                (0x1)
#define I2S_TX2INT_EN_POS                 (1)
#define I2S_TX2INT_EN_MASK                (0x1)
#define I2S_RX2OVF_EN_POS                 (2)
#define I2S_RX2OVF_EN_MASK                (0x1)
#define I2S_TX2UDF_EN_POS                 (3)
#define I2S_TX2UDF_EN_MASK                (0x1)
#define I2S_RX3INT_EN_POS                 (4)
#define I2S_RX3INT_EN_MASK                (0x1)
#define I2S_TX3NT_EN_POS                  (5)
#define I2S_TX3NT_EN_MASK                 (0x1)
#define I2S_RX3OVF_EN_POS                 (6)
#define I2S_RX3OVF_EN_MASK                (0x1)
#define I2S_TX3UDF_EN_POS                 (7)
#define I2S_TX3UDF_EN_MASK                (0x1)
#define I2S_RX4INT_EN_POS                 (8)
#define I2S_RX4INT_EN_MASK                (0x1)
#define I2S_TX4NT_EN_POS                  (9)
#define I2S_TX4NT_EN_MASK                 (0x1)
#define I2S_RX4OVF_EN_POS                 (10)
#define I2S_RX4OVF_EN_MASK                (0x1)
#define I2S_TX4UDF_EN_POS                 (11)
#define I2S_TX4UDF_EN_MASK                (0x1)

#define I2S_PCM_STAT_LT2_ADDR             (I2S_LL_REG_BASE  + (0x9 << 2))
#define I2S_RX2INT_POS                    (0)
#define I2S_RX2INT_MASK                   (0x1)
#define I2S_TX2INT_POS                    (1)
#define I2S_TX2INT_MASK                   (0x1)
#define I2S_RX2OVF_POS                    (2)
#define I2S_RX2OVF_MASK                   (0x1)
#define I2S_TX2UDF_POS                    (3)
#define I2S_TX2UDF_MASK                   (0x1)
#define I2S_RX3INT_POS                    (4)
#define I2S_RX3INT_MASK                   (0x1)
#define I2S_TX3INT_POS                    (5)
#define I2S_TX3INT_MASK                   (0x1)
#define I2S_RX3OVF_POS                    (6)
#define I2S_RX3OVF_MASK                   (0x1)
#define I2S_TX3UDF_POS                    (7)
#define I2S_TX3UDF_MASK                   (0x1)
#define I2S_RX4INT_POS                    (8)
#define I2S_RX4INT_MASK                   (0x1)
#define I2S_TX4INT_POS                    (9)
#define I2S_TX4INT_MASK                   (0x1)
#define I2S_RX4OVF_POS                    (10)
#define I2S_RX4OVF_MASK                   (0x1)
#define I2S_TX4UDF_POS                    (11)
#define I2S_TX4UDF_MASK                   (0x1)

#define I2S_PCM_DAT2_ADDR                 (I2S_LL_REG_BASE  + (0xA << 2))

#define I2S_PCM_DAT3_ADDR                 (I2S_LL_REG_BASE  + (0xB << 2))

#define I2S_PCM_DAT4_ADDR                 (I2S_LL_REG_BASE  + (0xC << 2))

#ifdef __cplusplus
}
#endif
