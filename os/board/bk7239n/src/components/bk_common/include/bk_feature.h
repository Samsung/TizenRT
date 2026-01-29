// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/* @brief Overview about this API header
 *
 */

/**
 * @brief     Interface for customer to Enable/Disable bssid connect functionality
 *
 * @attention 1 Controled by middleware/soc/bk72xx.defconfig
 * @attention 2 Support to use with fast connect
 *
 * @return
 *    - 1: enable
 *    - 0: disable
 */
int bk_feature_bssid_connect_enable(void);

/**
 * @brief     Interface for customer to Enable/Disable fast connect functionality
 *
 * @attention 1 This API only enable phase 1 fast connect (skip psk calculate、all channel scanning)
 * @attention 2 Controled by middleware/soc/bk72xx.defconfig
 *
 * @return
 *    - 1: enable
 *    - 0: disable
 */
int bk_feature_fast_connect_enable(void);

/**
 * @brief     Interface for customer to Enable/Disable fast dhcp functionality
 *
 * @attention 1 This API only enable phase 2 fast connect (skip dhcp and use static ip saved in flash)
 * @attention 2 Controled by middleware/soc/bk72xx.defconfig
 *
 * @return
 *    - 1: enable
 *    - 0: disable
 */
int bk_feature_fast_dhcp_enable(void);

/**
 * @brief     Interface for customer to Enable/Disable Station Vendor Specific IEs
 *
 * @attention 1 Controled by middleware/soc/bk72xx.defconfig
 *
 * @return
 *    - 1: enable
 *    - 0: disable
 */
int bk_feature_sta_vsie_enable(void);

/**
 * @brief     Interface for customer to Enable/Disable max limit connection of sta to softap
 *
 * @attention 1 Controled by middleware/soc/bk72xx.defconfig
 *
 * @return
 *    - 1: enable
 *    - 0: disable
 */
int bk_feature_ap_statype_limit_enable(void);

/**
 * @brief     Interface for customer to Enable/Disable temp detect functionality
 *
 * @attention 1 Controled by middleware/soc/bk72xx.defconfig
 *
 * @return
 *    - 1: enable
 *    - 0: disable
 */
int bk_feature_temp_detect_enable(void);


typedef enum {
    BK_CPU0 = 0, /**< cpu0 id */
    BK_CPU1 = 1, /**< cpu1 id */
    BK_CPU2 = 2 /**< cpu2 id */
} BK_CPU_ID;


/**
 * @brief     Interface to get if current system is cpu0
 *
 *
 * @return
 *    - BK_CPU0: current system is cpu0
 *    - BK_CPU1: current system is cpu1
 *    - BK_CPU2: current system is cpu2
 *
 */
int bk_feature_get_cpu_id(void);


/**
 * @brief     Interface to get the cpu count of system
 *
 *
 * @return
 *    - 1: current system is single core
 *    - 2: current system is dual core
 *    - 3: current system is triple core
 *
 */
int bk_feature_get_cpu_cnt(void);


/**
 * @brief     Interface for customer to Enable/Disable Station receiving BCMC frame in dtim10
 *
 *
 * @return
 *    - 0: Disable Station to receive BCMC frame in dtim10
 *    - 1: Enable Station to receive BCMC frame in dtim10
 *
 */
int bk_feature_receive_bcmc_enable(void);

/**
 * @brief     Interface for customer to Enable/Disable not check beacon ssid changes
 *
 *
 * @return
 *    - 0: Disable not check beacon ssid changes
 *    - 1: Enable not check beacon ssid changes
 *
 */
int bk_feature_not_check_ssid_enable(void);

/**
 * @brief     Interface for customer to Enable/Disable station to get WIFI CSI
 *
 *
 * @return
 *    - 0: Disable station to get WIFI CSI
 *    - 1: Enable station to get WIFI CSI
 *
 */
int bk_feature_config_wifi_csi_enable(void);
/**
 * @brief     Interface for customer to close ap csa in coexist mode
 *
 *
 * @return
 *    - 0: Open coexist csa
 *    - 1: Close coexist csa
 *
 */
int bk_feature_close_coexist_csa(void);
/**
 * @brief     Interface for customer to set the MAX number of stations supported by Mac
 *
 *
 * @return
 *    - the MAX number of stations supported by Mac
 */
int bk_feature_get_mac_sup_sta_max_num(void);

/**
 * @brief     Interface for customer to control network found event report
 *
 *
 * @return
 *    - 0: disable network found evevnt
 *    - 1: enable network found event
 *
 */

int bk_feature_network_found_event(void);


int bk_feature_config_cache_enable(void);
int bk_feature_ckmn_enable(void);
int bk_feature_send_deauth_before_connect(void);
int bk_feature_get_scan_speed_level(void);
/**
 * @brief     Interface for customer to save cali data to OTP2
 *
 *
 * @return
 *    - 0: Disable save cali data to OTP2
 *    - 1: Enable save cali data to OTP2
 *
 */
int bk_feature_save_rfcali_to_otp_enable(void);

/**
 * @brief     Interface for customer change iocurlim when volt is 5v
 *
 *
 * @return
 *    - 0: set iocurlim 0
 *    - 1: set iocurlim 1
 *
 */
int bk_feature_volt_5v(void);

/**
 * @brief     Interface for customer to close or open phy log
 *
 *
 * @return
 *    - 0: close phy log
 *    - 1: open phy log
 *
 */
int bk_feature_phy_log_enable(void);

/**
 * @brief     Interface for customer to close or open mac pd during lv sleep
 *
 *
 * @return
 *    - 0: mac always on
 *    - 1: mac power down during lv
 *
 */
int bk_feature_mac_pwd_enable(void);

/**
 * @brief     Interface for customer to close or open wrls pd during lv sleep
 *
 *
 * @return
 *    - 0: wrls always on
 *    - 1: wrls power down during lv
 *
 */
int bk_feature_wrls_pwd_enable(void);

/**
 * @brief     Interface for customer to close or open cpu pd during lv sleep
 *
 *
 * @return
 *    - 0: cpu always on
 *    - 1: cpu power down during lv
 *
 */
int bk_feature_cpu_pwd_enable(void);

/**
 * @brief     Interface for customer to close or open mac use rtc during lv sleep
 *
 *
 * @return
 *    - 0: mac use rtc
 *    - 1: mac use timer
 */
int bk_feature_mac_use_rtc_enable(void);

/**
 * @brief     Interface for customer to change to wifi pll when wakeup from sleep
 *
 *
 * @return
 *    - 0: disable change to wifi pll
 *    - 1: enable change to wifi pll
 *
 */
int bk_feature_change_to_wifi_pll_enable(void);

/**
 * @brief     Interface for customer to enable or disable BK7239N MP
 *
 *
 * @return
 *    - 0: disable BK7239N MP
 *    - 1: enable BK7239N MP
 */
int bk_feature_bk7239n_mp_enable(void);

/**
 * @brief     Interface for customer to close or open temp high volt
 *
 *
 * @return
 *    - 0: disable high volt
 *    - 1: enable high volt
 *
 */
int bk_feature_temp_high_volt_enable(void);

/**
 * @brief     Interface for update power with RSSI
 *
 *
 * @return
 *    - 0: disable update power with RSSI
 *    - 1: enable update power with RSSI
 *
 */
int bk_feature_update_power_with_rssi(void);