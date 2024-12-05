/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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

#include "include/CSIPacketReceiver.h"
#include <sys/ioctl.h>
#include <tinyara/wifi_csi/wifi_csi.h>
#include <tinyara/wifi_csi/wifi_csi_struct.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>

#define RCVR_TH_NAME "csifw_receiver"

#define OPEN_DRIVER_OR_EXIT(_FD) \
	_FD = open(CONFIG_WIFICSI_CUSTOM_DEV_PATH, O_RDONLY);\
	if (_FD < 0) {\
		CSIFW_LOGE("Failed to open device path : %s errno : %d", CONFIG_WIFICSI_CUSTOM_DEV_PATH, get_errno());\
		return CSIFW_ERROR;\
	}\

#define CLOSE_DRIVER_OR_EXIT(_FD) close(_FD);

extern int g_pkt_count;
static pthread_t gCSIDataReceiver;
static unsigned char *g_get_data_buffptr; // the buffer to get data from driver
static uint16_t gCSIRawBufLen;
static CSIDataListener gCSIDataCallback;
static bool g_csi_thread_stop = true;
static bool g_csi_enabled; // this flag maintains wifi_csi_config enable state during wifi re-connection
static bool g_skip_pkt;
static mqd_t g_mq_handle;

typedef enum CSI_SET_CONFIG_CMD {
	CSIFW_CONFIG_INIT,
	CSIFW_CONFIG_ENABLE,
	CSIFW_CONFIG_DISABLE
} CSI_SET_CONFIG_CMD;

CSIFW_RES csi_packet_receiver_set_csi_config(csi_action_param_t *config, CSI_SET_CONFIG_CMD config_cmd);

static int readCSIData(int fd,char* buf, int size)
{
	int err=0;
	csi_driver_buffer_args_t buf_args;
	buf_args.buflen = size;
	buf_args.buffer = buf;
	/* ToDo: store length in buf_args not in err*/
	err = ioctl(fd, CSIIOC_GET_DATA, (unsigned long)&buf_args);
	if (err <= OK) {
		CSIFW_LOGE("Fail to ioctl(%d, CSIIOC_GET_DATA), ret: %d, errno : %d", fd, err, get_errno());
	}
	return err;
}

static void* dataReceiverThread(void *vargp) {
	int len;
	int fd, ret;
	/* open mq */
	struct mq_attr attr_mq;
	attr_mq.mq_maxmsg = CSI_MQ_MSG_COUNT;
	attr_mq.mq_msgsize = sizeof(struct wifi_csi_msg_s);
	attr_mq.mq_flags = 0;
	CSIFW_LOGD("MQ_NAME: %s, MSG_SIZE: %zu, MQ_TYPE: BLOCKING", CSI_MQ_NAME, attr_mq.mq_msgsize);
	g_mq_handle = mq_open(CSI_MQ_NAME, O_RDWR | O_CREAT, 0666, &attr_mq);
	if (g_mq_handle == (mqd_t)ERROR) {
		CSIFW_LOGE("Failed to open mq %d \n", get_errno());
		return NULL;
	}
	OPEN_DRIVER_OR_EXIT(fd)
	// start csi data from driver
	ret = ioctl(fd, CSIIOC_START_CSI , NULL);
	if (ret < OK) {
		CSIFW_LOGE("Fail to ioctl(%d, CSIIOC_START_CSI), errno : %d", fd, get_errno());
		mq_close(g_mq_handle);
		CLOSE_DRIVER_OR_EXIT(fd)
		return NULL;
	}
	CSIFW_LOGI("Driver data collect started");
	
	int prio;
	size_t size;
	struct wifi_csi_msg_s msg;

	while (!g_csi_thread_stop) {
		size = mq_receive(g_mq_handle, (char *)&msg, sizeof(msg), &prio);
		if (size != sizeof(msg)) {
			CSIFW_LOGD("Interrupted while waiting for deque message from kernel %zu, errono: %d", size, errno);
		} else {
			switch (msg.msgId) {
			case CSI_MSG_DATA_READY_CB:
				if (g_skip_pkt && g_pkt_count <= 0) {
					g_pkt_count--;
					CSIFW_LOGD("Skipping extra packet number: [%d] size: %d", g_pkt_count, msg.data_len);
					continue;
				}
				if (msg.data_len == 0 || msg.data_len > CSIFW_MAX_RAW_BUFF_LEN) {
					CSIFW_LOGE("Skipping packet: invalid data length: %d", msg.data_len);
					continue;
				}
				g_pkt_count--;
				gCSIRawBufLen = msg.data_len;
				len = readCSIData(fd, g_get_data_buffptr, gCSIRawBufLen);
				if (len < 0) {
					CSIFW_LOGE("Skipping packet: error: %d", len);
					continue;
				}
				if (len != gCSIRawBufLen - CSIFW_CSI_HEADER_LEN) {
					CSIFW_LOGE("Invalid csi data packet, gCSIRawBufLen: %d, new size (w/o header): %d", gCSIRawBufLen, len);
					// continue; ==> Cant skip as the message queue call might not be in sync in case of error packets.
					gCSIRawBufLen = len + CSIFW_CSI_HEADER_LEN;
				}
				gCSIDataCallback(CSIFW_OK, gCSIRawBufLen, g_get_data_buffptr, len);
			break;

			case CSI_MSG_ERROR:
				CSIFW_LOGE("CSI_MSG_ERROR received");
			break;

			default:
				CSIFW_LOGE("Received unknown message ID: %d", msg.msgId);
			break;
			}
		}
	}
	CSIFW_LOGD("dataReceiverThread: Stopping thread");
	//close mq
	mq_close(g_mq_handle);
	CSIFW_LOGD("dataReceiverThread: MQ closed");
	// stop csi data from driver
	ret = ioctl(fd, CSIIOC_STOP_CSI , NULL);
	if (ret < OK) {
		CSIFW_LOGE("Fail to ioctl(%d, CSIIOC_STOP_CSI), errno : %d", fd, get_errno());
	} else {
		CSIFW_LOGD("Driver data collect stopped");
	}
	CLOSE_DRIVER_OR_EXIT(fd)
	CSIFW_LOGD("[dataReceiverThread] THREAD EXIT");
	return NULL;
}

