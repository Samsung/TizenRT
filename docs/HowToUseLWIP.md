# How to use LwIP
**LwIP (LightWeight IP)** is an open source TCP/IP stack designed for embedded systems.
Please refer to [lwip_summary](https://savannah.nongnu.org/projects/lwip/) for more details.

## Contents
- [Main Features](#main-features)  
- [How to Use](#how-to-use)

## Main Features
**LwIP** provides various features needed for the tcp/ip stack across multiple network layers as below.
- **Application layer** - Domain Name Server (DNS), Simple Network Management Protocol (SNMP) agent, Dynamic Host Configuration Protocol (DHCP) server/client, Simple Network Time Protocol (SNTP), Multicast DNS, MQTT client.
- **Transport layer** - User Datagram Protocol (UDP), UDP-Lite extensions, Transmission Control Protocol (TCP), RAW socket and pcbs, Berkeley-like socket API.
- **Internet layer** - IPv4 and IPv6, packet forwarding over multiple network interfaces, Internet Control Message Protocol (ICMP) and ICMPv6, Internet Group Management Protocol (IGMP).
- **Link layer** - Point-to-point protocol (PPP), Ethernet Address Resolution Protocol (ARP), Neighbor Discovery Protocol (NDP) and NDPv6.

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

<br>
## How to Use
TizenRT supports LwIP as a default network stack, managed by [Network Manager](../docs/HowToPortTizenRTOnWiFiChipset.md).
Since Network Manager encapsulates the network stack,
the vendor does not need to take the configuration of LwIP into account,
following the guide linked above.

One thing to note is that
the vendor should carefully use the network buffer called *pbuf* in LwIP.
Especially, TizenRT does not allow the vendor to directly allocate *struct pbuf*,
but provides *tx_buf* inside the network device structure.
Moreover, TizenRT recommends to use a static memory pool for *pbuf*,
and the memory configuration is implemented in [memp.c](../os/net/lwip/src/core/memp.c).
The vendor, then, can set up the memory pool,
depending on the main configuration inside menuconfig below:
```
Networking Support -> LwIP options -> Memory Configurations
```

Finally, considering the memory pool size of the board and Wi-Fi driver,
the vendor should tune the size of the message box (*mbox*) used in LwIP,
defined as CONFIG_NET_TCPIP_MBOX_SIZE in .config file.
This value can also be set into menuconfig below:
```
Networking Support -> LwIP options -> LWIP Mailbox Configurations -> LWIP Task Mailbox Size
```
