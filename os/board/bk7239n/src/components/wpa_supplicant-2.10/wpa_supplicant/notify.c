/*
 * wpa_supplicant - Event notifications
 * Copyright (c) 2009-2010, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#include "../src/utils/includes.h"

#include "bk_compiler.h"
#include "../src/utils/common.h"
#include "../src/common/wpa_ctrl.h"
#include "config.h"
#include "wpa_supplicant_i.h"
#include "wps_supplicant.h"
#ifdef CONFIG_BINDER
#include "binder/binder.h"
#endif
#ifdef CONFIG_DBUS
#include "dbus/dbus_common.h"
#include "dbus/dbus_new.h"
#endif
#include "../src/rsn_supp/wpa.h"
#include "../src/fst/fst.h"
#include "../src/crypto/tls.h"
#include "bss.h"
#include "driver_i.h"
#include "wpa_scan.h"
#include "p2p_supplicant.h"
#include "sme.h"
#include "notify.h"
#if BK_SUPPLICANT
#include <components/event.h>
#include <modules/wifi.h>
//#include "bk_rw.h"
#include "../src/ap/hostapd.h"
#include "bss.h"
//#include "bk_wifi_types.h"
#include "bk_wifi.h"
#include "ctrl_iface.h"
#include "bk_wifi_types.h"
#include <modules/wifi_types.h>
#include "bk_feature.h"
 #include "../src/common/wpa_psk_cache.h"
#if CONFIG_AP_STATYPE_LIMIT
#include "bk_vsie_cus.h"
#endif
#endif
#if CONFIG_EASY_FLASH_FAST_CONNECT
#include "bk_ef.h"
#endif

#if BK_SUPPLICANT
extern uint32_t wpa_hostapd_no_password_connected(const uint8_t *addr);
#endif

int wpas_notify_supplicant_initialized(struct wpa_global *global)
{
#ifdef CONFIG_CTRL_IFACE_DBUS_NEW
	if (global->params.dbus_ctrl_interface) {
		global->dbus = wpas_dbus_init(global);
		if (global->dbus == NULL)
			return -1;
	}
#endif /* CONFIG_CTRL_IFACE_DBUS_NEW */

#ifdef CONFIG_BINDER
	global->binder = wpas_binder_init(global);
	if (!global->binder)
		return -1;
#endif /* CONFIG_BINDER */

	return 0;
}


void wpas_notify_supplicant_deinitialized(struct wpa_global *global)
{
#ifdef CONFIG_CTRL_IFACE_DBUS_NEW
	if (global->dbus)
		wpas_dbus_deinit(global->dbus);
#endif /* CONFIG_CTRL_IFACE_DBUS_NEW */

#ifdef CONFIG_BINDER
	if (global->binder)
		wpas_binder_deinit(global->binder);
#endif /* CONFIG_BINDER */
}


int wpas_notify_iface_added(struct wpa_supplicant *wpa_s)
{
	if (wpa_s->p2p_mgmt)
		return 0;

#ifdef CONFIG_DBUS
	if (wpas_dbus_register_interface(wpa_s))
		return -1;
#endif /* CONFIG_DBUS */

	return 0;
}


void wpas_notify_iface_removed(struct wpa_supplicant *wpa_s)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_DBUS
	/* unregister interface in new DBus ctrl iface */
	wpas_dbus_unregister_interface(wpa_s);
#endif /* CONFIG_DBUS */
}


#if BK_SUPPLICANT
// #if CONFIG_WIFI_MFP_CONNECT_DEAUTH
uint8_t s_tk[16];
// #endif
void wlan_store_fci(struct wpa_supplicant *wpa_s)
{
	struct wlan_fast_connect_info fci;
	char temp[4];
	int i;
	unsigned char *psk;
	struct wpa_bss *bss;
	struct wpa_ssid *ssid;
	struct wlan_fast_connect_info pre_fci = {0};
	// #if CONFIG_WIFI_MFP_CONNECT_DEAUTH
	uint8_t pmf = 0;
	// #endif

	if (!bk_feature_fast_connect_enable())
		goto out;

	if (unlikely(!wpa_s || !wpa_s->current_ssid || !wpa_s->current_bss))
		goto out;

	bss = wpa_s->current_bss;
	ssid = wpa_s->current_ssid;

	os_memset(&fci, 0, sizeof(fci));

	//read the static ip param that obtained
#if CONFIG_EASY_FLASH_FAST_CONNECT
	bk_get_env_enhance("fast_connect_id", (void *)&pre_fci, sizeof(struct wlan_fast_connect_info));
#endif

	os_memcpy(fci.ip_addr, pre_fci.ip_addr, sizeof(pre_fci.ip_addr));
	os_memcpy(fci.netmask, pre_fci.netmask, sizeof(pre_fci.netmask));
	os_memcpy(fci.gw, pre_fci.gw, sizeof(pre_fci.gw));
	os_memcpy(fci.dns1, pre_fci.dns1, sizeof(pre_fci.dns1));
	os_memcpy(fci.ssid, bss->ssid, ssid->ssid_len);
	os_memcpy(fci.bssid, bss->bssid, ETH_ALEN);
#if CONFIG_FAST_CONNECT_RESTORE_CHAN
	ieee80211_freq_to_chan(bss->freq, &fci.channel);
#endif

	if (wpa_s->pairwise_cipher > WPA_CIPHER_NONE) {
		if (ssid->passphrase)
			os_strcpy((char*)fci.pwd, ssid->passphrase);
		fci.security = wpa_s->pairwise_cipher;
		if (ssid->psk_set) {
			psk = ssid->psk;
			for (i = 0; i < PMK_LEN; i++) {
				sprintf(temp, "%02x", psk[i]);
				strcat((char*)fci.psk, temp);
			}
		}
	} else {
		fci.security = WPA_CIPHER_NONE;
	}

#if CONFIG_WLAN_FAST_CONNECT_WPA3
	if (wpa_s->wpa && wpa_s->wpa->cur_pmksa) {
		struct rsn_pmksa_cache_entry *cur_pmksa = wpa_s->wpa->cur_pmksa;
		fci.pmk_len = cur_pmksa->pmk_len;
		os_memcpy(fci.pmk, cur_pmksa->pmk, cur_pmksa->pmk_len);
		os_memcpy(fci.pmkid, cur_pmksa->pmkid, 16);
		fci.akmp = wpa_s->wpa->key_mgmt;
	}
#endif

#if CONFIG_WLAN_FAST_CONNECT_WITHOUT_SCAN
	fci.freq = bss->freq;
	fci.caps = bss->caps;
	fci.ie_len = bss->ie_len;
	fci.level = bss->level;
	fci.beacon_int = bss->beacon_int;
	os_memcpy(fci.ies, bss->ies, bss->ie_len);
#endif

	// #if CONFIG_WIFI_MFP_CONNECT_DEAUTH
	os_memcpy(fci.tk, s_tk, 16);
#ifdef CONFIG_IEEE80211W
	pmf = wpas_get_ssid_pmf(wpa_s, ssid);
	if (pmf != NO_MGMT_FRAME_PROTECTION) {
		const u8 *rsn = wpa_bss_get_ie(bss, WLAN_EID_RSN);
		struct wpa_ie_data ie;
		if (rsn && wpa_parse_wpa_ie(rsn, 2 + rsn[1], &ie) == 0 &&
			ie.capabilities & (WPA_CAPABILITY_MFPC | WPA_CAPABILITY_MFPR)) {
			pmf = MGMT_FRAME_PROTECTION_REQUIRED;
#ifdef CONFIG_OWE
		} else if (!rsn && (ssid->key_mgmt & WPA_KEY_MGMT_OWE) &&
			   !ssid->owe_only) {
			pmf = NO_MGMT_FRAME_PROTECTION;
#endif /* CONFIG_OWE */
		}
	}
#endif /* CONFIG_IEEE80211W */
	fci.pmf = pmf;
	// #endif

	/* XXX: security not set */
	//print_hex_dump("fci: ", &fci, sizeof(fci));
	wpa_hexdump(MSG_DEBUG, "fci", &fci, sizeof(fci));

	/* write fci to flash if previous fci is different with current */
#if CONFIG_EASY_FLASH_FAST_CONNECT
	bk_set_env_enhance("fast_connect_id", (void *)&fci, sizeof(struct wlan_fast_connect_info));
#endif

out:
	// #if CONFIG_WIFI_MFP_CONNECT_DEAUTH
	/* clear TK */
	os_memset(s_tk, 0, sizeof(s_tk));
	// #endif
	return;
}
#endif

