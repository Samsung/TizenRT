#include "rtw_wifi_constants.h"
#include "utils/includes.h"
#include "utils/common.h"
#include "utils/wpa_debug.h"
#include "p2p/p2p.h"
#include "p2p/p2p_i.h"
#include "common/ieee802_11_defs.h"
#include "common/ieee802_11_common.h"
#include "wps_protocol_handler.h"
#include "wpa_lite_intf.h"
#include "wifi_intf_drv_to_upper.h"
#include "wifi_p2p_supplicant.h"
#include "wifi_wps_config.h"
#include "dhcps.h"

#ifndef P2P_MAX_INITIAL_CONN_WAIT
/*
 * How many seconds to wait for initial 4-way handshake to get completed after
 * WPS provisioning step.
 */
#define P2P_MAX_INITIAL_CONN_WAIT 10
#endif

struct p2p_cmd_priv g_p2p_cmdpriv;
struct p2p_context g_p2p_context;

int wifi_init_ap(void);

void wifi_p2p_notify_dev_scanned(u8 *addr, u8 role, char *name, u8 channel, int rssi)
{
	RTK_LOGA(NOTAG, "P2P-NOTIFY: "P2P_EVENT_DEVICE_SCANNED MACSTR, MAC2STR(addr));

	switch (role) {
	case P2P_ROLE_DEVICE:
		RTK_LOGA(NOTAG, " role=DEVICE");
		break;
	case P2P_ROLE_GO:
		RTK_LOGA(NOTAG, " role=GO");
		break;
	default:
		RTK_LOGA(NOTAG, " role=NONE");
		break;
	}
	if (role == P2P_ROLE_GO) {
		RTK_LOGA(NOTAG, " ssid=%s oper_ch=%d rssi=%d\n", name, channel, rssi);
	} else {
		RTK_LOGA(NOTAG, " name=%s listen_ch=%d\n", name, channel);
	}
}

void wifi_p2p_notify_dev_found(const u8 *addr,
							   const struct p2p_peer_info *info,
							   int new_device)
{
	char devtype[WPS_DEV_TYPE_BUFSIZE];

	(void) new_device;

	RTK_LOGA(NOTAG, "P2P-NOTIFY: "P2P_EVENT_DEVICE_FOUND MACSTR
			 " p2p_dev_addr=" MACSTR
			 " pri_dev_type=%s name='%s' config_methods=0x%x "
			 "dev_capab=0x%x group_capab=0x%x\n",
			 MAC2STR(addr), MAC2STR(info->p2p_device_addr),
			 wps_dev_type_bin2str(info->pri_dev_type, devtype,
								  sizeof(devtype)),
			 info->device_name, info->config_methods,
			 info->dev_capab, info->group_capab);
}

void wifi_p2p_notify_dev_lost(const u8 *dev_addr)
{
	RTK_LOGA(NOTAG, "P2P-NOTIFY: "P2P_EVENT_DEVICE_LOST
			 "p2p_dev_addr=" MACSTR "\n", MAC2STR(dev_addr));
}

void wifi_p2p_notify_find_stopped(void)
{
	RTK_LOGA(NOTAG, "P2P-NOTIFY: "P2P_EVENT_FIND_STOPPED"\n");
}

void wifi_p2p_notify_go_neg_req_rx(const u8 *src, u16 dev_passwd_id)
{
	RTK_LOGA(NOTAG, "P2P-NOTIFY: "P2P_EVENT_GO_NEG_REQUEST MACSTR
			 " dev_passwd_id=%u\n", MAC2STR(src), dev_passwd_id);
}

void wifi_p2p_notify_go_neg_result(int bsuccess)
{
	if (bsuccess) {
		RTK_LOGA(NOTAG, "P2P-NOTIFY: "P2P_EVENT_GO_NEG_SUCCESS"\n");
	} else {
		RTK_LOGA(NOTAG, "P2P-NOTIFY: "P2P_EVENT_GO_NEG_FAILURE"\n");
	}

}

void wifi_p2p_notify_group_formation_result(int bsuccess)
{
	if (bsuccess) {
		RTK_LOGA(NOTAG, "P2P-NOTIFY: "P2P_EVENT_GROUP_FORMATION_SUCCESS"\n");
	} else {
		RTK_LOGA(NOTAG, "P2P-NOTIFY: "P2P_EVENT_GROUP_FORMATION_FAILURE"\n");
	}
}

void wifi_p2p_notify_group_started(char *ssid,
								   u8 channel, char *passphrase,
								   u8 *go_dev_addr, int persistent)
{
	RTK_LOGA(NOTAG, "P2P-NOTIFY: "P2P_EVENT_GROUP_STARTED
			 " GO ssid=\"%s\" channel=%d passphrase=\"%s\" "
			 "go_dev_addr=" MACSTR "%s\n",
			 ssid, channel, passphrase,
			 MAC2STR(go_dev_addr),
			 persistent ? " [PERSISTENT]" : "");
}

void wifi_p2p_notify_group_removed(u32 role, const char *reason)
{
	if (role == P2P_R_GO) {
		RTK_LOGA(NOTAG, "P2P-NOTIFY: "P2P_EVENT_GROUP_REMOVED "GO %s\n", reason);
	} else if (role == P2P_R_CLIENT) {
		RTK_LOGA(NOTAG, "P2P-NOTIFY: "P2P_EVENT_GROUP_REMOVED "CLIENT %s\n", reason);
	}
}

void wifi_p2p_notify_ap_sta_connected(u8 *addr)
{
	RTK_LOGA(NOTAG, "NOTIFY: "AP_STA_CONNECTED MAC_FMT"\n", MAC_ARG(addr));
}

void wifi_p2p_notify_ap_sta_disconnected(u8 *addr)
{
	RTK_LOGA(NOTAG, "NOTIFY: "AP_STA_DISCONNECTED MAC_FMT"\n", MAC_ARG(addr));
}

static void wifi_p2p_add_chan(struct p2p_reg_class *reg, u8 chan)
{
	reg->channel[reg->channels] = chan;
	reg->channels++;
}

static int wifi_p2p_default_channels(struct p2p_channels *chan)
{
	int i, cla = 0;

	wpa_printf(MSG_DEBUG, "P2P: Enable operating classes for 2.4 GHz "
			   "band");

	/* Operating class 81 - 2.4 GHz band channels 1..13 */
	chan->reg_class[cla].reg_class = 81;
	chan->reg_class[cla].channels = 0;
	for (i = 0; i < 11; i++) {
		wifi_p2p_add_chan(&chan->reg_class[cla], i + 1);
	}

	if (chan->reg_class[cla].channels) {
		cla++;
	}

	wpa_printf(MSG_DEBUG, "P2P: Enable operating classes for lower 5 GHz "
			   "band");
	/* Operating class 115 - 5 GHz, channels 36-48 */
	chan->reg_class[cla].reg_class = 115;
	chan->reg_class[cla].channels = 0;
	wifi_p2p_add_chan(&chan->reg_class[cla], 36);
	wifi_p2p_add_chan(&chan->reg_class[cla], 40);
	wifi_p2p_add_chan(&chan->reg_class[cla], 44);
	wifi_p2p_add_chan(&chan->reg_class[cla], 48);

	if (chan->reg_class[cla].channels) {
		cla++;
	}

	wpa_printf(MSG_DEBUG, "P2P: Enable operating classes for higher 5 GHz "
			   "band");
	/* Operating class 124 - 5 GHz, channels 149,153,157,161 */
	chan->reg_class[cla].reg_class = 124;
	chan->reg_class[cla].channels = 0;
	wifi_p2p_add_chan(&chan->reg_class[cla], 149);
	wifi_p2p_add_chan(&chan->reg_class[cla], 153);
	wifi_p2p_add_chan(&chan->reg_class[cla], 157);
	wifi_p2p_add_chan(&chan->reg_class[cla], 161);
	if (chan->reg_class[cla].channels) {
		cla++;
	}

	chan->reg_classes = cla;

	return 0;
}

int wifi_p2p_set_assoc_req_ie(u8 *bssid, u8 *ies, u16 len)
{
	int ret;

	if (g_p2p_context.role == P2P_R_DISABLE || g_p2p_context.p2p == NULL) {
		return 0;
	}

	ret = p2p_assoc_req_ie(g_p2p_context.p2p, bssid, ies, len,
						   1, NULL);

	return ret;
}

static void wifi_p2p_set_probe_req_ie(unsigned int num_req_dev_types,
									  const u8 *req_dev_types, const u8 *dev_id, u16 pw_id)
{
	struct wpabuf *wps_ie, *ies;
	size_t ielen;
	u8 wlan_idx = (g_p2p_context.role == P2P_R_GO) ? SOFTAP_WLAN_INDEX : STA_WLAN_INDEX;

	wpa_printf(MSG_DEBUG, "P2P: Set Probe Req ie. num_req_dev_types=%d, num_sec_dev_types=%d.",
			   num_req_dev_types, g_wps_context.wps->dev.num_sec_dev_types);
	g_wps_context.wps->dev.p2p = 1;
	wps_ie = wps_build_probe_req_ie(pw_id, &(g_wps_context.wps->dev),
									g_wps_context.wps->uuid, WPS_REQ_ENROLLEE,
									num_req_dev_types, req_dev_types);
	if (wps_ie == NULL) {
		return;
	}

	ielen = p2p_scan_ie_buf_len(g_p2p_context.p2p);
	ies = wpabuf_alloc(wpabuf_len(wps_ie) + ielen);
	if (ies == NULL) {
		wpabuf_free(wps_ie);
		return;
	}
	wpabuf_put_buf(ies, wps_ie);
	wpabuf_free(wps_ie);

	p2p_scan_ie(g_p2p_context.p2p, ies, dev_id);

	wpa_hexdump_key(MSG_MSGDUMP, "P2P IEs:", (const void *) wpabuf_head(ies), wpabuf_len(ies));
	wifi_set_gen_ie(wlan_idx, (char *)wpabuf_head(ies), wpabuf_len(ies), P2PWPS_PROBE_REQ_IE);

	wpabuf_free(ies);
}

void wifi_p2p_scan_done(u8 *param)
{
	(void) param;

	struct p2p_data *p2p = g_p2p_context.p2p;

	p2p_scan_res_handled(p2p, 0);
}

int wifi_p2p_enqueue_cmd(u8 *parmbuf, void(*cmd_hdl)(u8 *pcmd))
{
	int res = RTK_SUCCESS;
	struct p2p_cmd_obj *cmd_obj = NULL;

	cmd_obj = (struct p2p_cmd_obj *)rtos_mem_zmalloc(sizeof(struct p2p_cmd_obj));
	if (cmd_obj == NULL) {
		if (parmbuf) {
			rtos_mem_free(parmbuf);
		}
		res = RTK_FAIL;
		goto exit;
	}

	INIT_LIST_HEAD(&cmd_obj->list);
	cmd_obj->parmbuf = parmbuf;
	cmd_obj->cmd_hdl = cmd_hdl;

	rtos_mutex_take(g_p2p_cmdpriv.lock, 0xFFFFFFFF);
	list_add_tail(&cmd_obj->list, &g_p2p_cmdpriv.queue);
	rtos_mutex_give(g_p2p_cmdpriv.lock);

	rtos_sema_give(g_p2p_cmdpriv.wakeup_sema);

exit:
	if (res == RTK_FAIL) {
		RTK_LOGS(TAG_WLAN_DRV, RTK_LOG_ERROR, "Enqueue p2p cmd 0x%08x fail\n ", (void *)cmd_hdl);
	}
	return res;
}

struct	p2p_cmd_obj	*wifi_p2p_dequeue_cmd(void)
{
	struct p2p_cmd_obj *cmd_obj;

	rtos_mutex_take(g_p2p_cmdpriv.lock, 0xFFFFFFFF);
	if (list_empty(&g_p2p_cmdpriv.queue)) {
		cmd_obj = NULL;
	} else {
		cmd_obj = list_entry(g_p2p_cmdpriv.queue.next, struct p2p_cmd_obj, list);
		list_del_init(&cmd_obj->list);
	}
	rtos_mutex_give(g_p2p_cmdpriv.lock);

	return cmd_obj;
}

void wifi_p2p_cmd_process_task(void *param)
{
	struct p2p_cmd_obj *pcmd;

	(void) param;

	while (1) {
		/* wait for sema */
		rtos_sema_take(g_p2p_cmdpriv.wakeup_sema, 0xFFFFFFFF);

		/* dequeue */
		pcmd = wifi_p2p_dequeue_cmd();
		if (!pcmd) {
			continue;
		}

		/* process */
		pcmd->cmd_hdl(pcmd->parmbuf);

		/* free mem */
		rtos_mem_free((unsigned char *)pcmd->parmbuf);
		rtos_mem_free((unsigned char *)pcmd);
	}

	rtos_task_delete(NULL);
}

#define MAX_IE_SZ	768
u8 *rtw_get_p2p_ie(u8 *in_ie, u32 in_len, u8 *p2p_ie, u32 *p2p_ielen)
{
	u32 cnt;
	u8 *p2p_ie_ptr = NULL;
	u8 eid;
	const u8 P2P_OUI[4] = {0x50, 0x6F, 0x9A, 0x09};

	if (p2p_ielen) {
		*p2p_ielen = 0;
	}

	if (!in_ie) {
		return (u8 *)p2p_ie_ptr;
	}
	cnt = 0;

	while (cnt + 1 + 4 < in_len) {
		eid = in_ie[cnt];

		if (cnt + 1 + 4 >= MAX_IE_SZ) {
			return NULL;
		}

		if (eid == WLAN_EID_VENDOR_SPECIFIC && memcmp(&in_ie[cnt + 2], P2P_OUI, 4) == 0) {
			p2p_ie_ptr = in_ie + cnt;

			if (p2p_ie) {
				memcpy(p2p_ie, &in_ie[cnt], in_ie[cnt + 1] + 2);
			}

			if (p2p_ielen) {
				*p2p_ielen = in_ie[cnt + 1] + 2;
			}

			break;
		} else {
			cnt += in_ie[cnt + 1] + 2;
		}

	}

	return (u8 *)p2p_ie_ptr;
}

