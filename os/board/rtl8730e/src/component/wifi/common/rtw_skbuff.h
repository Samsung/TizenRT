/**
  ******************************************************************************
  * @file    rtw_skbuff.h
  * @author
  * @version
  * @date
  * @brief
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef __SKBUFF_H__
#define __SKBUFF_H__

#include "rtw_wifi_constants.h"
#include "os_wrapper.h"
#include "rtw_atomic.h"
#include "rtw_misc.h"
#include "dlist.h"

/*skb size = HW info + wlan over head + ethernet len + safety*/
#define MAX_SKB_BUF_SIZE	(((WLAN_HW_INFO_LEN+WLAN_MAX_PROTOCOL_OVERHEAD+WLAN_MAX_ETHFRM_LEN+8)\
							+ SKB_CACHE_SZ) & ~(SKB_CACHE_SZ-1))

/*TX reserve size before 802.3 pkt*/
#define WLAN_ETHHDR_LEN	14
#define SKB_WLAN_TX_EXTRA_LEN	(TXDESC_SIZE+WLAN_HDR_A4_QOS_HTC_LEN+WLAN_MAX_IV_LEN+WLAN_SNAP_HEADER-WLAN_ETHHDR_LEN)

#define SKB_DATA_ALIGN(X)	(((X) + (4 - 1)) & ~(4 - 1))

struct  sk_buff_head {
	struct list_head	*next, *prev;
	unsigned int 		qlen;
};

struct skb_raw_para {
	unsigned char rate;             /* tx rate of tx_raw packets */
	unsigned char retry_limit;      /* the number of tx retry when tx fail for tx_raw packet */
	unsigned char device_id;        /* index of peer device which as a rx role for receiving this pkt, and will be update when linked peer */
	unsigned char ac_queue;         /* 0/3 for BE, 1/2 for BK, 4/5 for VI, 6/7 for VO */
	unsigned char enable : 1;       /* indicate whether this packet is a tx_raw packet. set to 1 when tx_raw */
	unsigned char sgi : 1;          /* 1 for enable data short */
	unsigned char agg_en : 1;       /* aggregation of tx_raw frames. 1:enable; 0-disable */
	unsigned char rom_rsvd[8];
};

struct sk_buff {
	struct list_head list;
	unsigned char		*head;		/* Head of buffer */
	unsigned char		*data;		/* Data head pointer */
	unsigned char		*tail;		/* Tail pointer	*/
	unsigned char		*end;		/* End pointer */
	void	*dev;		/* Device we arrived on/are leaving by */
	unsigned int 		len;		/* Length of actual data */

	int 			dyalloc_flag;
	unsigned char	busy;
	unsigned char	no_free;

	struct skb_raw_para	tx_raw;

	unsigned char buf[MAX_SKB_BUF_SIZE] SKB_ALIGNMENT;/* buf start address and size alignmengt for pre allocate skb*/
	atomic_t ref;
}; /*total size should be align to max(AP_cache_size, NP_cache_size), single core no need*/

struct skb_priv_t {
	/*skb_buff for managing and store skb data*/
	struct sk_buff *skb_buff_pool;
	struct list_head skb_buff_list;
	int skb_buff_num;
	int skb_buff_used;
	int skb_buff_max_used;
	int skb_buf_max_size;
	int skb_fail_tx;
	int skb_fail_rx;
	int rom_rsvd;
};

extern struct skb_priv_t skbpriv;
struct net_device;

__inline static unsigned char *skb_pull(struct sk_buff *skb, unsigned int len)
{
	if (len > skb->len) {
		return NULL;
	}

	skb->len -= len;
	skb->data = (unsigned char *)(((unsigned int)skb->data) + len);

	return skb->data;
}

/**
 *	skb_put - add data to a buffer
 *	@skb: buffer to use
 *	@len: amount of data to add
 *
 *	This function extends the used data area of the buffer. If this would
 *	exceed the total buffer size the kernel will panic. A pointer to the
 *	first byte of the extra data is returned.
 */

__inline static unsigned char *skb_put(struct sk_buff *skb, unsigned int len)
{
	unsigned char *tmp = skb->tail;
	skb->tail += len;
	skb->len += len;
	if (skb->tail > skb->end) {
		assert_param(0);
	}

	return tmp;
}


void skb_reserve(struct sk_buff *skb, unsigned int len);
void skb_assign_buf(struct sk_buff *skb, unsigned char *buf, unsigned int len);
unsigned char *skb_tail_pointer(const struct sk_buff *skb);
void skb_set_tail_pointer(struct sk_buff *skb, const int offset);
unsigned char *skb_end_pointer(const struct sk_buff *skb);
struct sk_buff *dev_alloc_skb(unsigned int length, unsigned int reserve_len);
void kfree_skb(struct sk_buff *skb);
struct sk_buff *skb_clone(struct sk_buff *skb, int gfp_mask);
struct sk_buff *skb_copy(const struct sk_buff *skb, int gfp_mask, unsigned int reserve_len);
void dev_kfree_skb_any(struct sk_buff *skb);
void init_skb_pool(uint32_t skb_num_np, uint32_t skb_buf_size, unsigned char skb_cache_zise);
void deinit_skb_pool(void);
struct sk_buff *get_buf_from_poll(void);
void release_buf_to_poll(struct sk_buff *skb);
#endif //__SKBUFF_H__
