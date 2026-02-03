#include <common/bk_include.h>
#include "rw_ieee80211.h"
#include "bk_rw.h"
#include "rw_msdu.h"
#include <os/mem.h>
#include <os/str.h>
#include "rwnx_intf.h"
#include "rwnx_defs.h"
#include "rwnx_config.h"
#include "rwnx_rx.h"
#include "rwnx_params.h"
#include "bk_workqueue.h"
#include "../wpa_supplicant-2.10/wpa_supplicant/ctrl_iface.h"
#include "rwnx_defs.h"
#include "components/ate.h"
#include "rwnx_tx.h"
#include "wifi_v2.h"
#if CONFIG_RWNX_TD
#include "rwnx_td.h"
#endif

int rwnx_reg_notifier(struct wiphy *wiphy,
	struct regulatory_request *request);
extern int manual_cal_get_tx_power(wifi_standard standard, float *powerdBm);
extern int manual_cal_set_tx_power(wifi_standard standard, float powerdBm);

#if CONFIG_POWER_TABLE
//#include "bk_pwr_tbl.h"
#endif
#include <modules/wifi.h>
#include "bk_wifi.h"
#include "bk_wifi_prop_private.h"
#include "reg_domain.h"

#if !CONFIG_WIFI_REGDOMAIN
typedef struct _wifi_cn_code_st_ {
	UINT32 init;
	wifi_country_t cfg;
	wifi_country_policy_t policy;         /**< country policy */
#if CONFIG_WIFI_BAND_5G
	uint8_t chan_5g_cnt;
	uint8_t chan_5g_list[WIFI_CHANNEL_NUM_5G];
#endif
} WIFI_CN_ST, *WIFI_CN_PTR;

WIFI_CN_ST g_country_code = {0};

#define COUNTRY_CODE_CN   {.cc= "CN", .schan=1, .nchan=13, .max_tx_power=0, .policy=WIFI_COUNTRY_POLICY_MANUAL};
#define COUNTRY_CODE_US   {.cc= "US", .schan=1, .nchan=11, .max_tx_power=0, .policy=WIFI_COUNTRY_POLICY_MANUAL};
#define COUNTRY_CODE_EP   {.cc= "EP", .schan=1, .nchan=13, .max_tx_power=0, .policy=WIFI_COUNTRY_POLICY_MANUAL};
#define COUNTRY_CODE_JP   {.cc= "JP", .schan=1, .nchan=14, .max_tx_power=0, .policy=WIFI_COUNTRY_POLICY_MANUAL};
#define COUNTRY_CODE_AU   {.cc= "AU", .schan=1, .nchan=13, .max_tx_power=0, .policy=WIFI_COUNTRY_POLICY_MANUAL};
#endif // CONFIG_WIFI_REGDOMAIN

static struct ieee80211_channel rw_2ghz_channels[] = {
	CHAN2G(1, 2412, 0),
	CHAN2G(2, 2417, 0),
	CHAN2G(3, 2422, 0),
	CHAN2G(4, 2427, 0),
	CHAN2G(5, 2432, 0),
	CHAN2G(6, 2437, 0),
	CHAN2G(7, 2442, 0),
	CHAN2G(8, 2447, 0),
	CHAN2G(9, 2452, 0),
	CHAN2G(10, 2457, 0),
	CHAN2G(11, 2462, 0),
	CHAN2G(12, 2467, 0),
	CHAN2G(13, 2472, 0),
	CHAN2G(14, 2484, 0),
};

// Total MAC_DOMAINCHANNEL_5G_MAX 5G channels
struct ieee80211_channel rw_5ghz_channels[] = {
	CHAN5G(36, 0),
	CHAN5G(40, 0),
	CHAN5G(44, 0),
	CHAN5G(48, 0),
	CHAN5G(52, 0),
	CHAN5G(56, 0),
	CHAN5G(60, 0),
	CHAN5G(64, 0),
	CHAN5G(100, 0),
	CHAN5G(104, 0),
	CHAN5G(108, 0),
	CHAN5G(112, 0),
	CHAN5G(116, 0),
	CHAN5G(120, 0),
	CHAN5G(124, 0),
	CHAN5G(128, 0),
	CHAN5G(132, 0),
	CHAN5G(136, 0),
	CHAN5G(140, 0),
	CHAN5G(144, 0),
	CHAN5G(149, 0),
	CHAN5G(153, 0),
	CHAN5G(157, 0),
	CHAN5G(161, 0),
	CHAN5G(165, 0),
	CHAN5G(169, 0),
	CHAN5G(173, 0),
	CHAN5G(177, 0),
};

typedef struct {
	char countrycode[3];
	int channels[MAC_DOMAINCHANNEL_24G_MAX];
	int num_channels;
} Country24gchannels;

Country24gchannels country_24g_channel_table[] = {
	{"CN", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}, 13},
	{"EP", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}, 13},
	{"AU", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}, 13},
	{"CA", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, 11},
	{"FM", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, 11},
	{"GU", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, 11},
	{"JP", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,14},14},
	{"KY", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, 11},
	{"MP", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, 11},
	{"UM", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, 11},
	{"US", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, 11},
	{"VI", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, 11},
	{"RU", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}, 13},
	{"SIN", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}, 13},
	{"ISR", {3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}, 11},
	{"KOR", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}, 13},
	{"TUR", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}, 13},
	{"SAF", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}, 13},
	{"BRA", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}, 13},
	{"NZE", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}, 13},
	{"TAI", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}, 13},
};


#if CONFIG_WIFI_BAND_5G && !CONFIG_WIFI_REGDOMAIN
//struct for channels list
typedef struct {
	char countrycode[3];
	int channels_5g[MAC_DOMAINCHANNEL_5G_MAX];
	int num_channels;
} Countrychannels;

Countrychannels non_radar_avaliable_channel_table[] = {
	{"CN",  {36, 40, 44, 48, 149, 153, 157, 161, 165}, 9},
#if 0
	{"US", {36, 40, 44, 48, 149, 153, 157, 161, 165}, 9},
	{"CAN", {36, 40, 44, 48, 149, 153, 157, 161, 165}, 9},
	{"EP",  {36, 40, 44, 48, 149, 153, 157, 161, 165}, 9},
	{"SWI", {36, 40, 44, 48, 149, 153, 157, 161, 165}, 9},
	{"RU",  {36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132,\
			136, 140, 144, 149, 153, 157, 161, 165}, 25},
	{"JP",  {36, 40, 44, 48}, 4},
	{"SIN", {36, 40, 44, 48, 149, 153, 157, 161, 165}, 9},
	{"TAI", {36, 40, 44, 48, 149, 153, 157, 161, 165}, 9},
	{"ISR", {36, 40, 44, 48}, 4},
	{"KOR", {36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 149,\
			153, 157, 161, 165}, 21},
	{"TUR", {36, 40, 44, 48, 52, 56, 60, 64}, 8},
	{"AU", {36, 40, 44, 48, 149, 153, 157, 161, 165}, 9},
	{"SAF", {36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132,\
			136, 140}, 19},
	{"BRA", {36, 40, 44, 48, 52, 56, 60, 64, 149, 153, 157, 161, 165}, 13},
	{"NZE", {36, 40, 44, 48, 149, 153, 157, 161, 165}, 9},
#endif
};

//define 5g channels list for country
Countrychannels country_channel_table[] = {
	{"CN", {36, 40, 44, 48, 52, 56, 60, 64, 149, 153, 157, 161, 165}, 13},
#if 1
	{"US", {36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132, \
			136, 140, 144, 149, 153, 157, 161, 165}, 25},
	{"CA", {36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 132, 136, 140, 144,\
			149, 153, 157, 161, 165}, 22},
	{"EP", {36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132,\
			136, 140, 149, 153, 157, 161, 165}, 24},
	#if 0
	{"SWI", {36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132,\
			136, 140, 149, 153, 157, 161, 165}, 24},
	#endif
	{"RU", {36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132,\
			136, 140, 144, 149, 153, 157, 161, 165}, 25},
	{"JP", {36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132,\
			136, 140}, 19},
	{"SIN", {36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132,\
			136, 140, 144, 149, 153, 157, 161, 165}, 25},
	{"TAI", {36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132,\
			136, 140, 149, 153, 157, 161, 165}, 24},
	{"ISR", {36, 40, 44, 48, 52, 56, 60, 64}, 8},
	{"KOR", {36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 149,\
			153, 157, 161, 165}, 21},
	{"TUR", {36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 132, 136, 140, 144,\
			149, 153, 157, 161, 165}, 22},
	{"AU", {36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 132, 136, 140, 144, \
			149, 153, 157, 161, 165}, 22},
	{"SAF", {36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132,\
			136, 140}, 19},
	{"BRA", {36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132,\
			136, 140, 149, 153, 157, 161, 165}, 24},
	{"NZE", {36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132,\
			136, 140, 144, 149, 153, 157, 161, 165}, 25},
	{"KY", {36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132,\
			136, 140, 144, 149, 153, 157, 161, 165}, 25},
	{"FM", {36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132, \
			136, 140, 144, 149, 153, 157, 161, 165}, 25},///refer to US
	{"MP", {36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132, \
			136, 140, 144, 149, 153, 157, 161, 165}, 25},///refer to US
	{"UM", {36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132, \
			136, 140, 144, 149, 153, 157, 161, 165}, 25},///refer to US
	{"VI", {36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108, 112, 116, 120, 124, 128, 132, \
			136, 140, 144, 149, 153, 157, 161, 165}, 25},///refer to US
#endif
};
#endif //CONFIG_WIFI_BAND_5G



