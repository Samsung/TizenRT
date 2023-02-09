/******************************************************************************
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  *
******************************************************************************/

#ifndef _WIRELESS_H
#define _WIRELESS_H


/***************************** INCLUDES *****************************/

//#include <sockets.h>
#undef IFNAMSIZ
#define IFNAMSIZ	16
#define	ARPHRD_ETHER	1	/* ethernet hardware format */

/***************************** VERSION *****************************/
/*
 * This constant is used to know the availability of the wireless
 * extensions and to know which version of wireless extensions it is
 * (there is some stuff that will be added in the future...)
 * I just plan to increment with each new version.
 */
#define WIRELESS_EXT	22

/**************************** CONSTANTS ****************************/
#include <stdint.h>
typedef uint8_t __u8;
typedef int8_t __s8;
typedef uint16_t __u16;
typedef int16_t __s16;
typedef uint32_t __u32;
typedef int32_t __s32;
typedef	uint64_t __u64;
typedef	int64_t __i64;

#define	E2BIG		 7	/* Argument list too long */

#define ETH_ALEN	6		/* Octets in one ethernet addr	 */


/* ----------------------- OTHER CONSTANTS ----------------------- */


/* Maximum size of the ESSID and NICKN strings */
#define RTW_ESSID_MAX_SIZE	32

/* Modes of operation */
enum RTK_IW_MODE {
	RTW_MODE_AUTO = 0,		/* Let the driver decides */
	RTW_MODE_INFRA,			/* Multi cell network, roaming, ... */
	RTW_MODE_MASTER			/* Synchronisation master or Access Point */
};

/* Flags for encoding (along with the token) */
#define RTW_ENCODE_DISABLED	0x8000	/* Encoding disabled */
#define RTW_ENCODE_TEMP		0x0400  /* Temporary key */

/* SIOCSIWAUTH/SIOCGIWAUTH struct rtw_param flags */
#define RTW_AUTH_INDEX		0x0FFF

/* RTW_AUTH_80211_AUTH_ALG values (bit field) */
#define RTW_AUTH_ALG_OPEN_SYSTEM	0x00000001
#define RTW_AUTH_ALG_SHARED_KEY		0x00000002

/* struct iw_encode_ext ->alg */
enum RTW_IW_ENC_ALG {
	RTW_ENCODE_ALG_NONE = 0,
	RTW_ENCODE_ALG_WEP,
	RTW_ENCODE_ALG_TKIP,
	RTW_ENCODE_ALG_CCMP,
	RTW_ENCODE_ALG_PMK,
	RTW_ENCODE_ALG_AES_CMAC //IGTK
};

/* struct iw_encode_ext ->ext_flags */
#define RTW_ENCODE_EXT_RX_SEQ_VALID	0x00000002
#define RTW_ENCODE_EXT_GROUP_KEY	0x00000004
#define RTW_ENCODE_EXT_SET_TX_KEY	0x00000008


/****************************** TYPES ******************************/

/* --------------------------- SUBTYPES --------------------------- */

struct sockaddr_t {
	__u8 sa_len;
	__u8 sa_family;
	char sa_data[14];
};

/*
 *	Generic format for most parameters that fit in an int
 */
struct	rtw_param {
	__s32		value;		/* The value of the parameter itself */
	__u8		fixed;		/* Hardware should not use auto select */
	__u8		disabled;	/* Disable the feature */
	__u16		flags;		/* Various specifc flags (if any) */
};

/*
 *	For all data larger than 16 octets, we need to use a
 *	pointer to memory allocated in user space.
 */
struct	rtw_point {
	void		*pointer;	/* Pointer to the data  (in user space) */
	__u16		length;		/* number of fields or size in bytes */
	__u16		flags;		/* Optional params */
};

/*
 *	A frequency
 *	For numbers lower than 10^9, we encode the number in 'm' and
 *	set 'e' to 0
 *	For number greater than 10^9, we divide it by the lowest power
 *	of 10 to get 'm' lower than 10^9, with 'm'= f / (10^'e')...
 *	The power of 10 is in 'e', the result of the division is in 'm'.
 */
