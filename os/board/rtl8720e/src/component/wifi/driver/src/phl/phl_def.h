#ifndef _PHL_DEF_H_
#define _PHL_DEF_H_
/*
* structs defined in this file are shared with halbb or halrf
*/

#define MAX_BAND_NUM 1 /*DBCC use 2 bands*/

struct rtw_chan_def {
	enum band_type band; /* protocol -2.4G,5G,6G*/
	enum channel_width bw;
	enum chan_offset offset;/*used as offset of 40MHz, g6 driver may also used as offset of primary channel*/
	u8 chan; /*primary channel*/
	u8 center_ch;
};

enum rtw_edcca_mode_t {
	RTW_EDCCA_NORM	= 0, /* normal */
	RTW_EDCCA_ADAPT	= 1, /* adaptivity */
	RTW_EDCCA_CS	= 2, /* carrier sense */

	RTW_EDCCA_MODE_NUM,
	RTW_EDCCA_DEF		= RTW_EDCCA_MODE_NUM, /* default (ref to domain code), used at country chplan map's override field */
};

enum REGULATION {
	REGULATION_WW        =  0,
	REGULATION_ETSI      =  1,
	REGULATION_FCC       =  2,
	REGULATION_MKK       =  3,
	REGULATION_KCC       =  4,
	REGULATION_NCC       =  5,
	REGULATION_ACMA      =  6,
	REGULATION_NA        =  7,
	REGULATION_IC        =  8,
	REGULATION_CHILE     =  9,
	REGULATION_MEX       = 10,
	REGULATION_MAX       = 11,
};

enum rtw_drv_mode {
	RTW_DRV_MODE_NORMAL = 0,
	RTW_DRV_MODE_MP = 1,/*mp mode has to be 1 to be compatible with phydm code 'dm->mp_mode == 1'*/

	/* Fixed Max Value*/
	RTW_DRV_MODE_MAX = 255
};

/*==================phl_country.h of g6 driver============= ===*/
enum TP_OVERWRITE {
	TPO_CHILE = 0,
	TPO_QATAR = 1,
	TPO_UKRAINE = 2,
	TPO_CN = 3,
	TPO_NA = 4
};

struct rtw_regulation_info {
	u8 domain_code;			/* just for halrf print */
	u8 regulation_2g;		/* halrf use */
	u8 regulation_5g;		/* halrf use */
	u8 regulation_6g;			/* not support */
	u8 tpo;				/* halrf use, currently set NA, TODO */
	u8 chplan_ver;			/* just for halrf print */
	u8 country_ver;			/* just for halrf print */
};

/*==================rtw_general_def.h of g6 driver==============*/
#define RTW_PHL_MAX_RF_PATH 4

enum wlan_mode {
	WLAN_MD_INVALID = 0,
	WLAN_MD_11B	= BIT0,
	WLAN_MD_11A	= BIT1,
	WLAN_MD_11G	= BIT2,
	WLAN_MD_11N	= BIT3,
	WLAN_MD_11AC	= BIT4,
	WLAN_MD_11AX	= BIT5,

	/* Type for current wireless mode */
	WLAN_MD_11BG	= (WLAN_MD_11B | WLAN_MD_11G),
	WLAN_MD_11GN	= (WLAN_MD_11G | WLAN_MD_11N),
	WLAN_MD_11AN	= (WLAN_MD_11A | WLAN_MD_11N),
	WLAN_MD_11BN	= (WLAN_MD_11B | WLAN_MD_11N),
	WLAN_MD_11BGN	= (WLAN_MD_11B | WLAN_MD_11G | WLAN_MD_11N),
	WLAN_MD_11BGAC = (WLAN_MD_11B | WLAN_MD_11G | WLAN_MD_11AC),
	WLAN_MD_11BGAX = (WLAN_MD_11B | WLAN_MD_11G | WLAN_MD_11AX),
	WLAN_MD_11GAC  = (WLAN_MD_11G | WLAN_MD_11AC),
	WLAN_MD_11GAX  = (WLAN_MD_11G | WLAN_MD_11AX),
	WLAN_MD_11A_AC = (WLAN_MD_11A | WLAN_MD_11AC),
	WLAN_MD_11A_AX = (WLAN_MD_11A | WLAN_MD_11AX),

