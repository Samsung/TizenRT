/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/
#ifndef _RTW_RECV_H_
#define _RTW_RECV_H_
#include <hal_pg.h>

#define NR_RECVFRAME 16	//Increase recv frame due to rx reorder - Andy Sun


#ifndef CONFIG_HIGH_TP
//	#define NR_RECVBUFF (8)	//Decrease recv buffer due to memory limitation - Alex Fang
#define NR_RECVBUFF (1)	//Decrease recv buffer due to memory limitation - YangJue
#else
#define NR_RECVBUFF (32)
#endif //CONFIG_HIGH_TP

#define RECV_BULK_IN_ADDR			0x80
#define RECV_INT_IN_ADDR			0x81

#define PHY_RSSI_SLID_WIN_MAX			100
#define PHY_LINKQUALITY_SLID_WIN_MAX		20

// Rx smooth factor
#define Rx_Smooth_Factor			(20)

#define RXFRAME_ALIGN				8
#define RXFRAME_ALIGN_SZ			(1<<RXFRAME_ALIGN)

#define DRVINFO_SZ				4 // unit is 8bytes

#define MAX_RXFRAME_CNT				512
#define MAX_RX_NUMBLKS				(32)
#define RECVFRAME_HDR_ALIGN			128

#define SNAP_SIZE sizeof(struct ieee80211_snap_hdr)

#define RX_MPDU_QUEUE				0
#define RX_CMD_QUEUE				1
#define RX_MAX_QUEUE				2

#define MAX_SUBFRAME_COUNT			64
#define RX_SC_ENTRY_NUM				4

#define SIG_SAMP_INTERVAL			1000

//for Rx reordering buffer control
struct recv_reorder_ctrl {
	_adapter	*padapter;
	u8 enable;
	u16 indicate_seq;//=wstart_b, init_value=0xffff
	u16 wend_b;
	u16 wsize_b;
	_queue pending_recvframe_queue;
	_timer reordering_ctrl_timer;
	u8 bReorderWaiting;
};

struct	stainfo_rxcache	{
	u16 	tid_rxseq[16];
	/*
		unsigned short 	tid0_rxseq;
		unsigned short 	tid1_rxseq;
		unsigned short 	tid2_rxseq;
		unsigned short 	tid3_rxseq;
		unsigned short 	tid4_rxseq;
		unsigned short 	tid5_rxseq;
		unsigned short 	tid6_rxseq;
		unsigned short 	tid7_rxseq;
		unsigned short 	tid8_rxseq;
		unsigned short 	tid9_rxseq;
		unsigned short 	tid10_rxseq;
		unsigned short 	tid11_rxseq;
		unsigned short 	tid12_rxseq;
		unsigned short 	tid13_rxseq;
		unsigned short 	tid14_rxseq;
		unsigned short 	tid15_rxseq;
	*/
};

struct smooth_rssi_data {
	u32	elements[100];	//array to store values
	u32	index;			//index to current array to store
	u32	total_num;		//num of valid elements
	u32	total_val;		//sum of valid elements
};

struct rx_pkt_attrib	{
	u16	pkt_len;
	u8	physt;
	u8	drvinfo_sz;
	u8	shift_sz;
	u8	hdrlen; //the WLAN Header Len
	u8 	to_fr_ds;
	u8 	amsdu;
	u8	qos;
	u8	priority;
	u8	pw_save;
	u8	mdata;
	u16	seq_num;
	u8	frag_num;
	u8	mfrag;
	u8	order;
	u8	privacy; //in frame_ctrl field
	u8	bdecrypted;
	u8	encrypt; //when 0 indicate no encrypt. when non-zero, indicate the encrypt algorith
	u8	iv_len;
	u8	icv_len;
	u8	crc_err;
	u8	icv_err;

	u16 eth_type;

	u8 	dst[ETH_ALEN];
	u8 	src[ETH_ALEN];
	u8 	ta[ETH_ALEN];
	u8 	ra[ETH_ALEN];
	u8 	bssid[ETH_ALEN];

	u8 ack_policy;

//#ifdef CONFIG_TCP_CSUM_OFFLOAD_RX
	u8	tcpchk_valid; // 0: invalid, 1: valid
	u8	ip_chkrpt; //0: incorrect, 1: correct
	u8	tcp_chkrpt; //0: incorrect, 1: correct
//#endif
	u8 	key_index;

	u8	mcs_rate;
	u8	rxht;
	u8 	sgi;
	u8 	pkt_rpt_type;
	u32	MacIDValidEntry[2];	// 64 bits present 64 entry.


	u16   data_rate;
	/*
		u8	signal_qual;
		s8	rx_mimo_signal_qual[2];
		u8	signal_strength;
		u32	RxPWDBAll;
		s32	RecvSignalPower;
	*/
#ifdef RX_SHORTCUT
	u8 wifi_addr[3 * ETH_ALEN];
#endif

