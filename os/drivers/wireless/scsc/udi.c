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

#include <tinyara/config.h>
#include <sys/types.h>
#include <stdbool.h>
#include <string.h>
#include <poll.h>
#include <errno.h>
#include <tinyara/fs/fs.h>
#include <semaphore.h>

#include "dev.h"

#include "hip.h"
#include "log_clients.h"
#include "mlme.h"
#include "fw_test.h"
#include "debug_scsc.h"
#include "udi.h"
#include "tinyara_wrapper.h"
#include "max_buf.h"

#define UDI_CHAR_DEVICE_NAME0 "s5n2560udi0"
#define UDI_CHAR_DEVICE_NAME1 "s5n2560udi1"
#define UDI_CLASS_NAME "s5n2560"

#ifndef ETH_P_WAPI
#define ETH_P_WAPI 0x88b4
#endif
#define ETH_P_IP   0x0800
#define ETH_P_ARP  0x0806

#define SLSI_IP_TYPE_UDP 0x11
#define SLSI_DHCP_SERVER_PORT 67
#define SLSI_DHCP_CLIENT_PORT 68
#define SLSI_DHCP_MAGIC_OFFSET 272
#define SLSI_DHCP_MESSAGE_TYPE_ACK 0x05

/**
 * Control character device for debug
 * ==================================
 */
#define NUM_CHAR_CLIENTS 12		/* Number of client programmes on one node. */

#define MAX_MINOR (SLSI_UDI_MINOR_NODES - 1)	/* Maximum node number. */

struct slsi_cdev_client;

struct slsi_cdev {
	int minor;
	struct slsi_cdev_client *client[NUM_CHAR_CLIENTS];

	struct slsi_dev *sdev;
	struct device *parent;
};

struct slsi_cdev_client {
	struct slsi_cdev *ufcdev;
	int log_enabled;
	int log_allow_driver_signals;

	u16 tx_sender_id;
	struct slsi_fw_test fw_test;

	/* Flags set for special filtering of ma_packet data */
	u16 ma_unitdata_filter_config;

	struct max_buff_head log_list;
	sem_t log_mutex;
	struct completion log_wq;

	/* Drop Frames and report the number dropped */
#define UDI_MAX_QUEUED_FRAMES 10000
#define UDI_RESTART_QUEUED_FRAMES 9000

#define UDI_MAX_QUEUED_DATA_FRAMES 9000
#define UDI_RESTART_QUEUED_DATA_FRAMES 8000

	/* Start dropping ALL frames at    queue_len == UDI_MAX_QUEUED_FRAMES
	 * Restart queueing ALL frames at  queue_len == UDI_RESTART_QUEUED_FRAMES
	 * Enable MA_PACKET filters at     queue_len == UDI_MAX_QUEUED_DATA_FRAMES
	 * Disable MA_PACKET filters at    queue_len == UDI_RESTART_QUEUED_DATA_FRAMES
	 */
	u32 log_dropped;
	u32 log_dropped_data;
	bool log_drop_data_packets;
	struct pollfd *fds;
};

static inline bool slsi_cdev_unitdata_filter_allow(struct slsi_cdev_client *client, u16 filter)
{
	return (client->ma_unitdata_filter_config & filter) == filter;
}

/* One minor node per phy. In normal driver mode, this may be one.
 * In unit test mode, this may be several.
 */
static struct slsi_cdev *uf_cdevs[SLSI_UDI_MINOR_NODES];

static int udi_log_event(struct slsi_log_client *log_client, struct max_buff *mbuf, int dir);
static int send_signal_to_log_filter(struct slsi_log_client *log_client, struct max_buff *mbuf, int dir);
static int send_signal_to_inverse_log_filter(struct slsi_log_client *log_client, struct max_buff *mbuf, int dir);

int slsi_check_cdev_refs(void)
{
	int client_num;
	int cdev_num;
	struct slsi_cdev *cdev = NULL;

	for (cdev_num = 0; cdev_num < SLSI_UDI_MINOR_NODES; cdev_num++) {
		cdev = uf_cdevs[cdev_num];

		if (cdev == NULL) {
			continue;
		}

		for (client_num = 0; client_num < NUM_CHAR_CLIENTS; client_num++)
			if (cdev->client[client_num] != NULL) {
				return 1;
			}
	}

	return 0;
}

int slsi_kernel_to_user_space_event(struct slsi_log_client *log_client, u16 event, u32 data_length, const u8 *data)
{
#ifdef CONFIG_SCSC_WLANLITE
	struct slsi_cdev_client *client = log_client->log_client_ctx;
	struct max_buff *mbuf;
	int ret;

	if (WARN_ON(client == NULL)) {
		return -EINVAL;
	}

	if (!client->log_allow_driver_signals) {
		return 0;
	}

	mbuf = fapi_alloc_f(sizeof(struct fapi_signal_header), data_length, event, 0, __FILE__, __LINE__);
	if (WARN_ON(!mbuf)) {
		return -ENOMEM;
	}

	if (data_length) {
		fapi_append_data(mbuf, data, data_length);
	}

	ret = udi_log_event(log_client, mbuf, UDI_CONFIG_IND);
	if (ret) {
		SLSI_WARN_NODEV("Udi log event not registered\n");
	}

	/* udi_log_event takes a copy, so ensure that the mbuf allocated in this
	 * function is freed again.
	 */
	slsi_kfree_mbuf(mbuf);
	return ret;
#else
	return 0;
#endif
}