//#if BK_SUPPLICANT
#if 0
#define RATE_11B_1MBPS      0x02
#define RATE_11B_2MBPS      0x04
#define RATE_11B_5_5MBPS    0x0B
#define RATE_11B_11MBPS     0x16
#define RATE_11A_6MBPS      0xC
static bool user_is_11b_1mbps_supported = false;

int wpa_check_rates_11b_1mbps_support(enum wpa_radio_work_band band, uint8_t *rates, uint8_t num_mac_rates) {

	int supported_11b = 0;

	for (int i = 0; i < num_mac_rates; i++) {

		///rate must contain 1MBPS,then we can switch btpll
		///other 11b rate with out 1MBPS can't use btpll
		if (band == BAND_2_4_GHZ && rates[i] == RATE_11B_1MBPS)
		{
			supported_11b = 1;
			break;
		}
		else if (band == BAND_5_GHZ && rates[i] == RATE_11A_6MBPS)
		{
			supported_11b = 1;
			break;
		}
	}

	return supported_11b;
}

void wpa_parse_mac_rates_ie(struct wpa_supplicant *wpa_s) {

	u8 *rates = 0;
	int num_rates, rates_is_11b_1mbps_supported;
	struct wpa_bss *bss = wpa_s->current_bss;

	/* get basic support rates and ext support rates */
	num_rates = wpa_bss_get_bit_rates(bss, &rates);

	/* check user if 11b supported according to rates */
	rates_is_11b_1mbps_supported = wpa_check_rates_11b_1mbps_support(
		wpas_freq_to_band(bss->freq), rates, num_rates);

	/* check the user is 11b only support */
	if (rates_is_11b_1mbps_supported)
		user_is_11b_1mbps_supported = true;
	else
		user_is_11b_1mbps_supported = false;

	// os_printf("XXX XXX: can use bt_pll %d\n", rates_is_11b_1mbps_supported);

	os_free(rates);
}

__IRAM_SEC bool sta_check_user_is_11b_1mbps_supported()
{
	if(user_is_11b_1mbps_supported)
		return true;
	else
		return false;
}
#endif

#if BK_SUPPLICANT
void wpas_notify_connected(struct wpa_supplicant *wpa_s)
{
	if (mhdr_get_station_status().state < WIFI_LINKSTATE_STA_CONNECTED) {
		wifi_event_sta_connected_t sta_connected = {0};
		wifi_linkstate_reason_t state = {
			.state = WIFI_LINKSTATE_STA_CONNECTED,
			.reason_code = WIFI_REASON_MAX};

		/* set mac status */
		mhdr_set_station_status(state);

#if (CONFIG_RF_POLICY_WIFI_REQ)
		wifi_station_status_event_notice(0, EVENT_WIFI_STA_CONNECTED);
#endif

		/* post event */
		os_memset(&sta_connected, 0, sizeof(sta_connected));
		if (wpa_s->current_ssid && wpa_s->current_ssid->ssid) {
			os_memcpy(&sta_connected.ssid, wpa_s->current_ssid->ssid, wpa_s->current_ssid->ssid_len);
			sta_connected.ssid[wpa_s->current_ssid->ssid_len] = '\0';
			//检查bssid是否有效
			if (is_zero_ether_addr(wpa_s->current_ssid->bssid) && !is_zero_ether_addr(wpa_s->bssid)) {
				//如果current_ssid的bssid无效但wpa_s->bssid有效，则使用后者
				os_memcpy(&sta_connected.bssid, wpa_s->bssid, ETH_ALEN);
			} else {
				os_memcpy(&sta_connected.bssid, wpa_s->current_ssid->bssid, ETH_ALEN);
			}
		}
		BK_LOG_ON_ERR(bk_event_post(EVENT_MOD_WIFI, EVENT_WIFI_STA_CONNECTED,
					&sta_connected, sizeof(sta_connected), BEKEN_NEVER_TIMEOUT));
#if BK_SUPPLICANT
		wpa_bss_flush(wpa_s);
		/* parse mac rates in the beacon frame and set 11b flags */
		//wpa_parse_mac_rates_ie(wpa_s);
#endif
#ifdef CONFIG_WPA_PSK_CACHE
		wpa_psk_thread_lower_prio();  // stop_wpa_psk_cal_thread();
#endif
		/* Store fast connect info: psk, channel, ssid, bssid, etc. */
		wlan_store_fci(wpa_s);

#if defined(BK_SUPPLICANT) && defined(CONFIG_AUTO_RECONNECT)
		wpa_s->notified_disconn = false;
#endif
	}
}

