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
#ifndef __RTW_LAYER2_RM_H_
#define __RTW_LAYER2_RM_H_

#ifdef CONFIG_LAYER2_ROAMING
#define rtw_roam_flags(adapter)		(rtw_get_rmpriv(adapter)->roam_flags)
#define rtw_chk_roam_flags(adapter, flags) (rtw_roam_flags(adapter) & flags)
#define rtw_clr_roam_flags(adapter, flags) \
	do { \
		struct rm_priv *prmpriv = rtw_get_rmpriv(adapter); \
		(prmpriv->roam_flags &= ~flags); \
	} while (0)

#define rtw_set_roam_flags(adapter, flags) \
	do { \
		struct rm_priv *prmpriv = rtw_get_rmpriv(adapter); \
		(prmpriv->roam_flags |= flags); \
	} while (0)

#define rtw_assign_roam_flags(adapter, flags) \
	do { \
		struct rm_priv *prmpriv = rtw_get_rmpriv(adapter); \
		(prmpriv->roam_flags = flags); \
	} while (0)

int is_same_ess(WLAN_BSSID_EX *a, WLAN_BSSID_EX *b);
void _rtw_roaming(_adapter *adapter, struct wlan_network *tgt_network);
void rtw_roaming(_adapter *adapter, struct wlan_network *tgt_network);
void rtw_set_to_roam(_adapter *adapter, u8 to_roam);
u8 rtw_dec_to_roam(_adapter *adapter);
u8 rtw_to_roam(_adapter *adapter);
void rtw_roaming_sitesurvey_again(_adapter *padapter);
void rtw_start_roam(_adapter *padapter);
void rtw_drv_scan_by_self(_adapter *padapter, u8 reason);
void rtw_roamimg_info_init(_adapter *padapter);
int rtw_select_roaming_candidate(struct mlme_priv *pmlmepriv);
#endif /* CONFIG_LAYER2_ROAMING */


#endif //__RTW_LAYER2_RM_H_

