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
#ifndef _RTW_RECV_H_
#define _RTW_RECV_H_
#include <hal_pg.h>
#ifdef CONFIG_USB_HCI
#include "drv_types.h"
#include "skbuff.h"
#endif

#ifdef CONFIG_RECV_REORDERING_CTRL
#define NR_RECVFRAME 16 //Increase recv frame due to rx reorder - Andy Sun
#else
#if DEFRAGMENTATION
#define NR_RECVFRAME 8
#else
#ifndef CONFIG_HIGH_TP
#define NR_RECVFRAME 2 //Decrease recv frame due to memory limitation - YangJue
#else
#define NR_RECVFRAME 256
#endif
#endif
#endif

#define NR_RECVBUFF (8)

#define NR_PREALLOC_RECV_SKB (8)

#define RECV_BULK_IN_ADDR 0x80
#define RECV_INT_IN_ADDR 0x81

#define PHY_RSSI_SLID_WIN_MAX 100
#define PHY_LINKQUALITY_SLID_WIN_MAX 20

// Rx smooth factor
#define Rx_Smooth_Factor (20)

#define RXFRAME_ALIGN 8
#define RXFRAME_ALIGN_SZ (1 << RXFRAME_ALIGN)

#define DRVINFO_SZ 4 // unit is 8bytes

#define MAX_RXFRAME_CNT 512
#define MAX_RX_NUMBLKS (32)
#define RECVFRAME_HDR_ALIGN 128

#define SNAP_SIZE sizeof(struct ieee80211_snap_hdr)

#define RX_MPDU_QUEUE 0
#define RX_CMD_QUEUE 1
#define RX_MAX_QUEUE 2

#define MAX_SUBFRAME_COUNT 64

//for Rx reordering buffer control
struct recv_reorder_ctrl {
	_adapter *padapter;
	u8 enable;
	u16 indicate_seq; //=wstart_b, init_value=0xffff
	u16 wend_b;
	u8 wsize_b;
	_queue pending_recvframe_queue;
	_timer reordering_ctrl_timer;
};

struct stainfo_rxcache {
	u16 tid_rxseq[16];
};

struct smooth_rssi_data {
	u32 elements[100]; //array to store values
	u32 index;		   //index to current array to store
	u32 total_num;	 //num of valid elements
	u32 total_val;	 //sum of valid elements
};

struct signal_stat {
	u8 update_req; //used to indicate
	u8 avg_val;	//avg of valid elements
	u32 total_num; //num of valid elements
	u32 total_val; //sum of valid elements
};

struct rx_pkt_attrib {
	u16 pkt_len;
	u8 physt;
	u8 drvinfo_sz;
	u8 shift_sz;
	u8 hdrlen; //the WLAN Header Len
	u8 to_fr_ds;
	u8 amsdu;
	u8 qos;
	u8 priority;
	u8 pw_save;
	u8 mdata;
	u16 seq_num;
	u8 frag_num;
	u8 mfrag;
	u8 order;
	u8 privacy; //in frame_ctrl field
	u8 bdecrypted;
	u8 encrypt; //when 0 indicate no encrypt. when non-zero, indicate the encrypt algorith
	u8 iv_len;
	u8 icv_len;
	u8 crc_err;
	u8 icv_err;

	u16 eth_type;

	u8 dst[ETH_ALEN];
	u8 src[ETH_ALEN];
	u8 ta[ETH_ALEN];
	u8 ra[ETH_ALEN];
	u8 bssid[ETH_ALEN];

	u8 ack_policy;

	u8 tcpchk_valid; // 0: invalid, 1: valid
	u8 ip_chkrpt;	//0: incorrect, 1: correct
	u8 tcp_chkrpt;   //0: incorrect, 1: correct
					 //#endif
	u8 key_index;

	u8 mcs_rate;
	u8 rxht;
	u8 sgi;
	u8 pkt_rpt_type;
	u32 MacIDValidEntry[2]; // 64 bits present 64 entry.

