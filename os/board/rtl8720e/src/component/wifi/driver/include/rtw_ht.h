/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/
#ifndef _RTW_HT_H_
#define _RTW_HT_H_

#include "wifi.h"

#define HT_OP_IE_LEN 22

struct ht_priv {
	u8	ht_enable;
	u32	ht_option;
	u32	ampdu_enable;//for enable Tx A-MPDU
	//u8	baddbareq_issued[16];
	//u32	tx_amsdu_enable;//for enable Tx A-MSDU
	//u32	tx_amdsu_maxlen; // 1: 8k, 0:4k ; default:8k, for tx
	//u32	rx_ampdu_maxlen; //for rx reordering ctrl win_sz, updated when join_callback.

	u8	bwmode;//
	u8	ch_offset;//PRIME_CHNL_OFFSET
	u8	sgi_20m;//short GI
	u8	sgi_40m;

	//for processing Tx A-MPDU
	u8	agg_enable_bitmap;
	//u8	ADDBA_retry_count;
	u8	candidate_tid_bitmap;

	u8	stbc_cap;
	u8	smps_cap; /*spatial multiplexing power save mode. 0:static SMPS, 1:dynamic SMPS, 3:SMPS disabled, 2:reserved*/

	struct rtw_ieee80211_ht_cap ht_cap;

};

#define	STBC_HT_ENABLE_RX			BIT0
#define	STBC_HT_ENABLE_TX			BIT1
#define	STBC_HT_TEST_TX_ENABLE		BIT2
#define	STBC_HT_CAP_TX				BIT3

typedef enum AGGRE_SIZE {
	HT_AGG_SIZE_8K = 0,
	HT_AGG_SIZE_16K = 1,
	HT_AGG_SIZE_32K = 2,
	HT_AGG_SIZE_64K = 3,
	VHT_AGG_SIZE_128K = 4,
	VHT_AGG_SIZE_256K = 5,
	VHT_AGG_SIZE_512K = 6,
	VHT_AGG_SIZE_1024K = 7,
} AGGRE_SIZE_E, *PAGGRE_SIZE_E;

/* HT Operation element */
#define GET_HT_OP_ELE_PRI_CHL(_pEleStart)					LE_BITS_TO_1BYTE(((u8 *)(_pEleStart)), 0, 8)
#define SET_HT_OP_ELE_PRI_CHL(_pEleStart, _val)				SET_BITS_TO_LE_1BYTE(((u8 *)(_pEleStart)), 0, 8, _val)

/* HT Operation Info field */
#define GET_HT_OP_ELE_2ND_CHL_OFFSET(_pEleStart)			LE_BITS_TO_1BYTE(((u8 *)(_pEleStart)) + 1, 0, 2)
#define GET_HT_OP_ELE_STA_CHL_WIDTH(_pEleStart)				LE_BITS_TO_1BYTE(((u8 *)(_pEleStart)) + 1, 2, 1)

/* HT Capabilities Info field */
#define HT_CAP_ELE_CAP_INFO(_pEleStart)					((u8 *)(_pEleStart))
#define GET_HT_CAP_ELE_LDPC_CAP(_pEleStart)				LE_BITS_TO_1BYTE(((u8 *)(_pEleStart)), 0, 1)
#define GET_HT_CAP_ELE_CHL_WIDTH(_pEleStart)			LE_BITS_TO_1BYTE(((u8 *)(_pEleStart)), 1, 1)
#define GET_HT_CAP_ELE_SM_PS(_pEleStart)				LE_BITS_TO_1BYTE(((u8 *)(_pEleStart)), 2, 2)
#define GET_HT_CAP_ELE_GREENFIELD(_pEleStart)			LE_BITS_TO_1BYTE(((u8 *)(_pEleStart)), 4, 1)
#define GET_HT_CAP_ELE_SHORT_GI20M(_pEleStart)			LE_BITS_TO_1BYTE(((u8 *)(_pEleStart)), 5, 1)
#define GET_HT_CAP_ELE_SHORT_GI40M(_pEleStart)			LE_BITS_TO_1BYTE(((u8 *)(_pEleStart)), 6, 1)
#define GET_HT_CAP_ELE_TX_STBC(_pEleStart)				LE_BITS_TO_1BYTE(((u8 *)(_pEleStart)), 7, 1)
#define GET_HT_CAP_ELE_RX_STBC(_pEleStart)				LE_BITS_TO_1BYTE(((u8 *)(_pEleStart))+1, 0, 2)
#define GET_HT_CAP_ELE_DELAYED_BA(_pEleStart)			LE_BITS_TO_1BYTE(((u8 *)(_pEleStart))+1, 2, 1)
#define GET_HT_CAP_ELE_MAX_AMSDU_LENGTH(_pEleStart)		LE_BITS_TO_1BYTE(((u8 *)(_pEleStart))+1, 3, 1)
#define GET_HT_CAP_ELE_DSSS_CCK_40M(_pEleStart)			LE_BITS_TO_1BYTE(((u8 *)(_pEleStart))+1, 4, 1)
#define GET_HT_CAP_ELE_FORTY_INTOLERANT(_pEleStart)		LE_BITS_TO_1BYTE(((u8 *)(_pEleStart))+1, 6, 1)
#define GET_HT_CAP_ELE_LSIG_TXOP_PROTECT(_pEleStart)	LE_BITS_TO_1BYTE(((u8 *)(_pEleStart))+1, 7, 1)