struct wiphy g_wiphy;
struct rwnx_hw g_rwnx_hw;

unsigned char beacon[149] = {
	0x80, 0x00, // Frame Control
	0x00, 0x00, // Duration
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // Addr1
	0x12, 0x71, 0x11, 0x71, 0x0B, 0x71, // Addr2
	0x12, 0x71, 0x11, 0x71, 0x0B, 0x71, // Addr3
	0x00, 0x00, // SN
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Timestamp
	0x64, 0x00, // Beacon Interval
	0x31, 0x04, // Capabilities
	0x00, 0x05, 0x4A, 0x49, 0x45, 0x57, 0x55, // SSID
	0x01, 0x08, 0x82, 0x84, 0x8B, 0x96, 0x0C, 0x12, 0x18, 0x24, // Supported rates
	0x03, 0x01, 0x06, // DS
	0x05, 0x04, 0x00, 0x02, 0x00, 0x00, // TIM
	0x2A, 0x01, 0x00, // ERP
	0x32, 0x04, 0x30, 0x48, 0x60, 0x6C, // Extended Supported Rates
	0x2D, 0x1A, 0x0C, 0x00, 0x1B, 0xFF, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x96,
	0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // HT Capabilities
	0x3D, 0x16, 0x08, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // HT Information
	0xDD, 0x18, 0x00, 0x50, 0xF2, 0x02, 0x01, 0x01, 0x00, 0x00, 0x03, 0xA4, 0x00, 0x00, 0x27, 0xA4,
	0x00, 0x00, 0x42, 0x43, 0x5E, 0x00, 0x62, 0x32, 0x2F, 0x00 // WMM
};

#define RWNX_HT_CAPABILITIES                                    \
	{                                                               \
		.ht_supported   = true,                                     \
						  .cap            = 0,                                        \
		.ampdu_factor   = IEEE80211_HT_MAX_AMPDU_8K,                \
		.ampdu_density  = IEEE80211_HT_MPDU_DENSITY_16,             \
		.mcs        = {                                             \
		.rx_mask = { 0xff, 0, 0, 0, 0, 0, 0, 0, 0, 0, },        \
		.rx_highest = cpu_to_le16(65),                          \
		.tx_params = IEEE80211_HT_MCS_TX_DEFINED,               \
		},                                                          \
	}

#define RWNX_VHT_CAPABILITIES                                   \
	{                                                               \
		.vht_supported = false,                                     \
		.cap       =                                                \
			(3 << IEEE80211_VHT_CAP_MAX_A_MPDU_LENGTH_EXPONENT_SHIFT),\
		.vht_mcs       = {                                          \
		.rx_mcs_map = cpu_to_le16(                              \
			IEEE80211_VHT_MCS_SUPPORT_0_9    << 0  |  \
			IEEE80211_VHT_MCS_NOT_SUPPORTED  << 2  |  \
			IEEE80211_VHT_MCS_NOT_SUPPORTED  << 4  |  \
			IEEE80211_VHT_MCS_NOT_SUPPORTED  << 6  |  \
			IEEE80211_VHT_MCS_NOT_SUPPORTED  << 8  |  \
			IEEE80211_VHT_MCS_NOT_SUPPORTED  << 10 |  \
			IEEE80211_VHT_MCS_NOT_SUPPORTED  << 12 |  \
			IEEE80211_VHT_MCS_NOT_SUPPORTED  << 14),  \
		.tx_mcs_map = cpu_to_le16(                              \
			IEEE80211_VHT_MCS_SUPPORT_0_9    << 0  |  \
			IEEE80211_VHT_MCS_NOT_SUPPORTED  << 2  |  \
			IEEE80211_VHT_MCS_NOT_SUPPORTED  << 4  |  \
			IEEE80211_VHT_MCS_NOT_SUPPORTED  << 6  |  \
			IEEE80211_VHT_MCS_NOT_SUPPORTED  << 8  |  \
			IEEE80211_VHT_MCS_NOT_SUPPORTED  << 10 |  \
			IEEE80211_VHT_MCS_NOT_SUPPORTED  << 12 |  \
			IEEE80211_VHT_MCS_NOT_SUPPORTED  << 14),  \
		}                                                           \
	}

#define RWNX_HE_CAPABILITIES                                    \
	{                                                               \
		.has_he = false,                                            \
		.he_cap_elem = {                                            \
			.mac_cap_info[0] = 0,                                   \
			.mac_cap_info[1] = 0,                                   \
			.mac_cap_info[2] = 0,                                   \
			.mac_cap_info[3] = 0,                                   \
			.mac_cap_info[4] = 0,                                   \
			.mac_cap_info[5] = 0,                                   \
			.phy_cap_info[0] = 0,                                   \
			.phy_cap_info[1] = 0,                                   \
			.phy_cap_info[2] = 0,                                   \
			.phy_cap_info[3] = 0,                                   \
			.phy_cap_info[4] = 0,                                   \
			.phy_cap_info[5] = 0,                                   \
			.phy_cap_info[6] = 0,                                   \
			.phy_cap_info[7] = 0,                                   \
			.phy_cap_info[8] = 0,                                   \
			.phy_cap_info[9] = 0,                                   \
			.phy_cap_info[10] = 0,                                  \
		},                                                          \
		.he_mcs_nss_supp = {                                        \
			.rx_mcs_80 = cpu_to_le16(0xfffa),                       \
			.tx_mcs_80 = cpu_to_le16(0xfffa),                       \
			.rx_mcs_160 = cpu_to_le16(0xffff),                      \
			.tx_mcs_160 = cpu_to_le16(0xffff),                      \
			.rx_mcs_80p80 = cpu_to_le16(0xffff),                    \
			.tx_mcs_80p80 = cpu_to_le16(0xffff),                    \
		},                                                          \
		.ppe_thres = {0x08, 0x1c, 0x07},                            \
	}

#if CONFIG_WIFI6
static struct ieee80211_sband_iftype_data rwnx_he_capa = {
	//	.types_mask = BIT(NL80211_IFTYPE_STATION),
	.he_cap = RWNX_HE_CAPABILITIES,
};
#endif

static struct ieee80211_supported_band rwnx_band_2GHz = {
	.channels   = rw_2ghz_channels,
	.n_channels = ARRAY_SIZE(rw_2ghz_channels),
	//	.bitrates   = rwnx_ratetable,
	//	.n_bitrates = ARRAY_SIZE(rwnx_ratetable),
	.ht_cap     = RWNX_HT_CAPABILITIES,
#if CONFIG_WIFI6
	.iftype_data = &rwnx_he_capa,
	.n_iftype_data = 1,
#else
	.iftype_data = NULL,
	.n_iftype_data = 0,
#endif
};

static struct ieee80211_supported_band rwnx_band_5GHz = {
#if CONFIG_WIFI_BAND_5G
	.channels   = rw_5ghz_channels,
	.n_channels = ARRAY_SIZE(rw_5ghz_channels),
#else
	.channels	= NULL,
	.n_channels = 0,
#endif
	//	.bitrates   = &rwnx_ratetable[4],
	//	.n_bitrates = ARRAY_SIZE(rwnx_ratetable) - 4,
	.ht_cap     = RWNX_HT_CAPABILITIES,
	.vht_cap    = RWNX_VHT_CAPABILITIES,
#if CONFIG_WIFI6
	.iftype_data = &rwnx_he_capa,
	.n_iftype_data = 1,
#else
	.iftype_data = NULL,
	.n_iftype_data = 0,
#endif
};

