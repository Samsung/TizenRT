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
#ifndef _RTW_XMIT_H_
#define _RTW_XMIT_H_

/*---------------------------------------
  Define MAX_XMITBUF_SZ
---------------------------------------*/
#if defined(CONFIG_LX_HCI) || defined(CONFIG_AXI_HCI)

#ifdef CONFIG_TX_AGGREGATION	//effect only for SDIO and GSPI Interface
#define MAX_XMITBUF_SZ	(20480)	// 20k
#else

#if defined(CONFIG_LX_HCI) || defined(CONFIG_AXI_HCI)
#define HAL_INTERFACE_OVERHEAD_XMIT_BUF 0
#endif

// Consideration for MAX_XMITBUF_SZ size
// 	Check more detail information in MAX_SKB_BUF_SIZE
//	Tx: [INTF_CMD][TX_DESC][WLAN_HDR][QoS][IV][SNAP][Data][MIC][ICV][INTF_STATUS]
//	HAL_INTERFACE_OVERHEAD: HAL_INTERFACE_CMD is 4/0 for SPI/PCIE, HAL_INTERFACE_STATUS is 8/0 for SPI/PCIE
//	WLAN_MAX_ETHFRM_LEN : May not be required because WLAN_HEADER +SNAP can totally
//			cover ethernet header.Keep in only for safety.

#define MAX_XMITBUF_SZ		(HAL_INTERFACE_OVERHEAD_XMIT_BUF+\
							TXDESC_SIZE+\
							WLAN_MAX_PROTOCOL_OVERHEAD + WLAN_MAX_ETHFRM_LEN +\
							SKB_RESERVED_FOR_SAFETY)

#endif	//#ifdef CONFIG_TX_AGGREGATION

#endif	//interface define. SDIO/GSPI/LXbus/PCI/USB

/*--------------------------------------------------------------*/
/*  Define MAX_XMITBUF_SZ										*/
/*--------------------------------------------------------------*/
#if defined(CONFIG_LX_HCI) || defined(CONFIG_AXI_HCI)
#define NR_XMITBUFF (8)
#endif //interface define

/*--------------------------------------------------------------*/
/*  Define XMITBUF_ALIGN_SZ										*/
/*--------------------------------------------------------------*/
#define XMITBUF_ALIGN_SZ 4

#define MAX_CMDBUF_SZ	(5120)	//(4096)
/*--------------------------------------------------------------*/
/*  Define xmit extension buff, size/numbers							*/
/*--------------------------------------------------------------*/
#define MAX_XMIT_EXTBUF_SZ	(1536)

#define NR_XMIT_EXTBUFF		(8)	//Decrease ext xmit buffer due to memory limitation - Alex Fang

#define MAX_NUMBLKS		(1)

#define XMIT_VO_QUEUE (0)
#define XMIT_VI_QUEUE (1)
#define XMIT_BE_QUEUE (2)
#define XMIT_BK_QUEUE (3)

#if defined(CONFIG_PLATFORM_8735B)
#define VO_QUEUE_INX		0
#define VI_QUEUE_INX		1
#define BE_QUEUE_INX		2
#define BK_QUEUE_INX		3
#define BCN_QUEUE_INX		4
#define MGT_QUEUE_INX		5
#define HIGH_QUEUE_INX		6
#define HIGH1_QUEUE_INX		7
#define H2C_QUEUE_INX		8
#define HW_QUEUE_ENTRY		9
#else
#define VO_QUEUE_INX		0
#define VI_QUEUE_INX		1
#define BE_QUEUE_INX		2
#define BK_QUEUE_INX		3
#define BCN_QUEUE_INX		4
#define MGT_QUEUE_INX		5
#define HIGH_QUEUE_INX		6
#define TXCMD_QUEUE_INX		7
#ifdef CONFIG_WLAN_HAL_TEST
#define HIGH1_QUEUE_INX		8
#define HIGH2_QUEUE_INX		9
#define HIGH3_QUEUE_INX		10
#define HIGH4_QUEUE_INX		11
#define HIGH5_QUEUE_INX		12
#define HIGH6_QUEUE_INX		13
#define HIGH7_QUEUE_INX		14
#define HW_QUEUE_ENTRY		15
#else
#define HW_QUEUE_ENTRY		8
#endif
#endif // #if defined(CONFIG_PLATFORM_8735B)

