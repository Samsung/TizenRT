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
 * @defgroup DHCP DHCP
 * @ingroup NETWORK
 * @{
 */

/**
 * @file net/dhcp.h
 * @brief APIs for DHCP client
 */
#ifndef LWIP_HDR_DHCP_H
#define LWIP_HDR_DHCP_H

#include <net/lwip/opt.h>

#if LWIP_DHCP					/* don't build if not configured for use in lwipopts.h */

#include <net/lwip/netif.h>
#include <net/lwip/udp.h>

#ifdef __cplusplus
extern "C" {
#endif

/** period (in seconds) of the application calling dhcp_coarse_tmr() */
#define DHCP_COARSE_TIMER_SECS  60
/** period (in milliseconds) of the application calling dhcp_coarse_tmr() */
#define DHCP_COARSE_TIMER_MSECS (DHCP_COARSE_TIMER_SECS * 1000UL)
/** period (in milliseconds) of the application calling dhcp_fine_tmr() */
#define DHCP_FINE_TIMER_MSECS   500

#define DHCP_BOOT_FILE_LEN      128U

/* AutoIP cooperation flags (struct dhcp.autoip_coop_state) */
typedef enum {
	DHCP_AUTOIP_COOP_STATE_OFF = 0,
	DHCP_AUTOIP_COOP_STATE_ON = 1
} dhcp_autoip_coop_state_enum_t;

struct dhcp {
	/** transaction identifier of last sent request */
	u32_t xid;
	/** incoming msg */
	struct dhcp_msg *msg_in;
	/** track PCB allocation state */
	u8_t pcb_allocated;
	/** current DHCP state machine state */
	u8_t state;
/** retries of current request */
	u8_t tries;
#if LWIP_DHCP_AUTOIP_COOP
	u8_t autoip_coop_state;
#endif
	u8_t subnet_mask_given;

	struct pbuf *p_out;		/* pbuf of outcoming msg */
	struct dhcp_msg *msg_out;	/* outgoing msg */
	u16_t options_out_len;	/* outgoing msg options length */
	u16_t request_timeout;	/* #ticks with period DHCP_FINE_TIMER_SECS for request timeout */
	u16_t t1_timeout;		/* #ticks with period DHCP_COARSE_TIMER_SECS for renewal time */
	u16_t t2_timeout;		/* #ticks with period DHCP_COARSE_TIMER_SECS for rebind time */
	u16_t t1_renew_time;	/* #ticks with period DHCP_COARSE_TIMER_SECS until next renew try */
	u16_t t2_rebind_time;	/* #ticks with period DHCP_COARSE_TIMER_SECS until next rebind try */
	u16_t lease_used;		/* #ticks with period DHCP_COARSE_TIMER_SECS since last received DHCP ack */
	u16_t t0_timeout;		/* #ticks with period DHCP_COARSE_TIMER_SECS for lease time */
	ip_addr_t server_ip_addr;	/* dhcp server address that offered this lease (ip_addr_t because passed to UDP) */
	ip4_addr_t offered_ip_addr;
	ip4_addr_t offered_sn_mask;
	ip4_addr_t offered_gw_addr;

	u32_t offered_t0_lease;	/* lease period (in seconds) */
	u32_t offered_t1_renew;	/* recommended renew time (usually 50% of lease period) */
	u32_t offered_t2_rebind;	/* recommended rebind time (usually 87.5 of lease period)  */
#if LWIP_DHCP_BOOTP_FILE
	ip4_addr_t offered_si_addr;
	char boot_file_name[DHCP_BOOT_FILE_LEN];
#endif	/* LWIP_DHCP_BOOTPFILE */
};

void dhcp_set_struct(struct netif *netif, struct dhcp *dhcp);
/** Remove a struct dhcp previously set to the netif using dhcp_set_struct() */
#define dhcp_remove_struct(netif) do { (netif)->dhcp = NULL; } while (0)
/**
 * @brief Removes a struct dhcp from a netif.
 *
 * ATTENTION: Only use this when not using dhcp_set_struct() to allocate the
 *            struct dhcp since the memory is passed back to the heap.
 *
 * @param netif the netif from which to remove the struct dhcp
 * @return none
 * @since TizenRT v1.0
*/
void dhcp_cleanup(struct netif *netif);

/** start DHCP configuration */
/**
 * @brief If no DHCP client instance was attached to this interface,
 *	a new client is created first. If a DHCP client instance was already present,
 *	it restarts negotiation.
 *
 * @param netif The lwIP network interface
 * @return lwIP error code
 * - ERR_OK - No error
 * - ERR_MEM - Out of memory
 * @since TizenRT v1.0
*/
err_t dhcp_start(struct netif *netif);

/// @cond
/** enforce early lease renewal (not needed normally)*/
err_t dhcp_renew(struct netif *netif);
/// @endcond

/** release the DHCP lease, usually called before dhcp_stop()*/
/**
 * @brief Release a DHCP lease.
 *
 * @param etif network interface which must release its lease
 * @return lwIP error code
 * - ERR_OK - No error
 * - ERR_MEM - Out of memory
 * @since TizenRT v1.0
*/
err_t dhcp_release(struct netif *netif);

/** stop DHCP configuration */
/**
 * @brief Remove the DHCP client from the interface.
 *
 * @param netif The network interface to stop DHCP on
 * @return none
 * @since TizenRT v1.0
*/
void dhcp_stop(struct netif *netif);

/// @cond
/** inform server of our manual IP address */
void dhcp_inform(struct netif *netif);
/** Handle a possible change in the network configuration */
void dhcp_network_changed(struct netif *netif);

/** if enabled, check whether the offered IP address is not in use, using ARP */
#if DHCP_DOES_ARP_CHECK
void dhcp_arp_reply(struct netif *netif, const ip4_addr_t * addr);
#endif
u8_t dhcp_supplied_address(const struct netif *netif);
err_t dhcp_address_valid(struct netif *netif);
/* to be called every minute */
void dhcp_coarse_tmr(void);
/* to be called every half second */
void dhcp_fine_tmr(void);

#if LWIP_DHCP_GET_NTP_SRV
/** This function must exist, in other to add offered NTP servers to
 * the NTP (or SNTP) engine.
 * See LWIP_DHCP_MAX_NTP_SERVERS
 */
extern void dhcp_set_ntp_servers(u8_t num_ntp_servers, const ip4_addr_t *ntp_server_addrs);
#endif /* LWIP_DHCP_GET_NTP_SRV */

#define netif_dhcp_data(netif) ((struct dhcp*)netif_get_client_data(netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP))

/** Function prototype for dhcps link callback */
typedef void (*dhcp_sta_joined)(void);

/* Register user callback for IP assign */
err_t dhcps_register_cb(dhcp_sta_joined dhcp_join_cb);

/**
 * @brief Starts DHCP server of network interface.
 *       If no DHCP server instance was attached to this interface, a new server is created first.
 *       If a DHCP server instance was already present, it will continue its negotiation.
 *
 * @internal
 * @param[in] netif generic data structure used for all lwIP network interfaces
 * @note netif parameter must not be null
 * @return On success, ERR_OK. On failure, returns error @ref err_t
*/
err_t dhcps_start(struct netif *netif);
/**
 * @brief dhcps_stop() stops DHCP server of network interface
 *
 * @internal
 * @param[in] netif generic data structure used for all lwIP network interfaces
 * @note netif parameter must not be null
 * @return none
*/
void dhcps_stop(struct netif *netif);
/// @endcond

#ifdef __cplusplus
}
#endif
#endif							/* LWIP_DHCP */
#endif							/*LWIP_HDR_DHCP_H */
