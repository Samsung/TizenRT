#include "stdio.h"
#include "string.h"
#include "rwnx_version.h"
#include "bk_list.h"
#include "rw_ieee80211.h"
#include "rwnx_config.h"
#include "rwnx_defs.h"
#include "rwnx_params.h"
#include "bk_wifi.h"
#include "components/ate.h"
#include "rwnx_defs.h"
#include <os/str.h>

struct mac_htcapability rwnx_htcap = {
	.ht_capa_info   = 0x0f32,
	.a_mpdu_param   = 0x1c,
	.mcs_rate       = {0xff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x1, 0, 0, 0, 0},
	.ht_extended_capa = 0,
	.tx_beamforming_capa = 0x64000000,
	.asel_capa = 0x1,
};

#define PARAM(name, default)  .name = default

struct rwnx_mod_params rwnx_mod_params = {
	/* common parameters */
	PARAM(erp_on, true),
	PARAM(ht_on, true),
#if CONFIG_WIFI5
	PARAM(vht_on, true),
#else
	PARAM(vht_on, false),
#endif
#if CONFIG_WIFI6
	PARAM(he_on, true),
#else
	PARAM(he_on, false),
#endif
	PARAM(mcs_map, IEEE80211_VHT_MCS_SUPPORT_0_9),
	PARAM(he_mcs_map, IEEE80211_HE_MCS_SUPPORT_0_9),
	PARAM(he_ul_on, false),
	/* reopen ldpc on for solve rx throughput */
#if CONFIG_WIFI6 && (!CONFIG_SOC_BK7236N) && (!CONFIG_SOC_BK7239XX)
	PARAM(ldpc_on, true),
#else
	PARAM(ldpc_on, false),
#endif
	PARAM(stbc_on, true),
#if CONFIG_WIFI6
	PARAM(gf_rx_on, true),
#else
	PARAM(gf_rx_on, false),
#endif
	PARAM(phy_cfg, 2),
	PARAM(uapsd_timeout, 300),
	PARAM(ap_uapsd_on, true),
	PARAM(sgi, true),
	//PARAM(sgi80, true),
#if CONFIG_WIFI6
	PARAM(use_2040, 0),
#else
	PARAM(use_2040, 0),
#endif
	PARAM(nss, 1),
	PARAM(amsdu_rx_max, 0), // max amsdu size 3839
	PARAM(bfmee, false),
	PARAM(bfmer, false),
	//PARAM(mesh, true),
	PARAM(murx, true),
	//PARAM(mutx, true),
	//PARAM(mutx_on, true),
	//PARAM(use_80, true),
	PARAM(custregd, false),
	PARAM(custchan, false),
	PARAM(roc_dur_max, 500),
	PARAM(listen_itv, 0),  // hm set to 2
	PARAM(listen_bcmc, true),
	PARAM(lp_clk_ppm, 1000),   // original 20
	PARAM(ps_on, false),
	PARAM(tx_lft, RWNX_TX_LIFETIME_MS), // hm set to 0
	PARAM(amsdu_maxnb, NX_TX_PAYLOAD_MAX),
	// By default, only enable UAPSD for Voice queue (see IEEE80211_DEFAULT_UAPSD_QUEUE comment)
	PARAM(uapsd_queues, IEEE80211_WMM_IE_STA_QOSINFO_AC_VO), // hm set to 0
	//PARAM(tdls, true),
	PARAM(uf, false),  // hm set true
	PARAM(ftl, ""),
	PARAM(dpsm, false), // original true

	/* FULLMAC only parameters */
	PARAM(ant_div, false), // original true

	//.key = {0xabc47fd0, 0x57498892, 0x11320490, 0x10815562},

	PARAM(rx_ampdu_on, true),
	PARAM(rx_agg_num, 0),
	PARAM(tx_ampdu_on, true),
	PARAM(tx_agg_num, 0),
	PARAM(amsdu_require_spp, false)
};

static const int mcs_map_to_rate[4][3] = {
	[PHY_CHNL_BW_20][IEEE80211_VHT_MCS_SUPPORT_0_7] = 65,
	[PHY_CHNL_BW_20][IEEE80211_VHT_MCS_SUPPORT_0_8] = 78,
	[PHY_CHNL_BW_20][IEEE80211_VHT_MCS_SUPPORT_0_9] = 78,
	[PHY_CHNL_BW_40][IEEE80211_VHT_MCS_SUPPORT_0_7] = 135,
	[PHY_CHNL_BW_40][IEEE80211_VHT_MCS_SUPPORT_0_8] = 162,
	[PHY_CHNL_BW_40][IEEE80211_VHT_MCS_SUPPORT_0_9] = 180,
	[PHY_CHNL_BW_80][IEEE80211_VHT_MCS_SUPPORT_0_7] = 292,
	[PHY_CHNL_BW_80][IEEE80211_VHT_MCS_SUPPORT_0_8] = 351,
	[PHY_CHNL_BW_80][IEEE80211_VHT_MCS_SUPPORT_0_9] = 390,
	[PHY_CHNL_BW_160][IEEE80211_VHT_MCS_SUPPORT_0_7] = 585,
	[PHY_CHNL_BW_160][IEEE80211_VHT_MCS_SUPPORT_0_8] = 702,
	[PHY_CHNL_BW_160][IEEE80211_VHT_MCS_SUPPORT_0_9] = 780,
};

#define MAX_VHT_RATE(map, nss, bw) (mcs_map_to_rate[bw][map] * (nss))

/**
 * Do some sanity check
 *
 */
static int rwnx_check_fw_hw_feature(struct rwnx_hw *rwnx_hw,
									struct wiphy *wiphy)
{
	uint64_t sys_feat = rwnx_hw->version_cfm.features;
	uint32_t mac_feat = rwnx_hw->version_cfm.version_machw_1;
	uint32_t phy_feat = rwnx_hw->version_cfm.version_phy_1;
	uint32_t phy_vers = rwnx_hw->version_cfm.version_phy_2;
	int bw, res = 0;
	int amsdu_rx;
	uint32_t rf;

	rwnx_hw->mod_params = &rwnx_mod_params;

	if (!rwnx_hw->mod_params->custregd)
		rwnx_hw->mod_params->custchan = false;

	if (rwnx_hw->mod_params->custchan) {
		rwnx_hw->mod_params->mesh = false;
		rwnx_hw->mod_params->tdls = false;
	}

