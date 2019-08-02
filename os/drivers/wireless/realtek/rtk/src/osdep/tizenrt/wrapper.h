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
 ******************************************************************************
 *  Wrapper provide a linux-like interface
 ************************************************************************/
#ifndef __WRAPPER_H__
#define __WRAPPER_H__

//----- ------------------------------------------------------------------
// Include Files
//----- ------------------------------------------------------------------
#include "osdep_service.h"
#include "wireless.h"
#include <skbuff.h>
#include <stdio.h>
#include <string.h>

#ifndef __LIST_H
#warning "DLIST_NOT_DEFINE!!!!!!"
//----- ------------------------------------------------------------------
// Linled List
//----- ------------------------------------------------------------------
/*
 * Simple doubly linked list implementation.
 *
 * Some of the internal functions ("__xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */

#define LIST_HEAD_INIT(name) \
	do {                     \
		&(name), &(name)     \
	} while (0)

#define INIT_LIST_HEAD(ptr)  \
	do {                     \
		(ptr)->next = (ptr); \
		(ptr)->prev = (ptr); \
	} while (0)

/*
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static __inline void __list_add(struct list_head *new, struct list_head *prev,
								struct list_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static __inline void __list_del(struct list_head *prev,
								struct list_head *next)
{
	next->prev = prev;
	prev->next = next;
}

/**
 * list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: list_empty on entry does not return true after this, the entry is in an
 * undefined state.
 */
static __inline void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
}

/**
 * list_del_init - deletes entry from list and reinitialize it.
 * @entry: the element to delete from the list.
 */
static __inline void list_del_init(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	INIT_LIST_HEAD(entry);
}

/**
 * list_empty - tests whether a list is empty
 * @head: the list to test.
 */
static __inline int list_empty(struct list_head *head)
{
	return head->next == head;
}

/**
 * list_splice - join two lists
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 */
static __inline void list_splice(struct list_head *list,
								 struct list_head *head)
{
	struct list_head *first = list->next;

	if (first != list) {
		struct list_head *last = list->prev;
		struct list_head *at = head->next;

		first->prev = head;
		head->next = first;

		last->next = at;
		at->prev = last;
	}
}

void list_add(struct list_head *new, struct list_head *head);
void list_add_tail(struct list_head *new, struct list_head *head);
#endif

extern void save_and_cli(void);
extern void restore_flags(void);
//----- ------------------------------------------------------------------
// SKB Operation
//----- ------------------------------------------------------------------

#define SMP_CACHE_BYTES 4
#define SKB_DATA_ALIGN(X) \
	(((X) + (SMP_CACHE_BYTES - 1)) & ~(SMP_CACHE_BYTES - 1))

#define SKB_RESERVED_FOR_SAFETY 8 // 0
#define SKB_WLAN_TX_EXTRA_LEN                                                 \
	(TXDESC_SIZE + WLAN_HDR_A4_QOS_LEN + WLAN_MAX_IV_LEN + WLAN_SNAP_HEADER - \
	 WLAN_ETHHDR_LEN)
#define RX_DRIVER_INFO 32

#if (defined CONFIG_GSPI_HCI || defined CONFIG_SDIO_HCI)
#define HAL_INTERFACE_OVERHEAD_SKB_DATA \
	12 // HAL_INTERFACE_CMD (4) + HAL_INTERFACE_STATUS (8)
#elif defined(CONFIG_LX_HCI) || defined(CONFIG_AXI_HCI)
#define HAL_INTERFACE_OVERHEAD_SKB_DATA 0
#endif

#if defined CONFIG_GSPI_HCI || defined CONFIG_SDIO_HCI || \
	defined(CONFIG_LX_HCI) || defined(CONFIG_AXI_HCI)
#if defined(CONFIG_MP_INCLUDED)
#ifdef CONFIG_DONT_CARE_TP
#define MAX_RX_PKT_LIMIT                                           \
	((WLAN_MAX_PROTOCOL_OVERHEAD + WLAN_MAX_RX_ETHFRM_LEN + 511) / \
	 512) // 4, for lxbus
#else
#define MAX_RX_PKT_LIMIT                                        \
	((WLAN_MAX_PROTOCOL_OVERHEAD + WLAN_MAX_ETHFRM_LEN + 511) / \
	 512) // 4, for lxbus
#endif
#define MAX_RX_PKT_SIZE \
	MAX_RX_PKT_LIMIT * 512 // MAX_SKB_BUF_SIZE = 0+32+40+512*4+0 = 2120
#else
#ifdef CONFIG_DONT_CARE_TP
#define MAX_RX_PKT_SIZE WLAN_MAX_PROTOCOL_OVERHEAD + WLAN_MAX_RX_ETHFRM_LEN
#else
#define MAX_RX_PKT_SIZE          \
	WLAN_MAX_PROTOCOL_OVERHEAD + \
		WLAN_MAX_ETHFRM_LEN // MAX_RX_PKT_SIZE = 64+1514 = 1578
#endif
#define MAX_RX_PKT_LIMIT \
	((MAX_RX_PKT_SIZE + 511) / 512) // ((1578 + 512)  / 512) = 4
#endif

#ifdef CONFIG_DONT_CARE_TP
#define MAX_TX_SKB_BUF_SIZE                                      \
	(HAL_INTERFACE_OVERHEAD_SKB_DATA + RX_DRIVER_INFO +          \
	 ((TXDESC_SIZE > RXDESC_SIZE) ? TXDESC_SIZE : RXDESC_SIZE) + \
	 WLAN_MAX_PROTOCOL_OVERHEAD + WLAN_MAX_TX_ETHFRM_LEN +       \
	 SKB_RESERVED_FOR_SAFETY)
