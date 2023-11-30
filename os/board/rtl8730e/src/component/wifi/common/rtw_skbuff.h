/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/
#ifndef __SKBUFF_H__
#define __SKBUFF_H__

#include "rtw_wifi_constants.h"
#include "osdep_service.h"

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
	unsigned char enable;
	unsigned char rate;
	unsigned char retry_limit;
};

struct sk_buff {
	/* These two members must be first. */
	struct sk_buff		*next;		/* Next buffer in list */
	struct sk_buff		*prev;		/* Previous buffer in list */

	struct sk_buff_head	*list;		/* List we are on */
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
};

struct skb_info {
	struct list_head list;
	struct sk_buff skb;
} SKB_ALIGNMENT; /*total size should be align to max(AP_cache_size, NP_cache_size), single core no need*/

struct skb_data {
	struct list_head list;
	unsigned char buf[MAX_SKB_BUF_SIZE] SKB_ALIGNMENT;/* buf start address and size alignmengt for pre allocate skb*/
	atomic_t ref;
};

struct skb_priv_t {
	/*skb_data for store data*/
	struct skb_data *skb_data_pool;
	struct list_head skb_data_list;
	int skb_data_num;
	int skb_data_used;
	int skb_data_max_used;
	/*skb_info for managing skb_data*/
	struct skb_info *skb_info_pool;
	struct list_head skb_info_list;
	int skb_info_num;
	int skb_info_used;
	int skb_info_max_used;
	int skb_fail_tx;
	int skb_fail_rx;
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
		ASSERT(0);
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
void init_skb_pool(void);
void deinit_skb_pool(void);

#endif //__SKBUFF_H__
