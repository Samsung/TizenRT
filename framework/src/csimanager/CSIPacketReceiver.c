/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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

#include <fcntl.h>
#include <errno.h>
#include "mqueue.h"
#include <pthread.h>
#include <sys/ioctl.h>
#include "CSIPacketReceiver.h"
#include <tinyara/wifi_csi/wifi_csi.h>

#define RCVR_TH_NAME "csifw_data_receiver_th"

static inline CSIFW_RES open_driver(int *fd) {
  *fd = open(CONFIG_WIFICSI_CUSTOM_DEV_PATH, O_RDONLY);
  if (*fd < 0) {
    CSIFW_LOGE("Failed to open device path : %s errno : %d",
               CONFIG_WIFICSI_CUSTOM_DEV_PATH, get_errno());
    return CSIFW_ERROR;
  }
  return CSIFW_OK;
}

static inline void close_driver(int fd) {
  close(fd);
}

CSIFW_RES csi_packet_receiver_set_csi_config(csi_config_action_t config_action);

static int readCSIData(int fd, unsigned char *buf, int size)
{
	int err = 0;
	csi_driver_buffer_args_t buf_args;
	buf_args.buflen = size;
	buf_args.buffer = buf;
	err = ioctl(fd, CSIIOC_GET_DATA, (unsigned long)&buf_args);
	if (err <= OK) {
		CSIFW_LOGE("IOCTL CSIIOC_GET_DATA failed - fd: %d, errno: %d (%s)", 
			fd, get_errno(), strerror(get_errno()));
	}
	return err;
}

