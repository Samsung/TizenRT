#include <common/bk_include.h>
#include "fhost_tx.h"
#include "rw_msdu.h"
#include "bk_rw.h"
//#include <os/str.h>
//#include <os/mem.h>
#include "rwnx_defs.h"
#include "rwnx_config.h"

#if (CONFIG_FULLY_HOSTED || CONFIG_SEMI_HOSTED)
#include "pbuf.h"
#if CONFIG_SEMI_HOSTED
#include "sk_intf.h"
#endif
#endif
#include "rwnx_rx.h"
#include "../wpa_supplicant-2.10/src/common/ieee802_11_defs.h"
#include "../wpa_supplicant-2.10/src/common/wpa_ctrl.h"
#include "../wpa_supplicant-2.10/hostapd/main_none.h"
#include "bk_wifi_private.h"
#include "../wpa_supplicant-2.10/wpa_supplicant/ctrl_iface.h"
#include "rwnx_tx.h"
#include "rwnx_misc.h"
#if 1//CONFIG_RWNX_PROTO_DEBUG
#include "rwm_proto.h"
#endif
#if CONFIG_RWNX_TD
#include "rwnx_td.h"
#endif
#include "bk_general_dma.h"
//#include "prot/ieee.h"
#include "lwip/def.h"
#include "bk_lwipif_tizenrt.h"

//#define dbg(...)

/// Length of a char in bytes
#define CHAR_LEN    (CHAR_BIT/8)

/**
 ****************************************************************************************
 * @brief Converts a CPU pointer into a HW address
 * This macro is used to convert a SW pointer into the corresponding HW address. With CPUs
 * having native byte support, the value returned will be the same as the pointer passed.
 * With TL4, the value returned is the pointer multiplied by 2.
 * @param[in] ptr Pointer to be converted
 * @return The corresponding HW address
 ****************************************************************************************
 */
#define CPU2HW(ptr) (((uint32_t)(ptr)) * CHAR_LEN)
/// Array converting a TID to its associated AC
extern const uint8_t mac_tid2ac[];

extern int ke_mgmt_packet_tx(unsigned char *buf, int len, int flag);
uint64_t rwnx_hw_mm_features(void);

#if !CONFIG_RWNX_SW_TXQ
__IRAM2 static void rwnx_tx_confirm(void *param)
{
	struct txdesc *txdesc = (struct txdesc *)param;
	MSDU_NODE_T *node = txdesc->host.buf;

	if (txdesc && node) {
		struct fhost_tx_desc_tag *fhost_txdesc = container_of(txdesc, struct fhost_txdesc, txdesc);
		void (*cfm_cb)(void *) = txdesc->host.cfm_cb;

		os_null_printf("%s %d: txdesc %p, fhost_txdesc %p, cfm_cb %p, arg %p\n",
					   __func__, __LINE__, txdesc, fhost_txdesc, cfm_cb, txdesc->host.cfm_cb_arg);

		if (co_list_is_empty(&vif_mgmt_env.used_list)) {
			//do nothing
			RWNX_LOGI("rwnx_tx_confirm, vif empty\n");
		} else {
			// callback for mgmt frame tx
			if (cfm_cb)
				cfm_cb(txdesc->host.cfm_cb_arg);
		}
		RWNX_LOGD("flush_desc: node %p, pbuf %p\n", node, node->p);

		if (node->p) {
#if BK_SS_WIFI_DP
			bk_wlan_buf_free(skb->p);
#else
			pbuf_free(node->p);
#endif
			node->p = NULL;
		} else if (node->msdu_ptr) {
			//fix jira BK7256-4139
			os_free(node->msdu_ptr);
			node->msdu_ptr = NULL;
		}
		// free msdu
		os_free(node);
		txdesc->host.buf = NULL;

		// free fhost tx descriptor
		os_free(fhost_txdesc);
	}
}

#else // CONFIG_RWNX_SW_TXQ

#if CONFIG_MAC_SFRAME_SOFTWARE_RETRY
/**
 * check if the frame needs to be retransmitted if tx failed.
 */
__IRAM2 static bool rwnx_need_retry_tx_frame(struct sk_buff *skb)
{
	struct txdesc *txdesc = &skb->ftxdesc->txdesc;

	switch (PP_NTOHS(txdesc->host.ethertype)) {
	//case ETH_P_ARP:
		// Don't retry send special frame that doesn't open TD window
		if (txdesc->host.flags & TXU_CNTRL_IS_SPECIAL_FRAME)
			return false;
		return true;
	case ETH_P_EAPOL:
	//case ETH_P_RARP:
		return true;
	case ETH_P_IP:
		// Check DHCP
		if (skb->sframe_type == S_FRAME_DHCP)
			return true;
		return false;
	default:
		return false;
	}
}
#endif

