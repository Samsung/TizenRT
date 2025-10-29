/*
 * WPA Supplicant / Configuration backend: empty starting point
 * Copyright (c) 2003-2005, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 *
 * This file implements stub example of a configuration backend. None of the
 * functions are actually implemented so this can be used as a simple
 * compilation test or a starting point for a new configuration backend.
 */

#include "../src/utils/includes.h"

#include "../src/utils/common.h"
#include "config.h"
#include "../src/utils/base64.h"
#include "bk_wifi_types.h"
#include "modules/wifi.h"
#include "bk_wifi.h"
#include "../src/common/wpa_common.h"
#include "bk_wifi_types.h"
#include <components/event.h>
#include <modules/wifi.h>
#include "wpa_supplicant_i.h"
#include "bss.h"

#if !CONFIG_QUICK_TRACK
/**
 * wpa_config_set_akm update ssid's key_mgmt based on bss's IEs. If CONFIG_WPA2_PREFER_TO_SAE
 * is enabled, wpa2(prefer to use non-mfp) will be used instead of wpa3-sae if bss is
 * in WPA3 transistion mode, and wpa3 always sets to mfp capable to avoid disconnection with
 * reason code set to "Robust managment frame policy viloation (0x001f)" even if bss sets mfp
 * required bit to 0 (No Required).
 */
static int wpa_config_set_akm(struct wpa_ssid *ssid, struct wpa_ie_data *ie)
{
#ifdef CONFIG_WPA2_PREFER_TO_SAE
	bool bss_mfp_required = !!(ie->capabilities & WPA_CAPABILITY_MFPR);
#endif

	ssid->key_mgmt |= WPA_KEY_MGMT_PSK;
#ifdef CONFIG_IEEE80211W
	ssid->key_mgmt |= WPA_KEY_MGMT_PSK_SHA256;
#endif
#ifdef CONFIG_SAE
#if CONFIG_WPA2_PREFER_TO_SAE
	if (ie->key_mgmt & WPA_KEY_MGMT_PSK) {
		/* if bss supports WPA2, disable WPA3-SAE, and don't use mfp if bss doesn't requires */
		ssid->key_mgmt &= ~WPA_KEY_MGMT_SAE;
		ssid->ieee80211w = bss_mfp_required ? MGMT_FRAME_PROTECTION_OPTIONAL : NO_MGMT_FRAME_PROTECTION;
	} else {
		/* for other akm, set all supported akm, and update mfp */
		ssid->key_mgmt |= WPA_KEY_MGMT_SAE;
		/* even if RSN indicates it's mfp capable and not required, in SAE akm, cannot disable mfp */
		ssid->ieee80211w = MGMT_FRAME_PROTECTION_OPTIONAL;
	}
#else /* CONFIG_WPA2_PREFER_TO_SAE */
	ssid->key_mgmt |= WPA_KEY_MGMT_SAE;
#endif /* CONFIG_WPA2_PREFER_TO_SAE */
#endif /* CONFIG_SAE */
#ifdef IEEE8021X_EAPOL
	ssid->key_mgmt |= WPA_KEY_MGMT_IEEE8021X;
#endif
#ifdef CONFIG_OWE
	ssid->key_mgmt |= WPA_KEY_MGMT_OWE;
#endif
#ifdef CONFIG_IEEE80211R
	ssid->key_mgmt |= WPA_KEY_MGMT_FT;
#endif
	/* key_mgmt that STA and AP both support */
	ssid->key_mgmt &= ie->key_mgmt;

	return 0;
}

static int set_wpa_psk(struct wpa_ssid *ssid, struct wpa_ie_data *ie)
{
	int errors = 0;

	//if bss is OWE or SAE only, return directlly
	if (ie->key_mgmt == WPA_KEY_MGMT_OWE || ie->key_mgmt == WPA_KEY_MGMT_SAE) {
	    return errors;
	} else if (g_sta_param_ptr->key_len < 8 || g_sta_param_ptr->key_len > 64) {
		   WPA_LOGE("Invalid passphrase " "length %lu (expected: 8..63) '%s'.",
                   (unsigned long) g_sta_param_ptr->key_len, (char *)g_sta_param_ptr->key);
		   errors++;
		   return errors;
	} else if (g_sta_param_ptr->key_len == 64) {
		wpa_printf(MSG_ERROR, "use PSK");
		if (hexstr2bin((char*)(g_sta_param_ptr->key), ssid->psk, 32) ||
			g_sta_param_ptr->key[64] != '\0') {
			wpa_printf(MSG_ERROR, "Invalid PSK '%s'.",
					   g_sta_param_ptr->key);
			errors++;
		}
		ssid->psk_set = 1;
	}

	return errors;
}