u8 *rtw_get_p2p_attr(u8 *p2p_ie, int p2p_ielen, u8 target_attr_id, u32 *len_attr)
{
	u8 *attr_ptr = NULL;
	u8 *target_attr_ptr = NULL;
	u8 attr_id = 0;
	u16 attr_len = 0;

	if (!p2p_ie || p2p_ielen <= 6) {
		return attr_ptr;
	}

	if (len_attr) {
		*len_attr = 0;
	}

	/* 6 = 1(Element ID) + 1(Length) + 3 (OUI) + 1(OUI Type), go to first attr */
	attr_ptr = p2p_ie + 6;
	while ((attr_ptr - p2p_ie + 3) <= p2p_ielen) {/* 3 = 1(Attribute ID) + 2(Length) */
		attr_id = *attr_ptr;
		attr_len = (u16)((attr_ptr[2] << 8) | attr_ptr[1]) + 3;

		if ((attr_ptr - p2p_ie + attr_len) > p2p_ielen) {
			break;
		}
		if (attr_id == target_attr_id) {
			target_attr_ptr = attr_ptr;
			if (len_attr) {
				*len_attr = attr_len;
			}
			break;
		} else {
			attr_ptr += attr_len;
		}
	}

	return target_attr_ptr;
}

s32 wifi_p2p_scan_report_each_cb(struct rtw_scan_result *scanned_ap_info, void *user_data, u8 *ies, u32 ie_len)
{
	u8 p2p_role = P2P_ROLE_DISABLE;
	u8 *p2p_ie;
	u32 p2p_ielen;
	struct p2p_scan_report_node *node;
	struct p2p_scan_report_node *item;
	struct list_head *iterator;
	u8 new_node = TRUE, free_ie = TRUE;
	s32 res = 0;

	(void) user_data;

	if (scanned_ap_info == NULL) {
		res = -1;
		goto exit;
	}

	if (ies && ie_len) {
		p2p_ie = rtw_get_p2p_ie(ies + _FIXED_IE_LENGTH_,  ie_len - _FIXED_IE_LENGTH_, NULL, &p2p_ielen);
		if (p2p_ie) {
			if (rtw_get_p2p_attr(p2p_ie, (int)p2p_ielen, P2P_ATTR_GROUP_INFO, NULL)) {
				p2p_role = P2P_ROLE_GO;
			} else {
				p2p_role = P2P_ROLE_DEVICE;
			}
		} else {
			p2p_role = P2P_ROLE_DISABLE;
		}
	}

	if (p2p_role != P2P_ROLE_DISABLE) {
		rtos_mutex_take(g_p2p_context.scan_report_lock, 0xFFFFFFFF);
		list_for_each(iterator, &g_p2p_context.p2p_scan_report_list) {
			item = list_entry(iterator, struct p2p_scan_report_node, list);
			if (memcmp(item->p_scanned_AP_info->bssid.octet, scanned_ap_info->bssid.octet, 6) == 0) {
				if (item->p_scanned_AP_info->signal_strength < scanned_ap_info->signal_strength) {
					/* replace old node */
					rtos_mem_free((void *)item->p_scanned_AP_info);
					item->p_scanned_AP_info = scanned_ap_info;
					item->role = p2p_role;

					if (item->ies) {
						rtos_mem_free(item->ies);
					}
					item->ies = ies;
					item->ie_len = ie_len;
					free_ie = FALSE;
				}
				new_node = FALSE;
			}
		}

		if (new_node) {
			/* insert new node */
			node = (struct p2p_scan_report_node *)rtos_mem_zmalloc(sizeof(struct p2p_scan_report_node));
			if (node == NULL) {
				rtos_mutex_give(g_p2p_context.scan_report_lock);
				res = -1;
				goto exit;
			}

			node->p_scanned_AP_info = scanned_ap_info;
			node->role = p2p_role;
			node->ies = ies;
			node->ie_len = ie_len;
			free_ie = FALSE;

			list_add_tail(&node->list, &g_p2p_context.p2p_scan_report_list);
		}
		rtos_mutex_give(g_p2p_context.scan_report_lock);
	}

exit:
	if (free_ie && ies) {
		rtos_mem_free(ies);
	}

	return res;
}

static s32 wifi_p2p_scan_res_callback(u32 ap_num, void *user_data)
{
	struct p2p_scan_report_node *item;
	struct list_head *iterator;
	struct rtw_scan_result	*p_scanned_AP_info;
	struct p2p_data *p2p = g_p2p_context.p2p;

	(void) ap_num;
	(void) user_data;

	if (g_p2p_context.role == P2P_R_DISABLE || g_p2p_context.p2p == NULL) {
		return RTK_FAIL;
	}

	if (p2p->state == P2P_SEARCH) {
		/* print scan results */
		rtos_mutex_take(g_p2p_context.scan_report_lock, 0xFFFFFFFF);
		list_for_each(iterator, &g_p2p_context.p2p_scan_report_list) {
			item = list_entry(iterator, struct p2p_scan_report_node, list);
			p_scanned_AP_info = item->p_scanned_AP_info;
			wifi_p2p_notify_dev_scanned(p_scanned_AP_info->bssid.octet, item->role, (char *)p_scanned_AP_info->ssid.val,
										p_scanned_AP_info->channel, p_scanned_AP_info->signal_strength);
		}
		rtos_mutex_give(g_p2p_context.scan_report_lock);
	}

	wpa_printf(MSG_DEBUG, "P2P: scan done!");

	/* create task to handle, because in p2p_scan_res_handled, device may enter listen state and remain on channel.
		roc would reuse scan flow, so this scan flow must be completed first. */
	wifi_p2p_enqueue_cmd(NULL, wifi_p2p_scan_done);

	return RTK_SUCCESS;
}

static int wifi_p2p_scan(void *ctx, enum p2p_scan_type type, int freq,
						 unsigned int num_req_dev_types,
						 const u8 *req_dev_types, const u8 *dev_id, u16 pw_id)
{
	int ret;
	struct rtw_scan_param scan_param = {0};
	u8 social_ch_plus_one[] = {1, 6, 11, 0};
	u8 channel = 0, reg_class;

	(void) ctx;
	(void) freq;

	if (g_p2p_context.role == P2P_R_DISABLE || g_p2p_context.p2p == NULL) {
		return -1;
	}

	wifi_p2p_set_probe_req_ie(num_req_dev_types, req_dev_types, dev_id, pw_id);
	wifi_p2p_set_state(P2P_STATE_SCAN);

	switch (type) {
	case P2P_SCAN_SOCIAL:
		scan_param.channel_list_num = 3;
		scan_param.channel_list = social_ch_plus_one;
		scan_param.options = RTW_SCAN_ACTIVE;
		break;
	case P2P_SCAN_FULL:
		scan_param.channel_list_num = 0;
		break;
	case P2P_SCAN_SPECIFIC:
		p2p_freq_to_channel(freq,
							&reg_class,
							&channel);
		scan_param.channel_list_num = 1;
		scan_param.channel_list = &channel;
		scan_param.options = RTW_SCAN_ACTIVE;
		break;
	case P2P_SCAN_SOCIAL_PLUS_ONE:
		if (p2p_supported_freq(g_p2p_context.p2p, freq)) {
			p2p_freq_to_channel(freq,
								&reg_class,
								&channel);
			social_ch_plus_one[3] = channel;
		}
		scan_param.channel_list_num = 4;
		scan_param.channel_list = social_ch_plus_one;
		scan_param.options = RTW_SCAN_ACTIVE;
		break;
	}
	wpa_printf(MSG_DEBUG, "P2P: wifi_p2p_scan, type: %d, p2p_role:%d!", type, g_p2p_context.role);

	scan_param.ssid = (u8 *)P2P_WILDCARD_SSID;
	scan_param.options |= RTW_SCAN_REPORT_EACH;
	if (g_p2p_context.role == P2P_R_GO) {
		scan_param.options |= RTW_SCAN_WITH_PORT1;
	}
	scan_param.scan_report_each_mode_user_callback = wifi_p2p_scan_report_each_cb;
	scan_param.scan_user_callback = wifi_p2p_scan_res_callback;

	ret = wifi_scan_networks(&scan_param, 0);
	ret = ret < 0 ? ret : 0;

	/* notify scan status */
	p2p_notify_scan_trigger_status(g_p2p_context.p2p, ret);

	return ret;
}

static u8 wifi_p2p_get_dev_chnl_from_scan_res(u8 *dest)
{
	struct p2p_scan_report_node *item;
	struct list_head *iterator;
	struct rtw_scan_result *p_scanned_AP_info;
	u8 ch = 0;

	rtos_mutex_take(g_p2p_context.scan_report_lock, 0xFFFFFFFF);
	list_for_each(iterator, &g_p2p_context.p2p_scan_report_list) {
		item = list_entry(iterator, struct p2p_scan_report_node, list);
		p_scanned_AP_info = item->p_scanned_AP_info;

		if (!memcmp(p_scanned_AP_info->bssid.octet, dest, ETH_ALEN)) {
			wpa_printf(MSG_DEBUG, "P2P: Find the dest device from global scan buffer! channel=%d\n", p_scanned_AP_info->channel);
			ch = p_scanned_AP_info->channel;
			break;
		}
	}
	rtos_mutex_give(g_p2p_context.scan_report_lock);

	return ch;
}

static struct p2p_scan_report_node *wifi_p2p_get_dev_scan_res(u8 *dest)
{
	struct p2p_scan_report_node *item;
	struct list_head *iterator;
	struct rtw_scan_result *p_scanned_AP_info;

	rtos_mutex_take(g_p2p_context.scan_report_lock, 0xFFFFFFFF);
	list_for_each(iterator, &g_p2p_context.p2p_scan_report_list) {
		item = list_entry(iterator, struct p2p_scan_report_node, list);
		p_scanned_AP_info = item->p_scanned_AP_info;

		if (!memcmp(p_scanned_AP_info->bssid.octet, dest, ETH_ALEN)) {
			wpa_printf(MSG_DEBUG, "P2P: Find the dest device from global scan buffer! channel=%d\n", p_scanned_AP_info->channel);
			rtos_mutex_give(g_p2p_context.scan_report_lock);

			return item;
		}
	}
	rtos_mutex_give(g_p2p_context.scan_report_lock);

	return NULL;
}

s32 wifi_p2p_cancel_remain_on_ch(void)
{
	if (g_p2p_context.roch_onging) {
		g_p2p_context.roch_onging = FALSE;

		wpa_printf(MSG_DEBUG, "@roc cancel");
		wifi_scan_abort();
	}
	return 0;
}

static s32 wifi_p2p_roc_done_callback(u32 ap_num, void *user_data)
{
	(void) ap_num;
	(void) user_data;

	wpa_printf(MSG_DEBUG, "@roc done");
	g_p2p_context.roch_onging = FALSE;

	return 1;
}

static void wifi_p2p_remain_on_channel(u8 wlan_idx, u8 channel, u32 wait_time)
{
	struct rtw_scan_param scan_param = {0};
	s32 ret = 0;

	if (g_p2p_context.roch_onging) {
		return;
	}

	if (wifi_p2p_set_remain_on_ch(wlan_idx, ENABLE) != 0) {
		return;
	}

	wpa_printf(MSG_DEBUG, "roc: %d ms, ch:%d", wait_time, channel);

	g_p2p_context.roch_onging = TRUE;
	scan_param.ssid = NULL;
	scan_param.options = RTW_SCAN_PASSIVE;
	scan_param.chan_scan_time.passive_scan_time = wait_time;
	scan_param.scan_user_data = NULL;
	scan_param.channel_list_num = 1;
	scan_param.channel_list = &channel;
	scan_param.scan_user_callback = wifi_p2p_roc_done_callback;
	if (g_p2p_context.role == P2P_R_GO) {
		scan_param.options |= RTW_SCAN_WITH_PORT1;
	}

	/* switch channel with scan flow */
	ret = wifi_scan_networks(&scan_param, 0);
	if (ret < 0) {
		RTK_LOGS(NOTAG, RTK_LOG_ALWAYS, "p2p roc fail, ret:%d\n", ret);
	}

	/* wait for channel ready */
	rtos_sema_take(g_p2p_context.roc_ready_sema, 0xFFFFFFFF);
}

static int wifi_p2p_send_action(void *priv, unsigned int freq, const u8 *dst,
								const u8 *src, const u8 *bssid, const u8 *data,
								size_t data_len, unsigned int wait_time, int *scheduled)
{
	int ret = -1;
	u8 *buf;
	struct ieee80211_hdr *hdr;
	u8 channel = 0, reg_class;
	struct _raw_data_desc_t raw_data_desc = {0};
	u8 wlan_idx;

	(void) priv;

	if (g_p2p_context.role == P2P_R_DISABLE || g_p2p_context.p2p == NULL) {
		return -1;
	}

	if (scheduled) {
		*scheduled = 0;
	}

	wpa_printf(MSG_INFO, "P2P: Send action (freq=%u MHz wait=%d ms)",
			   freq, wait_time);

	buf = os_zalloc(24 + data_len);
	if (buf == NULL) {
		return ret;
	}

	os_memcpy(buf + 24, data, data_len);
	hdr = (struct ieee80211_hdr *) buf;
	hdr->frame_control = IEEE80211_FC(WLAN_FC_TYPE_MGMT, WLAN_FC_STYPE_ACTION);
	os_memcpy(hdr->addr1, dst, ETH_ALEN);
	os_memcpy(hdr->addr2, src, ETH_ALEN);
	os_memcpy(hdr->addr3, bssid, ETH_ALEN);

	p2p_freq_to_channel(freq,
						&reg_class,
						&channel);

	wpa_hexdump_key(MSG_MSGDUMP,  "P2P Action:", data, data_len);

	wlan_idx = (g_p2p_context.role == P2P_R_GO) ? SOFTAP_WLAN_INDEX : STA_WLAN_INDEX;
	wifi_p2p_remain_on_channel(wlan_idx, channel, wait_time);

	raw_data_desc.buf = (unsigned char *)buf;
	raw_data_desc.buf_len = 24 + data_len;
	raw_data_desc.wlan_idx = wlan_idx;
	raw_data_desc.flags = RTW_SEND_AND_WAIT_ACK;

	wifi_send_mgnt(&raw_data_desc);

	os_free(buf, 0);

	p2p_send_action_cb(g_p2p_context.p2p, freq, dst, src, bssid, P2P_SEND_ACTION_SUCCESS);

	return 0;
}

