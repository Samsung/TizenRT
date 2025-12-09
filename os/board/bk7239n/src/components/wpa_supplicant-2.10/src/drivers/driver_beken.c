/*
 * Driver interaction with Linux Host AP driver
 * Copyright (c) 2003-2005, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#include <common/bk_include.h>
#include "bk_hostapd_intf.h"
#include "../utils/includes.h"
#include "../utils/common.h"
#include "driver.h"
#include "../utils/eloop.h"
#include "driver_beken.h"
#include "bk_wifi_types.h"
#include "../common/ieee802_11_defs.h"
#include "../common/ieee802_11_common.h"
#include "../l2_packet/l2_packet.h"
#include "../../bk_patch/fake_socket.h"
#include "priv_netlink.h"
#include "netlink.h"
#include "../../hostapd/hostapd_cfg.h"
//#include "mac_common.h"
#include "../../bk_patch/signal.h"
#include "../utils/eloop.h"
#include "bk_rw.h"
#include "modules/wifi.h"
#include "../../wpa_supplicant/wpa_supplicant_i.h"
#include "../../wpa_supplicant/wpa_scan.h"
#ifdef CONFIG_SME
#include "driver.h"
#endif
#include "../../hostapd/main_none.h"
#include "../../bk_patch/wpa_err.h"
#include "bk_wifi_private.h"
#ifdef CONFIG_P2P
#include "sa_station.h"
#endif
#if CONFIG_LWIP
//#include "net.h"
//#include "bk_net.h"
#endif
#include "bk_feature.h"
#include "rw_ieee80211.h"
#include "fhost_msg.h"

static const u8 rfc1042_header[6] = { 0xaa, 0xaa, 0x03, 0x00, 0x00, 0x00 };

/**
 * enum rwnx_sta_flags - STATION flags
 *
 * @RWNX_STA_EXT_AUTH: External authentication is in progress
 */
enum rwnx_sta_flags {
	RWNX_STA_EXT_AUTH = BIT(0),
	RWNX_STA_FT_OVER_DS = BIT(1),
	RWNX_STA_FT_OVER_AIR = BIT(2),
};

struct hostap_driver_data {
	struct hostapd_data *hapd;
	struct wpa_supplicant *wpa_s;

	/* extended capabilties */
	u8 sta_ext_capa[9];

	char iface[IFNAMSIZ + 1];
	struct l2_packet_data *sock_xmit;	/* raw packet xmit socket */
	int sock; /* raw packet socket for driver access */
	int ioctl_sock; /* socket for ioctl() use */

	struct netlink_data *netlink;
	u8 own_addr[ETH_ALEN];
	u8 vif_index;

	int we_version;

	u8 *generic_ie;
	size_t generic_ie_len;
	u8 *wps_ie;
	size_t wps_ie_len;
#ifdef CONFIG_IEEE80211R
	uint32_t flags;  // for STA connection
	u8 target_ap[ETH_ALEN];  // valid for STA
	u8 *ft_ie;
	size_t ft_ie_len;
#endif

#ifdef CONFIG_P2P_GO
	enum nl80211_iftype nlmode;
#endif
};

static int hostapd_ioctl(void *priv, struct prism2_hostapd_param *param, int len);
static int hostap_set_iface_flags(void *priv, int dev_up);
extern uint32_t wpa_hostapd_queue_poll(uint32_t param);
extern int hapd_intf_add_vif(struct prism2_hostapd_param *param, int len);
extern int eloop_get_signal_count(void);
void wpa_driver_scan_timeout(void *eloop_ctx, void *timeout_ctx);

static void handle_data(struct hostap_driver_data *drv, u8 *buf, size_t len,
                        u16 stype)
{
    struct ieee80211_hdr *hdr;
    u16 fc, ethertype;
    u8 *pos, *sa;
    size_t left;
    union wpa_event_data event;

    if (len < sizeof(struct ieee80211_hdr))
        return;

    hdr = (struct ieee80211_hdr *) buf;
    fc = le_to_host16(hdr->frame_control);

    if ((fc & (WLAN_FC_FROMDS | WLAN_FC_TODS)) != WLAN_FC_TODS)
    {
        WPA_LOGE("invalid data frame 0x%04x\n", fc);
        return;
    }

    sa = hdr->addr2;
    os_memset(&event, 0, sizeof(event));
    event.rx_from_unknown.bssid = 0; // get_hdr_bssid(hdr, len); wangzhilei0
    event.rx_from_unknown.addr = sa;
    wpa_supplicant_event(drv->hapd, EVENT_RX_FROM_UNKNOWN, &event);

    pos = (u8 *) (hdr + 1);
    left = len - sizeof(*hdr);

    if (left < sizeof(rfc1042_header))
    {
        WPA_LOGE("Too short data frame\n");
        return;
    }

    if (memcmp(pos, rfc1042_header, sizeof(rfc1042_header)) != 0)
    {
        WPA_LOGE("Need RFC1042 header\n");
        return;
    }
    pos += sizeof(rfc1042_header);
    left -= sizeof(rfc1042_header);

    if (left < 2)
    {
        WPA_LOGE("Invalid ethertype\n");
        return;
    }

    ethertype = WPA_GET_BE16(pos);
    pos += 2;
    left -= 2;
    switch (ethertype)
    {
    case ETH_P_PAE:
        drv_event_eapol_rx(drv->hapd, sa, pos, left);
        break;

    default:
        WPA_LOGE("Unknown ethertype 0x%04x\n", ethertype);
        break;
    }
}


static void handle_tx_callback(struct hostap_driver_data *drv, u8 *buf,
                               size_t len, int ok)
{
    struct ieee80211_hdr *hdr;
    u16 fc;
    union wpa_event_data event;

    hdr = (struct ieee80211_hdr *) buf;
    fc = le_to_host16(hdr->frame_control);

    os_memset(&event, 0, sizeof(event));
    event.tx_status.type = WLAN_FC_GET_TYPE(fc);
    event.tx_status.stype = WLAN_FC_GET_STYPE(fc);
    event.tx_status.dst = hdr->addr1;
    event.tx_status.data = buf;
    event.tx_status.data_len = len;
    event.tx_status.ack = ok;

    if (drv->wpa_s)
        wpa_supplicant_event_sta(drv->wpa_s, EVENT_TX_STATUS, &event);
    else
        wpa_supplicant_event(drv->hapd, EVENT_TX_STATUS, &event);
}


static void handle_frame(struct hostap_driver_data *drv, u8 *buf, size_t len)
{
    struct ieee80211_hdr *hdr;
    u16 fc, type, stype;
    size_t data_len = len;
    int ver;
    union wpa_event_data event;

    /* PSPOLL is only 16 bytes, but driver does not (at least yet) pass
     * these to user space */
    if (len < 24)
    {
        wpa_printf(MSG_MSGDUMP, "handle_frame: too short (%lu)",
                   (unsigned long) len);
        return;
    }

    hdr = (struct ieee80211_hdr *) buf;
    fc = le_to_host16(hdr->frame_control);
    type = WLAN_FC_GET_TYPE(fc);
    stype = WLAN_FC_GET_STYPE(fc);

    if (type != WLAN_FC_TYPE_MGMT || stype != WLAN_FC_STYPE_BEACON)
    {
        wpa_hexdump(MSG_MSGDUMP, "Received management frame",
                    buf, len);
    }

    ver = fc & WLAN_FC_PVER;

    /* protocol version 2 is reserved for indicating ACKed frame (TX
     * callbacks), and version 1 for indicating failed frame (no ACK, TX
     * callbacks) */
    if (ver == 1 || ver == 2)
    {
        handle_tx_callback(drv, buf, data_len, ver == 2 ? 1 : 0);
        return;
    }
    else if (ver != 0)
    {
        WPA_LOGE("unknown proto version %d\n", ver);
        return;
    }

    switch (type)
    {
    case WLAN_FC_TYPE_MGMT:
        os_memset(&event, 0, sizeof(event));
        event.rx_mgmt.frame = buf;
        event.rx_mgmt.frame_len = data_len;
		if (drv->wpa_s)
	        wpa_supplicant_event_sta(drv->wpa_s, EVENT_RX_MGMT, &event);
		else
			wpa_supplicant_event(drv->hapd, EVENT_RX_MGMT, &event);
        break;
    case WLAN_FC_TYPE_CTRL:
        wpa_printf(MSG_DEBUG, "CTRL");
        break;
    case WLAN_FC_TYPE_DATA:
        wpa_printf(MSG_DEBUG, "DATA");
        handle_data(drv, buf, data_len, stype);
        break;
    default:
        wpa_printf(MSG_DEBUG, "unknown frame type %d", type);
        break;
    }
}

static void handle_read(int sock, void *eloop_ctx, void *sock_ctx)
{
    int len;
    unsigned char *buf;
    struct hostap_driver_data *drv;

    drv = eloop_ctx;
    buf = os_malloc(TMP_BUF_LEN);
    if(!buf) {
        WPA_LOGE("handle_read oom\r\n");
        goto read_exit;
    }

    len = fsocket_recv(sock, buf, TMP_BUF_LEN, 0);
    if (len < 0)
    {
        wpa_printf(MSG_ERROR, "recv: %s", strerror(errno));
        goto read_exit;
    }

#if HOSTAP_THREAD_SAFE_WORKAROUND
	if (hostapd_is_init_completed()) {
		handle_frame(drv, buf, len);
	} else {
		wpa_printf(MSG_DEBUG, "hapd: rx frame, but init not completed!\n");
	}
#else
	handle_frame(drv, buf, len);
#endif

read_exit:
    if(buf)
    {
        os_free(buf);
    }

    return;
}

static void handle_eapol(void *ctx, const u8 *src_addr, const u8 *buf, size_t len)
{
    struct hostap_driver_data *drv = ctx;
    drv_event_eapol_rx(drv->hapd, src_addr, buf, len);
}

int hostap_get_ifhwaddr(int sock, const char *ifname, u8 *addr)
{
    return 0;
}

static int hostap_init_sockets(struct hostap_driver_data *drv, u8 *own_addr)
{
    int protocol = ETH_P_ALL;
    protocol += drv->vif_index;

    drv->sock = fsocket_init(PF_PACKET, SOCK_RAW, protocol);
    drv->sock_xmit = l2_packet_init(drv->iface, drv->own_addr, ETH_P_EAPOL,
                                    handle_eapol, drv, 0);

    if (eloop_register_read_sock(drv->sock, handle_read, drv, NULL))
    {
        wpa_printf(MSG_ERROR, "Could not register read socket");
        return -1;
    }

    if (hostap_set_iface_flags(drv, 1))
    {
        return -1;
    }

    return hostap_get_ifhwaddr(drv->sock, drv->iface, own_addr);
}


static int hostap_send_mlme(void *priv, const u8 *msg, size_t len, int noack,
                            unsigned int freq, const u16 *csa_offs, size_t csa_offs_len,
                            int no_encrypt, unsigned int wait)

{
    struct hostap_driver_data *drv = priv;
    int res;
    S_TYPE_PTR type_ptr = os_zalloc(sizeof(S_TYPE_ST));
	if (type_ptr == NULL) {
		WPA_LOGE("hostap_send_mlme oom\r\n");
		return kNoMemoryErr;
    }
    type_ptr->type = HOSTAPD_MGMT;
    type_ptr->vif_index = drv->vif_index;

    res = fsocket_send(drv->sock, msg, len, type_ptr);
	res = res > 0 ? 0 : -1; /* 0 on success, -1 on failure */

    return res;
}


static int hostap_send_eapol(void *priv, const u8 *addr, const u8 *data,
                             size_t data_len, int encrypt, const u8 *own_addr,
                             u32 flags)
{
    struct hostap_driver_data *drv = priv;
    int status;

    status = l2_packet_send(drv->sock_xmit, addr, ETH_P_EAPOL, data, data_len);

    return status;
}


