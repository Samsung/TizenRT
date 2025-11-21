/**
 ****************************************************************************************
 *
 * @file rw_msdu.c
 *
 * @brief MSDU/MPDU TX module
 *
 *
 * Copyright (C) BEKEN corperation 2021-2022
 *
 ****************************************************************************************
 */
#include <common/bk_include.h>
#include <os/os.h>
#include <os/str.h>
#include <os/mem.h>
#include "fhost_msg.h"
#include "rw_msdu.h"
#include "bk_rw.h"
#ifdef CONFIG_TX_BUFING
//#include "ps.h"
#endif
#include "bk_lwipif_tizenrt.h"
#if !CONFIG_RWNX_SW_TXQ
#if (CONFIG_FULLY_HOSTED || CONFIG_SEMI_HOSTED)
#include "pbuf.h"
#if CONFIG_SEMI_HOSTED
#include "sk_intf.h"
#endif
#endif
#ifdef CONFIG_WFA_CERTIFICATION
#include "prot/ip4.h"
#include "prot/ip6.h"
#include "prot/ethernet.h"
#endif

#include "bk_arm_arch.h"
#if CONFIG_GENERAL_DMA
#include "bk_general_dma.h"
#endif
#include "bk_wifi_types.h"
#include "bk_wifi.h"
#include "bk_fake_clock.h"
#include "bk_ps.h"
#include "rwnx_rx.h"
#include "rwnx_tx.h"
#include "ieee802_11_defs.h"
#include "wpa_ctrl.h"
#include "main_none.h"
#include "ctrl_iface.h"
#include "rwnx_defs.h"
#include "bk_wifi_private.h"

#ifdef CONFIG_TX_BUFING
#include "rw_tx_buffering.h"
#endif

#if CONFIG_RWNX_PROTO_DEBUG
#include "rwm_proto.h"
#endif

void ethernetif_input(int iface, struct pbuf *p);
#if !CONFIG_SPECIAL_TX_TYPE
UINT32 rwm_transfer_node(MSDU_NODE_T *node, u8 flag);
#endif
extern int ke_mgmt_packet_tx(unsigned char *buf, int len, int flag);

LIST_HEAD_DEFINE(msdu_rx_list);


UINT8 g_tid = 0xFF;

void rwm_push_rx_list(MSDU_NODE_T *node)
{
	WIFI_GLOBAL_INT_DECLARATION();

	WIFI_GLOBAL_INT_DISABLE();
	list_add_tail(&node->hdr, &msdu_rx_list);
	WIFI_GLOBAL_INT_RESTORE();
}

MSDU_NODE_T *rwm_pop_rx_list(void)
{
	LIST_HEADER_T *tmp;
	LIST_HEADER_T *pos;
	MSDU_NODE_PTR node;

	WIFI_GLOBAL_INT_DECLARATION();

	WIFI_GLOBAL_INT_DISABLE();

	node = NULLPTR;
	list_for_each_safe(pos, tmp, &msdu_rx_list) {
		list_del(pos);
		node = list_entry(pos, MSDU_NODE_T, hdr);

		break;
	}

	WIFI_GLOBAL_INT_RESTORE();

	return node;
}

void rwm_flush_rx_list(void)
{
	MSDU_NODE_T *node_ptr;

	while (1) {
		node_ptr = rwm_pop_rx_list();
		if (node_ptr)
			wifi_os_free(node_ptr);
		else
			break;
	}
}

void rwm_tx_msdu_renew(UINT8 *buf, UINT32 len, UINT8 *orig_addr)
{
#if CONFIG_GENERAL_DMA
	dma_memcpy((void *)((UINT32)orig_addr + CONFIG_MSDU_RESV_HEAD_LEN), buf, len);
#else
	os_memmove((void *)((UINT32)orig_addr + CONFIG_MSDU_RESV_HEAD_LEN), buf, len);
#endif
}

void rwm_tx_mpdu_renew(UINT8 *buf, UINT32 len, UINT8 *orig_addr)
{
#if CONFIG_GENERAL_DMA
	dma_memcpy((void *)((UINT32)orig_addr), buf, len);
#else
	os_memmove((void *)((UINT32)orig_addr), buf, len);
#endif
}

UINT8 *rwm_get_msdu_content_ptr(MSDU_NODE_T *node)
{
	return node->msdu_ptr;
}