	/* Capability -Type for registry default wireless mode */
	WLAN_MD_11AGN	= (WLAN_MD_11A | WLAN_MD_11G | WLAN_MD_11N),
	WLAN_MD_11ABGN	= (WLAN_MD_11A | WLAN_MD_11B | WLAN_MD_11G | WLAN_MD_11N),
	WLAN_MD_11ANAC	= (WLAN_MD_11A | WLAN_MD_11N | WLAN_MD_11AC),
	WLAN_MD_11BGNAC = (WLAN_MD_11B | WLAN_MD_11G | WLAN_MD_11N | WLAN_MD_11AC),
	WLAN_MD_11GNAC  = (WLAN_MD_11G | WLAN_MD_11N | WLAN_MD_11AC),
	WLAN_MD_24G_MIX = (WLAN_MD_11B | WLAN_MD_11G | WLAN_MD_11N  | WLAN_MD_11AX),
	WLAN_MD_5G_MIX	= (WLAN_MD_11A | WLAN_MD_11N | WLAN_MD_11AC | WLAN_MD_11AX),
	WLAN_MD_6G_MIX 	= (WLAN_MD_11A | WLAN_MD_11AX),
	WLAN_MD_MAX	= (WLAN_MD_24G_MIX | WLAN_MD_5G_MIX),
};

enum rtw_gi_ltf {
	RTW_GILTF_LGI_4XHE32 = 0,
	RTW_GILTF_SGI_4XHE08 = 1,
	RTW_GILTF_2XHE16 = 2,
	RTW_GILTF_2XHE08 = 3,
	RTW_GILTF_1XHE16 = 4,
	RTW_GILTF_1XHE08 = 5,
	RTW_GILTF_MAX
};