static int hostap_sta_set_flags(void *priv, const u8 *addr,
                                unsigned int total_flags, unsigned int flags_or,
                                unsigned int flags_and)
{
    struct hostap_driver_data *drv = priv;
    struct prism2_hostapd_param param;

    if (flags_or & WPA_STA_AUTHORIZED)
        flags_or = BIT(5); /* WLAN_STA_AUTHORIZED */
    if (!(flags_and & WPA_STA_AUTHORIZED))
        flags_and = ~ BIT(5);
    else
        flags_and = ~0;

    memset(&param, 0, sizeof(param));
    param.cmd = PRISM2_HOSTAPD_SET_FLAGS_STA;
    memcpy(param.sta_addr, addr, ETH_ALEN);
    param.u.set_flags_sta.flags_or = flags_or;
    param.u.set_flags_sta.flags_and = flags_and;
    param.vif_idx = drv->vif_index;

    return hostapd_ioctl(drv, &param, sizeof(param));
}


static int hostap_set_iface_flags(void *priv, int dev_up)
{
    return 0;
}


static int hostapd_ioctl(void *priv, struct prism2_hostapd_param *param,
                         int len)
{
    struct hostap_driver_data *drv = priv;
    struct iwreq iwr;

    memset(&iwr, 0, sizeof(iwr));
    os_strlcpy(iwr.ifr_name, drv->iface, IFNAMSIZ);
    iwr.u.data.pointer = (c_addr_t) param;
    iwr.u.data.length = len;

    if (ioctl_inet(drv->ioctl_sock, drv->vif_index, PRISM2_IOCTL_HOSTAPD, (unsigned long)&iwr) < 0)
    {
        return -1;
    }

    return 0;
}

static int wpa_driver_hostap_set_key(void *priv, struct wpa_driver_set_key_params *params)
{
    struct hostap_driver_data *drv = priv;
    struct prism2_hostapd_param *param;
    u8 *buf;
    size_t blen;
    int ret = 0;

    blen = sizeof(*param) + params->key_len;
    buf = os_zalloc(blen);
    if (!buf)
        return -1;

    param = (struct prism2_hostapd_param *) buf;
    param->cmd = PRISM2_SET_ENCRYPTION;
    if (params->addr == NULL)
        memset(param->sta_addr, 0xff, ETH_ALEN);
    else
        memcpy(param->sta_addr, params->addr, ETH_ALEN);

    switch (params->alg) {
    case WPA_ALG_NONE:
        os_strlcpy((char *) param->u.crypt.alg, "NONE",
                   HOSTAP_CRYPT_ALG_NAME_LEN);
        break;
    case WPA_ALG_WEP:
        if (params->key_len == 13)
            os_strlcpy((char *) param->u.crypt.alg, "WEP104",
                       HOSTAP_CRYPT_ALG_NAME_LEN);
        else
            os_strlcpy((char *) param->u.crypt.alg, "WEP40",
                       HOSTAP_CRYPT_ALG_NAME_LEN);
        break;
    case WPA_ALG_TKIP:
        os_strlcpy((char *) param->u.crypt.alg, "TKIP",
                   HOSTAP_CRYPT_ALG_NAME_LEN);
        break;
    case WPA_ALG_CCMP_256:
        os_strlcpy((char *) param->u.crypt.alg, "CCMP-256",
                   HOSTAP_CRYPT_ALG_NAME_LEN);
        break;
    case WPA_ALG_CCMP:
        os_strlcpy((char *) param->u.crypt.alg, "CCMP",
                   HOSTAP_CRYPT_ALG_NAME_LEN);
        break;
	case WPA_ALG_BIP_CMAC_128:
        os_strlcpy((char *) param->u.crypt.alg, "BIP",
                   HOSTAP_CRYPT_ALG_NAME_LEN);
		break;
    default:
    	WPA_LOGE("%s: unsupport alg %d\n", __func__, params->alg);
        os_free(buf);
        return -1;
    }
    param->u.crypt.flags = params->set_tx ? HOSTAP_CRYPT_FLAG_SET_TX_KEY : 0;
    param->u.crypt.idx = params->key_idx;
    param->u.crypt.key_len = params->key_len;
    param->vif_idx = drv->vif_index;

    memcpy((u8 *) (param + 1), params->key, params->key_len);

    if (hostapd_ioctl(drv, param, blen))
        ret = -1;

    os_free(buf);

    return ret;
}


static int hostap_get_seqnum(const char *ifname, void *priv, const u8 *addr,
                             int idx, u8 *seq)
{
    struct hostap_driver_data *drv = priv;
    struct prism2_hostapd_param *param;
    u8 *buf;
    size_t blen;
    int ret = 0;

    blen = sizeof(*param) + 32;
    buf = os_zalloc(blen);
    if (buf == NULL)
        return -1;

    param = (struct prism2_hostapd_param *) buf;
    param->cmd = PRISM2_GET_ENCRYPTION;
    if (addr == NULL)
        os_memset(param->sta_addr, 0xff, ETH_ALEN);
    else
        os_memcpy(param->sta_addr, addr, ETH_ALEN);
    param->u.crypt.idx = idx;

    if (hostapd_ioctl(drv, param, blen))
    {
        WPA_LOGE("Failed to get encryption.\n");
        ret = -1;
    }
    else
    {
        os_memcpy(seq, param->u.crypt.seq, 8);
    }
    os_free(buf);

    return ret;
}


static int hostap_ioctl_prism2param(void *priv, int param, int value)
{
    struct hostap_driver_data *drv = priv;
    struct iwreq iwr;
    int *i;

    memset(&iwr, 0, sizeof(iwr));
    os_strlcpy(iwr.ifr_name, drv->iface, IFNAMSIZ);
    i = (int *) iwr.u.name;
    *i++ = param;
    *i++ = value;

    if (ioctl_inet(drv->ioctl_sock, drv->vif_index, PRISM2_IOCTL_PRISM2_PARAM, (unsigned long)&iwr) < 0)
    {
        return -1;
    }

    return 0;
}


static int hostap_set_ieee8021x(void *priv, struct wpa_bss_params *params)
{
    struct hostap_driver_data *drv = priv;
    int enabled = params->enabled;

    /* enable kernel driver support for IEEE 802.1X */
    if (hostap_ioctl_prism2param(drv, PRISM2_PARAM_IEEE_802_1X, enabled))
    {
        WPA_LOGE("setup IEEE 802.1X support fail\r\n");
        return -1;
    }

    if (!enabled)
        return 0;

    /* use host driver implementation of encryption to allow
     * individual keys and passing plaintext EAPOL frames */
    if (hostap_ioctl_prism2param(drv, PRISM2_PARAM_HOST_DECRYPT, 1) ||
            hostap_ioctl_prism2param(drv, PRISM2_PARAM_HOST_ENCRYPT, 1))
    {
        WPA_LOGE("setup host-based encryption fail\r\n");
        return -1;
    }

    return 0;
}


static int hostap_set_privacy(void *priv, int enabled)
{
    struct hostap_drvier_data *drv = priv;

    return hostap_ioctl_prism2param(drv, PRISM2_PARAM_PRIVACY_INVOKED,
                                    enabled);
}


static int hostap_set_ssid(void *priv, const u8 *buf, int len)
{
    struct hostap_driver_data *drv = priv;
    struct iwreq iwr;

    memset(&iwr, 0, sizeof(iwr));
    os_strlcpy(iwr.ifr_name, drv->iface, IFNAMSIZ);
    iwr.u.essid.flags = 1; /* SSID active */
    iwr.u.essid.pointer = (c_addr_t) buf;
    iwr.u.essid.length = len + 1;

    if (ioctl_inet(drv->ioctl_sock, drv->vif_index, SIOCSIWESSID, (unsigned long)&iwr) < 0)
    {
        return -1;
    }

    return 0;
}


static int hostap_flush(void *priv)
{
    struct hostap_driver_data *drv = priv;
    struct prism2_hostapd_param param;

    memset(&param, 0, sizeof(param));
    param.cmd = PRISM2_HOSTAPD_FLUSH;
    return hostapd_ioctl(drv, &param, sizeof(param));
}


static int hostap_read_sta_data(void *priv,
                                struct hostap_sta_driver_data *data,
                                const u8 *addr)
{
	STA_INF_PTR sta_entry;

	os_memset(data, 0, sizeof(*data));

	sta_entry = rwm_mgmt_sta_mac2ptr((u8 *)addr);
	if (sta_entry) {
		data->signal = sta_mgmt_get_rssi(sta_entry);
		/*TODO: add other information about the sta */
	}

	return 0;
}

#ifdef CONFIG_P2P
#define WLAN_DEFAULT_GO_IP         "192.168.49.1"
#define WLAN_DEFAULT_GO_GW         "192.168.49.1"
#define WLAN_DEFAULT_GO_MASK       "255.255.255.0"
/**
 * set vif mode to @nlmode
 */
static int wpa_driver_set_mode(void *priv, enum nl80211_iftype nlmode)
{
	int ret;
	uint8_t mac[ETH_ALEN];
    struct hostap_driver_data *drv = priv;
	VIF_INF_PTR vif = rwm_mgmt_vif_idx2ptr(drv->vif_index);
	struct mm_add_if_cfm cfm;

	if (!vif)
		return -1;

	os_memcpy(mac, &vif->mac_addr, ETH_ALEN);
#ifdef CONFIG_P2P_GO
	drv->nlmode = nlmode;
#endif

	if (nlmode == NL80211_IFTYPE_P2P_CLIENT
#ifdef CONFIG_P2P_GO
		|| nlmode == NL80211_IFTYPE_P2P_GO
#endif
		) {
		if (vif->type != VIF_STA || !vif->p2p) {
#if CONFIG_LWIP
			net_wlan_remove_netif(mac);
#endif
			ret = rw_msg_send_remove_if(drv->vif_index);
			if (ret)
				return ret;
			sa_station_init();

			ret = rw_msg_send_add_if(mac, nlmode, false /* dummy */, &cfm);
			if (ret || cfm.status)
				return -1;
			drv->vif_index = cfm.inst_nbr;
#if CONFIG_LWIP
			net_wlan_add_netif(mac);
#endif
#ifdef CONFIG_P2P_GO
			if (nlmode == NL80211_IFTYPE_P2P_GO) {
				g_ap_param_ptr->chann = bk_wlan_ap_get_default_channel();
				wpa_printf(MSG_DEBUG, "%s, %d, channel: %u", __func__, __LINE__, g_ap_param_ptr->chann);
				drv->sock_xmit = l2_packet_init(drv->iface, drv->own_addr, ETH_P_EAPOL,
                                    handle_eapol, drv, 0);

			ip_address_set(BK_SOFT_AP,
	                   DHCP_SERVER,
	                   WLAN_DEFAULT_GO_IP,
	                   WLAN_DEFAULT_GO_MASK,
	                   WLAN_DEFAULT_GO_GW,
	                   WLAN_DEFAULT_GO_GW);

			/* restart lwip network */
			uap_ip_start();
			}
#endif
		}
		return 0;
	} else if (nlmode == NL80211_IFTYPE_STATION) {
		if (vif->type != VIF_STA || vif->p2p) {
#if CONFIG_LWIP
			net_wlan_remove_netif(mac);
#endif
			ret = rw_msg_send_remove_if(drv->vif_index);
			if (ret)
				return ret;

			sa_station_init();

			ret = rw_msg_send_add_if(mac, nlmode, false /* dummy */, &cfm);
			if (ret || cfm.status)
				return -1;
			drv->vif_index = cfm.inst_nbr;
#if CONFIG_LWIP
			net_wlan_add_netif(mac);
#endif
		}
		return 0;
	}

	return -1;
}
#endif /* CONFIG_P2P */


