

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "connectivity_interface.h"

#ifdef CONFIG_NET_LWIP
#include <net/lwip/netif.h>
#endif


/*Read APIs*/

/****************************************************************************
 * Name: get_interface_name
 *
 * Description:
 *   Get the underlined interface name
 *   
 *
 * Returned Value:
 *
 ****************************************************************************/
 
void get_interface_name(char *mac)
{
#if defined(CONFIG_NET_ETHERNET)
	strcpy(mac,"eth0");
#elif defined(CONFIG_NET_802154)
	strcpy(mac,"wlan0");
#endif
}


/****************************************************************************
 * Name: get_ip_address
 *
 * Description:
 *   Fetch IPv4/IPv6 address
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/
 
void get_ip_address(char *ipAddr)
{
	printf("Calling inet_ntop\n");
	char mac[4];
	get_interface_name(mac);
#ifdef CONFIG_NET_IPv4
	struct in_addr addr;
	netlib_get_ipv4addr(mac, &addr);
	inet_ntop(AF_INET, &addr, ipAddr, INET_ADDRSTRLEN);
#endif
#ifdef CONFIG_NET_IPv6
	struct in6_addr addr;
	netlib_get_ipv6addr(mac, &addr);
	inet_ntop(AF_INET6, &addr, ipAddr, INET6_ADDRSTRLEN);
#endif
	printf("After inet_ntop\n");
}

/****************************************************************************
 * Name: get_router_ip_address
 *
 * Description:
 *   Get the router IPv4/IPv6 address
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/
 
void get_router_ip_address(char *routerIPAddr)
{
	char mac[4];
	get_interface_name(mac);
#ifdef CONFIG_NET_IPv4	
	struct in_addr addr;
	netlib_get_dripv4addr(mac,addr);
	inet_ntop(AF_INET, &addr, routerIPAddr, INET_ADDRSTRLEN);
#endif
#ifdef CONFIG_NET_IPv6
	struct in6_addr addr;
	netlib_get_dripv6addr(mac,addr);
	inet_ntop(AF_INET6, &addr, routerIPAddr, INET6_ADDRSTRLEN);
#endif		
}

/****************************************************************************
 * Name: get_signal_strength
 *
 * Description:
 *  Get Signal Strength value (RSSI in case of IEEE 802.15.4) 
 *
 * Returned Value:
 *   Return the appropriate Callback flags
 *
 ****************************************************************************/
 
uint8_t get_signal_strength(void)
{
#if defined(CONFIG_NET_802154)
    return get_rssi();
#endif
    return -1;
}

/****************************************************************************
 * Name: get_lqi
 *
 * Description:
 *   Get link quality index value from MAC/PHY
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/

int get_lqi(void)
{
#if defined(CONFIG_NET_802154)
    return get_lqi_val();
#endif
    return -1;
}

/****************************************************************************
 * Name: get_link_utilization
 *
 * Description:
 *   Get link utilization statictics from MAC/PHY
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/
 
int get_link_utilization(void)
{
	return 0;
}


/****************************************************************************
 * Name: get_tx_data
 *
 * Description:
 *   Get tx data value from MAC/PHY
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/
 
int get_tx_data()
{
//TODO: Feature implementation
	return 0;
}

/****************************************************************************
 * Name: get_rx_data
 *
 * Description:
 *   Get rx data value from MAC/PHY
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/
 
int get_rx_data()
{
///TODO: Feature implementation
	return 0;
}

/****************************************************************************
 * Name: get_max_message_size
 *
 * Description:
 *   Get maximum message size
 *
 * Returned Value:
 *   
 *
 ****************************************************************************/
int get_max_message_size()
{
///TODO: Feature implementation
	return 0;
}
