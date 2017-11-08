/*****************************************************************************
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
#include "debug_scsc.h"
#include "dev.h"
#include "sap.h"
#include "sap_mlme.h"
#include "hip.h"

#define SUPPORTED_VERSION       13
#define SUPPORTED_OLD_VERSION   0

static int sap_mlme_version_supported(u16 version);
static int sap_mlme_rx_handler(struct slsi_dev *sdev, struct max_buff *mbuf);

static struct sap_api sap_mlme = {
	.sap_class = SAP_MLME,
	.sap_version_supported = sap_mlme_version_supported,
	.sap_handler = sap_mlme_rx_handler,
	.sap_versions = {SUPPORTED_VERSION, SUPPORTED_OLD_VERSION},
};

int sap_mlme_notifier(struct slsi_dev *sdev, unsigned long event)
{
	int i;
	struct netdev_vif *ndev_vif;

	SLSI_INFO_NODEV("Notifier event received %s\n", event ? "SCSC_WIFI_FAILURE_RESET" : "SCSC_WIFI_STOP");
	if ((event != SCSC_WIFI_FAILURE_RESET) && (event != SCSC_WIFI_STOP)) {
		return -EIO;
	}

	switch (event) {
	case SCSC_WIFI_STOP:
		/* Stop sending signals down */
		sdev->mlme_blocked = true;
		SLSI_INFO_NODEV("MLME BLOCKED\n");

		/* cleanup all the VIFs and scan data */
		SLSI_MUTEX_LOCK(sdev->netdev_add_remove_mutex);
		complete_all(&sdev->sig_wait.completion);
		for (i = 1; i <= CONFIG_SCSC_WLAN_MAX_INTERFACES; i++)
			if (sdev->netdev[i]) {
				ndev_vif = netdev_priv(sdev->netdev[i]);
				slsi_scan_cleanup(sdev, sdev->netdev[i]);
				SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);
				slsi_vif_cleanup(sdev, sdev->netdev[i], 0);
				SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
			}
		SLSI_MUTEX_UNLOCK(sdev->netdev_add_remove_mutex);
		SLSI_INFO_NODEV("Force cleaned all VIFs\n");
		break;

	case SCSC_WIFI_FAILURE_RESET:
		break;
	}

	return 0;
}

static int sap_mlme_version_supported(u16 version)
{
	unsigned int major = SAP_MAJOR(version);

#ifdef CONFIG_DEBUG
	unsigned int minor = SAP_MINOR(version);
#endif
	u8 i = 0;

	for (i = 0; i < SAP_MAX_VER; i++)
		if (sap_mlme.sap_versions[i] == major) {
			return 0;
		}

	pr_err("%s: Version %d.%d Not supported\n", __func__, major, minor);

	return -EINVAL;
}

static int slsi_rx_netdev_mlme(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf)
{
	u16 id = fapi_get_u16(mbuf, id);

	mbuf_set_mac_header(mbuf, 0);

	/* The mbuf is consumed by the functions called */
	switch (id) {
	case MLME_SCAN_IND:
		slsi_rx_scan_ind(sdev, dev, mbuf);
		break;
	case MLME_SCAN_DONE_IND:
		slsi_rx_scan_done_ind(sdev, dev, mbuf);
		break;
	case MLME_CONNECT_IND:
		slsi_rx_connect_ind(sdev, dev, mbuf);
		break;
	case MLME_CONNECTED_IND:
		slsi_rx_connected_ind(sdev, dev, mbuf);
		break;
	case MLME_DISCONNECT_IND:
		slsi_rx_disconnect_ind(sdev, dev, mbuf);
		break;
	case MLME_DISCONNECTED_IND:
		slsi_rx_disconnected_ind(sdev, dev, mbuf);
		break;
	case MLME_PROCEDURE_STARTED_IND:
		slsi_rx_procedure_started_ind(sdev, dev, mbuf);
		break;
	case MA_BLOCKACK_IND:
		slsi_rx_blockack_ind(sdev, dev, mbuf);
		break;
	case MLME_AC_PRIORITY_UPDATE_IND:
		SLSI_DBG1(sdev, SLSI_MLME, "Unexpected MLME_AC_PRIORITY_UPDATE_IND\n");
		slsi_kfree_mbuf(mbuf);
		break;
	case MLME_RECEIVED_FRAME_IND:
		slsi_rx_received_frame_ind(sdev, dev, mbuf);
		break;
	case MLME_FRAME_TRANSMISSION_IND:
		slsi_rx_frame_transmission_ind(sdev, dev, mbuf);
		break;
	case MLME_MIC_FAILURE_IND:
		slsi_rx_mic_failure_ind(sdev, dev, mbuf);
		break;
	default:
		slsi_kfree_mbuf(mbuf);
		SLSI_NET_ERR(dev, "Unhandled Ind: 0x%.4x\n", id);
		break;
	}
	return 0;
}

void slsi_rx_netdev_mlme_work(FAR void *arg)
{
	FAR struct slsi_mbuf_work *w = (FAR struct slsi_mbuf_work *)arg;
	struct slsi_dev *sdev = w->sdev;
	struct netif *dev = w->dev;
	struct max_buff *mbuf = slsi_mbuf_work_dequeue(w);

	if (WARN_ON(!dev)) {
		return;
	}

	while (mbuf) {
		slsi_rx_netdev_mlme(sdev, dev, mbuf);
		mbuf = slsi_mbuf_work_dequeue(w);
	}
}

