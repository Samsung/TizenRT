/*****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef __UNIFIIO_H__
#define __UNIFIIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "utils_scsc.h"

#define GET_UDI_ENABLE    _IOR('u',  1, int)
#define SET_UDI_ENABLE    _IOW('u',  2, int)
/* Values for UDI_ENABLE */
#define UDI_ENABLE_DATA         0x1
#define UDI_ENABLE_CONTROL      0x2

#define SIOCIWFIRSTPRIV     (0x8BE0)

#define SLSI_IOC_NRBITS     (8)
#define SLSI_IOC_TYPEBITS   (8)
#define SLSI_IOC_SIZEBITS   (14)
#define SLSI_IOC_DIRBITS    (2)

#define SLSI_IOC_NRMASK     ((1 << SLSI_IOC_NRBITS) - 1)
#define SLSI_IOC_TYPEMASK   ((1 << SLSI_IOC_TYPEBITS) - 1)
#define SLSI_IOC_SIZEMASK   ((1 << SLSI_IOC_SIZEBITS) - 1)
#define SLSI_IOC_DIRMASK    ((1 << SLSI_IOC_DIRBITS) - 1)

#define SLSI_IOC_NRSHIFT    (0)
#define SLSI_IOC_TYPESHIFT  (SLSI_IOC_NRSHIFT + SLSI_IOC_NRBITS)
#define SLSI_IOC_SIZESHIFT  (SLSI_IOC_TYPESHIFT + SLSI_IOC_TYPEBITS)
#define SLSI_IOC_DIRSHIFT   (SLSI_IOC_SIZESHIFT + SLSI_IOC_SIZEBITS)

#define SLSI_IOC_NONE       (0U)
#define SLSI_IOC_WRITE      (1U)
#define SLSI_IOC_READ       (2U)

#define SLSI_IOC(dir, type, nr, size) \
	(((dir) << SLSI_IOC_DIRSHIFT) | \
	 ((type) << SLSI_IOC_TYPESHIFT) | \
	 ((nr) << SLSI_IOC_NRSHIFT) | \
	 ((size) << SLSI_IOC_SIZESHIFT))

#define SLSI_IOC_TYPECHECK(t) (sizeof(t))

//#define _IO(type,nr)            SLSI_IOC(SLSI_IOC_NONE,(type),(nr),0)
//#define _IOR(type,nr,size)      SLSI_IOC(SLSI_IOC_READ,(type),(nr),(SLSI_IOC_TYPECHECK(size)))
//#define _IOW(type,nr,size)      SLSI_IOC(SLSI_IOC_WRITE,(type),(nr),(SLSI_IOC_TYPECHECK(size)))
#define _IOWR(type, nr, size)     SLSI_IOC(SLSI_IOC_READ | SLSI_IOC_WRITE, (type), (nr), (SLSI_IOC_TYPECHECK(size)))

#define SIOCIWFIRSTPRIV     (0x8BE0)

#define SLSI_IOC_NRBITS     (8)
#define SLSI_IOC_TYPEBITS   (8)
#define SLSI_IOC_SIZEBITS   (14)
#define SLSI_IOC_DIRBITS    (2)

#define SLSI_IOC_NRMASK     ((1 << SLSI_IOC_NRBITS) - 1)
#define SLSI_IOC_TYPEMASK   ((1 << SLSI_IOC_TYPEBITS) - 1)
#define SLSI_IOC_SIZEMASK   ((1 << SLSI_IOC_SIZEBITS) - 1)
#define SLSI_IOC_DIRMASK    ((1 << SLSI_IOC_DIRBITS) - 1)

#define SLSI_IOC_NRSHIFT    (0)
#define SLSI_IOC_TYPESHIFT  (SLSI_IOC_NRSHIFT + SLSI_IOC_NRBITS)
#define SLSI_IOC_SIZESHIFT  (SLSI_IOC_TYPESHIFT + SLSI_IOC_TYPEBITS)
#define SLSI_IOC_DIRSHIFT   (SLSI_IOC_SIZESHIFT + SLSI_IOC_SIZEBITS)

#define SLSI_IOC_NONE       (0U)
#define SLSI_IOC_WRITE      (1U)
#define SLSI_IOC_READ       (2U)