static int wpa_driver_hostap_init_vif(void *priv, u32 type)
{
    struct hostap_driver_data *drv = priv;
    struct prism2_hostapd_param param;

    memset(&param, 0, sizeof(param));
    param.cmd = PRISM2_HOSTAPD_WPA_INIT_VIF;
    memcpy(param.sta_addr, drv->own_addr, ETH_ALEN);
    param.u.add_if.indx_ptr = &drv->vif_index;
    param.u.add_if.type = type;

    return hapd_intf_add_vif(&param, sizeof(param));
}

static int wpa_driver_hostap_deinit_vif(void *priv)
{
    struct hostap_driver_data *drv = priv;
    struct prism2_hostapd_param param;

    memset(&param, 0, sizeof(param));
    param.cmd = PRISM2_HOSTAPD_WPA_DEINIT_VIF;
    memcpy(param.sta_addr, drv->own_addr, ETH_ALEN);
    param.vif_idx = drv->vif_index;

    return hostapd_ioctl(drv, &param, sizeof(param));
}

static int wpa_driver_hostap_start_apm(void *priv)
{
    struct hostap_driver_data *drv = priv;
    struct prism2_hostapd_param param;

    memset(&param, 0, sizeof(param));
    param.cmd = PRISM2_HOSTAPD_START_APM;
    memcpy(param.sta_addr, drv->own_addr, ETH_ALEN);
    param.vif_idx = drv->vif_index;

    return hostapd_ioctl(drv, &param, sizeof(param));
}

static int wpa_driver_hostap_stop_apm(void *priv)
{
    struct hostap_driver_data *drv = priv;
    struct prism2_hostapd_param param;

    memset(&param, 0, sizeof(param));
    param.cmd = PRISM2_HOSTAPD_STOP_APM;
    memcpy(param.sta_addr, drv->own_addr, ETH_ALEN);
    param.vif_idx = drv->vif_index;

    return hostapd_ioctl(drv, &param, sizeof(param));
}

/*
 * FIXME: params.set will set instead of add
 */
static int hostap_sta_add(void *priv, struct hostapd_sta_add_params *params)
{
    struct hostap_driver_data *drv = priv;
    struct prism2_hostapd_param param;
    size_t i;
	u32 flags = 0;

    memset(&param, 0, sizeof(param));
    param.cmd = PRISM2_HOSTAPD_ADD_STA;
    memcpy(param.sta_addr, params->addr, ETH_ALEN);
    param.u.add_sta.aid = params->aid;
    param.u.add_sta.capability = params->capability;
    param.u.add_sta.supp_rates_len = params->supp_rates_len;
    for(i = 0; i < param.u.add_sta.supp_rates_len; i++)
        param.u.add_sta.supp_rates[i] = params->supp_rates[i];
        
#ifdef DOUBLE_CHECK_TCJ
	param.u.add_sta.set = params->set;
#ifdef CONFIG_MESH
	param.u.add_sta.arg = params->sta;
#endif
#endif
    param.vif_idx = drv->vif_index;

	if (params->ht_capabilities)
		flags |= STA_HT_CAPA;
	if (params->vht_capabilities)
		flags |= STA_VHT_CAPA;
	if (params->he_capab)
		flags |= STA_HE_CAPA;
	if (params->flags & WPA_STA_WMM)
		flags |= STA_QOS_CAPA;
	if (params->flags & WPA_STA_MFP)
		flags |= STA_MFP_CAPA;
	param.u.add_sta.flags = flags;

    return hostapd_ioctl(drv, &param, sizeof(param));
}


static int hostap_sta_remove(void *priv, const u8 *addr)
{
    struct hostap_driver_data *drv = priv;
    struct prism2_hostapd_param param;

    hostap_sta_set_flags(drv, addr, 0, 0, ~WPA_STA_AUTHORIZED);

    memset(&param, 0, sizeof(param));
    param.cmd = PRISM2_HOSTAPD_REMOVE_STA;
    memcpy(param.sta_addr, addr, ETH_ALEN);

    param.vif_idx = drv->vif_index;
    if (hostapd_ioctl(drv, &param, sizeof(param)))
    {
        WPA_LOGE("remove station fail\n");
        return -1;
    }
    return 0;
}


static int hostap_get_inact_sec(void *priv, const u8 *addr)
{
    struct hostap_driver_data *drv = priv;
    struct prism2_hostapd_param param;

    memset(&param, 0, sizeof(param));
    param.cmd = PRISM2_HOSTAPD_GET_INFO_STA;
    memcpy(param.sta_addr, addr, ETH_ALEN);
    if (hostapd_ioctl(drv, &param, sizeof(param)))
    {
        return -1;
    }

    return param.u.get_info_sta.inactive_sec;
}


static int hostap_sta_clear_stats(void *priv, const u8 *addr)
{
    struct hostap_driver_data *drv = priv;
    struct prism2_hostapd_param param;

    memset(&param, 0, sizeof(param));
    param.cmd = PRISM2_HOSTAPD_STA_CLEAR_STATS;
    memcpy(param.sta_addr, addr, ETH_ALEN);
    if (hostapd_ioctl(drv, &param, sizeof(param)))
    {
        return -1;
    }

    return 0;
}


static int hostapd_ioctl_set_generic_elem(struct hostap_driver_data *drv)
{
    struct prism2_hostapd_param *param;
    int res;
    size_t blen, elem_len;

    elem_len = drv->generic_ie_len + drv->wps_ie_len;
    blen = PRISM2_HOSTAPD_GENERIC_ELEMENT_HDR_LEN + elem_len;
    if (blen < sizeof(*param))
        blen = sizeof(*param);

    param = os_zalloc(blen);
    if (param == NULL)
        return -1;

    param->cmd = PRISM2_HOSTAPD_SET_GENERIC_ELEMENT;
    param->u.generic_elem.len = elem_len;
    if (drv->generic_ie)
    {
        os_memcpy(param->u.generic_elem.data, drv->generic_ie,
                  drv->generic_ie_len);
    }
    if (drv->wps_ie)
    {
        os_memcpy(&param->u.generic_elem.data[drv->generic_ie_len],
                  drv->wps_ie, drv->wps_ie_len);
    }
    wpa_hexdump(MSG_DEBUG, "hostap: Set generic IE",
                param->u.generic_elem.data, elem_len);
    res = hostapd_ioctl(drv, param, blen);

    os_free(param);

    return res;
}


static int hostap_set_generic_elem(void *priv,
                                   const u8 *elem, size_t elem_len)
{
    struct hostap_driver_data *drv = priv;

    os_free(drv->generic_ie);
    drv->generic_ie = NULL;
    drv->generic_ie_len = 0;
    if (elem)
    {
        drv->generic_ie = os_malloc(elem_len);
        if (drv->generic_ie == NULL)
            return -1;
        os_memcpy(drv->generic_ie, elem, elem_len);
        drv->generic_ie_len = elem_len;
    }

    return hostapd_ioctl_set_generic_elem(drv);
}

#ifdef ANDROID_P2P
static int hostap_set_ap_wps_ie(void *priv, const struct wpabuf *beacon,
                                const struct wpabuf *proberesp,
                                const struct wpabuf *assocresp)
{
    struct hostap_driver_data *drv = priv;

    /*
     * Host AP driver supports only one set of extra IEs, so we need to
     * use the Probe Response IEs also for Beacon frames since they include
     * more information.
     */

    os_free(drv->wps_ie);
    drv->wps_ie = NULL;
    drv->wps_ie_len = 0;
    if (proberesp)
    {
        drv->wps_ie = os_malloc(wpabuf_len(proberesp));
        if (drv->wps_ie == NULL)
            return -1;
        os_memcpy(drv->wps_ie, wpabuf_head(proberesp),
                  wpabuf_len(proberesp));
        drv->wps_ie_len = wpabuf_len(proberesp);
    }

    return hostapd_ioctl_set_generic_elem(drv);
}
#endif


static void
hostapd_wireless_event_wireless_custom(struct hostap_driver_data *drv,
                                       char *custom)
{
    wpa_printf(MSG_DEBUG, "Custom wireless event: '%s'", custom);

    if (strncmp(custom, "MLME-MICHAELMICFAILURE.indication", 33) == 0)
    {
        char *pos;
        u8 addr[ETH_ALEN];
        pos = strstr(custom, "addr=");
        if (pos == NULL)
        {
            wpa_printf(MSG_DEBUG,
                       "MLME-MICHAELMICFAILURE.indication "
                       "without sender address ignored");
            return;
        }
        pos += 5;
        if (hwaddr_aton(pos, addr) == 0)
        {
            union wpa_event_data data;
            os_memset(&data, 0, sizeof(data));
            data.michael_mic_failure.unicast = 1;
            data.michael_mic_failure.src = addr;
            wpa_supplicant_event(drv->hapd,
                                 EVENT_MICHAEL_MIC_FAILURE, &data);
        }
        else
        {
            wpa_printf(MSG_DEBUG,
                       "MLME-MICHAELMICFAILURE.indication "
                       "with invalid MAC address");
        }
    }
}


static void hostapd_wireless_event_wireless(struct hostap_driver_data *drv,
        char *data, int len)
{
    struct iw_event iwe_buf, *iwe = &iwe_buf;
    char *pos, *end, *custom, *buf;

    pos = data;
    end = data + len;

    while (pos + IW_EV_LCP_LEN <= end)
    {
        /* Event data may be unaligned, so make a local, aligned copy
         * before processing. */
        memcpy(&iwe_buf, pos, IW_EV_LCP_LEN);
        wpa_printf(MSG_DEBUG, "Wireless event: cmd=0x%x len=%d",
                   iwe->cmd, iwe->len);
        if (iwe->len <= IW_EV_LCP_LEN)
            return;

        custom = pos + IW_EV_POINT_LEN;
        if (drv->we_version > 18 &&
                (iwe->cmd == IWEVMICHAELMICFAILURE ||
                 iwe->cmd == IWEVCUSTOM))
        {
            /* WE-19 removed the pointer from struct iw_point */
            char *dpos = (char *) &iwe_buf.u.data.length;
            int dlen = dpos - (char *) &iwe_buf;
            memcpy(dpos, pos + IW_EV_LCP_LEN,
                   sizeof(struct iw_event) - dlen);
        }
        else
        {
            memcpy(&iwe_buf, pos, sizeof(struct iw_event));
            custom += IW_EV_POINT_OFF;
        }

        switch (iwe->cmd)
        {
        case IWEVCUSTOM:
            if (custom + iwe->u.data.length > end)
                return;
            buf = os_malloc(iwe->u.data.length + 1);
            if (buf == NULL)
                return;
            memcpy(buf, custom, iwe->u.data.length);
            buf[iwe->u.data.length] = '\0';
            hostapd_wireless_event_wireless_custom(drv, buf);

            os_free(buf);
            break;
        }

        pos += iwe->len;
    }
}


static void hostapd_wireless_event_rtm_newlink(void *ctx,
        struct ifinfomsg *ifi,
        u8 *buf, size_t len)
{
    struct hostap_driver_data *drv = ctx;
    int attrlen, rta_len;
    struct rtattr *attr;

    /* TODO: use ifi->ifi_index to filter out wireless events from other
     * interfaces */

    attrlen = len;
    attr = (struct rtattr *) buf;

    rta_len = RTA_ALIGN(sizeof(struct rtattr));
    while (RTA_OK(attr, attrlen))
    {
        if (attr->rta_type == IFLA_WIRELESS)
        {
            hostapd_wireless_event_wireless(
                drv, ((char *) attr) + rta_len,
                attr->rta_len - rta_len);
        }
        attr = RTA_NEXT(attr, attrlen);
    }
}


