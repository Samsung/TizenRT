# How to port TizenRT on new WiFi chipset

There are two cases to consider towards porting TizenRT on a new WiFi chipset. The specific case depends on the vendor's decision to either use their proprietary supplicant codebase, or
to directly use TizenRT's WiFi solution.

Below are mentioned the two cases in detail. Additionally, instructions are provided to incorporate the WiFi chipset driver in TizenRT's code tree. Specifically, details are provided
to interface driver to network stack, and to get/set WiFi configuration from a higher abstraction layer.
 
## How to add external WiFi library

External vendors may choose to build TizenRT application that includes their compiled WiFi libary, rather than opting for TizenRT's *wpa_supplicant*.
To support external and independently compiled WiFi supplicant libraries, they must be added to TizenRT.  
For details on how to add libraries, please refer to [How to add Static Library](HowToAddStaticLibrary.md).

## How to use WPA_SUPPLICANT
If the developer wishes to use TizenRT's *wpa_supplicant* for WiFi-based applications, then following changes need to be performed at *[wpa_suppliant](#supplicant)*.

### Supplicant

Changes in *external* directory for a new WiFi board to include new chipset driver.

1. Make sure you have defined *CONFIG_DRIVER_\<DRIVER_NAME\>* in *external/wpa_supplicant/Kconfig*. Refer to the example below for *CONFIG_DRIVER_T20*, which
is subject to the condition that *CONFIG_SCSC_WLAN* is enabled already.
```
config DRIVER_T20
	bool "Driver T20 for SLSI WiFi"
	default y
	depends on SCSC_WLAN
```

2. Add new chipset driver under *external/wpa_supplicant/src/drivers/*, and name it as
*driver_\<driver_name\>.c*. Select this file for build in *external/wpa_supplicant/src/drivers/Make.defs*, depending on the Kconfig that is defined for your WiFi driver.
An example is shown for LSI WiFi:

```
ifeq ($(CONFIG_DRIVER_T20), y)
CSCRS += driver_t20.c
endif
```

Inside *driver_\<driver_name\>.c*, declare a driver structure variable named *wpa_driver_\<driver_name\>_ops*.
*wpa_driver_\<driver_name\>_ops* is a structure of type *wpa_driver_ops* that is declared in *external/wpa_supplicant/src/drivers/driver.h*.
The *wpa_driver_ops* structure contains function pointers to the specific WiFi driver that the supplicant links to.
As an example, refer to the *wpa_driver_t20_ops* structure declared in *external/wpa_supplicant/src/drivers/driver_t20.c* for the LSI WiFi chipset.
```
const struct wpa_driver_ops wpa_driver_t20_ops = {
	.name = "slsi_t20",
	.desc = "SLSI T20 Driver",
	.init2 = slsi_t20_init,
	.deinit = slsi_t20_deinit,
	.get_mac_addr = slsi_get_mac_addr,
	.get_capa = slsi_t20_get_capa,
	.scan2 = slsi_hw_scan,…
} 
```

3. Add the driver structure variable created in step 2 to the *wpa_drivers* list in *external/wpa_supplicant/src/drivers/drivers.c*, as shown below:
```
#ifdef CONFIG_DRIVER_<DRIVER_NAME>
extern struct wpa_driver_ops wpa_driver_<driver_name>_ops;	/* driver_<driver_name>.c */
#endif

const struct wpa_driver_ops *const wpa_drivers[] = {
#ifdef CONFIG_DRIVER_<DRIVER_NAME>
	&wpa_driver_<DRIVER_NAME>_ops,
#endif
...
}
```
This links the supplicant to the relevant WiFi driver.


## Incorporating WiFi Chipset Driver

The WiFi driver source files should reside under a newly created *os/driver/wireless/<wifi_driver_name>* folder.
Please follow the steps below to initialize the driver and interface it with the overlying network stack.

### WiFi Driver Initialization
This driver should be initialized from the main board's initialization routine.
An example initialization can be found under *os/arch/arm/src/sidk_s5jt200/src/s5jt200_boot.c*:

```
void board_initialize(void) {
...
#ifdef CONFIG_SCSC_WLAN
	slldbg("Initialize SCSC driver\n");
	slsi_driver_initialize();
#endif
}
```

Make sure you create your board-specific files at *os/arch/arm/src/\<board_name\>/src/\<board\>_boot.c*, and call the
driver initialization function as shown in the example above.

Next, inside your board specific directory, create a *\<board\>_wlan.c* file. This file should include following functionalities:
#### 1. Interfacing the WiFi driver to the Network Stack
In TizenRT, the *netif* structure links the WiFi driver to the overlying network layer. Netif creation should
be called from the WiFi driver initialization function. Please make sure you have implemented *netif.c* and *netif.h* files in your driver code for this purpose.
An example netif creation can be found under *up_wlan_init* function  in *os/arch/arm/src/sidk_s5jt200/s5jt200_wlan.c* file. In this case,
during bootup, *up_wlan_init* is invoked from the driver initialization routine, and creates the *netif* structure. The *up_wlan_init* function is shown below for reference:

```
static err_t wlan_init(struct netif *netif)
{
	netif->name[0] = IFNAME0;
	netif->name[1] = IFNAME1;

	snprintf(netif->d_ifname, IFNAMSIZ, "%c%c%d", netif->name[0], netif->name[1], netif->num);

	netif->flags = NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET | NETIF_FLAG_BROADCAST | NETIF_FLAG_IGMP;

	return ERR_OK;
}

void up_wlan_init(struct netif *dev)
{
	struct ip_addr ipaddr;
	struct ip_addr netmask;
	struct ip_addr gw;

	/* Start LWIP network thread */
	ipaddr.addr = inet_addr("0.0.0.0");
	netmask.addr = inet_addr("255.255.255.255");
	gw.addr = inet_addr("0.0.0.0");

	netif_set_default(dev);

	wlan_netif = netif_add(dev, &ipaddr, &netmask, &gw,
					NULL, wlan_init, tcpip_input);
	wlan_netif->flags |= NETIF_FLAG_IGMP;
}
```

#### 2. WiFi configuration data
Please define two functions for reading and writing WiFi configuration data, as *up_wlan_read_config* and *up_wlan_write_config* functions.
The prototype and implementation for the functions are vendor-specific. As a reference, please refer to the *up_wlan_read_config* function in *os/arch/arm/src/sidk_s5jt200/s5jt200_wlan.c*.


#### 3. Getting WiFi MAC address and the WiFi firmware address.
Please refer to the functions *up_wlan_get_mac_addr* and *up_wlan_get_firmware* in *os/arch/arm/src/sidk_s5jt200/s5jt200_wlan.c*.
