/* use LSIG to do monitor */
#include <common/bk_include.h>
#include "../wpa_supplicant-2.10/src/utils/common.h"

#include "rwnx_version.h"
#include "bk_wifi.h"
#include "bk_rw.h"
#include "bk_wifi_types.h"
#include "../wpa_supplicant-2.10/src/common/ieee802_11_defs.h"
#include "../wpa_supplicant-2.10/src/drivers/driver_beken.h"
#include "sa_ap.h"
#include "../wpa_supplicant-2.10/hostapd/main_none.h"
#include "fhost_mac.h"
#include "reg_access.h"
#include "bk_wifi_private.h"

#define MAX_RTS_TBL 32

/* Most ofr RTS is STA send to DS. */
struct rts_cts_st {
	uint8_t ra[6];    // the receive address
	uint8_t ta[6];    // the transmite address
	int8_t  rssi;
	uint8_t security;
	uint16_t seq;
	uint32_t last_len;
	uint32_t last_time;
};

struct rts_cts_pkt_info {
	uint8_t  ra[6];    // the receive address
	uint8_t  ta[6];    // the transmite address
	int8_t   rssi;
	uint32_t time_ms;
};

static struct rts_cts_st rts_tbl[MAX_RTS_TBL];
static struct rts_cts_pkt_info last_rts;
static int rts_tbl_num = 0;

static wifi_monitor_cb_t s_monitor_cb = NULL;
static wifi_monitor_config_t s_monitor_config = {
	.rx_mcast_frame = 0,
	.rx_dup_frame = 0,
	.reserved = 0,
};
static bool s_monitor_enabled = false;

static wifi_filter_cb_t s_filter_cb = NULL;
static wifi_filter_config_t s_filter_config =
{
	.rx_all_default_mgmt = 1,
	.rx_probe_req = 0,
	.rx_probe_rsp = 0,
	.rx_all_beacon = 0,
	.rx_action = 0,
	.reserved = 0,
};

int wifi_filter_set_config(const wifi_filter_config_t *filter_config)
{
	uint32_t filter = 0;

	//The caller do NULL checking
	memcpy(&s_filter_config, filter_config, sizeof(s_filter_config));


	if (filter_config->rx_all_beacon) {
		filter |= NXMAC_ACCEPT_ALL_BEACON_BIT;
	}

	if (filter_config->rx_probe_req) {
		filter |= NXMAC_ACCEPT_PROBE_REQ_BIT;
	}

	if (filter_config->rx_probe_rsp) {
		filter |= NXMAC_ACCEPT_PROBE_RESP_BIT;
	}

	/**
	 * Cannot call mm_rx_filter_app_set(filter) directly, because
	 * machw may not in IDLE state, use ke_msg instead.
	 */
#if NX_VERSION >= NX_VERSION_PACK(6, 22, 0, 0)
	rw_msg_set_filter(filter);
#else
	rw_msg_set_app_filter(filter);
#endif

	return BK_OK;
}

int wifi_filter_get_config(wifi_filter_config_t *filter_config)
{
	//The caller do NULL checking
	memcpy(filter_config, &s_filter_config, sizeof(s_filter_config));
	return BK_OK;
}

int wifi_filter_register_cb(const wifi_filter_cb_t filter_cb)
{
	s_filter_cb = filter_cb;
	return BK_OK;
}

bool wifi_monitor_mcast_enabled(void)
{
	return !!(s_monitor_config.rx_mcast_frame);
}

bool wifi_monitor_dup_enabled(void)
{
	return !!(s_monitor_config.rx_dup_frame);
}

int wifi_monitor_register_cb(const wifi_monitor_cb_t monitor_cb)
{
	s_monitor_cb = monitor_cb;
	return BK_OK;
}

wifi_monitor_cb_t wifi_monitor_get_cb(void)
{
	return s_monitor_cb;
}

int wifi_monitor_set_config(const wifi_monitor_config_t *monitor_config)
{
	//The caller do NULL checking
	memcpy(&s_monitor_config, monitor_config, sizeof(s_monitor_config));
	return BK_OK;
}

