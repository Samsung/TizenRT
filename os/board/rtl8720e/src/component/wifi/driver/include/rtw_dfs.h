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
#ifndef __RTW_DFS_H_
#define __RTW_DFS_H_

#define CAC_TIME_MS (60*1000)
#define CAC_TIME_CE_MS (10*60*1000)
#define NON_OCP_TIME_MS (30*60*1000)

void rtw_rfctl_init(_adapter *adapter);
void rtw_rfctl_deinit(_adapter *adapter);

#define CH_IS_NON_OCP(rt_ch_info) 0
#define rtw_chset_is_chbw_non_ocp(ch_set, ch, bw, offset) _FALSE
#define rtw_chset_is_ch_non_ocp(ch_set, ch) _FALSE
#define rtw_rfctl_is_tx_blocked_by_ch_waiting(rfctl) _FALSE

#if (PHYDM_VERSION == 2)
u8 rtw_odm_get_dfs_domain(_adapter *adapter);
u8 rtw_odm_dfs_domain_unknown(_adapter *adapter);
#endif
bool rtw_adjust_chbw(_adapter *adapter, u8 req_ch, u8 *req_bw, u8 *req_offset);

#endif