UINT8 *rwm_get_mpdu_content_ptr(MSDU_NODE_T *node)
{
	return (UINT8 *)((UINT32)node->msdu_ptr);
}

void rwm_txdesc_copy(struct txdesc *dst_local, ETH_HDR_PTR eth_hdr_ptr)
{
	struct hostdesc *host_ptr;

	host_ptr = &dst_local->host;

	os_memcpy(&host_ptr->eth_dest_addr, &eth_hdr_ptr->e_dest, sizeof(host_ptr->eth_dest_addr));
	os_memcpy(&host_ptr->eth_src_addr, &eth_hdr_ptr->e_src, sizeof(host_ptr->eth_src_addr));
}

#if !CONFIG_FULLY_HOSTED
int rwm_raw_frame_with_cb(uint8_t *buffer, int len, void *cb, void *param)
{
	/* TODO: BK7236 monitor tx */
	return len;
}
#endif /* !CONFIG_FULLY_HOSTED */

MSDU_NODE_T *rwm_tx_node_alloc(UINT32 len, pbuf_layer layer)
{
	MSDU_NODE_T *node_ptr = 0;
	struct pbuf *p;

	/* no payload here */
	node_ptr = (MSDU_NODE_T *)wifi_zalloc(sizeof(MSDU_NODE_T));

	if (!node_ptr)
		goto alloc_exit;
#if BK_SS_WIFI_DP
	p = bk_wlan_buf_alloc(len);
#else
	p = pbuf_alloc(layer, len, PBUF_RAM);
#endif
	if (!p) {
		wifi_os_free(node_ptr);
		node_ptr = 0;
		goto alloc_exit;
	}

	node_ptr->p = p;
	node_ptr->msdu_ptr = p->payload;
	node_ptr->len = p->tot_len;

alloc_exit:
	return node_ptr;
}

MSDU_NODE_T *rwm_tx_mgmt_node_alloc(UINT32 len, pbuf_layer layer)
{
	MSDU_NODE_T *node_ptr = 0;
	unsigned char *payload = 0;

	/* no payload here */
	node_ptr = (MSDU_NODE_T *)wifi_zalloc(sizeof(MSDU_NODE_T));

	if (!node_ptr)
		goto alloc_exit;

	payload = (unsigned char *)wifi_zalloc(len);
	if (!payload) {
		wifi_os_free(node_ptr);
		node_ptr = 0;
		goto alloc_exit;
	}

	node_ptr->p = 0;
	node_ptr->msdu_ptr = payload;
	node_ptr->len = len;

alloc_exit:
	return node_ptr;
}

MSDU_NODE_T *rwm_tx_node_alloc_with_pbuf(struct pbuf *p)
{
	MSDU_NODE_T *node_ptr = 0;

	node_ptr = (MSDU_NODE_T *)wifi_zalloc(sizeof(MSDU_NODE_T));

	if (!node_ptr)
		goto alloc_exit;

	node_ptr->p = p;
	node_ptr->msdu_ptr = p->payload;
	node_ptr->len = p->tot_len;

alloc_exit:
	return node_ptr;
}

void rwm_node_free(MSDU_NODE_T *node)
{
	if (!node)
		return;

	if (node->ftxdesc) {
		wifi_os_free(node->ftxdesc);
		node->ftxdesc = NULL;
	}
	if (node->p) {
		wifi_pbuf_free(node->p);
		node->p = NULL;
	}
	wifi_os_free(node);
}

UINT8 *rwm_rx_buf_alloc(UINT32 len)
{
	return (UINT8 *)wifi_malloc(len);
}

UINT32 rwm_get_rx_valid(void)
{
	UINT32 count = 0;
	LIST_HEADER_T *tmp;
	LIST_HEADER_T *pos;
	LIST_HEADER_T *head = &msdu_rx_list;

	WIFI_GLOBAL_INT_DECLARATION();

	WIFI_GLOBAL_INT_DISABLE();
	list_for_each_safe(pos, tmp, head) {
		count ++;
	}
	WIFI_GLOBAL_INT_RESTORE();

	return ((count >= MSDU_RX_MAX_CNT) ? 0 : 1);
}

