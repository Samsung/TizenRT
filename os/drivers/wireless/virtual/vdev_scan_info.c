#include <sys/types.h>
#include <time.h>
#include <tinyara/net/if/wifi.h>

trwifi_ap_scan_info_s g_ap_info[5] = {
	{1, "testhub_01", 10, "01:02:03:04:05:06", 30, 20, TRWIFI_IEEE_80211_N, TRWIFI_AUTH_WPA2_PSK, TRWIFI_CRYPTO_AES},
	{2, "testhub_02", 10, "01:02:03:04:05:06", 30, 20, TRWIFI_IEEE_80211_N, TRWIFI_AUTH_WPA2_PSK, TRWIFI_CRYPTO_AES},
	{3, "testhub_03", 10, "01:02:03:04:05:06", 30, 20, TRWIFI_IEEE_80211_N, TRWIFI_AUTH_WPA2_PSK, TRWIFI_CRYPTO_AES},
	{4, "testhub_04", 10, "01:02:03:04:05:06", 30, 20, TRWIFI_IEEE_80211_N, TRWIFI_AUTH_WPA2_PSK, TRWIFI_CRYPTO_AES},
	{5, "testhub_05", 10, "01:02:03:04:05:06", 30, 20, TRWIFI_IEEE_80211_N, TRWIFI_AUTH_WPA2_PSK, TRWIFI_CRYPTO_AES},
};

trwifi_scan_list_s g_vwifi_scan_list[5];

void vwifi_initialize_scan(void)
{
	for (int i = 0; i < 5; i++) {
		memcpy(&(g_vwifi_scan_list[i]), &g_ap_info[i], sizeof(trwifi_ap_scan_info_s));
	}
	for (int i = 0; i < 4; i++) {
		g_vwifi_scan_list[i].next = &g_vwifi_scan_list[i+1];
	}
	g_vwifi_scan_list[4].next = NULL;
}

trwifi_scan_list_s *vwifi_get_scan_list(void)
{
	return &g_vwifi_scan_list[0];
}
