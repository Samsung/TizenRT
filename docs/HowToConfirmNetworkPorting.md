# How to Confirm Network Porting
TizenRT provides the network performance measurement tool 
to assist in successful porting of the network stack  
such as LwIP, Wi-Fi Manager, and Wi-Fi driver.  
In order to verify the stability and robustness,
please make sure to follow the procedures outlined below.

## Contents
> [Wi-Fi Manager Test](#wi-fi-manager-test)  
> [TCP and UDP Performance](#tcp-and-udp-performance)  

## Wi-Fi Manager Test
Wi-Fi Manager supports sample application, called **wm_test**,  
for ease of use and performance evaluation of the Wi-Fi Manager APIs.  
Please refer to [How To Add TASH](HowToAddTASHCMD.md), to newly install TASH command.  

Below is a list of services provided by **wm_test**:  
start and stop of Wi-Fi Manager,
joining and leaving the network,
running SoftAP,
scanning,
getting connection information.

### How to Enable **wm_test**
To enable/disable Wi-Fi Manager sample application,
```
cd $TIZENRT_BASEDIR
cd os
./dbuild.sh menuconfig (or make menuconfig)
```
Select Application Configuration -> Examples -> Wi-Fi Manager Sample

### How to Use **wm_test**
0. Please type wm_test as below to obtain how to use it,
```
TASH>> wm_test
```
1. To initialize Wi-Fi Manager,
```
TASH>> wm_test start
```
2. To connect the existing network,
```
TASH>> wm_test join <SSID> <SECURITY> <PASSWORD>
```
Note that Wi-Fi Manager runs DHCPC automatically.  
SECURITY list: open, wep_shared, wpa_aes, wpa2_aes, wpa2_mixed.
  
3. To disconnect the current connection,
```
TASH>> wm_test leave (or wm_test cancel)
```
4. To scan the nearby APs,
```
TASH>> wm_test scan
```
5. To obtain current Wi-Fi Manager's status,
```
TASH>> wm_test mode
```
6. To save the AP configuration on the file system,
```
TASH>> wm_test set <SSID> <SECURITY> <PASSWORD>
```
7. To get/remove the saved AP configuration,
```
TASH>> wm_test get/reset
```
8. To test all of the Wi-Fi Manager APIs,
```
TASH>> wm_test auto <SOFTAP_SSID> <SOFTAP_PASSWORD> <SSID> <SECURITY> <PASSWORD>
```
9. To obtain Wi-Fi Manager stats,
```
TASH>> wm_test stats
```
10. To deinitialize Wi-Fi Manager,
```
TASH>> wm_test stop
```

### Stress Test
**wm_test** utilizes stress test tool defined in [stress_tool](../external/stress_tool/),
which provides recursive aging test modules.  
To enable it,
```
cd $TIZENRT_BASEDIR
cd os
./dbuild.sh menuconfig (or make menuconfig)
```
Select External Libraries -> Enable Stress Tool  
Select Application Configuration -> Enable Stress Tool For WiFi Manager  
Please refer to 'Help' and fill in the necessary parts below.  
Finally you can run stress test:
```
TASH>> wm_test stress
```

### Wi-Fi Manager UTC
To enable Wi-Fi Manager UTC,
```
cd $TIZENRT_BASEDIR
cd os
./dbuild.sh menuconfig (or make menuconfig)
```
Select Application Configuration -> Examples -> TestCase Example  
Select TestCase Example -> Wi-Fi Manager UTC TestCase Example  
Please refer to 'Help' and fill in the necessary parts below.  
Finally you can run Wi-Fi Manager UTC:
```
TASH>> wifi_manager_utc
```  
If you see the results shown below, all UTC are passed successfully.
```
############ WiFiManager UTC End [PASS : 25, FAIL : 0] ##############
```
  

## TCP and UDP Performance
Iperf3 is a tool for active measurments of the maximum achievable bandwidth on IP networks.   
It supports tuning of various parameters related to timing, buffers and protocols.  
Please refer to [iperf](https://iperf.fr) for more details.

### How to Use iperf
Assumming successful LwIP loading on the system,  
TCP/UDP throughput can be measured by using iperf3.  
To enable it,
```
cd $TIZENRT_BASEDIR
cd os
./dbuild.sh menuconfig (or make menuconfig)
```
Select External Libraries -> cJSON library  
Select Application Configuration -> System Libraries and Add-Ons -> iperf App  
After a successful Wi-Fi connection,  
* iperf server on the board
```
TASH>> iperf -s
```

* iperf client on the board
```
TCP: TASH>> iperf -c <SERVER_IPADDR> -t <TIME> 
UDP: TASH>> iperf -c <SERVER_IPADDR> -u -t <TIME>
```