#define SLSI_IOC(dir, type, nr, size) \
	(((dir) << SLSI_IOC_DIRSHIFT) | \
	 ((type) << SLSI_IOC_TYPESHIFT) | \
	 ((nr) << SLSI_IOC_NRSHIFT) | \
	 ((size) << SLSI_IOC_SIZESHIFT))

#define SLSI_IOC_TYPECHECK(t) (sizeof(t))

/* Private configuration commands */
#define UNIFI_CFG               _IOWR('u',  5, unsigned char[4])
/**
 * <------------------  Read/Write Buffer  -------------------->
 * _____________________________________________________________
 * |    Cmd    |    Arg    |   ...  Buffer (opt)  ...          |
 * -------------------------------------------------------------
 * <-- uint --><-- uint --><-----  unsigned char buffer  ------>
 *
 * Cmd:    A enum unifi_cfg_command command.
 * Arg:    Out:Length     if Cmd==UNIFI_CFG_GET
 *         In:PowerOnOff  if Cmd==UNIFI_CFG_POWER
 *         In:PowerMode   if Cmd==UNIFI_CFG_POWERSAVE
 *         In:Length      if Cmd==UNIFI_CFG_FILTER
 *         In:WMM Qos Info if Cmd==UNIFI_CFG_WMM_QOS_INFO
 * Buffer: Out:Data       if Cmd==UNIFI_CFG_GET
 *         NULL           if Cmd==UNIFI_CFG_POWER
 *         NULL           if Cmd==UNIFI_CFG_POWERSAVE
 *         In:Filters     if Cmd==UNIFI_CFG_FILTER
 *
 * where Filters is a struct uf_cfg_bcast_packet_filter structure
 * followed by 0 - n struct tclas structures. The length of the struct tclas
 * structures is obtained by struct uf_cfg_bcast_packet_filter::tclas_ies_length.
 */

/* MIB set/get. Arg is a pointer to a varbind */
#define UNIFI_GET_MIB           _IOWR('u',  3, unsigned char)
#define UNIFI_SET_MIB           _IOW('u',  4, unsigned char)
#define MAX_MIB_DATA_LENGTH 2048

#define UNIFI_BUILD_TYPE _IOWR('u', 7, unsigned char)

#define UNIFI_BUILD_NME 1
#define UNIFI_BUILD_WEXT 2
#define UNIFI_BUILD_AP 3

#define UNIFI_GET_UDI_ENABLE    _IOR('u',  1, int)
#define UNIFI_SET_UDI_ENABLE    _IOW('u',  2, int)
#define IFNAMSIZ           6	/* Older naming standard */

/* debugging */
#define UNIFI_SET_DEBUG         _IO('u',  0x11)
#define UNIFI_SET_TRACE         _IO('u',  0x12)

#define UNIFI_GET_INIT_STATUS   _IOR('u', 0x15, int)
#define UNIFI_SET_UDI_LOG_MASK  _IOR('u',  0x18, struct unifiio_filter_t)

#define UNIFI_INIT_HW           _IOR('u', 0x13, unsigned char)
#define UNIFI_INIT_NETDEV       _IOW('u', 0x14, unsigned char[6])
#define UNIFI_SME_PRESENT       _IOW('u', 0x19, int)

#define UNIFI_CFG_PERIOD_TRAFFIC _IOW('u', 0x21, unsigned char[4])
#define UNIFI_CFG_UAPSD_TRAFFIC _IOW('u', 0x22, unsigned char)

#define UNIFI_SOFTMAC_CFG   _IOWR('u',  0x30, unsigned char[4])

#define UNIFI_NETDEV_CFG   _IOWR('u',  0x31, struct unifiio_netdev_cfg_t)
#define UNIFI_STREAM_CFG   _IOWR('u',  0x32, struct unifiio_stream_cfg_t)
#define UNIFI_AMSDU_CFG    _IOWR('u',  0x33, struct unifiio_amsdu_cfg_t)
#define UNIFI_BA_CFG       _IOWR('u',  0x34, struct unifiio_ba_cfg_t)
#define UNIFI_SUSPEND_RESUME_CFG       _IOWR('u',  0x35, struct unifiio_suspend_resume_cfg_t)
#define UNIFI_SRC_SINK_IOCTL        _IOWR('u',  0x41, struct unifiio_src_sink_arg_t)

