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

#ifndef _HALBB_PLCP_TX_H_
#define _HALBB_PLCP_TX_H_

/*  ============================================================
 			   structure
    ============================================================
*/
struct bb_plcp_cr_info {
	u32 b_header_0;
	u32 b_header_0_m;	
	u32 b_header_1;
	u32 b_header_1_m;	
	u32 b_header_2;	
	u32 b_header_2_m;	
	u32 b_header_3;	
	u32 b_header_3_m;	
	u32 b_header_4;	
	u32 b_header_4_m;	
	u32 b_header_5;	
	u32 b_header_5_m;	
	u32 b_psdu_byte;	
	u32 b_psdu_byte_m;
	u32 b_carrier_suppress_tx;	
	u32 b_carrier_suppress_tx_m;	
	u32 b_ppdu_type;		
	u32 b_ppdu_type_m;	
	u32 b_psdu_rate;		
	u32 b_psdu_rate_m;	
	u32 b_service_bit2;	
	u32 b_service_bit2_m;	
	u32 he_sigb_ch1_0;		
	u32 he_sigb_ch1_0_m;	
	u32 he_sigb_ch1_1;		
	u32 he_sigb_ch1_1_m;	
	u32 he_sigb_ch1_10;		
	u32 he_sigb_ch1_10_m;	
	u32 he_sigb_ch1_11;		
	u32 he_sigb_ch1_11_m;	
	u32 he_sigb_ch1_12;		
	u32 he_sigb_ch1_12_m;	
	u32 he_sigb_ch1_13;		
	u32 he_sigb_ch1_13_m;	
	u32 he_sigb_ch1_14;		
	u32 he_sigb_ch1_14_m;	
	u32 he_sigb_ch1_15;		
	u32 he_sigb_ch1_15_m;	
	u32 he_sigb_ch1_2;		
	u32 he_sigb_ch1_2_m;	
	u32 he_sigb_ch1_3;		
	u32 he_sigb_ch1_3_m;	
	u32 he_sigb_ch1_4;		
	u32 he_sigb_ch1_4_m;	
	u32 he_sigb_ch1_5;		
	u32 he_sigb_ch1_5_m;	
	u32 he_sigb_ch1_6;		
	u32 he_sigb_ch1_6_m;	
	u32 he_sigb_ch1_7;		
	u32 he_sigb_ch1_7_m;	
	u32 he_sigb_ch1_8;		
	u32 he_sigb_ch1_8_m;	
	u32 he_sigb_ch1_9;		
	u32 he_sigb_ch1_9_m;	
	u32 he_sigb_ch2_0;		
	u32 he_sigb_ch2_0_m;	
	u32 he_sigb_ch2_1;		
	u32 he_sigb_ch2_1_m;	
	u32 he_sigb_ch2_10;		
	u32 he_sigb_ch2_10_m;	
	u32 he_sigb_ch2_11;		
	u32 he_sigb_ch2_11_m;	
	u32 he_sigb_ch2_12;		
	u32 he_sigb_ch2_12_m;	
	u32 he_sigb_ch2_13;		
	u32 he_sigb_ch2_13_m;	
	u32 he_sigb_ch2_14;		
	u32 he_sigb_ch2_14_m;	
	u32 he_sigb_ch2_15;		
	u32 he_sigb_ch2_15_m;	
	u32 he_sigb_ch2_2;		
	u32 he_sigb_ch2_2_m;	
	u32 he_sigb_ch2_3;		
	u32 he_sigb_ch2_3_m;	
	u32 he_sigb_ch2_4;		
	u32 he_sigb_ch2_4_m;	
	u32 he_sigb_ch2_5;		
	u32 he_sigb_ch2_5_m;	
	u32 he_sigb_ch2_6;		
	u32 he_sigb_ch2_6_m;	
	u32 he_sigb_ch2_7;		
	u32 he_sigb_ch2_7_m;	
	u32 he_sigb_ch2_8;		
	u32 he_sigb_ch2_8_m;	
	u32 he_sigb_ch2_9;		
	u32 he_sigb_ch2_9_m;	
	u32 usr0_delmter;		
	u32 usr0_delmter_m;	
	u32 usr0_eof_padding_len;	
	u32 usr0_eof_padding_len_m;	
	u32 usr0_init_seed;		
	u32 usr0_init_seed_m;		
	u32 usr1_delmter;		
	u32 usr1_delmter_m;	
	u32 usr1_eof_padding_len;	
	u32 usr1_eof_padding_len_m;	
	u32 usr1_init_seed;	
	u32 usr1_init_seed_m;	
	u32 usr2_delmter;		
	u32 usr2_delmter_m;	
	u32 usr2_eof_padding_len;	
	u32 usr2_eof_padding_len_m;	
	u32 usr2_init_seed;	
	u32 usr2_init_seed_m;	
	u32 usr3_delmter;		
	u32 usr3_delmter_m;	
	u32 usr3_eof_padding_len;	
	u32 usr3_eof_padding_len_m;
	u32 usr3_init_seed;	
	u32 usr3_init_seed_m;	
	u32 vht_sigb0;		
	u32 vht_sigb0_m;	
	u32 vht_sigb1;		
	u32 vht_sigb1_m;	
	u32 vht_sigb2;		
	u32 vht_sigb2_m;
	u32 he_sigb_mcs;
	u32 he_sigb_mcs_m;	
	u32 vht_sigb3;	
	u32 vht_sigb3_m;
	u32 n_ltf;	
	u32 n_ltf_m;
	u32 siga1;		
	u32 siga1_m;	
	u32 siga2;		
	u32 siga2_m;	
	u32 lsig;		
	u32 lsig_m;		
	u32 cca_pw_th;	
	u32 cca_pw_th_m;	
	u32 n_sym;		
	u32 n_sym_m;	
	u32 usr0_service;		
	u32 usr0_service_m;	
	u32 usr1_service;		
	u32 usr1_service_m;	
	u32 usr2_service;		
	u32 usr2_service_m;	
	u32 usr3_service;		
	u32 usr3_service_m;	
	u32 usr0_mdpu_len_byte;
	u32 usr0_mdpu_len_byte_m;		
	u32 usr1_mdpu_len_byte;		
	u32 usr1_mdpu_len_byte_m;		
	u32 obw_cts2self_dup_type;		
	u32 obw_cts2self_dup_type_m;		
	u32 usr2_mdpu_len_byte;	
	u32 usr2_mdpu_len_byte_m;	
	u32 usr3_mdpu_len_byte;
	u32 usr3_mdpu_len_byte_m;	
	u32 usr0_csi_buf_id;	
	u32 usr0_csi_buf_id_m;	
	u32 usr1_csi_buf_id;	
	u32 usr1_csi_buf_id_m;	
	u32 rf_gain_idx;	
	u32 rf_gain_idx_m;	
	u32 usr2_csi_buf_id;	
	u32 usr2_csi_buf_id_m;	
	u32 usr3_csi_buf_id;	
	u32 usr3_csi_buf_id_m;	
	u32 usr0_n_mpdu;	
	u32 usr0_n_mpdu_m;		
	u32 usr1_n_mpdu;		
	u32 usr1_n_mpdu_m;		
	u32 usr2_n_mpdu;		
	u32 usr2_n_mpdu_m;		
	u32 usr0_pw_boost_fctr_db;		
	u32 usr0_pw_boost_fctr_db_m;	
	u32 usr3_n_mpdu;		
	u32 usr3_n_mpdu_m;		
	u32 ch20_with_data;		
	u32 ch20_with_data_m;	
	u32 n_usr;		
	u32 n_usr_m;		
	u32 txcmd_txtp;
	u32 txcmd_txtp_m;
	u32 usr0_ru_alloc;	
	u32 usr0_ru_alloc_m;
	u32 usr0_u_id;		
	u32 usr0_u_id_m;
	u32 usr1_ru_alloc;	
	u32 usr1_ru_alloc_m;
	u32 usr1_u_id;		
	u32 usr1_u_id_m;	
	u32 usr2_ru_alloc;	
	u32 usr2_ru_alloc_m;
	u32 usr2_u_id;		
	u32 usr2_u_id_m;	
	u32 usr3_ru_alloc;	
	u32 usr3_ru_alloc_m;
	u32 usr3_u_id;		
	u32 usr3_u_id_m;	
	u32 n_sym_hesigb;		
	u32 n_sym_hesigb_m;		
	u32 usr0_mcs;	
	u32 usr0_mcs_m;	
	u32 usr1_mcs;	
	u32 usr1_mcs_m;	
	u32 usr2_mcs;	
	u32 usr2_mcs_m;	
	u32 usr3_mcs;	
	u32 usr3_mcs_m;	
	u32 usr1_pw_boost_fctr_db;	
	u32 usr1_pw_boost_fctr_db_m;
	u32 usr2_pw_boost_fctr_db;	
	u32 usr2_pw_boost_fctr_db_m;
	u32 usr3_pw_boost_fctr_db;	
	u32 usr3_pw_boost_fctr_db_m;
	u32 ppdu_type;	
	u32 ppdu_type_m;	
	u32 txsc;		
	u32 txsc_m;		
	u32 cfo_comp;	
	u32 cfo_comp_m;	
	u32 pkt_ext_idx;
	u32 pkt_ext_idx_m;		
	u32 usr0_n_sts;		
	u32 usr0_n_sts_m;	
	u32 usr0_n_sts_ru_tot;	
	u32 usr0_n_sts_ru_tot_m;
	u32 usr0_strt_sts;		
	u32 usr0_strt_sts_m;	
	u32 usr1_n_sts;		
	u32 usr1_n_sts_m;	
	u32 usr1_n_sts_ru_tot;
	u32 usr1_n_sts_ru_tot_m;
	u32 usr1_strt_sts;	
	u32 usr1_strt_sts_m;
	u32 usr2_n_sts;	
	u32 usr2_n_sts_m;	
	u32 usr2_n_sts_ru_tot;	
	u32 usr2_n_sts_ru_tot_m;	
	u32 usr2_strt_sts;	
	u32 usr2_strt_sts_m;	
	u32 usr3_n_sts;		
	u32 usr3_n_sts_m;	
	u32 usr3_n_sts_ru_tot;
	u32 usr3_n_sts_ru_tot_m;
	u32 usr3_strt_sts;		
	u32 usr3_strt_sts_m;	
	u32 source_gen_mode_idx;	
	u32 source_gen_mode_idx_m;	
	u32 gi_type;		
	u32 gi_type_m;		
	u32 ltf_type;		
	u32 ltf_type_m;	
	u32 dbw_idx;		
	u32 dbw_idx_m;	
	u32 pre_fec_fctr;	
	u32 pre_fec_fctr_m;	
	u32 beam_change_en;	
	u32 beam_change_en_m;
	u32 doppler_en;		
	u32 doppler_en_m;	
	u32 fb_mumimo_en;	
	u32 fb_mumimo_en_m;	
	u32 feedback_status;
	u32 feedback_status_m;		
	u32 he_sigb_dcm_en;	
	u32 he_sigb_dcm_en_m;	
	u32 midamble_mode;	
	u32 midamble_mode_m;
	u32 mumimo_ltf_mode_en;		
	u32 mumimo_ltf_mode_en_m;	
	u32 ndp;		
	u32 ndp_m;		
	u32 stbc_en;	
	u32 stbc_en_m;	
	u32 ant_sel_a;	
	u32 ant_sel_a_m;	
	u32 ant_sel_b;	
	u32 ant_sel_b_m;	
	u32 ant_sel_c;	
	u32 ant_sel_c_m;	
	u32 ant_sel_d;	
	u32 ant_sel_d_m;	
	u32 cca_pw_th_en;
	u32 cca_pw_th_en_m;	
	u32 rf_fixed_gain_en;
	u32 rf_fixed_gain_en_m;
	u32 ul_cqi_rpt_tri;		
	u32 ul_cqi_rpt_tri_m;	
	u32 ldpc_extr;		
	u32 ldpc_extr_m;	
	u32 usr0_dcm_en;		
	u32 usr0_dcm_en_m;		
	u32 usr0_fec_type;		
	u32 usr0_fec_type_m;		
	u32 usr0_txbf_en;		
	u32 usr0_txbf_en_m;		
	u32 usr1_dcm_en;	
	u32 usr1_dcm_en_m;		
	u32 usr1_fec_type;		
	u32 usr1_fec_type_m;		
	u32 usr1_txbf_en;		
	u32 usr1_txbf_en_m;		
	u32 usr2_dcm_en;		
	u32 usr2_dcm_en_m;		
	u32 usr2_fec_type;		
	u32 usr2_fec_type_m;		
	u32 usr2_txbf_en;		
	u32 usr2_txbf_en_m;		
	u32 usr3_dcm_en;		
	u32 usr3_dcm_en_m;		
	u32 usr3_fec_type;		
	u32 usr3_fec_type_m;	
	u32 usr3_txbf_en;		
	u32 usr3_txbf_en_m;
	u32 usr0_precoding_mode_idx;
	u32 usr0_precoding_mode_idx_m;
	u32 usr1_precoding_mode_idx;
	u32 usr1_precoding_mode_idx_m;
	u32 usr2_precoding_mode_idx;
	u32 usr2_precoding_mode_idx_m;
	u32 usr3_precoding_mode_idx;
	u32 usr3_precoding_mode_idx_m;
	u32 b_rate_idx;
	u32 b_rate_idx_m;
	u32 b_locked_clk_en;
	u32 b_locked_clk_en_m;
	u32 max_mcs;
	u32 max_mcs_m;
	u32 tx_tri_idx;
	u32 tx_tri_idx_m;
	u32 tx_tri_pw_ofst;
	u32 tx_tri_pw_ofst_m;
};

struct bb_plcp_info {
	struct bb_plcp_cr_info bb_plcp_cr_i;
};


/*  ============================================================
 		     Function Prototype
    ============================================================
*/

struct bb_info;

void halbb_cr_cfg_plcp_init(struct bb_info *bb);

void halbb_plcp_init(struct bb_info *bb);

#endif
