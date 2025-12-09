#ifndef __RWNX_TX_H_
#define __RWNX_TX_H_

#include "bk_compiler.h"
#include "rwnx_intf.h"
#include "lwip/pbuf.h"

/* Define __section macro if not already defined */
#ifndef __section
#define __section(x) __attribute__((__section__(x)))
#endif

struct sk_buff;

#define CFG_MAX_SW_RETRY_CNT                5
#define MGMT_FRAME_MAX_TX_PBD_CNT          3

#define RWNX_HWQ_BK                     0
#define RWNX_HWQ_BE                     1
#define RWNX_HWQ_VI                     2
#define RWNX_HWQ_VO                     3
#define RWNX_HWQ_BCMC                   4
#define RWNX_HWQ_NB                     NX_TXQ_CNT
#define RWNX_HWQ_ALL_ACS (RWNX_HWQ_BK | RWNX_HWQ_BE | RWNX_HWQ_VI | RWNX_HWQ_VO)
#define RWNX_HWQ_ALL_ACS_BIT ( BIT(RWNX_HWQ_BK) | BIT(RWNX_HWQ_BE) |    \
                               BIT(RWNX_HWQ_VI) | BIT(RWNX_HWQ_VO) )

extern const int rwnx_tid2hwq[];

#define WARN(condition, format...) ({			\
		int __ret_warn_on = !!(condition);		\
		if (unlikely(__ret_warn_on))			\
			RWNX_LOGI(format);					\
		unlikely(__ret_warn_on);				\
	})

#ifndef WARN_ON
#define WARN_ON(condition) ({						\
	int __ret_warn_on = !!(condition);				\
	if (unlikely(__ret_warn_on))					\
		RWNX_LOGI("WARN: %:%d", __func__, __LINE__);\
	unlikely(__ret_warn_on);					\
})
#endif

#define WARN_ONCE(condition, format...)	({			\
	static bool __section(".data.unlikely") __warned;		\
	int __ret_warn_once = !!(condition);			\
								\
	if (unlikely(__ret_warn_once))				\
		if (WARN(!__warned, format)) 			\
			__warned = true;			\
	unlikely(__ret_warn_once);				\
})

#define BUG_ON(condition) do { if (unlikely(condition)) BK_ASSERT(0); } while(0)


void rwnx_set_traffic_status(STA_INF_PTR sta, bool available, u8 ps_id);
void rwnx_tx_push(struct sk_buff *skb);
uint32_t rwnx_tx_get_pbuf_chain_cnt(struct pbuf *p);
int rwnx_start_xmit(uint8_t vif_idx, struct pbuf *p, BUS_MSG_T *msg);
void rwnx_start_xmit_mgmt(struct sk_buff *skb);
void rwnx_start_xmit_raw_ex(struct sk_buff *skb,raw_tx_cntrl_t *raw_tx_cntrl);

void fhost_tx_cfm_push(uint8_t queue_idx, struct txdesc *txdesc);
int fhost_txbuf_push(void *desc, uint8_t queue_idx);

#endif