	u8 data_rate;
	struct phydm_phyinfo_struct phy_info;
};

//These definition is used for Rx packet reordering.
#define SN_LESS(a, b) (((a - b) & 0x800) != 0)
#define SN_EQUAL(a, b) (a == b)
#define REORDER_WAIT_TIME (30) // (ms)

#define RECVBUFF_ALIGN_SZ 8

#define RXDESC_SIZE 24
#define RXDESC_OFFSET RXDESC_SIZE

struct recv_stat {
	unsigned int rxdw0;

	unsigned int rxdw1;

	unsigned int rxdw2;

	unsigned int rxdw3;

	unsigned int rxdw4;

	unsigned int rxdw5;

#ifdef CONFIG_PCI_HCI
	unsigned int rxdw6;

	unsigned int rxdw7;
#endif
};

struct recv_buf_stat {
	unsigned int rxdw0;

	unsigned int rxdw1;
};

#define EOR BIT(30)

#if defined(CONFIG_LX_HCI)
#define LX_MAX_RX_QUEUE 1 // MSDU packet queue, Rx Command Queue
#define LX_MAX_RX_COUNT 4 //RX_Q_DESC_NUM// 128

struct rtw_rx_ring {
#if ((RTL8195A_SUPPORT == 1) || (RTL8711B_SUPPORT == 1) || (RTL8721D_SUPPORT == 1))
	struct recv_buf_stat *desc;
#else
	struct recv_stat *desc;
#endif

	dma_addr_t dma;
	unsigned int idx;
	struct sk_buff *rx_buf[LX_MAX_RX_COUNT];
};
#endif // CONFIG_LX_HCI

#if defined(CONFIG_AXI_HCI)
#define AXI_MAX_RX_QUEUE 1 // MSDU packet queue, Rx Command Queue
#define AXI_MAX_RX_COUNT 4 //RX_Q_DESC_NUM// 128

struct rtw_rx_ring {
	struct recv_buf_stat *desc;

	dma_addr_t dma;
	unsigned int idx;
	struct sk_buff *rx_buf[AXI_MAX_RX_COUNT];
};
#endif // CONFIG_AXI_HCI

/*
accesser of recv_priv: rtw_recv_entry(dispatch / passive level); recv_thread(passive) ; returnpkt(dispatch)
; halt(passive) ;

using enter_critical section to protect
*/
struct recv_priv {
	_lock lock;

	//_queue	blk_strms[MAX_RX_NUMBLKS];    // keeping the block ack frame until return ack
	_queue free_recv_queue;
	_queue recv_pending_queue;
	_queue uc_swdec_pending_queue;

	u8 *pallocated_frame_buf;
	u8 *precv_frame_buf;

	uint free_recvframe_cnt;

	_adapter *adapter;

#ifdef PLATFORM_WINDOWS
	_nic_hdl RxPktPoolHdl;
	_nic_hdl RxBufPoolHdl;

#ifdef PLATFORM_OS_XP
	PMDL pbytecnt_mdl;
#endif
	uint counter; //record the number that up-layer will return to drv; only when counter==0 can we  release recv_priv
	NDIS_EVENT recv_resource_evt;
#endif

#if (PHYDM_LINUX_CODING_STYLE == 1)
	u32 is_any_non_be_pkts;
#else
	u32 bIsAnyNonBEPkts;
#endif
	u64 rx_bytes;
	u64 rx_pkts;
	u64 rx_drop;
	u64 rx_overflow;
	u64 last_rx_bytes;

