/*
 * wpa_supplicant - Event notifications
 * Copyright (c) 2009-2010, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#include "utils/includes.h"

#include "utils/common.h"
#include "common/wpa_ctrl.h"
#include "config.h"
#include "wpa_supplicant_i.h"
#include "wps_supplicant.h"
#include "rsn_supp/wpa.h"
#include "driver_i.h"
#include "scan.h"
#include "p2p_supplicant.h"
#include "sme.h"
#include "notify.h"

int wpas_notify_supplicant_initialized(struct wpa_global *global)
{
	return 0;
}

void wpas_notify_supplicant_deinitialized(struct wpa_global *global)
{
}

int wpas_notify_iface_added(struct wpa_supplicant *wpa_s)
{
	if (wpa_s->p2p_mgmt) {
		return 0;
	}

	return 0;
}

void wpas_notify_iface_removed(struct wpa_supplicant *wpa_s)
{
	if (wpa_s->p2p_mgmt) {
		return;
	}
}

void wpas_notify_state_changed(struct wpa_supplicant *wpa_s, enum wpa_states new_state, enum wpa_states old_state)
{
	if (wpa_s->p2p_mgmt) {
		return;
	}

	if (new_state == WPA_COMPLETED) {
		wpas_p2p_notif_connected(wpa_s);
	} else if (old_state >= WPA_ASSOCIATED && new_state < WPA_ASSOCIATED) {
		wpas_p2p_notif_disconnected(wpa_s);
	}

	sme_state_changed(wpa_s);

#ifdef ANDROID
	wpa_msg_ctrl(wpa_s, MSG_INFO, WPA_EVENT_STATE_CHANGE "id=%d state=%d BSSID=" MACSTR " SSID=%s", wpa_s->current_ssid ? wpa_s->current_ssid->id : -1, new_state, MAC2STR(wpa_s->bssid), wpa_s->current_ssid && wpa_s->current_ssid->ssid ? wpa_ssid_txt(wpa_s->current_ssid->ssid, wpa_s->current_ssid->ssid_len) : "");
#endif							/* ANDROID */
}

void wpas_notify_disconnect_reason(struct wpa_supplicant *wpa_s)
{
	if (wpa_s->p2p_mgmt) {
		return;
	}
}

void wpas_notify_network_changed(struct wpa_supplicant *wpa_s)
{
	if (wpa_s->p2p_mgmt) {
		return;
	}
}

void wpas_notify_ap_scan_changed(struct wpa_supplicant *wpa_s)
{
	if (wpa_s->p2p_mgmt) {
		return;
	}
}

void wpas_notify_bssid_changed(struct wpa_supplicant *wpa_s)
{
	if (wpa_s->p2p_mgmt) {
		return;
	}
}

void wpas_notify_auth_changed(struct wpa_supplicant *wpa_s)
{
	if (wpa_s->p2p_mgmt) {
		return;
	}
}

void wpas_notify_network_enabled_changed(struct wpa_supplicant *wpa_s, struct wpa_ssid *ssid)
{
	if (wpa_s->p2p_mgmt) {
		return;
	}
}

void wpas_notify_network_selected(struct wpa_supplicant *wpa_s, struct wpa_ssid *ssid)
{
	if (wpa_s->p2p_mgmt) {
		return;
	}
}

void wpas_notify_network_request(struct wpa_supplicant *wpa_s, struct wpa_ssid *ssid, enum wpa_ctrl_req_type rtype, const char *default_txt)
{
	if (wpa_s->p2p_mgmt) {
		return;
	}
}

void wpas_notify_scanning(struct wpa_supplicant *wpa_s)
{
	if (wpa_s->p2p_mgmt) {
		return;
	}
}

void wpas_notify_scan_done(struct wpa_supplicant *wpa_s, int success)
{
	if (wpa_s->p2p_mgmt) {
		return;
	}
}

void wpas_notify_scan_results(struct wpa_supplicant *wpa_s)
{
	if (wpa_s->p2p_mgmt) {
		return;
	}

	wpas_wps_notify_scan_results(wpa_s);
}

