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
#ifndef __HALBB_RUA_TBL_B_ENDIAN_H__
#define __HALBB_RUA_TBL_B_ENDIAN_H__

#ifdef HALBB_RUA_SUPPORT
/*@--------------------------[Define] ---------------------------------------*/
/*[IO Reg]*/

/*@--------------------------[Enum]------------------------------------------*/



/*@--------------------------[Structure]-------------------------------------*/

	
struct halbb_rua_tbl_hdr_info {
	u8 idx:7;
	u8 rw:1;

	u8 len_l:3;
	u8 offset:5;

	u8 type:1;
	u8 len_m:7;

	u8 tbl_class;
};

struct halbb_ru_rate_info {
	u8 mcs:4;
	u8 ss:3;
	u8 dcm:1;
};



struct halbb_tf_ba_tbl_info {
	u8 ru_psd_l:7;
	u8 fix_ba:1;
	
	u8 tf_rate_l:6;
	u8 ru_psd_m:2;

	u8 rf_gain_idx_l:4;
	u8 rf_gain_fix:1;
	u8 tf_rate_m:3;

	u8 tb_ppdu_bw:2;
	u8 rf_gain_idx_m:6;

	struct halbb_ru_rate_info rate_i;
	
	u8 tb_t_pe_nom:2;
	u8 sta_coding:1;
	u8 stbc:1;
	u8 doppler:1;
	u8 gi_ltf:3;

	u8 rsvd1: 6;
	u8 ma_type: 1;
	u8 pr20_bw_en:1;

	u8 rsvd2;
};

struct halbb_dl_ru_gp_tbl_info {

	struct halbb_rua_tbl_hdr_info tbl_hdr_i;
	
	u8 tx_pwr_l:6;
	u8 ppdu_bw:2;
	
	u8 pwr_boost_fac:5;
	u8 tx_pwr_m:3;
	
	u8 rsvd1:7;
	u8 fix_mode_flag:1;

	u8 rsvd2;
	
	struct halbb_tf_ba_tbl_info tf;
};



struct halbb_ul_ru_gp_tbl_info {
	
	struct halbb_rua_tbl_hdr_info tbl_hdr_i;
	u8 grp_psd_max_l;
	
	u8 grp_psd_min_l:7;
	u8 grp_psd_max_m:1;

	u8 tf_rate_l:6;
	u8 grp_psd_min_m:2;
	
	u8 rsvd2:4;
	u8 fix_tf_rate:1;
	u8 tf_rate_m:3;
	
	u8 rf_gain_idx_l:5;
	u8 rf_gain_fix:1;
	u8 ppdu_bw:2;
	
	u8 rsvd1: 2;
	u8 fix_mode_flag: 1;
	u8 rf_gain_idx_m:5;

	u8 rsvd3;

	u8 rsvd4;
};

struct halbb_ru_sta_info {
	
	struct halbb_rua_tbl_hdr_info tbl_hdr_i;
/* sta capability */
	u8 rsvd0:6;
	u8 gi_ltf_18spt:1;
	u8 gi_ltf_48spt:1;

/* dl su */
	u8 dlsu_doppler_ctrl:2;
	u8 dlsu_gi_ltf:3;
	u8 dlsu_bw:2;
	u8 dlsu_info_en:1;
	
	u8 dlru_ratetbl_ridx:4;
	u8 dl_fwcqi_flag:1;
	u8 dlsu_stbc:1;
	u8 dlsu_txbf:1;
	u8 dlsu_coding:1;

	u8 csi_info_bitmap;

	u8 dl_swgrp_bitmap[4];
	
	u8 dlsu_rate_l:1;
	u8 rsvd1:6;
	u8 dlsu_dcm:1;

	u8 dlsu_rate_m;

	u8 rsvd2:2;
	u8 dlsu_pwr:6;

	u8 rsvd4;
/* ul su */
	u8 ulsu_doppler_ctrl:2;
	u8 ulsu_gi_ltf:3;
	u8 ulsu_bw:2;
	u8 ulsu_info_en:1;

	u8 ulsu_mcs:4;
	u8 ulsu_ss:3;
	u8 ulsu_dcm:1;

	u8 ulsu_rssi_m_l:1;
	u8 ulsu_coding:1;
	u8 ulsu_stbc:1;
	u8 ulru_ratetbl_ridx:4;
	u8 ul_fwcqi_flag:1;
	
	u8 ulsu_rssi_m_m;
	
