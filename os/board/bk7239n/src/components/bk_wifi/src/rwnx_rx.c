#include <common/bk_include.h>

#include "stdio.h"
#include "string.h"
#include "rwnx_config.h"
#include "fhost_msg.h"
#include "rwnx_rx.h"
#include "rw_msdu.h"
#include "../wpa_supplicant-2.10/bk_patch/sk_intf.h"
#include "rwnx_defs.h"
#include "../wpa_supplicant-2.10/hostapd/main_none.h"
#include "../wpa_supplicant-2.10/src/common/wpa_ctrl.h"
#include "../wpa_supplicant-2.10/wpa_supplicant/ctrl_iface.h"

#if CONFIG_AP_PROBEREQ_CB
#include "rw_msg_rx.h"
#endif
#if 1//CONFIG_RWNX_PROTO_DEBUG
#include "rwm_proto.h"
#endif
#include "rwnx_misc.h"
#include "lwip/pbuf.h"
#include "../wpa_supplicant-2.10/src/common/ieee802_11_defs.h"
#include "bk_phy_internal.h"
//#include "prot/ethernet.h"
#include <os/str.h>
#include <lwip/def.h>
#include "net/ethernet.h"  //Tizen Lwip

#include "bk_lwipif_tizenrt.h"

void ethernetif_input(int iface, struct pbuf *p);
wifi_monitor_cb_t wifi_monitor_get_cb(void);
#define LLC_ETHERTYPE_IPX            0x8137

/// Structure of A-MSDU Header
struct amsdu_hdr
{
    /// Destination Address
    struct eth_addr dest;
    /// Source Address
    struct eth_addr src;
    /// Length
    uint16_t len;
} __PACKED;

/// LLC/SNAP structure
struct llc_snap_short
{
    /// DSAP + SSAP fieldsr
    uint16_t dsap_ssap;
    /// LLC control + OUI byte 0
    uint16_t control_oui0;
    /// OUI bytes 1 and 2
    uint16_t oui1_2;
};

/// LLC/SNAP structure
struct llc_snap
{
    /// DSAP + SSAP fieldsr
    uint16_t dsap_ssap;
    /// LLC control + OUI byte 0
    uint16_t control_oui0;
    /// OUI bytes 1 and 2
    uint16_t oui1_2;
    /// Protocol
    uint16_t proto_id;
};

extern struct llc_snap_short llc_rfc1042_hdr;
extern struct llc_snap_short llc_bridge_tunnel_hdr;

#define RWM_MAC_ADDR_IS_BSCT(addr1_ptr)                                                     \
    ((*(((uint8_t*)(addr1_ptr)) + 0) == 0xFF) &&                                        \
     (*(((uint8_t*)(addr1_ptr)) + 1) == 0xFF) &&                                        \
     (*(((uint8_t*)(addr1_ptr)) + 2) == 0xFF) &&                                        \
     (*(((uint8_t*)(addr1_ptr)) + 3) == 0xFF) &&                                        \
     (*(((uint8_t*)(addr1_ptr)) + 4) == 0xFF) &&                                        \
     (*(((uint8_t*)(addr1_ptr)) + 5) == 0xFF))

#define RWM_MAC_ADDR_IS_MULTI(addr1_ptr)                                                    \
    ((*(((uint8_t*)(addr1_ptr)) + 0) == 0x01) &&                                        \
     (*(((uint8_t*)(addr1_ptr)) + 1) == 0x00) &&                                        \
     (*(((uint8_t*)(addr1_ptr)) + 2) == 0x5E))

void ethernetif_input(int iface, struct pbuf *p);

static bool rwm_udp_filter(struct ipv4_hdr *ip_hdr)
{
	char *udp_tmp = (char *)ip_hdr + IP_HDR_LEN;
	struct udphdr *udphdr = (struct udphdr *)udp_tmp;

	if (((htons(DHCP_SPORT(udphdr)) == 68) && (htons(DHCP_DPORT(udphdr)) == 67)) ||
		((htons(DHCP_SPORT(udphdr)) == 67) && (htons(DHCP_DPORT(udphdr)) == 68))) {
		/*dhcp message of udp*/
		return true;
	} else {
		return false;
	}
}

