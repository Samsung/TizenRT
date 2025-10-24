/**
 ****************************************************************************************
 *
 * @file rwnx_txq.h
 *
 * Copyright (C) RivieraWaves 2012-2020
 *
 ****************************************************************************************
 */
#ifndef _RWNX_TXQ_H_
#define _RWNX_TXQ_H_

#include <common/sys_config.h>
#include "skbuff.h"
#include "bk_list.h"

/**
 * Fullmac TXQ configuration:
 *  - STA: 1 TXQ per TID (limited to 8)
 *         1 TXQ for bufferable MGT frames
 *  - VIF: 1 TXQ for Multi/Broadcast +
 *         1 TXQ for MGT for unknown STAs or non-bufferable MGT frames
 *  - 1 TXQ for offchannel transmissions
 *
 *
 * Txq mapping looks like
 * for NX_REMOTE_STA_MAX=10 and NX_VIRT_DEV_MAX=4
 *
 * | TXQ | NDEV_ID | VIF |   STA |  TID | HWQ |
 * |-----+---------+-----+-------+------+-----|-
 * |   0 |       0 |     |     0 |    0 |   1 | 9 TXQ per STA
 * |   1 |       1 |     |     0 |    1 |   0 | (8 data + 1 mgmt)
 * |   2 |       2 |     |     0 |    2 |   0 |
 * |   3 |       3 |     |     0 |    3 |   1 |
 * |   4 |       4 |     |     0 |    4 |   2 |
 * |   5 |       5 |     |     0 |    5 |   2 |
 * |   6 |       6 |     |     0 |    6 |   3 |
 * |   7 |       7 |     |     0 |    7 |   3 |
 * |   8 |     N/A |     |     0 | MGMT |   3 |
 * |-----+---------+-----+-------+------+-----|-
 * | ... |         |     |       |      |     | Same for all STAs
 * |-----+---------+-----+-------+------+-----|-
 * |  90 |      80 |   0 | BC/MC |    0 | 1/4 | 1 TXQ for BC/MC per VIF
 * | ... |         |     |       |      |     |
 * |  93 |      80 |   3 | BC/MC |    0 | 1/4 |
 * |-----+---------+-----+-------+------+-----|-
 * |  94 |     N/A |   0 |   N/A | MGMT |   3 | 1 TXQ for unknown STA per VIF
 * | ... |         |     |       |      |     |
 * |  97 |     N/A |   3 |   N/A | MGMT |   3 |
 * |-----+---------+-----+-------+------+-----|-
 * |  98 |     N/A |     |   N/A | MGMT |   3 | 1 TXQ for offchannel frame
 *
 * For Non-WFA Certification Version
 * for NX_REMOTE_STA_MAX=2 and NX_VIRT_DEV_MAX=2
 *
 * | TXQ | NDEV_ID | VIF |   STA |  TID | HWQ |
 * |-----+---------+-----+-------+------+-----|-
 * |  0  |       0 |     |     0 |    0 |   2 | 2 TXQ per STA
 * |  1  |     N/A |     |     0 | MGMT |   3 | (1 data + 1 mgmt)
 * |-----+---------+-----+-------+------+-----|-
 * | ... |         |     |       |      |     | Same for all STAs
 * |-----+---------+-----+-------+------+-----|-
 * |  4  |      80 |   0 | BC/MC |    0 | 1/4 | 1 TXQ for BC/MC per VIF
 * |  5  |      80 |   1 | BC/MC |    0 | 1/4 |
 * |-----+---------+-----+-------+------+-----|-
 * |  6  |     N/A |   0 |   N/A | MGMT |   3 | 1 TXQ for unknown STA per VIF
 * |  7  |     N/A |   1 |   N/A | MGMT |   3 |
 * |-----+---------+-----+-------+------+-----|-
 * |  8  |     N/A |     |   N/A | MGMT |   3 | 1 TXQ for offchannel frame
 */
#if CONFIG_WFA_CERT
#define NX_NB_TID_PER_STA 8
#else
/* Only one hw queue for all TIDs */
#define NX_NB_TID_PER_STA 1
#endif
#define NX_NB_TXQ_PER_STA (NX_NB_TID_PER_STA + 1)
#define NX_STA_MGMT_TXQ_IDX NX_NB_TID_PER_STA
#define NX_NB_TXQ_PER_VIF 2
#define NX_NB_TXQ ((NX_NB_TXQ_PER_STA * NX_REMOTE_STA_MAX) +    \
                   (NX_NB_TXQ_PER_VIF * NX_VIRT_DEV_MAX) + 1)