static void wifi_p2p_send_action_done(void *ctx)
{
	(void) ctx;
}

static int wifi_p2p_send_probe_resp(void *ctx, const struct wpabuf *buf,
									unsigned int freq)
{
	struct _raw_data_desc_t raw_data_desc = {0};

	(void) ctx;
	(void) freq;

	raw_data_desc.buf = (unsigned char *)wpabuf_head(buf);
	raw_data_desc.buf_len = wpabuf_len(buf);
	raw_data_desc.wlan_idx = (g_p2p_context.role == P2P_R_GO) ? SOFTAP_WLAN_INDEX : STA_WLAN_INDEX;
	wifi_send_mgnt(&raw_data_desc);

	return 0;
}

void wifi_p2p_rx_mgnt_handle(u8 *evt_info)
{
	struct rtw_event_rx_mgnt *rx_mgnt_info = (struct rtw_event_rx_mgnt *)evt_info;
	u8 *data = rx_mgnt_info->frame;
	u16 len = rx_mgnt_info->frame_len;
	u8 *da, *sa, *bssid, *ie, *frame_body;
	u8 stype, category;
	u16 ie_len = 0, frm_len = 0;
	int freq = 0;

	if (g_p2p_context.role == P2P_R_DISABLE || g_p2p_context.p2p == NULL) {
		return;
	}

	freq = p2p_channel_to_freq(g_p2p_context.p2p->cfg->reg_class, rx_mgnt_info->channel);
	if (freq < 0) {
		return;
	}

	stype = GetFrameSubType(data);
	da = GetAddr1Ptr(data);
	sa = GetAddr2Ptr(data);
	bssid = GetAddr3Ptr(data);

	wpa_printf(MSG_DEBUG, "p2p rx mgnt: stype=0x%x sa="MAC_FMT, stype, MAC_ARG(sa));
	if (stype == RTW_PROBEREQ) {
		ie = data + WLAN_HDR_A3_LEN + _PROBEREQ_IE_OFFSET_;
		ie_len = len - (WLAN_HDR_A3_LEN + _PROBEREQ_IE_OFFSET_);
		p2p_probe_req_rx(g_p2p_context.p2p, sa, da, bssid, ie, ie_len, freq, 0);
	} else if (stype == RTW_ACTION) {
		wpa_hexdump_key(MSG_DEBUG, "\r\nRx Action", data, len);
		frame_body = data + sizeof(struct ieee80211_hdr);
		category = frame_body[0];
		frm_len = len - sizeof(struct ieee80211_hdr) - 1;
		p2p_rx_action(g_p2p_context.p2p, da, sa, bssid, category, frame_body + 1, frm_len, freq);
	}

	rtos_mem_free(evt_info);

	return;
}

void wifi_p2p_rx_mgnt(u8 *evt_info)
{
	struct rtw_event_rx_mgnt *info = (struct rtw_event_rx_mgnt *)evt_info;
	u32 len = sizeof(struct rtw_event_rx_mgnt) + info->frame_len;
	u8 *rx_mgnt_info = rtos_mem_zmalloc(len);

	if (rx_mgnt_info == NULL) {
		return;
	}

	memcpy(rx_mgnt_info, (void *)evt_info, len);

	wifi_p2p_enqueue_cmd(rx_mgnt_info, wifi_p2p_rx_mgnt_handle);
}

void wifi_p2p_dev_found(void *ctx, const u8 *addr,
						const struct p2p_peer_info *info,
						int new_device)
{
	(void) ctx;
	wifi_p2p_notify_dev_found(addr, info, new_device);
}

static void wifi_p2p_dev_lost(void *ctx, const u8 *dev_addr)
{
	(void) ctx;
	wifi_p2p_notify_dev_lost(dev_addr);
}

void wifi_p2p_channel_switch_ready(u8 *evt_info)
{
	(void)evt_info;
	rtos_sema_give(g_p2p_context.roc_ready_sema);
}

static int wifi_p2p_start_listen(void *ctx, unsigned int freq,
								 unsigned int duration,
								 const struct wpabuf *probe_resp_ie)
{
	u8 channel;
	u8 reg_class;
	u8 wlan_idx = (g_p2p_context.role == P2P_R_GO) ? SOFTAP_WLAN_INDEX : STA_WLAN_INDEX;

	(void) ctx;

	/* Set IEs */
	wifi_set_gen_ie(wlan_idx, NULL, 0, P2PWPS_BEACON_IE);
	wifi_set_gen_ie(wlan_idx,
					(char *)wpabuf_head(probe_resp_ie),
					wpabuf_len(probe_resp_ie),
					P2PWPS_PROBE_RSP_IE);
	wifi_set_gen_ie(wlan_idx, NULL, 0, P2PWPS_ASSOC_RSP_IE);

	wifi_p2p_set_state(P2P_STATE_LISTEN);

	p2p_freq_to_channel(freq,
						&reg_class,
						&channel);

	p2p_listen_cb(g_p2p_context.p2p, freq, duration);

	/* remain on channel */
	wifi_p2p_remain_on_channel(wlan_idx, channel, duration);

	return 0;
}

static void wifi_p2p_find_stopped(void *ctx)
{
	(void) ctx;

	if (g_p2p_context.state == P2P_STATE_SCAN) {
		wifi_scan_abort();
	}

	wifi_p2p_notify_find_stopped();
}

static void wifi_p2p_stop_listen(void *ctx)
{
	(void) ctx;

	u8 wlan_idx = (g_p2p_context.role == P2P_R_GO) ? SOFTAP_WLAN_INDEX : STA_WLAN_INDEX;

	/* cancel remain on channel */
	wifi_p2p_cancel_remain_on_ch();

	//p2p_listen_end(g_p2p_context.p2p, 0);

	wifi_set_gen_ie(wlan_idx, NULL, 0, P2PWPS_BEACON_IE);
	wifi_set_gen_ie(wlan_idx, NULL, 0, P2PWPS_PROBE_RSP_IE);
	wifi_set_gen_ie(wlan_idx, NULL, 0, P2PWPS_ASSOC_RSP_IE);

	os_cancel_timer(&g_p2p_context.long_listen_timer);
	g_p2p_context.p2p_long_listen = 0;
}

void wifi_p2p_dump_role(u8 role)
{
	switch (role) {
	case P2P_R_DISABLE:
		RTK_LOGA(NOTAG, "\r\nP2P Role: Disable");
		break;
	case P2P_R_DEVICE:
		RTK_LOGA(NOTAG, "\r\nP2P Role: Device");
		break;
	case P2P_R_CLIENT:
		RTK_LOGA(NOTAG, "\r\nP2P Role: Client");
		break;
	case P2P_R_GO:
		RTK_LOGA(NOTAG, "\r\nP2P Role: Group Owner");
		break;
	default:
		RTK_LOGA(NOTAG, "\r\nP2P Role(%d) not defined!", role);
		break;
	}
}

void wifi_p2p_set_state(u8 p2p_state)
{
	switch (p2p_state) {
	case P2P_STATE_IDLE:
		wpa_printf(MSG_INFO, "P2P: Set P2P_STATE_IDLE");
		break;
	case P2P_STATE_LISTEN:
		wpa_printf(MSG_DEBUG, "P2P: Set P2P_STATE_LISTEN");
		break;
	case P2P_STATE_SCAN:
		wpa_printf(MSG_DEBUG, "P2P: Set P2P_STATE_SCAN");
		break;
	case P2P_STATE_GONEGO_ING:
		wpa_printf(MSG_INFO, "P2P: Set P2P_STATE_GONEGO_ING");
		break;
	case P2P_STATE_PROVISIONING_ING:
		wpa_printf(MSG_INFO, "P2P: Set P2P_STATE_PROVISIONING_ING");
		break;
	case P2P_STATE_FORMATION_COMPLETE:
		wpa_printf(MSG_INFO, "P2P: Set P2P_STATE_FORMATION_COMPLETE");
		break;
	case P2P_STATE_CONNECTED:
		wpa_printf(MSG_INFO, "P2P: Set P2P_STATE_CONNECTED");
		break;
	case P2P_STATE_DISCONNECT:
		wpa_printf(MSG_INFO, "P2P: Set P2P_STATE_DISCONNECT");
		break;
	case P2P_STATE_FOUND:
		wpa_printf(MSG_INFO, "P2P: Set P2P_STATE_FOUND");
		break;
	case P2P_STATE_WSC_EXCHAGE_START:
		wpa_printf(MSG_INFO, "P2P: Set P2P_STATE_WSC_EXCHAGE_START");
		break;
	default:
		wpa_printf(MSG_INFO, "P2P: State(%d) not supported!", p2p_state);
		break;
	}

	g_p2p_context.state = p2p_state;
}

static void wifi_p2p_set_idle_state(void *ctx, u8 state)
{
	(void) ctx;

	if (state == P2P_IDLE) {
		wifi_p2p_set_state(P2P_STATE_IDLE);
	}
}

static void wifi_p2p_prov_disc_req(void *ctx, const u8 *peer, u16 config_methods,
								   const u8 *dev_addr, const u8 *pri_dev_type,
								   const char *dev_name, u16 supp_config_methods,
								   u8 dev_capab, u8 group_capab, const u8 *group_id,
								   size_t group_id_len)
{
	char devtype[WPS_DEV_TYPE_BUFSIZE];
	char params[300];
	u8 empty_dev_type[8];
	unsigned int generated_pin = 0;
	u8 device_pin[10];
	char *group = NULL;

	(void) ctx;
	(void) peer;
	(void) group_id;
	(void) group_id_len;

	if (g_p2p_context.role == P2P_R_DISABLE || g_p2p_context.p2p == NULL) {
		return;
	}

	if (g_p2p_context.role == P2P_R_GO) {
		group = "p2p0";
	}

	if (pri_dev_type == NULL) {
		os_memset(empty_dev_type, 0, sizeof(empty_dev_type));
		pri_dev_type = empty_dev_type;
	}
	os_snprintf(params, sizeof(params), " p2p_dev_addr=" MACSTR
				" pri_dev_type=%s name='%s' config_methods=0x%x "
				"dev_capab=0x%x group_capab=0x%x%s%s",
				MAC2STR(dev_addr),
				wps_dev_type_bin2str(pri_dev_type, devtype,
									 sizeof(devtype)),
				dev_name, supp_config_methods, dev_capab, group_capab,
				group ? " group=" : "",
				group ? group : "");
	params[sizeof(params) - 1] = '\0';

	if (config_methods & WPS_CONFIG_DISPLAY) {
		generated_pin = wps_generate_pin();
		os_sprintf(device_pin, "%08d", generated_pin);
		wpa_printf(MSG_INFO, "P2P-PROV-DISC-SHOW-PIN " MACSTR " %08d%s",
				   MAC2STR(peer), generated_pin, params);

	} else if (config_methods & WPS_CONFIG_KEYPAD)
		wpa_printf(MSG_INFO, "P2P-PROV-DISC-ENTER-PIN " MACSTR "%s",
				   MAC2STR(peer), params);
	else if (config_methods & WPS_CONFIG_PUSHBUTTON)
		wpa_printf(MSG_INFO, "P2P-PROV-DISC-PBC-REQ " MACSTR
				   "%s", MAC2STR(peer), params);
	if (g_p2p_context.role == P2P_R_GO) {
		wifi_start_ap_wps_thread(config_methods, (char *)device_pin);
	}
}


static void wifi_p2p_prov_disc_resp(void *ctx, const u8 *peer, u16 config_methods)
{
	unsigned int generated_pin = 0;
	char params[20] = {0};

	(void) ctx;
	(void) peer;
	(void) params;
	(void) generated_pin;

	if (g_p2p_context.role == P2P_R_DISABLE || g_p2p_context.p2p == NULL) {
		return;
	}

	if (config_methods & WPS_CONFIG_DISPLAY)
		wpa_printf(MSG_INFO, "P2P-PROV-DISC-ENTER-PIN " MACSTR "%s",
				   MAC2STR(peer), params);
	else if (config_methods & WPS_CONFIG_KEYPAD) {
		generated_pin = wps_generate_pin();
		wpa_printf(MSG_INFO, "P2P-PROV-DISC-SHOW-PIN " MACSTR " %08d%s",
				   MAC2STR(peer), generated_pin, params);
	} else if (config_methods & WPS_CONFIG_PUSHBUTTON)
		wpa_printf(MSG_INFO, "P2P-PROV-DISC-PBC-RESP " MACSTR
				   "%s", MAC2STR(peer), params);
}


static void wifi_p2p_prov_disc_fail(void *ctx, const u8 *peer,
									enum p2p_prov_disc_status status,
									u32 adv_id, const u8 *adv_mac,
									const char *deferred_session_resp)
{
	(void) ctx;
	(void) peer;
	(void) status;
	(void) adv_id;
	(void) adv_mac;
	(void) deferred_session_resp;
	wpa_printf(MSG_INFO, "P2P-PROV-DISC-FAILURE"
			   " p2p_dev_addr=" MACSTR " status=%d",
			   MAC2STR(peer), status);
}