__IRAM2 static inline struct pbuf *macif_get_txdesc_pbuf(struct txdesc *txdesc)
{
    MSDU_NODE_T * node = (MSDU_NODE_T *)(txdesc->host.buf);
    return node ? node->p : NULL;
}

__IRAM2 static void rwnx_tx_confirm(void *param)
{
	struct txdesc *txdesc = (struct txdesc *)param;
#if NX_VERSION >= NX_VERSION_PACK(6, 22, 0, 0)
	struct sk_buff *skb = txdesc->host.hostid;
#else
	struct sk_buff *skb = txdesc->host.buf;
#endif

	if (txdesc && skb) {
		struct fhost_tx_desc_tag *fhost_txdesc =
				container_of(txdesc, struct fhost_tx_desc_tag, txdesc);
		//struct tx_cfm_tag *cfm = txl_cfm_tag_get(txdesc);
		struct tx_cfm_tag *cfm = &txdesc->host.cfm;
		struct rwnx_txq *txq = skb->txq;
		void (*cb)(void *, bool) = skb->cb;

		bk_null_printf("%s %d: txdesc %p, fhost_txdesc %p, cb %p, arg %p\n",
					   __func__, __LINE__, txdesc, fhost_txdesc,
					   cb, skb->args);

		if (txq && cfm->credits) {
			txq->credits += cfm->credits;
			if (txq->credits <= 0)
				rwnx_txq_stop(txq, RWNX_TXQ_STOP_FULL);
			else if (txq->credits > 0)
				rwnx_txq_start(txq, RWNX_TXQ_STOP_FULL);
		}

#if 0// TODO ,CONFIG_MAC_SFRAME_SOFTWARE_RETRY
		RWNX_LOGD("%s: stype %d, jiffies %llu, current %llu\n", __func__,
			skb->sframe_type, skb->jiffies, bk_get_tick());

		// ACK not received, and not discard by txu, do software retry
		if (rwnx_need_retry_tx_frame(skb) &&
			!(cfm->status & TX_STATUS_ACKNOWLEDGED) &&
			!(cfm->status & TX_STATUS_DISCARDED)) {
			// Check software retry
			// 1. lifetime not exceeds 2s
			// 2. counter not exceeds 10 times
			if (time_before(bk_get_tick(), skb->jiffies + 2 * bk_get_ticks_per_second()) &&
				skb->sframe_sw_retry_cnt < 10) {
				skb->sframe_sw_retry_cnt++;

				RWNX_LOGI("%s: requeue skb\n", __func__);
				// Requeue skb
				if (rwnx_txq_queue_skb(skb, txq, 0, NULL))
					rwnx_hwq_process(txq->hwq);
				return;
			}
		}
#endif

		// callback for mgmt frame tx
		if (cb && skb->args)
			cb(skb->args, !!(cfm->status & TX_STATUS_ACKNOWLEDGED));

		bk_null_printf("flush_desc: node %p, pbuf %p\n", skb, skb->p);

		kfree_skb(skb);
	}
}

#define PRIO_STA_NULL 0xAA

static const int rwnx_down_hwq2tid[3] = {
    [RWNX_HWQ_BK] = 2,
    [RWNX_HWQ_BE] = 3,
    [RWNX_HWQ_VI] = 4,
};

__IRAM2 static void rwnx_downgrade_ac(void *vif, struct sk_buff *skb)
{
    int8_t ac = rwnx_tid2hwq[skb->priority];
	uint8_t acm = mac_vif_mgmt_get_bss_info_edca_param_acm(vif);

    if (WARN((ac > RWNX_HWQ_VO),
             "Unexepcted ac %d for skb before downgrade", ac))
        ac = RWNX_HWQ_VO;

    while (acm & BIT(ac)) {
        if (ac == RWNX_HWQ_BK) {
            skb->priority = 1;
            return;
        }
        ac--;
        skb->priority = rwnx_down_hwq2tid[ac];
    }
}

