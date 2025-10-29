/* rwm protocol printf */

#include <stddef.h>
#include "lwip/arch.h"
#include "components/log.h"
#include "rwm_proto.h"
#include "rwnx_defs.h"

UINT8 proto_debug_flag = 0;
UINT8 rwm_set_proto_debug(uint8_t flag)
{
	if (flag == 1 || flag == 0) {
		proto_debug_flag = flag;
		return 0;
	} else {
		return -1;
	}
}

static void rwm_udp_printf(char *dir, struct ipv4_hdr *ip_hdr)
{
	char *udp_tmp = (char *)ip_hdr + IP_HDR_LEN;
	struct udphdr *udphdr = (struct udphdr *)udp_tmp;

	if (((htons(DHCP_SPORT(udphdr)) == 68) && (htons(DHCP_DPORT(udphdr)) == 67)) ||
		((htons(DHCP_SPORT(udphdr)) == 67) && (htons(DHCP_DPORT(udphdr)) == 68))) {
		/*dhcp message of udp*/
		char *dmsg_tmp = NULL;
		struct dhcpmsg *dmsg = NULL;
		dmsg_tmp = (char *)udphdr + UDP_HDR_LEN;
		dmsg = (struct dhcpmsg *)dmsg_tmp;

		if ((dmsg->options[2] & 0xff) == 0x01)
			RWNX_LOGI("%s DHCP-discover  id %0x(%u)\n", dir, ntohs(ip_hdr->id), ntohs(ip_hdr->id));
		else if ((dmsg->options[2] & 0xff) == 0x02)
			RWNX_LOGI("%s DHCP-offer  id %0x(%u)\n", dir, ntohs(ip_hdr->id), ntohs(ip_hdr->id));
		else if ((dmsg->options[2] & 0xff) == 0x03)
			RWNX_LOGI("%s DHCP-request  id %0x(%u)\n", dir, ntohs(ip_hdr->id), ntohs(ip_hdr->id));
		else if ((dmsg->options[2] & 0xff) == 0x04)
			RWNX_LOGI("%s DHCP-decline  id %0x(%u)\n", dir, ntohs(ip_hdr->id), ntohs(ip_hdr->id));
		else if ((dmsg->options[2] & 0xff) == 0x05)
			RWNX_LOGI("%s DHCP-ACK  id %0x(%u)\n", dir, ntohs(ip_hdr->id), ntohs(ip_hdr->id));
		else if ((dmsg->options[2] & 0xff) == 0x06)
			RWNX_LOGI("%s DHCP-NAK  id %0x(%u)\n", dir, ntohs(ip_hdr->id), ntohs(ip_hdr->id));
		else if ((dmsg->options[2] & 0xff) == 0x07)
			RWNX_LOGI("%s DHCP-release  id %0x(%u)\n", dir, ntohs(ip_hdr->id), ntohs(ip_hdr->id));
		else if ((dmsg->options[2] & 0xff) == 0x08)
			RWNX_LOGI("%s DHCP-inform  id %0x(%u)\n", dir, ntohs(ip_hdr->id), ntohs(ip_hdr->id));
		else
			RWNX_LOGI("%s UDP, id %0x(%u)\n, Unknow DHCP type\n", dir, ntohs(ip_hdr->id), ntohs(ip_hdr->id));
	} else {
		/* other udp packets*/
		RWNX_LOGI("%s UDP, id %0x(%u)\n", dir, ntohs(ip_hdr->id), ntohs(ip_hdr->id));
	}
}

static void rwm_ip_printf(char *dir, struct ethhdr *eth_hdr_ptr)
{
	char *tmp = (char *)eth_hdr_ptr + ETHDR_LEN;
	struct ipv4_hdr *iphdr = (struct ipv4_hdr *)tmp;

	switch (IP4H_PROTO(iphdr)) {
	case 1:     //IP_PROTO_ICMP
		RWNX_LOGI("%s ICMP, id %0x(%u)\n", dir, ntohs(iphdr->id), ntohs(iphdr->id));
		break;
	case 2:     //IP_PROTO_IGMP
		RWNX_LOGI("%s IGMP, id %0x(%u)\n", dir, ntohs(iphdr->id), ntohs(iphdr->id));
		break;
	case 17:     //IP_PROTO_UDP
		rwm_udp_printf(dir, iphdr);
		break;
	case 136:     //IP_PROTO_UDP_LITE
		RWNX_LOGI("%s UDP_LITE, id %0x(%u)\n", dir, ntohs(iphdr->id), ntohs(iphdr->id));
		break;
	case 6:     //IP_PROTO_TCP
		RWNX_LOGI("%s TCP, id %0x(%u)\n", dir, ntohs(iphdr->id), ntohs(iphdr->id));
		break;

	default:     //UNKNOW TYPE
		RWNX_LOGI("%s other ip type, id %0x(%u)\n", dir, ntohs(iphdr->id), ntohs(iphdr->id));
		break;
	}
}

