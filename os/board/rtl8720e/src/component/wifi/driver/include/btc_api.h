/******************************************************************************
 *
 * Copyright(c) 2013 Realtek Corporation. All rights reserved.
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
#ifndef __BTC_API_H__
#define __BTC_API_H__

#include <drv_types.h>
#include "bt_intf.h"


#define	PACKET_NORMAL			0
#define	PACKET_DHCP				1
#define	PACKET_ARP				2
#define	PACKET_EAPOL			3

void rtw_hal_btc_Initialize(PADAPTER padapter);
void rtw_hal_btc_DeInitialize(PADAPTER);
void rtw_hal_btc_PowerOnSetting(PADAPTER padapter);
void rtw_hal_btc_HAL_Initialize(PADAPTER padapter, u8 bWifiOnly);
void rtw_hal_btc_IpsNotify(PADAPTER, u8 type);
void rtw_hal_btc_LpsNotify(PADAPTER, u8 type);
void rtw_hal_btc_ScanNotify(PADAPTER, u8 type);
void rtw_hal_btc_set_ant_info(PADAPTER padapter);
void rtw_hal_btc_ConnectNotify(PADAPTER, u8 action);
void rtw_hal_btc_MediaStatusNotify(PADAPTER, u8 mediaStatus);
void rtw_hal_btc_SpecialPacketNotify(PADAPTER, u8 pktType);
void rtw_hal_btc_BtInfoNotify(PADAPTER, u8 length, u8 *tmpBuf);
void rtw_hal_btc_HaltNotify(PADAPTER);
void rtw_hal_btc_switchband_notify(u8 under_scan, u8 band_type);
void rtw_hal_btc_switchchnl_notify(u8 channel, u8 band_type);
u8 rtw_hal_btc_BtRFK(PADAPTER padapter, struct bt_rfk_param *rfk_param);
u8 rtw_hal_btc_IsBtDisabled(PADAPTER);
void rtw_hal_btc_Handler(PADAPTER);
u8 rtw_hal_btc_IsBtControlLps(PADAPTER);
u8 rtw_hal_btc_IsLpsOn(PADAPTER);
u8 rtw_hal_btc_RpwmVal(PADAPTER);
u8 rtw_hal_btc_LpsVal(PADAPTER);
void rtw_hal_btc_RecordPwrMode(PADAPTER, u8 *pCmdBuf, u8 cmdLen);
void rtw_hal_btc_LPS_Enter(PADAPTER);
void rtw_hal_btc_LPS_Leave(PADAPTER);
u8 rtw_hal_btc_get_pg_rfe_type(PADAPTER padapter);
u8 rtw_hal_btc_get_ant_div_cfg(PADAPTER padapter);

#endif /* __BTC_API_H__ */