int slsi_rx_enqueue_netdev_mlme(struct slsi_dev *sdev, struct max_buff *mbuf, u16 vif)
{
	struct netif *dev;
	struct netdev_vif *ndev_vif;

	dev = slsi_get_netdev(sdev, vif);
	if (!dev) {
		/* Calling function should free the mbuf */
		return -ENODEV;
	}

	ndev_vif = netdev_priv(dev);

	if ((ndev_vif->is_fw_test) && (fapi_get_sigid(mbuf) != MA_BLOCKACK_IND)) {
		slsi_kfree_mbuf(mbuf);
		return 0;
	}

	slsi_mbuf_work_enqueue(&ndev_vif->rx_mlme, mbuf, slsi_rx_netdev_mlme_work);
	return 0;
}

static int slsi_rx_action_enqueue_netdev_mlme(struct slsi_dev *sdev, struct max_buff *mbuf, u16 vif)
{
	struct netif *dev;
	struct netdev_vif *ndev_vif;

	dev = slsi_get_netdev(sdev, vif);
	if (!dev) {
		/* Calling function should free the mbuf */
		return -ENODEV;
	}

	ndev_vif = netdev_priv(dev);

	if (ndev_vif->is_fw_test) {
		slsi_kfree_mbuf(mbuf);
		return 0;
	}
#ifdef CONFIG_SCSC_ENABLE_P2P
	if (ndev_vif->iftype == SLSI_80211_IFTYPE_P2P_GO || ndev_vif->iftype == SLSI_80211_IFTYPE_P2P_CLIENT) {
		struct slsi_80211_mgmt *mgmt = fapi_get_mgmt(mbuf);
		/*  Check the DA of received action frame with the GO interface address */
		if (memcmp(mgmt->da, dev->dev_addr, ETH_ALEN) != 0) {
			/* If not equal, compare DA of received action frame with the P2P DEV address */
			struct netif *p2pdev = slsi_get_netdev(sdev, SLSI_NET_INDEX_P2P);

			if (WARN_ON(!p2pdev)) {
				/* Calling function should free the mbuf */
				return -ENODEV;
			}
			if (memcmp(mgmt->da, p2pdev->dev_addr, ETH_ALEN) == 0) {
				/* If destination address is equal to P2P DEV ADDR, then action frame is received on
				 * GO interface. Hence indicate action frames on P2P DEV
				 */
				ndev_vif = netdev_priv(p2pdev);

				if (ndev_vif->is_fw_test) {
					slsi_kfree_mbuf(mbuf);
					return 0;
				}
			}
		}
	}
#endif

	slsi_mbuf_work_enqueue(&ndev_vif->rx_mlme, mbuf, slsi_rx_netdev_mlme_work);

	return 0;
}

static int sap_mlme_rx_handler(struct slsi_dev *sdev, struct max_buff *mbuf)
{
	u16 scan_id;
	u16 vif = fapi_get_vif(mbuf);

#ifdef CONFIG_SLSI_WLAN_FAPI_LOG
	if (fapi_get_sigid(mbuf) != MLME_SCAN_IND) {
		struct timespec sig_time;

		clock_gettime(CLOCK_REALTIME, &sig_time);
		slsi_wlan_fapi_log_add_record(fapi_get_sigid(mbuf), sig_time);
	}
#endif

	if (slsi_rx_blocking_signals(sdev, mbuf) == 0) {
		return 0;
	}

	if (fapi_is_ind(mbuf)) {
		switch (fapi_get_sigid(mbuf)) {
		case MLME_SCAN_DONE_IND:
			scan_id = fapi_get_u16(mbuf, u.mlme_scan_done_ind.scan_id);
			return slsi_rx_enqueue_netdev_mlme(sdev, mbuf, (scan_id >> 8));
		case MLME_SCAN_IND:
			if (vif) {
				return slsi_rx_enqueue_netdev_mlme(sdev, mbuf, vif);
			}
			scan_id = fapi_get_u16(mbuf, u.mlme_scan_ind.scan_id);
			return slsi_rx_enqueue_netdev_mlme(sdev, mbuf, (scan_id >> 8));
		case MLME_RECEIVED_FRAME_IND:
			if (vif == 0) {
				SLSI_WARN(sdev, "Received MLME_RECEIVED_FRAME_IND on VIF 0\n");
				goto err;
			}
			return slsi_rx_action_enqueue_netdev_mlme(sdev, mbuf, vif);
		default:
			if (vif == 0) {
				SLSI_WARN(sdev, "Received signal 0x%04x on VIF 0, return error\n", fapi_get_sigid(mbuf));
				goto err;
			} else {
				return slsi_rx_enqueue_netdev_mlme(sdev, mbuf, vif);
			}
		}
	}
	if (WARN_ON(fapi_is_req(mbuf))) {
		goto err;
	}

	if (slsi_is_test_mode_enabled()) {
		slsi_kfree_mbuf(mbuf);
		return 0;
	}

	WARN_ON(1);

err:
	return -EINVAL;
}

int sap_mlme_init(void)
{
	SLSI_INFO_NODEV("Registering SAP\n");
	slsi_hip_sap_register(&sap_mlme);
	return 0;
}

int sap_mlme_deinit(void)
{
	SLSI_INFO_NODEV("Unregistering SAP\n");
	slsi_hip_sap_unregister(&sap_mlme);
	return 0;
}
