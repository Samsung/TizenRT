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

#include <common/bk_include.h>
#include <os/mem.h>
#include <os/str.h>
#include "co_bit.h"
#include "bk_rw.h"
#if CONFIG_RWNX_SW_TXQ
#include "wifi_spinlock.h"
#include "skbuff.h"
#include "rwnx_tx.h"
#endif
#include "../wpa_supplicant-2.10/src/common/ieee802_11_defs.h"
#include "bk_wifi.h"
#include "../wpa_supplicant-2.10/src/drivers/driver.h"
#include "signal.h"
#include "../wpa_supplicant-2.10/wpa_supplicant/ctrl_iface.h"
#include "rw_msg_rx.h"
#include "rwnx_misc.h"

#include "../wpa_supplicant-2.10/src/common/wpa_ctrl.h"
#include "../wpa_supplicant-2.10/hostapd/main_none.h"
#include "rwnx_defs.h"
#include "bk_hostapd_intf.h"
#include "bk_wifi_private.h"
#include <components/event.h>
#include <modules/wifi.h>
#include <components/netif.h>
#if CONFIG_TX_BUFING
#include "rw_tx_buffering.h"
#endif
#include "../wpa_supplicant-2.10/src/common/ieee802_11_common.h"
#include "bk_wifi_types.h"
#include "bk_wifi_prop_private.h"
//#include "net.h"
#include "fhost_msg.h"
#include "../include/generated/lmac_msg.h"
#include "rw_ieee80211.h"

uint32_t resultful_scan_cfm = 0;
uint8_t *ind_buf_ptr = 0;
struct co_list rw_msg_rx_head;
struct co_list rw_msg_tx_head;
wifi_linkstate_reason_t connect_flag = {WIFI_LINKSTATE_STA_IDLE, WIFI_REASON_MAX};
SCAN_RST_UPLOAD_T *scan_rst_set_ptr = 0;

IND_CALLBACK_T scan_cfm_cb_user = {0};
IND_CALLBACK_T wlan_connect_user_cb = {0};
rw_event_handler rw_event_handlers[WIFI_LINKSTATE_MAX] = {0};

beken_mutex_t sr_mutex;
uint32_t mac_ie_he_capa_find(uint32_t buffer, uint16_t buflen, uint8_t *ie_len);


/**
 ****************************************************************************************
 * Test whether the specified capability is supported locally
 * @param[in] type Capability type (HT, VHT or HE)
 * @return true if supported, false otherwise
 ****************************************************************************************
 */
#define LOCAL_CAPA(type) ((me_env_get_capa_flags() & ME_##type##_CAPA) != 0)

#if CONFIG_AP_PROBEREQ_CB
apvif_probereq_frame_t apvif_probereq_frame_cb = NULL;

void ap_vif_probe_req_frame_cb_register(apvif_probereq_frame_t cb)
{
	apvif_probereq_frame_cb = cb;
}

void ap_vif_probe_req_frame_cb_hander(unsigned char *data, unsigned int length)
{
	apvif_probereq_frame_t probereq_cb = apvif_probereq_frame_cb;

	if (probereq_cb) {
		uint8_t *mgnt_buf;
		mgnt_buf = wifi_zalloc(length);
		if (mgnt_buf) {
			os_memcpy(mgnt_buf, (uint8_t *)data, length);
			probereq_cb(mgnt_buf, length);
			os_free(mgnt_buf);
		}
	}
}
#endif

extern FUNC_1PARAM_PTR bk_wlan_get_status_cb(void);
extern void app_set_sema(void);
extern int get_security_type_from_ie(u8 *, int, u16);
extern void rwnx_cal_set_txpwr(UINT32 pwr_gain, UINT32 grate);
extern void bk7011_default_rxsens_setting(void);

/*
 * Reduce scan result IEs stored in heap to avoid peak memory explosion.
 *
 * When CONFIG_MINIMIZE_SCAN_RESULT_IE is enabled, only necessary IEs are stored
 * in each result item to reduce peak heap usage during scan.
 *
 * Current selection: SSID + RSN + WPA vendor IE.
 */
#ifdef CONFIG_MINIMIZE_SCAN_RESULT_IE
static size_t sr_calc_reduced_ie_len(const u8 *ies, size_t ies_len)
{
	size_t total = 0;
	const u8 *ie;

	ie = get_ie(ies, ies_len, WLAN_EID_SSID);
	if (ie)
		total += (size_t)ie[1] + 2;

	ie = get_ie(ies, ies_len, WLAN_EID_RSN);
	if (ie)
		total += (size_t)ie[1] + 2;

	ie = get_vendor_ie(ies, ies_len, WPA_IE_VENDOR_TYPE);
	if (ie)
		total += (size_t)ie[1] + 2;

	return total;
}

static size_t sr_copy_reduced_ies(u8 *dst, size_t dst_len, const u8 *ies, size_t ies_len)
{
	size_t used = 0;
	const u8 *ie;
	size_t ilen;

	ie = get_ie(ies, ies_len, WLAN_EID_SSID);
	if (ie) {
		ilen = (size_t)ie[1] + 2;
		if (used + ilen <= dst_len) {
			os_memcpy(dst + used, ie, ilen);
			used += ilen;
		}
	}

	ie = get_ie(ies, ies_len, WLAN_EID_RSN);
	if (ie) {
		ilen = (size_t)ie[1] + 2;
		if (used + ilen <= dst_len) {
			os_memcpy(dst + used, ie, ilen);
			used += ilen;
		}
	}

	ie = get_vendor_ie(ies, ies_len, WPA_IE_VENDOR_TYPE);
	if (ie) {
		ilen = (size_t)ie[1] + 2;
		if (used + ilen <= dst_len) {
			os_memcpy(dst + used, ie, ilen);
			used += ilen;
		}
	}

	return used;
}
#endif /* CONFIG_MINIMIZE_SCAN_RESULT_IE */

/* scan result malloc item */
UINT8 *sr_malloc_result_item(UINT32 vies_len)
{
	#if CONFIG_MINIMUM_SCAN_RESULTS
	return os_zalloc(vies_len + sizeof(struct wpa_scan_res));
	#else
	return os_zalloc(vies_len + sizeof(struct sta_scan_res));
	#endif
}

/* free scan result item */
void sr_free_result_item(UINT8 *item_ptr)
{
	os_free(item_ptr);
}

UINT8 *sr_malloc_shell(void)
{
	UINT8 *ptr;
	UINT32 layer1_space_len;
	UINT32 layer2_space_len;

	layer1_space_len = sizeof(SCAN_RST_UPLOAD_T);
	#if CONFIG_MINIMUM_SCAN_RESULTS
	layer2_space_len = MAX_BSS_LIST * sizeof(struct wpa_scan_res *);
	#else
	layer2_space_len = MAX_BSS_LIST * sizeof(struct sta_scan_res *);
	#endif
	ptr = os_zalloc(layer1_space_len + layer2_space_len);

	if (ptr)
		return ptr;
	else {
		RWNX_LOGI("sr_malloc fail \r\n");
		return 0;
	}
}

void sr_free_shell(UINT8 *shell_ptr)
{
	os_free(shell_ptr);
}

void sr_free_all(SCAN_RST_UPLOAD_T *scan_rst)
{
	UINT32 i;

	for (i = 0; i < scan_rst->scanu_num; i ++) {
		sr_free_result_item((UINT8 *)scan_rst->res[i]);
		scan_rst->res[i] = 0;
	}
	scan_rst->scanu_num = 0;
	scan_rst->ref = 0;

	sr_free_shell((UINT8 *)scan_rst);
}

uint32_t sr_get_scan_number(void)
{
	uint32_t count = 0;
	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();
	if (scan_rst_set_ptr)
		count = scan_rst_set_ptr->scanu_num;
	GLOBAL_INT_RESTORE();

	return count;
}

/* Attention: sr_get_scan_results and sr_release_scan_results have to come in pairs*/
void *sr_get_scan_results(void)
{
	void *ptr;
	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();
	ptr = scan_rst_set_ptr;
	if (ptr != NULL)
		scan_rst_set_ptr->ref += 1;
	GLOBAL_INT_RESTORE();

	return ptr;
}