#define MAX_RX_SKB_BUF_SIZE                                      \
	(HAL_INTERFACE_OVERHEAD_SKB_DATA + RX_DRIVER_INFO +          \
	 ((TXDESC_SIZE > RXDESC_SIZE) ? TXDESC_SIZE : RXDESC_SIZE) + \
	 MAX_RX_PKT_SIZE + SKB_RESERVED_FOR_SAFETY)
#else
#ifdef CONFIG_HIGH_TP_TEST
#define MAX_SKB_BUF_SIZE 2104
#else
#define MAX_SKB_BUF_SIZE                                         \
	(HAL_INTERFACE_OVERHEAD_SKB_DATA + RX_DRIVER_INFO +          \
	 ((TXDESC_SIZE > RXDESC_SIZE) ? TXDESC_SIZE : RXDESC_SIZE) + \
	 MAX_RX_PKT_SIZE + SKB_RESERVED_FOR_SAFETY) // 0+32+40+1578+0 = 1650
#endif
#endif
#else
#define MAX_SKB_BUF_SIZE 2048
#endif

/*
 *	External functions
 */
struct net_device;
extern void kfree_skb_chk_key(struct sk_buff *skb, struct net_device *root_dev);
#ifdef CONFIG_TRACE_SKB
extern void show_skb(void);
extern int _set_skb_list_flag(struct sk_buff *skb, unsigned int queueflag);
extern void dump_skb_list(void);
#define set_skb_list_flag(skb, queueflag)  \
	(_set_skb_list_flag((skb), queueflag), \
	 (skb) ? (skb)->funcname[(skb)->list_idx] = __FUNCTION__ : NULL)
extern int _clear_skb_list_flag(struct sk_buff *skb, unsigned int queueflag);
#define clear_skb_list_flag(skb, queueflag)  \
	(_clear_skb_list_flag((skb), queueflag), \
	 (skb) ? (skb)->funcname[(skb)->list_idx] = __FUNCTION__ : NULL)
#define dev_kfree_skb_any(trx, holder, skb)                     \
	do {                                                        \
		clear_skb_list_flag(skb, SKBLIST_##trx##holder##_MASK); \
		set_skb_list_flag(skb, SKBLIST_POOL);                   \
		kfree_skb_chk_key(skb, skb->dev);                       \
	} while (0)
#else
#define dev_kfree_skb_any(skb) kfree_skb_chk_key(skb, skb->dev)
#endif
extern struct sk_buff *dev_alloc_skb(unsigned int length,
									 unsigned int reserve_len);
extern struct sk_buff *skb_clone(struct sk_buff *skb, int gfp_mask);
extern struct sk_buff *skb_copy(const struct sk_buff *skb, int gfp_mask,
								unsigned int reserve_len);
extern unsigned char *skb_pull(struct sk_buff *skb, unsigned int len);

//----- ------------------------------------------------------------------
// Device structure
//----- ------------------------------------------------------------------
struct net_device_stats {
	unsigned long rx_packets;  /* total packets received       */
	unsigned long tx_packets;  /* total packets transmitted    */
	unsigned long rx_dropped;  /* no space in linux buffers    */
	unsigned long tx_dropped;  /* no space available in linux  */
	unsigned long rx_bytes;	/* total bytes received         */
	unsigned long tx_bytes;	/* total bytes transmitted      */
	unsigned long rx_overflow; /* rx fifo overflow count       */
};

struct net_device {
	char name[16];
	void *priv;				   /* pointer to private data */
	unsigned char dev_addr[6]; /* set during bootup */
	int (*init)(void);
	int (*open)(struct net_device *dev);
	int (*stop)(struct net_device *dev);
	int (*hard_start_xmit)(struct sk_buff *skb, struct net_device *dev);
	int (*do_ioctl)(struct net_device *dev, struct iwreq *ifr, int cmd);
	struct net_device_stats *(*get_stats)(struct net_device *dev);
};

typedef struct {
	struct net_device *dev; /* Binding wlan driver netdev */
	void *skb;				/* pending Rx packet */
	unsigned int tx_busy;
	unsigned int rx_busy;
	unsigned char enable;
	unsigned char mac[6];
} Rltk_wlan_t;

#define netdev_priv(dev) dev->priv

extern struct net_device *alloc_etherdev(int sizeof_priv);
void free_netdev(struct net_device *dev);
int dev_alloc_name(struct net_device *net_dev, const char *ifname);

//----- ------------------------------------------------------------------
// Timer Operation
//----- ------------------------------------------------------------------
void init_timer(struct timer_list *timer);
void mod_timer(struct timer_list *timer, u32 delay_time_ms);
void cancel_timer_ex(struct timer_list *timer);
void del_timer_sync(struct timer_list *timer);
void init_timer_wrapper(void);
void deinit_timer_wrapper(void);

void rtw_init_timer(_timer *ptimer, void *adapter, TIMER_FUN pfunc, void *cntx,
					const char *name);
void rtw_set_timer(_timer *ptimer, u32 delay_time);
u8 rtw_cancel_timer(_timer *ptimer);
void rtw_del_timer(_timer *ptimer);

#endif //__WRAPPER_H__
