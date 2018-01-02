/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

#include "dev.h"
#include "hip.h"
#include "mgt.h"
#include "debug_scsc.h"
#include "udi.h"
#include "hydra.h"
#include "t20_ops.h"
#include "netif.h"
#include "ba.h"

#include "sap_mlme.h"
#include "sap_ma.h"
#include "sap_dbg.h"
#include "sap_test.h"
#include <scsc/scsc_mx.h>

extern int mx_mmap_init(void);
extern void mx_mmap_deinit(void);

void slsi_driver_initialize(void)
{
	platform_mif_module_probe();
	scsc_mx_module_init();
	slsi_dev_load();
}

static void slsi_regd_init(struct slsi_dev *sdev)
{
	struct slsi_802_11d_reg_domain *slsi_world_regdom_custom = sdev->device_config.domain_info;
	struct slsi_wlan_reg_rule reg_rules[] = {
		/* Channel 1 - 11 */
		SLSI_REG_RULE(2412 - 10, 2462 + 10, 20, 0, 20, 0),
		/* Channel 12 - 13 NO_IR */
		SLSI_REG_RULE(2467 - 10, 2472 + 10, 20, 0, 20, SLSI_REG_RULE_FLAG_NO_IR),
		/* Channel 14 */
		SLSI_REG_RULE(2484 - 10, 2484 + 10, 20, 0, 20, (SLSI_REG_RULE_FLAG_PASSIVE_SCAN | SLSI_REG_RULE_FLAG_NO_OFDM)),
#ifdef CONFIG_SCSC_ADV_FEATURE
		/* Channel 36 - 48 */
		SLSI_REG_RULE(5180 - 10, 5240 + 10, 80, 0, 20, 0),
		/* Channel 149 - 165 */
		SLSI_REG_RULE(5745 - 10, 5825 + 10, 80, 0, 20, 0),
		/* Channel 52 - 64 */
		SLSI_REG_RULE(5260 - 10, 5320 + 10, 80, 0, 20, SLSI_REG_RULE_FLAG_DFS),
		/* Channel 100 - 140 */
		SLSI_REG_RULE(5500 - 10, 5700 + 10, 80, 0, 20, SLSI_REG_RULE_FLAG_DFS),
#endif
	};
	int i;

	SLSI_DBG1_NODEV(SLSI_INIT_DEINIT, "regulatory init\n");

	SLSI_DBG1(sdev, SLSI_INIT_DEINIT, "chip ver=Maxwell, chan supp=2.4 GHz");
#ifdef CONFIG_SCSC_ADV_FEATURE
	slsi_world_regdom_custom->n_reg_rules = 6;
#else
	slsi_world_regdom_custom->n_reg_rules = 2;
#endif
	for (i = 0; i < slsi_world_regdom_custom->n_reg_rules; i++) {
		slsi_world_regdom_custom->reg_rules[i] = reg_rules[i];
	}

	/* Country code '00' indicates world regulatory domain */
	slsi_world_regdom_custom->alpha2[0] = '0';
	slsi_world_regdom_custom->alpha2[1] = '0';

	sisi_update_supported_channels(sdev);
}

static void slsi_t20_drv_init(struct slsi_dev *sdev)
{
	struct slsi_t20_drv *drv;

	drv = kmm_zalloc(sizeof(struct slsi_t20_drv));
	if (!drv) {
		SLSI_ERR_NODEV("Failed to allocate drv\n");
		return;
	}

	drv->capa.flags = sdev->flags;
	drv->capa.key_mgmt = WPA_DRIVER_CAPA_KEY_MGMT_WPA | WPA_DRIVER_CAPA_KEY_MGMT_WPA_PSK | WPA_DRIVER_CAPA_KEY_MGMT_WPA2 | WPA_DRIVER_CAPA_KEY_MGMT_WPA2_PSK;

	drv->capa.auth = WPA_DRIVER_AUTH_OPEN | WPA_DRIVER_AUTH_SHARED;

	drv->capa.flags |= WPA_DRIVER_FLAGS_SANE_ERROR_CODES;
	drv->capa.flags |= WPA_DRIVER_FLAGS_SET_KEYS_AFTER_ASSOC_DONE;
	drv->capa.flags |= WPA_DRIVER_FLAGS_EAPOL_TX_STATUS;

	drv->capa.enc = WPA_DRIVER_CAPA_ENC_WEP40 | WPA_DRIVER_CAPA_ENC_WEP104 | WPA_DRIVER_CAPA_ENC_TKIP | WPA_DRIVER_CAPA_ENC_CCMP | WPA_DRIVER_CAPA_ENC_WEP128;

	drv->capa.max_remain_on_chan = 5000;
	drv->capa.wmm_ac_supported = 1;

	drv->capa.flags |= WPA_DRIVER_FLAGS_AP_TEARDOWN_SUPPORT;
	drv->capa.max_scan_ssids = sdev->max_scan_ssids;
	drv->capa.max_sched_scan_ssids = sdev->max_sched_scan_ssids;
	drv->capa.sched_scan_supported = sdev->sched_scan_supported;
	drv->capa.max_match_sets = sdev->max_match_sets;
	drv->capa.max_remain_on_chan = sdev->max_remain_on_chan;
	drv->capa.max_stations = sdev->max_stations;
	drv->capa.probe_resp_offloads = sdev->probe_resp_offloads;
	drv->capa.max_acl_mac_addrs = sdev->max_acl_mac_addrs;
	sdev->drv = drv;
	return;

}

