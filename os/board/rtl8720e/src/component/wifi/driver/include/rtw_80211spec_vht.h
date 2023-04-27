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
#ifndef	__RTW_80211SPEC_VHT_H_
#define __RTW_80211SPEC_VHT_H_

#define VHT_CAP_IE_LEN 12
#define VHT_OP_IE_LEN 5

#define	LDPC_VHT_ENABLE_RX			BIT0
#define	LDPC_VHT_ENABLE_TX			BIT1
#define	LDPC_VHT_TEST_TX_ENABLE		BIT2
#define	LDPC_VHT_CAP_TX				BIT3

#define	STBC_VHT_ENABLE_RX			BIT0
#define	STBC_VHT_ENABLE_TX			BIT1
#define	STBC_VHT_TEST_TX_ENABLE		BIT2
#define	STBC_VHT_CAP_TX				BIT3

/* VHT capability info */
#define SET_VHT_CAPABILITY_ELE_MAX_MPDU_LENGTH(_pEleStart, _val)			SET_BITS_TO_LE_1BYTE(_pEleStart, 0, 2, _val)
#define SET_VHT_CAPABILITY_ELE_CHL_WIDTH(_pEleStart, _val)			SET_BITS_TO_LE_1BYTE(_pEleStart, 2, 2, _val)
#define SET_VHT_CAPABILITY_ELE_RX_LDPC(_pEleStart, _val)			SET_BITS_TO_LE_1BYTE(_pEleStart, 4, 1, _val)
#define SET_VHT_CAPABILITY_ELE_TX_STBC(_pEleStart, _val)				SET_BITS_TO_LE_1BYTE(_pEleStart, 7, 1, _val)
#define SET_VHT_CAPABILITY_ELE_RX_STBC(_pEleStart, _val)				SET_BITS_TO_LE_1BYTE((_pEleStart)+1, 0, 3, _val)
#define SET_VHT_CAPABILITY_ELE_SU_BFER(_pEleStart, _val)				SET_BITS_TO_LE_1BYTE((_pEleStart)+1, 3, 1, _val)
#define SET_VHT_CAPABILITY_ELE_SU_BFEE(_pEleStart, _val)				SET_BITS_TO_LE_1BYTE((_pEleStart)+1, 4, 1, _val)
#define SET_VHT_CAPABILITY_ELE_BFER_ANT_SUPP(_pEleStart, _val)				SET_BITS_TO_LE_1BYTE((_pEleStart)+1, 5, 3, _val)
#define SET_VHT_CAPABILITY_ELE_SOUNDING_DIMENSIONS(_pEleStart, _val)				SET_BITS_TO_LE_1BYTE((_pEleStart)+2, 0, 3, _val)

#define SET_VHT_CAPABILITY_ELE_MU_BFER(_pEleStart, _val)				SET_BITS_TO_LE_1BYTE((_pEleStart)+2, 3, 1, _val)
#define SET_VHT_CAPABILITY_ELE_MU_BFEE(_pEleStart, _val)				SET_BITS_TO_LE_1BYTE((_pEleStart)+2, 4, 1, _val)
#define SET_VHT_CAPABILITY_ELE_TXOP_PS(_pEleStart, _val)				SET_BITS_TO_LE_1BYTE((_pEleStart)+2, 5, 1, _val)
#define SET_VHT_CAPABILITY_ELE_HTC_VHT(_pEleStart, _val)				SET_BITS_TO_LE_1BYTE((_pEleStart)+2, 6, 1, _val)
#define SET_VHT_CAPABILITY_ELE_MAX_RXAMPDU_FACTOR(_pEleStart, _val)		SET_BITS_TO_LE_2BYTE((_pEleStart)+2, 7, 3, _val) /* B23~B25 */
#define SET_VHT_CAPABILITY_ELE_LINK_ADAPTION(_pEleStart, _val)				SET_BITS_TO_LE_1BYTE((_pEleStart)+2, 2, 2, _val)
#define SET_VHT_CAPABILITY_ELE_MCS_RX_MAP(_pEleStart, _val)				SET_BITS_TO_LE_2BYTE((_pEleStart)+4, 0, 16, _val)   /* B0~B15 indicate Rx MCS MAP, we write 0 to indicate MCS0~7. by page */
#define SET_VHT_CAPABILITY_ELE_MCS_RX_HIGHEST_RATE(_pEleStart, _val)				SET_BITS_TO_LE_2BYTE((_pEleStart)+6, 0, 13, _val)
#define SET_VHT_CAPABILITY_ELE_MCS_TX_MAP(_pEleStart, _val)				SET_BITS_TO_LE_2BYTE((_pEleStart)+8, 0, 16, _val)   /* B0~B15 indicate Tx MCS MAP, we write 0 to indicate MCS0~7. by page */
#define SET_VHT_CAPABILITY_ELE_MCS_TX_HIGHEST_RATE(_pEleStart, _val)				SET_BITS_TO_LE_2BYTE((_pEleStart)+10, 0, 13, _val)


