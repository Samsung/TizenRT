#ifndef HWMP_H
#define HWMP_H

#include "basic_types.h"
#include "wireless.h"
#include "wifi.h"
#include "autoconf.h"
#include "netif.h"
#include "wifi_conf.h"
#include "wifi_constants.h"
#include "sockets.h"
#include "mesh.h"
#include "task.h"

#ifndef _AMEBA_
#define _AMEBA_
#endif

#ifndef _RTL8187B_
#define _RTL8187B_
#endif


#ifndef __uint64_t
typedef __uint64_t	u_int64_t;
#endif

#ifndef caddr_t
typedef void *caddr_t;
#endif

#ifndef cpu_to_le16
#define cpu_to_le16(x) ((__u16)(x))
#endif

#define USE_IPC             0

#if     USE_IPC
#define IPC_BASED_PATHSEL
#else
#undef  IPC_BASED_PATHSEL
#define FIFO_SIZE           1024
static void *FIFO_BUF;
#endif

#ifndef SZ_HASH_IDX2
#define SZ_HASH_IDX2        512 //in mesh.h
#endif

#ifndef ETH_P_ALL
#define ETH_P_ALL           0x0003
#endif

#define BR_STATE_FORWARDING 3
#define BR_STATE_BLOCKING   4
#define PF_PACKET           17 //not used in lwip_socket() 

#define NODE_CFG_PORTOAL	1
#define NODE_CFG_BE_ROOT	0
#define NODE_CFG_DAEMON		0
#define NODE_CFG_IFNAME     "wlan0"
#define NODE_CFG_IFACE_IDX  0//1 //interface index for "wlan0"

#ifndef sockaddr_ll
struct sockaddr_ll {
	unsigned short  sll_family;
	unsigned short  sll_protocol;
	int		        sll_ifindex;
	unsigned short	sll_hatype;
	unsigned char	sll_pkttype;
	unsigned char	sll_halen;
	unsigned char	sll_addr[8];
};
#endif


#ifndef timespec
typedef unsigned long time_t;
struct timespec {
	time_t  tv_sec;
	time_t  tv_nsec;
};
#endif

// Original define in \linux-2.4.18\include\linux\byteorder\generic.h
//#define cpu_to_le16 __cpu_to_le16
//#define le16_to_cpu __le16_to_cpu

//#define PATHSEL_FIFO		"/var/pathselection.fifo"
//#define FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

//#define MAX_MSG_SIZE	1600
//#define WLAN_HDR_A4_MESH_MGT_LEN 34
//#define ETH_ALEN		ETH_ALEN
#define MAX_MPP_NUM		15
#define _MESH_HEADER_TTL_	MAX_MPP_NUM

#define SIOC_GET_ROUTING_INFO 0x8BAB // send routing info to user space
//#define  SIOC_GET_PATHSWITCH_THD 0x8BAD //

//tsananiu begin
//#define SAVEPID_IOCTL   0x8BB0   //PID ioctl
//tsananiu


#define _MESH_CATEGORY_ID_             13

/*used in Mesh category */
#define _HWMP_MESH_PATHSEL_ACTION_ID_  1
#define _GANN_ACTION_ID_               2


// The following info for 8186 is defined in AP/auth/include/1x_ioct.h
#if defined(_RTL865X_) || defined(_RTL8187B_)

// for backward compatible with non-ipc-based
// struct size need less than MAXDATALEN(1560)	, which defined in iwreq.c

#pragma pack(1)
typedef struct _DOT11s_GEN_RREQ_PACKET {
	unsigned char	EventId;
	unsigned char   IsMoreEvent;
	unsigned char	MyMACAddr[ETH_ALEN];
	unsigned char	destMACAddr[ETH_ALEN];
	unsigned char	TTL;
	unsigned short	Seq_num;
} DOT11s_GEN_RREQ_PACKET;
#pragma pack()

