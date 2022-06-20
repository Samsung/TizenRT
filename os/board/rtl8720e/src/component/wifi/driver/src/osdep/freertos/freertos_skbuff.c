#include <drv_types.h>
#include <freertos/freertos_skbuff.h>

//----- ------------------------------------------------------------------
// SKB Operation
//----- ------------------------------------------------------------------

static int skb_fail_count;

struct skb_buf {
	struct list_head list;
	struct sk_buff skb;
#if defined(CONFIG_INIC_IPC) && CONFIG_INIC_IPC
	u8 rsvd[16]; /* keep total size 64B alignment */
#endif
};

_WEAK int max_local_skb_num = MAX_LOCAL_SKB_NUM;
#if defined(CONFIG_INIC_IPC) && CONFIG_INIC_IPC
_WEAK struct skb_buf skb_pool[MAX_LOCAL_SKB_NUM] __attribute__((aligned(64)));
#else
_WEAK struct skb_buf skb_pool[MAX_LOCAL_SKB_NUM];
#endif
static struct list_head wrapper_skbbuf_list;
int skbbuf_used_num;
int max_skbbuf_used_num;

struct skb_data {
	/* starting address must be aligned by 32 bytes for km4 cache. */
	struct list_head list __attribute__((aligned(32)));
	unsigned char buf[MAX_SKB_BUF_SIZE];
	/* to protect ref when to invalid cache, its address must be
	 * aligned by 32 bytes. */
	atomic_t ref __attribute__((aligned(32)));
};

_WEAK int max_skb_buf_num = MAX_SKB_BUF_NUM;
SRAM_BD_DATA_SECTION
#if defined(CONFIG_INIC_IPC) && CONFIG_INIC_IPC
_WEAK struct skb_data skb_data_pool[MAX_SKB_BUF_NUM] __attribute__((aligned(64)));
#else
_WEAK struct skb_data skb_data_pool[MAX_SKB_BUF_NUM];
#endif
SRAM_BD_DATA_SECTION
struct list_head skbdata_list;

int skbdata_used_num;
int max_skbdata_used_num;

#ifdef CONFIG_TRACE_SKB
extern void snapshot_skb(void);
#endif

void skb_fail_inc(void)
{
	unsigned int irq_flags = save_and_cli();
	skb_fail_count ++;
	restore_flags(irq_flags);
}

int skb_fail_get_and_rst(void)
{
	int fail;

	unsigned int irq_flags = save_and_cli();
	fail = skb_fail_count;
	skb_fail_count = 0;
	restore_flags(irq_flags);

	return fail;
}

static __inline__ unsigned char *get_buf_from_poll(struct list_head *phead, int *count)
{
	struct sk_buff *skb;
	struct list_head *plist;

	if (list_empty(phead)) {
#ifdef CONFIG_TRACE_SKB
		snapshot_skb();
#endif
		return NULL;
	}

	plist = phead->next;
	list_del_init(plist);

	skb = (struct sk_buff *)((unsigned int)plist + sizeof(struct list_head));

	*count = *count + 1;

	return (unsigned char *)skb;
}

static void release_buf_to_poll(unsigned char *buf, struct list_head *phead, int *count)
{
	struct list_head *plist;
	plist = (struct list_head *)(((unsigned int)buf) - sizeof(struct list_head));

	list_add_tail(plist, phead);

	*count = *count - 1;
}

void init_skb_pool(void)
{
	int i;

	memset(skb_pool, '\0', max_local_skb_num * sizeof(struct skb_buf));
	INIT_LIST_HEAD(&wrapper_skbbuf_list);

	for (i = 0; i < max_local_skb_num; i++) {
		INIT_LIST_HEAD(&skb_pool[i].list);
		list_add_tail(&skb_pool[i].list, &wrapper_skbbuf_list);
#ifdef CONFIG_TRACE_SKB
		set_skb_list_flag(&skb_pool[i].skb, SKBLIST_POOL);
#endif
	}
	skbbuf_used_num = 0;
	max_skbbuf_used_num = 0;

}

_WEAK void init_skb_data_pool(void)
{
	int i;

	memset(skb_data_pool, '\0', max_skb_buf_num * sizeof(struct skb_data));
	INIT_LIST_HEAD(&skbdata_list);

	for (i = 0; i < max_skb_buf_num; i++) {
		INIT_LIST_HEAD(&skb_data_pool[i].list);
		list_add_tail(&skb_data_pool[i].list, &skbdata_list);
	}
	skbdata_used_num = 0;
	max_skbdata_used_num = 0;
}

