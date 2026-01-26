#include <stdint.h>
#include "../src/utils/includes.h"

#include "../src/utils/common.h"
#include "../src/utils/eloop.h"
//#include "bk_wifi_types.h"
#include "wpa_supplicant_i.h"
#include "../src/common/wpa_ctrl.h"
#include <os/mem.h>
#include "config.h"
#include "wpa_scan.h"
#include "../hostapd/main_none.h"
#include "bk_wifi_types.h"
#include "../src/common/wpa_common.h"
#include "../src/rsn_supp/wpa.h"
#include "bss.h"
#include "../hostapd/main_none.h"
#include "modules/wifi.h"
//#include "mm_bcn.h"
#include "../src/ap/hostapd.h"
#include "ctrl_iface.h"
//#include "bk_private/sa_station.h"
//#include "rxu_task.h"
#include "../src/common/wpa_psk_cache.h"
#include "ap.h"
#ifdef CONFIG_WPS
#include "../src/wps/wps.h"
#include "wps_supplicant.h"
#endif
#ifdef CONFIG_P2P
#include "../src/p2p/p2p.h"
#include "p2p_supplicant.h"
#include "chan.h"
#include "rwnx_defs.h"
#endif
#if CONFIG_LWIP
//#include "net.h"
//#include "bk_net.h"
#else
#include "lwip/pbuf.h"
#endif
//#include "sm_task.h"
#include "../src/ap/sta_info.h"
#include "../src/ap/ap_drv_ops.h"
#include "../src/ap/beacon.h"
#include "../src/rsn_supp/pmksa_cache.h"
#include "../src/rsn_supp/wpa_i.h"
#include "../bk_patch/wpa_err.h"
//#include "bk_wifi_ip.h"
#include "bk_wifi_private.h"
#include "bk_hostapd_intf.h"
//#include "bk_rw.h"
#include "bk_feature.h"
//#include "bk_wifi_types.h"
#include "bssid_ignore.h"
#include "driver_i.h"
#include "bk_rw.h"
#include "rwnx_defs.h"
#if CONFIG_EASY_FLASH_FAST_CONNECT
#include "bk_ef.h"
#endif

extern beken_thread_t wpas_thread_handle;
extern bool site_survey_cc;
static int wpa_ctrl_debug_info_dump(struct wpa_supplicant *wpas, uint32_t type);
extern void wpa_driver_scan_timeout(void *eloop_ctx, void *timeout_ctx);
extern uint32_t bk_lookup_ipaddr_wrapper(void *addr);
extern void rwnx_regulatory_hint_11d(int freq, const u8 *country_ie, u8 country_ie_len);
extern void regulatory_hint_disconnect(void);

int __wpa_ctrl_request(wpa_ctrl_cmd_t cmd, void *data, int wait, uint16_t flags)
{
	wpah_msg_t msg = {0};
	int result = WPA_FAIL;
	int res = 0;

	msg.cmd = cmd;
	msg.argu = (uint32_t)data;
	msg.sema = 0;
	msg.flags = flags;

	if(wait)
		msg.result = &result;
	else
		result = 0;

	/* for wifi events, always queue this command */
	if (msg.cmd >= WPA_CTRL_CMD_RW_EVT_START)
		msg.flags |= WPAH_FLAG_DEFER;

	if (rtos_is_current_thread(&wpas_thread_handle) && !(msg.flags & WPAH_FLAG_DEFER)) {
		res = wpa_supplicant_ctrl_iface_receive(&msg);
		result = res;
	} else {
		int ret = 0;

		if (wait && rtos_init_semaphore(&msg.sema, 1) != kNoErr)
			return WPA_ERR_CTRL_SEM_INIT;

		ret = wpa_hostapd_queue_command(&msg);
		if (ret != kNoErr) {
			result = ret;

			/* free data when this data is going to be freed in wpa_s */
			if (flags & WPAH_FLAG_FREE)
				os_free(data);

			goto out;
		}

		if (wait)
			rtos_get_semaphore(&msg.sema, BEKEN_WAIT_FOREVER);	// XXX: forever?

out:
		if (wait)
			rtos_deinit_semaphore(&msg.sema);
	}

	return result;
}

/* sync */
int wpa_ctrl_request(wpa_ctrl_cmd_t cmd, void *data)
{
	return __wpa_ctrl_request(cmd, data, 1, 0);
}

int wpa_ctrl_request_async(wpa_ctrl_cmd_t cmd, void *data)
{
	return __wpa_ctrl_request(cmd, data, 0, 0);
}

/*
 * event >= WPA_CTRL_CMD_RW_EVT_START
 * TODO: free
 */
int wpa_ctrl_event(int event, void *data)
{
	return __wpa_ctrl_request(event, data, 0, 0);
}

int wpa_ctrl_event_sync(int event, void *data)
{
	return __wpa_ctrl_request(event, data, 1, 0);
}

int wpa_ctrl_event_copy(int event, void *data, int len)
{
	void *p = os_malloc(len);
	if (!p)
		return -1;
	os_memcpy(p, data, len);

	return __wpa_ctrl_request(event, p, 0, WPAH_FLAG_FREE);
}

static int wpa_supplicant_ctrl_iface_select_network(
	struct wpa_supplicant *wpa_s, int id, int chan)
{
	struct wpa_ssid *ssid;

	/* cmd: "<network id>" or "any" */
	if (id == -1) {
		wpa_printf(MSG_DEBUG, "CTRL_IFACE: SELECT_NETWORK any");
		ssid = NULL;
	} else {
		wpa_printf(MSG_DEBUG, "CTRL_IFACE: SELECT_NETWORK id=%d", id);

		ssid = wpa_config_get_network(wpa_s->conf, id);
		if (ssid == NULL) {
			wpa_printf(MSG_DEBUG, "CTRL_IFACE: Could not find "
				   "network id=%d", id);
			return -1;
		}
		if (ssid->disabled == 2) {
			wpa_printf(MSG_DEBUG, "CTRL_IFACE: Cannot use "
				   "SELECT_NETWORK with persistent P2P group");
			return -1;
		}
	}

	if (chan) {
		int *freqs = (int *)os_zalloc(sizeof(int) * 3);	// zero terminated
		if (freqs) {
			os_free(wpa_s->select_network_scan_freqs);

			freqs[0] = rw_ieee80211_get_centre_frequency(chan);
			if (!bk_feature_fast_connect_enable()) {
			/* scan twice in this fixed channel */
				freqs[1] = freqs[0];
			}
			wpa_s->select_network_scan_freqs = freqs;
		}
	}

#if 0
	pos = os_strstr(cmd, " freq=");
	if (pos) {
		int *freqs = freq_range_to_channel_list(wpa_s, pos + 6);
		if (freqs) {
			wpa_s->scan_req = MANUAL_SCAN_REQ;
			os_free(wpa_s->manual_scan_freqs);
			wpa_s->manual_scan_freqs = freqs;
		}
	}
#endif

#if 0
	if(oob_ssid)
	{
		int len;
		int oob_ssid_len;

		BK_ASSERT(0 == wpa_s->ssids_from_scan_req);
		oob_ssid_len = os_strlen(oob_ssid);

		if(0 == wpas_connect_ssid)
		{
			wpas_connect_ssid = (struct wpa_ssid_value *)os_malloc(sizeof(struct wpa_ssid_value));
			BK_ASSERT(wpas_connect_ssid);
		}

		len = MIN(SSID_MAX_LEN, oob_ssid_len);

		wpas_connect_ssid->ssid_len = len;
		os_memcpy(wpas_connect_ssid->ssid, oob_ssid, len);

		wpa_s->num_ssids_from_scan_req = 1;
		wpa_s->ssids_from_scan_req = wpas_connect_ssid;
		wpa_s->scan_req = MANUAL_SCAN_REQ;
		WPA_LOGI("MANUAL_SCAN_REQ\r\n");
	}
#endif

	wpa_s->scan_min_time.sec = 0;
	wpa_s->scan_min_time.usec = 0;

#if defined(BK_SUPPLICANT) && defined(CONFIG_AUTO_RECONNECT)
	wpa_s->notified_disconn = false;
	/* reinit count */
	if (wpa_s->auto_reconnect_max_count != 0)
		wpa_s->auto_reconnect_count = wpa_s->auto_reconnect_max_count;

	/* if specified timeout, start timer */
	if (wpa_s->auto_reconnect_timeout != 0) {
		os_get_reltime(&wpa_s->auto_reconnect_start_time);
		eloop_cancel_timeout(wpa_supplicant_auto_reconnect_timeout,
			wpa_s, NULL);
		eloop_register_timeout(wpa_s->auto_reconnect_timeout, 0,
			wpa_supplicant_auto_reconnect_timeout, wpa_s, NULL);
	}
#endif

	wpa_s->suitable_network = 0;
	wpa_s->no_suitable_network = 0;

	wpa_supplicant_select_network(wpa_s, ssid);

	return 0;
}

int wpa_supplicant_ctrl_iface_scan_results(
	struct wpa_supplicant *wpa_s, ScanResult_adv *results)
{
	struct wpa_bss *bss;
	int bss_num = 0;
	int index = 0;
	size_t ssid_len = 0;
	u8 *ssid = NULL;
	int i = 0;

	results->ApNum = 0;
	results->ApList = 0;

	dl_list_for_each(bss, &wpa_s->bss_id, struct wpa_bss, list_id) {
		/* if no ssid specified, or match the specified ssid */
		for (i = 0; i < wpa_s->num_ssids_from_scan_req; i ++) {
			ssid = NULL;
			ssid_len = 0;
			if (&wpa_s->ssids_from_scan_req[i] && wpa_s->ssids_from_scan_req[i].ssid_len) {
				ssid = wpa_s->ssids_from_scan_req[i].ssid;
				ssid_len = wpa_s->ssids_from_scan_req[i].ssid_len;
			}

			if (!ssid ||
				(bss->ssid_len == ssid_len && !os_memcmp(ssid, bss->ssid, ssid_len))) {
				bss_num++;
				break;
			}
		}
	}
	if (!bss_num)
		return 0;

