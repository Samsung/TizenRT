#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <stdint.h>


#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#include "connectivity_interface.h"

#ifdef CONFIG_NET_LWIP
#include <net/lwip/netif.h>
#endif

#include "common_monitor_interface.h"


char cm_iobuffer[PROC_BUFFER_LEN];



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
#if defined(CONFIG_WIFI_MANAGER)
	strcpy(mac,CONFIG_WIFIMGR_STA_IFNAME);
#elif defined(CONFIG_WICED)
    strcpy(mac,"en1");
#elif defined(CONFIG_NET_ETHERNET)
    strcpy(mac,"eth0");
#elif defined(CONFIG_NET_802154)
    strcpy(mac,"wlan0");
#else
	strcpy(mac,"wl1");
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
    int ret;
    char *filepath;
    ret = mount(NULL, PROC_MOUNTPOINT, "procfs", 0, NULL);
    printf("mount returns value %d\n", ret);
    ret = asprintf(&filepath, "%s/connectivity/ipaddr", PROC_MOUNTPOINT);
    ret = readonce(filepath, cm_iobuffer, PROC_BUFFER_LEN);
    strncpy(ipAddr, cm_iobuffer, strlen(cm_iobuffer));
    if (ret == OK)
    {
        printf("IP addr is %s\n", ipAddr);
    }
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
		struct in6_addr addr6;
		netlib_get_dripv6addr(mac, addr6 );
		inet_ntop(AF_INET6, &addr6, routerIPAddr, INET6_ADDRSTRLEN);
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

void get_signal_strength(int *rssi_value)
{
    int ret;
    char *filepath;
    ret = mount(NULL, PROC_MOUNTPOINT, "procfs", 0, NULL);
    printf("mount returns value %d\n", ret);
    ret = asprintf(&filepath, "%s/connectivity/rssi", PROC_MOUNTPOINT);
    ret = readonce(filepath, cm_iobuffer, PROC_BUFFER_LEN);
    if (ret == OK)
    {
//         *rssi_value = atoi(cm_iobuffer);
        sscanf(cm_iobuffer, "%d", rssi_value);
        printf("rssi is %d\n", *rssi_value);
    }
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

int get_lqi()
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

int get_link_utilization()
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


/****************************************************************************
 * Name: get_bitrate
 *
 * Description:
 *   Get bitrate
 *
 * Returned Value:
 *
 *
 ****************************************************************************/
 void get_bitrate(int *bitrate)
 {
    int ret;
    char *filepath;
    ret = mount(NULL, PROC_MOUNTPOINT, "procfs", 0, NULL);
    printf("mount returns value %d\n", ret);
    ret = asprintf(&filepath, "%s/connectivity/bitrate", PROC_MOUNTPOINT);
    ret = readonce(filepath, cm_iobuffer, PROC_BUFFER_LEN);
    if (ret == OK)
    {
        *bitrate = atoi(cm_iobuffer);
    }
 }

/****************************************************************************
 * Name: get_network_bearer
 *
 * Description:
 *   Get Network Bearer
 *
 * Returned Value:
 *
 *
 ****************************************************************************/
void get_network_bearer(int *nwbearer)
{
    int ret;
    char *filepath;
    ret = mount(NULL, PROC_MOUNTPOINT, "procfs", 0, NULL);
    printf("mount returns value %d\n", ret);
    ret = asprintf(&filepath, "%s/connectivity/nwbearer", PROC_MOUNTPOINT);
    ret = readonce(filepath, cm_iobuffer, PROC_BUFFER_LEN);
    if (ret == OK)
        sscanf(cm_iobuffer, "%d", nwbearer);
}

/****************************************************************************
 * Name: get_avl_network_bearer
 *
 * Description:
 *   Get Available Network Bearer
 *
 * Returned Value:
 *
 *
 ****************************************************************************/
void get_avl_network_bearer(int *nwbearer)
{
    int ret;
    char *filepath;
    ret = mount(NULL, PROC_MOUNTPOINT, "procfs", 0, NULL);
    printf("mount returns value %d\n", ret);
    ret = asprintf(&filepath, "%s/connectivity/available_bearer", PROC_MOUNTPOINT);
    ret = readonce(filepath, cm_iobuffer, PROC_BUFFER_LEN);
    if (ret == OK)
        sscanf(cm_iobuffer, "%d", nwbearer);
}