enum rtw_data_rate {
	RTW_DATA_RATE_CCK1		= 0x0,
	RTW_DATA_RATE_CCK2		= 0x1,
	RTW_DATA_RATE_CCK5_5	= 0x2,
	RTW_DATA_RATE_CCK11		= 0x3,
	RTW_DATA_RATE_OFDM6		= 0x4,
	RTW_DATA_RATE_OFDM9		= 0x5,
	RTW_DATA_RATE_OFDM12	= 0x6,
	RTW_DATA_RATE_OFDM18	= 0x7,
	RTW_DATA_RATE_OFDM24	= 0x8,
	RTW_DATA_RATE_OFDM36	= 0x9,
	RTW_DATA_RATE_OFDM48	= 0xA,
	RTW_DATA_RATE_OFDM54	= 0xB,
	RTW_DATA_RATE_MCS0		= 0x80,
	RTW_DATA_RATE_MCS1		= 0x81,
	RTW_DATA_RATE_MCS2		= 0x82,
	RTW_DATA_RATE_MCS3		= 0x83,
	RTW_DATA_RATE_MCS4		= 0x84,
	RTW_DATA_RATE_MCS5		= 0x85,
	RTW_DATA_RATE_MCS6		= 0x86,
	RTW_DATA_RATE_MCS7		= 0x87,
	RTW_DATA_RATE_MCS8		= 0x88,
	RTW_DATA_RATE_MCS9		= 0x89,
	RTW_DATA_RATE_MCS10		= 0x8A,
	RTW_DATA_RATE_MCS11		= 0x8B,
	RTW_DATA_RATE_MCS12		= 0x8C,
	RTW_DATA_RATE_MCS13		= 0x8D,
	RTW_DATA_RATE_MCS14		= 0x8E,
	RTW_DATA_RATE_MCS15		= 0x8F,
	RTW_DATA_RATE_MCS16		= 0x90,
	RTW_DATA_RATE_MCS17		= 0x91,
	RTW_DATA_RATE_MCS18		= 0x92,
	RTW_DATA_RATE_MCS19		= 0x93,
	RTW_DATA_RATE_MCS20		= 0x94,
	RTW_DATA_RATE_MCS21		= 0x95,
	RTW_DATA_RATE_MCS22		= 0x96,
	RTW_DATA_RATE_MCS23		= 0x97,
	RTW_DATA_RATE_MCS24		= 0x98,
	RTW_DATA_RATE_MCS25		= 0x99,
	RTW_DATA_RATE_MCS26		= 0x9A,
	RTW_DATA_RATE_MCS27		= 0x9B,
	RTW_DATA_RATE_MCS28		= 0x9C,
	RTW_DATA_RATE_MCS29		= 0x9D,
	RTW_DATA_RATE_MCS30		= 0x9E,
	RTW_DATA_RATE_MCS31		= 0x9F,
	RTW_DATA_RATE_VHT_NSS1_MCS0	= 0x100,
	RTW_DATA_RATE_VHT_NSS1_MCS1	= 0x101,
	RTW_DATA_RATE_VHT_NSS1_MCS2	= 0x102,
	RTW_DATA_RATE_VHT_NSS1_MCS3	= 0x103,
	RTW_DATA_RATE_VHT_NSS1_MCS4	= 0x104,
	RTW_DATA_RATE_VHT_NSS1_MCS5	= 0x105,
	RTW_DATA_RATE_VHT_NSS1_MCS6	= 0x106,
	RTW_DATA_RATE_VHT_NSS1_MCS7	= 0x107,
	RTW_DATA_RATE_VHT_NSS1_MCS8	= 0x108,
	RTW_DATA_RATE_VHT_NSS1_MCS9	= 0x109,
	RTW_DATA_RATE_VHT_NSS2_MCS0	= 0x110,
	RTW_DATA_RATE_VHT_NSS2_MCS1	= 0x111,
	RTW_DATA_RATE_VHT_NSS2_MCS2	= 0x112,
	RTW_DATA_RATE_VHT_NSS2_MCS3	= 0x113,
	RTW_DATA_RATE_VHT_NSS2_MCS4	= 0x114,
	RTW_DATA_RATE_VHT_NSS2_MCS5	= 0x115,
	RTW_DATA_RATE_VHT_NSS2_MCS6	= 0x116,
	RTW_DATA_RATE_VHT_NSS2_MCS7	= 0x117,
	RTW_DATA_RATE_VHT_NSS2_MCS8	= 0x118,
	RTW_DATA_RATE_VHT_NSS2_MCS9	= 0x119,
	RTW_DATA_RATE_VHT_NSS3_MCS0	= 0x120,
	RTW_DATA_RATE_VHT_NSS3_MCS1	= 0x121,
	RTW_DATA_RATE_VHT_NSS3_MCS2	= 0x122,
	RTW_DATA_RATE_VHT_NSS3_MCS3	= 0x123,
	RTW_DATA_RATE_VHT_NSS3_MCS4	= 0x124,
	RTW_DATA_RATE_VHT_NSS3_MCS5	= 0x125,
	RTW_DATA_RATE_VHT_NSS3_MCS6	= 0x126,
	RTW_DATA_RATE_VHT_NSS3_MCS7	= 0x127,
	RTW_DATA_RATE_VHT_NSS3_MCS8	= 0x128,
	RTW_DATA_RATE_VHT_NSS3_MCS9	= 0x129,
	RTW_DATA_RATE_VHT_NSS4_MCS0	= 0x130,
	RTW_DATA_RATE_VHT_NSS4_MCS1	= 0x131,
	RTW_DATA_RATE_VHT_NSS4_MCS2	= 0x132,
	RTW_DATA_RATE_VHT_NSS4_MCS3	= 0x133,
	RTW_DATA_RATE_VHT_NSS4_MCS4	= 0x134,
	RTW_DATA_RATE_VHT_NSS4_MCS5	= 0x135,
	RTW_DATA_RATE_VHT_NSS4_MCS6	= 0x136,
	RTW_DATA_RATE_VHT_NSS4_MCS7	= 0x137,
	RTW_DATA_RATE_VHT_NSS4_MCS8	= 0x138,
	RTW_DATA_RATE_VHT_NSS4_MCS9	= 0x139,
	RTW_DATA_RATE_HE_NSS1_MCS0	= 0x180,
	RTW_DATA_RATE_HE_NSS1_MCS1	= 0x181,
	RTW_DATA_RATE_HE_NSS1_MCS2	= 0x182,
	RTW_DATA_RATE_HE_NSS1_MCS3	= 0x183,
	RTW_DATA_RATE_HE_NSS1_MCS4	= 0x184,
	RTW_DATA_RATE_HE_NSS1_MCS5	= 0x185,
	RTW_DATA_RATE_HE_NSS1_MCS6	= 0x186,
	RTW_DATA_RATE_HE_NSS1_MCS7	= 0x187,
	RTW_DATA_RATE_HE_NSS1_MCS8	= 0x188,
	RTW_DATA_RATE_HE_NSS1_MCS9	= 0x189,
	RTW_DATA_RATE_HE_NSS1_MCS10	= 0x18A,
	RTW_DATA_RATE_HE_NSS1_MCS11	= 0x18B,
	RTW_DATA_RATE_HE_NSS2_MCS0	= 0x190,
	RTW_DATA_RATE_HE_NSS2_MCS1	= 0x191,
	RTW_DATA_RATE_HE_NSS2_MCS2	= 0x192,
	RTW_DATA_RATE_HE_NSS2_MCS3	= 0x193,
	RTW_DATA_RATE_HE_NSS2_MCS4	= 0x194,
	RTW_DATA_RATE_HE_NSS2_MCS5	= 0x195,
	RTW_DATA_RATE_HE_NSS2_MCS6	= 0x196,
	RTW_DATA_RATE_HE_NSS2_MCS7	= 0x197,
	RTW_DATA_RATE_HE_NSS2_MCS8	= 0x198,
	RTW_DATA_RATE_HE_NSS2_MCS9	= 0x199,
	RTW_DATA_RATE_HE_NSS2_MCS10	= 0x19A,
	RTW_DATA_RATE_HE_NSS2_MCS11	= 0x19B,
	RTW_DATA_RATE_HE_NSS3_MCS0	= 0x1A0,
	RTW_DATA_RATE_HE_NSS3_MCS1	= 0x1A1,
	RTW_DATA_RATE_HE_NSS3_MCS2	= 0x1A2,
	RTW_DATA_RATE_HE_NSS3_MCS3	= 0x1A3,
	RTW_DATA_RATE_HE_NSS3_MCS4	= 0x1A4,
	RTW_DATA_RATE_HE_NSS3_MCS5	= 0x1A5,
	RTW_DATA_RATE_HE_NSS3_MCS6	= 0x1A6,
	RTW_DATA_RATE_HE_NSS3_MCS7	= 0x1A7,
	RTW_DATA_RATE_HE_NSS3_MCS8	= 0x1A8,
	RTW_DATA_RATE_HE_NSS3_MCS9	= 0x1A9,
	RTW_DATA_RATE_HE_NSS3_MCS10	= 0x1AA,
	RTW_DATA_RATE_HE_NSS3_MCS11	= 0x1AB,
	RTW_DATA_RATE_HE_NSS4_MCS0	= 0x1B0,
	RTW_DATA_RATE_HE_NSS4_MCS1	= 0x1B1,
	RTW_DATA_RATE_HE_NSS4_MCS2	= 0x1B2,
	RTW_DATA_RATE_HE_NSS4_MCS3	= 0x1B3,
	RTW_DATA_RATE_HE_NSS4_MCS4	= 0x1B4,
	RTW_DATA_RATE_HE_NSS4_MCS5	= 0x1B5,
	RTW_DATA_RATE_HE_NSS4_MCS6	= 0x1B6,
	RTW_DATA_RATE_HE_NSS4_MCS7	= 0x1B7,
	RTW_DATA_RATE_HE_NSS4_MCS8	= 0x1B8,
	RTW_DATA_RATE_HE_NSS4_MCS9	= 0x1B9,
	RTW_DATA_RATE_HE_NSS4_MCS10	= 0x1BA,
	RTW_DATA_RATE_HE_NSS4_MCS11	= 0x1BB,
	RTW_DATA_RATE_MAX = 0x1FF
};