static int hostap_get_we_version(struct hostap_driver_data *drv)
{
    struct iw_range *range;
    struct iwreq iwr;
    int minlen;
    size_t buflen;

    drv->we_version = 0;

    /*
     * Use larger buffer than struct iw_range in order to allow the
     * structure to grow in the future.
     */
    buflen = sizeof(struct iw_range) + 500;
    range = os_zalloc(buflen);
    if (range == NULL)
        return -1;

    memset(&iwr, 0, sizeof(iwr));
    os_strlcpy(iwr.ifr_name, drv->iface, IFNAMSIZ);
    iwr.u.data.pointer = (c_addr_t) range;
    iwr.u.data.length = buflen;

    minlen = ((char *) &range->enc_capa) - (char *) range +
             sizeof(range->enc_capa);

    if (ioctl_inet(drv->ioctl_sock, drv->vif_index, SIOCGIWRANGE, (unsigned long)&iwr) < 0)
    {
        os_free(range);
        return -1;
    }
    else if (iwr.u.data.length >= minlen &&
             range->we_version_compiled >= 18)
    {
        drv->we_version = range->we_version_compiled;
    }

    os_free(range);
    return 0;
}


static int hostap_wireless_event_init(struct hostap_driver_data *drv)
{
    struct netlink_config *cfg;

    hostap_get_we_version(drv);

    cfg = os_zalloc(sizeof(*cfg));
    if (cfg == NULL)
        return -1;
    cfg->ctx = drv;
    cfg->newlink_cb = hostapd_wireless_event_rtm_newlink;
    drv->netlink = 0;
    if (drv->netlink == NULL)
    {
        os_free(cfg);
    }

    return 0;
}


static void *hostap_init(struct hostapd_data *hapd,
                         struct wpa_init_params *params)
{
    struct hostap_driver_data *drv;
    int ret;

    drv = os_zalloc(sizeof(struct hostap_driver_data));
    if (drv == NULL) {
        WPA_LOGE("hostap_init oom\n");
        return NULL;
    }

    drv->hapd = hapd;
    drv->ioctl_sock = -1;
    drv->sock = -1;
    os_memcpy(drv->iface, params->ifname, sizeof(drv->iface));
    os_memcpy(drv->own_addr, params->own_addr, ETH_ALEN);

    ret = wpa_driver_hostap_init_vif(drv, NL80211_IFTYPE_AP);
    if(ret || (drv->vif_index == 0xff)) {
        WPA_LOGI("Could not found vif indix: %d, %d\n", drv->vif_index, ret);
        os_free(drv);
        return NULL;
    }

    drv->ioctl_sock = fsocket_init(PF_INET, SOCK_DGRAM, drv->vif_index);

    if (hostap_ioctl_prism2param(drv, PRISM2_PARAM_HOSTAPD, 1)) {
        wpa_printf(MSG_ERROR,
                   "Could not enable hostapd mode for interface %s",
                   drv->iface);
        fsocket_close(drv->ioctl_sock);

        os_free(drv);
		drv = NULL;

        return NULL;
    }

    ret = wpa_driver_hostap_start_apm(drv);
    if(ret) {
        WPA_LOGD("wpa_driver_hostap_start_apm failed\n");
        return NULL;
    }

    if (hostap_init_sockets(drv, params->own_addr) ||
            hostap_wireless_event_init(drv)) {
        fsocket_close(drv->ioctl_sock);

        os_free(drv);
		drv = NULL;

        return NULL;
    }

    return drv;
}


static void hostap_driver_deinit(void *priv)
{
    struct hostap_driver_data *drv = priv;

    (void) hostap_set_iface_flags(drv, 0);
    (void) hostap_ioctl_prism2param(drv, PRISM2_PARAM_HOSTAPD, 0);
    (void) hostap_ioctl_prism2param(drv, PRISM2_PARAM_HOSTAPD_STA, 0);

	if(wpa_driver_hostap_stop_apm(drv))
		WPA_LOGE("Could not stop apm vif: %d\n", drv->vif_index);

    if(wpa_driver_hostap_deinit_vif(drv))
        WPA_LOGE("Could not remove vif: %d\n", drv->vif_index);

    if (drv->ioctl_sock >= 0)
    {
        fsocket_close(drv->ioctl_sock);
    }

    if (drv->sock >= 0)
	{
	    eloop_unregister_read_sock(drv->sock);
        fsocket_close(drv->sock);
    }

	l2_packet_deinit(drv->sock_xmit);
	drv->sock_xmit = NULL;

    os_free(drv->generic_ie);
	drv->generic_ie = NULL;

    os_free(drv->wps_ie);
	drv->wps_ie = NULL;

    os_free(drv);
	drv = NULL;
}

static int hostap_sta_deauth(void *priv, const u8 *own_addr, const u8 *addr,
                             u16 reason)
{
    struct hostap_driver_data *drv = priv;
    struct ieee80211_mgmt mgmt;

    if (is_broadcast_ether_addr(addr))
    {
        /*
         * New Prism2.5/3 STA firmware versions seem to have issues
         * with this broadcast deauth frame. This gets the firmware in
         * odd state where nothing works correctly, so let's skip
         * sending this for the hostap driver.
         */
        return 0;
    }

    memset(&mgmt, 0, sizeof(mgmt));
    mgmt.frame_control = IEEE80211_FC(WLAN_FC_TYPE_MGMT,
                                      WLAN_FC_STYPE_DEAUTH);
    memcpy(mgmt.da, addr, ETH_ALEN);
    memcpy(mgmt.sa, own_addr, ETH_ALEN);
    memcpy(mgmt.bssid, own_addr, ETH_ALEN);
    mgmt.u.deauth.reason_code = host_to_le16(reason);
    return hostap_send_mlme(drv, (u8 *) &mgmt, IEEE80211_HDRLEN +
                            sizeof(mgmt.u.deauth), 0, 0, NULL, 0, 0, 0);
}


int hostap_channel_switch(void *priv, struct csa_settings *settings)
{
    struct hostap_driver_data *drv = priv;
    struct prism2_hostapd_param *param;
    u8 *buf;
    size_t blen;

    blen = sizeof(*param);
    buf = os_zalloc(blen);
    if(buf == NULL)
    {
        return -1;
    }

    param = (struct prism2_hostapd_param *)buf;
    param->cmd = PRISM2_HOSTAPD_CHANNEL_SWITCH;
    param->vif_idx = drv->vif_index;
    param->u.chan_switch.freq = settings->freq_params.freq;
    param->u.chan_switch.csa_cnt = settings->cs_count;
    param->u.chan_switch.settings = settings;

    if (hostapd_ioctl(drv, param, blen))
    {
    	os_free(buf);
        return -1;
    }

    os_free(buf);

    return 0;
}

static int hostap_set_freq(void *priv, struct hostapd_freq_params *freq)
{
    struct hostap_driver_data *drv = priv;
    struct iwreq iwr;

    os_memset(&iwr, 0, sizeof(iwr));
    os_strlcpy(iwr.ifr_name, drv->iface, IFNAMSIZ);
    iwr.u.freq.m = freq->channel;
    iwr.u.freq.e = 0;

    if (ioctl_inet(drv->ioctl_sock, drv->vif_index, SIOCSIWFREQ, (unsigned long)&iwr) < 0) {
        wpa_printf(MSG_ERROR, "ioctl[SIOCSIWFREQ]: %s",
                   strerror(errno));
        return -1;
    }

    return 0;
}

static int hostap_sta_disassoc(void *priv, const u8 *own_addr, const u8 *addr,
                               u16 reason)
{
    struct hostap_driver_data *drv = priv;
    struct ieee80211_mgmt mgmt;

    memset(&mgmt, 0, sizeof(mgmt));
    mgmt.frame_control = IEEE80211_FC(WLAN_FC_TYPE_MGMT,
                                      WLAN_FC_STYPE_DISASSOC);
    memcpy(mgmt.da, addr, ETH_ALEN);
    memcpy(mgmt.sa, own_addr, ETH_ALEN);
    memcpy(mgmt.bssid, own_addr, ETH_ALEN);
    mgmt.u.disassoc.reason_code = host_to_le16(reason);

    return  hostap_send_mlme(drv, (u8 *) &mgmt, IEEE80211_HDRLEN +
                             sizeof(mgmt.u.disassoc), 0, 0, NULL, 0, 0, 0);
}


#if CONFIG_WIFI_AP_HW_MODE
/* support multiple hw modes: B and G(N,AX) */
static struct hostapd_hw_modes *hostap_get_hw_feature_data(void *priv,
		u16 *num_modes, u16 *flags, u8 *dfs)
{
	struct hostapd_hw_modes *modes;
	struct hostapd_hw_modes *mode;
	int i, clen, rlen;
	const short chan2freq[14] = {
		2412, 2417, 2422, 2427, 2432, 2437, 2442,
		2447, 2452, 2457, 2462, 2467, 2472, 2484
	};

	/* two hw modes: b & g(n,ax) */
	modes = os_zalloc(sizeof(struct hostapd_hw_modes) * 2);
	if (!modes)
		return NULL;

	*num_modes = 2;
	*flags = 0;
	*dfs = 0;

	// 11B
	mode = modes;
	mode->num_channels = 14;
	mode->mode = HOSTAPD_MODE_IEEE80211B;
	mode->num_rates = 4;

	clen = mode->num_channels * sizeof(struct hostapd_channel_data);
	rlen = mode->num_rates * sizeof(int);

	mode->channels = os_zalloc(clen);
	mode->rates = os_zalloc(rlen);
	if (!mode->channels || !mode->rates)
		goto fail;

	/* channels */
	for (i = 0; i < 14; i++) {
		mode->channels[i].chan = i + 1;
		mode->channels[i].freq = chan2freq[i];
		mode->channels[i].allowed_bw = HOSTAPD_CHAN_WIDTH_20;
	}

	/* 11b rates */
	mode->rates[0] = 10;
	mode->rates[1] = 20;
	mode->rates[2] = 55;
	mode->rates[3] = 110;

	/* 11G/N/AX */
	mode++;
	mode->num_channels = 14;
	mode->mode = HOSTAPD_MODE_IEEE80211G;
	mode->num_rates = 12;

	clen = mode->num_channels * sizeof(struct hostapd_channel_data);
	rlen = mode->num_rates * sizeof(int);

	mode->channels = os_zalloc(clen);
	mode->rates = os_zalloc(rlen);
	if (!mode->channels || !mode->rates)
		goto fail;

	/* channels */
	for (i = 0; i < 14; i++) {
		mode->channels[i].chan = i + 1;
		mode->channels[i].freq = chan2freq[i];
		mode->channels[i].allowed_bw = HOSTAPD_CHAN_WIDTH_20;
	}

	/* rates */
	mode->rates[0] = 10;
	mode->rates[1] = 20;
	mode->rates[2] = 55;
	mode->rates[3] = 110;
	mode->rates[4] = 60;
	mode->rates[5] = 90;
	mode->rates[6] = 120;
	mode->rates[7] = 180;
	mode->rates[8] = 240;
	mode->rates[9] = 360;
	mode->rates[10] = 480;
	mode->rates[11] = 540;

	/* HT capa */
#if CONFIG_AP_HT_IE
	mode->mcs_set[0] = 0xff;
	mode->a_mpdu_params = 0;
	mode->ht_capab = HT_CAP_INFO_SHORT_GI20MHZ
					 | HT_CAP_INFO_TX_STBC
					 | HT_CAP_INFO_RX_STBC_1
					 | HT_CAP_INFO_MAX_AMSDU_SIZE;
#endif // CONFIG_AP_HT_IE

	return modes;

fail:
	/* free allocated data */
	for (mode = modes, i = 0; i < 2; i++, mode++) {
		if (mode->channels)
			os_free(mode->channels);
		if (mode->rates)
			os_free(mode->rates);
	}
	os_free(modes);

	return NULL;
}