	results->ApList = os_malloc(sizeof(*results->ApList) * bss_num);
	if (!results->ApList)
		return -1;
	dl_list_for_each(bss, &wpa_s->bss_id, struct wpa_bss, list_id) {
		/* if no ssid specified, or match the specified ssid */
		for (i = 0; i < wpa_s->num_ssids_from_scan_req; i ++) {
			ssid = NULL;
			ssid_len = 0;
			if (&wpa_s->ssids_from_scan_req[i] && wpa_s->ssids_from_scan_req[i].ssid_len) {
				ssid = wpa_s->ssids_from_scan_req[i].ssid;
				ssid_len = wpa_s->ssids_from_scan_req[i].ssid_len;
			}

			if (!ssid ||
				(bss->ssid_len == ssid_len && !os_memcmp(ssid, bss->ssid, ssid_len))) {
				os_memcpy(results->ApList[index].ssid, bss->ssid, bss->ssid_len);
				results->ApList[index].ssid[bss->ssid_len] = '\0';
				results->ApList[index].ApPower = bss->level;
				ieee80211_freq_to_chan(bss->freq, (u8 *)&results->ApList[index].channel);
				os_memcpy(results->ApList[index].bssid, bss->bssid, ETH_ALEN);
				results->ApList[index].security = get_security_type_from_ie(bss->ies, bss->ie_len, bss->caps);

				index++;
				break;
			}
		}
	}
	results->ApNum = bss_num;

	return 0;
}

static int wpas_ctrl_scan(struct wpa_supplicant *wpa_s, wlan_sta_scan_param_t *params)
{
	unsigned int manual_scan_passive = 0;
	unsigned int manual_scan_use_id = 1;
	unsigned int manual_scan_only_new = 0;
	unsigned int scan_only = 1;
	unsigned int scan_id_count = 0;
	int scan_id[MAX_SCAN_ID] = {0};
	void (*scan_res_handler)(struct wpa_supplicant *wpa_s,
				 struct wpa_scan_results *scan_res);
	int *manual_scan_freqs = NULL;
	struct wpa_ssid_value *ssid = NULL;
	unsigned int ssid_count = 0;
	int ret = 0;
	bool scan_pending;
#ifdef CONFIG_FULL_SUPPLICANT
	struct wpa_ssid_value *ns;
	char *pos;
#endif

	if (wpa_s->wpa_state == WPA_INTERFACE_DISABLED) {
		return WPA_ERR_STATE;
	}

	if (radio_work_pending(wpa_s, "scan")) {
		wpa_printf(MSG_DEBUG,
			   "Pending scan scheduled - reject new request");
		return WPA_ERR_SCAN_PENDING;
	}

#ifdef CONFIG_INTERWORKING
	if (wpa_s->fetch_anqp_in_progress || wpa_s->network_select) {
		wpa_printf(MSG_DEBUG,
			   "Interworking select in progress - reject new scan");
		return WPA_ERR_SCAN_SELECT_IN_PROGRESS;
	}
#endif /* CONFIG_INTERWORKING */

#ifdef CONFIG_FULL_SUPPLICANT
	if (params) {
		if (os_strncasecmp(params, "TYPE=ONLY", 9) == 0)
			scan_only = 1;

		pos = os_strstr(params, "freq=");
		if (pos) {
			manual_scan_freqs = freq_range_to_channel_list(wpa_s,
								       pos + 5);
			if (manual_scan_freqs == NULL) {
				ret = WPA_ERR_SCAN_FREQ;
				goto done;
			}
		}

		pos = os_strstr(params, "passive=");
		if (pos)
			manual_scan_passive = !!atoi(pos + 8);

		pos = os_strstr(params, "use_id=");
		if (pos)
			manual_scan_use_id = atoi(pos + 7);

		pos = os_strstr(params, "only_new=1");
		if (pos)
			manual_scan_only_new = 1;

		pos = os_strstr(params, "scan_id=");
		if (pos && scan_id_list_parse(wpa_s, pos + 8, &scan_id_count,
					      scan_id) < 0) {
			ret = WPA_ERR_SCAN_ID;
			goto done;
		}

		pos = os_strstr(params, "bssid=");
		if (pos) {
			u8 bssid[ETH_ALEN];

			pos += 6;
			if (hwaddr_aton(pos, bssid)) {
				wpa_printf(MSG_ERROR, "Invalid BSSID %s", pos);
				ret = WPA_ERR_SCAN_BSSID;
				goto done;
			}
			os_memcpy(wpa_s->next_scan_bssid, bssid, ETH_ALEN);
		}

		pos = params;
		while (pos && *pos != '\0') {
			if (os_strncmp(pos, "ssid ", 5) == 0) {
				char *end;

				pos += 5;
				end = pos;
				while (*end) {
					if (*end == '\0' || *end == ' ')
						break;
					end++;
				}

				ns = os_realloc_array(
					ssid, ssid_count + 1,
					sizeof(struct wpa_ssid_value));
				if (ns == NULL) {
					*reply_len = -1;
					goto done;
				}
				ssid = ns;

				if ((end - pos) & 0x01 ||
				    end - pos > 2 * SSID_MAX_LEN ||
				    hexstr2bin(pos, ssid[ssid_count].ssid,
					       (end - pos) / 2) < 0) {
					wpa_printf(MSG_DEBUG,
						   "Invalid SSID value '%s'",
						   pos);
					ret = WPA_ERR_SCAN_SSID;
					goto done;
				}
				ssid[ssid_count].ssid_len = (end - pos) / 2;
				wpa_hexdump_ascii(MSG_DEBUG, "scan SSID",
						  ssid[ssid_count].ssid,
						  ssid[ssid_count].ssid_len);
				ssid_count++;
				pos = end;
			}

			pos = os_strchr(pos, ' ');
			if (pos)
				pos++;
		}
	}
#endif
	scan_pending = eloop_is_timeout_registered(wpa_supplicant_scan, wpa_s, NULL);

	if (!wpa_s->sched_scanning && !wpa_s->scanning &&
		((wpa_s->wpa_state <= WPA_SCANNING) ||
		 (wpa_s->wpa_state == WPA_COMPLETED)) && !scan_pending) {
		if (params) {
			ssid_count = params->num_ssids;
			ssid = os_realloc_array(ssid, ssid_count, sizeof(struct wpa_ssid_value));
			if (ssid == NULL) {
				ret = WPA_ERR_NO_MEM;
				goto done;
			}

			/* set scan ssid */
			for (int i = 0; i < ssid_count; i++) {
				os_memcpy(ssid[i].ssid, params->ssids[i].ssid, params->ssids[i].ssid_len);
				ssid[i].ssid_len = params->ssids[i].ssid_len;
				wpa_hexdump_ascii(MSG_DEBUG, "scan SSID",
						  ssid[i].ssid,
						  ssid[i].ssid_len);
			}
		} else {
			/* do wildcard scan */
			ssid_count = 1;
			ssid = os_calloc(1, sizeof(*ssid));
			if (!ssid) {
				ret = WPA_ERR_NO_MEM;
				goto done;
			}
		}

		wpa_s->num_ssids_from_scan_req = ssid_count;
		os_free(wpa_s->ssids_from_scan_req);
		if (ssid_count) {
			wpa_s->ssids_from_scan_req = ssid;
			ssid = NULL;
		} else {
			wpa_s->ssids_from_scan_req = NULL;
		}

		if (scan_only)
			scan_res_handler = scan_only_handler;
		else if (wpa_s->scan_res_handler == scan_only_handler)
			scan_res_handler = NULL;
		else
			scan_res_handler = wpa_s->scan_res_handler;

		wpa_s->manual_scan_passive = manual_scan_passive;
		wpa_s->manual_scan_use_id = manual_scan_use_id;
		wpa_s->manual_scan_only_new = manual_scan_only_new;
		wpa_s->scan_id_count = scan_id_count;
		os_memcpy(wpa_s->scan_id, scan_id, scan_id_count * sizeof(int));
		wpa_s->scan_res_handler = scan_res_handler;
		os_free(wpa_s->manual_scan_freqs);
		wpa_s->manual_scan_freqs = manual_scan_freqs;
		manual_scan_freqs = NULL;

		wpa_s->normal_scans = 0;
		wpa_s->scan_req = MANUAL_SCAN_REQ;
#ifdef CONFIG_WPS
		wpa_s->after_wps = 0;
		wpa_s->known_wps_freq = 0;
#endif
#if CONFIG_WIFI_SCAN_COUNTRY_CODE
		if (params->scan_cc)
			site_survey_cc = true;
		else
			site_survey_cc = false;
#endif
		ret = wpa_supplicant_req_scan(wpa_s, 0, 0);
		if (wpa_s->manual_scan_use_id) {
			if (params->id) {
				wpa_s->manual_scan_id = params->id;
			} else {
				wpa_s->manual_scan_id++;
				if (!wpa_s->manual_scan_id)
					wpa_s->manual_scan_id = 1;
			}

#ifdef CONFIG_NO_STDOUT_DEBUG
			WPA_LOGI("Assigned scan id 0x%x\n", wpa_s->manual_scan_id);
#else
			wpa_dbg(wpa_s, MSG_DEBUG, "Assigned scan id %u",
				wpa_s->manual_scan_id);
#endif
		}
	} else {
		wpa_printf(MSG_DEBUG, "Ongoing scan action - reject new request");

		if (wpa_s->sched_scanning)
			ret = WPA_ERR_SCAN_SCHED_IN_PROGRESS;
		else if (wpa_s->scanning)
			ret = WPA_ERR_SCAN_IN_PROGRESS;
		else
			ret = WPA_ERR_TRY_AGAIN;
	}

done:
	os_free(manual_scan_freqs);
	os_free(ssid);

	return ret;
}


#ifdef CONFIG_WPA_SUPPLICANT_MULTI_NETWORK
static int wpa_supplicant_ctrl_iface_add_network(struct wpa_supplicant *wpa_s, int id)
{
	struct wpa_ssid *ssid;
	int ret;

	wpa_printf(MSG_DEBUG, "CTRL_IFACE: ADD_NETWORK");

	ssid = wpa_config_add_network(wpa_s->conf);
	if (ssid == NULL)
		return -1;

	wpas_notify_network_added(wpa_s, ssid);

	ssid->disabled = 1;
	wpa_config_set_network_defaults(ssid);

	return 0;
}

