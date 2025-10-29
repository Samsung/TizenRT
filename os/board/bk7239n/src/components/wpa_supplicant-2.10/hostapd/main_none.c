/*
 * hostapd / main()
 * Copyright (c) 2002-2022, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */
#include "../src/utils/includes.h"
#include "hostapd_cfg.h"
#include "../wpa_supplicant-2.10/src/utils/common.h"
#include "../src/ap/hostapd.h"
#include "../src/utils/eloop.h"
#include "main_none.h"
#include "../src/ap/sta_info.h"
//#include "ps.h"
#include "bk_wifi_types.h"
#include "../src/utils/os.h"
#include <common/bk_kernel_err.h>
#include "modules/wifi.h"
#include "bk_wifi.h"
#include "../src/ap/ap_drv_ops.h"
#include "../src/common/eapol_common.h"
#include "signal.h"
#include "../src/crypto/sha1.h"

#include "../src/common/wpa_ctrl.h"
#include "../bk_patch/wpa_err.h"

beken_queue_t wpah_queue = NULL;
static struct hapd_global s_hapd_global;
struct hapd_interfaces g_hapd_interfaces;

const char *bss_iface = "wlan0";

extern int ap_channel_switch(struct hostapd_iface *ap_iface, int new_freq);

struct hapd_interfaces *hostapd_ctrl_get_interfaces()
{
	return &g_hapd_interfaces;
}

int hostap_interfaces_is_valid(void)
{
	return ((g_hapd_interfaces.iface) && (0 < g_hapd_interfaces.count));
}

struct hostapd_iface **hostapd_ctrl_get_hostapd_iface(void)
{
	return g_hapd_interfaces.iface;
}

int hostapd_ctrl_get_hostapd_iface_count(void)
{
	return g_hapd_interfaces.count;
}

struct hostapd_config *hostapd_config_read(const char *fname)
{
	struct hostapd_config *conf = 0;
	int i;
	int errors = 0;
	struct hostapd_bss_config *bss;
#if CONFIG_WIFI_AP_CUSTOM_RATES
	int len;
#endif

	conf = hostapd_config_defaults();
	if (NULL == conf)
		return NULL;

	conf->last_bss = conf->bss[0];
	bss = conf->last_bss;

	os_strcpy(bss->iface, bss_iface);
	bk_wifi_ap_get_mac((uint8_t *)&bss->bssid);
	/* set default driver based on configuration */
	conf->driver = wpa_drivers[0];
	conf->last_bss = conf->bss[0];
	conf->channel = g_ap_param_ptr->chann;

	if (conf->channel >= 1 && conf->channel<= 14)
		conf->hw_mode = HOSTAPD_MODE_IEEE80211G;
#if CONFIG_SOC_BK7239XX || CONFIG_SOC_BK7286XX
	else if (conf->channel >= 36 && conf->channel<= 177)
		conf->hw_mode = HOSTAPD_MODE_IEEE80211A;
#endif
#if CONFIG_AP_HT_IE
	conf->ieee80211n = 1;
	conf->ht_capab = HT_CAP_INFO_SMPS_DISABLED | HT_CAP_INFO_SHORT_GI20MHZ
						| HT_CAP_INFO_TX_STBC
						| HT_CAP_INFO_RX_STBC_1
						| HT_CAP_INFO_MAX_AMSDU_SIZE;
#endif

#if CONFIG_WIFI_AP_HW_MODE
	switch (g_ap_param_ptr->hw_mode) {
	default:
		/* fall through, select first supported hw mode */
#if CONFIG_AP_HT_IE
	case BK_HW_MODE_BGN:
		conf->hw_mode = HOSTAPD_MODE_IEEE80211G;
		conf->ieee80211n = 1;
		conf->ieee80211ac = 0;
		conf->ieee80211ax = 0;
		break;
#endif
	case BK_HW_MODE_BG:
		conf->hw_mode = HOSTAPD_MODE_IEEE80211G;
		conf->ieee80211n = 0;
		conf->ieee80211ac = 0;
		conf->ieee80211ax = 0;
		break;
	case BK_HW_MODE_B:
		conf->hw_mode = HOSTAPD_MODE_IEEE80211B;
		conf->ieee80211n = 0;
		conf->ieee80211ac = 0;
		conf->ieee80211ax = 0;
		break;
	}
#endif

#if CONFIG_AP_VHT
	conf->ieee80211ac = 1;
#endif

#if CONFIG_AP_HE
	conf->ieee80211ax = 1;
#endif

	bss->ssid.ssid_len = g_ap_param_ptr->ssid.length;
	os_memcpy(bss->ssid.ssid, g_ap_param_ptr->ssid.array, bss->ssid.ssid_len);
	bss->max_listen_interval = 65535;
	bss->ieee802_1x = 0;
	bss->ssid.ssid_set = 1;
	bss->ignore_broadcast_ssid = g_ap_param_ptr->hidden_ssid;

