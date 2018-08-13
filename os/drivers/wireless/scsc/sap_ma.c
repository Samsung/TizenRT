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
#include "sap_ma.h"
#include "hip.h"
#include "ba.h"
#include "mgt.h"
#include <errno.h>
#include "utils_scsc.h"
#include <tinyara/net/netdev.h>
#include <tinyara/net/ethernet.h>
#include <tinyara/net/ip.h>

#define SUPPORTED_VERSION       13
#define SUPPORTED_OLD_VERSION   0

static int sap_ma_version_supported(u16 version);
static int sap_ma_rx_handler(struct slsi_dev *sdev, struct max_buff *mbuf);
static int sap_ma_txdone(struct slsi_dev *sdev, u16 colour);
int sap_ma_notifier(struct slsi_dev *sdev, unsigned long event);

static struct sap_api sap_ma = {
	.sap_class = SAP_MA,
	.sap_version_supported = sap_ma_version_supported,
	.sap_handler = sap_ma_rx_handler,
	.sap_versions = {SUPPORTED_VERSION, SUPPORTED_OLD_VERSION},
	.sap_txdone = sap_ma_txdone,
	.sap_notifier = sap_ma_notifier,
};

int sap_ma_notifier(struct slsi_dev *sdev, unsigned long event)
{
	SLSI_INFO_NODEV("Notifier event received %s\n", event);
	if ((event != SCSC_WIFI_FAILURE_RESET) && (event != SCSC_WIFI_STOP)) {
		return -EIO;
	}
	switch (event) {
	case SCSC_WIFI_STOP:
		break;
	case SCSC_WIFI_FAILURE_RESET:
		SLSI_INFO_NODEV("Netdevs queues will not be restarted - recovery will take care of it\n");
		break;
	}
	return 0;
}

static int sap_ma_version_supported(u16 version)
{
	unsigned int major = SAP_MAJOR(version);
	u8 i = 0;

	SLSI_INFO_NODEV("Reported version: %d\n", major);

	for (i = 0; i < SAP_MAX_VER; i++)
		if (sap_ma.sap_versions[i] == major) {
			return 0;
		}

	pr_err("%s: Version %d Not supported\n", __func__, major);

	return -EINVAL;
}

static int slsi_rx_amsdu_deaggregate(struct netif *dev, struct max_buff *mbuf)
{
	unsigned int msdu_len;
	unsigned int subframe_len;
	int padding;
	u8 *mbuf_data;

#ifdef CONFIG_SLSI_WLAN_STATS
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_dev *sdev = ndev_vif->sdev;
#endif

	SLSI_NET_DBG3(dev, SLSI_RX, "A-MSDU received, length = %d\n", mbuf->data_len);

	mbuf_pull(mbuf, fapi_get_siglen(mbuf));

	mbuf_data = slsi_mbuf_get_data(mbuf);
	while (mbuf->data_len > 0) {
		msdu_len = (mbuf_data[ETH_ALEN * 2] << 8) | mbuf_data[(ETH_ALEN * 2) + 1];

		if ((msdu_len > (ETH_DATA_LEN + LLC_SNAP_HDR_LEN)) || (msdu_len > mbuf->data_len) || (msdu_len < (ETH_ZLEN - (ETH_HLEN - LLC_SNAP_HDR_LEN))) || (mbuf->data_len < (ETH_ZLEN + LLC_SNAP_HDR_LEN))) {
			SLSI_NET_ERR(dev, "Wrong MSDU length %d, mbuf length = %d\n", msdu_len, mbuf->data_len);
			slsi_kfree_mbuf(mbuf);
			return -EINVAL;
		}

		subframe_len = msdu_len + (2 * ETH_ALEN) + 2;

		/* For the last subframe mbuf length and subframe length will be same */
		if (mbuf->data_len == subframe_len) {
			/* There is no padding for last subframe */
			padding = 0;
		} else {
			padding = (4 - (subframe_len % 4)) & 0x3;
		}

		/* Overwrite LLC+SNAP header with src & dest addr */
		SLSI_ETHER_COPY(&mbuf_data[14], &mbuf_data[6]);
		SLSI_ETHER_COPY(&mbuf_data[8], &mbuf_data[0]);

		/* Remove 8 bytes of LLC+SNAP header */
		mbuf_pull(mbuf, LLC_SNAP_HDR_LEN);
		subframe_len -= LLC_SNAP_HDR_LEN;

		SLSI_NET_DBG3(dev, SLSI_RX, "msdu_len = %d, subframe_len = %d, padding = %d\n", msdu_len, subframe_len, padding);

		SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.rx_num_msdu_in_amsdu);
		slsi_ethernetif_input(dev, mbuf_data, subframe_len);

		/* Move to the next subframe */
		mbuf_pull(mbuf, (subframe_len + padding));

		SLSI_NET_DBG3(dev, SLSI_RX, "mbuf->data_len = %d\n", mbuf->data_len);
	}

	slsi_kfree_mbuf(mbuf);
	return 0;
}

