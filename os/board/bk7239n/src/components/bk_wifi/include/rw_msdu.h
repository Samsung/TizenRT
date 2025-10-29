/**
 ****************************************************************************************
 *
 * @file rw_msdu.h
 *
 * @brief MSDU/MPDU TX module
 *
 *
 * Copyright (C) BEKEN corperation 2021-2022
 *
 ****************************************************************************************
 */

#ifndef _RW_MSDU_H_
#define _RW_MSDU_H_

#include <common/sys_config.h>
//#include "tx_swdesc.h"
#if (CONFIG_FULLY_HOSTED || CONFIG_SEMI_HOSTED)
#include "pbuf.h"
#endif
////#include <os/os.h>
//#include "rwnx.h"
#include "bk_list.h"
#include "bk_private/bk_wifi.h"
#include "bk_wifi_adapter.h"
#include "fhost_tx.h"

#define MSDU_TX_MAX_CNT               (64)
#define MSDU_RX_MAX_CNT               (32)

#define MSDU_TX_MIN_FREE_MEM_SIZE     (5*1024)

#define ETH_ADDR_LEN	               6		/* Octets in one ethernet addr	 */

struct sk_buff;

static inline int is_broadcast_eth_addr(const u8 *a)
{
	return (a[0] & a[1] & a[2] & a[3] & a[4] & a[5]) == 0xff;
}

typedef struct _eth_hdr {
	UINT8 e_dest[ETH_ADDR_LEN];
	UINT8 e_src[ETH_ADDR_LEN];
	UINT16 e_proto;
} __attribute__((packed)) ETH_HDR_T, *ETH_HDR_PTR;

struct ieee80211_tx_cb {
	void *sema;
	int result;
	int l2_tag;
};

/**
 * MSDU descriptor
 *
 * @hdr: chain msdu node to an list
 * @msdu_ptr: buffer of msdu
 * @len: length of msdu
 * @vif_index: interface index
 * @sta_index: STA index
 * @args:
 * @sync:
 */
struct sk_buff {
#if CONFIG_RWNX_SW_TXQ
	/* These two members must be first. */
	struct sk_buff		*next;
	struct sk_buff		*prev;
#else
	LIST_HEADER_T hdr;
#endif

	/// Pointer to 802.3 frame
	uint8_t *msdu_ptr;
	/// 802.3 frame len
	uint32_t len;
	/// fullhost txdesc
	struct fhost_tx_desc_tag *ftxdesc;
	/// lwip buffer
	struct pbuf *p;
	/// callback of TX, prototype void (*)(void *, bool)
	/// cb has two arguments, first is args, second is tx success flag
	void *cb;
	/// callback arguments
	void *args;
	/// vif index in umac/lmac
	uint8_t vif_idx;
	/// sta index in umac/lmac
	uint8_t sta_idx;

	/// For mgmt no cck frame
	uint16_t nocck: 1;
	/// off channel mgmt tx
	uint16_t offchannel: 1;

#if CONFIG_RWNX_SW_TXQ
	/// jiffies that queue this frame to umac
	uint64_t jiffies;
	/// txq associated with this skb
	void *txq;
	/// priority, for bc/mc in AP mode, set to 0xFF
	uint8_t priority;
	/// HW Queue
	uint8_t ac;
	/// Frame type
	uint8_t sframe_type;
#if CONFIG_MAC_SFRAME_SOFTWARE_RETRY
	/// Software retry count
	uint8_t sframe_sw_retry_cnt;
#endif
#endif
};

typedef struct sk_buff MSDU_NODE_T, *MSDU_NODE_PTR;

/**
 * struct wlan_mgmt_tx_params - mgmt tx parameters
 *
 * This structure provides information needed to transmit a mgmt frame
 *
 * @chan: channel to use
 * @offchan: indicates wether off channel operation is required
 * @wait: duration for ROC
 * @buf: buffer to transmit
 * @len: buffer length
 * @no_cck: don't use cck rates for this frame
 * @dont_wait_for_ack: tells the low level not to wait for an ack
 * @n_csa_offsets: length of csa_offsets array
 * @csa_offsets: array of all the csa offsets in the frame
 */
struct wlan_mgmt_tx_params {
	//	struct ieee80211_channel *chan;
	//	bool offchan;
	//	unsigned int wait;
	const u8 *buf;
	size_t len;
	bool no_cck;
	bool dont_wait_for_ack;
	//	int n_csa_offsets;
	//	const u16 *csa_offsets;
};

struct txdesc;
extern void rwm_push_rx_list(MSDU_NODE_T *node);
extern MSDU_NODE_T *rwm_pop_rx_list(void);
extern void rwm_tx_msdu_renew(UINT8 *buf, UINT32 len, UINT8 *orig_addr);
extern void rwm_tx_mpdu_renew(UINT8 *buf, UINT32 len, UINT8 *orig_addr);
extern UINT8 *rwm_get_msdu_content_ptr(MSDU_NODE_T *node);
extern UINT8 *rwm_get_mpdu_content_ptr(MSDU_NODE_T *node);
extern void rwm_txdesc_copy(struct txdesc *dst_local, ETH_HDR_PTR eth_hdr_ptr);
extern int rwm_raw_frame_with_cb(uint8_t *buffer, int len, void *cb, void *param);

extern void rwm_node_free(MSDU_NODE_T *node);
extern UINT8 *rwm_rx_buf_alloc(UINT32 len);

MSDU_NODE_T *rwm_tx_node_alloc(UINT32 len, bk_pbuf_layer layer);
MSDU_NODE_T *rwm_tx_mgmt_node_alloc(UINT32 len, bk_pbuf_layer layer);
extern UINT32 rwm_upload_data(void *buff_addr, uint32_t frame_len);
extern UINT32 rwm_rx_monitor(void *buff_addr, uint32_t frame_len);
UINT32 rwm_get_rx_free_node(uint32_t *host_id, int len);
void rwm_transfer_mgmt_node(MSDU_NODE_T *node);
void rwm_transfer_raw_ex_node(MSDU_NODE_T *node, raw_tx_cntrl_t *raw_tx_cntrl);
UINT32 rwm_transfer_node(MSDU_NODE_T *node, u8 flag
#if CONFIG_SPECIAL_TX_TYPE
    ,bool is_special_tx
#endif
);
extern UINT32 rwm_get_rx_valid(void);
extern UINT8 rwm_get_tid(void);
extern void rwm_set_tid(UINT8 tid);
extern int rwm_set_tx_raw_rate_power(UINT32 rate, UINT32 power);

#endif // _RW_MSDU_H_
// eof

