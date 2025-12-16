#include "bk_compiler.h"
#include "rw_msdu.h"
#include "bk_rw.h"
#include "rwnx_config.h"
#include "components/system.h"

#if CONFIG_RWNX_SW_TXQ
#include "rwnx_txq.h"
#include "bk_fake_clock.h"
#include "rwnx_defs.h"
#include "rwnx_txq.h"
#include "rwnx_tx.h"
#include "skbuff.h"
//#include "trace.h"
#include "rwnx_version.h"
#include "fhost_msg.h"

#if CONFIG_RWNX_TD
#include "rwnx_td.h"
#endif

#define TRACE_TXQ(lvl,...)
#define TRACE_BUF_TXQ(lvl,...)

void bk_wifi_sw_txq_event_clear(void);
void bk_wifi_sw_txq_event_set(void);

#if CONFIG_WFA_CERT
const int nx_tid_prio[NX_NB_TID_PER_STA] = {7, 6, 5, 4, 3, 0, 2, 1};
#else
const int nx_tid_prio[NX_NB_TID_PER_STA] = {4};
#endif
void bmsg_tx_drop_traffic_sender(uint8_t arg);
ke_state_t ke_state_get(ke_task_id_t const id);

const int rwnx_tid2hwq[16] = {
	RWNX_HWQ_BE,
	RWNX_HWQ_BK,
	RWNX_HWQ_BK,
	RWNX_HWQ_BE,
	RWNX_HWQ_VI,
	RWNX_HWQ_VI,
	RWNX_HWQ_VO,
	RWNX_HWQ_VO,
	/* TID_8 is used for management frames */
	RWNX_HWQ_VO,
	/* At the moment, all others TID are mapped to BE */
	RWNX_HWQ_BE,
	RWNX_HWQ_BE,
	RWNX_HWQ_BE,
	RWNX_HWQ_BE,
	RWNX_HWQ_BE,
	RWNX_HWQ_BE,
	RWNX_HWQ_BE,
};

//#undef TRACE_TXQ
//#define TRACE_TXQ(mod, ...) os_printf(__VA_ARGS__)

__IRAM2 static inline int rwnx_txq_sta_idx(STA_INF_PTR sta, u8 tid)
{
#if !CONFIG_WFA_CERT
	/* For Non WFA Certification, SW only use one tid */
	tid = 0;
#endif
	if (is_multicast_sta(mac_sta_mgmt_get_staid(sta)))
		return NX_FIRST_VIF_TXQ_IDX + mac_sta_mgmt_get_inst_nbr(sta);
	else
		return (mac_sta_mgmt_get_staid(sta) * NX_NB_TXQ_PER_STA) + tid;
}

__IRAM2 static inline int rwnx_txq_vif_idx(VIF_INF_PTR vif, u8 type)
{
	return NX_FIRST_VIF_TXQ_IDX + mac_vif_mgmt_get_index(vif) + (type * NX_VIRT_DEV_MAX);
}

__IRAM2 inline struct rwnx_txq *rwnx_txq_sta_get(STA_INF_PTR sta, u8 tid)
{
	if (tid >= NX_NB_TXQ_PER_STA)
		tid = 0;

#if CONFIG_WFA_CERT
	int index = rwnx_txq_sta_idx(sta, tid);
	// RWNX_LOGI("XXX %s: tid %d, q index %d\n", __func__, tid, index);

	if (tid != 0 && index == 0)
		BK_ASSERT(0); /* ASSERT VERIFIED */
#endif

	return &g_rwnx_hw.txq[rwnx_txq_sta_idx(sta, tid)];
}

__IRAM2 inline struct rwnx_txq *rwnx_txq_vif_get(VIF_INF_PTR vif, u8 type)
{
	if (type > NX_UNK_TXQ_TYPE)
		type = NX_BCMC_TXQ_TYPE;

	return &g_rwnx_hw.txq[rwnx_txq_vif_idx(vif, type)];
}


/******************************************************************************
 * Init/Deinit functions
 *****************************************************************************/
/**
 * rwnx_txq_init - Initialize a TX queue
 *
 * @txq: TX queue to be initialized
 * @idx: TX queue index
 * @status: TX queue initial status
 * @hwq: Associated HW queue
 * @ndev: Net device this queue belongs to
 *        (may be null for non netdev txq)
 *
 * Each queue is initialized with the credit of @NX_TXQ_INITIAL_CREDITS.
 */
static void rwnx_txq_init(struct rwnx_txq *txq, int idx, u16 status,
						  struct rwnx_hwq *hwq, int tid,
						  STA_INF_PTR sta)
{
	TRACE_TXQ(TXQ, "%s: txq %p, status 0x%x", __func__, txq, status);

	txq->idx = idx;
	txq->status = status;
	txq->credits = NX_DEFAULT_TX_CREDIT_CNT;
	skb_queue_head_init(&txq->sk_list);
	txq->last_retry_skb = NULL;
	txq->nb_retry = 0;
	txq->hwq = hwq;
	txq->sta = sta;
	txq->tid = tid;
	if (idx < NX_FIRST_VIF_TXQ_IDX) {
		int sta_idx = mac_sta_mgmt_get_staid(sta);
		int tid_local = idx - (sta_idx * NX_NB_TXQ_PER_STA);
		if (tid_local < NX_NB_TID_PER_STA)
			txq->ndev_idx = NX_STA_NDEV_IDX(tid_local, sta_idx);
		else
			txq->ndev_idx = NDEV_NO_TXQ;
	} else if (idx < NX_FIRST_UNK_TXQ_IDX) {
		txq->ndev_idx = NX_BCMC_TXQ_NDEV_IDX;
	} else {
		txq->ndev_idx = NDEV_NO_TXQ;
	}
}