#pragma pack(1)
typedef struct _DOT11s_GEN_RERR_PACKET {
	unsigned char	EventId;
	unsigned char   IsMoreEvent;
	unsigned char	MyMACAddr[ETH_ALEN];
	unsigned char	SorNMACAddr[ETH_ALEN];   //this address is the neighbor that hello message finds it is unreachabel.
	// if flag == 1, this field means disconnected neighbor MAC addr
	// if flag == 2, this field means source MAC addr of the data frame
	unsigned char	DataDestMAC[ETH_ALEN];	// this field is valid if and only if flag == 2
	unsigned char	PrehopMAC[ETH_ALEN];		// this field is valid if and only if flag == 2
	unsigned char   TTL;
	unsigned short  Seq_num;
	unsigned char   Flag;       // flag = 1 means the link to neighbor has broken
	// flag = 2 means this MP doesn't have the nexthop information for the destination in pathseleciton table
} DOT11s_GEN_RERR_PACKET;
#pragma pack()

#pragma pack(1)
typedef struct _DOT11s_RECV_PACKET {
	unsigned char	EventId;
	unsigned char   IsMoreEvent;
	unsigned char	MyMACAddr[ETH_ALEN];
	unsigned char	PreHopMACAddr[ETH_ALEN];
	unsigned char	DesMACAddr[ETH_ALEN];
	unsigned int	Pre_Hop_Metric;
	unsigned char	ReceiveData[128];
	unsigned char	TTL;
	unsigned short	Seq_num;
	unsigned char	Is6AddrFormat;
	unsigned char	MACAddr5[ETH_ALEN];
	unsigned char	MACAddr6[ETH_ALEN];
#ifdef CONFIG_RTK_MESH_METRIC_REFINE
	unsigned char	InterfaceMACAddr[ETH_ALEN];
	unsigned int	RSSI_Metric;
	unsigned int	Cu_Metric;
	unsigned int	Noise_Metric;
#endif
} DOT11s_RECV_PACKET;
#pragma pack()

#endif // defined(_RTL865X_) || defined(_RTL8187B_)

enum {
	HOSTAPD_DEBUG_NO = 0,
	HOSTAPD_DEBUG_MINIMAL = 1,
	HOSTAPD_DEBUG_VERBOSE = 2,
	HOSTAPD_DEBUG_MSGDUMPS = 3
} ;


// context
/*
struct pathsel_context {
	int				driver_ver;
	u16				pathsel_identifier;	// next IAPP identifier
	//int			readfifo;
    u8*				readfifo;
	u8				RecvBuf[MAX_MSG_SIZE];
	char			wlan_iface[2][16];//hunter: wlan_iface[10][16];
	int				wlan_sock[10];
	int				is_daemon;
	int				debug;				// debug verbosity level
};
*/

// 802.11s Path selection table
#pragma pack(1)
struct path_sel_entry {
	unsigned char destMAC[ETH_ALEN];
	unsigned char nexthopMAC[ETH_ALEN];
	unsigned int dsn;
	unsigned int metric;
	unsigned char hopcount;
	// the following entries are NOT shared by daemon and driver
	//	unsigned char isvalid;
	//	unsigned char modify_time[8];
	unsigned int start, end;
	unsigned short RecentSeq[SZ_HASH_IDX2];
	void *priv;
	unsigned long update_time;
	unsigned long routeMaintain;
#ifdef  SUPPORT_TX_MCAST2UNI
	int	ipmc_num;
	struct mesh_ip_mcast_info	ipmc[MAX_IP_MC_ENTRY];
#endif

	u8 rann_snd_addr[ETH_ALEN];
	u32 rann_metric;
	bool is_root;
	bool is_gate;
	u32 sn;
	unsigned long exp_time;
	enum mesh_path_flags flags;
	u32 discovery_timeout;
	unsigned long last_preq_to_root;

};
#pragma pack()


typedef struct _timer_instance_ {
	unsigned int c_count; //current count
	unsigned int p_count; //period count for restoring to current count
	void (*func)(void);
} ti_t;


#pragma pack(1)
struct relay_node {
	//#JZDBG
	unsigned char Reserved: 6; //Endian