UINT32 rwm_get_rx_valid_node_len(void)
{
	UINT32 len = 0;
	LIST_HEADER_T *tmp;
	LIST_HEADER_T *pos;
	MSDU_NODE_PTR node;

	WIFI_GLOBAL_INT_DECLARATION();

	WIFI_GLOBAL_INT_DISABLE();

	node = NULLPTR;
	list_for_each_safe(pos, tmp, &msdu_rx_list) {
		node = list_entry(pos, MSDU_NODE_T, hdr);
		len = node->len;
		break;
	}

	WIFI_GLOBAL_INT_RESTORE();

	return len;
}

UINT8 rwm_get_tid()
{
	return g_tid;
}

void rwm_set_tid(UINT8 tid)
{
	if (0xFF == tid)
		g_tid = tid;
	else
		g_tid = tid & MAC_QOSCTRL_UP_MSK;
}

int rwm_set_tx_raw_rate_power(UINT32 rate, UINT32 power)
{
	if (txl_buffer_control_24G.policy_tbl.ratecntrlinfo[0] != rate)
	{
		txl_buffer_control_24G.policy_tbl.ratecntrlinfo[0] = rate;
	}

	if (txl_buffer_control_24G.policy_tbl.powercntrlinfo[0] != power)
	{
		txl_buffer_control_24G.policy_tbl.powercntrlinfo[0] = power;
	}

	return BK_OK;
}
#ifdef CONFIG_USE_P2P_PS
void rwm_p2p_ps_change_ind_handler(void *msg)
{
	struct ke_msg *msg_ptr = (struct ke_msg *)msg;
	struct mm_p2p_vif_ps_change_ind *ind;

	if (!msg_ptr || !msg_ptr->param)
		return;

	ind = (struct mm_p2p_vif_ps_change_ind *)msg_ptr->param;

	if (ind->ps_state == PS_MODE_OFF)
		rwm_trigger_tx_bufing_stop(TX_BUFING_SRC_P2P_PS, ind->vif_index);
	else if (ind->ps_state == PS_MODE_ON)
		rwm_trigger_tx_bufing_start(TX_BUFING_SRC_P2P_PS, ind->vif_index);
}

#endif

#if CONFIG_AP_PS
void rwm_msdu_ps_change_ind_handler(void *msg)
{
	struct ke_msg *msg_ptr = (struct ke_msg *)msg;
	struct mm_ps_change_ind *ind;

	if (!msg_ptr || !msg_ptr->param)
		return;

	ind = (struct mm_ps_change_ind *)msg_ptr->param;

	if (ind->ps_state == PS_MODE_OFF)
		rwm_trigger_tx_bufing_stop(TX_BUFING_SRC_STA_PS, ind->sta_idx);
	else if (ind->ps_state == PS_MODE_ON)
		rwm_trigger_tx_bufing_start(TX_BUFING_SRC_STA_PS, ind->sta_idx);
}
#endif

void rwm_msdu_init(void)
{
#if CONFIG_TX_BUFING
	rwm_tx_bufing_init();
#endif

	g_tid = 0xFF;
}

#ifdef CONFIG_WFA_CERTIFICATION
/*
 * IEEE802.11-2016: Table 10-1—UP-to-AC mappings
 */
uint8_t ipv4_ieee8023_dscp(UINT8 *buf)
{
	uint8_t tos;
	struct ip_hdr *hdr = (struct ip_hdr *)buf;

	tos = IPH_TOS(hdr);

	return (tos & 0xfc) >> 5;
}

/* extract flow control field */
uint8_t ipv6_ieee8023_dscp(UINT8 *buf)
{
	uint8_t tos;
	struct ip6_hdr *hdr = (struct ip6_hdr *)buf;

	tos = IP6H_FL(hdr);

	return (tos & 0xfc) >> 5;
}
#endif

int qos_need_enabled(struct sta_info_tag *sta)
{
	if (!sta)
		return 0;
	if (!STA_CAPA(sta, QOS))
		return 0;

	return 1;
}

/*
 * get user priority from @buf.
 * ipv4 dscp/tos, ipv6 flow control. for eapol packets, disable qos.
 */