static int slsi_cdev_open(FAR struct file *filep)
{
	struct slsi_cdev *uf_cdev;
	struct slsi_cdev_client *client;
	int indx;
	int minor;

	if (filep->f_inode == NULL) {
		SLSI_ERR_NODEV("inode is NULL\n");
		return -EINVAL;
	}

	SLSI_INFO_NODEV("node name: %s\n", filep->f_inode->i_name);
	if (strcmp(filep->f_inode->i_name, UDI_CHAR_DEVICE_NAME0) == 0) {
		minor = 0;
	} else if (strcmp(filep->f_inode->i_name, UDI_CHAR_DEVICE_NAME1) == 0) {
		minor = 1;
	} else {
		SLSI_ERR_NODEV("Unknown node name: %s\n", filep->f_inode->i_name);
		return -EINVAL;
	}

	uf_cdev = uf_cdevs[minor];
	if (!uf_cdev) {
		SLSI_ERR_NODEV("no cdev instance for minor %d\n", minor);
		return -EINVAL;
	}

	for (indx = 0; indx < NUM_CHAR_CLIENTS; indx++)
		if (uf_cdev->client[indx] == NULL) {
			break;
		}
	if (indx >= NUM_CHAR_CLIENTS) {
		SLSI_ERR_NODEV("already opened\n");
		return -EOPNOTSUPP;
	}

	client = kmm_malloc(sizeof(*client));
	if (client == NULL) {
		return -ENOMEM;
	}
	memset(client, 0, sizeof(struct slsi_cdev_client));

	/* init other resource */
	mbuf_queue_head_init(&client->log_list);
	init_completion(&client->log_wq);
	sem_init(&client->log_mutex, 0, 1);
	client->tx_sender_id = SLSI_TX_PROCESS_ID_UDI_MIN;
	slsi_fw_test_init(uf_cdev->sdev, &client->fw_test);

	client->ufcdev = uf_cdev;
	uf_cdev->client[indx] = client;
	filep->f_priv = client;
	//slsi_procfs_inc_node();

	SLSI_DBG1_NODEV(SLSI_UDI, "Client:%d added\n", indx);

	return 0;
}

static int slsi_cdev_close(FAR struct file *filep)
{
	struct slsi_cdev_client *client = (void *)filep->f_priv;
	struct slsi_cdev *uf_cdev;
	int indx;
	int minor;

	SLSI_INFO_NODEV("node name: %s\n", filep->f_inode->i_name);
	if (strcmp(filep->f_inode->i_name, UDI_CHAR_DEVICE_NAME0) == 0) {
		minor = 0;
	} else if (strcmp(filep->f_inode->i_name, UDI_CHAR_DEVICE_NAME1) == 0) {
		minor = 1;
	} else {
		SLSI_ERR_NODEV("Unknown node name: %s\n", filep->f_inode->i_name);
		return -EINVAL;
	}

	uf_cdev = uf_cdevs[minor];
	if (!uf_cdev) {
		SLSI_ERR_NODEV("no cdev instance for minor %d\n", minor);
		return -EINVAL;
	}

	if (client == NULL) {
		return -EINVAL;
	}

	for (indx = 0; indx < NUM_CHAR_CLIENTS; indx++)
		if (uf_cdev->client[indx] == client) {
			break;
		}
	if (indx >= NUM_CHAR_CLIENTS) {
		SLSI_ERR_NODEV("client not found in list\n");
		return -EINVAL;
	}

	complete(&client->log_wq);

	if (client->log_enabled) {
		slsi_log_client_unregister(client->ufcdev->sdev, client);
	}

	slsi_mbuf_queue_purge(&client->log_list);

	slsi_fw_test_deinit(uf_cdev->sdev, &client->fw_test);
	uf_cdev->client[indx] = NULL;

	/* free other resource */
	kmm_free(client);
	//slsi_procfs_dec_node();

	SLSI_DBG1_NODEV(SLSI_UDI, "Client:%d removed\n", indx);

	return 0;
}

static ssize_t slsi_cdev_read(FAR struct file *filp, FAR char *p, size_t len)
{
	struct slsi_cdev_client *client = (void *)filp->f_priv;
	struct slsi_dev *sdev;
	int msglen;
	struct max_buff *mbuf;

	SLSI_INFO_NODEV("\n");
	if (client == NULL) {
		return -EINVAL;
	}

	if (!client->log_list.queue_len) {
		/* wait until getting a signal */
		wait_for_completion(&client->log_wq);
	}

	sdev = client->ufcdev->sdev;
	if (!sdev) {
		SLSI_ERR_NODEV("sdev not set\n");
		return -EINVAL;
	}

	mbuf = slsi_mbuf_dequeue(&client->log_list);
	if (!mbuf) {
		SLSI_ERR(sdev, "No Data\n");
		return -EINVAL;
	}

	slsi_fw_test_signal_with_udi_header(sdev, &client->fw_test, mbuf);

	msglen = mbuf->data_len;
	if (msglen > (s32) len) {
		SLSI_WARN(sdev, "truncated read to %d actual msg len is %lu\n", msglen, (unsigned long int)len);
		msglen = len;
	}

	if (copy_to_user(p, slsi_mbuf_get_data(mbuf), msglen)) {
		SLSI_ERR(sdev, "Failed to copy UDI log to user\n");
		slsi_kfree_mbuf(mbuf);
		return -EFAULT;
	}

	slsi_kfree_mbuf(mbuf);
	return msglen;
}