	if (g_ap_param_ptr->cipher_suite == BK_SECURITY_TYPE_WEP) {
#ifdef CONFIG_WEP_AP
		bss->default_wep_key_len = 0;
		bss->ssid.wep.keys_set = 1;
		bss->ssid.wep.default_len = 10;
		bss->ssid.wep.idx = 0;
		bss->ssid.wep.len[0] = 5;
		bss->ssid.wep.key[0] = (u8 *)os_malloc(bss->ssid.wep.len[0]);
		if (bss->ssid.wep.key[0]) {
			int wkey;
			const char *wep_key = (char *)g_ap_param_ptr->key;
			if (g_ap_param_ptr->key_len == 5) {
				os_memcpy(bss->ssid.wep.key[0], wep_key, g_ap_param_ptr->key_len);
			} else if (g_ap_param_ptr->key_len == 10) {
				for (i = 0; i < bss->ssid.wep.len[0]; i ++) {
					wkey = hex2byte(&wep_key[2 * i]);
					BK_ASSERT(-1 != wkey); /* ASSERT VERIFIED */

					bss->ssid.wep.key[0][i] = wkey;
				}
			} else {
				WPA_LOGE("WEP_KEY_len_exception\r\n");
			}
		}
#endif
	} else if (g_ap_param_ptr->cipher_suite == BK_SECURITY_TYPE_WPA_TKIP) {
		bss->wpa = 1;
		bss->wpa_pairwise = WPA_CIPHER_TKIP;
	} else if (g_ap_param_ptr->cipher_suite == BK_SECURITY_TYPE_WPA2_AES) {
		bss->wpa = 2;
		bss->wpa_pairwise = WPA_CIPHER_CCMP;
	} else if (g_ap_param_ptr->cipher_suite == BK_SECURITY_TYPE_WPA2_MIXED) {
		bss->wpa = 2;
		bss->wpa_pairwise = WPA_CIPHER_TKIP | WPA_CIPHER_CCMP;
	} else if (g_ap_param_ptr->cipher_suite == BK_SECURITY_TYPE_WPA3_SAE ||
		g_ap_param_ptr->cipher_suite == BK_SECURITY_TYPE_WPA3_WPA2_MIXED) {
		bss->wpa = 2;
		bss->wpa_pairwise = WPA_CIPHER_CCMP;
	}

	bss->wpa_key_mgmt = 0;
	if (g_ap_param_ptr->cipher_suite > BK_SECURITY_TYPE_WEP) {
		const char *wpa_key = (char *)g_ap_param_ptr->key;
		if (os_strlen(wpa_key) == 2 * PMK_LEN) {
			WPA_LOGD("Use PSK instead of passphrase for softap\n");
			hostapd_config_clear_wpa_psk(&bss->ssid.wpa_psk);
			bss->ssid.wpa_psk = os_zalloc(sizeof(struct hostapd_wpa_psk));
			if (!bss->ssid.wpa_psk) {
				WPA_LOGE("%s: OOM\n", __FUNCTION__);
				return NULL;
			}
			if (hexstr2bin(wpa_key, bss->ssid.wpa_psk->psk, PMK_LEN)) {
				WPA_LOGE("Key contains non-hex value\n");
				hostapd_config_clear_wpa_psk(&bss->ssid.wpa_psk);
				return NULL;
			}
			bss->ssid.wpa_psk->group = 1;
			os_free(bss->ssid.wpa_passphrase);
			bss->ssid.wpa_passphrase = NULL;
			bss->ssid.wpa_psk_set = 1;
		} else {
			os_free(bss->ssid.wpa_passphrase);
			bss->ssid.wpa_passphrase = os_strdup(wpa_key);
			if (bss->ssid.wpa_passphrase) {
				hostapd_config_clear_wpa_psk(&bss->ssid.wpa_psk);
				bss->ssid.wpa_passphrase_set = 1;
			}
		}
		bss->wpa_key_mgmt |= WPA_KEY_MGMT_PSK;
	}

#if CONFIG_AP_VSIE
	if (g_ap_param_ptr->vsie_len) {
		bss->vendor_elements = wpabuf_alloc_copy(g_ap_param_ptr->vsie, g_ap_param_ptr->vsie_len);
		if (!bss->vendor_elements) {
			wpa_printf(MSG_ERROR, "%s OOM\n", __func__);
			errors++;
		}
	}
#endif

#if CONFIG_WIFI_AP_CUSTOM_RATES
	/* read basic rates */
	len = int_array_len(g_ap_param_ptr->basic_rates);
	if (len > 0) {
		conf->basic_rates = os_zalloc((len + 1) * sizeof(int));
		if (conf->basic_rates) {
			for (i = 0; i < len; i++)
				conf->basic_rates[i] = g_ap_param_ptr->basic_rates[i];
			/* mark last one be -1 */
			conf->basic_rates[len] = -1;
		}
	}

	/* read supported rates */
	len = int_array_len(g_ap_param_ptr->supported_rates);
	if (len > 0) {
		conf->supported_rates = os_zalloc((len + 1) * sizeof(int));
		if (conf->supported_rates) {
			for (i = 0; i < len; i++)
				conf->supported_rates[i] = g_ap_param_ptr->supported_rates[i];
			/* mark last one be -1 */
			conf->supported_rates[len] = -1;
		}
	}

	/* read mcs set */
	len = char_array_len(g_ap_param_ptr->mcs_set);
	if (len > 0) {
		conf->mcs_set = os_memdup(g_ap_param_ptr->mcs_set,
			sizeof(g_ap_param_ptr->mcs_set));
	}
#endif
#ifdef CONFIG_IEEE80211W_AP
	if (g_ap_param_ptr->cipher_suite == BK_SECURITY_TYPE_WPA3_SAE) {
		bss->wpa_key_mgmt = WPA_KEY_MGMT_SAE;
		bss->ieee80211w = MGMT_FRAME_PROTECTION_REQUIRED;
	} else if (g_ap_param_ptr->cipher_suite == BK_SECURITY_TYPE_WPA3_WPA2_MIXED) {
		bss->wpa_key_mgmt |= WPA_KEY_MGMT_SAE;
		bss->ieee80211w = MGMT_FRAME_PROTECTION_REQUIRED;
	}
#endif
	//WPA_LOGI("%s: wpa_key_mgmt 0x%x, cipher_suite %d\n", __func__,
	//			bss->wpa_key_mgmt, g_ap_param_ptr->cipher_suite);

	for (i = 0; i < conf->num_bss; i++)
		hostapd_set_security_params(conf->bss[i], 1);

	if (hostapd_config_check(conf, 1))
		errors++;

	return conf;
}