	if (!(sys_feat & BIT(MM_FEAT_UMAC_BIT))) {
		RWNX_LOGI("Loading softmac firmware with fullmac driver\n");
		res = -1;
	}

	if (!(sys_feat & BIT(MM_FEAT_ANT_DIV_BIT)))
		rwnx_hw->mod_params->ant_div = false;

	if (!(sys_feat & BIT(MM_FEAT_VHT_BIT)))
		rwnx_hw->mod_params->vht_on = false;

	// Check if HE is supported
	if (!(sys_feat & BIT(MM_FEAT_HE_BIT))) {
		rwnx_hw->mod_params->he_on = false;
		rwnx_hw->mod_params->he_ul_on = false;
	}

	if (!(sys_feat & BIT(MM_FEAT_PS_BIT)))
		rwnx_hw->mod_params->ps_on = false;

	/* AMSDU (non)support implies different shared structure definition
	   so insure that fw and drv have consistent compilation option */
	if (sys_feat & BIT(MM_FEAT_AMSDU_BIT)) {
		if (rwnx_hw->mod_params->amsdu_maxnb > NX_TX_PAYLOAD_MAX)
			rwnx_hw->mod_params->amsdu_maxnb = NX_TX_PAYLOAD_MAX;
	}

	if (!(sys_feat & BIT(MM_FEAT_UAPSD_BIT)))
		rwnx_hw->mod_params->uapsd_timeout = 0;

	if (!(sys_feat & BIT(MM_FEAT_BFMEE_BIT)))
		rwnx_hw->mod_params->bfmee = false;

	if ((sys_feat & BIT(MM_FEAT_BFMER_BIT))) {
		// Check PHY and MAC HW BFMER support and update parameter accordingly
		if (!(phy_feat & MDM_BFMER_BIT) || !(mac_feat & NXMAC_BFMER_BIT)) {
			rwnx_hw->mod_params->bfmer = false;
			// Disable the feature in the bitfield so that it won't be displayed
			sys_feat &= ~BIT(MM_FEAT_BFMER_BIT);
		}
	} else
		rwnx_hw->mod_params->bfmer = false;

	if (!(sys_feat & BIT(MM_FEAT_MESH_BIT)))
		rwnx_hw->mod_params->mesh = false;

	if (!(sys_feat & BIT(MM_FEAT_TDLS_BIT)))
		rwnx_hw->mod_params->tdls = false;

	if (!(sys_feat & BIT(MM_FEAT_UF_BIT)))
		rwnx_hw->mod_params->uf = false;

	// Check supported AMSDU RX size
	amsdu_rx = (sys_feat >> MM_AMSDU_MAX_SIZE_BIT0) & 0x03;
	if (amsdu_rx < rwnx_hw->mod_params->amsdu_rx_max)
		rwnx_hw->mod_params->amsdu_rx_max = amsdu_rx;

	// Check supported BW
	bw = (phy_feat & MDM_CHBW_MASK) >> MDM_CHBW_LSB;
	// Check if 80MHz BW is supported
	if (bw < 2)
		rwnx_hw->mod_params->use_80 = false;
	// Check if 40MHz BW is supported
	if (bw < 1)
		rwnx_hw->mod_params->use_2040 = false;

	// 80MHz BW shall be disabled if 40MHz is not enabled
	if (!rwnx_hw->mod_params->use_2040)
		rwnx_hw->mod_params->use_80 = false;

	// Check if HT is supposed to be supported. If not, disable VHT/HE too
	if (!rwnx_hw->mod_params->ht_on) {
		rwnx_hw->mod_params->vht_on = false;
		rwnx_hw->mod_params->he_on = false;
		rwnx_hw->mod_params->he_ul_on = false;
		rwnx_hw->mod_params->use_80 = false;
		rwnx_hw->mod_params->use_2040 = false;
	}

	// LDPC is mandatory for HE40 and above, so if LDPC is not supported, then disable
	// HE to use HT/VHT only
	if (rwnx_hw->mod_params->he_on && !rwnx_hw->mod_params->ldpc_on && !ate_is_enabled()) {
		rwnx_hw->mod_params->use_80 = false;
		rwnx_hw->mod_params->use_2040 = false;
	}

	// HT greenfield is not supported in modem >= 3.0
	if (__MDM_MAJOR_VERSION(phy_vers) > 0)
		rwnx_hw->mod_params->gf_rx_on = false;

	if (!(sys_feat & BIT(MM_FEAT_MU_MIMO_RX_BIT)) ||
		!rwnx_hw->mod_params->bfmee)
		rwnx_hw->mod_params->murx = false;

	if ((sys_feat & BIT(MM_FEAT_MU_MIMO_TX_BIT))) {
		if (!rwnx_hw->mod_params->bfmer)
			rwnx_hw->mod_params->mutx = false;
		// Check PHY and MAC HW MU-MIMO TX support and update parameter accordingly
		else if (!(phy_feat & MDM_MUMIMOTX_BIT) || !(mac_feat & NXMAC_MU_MIMO_TX_BIT)) {
			rwnx_hw->mod_params->mutx = false;
			// Disable the feature in the bitfield so that it won't be displayed
			sys_feat &= ~BIT(MM_FEAT_MU_MIMO_TX_BIT);
		}
	} else
		rwnx_hw->mod_params->mutx = false;

	rf = __MDM_PHYCFG_FROM_VERS(phy_feat);
	switch (rf) {
	case MDM_PHY_CONFIG_TRIDENT:
	case MDM_PHY_CONFIG_ELMA:
		rwnx_hw->mod_params->nss = 1;
		if ((rwnx_hw->mod_params->phy_cfg < 0) || (rwnx_hw->mod_params->phy_cfg > 2))
			rwnx_hw->mod_params->phy_cfg = 2;
		break;
	case MDM_PHY_CONFIG_KARST: {
		int nss_supp = (phy_feat & MDM_NSS_MASK) >> MDM_NSS_LSB;
		if (rwnx_hw->mod_params->nss > nss_supp)
			rwnx_hw->mod_params->nss = nss_supp;

		if ((rwnx_hw->mod_params->phy_cfg < 0) || (rwnx_hw->mod_params->phy_cfg > 1))
			rwnx_hw->mod_params->phy_cfg = 0;
	}
	break;
	default:
		//WARN_ON(1);
		break;
	}