static ssize_t slsi_cdev_write(FAR struct file *filep, FAR const char *p, size_t len)
{
	struct slsi_cdev_client *client;
	struct slsi_dev *sdev;
	struct max_buff *mbuf;
	u8 *data;

	SLSI_INFO_NODEV("\n");

	client = (void *)filep->f_priv;
	if (client == NULL) {
		SLSI_ERR_NODEV("filep private data not set\n");
		return -EINVAL;
	}

	if (!client->ufcdev) {
		SLSI_ERR_NODEV("ufcdev not set\n");
		return -EINVAL;
	}

	sdev = client->ufcdev->sdev;
	if (!sdev) {
		SLSI_ERR_NODEV("sdev not set\n");
		return -EINVAL;
	}
	mbuf = slsi_mbuf_alloc(len);
	data = mbuf_put(mbuf, len);
	memcpy(data, p, len);

	mbuf->fapi.sig_length = fapi_get_expected_size(mbuf);
	mbuf->fapi.data_length = mbuf->data_len;
	/* colour is defined as: */
	/* u16 register bits:
	 * 0      - do not use
	 * [2:1]  - vif
	 * [7:3]  - peer_index
	 * [10:8] - ac queue
	 */
	if (fapi_is_ma(mbuf)) {
		mbuf->colour = (slsi_frame_priority_to_ac_queue(mbuf->user_priority) << 8) | (fapi_get_u16(mbuf, u.ma_unitdata_req.peer_index) << 3) | (fapi_get_u16(mbuf, u.ma_unitdata_req.vif) << 1);
	}

	/* F/w will panic if fw_reference is not zero. */
	fapi_set_u32(mbuf, fw_reference, 0);
	mbuf_set_mac_header(mbuf, fapi_get_data(mbuf) - slsi_mbuf_get_data(mbuf));

	SLSI_DBG3_NODEV(SLSI_UDI, "UDI Signal:%.4X  SigLEN:%d  DataLen:%d  MBUF Headroom:%d  bytes:%d\n", fapi_get_sigid(mbuf), fapi_get_siglen(mbuf), fapi_get_datalen(mbuf), mbuf_headroom(mbuf), (int)len);

	/* In WlanLite test mode req signals IDs are 0x1000, 0x1002, 0x1004 */
	if (slsi_is_test_mode_enabled() || fapi_is_req(mbuf) || fapi_is_res(mbuf)) {
		/* Use the range of PIDs allocated to the udi clients */
		client->tx_sender_id++;
		if (client->tx_sender_id > SLSI_TX_PROCESS_ID_UDI_MAX) {
			client->tx_sender_id = SLSI_TX_PROCESS_ID_UDI_MIN;
		}

		fapi_set_u16(mbuf, sender_pid, client->tx_sender_id);
		if (!slsi_is_test_mode_enabled()) {
			slsi_fw_test_signal(sdev, &client->fw_test, mbuf);
		}
		if (fapi_is_ma(mbuf)) {
			if (slsi_tx_data_lower(sdev, mbuf)) {
				slsi_kfree_mbuf(mbuf);
				return -EINVAL;
			}
		} else if (slsi_tx_control(sdev, NULL, mbuf, true)) {
			slsi_kfree_mbuf(mbuf);
			return -EINVAL;
		}
	} else if (slsi_hip_rx(sdev, mbuf)) {
		slsi_kfree_mbuf(mbuf);
		return -EINVAL;
	}

	return len;
}