static uint8_t classify8021d(UINT8 *buf)
{
#ifdef CONFIG_WFA_CERTIFICATION
	struct eth_hdr *ethhdr = (struct eth_hdr *)buf;

	switch (PP_HTONS(ethhdr->type)) {
	case ETHTYPE_IP:
		return ipv4_ieee8023_dscp(ethhdr + 1);
	case ETHTYPE_IPV6:
		return ipv6_ieee8023_dscp(ethhdr + 1);
	case ETH_P_PAE:
		return 7;	/* TID7 highest user priority */
	default:
		return 0;
	}
#else
	return 0;		// TID4: mapped to AC_BE
#endif
}

static void rwm_confirm_wrapper(void *arg)
{
	struct txdesc *txdesc_new = (struct txdesc *)arg;
	MSDU_NODE_T *node = txdesc_new->host.buf;

	if (node && node->cb) {
		struct tx_hd *txhd = &txdesc_new->lmac.hw_desc->thd;
		uint32_t status = txhd->statinfo;
		void (*cb)(void *, bool) = node->cb;

		cb(node->args, !!(status & FRAME_SUCCESSFUL_TX_BIT));
	}
}

//pbuf_coalesce, pbuf_copy
static int rwm_add_payload(struct pbuf *p, struct fhost_txdesc *ftxdesc)
{
	int i;
	struct tx_pbd *pbd;
	struct txl_buffer_tag *txl_buf;
	txl_buf = &ftxdesc->txl_buf;

	/* iterate through pbuf chain */
	pbd = txl_buf->pbd;
	for (i = 0; p && i < TX_PBD_CNT; i++) {
		pbd->upatterntx = TX_PAYLOAD_DESC_PATTERN;
		if (i == 0) {
			/* pbd[0] trim ethernet hdr */
			pbd->datastartptr = (uint32_t)p->payload + sizeof(ETH_HDR_T);
		} else
			pbd->datastartptr = (uint32_t)p->payload;
		pbd->dataendptr = (uint32_t)p->payload + p->len - 1;
		pbd->bufctrlinfo = 0;
		pbd->next = CPU2HW(pbd + 1);

		pbd++;
		p = p->next;
	}

	if (i > 0)
		txl_buf->pbd[i - 1].next = 0;

	if (p || i == TX_PBD_CNT) {
		RWNX_LOGE("not all data are pushed\n");
		return -1;
	}

	return 0;
}