static void *dataReceiverThread(void *vargp)
{
	csifw_context_t *p_csifw_ctx = get_csifw_context();
	if (!p_csifw_ctx) {
		CSIFW_LOGE("Invalid context pointer (NULL)");
		return NULL;
	}

	int len;
	int prio;
	size_t size;
	int csi_data_len = 0;
	struct wifi_csi_msg_s msg;
	int fd = p_csifw_ctx->data_receiver_fd;
	mqd_t mq_handle = p_csifw_ctx->mq_handle;
	unsigned char *get_data_buffptr = p_csifw_ctx->get_data_buffptr;

	while (!p_csifw_ctx->data_receiver_thread_stop) {
		size = mq_receive(mq_handle, (char *)&msg, sizeof(msg), &prio);
		if (size != sizeof(msg)) {
			CSIFW_LOGE("Interrupted while waiting for deque message from kernel %zu, errno: %d (%s)", size, get_errno(), strerror(get_errno()));
		} else {
			switch (msg.msgId) {
			case CSI_MSG_DATA_READY_CB:
				if (msg.data_len == 0 || msg.data_len > CSIFW_MAX_RAW_BUFF_LEN) {
					CSIFW_LOGE("Skipping packet: invalid data length: %d", msg.data_len);
					continue;
				}
				csi_data_len = msg.data_len;
				len = readCSIData(fd, get_data_buffptr, csi_data_len);
				if (len < 0) {
					CSIFW_LOGE("Skipping packet: error: %d", len);
					continue;
				}
				if (len != csi_data_len - CSIFW_CSI_HEADER_LEN) {
					csi_data_len = len + CSIFW_CSI_HEADER_LEN;
				}
				p_csifw_ctx->CSI_DataCallback(CSIFW_OK, csi_data_len, get_data_buffptr, len);
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
	CSIFW_LOGD("[THREAD] : EXIT");
	return NULL;
}

CSIFW_RES csi_packet_callback_register(CSIDataListener CSIDataCallback)
{
	CSIFW_LOGD("Initializing packet receiver with callback: %p", CSIDataCallback);
	csifw_context_t *p_csifw_ctx = get_csifw_context();
	if (!p_csifw_ctx) {
		CSIFW_LOGE("Invalid context pointer (NULL)");
		return CSIFW_ERROR_NOT_INITIALIZED;
	}
	p_csifw_ctx->CSI_DataCallback = CSIDataCallback;
	p_csifw_ctx->data_receiver_thread_stop = false;
	p_csifw_ctx->mq_handle = (mqd_t) ERROR;
	p_csifw_ctx->get_data_buffptr = NULL;
	CSIFW_LOGD("Packet receiver initialized successfully");
	return CSIFW_OK;
}

CSIFW_RES csi_packet_receiver_set_csi_config(csi_config_action_t config_action)
{
	csifw_context_t *p_csifw_ctx = get_csifw_context();
	if (!p_csifw_ctx) {
		CSIFW_LOGE("Invalid context pointer (NULL)");
		return CSIFW_ERROR_NOT_INITIALIZED;
	}
	int fd;
	CSIFW_RES res = open_driver(&fd);
	if (res != CSIFW_OK) {
		return res;
	}
	int ret;
	csi_config_args_t config_args;
	config_args.config_action = config_action;
	config_args.config_type = p_csifw_ctx->csi_config;
	config_args.interval = p_csifw_ctx->csi_interval;
	CSIFW_LOGI("Setting CSI config (type: %d, interval: %d)", config_args.config_type, config_args.interval);
	ret = ioctl(fd, CSIIOC_SET_CONFIG, (unsigned long)&config_args);
	if (ret < OK) {
		CSIFW_LOGE("Failed to set CSI config (action: %d, type: %d, interval: %d) - errno: %d (%s)",
		config_action, config_args.config_type, config_args.interval,
		get_errno(), strerror(get_errno()));
		res = CSIFW_ERROR;
	} else {
		CSIFW_LOGD("IOCTL : CSIIOC_SET_CONFIG, Success");
	}
	close_driver(fd);
	return res;
}

CSIFW_RES csi_packet_receiver_change_interval(void)
{
	csifw_context_t *p_csifw_ctx = get_csifw_context();
	if (!p_csifw_ctx) {
		CSIFW_LOGE("Invalid context pointer (NULL)");
		return CSIFW_ERROR_NOT_INITIALIZED;
	}
	CSIFW_RES res = CSIFW_OK;
	res = csi_packet_receiver_set_csi_config(CSI_CONFIG_DISABLE);
	if (res != CSIFW_OK) {
		CSIFW_LOGE("Failed to disable CSI configuration - error: %d", res);
		return res;
	}
	usleep(10000);				//10ms sleep
	res = csi_packet_receiver_set_csi_config(CSI_CONFIG_ENABLE);
	if (res != CSIFW_OK) {
		CSIFW_LOGE("Failed to enable CSI configuration - error: %d", res);
	}
	return res;
}

CSIFW_RES csi_packet_receiver_get_mac_addr(csifw_mac_info *mac_info)
{
	int fd;
	CSIFW_RES res = open_driver(&fd);
	if (res != CSIFW_OK) {
		return res;
	}
	int ret = ioctl(fd, CSIIOC_GET_MAC_ADDR, (unsigned long)(mac_info));
	if (ret < OK) {
		CSIFW_LOGE("IOCTL : CSIIOC_GET_MAC_ADDR Failed errno: %d (%s)", get_errno(), strerror(get_errno()));
		close_driver(fd);
		return CSIFW_ERROR;
	}
	close_driver(fd);
	CSIFW_LOGD("MAC address from driver: [%02x:%02x:%02x:%02x:%02x:%02x]", 
		mac_info->mac_addr[0], mac_info->mac_addr[1], 
		mac_info->mac_addr[2], mac_info->mac_addr[3], 
		mac_info->mac_addr[4], mac_info->mac_addr[5]);
	return CSIFW_OK;
}

CSIFW_RES csi_packet_receiver_start_collect(void)
{
	CSIFW_RES res = CSIFW_OK;
	csifw_context_t *p_csifw_ctx = get_csifw_context();
	if (!p_csifw_ctx) {
		CSIFW_LOGE("Invalid context pointer (NULL)");
		return CSIFW_ERROR_NOT_INITIALIZED;
	}
	// allocate buffer for receiveing data from driver
	if (!p_csifw_ctx->get_data_buffptr) {
		p_csifw_ctx->get_data_buffptr = (unsigned char *)malloc(CSIFW_MAX_RAW_BUFF_LEN);
		if (!p_csifw_ctx->get_data_buffptr) {
			CSIFW_LOGE("Buffer allocation Fail.");
			return CSIFW_ERROR;
		}
		CSIFW_LOGD("Get data buffer allocation done, size: %d", CSIFW_MAX_RAW_BUFF_LEN);
	}
	if (p_csifw_ctx->disable_required) {
		CSIFW_LOGD("WiFi reconnected: Disabling CSI config");
		csi_packet_receiver_set_csi_config(CSI_CONFIG_ENABLE);
		csi_packet_receiver_set_csi_config(CSI_CONFIG_DISABLE);
		p_csifw_ctx->disable_required = false;
	}
	CSIFW_LOGD("Enabling CSI config");
	res = csi_packet_receiver_set_csi_config(CSI_CONFIG_ENABLE);
	if (res != CSIFW_OK) {
		return res;
	}
	p_csifw_ctx->data_receiver_thread_stop = false;

	pthread_attr_t recv_th_attr;
	if (pthread_attr_init(&recv_th_attr) != 0) {
		CSIFW_LOGE("Failed to initialize receiver thread attributes - errno: %d (%s)", get_errno(), strerror(get_errno()));
		csi_packet_receiver_set_csi_config(CSI_CONFIG_DISABLE);
		free(p_csifw_ctx->get_data_buffptr);
		p_csifw_ctx->get_data_buffptr = NULL;
		return CSIFW_ERROR;
	}
	if (pthread_attr_setstacksize(&recv_th_attr, (3*1024)) != 0) {
		CSIFW_LOGE("Failed to set receiver thread stack size to 5KB - errno: %d (%s). Proceeding with default stack size.", get_errno(), strerror(get_errno()));
	}
	if (pthread_create(&p_csifw_ctx->csi_data_receiver_th, &recv_th_attr, dataReceiverThread, NULL) != 0) {
		CSIFW_LOGE("Failed to create CSI data receiver thread - errno: %d (%s)", get_errno(), strerror(get_errno()));
		csi_packet_receiver_set_csi_config(CSI_CONFIG_DISABLE);
		free(p_csifw_ctx->get_data_buffptr);
		p_csifw_ctx->get_data_buffptr = NULL;
		return CSIFW_ERROR;
	}
	if (pthread_setname_np(p_csifw_ctx->csi_data_receiver_th, RCVR_TH_NAME) != 0) {
		CSIFW_LOGE("Failed to set receiver thread name - errno: %d (%s)", get_errno(), strerror(get_errno()));
	}
	CSIFW_LOGD("CSI Data_Receive_Thread created (thread ID: %lu)", (unsigned long)p_csifw_ctx->csi_data_receiver_th);
	return res;
}

CSIFW_RES csi_packet_receiver_initialize(void)
{
	csifw_context_t *p_csifw_ctx = get_csifw_context();
	if (!p_csifw_ctx) {
		CSIFW_LOGE("Invalid context pointer (NULL)");
		return CSIFW_ERROR_NOT_INITIALIZED;
	}
	int fd;
	mqd_t mq_handle = (mqd_t) ERROR;
	struct mq_attr attr_mq;
	attr_mq.mq_maxmsg = CSI_MQ_MSG_COUNT;
	attr_mq.mq_msgsize = sizeof(struct wifi_csi_msg_s);
	attr_mq.mq_flags = 0;
	CSIFW_LOGD("MQ_NAME: %s, MSG_SIZE: %zu, MQ_TYPE: BLOCKING", CSI_MQ_NAME, attr_mq.mq_msgsize);
	CSIFW_LOGD("Opening message queue: name=%s, maxmsg=%ld, msgsize=%ld",CSI_MQ_NAME, attr_mq.mq_maxmsg, attr_mq.mq_msgsize);
	mq_handle = mq_open(CSI_MQ_NAME, O_RDWR | O_CREAT, 0666, &attr_mq);
	if (mq_handle == (mqd_t) ERROR) {
		CSIFW_LOGE("Failed to open message queue %s. errno: %d (%s)",
			CSI_MQ_NAME, get_errno(), strerror(get_errno()));
		return CSIFW_ERROR;
	}
	CSIFW_LOGD("Message queue opened successfully (mq_handle=%p)", mq_handle);
	CSIFW_RES res = open_driver(&fd);
	if (res != CSIFW_OK) {
		mq_close(mq_handle);
		mq_handle = (mqd_t) ERROR;
		return res;
	}
	CSIFW_LOGD("Starting CSI data collection via IOCTL (fd=%d)", fd);
	int ret = ioctl(fd, CSIIOC_START_CSI, NULL);
	if (ret < OK) {
		CSIFW_LOGE("IOCTL : CSIIOC_START_CSI Failed errno: %d (%s)", get_errno(), strerror(get_errno()));
		mq_close(mq_handle);
		mq_handle = (mqd_t) ERROR;
		close_driver(fd);
		return CSIFW_ERROR;
	}
	CSIFW_LOGD("Driver data collection started successfully (fd=%d)", fd);
	p_csifw_ctx->data_receiver_fd = fd;
	p_csifw_ctx->mq_handle = mq_handle;
    return CSIFW_OK;
}

CSIFW_RES csi_packet_receiver_cleanup(void)
{
	csifw_context_t *p_csifw_ctx = get_csifw_context();
	if (!p_csifw_ctx) {
		CSIFW_LOGE("Invalid context pointer (NULL)");
		return CSIFW_ERROR_NOT_INITIALIZED;
	}
	int fd = p_csifw_ctx->data_receiver_fd;
	mq_close(p_csifw_ctx->mq_handle);
	p_csifw_ctx->mq_handle = (mqd_t) ERROR;
	CSIFW_LOGD("Data Receiver MQ closed (fd: %d, mq_handle: %p)", 
		p_csifw_ctx->data_receiver_fd, p_csifw_ctx->mq_handle);
	int ret = ioctl(fd, CSIIOC_STOP_CSI, NULL);
	if (ret < OK) {
		CSIFW_LOGE("IOCTL : CSIIOC_STOP_CSI Failed errno: %d (%s)", get_errno(), strerror(get_errno()));
		close_driver(fd);
		return CSIFW_ERROR;
	} else {
		CSIFW_LOGD("Driver data collect stopped");
	}
	close_driver(fd);
	return CSIFW_OK;
}

CSIFW_RES csi_packet_receiver_stop_collect(void)
{
	CSIFW_RES res = CSIFW_OK;
	csifw_context_t *p_csifw_ctx = get_csifw_context();
	if (!p_csifw_ctx) {
		CSIFW_LOGE("Invalid context pointer (NULL)");
		return CSIFW_ERROR_NOT_INITIALIZED;
	}
	p_csifw_ctx->data_receiver_thread_stop = true;
	if (p_csifw_ctx->mq_handle != (mqd_t) ERROR) {
		CSIFW_LOGD("Sending dummy message to close blocking mq");
		struct wifi_csi_msg_s msg;
		msg.msgId = CSI_MSG_ERROR;
		mq_send(p_csifw_ctx->mq_handle, (FAR const char *)&msg, sizeof(msg), MQ_PRIO_MAX);
	} else {
		CSIFW_LOGE("Failed to close blocking mq as MQ_Discriptor is Invalid");
	}
	pthread_join(p_csifw_ctx->csi_data_receiver_th, NULL);
	CSIFW_LOGD("Receiver thread join done");
	if (p_csifw_ctx->get_data_buffptr) {
		free(p_csifw_ctx->get_data_buffptr);
		p_csifw_ctx->get_data_buffptr = NULL;
	}
	if (p_csifw_ctx->disable_required) {
		p_csifw_ctx->disable_required = false;
		res = csi_packet_receiver_set_csi_config(CSI_CONFIG_DISABLE);
		if (res != CSIFW_OK) {
			return res;
		}
		CSIFW_LOGD("CSI Data Collection Stopped");
		return res;
	}
	CSIFW_LOGI("Disable not required as wifi disconnected");
	CSIFW_LOGI("CSI Data Collection Stopped");
	return CSIFW_OK;
}

CSIFW_RES csi_packet_receiver_deinit(void)
{
	csifw_context_t *p_csifw_ctx = get_csifw_context();
	if (!p_csifw_ctx) {
		CSIFW_LOGE("Invalid context pointer (NULL)");
		return CSIFW_ERROR_NOT_INITIALIZED;
	}
	p_csifw_ctx->CSI_DataCallback = NULL;
	CSIFW_LOGD("CSI packet receiver successfully deinitialized (callback: %p)", p_csifw_ctx->CSI_DataCallback);
	return CSIFW_OK;
}