#else /* CONFIG_WIFI_AP_HW_MODE */
static struct hostapd_hw_modes *hostap_get_hw_feature_data(void *priv,
				u16 *num_modes, u16 *flags, u8 *dfs)
{
	struct hostapd_hw_modes *modes; // all hw modes
	struct hostapd_hw_modes *mode;  // current hw mode
	int i, clen, rlen;
	const short chan2freq[] = {
		2412, 2417, 2422, 2427, 2432, 2437, 2442,
		2447, 2452, 2457, 2462, 2467, 2472, 2484
	};

#if CONFIG_AP_HE
	// FIXME: BK7236, fix hardcoded
	struct he_capabilities *he_capab;
	u8 he_mac_cap[HE_MAX_MAC_CAPAB_SIZE] = { 0x02, 0x00, 0x02, 0x00, 0x00, 0x00 };
#if CONFIG_RX_DCM_DISABLE
	//Close RX DCM
	u8 he_phy_cap[HE_MAX_PHY_CAPAB_SIZE] = { 0x02, 0xe0, 0x2b, 0x40, 0x0c, 0xc0, 0xef, 0x04, 0x53, 0xb0, 0x00 };
#else
	u8 he_phy_cap[HE_MAX_PHY_CAPAB_SIZE] = { 0x02, 0xe0, 0x2b, 0x78, 0x0c, 0xc0, 0xef, 0x04, 0x53, 0xb0, 0x00 };
#endif
	u8 he_mcs[HE_MAX_MCS_CAPAB_SIZE] = { 0xfd, 0xff, 0xfd, 0xff };
	u8 he_ppet[HE_MAX_PPET_CAPAB_SIZE] = { 0x18, 0x1c, 0x07 };
#endif

#if CONFIG_WIFI_BAND_5G || CONFIG_WIFI_BAND_6G
	*num_modes = 2;     // for 11A and 11G/11B
#else
	*num_modes = 1;     // for 11G/11B
#endif

	modes = os_zalloc(sizeof(struct hostapd_hw_modes) * (*num_modes));
	if (!modes) {
		*num_modes = 0;
		return NULL;
	}

	*flags = 0;
	*dfs = 0;

	// 11B or 11G
	mode = modes;
	mode->num_channels = ARRAY_SIZE(chan2freq);

#if CFG_SUPPORT_80211G
	mode->mode = HOSTAPD_MODE_IEEE80211G;
	mode->num_rates = 12;
#else
	mode->mode = HOSTAPD_MODE_IEEE80211B;
	mode->num_rates = 4;
#endif

	clen = mode->num_channels * sizeof(struct hostapd_channel_data);
	rlen = mode->num_rates * sizeof(int);

	mode->channels = os_zalloc(clen);
	mode->rates = os_zalloc(rlen);
	if (!mode->channels || !mode->rates)
		goto fail;

	for (i = 0; i < ARRAY_SIZE(chan2freq); i++) {
		mode->channels[i].chan = i + 1;
		mode->channels[i].freq = chan2freq[i];
		mode->channels[i].allowed_bw = HOSTAPD_CHAN_WIDTH_20;
	}

	mode->rates[0] = 10;
	mode->rates[1] = 20;
	mode->rates[2] = 55;
	mode->rates[3] = 110;

#if CFG_SUPPORT_80211G
	mode->rates[4] = 60;
	mode->rates[5] = 90;
	mode->rates[6] = 120;
	mode->rates[7] = 180;
	mode->rates[8] = 240;
	mode->rates[9] = 360;
	mode->rates[10] = 480;
	mode->rates[11] = 540;
#endif

#if CONFIG_AP_HT_IE
	mode->mcs_set[0] = 0xff;
	mode->a_mpdu_params = 0;
	mode->ht_capab = HT_CAP_INFO_SHORT_GI20MHZ |
					 HT_CAP_INFO_TX_STBC |
					 HT_CAP_INFO_RX_STBC_1 |
					 HT_CAP_INFO_MAX_AMSDU_SIZE;
#endif // CONFIG_AP_HT_IE

#if CONFIG_AP_VHT
	// TODO: BK7236, set vht cap
#endif

#if CONFIG_AP_HE
	// TODO: BK7236
	he_capab = &mode->he_capab[IEEE80211_MODE_AP];  // TODO: only set for softap
	he_capab->he_supported = 1;
	memcpy(he_capab->mac_cap, he_mac_cap, HE_MAX_MAC_CAPAB_SIZE);
	memcpy(he_capab->phy_cap, he_phy_cap, HE_MAX_PHY_CAPAB_SIZE);
	memcpy(he_capab->mcs, he_mcs, HE_MAX_MCS_CAPAB_SIZE);
	memcpy(he_capab->ppet, he_ppet, HE_MAX_PPET_CAPAB_SIZE);
#endif

#if CONFIG_WIFI_BAND_5G || CONFIG_WIFI_BAND_6G
	// -------------------------------------------------- //
	// Forward to next hw mode(11A)
	mode++;
	mode->num_channels = g_wiphy.bands[IEEE80211_BAND_5GHZ]->n_channels;
	mode->mode = HOSTAPD_MODE_IEEE80211A;
	mode->num_rates = 8;

	clen = mode->num_channels * sizeof(struct hostapd_channel_data);
	rlen = mode->num_rates * sizeof(int);

	mode->channels = os_zalloc(clen);
	mode->rates = os_zalloc(rlen);
	if (!mode->channels || !mode->rates)
		goto fail;

	/* channels */
	struct ieee80211_channel *channels = g_wiphy.bands[IEEE80211_BAND_5GHZ]->channels;
	for (i = 0; i < mode->num_channels; i++, channels++) {
		mode->channels[i].chan = channels->hw_value;
		mode->channels[i].freq = channels->center_freq;
		mode->channels[i].allowed_bw = HOSTAPD_CHAN_WIDTH_20 /* | HOSTAPD_CHAN_WIDTH_40P | HOSTAPD_CHAN_WIDTH_40M */;
		// WPA_LOGE("chan %d, freq %d\n", mode->channels[i].chan, mode->channels[i].freq);
	}

	/* rates */
	mode->rates[0] = 60;
	mode->rates[1] = 90;
	mode->rates[2] = 120;
	mode->rates[3] = 180;
	mode->rates[4] = 240;
	mode->rates[5] = 360;
	mode->rates[6] = 480;
	mode->rates[7] = 540;

	/* HT capa */
#if CONFIG_AP_HT_IE
	mode->mcs_set[0] = 0xff;
	mode->a_mpdu_params = 0;
	mode->ht_capab = HT_CAP_INFO_SHORT_GI20MHZ |
					 HT_CAP_INFO_TX_STBC |
					 HT_CAP_INFO_RX_STBC_1 |
					 HT_CAP_INFO_MAX_AMSDU_SIZE;
#endif // CONFIG_AP_HT_IE

#if CONFIG_AP_VHT
	mode->vht_capab = g_wiphy.bands[IEEE80211_BAND_5GHZ]->vht_cap.cap;
#endif

#if CONFIG_AP_HE
	he_capab = &mode->he_capab[IEEE80211_MODE_AP];  // TODO: only set for softap
	he_capab->he_supported = 1;
	memcpy(he_capab->mac_cap, he_mac_cap, HE_MAX_MAC_CAPAB_SIZE);
	memcpy(he_capab->phy_cap, he_phy_cap, HE_MAX_PHY_CAPAB_SIZE);
	memcpy(he_capab->mcs, he_mcs, HE_MAX_MCS_CAPAB_SIZE);
	memcpy(he_capab->ppet, he_ppet, HE_MAX_PPET_CAPAB_SIZE);
#endif

#endif // CONFIG_WIFI_BAND_5G || CONFIG_WIFI_BAND_6G

	return modes;

fail:
	/* free allocated data */
	for (mode = modes, i = 0; i < 2; i++, mode++) {
		if (mode->channels)
			os_free(mode->channels);
		if (mode->rates)
			os_free(mode->rates);
	}
	os_free(modes);

	return NULL;
}
#endif /* CONFIG_WIFI_AP_HW_MODE */

int hostap_set_ap(void *priv, struct wpa_driver_ap_params *params)
{
    struct hostap_driver_data *drv = priv;
    char *pos;
    char *beacon_buf;
    int bcn_len;
    int ret;
    struct prism2_hostapd_param param;
#ifdef CONFIG_P2P_GO
    int bcn_extra_size = 0, i;
#endif

    memset(&param, 0, sizeof(param));

#ifdef CONFIG_P2P_GO
    if (params->beacon_ies && params->beacon_ies->size > 0)
	bcn_extra_size = params->beacon_ies->size;
    bcn_len = params->head_len + params->tail_len + WLAN_EID_TIM_LEN + bcn_extra_size;
#else
    bcn_len = params->head_len + params->tail_len + WLAN_EID_TIM_LEN;
#endif
    beacon_buf = (char *)os_malloc(bcn_len);
    if(0 == beacon_buf)
    {
        return 0;
    }

    pos = beacon_buf;
    os_memcpy(pos, params->head, params->head_len);
    pos = pos + params->head_len;

    *pos++ = WLAN_EID_TIM;
    *pos++ = 4;
    *pos++ = 0;
    *pos++ = 1;//dtim_period = 1
    *pos++ = 0;
    *pos++ = 0;

    os_memcpy(pos, params->tail, params->tail_len);
    pos = pos + params->tail_len;

#ifdef CONFIG_P2P_GO
    if (bcn_extra_size > 0) {
	for (i = 0; i < bcn_extra_size; i ++)
		*pos++ = params->beacon_ies->buf[i];
	wpa_hexdump(MSG_DEBUG, "p2p bcn ie:", params->beacon_ies->buf, bcn_extra_size);
	wpa_hexdump(MSG_DEBUG, "bcn ie:", beacon_buf, bcn_len);
    }
#endif

    param.cmd = PRISM2_HOSTAPD_SET_AP_BCN;
    param.u.bcn_change.beacon = beacon_buf;
    param.u.bcn_change.bcn_len = bcn_len;
    param.u.bcn_change.head_len = params->head_len;
    param.u.bcn_change.tim_len = 6;
    param.vif_idx = drv->vif_index;

    ret = hostapd_ioctl(drv, &param, sizeof(param));

    os_free(beacon_buf);
    return ret;
}

static void hostap_poll_client_null_frame(void *priv, const u8 *own_addr,
        const u8 *addr, int qos)
{
    struct hostap_driver_data *drv = priv;
    struct prism2_hostapd_param param;

	param.cmd = PRISM2_HOSTAPD_POLL_CLIENT_NULL_DATA;
	param.u.poll_null_data.own_addr = (u8 *)own_addr;
	param.u.poll_null_data.sta_addr = (u8 *)addr;

	hostapd_ioctl(drv, &param, sizeof(param));

	return;
}

void wpa_driver_hostap_poll_client(void *priv, const u8 *own_addr,
        const u8 *addr, int qos)
{
    struct ieee80211_hdr hdr;

    os_memset(&hdr, 0, sizeof(hdr));

    /*
     * WLAN_FC_STYPE_NULLFUNC would be more appropriate,
     * but it is apparently not retried so TX Exc events
     * are not received for it.
     * This is the reason the driver overrides the default
     * handling.
     */
    hdr.frame_control = IEEE80211_FC(WLAN_FC_TYPE_DATA,
                                     WLAN_FC_STYPE_DATA);
    hdr.frame_control |= host_to_le16(WLAN_FC_FROMDS);

    os_memcpy(hdr.IEEE80211_DA_FROMDS, addr, ETH_ALEN);
    os_memcpy(hdr.IEEE80211_BSSID_FROMDS, own_addr, ETH_ALEN);
    os_memcpy(hdr.IEEE80211_SA_FROMDS, own_addr, ETH_ALEN);

    hostap_send_mlme(priv, (u8 *)&hdr, sizeof(hdr), 0, 0, NULL, 0, 0, 0);
}