	unsigned char DO: 1;
	unsigned char RF: 1;
	//unsigned char Reserved:6;
	unsigned char DesAddress[ETH_ALEN];
	unsigned int  DesSeqNum;
//	struct relay_node* next;
//	struct relay_node* before;
};
#pragma pack()

#pragma pack(1)
struct rreq {
	unsigned char Category;
	unsigned char Action;
	unsigned char Id;
	unsigned char Length;
	unsigned char ModeFlags;
	unsigned char HopCount;
	unsigned char ttl;
	unsigned int  RreqId;
	unsigned char SourceAddress[ETH_ALEN];
	unsigned int  SourceSeqNum;
	unsigned int  Lifetime;
	unsigned int  Metric;
	struct relay_node first;
//	struct rrep_relay_node* next;
//	struct rrep_relay_node* before;
};
#pragma pack()

#pragma pack(1)
struct rrep_relay_node {
	unsigned char SourceAddress[ETH_ALEN];
	unsigned int  SourceSeqNum;
	unsigned char DepCount;
	unsigned char DepMacAddress[ETH_ALEN];
	unsigned int  DepDesSeqNum;
	//struct rrep_relay_node* next;
	//struct rrep_relay_node* before;
};
#pragma pack()

#pragma pack(1)
struct rrep {
	unsigned char Category;
	unsigned char Action;
	unsigned char Id;
	unsigned char Length;
	unsigned char ModeFlags;
	unsigned char HopCount;
	unsigned char ttl;
	unsigned char DesAddress[ETH_ALEN];
	unsigned int  DesSeqNum;
	unsigned int  LifeTime;
	unsigned int  Metric;
	struct rrep_relay_node first;
	//struct rrep_relay_node* next;
	//struct rrep_relay_node* before;
};
#pragma pack()

#pragma pack(1)
struct rerr {
	unsigned char Category;
	unsigned char Action;
	unsigned char Id;
	unsigned char Length;
	unsigned char ModeFlags;
	unsigned int  NumOfDes;     // 4 bytes format
	unsigned char DesAddress[ETH_ALEN];
	unsigned int  DesSeqNum;
};
#pragma pack()

#pragma pack(1)
struct rrep_ack {
	unsigned char Category;
	unsigned char Action;
	unsigned char Id;
	unsigned char Length;
	unsigned char DesAddress[ETH_ALEN];
	unsigned int  DesSeqNum;
	unsigned char SourceAddress[ETH_ALEN];
	unsigned int  SourceSeqNum;
};
#pragma pack()

#pragma pack(1)
struct preq {
	u8  category;
	u8  action;
	u8  element_id;
	u8  length;//37~252
	u8  flags;//GA|AddressMode|PREP|RESERVED|AE|RESERVED
	u8  hop_count;
	u8  ttl;
	u32 preq_id;
	//u8   orig_addr[MACADDRLEN];
	u32 orig_sn;
	u8  da[ETH_ALEN];//only present if the AE subfield in the Flags field is set to 1
	u32 lifetime;//TUs
	u32 metric;
	u8  target_flags;//maximun 20
	u8 	target[ETH_ALEN];
	u32 target_sn;//latest

	//I know its N targets but open11s  is like this
	//struct preq_relay_node first;
//	struct rrep_relay_node* next;
//	struct rrep_relay_node* before;
};
#pragma pack()


#pragma pack(1)
struct prep {
	u8  category;
	u8  action;
	u8  element_id;
	u8  length;//31 or 37
	u8  flags;//|reserved 6 bits|AE|reserved
	u8  hop_count;
	u8  ttl;
	u8 	target[ETH_ALEN];
	u32 target_sn;
	u8 	da[ETH_ALEN];//0 or 6
	u32 lifetime;
	u32 metric;
	u8 	orig_addr[ETH_ALEN];
	u32 orig_sn;
	//struct rrep_relay_node first;
	//struct rrep_relay_node* next;
	//struct rrep_relay_node* before;
};
#pragma pack()