#define src_sink_action uint16_t
#define SRC_SINK_ACTION_NONE               ((src_sink_action)0x0000)
#define SRC_SINK_ACTION_ENABLE_BA          ((src_sink_action)0x0001)
#define SRC_SINK_ACTION_SINK_START         ((src_sink_action)0x0002)
#define SRC_SINK_ACTION_SINK_STOP          ((src_sink_action)0x0003)
#define SRC_SINK_ACTION_GEN_START          ((src_sink_action)0x0004)
#define SRC_SINK_ACTION_GEN_STOP           ((src_sink_action)0x0005)
#define SRC_SINK_ACTION_SINK_REPORT        ((src_sink_action)0x0006)
#define SRC_SINK_ACTION_GEN_REPORT         ((src_sink_action)0x0007)
#define SRC_SINK_ACTION_SINK_REPORT_CACHED ((src_sink_action)0x0008)
#define SRC_SINK_ACTION_GEN_REPORT_CACHED  ((src_sink_action)0x0009)

#define src_sink_direction uint16_t
#define SRC_SINK_DIRECTION_TX             ((src_sink_direction)0x0000)
#define SRC_SINK_DIRECTION_RX             ((src_sink_direction)0x0001)

#define src_sink_endpoint uint16_t
#define SRC_SINK_ENDPOINT_HOSTIO          ((src_sink_endpoint)0x0000)
#define SRC_SINK_ENDPOINT_MACRAME         ((src_sink_endpoint)0x0001)

struct unifiio_src_sink_report {
	/* total reporting time requested by the user in seconds */
	uint32_t time;
	/* requested report interval in microseconds */
	uint32_t interval;

	/* timestamp [jiffies in millisec] for each report returned by the driver */
	uint32_t timestamp;
	/* actual interval used for the report calculations from the firmware */
	uint32_t duration;
	/* number of packets during the above duration */
	uint32_t count;
	/* number of octets during the above duration */
	uint32_t octet;
	/* throughput in kbps */
	uint32_t kbps;
	/* CPU idle ratios */
	uint16_t idle_ratio;
	uint16_t interrupt_latency;
	uint16_t free_kbytes;
	/* only relevant for SRC a.k.a GEN mode */
	uint32_t failed_count;
};

/* fields specific to SRC a.k.a GEN <CONFIG> IOCTLs */
struct unifiio_src_sink_gen_config {
	uint16_t size;
	uint16_t use_streaming;
	uint16_t pkts_per_intr;
	uint32_t ipv4_dest;
};

/* fields specific to SRC/SINK CONFIG IOCTLs */
struct unifiio_src_sink_config {
	uint32_t interval;
	uint16_t pkts_per_int;
	union {
		struct unifiio_src_sink_gen_config gen;
	} u;
};

/* fields common to <ALL> SRC/SINK IOCTLs */
struct unifiio_src_sink_common {
	/* one of SRC_SINK_ACTION_* for driver IOCTL */
	src_sink_action action;
	uint16_t vif;
	src_sink_direction direction;
	src_sink_endpoint endpoint;
};

struct unifiio_src_sink_arg_t {
	/* arg in  */
	struct unifiio_src_sink_common common;
	union {
		struct unifiio_src_sink_config config;
		struct unifiio_src_sink_report report;
	} u;
};

/* Structure of data read from the unifi device. */
struct udi_msg_t {
	/* Length (in bytes) of entire structure including appended bulk data */
	int length;

	/* System time (in milliseconds) that signal was transferred */
	int timestamp;

	/* Direction in which signal was transferred. */
	int direction;
#define UDI_FROM_HOST   0
#define UDI_TO_HOST     1
#define UDI_CONFIG_IND  2

	/* The length of the signal (in bytes) not including bulk data */
	int signal_length;

	/* Signal body follows, then any bulk data */
};

/* Add these to the filter signal_ids to enable partial filtering of the MA_UNITDATA Data
 * the top bit is set to indicate this is not a HIP Signal ID but a special Filter
 */