// TODO: run in workqueue, need some lock.
static void rwnx_csa_finish(void *arg)
{
 	struct rwnx_hw *rwnx_hw = (struct rwnx_hw *)arg;
	int ret;

	if (rwnx_hw->csa) {
		ret = rw_msg_send_bcn_change(rwnx_hw->csa);  // FIXME: handle Only for AP/P2P-GO
		if (ret)
			RWNX_LOGE("%s: csa finish failed\n", __func__);

		os_free(rwnx_hw->csa->bcn_ptr);
		os_free(rwnx_hw->csa);
		rwnx_hw->csa = 0;

		bk_wlan_ap_set_channel_config(rwnx_hw->freq_params.channel);

		// cfg80211_ch_switch_notify
		wpa_ctrl_event_copy(WPA_CTRL_EVENT_CHAN_SWITCH_IND, &rwnx_hw->freq_params,
				sizeof(rwnx_hw->freq_params));
	} else {
		RWNX_LOGI("CSA finish indication but no active CSA\n");
	}
}

extern void rwnxl_register_connector(RW_CONNECTOR_T *intf);

#if CONFIG_WIFI_BAND_5G && !CONFIG_WIFI_REGDOMAIN
static int rw_ieee80211_cc_use_default_chan(wifi_country_t *wifi_country)
{
	uint8_t i = 0;
	int ret = BK_FAIL;

	for (i = 0; i < sizeof(country_channel_table) / sizeof(Countrychannels); i++) {
		if (os_strncmp(wifi_country->cc, country_channel_table[i].countrycode, 3*sizeof(char)) == 0) {

			Countrychannels *chan_table = &country_channel_table[i];
			struct ieee80211_channel *channels = (struct ieee80211_channel *)rw_5ghz_channels;
			uint8_t num_chans = ARRAY_SIZE(rw_5ghz_channels);
			uint8_t triplet_cnt = 0;
			uint8_t current_chan_idx = 0;
			bool found_match = false;
	
			// Iterate through all 5GHz channels supported by this country
			for (uint8_t j = 0; j < chan_table->num_channels; j++) {
				// When need to start a new channel segment
				if (j == 0 || !found_match) {
					// Record the start channel of the new segment
					if (triplet_cnt < WIFI_CHANNEL_NUM_5G) {
						wifi_country->schan_5g[triplet_cnt] = chan_table->channels_5g[j];
					} else {
						break; // Maximum number of segments reached
					}
	
					// Find the matching position in rw_5ghz_channels for current channel
					found_match = false;
					for (uint8_t k = 0; k < num_chans; k++) {
						if (chan_table->channels_5g[j] == channels[k].hw_value) {
							current_chan_idx = k;
							found_match = true;
							//RWNX_LOGW("found %d,k %d,chan %d, %d\r\n", found_match, k, chan_table->channels_5g[j], channels[k].hw_value);
							break;
						}
					}
	
					// If no matching channel found, continue to next channel
					if (!found_match) {
						RWNX_LOGW("no match j %d, %d\r\n", j, chan_table->channels_5g[j]);
						continue;
					}
	
					// Count consecutive matching channels
					uint8_t match_count = 1;
					for (uint8_t k = j + 1; k < chan_table->num_channels; k++) {
						// Check if next position exceeds rw_5ghz_channels range
						if ((current_chan_idx + (k - j)) >= num_chans) {
							break;
						}
	
						// Check if channels match consecutively
						if (chan_table->channels_5g[k] == channels[current_chan_idx + (k - j)].hw_value) {
							match_count++;
						} else {
							found_match = false;
							break;
						}
					}
	
					// Save the number of channels for this segment
					if (triplet_cnt < WIFI_CHANNEL_NUM_5G) {
						wifi_country->nchan_5g[triplet_cnt] = match_count;
						wifi_country->max_tx_power_5g[triplet_cnt] = 0;
						triplet_cnt++;
					}
	
					// Skip already processed consecutive channels
					j += (match_count - 1);
					//RWNX_LOGW("j %d, match %d,triplet_cnt %d\r\n", j, match_count, triplet_cnt);
				}
			}

			g_country_code.chan_5g_cnt = chan_table->num_channels;
			for(i = 0; i < chan_table->num_channels; i++) {
				g_country_code.chan_5g_list[i] = chan_table->channels_5g[i];
			}

			// Set segment count and return status
			wifi_country->triplet_cnt = triplet_cnt;
			ret = BK_OK;
			break;
		}
	}

	return ret;
}
#endif


UINT32 rw_ieee80211_init(void)
{
	struct rwnx_hw *rwnx_hw;
	int i;
	RW_CONNECTOR_T intf = {0, };
	struct wiphy *wiphy;

	wiphy = &g_wiphy;
	memset(wiphy, 0, sizeof(*wiphy));

	rwnx_hw = &g_rwnx_hw;
	memset(rwnx_hw, 0, sizeof(*rwnx_hw));

	rwnx_hw->txq = os_zalloc(NX_NB_TXQ*sizeof(struct rwnx_txq));
	if(rwnx_hw->txq == NULL){
		BK_ASSERT(0);
	}
	else{
		memset(rwnx_hw->txq, 0, (NX_NB_TXQ*sizeof(struct rwnx_txq)));
	}

	rwnx_hw->wiphy = wiphy;
	rwnx_hw->mod_params = &rwnx_mod_params;

	rwnx_hw->mod_params->bfmee = false;

	if (ate_is_enabled())
		rwnx_hw->mod_params->use_2040 = true;

	rwnx_hw->vif_started = 0;
	rwnx_hw->monitor_vif = RWNX_INVALID_VIF;
	rwnx_hw->adding_sta = false;

	for (i = 0; i < NX_VIRT_DEV_MAX + NX_REMOTE_STA_MAX; i++)
		rwnx_hw->avail_idx_map |= BIT(i);

#if CONFIG_RWNX_SW_TXQ
	rwnx_hwq_init();
	rwnx_txq_prepare();
#endif

#if CONFIG_RWNX_TD
	rwnx_td_init();
#endif

	bk_work_init(&rwnx_hw->csa_work, rwnx_csa_finish, rwnx_hw);

	wiphy->bands[IEEE80211_BAND_2GHZ] = &rwnx_band_2GHz;
	wiphy->bands[IEEE80211_BAND_5GHZ] = &rwnx_band_5GHz;
	wiphy->reg_notifier = rwnx_reg_notifier;

	intf.msg_outbound_func = mr_kmsg_fwd;
	intf.data_outbound_func = rwm_upload_data;
	intf.rx_alloc_func = rwm_get_rx_free_node;
	intf.monitor_outbound_func = rwm_rx_monitor;

	rwnxl_register_connector(&intf);

#if CONFIG_WIFI_REGDOMAIN
	//	wiphy->regulatory_flags = REGULATORY_WIPHY_SELF_MANAGED;
	//	bk_wifi_set_country_code(CONFIG_DEFAULT_COUNTRY_CODE);
	struct ieee80211_supported_band *sband;
	wifi_band_t band;

	/* sanity check supported bands/channels */
	for (band = 0; band < IEEE80211_NUM_BANDS; band++) {
		sband = wiphy->bands[band];
		if (!sband)
			continue;

		sband->band = band;
		if (WARN_ON(!sband->n_channels))
			return -EINVAL;

		for (i = 0; i < sband->n_channels; i++) {
			sband->channels[i].orig_flags =
				sband->channels[i].flags;
			sband->channels[i].orig_mag = INT_MAX;
			sband->channels[i].orig_mpwr =
				sband->channels[i].max_power;
			sband->channels[i].band = band;

			if (WARN_ON(sband->channels[i].freq_offset >= 1000))
				return -EINVAL;
		}
	}
#else // !CONFIG_WIFI_REGDOMAIN
	/* init country code */
	g_country_code.cfg.cc[0] = 'C';
	g_country_code.cfg.cc[1] = 'N';
	g_country_code.cfg.cc[2] = 0;
	g_country_code.cfg.schan = 1;
	g_country_code.cfg.nchan = 13;
	g_country_code.cfg.max_tx_power = 0;
#if CONFIG_WIFI_BAND_5G  // FIXME: BK7239, temporarily code for 5G because country code is not suitable for 5G/6G
	rw_ieee80211_cc_use_default_chan(&g_country_code.cfg);
	g_country_code.policy = WIFI_COUNTRY_POLICY_AUTO;
	#if 0
	RWNX_LOGI("Set country code: cc=%s, policy=%s, 2.4G_chan=<%d-%d>, num=%d\r\n", 
		g_country_code.cfg.cc, (g_country_code.policy == WIFI_COUNTRY_POLICY_MANUAL ? "manual" : "auto"),
		g_country_code.cfg.schan, (g_country_code.cfg.schan + g_country_code.cfg.nchan - 1), g_country_code.cfg.nchan);

	for (i = 0; i < g_country_code.cfg.triplet_cnt; i++) {
		RWNX_LOGI("5G country code:<s %d,n %d>\r\n", g_country_code.cfg.schan_5g[i], g_country_code.cfg.nchan_5g[i]);
	}
	for (i = 0; i < g_country_code.chan_5g_cnt; i++) {
		RWNX_LOGI_RAW(" %d ", g_country_code.chan_5g_list[i]);
	}
	RWNX_LOGI_RAW("\r\n");
	#endif
#else
#if CONFIG_WIFI_AUTO_COUNTRY_CODE
	g_country_code.policy = WIFI_COUNTRY_POLICY_AUTO;
#else
	g_country_code.policy = WIFI_COUNTRY_POLICY_MANUAL;
#endif
#endif

	g_country_code.init = 1;
#endif // CONFIG_WIFI_REGDOMAIN

	return 0;
}