static bool rwm_ip_filter(struct ethhdr *eth_hdr_ptr)
{
	bool finded = false;
	char *tmp = (char *)eth_hdr_ptr + ETHDR_LEN;
	struct ipv4_hdr *iphdr = (struct ipv4_hdr *)tmp;

	switch (IP4H_PROTO(iphdr)) {
	case 17:	 //IP_PROTO_UDP
		finded =  rwm_udp_filter(iphdr);
		break;
	default:	 //UNKNOW TYPE
		break;
	}
	return finded;
}

static bool rwm_proto_filter(struct ethhdr *eth_hdr_ptr)
{
	bool finded = false;
	switch (htons(eth_hdr_ptr->proto)) {
	case 0x0800U:	 //ETHTYPE_IP
		finded = rwm_ip_filter(eth_hdr_ptr);
		break;
	case 0x0806U:	 //ETHTYPE_ARP
		finded = true;
		break;
	default:
		break;
	}
	return finded;
}

static bool rwm_block_bcmc(struct ethhdr *eth_hdr_ptr,uint8_t type)
{
	bool is_multi = RWM_MAC_ADDR_IS_MULTI(eth_hdr_ptr->dest);
	bool is_bsct = RWM_MAC_ADDR_IS_BSCT(eth_hdr_ptr->dest);

	switch(type)
	{
		case WIFI_MC_ON:
			if(is_multi && (!rwm_proto_filter(eth_hdr_ptr)))
			{
				return true;
			}
			break;
		case WIFI_BC_ON:
			if(is_bsct && (!rwm_proto_filter(eth_hdr_ptr)))
			{
				return true;
			}
			break;
		case WIFI_BC_MC_ON:
			if((is_bsct || is_multi) && (!rwm_proto_filter(eth_hdr_ptr)))
			{
				return true;
			}
			break;

		default:
			break;
		
	}
	return false;
}

void fhost_msdu_init(void)
{
}

__IRAM2 void rwm_check_rx_header_pattern(void *host_id)
{
	struct pbuf *p = NULL;
	struct fhost_rx_header *rxhdr = NULL;

	if (!rwm_check_rx_header_pattern_enabled())
		return;

	if (!host_id)
	{
		RWNX_LOGE("host_id is NULL\n");
		BK_ASSERT(0);
	}

	p = (struct pbuf *)host_id;
#if BK_SS_WIFI_DP
	rxhdr = (struct fhost_rx_header *)(*(uint32_t*)p->payload);
#else
	rxhdr = (struct fhost_rx_header *)p->payload;
#endif

	if (!rxhdr)
	{
		RWNX_LOGE("rxhdr is NULL\n");
		BK_ASSERT(0);
	}

	if (rxhdr->pattern != DMA_HD_RXPATTERN)
	{
		RWNX_LOGE("pattern:%x, offsetof %d\n", rxhdr->pattern, offsetof(struct fhost_rx_header, pattern));
		BK_ASSERT(0);
	}

	if (rxhdr->flags_is_amsdu)
	{
		RWNX_LOGD("amsdu_pattern:%x\n", rxhdr->amsdu_pattern);
		BK_ASSERT(rxhdr->amsdu_pattern == RX_AMSDU_RXPATTERN);
	}
}