#pragma pack(1)
typedef struct _DOT11s_HWMP_PACKET {
	u8 	action;
	u8 	is_more_event;
	u8 	flags;
	u8	orig_addr[ETH_ALEN];
	u32 orig_sn;
	u8  target_flags;
	u8  target[ETH_ALEN];
	u32 target_sn;
	//unsigned char	PreHopMACAddr[MACADDRLEN];
	u8	da[ETH_ALEN];
	u8  hop_count;
	//unsigned int	Pre_Hop_Metric;
	//unsigned char	ReceiveData[128];
	u8	ttl;
	u32 lifetime;
	u32 metric;
	u32 preq_id;
	u8	Is6AddrFormat;
	u8	MACAddr5[ETH_ALEN];
	u8	MACAddr6[ETH_ALEN];
} DOT11s_HWMP_PACKET;
#pragma pack()


#pragma pack(1)
typedef struct _DOT11s_GEN_PREQ_PACKET {
	u8	EventId;
	u8  IsMoreEvent;
	u8  flag;
	u8	orig_addr[ETH_ALEN];
	u8	destMACAddr[ETH_ALEN];
	u8	TTL;
	u32 pathDiscoveryId;
	u32 Lifetime;//TUs
	u32 Metric;
	u32 Seq_num;
} DOT11s_GEN_PREQ_PACKET;
#pragma pack()


// debug printing
#define HOSTAPD_DEBUG(level, args...) \
do { \
	if (hapd->debug >= (level)) \
		printf(args); \
} while (0)

// not use #define HWMP_NET_DIAMETER 20 // rreq TTL, chuangch 0928


//#define DUMP_NBRTABLE  0x8B84						// dump neighbor table
//#define UPDATE_PROXY_TABLE 0x8B87


#define WLAN_HDR_A4_MESH_MGT_LEN 34		// mesh mgt frame header length
#define WLAN_HDR_A6_MESH_MGT_LEN 46		// mesh mgt frame header length
//#define PATH_SEL_TBL_SIZE 128			// path selection table size, defined in wireless_ag/8185s_pathsel.h

#ifndef _RTL865X_
#define NUM_STAT 64						// neighbor table size, defined in wireless_ag/rtk8185ag_cfg.h
#endif


#pragma pack(1)
struct pann {
	unsigned char category;
	unsigned char action;
	unsigned char eid;
	unsigned char length;
	unsigned char flag;
	unsigned char hopCount;
	unsigned char ttl;
	unsigned char origAddr[ETH_ALEN];
	unsigned int  seqNum;
	unsigned int  metric;
};
#pragma pack()

typedef struct _pann_retx_instance_ {
	struct pann  pann;
	unsigned int timeout;
	short int    flag;
} pann_retx_t;


typedef struct _pann_mpp_entry_ {
	unsigned char mac[ETH_ALEN];
	unsigned int timeout;
	unsigned int seqNum;
	short int    flag;
	int          beRoot;
} pann_mpp_t;


#ifdef CONFIG_RTK_MESH_METRIC_REFINE
#define IS_5G_BAND 2
//configurable mib parameters
#pragma pack(1)
struct mibValue {
	unsigned char rssi_threshold;
	unsigned char cu_threshold;
	unsigned char noise_threshold;
	unsigned char rssi_weight;
	unsigned char cu_weight;
	unsigned char noise_weight;
};
#pragma pack()
#endif

//#define ETH_ALEN 6
//#define MAX_MP_NUM 5
//#define MAX_CONNECTION 100
#define MAX_TIMER_INSTANCE 7

#define PORTAL_ANNOUNCEMENT_INTERVAL 3
#define PORTAL_ANNOUNCEMENT_TIMEOUT 17 // must be larger than PORTAL_ANNOUNCEMENT_INTERVAL
#define PORTAL_PROPAGATION_DELAY 1

#define PORTAL_ARBITRATION_TIME 20

#define ROOT_ANNOUNCEMENT_INTERVAL 1
#define ROOT_ANNOUNCEMENT_TIMEOUT 17 // must be larger than ROOT_ANNOUNCEMENT_INTERVAL
#define ROOT_PROPAGATION_DELAY 1
#define HWMP_ROUTE_LOSS_THRESHOLD 5