/**
 * rwnx_txq_drop_skb - Drop the buffer skb from the TX queue
 *
 * @txq:          TX queue
 * @skb:          skb packet that should be dropped.
 * @rwnx_hw:      Driver main data
 * @retry_packet: Is it a retry packet
 *
 */
void rwnx_txq_drop_skb(struct rwnx_txq *txq, struct sk_buff *skb, bool retry_packet)
{
	MSDU_NODE_T *msdu = (MSDU_NODE_T *)skb;
	/*Pointer 'skb_peek( &txq->sk_list)' returned from call to function 'skb_peek' at line 569 may be NULL and will be dereferenced at line 569.*/
	if (!skb)
		return;
	#if CONFIG_RWNX_TD
	rwnx_td_pkt_dec(skb->vif_idx,skb->ac);
	#endif

	skb_unlink(skb, &txq->sk_list);

	kfree_skb(msdu);

	if (retry_packet) {
		txq->nb_retry--;
		if (txq->nb_retry == 0) {
			WARN(skb != txq->last_retry_skb,
				 "last dropped retry buffer is not the expected one");
			txq->last_retry_skb = NULL;
		}
	}
}

/**
 * rwnx_txq_flush - Flush all buffers queued for a TXQ
 *
 * @rwnx_hw: main driver data
 * @txq: txq to flush
 */
void rwnx_txq_flush(struct rwnx_txq *txq)
{
	while (!skb_queue_empty(&txq->sk_list))
		rwnx_txq_drop_skb(txq, skb_peek(&txq->sk_list), txq->nb_retry);
}

/**
 * rwnx_txq_deinit - De-initialize a TX queue
 *
 * @rwnx_hw: Driver main data
 * @txq: TX queue to be de-initialized
 * Any buffer stuck in a queue will be freed.
 */
static void rwnx_txq_deinit(struct rwnx_txq *txq)
{
	if (txq->idx == TXQ_INACTIVE)
		return;

	spin_lock_bh(&g_rwnx_hw.tx_lock);
	rwnx_txq_del_from_hw_list(txq);
	txq->idx = TXQ_INACTIVE;
	spin_unlock_bh(&g_rwnx_hw.tx_lock);

	rwnx_txq_flush(txq);
}

/**
 * rwnx_txq_vif_init - Initialize all TXQ linked to a vif
 *
 * @rwnx_hw: main driver data
 * @vif_info_tag: Pointer on VIF
 * @status: Intial txq status
 *
 * Fullmac : 1 VIF TXQ for BC/MC
 *           1 VIF TXQ for MGMT to unknown STA
 */
void rwnx_txq_vif_init(VIF_INF_PTR rwnx_vif, u16 status)
{
	struct rwnx_txq *txq;
	int idx;

	RWNX_LOGD("%s %d\n", __func__, __LINE__);

	txq = rwnx_txq_vif_get(rwnx_vif, NX_BCMC_TXQ_TYPE);
	idx = rwnx_txq_vif_idx(rwnx_vif, NX_BCMC_TXQ_TYPE);
	rwnx_txq_init(txq, idx, status, &g_rwnx_hw.hwq[RWNX_HWQ_BE], 0,
				  sta_mgmt_get_entry(VIF_TO_BCMC_IDX(mac_vif_mgmt_get_index(rwnx_vif))));

	txq = rwnx_txq_vif_get(rwnx_vif, NX_UNK_TXQ_TYPE);
	idx = rwnx_txq_vif_idx(rwnx_vif, NX_UNK_TXQ_TYPE);
	rwnx_txq_init(txq, idx, status, &g_rwnx_hw.hwq[RWNX_HWQ_VO], TID_MGT, NULL);
}

/**
 * rwnx_txq_vif_deinit - Deinitialize all TXQ linked to a vif
 *
 * @rwnx_hw: main driver data
 * @vif_info_tag: Pointer on VIF
 */
void rwnx_txq_vif_deinit(VIF_INF_PTR rwnx_vif)
{
	struct rwnx_txq *txq;

	txq = rwnx_txq_vif_get(rwnx_vif, NX_BCMC_TXQ_TYPE);
	rwnx_txq_deinit(txq);

	txq = rwnx_txq_vif_get(rwnx_vif, NX_UNK_TXQ_TYPE);
	rwnx_txq_deinit(txq);
}


/**
 * rwnx_txq_sta_init - Initialize TX queues for a STA
 *
 * @rwnx_hw: Main driver data
 * @rwnx_sta: STA for which tx queues need to be initialized
 * @status: Intial txq status
 *
 * This function initialize all the TXQ associated to a STA.
 * Softmac : 1 TXQ per TID
 *
 * Fullmac : 1 TXQ per TID (limited to 8)
 *           1 TXQ for MGMT
 */
