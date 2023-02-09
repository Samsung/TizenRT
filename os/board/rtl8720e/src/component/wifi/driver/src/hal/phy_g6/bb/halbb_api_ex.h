/******************************************************************************
 *
 * Copyright(c) 2020 Realtek Corporation.
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
 *****************************************************************************/
#ifndef _HALBB_API_EX_H_
#define _HALBB_API_EX_H_
#include "halbb_ic_hw_info.h"
#include "halbb_api.h"
/*@--------------------------[Prptotype]-------------------------------------*/
struct bb_info;
struct bb_mcc_i {
	enum role_type type;
	struct rtw_chan_def *chandef;
	u32 *macid_bitmap;
	u8 macid_map_len;
	u8 self_macid;
};

u16 halbb_cfg_cmac_tx_ant(struct bb_info *bb, enum rf_path tx_path);
void halbb_gpio_setting_all(struct bb_info *bb, u8 rfe_idx);
void halbb_gpio_setting(struct bb_info *bb, u8 gpio_idx, enum bb_path path,
			bool inv, enum bb_rfe_src_sel src);
u8 halbb_get_txsc(struct bb_info *bb, u8 pri_ch, u8 central_ch,
		  enum channel_width cbw, enum channel_width dbw);

void halbb_reset_bb(struct bb_info *bb);

u32 halbb_read_rf_reg(struct bb_info *bb, enum rf_path path, u32 addr, u32 mask);

bool halbb_write_rf_reg(struct bb_info *bb, enum rf_path path, u32 addr, u32 mask,
			u32 data);
bool halbb_rf_set_bb_reg(struct bb_info *bb, u32 addr, u32 bit_mask, u32 data);
u32 halbb_rf_get_bb_reg(struct bb_info *bb, u32 addr, u32 mask);
void halbb_ctrl_rf_mode(struct bb_info *bb, enum phl_rf_mode mode);

bool halbb_ctrl_rx_path(struct bb_info *bb, enum rf_path rx_path);

bool halbb_ctrl_tx_path(struct bb_info *bb, enum rf_path rx_path);

void halbb_ctrl_trx_path(struct bb_info *bb, enum rf_path tx_path, u8 tx_nss,
			 enum rf_path rx_path, u8 rx_nss);

void halbb_tssi_bb_reset(struct bb_info *bb);

void halbb_dfs_en(struct bb_info *bb, bool en);

void halbb_adc_en(struct bb_info *bb, bool en);

void halbb_tssi_cont_en(struct bb_info *bb, bool en, enum rf_path path);

void halbb_bb_reset_en(struct bb_info *bb, bool en, enum phl_phy_idx phy_idx);

bool halbb_ctrl_bw(struct bb_info *bb, u8 pri_ch, enum band_type band, enum channel_width bw,
		   enum phl_phy_idx phy_idx);

bool halbb_ctrl_bw_ch(struct bb_info *bb, u8 pri_ch, u8 central_ch_seg0,
		      u8 central_ch_seg1, enum band_type band,
		      enum channel_width bw, enum phl_phy_idx phy_idx);

void halbb_ctrl_rx_cca(struct bb_info *bb, bool cca_en, enum phl_phy_idx phy_idx);

bool halbb_query_cck_en(struct bb_info *bb, enum phl_phy_idx phy_idx);

void halbb_ctrl_cck_en(struct bb_info *bb, bool cck_enable,
		       enum phl_phy_idx phy_idx);

void halbb_ctrl_ofdm_en(struct bb_info *bb, bool ofdm_enable,
			enum phl_phy_idx phy_idx);

void halbb_ctrl_btg(struct bb_info *bb, bool btg);

void halbb_ctrl_btc_preagc(struct bb_info *bb, bool bt_en);

void halbb_pop_en(struct bb_info *bb, bool en, enum phl_phy_idx phy_idx);

bool halbb_querry_pop_en(struct bb_info *bb, enum phl_phy_idx phy_idx);

bool halbb_set_pd_lower_bound(struct bb_info *bb, u8 bound,
			      enum channel_width bw, enum phl_phy_idx phy_idx);

bool halbb_set_pd_lower_bound_cck(struct bb_info *bb, u8 bound,
			      enum channel_width bw, enum phl_phy_idx phy_idx);

u8 halbb_querry_pd_lower_bound(struct bb_info *bb, bool get_en_info,
			       enum phl_phy_idx phy_idx);

u8 halbb_get_losel(struct bb_info *bb);

void halbb_set_igi(struct bb_info *bb, u8 lna_idx, bool tia_idx, u8 rxbb_idx,
		   enum rf_path path);
void halbb_set_tx_pow_pattern_shap(struct bb_info *bb, u8 ch,
				   bool is_ofdm, enum phl_phy_idx phy_idx);
void halbb_set_tx_pow_ref(struct bb_info *bb, enum phl_phy_idx phy_idx);

void halbb_normal_efuse_verify(struct bb_info *bb, s8 rx_gain_offset,
			       enum rf_path rx_path, enum phl_phy_idx phy_idx);

u8 halbb_upd_mcc_macid(struct bb_info *bb, struct bb_mcc_i *mi);

void halbb_mcc_stop(struct bb_info *bb);

u8 halbb_mcc_start(struct bb_info *bb, struct bb_mcc_i *mi_1,
		   struct bb_mcc_i *mi_2);

void halbb_normal_efuse_verify_cck(struct bb_info *bb, s8 rx_gain_offset,
				   enum rf_path rx_path,
				   enum phl_phy_idx phy_idx);
#endif