#define UDI_MA_UNITDATA_FILTER_ALLOW_MASK         0x8000	/* Filter MA_UNITDATA_REQ and MA_UNITDATA_IND */
#define UDI_MA_UNITDATA_FILTER_ALLOW_MGT_ID       0x8001	/* Filter MA_UNITDATA_REQ and MA_UNITDATA_IND but Log management Frames */
#define UDI_MA_UNITDATA_FILTER_ALLOW_KEY_ID       0x8002	/* Filter MA_UNITDATA_REQ and MA_UNITDATA_IND but Log Key Data Frames  (Arp, Eapol, Dhcp etc) */
#define UDI_MA_UNITDATA_FILTER_ALLOW_CFM_ERROR_ID 0x8004	/* Filter MA_UNITDATA_CFM but MA_UNITDATA_CFM(error) */
#define UDI_MA_UNITDATA_FILTER_ALLOW_EAPOL_ID     0x8008	/* Filter MA_UNITDATA_REQ and MA_UNITDATA_IND but Log Eapol data */

/**
 * Signals used to indicate to user space that the kernel module has been
 * unloaded. The user space applications can based on this indication determine
 * if they should unregister from the char device.
 */
#define UDI_DRV_UNLOAD_IND           0x8010
#define UDI_DRV_DROPPED_FRAMES       0x8011
#define UDI_DRV_DROPPED_DATA_FRAMES  0x8012
#define UDI_DRV_SUSPEND_IND          0x8013
#define UDI_DRV_RESUME_IND           0x8014

struct unifiio_filter_t {
	uint16_t log_listed_flag;	/* if non-zero, log listed sigs and ignore others (otherwise vice versa) */
	uint16_t signal_ids_n;	/* Number of elements in signal_ids[] */
	uint16_t signal_ids[5];	/* list of the signals to log */
};

#define unifi_cfg_command uint32_t
#define UNIFI_CFG_GET             ((unifi_cfg_command)0)
#define UNIFI_CFG_POWER           ((unifi_cfg_command)1)
#define UNIFI_CFG_POWERSAVE       ((unifi_cfg_command)2)
#define UNIFI_CFG_FILTER          ((unifi_cfg_command)3)
#define UNIFI_CFG_POWERSUPPLY     ((unifi_cfg_command)4)
#define UNIFI_CFG_WMM_QOSINFO     ((unifi_cfg_command)5)
#define UNIFI_CFG_WMM_ADDTS       ((unifi_cfg_command)6)
#define UNIFI_CFG_WMM_DELTS       ((unifi_cfg_command)7)
#define UNIFI_CFG_STRICT_DRAFT_N  ((unifi_cfg_command)8)
#define UNIFI_CFG_SET_AP_CONFIG   ((unifi_cfg_command)9)
#define UNIFI_CFG_CORE_DUMP       ((unifi_cfg_command)10)

#define unifi_cfg_power uint32_t
#define UNIFI_CFG_POWER_UNSPECIFIED  ((unifi_cfg_power)0)
#define UNIFI_CFG_POWER_OFF          ((unifi_cfg_power)1)
#define UNIFI_CFG_POWER_ON           ((unifi_cfg_power)2)

#define unifi_cfg_powersupply uint32_t
#define UNIFI_CFG_POWERSUPPLY_UNSPECIFIED ((unifi_cfg_powersupply)0)
#define UNIFI_CFG_POWERSUPPLY_MAINS       ((unifi_cfg_powersupply)1)
#define UNIFI_CFG_POWERSUPPLY_BATTERIES   ((unifi_cfg_powersupply)2)

#define unifi_cfg_powersave uint32_t
#define UNIFI_CFG_POWERSAVE_UNSPECIFIED  ((unifi_cfg_powersave)0)
#define UNIFI_CFG_POWERSAVE_NONE         ((unifi_cfg_powersave)1)
#define UNIFI_CFG_POWERSAVE_FAST         ((unifi_cfg_powersave)2)
#define UNIFI_CFG_POWERSAVE_FULL         ((unifi_cfg_powersave)3)
#define UNIFI_CFG_POWERSAVE_AUTO         ((unifi_cfg_powersave)4)

#define unifi_cfg_get uint32_t
#define UNIFI_CFG_GET_COEX          ((unifi_cfg_get)0)
#define UNIFI_CFG_GET_POWER_MODE    ((unifi_cfg_get)1)
#define UNIFI_CFG_GET_VERSIONS      ((unifi_cfg_get)2)
#define UNIFI_CFG_GET_POWER_SUPPLY  ((unifi_cfg_get)3)
#define UNIFI_CFG_GET_INSTANCE      ((unifi_cfg_get)4)
#define UNIFI_CFG_GET_AP_CONFIG     ((unifi_cfg_get)5)

