/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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

#pragma once

#ifdef __cplusplus				/* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

#include <tinyara/config.h>
#include <tinyara/wifi_csi/wifi_csi_struct.h>

typedef enum _CSIFW_RES {
	CSIFW_ERROR_MAX_NUM_SERVICE_REGISTERED = -11,		/* ERROR: MAX number of Services is alreay Registered */
	CSIFW_ERROR_NOT_INITIALIZED = -10,			/* ERROR: Not Initialized */
	CSIFW_ERROR_INVALID_SERVICE_STATE = -9,			/* ERROR: Invalid Service State */
	CSIFW_ERROR_ALREADY_INIT_WITH_DIFFERENT_CONFIG = -8,	/* ERROR: Service already initialized with different CSI_Configuration  */
	CSIFW_ERROR_SERVICE_NOT_REGISTERED = -7,		/* ERROR: Service not registered. Cannot start/stop/deinit */
	CSIFW_ERROR_WIFI_DIS_CONNECTED = -6,			/* ERROR: WIFI WIFI_DISCONNECTED */
	CSIFW_INVALID_RAWDATA = -5,				/* Invalid Raw Data */
	CSIFW_INVALID_ARG = -4,					/* Invalid argument */
	CSIFW_NOT_ENOUGH_SPACE = -3,				/* read/write/other buffer has empty space less than required size */
	CSIFW_NO_MEM = -2,					/* Memory allocation (malloc/calloc) failed */
	CSIFW_ERROR = -1,					/* ERROR: All other types of error not specified by any following enum */
	CSIFW_OK = 0,						/* OK: Without any error  */
	CSIFW_OK_WIFI_CONNECTED = 1,				/* OK: Without any error  */
	CSIFW_OK_ALREADY_STOPPED = 2,				/* CSIFW already stopped by running service, but no error */
	CSIFW_OK_ALREADY_STARTED = 3,				/* CSIFW already started by running service, but no error */
	CSIFW_OK_WIFI_NOT_CONNECTED = 4				/* CSIFW already started but WIFI not connected */
} CSIFW_RES;

typedef int csifw_service_handle;

/**
 * @brief Callback function for receiving raw CSI data.
 *
 * This callback function is used by the CSI service to send the raw CSI data to the application. The CSI service will call this callback function when it has collected raw CSI data.
 *
 * @param[in] status Status of the operation. Check status == CSIFW_OK before using csi_raw_buff. status == CSIFW_ERROR_WIFI_DIS_CONNECTED, if network is disconnected.
 * @param[in] csi_data_len Length of the raw CSI data.
 * @param[in] csi_raw_buff Pointer to the raw CSI data buffer.
 */
typedef void (*client_raw_data_listener)(CSIFW_RES status, int csi_data_len, unsigned char *csi_raw_buff, void *ptr);

/**
 * @brief Callback function type for receiving parsed CSI data.
 *
 * This callback function is used by the CSI service to send the parsed CSI data to the application. The CSI service will call this callback function when it has collected parsed CSI data.
 *
 * @param[in] status Status of the operation. Check status == CSIFW_OK before using csi_parsed_buff. status == CSIFW_ERROR_WIFI_DIS_CONNECTED, if network is disconnected
 * @param[in] csi_data_len Length of the parsed CSI data.
 * @param[in] csi_parsed_buff Pointer to the parsed CSI data buffer.
 */
typedef void (*client_parsed_data_listener)(CSIFW_RES status, int csi_data_len, float *csi_parsed_buff, void *ptr);

typedef struct {
	client_parsed_data_listener parsed_data_cb;
	client_raw_data_listener raw_data_cb;
	void *user_data;		//opaque for csifw
} service_callbacks_t;

/**
 * @file CSIManager.c
 * @brief CSI Framework Management API
 * @defgroup csifw_group CSI Framework Manager
 * @{
 */

/**
 * @brief Registers a new CSI service with the framework
 * 
 * Initializes the CSI framework context if not already initialized, sets up network monitoring,
 * packet receiver, and registers client callbacks. Must be called before any other CSI operations.
 * 
 * @param [out] p_hnd Pointer to store the service handle (output)
 * @param [in] p_svc_cb Structure containing raw/parsed data callbacks (input)
 * @param [in] config_type CSI data type (HT_CSI_DATA, NON_HT_CSI_DATA, etc.)
 * @param [in] interval Data collection interval in milliseconds (10-5000ms)
 * 
 * @retval CSIFW_OK Service registered successfully
 * @retval CSIFW_ERROR Mutex init or Network init fail
 * @retval CSIFW_NO_MEM Memory allocation failure
 * @retval CSIFW_INVALID_ARG Invalid config_type or interval
 * @retval CSIFW_ERROR_NOT_INITIALIZED
 * @retval CSIFW_ERROR_MAX_NUM_SERVICE_REGISTERED Maximum services (5) already registered
 * @retval CSIFW_ERROR_ALREADY_INIT_WITH_DIFFERENT_CONFIG Mismatched configuration with existing services
 * 
 * @note First caller determines framework configuration. Subsequent callers must match config_type and interval.
 */
CSIFW_RES csifw_registerService(csifw_service_handle *p_hnd, service_callbacks_t *p_svc_cb, csi_config_type_t config_type, unsigned int interval);