static int hostapd_config_parse_key_mgmt(int line, const char *value)
{
	int val = 0, last;
	char *start, *end, *buf;

	buf = os_strdup(value);
	if (buf == NULL)
		return -1;
	start = buf;

	while (*start != '\0') {
		while (*start == ' ' || *start == '\t')
			start++;
		if (*start == '\0')
			break;
		end = start;
		while (*end != ' ' && *end != '\t' && *end != '\0')
			end++;
		last = *end == '\0';
		*end = '\0';
		if (os_strcmp(start, "WPA-PSK") == 0)
			val |= WPA_KEY_MGMT_PSK;
		else if (os_strcmp(start, "WPA-EAP") == 0)
			val |= WPA_KEY_MGMT_IEEE8021X;
#ifdef CONFIG_SAE_AP
		else if (os_strcmp(start, "SAE") == 0)
			val |= WPA_KEY_MGMT_SAE;
		else if (os_strcmp(start, "FT-SAE") == 0)
			val |= WPA_KEY_MGMT_FT_SAE;
#endif /* CONFIG_SAE_AP */
		else {
			wpa_printf(MSG_ERROR, "Line %d: invalid key_mgmt '%s'",
				   line, start);
			os_free(buf);
			return -1;
		}

		if (last)
			break;
		start = end + 1;
	}

	os_free(buf);
	if (val == 0) {
		wpa_printf(MSG_ERROR, "Line %d: no key_mgmt values "
			   "configured.", line);
		return -1;
	}

	return val;
}

static int hostapd_config_parse_cipher(int line, const char *value)
{
	int val = wpa_parse_cipher(value);
	if (val < 0) {
		wpa_printf(MSG_ERROR, "Line %d: invalid cipher '%s'.",
			   line, value);
		return -1;
	}
	if (val == 0) {
		wpa_printf(MSG_ERROR, "Line %d: no cipher values configured.",
			   line);
		return -1;
	}
	return val;
}


#ifdef CONFIG_WEP_AP
static int hostapd_config_read_wep(struct hostapd_wep_keys *wep, int keyidx, char *val)
{
	size_t len = os_strlen(val);

	if (keyidx < 0 || keyidx > 3 || wep->key[keyidx] != NULL)
		return -1;

	if (val[0] == '"') {
		if (len < 2 || val[len - 1] != '"')
			return -1;
		len -= 2;
		wep->key[keyidx] = os_malloc(len);
		if (wep->key[keyidx] == NULL)
			return -1;
		os_memcpy(wep->key[keyidx], val + 1, len);
		wep->len[keyidx] = len;
	} else {
		if (len & 1)
			return -1;
		len /= 2;
		wep->key[keyidx] = os_malloc(len);
		if (wep->key[keyidx] == NULL)
			return -1;
		wep->len[keyidx] = len;
		if (hexstr2bin(val, wep->key[keyidx], len) < 0)
			return -1;
	}

	wep->keys_set++;

	return 0;
}
#endif


__maybe_unused static int hostapd_parse_chanlist(struct hostapd_config *conf, char *val);
static int hostapd_parse_chanlist(struct hostapd_config *conf, char *val)
{
	char *pos;

	/* for backwards compatibility, translate ' ' in conf str to ',' */
	pos = val;
	while (pos) {
		pos = os_strchr(pos, ' ');
		if (pos)
			*pos++ = ',';
	}
	if (freq_range_list_parse(&conf->acs_ch_list, val))
		return -1;

	return 0;
}

static int hostapd_parse_intlist(int **int_list, char *val)
{
	int *list;
	int count;
	char *pos, *end;

	os_free(*int_list);
	*int_list = NULL;

	pos = val;
	count = 0;
	while (*pos != '\0') {
		if (*pos == ' ')
			count++;
		pos++;
	}

	list = os_malloc(sizeof(int) * (count + 2));
	if (list == NULL)
		return -1;
	pos = val;
	count = 0;
	while (*pos != '\0') {
		end = os_strchr(pos, ' ');
		if (end)
			*end = '\0';

		list[count++] = atoi(pos);
		if (!end)
			break;
		pos = end + 1;
	}
	list[count] = -1;

	*int_list = list;
	return 0;
}