static void slsi_t20_drv_deinit(struct slsi_dev *sdev)
{
	/* Free the drv context allocated during init */
	kmm_free(sdev->drv);
	sdev->drv = NULL;
}

struct slsi_dev *slsi_dev_attach(struct scsc_mx *core, struct scsc_service_client *mx_wlan_client)
{
	struct slsi_dev *sdev;

	SLSI_INFO_NODEV("Add SCSC WLAN Device\n");

	sdev = slsi_new();
	if (sdev == NULL) {
		SLSI_ERR_NODEV("No sdev\n");
		return NULL;
	}
	sdev->mlme_blocked = false;

	SLSI_MUTEX_INIT(sdev->netdev_add_remove_mutex);
	SLSI_MUTEX_INIT(sdev->start_stop_mutex);
	SLSI_MUTEX_INIT(sdev->device_config_mutex);

	sdev->maxwell_core = core;
	memcpy(&sdev->mx_wlan_client, mx_wlan_client, sizeof(struct scsc_service_client));

	sdev->fail_reported = false;
	sdev->device_config.qos_info = 0;
	memset(&sdev->chip_info_mib, 0xFF, sizeof(struct slsi_chip_info_mib));

	sdev->scan_init_24g = true;
	slsi_log_clients_init(sdev);

	sdev->recovery_next_state = 0;
	init_completion(&sdev->recovery_remove_completion);
	init_completion(&sdev->recovery_stop_completion);
	sdev->recovery_status = 0;

	slsi_sig_send_init(&sdev->sig_wait);
	sdev->tx_host_tag = 1;

	if (slsi_mbuf_work_init(sdev, NULL, &sdev->rx_dbg_sap, "slsi_wlan_rx_dbg_sap", slsi_rx_dbg_sap_work) != 0) {
		goto err_if;
	}
	SLSI_INFO_NODEV("Call slsi_netif_init()\n");
	if (slsi_netif_init(sdev) != 0) {
		SLSI_ERR(sdev, "Can not create the network interface\n");
		goto err_ctrl_wq_init;
	}

	if (slsi_hip_init(sdev) != 0) {
		SLSI_ERR(sdev, "slsi_hip_init() Failed\n");
		goto err_netif_init;
	}

	if (slsi_udi_node_init(sdev) != 0) {
		SLSI_ERR(sdev, "failed to init UDI\n");
		goto err_hip_init;
	}

	/* update regulatory domain */
	slsi_regd_init(sdev);

	slsi_rx_ba_init(sdev);
	slsi_t20_drv_init(sdev);

	SLSI_MUTEX_INIT(sdev->rx_data_mutex);

	slsi_alloc_tx_mbuf(sdev);
#ifdef CONFIG_SLSI_WLAN_STATS
	slsi_init_dath_path_stats(sdev);
#endif

	if (slsi_netif_register(sdev, sdev->netdev[SLSI_NET_INDEX_WLAN]) != 0) {
		SLSI_ERR(sdev, "failed to register with wlan netdev\n");
		goto err_netif_registered;
	}
#ifdef CONFIG_SLSI_WLAN_P2P
	if (slsi_netif_register(sdev, sdev->netdev[SLSI_NET_INDEX_P2P]) != 0) {
		SLSI_ERR(sdev, "failed to register with p2p netdev\n");
		goto err_netif_registered;
	}
	if (slsi_netif_register(sdev, sdev->netdev[SLSI_NET_INDEX_P2PX]) != 0) {
		SLSI_ERR(sdev, "failed to register with p2px netdev\n");
		goto err_netif_registered;
	}
#endif
	sdev->device_state = SLSI_DEVICE_STATE_STOPPED;
	sdev->current_tspec_id = -1;
	sdev->tspec_error_code = -1;