#define SET_HT_CAP_ELE_LDPC_CAP(_pEleStart, _val)			SET_BITS_TO_LE_1BYTE(((u8 *)(_pEleStart)), 0, 1, _val)
#define SET_HT_CAP_ELE_CHL_WIDTH(_pEleStart, _val)			SET_BITS_TO_LE_1BYTE(((u8 *)(_pEleStart)), 1, 1, _val)
#define SET_HT_CAP_ELE_SM_PS(_pEleStart, _val)				SET_BITS_TO_LE_1BYTE(((u8 *)(_pEleStart)), 2, 2, _val)
#define SET_HT_CAP_ELE_GREENFIELD(_pEleStart, _val)			SET_BITS_TO_LE_1BYTE(((u8 *)(_pEleStart)), 4, 1, _val)
#define SET_HT_CAP_ELE_SHORT_GI20M(_pEleStart, _val)		SET_BITS_TO_LE_1BYTE(((u8 *)(_pEleStart)), 5, 1, _val)
#define SET_HT_CAP_ELE_SHORT_GI40M(_pEleStart, _val)		SET_BITS_TO_LE_1BYTE(((u8 *)(_pEleStart)), 6, 1, _val)
#define SET_HT_CAP_ELE_TX_STBC(_pEleStart, _val)			SET_BITS_TO_LE_1BYTE(((u8 *)(_pEleStart)), 7, 1, _val)
#define SET_HT_CAP_ELE_RX_STBC(_pEleStart, _val)			SET_BITS_TO_LE_1BYTE(((u8 *)(_pEleStart)) + 1, 0, 2, _val)
#define SET_HT_CAP_ELE_DELAYED_BA(_pEleStart, _val)			SET_BITS_TO_LE_1BYTE(((u8 *)(_pEleStart)) + 1, 2, 1, _val)
#define SET_HT_CAP_ELE_MAX_AMSDU_LENGTH(_pEleStart, _val)	SET_BITS_TO_LE_1BYTE(((u8 *)(_pEleStart)) + 1, 3, 1, _val)
#define SET_HT_CAP_ELE_DSSS_CCK_40M(_pEleStart, _val)		SET_BITS_TO_LE_1BYTE(((u8 *)(_pEleStart)) + 1, 4, 1, _val)
#define SET_HT_CAP_ELE_FORTY_INTOLERANT(_pEleStart, _val)	SET_BITS_TO_LE_1BYTE(((u8 *)(_pEleStart)) + 1, 6, 1, _val)
#define SET_HT_CAP_ELE_LSIG_TXOP_PROTECT(_pEleStart, _val)	SET_BITS_TO_LE_1BYTE(((u8 *)(_pEleStart)) + 1, 7, 1, _val)

/* A-MPDU Parameters field */
#define HT_CAP_ELE_AMPDU_PARA(_pEleStart)				(((u8 *)(_pEleStart))+2)
#define GET_HT_CAP_ELE_MAX_AMPDU_LEN_EXP(_pEleStart)	LE_BITS_TO_1BYTE(((u8 *)(_pEleStart))+2, 0, 2)
#define GET_HT_CAP_ELE_MIN_MPDU_S_SPACE(_pEleStart)		LE_BITS_TO_1BYTE(((u8 *)(_pEleStart))+2, 2, 3)

/* HT Operation element */

#define GET_HT_OP_ELE_PRI_CHL(_pEleStart)					LE_BITS_TO_1BYTE(((u8 *)(_pEleStart)), 0, 8)
#define SET_HT_OP_ELE_PRI_CHL(_pEleStart, _val)				SET_BITS_TO_LE_1BYTE(((u8 *)(_pEleStart)), 0, 8, _val)