/* FIXME: Use with caution */
void rwnx_hw_reinit(void)
{
	struct rwnx_hw *rwnx_hw;

	rwnx_hw = &g_rwnx_hw;
	struct rwnx_txq *txq_temp = rwnx_hw->txq;

	memset(rwnx_hw, 0, sizeof(*rwnx_hw));

	if(txq_temp == NULL){
		rwnx_hw->txq = os_zalloc(NX_NB_TXQ*sizeof(struct rwnx_txq));
		if(rwnx_hw->txq == NULL){
			BK_ASSERT(0);
		}
	}
	else{
		rwnx_hw->txq = txq_temp;
	}

	memset(rwnx_hw->txq, 0, (NX_NB_TXQ*sizeof(struct rwnx_txq)));

}

#if CONFIG_POWER_TABLE
static UINT32 rw_ieee80211_set_chan_power(void)
{
	uint8_t chan_idx;
	uint16_t txpwr_11b = 0;
    uint16_t txpwr_11g = 0;
    uint16_t txpwr_ht = 0;
    
    UINT32 i;
    PW_LMT_REGULATION_TYPE regulation = PW_LMT_REGU_NULL;

	if (g_country_code.init == 0)
		return kNotInitializedErr;

	if (g_country_code.policy == WIFI_COUNTRY_POLICY_AUTO)
		return kNoErr;

    for(i=0; i < bk_get_country_regulation_table_size(); i++)
    {
        if(os_strcmp(g_country_code.cfg.cc,country_regulation_table[i].countrycode) == 0)
            {
                regulation = country_regulation_table[i].regulation;
                break;
            }
    }

	if (g_country_code.cfg.max_tx_power < 0) {
		// disable this channel
		txpwr_11b = 0;
        txpwr_11g = 0;
        txpwr_ht = 0;
        
	} else if (g_country_code.cfg.max_tx_power == 0) {
		// use default
		txpwr_11b = 30;
        txpwr_11g = 30;
        txpwr_ht = 30;
        
	}

	for (chan_idx = 1; chan_idx <= 14; chan_idx++) {
		if (rw_ieee80211_is_scan_rst_in_countrycode(chan_idx) == 1)
        {
            if(PW_LMT_REGU_NULL != regulation)
            {
                txpwr_11b = bk_get_txpwr_lmt_tbl_entry_power(chan_idx, regulation,0)/2;
                txpwr_11g = bk_get_txpwr_lmt_tbl_entry_power(chan_idx, regulation,1)/2;
                txpwr_ht = bk_get_txpwr_lmt_tbl_entry_power(chan_idx, regulation,2)/2;
                
            }
            else
            {
                txpwr_11b = 30;   
                txpwr_11g = 30;   
                txpwr_ht = 30;   
                
            }
            pwr_tbl_set_chan_pwr(chan_idx, txpwr_11b*10, txpwr_11g*10, txpwr_ht*10, txpwr_ht*10);  ///unit  0.1dbm
        }
		else {
			// disable this channel
			pwr_tbl_set_chan_pwr(chan_idx, 0, 0, 0, 0);
		}
	}
    manual_cal_set_cc_backoff_flag(true);
	RWNX_LOGI("set regulation %d chan maxpower:11b=%ddbm,11g=%ddbm,ht20=%ddbm\r\n", regulation,txpwr_11b,txpwr_11g,txpwr_ht);

	return kNoErr;
}
#endif

#if !CONFIG_WIFI_REGDOMAIN
static int country_validate(const wifi_country_t *country)
{
#if !CONFIG_WIFI_BAND_5G
	// 2.4G single band validation
	int min_chan = country->schan;
	int max_chan = country->schan + country->nchan - 1;

	// Check channel range and count
	if ((min_chan < WIFI_MIN_CHAN_NUM) || (max_chan > WIFI_MAX_CHAN_NUM) ||
		(country->nchan <= 0)) {
		RWNX_LOGW("country_validate: invalid 2.4G channel range: schan=%d, nchan=%d\r\n", 
			country->schan, country->nchan);
		return BK_ERR_WIFI_CHAN_RANGE;
	}
#else
	// 2.4G and 5G dual band validation
	uint8_t i = 0,j = 0;
	bool country_found = false;

	if((0 == country->nchan) && (0 == country->triplet_cnt)) {
		RWNX_LOGW("country_validate: not ser chan\r\n", 
		country->schan, country->nchan);
		return BK_ERR_WIFI_CHAN_RANGE;
	}

	// 2.4G part validation
	if (country->nchan != 0) {
		int min_chan = country->schan;
		int max_chan = country->schan + country->nchan - 1;

		// Check channel range and count
		if ((min_chan < WIFI_MIN_CHAN_NUM) || (max_chan > WIFI_MAX_CHAN_NUM) ||
			(country->nchan < 0)) {
			RWNX_LOGW("country_validate: invalid 2.4G channel range: schan=%d, nchan=%d\r\n", 
				country->schan, country->nchan);
			return BK_ERR_WIFI_CHAN_RANGE;
		}

		// Check if country code supports this channel
		for (i = 0; i < sizeof(country_24g_channel_table)/sizeof(Country24gchannels); i++) {
			if (os_strncmp(country->cc, country_24g_channel_table[i].countrycode, 3*sizeof(char)) == 0) {
				country_found = true;
				Country24gchannels *chan_table = &country_24g_channel_table[i];
				if ((max_chan > chan_table->channels[chan_table->num_channels - 1])
					|| (min_chan < chan_table->channels[0])){
					RWNX_LOGW("country_validate: country %s not support 2.4G channel max %d,min %d(supp: max %d, min %d)\r\n",
						country->cc, max_chan,min_chan, chan_table->channels[chan_table->num_channels - 1],chan_table->channels[0]);
					return BK_ERR_WIFI_CHAN_RANGE;
				}
				break;
			}
		}
	}

	// 5G part validation
	// Add check to prevent array overflow
	if (country->triplet_cnt > MAC_DOMAINCHANNEL_5G_MAX) {
		RWNX_LOGW("country_validate: triplet_cnt %d exceeds maximum supported %d\r\n",
			country->triplet_cnt, MAC_DOMAINCHANNEL_5G_MAX);
		return BK_ERR_WIFI_CHAN_RANGE;
	}

	// Check if country code supports this 5G channel
	Countrychannels *country_5g_chan_table = NULL;
	for (i= 0; i < sizeof(country_channel_table)/sizeof(Countrychannels); i++) {
		if (os_strncmp(country->cc, country_channel_table[i].countrycode, 3*sizeof(char)) == 0) {
			country_found = true;
			country_5g_chan_table = &country_channel_table[i];
			break;
		}
	}

	if ((country->triplet_cnt > 0) && (country_5g_chan_table == NULL)) {
		RWNX_LOGW("country_validate: no 5G table for country %s\r\n", country->cc);
		return BK_ERR_PARAM;
	}

	if(NULL != country_5g_chan_table)
	{
		uint8_t chan_5g_list[MAC_DOMAINCHANNEL_5G_MAX];
		uint8_t chan_5g_list_index = 0;
		struct ieee80211_channel *channel = rw_5ghz_channels;
		int num_chans = ARRAY_SIZE(rw_5ghz_channels);

		for (i = 0; i < country->triplet_cnt; i++) {
			int min_chan = country->schan_5g[i];
			int start_idx = -1;
		
			for (j = 0; j < num_chans; j++) {
				if (channel[j].hw_value == min_chan) { 
					start_idx = j; 
					break;
				}
			}

			if (start_idx < 0) {
				RWNX_LOGW("country_validate: 5G schan %d not found\r\n", min_chan);
				return BK_ERR_WIFI_CHAN_RANGE;
			}
		
			for (uint8_t k = 0; k < country->nchan_5g[i]; k++) {
				if (((start_idx + k) >= num_chans) || (chan_5g_list_index >= MAC_DOMAINCHANNEL_5G_MAX)) {
					RWNX_LOGW("country_validate: invalid 5G sequence for schan=%d, n=%d\r\n",
							  min_chan, country->nchan_5g[i]);
					return BK_ERR_WIFI_CHAN_RANGE;
				}
				chan_5g_list[chan_5g_list_index++] = channel[start_idx + k].hw_value;
			}
		}

		for(i = 0; i < chan_5g_list_index; i++) {
			if(chan_5g_list[i] < WIFI_5G_MIN_CHAN_NUM || chan_5g_list[i] > WIFI_5G_MAX_CHAN_NUM) {
				RWNX_LOGW("country_validate: invalid 5G channel:%d\r\n", chan_5g_list[i]);
				return BK_ERR_WIFI_CHAN_RANGE;
			}

			for(j = 0; j < country_5g_chan_table->num_channels; j++) {
				if(chan_5g_list[i] == country_5g_chan_table->channels_5g[j]) {
					break;
				}
			}

			if(j == country_5g_chan_table->num_channels) {
				RWNX_LOGW("country_validate: invalid 5G channel:%d\r\n", chan_5g_list[i]);
				return BK_ERR_WIFI_CHAN_RANGE;
			}
		}

		os_memcpy(g_country_code.chan_5g_list, chan_5g_list, chan_5g_list_index);
		g_country_code.chan_5g_cnt = chan_5g_list_index;
	}
	
	// Ensure country code is supported
	if (!country_found) {
		RWNX_LOGW("country_validate: unsupported country code: %s\r\n", country->cc);
		return BK_ERR_PARAM;
	}

#endif

	return BK_OK;
}
#endif // !CONFIG_WIFI_REGDOMAIN

