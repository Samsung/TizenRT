# A. How to build csifw test app

1. Run command `./os/dbuild.sh menu`.
2. Select board `rtl8730e`
3. Select configuration `loadable_ext_psram`
4. Modify build by selecting `Modify Current Configuration`
5. Enable csifw_test and wm_test app from menuconfig:

		Application Configuration-> Examples-> [*] CSIFW test application
		
		                                       Wifi Manager-> [*] Wi-Fi Manager Sample

6. Turn on CSIFW logs from menuconfig:
		
		CSI Framework-> CSIFW Debug Logs-> [*] CSIFW Logs
		                                   [*]   CSIFW Error Logs
		                                   [ ]   CSIFW Information Logs
		                                   [ ]   CSIFW Debug Logs
		                                   [ ]   CSIFW Verbose Logs 

# B. How to run csifw test app on board

1. Connect board to wifi.

3. Run csifw_test command as shown below:
	
		csifw_test <interval> <config_type>

	Example:

			csifw_test 30 0 // CSI service will start sending HT data at 30ms.
			csifw_test 40 1 // CSI service will start sending NON-HT data at 40ms.
			 
 
<br>
<br>
<hr>

# B. CSI Framework Guide


### Supported CSI Data Configs

CSIFW supports following configurations in STA mode: 
- `HT_CSI_DATA` (56 subcarriers each sub-carrier data is 2 bytes)
- `NON_HT_CSI_DATA` (52 subcarriers each sub-carrier data is 2 bytes)
- `HT_CSI_DATA_ACC1` (56 subcarriers each sub-carrier data is 4 bytes)
- `NON_HT_CSI_DATA_ACC1` (52 subcarriers each sub-carrier data is 4 bytes)
<br>

The config can be set using `CSI_CONFIG_TYPE` enum.




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

		CSIFW_RES csi_service_init(CSI_CONFIG_TYPE config_type, client_raw_data_listener raw_callback, client_parsed_data_listener parsed_callback, unsigned int interval, void* ptr);
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