UINT32 rwm_transfer_node(MSDU_NODE_T *node, u8 flag
#if CONFIG_SPECIAL_TX_TYPE
    ,bool is_special_tx
#endif
)
{
	uint8_t                 queue_idx;
	struct fhost_txdesc     *fhost_txdesc = 0;
	struct txdesc           *txdesc;
	struct hostdesc         *host;
	struct txl_buffer_tag   *txl_buf;
	struct tx_pbd           *pbd;
	struct tx_hw_desc       *hwdesc;
	UINT8 tid;
	UINT32 ret = 0;
	UINT8 *content_ptr;
	ETH_HDR_PTR eth_hdr_ptr;
	struct pbuf *p;

#if CONFIG_RWNX_QOS_MSDU
	struct sta_info_tag *sta;
	struct vif_info_tag *vif;
#endif

	if (!node)
		goto tx_exit;

	p = node->p;

	content_ptr = p->payload;
	eth_hdr_ptr = (ETH_HDR_PTR)content_ptr;

#if CONFIG_RWNX_QOS_MSDU
	vif = rwm_mgmt_vif_idx2ptr(node->vif_idx);
	if (NULL == vif) {
		RWNX_LOGI("%s: vif is NULL!\r\n", __func__);
		goto tx_exit;
	}

	if (likely(vif->active)) {
		sta = &sta_info_tab[vif->u.sta.ap_id];
		if (qos_need_enabled(sta)) {
			int i;
			tid = classify8021d((UINT8 *)eth_hdr_ptr);
			/* check admission ctrl */
			for (i = mac_tid2ac[tid]; i >= 0; i--)
				if (!(vif->bss_info.edca_param.acm & BIT(i)))
					break;
			if (i < 0)
				goto tx_exit;
			queue_idx = i;	/* AC_* */
		} else {
			/*
			 * non-WMM STA
			 *
			 * CWmin 15, CWmax 1023, AIFSN 2, TXOP 0. set these values when joining with this BSS.
			 */
			tid = 0xFF;
			queue_idx = AC_BE;
		}
	} else {
		tid = 0xFF;
		queue_idx = AC_BE;
	}
#else /* !CONFIG_RWNX_QOS_MSDU */
	tid = rwm_get_tid();

	queue_idx = AC_VI;
#endif /* CONFIG_RWNX_QOS_MSDU */

	//alloc tx desc
	fhost_txdesc = (struct fhost_txdesc *)wifi_zalloc(sizeof(struct fhost_txdesc));
	if (!fhost_txdesc)
		goto tx_exit;

	node->ftxdesc = fhost_txdesc;

	txdesc = &fhost_txdesc->txdesc;
	host = &txdesc->host;
	txl_buf = &fhost_txdesc->txl_buf;
	pbd = &txl_buf->pbd[0];
	hwdesc = &fhost_txdesc->hwdesc;

	//fill host desc
	rwm_txdesc_copy(txdesc, eth_hdr_ptr);
	host->buf = node;
	host->flags = flag;
#if CONFIG_SPECIAL_TX_TYPE
	if (is_special_tx)
	    host->flags |= TXU_CNTRL_IS_SPECIAL_FRAME;
#endif

#if NX_AMSDU_TX
	host->orig_addr[0]     = (UINT32)node->msdu_ptr;
	host->packet_addr[0]   = (UINT32)content_ptr + sizeof(ETH_HDR_T);
	host->packet_len[0]    = node->len - sizeof(ETH_HDR_T);
	host->packet_cnt       = 1;
#else
	host->packet_addr      = (UINT32)content_ptr + sizeof(ETH_HDR_T);
	host->packet_len       = node->len - sizeof(ETH_HDR_T);
#endif
#if !NX_FULLY_HOSTED
	host.status_desc_addr = (UINT32)content_ptr + sizeof(ETH_HDR_T);
#endif
	host->ethertype       = eth_hdr_ptr->e_proto;
	host->tid = tid;

	host->vif_idx = node->vif_idx;
	host->staid = node->sta_idx;

	//set SW retry TX count to 0
	host->sw_retry_cnt = 0;

	if (node->cb) {
		host->cfm_cb      = (mgmt_tx_cb_t)rwm_confirm_wrapper;;
		host->cfm_cb_arg  = txdesc;
	}
	//fill lmac desc
	txdesc->lmac.hw_desc = hwdesc;
	txdesc->lmac.buffer  = txl_buf;
	txdesc->lmac.agg_desc = NULL;
  #if NX_AMPDU_TX_FAST_RETRY
	txdesc->lmac.original_tx_ac = queue_idx;
  #endif
#if !NX_FULLY_HOSTED
	txdesc->lmac.hw_desc->cfm.status = 0;
#endif

#if BK_MAC
#if MAC_SFRAME_SOFTWARE_RETRY
    txdesc->umac.sframe_sw_retry_cnt = 0;
    txdesc->umac.sframe_type = S_FRAME_INVALID;
    txdesc->umac.sframe_sw_retry_finish = 0xff;
#else
    txdesc->umac.sframe_type = S_FRAME_INVALID;
#endif
    txdesc->umac.tx_discard = 0;
    txdesc->umac.bam_idx = BAM_INVALID_TASK_IDX;
#endif

	hwdesc->thd.first_pbd_ptr = (uint32_t)pbd;
	hwdesc->thd.dataendptr = 0;
	hwdesc->thd.datastartptr = 0;

	rwm_add_payload(p, fhost_txdesc);

	//notify fw
	ret = fhost_txbuf_push(txdesc, queue_idx);
	if (ret)
		goto tx_exit;

	return 0;

tx_exit:
	if (node)
		rwm_node_free(node);
	return ret;
}
#if CONFIG_RWNX_PROTO_DEBUG
extern uint8_t proto_debug_flag;
#endif
UINT32 rwm_transfer(UINT8 vif_idx, struct pbuf *p, UINT8 *buf, UINT32 len, void *cb, void *args
        #if CONFIG_SPECIAL_TX_TYPE
        ,bool is_special_tx
        #endif
        )
{
	UINT32 ret = 0;
	MSDU_NODE_T *node;
	ETH_HDR_PTR eth_hdr_ptr;

	ret = BK_FAIL;
	node = rwm_tx_node_alloc_with_pbuf(p);
	if (NULL == node) {
		RWNX_LOGI("rwm_transfer no node\r\n");
		goto tx_exit;
	}

	eth_hdr_ptr = (ETH_HDR_PTR)buf;
	node->vif_idx = vif_idx;
	node->cb = cb;
	node->args = args;
	node->sta_idx = rwm_mgmt_tx_get_staidx(vif_idx,
										   &eth_hdr_ptr->e_dest);
#if CONFIG_RWNX_PROTO_DEBUG
	if(proto_debug_flag)
		rwm_proto_printf(0, (struct ethhdr *)eth_hdr_ptr);
#endif
#if CONFIG_TX_BUFING
        if (!is_special_tx && rwm_check_tx_bufing(node))
        {
                RWNX_LOGI("tx buffer: save node %x\n",node);
                rwm_tx_bufing_save_data(node);
                return ret;
        }

        rwm_transfer_node(node, 0
        #if CONFIG_SPECIAL_TX_TYPE
        ,is_special_tx
        #endif
        );

        return ret;

#else
	rwm_transfer_node(node, 0
	#if CONFIG_SPECIAL_TX_TYPE
	,is_special_tx
	#endif
	);
#endif

	return ret;

tx_exit:
	if (node)
		rwm_node_free(node);

	return ret;
}