void sr_flush_scan_results(SCAN_RST_UPLOAD_PTR ptr)
{
	//GLOBAL_INT_DECLARATION();

	//GLOBAL_INT_DISABLE();
	ptr->ref = 1;
	sr_release_scan_results(ptr);
	//GLOBAL_INT_RESTORE();
}

/* Attention: sr_get_scan_results and sr_release_scan_results have to come in pairs*/
void sr_release_scan_results(SCAN_RST_UPLOAD_PTR ptr)
{
	rtos_lock_mutex(&sr_mutex);
	if ((0 == ptr) || (0 == ptr->ref)) {
		RWNX_LOGI("released_scan_results\r\n");
		goto release_exit;
	}

	ptr->ref -= 1;

	if (ptr->ref) {
		RWNX_LOGI("release_scan_results later\r\n");
		goto release_exit;
	}

	if (ptr)
		sr_free_all(ptr);
	scan_rst_set_ptr = 0;
	resultful_scan_cfm = 0;

	wpa_clear_scan_results();

release_exit:
	rtos_unlock_mutex(&sr_mutex);
	return;

}

void mr_kmsg_init(void)
{
	co_list_init(&rw_msg_tx_head);
	co_list_init(&rw_msg_rx_head);
}

UINT32 mr_kmsg_fwd(struct ke_msg *msg)
{
	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();
	co_list_push_back(&rw_msg_rx_head, &msg->hdr);
	GLOBAL_INT_RESTORE();

	app_set_sema();

	return 0;
}

void mr_kmsg_flush(void)
{
	while (mr_kmsg_fuzzy_handle())
		;
}

UINT32 mr_kmsg_fuzzy_handle(void)
{
	UINT32 ret = 0;
	struct ke_msg *msg;
	struct co_list_hdr *node;

	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();
	node = co_list_pop_front(&rw_msg_rx_head);
	GLOBAL_INT_RESTORE();

	if (node) {
		msg = (struct ke_msg *)node;
		ke_msg_free(msg);

		ret = 1;
	}

	return ret;
}

UINT32 mr_kmsg_exact_handle(UINT16 rsp)
{
	UINT32 ret = 0;
	struct ke_msg *msg;
	struct co_list_hdr *node;

	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();
	node = co_list_pop_front(&rw_msg_rx_head);
	GLOBAL_INT_RESTORE();

	if (node) {
		msg = (struct ke_msg *)node;
		if (rsp == msg->id)
			ret = 1;
		ke_msg_free(msg);
	}

	return ret;
}

void mhdr_connect_user_cb(FUNC_2PARAM_PTR ind_cb, void *ctxt)
{
	wlan_connect_user_cb.cb = ind_cb;
	wlan_connect_user_cb.ctxt_arg = ctxt;
}

/* depreciated, compatible with wifi api v1 */
static int scan_ap_cb_for_new_api(void *arg, event_module_t event_module,
								  int event_id, void *event_data)
{
	if (scan_cfm_cb_user.cb)
		scan_cfm_cb_user.cb(scan_cfm_cb_user.ctxt_arg, 0/* dummy */);

	return BK_OK;
}

/* depreciated, compatible with wifi api v1 */
void mhdr_scanu_reg_cb(FUNC_2PARAM_PTR ind_cb, void *ctxt)
{
	scan_cfm_cb_user.cb = ind_cb;
	scan_cfm_cb_user.ctxt_arg = ctxt;

	bk_event_register_cb(EVENT_MOD_WIFI, EVENT_WIFI_SCAN_DONE,
						 scan_ap_cb_for_new_api, NULL);
}

#if CONFIG_WIFI_SCAN_COUNTRY_CODE
// Country code beacon received
static wifi_beacon_cc_rxed_t g_scan_cc_rxed_cb = NULL;
void *g_scan_cc_ctxt = NULL;
static beken_thread_t g_scan_cc_thread = NULL;
static beken_semaphore_t cc_scan_handle = NULL;
bool site_survey_cc = false;

bk_err_t bk_wifi_bcn_cc_rxed_register_cb(const wifi_beacon_cc_rxed_t cc_cb, void *ctxt)
{
	g_scan_cc_rxed_cb = cc_cb;
	g_scan_cc_ctxt = ctxt;

	return 0;
}

static int cc_scan_ap_cb(void *arg, event_module_t event_module,
						 int event_id, void *_event_data)
{
	uint32_t thread_id = (uint32_t)arg;
	wifi_event_scan_done_t *event_data = _event_data;

	// If EVENT_WIFI_SCAN_DONE event's scan_id equals our thread_id
	if (thread_id == event_data->scan_id) {
		if (cc_scan_handle)
			rtos_set_semaphore(&cc_scan_handle);
	}

	return BK_OK;
}

static void cc_scan_thread(beken_thread_arg_t arg)
{
	bk_err_t err = kNoErr;

	err = rtos_init_semaphore(&cc_scan_handle, 1);
	if (err == kNoErr) {
		int loop = 0;
		uint8 cfm;
		bk_event_register_cb(EVENT_MOD_WIFI, EVENT_WIFI_SCAN_DONE,
							 cc_scan_ap_cb, rtos_get_current_thread());
		wifi_scan_config_t scan_param = {0};
		scan_param.ssid[0] = 0;
		scan_param.flag = SCAN_TYPE_CC;
		err = bk_wifi_scan_start(&scan_param);

		if(err == kNoErr){
			while((loop < 100)&&(g_scan_cc_thread)) {
				err = rtos_get_semaphore(&cc_scan_handle, 50);
				if(err == kNoErr) {
					break;
				} else if(err == kTimeoutErr) {
					loop ++;
					continue;
				}
			}
			if(g_scan_cc_thread == NULL)
			{
				rw_msg_send_scan_cancel_req(&cfm);
			}
		}
		bk_event_unregister_cb(EVENT_MOD_WIFI, EVENT_WIFI_SCAN_DONE,
							 cc_scan_ap_cb);
		if (cc_scan_handle)
			rtos_deinit_semaphore(&cc_scan_handle);
		cc_scan_handle = NULL;
	}

	g_scan_cc_thread = NULL;
	rtos_delete_thread(NULL);
}

int cc_scan_start(void)
{
	bk_err_t err = kNoErr;

	if(g_scan_cc_thread == NULL)
	{
		err = rtos_create_thread(&g_scan_cc_thread, BEKEN_APPLICATION_PRIORITY,
								"ccscan",
								(beken_thread_function_t)cc_scan_thread,
								0x800,
								(beken_thread_arg_t)0);
	}
	else
	{
		err = kInProgressErr;
	}

	return err;
}

void cc_scan_stop(void)
{
	if(g_scan_cc_thread)
	{
		GLOBAL_INT_DECLARATION();

		GLOBAL_INT_DISABLE();
		g_scan_cc_thread = NULL;
		GLOBAL_INT_RESTORE();

		while(cc_scan_handle)
			rtos_delay_milliseconds(50);
	}
}
#endif // CONFIG_WIFI_SCAN_COUNTRY_CODE

void mhdr_connect_ind(void *msg, UINT32 len)
{
	struct ke_msg *msg_ptr;
	struct sm_connect_ind *ind;

	msg_ptr = (struct ke_msg *)msg;
	ind = (struct sm_connect_ind *)msg_ptr->param;

	if (!ind->status_code) {
		RWNX_LOGD("connect ok\n");

		bk7011_default_rxsens_setting();

#if NX_VERSION > NX_VERSION_PACK(6, 22, 0, 0)
		rwnx_misc_connect_ind(ind);
#endif

		if (wlan_connect_user_cb.cb)
			(*wlan_connect_user_cb.cb)(wlan_connect_user_cb.ctxt_arg, 0);
	} else
		RWNX_LOGD("connect fail\n");

	/* Send to wpa_supplicant */
	/* ind->assoc_ie_buf is a flexible array */
	wpa_ctrl_event_copy(WPA_CTRL_EVENT_CONNECT_IND, ind, macif_max_api_msg_len());
}

/* RXU_MGT_IND handler, send it to wpa_s */
void mhdr_mgmt_ind(void *msg, UINT32 len)
{
	struct ke_msg *msg_ptr = (struct ke_msg *)msg;
	struct rxu_mgt_ind *ind = (struct rxu_mgt_ind *)msg_ptr->param;

	wpa_ctrl_event_copy(WPA_CTRL_EVENT_MGMT_IND, ind, sizeof(*ind) + ind->length);
}

