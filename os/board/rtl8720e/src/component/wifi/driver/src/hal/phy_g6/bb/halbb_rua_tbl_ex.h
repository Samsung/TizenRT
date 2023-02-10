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
#ifndef __HALBB_RUA_TBL_EX_H__
#define __HALBB_RUA_TBL_EX_H__
#ifdef HALBB_RUA_SUPPORT
/*@--------------------------[Define] ---------------------------------------*/
#define HALBB_AX4RU_STA_NUM 4
#define HALBB_AX8RU_STA_NUM 8
/*@--------------------------[Enum]------------------------------------------*/
enum rtw_rua_tbl_hdr_rw {
        RUA_TBL_RW_READ = 0,
        RUA_TBL_RW_WRITE = 1
};
enum rtw_rua_tbl_hdr_type {
        RUA_TBL_TYPE_SW = 0,
        RUA_TBL_TYPE_HW = 1
};
enum rtw_rua_tbl_hdr_class {
        RUA_TBL_CL_DLRU_SW = 0x0,
        RUA_TBL_CL_ULRU_SW  = 0x1,
        RUA_TBL_CL_RU_STA = 0x2,
        RUA_TBL_CL_DLRU_SW_FIX = 0x3,
        RUA_TBL_CL_ULRU_SW_FIX = 0x4,
        RUA_TBL_CL_BA_INFO = 0x5
};
/*@--------------------------[Structure]-------------------------------------*/
 struct rtw_rua_tbl_hdr {
	u8 rw:1;
	u8 idx:7;

	u16 offset:5;
	u16 len:10;
	u16 type:1;

	u8 tbl_class;
};

struct rtw_ru_rate_ent {
	u8 dcm:1;
	u8 ss:3;
	u8 mcs:4;
};

struct rtw_tf_ba_tbl {
	u32 fix_ba:1;
	u32 ru_psd:9;
	u32 tf_rate:9;
	u32 rf_gain_fix:1;
	u32 rf_gain_idx:10;
	u32 tb_ppdu_bw:2;

	struct rtw_ru_rate_ent rate;
	
	u8 gi_ltf:3;
	u8 doppler:1;
	u8 stbc:1;
	u8 sta_coding:1;
	u8 tb_t_pe_nom:2;

	u8 pr20_bw_en:1;
	u8 ma_type:1;
	u8 rsvd1:6;
	
	u8 rsvd2;
};

struct rtw_dl_ru_gp_tbl {

	struct rtw_rua_tbl_hdr tbl_hdr;
	
	u16 ppdu_bw:2;
	u16 tx_pwr:9;
	u16 pwr_boost_fac:5;

	u8 fix_mode_flag:1;
	u8 rsvd1:7;

	u8 rsvd2;
	
	struct rtw_tf_ba_tbl tf;
};

struct rtw_ul_ru_gp_tbl {
	
	struct rtw_rua_tbl_hdr tbl_hdr;
	u32 grp_psd_max:9;
	u32 grp_psd_min:9;
	u32 tf_rate:9;
	u32 fix_tf_rate:1;
	u32 rsvd2:4;
	
	u16 ppdu_bw:2;
	u16 rf_gain_fix:1;
	u16 rf_gain_idx:10;
	u16 fix_mode_flag:1;
	u16 rsvd1:2;
};

struct rtw_ru_sta_info {
	
	struct rtw_rua_tbl_hdr tbl_hdr;
/* sta capability */
	u8 gi_ltf_48spt:1;
	u8 gi_ltf_18spt:1;
	u8 rsvd0:6;
/* dl su */
	u8 dlsu_info_en:1;
	u8 dlsu_bw:2;
	u8 dlsu_gi_ltf:3;
	u8 dlsu_doppler_ctrl:2;

	u8 dlsu_coding:1;
	u8 dlsu_txbf:1;
	u8 dlsu_stbc:1;
	u8 dl_fwcqi_flag:1;
	u8 dlru_ratetbl_ridx:4;
	u8 csi_info_bitmap;
	u32 dl_swgrp_bitmap;
	