static int hostapd_config_fill(struct hostapd_config *conf,
			       struct hostapd_bss_config *bss,
			       const char *buf, char *pos, int line)
{
	if (os_strcmp(buf, "ssid") == 0) {
		bss->ssid.ssid_len = os_strlen(pos);
		if (bss->ssid.ssid_len > SSID_MAX_LEN ||
		    bss->ssid.ssid_len < 1) {
			wpa_printf(MSG_ERROR, "Line %d: invalid SSID '%s'",
				   line, pos);
			return 1;
		}
		os_memcpy(bss->ssid.ssid, pos, bss->ssid.ssid_len);
		bss->ssid.ssid_set = 1;
	} else if (os_strcmp(buf, "utf8_ssid") == 0) {
		bss->ssid.utf8_ssid = atoi(pos) > 0;
	} else if (os_strcmp(buf, "ap_isolate") == 0) {
		bss->isolate = atoi(pos);
	} else if (os_strcmp(buf, "ap_max_inactivity") == 0) {
		bss->ap_max_inactivity = atoi(pos);
	} else if (os_strcmp(buf, "skip_inactivity_poll") == 0) {
		bss->skip_inactivity_poll = atoi(pos);
	} else if (os_strcmp(buf, "country_code") == 0) {
		os_memcpy(conf->country, pos, 2);
		/* FIX: make this configurable */
		conf->country[2] = ' ';
	} else if (os_strcmp(buf, "ieee80211d") == 0) {
		conf->ieee80211d = atoi(pos);
	} else if (os_strcmp(buf, "ieee80211h") == 0) {
		conf->ieee80211h = atoi(pos);
	} else if (os_strcmp(buf, "eapol_version") == 0) {
		bss->eapol_version = atoi(pos);
		if (bss->eapol_version < 1 || bss->eapol_version > 2) {
			wpa_printf(MSG_ERROR,
				   "Line %d: invalid EAPOL version (%d): '%s'.",
				   line, bss->eapol_version, pos);
			return 1;
		}
		wpa_printf(MSG_DEBUG, "eapol_version=%d", bss->eapol_version);
#ifdef CONFIG_WEP_AP
	} else if (os_strcmp(buf, "wep_rekey_period") == 0) {
		bss->wep_rekeying_period = atoi(pos);
		if (bss->wep_rekeying_period < 0) {
			wpa_printf(MSG_ERROR, "Line %d: invalid period %d",
				   line, bss->wep_rekeying_period);
			return 1;
		}
#endif
	} else if (os_strcmp(buf, "eap_reauth_period") == 0) {
		bss->eap_reauth_period = atoi(pos);
		if (bss->eap_reauth_period < 0) {
			wpa_printf(MSG_ERROR, "Line %d: invalid period %d",
				   line, bss->eap_reauth_period);
			return 1;
		}
	} else if (os_strcmp(buf, "eapol_key_index_workaround") == 0) {
		bss->eapol_key_index_workaround = atoi(pos);
	} else if (os_strcmp(buf, "auth_algs") == 0) {
		bss->auth_algs = atoi(pos);
		if (bss->auth_algs == 0) {
			wpa_printf(MSG_ERROR, "Line %d: no authentication algorithms allowed",
				   line);
			return 1;
		}
	} else if (os_strcmp(buf, "max_num_sta") == 0) {
		bss->max_num_sta = atoi(pos);
		if (bss->max_num_sta < 0 ||
		    bss->max_num_sta > MAX_STA_COUNT) {
			wpa_printf(MSG_ERROR, "Line %d: Invalid max_num_sta=%d; allowed range 0..%d",
				   line, bss->max_num_sta, MAX_STA_COUNT);
			return 1;
		}
	} else if (os_strcmp(buf, "wpa") == 0) {
		bss->wpa = atoi(pos);
	} else if (os_strcmp(buf, "wpa_group_rekey") == 0) {
		bss->wpa_group_rekey = atoi(pos);
	} else if (os_strcmp(buf, "wpa_strict_rekey") == 0) {
		bss->wpa_strict_rekey = atoi(pos);
	} else if (os_strcmp(buf, "wpa_gmk_rekey") == 0) {
		bss->wpa_gmk_rekey = atoi(pos);
	} else if (os_strcmp(buf, "wpa_ptk_rekey") == 0) {
		bss->wpa_ptk_rekey = atoi(pos);
	} else if (os_strcmp(buf, "wpa_passphrase") == 0) {
		int len = os_strlen(pos);
		if (len < 8 || len > 63) {
			wpa_printf(MSG_ERROR, "Line %d: invalid WPA passphrase length %d (expected 8..63)",
				   line, len);
			return 1;
		}
		os_free(bss->ssid.wpa_passphrase);
		bss->ssid.wpa_passphrase = os_strdup(pos);
		if (bss->ssid.wpa_passphrase) {
			hostapd_config_clear_wpa_psk(&bss->ssid.wpa_psk);
			bss->ssid.wpa_passphrase_set = 1;
		}
	} else if (os_strcmp(buf, "wpa_psk") == 0) {
		hostapd_config_clear_wpa_psk(&bss->ssid.wpa_psk);
		bss->ssid.wpa_psk = os_zalloc(sizeof(struct hostapd_wpa_psk));
		if (bss->ssid.wpa_psk == NULL)
			return 1;
		if (hexstr2bin(pos, bss->ssid.wpa_psk->psk, PMK_LEN) ||
		    pos[PMK_LEN * 2] != '\0') {
			wpa_printf(MSG_ERROR, "Line %d: Invalid PSK '%s'.",
				   line, pos);
			hostapd_config_clear_wpa_psk(&bss->ssid.wpa_psk);
			return 1;
		}
		bss->ssid.wpa_psk->group = 1;
		os_free(bss->ssid.wpa_passphrase);
		bss->ssid.wpa_passphrase = NULL;
		bss->ssid.wpa_psk_set = 1;
	} else if (os_strcmp(buf, "wpa_key_mgmt") == 0) {
		bss->wpa_key_mgmt = hostapd_config_parse_key_mgmt(line, pos);
		if (bss->wpa_key_mgmt == -1)
			return 1;
	} else if (os_strcmp(buf, "wpa_pairwise") == 0) {
		bss->wpa_pairwise = hostapd_config_parse_cipher(line, pos);
		if (bss->wpa_pairwise == -1 || bss->wpa_pairwise == 0)
			return 1;
		if (bss->wpa_pairwise &
		    (WPA_CIPHER_NONE | WPA_CIPHER_WEP40 | WPA_CIPHER_WEP104)) {
			wpa_printf(MSG_ERROR, "Line %d: unsupported pairwise cipher suite '%s'",
				   bss->wpa_pairwise, pos);
			return 1;
		}
	} else if (os_strcmp(buf, "rsn_pairwise") == 0) {
		bss->rsn_pairwise = hostapd_config_parse_cipher(line, pos);
		if (bss->rsn_pairwise == -1 || bss->rsn_pairwise == 0)
			return 1;
		if (bss->rsn_pairwise &
		    (WPA_CIPHER_NONE | WPA_CIPHER_WEP40 | WPA_CIPHER_WEP104)) {
			wpa_printf(MSG_ERROR, "Line %d: unsupported pairwise cipher suite '%s'",
				   bss->rsn_pairwise, pos);
			return 1;
		}
#ifdef CONFIG_PEERKEY
	} else if (os_strcmp(buf, "peerkey") == 0) {
		bss->peerkey = atoi(pos);
#endif /* CONFIG_PEERKEY */
	} else if (os_strcmp(buf, "use_pae_group_addr") == 0) {
		bss->use_pae_group_addr = atoi(pos);
	} else if (os_strcmp(buf, "hw_mode") == 0) {
		if (0) {}
#ifdef CONFIG_FULL_HOSTAPD
		else if (os_strcmp(pos, "a") == 0)
			conf->hw_mode = HOSTAPD_MODE_IEEE80211A;
#endif
		else if (os_strcmp(pos, "b") == 0)
			conf->hw_mode = HOSTAPD_MODE_IEEE80211B;
		else if (os_strcmp(pos, "g") == 0)
			conf->hw_mode = HOSTAPD_MODE_IEEE80211G;
#ifdef CONFIG_FULL_HOSTAPD
		else if (os_strcmp(pos, "ad") == 0)
			conf->hw_mode = HOSTAPD_MODE_IEEE80211AD;
#endif
		else if (os_strcmp(pos, "any") == 0)
			conf->hw_mode = HOSTAPD_MODE_IEEE80211ANY;
		else {
			wpa_printf(MSG_ERROR, "Line %d: unknown hw_mode '%s'",
				   line, pos);
			return 1;
		}
#ifdef CONFIG_WPS
	} else if (os_strcmp(buf, "wps_rf_bands") == 0) {
		if (os_strcmp(pos, "ad") == 0)
			bss->wps_rf_bands = WPS_RF_60GHZ;
		else if (os_strcmp(pos, "a") == 0)
			bss->wps_rf_bands = WPS_RF_50GHZ;
		else if (os_strcmp(pos, "g") == 0 ||
			 os_strcmp(pos, "b") == 0)
			bss->wps_rf_bands = WPS_RF_24GHZ;
		else if (os_strcmp(pos, "ag") == 0 ||
			 os_strcmp(pos, "ga") == 0)
			bss->wps_rf_bands = WPS_RF_24GHZ | WPS_RF_50GHZ;
		else {
			wpa_printf(MSG_ERROR,
				   "Line %d: unknown wps_rf_band '%s'",
				   line, pos);
			return 1;
		}
#endif
	} else if (os_strcmp(buf, "channel") == 0) {
		conf->channel = atoi(pos);
		conf->acs = conf->channel == 0;
	} else if (os_strcmp(buf, "beacon_int") == 0) {
		int val = atoi(pos);
		/* MIB defines range as 1..65535, but very small values
		 * cause problems with the current implementation.
		 * Since it is unlikely that this small numbers are
		 * useful in real life scenarios, do not allow beacon
		 * period to be set below 15 TU. */
		if (val < 15 || val > 65535) {
			wpa_printf(MSG_ERROR, "Line %d: invalid beacon_int %d (expected 15..65535)",
				   line, val);
			return 1;
		}
		conf->beacon_int = val;
	} else if (os_strcmp(buf, "dtim_period") == 0) {
		bss->dtim_period = atoi(pos);
		if (bss->dtim_period < 1 || bss->dtim_period > 255) {
			wpa_printf(MSG_ERROR, "Line %d: invalid dtim_period %d",
				   line, bss->dtim_period);
			return 1;
		}
	} else if (os_strcmp(buf, "supported_rates") == 0) {
		if (hostapd_parse_intlist(&conf->supported_rates, pos)) {
			wpa_printf(MSG_ERROR, "Line %d: invalid rate list",
				   line);
			return 1;
		}
	} else if (os_strcmp(buf, "basic_rates") == 0) {
		if (hostapd_parse_intlist(&conf->basic_rates, pos)) {
			wpa_printf(MSG_ERROR, "Line %d: invalid rate list",
				   line);
			return 1;
		}
	} else if (os_strcmp(buf, "ignore_broadcast_ssid") == 0) {
		bss->ignore_broadcast_ssid = atoi(pos);
#ifdef CONFIG_WEP_AP
	} else if (os_strcmp(buf, "wep_default_key") == 0) {
		bss->ssid.wep.idx = atoi(pos);
		if (bss->ssid.wep.idx > 3) {
			wpa_printf(MSG_ERROR,
				   "Invalid wep_default_key index %d",
				   bss->ssid.wep.idx);
			return 1;
		}
	} else if (os_strcmp(buf, "wep_key0") == 0 ||
		   os_strcmp(buf, "wep_key1") == 0 ||
		   os_strcmp(buf, "wep_key2") == 0 ||
		   os_strcmp(buf, "wep_key3") == 0) {
		if (hostapd_config_read_wep(&bss->ssid.wep,
					    buf[7] - '0', pos)) {
			wpa_printf(MSG_ERROR, "Line %d: invalid WEP key '%s'",
				   line, buf);
			return 1;
		}
#endif
	} else if (os_strcmp(buf, "ap_table_max_size") == 0) {
		conf->ap_table_max_size = atoi(pos);
	} else if (os_strcmp(buf, "ap_table_expiration_time") == 0) {
		conf->ap_table_expiration_time = atoi(pos);
	} else if (os_strcmp(buf, "max_listen_interval") == 0) {
		bss->max_listen_interval = atoi(pos);
	} else if (os_strcmp(buf, "disable_pmksa_caching") == 0) {
		bss->disable_pmksa_caching = atoi(pos);
	} else if (os_strcmp(buf, "okc") == 0) {
		bss->okc = atoi(pos);
	} else if (os_strcmp(buf, "disassoc_low_ack") == 0) {
		bss->disassoc_low_ack = atoi(pos);
#ifdef CONFIG_SAE_AP
	} else if (os_strcmp(buf, "sae_anti_clogging_threshold") == 0 ||
		   os_strcmp(buf, "anti_clogging_threshold") == 0) {
		bss->anti_clogging_threshold = atoi(pos);
	} else if (os_strcmp(buf, "sae_groups") == 0) {
		if (hostapd_parse_intlist(&bss->sae_groups, pos)) {
			wpa_printf(MSG_ERROR,
				   "Line %d: Invalid sae_groups value '%s'",
				   line, pos);
			return 1;
		}
#endif
	} else {
		wpa_printf(MSG_ERROR,
			   "Line %d: unknown configuration item '%s'",
			   line, buf);
		return 1;
	}

	return 0;
}