static int slsi_cdev_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	struct slsi_cdev_client *client = (void *)filep->f_priv;
	struct slsi_dev *sdev;
	long r = 0;
	int int_param;
	int mib_data_length;		/* Length of valid Mib data in the buffer */
	int mib_data_size;			/* Size of the mib buffer  */
	unsigned char *mib_data;	/* Mib Input/Output Buffer */
	u16 mib_vif;

	if (client == NULL || client->ufcdev == NULL) {
		return -EINVAL;
	}
	sdev = client->ufcdev->sdev;

	switch (cmd) {
	case UNIFI_GET_UDI_ENABLE:
		int_param = client->log_enabled;
		*(int *)arg = int_param;
		break;

	case UNIFI_SET_UDI_ENABLE:
		int_param = *(int *)arg;
		if (int_param) {
			slsi_log_client_register(sdev, client, udi_log_event, NULL, 0, 0);
			client->log_enabled = 1;
			if (int_param > 1) {
				client->log_allow_driver_signals = 1;
			}
		} else {
			slsi_log_client_unregister(sdev, client);
			client->log_enabled = 0;
		}

		break;

	case UNIFI_SET_UDI_LOG_MASK: {
		struct unifiio_filter_t filter;
		int i;

		/* to minimise load on data path, list is converted here to array indexed by signal number */
		if (copy_from_user(&filter, (void *)arg, sizeof(filter))) {
			SLSI_ERR(sdev, "UNIFI_SET_UDI_LOG_MASK: Failed to copy from userspace\n");
			r = -EFAULT;
			break;
		}
		if (filter.signal_ids_n) {
			char *signal_filter_index;
			int max;
			int min;

			max = filter.signal_ids[0];
			min = filter.signal_ids[0];

			/* find maximum and minimum signal id in filter */
			for (i = 0; i < filter.signal_ids_n; i++) {
				if (filter.signal_ids[i] & UDI_MA_UNITDATA_FILTER_ALLOW_MASK) {
					client->ma_unitdata_filter_config |= filter.signal_ids[i];
					continue;
				}
				if (filter.signal_ids[i] > max) {
					max = filter.signal_ids[i];
				} else if (filter.signal_ids[i] < min) {
					min = filter.signal_ids[i];
				}
			}
			/* and create array only big enough to index the range of signal id specified */
			signal_filter_index = kmm_malloc(max - min + 1);
			if (signal_filter_index) {
				memset(signal_filter_index, 0, max - min + 1);
				for (i = 0; i < filter.signal_ids_n; i++) {
					if (filter.signal_ids[i] & UDI_MA_UNITDATA_FILTER_ALLOW_MASK) {
						continue;
					}
					signal_filter_index[filter.signal_ids[i] - min] = 1;
				}
				slsi_log_client_unregister(sdev, client);
				slsi_log_client_register(sdev, client, filter.log_listed_flag ? send_signal_to_inverse_log_filter : send_signal_to_log_filter, signal_filter_index, min, max);
			} else {
				r = -ENOMEM;
			}
		}
		break;
	}
	case UNIFI_SET_MIB: {
		struct netif *dev = NULL;

		if (sdev->device_state != SLSI_DEVICE_STATE_STARTED) {
			SLSI_ERR(sdev, "UNIFI_SET_MIB: Device not yet available\n");
			r = -EFAULT;
			break;
		}

		/* First 2 Bytes are the VIF */
		if (copy_from_user((void *)&mib_vif, (void *)arg, 2)) {
			SLSI_ERR(sdev, "UNIFI_SET_MIB: Failed to copy in vif\n");
			r = -EFAULT;
			break;
		}

		/* First 4 Bytes are the Number of Bytes of input Data */
		if (copy_from_user((void *)&mib_data_length, (void *)(arg + 2), 4)) {
			SLSI_ERR(sdev, "UNIFI_SET_MIB: Failed to copy in mib_data_length\n");
			r = -EFAULT;
			break;
		}

		/* Second 4 Bytes are the size of the Buffer */
		if (copy_from_user((void *)&mib_data_size, (void *)(arg + 6), 4)) {
			SLSI_ERR(sdev, "UNIFI_SET_MIB: Failed to copy in mib_data_size\n");
			r = -EFAULT;
			break;
		}

		mib_data = kmm_malloc(mib_data_size);

		/* Read the rest of the Mib Data */
		if (copy_from_user((void *)mib_data, (void *)(arg + 10), mib_data_length)) {
			SLSI_ERR(sdev, "UNIFI_SET_MIB: Failed to copy in mib_data\n");
			kmm_free(mib_data);
			r = -EFAULT;
			break;
		}

		SLSI_MUTEX_LOCK(sdev->netdev_add_remove_mutex);
		dev = slsi_get_netdev_locked(sdev, mib_vif);
		if (mib_vif != 0 && !dev) {
			SLSI_ERR(sdev, "UNIFI_SET_MIB: Failed - net_device is NULL for interface = %d\n", mib_vif);
			kmm_free(mib_data);
			r = -EFAULT;
			SLSI_MUTEX_UNLOCK(sdev->netdev_add_remove_mutex);
			break;
		}

		r = slsi_mlme_set(sdev, dev, mib_data, mib_data_length);
		SLSI_MUTEX_UNLOCK(sdev->netdev_add_remove_mutex);
		kmm_free(mib_data);
		break;
	}
	case UNIFI_GET_MIB: {
		struct netif *dev = NULL;

		if (sdev->device_state != SLSI_DEVICE_STATE_STARTED) {
			SLSI_ERR(sdev, "UNIFI_GET_MIB: Device not yet available\n");
			r = -EFAULT;
			break;
		}

		/* First 2 Bytes are the VIF */
		if (copy_from_user((void *)&mib_vif, (void *)arg, 2)) {
			SLSI_ERR(sdev, "UNIFI_SET_MIB: Failed to copy in vif\n");
			r = -EFAULT;
			break;
		}

		/* First 4 Bytes are the Number of Bytes of input Data */
		if (copy_from_user((void *)&mib_data_length, (void *)(arg + 2), 4)) {
			SLSI_ERR(sdev, "UNIFI_GET_MIB: Failed to copy in mib_data_length\n");
			r = -EFAULT;
			break;
		}

		/* Second 4 Bytes are the size of the Buffer */
		if (copy_from_user((void *)&mib_data_size, (void *)(arg + 6), 4)) {
			SLSI_ERR(sdev, "UNIFI_GET_MIB: Failed to copy in mib_data_size\n");
			r = -EFAULT;
			break;
		}

		mib_data = kmm_malloc(mib_data_size);

		/* Read the rest of the Mib Data */
		if (copy_from_user((void *)mib_data, (void *)(arg + 10), mib_data_length)) {
			SLSI_ERR(sdev, "UNIFI_GET_MIB: Failed to copy in mib_data\n");
			kmm_free(mib_data);
			r = -EFAULT;
			break;
		}

		SLSI_MUTEX_LOCK(sdev->netdev_add_remove_mutex);
		dev = slsi_get_netdev_locked(sdev, mib_vif);
		if (mib_vif != 0 && !dev) {
			SLSI_ERR(sdev, "UNIFI_SET_MIB: Failed - net_device is NULL for interface = %d\n", mib_vif);
			kmm_free(mib_data);
			r = -EFAULT;
			SLSI_MUTEX_UNLOCK(sdev->netdev_add_remove_mutex);
			break;
		}
		if (slsi_mlme_get(sdev, dev, mib_data, mib_data_length, mib_data, mib_data_size, &mib_data_length)) {
			kmm_free(mib_data);
			r = -EINVAL;
			SLSI_MUTEX_UNLOCK(sdev->netdev_add_remove_mutex);
			break;
		}

		SLSI_MUTEX_UNLOCK(sdev->netdev_add_remove_mutex);

		/* Check the buffer is big enough */
		if (mib_data_length > mib_data_size) {
			SLSI_ERR(sdev, "UNIFI_GET_MIB: Mib result data is to long. (%d bytes when the max is %d bytes)\n", mib_data_length, mib_data_size);
			kmm_free(mib_data);
			r = -EINVAL;
			break;
		}

		/* Copy back the number of Bytes in the Mib result */
		if (copy_to_user((void *)arg, (void *)&mib_data_length, 4)) {
			SLSI_ERR(sdev, "UNIFI_GET_MIB: Failed to copy in mib_data_length back to user\n");
			kmm_free(mib_data);
			r = -EINVAL;
			break;
		}

		/* Copy back the Mib data */
		if (copy_to_user((void *)(arg + 4), mib_data, mib_data_length)) {
			SLSI_ERR(sdev, "UNIFI_GET_MIB: Failed to copy in mib_data back to user\n");
			kmm_free(mib_data);
			r = -EINVAL;
			break;
		}
		kmm_free(mib_data);
		break;
	}
	case UNIFI_SRC_SINK_IOCTL:
		if (sdev->device_state != SLSI_DEVICE_STATE_STARTED) {
			SLSI_ERR(sdev, "UNIFI_SRC_SINK_IOCTL: Device not yet available\n");
			r = -EFAULT;
			break;
		}
		SLSI_ERR(sdev, "UNIFI_SRC_SINK_IOCTL: Not supported\n");
		//r = slsi_src_sink_cdev_ioctl_cfg(sdev, arg);
		break;

	case UNIFI_SOFTMAC_CFG: {
		u32 softmac_cmd;
		u8 cmd_param_size;

		SLSI_ERR(sdev, "UNIFI_SOFTMAC_CFG\n");

		if (copy_from_user((void *)&softmac_cmd, (void *)arg, 4)) {
			SLSI_ERR(sdev, "Failed to get the command\n");
			r = -EFAULT;
			break;
		}
		SLSI_DBG4_NODEV(SLSI_UDI, "softmac_cmd -> %u\n", softmac_cmd);

		arg += sizeof(softmac_cmd);	/* Advance past the command bit */
		if (copy_from_user((void *)&cmd_param_size, (void *)(arg + 4), 1)) {
			SLSI_ERR(sdev, "Failed to get the command size\n");
			r = -EFAULT;
			break;
		}
		SLSI_DBG4_NODEV(SLSI_UDI, "cmd_param_size -> %u\n", cmd_param_size);

		if (cmd_param_size) {
			client->ma_unitdata_filter_config = UDI_MA_UNITDATA_FILTER_ALLOW_EAPOL_ID;
		} else {
			client->ma_unitdata_filter_config = 0;
		}
		break;
	}
	default:
		SLSI_WARN(sdev, "Operation (%d) not supported\n", cmd);
		r = -EINVAL;
	}

	return r;
}