static int wifi_p2p_start_go_neg(const u8 *peer_addr,
								 enum p2p_wps_method wps_method,
								 int go_intent, const u8 *own_interface_addr,
								 unsigned int force_freq, int persistent_group,
								 const u8 *ssid, size_t ssid_len, unsigned int pref_freq)
{
	if (persistent_group) {
		persistent_group = 2;
	}

	p2p_set_config_timeout(g_p2p_context.p2p, 100, 20);

	return p2p_connect(g_p2p_context.p2p, peer_addr, wps_method,
					   go_intent, own_interface_addr, force_freq,
					   persistent_group, ssid,
					   ssid_len,
					   g_p2p_context.p2p_pd_before_go_neg, pref_freq, 0);
}

static int wifi_p2p_auth_go_neg(const u8 *peer_addr,
								enum p2p_wps_method wps_method,
								int go_intent, const u8 *own_interface_addr,
								unsigned int force_freq, int persistent_group,
								const u8 *ssid, size_t ssid_len, unsigned int pref_freq)
{
	if (persistent_group) {
		persistent_group = 2;
	}

	return p2p_authorize(g_p2p_context.p2p, peer_addr, wps_method,
						 go_intent, own_interface_addr, force_freq,
						 persistent_group, ssid,
						 ssid_len, pref_freq, 0);
}

__weak u8 wifi_p2p_check_go(void)
{
	return g_p2p_context.role == P2P_R_GO;
}

static void wifi_p2p_go_neg_completed(void *ctx, struct p2p_go_neg_results *res)
{
	(void) ctx;

	/* cancel remain on channel */
	wifi_p2p_cancel_remain_on_ch();

	if (res->status) {
		wifi_p2p_notify_go_neg_result(0);
		wpa_printf(MSG_ALWAYS, "P2P-GO-NEG-FAILURE ""status=%d",
				   res->status);
		return;
	}

	wpa_printf(MSG_ALWAYS, "P2P-GO-NEG-SUCCESS ");
	wifi_p2p_notify_go_neg_result(1);

	if (res->role_go) {
		wpa_printf(MSG_INFO, "P2P: Start GO!");
		g_p2p_context.role = P2P_R_GO;
		wifi_p2p_set_role(P2P_R_GO);
	} else {
		wpa_printf(MSG_INFO, "P2P: Start Client!");
		g_p2p_context.role = P2P_R_CLIENT;
		wifi_p2p_set_role(P2P_R_CLIENT);
	}

	os_xqueue_send(g_p2p_context.queue_for_p2p_nego, (const void *)res, 0);

	os_cancel_timer(&g_p2p_context.group_formation_timer);
	os_set_timer(&g_p2p_context.group_formation_timer,
				 (30 + res->peer_config_timeout / 100) * 1000 + (res->peer_config_timeout % 100) * 10);

}

void wifi_p2p_go_neg_req_rx(void *ctx, const u8 *src, u16 dev_passwd_id,
							u8 go_intent)
{
	u8 channel;
	struct p2p_device *dev = NULL;

	(void) ctx;
	(void) go_intent;

	wifi_p2p_notify_go_neg_req_rx(src, dev_passwd_id);
	wpa_printf(MSG_INFO, "P2P: P2P link request from "MAC_FMT" with dev_passwd_id=%d go_intent=%u!\n", MAC_ARG(src), dev_passwd_id, go_intent);

	/* stop last find */
	if (g_p2p_context.p2p->state != P2P_IDLE) {
		p2p_stop_find(g_p2p_context.p2p);
	}

	channel = wifi_p2p_get_dev_chnl_from_scan_res((u8 *)src);
	if (channel == 0) {
		wpa_printf(MSG_ALWAYS, "P2P: Device "MAC_FMT" not found! Unkown listen frequency!\n", MAC_ARG(src));
		return;
	}
	dev = p2p_get_device(g_p2p_context.p2p, src);
	if (dev) {
		dev->listen_freq = p2p_channel_to_freq(g_p2p_context.p2p->cfg->reg_class, channel);
	}
	wpa_printf(MSG_INFO, "P2P: Listen channel of "MAC_FMT" is  %d (freq %d)!", MAC_ARG(src), channel, dev->listen_freq);
}

static void wifi_p2p_ie_update(void *ctx, struct wpabuf *beacon_ies,
							   struct wpabuf *proberesp_ies)
{
	(void) ctx;

	if (g_p2p_context.role == P2P_R_GO) {
		wpa_printf(MSG_INFO, "P2P: GO IE update!");
		if (beacon_ies) {
			wifi_set_gen_ie(SOFTAP_WLAN_INDEX, (char *)beacon_ies->buf, beacon_ies->used, P2PWPS_BEACON_IE);
		}
		if (proberesp_ies) {
			wifi_set_gen_ie(SOFTAP_WLAN_INDEX, (char *)proberesp_ies->buf, proberesp_ies->used, P2PWPS_PROBE_RSP_IE);
		}
	}
	wpabuf_free(beacon_ies);
	wpabuf_free(proberesp_ies);
}

static void wifi_p2p_idle_update(void *ctx, int idle)
{
	(void) ctx;

	if (g_p2p_context.role != P2P_R_GO) {
		return;
	}
	if (idle) {
		//TODO
	}
}

static int wifi_p2p_drv_deinit(void)
{
	struct rtw_wifi_setting setting = {0};

	if (g_p2p_context.role == P2P_R_CLIENT) {
		wifi_get_setting(STA_WLAN_INDEX, &setting);
		if (strlen((char *)setting.ssid) > 0) {
			wifi_disconnect();
		}
	}

	g_p2p_context.role = P2P_R_DEVICE;
	wifi_p2p_set_role(P2P_R_DEVICE);
	wifi_p2p_set_state(P2P_STATE_IDLE);

	return 0;
}

struct p2p_group *wifi_p2p_group_init(struct p2p_go_neg_results *params)
{
	struct p2p_group *group;
	struct p2p_group_config *cfg;

	cfg = (struct p2p_group_config *)os_zalloc(sizeof(*cfg));
	if (cfg == NULL) {
		return NULL;
	}

	if (params->persistent_group && g_p2p_context.persistent_reconnect) {
		cfg->persistent_group = 2;
	} else if (params->persistent_group) {
		cfg->persistent_group = 1;
	}
	os_memcpy(cfg->interface_addr, g_p2p_context.p2p->cfg->dev_addr, ETH_ALEN); //get from wehere?
	cfg->max_clients = g_p2p_context.max_num_sta;
	os_memcpy(cfg->ssid, params->ssid, params->ssid_len);
	cfg->ssid_len = params->ssid_len;
	//cfg->freq = params->freq;
	cfg->cb_ctx = &g_p2p_context;
	cfg->ie_update = wifi_p2p_ie_update;
	cfg->idle_update = wifi_p2p_idle_update;

	group = p2p_group_init(g_p2p_context.p2p, cfg);
	if (group == NULL) {
		os_free(cfg, 0);
	}

	return group;
}

static int wifi_p2p_group_delete(struct p2p_data *p2p,
								 enum p2p_group_removal_reason removal_reason)
{
	const char *reason;

	(void) p2p;

	switch (removal_reason) {
	case P2P_GROUP_REMOVAL_REQUESTED:
		reason = " reason=REQUESTED";
		break;
	case P2P_GROUP_REMOVAL_FORMATION_FAILED:
		reason = " reason=FORMATION_FAILED";
		break;
	case P2P_GROUP_REMOVAL_IDLE_TIMEOUT:
		reason = " reason=IDLE";
		break;
	case P2P_GROUP_REMOVAL_UNAVAILABLE:
		reason = " reason=UNAVAILABLE";
		break;
	case P2P_GROUP_REMOVAL_GO_ENDING_SESSION:
		reason = " reason=GO_ENDING_SESSION";
		break;
	default:
		reason = "";
		break;
	}
	wpa_printf(MSG_INFO, "P2P: Delete Group %s ", reason);
	wifi_p2p_notify_group_removed(g_p2p_context.role, reason);

	if (g_p2p_context.role == P2P_R_GO) {
		/*
		 * Deinit GO data on any possibly remaining interface (if main
		 * interface is used as GO).
		 */
		p2p_group_deinit(g_p2p_context.group);
		g_p2p_context.group = NULL;
	}

	return 0;
}

int wifi_p2p_group_notify_assoc(u8 *buf, u16 buf_len)
{
	struct wpabuf *p2p_wpa_buf;
	u8 frame_type, ie_offset;
	u8 *p2p_ie = NULL, *addr = NULL;
	u32 ie_len = 0;
	int ret = 0;

	if (g_p2p_context.role != P2P_R_GO || g_p2p_context.p2p == NULL) {
		return -1;
	}

	wpa_hexdump_key(MSG_DEBUG, "\r\nAssoc Req", buf, buf_len);
	frame_type = GetFrameSubType(buf);
	if (frame_type == RTW_ASSOCREQ) {
		ie_offset = _ASOCREQ_IE_OFFSET_ + WLAN_HDR_A3_LEN;
	} else {
		ie_offset = _REASOCREQ_IE_OFFSET_ + WLAN_HDR_A3_LEN;
	}

	addr = GetAddr2Ptr(buf);
	p2p_wpa_buf = ieee802_11_vendor_ie_concat(buf + ie_offset, buf_len - ie_offset,
				  P2P_IE_VENDOR_TYPE);
	if (p2p_wpa_buf) {
		p2p_ie = (u8 *)wpabuf_head_u8(p2p_wpa_buf);
		ie_len = wpabuf_len(p2p_wpa_buf);
		wpa_hexdump_key(MSG_DEBUG, "\r\nP2P IE", p2p_ie, ie_len);
		ret = p2p_group_notif_assoc(g_p2p_context.group, addr, p2p_ie, ie_len);
		wpabuf_free(p2p_wpa_buf);

		os_cancel_timer(&g_p2p_context.group_idle_timer);
		wifi_p2p_notify_ap_sta_connected(addr);
		wifi_p2p_set_state(P2P_STATE_CONNECTED);
	}
	return ret;
}

int wifi_p2p_group_notify_disassoc(u8 *addr)
{
	if (g_p2p_context.role != P2P_R_GO || g_p2p_context.p2p == NULL) {
		return -1;
	}

	if (g_p2p_context.state != P2P_STATE_CONNECTED) {
		return 0;
	}

	p2p_group_notif_disassoc(g_p2p_context.group, addr);
	wifi_p2p_notify_ap_sta_disconnected(addr);

	if (p2p_get_group_num_members(g_p2p_context.group) == 0) {
		os_set_timer(&g_p2p_context.group_idle_timer, 1);
	}
	return 0;
}

void wifi_p2p_dump_group_info(void)
{
	if (g_p2p_context.role > P2P_R_DEVICE) {
		RTK_LOGA(NOTAG, "\n==============================");
		RTK_LOGA(NOTAG, "\nGroup Info");
		RTK_LOGA(NOTAG, "\n==============================");
		RTK_LOGA(NOTAG, "\n       SSID => %s", g_p2p_context.group_ssid);
		RTK_LOGA(NOTAG, "\n    Channel => %d", g_p2p_context.oper_chnl);

		if (g_p2p_context.role == P2P_R_GO) {
			RTK_LOGA(NOTAG, "\n PASSPHRASE => %s", g_p2p_context.passphrase);
			p2p_group_info_dump(g_p2p_context.group);
		} else if (g_p2p_context.role == P2P_R_CLIENT) {
			RTK_LOGA(NOTAG, "\n         GO => "MAC_FMT, MAC_ARG(g_p2p_context.go_dev_addr));
		}
		RTK_LOGA(NOTAG, "\n==============================");
	}
}

static void wifi_p2p_group_formation_completed(int success)
{
	struct p2p_data *p2p = g_p2p_context.p2p;

	wpa_printf(MSG_DEBUG, "P2P-GROUP-FORMATION-COMPLETE ");

	if (!success) {
		wifi_p2p_notify_group_formation_result(0);
		//exit cmd_ap_wps or cmd_wps
		os_xqueue_send(queue_for_credential, NULL, 0);
		wifi_p2p_group_delete(p2p, P2P_GROUP_REMOVAL_FORMATION_FAILED);
		wifi_p2p_drv_deinit();
		wifi_p2p_set_state(P2P_STATE_IDLE);
		return;
	}

	wifi_p2p_notify_group_formation_result(1);

	if (g_p2p_context.role == P2P_R_GO) {
		p2p_group_notif_formation_done(g_p2p_context.group);
		//wpa_supplicant_ap_mac_addr_filter(wpa_s, NULL);
		wifi_p2p_notify_group_started(g_p2p_context.group_ssid,
									  g_p2p_context.oper_chnl,
									  g_p2p_context.passphrase,
									  g_p2p_context.go_dev_addr,
									  g_p2p_context.persistent);
	}
	wifi_p2p_set_state(P2P_STATE_FORMATION_COMPLETE);
}

int wifi_p2p_disconnect(struct p2p_data *p2p)
{
	int ret;
	if (p2p == NULL) {
		return -1;
	}

	ret = wifi_p2p_group_delete(p2p, P2P_GROUP_REMOVAL_REQUESTED);
	wifi_p2p_drv_deinit();
	return  ret < 0 ? -1 : 0;
}

static int wifi_p2p_go_connected(void *ctx, const u8 *dev_addr)
{
	struct p2p_context *p2p_cont = (struct p2p_context *)ctx;

	if ((p2p_cont->role == P2P_R_CLIENT)
		&& (os_memcmp_p2p(p2p_cont->go_dev_addr, dev_addr, ETH_ALEN) == 0)) {
		wpa_printf(MSG_DEBUG, "P2P: The device is my connected GO.");
		return 1;
	}
	return 0;
}