_WEAK void deinit_skb_data_pool(void)
{
}

struct sk_buff *alloc_skb(int size)
{
	struct sk_buff *skb;
	unsigned char *data;

	if (size + HAL_INTERFACE_CMD_STATUS_LEN > MAX_SKB_BUF_SIZE) {
		DBG_TRACE("no enough room(max.%d) for desired packet(%d)", MAX_SKB_BUF_SIZE, size + HAL_INTERFACE_CMD_STATUS_LEN);
	}

	unsigned int irq_flags = save_and_cli();
	skb = (struct sk_buff *)get_buf_from_poll(&wrapper_skbbuf_list, &skbbuf_used_num);
	restore_flags(irq_flags);

	if (skb == NULL) {
#ifdef CONFIG_TRACE_SKB
		DBG_ERR("Wait for skbbuf, skbbuf:%d, skbdata:%d\r\n", skbbuf_used_num, skbdata_used_num);
		show_skb();
#else
		DBG_ERR("Wait for skbbuf");
#endif
		return NULL;
	}

#ifdef CONFIG_TRACE_SKB
	memset(skb, '\0', (unsigned int) & ((struct sk_buff *)0)->liston[0]);
#else
	memset(skb, '\0', sizeof(*skb));
#endif

	if (size + GSPI_STATUS_LEN > MAX_SKB_BUF_SIZE) {
		data = rtw_zmalloc(size + GSPI_STATUS_LEN);
	} else {
		unsigned int irq_flags = save_and_cli();
		data = (unsigned char *) get_buf_from_poll(&skbdata_list, &skbdata_used_num);
		restore_flags(irq_flags);
	}

	if (data == NULL) {
#ifdef CONFIG_TRACE_SKB
		DBG_ERR("Wait for skbdata, skbbuf:%d, skbdata:%d\r\n", skbbuf_used_num, skbdata_used_num);
		show_skb();
#else
		DBG_ERR("Wait for skbdata\n");
#endif
		unsigned int irq_flags =save_and_cli();
		release_buf_to_poll((unsigned char *)skb, &wrapper_skbbuf_list, &skbbuf_used_num);
		restore_flags(irq_flags);
		return NULL;
	}

	if (size + GSPI_STATUS_LEN > MAX_SKB_BUF_SIZE) {
		skb->dyalloc_flag = 1;
	} else {
		skb->dyalloc_flag = 0;
	}

	if (skbbuf_used_num > max_skbbuf_used_num) {
		max_skbbuf_used_num = skbbuf_used_num;
	}

	if (skbdata_used_num > max_skbdata_used_num) {
		max_skbdata_used_num = skbdata_used_num;
	}

#ifdef CONFIG_TRACE_SKB
	clear_skb_list_flag(skb, SKBLIST_POOL);
#endif

	size = SKB_DATA_ALIGN(size);
#ifndef CONFIG_HIGH_TP
	skb_assign_buf(skb, (unsigned char *)data, size);
#else
	skb_assign_buf(skb, (unsigned char *)data, MAX_SKB_BUF_SIZE - HAL_INTERFACE_CMD_STATUS_LEN);
#endif

	atomic_set(&LIST_CONTAINOR(skb->head, struct skb_data, buf)->ref, 1);

	return skb;
}

struct sk_buff *dev_alloc_skb(unsigned int length, unsigned int reserve_len)
{
	struct sk_buff *skb;

	skb = alloc_skb(length + SKB_DATA_ALIGN(reserve_len));

	//DBG_871X("%s(): length=%d reserve_len=%d skb->data=%p skb->tail=%p skb->end=%p\n", __func__, length, reserve_len, skb->data, skb->tail, skb->end);

	if (skb) {
		skb_reserve(skb, SKB_DATA_ALIGN(reserve_len));
	}

	return skb;
}

void kfree_skb(struct sk_buff *skb)
{
	unsigned int irq_flags = save_and_cli();
	if (skb->dyalloc_flag == 1) {
		skb->dyalloc_flag = 0;
		rtw_free(skb->head);
	} else {
		if (ATOMIC_DEC_AND_TEST(&LIST_CONTAINOR(skb->head, struct skb_data, buf)->ref)) {
			release_buf_to_poll(skb->head, &skbdata_list, &skbdata_used_num);
		}
	}
	release_buf_to_poll((unsigned char *)skb, &wrapper_skbbuf_list, &skbbuf_used_num);

	restore_flags(irq_flags);
}