void rwnx_txq_sta_init(STA_INF_PTR rwnx_sta, u16 status)
{
	struct rwnx_txq *txq;
	int tid, idx;

	idx = rwnx_txq_sta_idx(rwnx_sta, 0);

	foreach_sta_txq(rwnx_sta, txq, tid) {
		rwnx_txq_init(txq, idx, status, &g_rwnx_hw.hwq[rwnx_tid2hwq[tid]], tid,
					  rwnx_sta);
		idx++;
	}
}

/**
 * rwnx_txq_sta_deinit - Deinitialize TX queues for a STA
 *
 * @rwnx_hw: Main driver data
 * @rwnx_sta: STA for which tx queues need to be deinitialized
 */
void rwnx_txq_sta_deinit(STA_INF_PTR rwnx_sta)
{
	struct rwnx_txq *txq;
	int tid;

	foreach_sta_txq(rwnx_sta, txq, tid) {
		rwnx_txq_deinit(txq);
	}
}

/**
 * rwnx_txq_unk_vif_init - Initialize TXQ for unknown STA linked to a vif
 *
 * @vif_info_tag: Pointer on VIF
 */
void rwnx_txq_unk_vif_init(VIF_INF_PTR rwnx_vif)
{
	struct rwnx_txq *txq;
	int idx;

	txq = rwnx_txq_vif_get(rwnx_vif, NX_UNK_TXQ_TYPE);
	idx = rwnx_txq_vif_idx(rwnx_vif, NX_UNK_TXQ_TYPE);
	rwnx_txq_init(txq, idx, 0, &g_rwnx_hw.hwq[RWNX_HWQ_VO], TID_MGT, NULL);
}

/**
 * rwnx_txq_unk_vif_deinit - Deinitialize TXQ for unknown STA linked to a vif
 *
 * @vif_info_tag: Pointer on VIF
 */
void rwnx_txq_unk_vif_deinit(VIF_INF_PTR rwnx_vif)
{
	struct rwnx_txq *txq;

	txq = rwnx_txq_vif_get(rwnx_vif, NX_UNK_TXQ_TYPE);
	rwnx_txq_deinit(txq);
}

/**
 * rwnx_txq_offchan_init - Initialize TX queue for the transmission on a offchannel
 *
 * @vif: Interface for which the queue has to be initialized
 *
 * NOTE: Offchannel txq is only active for the duration of the ROC
 */
void rwnx_txq_offchan_init(VIF_INF_PTR rwnx_vif)
{
	struct rwnx_txq *txq;

	txq = &g_rwnx_hw.txq[NX_OFF_CHAN_TXQ_IDX];
	rwnx_txq_init(txq, NX_OFF_CHAN_TXQ_IDX, RWNX_TXQ_STOP_CHAN,
				  &g_rwnx_hw.hwq[RWNX_HWQ_VO], TID_MGT, NULL);
}

/**
 * rwnx_deinit_offchan_txq - Deinitialize TX queue for offchannel
 *
 * @vif: Interface that manages the STA
 *
 * This function deintialize txq for one STA.
 * Any buffer stuck in a queue will be freed.
 */
void rwnx_txq_offchan_deinit(VIF_INF_PTR rwnx_vif)
{
	struct rwnx_txq *txq;

	txq = &g_rwnx_hw.txq[NX_OFF_CHAN_TXQ_IDX];
	rwnx_txq_deinit(txq);
}

/**
 * rwnx_txq_drop_old_traffic - Drop pkt queued for too long in a TXQ
 *
 * @txq: TXQ to process
 * @rwnx_hw: Driver main data
 * @skb_timeout: Max queue duration, in jiffies, for this queue
 * @dropped: Updated to inidicate if at least one skb was dropped
 *
 * @return Whether there is still pkt queued in this queue.
 */
static bool rwnx_txq_drop_old_traffic(struct rwnx_txq *txq,
									  unsigned long skb_timeout, uint32_t *dropped)
{
	struct sk_buff *skb, *skb_next;
	bool pkt_queued = false;
	int retry_packet = txq->nb_retry;
	unsigned long jiffies = bk_get_tick();
	struct rwnx_hw *rwnx_hw = &g_rwnx_hw;

	if (txq->idx == TXQ_INACTIVE)
		return false;

	spin_lock(&rwnx_hw->tx_lock);

	skb_queue_walk_safe(&txq->sk_list, skb, skb_next) {

		if (retry_packet) {
			// Don't drop retry packets
			retry_packet--;
			continue;
		}

		if (!time_after(jiffies, skb->jiffies + skb_timeout)) {
			pkt_queued = true;
			break;
		}

		(*dropped)++;
		rwnx_txq_drop_skb(txq, skb, false);

		if (txq->sta) {
			struct rwnx_sta_ps *ps = sta_mgmt_get_sta_ps(txq->sta);
			if (ps->active) {
				ps->pkt_ready[txq->ps_id]--;
				if (ps->pkt_ready[txq->ps_id] == 0)
					rwnx_set_traffic_status(txq->sta, false, txq->ps_id);
			}
		}
	}

	if (skb_queue_empty(&txq->sk_list))
		rwnx_txq_del_from_hw_list(txq);

	spin_unlock(&rwnx_hw->tx_lock);

	return pkt_queued;
}

/**
 * rwnx_txq_drop_ap_vif_old_traffic - Drop pkt queued for too long in TXQs
 * linked to an "AP" vif (AP, MESH, P2P_GO)
 *
 * @vif: Vif to process
 * @return Whether there is still pkt queued in any TXQ.
 */