int wifi_monitor_get_config(wifi_monitor_config_t *monitor_config)
{
	//The caller do NULL checking
	memcpy(monitor_config, &s_monitor_config, sizeof(s_monitor_config));
	return BK_OK;
}

int wifi_monitor_set_enabled(bool enabled)
{
	s_monitor_enabled = enabled;
	return BK_OK;
}

bool wifi_monitor_is_enabled(void)
{
	return s_monitor_enabled;
}

#if (SUPPORT_LSIG_MONITOR)
static int get_cipher_info(uint8_t *frame, int frame_len,
						   uint8_t *pairwise_cipher_type)
{
	uint8_t cap = frame[24 + 10]; // 24 is mac header; 8 timestamp, 2 beacon interval;
	u8 is_privacy = !!(cap & 0x10); // bit 4 = privacy
	const u8 *ie = frame + 36; // 24 + 12
	u16 ielen = frame_len - 36;
	const u8 *tmp;
	int ret = 0;

	tmp = (uint8_t *)mac_ie_find((uint32_t)ie, ielen, WLAN_EID_RSN);
	if (tmp && tmp[1])
		*pairwise_cipher_type = WLAN_ENC_CCMP;
	else {
		tmp = mac_vendor_ie_find(OUI_MICROSOFT, OUI_TYPE_MICROSOFT_WPA, ie, ielen);
		if (tmp)
			*pairwise_cipher_type = WLAN_ENC_TKIP;
		else {
			if (is_privacy)
				*pairwise_cipher_type = WLAN_ENC_WEP;
			else
				*pairwise_cipher_type = WLAN_ENC_OPEN;
		}
	}

	return ret;
}

static void lsig_monitor(uint8_t *data, int len, wifi_frame_info_t *info)
{
	uint8_t enc_type;

	/* check the RTS packet */
	if ((data[0] == 0xB4) && (len == 16)) { // RTS
		rts_update(data, info->rssi, rtos_get_time());
		return;
	}
	/* check beacon/probe reponse packet */
	if ((data[0] == 0x80) || (data[0] == 0x50)) {
		get_cipher_info(data, len, &enc_type);
		beacon_update(&data[16], enc_type);
	}

	if (s_monitor_cb)
		(*s_monitor_cb)(data, len, info);
}
#endif

void lsig_init(void)
{
	rts_tbl_num = 0;
	os_memset(&last_rts, 0, sizeof(last_rts));
}

void rts_update(uint8_t *data, int8_t rssi, uint32_t time_ms)
{
	os_memcpy(last_rts.ra, &data[4], 6);
	last_rts.rssi = -rssi;
	last_rts.time_ms = time_ms;
}

void beacon_update(uint8_t *bssid, uint8_t security)
{
	int i;

	for (i = 0; i < rts_tbl_num; i++) {
		if (rts_tbl[i].security != 0xFF)
			continue;
		if (os_memcmp(rts_tbl[i].ra, bssid, 6) == 0) {
			rts_tbl[i].security = security;
			rts_tbl[i].seq = 1;
		}
	}
}

static void report_lsig(int len, int8_t rssi, int index)
{
	uint8_t data[64]; // mac header & qos & vector
	uint16_t seq_frag;
	int i = 0;
	uint32_t time = rtos_get_time();

	if (rts_tbl[index].security == 0xFF)
		return;

	if ((len == rts_tbl[index].last_len) && (time < rts_tbl[index].last_time + 2)) {
		rts_tbl[index].last_time = time;
		return;
	}

	if (len < 24)
		return;


	data[i++] = 0x88; // data
	if (rts_tbl[index].security == WLAN_ENC_OPEN)
		data[i++] = 0x1;
	else {
		data[i++] = 0x41; // protect data;
	}

	data[i++] = 0x30; // duration
	data[i++] = 0;

	os_memcpy(&data[i], rts_tbl[index].ra, 6);
	i += 6;
	os_memcpy(&data[i], rts_tbl[index].ta, 6);
	i += 6;
	os_memset(&data[i], 0xFF, 6);
	i += 6;

	seq_frag = rts_tbl[index].seq;
	seq_frag <<= 4;
	os_memcpy(&data[i], &seq_frag, 2);
	i += 2;

	data[i++] = 0;// QOS
	data[i++] = 0;

	os_memset(&data[i], 0, 4);
	if (rts_tbl[index].security == WLAN_ENC_CCMP)
		data[i + 3] = 0x20;
	else if (rts_tbl[index].security == WLAN_ENC_TKIP) {
		data[i + 3] = 0x20;
		data[i] = 1;
		data[i + 1] = ((data[i] | 0x20) & 0x7F);
	}

	rts_tbl[index].seq++;
	rts_tbl[index].last_len = len;
	rts_tbl[index].last_time = time;

#if (SUPPORT_LSIG_MONITOR)
	wifi_frame_info_t info = {0};
	info.rssi = -rssi;
	lsig_monitor(data, len, &info);
#endif
}

