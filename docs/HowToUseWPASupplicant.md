# How to use WPA_SUPPLICANT

Changes in *[wpa_suppliant](#supplicant)* and *[wireless driver](#drivers)* folders are mandantory to add WPA_SUPPLICANT support to a new WiFi board.  

## Supplicant

Changes in *external* directory for a new WiFi board to include new chipset driver.

1. Add new chipset driver under *external/wpa_supplicant/src/drivers/*, and name it as
*driver_\<driver_name\>.c*

2. Inside *driver_\<driver_name\>.c*, declare a driver structure variable named *wpa_driver_\<driver_name\>_ops*.
*wpa_driver_\<driver_name\>_ops* is a strcuture of type *wpa_driver_ops* that is declared in *external/wpa_supplicant/src/drivers/driver.h*.
The *wpa_driver_ops* structure contains function pointers to the specific WiFi driver that the supplicant links to.


3. As an example, refer to the *wpa_driver_t20_ops* structure decalred in 
*external/wpa_supplicant/src/drivers/driver_t20.c* for the LSI WiFi chipset.
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

4. Add the driver structure variable created in step 2 to the *wpa_drivers[]* list in *external/wpa_supplicant/src/drivers/drivers.c*, as shown below:
```
#ifdef CONFIG_DRIVER_\<DRIVER_NAME\>
extern struct wpa_driver_ops wpa_driver_\<driver_name\>_ops;	/* driver_\<driver_name\>.c */
#endif
```
This links the supplicant to the relevent WiFi driver.


5. Make sure you have defined *CONFIG_DRIVER_\<DRIVER_NAME\>* using *external/wpa_supplicant/Make.defs*, as shown below:
```
CFLAGS += -DCONFIG_DRIVER_\<DRIVER_NAME\>
```


## Driver

The WiFi driver needs to be initialized from the main board's initialization routine.
An example initialization can be found under *os/arch/arm/src/sidk_s5jt200/src/s5jt200_boot.c*:

```
void board_initialize(void) { ...
#ifdef CONFIG_SCSC_WLAN
	slldbg("Initialize SCSC driver\n");
	slsi_driver_initialize();
#endif
}
```

Make sure you create your board-specific files at *os/arch/arm/src/\<board_name\>/src/\<board\>_boot.c*, and call the
driver initialization function as shown in the example above.