/**
 * @brief Set the country code and regulatory domain configuration
 * 
 * This function sets the country code and configures the wireless settings
 * based on the regulatory domain. If the provided country information is
 * invalid, it will try to automatically configure based on the country code.
 * 
 * @param country Pointer to wifi_country_t structure containing country information
 * @return BK_OK on success, error code on failure
 */
int rw_ieee80211_set_country(const wifi_country_t *country)
{
	// Basic parameter check
	if (!country) {
		RWNX_LOGW("set_country: NULL parameter\r\n");
		return BK_ERR_NULL_PARAM;
	}

	// Validate country code format (should be 2-character string)
	if (!country->cc) {
		RWNX_LOGW("set_country: cc is null\r\n");
		return BK_ERR_NULL_PARAM;
	}
#if CONFIG_WIFI_REGDOMAIN
	char alpha2[4] = {0};

	if (!country)
		return BK_ERR_NULL_PARAM;

	alpha2[0] = country->cc[0];
	alpha2[1] = country->cc[1];

	return bk_wifi_set_country_code(alpha2);
#else // !CONFIG_WIFI_REGDOMAIN

	// Check if country code system is initialized
	if (g_country_code.init == 0) {
		RWNX_LOGW("Country code not init %d\r\n", g_country_code.init);
		return BK_ERR_WIFI_NOT_INIT;
	}

	int ret = country_validate(country);
	uint32 i;
	wifi_country_t wifi_country;

	// Copy the input country configuration
	os_memcpy(&wifi_country, country, sizeof(wifi_country_t));

	// Handle the case where user might only set country code (cc)
	// and other parameters need to be selected automatically
	if (ret != BK_OK) {
		RWNX_LOGD("country_validate:ret %d, cc=%s,schan=%d,nchan=%d,max_tx_power=%d\r\n",
			ret, country->cc, country->schan, country->nchan, country->max_tx_power);

		// Configure 2.4GHz band settings if needed
		#if CONFIG_WIFI_BAND_5G
		if((country->nchan == 0) && (country->triplet_cnt == 0))
		#endif
		{
			// Set 2.4GHz channels based on country code if start channel and channel count are not provided
			if ((country->schan == 0) && (country->nchan == 0)) {
				// Lookup country-specific 2.4GHz channels
				for (i = 0; i < sizeof(country_24g_channel_table) / sizeof(Country24gchannels); i++) {
					if (os_strncmp(country->cc, country_24g_channel_table[i].countrycode, 3*sizeof(char)) == 0) {
						wifi_country.nchan = country_24g_channel_table[i].num_channels;
						wifi_country.schan = country_24g_channel_table[i].channels[0];
						ret = BK_OK;
						break;
					}
				}
			}

			// Set 5GHz channels based on country code if start channel and channel count are not provided
			#if CONFIG_WIFI_BAND_5G 
			if(BK_OK == rw_ieee80211_cc_use_default_chan(&wifi_country))
				ret = BK_OK;
			#endif
		}
	}

	// If configuration is valid, apply the settings
	if (ret == BK_OK) {
		// Update global country code configuration
		os_memcpy(&g_country_code.cfg, &wifi_country, sizeof(wifi_country_t));
		g_country_code.policy = WIFI_COUNTRY_POLICY_MANUAL;

		// Log the configured country settings
		RWNX_LOGI("Set country code: cc=%s, policy=%s, 2.4G_chan=<%d-%d>, num=%d\r\n", 
			g_country_code.cfg.cc, (g_country_code.policy == WIFI_COUNTRY_POLICY_MANUAL ? "manual" : "auto"),
			g_country_code.cfg.schan, (g_country_code.cfg.schan + g_country_code.cfg.nchan - 1), g_country_code.cfg.nchan);

		// Log 5GHz channel information if supported
		#if CONFIG_WIFI_BAND_5G
		for (i = 0; i < g_country_code.cfg.triplet_cnt; i++) {
			RWNX_LOGI("5G country code:<s %d,n %d>\r\n", g_country_code.cfg.schan_5g[i], g_country_code.cfg.nchan_5g[i]);
		}

		for (i = 0; i < g_country_code.chan_5g_cnt; i++) {
			RWNX_LOGI_RAW(" %d ", g_country_code.chan_5g_list[i]);
		}
		RWNX_LOGI_RAW("\r\n");
		#endif
		
		#if CONFIG_POWER_TABLE
		// Set channel power based on country regulations if power table is enabled
		rw_ieee80211_set_chan_power();
		#endif
	}
	else {
		RWNX_LOGW("Failed to set country code: %d\r\n", ret);
	}
	return ret;
#endif // !CONFIG_WIFI_REGDOMAIN
}

int rw_ieee80211_get_country(wifi_country_t *country)
{
#if !CONFIG_WIFI_REGDOMAIN
	if (country) {
		if (g_country_code.init == 0)
			return BK_ERR_WIFI_NOT_INIT;

		os_memcpy(country, &g_country_code.cfg, sizeof(wifi_country_t));
		return BK_OK;
	}
#endif
	return BK_ERR_PARAM;
}

UINT32 rw_ieee80211_get_centre_frequency(UINT32 chan_id)
{
	UINT32 freq = 0;
	struct ieee80211_channel *channels = NULL;

	// FIXME: BK7239 6E, 6E channel will start with channel 1
	if ((chan_id >= 1) && (chan_id <= 14)) {
		channels = (struct ieee80211_channel *)rw_2ghz_channels;
		freq = channels[chan_id - 1].center_freq;
	} else {
		int num_chans = ARRAY_SIZE(rw_5ghz_channels);
		channels = (struct ieee80211_channel *)rw_5ghz_channels;

		for (int i = 0; i < num_chans; i++) {
			if (channels[i].hw_value == chan_id) {
				freq = channels[i].center_freq;
			}
		}
	}

	if (freq != 0) {
		return freq;
	} else {
		RWNX_LOGI("centre freq is 0, chan_id %d\n", chan_id);
		return 0;
	}
}

struct ieee80211_channel *ieee80211_get_channel(struct wiphy *wiphy, int freq)
{
	wifi_band_t band;
	struct ieee80211_supported_band *sband;
	int i;

	for (band = 0; band < IEEE80211_NUM_BANDS; band++) {
		sband = wiphy->bands[band];

		if (!sband)
			continue;

		for (i = 0; i < sband->n_channels; i++) {
			if (sband->channels[i].center_freq == freq)
				return &sband->channels[i];
		}
	}

	return NULL;
}

UINT8 rw_ieee80211_get_chan_id(UINT32 freq)
{
	int i;
	struct ieee80211_channel *channels = NULL;
	int num_chans = 0;

	if (freq >= 5925) {
		// FIXME: BK7239 6E
	} else if (freq >= 4900) {
		channels = (struct ieee80211_channel *)rw_5ghz_channels;
		num_chans = ARRAY_SIZE(rw_5ghz_channels);
	} else {
		channels = (struct ieee80211_channel *)rw_2ghz_channels;
		num_chans = ARRAY_SIZE(rw_2ghz_channels);
	}

	for (i = 0; i < num_chans; i++) {
		if (channels[i].center_freq == freq)
			return channels[i].hw_value;
	}


	return 0;
}

