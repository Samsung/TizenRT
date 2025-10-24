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

#define MPB_DEV_NAME                 "mac_phy_bypass"

#define MPB_CMD_MAGIC               (0xcbe0000)
enum
{
    MCMD_RX_MODE_BYPASS_MAC                 = MPB_CMD_MAGIC + 0,
    MCMD_TX_MODE_BYPASS_MAC                 = MPB_CMD_MAGIC + 1, 
    MCMD_TX_LEGACY_SET_LEN,
    MCMD_TX_HT_VHT_SET_LEN,
    MCMD_STOP_BYPASS_MAC,
    MCMD_START_BYPASS_MAC,
    MCMD_SET_BANDWIDTH, 
    MCMD_SET_GI,
    MCMD_SET_DCM,
    MCMD_SET_FEC_CODING,
    MCMD_BYPASS_TX_SET_RATE_MFORMAT,
    MCMD_SET_TXDELAY,
    MCMD_INIT_BYPASS_MAC,
    MCMD_BYPASS_MAC_SET_TX_PLD_TYPE,
    MCMD_BYPASS_MAC_SET_TX_PKT_NUM,
    MCMD_TX_HE_SET_LEN,
    MCMD_ONLY_VHT_SET_LEN,
};

/*
b0000: NON-HT,  b0001: NON-HT-DUP-OFDM,  b0010: HT-MM
b0011: HT-GF,   b0100: VHT,              b0101: HE-SU
b0110: HE-MU,   b0111: HE-EXT-SU,        b1000: HE-TB
*/
typedef enum
{
	MOD_FMT_NON_HT          = 0x0,
	MOD_FMT_NON_HT_DUP_OFDM = 0x1,
	MOD_FMT_HT_MM           = 0x2,
	MOD_FMT_HT_GF           = 0x3,
	MOD_FMT_VHT             = 0x4,
	MOD_FMT_HE_SU           = 0x5,
	MOD_FMT_HE_MU           = 0x6,
	MOD_FMT_HE_EXT_SU       = 0x7,
	MOD_FMT_HE_TB           = 0x8,
	MOD_FMT_MASK            = 0xF,
} ModulationFormat;

#define TX_LEGACY_DATA_LEN_MASK              (0xFFF)
#define TX_HT_VHT_DATA_LEN_MASK              (0xFFFFF)
#define TX_HE_DATA_LEN_MASK                  (0xFFFFF)

/*MCMD_TX_BYPASS_MAC_RATE*/
#define PPDU_RATE_POSI                           (4)
#define PPDU_RATE_MASK                           (0xF)

#define PPDU_BANDWIDTH_POSI                      (6)
#define PPDU_BANDWIDTH_MASK                      (0x1)

#define PPDU_LEG_RATE_1MBPS                      (0x00)
#define PPDU_LEG_RATE_2MBPS                      (0x01)
#define PPDU_LEG_RATE_5_5MBPS                    (0x02)
#define PPDU_LEG_RATE_11MBPS                     (0x03)
#define PPDU_LEG_RATE_6MBPS                      (0x0B)
#define PPDU_LEG_RATE_9MBPS                      (0x0F)
#define PPDU_LEG_RATE_12MBPS                     (0x0A)
#define PPDU_LEG_RATE_18MBPS                     (0x0E)
#define PPDU_LEG_RATE_24MBPS                     (0x09)
#define PPDU_LEG_RATE_36MBPS                     (0x0D)
#define PPDU_LEG_RATE_48MBPS                     (0x08)
#define PPDU_LEG_RATE_54MBPS                     (0x0C)

typedef struct mbps_txs_mfr_st {
    UINT32 mod_format;
    UINT32 rate;
}MBPS_TXS_MFR_ST, *MBPS_TXS_MFR_PTR;

void mpb_init(void);
void mpb_exit(void);
extern UINT32 mpb_ctrl(UINT32 cmd, void *param);
#ifdef __cplusplus
}
#endif