static bool rwnx_txq_drop_ap_vif_old_traffic(VIF_INF_PTR vif)
{
	// STA Information entry
	STA_INF_PTR sta;
	unsigned long timeout = BK_MS_TO_TICKS((mac_vif_mgmt_get_u_ap_bcn_int(vif) << 10) / 1000) * 3;	// 3 beacon interval
	//(vif->u.ap.bcn_int * HZ * 3) >> 10;
	bool pkt_queued = false;
	uint32_t pkt_dropped = 0;

	// Should never be needed but still check VIF queues
	rwnx_txq_drop_old_traffic(rwnx_txq_vif_get(vif, NX_BCMC_TXQ_TYPE),
							  RWNX_TXQ_MAX_QUEUE_JIFFIES, &pkt_dropped);
	rwnx_txq_drop_old_traffic(rwnx_txq_vif_get(vif, NX_UNK_TXQ_TYPE),
							  RWNX_TXQ_MAX_QUEUE_JIFFIES, &pkt_dropped);
	WARN(pkt_dropped, "Dropped packet in BCMC/UNK queue");

	sta = mac_vif_mgmt_get_sta_list(vif);

	while (sta != NULL) {
		struct rwnx_txq *txq;
		int tid;
		foreach_sta_txq(sta, txq, tid) {
			pkt_queued |= rwnx_txq_drop_old_traffic(txq,
													/* timeout * sta->listen_interval */ timeout, /* FIXME !!! */
													&pkt_dropped);
		}

		// Get next STA entry
		sta = sta_mgmt_get_next_sta(sta);
	}

	return pkt_queued;
}

/**
 * rwnx_txq_drop_sta_vif_old_traffic - Drop pkt queued for too long in TXQs
 * linked to a "STA" vif. In theory this should not be required as there is no
 * case where traffic can accumulate in a STA interface.
 *
 * @vif: Vif to process
 * @return Whether there is still pkt queued in any TXQ.
 */
static bool rwnx_txq_drop_sta_vif_old_traffic(VIF_INF_PTR vif)
{
	struct rwnx_txq *txq;
	bool pkt_queued = false;
	uint32_t pkt_dropped = 0;
	int tid;
	STA_INF_PTR sta;

	if (mac_vif_mgmt_get_active(vif)) {
		sta = sta_mgmt_get_entry(mac_vif_mgmt_get_u_sta_ap_id(vif));
		foreach_sta_txq(sta, txq, tid) {
			pkt_queued |= rwnx_txq_drop_old_traffic(txq,
													RWNX_TXQ_MAX_QUEUE_JIFFIES,
													&pkt_dropped);
		}
	}

	if (pkt_dropped)
		RWNX_LOGI("Dropped %d packets in STA interface TXQs\n", pkt_dropped);
	return pkt_queued;
}

/**
 * rwnx_txq_cleanup_timer_cb - callack for TXQ cleaup timer
 * Used to prevent pkt to accumulate in TXQ. The main use case is for AP
 * interface with client in Power Save mode but just in case all TXQs are
 * checked.
 *
 * @t: timer structure
 */
void rwnx_txq_cleanup_timer_cb(void *dummy)
{
	bool pkt_queue = false;
	struct rwnx_hw *rwnx_hw = &g_rwnx_hw;
	VIF_INF_PTR vif = mac_vif_mgmt_first_vif();

	while (vif) {
		if (mac_vif_mgmt_get_active(vif)) {
			if (mac_vif_mgmt_get_type(vif) == VIF_AP)
				pkt_queue |= rwnx_txq_drop_ap_vif_old_traffic(vif);
			else if (mac_vif_mgmt_get_type(vif) == VIF_STA)
				pkt_queue |= rwnx_txq_drop_sta_vif_old_traffic(vif);
		}

		vif = (VIF_INF_PTR )mac_vif_mgmt_next_vif(vif);
	}

	if (!pkt_queue)
		rtos_stop_timer(&rwnx_hw->txq_cleanup);
}

static void __rwnx_txq_cleanup_timer_cb(void *dummy)
{
	bmsg_tx_drop_traffic_sender(0);
}

/**
 * rwnx_txq_start_cleanup_timer - Start 'cleanup' timer if not started
 *
 * @rwnx_hw: Driver main data
 */
__IRAM2 void rwnx_txq_start_cleanup_timer(STA_INF_PTR sta)
{
	struct rwnx_hw *rwnx_hw = &g_rwnx_hw;

	if (sta && !is_multicast_sta(mac_sta_mgmt_get_staid(sta)) &&
		!rtos_is_timer_running(&rwnx_hw->txq_cleanup))
		rtos_reload_timer(&rwnx_hw->txq_cleanup);
}


/**
 * rwnx_txq_credit_update - Update credit for one txq
 *
 * @rwnx_hw: Driver main data
 * @sta_idx: STA idx
 * @tid: TID
 * @update: offset to apply in txq credits
 *
 * Called when fw send ME_TX_CREDITS_UPDATE_IND message.
 * Apply @update to txq credits, and stop/start the txq if needed
 */