#define WEP_IV(pattrib_iv, dot11txpn, keyidx)\
do{\
	pattrib_iv[0] = dot11txpn._byte_.TSC0;\
	pattrib_iv[1] = dot11txpn._byte_.TSC1;\
	pattrib_iv[2] = dot11txpn._byte_.TSC2;\
	pattrib_iv[3] = ((keyidx & 0x3)<<6);\
	dot11txpn.val = (dot11txpn.val == 0xffffff) ? 0: (dot11txpn.val+1);\
}while(0)


#define TKIP_IV(pattrib_iv, dot11txpn, keyidx)\
do{\
	pattrib_iv[0] = dot11txpn._byte_.TSC1;\
	pattrib_iv[1] = (dot11txpn._byte_.TSC1 | 0x20) & 0x7f;\
	pattrib_iv[2] = dot11txpn._byte_.TSC0;\
	pattrib_iv[3] = BIT(5) | ((keyidx & 0x3)<<6);\
	pattrib_iv[4] = dot11txpn._byte_.TSC2;\
	pattrib_iv[5] = dot11txpn._byte_.TSC3;\
	pattrib_iv[6] = dot11txpn._byte_.TSC4;\
	pattrib_iv[7] = dot11txpn._byte_.TSC5;\
	dot11txpn.val = dot11txpn.val == 0xffffffffffffULL ? 0: (dot11txpn.val+1);\
}while(0)

#define AES_IV(pattrib_iv, dot11txpn, keyidx)\
do{\
	pattrib_iv[0] = dot11txpn._byte_.TSC0;\
	pattrib_iv[1] = dot11txpn._byte_.TSC1;\
	pattrib_iv[2] = 0;\
	pattrib_iv[3] = BIT(5) | ((keyidx & 0x3)<<6);\
	pattrib_iv[4] = dot11txpn._byte_.TSC2;\
	pattrib_iv[5] = dot11txpn._byte_.TSC3;\
	pattrib_iv[6] = dot11txpn._byte_.TSC4;\
	pattrib_iv[7] = dot11txpn._byte_.TSC5;\
	dot11txpn.val = dot11txpn.val == 0xffffffffffffULL ? 0: (dot11txpn.val+1);\
}while(0)

#define GCMP_IV(pattrib_iv, dot11txpn, keyidx)\
do {\
	pattrib_iv[0] = dot11txpn._byte_.TSC0;\
	pattrib_iv[1] = dot11txpn._byte_.TSC1;\
	pattrib_iv[2] = 0;\
	pattrib_iv[3] = BIT(5) | ((keyidx & 0x3)<<6);\
	pattrib_iv[4] = dot11txpn._byte_.TSC2;\
	pattrib_iv[5] = dot11txpn._byte_.TSC3;\
	pattrib_iv[6] = dot11txpn._byte_.TSC4;\
	pattrib_iv[7] = dot11txpn._byte_.TSC5;\
	dot11txpn.val = dot11txpn.val == 0xffffffffffffULL ? 0 : (dot11txpn.val+1);\
} while (0)


#if defined(CONFIG_RTL8721D) || defined(CONFIG_RTL8730A) \
	|| defined(CONFIG_RTL8710C) || defined(CONFIG_RTL8720E) || defined(CONFIG_RTL8730E) || defined(CONFIG_RTL8721F)
#define TXDESC_SIZE 40
#elif defined(CONFIG_RTL8195B) || defined(CONFIG_RTL8735B)
#define TXDESC_SIZE 48
#else
#define TXDESC_SIZE 32
#endif