	return sdev;

err_netif_registered:
#ifdef CONFIG_SLSI_WLAN_P2P
	slsi_netif_remove(sdev, sdev->netdev[SLSI_NET_INDEX_P2PX]);
	slsi_netif_remove(sdev, sdev->netdev[SLSI_NET_INDEX_P2P]);
#endif
	slsi_netif_remove(sdev, sdev->netdev[SLSI_NET_INDEX_WLAN]);
	slsi_udi_node_deinit(sdev);
err_hip_init:
	slsi_hip_deinit(sdev);

err_netif_init:
	slsi_netif_deinit(sdev);

err_ctrl_wq_init:
	slsi_mbuf_work_deinit(&sdev->rx_dbg_sap);

err_if:
	return NULL;
}

void slsi_dev_detach(struct slsi_dev *sdev)
{
	SLSI_DBG1(sdev, SLSI_INIT_DEINIT, "Remove Device\n");

	slsi_stop(sdev);

	complete_all(&sdev->sig_wait.completion);
	complete_all(&sdev->recovery_remove_completion);
	complete_all(&sdev->recovery_stop_completion);

	sdev->scan_init_24g = false;

	SLSI_DBG2(sdev, SLSI_INIT_DEINIT, "Unregister netif\n");
	slsi_netif_remove_all(sdev);

	SLSI_DBG2(sdev, SLSI_INIT_DEINIT, "De-initialise the UDI\n");
	slsi_log_clients_terminate(sdev);

	slsi_udi_node_deinit(sdev);

	SLSI_DBG2(sdev, SLSI_INIT_DEINIT, "De-initialise Hip\n");
	slsi_hip_deinit(sdev);

	SLSI_DBG2(sdev, SLSI_INIT_DEINIT, "De-initialise netif\n");
	slsi_netif_deinit(sdev);

	slsi_free_tx_mbuf(sdev);
	SLSI_DBG2(sdev, SLSI_INIT_DEINIT, "De-initialise TA\n");

	slsi_regd_deinit(sdev);

	SLSI_DBG2(sdev, SLSI_INIT_DEINIT, "Stop Work Queues\n");
	slsi_mbuf_work_deinit(&sdev->rx_dbg_sap);

	SLSI_DBG2(sdev, SLSI_INIT_DEINIT, "t20_drvier deinit\n");
	slsi_t20_drv_deinit(sdev);

	SLSI_DBG2(sdev, SLSI_INIT_DEINIT, "Cleanup Device Data\n");
	slsi_kfree_mbuf(sdev->device_config.channel_config);
}

int slsi_dev_load(void)
{
	SLSI_INFO_NODEV("Loading Maxwell Wi-Fi driver\n");
#ifdef CONFIG_SCSC_WLANLITE
	SLSI_INFO_NODEV("--- WLANLITE mode ---\n");
#endif

	if (slsi_udi_init()) {
		SLSI_INFO_NODEV("Failed to init udi - continuing\n");
	}
	if (mx_mmap_init()) {
		SLSI_INFO_NODEV("Failed to init gdb_transport - continuing\n");
	}

	if (slsi_sm_service_driver_register()) {
		SLSI_INFO_NODEV("slsi_sm_service_driver_register failed - continuing\n");
	}

	/* Register SAPs */
	sap_mlme_init();
	sap_ma_init();
	sap_dbg_init();
	sap_test_init();

#ifdef CONFIG_SLSI_WLAN_FAPI_LOG
	slsi_wlan_fapi_log_init();
#endif

	SLSI_INFO_NODEV("--- Maxwell Wi-Fi driver loaded successfully ---\n");
	return 0;
}

void slsi_dev_unload(void)
{
	SLSI_INFO_NODEV("Unloading Maxwell Wi-Fi driver\n");

	/* Unregister SAPs */
	sap_mlme_deinit();
	sap_ma_deinit();
	sap_dbg_deinit();
	sap_test_deinit();

	slsi_sm_service_driver_unregister();
	mx_mmap_deinit();
	slsi_udi_deinit();

	SLSI_INFO_NODEV("--- Maxwell Wi-Fi driver unloaded successfully ---\n");
}