/* HT Operation Info field */
#define HT_OP_ELE_OP_INFO(_pEleStart)						(((u8 *)(_pEleStart)) + 1)
#define GET_HT_OP_ELE_2ND_CHL_OFFSET(_pEleStart)			LE_BITS_TO_1BYTE(((u8 *)(_pEleStart)) + 1, 0, 2)
#define GET_HT_OP_ELE_STA_CHL_WIDTH(_pEleStart)				LE_BITS_TO_1BYTE(((u8 *)(_pEleStart)) + 1, 2, 1)
#define GET_HT_OP_ELE_RIFS_MODE(_pEleStart)					LE_BITS_TO_1BYTE(((u8 *)(_pEleStart)) + 1, 3, 1)
#define GET_HT_OP_ELE_HT_PROTECT(_pEleStart)				LE_BITS_TO_1BYTE(((u8 *)(_pEleStart)) + 2, 0, 2)
#define GET_HT_OP_ELE_NON_GREEN_PRESENT(_pEleStart)			LE_BITS_TO_1BYTE(((u8 *)(_pEleStart)) + 2, 2, 1)
#define GET_HT_OP_ELE_OBSS_NON_HT_PRESENT(_pEleStart)		LE_BITS_TO_1BYTE(((u8 *)(_pEleStart)) + 2, 4, 1)
#define GET_HT_OP_ELE_DUAL_BEACON(_pEleStart)				LE_BITS_TO_1BYTE(((u8 *)(_pEleStart)) + 4, 6, 1)
#define GET_HT_OP_ELE_DUAL_CTS(_pEleStart)					LE_BITS_TO_1BYTE(((u8 *)(_pEleStart)) + 4, 7, 1)
#define GET_HT_OP_ELE_STBC_BEACON(_pEleStart)				LE_BITS_TO_1BYTE(((u8 *)(_pEleStart)) + 5, 0, 1)
#define GET_HT_OP_ELE_LSIG_TXOP_PROTECT(_pEleStart)			LE_BITS_TO_1BYTE(((u8 *)(_pEleStart)) + 5, 1, 1)
#define GET_HT_OP_ELE_PCO_ACTIVE(_pEleStart)				LE_BITS_TO_1BYTE(((u8 *)(_pEleStart)) + 5, 2, 1)
#define GET_HT_OP_ELE_PCO_PHASE(_pEleStart)					LE_BITS_TO_1BYTE(((u8 *)(_pEleStart)) + 5, 3, 1)

#define SET_HT_OP_ELE_2ND_CHL_OFFSET(_pEleStart, _val)		SET_BITS_TO_LE_1BYTE(((u8 *)(_pEleStart)) + 1, 0, 2, _val)
#define SET_HT_OP_ELE_STA_CHL_WIDTH(_pEleStart, _val)		SET_BITS_TO_LE_1BYTE(((u8 *)(_pEleStart)) + 1, 2, 1, _val)
#define SET_HT_OP_ELE_RIFS_MODE(_pEleStart, _val)			SET_BITS_TO_LE_1BYTE(((u8 *)(_pEleStart)) + 1, 3, 1, _val)
#define SET_HT_OP_ELE_HT_PROTECT(_pEleStart, _val)			SET_BITS_TO_LE_1BYTE(((u8 *)(_pEleStart)) + 2, 0, 2, _val)
#define SET_HT_OP_ELE_NON_GREEN_PRESENT(_pEleStart, _val)	SET_BITS_TO_LE_1BYTE(((u8 *)(_pEleStart)) + 2, 2, 1, _val)
#define SET_HT_OP_ELE_OBSS_NON_HT_PRESENT(_pEleStart, _val)	SET_BITS_TO_LE_1BYTE(((u8 *)(_pEleStart)) + 2, 4, 1, _val)
#define SET_HT_OP_ELE_DUAL_BEACON(_pEleStart, _val)			SET_BITS_TO_LE_1BYTE(((u8 *)(_pEleStart)) + 4, 6, 1, _val)
#define SET_HT_OP_ELE_DUAL_CTS(_pEleStart, _val)			SET_BITS_TO_LE_1BYTE(((u8 *)(_pEleStart)) + 4, 7, 1, _val)
#define SET_HT_OP_ELE_STBC_BEACON(_pEleStart, _val)			SET_BITS_TO_LE_1BYTE(((u8 *)(_pEleStart)) + 5, 0, 1, _val)
#define SET_HT_OP_ELE_LSIG_TXOP_PROTECT(_pEleStart, _val)	SET_BITS_TO_LE_1BYTE(((u8 *)(_pEleStart)) + 5, 1, 1, _val)
#define SET_HT_OP_ELE_PCO_ACTIVE(_pEleStart, _val)			SET_BITS_TO_LE_1BYTE(((u8 *)(_pEleStart)) + 5, 2, 1, _val)
#define SET_HT_OP_ELE_PCO_PHASE(_pEleStart, _val)			SET_BITS_TO_LE_1BYTE(((u8 *)(_pEleStart)) + 5, 3, 1, _val)

#endif	//_RTL871X_HT_H_