static void wifi_p2p_group_formation_failed(void *ctx)
{
	struct p2p_data *p2p = ctx;
	wpa_printf(MSG_DEBUG, "P2P: Group Formation failed");
	os_cancel_timer(&g_p2p_context.group_formation_timer);
	if (p2p) {
		p2p_group_formation_failed(p2p);
	}
	wifi_p2p_group_formation_completed(0);
}

void _wifi_p2p_wps_success(const u8 *peer_addr, int registrar)
{
	(void) registrar;

	if (g_p2p_context.role == P2P_R_DISABLE || g_p2p_context.p2p == NULL) {
		return;
	}

	if (g_p2p_context.p2p->state != P2P_PROVISIONING) {
		wpa_printf(MSG_INFO, "P2P: Ignore WPS fail event - P2P "
				   "provisioning not in progress");
		return;
	}

	if (g_p2p_context.role == P2P_R_CLIENT)
		wpa_printf(MSG_ALWAYS, "P2P: WPS Succeeded - "
				   "Work as P2P Client. GO is "MAC_FMT".", MAC_ARG(peer_addr));
	else
		wpa_printf(MSG_ALWAYS, "P2P: WPS Succeeded - "
				   "Work as P2P GO. Client is "MAC_FMT".", MAC_ARG(peer_addr));

	if (g_p2p_context.role == P2P_R_CLIENT) {
		u8 go_dev_addr[ETH_ALEN];
		os_memcpy(go_dev_addr, peer_addr, ETH_ALEN);
		/* Clear any stored provisioning info */
		p2p_clear_provisioning_info(g_p2p_context.p2p, go_dev_addr);
	}

	os_cancel_timer(&g_p2p_context.group_formation_timer);

	if (g_p2p_context.role == P2P_R_CLIENT) {
		/*
		 * Use a separate timeout for initial data connection to
		 * complete to allow the group to be removed automatically if
		 * something goes wrong in this step before the P2P group idle
		 * timeout mechanism is taken into use.
		 */
		//os_set_timer(&g_p2p_context.group_formation_timer, P2P_MAX_INITIAL_CONN_WAIT*1000);
	}
	if (g_p2p_context.p2p) {
		p2p_wps_success_cb(g_p2p_context.p2p, peer_addr);
	}
	wifi_p2p_group_formation_completed(1);
}


void _wifi_p2p_wps_failed(void)
{
	if (g_p2p_context.role == P2P_R_DISABLE || g_p2p_context.p2p == NULL) {
		return;
	}

	if (g_p2p_context.p2p->state != P2P_PROVISIONING) {
		wpa_printf(MSG_DEBUG, "P2P: Ignore WPS fail event - P2P "
				   "provisioning not in progress");
		return;
	}

	wpa_printf(MSG_ALWAYS, "P2P: WPS Failed");

#if 0//TODO auto GO
	if (g_p2p_context.go_params) {
		p2p_clear_provisioning_info(
			g_p2p_context.p2p,
			g_p2p_context.go_params->peer_device_addr);
	}
#endif
}

static void wifi_p2p_group_formation_timeout(void *timeout_ctx)
{
	struct p2p_data *p2p = timeout_ctx;
	wpa_printf(MSG_INFO, "P2P: Group Formation timed out");
	if (p2p) {
		p2p_group_formation_failed(p2p);
	}
	wifi_p2p_group_formation_completed(0);
}

static void wifi_p2p_group_idle_timeout(void *timeout_ctx)
{
	struct p2p_data *p2p = timeout_ctx;

	if (g_p2p_context.role != P2P_R_CLIENT) {
		wpa_printf(MSG_DEBUG, "P2P: Ignore group idle timeout - "
				   "disabled");
		return;
	}

	wpa_printf(MSG_INFO, "P2P: Group idle timeout reached - terminate "
			   "group");
	wifi_p2p_group_delete(p2p, P2P_GROUP_REMOVAL_IDLE_TIMEOUT);
	wifi_p2p_drv_deinit();
}

static void wifi_p2p_long_listen_timeout(void *timeout_ctx)
{
	(void) timeout_ctx;
	g_p2p_context.p2p_long_listen = 0;
	wifi_p2p_set_state(P2P_STATE_IDLE);
}

int wifi_p2p_start_go(char *ssid, char *passphrase, u8 channel)
{
	extern struct netif xnetif[NET_IF_NUM];
	struct netif *pnetif = &xnetif[NETIF_WLAN_AP_INDEX];
	struct rtw_softap_info softAP_config = {0};
	u8 bk_addr[ETH_ALEN];
	u32 addr = CONCAT_TO_UINT32(AP_IP_ADDR0, AP_IP_ADDR1, AP_IP_ADDR2, AP_IP_ADDR3);
	u32 netmask = CONCAT_TO_UINT32(AP_NETMASK_ADDR0, AP_NETMASK_ADDR1, AP_NETMASK_ADDR2, AP_NETMASK_ADDR3);
	u32 gw = CONCAT_TO_UINT32(AP_GW_ADDR0, AP_GW_ADDR1, AP_GW_ADDR2, AP_GW_ADDR3);

	/* Stop AP */
	dhcps_deinit();
	wifi_stop_ap();

	memcpy(softAP_config.ssid.val, ssid, strlen(ssid));
	softAP_config.ssid.len = strlen(ssid);
	softAP_config.password = (u8 *)passphrase;
	softAP_config.password_len = strlen(passphrase);
	softAP_config.security_type = RTW_SECURITY_WPA2_AES_PSK;
	softAP_config.channel = channel;

	/* Init go, because switch port0&port1 mac addr need to init mac addr firstly */
	wifi_init_ap();

	/* Switch port0 & port1 mac addr */
	memcpy(bk_addr, xnetif[STA_WLAN_INDEX].hwaddr, ETH_ALEN);
	wifi_set_mac_address(0, xnetif[SOFTAP_WLAN_INDEX].hwaddr, 0);
	wifi_set_mac_address(1, bk_addr, 0);

	/* Start GO */
	if (wifi_start_ap(&softAP_config) != RTK_SUCCESS) {
		RTK_LOGW(NOTAG, "[+WLP2PGO] wifi_start_ap failed\r\n");
		return -1;
	}

	LwIP_SetIP(NETIF_WLAN_AP_INDEX, addr, netmask, gw);

	/* start dhcp server */
	dhcps_init(pnetif);

	return 0;
}

static int wifi_p2p_start_wps_go(struct p2p_go_neg_results *params,
								 int group_formation)
{
	u8 channel, reg_class;
	u16 wps_config = 0;

	(void) group_formation;

	p2p_freq_to_channel(params->freq,
						&reg_class,
						&channel);

	wifi_p2p_start_go((char *)params->ssid, params->passphrase, channel);

	g_p2p_context.oper_chnl = channel;
	os_memcpy(g_p2p_context.group_ssid, params->ssid, strlen((char *)params->ssid));
	g_p2p_context.group_ssid[strlen((char *)params->ssid)] = '\0';
	os_memcpy(g_p2p_context.go_dev_addr, g_p2p_context.p2p->cfg->dev_addr, ETH_ALEN);
	os_memcpy(g_p2p_context.passphrase, params->passphrase, strlen(params->passphrase));
	g_p2p_context.passphrase[strlen(params->passphrase)] = '\0';
	g_p2p_context.persistent = params->persistent_group;
	g_p2p_context.group = wifi_p2p_group_init(params);

	wpa_printf(MSG_DEBUG, "P2P: params->wps_method=%d ", params->wps_method);
	if (params->wps_method == WPS_PBC) {
		wps_config = WPS_CONFIG_PUSHBUTTON;
	} else	if (params->wps_method == WPS_PIN_KEYPAD) {
		wps_config = WPS_CONFIG_KEYPAD;
	} else	if (params->wps_method == WPS_PIN_DISPLAY) {
		wps_config = WPS_CONFIG_DISPLAY;
	}

	wifi_start_ap_wps_thread(wps_config, g_p2p_context.p2p_pin);

	return 0;
}

static int wifi_p2p_start_wps_enrollee(struct p2p_go_neg_results *res)
{
	u8 channel, reg_class;
	int ret = 0;
	u8 *ies;
	u16 ie_len = 256;
	u16 wps_config = 0;

	wpa_printf(MSG_INFO, "P2P: start wps enrollee");
	p2p_freq_to_channel(res->freq,
						&reg_class,
						&channel);

	os_memcpy(g_p2p_context.go_dev_addr, res->peer_device_addr, ETH_ALEN);
	g_p2p_context.oper_chnl = channel;
	os_memcpy(g_p2p_context.group_ssid, res->ssid, res->ssid_len);
	g_p2p_context.group_ssid[res->ssid_len] = '\0';
	strcpy(g_p2p_context.passphrase, res->passphrase);
	g_p2p_context.persistent = res->persistent_group;

	wpa_printf(MSG_DEBUG, "\n\rRes->ssid=%s ssid_len=%d strlen(ssid)=%d\n", res->ssid, res->ssid_len, strlen((char *)res->ssid));

	struct p2p_scan_report_node *dev_node = wifi_p2p_get_dev_scan_res(res->peer_interface_addr);
	if (!dev_node) {
		ret = -1;
		goto exit;
	}

	ies = rtos_mem_zmalloc(ie_len);
	if (!ies) {
		ret = -2;
		goto exit;
	}
	ie_len = wifi_p2p_set_assoc_req_ie(dev_node->p_scanned_AP_info->bssid.octet, ies, ie_len);
	wifi_set_gen_ie(STA_WLAN_INDEX, (char *)ies, ie_len, P2PWPS_ASSOC_REQ_IE);

	if (res->wps_method == WPS_PBC) {
		wps_config = WPS_CONFIG_PUSHBUTTON;
	} else if (res->wps_method == WPS_PIN_KEYPAD) {
		wps_config = WPS_CONFIG_KEYPAD;
	} else if (res->wps_method == WPS_PIN_DISPLAY) {
		wps_config = WPS_CONFIG_DISPLAY;
	}

	wps_start(wps_config, g_p2p_context.p2p_pin, channel, (char *)res->ssid);

exit:
	if (ies) {
		rtos_mem_free(ies);
	}
	if (ret != 0) {
		wpa_printf(MSG_INFO, "P2P: Group formation failed! "MAC_FMT"\n", MAC_ARG(res->peer_interface_addr));
		wifi_p2p_group_formation_failed(g_p2p_context.p2p);
	}
	return ret;
}

int wifi_p2p_start_wps(void *res)
{
	int ret = -1;

	if (g_p2p_context.role == P2P_R_DISABLE || g_p2p_context.p2p == NULL) {
		return -1;
	}

	if (g_p2p_context.role == P2P_R_GO) {
		wifi_p2p_set_state(P2P_STATE_PROVISIONING_ING);
		ret = wifi_p2p_start_wps_go((struct p2p_go_neg_results *)res, 1);
	} else if (g_p2p_context.role == P2P_R_CLIENT) {
		wifi_p2p_set_state(P2P_STATE_PROVISIONING_ING);
		ret = wifi_p2p_start_wps_enrollee((struct p2p_go_neg_results *)res);
	} else {
		wpa_printf(MSG_INFO, "P2P: GO Negotiation Failed!");
		p2p_flush(g_p2p_context.p2p);
		wifi_p2p_set_state(P2P_STATE_IDLE);
	}
	return ret;
}

int wifi_p2p_join(const u8 *iface_addr,
				  const u8 *dev_addr, enum p2p_wps_method wps_method,
				  int auto_join)
{
	(void) iface_addr;
	(void) dev_addr;
	(void) wps_method;
	(void) auto_join;

	return 0;
}
/**
 * wpas_p2p_connect - Request P2P Group Formation to be started
 * @wpa_s: Pointer to wpa_supplicant data from wpa_supplicant_add_iface()
 * @peer_addr: Address of the peer P2P Device
 * @pin: PIN to use during provisioning or %NULL to indicate PBC mode
 * @persistent_group: Whether to create a persistent group
 * @auto_join: Whether to select join vs. GO Negotiation automatically
 * @join: Whether to join an existing group (as a client) instead of starting
 *	Group Owner negotiation; @peer_addr is BSSID in that case
 * @auth: Whether to only authorize the connection instead of doing that and
 *	initiating Group Owner negotiation
 * @go_intent: GO Intent or -1 to use default
 * @freq: Frequency for the group or 0 for auto-selection
 * @persistent_id: Persistent group credentials to use for forcing GO
 *	parameters or -1 to generate new values (SSID/passphrase)
 * @pd: Whether to send Provision Discovery prior to GO Negotiation as an
 *	interoperability workaround when initiating group formation
 * Returns: 0 or new PIN (if pin was %NULL) on success, -1 on unspecified
 *	failure, -2 on failure due to channel not currently available,
 *	-3 if forced channel is not supported
 */