static int wpa_supplicant_ctrl_iface_remove_network(struct wpa_supplicant *wpa_s, int id)
{
	int id;
	struct wpa_ssid *ssid;
	int was_disabled;

	/* cmd: "<network id>" or "all" */
	if (id == -1) {
		wpa_printf(MSG_DEBUG, "CTRL_IFACE: REMOVE_NETWORK all");
		if (wpa_s->sched_scanning)
			wpa_supplicant_cancel_sched_scan(wpa_s);

		eapol_sm_invalidate_cached_session(wpa_s->eapol);
		if (wpa_s->current_ssid) {
#ifdef CONFIG_SME
			wpa_s->sme.prev_bssid_set = 0;
#endif /* CONFIG_SME */
			wpa_sm_set_config(wpa_s->wpa, NULL);
			eapol_sm_notify_config(wpa_s->eapol, NULL, NULL);
			if (wpa_s->wpa_state >= WPA_AUTHENTICATING)
				wpa_s->own_disconnect_req = 1;
			wpa_supplicant_deauthenticate(
				wpa_s, WLAN_REASON_DEAUTH_LEAVING);
		}
		ssid = wpa_s->conf->ssid;
		while (ssid) {
			struct wpa_ssid *remove_ssid = ssid;
			id = ssid->id;
			ssid = ssid->next;
			if (wpa_s->last_ssid == remove_ssid)
				wpa_s->last_ssid = NULL;
			wpas_notify_network_removed(wpa_s, remove_ssid);
			wpa_config_remove_network(wpa_s->conf, id);
		}
		return 0;
	}

	wpa_printf(MSG_DEBUG, "CTRL_IFACE: REMOVE_NETWORK id=%d", id);

	ssid = wpa_config_get_network(wpa_s->conf, id);
	if (ssid)
		wpas_notify_network_removed(wpa_s, ssid);
	if (ssid == NULL) {
		wpa_printf(MSG_DEBUG, "CTRL_IFACE: Could not find network "
			   "id=%d", id);
		return -1;
	}

	if (wpa_s->last_ssid == ssid)
		wpa_s->last_ssid = NULL;

	if (ssid == wpa_s->current_ssid || wpa_s->current_ssid == NULL) {
#ifdef CONFIG_SME
		wpa_s->sme.prev_bssid_set = 0;
#endif /* CONFIG_SME */
		/*
		 * Invalidate the EAP session cache if the current or
		 * previously used network is removed.
		 */
		eapol_sm_invalidate_cached_session(wpa_s->eapol);
	}

	if (ssid == wpa_s->current_ssid) {
		wpa_sm_set_config(wpa_s->wpa, NULL);
		eapol_sm_notify_config(wpa_s->eapol, NULL, NULL);

		if (wpa_s->wpa_state >= WPA_AUTHENTICATING)
			wpa_s->own_disconnect_req = 1;
		wpa_supplicant_deauthenticate(wpa_s,
					      WLAN_REASON_DEAUTH_LEAVING);
	}

	was_disabled = ssid->disabled;

	if (wpa_config_remove_network(wpa_s->conf, id) < 0) {
		wpa_printf(MSG_DEBUG, "CTRL_IFACE: Not able to remove the "
			   "network id=%d", id);
		return -1;
	}

	if (!was_disabled && wpa_s->sched_scanning) {
		wpa_printf(MSG_DEBUG, "Stop ongoing sched_scan to remove "
			   "network from filters");
		wpa_supplicant_cancel_sched_scan(wpa_s);
		wpa_supplicant_req_scan(wpa_s, 0, 0);
	}

	return 0;
}
#endif

extern int wpa_config_parse_eap(const void *,
		struct wpa_ssid *ssid, int line,
		const char *value);

int wpa_supplicant_ctrl_iface_set_network(struct wpa_supplicant *wpa_s, wlan_sta_config_t *config)
{
	struct wpa_ssid *ssid;
	int id = 0;
#ifdef CONFIG_WPA_SUPPLICANT_MULTI_NETWORK
	id = config->id;
#endif

	ssid = wpa_config_get_network(wpa_s->conf, id);
	if (ssid == NULL) {
		wpa_printf(MSG_DEBUG, "CTRL_IFACE: Could not find network "
			   "id=%d", id);
		return -1;
	}

	/* when set/change ssid configuration, always set mem_only_psk to 1 */
	ssid->mem_only_psk = 1;

	switch (config->field) {
	case WLAN_STA_FIELD_SSID:
		//WPA_LOGI("ssid: |%s|\n", config->u.ssid.ssid);
		if ((ssid->ssid_len != config->u.ssid.ssid_len) ||
			os_memcmp(ssid->ssid, config->u.ssid.ssid, ssid->ssid_len)) {
#if BK_SUPPLICANT
#ifdef CONFIG_SAE
			sae_deinit_pt(ssid->pt);	// clear the last pt
			ssid->pt = NULL;
#endif
#endif
			ssid->psk_set = 0;	// recalc psk
			if (wpa_s->wpa)
				wpa_sm_pmksa_cache_flush(wpa_s->wpa, ssid);
			// if ssid changes, reset retry_count
			wpa_s->sme.retry_count = 0;
		}
		if (ssid->ssid) {
			os_free(ssid->ssid);
			ssid->ssid = 0;
			ssid->ssid_len = 0;
		}
		if (config->u.ssid.ssid && config->u.ssid.ssid_len) {
			ssid->ssid = (u8 *)dup_binstr(config->u.ssid.ssid, config->u.ssid.ssid_len);
			ssid->ssid_len = config->u.ssid.ssid_len;
		}
		if (bk_feature_bssid_connect_enable()) {
			if (ssid->bssid) {
				WPA_LOGD("clear bssid\r\n");
				os_memset(ssid->bssid, 0, ETH_ALEN);
				ssid->bssid_set = 0;
			}
		}

		break;

	case WLAN_STA_FIELD_BSSID:
		//WPA_LOGI("bssid set\n");
		os_memcpy(ssid->bssid, config->u.bssid, ETH_ALEN);
		ssid->bssid_set = 1;
		break;

	case WLAN_STA_FIELD_FREQ:
		//ssid->frequency = ieee80211_chan_to_freq("CN", 7, config->u.channel);
		//WPA_LOGI("ssid->freq = %d\n", ssid->frequency);
		break;

	case WLAN_STA_FIELD_PSK:
		//WPA_LOGI("psk: |%s|\n", config->u.psk);
		if (config->u.psk[0] != '\0') {
			if (ssid->passphrase && os_strcmp(ssid->passphrase, (char *)config->u.psk)) {
#if BK_SUPPLICANT
#ifdef CONFIG_SAE
				sae_deinit_pt(ssid->pt);	// clear the last pt
				ssid->pt = NULL;
#endif
				// if passphrase changes, reset retry_count
				wpa_s->sme.retry_count = 0;
#endif
				ssid->psk_set = 0;
				if (wpa_s->wpa)
					wpa_sm_pmksa_cache_flush(wpa_s->wpa, ssid);
			}
			str_clear_free(ssid->passphrase);
			ssid->passphrase = os_strdup(config->u.psk);	//dup_binstr(g_sta_param_ptr->key, g_sta_param_ptr->key_len);
#if CONFIG_QUICK_TRACK
			/* NOTICE: ssid must be set first */
			wpa_config_update_psk(ssid);
#endif
			//ssid->mem_only_psk = 1;

			//if (ssid->changes) {
				//wpa_config_update_psk(ssid);
				//ssid->changes = 0;
			//}
		}
		break;
	case WLAN_STA_FIELD_WEP_KEY0:
		//wpa_config_parse_wep_key
		break;
	case WLAN_STA_FIELD_WEP_KEY1:
		break;
	case WLAN_STA_FIELD_WEP_KEY2:
		break;
	case WLAN_STA_FIELD_WEP_KEY3:
		break;
	case WLAN_STA_FIELD_WEP_KEY_INDEX:
		ssid->wep_tx_keyidx = config->u.wep_tx_keyidx;
		break;
	case WLAN_STA_FIELD_KEY_MGMT:
		//WPA_LOGI("key_mgmt: %d\n", __func__, config->u.key_mgmt);
		ssid->key_mgmt = config->u.key_mgmt;
		break;
	case WLAN_STA_FIELD_PAIRWISE_CIPHER:
		//WPA_LOGI("pairwise_cipher: %d\n", __func__, config->u.pairwise_cipher);
		ssid->pairwise_cipher = config->u.pairwise_cipher;
		break;
	case WLAN_STA_FIELD_GROUP_CIPHER:
		//WPA_LOGI("group_cipher: %d\n", __func__, config->u.group_cipher);
		ssid->group_cipher = config->u.group_cipher;
		break;
	case WLAN_STA_FIELD_PROTO:
		//WPA_LOGI("proto: %d\n", __func__, config->u.proto);
		ssid->proto = config->u.proto;
		break;
	case WLAN_STA_FIELD_AUTH_ALG:
		//WPA_LOGI("auth_alg: %d\n", __func__, config->u.auth_alg);
		ssid->auth_alg = config->u.auth_alg;
		break;
	case WLAN_STA_FIELD_WPA_PTK_REKEY:
		//WPA_LOGI("wpa_ptk_rekey: %d\n", __func__, config->u.wpa_ptk_rekey);
		ssid->wpa_ptk_rekey = config->u.wpa_ptk_rekey;
		break;
	case WLAN_STA_FIELD_SCAN_SSID:
		//WPA_LOGI("scan_ssid: %d\n", __func__, config->u.scan_ssid);
		ssid->scan_ssid = config->u.scan_ssid;
		break;

	case WLAN_STA_FIELD_SAE_GROUPS: {
		int *groups = os_malloc(sizeof(config->u.sae_groups));
		if (groups) {
			os_free(wpa_s->conf->sae_groups);
			os_memcpy(groups, config->u.sae_groups, sizeof(config->u.sae_groups));
			wpa_s->conf->sae_groups = groups;
			//while (*groups) {
			//	WPA_LOGI("\t sae_group: %d\n", *groups);
			//	groups++;
			//}
		}
	}	break;

	case WLAN_STA_FIELD_MFP:
#ifdef CONFIG_IEEE80211W
		ssid->ieee80211w = config->u.ieee80211w;
#endif
		break;

	case WLAN_STA_FIELD_OCV:
#ifdef CONFIG_OCV
		ssid->ocv = config->u.ocv;
#endif
		break;

	case WLAN_STA_FIELD_DEBUG_LEVEL:
		wpa_debug_level = config->u.debug_level;
		break;

	case WLAN_STA_FIELD_DEBUG_SHOW_KEYS:
		wpa_debug_show_keys = config->u.debug_show_keys;
		break;

#ifdef IEEE8021X_EAPOL
	case WLAN_STA_FIELD_EAP:
		wpa_config_parse_eap(NULL, ssid, 0, config->u.eap);
		break;

	case WLAN_STA_FIELD_IDENTITY:
		ssid->eap.identity_len = os_strlen(config->u.ca_cert);
		ssid->eap.identity = (u8 *)dup_binstr(config->u.ca_cert, ssid->eap.identity_len);
		break;

	case WLAN_STA_FIELD_CA_CERT:
		ssid->eap.ca_cert = os_strdup(config->u.ca_cert);
		break;

	case WLAN_STA_FIELD_CLIENT_CERT:
		ssid->eap.client_cert = os_strdup(config->u.client_cert);
		break;

	case WLAN_STA_FIELD_PRIVATE_KEY:
		ssid->eap.private_key = os_strdup(config->u.private_key);
		break;

	case WLAN_STA_FIELD_PRIVATE_KEY_PASSWD:
		ssid->eap.private_key_passwd = os_strdup(config->u.private_key_passwd);
		break;

	case WLAN_STA_FIELD_PHASE1:
		ssid->eap.phase1 = os_strdup(config->u.phase1);
		break;
#endif

	default:
		return -1;
	}

	return 0;
}