static int slsi_rx_data_process_mbuf(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf, bool fromBA)
{
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct slsi_peer *peer = NULL;
	struct ethhdr *ehdr = (struct ethhdr *)fapi_get_data(mbuf);
	u16 seq_num;
	bool skip_ba = fromBA;

	if (!((fapi_get_u16(mbuf, u.ma_unitdata_ind.data_unit_descriptor) == FAPI_DATAUNITDESCRIPTOR_IEEE802_3_FRAME) || (fapi_get_u16(mbuf, u.ma_unitdata_ind.data_unit_descriptor) == FAPI_DATAUNITDESCRIPTOR_AMSDU))) {
		SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.rx_drop_unsupported_frame);
		slsi_kfree_mbuf(mbuf);
		return -EOPNOTSUPP;
	}

	seq_num = fapi_get_u16(mbuf, u.ma_unitdata_ind.sequence_number);

	peer = slsi_get_peer_from_mac(sdev, dev, ehdr->h_source);
	if (!peer) {
		SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.rx_drop_peer_not_present);
		SLSI_NET_DBG1(dev, SLSI_RX, "Packet dropped, peer not found\n");
		/* Race in Data plane (Shows up in fw test mode) */
		slsi_kfree_mbuf(mbuf);
		return -EINVAL;
	}

	/* Buffering of the frames before mlme_connected_ind */
	if ((ndev_vif->vif_type == FAPI_VIFTYPE_AP) && (peer->connected_state == SLSI_STA_CONN_STATE_CONNECTING)) {
		SLSI_DBG2(sdev, SLSI_MLME, "Buffering MA-UnitData FRAMES");
		slsi_mbuf_queue_tail(&peer->buffered_frames, mbuf);
		return 1;
	}

	if (!skip_ba && (slsi_ba_check(peer, fapi_get_u16(mbuf, u.ma_unitdata_ind.priority))))
		if (!slsi_ba_process_frame(dev, peer, mbuf, (seq_num & SLSI_RX_SEQ_NUM_MASK), fapi_get_u16(mbuf, u.ma_unitdata_ind.priority))) {
			return 1;
		}

	/* A-MSDU deaggregation */
	if (fapi_get_u16(mbuf, u.ma_unitdata_ind.data_unit_descriptor) == FAPI_DATAUNITDESCRIPTOR_AMSDU) {
		SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.rx_num_amsdu);
		/* This function will consume the mbuf */
		slsi_rx_amsdu_deaggregate(dev, mbuf);
		return 1;
	}

	/* strip signal and any signal/bulk roundings/offsets */
	mbuf_pull(mbuf, fapi_get_siglen(mbuf));

	/* In STA mode, the AP relays back our multicast traffic.
	 * Receiving these frames and passing it up confuses some
	 * protocols and applications, notably IPv6 Duplicate
	 * Address Detection.
	 *
	 * So these frames are dropped instead of passing it further.
	 * No need to update the drop statistics as these frames are
	 * locally generated and should not be accounted in reception.
	 */
	if (ndev_vif->vif_type == FAPI_VIFTYPE_STATION) {
		ehdr = (struct ethhdr *)(slsi_mbuf_get_data(mbuf));

		if (is_multicast_ether_addr(ehdr->h_dest) && !compare_ether_addr(ehdr->h_source, dev->d_mac.ether_addr_octet)) {
			SLSI_NET_DBG2(dev, SLSI_RX, "drop locally generated multicast frame relayed back by AP\n");
			slsi_kfree_mbuf(mbuf);
			return -EINVAL;
		}
	}

	return 0;
}