void rwnx_txq_credit_update(int sta_idx, u8 tid, s8 update)
{
	STA_INF_PTR sta = sta_mgmt_get_entry(sta_idx);
	struct rwnx_txq *txq;
	struct rwnx_hw *rwnx_hw = &g_rwnx_hw;

	txq = rwnx_txq_sta_get(sta, tid);

	spin_lock(&rwnx_hw->tx_lock);

	if (txq->idx != TXQ_INACTIVE) {
		txq->credits += update;
		// trace_credit_update(txq, update);
		if (txq->credits <= 0)
			rwnx_txq_stop(txq, RWNX_TXQ_STOP_FULL);
		else
			rwnx_txq_start(txq, RWNX_TXQ_STOP_FULL);
	}

	// Drop all the retry packets of a BA that was deleted
	if (update < NX_DEFAULT_TX_CREDIT_CNT) {
		int packet;

		for (packet = 0; packet < txq->nb_retry; packet++)
			rwnx_txq_drop_skb(txq, skb_peek(&txq->sk_list), true);
		if (skb_queue_empty(&txq->sk_list))
			rwnx_txq_del_from_hw_list(txq);
	}

	spin_unlock(&rwnx_hw->tx_lock);
}


/**
 * rwnx_txq_prepare - Global initialization of txq
 *
 * @rwnx_hw: Driver main data
 */
void rwnx_txq_prepare()
{
	int i;

	for (i = 0; i < NX_NB_TXQ; i++)
		g_rwnx_hw.txq[i].idx = TXQ_INACTIVE;

	rtos_init_timer(&g_rwnx_hw.txq_cleanup, RWNX_TXQ_CLEANUP_INTERVAL,
					__rwnx_txq_cleanup_timer_cb, 0);
}

/******************************************************************************
 * Start/Stop functions
 *****************************************************************************/
/**
 * rwnx_txq_add_to_hw_list - Add TX queue to a HW queue schedule list.
 *
 * @txq: TX queue to add
 *
 * Add the TX queue if not already present in the HW queue list.
 * To be called with tx_lock hold
 */
__IRAM2 inline void rwnx_txq_add_to_hw_list(struct rwnx_txq *txq)
{
	if (!(txq->status & RWNX_TXQ_IN_HWQ_LIST)) {
		// trace_txq_add_to_hw(txq);
		txq->status |= RWNX_TXQ_IN_HWQ_LIST;
		list_add_tail(&txq->sched_list, &txq->hwq->list);
		txq->hwq->need_processing = true;
	}
}

/**
 * rwnx_txq_del_from_hw_list - Delete TX queue from a HW queue schedule list.
 *
 * @txq: TX queue to delete
 *
 * Remove the TX queue from the HW queue list if present.
 * To be called with tx_lock hold
 */
__IRAM2 inline void rwnx_txq_del_from_hw_list(struct rwnx_txq *txq)
{
	if (txq->status & RWNX_TXQ_IN_HWQ_LIST) {
		// trace_txq_del_from_hw(txq);
		txq->status &= ~RWNX_TXQ_IN_HWQ_LIST;
		list_del(&txq->sched_list);
	}
}

/**
 * rwnx_txq_skb_ready - Check if skb are available for the txq
 *
 * @txq: Pointer on txq
 * @return True if there are buffer ready to be pushed on this txq,
 * false otherwise
 */
__IRAM2 static inline bool rwnx_txq_skb_ready(struct rwnx_txq *txq)
{
	return !skb_queue_empty(&txq->sk_list);
}

/**
 * rwnx_txq_start - Try to Start one TX queue
 *
 * @txq: TX queue to start
 * @reason: reason why the TX queue is started (among RWNX_TXQ_STOP_xxx)
 *
 * Re-start the TX queue for one reason.
 * If after this the txq is no longer stopped and some buffers are ready,
 * the TX queue is also added to HW queue list.
 * To be called with tx_lock hold
 */
__IRAM2 void rwnx_txq_start(struct rwnx_txq *txq, u16 reason)
{
	BK_ASSERT(txq != NULL); /* ASSERT VERIFIED */

	if (txq->idx != TXQ_INACTIVE && (txq->status & reason)) {
		// trace_txq_start(txq, reason);
		TRACE_TXQ(TXQ, "%s: txq %p, status 0x%x, reason 0x%x", __func__,
			  txq, txq->status, reason);

		txq->status &= ~reason;
		if (!rwnx_txq_is_stopped(txq) && rwnx_txq_skb_ready(txq))
			rwnx_txq_add_to_hw_list(txq);

		bk_wifi_sw_txq_event_set();
	}
}

/**
 * rwnx_txq_stop - Stop one TX queue
 *
 * @txq: TX queue to stop
 * @reason: reason why the TX queue is stopped (among RWNX_TXQ_STOP_xxx)
 *
 * Stop the TX queue. It will remove the TX queue from HW queue list
 * To be called with tx_lock hold
 */
void rwnx_txq_stop(struct rwnx_txq *txq, u16 reason)
{
	BK_ASSERT(txq != NULL); /* ASSERT VERIFIED */

	if (txq->idx != TXQ_INACTIVE) {
		TRACE_TXQ(TXQ, "%s: txq %p, status 0x%x, reason 0x%x", __func__,
			  txq, txq->status, reason);

		txq->status |= reason;
		rwnx_txq_del_from_hw_list(txq);
	}
}


