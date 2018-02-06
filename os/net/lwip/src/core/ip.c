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
#include <net/lwip/opt.h>

#if LWIP_IPV4 || LWIP_IPV6

#include <net/lwip/ip_addr.h>
#include <net/lwip/ip.h>

/** Global data for both IPv4 and IPv6 */
struct ip_globals ip_data;

#if LWIP_IPV4 && LWIP_IPV6

const ip_addr_t ip_addr_any_type = IPADDR_ANY_TYPE_INIT;

/**
 * @ingroup ipaddr
 * Convert IP address string (both versions) to numeric.
 * The version is auto-detected from the string.
 *
 * @param cp IP address string to convert
 * @param addr conversion result is stored here
 * @return 1 on success, 0 on error
 */
int ipaddr_aton(const char *cp, ip_addr_t *addr)
{
	if (cp != NULL) {
		const char *c;
		for (c = cp; *c != 0; c++) {
			if (*c == ':') {
				/* contains a colon: IPv6 address */
				if (addr) {
					IP_SET_TYPE_VAL(*addr, IPADDR_TYPE_V6);
				}
				return ip6addr_aton(cp, ip_2_ip6(addr));
			} else if (*c == '.') {
				/* contains a dot: IPv4 address */
				break;
			}
		}
		/* call ip4addr_aton as fallback or if IPv4 was found */
		if (addr) {
			IP_SET_TYPE_VAL(*addr, IPADDR_TYPE_V4);
		}
		return ip4addr_aton(cp, ip_2_ip4(addr));
	}
	return 0;
}

/**
 * @ingroup lwip_nosys
 * If both IP versions are enabled, this function can dispatch packets to the correct one.
 * Don't call directly, pass to netif_add() and call netif->input().
 */
err_t ip_input(struct pbuf *p, struct netif *inp)
{
	if (p != NULL) {
		if (IP_HDR_GET_VERSION(p->payload) == 6) {
			return ip6_input(p, inp);
		}
		return ip4_input(p, inp);
	}
	return ERR_VAL;
}

#endif	/* LWIP_IPV4 && LWIP_IPV6 */

#endif	/* LWIP_IPV4 || LWIP_IPV6 */