int wpa_supplicant_ctrl_iface_update_psk(struct wpa_supplicant *wpa_s, wlan_gen_psk_param_t *psk)
{
	struct wpa_ssid *ssid;
	int id = 0;
#ifdef CONFIG_WPA_SUPPLICANT_MULTI_NETWORK
	id = config->id;
#endif

	ssid = wpa_config_get_network(wpa_s->conf, id);
	if (ssid == NULL) {
		wpa_printf(MSG_DEBUG, "CTRL_IFACE: Could not find network "
			   "id=%d", id);
		return -1;
	}

	/* if ssid and passphrase match, try to connect to ap */
	if (ssid->ssid_len == psk->ssid_len &&
		!os_memcmp(ssid->ssid, psk->ssid, ssid->ssid_len) &&
		!os_strcmp(ssid->passphrase, psk->passphrase)) {

		/* copy caculated PSK from pskc thread */
		os_memcpy(ssid->psk, psk->psk, PMK_LEN);
		ssid->psk_set = 1;
		ssid->mem_only_psk = 0;

		/* reconnect to ap */
		if (!radio_work_pending(wpa_s, "scan"))
			wpas_select_network_from_last_scan(wpa_s, 0, 1);
	}

	return 0;
}


#if CONFIG_STA_VSIE
/**
 * frame: type of frame
 */
static int wpas_ctrl_vendor_elem_add(struct wpa_supplicant *wpa_s, int frame, const uint8_t *ie, size_t len)
{
	struct wpabuf *buf;
	struct ieee802_11_elems elems;

	if (frame < 0 || frame >= NUM_VENDOR_ELEM_FRAMES)
		return -1;
	wpa_s = wpas_vendor_elem(wpa_s, frame);

	if (len == 0) {
		/* clear vendor ie */
		if (wpa_s->vendor_elem[frame]) {
			wpabuf_free(wpa_s->vendor_elem[frame]);
			wpa_s->vendor_elem[frame] = NULL;
		}
		return 0;
	}

	buf = wpabuf_alloc_copy(ie, len);
	if (buf == NULL)
		return -1;

	if (ieee802_11_parse_elems(wpabuf_head_u8(buf), len, &elems, 0) ==
	    ParseFailed) {
		wpabuf_free(buf);
		return -1;
	}

	if (wpa_s->vendor_elem[frame] == NULL) {
		wpa_s->vendor_elem[frame] = buf;
		goto update_ies;
	}

	if (wpabuf_resize(&wpa_s->vendor_elem[frame], len) < 0) {
		wpabuf_free(buf);
		return -1;
	}

	wpabuf_put_buf(wpa_s->vendor_elem[frame], buf);
	wpabuf_free(buf);

update_ies:
	wpas_vendor_elem_update(wpa_s);

	if (frame == VENDOR_ELEM_PROBE_REQ ||
	    frame == VENDOR_ELEM_PROBE_REQ_P2P)
		wpa_supplicant_set_default_scan_ies(wpa_s);

	return 0;
}
#endif

#ifdef CONFIG_P2P
static int p2p_ctrl_find(struct wpa_supplicant *wpa_s, char *cmd)
{
	unsigned int timeout = atoi(cmd);
	enum p2p_discovery_type type = P2P_FIND_START_WITH_FULL;
	u8 dev_id[ETH_ALEN], *_dev_id = NULL;
	u8 dev_type[WPS_DEV_TYPE_LEN], *_dev_type = NULL;
	char *pos;
	unsigned int search_delay;
	const char *_seek[P2P_MAX_QUERY_HASH + 1], **seek = NULL;
	u8 seek_count = 0;
	int freq = 0;

	if (wpa_s->wpa_state == WPA_INTERFACE_DISABLED) {
		wpa_dbg(wpa_s, MSG_INFO,
			"Reject P2P_FIND since interface is disabled");
		return -1;
	}
	if (os_strstr(cmd, "type=social"))
		type = P2P_FIND_ONLY_SOCIAL;
	else if (os_strstr(cmd, "type=progressive"))
		type = P2P_FIND_PROGRESSIVE;

	pos = os_strstr(cmd, "dev_id=");
	if (pos) {
		pos += 7;
		if (hwaddr_aton(pos, dev_id))
			return -1;
		_dev_id = dev_id;
	}

	pos = os_strstr(cmd, "dev_type=");
	if (pos) {
		pos += 9;
		if (wps_dev_type_str2bin(pos, dev_type) < 0)
			return -1;
		_dev_type = dev_type;
	}

	pos = os_strstr(cmd, "delay=");
	if (pos) {
		pos += 6;
		search_delay = atoi(pos);
	} else
		search_delay = wpas_p2p_search_delay(wpa_s);

	pos = os_strstr(cmd, "freq=");
	if (pos) {
		pos += 5;
		freq = atoi(pos);
		if (freq <= 0)
			return -1;
	}

	/* Must be searched for last, because it adds nul termination */
	pos = os_strstr(cmd, " seek=");
	if (pos)
		pos += 6;
	while (pos && seek_count < P2P_MAX_QUERY_HASH + 1) {
		char *term;

		_seek[seek_count++] = pos;
		seek = _seek;
		term = os_strchr(pos, ' ');
		if (!term)
			break;
		*term = '\0';
		pos = os_strstr(term + 1, "seek=");
		if (pos)
			pos += 5;
	}
	if (seek_count > P2P_MAX_QUERY_HASH) {
		seek[0] = NULL;
		seek_count = 1;
	}

	return wpas_p2p_find(wpa_s, timeout, type, _dev_type != NULL, _dev_type,
			     _dev_id, search_delay, seek_count, seek, freq);
}

static int p2p_ctrl_listen(struct wpa_supplicant *wpa_s, char *cmd)
{
	unsigned int timeout = atoi(cmd);
	if (wpa_s->wpa_state == WPA_INTERFACE_DISABLED) {
		wpa_dbg(wpa_s, MSG_INFO,
			"Reject P2P_LISTEN since interface is disabled");
		return -1;
	}
	return wpas_p2p_listen(wpa_s, timeout);
}

static int p2p_ctrl_connect(struct wpa_supplicant *wpa_s,
		struct wlan_p2p_connect_param *param)
{
	u8 addr[ETH_ALEN];
	char *pin = NULL;
	enum p2p_wps_method wps_method;
	int new_pin;
	int persistent_group, persistent_id = -1;
	int join;
	int auth;
	int automatic;
	int go_intent = -1;
	int freq = 0;
	int pd;
	int ht40, vht, max_oper_chwidth, freq2 = 0;
	u8 *group_ssid = NULL;
	size_t group_ssid_len = 0;
	int he;

	if (!wpa_s->global->p2p_init_wpa_s)
		return -1;
	if (wpa_s->global->p2p_init_wpa_s != wpa_s) {
		wpa_dbg(wpa_s, MSG_DEBUG, "Direct P2P_CONNECT command to %s",
			wpa_s->global->p2p_init_wpa_s->ifname);
		wpa_s = wpa_s->global->p2p_init_wpa_s;
	}

	/* <addr> <"pbc" | "pin" | PIN> [label|display|keypad|p2ps]
	 * [persistent|persistent=<network id>]
	 * [join] [auth] [go_intent=<0..15>] [freq=<in MHz>] [provdisc]
	 * [ht40] [vht] [he] [auto] [ssid=<hexdump>] */
	os_memcpy(addr, param->addr, ETH_ALEN);

	persistent_group = 0;
	join = 0;
	auth = 0;
	automatic = 0;
	pd = 0;
	vht = wpa_s->conf->p2p_go_vht;
	ht40 = wpa_s->conf->p2p_go_ht40 || vht;
	he = wpa_s->conf->p2p_go_he;

	go_intent = param->intent;

	max_oper_chwidth = CHANWIDTH_USE_HT;

	wps_method = WPS_PBC;

	new_pin = wpas_p2p_connect(wpa_s, addr, pin, wps_method,
				   persistent_group, automatic, join,
				   auth, go_intent, freq, freq2, persistent_id,
				   pd, ht40, vht, max_oper_chwidth, he,
				   group_ssid, group_ssid_len);

	return new_pin;
}
#endif /* CONFIG_P2P */


int hostapd_ctrl_iface_deauthenticate(struct hostapd_data *hapd,
					const uint8_t *addr, u16 reason)
{
	struct sta_info *sta;

	wpa_dbg(hapd->msg_ctx, MSG_DEBUG, "CTRL_IFACE DEAUTHENTICATE %pM", addr);

	hostapd_drv_sta_deauth(hapd, addr, reason);
	sta = ap_get_sta(hapd, addr);
	if (sta)
		ap_sta_deauthenticate(hapd, sta, reason);
	else if (addr[0] == 0xff)
		hostapd_free_stas(hapd);

	return 0;
}


#ifdef CONFIG_HOSTAPD_ACL
void hostapd_disassoc_deny_mac(struct hostapd_data *hapd)
{
	struct sta_info *sta;
	struct vlan_description vlan_id;

	for (sta = hapd->sta_list; sta; sta = sta->next) {
		if (hostapd_maclist_found(hapd->conf->deny_mac,
					  hapd->conf->num_deny_mac, sta->addr,
					  &vlan_id) &&
		    (!vlan_id.notempty ||
		     !vlan_compare(&vlan_id, sta->vlan_desc)))
			ap_sta_disconnect(hapd, sta, sta->addr,
					  WLAN_REASON_UNSPECIFIED);
	}
}
#endif