/* Get RX Buffer */
#if BK_SS_WIFI_DP
__IRAM2 UINT32 rwm_get_rx_free_node(uint32_t *host_id, int len)
{
	uint32_t buf_addr;
	struct pbuf *p;
	void* fhost_rx_hdr = NULL;
	pbuf_type type = PBUF_POOL;

	p = pbuf_alloc(PBUF_RAW, len, type);
	if (p) 
	{
		*host_id = (uint32_t)p;
		buf_addr = (uint32_t)(p->payload);
		fhost_rx_hdr = os_zalloc_sram(macif_get_rxl_hdr_offset());
		if(fhost_rx_hdr != NULL)
		{
			*((uint32_t*)buf_addr) = (uint32_t)fhost_rx_hdr;
		}
		else
		{
			pbuf_free(p);
			*host_id = 0;
			buf_addr = 0;
		}
	}
	else 
	{
		*host_id = 0;
		buf_addr = 0;
	}

	//if (!buf_addr)
	//    RWNX_LOGE("%s: xxxxxxxxxx oom\n", __func__);
	return buf_addr;
}
#else
__IRAM2 UINT32 rwm_get_rx_free_node(uint32_t *host_id, int len)
{
	uint32_t buf_addr;
	struct pbuf *p;
#if MEM_TRX_DYNAMIC_EN
	pbuf_type type = PBUF_RAM_RX;
#else
	pbuf_type type = PBUF_POOL;

	/*
	 * When requested pbuf type is PBUF_POOL, pbuf_alloc will return a valid memory address even the
	 * requested length is greater than PBUF_POOL_BUFSIZE, and it will lead to memory OVERFLOW when
	 * memory is free, so a condition is added to ensure do NOT alloc buffer from PBUF pool if the
	 * requested length is greater than PBUF_POOL_BUFSIZE
	 */
	if (len > PBUF_POOL_BUFSIZE)
	{
		type = PBUF_RAM;
	}
#endif
	p = pbuf_alloc(PBUF_RAW, len, type);
	if (p) {
		*host_id = (uint32_t)p;
		buf_addr = (uint32_t)(p->payload);
	} else {
		*host_id = 0;
		buf_addr = 0;
	}

	//if (!buf_addr)
	//    RWNX_LOGE("%s: xxxxxxxxxx oom\n", __func__);
	return buf_addr;
}
#endif
/**
 * rwnx_rx_mgmt - Process one 802.11 management frame
 *
 * @pbuf: pbuf received
 * @rxhdr: HW rx descriptor
 *
 * return: dont_free: 0: need free pbuf, 1: dont free pbuf
 *
 * Process the management frame and free the corresponding skb.
 * If vif is not specified in the rx descriptor, the the frame is uploaded
 * on all active vifs.
 */
static int rwnx_rx_mgmt(struct pbuf *p, struct fhost_rx_header *rxhdr, int vif_idx)
{
	struct ieee80211_mgmt *mgmt = (struct ieee80211_mgmt *)p->payload;
#if (CONFIG_SOC_BK7236XX) || CONFIG_SOC_BK7239XX || CONFIG_SOC_BK7286XX
	void *vif;

	if (vif_idx >= NX_VIRT_DEV_MAX)
		return 0;

	vif = mac_vif_mgmt_get_entry(vif_idx);
#endif

	if (ieee80211_is_beacon(mgmt->frame_control)) {
#if (CONFIG_SOC_BK7236XX) || CONFIG_SOC_BK7239XX || CONFIG_SOC_BK7286XX // FIXME: bk7236, small memory now (need to enable other memory section)
		// cfg80211_report_obss_beacon
		if (mac_vif_mgmt_get_type(vif) == VIF_STA)
			return 0;
#endif
		if (!is_wpah_queue_full())
			ke_mgmt_packet_tx(p->payload, p->len, vif_idx);
		return 0;
	} else if ((ieee80211_is_deauth(mgmt->frame_control) ||
				ieee80211_is_disassoc(mgmt->frame_control)) &&
			   (mgmt->u.deauth.reason_code == WLAN_REASON_CLASS2_FRAME_FROM_NONAUTH_STA ||
				mgmt->u.deauth.reason_code == WLAN_REASON_CLASS3_FRAME_FROM_NONASSOC_STA)) {
		int ret;

		pbuf_ref(p);

		if (mgmt->u.deauth.reason_code == WLAN_REASON_CLASS2_FRAME_FROM_NONAUTH_STA)
			ret = wpa_ctrl_event(WPA_CTRL_EVENT_UNPROT_DEAUTHENTICATE, p);
		else
			ret = wpa_ctrl_event(WPA_CTRL_EVENT_UNPROT_DISASSOCIATE, p);

		if (ret != BK_OK)
			pbuf_free(p);

		return 0;
	} else {
#if CONFIG_AP_PROBEREQ_CB
		if (ieee80211_is_probe_req(mgmt->frame_control))
			ap_vif_probe_req_frame_cb_hander(p->payload, p->len);
#endif

#if (CONFIG_SOC_BK7236XX) || CONFIG_SOC_BK7239XX || CONFIG_SOC_BK7286XX
		// FIXME: bk7236, don't pass probe req to wpa_s unless CONFIG_P2P is enabled
		if (mac_vif_mgmt_get_type(vif) == VIF_STA) {
#if !CONFIG_P2P
			if (ieee80211_is_probe_req(mgmt->frame_control) ||
				ieee80211_is_probe_resp(mgmt->frame_control)) {
				return 0;
			}
#endif
		}
#endif
		if (!is_wpah_queue_full())
			ke_mgmt_packet_tx(p->payload, p->len, vif_idx);

		return 0;
	}
}

