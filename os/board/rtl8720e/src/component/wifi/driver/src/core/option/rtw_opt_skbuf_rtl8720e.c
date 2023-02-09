#include <osdep_service.h>
#include <skbuff.h>
#include <autoconf.h>
#include <section_config.h>

#ifndef CONFIG_MP_INCLUDED

#ifdef CONFIG_HIGH_TP_TEST
#define MAX_SKB_BUF_NUM      10
#define MAX_LOCAL_SKB_NUM    (MAX_SKB_BUF_NUM + 2)
#define RX_Q_DESC_NUM		4
#else
#define MAX_SKB_BUF_NUM      6
#define MAX_LOCAL_SKB_NUM    (MAX_SKB_BUF_NUM + 2)
#define RX_Q_DESC_NUM		4
#endif

unsigned int nr_recvframe = MAX_SKB_BUF_NUM;
unsigned int nr_xmitframe = MAX_SKB_BUF_NUM;
unsigned int nr_xmitbuff = MAX_SKB_BUF_NUM;
int max_local_skb_num = MAX_LOCAL_SKB_NUM;
int max_skb_buf_num = MAX_SKB_BUF_NUM;
u8 rxq_desc_num = RX_Q_DESC_NUM;

/* DO NOT modify below structure */
//----------------------------------------------------------------------------//
#if WIFI_LOGO_CERTIFICATION
#define MAX_SKB_BUF_SIZE     4144
#else
#define MAX_SKB_BUF_SIZE     1658
#endif

struct skb_buf {
	struct list_head list;
	struct sk_buff skb;
#if defined(CONFIG_AS_INIC_NP)
	u8 rsvd[10]; /* keep total size 64B alignment */
#endif
};

struct skb_data {
	/* starting address must be aligned by 32 bytes for km4 cache. */
	struct list_head list __attribute__((aligned(32)));
	unsigned char buf[MAX_SKB_BUF_SIZE];
	/* to protect ref when to invalid cache, its address must be
	 * aligned by 32 bytes. */
	atomic_t ref __attribute__((aligned(32)));
};


#if WIFI_LOGO_CERTIFICATION
#define MAX_RECVBUF_SZ     4598
#else
#define MAX_RECVBUF_SZ     2112
#endif


struct rx_ring_pool_t {
	u8 buf[RX_Q_DESC_NUM][MAX_RECVBUF_SZ];
} __attribute__((aligned(32)));

struct rx_ring_pool_t rx_ring_pool __attribute__((aligned(32)));

SRAM_NOCACHE_DATA_SECTION
#ifdef RSICV_CORE_KR4
//rx_desc size/addr 32 bytes alignment for lite-kr4 cache
u8 rx_ring_des[RX_Q_DESC_NUM][8] __attribute__((aligned(32)));
#else
u8 rx_ring_des[RX_Q_DESC_NUM][8];
#endif
//----------------------------------------------------------------------------//

/* DO NOT access skb_pool and skb_data_pool out of wlan driver */
struct skb_buf skb_pool[MAX_LOCAL_SKB_NUM];
struct skb_data skb_data_pool[MAX_SKB_BUF_NUM];
#endif