#define TXDESC_OFFSET TXDESC_SIZE
#define TX_DESC_NEXT_DESC_OFFSET	40
#define TX_FRAGMENTATION_THRESHOLD 	2346

#ifdef TX_SHORTCUT
#define TX_SC_ENTRY_NUM 		4
#endif

union Keytype {
	u8   skey[16];
	u32  lkey[4];
};

// Suppose (TX_DESC_MODE=1) ==> Segment number for each tx_buf_desc is 4. 2X4 = 8 (double words).
struct tx_buf_desc {
	unsigned int txdw0;
	unsigned int txdw1;
	unsigned int txdw2;
	unsigned int txdw3;
	unsigned int txdw4;
	unsigned int txdw5;
	unsigned int txdw6;
	unsigned int txdw7;
};

struct tx_desc {
	//DWORD 0
	unsigned int txdw0;
	unsigned int txdw1;
	unsigned int txdw2;
	unsigned int txdw3;
	unsigned int txdw4;
	unsigned int txdw5;
	unsigned int txdw6;
	unsigned int txdw7;
#if defined(CONFIG_LX_HCI)|| defined(CONFIG_RTL8195B) || defined(CONFIG_RTL8710C) || defined(CONFIG_RTL8735B) || \
	defined(CONFIG_RTL8730A) || defined(CONFIG_RTL8720E) || defined(CONFIG_RTL8730E) || defined(CONFIG_RTL8721F)
	unsigned int txdw8;
	unsigned int txdw9;
#endif
#if defined(CONFIG_RTL8195B) || defined(CONFIG_RTL8735B)
	unsigned int txdw10;
	unsigned int txdw11;
#endif
};


union txdesc {
	struct tx_desc txdesc;
	unsigned int value[TXDESC_SIZE >> 2];
};

#if defined(CONFIG_AXI_HCI) || defined(CONFIG_WLAN_HAL_TEST)
#define PCI_MAX_TX_QUEUE_COUNT	 HW_QUEUE_ENTRY
#else
#define PCI_MAX_TX_QUEUE_COUNT	8
#endif

struct rtw_tx_ring {
#if ((RTL8721D_SUPPORT == 1) || (RTL8735B_SUPPORT == 1) || (RTL8720E_SUPPORT == 1) || (RTL8730E_SUPPORT == 1)) || (RTL8721F_SUPPORT == 1)
	struct tx_buf_desc	*desc;
#else
	struct tx_desc	*desc;
#endif
	dma_addr_t		dma;
	unsigned int		idx;
	unsigned int		entries;
	_queue			queue;
	u32				qlen;
};

struct	hw_xmit	{
	_queue sta_queue;
	int	accnt;
};

//reduce size
struct pkt_attrib {
	u8	type;
	u8	subtype;
	u8	bswenc;
	u8	dhcp_pkt;
	u16	ether_type;
	u16	seqnum;
#if defined(CONFIG_RTL8195B) || defined(CONFIG_RTL8710C) || defined(CONFIG_RTL8735B)
	u8	hw_ssn_sel;	/* for HW_SEQ0,1,2,3 */
#endif
	u16	pkt_hdrlen;	//the original 802.3 pkt header len
	u16	hdrlen;		//the WLAN Header Len
	u32	pktlen;		//the original 802.3 pkt raw_data len (not include ether_hdr data)
	u32	last_txcmdsz;
	u8	encrypt;	//when 0 indicate no encrypt. when non-zero, indicate the encrypt algorith
#if defined(CONFIG_CONCURRENT_MODE)
	u8	bmc_camid;
#endif
	u8	iv_len;
	u8	icv_len;
	u8	iv[18];
	u8	icv[16];
	u8	priority;
	u8	ack_policy;
	u8	mac_id;
	u8	vcs_mode;	//virtual carrier sense method
	u8 	dst[ETH_ALEN];
	u8	src[ETH_ALEN];
	u8	ta[ETH_ALEN];
	u8 	ra[ETH_ALEN];
	u8	key_idx;
	u8	qos_en;
	u8	ht_en;
	u8	raid;//rate adpative id
	u8	bwmode;
	u8	ch_offset;//PRIME_CHNL_OFFSET
	u8	sgi;//short GI
	u8	ampdu_en;//tx ampdu enable
	u8	mdata;//more data bit
	u8	pctrl;//per packet txdesc control enable
	u8	triggered;//for ap mode handling Power Saving sta
	u8	qsel;
	u8	eosp;
	u8	order;// +htc/order bit: fc->bit15
	u8	rate;
	u8	intel_proxim;
	u8 	retry_ctrl;
	u8 	retry_limit_by_user; //for user control retry limit
	struct sta_info *psta;

#ifdef TX_SHORTCUT
	u8 sc_entry_id;
	u8 ether_hdr[14];
#endif
	u8	navusehdr;	//NAVUSEHDR

#if defined (CONFIG_MSFT) && CONFIG_MSFT
	u8 rate_fallback_table_idx;
#endif
#ifdef CONFIG_RELAY_MESH
	u8	tx_swap;
	u8 	ul_swap;
#endif
};