#define NX_FIRST_VIF_TXQ_IDX (NX_REMOTE_STA_MAX * NX_NB_TXQ_PER_STA)
#define NX_FIRST_BCMC_TXQ_IDX  NX_FIRST_VIF_TXQ_IDX
#define NX_FIRST_UNK_TXQ_IDX  (NX_FIRST_BCMC_TXQ_IDX + NX_VIRT_DEV_MAX)

#define NX_OFF_CHAN_TXQ_IDX (NX_FIRST_VIF_TXQ_IDX +                     \
                             (NX_VIRT_DEV_MAX * NX_NB_TXQ_PER_VIF))
#define NX_BCMC_TXQ_TYPE 0
#define NX_UNK_TXQ_TYPE  1

/**
 * Each data TXQ is a netdev queue. TXQ to send MGT are not data TXQ as
 * they did not recieved buffer from netdev interface.
 * Need to allocate the maximum case.
 * AP : all STAs + 1 BC/MC
 */
#define NX_NB_NDEV_TXQ ((NX_NB_TID_PER_STA * NX_REMOTE_STA_MAX) + 1 )
#define NX_BCMC_TXQ_NDEV_IDX (NX_NB_TID_PER_STA * NX_REMOTE_STA_MAX)
#define NX_STA_NDEV_IDX(tid, sta_idx) ((tid) + (sta_idx) * NX_NB_TID_PER_STA)
#define NDEV_NO_TXQ 0xffff


#define TXQ_INACTIVE 0xffff

/* stop netdev queue when number of queued buffers if greater than this  */
#define RWNX_NDEV_FLOW_CTRL_STOP    200
/* restart netdev queue when number of queued buffers is lower than this */
#define RWNX_NDEV_FLOW_CTRL_RESTART 100

#define NX_TXQ_INITIAL_CREDITS 4

#define RWNX_TXQ_CLEANUP_INTERVAL (5000) //5s in jiffies
//#define RWNX_TXQ_MAX_QUEUE_JIFFIES (20 * FCLK_SECOND)
#define RWNX_TXQ_MAX_QUEUE_JIFFIES (20 * bk_get_ticks_per_second())


/**
 * TXQ tid sorted by decreasing priority
 */
extern const int nx_tid_prio[NX_NB_TID_PER_STA];

/**
 * struct rwnx_hwq - Structure used to save information relative to
 *                   an AC TX queue (aka HW queue)
 * @list: List of TXQ, that have buffers ready for this HWQ
 * @credits: available credit for the queue (i.e. nb of buffers that
 *           can be pushed to FW )
 * @id Id of the HWQ among RWNX_HWQ_....
 * @size size of the queue
 * @need_processing Indicate if hwq should be processed
 * @len number of packet ready to be pushed to fw for this HW queue
 * @len_stop threshold to stop mac80211(i.e. netdev) queues. Stop queue when
 *           driver has more than @len_stop packets ready.
 * @len_start threshold to wake mac8011 queues. Wake queue when driver has
 *            less than @len_start packets ready.
 */
struct rwnx_hwq {
    struct list_head list;
    bool need_processing;
};

/**
 * enum rwnx_txq_flags - TXQ status flag
 *
 * @RWNX_TXQ_IN_HWQ_LIST: The queue is scheduled for transmission
 * @RWNX_TXQ_STOP_FULL: No more credits for the queue
 * @RWNX_TXQ_STOP_CSA: CSA is in progress
 * @RWNX_TXQ_STOP_STA_PS: Destiniation sta is currently in power save mode
 * @RWNX_TXQ_STOP_VIF_PS: Vif owning this queue is currently in power save mode
 * @RWNX_TXQ_STOP_CHAN: Channel of this queue is not the current active channel
 * @RWNX_TXQ_STOP_MU_POS: TXQ is stopped waiting for all the buffers pushed to
 *                       fw to be confirmed
 * @RWNX_TXQ_STOP: All possible reason to have a txq stopped
 * @RWNX_TXQ_NDEV_FLOW_CTRL: associated netdev queue is currently stopped.
 *                          Note: when a TXQ is flowctrl it is NOT stopped
 */
