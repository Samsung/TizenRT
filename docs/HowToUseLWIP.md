# How to use LwIP
**LwIP (LightWeight IP)** is an open source TCP/IP stack designed for embedded systems.
Please refer to [lwip_summary](https://savannah.nongnu.org/projects/lwip/) for more details.

## Contents
-[Main Features](#main-features)  
-[Writing device driver](#writing-device-driver)

## Main Features
**LwIP** provides various features needed for the tcp/ip stack across multiple network layers as below.
- **Application layer** - Domain Name Server (DNS), Simple Network Management Protocol (SNMP) agent, Dynamic Host Configuration Protocol (DHCP) server/client, Simple Network Time Protocol (SNTP), Multicast DNS, MQTT client.
- **Transport layer** - User Datagram Protocol (UDP), UDP-Lite extentions, Transmission Control Protocol (TCP), RAW socket and pcbs, Berkeley-like socket API.
- **Internet layer** - IPv4 and IPv6, packet forwarding over multiple network interfaces, Internet Control Message Protocol (ICMP) and ICMPv6, Internet Group Management Protocol (IGMP).
- **Link layer** - Point-to-point protocol (PPP), Ethernet Address Resoultion Protocol (ARP), Neighbor Discovery Protocol (NDP) and NDPv6.

For more details, please refer to [lwip_document](http://www.nongnu.org/lwip/2_0_x/index.html).

### How to Enable
To enable/disable supported features, 
```
cd $TIZENRT_BASEDIR
cd os
./dbuild.sh menuconfig (or make menuconfig)
```
1. Enable LwIP
	```
	Networking Support -> Networking Stack (LwIP)
	```
2. Select features 
	```
	Networking Support -> LwIP options
	```
3. Initialize LwIP by calling lwip_init() defined in [init.c](../os/net/lwip/src/core/init.c).

## Writing device driver
Assuming successful LwIP loading on the system, network interface structure (netif) defined in [netif.h](../os/include/net/lwip/netif.h) needs to be initialized, and connected with device driver for in/out packets.
[ethernetif.c](../os/net/mac/ethernetif.c) offers a good outline for what device driver should include.

### Network Interface Initialization
Netif is generic data structure used for all LwIP network interfaces.
The following is a list of parameters that must be defined for initialization.

#### State variables
* hostname - Name of this interface
* mtu - Maximum transfer unit (in bytes)
* hwaddr_len - Number of bytes used in hardware address
* hwaddr - Link layer hardware address of this interface
* flags
	* NETIF_FLAG_UP - Software flag used to control whether this network interface is enabled and processes traffic
	* NETIF_FLAG_BROADCAST - Broadcast capability
	* NETIF_FLAG_LINK_UP - Active link in the driver (Please use netifapi to set this flag in the driver, [netifapi.h](../os/include/net/lwip/netifapi.h))
	* NETIF_FLAG_ETHARP - ARP capability of ethernet type device
	* NETIF_FLAG_ETHERNET - Ethernet device
	* NETIF_FLAG_IGMP - IGMP capability
	* NETIF_FLAG_MLD6 - MLD6 capability

#### Function pointers
* input - To pass a packet up the TCP/IP stack by device driver (set by netif_add function. Please see below.)
* output - To send a packet on the interface by the IP module
* linkoutput - To send a packet on the interface to the link set by device driver

#### Driver specific variables
* d_mac - MAC layer address
* d_ifup - A function for ifup command
* d_ifdown - A function for ifdown command
* d_private - driver-specific, private information

### Network Interface Management
#### Add interface to LwIP

After initialization of the netif parameters above, device driver should call netif_add defined in [netif.c](../os/net/lwip/src/core/netif.c), to allocate network interface to LwIP.

```
struct netif *netif_add(struct netif *netif, 
#ifdef LWIP_IPv4
                        const ip4_addr_t * ipaddr, const ip4_addr_t * netmask, const ip4_addr_t * gw,
#endif
                        void *state, netif_init_fn init, netif_input_fn input);
```
* netif - This interface
* ipaddr - Default IPv4 address  
* netmask - Default IPv4 netmask address  
* gw - Default IPv4 gateway address  
* state - Driver specific state information, generally set to NULL  
* init - Function pointer for netif initialization listed above  
* input - Function pointer that device driver will call when it has received a new packet (ex. tcpip_input() defined in [tcpip.c](../os/net/lwip/src/api/tcpip.c))  

A netif can be assigned as a default in LwIP, by calling **netif_set_default()** defined in [netif.c](../os/net/lwip/src/core/netif.c).  

#### Bring the interface up

Before you can use the interface, it should be brought up.  
This can be accomplished depending on how the interface gets its IP address.  

* Static IP address - **netif_set_up** and **netif_set_down**, using [netifapi.h](../os/include/net/lwip/netifapi.h) directly  
* DHCP client - Use **dhcp_start** and **dhcp_stop**  
* DHCP server - Use **dhcps_start** and **dhcps_stop**  
* AUTOIP - Use **autoip_start** and **autoip_stop**  

Please note that **netif_is_up()** returns whether the netif is up or not.  

#### Further management

Callback function registration (netif structure)  
* status_callback - Status callback function is provided if you set the LWIP_NETIF_STATUS_CALLBACK in [lwipopts.h](../os/include/net/lwip/lwipopts.h)  
* link_callback - Link callback function is provided if you set the LWIP_NETIF_LINK_CALLBACK in [lwipopts.h](../os/include/net/lwip/lwipopts.h)  
* remove_callback - Remove callback function is provided if you set the LWIP_NETIF_REMOVE_CALLBACK in [lwipopts.h](../os/include/net/lwip/lwipopts.h)  


### Interoperability b/w LwIP and Device Driver
Device driver needs to manage **LwIP state flags** and **network buffer (called pbuf)**. Please refer to the below.

#### LwIP state flags
- NETIF_LWIP_UP: When a network interface is enabled and is able to process traffic, this flag should be set to true. 
Simply, set it to true or false, according to the device driver's status; initialized or deinitialized.
Use LwIP APIs as below (defined in netif.c) to control the flag.
```
void netif_set_down(struct netif *netif);
void netif_set_up(struct netif *netif);
```

- NETIF_LWIP_LINK_UP: This flag should be set to true or false, when the link is activated or deactivated, respectively.
Use LwIP APIs as below (defined in netif.c) to control the flag.
```
void netif_set_link_down(struct netif *netif);
void netif_set_link_up(struct netif *netif);
```

> **Note**
> In general, **device driver up/down** and **link up/down** are not strictly distinguished in LwIP structure.
> However, when the device driver changes its operation mode from STATION to AP,
> both NETIF_LWIP_UP and NETIF_LWIP_LINK_UP can be set to 0 simultaneously,
> or only NETIF_LWIP_LINK_UP is set to 0 without changing NETIF_LWIP_UP, depending on the device driver structure.

#### Network buffer (struct pbuf *p)
- INPUT: To pass a packet up the TCP/IP stack by the device driver, **netif->input** registered by netif_add() API should be called with pbuf.
To allocate pbuf and manage it in the driver level, please refer to APIs defined in pbuf.h (LwIP pbuf APIs).
In general, **tcpip_input()** defined in tcpip.c is used as below.
```
err_t tcpip_input(struct pbuf *p, struct netif *netif_in);
```
> **Note**
> Even if pbuf allocation fails due to memory pool overflow from LwIP, the driver should not discard the packet rashly.


- OUTPUT: To send a packet on the interface to the link, **netif->linkoutput** implemented by the device driver should be called with pbuf.
```
typedef err_t (*netif_linkoutput_fn)(struct netif * netif, struct pbuf * p);
```
> **Note**
> The driver can optimize its structure to manage the packets received from the pbuf.
> However, it cannot drop the packet arbitrarily, depending on the queue or status of the driver.