static int set_wep_key(struct wpa_ssid *ssid)
{
	int errors = 0;

	ssid->wep_tx_keyidx = 0;
#ifdef CONFIG_IEEE80211W
	ssid->ieee80211w = MGMT_FRAME_PROTECTION_OPTIONAL;
#endif
	ssid->proto = DEFAULT_PROTO;
	ssid->group_cipher = DEFAULT_GROUP;
	ssid->auth_alg = WPA_AUTH_ALG_OPEN | WPA_AUTH_ALG_SHARED;
	if (g_sta_param_ptr->key_len == 5 ||
		g_sta_param_ptr->key_len == 13) {
		os_memcpy(ssid->wep_key[0], (char *)g_sta_param_ptr->key, g_sta_param_ptr->key_len);
		ssid->wep_key_len[0] = g_sta_param_ptr->key_len;
	} else if (g_sta_param_ptr->key_len == 10 ||
			   g_sta_param_ptr->key_len == 26) {
		ssid->wep_key_len[0] = g_sta_param_ptr->key_len / 2;
		hexstr2bin((char*)(g_sta_param_ptr->key), ssid->wep_key[0], ssid->wep_key_len[0]);
	} else {
		errors++;
	}
	ssid->mem_only_psk = 0;
	return errors;
}

int wpa_config_set_none(struct wpa_ssid *ssid)
{
	ssid->key_mgmt = WPA_KEY_MGMT_NONE;
	ssid->mem_only_psk = 0;
	ssid->auth_alg = 0;
	g_sta_param_ptr->cipher_suite = BK_SECURITY_TYPE_NONE;
	ssid->wep_key[0][0] = 0;
	ssid->wep_key_len[0] = 0;
#ifdef CONFIG_IEEE80211W
	ssid->ieee80211w = MGMT_FRAME_PROTECTION_OPTIONAL;
#endif
	ssid->proto = DEFAULT_PROTO;
	ssid->group_cipher = DEFAULT_GROUP;

	return 0;
}

int wpa_config_set_wep(struct wpa_ssid *ssid)
{
	int ret;

	ret = set_wep_key(ssid);
	if (!ret) {
		g_sta_param_ptr->cipher_suite = BK_SECURITY_TYPE_WEP;
	} else {
		wifi_linkstate_reason_t info;
		wifi_event_sta_disconnected_t sta_disconnected = {0};

		/* set mac status */
		info.state = WIFI_LINKSTATE_STA_DISCONNECTED;
		info.reason_code= WIFI_REASON_WRONG_PASSWORD;
		mhdr_set_station_status(info);

		/* post event */
		sta_disconnected.disconnect_reason = info.reason_code;
		sta_disconnected.local_generated = true;
		bk_event_post(EVENT_MOD_WIFI, EVENT_WIFI_STA_DISCONNECTED,
						&sta_disconnected, sizeof(sta_disconnected), BEKEN_NEVER_TIMEOUT);
	}

	return 0;
}


int wpa_config_set_wpa(struct wpa_bss *bss, struct wpa_ssid *ssid, struct wpa_ie_data *ie)
{
	int ret = 0;

	if (0 == (ssid->group_cipher & ie->group_cipher))
		ssid->group_cipher = ie->group_cipher;
	if (0 == (ssid->pairwise_cipher & ie->pairwise_cipher))
		ssid->pairwise_cipher = ie->pairwise_cipher;
	if (0 == (ssid->key_mgmt & ie->key_mgmt))
		ssid->key_mgmt = ie->key_mgmt;
	if (0 == (ssid->proto & ie->proto))
		ssid->proto = ie->proto;
	ssid->auth_alg = 0;	// let supplicant choose automatically.
#ifdef CONFIG_IEEE80211W
	ssid->ieee80211w = MGMT_FRAME_PROTECTION_OPTIONAL;
#endif
	ssid->proto = DEFAULT_PROTO;
	ssid->group_cipher = DEFAULT_GROUP;

	wpa_config_set_akm(ssid, ie);

#ifdef IEEE8021X_EAPOL
	if (wpa_key_mgmt_wpa_ieee8021x(ie->key_mgmt))
		return 0;
#endif

	if (ssid->psk_set) {
		WPA_LOGD("cipher2security %d %d %d %d\n", ie->key_mgmt, ie->proto, ie->pairwise_cipher, ie->group_cipher);
		g_sta_param_ptr->cipher_suite = get_security_type_from_ie(bss->ies, bss->ie_len, bss->caps);
		return 0;
	}

	ret = set_wpa_psk(ssid, ie);
	if (!ret) {
		WPA_LOGD("cipher2security %d %d %d %d\n", ie->key_mgmt, ie->proto, ie->pairwise_cipher, ie->group_cipher);
		g_sta_param_ptr->cipher_suite = get_security_type_from_ie(bss->ies, bss->ie_len, bss->caps);

		if (ssid->passphrase && (ssid->psk_set == 0)) {
			if (!wpa_config_update_psk(ssid)) {
				ssid->psk_set = 1;
				ssid->mem_only_psk = 0;
			}
		}
	} else {
		wifi_linkstate_reason_t info;
		wifi_event_sta_disconnected_t sta_disconnected = {0};

		/* set mac status */
		info.state = WIFI_LINKSTATE_STA_DISCONNECTED;
		info.reason_code = WIFI_REASON_WRONG_PASSWORD;
		mhdr_set_station_status(info);

		/* post event */
		sta_disconnected.disconnect_reason = info.reason_code;
		sta_disconnected.local_generated = true;
		BK_LOG_ON_ERR(bk_event_post(EVENT_MOD_WIFI, EVENT_WIFI_STA_DISCONNECTED,
					  &sta_disconnected, sizeof(sta_disconnected), BEKEN_NEVER_TIMEOUT));
	}

	return ret;
}
#endif /* !CONFIG_QUICK_TRACK */