int wifi_p2p_connect(const u8 *peer_addr,
					 const char *pin, enum p2p_wps_method wps_method,
					 int persistent_group, int auto_join, int join, int auth,
					 int go_intent, int freq, int persistent_id, int pd)
{
	int force_freq = 0, pref_freq = 0;
	int ret = 0;
	unsigned int val;
	struct p2p_data *p2p = g_p2p_context.p2p;

	(void) persistent_id;
	(void) pd;

	if (g_p2p_context.role == P2P_R_DISABLE || g_p2p_context.p2p == NULL) {
		return -1;
	}

	if (!auth) {
		g_p2p_context.p2p_long_listen = 0;
	}

	if (pin) {
		wpa_printf(MSG_INFO, "P2P: get PIN: %s", pin);
		os_strncpy(g_p2p_context.p2p_pin, pin, sizeof(g_p2p_context.p2p_pin));
	} else if (wps_method == WPS_PIN_DISPLAY) {
		val = wps_generate_pin();
		os_snprintf(g_p2p_context.p2p_pin, sizeof(g_p2p_context.p2p_pin), "%08d",
					val);
		wpa_printf(MSG_ALWAYS, "P2P: Randomly generated PIN: %s",
				   g_p2p_context.p2p_pin);
	} else {
		g_p2p_context.p2p_pin[0] = '\0';
	}

	if (join || auto_join) {
		u8 iface_addr[ETH_ALEN], dev_addr[ETH_ALEN];
		os_memcpy(dev_addr, peer_addr, ETH_ALEN);
		if (p2p_get_interface_addr(p2p, peer_addr,
								   iface_addr) < 0) {
			os_memcpy(iface_addr, peer_addr, ETH_ALEN);
			p2p_get_dev_addr(p2p, peer_addr,
							 dev_addr);
		}
		if (wifi_p2p_join(iface_addr, dev_addr, wps_method,
						  auto_join) < 0) {
			return -1;
		}
		return ret;
	}

	if (freq > 0) {
		if (!p2p_supported_freq(p2p, freq)) {
			wpa_printf(MSG_DEBUG, "P2P: The forced channel "
					   "(%u MHz) is not supported for P2P uses",
					   freq);
			return -3;
		}
		wpa_printf(MSG_DEBUG, "P2P: Trying to force us to use the "
				   "requested channel (%u MHz)", freq);
		force_freq = freq;
	}

	wifi_p2p_set_state(P2P_STATE_LISTEN);
	if (auth) {
		if (wifi_p2p_auth_go_neg(peer_addr, wps_method,
								 go_intent, p2p->cfg->dev_addr,
								 force_freq, persistent_group, NULL, 0,
								 pref_freq) < 0) {
			return -1;
		}
		return ret;
	}

	if (wifi_p2p_start_go_neg(peer_addr, wps_method,
							  go_intent, p2p->cfg->dev_addr, force_freq,
							  persistent_group, NULL, 0, pref_freq) < 0) {
		return -1;
	}
	return ret;
}

int wifi_cmd_p2p_listen(unsigned int timeout)
{
	int res;

	if (g_p2p_context.role == P2P_R_DISABLE || g_p2p_context.p2p == NULL) {
		return -1;
	}

	if (timeout == 0) {
		/*
		 * This is a request for unlimited Listen state. However, at
		 * least for now, this is mapped to a Listen state for one
		 * hour.
		 */
		timeout = 3600;  /* unit: second */
	}
	os_cancel_timer(&g_p2p_context.long_listen_timer);
	g_p2p_context.p2p_long_listen = 0;

	/*
	 * Stop previous find/listen operation to avoid trying to request a new
	 * remain-on-channel operation while the driver is still running the
	 * previous one.
	 */
	p2p_stop_find(g_p2p_context.p2p);

	res = p2p_listen(g_p2p_context.p2p, timeout * 1000);
	if (res == 0 && timeout * 1000 > 1000) {
		g_p2p_context.p2p_long_listen = timeout * 1000;
		os_set_timer(&g_p2p_context.long_listen_timer, timeout * 1000);
	}

	return res;
}

void wifi_cmd_p2p_find(u32 timeout)
{
	struct list_head *plist, *phead;
	struct p2p_scan_report_node *p_node;

	if (g_p2p_context.role != P2P_R_DEVICE || g_p2p_context.p2p == NULL) {
		return;
	}

	g_p2p_context.p2p_long_listen = 0;

	/* clear scan report list */
	rtos_mutex_take(g_p2p_context.scan_report_lock, 0xFFFFFFFF);
	while (list_empty(&g_p2p_context.p2p_scan_report_list) != true) {
		phead = &g_p2p_context.p2p_scan_report_list;
		plist = phead->next;
		p_node = list_entry(plist, struct p2p_scan_report_node, list);
		list_del(&(p_node->list));

		if (p_node->p_scanned_AP_info) {
			rtos_mem_free(p_node->p_scanned_AP_info);
		}

		if (p_node->ies) {
			rtos_mem_free(p_node->ies);
		}
		rtos_mem_free(p_node);
	}
	rtos_mutex_give(g_p2p_context.scan_report_lock);

	p2p_find(g_p2p_context.p2p, timeout,  P2P_FIND_START_WITH_FULL, 0, NULL, NULL, 0, 0, NULL, 0, false);
}

void wifi_cmd_p2p_peers(void)
{
	struct p2p_scan_report_node *item;
	struct list_head *iterator;
	struct rtw_scan_result *p_scanned_AP_info;
	u8 count = 0;

	RTK_LOGA(NOTAG, "Found p2p peers:\n");

	rtos_mutex_take(g_p2p_context.scan_report_lock, 0xFFFFFFFF);
	list_for_each(iterator, &g_p2p_context.p2p_scan_report_list) {
		item = list_entry(iterator, struct p2p_scan_report_node, list);
		p_scanned_AP_info = item->p_scanned_AP_info;

		RTK_LOGA(NOTAG, "[%d]:"MAC_FMT", ", count++, MAC_ARG(p_scanned_AP_info->bssid.octet));
		if (item->role == P2P_R_DEVICE) {
			RTK_LOGA(NOTAG, "%s, listen_ch %d\n", "P2P_DEV", p_scanned_AP_info->channel);
		} else {
			RTK_LOGA(NOTAG, "%s, %s, oper_ch %d, %d\n", "P2P_GO", p_scanned_AP_info->ssid.val, p_scanned_AP_info->channel, p_scanned_AP_info->signal_strength);
		}
	}
	rtos_mutex_give(g_p2p_context.scan_report_lock);
}

void wifi_cmd_p2p_state(void)
{
	if (g_p2p_context.role == P2P_R_DISABLE || g_p2p_context.p2p == NULL) {
		return;
	}

	wifi_p2p_dump_role(g_p2p_context.role);
	wifi_p2p_dump_group_info();
}

void wifi_cmd_p2p_disconnect(void)
{
	wifi_p2p_disconnect(g_p2p_context.p2p);
}

int wifi_cmd_p2p_connect(u8 *dest, enum p2p_wps_method config_method, char *pin, int go_intent, u32 timeout_sec)
{
	struct p2p_scan_report_node *dev_node = NULL;
	struct p2p_data *p2p = g_p2p_context.p2p;
	struct p2p_device *dev = NULL;
	int ret = 0;
	int count = 1000;
	int freq;
	u8 *nego_res = NULL;

	if (g_p2p_context.role != P2P_R_DEVICE || g_p2p_context.p2p == NULL) {
		return -1;
	}

	//stop last find
	if (p2p->state != P2P_IDLE) {
		p2p_stop_find(p2p);
	}

	//wait scan done
	while (p2p->p2p_scan_running && (count-- > 0)) {
		wpa_printf(MSG_DEBUG, "P2P: p2p_scan is running, "
				   "wait p2p scan done!");

		os_sleep(0, 1000);
	}

	dev = p2p_get_device(p2p, dest);
	if (dev && ((dev->flags & P2P_DEV_PROBE_REQ_ONLY) != P2P_DEV_PROBE_REQ_ONLY)) {
		wpa_printf(MSG_INFO, "P2P: Device "MACSTR" Found. Do connect!",
				   MAC2STR(dest));
		goto Connect;
	}

	dev_node = wifi_p2p_get_dev_scan_res(dest);
	if (dev_node == NULL) {
		wpa_printf(MSG_INFO, "Can't find device "MAC_FMT"!", MAC_ARG(dest));
		goto Exit;
	}

	freq = p2p_channel_to_freq(p2p->cfg->reg_class, dev_node->p_scanned_AP_info->channel);
	ret = p2p_add_device(p2p, dev_node->p_scanned_AP_info->bssid.octet,
						 freq, NULL,
						 dev_node->p_scanned_AP_info->signal_strength,
						 dev_node->ies + _FIXED_IE_LENGTH_,
						 dev_node->ie_len - _FIXED_IE_LENGTH_,
						 1) ;

	if (ret < 0) {
		wpa_printf(MSG_INFO, "Add device "MAC_FMT" failed!", MAC_ARG(dest));
		goto Exit;
	}

Connect:
	nego_res = rtos_mem_zmalloc(sizeof(struct p2p_go_neg_results));
	if (!nego_res) {
		goto Exit;
	}
	g_p2p_context.queue_for_p2p_nego = os_xqueue_create(1, sizeof(struct p2p_go_neg_results));

	//start p2p connect
	ret = wifi_p2p_connect(dest,
						   pin, config_method,
						   0, 0, 0, 0,
						   go_intent, 0, 0,
						   g_p2p_context.p2p_pd_before_go_neg);
	if (ret == 0) {
		if (os_xqueue_receive(g_p2p_context.queue_for_p2p_nego, nego_res, timeout_sec) == RTK_SUCCESS) {
			wifi_p2p_start_wps(nego_res);
		} else {
			wpa_printf(MSG_ERROR, "P2P: wait for nego result timeout!!!");
		}
	} else {
		wpa_printf(MSG_ERROR, "P2P: P2P connect fail, ret=%d!!!", ret);
	}

Exit:
	if (g_p2p_context.queue_for_p2p_nego != NULL) {
		os_xqueue_delete(g_p2p_context.queue_for_p2p_nego);
		g_p2p_context.queue_for_p2p_nego = NULL;
	}

	if (nego_res) {
		rtos_mem_free(nego_res);
	}

	return ret;
}

void wifi_p2p_join_status_hdl(u8 *evt_info)
{
	struct rtw_event_join_status_info *join_status_info = (struct rtw_event_join_status_info *)evt_info;
	u8 join_status = join_status_info->status;

	if (join_status == RTW_JOINSTATUS_SUCCESS) {
		if (g_p2p_context.role != P2P_R_CLIENT || g_p2p_context.p2p == NULL) {
			return;
		}
		wpa_printf(MSG_INFO, "P2P: Connected event handler. state=%d", g_p2p_context.state);
		if (g_p2p_context.state == P2P_STATE_FORMATION_COMPLETE) {
			wifi_p2p_set_state(P2P_STATE_CONNECTED);
			wifi_p2p_notify_group_started(g_p2p_context.group_ssid,
										  g_p2p_context.oper_chnl,
										  g_p2p_context.passphrase,
										  g_p2p_context.go_dev_addr,
										  g_p2p_context.persistent);
		}
		return;
	}

	if (join_status == RTW_JOINSTATUS_FAIL || join_status == RTW_JOINSTATUS_DISCONNECT) {
		if (g_p2p_context.role != P2P_R_CLIENT || g_p2p_context.p2p == NULL) {
			return;
		}

		wpa_printf(MSG_INFO, "P2P: Disconnect event handler. state=%d", g_p2p_context.state);
		if (g_p2p_context.state == P2P_STATE_CONNECTED) {
			//wifi_p2p_set_state(P2P_STATE_CONNECTED);
			wifi_p2p_group_delete(g_p2p_context.p2p, P2P_GROUP_REMOVAL_GO_ENDING_SESSION);
		}

		if (!get_wps_phase()) {
			/* avoid to reset p2p_role and p2p_state by disconnect during wps */
			wifi_p2p_drv_deinit();
		}
	}
}