/**
 * transmit mgmt frame
 */
void rwm_transfer_mgmt_node(MSDU_NODE_T *node)
{
	struct hostdesc *host;
	UINT8 *content_ptr;
	UINT32 queue_idx = AC_VI;
	struct txdesc *txdesc;
	struct fhost_txdesc *fhost_txdesc;
	struct txl_buffer_tag *txl_buf;
	struct tx_pbd *pbd;
	struct tx_hw_desc *hwdesc;
	int ret = 0;
	//uint8_t vif_index = 0xFF;
	//struct vif_info_tag *vif;
	bool robust;

	content_ptr = rwm_get_mpdu_content_ptr(node);

	// alloc fhost tx desc
	fhost_txdesc = (struct fhost_txdesc *)wifi_zalloc(sizeof(struct fhost_txdesc));

	if (!fhost_txdesc)
		goto tx_exit;

	node->ftxdesc = fhost_txdesc;

	// initialize variable
	txdesc = &fhost_txdesc->txdesc;
	host = &txdesc->host;
	txl_buf = &fhost_txdesc->txl_buf;
	pbd = &txl_buf->pbd[0];
	hwdesc = &fhost_txdesc->hwdesc;

	robust = ieee80211_is_robust_mgmt_frame(content_ptr, node->len);

	// setup hostdesc
	host->flags = TXU_CNTRL_MGMT;

	if (robust)
		host->flags |= TXU_CNTRL_MGMT_ROBUST;

#if 0 //keepme
	bool robust;
	robust = ieee80211_is_robust_mgmt_frame(content_ptr, len); // TBD: BK7236

	if (robust)
		host.flags |= TXU_CNTRL_MGMT_ROBUST;

	if (params->no_cck)
		desc->host.flags |= TXU_CNTRL_MGMT_NO_CCK;
#endif
	host->buf = node;
	host->packet_addr = (UINT32)content_ptr;	//mark not-internal frame
	host->packet_len = node->len;
	host->tid = 0xff;
	host->vif_idx = node->vif_idx;
	host->staid = node->sta_idx;

	if (node->cb) {
		/* host->cfm_cb has only one argument, so use wrapper here */
		host->cfm_cb = (mgmt_tx_cb_t)rwm_confirm_wrapper;
		host->cfm_cb_arg = (void *)txdesc;
	}

	// fill lmac desc
	txdesc->lmac.hw_desc = hwdesc;
	txdesc->lmac.buffer  = txl_buf;
	txdesc->lmac.agg_desc = NULL;

	hwdesc->thd.first_pbd_ptr = (uint32_t)pbd;
	hwdesc->thd.dataendptr = 0;
	hwdesc->thd.datastartptr = 0;

	// fill tx_pbd
	pbd->upatterntx = TX_PAYLOAD_DESC_PATTERN;
	pbd->datastartptr = (uint32_t)content_ptr;
	pbd->dataendptr = (uint32_t)content_ptr + node->len - 1;
	pbd->bufctrlinfo = 0;
	pbd->next = 0;

	// notify fw
	ret = fhost_txbuf_push((void *)txdesc, queue_idx);
	if (ret)
		goto tx_exit;

	return;

tx_exit:
	if (node)
		rwm_node_free(node);
}

