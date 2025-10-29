#ifndef _WIFI_CONFIG_H_
#define _WIFI_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * brief          WIFI mac context structure
 */

// default RAW_TX_AC value
#define WIFI_CFG_DFT_RAW_TX_AC					3
// maximum RAW_TX_AC value
#define WIFI_CFG_MAX_RAW_TX_AC					4
// default RAW_TX_AC_TIMEOUT value
#define WIFI_CFG_DFT_RAW_TX_AC_TIMEOUT_MS		50
// default minimux memory size Wi-Fi should reserve value
#define WIFI_CFG_DFT_MIN_RSV_MEM				10*1024
// maximum memory size Wi-Fi should reserve value
#define WIFI_CFG_MAX_RSV_MEM					30*1024
// default TX maximum MSDU count value
#define WIFI_CFG_DFT_TX_MSDU_MAX_CNT			64
// default media quality value
#define WIFI_CFG_DFT_MEDIA_QUALITY				2
// default data retry value
#define WIFI_CFG_DATA_RTY_NUM					0xF0F0
// default mgmt retry value
#define WIFI_CFG_MGMT_RTY_NUM					0x0704

typedef struct wifi_mac_config {
	// raw tx interface AC
	uint8_t raw_tx_ac;
	// raw tx interface TX timeout
	uint16_t raw_tx_timeout_ms;
	// maximum MSDU count that Wi-Fi could handle
	uint16_t tx_max_msdu_cnt;
	// minimum heap memory size that Wi-Fi should reserve
	uint16_t min_rsv_mem;
	//wifi media flag
	bool media_flag;
	//media quality
	uint8_t media_quality;
	//management frame retry num
	uint16_t mgmt_rty_num;
	//data frame retry num
	uint16_t data_rty_num;
	//block wifi broadcast and multicast frame
	uint8_t block_bcmc_en;
	//Microwave anti-interference policy enable
	bool ani_en;
} wifi_mac_config_t;

extern wifi_mac_config_t g_wifi_mac_config;

#ifdef __cplusplus
}
#endif
#endif //_WIFI_CONFIG_H_
// eof