static int rwnx_get_specific_vif_id(int vif_type)
{
	void *vif;

	for (int i = 0; i < NX_VIRT_DEV_MAX; i++)  {
		vif = mac_vif_mgmt_get_entry(i);
		if (mac_vif_mgmt_get_type(vif) == vif_type)
			return mac_vif_mgmt_get_index(vif);
	}

	return INVALID_VIF_IDX;
}

#if CONFIG_P2P
static int rwnx_get_probe_req_vif_id( )
{
	struct vif_info_tag *vif;

	for (int i = 0; i < NX_VIRT_DEV_MAX; i++){
		vif = &vif_info_tab[i];
		if ((VIF_STA == vif->type) && (vif->p2p_rec_probe_req))
			return vif->index;
	}

	for (int i = 0; i < NX_VIRT_DEV_MAX; i++){
		vif = &vif_info_tab[i];
		if (VIF_AP == vif->type)
			return vif->index;
	}

	return INVALID_VIF_IDX;
}
#endif

/**
 * rwnx_rx_mgmt_any - Process one 802.11 management frame
 *
 * @rwnx_hw: main driver data
 * @skb: skb received
 * @rxhdr: HW rx descriptor
 *
 * Process the management frame and free the corresponding skb.
 * If vif is not specified in the rx descriptor, the the frame is uploaded
 * on all active vifs.
 */
static void rwnx_rx_mgmt_any(struct pbuf *p, struct fhost_rx_header *rxhdr)
{
	int vif_idx = rxhdr->flags_vif_idx;

	if (vif_idx == INVALID_VIF_IDX) {
		struct ieee80211_mgmt *hdr = (struct ieee80211_mgmt *)p->payload;
		int dest_vif;

		/* Ways to mitigate forwarding too many pkts to supplicant:
		 *
		 * 1. Forward beacon/probe response to STA only
		 * 2. Forware probe request to AP only
		 *
		 * TODO - flow control for pkts to supplicant
		 **/
		if (ieee80211_is_beacon(hdr->frame_control) || ieee80211_is_probe_resp(hdr->frame_control))
			dest_vif = rwnx_get_specific_vif_id(VIF_STA);
		else if (ieee80211_is_probe_req(hdr->frame_control))
#if CONFIG_P2P
			dest_vif = rwnx_get_probe_req_vif_id();
#else
			dest_vif = rwnx_get_specific_vif_id(VIF_AP);
#endif
		else
			dest_vif = INVALID_VIF_IDX;

		if (dest_vif != INVALID_VIF_IDX)
			rwnx_rx_mgmt(p, rxhdr, dest_vif);
	} else {
		rwnx_rx_mgmt(p, rxhdr, vif_idx);
	}

	pbuf_free(p);
}