void mhdr_set_station_status(wifi_linkstate_reason_t info)
{
	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();
	connect_flag.state = info.state;
	connect_flag.reason_code = info.reason_code;
	GLOBAL_INT_RESTORE();

#if (CONFIG_ALIOS)
	if (info.state == WIFI_LINKSTATE_STA_DISCONNECTED)
		connection_fail_cb(info);
#endif
}

wifi_linkstate_reason_t mhdr_get_station_status(void)
{
	return connect_flag;
}

bool mhdr_station_status_not_connected(void)
{
    wifi_linkstate_reason_t info = mhdr_get_station_status();

    return info.state < WIFI_LINKSTATE_STA_CONNECTED;
}

static void sort_scan_result(SCAN_RST_UPLOAD_T *ap_list)
{
	int i, j;
	#if CONFIG_MINIMUM_SCAN_RESULTS
	struct wpa_scan_res *tmp;
	#else
	struct sta_scan_res *tmp;
	#endif
	if (!ap_list)
		return;

	if (ap_list->scanu_num == 0)
		return;

	for (i = 0; i < (ap_list->scanu_num - 1); i++) {
		for (j = i + 1; j < ap_list->scanu_num; j++) {
			if (ap_list->res[j]->level > ap_list->res[i]->level) {
				tmp = ap_list->res[j];
				ap_list->res[j] = ap_list->res[i];
				ap_list->res[i] = tmp;
			}
		}
	}
}

#if CONFIG_RWNX_MULTIPLE_SSID_STA_MODE
/* find the match ie element */
const struct element *
rwnx_find_elem_match(u8 eid, const u8 *ies, unsigned int len,
			 const u8 *match, unsigned int match_len,
			 unsigned int match_offset)
{
	const struct element *elem;

	for_each_element_id(elem, eid, ies, len) {
		if (elem->datalen >= match_offset + match_len &&
		    !os_memcmp(elem->data + match_offset, match, match_len))
			return elem;
	}

	return NULL;
}

static inline const struct element *
rwnx_find_elem(u8 eid, const u8 *ies, int len)
{
	return rwnx_find_elem_match(eid, ies, len, NULL, 0, 0);
}

static const struct element
*rwnx_get_profile_continuation(const u8 *ie, size_t ielen,
										const struct element *mbssid_elem,
										const struct element *sub_elem)
{
	const u8 *mbssid_end = mbssid_elem->data + mbssid_elem->datalen;
	const struct element *next_mbssid;
	const struct element *next_sub;

	next_mbssid = rwnx_find_elem(WLAN_EID_MULTIPLE_BSSID, mbssid_end,
									ielen - (mbssid_end - ie));

	/*
	 * If it is not the last subelement in current MBSSID IE or there isn't
	 * a next MBSSID IE - profile is complete.
	*/
	if ((sub_elem->data + sub_elem->datalen < mbssid_end - 1) ||
	    !next_mbssid)
		return NULL;

	/* For any length error, just return NULL */

	if (next_mbssid->datalen < 4)
		return NULL;

	next_sub = (void *)&next_mbssid->data[1];

	if (next_mbssid->data + next_mbssid->datalen <
	    next_sub->data + next_sub->datalen)
		return NULL;

	if (next_sub->id != 0 || next_sub->datalen < 2)
		return NULL;

	/*
	 * Check if the first element in the next sub element is a start
	 * of a new profile
	 */
	return next_sub->data[0] == WLAN_EID_NONTRANSMITTED_BSSID_CAPA ?
	       NULL : next_mbssid;
}

/**
 * ether_addr_to_u64 - Convert an Ethernet address into a u64 value.
 * @addr: Pointer to a six-byte array containing the Ethernet address
 *
 * Return a u64 value of the address
 */
static inline u64 ether_addr_to_u64(const u8 *addr)
{
	u64 u = 0;
	int i;

	for (i = 0; i < ETH_ALEN; i++)
		u = u << 8 | addr[i];

	return u;
}

/**
 * u64_to_ether_addr - Convert a u64 to an Ethernet address.
 * @u: u64 to convert to an Ethernet MAC address
 * @addr: Pointer to a six-byte array to contain the Ethernet address
 */
static inline void u64_to_ether_addr(u64 u, u8 *addr)
{
	int i;

	for (i = ETH_ALEN - 1; i >= 0; i--) {
		addr[i] = u & 0xff;
		u = u >> 8;
	}
}

/**
 * cfg80211_gen_new_bssid - generate a nontransmitted BSSID for multi-BSSID
 * @bssid: transmitter BSSID
 * @max_bssid: max BSSID indicator, taken from Multiple BSSID element
 * @mbssid_index: BSSID index, taken from Multiple BSSID index element
 * @new_bssid: calculated nontransmitted BSSID
 */
static inline void rwnx_gen_new_bssid(const u8 *bssid, u8 max_bssid,
					                       u8 mbssid_index, u8 *new_bssid)
{
	u64 bssid_u64 = ether_addr_to_u64(bssid);
	u64 mask = GENMASK_ULL(max_bssid - 1, 0);
	u64 new_bssid_u64;

	new_bssid_u64 = bssid_u64 & ~mask;
	new_bssid_u64 |= ((bssid_u64 & mask) + mbssid_index) & mask;
	u64_to_ether_addr(new_bssid_u64, new_bssid);
}

/* element is non_inherit_elem or not */
bool rwnx_is_element_inherited(const struct element *elem,
				               const struct element *non_inherit_elem)
{
	u8 id_len, ext_id_len, i, loop_len, id;
	const u8 *list;

	if (elem->id == WLAN_EID_MULTIPLE_BSSID)
		return false;

	if (!non_inherit_elem || non_inherit_elem->datalen < 2)
		return true;

	/*
	 * non inheritance element format is:
	 * ext ID (56) | IDs list len | list | extension IDs list len | list
	 * Both lists are optional. Both lengths are mandatory.
	 * This means valid length is:
	 * elem_len = 1 (extension ID) + 2 (list len fields) + list lengths
	 */
	id_len = non_inherit_elem->data[1];
	if (non_inherit_elem->datalen < 3 + id_len)
		return true;

	ext_id_len = non_inherit_elem->data[2 + id_len];
	if (non_inherit_elem->datalen < 3 + id_len + ext_id_len)
		return true;

	if (elem->id == WLAN_EID_EXTENSION) {
		if (!ext_id_len)
			return true;
		loop_len = ext_id_len;
		list = &non_inherit_elem->data[3 + id_len];
		id = elem->data[0];
	} else {
		if (!id_len)
			return true;
		loop_len = id_len;
		list = &non_inherit_elem->data[2];
		id = elem->id;
	}

	for (i = 0; i < loop_len; i++) {
		if (list[i] == id)
			return false;
	}

	return true;
}

/* find the match ie */
static inline const u8 *
rwnx_find_ie_match(u8 eid, const u8 *ies, unsigned int len,
		           const u8 *match, unsigned int match_len,
		           unsigned int match_offset)
{
	return (void *)rwnx_find_elem_match(eid, ies, len,
						                match, match_len,
						                match_offset ?
					                    match_offset - 2 : 0);
}

/* find the match ie */
static inline const u8 *rwnx_find_ie(u8 eid, const u8 *ies, int len)
{
	return rwnx_find_ie_match(eid, ies, len, NULL, 0, 0);
}


/* find the match ext ie */
static inline const u8 *rwnx_find_ext_ie(u8 ext_eid, const u8 *ies, int len)
{
	return rwnx_find_ie_match(WLAN_EID_EXTENSION, ies, len, &ext_eid, 1, 2);
}

/* find the match ext element */
static inline const struct element *
rwnx_find_ext_elem(u8 ext_eid, const u8 *ies, int len)
{
	return rwnx_find_elem_match(WLAN_EID_EXTENSION, ies, len,
					            &ext_eid, 1, 0);
}

