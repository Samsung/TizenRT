/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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

#ifndef __CSIFW_API_H__
#define __CSIFW_API_H__

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
	extern "C" {
#endif

#include <tinyara/config.h>
#include <tinyara/wifi_csi/wifi_csi_struct.h>

typedef enum {
	CSIFW_ERROR_CLIENT_NOT_REGISTERED = -8,    /* Client not registered. Cannot start/stop/deinit */
	CSIFW_ERROR_WIFI_NOT_CONNECTED = -7,       /* WIFI not connected */
	CSIFW_ERROR_WIFI_DIS_CONNECTED = -6,       /* WIFI WIFI_DISCONNECTED */
	CSIFW_INVALID_RAWDATA = -5,                /* Invalid Raw Data */
	CSIFW_INVALID_ARG = -4,                    /* Invalid argument */
	CSIFW_NOT_ENOUGH_SPACE = -3,               /* read/write/other buffer has empty space less than required size */
	CSIFW_NO_MEM = -2,                         /* Memory allocation (malloc/calloc) failed */
	CSIFW_ERROR = -1,                          /* ERROR: All other types of error not specified by any following enum */
	CSIFW_OK = 0,                              /* OK: Without any error  */
	CSIFW_OK_WIFI_CONNECTED = 1                /* OK: Without any error  */
} CSIFW_RES;

typedef enum {
	CSIFW_WIFI_DISCONNECTED = -1,
	CSIFW_NORMAL = 0
} CSIFW_REASON;

/**
 * @brief Callback function for receiving raw CSI data.
 *
 * This callback function is used by the CSI service to send the raw CSI data to the application. The CSI service will call this callback function when it has collected raw CSI data.
 *
 * @param[in] status Status of the operation. Check status == CSIFW_OK before using csi_raw_buff. status == CSIFW_ERROR_WIFI_DIS_CONNECTED, if network is disconnected.
 * @param[in] csi_data_len Length of the raw CSI data.
 * @param[in] csi_raw_buff Pointer to the raw CSI data buffer.
 */
typedef void (*client_raw_data_listener)(CSIFW_RES status, int csi_data_len, unsigned char *csi_raw_buff, void* ptr); 

/**
 * @brief Callback function type for receiving parsed CSI data.
 *
 * This callback function is used by the CSI service to send the parsed CSI data to the application. The CSI service will call this callback function when it has collected parsed CSI data.
 *
 * @param[in] status Status of the operation. Check status == CSIFW_OK before using csi_parsed_buff. status == CSIFW_ERROR_WIFI_DIS_CONNECTED, if network is disconnected 
 * @param[in] csi_data_len Length of the parsed CSI data.
 * @param[in] csi_parsed_buff Pointer to the parsed CSI data buffer.
 */
typedef void (*client_parsed_data_listener)(CSIFW_RES status, int csi_data_len, float *csi_parsed_buff, void* ptr);

/**
 * @brief Initializes the CSI service.
 * It should be called before calling any other CSI service APIs.
 * 
 * @note The CSI data buffer pointer is shared in service's callback, the service must memcpy data and release the callback.
 * 
 * 
 * @param[in] config_type Configuration type for the CSI service. These parameters control various aspects of the CSI service, such as the number of antennas, accuracy, mode  of CSI data, etc.
 * @param[in] raw_callback Callback function for receiving raw CSI data. Pass NULL if raw csi data not required.
 * @param[in] parsed_callback Callback function for receiving parsed CSI data. Pass NULL if parsed csi data not required.
 * @param[in] interval Data collection interval in ms. For Non-HT data Interval range (3.2 ~ 500 ms), HT data Interval range (30ms ~ 1000ms)
 * @param[in] ptr An optional pointer that can be used to pass additional context data to the callbacks.
 *
 * @return #CSIFW_OK on success, otherwise an error code.
 */
CSIFW_RES csi_service_init(csi_config_type_t config_type, client_raw_data_listener raw_callback, client_parsed_data_listener parsed_callback, unsigned int interval, void* ptr);

/**
 * @brief Get MAC address of the connected AP.
 *
 * struct csifw_mac_info object is provided as argument.
 *
 * @param[out] mac_info mac_addr is updated with current AP mac address.
 *
 * @return #CSIFW_OK on success, otherwise an error code.
 */
CSIFW_RES csi_get_ap_mac_addr(csifw_mac_info *mac_info);

/**
 * @brief Starts the CSI service.
 *
 * The CSI service will start collecting CSI data and sending it to the application through the registered callback functions.
 *
 * @return #CSIFW_OK on success, otherwise an error code.
 */
CSIFW_RES csi_service_start(void);

/**
 * @brief Stops the CSI service.
 *
 * The CSI service will stop collecting CSI data and sending it to the application.
 *
 * @return #CSIFW_OK on success, otherwise an error code.
 */
CSIFW_RES csi_service_stop(CSIFW_REASON reason);

/**
 * @brief Deinitializes the CSI service.
 *
 * It should be called after stopping the CSI service and before exiting the application.
 *
 * @return #CSIFW_OK on success, otherwise an error code.
 */
CSIFW_RES csi_service_deinit(void);

/**
 * @brief Change CSI Data collection interval in ms.
 *
 * It should be called after CSI service Init and start state of csi.
 *
 * @param[in] interval Data collection interval in ms. For Non-HT data Interval range (3.2 ~ 500 ms), HT data Interval range (30ms ~ 1000ms)
 *
 * @return #CSIFW_OK on success, otherwise an error code.
 */
CSIFW_RES csi_service_change_interval(unsigned int interval);

/**
 * @brief Get Current interval of CSI Data collection in ms.
 *
 * It should be called after CSI service Init state of csi.
 *
 * @return Current CSI Data collection interval in ms.
 */
unsigned int csi_service_get_current_interval(void);


#ifdef __cplusplus /* If this is a C++ compiler, end C linkage */
	}
#endif
#endif /* __CSIFW_API_H__ */