static int wpa_supplicant_ctrl_iface_get_network(struct wpa_supplicant *wpa_s, wlan_sta_config_t *config)
{
	struct wpa_ssid *ssid;
	struct wlan_fast_connect_info fci = {0};
	int id = 0;
	unsigned char *get_psk = 0;

	ssid = wpa_config_get_network(wpa_s->conf, id);
	if (ssid == NULL) {
		wpa_printf(MSG_DEBUG, "CTRL_IFACE: Could not find network "
			   "id=%d", id);
		return -1;
	}
	switch (config->field) {
	case WLAN_STA_FIELD_SSID:
		wpa_printf(MSG_DEBUG, "wpa_supplicant_ctrl_iface_get_network ssid_len:%d", ssid->ssid_len);
		if (ssid->ssid && ssid->ssid_len) {
			config->u.ssid.ssid_len = ssid->ssid_len;
			os_memcpy(config->u.ssid.ssid, ssid->ssid, ssid->ssid_len);
			config->u.ssid.ssid[ssid->ssid_len] = 0;
		}
		break;

	case WLAN_STA_FIELD_PSK:
		os_memset(&fci, 0, sizeof(fci));
#if CONFIG_EASY_FLASH_FAST_CONNECT
		bk_get_env_enhance("fast_connect_id", (void *)&fci, sizeof(struct wlan_fast_connect_info));
#endif
		get_psk = fci.psk;
		uint8_t psk_len = os_strlen((const char *)get_psk);
		uint8_t psk_max_len = 64;
		if (psk_len >= 8 && psk_len <= psk_max_len) {
			os_strlcpy((char *)config->u.psk, (const char *)get_psk, sizeof(config->u.psk));
			os_strlcpy((char *)(g_sta_param_ptr->key), (const char *)get_psk, sizeof(config->u.psk));
			g_sta_param_ptr->key_len = os_strlen((const char *)get_psk);
		}
		break;

	default:
		wpa_printf(MSG_DEBUG, "not support the filed:%d", config->field);
		break;
	}
	return 0;
}

static int supplicant_started = 0;
static int hostapd_started = 0;

__INLINE int phy_freq_to_channel(uint8_t band, uint16_t freq)
{
	if ((band == PHY_BAND_2G4) && (freq >= 2412) && (freq <= 2484)) {
		if (freq == 2484)
			return 14;
		else
			return (freq - 2407) / 5;
	} else if ((band == PHY_BAND_5G) && (freq >= 5005) && (freq <= 5825))
		return (freq - PHY_FREQ_5G) / 5;

	return 0;
}