int hostapd_set_iface(struct hostapd_config *conf,
		      struct hostapd_bss_config *bss, const char *field,
		      char *value)
{
	int errors;
	size_t i;

	errors = hostapd_config_fill(conf, bss, field, value, 0);
	if (errors) {
		wpa_printf(MSG_INFO, "Failed to set configuration field '%s' "
			   "to value '%s'", field, value);
		return -1;
	}

	for (i = 0; i < conf->num_bss; i++)
		hostapd_set_security_params(conf->bss[i], 0);

	if (hostapd_config_check(conf, 0)) {
		wpa_printf(MSG_ERROR, "Configuration check failed");
		return -1;
	}

	return 0;
}

/**
 * hostapd_driver_init - Preparate driver interface
 */
static int hostapd_driver_init(struct hostapd_iface *iface)
{
	struct wpa_init_params params;
	size_t i;
	struct hostapd_data *hapd = iface->bss[0];
	struct hostapd_bss_config *conf = hapd->conf;
	u8 *b = conf->bssid;
	struct wpa_driver_capa capa;

	if (hapd->driver == NULL || hapd->driver->hapd_init == NULL) {
		wpa_printf(MSG_ERROR, "No hostapd driver wrapper available");
		return -1;
	}

	/* Initialize the driver interface */
	if (is_zero_ether_addr(b)) {
		b = NULL;
		WPA_LOGE("hostapd_driver_init conf->bssid is null\r\n");
		return -1;
	}

	os_memset(&params, 0, sizeof(params));
	for (i = 0; wpa_drivers[i]; i++) {
		if (wpa_drivers[i] != hapd->driver)
			continue;

		if (s_hapd_global.drv_priv[i] == NULL &&
		    wpa_drivers[i]->global_init) {
			s_hapd_global.drv_priv[i] = wpa_drivers[i]->global_init(iface->interfaces);
			if (s_hapd_global.drv_priv[i] == NULL) {
				wpa_printf(MSG_ERROR, "Failed to initialize "
					   "driver '%s'",
					   wpa_drivers[i]->name);
				return -1;
			}
		}

		params.global_priv = s_hapd_global.drv_priv[i];
		break;
	}
	params.bssid = b;
	params.ifname = hapd->conf->iface;
	params.driver_params = hapd->iconf->driver_params;
	params.use_pae_group_addr = hapd->conf->use_pae_group_addr;

	params.num_bridge = hapd->iface->num_bss;
	params.bridge = os_calloc(hapd->iface->num_bss, sizeof(char *));
	if (params.bridge == NULL)
		return -1;

	for (i = 0; i < hapd->iface->num_bss; i++) {
		struct hostapd_data *bss = hapd->iface->bss[i];
#ifdef CONFIG_FULL_HOSTAPD
		if (bss->conf->bridge[0]) {
			params.bridge[i] = bss->conf->bridge;
		}
#endif
		WPA_LOGD("clear CSA in progress flag [%d:%d]\r\n", i, bss->csa_in_progress);
		bss->csa_in_progress = 0;
	}

    os_memcpy(hapd->own_addr, b, ETH_ALEN);
	params.own_addr = hapd->own_addr;

	hapd->drv_priv = hapd->driver->hapd_init(hapd, &params);
	os_free(params.bridge);
	if (hapd->drv_priv == NULL) {
		wpa_printf(MSG_ERROR, "%s driver initialization failed.",
			   hapd->driver->name);
		hapd->driver = NULL;
		return -1;
	}

	if (hapd->driver->get_capa &&
	    hapd->driver->get_capa(hapd->drv_priv, &capa) == 0) {
#ifdef CONFIG_FULL_HOSTAPD
		struct wowlan_triggers *triggs;
#endif

		iface->drv_flags = capa.flags;
		iface->drv_flags2 = capa.flags2;
		iface->probe_resp_offloads = capa.probe_resp_offloads;
		/*
		 * Use default extended capa values from per-radio information
		 */
		iface->extended_capa = capa.extended_capa;
		iface->extended_capa_mask = capa.extended_capa_mask;
		iface->extended_capa_len = capa.extended_capa_len;
		iface->drv_max_acl_mac_addrs = capa.max_acl_mac_addrs;

		/*
		 * Override extended capa with per-interface type (AP), if
		 * available from the driver.
		 */
		hostapd_get_ext_capa(iface);

#ifdef CONFIG_FULL_HOSTAPD
		triggs = wpa_get_wowlan_triggers(conf->wowlan_triggers, &capa);
		if (triggs && hapd->driver->set_wowlan) {
			if (hapd->driver->set_wowlan(hapd->drv_priv, triggs))
				wpa_printf(MSG_ERROR, "set_wowlan failed");
		}
		os_free(triggs);
#endif
	}

	return 0;
}