/* generate new ie */
static size_t rwnx_gen_new_ie(const u8 *ie, size_t ielen,
                              const u8 *subelement, size_t subie_len, u8 *new_ie)
{
	u8 *pos, *tmp;
	const u8 *tmp_old, *tmp_new;
	const struct element *non_inherit_elem;
	u8 *sub_copy;

	/* copy subelement as we need to change its content to
	 * mark an ie after it is processed.
	 */
    sub_copy = os_zalloc(subie_len);
	if (!sub_copy)
		return 0;

    os_memcpy(sub_copy, subelement, subie_len);
	pos = &new_ie[0];

	/* set new ssid save the ssid in profile */
    tmp_new = rwnx_find_ie(WLAN_EID_SSID, sub_copy, subie_len);
	if (tmp_new) {
		os_memcpy(pos, tmp_new, tmp_new[1] + 2);
		pos += (tmp_new[1] + 2);
	}

	/* get non inheritance list if exists */
	non_inherit_elem = rwnx_find_ext_elem(WLAN_EID_EXT_NON_INHERITANCE,
				                          sub_copy, subie_len);

	/* go through IEs in ie (skip SSID) and subelement,
	 * merge them into new_ie
	 */
	tmp_old = rwnx_find_ie(WLAN_EID_SSID, ie, ielen);
	tmp_old = (tmp_old) ? tmp_old + tmp_old[1] + 2 : ie;

	while (tmp_old + tmp_old[1] + 2 - ie <= ielen) {
		if (tmp_old[0] == 0) {
			tmp_old++;
			continue;
		}

		if (tmp_old[0] == WLAN_EID_EXTENSION)
			tmp = (u8 *)rwnx_find_ext_ie(tmp_old[2], sub_copy, subie_len);
		else
            tmp = (u8 *)rwnx_find_ie(tmp_old[0], sub_copy,
                                         subie_len);

		if (!tmp) {
			const struct element *old_elem = (void *)tmp_old;

			/* ie in old ie but not in subelement */
			if (rwnx_is_element_inherited(old_elem, non_inherit_elem)) {
				os_memcpy(pos, tmp_old, tmp_old[1] + 2);
				pos += tmp_old[1] + 2;
			}
		} else {
			/* ie in transmitting ie also in subelement,
			 * copy from subelement and flag the ie in subelement
			 * as copied (by setting eid field to WLAN_EID_SSID,
			 * which is skipped anyway).
			 * For vendor ie, compare OUI + type + subType to
			 * determine if they are the same ie.
			 */
			if (tmp_old[0] == WLAN_EID_VENDOR_SPECIFIC) {
				if (!os_memcmp(tmp_old + 2, tmp + 2, 5)) {
					/* same vendor ie, copy from
					 * subelement
					 */
					os_memcpy(pos, tmp, tmp[1] + 2);
					pos += tmp[1] + 2;
					tmp[0] = WLAN_EID_SSID;
				} else {
					os_memcpy(pos, tmp_old, tmp_old[1] + 2);
					pos += tmp_old[1] + 2;
				}
			} else {
				/* copy ie from subelement into new ie */
				os_memcpy(pos, tmp, tmp[1] + 2);
				pos += tmp[1] + 2;
				tmp[0] = WLAN_EID_SSID;
			}
		}

		if (tmp_old + tmp_old[1] + 2 - ie == ielen)
			break;

		tmp_old += tmp_old[1] + 2;
	}

	/* go through subelement again to check if there is any ie not
	 * copied to new ie, skip ssid, capability, bssid-index ie
	 */
	tmp_new = sub_copy;
	while (tmp_new + tmp_new[1] + 2 - sub_copy <= subie_len) {
		if (!(tmp_new[0] == WLAN_EID_NONTRANSMITTED_BSSID_CAPA ||
		      tmp_new[0] == WLAN_EID_SSID)) {
			os_memcpy(pos, tmp_new, tmp_new[1] + 2);
			pos += tmp_new[1] + 2;
		}
		if (tmp_new + tmp_new[1] + 2 - sub_copy == subie_len)
			break;
		tmp_new += tmp_new[1] + 2;
	}

	free(sub_copy);
	return pos - new_ie;
}

/* merge subdata into profile(for example EID 0;EID 83;EID 85) */
size_t rwnx_merge_profile(const u8 *ie, size_t ielen,
			              const struct element *mbssid_elem,
			              const struct element *sub_elem,
			              u8 *merged_ie, size_t max_copy_len)
{
	size_t copied_len = sub_elem->datalen;
	const struct element *next_mbssid;

	if (sub_elem->datalen > max_copy_len)
		return 0;

	os_memcpy(merged_ie, sub_elem->data, sub_elem->datalen);

	while ((next_mbssid = rwnx_get_profile_continuation(ie, ielen, mbssid_elem, sub_elem))) {
		    const struct element *next_sub = (void *)&next_mbssid->data[1];

		    if (copied_len + next_sub->datalen > max_copy_len)
			    break;
		    os_memcpy(merged_ie + copied_len, next_sub->data,
		              next_sub->datalen);
		    copied_len += next_sub->datalen;
	}

	return copied_len;
}

static int rwnx_inform_single_bss_data(SCAN_RST_UPLOAD_PTR result_ptr, SCAN_RST_ITEM_PTR item)
{
	int replace_index = -1, ret = BK_OK, i;
	/* check the duplicate bssid*/
	do {
		for (i = 0; i < result_ptr->scanu_num; i ++) {
			if (!os_memcmp(item->bssid, result_ptr->res[i]->bssid, ETH_ALEN)) {
				if ((result_ptr->res[i]->on_channel == 1) || (item->on_channel == 0)) {
					ret = BK_FAIL;
					os_free(item);
					goto scan_rst_exit;
				}
				else {
					replace_index = i; // should replace it.
				}
			}
		}
	} while (0);

	if (replace_index >= 0) {
		sr_free_result_item((UINT8 *)result_ptr->res[replace_index]);
		result_ptr->res[replace_index] = item;
#if CONFIG_RWNX_MULTIPLE_SSID_STA_MODE
	} else if (result_ptr->scanu_num < MAX_BSS_LIST) {
			result_ptr->res[result_ptr->scanu_num] = item;
			result_ptr->scanu_num ++;
#else
	} else {
		result_ptr->res[result_ptr->scanu_num] = item;
		result_ptr->scanu_num ++;
#endif
	}
scan_rst_exit:
	return ret;
}

/* process each multiple ssid element data and generate new_ie */
static void rwnx_inform_multiple_bss_data(SCAN_RST_UPLOAD_PTR result_ptr, SCAN_RST_ITEM_PTR item,
											UINT8 * var_part_addr, UINT32 vies_len)
{
	UINT8 *profile;
	UINT8 *new_ie;
	size_t new_ie_len;
	const struct element *elem;
	const struct element *sub;
	const u8 *mbssid_index_ie;
	u8 new_bssid[ETH_ALEN];
	u16 capability;
	SCAN_RST_ITEM_PTR multiple_item;

	if (!rwnx_find_ie(WLAN_EID_MULTIPLE_BSSID, var_part_addr, vies_len))
		return;

	/* alloc buffer for generate new ie */
	new_ie = (UINT8*)os_zalloc(vies_len);
	if (!new_ie)
		return;

	profile = (UINT8*)os_zalloc(vies_len);
	if (!profile)
		goto out;

	for_each_element_id(elem, WLAN_EID_MULTIPLE_BSSID, var_part_addr, vies_len) {
		if (elem->datalen < 4)
			continue;
		for_each_element(sub, elem->data + 1, elem->datalen - 1) {

			u8 profile_len;

			if (sub->id != 0 || sub->datalen < 4) {
				/* not a valid BSS profile */
				continue;
			}
			/* first sub_data[0] EID = 83 */
			if (sub->data[0] != WLAN_EID_NONTRANSMITTED_BSSID_CAPA ||
				sub->data[1] != 2) {
				/* The first element within the Nontransmitted
				* BSSID Profile is not the Nontransmitted
				* BSSID Capability element.
				*/
				continue;
			}

			/* merge the subdata into profile(EID 83- EID 85) */
			os_memset(profile, 0, vies_len);
			profile_len = rwnx_merge_profile(var_part_addr, vies_len, elem,
                                                 sub, profile, vies_len);

			/* found a Nontransmitted BSSID Profile EID = 85 */
			mbssid_index_ie = rwnx_find_ie(WLAN_EID_MULTIPLE_BSSID_INDEX, profile, profile_len);

			/* mbssid_index_ie[2]= BSS index, related to the maxbssid indicator */
			if (!mbssid_index_ie || mbssid_index_ie[1] < 1 ||
				mbssid_index_ie[2] == 0 ||
				mbssid_index_ie[2] > 46) {
				/* No valid Multiple BSSID-Index element */
				continue;
			}

			//save the bssid_idx and max_bssid_indicator of each multiple ssid element
			u8 bssid_index = mbssid_index_ie[2];
			u8 max_bssid_indicator = elem->data[0];

			/* new multiple ssid ie to be upload */
			multiple_item = (SCAN_RST_ITEM_PTR)sr_malloc_result_item(vies_len);
			multiple_item->on_channel = item->on_channel;
			multiple_item->channel = item->channel;
			multiple_item->beacon_int = item->beacon_int;
			multiple_item->level = item->level;
			multiple_item->security = item->security;
#if CONFIG_MINIMUM_SCAN_RESULTS
			multiple_item->tsf = item->tsf;
#else
			os_memcpy(multiple_item->tsf, item->tsf, 8);
#endif

			//generate new bssid and update the bss_id in ie_info
			rwnx_gen_new_bssid(item->bssid, max_bssid_indicator, bssid_index, new_bssid);
			os_memcpy(multiple_item->bssid, new_bssid, ETH_ALEN);

			//clean the new_ie buffer
			os_memset(new_ie, 0, vies_len);

			/* set the new SSID,
			go through IEs in ie (skip old SSID) and subelement, merge them into new_ie */
			new_ie_len = rwnx_gen_new_ie(var_part_addr, vies_len,
			                                  profile, profile_len, new_ie);
			if (!new_ie_len)
				continue;
			//save the nontransmitted capability for bssid_idx
			capability = co_read16(profile + 2);

			//save the new_ie_info into struct ie_info
			multiple_item->caps = capability;

			const uint8_t *elmt_addr = rwnx_find_ie(WLAN_EID_SSID, new_ie, new_ie_len);

			if (elmt_addr)
				os_memcpy(multiple_item->ssid, elmt_addr + 2, *(elmt_addr + 1));

			multiple_item->ie_len = new_ie_len;

			//upload the payload to the upper
			os_memcpy(multiple_item + 1, new_ie, new_ie_len);

			/* upload the single infor to the upper */
			rwnx_inform_single_bss_data(result_ptr, multiple_item);
		}
	}

out:
	os_free(new_ie);
	os_free(profile);
}
#endif