int wpa_supplicant_ctrl_iface_receive(wpah_msg_t *msg)
{
	struct wpa_supplicant *wpa_s = wpa_suppliant_ctrl_get_wpas();
	struct hapd_interfaces *interfaces = hostapd_ctrl_get_interfaces();
	int res = 0;

#define CHECK_WPA_S()				\
	do {							\
		if (!wpa_s) {\
			res = WPA_ERR_WPAS_INIT;\
			goto exit;		\
		}\
	} while (0)

#define CHECK_HAPD()				\
	do {							\
		if (!interfaces || !interfaces->count) {\
			res = WPA_ERR_HAPD_INIT; \
			goto exit;				\
		}\
	} while (0)


#if CONFIG_QUICK_TRACK
	WPA_LOGI("%s: cmd %d\n", __func__, msg->cmd);
#endif
	switch (msg->cmd) {
#ifdef CONFIG_WPA_SUPPLICANT_MULTI_NETWORK
	case WPA_CTRL_CMD_ADD_NETWORK:
		CHECK_WPA_S();
		wpa_supplicant_ctrl_iface_add_network(wpa_s, (int)msg->argu);
		break;
	case WPA_CTRL_CMD_REMOVE_NETWORK:
		CHECK_WPA_S();
		wpa_supplicant_ctrl_iface_remove_network(wpa_s, (int)msg->argu);
		break;
#endif
	case WPA_CTRL_CMD_SELECT_NETWORK: {
		res = -1;
		CHECK_WPA_S();

		res = wpa_supplicant_ctrl_iface_select_network(wpa_s, 0/*(int)msg->param*/, (int)msg->argu);
	}	break;

	case WPA_CTRL_CMD_STA_SCAN:
		CHECK_WPA_S();
		res = wpas_ctrl_scan(wpa_s, (wlan_sta_scan_param_t *)msg->argu);
		break;
	case WPA_CTRL_CMD_STA_SCAN_INTERVAL: {
		CHECK_WPA_S();
		int sec = (int)msg->argu;

		if (sec < 0) {
			res = -1;
			break;
		}
		//WPA_LOGI("set scan interval from %d to %d\n", wpa_s->scan_interval, sec);
		wpa_supplicant_update_scan_int(wpa_s, sec);
		if (wpa_s->conf)
			wpa_s->conf->auth_dur = sec;
	}	break;
//	case WPA_CTRL_CMD_STA_CONNECT:
//		CHECK_WPA_S();
//		wpa_supplicant_ctrl_iface_select_network(wpa_s, (int)msg->argu);
//		break;
	case WPA_CTRL_CMD_STA_REASSOCIATE:
		CHECK_WPA_S();
		if (wpa_s->disconnected)
			wpas_request_connection(wpa_s);
		break;
	case WPA_CTRL_CMD_STA_DISCONNECT:
		CHECK_WPA_S();

#ifdef CONFIG_SME
		wpa_s->sme.prev_bssid_set = 0;
#endif /* CONFIG_SME */
		wpa_s->reassociate = 0;
		wpa_s->disconnected = 1;
		wpa_supplicant_cancel_sched_scan(wpa_s);
		wpa_supplicant_cancel_scan(wpa_s);
		wpa_supplicant_deauthenticate(wpa_s,
						  WLAN_REASON_DEAUTH_LEAVING);
		eloop_cancel_timeout(wpas_network_reenabled, wpa_s, NULL);
		break;
	case WPA_CTRL_CMD_STA_ENABLE:
		/* Enable the station */
		if (!supplicant_started) {
			uint8_t mac[ETH_ALEN];

			//sa_station_init();

			res = supplicant_main_entry(NULL);
			if (!res)
				supplicant_started = 1;
			bk_wifi_sta_get_mac((uint8_t *)mac);
#if CONFIG_QUICK_TRACK
			os_memcpy(&g_sta_param_ptr->own_mac, mac, ETH_ALEN);
#endif
#if CONFIG_LWIP
#if !CONFIG_FIXED_NETIF
			net_wlan_add_netif(mac);
#else
			//wifi_netif_set_vif_private_data(wifi_netif_vifid_to_vif(0), net_get_sta_handle());
#endif
#endif
		}
		break;
	case WPA_CTRL_CMD_STA_DISABLE:
		/* disable the station */
		if (supplicant_started) {
#if CONFIG_LWIP
#if !CONFIG_FIXED_NETIF
			net_wlan_remove_netif((uint8_t*)&g_sta_param_ptr->own_mac);
#endif
#endif
			supplicant_main_exit();
			wpa_hostapd_release_scan_rst();
			supplicant_started = 0;
		}
		break;
	case WPA_CTRL_CMD_STA_SET: {
		CHECK_WPA_S();
		wlan_sta_config_t *config = (wlan_sta_config_t *)msg->argu;
		*msg->result = wpa_supplicant_ctrl_iface_set_network(wpa_s, config);
	}	break;
	case WPA_CTRL_CMD_WPAS_SET: {
		CHECK_WPA_S();
		bool reconnect = (bool)msg->argu;
		wpa_s->reconnect = reconnect;
	}	break;
	case WPA_CTRL_CMD_STA_GET: {
		CHECK_WPA_S();
		wlan_sta_config_t *config = (wlan_sta_config_t *)msg->argu;
		*msg->result = wpa_supplicant_ctrl_iface_get_network(wpa_s, config);
	}	break;
	case WPA_CTRL_CMD_STA_AUTORECONNECT: {
		CHECK_WPA_S();
#if defined(BK_SUPPLICANT) && defined(CONFIG_AUTO_RECONNECT)
		wlan_auto_reconnect_t *config = (wlan_auto_reconnect_t *)msg->argu;
		wpa_s->auto_reconnect_max_count = config->max_count;
		wpa_s->auto_reconnect_timeout = config->timeout;
		wpa_s->disable_auto_reconnect_after_disconnect = config->disable_reconnect_when_disconnect;
		WPA_LOGI("auto_reconnect: count %d, timeout %d, disable_auto_reconnect_after_disconnect %d\n",
			wpa_s->auto_reconnect_max_count, wpa_s->auto_reconnect_timeout,
			wpa_s->disable_auto_reconnect_after_disconnect);
#endif
	}	break;
	case WPA_CTRL_CMD_STA_STATE: {
		wlan_sta_states_t *state = (wlan_sta_states_t *)msg->argu;
		CHECK_WPA_S();
		if (wpa_s->wpa_state == WPA_COMPLETED)
			*state = WLAN_STA_STATE_CONNECTED;
		else
			*state = WLAN_STA_STATE_DISCONNECTED;
	}	break;
	case WPA_CTRL_CMD_STA_AP: {
		/* Get the information of connected AP */
		struct ApListStruct *ap = (struct ApListStruct *)msg->argu;
		CHECK_WPA_S();

		if (wpa_s->wpa_state >= WPA_ASSOCIATED) {
			struct wpa_ssid *ssid = wpa_s->current_ssid;
			struct wpa_bss *bss = wpa_s->current_bss;
			memcpy(ap->bssid, wpa_s->bssid, ETH_ALEN);
			memcpy(ap->ssid, ssid->ssid, ssid->ssid_len);	/* FIXME: Overflow */
			ap->ssid[ssid->ssid_len] = 0;

			if (wpa_s->assoc_freq >= 5925) {
				// FIXME: BK7239 6E
			} else if (wpa_s->assoc_freq >= 4900) {
				ap->channel = phy_freq_to_channel(PHY_BAND_5G, wpa_s->assoc_freq);
			} else {
				ap->channel = phy_freq_to_channel(PHY_BAND_2G4, wpa_s->assoc_freq);
			}
			ap->security = get_security_type_from_ie(bss->ies, bss->ie_len, bss->caps);
		} else {
			res = -1;
		}
	}	break;
	case WPA_CTRL_CMD_STA_BSS_MAX_COUNT:
		CHECK_WPA_S();
		break;
	case WPA_CTRL_CMD_STA_BSS_FLUSH:
		CHECK_WPA_S();
		wpa_bss_flush(wpa_s);
		break;
	case WPA_CTRL_CMD_STA_BSS_SIZE_GET:
		CHECK_WPA_S();
		break;
	case WPA_CTRL_CMD_STA_BSS_GET:
		CHECK_WPA_S();
		break;
	case WPA_CTRL_CMD_STA_BSS_SET:
		CHECK_WPA_S();
		break;
	case WPA_CTRL_CMD_STA_GEN_PSK: {
		CHECK_WPA_S();
		wlan_gen_psk_param_t *psk = (wlan_gen_psk_param_t *)msg->argu;
		wpa_supplicant_ctrl_iface_update_psk(wpa_s, psk);
	}	break;
	case WPA_CTRL_CMD_STA_ADD_BSS: {
		/* add or update bss */
		// refer: wpa_get_scan_rst
		CHECK_WPA_S();
		wlan_sta_add_bss_t *add_req = (wlan_sta_add_bss_t *)msg->argu;
		struct wpa_scan_res *r;
		struct os_reltime fetch_time;

		res = -1;
		r = os_zalloc(sizeof(*r) + add_req->ie_len);
		if (!r)
			break;

		os_get_reltime(&fetch_time);
		os_memcpy(r->bssid, add_req->bssid, ETH_ALEN);
		r->level = add_req->level;  // FIXME
		r->freq = add_req->freq;
		r->beacon_int = add_req->beacon_int; // FIXME
		r->caps = add_req->caps;
		r->tsf = 0; // FIXME
		r->ie_len = add_req->ie_len;
		os_memcpy(r + 1, add_req->ies, r->ie_len);

		wpa_bss_update_scan_res(wpa_s, r, &fetch_time);
		wpa_bss_update_end(wpa_s, NULL, 1);

		os_free(r);
	}	break;

	case WPA_CTRL_CMD_STA_RX_BEACON: {
		wlan_sta_new_rx_beacon_t *bcn = (wlan_sta_new_rx_beacon_t *)msg->argu;
		// wlan_store_fci
		os_free(bcn);
	}	break;

#if CONFIG_WLAN_FAST_CONNECT_WPA3
	case WPA_CTRL_CMD_STA_ADD_PMKSA_CACHE_ENTRY: {
		// sme_sae_set_pmk
		CHECK_WPA_S();

		wlan_sta_add_pmksa_cache_entry_t *entry = (wlan_sta_add_pmksa_cache_entry_t *)msg->argu;
		struct wpa_ssid *ssid = wpa_config_get_network(wpa_s->conf, 0);

		pmksa_cache_add(wpa_s->wpa->pmksa, entry->pmk, entry->pmk_len,
							entry->pmkid, NULL, 0, entry->bssid,
							wpa_s->wpa->own_addr,
							ssid, entry->akmp,
							NULL);
	}	break;
#endif

	case WPA_CTRL_CMD_STA_SCAN_RESULTS: {
		CHECK_WPA_S();
		res = wpa_supplicant_ctrl_iface_scan_results(wpa_s, (ScanResult_adv *)msg->argu);
	}	break;
	case WPA_CTRL_CMD_STA_WPS_PBC: {
		CHECK_WPA_S();
#ifdef CONFIG_WPS
		wpas_wps_start_pbc(wpa_s, NULL, 0, 0);
#endif
	}	break;
	case WPA_CTRL_CMD_STA_WPS_GET_PIN: {
		CHECK_WPA_S();
#ifdef CONFIG_WPS
		wlan_sta_wps_pin_t *wps = (wlan_sta_wps_pin_t *)msg->argu;
		unsigned int pin;
		if (wps_generate_pin((unsigned int *) &pin) < 0) {
			res = -1;
			break;
		}
		snprintf(wps->pin, sizeof(wps->pin), "%08d", pin);
#else
		res = -1;
#endif
	}	break;
	case WPA_CTRL_CMD_STA_WPS_SET_PIN: {
		CHECK_WPA_S();
#ifdef CONFIG_WPS
		wlan_sta_wps_pin_t *wps = (wlan_sta_wps_pin_t *)msg->argu;
		if (!wps_pin_str_valid(wps->pin)) {
			res = -1;
			break;
		}
		res = wpas_wps_start_pin(wpa_s, NULL, wps->pin, 0, DEV_PW_DEFAULT);
#else
		res = -1;
#endif
	}	break;

#if CONFIG_STA_VSIE
	case WPA_CTRL_CMD_STA_SET_VSIE: {
		CHECK_WPA_S();
		wlan_sta_vsie_t *vsie = (wlan_sta_vsie_t *)msg->argu;

		res = wpas_ctrl_vendor_elem_add(wpa_s, vsie->frame, vsie->vsie, vsie->len);

	}	break;
#endif

	case WPA_CTRL_CMD_STA_TWT_SETUP:
		CHECK_WPA_S();
		break;
	case WPA_CTRL_CMD_STA_TWT_TEARDOWN: {
		CHECK_WPA_S();
	}
		break;

	case WPA_CTRL_CMD_STA_GET_ASSOC_STATUS: {
		CHECK_WPA_S();
		int *status_code = (int *)msg->argu;
		*status_code = wpa_s->assoc_status_code;
	}
		break;

	/* ============== softap ================= */
	case WPA_CTRL_CMD_AP_ENABLE:
		/* Enable the AP */
		//hostapd_main_entry(2, 0);
		if (!hostapd_started) {
			if (hostapd_main_entry(1, 0)) {
				WPA_LOGE("hostapd main entry fail!\n");
				res = -1;
				break;
			}
			// don't add any interface, just a dummy ctrl iface
			hostapd_started = 1;
		}
		break;

	/* Reload AP configuration */
	case WPA_CTRL_CMD_AP_RELOAD: {
		int i;

		if (!interfaces) {
			res = -1;
			break;
		}

		// try to add a new hostapd_iface if not exist
		if (!interfaces->count) {
			char bss_config[] = "bss_config=phy0:dummy.conf";
			res = hostapd_add_iface(interfaces, bss_config);
			if (res) {
				WPA_LOGE("%s:add iface fail!\n", __func__);
				res = -1;
				break;
			}
			/* start receiving mgmt frames */
			// rw_msg_send_apm_start_done_ind(true);

#if CONFIG_LWIP
#if !CONFIG_FIXED_NETIF
			if(net_wlan_add_netif(&g_ap_param_ptr->bssid)) {
				WPA_LOGE("%s:add netif fail!\n", __func__);
				res = -1;
				break;
			}
#else
			//wifi_netif_set_vif_private_data(wifi_netif_vifid_to_vif(1), net_get_uap_handle());
#endif
#endif

			/* new added iface, just return*/
			break;
		}

		/* reload configuration */
		if (interfaces->reload_config) {
			/* stop receiving mgmt frames */
			// rw_msg_send_apm_start_done_ind(false);
			for (i = 0; i < interfaces->count; i++) {
				res = interfaces->reload_config(interfaces->iface[i]);
				if (res) {
					res = -1;
					break;
				}
			}
			/* start receiving mgmt frames */
			// rw_msg_send_apm_start_done_ind(true);
		}
	}	break;

	case WPA_CTRL_CMD_AP_DISABLE: {
#if CONFIG_LWIP
		//uap_ip_down();
#if !CONFIG_FIXED_NETIF
		if(net_wlan_remove_netif((uint8_t*)&g_ap_param_ptr->bssid)) {
			WPA_LOGE("%s:remove netif fail!\n", __func__);
			res = -1;
			break;
		}
#endif
#endif
		if (hostapd_started) {
			hostapd_main_exit();
			hostapd_started = 0;
		}

		struct rwnx_hw *rwnx_hw = &g_rwnx_hw;
		if(rwnx_hw->csa)
		{
			os_free(rwnx_hw->csa->bcn_ptr);
			os_free(rwnx_hw->csa);
			rwnx_hw->csa = 0;
		}
	}	break;

	case WPA_CTRL_CMD_AP_SET:
		//hostapd_set_iface -> hostapd_config_fill
		break;
	case WPA_CTRL_CMD_AP_GET:
		break;
	case WPA_CTRL_CMD_AP_STA_NUM:
		break;
	case WPA_CTRL_CMD_AP_STA_INFO: {
		wlan_ap_stas_t *stas = (wlan_ap_stas_t *)msg->argu;
		extern char *dhcp_lookup_mac(uint8_t *addr);
		struct hostapd_data *hapd;
		struct sta_info *sta;
		int k;
		struct hostap_sta_driver_data data;

		res = -1;

		CHECK_HAPD();

		if (!interfaces->iface[0]->bss)
			break;
		hapd = interfaces->iface[0]->bss[0];
		stas->num = hapd->num_sta;
		stas->sta = 0;
		if (stas->num > 0) {
			stas->sta = os_malloc(stas->num * sizeof(wlan_ap_sta_t));
			if (!stas->sta)
				break;

			for (k = 0, sta = hapd->sta_list; sta; sta = sta->next, k++) {
				// MAC address
				os_memcpy(stas->sta[k].addr, sta->addr, ETH_ALEN);

				// IP address
				stas->sta[k].ipaddr = bk_lookup_ipaddr_wrapper(sta->addr);
				// RSSI
				hostapd_drv_read_sta_data(hapd, &data, sta->addr);
				stas->sta[k].rssi = data.signal;
			}
		}
		res = 0;
	}	break;

#ifdef CONFIG_HOSTAPD_ACL
	case WPA_CTRL_CMD_AP_ADD_BLACKLIST: {
		wlan_ap_blacklist_t *req = (wlan_ap_blacklist_t *)msg->argu;
		struct hostapd_data *hapd;

		CHECK_HAPD();

		if (!interfaces->iface[0] || !interfaces->iface[0]->bss[0])
			goto exit;
		hapd = interfaces->iface[0]->bss[0];

		res = hostapd_ctrl_iface_acl_add_mac(&hapd->conf->deny_mac,
				  &hapd->conf->num_deny_mac, req->addr);
		if (!res) {
			hostapd_disassoc_deny_mac(hapd);
		}
	}	break;

	case WPA_CTRL_CMD_AP_DEL_BLACKLIST: {
		wlan_ap_blacklist_t *req = (wlan_ap_blacklist_t *)msg->argu;
		struct hostapd_data *hapd;

		CHECK_HAPD();

		if (!interfaces->iface[0] || !interfaces->iface[0]->bss[0])
			goto exit;
		hapd = interfaces->iface[0]->bss[0];

		res = hostapd_ctrl_iface_acl_del_mac(&hapd->conf->deny_mac,
				  &hapd->conf->num_deny_mac, req->addr);
	}	break;

	case WPA_CTRL_CMD_AP_CLEAR_BLACKLIST: {
		struct hostapd_data *hapd;

		CHECK_HAPD();

		if (!interfaces->iface[0] || !interfaces->iface[0]->bss[0])
			goto exit;
		hapd = interfaces->iface[0]->bss[0];

		hostapd_ctrl_iface_acl_clear_list(&hapd->conf->deny_mac,
				  &hapd->conf->num_deny_mac);
		res = 0;
	}	break;
#endif

	/* channel switch request */
	case WPA_CTRL_CMD_AP_CHAN_SWITCH: {
		int new_freq = (int)msg->argu;

		res = -1;
		CHECK_HAPD();
		res = ap_channel_switch(interfaces->iface[0], new_freq);
	}	break;

	case WPA_CTRL_CMD_AP_STA_DEAUTH: {
		wlan_ap_sta_deauth_t *req = (wlan_ap_sta_deauth_t *)msg->argu;

		CHECK_HAPD();

		if (!interfaces->iface[0] || !interfaces->iface[0]->bss[0])
			goto exit;

		res = hostapd_ctrl_iface_deauthenticate(
				interfaces->iface[0]->bss[0],
				req->addr,
				req->reason <= 0 ? WLAN_REASON_PREV_AUTH_NOT_VALID : req->reason);
	}	break;

#if CONFIG_AP_VSIE
	case WPA_CTRL_CMD_UPDATE_BEACON: {
		wlan_ap_vsie_t *vsie = (wlan_ap_vsie_t *)msg->argu;
		struct hostapd_data *hapd;
		struct hostapd_iface *iface;
		CHECK_HAPD();

		res = -1;
		iface = interfaces->iface[0];
		if (!iface)
			break;
		hapd = iface->bss ? iface->bss[0] : NULL;
		if (!hapd)
			break;

		wpabuf_free(hapd->conf->vendor_elements);
		hapd->conf->vendor_elements = NULL;
		if (vsie->len) {
			hapd->conf->vendor_elements =
				wpabuf_alloc_copy(vsie->vsie, vsie->len);
		}

		res = ieee802_11_set_beacon(hapd);

	}	break;
#endif

#ifdef CONFIG_P2P
	case WPA_CTRL_CMD_P2P_LISTEN:
		CHECK_WPA_S();
		p2p_ctrl_listen(wpa_s, "");
		break;
	case WPA_CTRL_CMD_P2P_FIND:
		CHECK_WPA_S();
		p2p_ctrl_find(wpa_s, "");
		break;
	case WPA_CTRL_CMD_P2P_STOP_FIND:
		CHECK_WPA_S();
		wpas_p2p_stop_find(wpa_s);
		break;
	case WPA_CTRL_CMD_P2P_CONNECT: {
		CHECK_WPA_S();
		struct wlan_p2p_connect_param *param = (struct wlan_p2p_connect_param *)msg->argu;
		p2p_ctrl_connect(wpa_s, param);
	}	break;
	case WPA_CTRL_CMD_P2P_CANCEL:
		CHECK_WPA_S();
		wpas_p2p_disconnect(wpa_s);
		break;
#endif

	/* ============== MISC ======== */
	case WPA_CTRL_CMD_DEBUG_INFO_DUMP: {
		CHECK_WPA_S();
		wpa_ctrl_debug_info_dump(wpa_s, (uint32_t)msg->argu);
		break;
	}
	}

exit:
	if (msg->result)
		*msg->result = res;	//FIXME
	if (msg->sema)
		rtos_set_semaphore(&msg->sema);
	if (msg->flags & WPAH_FLAG_FREE)
		os_free((void *)msg->argu);

#undef CHECK_WPA_S
#undef CHECK_HAPD

	return res;
}