void kfree_skb_chk_key(struct sk_buff *skb, struct net_device *root_dev)
{
	/* To avoid gcc warnings */
	(void) root_dev;

	kfree_skb(skb);
}

#ifdef CONFIG_TRACE_SKB
int _set_skb_list_flag(struct sk_buff *skb, unsigned int queueflag)
{
	if (skb) {
		unsigned int liston = skb->liston[skb->list_idx];
		save_and_cli();
		if (++skb->list_idx >= TRACE_SKB_DEPTH) {
			skb->list_idx = 0;
		}
		skb->liston[skb->list_idx] = liston | queueflag;
		//skb->liston[++skb->list_idx >= TRACE_SKB_DEPTH ? 0 : skb->list_idx] = skb->liston[skb->list_idx] | queueflag;
		//skb->list_idx = skb->list_idx >= TRACE_SKB_DEPTH ? 0 : skb->list_idx;
		restore_flags();

		//do{ // for break point setting
		//	int idx = skb->list_idx;
		//
		//	if(!(skb->liston[idx] & SKBLIST_POOL) &&
		//		(skb->liston[idx] & 0x7FFFFFFF))
		//	{
		//		idx = --idx < 0 ? TRACE_SKB_DEPTH-1 : idx;
		//		if(!(skb->liston[idx] & 0x7FFFFFFF) &&
		//			((skb->liston[idx] & SKBLIST_POOL)))
		//			break;
		//	}
		//}while(0);
		return skb->list_idx;
	}

	return -1;
}

int _clear_skb_list_flag(struct sk_buff *skb, unsigned int queueflag)
{
	if (skb) {
		unsigned int liston = skb->liston[skb->list_idx];
		save_and_cli();
		if (++skb->list_idx >= TRACE_SKB_DEPTH) {
			skb->list_idx = 0;
		}
		skb->liston[skb->list_idx] = liston & ~queueflag;
		//skb->liston[++skb->list_idx >= TRACE_SKB_DEPTH ? 0 : skb->list_idx] = skb->liston[skb->list_idx] & ~queueflag;
		//skb->list_idx = skb->list_idx >= TRACE_SKB_DEPTH ? 0 : skb->list_idx;
		restore_flags();

		//do{ // for break point setting
		//	int idx = skb->list_idx;
		//
		//	if(!(skb->liston[idx] & SKBLIST_POOL) &&
		//		(skb->liston[idx] & 0x7FFFFFFF))
		//	{
		//		idx = --idx < 0 ? TRACE_SKB_DEPTH-1 : idx;
		//		if(!(skb->liston[idx] & 0x7FFFFFFF) &&
		//			((skb->liston[idx] & SKBLIST_POOL)))
		//			break;
		//	}
		//
		//	if(skb->liston[idx] == 0)
		//	{
		//		idx = --idx < 0 ? TRACE_SKB_DEPTH-1 : idx;
		//		if(skb->liston[idx] == 0){
		//			idx = --idx < 0 ? TRACE_SKB_DEPTH-1 : idx;
		//			if(skb->liston[idx] == 0x00410000)
		//				break;
		//		}
		//	}
		//}while(0);
		return skb->list_idx;
	}

	return -1;
}

static int liston_copy[MAX_LOCAL_SKB_NUM][TRACE_SKB_DEPTH];
static const char *funcname_copy[MAX_LOCAL_SKB_NUM][TRACE_SKB_DEPTH];
static char *liston_string[] = {
	"TRANSIENT",

	"RECVBUF",
	"RECVBUF_FREEQUEUE",
	"RECVBUF_PENDINGQUEUE",
	NULL,

	"RECVFRAME",
	"RECVFRAME_FREEQUEUE",
	"RECVFRAME_SWDECQUEUE",
	"RECVFRAME_REORDERQUEUE",

	"XMITBUF",
	"XMITEXTBUF",
	"XMITBUF_FREEQUEUE",
	"XMITEXTBUF_FREEQUEUE",
	"XMITBUF_PENDINGQUEUE",
	"XMITBUF_PENDING0QUEUE",
	"XMITBUF_PENDING1QUEUE",
	"XMITBUF_PENDING2QUEUE",

	"XMITFRAME",
	"XMITFRAME_FREEQUEUE",
	"XMITFRAME_SLEEPQUEUE",
	NULL,
	"XMITFRAME_VOQUEUE",
	"XMITFRAME_VIQUEUE",
	"XMITFRAME_BEQUEUE",
	"XMITFRAME_BKQUEUE",
	"XMITFRAME_BMQUEUE",
	NULL,
	NULL,
	NULL,

	"POOL",
	NULL,
	NULL,
	NULL
};