/*==================phl_def.h of g6 driver====================*/
enum dfs_regd_t {
	DFS_REGD_UNKNOWN	= 0,
	DFS_REGD_FCC	= 1,
	DFS_REGD_JAP	= 2,
	DFS_REGD_ETSI	= 3,
	DFS_REGD_KCC	= 4,
};

struct rtw_dfs_t {
	u8 region_domain;
	bool dfs_enabled;
	bool is_dfs_detectd;
};

enum role_type {
	PHL_RTYPE_NONE,
	PHL_RTYPE_STATION,
	PHL_RTYPE_AP,
	PHL_RTYPE_VAP,
	PHL_RTYPE_ADHOC,
	PHL_RTYPE_ADHOC_MASTER,
	PHL_RTYPE_MESH,
	PHL_RTYPE_MONITOR,
	PHL_RTYPE_P2P_DEVICE,
	PHL_RTYPE_P2P_GC,
	PHL_RTYPE_P2P_GO,
	PHL_RTYPE_TDLS,
	PHL_RTYPE_NAN,
	PHL_MLME_MAX
};

/* General phl event id shall share this common enum definition
 * if definition of private events for a specific module is required,
 * please be sure to start its enum from PRIVATE_EVT_START(0x8000)
 */
enum phl_msg_evt_id {
	MSG_EVT_SCAN_START 		= 0,		//core notify halbb&halrf
	MSG_EVT_SCAN_END 			= 1,		//core notify halbb&halrf
	MSG_EVT_CONNECT_START 	= 2,		//currenty not notify
	MSG_EVT_CONNECT_END 		= 3,		//currenty not notify
	MSG_EVT_SWCH_START 		= 4,		//core notidy halrf to backup tssi, AX_TODO, currently just backup, no resume