	struct phydm_phyinfo_struct phy_info;
#if (PHYDM_VERSION == 3) /*for halbb physts_rxd*/
	u8 A1_matched;
	u8 macid_from_rxdesc;
	u8 frame_type; //subtype not included
	u8 bc;
#endif

};

//These definition is used for Rx packet reordering.
#define SN_LESS(a, b)		(((a-b)&0x800)!=0)
#define SN_EQUAL(a, b)	(a == b)
#if defined(CONFIG_RX_REORDER_WIN_SIZE)
#define REORDER_WIN_SIZE	CONFIG_RX_REORDER_WIN_SIZE	//NR_RECVFRAME
#else
#define REORDER_WIN_SIZE	4	// reduce due to skbdata num limit//NR_RECVFRAME
#endif
//#define REORDER_ENTRY_NUM	128
#define REORDER_WAIT_TIME	(30) // (ms)

#define RECVBUFF_ALIGN_SZ 8

#define RXDESC_SIZE	24
#define RXDESC_OFFSET RXDESC_SIZE

struct recv_stat {
	unsigned int rxdw0;

	unsigned int rxdw1;

	unsigned int rxdw2;

	unsigned int rxdw3;

	unsigned int rxdw4;

	unsigned int rxdw5;

};


struct recv_buf_stat {
	unsigned int rxdw0;

	unsigned int rxdw1;
};

#define EOR BIT(30)

#define MAX_RX_QUEUE		1 // MSDU packet queue, Rx Command Queue

#if defined(CONFIG_LX_HCI)
#define LX_MAX_RX_COUNT		4 //RX_Q_DESC_NUM// 128
struct rtw_rx_ring {
#if (RTL8721D_SUPPORT == 1)
	struct recv_buf_stat	*desc;
#else
	struct recv_stat	*desc;
#endif
	dma_addr_t		dma;
	unsigned int		idx;
	struct sk_buff		*rx_buf[LX_MAX_RX_COUNT];
};
#endif // CONFIG_LX_HCI

#if defined(CONFIG_AXI_HCI)
#if defined(CONFIG_RX_RING_COUNT)
#define AXI_MAX_RX_COUNT	CONFIG_RX_RING_COUNT //RX_Q_DESC_NUM// 128
#else
#define AXI_MAX_RX_COUNT	4 //RX_Q_DESC_NUM// 128
#endif
struct rtw_rx_ring {
	struct recv_buf_stat	*desc;

	dma_addr_t		dma;
	unsigned int		idx;
	struct sk_buff		*rx_buf[AXI_MAX_RX_COUNT];
};
#endif // CONFIG_AXI_HCI

#ifdef RX_SHORTCUT
struct rx_sc_entry {
	u8 shortcut_hdr[2 * ETH_ALEN];
	u8 wifi_addr[3 * ETH_ALEN]; //A1 A2 A3
	u32 is_dirty;//0 is free, 1 is alloced
};
#endif // RX_SHORTCUT

/*
accesser of recv_priv: rtw_recv_entry(dispatch / passive level); recv_thread(passive) ; returnpkt(dispatch)
; halt(passive) ;

using enter_critical section to protect
*/
struct recv_priv {
	_queue			free_recv_queue;
	_queue			uc_swdec_pending_queue;
	u8			*pallocated_frame_buf;
	u8			*precv_frame_buf;
	uint			free_recvframe_cnt;
	_adapter		*adapter;
	u8			*pallocated_recv_buf;
	u8			*precv_buf;    // 4 alignment
	_queue			free_recv_buf_queue;
	u32			free_recv_buf_queue_cnt;
	struct rtw_rx_ring	rx_ring[MAX_RX_QUEUE];
	int			rxringcount;
	u16			rxbuffersize;
#ifdef RX_SHORTCUT
	struct rx_sc_entry	rx_sc_ent[RX_SC_ENTRY_NUM];
	int			rx_sc_replace_idx;
#endif
};

#ifdef CONFIG_PROMISC
struct promisc_priv {
	u8 promisc_enabled;
	u8 promisc_len_used;
	u8 promisc_type;
	_list promisc_list;
	u32 promisc_bk_rcr;
	u16 promisc_bk_rxfltmap2;
	u8 promisc_mgntframe_enabled;
	s8 promisc_dest_ap_rssi_avg;
};
#endif

struct sta_recv_priv {
	sint	option;
	_queue defrag_q;	 //keeping the fragment frame until defrag
	struct	stainfo_rxcache rxcache;
};

struct recv_buf {
	_list list;
	PADAPTER adapter;
	u32	len;
	u8	*phead;
	u8	*pdata;
	u8	*ptail;
	u8	*pend;
	_pkt	*pskb;
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
	_list	list;
#ifndef CONFIG_BSD_RX_USE_MBUF
	struct sk_buff	 *pkt;
	struct sk_buff	 *pkt_newalloc;
#else // CONFIG_BSD_RX_USE_MBUF
	_pkt	*pkt;
	_pkt *pkt_newalloc;
#endif // CONFIG_BSD_RX_USE_MBUF

	_adapter  *adapter;