	uint rx_icv_err;
	uint rx_largepacket_crcerr;
	uint rx_smallpacket_crcerr;
	uint rx_middlepacket_crcerr;

#ifdef CONFIG_USB_HCI
	_sema allrxreturnevt;
	uint ff_hwaddr;
	u8 rx_pending_cnt;

#ifdef CONFIG_USB_INTERRUPT_IN_PIPE
#ifdef PLATFORM_LINUX
	PURB int_in_urb;
#endif
	u8 *int_in_buf;
#endif //CONFIG_USB_INTERRUPT_IN_PIPE
	struct sk_buff_head rx_skb_queue;

#endif
#if defined(PLATFORM_LINUX) || defined(PLATFORM_FREEBSD)
#ifdef PLATFORM_FREEBSD
	struct task irq_prepare_beacon_tasklet;
	struct task recv_tasklet;
#else  //PLATFORM_FREEBSD
	struct tasklet_struct irq_prepare_beacon_tasklet;
	struct tasklet_struct recv_tasklet;
#endif //PLATFORM_FREEBSD
	struct sk_buff_head free_recv_skb_queue;
	struct sk_buff_head rx_skb_queue;
#ifdef CONFIG_RX_INDICATE_QUEUE
	struct task rx_indicate_tasklet;
	struct ifqueue rx_indicate_queue;
#endif // CONFIG_RX_INDICATE_QUEUE

#ifdef CONFIG_USE_USB_BUFFER_ALLOC_RX
	_queue recv_buf_pending_queue;
#endif // CONFIG_USE_USB_BUFFER_ALLOC_RX
#endif //defined(PLATFORM_LINUX) || defined(PLATFORM_FREEBSD)

	u8 *pallocated_recv_buf;
	u8 *precv_buf; // 4 alignment
	_queue free_recv_buf_queue;
	u32 free_recv_buf_queue_cnt;

#if defined(CONFIG_SDIO_HCI) || defined(CONFIG_GSPI_HCI) || defined(CONFIG_USB_HCI)
	_queue recv_buf_pending_queue;
#endif

#if defined(CONFIG_PCI_HCI) || defined(CONFIG_LX_HCI) || defined(CONFIG_AXI_HCI)
// Rx
#if defined(CONFIG_AXI_HCI)
	struct rtw_rx_ring rx_ring[AXI_MAX_RX_QUEUE];
#else
	struct rtw_rx_ring rx_ring[LX_MAX_RX_QUEUE];
#endif
	int rxringcount;
	u16 rxbuffersize;
#endif

	//For display the phy informatiom
	u8 is_signal_dbg;		// for debug
	u8 signal_strength_dbg; // for debug
	s8 rssi;
	s8 rxpwdb;
	u8 signal_strength;
	u8 signal_qual;
	u8 noise;
	int RxSNRdB[2];
	s8 RxRssi[2];
	int FalseAlmCnt_all;

#ifdef CONFIG_NEW_SIGNAL_STAT_PROCESS
	_timer signal_stat_timer;
	u32 signal_stat_sampling_interval;
	//u32 signal_stat_converging_constant;
	struct signal_stat signal_qual_data;
	struct signal_stat signal_strength_data;
#else  //CONFIG_NEW_SIGNAL_STAT_PROCESS
	struct smooth_rssi_data signal_qual_data;
	struct smooth_rssi_data signal_strength_data;
#endif //CONFIG_NEW_SIGNAL_STAT_PROCESS

#ifdef CONFIG_PROMISC
	u8 promisc_enabled;
	u8 promisc_len_used;
	_list promisc_list;
	_lock promisc_lock;
	u32 promisc_bk_rcr;
	u16 promisc_bk_rxfltmap2;
	u8 promisc_mgntframe_enabled;
	s8 promisc_dest_ap_rssi_avg;
#endif
};

#ifdef CONFIG_NEW_SIGNAL_STAT_PROCESS
#define rtw_set_signal_stat_timer(recvpriv) rtw_set_timer(&(recvpriv)->signal_stat_timer, (recvpriv)->signal_stat_sampling_interval)
#endif //CONFIG_NEW_SIGNAL_STAT_PROCESS

struct sta_recv_priv {

	_lock lock;
	sint option;

	_queue defrag_q; //keeping the fragment frame until defrag

	struct stainfo_rxcache rxcache;
};

struct recv_buf {
	_list list;

	//	_lock recvbuf_lock;

	//	u32	ref_cnt;