#define UNIFI_CFG_FILTER_NONE            0x0000
#define UNIFI_CFG_FILTER_DHCP            0x0001
#define UNIFI_CFG_FILTER_ARP             0x0002
#define UNIFI_CFG_FILTER_NBNS            0x0004
#define UNIFI_CFG_FILTER_NBDS            0x0008
#define UNIFI_CFG_FILTER_CUPS            0x0010
#define UNIFI_CFG_FILTER_ALL             0xFFFF

#define uf_cfg_packet_filter_type uint32_t
#define UNIFI_CFG_FILTER_TYPE_NONE             ((uf_cfg_packet_filter_type)0)
#define UNIFI_CFG_FILTER_TYPE_ACTIVE_HOST      ((uf_cfg_packet_filter_type)1)
#define UNIFI_CFG_FILTER_TYPE_SUSPENDED_HOST   ((uf_cfg_packet_filter_type)2)

struct uf_cfg_bcast_packet_filter {
	unsigned long filter_mode;	/* as defined by HIP protocol */
	uf_cfg_packet_filter_type packet_filter_type;
	unsigned char arp_filter;
	unsigned char dhcp_filter;
	unsigned long tclas_ies_length;	/* length of tclas_ies in bytes */
	unsigned char tclas_ies[1];	/* variable length depending on above field */
};

struct uf_cfg_ap_config {
	uint8_t phySupportedBitmap;
	uint8_t channel;
	uint16_t beaconInterval;
	uint8_t dtimPeriod;
	bool wmmEnabled;
	uint8_t shortSlotTimeEnabled;
	uint16_t groupkeyTimeout;
	bool strictGtkRekeyEnabled;
	uint16_t gmkTimeout;
	uint16_t responseTimeout;
	uint8_t retransLimit;
	uint8_t rxStbc;
	bool rifsModeAllowed;
	uint8_t dualCtsProtection;
	uint8_t ctsProtectionType;
	uint16_t maxListenInterval;
};

struct csr_wifi_cfg_coex_info {
	bool hasTrafficData;
	uint8_t currentTrafficType;
	uint16_t currentPeriodMs;
	uint8_t currentPowerSave;
	uint16_t currentCoexPeriodMs;
	uint16_t currentCoexLatencyMs;
	bool hasBtDevice;
	uint32_t currentBlackOutDurationUs;
	uint32_t currentBlackOutPeriodUs;
	uint8_t currentCoexScheme;
};

struct tcpip_clsfr {
	__u8 cls_fr_type;
	__u8 cls_fr_mask;
	__u8 version;
	__u8 source_ip_addr[4];
	__u8 dest_ip_addr[4];
	__u16 source_port;
	__u16 dest_port;
	__u8 dscp;
	__u8 protocol;
	__u8 reserved;
} STRUCT_PACKED;
#define tcpip_clsfr_t struct tcpip_clsfr

struct tclas {
	__u8 element_id;
	__u8 length;
	__u8 user_priority;
	tcpip_clsfr_t tcp_ip_cls_fr;
} STRUCT_PACKED;

#define CONFIG_IND_ERROR            0x01
#define CONFIG_IND_EXIT             0x02
#define CONFIG_SME_NOT_PRESENT      0x10
#define CONFIG_SME_PRESENT          0x20

/* This is used by the UNIFI_NETDEV_CFG ioctl which is a Multi-netdev replacement for
 * UNIFI_INIT_NETDEV. The all netdev are created statically and can be mapped/registered
 * to a particular vif_index/MAC address/interface mode dynamically using this call.
 *
 * Configure operations (operation == 0):
 *
 * Attempting to operate on a negative interfaceTag is an attempt to
 * create a NEW netdevice.A valid vif_index and macaddr must be supplied.
 * interfaceMode will not be checked. The new interfaceTag and name will be
 * copied back on success.
 *
 * Using an interfaceTag >=0 attempts to reconfigure an existing netdevice. The new
 * vif_index, macaddr and interfaceMode will be applied if all is valid. Setting a
 * vif_index of zero causes the given netdevice to be unregistered instead.
 * The contents of the name[] field will be ignored as input, and the existing
 * interface name copied back as output.
 *
 * Query operations (operation == 1):
 *
 * The first identifying field with a potentially valid value will be used to
 * identify an interface, and if a match is found its values will be copied back
 * into the structure and returned to the user. The fields are checked in this order:
 *
 * interfaceTag     (valid if >= 0)
 * vif_index        (valid if non-zero)
 * name             (valid if first byte non-zero)
 * macaddr          (valid if not broadcast)
 */
