#include "bk_feature.h"
#include "tinyara/config.h"



int bk_feature_bssid_connect_enable(void) {
#if defined(CONFIG_BSSID_CONNECT)
	return 1;
#else
	return 0;
#endif
}

int bk_feature_fast_connect_enable(void) {
#if defined(CONFIG_WIFI_FAST_CONNECT)
	return 1;
#else
	return 0;
#endif
}

int bk_feature_fast_dhcp_enable(void) {
#if defined(CONFIG_WIFI_FAST_DHCP)
	return 1;
#else
	return 0;
#endif
}

int bk_feature_sta_vsie_enable(void) {
#if defined(CONFIG_COMPONENTS_STA_VSIE)
	return 1;
#else
	return 0;
#endif
}

int bk_feature_ap_statype_limit_enable(void) {
#if defined(CONFIG_AP_STATYPE_LIMIT)
	return 1;
#else
	return 0;
#endif
}

int bk_feature_temp_detect_enable(void) {
#if defined(CONFIG_TEMP_DETECT)
    return 1;
#else
    return 0;
#endif
}

int bk_feature_get_cpu_id(void) {
#if defined(CONFIG_SYS_CPU0)
    return BK_CPU0;
#elif defined(CONFIG_SYS_CPU1)
    return BK_CPU1;
#elif defined(CONFIG_SYS_CPU2)
    return BK_CPU2;
#else
    return 0;
#endif
}


int bk_feature_get_cpu_cnt(void) {
#if defined(CONFIG_CPU_CNT)
    return CONFIG_CPU_CNT;
#else
    return 1;
#endif
}


int bk_feature_receive_bcmc_enable(void) {
#if defined(CONFIG_RECEIVE_BCMC_IN_DTIM10)
    return 1;
#else
    return 0;
#endif
}

int bk_feature_not_check_ssid_enable(void) {
#if defined(CONFIG_NOT_CHECK_SSID_CHANGE)
    return 1;
#else
    return 0;
#endif
}

int bk_feature_config_cache_enable(void) {
#if defined(CONFIG_CACHE_ENABLE)
	return 1;
#else
	return 0;
#endif
}

int bk_feature_ckmn_enable(void) {
#if defined(CONFIG_CKMN)
	return 1;
#else
	return 0;
#endif
}

int bk_feature_send_deauth_before_connect(void) {
#if defined(CONFIG_DEAUTH_BEFORE_CONNECT)
	return 1;
#else
	return 0;
#endif
}

int bk_feature_config_wifi_csi_enable(void) {
#if defined(CONFIG_WIFI_CSI_EN)
	return 1;
#else
	return 0;
#endif
}

int bk_feature_close_coexist_csa(void) {
#if defined(CONFIG_CLOSE_COEXIST_CSA)
	return 1;
#else
	return 0;
#endif
}

int bk_feature_get_scan_speed_level(void) {
	return CONFIG_SCAN_SPEED_LEVEL;
}

int bk_feature_get_mac_sup_sta_max_num(void) {
#if defined(CONFIG_WIFI_MAC_SUPPORT_STAS_MAX_NUM)
    return CONFIG_WIFI_MAC_SUPPORT_STAS_MAX_NUM;
#else
    return 2;
#endif
}

int bk_feature_network_found_event(void) {
#if defined(CONFIG_NETWORK_FOUND_EVENT_ENABLE)
	return 1;
#else
	return 0;
#endif
}

int bk_feature_save_rfcali_to_otp_enable(void) {
#if (defined(CONFIG_OTP) && defined(CONFIG_PHY_RFCALI_TO_OTP))
    return 1;
#else
    return 0;
#endif
}

int bk_feature_volt_5v(void) {
#if defined(CONFIG_VBAT_5V)
    return 1;
#else
    return 0;
#endif
}

int bk_feature_phy_log_enable(void) {
#if defined(CONFIG_PHY_LOG_ENABLE)
	return 1;
#else
	return 0;
#endif
}

int bk_feature_mac_pwd_enable(void)
{
#if defined(CONFIG_MAC_PWD)
	return 1;
#else
	return 0;
#endif
}

int bk_feature_wrls_pwd_enable(void)
{
#if defined(CONFIG_WRLS_PWD)
	return 1;
#else
	return 0;
#endif
}

int bk_feature_cpu_pwd_enable(void)
{
#if defined(CONFIG_DEEP_LV)
	return 1;
#else
	return 0;
#endif
}

int bk_feature_mac_use_rtc_enable(void)
{
#if defined(CONFIG_MAC_USE_RTC)
	return 1;
#else
	return 0;
#endif
}

int bk_feature_change_to_wifi_pll_enable(void)
{
#if defined(CONFIG_WIFI_WAKEUP_CHANGE_TO_WIFI_PLL)
	return 1;
#else
	return 0;
#endif
}

int bk_feature_bk7239n_mp_enable(void)
{
#if defined(CONFIG_BK7239N_MP)
	return 1;
#else
	return 0;
#endif
}

int bk_feature_temp_high_volt_enable(void)
{
#if defined(CONFIG_TEMPERATURE_HIGH_VOLT)
	return 1;
#else
	return 0;
#endif
}

int bk_feature_update_power_with_rssi(void) {
#if (CONFIG_UPDATE_POWER_WITH_RSSI)
	return 1;
#else
	return 0;
#endif
}