bool rw_ieee80211_5g_channel_supported(uint8_t chan)
{
	struct ieee80211_channel *channels = rw_5ghz_channels;;
	int num_chans = ARRAY_SIZE(rw_5ghz_channels);;
	for (int i = 0; i < num_chans; i++) {
		if (channels[i].hw_value == chan)
			return true;
	}

	return false;
}

#if CONFIG_WIFI_BAND_5G
#if CONFIG_WIFI_REGDOMAIN
bool check_non_radar_channel_available(int chan)
{
	struct wiphy *wiphy = &g_wiphy;
	struct ieee80211_channel *channel;
	int num_chan;
	int i;

	// Iterate all bands and channels
	for (int band = IEEE80211_BAND_2GHZ; band < IEEE80211_NUM_BANDS; band++) {
		if (!wiphy->bands[band])
			continue;
		num_chan = wiphy->bands[band]->n_channels;
		channel = wiphy->bands[band]->channels;
		for (i = 0; i < num_chan; i++, channel++) {
			// For ATE, assume channel is available
			if ((channel->flags & IEEE80211_CHAN_DISABLED) && !ate_is_enabled())
				continue;

			// Found channel, check RADAR flag
			if (channel->hw_value == chan) {
				if (channel->flags & IEEE80211_CHAN_RADAR)
					return false;
				return true;
			}
		}
	}

	return false;
}
#else
/* select 5g non radar avaliable channels list according to country code*/
int* rw_select_5g_non_radar_avaliable_channels(int *selected_channel_size)
{
	int *selected_non_radar_avaliable_channels = NULL;
	for (int i = 0; i < sizeof(non_radar_avaliable_channel_table) / sizeof(non_radar_avaliable_channel_table[0]); i++) {
		if ((non_radar_avaliable_channel_table[i].countrycode[0] == g_country_code.cfg.cc[0]) &&
			(non_radar_avaliable_channel_table[i].countrycode[1] == g_country_code.cfg.cc[1]) &&
			(non_radar_avaliable_channel_table[i].countrycode[2] == g_country_code.cfg.cc[2])) {
			*selected_channel_size = non_radar_avaliable_channel_table[i].num_channels;
			selected_non_radar_avaliable_channels = non_radar_avaliable_channel_table[i].channels_5g;
		}
	}
	return selected_non_radar_avaliable_channels;
}

/**
 * @brief Select 5GHz channels list according to country code and regulatory domain
 * 
 * This function selects 5GHz channels based on the current country code configuration.
 * It supports both auto and manual country code policies.
 * In auto mode, all available 5GHz channels are selected. In manual mode,
 * channels are selected according to the configured country-specific triplets.
 * 
 * @param selected_channels_size Pointer to store the number of selected channels
 * @return Pointer to dynamically allocated array of selected channel numbers,
 *         or NULL if allocation fails
 * @note The caller is responsible for freeing the returned array
 */
int* rw_select_5g_channels_by_regulatory_domain(int *selected_channels_size)
{
	int *selected_channels_5g = NULL;
	
	// Handle auto country code policy
	if(WIFI_COUNTRY_POLICY_AUTO == g_country_code.policy) {
		// Get the total number of available 5GHz channels
		*selected_channels_size = g_wiphy.bands[IEEE80211_BAND_5GHZ]->n_channels;
		
		// Allocate memory for channel array
		selected_channels_5g = (int*)os_malloc(sizeof(int) * (*selected_channels_size));
		// Check memory allocation
		if( NULL == selected_channels_5g) {
			RWNX_LOGI("malloc fail\r\n");
			BK_ASSERT(false);
		}

		// Copy all available 5GHz channel values if allocation succeeds
		if (selected_channels_5g) {
			struct ieee80211_channel *channel = rw_5ghz_channels;
			for (int i = 0; i < (*selected_channels_size); i++) {
				selected_channels_5g[i] = channel[i].hw_value;
			}
		}
	}
	// Handle manual country code policy
	else {
		// Calculate total number of channels across all triplets
		*selected_channels_size = g_country_code.chan_5g_cnt;
		selected_channels_5g = (int*)os_malloc(sizeof(int) * (*selected_channels_size));
		if( NULL == selected_channels_5g) {
			RWNX_LOGI("malloc fail\r\n");
			BK_ASSERT(false);
		}

		for (int i = 0; i < (*selected_channels_size); i++) {
			selected_channels_5g[i] = g_country_code.chan_5g_list[i];
		}
	}

	// Return pointer to the dynamically allocated channel array
	return selected_channels_5g;
}
#endif
#endif

UINT8 rw_ieee80211_init_scan_chan(struct scanu_start_req *req)
{
#if CONFIG_WIFI_REGDOMAIN
	struct wiphy *wiphy = &g_wiphy;
	struct ieee80211_channel *channel;
	int num_chan;
	int cnt = 0;
	int i;

	// Iterate all bands and channels
	for (int band = IEEE80211_BAND_2GHZ; band < IEEE80211_NUM_BANDS; band++) {
		if (!wiphy->bands[band])
			continue;
		num_chan = wiphy->bands[band]->n_channels;
		channel = wiphy->bands[band]->channels;
		for (i = 0; i < num_chan; i++, channel++) {
			if ((channel->flags & IEEE80211_CHAN_DISABLED) && !ate_is_enabled()
				&& !rwnx_ieee80211_check_conn_instrument(&req->ssid[0], &req->bssid))
				continue;

			req->chan[cnt].band = channel->band;
			req->chan[cnt].freq = channel->center_freq;
			req->chan[cnt].tx_power = VIF_UNDEF_POWER;
			if(!(ate_is_enabled() || rwnx_ieee80211_check_conn_instrument(&req->ssid[0], &req->bssid)))
				req->chan[cnt].flags = get_chan_flags(channel->flags);
			else///ATE mode or conn instrument, set all channels to active scan
				req->chan[cnt].flags = 0;
			//os_printf("freq %d ,flags 0x%x,channel->flags 0x%x\r\n ",req->chan[cnt].freq,req->chan[cnt].flags,channel->flags);

			cnt++;
		}
	}

	req->chan_cnt = cnt;
#else // !CONFIG_WIFI_REGDOMAIN
	UINT32 i, start_chan, num_chan_2g, num_chan_5g = 0, num_chan_6g = 0;  // FIXME: bk7239 6E

	BK_ASSERT(g_country_code.init); /* ASSERT VERIFIED */

	if (g_country_code.policy == WIFI_COUNTRY_POLICY_MANUAL) {
		start_chan = g_country_code.cfg.schan;
		num_chan_2g = g_country_code.cfg.nchan;
	}
	else {
		// auto mode, sta need scan all channel
		struct ieee80211_channel *channel = rw_2ghz_channels;
		start_chan = channel[0].hw_value;
		num_chan_2g = g_wiphy.bands[IEEE80211_BAND_2GHZ]->n_channels;
	}

	for (i = 0; i < num_chan_2g; i ++) {
		req->chan[i].band = IEEE80211_BAND_2GHZ;
		req->chan[i].flags = 0;
		req->chan[i].freq = rw_ieee80211_get_centre_frequency(i + start_chan);
		req->chan[i].tx_power = VIF_UNDEF_POWER;

#if CONFIG_WIFI_AUTO_COUNTRY_CODE
		uint16_t freq = req->chan[i].freq;
		// If auto mode, disable 12, 13 active scan
		if ((g_country_code.policy == WIFI_COUNTRY_POLICY_AUTO) &&
			(freq == 2467 || freq == 2472 || freq == 2484)) {
			req->chan[i].flags |= CHAN_NO_IR;
			// os_printf("XXX disable IR for freq %d\n", freq);
		}
#else
		uint16_t freq = req->chan[i].freq;
		//disable 12, 13, 14 active scan
		if ((freq == 2467 || freq == 2472 || freq == 2484)) {
			req->chan[i].flags |= CHAN_NO_IR;
			 os_printf("2.4G disable IR for freq %d\n", freq);
		}
#endif // CONFIG_WIFI_AUTO_COUNTRY_CODE
	}

#if CONFIG_WIFI_BAND_5G
	if (ate_is_enabled()) {
		struct ieee80211_channel *channel = rw_5ghz_channels;
		for (i = 0; i < ARRAY_SIZE(rw_5ghz_channels); i++) {
			req->chan[i + num_chan_2g].band = IEEE80211_BAND_5GHZ;
			req->chan[i + num_chan_2g].flags = 0;
			req->chan[i + num_chan_2g].freq = channel[i].center_freq;

#if 0 ///ATE mode
			uint16_t freq = req->chan[i + num_chan_2g].freq;
			//disable 52/56/60/64 active scan
			if ((freq == 5260 || freq == 5280 || freq == 5300 || freq == 5320)) {
				req->chan[i + num_chan_2g].flags |= CHAN_NO_IR;
				// os_printf("disable IR for freq %d\n", freq);
			}
#endif
			BK_ASSERT(req->chan[i + num_chan_2g].freq);
		}
		num_chan_5g = g_wiphy.bands[IEEE80211_BAND_5GHZ]->n_channels;
	} else {
		int selected_channels_size = 0;
		int *selected_channels_5g = rw_select_5g_channels_by_regulatory_domain(&selected_channels_size);

		for (i = 0; i < selected_channels_size; i++) {
			req->chan[i + num_chan_2g].band = IEEE80211_BAND_5GHZ;
			req->chan[i + num_chan_2g].flags = 0;
			req->chan[i + num_chan_2g].freq = (5000 + (5 * selected_channels_5g[i]));
			req->chan[i + num_chan_2g].tx_power = VIF_UNDEF_POWER;

			uint16_t freq = req->chan[i + num_chan_2g].freq;
			//disable 52/56/60/64 active scan
			if ((freq == 5260 || freq == 5280 || freq == 5300 || freq == 5320)) {
				req->chan[i + num_chan_2g].flags |= CHAN_NO_IR;
				 os_printf("5G disable IR for freq %d\n", freq);
			}

			BK_ASSERT(req->chan[i + num_chan_2g].freq);
		}
		num_chan_5g = selected_channels_size;
		if(selected_channels_5g)
			os_free(selected_channels_5g);
	}
#endif // CONFIG_WIFI_BAND_5G
	req->chan_cnt = num_chan_2g + num_chan_5g + num_chan_6g;
#endif // CONFIG_WIFI_REGDOMAIN

	return 0;
}