void rwnx_amsdu_mac2eth_update(struct pbuf *p)
{
	struct mac_addr temp_mac;
	struct llc_snap *llc_snap;
	struct eth_hdr *eth_hdr;
	struct amsdu_hdr *amsdu_subfrm_hdr = (struct amsdu_hdr *)p->payload;

	#if 0//(RW_MESH_EN)
	VIF_INF_PTR p_vif_entry = rwm_mgmt_vif_idx2ptr(rxhdr->flags_vif_idx);
	#endif //(RW_MESH_EN)

	/*
	 * format of p->payload
	 * 1 MESH type
	 ****************************************************
	 *  |  DA  |  SA  |  LENGTH  |  MESH_CONTROL  |  DATA  |  PADDING  |
	 ****************************************************
	 * 2 SNAP type
	 ***********************************************************************
	 *  |  DA  |  SA  |  LENGTH  |  LLC/SNAP  |  DATA  |  PADDING  |
	 ***********************************************************************
	 * 3 RAW type (should not happen actually)
	 ****************************************************
	 *  |  DA  |  SA  |  LENGTH  |  DATA  |  PADDING  |
	 ****************************************************
	 */

	//RWNX_LOGI("%s amsdu_len=%d\n", __FUNCTION__, p->len);

	//ieee802.11 amsdu_hdr to ieee802.3 mac_eth_hdr
#if 0//(RW_MESH_EN)
	if ((p_vif_entry->type == VIF_MESH_POINT) && (rxhdr->flags_dst_idx != INVALID_STA_IDX)) {
		/*
		    ****************************************************
		    *  |  DA  |  SA  |  LENGTH  |  MESH_CONTROL  |  LLC/SNAP    |  DATA  |  PADDING  |
		    ****************************************************
		    * ==>
		    ****************************************************
		    *  |  DA  |  SA  |  ETHERTYPE  |  MESH_CONTROL  |  LLC/SNAP    |  DATA  |  PADDING  |
		    ****************************************************
		    *  Keep subframe as mesh frame, since amsdu_hdr=mac_eth_hdr
		    *  set eth_hdr->len as ethertype like rxu_cntrl_mac2eth_update
		    */
		eth_hdr = (struct mac_eth_hdr *)amsdu_subfrm_hdr;
		llc_snap = (struct llc_snap *)((uint8_t *)amsdu_subfrm_hdr + sizeof(struct mac_eth_hdr) + rxhdr->mesh_ctrl_len);
		eth_hdr->len = llc_snap->proto_id;
	} else
#endif //(RW_MESH_EN)

	llc_snap = (struct llc_snap *)(amsdu_subfrm_hdr + 1);

	if ((!memcmp(llc_snap, &llc_rfc1042_hdr, sizeof(llc_rfc1042_hdr))
		 //&& (llc_snap->ether_type != RX_ETH_PROT_ID_AARP) - Appletalk depracated ?
		 && (llc_snap->proto_id != LLC_ETHERTYPE_IPX))
		|| (!memcmp(llc_snap, &llc_bridge_tunnel_hdr, sizeof(llc_bridge_tunnel_hdr)))) {
		/*
			****************************************************
			*  |  DA  |  SA  |	LENGTH	|  LLC/SNAP  |	DATA  |  PADDING  |
			****************************************************
			* ==>
			****************************************************
			*  |  DA  |  SA  |	DATA  |  PADDING  |
			****************************************************
			*/
		eth_hdr = (struct eth_hdr *)((uint8_t *)amsdu_subfrm_hdr + sizeof(struct llc_snap_short) + sizeof(amsdu_subfrm_hdr->len));
		os_memcpy(&eth_hdr->src, &amsdu_subfrm_hdr->src, ETH_ALEN);
		os_memcpy(&eth_hdr->dest, &amsdu_subfrm_hdr->dest, ETH_ALEN);

		pbuf_header(p, -(s16)(sizeof(struct llc_snap_short) + sizeof(amsdu_subfrm_hdr->len)));	// skip off
	} else {
		/*
			****************************************************
			*  |  DA  |  SA  |	LENGTH	|  DATA  |	PADDING  |
			****************************************************
			* ==>
			****************************************************
			*  |  DA  |  SA  |	DATA  |  PADDING  |
			****************************************************
			*/
		eth_hdr = (struct eth_hdr *)((uint8_t *)amsdu_subfrm_hdr + sizeof(amsdu_subfrm_hdr->len));
		os_memcpy(&temp_mac, &amsdu_subfrm_hdr->src, ETH_ALEN);
		os_memcpy(&eth_hdr->src, &temp_mac, ETH_ALEN);
		os_memcpy(&temp_mac, &amsdu_subfrm_hdr->dest, ETH_ALEN);
		os_memcpy(&eth_hdr->dest, &temp_mac, ETH_ALEN);

		pbuf_header(p, -(s16)sizeof(amsdu_subfrm_hdr->len));	// skip off
	}

}

/**
 ****************************************************************************************
 * @brief process all the A-MSDU subframes
 * @param[in] rxhdr - the fhost rx header which contains all the address of subframes in
 *                    A-MSDU, the address of subframes are stored in rxhdr->amsdu_hostids
 *
 * BK A-MSDU buffer allocation strategy:
 *
 * To save memory usage, the RX buffer is only malloced with known size after the length
 * of each subframe in A-MSDU is parsed.
 *
 * amsdu_hostids[0] contains fhost_rx_header and the payload of first subframe in A-MSDU,
 * amsdu_hostids[N] contains only the payload of subframe(#n+1) in A-MSDU, which N meets
 * the condition as follow:(N != 0 && N < NX_MAX_MSDU_PER_RX_AMSDU).
 ****************************************************************************************
 */
