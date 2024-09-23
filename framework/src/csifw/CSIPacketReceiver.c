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

#define HNDLR_TH_NAME "csifw_data_handler"
#define RCVR_TH_NAME "csifw_receiver"
#define MQ_NAME "WIFI_CSI_DRIVER_Q"
#define RB_HDR_LEN sizeof(uint16_t) // 2 Bytes reserved at start of ring buffer to store data size
#define CSIFW_MQ_MSG_COUNT 5

#define OPEN_DRIVER_OR_EXIT(_FD) \
	_FD = open(CONFIG_WIFICSI_CUSTOM_DEV_PATH, O_RDONLY);\
	if (_FD < 0) {\
		CSIFW_LOGE("Failed to open device path : %s errno : %d", CONFIG_WIFICSI_CUSTOM_DEV_PATH, get_errno());\
		return CSIFW_ERROR;\
	}\

#define CLOSE_DRIVER_OR_EXIT(_FD) close(_FD);

static pthread_t gCSIDataReceiver; 
static pthread_t gCSIDataHandlerWorker; 
static unsigned char *g_send_data_buffptr; // the buffer to send to services
static unsigned char *g_get_data_buffptr; // the buffer to get data from driver and copy to ring buffer
static rb_t g_raw_rb_t;	// the ring buffer object
static rb_p g_raw_rb_p;	// the ring buffer pointer
static uint16_t gCSIRawBufLen;
static CSIDataListener gCSIDataCallback;
static bool g_csi_thread_stop = true;
static bool g_csi_enabled; // this flag maintains wifi_csi_config enable state during wifi re-connection

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
	if (err < 0) {
		CSIFW_LOGE("Fail to ioctl(%d, CSIIOC_GET_DATA), ret: %d, errno : %d", fd, err, get_errno());
	}
	return err;
}

static void* dataHandlerThread(void *vargp) {
	uint16_t read_len;
	while(!g_csi_thread_stop) {
		size_t ret = rb_used(g_raw_rb_p);
		if (ret > 0) {
			// read header first to get size
			ret = rb_read(g_raw_rb_p, &read_len, RB_HDR_LEN);
			if (ret != RB_HDR_LEN) {
				CSIFW_LOGE("rb read error %zu\n", ret);
				continue;
			}
			// read received size from RB
			ret = rb_read(g_raw_rb_p, g_send_data_buffptr, read_len);
			if (ret != read_len) {
				CSIFW_LOGE("rb read error %zu\n", ret);
				continue;
			}
			gCSIDataCallback(CSIFW_OK, read_len, g_send_data_buffptr, read_len - CSIFW_CSI_HEADER_LEN); 
		}
		pthread_yield();
	}
	CSIFW_LOGD("dataHandlerThread stopping");
}

