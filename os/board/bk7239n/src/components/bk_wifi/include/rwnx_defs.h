/**
 ******************************************************************************
 *
 * @file rwnx_defs.h
 *
 * @brief Main driver structure declarations for fullmac driver
 *
 * Copyright (C) RivieraWaves 2012-2019
 *
 ******************************************************************************
 */

#ifndef _RWNX_DEFS_H_
#define _RWNX_DEFS_H_

#include "bk_rw.h"
#include "bk_workqueue.h"
#include "../../../../../../../components/wpa_supplicant-2.10/src/drivers/driver.h"
#if CONFIG_RWNX_SW_TXQ
#include "rwnx_txq.h"
//#include "os/os.h"
#endif

#define __MDM_MAJOR_VERSION(v)     (((v) & 0xFF000000) >> 24)
#define __MDM_MINOR_VERSION(v)     (((v) & 0x00FF0000) >> 16)
#define __MDM_PHYCFG_FROM_VERS(v)  (((v) & MDM_RFMODE_MASK) >> MDM_RFMODE_LSB)
#define __MDM_VERSION(v)           ((__MDM_MAJOR_VERSION(v) + 2) * 10 + __MDM_MINOR_VERSION(v))

#define MDM_PHY_CONFIG_TRIDENT     0
#define MDM_PHY_CONFIG_ELMA        1
#define MDM_PHY_CONFIG_KARST       2

#define RWNX_TX_LIFETIME_MS             200

struct rwnx_sec_phy_chan {
	u16 prim20_freq;
	u16 center_freq1;
	u16 center_freq2;
	//enum nl80211_band band;
	u8 type;
};

#define RWNX_CH_NOT_SET 0xFF
#define RWNX_INVALID_VIF 0xFF
#define RWNX_INVALID_STA 0xFF

struct rwnx_hw;

/**
 * rwnx_phy_info - Phy information
 *
 * @phy_cnt: Number of phy interface
 * @cfg: Configuration send to firmware
 * @sec_chan: Channel configuration of the second phy interface (if phy_cnt > 1)
 * @limit_bw: Set to true to limit BW on requested channel. Only set to use
 * VHT with old radio that don't support 80MHz (deprecated)
 */
struct rwnx_phy_info {
	u8 cnt;
	struct phy_cfg_tag cfg;
	struct rwnx_sec_phy_chan sec_chan;
	bool limit_bw;
};

/* Structure that will contains all RoC information received from cfg80211 */
struct rwnx_roc_elem {
	unsigned int freq;
    unsigned int duration;
    /* Used to avoid call of CFG80211 callback upon expiration of RoC */
    //bool mgmt_roc;
    /* Indicate if we have switch on the RoC channel */
    bool on_chan;
};

/**
 * struct rwnx_bcn - Information of the beacon in used (AP mode)
 *
 * @head: head portion of beacon (before TIM IE)
 * @tail: tail portion of beacon (after TIM IE)
 * @ies: extra IEs (not used ?)
 * @head_len: length of head data
 * @tail_len: length of tail data
 * @ies_len: length of extra IEs data
 * @tim_len: length of TIM IE
 * @len: Total beacon len (head + tim + tail + extra)
 * @dtim: dtim period
 */
struct rwnx_bcn {
    u8 *head;
    u8 *tail;
    u8 *ies;
    size_t head_len;
    size_t tail_len;
    size_t ies_len;
    size_t tim_len;
    size_t len;
    u8 dtim;
};

/**
 * struct rwnx_csa - Information for CSA (Channel Switch Announcement)
 *
 * @vif: Pointer to the vif doing the CSA
 * @bcn: Beacon to use after CSA
 * @elem: IPC buffer to send the new beacon to the fw
 * @chandef: defines the channel to use after the switch
 * @count: Current csa counter
 * @status: Status of the CSA at fw level
 * @ch_idx: Index of the new channel context
 * @work: work scheduled at the end of CSA
 */
struct rwnx_csa {
    //struct rwnx_vif *vif;
    uint8_t vif_idx;
    struct rwnx_bcn bcn;
    //struct rwnx_ipc_elem_var elem;
    //struct cfg80211_chan_def chandef;
    int count;
    //int status;
    //int ch_idx;
    //struct work_struct work;
};

// TODO: add lock (mutex, spinlock, etc.)
struct rwnx_hw {
	struct rwnx_mod_params *mod_params;
	struct wiphy *wiphy;
	unsigned long drv_flags;
	u8 monitor_vif; 		/* FW id of the monitor interface, RWNX_INVALID_VIF if no monitor vif at fw level */

	struct mm_version_cfm version_cfm;          /* Lower layers versions - obtained via MM_VERSION_REQ */

	u8 avail_idx_map;
	u8 vif_started;
	bool adding_sta;

    /* RoC Management */
    struct rwnx_roc_elem *roc_elem;             /* Information provided by cfg80211 in its remain on channel request */

	//struct rwnx_phy_info phy;

	//struct rwnx_radar radar;

	/* extended capabilities supported */
	u8 ext_capa[10];

#ifdef CONFIG_RWNX_MUMIMO_TX
	struct rwnx_mu_info mu;
#endif

#if CONFIG_RWNX_SW_TXQ
	// TX path
	spinlock_t tx_lock;
	struct rwnx_txq *txq;//[NX_NB_TXQ];
	struct rwnx_hwq hwq[NX_TXQ_CNT];
	beken_timer_t txq_cleanup;
	uint8_t cur_chanctx;
#endif

	//struct rwnx_csa *csa;
	BCN_PARAM_ST *csa;
	struct hostapd_freq_params freq_params;

	struct bk_work csa_work;
};