	MSG_EVT_DBG_RX_DUMP 	= 5,
	MSG_EVT_DBG_TX_DUMP 		= 6,

	MSG_EVT_NOTIFY_BB 		= 7,		//halbb notify halbb, AX_TODO may delete

	MSG_EVT_MAX 				= 0x7fff
};

/*
Protocol - RX CAP from 80211 PKT,
driver TX related function need to
reference __rx__ of rtw_phl_stainfo_t->asoc_cap
*/
struct protocol_cap_t {
	/* MAC related */
	u8 htc_rx: 1;
	u8 trig_padding: 2;	//hal_mac used
	/* bit0:twt requester; bit1:twt responder; bit2:b-twt;
	 * bit3:Flex twt schedule; bit4:psr responder; bit5:sst;
	 */
	u8 twt: 6;
	u8 all_ack: 1;
	u8 a_ctrl: 3;	/* bit0:TRS; bit1:BSR; bit2:BQR */
	u8 ops: 1;
	u8 ht_vht_trig_rx: 1;
	u8 bsscolor;
	u16 rts_th: 10;
	u8 sgi_20: 1;		/* HT Short GI for 20 MHz */
	u8 sgi_40: 1;		/* HT Short GI for 40 MHz */
	u8 sgi_80: 1;		/* VHT Short GI for 80 MHz */
	u8 sgi_160: 1;		/* VHT Short GI for 160/80+80 MHz */

	/* BB related */
	u8 ht_ldpc: 1;
	u8 vht_ldpc: 1;
	u8 he_ldpc: 1;	//halbb used

	/*
	 * supported_rates: Supported data rate of CCK/OFDM.
	 * The rate definition follow Wi-Fi spec, unit is 500kb/s,
	 * and the MSB(bit 7) represent basic rate.
	 * ex. CCK 2Mbps not basic rate is encoded as 0x04,
	 *     and OFDM 6M basic rate is encoded as 0x8c.
	 * Suppose rates come from Supported Rates and Extended Supported
	 * Rates IE.
	 * Value 0 means it is end of array, and no more valid data rate follow.
	 */
	u8 supported_rates[12];
	u8 ht_rx_mcs[4];
	u8 ht_tx_mcs[4];
	u8 vht_rx_mcs[2];
	u8 vht_tx_mcs[2];
	u8 he_rx_mcs[6];/*80,160,80+80*/
	u8 he_tx_mcs[6];/*80,160,80+80*/
	u8 stbc_ht_tx: 1;	//halbb used
	u8 stbc_ht_rx: 2;	//halbb used
	u8 stbc_vht_rx: 3;	//halbb used
	u8 stbc_vht_tx: 1;
	u8 stbc_he_rx: 1;	//halbb used
	u8 stbc_he_tx: 1;
	u8 ltf_gi;	//halbb used
	u8 dcm_max_const_rx: 2;	//halbb used
	u8 pkt_padding: 2;
	u8 ppe_thr[8][4];
	u8 er_su: 1;	//halbb used
	u8 partial_bw_su_er: 1;

