#ifndef __CSIFW_API_H__
#define __CSIFW_API_H__

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
	extern "C" {
#endif

#include <tinyara/config.h>
#include <tinyara/wifi/rtk/wifi_structures.h>
#include <tinyara/wifi/rtk/wifi_constants.h>


typedef enum {
	CSIFW_ERROR_WIFI_NOT_CONNECTED = -7,	/* WIFI WIFI_DISCONNECTED */
	CSIFW_ERROR_WIFI_DIS_CONNECTED = -6,	/* WIFI WIFI_DISCONNECTED */
	CSIFW_INVALID_RAWDATA = -5,	/* Invalid Raw Data */
	CSIFW_INVALID_ARG = -4,	/* Invalid argument */
	CSIFW_NOT_ENOUGH_SPACE = -3,	/* read/write/other buffer has empty space less than required size */
	CSIFW_NO_MEM = -2,		/* Memory allocation (malloc/calloc) failed */
	CSIFW_ERROR = -1,		/* ERROR: All other types of error not specified by any following enum */
	CSIFW_OK = 0			/* OK: Without any error  */
} CSIFW_RES;

typedef enum {
	CSIFW_WIFI_DISCONNECTED = -1,
	CSIFW_NORMAL = 0
} CSIFW_REASON;

typedef enum {
   BTYPE_RAW = 1,
   BTYPE_PARSED = 2,
   BTYPE_ALL = 3
} BUF_TYPE;

typedef struct {
	char mac_addr[6];
} csifw_mac_info;

/**
 * @brief Callback function type for getting an empty data buffer to which data will be written. this will avoid memcpy at csifw side.
 *
 * This callback function is used by the CSI service to get an empty data buffer for storing the CSI data. The CSI service will call this callback function when it needs an empty buffer to store the CSI data.
 *
 * @param[in] buftype Type of the buffer required.
 * @param[out] buffptr Pointer to the empty buffer.
 * @param[in] data_size Size of the data to be stored in the buffer.
 */
typedef void (*get_empty_data_buffer)(BUF_TYPE buftype, void **buffptr, int data_size);

/**
 * @brief Callback function for receiving raw CSI data.
 *
 * This callback function is used by the CSI service to send the raw CSI data to the application. The CSI service will call this callback function when it has collected raw CSI data.
 *
 * @param[in] status Status of the operation. Check status == CSIFW_OK before using csi_raw_buff. status == CSIFW_ERROR_WIFI_DIS_CONNECTED, if network is disconnected.
 * @param[in] csi_data_len Length of the raw CSI data.
 * @param[in] csi_raw_buff Pointer to the raw CSI data buffer.
 */
typedef void (*upd_raw_data_listener)(CSIFW_RES status, int csi_data_len, unsigned char *csi_raw_buff, void* ptr); 

/**
 * @brief Callback function type for receiving parsed CSI data.
 *
 * This callback function is used by the CSI service to send the parsed CSI data to the application. The CSI service will call this callback function when it has collected parsed CSI data.
 *
 * @param[in] status Status of the operation. Check status == CSIFW_OK before using csi_parsed_buff. status == CSIFW_ERROR_WIFI_DIS_CONNECTED, if network is disconnected 
 * @param[in] csi_data_len Length of the parsed CSI data.
 * @param[in] csi_parsed_buff Pointer to the parsed CSI data buffer.
 */
typedef void (*upd_parsed_data_listener)(CSIFW_RES status, int csi_data_len, float *csi_parsed_buff, void* ptr);

/**
 * @brief Initializes the CSI Manager
 */
CSIFW_RES csi_manager_init(void);

/**
 * @brief Initializes the CSI service.
 * It should be called before calling any other CSI service APIs.
 *
 * @param[in] get_buf_cb Callback function for getting an empty data buffer.
 * @param[in] raw_callback Callback function for receiving raw CSI data. Pass NULL if raw csi data not required.
 * @param[in] parsed_callback Callback function for receiving parsed CSI data. Pass NULL if parsed csi data not required.
 * @param[in] interval Data collection interval in microseconds.
 *
 * @return #CSIFW_OK on success, otherwise an error code.
 */
CSIFW_RES csi_service_init(get_empty_data_buffer get_buf_cb, upd_raw_data_listener raw_callback, upd_parsed_data_listener parsed_callback, unsigned int interval, void* ptr);

/**
 * @brief Sets the configuration parameters for the CSI service.
 *
 * These parameters control various aspects of the CSI service, such as the number of antennas, accuracy, mode  of CSI data, etc.
 *
 * @param[in] config Configuration parameters for the CSI service.
 *
 * @return #CSIFW_OK on success, otherwise an error code.
 */
CSIFW_RES csi_service_set_config(rtw_csi_action_parm_t config);

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
 * @brief Sets the data collection interval for the CSI service.
 *
 * The CSI service will collect CSI data at this interval.
 *
 * @param[in] interval Data collection interval in microseconds.
 *
 * @return #CSIFW_OK on success, otherwise an error code.
 */
CSIFW_RES csi_set_data_collect_interval(unsigned int interval);

/**
 * @brief Starts the CSI service.
 *
 * The CSI service will start collecting CSI data and sending it to the application through the registered callback functions.
 *
 * @return #CSIFW_OK on success, otherwise an error code.
 */
CSIFW_RES csi_service_start();

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
CSIFW_RES csi_service_deinit();

#ifdef __cplusplus /* If this is a C++ compiler, end C linkage */
	}
#endif
#endif /* __CSIFW_API_H__ */