//hostap_init_sockets
static void *wpa_driver_init(void *ctx, const char *ifname)
{
    struct hostap_driver_data *drv;
    int ret;
    int protocol = ETH_P_ALL;

    drv = os_zalloc(sizeof(struct hostap_driver_data));
    if (drv == NULL) {
        return NULL;
    }

    drv->wpa_s = ctx;
    os_memcpy(drv->iface, ifname, sizeof(drv->iface));
	bk_wifi_sta_get_mac((uint8_t *)drv->own_addr);

    ret = wpa_driver_hostap_init_vif(drv, NL80211_IFTYPE_STATION);
    if(ret || (drv->vif_index == 0xff)) {
        WPA_LOGE("Could not found vif indix: %d, %d\n", drv->vif_index, ret);
        os_free(drv);
        return NULL;
    }

    protocol += drv->vif_index;

    drv->ioctl_sock = fsocket_init(PF_INET, SOCK_DGRAM, drv->vif_index);
	drv->sock = fsocket_init(PF_PACKET, SOCK_RAW, protocol);

	// receive rx mgmt frames, or handle tx status
    if (eloop_register_read_sock(drv->sock, handle_read, drv, NULL))
    {
        //FIXME need to rollback and release all resources
        wpa_printf(MSG_ERROR, "Could not register read socket, memory leak!");
        os_free(drv);
        return NULL;
    }

    return drv;
}

static void wpa_driver_deinit(void *priv)
{
    struct hostap_driver_data *drv = priv;
	int retries = 0;

    (void) hostap_set_iface_flags(drv, 0);
    (void) hostap_ioctl_prism2param(drv, PRISM2_PARAM_HOSTAPD, 0);
    (void) hostap_ioctl_prism2param(drv, PRISM2_PARAM_HOSTAPD_STA, 0);

	/* wait hw cleanup complete */
	while (ke_state_get(TASK_SCANU) == SCANU_SCANNING) {
		rtos_delay_milliseconds(100);
		if(0 == retries)
			WPA_LOGI("wait scan complete\r\n");
		if (retries++ > 100)
			break;
	}
	WPA_LOGI("scanu completed\r\n");
    eloop_cancel_timeout(wpa_driver_scan_timeout, drv->wpa_s, NULL);

    if(wpa_driver_hostap_deinit_vif(drv))
        WPA_LOGE("Could not remove vif: %d\n", drv->vif_index);

    if (drv->ioctl_sock > 0)
    {
        fsocket_close(drv->ioctl_sock);
    }

    if (drv->sock > 0)
    {
        eloop_unregister_read_sock(drv->sock);
        fsocket_close(drv->sock);
    }

    os_free(drv->generic_ie);
	drv->generic_ie = NULL;

    os_free(drv->wps_ie);
	drv->wps_ie = NULL;

    os_free(drv);
	drv = NULL;
}

void wpa_handler_signal(void *arg, u8 vif_idx)
{
	int ret;
	int sig = (int)arg;

    if (!eloop_get_signal_count())
	{
		WPA_LOGE("wpa_handler_signal err failed\r\n");
        return;
	}

	if (!sig) {
		WPA_LOGE("%s: sig %d invalid\r\n", __func__, sig);
	}

	//WPA_LOGI("%s: sig %d\r\n", __func__, sig);
	ret = eloop_handle_signal(sig);
	if(ret)
	{
		WPA_LOGE("eloop_handle_signal failed: sig %d\r\n", sig);
		return;
	}

    wpa_hostapd_queue_poll((uint32_t)vif_idx);
}

/**
 * wpa_driver_scan_timeout - Scan timeout to report scan completion
 * @eloop_ctx: Driver private data
 * @timeout_ctx: ctx argument given to wpa_driver_scan_timeout()
 *
 * This function can be used as registered timeout when starting a scan to
 * generate a scan completed event if the driver does not report this.
 */
void wpa_driver_scan_timeout(void *eloop_ctx, void *timeout_ctx)
{
	wpa_printf(MSG_DEBUG, "nl80211: Scan timeout - try to abort it");
	wpa_printf(MSG_DEBUG, "nl80211: Try to get scan results");

	wpa_supplicant_event_sta(eloop_ctx, EVENT_SCAN_RESULTS, NULL);
}

int wpa_driver_scan2(void *priv, struct wpa_driver_scan_params *params)
{
	struct hostap_driver_data *drv = priv;
	struct prism2_hostapd_param *param;
	u8 *buf;
	size_t blen;
	int ret = 0, i;
	int *freqs;

	blen = sizeof(*param) + params->extra_ies_len;
	buf = os_zalloc(blen);
	if (buf == NULL)
		return WPA_ERR_NO_MEM;

	param = (struct prism2_hostapd_param *)buf;
	param->cmd = PRISM2_HOSTAPD_SCAN_REQ;
	param->u.scan_req.ssids_num = MIN(SCAN_SSID_MAX, params->num_ssids);
	if (params->extra_ies != NULL)
		os_memcpy(param->u.scan_req.extra_ies, params->extra_ies, params->extra_ies_len);
	param->u.scan_req.extra_ies_len = params->extra_ies_len;
	param->vif_idx = drv->vif_index;

	os_memcpy(param->u.scan_req.bssid,
		params->bssid ? params->bssid : broadcast_ether_addr, ETH_ALEN);
	for (i = 0; i < param->u.scan_req.ssids_num; i++) {
		param->u.scan_req.ssids[i].ssid_len = params->ssids[i].ssid_len;
		if (params->ssids[i].ssid) {
			os_memcpy(param->u.scan_req.ssids[i].ssid, params->ssids[i].ssid,
						param->u.scan_req.ssids[i].ssid_len);
		}
		if (bk_feature_bssid_connect_enable()) {
			if(params->bssid) {
				os_memcpy(param->u.scan_req.bssid, params->bssid, sizeof(param->u.scan_req.bssid));
			}
		}
		WPA_LOGD("%s: Scan specified BSSID " MACSTR  "\n", __func__, MAC2STR(param->u.scan_req.bssid));
	}

	freqs = params->freqs;
	if (freqs) {
		for (i = 0; i < ARRAY_SIZE(param->u.scan_req.freqs); i++, freqs++) {
			if (!*freqs)
				break;
			param->u.scan_req.freqs[i] = *freqs;
		}
	}

	if (hostapd_ioctl(drv, param, blen))
		ret = WPA_ERR_SCAN_REQ;

	os_free(buf);

	if (!ret && drv->wpa_s) {
		eloop_cancel_timeout(wpa_driver_scan_timeout, drv->wpa_s, NULL);
		eloop_register_timeout(10, 0, wpa_driver_scan_timeout,
				       drv->wpa_s, NULL);
	}

	return ret;
}


struct wpa_scan_results *wpa_driver_get_scan_results2(void *priv)
{
	struct hostap_driver_data *drv = priv;
	struct prism2_hostapd_param *param;
	struct wpa_scan_results *results = NULL;
	u8 *buf;
	size_t blen;
	int ret = 0;
	struct wpa_supplicant *wpa_s = drv->wpa_s;

	blen = sizeof(*param);
	buf = os_zalloc(blen);
	if(buf == NULL) {
		goto fail_result;
	}

	param = (struct prism2_hostapd_param *)buf;
	param->cmd = PRISM2_HOSTAPD_GET_SCAN_RESULT;
	param->vif_idx = drv->vif_index;
	if (wpa_s) {
		if (wpa_s->scan_res_handler == scan_only_handler)
			param->sta_addr[0] = 0xFF;
	}

	results = os_zalloc(sizeof(*results));
	if(results == NULL) {
		goto fail_result;
	}
	results->res = os_calloc(MAX_BSS_LIST, sizeof(struct wpa_scan_res *));
	if (results->res == NULL) {
		goto fail_result;
	}

	param->u.scan_rst = results;

	if (hostapd_ioctl(drv, param, blen)) {
		ret = -1;
	}

	if(!ret) {
		os_free(buf);
		return results;
	}

fail_result:
	os_free(buf);
	#if CONFIG_MINIMUM_SCAN_RESULTS
	wpa_hostapd_release_scan_rst();
	#endif
	wpa_scan_results_free(results);
	return NULL;
}

int wpa_driver_associate(void *priv, struct wpa_driver_associate_params *params)
{
	struct hostap_driver_data *drv = priv;
	struct prism2_hostapd_param *param;
	u8 *buf;
	size_t blen;
	int ret = 0;
	enum nl80211_iftype nlmode = NL80211_IFTYPE_UNSPECIFIED;

#ifdef CONFIG_P2P
	if (params->mode == IEEE80211_MODE_IBSS)
		/* Not supported, Current Support P2P Client, P2P GO, AP and STA */
		return -1;
	if (params->p2p) {
		if (params->mode == IEEE80211_MODE_AP) {
			nlmode = NL80211_IFTYPE_P2P_GO;
			/*just for go*/
			g_ap_param_ptr->cipher_suite = BK_SECURITY_TYPE_AUTO;
		} else {
			nlmode = NL80211_IFTYPE_P2P_CLIENT;
		}
	} else {
		if (params->mode == IEEE80211_MODE_AP)
			return -1;
		else
			nlmode = NL80211_IFTYPE_STATION;
	}

	if (wpa_driver_set_mode(drv, nlmode))
		return -1;

	if ((nlmode == NL80211_IFTYPE_P2P_GO) && wpa_driver_hostap_start_apm(drv))
		return -1;

#endif

	blen = sizeof(*param) + params->bcn_len;
	buf = os_zalloc(blen);
	if (buf == NULL)
		return -1;

	if (nlmode != NL80211_IFTYPE_P2P_GO) {
		WPA_LOGD("wpa_driver_associate: auth_alg 0x%x\r\n", params->auth_alg);
		param = (struct prism2_hostapd_param *)buf;
		param->cmd = PRISM2_HOSTAPD_ASSOC_REQ;
		param->vif_idx = drv->vif_index;

#ifdef CONFIG_IEEE80211R
		if (params->auth_alg & WPA_AUTH_ALG_FT)
			param->u.assoc_req.auth_alg = HOSTAP_AUTH_ALGO_FT;
		else
#endif
		if (params->auth_alg & WPA_AUTH_ALG_OPEN)
			param->u.assoc_req.auth_alg = HOSTAP_AUTH_OPEN;
		else if (params->auth_alg & WPA_AUTH_ALG_SHARED)
			param->u.assoc_req.auth_alg = HOSTAP_AUTH_SHARED;
		else if (params->auth_alg & WPA_AUTH_ALG_SAE)
			param->u.assoc_req.auth_alg = HOSTAP_AUTH_SAE;
		os_memcpy(param->u.assoc_req.bssid, params->bssid, ETH_ALEN);
		param->u.assoc_req.ssid_len = params->ssid_len;
		os_memcpy(param->u.assoc_req.ssid, params->ssid, param->u.assoc_req.ssid_len);
		param->u.assoc_req.proto = params->wpa_proto;
		param->u.assoc_req.key_mgmt_suite = params->key_mgmt_suite;
		param->u.assoc_req.pairwise_suite = params->pairwise_suite;
		param->u.assoc_req.group_suite = params->group_suite;
#ifdef CONFIG_IEEE80211R
		// Save target ap
		os_memcpy(drv->target_ap, params->bssid, ETH_ALEN);

		// use ft ie
		if ((params->auth_alg & WPA_AUTH_ALG_FT) && drv->ft_ie_len && drv->ft_ie) {
			param->u.assoc_req.ie_len = drv->ft_ie_len;
			os_memcpy((u8 *)param->u.assoc_req.ie_buf, drv->ft_ie, param->u.assoc_req.ie_len);
			os_free(drv->ft_ie);
			drv->ft_ie_len = 0;
			drv->ft_ie = NULL;
		} else
#endif
		{
			param->u.assoc_req.ie_len = params->wpa_ie_len;
			os_memcpy((u8 *)param->u.assoc_req.ie_buf, params->wpa_ie, param->u.assoc_req.ie_len);
		}
		param->u.assoc_req.mfp = params->mgmt_frame_protection;
		param->u.assoc_req.bcn_len = params->bcn_len;
#if defined(CONFIG_P2P) || CONFIG_WLAN_FAST_CONNECT_WITHOUT_SCAN
		param->u.assoc_req.freq = params->freq_hint;
#endif
		//WPA_LOGI("%s: bcn_len %d\n", __func__, params->bcn_len);
		if (params->bcn_len) {
			os_memcpy(param->u.assoc_req.bcn_buf, params->bcn_ie, params->bcn_len);
			//BK_ASSERT(sizeof(param->u.assoc_req.bcn_buf) >= params->bcn_len);
		}

#if CONFIG_WLAN_FAST_CONNECT_WITHOUT_SCAN
		param->u.assoc_req.rssi = params->rssi;
		param->u.assoc_req.cap_info = params->cap_info;
		param->u.assoc_req.beacon_period = params->beacon_int;
#endif

#if defined(CONFIG_IEEE80211R) || defined(CONFIG_WNM)
		// previous ssid
		if (params->prev_bssid)
			param->u.assoc_req.flags |= REASSOCIATION;
#ifdef CONFIG_IEEE80211R
		if ((param->u.assoc_req.auth_alg == HOSTAP_AUTH_ALGO_FT) && (drv->flags & RWNX_STA_FT_OVER_DS))
			param->u.assoc_req.flags |= (FT_OVER_DS | REASSOCIATION);
#endif
#endif
		if (hostapd_ioctl(drv, param, blen)) {
			ret = -1;
		} else {
			param->cmd = PRISM2_HOSTAPD_ASSOC_ACK;
			param->vif_idx = drv->vif_index;
			hostapd_ioctl(drv, param, blen);
		}
	}

	os_free(buf);

	return ret;
}