#if CONFIG_RWNX_SW_TXQ
static inline bool is_multicast_sta(uint8_t sta_idx)
{
    return (sta_idx >= NX_REMOTE_STA_MAX);
}

#define LEGACY_PS_ID   0
#define UAPSD_ID       1
#endif


// MODEM features (from reg_mdm_stat.h)
/// MUMIMOTX field bit
#define MDM_MUMIMOTX_BIT    ((u32)0x80000000)
/// MUMIMOTX field position
#define MDM_MUMIMOTX_POS    31
/// MUMIMORX field bit
#define MDM_MUMIMORX_BIT    ((u32)0x40000000)
/// MUMIMORX field position
#define MDM_MUMIMORX_POS    30
/// BFMER field bit
#define MDM_BFMER_BIT       ((u32)0x20000000)
/// BFMER field position
#define MDM_BFMER_POS       29
/// BFMEE field bit
#define MDM_BFMEE_BIT       ((u32)0x10000000)
/// BFMEE field position
#define MDM_BFMEE_POS       28
/// LDPCDEC field bit
#define MDM_LDPCDEC_BIT     ((u32)0x08000000)
/// LDPCDEC field position
#define MDM_LDPCDEC_POS     27
/// LDPCENC field bit
#define MDM_LDPCENC_BIT     ((u32)0x04000000)
/// LDPCENC field position
#define MDM_LDPCENC_POS     26
/// CHBW field mask
#define MDM_CHBW_MASK       ((u32)0x03000000)
/// CHBW field LSB position
#define MDM_CHBW_LSB        24
/// CHBW field width
#define MDM_CHBW_WIDTH      ((u32)0x00000002)
/// DSSSCCK field bit
#define MDM_DSSSCCK_BIT     ((u32)0x00800000)
/// DSSSCCK field position
#define MDM_DSSSCCK_POS     23
/// VHT field bit
#define MDM_VHT_BIT         ((u32)0x00400000)
/// VHT field position
#define MDM_VHT_POS         22
/// HE field bit
#define MDM_HE_BIT          ((u32)0x00200000)
/// HE field position
#define MDM_HE_POS          21
/// ESS field bit
#define MDM_ESS_BIT         ((u32)0x00100000)
/// ESS field position
#define MDM_ESS_POS         20
/// RFMODE field mask
#define MDM_RFMODE_MASK     ((u32)0x000F0000)
/// RFMODE field LSB position
#define MDM_RFMODE_LSB      16
/// RFMODE field width
#define MDM_RFMODE_WIDTH    ((u32)0x00000004)
/// NSTS field mask
#define MDM_NSTS_MASK       ((u32)0x0000F000)
/// NSTS field LSB position
#define MDM_NSTS_LSB        12
/// NSTS field width
#define MDM_NSTS_WIDTH      ((u32)0x00000004)
/// NSS field mask
#define MDM_NSS_MASK        ((u32)0x00000F00)
/// NSS field LSB position
#define MDM_NSS_LSB         8
/// NSS field width
#define MDM_NSS_WIDTH       ((u32)0x00000004)
/// NTX field mask
#define MDM_NTX_MASK        ((u32)0x000000F0)
/// NTX field LSB position
#define MDM_NTX_LSB         4
/// NTX field width
#define MDM_NTX_WIDTH       ((u32)0x00000004)
/// NRX field mask
#define MDM_NRX_MASK        ((u32)0x0000000F)
/// NRX field LSB position
#define MDM_NRX_LSB         0
/// NRX field width
#define MDM_NRX_WIDTH       ((u32)0x00000004)

#define __MDM_PHYCFG_FROM_VERS(v)  (((v) & MDM_RFMODE_MASK) >> MDM_RFMODE_LSB)


#define NXMAC_VERSION_1_ADDR           0x00B00004
#define   NXMAC_MU_MIMO_TX_BIT         BIT(19)
#define   NXMAC_BFMER_BIT              BIT(18)
#define   NXMAC_BFMEE_BIT              BIT(17)
#define   NXMAC_MAC_80211MH_FORMAT_BIT BIT(16)
#define   NXMAC_COEX_BIT               BIT(14)
#define   NXMAC_WAPI_BIT               BIT(13)
#define   NXMAC_TPC_BIT                BIT(12)
#define   NXMAC_VHT_BIT                BIT(11)
#define   NXMAC_HT_BIT                 BIT(10)
#define   NXMAC_RCE_BIT                BIT(8)
#define   NXMAC_CCMP_BIT               BIT(7)
#define   NXMAC_TKIP_BIT               BIT(6)
#define   NXMAC_WEP_BIT                BIT(5)
#define   NXMAC_SECURITY_BIT           BIT(4)
#define   NXMAC_SME_BIT                BIT(3)
#define   NXMAC_HCCA_BIT               BIT(2)
#define   NXMAC_EDCA_BIT               BIT(1)
#define   NXMAC_QOS_BIT                BIT(0)

#define RWNX_TAG "rwnx"
#define RWNX_LOGI(...) BK_WIFI_LOGI(RWNX_TAG, ##__VA_ARGS__)
#define RWNX_LOGW(...) BK_WIFI_LOGW(RWNX_TAG, ##__VA_ARGS__)
#define RWNX_LOGE(...) BK_WIFI_LOGE(RWNX_TAG, ##__VA_ARGS__)
#define RWNX_LOGD(...) BK_WIFI_LOGD(RWNX_TAG, ##__VA_ARGS__)
#define RWNX_LOGI_RAW(...) BK_WIFI_RAW_LOGI(RWNX_TAG, ##__VA_ARGS__)
extern struct rwnx_hw g_rwnx_hw;
#endif /* _RWNX_DEFS_H_ */

