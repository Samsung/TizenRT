# How to Confirm Network Porting
TizenRT provides the network performance measurement tool 
to assist in successful porting of the network stack  
such as LwIP, Wi-Fi Manager, and Wi-Fi driver.  
In order to verify the stability and robustness,
please make sure to follow the procedures outlined below.

## Contents
- [Wi-Fi Manager Test](#wi-fi-manager-test)
- [TCP and UDP Performance](#tcp-and-udp-performance)

## Wi-Fi Manager Test
For ease of use and performance evaluation of the Wi-Fi Manager APIs, Wi-Fi Manager supports sample application, called **wm_test**,
Please refer to [How To Add TASH](HowToAddTASHCMD.md), to newly install TASH command.  

The list of services provided by **wm_test** is as follows:
- start and stop of Wi-Fi Manager
- joining and leaving the network
- running SoftAP
- scanning
- getting connection information.

### How to Enable/Disable **wm_test**
To enable/disable Wi-Fi Manager sample application, do the following:
1. Execute the dbuild.sh script with the menuconfig option
	```
	cd $TIZENRT_BASEDIR
	cd os
	./dbuild.sh menuconfig (or make menuconfig)
	```
2. Select Application Configuration -> Examples -> Wi-Fi Manager Sample

### How to Use **wm_test**
1. Please type wm_test as below to obtain how to use it,
	```
	TASH>> wm_test
	```
2. To initialize Wi-Fi Manager,
	```
	TASH>> wm_test start
	```
3. To connect the existing network,
	```
	TASH>> wm_test join <SSID> <SECURITY> <PASSWORD>
	```
> **Note**
> Wi-Fi Manager runs DHCPC automatically. The list of options under SECURITY is as follows:
> - open,
> - wep_shared,
> - wpa_aes,
> - wpa2_aes,
> - wpa2_mixed.
  
4. To disconnect the current connection,
	```
	TASH>> wm_test leave (or wm_test cancel)
	```
5. To scan the nearby APs,
	```
	TASH>> wm_test scan
	```
6. To obtain current Wi-Fi Manager's status,
	```
	TASH>> wm_test mode
	```
7. To save the AP configuration on the file system,
	```
	TASH>> wm_test set <SSID> <SECURITY> <PASSWORD>
	```
8. To get/remove the saved AP configuration,
	```
	TASH>> wm_test get/reset
	```
9. To test all of the Wi-Fi Manager APIs,
	```
	TASH>> wm_test auto <SOFTAP_SSID> <SOFTAP_PASSWORD> <SSID> <SECURITY> <PASSWORD>
	```
10. To obtain Wi-Fi Manager stats,
	```
	TASH>> wm_test stats
	```
11. To deinitialize Wi-Fi Manager,
	```
	TASH>> wm_test stop
	```

### Stress Test
**wm_test** utilizes stress test tool defined in [stress_tool](../external/stress_tool/),
which provides recursive aging test modules.  
To enable the stress test tool, do the following:
1. Execute the dbuild.sh script with the menuconfig option
	```
	cd $TIZENRT_BASEDIR
	cd os
	./dbuild.sh menuconfig (or make menuconfig)
	```
2. Select External Libraries -> Enable Stress Tool
3. Select Application Configuration -> Enable Stress Tool For WiFi Manager
4. Type as below to run stress test:
Finally you can run stress test:
	```
	TASH>> wm_test stress
	```
> **Note**
> Please refer to 'Help' wherever necessary to fill in the configurations above in points 2 and 3.
### Wi-Fi Manager UTC
To enable Wi-Fi Manager UTC, do the following:
1. Execute the dbuild.sh script with the menuconfig option
	```
	cd $TIZENRT_BASEDIR
	cd os
	./dbuild.sh menuconfig (or make menuconfig)
	```
2. Select Application Configuration -> Examples -> TestCase Example
3. Select TestCase Example -> Wi-Fi Manager UTC TestCase Example
4. Type as below to tun Wi-Fi Manager UTC:
	```
	TASH>> wifi_manager_utc
	```
> **Note**
> Please refer to 'Help' wherever necessary to fill in the configurations above in points 2 and 3.
If all UTC are passed successfully, the results are shown as below.
```
############ WiFiManager UTC End [PASS : 25, FAIL : 0] ##############
```

## TCP and UDP Performance
Iperf3 is a tool for active measurments of the maximum achievable bandwidth on IP networks.
It supports tuning of various parameters related to timing, buffers and protocols.  
Please refer to [iperf](https://iperf.fr) for more details.

### How to Use iperf
Assumming successful LwIP loading on the system, TCP/UDP throughput can be measured by using iperf3.
To enable iperf, do the following:
1. Execute the dbuild.sh script with the menuconfig option
	```
	cd $TIZENRT_BASEDIR
	cd os
	./dbuild.sh menuconfig (or make menuconfig)
	```
2. Select External Libraries -> cJSON library
3. Select Application Configuration -> System Libraries and Add-Ons -> iperf App
4. After a successful Wi-Fi connection, type the following:
* iperf server on the board
	```
	TASH>> iperf -s
	```
* iperf client on the board
	```
	TCP: TASH>> iperf -c <SERVER_IPADDR> -t <TIME>
	UDP: TASH>> iperf -c <SERVER_IPADDR> -u -t <TIME>
	```