int liston_string_getidx(int flag)
{
	int i, idx = -1;

	for (i = 0; i < 32; i++) {
		if (((flag >> i) & 0x1)) {
			idx = i;
		}
	}

	return idx == -1 ? 0 : idx + 1;
}


void snapshot_skb(void)
{
	int i, j, idx;

	for (i = 0; i < MAX_LOCAL_SKB_NUM; i++) {
		idx = skb_pool[i].skb.list_idx;
		for (j = 0; j < TRACE_SKB_DEPTH; j++) {
			liston_copy[i][j] = skb_pool[i].skb.liston[idx];
			funcname_copy[i][j] = skb_pool[i].skb.funcname[idx--];
			idx = idx < 0 ? TRACE_SKB_DEPTH - 1 : idx;
		}
	}
}

void show_skb(void)
{
	int i, j, idx;

	printk("\n\n\rskbbuf_used_num:%d, skbdata_used_num:%d\r\n", skbbuf_used_num, skbdata_used_num);
	for (i = 0; i < MAX_LOCAL_SKB_NUM; i++) {
		printk("skbbuf[%d] is on list:", i);
		for (j = 0; j < TRACE_SKB_DEPTH; j++) {
			//printk(" %08x <-", liston_copy[i][j]);
			idx = liston_string_getidx(liston_copy[i][j]);
			if (idx == 0) {
				continue;
			}
			if (liston_string[idx]) {
				printk(" %s <-", liston_string[idx]);
			} else {
				printk(" oops!!!");
			}
		}
		printk(" ...\r\n");
		printk("skbbuf[%d] go through:", i);
		for (j = 0; j < TRACE_SKB_DEPTH; j++) {
			if (funcname_copy[i][j] > (const char *)0x08000000) {
				printk(" %s <-", (char *)funcname_copy[i][j]);
			} else {
				printk(" * <-");
			}
		}
		printk(" ...\r\n");
	}
}
void dump_skb_list(void)
{
	int i, j, idx;

	save_and_cli();
	snapshot_skb();
	restore_flags();

	show_skb();
}

#endif //CONFIG_TRACE_SKB

/**
 *	skb_put - add data to a buffer
 *	@skb: buffer to use
 *	@len: amount of data to add
 *
 *	This function extends the used data area of the buffer. If this would
 *	exceed the total buffer size the kernel will panic. A pointer to the
 *	first byte of the extra data is returned.
 */

unsigned char *skb_put(struct sk_buff *skb, unsigned int len)
{
	unsigned char *tmp = skb->tail;
	skb->tail += len;
	skb->len += len;
	if (skb->tail > skb->end) {
		ASSERT(0);
	}

	return tmp;
}

static __inline__ unsigned char *__skb_pull(struct sk_buff *skb, unsigned int len)
{
	skb->len -= len;
	skb->data = (unsigned char *)(((unsigned int)skb->data) + len);

	return skb->data;
}

static __inline__ unsigned char *__skb_push(struct sk_buff *skb, unsigned int len)
{
	skb->len += len;
	skb->data = (unsigned char *)(((unsigned int)skb->data) - len);

	return skb->data;
}

/**
 *	skb_reserve - adjust headroom
 *	@skb: buffer to alter
 *	@len: bytes to move
 *
 *	Increase the headroom of an empty &sk_buff by reducing the tail
 *	room. This is only allowed for an empty buffer.
 */

void skb_reserve(struct sk_buff *skb, unsigned int len)
{
	skb->data += len;
	skb->tail += len;
}

#if 0
static __inline__ void skb_queue_head_init(struct sk_buff_head *list)
{
	list->prev = (struct list_head *)list;
	list->next = (struct list_head *)list;
	list->qlen = 0;
}

/**
 *      __skb_queue_tail - queue a buffer at the list tail
 *      @list: list to use
 *      @newsk: buffer to queue
 *
 *      Queue a buffer at the end of a list. This function takes no locks
 *      and you must therefore hold required locks before calling it.
 *
 *      A buffer cannot be placed on two lists at the same time.
 */