UINT8 rw_ieee80211_init_scan_chan_band(struct scanu_start_req *req, UINT8 band)
{
#if CONFIG_WIFI_REGDOMAIN
	struct wiphy *wiphy = &g_wiphy;
	struct ieee80211_channel *channel;
	int num_chan;
	int cnt = 0;
	int i;

	// Iterate all bands and channels
	for (int band_idx = IEEE80211_BAND_2GHZ; band_idx < IEEE80211_NUM_BANDS; band_idx++) {
		if(band_idx != band)
			continue;
		if (!wiphy->bands[band_idx])
			continue;
		num_chan = wiphy->bands[band_idx]->n_channels;
		channel = wiphy->bands[band_idx]->channels;
		for (i = 0; i < num_chan; i++, channel++) {
			if ((channel->flags & IEEE80211_CHAN_DISABLED) && !ate_is_enabled()
				&& !rwnx_ieee80211_check_conn_instrument(&req->ssid[0], &req->bssid))
				continue;

			req->chan[cnt].band = channel->band;
			req->chan[cnt].freq = channel->center_freq;
			req->chan[cnt].tx_power = VIF_UNDEF_POWER;
			if(!(ate_is_enabled() || rwnx_ieee80211_check_conn_instrument(&req->ssid[0], &req->bssid)))
				req->chan[cnt].flags = get_chan_flags(channel->flags);
			else///ATE mode or conn instrument, set all channels to active scan
				req->chan[cnt].flags = 0;
			//os_printf("freq %d ,flags 0x%x\r\n ",req->chan[cnt].freq,req->chan[cnt].flags);
			cnt++;
		}
	}

	req->chan_cnt = cnt;

#else
	UINT8 i = 0;
	UINT8 start_chan = 0;
	UINT32 num_chan = 0;

	BK_ASSERT(g_country_code.init); /* ASSERT VERIFIED */

	if(IEEE80211_BAND_2GHZ == band)
	{
		if (g_country_code.policy == WIFI_COUNTRY_POLICY_MANUAL) {
			start_chan = g_country_code.cfg.schan;
			num_chan = g_country_code.cfg.nchan;
		}
		else {
			// auto mode, sta need scan all channel
			struct ieee80211_channel *channel = rw_2ghz_channels;
			start_chan = channel[0].hw_value;
			num_chan = g_wiphy.bands[IEEE80211_BAND_2GHZ]->n_channels;
		}

		for (i = 0; i < num_chan; i ++) {
			req->chan[i].band = IEEE80211_BAND_2GHZ;
			req->chan[i].flags = 0;
			req->chan[i].freq = rw_ieee80211_get_centre_frequency(i + start_chan);
			req->chan[i].tx_power = VIF_UNDEF_POWER;
	
			uint16_t freq = req->chan[i].freq;
			//disable 12, 13, 14 active scan
			if ((freq == 2467 || freq == 2472 || freq == 2484)) {
				req->chan[i].flags |= CHAN_NO_IR;
				 //os_printf("2.4G disable IR for freq %d\n", freq);
			}
		}
	}
	#if CONFIG_WIFI_BAND_5G
	else if(IEEE80211_BAND_5GHZ == band)
	{
		if (ate_is_enabled()) {
			struct ieee80211_channel *channel = rw_5ghz_channels;
			for (i = 0; i < ARRAY_SIZE(rw_5ghz_channels); i++) {
				req->chan[i].band = IEEE80211_BAND_5GHZ;
				req->chan[i].flags = 0;
				req->chan[i].freq = channel[i].center_freq;
	
				BK_ASSERT(req->chan[i].freq);
			}
			num_chan = g_wiphy.bands[IEEE80211_BAND_5GHZ]->n_channels;
			} else {
				int selected_channels_size = 0;
				int *selected_channels_5g = rw_select_5g_channels_by_regulatory_domain(&selected_channels_size);

				for (i = 0; i < selected_channels_size; i++) {
					req->chan[i].band = IEEE80211_BAND_5GHZ;
					req->chan[i].flags = 0;
					req->chan[i].freq = (5000 + (5 * selected_channels_5g[i]));
					req->chan[i].tx_power = VIF_UNDEF_POWER;

					uint16_t freq = req->chan[i].freq;
					//disable 52/56/60/64 active scan
					if ((freq == 5260 || freq == 5280 || freq == 5300 || freq == 5320)) {
						req->chan[i].flags |= CHAN_NO_IR;
						 //os_printf("5G disable IR for freq %d\n", freq);
					}

					BK_ASSERT(req->chan[i].freq);
				}
				num_chan = selected_channels_size;
				if(selected_channels_5g)
					os_free(selected_channels_5g);
			}
	}
	#endif

	req->chan_cnt = num_chan;
#endif

	return 0;
}

// FIXME: wifi 6E, add band as argument.
UINT8 rw_ieee80211_is_scan_rst_in_countrycode(uint8_t chan)
{
#if CONFIG_WIFI_REGDOMAIN
	struct wiphy *wiphy = &g_wiphy;
	struct ieee80211_channel *channel;
	int num_chan;
	int i;

	// Iterate all bands and channels
	for (int band = IEEE80211_BAND_2GHZ; band < IEEE80211_NUM_BANDS; band++) {
		if (!wiphy->bands[band])
			continue;
		num_chan = wiphy->bands[band]->n_channels;
		channel = wiphy->bands[band]->channels;
		for (i = 0; i < num_chan; i++, channel++) {
			// For ATE, assume channel is available
			if ((channel->flags & IEEE80211_CHAN_DISABLED) && !ate_is_enabled())
				continue;

			if (channel->hw_value == chan)
				return 1;
		}
	}

	return 0;
#else
	UINT32 start_chan, end_chan;

	BK_ASSERT(g_country_code.init); /* ASSERT VERIFIED */

	if (g_country_code.policy != WIFI_COUNTRY_POLICY_MANUAL) {
		// auto mode, sta need scan all channel, and get all rst
		return 1;
	}

	start_chan = g_country_code.cfg.schan;
	end_chan = (start_chan + g_country_code.cfg.nchan - 1);

	if (chan <= 14) {
		return (chan >= start_chan && chan <= end_chan);
	} else {
#if CONFIG_WIFI_BAND_5G
		// 5G // FIXME: 6E
		int selected_channels_size = 0;
		int *selected_channels_5g = rw_select_5g_channels_by_regulatory_domain(&selected_channels_size);
		for (int i = 0; i < selected_channels_size; i++) {
			if (chan == selected_channels_5g[i]) {
				if(selected_channels_5g)
					os_free(selected_channels_5g);
				return 1;
			}
		}

		if(selected_channels_5g)
			os_free(selected_channels_5g);
#endif
	}

	return 0;
#endif
}