UINT32 mhdr_scanu_result_ind(SCAN_RST_UPLOAD_T *scan_rst, void *msg, UINT32 len)
{
	UINT32 ret = BK_OK;
	UINT32 chann;
	UINT8 *elmt_addr;
	UINT32 vies_len;
	UINT8 *var_part_addr;
	struct ke_msg *msg_ptr;
	SCAN_RST_ITEM_PTR item;
	SCAN_RST_UPLOAD_PTR result_ptr;
	SCAN_IND_PTR scanu_ret_ptr;
	IEEE802_11_PROBE_RSP_PTR probe_rsp_ieee80211_ptr;
	char on_channel;
#if CONFIG_RWNX_MULTIPLE_SSID_STA_MODE
	int infor_state;
#else
	int replace_index = -1;
	UINT32 i;
#endif

	ret = BK_OK;
	result_ptr = scan_rst;

	msg_ptr = (struct ke_msg *)msg;
	scanu_ret_ptr = (SCAN_IND_PTR)msg_ptr->param;
	probe_rsp_ieee80211_ptr = (IEEE802_11_PROBE_RSP_PTR)scanu_ret_ptr->payload;
	vies_len = scanu_ret_ptr->length - MAC_BEACON_VARIABLE_PART_OFT;
	var_part_addr = probe_rsp_ieee80211_ptr->rsp.variable;

	#if CONFIG_WIFI_REGDOMAIN
	struct ieee80211_channel *chan = ieee80211_get_channel(&g_wiphy, scanu_ret_ptr->center_freq);

	if (chan)
		regulatory_hint_found_beacon(&g_wiphy, chan);
	#endif // CONFIG_WIFI_REGDOMAIN

	#if CONFIG_WIFI_SCAN_COUNTRY_CODE
	if (site_survey_cc && g_scan_cc_rxed_cb) {
		elmt_addr = (UINT8 *)get_ie(var_part_addr, vies_len, MAC_ELTID_COUNTRY);
		if (elmt_addr) {
			UINT8 cc_len = *(elmt_addr + MAC_COUNTRY_LEN_OFT);
			g_scan_cc_rxed_cb(g_scan_cc_ctxt, (uint8_t *)(elmt_addr + MAC_COUNTRY_STRING_OFT), cc_len);
		}
	}
	#endif // CONFIG_WIFI_SCAN_COUNTRY_CODE

	if (result_ptr->scanu_num >= MAX_BSS_LIST)
		goto scan_rst_exit;

	elmt_addr = (UINT8 *)get_ie(var_part_addr, vies_len, MAC_ELTID_DS);
	if (elmt_addr) { // adjust channel
		chann = *(elmt_addr + MAC_DS_CHANNEL_OFT);
		if (rw_ieee80211_is_scan_rst_in_countrycode(chann) == 0) {
			elmt_addr = (UINT8 *)get_ie(var_part_addr, vies_len, WLAN_EID_SSID);
			if (elmt_addr) {
				UINT8 ssid_b[MAC_SSID_LEN+1] = {0};
				UINT8 ssid_len = *(elmt_addr + 1);

				if (ssid_len > MAC_SSID_LEN)
					ssid_len = MAC_SSID_LEN;

				os_memcpy(ssid_b, elmt_addr + 2, ssid_len);
				RWNX_LOGI("drop: %s, chan:%d,not in countrycode\r\n", wpa_ssid_txt(ssid_b, ssid_len), chann);
			}

			goto scan_rst_exit;
		}

		if (chann == rw_ieee80211_get_chan_id(scanu_ret_ptr->center_freq))
			on_channel = 1;
		else
			on_channel = 0;
	} else {
		uint8_t elmt_length;
		elmt_addr = (UINT8 *)mac_ie_he_capa_find((UINT32)var_part_addr, vies_len, &elmt_length);
		if (elmt_addr && (LOCAL_CAPA(HE))) {
			// DS IE for 2.4G Only
			if (scanu_ret_ptr->center_freq < 4900)
				RWNX_LOGI("he+he,scan rst no ds param, not drop it?\r\n");
			chann = rw_ieee80211_get_chan_id(scanu_ret_ptr->center_freq);
			if (rw_ieee80211_is_scan_rst_in_countrycode(chann) == 0) {
				elmt_addr = (UINT8 *)get_ie(var_part_addr, vies_len, WLAN_EID_SSID);
				if (elmt_addr) {
					UINT8 ssid_b[MAC_SSID_LEN+1] = {0};
					UINT8 ssid_len = *(elmt_addr + 1);

					if (ssid_len > MAC_SSID_LEN)
						ssid_len = MAC_SSID_LEN;

					os_memcpy(ssid_b, elmt_addr + 2, ssid_len);
					RWNX_LOGI("drop: %s, chan:%d,not in countrycode\r\n", wpa_ssid_txt(ssid_b, ssid_len), chann);
				}
				goto scan_rst_exit;
			}
			on_channel = 1;
		} else {
			chann = rw_ieee80211_get_chan_id(scanu_ret_ptr->center_freq);
			on_channel = 0;
			if (scanu_ret_ptr->center_freq < 4900)
				RWNX_LOGI("scan rst no ds param, drop it?\r\n");
		}
	}

#if !CONFIG_RWNX_MULTIPLE_SSID_STA_MODE
	/* check the duplicate bssid*/
	do {
		for (i = 0; i < result_ptr->scanu_num; i++) {
			if (!os_memcmp(probe_rsp_ieee80211_ptr->bssid, result_ptr->res[i]->bssid, ETH_ALEN)) {
				if ((result_ptr->res[i]->on_channel == 1) || (on_channel == 0)) {
					if (on_channel == 1)
						result_ptr->res[i]->level = ((result_ptr->res[i]->level + scanu_ret_ptr->rssi) >> 1);
					goto scan_rst_exit;
				} else {
					replace_index = i; // should replace it.
				}
			}
		}
	} while (0);
#endif

	#ifdef CONFIG_MINIMIZE_SCAN_RESULT_IE
	/*
	 * Store reduced IEs (SSID/RSN/WPA vendor) in the result item to reduce
	 * peak heap usage during scan.
	 */
	UINT32 reduced_len = (UINT32)sr_calc_reduced_ie_len((const u8 *)var_part_addr, (size_t)vies_len);
	item = (SCAN_RST_ITEM_PTR)sr_malloc_result_item(reduced_len);
	#else
	item = (SCAN_RST_ITEM_PTR)sr_malloc_result_item(vies_len);
	#endif
	if (item == NULL)
		goto scan_rst_exit;

	elmt_addr = (UINT8 *)get_ie(var_part_addr, vies_len, WLAN_EID_SSID);
	if (elmt_addr) {
		UINT8 ssid_len = *(elmt_addr + 1);

		if (ssid_len > MAC_SSID_LEN)
			ssid_len = MAC_SSID_LEN;

		os_memcpy(item->ssid, elmt_addr + 2, ssid_len);
	} else {
		RWNX_LOGI("No ssid\r\n");
	}


	item->channel = chann;
	item->beacon_int = probe_rsp_ieee80211_ptr->rsp.beacon_int;
	item->caps = probe_rsp_ieee80211_ptr->rsp.capab_info;
	item->level = scanu_ret_ptr->rssi;
	item->on_channel = on_channel;
	#ifdef CONFIG_MINIMIZE_SCAN_RESULT_IE
	{
		size_t copied = sr_copy_reduced_ies((u8 *)(item + 1), (size_t)reduced_len,
						   (const u8 *)var_part_addr, (size_t)vies_len);
		item->ie_len = (UINT32)copied;
		item->security = get_security_type_from_ie((u8 *)(item + 1), (int)copied, item->caps);
	}
	#else
	item->ie_len = vies_len;
	item->security = get_security_type_from_ie((u8 *)var_part_addr, vies_len, item->caps);
	#endif
	os_memcpy(item->bssid, probe_rsp_ieee80211_ptr->bssid, ETH_ALEN);
#if CONFIG_MINIMUM_SCAN_RESULTS
	item->tsf = probe_rsp_ieee80211_ptr->rsp.timestamp;
#else
	os_memcpy(item->tsf, probe_rsp_ieee80211_ptr->rsp.timestamp, 8);
#endif
	#if !CONFIG_MINIMUM_SCAN_RESULTS
	os_memcpy(item + 1, var_part_addr, vies_len);
	#endif
#if CONFIG_RWNX_MULTIPLE_SSID_STA_MODE
	/* upload single transmit-bss infor to upper */
	infor_state = rwnx_inform_single_bss_data(result_ptr, item);
	if (infor_state == BK_FAIL)
		goto scan_rst_exit;

	/* check multiple nontransmit-bss and upload multiple bss infor to upper*/
	const u8 *multiple_ssid_required = rwnx_find_ie(WLAN_EID_MULTIPLE_BSSID, var_part_addr, vies_len);
	if (multiple_ssid_required)
		rwnx_inform_multiple_bss_data(result_ptr, item, var_part_addr, vies_len);

#else
	if (replace_index >= 0) {
		sr_free_result_item((UINT8 *)result_ptr->res[replace_index]);
		result_ptr->res[replace_index] = item;
	} else {
		result_ptr->res[result_ptr->scanu_num] = item;
		result_ptr->scanu_num++;
	}
#endif

scan_rst_exit:
	return ret;
}