int wifi_p2p_init(u8 *dev_addr, u8 listen_ch, u8 op_ch)
{
	struct p2p_config *p2pcfg;
	struct wps_device_data *wpsdev;
	int ret = 0;

	p2pcfg = rtos_mem_zmalloc(sizeof(struct p2p_config));
	if (!p2pcfg) {
		return -1;
	}

	p2pcfg->cb_ctx = &g_p2p_context;
	p2pcfg->p2p_scan = wifi_p2p_scan;
	p2pcfg->send_action = wifi_p2p_send_action;
	p2pcfg->send_action_done = wifi_p2p_send_action_done;
	p2pcfg->go_neg_completed = wifi_p2p_go_neg_completed;
	p2pcfg->go_neg_req_rx = wifi_p2p_go_neg_req_rx;
	p2pcfg->dev_found = wifi_p2p_dev_found;
	p2pcfg->dev_lost = wifi_p2p_dev_lost;
	p2pcfg->start_listen = wifi_p2p_start_listen;
	p2pcfg->stop_listen = wifi_p2p_stop_listen;
	p2pcfg->find_stopped = wifi_p2p_find_stopped;
	p2pcfg->send_probe_resp = wifi_p2p_send_probe_resp;
	p2pcfg->prov_disc_req = wifi_p2p_prov_disc_req;
	p2pcfg->prov_disc_resp = wifi_p2p_prov_disc_resp;
	p2pcfg->prov_disc_fail = wifi_p2p_prov_disc_fail;
	p2pcfg->go_connected = wifi_p2p_go_connected;
	p2pcfg->set_idle_state = wifi_p2p_set_idle_state;

	wpas_wps_dev_config(dev_addr, 1);
	if (NULL == g_wps_context.wps) {
		wpa_printf(MSG_ERROR, "P2P: wps device initialize failed!");
		ret = -1;
		goto exit;
	}

	wpsdev = &(g_wps_context.wps->dev);
	wpa_printf(MSG_DEBUG, "P2P: wifi_p2p_init(): wps=%p, dev=%p, num_sec_dev_types=%d",
			   g_wps_context.wps, wpsdev, wpsdev->num_sec_dev_types);

	os_memcpy(p2pcfg->dev_addr, dev_addr, ETH_ALEN);
	p2pcfg->dev_name = wpsdev->device_name;
	p2pcfg->manufacturer = wpsdev->manufacturer;
	p2pcfg->model_name = wpsdev->model_name;
	p2pcfg->model_number = wpsdev->model_number;
	p2pcfg->serial_number = wpsdev->serial_number;
	os_memcpy(p2pcfg->uuid, g_wps_context.wps->uuid, 16);
	p2pcfg->config_methods = WPS_CONFIG_PUSHBUTTON | WPS_CONFIG_DISPLAY;

	if (listen_ch >= 149) {
		p2pcfg->reg_class = 124;
	} else if (listen_ch >= 36) {
		p2pcfg->reg_class = 115;
	} else {
		p2pcfg->reg_class = 81;
	}
	p2pcfg->channel = listen_ch;
	p2pcfg->channel_forced = 1;
	wpa_printf(MSG_DEBUG, "P2P: Own listen channel: %d", p2pcfg->channel);

	if (op_ch >= 149) {
		p2pcfg->op_reg_class = 124;
	} else if (op_ch >= 36) {
		p2pcfg->op_reg_class = 115;
	} else {
		p2pcfg->op_reg_class = 81;
	}
	p2pcfg->op_channel = op_ch;
	p2pcfg->cfg_op_channel = 1;
	wpa_printf(MSG_DEBUG, "P2P: operating channel: %d:%d", p2pcfg->op_reg_class, p2pcfg->op_channel);

	os_memcpy(p2pcfg->country, "XX\x04", 3);

	if (wifi_p2p_default_channels(&p2pcfg->channels)) {
		wpa_printf(MSG_ERROR, "P2P: Failed to configure supported "
				   "channel list");
		ret = -1;
		goto exit;
	}

	os_memcpy(p2pcfg->pri_dev_type, wpsdev->pri_dev_type,
			  WPS_DEV_TYPE_LEN);
	p2pcfg->num_sec_dev_types = wpsdev->num_sec_dev_types;
	if (wpsdev->num_sec_dev_types)
		os_memcpy(p2pcfg->sec_dev_type, wpsdev->sec_dev_type,
				  wpsdev->num_sec_dev_types * WPS_DEV_TYPE_LEN);

	p2pcfg->concurrent_operations = 0;
	p2pcfg->max_peers = 100;
	p2pcfg->ssid_postfix_len = os_strlen("-REALTEKDEV");
	os_memcpy(p2pcfg->ssid_postfix, "-REALTEKDEV", p2pcfg->ssid_postfix_len);

	p2pcfg->p2p_intra_bss = 0;
	p2pcfg->max_listen = 200;  /* keep in listen channel 200ms */
	p2pcfg->passphrase_len = 8;

	g_p2p_context.p2p = p2p_init(p2pcfg);
	if (g_p2p_context.p2p == NULL) {
		ret = -1;
		goto exit;
	}

	g_p2p_context.p2p_pd_before_go_neg = 0;

	g_p2p_context.role = P2P_R_DEVICE;
	wifi_p2p_set_role(P2P_R_DEVICE);

	os_init_timer(&g_p2p_context.group_formation_timer,
				  g_p2p_context.p2p,
				  wifi_p2p_group_formation_timeout,
				  g_p2p_context.p2p,
				  "group_formation_timer");
	os_init_timer(&g_p2p_context.long_listen_timer,
				  g_p2p_context.p2p,
				  wifi_p2p_long_listen_timeout,
				  g_p2p_context.p2p,
				  "long_listen_timer");
	os_init_timer(&g_p2p_context.group_idle_timer,
				  g_p2p_context.p2p,
				  wifi_p2p_group_idle_timeout,
				  g_p2p_context.p2p,
				  "group_idle_timer");

	rtos_sema_create_static(&g_p2p_context.roc_ready_sema, 0, 0xFFFFFFFF);
	rtos_mutex_create_static(&g_p2p_context.scan_report_lock);
	INIT_LIST_HEAD(&g_p2p_context.p2p_scan_report_list);

	INIT_LIST_HEAD(&g_p2p_cmdpriv.queue);
	rtos_mutex_create_static(&g_p2p_cmdpriv.lock);
	rtos_sema_create_static(&g_p2p_cmdpriv.wakeup_sema, 0, 0xFFFFFFFF);

	rtos_task_create(NULL, "p2p_cmd_task", wifi_p2p_cmd_process_task, NULL, 1024 * 8, 2);

exit:
	if (p2pcfg) {
		rtos_mem_free(p2pcfg);
	}
	return ret;
}


/**
 * wpas_p2p_deinit - Deinitialize per-interface P2P data
 * @wpa_s: Pointer to wpa_supplicant data from wpa_supplicant_add_iface()
 *
 * This function deinitialize per-interface P2P data.
 */
void wifi_p2p_deinit(void)
{
	if (g_p2p_context.role == P2P_R_DISABLE || g_p2p_context.p2p == NULL) {
		return;
	}
#if 0 //TODO auto GO
	if (g_p2p_context.go_params) {
		/* Clear any stored provisioning info */
		p2p_clear_provisioning_info(
			g_p2p_context.p2p,
			g_p2p_context.go_params);
	}

	os_free(g_p2p_context.go_params, 0);
	g_p2p_context.go_params = NULL;

#endif

	os_del_timer(&g_p2p_context.group_formation_timer);
	os_del_timer(&g_p2p_context.long_listen_timer);
	os_del_timer(&g_p2p_context.group_idle_timer);

	/* Disconnect from the P2P group and deinit the interface */
	wifi_p2p_disconnect(g_p2p_context.p2p);

	p2p_deinit(g_p2p_context.p2p);
	g_p2p_context.p2p = NULL;

	g_p2p_context.role = P2P_R_DISABLE;
	wifi_p2p_set_role(P2P_R_DISABLE);

	wpas_wps_deinit();

	wpa_printf(MSG_INFO, "P2P: P2p mode stopped!");
}

void wifi_p2p_set_dev_name(const char *dev_name)
{
	struct p2p_data *p2p = g_p2p_context.p2p;
	int len = os_strlen(dev_name);
	os_memset(p2p->cfg->dev_name, 0, 33);
	os_memcpy(p2p->cfg->dev_name, dev_name, len > 32 ? 32 : len);
}

void wifi_p2p_set_manufacturer(const char *manufacturer)
{
	struct p2p_data *p2p = g_p2p_context.p2p;
	int len = os_strlen(manufacturer);
	os_memset(p2p->cfg->manufacturer, 0, 65);
	os_memcpy(p2p->cfg->manufacturer, manufacturer, len > 64 ? 64 : len);
}

void wifi_p2p_set_model_name(const char *model_name)
{
	struct p2p_data *p2p = g_p2p_context.p2p;
	int len = os_strlen(model_name);
	os_memset(p2p->cfg->model_name, 0, 33);
	os_memcpy(p2p->cfg->model_name, model_name, len > 32 ? 32 : len);
}

void wifi_p2p_set_model_number(const char *model_number)
{
	struct p2p_data *p2p = g_p2p_context.p2p;
	int len = os_strlen(model_number);
	os_memset(p2p->cfg->model_number, 0, 33);
	os_memcpy(p2p->cfg->model_number, model_number, len > 32 ? 32 : len);
}

void wifi_p2p_set_serial_number(const char *serial_number)
{
	struct p2p_data *p2p = g_p2p_context.p2p;
	int len = os_strlen(serial_number);
	os_memset(p2p->cfg->serial_number, 0, 33);
	os_memcpy(p2p->cfg->serial_number, serial_number, len > 32 ? 32 : len);
}

void wifi_p2p_set_pri_dev_type(const u8 *pri_dev_type)
{
	struct p2p_data *p2p = g_p2p_context.p2p;
	p2p_set_pri_dev_type(p2p, pri_dev_type);
	os_memcpy(g_ap_device_type, pri_dev_type, WPS_DEV_TYPE_LEN);
}

void wifi_p2p_set_ssid(const char *ssid_in)
{
	struct p2p_data *p2p = g_p2p_context.p2p;

	p2p->ssid_len = os_strlen(ssid_in);
	os_memcpy(p2p->ssid, ssid_in, p2p->ssid_len);
}
void wifi_p2p_set_config_methods(u16 config_methods)
{
	p2p_set_config_methods(g_p2p_context.p2p, config_methods);
}

void wifi_p2p_init_auto_go_params(void *res, u8 *passphrase, u8 channel)
{
	struct p2p_data *p2p = g_p2p_context.p2p;
	struct p2p_go_neg_results *params = (struct p2p_go_neg_results *)res;
	os_memset(params, 0, sizeof(struct p2p_go_neg_results));

	params->role_go = 1;
	params->wps_method = WPS_PBC;
	g_p2p_context.p2p_pin[0] = '\0';

	params->ssid_len = p2p->ssid_len;
	os_memcpy(params->ssid, p2p->ssid, p2p->ssid_len);
	os_memcpy(params->passphrase, passphrase, os_strlen((char *)passphrase));

	p2p_set_state(p2p, P2P_PROVISIONING);
	if (params->role_go) {
		wpa_printf(MSG_INFO, "P2P: Start GO!");
		g_p2p_context.role = P2P_R_GO;
		wifi_p2p_set_role(P2P_R_GO);
	}

	p2p->op_channel = channel;
	if (channel >= 149) {
		p2p->op_reg_class = 124;
	} else if (channel >= 36) {
		p2p->op_reg_class = 115;
	} else {
		p2p->op_reg_class = 81;
	}

	params->freq = p2p_channel_to_freq(p2p->op_reg_class, p2p->op_channel);
	params->persistent_group = 0;
	g_p2p_context.persistent_reconnect = 0;
	g_p2p_context.max_num_sta = wifi_user_config.ap_sta_num;
}

int wifi_p2p_start_auto_go(u8 channel)
{
	char *passphrase = "12345678";
	const char *ssid_in = "DIRECT-34-Ameba";
	const char *dev_name = "Ameba1234";	// max strlen 32
	const char *manufacturer = "by customer";	// max strlen 64
	const char *model_name = "customer";	// max strlen 32
	const char *model_number = "v2.0";	// max strlen 32
	const char *serial_number = "9";	// max strlen 32
	const u8 pri_dev_type[8] = {0x00, 0x0A, 0x00, 0x50, 0xF2, 0x04, 0x00, 0x01};	// category ID:0x00,0x0A; sub category ID:0x00,0x01
	u16 config_methods = WPS_CONFIG_DISPLAY | WPS_CONFIG_KEYPAD | WPS_CONFIG_PUSHBUTTON;
	struct p2p_go_neg_results *params = NULL;

	if (g_p2p_context.role == P2P_R_DISABLE || g_p2p_context.p2p == NULL) {
		return -1;
	}

	params = (struct p2p_go_neg_results *) rtos_mem_zmalloc(sizeof(struct p2p_go_neg_results));
	if (!params) {
		return -1;
	}

	wifi_p2p_set_dev_name(dev_name);
	wifi_p2p_set_manufacturer(manufacturer);
	wifi_p2p_set_model_name(model_name);
	wifi_p2p_set_model_number(model_number);
	wifi_p2p_set_serial_number(serial_number);
	wifi_p2p_set_pri_dev_type(pri_dev_type);
	wifi_p2p_set_ssid(ssid_in);
	wifi_p2p_set_config_methods(config_methods);
	wifi_p2p_init_auto_go_params(params, (u8 *)passphrase, channel);

	wifi_p2p_start_go((char *)params->ssid, params->passphrase, channel);

	g_p2p_context.oper_chnl = channel;
	os_memcpy(g_p2p_context.group_ssid, params->ssid, strlen((char *)params->ssid));
	g_p2p_context.group_ssid[strlen((char *)params->ssid)] = '\0';
	os_memcpy(g_p2p_context.go_dev_addr, g_p2p_context.p2p->cfg->dev_addr, ETH_ALEN);
	os_memcpy(g_p2p_context.passphrase, params->passphrase, strlen(params->passphrase));
	g_p2p_context.passphrase[strlen(params->passphrase)] = '\0';
	g_p2p_context.persistent = params->persistent_group;
	g_p2p_context.group = wifi_p2p_group_init(params);

	if (params) {
		rtos_mem_free(params);
	}

	return 0;
}

extern struct eap_server_method wsc_server_eap;
extern void wpas_wsc_registrar_send_eap_fail(void *priv);
extern void wpas_wsc_send_packet(struct rtw_wps_context *rtw_wps, struct wpabuf *sendData);
extern void *eap_wsc_server_init(char *identity, char identity_len);

/* AP handle, the role of registrar or enrollee will be determined after identify req/rsp. */
void _wpas_wsc_ap_send_eap_reqidentity(void *priv, u8 *rx_buf)
{
	struct wlan_ethhdr_t *eth_hdr;
	struct lib1x_eapol *eapol;
	struct lib1x_eapol_message_hdr *received_eapol_identity_payload_hdr;
	struct lib1x_eapol_message_hdr *eapol_identity_request;
	struct rtw_wps_context *rtw_wps = (struct rtw_wps_context *)priv;
	u8 *buf = NULL;
	u32 length = ETH_HLEN + LIB1X_EAPOL_HDRLEN + LIB1X_EAPOL_MESSAGE_HDRLEN;

	(void) received_eapol_identity_payload_hdr;

	wpa_printf(MSG_INFO, "WPS: Send EAP REQ Identity!");

	buf = os_malloc(length);
	if (NULL == buf) {
		wpa_printf(MSG_INFO, "\r\nAllocate buf for send eap fail failed!\n");
		return;
	}

	// get the sending address which we want to put data into(EAPOL packet)
	eth_hdr = (struct wlan_ethhdr_t *)buf;
	os_memcpy(eth_hdr->daddr, rx_buf + 6, ETH_ALEN);
	os_memcpy(eth_hdr->saddr, rx_buf, ETH_ALEN);
	eth_hdr->type = _htons(LIB1X_ETHER_EAPOL_TYPE);

	eapol = (struct lib1x_eapol *)((unsigned char *)eth_hdr + ETH_HLEN);
	eapol->protocol_version = LIB1X_EAPOL_VER;
	eapol->packet_type = LIB1X_EAPOL_EAPPKT;
	eapol->packet_body_length = _htons(LIB1X_EAPOL_MESSAGE_HDRLEN);

	// translate received packet to EAP message form in order to fill it
	received_eapol_identity_payload_hdr = (struct lib1x_eapol_message_hdr *)(rx_buf + ETH_HLEN + LIB1X_EAPOL_HDRLEN);

	// get the sending address which we want to put data into(EAPOL packet)
	eapol_identity_request = (struct lib1x_eapol_message_hdr *)((u8 *)eapol + LIB1X_EAPOL_HDRLEN);

	// EAP Code
	eapol_identity_request->Code = LIB1X_EAP_CODE_REQUEST;

	// EAP Identifier
	wpa_printf(MSG_DEBUG, "EAP Identifier = 0x%x", received_eapol_identity_payload_hdr->Identifier);
	eapol_identity_request->Identifier = rtw_wps->identifier++;//received_eapol_identity_payload_hdr->Identifier;

	// EAP Length
	eapol_identity_request->Length = _htons(LIB1X_EAPOL_MESSAGE_HDRLEN);

	// EAP Type
	eapol_identity_request->Type = LIB1X_EAP_TYPE_IDENTITY;

	wifi_if_send_eapol(SOFTAP_WLAN_INDEX, (char *)buf, length, 0);

	// reset the WPS state machine
	rtw_wps->clientWpsDone = 0;
	rtw_wps->clientWpsProcessing = 0;

	os_free(buf, length);

	return;
}