#define WLANHDR_OFFSET	64

#define NULL_FRAMETAG		(0x0)
#define DATA_FRAMETAG		0x01
#define L2_FRAMETAG		0x02
#define MGNT_FRAMETAG		0x03
#define AMSDU_FRAMETAG		0x04

#define EII_FRAMETAG		0x05
#define IEEE8023_FRAMETAG  	0x06

#define MP_FRAMETAG		0x07

#define TXAGG_FRAMETAG 		0x08

enum {
	XMITBUF_DATA = 0,
	XMITBUF_MGNT = 1,
	XMITBUF_CMD = 2,
};

struct  submit_ctx {
	u32 submit_time; /* */
	u32 timeout_ms; /* <0: not synchronous, 0: wait forever, >0: up to ms waiting */
	int status; /* status for operation */
};

enum {
	RTW_SCTX_SUBMITTED = -1,
	RTW_SCTX_DONE_SUCCESS = 0,
	RTW_SCTX_DONE_UNKNOWN,
	RTW_SCTX_DONE_BUF_ALLOC,
	RTW_SCTX_DONE_BUF_FREE,
	RTW_SCTX_DONE_WRITE_PORT_ERR,
	RTW_SCTX_DONE_TX_DESC_NA,
	RTW_SCTX_DONE_TX_DENY,
};


void rtw_sctx_init(struct submit_ctx *sctx, int timeout_ms);
int rtw_sctx_wait(struct submit_ctx *sctx);
void rtw_sctx_done_err(struct submit_ctx **sctx, int status);
void rtw_sctx_done(struct submit_ctx **sctx);

typedef struct _XIMT_BUF_ {
	u32			AllocatBufAddr;
	u32			BufAddr;
	u32			BufLen;
} XIMT_BUF, *PXIMT_BUF;

struct xmit_buf {
	_list			list;
	_adapter		*padapter;
	_pkt			*pkt;
	u8			*pbuf;
	void			*priv_data;
	u16			buf_tag; // 0: Normal xmitbuf, 1: extension xmitbuf, 2:cmd xmitbuf
	u16			flags;
	u32			alloc_sz;
	u32			len;
	struct submit_ctx	*sctx;
#if defined(DBG_XMIT_BUF )|| defined(DBG_XMIT_BUF_EXT)
	u8			no;
#endif
#if defined(CONFIG_LX_HCI) || defined(CONFIG_AXI_HCI)
	XIMT_BUF		BufInfo[4];
	u32			BlockNum;
#endif
};