void rwnx_upload_amsdu(struct fhost_rx_header *rxhdr)
{
	// Iterate sub-MSDU of A-MSDU
	int i = 0;
	uint32_t amsdu_hostids[NX_MAX_MSDU_PER_RX_AMSDU];
	int iface = 0;
	struct pbuf *q;

	os_memcpy(amsdu_hostids, rxhdr->amsdu_hostids, sizeof(amsdu_hostids));
	iface = rxhdr->flags_vif_idx;

	// upload the other sub-MSDUs of A-MSDU
	q = (struct pbuf *)(amsdu_hostids[i]);

	while (q && i < NX_MAX_MSDU_PER_RX_AMSDU) {
#if BK_SS_WIFI_DP
		uint32_t rxhdr_addr = (*((uint32_t*)q->payload));
		pbuf_header(q, -(s16)macif_get_rxl_payload_offset());
#endif
		#if NX_VERSION > NX_VERSION_PACK(6, 22, 0, 0)
		/* preprocess for pbuf */
		rwnx_rx_preprocess(iface, q);
		#endif
		/* upload to tcp/ip stack */
		ethernetif_input(iface, q);
#if BK_SS_WIFI_DP
		if(i != 0) os_free((void *)rxhdr_addr);
#endif
		/* get the next sub-MSDU */
		q = (struct pbuf *)(amsdu_hostids[++i]);
	}
}


/**
 * rwm_upload_data - upload 802.3 frame to lwip, and 802.11 frame to wpa_s/hostapd
 *
 * @host_id: pbuf address
 * @frame_len: 802.3/802.11 frame len
 */
#if CONFIG_RWNX_PROTO_DEBUG
extern uint8_t proto_debug_flag;
#endif
__IRAM2 UINT32 rwm_upload_data(void *host_id, uint32_t frame_len)
{
	/*
	 * +-----  host_id (struct pbuf{} *)
	 * |
	 * V
	 * +----------+-------------------+
	 * |  rxvect  |   IEEE 802.3 Data |
	 * +----------+-------------------+
	 */
	struct fhost_rx_header *rxhdr;
	struct pbuf *p;
	struct pbuf *q;
	//struct mem *p_mem = NULL;
	STA_INF_PTR sta_entry;
#if (CONFIG_SOC_BK7236XX)
	VIF_INF_PTR vif_entry;
#endif

	if (!host_id)
		return BK_FAIL;

	rwm_check_rx_header_pattern(host_id);

	p = (struct pbuf *)host_id;

	//p_mem = (struct mem *)((uint8_t *)p - 8);
	//WIFI_ASSERT(p_mem->used);

	/* TBD: Do we need to linearize pbuf here ? Will lwip do the work */
	if (p->next) {
		q = (struct pbuf *)pbuf_coalesce(p, PBUF_RAW);
		if (q == p) // OOM
			RWNX_LOGE("nonlinear pbuf\n");
	} else {
		q = p;
	}

	q->len = frame_len + macif_get_rxl_payload_offset();
#if BK_SS_WIFI_DP
	rxhdr = (struct fhost_rx_header *)(*(uint32_t*)q->payload);
#else
	rxhdr = (struct fhost_rx_header *)q->payload;
#endif
	pbuf_header(q, -(s16)macif_get_rxl_payload_offset());
	/* record rssi */
	sta_entry = rwm_mgmt_sta_idx2ptr(rxhdr->flags_sta_idx);
	if (sta_entry) {
		int8_t modf, mcs;

		/* byte 5 from recvect, refer MAC-PHY-IF-FS */
		//sta_entry->rssi = (rxhdr->hwvect.recvec1a >> 16) & 0xff; //the same with recvec1b[15:8]
		sta_mgmt_set_rssi(sta_entry, (rxhdr->hwvect.recvec1b >> 8) & 0xff);
		sta_mgmt_set_freq_offset(sta_entry, (rxhdr->hwvect.recvec2a >> 24) & 0xff);
		modf    = rxhdr->hwvect.recvec1a & 0xf;
		if (modf < 2) {
			mcs = (rxhdr->hwvect.recvec1b >> 4) & 0xf;   //nonht
		} else if (modf < 4) {
			mcs = (rxhdr->hwvect.recvec1b >> 24) & 0x7f; //htmf & htgf
		} else if (modf < 5) {
			mcs = (rxhdr->hwvect.recvec1c >> 8) & 0xf;   //vht
		} else {
			mcs = rxhdr->hwvect.recvec1d & 0xf;         //he
		}

		if (modf == 0 && mcs < 4) {
			sta_mgmt_set_freq_offset(sta_entry, (int8_t)(((float)(int)-sta_mgmt_get_freq_offset(sta_entry)) / 1.35));
		}
#if (CONFIG_SOC_BK7236XX)
		vif_entry = rwm_mgmt_vif_idx2ptr(rxhdr->flags_vif_idx);
		if ((NULL != vif_entry) && (VIF_STA == mac_vif_mgmt_get_type(vif_entry))) {
			bk7011_update_by_rx(sta_mgmt_get_rssi(sta_entry), sta_mgmt_get_freq_offset(sta_entry));
		}
#endif
	}
	if (rxhdr->flags_is_80211_mpdu) {
		rwnx_rx_mgmt_any(q, rxhdr);
	} else if (rxhdr->flags_is_amsdu) {
		/* A-MSDU subframe, convert like `rxu_cntrl_mac2eth_update()' and then pass it to lwip */
		RWNX_LOGD("rx_up_data len_p:%x,len_q:%x,p_next:%x,p_totlen:%x,p_payload:%x,q_payload:%x\n",
		p->len,q->len,p->next,p->tot_len,p->payload,q->payload);
		RWNX_LOGD("data_flag flag:%x \n", rxhdr->flags_is_amsdu);
		rwnx_upload_amsdu(rxhdr);
	} else {
#if CONFIG_RWNX_PROTO_DEBUG
		if(proto_debug_flag) {
			struct ethhdr *eth_hdr_ptr;
			eth_hdr_ptr = (struct ethhdr *)q->payload;
			rwm_proto_printf(1, eth_hdr_ptr);
		}
#endif

#if NX_VERSION > NX_VERSION_PACK(6, 22, 0, 0)
		/* preprocess for pbuf */
		rwnx_rx_preprocess(rxhdr->flags_vif_idx, q);
#endif
		if(bk_wifi_get_block_bcmc_en())
		{
			if(rwm_block_bcmc((struct ethhdr *)q->payload,bk_wifi_get_block_bcmc_en()))
			{
				pbuf_free(p);
#if BK_SS_WIFI_DP
				os_free(rxhdr);
#endif
				p = NULL;
				return BK_OK;
			}
		}
		/* for 802.3 frame, pass it to lwip */
		ethernetif_input(rxhdr->flags_vif_idx, q);
	}
#if BK_SS_WIFI_DP
	os_free(rxhdr);
#endif
	return BK_OK;
}

