/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/
#ifndef _RTW_HT_H_
#define _RTW_HT_H_

#include "wifi.h"

#define HT_OP_IE_LEN 22

struct ht_priv {
	u32 ht_option;
	u32 ampdu_enable; //for enable Tx A-MPDU
	//u8	baddbareq_issued[16];
	//u32	tx_amsdu_enable;//for enable Tx A-MSDU
	//u32	tx_amdsu_maxlen; // 1: 8k, 0:4k ; default:8k, for tx
	//u32	rx_ampdu_maxlen; //for rx reordering ctrl win_sz, updated when join_callback.

	u8 bwmode;	//
	u8 ch_offset; //PRIME_CHNL_OFFSET
	u8 sgi_20m;   //short GI
	u8 sgi_40m;

	//for processing Tx A-MPDU
	u8 agg_enable_bitmap;
	//u8	ADDBA_retry_count;
	u8 candidate_tid_bitmap;

	u8 stbc_cap;
	u8 smps_cap; /*spatial multiplexing power save mode. 0:static SMPS, 1:dynamic SMPS, 3:SMPS disabled, 2:reserved*/

	struct rtw_ieee80211_ht_cap ht_cap;
};

#define STBC_HT_ENABLE_RX BIT0
#define STBC_HT_ENABLE_TX BIT1
#define STBC_HT_TEST_TX_ENABLE BIT2
#define STBC_HT_CAP_TX BIT3

typedef enum AGGRE_SIZE {
	HT_AGG_SIZE_8K = 0,
	HT_AGG_SIZE_16K = 1,
	HT_AGG_SIZE_32K = 2,
	HT_AGG_SIZE_64K = 3,
	VHT_AGG_SIZE_128K = 4,
	VHT_AGG_SIZE_256K = 5,
	VHT_AGG_SIZE_512K = 6,
	VHT_AGG_SIZE_1024K = 7,
} AGGRE_SIZE_E,
	*PAGGRE_SIZE_E;

/* HT Operation element */
#define GET_HT_OP_ELE_PRI_CHL(_pEleStart) LE_BITS_TO_1BYTE(((u8 *)(_pEleStart)), 0, 8)
#define SET_HT_OP_ELE_PRI_CHL(_pEleStart, _val) SET_BITS_TO_LE_1BYTE(((u8 *)(_pEleStart)), 0, 8, _val)

/* HT Operation Info field */
#define GET_HT_OP_ELE_2ND_CHL_OFFSET(_pEleStart) LE_BITS_TO_1BYTE(((u8 *)(_pEleStart)) + 1, 0, 2)
#define GET_HT_OP_ELE_STA_CHL_WIDTH(_pEleStart) LE_BITS_TO_1BYTE(((u8 *)(_pEleStart)) + 1, 2, 1)

#endif //_RTL871X_HT_H_
