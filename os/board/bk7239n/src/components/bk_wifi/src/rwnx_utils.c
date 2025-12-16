/**
 ****************************************************************************************
 *
 * @file rwnx_utils.c
 *
 * @brief vif, sta utilities
 *
 * Copyright (C) BEKEN corperation 2021-2022
 *
 ****************************************************************************************
 */
#include "bk_compiler.h"
#include "rw_msdu.h"
#include "bk_rw.h"
#include "rwnx_config.h"
#include "os/str.h"
#include "os/mem.h"
#include "rwnx_defs.h"
#include "lwip/netif.h"

///////////////////////////////////////////////////////////////////////////////
__IRAM2 inline void *rwm_mgmt_vif_idx2ptr(UINT8 vif_idx)
{
	void *vif_entry = NULL;

	if (vif_idx < NX_VIRT_DEV_MAX)
		vif_entry = mac_vif_mgmt_get_entry(vif_idx);

	return vif_entry;
}

void *rwm_mgmt_vif_type2ptr(UINT8 vif_type)
{
	void *vif_entry = NULL;
	UINT32 i;

	for (i = 0; i < NX_VIRT_DEV_MAX; i++) {
		vif_entry = mac_vif_mgmt_get_entry(i);
		if (mac_vif_mgmt_get_type(vif_entry) == vif_type)
			break;
	}

	if (i == NX_VIRT_DEV_MAX)
		vif_entry = NULL;

	return vif_entry;
}

__IRAM2 inline void *rwm_mgmt_sta_idx2ptr(UINT8 staid)
{
	void *sta_entry = NULL;

	if (staid < NX_REMOTE_STA_MAX)
		sta_entry = sta_mgmt_get_entry(staid);

	return sta_entry;
}

void *rwm_mgmt_sta_mac2ptr(void *mac)
{
	UINT32 i;
	void *sta_entry = NULL;

	for (i = 0; i < NX_REMOTE_STA_MAX; i++) {
		sta_entry = sta_mgmt_get_entry(i);
		if (os_memcmp(sta_mgmt_get_mac_addr(sta_entry), mac, ETH_ALEN) == 0)
			break;
	}

	return sta_entry;
}

UINT8 rwm_mgmt_sta_mac2idx(void *mac)
{
	UINT32 i;
	UINT8 staid = 0xff;
	void *sta_entry = NULL;

	for (i = 0; i < NX_REMOTE_STA_MAX; i++) {
		sta_entry = sta_mgmt_get_entry(i);
		if (os_memcmp(sta_mgmt_get_mac_addr(sta_entry), mac, ETH_ALEN) == 0)
			break;
	}
	if (i < NX_REMOTE_STA_MAX)
		staid = i;

	return staid;
}

UINT8 rwm_mgmt_sta_mac2port(void *mac)
{
	UINT32 i;
	void *sta_entry = NULL;

	for (i = 0; i < NX_REMOTE_STA_MAX; i++) {
		sta_entry = sta_mgmt_get_entry(i);
		if (os_memcmp(sta_mgmt_get_mac_addr(sta_entry), mac, ETH_ALEN) == 0)
			break;
	}

	if (sta_entry) {
		if (sta_mgmt_get_ctrl_port_state(sta_entry) == PORT_OPEN)
			return 1;
	}

	return 0;
}

UINT8 rwm_mgmt_vif_mac2idx(void *mac)
{
	return mac_vif_mgmt_mac_to_index(mac);
}

UINT8 rwm_mgmt_get_hwkeyidx(UINT8 vif_idx, UINT8 staid)
{
	return sta_mgmt_get_hwkeyidx(vif_idx, staid);
}

/**
 ****************************************************************************************
 * @brief trace mac tx and rx statistics
 *
 * @param[in] reset_status   check if statistic should be reset
 *
 * @return Void
 *
 ****************************************************************************************
 */


#if CONFIG_NO_HOSTED
UINT8 rwm_mgmt_vif_name2idx(char *name)
{
	VIF_INF_PTR vif_entry = NULL;
	const char *hostname;
	UINT8 vif_idx = 0xff;
	UINT32 i;
	struct netif *netif;

	for (i = 0; i < NX_VIRT_DEV_MAX; i++) {
		vif_entry = mac_vif_mgmt_get_entry(i);
		netif = mac_vif_mgmt_get_priv(vif_entry);
		if (netif) {
			hostname = netif->name;
			if (!os_strncmp(hostname, name, os_strlen(hostname)))
				break;
		}
	}

	if (i < NX_VIRT_DEV_MAX)
		vif_idx = i;

	return vif_idx;
}