	u16 dlsu_dcm:1;
	u16 rsvd1:6;
	u16 dlsu_rate:9;
	u8 dlsu_pwr:6;
	u8 rsvd2:2;
	u8 rsvd4;
/* ul su */
	u8 ulsu_info_en:1;
	u8 ulsu_bw:2;
	u8 ulsu_gi_ltf:3;
	u8 ulsu_doppler_ctrl:2;
	u8 ulsu_dcm:1;
	u8 ulsu_ss:3;
	u8 ulsu_mcs:4;
	u16 ul_fwcqi_flag:1;
	u16 ulru_ratetbl_ridx:4;
	u16 ulsu_stbc:1;
	u16 ulsu_coding:1;
	u16 ulsu_rssi_m:9;
	u32 ul_swgrp_bitmap;
/* tb info */
};

/*
struct rtw_dl_fix_sta_ent {
	u8 mac_id;
	u8 ru_pos[3];

	u8 fix_rate:1;
	u8 fix_coding:1;
	u8 fix_txbf:1;
	u8 fix_pwr_fac:1;
	u8 rsvd0:4;
	
	struct rtw_ru_rate_ent rate;
	
	u8 txbf:1;
	u8 coding:1;
	u8 pwr_boost_fac:5;
	u8 rsvd1: 1;
	u8 rsvd2;

};

struct rtw_dl_ru_fix_tbl {
	
	struct rtw_rua_tbl_hdr tbl_hdr;
	u8 max_sta_num:3;
	u8 min_sta_num:3;
	u8 ru_swp_flg:1;
	u8 rsvd0:1;

	u8 doppler:1;
	u8 stbc:1;
	u8 gi_ltf:3;
	u8 ma_type:1;
	u8 fixru_flag:1;
	u8 rupos_csht_flag:1;

	u8 rsvd2;

	struct rtw_dl_fix_sta_ent sta[HALBB_AX4RU_STA_NUM];

};
*/

struct rtw_dlfix_sta_i_ax4ru {
	u8 mac_id;
	u8 ru_pos[3];

	u8 fix_rate:1;
	u8 fix_coding:1;
	u8 fix_txbf:1;
	u8 fix_pwr_fac:1;
	u8 rsvd0:4;
	
	struct rtw_ru_rate_ent rate;
	
	u8 txbf:1;
	u8 coding:1;
	u8 pwr_boost_fac:5;
	u8 rsvd1: 1;
	u8 rsvd2;

};

struct rtw_dlfix_sta_i_ax8ru {
	u8 mac_id;
	u8 ru_pos[7];

	u8 fix_rate:1;
	u8 fix_coding:1;
	u8 fix_txbf:1;
	u8 fix_pwr_fac:1;
	u8 rsvd0:4;
	
	struct rtw_ru_rate_ent rate;
	
	u8 txbf:1;
	u8 coding:1;
	u8 pwr_boost_fac:5;
	u8 rsvd1: 1;
	u8 rsvd2;

};

struct rtw_dlru_fixtbl_ax4ru {
	
	struct rtw_rua_tbl_hdr tbl_hdr;
	u8 max_sta_num:3;
	u8 min_sta_num:3;
	u8 ru_swp_flg:1;
	u8 rsvd0:1;

	u8 doppler:1;
	u8 stbc:1;
	u8 gi_ltf:3;
	u8 ma_type:1;
	u8 fixru_flag:1;
	u8 rupos_csht_flag:1;

	u8 rsvd2;

	struct rtw_dlfix_sta_i_ax4ru sta[HALBB_AX4RU_STA_NUM];
};
struct rtw_dlru_fixtbl_ax8ru {
	
	struct rtw_rua_tbl_hdr tbl_hdr;
	u8 max_sta_num:4;
	u8 min_sta_num:4;
	
	u8 doppler:1;
	u8 stbc:1;
	u8 gi_ltf:3;
	u8 ma_type:1;
	u8 fixru_flag:1;
	u8 rupos_csht_flag:1;
	
	u8 ru_swp_flg:1;
	u8 rsvd1:7;

	u8 rsvd2;

	struct rtw_dlfix_sta_i_ax8ru sta[HALBB_AX8RU_STA_NUM];
};

union rtw_dlru_fixtbl{
	struct rtw_dlru_fixtbl_ax4ru ax4ru;
	struct rtw_dlru_fixtbl_ax8ru ax8ru;
};

