// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <common/bk_err.h>
#include <components/netif_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief BK Netif API
 * @defgroup bk_api_netif BK Netif API
 * @{
 */

/**
 * @brief  Initialize the TCP/IP stack
 *
 * @attention This API should be called exactly once from application code, when the application starts up.
 * @return
 *   - BK_OK: success
 *   - BK_FAIL: otherwise
 */
bk_err_t bk_netif_init(void);

/**
 * @brief  Set netif interface's IP4 address information
 *
 * This function is mainly used to set a static IP on an interface.
 *
 * @attention 1. For STA, if this API never calls, it gets the IP address via DHCP by default.
 * @attention 2. If the interface is already up, we need to stop the interface first
 *               before calling this API, or restart the interface after the this API
 *               is called.
 * @attention 3. Currently no sanity checking for the IP4 address to be configured,
 *               also no checking about the relationship between ip/mask/gateway/dns,
 *               the API caller need to guarantee that the correctness of the
 *               configured info.
 * @attention 4. Avoid configuring same subnet for STA and AP.
 *
 * @param ifx The interface index
 * @param config the IP4 information of the interface
 *
 * @return
 *   - BK_OK: succeed
 *   - BK_ERR_NETIF_IF: invalid netif interface ID
 *   - BK_ERR_NULL_PARAM: config is NULL
 */
bk_err_t bk_netif_set_ip4_config(netif_if_t ifx, const netif_ip4_config_t *config);

/**
 * @brief  Get netif interface's IP address information
 *
 * If the interface is up, IP information is read directly from the TCP/IP stack.
 * If the interface is down, the API just return all 0 information
 *
 * @param ifx  netif interface ID
 * @param config store the IP4 configuration info
 *
 * @return
 *   - BK_OK: succeed
 *   - BK_ERR_NETIF_IF: invalid netif interface ID
 *   - BK_ERR_NULL_PARAM: config is NULL
 */
bk_err_t bk_netif_get_ip4_config(netif_if_t ifx, netif_ip4_config_t *config);

/**
 * @brief  Get netif interface's IP6 address information
 *
 * If the interface is up, IP6 information is read directly from the TCP/IP stack.
 * If the interface is down, the API just return all 0 information
 *
 * @param ifx  netif interface ID
 *
 * @return
 *   - BK_OK: succeed
 *   - BK_ERR_NETIF_IF: invalid netif interface ID
 *   - BK_ERR_NULL_PARAM: config is NULL
 */
bk_err_t bk_netif_get_ip6_addr_info(netif_if_t ifx);

/**
 * @brief  Start the DHCP client for specified interface
 *
 * If this API is called, the interface will get the IP address via DHCP.
 *
 * If the interface is has a static IP and is already up, we need to stop
 * the interface (for STA, call bk_wifi_sta_stop() or bk_wifi_sta_disconnect()
 * to stop the interface) before this API is called, or restart the interface
 * after this API is called.
 *
 * @param ifx  netif interface ID, currently only support NETIF_IF_STA.
 *
 * @return
 *   - BK_OK: succeed
 *   - BK_ERR_NETIF_IF: invalid netif interface ID
 */
bk_err_t bk_netif_dhcpc_start(netif_if_t ifx);

/**
 * @brief
 *
 * this function is only for the interface of NETIF_IF_STA to set a static IP .
 *
 * call back this function in netif_wifi_event_cb()(in components/bk_netif/bk_netif.c)
 * when case EVENT_WIFI_STA_CONNECTED, and pass the param static_ip4_config includes ip,
 * mask, gateway and dns for your static IP config.
 * example:
 * switch (event_id) {
 * case EVENT_WIFI_STA_CONNECTED:
 * #if CONFIG_LWIP
 *     bk_netif_static_ip(static_ip4_config);
 *     sta_ip_start();
 * #endif
 *
 * @param static_ip4_config  ipv4 config for this netif(only for NETIF_IF_STA).
 *
 * @return
 *   - BK_OK: succeed
 *   - else: fail
 */
bk_err_t bk_netif_static_ip(netif_ip4_config_t static_ip4_config);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
