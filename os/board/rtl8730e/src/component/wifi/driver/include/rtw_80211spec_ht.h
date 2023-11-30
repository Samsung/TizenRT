/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
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
#ifndef	__RTW_80211SPEC_HT_H_
#define __RTW_80211SPEC_HT_H_

/* 802.11n HT capabilities masks */
#define IEEE80211_HT_CAP_SUP_WIDTH		0x0002
#define IEEE80211_HT_CAP_GRN_FLD		0x0010
#define IEEE80211_HT_CAP_SGI_20			0x0020
#define IEEE80211_HT_CAP_SGI_40			0x0040
#define IEEE80211_HT_CAP_TX_STBC			0x0080
#define IEEE80211_HT_CAP_RX_STBC		0x0300
#define IEEE80211_HT_CAP_DELAY_BA		0x0400
#define IEEE80211_HT_CAP_MAX_AMSDU		0x0800
#define IEEE80211_HT_CAP_DSSSCCK40		0x1000
/* 802.11n HT capability AMPDU settings */
#define IEEE80211_HT_CAP_AMPDU_FACTOR		0x03
#define IEEE80211_HT_CAP_AMPDU_DENSITY		0x1C
/* 802.11n HT capability MSC set */
#define IEEE80211_SUPP_MCS_SET_UEQM		4
#define IEEE80211_HT_CAP_MAX_STREAMS		4
#define IEEE80211_SUPP_MCS_SET_LEN		10
/* maximum streams the spec allows */
#define IEEE80211_HT_CAP_MCS_TX_DEFINED		0x01
#define IEEE80211_HT_CAP_MCS_TX_RX_DIFF		0x02
#define IEEE80211_HT_CAP_MCS_TX_STREAMS		0x0C
#define IEEE80211_HT_CAP_MCS_TX_UEQM		0x10
/* 802.11n HT IE masks */
#define IEEE80211_HT_IE_CHA_SEC_OFFSET		0x03
#define IEEE80211_HT_IE_CHA_SEC_NONE	 	0x00
#define IEEE80211_HT_IE_CHA_SEC_ABOVE 		0x01
#define IEEE80211_HT_IE_CHA_SEC_BELOW 		0x03
#define IEEE80211_HT_IE_CHA_WIDTH		0x04
#define IEEE80211_HT_IE_HT_PROTECTION		0x0003
#define IEEE80211_HT_IE_NON_GF_STA_PRSNT	0x0004
#define IEEE80211_HT_IE_NON_HT_STA_PRSNT	0x0010

#define HT_CAP_IE_LEN 26
#define HT_OP_IE_LEN 22

#define	STBC_HT_ENABLE_RX			BIT0
#define	STBC_HT_ENABLE_TX			BIT1
#define	STBC_HT_TEST_TX_ENABLE		BIT2
#define	STBC_HT_CAP_TX				BIT3

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

#endif