UINT8 rw_ieee80211_get_scan_default_chan_num(void)
{
#if CONFIG_WIFI_REGDOMAIN
	struct wiphy *wiphy = &g_wiphy;
	struct ieee80211_channel *channel;
	int num_chan;
	int i;
	int cnt = 0;

	// Iterate all bands and channels
	for (int band = IEEE80211_BAND_2GHZ; band < IEEE80211_NUM_BANDS; band++) {
		if (!wiphy->bands[band])
			continue;
		num_chan = wiphy->bands[band]->n_channels;
		channel = wiphy->bands[band]->channels;
		for (i = 0; i < num_chan; i++, channel++) {
			// For ATE, assume channel is available
			if ((channel->flags & IEEE80211_CHAN_DISABLED) && !ate_is_enabled())
				continue;
			cnt++;
		}
	}

	return cnt;
#else
	UINT8 chan_num = 0;
	UINT8 num_chan_2g = 0;
	UINT8 num_chan_5g = 0;

	BK_ASSERT(g_country_code.init); /* ASSERT VERIFIED */

	if (g_country_code.policy == WIFI_COUNTRY_POLICY_MANUAL)
		num_chan_2g = g_country_code.cfg.nchan;
	else {
		// auto mode, sta need scan all channel
		num_chan_2g = g_wiphy.bands[IEEE80211_BAND_2GHZ]->n_channels;
	}

#if CONFIG_WIFI_BAND_5G
	int selected_channels_size = 0;
	int *selected_channels_5g = rw_select_5g_channels_by_regulatory_domain(&selected_channels_size);

	num_chan_5g = selected_channels_size;
	if(selected_channels_5g)
		os_free(selected_channels_5g);
#endif

	chan_num = num_chan_2g + num_chan_5g;

	return chan_num;
#endif
}

#if CONFIG_WIFI_AUTO_COUNTRY_CODE
bool country_code_policy_is_auto(void)
{
	return (g_country_code.policy == WIFI_COUNTRY_POLICY_AUTO);
}
#endif // CONFIG_WIFI_AUTO_COUNTRY_CODE

#if CONFIG_WIFI4
void rw_ieee80211_set_ht_cap(UINT8 ht_supp)
{
	g_wiphy.bands[IEEE80211_BAND_2GHZ]->ht_cap.ht_supported = ht_supp;
	rw_msg_send_me_config_req();
}
#endif

__IRAM2 inline uint64_t rwnx_hw_mm_features()
{
	return g_rwnx_hw.version_cfm.features;
}

u32 ieee80211_channel_to_freq_khz(int chan, wifi_band_t band)
{
	/* see 802.11 17.3.8.3.2 and Annex J
	 * there are overlapping channel numbers in 5GHz and 2GHz bands */
	if (chan <= 0)
		return 0; /* not supported */
	switch (band) {
	case IEEE80211_BAND_2GHZ:
		if (chan == 14)
			return MHZ_TO_KHZ(2484);
		else if (chan < 14)
			return MHZ_TO_KHZ(2407 + chan * 5);
		break;
	case IEEE80211_BAND_5GHZ:
		if (chan >= 182 && chan <= 196)
			return MHZ_TO_KHZ(4000 + chan * 5);
		else
			return MHZ_TO_KHZ(5000 + chan * 5);
		break;
	case IEEE80211_BAND_6GHZ:
		/* see 802.11ax D6.1 27.3.23.2 */
		if (chan == 2)
			return MHZ_TO_KHZ(5935);
		if (chan <= 233)
			return MHZ_TO_KHZ(5950 + chan * 5);
		break;
	case IEEE80211_BAND_60GHZ:
		if (chan < 7)
			return MHZ_TO_KHZ(56160 + chan * 2160);
		break;
#if 0
	case IEEE80211_BAND_S1GHZ:
		return 902000 + chan * 500;
#endif
	default:
		;
	}
	return 0; /* not supported */
}

int ieee80211_channel_to_frequency(int chan, wifi_band_t band)
{
	return KHZ_TO_MHZ(ieee80211_channel_to_freq_khz(chan, band));
}

int ieee80211_freq_khz_to_channel(u32 freq)
{
	/* TODO: just handle MHz for now */
	freq = KHZ_TO_MHZ(freq);

	/* see 802.11 17.3.8.3.2 and Annex J */
	if (freq == 2484)
		return 14;
	else if (freq < 2484)
		return (freq - 2407) / 5;
	else if (freq >= 4910 && freq <= 4980)
		return (freq - 4000) / 5;
	else if (freq < 5925)
		return (freq - 5000) / 5;
	else if (freq == 5935)
		return 2;
	else if (freq <= 45000) /* DMG band lower limit */
		/* see 802.11ax D6.1 27.3.22.2 */
		return (freq - 5950) / 5;
	else if (freq >= 58320 && freq <= 70200)
		return (freq - 56160) / 2160;
	else
		return 0;
}

/**
 * ieee80211_frequency_to_channel - convert frequency to channel number
 * @freq: center frequency in MHz
 * Return: The corresponding channel, or 0 if the conversion failed.
 */
int ieee80211_frequency_to_channel(int freq)
{
	return ieee80211_freq_khz_to_channel(MHZ_TO_KHZ(freq));
}

int get_wiphy_idx(struct wiphy *wiphy)
{
	return 1;
}

int rwnx_reg_notifier(struct wiphy *wiphy,
			    struct regulatory_request *request)
{
	float pwr;
	int regd_max_pwr = 0;

	// Iterates all bands
	for (wifi_band_t band = 0; band < IEEE80211_NUM_BANDS; band++) {
		struct ieee80211_supported_band *sband = wiphy->bands[band];
		if (!sband)
			continue;

		// Iterates all channels
		for (int i = 0; i < sband->n_channels; i++) {
			struct ieee80211_channel *channel = &sband->channels[i];

			// Skip disabled channel
			if (channel->flags & IEEE80211_CHAN_DISABLED)
				continue;

			// Since we doesn't support different channel use different tx pwr,
			// save max tx pwr here
			if (regd_max_pwr == 0)
				regd_max_pwr = channel->max_power;
			if (regd_max_pwr > channel->max_power)
				regd_max_pwr = channel->max_power;

			WIFI_LOGD("band %d, center %d, flags %s, max_power %d/%d, dfs region %d\n",
					  channel->band, channel->center_freq, regdom_flag_str(channel->flags),
					  channel->max_power, regd_max_pwr,
					  reg_get_dfs_region(&g_wiphy));

			for (wifi_standard std = WIFI_STANDARD_11A; std <= WIFI_STANDARD_11AX; std++) {
				if (manual_cal_get_tx_power(std, &pwr) == BK_OK) {
					WIFI_LOGD("current txpwr %f\n", pwr);
					if (pwr > regd_max_pwr) {
						if (manual_cal_set_tx_power(std, regd_max_pwr) != BK_OK)
							WIFI_LOGE("set regd txpwr fail, band %d, freq %d\n",
									  channel->band, channel->freq_offset);
					}
				}
			}
		}
	}

	// Reconfig mac channel
	if (rwm_mgmt_is_vif_first_used()) {
		rw_msg_send_me_chan_config_req();
	}

	return 0;
}

void rwnx_regulatory_hint_11d(int freq, const u8 *country_ie, u8 country_ie_len)
{
    wifi_band_t band = IEEE80211_BAND_2GHZ;
    if (freq >= 5925) {
        band = IEEE80211_BAND_6GHZ;
    } else if (freq >= 4900) {
        band = IEEE80211_BAND_5GHZ;
    } else if (freq >= 2400) {
        band = IEEE80211_BAND_2GHZ;
    }
	regulatory_hint_11d(&g_wiphy, band, country_ie, country_ie_len);
}

/// Instrument discernible ssid & bssid
bool rwnx_ieee80211_check_conn_instrument(const struct mac_ssid *ssid, const struct mac_addr *bssid)
{
    // check null pointer
    if (ssid == NULL && bssid == NULL) {
        return false;
    }

    UINT8 ssid_array[] = "CMW-AP";
    UINT8 ssid_length = sizeof(ssid_array) - 1;
    UINT8 ssid_array2[] = "P-DCTEST";
    UINT8 ssid_length2 = sizeof(ssid_array2) - 1;
    UINT16 bssid_addr1 = 0x0100; //"00:01:02:03:04:05"
    UINT16 bssid_addr2 = 0x0302;
    UINT16 bssid_addr3 = 0x0504;

    // check SSID is matched
    if (ssid && (ssid_length == ssid->length) && (!memcmp(ssid_array, ssid->array, ssid_length))) {
        return true;
    } else if (ssid && (ssid_length2 == ssid->length) && (!memcmp(ssid_array2, ssid->array, ssid_length2))) {
        return true;
    }

    // check BSSID is matched (two SSID use the same BSSID check logic)
    if (bssid && (bssid->array[0] == bssid_addr1) &&
        (bssid->array[1] == bssid_addr2) &&
        (bssid->array[2] == bssid_addr3)) {
        return true;
    }

    return false;
}
// eof