void wpas_notify_wps_credential(struct wpa_supplicant *wpa_s, const struct wps_credential *cred)
{
	if (wpa_s->p2p_mgmt) {
		return;
	}
}

void wpas_notify_wps_event_m2d(struct wpa_supplicant *wpa_s, struct wps_event_m2d *m2d)
{
	if (wpa_s->p2p_mgmt) {
		return;
	}
}

void wpas_notify_wps_event_fail(struct wpa_supplicant *wpa_s, struct wps_event_fail *fail)
{
	if (wpa_s->p2p_mgmt) {
		return;
	}
}

void wpas_notify_wps_event_success(struct wpa_supplicant *wpa_s)
{
	if (wpa_s->p2p_mgmt) {
		return;
	}
}

void wpas_notify_wps_event_pbc_overlap(struct wpa_supplicant *wpa_s)
{
	if (wpa_s->p2p_mgmt) {
		return;
	}
}

void wpas_notify_network_added(struct wpa_supplicant *wpa_s, struct wpa_ssid *ssid)
{
	if (wpa_s->p2p_mgmt) {
		return;
	}
}

void wpas_notify_persistent_group_added(struct wpa_supplicant *wpa_s, struct wpa_ssid *ssid)
{
}

void wpas_notify_persistent_group_removed(struct wpa_supplicant *wpa_s, struct wpa_ssid *ssid)
{
}

void wpas_notify_network_removed(struct wpa_supplicant *wpa_s, struct wpa_ssid *ssid)
{
	if (wpa_s->next_ssid == ssid) {
		wpa_s->next_ssid = NULL;
	}
	if (wpa_s->wpa) {
		wpa_sm_pmksa_cache_flush(wpa_s->wpa, ssid);
	}
	if (network_is_persistent_group(ssid)) {
		wpas_notify_persistent_group_removed(wpa_s, ssid);
	}

	wpas_p2p_network_removed(wpa_s, ssid);
}

void wpas_notify_bss_added(struct wpa_supplicant *wpa_s, u8 bssid[], unsigned int id)
{
	if (wpa_s->p2p_mgmt) {
		return;
	}

	/*	wpa_msg_ctrl(wpa_s, MSG_INFO, WPA_EVENT_BSS_ADDED "%u " MACSTR,
			     id, MAC2STR(bssid));
	*/
}

void wpas_notify_bss_removed(struct wpa_supplicant *wpa_s, u8 bssid[], unsigned int id)
{
	if (wpa_s->p2p_mgmt) {
		return;
	}

	/*	wpa_msg_ctrl(wpa_s, MSG_INFO, WPA_EVENT_BSS_REMOVED "%u " MACSTR,
			     id, MAC2STR(bssid));
	*/
}

void wpas_notify_bss_freq_changed(struct wpa_supplicant *wpa_s, unsigned int id)
{
	if (wpa_s->p2p_mgmt) {
		return;
	}
}

void wpas_notify_bss_signal_changed(struct wpa_supplicant *wpa_s, unsigned int id)
{
	if (wpa_s->p2p_mgmt) {
		return;
	}
}

void wpas_notify_bss_privacy_changed(struct wpa_supplicant *wpa_s, unsigned int id)
{
	if (wpa_s->p2p_mgmt) {
		return;
	}
}

void wpas_notify_bss_mode_changed(struct wpa_supplicant *wpa_s, unsigned int id)
{
	if (wpa_s->p2p_mgmt) {
		return;
	}
}

void wpas_notify_bss_wpaie_changed(struct wpa_supplicant *wpa_s, unsigned int id)
{
	if (wpa_s->p2p_mgmt) {
		return;
	}
}

void wpas_notify_bss_rsnie_changed(struct wpa_supplicant *wpa_s, unsigned int id)
{
	if (wpa_s->p2p_mgmt) {
		return;
	}
}

void wpas_notify_bss_wps_changed(struct wpa_supplicant *wpa_s, unsigned int id)
{
	if (wpa_s->p2p_mgmt) {
		return;
	}
}