/**
 * @brief Starts CSI data collection for a registered service
 * 
 * Activates data collection threads and ping generator. Automatically starts on Wi-Fi connection
 * if currently disconnected.
 * 
 * @param [in] hnd Service handle from csifw_registerService
 * 
 * @retval CSIFW_OK Data collection started
 * @retval CSIFW_OK_ALREADY_STARTED Service already active
 * @retval CSIFW_OK_WIFI_NOT_CONNECTED Deferred start (will auto-start on connection)
 * @retval CSIFW_ERROR_INVALID_HANDLE Invalid service handle
 * @retval CSIFW_ERROR Framework start failure
 * 
 * @warning Requires Wi-Fi connection for immediate start. Check return code for deferred status.
 */
CSIFW_RES csifw_start(csifw_service_handle hnd);

/**
 * @brief Stops CSI data collection for a service
 * 
 * Deactivates data collection while preserving service registration. Stops framework if last active service.
 * 
 * @param [in] hnd Service handle to stop
 * 
 * @retval CSIFW_OK Service stopped successfully
 * @retval CSIFW_OK_ALREADY_STOPPED Service already inactive
 * @retval CSIFW_ERROR_INVALID_HANDLE Invalid service handle
 * 
 * @note Does not release resources. Use csifw_unregisterService for full cleanup.
 */
CSIFW_RES csifw_stop(csifw_service_handle hnd);

/**
 * @brief Unregisters a CSI service and releases resources
 * 
 * Removes service registration and cleans up framework if last service. Must be called after csifw_stop.
 * 
 * @param [in] hnd Service handle to unregister
 * 
 * @retval CSIFW_OK Service unregistered
 * @retval CSIFW_ERROR_INVALID_SERVICE_STATE Service still active (call csifw_stop first)
 * @retval CSIFW_ERROR_INVALID_HANDLE Invalid service handle
 * @retval CSIFW_ERROR unregistering failed due to internal error.
 * 
 * @warning Invalidates the service handle. Do not use handle after unregistration.
 */
CSIFW_RES csifw_unregisterService(csifw_service_handle hnd);

/**
 * @brief Updates CSI data collection interval
 * 
 * Adjusts packet collection and ping frequency. Affects all services.
 * 
 * @param [in] hnd Service handle (for validation)
 * @param [in] interval New interval in milliseconds (10-5000ms)
 * 
 * @retval CSIFW_OK Interval updated
 * @retval CSIFW_INVALID_ARG Interval out of range
 * @retval CSIFW_ERROR_INVALID_HANDLE Invalid service handle
 * 
 * @note For HT configurations, adjusts ping frequency. For NON_HT, adjusts kernel driver sampling.
 */
CSIFW_RES csifw_set_interval(csifw_service_handle hnd, unsigned int interval);

/**
 * @brief Retrieves current collection interval
 * 
 * @param [in] hnd Service handle (for validation)
 * @param [out] p_interval Pointer to store current interval
 * 
 * @retval CSIFW_OK Interval retrieved
 * @retval CSIFW_INVALID_ARG Null pointer argument
 * @retval CSIFW_ERROR_INVALID_HANDLE Invalid service handle
 */
CSIFW_RES csifw_get_interval(csifw_service_handle hnd, unsigned int *p_interval);

/**
 * @brief Retrieves current CSI configuration type
 * 
 * @param [in] hnd Service handle (for validation)
 * @param [out] config_type Pointer to store config type
 * 
 * @retval CSIFW_OK Config retrieved
 * @retval CSIFW_INVALID_ARG Null pointer argument
 * @retval CSIFW_ERROR_INVALID_HANDLE Invalid service handle
 */
CSIFW_RES csifw_get_config(csifw_service_handle hnd, csi_config_type_t *config_type);

/**
 * @brief Retrieves AP MAC address
 * 
 * Requires active Wi-Fi connection and data collection.
 * 
 * @param [in] hnd Service handle (for validation)
 * @param [out] p_mac_info Buffer for 6-byte MAC address
 * 
 * @retval CSIFW_OK MAC address retrieved
 * @retval CSIFW_INVALID_ARG Null pointer argument
 * @retval CSIFW_ERROR_INVALID_SERVICE_STATE Service not active
 * @retval CSIFW_ERROR_INVALID_HANDLE Invalid service handle
 * 
 * @note MAC info structure: typedef uint8_t csifw_mac_info[6];
 */
CSIFW_RES csifw_get_ap_mac_addr(csifw_service_handle hnd, csifw_mac_info *p_mac_info);

/**
 * @brief Forcefully Re-Start the CSI service.
 *
 * This API will affect all running csi clients. 
 * So, not recommended API for normal stop and then start cases.
 * 
 * When stop() -> start() is called by any csi client service, only caller service data will stop. 
 * But calling this API will stop and then restart data for all other csi clients also. 
 * So needs to be called in case of data is stuck due to some unknown reasons, 
 * which is not reported through data callbacks.
 * If data is stops due to any reason. 
 * Application can to re-start() -> with currect start(), stop() 
 * This can not happen if more than one client services are running, 
 * both needs to stop in sync and then re-start
 * @param[in] csifw_service_handle Handle of Service.
 *
 * @return #CSIFW_OK on success, otherwise an error code.
 */
CSIFW_RES csifw_force_restart(csifw_service_handle hnd);


#ifdef __cplusplus				/* If this is a C++ compiler, end C linkage */
}
#endif
