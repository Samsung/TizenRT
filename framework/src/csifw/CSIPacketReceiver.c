#include "csifw/CSIPacketReceiver.h"
#include <sys/ioctl.h>
#include <tinyara/wifi_csi/wifi_csi.h>
#include <tinyara/wifi_csi/wifi_csi_common.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>

#define DRIVER_PATH "/dev/wificsi"
extern COLLECT_STATE g_service_state;
extern CONNECTION_STATE g_nw_state;
static sem_t gSem;
static pthread_t gThread; 
static cb_count;
static unsigned char *g_raw_buffptr;
static unsigned int gCSIRawBufLen;
static CSIDataListener gCSIDataCallback;
static  get_empty_data_buffer g_get_buf_cb;
static bool g_csi_thread_stop;
#define PKT_RCVR_TH_NAME "csifw_receiver"

#define OPEN_DRIVER_OR_EXIT(_FD) \
	_FD = open(CONFIG_WIFI_CSI_DEVPATH, O_RDONLY);\
	if (_FD < 0) {\
		CSIFW_LOGE("Failed to open device path : %s errno : %d", CONFIG_WIFI_CSI_DEVPATH, get_errno());\
		return CSIFW_ERROR;\
	}\

#define CLOSE_DRIVER_OR_EXIT(_FD) close(_FD);

typedef enum CSI_SET_CONFIG_CMD {
	CSIFW_CONFIG_INIT,
	CSIFW_CONFIG_ENABLE,
	CSIFW_CONFIG_DISABLE
} CSI_SET_CONFIG_CMD;

CSIFW_RES csi_packet_receiver_set_csi_config(rtw_csi_action_parm_t *config, CSI_SET_CONFIG_CMD config_cmd);
typedef enum CSI_SETCONFIG_CMD {
    SET_CONFIG_INIT,
    SET_CONFIG_ENABLE,
    SET_CONFIG_DISABLE
} CSI_SETCONFIG_CMD;

static void get_callback_from_driver(char *buf, int buf_len, int flags, void *userdata) {
	cb_count++;
	if (g_service_state >= CSI_COLLECT_STATE_STOPPED) {
		CSIFW_LOGI("%d [CB hit] Collect state: CSI_COLLECT_STATE_PAUSED/STOPPED!! %d", cb_count, flags);
		return;
	}
	gCSIRawBufLen = flags;
	sem_post(&gSem);
}

static int readCSIData(int fd,char* buf, int size)
{
	int err=0;
	err=read(fd,buf, size);
	if (err < 0) {
		CSIFW_LOGE("READ ERROR %d", err);
	}
	return err;
}


static void* collectDataThread(void *vargp) {
	int len;
	int fd;
	OPEN_DRIVER_OR_EXIT(fd)
		CSIFW_LOGD("collectDataThread: Entering thread");
	while (!g_csi_thread_stop) {
		if (sem_wait(&gSem) == -1) {
			csi_packet_receiver_deinit();
			break;
		}
		if (g_csi_thread_stop) {
			CSIFW_LOGD("collectDataThread: Stopping thread");
			break;
		}
		if (g_get_buf_cb) {
			g_get_buf_cb(BTYPE_RAW, &g_raw_buffptr, gCSIRawBufLen);
		}
		
		len = readCSIData(fd,g_raw_buffptr, gCSIRawBufLen);
		if (len > 0){
			// Send data to CSI Service
			gCSIDataCallback(CSIFW_OK, gCSIRawBufLen, g_raw_buffptr, len);
		}
		else{
			gCSIDataCallback(CSIFW_ERROR, gCSIRawBufLen, g_raw_buffptr, gCSIRawBufLen);
		}
	}
	CLOSE_DRIVER_OR_EXIT(fd)
	CSIFW_LOGD("[collectDataThread] THREAD EXIT");
	return NULL;
}


