# How to Port TizenRT on New Wi-Fi Chipset
TizenRT features a generic network management architecture to interface the vendor's network driver on TizenRT's network stack,
targeted to ethernet, Wi-Fi, bluetooth and so on.
To this end, we provide __Network Manager__,
the one-hand solution to manage all network stacks and drivers in the TizenRT's kernel,
providing an environment for vendors to easily port their drivers.

We provide guidelines on how to interface TizenRT's Network Manager to the newly added chipset
in order to incorporate the Wi-Fi chipset driver in TizenRT's code tree,
and how to attach the vendor's driver to TizenRT's network stack,
and to get and set the Wi-Fi configuration from the higher abstraction layer.

Furthermore, we introduce how to port a new Wi-Fi chipset library and corresponding Wi-Fi driver code.
Especially, towards porting a new Wi-Fi chipset over TizenRT,
the vendor is able to decide either  
1) to use their proprietary supplicant codebase, or  
2) to directly use TizenRT's Wi-Fi solution,  
and a set of code can be opened or provided by a static library (_*.a_),
depending on the decision by the vendors.

## Contents
- [Interface to Network Manager](#interface-to-network-manager)
- [How to add external Wi-Fi library](#how-to-add-external-wi-fi-library)
- [How to use WPA_SUPPLICANT](#how-to-use-wpa_supplicant)
- [Incorporate Wi-Fi Chipset Driver](#incorporate-wi-fi-chipset-driver)

## Interface to Network Manager
In this section,
we offer how to configure Network Manager including vendor specific,
leveraging network stack resources and controlling data flow attached to network stack.

Furthermore, we introduce __Wi-Fi Manager__,
a management framework for controlling Wi-Fi connections
at the user-domain higher layer,
and provide how to define a light-weight netlink 802.11 (*lwnl80211*)
which is a driver interface between Network Manager and Wi-Fi Manager, across the kernel and user space, respectively.

### Network stack bring-up
Network manager encapsulates its own network stack,
so the vendors and/or users do not need to access or set up the network stack directly.
Instead, Network Manager provides a way 1) to configure the network stack and
2) to connect it with the vendor's Wi-Fi driver interface.
Please follow steps below:

#### 1. Network Manager initialization
When *CONFIG_NET* is enabled,
Network Manager automatically starts when the OS is brought up,
including network stack, **Light-weight IP (LwIP)**, by default.
If CONFIG_NET_LOOPBACK_INTERFACE is set,
then a loopback interface named *lo* is initialized with an IP address of *127.0.0.1*,
being ready to be used.
Therefore, the vendors do not need to initialize any other network stack,
and also to newly register loopback interface of its own.
Instead, the vendors can utilize TizenRT's network stack only through network device structure (*netdev*).

#### 2. Network Manager configuration
The vendors should register the network device (*netdev*) structure on TizenRT's Network Manager,
which consists of the name of interface, type, ops, transmit buffer, vendor-specific structure, etc.
To fill out netdev_config structure needed to open an interface on the network stack,
please refer to the below [netdev_mgr.h](../os/include/tinyara/netmgr/netdev_mgr.h):

```
	struct netdev_config {
		struct nic_io_ops *ops;
		int flag;
		int mtu;
		int hwaddr_len;
		uint8_t hwaddr[NM_MAX_HWADDR_LEN];

		int is_default;
		union {
			struct ethernet_ops *eth;
			struct trwifi_ops *wl;
		} t_ops;
		netdev_type type;

		int (*d_ioctl)(struct netdev *dev, int cmd, unsigned long arg); // SIOCSMIIREG
		void *priv;
	};

```
- *struct nic_io_ops ops*: Driver-specific function pointer structure of *linkoutput* and *igmp_mac_filter*. The former is used for the vendor-specific linkoutput ops and the latter is optionally set to IGMP MAC filter allowing the IGMP messages at the MAC level. This ops then will be registered to the network stack in order to be used for packet transmissions and IGMP details.
- *flag*: Interface configuration flags such as NM_FLAG_BROADCAST, NM_FLAG_ETHARP, NM_FLAG_ETHERNET, NM_FLAG_IGMP, and NM_FLAG_MLD6 defined in [netdev_mgr.h](../os/include/tinyara/netmgr/netdev_mgr.h).
- *mtu*: MTU size supported by the vendor's driver.
- *hwaddr_len*: MAC addr byte length (6 bytes in general).
- *is_default*: Specification of the default interface in the network stack.
- *t_ops*: Set of control-domain vendor-specific driver's ops linked to [lwnl80211](#light-weight-netlink-80211-driver).
- *netdev_type type*: NM_LOOPBACK, NM_WIFI, NM_ETHERNET, or NM_UNKNOWN.
- *d_ioctl*: Vendor-specific ioctl command to communicate with PHY layer (called by SIOCSMIIREG).
- *priv*: Vendor-specific network device structure

#### 3. Network device registration
The Wi-Fi driver should call the netdev registration function as below,
when the driver is ready to be used:
```
struct netdev *netdev_register(struct netdev_config *config)
```
Following the netdev configuration [guide](#2-network-manager-configuration) above,
we provide a simple example for Samsung LSI Wi-Fi driver how to use it.
We recommend the below code to be implemented in the location of *os/drivers/wireless/<DRV_NAME>/<DRV_PREFIX>_drv_netmgr.c*.
Please refer to the code, *[slsi_drv_netmgr.c](../os/drivers/wireless/scsc/slsi_drv_netmgr.c)*.
```
	struct netdev* slsidrv_register_dev(int sizeof_priv)
	{
		struct nic_io_ops nops = {slsi_linkoutput, slsi_set_multicast_list};
		struct netdev_config nconfig;
		nconfig.ops = &nops;
		nconfig.flag = NM_FLAG_ETHARP | NM_FLAG_ETHERNET | NM_FLAG_BROADCAST | NM_FLAG_IGMP;
		nconfig.mtu = CONFIG_NET_ETH_MTU; // is it right that vendor decides MTU size??
		nconfig.hwaddr_len = IFHWADDRLEN;
		nconfig.is_default = 1;
		nconfig.type = NM_WIFI;
		nconfig.t_ops.wl = &g_trwifi_drv_ops;

		void *priv = kmm_zalloc(sizeof_priv);
		if (priv == NULL) {
			return NULL;
		}
		nconfig.priv = priv;

		return netdev_register(&nconfig);
	}
```
This registration function is called when the network interface of Wi-Fi driver is initialized,
and the vendor-specific netdev structure can be managed and shared by _void *priv_ defined inside netdev structure of Network Manager.
In other words, the Wi-Fi driver can also access to the netdev of Network Manager through the pointer it has,
but we do not recommend a direct access to netdev of Network Manager from the Wi-Fi driver.

### Wi-Fi Manager
TizenRT provide __Wi-Fi Manager__, a Wi-Fi management framework used by the user-domain higher layer.
It allows the application to initialize and de-initialize the Wi-Fi driver,
connect and disconnect to the wireless access point (AP),
scan the neighbor APs,
and get and set the framework specific information such as IP address and AP configurations.
Two header files of *[wifi_utils.h](../os/include/tinyara/wifi/wifi_utils.h)* and
*[wifi_manager.h](../framework/include/wifi_manager/wifi_manager.h)* describe the list of internally used and public APIs of Wi-Fi Manager, respectively.
The vendors do not need to directly modify or add the existing or new WiFi Manager APIs,
but these can be referred to as what the vendors should implement in the lower layer Wi-Fi driver.

### Light-weight netlink 802.11 driver
Next, TizenRT provides Light-weight netlink 802.11 driver (*lwnl80211*),
which is an interface to transfer information between the kernel and user-space processes,
to control the Wi-Fi basic functionalities such as connecting/disconnecting to the AP, scanning, and driver init/deinit.

It defines a set of commands between Wi-Fi driver and Wi-Fi utilities in TizenRT.
Wi-Fi Manager API, then, invokes system calls to send the command to the driver,
using a standard API used by the user-domain higher layer.
Please refer to a Wi-Fi utility file named
*[wifi_manager_lwnl.c](../framework/src/wifi_manager/wifi_manager_lwnl.c)* is provided as in Wi-Fi Manager framework.

To make a link of Wi-Fi drivers ops with the upper layer API,
the developer needs to create the corresponding driver interfaces in *os/drivers/wireless/<DRV_NAME>/<DRV_PREFIX>_drv_netmgr.c*.
Specifically, the driver interface should provide a generic set of function pointers, ops, listed by *struct trwifi_ops*,
where each of ops is defined in *os/include/tinyara/net/if/wifi.h*.
```
	typedef trwifi_result_e (*trwifi_init)(struct netdev *dev);
	typedef trwifi_result_e (*trwifi_deinit)(struct netdev *dev);
	typedef trwifi_result_e (*trwifi_scan_ap)(struct netdev *dev, trwifi_ap_config_s *config);
	typedef trwifi_result_e (*trwifi_connect_ap)(struct netdev *dev, trwifi_ap_config_s *config, void *arg);
	typedef trwifi_result_e (*trwifi_disconnect_ap)(struct netdev *dev, void *arg);
	typedef trwifi_result_e (*trwifi_get_info)(struct netdev *dev, trwifi_info *info);
	typedef trwifi_result_e (*trwifi_start_sta)(struct netdev *dev);
	typedef trwifi_result_e (*trwifi_start_softap)(struct netdev *dev, trwifi_softap_config_s *config);
	typedef trwifi_result_e (*trwifi_stop_softap)(struct netdev *dev);
	typedef trwifi_result_e (*trwifi_set_autoconnect)(struct netdev *dev, uint8_t chk);
	typedef trwifi_result_e (*trwifi_drv_ioctl)(struct netdev *dev, int cmd, unsigned long arg);

	struct trwifi_ops {
		trwifi_init init;
		trwifi_deinit deinit;
		trwifi_scan_ap scan_ap;
		trwifi_connect_ap connect_ap;
		trwifi_disconnect_ap disconnect_ap;
		trwifi_get_info get_info;
		trwifi_start_sta start_sta;
		trwifi_start_softap start_softap;
		trwifi_stop_softap stop_softap;
		trwifi_set_autoconnect set_autoconnect;
		trwifi_drv_ioctl drv_ioctl;
	};
```

Please note that those ops functions are basically blocking operations.
However, *scan_ap*, *connect_ap*, and *disconnect_ap* are non-blocking functions,
because they require a non-blocking callback from the bottom of Wi-Fi drivers, depending on the results it obtains.
For example, a scanning result should be updated and passed from the lower layer driver.
*lwnl80211* provides a queueing system, to send and receive the callback data between the Wi-Fi driver and the applications,
where the queue and corresponding APIs are defined in *[lwnl_evt_queue.c](../os/drivers/lwnl/lwnl_evt_queue.c)*.
Consequently, the vendors should implement a callback logic using the queue above,
inside *os/drivers/wireless/<DRV_NAME>/<DRV_PREFIX>_drv_netmgr.c*.
The developers are encouraged to consult from Samsung LSI driver code example,
*[slsi_drv_netmgr.c](../os/drivers/wireless/scsc/slsi_drv_netmgr.c)*, as a reference implementation.
It declares *static int slsi_drv_callback_handler()* and *static int8_t slsi_drv_scan_callback_handler()* functions,
called by the callback handler (*linkup_handler* and *linkdown_handler*) self-defined in the Samsung LSI Wi-Fi driver.

For adding a logging system such as *DEBUG_LWNL80211_<driver_prefix>_ERROR* and *DEBUG_LWNL80211_<driver_prefix>_INFO*,
please refer to [How to Debug](HowToDebug.md).

## How to Add External Wi-Fi Library
The vendors is able to select whether they use their compiled Wi-Fi library,
rather than opting for TizenRT's *wpa_supplicant*.
To support external and independently compiled Wi-Fi supplicant libraries,
the vendors should add the static library into the output binary of TizenRT.
For details on how to add libraries, please refer to [How to add Static Library](HowToAddStaticLibrary.md).

## Incorporate Wi-Fi Chipset Driver
The Wi-Fi driver source files should reside under a newly created *os/drivers/wireless/<DRV_NAME>* directory.
In the following subsections, we describe how to build the new Wi-Fi driver in TizenRT's code tree,
to initialize and interface it with the overlying TizenRT's Network Manager.

### Wi-Fi driver configuration
As noted above, the driver source files should be located in *os/drivers/wireless/<DRV_NAME>*.
Files to be compiled and built should be included in *CSRCS* in the newly created Make.defs.
If the driver code requires to use the configurations set in TizenRT, all the source files should include tinyara/config.h.

In order for a user to choose the target Wi-Fi driver,
the vendor should define a new set of driver-related configurations in Kconfig files as below:  

1. *CONFIG_SELECT_<DRV_NAME>_WLAN* in *framework/src/wifi_manager/Kconfig*  
2. *CONFIG_<DRV_NAME>_WLAN* in *os/drivers/wireless/<DRV_NAME>/Kconfig*  

Finally, the new Kconfig file should be added into *os/drivers/wireless/Kconfig* as shown below:
```
if DRIVERS_WIRELESS && SELECT_DRIVER_<DRV_NAME>

source drivers/wireless/<DRV_NAME>/Kconfig

endif # DRIVERS_WIRELESS
```
Please note that Make.defs should have a dependency with *CONFIG_<DRV_NAME>_WLAN*.

### Wi-Fi driver initialization
The driver needs to be initialized during the main board's initialization routine.
An example can be found under *os/arch/arm/src/sidk_s5jt200/src/s5jt200_boot.c* as below:
```
void board_initialize(void) {
...
#ifdef CONFIG_SCSC_WLAN
	slldbg("Initialize SCSC driver\n");
	slsi_driver_initialize();
#endif
}
```
This procedure only covers the driver's internal initialization,
prior to bringing up the network stack and device used by Network Manager.

## How to use wpa_supplicant
The vendor is able either to use their proprietary (vendor-specific) supplicant codebase, or
to directly use TizenRT's Wi-Fi solution.

### Configure vendor-specific Wi-Fi supplicant
When activating Wi-Fi Manager,
the vendor should specify the corresponding Wi-Fi library and Wi-Fi driver
as shown in the above subsection.
Here, the default is set to use the vendor-specific supplicant,
the vendors can easily enable their own supplicant,
without an additional modification in the Wi-Fi Manager's Kconfig.

```
choice
	prompt "Wi-Fi library"
	default SELECT_PROPRIETARY_SUPPLICANT

...
config SELECT_PROPRIETARY_SUPPLICANT
	depends on !SELECT_SCSC_WLAN && !SELECT_NO_DRIVER
	bool "Enable vendor-specific supplicant"
	---help---
		Select the third party supplicant
```

### WPA supplicant
*wpa_supplicant* is an open-source software implementation of an IEEE 802.11i supplicant.
TizenRT officially supports *wpa_supplicant* in *external/wpa_supplicant* directory, and
now Samsung LSI driver adopts it as a default supplicant base.

To use TizenRT's *wpa_supplicant*, please follow the instructions below:

1. Define *CONFIG_DRIVER_<DRV_NAME>* in *external/wpa_supplicant/Kconfig*.
Refer to the example below for *CONFIG_DRIVER_T20*,
which is subject to the condition that *CONFIG_SCSC_WLAN* is enabled already.
```
	config DRIVER_T20
		bool "Driver T20 for SLSI Wi-Fi"
		default y
		depends on SCSC_WLAN
```

2. Add new chipset driver code of *wpa_driver_ops* under *external/wpa_supplicant/src/drivers/*,
and name it as *driver_<DRV_NAME>.c*.
Select this file for build in *external/wpa_supplicant/src/drivers/Make.defs*,
depending on the Kconfig defined for the new Wi-Fi driver.  
An example is shown for System LSI Wi-Fi:
```
	ifeq ($(CONFIG_DRIVER_T20), y)
	CSRCS += driver_t20.c
	endif
```
Inside *driver_<DRV_NAME>.c*, declare a driver structure variable named *wpa_driver_<DRV_NAME>_ops*.
*wpa_driver_<DRV_NAME>_ops* is a structure of type *wpa_driver_ops*
that is defined in *external/wpa_supplicant/src/drivers/driver.h*.
The *wpa_driver_ops* structure contains function pointers to the specific Wi-Fi driver that the supplicant links to.
As an example, refer to the *wpa_driver_t20_ops* structure
defined in *external/wpa_supplicant/src/drivers/driver_t20.c* for the Samsung LSI Wi-Fi chipset.  
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

3. Add the driver structure variable created in step 2 to the *wpa_drivers* list
in *external/wpa_supplicant/src/drivers/drivers.c*, as shown below:
```
	#ifdef CONFIG_DRIVER_<DRIVER_NAME>
	extern struct wpa_driver_ops wpa_driver_<DRV_NAME>_ops; /* driver_<DRV_NAME>.c */
	#endif

	const struct wpa_driver_ops *const wpa_drivers[] = {
	#ifdef CONFIG_DRIVER_<DRV_NAME>
		&wpa_driver_<DRV_NAME>_ops,
	#endif
	    ...
	}
```
This links the supplicant to the relevant Wi-Fi driver.  