static void mlme_event_unprot_disconnect(struct wpa_supplicant *wpa_s,
					 enum wpa_event_type type,
					 const u8 *frame, size_t len)
{
	const struct ieee80211_mgmt *mgmt;
	union wpa_event_data event;
	u16 reason_code = 0;

	if (type == EVENT_UNPROT_DEAUTH)
		wpa_printf(MSG_DEBUG, "Unprot Deauthenticate event");
	else
		wpa_printf(MSG_DEBUG, "Unprot Disassociate event");

	if (len < 24)
		return;

	mgmt = (const struct ieee80211_mgmt *) frame;

	os_memset(&event, 0, sizeof(event));
	/* Note: Same offset for Reason Code in both frame subtypes */
	if (len >= 24 + sizeof(mgmt->u.deauth))
		reason_code = le_to_host16(mgmt->u.deauth.reason_code);

	if (type == EVENT_UNPROT_DISASSOC) {
		event.unprot_disassoc.sa = mgmt->sa;
		event.unprot_disassoc.da = mgmt->da;
		event.unprot_disassoc.reason_code = reason_code;
	} else {
		event.unprot_deauth.sa = mgmt->sa;
		event.unprot_deauth.da = mgmt->da;
		event.unprot_deauth.reason_code = reason_code;
	}

	wpa_supplicant_event_sta(wpa_s, type, &event);
}