static void* dataReceiverThread(void *vargp) {
	int len;
	int fd, ret;
	uint16_t write_len;
	mqd_t mq_handle;
	struct mq_attr attr_mq;
	attr_mq.mq_maxmsg = CSIFW_MQ_MSG_COUNT;
	attr_mq.mq_msgsize = sizeof(struct wifi_csi_msg_s);
	attr_mq.mq_flags = 0;
	CSIFW_LOGD("MQ_NAME: %s, MSG_SIZE: %zu", MQ_NAME, attr_mq.mq_msgsize);
	mq_handle = mq_open(MQ_NAME, O_RDWR | O_CREAT | O_NONBLOCK, 0666, &attr_mq);
	
	if (mq_handle == (mqd_t)ERROR) {
		CSIFW_LOGE("Failed to open mq %d \n", get_errno());
		return NULL;
	}
	OPEN_DRIVER_OR_EXIT(fd)
	//register mq
	ret = ioctl(fd, CSIIOC_REGISTERMQ, (unsigned long)mq_handle);
	if (ret < 0) {
		CSIFW_LOGE("Fail to ioctl(%d, CSIIOC_REGISTERMQ ), errno : %d", fd, get_errno());
		mq_close(mq_handle);
		CLOSE_DRIVER_OR_EXIT(fd)
		return NULL;
	}
	CSIFW_LOGD("MQ register done");

	//register callback
	ret = ioctl(fd, CSIIOC_REGISTER_CALLBACK , NULL);
	if (ret < 0) {
		CSIFW_LOGE("Fail to ioctl(%d, CSIIOC_REGISTER_CALLBACK), errno : %d", fd, get_errno());
		mq_close(mq_handle);
		CLOSE_DRIVER_OR_EXIT(fd)
		return NULL;
	}
	CSIFW_LOGI("CSI event handler register done");
	int prio;
	size_t size;
	struct wifi_csi_msg_s msg;

	while (!g_csi_thread_stop) {
		size = mq_receive(mq_handle, (char *)&msg, sizeof(msg), &prio);
		if (size != sizeof(msg)) {
			CSIFW_LOGD("Interrupted while waiting for deque message from kernel %zu, errono: %d", size, errno);
		} else {
			switch (msg.msgId)
			{
			case CSI_MSG_DATA_READY_CB:
				if (msg.data_len == 0 || msg.data_len > CSIFW_MAX_RAW_BUFF_LEN) {
					CSIFW_LOGE("Skipping packet: invalid data length: %d", msg.data_len);
					continue;
				}
				gCSIRawBufLen = msg.data_len;
				len = readCSIData(fd, g_get_data_buffptr + RB_HDR_LEN, gCSIRawBufLen); // first two bytes reserved for size
				if (len < 0) {
					CSIFW_LOGE("Skipping packet: error: %d", len);
					continue;
				}
				if (len != gCSIRawBufLen - CSIFW_CSI_HEADER_LEN) {
					CSIFW_LOGE("Invalid csi data packet, gCSIRawBufLen: %d, new size (w/o header): %d", gCSIRawBufLen, len);
					// continue; ==> Cant skip as the message queue call might not be in sync in case of error packets.
					gCSIRawBufLen = len + CSIFW_CSI_HEADER_LEN;
				}
				write_len = gCSIRawBufLen + RB_HDR_LEN;
				// Write data to ring buffer
				// store lenght in first 2 bytes
				memcpy(g_get_data_buffptr, &gCSIRawBufLen, RB_HDR_LEN);
				uint16_t temp;
				// remove packets till space for write is available 
				while(rb_avail(g_raw_rb_p) < write_len) {
					// get size of packet to be removed
					rb_read(g_raw_rb_p, &temp, RB_HDR_LEN);
					// increment read idx (ignore packet to make space)
					rb_read(g_raw_rb_p, NULL, temp);
				}
				ret = rb_write(g_raw_rb_p, g_get_data_buffptr, write_len); 
				if (ret != write_len) {
					CSIFW_LOGE("Rb_write fail. packet size: %d, write size: %zu", len);
				}
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

	// unregister callback
	ret = ioctl(fd, CSIIOC_UNREGISTER_CALLBACK, NULL);
	if (ret < 0) {
		CSIFW_LOGE("Fail to ioctl(%d, CSIIOC_UNREGISTER_CALLBACK), errno : %d", fd, get_errno());
		return CSIFW_ERROR;
	}
	CSIFW_LOGD("Callback unregistered");
	// unregister MQ
	ret = ioctl(fd, CSIIOC_UNREGISTERMQ, (unsigned long)mq_handle);
	if (ret < 0) {
		CSIFW_LOGE("Fail to ioctl(%d, CSIIOC_UNREGISTERMQ ), errno : %d", fd, get_errno());
	}
	CSIFW_LOGD("MQ unregistered");

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
			if (ret < 0) {
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
	if (ret < 0) {
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
	if (ret < 0) {
		CSIFW_LOGE("Fail to ioctl(%d, CSIIOC_SET_CONFIG ), errno : %d", fd, get_errno());
		return CSIFW_ERROR;
	}
	CLOSE_DRIVER_OR_EXIT(fd)
	CSIFW_LOGD("Got mac address from driver: [%02x:%02x:%02x:%02x:%02x:%02x]", mac_info->mac_addr[0], mac_info->mac_addr[1], mac_info->mac_addr[2], mac_info->mac_addr[3], mac_info->mac_addr[4], mac_info->mac_addr[5]);
	return CSIFW_OK;
}

CSIFW_RES csi_packet_receiver_start_collect(csi_action_param_t *config) {
	CSIFW_RES res;
		// allocate buffer for sending data to services
	if (!g_send_data_buffptr) {
		g_send_data_buffptr = (unsigned char *)malloc(gCSIRawBufLen);
		if (!g_send_data_buffptr) {
			CSIFW_LOGE("Send data buffer allocation fail.");
			return CSIFW_ERROR;
		}
	}
	// allocate buffer for receiveing data from driver
	if (!g_get_data_buffptr) {
		g_get_data_buffptr = (unsigned char *)malloc(gCSIRawBufLen);
		if (!g_get_data_buffptr) {
			CSIFW_LOGE("Buffer allocation fail.");
			return CSIFW_ERROR;
		}
		CSIFW_LOGD("Get data buffer allocation done, size: %d", gCSIRawBufLen);
	}
	// create ring buffer
	if (!g_raw_rb_p) {
		size_t rb_size = CSIFW_RB_SIZE;
		g_raw_rb_t.buf = NULL;
		g_raw_rb_t.depth=0;
		g_raw_rb_t.rd_idx=0;
		g_raw_rb_t.wr_idx=0;
		g_raw_rb_p = &g_raw_rb_t;
		if (!rb_init(g_raw_rb_p, rb_size)) {
			CSIFW_LOGE("Ring buffer init failed, size: %zu", rb_size);
			return CSIFW_ERROR;
		}
		CSIFW_LOGD("Ring buffer init success, size: %zu", rb_size);
	}

	g_csi_thread_stop = false;
	//create collector thread
	pthread_attr_t send_th_attr;
	pthread_attr_init(&send_th_attr);
	/* ToDo: stack size of thread needs to be optimized */
	pthread_attr_setstacksize(&send_th_attr, (10*1024));

	if (pthread_create(&gCSIDataHandlerWorker,  &send_th_attr, dataHandlerThread, NULL) != 0) {
		CSIFW_LOGE("Failed to create csi data collect thread");
		return CSIFW_ERROR;
	}
	if (pthread_setname_np(gCSIDataHandlerWorker, HNDLR_TH_NAME) != 0) {
		CSIFW_LOGE("Error in setting collector thread name, error_no: %d", get_errno());
	}
	CSIFW_LOGD("CSI data send thread created");
	
	if (pthread_create(&gCSIDataReceiver, NULL, dataReceiverThread, NULL) != 0) {
		CSIFW_LOGE("Failed to create csi data receive thread");
		return CSIFW_ERROR;
	}
	if (pthread_setname_np(gCSIDataReceiver, RCVR_TH_NAME) != 0) {
		CSIFW_LOGE("Error in setting receiver thread name, error_no: %d", get_errno());
	}
	CSIFW_LOGD("CSI data receive thread created");
	int fd;

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
	pthread_join(gCSIDataReceiver, NULL);
	pthread_join(gCSIDataHandlerWorker, NULL);
	if (g_get_data_buffptr) {
		free(g_get_data_buffptr);
		g_get_data_buffptr = NULL;
	}
	if (g_send_data_buffptr) {
		free(g_send_data_buffptr);
		g_send_data_buffptr = NULL;
	}
	if (g_raw_rb_p) {
		rb_free(g_raw_rb_p);
		g_raw_rb_p = NULL;
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
	if (ret < 0) {
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

