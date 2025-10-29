/* protocol header */

#ifndef __RWM_PROTO_H__
#define __RWM_PROTO_H__
#ifdef __cplusplus
extern "C" {
#endif

#define MAC_ADDR_LEN 6

/* ETHER */
#define ETHDR_LEN 14
#define ETH_ADDR_LEN 6

#ifndef PP_HTONS
#define PP_HTONS(x) ((u16_t)((((x) & (u16_t)0x00ffU) << 8) | (((x) & (u16_t)0xff00U) >> 8)))
#endif
#define PP_NTOHS(x) PP_HTONS(x)
#ifndef PP_HTONL
#define PP_HTONL(x) ((((x) & (u32_t)0x000000ffUL) << 24) | \
                     (((x) & (u32_t)0x0000ff00UL) <<  8) | \
                     (((x) & (u32_t)0x00ff0000UL) >>  8) | \
                     (((x) & (u32_t)0xff000000UL) >> 24))
#endif
#define PP_NTOHL(x) PP_HTONL(x)

struct ethhdr {
	u8_t dest[ETH_ADDR_LEN];
	u8_t src[ETH_ADDR_LEN];
	u16_t proto;
};

/* UDP */
#define UDP_HDR_LEN	8U
struct udphdr {
	/*source port*/
	u16_t sport;
	/*destination port*/
	u16_t dport;
	/*udp packet length, includes 8 bytes udp_hdr and udp payload*/
	u16_t len;
	/*udp checksum*/
	u16_t checksum;
};

/* IPv4 */
#define IP_ADDR_LEN		4U
#define IP_HDR_LEN		20U
#define IP4H_PROTO(hdr) ((hdr)->proto)
#define IPH_TOS(hdr) ((hdr)->tos)
#define BK_IP_STR(_ip) ((_ip[0]) & 0xFF), (((_ip[1])) & 0xFF), (((_ip[2])) & 0xFF), (((_ip[3])) & 0xFF)
struct ipv4_hdr {
  	/*version / header length*/
  	u8_t v_hl;
  	/*type of service*/
  	u8_t tos;
  	/*total length*/
  	u16_t len;
  	/*identification*/
  	u16_t id;
  	/*fragment offset field*/
  	u16_t offset;
#define IP_RF 0x8000U        /*reserved fragment flag*/
#define IP_DF 0x4000U        /*don't fragment flag*/
#define IP_MF 0x2000U        /*more fragments flag*/
#define IP_OFFMASK 0x1fffU   /*mask for fragmenting bits*/
	/*time to live*/
  	u8_t ttl;
  	/*protocol*/
  	u8_t proto;
  	/*checksum*/
  	u16_t chksum;
  	/*source IP addresses*/
  	u32_t src;
	/*destination IP addresses*/
  	u32_t dest;
};

/* ARP */
#define ARP_OPCODE(hdr) ((hdr)->opcode)
#define ARP_TYPE(hdr) ((hdr)->hwtype)
#define ARP_PROTO(hdr) ((hdr)->proto)
struct etharp_hdr {
	u16_t hwtype;
 	u16_t proto;
	u8_t  hwlen;
  	u8_t  protolen;
 	u16_t opcode;
	/*sender hardware addr*/
 	u8_t shwaddr[MAC_ADDR_LEN];
	/*sender Internet addr*/
 	u8_t sipaddr[IP_ADDR_LEN];
	/*target hardware addr*/
	u8_t dhwaddr[MAC_ADDR_LEN];
	/*target Internet addr*/
	u8_t dipaddr[IP_ADDR_LEN];
};

/* DHCP */
/*DHCP message item offsets and length*/
#define DHCP_CHADDR_LEN	16U
#define DHCP_SNAME_OFS	44U
#define DHCP_SNAME_LEN	64U
#define DHCP_FILE_LEN		128U
#define DHCP_MSG_LEN		236U
#define DHCP_OPTIONS_OFS	(DHCP_MSG_LEN + 4U)/*4byte:cookie*/
#define DHCP_SPORT(hdr) ((hdr)->sport)
#define DHCP_DPORT(hdr) ((hdr)->dport)
struct dhcpmsg
{
	/*dhcp msg type:boot request/boot reply*/
	u8_t msg_type;
	/*hardware type: ethernet(0x01)*/
  	u8_t hw_type;
	/*hardware address length:6*/
  	u8_t hw_len;
  	u8_t hops;
	/*transaction ID*/
  	u32_t tran_id;
	/*second since boot start*/
	u16_t secs;
	/*bootp flags:unicast*/
  	u16_t flags;
	/*client ip address, ip addr known by client*/
 	u8_t ciaddr[IP_ADDR_LEN];
	/*your (client) ip address, client ip addr given by srv*/
	u8_t yiaddr[IP_ADDR_LEN];
	/*server ip address*/
  	u8_t siaddr[IP_ADDR_LEN];
	/*gateway ip address*/
  	u8_t giaddr[IP_ADDR_LEN];
	/*client mac address,
	    includes 6bytes mac address and 10bytes padding*/
  	u8_t chaddr[DHCP_CHADDR_LEN];
	/*server host name*/
	u8_t sname[DHCP_SNAME_LEN];
	/*boot file name*/
  	u8_t file[DHCP_FILE_LEN];
	/*magic cookie*/
	u32_t cookie;
#define DHCP_MIN_OPTIONS_LEN 68U

/*make sure user does not configure this too small */
#if ((defined(DHCP_OPTIONS_LEN)) && (DHCP_OPTIONS_LEN < DHCP_MIN_OPTIONS_LEN))
#undef DHCP_OPTIONS_LEN
#endif
/*allow this to be configured in lwipopts.h, but not too small */
#if (!defined(DHCP_OPTIONS_LEN))
/*set this to be sufficient for your options in outgoing DHCP msgs */
#define DHCP_OPTIONS_LEN DHCP_MIN_OPTIONS_LEN
#endif
	u8_t options[DHCP_OPTIONS_LEN];
};

/* EAPOL */
struct eapol {
	/*version of 802.1X Authentication*/
	u8_t ver;
	/*type of key*/
	u8_t type;
	/*length of eapol data*/
	u16_t length;
	/*key descriptor type*/
	u8_t dtype;
	/*key information*/
	/* Note: key_info, key_length, and key_data_length are unaligned */
	u8 key_info[2]; /* big endian */
	/*length of key*/
	u16_t keylen;
	/*replay counter*/
	u8 replay_counter[8];
	u8 key_nonce[32];
	u8 key_iv[16];
	u8 key_rsc[8];
	u8 key_id[8];
};


/* IPV6 */
#define IP6_HLEN 40
#define ICMPV6_TYPE(hdr) ((hdr)->type)
#define IP6_NEXT_DEADER(hdr) ((hdr)->nexth)
#define IP6H_FL(hdr) (lwip_ntohl((hdr)->v_tc_fl) & 0x000fffff)
struct ipv6_hdr {
  /*version / traffic class / flow label */
 	u32_t v_tc_fl;
  /*payload length */
	u16_t plen;
  /*next header */
	u8_t nexth;
  /*hop limit */
	u8_t hoplim;
  /*source and destination IP addresses */
	u32_t src[4];
	u32_t dest[4];
};

struct icmpv6_hdr {
	u8_t type;
	u8_t code;
	u16_t chksum;
  	u32_t data;
};
#if CONFIG_RWNX_PROTO_DEBUG
void rwm_proto_printf(int direction, struct ethhdr *eth_hdr_ptr);
#ifdef __cplusplus
}
#endif
#endif
#endif