	PADAPTER adapter;
#ifdef CONFIG_USB_HCI
	u8 *pbuf;
	u8 *pallocated_buf;
#endif
	u32 len;
	u8 *phead;
	u8 *pdata;
	u8 *ptail;
	u8 *pend;

#ifdef CONFIG_USB_HCI

	PURB purb;
	dma_addr_t dma_transfer_addr; /* (in) dma addr for transfer_buffer */
	u32 alloc_sz;

#ifdef PLATFORM_OS_XP
	PIRP pirp;
#endif

#ifdef PLATFORM_OS_CE
	USB_TRANSFER usb_transfer_read_port;
#endif

	u8 irp_pending;
	int transfer_len;

#endif

	_pkt *pskb;
#ifdef PLATFORM_FREEBSD //skb solution
	struct sk_buff *pskb;
	u8 reuse;
#endif //PLATFORM_FREEBSD //skb solution
};

/*
   head  ----->

   data  ----->

   payload

   tail  ----->

   end   ----->

   len = (unsigned int )(tail - data);
*/
struct recv_frame_hdr {
	_list list;
#ifndef CONFIG_BSD_RX_USE_MBUF
	struct sk_buff *pkt;
	struct sk_buff *pkt_newalloc;
#else  // CONFIG_BSD_RX_USE_MBUF
	_pkt *pkt;
	_pkt *pkt_newalloc;
#endif // CONFIG_BSD_RX_USE_MBUF

	_adapter *adapter;

	u8 fragcnt;

	int frame_tag;

	struct rx_pkt_attrib attrib;

	uint len;
	u8 *rx_head;
	u8 *rx_data;
	u8 *rx_tail;
	u8 *rx_end;

	void *precvbuf;

	struct sta_info *psta;
#ifdef CONFIG_RECV_REORDERING_CTRL
	//for A-MPDU Rx reordering buffer control
	struct recv_reorder_ctrl *preorder_ctrl;
#endif
#ifdef CONFIG_WAPI_SUPPORT
	u8 UserPriority;
	u8 WapiTempPN[16];
	u8 WapiSrcAddr[6];
	u8 bWapiCheckPNInDecrypt;
	u8 bIsWaiPacket;
#endif
};

union recv_frame {

	union {
		_list list;
		struct recv_frame_hdr hdr;
		uint mem[RECVFRAME_HDR_ALIGN >> 2];
	} u;
};

typedef enum _RX_PACKET_TYPE {
	NORMAL_RX,  //Normal rx packet
	TX_REPORT1, //CCX
	TX_REPORT2, //TX RPT
	HIS_REPORT, // USB HISR RPT
	C2H_PACKET
} RX_PACKET_TYPE,
	*PRX_PACKET_TYPE;

extern union recv_frame *_rtw_alloc_recvframe(_queue *pfree_recv_queue); //get a free recv_frame from pfree_recv_queue
extern void rtw_init_recvframe(union recv_frame *precvframe, struct recv_priv *precvpriv);
extern int rtw_free_recvframe(union recv_frame *precvframe, _queue *pfree_recv_queue);

#define rtw_dequeue_recvframe(queue) rtw_alloc_recvframe(queue)
extern int _rtw_enqueue_recvframe(union recv_frame *precvframe, _queue *queue);

#ifdef CONFIG_TRACE_SKB
int __rtw_enqueue_recvframe(union recv_frame *precvframe, _queue *queue);
union recv_frame *__rtw_alloc_recvframe(_queue *pfree_recv_queue); //get a free recv_frame from pfree_recv_queue