void wpas_notify_disconnected(struct wpa_supplicant *wpa_s)
{
	wifi_event_sta_disconnected_t sta_disconnected = {0};
	// get previous state
	wifi_linkstate_reason_t state = mhdr_get_station_status();
	bool local_generated = !!(wpa_s->disconnect_reason < 0);

#if defined(BK_SUPPLICANT) && defined(CONFIG_AUTO_RECONNECT)
	if (wpa_s->notified_disconn == false)
#endif
	{
		/* negative reason code for local generated */
		u16 reason = wpa_s->disconnect_reason < 0 ?
			-wpa_s->disconnect_reason : wpa_s->disconnect_reason;

		// take password wrong in action: wpas_notify_psk_mismatch set mac status in advance.
		if (state.state == WIFI_LINKSTATE_STA_DISCONNECTED &&
			(state.reason_code == WIFI_REASON_WRONG_PASSWORD ||
			state.reason_code == WIFI_REASON_NO_AP_FOUND)) {
			/* do nothing, just keep WIFI_REASON_WRONG_PASSWORD as its reason code. */
			/* When fast connect, clean fast_connnect_set flag */
			g_sta_param_ptr->fast_connect_set = 0;
		} else if ((state.state == WIFI_LINKSTATE_STA_DISCONNECTED && state.reason_code == WIFI_REASON_BEACON_LOST) || reason == WIFI_REASON_BEACON_LOST) {
			wpa_sm_pmksa_cache_flush(wpa_s->wpa, wpa_s->current_ssid);
			WPA_LOGD("AP linkloss, flush current pmksa cache\r\n");
		} else {
			state.reason_code = reason;
			state.state = WIFI_LINKSTATE_STA_DISCONNECTED;
		}
		/* set mac status */
		mhdr_set_station_status(state);

#if defined(BK_SUPPLICANT) && defined(CONFIG_AUTO_RECONNECT)
		if (!wpa_s->reconnect)
#endif
		{
			sta_disconnected.local_generated = local_generated;
			sta_disconnected.disconnect_reason = state.reason_code;
			BK_LOG_ON_ERR(bk_event_post(EVENT_MOD_WIFI, EVENT_WIFI_STA_DISCONNECTED,
					&sta_disconnected, sizeof(sta_disconnected), BEKEN_NEVER_TIMEOUT));
		}
	}
#if defined(BK_SUPPLICANT) && defined(CONFIG_AUTO_RECONNECT)
	wpa_s->notified_disconn = true;
#endif
}
#endif
extern wifi_connect_tick_t sta_tick;
extern sta_param_t *g_sta_param_ptr;
void wpas_notify_state_changed(struct wpa_supplicant *wpa_s,
			       enum wpa_states new_state,
			       enum wpa_states old_state)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_DBUS
	/* notify the new DBus API */
	wpas_dbus_signal_prop_changed(wpa_s, WPAS_DBUS_PROP_STATE);
#endif /* CONFIG_DBUS */

#ifdef CONFIG_FST
	if (wpa_s->fst && !is_zero_ether_addr(wpa_s->bssid)) {
		if (new_state == WPA_COMPLETED)
			fst_notify_peer_connected(wpa_s->fst, wpa_s->bssid);
		else if (old_state >= WPA_ASSOCIATED &&
			 new_state < WPA_ASSOCIATED)
			fst_notify_peer_disconnected(wpa_s->fst, wpa_s->bssid);
	}
#endif /* CONFIG_FST */

	if (new_state == WPA_COMPLETED)
		wpas_p2p_notif_connected(wpa_s);
	else if (old_state >= WPA_ASSOCIATED && new_state < WPA_ASSOCIATED)
		wpas_p2p_notif_disconnected(wpa_s);

	sme_state_changed(wpa_s);

#ifdef ANDROID
	wpa_msg_ctrl(wpa_s, MSG_INFO, WPA_EVENT_STATE_CHANGE
		     "id=%d state=%d BSSID=" MACSTR " SSID=%s",
		     wpa_s->current_ssid ? wpa_s->current_ssid->id : -1,
		     new_state,
		     MAC2STR(wpa_s->bssid),
		     wpa_s->current_ssid && wpa_s->current_ssid->ssid ?
		     wpa_ssid_txt(wpa_s->current_ssid->ssid,
				  wpa_s->current_ssid->ssid_len) : "");
#endif /* ANDROID */

#ifdef BK_SUPPLICANT
	if (new_state == WPA_COMPLETED) {
		if (wpa_s->key_mgmt == WPA_KEY_MGMT_NONE ||
			wpa_s->key_mgmt == WPA_KEY_MGMT_WPA_NONE ||
			(wpa_s->current_ssid &&
			wpa_s->current_ssid->mode == WPAS_MODE_IBSS)) {
			sta_tick.sta_eapol_tick = sta_tick.sta_assoc_tick;
		} else {
			sta_tick.sta_eapol_tick = rtos_get_time();
		}

		wpas_notify_connected(wpa_s);
	}

	/* FIXME: GTK rekey may failed */
	if (new_state == WPA_DISCONNECTED && old_state > WPA_SCANNING) {
#if defined(BK_SUPPLICANT) && defined(CONFIG_AUTO_RECONNECT)

		/* if auto reconnect has no limit, notify each connection state */
		if ((old_state == WPA_COMPLETED && wpa_s->disable_auto_reconnect_after_disconnect) || /* don't reconnect */
			!wpas_auto_reconnect_limited(wpa_s) || /* reconnect all handled by supplicant */
			wpa_s->disconnected) {
			if ((wpa_s->disconnect_reason == 0 || wpa_s->assoc_status_code == 1) &&
				old_state == WPA_ASSOCIATING)
			{
				if (g_sta_param_ptr->cipher_suite == BK_SECURITY_TYPE_WPA3_WPA2_MIXED ||
					g_sta_param_ptr->cipher_suite == BK_SECURITY_TYPE_WPA3_SAE)
				{
					wpa_s->disconnect_reason = WIFI_REASON_WRONG_PASSWORD;
				}
				//else if(g_sta_param_ptr->cipher_suite >= BK_SECURITY_TYPE_WPA_TKIP &&
				//	g_sta_param_ptr->cipher_suite <= BK_SECURITY_TYPE_WPA2_MIXED)
				//{
				//	wpa_s->disconnect_reason = WIFI_REASON_AUTH_FAIL;
				//}
				//else if(g_sta_param_ptr->cipher_suite == BK_SECURITY_TYPE_NONE)
				//{
				//	wpa_s->disconnect_reason = WIFI_REASON_AUTH_FAIL;
				//}
			}
			/* don't reconnect */
			wpas_notify_disconnected(wpa_s);
		}
#else
#if 0
		if ((g_sta_param_ptr->cipher_suite == BK_SECURITY_TYPE_WPA3_WPA2_MIXED ||
			g_sta_param_ptr->cipher_suite == BK_SECURITY_TYPE_WPA3_SAE) &&
			wpa_s->disconnect_reason == 0 && old_state == WPA_ASSOCIATING)
			wpa_s->disconnect_reason = WIFI_REASON_WRONG_PASSWORD;
#else
		if ((wpa_s->disconnect_reason == 0 || wpa_s->assoc_status_code == 1) &&
			old_state == WPA_ASSOCIATING)
		{
			if (g_sta_param_ptr->cipher_suite == BK_SECURITY_TYPE_WPA3_WPA2_MIXED ||
				g_sta_param_ptr->cipher_suite == BK_SECURITY_TYPE_WPA3_SAE)
			{
				wpa_s->disconnect_reason = WIFI_REASON_WRONG_PASSWORD;
			}
			//else if(g_sta_param_ptr->cipher_suite >= BK_SECURITY_TYPE_WPA_TKIP &&
			//	g_sta_param_ptr->cipher_suite <= BK_SECURITY_TYPE_WPA2_MIXED)
			//{
			//	wpa_s->disconnect_reason = WIFI_REASON_AUTH_FAIL;
			//}
			//else if(g_sta_param_ptr->cipher_suite == BK_SECURITY_TYPE_NONE)
			//{
			//	wpa_s->disconnect_reason = WIFI_REASON_AUTH_FAIL;
			//}
		}
#endif
		wpas_notify_disconnected(wpa_s);
#endif
	}