void wpas_notify_bss_ies_changed(struct wpa_supplicant *wpa_s, unsigned int id)
{
	if (wpa_s->p2p_mgmt) {
		return;
	}
}

void wpas_notify_bss_rates_changed(struct wpa_supplicant *wpa_s, unsigned int id)
{
	if (wpa_s->p2p_mgmt) {
		return;
	}
}

void wpas_notify_bss_seen(struct wpa_supplicant *wpa_s, unsigned int id)
{
	if (wpa_s->p2p_mgmt) {
		return;
	}
}

void wpas_notify_blob_added(struct wpa_supplicant *wpa_s, const char *name)
{
	if (wpa_s->p2p_mgmt) {
		return;
	}
}

void wpas_notify_blob_removed(struct wpa_supplicant *wpa_s, const char *name)
{
	if (wpa_s->p2p_mgmt) {
		return;
	}
}

void wpas_notify_debug_level_changed(struct wpa_global *global)
{
}

void wpas_notify_debug_timestamp_changed(struct wpa_global *global)
{
}

void wpas_notify_debug_show_keys_changed(struct wpa_global *global)
{
}

void wpas_notify_suspend(struct wpa_global *global)
{
	struct wpa_supplicant *wpa_s;

	os_get_time(&global->suspend_time);
	wpa_printf(MSG_DEBUG, "System suspend notification");
	for (wpa_s = global->ifaces; wpa_s; wpa_s = wpa_s->next) {
		wpa_drv_suspend(wpa_s);
	}
}

void wpas_notify_resume(struct wpa_global *global)
{
	struct os_time now;
	int slept;
	struct wpa_supplicant *wpa_s;

	if (global->suspend_time.sec == 0) {
		slept = -1;
	} else {
		os_get_time(&now);
		slept = now.sec - global->suspend_time.sec;
	}
	wpa_printf(MSG_DEBUG, "System resume notification (slept %d seconds)", slept);

	for (wpa_s = global->ifaces; wpa_s; wpa_s = wpa_s->next) {
		wpa_drv_resume(wpa_s);
		if (wpa_s->wpa_state == WPA_DISCONNECTED) {
			wpa_supplicant_req_scan(wpa_s, 0, 100000);
		}
	}
}

#ifdef CONFIG_P2P

void wpas_notify_p2p_find_stopped(struct wpa_supplicant *wpa_s)
{
}

void wpas_notify_p2p_device_found(struct wpa_supplicant *wpa_s, const u8 *dev_addr, int new_device)
{
}

void wpas_notify_p2p_device_lost(struct wpa_supplicant *wpa_s, const u8 *dev_addr)
{
}

void wpas_notify_p2p_group_removed(struct wpa_supplicant *wpa_s, const struct wpa_ssid *ssid, const char *role)
{
}

void wpas_notify_p2p_go_neg_req(struct wpa_supplicant *wpa_s, const u8 *src, u16 dev_passwd_id, u8 go_intent)
{
}

void wpas_notify_p2p_go_neg_completed(struct wpa_supplicant *wpa_s, struct p2p_go_neg_results *res)
{
}

void wpas_notify_p2p_invitation_result(struct wpa_supplicant *wpa_s, int status, const u8 *bssid)
{
}

void wpas_notify_p2p_sd_request(struct wpa_supplicant *wpa_s, int freq, const u8 *sa, u8 dialog_token, u16 update_indic, const u8 *tlvs, size_t tlvs_len)
{
}