#ifndef CONFIG_DISABLE_POLL
static int slsi_cdev_poll(FAR struct file *filp, struct pollfd *fds, bool setup)
{
	struct slsi_cdev_client *client = (void *)filp->f_priv;

	SLSI_DBG4_NODEV(SLSI_UDI, "Poll(%d), events: %x, revents: %x, setup: %d\n", client->log_list.queue_len, fds->events, fds->revents, setup);

	if (setup == true) {
		if (client->log_list.queue_len) {
			fds->revents |= (fds->events & (POLLIN | POLLOUT));
			if (fds->revents != 0) {
				sem_post(fds->sem);
			}
		} else {
			client->fds = fds;
		}
	} else {
		client->fds = NULL;
	}

	return OK;
}
#endif

/* we know for sure that there is a filter present in log_client->signal_filter if this function is called.
 * we know this because it is called only through a function pointer that is assigned
 * only when a filter is also set up in the log_client
 */
static int send_signal_to_log_filter(struct slsi_log_client *log_client, struct max_buff *mbuf, int dir)
{
	int ret = 0;
	u16 signal_id = fapi_get_sigid(mbuf);

	if (signal_id > log_client->max_signal_id || signal_id < log_client->min_signal_id || !log_client->signal_filter[signal_id - log_client->min_signal_id]) {
		ret = udi_log_event(log_client, mbuf, dir);
	}

	return ret;
}

static int send_signal_to_inverse_log_filter(struct slsi_log_client *log_client, struct max_buff *mbuf, int dir)
{
	int ret = 0;
	u16 signal_id = fapi_get_sigid(mbuf);

	if (signal_id <= log_client->max_signal_id && signal_id >= log_client->min_signal_id && log_client->signal_filter[signal_id - log_client->min_signal_id]) {
		ret = udi_log_event(log_client, mbuf, dir);
	}

	return ret;
}