/**
 * NOTE: monitor frame is converted to 802.3 frame if the original
 * 802.11 frameis Data format, but the 802.11 mac header is backed
 * up to `fhost_rx_header.mac_hdr_backup'.
 *
 * host_id: struct pbuf *
 * frame_len: ieee80211 len(rxvect not included)
 */
UINT32 rwm_rx_monitor(void *host_id, uint32_t frame_len)
{
	wifi_monitor_cb_t cb;
	cb = wifi_monitor_get_cb();
	int8_t rssi;
	int8_t rx_rssi[2];

	if (cb) {
		wifi_frame_info_t frame_info;
		struct fhost_rx_header *rxhdr;
		struct pbuf *p;

		p = (struct pbuf *)host_id;
		p->len = frame_len + macif_get_rxl_payload_offset();
#if BK_SS_WIFI_DP
		rxhdr = (struct fhost_rx_header *)(*(uint32_t*)p->payload);
#else
		rxhdr = (struct fhost_rx_header *)p->payload;
#endif

		pbuf_header(p, -(s16)macif_get_rxl_payload_offset());	// strip rxvect

		rssi = macif_rx_monitor_get_rssi(rxhdr->hwvect.recvec1b,rx_rssi);
		frame_info.rssi = rssi;

		/*
		 * payload is 802.3 format if original 802.11 is Data format, see
		 * function `rxu_cntrl_mac2eth_update()'.
		 */
		cb(p->payload, frame_len, &frame_info);

		// recovery pbuf for rwm_upload_data if needed
		pbuf_header(p, (s16)macif_get_rxl_payload_offset());
	}

	return 0;
}