int wpa_supplicant_handle_events(wpah_msg_t *msg)
{
	struct wpa_supplicant *wpa_s = wpa_suppliant_ctrl_get_wpas();
	struct hapd_interfaces *interfaces = hostapd_ctrl_get_interfaces();
	int res = 0;

	// WPA_LOGI("%s: evt %d\r\n", __func__, msg->cmd);

#define CHECK_WPA_S()				\
	do {							\
		if (!wpa_s) goto exit;		\
	} while (0)

#define CHECK_HAPD()\
	do {							\
		if (!interfaces || !interfaces->count) {\
			res = WPA_ERR_HAPD_INIT; \
			goto exit;				\
		}\
	} while (0)


	/* CHAN_SWITCH_IND and CHAN_SWITCH_START_IND belong to hostpad */
	if (msg->cmd == WPA_CTRL_EVENT_CHAN_SWITCH_IND ||
		msg->cmd == WPA_CTRL_EVENT_CHAN_SWITCH_START_IND ||
		msg->cmd == WPA_CTRL_EVENT_PROBE_STATUS ||
		msg->cmd == WPA_CTRL_EVENT_DEL_STATION)
		CHECK_HAPD();
	else if (msg->cmd == WPA_CTRL_EVENT_UNPROT_DEAUTHENTICATE ||
		msg->cmd == WPA_CTRL_EVENT_UNPROT_DISASSOCIATE) {
		if (!wpa_s) {
			struct pbuf *p = (struct pbuf *)msg->argu;
			pbuf_free(p);
			goto exit;
		}
	} else
		CHECK_WPA_S();

	switch (msg->cmd) {
	case WPA_CTRL_EVENT_SCAN_STARTED: {
		wpa_supplicant_event_sta(wpa_s, EVENT_SCAN_STARTED, NULL);
	}	break;

	case WPA_CTRL_EVENT_SCAN_RESULTS: {
		eloop_cancel_timeout(wpa_driver_scan_timeout, wpa_s, NULL);
		wpa_supplicant_event_sta(wpa_s, EVENT_SCAN_RESULTS, NULL);
	}	break;

	case WPA_CTRL_EVENT_DISCONNECT_IND: {
		struct sm_disconnect_ind *ind = (struct sm_disconnect_ind *)msg->argu;
		union wpa_event_data data;
		struct disassoc_info *info = (struct disassoc_info *)&data.disassoc_info;

#if CONFIG_WIFI_REGDOMAIN
		regulatory_hint_disconnect();
#endif

		os_memset(&data, 0, sizeof(data));
		/*
		 * FIXME: IP specified that reason code <= 1 are local generated, but AP still can send us
		 * a reason code equals 1 which we cannot distinguish local generated or not
		 */
		info->locally_generated = !!((ind->reason_code <= 1) || ind->reason_code == WIFI_REASON_BEACON_LOST);
		info->reason_code = ind->reason_code;

		// wpa_supplicant_event_sta(wpa_s, EVENT_DISASSOC, &data);
		wpa_supplicant_event_sta(wpa_s, EVENT_DEAUTH, &data);
	}	break;

	case WPA_CTRL_EVENT_AUTH_IND: {
#ifdef CONFIG_SME
	    struct sm_auth_indication *ind = (struct sm_auth_indication *)msg->argu;

		if (ind) {
			union wpa_event_data data;

			os_memset(&data, 0, sizeof(data));

			os_memcpy(data.auth.bssid, &ind->bssid, ETH_ALEN);
			os_memcpy(data.auth.peer, &ind->bssid, ETH_ALEN);
			data.auth.auth_type = ind->auth_type;
			data.auth.auth_transaction = ind->auth_transaction;
			data.auth.status_code = ind->status_code;
			data.auth.ies = ind->ie_buf;
			data.auth.ies_len = ind->ie_len;

			//WPA_LOGI("%s: WPA_CTRL_EVENT_AUTH_IND\n", __func__);
			wpa_supplicant_event_sta(wpa_s, EVENT_AUTH, &data);
		} else {
			WPA_LOGE("%s %d: null\n", __func__, __LINE__);
		}
#endif
	}	break;

#ifdef CONFIG_SAE_EXTERNAL
	case WPA_CTRL_EVENT_EXTERNAL_AUTH_IND: {
		struct sm_external_auth_required_ind *ind = (struct sm_external_auth_required_ind *)msg->argu;

		if (ind) {
			union wpa_event_data data;

			os_memset(&data, 0, sizeof(data));

			data.external_auth.ssid = ind->ssid.array;
			data.external_auth.ssid_len = ind->ssid.length;
			data.external_auth.bssid = (u8 *)&ind->bssid;
			data.external_auth.key_mgmt_suite = ind->akm;

			wpa_supplicant_event_sta(wpa_s, EVENT_EXTERNAL_AUTH, &data);
		}
	}	break;
#endif
	case WPA_CTRL_EVENT_MM_RSSI_STATUS_IND: {
		struct mm_rssi_status_ind *ind = (struct mm_rssi_status_ind *)msg->argu;

		if (ind) {
			union wpa_event_data data;

			os_memset(&data, 0, sizeof(data));
			data.signal_change.above_threshold = ind->rssi_status;
			data.signal_change.current_signal = ind->rssi;
			wpa_supplicant_event_sta(wpa_s, EVENT_SIGNAL_CHANGE, &data);
		}
	} break;

	case WPA_CTRL_EVENT_ASSOC_IND: {
#ifdef CONFIG_SME
    	struct sm_assoc_indication *ind = (struct sm_assoc_indication *)msg->argu;

		if (ind) {
			if (ind->status_code == 0) {
				/* success */
				wpa_supplicant_event_sta(wpa_s, EVENT_ASSOC, NULL);
			} else {
				union wpa_event_data data;

				os_memset(&data, 0, sizeof(data));
				data.assoc_reject.status_code = ind->status_code;

				wpa_supplicant_event_sta(wpa_s, EVENT_ASSOC_REJECT, &data);
			}
		} else {
			WPA_LOGE("%s %d: null\n", __func__, __LINE__);
		}
#endif
	}	break;

#ifdef CONFIG_IEEE80211R
	case WPA_CTRL_EVENT_FT_AUTH_IND: {
		struct sm_ft_auth_ind *ind = (struct sm_ft_auth_ind *)msg->argu;

		if (ind) {
			union wpa_event_data data;

			os_memset(&data, 0, sizeof(data));

			data.ft_ies.ies = (uint8_t *)ind->ft_ie_buf;
			data.ft_ies.ies_len = ind->ft_ie_len;

			// set RWNX_STA_FT_OVER_AIR
			wpa_drv_handle_ft_auth_response(wpa_s, &data);

			wpa_supplicant_event_sta(wpa_s, EVENT_FT_RESPONSE, &data);
		}
	}
#endif

	case WPA_CTRL_EVENT_CONNECT_IND: {
	    struct sm_connect_ind *ind = (struct sm_connect_ind *)msg->argu;

		if (ind) {
			if (ind->status_code == 0) {
				/* success, Association completed */
				union wpa_event_data data;

				os_memset(&data, 0, sizeof(data));
				data.assoc_info.freq = ind->chan.prim20_freq;
				data.assoc_info.req_ies = (u8 *)ind->assoc_ie_buf;
				data.assoc_info.req_ies_len = ind->assoc_req_ie_len;
				data.assoc_info.resp_ies = (u8 *)ind->assoc_ie_buf + ind->assoc_req_ie_len;
				data.assoc_info.resp_ies_len = ind->assoc_rsp_ie_len;
				wpa_supplicant_event_sta(wpa_s, EVENT_ASSOC, &data);

#if CONFIG_WIFI_REGDOMAIN
				// handle dot11d
				const uint8_t *country_ie = get_ie(data.assoc_info.resp_ies,
						data.assoc_info.resp_ies_len, WLAN_EID_COUNTRY);
				if (!country_ie)
					country_ie = wpa_bss_get_ie(wpa_s->current_bss, WLAN_EID_COUNTRY);
				if (country_ie)
					rwnx_regulatory_hint_11d(wpa_s->current_bss->freq, country_ie + 2, *(country_ie + 1));
#endif

			} else {
				union wpa_event_data data;

				os_memset(&data, 0, sizeof(data));
				data.assoc_reject.status_code = ind->status_code;
				data.assoc_reject.timed_out = 0; /* 0: Unspecified, Drop PMKSA Cache */

				wpa_supplicant_event_sta(wpa_s, EVENT_ASSOC_REJECT, &data);
			}
		}
	}	break;

	case WPA_CTRL_EVENT_MGMT_IND: {
		struct rxu_mgt_ind *ind = (struct rxu_mgt_ind *)msg->argu;

		if (ind) {
			union wpa_event_data data;

			//WPA_LOGI("%s: WPA_CTRL_EVENT_MGMT_IND\n", __func__);

			os_memset(&data, 0, sizeof(data));
			data.rx_mgmt.ssi_signal = ind->rssi;
			data.rx_mgmt.frame = (u8 *)ind->payload;
			data.rx_mgmt.frame_len = ind->length;
			data.rx_mgmt.freq = ind->center_freq;

			//print_hex_dump("MGMT: ", ind->payload, ind->length);
			wpa_supplicant_event_sta(wpa_s, EVENT_RX_MGMT, &data);
		} else {
			WPA_LOGI("%s %d: null\n", __func__, __LINE__);
		}
	}	break;

	case WPA_CTRL_EVENT_RX_MGMT_FRAME:
		break;

	case WPA_CTRL_EVENT_UNPROT_DEAUTHENTICATE:
		/* fall through */
	case WPA_CTRL_EVENT_UNPROT_DISASSOCIATE: {
		struct pbuf *p = (struct pbuf *)msg->argu;

		mlme_event_unprot_disconnect(wpa_s,
			msg->cmd == WPA_CTRL_EVENT_UNPROT_DEAUTHENTICATE ? EVENT_UNPROT_DEAUTH : EVENT_UNPROT_DISASSOC,
			p->payload, p->len);
		pbuf_free(p);
	}	break;

	case WPA_CTRL_EVENT_CHAN_SWITCH_START_IND:
		/* fall through */
	case WPA_CTRL_EVENT_CHAN_SWITCH_IND: {
		/* TODO: multiple bss */
		/* TODO: wpa_supplicant / hostapd may both use this indication ? */

		if (interfaces->iface && interfaces->iface[0]->bss[0]) {
			struct hostapd_data *hapd = interfaces->iface[0]->bss[0];
			struct hostapd_freq_params *freq_params = (struct hostapd_freq_params *)msg->argu;
			union wpa_event_data data;

			data.ch_switch.freq = freq_params->freq;
			data.ch_switch.ht_enabled = freq_params->ht_enabled;
			data.ch_switch.ch_offset = freq_params->sec_channel_offset;
			data.ch_switch.ch_width = freq_params->bandwidth;
			data.ch_switch.cf1 = freq_params->center_freq1;
			data.ch_switch.cf2 = freq_params->center_freq2;

			wpa_supplicant_event(hapd,
					msg->cmd == WPA_CTRL_EVENT_CHAN_SWITCH_START_IND ?
						EVENT_CH_SWITCH_STARTED : EVENT_CH_SWITCH,
					&data);
		}
	}	break;

	case WPA_CTRL_EVENT_PROBE_STATUS: {
		struct apm_probe_client_ind *ind = (struct apm_probe_client_ind *)msg->argu;

		if (!interfaces->iface || !interfaces->iface[0]->bss[0])
			break;

		if (ind->client_present && ind->sta_idx != INVALID_STA_IDX) {
			struct hostapd_data *hapd = interfaces->iface[0]->bss[0];
			union wpa_event_data data;
			STA_INF_PTR sta = sta_mgmt_get_entry(ind->sta_idx);

			os_memset(&data, 0, sizeof(data));
			os_memcpy(data.client_poll.addr, sta_mgmt_get_mac(sta), ETH_ALEN);
			wpa_supplicant_event(hapd, EVENT_DRIVER_CLIENT_POLL_OK, &data);
		}
	}	break;

#ifdef CONFIG_P2P
	case WPA_CTRL_EVENT_REMAIN_ON_CHANNEL: {
		union wpa_event_data data;
		struct rwnx_hw *rwnx_hw = &g_rwnx_hw;
		struct rwnx_roc_elem *roc_elem = rwnx_hw->roc_elem;

		os_memset(&data, 0, sizeof(data));
		data.remain_on_channel.freq = roc_elem->freq;
		data.remain_on_channel.duration = roc_elem->duration;
		wpa_supplicant_event_sta(wpa_s, EVENT_REMAIN_ON_CHANNEL, &data);
	}	break;

	case WPA_CTRL_EVENT_CANCEL_REMAIN_ON_CHANNEL: {
		union wpa_event_data data;

		os_memset(&data, 0, sizeof(data));
		data.remain_on_channel.freq = -1;  /* FIXME: P2P */
		wpa_supplicant_event_sta(wpa_s, EVENT_CANCEL_REMAIN_ON_CHANNEL, &data);
	}	break;

	case WPA_CTRL_EVENT_P2P_GO_NEG_REQUEST: {
		struct p2p_event_go_req *req = (struct p2p_event_go_req *)msg->argu;
		struct wlan_p2p_connect_param param;

		WPA_LOGI("WPA_CTRL_EVENT_P2P_GO_NEG_REQUEST: peer_addr %pm\n", req->src);
		os_memcpy(param.addr, req->src, ETH_ALEN);
		//param.intent = bk_rand()%5 + 1;
		param.intent = 0;
		param.method = WPS_PBC;
		p2p_ctrl_connect(wpa_s, &param);
	}	break;
#endif

	case WPA_CTRL_EVENT_DEL_STATION:
		if (interfaces->iface && interfaces->iface[0]->bss[0]) {
			struct hostapd_data *hapd = interfaces->iface[0]->bss[0];
			drv_event_disassoc(hapd, (u8 *)msg->argu);
		}
		break;

	default:
		break;
	}

exit:
	if (msg->result)
		*msg->result = res; //FIXME
	if (msg->sema)
		rtos_set_semaphore(&msg->sema);
	if (msg->flags & WPAH_FLAG_FREE)
		os_free((void *)msg->argu);

#undef CHECK_WPA_S
#undef CHECK_HAPD

	return 0;
}

static int wpa_ctrl_debug_info_dump_scan(struct wpa_supplicant *wpas)
{
	struct os_reltime now;

	if (!wpas)
		return WPA_ERR_PARAM;

	WPA_LOGI("wpa_state=%d\n", wpas->wpa_state);
	WPA_LOGI("scan_prev_wpa_state=%d\n", wpas->scan_prev_wpa_state);
	struct wpa_radio_work *scan_work = wpas->scan_work;
	if (scan_work) {
		os_get_reltime(&now);
		WPA_LOGI("scan_work: freq=%d type=%s start=%d bands=%d time(%d.%d) now(%d.%d)\n",
			scan_work->freq, scan_work->type, scan_work->started, scan_work->bands,
			scan_work->time.sec, scan_work->time.usec);
	}
	WPA_LOGI("scanning=%d\n", wpas->scanning);
	WPA_LOGI("sched_scanning=%d\n", wpas->sched_scanning);
	WPA_LOGI("sched_scan_stop_req=%d\n", wpas->sched_scan_stop_req);
	WPA_LOGI("disconnected=%d\n", wpas->disconnected);
	WPA_LOGI("scan_req=%d\n", wpas->scan_req);
	WPA_LOGI("last_scan_req=%d\n", wpas->last_scan_req);
	WPA_LOGI("enabled_networks=%d\n", wpa_supplicant_enabled_networks(wpas));
	WPA_LOGI("scan_for_connection=%d\n", wpas->scan_for_connection);

	return WPA_OK;
}

static int wpa_ctrl_debug_info_dump(struct wpa_supplicant *wpas, uint32_t type)
{
	if (type == WPA_CTRL_DEBUG_INFO_DUMP_SCAN)
		wpa_ctrl_debug_info_dump_scan(wpas);

	return WPA_OK;
}

int bk_get_chan_by_ssid_and_bssid(uint8_t *ssid, uint8_t *bssid, int ssid_len)
{
	struct wpa_bss *bss;
	int chan = 0;
	struct wpa_supplicant *wpa_s = wpa_suppliant_ctrl_get_wpas();
	if (NULL == wpa_s) {
		return -1;
	}
	bss = wpa_bss_get(wpa_s, bssid, ssid, ssid_len);
	if (NULL == bss) {
		return -1;
	}
	chan = bss->freq;
	return chan;
}