enum rwnx_txq_flags {
    RWNX_TXQ_IN_HWQ_LIST  = BIT(0),
    RWNX_TXQ_STOP_FULL    = BIT(1),
    RWNX_TXQ_STOP_CSA     = BIT(2),
    RWNX_TXQ_STOP_STA_PS  = BIT(3),
    RWNX_TXQ_STOP_VIF_PS  = BIT(4),
    RWNX_TXQ_STOP_CHAN    = BIT(5),
    RWNX_TXQ_STOP_MU_POS  = BIT(6),
    RWNX_TXQ_STOP_RESET   = BIT(7),
    RWNX_TXQ_STOP_TWT_PS  = BIT(8),
    RWNX_TXQ_STOP         = (RWNX_TXQ_STOP_FULL | RWNX_TXQ_STOP_CSA |
                             RWNX_TXQ_STOP_STA_PS | RWNX_TXQ_STOP_VIF_PS |
                             RWNX_TXQ_STOP_CHAN | RWNX_TXQ_STOP_RESET | RWNX_TXQ_STOP_TWT_PS),
    RWNX_TXQ_NDEV_FLOW_CTRL = BIT(9),
};


/**
 * struct rwnx_txq - Structure used to save information relative to
 *                   a RA/TID TX queue
 *
 * @idx: Unique txq idx. Set to TXQ_INACTIVE if txq is not used.
 * @status: bitfield of @rwnx_txq_flags.
 * @credits: available credit for the queue (i.e. nb of buffers that
 *           can be pushed to FW).
 * @sched_list: list node for HW queue schedule list (rwnx_hwq.list)
 * @sk_list: list of buffers to push to fw
 * @last_retry_skb: pointer on the last skb in @sk_list that is a retry.
 *                  (retry skb are stored at the beginning of the list)
 *                  NULL if no retry skb is queued in @sk_list
 * @nb_retry: Number of retry packet queued.
 * @hwq: Pointer on the associated HW queue.
 * @tid: TID
 * @ps_id: Index to use for Power save mode (LEGACY or UAPSD)
 * @ndev_idx: txq idx from netdev point of view (0xFF for non netdev queue)
 */
struct rwnx_txq {
    u16 idx;
    u16 status;
    s8 credits;
    struct list_head sched_list;
    struct sk_buff_head sk_list;
    struct sk_buff *last_retry_skb;
    struct rwnx_hwq *hwq;
    STA_INF_PTR sta;
    int nb_retry;
    u8 tid;
    u8 ps_id;
    u16 ndev_idx;
};

static inline bool rwnx_txq_is_stopped(struct rwnx_txq *txq)
{
    return (txq->status & RWNX_TXQ_STOP);
}

static inline bool rwnx_txq_is_full(struct rwnx_txq *txq)
{
    return (txq->status & RWNX_TXQ_STOP_FULL);
}

static inline bool rwnx_txq_is_scheduled(struct rwnx_txq *txq)
{
    return (txq->status & RWNX_TXQ_IN_HWQ_LIST);
}

/**
 * foreach_sta_txq - Macro to iterate over all TXQ of a STA in increasing
 *					 TID order
 *
 * @sta: pointer to rwnx_sta
 * @txq: pointer to rwnx_txq updated with the next TXQ at each iteration
 * @tid: int updated with the TXQ tid at each iteration
 * @rwnx_hw: main driver data
 */
#define foreach_sta_txq(sta, txq, tid)                          \
    for (tid = 0, txq = rwnx_txq_sta_get(sta, 0);               \
         tid < (is_multicast_sta(mac_sta_mgmt_get_staid(sta)) ? 1 : NX_NB_TXQ_PER_STA); \
         tid++, txq++)


/**
 * foreach_sta_txq_prio - Macro to iterate over all TXQ of a STA in
 *                        decreasing priority order
 *
 * @sta: pointer to rwnx_sta
 * @txq: pointer to rwnx_txq updated with the next TXQ at each iteration
 * @tid: int updated with the TXQ tid at each iteration
 * @i: int updated with ieration count
 * @rwnx_hw: main driver data
 *
 * Note: For fullmac txq for mgmt frame is skipped
 */