	u8 ul_swgrp_bitmap[4];
/* tb info */
};
	
struct halbb_dl_fix_sta_info {
	u8 mac_id;
	u8 ru_pos[3];
	
	u8 rsvd0: 4;
	u8 fix_pwr_fac:1;
	u8 fix_txbf:1;
	u8 fix_coding:1;
	u8 fix_rate:1;
	
	struct halbb_ru_rate_info rate_i;
	
	u8 rsvd1: 1;
	u8 pwr_boost_fac:5;
	u8 coding:1;
	u8 txbf:1;
	u8 rsvd2;
};

struct halbb_dl_fix_sta_info_8ru {
	u8 mac_id;
	u8 ru_position[7];
	
	u8 rsvd0: 4;
	u8 fix_pwr_boost_factor_flag:1;
	u8 fix_txbf_flag:1;
	u8 fix_coding_flag:1;
	u8 fix_rate_flag:1;
	
	struct halbb_ru_rate_info rate_i;
	
	u8 rsvd1: 1;
	u8 pwr_boost_factor:5;
	u8 coding:1;
	u8 txbf:1;
	u8 rsvd2;
};

struct halbb_dl_ru_fix_tbl_info {
	
	struct halbb_rua_tbl_hdr_info tbl_hdr_i;
	u8 rsvd0:1;
	u8 ru_swp_flg:1;
	u8 min_sta_num:3;
	u8 max_sta_num:3;

	u8 rupos_csht_flag:1;
	u8 fixru_flag:1;
	u8 ma_type:1;
	u8 gi_ltf:3;
	u8 stbc:1;
	u8 doppler:1;

	u8 rsvd2;
	u8 rsvd3;
	struct halbb_dl_fix_sta_info sta_i[HALBB_AX4RU_STA_NUM];

};

struct halbb_dl_ru_fix_tbl_info_8ru {
	
	struct halbb_rua_tbl_hdr_info tbl_hdr_i;
	u8 min_sta_num:4;
	u8 max_sta_num:4;
	
	u8 rupos_csht_flg:1;
	u8 fix_ru_flag:1;
	u8 ma_type:1;
	u8 gi_ltf:3;
	u8 stbc:1;
	u8 doppler:1;
	
	u8 rsvd0:7;
	u8 ru_swp_flg:1;
	
	u8 rsvd1;

	struct halbb_dl_fix_sta_info_8ru sta_i[HALBB_AX8RU_STA_NUM];
};

struct halbb_ul_fix_sta_info {
	u8 mac_id;
	u8 ru_pos[3];
	u8 tgt_rssi[3];
	u8 rsvd1: 4;
	u8 coding: 1;
	u8 fix_coding: 1;
	u8 fix_rate: 1;
	u8 fix_tgt_rssi: 1;
	struct halbb_ru_rate_info rate_i;
};

struct halbb_ul_fix_sta_info_8ru {
	u8 mac_id;
	u8 ru_position[7];
	u8 rsvd1;
	u8 target_rssi[7];
	u8 rsvd2: 4;
	u8 coding: 1;
	u8 fix_coding_flag: 1;
	u8 fix_rate_flag: 1;
	u8 fix_target_rssi_flag: 1;
	struct halbb_ru_rate_info rate_i;
	u8 rsvd3;
	u8 rsvd4;
};
	
struct halbb_ul_ru_fix_tbl_info {

	struct halbb_rua_tbl_hdr_info tbl_hdr_i;
	u8 ma_type:1;
	u8 doppler:1;
	u8 min_sta_num:3;
	u8 max_sta_num:3;
	u8 fixru_flag: 1;
	u8 tb_t_pe_nom: 2;
	u8 fix_tb_t_pe_nom: 1;
	u8 stbc:1;
	u8 gi_ltf:3;

	struct halbb_ul_fix_sta_info sta_i[HALBB_AX4RU_STA_NUM];

};


struct halbb_ul_ru_fix_tbl_info_8ru {

	struct halbb_rua_tbl_hdr_info tbl_hdr_i;
	u8 min_sta_num:4;
	u8 max_sta_num:4;

	u8 fix_ru_flag: 1;
	u8 tb_t_pe_nominal:2;
	u8 fix_tb_t_pe_nominal_flag:1;
	u8 stbc:1;
	u8 gi_ltf:3;

	u8 rsvd1:6;
	u8 ma_type:1;
	u8 doppler:1;