/* When AP recv eap identity rsp,decide to be enrolle or registrar */
void _wpas_wsc_ap_check_eap_rspidentity(void *priv, u8 *rx_buf)
{
	struct lib1x_eapol_message_hdr *eapol_payload_hdr;
	struct wpabuf *sendData = NULL;
	struct eap_wsc_data *wsc_data;
	struct rtw_wps_context *rtw_wps = (struct rtw_wps_context *)priv;
	struct psk_info pskinfo;

	wpa_printf(MSG_INFO, "WPS: Received EAP Response Identity");

	eapol_payload_hdr = (struct lib1x_eapol_message_hdr *)(rx_buf + ETH_HLEN + LIB1X_EAPOL_HDRLEN);
	wpa_hexdump_key(MSG_DEBUG, "rev identity:", (u8 *)eapol_payload_hdr, _htons(eapol_payload_hdr->Length));

	rtw_wps->wsc_data = (struct eap_wsc_data *)eap_wsc_server_init(
							(char *)eapol_payload_hdr + LIB1X_EAPOL_MESSAGE_HDRLEN,
							_htons(eapol_payload_hdr->Length) - LIB1X_EAPOL_MESSAGE_HDRLEN);
	if (rtw_wps->wsc_data == NULL) {
		wpa_printf(MSG_ERROR, "eap_wsc_server_init failed!");
		return;
	}
	rtw_wps->wsc_data->wps->wps = rtw_wps->wps;
	wsc_data = rtw_wps->wsc_data;
	os_memcpy(wsc_data->wps->peer_dev.mac_addr, (rx_buf + 6), ETH_ALEN);
	wpa_printf(MSG_DEBUG, "AP will be registrar:%d!!state=%d", wsc_data->registrar, wsc_data->state);

	/* set psk*/
	memset(&pskinfo, 0, sizeof(struct psk_info));
	pskinfo.index = SOFTAP_WLAN_INDEX;
	rtw_psk_get_psk_info(&pskinfo);
	memcpy(rtw_wps->passphrase, pskinfo.psk_passphrase, sizeof(rtw_wps->passphrase));

	rtw_wps->wps->network_key = (u8 *)rtw_wps->passphrase;
	rtw_wps->wps->network_key_len = strlen(rtw_wps->passphrase);

	//network_key
	wpa_printf(MSG_DEBUG, "*set network_key OK :: %s", rtw_wps->wps->network_key);
	/*END set psk*/
	sendData = wsc_server_eap.buildReq(wsc_data, rtw_wps->identifier++); //wsc start
	if (sendData == NULL) {
		wpa_printf(MSG_WARNING, "[WPS-START]SendData is NULL!!!");
		return;
	}
	wpa_printf(MSG_DEBUG, "EapolWSCMsgSendLength = 0x%x", sendData->used);

	if (rtw_wps->clientWpsProcessing == 0) {
		rtw_wps->clientWpsProcessing = 1;
		rtw_wps->clientWpsDone = 0;
	}
	wpas_wsc_send_packet(rtw_wps, sendData);
}

/* Registrar handle */
void _wpas_wsc_registrar_send_eap_fail(void *priv)
{
	struct wlan_ethhdr_t *eth_hdr;
	struct lib1x_eapol *eapol;
	struct rtw_wps_context *rtw_wps = (struct rtw_wps_context *)priv;
	struct lib1x_eapol_message_hdr *eapol_identity_request;
	struct eap_wsc_data *wsc_data = rtw_wps->wsc_data;
	u8 *buf = NULL;
	u32 length = ETH_HLEN + LIB1X_EAPOL_HDRLEN + 4;

	wpa_printf(MSG_INFO, "WPS: Send EAP Fail!");
	buf = os_malloc(length);
	if (NULL == buf) {
		wpa_printf(MSG_INFO, "\r\nAllocate buf for send eap fail failed!\n");
		return;
	}

	// get the sending address which we want to put data into(EAPOL packet)
	eth_hdr = (struct wlan_ethhdr_t *)buf;
	os_memcpy(eth_hdr->daddr, wsc_data->wps->peer_dev.mac_addr, ETH_ALEN);
	os_memcpy(eth_hdr->saddr, rtw_wps->wps->dev.mac_addr, ETH_ALEN);
	eth_hdr->type = _htons(LIB1X_ETHER_EAPOL_TYPE);

	eapol = (struct lib1x_eapol *)((unsigned char *)eth_hdr + ETH_HLEN);
	eapol->protocol_version = LIB1X_EAPOL_VER;
	eapol->packet_type = LIB1X_EAPOL_EAPPKT;
	eapol->packet_body_length = _htons(4);

	// get the sending address which we want to put data into(EAPOL packet)
	eapol_identity_request = (struct lib1x_eapol_message_hdr *)((unsigned char *)eapol + LIB1X_EAPOL_HDRLEN);

	// EAP Code
	eapol_identity_request->Code = LIB1X_EAP_CODE_FAIL;

	// EAP Identifier
	eapol_identity_request->Identifier = rtw_wps->identifier++;//received_eapol_identity_payload_hdr->Identifier;

	// EAP Length
	eapol_identity_request->Length = _htons(4);

	wifi_if_send_eapol(SOFTAP_WLAN_INDEX, (char *)buf, length, 0);

	os_free(buf, length);

	return;
}

static int eap_copy_buf(struct wpabuf **dst, const struct wpabuf *src)
{
	if (src == NULL) {
		return -1;
	}

	wpabuf_free(*dst);
	*dst = wpabuf_dup(src);
	return *dst ? 0 : -1;
}

static void wpas_wsc_registrar_set_retrans(struct eap_wsc_data *wsc_data, struct wpabuf *sendData)
{
	int timeout = wsc_server_eap.getTimeout((void *) wsc_data) * 1000;
	eap_copy_buf(&wsc_data->last_reqdata, sendData);
	os_set_timer(&wsc_data->retrans_timer, timeout);
}

void wpas_wsc_registrar_retrans_timeout(void *timeout_ctx)
{
	struct eap_wsc_data *wsc_data = (struct eap_wsc_data *)timeout_ctx;
	int timeout = 5000;

	wpa_printf(MSG_INFO, "WPS: Transmission timeout.");
	if ((wsc_data->state == MESG) && (wsc_data->last_reqdata)) {
		wpa_printf(MSG_INFO, "WPS: Resend request data!");
		wpas_wsc_send_packet(&g_wps_context, wpabuf_dup(wsc_data->last_reqdata));
		if (wsc_data->MaxRetrans > 0) {
			wsc_data->MaxRetrans --;
			os_set_timer(&wsc_data->retrans_timer, timeout);
		}
	}
}


void wpas_wsc_registrar_send_eapol(void *priv, u8 ReceiveMessageType, void *eapol)
{
	struct wpabuf *sendData = NULL;
	struct wpabuf reqData;
	struct rtw_wps_context *rtw_wps = (struct rtw_wps_context *)priv;
	struct eap_wsc_data *wsc_data = rtw_wps->wsc_data;
	struct _LIB1X_EAPOL_WSC *eapol_wsc = (struct _LIB1X_EAPOL_WSC *)eapol;
	u8 *peer_mac = wsc_data->wps->peer_dev.mac_addr;
	memset(&reqData, 0, sizeof(struct wpabuf));

	wpa_printf(MSG_DEBUG, "Recv MSG:%d", ReceiveMessageType);

	reqData.buf = (unsigned char *)eapol_wsc;
	wsc_server_eap.process(wsc_data, &reqData);
	if (ReceiveMessageType != WPS_WSC_DONE) {
		sendData = (struct wpabuf *)wsc_server_eap.buildReq(wsc_data, rtw_wps->identifier++);
		if (sendData == NULL) {
			if (wsc_data->state == FAIL) {
				wpas_wsc_registrar_send_eap_fail((void *)rtw_wps);
				goto exit;
			} else {
				wpa_printf(MSG_WARNING, "[WPS-Mx]SendData is NULL!!!");
			}
			return;
		}
		wpa_printf(MSG_DEBUG, "EapolWSCMsgSendLength = 0x%x", sendData->used);
		wpas_wsc_registrar_set_retrans(wsc_data, sendData);
		wpas_wsc_send_packet(rtw_wps, sendData);
	} else {
		if (wsc_data->state == FAIL) {
			wpas_wsc_registrar_send_eap_fail((void *)rtw_wps);
		}
	}

exit:
	if (ReceiveMessageType == WPS_WSC_NACK || ReceiveMessageType == WPS_WSC_DONE || wsc_data->state == FAIL) {
		wpa_printf(MSG_DEBUG, "tell upper the wps process has end!");
		rtw_wps->clientWpsProcessing = 0;
		rtw_wps->identifier = 0;
		if (ReceiveMessageType == WPS_WSC_DONE) {
			wsc_server_eap.reset(wsc_data);
			rtw_wps->wsc_data = NULL;
			rtw_wps->clientWpsDone = 1;
		}
		//here the WPS has end.
		os_xqueue_send(queue_for_credential, peer_mac, 0);
	}
}

void _wpas_wsc_registrar_handle_recvd(void *priv, u8 *rx_buf)
{
	struct _LIB1X_EAPOL_WSC *eapol_wsc = (struct _LIB1X_EAPOL_WSC *)((u8 *)rx_buf + ETH_HLEN + LIB1X_EAPOL_HDRLEN);
	unsigned char  Receive_message_type;
	struct rtw_wps_context *rtw_wps = (struct rtw_wps_context *)priv;

	if (eapol_wsc->Code != LIB1X_EAP_CODE_RESPONSE) {
		wpa_printf(MSG_WARNING, "WPS Registrar: Just handle eap response!");
		return;
	}

	os_cancel_timer(&rtw_wps->wsc_data->retrans_timer);

	switch (eapol_wsc->OpCode) {
	case LIB1X_EAP_OPCODE_WSC_MSG:
	case LIB1X_EAP_OPCODE_WSC_DONE:
		if (eapol_wsc->OpCode == LIB1X_EAP_OPCODE_WSC_DONE) {
			rtos_task_priority_set(NULL, rtos_task_priority_get(NULL) + 2);
		}
		if (eapol_wsc->Flags & WSC_FLAGS_LF) {
			Receive_message_type = (*((unsigned char *)eapol_wsc + 22));
			wpa_printf(MSG_DEBUG, "Receive_message_type = 0x%x\n", Receive_message_type);
			if (rtw_wps->record_last_message_type != Receive_message_type) {
				rtw_wps->record_last_message_type = Receive_message_type;
				wpa_printf(MSG_DEBUG, "record_last_message_type = 0x%x\n", rtw_wps->record_last_message_type);
				wpas_wsc_registrar_send_eapol((void *)rtw_wps, Receive_message_type, eapol_wsc);
			} else {
				wpa_printf(MSG_DEBUG, "receive duplicate WPS MSG = 0x%x\n", Receive_message_type);
			}
		} else {
			Receive_message_type = (*((unsigned char *)eapol_wsc + 23));
			wpa_printf(MSG_DEBUG, "Receive_message_type = 0x%x\n", Receive_message_type);
			wpas_wsc_registrar_send_eapol((void *)rtw_wps, Receive_message_type, eapol_wsc);
		}
		if (eapol_wsc->OpCode == LIB1X_EAP_OPCODE_WSC_DONE) {
			rtos_task_priority_set(NULL, rtos_task_priority_get(NULL) - 2);
		}
		break;
	case LIB1X_EAP_OPCODE_WSC_NACK:
	case LIB1X_EAP_OPCODE_WSC_ACK:
		if (eapol_wsc->Flags & WSC_FLAGS_LF) {
			Receive_message_type = (*((unsigned char *)eapol_wsc + 22));
			wpa_printf(MSG_DEBUG, "record_last_message_type = 0x%x\n", rtw_wps->record_last_message_type);
			wpas_wsc_registrar_send_eapol((void *)rtw_wps, Receive_message_type, eapol_wsc);
		} else {
			Receive_message_type = (*((unsigned char *)eapol_wsc + 23));
			wpa_printf(MSG_DEBUG, "Receive_message_type = 0x%x\n", Receive_message_type);
			wpas_wsc_registrar_send_eapol((void *)rtw_wps, Receive_message_type, eapol_wsc);
		}
		break;
	default:
		wpa_printf(MSG_DEBUG, "EAP-WSC: Unexpected OpCode %d in "
				   "registrar handle", eapol_wsc->OpCode);

	}
}