void rwm_transfer_raw_ex_node(MSDU_NODE_T *node, raw_tx_cntrl_t *raw_tx_cntrl)
{
	struct hostdesc *host;
	UINT8 *content_ptr;
	uint8_t queue_idx = 0;
	uint16_t raw_timeout = 0;
	struct txdesc *txdesc;
	struct fhost_txdesc *fhost_txdesc;
	struct txl_buffer_tag *txl_buf;
	struct tx_pbd *pbd;
	struct tx_hw_desc *hwdesc;
	int ret = 0;
	//uint8_t vif_index = 0xFF;
	//struct vif_info_tag *vif;
	UINT8 retry_cnt = 0;
	content_ptr = rwm_get_mpdu_content_ptr(node);

	bk_wifi_get_tx_raw_ac(&queue_idx);
	bk_wifi_get_tx_raw_timeout(&raw_timeout);

	// alloc fhost tx desc
	fhost_txdesc = (struct fhost_txdesc *)wifi_zalloc(sizeof(struct fhost_txdesc));

	if (!fhost_txdesc)
		goto tx_exit;

	node->ftxdesc = fhost_txdesc;

	// initialize variable
	txdesc = &fhost_txdesc->txdesc;
	host = &txdesc->host;
	txl_buf = &fhost_txdesc->txl_buf;
	pbd = &txl_buf->pbd[0];
	hwdesc = &fhost_txdesc->hwdesc;

	txl_buf->buffer_control.policy_tbl.upatterntx        = POLICY_TABLE_PATTERN;
	txl_buf->buffer_control.policy_tbl.phycntrlinfo1     = phy_get_ntx() << NX_TX_PT_OFT;
	txl_buf->buffer_control.policy_tbl.phycntrlinfo2     = phy_get_antenna_set() << ANTENNA_SET_PT_OFT;
	txl_buf->buffer_control.policy_tbl.maccntrlinfo1     = 0;

	retry_cnt = raw_tx_cntrl->tx_retry_cnt & 0xFF;

	txl_buf->buffer_control.policy_tbl.maccntrlinfo2 = 0xFFFF0000 | retry_cnt<<8 | retry_cnt;
	txl_buf->buffer_control.policy_tbl.ratecntrlinfo[0] = (raw_tx_cntrl->tx_rate & 0x3fffu) << MCS_INDEX_TX_RCX_OFT;
	txl_buf->buffer_control.policy_tbl.powercntrlinfo[0] = TX_PWR_LEVEL_SET(raw_tx_cntrl->tx_power& 0xffu);

	// update tx timeout to 50ms
	txl_cntrl_set_timeout_per_ac(queue_idx, raw_timeout);

	// setup hostdesc
	host->flags = TXU_CNTRL_MGMT;
	host->buf = node;
	host->packet_addr = (UINT32)content_ptr;	//mark not-internal frame
	host->packet_len = node->len;
	host->tid = 0xff;
	host->vif_idx = node->vif_idx;
	host->staid = node->sta_idx;

	host->flags |= TXU_CNTRL_INDEPENDENT_PT;

	if (node->cb) {
		/* host->cfm_cb has only one argument, so use wrapper here */
		host->cfm_cb = (mgmt_tx_cb_t)rwm_confirm_wrapper;
		host->cfm_cb_arg = (void *)txdesc;
	}

	// fill lmac desc
	txdesc->lmac.hw_desc = hwdesc;
	txdesc->lmac.buffer  = txl_buf;
	txdesc->lmac.agg_desc = NULL;

	hwdesc->thd.first_pbd_ptr = (uint32_t)pbd;
	hwdesc->thd.dataendptr = 0;
	hwdesc->thd.datastartptr = 0;

	// fill tx_pbd
	pbd->upatterntx = TX_PAYLOAD_DESC_PATTERN;
	pbd->datastartptr = (uint32_t)content_ptr;
	pbd->dataendptr = (uint32_t)content_ptr + node->len - 1;
	pbd->bufctrlinfo = 0;
	pbd->next = 0;

	// notify fw
	ret = fhost_txbuf_push((void *)txdesc, queue_idx);
	if (ret)
		goto tx_exit;

	return;

tx_exit:
	if (node)
		rwm_node_free(node);
}

#endif /* !CONFIG_RWNX_SW_TXQ */