struct unifiio_netdev_cfg_t {
	uint8_t operation;		/* 0 = configure, 1 = query */
	/* All other fields potentially bidirectional */
	int interfaceTag;		/* <0 => create new, >=0 => reconfigure existing */
	uint8_t vif_index;		/* VIF index to use (0 => remove netdev) */
	uint8_t interfaceMode;	/* Values defined in csr_wifi_router_ctrl_prim.h */
	unsigned char macaddr[6];	/* MAC address */
	char name[IFNAMSIZ];	/* Interface name */
	unsigned char peer_macaddr[6];	/* peer MAC address */
	uint16_t association_id;	/* Associate ID used for the peer */
};

#define unifiio_stream_cfg_operation_type uint32_t
#define UNIFI_CFG_STREAM_OPERATION_TYPE_NONE       ((unifiio_stream_cfg_operation_type)0)
#define UNIFI_CFG_STREAM_OPERATION_TYPE_CREATE     ((unifiio_stream_cfg_operation_type)1)
#define UNIFI_CFG_STREAM_OPERATION_TYPE_STOP       ((unifiio_stream_cfg_operation_type)2)
#define UNIFI_CFG_STREAM_OPERATION_TYPE_DELETE     ((unifiio_stream_cfg_operation_type)3)

struct unifiio_stream_cfg_t {
	unifiio_stream_cfg_operation_type operation;
	int interfaceTag;		/* A valid interface tag */
	unsigned char peer_macaddr[6];	/* Peer STA MAC address */
	uint16_t tId;			/* Traffic identifier user priority */
	uint16_t handle;		/* Handle as indicated in MA-stream.indication */
	uint16_t size;			/* Size as indicated in MA-stream.indication */
};

#define unifiio_amsdu_cfg_operation_type uint32_t
#define UNIFI_CFG_AMSDU_OPERATION_TYPE_SET_MAX_SIZE   ((unifiio_amsdu_cfg_operation_type)0)

struct unifiio_amsdu_cfg_t {
	unifiio_amsdu_cfg_operation_type operation;
	int interfaceTag;		/* A valid interface tag */
	unsigned char peer_macaddr[6];	/* Peer STA MAC address */
	uint16_t size;			/* Maximum A-MSDU length for peer obtained from HT element */
};

#define unifiio_ba_cfg_operation_type uint32_t
#define UNIFI_CFG_BA_SESSION_STOP    ((unifiio_ba_cfg_operation_type)0)
#define UNIFI_CFG_BA_SESSION_START   ((unifiio_ba_cfg_operation_type)1)

struct unifiio_ba_cfg_t {
	unifiio_ba_cfg_operation_type operation;
	int interfaceTag;		/* A valid interface tag */
	unsigned char peer_macaddr[6];	/* Peer STA MAC address */
	uint16_t tId;			/* Traffic identifier */
	uint8_t role;			/* Role - 0: originator, 1 - recipient */
	uint16_t window_size;	/* Window size as negotiated in BA establish */
	uint16_t ssn;			/* Starting sequence number as negotiated in BA establish */
};

#define unifiio_suspend_resume_cfg_operation_type uint32_t
#define UNIFI_CFG_SUSPEND    ((unifiio_suspend_resume_cfg_operation_type)0)
#define UNIFI_CFG_RESUME   ((unifiio_suspend_resume_cfg_operation_type)1)
#define UNIFI_CFG_WAIT_FOR_RESUME   ((unifiio_suspend_resume_cfg_operation_type)2)

struct unifiio_suspend_resume_cfg_t {
	unifiio_suspend_resume_cfg_operation_type operation;
	unsigned timeout_ms;	/* used with UNIFI_CFG_WAIT_FOR_RESUME only */
};

#ifdef __cplusplus
}
#endif
#endif							/* __UNIFIIO_H__ */