#if CONFIG_RWNX_SW_TXQ

/******************************************************************************
 * Power Save functions
 *****************************************************************************/
/**
 * rwnx_set_traffic_status - Inform FW if traffic is available for STA in PS
 *
 * @rwnx_hw: Driver main data
 * @sta: Sta in PS mode
 * @available: whether traffic is buffered for the STA
 * @ps_id: type of PS data requested (@LEGACY_PS_ID or @UAPSD_ID)
  */
void rwnx_set_traffic_status(STA_INF_PTR sta, bool available, u8 ps_id)
{
	bool uapsd = (ps_id != LEGACY_PS_ID);
	rw_send_me_traffic_ind(mac_sta_mgmt_get_staid(sta), uapsd, available);
	// trace_ps_traffic_update(sta->sta_idx, available, uapsd);
}

/**
 * rwnx_ps_bh_enable - Enable/disable PS mode for one STA
 *
 * @rwnx_hw: Driver main data
 * @sta: Sta which enters/leaves PS mode
 * @enable: PS mode status
 *
 * This function will enable/disable PS mode for one STA.
 * When enabling PS mode:
 *  - Stop all STA's txq for RWNX_TXQ_STOP_STA_PS reason
 *  - Count how many buffers are already ready for this STA
 *  - For BC/MC sta, update all queued SKB to use hw_queue BCMC
 *  - Update TIM if some packet are ready
 *
 * When disabling PS mode:
 *  - Start all STA's txq for RWNX_TXQ_STOP_STA_PS reason
 *  - For BC/MC sta, update all queued SKB to use hw_queue AC_BE
 *  - Update TIM if some packet are ready (otherwise fw will not update TIM
 *    in beacon for this STA)
 *
 * All counter/skb updates are protected from TX path by taking tx_lock
 *
 * NOTE: _bh_ in function name indicates that this function is called
 * from a bottom_half tasklet.
 */
void rwnx_ps_bh_enable(STA_INF_PTR sta, bool enable)
{
	struct rwnx_txq *txq;
	struct rwnx_hw *rwnx_hw = &g_rwnx_hw;
	struct rwnx_sta_ps *ps = sta_mgmt_get_sta_ps(sta);


	if (enable) {
		// trace_ps_enable(sta);

		spin_lock(&rwnx_hw->tx_lock);
		ps->active = true;
		ps->sp_cnt[LEGACY_PS_ID] = 0;
		ps->sp_cnt[UAPSD_ID] = 0;
		rwnx_txq_sta_stop(sta, RWNX_TXQ_STOP_STA_PS);

		if (is_multicast_sta(mac_sta_mgmt_get_staid(sta))) {
			txq = rwnx_txq_sta_get(sta, 0);
			ps->pkt_ready[LEGACY_PS_ID] = skb_queue_len(&txq->sk_list);
			ps->pkt_ready[UAPSD_ID] = 0;
			txq->hwq = &rwnx_hw->hwq[RWNX_HWQ_BCMC];
		} else {
			int i;
			ps->pkt_ready[LEGACY_PS_ID] = 0;
			ps->pkt_ready[UAPSD_ID] = 0;
			foreach_sta_txq(sta, txq, i) {
				ps->pkt_ready[txq->ps_id] += skb_queue_len(&txq->sk_list);
			}
		}

		spin_unlock(&rwnx_hw->tx_lock);

		if (ps->pkt_ready[LEGACY_PS_ID])
			rwnx_set_traffic_status(sta, true, LEGACY_PS_ID);

		if (ps->pkt_ready[UAPSD_ID])
			rwnx_set_traffic_status(sta, true, UAPSD_ID);

		rw_msg_send_recycle_txlist_req(mac_sta_mgmt_get_staid(sta));


	} else {
		// trace_ps_disable(mac_sta_mgmt_get_staid(sta));

		spin_lock(&rwnx_hw->tx_lock);
		ps->active = false;

		if (is_multicast_sta(mac_sta_mgmt_get_staid(sta))) {
			txq = rwnx_txq_sta_get(sta, 0);
			txq->hwq = &rwnx_hw->hwq[RWNX_HWQ_BE];
		}

		rwnx_txq_sta_start(sta, RWNX_TXQ_STOP_STA_PS);
		spin_unlock(&rwnx_hw->tx_lock);

		if (ps->pkt_ready[LEGACY_PS_ID])
			rwnx_set_traffic_status(sta, false, LEGACY_PS_ID);

		if (ps->pkt_ready[UAPSD_ID])
			rwnx_set_traffic_status(sta, false, UAPSD_ID);
	}
}


