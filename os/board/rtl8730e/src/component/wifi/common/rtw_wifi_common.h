
#ifndef _RTW_WIFI_COMMON_H
#define _RTW_WIFI_COMMON_H
#include "basic_types.h"
#include "rtw_autoconf.h"

#define TAG_WLAN_DRV   "WLAN"
#define TAG_WLAN_COEX  "COEX"
#define TAG_WLAN_INIC  "INIC"
#define TAG_WLAN_NAN   "NAN"
#define TAG_WLAN_SME   "SME"


/*
 *	IEEE 802.3 Ethernet magic constants.  The frame sizes omit the preamble
 *	and FCS/CRC (frame check sequence).
 */

#define ETH_ALEN	6		/* Octets in one ethernet addr	 */
#define ETH_HLEN	14		/* Total octets in header.	 */
#define ETH_DATA_LEN	1500		/* Max. octets in payload	 */
#define ETH_TYPE_LEN		2
#define ETH_ARPHRD		1	/* ethernet hardware format */

#define MINIMUM_ETHERNET_PACKET_SIZE		60		//!< Minimum Ethernet Packet Size
#ifdef NAN_CUSTOMER_NANDOW
#define MAXIMUM_ETHERNET_PACKET_SIZE		MAX_NANDOW_PARA_LEN	//!< Maximum Ethernet Packet Size
#else
#define MAXIMUM_ETHERNET_PACKET_SIZE		1514	//!< Maximum Ethernet Packet Size
#endif

#define WLAN_HDR_A4_QOS_HTC_LEN	36
#define WLAN_MAX_IV_LEN	8
#define WLAN_SNAP_HEADER	8
#define WLAN_MAX_MIC_LEN	8
#define WLAN_MAX_ICV_LEN	8
#define WLAN_MAX_PROTOCOL_OVERHEAD (WLAN_HDR_A4_QOS_HTC_LEN+WLAN_MAX_IV_LEN+\
							WLAN_SNAP_HEADER+WLAN_MAX_MIC_LEN+WLAN_MAX_ICV_LEN)/*=68*/

#define RSNXE_MAX_LEN (18)/*Draft P802.11REVmd_D5.0 p1464*/

#define INFO_ELEMENT_SIZE       128 /*TODO: rom should check because moved from rom_rtw_defs.h*/

enum _IFACE_TYPE {
	IFACE_PORT0, //mapping to port0 for C/D series chips
	IFACE_PORT1, //mapping to port1 for C/D series chip
	IFACE_PORT2,
	MAX_IFACE_PORT,
};

/**
  * The enumeration lists the flash operation status.
  */
enum flash_operation_type {
	FLASH_READ = 0,   /**< read  flash                       */
	FLASH_WRITE,       /**< write  flash                       */
};

struct eth_drv_sg {
	unsigned int    buf;
	unsigned int     len;
};

/*
 *	This is an Ethernet frame header.
 */
RTW_PACK_STRUCT_BEGIN
struct ethhdr {
	unsigned char	h_dest[ETH_ALEN];	/* destination eth addr	*/
	unsigned char	h_source[ETH_ALEN];	/* source ether addr	*/
	unsigned short	h_proto;		/* packet type ID field	*/
} RTW_PACK_STRUCT_STRUCT;
RTW_PACK_STRUCT_END

#endif