struct xmit_frame {
	_list			list;
	struct pkt_attrib	attrib;
	_pkt			*pkt;
	int			frame_tag;
	_adapter		*padapter;
	u8			*buf_addr;
	struct xmit_buf		*pxmitbuf;

#if defined(CONFIG_LX_HCI) || defined(CONFIG_AXI_HCI)
	u32 TxDexAddr;
	u32 HdrLen;
	u32 PayLoadAddr;
	u32 PayLoadLen;
	u32 TotalLen;
	u32 BlockNum;
	XIMT_BUF BufInfo[4];
	BOOLEAN NoCoalesce;
#endif
#ifdef CONFIG_XMIT_ACK
	u8 ack_report;
#endif
#ifdef TX_SHORTCUT
	int tx_shortcut_idx;
#endif
	u8 raw_type;//Test code construct trigger frame for mu test
};

struct tx_servq {
	_list	tx_pending;
	_queue	sta_pending;
	int qcnt;
};

struct sta_xmit_priv {
	sint	option;
	sint	apsd_setting;	//When bit mask is on, the associated edca queue supports APSD.
	struct tx_servq	blk_q;
	_list 	legacy_dz;
	_list  apsd;
	u16 txseq_tid[MAXTID];
};


struct	hw_txqueue	{
	volatile sint	head;
	volatile sint	tail;
	volatile sint 	free_sz;	//in units of 64 bytes
	volatile sint      free_cmdsz;
	volatile sint	 txsz[8];
	uint	ff_hwaddr;
	uint	cmd_hwaddr;
	sint	ac_tag;
};

struct agg_pkt_info {
	u16 offset;
	u16 pkt_len;
};


struct xmit_priv {
	_lock			lock;
	u8			*pallocated_frame_buf;
	u8			*pxmit_frame_buf;
	uint			free_xmitframe_cnt;
	_queue			free_xmit_queue;
	_adapter		*adapter;
	u8			vcs;
	// Tx
	struct rtw_tx_ring	tx_ring[PCI_MAX_TX_QUEUE_COUNT];
	u8			beaconDMAing;		//flag of indicating beacon is transmiting to HW by DMA
	_queue			free_xmitbuf_queue;
	u8			*pallocated_xmitbuf;
	u8			*pxmitbuf;
	uint			free_xmitbuf_cnt;
#if USE_XMIT_EXTBUFF
	_queue			free_xmit_extbuf_queue;
	u8			*pallocated_xmit_extbuf;
	u8			*pxmit_extbuf;
	uint			free_xmit_extbuf_cnt;
#endif
#if (defined(CONFIG_RTL8195B) || defined(CONFIG_RTL8735B))
	u8			hw_ssn_seq_no;/* mapping to REG_HW_SEQ 0,1,2,3 */
#endif // (defined(CONFIG_RTL8195B) || defined(CONFIG_RTL8735B))
#ifdef CONFIG_XMIT_ACK
	u8			ccx_txok;
#endif
	u16			tx_agg_max_num;
};

extern struct xmit_buf *rtw_alloc_xmitbuf_ext(struct xmit_priv *pxmitpriv, u32 size);
extern s32 rtw_free_xmitbuf_ext(struct xmit_priv *pxmitpriv, struct xmit_buf *pxmitbuf);

extern struct xmit_buf *rtw_alloc_xmitbuf(struct xmit_priv *pxmitpriv);
extern s32 rtw_free_xmitbuf(struct xmit_priv *pxmitpriv, struct xmit_buf *pxmitbuf);

extern struct xmit_frame *rtw_alloc_xmitframe(struct xmit_priv *pxmitpriv);
extern s32 rtw_free_xmitframe(struct xmit_priv *pxmitpriv, struct xmit_frame *pxmitframe);
extern void rtw_free_xmitframe_queue(struct xmit_priv *pxmitpriv, _queue *pframequeue);
struct tx_servq *rtw_get_sta_pending(_adapter *padapter, struct sta_info *psta);
extern s32 rtw_xmitframe_enqueue(_adapter *padapter, struct xmit_frame *pxmitframe);
extern struct xmit_frame *rtw_dequeue_xframe(struct xmit_priv *pxmitpriv, struct hw_xmit *phwxmit_i);