	if ((rf !=  MDM_PHY_CONFIG_KARST) ||
		(phy_feat & (MDM_LDPCDEC_BIT | MDM_LDPCENC_BIT)) !=
		(MDM_LDPCDEC_BIT | MDM_LDPCENC_BIT))
		rwnx_hw->mod_params->ldpc_on = false;

	if (rwnx_hw->mod_params->nss < 1 || rwnx_hw->mod_params->nss > 2)
		rwnx_hw->mod_params->nss = 1;

	if (rwnx_hw->mod_params->mcs_map < 0 || rwnx_hw->mod_params->mcs_map > 2)
		rwnx_hw->mod_params->mcs_map = 0;

#define PRINT_RWNX_PHY_FEAT(feat)                                   \
	(phy_feat & MDM_##feat##_BIT ? "["#feat"]" : "")

	RWNX_LOGD("PHY features: [NSS=%d][CHBW=%d]%s%s%s%s%s%s%s%s\n",
			  (phy_feat & MDM_NSS_MASK) >> MDM_NSS_LSB,
			  20 * (1 << ((phy_feat & MDM_CHBW_MASK) >> MDM_CHBW_LSB)),
			  (phy_feat & (MDM_LDPCDEC_BIT | MDM_LDPCENC_BIT)) ==
			  (MDM_LDPCDEC_BIT | MDM_LDPCENC_BIT) ? "[LDPC]" : "",
			  PRINT_RWNX_PHY_FEAT(VHT),
			  PRINT_RWNX_PHY_FEAT(HE),
			  PRINT_RWNX_PHY_FEAT(BFMER),
			  PRINT_RWNX_PHY_FEAT(BFMEE),
			  PRINT_RWNX_PHY_FEAT(MUMIMOTX),
			  PRINT_RWNX_PHY_FEAT(MUMIMORX)
			 );

#define PRINT_RWNX_FEAT(feat)                                   \
	(sys_feat & (1ULL << MM_FEAT_##feat##_BIT) ? "["#feat"]" : "")

	RWNX_LOGI_RAW("FW features:%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
			  PRINT_RWNX_FEAT(BCN),
#if NX_VERSION >= NX_VERSION_PACK(6, 22, 0, 0)
			  "", "", "", "", /* enabled by default */
#else
			  PRINT_RWNX_FEAT(AUTOBCN),
			  PRINT_RWNX_FEAT(HWSCAN),
			  PRINT_RWNX_FEAT(CMON),
			  PRINT_RWNX_FEAT(MROLE),
#endif
			  PRINT_RWNX_FEAT(RADAR),
			  PRINT_RWNX_FEAT(PS),
			  PRINT_RWNX_FEAT(UAPSD),
#if NX_VERSION >= NX_VERSION_PACK(6, 22, 0, 0)
			  "",
#else
			  PRINT_RWNX_FEAT(DPSM),
#endif
			  PRINT_RWNX_FEAT(AMPDU),
			  PRINT_RWNX_FEAT(AMSDU),
#if NX_VERSION >= NX_VERSION_PACK(6, 22, 0, 0)
			  "", "",
#else
			  PRINT_RWNX_FEAT(CHNL_CTXT),
			  PRINT_RWNX_FEAT(REORD),
#endif
			  PRINT_RWNX_FEAT(P2P),
			  PRINT_RWNX_FEAT(P2P_GO)
			  );
	RWNX_LOGI_RAW("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s\n",
			  PRINT_RWNX_FEAT(UMAC),
			  PRINT_RWNX_FEAT(VHT),
			  PRINT_RWNX_FEAT(HE),
			  PRINT_RWNX_FEAT(BFMEE),
			  PRINT_RWNX_FEAT(BFMER),
			  PRINT_RWNX_FEAT(WAPI),
			  PRINT_RWNX_FEAT(MFP),
			  PRINT_RWNX_FEAT(MU_MIMO_RX),
			  PRINT_RWNX_FEAT(MU_MIMO_TX),
			  PRINT_RWNX_FEAT(MESH),
			  PRINT_RWNX_FEAT(TDLS),
			  PRINT_RWNX_FEAT(ANT_DIV),
			  PRINT_RWNX_FEAT(UF),
			  PRINT_RWNX_FEAT(TWT),
			  PRINT_RWNX_FEAT(PM_DSSS_ONLY),
			  PRINT_RWNX_FEAT(MORE_TBD));
#undef PRINT_RWNX_FEAT
	return res;
}

static void rwnx_set_ppe_threshold(struct rwnx_hw *rwnx_hw,
                                   struct ieee80211_sta_he_cap *he_cap)
{
	const uint8_t PPE_THRES_INFO_OFT = 7;
	const uint8_t PPE_THRES_INFO_BIT_LEN = 6;
	struct ppe_thres_info_tag {
		uint8_t ppet16 : 3;
		uint8_t ppet8 : 3;
	} __packed;

	struct ppe_thres_field_tag {
		uint8_t nsts : 3;
		uint8_t ru_idx_bmp : 4;
	};
	int nss = rwnx_hw->mod_params->nss;
	struct ppe_thres_field_tag *ppe_thres_field = (struct ppe_thres_field_tag *)he_cap->ppe_thres;
	struct ppe_thres_info_tag ppe_thres_info = {
		.ppet16	= 0,        //BSPK
		.ppet8  = 7     //None
	};
	uint8_t *ppe_thres_info_ptr = (uint8_t *)&ppe_thres_info;
	uint8_t *ppe_thres_ptr = he_cap->ppe_thres;
	uint8_t i, j, cnt, offset;

	if (0) {
#if (!CONFIG_SOC_BK7236N) && (!CONFIG_SOC_BK7239XX)
	} else if (rwnx_hw->mod_params->use_80) {
		ppe_thres_field->ru_idx_bmp = 7;
		cnt = 3;
	} else if (rwnx_hw->mod_params->use_2040) {
		ppe_thres_field->ru_idx_bmp = 3;
		cnt = 2;
#endif
	} else {
		ppe_thres_field->ru_idx_bmp = 1;
		cnt = 1;
	}
	ppe_thres_field->nsts = nss - 1;
	for (i = 0; i < nss ; i++) {
		for (j = 0; j < cnt; j++) {
			offset = (i * cnt + j) * PPE_THRES_INFO_BIT_LEN + PPE_THRES_INFO_OFT;
			ppe_thres_ptr = &he_cap->ppe_thres[offset / 8];
			*ppe_thres_ptr |= *ppe_thres_info_ptr << (offset % 8);
			*(++ ppe_thres_ptr) |= *ppe_thres_info_ptr >> (8 - (offset % 8));
		}
	}
}

static void rwnx_set_erp_capa(struct rwnx_hw *rwnx_hw, struct wiphy *wiphy)
{
	struct ieee80211_supported_band *band_5GHz = wiphy->bands[IEEE80211_BAND_5GHZ];
	struct ieee80211_supported_band *band_2GHz = wiphy->bands[IEEE80211_BAND_2GHZ];

	if (!rwnx_hw->mod_params->erp_on) {
		band_2GHz->erp_cap.erp_supported = false;
		band_5GHz->erp_cap.erp_supported = false;
	}
	else {
		band_2GHz->erp_cap.erp_supported = true;
		band_5GHz->erp_cap.erp_supported = true;
	}
}

/*
 * HT Capabilities Info: 0x01ad
 * 	.... .... .... ...1 = HT LDPC coding capability: Transmitter supports receiving LDPC coded packets
 * 	.... .... .... ..0. = HT Support channel width: Transmitter only supports 20MHz operation
 * 	.... .... .... 11.. = HT SM Power Save: SM Power Save disabled (0x3)
 * 	.... .... ...0 .... = HT Green Field: Transmitter is not able to receive PPDUs with Green Field (GF) preamble
 * 	.... .... ..1. .... = HT Short GI for 20MHz: Supported
 * 	.... .... .0.. .... = HT Short GI for 40MHz: Not supported
 * 	.... .... 1... .... = HT Tx STBC: Supported
 * 	.... ..01 .... .... = HT Rx STBC: Rx support of one spatial stream (0x1)
 * 	.... .0.. .... .... = HT Delayed Block ACK: Transmitter does not support HT-Delayed BlockAck
 * 	.... 0... .... .... = HT Max A-MSDU length: 3839 bytes
 * 	...0 .... .... .... = HT DSSS/CCK mode in 40MHz: Won't/Can't use of DSSS/CCK in 40 MHz
 * 	..0. .... .... .... = HT PSMP Support: Won't/Can't support PSMP operation
 * 	.0.. .... .... .... = HT Forty MHz Intolerant: Use of 40 MHz transmissions unrestricted/allowed
 * 	0... .... .... .... = HT L-SIG TXOP Protection support: Not supported
 */
static void rwnx_set_ht_capa(struct rwnx_hw *rwnx_hw, struct wiphy *wiphy)
{
	struct ieee80211_supported_band *band_5GHz = wiphy->bands[IEEE80211_BAND_5GHZ];
	struct ieee80211_supported_band *band_2GHz = wiphy->bands[IEEE80211_BAND_2GHZ];
	int i;
	int nss = rwnx_hw->mod_params->nss;

	if (!rwnx_hw->mod_params->ht_on) {
		band_2GHz->ht_cap.ht_supported = false;
		band_5GHz->ht_cap.ht_supported = false;
		return;
	}

	if (rwnx_hw->mod_params->stbc_on)
		band_2GHz->ht_cap.cap |= 1 << IEEE80211_HT_CAP_RX_STBC_SHIFT;
	if (rwnx_hw->mod_params->ldpc_on)
		band_2GHz->ht_cap.cap |= IEEE80211_HT_CAP_LDPC_CODING;
	if (rwnx_hw->mod_params->use_2040) {
		band_2GHz->ht_cap.mcs.rx_mask[4] = 0x1; /* MCS32 */
		band_2GHz->ht_cap.cap |= IEEE80211_HT_CAP_SUP_WIDTH_20_40;
		band_2GHz->ht_cap.mcs.rx_highest = cpu_to_le16(135 * nss);
	} else {
		band_2GHz->ht_cap.mcs.rx_highest = cpu_to_le16(65 * nss);
	}
	if (nss > 1)
		band_2GHz->ht_cap.cap |= IEEE80211_HT_CAP_TX_STBC;

	// Update the AMSDU max RX size
	if (rwnx_hw->mod_params->amsdu_rx_max)
		band_2GHz->ht_cap.cap |= IEEE80211_HT_CAP_MAX_AMSDU;

	if (rwnx_hw->mod_params->sgi) {
		band_2GHz->ht_cap.cap |= IEEE80211_HT_CAP_SGI_20;
		if (rwnx_hw->mod_params->use_2040) {
			band_2GHz->ht_cap.cap |= IEEE80211_HT_CAP_SGI_40;
			band_2GHz->ht_cap.mcs.rx_highest = cpu_to_le16(150 * nss);
		} else
			band_2GHz->ht_cap.mcs.rx_highest = cpu_to_le16(72 * nss);
	}
	if (rwnx_hw->mod_params->gf_rx_on)
		band_2GHz->ht_cap.cap |= IEEE80211_HT_CAP_GRN_FLD;

	for (i = 0; i < nss; i++)
		band_2GHz->ht_cap.mcs.rx_mask[i] = 0xFF;

	band_5GHz->ht_cap = band_2GHz->ht_cap;
}

/*
 * VHT Capabilities Info: 0x00000000
 *     .... .... .... .... .... .... .... ..00 = Maximum MPDU Length: 3 895 (0x0)
 *     .... .... .... .... .... .... .... 00.. = Supported Channel Width Set: Neither 160MHz nor 80+80 supported (0x0)
 *     .... .... .... .... .... .... ...0 .... = Rx LDPC: Not supported
 *     .... .... .... .... .... .... ..0. .... = Short GI for 80MHz/TVHT_MODE_4C: Not supported
 *     .... .... .... .... .... .... .0.. .... = Short GI for 160MHz and 80+80MHz: Not supported
 *     .... .... .... .... .... .... 0... .... = Tx STBC: Not supported
 *     .... .... .... .... .... .000 .... .... = Rx STBC: None (0x0)
 *     .... .... .... .... .... 0... .... .... = SU Beamformer Capable: Not supported
 *     .... .... .... .... ...0 .... .... .... = SU Beamformee Capable: Not supported
 *     .... .... .... .... 000. .... .... .... = Beamformee STS Capability: 1 (0x0)
 *     .... .... .... .000 .... .... .... .... = Number of Sounding Dimensions: 1 (0x0)
 *     .... .... .... 0... .... .... .... .... = MU Beamformer Capable: Not supported
 *     .... .... ...0 .... .... .... .... .... = MU Beamformee Capable: Not supported
 *     .... .... ..0. .... .... .... .... .... = TXOP PS: Not supported
 *     .... .... .0.. .... .... .... .... .... = +HTC-VHT Capable: Not supported
 *     .... ..00 0... .... .... .... .... .... = Max A-MPDU Length Exponent: 8 191 (0x0)
 *     .... 00.. .... .... .... .... .... .... = VHT Link Adaptation: No Feedback (0x0)
 *     ...0 .... .... .... .... .... .... .... = Rx Antenna Pattern Consistency: Not supported
 *     ..0. .... .... .... .... .... .... .... = Tx Antenna Pattern Consistency: Not supported
 *     00.. .... .... .... .... .... .... .... = Extended NSS BW Support: 0x0
 */
static void rwnx_set_vht_capa(struct rwnx_hw *rwnx_hw, struct wiphy *wiphy)
{
	struct ieee80211_supported_band *band_5GHz = wiphy->bands[IEEE80211_BAND_5GHZ];
	int i;
	int nss = rwnx_hw->mod_params->nss;
	int mcs_map;
	int mcs_map_max;
	int bw_max;

	if (!rwnx_hw->mod_params->vht_on)
		return;

	band_5GHz->vht_cap.vht_supported = true;
	if (rwnx_hw->mod_params->sgi80)
		band_5GHz->vht_cap.cap |= IEEE80211_VHT_CAP_SHORT_GI_80;
	if (rwnx_hw->mod_params->stbc_on)
		band_5GHz->vht_cap.cap |= IEEE80211_VHT_CAP_RXSTBC_1;
	if (rwnx_hw->mod_params->ldpc_on)
		band_5GHz->vht_cap.cap |= IEEE80211_VHT_CAP_RXLDPC;
	if (rwnx_hw->mod_params->bfmee) {
		band_5GHz->vht_cap.cap |= IEEE80211_VHT_CAP_SU_BEAMFORMEE_CAPABLE;
		band_5GHz->vht_cap.cap |= 3 << IEEE80211_VHT_CAP_BEAMFORMEE_STS_SHIFT;
	}
	if (nss > 1)
		band_5GHz->vht_cap.cap |= IEEE80211_VHT_CAP_TXSTBC;

	// Update the AMSDU max RX size (not shifted as located at offset 0 of the VHT cap)
	band_5GHz->vht_cap.cap |= rwnx_hw->mod_params->amsdu_rx_max;

	if (rwnx_hw->mod_params->bfmer) {
		band_5GHz->vht_cap.cap |= IEEE80211_VHT_CAP_SU_BEAMFORMER_CAPABLE;
		/* Set number of sounding dimensions */
		band_5GHz->vht_cap.cap |= (nss - 1) << IEEE80211_VHT_CAP_SOUNDING_DIMENSIONS_SHIFT;
	}
	if (rwnx_hw->mod_params->murx)
		band_5GHz->vht_cap.cap |= IEEE80211_VHT_CAP_MU_BEAMFORMEE_CAPABLE;
	if (rwnx_hw->mod_params->mutx)
		band_5GHz->vht_cap.cap |= IEEE80211_VHT_CAP_MU_BEAMFORMER_CAPABLE;

	/*
	 * MCS map:
	 * This capabilities are filled according to the mcs_map module parameter.
	 * However currently we have some limitations due to FPGA clock constraints
	 * that prevent always using the range of MCS that is defined by the
	 * parameter:
	 *   - in RX, 2SS, we support up to MCS7
	 *   - in TX, 2SS, we support up to MCS8
	 */
	// Get max supported BW
	if (rwnx_hw->mod_params->use_80)
		bw_max = PHY_CHNL_BW_80;
	else if (rwnx_hw->mod_params->use_2040)
		bw_max = PHY_CHNL_BW_40;
	else
		bw_max = PHY_CHNL_BW_20;

	// Check if MCS map should be limited to MCS0_8 due to the standard. Indeed in BW20,
	// MCS9 is not supported in 1 and 2 SS
	if (rwnx_hw->mod_params->use_2040)
		mcs_map_max = IEEE80211_VHT_MCS_SUPPORT_0_9;
	else
		mcs_map_max = IEEE80211_VHT_MCS_SUPPORT_0_8;

	mcs_map = min_t(int, rwnx_hw->mod_params->mcs_map, mcs_map_max);
	band_5GHz->vht_cap.vht_mcs.rx_mcs_map = cpu_to_le16(0);
	for (i = 0; i < nss; i++) {
		band_5GHz->vht_cap.vht_mcs.rx_mcs_map |= cpu_to_le16(mcs_map << (i * 2));
		band_5GHz->vht_cap.vht_mcs.rx_highest = MAX_VHT_RATE(mcs_map, nss, bw_max);
		mcs_map = IEEE80211_VHT_MCS_SUPPORT_0_7;
	}
	for (; i < 8; i++) {
		band_5GHz->vht_cap.vht_mcs.rx_mcs_map |= cpu_to_le16(
					IEEE80211_VHT_MCS_NOT_SUPPORTED << (i * 2));
	}

	mcs_map = min_t(int, rwnx_hw->mod_params->mcs_map, mcs_map_max);
	band_5GHz->vht_cap.vht_mcs.tx_mcs_map = cpu_to_le16(0);
	for (i = 0; i < nss; i++) {
		band_5GHz->vht_cap.vht_mcs.tx_mcs_map |= cpu_to_le16(mcs_map << (i * 2));
		band_5GHz->vht_cap.vht_mcs.tx_highest = MAX_VHT_RATE(mcs_map, nss, bw_max);
		mcs_map = min_t(int, rwnx_hw->mod_params->mcs_map,
						IEEE80211_VHT_MCS_SUPPORT_0_8);
	}
	for (; i < 8; i++) {
		band_5GHz->vht_cap.vht_mcs.tx_mcs_map |= cpu_to_le16(
					IEEE80211_VHT_MCS_NOT_SUPPORTED << (i * 2));
	}

	if (!rwnx_hw->mod_params->use_80) {
#ifdef CONFIG_VENDOR_RWNX_VHT_NO80
		band_5GHz->vht_cap.cap |= IEEE80211_VHT_CAP_NOT_SUP_WIDTH_80;
#endif
		band_5GHz->vht_cap.cap &= ~IEEE80211_VHT_CAP_SHORT_GI_80;
	}
}

/*
 * HE MAC Capabilities Information: 0x084012087801
 *	  .... .... .... .... .... .... .... .... .... .... .... ...1 = +HTC HE Support: Supported
 *	  .... .... .... .... .... .... .... .... .... .... .... ..0. = TWT Requester Support: Not supported
 *	  .... .... .... .... .... .... .... .... .... .... .... .0.. = TWT Responder Support: Not supported
 *	  .... .... .... .... .... .... .... .... .... .... ...0 0... = Fragmentation Support: No support for dynamic fragmentation (0)
 *	  .... .... .... .... .... .... .... .... .... .... 000. .... = Reserved: 0x0
 *	  .... .... .... .... .... .... .... .... .... ..00 .... .... = Reserved: 0x0
 *	  .... .... .... .... .... .... .... .... .... 10.. .... .... = Trigger Frame MAC Padding Duration: 2
 *	  .... .... .... .... .... .... .... .... .111 .... .... .... = Multi-TID Aggregation Support: 7
 *	  .... .... .... .... .... .... .... ...0 0... .... .... .... = HE Link Adaptation Support: No feedback if the STA does not provide HE MFB (0)
 *	  .... .... .... .... .... .... .... ..0. .... .... .... .... = All Ack Support: Not supported
 *	  .... .... .... .... .... .... .... .0.. .... .... .... .... = TRS Support: Not supported
 *	  .... .... .... .... .... .... .... 1... .... .... .... .... = BSR Support: Supported
 *	  .... .... .... .... .... .... ...0 .... .... .... .... .... = Broadcast TWT Support: Not supported
 *	  .... .... .... .... .... .... ..0. .... .... .... .... .... = 32-bit BA Bitmap Support: Not supported
 *	  .... .... .... .... .... .... .0.. .... .... .... .... .... = MU Cascading Support: Not supported
 *	  .... .... .... .... .... .... 0... .... .... .... .... .... = Ack-Enabled Aggregation Support: Not supported
 *	  .... .... .... .... .... ...0 .... .... .... .... .... .... = Reserved: 0x0
 *	  .... .... .... .... .... ..1. .... .... .... .... .... .... = OM Control Support: Supported
 *	  .... .... .... .... .... .0.. .... .... .... .... .... .... = OFDMA RA Support: Not supported
 *	  .... .... .... .... ...1 0... .... .... .... .... .... .... = Maximum A-MPDU Length Exponent Extension: 2
 *	  .... .... .... .... ..0. .... .... .... .... .... .... .... = A-MSDU Fragmentation Support: Not supported
 *	  .... .... .... .... .0.. .... .... .... .... .... .... .... = Flexible TWT Schedule Support: Not supported
 *	  .... .... .... .... 0... .... .... .... .... .... .... .... = Rx Control Frame to MultiBSS: Not supported
 *	  .... .... .... ...0 .... .... .... .... .... .... .... .... = BSRP BQRP A-MPDU Aggregation: Not supported
 *	  .... .... .... ..0. .... .... .... .... .... .... .... .... = QTP Support: Not supported
 *	  .... .... .... .0.. .... .... .... .... .... .... .... .... = BQR Support: Not supported
 *	  .... .... .... 0... .... .... .... .... .... .... .... .... = SRP Responder Role: Not supported
 *	  .... .... ...0 .... .... .... .... .... .... .... .... .... = NDP Feedback Report Support: Not supported
 *	  .... .... ..0. .... .... .... .... .... .... .... .... .... = OPS Support: Not supported
 *	  .... .... .1.. .... .... .... .... .... .... .... .... .... = A-MSDU in A-MPDU Support: Supported
 *	  .... ..00 0... .... .... .... .... .... .... .... .... .... = Multi-TID Aggregation TX Support: 0
 *	  .... .0.. .... .... .... .... .... .... .... .... .... .... = HE Subchannel Selective Transmission Support: Not supported
 *	  .... 1... .... .... .... .... .... .... .... .... .... .... = UL 2x996-tone RU Support: Supported
 *	  ...0 .... .... .... .... .... .... .... .... .... .... .... = OM Control UL MU Data Disable RX Support: Not supported
 *	  ..0. .... .... .... .... .... .... .... .... .... .... .... = HE Dynamic SM Power Save: Not supported
 *	  .0.. .... .... .... .... .... .... .... .... .... .... .... = Punctured Sounding Support: Not supported
 *	  0... .... .... .... .... .... .... .... .... .... .... .... = HT And VHT Trigger Frame RX Support: Not supported
 */
static void rwnx_set_he_capa(struct rwnx_hw *rwnx_hw, struct wiphy *wiphy)
{
	struct ieee80211_supported_band *band_5GHz = wiphy->bands[IEEE80211_BAND_5GHZ];
	struct ieee80211_supported_band *band_2GHz = wiphy->bands[IEEE80211_BAND_2GHZ];
	int i;
	int nss = rwnx_hw->mod_params->nss;
	struct ieee80211_sta_he_cap *he_cap;
	int mcs_map, mcs_map_max_2ss = IEEE80211_HE_MCS_SUPPORT_0_11;
	u8 dcm_max_ru = IEEE80211_HE_PHY_CAP8_DCM_MAX_RU_242;
	uint32_t phy_vers = rwnx_hw->version_cfm.version_phy_2;

	if (!rwnx_hw->mod_params->he_on) {
		band_2GHz->iftype_data = NULL;
		band_2GHz->n_iftype_data = 0;
		band_5GHz->iftype_data = NULL;
		band_5GHz->n_iftype_data = 0;
		return;
	}

	he_cap = (struct ieee80211_sta_he_cap *)&band_2GHz->iftype_data->he_cap;
	he_cap->has_he = true;

	if (rwnx_hw->version_cfm.features & BIT(MM_FEAT_TWT_BIT)) {
		rwnx_hw->ext_capa[9] = WLAN_EXT_CAPA10_TWT_REQUESTER_SUPPORT;
		he_cap->he_cap_elem.mac_cap_info[0] |= IEEE80211_HE_MAC_CAP0_TWT_REQ;
	}

	he_cap->he_cap_elem.mac_cap_info[2] |= IEEE80211_HE_MAC_CAP2_ALL_ACK;
	rwnx_set_ppe_threshold(rwnx_hw, he_cap);
#if (!CONFIG_SOC_BK7236N) && (!CONFIG_SOC_BK7239XX)
	if (rwnx_hw->mod_params->use_2040) {
		he_cap->he_cap_elem.phy_cap_info[0] |=
			IEEE80211_HE_PHY_CAP0_CHANNEL_WIDTH_SET_40MHZ_IN_2G;
		dcm_max_ru = IEEE80211_HE_PHY_CAP8_DCM_MAX_RU_484;
	}
	if (rwnx_hw->mod_params->use_80) {
		he_cap->he_cap_elem.phy_cap_info[0] |=
			IEEE80211_HE_PHY_CAP0_CHANNEL_WIDTH_SET_40MHZ_80MHZ_IN_5G;
		mcs_map_max_2ss = IEEE80211_HE_MCS_SUPPORT_0_7;
		dcm_max_ru = IEEE80211_HE_PHY_CAP8_DCM_MAX_RU_996;
	}
#endif
	if (rwnx_hw->mod_params->ldpc_on) {
		he_cap->he_cap_elem.phy_cap_info[1] |= IEEE80211_HE_PHY_CAP1_LDPC_CODING_IN_PAYLOAD;
	} else {
		// If no LDPC is supported, we have to limit to MCS0_9, as LDPC is mandatory
		// for MCS 10 and 11
		rwnx_hw->mod_params->he_mcs_map = min_t(int, rwnx_hw->mod_params->he_mcs_map,
												IEEE80211_HE_MCS_SUPPORT_0_9);
	}

	// CONFIG_HE_MCS_LIMIT_7
	if (rwnx_hw->version_cfm.features & (1ULL << MM_FEAT_HE_MCS_LIMIT_7_BIT)) {
		// limit he mcs to 7
		rwnx_hw->mod_params->mcs_map = min_t(int, rwnx_hw->mod_params->mcs_map,
												IEEE80211_VHT_MCS_SUPPORT_0_7);
		rwnx_hw->mod_params->he_mcs_map = min_t(int, rwnx_hw->mod_params->he_mcs_map,
												IEEE80211_HE_MCS_SUPPORT_0_7);
	}

	he_cap->he_cap_elem.phy_cap_info[1] |= IEEE80211_HE_PHY_CAP1_HE_LTF_AND_GI_FOR_HE_PPDUS_0_8US |
										   IEEE80211_HE_PHY_CAP1_MIDAMBLE_RX_TX_MAX_NSTS;
	he_cap->he_cap_elem.phy_cap_info[2] |= IEEE80211_HE_PHY_CAP2_MIDAMBLE_RX_TX_MAX_NSTS |
										   IEEE80211_HE_PHY_CAP2_NDP_4x_LTF_AND_3_2US |
										   IEEE80211_HE_PHY_CAP2_DOPPLER_RX;
	if (rwnx_hw->mod_params->stbc_on)
		he_cap->he_cap_elem.phy_cap_info[2] |= IEEE80211_HE_PHY_CAP2_STBC_RX_UNDER_80MHZ;
	he_cap->he_cap_elem.phy_cap_info[3] |=
#if CONFIG_SOC_BK7256XX
		// Disable sta DCM capabilities temporarily because we can't receive DCM MCS0 ACK for BK7256XX
		IEEE80211_HE_PHY_CAP3_DCM_MAX_CONST_RX_NO_DCM |
#else
		IEEE80211_HE_PHY_CAP3_DCM_MAX_CONST_RX_16_QAM |
#endif
		IEEE80211_HE_PHY_CAP3_RX_PARTIAL_BW_SU_IN_20MHZ_MU;
	if (nss > 0)
		he_cap->he_cap_elem.phy_cap_info[3] |= IEEE80211_HE_PHY_CAP3_DCM_MAX_RX_NSS_2;
	else
		he_cap->he_cap_elem.phy_cap_info[3] |= IEEE80211_HE_PHY_CAP3_DCM_MAX_RX_NSS_1;

	if (rwnx_hw->mod_params->bfmee) {
		he_cap->he_cap_elem.phy_cap_info[4] |= IEEE80211_HE_PHY_CAP4_SU_BEAMFORMEE;
		he_cap->he_cap_elem.phy_cap_info[4] |=
			IEEE80211_HE_PHY_CAP4_BEAMFORMEE_MAX_STS_UNDER_80MHZ_4;
	}
	he_cap->he_cap_elem.phy_cap_info[5] |= IEEE80211_HE_PHY_CAP5_NG16_SU_FEEDBACK |
										   IEEE80211_HE_PHY_CAP5_NG16_MU_FEEDBACK;
	he_cap->he_cap_elem.phy_cap_info[6] |= IEEE80211_HE_PHY_CAP6_CODEBOOK_SIZE_42_SU |
										   IEEE80211_HE_PHY_CAP6_CODEBOOK_SIZE_75_MU |
										   IEEE80211_HE_PHY_CAP6_TRIG_SU_BEAMFORMING_FB |
										   IEEE80211_HE_PHY_CAP6_TRIG_MU_BEAMFORMING_PARTIAL_BW_FB |
										   IEEE80211_HE_PHY_CAP6_PPE_THRESHOLD_PRESENT |
										   IEEE80211_HE_PHY_CAP6_PARTIAL_BANDWIDTH_DL_MUMIMO;
	he_cap->he_cap_elem.phy_cap_info[7] |= IEEE80211_HE_PHY_CAP7_HE_SU_MU_PPDU_4XLTF_AND_08_US_GI;
	he_cap->he_cap_elem.phy_cap_info[8] |= IEEE80211_HE_PHY_CAP8_20MHZ_IN_40MHZ_HE_PPDU_IN_2G |
										   dcm_max_ru;
	he_cap->he_cap_elem.phy_cap_info[9] |= IEEE80211_HE_PHY_CAP9_RX_FULL_BW_SU_USING_MU_WITH_COMP_SIGB |
										   IEEE80211_HE_PHY_CAP9_RX_FULL_BW_SU_USING_MU_WITH_NON_COMP_SIGB |
										   IEEE80211_HE_PHY_CAP9_NOMIMAL_PKT_PADDING_16US;

	// Starting from version v31 more HE_ER_SU modulations is supported
	if (__MDM_VERSION(phy_vers) > 30) {
		he_cap->he_cap_elem.phy_cap_info[6] |= IEEE80211_HE_PHY_CAP6_PARTIAL_BW_EXT_RANGE;
		he_cap->he_cap_elem.phy_cap_info[8] |= IEEE80211_HE_PHY_CAP8_HE_ER_SU_1XLTF_AND_08_US_GI |
											   IEEE80211_HE_PHY_CAP8_HE_ER_SU_PPDU_4XLTF_AND_08_US_GI;
	}

	mcs_map = rwnx_hw->mod_params->he_mcs_map;
	memset(&he_cap->he_mcs_nss_supp, 0, sizeof(he_cap->he_mcs_nss_supp));
	for (i = 0; i < nss; i++) {
		__le16 unsup_for_ss = cpu_to_le16(IEEE80211_HE_MCS_NOT_SUPPORTED << (i * 2));
		he_cap->he_mcs_nss_supp.rx_mcs_80 |= cpu_to_le16(mcs_map << (i * 2));
		he_cap->he_mcs_nss_supp.rx_mcs_160 |= unsup_for_ss;
		he_cap->he_mcs_nss_supp.rx_mcs_80p80 |= unsup_for_ss;
		mcs_map = min_t(int, rwnx_hw->mod_params->he_mcs_map,
						mcs_map_max_2ss);
	}
	for (; i < 8; i++) {
		__le16 unsup_for_ss = cpu_to_le16(IEEE80211_HE_MCS_NOT_SUPPORTED << (i * 2));
		he_cap->he_mcs_nss_supp.rx_mcs_80 |= unsup_for_ss;
		he_cap->he_mcs_nss_supp.rx_mcs_160 |= unsup_for_ss;
		he_cap->he_mcs_nss_supp.rx_mcs_80p80 |= unsup_for_ss;
	}
	mcs_map = rwnx_hw->mod_params->he_mcs_map;
	for (i = 0; i < nss; i++) {
		__le16 unsup_for_ss = cpu_to_le16(IEEE80211_HE_MCS_NOT_SUPPORTED << (i * 2));
		he_cap->he_mcs_nss_supp.tx_mcs_80 |= cpu_to_le16(mcs_map << (i * 2));
		he_cap->he_mcs_nss_supp.tx_mcs_160 |= unsup_for_ss;
		he_cap->he_mcs_nss_supp.tx_mcs_80p80 |= unsup_for_ss;
		mcs_map = min_t(int, rwnx_hw->mod_params->he_mcs_map,
						mcs_map_max_2ss);
	}
	for (; i < 8; i++) {
		__le16 unsup_for_ss = cpu_to_le16(IEEE80211_HE_MCS_NOT_SUPPORTED << (i * 2));
		he_cap->he_mcs_nss_supp.tx_mcs_80 |= unsup_for_ss;
		he_cap->he_mcs_nss_supp.tx_mcs_160 |= unsup_for_ss;
		he_cap->he_mcs_nss_supp.tx_mcs_80p80 |= unsup_for_ss;
	}
}

int rwnx_handle_dynparams()
{
	int ret;
	struct rwnx_hw *rwnx_hw = &g_rwnx_hw;
	struct wiphy *wiphy = &g_wiphy;

	// enable MCS 0-9 when ATE is enable
	if (ate_is_enabled())
	{
		rwnx_update_vht_mcs(IEEE80211_VHT_MCS_SUPPORT_0_9);
		rwnx_update_he_mcs(IEEE80211_HE_MCS_SUPPORT_0_9);
	}

	/* Check compatibility between requested parameters and HW/SW features */
	ret = rwnx_check_fw_hw_feature(rwnx_hw, wiphy);
	if (ret)
		return ret;

	/* Set VHT capabilities */
	rwnx_set_vht_capa(rwnx_hw, wiphy);

	/* Set HE capabilities */
	rwnx_set_he_capa(rwnx_hw, wiphy);

	/* Set HT capabilities */
	rwnx_set_ht_capa(rwnx_hw, wiphy);

	/* Set ERP capabilities */
	rwnx_set_erp_capa(rwnx_hw, wiphy);

#ifdef XXX
	/* Set RF specific parameters (shall be done last as it might change some
	   capabilities previously set) */
	rwnx_set_rf_params(rwnx_hw, wiphy);
#endif

	return 0;
}
void rwnx_udpate_capability(uint32_t capa_id, uint32_t capa_val)
{
	switch (capa_id)
	{
		case WIFI_CAPA_ID_ERP_EN:
			rwnx_update_erp_capa(capa_val);
			break;
		case WIFI_CAPA_ID_HT_EN:
			rwnx_update_ht_capa(capa_val);
			break;
		case WIFI_CAPA_ID_VHT_EN:
			rwnx_update_vht_capa(capa_val);
			break;
		case WIFI_CAPA_ID_HE_EN:
			rwnx_update_he_capa(capa_val);
			break;
		case WIFI_CAPA_ID_TX_AMPDU_EN:
			rwnx_update_tx_ampdu_capa(capa_val);
			break;
		case WIFI_CAPA_ID_RX_AMPDU_EN:
			rwnx_update_rx_ampdu_capa(capa_val);
			break;
		case WIFI_CAPA_ID_TX_AMPDU_NUM:
			rwnx_update_tx_ampdu_num_capa(capa_val);
			break;
		case WIFI_CAPA_ID_RX_AMPDU_NUM:
			rwnx_update_rx_ampdu_num_capa(capa_val);
			break;
		case WIFI_CAPA_ID_VHT_MCS:
			rwnx_update_vht_mcs(capa_val);
			break;
		case WIFI_CAPA_ID_HE_MCS:
			rwnx_update_he_mcs(capa_val);
			break;
		case WIFI_CAPA_ID_B40_EN:
			rwnx_update_b40(capa_val);
			break;
		case WIFI_CAPA_ID_STBC_EN:
			rwnx_update_stbc(capa_val);
			break;
		case WIFI_CAPA_ID_SGI_EN:
			rwnx_update_sgi(capa_val);
			break;
		case WIFI_CAPA_ID_LDPC_EN:
			rwnx_update_ldpc(capa_val);
			break;
		case WIFI_CAPA_ID_BEAMFORMEE_EN:
			rwnx_update_beamformee(capa_val);
			break;
		case WIFI_CAPA_ID_11B_ONLY_EN:
			rwnx_update_11b_only(capa_val);
			break;
		default:
			break;
	}
}

bool rwnx_get_tx_ampdu_capa_on(void)
{
    return rwnx_mod_params.tx_ampdu_on;
}

uint32_t rwnx_get_tx_ampdu_num(void)
{
    return rwnx_mod_params.tx_agg_num;
}

bool rwnx_get_rx_ampdu_capa_on(void)
{
	return (rwnx_mod_params.rx_ampdu_on);
}

uint8_t rwnx_get_rx_ampdu_num(void)
{
	return (rwnx_mod_params.rx_agg_num);
}