#pragma pack(1)
struct rann {
	unsigned char   category;
	unsigned char   action;
	unsigned char   Id;
	unsigned char   length;
	unsigned char   flag;
	unsigned char   hopCount;
	unsigned char   ttl;
	unsigned char   origAddr[ETH_ALEN];
	unsigned int    seqNum;
	unsigned int    lifetime;
	unsigned int    metric;
};
#pragma pack()

struct _rann_retx_entry_ { // RANN retransmit table, record what time shoude RANN be relayed.
	struct rann     rann;
	unsigned int    timeout;
	short int       flag;
};

struct _rann_root_entry_ { // root info table.
	unsigned char   mac[ETH_ALEN];
	unsigned int    timeout;
	unsigned int    seqNum;
	short int       flag;
};

int init_pann(void);

void gen_pann(void);

void tbl_pann(void);

void retx_pann(void);

void gen_rann(void);

void retx_rann(void);

void tbl_rann(void);

int init_root(void);

void root_decision(void);

void pann_handler();

int updatePathByFreshness(unsigned char *src,
						  unsigned int  dsn,
						  unsigned int  metric,
						  unsigned char hopcount,
						  unsigned char *My_Prehop,
						  int           My_interFace);

int query_table(unsigned char           *destaddr,
				struct path_sel_entry   *pEntry_out);


int query_whole_table(int *ptable_no_out);

int remove_table(unsigned char *invalid_addr);


static void send_packet_with_6addr(unsigned char    *RBody,
								   unsigned char    *self_mac_addr,
								   unsigned char    *dest_mac_addr,
								   unsigned char    *src_mac_addr,
								   int              flag,
								   unsigned char    TTL,
								   unsigned short   SeqNum,
								   unsigned char    *addr5,
								   unsigned char    *addr6);

static void send_packet(unsigned char   *RBody,
						unsigned char   *self_mac_addr,
						unsigned char   *dest_mac_addr,
						unsigned char   *src_mac_addr,
						int             flag,
						unsigned char   TTL,
						unsigned short  SeqNum);

static int send_rawsocket(char  *meshframe,
						  int   len,
						  char  *src_mac_addr);

#ifdef CONFIG_RTK_MESH_METRIC_REFINE
int calculateMetric(unsigned char   *My_Add,
					unsigned int    metric,
					unsigned int    rssiMetric,
					unsigned int    cuMetric,
					unsigned int    noiseMetric);
int updateMetric(unsigned char  *My_Add,
				 unsigned char  *My_Prehop,
				 unsigned int   *metric);
#endif //CONFIG_RTK_MESH_METRIC_REFINE

int gen_rreq(unsigned char  *DesAdd,
			 unsigned char  TTL,
			 unsigned short SeqNum);

int gen_rerr(unsigned char  *DesAddr,
			 unsigned char  *SrcAddr,
			 unsigned char  TTL,
			 unsigned short SeqNum,
			 unsigned char  RERR_flag);

#ifdef CONFIG_RTK_MESH_METRIC_REFINE
int recv_rreq(struct rreq       *input,
			  unsigned char     *My_Prehop,
			  unsigned char     *My_Add,
			  unsigned int      My_Prehop_metric,
			  unsigned int      rssiMetric,
			  unsigned int      cuMetric,
			  unsigned int      noiseMetric,
			  int               My_interFace,
			  unsigned char     TTL,
			  unsigned short    SeqNum);
int recv_rrep(struct rrep       *input_rrep,
			  unsigned char      *My_Prehop,
			  unsigned char      *My_Add,
			  unsigned int      My_Prehop_metric,
			  unsigned int      rssiMetric,
			  unsigned int      cuMetric,
			  unsigned int      noiseMetric,
			  int               My_interFace,
			  unsigned char     TTL,
			  unsigned short    SeqNum,
			  unsigned char     Is6AddrFormat,
			  unsigned char     *addr5,
			  unsigned char     *addr6);