void rtw_count_tx_stats(_adapter *padapter, struct xmit_frame *pxmitframe, int sz);
extern void rtw_update_protection(_adapter *padapter, u8 *ie, uint ie_len);
extern s32 rtw_make_wlanhdr(_adapter *padapter, u8 *hdr, struct pkt_attrib *pattrib);
extern s32 rtw_put_snap(u8 *data, u16 h_proto);

extern s32 rtw_xmit_classifier(_adapter *padapter, struct xmit_frame *pxmitframe);
extern u32 rtw_calculate_wlan_pkt_size_by_attribue(struct pkt_attrib *pattrib);
#define rtw_wlan_pkt_size(f) rtw_calculate_wlan_pkt_size_by_attribue(&f->attrib)
extern s32 rtw_xmitframe_coalesce(_adapter *padapter, _pkt *pkt, struct xmit_frame *pxmitframe);
s32 _rtw_init_hw_txqueue(struct hw_txqueue *phw_txqueue, u8 ac_tag);
void _rtw_init_sta_xmit_priv(struct sta_xmit_priv *psta_xmitpriv);


s32 rtw_txframes_pending(_adapter *padapter);
s32 rtw_txframes_sta_ac_pending(_adapter *padapter, struct pkt_attrib *pattrib);
void rtw_txframes_update_attrib_vcs_info(_adapter *padapter, struct xmit_frame *pxmitframe);


s32 _rtw_init_xmit_priv(struct xmit_priv *pxmitpriv, _adapter *padapter);
void _rtw_free_xmit_priv(struct xmit_priv *pxmitpriv);


s32 rtw_xmit(_adapter *padapter, _pkt **pkt);

sint xmitframe_enqueue_for_sleeping_sta(_adapter *padapter, struct xmit_frame *pxmitframe);
void stop_sta_xmit(_adapter *padapter, struct sta_info *psta);
void wakeup_sta_to_xmit(_adapter *padapter, struct sta_info *psta);
void xmit_delivery_enabled_frames(_adapter *padapter, struct sta_info *psta);

u8 query_ra_short_GI(struct sta_info *psta, u8 bw);
u8	qos_acm(u8 acm_mask, u8 priority);

s32 xmitframe_addmic(_adapter *padapter, struct xmit_frame *pxmitframe);
s32 xmitframe_swencrypt(_adapter *padapter, struct xmit_frame *pxmitframe);

#ifdef CONFIG_XMIT_THREAD_MODE
void	enqueue_pending_xmitbuf(struct xmit_priv *pxmitpriv, struct xmit_buf *pxmitbuf);
struct xmit_buf	*dequeue_pending_xmitbuf(struct xmit_priv *pxmitpriv);
struct xmit_buf	*dequeue_pending_xmitbuf_under_survey(struct xmit_priv *pxmitpriv);
sint	check_pending_xmitbuf(struct xmit_priv *pxmitpriv);
thread_return	rtw_xmit_thread(thread_context context);
#endif

u32	rtw_get_ff_hwaddr(struct xmit_frame	*pxmitframe);
int dump_mgntframe(_adapter *padapter, struct xmit_frame *pmgntframe);

extern s32 rtw_xmit_data(PADAPTER padapter, struct xmit_frame *pxmitframe);
extern s32 rtw_xmit_xmitbuf(_adapter *padapter, struct xmit_buf *pxmitbuf);
#ifdef CONFIG_IEEE80211W
extern s32 rtw_mgmt_xmitframe_coalesce(_adapter *padapter, _pkt *pkt, struct xmit_frame *pxmitframe);
#endif

#ifdef TX_SHORTCUT
void rtw_free_tx_sc_entrys(ADAPTER *Adapter, struct sta_info *pstat);
int rtw_get_tx_sc_index(struct sta_info *pstat, unsigned char *hdr);
int rtw_get_tx_sc_free_entry(struct sta_info *pstat, unsigned char *hdr);
#endif

extern unsigned int nr_xmitframe;
extern unsigned int nr_xmitbuff;
#endif	//_RTL871X_XMIT_H_