int wpa_driver_get_bssid(void *priv, u8 *bssid)
{
    struct hostap_driver_data *drv = priv;
    struct prism2_hostapd_param *param;
    u8 *buf;
    size_t blen;

    blen = sizeof(*param);
    buf = os_zalloc(blen);
    if(buf == NULL)
    {
        return -1;
    }

    param = (struct prism2_hostapd_param *)buf;
    param->cmd = PRISM2_HOSTAPD_GET_BSS_INFO;
    param->vif_idx = drv->vif_index;
    if (hostapd_ioctl(drv, param, blen))
    {
    	os_free(buf);
        return -1;
    }
    os_memcpy(bssid, param->u.bss_info.bssid, ETH_ALEN);
    os_free(buf);

    return 0;
}

int wpa_driver_get_ssid(void *priv, u8 *ssid)
{
    struct hostap_driver_data *drv = priv;
    struct prism2_hostapd_param *param;
    u8 *buf, len;
    size_t blen;

    blen = sizeof(*param);
    buf = os_zalloc(blen);
    if(buf == NULL)
    {
        return -1;
    }

    param = (struct prism2_hostapd_param *)buf;
    param->cmd = PRISM2_HOSTAPD_GET_BSS_INFO;
    param->vif_idx = drv->vif_index;
    if (hostapd_ioctl(drv, param, blen))
    {
    	os_free(buf);
        return -1;
    }

	len = MIN(SSID_MAX_LEN, os_strlen((char *)param->u.bss_info.ssid));
    os_memcpy(ssid, param->u.bss_info.ssid, len);
    os_free(buf);

    return len;
}

const u8 *wpa_driver_get_mac(void *priv)
{
    struct hostap_driver_data *drv = priv;

    return drv->own_addr;
}

uint64_t rwnx_hw_mm_features(void);

int wpa_driver_get_capa(void *priv, struct wpa_driver_capa *capa)
{
	struct hostap_driver_data *drv = priv;
	u64 hw_capa_flags = rwnx_hw_mm_features();

	os_memset(capa, 0, sizeof(*capa));

	capa->max_scan_ssids = WPAS_MAX_SCAN_SSIDS;
	capa->enc = WPA_DRIVER_CAPA_ENC_WEP40
				| WPA_DRIVER_CAPA_ENC_WEP104
				| WPA_DRIVER_CAPA_ENC_TKIP
				| WPA_DRIVER_CAPA_ENC_CCMP;
	capa->flags = WPA_DRIVER_FLAGS_AP_CSA;
#ifdef CONFIG_SME
	capa->flags |= WPA_DRIVER_FLAGS_SME;	/* supplicant handles SME */
#endif

#ifdef CONFIG_OCV
	capa->flags2 |= WPA_DRIVER_FLAGS2_OCV;
#endif

#if defined(CONFIG_SAE) || defined(CONFIG_SAE_AP)
	capa->flags |= WPA_DRIVER_FLAGS_SAE;	/* supplicant handles SAE */
#endif
#ifdef CONFIG_P2P
	capa->flags |= WPA_DRIVER_FLAGS_P2P_CAPABLE;
#ifdef CONFIG_P2P_GO
	capa->flags |= WPA_DRIVER_FLAGS_AP;
#endif
#endif
#ifdef CONFIG_IEEE80211R
	capa->flags |= WPA_DRIVER_FLAGS_UPDATE_FT_IES;
#endif
	if (hw_capa_flags & (1ULL << MM_FEAT_SM_FAST_CONNECT_BIT))
		capa->flags2 |= WPA_DRIVER_FLAGS2_SM_FAST_CONNECT;

	/* set extended capabilities, mask and length */
	drv->sta_ext_capa[0] = BIT(WLAN_EXT_CAPAB_EXT_CHAN_SWITCH);
#if CONFIG_IEEE80211N_HT40
	/* If 40M is enabled, always set 20/40 Coexistence */
	drv->sta_ext_capa[0] |= BIT(WLAN_EXT_CAPAB_20_40_COEX);
#endif
#if CONFIG_WIFI_FTM
#if CONFIG_WIFI_FTM_RESPONDER
	drv->sta_ext_capa[8] |= BIT(6);  // ftm responder
#endif
#if CONFIG_WIFI_FTM_INITIATOR
	drv->sta_ext_capa[8] |= BIT(7);  // ftm Initiator
#endif
#endif

	capa->extended_capa = drv->sta_ext_capa;
	capa->extended_capa_len = sizeof(drv->sta_ext_capa);
	capa->extended_capa_mask = drv->sta_ext_capa;

    return 0;
}

int wpa_driver_set_operstate(void *priv, int state)
{
    int ret = 0;

#ifdef CONFIG_SME
    struct hostap_driver_data *drv = priv;
    struct prism2_hostapd_param *param;
    u8 *buf;
    size_t blen;

    blen = sizeof(*param);
    buf = os_zalloc(blen);
    if(buf == NULL)
        return -1;

    param = (struct prism2_hostapd_param *)buf;
    param->cmd = PRISM2_HOSTAPD_SET_OPER_STATE;
	param->u.oper_state.state = state;
    param->vif_idx = drv->vif_index;
    if(hostapd_ioctl(drv, param, blen)) {
        ret = -1;
    }
    os_free(buf);
#endif

    return ret;
}

#ifdef CONFIG_SME
/**
 * authenticate - Request driver to authenticate
 * @priv: private driver interface data
 * @params: authentication parameters
 * Returns: 0 on success, -1 on failure
 *
 * This is an optional function that can be used with drivers that
 * support separate authentication and association steps, i.e., when
 * wpa_supplicant can act as the SME. If not implemented, associate()
 * function is expected to take care of IEEE 802.11 authentication,
 * too.
 */
int wpa_driver_authenticate(void *priv, struct wpa_driver_auth_params *params)
{
	struct hostap_driver_data *drv = priv;
	struct prism2_hostapd_param *param;
	u8 *buf;
	size_t blen;
	int ret = 0;

	blen = sizeof(*param) + params->auth_data_len;
	buf = os_zalloc(blen);
	if (buf == NULL) {
		return -1;
	}

#if 0
	// register callback
    param = (struct prism2_hostapd_param *)buf;
    param->vif_idx = drv->vif_index;
    param->cmd = PRISM2_HOSTAPD_REG_AUTH_CALLBACK;
    param->u.reg_assoc_cfm.cb = wpa_handler_signal;
    param->u.reg_assoc_cfm.arg = (void *)SIGASSOC;
    if (hostapd_ioctl(drv, param, blen))
    {
        ret = -1;
    }

	eloop_register_signal(SIGAUTH, wpa_driver_auth_sig_handler, drv->wpa_s);
#endif

	param = (struct prism2_hostapd_param *)buf;
	param->cmd = PRISM2_HOSTAPD_AUTHENTICATE;
	param->vif_idx = drv->vif_index;
	os_memcpy(param->u.authen_req.bssid, params->bssid, ETH_ALEN);
	os_memcpy(param->u.authen_req.ssid, params->ssid, params->ssid_len);
	param->u.authen_req.ssid_len = params->ssid_len;
	param->u.authen_req.freq = params->freq;
	param->u.authen_req.auth_alg = params->auth_alg;

	// IE
	param->u.authen_req.ie_len = params->ie_len;
	//param->u.authen_req.ie = (u8 *)(param + 1);
	if (params->ie_len)
		os_memcpy(param->u.authen_req.ie, params->ie, params->ie_len);

	// SAE Data
	param->u.authen_req.sae_data_len = params->auth_data_len;
	//param->u.authen_req.sae_data = param->u.authen_req.ie + params->ie_len;
	if (params->auth_data_len)
		os_memcpy(param->u.authen_req.sae_data, params->auth_data, params->auth_data_len);
	if (hostapd_ioctl(drv, param, blen))
		ret = -1;

	os_free(buf);

	return ret;
}
#endif

/**
 * send_action - Transmit an Action frame
 * @priv: Private driver interface data
 * @freq: Frequency (in MHz) of the channel
 * @wait: Time to wait off-channel for a response (in ms), or zero
 * @dst: Destination MAC address (Address 1)
 * @src: Source MAC address (Address 2)
 * @bssid: BSSID (Address 3)
 * @data: Frame body
 * @data_len: data length in octets
 * @no_cck: Whether CCK rates must not be used to transmit this frame
 * Returns: 0 on success, -1 on failure
 *
 * This command can be used to request the driver to transmit an action
 * frame to the specified destination.
 *
 * If the %WPA_DRIVER_FLAGS_OFFCHANNEL_TX flag is set, the frame will
 * be transmitted on the given channel and the device will wait for a
 * response on that channel for the given wait time.
 *
 * If the flag is not set, the wait time will be ignored. In this case,
 * if a remain-on-channel duration is in progress, the frame must be
 * transmitted on that channel; alternatively the frame may be sent on
 * the current operational channel (if in associated state in station
 * mode or while operating as an AP.)
 *
 * If @src differs from the device MAC address, use of a random
 * transmitter address is requested for this message exchange.
 */

int wpa_driver_send_action(void *priv, unsigned int freq, unsigned int wait,
			   const u8 *dst, const u8 *src, const u8 *bssid,
			   const u8 *data, size_t data_len, int no_cck)
{
    struct hostap_driver_data *drv = priv;
	int total_len;
	u8 *buf;
	char *payload;
	struct ieee80211_hdr *hdr;;
	S_TYPE_PTR type_ptr;
    int res;

	total_len = sizeof(struct ieee80211_hdr) + data_len;;

	buf = os_malloc(total_len);
	if (!buf)
		return -1;

	os_memset(buf, 0, total_len);

	hdr = (struct ieee80211_hdr *)buf;

	os_memcpy(hdr->addr1, dst, ETH_ALEN);
	os_memcpy(hdr->addr2, src, ETH_ALEN);
	os_memcpy(hdr->addr3, bssid, ETH_ALEN);
	hdr->frame_control = IEEE80211_FC(WLAN_FC_TYPE_MGMT, WLAN_FC_STYPE_ACTION);
	payload = (char *)(hdr + 1);
	os_memcpy(payload, data, data_len);

	wpa_hexdump(MSG_DEBUG, "ACTION: ", buf, total_len);

	//hostap_send_mlme
    type_ptr = os_zalloc(sizeof(S_TYPE_ST));
	if (!type_ptr)
		return -1;

    type_ptr->type = HOSTAPD_MGMT_ROBUST;
    type_ptr->vif_index = drv->vif_index;

    res = fsocket_send(drv->sock, buf, total_len, type_ptr);
	res = res > 0 ? 0 : -1; /* 0 on success, -1 on failure */

	os_free(buf);

    return res;
}