#define rtw_enqueue_recvframe(precvframe, queue, Q)                      \
	do {                                                                 \
		set_skb_list_flag(precvframe->u.hdr.pkt, SKBLIST_RECVFRAME_##Q); \
		__rtw_enqueue_recvframe(precvframe, queue);                      \
	} while (0)
#define rtw_alloc_recvframe(queue, precvframe, Q)                                           \
	do {                                                                                    \
		precvframe = __rtw_alloc_recvframe(queue),                                          \
		precvframe ? clear_skb_list_flag(precvframe->u.hdr.pkt, SKBLIST_RECVFRAME_##Q) : 0, \
		precvframe                                                                          \
	} while (0)
#else
extern int rtw_enqueue_recvframe(union recv_frame *precvframe, _queue *queue);
extern union recv_frame *rtw_alloc_recvframe(_queue *pfree_recv_queue); //get a free recv_frame from pfree_recv_queue
#endif

extern void rtw_free_recvframe_queue(_queue *pframequeue, _queue *pfree_recv_queue);
u32 rtw_free_uc_swdec_pending_queue(_adapter *adapter);

#ifdef CONFIG_TRACE_SKB
sint _rtw_enqueue_recvbuf_to_head(struct recv_buf *precvbuf, _queue *queue);
sint _rtw_enqueue_recvbuf(struct recv_buf *precvbuf, _queue *queue);
struct recv_buf *_rtw_dequeue_recvbuf(_queue *queue);

#define rtw_enqueue_recvbuf_to_head(precvbuf, queue, Q)         \
	do {                                                        \
		set_skb_list_flag(precvbuf->pskb, SKBLIST_RECVBUF_##Q); \
		_rtw_enqueue_recvbuf_to_head(precvbuf, queue);          \
	} while (0)
#define rtw_enqueue_recvbuf(precvbuf, queue, Q)                 \
	do {                                                        \
		set_skb_list_flag(precvbuf->pskb, SKBLIST_RECVBUF_##Q); \
		_rtw_enqueue_recvbuf(precvbuf, queue);                  \
	} while (0)
#define rtw_dequeue_recvbuf(queue, precvbuf, Q)                                  \
	do {                                                                         \
		precvbuf = _rtw_dequeue_recvbuf(queue),                                  \
		precvbuf ? clear_skb_list_flag(precvbuf->pskb, SKBLIST_RECVBUF_##Q) : 0, \
		precvbuf                                                                 \
	} while (0)

#else
sint rtw_enqueue_recvbuf_to_head(struct recv_buf *precvbuf, _queue *queue);
sint rtw_enqueue_recvbuf(struct recv_buf *precvbuf, _queue *queue);
struct recv_buf *rtw_dequeue_recvbuf(_queue *queue);
#endif

void rtw_reordering_ctrl_timeout_handler(void *pcontext);

__inline static u8 *get_rxmem(union recv_frame *precvframe)
{
	//always return rx_head...
	if (precvframe == NULL)
		return NULL;

	return precvframe->u.hdr.rx_head;
}

__inline static u8 *get_rx_status(union recv_frame *precvframe)
{

	return get_rxmem(precvframe);
}

__inline static u8 *get_recvframe_data(union recv_frame *precvframe)
{

	//alwasy return rx_data
	if (precvframe == NULL)
		return NULL;

	return precvframe->u.hdr.rx_data;
}

__inline static u8 *recvframe_pull(union recv_frame *precvframe, sint sz)
{
	// rx_data += sz; move rx_data sz bytes  hereafter

	//used for extract sz bytes from rx_data, update rx_data and return the updated rx_data to the caller

	if (precvframe == NULL)
		return NULL;

	precvframe->u.hdr.rx_data += sz;

	if (precvframe->u.hdr.rx_data > precvframe->u.hdr.rx_tail) {
		precvframe->u.hdr.rx_data -= sz;
		return NULL;
	}

	precvframe->u.hdr.len -= sz;

	return precvframe->u.hdr.rx_data;
}

__inline static u8 *recvframe_put(union recv_frame *precvframe, sint sz)
{
	// rx_tai += sz; move rx_tail sz bytes  hereafter

	//used for append sz bytes from ptr to rx_tail, update rx_tail and return the updated rx_tail to the caller
	//after putting, rx_tail must be still larger than rx_end.

	if (precvframe == NULL)
		return NULL;

	precvframe->u.hdr.rx_tail += sz;

	if (precvframe->u.hdr.rx_tail > precvframe->u.hdr.rx_end) {
		precvframe->u.hdr.rx_tail -= sz;
		return NULL;
	}

	precvframe->u.hdr.len += sz;

	return precvframe->u.hdr.rx_tail;
}

__inline static u8 *recvframe_pull_tail(union recv_frame *precvframe, sint sz)
{
	// rmv data from rx_tail (by yitsen)

	//used for extract sz bytes from rx_end, update rx_end and return the updated rx_end to the caller
	//after pulling, rx_end must be still larger than rx_data.

	if (precvframe == NULL)
		return NULL;

	precvframe->u.hdr.rx_tail -= sz;

	if (precvframe->u.hdr.rx_tail < precvframe->u.hdr.rx_data) {
		precvframe->u.hdr.rx_tail += sz;
		return NULL;
	}

	precvframe->u.hdr.len -= sz;

	return precvframe->u.hdr.rx_tail;
}

__inline static _buffer *get_rxbuf_desc(union recv_frame *precvframe)
{
	_buffer *buf_desc = NULL;

	if (precvframe == NULL)
		return NULL;
#ifdef PLATFORM_WINDOWS
	NdisQueryPacket(precvframe->u.hdr.pkt, NULL, NULL, &buf_desc, NULL);
#endif

	return buf_desc;
}

__inline static union recv_frame *rxmem_to_recvframe(u8 *rxmem)
{
	//due to the design of 2048 bytes alignment of recv_frame, we can reference the union recv_frame
	//from any given member of recv_frame.
	// rxmem indicates the any member/address in recv_frame

	return (union recv_frame *)(((SIZE_PTR)rxmem >> RXFRAME_ALIGN) << RXFRAME_ALIGN);
}

__inline static union recv_frame *pkt_to_recvframe(_pkt *pkt)
{

	u8 *buf_star = NULL;
	union recv_frame *precv_frame = NULL;
#ifdef PLATFORM_WINDOWS
	_buffer *buf_desc;
	uint len;

	NdisQueryPacket(pkt, NULL, NULL, &buf_desc, &len);
	NdisQueryBufferSafe(buf_desc, &buf_star, &len, HighPagePriority);
#endif
	precv_frame = rxmem_to_recvframe((unsigned char *)buf_star);

	return precv_frame;
}

__inline static u8 *pkt_to_recvmem(_pkt *pkt)
{
	// return the rx_head

	union recv_frame *precv_frame = pkt_to_recvframe(pkt);

	return precv_frame->u.hdr.rx_head;
}

__inline static u8 *pkt_to_recvdata(_pkt *pkt)
{
	// return the rx_data

	union recv_frame *precv_frame = pkt_to_recvframe(pkt);

	return precv_frame->u.hdr.rx_data;
}

__inline static sint get_recvframe_len(union recv_frame *precvframe)
{
	return precvframe->u.hdr.len;
}

__inline static s32 translate_percentage_to_dbm(u32 SignalStrengthIndex)
{
	s32 SignalPower; // in dBm.

#ifndef CONFIG_SKIP_SIGNAL_SCALE_MAPPING
	// Translate to dBm (x=0.9y-95).
	SignalPower = (s32)((SignalStrengthIndex * 18) / 20);
	SignalPower -= 95;
#else
	/* Translate to dBm (x=y-100) */
	SignalPower = SignalStrengthIndex - 100;
#endif

	return SignalPower;
}

struct sta_info;

extern void _rtw_init_sta_recv_priv(struct sta_recv_priv *psta_recvpriv);
extern void mgt_dispatcher(_adapter *padapter, union recv_frame *precv_frame);
int process_recv_indicatepkts(_adapter *padapter, union recv_frame *prframe);

void rtw_rxhandler(_adapter *padapter, struct recv_buf *precvbuf);
u32 rtw_free_buf_pending_queue(_adapter *adapter);

#ifdef CONFIG_RECV_TASK_THREAD_MODE
thread_return rtw_recv_task_thread(thread_context context);
#endif

#endif