CSIFW_RES csi_packet_receiver_init(CSIDataListener CSIDataCallback) {
	g_csi_enabled = false;
	gCSIRawBufLen = CSIFW_MAX_RAW_BUFF_LEN;
	gCSIDataCallback = CSIDataCallback;
	return CSIFW_OK;
}

CSIFW_RES csi_packet_receiver_set_csi_config(csi_action_param_t *config, CSI_SET_CONFIG_CMD config_cmd) {

	int fd, ret;
	OPEN_DRIVER_OR_EXIT(fd)
	switch (config_cmd) {
		case CSIFW_CONFIG_INIT:
			config->act = 1;
			config->enable = 0;
			// memset config to 0
			CSIFW_LOGI("Memset 0 config param before setting config");
			ret = ioctl(fd, CSIIOC_PARAM_SETZERO, NULL);
			if (ret < OK) {
				CSIFW_LOGE("Fail to ioctl(%d, CSIIOC_PARAM_SETZERO ), errno : %d", fd, get_errno());
				CLOSE_DRIVER_OR_EXIT(fd)
				return CSIFW_ERROR;
			}
			CSIFW_LOGI("Set CSI config");
			break;

		case CSIFW_CONFIG_ENABLE:
			config->act = 0;
			config->enable = 1;
			break;
		case CSIFW_CONFIG_DISABLE:
			config->act = 0;
			config->enable = 0;
			break;
		default:
			CSIFW_LOGE("Invalid command");
			CLOSE_DRIVER_OR_EXIT(fd)
			return CSIFW_INVALID_ARG;
	}

	ret = ioctl(fd, CSIIOC_SET_CONFIG, (unsigned long)config);
	if (ret < OK) {
		CSIFW_LOGE("Fail to ioctl(%d, CSIIOC_SET_CONFIG ), errno : %d", fd, get_errno());
		CLOSE_DRIVER_OR_EXIT(fd)
		return CSIFW_ERROR;
	}
    CLOSE_DRIVER_OR_EXIT(fd)
	return CSIFW_OK;
}

CSIFW_RES csi_packet_receiver_get_mac_addr(csifw_mac_info *mac_info) {
	int fd;
	csifw_mac_info *mac_info_t = (csifw_mac_info *)mac_info; 
	OPEN_DRIVER_OR_EXIT(fd)
	int ret = ioctl(fd, CSIIOC_GET_MAC_ADDR, (unsigned long)(mac_info_t));
	if (ret < OK) {
		CSIFW_LOGE("Fail to ioctl(%d, CSIIOC_SET_CONFIG ), errno : %d", fd, get_errno());
		return CSIFW_ERROR;
	}
    CLOSE_DRIVER_OR_EXIT(fd)
	CSIFW_LOGD("Got mac address from driver: [%02x:%02x:%02x:%02x:%02x:%02x]", mac_info->mac_addr[0], mac_info->mac_addr[1], mac_info->mac_addr[2], mac_info->mac_addr[3], mac_info->mac_addr[4], mac_info->mac_addr[5]);
	return CSIFW_OK;
}