#ifdef CONFIG_SLSI_RX_PERFORMANCE_TEST
static void slsi_rx_performance_test_timer(void *data)
{
	struct slsi_dev *sdev = (struct slsi_dev *)data;
	static int interval;
	int actual_interval;
	clock_t current_timestamp;

	current_timestamp = clock_systimer();
	actual_interval = TICK2MSEC(current_timestamp - sdev->rx_perf.last_timestamp);	/* Miliseconds */

	/* Number of bits received per milisecond */
	sdev->rx_perf.num_bits_received /= actual_interval;

	/* Number of bits received per second */
	sdev->rx_perf.num_bits_received *= 1000;

	if (sdev->rx_perf.num_bits_received > (1024 * 1024)) {
		int bandwidth;

		/* Bandwidth in kilobits */
		bandwidth = sdev->rx_perf.num_bits_received / 1024;

		/*
		 * Bandwidth in megabits - floating point is not supported in the driver.
		 * So 100 is multiplied to get 2 decimal points.
		 */
		bandwidth = (bandwidth * 100) / 1024;
		lldbg("Interval: %d-%d Throughput: %d.%d Mbps\n", interval, interval + SLSI_RX_PERFORMANCE_REPORT_INTERVAL, bandwidth / 100, bandwidth % 100);
	} else if (sdev->rx_perf.num_bits_received > 1024) {
		lldbg("Interval: %d-%d Throughput: %d Kbps\n", interval, interval + SLSI_RX_PERFORMANCE_REPORT_INTERVAL, sdev->rx_perf.num_bits_received / 1024);
	} else {
		lldbg("Interval: %d-%d Throughput: %d bps\n", interval, interval + SLSI_RX_PERFORMANCE_REPORT_INTERVAL, sdev->rx_perf.num_bits_received);
	}

	interval += SLSI_RX_PERFORMANCE_REPORT_INTERVAL;
	pthread_mutex_lock(&sdev->rx_perf.rx_perf_lock);
	sdev->rx_perf.num_bits_received = 0;
	pthread_mutex_unlock(&sdev->rx_perf.rx_perf_lock);

	sdev->rx_perf.last_timestamp = clock_systimer();

	sys_untimeout(slsi_rx_performance_test_timer, sdev);
	if (sdev->rx_perf.rx_perf_test_started == true) {
		sys_timeout(SLSI_RX_PERFORMANCE_REPORT_INTERVAL * 1000, slsi_rx_performance_test_timer, sdev);
	}
}

void slsi_rx_stop_performance_test_timer(struct slsi_dev *sdev)
{
	sdev->rx_perf.rx_perf_test_started = false;
	sys_untimeout(slsi_rx_performance_test_timer, sdev);
}

static void slsi_rx_start_performance_test_timer(struct slsi_dev *sdev)
{
	sdev->rx_perf.rx_perf_test_started = true;
	sys_timeout(SLSI_RX_PERFORMANCE_REPORT_INTERVAL * 1000, slsi_rx_performance_test_timer, sdev);
}

static void slsi_rx_performance_test(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf)
{
	static bool init_done;

	if (init_done == false) {
		pthread_mutex_init(&sdev->rx_perf.rx_perf_lock, NULL);
		init_done = true;
	}

	if (sdev->rx_perf.rx_perf_test_started == false) {
		slsi_rx_start_performance_test_timer(sdev);
	}

	if (mbuf->data_len < SLSI_RX_PERFORMANCE_PKT_LEN_THRESHOLD) {
		slsi_ethernetif_input(dev, slsi_mbuf_get_data(mbuf), mbuf->data_len);
	}

	pthread_mutex_lock(&sdev->rx_perf.rx_perf_lock);
	/* Update number of bits received */
	sdev->rx_perf.num_bits_received += ((mbuf->data_len - ETH_HDRLEN - IPv4_HDRLEN - UDP_HDRLEN) * 8);
	pthread_mutex_unlock(&sdev->rx_perf.rx_perf_lock);
}
#endif