static __inline__ void __skb_queue_tail(struct sk_buff_head *list, struct sk_buff *newsk)
{
	struct sk_buff *prev, *next;

	newsk->list = list;
	list->qlen++;
	next = (struct sk_buff *)list;
	prev = next->prev;
	newsk->next = next;
	newsk->prev = prev;
	next->prev = newsk;
	prev->next = newsk;
}

/**
 *      skb_queue_tail - queue a buffer at the list tail
 *      @list: list to use
 *      @newsk: buffer to queue
 *
 *      Queue a buffer at the tail of the list. This function takes the
 *      list lock and can be used safely with other locking &sk_buff functions
 *      safely.
 *
 *      A buffer cannot be placed on two lists at the same time.
 */

static __inline__ void skb_queue_tail(struct sk_buff_head *list, struct sk_buff *newsk)
{
	save_and_cli();
	__skb_queue_tail(list, newsk);
	restore_flags();
}
#endif

void skb_assign_buf(struct sk_buff *skb, unsigned char *buf, unsigned int len)
{
	skb->head = buf;
	skb->data = buf;
	skb->tail = buf;
	skb->end = buf + len;
}

unsigned char *skb_tail_pointer(const struct sk_buff *skb)
{
	return skb->tail;
}
#if 0
static __inline__ void skb_reset_tail_pointer(struct sk_buff *skb)
{
	skb->tail = skb->data;
}
#endif

unsigned char *skb_end_pointer(const struct sk_buff *skb)
{
	return skb->end;
}

void skb_set_tail_pointer(struct sk_buff *skb, const int offset)
{
	skb->tail = skb->data + offset;
}

unsigned char *skb_pull(struct sk_buff *skb, unsigned int len)
{
	if (len > skb->len) {
		return NULL;
	}

	return __skb_pull(skb, len);
}

unsigned char *skb_push(struct sk_buff *skb, unsigned int len)
{
	if ((skb->data - len) < skb->head) {
		return NULL;
	}
	return __skb_push(skb, len);
}

struct sk_buff *skb_copy(const struct sk_buff *skb, int gfp_mask, unsigned int reserve_len)
{
	/* To avoid gcc warnings */
	(void) gfp_mask;

	struct sk_buff *copy;

	/*
	 * Allocate the copy buffer
	 */
	copy = dev_alloc_skb(skb->len, reserve_len);

	if (copy == NULL) {
		return NULL;
	}

	/* Set the tail pointer and length */
	skb_put(copy, skb->len);
	memcpy(copy->data, skb->data, skb->len);

	copy->dev = skb->dev;

	return copy;
}

/**
 *      skb_clone       -       duplicate an sk_buff
 *      @skb: buffer to clone
 *      @gfp_mask: allocation priority
 *
 *      Duplicate an &sk_buff. The new one is not owned by a socket. Both
 *      copies share the same packet data but not structure. The new
 *      buffer has a reference count of 1. If the allocation fails the
 *      function returns %NULL otherwise the new buffer is returned.
 *
 *      If this function is called from an interrupt gfp_mask() must be
 *      %GFP_ATOMIC.
 */

#define C(x) clone->x = skb->x

struct sk_buff *skb_clone(struct sk_buff *skb, int gfp_mask)
{
	/* To avoid gcc warnings */
	(void) gfp_mask;

	struct sk_buff *clone;

	unsigned int irq_flags = save_and_cli();
	clone = (struct sk_buff *)get_buf_from_poll(&wrapper_skbbuf_list, &skbbuf_used_num);
	restore_flags(irq_flags);

	if (clone == NULL) {
		DBG_ERR("Wait to clone skbbuf");
		return NULL;
	}

#ifdef CONFIG_TRACE_SKB
	clear_skb_list_flag(clone, SKBLIST_POOL);
#endif

#ifdef CONFIG_TRACE_SKB
	memset(clone, '\0', (unsigned int) & ((struct sk_buff *)0)->liston[0]);
#else
	memset(clone, '\0', sizeof(*clone));
#endif

	ATOMIC_INC(&LIST_CONTAINOR(skb->head, struct skb_data, buf)->ref);

	C(dev);
	C(len);
	C(head);
	C(data);
	C(tail);
	C(end);

	return clone;
}

#ifndef __LIST_H
#warning "DLIST_NOT_DEFINE!!!!!!"
//----- ------------------------------------------------------------------
// Linled List
//----- ------------------------------------------------------------------
void list_add(struct list_head *new, struct list_head *head)
{
	__list_add(new, head, head->next);
}

void list_add_tail(struct list_head *new, struct list_head *head)
{
	__list_add(new, head->prev, head);
}
#endif


