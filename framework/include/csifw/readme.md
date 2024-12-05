# How to build and run CSIFW

## A. Guide to build

### 1. Make dir
```
mkdir -p product_code
cd product_code
```


### 2. cbuild.sh
```
#!/usr/bin/env bash

# Set helper home path
export CONANHELPER_HOME="/data1/1212/sahil1.gupta/product_code/myhelperpath" <-- Change this path as per your environment
if [ -z "$CONANHELPER_HOME" ]; then
    export CONANHELPER_HOME="/conanhelper"
fi

if [ ! -d "$CONANHELPER_HOME" ]; then
    echo "The conanhelper does not exist."
    mkdir $CONANHELPER_HOME
    chmod 777 $CONANHELPER_HOME
    git clone git@github.ecodesamsung.com:TizenRT/ssot.git $CONANHELPER_HOME
fi

$CONANHELPER_HOME/conanscript/conanscript.sh "$@"

```

### 3. Clone repo

Clone product_TizenLite5 and CSI_Platform

	git clone git@github.ecodesamsung.com:TizenRT/product_TizenLite5.git
	git clone git@github.ecodesamsung.com:TizenRT/CSI_Platform.git



### 4. (OPTIONAL) Changes to enable csifw_test app

|STEPS | |
|--- | ---|
|1.   Disable Presence detection service | In   file *'product_conan/config_presets/modules/csi_prs.config'* Disable following flag: `CONFIG_PRESENCE_DETECTION_APP`|
|2. Enable csifw_test app | In   file *'CSI_Platform/configs/default/defconfig'*     Enable following flag: `CONFIG_CSIFW_TEST`|
|3. Extra step for long run | In   file *'CSI_Platform/apps/csifw_test/src/csifw_test.c'*     `#define LONG_RUN_TEST 1`|


## 5. Build commands

### 5.1 If changes in repo CSI_Platform (if step 4 performed)
```
./cbuild.sh repo create -p=product_conan -b=CSI_Platform
```
### 5.2 If no changes in CSI_Platform
```
./cbuild.sh repo create -p=product_conan
```


### 6. target board download
```
./cbuild.sh repo -C product_conan download ALL
```
<br>

# B. How to run csifw test app on board

### 1. __TURN ON LOGS__:
- First run command `setlog` on TASH, note the number for CSIFW.

			TASH>> setlog
			PRODUCT_OCF_COMMON                 : 55
			SEC_CPM                            : 69
			CSIFW                              : 70 // ==> num: use this number to control csifw logs
	- `setlog num 3` (to enable info logs)
	- `setlog num 6` (to enable all logs)
2. Connect board to wifi.

3. Run csifw_test command as shown below:
	
		csifw_test <interval> <config_type>

	Example:

			csifw_test 30 0 // CSI service will start sending HT data at 30ms.
			csifw_test 40 1 // CSI service will start sending NON-HT data at 40ms.
			 
 
<br>
<br>
<hr>

# C. CSI Framework Guide


### Supported CSI Data Configs

CSIFW supports following configurations in STA mode: 
- `HT_CSI_DATA` (56 subcarriers each sub-carrier data is 2 bytes)
- `NON_HT_CSI_DATA` (52 subcarriers each sub-carrier data is 2 bytes)
- `HT_CSI_DATA_ACC1` (56 subcarriers each sub-carrier data is 4 bytes)
- `NON_HT_CSI_DATA_ACC1` (52 subcarriers each sub-carrier data is 4 bytes)
<br>

The config can be set using `csi_config_type_t` enum.




<hr>

## HOW TO USE CSIFW APIs (*CSI_Platform/include/csimanager/csifw_api.h*)
<br>

1. __Create callbacks__: Before initializing CSIFW, client needs to implement raw and/or parsed callbacks to receive csi data:

	**RAW DATA CALLBACK**

		typedef void (*client_raw_data_listener)(CSIFW_RES status, int csi_data_len, unsigned char *csi_raw_buff, void* ptr);

	**PARSED DATA CALLBACK**

		typedef void (*client_parsed_data_listener)(CSIFW_RES status, int csi_data_len, float *csi_parsed_buff, void* ptr);

	> Note: Client should copy the data from buffer and return the callback, **the callback must not be blocked**.

<br>

2. __csi_service_init__: After implementing the callbacks, client can initialize CSIFW by calling `csi_service_init`

		CSIFW_RES csi_service_init(csi_config_type_t config_type, client_raw_data_listener raw_callback, client_parsed_data_listener parsed_callback, unsigned int interval, void* ptr);
	- `interval` : The interval in microseconds.
	- If raw/parsed data is not required, pass `NULL` in place of callback.
<br>

3. __csi_service_start__: Call `csi_service_start` and csi service will start sending data to client.

		CSIFW_RES csi_service_start();
<br>

4. __csi_service_stop__: Call `csi_service_stop` and csi service will stop sending data to client.

		CSIFW_RES csi_service_stop(CSIFW_REASON reason);
	- `reason`: Reason for stopping the service. {`CSIFW_NORMAL`, `CSIFW_WIFI_DISCONNECTED`}
		- `CSIFW_WIFI_DISCONNECTED`: If wifi monitoring is done by clinet then client should call stop api with `CSIFW_WIFI_DISCONNECTED` reason when wifi disconnects.
		- `CSIFW_NORMAL`: If client wants to stop the service normally.
<br>

5. __csi_service_deinit__: Call `csi_service_deinit` to deinit csi service.

		CSIFW_RES csi_service_deinit();

<br>

6. __csi_get_ap_mac_addr__: Call `csi_get_ap_mac_addr` to get mac address of board.

		CSIFW_RES csi_get_ap_mac_addr(csifw_mac_info *mac_info);