	/* RF related */
	u8 nss_tx: 3;
	u8 nss_rx: 3;	//halbb used

};

/*for halbb, driver can set these or use default value in halbb*/
struct rtw_edcca_cap_t {
	u8 edcca_adap_th_2g;
	u8 edcca_adap_th_5g;
	u8 edcca_cbp_th_6g;
	u8 edcca_carrier_sense_th;
};

struct dev_cap_t {
#if (PHYDM_VERSION == 3)
	struct rtw_edcca_cap_t edcca_cap;
	u32 rfk_cap;
	u8 pkg_type;/*todo, halbb/halrf not really used this right now*/
	u8 pwrbyrate_off;
	u8 pwrlmt_type;
	u8 rf_board_opt;
#endif
	u8 xcap;
	u8 rfe_type;
};

struct rtw_stats {
	u64 tx_byte_uni;/*halbb just print for debug*/
	u64 rx_byte_uni;/*halbb just print for debug*/
	u64 tx_byte_total;/*halbb just print for debug*/
	u64 rx_byte_total;/*halbb just print for debug*/
	u32 tx_tp_kbits;/*halbb needs*/
	u32 rx_tp_kbits;/*halbb needs*/
	u16 tx_moving_average_tp; /*todo, g6 driver todo, halbb just print for debug*/
	u16 rx_moving_average_tp; /*todo, g6 driver todo,halbb just print for debug*/
	u64 last_rx_byte_uni;
	u64 last_tx_byte_uni;
	u32 rx_tf_cnt; /* rx trigger frame number (accumulated, only reset in disconnect) */
};

struct rtw_iot_t {
	u32 id;
};

struct rtw_wifi_role_link_t {
	u8 hw_band;                    /*MAC Band0 or Band1*/
};

enum rtw_dev_state {
	RTW_DEV_WORKING = BIT0,
	RTW_DEV_SUSPENDING = BIT1,
	RTW_DEV_RESUMING = BIT2,
	RTW_DEV_SURPRISE_REMOVAL = BIT3,
	RTW_DEV_IN_DFS_CAC_PERIOD = BIT4,
	RTW_DEV_SHUTTING_DOWN = BIT5, /* set by core */
	RTW_DEV_MAX
};

struct rtw_hal_stainfo_t;
struct rtw_phl_stainfo_t {
	bool active;
	struct rtw_hal_stainfo_t *hal_sta;
	struct rtw_stats stats;
	enum wlan_mode wmode;
	struct rtw_chan_def chandef;
	struct protocol_cap_t asoc_cap;
	u8 macid;		/* driver & halbb */
	u16 aid;			/* driver */
	u8 mac_addr[6];	/* driver */
};

struct efuse_info {
	u8 thermal_a;
	u8 tssi_2g[11];
	u8 rxgain_2g_cck;
	u8 rxgain_2g_ofdm;
#if defined(SUPPORT_5G_CHANNEL)
	u8 tssi_5g[14];
	u8 rxgain_5gl;
	u8 rxgain_5gm;
	u8 rxgain_5gh;
#endif
};

struct rtw_phl_com_t {
	enum rtw_hci_type hci_type;/*useless, ameba is always axi, but hallbb need this!*/
	enum rtw_drv_mode drv_mode;/*normal or mp mode*/
	struct dev_cap_t dev_cap;
	enum rtw_dev_state dev_state;
	struct rtw_dfs_t dfs_info;
	struct rtw_iot_t id;
	void *phl_priv; /*pointer to adapter*/
	bool ap_mode; /*provide ap mode information to BB DIG*/
#if (PHYDM_VERSION == 3)
	struct rtw_stats phl_stats;
	u8 edcca_mode;
	struct efuse_info efuse_data;
#endif
	u8 bb_log;
	u8 csa_switch_ch;/*add for bb reset rssi only when csa switch channel*/
};
#endif