#endif /* BK_SUPPLICANT */
}

#ifdef BK_SUPPLICANT
void wpas_notify_psk_mismatch(struct wpa_supplicant *wpa_s, u16 reason_code,
						 int locally_generated)
{
	wifi_linkstate_reason_t info;
	info.reason_code = WIFI_REASON_WRONG_PASSWORD;
	info.state = WIFI_LINKSTATE_STA_DISCONNECTED;

	wpa_printf(MSG_INFO, "%s: reason code %d, locally_generated %d\n", __func__, reason_code,
		locally_generated);

	mhdr_set_station_status(info);
}

void hapd_notify_sta_connected(struct hostapd_data *hapd, const u8 *mac)
{
	wifi_event_ap_connected_t ap_connected = {0};
#if !CONFIG_DISABLE_DEPRECIATED_WIFI_API
	FUNC_1PARAM_PTR fn;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	fn = bk_wlan_get_status_cb();
#pragma GCC diagnostic pop

	if (fn) {
		uint32_t param = WIFI_LINKSTATE_AP_CONNECTED;
		(*fn)(&param);
	}

	if (wpa_hostapd_no_password_connected(mac)) {
		WPA_LOGD("send connected msg\r\n"); // the position is for wangxuejun momentarily
	}
#endif

	os_memcpy(ap_connected.mac, mac, ETH_ALEN);
	BK_LOG_ON_ERR(bk_event_post(EVENT_MOD_WIFI, EVENT_WIFI_AP_CONNECTED,
				&ap_connected, sizeof(ap_connected), BEKEN_NEVER_TIMEOUT));
}

void hapd_notify_sta_disconnected(struct hostapd_data *hapd, const u8 *mac)
{
	wifi_event_ap_disconnected_t ap_disconnected = {0};
#if !CONFIG_DISABLE_DEPRECIATED_WIFI_API
	FUNC_1PARAM_PTR fn;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	fn = bk_wlan_get_status_cb();
#pragma GCC diagnostic pop

	if (fn) {
		uint32_t param = WIFI_LINKSTATE_AP_DISCONNECTED;
		(*fn)(&param);
	}
#endif
	os_memcpy(ap_disconnected.mac, mac, ETH_ALEN);
#if CONFIG_AP_STATYPE_LIMIT
	if (bk_feature_ap_statype_limit_enable())
		bk_vsie_cus_del_sta(ap_disconnected.mac, true);
#endif
	BK_LOG_ON_ERR(bk_event_post(EVENT_MOD_WIFI, EVENT_WIFI_AP_DISCONNECTED,
				&ap_disconnected, sizeof(ap_disconnected), BEKEN_NEVER_TIMEOUT));
}

void hapd_notify_sta_psk_failure(struct hostapd_data *hapd, const u8 *mac)
{
#if !CONFIG_DISABLE_DEPRECIATED_WIFI_API
	FUNC_1PARAM_PTR fn;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	fn = bk_wlan_get_status_cb();
#pragma GCC diagnostic pop

	if (fn) {
		uint32_t param = WIFI_LINKSTATE_AP_CONNECT_FAILED;
		(*fn)(&param);
	}
#endif
}
#endif


void wpas_notify_disconnect_reason(struct wpa_supplicant *wpa_s)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_DBUS
	wpas_dbus_signal_prop_changed(wpa_s, WPAS_DBUS_PROP_DISCONNECT_REASON);
#endif /* CONFIG_DBUS */
}


void wpas_notify_auth_status_code(struct wpa_supplicant *wpa_s)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_DBUS
	wpas_dbus_signal_prop_changed(wpa_s, WPAS_DBUS_PROP_AUTH_STATUS_CODE);
#endif
}


void wpas_notify_assoc_status_code(struct wpa_supplicant *wpa_s)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_DBUS
	wpas_dbus_signal_prop_changed(wpa_s, WPAS_DBUS_PROP_ASSOC_STATUS_CODE);
#endif /* CONFIG_DBUS */
}


void wpas_notify_roam_time(struct wpa_supplicant *wpa_s)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_DBUS
	wpas_dbus_signal_prop_changed(wpa_s, WPAS_DBUS_PROP_ROAM_TIME);
#endif /* CONFIG_DBUS */
}


void wpas_notify_roam_complete(struct wpa_supplicant *wpa_s)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_DBUS
	wpas_dbus_signal_prop_changed(wpa_s, WPAS_DBUS_PROP_ROAM_COMPLETE);
#endif /* CONFIG_DBUS */
}


void wpas_notify_session_length(struct wpa_supplicant *wpa_s)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_DBUS
	wpas_dbus_signal_prop_changed(wpa_s, WPAS_DBUS_PROP_SESSION_LENGTH);
#endif /* CONFIG_DBUS */
}


void wpas_notify_bss_tm_status(struct wpa_supplicant *wpa_s)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_DBUS
	wpas_dbus_signal_prop_changed(wpa_s, WPAS_DBUS_PROP_BSS_TM_STATUS);
#endif /* CONFIG_DBUS */
}


void wpas_notify_network_changed(struct wpa_supplicant *wpa_s)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_DBUS
	wpas_dbus_signal_prop_changed(wpa_s, WPAS_DBUS_PROP_CURRENT_NETWORK);
#endif /* CONFIG_DBUS */
}


void wpas_notify_ap_scan_changed(struct wpa_supplicant *wpa_s)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_DBUS
	wpas_dbus_signal_prop_changed(wpa_s, WPAS_DBUS_PROP_AP_SCAN);
#endif /* CONFIG_DBUS */
}