int wpa_driver_deauthenticate(void *priv, const u8 *addr, u16 reason_code)
{
    struct hostap_driver_data *drv = priv;
    struct prism2_hostapd_param *param;
    u8 *buf;
    size_t blen;
    int ret = 0;

    blen = sizeof(*param);
    buf = os_zalloc(blen);
    if(buf == NULL)
    {
        return -1;
    }

    param = (struct prism2_hostapd_param *)buf;
    param->cmd = PRISM2_HOSTAPD_DISCONN_REQ;
    param->vif_idx = drv->vif_index;
    param->u.disconnect_req.reason = reason_code;
    if (hostapd_ioctl(drv, param, blen))
    {
        ret = -1;
    }

    os_free(buf);

    return ret;
}

#ifdef CONFIG_IEEE80211R
// rwnx_cfg80211_update_ft_ies
int wpa_driver_update_ft_ies(void *priv, const u8 *md, const u8 *ies, size_t ies_len)
{
	struct hostap_driver_data *drv = priv;
	struct prism2_hostapd_param *param;
	u8 *buf;
	size_t blen;
	int ret = 0;

	// If RWNX_STA_FT_OVER_DS, send SM_CONNECT_REQ
	// If RWNX_STA_FT_OVER_AIR, send SM_FT_AUTH_RSP
	if (drv->ft_ie)
		os_free(drv->ft_ie);
	drv->ft_ie = os_malloc(ies_len);
	if (!drv->ft_ie)
		return -1;
	os_memcpy(drv->ft_ie, ies, ies_len);
	drv->ft_ie_len = ies_len;

	if (drv->flags & RWNX_STA_FT_OVER_AIR) {
		WPA_LOGD("%s: update ft ie for FT_OVER_AIR\n", __func__);
		drv->flags &= ~RWNX_STA_FT_OVER_AIR;

		blen = sizeof(*param);
		buf = os_zalloc(blen);
		if (!buf)
			return -1;

		param = (struct prism2_hostapd_param *)buf;
		param->cmd = PRISM2_HOSTAPD_UPDATE_FT_IES;
		param->vif_idx = drv->vif_index;

		param->u.assoc_req.ie_len = ies_len;
		os_memcpy((UINT8 *)param->u.assoc_req.ie_buf, ies, ies_len);

		if (hostapd_ioctl(drv, param, blen))
			ret = -1;

		os_free(buf);
	} else if (drv->flags & RWNX_STA_FT_OVER_DS) {
		/* FIXME: implement over-ds */
		ret = -1;
	}

	return ret;
}

int wpa_driver_handle_auth_response(void *priv, void *_data)
{
	struct hostap_driver_data *drv = priv;
	union wpa_event_data *data = _data;

	drv->flags |= RWNX_STA_FT_OVER_AIR;
	os_memcpy(data->ft_ies.target_ap, drv->target_ap, ETH_ALEN);

	return 0;
}
#endif

static int wpa_driver_signal_poll(void *priv, struct wpa_signal_info *si)
{
	struct hostap_driver_data *drv = priv;
	struct prism2_hostapd_param *param;

	u8 *buf;
	size_t blen;
	int ret = 0;
	blen = sizeof(*param);
	buf = os_zalloc(blen);
	if (buf == NULL)
		return -1;

	param = (struct prism2_hostapd_param *)buf;

	param->cmd = PRISM2_HOSTAPD_GET_SIGNAL_POLL;
	param->vif_idx = drv->vif_index;

	os_memset(si, 0, sizeof(*si));

	if (hostapd_ioctl(drv, param, blen))
		ret = -1;

	si->frequency = param->u.signal_info.frequency;
	si->current_signal = param->u.signal_info.current_signal;
	si->avg_signal = param->u.signal_info.avg_signal;
	si->current_noise = param->u.signal_info.current_noise;

	os_free(buf);

	return ret;
}

static int wpa_driver_signal_monitor(void *priv, int threshold, int hysteresis)
{
	struct hostap_driver_data *drv = priv;
	struct prism2_hostapd_param *param;

	u8 *buf;
	size_t blen;
	int ret = 0;
	blen = sizeof(*param);
	buf = os_zalloc(blen);
	if (buf == NULL)
		return -1;

	param = (struct prism2_hostapd_param *)buf;

	param->cmd = PRISM2_HOSTAPD_SIGNAL_MONITOR;
	param->vif_idx = drv->vif_index;
	param->u.signal_monitor.threshold = threshold;
	param->u.signal_monitor.hysteresis = hysteresis;

	if (hostapd_ioctl(drv, param, blen))
		ret = -1;

	os_free(buf);

	return ret;
}

#ifdef CONFIG_SAE_EXTERNAL
int wpa_driver_send_external_auth_status(void *priv,
		struct external_auth *params)
{
	struct hostap_driver_data *drv = priv;
	struct prism2_hostapd_param *param;
	u8 *buf;
	size_t blen;
	int ret = 0;

	/* External SAE only valid for STA. For softap, MLME/SME are all implemented in hapd */
	if (drv->hapd)
		return 0;

	blen = sizeof(*param);
	buf = os_zalloc(blen);
	if (buf == NULL)
		return -1;

	param = (struct prism2_hostapd_param *)buf;
	param->cmd = PRISM2_HOSTAPD_EXTERNAL_AUTH_STATUS;
	param->vif_idx = drv->vif_index;
	param->u.external_auth_status.status = params->status;
	if (hostapd_ioctl(drv, param, blen))
		ret = -1;

	os_free(buf);

	return ret;
}
#endif

int wpa_driver_get_channel_info(void *priv, struct wpa_channel_info *ci)
{
	u8 *buf;
	size_t blen;
	int ret = 0;
	struct prism2_hostapd_param *param;
	struct hostap_driver_data *drv = priv;

	blen = sizeof(*param);
	buf = os_zalloc(blen);
	if (buf == NULL)
		return -1;

	os_memset(ci, 0, sizeof(*ci));
	param = (struct prism2_hostapd_param *)buf;
	param->cmd = PRISM2_HOSTAPD_GET_CHANNEL_REQ;

	if (hostapd_ioctl(drv, param, blen)) {
		os_free(buf);
		return -1;
	}

	ci->center_frq1 = param->u.channel_info.center_frq1;
	ci->center_frq2 = param->u.channel_info.center_frq2;
	ci->chanwidth = param->u.channel_info.chanwidth;
	ci->frequency = param->u.channel_info.frequency;
	ci->sec_channel = param->u.channel_info.sec_channel;
	ci->seg1_idx = param->u.channel_info.seg1_idx;

	os_free(buf);

	return ret;
}

#ifdef CONFIG_P2P

int wpa_driver_probe_req_report(void *priv, int report)
{
	// FIXME: MAC HW may not be in IDLE state
	mm_rx_filter_app_set(NXMAC_ACCEPT_PROBE_REQ_BIT);

	return 0;
}

int wpa_driver_remain_on_channel(void *priv, unsigned int freq,
		unsigned int duration)
{
	struct hostap_driver_data *drv = priv;
	struct prism2_hostapd_param *param;
	u8 *buf;
	size_t blen;
	int ret = 0;

	blen = sizeof(*param);
	buf = os_zalloc(blen);
	if (buf == NULL)
		return -1;

	param = (struct prism2_hostapd_param *)buf;
	param->cmd = PRISM2_HOSTAPD_ROC;
	param->vif_idx = drv->vif_index;
	param->u.roc.freq = freq;
	param->u.roc.duration = duration;
	if (hostapd_ioctl(drv, param, blen))
		ret = -1;

	os_free(buf);

	return ret;
}

int wpa_driver_remain_cancel_on_channel(void *priv)
{
	struct hostap_driver_data *drv = priv;
	struct prism2_hostapd_param *param;
	u8 *buf;
	size_t blen;
	int ret = 0;

	blen = sizeof(*param);
	buf = os_zalloc(blen);
	if (buf == NULL)
		return -1;

	param = (struct prism2_hostapd_param *)buf;
	param->cmd = PRISM2_HOSTAPD_CANCEL_ROC;
	param->vif_idx = drv->vif_index;
	if (hostapd_ioctl(drv, param, blen))
		ret = -1;

	os_free(buf);

	return ret;
}

int is_ap_interface(enum nl80211_iftype nlmode)
{
	return nlmode == NL80211_IFTYPE_AP ||
		nlmode == NL80211_IFTYPE_P2P_GO;
}

int wpa_driver_nl80211_deinit_ap(void *priv)
{
	struct hostap_driver_data *drv = priv;

	if (!is_ap_interface(drv->nlmode))
		return -1;
	wpa_driver_hostap_stop_apm(drv);

	return wpa_driver_set_mode(drv, NL80211_IFTYPE_STATION);
}

#endif

const struct wpa_driver_ops wpa_driver_hostap_ops =
{
    .name = "hostap_beken",
    .desc = "Host AP driver",

    .hapd_init = hostap_init,
    .hapd_deinit = hostap_driver_deinit,
    .set_ieee8021x = hostap_set_ieee8021x,
    .set_privacy = hostap_set_privacy,
    .get_seqnum = hostap_get_seqnum,
    .flush = hostap_flush,
    .set_generic_elem = hostap_set_generic_elem,
    .read_sta_data = hostap_read_sta_data,
    .hapd_send_eapol = hostap_send_eapol,
    .sta_set_flags = hostap_sta_set_flags,
    .sta_deauth = hostap_sta_deauth,
    .sta_disassoc = hostap_sta_disassoc,
    .sta_remove = hostap_sta_remove,
    .hapd_set_ssid = hostap_set_ssid,
    .send_mlme = hostap_send_mlme,
    .sta_add = hostap_sta_add,
    .get_inact_sec = hostap_get_inact_sec,
    .sta_clear_stats = hostap_sta_clear_stats,
    .get_hw_feature_data = hostap_get_hw_feature_data,
#ifdef ANDROID_P2P
    .set_ap_wps_ie = hostap_set_ap_wps_ie,
#endif
    .set_freq = hostap_set_freq,
    .switch_channel = hostap_channel_switch,

    .set_key = wpa_driver_hostap_set_key,
#ifdef CONFIG_OCV
    .channel_info = wpa_driver_get_channel_info,
#endif
    .poll_client = hostap_poll_client_null_frame,
    .set_ap = hostap_set_ap,
    .init = wpa_driver_init,
    .deinit = wpa_driver_deinit,
    .scan2 = wpa_driver_scan2,
    .get_scan_results2 = wpa_driver_get_scan_results2,
#ifdef CONFIG_SME
	.authenticate = wpa_driver_authenticate,
#endif
	.send_action = wpa_driver_send_action,
    .deauthenticate = wpa_driver_deauthenticate,
    .associate = wpa_driver_associate,
#ifdef CONFIG_IEEE80211R
    .update_ft_ies = wpa_driver_update_ft_ies,
    .handle_ft_response = wpa_driver_handle_auth_response,
#endif
    .get_bssid = wpa_driver_get_bssid,
    .get_ssid = wpa_driver_get_ssid,
    .get_mac_addr = wpa_driver_get_mac,
    .get_capa = wpa_driver_get_capa,
    .set_operstate = wpa_driver_set_operstate,
#ifdef CONFIG_SAE_EXTERNAL
    .send_external_auth_status  = wpa_driver_send_external_auth_status,
#endif

	.signal_monitor = wpa_driver_signal_monitor,
	.signal_poll = wpa_driver_signal_poll,

#ifdef CONFIG_P2P
	.probe_req_report = wpa_driver_probe_req_report,
	.remain_on_channel = wpa_driver_remain_on_channel,
	.cancel_remain_on_channel = wpa_driver_remain_cancel_on_channel,
	.deinit_ap = wpa_driver_nl80211_deinit_ap,
#endif
};
