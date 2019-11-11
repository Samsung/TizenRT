# How to port TizenRT on new WiFi chipset

## Contents
- [How to add external WiFi library](#how-to-add-external-wifi-library)
- [How to use WPA_SUPPLICANT](#how-to-use-wpa_supplicant)
- [Interfacing to WiFi Manager](#interfacing-to-wifi-manager)
- [Incorporating WiFi Chipset Driver](#incorporating-wifi-chipset-driver)


There are two cases to consider towards porting TizenRT on a new WiFi chipset. The specific case depends on the vendor's decision to either use their proprietary supplicant codebase, or
to directly use TizenRT's WiFi solution.

Below are mentioned the two cases in detail. Additionally, we provide guidelines on how to interface TizenRT's WiFi Manager to the newly added chipset. Lastly, instructions are provided to incorporate the WiFi chipset driver in TizenRT's code tree. Specifically, details are provided
to interface driver to network stack, and to get/set WiFi configuration from a higher abstraction layer.
 
## How to add external WiFi library

External vendors may choose to build TizenRT application that includes their compiled WiFi library, rather than opting for TizenRT's *wpa_supplicant*.
To support external and independently compiled WiFi supplicant libraries, they must be added to TizenRT.  
For details on how to add libraries, please refer to [How to add Static Library](HowToAddStaticLibrary.md).

## How to use WPA_SUPPLICANT
If the developer wishes to use TizenRT's *wpa_supplicant* for WiFi-based applications, then following changes need to be performed at *[wpa_suppliant](#supplicant)*.

### Supplicant

Change in *external* directory for a new WiFi board to include new chipset driver.

1. Make sure you have defined *CONFIG_DRIVER_\<DRIVER_NAME\>* in *external/wpa_supplicant/Kconfig*. Refer to the example below for *CONFIG_DRIVER_T20*, which
is subject to the condition that *CONFIG_SCSC_WLAN* is enabled already.
	```
	config DRIVER_T20
		bool "Driver T20 for SLSI WiFi"
		default y
		depends on SCSC_WLAN
	```

2. Add new chipset driver under *external/wpa_supplicant/src/drivers/*, and name it as
*driver_\<driver_name\>.c*. Select this file for build in *external/wpa_supplicant/src/drivers/Make.defs*, depending on the Kconfig that is defined for your WiFi Driver.
An example is shown for LSI WiFi:
	```
	ifeq ($(CONFIG_DRIVER_T20), y)
	CSCRS += driver_t20.c
	endif
	```

Inside *driver_\<driver_name\>.c*, declare a driver structure variable named *wpa_driver_\<driver_name\>_ops*.
*wpa_driver_\<driver_name\>_ops* is a structure of type *wpa_driver_ops* that is declared in *external/wpa_supplicant/src/drivers/driver.h*.
The *wpa_driver_ops* structure contains function pointers to the specific WiFi Driver that the supplicant links to.
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
This links the supplicant to the relevant WiFi Driver.

## Interfacing to WiFi Manager

TizenRT features a generic WiFi Manager framework (API located at *framework/inc/wifi_manager*) for applications.
In order to interface the new WiFi chipset to this WiFi Manager, please do the following:

### Configuring new WiFi library, WiFi Driver for WiFi Manager

When you activate WiFi Manager, you have to specify your preference for WiFi library and WiFi Driver.
Accordingly, please modify the *framework/src/wifi_manager/Kconfig* as shown in the example below:

```
if WIFI_MANAGER
choice
	prompt "WiFi library"
	default SLSI_WPA_SUPPLICANT

...
config SELECT_<SUPPLICANT_LIBRARY_NAME>
	bool "Select external vendor supplicant"
	default n
	select <SUPPLICANT_LIBRARY_NAME>
	---help---
		select the third party supplicant
endchoice # WiFi supplicant library

choice
	prompt "WiFi Driver"
	default SELECT_SCSC_WLAN
...
config SELECT_DRIVER_<DRIVER_NAME>
	bool "Enable vendor-specific Wireless Module"
	default n
	select DRIVER_<DRIVER_NAME>
endchoice # WiFi Driver choice

if <SUPPLICANT_LIBRARY_NAME>
source "$EXTERNALDIR/<wifi_lib_dir>/Kconfig"
source "$EXTERNALDIR/<supplicant_dir>/Kconfig"
endif #<SUPPLICANT_LIBRARY_NAME>

```
Here, *SELECT_<SUPPLICANT_LIBRARY_NAME>*, and *SELECT_DRIVER_<DRIVER_NAME>* are flags to enable the third-party WiFi supplicant and driver, respectively.
When enabled, menuconfig will automatically include *<SUPPLICANT_LIBRARY_NAME>* and *DRIVER_<DRIVER_NAME>* for build with WiFi Manager.
Finally, enable the configuration parameters for the third-party WiFi library by sourcing the Kconfigs in the relevant *driver* folder
Recommended folder path is *os/driver/wireless/<wifi_driver_name>/wpa_supplicant*.

Note that, if your WiFi software solution already includes the driver as an external library, you do *not* need a config parameter for build.
However, you do need to inform TizenRT's build system accordingly, so please add in a *SELECT_DRIVER_NONE* as shown below:

```
config SELECT_DRIVER_NONE
	bool "No Wireless Driver Module Required"
	default n
	depends on SELECT_<SUPPLICANT_LIBRARY_NAME>
```
Please add the above lines *ONLY* if you do not need wireless driver support from TizenRT. Also note, that in such cases, you should interface TizenRT's network stack directly
to the WiFi library. These details are covered further in [Incorporating WiFi Chipset Driver](#incorporating-wifi-chipset-driver).

### Choosing the right WiFi utils for build

The WiFi Manager framework implements specific WiFi utility functions that will either 1) directly
talk to the WiFi supplicant software or 2) using an interface called light-weight netlink 80211 (*LWNL80211*).

#### Direct call WiFi supplicant software
In case of a proprietary WiFi API used over WiFi supplicant, the developer
should invoke those APIs using the WiFi utility functions. TizenRT provides a standard API for WiFi utilities, which is declared under
*framework/src/wifi_manager/wifi_utils.h*. Please make sure you create a new WiFi utility file named *<driver_prefix>_wifi_utils.c* in the same
folder, and interface to the WiFi supplicant, using the API in *wifi_utils.h*. Developers are encouraged to consult *slsi_wifi_utils.c* as a reference implementation
that interfaces to TizenRT's WPA_SUPPLICANT. Finally, to build the utility file that you have created, please edit *framework/src/wifi_manager/Make.defs*,
and add the following lines as shown below:

```
ifeq ($(CONFIG_<SUPPLICANT_LIBRARY_NAME>), y)
CSRCS += <driver_prefix>_wifi_utils.c
endif
```
#### Light-Weight Netlink 802.11 (LWNL80211)
*LWNL80211* is an interface transffering information between the kernel and user-space processes.
It defines a set of commands between WiFi driver and WiFi utilities in TizenRT. The developer, then, should invoke system calls to send the command to the driver,
using a standard API for WiFi utilities provided by TizenRT. Please make sure the developer needs to create a new WiFi utility file named *<driver_prefix>_wifi_utils_lwnl80211.c*
in the same folder, and implement the corresponding driver interfaces in *os/drivers/lwnl/<driver_prefix>_drv.c*.
The driver interface should provide a generic ops functions listed by *struct lwnl80211_ops_s* in *<driver_prfix>_drv.c* as shown below,
where each of ops is defined in *os/include/tinyara/lwnl/lwnl80211.h*.
```
struct lwnl80211_ops_s {
	CODE lwnl80211_result_e (*init)(struct lwnl80211_lowerhalf_s *dev);
	CODE lwnl80211_result_e (*deinit)(void);
	CODE lwnl80211_result_e (*scan_ap)(void *arg);
	CODE lwnl80211_result_e (*connect_ap)(lwnl80211_ap_config_s *config, void *arg);
	CODE lwnl80211_result_e (*disconnect_ap)(void *arg);
	CODE lwnl80211_result_e (*get_info)(lwnl80211_info *info);
	CODE lwnl80211_result_e (*start_sta)(void);
	CODE lwnl80211_result_e (*start_softap)(lwnl80211_softap_config_s *config);
	CODE lwnl80211_result_e (*stop_softap)(void);
	CODE lwnl80211_result_e (*set_autoconnect)(uint8_t chk);
	CODE lwnl80211_result_e (*drv_ioctl)(int cmd, unsigned long arg);
};
```
Please note that developers need to inform TizenRT's build system accordingly,
so add *LWNL80211_<driver_prefix>* in Kconfig as shown below.
```
config LWNL80211_<driver_prefix>
	bool "<driver_prefix> driver"
	---help---
		<driver_prefix> WiFi driver interface

```
For adding a logging system such as *DEBUG_LWNL80211_<driver_prefix>_ERROR* and *DEBUG_LWNL80211_<driver_prefix>_INFO*,
please refer to [How to Debug](HowToDebug.md).
Developers are encouraged to consult *slsi_wifi_utils_lwnl80211.c* and *slsi_drv.c* as reference implementations defined in
*framework/src/wifi_manager/* and *os/drivers/lwnl/*, respectively.
Finally, to build the utility and driver interface files, Make.defs in each folder should include the below.
```
ifeq ($(CONFIG_SELECT_<SUPPLICANT_LIBRARY_NAME>), y)
ifeq ($(CONFIG_LWNL80211), y)
CSRCS += <driver_prefix>_wifi_utils_lwnl80211.c
else
CSRCS += <driver_prefix>_wifi_utils.c
endif
endif
```
```
ifeq ($(CONFIG_LWNL80211_<driver_prefix>),y)
	CSRCS += <driver_prefix>_drv.c
endif
```

## Incorporating WiFi Chipset Driver

The WiFi Driver source files should reside under a newly created *os/driver/wireless/<wifi_driver_name>* folder.
In the following subsections, we describe how to configure the new WiFi Driver for build. Additionally, we also describe how
to initialize and interface it with the overlying network stack.

### WiFi Driver Configuration

The choice for the WiFi Driver is already made when configuring WiFi Manager, as described in [configuring new wifi library, Wifi Driver for wifi manager](#configuring-new-wifi-library-wifi-driver-for-wifi-manager).
Accordingly, please select the WiFi Driver to build, by adding the lines to *os/drivers/wireless/Kconfig* as shown below:
```
if DRIVERS_WIRELESS && SELECT_DRIVER_<DRIVER_NAME>

source drivers/wireless/<wifi_driver_name>/Kconfig

endif # DRIVERS_WIRELESS
```

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
#### 1. Initialization of LWNL80211 and driver interface
When the driver is loaded during the board's initialzation routine, LWNL80211 and corresponding vendor-sepcific driver also need to be initiailzed.
LWNL80211 now adopts a generic virtual file system (vfs) of TizenRT, so the registration API is provided in *os/drivers/lwnl/lwnl80211.c*.
Please refer to the example code in *os/drivers/wireless/scsc/dev.c*.

```
#ifdef CONFIG_LWNL80211_<driver_prefix>
	FAR struct lwnl80211_lowerhalf_s *<driver_prefix>_drv;
	int ret;
	<driver_prefix>_drv = <driver_prefix>_drv_initialize();
	if (!<driver_prefix>_drv) {
		return;
	}

	ret = lwnl80211_register(<driver_prefix>_drv);
	if (ret < 0) {
		return;
	}
#endif
```

#### 2. Interfacing the WiFi Driver to the network stack
In TizenRT, the *netif* structure links the WiFi Driver to the overlying network layer. Netif creation and initialization should
follow immediately after WiFi Driver initialization. Inside the *\<board\>_wlan.c* file, this can be implemented as a three step process:
1. Allocate memory for LWIP's netif structure, and populate its fields with the driver API
2. Add the netif created in step 1 to TizenRT's runtime environment
3. Initialize the netif created in step 2, thus completing the interface between LWIP and the WiFi Driver.

Please note that netif creation (Step 1) should be called from *board_initialize()* function defined at *os/arch/arm/src/\<board_name\>/src/\<board\>_boot.c*.
The three steps above can be implemented as functions, as shown in the template code below:

```
/* STEP 3: Initialization of the netif structure - this completes the linking
 * between the driver and the network layer
 */
static err_t wlan_init(struct netif *netif)
{
	// netif init code ...
}
/* STEP 2: Add the netif structure globally, so that different protocol layers
 * can access it.
 */
void up_wlan_init(struct netif *dev)
{
	// netif_add code ...
}
/* STEP 1: Create a netif structure, and populate its fields with driver API functions
 */
void up_wlan_netif_create(void)
{
	struct netif *dev = NULL;
	// netif creation code ...
#ifndef SELECT_DRIVER_NONE
	dev->d_ifup = <wifi_driver_init_fn>;
	dev->d_ifdown = <wifi_driver_deinit_fn>;
	dev->linkoutput = <wifi_driver_linkoutput_fn;
	dev->output = <wifi_driver_output_fn>;
#else
	dev->d_ifup = <wifi_library_init_fn>;
	dev->d_ifdown = <wifi_library_deinit_fn>;
	dev->linkoutput = <wifi_library_linkoutput_fn;
	dev->output = <wifi_library_output_fn>;
#endif
	up_wlan_init(dev);
}

```
In Step 1 above, we link the *netif* structure to either WiFi Driver functions or external WiFi library functions, depending upon the third-party WiFi solution type.
The *netif* structure is populated with function handlers for managing the network interface (*d_ifup* and *d_ifdown*), as well as handlers for data (*linkoutput* and *output*).
An example netif initialization can be found under *up_wlan_init* function  in *os/arch/arm/src/sidk_s5jt200/s5jt200_wlan.c* file. In this case,
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

#### 3. WiFi configuration data
Please define two functions for reading and writing WiFi configuration data, as *up_wlan_read_config* and *up_wlan_write_config* functions.
The prototype and implementation for the functions are vendor-specific. As a reference, please refer to the *up_wlan_read_config* function in *os/arch/arm/src/sidk_s5jt200/s5jt200_wlan.c*.


#### 4. Getting WiFi MAC address and the WiFi firmware address.
Please refer to the functions *up_wlan_get_mac_addr* and *up_wlan_get_firmware* in *os/arch/arm/src/sidk_s5jt200/s5jt200_wlan.c*.