/*
struct rtw_ul_fix_sta_ent {
	u8 mac_id;
	u8 ru_pos[3];
	u8 tgt_rssi[3];
	u8 fix_tgt_rssi:1;
	u8 fix_rate:1;
	u8 fix_coding:1;
	u8 coding:1;
	u8 rsvd1:4;
	struct rtw_ru_rate_ent rate;
};

struct rtw_ul_ru_fix_tbl {

	struct rtw_rua_tbl_hdr tbl_hdr;
	u8 max_sta_num:3;
	u8 min_sta_num:3;
	u8 doppler:1;
	u8 ma_type:1;
	u8 gi_ltf:3;
	u8 stbc:1;
	u8 fix_tb_t_pe_nom: 1;
	u8 tb_t_pe_nom: 2;
	u8 fixru_flag: 1;
	struct rtw_ul_fix_sta_ent sta[HALBB_AX4RU_STA_NUM];

};
*/

struct rtw_ulfix_sta_i_ax4ru {
	u8 mac_id;
	u8 ru_pos[3];
	u8 tgt_rssi[3];
	u8 fix_tgt_rssi:1;
	u8 fix_rate:1;
	u8 fix_coding:1;
	u8 coding:1;
	u8 rsvd1:4;
	struct rtw_ru_rate_ent rate;
};

struct rtw_ulfix_sta_i_ax8ru {
	u8 mac_id;
	u8 ru_pos[7];
	u8 rsvd1;
	u8 tgt_rssi[7];

	u8 fix_tgt_rssi:1;
	u8 fix_rate:1;
	u8 fix_coding:1;
	u8 coding:1;
	u8 rsvd2:4;
	struct rtw_ru_rate_ent rate;
	u8 rsvd3;
	u8 rsvd4;
};

struct rtw_ulru_fixtbl_ax4ru {

	struct rtw_rua_tbl_hdr tbl_hdr;
	u8 max_sta_num:3;
	u8 min_sta_num:3;
	u8 doppler:1;
	u8 ma_type:1;
	u8 gi_ltf:3;
	u8 stbc:1;
	u8 fix_tb_t_pe_nom: 1;
	u8 tb_t_pe_nom: 2;
	u8 fixru_flag: 1;
	struct rtw_ulfix_sta_i_ax4ru sta[HALBB_AX4RU_STA_NUM];

};

struct rtw_ulru_fixtbl_ax8ru {

	struct rtw_rua_tbl_hdr tbl_hdr;
	u8 max_sta_num:4;
	u8 min_sta_num:4;
	
	u8 gi_ltf:3;
	u8 stbc:1;
	u8 fix_tb_t_pe_nom: 1;
	u8 tb_t_pe_nom: 2;
	u8 fixru_flag: 1;

	u8 doppler:1;
	u8 ma_type:1;
	u8 rsvd1:6;
	
	u8 rsvd2;
	struct rtw_ulfix_sta_i_ax8ru sta[HALBB_AX8RU_STA_NUM];

};

union rtw_ulru_fixtbl{
	struct rtw_ulru_fixtbl_ax4ru ax4ru;
	struct rtw_ulru_fixtbl_ax8ru ax8ru;
};

struct rtw_ba_tbl_info {
	struct rtw_rua_tbl_hdr tbl_hdr;
	
	struct rtw_tf_ba_tbl tf_ba_t;
};

struct rtw_sw_grp_bitmap {
	u8 macid;

	u8 en_upd_dl_swgrp:1;
	u8 en_upd_ul_swgrp:1;
	u8 cmdend:1; // add for determine whether last user or not
	u8 rsvd1:5;
	
	u32 dl_sw_grp_bitmap;
	u32 ul_sw_grp_bitmap;
};

struct rtw_sw_grp_set {
	
	struct rtw_sw_grp_bitmap swgrp_bitmap[8];
	
};

struct rtw_dl_macid_cfg {
	
    u32 macid: 8;
    u32 dl_su_rate_cfg: 1;
    u32 dl_su_rate: 9;
    u32 dl_su_bw: 2;
    u32 dl_su_pwr_cfg: 1;
    u32 dl_su_pwr: 6;
    u32 rsvd0: 5;