int slsi_rx_data(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf, bool fromBA)
{
	if (slsi_rx_data_process_mbuf(sdev, dev, mbuf, fromBA) == 0) {
#ifdef CONFIG_SLSI_RX_PERFORMANCE_TEST
		slsi_rx_performance_test(sdev, dev, mbuf);
#else
		slsi_ethernetif_input(dev, slsi_mbuf_get_data(mbuf), mbuf->data_len);
#endif
		slsi_kfree_mbuf(mbuf);
	}
	return 0;
}

static int slsi_rx_data_cfm(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf)
{
	SLSI_NET_DBG3(dev, SLSI_TX, "ma_unitdata_cfm(vif:%d, host_tag:0x%x, status:%d)\n", fapi_get_vif(mbuf), fapi_get_u16(mbuf, u.ma_unitdata_cfm.host_tag), fapi_get_u16(mbuf, u.ma_unitdata_cfm.transmission_status));

#ifdef CONFIG_SLSI_WLAN_STATS
	switch (fapi_get_u16(mbuf, u.ma_unitdata_cfm.transmission_status)) {
	case FAPI_TRANSMISSIONSTATUS_SUCCESSFUL:
		break;
	case FAPI_TRANSMISSIONSTATUS_RETRY_LIMIT:
		sdev->dp_stats.tx_failed_retry_limit++;
		break;
	case FAPI_TRANSMISSIONSTATUS_TX_LIFETIME:
		sdev->dp_stats.tx_failed_tx_lifetime++;
		break;
	case FAPI_TRANSMISSIONSTATUS_NO_BSS:
	case FAPI_TRANSMISSIONSTATUS_EXCESSIVE_DATA_LENGTH:
	case FAPI_TRANSMISSIONSTATUS_UNAVAILABLE_KEY_MAPPING:
	case FAPI_TRANSMISSIONSTATUS_UNSPECIFIED_FAILURE:
	default:
		sdev->dp_stats.tx_failed_cfm_err++;
		break;
	}
#endif
	slsi_kfree_mbuf(mbuf);
	return 0;
}

void slsi_rx_netdev_data_work(FAR void *arg)
{
	FAR struct slsi_mbuf_work *w = (FAR struct slsi_mbuf_work *)arg;
	struct slsi_dev *sdev = w->sdev;
	struct netif *dev = w->dev;
	struct netdev_vif *ndev_vif = netdev_priv(dev);
	struct max_buff *mbuf;

	if (WARN_ON(!dev)) {
		return;
	}

	SLSI_MUTEX_LOCK(ndev_vif->vif_mutex);

#ifdef CONFIG_SLSI_WLAN_STATS
	if (w->queue.queue_len > sdev->dp_stats.rx_mbuf_q_max_len) {
		sdev->dp_stats.rx_mbuf_q_max_len = w->queue.queue_len;
	}
#endif

	while (1) {
		if (ndev_vif->ba_flush) {
			ndev_vif->ba_flush = 0;
			slsi_ba_process_complete(dev);
		}

		mbuf = slsi_mbuf_work_dequeue(w);
		if (!mbuf) {
			break;
		}
		switch (fapi_get_u16(mbuf, id)) {
		case MA_UNITDATA_IND:
			SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.rx_num_unit_data_ind);
			(void)slsi_rx_data(sdev, dev, mbuf, false);
			break;
		case MA_UNITDATA_CFM:
			(void)slsi_rx_data_cfm(sdev, dev, mbuf);
			break;
		default:
			SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.rx_drop_wrong_signal_id);
			SLSI_DBG1(sdev, SLSI_MLME, "Unexpected Data: 0x%.4x\n", fapi_get_sigid(mbuf));
			slsi_kfree_mbuf(mbuf);
			break;
		}
	}
	SLSI_MUTEX_UNLOCK(ndev_vif->vif_mutex);
}