static bool is_allowed_ip_frame(struct ethhdr *ehdr, u16 signal_id)
{
	u8 *ip_frame = ((u8 *)ehdr) + sizeof(struct ethhdr);
	u8 *ip_data;
	u16 ip_data_offset = 20;
	/*u8  version         = ip_frame[0] >> 4; */
	u8 hlen = ip_frame[0] & 0x0F;
	/*u8  tos             = ip_frame[1]; */
	/*u16 len             = ip_frame[2] << 8 | frame[3]; */
	/*u16 id              = ip_frame[4] << 8 | frame[5]; */
	/*u16 flags_foff      = ip_frame[6] << 8 | frame[7]; */
	/*u8  ttl             = ip_frame[8]; */
	u8 ip_proto = ip_frame[9];

	/*u16 cksum           = ip_frame[10] << 8 | frame[11]; */
	/*u8 *src_ip            = &ip_frame[12]; */
	/*u8 *dest_ip           = &ip_frame[16]; */

	SLSI_UNUSED_PARAMETER(signal_id);

	if (hlen > 5) {
		ip_data_offset += (hlen - 5) * 4;
	}

	ip_data = ip_frame + ip_data_offset;

	switch (ip_proto) {
	case SLSI_IP_TYPE_UDP: {
		u16 srcport = ip_data[0] << 8 | ip_data[1];
		u16 dstport = ip_data[2] << 8 | ip_data[3];

		SLSI_DBG4_NODEV(SLSI_UDI, "FILTER(0x%.4X) Key -> Proto(0x%.4X) -> IpProto(%d) ->UDP(s:%d, d:%d)\n", signal_id, ntohs(ehdr->h_proto), ip_proto, srcport, dstport);
		if (srcport == SLSI_DHCP_CLIENT_PORT || srcport == SLSI_DHCP_SERVER_PORT || dstport == SLSI_DHCP_CLIENT_PORT || dstport == SLSI_DHCP_SERVER_PORT) {
			SLSI_DBG4_NODEV(SLSI_UDI, "FILTER(0x%.4X) Key -> Proto(0x%.4X) -> IpProto(%d) ->UDP(s:%d, d:%d) ALLOW\n", signal_id, ntohs(ehdr->h_proto), ip_proto, srcport, dstport);
			return true;
		}
	}
	default:
		break;
	}

	return false;
}