	u8 fragcnt;

	int frame_tag;

	struct rx_pkt_attrib attrib;

	uint  len;
	u8 *rx_head;
	u8 *rx_data;
	u8 *rx_tail;
	u8 *rx_end;

	void *precvbuf;


	//
	struct sta_info *psta;
	//for A-MPDU Rx reordering buffer control
	struct recv_reorder_ctrl *preorder_ctrl;
};

union recv_frame {

	union {
		_list list;
		struct recv_frame_hdr hdr;
		uint mem[RECVFRAME_HDR_ALIGN >> 2];
	} u;

	//uint mem[MAX_RXSZ>>2];

};

typedef enum _RX_PACKET_TYPE {
	NORMAL_RX = 0,//Normal rx packet
	CH_INFO_NDPA = 1,//Channel info trigger by NDPA or normal pkt
	CH_INFO_RSP = 2,//Channel info trigger by response pkt
	PPDU_STATUS = 4,// phystatus
	C2H_PACKET = 5
} RX_PACKET_TYPE, *PRX_PACKET_TYPE;

extern void rtw_init_recvframe(union recv_frame *precvframe, struct recv_priv *precvpriv);
extern int	 rtw_free_recvframe(union recv_frame *precvframe, _queue *pfree_recv_queue);

#define rtw_dequeue_recvframe(queue) rtw_alloc_recvframe(queue)

extern union recv_frame *rtw_alloc_recvframe(_queue *pfree_recv_queue);   //get a free recv_frame from pfree_recv_queue
union recv_frame *recvframe_chk_defrag(PADAPTER padapter, union recv_frame *precv_frame);
extern void rtw_free_recvframe_queue(_queue *pframequeue,  _queue *pfree_recv_queue);
u32 rtw_free_uc_swdec_pending_queue(_adapter *adapter);

void rtw_reordering_ctrl_timeout_handler(void *pcontext);


__inline static u8 *get_rxmem(union recv_frame *precvframe)
{
	//always return rx_head...
	if (precvframe == NULL) {
		return NULL;
	}

	return precvframe->u.hdr.rx_head;
}

__inline static u8 *get_rx_status(union recv_frame *precvframe)
{

	return get_rxmem(precvframe);

}



__inline static u8 *get_recvframe_data(union recv_frame *precvframe)
{

	//alwasy return rx_data
	if (precvframe == NULL) {
		return NULL;
	}

	return precvframe->u.hdr.rx_data;

}

__inline static u8 *recvframe_pull(union recv_frame *precvframe, sint sz)
{
	// rx_data += sz; move rx_data sz bytes  hereafter

	//used for extract sz bytes from rx_data, update rx_data and return the updated rx_data to the caller


	if (precvframe == NULL) {
		return NULL;
	}


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

	if (precvframe == NULL) {
		return NULL;
	}

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

	if (precvframe == NULL) {
		return NULL;
	}

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

	if (precvframe == NULL) {
		return NULL;
	}

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
	/* To avoid gcc warnings */
	(void) pkt;

	u8 *buf_star = NULL;
	union recv_frame *precv_frame = NULL;

	precv_frame = rxmem_to_recvframe((unsigned char *)buf_star);

	return precv_frame;
}

__inline static u8 *pkt_to_recvmem(_pkt *pkt)
{
	// return the rx_head

	union recv_frame *precv_frame = pkt_to_recvframe(pkt);

	return 	precv_frame->u.hdr.rx_head;

}

__inline static u8 *pkt_to_recvdata(_pkt *pkt)
{
	// return the rx_data

	union recv_frame *precv_frame = pkt_to_recvframe(pkt);

	return 	precv_frame->u.hdr.rx_data;

}


__inline static sint get_recvframe_len(union recv_frame *precvframe)
{
	return precvframe->u.hdr.len;
}


__inline static s32 translate_percentage_to_dbm(u32 SignalStrengthIndex)
{
	s32	SignalPower; // in dBm.

	// Translate to dBm (x=0.9y-95).
	SignalPower = (s32)((SignalStrengthIndex * 18) / 20);
	SignalPower -= 95;

	return SignalPower;
}


struct sta_info;

extern void  mgt_dispatcher(_adapter *padapter, union recv_frame *precv_frame);
int process_recv_indicatepkts(_adapter *padapter, union recv_frame *prframe);

void rtw_rxhandler(_adapter *padapter, struct recv_buf *precvbuf);
union recv_frame *decryptor(_adapter *padapter, union recv_frame *precv_frame);

#ifdef RX_SHORTCUT
void rtw_free_rx_sc_entrys(ADAPTER *Adapter);
int rtw_get_rx_sc_index(ADAPTER *Adapter, unsigned char *hdr);
int rtw_get_rx_sc_free_entry(ADAPTER *Adapter, unsigned char *hdr);
int rtw_check_rx_shortcut_path(_adapter *adapter, union recv_frame *precv_frame);
#endif
extern void rtw_signal_stat_timer_hdl(void *FunctionContext);

#endif