static int slsi_rx_queue_data(struct slsi_dev *sdev, struct max_buff *mbuf)
{
	struct netif *dev;
	struct netdev_vif *ndev_vif;
	int vif;

	vif = fapi_get_vif(mbuf);

	dev = slsi_get_netdev(sdev, vif);
	if (!dev) {
		SLSI_ERR(sdev, "netdev(%d) No longer exists\n", vif);
		goto err;
	}
	ndev_vif = netdev_priv(dev);

	/* If the mbuf queue length has already reached maximum limit then drop the received packets */
	if (ndev_vif->rx_data.queue.queue_len >= SCSC_MAX_RX_MBUF_QUEUE_LEN) {
		slsi_kfree_mbuf(mbuf);
		SLSI_INCR_DATA_PATH_STATS(ndev_vif->sdev->dp_stats.rx_drop_mbuf_q_full);
		return 0;
	}

	slsi_mbuf_work_enqueue(&ndev_vif->rx_data, mbuf, slsi_rx_netdev_data_work);

	return 0;
err:
	/* Calling function should free the mbuf */
	return -EINVAL;
}

static int sap_ma_rx_handler(struct slsi_dev *sdev, struct max_buff *mbuf)
{
	switch (fapi_get_sigid(mbuf)) {
	case MA_UNITDATA_IND:
	case MA_TX_FAILURE_IND:
	case MA_UNITDATA_CFM:
		return slsi_rx_queue_data(sdev, mbuf);
	case MA_BLOCKACK_IND:
		/* It is anomolous to handle the MA_BLOCKACK_IND in the mlme wq */
		return slsi_rx_enqueue_netdev_mlme(sdev, mbuf, fapi_get_vif(mbuf));
	default:
		break;
	}

	pr_err("%s: Shouldn't be getting here!\n", __func__);
	return -EINVAL;
}

/* Adjust the scod value and flow control appropriately. */
static int sap_ma_txdone(struct slsi_dev *sdev, u16 colour)
{
	int ret = 0;

#ifdef CONFIG_SCSC_TX_FLOW_CONTROL
	struct netif *dev;
	struct slsi_peer *peer;
	u16 vif, peer_index, ac;

	SLSI_INCR_DATA_PATH_STATS(sdev->dp_stats.tx_done);

	/* Extract information from the coloured mbulk */
	/* colour is defined as: */
	/* u16 register bits:
	 * 0      - do not use
	 * [2:1]  - vif
	 * [7:3]  - peer_index
	 * [10:8] - ac queue
	 */
	vif = (colour & 0x6) >> 1;
	peer_index = (colour & 0xf8) >> 3;
	ac = (colour & 0x300) >> 8;

	dev = slsi_get_netdev(sdev, vif);
	if (!dev) {
		SLSI_ERR(sdev, "netdev(%d) No longer exists\n", vif);
		return -EINVAL;
	}

	if (peer_index <= SLSI_PEER_INDEX_MAX) {
		/* peer_index = 0 for Multicast queues */
		if (peer_index == 0) {
			struct netdev_vif *ndev_vif = netdev_priv(dev);
			return scsc_wifi_fcq_receive_data(dev, &ndev_vif->ap.group_data_qs, ac);
		}
		peer = slsi_get_peer_from_qs(sdev, dev, MAP_AID_TO_QS(peer_index));
		if (peer) {
			return scsc_wifi_fcq_receive_data(dev, &peer->data_qs, ac);
		} else {
			ret = -EINVAL;
			SLSI_DBG3(sdev, SLSI_RX, "peer record NOT found for peer_index=%d\n", peer_index);
		}
	} else {
		ret = -EINVAL;
		SLSI_ERR(sdev, "illegal peer_index vif=%d peer_index=%d\n", vif, peer_index);
	}
#endif
	return ret;
}

int sap_ma_init(void)
{
	SLSI_INFO_NODEV("Registering SAP\n");
	slsi_hip_sap_register(&sap_ma);
	return 0;
}

int sap_ma_deinit(void)
{
	SLSI_INFO_NODEV("Unregistering SAP\n");
	slsi_hip_sap_unregister(&sap_ma);
	return 0;
}