static int udi_log_event(struct slsi_log_client *log_client, struct max_buff *mbuf, int dir)
{
	struct slsi_cdev_client *client = log_client->log_client_ctx;
	struct udi_msg_t msg;
	u16 signal_id = fapi_get_sigid(mbuf);
	struct timespec ts;
	struct max_buff *mbuf_new;
	u8 *mbuf_data;

	if (WARN_ON(client == NULL)) {
		return -EINVAL;
	}
	if (WARN_ON(mbuf == NULL)) {
		return -EINVAL;
	}
	if (WARN_ON(mbuf->data_len == 0)) {
		return -EINVAL;
	}

	/* Special Filtering of MaPacket frames */
	if (slsi_cdev_unitdata_filter_allow(client, UDI_MA_UNITDATA_FILTER_ALLOW_MASK) && (signal_id == MA_UNITDATA_REQ || signal_id == MA_UNITDATA_IND)) {
		u16 frametype;

		SLSI_DBG4_NODEV(SLSI_UDI, "FILTER(0x%.4X)\n", signal_id);
		if (signal_id == MA_UNITDATA_REQ) {
			frametype = fapi_get_u16(mbuf, u.ma_unitdata_req.data_unit_descriptor);
		} else {
			frametype = fapi_get_u16(mbuf, u.ma_unitdata_ind.data_unit_descriptor);
		}
		SLSI_DBG4_NODEV(SLSI_UDI, "FILTER(0x%.4X) frametype:%d\n", signal_id, frametype);

		if (frametype == FAPI_DATAUNITDESCRIPTOR_IEEE802_3_FRAME) {
			struct ethhdr *ehdr = (struct ethhdr *)fapi_get_data(mbuf);

			if (signal_id == MA_UNITDATA_REQ) {
				ehdr = (struct ethhdr *)fapi_get_data(mbuf);
			}

			if (slsi_cdev_unitdata_filter_allow(client, UDI_MA_UNITDATA_FILTER_ALLOW_EAPOL_ID) || slsi_cdev_unitdata_filter_allow(client, UDI_MA_UNITDATA_FILTER_ALLOW_KEY_ID)) {
				SLSI_DBG4_NODEV(SLSI_UDI, "FILTER(0x%.4X) Eap -> Proto(0x%.4X)\n", signal_id, ntohs(ehdr->h_proto));
				switch (ntohs(ehdr->h_proto)) {
				case ETH_P_PAE:
				case ETH_P_WAI:
					SLSI_DBG4_NODEV(SLSI_UDI, "FILTER(0x%.4X) Eap -> Proto(0x%.4X) ALLOW\n", signal_id, ntohs(ehdr->h_proto));
					goto allow_frame;
				default:
					break;
				}
			}

			if (slsi_cdev_unitdata_filter_allow(client, UDI_MA_UNITDATA_FILTER_ALLOW_KEY_ID)) {
				SLSI_DBG4_NODEV(SLSI_UDI, "FILTER(0x%.4X) Key -> Proto(0x%.4X)\n", signal_id, ntohs(ehdr->h_proto));
				switch (ntohs(ehdr->h_proto)) {
				case ETH_P_ARP:
					SLSI_DBG4_NODEV(SLSI_UDI, "FILTER(0x%.4X) Key -> Proto(0x%.4X) -> Arp ALLOW\n", signal_id, ntohs(ehdr->h_proto));
					goto allow_frame;
				case ETH_P_IP:
					if (is_allowed_ip_frame(ehdr, signal_id)) {
						goto allow_frame;
					}
				default:
					break;
				}
			}
		}
		if (frametype == FAPI_DATAUNITDESCRIPTOR_IEEE802_11_FRAME)
			if (slsi_cdev_unitdata_filter_allow(client, UDI_MA_UNITDATA_FILTER_ALLOW_MGT_ID))
				if (slsi_80211_is_mgmt(fapi_get_mgmt(mbuf)->frame_control)) {
					goto allow_frame;
				}

		SLSI_DBG4_NODEV(SLSI_UDI, "FILTER(0x%.4X) DROP\n", signal_id);

		if (sem_wait(&client->log_mutex)) {
			SLSI_WARN_NODEV("Failed to get udi sem\n");
			sem_post(&client->log_mutex);
			return -ERESTARTSYS;
		}
		if (client->log_drop_data_packets) {
			client->log_dropped_data++;
		}
		sem_post(&client->log_mutex);
		return -ECANCELED;
	}

	/* Special Filtering of MaPacketCfm.
	 * Only log ma_packet_cfm if the tx status != Success
	 */
	if (signal_id == MA_UNITDATA_CFM && slsi_cdev_unitdata_filter_allow(client, UDI_MA_UNITDATA_FILTER_ALLOW_CFM_ERROR_ID))
		if (fapi_get_u16(mbuf, u.ma_unitdata_cfm.transmission_status) == FAPI_TRANSMISSIONSTATUS_SUCCESSFUL) {
			return -ECANCELED;
		}

	/* Exception for driver configuration frames.
	 * these frames must be sent irrespective of number of frames
	 * in queue.
	 */
	if (dir == UDI_CONFIG_IND) {
		goto allow_config_frame;
	}

allow_frame:
	if (sem_wait(&client->log_mutex)) {
		SLSI_WARN_NODEV("Failed to get udi sem\n");
		sem_post(&client->log_mutex);	// release before returning.
		return -ERESTARTSYS;
	}

	/* Handle hitting the UDI_MAX_QUEUED_FRAMES Limit */
	if (client->log_dropped) {
		if (client->log_list.queue_len <= UDI_RESTART_QUEUED_FRAMES) {
			u32 dropped = client->log_dropped;

			SLSI_WARN_NODEV("Stop Dropping UDI Frames : %d frames Dropped\n", dropped);
			client->log_dropped = 0;
			sem_post(&client->log_mutex);
			slsi_kernel_to_user_space_event(log_client, UDI_DRV_DROPPED_FRAMES, sizeof(u32), (u8 *)&dropped);
			return -ECANCELED;
		}
		client->log_dropped++;
		sem_post(&client->log_mutex);
		return -ECANCELED;
	} else if (!client->log_dropped && client->log_list.queue_len >= UDI_MAX_QUEUED_FRAMES) {
		SLSI_WARN_NODEV("Start Dropping UDI Frames\n");
		client->log_dropped++;
		sem_post(&client->log_mutex);
		return -ECANCELED;
	}

	/* Handle hitting the UDI_MAX_QUEUED_DATA_FRAMES Limit
	 * Turn ON the MA_PACKET Filters before we get near the absolute limit of UDI_MAX_QUEUED_FRAMES
	 * This should allow key frames (mgt, dhcp and eapol etc) to still be in the logs but stop the logging general data frames.
	 * This occurs when the Transfer rate is higher than we can take the frames out of the UDI list.
	 */
	if (client->log_drop_data_packets && client->log_list.queue_len < UDI_RESTART_QUEUED_DATA_FRAMES) {
		u32 dropped = client->log_dropped_data;

		SLSI_WARN_NODEV("Stop Dropping UDI Frames : %d Basic Data frames Dropped\n", client->log_dropped_data);
		client->log_drop_data_packets = false;
		client->ma_unitdata_filter_config = 0;
		client->log_dropped_data = 0;
		sem_post(&client->log_mutex);
		slsi_kernel_to_user_space_event(log_client, UDI_DRV_DROPPED_DATA_FRAMES, sizeof(u32), (u8 *)&dropped);
		return -ECANCELED;
	} else if (!client->log_drop_data_packets && client->log_list.queue_len >= UDI_MAX_QUEUED_DATA_FRAMES && !slsi_cdev_unitdata_filter_allow(client, UDI_MA_UNITDATA_FILTER_ALLOW_MASK)) {
		SLSI_WARN_NODEV("Start Dropping UDI Basic Data Frames\n");
		client->log_drop_data_packets = true;
		client->ma_unitdata_filter_config = UDI_MA_UNITDATA_FILTER_ALLOW_MGT_ID | UDI_MA_UNITDATA_FILTER_ALLOW_KEY_ID | UDI_MA_UNITDATA_FILTER_ALLOW_CFM_ERROR_ID | UDI_MA_UNITDATA_FILTER_ALLOW_EAPOL_ID;
	}
	sem_post(&client->log_mutex);

allow_config_frame:

	mbuf_new = mbuf_alloc(mbuf->data_len + sizeof(msg));
	if (mbuf_new == NULL) {
		return -ENOMEM;
	}

	msg.length = sizeof(msg) + mbuf->data_len;
	clock_gettime(CLOCK_REALTIME, &ts);
	msg.timestamp = (ts.tv_sec * 1000) + (ts.tv_nsec / 1000);
	msg.direction = dir;
	msg.signal_length = fapi_get_siglen(mbuf);

	mbuf_put(mbuf_new, sizeof(msg) + mbuf->data_len);
	mbuf_data = slsi_mbuf_get_data(mbuf_new);
	memcpy(mbuf_data, &msg, sizeof(msg));
	memcpy(&mbuf_data[sizeof(msg)], slsi_mbuf_get_data(mbuf), mbuf->data_len);

	slsi_mbuf_queue_tail(&client->log_list, mbuf_new);

	if (client->fds != NULL) {
		client->fds->revents |= (client->fds->events & (POLLIN | POLLOUT));
		if (client->fds->revents != 0) {
			sem_post(client->fds->sem);
		}
	}

	/* Wake any waiting user process */
	complete(&client->log_wq);

	return 0;
}