static bool g_csi_enabled;
CSIFW_RES csi_packet_receiver_init(CSIDataListener CSIDataCallback, get_empty_data_buffer get_buf_cb) {
	g_csi_enabled = false;
	gCSIRawBufLen = MAX_CSI_BUFF_LEN;
	g_get_buf_cb = get_buf_cb;
	if (!g_get_buf_cb) {
		CSIFW_LOGI("ALLOCATING FW BUFFER");
		g_raw_buffptr = (unsigned char *)malloc(gCSIRawBufLen);
		if (!g_raw_buffptr) {
			CSIFW_LOGE("Buffer allocation fail.");
			return CSIFW_ERROR;
		}
		CSIFW_LOGI("Raw Buffer allocation done");
	}
	cb_count = 0; //debug
	gCSIDataCallback = CSIDataCallback;
	return CSIFW_OK;
}

CSIFW_RES csi_packet_receiver_set_csi_config(rtw_csi_action_parm_t *config, CSI_SET_CONFIG_CMD config_cmd) {

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
	wifi_csi_mac_info *mac_info_t = (wifi_csi_mac_info *)mac_info; 
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

/* CSI Service makes sure that this is only called when <INTERNET IS WIFI_CONNECTED> */
CSIFW_RES csi_packet_receiver_start_collect(rtw_csi_action_parm_t *config) {
	CSIFW_RES res;
	if (sem_init(&gSem, 0, 0) == -1) {
		CSIFW_LOGE("Semaphore init fail.");
		return CSIFW_ERROR;
	}
	CSIFW_LOGI("Semaphore init done");
	//create thread
	g_csi_thread_stop = false;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setstacksize(&attr, (10*1024));
		
	if (pthread_create(&gThread, &attr, collectDataThread, NULL) != 0) {
		CSIFW_LOGE("Failed to create csi data collect thread");
		return CSIFW_ERROR;
	}
	if (pthread_setname_np(gThread, PKT_RCVR_TH_NAME) != 0) {
		CSIFW_LOGE("Error in setting receiver thread name, error_no: %d", get_errno());
	}
	CSIFW_LOGD("CSI data collect thread created");
	//register callback
	int fd;
	OPEN_DRIVER_OR_EXIT(fd)
	int ret = ioctl(fd, CSIIOC_REGISTER_CALLBACK , (unsigned long)get_callback_from_driver);
	if (ret < 0) {
		CSIFW_LOGE("Fail to ioctl(%d, CSIIOC_REGISTER_CALLBACK), errno : %d", fd, get_errno());
		return CSIFW_ERROR;
	}
	CLOSE_DRIVER_OR_EXIT(fd)
	CSIFW_LOGI("CSI event handler register done");


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

CSIFW_RES csi_packet_receiver_stop_collect(CSIFW_REASON reason, rtw_csi_action_parm_t *config)
{
	//unregister callback
	int fd;
	CSIFW_LOGI("Unregistering callback");
	OPEN_DRIVER_OR_EXIT(fd)
	int ret = ioctl(fd, CSIIOC_UNREGISTER_CALLBACK, (unsigned long)get_callback_from_driver);
	if (ret < 0) {
		CSIFW_LOGE("Fail to ioctl(%d, CSIIOC_UNREGISTER_CALLBACK), errno : %d", fd, get_errno());
		return CSIFW_ERROR;
	}
	CLOSE_DRIVER_OR_EXIT(fd)
	CSIFW_LOGI("Callback unregistered");
	
	//join thread
	g_csi_thread_stop = true;
	sem_post(&gSem);
	pthread_join(gThread, NULL);
	sem_destroy(&gSem);	
	if (reason == CSIFW_WIFI_DISCONNECTED) {
		CSIFW_LOGI("Disable not required as wifi disconnected");
		CSIFW_LOGI("csi data collect stopped");
		return CSIFW_OK;
	}
	
	// else-if WIFI_CONNECTED disable wifi csi report
	OPEN_DRIVER_OR_EXIT(fd)
	config->act = 0;
	config->enable = 0;
	CSIFW_LOGI("Disabling CSI config");
	ret = ioctl(fd, CSIIOC_SET_CONFIG, (unsigned long)config);
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
	if (g_raw_buffptr) {
		free(g_raw_buffptr);
		g_raw_buffptr = NULL;
	}
	CSIFW_LOGD("csi_packet_receiver_deinit done");
	return CSIFW_OK;
}