CSIFW_RES csi_packet_receiver_start_collect(csi_action_param_t *config) {
	CSIFW_RES res;
	// allocate buffer for receiveing data from driver
	if (!g_get_data_buffptr) {
		g_get_data_buffptr = (unsigned char *)malloc(CSIFW_MAX_RAW_BUFF_LEN);
		if (!g_get_data_buffptr) {
			CSIFW_LOGE("Buffer allocation fail.");
			return CSIFW_ERROR;
		}
		CSIFW_LOGD("Get data buffer allocation done, size: %d", gCSIRawBufLen);
	}
	g_csi_thread_stop = false;

	//create receiver thread
	pthread_attr_t recv_th_attr;
	pthread_attr_init(&recv_th_attr);
	/* ToDo: stack size of thread needs to be optimized */
	pthread_attr_setstacksize(&recv_th_attr, (10*1024));
	if (pthread_create(&gCSIDataReceiver, &recv_th_attr, dataReceiverThread, NULL) != 0) {
		CSIFW_LOGE("Failed to create csi data receiver thread");
		return CSIFW_ERROR;
	}
	if (pthread_setname_np(gCSIDataReceiver, RCVR_TH_NAME) != 0) {
		CSIFW_LOGE("Error in setting receiver thread name, error_no: %d", get_errno());
	}
	CSIFW_LOGD("CSI data receive thread created");

	// set config
	res = csi_packet_receiver_set_csi_config(config, CSIFW_CONFIG_INIT);
	if (res != CSIFW_OK) {
		return res;
	}

	if (g_csi_enabled) {
		// disable wifi csi report
		CSIFW_LOGD("Disabling CSI config");
		res = csi_packet_receiver_set_csi_config(config, CSIFW_CONFIG_DISABLE);
		if (res != CSIFW_OK) {
			return res;
		}
		g_csi_enabled = false;
		// set config
		CSIFW_LOGD("[AGAIN] SET CSI config");
		res = csi_packet_receiver_set_csi_config(config, CSIFW_CONFIG_INIT);
		if (res != CSIFW_OK) {
			return res;
		}
	}
	// enable csi report
	CSIFW_LOGD("Enabling CSI config");
	res = csi_packet_receiver_set_csi_config(config, CSIFW_CONFIG_ENABLE);
	if (res != CSIFW_OK) {
		return res;
	}
	g_csi_enabled = true;
	return CSIFW_OK;
}

CSIFW_RES csi_packet_receiver_stop_collect(CSIFW_REASON reason, csi_action_param_t *config)
{
	//join thread
	g_csi_thread_stop = true;
	// send dummy message to close blocking mq
	if (g_mq_handle) {
		CSIFW_LOGD("Sending dummy message to close blocking mq");
		struct wifi_csi_msg_s msg;
		msg.msgId = CSI_MSG_ERROR;
		mq_send(g_mq_handle, (FAR const char *)&msg, sizeof(msg), MQ_PRIO_MAX);
	}
	pthread_join(gCSIDataReceiver, NULL);
	CSIFW_LOGD("Receiver thread join done");
	if (g_get_data_buffptr) {
		free(g_get_data_buffptr);
		g_get_data_buffptr = NULL;
	}
	if (reason == CSIFW_WIFI_DISCONNECTED) {
		CSIFW_LOGI("Disable not required as wifi disconnected");
		CSIFW_LOGI("csi data collect stopped");
		return CSIFW_OK;
	}
	
	int fd;
	// else-if WIFI_CONNECTED disable wifi csi report
	OPEN_DRIVER_OR_EXIT(fd)
	config->act = 0;
	config->enable = 0;
	CSIFW_LOGI("Disabling CSI config");
	int ret = ioctl(fd, CSIIOC_SET_CONFIG, (unsigned long)config);
	if (ret < OK) {
		CSIFW_LOGE("Fail to ioctl(%d, CSIIOC_SET_CONFIG ), errno : %d", fd, get_errno());
		return CSIFW_ERROR;
	}
    CLOSE_DRIVER_OR_EXIT(fd)
	CSIFW_LOGD("csi data collect stopped");
	g_csi_enabled = false;
	return CSIFW_OK;
}

CSIFW_RES csi_packet_receiver_deinit()
{
	CSIFW_LOGI("csi_packet_receiver_deinit");
	gCSIDataCallback = NULL;
	CSIFW_LOGD("csi_packet_receiver_deinit done");
	return CSIFW_OK;
}