static const struct file_operations slsi_cdev_fops = {
	.open = slsi_cdev_open,
	.close = slsi_cdev_close,
	.read = slsi_cdev_read,
	.write = slsi_cdev_write,
	.ioctl = slsi_cdev_ioctl,
#ifndef CONFIG_DISABLE_POLL
	.poll = slsi_cdev_poll
#endif
};

static int slsi_get_minor(void)
{
	int minor;

	for (minor = 0; minor < SLSI_UDI_MINOR_NODES; minor++)
		if (uf_cdevs[minor] == NULL) {
			return minor;
		}
	return -1;
}

static int slsi_cdev_create(struct slsi_dev *sdev, int minor)
{
	struct slsi_cdev *pdev;

	SLSI_DBG3_NODEV(SLSI_UDI, "\n");
	minor = slsi_get_minor();
	if (minor < 0) {
		SLSI_ERR(sdev, "no minor numbers available\n");
		return -ENOMEM;
	}

	pdev = kmm_malloc(sizeof(*pdev));
	if (pdev == NULL) {
		return -ENOMEM;
	}
	memset(pdev, 0, sizeof(*pdev));

	pdev->minor = minor;
	pdev->sdev = sdev;
	sdev->uf_cdev = (void *)pdev;
	sdev->procfs_instance = minor;
	uf_cdevs[minor] = pdev;

	return 0;
}

static void slsi_cdev_destroy(struct slsi_dev *sdev, int minor)
{
	struct slsi_cdev *pdev = (struct slsi_cdev *)uf_cdevs[minor];

	if (!pdev) {
		return;
	}

	SLSI_DBG1(sdev, SLSI_UDI, "\n");
	while (slsi_check_cdev_refs()) {
		SLSI_ERR(sdev, "UDI Client still attached. Please Terminate!\n");
		//msleep(1000);
	}

	/* There exist a possibility of race such that the
	 *
	 * - file operation release callback (slsi_cdev_release) is called
	 * - the cdev client structure is freed
	 * - the context is pre-empted and this context (slsi_cdev_destroy) is executed
	 * - slsi_cdev_destroy deletes cdev and hence the kobject embedded inside cdev
	 *   and returns
	 * - the release context again executes and operates on a non-existent kobject
	 *   leading to kernel Panic
	 *
	 * Ideally the kernel should protect against such race. But it is not!
	 * So we check here that the file operation release callback is complete by
	 * checking the refcount in the kobject embedded in cdev structure.
	 * The refcount is initialized to 1; so anything more than that means
	 * there exists attached clients.
	 */

	sdev->uf_cdev = NULL;
	uf_cdevs[pdev->minor] = NULL;
	kmm_free(pdev);
}

static int udi_initialised;

int slsi_udi_init(void)
{
	int ret;

	memset(uf_cdevs, 0, sizeof(uf_cdevs));

	SLSI_DBG1_NODEV(SLSI_UDI, "Registering /dev/%s\n", UDI_CHAR_DEVICE_NAME0);
	ret = register_driver("/dev/" UDI_CHAR_DEVICE_NAME0, &slsi_cdev_fops, 0666, NULL);
	if (ret) {
		SLSI_ERR_NODEV("Failed to register /dev/%s (ret: %d)\n", UDI_CHAR_DEVICE_NAME0, ret);
		return ret;
	}

	SLSI_DBG1_NODEV(SLSI_UDI, "Registering /dev/%s\n", UDI_CHAR_DEVICE_NAME1);
	ret = register_driver("/dev/" UDI_CHAR_DEVICE_NAME1, &slsi_cdev_fops, 0666, NULL);
	if (ret) {
		unregister_driver("/dev/" UDI_CHAR_DEVICE_NAME0);
		SLSI_ERR_NODEV("Failed to register /dev/%s (ret: %d)\n", UDI_CHAR_DEVICE_NAME1, ret);
		return ret;
	}

	udi_initialised = 1;

	return 0;
}

int slsi_udi_deinit(void)
{
	if (!udi_initialised) {
		return -1;
	}

	SLSI_DBG1_NODEV(SLSI_UDI, "Unregiser /dev/%s\n", UDI_CHAR_DEVICE_NAME0);
	unregister_driver("/dev/" UDI_CHAR_DEVICE_NAME0);

	SLSI_DBG1_NODEV(SLSI_UDI, "Unregiser /dev/%s\n", UDI_CHAR_DEVICE_NAME1);
	unregister_driver("/dev/" UDI_CHAR_DEVICE_NAME1);

	udi_initialised = 0;
	return 0;
}

int slsi_udi_node_init(struct slsi_dev *sdev)
{
	slsi_cdev_create(sdev, 0);
	slsi_cdev_create(sdev, 1);
	return 0;
}

int slsi_udi_node_deinit(struct slsi_dev *sdev)
{
	slsi_cdev_destroy(sdev, 0);
	slsi_cdev_destroy(sdev, 1);
	return 0;
}