void rwm_tx_msdu_renew(UINT8 *buf, UINT32 len, UINT8 *orig_addr)
{
#if CONFIG_GENERAL_DMA
	dma_memcpy((void *)((UINT32)orig_addr + CONFIG_MSDU_RESV_HEAD_LENGTH), buf, len);
#else
	os_memmove((void *)((UINT32)orig_addr + CONFIG_MSDU_RESV_HEAD_LENGTH), buf, len);
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

#if !CONFIG_FULLY_HOSTED
int rwm_raw_frame_with_cb(uint8_t *buffer, int len, void *cb, void *param)
{
	/* TODO: BK7236 monitor tx */
	return len;
}
#endif /* !CONFIG_FULLY_HOSTED */

/*
 * IEEE802.11-2016: Table 10-1—UP-to-AC mappings
 */
static uint8_t ipv4_ieee8023_dscp(void *buf)
{
	uint8_t tos;
	struct ipv4_hdr *hdr = (struct ipv4_hdr *)buf;

	tos = IPH_TOS(hdr);

	return (tos & 0xfc) >> 5;
}

/* extract flow control field */
static uint8_t ipv6_ieee8023_dscp(void *buf)
{
	uint8_t tos;
	struct ipv6_hdr *hdr = (struct ipv6_hdr *)buf;

	tos = IP6H_FL(hdr);

	return (tos & 0xfc) >> 5;
}

/*
 * get user priority from @buf.
 * ipv4 dscp/tos, ipv6 flow control. for eapol packets, disable qos.
 */
__IRAM2 static inline uint8_t classify8021d(UINT8 *buf)
{
	struct ethhdr *ethhdr = (struct ethhdr *)buf;

	switch (PP_HTONS(ethhdr->proto)) {
#if CONFIG_WFA_CERT
	case ETHTYPE_IP:
		return ipv4_ieee8023_dscp(ethhdr + 1);
	case ETHTYPE_IPV6:
		return ipv6_ieee8023_dscp(ethhdr + 1);
#endif
	case ETH_P_PAE:
		return 7;	/* TID7 highest user priority */
	default:
		return 0;   /* TID0: mapped to AC_BE */
	}
}


/**
 * rwnx_set_more_data_flag - Update MORE_DATA flag in tx sw desc
 *
 * @rwnx_hw: Driver main data
 * @sw_txhdr: Header for pkt to be pushed
 *
 * If STA is in PS mode
 *  - Set EOSP in case the packet is the last of the UAPSD service period
 *  - Set MORE_DATA flag if more pkt are ready for this sta
 *  - Update TIM if this is the last pkt buffered for this sta
 *
 * note: tx_lock already taken.
 */
__IRAM2 static void rwnx_set_more_data_flag(struct sk_buff *skb)
{
    void *sta = sta_mgmt_get_entry(skb->sta_idx);
    struct rwnx_txq *txq = skb->txq;
    struct rwnx_sta_ps *ps;
	struct txdesc *txdesc = &skb->ftxdesc->txdesc;

	ps = sta_mgmt_get_sta_ps(sta);

    if (unlikely(ps->active)) {
        ps->pkt_ready[txq->ps_id]--;
        // sta->ps.sp_cnt[txq->ps_id]--;

        // trace_ps_push(sta);

        if (((txq->ps_id == UAPSD_ID) /*|| (vif->wdev.iftype == NL80211_IFTYPE_MESH_POINT)*/)
                && !ps->sp_cnt[txq->ps_id]) {
            txdesc->host.flags |= TXU_CNTRL_EOSP;
        }

        if (ps->pkt_ready[txq->ps_id]) {
            txdesc->host.flags |= TXU_CNTRL_MORE_DATA;
        } else {
            rwnx_set_traffic_status(sta, false, txq->ps_id);
        }
    }
}

/**
 *  rwnx_tx_push - Push one packet to fw
 *
 * @rwnx_hw: Driver main data
 * @txhdr: tx desc of the buffer to push
 * @flags: push flags (see @rwnx_push_flags)
 *
 * Push one packet to fw. Sw desc of the packet has already been updated.
 * Only MORE_DATA flag will be set if needed.
 */
__IRAM2 void rwnx_tx_push(struct sk_buff *skb)
{
	struct rwnx_txq *txq = skb->txq;
	struct txdesc *txdesc = &skb->ftxdesc->txdesc;
	int ret;

	if (txq->nb_retry) {
		txq->nb_retry--;
		if (txq->nb_retry == 0) {
			BK_ASSERT((struct sk_buff *)skb == txq->last_retry_skb);
			txq->last_retry_skb = NULL;
		}
	}

	// Set More Data Bit / EOSP bit
	if (skb->sta_idx != INVALID_STA_IDX)
		rwnx_set_more_data_flag(skb);

	// Push to UMAC
	ret = fhost_txbuf_push(txdesc, skb->ac);
	if (ret){
		if(txdesc->host.flags & TXU_CNTRL_RETRY){
		RWNX_LOGE("%s: retry frame discarded!\r\n", __func__);
		}
		goto tx_exit;
	}
	// If successfully push to umac, decrease txq credits
	txq->credits--;
	if (txq->credits <= 0)
		rwnx_txq_stop(txq, RWNX_TXQ_STOP_FULL);

	return;
tx_exit:
	if (skb)
	{
		#if CONFIG_RWNX_TD
		rwnx_td_pkt_dec(skb->vif_idx,skb->ac);
		#endif
		kfree_skb(skb);
	}
}

__IRAM2 static struct rwnx_txq *rwnx_select_txq(struct sk_buff *skb)
{
	void *rwnx_vif;
	struct rwnx_txq *txq = NULL;
	ETH_HDR_PTR eth_hdr_ptr;
	UINT8 *content_ptr;
	void *sta = NULL;

	/* default to no sta, packet will be dropped */
	skb->priority = PRIO_STA_NULL;

	content_ptr = skb->p->payload;
	eth_hdr_ptr = (ETH_HDR_PTR)content_ptr;

	rwnx_vif = rwm_mgmt_vif_idx2ptr(skb->vif_idx);
	if (!rwnx_vif) {
		RWNX_LOGI("%s: vif is NULL!\r\n", __func__);
		return NULL;
	}

	if (likely(mac_vif_mgmt_get_active(rwnx_vif))) {
		uint8_t sta_idx = rwm_mgmt_tx_get_staidx(skb->vif_idx, eth_hdr_ptr->e_dest);
		if (sta_idx != INVALID_STA_IDX)
			sta = sta_mgmt_get_entry(sta_idx);
	}

	if (sta && (!sta_mgmt_get_valid(sta)))
	{
		RWNX_LOGI("%s: sta is invalid!\r\n", __func__);
		return NULL;
	}

	if (sta && (mac_sta_mgmt_get_capa_flags(sta) & STA_QOS_CAPA)) {
		skb->priority = classify8021d((uint8_t *)eth_hdr_ptr);
		rwnx_downgrade_ac(rwnx_vif, skb);

		txq = rwnx_txq_sta_get(sta, skb->priority);
		skb->ac = mac_tid2ac[skb->priority];
	} else if (sta) {
		// Non Qos: BC/MC
		txq = rwnx_txq_sta_get(sta, 0);
		skb->priority = 0xFF;
		skb->ac = AC_BE;
	}
	if (txq) {
		if (!txq->hwq) {
			RWNX_LOGI("%s: vif_idx %d, rwnx_vif %p, dst mac %pm, sta %p\n",
				__func__, skb->vif_idx, rwnx_vif, eth_hdr_ptr->e_dest, sta);
		}
		BK_ASSERT(txq->hwq);
	}
	return txq;
}

__ITCM_N uint32_t rwnx_tx_get_pbuf_chain_cnt(struct pbuf *p)
{
	struct pbuf *p_ptr = p;
	uint32_t p_cnt = 0;

	while (p_ptr != NULL)
	{
		p_cnt++;
		p_ptr = p_ptr->next;
	}

	return p_cnt;
}

/**
 *    Called when a packet needs to be transmitted.
 *  - Initialize the desciptor for this pkt (stored in skb before data)
 *  - Push the pkt in the corresponding Txq
 *  - If possible (i.e. credit available and not in PS) the pkt is pushed
 *    to fw
 */
#if CONFIG_RWNX_PROTO_DEBUG
extern uint8_t proto_debug_flag;
#endif

__IRAM2 static inline int fhost_txdesc_extra_size(void)
{
	return txdesc_extra_size() + 8;
}

__IRAM2 static int rwnx_sg_init(struct sk_buff *skb, uint32_t seg_addr[], int seg_len[], int *seg_cnt)
{
	int i;
	struct pbuf *p = skb->p;
	int max_sg_size = macif_max_sg_size();

	for (i = 0; p && i < max_sg_size; i++) {
		if (i == 0) {
			/* pbd[0] trim ethernet hdr */
			seg_addr[i] = (uint32_t)p->payload + sizeof(ETH_HDR_T);
			seg_len[i] = p->len - sizeof(ETH_HDR_T);
		} else {
			seg_addr[i] = (uint32_t)p->payload;
			seg_len[i] = p->len;
		}
		p = p->next;
	}

	*seg_cnt = i;

	if (p && i == max_sg_size) {
		int remain = 0;

		while (p) {
			remain++;
			p = p->next;
		}

		RWNX_LOGE("not all data are pushed, remain pbuf %d, pbd %d\n", remain, i);

		return -1;
	}

	return 0;
}

__ITCM_N int rwnx_start_xmit(uint8_t vif_idx, struct pbuf *p, BUS_MSG_T *msg)
{
	struct fhost_tx_desc_tag *fhost_txdesc;
	struct hostdesc *host;
	struct txdesc *txdesc;
	ETH_HDR_PTR eth_hdr_ptr;
	struct rwnx_txq *txq;
	struct sk_buff *skb;
	int ret = 0;
	bool more_pbuf = !!(rwnx_hw_mm_features() & (1ULL << MM_FEAT_MORE_TBD_BIT));
	uint32_t p_cnt = 0;
	int max_sg_size = macif_max_sg_size();
	// TBD: dynamic alloc array in stack
    uint32_t seg_addr[max_sg_size];
    int seg_len[max_sg_size];
    int seg_cnt = max_sg_size;

	//RWNX_LOGI("%s: sizeof(fhost_tx_desc_tag)=%d, sizeof(txdesc)=%d, more_pbuf %d\n", __func__,
	//	sizeof(*fhost_txdesc), sizeof(*txdesc), more_pbuf);

	skb = alloc_skb_with_pbuf(p);
	if (!skb) {
		RWNX_LOGI("rwnx_start_xmit no node\r\n");
		goto exit;
	}

	if (rwnx_sg_init(skb, seg_addr, seg_len, &seg_cnt)) {
		goto exit;
	}

	// Fill skb members
	skb->vif_idx = vif_idx;
	//skb->p = p;
	skb->cb = msg->cb;
	skb->args = msg->param;
	eth_hdr_ptr = (ETH_HDR_PTR)p->payload;
#if CONFIG_RWNX_PROTO_DEBUG
	if(proto_debug_flag) {
		rwm_proto_printf(0, (struct ethhdr *)eth_hdr_ptr);
	}
#endif
	skb->sta_idx = rwm_mgmt_tx_get_staidx(vif_idx, &eth_hdr_ptr->e_dest);

	// Select TXQ
	txq = rwnx_select_txq(skb);
	if (!txq || skb->priority == PRIO_STA_NULL) {
		goto exit;
	}
	// TXQ not initialized
	if (txq->idx == TXQ_INACTIVE || !txq->hwq) {
		RWNX_LOGE("%s %d: txq %p, idx %d, hwq %p\n", __func__, __LINE__, txq, txq->idx, txq->hwq);
		BK_ASSERT(0);
		goto exit;
	}
	skb->txq = txq;

	if (more_pbuf)
		p_cnt = rwnx_tx_get_pbuf_chain_cnt(p);

	// alloc tx desc
	fhost_txdesc = (struct fhost_tx_desc_tag *)os_zalloc_sram(sizeof(struct fhost_tx_desc_tag) + fhost_txdesc_extra_size() + p_cnt * sizeof(struct tx_pbd));

	if (!fhost_txdesc)
		goto exit;

	// Fill skb members
	skb->ftxdesc = fhost_txdesc;

	// descriptor
	txdesc = &fhost_txdesc->txdesc;
	host = &txdesc->host;

	//fill host desc
#if NX_VERSION >= NX_VERSION_PACK(6, 22, 0, 0)
	host->hostid = skb;
#else
	host->buf = skb;
#endif
	os_memcpy(&host->eth_dest_addr, &eth_hdr_ptr->e_dest, ETH_ALEN);
	os_memcpy(&host->eth_src_addr, &eth_hdr_ptr->e_src, ETH_ALEN);
#if NX_AMSDU_TX || NX_VERSION >= NX_VERSION_PACK(6, 22, 0, 0)
	host->packet_addr[0]   = CPU2HW(p->payload + sizeof(ETH_HDR_T));
	host->packet_len[0]    = skb->len - sizeof(ETH_HDR_T);
#if NX_AMSDU_TX
	host->packet_cnt	   = 1;
#endif
#else
	host->packet_addr	   = CPU2HW(p->payload + sizeof(ETH_HDR_T));
	host->packet_len	   = skb->len - sizeof(ETH_HDR_T);
#endif
#if !NX_FULLY_HOSTED
	host.status_desc_addr  = (uint32_t)p->payload + sizeof(ETH_HDR_T);
#endif
	host->ethertype 	   = eth_hdr_ptr->e_proto;
	// if (rwnx_use_lowest_rate(eth_hdr_ptr))
	//	host->flags |= TXU_CNTRL_MGMT_LOWEST_RATE;
	host->tid = skb->priority;
	host->vif_idx = skb->vif_idx;
	host->staid = skb->sta_idx;

	// if frame that doesn't need to open TD window, set SPECIAL_FRAME flag
	if (msg->type == BMSG_SPECIAL_TX_TYPE)
		host->flags |= TXU_CNTRL_IS_SPECIAL_FRAME;

	if (fhost_txdesc_init(txdesc, seg_addr, seg_len, seg_cnt))
		goto exit;

	// Set skb jiffies
	skb->jiffies = bk_get_tick();

#if NX_VERSION > NX_VERSION_PACK(6, 22, 0, 0)
	rwnx_tx_postprocess(vif_idx, skb);
#endif

#if 1
	if (skb_queue_len(&txq->sk_list) >= 3 * NX_DEFAULT_TX_CREDIT_CNT) {
		RWNX_LOGD("txq full\n");
		goto exit;
	}
#else
	if (rwnx_txq_is_full(txq)) {
		RWNX_LOGW("txq full\n");
		goto exit;
	}
#endif

	// Queue skb
	if (rwnx_txq_queue_skb(skb, txq, 0, NULL))
		rwnx_hwq_process(txq->hwq);

	return 0;

exit:
	// free skb/pbuf
	if (skb)
		kfree_skb(skb);
	else
#if BK_SS_WIFI_DP
		bk_wlan_buf_free(p);
#else
		pbuf_free(p);
#endif
	return ret;
}

/**
 * transmit mgmt frame
 * FIXME: 1. non-CCK; 2. offchannel tx
 */
void rwnx_start_xmit_mgmt(struct sk_buff *skb)
{
	struct hostdesc *host;
	struct txdesc *txdesc;
	struct fhost_tx_desc_tag *fhost_txdesc = NULL;
	bool robust;
	struct rwnx_txq *txq;
	struct ieee80211_mgmt *mac_hdr;
	bool more_pbuf = !!(rwnx_hw_mm_features() & (1ULL << MM_FEAT_MORE_TBD_BIT));
	int p_cnt = 0;

	//RWNX_LOGI("%s: sizeof(fhost_tx_desc_tag)=%d, sizeof(txdesc)=%d\n", __func__,
	//	sizeof(*fhost_txdesc), sizeof(*txdesc));

	// sanity check
	if (skb->vif_idx == INVALID_VIF_IDX)
		goto tx_exit;

	if (more_pbuf)
		p_cnt  = MGMT_FRAME_MAX_TX_PBD_CNT;

	fhost_txdesc = (struct fhost_tx_desc_tag *)os_zalloc_sram(sizeof(struct fhost_tx_desc_tag) + fhost_txdesc_extra_size() + p_cnt * sizeof(struct tx_pbd));

	if (!fhost_txdesc)
		goto tx_exit;
	skb->ftxdesc = fhost_txdesc;

	if (skb->sta_idx != INVALID_STA_IDX) {
		txq = rwnx_txq_sta_get(sta_mgmt_get_entry(skb->sta_idx), NX_STA_MGMT_TXQ_IDX);
	} else {
		if (skb->offchannel)
			txq = rwnx_txq_vif_get(mac_vif_mgmt_get_entry(skb->vif_idx), NX_OFF_CHAN_TXQ_IDX);
		else
			txq = rwnx_txq_vif_get(mac_vif_mgmt_get_entry(skb->vif_idx), NX_UNK_TXQ_TYPE);
	}
	if (!txq || txq->idx == TXQ_INACTIVE)
		goto tx_exit;

#if 0	 //TODO
	if (txq->credits <= 0)
		goto tx_exit;
#endif

	skb->txq = txq;

	// initialize variable
	txdesc = &fhost_txdesc->txdesc;
	host = &txdesc->host;

	mac_hdr = (struct ieee80211_mgmt *)skb->msdu_ptr;

	#ifdef CFG_MFP
	robust = mfp_is_robust_frame(mac_hdr->frame_control, *((uint8_t *)(mac_hdr + 1)));
	#else
	robust = false;
	#endif

	// setup hostdesc
	host->flags = TXU_CNTRL_MGMT;
	if (robust)
		host->flags |= TXU_CNTRL_MGMT_ROBUST;
	if (skb->nocck)
		host->flags |= TXU_CNTRL_MGMT_NO_CCK;
#if NX_VERSION >= NX_VERSION_PACK(6, 22, 0, 0)
	host->hostid = skb;
	host->packet_addr[0] = CPU2HW(skb->msdu_ptr);	//mark not-internal frame
	host->packet_len[0] = skb->len;
#else
	host->buf = skb;
	host->packet_addr = CPU2HW(skb->msdu_ptr);	//mark not-internal frame
	host->packet_len = skb->len;
#endif
	host->tid = 0xFF;
	host->vif_idx = skb->vif_idx;
	host->staid = skb->sta_idx;

	// fill lmac desc
	if (fhost_txdesc_mgmt_init(txdesc, p_cnt, (uint32_t)skb->msdu_ptr, skb->len))
		goto tx_exit;

	// VO for all mgmt frames
	skb->ac = AC_VO;

	// Set skb jiffies
	skb->jiffies = bk_get_tick();

	// Queue skb
	if (rwnx_txq_queue_skb(skb, txq, 0, NULL))
		rwnx_hwq_process(txq->hwq);

	return;

tx_exit:
	if (skb)
		kfree_skb(skb);
	else if (fhost_txdesc)
		os_free(fhost_txdesc);
}

void rwnx_start_xmit_raw_ex(struct sk_buff *skb, raw_tx_cntrl_t *raw_tx_cntrl)
{
	struct hostdesc *host;
	struct txdesc *txdesc;
	struct fhost_tx_desc_tag *fhost_txdesc = NULL;
	uint8_t queue_idx = 0;
	uint16_t raw_timeout = 0;
	//bool robust;
	struct rwnx_txq *txq;
	bool more_pbuf = !!(rwnx_hw_mm_features() & (1ULL << MM_FEAT_MORE_TBD_BIT));
	int p_cnt = 0;

	bk_wifi_get_tx_raw_ac(&queue_idx);
	bk_wifi_get_tx_raw_timeout(&raw_timeout);

	// sanity check
	if (skb->vif_idx == INVALID_VIF_IDX)
		goto tx_exit;

	if (more_pbuf)
		p_cnt = MGMT_FRAME_MAX_TX_PBD_CNT;

	fhost_txdesc = (struct fhost_tx_desc_tag *)os_zalloc_sram(sizeof(struct fhost_tx_desc_tag) + fhost_txdesc_extra_size() + p_cnt * sizeof(struct tx_pbd));

	if (!fhost_txdesc)
		goto tx_exit;

	skb->ftxdesc = fhost_txdesc;

	if (skb->sta_idx != INVALID_STA_IDX) {
		txq = rwnx_txq_sta_get(sta_mgmt_get_entry(skb->sta_idx), NX_STA_MGMT_TXQ_IDX);
	} else {
		if (skb->offchannel)
			txq = rwnx_txq_vif_get(mac_vif_mgmt_get_entry(skb->vif_idx), NX_OFF_CHAN_TXQ_IDX);
		else
			txq = rwnx_txq_vif_get(mac_vif_mgmt_get_entry(skb->vif_idx), NX_UNK_TXQ_TYPE);
	}

	if (!txq || txq->idx == TXQ_INACTIVE)
		goto tx_exit;

#if 0	 //TODO
	if (txq->credits <= 0)
		goto tx_exit;
#endif

	skb->txq = txq;

	// initialize variable
	txdesc = &fhost_txdesc->txdesc;
	host = &txdesc->host;

	fhost_txdesc_raw_ext_init(txdesc, (uint32_t)skb->msdu_ptr, skb->len,
		raw_tx_cntrl->tx_retry_cnt, raw_tx_cntrl->tx_rate, raw_tx_cntrl->tx_power);

	// update tx timeout to 50ms
	txl_cntrl_set_timeout_per_ac(queue_idx, raw_timeout);

	// setup hostdesc
	host->flags = TXU_CNTRL_MGMT;
	if (skb->nocck)
		host->flags |= TXU_CNTRL_MGMT_NO_CCK;
#if NX_VERSION >= NX_VERSION_PACK(6, 22, 0, 0)
	host->hostid = skb;
	host->packet_addr[0] = CPU2HW(skb->msdu_ptr);	//mark not-internal frame
	host->packet_len[0] = skb->len;
#else
	host->buf = skb;
	host->packet_addr = CPU2HW(skb->msdu_ptr);	//mark not-internal frame
	host->packet_len = skb->len;
#endif
	host->tid = 0xFF;
	host->vif_idx = skb->vif_idx;
	host->staid = skb->sta_idx;

    host->flags |= TXU_CNTRL_INDEPENDENT_PT;

	// VO for all mgmt frames
	skb->ac = queue_idx;

	// Set skb jiffies
	skb->jiffies = bk_get_tick();

	// Queue skb
	if (rwnx_txq_queue_skb(skb, txq, 0, NULL))
		rwnx_hwq_process(txq->hwq);

	return;

tx_exit:
	if (skb)
		kfree_skb(skb);
	else if (fhost_txdesc)
		os_free(fhost_txdesc);
}
#endif /* CONFIG_RWNX_SW_TXQ */

/*
 * Push the confirmation to the FHOST, refer rwnx_drv: rwnx_txdatacfm
 *
 */
__IRAM2 void fhost_tx_cfm_push(uint8_t queue_idx, struct txdesc *txdesc)
{
	struct tx_cfm_tag *cfm = &txdesc->host.cfm;
#if CONFIG_RWNX_SW_TXQ
	struct rwnx_txq *txq;
	struct sk_buff *skb;
#endif

	if (cfm->status & TX_STATUS_DONE) {
		if (txdesc->host.flags & TXU_CNTRL_MGMT) {
#if 0
			/* Confirm transmission to CFG80211 */
			cfg80211_mgmt_tx_status(&sw_txhdr->rwnx_vif->wdev,
									(unsigned long)skb,
									(skb->data + sw_txhdr->headroom),
									sw_txhdr->frame_len,
									rwnx_txst.acknowledged,
									GFP_ATOMIC);
#endif
		}

		//retry xmit: packet has been transmitted but not acknowledged, driver must repush it.
		if (cfm->status & TX_STATUS_RETRY_REQUIRED) {
			/* retransmit the frame: BA bit lost */
#if CONFIG_RWNX_SW_TXQ
#if NX_VERSION >= NX_VERSION_PACK(6, 22, 0, 0)
			skb = (struct sk_buff *)txdesc->host.hostid;
			txdesc->host.flags |= TXU_CNTRL_REUSE_SN;
			txdesc->host.sn_for_retry = (cfm->status & TX_STATUS_SN_MSK) >> TX_STATUS_SN_OFT;
#else
			skb = (struct sk_buff *)txdesc->host.buf;
			txdesc->host.flags |= TXU_CNTRL_RETRY;
#endif
			txdesc->host.flags &= ~TXU_CNTRL_MORE_DATA;

			cfm->status = 0;

			txq = skb->txq;
			if (!txq) {
				goto err_tx;
			}
			/* This txq if flushed before confirmed:
			 * 1. STA sends one packet, waits for txl confirm
			 * 2. STA disconnected from AP, packets are flushed by sw txq
			 * 3. confirm cb called
			 */
			if (txq->idx == TXQ_INACTIVE)
				goto err_tx;

			txq->credits++;
			if (txq->credits > 0)
				rwnx_txq_start(txq, RWNX_TXQ_STOP_FULL);

			if (rwnx_txq_queue_skb(skb, txq, 1, NULL))
				rwnx_hwq_process(txq->hwq);
#else /* !CONFIG_RWNX_SW_TXQ */

#if NX_VERSION >= NX_VERSION_PACK(6, 22, 0, 0)
			txdesc->host.flags |= TXU_CNTRL_REUSE_SN;
			txdesc->host.sn_for_retry = (cfm->status & TX_STATUS_SN_MSK) >> TX_STATUS_SN_OFT;
#else
			txdesc->host.flags |= TXU_CNTRL_RETRY;
#endif
			txdesc->host.flags &= ~TXU_CNTRL_MORE_DATA;
			cfm->status = 0;

			if (fhost_txbuf_prepend(txdesc, queue_idx) != kNoErr)
				goto err_tx;
#endif
		} else if (cfm->status & TX_STATUS_SW_RETRY_REQUIRED) {
#if CONFIG_RWNX_SW_TXQ
			// software retry: need to set pn, rebuild frame header, mic, etc.
			cfm->status = 0;

			// uint8_t *eth_payload = HW2CPU(txdesc->host.packet_addr);
			// ETH_HDR_T *eth_hdr = (ETH_HDR_T *)eth_payload - 1;
#if NX_VERSION >= NX_VERSION_PACK(6, 22, 0, 0)
			skb = (struct sk_buff *)txdesc->host.hostid;
#else
			skb = (struct sk_buff *)txdesc->host.buf;
#endif
			txq = skb->txq;
			if (!txq) {
				goto err_tx;
			}
			/* This txq if flushed before confirmed:
			 * 1. STA sends one packet, waits for txl confirm
			 * 2. STA disconnected from AP, packets are flushed by sw txq
			 * 3. confirm cb called
			 */
			if (txq->idx == TXQ_INACTIVE)
				goto err_tx;

			//Do NOT re-transmit the packet if retry count has reached the limit
			if (txdesc->host.sw_retry_cnt++ >= CFG_MAX_SW_RETRY_CNT) {
				RWNX_LOGI("SW retry has reached the limit\r\n");
				txdesc->host.sw_retry_cnt = 0;
				goto err_tx;
			}

			txq->credits++;
			if (txq->credits > 0)
				rwnx_txq_start(txq, RWNX_TXQ_STOP_FULL);

			if (rwnx_txq_queue_skb(skb, txq, 1, NULL))
				rwnx_hwq_process(txq->hwq);
#else
			//Do NOT re-transmit the packet if retry count has reached the limit
			if (txdesc->host.sw_retry_cnt++ >= CFG_MAX_SW_RETRY_CNT) {
				RWNX_LOGI("SW retry has reached the limit\r\n");
				txdesc->host.sw_retry_cnt = 0;
				goto err_tx;
			}

			// software retry: need to set pn, rebuild frame header, mic, etc.
			cfm->status = 0;

			if (fhost_txbuf_prepend(txdesc, queue_idx) != kNoErr)
				goto err_tx;
#endif
		} else if (cfm->status & TX_STATUS_ACKNOWLEDGED) {
			//xmit successfully, run callback
			rwnx_tx_confirm(txdesc);
			//dbg(D_INF D_FHOST "FHOST: xmit one packet success by queue(%0d)\r\n", queue_idx);
		} else {
err_tx:
			rwnx_tx_confirm(txdesc);
			//dbg(D_INF D_FHOST "FHOST: xmit one packet failed by queue(%0d)\r\n", queue_idx);
		}
	} else {
		//dbg(D_ERR D_FHOST "FHOST: Rx one error tx_cfm\r\n");
	}
}