void wpas_notify_p2p_sd_response(struct wpa_supplicant *wpa_s, const u8 *sa, u16 update_indic, const u8 *tlvs, size_t tlvs_len)
{
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
void wpas_notify_p2p_provision_discovery(struct wpa_supplicant *wpa_s, const u8 *dev_addr, int request, enum p2p_prov_disc_status status, u16 config_methods, unsigned int generated_pin)
{
}

void wpas_notify_p2p_group_started(struct wpa_supplicant *wpa_s, struct wpa_ssid *ssid, int network_id, int client)
{
}

void wpas_notify_p2p_wps_failed(struct wpa_supplicant *wpa_s, struct wps_event_fail *fail)
{
}

#endif							/* CONFIG_P2P */

static void wpas_notify_ap_sta_authorized(struct wpa_supplicant *wpa_s, const u8 *sta, const u8 *p2p_dev_addr)
{
#ifdef CONFIG_P2P
	wpas_p2p_notify_ap_sta_authorized(wpa_s, p2p_dev_addr);
#endif							/* CONFIG_P2P */
}

static void wpas_notify_ap_sta_deauthorized(struct wpa_supplicant *wpa_s, const u8 *sta, const u8 *p2p_dev_addr)
{
}

void wpas_notify_sta_authorized(struct wpa_supplicant *wpa_s, const u8 *mac_addr, int authorized, const u8 *p2p_dev_addr)
{
	if (authorized) {
		wpas_notify_ap_sta_authorized(wpa_s, mac_addr, p2p_dev_addr);
	} else {
		wpas_notify_ap_sta_deauthorized(wpa_s, mac_addr, p2p_dev_addr);
	}
}

void wpas_notify_certification(struct wpa_supplicant *wpa_s, int depth, const char *subject, const char *altsubject[], int num_altsubject, const char *cert_hash, const struct wpabuf *cert)
{
	wpa_msg(wpa_s, MSG_INFO, WPA_EVENT_EAP_PEER_CERT "depth=%d subject='%s'%s%s", depth, subject, cert_hash ? " hash=" : "", cert_hash ? cert_hash : "");

	if (cert) {
		char *cert_hex;
		size_t len = wpabuf_len(cert) * 2 + 1;
		cert_hex = os_malloc(len);
		if (cert_hex) {
			wpa_snprintf_hex(cert_hex, len, wpabuf_head(cert), wpabuf_len(cert));
			wpa_msg_ctrl(wpa_s, MSG_INFO, WPA_EVENT_EAP_PEER_CERT "depth=%d subject='%s' cert=%s", depth, subject, cert_hex);
			os_free(cert_hex);
		}
	}

	if (altsubject) {
		int i;

		for (i = 0; i < num_altsubject; i++) {
			wpa_msg(wpa_s, MSG_INFO, WPA_EVENT_EAP_PEER_ALT "depth=%d %s", depth, altsubject[i]);
		}
	}
}

void wpas_notify_preq(struct wpa_supplicant *wpa_s, const u8 *addr, const u8 *dst, const u8 *bssid, const u8 *ie, size_t ie_len, u32 ssi_signal)
{
}

void wpas_notify_eap_status(struct wpa_supplicant *wpa_s, const char *status, const char *parameter)
{
	wpa_msg_ctrl(wpa_s, MSG_INFO, WPA_EVENT_EAP_STATUS "status='%s' parameter='%s'", status, parameter);
}

void wpas_notify_network_bssid_set_changed(struct wpa_supplicant *wpa_s, struct wpa_ssid *ssid)
{
	if (wpa_s->current_ssid != ssid) {
		return;
	}

	wpa_dbg(wpa_s, MSG_DEBUG, "Network bssid config changed for the current network - within-ESS roaming %s", ssid->bssid_set ? "disabled" : "enabled");

	wpa_drv_roaming(wpa_s, !ssid->bssid_set, ssid->bssid_set ? ssid->bssid : NULL);
}

void wpas_notify_network_type_changed(struct wpa_supplicant *wpa_s, struct wpa_ssid *ssid)
{
#ifdef CONFIG_P2P
	if (ssid->disabled == 2) {
		/* Changed from normal network profile to persistent group */
		ssid->disabled = 0;
		ssid->disabled = 2;
		ssid->p2p_persistent_group = 1;
	} else {
		/* Changed from persistent group to normal network profile */
		ssid->p2p_persistent_group = 0;
	}
#endif							/* CONFIG_P2P */
}