/**
 * rwnx_txq_sta_start - Start all the TX queue linked to a STA
 *
 * @sta: STA whose TX queues must be re-started
 * @reason: Reason why the TX queue are restarted (among RWNX_TXQ_STOP_xxx)
 * @rwnx_hw: Driver main data
 *
 * This function will re-start all the TX queues of the STA for the reason
 * specified. It can be :
 * - RWNX_TXQ_STOP_STA_PS: the STA is no longer in power save mode
 * - RWNX_TXQ_STOP_VIF_PS: the VIF is in power save mode (p2p absence)
 * - RWNX_TXQ_STOP_CHAN: the STA's VIF is now on the current active channel
 *
 * Any TX queue with buffer ready and not Stopped for other reasons, will be
 * added to the HW queue list
 * To be called with tx_lock hold
 */
__IRAM2 void rwnx_txq_sta_start(STA_INF_PTR rwnx_sta, u16 reason)
{
	struct rwnx_txq *txq;
	int tid;

	TRACE_TXQ(STA, "%s: sta %p", __func__, rwnx_sta);

	foreach_sta_txq(rwnx_sta, txq, tid)
		rwnx_txq_start(txq, reason);
}


/**
 * rwnx_stop_sta_txq - Stop all the TX queue linked to a STA
 *
 * @sta: STA whose TX queues must be stopped
 * @reason: Reason why the TX queue are stopped (among RWNX_TX_STOP_xxx)
 * @rwnx_hw: Driver main data
 *
 * This function will stop all the TX queues of the STA for the reason
 * specified. It can be :
 * - RWNX_TXQ_STOP_STA_PS: the STA is in power save mode
 * - RWNX_TXQ_STOP_VIF_PS: the VIF is in power save mode (p2p absence)
 * - RWNX_TXQ_STOP_CHAN: the STA's VIF is not on the current active channel
 *
 * Any TX queue present in a HW queue list will be removed from this list.
 * To be called with tx_lock hold
 */
void rwnx_txq_sta_stop(STA_INF_PTR rwnx_sta, u16 reason)
{
	struct rwnx_txq *txq;
	int tid;

	if (!rwnx_sta)
		return;

	TRACE_TXQ(STA, "%s: sta %p", __func__, rwnx_sta);

	foreach_sta_txq(rwnx_sta, txq, tid)
		rwnx_txq_stop(txq, reason);
}

static inline
void rwnx_txq_vif_for_each_sta(VIF_INF_PTR rwnx_vif,
							   void (*f)(STA_INF_PTR , u16),
							   u16 reason)
{

	switch (mac_vif_mgmt_get_type(rwnx_vif)) {
	case VIF_STA: {
		if (mac_vif_mgmt_get_active(rwnx_vif))
			f(sta_mgmt_get_entry(mac_vif_mgmt_get_u_sta_ap_id(rwnx_vif)), reason);
		break;
	}
	case VIF_AP:
	case VIF_MESH_POINT: {
		STA_INF_PTR sta;

		// Get first STA associated with peer
		sta = mac_vif_mgmt_get_sta_list(rwnx_vif);

		while (sta != NULL) {
			f(sta, reason);

			// Get next STA entry
			sta = sta_mgmt_get_next_sta(sta);
		}
		break;
	}
	default:
		// BUG();
		break;
	}
}

/**
 * rwnx_txq_vif_start - START TX queues of all STA associated to the vif
 *                      and vif's TXQ
 *
 * @vif: Interface to start
 * @reason: Start reason (RWNX_TXQ_STOP_CHAN or RWNX_TXQ_STOP_VIF_PS)
 * @rwnx_hw: Driver main data
 *
 * Iterate over all the STA associated to the vif and re-start them for the
 * reason @reason
 * Take tx_lock
 */
__IRAM2 void rwnx_txq_vif_start(VIF_INF_PTR rwnx_vif, u16 reason)
{
	struct rwnx_txq *txq;

	TRACE_TXQ(VIF, "%s: vif %p, reason 0x%x", __func__, rwnx_vif, reason);

	spin_lock_bh(&g_rwnx_hw.tx_lock);

	//Reject if monitor interface
	if (mac_vif_mgmt_get_type(rwnx_vif) == VIF_MONITOR)
		goto end;

	rwnx_txq_vif_for_each_sta(rwnx_vif, rwnx_txq_sta_start, reason);


	txq = rwnx_txq_vif_get(rwnx_vif, NX_BCMC_TXQ_TYPE);
	rwnx_txq_start(txq, reason);
	txq = rwnx_txq_vif_get(rwnx_vif, NX_UNK_TXQ_TYPE);
	rwnx_txq_start(txq, reason);

end:
	spin_unlock_bh(&g_rwnx_hw.tx_lock);
}


/**
 * rwnx_txq_vif_stop - STOP TX queues of all STA associated to the vif
 *
 * @vif: Interface to stop
 * @arg: Stop reason (RWNX_TXQ_STOP_CHAN or RWNX_TXQ_STOP_VIF_PS)
 * @rwnx_hw: Driver main data
 *
 * Iterate over all the STA associated to the vif and stop them for the
 * reason RWNX_TXQ_STOP_CHAN or RWNX_TXQ_STOP_VIF_PS
 * Take tx_lock
 */