#define GET_VHT_CAPABILITY_ELE_MAX_MPDU_LENGTH(_pEleStart)			LE_BITS_TO_1BYTE(_pEleStart, 0, 2)
#define GET_VHT_CAPABILITY_ELE_CHL_WIDTH(_pEleStart)				LE_BITS_TO_1BYTE(_pEleStart, 2, 2)
#define GET_VHT_CAPABILITY_ELE_RX_LDPC(_pEleStart)			LE_BITS_TO_1BYTE(_pEleStart, 4, 1)
#define GET_VHT_CAPABILITY_ELE_TX_STBC(_pEleStart)				LE_BITS_TO_1BYTE(_pEleStart, 7, 1)
#define GET_VHT_CAPABILITY_ELE_RX_STBC(_pEleStart)				LE_BITS_TO_1BYTE((_pEleStart)+1, 0, 3)
#define GET_VHT_CAPABILITY_ELE_SU_BFER(_pEleStart)					LE_BITS_TO_1BYTE((_pEleStart)+1, 3, 1)
#define GET_VHT_CAPABILITY_ELE_SU_BFEE(_pEleStart)					LE_BITS_TO_1BYTE((_pEleStart)+1, 4, 1)
/*phydm-beamforming*/
#define GET_VHT_CAPABILITY_ELE_SU_BFEE_STS_CAP(_pEleStart)	LE_BITS_TO_2BYTE((_pEleStart)+1, 5, 3)
#define GET_VHT_CAPABILITY_ELE_SU_BFER_SOUND_DIM_NUM(_pEleStart)	LE_BITS_TO_2BYTE((_pEleStart)+2, 0, 3)
#define GET_VHT_CAPABILITY_ELE_MU_BFER(_pEleStart)				LE_BITS_TO_1BYTE((_pEleStart)+2, 3, 1)
#define GET_VHT_CAPABILITY_ELE_MU_BFEE(_pEleStart)				LE_BITS_TO_1BYTE((_pEleStart)+2, 4, 1)
#define GET_VHT_CAPABILITY_ELE_TXOP_PS(_pEleStart)				LE_BITS_TO_1BYTE((_pEleStart)+2, 5, 1)
#define GET_VHT_CAPABILITY_ELE_MAX_RXAMPDU_FACTOR(_pEleStart)	LE_BITS_TO_2BYTE((_pEleStart)+2, 7, 3)
#define GET_VHT_CAPABILITY_ELE_RX_MCS(_pEleStart)					       ((_pEleStart)+4)
#define GET_VHT_CAPABILITY_ELE_MCS_RX_HIGHEST_RATE(_pEleStart)			LE_BITS_TO_2BYTE((_pEleStart)+6, 0, 13)
#define GET_VHT_CAPABILITY_ELE_TX_MCS(_pEleStart)					       ((_pEleStart)+8)
#define GET_VHT_CAPABILITY_ELE_MCS_TX_HIGHEST_RATE(_pEleStart)			LE_BITS_TO_2BYTE((_pEleStart)+10, 0, 13)


/* VHT Operation Information Element */
#define SET_VHT_OPERATION_ELE_CHL_WIDTH(_pEleStart, _val)			SET_BITS_TO_LE_1BYTE(_pEleStart, 0, 8, _val)
#define SET_VHT_OPERATION_ELE_CHL_CENTER_FREQ1(_pEleStart, _val)			SET_BITS_TO_LE_1BYTE(_pEleStart+1, 0, 8, _val)
#define SET_VHT_OPERATION_ELE_CHL_CENTER_FREQ2(_pEleStart, _val)			SET_BITS_TO_LE_1BYTE(_pEleStart+2, 0, 8, _val)
#define SET_VHT_OPERATION_ELE_BASIC_MCS_SET(_pEleStart, _val)			SET_BITS_TO_LE_2BYTE((_pEleStart)+3, 0, 16, _val)

#define GET_VHT_OPERATION_ELE_CHL_WIDTH(_pEleStart)		LE_BITS_TO_1BYTE(_pEleStart, 0, 8)
#define GET_VHT_OPERATION_ELE_CENTER_FREQ1(_pEleStart)	LE_BITS_TO_1BYTE((_pEleStart)+1, 0, 8)
#define GET_VHT_OPERATION_ELE_CENTER_FREQ2(_pEleStart)     LE_BITS_TO_1BYTE((_pEleStart)+2, 0, 8)

/* VHT Operating Mode */
#define SET_VHT_OPERATING_MODE_FIELD_CHNL_WIDTH(_pEleStart, _val)		SET_BITS_TO_LE_1BYTE(_pEleStart, 0, 2, _val)
#define SET_VHT_OPERATING_MODE_FIELD_RX_NSS(_pEleStart, _val)			SET_BITS_TO_LE_1BYTE(_pEleStart, 4, 3, _val)
#define SET_VHT_OPERATING_MODE_FIELD_RX_NSS_TYPE(_pEleStart, _val)	SET_BITS_TO_LE_1BYTE(_pEleStart, 7, 1, _val)
#define GET_VHT_OPERATING_MODE_FIELD_CHNL_WIDTH(_pEleStart)			LE_BITS_TO_1BYTE(_pEleStart, 0, 2)
#define GET_VHT_OPERATING_MODE_FIELD_RX_NSS(_pEleStart)				LE_BITS_TO_1BYTE(_pEleStart, 4, 3)
#define GET_VHT_OPERATING_MODE_FIELD_RX_NSS_TYPE(_pEleStart)		LE_BITS_TO_1BYTE(_pEleStart, 7, 1)

#define SET_EXT_CAPABILITY_ELE_OP_MODE_NOTIF(_pEleStart, _val)			SET_BITS_TO_LE_1BYTE((_pEleStart)+7, 6, 1, _val)
#define GET_EXT_CAPABILITY_ELE_OP_MODE_NOTIF(_pEleStart)				LE_BITS_TO_1BYTE((_pEleStart)+7, 6, 1)

#endif