	u8 rsvd2;

	struct halbb_ul_fix_sta_info_8ru sta_i[HALBB_AX8RU_STA_NUM];

};

struct halbb_rua_tbl {
	struct halbb_dl_ru_gp_tbl_info dl_ru_gp_tbl_i;
	struct halbb_ul_ru_gp_tbl_info ul_ru_gp_tbl_i;
	struct halbb_ru_sta_info ru_sta_i;
	struct halbb_dl_ru_fix_tbl_info dl_ru_fix_tbl_i;
	struct halbb_ul_ru_fix_tbl_info ul_ru_fix_tbl_i;
	struct halbb_tf_ba_tbl_info tf_ba_tbl_i;
};

struct halbb_sw_grp_bitmap {
	u8 macid;

	u8 rsvd1:5;
	u8 cmdend:1; // add for determine whether last user or not
	u8 en_upd_ul_swgrp:1;
	u8 en_upd_dl_swgrp:1;

	u8 rsvd2;
	u8 rsvd3;
	
	u8 dl_sw_grp_bitmap[4];
	u8 ul_sw_grp_bitmap[4];
};

struct halbb_sw_grp_set {
	
	struct halbb_sw_grp_bitmap swgrp_bitmap[8];
	
};

struct dl_macid_cfg {
	u8 macid;
	
	u8 dl_su_rate_l:7;
	u8 dl_su_rate_cfg:1;
	
	u8 dl_su_pwr_l:3;
	u8 dl_su_pwr_cfg:1;
	u8 dl_su_bw:2;
	u8 dl_su_rate_m:2;
	
	u8 rsvd0:5;
	u8 dl_su_pwr_m:3;
	
	u8 rsvd1:6;
	u8 gi_ltf_1x8_support:1;
	u8 gi_ltf_4x8_support:1;
	
	u8 dl_su_doppler_ctrl:2;
	u8 dl_su_gi_ltf:3;
	u8 rsvd2:2;
	u8 dl_su_info_en:1;
	
	u8 rsvd3:4;
	u8 dl_su_dcm:1;
	u8 dl_su_stbc:1;
	u8 dl_su_txbf:1;
	u8 dl_su_coding:1;

	u8 rsvd4;
};

struct halbb_ul_macid_cfg {

	u8 macid;
	
	u8 rsvd0: 7;
	u8 endcmd: 1;

	u8 rsvd1;
	u8 rsvd2;

	u8 ul_su_doppler_ctrl: 2;
	u8 ul_su_gi_ltf: 3;
	u8 ul_su_bw: 2;
	u8 ul_su_info_en: 1;
	
	u8 ul_su_mcs: 4;
	u8 ul_su_ss: 3;
	u8 ul_su_dcm: 1;
	
	u8 ul_su_rssi_m_l: 1;
	u8 ul_su_coding: 1;
	u8 ul_su_stbc: 1;
	u8 rsvd3: 5;

	u8 ul_su_rssi_m_m;

};

struct halbb_ul_macid_set {
	
	struct halbb_ul_macid_cfg ul_macid_cfg[8];
	
};



struct halbb_ba_tbl_info {
	struct halbb_rua_tbl_hdr_info tbl_hdr_i;
	
	struct halbb_tf_ba_tbl_info tf_i;
};

struct csiinfo_cfg{
	u8 macid;
	u8 csi_info_bitmap;

	u8 rsvd0;
	u8 rsvd1;
};

struct halbb_cqi_info {
	u8 macid;

	u8 rsvd0: 1;
	u8 endcmd: 1;
	u8 ul_dl: 1; /*1'b0 means UL, 1'b1 means DL */
	u8 ru_rate_table_row_idx: 4; /* UL or DL*/
	u8 fw_cqi_flag: 1; /* UL or DL*/

	u8 rsvd1;
	u8 rsvd2;

	s8 cqi_diff_table[19]; /* UL or DL*/
	u8 rsvd3;
};

struct halbb_cqi_set{	
	struct halbb_cqi_info cqi_info[8];
};

struct halbb_bb_info_cfg {
	u8 p20_ch_bitmap;

	u8 rsvd0;
	u8 rsvd1;
	u8 rsvd2;
};

struct halbb_pwr_by_rt_tbl{
	u8 pwr_by_rt[64];
};


/*@--------------------------[Prptotype]-------------------------------------*/

#endif /* HALBB_RUA_SUPPORT */

#endif