struct	rtw_freq {
	__s32		m;		/* Mantissa */
	__s16		e;		/* Exponent */
	__u8		i;		/* List index (when in range struct) */
	__u8		flags;		/* Flags (fixed/auto) */
};



/* ------------------------ IOCTL REQUEST ------------------------ */
/*
 * This structure defines the payload of an ioctl, and is used
 * below.
 *
 * Note that this structure should fit on the memory footprint
 * of rtwreq (which is the same as ifreq), which mean a max size of
 * 16 octets = 128 bits. Warning, pointers might be 64 bits wide...
 * You should check this when increasing the structures defined
 * above in this file...
 */
union	rtwreq_data {
	/* Config - generic */
	char		name[IFNAMSIZ];
	/* Name : used to verify the presence of  wireless extensions.
	 * Name of the protocol/provider... */

	struct rtw_point	essid;		/* Extended network name */
	struct rtw_param	nwid;		/* network id (or domain - the cell) */
	struct rtw_freq	freq;		/* frequency or channel :
					 * 0-1000 = channel
					 * > 1000 = frequency in Hz */

	struct rtw_param	sens;		/* signal level threshold */
	struct rtw_param	snr;		/* signal noise ratio */
	struct rtw_param	bitrate;	/* default bit rate */
	struct rtw_param	txpower;	/* default transmit power */
	struct rtw_param	rts;		/* RTS threshold threshold */
	struct rtw_param	frag;		/* Fragmentation threshold */
	__u32		mode;		/* Operation mode */
	struct rtw_param	retry;		/* Retry limits & lifetime */

	struct rtw_point	encoding;	/* Encoding stuff : tokens */
	struct rtw_param	power;		/* PM duration/timeout */

	struct sockaddr_t	ap_addr;	/* Access point address */
	struct sockaddr_t	addr;		/* Destination address (hw/mac) */

	struct rtw_param	param;		/* Other small parameters */
	struct rtw_point	data;		/* Other large parameters */
	struct rtw_point	passphrase;		/* Extended network name */
};

/*
 * The structure to exchange data for ioctl.
 * This structure is the same as 'struct ifreq', but (re)defined for
 * convenience...
 * Do I need to remind you about structure size (32 octets) ?
 */
struct	rtwreq {
#if 0
	union {
		char	ifrn_name[IFNAMSIZ];	/* if name, e.g. "eth0" */
	} ifr_ifrn;
#endif
	char	ifr_name[IFNAMSIZ];	/* if name, e.g. "eth0" */

	/* Data part (defined just above) */
	union	rtwreq_data	u;
};

/* -------------------------- IOCTL DATA -------------------------- */
/*
 *	For those ioctl which want to exchange mode data that what could
 *	fit in the above structure...
 */



/*
 * Private ioctl interface information
 */

struct	rtw_priv_args {
	__u32		cmd;		/* Number of the ioctl to issue */
	char		name[IFNAMSIZ];	/* Name of the extension */
};


/* ----------------------- WIRELESS EVENTS ----------------------- */
/*
 * Wireless events are carried through the rtnetlink socket to user
 * space. They are encapsulated in the IFLA_WIRELESS field of
 * a RTM_NEWLINK message.
 */

#define IW_EXT_STR_FOURWAY_DONE  "WPA/WPA2 handshake done"
#define IW_EXT_STR_RECONNECTION_FAIL  "RECONNECTION FAILURE"
#define IW_EVT_STR_STA_ASSOC	"STA Assoc"
#define IW_EVT_STR_STA_DISASSOC	"STA Disassoc"
#define IW_EVT_STR_SEND_ACTION_DONE	"Send Action Done"
#define IW_EVT_STR_NO_NETWORK "No Assoc Network After Scan Done"
#define IW_EVT_STR_ICV_ERROR "ICV Eror"
#define IW_EVT_STR_CHALLENGE_FAIL "Auth Challenge Fail"
#endif	/* _WIRELESS_H */