static void rwm_arp_printf(char *dir, struct ethhdr *eth_hdr_ptr)
{
	char *tmp = NULL;
	struct etharp_hdr *arphdr = NULL;

	tmp = (char *)eth_hdr_ptr + ETHDR_LEN;
	arphdr = (struct etharp_hdr *)tmp;

	if (ntohs(ARP_OPCODE(arphdr)) == 1) {
		RWNX_LOGI("%s ARP - Request\n", dir);
	} else if (ntohs(ARP_OPCODE(arphdr)) == 2) {
		RWNX_LOGI("%s ARP - Response\n", dir);
	} else {
		RWNX_LOGI("%s ERR ARP\n", dir);
		return;
	}
	RWNX_LOGI("Hardware: %u\n", ntohs(ARP_TYPE(arphdr)));
	RWNX_LOGI("Protocol: 0x%02x\n", ntohs(ARP_PROTO(arphdr)));
	RWNX_LOGI("Hardware Addr Length: 6\n");
	RWNX_LOGI("Protocol Addr Length: 4\n");
	RWNX_LOGI("Operation: %u\n", ntohs(ARP_OPCODE(arphdr)));
	RWNX_LOGI("Sender Hardware Addr: "BK_MAC_FORMAT "\n", BK_MAC_STR(arphdr->shwaddr));
	RWNX_LOGI("Sender Internet Addr: "BK_IP4_FORMAT " \r\n", BK_IP_STR(arphdr->sipaddr));
	RWNX_LOGI("Target Hardware Addr: "BK_MAC_FORMAT "\n", BK_MAC_STR(arphdr->dhwaddr));
	RWNX_LOGI("Target Internet Addr: "BK_IP4_FORMAT " \r\n", BK_IP_STR(arphdr->dipaddr));
}

static void rwm_ipv6_printf(char *dir, struct ethhdr *eth_hdr_ptr)
{
	char *tmp = NULL;
	struct icmpv6_hdr *icmpv6 = NULL;
	struct ipv6_hdr *ip6hdr = NULL;

	tmp = (char *)eth_hdr_ptr + ETHDR_LEN;
	ip6hdr = (struct ipv6_hdr *)tmp;

	if (IP6_NEXT_DEADER(ip6hdr) == 0) {         //HOPBYHOP
	} else if (IP6_NEXT_DEADER(ip6hdr) == 6) {  //TCP
	} else if (IP6_NEXT_DEADER(ip6hdr) == 17) { //UDP
	} else if (IP6_NEXT_DEADER(ip6hdr) == 41) { //ENCAPS
	} else if (IP6_NEXT_DEADER(ip6hdr) == 43) { //ROUTING
	} else if (IP6_NEXT_DEADER(ip6hdr) == 58) { //ICMPV6
		tmp = (char *)eth_hdr_ptr + ETHDR_LEN + IP6_HLEN;
		icmpv6 = (struct icmpv6_hdr *)tmp;

		if (ICMPV6_TYPE(icmpv6) == 135)
			RWNX_LOGI("%s NS \n", dir);
		else if (ICMPV6_TYPE(icmpv6) == 134)
			RWNX_LOGI("%s RA \n", dir);
		else if (ICMPV6_TYPE(icmpv6) == 133)
			RWNX_LOGI("%s RS \n", dir);
		else if (ICMPV6_TYPE(icmpv6) == 136)
			RWNX_LOGI("%s NA \n", dir);
	} else if (IP6_NEXT_DEADER(ip6hdr) == 59) {     //NONE
		RWNX_LOGI("%s NONE\n", dir);
	} else if (IP6_NEXT_DEADER(ip6hdr) == 60) {     //DESTOPTS
		RWNX_LOGI("%s DESTOPTS\n", dir);
	} else if (IP6_NEXT_DEADER(ip6hdr) == 136) {    //UDPLITE
		RWNX_LOGI("%s UDPLITE\n", dir);
	} else {
		RWNX_LOGI("%s OTHER IPv6 type\n", dir);
	}
}

int eapol_cnt = 0;
static void rwm_eapol_printf(char *dir)
{
	if (eapol_cnt == 0) {
		RWNX_LOGI("%s Eapol frame process 1\n", dir);
		eapol_cnt++;
	} else if (eapol_cnt == 1) {
		RWNX_LOGI("%s Eapol frame process 2\n", dir);
		eapol_cnt++;
	} else if (eapol_cnt == 2) {
		RWNX_LOGI("%s Eapol frame process 3\n", dir);
		eapol_cnt++;
	} else {
		RWNX_LOGI("%s Eapol frame process 4\n", dir);
	}
}

void rwm_proto_printf(int direction, struct ethhdr *eth_hdr_ptr)
{
	//direction: 0:TX, 1:RX
	char *dir = (direction == 0 ? "Tx" : "Rx");
	RWNX_LOGW("da:"BK_MAC_FORMAT ", sa:"BK_MAC_FORMAT ", proto:%lu\r\n",
		BK_MAC_STR(eth_hdr_ptr->dest), BK_MAC_STR(eth_hdr_ptr->src), eth_hdr_ptr->proto);

	switch (htons(eth_hdr_ptr->proto)) {
	case 0x0800U:    //ETHTYPE_IP
		rwm_ip_printf(dir, eth_hdr_ptr);
		break;

	case 0x0806U:    //ETHTYPE_ARP
		rwm_arp_printf(dir, eth_hdr_ptr);
		break;

	case 0x86DDU:    //ETHTYPE_IPV6
		rwm_ipv6_printf(dir, eth_hdr_ptr);
		break;

	case 0x888EU:    //ETHTYPE_EAPOL:
		rwm_eapol_printf(dir);
		break;

	case 0x0842U:       //ETHTYPE_WOL
	case 0x8035U:       //ETHTYPE_RARP
	case 0x8100U:       //ETHTYPE_VLAN
	case 0x8863U:       //ETHTYPE_PPPOEDISC
	case 0x8864U:       //ETHTYPE_PPPOE
	case 0x8870U:       //ETHTYPE_JUMBO
	case 0x8892U:       //ETHTYPE_PROFINET
	case 0x88A4U:       //ETHTYPE_ETHERCAT
	case 0x88CCU:       //ETHTYPE_LLDP
	case 0x88CDU:       //ETHTYPE_SERCOS
	case 0x88E3U:       //ETHTYPE_MRP
	case 0x88F7U:       //ETHTYPE_PTP
	case 0x9100U:       //ETHTYPE_QINQ
		break;

	default:
		RWNX_LOGI("%s other ethernet type\n", dir);
		break;
	}
}