void wpas_notify_bssid_changed(struct wpa_supplicant *wpa_s)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_DBUS
	wpas_dbus_signal_prop_changed(wpa_s, WPAS_DBUS_PROP_CURRENT_BSS);
#endif /* CONFIG_DBUS */
}


void wpas_notify_auth_changed(struct wpa_supplicant *wpa_s)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_DBUS
	wpas_dbus_signal_prop_changed(wpa_s, WPAS_DBUS_PROP_CURRENT_AUTH_MODE);
#endif /* CONFIG_DBUS */
}


void wpas_notify_network_enabled_changed(struct wpa_supplicant *wpa_s,
					 struct wpa_ssid *ssid)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_DBUS
	wpas_dbus_signal_network_enabled_changed(wpa_s, ssid);
#endif
}


void wpas_notify_network_selected(struct wpa_supplicant *wpa_s,
				  struct wpa_ssid *ssid)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_DBUS
	wpas_dbus_signal_network_selected(wpa_s, ssid->id);
#endif
}


void wpas_notify_network_request(struct wpa_supplicant *wpa_s,
				 struct wpa_ssid *ssid,
				 enum wpa_ctrl_req_type rtype,
				 const char *default_txt)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_DBUS
	wpas_dbus_signal_network_request(wpa_s, ssid, rtype, default_txt);
#endif
}


void wpas_notify_scanning(struct wpa_supplicant *wpa_s)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_DBUS
	/* notify the old DBus API */
	wpa_supplicant_dbus_notify_scanning(wpa_s);

	/* notify the new DBus API */
	wpas_dbus_signal_prop_changed(wpa_s, WPAS_DBUS_PROP_SCANNING);
#endif
}


void wpas_notify_scan_done(struct wpa_supplicant *wpa_s, int success)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_DBUS
	wpas_dbus_signal_scan_done(wpa_s, success);
#endif
}


void wpas_notify_scan_results(struct wpa_supplicant *wpa_s, int scan_id)
{
#if BK_SUPPLICANT
	wifi_event_scan_done_t event_data = {0};
#endif

	if (wpa_s->p2p_mgmt)
		return;

#if BK_SUPPLICANT
	if (scan_id != 0) {
		event_data.scan_id = scan_id;
		event_data.scan_use_time = wpa_s->scan_use_time.sec*1000000 + wpa_s->scan_use_time.usec;
		bk_event_post(EVENT_MOD_WIFI, EVENT_WIFI_SCAN_DONE, &event_data,
				sizeof(event_data), BEKEN_NEVER_TIMEOUT);
	}
#endif
	wpas_wps_notify_scan_results(wpa_s);
}


void wpas_notify_wps_credential(struct wpa_supplicant *wpa_s,
				const struct wps_credential *cred)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_WPS
#ifdef CONFIG_DBUS
	/* notify the new DBus API */
	wpas_dbus_signal_wps_cred(wpa_s, cred);
#endif
#endif /* CONFIG_WPS */
}


void wpas_notify_wps_event_m2d(struct wpa_supplicant *wpa_s,
			       struct wps_event_m2d *m2d)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_WPS
#ifdef CONFIG_DBUS
	wpas_dbus_signal_wps_event_m2d(wpa_s, m2d);
#endif
#endif /* CONFIG_WPS */
}


void wpas_notify_wps_event_fail(struct wpa_supplicant *wpa_s,
				struct wps_event_fail *fail)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_WPS
#ifdef CONFIG_DBUS
	wpas_dbus_signal_wps_event_fail(wpa_s, fail);
#endif
#endif /* CONFIG_WPS */
}


void wpas_notify_wps_event_success(struct wpa_supplicant *wpa_s)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_WPS
#ifdef CONFIG_DBUS
	wpas_dbus_signal_wps_event_success(wpa_s);
#endif
#endif /* CONFIG_WPS */
}

void wpas_notify_wps_event_pbc_overlap(struct wpa_supplicant *wpa_s)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_WPS
#ifdef CONFIG_DBUS
	wpas_dbus_signal_wps_event_pbc_overlap(wpa_s);
#endif
#endif /* CONFIG_WPS */
}


void wpas_notify_network_added(struct wpa_supplicant *wpa_s,
			       struct wpa_ssid *ssid)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_DBUS
	/*
	 * Networks objects created during any P2P activities should not be
	 * exposed out. They might/will confuse certain non-P2P aware
	 * applications since these network objects won't behave like
	 * regular ones.
	 */
	if (!ssid->p2p_group && wpa_s->global->p2p_group_formation != wpa_s) {
		wpas_dbus_register_network(wpa_s, ssid);
		wpa_msg_ctrl(wpa_s, MSG_INFO, WPA_EVENT_NETWORK_ADDED "%d",
			     ssid->id);
	}
#endif
}


void wpas_notify_persistent_group_added(struct wpa_supplicant *wpa_s,
					struct wpa_ssid *ssid)
{
#ifdef CONFIG_P2P
#ifdef CONFIG_DBUS
	wpas_dbus_register_persistent_group(wpa_s, ssid);
#endif
#endif /* CONFIG_P2P */
}


void wpas_notify_persistent_group_removed(struct wpa_supplicant *wpa_s,
					  struct wpa_ssid *ssid)
{
#ifdef CONFIG_P2P
#ifdef CONFIG_DBUS
	wpas_dbus_unregister_persistent_group(wpa_s, ssid->id);
#endif
#endif /* CONFIG_P2P */
}


void wpas_notify_network_removed(struct wpa_supplicant *wpa_s,
				 struct wpa_ssid *ssid)
{
	if (wpa_s->next_ssid == ssid)
		wpa_s->next_ssid = NULL;
	if (wpa_s->wpa)
		wpa_sm_pmksa_cache_flush(wpa_s->wpa, ssid);
#ifdef CONFIG_DBUS
	if (!ssid->p2p_group && wpa_s->global->p2p_group_formation != wpa_s &&
	    !wpa_s->p2p_mgmt) {
		wpas_dbus_unregister_network(wpa_s, ssid->id);
		wpa_msg_ctrl(wpa_s, MSG_INFO, WPA_EVENT_NETWORK_REMOVED "%d",
			     ssid->id);
	}
#endif
	if (network_is_persistent_group(ssid))
		wpas_notify_persistent_group_removed(wpa_s, ssid);

	wpas_p2p_network_removed(wpa_s, ssid);

#ifdef CONFIG_PASN
	if (wpa_s->pasn.ssid == ssid)
		wpa_s->pasn.ssid = NULL;
#endif /* CONFIG_PASN */
}


void wpas_notify_bss_added(struct wpa_supplicant *wpa_s,
			   u8 bssid[], unsigned int id)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_DBUS
	wpas_dbus_register_bss(wpa_s, bssid, id);