/**
 * hostapd_interface_init - Read configuration file and init BSS data
 *
 * This function is used to parse configuration file for a full interface (one
 * or more BSSes sharing the same radio) and allocate memory for the BSS
 * g_hapd_interfaces. No actiual driver operations are started.
 */
static struct hostapd_iface *
hostapd_interface_init(struct hapd_interfaces *interfaces, const char *if_name,
		       const char *config_fname, int debug)
{
	struct hostapd_iface *iface;
	int k;

	wpa_printf(MSG_DEBUG, "Configuration file: %s", config_fname);
	iface = hostapd_init(interfaces, config_fname);
	if (!iface)
		return NULL;

	if (if_name) {
		os_strlcpy(iface->conf->bss[0]->iface, if_name,
			   sizeof(iface->conf->bss[0]->iface));
	}

	iface->interfaces = interfaces;

	for (k = 0; k < debug; k++) {
		if (iface->bss[0]->conf->logger_stdout_level > 0)
			iface->bss[0]->conf->logger_stdout_level--;
	}

	if (iface->conf->bss[0]->iface[0] == '\0' &&
	    !hostapd_drv_none(iface->bss[0])) {
		wpa_printf(MSG_ERROR,
			   "Interface name not specified in %s, nor by '-i' parameter",
			   config_fname);
		hostapd_interface_deinit_free(iface);
		return NULL;
	}

	return iface;
}

static int hostapd_global_init(struct hapd_interfaces *interfaces,
			       const char *entropy_file)
{
	int i;

	os_memset(&s_hapd_global, 0, sizeof(s_hapd_global));

	//hostapd_logger_register_cb(hostapd_logger_cb);

#ifdef CONFIG_IEEE8021X_AP
	if (eap_server_register_methods()) {
		wpa_printf(MSG_ERROR, "Failed to register EAP methods");
		return -1;
	}
#endif

	for (i = 0; wpa_drivers[i]; i++)
		s_hapd_global.drv_count++;
	if (s_hapd_global.drv_count == 0) {
		wpa_printf(MSG_ERROR, "No drivers enabled");
		return -1;
	}
	s_hapd_global.drv_priv = os_calloc(s_hapd_global.drv_count, sizeof(void *));
	if (s_hapd_global.drv_priv == NULL)
		return -1;

	return 0;
}