static inline int rwnx_rx_ps_change_ind(struct ke_msg *msg)
{
	struct mm_ps_change_ind *ind = (struct mm_ps_change_ind *)msg->param;
	STA_INF_PTR sta = NULL;
	GLOBAL_INT_DECLARATION();

	// RWNX_DBG(RWNX_FN_ENTRY_STR);
	GLOBAL_INT_DISABLE();
	if (ind->sta_idx >= (NX_REMOTE_STA_MAX + NX_VIRT_DEV_MAX)) {
		RWNX_LOGE("Invalid sta index reported by fw %d\n",
				  ind->sta_idx);
		GLOBAL_INT_RESTORE();
		return 1;
	}

	sta = sta_mgmt_get_entry(ind->sta_idx);

	//RWNX_LOGI("Sta %d, change PS mode to %s\n", ind->sta_idx,
	//		  ind->ps_state ? "ON" : "OFF");

	// sta->mac_addr.array[0] == 0x1 means BCMC virtual sta.
	if (sta_mgmt_get_valid(sta) || sta_mgmt_get_mac(sta)[0] == 0x1) {
		rwnx_ps_bh_enable(sta, ind->ps_state);
#if 0
	} else if (test_bit(RWNX_DEV_ADDING_STA, &rwnx_hw->flags)) {
		sta->ps.active = ind->ps_state ? true : false;
#endif
	} else {
		RWNX_LOGW("Ignore PS mode change on invalid sta %d\n", ind->sta_idx);
	}
	GLOBAL_INT_RESTORE();

	return 0;
}
#endif // CONFIG_RWNX_SW_TXQ

#if CONFIG_WIFI_P2P
static int rwnx_rx_p2p_vif_ps_change_ind(struct ke_msg *msg)
{
	int vif_idx  = ((struct mm_p2p_vif_ps_change_ind *)msg->param)->vif_index;
	int ps_state = ((struct mm_p2p_vif_ps_change_ind *)msg->param)->ps_state;
	struct vif_info_tag *vif_entry = NULL;
	GLOBAL_INT_DECLARATION();

	// RWNX_DBG(RWNX_FN_ENTRY_STR);

	GLOBAL_INT_DISABLE();

	if (vif_idx != INVALID_VIF_IDX)
		vif_entry = &vif_info_tab[vif_idx];

	if (!vif_entry || !vif_entry->active) {
		GLOBAL_INT_RESTORE();
		return 0;
	}

	if (ps_state == 0 /* MM_PS_MODE_OFF */) {
		// Start TX queues for provided VIF
		rwnx_txq_vif_start(vif_entry, RWNX_TXQ_STOP_VIF_PS);
	} else {
		// Stop TX queues for provided VIF
		rwnx_txq_vif_stop(vif_entry, RWNX_TXQ_STOP_VIF_PS);
	}
	GLOBAL_INT_RESTORE();
}
#endif

#if CONFIG_RWNX_SW_TXQ
static inline int rwnx_rx_twt_ps_change_ind(struct ke_msg *msg)
{
	struct mm_twt_ps_change_ind *ind  = (struct mm_twt_ps_change_ind *)msg->param;
	void *vif_entry = NULL;

	GLOBAL_INT_DECLARATION();

	// RWNX_DBG(RWNX_FN_ENTRY_STR);
	GLOBAL_INT_DISABLE();


	if (ind->vif_index != INVALID_VIF_IDX)
		vif_entry = mac_vif_mgmt_get_entry(ind->vif_index);

	if (!vif_entry || !mac_vif_mgmt_get_active(vif_entry)) {
		GLOBAL_INT_RESTORE();
		return 1;
	}

	if (ind->twt_ps_state) {
		// Stop TX queues for provided VIF
		RWNX_LOGD("TWT PS stop! state %d\n",ind->twt_ps_state);
		rwnx_txq_vif_stop(vif_entry, RWNX_TXQ_STOP_TWT_PS);
	} else {
		// Start TX queues for provided VIF
		RWNX_LOGD("TWT PS start! state %d\n",ind->twt_ps_state);
		rwnx_txq_vif_start(vif_entry, RWNX_TXQ_STOP_TWT_PS);
	}
	GLOBAL_INT_RESTORE();

	return 0;
}
#endif

void rwm_msdu_twt_ps_change_ind_handler(void *msg)
{
	struct ke_msg *msg_ptr = (struct ke_msg *)msg;
	struct mm_twt_ps_change_ind *ind;

	if(!msg_ptr || !msg_ptr->param)
		return;

	ind = (struct mm_twt_ps_change_ind *)msg_ptr->param;

	if (ind->twt_ps_state == 0)
	{
#if CONFIG_TX_BUFING
		rwm_trigger_tx_bufing_stop(TX_BUFING_SRC_TWT, ind->vif_index);
#endif
	}
	else if (ind->twt_ps_state == 1)
	{
#if CONFIG_TX_BUFING
		rwm_trigger_tx_bufing_start(TX_BUFING_SRC_TWT, ind->vif_index);
#endif
	}

}