#endif
	wpa_msg_ctrl(wpa_s, MSG_INFO, WPA_EVENT_BSS_ADDED "%u " MACSTR,
		     id, MAC2STR(bssid));
}


void wpas_notify_bss_removed(struct wpa_supplicant *wpa_s,
			     u8 bssid[], unsigned int id)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_DBUS
	wpas_dbus_unregister_bss(wpa_s, bssid, id);
#endif
	wpa_msg_ctrl(wpa_s, MSG_INFO, WPA_EVENT_BSS_REMOVED "%u " MACSTR,
		     id, MAC2STR(bssid));
}


void wpas_notify_bss_freq_changed(struct wpa_supplicant *wpa_s,
				  unsigned int id)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_DBUS
	wpas_dbus_bss_signal_prop_changed(wpa_s, WPAS_DBUS_BSS_PROP_FREQ, id);
#endif
}


void wpas_notify_bss_signal_changed(struct wpa_supplicant *wpa_s,
				    unsigned int id)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_DBUS
	wpas_dbus_bss_signal_prop_changed(wpa_s, WPAS_DBUS_BSS_PROP_SIGNAL,
					  id);
#endif
}


void wpas_notify_bss_privacy_changed(struct wpa_supplicant *wpa_s,
				     unsigned int id)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_DBUS
	wpas_dbus_bss_signal_prop_changed(wpa_s, WPAS_DBUS_BSS_PROP_PRIVACY,
					  id);
#endif
}


void wpas_notify_bss_mode_changed(struct wpa_supplicant *wpa_s,
				  unsigned int id)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_DBUS
	wpas_dbus_bss_signal_prop_changed(wpa_s, WPAS_DBUS_BSS_PROP_MODE, id);
#endif
}


void wpas_notify_bss_wpaie_changed(struct wpa_supplicant *wpa_s,
				   unsigned int id)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_DBUS
	wpas_dbus_bss_signal_prop_changed(wpa_s, WPAS_DBUS_BSS_PROP_WPA, id);
#endif
}


void wpas_notify_bss_rsnie_changed(struct wpa_supplicant *wpa_s,
				   unsigned int id)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_DBUS
	wpas_dbus_bss_signal_prop_changed(wpa_s, WPAS_DBUS_BSS_PROP_RSN, id);
#endif
}


void wpas_notify_bss_wps_changed(struct wpa_supplicant *wpa_s,
				 unsigned int id)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_WPS
#ifdef CONFIG_DBUS
	wpas_dbus_bss_signal_prop_changed(wpa_s, WPAS_DBUS_BSS_PROP_WPS, id);
#endif
#endif /* CONFIG_WPS */
}


void wpas_notify_bss_ies_changed(struct wpa_supplicant *wpa_s,
				   unsigned int id)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_DBUS
	wpas_dbus_bss_signal_prop_changed(wpa_s, WPAS_DBUS_BSS_PROP_IES, id);
#endif
}


void wpas_notify_bss_rates_changed(struct wpa_supplicant *wpa_s,
				   unsigned int id)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_DBUS
	wpas_dbus_bss_signal_prop_changed(wpa_s, WPAS_DBUS_BSS_PROP_RATES, id);
#endif
}


void wpas_notify_bss_seen(struct wpa_supplicant *wpa_s, unsigned int id)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_DBUS
	wpas_dbus_bss_signal_prop_changed(wpa_s, WPAS_DBUS_BSS_PROP_AGE, id);
#endif
}


void wpas_notify_blob_added(struct wpa_supplicant *wpa_s, const char *name)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_DBUS
	wpas_dbus_signal_blob_added(wpa_s, name);
#endif
}


void wpas_notify_blob_removed(struct wpa_supplicant *wpa_s, const char *name)
{
	if (wpa_s->p2p_mgmt)
		return;

#ifdef CONFIG_DBUS
	wpas_dbus_signal_blob_removed(wpa_s, name);
#endif
}


void wpas_notify_debug_level_changed(struct wpa_global *global)
{
#ifdef CONFIG_DBUS
	wpas_dbus_signal_debug_level_changed(global);
#endif
}


void wpas_notify_debug_timestamp_changed(struct wpa_global *global)
{
#ifdef CONFIG_DBUS
	wpas_dbus_signal_debug_timestamp_changed(global);
#endif
}


void wpas_notify_debug_show_keys_changed(struct wpa_global *global)
{
#ifdef CONFIG_DBUS
	wpas_dbus_signal_debug_show_keys_changed(global);
#endif
}


void wpas_notify_suspend(struct wpa_global *global)
{
	struct wpa_supplicant *wpa_s;

	os_get_time(&global->suspend_time);
	wpa_printf(MSG_DEBUG, "System suspend notification");
	for (wpa_s = global->ifaces; wpa_s; wpa_s = wpa_s->next)
		wpa_drv_suspend(wpa_s);
}


void wpas_notify_resume(struct wpa_global *global)
{
	struct os_time now;
	int slept;
	struct wpa_supplicant *wpa_s;

	if (global->suspend_time.sec == 0)
		slept = -1;
	else {
		os_get_time(&now);
		slept = now.sec - global->suspend_time.sec;
	}
    (void)slept;
	wpa_printf(MSG_DEBUG, "System resume notification (slept %d seconds)",
		   slept);

	for (wpa_s = global->ifaces; wpa_s; wpa_s = wpa_s->next) {
		wpa_drv_resume(wpa_s);
		if (wpa_s->wpa_state == WPA_DISCONNECTED)
			wpa_supplicant_req_scan(wpa_s, 0, 100000);
	}
}


#ifdef CONFIG_P2P

void wpas_notify_p2p_find_stopped(struct wpa_supplicant *wpa_s)
{
#ifdef CONFIG_DBUS
	/* Notify P2P find has stopped */
	wpas_dbus_signal_p2p_find_stopped(wpa_s);
#endif
}


void wpas_notify_p2p_device_found(struct wpa_supplicant *wpa_s,
				  const u8 *dev_addr, int new_device)
{
#ifdef CONFIG_DBUS
	if (new_device) {
		/* Create the new peer object */
		wpas_dbus_register_peer(wpa_s, dev_addr);
	}

	/* Notify a new peer has been detected*/
	wpas_dbus_signal_peer_device_found(wpa_s, dev_addr);
#endif
}


void wpas_notify_p2p_device_lost(struct wpa_supplicant *wpa_s,
				 const u8 *dev_addr)
{
#ifdef CONFIG_DBUS
	wpas_dbus_unregister_peer(wpa_s, dev_addr);

	/* Create signal on interface object*/
	wpas_dbus_signal_peer_device_lost(wpa_s, dev_addr);
#endif
}


void wpas_notify_p2p_group_removed(struct wpa_supplicant *wpa_s,
				   const struct wpa_ssid *ssid,
				   const char *role)
{
#ifdef CONFIG_DBUS
	wpas_dbus_signal_p2p_group_removed(wpa_s, role);

	wpas_dbus_unregister_p2p_group(wpa_s, ssid);
#endif
}


