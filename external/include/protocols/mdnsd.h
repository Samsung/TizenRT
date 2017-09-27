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
/*
 * tinysvcmdns - a tiny MDNS implementation for publishing services
 * Copyright (C) 2011 Darell Tan
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
 * @defgroup MDNS Multicast DNS
 *   Multicast Domain Name System
 * @ingroup NETWORK
 * @{
 */
/**
 * @file protocols/mdnsd.h
 * @brief APIs for Multicast DNS
 */

#ifndef __MDNSD_H__
#define __MDNSD_H__

#include <stdint.h>

#define MAX_NUMBER_OF_SERVICE_DISCOVERY_RESULT          10
#define MAX_SERVICE_DISCOVERY_TIME_MS          (60 * 1000)

#define MDNS_HOSTNAME_RESOLVER_TIMEOUT_MSEC       (3 * 1000)
#define MDNS_HOSTNAME_RESOLVER_MAX_TRY_COUNT    5
#define MDNS_HOSTNAME_RESOLVER_WAIT_TIME_MSEC   250

#define MDNS_SERVICE_DISCOVERY_MAX_TRY_COUNT    5
#define MDNS_SERVICE_DISCOVERY_WAIT_TIME_MSEC  250

/**
 * @brief Structure of MDNS service information
 */
struct mdns_service_info {
	char *type;
	char *instance_name;
	char *hostname;
	unsigned int ipaddr;		/* ipv4 */
	unsigned int port;
};

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

#if defined(CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT)

/**
 * @brief mdnsd_start() starts the MDNS daemon.
 *
 * @param[in] desired_hostname the desired host name as string type. if same name is in the network, an alternative name will be set as hostname.
 * @param[in] netif_name network interface name as string type
 * @return On success, 0 is returned. On failure, a negative value is returned.
 *
 */
int mdnsd_start(const char *desired_hostname, const char *netif_name);

/**
 * @brief mdnsd_stop() stops the MDNS daemon.
 *
 * @return On success, 0 is returned. On failure, a negative value is returned.
 *
 */
int mdnsd_stop(void);

/**
 * @brief mdnsd_get_hostname() gets the current host name as MDNS type.
 *
 * @param[out] hostname_result 32-bytes string buffer for the host name result
 * @return On success, 0 is returned. On failure, a negative value is returned.
 *
 */
int mdnsd_get_hostname(char *hostname_result);

/**
 * @brief mdnsd_register_service() register a service to expose through mDNS-SD.
 *
 * @param[in] instance instance name to expose
 * @param[in] type type of service. e.g. _http._tcp.local
 * @param[in] port port to which the service is reachable
 * @param[in] hostname if NULL, use the hostname configured when starting the daemon,
 *                     or use this parameter otherwise
 * @param[in] txt text records to add to the service announcement. Can be NULL.
 * @return On success, 0 is returned. On failure, a negative errno value is returned.
 *
 */
int mdnsd_register_service(const char *instance, const char *type,
				uint16_t port, const char *hostname, const char *txt[]);

#endif /* CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT */

/**
 * @brief mdnsd_resolve_hostname() gets ip address with the given hostname.
 *
 * @param[in] hostname host name as string type
 * @param[out] ipaddr the pointer of ip address result
 * @return On success, 0 is returned. On failure, a negative value is returned.
 *
 */
int mdnsd_resolve_hostname(char *hostname, int *ipaddr);

/**
 * @brief mdnsd_discover_service() discovers services with the given service type string
 *
 * @param[in] service_type mdns service type string
 * @param[in] discover_time_msec time in milliseconds for discovering service
 * @param[out] service_list the array of service list
 * @param[out] num_of_services number of services
 * @return On success, 0 is returned. On failure, a negative value is returned.
 *
 */
int mdnsd_discover_service(char *service_type, int discover_time_msec, struct mdns_service_info **service_list, int *num_of_services);


#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/*!__MDNSD_H__ */

/** @} */// end of MDNS group