    u32 gi_ltf_4x8_support: 1;
    u32 gi_ltf_1x8_support: 1;
    u32 rsvd1: 6;
    u32 dl_su_info_en: 1;
    u32 rsvd2: 2;
    u32 dl_su_gi_ltf: 3;
    u32 dl_su_doppler_ctrl: 2;
    u32 dl_su_coding: 1;
    u32 dl_su_txbf: 1;
    u32 dl_su_stbc: 1;
    u32 dl_su_dcm: 1;
    u32 rsvd3: 12;

};


struct rtw_ul_macid_cfg {
	
    u32 macid: 8;
    u32 endcmd: 1;
    u32 rsvd0: 23;

    u32 ul_su_info_en: 1;
    u32 ul_su_bw: 2;
    u32 ul_su_gi_ltf: 3;
    u32 ul_su_doppler_ctrl: 2;
    u32 ul_su_dcm: 1;
    u32 ul_su_ss: 3;
    u32 ul_su_mcs: 4;
    u32 rsvd2: 5;
    u32 ul_su_stbc: 1;
    u32 ul_su_coding: 1;
    u32 ul_su_rssi_m: 9;

};

struct rtw_ul_macid_set {
	
    struct rtw_ul_macid_cfg ul_macid_cfg[8];

};

struct rtw_csiinfo_cfg {
    u32 macid: 8;
    u32 csi_info_bitmap: 8;
    u32 rsvd0: 16;
};


struct rtw_cqi_info {
    u32 macid: 8;
    u32 fw_cqi_flag: 1; /* UL or DL*/
    u32 ru_rate_table_row_idx: 4; /* UL or DL*/
    u32 ul_dl: 1; /*1'b0 means UL, 1'b1 means DL */
    u32 endcmd: 1;
    u32 rsvd0: 1;
    u32 rsvd1: 16;

    s8 cqi_diff_table[19]; /* UL or DL*/
    u8 rsvd2;
};

struct rtw_cqi_set{	
    struct rtw_cqi_info cqi_info[8];
};

struct rtw_bbinfo_cfg {
    u32 p20_ch_bitmap: 8;
    u32 rsvd0: 24;
};

struct rtw_pwr_by_rt_tbl{
    s16 pwr_by_rt[32];
};

 
/*@--------------------------[Prptotype]-------------------------------------*/
struct bb_info;
// u32 halbb_upd_dlru_fixtbl(struct bb_info *bb,
// 			struct rtw_dl_ru_fix_tbl *info);
u32 halbb_upd_dlru_fixtbl(struct bb_info *bb,
			union rtw_dlru_fixtbl *union_info);
u32 halbb_upd_dlru_grptbl(struct bb_info *bb,
			struct rtw_dl_ru_gp_tbl *info);
// u32 halbb_upd_ulru_fixtbl(struct bb_info *bb,
// 			struct rtw_ul_ru_fix_tbl *info);
u32 halbb_upd_ulru_fixtbl(struct bb_info *bb,
			union rtw_ulru_fixtbl *union_info);			
u32 halbb_upd_ulru_grptbl(struct bb_info *bb,
			struct rtw_ul_ru_gp_tbl *info);
u32 halbb_upd_rusta_info(struct bb_info *bb,
		       struct rtw_ru_sta_info *info);
u32 halbb_upd_ba_infotbl(struct bb_info *bb,
		       struct rtw_ba_tbl_info *info);
u32 halbb_swgrp_hdl(struct bb_info *bb,
		       struct rtw_sw_grp_set *info);

u32 halbb_dlmacid_cfg(struct bb_info *bb, struct rtw_dl_macid_cfg *cfg);

u32 halbb_ulmacid_cfg(struct bb_info *bb, struct rtw_ul_macid_set *cfg);

u32 halbb_csiinfo_cfg(struct bb_info *bb, struct rtw_csiinfo_cfg *cfg);

u32 halbb_cqi_cfg(struct bb_info *bb, struct rtw_cqi_set *cfg);

u32 halbb_bbinfo_cfg(struct bb_info *bb, struct rtw_bbinfo_cfg *cfg);

u32 halbb_pbr_tbl_cfg(struct bb_info *bb, struct rtw_pwr_by_rt_tbl *cfg);

/*u32 halbb_rua_tbl_init(struct bb_info *bb);*/
#endif
#endif