void wpas_notify_p2p_go_neg_req(struct wpa_supplicant *wpa_s,
				const u8 *src, u16 dev_passwd_id, u8 go_intent)
{
#ifdef CONFIG_DBUS
	wpas_dbus_signal_p2p_go_neg_req(wpa_s, src, dev_passwd_id, go_intent);
#else
	struct p2p_event_go_req req;

	os_memcpy(req.src, src, ETH_ALEN);
	req.dev_passwd_id = dev_passwd_id;
	req.go_intent = go_intent;

	wpa_ctrl_event_copy(WPA_CTRL_EVENT_P2P_GO_NEG_REQUEST, &req, sizeof(req));
#endif
}


void wpas_notify_p2p_go_neg_completed(struct wpa_supplicant *wpa_s,
				      struct p2p_go_neg_results *res)
{
#ifdef CONFIG_DBUS
	wpas_dbus_signal_p2p_go_neg_resp(wpa_s, res);
#endif
}


void wpas_notify_p2p_invitation_result(struct wpa_supplicant *wpa_s,
				       int status, const u8 *bssid)
{
#ifdef CONFIG_DBUS
	wpas_dbus_signal_p2p_invitation_result(wpa_s, status, bssid);
#endif
}


void wpas_notify_p2p_sd_request(struct wpa_supplicant *wpa_s,
				int freq, const u8 *sa, u8 dialog_token,
				u16 update_indic, const u8 *tlvs,
				size_t tlvs_len)
{
#ifdef CONFIG_DBUS
	wpas_dbus_signal_p2p_sd_request(wpa_s, freq, sa, dialog_token,
					update_indic, tlvs, tlvs_len);
#endif
}


void wpas_notify_p2p_sd_response(struct wpa_supplicant *wpa_s,
				 const u8 *sa, u16 update_indic,
				 const u8 *tlvs, size_t tlvs_len)
{
#ifdef CONFIG_DBUS
	wpas_dbus_signal_p2p_sd_response(wpa_s, sa, update_indic,
					 tlvs, tlvs_len);
#endif
}


/**
 * wpas_notify_p2p_provision_discovery - Notification of provision discovery
 * @dev_addr: Who sent the request or responded to our request.
 * @request: Will be 1 if request, 0 for response.
 * @status: Valid only in case of response (0 in case of success)
 * @config_methods: WPS config methods
 * @generated_pin: PIN to be displayed in case of WPS_CONFIG_DISPLAY method
 *
 * This can be used to notify:
 * - Requests or responses
 * - Various config methods
 * - Failure condition in case of response
 */
void wpas_notify_p2p_provision_discovery(struct wpa_supplicant *wpa_s,
					 const u8 *dev_addr, int request,
					 enum p2p_prov_disc_status status,
					 u16 config_methods,
					 unsigned int generated_pin)
{
#ifdef CONFIG_DBUS
	wpas_dbus_signal_p2p_provision_discovery(wpa_s, dev_addr, request,
						 status, config_methods,
						 generated_pin);
#endif
}


void wpas_notify_p2p_group_started(struct wpa_supplicant *wpa_s,
				   struct wpa_ssid *ssid, int persistent,
				   int client, const u8 *ip)
{
	/* Notify a group has been started */
#ifdef CONFIG_DBUS
	wpas_dbus_register_p2p_group(wpa_s, ssid);

	wpas_dbus_signal_p2p_group_started(wpa_s, client, persistent, ip);
#endif
}


void wpas_notify_p2p_group_formation_failure(struct wpa_supplicant *wpa_s,
					     const char *reason)
{
	/* Notify a group formation failed */
#ifdef CONFIG_DBUS
	wpas_dbus_signal_p2p_group_formation_failure(wpa_s, reason);
#endif
}


void wpas_notify_p2p_wps_failed(struct wpa_supplicant *wpa_s,
				struct wps_event_fail *fail)
{
#ifdef CONFIG_DBUS
	wpas_dbus_signal_p2p_wps_failed(wpa_s, fail);
#endif
}


void wpas_notify_p2p_invitation_received(struct wpa_supplicant *wpa_s,
					 const u8 *sa, const u8 *go_dev_addr,
					 const u8 *bssid, int id, int op_freq)
{
	/* Notify a P2P Invitation Request */
#ifdef CONFIG_DBUS
	wpas_dbus_signal_p2p_invitation_received(wpa_s, sa, go_dev_addr, bssid,
						 id, op_freq);
#endif
}

#endif /* CONFIG_P2P */


static void wpas_notify_ap_sta_authorized(struct wpa_supplicant *wpa_s,
					  const u8 *sta,
					  const u8 *p2p_dev_addr)
{
#ifdef CONFIG_P2P
	wpas_p2p_notify_ap_sta_authorized(wpa_s, p2p_dev_addr);

	/*
	 * Create 'peer-joined' signal on group object -- will also
	 * check P2P itself.
	 */
#ifdef CONFIG_DBUS
	if (p2p_dev_addr)
		wpas_dbus_signal_p2p_peer_joined(wpa_s, p2p_dev_addr);
#endif
#endif /* CONFIG_P2P */

#ifdef CONFIG_DBUS
	/* Register the station */
	wpas_dbus_register_sta(wpa_s, sta);

	/* Notify listeners a new station has been authorized */
	wpas_dbus_signal_sta_authorized(wpa_s, sta);
#endif
}


static void wpas_notify_ap_sta_deauthorized(struct wpa_supplicant *wpa_s,
					    const u8 *sta,
					    const u8 *p2p_dev_addr)
{
#ifdef CONFIG_P2P
	/*
	 * Create 'peer-disconnected' signal on group object if this
	 * is a P2P group.
	 */
#ifdef CONFIG_DBUS
	if (p2p_dev_addr)
		wpas_dbus_signal_p2p_peer_disconnected(wpa_s, p2p_dev_addr);
#endif
#endif /* CONFIG_P2P */

#ifdef CONFIG_DBUS
	/* Notify listeners a station has been deauthorized */
	wpas_dbus_signal_sta_deauthorized(wpa_s, sta);

	/* Unregister the station */
	wpas_dbus_unregister_sta(wpa_s, sta);
#endif
}


void wpas_notify_sta_authorized(struct wpa_supplicant *wpa_s,
				const u8 *mac_addr, int authorized,
				const u8 *p2p_dev_addr)
{
	if (authorized)
		wpas_notify_ap_sta_authorized(wpa_s, mac_addr, p2p_dev_addr);
	else
		wpas_notify_ap_sta_deauthorized(wpa_s, mac_addr, p2p_dev_addr);
}