#if 0
void rwm_mgmt_set_vif_netif(uint8_t *mac, void *net_if)
{
	VIF_INF_PTR vif_entry = NULL;
	UINT8 vif_idx;

	if (!net_if)
		return;

	vif_idx = rwm_mgmt_vif_mac2idx(mac);
	vif_entry = rwm_mgmt_vif_idx2ptr(vif_idx);

	if (vif_entry) {
		vif_entry->priv = net_if;
		net_if->state = (void *)vif_entry;
	} else
		RWNX_LOGI("warnning: set_vif_netif failed\r\n");
}


struct netif *rwm_mgmt_get_vif2netif(UINT8 vif_idx)
{
	VIF_INF_PTR vif_entry = NULL;
	struct netif *netif = NULL;

	vif_entry = rwm_mgmt_vif_idx2ptr(vif_idx);

	if (vif_entry)
		netif = (struct netif *)vif_entry->priv;

	return netif;
}

UINT8 rwm_mgmt_get_netif2vif(struct netif *netif)
{
	UINT8 vif_idx = 0xff;
	VIF_INF_PTR vif_entry = NULL;

	if (netif && netif->state) {
		vif_entry = (VIF_INF_PTR)netif->state;
		vif_idx = vif_entry->index;
	}

	return vif_idx;
}
#endif
#endif /* CONFIG_NO_HOSTED */

__IRAM2 inline UINT8 rwm_mgmt_tx_get_staidx(UINT8 vif_idx, void *dstmac)
{
	return vif_mgmt_tx_get_staidx(vif_idx, dstmac);
}

UINT8 rwm_get_monitor_vif_idx(void)
{

	return mac_vif_mgmt_get_monitor_vif();
}

UINT8 rwm_first_vif_idx()
{
	VIF_INF_PTR vif = rwm_mgmt_is_vif_first_used();
	if (vif)
		return mac_vif_mgmt_get_index(vif);

	return INVALID_VIF_IDX;
}

u8 rwn_mgmt_is_only_sta_role_add(void)
{
	VIF_INF_PTR vif_entry = (VIF_INF_PTR)rwm_mgmt_is_vif_first_used();

	if (!vif_entry)
		return 0;

	if (mac_vif_mgmt_get_type(vif_entry) == VIF_STA)
		return 1;

	return 0;
}

#if CONFIG_NO_HOSTED
extern uint8_t *dhcp_lookup_mac(uint8_t *chaddr);
#if 0
void rwn_mgmt_show_vif_peer_sta_list(UINT8 role)
{
	struct vif_info_tag *vif = (VIF_INF_PTR)rwm_mgmt_is_vif_first_used();
	struct sta_info_tag *sta;
	UINT8 num = 0;

	while (vif) {
		if (vif->type == role) {
			sta = (struct sta_info_tag *)co_list_pick(&vif->sta_list);
			while (sta != NULL) {
				UINT8 *macptr = (UINT8 *)sta->mac_addr.array;
				UINT8 *ipptr = NULL;

				if (role == VIF_AP)
					ipptr = dhcp_lookup_mac(macptr);
				else if (role == VIF_STA) {
					struct netif *netif = (struct netif *)vif->priv;
					ipptr = (UINT8 *)inet_ntoa(netif->gw);
				}

				RWNX_LOGI("%d: mac:%02x-%02x-%02x-%02x-%02x-%02x, ip:%s\r\n", num++,
						  macptr[0], macptr[1], macptr[2],
						  macptr[3], macptr[4], macptr[5], ipptr);

				sta = (struct sta_info_tag *)co_list_next(&sta->list_hdr);
			}
		}
		vif = (VIF_INF_PTR) rwm_mgmt_next(vif);
	}
}
#endif
#endif /* CONFIG_NO_HOSTED */

UINT8 rwn_mgmt_if_ap_stas_empty(void)
{
	void *vif = rwm_mgmt_is_vif_first_used();
	UINT8 role = VIF_AP;

	while (vif) {
		if (mac_vif_mgmt_get_type(vif) == role) {
			if (mac_vif_mgmt_sta_list_empty(vif))
				return 1;
		}
		vif = (VIF_INF_PTR) rwm_mgmt_next(vif);
	}
	return 0;
}