void rwnx_txq_vif_stop(VIF_INF_PTR rwnx_vif, u16 reason)
{
	struct rwnx_txq *txq;

	TRACE_TXQ(VIF, "%s: vif %p, reason 0x%x", __func__, rwnx_vif, reason);

	// trace_txq_vif_stop(vif_info_tag->vif_index);
	spin_lock_bh(&g_rwnx_hw.tx_lock);

	//Reject if monitor interface
	if (mac_vif_mgmt_get_type(rwnx_vif) == VIF_MONITOR)
		goto end;

	rwnx_txq_vif_for_each_sta(rwnx_vif, rwnx_txq_sta_stop, reason);

	txq = rwnx_txq_vif_get(rwnx_vif, NX_BCMC_TXQ_TYPE);
	rwnx_txq_stop(txq, reason);
	txq = rwnx_txq_vif_get(rwnx_vif, NX_UNK_TXQ_TYPE);
	rwnx_txq_stop(txq, reason);

end:
	spin_unlock_bh(&g_rwnx_hw.tx_lock);
}

/**
 * rwnx_start_offchan_txq - START TX queue for offchannel frame
 *
 * @rwnx_hw: Driver main data
 */
void rwnx_txq_offchan_start()
{
	struct rwnx_txq *txq;

	txq = &g_rwnx_hw.txq[NX_OFF_CHAN_TXQ_IDX];
	spin_lock_bh(&g_rwnx_hw.tx_lock);
	rwnx_txq_start(txq, RWNX_TXQ_STOP_CHAN);
	spin_unlock_bh(&g_rwnx_hw.tx_lock);
}

/******************************************************************************
 * TXQ queue/schedule functions
 *****************************************************************************/
/**
 * rwnx_txq_queue_skb - Queue a buffer in a TX queue
 *
 * @skb: Buffer to queue
 * @txq: TX Queue in which the buffer must be added
 * @rwnx_hw: Driver main data
 * @retry: Should it be queued in the retry list
 * @skb_prev: If not NULL insert buffer after this skb instead of the tail
 * of the list (ignored if retry is true)
 *
 * @return: Return 1 if txq has been added to hwq list, 0 otherwise
 *
 * Add a buffer in the buffer list of the TX queue
 * and add this TX queue in the HW queue list if the txq is not stopped.
 * If this is a retry packet it is added after the last retry packet or at the
 * beginning if there is no retry packet queued.
 *
 * If the STA is in PS mode and this is the first packet queued for this txq
 * update TIM.
 *
 * To be called with tx_lock hold
 */
__IRAM2 int rwnx_txq_queue_skb(struct sk_buff *skb, struct rwnx_txq *txq,
					   bool retry,
					   struct sk_buff *skb_prev)
{

	if (txq->sta) {
		struct rwnx_sta_ps *ps = sta_mgmt_get_sta_ps(txq->sta);
		if (ps->active) {
			ps->pkt_ready[txq->ps_id]++;
			// trace_ps_queue(txq->sta);

			if (ps->pkt_ready[txq->ps_id] == 1)
				rwnx_set_traffic_status(txq->sta, true, txq->ps_id);
		}
	}

	if (!retry) {
		/* add buffer in the sk_list */
		if (skb_prev)
			skb_append(skb_prev, skb, &txq->sk_list);
		else
			skb_queue_tail(&txq->sk_list, skb);
		rwnx_txq_start_cleanup_timer(txq->sta);
	} else {
		if (txq->last_retry_skb)
			skb_append(txq->last_retry_skb, skb, &txq->sk_list);
		else
			skb_queue_head(&txq->sk_list, skb);

		txq->last_retry_skb = skb;
		txq->nb_retry++;
	}

	#if CONFIG_RWNX_TD
	rwnx_td_pkt_ind(skb->vif_idx,skb->ac);
	#endif

	/* add it in the hwq list if not stopped and not yet present */
	if (!rwnx_txq_is_stopped(txq)) {
		rwnx_txq_add_to_hw_list(txq);
		return 1;
	}

	return 0;
}

/**
 * skb_queue_extract - Extract buffer from skb list
 *
 * @list: List of skb to extract from
 * @head: List of skb to append to
 * @nb_elt: Number of skb to extract
 *
 * extract the first @nb_elt of @list and append them to @head
 * It is assume that:
 * - @list contains more that @nb_elt
 * - There is no need to take @list nor @head lock to modify them
 */
static inline void skb_queue_extract(struct sk_buff_head *list,
									 struct sk_buff_head *head, int nb_elt)
{
	int i;
	struct sk_buff *first, *last, *ptr;

	first = ptr = list->next;
	for (i = 0; i < nb_elt; i++)
		ptr = ptr->next;
	last = ptr->prev;

	/* unlink nb_elt in list */
	list->qlen -= nb_elt;
	list->next = ptr;
	ptr->prev = (struct sk_buff *)list;

	/* append nb_elt at end of head */
	head->qlen += nb_elt;
	last->next = (struct sk_buff *)head;
	head->prev->next = first;
	first->prev = head->prev;
	head->prev = last;
}