void lsig_input(int len, int8_t rssi, uint32_t time_ms)
{
	int rts_match = 0, reported = 0;
	int i, delta;
	uint32_t min_time = 0, rep_index = 0;

	len -= 8; // remove FCS

	if (last_rts.time_ms + 1 > time_ms)  //  RTS match
		rts_match = 1;
	for (i = 0; i < rts_tbl_num; i++) {
		if (rts_match == 1) {
			if (os_memcmp(last_rts.ra, rts_tbl[i].ra, 6) == 0) {
				report_lsig(len, rssi, i);
				reported++;
				break;
			}
		} else {
			delta = rts_tbl[i].rssi - rssi;
			if ((delta > -5) && (delta < 5)) { // DO report for each RSSI match.
				report_lsig(len, rssi, i);
				reported++;
			}
		}
	}
	if ((rts_match != 1) || (reported != 0))
		return;

	// new entry
	if (rts_tbl_num < MAX_RTS_TBL) {
		os_memcpy(rts_tbl[rts_tbl_num].ra, last_rts.ra, 6);
		rts_tbl[rts_tbl_num].security = 0xFF;
		rts_tbl[rts_tbl_num].rssi = rssi;
		rts_tbl[rts_tbl_num].last_time = rtos_get_time();
		rts_tbl[rts_tbl_num].last_len = len;
		rts_tbl_num++;
		return;
	}

	// replace old entry.
	rep_index = 0;
	min_time = rts_tbl[0].last_time;
	for (i = 1; i < MAX_RTS_TBL; i++) {
		if (min_time > rts_tbl[i].last_time) {
			min_time = rts_tbl[i].last_time;
			rep_index = i;
		}
	}

	os_memcpy(rts_tbl[rep_index].ra, last_rts.ra, 6);
	rts_tbl[rep_index].security = 0xFF;
	rts_tbl[rep_index].rssi = rssi;
	rts_tbl[rep_index].last_len = len;
	rts_tbl[rep_index].last_time = rtos_get_time();
}

int rx_mgmt_filter(uint16_t framectrl, void *payload, uint16_t length, int8_t rssi)
{
	bool discard = true;

	// if not mgmt frame or user filter cb is not set
	if (!ieee80211_is_mgmt(framectrl) || !s_filter_cb)
		return 0;

	// Accept all mgmt frames
	if (s_filter_config.rx_all_default_mgmt) {
		discard = false;
	} else {
		// Filter out non-interest frames
		if (ieee80211_is_probe_req(framectrl)) {
			if (s_filter_config.rx_probe_req)
				discard = false;
		} else if (ieee80211_is_probe_resp(framectrl)) {
			if (s_filter_config.rx_probe_rsp)
				discard = false;
		} else if (ieee80211_is_beacon(framectrl)) {
			if (s_filter_config.rx_all_beacon)
				discard = false;
		} else if (ieee80211_is_action(framectrl)) {
			if (s_filter_config.rx_action)
				discard = false;
		}
	}

	// If not discard this frame, call user function
	if (!discard) {
		wifi_frame_info_t frame_info = {0};
		frame_info.rssi = rssi;
		s_filter_cb(payload, length, &frame_info);
	}

	return 0;
}


