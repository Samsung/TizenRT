/****************************************************************************
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
#include <tinyara/config.h>

#include <stdio.h>
#include <fcntl.h>
#include <stdbool.h>
#include <debug.h>
#include <sys/ioctl.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <net/if.h>
#include <net/lwip/opt.h>
#include <net/lwip/netif.h>
#include <net/lwip/tcpip.h>

#include <tinyara/configdata.h>

#include "up_arch.h"
#include "artik05x.h"

struct netif *wlan_netif;

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
static err_t wlan_init(struct netif *netif)
{
	netif->name[0] = 'w';
	netif->name[1] = 'l';

	snprintf(netif->d_ifname, IFNAMSIZ, "wl%d", netif->num);
	netif->flags = NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET | NETIF_FLAG_BROADCAST | NETIF_FLAG_IGMP;

	return ERR_OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
unsigned int up_wlan_write_config(void *buf, unsigned int bufsize)
{
#if defined(CONFIG_MTD_CONFIG)
	int fd;
	struct config_data_s config;

	fd = open("/dev/config", O_RDOK);
	if (fd < 0) {
		lldbg("Failed to open /dev/config\n");
		return false;
	}

	config.id = ARTIK05X_CONFIGDATA_WIFI_NVRAM;
	config.instance = 0;
	config.configdata = (unsigned char *)buf;
	config.len = bufsize;

	ioctl(fd, CFGDIOC_SETCONFIG, &config);
	close(fd);

	return true;
#else
	return false;
#endif
}

unsigned int up_wlan_read_config(void *buf, unsigned int bufsize)
{
#if defined(CONFIG_MTD_CONFIG)
	int fd;
	struct config_data_s config;

	fd = open("/dev/config", O_RDOK);
	if (fd < 0) {
		lldbg("Failed to open /dev/config\n");
		return false;
	}

	config.id = ARTIK05X_CONFIGDATA_WIFI_NVRAM;
	config.instance = 0;
	config.configdata = (unsigned char *)buf;
	config.len = bufsize;

	ioctl(fd, CFGDIOC_GETCONFIG, &config);
	close(fd);

	return true;
#else
	return false;
#endif
}

unsigned int up_wlan_erase_config(void)
{
#if defined(CONFIG_MTD_CONFIG)
	return true;
#else
	return false;
#endif
}

int up_wlan_get_mac_addr(char *macaddr)
{
	int i;
	union {
		unsigned long long raw;
		struct {
			unsigned int low;
			unsigned int high;
		};
		unsigned char bytes[8];
	} addr;

	putreg32(0x003c0180, 0x8000032c);
	putreg32(0x00000001, 0x80000328);

	while ((getreg32(0x80000400) & 0x1) == 0) ;

	addr.low = getreg32(0x80000320);
	addr.high = getreg32(0x80000324);

	addr.raw = addr.raw >> 6;

	for (i = 5; i >= 0; i--) {
		*macaddr++ = addr.bytes[i];
	}

	return 0;
}

void *up_wlan_get_firmware(void)
{
	return (void *)0x04048000;
}

void up_wlan_init(struct netif *dev)
{
	struct ip4_addr ipaddr;
	struct ip4_addr netmask;
	struct ip4_addr gw;

	/* Start LWIP network thread */
	ipaddr.addr = inet_addr("0.0.0.0");
	netmask.addr = inet_addr("255.255.255.255");
	gw.addr = inet_addr("0.0.0.0");

	netif_set_default(dev);

	wlan_netif = netif_add(dev, &ipaddr, &netmask, &gw, NULL, wlan_init, tcpip_input);
}