/**
 * rwnx_txq_get_skb_to_push - Get list of buffer to push for one txq
 *
 * @rwnx_hw: main driver data
 * @hwq: HWQ on wich buffers will be pushed
 * @txq: TXQ to get buffers from
 * @user: user postion to use
 * @sk_list_push: list to update
 *
 *
 * This function will returned a list of buffer to push for one txq.
 * It will take into account the number of credit of the HWQ for this user
 * position and TXQ (and push_limit).
 * This allow to get a list that can be pushed without having to test for
 * hwq/txq status after each push
 *
 * If a MU group has been selected for this txq, it will also update the
 * counter for the group
 *
 * @return true if txq no longer have buffer ready after the ones returned.
 *         false otherwise
 */
__IRAM2 static bool rwnx_txq_get_skb_to_push(struct rwnx_hwq *hwq,
							  struct rwnx_txq *txq, int user,
							  struct sk_buff_head *sk_list_push)
{
	int nb_ready = skb_queue_len(&txq->sk_list);
	int credits = txq->credits;
	bool res = false;

	__skb_queue_head_init(sk_list_push);

	if (credits >= nb_ready) {
		skb_queue_splice_init(&txq->sk_list, sk_list_push);
		res = true;
		credits = nb_ready;
	} else {
		skb_queue_extract(&txq->sk_list, sk_list_push, credits);
	}

	return res;
}

/**
 * rwnx_hwq_process - Process one HW queue list
 *
 * @rwnx_hw: Driver main data
 * @hw_queue: HW queue index to process
 *
 * The function will iterate over all the TX queues linked in this HW queue
 * list. For each TX queue, push as many buffers as possible in the HW queue.
 * (NB: TX queue have at least 1 buffer, otherwise it wouldn't be in the list)
 * - If TX queue no longer have buffer, remove it from the list and check next
 *   TX queue
 * - If TX queue no longer have credits or has a push_limit (PS mode) and it
 *   is reached , remove it from the list and check next TX queue
 * - If HW queue is full, update list head to start with the next TX queue on
 *   next call if current TX queue already pushed "too many" pkt in a row, and
 *   return
 *
 * To be called when HW queue list is modified:
 * - when a buffer is pushed on a TX queue
 * - when new credits are received
 * - when a STA returns from Power Save mode or receives traffic request.
 * - when Channel context change
 *
 * To be called with tx_lock hold
 */
__IRAM2 void rwnx_hwq_process(struct rwnx_hwq *hwq)
{
	struct rwnx_txq *txq, *next;
	int user = 0;

	BK_ASSERT(hwq); /* ASSERT VERIFIED */

	hwq->need_processing = false;

	TRACE_TXQ(TXQ, "%s", __func__);

	list_for_each_entry_safe(txq, next, &hwq->list, sched_list) {
		// struct rwnx_txhdr *txhdr = NULL;
		struct sk_buff_head sk_list_push;
		struct sk_buff *skb;
		bool txq_empty;

		/* sanity check for debug */
		BK_ASSERT(txq->status & RWNX_TXQ_IN_HWQ_LIST); /* ASSERT VERIFIED */
		BK_ASSERT(txq->idx != TXQ_INACTIVE); /* ASSERT VERIFIED */
		BK_ASSERT(txq->credits > 0); /* ASSERT VERIFIED */
		BK_ASSERT(rwnx_txq_skb_ready(txq)); /* ASSERT VERIFIED */

		txq_empty = rwnx_txq_get_skb_to_push(hwq, txq, user, &sk_list_push);

		while ((skb = __skb_dequeue(&sk_list_push)) != NULL)
			rwnx_tx_push(skb);

		if (txq_empty)
			rwnx_txq_del_from_hw_list(txq);
	}
}

/**
 * rwnx_hwq_process_all - Process all HW queue list
 *
 * @rwnx_hw: Driver main data
 *
 * Loop over all HWQ, and process them if needed
 * To be called with tx_lock hold
 */
__IRAM2 void rwnx_hwq_process_all(int dummy)
{
	int id;
	ke_state_t state;

	bk_wifi_sw_txq_event_clear();

	TRACE_TXQ(TXQ, "%s", __func__);

	// if (nxmac_current_state_getf() == HW_IDLE)
	//	return;
	state = ke_state_get(TASK_MM);
	if (state == MM_GOING_TO_IDLE || state == MM_IDLE)
		return;

	for (id = ARRAY_SIZE(g_rwnx_hw.hwq) - 1; id >= 0 ; id--) {
		if (g_rwnx_hw.hwq[id].need_processing)
			rwnx_hwq_process(&g_rwnx_hw.hwq[id]);
	}
}

/**
 * rwnx_any_hwq_need_processing - Whether or not any hwq needs processing
 * *
 * Loop over all HWQ, and returns true if any hwq needs processing
 */
__IRAM2 inline bool rwnx_any_hwq_need_processing()
{
	int id;
	for (id = ARRAY_SIZE(g_rwnx_hw.hwq) - 1; id >= 0 ; id--) {
		if (g_rwnx_hw.hwq[id].need_processing)
			return true;
	}

	return false;
}

/**
 * rwnx_hwq_init - Initialize all hwq structures
 *
 * @rwnx_hw: Driver main data
 *
 */
void rwnx_hwq_init()
{
	int i;

	for (i = 0; i < ARRAY_SIZE(g_rwnx_hw.hwq); i++) {
		struct rwnx_hwq *hwq = &g_rwnx_hw.hwq[i];

		INIT_LIST_HEAD(&hwq->list);
	}
}

#endif // CONFIG_RWNX_SW_TXQ

