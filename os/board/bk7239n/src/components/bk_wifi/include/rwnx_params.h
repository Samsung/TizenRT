/**
 ******************************************************************************
 *
 * @file rwnx_mod_params.h
 *
 * @brief Declaration of module parameters
 *
 * Copyright (C) RivieraWaves 2012-2019
 *
 ******************************************************************************
 */

#ifndef _RWNX_PARAM_H_
#define _RWNX_PARAM_H_
#include "rwnx_defs.h"

struct rwnx_mod_params {
	bool erp_on;    //support 802.11g or not
	bool ht_on;
	bool vht_on;
	bool he_on;
	int mcs_map;	// vht mcs map
	int he_mcs_map;	// he mcs map
	bool he_ul_on;	// OFDMA UL
	bool ldpc_on;
	bool stbc_on;
	bool gf_rx_on;
	int phy_cfg;	// ant selection
	int uapsd_timeout;
	bool ap_uapsd_on;
	bool sgi;
	bool sgi80;
	bool use_2040;
	bool use_80;
	bool custregd;
	bool custchan;
	int nss;
	int amsdu_rx_max;
	bool bfmee;
	bool bfmer;
	bool mesh;
	bool murx;
	bool mutx;
	bool mutx_on;
	unsigned int roc_dur_max;
	int listen_itv;
	bool listen_bcmc;
	int lp_clk_ppm;
	bool ps_on;
	int tx_lft;
	int amsdu_maxnb;
	int uapsd_queues;
	bool tdls;
	bool uf;
	char *ftl;
	bool dpsm;
	bool ant_div;
	bool rx_ampdu_on;
	int  rx_agg_num;
	bool tx_ampdu_on;
	uint32_t tx_agg_num;
	bool amsdu_require_spp;
};

extern struct rwnx_mod_params rwnx_mod_params;

struct rwnx_hw;
struct wiphy;
int rwnx_handle_dynparams(void);
void rwnx_custregd(struct rwnx_hw *rwnx_hw, struct wiphy *wiphy);
void rwnx_enable_wapi(struct rwnx_hw *rwnx_hw);
void rwnx_enable_mfp(struct rwnx_hw *rwnx_hw);
bool rwnx_get_rx_ampdu_capa_on(void);
uint8_t rwnx_get_rx_ampdu_num(void);
__INLINE void rwnx_update_erp_capa(uint32_t erp_en)
{
	rwnx_mod_params.erp_on = erp_en;
	RWNX_LOGI("update ERP capa:%d\n", erp_en);
}

__INLINE void rwnx_update_ht_capa(uint32_t ht_en)
{
	rwnx_mod_params.ht_on = ht_en;
	RWNX_LOGI("update HT capa:%d\n", ht_en);
}
__INLINE void rwnx_update_vht_capa(uint32_t vht_en)
{
	rwnx_mod_params.vht_on = vht_en;
	RWNX_LOGI("update VHT capa:%d\n", vht_en);
}
__INLINE void rwnx_update_he_capa(uint32_t he_en)
{
	rwnx_mod_params.he_on = he_en;
	RWNX_LOGI("update HE capa:%d\n", he_en);
}
__INLINE void rwnx_update_tx_ampdu_capa(uint32_t tx_ampdu_en)
{
	rwnx_mod_params.tx_ampdu_on = tx_ampdu_en;
	RWNX_LOGI("update TX_AMPDU capa:%d\n", tx_ampdu_en);
}
__INLINE void rwnx_update_rx_ampdu_capa(uint32_t rx_ampdu_en)
{
	rwnx_mod_params.rx_ampdu_on =rx_ampdu_en;
	RWNX_LOGI("update RX_AMPDU capa:%d\n", rx_ampdu_en);
}
__INLINE void rwnx_update_tx_ampdu_num_capa(uint32_t tx_ampdu_num)
{
	rwnx_mod_params.tx_agg_num = tx_ampdu_num;
	RWNX_LOGI("update TX_AMPDU_NUM capa:%d\n", tx_ampdu_num);
}
__INLINE void rwnx_update_rx_ampdu_num_capa(uint32_t rx_ampdu_num)
{
	rwnx_mod_params.rx_agg_num = rx_ampdu_num;
	RWNX_LOGI("update RX_AMPDU_NUM capa:%d\n", rx_ampdu_num);
}
__INLINE void rwnx_update_vht_mcs(uint32_t vht_mcs)
{
	rwnx_mod_params.mcs_map = vht_mcs;
	RWNX_LOGI("update VHT mcs:%d\n", vht_mcs);
}
__INLINE void rwnx_update_he_mcs(uint32_t he_mcs)
{
	rwnx_mod_params.he_mcs_map = he_mcs;
	RWNX_LOGI("update HE mcs:%d\n", he_mcs);
}
__INLINE void rwnx_update_b40(uint32_t b40_en)
{
	rwnx_mod_params.use_2040 = b40_en;
	RWNX_LOGI("update B40:%d\n", b40_en);
}
__INLINE void rwnx_update_stbc(uint32_t stbc_en)
{
	rwnx_mod_params.stbc_on = stbc_en;
	RWNX_LOGI("update STBC:%d\n", stbc_en);
}
__INLINE void rwnx_update_sgi(uint32_t sgi_en)
{
	rwnx_mod_params.sgi = sgi_en;
	RWNX_LOGI("update SGI:%d\n", sgi_en);
}

__INLINE void rwnx_update_ldpc(uint32_t ldpc_en)
{
	rwnx_mod_params.ldpc_on = !!ldpc_en;
	RWNX_LOGI("update LDPC:%d\n", !!ldpc_en);
}

__INLINE void rwnx_update_beamformee(uint32_t bfmee_en)
{
	rwnx_mod_params.bfmee = bfmee_en;
	RWNX_LOGI("update BEAMFORMEE:%d\n", bfmee_en);
}

__INLINE void rwnx_update_11b_only(uint32_t b_only_en)
{
	if (b_only_en)
	{
		rwnx_mod_params.erp_on = false;
		rwnx_mod_params.ht_on = false;
		rwnx_mod_params.vht_on = false;
		rwnx_mod_params.he_on = false;
	}
	RWNX_LOGI("update 11b only:%d\n", b_only_en);
}

#endif /* _RWNX_MOD_PARAM_H_ */

