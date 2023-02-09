/******************************************************************************
 *
 * Copyright(c) 2007 - 2020  Realtek Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * The full GNU General Public License is included in this distribution in the
 * file called LICENSE.
 *
 * Contact Information:
 * wlanfae <wlanfae@realtek.com>
 * Realtek Corporation, No. 2, Innovation Road II, Hsinchu Science Park,
 * Hsinchu 300, Taiwan.
 *
 * Larry Finger <Larry.Finger@lwfinger.net>
 *
 *****************************************************************************/
#ifndef __HALBB_FWOFLD_H__
#define __HALBB_FWOFLD_H__

/*@--------------------------[Define] ---------------------------------------*/

/*@--------------------------[Enum]------------------------------------------*/

/*@--------------------------[Structure]-------------------------------------*/


struct bb_info;
/*@--------------------------[Prptotype]-------------------------------------*/
#ifdef HALBB_FW_OFLD_SUPPORT
bool halbb_fw_set_reg(struct bb_info *bb, u32 addr, u32 mask, u32 val, u8 lc);
bool halbb_fwcfg_bb_phy_8852a_2(struct bb_info *bb, u32 addr, u32 data,
			    enum phl_phy_idx phy_idx);
bool halbb_fwofld_bw_ch_8852a_2(struct bb_info *bb, u8 pri_ch, u8 central_ch,
			    enum channel_width bw, enum phl_phy_idx phy_idx);
void halbb_fwofld_set_pmac_tx_8852a_2(struct bb_info *bb,
			     struct halbb_pmac_info *tx_info,
			     enum phl_phy_idx phy_idx);
#endif
#endif