#else
int recv_rreq(struct rreq       *input,
			  unsigned char     *My_Prehop,
			  unsigned int      My_Prehop_metric,
			  int               My_interFace,
			  unsigned char     TTL,
			  unsigned short    SeqNum);
int recv_rrep(struct rrep       *input_rrep,
			  unsigned char     *My_Prehop,
			  unsigned int      My_Prehop_metric,
			  int               My_interFace,
			  unsigned char     TTL,
			  unsigned short    SeqNum,
			  unsigned char     Is6AddrFormat,
			  unsigned char     *addr5,
			  unsigned char     *addr6);
#endif //CONFIG_RTK_MESH_METRIC_REFINE

int recv_rerr(struct rerr       *input_rerr,
			  unsigned char     *PreHopAdd,
			  unsigned char     TTL,
			  unsigned short    SeqNum,
			  unsigned char     *DesAdd);

//int recv_rrep_ack(struct rrep_ack *input_rrep_ack, unsigned char* My_Address,unsigned char TTL,unsigned short SeqNum);
// int update_proxy_table(unsigned char *STAaddr, unsigned char *OWNERaddr);

void reset_timer();

int recv_rann(struct rann   *rann_ptr,
			  unsigned char *My_Prehop,
			  unsigned int  metric,
			  int           My_interFace,
			  int           TTL,
			  int           SeqNum);

#ifdef CONFIG_RTK_MESH_METRIC_REFINE
int recv_pann(struct pann   *pann_ptr,
			  unsigned char *PreHopAdd,
			  unsigned char *MyAdd,
			  unsigned int  PreMetric,
			  unsigned int  RssiMetric,
			  unsigned int  CuMetric,
			  unsigned int  NoiseMetric,
			  int           My_interFace,
			  int           TTL,
			  int           SeqNum);
#else
int recv_pann(struct pann   *pann_ptr,
			  unsigned char *PreHopAdd,
			  unsigned int  PreMetric,
			  int           My_interFace,
			  int           TTL,
			  int           SeqNum);
#endif //CONFIG_RTK_MESH_METRIC_REFINE

#ifdef CONFIG_RTK_MESH_METRIC_REFINE
#define betterPath(xmetric, xhop, ymetric, yhop) ( (xmetric <= ymetric)?((((ymetric - xmetric) * 100) / ymetric) >= pathSwitchThreshold):(xhop < yhop) )
#else
// betterPath: return better  when x.metric < y.metric or (x.metric = y.metric but x.hopcount < y.hopcount)
#define betterPath(xmetric, xhop, ymetric, yhop) ( ((xmetric + pathSwitchThreshold) < ymetric) || ( (xmetric <= ymetric) && (xhop < yhop) ) )
#endif


int gen_preq(DOT11s_HWMP_PACKET *hwmp_packet);
int recv_preq(DOT11s_HWMP_PACKET *hwmp_packet, int My_interFace);
void send_mesh_packet(u8 *RBody, u8 *self_mac_addr, u8 *dest_mac_addr, u8 *src_mac_addr, int flag, u8 TTL, u32 SeqNum);

int GetMeshPortStat(void);

int SendPidToBridge(void);

void TimerFunc(void);

int hwmp_init(void);

void hwmp_hdl(unsigned char *buf, int buf_len, int flags, void *userdata);

#define INIT_TIMER_INSTANCE() {{PORTAL_ANNOUNCEMENT_INTERVAL, PORTAL_ANNOUNCEMENT_INTERVAL, gen_pann},  \
                               {1, 1, retx_pann},														\
                               {1, 1, tbl_pann},														\
                               {ROOT_ANNOUNCEMENT_INTERVAL, ROOT_ANNOUNCEMENT_INTERVAL, gen_rann},      \
                               {1, 1, retx_rann},														\
                               {1, 1, tbl_rann},														\
                               {PORTAL_ARBITRATION_TIME, PORTAL_ARBITRATION_TIME, root_decision}}


#endif