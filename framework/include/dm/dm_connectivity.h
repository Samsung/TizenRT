/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

/**
 * @defgroup Connectivity Connectivity
 * @brief Provides APIs for Connectivity Monitoring
 * @ingroup DM
 * @{
 */

/**
 * @file dm/dm_connectivity.h
 * @brief DM Connectivity APIs
 */

#ifndef DM_CONNECTIVITY_H_
#define DM_CONNECTIVITY_H_

#ifdef __cplusplus
extern "C"
{
#endif
/**
 * @brief Specify the data structure to hold WiFi scanning information.
 * @details The information comprises SSID and BSSID of nearby Access Points (APs),
 * as well as RSSI (wireless received signal strength indicator).
 */
struct dm_scan_info_s {
	char ssid[33];               // 802.11 spec defined unspecified or uint8
	char bssid[18];                 // char string e.g. xx:xx:xx:xx:xx:xx
	int8_t rssi;                    // rssi level of scanned device
	struct dm_scan_info_s *next;
};
typedef struct dm_scan_info_s dm_scan_info_t;

typedef void (*conn_cb)(void);


/**
 * @brief register dm callback of wifi
 * @details @b #include <dm/dm_connectivity.h>
 * @since TizenRT v2.0
 */
void dm_cb_register_init(void);

/**
 * @brief get the rssi of network
 *
 * @details @b #include <dm/dm_connectivity.h>
 * @param[out] rssi current rssi. (the rssi is a negative value)
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v1.0
 */
int dm_conn_get_rssi(int *rssi);

/**
 * @brief get the ip address of network
 *
 * @details @b #include <dm/dm_connectivity.h>
 * @param[out] ipAddr current ip address. (the form is XXX.XXX.XXX.XXX)
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v1.0
 */
int dm_conn_get_address(char *ipAddr);

/**
 * @brief get the current interface of network
 *
 * @details @b #include <dm/dm_connectivity.h>
 * @param[out] interface current interface of network. (the value is a string)
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v1.0
 */
int dm_conn_get_interface(char *interface);

/**
 * @brief get the channel of network
 *
 * @details @b #include <dm/dm_connectivity.h>
 * @param[out] channel current channel of network.
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v1.0
 */
int dm_conn_get_channel(int *channel);

/**
 * @brief get the tx power of network
 *
 * @details @b #include <dm/dm_connectivity.h>
 * @param[out] dbm current tx power of network. (the value is between 0 ~ 30 dbm)
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v1.0
 */
int dm_conn_get_tx_power(int *dbm);

/**
 * @brief set the tx power of network
 *
 * @details @b #include <dm/dm_connectivity.h>
 * @param[in] dbm the power value to set up. (the value is between 0 ~ 30 dbm)
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v1.0
 */
int dm_conn_set_tx_power(const int *dbm);

/**
 * @brief register link up callback function for connectivity event.
 *
 * @details @b #include <dm/dm_connectivity.h>
 * @param[in] cb A pointer to call when a network link up event occurs.
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v1.0
 */
int dm_conn_register_linkup_cb(conn_cb cb);

/**
 * @brief register link down callback function for connectivity event.
 *
 * @details @b #include <dm/dm_connectivity.h>
 * @param[in] cb A pointer to call when a network link down event occurs.
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v1.0
 */
int dm_conn_register_linkdown_cb(conn_cb cb);

/**
 * @brief unregister link up callback function for connectivity event.
 *
 * @details @b #include <dm/dm_connectivity.h>
 * @param[in] cb A pointer to call when a network link up event occurs.
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v1.0
 */
int dm_conn_unregister_linkup_cb(conn_cb cb);

/**
 * @brief unregister link down callback function for connectivity event.
 *
 * @details @b #include <dm/dm_connectivity.h>
 * @param[in] cb A pointer to call when a network link down event occurs.
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @since TizenRT v1.0
 */
int dm_conn_unregister_linkdown_cb(conn_cb cb);

// @cond
/**
 * @brief Perform a WiFi scan over all channels
 *
 * @details @b #include <dm/dm_connectivity.h>
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @internal
 */
int dm_conn_wifi_scan(void);

/**
 * @brief Fetch WiFi scan result
 *
 * @details @b #include <dm/dm_connectivity.h>
 * @param[out] result pointer to WiFi scan structure to hold result.
 * @return On success, 0 is returned. On failure, a negative value is returned.
 * @internal
 */
int dm_conn_get_scan_result(dm_scan_info_t **result);

/**
 * @brief Free WiFi scan result
 *
 * @details @b #include <dm/dm_connectivity.h>
 * @param[out] result pointer to WiFi scan structure that holds result.
 * @return On completion, 0 is returned.
 * @internal
 */
int dm_conn_free_scan_result(dm_scan_info_t **result);

/**
 * @brief Start device as WiFi station and connect to designated Access Point
 *
 * @details @b #include <dm/dm_connectivity.h>
 * @param[in] linkUpEvent callback functions to handle WiFi link being up.
 * @param[in] linkDownEvent callback functions to handle WiFi link being down.
 * @return On completion, 0 is returned. On failure, a negative value is returned.
 * @internal
 */
int dm_conn_wifi_connect(conn_cb linkUpEvent, conn_cb linkDownEvent);

/**
 * @brief Perform DHCP client routine to obtain IP address for WiFi interface
 *
 * @details @b #include <dm/dm_connectivity.h>
 * @return On completion, 0 is returned. On failure, a negative value is returned.
 * @internal
 */
int dm_conn_dhcp_init(void);

/**
 * @brief Mark WiFi state as disconnected
 *
 * @details @b #include <dm/dm_connectivity.h>
 * @return On completion, 0 is returned. On failure, a negative value is returned.
 * @internal
 */
int dm_conn_wifi_disconnect(void);
// @endcond
#ifdef __cplusplus
}
#endif

#endif /* DM_CONNECTIVITY_H_ */

/** @} */ // end of Connectivity group