static void hostapd_global_deinit(const char *pid_file, int eloop_initialized)
{
	int i;

	for (i = 0; wpa_drivers[i] && s_hapd_global.drv_priv; i++) {
		if (!s_hapd_global.drv_priv[i])
			continue;

		if(wpa_drivers[i]->global_deinit)
			wpa_drivers[i]->global_deinit(s_hapd_global.drv_priv[i]);
	}
	os_free(s_hapd_global.drv_priv);
	s_hapd_global.drv_priv = NULL;

#ifdef EAP_SERVER_TNC
	tncs_global_deinit();
#endif /* EAP_SERVER_TNC */

	eloop_free_resource();

#ifdef CONFIG_IEEE8021X_AP
	eap_server_unregister_methods();
#endif

	os_daemonize_terminate(pid_file);
}

static int hostapd_global_run(struct hapd_interfaces *ifaces, int daemonize,
			      const char *pid_file)
{
	if (daemonize && os_daemonize(pid_file)) {
		return -1;
	}

	eloop_run();

	return 0;
}

__maybe_unused static const char * hostapd_msg_ifname_cb(void *ctx);
static const char * hostapd_msg_ifname_cb(void *ctx)
{
	struct hostapd_data *hapd = ctx;
	if (hapd && hapd->conf)
		return hapd->conf->iface;
	return NULL;
}

/* Periodic cleanup tasks */
static void hostapd_periodic(void *eloop_ctx, void *timeout_ctx)
{
}

int hostapd_main_exit(void)
{
	size_t i;

	if (0 == g_hapd_interfaces.count)
		return 0;

	for (i = 0; i < g_hapd_interfaces.count; i++) {
		if (!g_hapd_interfaces.iface[i])
			continue;

		g_hapd_interfaces.iface[i]->driver_ap_teardown =
			!!(g_hapd_interfaces.iface[i]->drv_flags &
			   WPA_DRIVER_FLAGS_AP_TEARDOWN_SUPPORT);

		hostapd_interface_deinit_free(g_hapd_interfaces.iface[i]);
		g_hapd_interfaces.iface[i] = NULL;
	}
	os_free(g_hapd_interfaces.iface);
	g_hapd_interfaces.iface = NULL;
	g_hapd_interfaces.count = 0;

	eloop_cancel_timeout(hostapd_periodic, &g_hapd_interfaces, NULL);
	hostapd_global_deinit(NULL, 1);

	return 0;
}

/**
 * Usage:
 *   hostapd -g /var/run/hostapd/global -b phy0:ath10k.conf -b phy0:ath10k-1.conf -b phy0:ath10k-2.conf
 *   -b <phyX>:<bss.conf> : add a bss config
 *
 *   ath10k-1.conf:
 *       interface=wlan0-1
 *       ssid=ath10k-1
 *       bssid=02:00:00:00:03:01
 *
 *   ath10k-2.conf:
 *       interface=wlan0-2
 *       ssid=ath10k-2
 *       bssid=02:00:00:00:03:02
 *       wpa=2
 *       wpa_passphrase=0987654321
 *       wpa_key_mgmt=WPA-PSK
 *       rsn_pairwise=CCMP
 *
 */
