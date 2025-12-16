#include "common/sys_config.h"
//#include "bk_wifi_adapter_internal.h"
#include "lwip/pbuf.h"

#if CONFIG_RWNX_SW_TXQ
#include <stddef.h>
#include <sys/types.h>
#include "skbuff.h"
#include "os/mem.h"
#include "bk_lwipif_tizenrt.h"
int32_t skb_cnt = 0;

/**
 *	skb_unlink	-	remove a buffer from a list
 *	@skb: buffer to remove
 *	@list: list to use
 *
 *	Remove a packet from a list. The list locks are taken and this
 *	function is atomic with respect to other list locked calls
 *
 *	You must know what list the SKB is on.
 */
void skb_unlink(struct sk_buff *skb, struct sk_buff_head *list)
{
	unsigned long flags;

	spin_lock_irqsave(&list->lock, flags);
	__skb_unlink(skb, list);
	spin_unlock_irqrestore(&list->lock, flags);
}

/**
 *	skb_append	-	append a buffer
 *	@old: buffer to insert after
 *	@newsk: buffer to insert
 *	@list: list to use
 *
 *	Place a packet after a given packet in a list. The list locks are taken
 *	and this function is atomic with respect to other list locked calls.
 *	A buffer cannot be placed on two lists at the same time.
 */
void skb_append(struct sk_buff *old, struct sk_buff *newsk, struct sk_buff_head *list)
{
	unsigned long flags;

	spin_lock_irqsave(&list->lock, flags);
	//os_printf("old %p, newsk %p, list %p\n", old, newsk, list);
	__skb_queue_after(list, old, newsk);
	spin_unlock_irqrestore(&list->lock, flags);
}

/**
 *	skb_queue_tail - queue a buffer at the list tail
 *	@list: list to use
 *	@newsk: buffer to queue
 *
 *	Queue a buffer at the tail of the list. This function takes the
 *	list lock and can be used safely with other locking &sk_buff functions
 *	safely.
 *
 *	A buffer cannot be placed on two lists at the same time.
 */
__IRAM2 void skb_queue_tail(struct sk_buff_head *list, struct sk_buff *newsk)
{
	unsigned long flags;

	spin_lock_irqsave(&list->lock, flags);
	__skb_queue_tail(list, newsk);
	spin_unlock_irqrestore(&list->lock, flags);
}

/**
 *	skb_queue_head - queue a buffer at the list head
 *	@list: list to use
 *	@newsk: buffer to queue
 *
 *	Queue a buffer at the start of the list. This function takes the
 *	list lock and can be used safely with other locking &sk_buff functions
 *	safely.
 *
 *	A buffer cannot be placed on two lists at the same time.
 */
void skb_queue_head(struct sk_buff_head *list, struct sk_buff *newsk)
{
	unsigned long flags;

	spin_lock_irqsave(&list->lock, flags);
	__skb_queue_head(list, newsk);
	spin_unlock_irqrestore(&list->lock, flags);
}

/**
 * alloc_skb - allocate a network buffer
 * @size: size to allocate
 *
 * This function is a convenient wrapper around __alloc_skb().
 */
struct sk_buff *alloc_skb(UINT32 len, int layer)
{
	struct sk_buff *skb = 0;
	struct pbuf *p;

	/* no payload here */
	skb = (struct sk_buff *)os_zalloc_sram(sizeof(struct sk_buff));

	if (!skb)
		goto alloc_exit;
#if BK_SS_WIFI_DP
	p = bk_wlan_buf_alloc(len);
#else
	p = pbuf_alloc(layer, len, PBUF_RAM);
#endif
	if (!p) {
		os_free(skb);
		skb = 0;
		goto alloc_exit;
	}

	skb->p = p;
	skb->msdu_ptr = p->payload;
	skb->len = p->tot_len;
	skb_inc_pending_cnt();

alloc_exit:
	return skb;
}

struct sk_buff *alloc_skb_for_tx_mgmt(UINT32 len)
{
	struct sk_buff *skb = 0;
	unsigned char *payload = 0;

	/* no payload here */
	skb = (struct sk_buff *)os_zalloc_sram(sizeof(struct sk_buff));

	if (!skb)
		goto alloc_exit;

	payload = (unsigned char *)os_zalloc_sram(len);
	if (!payload) {
		os_free(skb);
		skb = 0;
		goto alloc_exit;
	}

	skb->p = 0;
	skb->msdu_ptr = payload;
	skb->len = len;
	skb_inc_pending_cnt();

alloc_exit:
	return skb;
}

__IRAM2 struct sk_buff *alloc_skb_with_pbuf(struct pbuf *p)
{
	struct sk_buff *skb = 0;

	skb = (struct sk_buff *)os_zalloc_sram(sizeof(struct sk_buff));

	if (!skb)
		goto alloc_exit;

	skb->p = p;
	skb->msdu_ptr = p->payload;
	skb->len = p->tot_len;
	skb_inc_pending_cnt();

alloc_exit:
	return skb;
}

__IRAM2 void kfree_skb(struct sk_buff *skb)
{
	if (!skb)
		return;

	if (skb->ftxdesc) {
		os_free(skb->ftxdesc);
		skb->ftxdesc = NULL;
	}
	if (skb->p) {
#if BK_SS_WIFI_DP
		bk_wlan_buf_free(skb->p);
#else
		pbuf_free(skb->p);
#endif
		skb->p = NULL;
	} else if (skb->msdu_ptr) {
		os_free(skb->msdu_ptr);
		skb->msdu_ptr = NULL;
	}
	os_free(skb);
	skb_dec_pending_cnt();
}

void dev_kfree_skb_any(struct sk_buff *skb)
{
	kfree_skb(skb);
}
#endif