#define foreach_sta_txq_prio(sta, txq, tid, i)                          \
    for (i = 0, tid = nx_tid_prio[0], txq = rwnx_txq_sta_get(sta, tid); \
         i < NX_NB_TID_PER_STA;                                                  \
         i++, tid = nx_tid_prio[i], txq = rwnx_txq_sta_get(sta, tid))

/**
 * foreach_vif_txq - Macro to iterate over all TXQ of a VIF (in AC order)
 *
 * @vif: pointer to rwnx_vif
 * @txq: pointer to rwnx_txq updated with the next TXQ at each iteration
 * @ac:  int updated with the TXQ ac at each iteration
 */
#define foreach_vif_txq(vif, txq, ac)                                   \
    for (ac = RWNX_HWQ_BK, txq = &vif->txqs[0];                         \
         ac < NX_NB_TXQ_PER_VIF;                                        \
         ac++, txq++)

struct rwnx_txq *rwnx_txq_sta_get(STA_INF_PTR sta, u8 tid);
struct rwnx_txq *rwnx_txq_vif_get(VIF_INF_PTR vif, u8 type);

/**
 * rwnx_txq_vif_get_status - return status bits related to the vif
 *
 * @rwnx_vif: Pointer to vif structure
 */
static inline u8 rwnx_txq_vif_get_status(VIF_INF_PTR rwnx_vif)
{
    struct rwnx_txq *txq = rwnx_txq_vif_get(rwnx_vif, 0);
    return (txq->status & (RWNX_TXQ_STOP_CHAN | RWNX_TXQ_STOP_VIF_PS));
}

void rwnx_txq_vif_init(VIF_INF_PTR vif, u16 status);
void rwnx_txq_vif_deinit(VIF_INF_PTR vif);
void rwnx_txq_sta_init(STA_INF_PTR rwnx_sta, u16 status);
void rwnx_txq_sta_deinit(STA_INF_PTR rwnx_sta);
void rwnx_txq_unk_vif_init(VIF_INF_PTR rwnx_vif);
void rwnx_txq_unk_vif_deinit(VIF_INF_PTR vif);
void rwnx_txq_offchan_init(VIF_INF_PTR rwnx_vif);
void rwnx_txq_offchan_deinit(VIF_INF_PTR rwnx_vif);
void rwnx_txq_tdls_vif_init(VIF_INF_PTR rwnx_vif);
void rwnx_txq_tdls_vif_deinit(VIF_INF_PTR vif);
void rwnx_txq_tdls_sta_start(VIF_INF_PTR rwnx_vif, u16 reaso);
void rwnx_txq_tdls_sta_stop(VIF_INF_PTR rwnx_vif, u16 reason);
void rwnx_txq_prepare(void);
void rwnx_txq_add_to_hw_list(struct rwnx_txq *txq);
void rwnx_txq_del_from_hw_list(struct rwnx_txq *txq);
void rwnx_txq_stop(struct rwnx_txq *txq, u16 reason);
void rwnx_txq_start(struct rwnx_txq *txq, u16 reason);
void rwnx_txq_vif_start(VIF_INF_PTR vif, u16 reason);
void rwnx_txq_vif_stop(VIF_INF_PTR vif, u16 reason);
void rwnx_txq_sta_start(STA_INF_PTR sta, u16 reason);
void rwnx_txq_sta_stop(STA_INF_PTR sta, u16 reason);
void rwnx_txq_offchan_start(void);
int rwnx_txq_queue_skb(struct sk_buff *skb, struct rwnx_txq *txq, bool retry, struct sk_buff *skb_prev);
void rwnx_txq_drop_skb(struct rwnx_txq *txq,  struct sk_buff *skb, bool retry_packet);
void rwnx_txq_cleanup_timer_cb(void *dummy);
void rwnx_txq_credit_update(int sta_idx, u8 tid, s8 update);
void rwnx_hwq_init(void);
void rwnx_hwq_process(struct rwnx_hwq *hwq);
void rwnx_hwq_process_all(int);
bool rwnx_any_hwq_need_processing(void);

#endif // _RWNX_TXQ_H_