int hostapd_main_entry(int argc, char *argv[])
{
	int ret = 1;
	size_t i;
	int debug = 0;
	char *pid_file = NULL;
	const char *log_file = NULL;
	const char *entropy_file = NULL;
#ifdef CONFIG_MBSSID
	char *bss_config[1] = {CFG_BSS_CONFIG};		//"wangzhilei_config:bss_fname"
#endif
	size_t num_bss_configs = 0;
	//int start_ifaces_in_sync = 0;
	//char **if_names = NULL;
	//size_t if_names_size = 0;
#ifdef CONFIG_DPP
	struct dpp_global_config dpp_conf;
#endif /* CONFIG_DPP */

	if (os_program_init()) {
		//os_free(ap_iface_buf);
		return -1;
	}

	os_memset(&g_hapd_interfaces, 0, sizeof(g_hapd_interfaces));
	g_hapd_interfaces.reload_config  = hostapd_reload_config;
	g_hapd_interfaces.config_read_cb = hostapd_config_read;
	g_hapd_interfaces.for_each_interface = hostapd_for_each_interface;
	g_hapd_interfaces.ctrl_iface_init    = 0;
	g_hapd_interfaces.ctrl_iface_deinit  = 0;
	g_hapd_interfaces.driver_init = hostapd_driver_init;
#ifdef CONFIG_DPP
	os_memset(&dpp_conf, 0, sizeof(dpp_conf));
	/* TODO: dpp_conf.msg_ctx? */
	interfaces.dpp = dpp_global_init(&dpp_conf);
	if (!interfaces.dpp)
		return -1;
#endif /* CONFIG_DPP */

	wpa_msg_register_ifname_cb(hostapd_msg_ifname_cb);

	g_hapd_interfaces.count = argc - 1;
	if (g_hapd_interfaces.count || num_bss_configs) {
		g_hapd_interfaces.iface = os_calloc(g_hapd_interfaces.count + num_bss_configs,
											sizeof(struct hostapd_iface *));
		if (g_hapd_interfaces.iface == NULL) {
			//os_free(ap_iface_buf);
			WPA_LOGE("malloc failed\r\n");
			return -1;
		}
	}

	if (hostapd_global_init(&g_hapd_interfaces, entropy_file)) {
		//os_free(ap_iface_buf);
		WPA_LOGE("Failed to initialize global context\r\n");
		return -1;
	}

	/* Allocate and parse configuration for full interface files */
	for (i = 0; i < g_hapd_interfaces.count; i++) {
		char *config_fname = CFG_CONFIG_FNAME;	//"beken_cfg_fname"

		g_hapd_interfaces.iface[i] = hostapd_interface_init(&g_hapd_interfaces,
									    "wlan0",
									    config_fname,
									    debug);
		if (!g_hapd_interfaces.iface[i]) {
			WPA_LOGE("Failed to initialize interface\r\n");
			goto out;
		}
		//if (start_ifaces_in_sync)
		//	interfaces.iface[i]->need_to_start_in_sync = 1;
	}

#ifdef CONFIG_MBSSID
	/* Allocate and parse configuration for per-BSS files */
	for (i = 0; i < num_bss_configs; i++) {
		struct hostapd_iface *iface;
		char *fname;

		wpa_printf(MSG_INFO, "BSS config: %s", bss_config[i]);
		fname = os_strchr(bss_config[i], ':');
		if (fname == NULL) {
			wpa_printf(MSG_ERROR,
					   "Invalid BSS config identifier '%s'",
					   bss_config[i]);
			goto out;
		}
		*fname++ = '\0';
		iface = hostapd_interface_init_bss(&g_hapd_interfaces, bss_config[i],
										   fname, debug);
		if (iface == NULL)
			goto out;
		for (j = 0; j < g_hapd_interfaces.count; j++) {
			if (g_hapd_interfaces.iface[j] == iface)
				break;
		}
		if (j == g_hapd_interfaces.count) {
			struct hostapd_iface **tmp;
			tmp = os_realloc_array(g_hapd_interfaces.iface,
								   g_hapd_interfaces.count + 1,
								   sizeof(struct hostapd_iface *));
			if (tmp == NULL) {
				hostapd_interface_deinit_free(iface);
				goto out;
			}
			g_hapd_interfaces.iface = tmp;
			g_hapd_interfaces.iface[g_hapd_interfaces.count++] = iface;
		}
	}
#endif

	/*
	 * Enable configured g_hapd_interfaces. Depending on channel configuration,
	 * this may complete full initialization before returning or use a
	 * callback mechanism to complete setup in case of operations like HT
	 * co-ex scans, ACS, or DFS are needed to determine channel parameters.
	 * In such case, the interface will be enabled from eloop context within
	 * hostapd_global_run().
	 */
	g_hapd_interfaces.terminate_on_error = g_hapd_interfaces.count;
	for (i = 0; i < g_hapd_interfaces.count; i++) {
		if (hostapd_driver_init(g_hapd_interfaces.iface[i]) ||
			hostapd_setup_interface(g_hapd_interfaces.iface[i]))
			goto out;
	}

	ret = 0;

	return ret;

out:
	/* Deinitialize all g_hapd_interfaces */
	WPA_LOGE("hostapd_main_init_failed\r\n");
	for (i = 0; i < g_hapd_interfaces.count; i++) {
		if (!g_hapd_interfaces.iface[i])
			continue;

		g_hapd_interfaces.iface[i]->driver_ap_teardown =
			!!(g_hapd_interfaces.iface[i]->drv_flags &
			   WPA_DRIVER_FLAGS_AP_TEARDOWN_SUPPORT);

		hostapd_interface_deinit_free(g_hapd_interfaces.iface[i]);
		g_hapd_interfaces.iface[i] = NULL;
	}
	os_free(g_hapd_interfaces.iface);
	g_hapd_interfaces.iface = NULL;
	g_hapd_interfaces.count = 0;
#ifdef CONFIG_DPP
	dpp_global_deinit(g_hapd_interfaces.dpp);
#endif /* CONFIG_DPP */

	eloop_cancel_timeout(hostapd_periodic, &g_hapd_interfaces, NULL);
	hostapd_global_deinit(pid_file, 1);

	os_free(pid_file);
	pid_file = NULL;

	//os_free(ap_iface_buf);
	//ap_iface_buf = NULL;

	if (log_file)
		wpa_debug_close_file();
	wpa_debug_close_linux_tracing();

	os_program_deinit();

	return ret;
}


__maybe_unused static void hostapd_thread_main( void *arg );
static void hostapd_thread_main( void *arg )
{
	int daemonize = 0;
    char *pid_file = NULL;

    if (hostapd_global_run(&g_hapd_interfaces, daemonize, pid_file)) {
        wpa_printf(MSG_ERROR, "Failed to start eloop");
    }
}

int hostapd_channel_switch(int new_freq)
{
	return wpa_ctrl_request_async(WPA_CTRL_CMD_AP_CHAN_SWITCH, (void *)new_freq);
}

int wpa_hostapd_queue_command(wpah_msg_t *msg)
{
	int ret = WPA_FAIL;

	if (!wpah_queue)
		return WPA_ERR_WPAH_QUEUE_INIT;

	ret = rtos_push_to_queue(&wpah_queue, msg, BEKEN_NO_WAIT);
	if (kNoErr != ret)
		WPA_LOGE("wpa_hostapd_queue_command:%d\r\n", ret);

	return ret;
}

uint32_t wpa_hostapd_queue_poll(uint32_t param)
{
	bk_err_t ret = 0;
	wpah_msg_t msg = {0};

	if (NULL == wpah_queue)
		goto poll_exit;

	msg.cmd = WPA_CTRL_CMD_SOCKET;
	msg.argu = (u32)param;
	ret = rtos_push_to_queue(&wpah_queue, &msg, BEKEN_NO_WAIT);
	if (kNoErr != ret)
		WPA_LOGE("wpa_hostapd_queue_poll_failed:%d\r\n", ret);

poll_exit:
	return ret;
}

bool is_wpah_queue_full()
{
	return rtos_is_queue_full(&wpah_queue);
}

int wpas_calculate_ap_pmk(const char *passphrase, const uint8_t *ssid, uint8_t *pmk)
{
	int ret;

	const char *p = (const char*)(char *)ssid;
	ret = pbkdf2_sha1(passphrase, ssid, os_strlen(p), 4096, pmk, 32);

	return ret;
}

// eof

