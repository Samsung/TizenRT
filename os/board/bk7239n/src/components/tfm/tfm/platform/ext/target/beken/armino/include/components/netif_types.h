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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief BK Netif API
 * @addtogroup bk_api_netif BK Netif API
 * @{
 */

/**
 * @brief BK Netif typedefs
 * @defgroup bk_api_netif_typedef
 * @ingroup bk_api_netif
 * @{
 */

#define NETIF_IP4_STR_LEN 16      /**< IP4 string length */

#define BK_ERR_NETIF_IF      (BK_ERR_NETIF_BASE - 1)  /**< Invalid interface ID */

typedef enum {
	NETIF_IF_STA = 0,   /**< WiFi STA interface */
	NETIF_IF_AP,        /**< WiFi AP interface */
	NETIF_IF_COUNT,     /**< Number of BK netif */
	NETIF_IF_INVALID,   /**< Invalid BK netif */
} netif_if_t;

typedef enum {
	EVENT_NETIF_GOT_IP4, /**< Netif get IP4 event */
	EVENT_NETIF_DHCP_TIMEOUT, /**get IP4 addr timeout, 20s**/
} netif_event_t;

typedef struct {
	char ip[NETIF_IP4_STR_LEN];        /**< Local IP address. */
	char mask[NETIF_IP4_STR_LEN];      /**< Netmask. */
	char gateway[NETIF_IP4_STR_LEN];   /**< Gateway IP address. */
	char dns[NETIF_IP4_STR_LEN];       /**< DNS server IP address. */
} netif_ip4_config_t;

typedef struct {
	netif_if_t netif_if;               /**< Netif interface ID */
	char ip[NETIF_IP4_STR_LEN];        /**< Local IP address. */
} netif_event_got_ip4_t;

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