void wpas_notify_certification(struct wpa_supplicant *wpa_s,
			       struct tls_cert_data *cert,
			       const char *cert_hash)
{
	int i;

	wpa_msg(wpa_s, MSG_INFO, WPA_EVENT_EAP_PEER_CERT
		"depth=%d subject='%s'%s%s%s%s",
		cert->depth, cert->subject, cert_hash ? " hash=" : "",
		cert_hash ? cert_hash : "",
		cert->tod == 2 ? " tod=2" : "",
		cert->tod == 1 ? " tod=1" : "");

	if (cert->cert) {
		char *cert_hex;
		size_t len = wpabuf_len(cert->cert) * 2 + 1;
		cert_hex = os_malloc(len);
		if (cert_hex) {
			wpa_snprintf_hex(cert_hex, len, wpabuf_head(cert->cert),
					 wpabuf_len(cert->cert));
			wpa_msg_ctrl(wpa_s, MSG_INFO,
				     WPA_EVENT_EAP_PEER_CERT
				     "depth=%d subject='%s' cert=%s",
				     cert->depth, cert->subject, cert_hex);
			os_free(cert_hex);
		}
	}

	for (i = 0; i < cert->num_altsubject; i++)
		wpa_msg(wpa_s, MSG_INFO, WPA_EVENT_EAP_PEER_ALT
			"depth=%d %s", cert->depth, cert->altsubject[i]);

#ifdef CONFIG_DBUS
	/* notify the new DBus API */
	wpas_dbus_signal_certification(wpa_s, cert->depth, cert->subject,
				       cert->altsubject, cert->num_altsubject,
				       cert_hash, cert->cert);
#endif
}


void wpas_notify_preq(struct wpa_supplicant *wpa_s,
		      const u8 *addr, const u8 *dst, const u8 *bssid,
		      const u8 *ie, size_t ie_len, u32 ssi_signal)
{
#ifdef CONFIG_AP
#ifdef CONFIG_DBUS
	wpas_dbus_signal_preq(wpa_s, addr, dst, bssid, ie, ie_len, ssi_signal);
#endif
#endif /* CONFIG_AP */
}


void wpas_notify_eap_status(struct wpa_supplicant *wpa_s, const char *status,
			    const char *parameter)
{
#ifdef CONFIG_DBUS
	wpas_dbus_signal_eap_status(wpa_s, status, parameter);
#endif
	wpa_msg_ctrl(wpa_s, MSG_INFO, WPA_EVENT_EAP_STATUS
		     "status='%s' parameter='%s'",
		     status, parameter);
}


void wpas_notify_eap_error(struct wpa_supplicant *wpa_s, int error_code)
{
	wpa_msg(wpa_s, MSG_ERROR, WPA_EVENT_EAP_ERROR_CODE "%d", error_code);
}


void wpas_notify_network_bssid_set_changed(struct wpa_supplicant *wpa_s,
					   struct wpa_ssid *ssid)
{
	if (wpa_s->current_ssid != ssid)
		return;

	wpa_dbg(wpa_s, MSG_DEBUG,
		"Network bssid config changed for the current network - within-ESS roaming %s",
		ssid->bssid_set ? "disabled" : "enabled");

	wpa_drv_roaming(wpa_s, !ssid->bssid_set,
			ssid->bssid_set ? ssid->bssid : NULL);
}


void wpas_notify_network_type_changed(struct wpa_supplicant *wpa_s,
				      struct wpa_ssid *ssid)
{
#ifdef CONFIG_P2P
	if (ssid->disabled == 2) {
		/* Changed from normal network profile to persistent group */
		ssid->disabled = 0;
#ifdef CONFIG_DBUS
		wpas_dbus_unregister_network(wpa_s, ssid->id);
#endif
		ssid->disabled = 2;
		ssid->p2p_persistent_group = 1;
#ifdef CONFIG_DBUS
		wpas_dbus_register_persistent_group(wpa_s, ssid);
#endif
	} else {
		/* Changed from persistent group to normal network profile */
#ifdef CONFIG_DBUS
		wpas_dbus_unregister_persistent_group(wpa_s, ssid->id);
#endif
		ssid->p2p_persistent_group = 0;
#ifdef CONFIG_DBUS
		wpas_dbus_register_network(wpa_s, ssid);
#endif
	}
#endif /* CONFIG_P2P */
}


#ifdef CONFIG_MESH

void wpas_notify_mesh_group_started(struct wpa_supplicant *wpa_s,
				    struct wpa_ssid *ssid)
{
	if (wpa_s->p2p_mgmt)
		return;

	wpas_dbus_signal_mesh_group_started(wpa_s, ssid);
}


void wpas_notify_mesh_group_removed(struct wpa_supplicant *wpa_s,
				    const u8 *meshid, u8 meshid_len,
				    u16 reason_code)
{
	if (wpa_s->p2p_mgmt)
		return;

	wpas_dbus_signal_mesh_group_removed(wpa_s, meshid, meshid_len,
					    reason_code);
}


void wpas_notify_mesh_peer_connected(struct wpa_supplicant *wpa_s,
				     const u8 *peer_addr)
{
	if (wpa_s->p2p_mgmt)
		return;

	wpas_dbus_signal_mesh_peer_connected(wpa_s, peer_addr);
}


void wpas_notify_mesh_peer_disconnected(struct wpa_supplicant *wpa_s,
					const u8 *peer_addr, u16 reason_code)
{
	if (wpa_s->p2p_mgmt)
		return;

	wpas_dbus_signal_mesh_peer_disconnected(wpa_s, peer_addr, reason_code);
}

#endif /* CONFIG_MESH */


#ifdef CONFIG_INTERWORKING

void wpas_notify_interworking_ap_added(struct wpa_supplicant *wpa_s,
				       struct wpa_bss *bss,
				       struct wpa_cred *cred, int excluded,
				       const char *type, int bh, int bss_load,
				       int conn_capab)
{
	wpa_msg(wpa_s, MSG_INFO, "%s" MACSTR " type=%s%s%s%s id=%d priority=%d sp_priority=%d",
		excluded ? INTERWORKING_EXCLUDED : INTERWORKING_AP,
		MAC2STR(bss->bssid), type,
		bh ? " below_min_backhaul=1" : "",
		bss_load ? " over_max_bss_load=1" : "",
		conn_capab ? " conn_capab_missing=1" : "",
		cred->id, cred->priority, cred->sp_priority);

	wpas_dbus_signal_interworking_ap_added(wpa_s, bss, cred, type, excluded,
					       bh, bss_load, conn_capab);
}


void wpas_notify_interworking_select_done(struct wpa_supplicant *wpa_s)
{
	wpas_dbus_signal_interworking_select_done(wpa_s);
}

#endif /* CONFIG_INTERWORKING */