//TODO remove old event handler
void rwnx_handle_recv_msg(struct ke_msg *rx_msg)
{
	//uint32_t param;
	//FUNC_1PARAM_PTR fn = bk_wlan_get_status_cb();
	//wifi_event_sta_disconnected_t sta_disconnected = {0};
	//wifi_linkstate_reason_t info;

	switch (rx_msg->id) {

	/**************************************************************************/
	/*                          scan_hdlrs                                    */
	/**************************************************************************/
	case SCANU_START_CFM:
		/* scan complete */
		if (scan_rst_set_ptr) {
			/* scan activity has valid result*/
			resultful_scan_cfm = 1;
		}

		#if CONFIG_WIFI_SCAN_COUNTRY_CODE
		site_survey_cc = false;
		#endif // CONFIG_WIFI_SCAN_COUNTRY_CODE

		sort_scan_result(scan_rst_set_ptr);
			wpa_ctrl_event(WPA_CTRL_EVENT_SCAN_RESULTS, NULL);

		break;

	case SCANU_RESULT_IND:
		/* scan result indication */
		if (resultful_scan_cfm && scan_rst_set_ptr) {
			sr_flush_scan_results(scan_rst_set_ptr);

			scan_rst_set_ptr = 0;
			resultful_scan_cfm = 0;
		}

		if (0 == scan_rst_set_ptr) {
			scan_rst_set_ptr = (SCAN_RST_UPLOAD_T *)sr_malloc_shell();
			if (scan_rst_set_ptr) {
				scan_rst_set_ptr->scanu_num = 0;
				scan_rst_set_ptr->res = (SCAN_RST_ITEM_PTR *)&scan_rst_set_ptr[1];
				mhdr_scanu_result_ind(scan_rst_set_ptr, rx_msg, rx_msg->param_len);
			} else
				RWNX_LOGI("scan_rst_set_ptr malloc fail\r\n");
		} else {
			mhdr_scanu_result_ind(scan_rst_set_ptr, rx_msg, rx_msg->param_len);
		}

		break;

	case SCANU_JOIN_IND:
		break;

		/**************************************************************************/
		/*							sm_hdlrs								      */
		/**************************************************************************/
	case SM_CONNECT_IND:
		/* connect indication */
		if (scan_rst_set_ptr) {
			sr_flush_scan_results(scan_rst_set_ptr);

			scan_rst_set_ptr = 0;
			resultful_scan_cfm = 0;
		}

		mhdr_connect_ind(rx_msg, rx_msg->param_len);
		break;

#if defined(CONFIG_PMF)
	case RXU_MGT_IND:
		// STA mgmt: FIXME: AP may sends RXU_MGT_IND?
		mhdr_mgmt_ind(rx_msg, rx_msg->param_len);
		break;
#endif

	case SM_DISCONNECT_IND: {
		struct ke_msg *msg_ptr;
		struct sm_disconnect_ind *ind;

		msg_ptr = (struct ke_msg *)rx_msg;
		ind = (struct sm_disconnect_ind *)msg_ptr->param;

		RWNX_LOGD("disconnect\r\n");

#if defined(CONFIG_IEEE80211R) || defined(CONFIG_WNM)
				if (!ind->reassoc)
#endif
				{
#if NX_VERSION > NX_VERSION_PACK(6, 22, 0, 0)
					rwnx_misc_disconnect_ind(ind);
#endif
					wpa_ctrl_event_copy(WPA_CTRL_EVENT_DISCONNECT_IND, ind, sizeof(*ind));

#if (!CONFIG_SOC_BK7271)
					extern UINT32 rwnx_sys_is_enable_hw_tpc(void);
					if (rwnx_sys_is_enable_hw_tpc() == 0)
						rwnx_cal_set_txpwr(20, 11);
#endif
				}
			}	break;

	case SM_BEACON_LOSE_IND: {
		/* when beacon loss, umac will start disconnect procedure, and another ke_msg will uploaded */
#if 0
		info.state = WIFI_LINKSTATE_STA_DISCONNECTED;
		info.reason_code = WIFI_REASON_BEACON_LOST;
		mhdr_set_station_status(info);

#if !CONFIG_DISABLE_DEPRECIATED_WIFI_API
		if (fn) {
			param = WIFI_LINKSTATE_STA_DISCONNECTED;
			(*fn)(&param);
		}
#endif
		sta_disconnected.disconnect_reason = info.reason_code;
		sta_disconnected.local_generated = true;
		BK_LOG_ON_ERR(bk_event_post(EVENT_MOD_WIFI, EVENT_WIFI_STA_DISCONNECTED,
					  &sta_disconnected, sizeof(sta_disconnected), BEKEN_NEVER_TIMEOUT));
#if (CONFIG_RF_POLICY_WIFI_REQ)
		wifi_station_status_event_notice(0, WIFI_LINKSTATE_STA_DISCONNECTED);
#endif
#endif
	}
	break;


	/**************************************************************************/
	/*							me_hdlrs									  */
	/**************************************************************************/
	case ME_TKIP_MIC_FAILURE_IND:
		break;

	/**************************************************************************/
	/*							mm_hdlrs									  */
	/**************************************************************************/
	case MM_TAGGED_PARAM_CHANGE: {
		RWNX_LOGD("MM_TAGGED_PARAM_CHANGE\r\n");
	}
		break;

	case MM_CHANNEL_SWITCH_IND: {
		struct ke_msg *msg_ptr;
		struct mm_channel_switch_ind *ind;

		msg_ptr = (struct ke_msg *)rx_msg;
		ind = (struct mm_channel_switch_ind *)msg_ptr->param;
		if (ind->roc)
			wpa_ctrl_event_copy(WPA_CTRL_EVENT_REMAIN_ON_CHANNEL, ind, sizeof(*ind));
	}	break;
	case MM_CHANNEL_PRE_SWITCH_IND:
		/*
		 * Host should stop pushing packets for transmission after reception
		 * of this message.
		 */
		break;
	case MM_REMAIN_ON_CHANNEL_EXP_IND: {
		struct rwnx_hw *rwnx_hw = &g_rwnx_hw;
		struct rwnx_roc_elem *roc_elem = rwnx_hw->roc_elem;

		if (roc_elem) {
			wpa_ctrl_request_async(WPA_CTRL_EVENT_CANCEL_REMAIN_ON_CHANNEL, NULL);
			os_free(roc_elem);
		}
		rwnx_hw->roc_elem = 0;
	}	break;
	case MM_PS_CHANGE_IND:
#if CONFIG_RWNX_SW_TXQ
		rwnx_rx_ps_change_ind(rx_msg);
#else
#if CONFIG_AP_PS
		rwm_msdu_ps_change_ind_handler(rx_msg);
#endif
#endif
		break;
	case MM_TRAFFIC_REQ_IND:
		break;
#if CONFIG_USE_P2P_PS
	case MM_P2P_VIF_PS_CHANGE_IND:
#if CONFIG_RWNX_SW_TXQ
		rwnx_rx_p2p_vif_ps_change_ind(rx_msg);
#else
	    rwm_p2p_ps_change_ind_handler(rx_msg);
#endif /* CONFIG_RWNX_SW_TXQ */
		break;
#endif /* CONFIG_USE_P2P_PS */
	case MM_CSA_COUNTER_IND:
		break;
	case MM_CSA_FINISH_IND: {
		// rwnx_rx_csa_finish_ind();
		struct rwnx_hw *rwnx_hw = &g_rwnx_hw;

		bk_work_sched(&rwnx_hw->csa_work);
	}	break;
	case MM_CSA_TRAFFIC_IND:
		break;
	case MM_CHANNEL_SURVEY_IND:
		break;
	case MM_P2P_NOA_UPD_IND:
		break;

	case MM_RSSI_STATUS_IND: {
		struct ke_msg *msg_ptr;
		struct mm_rssi_status_ind *ind;
		msg_ptr = (struct ke_msg *)rx_msg;
		ind = (struct mm_rssi_status_ind *)msg_ptr->param;

		wpa_ctrl_event_copy(WPA_CTRL_EVENT_MM_RSSI_STATUS_IND, ind, sizeof(*ind));
	}	break;
	case ME_INDICATE_SEND_ARP_IND: {
		struct ke_msg *msg_ptr;
		struct me_indicate_send_arp_ind *ind;

		msg_ptr = (struct ke_msg *)rx_msg;
		ind = (struct me_indicate_send_arp_ind *)msg_ptr->param;
		//net_begin_send_arp_reply(ind->is_send_arp, ind->is_allow_send_req);

	}	break;
	case ME_SEND_MQTT_PINGREQ_IND: {
		bk_trigger_mqttping();
	}	break;

#ifdef CONFIG_SAE_EXTERNAL
	case SM_EXTERNAL_AUTH_REQUIRED_IND: {
		struct ke_msg *msg_ptr;
		struct sm_external_auth_required_ind *ind;

		msg_ptr = (struct ke_msg *)rx_msg;
		ind = (struct sm_external_auth_required_ind *)msg_ptr->param;

		wpa_ctrl_event_copy(WPA_CTRL_EVENT_EXTERNAL_AUTH_IND, ind, sizeof(*ind));
	}
	break;
#endif

#ifdef CONFIG_IEEE80211R
		case SM_FT_AUTH_IND: {
			struct ke_msg *msg_ptr;
			struct sm_ft_auth_ind *ind;

			msg_ptr = (struct ke_msg *)rx_msg;
			ind = (struct sm_ft_auth_ind *)msg_ptr->param;

			wpa_ctrl_event_copy(WPA_CTRL_EVENT_FT_AUTH_IND, ind, sizeof(*ind) + ind->ft_ie_len);
		}
		break;
#endif

	/**************************************************************************/
	/*							apm_hdlrs									  */
	/**************************************************************************/

	case APM_PROBE_CLIENT_IND: {
		// cfg80211_probe_status

		struct ke_msg *msg_ptr = (struct ke_msg *)rx_msg;
		struct apm_probe_client_ind *ind = (struct apm_probe_client_ind *)msg_ptr->param;

		wpa_ctrl_event_copy(WPA_CTRL_EVENT_PROBE_STATUS, ind, sizeof(*ind));
	}	break;

	case MM_TWT_PS_CHANGE_IND:
#if CONFIG_RWNX_SW_TXQ
		rwnx_rx_twt_ps_change_ind(rx_msg);
#else
		rwm_msdu_twt_ps_change_ind_handler(rx_msg);
#endif
		break;

	default:
		//RWNX_LOGI("unknown msg 0x%x\n", rx_msg->id);
		break;
	}
}

void rwnx_recv_msg(void)
{
	struct ke_msg *rx_msg;
	MSG_SND_NODE_PTR tx_msg;
	struct co_list_hdr *rx_node, *tx_node;
	uint8_t found = 0;

	GLOBAL_INT_DECLARATION();

	while (1) {
		found = 0;

		GLOBAL_INT_DISABLE();
		rx_node = co_list_pop_front(&rw_msg_rx_head);
		GLOBAL_INT_RESTORE();
		if (!rx_node)
			break;

		rx_msg = (struct ke_msg *)rx_node;

		GLOBAL_INT_DISABLE();
		tx_node = co_list_pick(&rw_msg_tx_head);
		while (tx_node) {
			tx_msg = (MSG_SND_NODE_PTR)tx_node;
			if (rx_msg->id == tx_msg->reqid) {
				found = 1;
				co_list_extract(&rw_msg_tx_head, tx_node);
				break;
			}

			tx_node = co_list_next(tx_node);
		}
		GLOBAL_INT_RESTORE();

		if (found) {
			int ret;

			if (tx_msg->cfm && rx_msg->param_len)
				os_memcpy(tx_msg->cfm, &rx_msg->param[0], rx_msg->param_len);

			ret = rtos_set_semaphore(&tx_msg->semaphore);
			BK_ASSERT(0 == ret); /* ASSERT VERIFIED */
		} else {
			rwnx_handle_recv_msg(rx_msg);
		}

		ke_msg_free(rx_msg);
	}
}