static struct wpa_ssid *wpa_config_read_network(int *line, int id)
{
	struct wpa_ssid *ssid;

	ssid = os_zalloc(sizeof(*ssid));
	if (ssid == NULL)
		return NULL;

	dl_list_init(&ssid->psk_list);
	ssid->id = id;

	wpa_config_set_network_defaults(ssid);

	ssid->disabled = 1;		/* disable scan @ wpa_supplicant_driver_init */
#ifdef CONFIG_IEEE80211W
	ssid->ieee80211w = MGMT_FRAME_PROTECTION_OPTIONAL;
#endif

#if CONFIG_WFA_CERT
	ssid->ssid = (u8 *)dup_binstr("Wi-Fi", 5);
	ssid->ssid_len = 5;
	ssid->passphrase = os_strdup("12345678");
#ifdef CONFIG_IEEE80211W
	ssid->ieee80211w = MGMT_FRAME_PROTECTION_REQUIRED;
#endif
	ssid->scan_ssid = 1;
	ssid->mem_only_psk = 1;
#endif

	return ssid;
}

struct wpa_config *wpa_config_read(const char *name, struct wpa_config *cfgp)
{
	struct wpa_config *config;
	struct wpa_ssid *ssid, *tail, *head;
	int id = 0, line = 0, errors = 0;

	if (cfgp)
		config = cfgp;
	else
		config = wpa_config_alloc_empty(NULL, NULL);
	if (config == NULL)
		return NULL;
	tail = head = config->ssid;
	while (tail && tail->next)
		tail = tail->next;

	ssid = wpa_config_read_network(&line, id++);
	if (ssid == NULL) {
		errors++;
		goto error;
	}
	if (head == NULL) {
		head = tail = ssid;
	} else {
		tail->next = ssid;
		tail = ssid;
	}
	if (wpa_config_add_prio_network(config, ssid)) {
		errors++;
		goto error;
	}

	config->ssid = head;
	wpa_config_debug_dump_networks(config);
	config->auth_dur = 3;
	//config->pmf = MGMT_FRAME_PROTECTION_OPTIONAL;

#ifdef CONFIG_P2P
	config->config_methods = os_strdup("push_button p2ps");

	//Temporarily enable persistent_reconnect to support invitation
	config->persistent_reconnect = 1;
#endif

#ifdef CONFIG_IEEE80211R
	/* set bgscan parameters : short_interval, signal_threshold, long_interval
	   scan mode: learn, scan short_interval 10 seconds, signal_threshold -60 db,
	   long_interval 40 seconds */
	config->bgscan = os_strdup("learn:10:-60:40");
#endif

	config->sae_pwe = 2;
error:
	if (errors) {
		wpa_config_free(config);
		config = NULL;
		head = NULL;
	}
	return config;
}


int wpa_config_write(const char *name, struct wpa_config *config)
{
	//struct wpa_ssid *ssid;
	//struct wpa_config_blob *blob;

	wpa_printf(MSG_DEBUG, "Writing configuration file '%s'", name);

	/* TODO: write global config parameters */


#if 0
	for (ssid = config->ssid; ssid; ssid = ssid->next) {
		/* TODO: write networks */
	}

	for (blob = config->blobs; blob; blob = blob->next) {
		/* TODO: write blobs */
	}
#endif

	return 0;
}
// eof