__IRAM2 int fhost_rxbuf_push()
{
#if !BK_MIN_RX_BUFSZ
#if CONFIG_LWIP
	/*
	 * +------+----------+-------------------+
	 * | pbuf |  rxvect  |   IEEE 802.3 Data |
	 * +------+----------+-------------------+
	 */
	struct pbuf *pbuf;

	pbuf = pbuf_alloc(PBUF_RAW, CFG_MSDU_MAX_LEN, PBUF_RAM);
	if (!pbuf) {
		RWNX_LOGI("%s alloc pbuf fail\r\n", __func__);
		return -ENOMEM;
	}

	if (pbuf && fhost_rxbuf_repush((uint32_t)pbuf)) {
		// queue failed, free pbuf
		pbuf_free(pbuf);
	}

	BUILD_BUG_ON(sizeof(struct fhost_rx_header) != RXL_HEADER_INFO_LEN);

#else
	uint8_t *payload;

	payload = ke_malloc(CFG_MSDU_MAX_LEN);
	if (!payload) {
		RWNX_LOGI("%s alloc payload fail\r\n", __func__);
		return -ENOMEM;
	}

	if (fhost_rxbuf_repush((uint32_t)payload))
		ke_free(payload);
#endif
#endif //!BK_MIN_RX_BUFSZ

	return 0;
}

void fhost_free_rx_buffer(uint32 host_id)
{
#if BK_SS_WIFI_DP
	struct pbuf *p = (struct pbuf *)host_id;
	os_free((void*)(*((uint32_t*)p->payload)));
	if (p)
		pbuf_free(p);

#elif CONFIG_NET_LWIP
	struct pbuf *p = (struct pbuf *)host_id;

	if (p)
		pbuf_free(p);
#else
	os_free((void *)host_id);
#endif
}

/*
 * FIXME: BK7236, don't free buff_addr when lwip is used or monitor.
 */
__IRAM2 void fhost_rx_desc_handler(struct rxu_stat_val *rxstat)
{
	uint16_t status = rxstat->status;
	struct fhost_rx_header *rxhdr;
	struct pbuf *p;
	uint32_t host_id;

	host_id = rxstat->host_id;
	p = (struct pbuf *)rxstat->host_id;

#if BK_SS_WIFI_DP
	rxhdr = (struct fhost_rx_header *)(*(uint32_t*)p->payload);
#else
	rxhdr = (struct fhost_rx_header *)p->payload;
#endif

	/* Check if we need to delete the buffer */
	if (status & RX_STAT_DELETE) {
		fhost_free_rx_buffer(host_id);
		goto end;
	}

	/* Check if we need to forward the buffer coming from a monitor interface */
	if (status & RX_STAT_MONITOR) {
		if (g_rwnx_connector.monitor_outbound_func /* && rxhdr->hwvect.len >= 24*/)  // e.g. rwm_rx_monitor
			g_rwnx_connector.monitor_outbound_func((void *)host_id, rxhdr->hwvect.len);

		/* Only monitor interface exist */
		if (status == RX_STAT_MONITOR) {
			status |= RX_STAT_ALLOC;
			fhost_free_rx_buffer(host_id);
		}
	}

	/* Check if we need to update the length */
	if (status & RX_STAT_LEN_UPDATE) {
		rxhdr->hwvect.len = rxstat->frame_len;

		if (status & RX_STAT_ETH_LEN_UPDATE) {
			/* Update Length Field inside the Ethernet Header */
			struct ethhdr  *hdr = (struct ethhdr  *)((uint8_t *)rxhdr + sizeof(struct fhost_rx_header));
			hdr->proto = htons(rxstat->frame_len - sizeof(struct ethhdr));
		}
		goto end;
	}

	/* Check if it must be discarded after informing upper layer */
	if (status & RX_STAT_SPURIOUS) {
		if (g_rwnx_connector.spurious_outbound_func)
			g_rwnx_connector.spurious_outbound_func((void *)host_id, rxhdr->hwvect.len);

		if (fhost_rxbuf_repush(host_id))
			pbuf_free((struct pbuf *)host_id);

		goto end;
	}

	/* Check if we need to forward the buffer */
	if (status & RX_STAT_FORWARD) {
		if (g_rwnx_connector.data_outbound_func)  //rwm_upload_data
			g_rwnx_connector.data_outbound_func((void *)host_id, rxhdr->hwvect.len);
		RX_STAT_INC(data_forward);
	}

	/* Check if we need to allocate a new buffer */
	if (status & RX_STAT_ALLOC)
		fhost_rxbuf_push();

end:
	return;
}

