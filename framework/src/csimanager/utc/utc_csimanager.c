/*
 * utc_new.c - Unit test cases for CSIManager and CSIFrameworkMain
 *
 * Created on: 2025.08.08
 *     Author: Rajat Srivastava
 */

#include <netdb.h>
#include "csifw.h"
#include <check.h>
#include <netdb.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include "utc_mocks.h"
#include "utc_config.h"
#include <sys/socket.h>
#include <system/system_log.h>
#include "csimanager/csifw_api.h"
#include <tinyara/wifi_csi/wifi_csi.h>
#include <wifi_manager/wifi_manager.h>
#include "task_manager/task_manager.h"

// Mock control variables
static int g_mock_register_result = 0;
static int g_mock_start_result = 0;
static int g_mock_unregister_result = 0;
static int g_mock_init_result = 0;
static int g_mock_destroy_result = 0;

static int Wifi_Manager_Register_Callback_API_Fail = 0;
static int Wifi_Manager_Get_Info_API_Fail = 0;
static int Addrinfo_Return_Fail = 0;
static int sa_family_return_fail = 0;
static int SETSOCKOPT_API_FAIL = 0;
static int sendto_Return_Fail = 0;
static int CSIIOC_GET_MAC_ADDR_API_FAIL = 0;
static int CSIIOC_SET_CONFIG_API_FAIL = 0;
static int CSIIOC_START_CSI_API_FAIL = 0;
static int CSIIOC_STOP_CSI_API_FAIL = 0;
static int CSIIOC_GET_DATA_API_FAIL = 0;

static int TASK_MANAGER_REG_API_FAIL = 0;
static int MQ_OPEN_API_FAIL = 0;

// Thread management variables
static pthread_t g_task_threads;
static int g_thread_count = 0;
extern csifw_task_Main(int argc, char *argv[]);

// Function pointer types for mocking
typedef csifw_context_t* (*get_csifw_context_fn)();
typedef int (*csi_packet_receiver_initialize_fn)();
typedef int (*csi_ping_generator_initialize_fn)();

// Declare original functions and variables
extern csifw_context_t* get_csifw_context();
extern int csi_packet_receiver_initialize();
extern int csi_ping_generator_initialize();

// Wrapper functions for mocking
static get_csifw_context_fn g_get_csifw_context;
static csi_packet_receiver_initialize_fn g_csi_packet_receiver_initialize;
static csi_ping_generator_initialize_fn g_csi_ping_generator_initialize;

// Initialize function pointers
static void init_mock_functions() {
    g_get_csifw_context = get_csifw_context;
    g_csi_packet_receiver_initialize = csi_packet_receiver_initialize;
    g_csi_ping_generator_initialize = csi_ping_generator_initialize;
}

void* entry(void *ptr)
{
    csifw_task_Main(0, NULL);
    return NULL;
}

// Enhanced task manager wraps with pthread support
int __wrap_task_manager_register_task(char *name, int priority, int stack_size, 
                                    main_t entry, char * argv[], int permission, int timeout) 
{
    if (TASK_MANAGER_REG_API_FAIL == 1) {
        return -1;
    }                                    
    return 15; // Return thread ID
}

int __wrap_task_manager_start(int handle, int timeout) 
{
    pthread_attr_t recv_th_attr;
    pthread_attr_init(&recv_th_attr);
    pthread_attr_setstacksize(&recv_th_attr, (4 * 1024));
    if (pthread_create(&g_task_threads, &recv_th_attr, entry, NULL) != 0) {
        pthread_attr_destroy(&recv_th_attr);
        return -1;
    }
    return g_mock_start_result;
}

int __wrap_task_manager_unregister(int handle, int timeout) 
{
    if (g_task_threads != 0) {
        if (pthread_join(g_task_threads, NULL) != 0) {
            return -1;
        }
        g_task_threads = 0;
    }
    return g_mock_unregister_result;
}

/* CSINetworkMonitor.c ******************************************************/

wifi_manager_result_e __wrap_wifi_manager_get_info(wifi_manager_info_s *info)
{
	if (Wifi_Manager_Get_Info_API_Fail == 1) {
		info->status = AP_DISCONNECTED;
		return WIFI_MANAGER_FAIL;
	} else if (Wifi_Manager_Get_Info_API_Fail == 2) {
		info->status = AP_DISCONNECTED;
	} else if (Wifi_Manager_Get_Info_API_Fail == 3) {
		info->status = STATUS_UNKNOWN;
	} else {
		info->status = AP_CONNECTED;
	}
	return WIFI_MANAGER_SUCCESS;

}

wifi_manager_cb_s wifi_manager_cb_object;

wifi_manager_result_e __wrap_wifi_manager_register_cb(wifi_manager_cb_s *wmcb)
{
	wifi_manager_cb_object.sta_connected = wmcb->sta_connected;
	wifi_manager_cb_object.sta_disconnected = wmcb->sta_disconnected;

	if (Wifi_Manager_Register_Callback_API_Fail == 1) {
		return WIFI_MANAGER_FAIL;
	} else {
		return WIFI_MANAGER_SUCCESS;
	}
}

wifi_manager_result_e __wrap_wifi_manager_unregister_cb(wifi_manager_cb_s *wmcb)
{
	if (Wifi_Manager_Register_Callback_API_Fail == 1) {
		return WIFI_MANAGER_FAIL;
	} else {
		return WIFI_MANAGER_SUCCESS;
	}
}

/* PingGenerator.c ******************************************************/

int __wrap_getaddrinfo(FAR const char *hostname, FAR const char *servname, FAR const struct addrinfo *hint, FAR struct addrinfo **res)
{
	// if (Addrinfo_Return_Fail == 0) {
		static struct sockaddr obj_sockaddr;
		// if (sa_family_return_fail == 0) {
			obj_sockaddr.sa_family = AF_INET;
		// }
		// else {
			// obj_sockaddr.sa_family != AF_INET;
		// }

		static struct addrinfo obj_addrinfo;
		obj_addrinfo.ai_family = AF_INET;
		obj_addrinfo.ai_socktype = SOCK_RAW;
		obj_addrinfo.ai_protocol = IPPROTO_ICMP;
		obj_addrinfo.ai_addr = &obj_sockaddr;

		*res = &obj_addrinfo;
		(*res)->ai_addr = &obj_sockaddr;
		return 0;
	// } else {
	// 	return -1;
	// }
}

int __wrap_socket(int domain, int type, int protocol)
{
	return 0;
}

int __wrap_netlib_get_ipv4_gateway_addr(FAR const char *ifname, FAR struct in_addr *addr)
{
	addr->s_addr = "192.168.0.1";
	return 0;
}

int __wrap_setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen)
{
	if (SETSOCKOPT_API_FAIL == 1) {
		return -1;
	} else {
		return 0;
	}
}

ssize_t __wrap_sendto(int sockfd, FAR const void *buf, size_t len, int flags, FAR const struct sockaddr *to, socklen_t tolen)
{
	// if (sendto_Return_Fail == 1) {
	// 	return -1;
	// } else {
		return 1;
	// }
}

void __wrap_freeaddrinfo(FAR struct addrinfo *ai)
{
	return;
}


/* CSIPacketReciever.c ******************************************************/

struct wrap_mq_des {
    struct mq_des *flink;    /* Forward link to next message descriptor */
    struct mqueue_inode_s *msgq;    /* Pointer to associated message queue */
    int oflags;    /* Flags set when message queue was opened */
}wrap_mq_des;

typedef struct wrap_mq_des *wrap_mqd_t; // Renamed from mqd_t to wrap_mqd_t

wrap_mqd_t __wrap_mq_open(const char *mq_name, int oflags, ...)
{
    if (MQ_OPEN_API_FAIL == 1) {
        return (mqd_t) ERROR;
    }
	wrap_mqd_t dummy_handle = (wrap_mqd_t)malloc(sizeof(wrap_mq_des));
	printf("[malloc %zu bytes]\n",sizeof(wrap_mqd_t));
	dummy_handle->flink = NULL;
	dummy_handle->msgq = NULL;
	dummy_handle->oflags = 1;
	
	return (wrap_mqd_t)dummy_handle;
}

int __wrap_mq_close(wrap_mqd_t mqdes)
{
	free(mqdes) ;
	return 0;
}

ssize_t __wrap_mq_receive(mqd_t mqdes, FAR char *msg, size_t msglen, FAR int *prio)
{
    struct wifi_csi_msg_s *msg_ptr = (struct wifi_csi_msg_s *)msg;
    msg_ptr->msgId = CSI_MSG_DATA_READY_CB;
    msg_ptr->data_len = 155;
    sleep(1);
    return sizeof(struct wifi_csi_msg_s);
}

//return real mocked data which should be passed in ioctl->req=6
unsigned char raw_temp_data[] = {
								0xcd, 0xab, 0x28, 0x9c, 0x44, 0x3d, 0x58, 0x80, 0x3a, 0x38, 0xd5, 0x47, 0xe3,
								0x6f, 0x48, 0xb4, 0xe5, 0x31, 0x06, 0x09, 0x00, 0x87, 0x01, 0x01, 0x00, 0x38,
								0x10, 0x00, 0x83, 0x00, 0xaf, 0x00, 0x00, 0xb0, 0x01, 0x00, 0x00, 0x70, 0x00,
								0x00, 0x01, 0x00, 0x08, 0xfc, 0x09, 0xf8, 0x07, 0xf4, 0x04, 0xf3, 0x00, 0xf2,
								0xfb, 0xf3, 0xf7, 0xf6, 0xf2, 0xfa, 0xef, 0xff, 0xee, 0x05, 0xee, 0x09, 0xef,
								0xf2, 0x11, 0xf6, 0x13, 0xfb, 0x14, 0x00, 0x14, 0x06, 0x12, 0x0b, 0x0f, 0x0f,
								0x0b, 0x12, 0x06, 0x14, 0x01, 0x15, 0xfc, 0x15, 0xf8, 0x13, 0xf3, 0x11, 0xf0,
								0xed, 0x09, 0xeb, 0x05, 0xec, 0xfb, 0xee, 0xf7, 0xf0, 0xf3, 0xf4, 0xf1, 0xf8,
								0xee, 0xfc, 0xee, 0x01, 0xee, 0x04, 0xef, 0x08, 0xf2, 0x0a, 0xf5, 0x0b, 0xf8,
								0xfb, 0x0c, 0xfe, 0x0b, 0x01, 0x09, 0x03, 0x08, 0x05, 0x05, 0x07, 0x03, 0x07,
								0x00, 0x07, 0xfe, 0x06, 0xfc, 0x05, 0xfb, 0x03, 0xfb, 0x01, 0xfb, 0xff, 0xfc,
								0xfd, 0xfd, 0xff, 0xfd, 0x00, 0xfd, 0xfe, 0x0d, 0x0e, 0x00, 0x00, 0x0c
								};

int __wrap_ioctl(int fd, int req, unsigned long arg)
{
	static csi_config_type_t g_config_type;
	static unsigned int g_interval_ms;

	csi_driver_buffer_args_t *buf_args = NULL;
	csi_driver_buffer_args_t buff_arg; 
	
	int ret = 0;
	if ( req == CSIIOC_GET_DATA){ // CSIIOC_GET_DATA
		// if (CSIIOC_GET_DATA_API_FAIL == 1){
		// 	ret = -1;
		// }
		// else {
			csi_driver_buffer_args_t *buf_args = (csi_driver_buffer_args_t *)arg;
			buf_args = (csi_driver_buffer_args_t *)arg;
			
			memmove(buf_args->buffer, raw_temp_data, 155);//to be checked
			ret = 155;
			
			memset(buf_args->buffer, 0, CSIFW_MAX_RAW_BUFF_LEN);
			// }
	}
	if (req == CSIIOC_SET_CONFIG) {
		if (CSIIOC_SET_CONFIG_API_FAIL == 1) {
			csi_config_action_t config_action;
			csi_config_args_t config_args; // CSIIOC_SET_CONFIG
			config_args.config_action = config_action;
			config_args.config_type = g_config_type;
			config_args.interval = g_interval_ms;
			ret = -1;
		}
		else {
			ret = 1;
		}
	}
    if (req == CSIIOC_GET_MAC_ADDR) {
        // if (CSIIOC_GET_MAC_ADDR_API_FAIL == 1) {
        //     ret = -1;
        // } else {
            // Return success immediately with mock MAC data if needed
            ret = 0;
        // }
    }
	if (req == CSIIOC_STOP_CSI) {
		if (CSIIOC_STOP_CSI_API_FAIL == 1) {
			ret = -1;
		}
		ret = 0; 
	}
	if (req == CSIIOC_START_CSI) {
		// if (CSIIOC_START_CSI_API_FAIL == 1 && req == CSIIOC_START_CSI) {
			// ret = -1;
		// } 
		ret = 0; 
	}

	return ret;
}

static int fd;
static unsigned int count;
static csifw_service_handle g_service_handle;

static void raw_data_listener(CSIFW_RES res, int csi_buff_len, unsigned char *csi_raw_buff, void *ptr)
{
    count++;

    if(csi_raw_buff != NULL) {
		logReceivedData(csi_buff_len, csi_raw_buff, 0);
	}

    if (res, CSIFW_OK) {
        return;
    }
    if (res, CSIFW_ERROR_WIFI_DIS_CONNECTED) {
        return;
    }
    if (res, CSIFW_ERROR) {
        return;
    }
    if (res, CSIFW_OK_WIFI_CONNECTED) {
    }
}

static void parsed_data_listener(CSIFW_RES res, int csi_data_len, float *csi_parsed_buff, void *ptr)
{
}

static void csi_service_setup(csi_config_type_t config_type)
{
    count = 0;
    service_callbacks_t callbacks = {
        .raw_data_cb = raw_data_listener,
        .parsed_data_cb = parsed_data_listener,
        .user_data = NULL
    };
    
    csifw_init();
    ck_assert_int_eq(csifw_registerService(&g_service_handle, &callbacks, config_type, 2500), CSIFW_OK);
    fd = open("/dev/wificsi", O_CREAT);
}

static void csi_service_teardown()
{
    close(fd);
    csifw_unregisterService(g_service_handle);
}

/****************************************************************************
 * TEST CASES
 ****************************************************************************/

START_TEST(test_csifw_registerService_valid1)
{
    csifw_service_handle handle;
    service_callbacks_t callbacks = {
        .raw_data_cb = raw_data_listener,
        .parsed_data_cb = NULL,
        .user_data = NULL
    };
    
    csifw_init();
    csi_config_type_t config_type = HT_CSI_DATA;
    ck_assert_int_eq(csifw_registerService(&handle, &callbacks, config_type, 250), CSIFW_OK);
    ck_assert_int_eq(csifw_unregisterService(handle), CSIFW_OK);
}
END_TEST

START_TEST(test_csifw_registerService_valid2)
{
    csifw_service_handle handle;
    service_callbacks_t callbacks = {
        .raw_data_cb = raw_data_listener,
        .parsed_data_cb = NULL,
        .user_data = NULL
    };
    
    csifw_init();
    csi_config_type_t config_type = HT_CSI_DATA_ACC1;
    ck_assert_int_eq(csifw_registerService(&handle, &callbacks, config_type, 250), CSIFW_OK);
    ck_assert_int_eq(csifw_unregisterService(handle), CSIFW_OK);
}
END_TEST

START_TEST(test_csifw_registerService_valid3)
{
    csifw_service_handle handle;
    service_callbacks_t callbacks = {
        .raw_data_cb = raw_data_listener,
        .parsed_data_cb = NULL,
        .user_data = NULL
    };
    
    csifw_init();
    csi_config_type_t config_type = NON_HT_CSI_DATA;
    ck_assert_int_eq(csifw_registerService(&handle, &callbacks, config_type, 250), CSIFW_OK);
    ck_assert_int_eq(csifw_unregisterService(handle), CSIFW_OK);
}
END_TEST

START_TEST(test_csifw_registerService_valid4)
{
    csifw_service_handle handle;
    service_callbacks_t callbacks = {
        .raw_data_cb = raw_data_listener,
        .parsed_data_cb = NULL,
        .user_data = NULL
    };
    
    csifw_init();
    csi_config_type_t config_type = NON_HT_CSI_DATA_ACC1;
    ck_assert_int_eq(csifw_registerService(&handle, &callbacks, config_type, 250), CSIFW_OK);
    ck_assert_int_eq(csifw_unregisterService(handle), CSIFW_OK);
}
END_TEST

START_TEST(test_csifw_register_UnregisterService_invalid)
{
    // Test NULL handle
    ck_assert_int_eq(csifw_unregisterService(NULL), CSIFW_INVALID_ARG);
    
    // Test invalid handle value
    csifw_service_handle invalid_handle = (csifw_service_handle)0xDEADBEEF;

    ck_assert_int_eq(csifw_unregisterService(invalid_handle), CSIFW_ERROR_NOT_INITIALIZED);
}
END_TEST

START_TEST(test_csifw_registerService_invalid_interval)
{
    csifw_service_handle handle;
    service_callbacks_t callbacks = {
        .raw_data_cb = raw_data_listener,
        .parsed_data_cb = parsed_data_listener,
        .user_data = NULL
    };
    
    csifw_init();
    
    // Test Valid config and Invalid interval
    ck_assert_int_eq(csifw_registerService(&handle, &callbacks, HT_CSI_DATA, 20), CSIFW_INVALID_ARG);
}
END_TEST

START_TEST(test_csifw_registerService_invalid_config1)
{
    csifw_service_handle handle;
    service_callbacks_t callbacks = {
        .raw_data_cb = raw_data_listener,
        .parsed_data_cb = parsed_data_listener,
        .user_data = NULL
    };
    
    csifw_init();
    
    // Test MIN_CSI_CONFIG_TYPE config
    ck_assert_int_eq(csifw_registerService(&handle, &callbacks, MIN_CSI_CONFIG_TYPE, 250), CSIFW_INVALID_ARG);
}
END_TEST

START_TEST(test_csifw_registerService_invalid_config2)
{
    csifw_service_handle handle;
    service_callbacks_t callbacks = {
        .raw_data_cb = raw_data_listener,
        .parsed_data_cb = parsed_data_listener,
        .user_data = NULL
    };
    
    csifw_init();
    
    // Test MAX_CSI_CONFIG_TYPE config
    ck_assert_int_eq(csifw_registerService(&handle, &callbacks, MAX_CSI_CONFIG_TYPE, 250), CSIFW_INVALID_ARG);
}
END_TEST

START_TEST(test_csifw_registerService_fail)
{
    csi_config_type_t config_type = HT_CSI_DATA;
    csifw_service_handle handle;
    service_callbacks_t callbacks = {
        .raw_data_cb = raw_data_listener,
        .parsed_data_cb = NULL,
        .user_data = NULL
    };
    
    ck_assert_int_eq(csifw_registerService(&handle, &callbacks, config_type, 250), CSIFW_ERROR_NOT_INITIALIZED);
    
    csifw_init();
    ck_assert_int_eq(csifw_registerService(NULL, &callbacks, config_type, 250), CSIFW_INVALID_ARG);
    
    Wifi_Manager_Register_Callback_API_Fail = 1;
    ck_assert_int_eq(csifw_registerService(&handle, &callbacks, config_type, 250), CSIFW_ERROR);
    Wifi_Manager_Register_Callback_API_Fail = 0;
    
    ck_assert_int_eq(csifw_registerService(&handle, &callbacks, config_type, 250), CSIFW_OK);
    ck_assert_int_eq(csifw_registerService(&handle, &callbacks, NON_HT_CSI_DATA, 250), CSIFW_ERROR_ALREADY_INIT_WITH_DIFFERENT_CONFIG);
    ck_assert_int_eq(csifw_unregisterService(handle), CSIFW_OK);
}
END_TEST

START_TEST(test_csifw_unregister_fail)
{
    csi_config_type_t config_type = HT_CSI_DATA;
    
    csi_service_setup(config_type);
    
    ck_assert_int_eq(csifw_unregisterService(NULL), CSIFW_INVALID_ARG);
    
    ck_assert_int_eq(csifw_start(g_service_handle), CSIFW_OK);
    ck_assert_int_eq(csifw_unregisterService(g_service_handle), CSIFW_ERROR_INVALID_SERVICE_STATE);
    
    ck_assert_int_eq(csifw_stop(g_service_handle), CSIFW_OK);
    Wifi_Manager_Register_Callback_API_Fail = 1;
    ck_assert_int_eq(csifw_unregisterService(g_service_handle), CSIFW_ERROR);
    Wifi_Manager_Register_Callback_API_Fail = 0;
}
END_TEST

START_TEST(test_csifw_start_stop_flow)
{
    csi_config_type_t config_type = HT_CSI_DATA;
    
    csi_service_setup(config_type);
    ck_assert_int_eq(csifw_start(g_service_handle), CSIFW_OK);
    ck_assert_int_eq(csifw_stop(g_service_handle), CSIFW_OK);
    csi_service_teardown();
}
END_TEST

START_TEST(test_csifw_start_stop_wm_cb_sta_connected)
{
    csi_config_type_t config_type = HT_CSI_DATA;
    
    csi_service_setup(config_type);
    ck_assert_int_eq(csifw_start(g_service_handle), CSIFW_OK);
    sleep(1);
    wifi_manager_cb_msg_s msgobj;
	msgobj.res = 0;
	msgobj.reason = 0;
	wifi_manager_cb_object.sta_disconnected(msgobj,NULL);
    
    ck_assert_int_eq(csifw_stop(g_service_handle), CSIFW_OK);
    csi_service_teardown();
}
END_TEST

START_TEST(test_csifw_start_stop_wm_cb_sta_disconnected)
{
    csi_config_type_t config_type = HT_CSI_DATA;
    
    csi_service_setup(config_type);
    ck_assert_int_eq(csifw_start(g_service_handle), CSIFW_OK);
    sleep(1);
    wifi_manager_cb_msg_s msgobj;
	msgobj.res = 0;
	msgobj.reason = 0;

	msgobj.bssid[0] = 01;
	msgobj.bssid[1] = 02;
	msgobj.bssid[2] = 03;
	msgobj.bssid[3] = 04;
	msgobj.bssid[4] = 05;
	msgobj.bssid[5] = 06;
	wifi_manager_cb_object.sta_disconnected(msgobj,NULL);
    sleep(1);
    wifi_manager_cb_object.sta_connected(msgobj,NULL);

    ck_assert_int_eq(csifw_stop(g_service_handle), CSIFW_OK);
    csi_service_teardown();
}
END_TEST

START_TEST(test_csifw_start_fail_1)
{
    csi_service_setup(HT_CSI_DATA);
    csifw_context_t *ctx = get_csifw_context();
    
    ck_assert_int_eq(csifw_start(NULL), CSIFW_INVALID_ARG);
    
    ctx->csifw_state = CSI_FRAMEWORK_STATE_STARTED_WAITING_FOR_NW;
    ck_assert_int_eq(csifw_start(g_service_handle), CSIFW_OK_WIFI_NOT_CONNECTED);
    ctx->csifw_state = CSI_FRAMEWORK_STATE_INITIALIZED;
    
    ck_assert_int_eq(csifw_stop(g_service_handle), CSIFW_OK_ALREADY_STOPPED);
    csi_service_teardown();
    }
END_TEST

START_TEST(test_csifw_start_fail_2)
{
    csi_service_setup(HT_CSI_DATA);
    csifw_context_t *ctx = get_csifw_context();

    ctx->csi_services->svc_state = CSI_SERVICE_START;
    ck_assert_int_eq(csifw_start(g_service_handle), CSIFW_OK_ALREADY_STARTED);
    ctx->csi_services->svc_state = CSI_SERVICE_REGISTERED;
    
    ck_assert_int_eq(csifw_stop(g_service_handle), CSIFW_OK_ALREADY_STOPPED);
    csi_service_teardown();
}
END_TEST

START_TEST(test_csifw_start_fail_3)
{
    csi_service_setup(HT_CSI_DATA);
    csifw_context_t *ctx = get_csifw_context();

    Wifi_Manager_Get_Info_API_Fail = 1;
    ck_assert_int_eq(csifw_start(g_service_handle), CSIFW_OK);
    Wifi_Manager_Get_Info_API_Fail = 0;

    ck_assert_int_eq(csifw_stop(g_service_handle), CSIFW_OK);
    csi_service_teardown();
}
END_TEST

START_TEST(test_csifw_stop_fail)
{
    csi_config_type_t config_type = HT_CSI_DATA;
    
    csi_service_setup(config_type);
    csifw_context_t *ctx = get_csifw_context();
    
    ck_assert_int_eq(csifw_stop(NULL), CSIFW_INVALID_ARG);
    
    ctx->csifw_state = CSI_FRAMEWORK_STATE_STOPPED;
    ck_assert_int_eq(csifw_stop(g_service_handle), CSIFW_OK_ALREADY_STOPPED);
    ctx->csifw_state = CSI_FRAMEWORK_STATE_STARTED;
    
    csi_service_teardown();
}
END_TEST

START_TEST(test_csifw_set_get_interval1)
{
    csi_config_type_t config_type = HT_CSI_DATA;
    unsigned int interval = 0;
    
    csi_service_setup(config_type);
    ck_assert_int_eq(csifw_set_interval(g_service_handle, 500), CSIFW_OK);
    ck_assert_int_eq(csifw_get_interval(g_service_handle, &interval), CSIFW_OK);
    csi_service_teardown();
}
END_TEST

START_TEST(test_csifw_set_get_interval2)
{
    csi_config_type_t config_type = NON_HT_CSI_DATA;
    unsigned int interval = 0;
    
    csi_service_setup(config_type);
    ck_assert_int_eq(csifw_set_interval(g_service_handle, 500), CSIFW_OK);
    ck_assert_int_eq(csifw_get_interval(g_service_handle, &interval), CSIFW_OK);
    csi_service_teardown();
}
END_TEST

START_TEST(test_csifw_set_interval_fail)
{
    csi_config_type_t config_type = HT_CSI_DATA;
    unsigned int interval = 0;
    
    csi_service_setup(config_type);

    ck_assert_int_eq(csifw_set_interval(NULL, &interval), CSIFW_INVALID_ARG);
    
    ck_assert_int_eq(csifw_set_interval(g_service_handle, 20), CSIFW_INVALID_ARG);

    ck_assert_int_eq(csifw_set_interval(g_service_handle, 2500), CSIFW_OK);

    csi_service_teardown();
}
END_TEST

START_TEST(test_csifw_get_interval_fail)
{
    csi_config_type_t config_type = HT_CSI_DATA;
    unsigned int interval = 0;
    
    csi_service_setup(config_type);

    ck_assert_int_eq(csifw_get_interval(NULL, &interval), CSIFW_INVALID_ARG);
    
    ck_assert_int_eq(csifw_get_interval(g_service_handle, NULL), CSIFW_INVALID_ARG);

    csi_service_teardown();
}
END_TEST

START_TEST(test_csifw_get_config)
{
    csi_config_type_t config_type = HT_CSI_DATA;
    csi_config_type_t retrieved_config;
    
    csi_service_setup(config_type);
    ck_assert_int_eq(csifw_get_config(g_service_handle, &retrieved_config), CSIFW_OK);
    csi_service_teardown();
}
END_TEST

START_TEST(test_csifw_get_config_fail)
{
    csi_config_type_t config_type = HT_CSI_DATA;
    csi_service_setup(config_type);
    ck_assert_int_eq(csifw_get_config(NULL, &config_type), CSIFW_INVALID_ARG);
    ck_assert_int_eq(csifw_get_config(g_service_handle, NULL), CSIFW_INVALID_ARG);
    csi_service_teardown();
}
END_TEST

START_TEST(test_csifw_get_ap_mac_addr)
{
    csi_config_type_t config_type = HT_CSI_DATA;
    csifw_mac_info mac_info;
    
    csi_service_setup(config_type);
    ck_assert_int_eq(csifw_start(g_service_handle), CSIFW_OK);
    ck_assert_int_eq(csifw_get_ap_mac_addr(g_service_handle, &mac_info), CSIFW_OK);
    ck_assert_int_eq(csifw_stop(g_service_handle), CSIFW_OK);
    csi_service_teardown();
}
END_TEST

START_TEST(test_csifw_get_ap_mac_addr_fail)
{
    csifw_mac_info mac_info;
    ck_assert_int_eq(csifw_get_ap_mac_addr(NULL, &mac_info), CSIFW_INVALID_ARG);
    
    csi_service_setup(HT_CSI_DATA);
    ck_assert_int_eq(csifw_get_ap_mac_addr(g_service_handle, NULL), CSIFW_INVALID_ARG);
    ck_assert_int_eq(csifw_get_ap_mac_addr(g_service_handle, &mac_info), CSIFW_ERROR_INVALID_SERVICE_STATE);
    csi_service_teardown();
}
END_TEST

START_TEST(test_csi_framework_null_context)
{
    // Test with NULL context - should safely return error
    ck_assert_int_eq(start_csi_framework(NULL), -1);
    ck_assert_int_eq(stop_csi_framework(NULL), 0);
}
END_TEST

START_TEST(test_start_csi_framework_already_running)
{
    csifw_context_t ctx;
    ctx.task_run_state = 1;
    ck_assert_int_ge(start_csi_framework(&ctx), 0);
}
END_TEST

START_TEST(test_stop_csi_framework_already_stopped)
{
    csifw_context_t ctx;
    ctx.csifw_state = CSI_FRAMEWORK_STATE_STOPPED;
    ctx.task_run_state = 0;
    ck_assert_int_eq(stop_csi_framework(&ctx), 0);
}
END_TEST

START_TEST(test_csi_framework_taskMngrReg_Fail)
{
    csi_config_type_t config_type = HT_CSI_DATA;
    csi_service_setup(config_type);
    TASK_MANAGER_REG_API_FAIL = 1;
    ck_assert_int_eq(csifw_start(g_service_handle), CSIFW_ERROR);
    TASK_MANAGER_REG_API_FAIL = 0;
    csi_service_teardown();
}
END_TEST

START_TEST(test_csi_framework_PcktRecvInit_Fail)
{
    csi_config_type_t config_type = HT_CSI_DATA;
    MQ_OPEN_API_FAIL = 1;
    csi_service_setup(config_type);
    ck_assert_int_eq(csifw_start(g_service_handle), CSIFW_ERROR);
    csi_service_teardown();
    MQ_OPEN_API_FAIL = 0;
}
END_TEST

START_TEST(test_csi_framework_PcktRecvStart_Fail)
{
    csi_config_type_t config_type = HT_CSI_DATA;
    CSIIOC_SET_CONFIG_API_FAIL = 1;
    csi_service_setup(config_type);
    ck_assert_int_eq(csifw_start(g_service_handle), CSIFW_ERROR);
    csi_service_teardown();
    CSIIOC_SET_CONFIG_API_FAIL = 0;
}
END_TEST

START_TEST(test_csi_framework_PcktRecvStop_Fail)
{
    csi_config_type_t config_type = HT_CSI_DATA;
    csi_service_setup(config_type);
    ck_assert_int_eq(csifw_start(g_service_handle), CSIFW_OK);
    CSIIOC_SET_CONFIG_API_FAIL = 1;
    ck_assert_int_eq(csifw_stop(g_service_handle), CSIFW_OK);
    CSIIOC_SET_CONFIG_API_FAIL = 0;
    csi_service_teardown();
}
END_TEST

START_TEST(test_csi_framework_PingInit_Fail)
{
    csi_config_type_t config_type = HT_CSI_DATA;
    SETSOCKOPT_API_FAIL = 1;
    csi_service_setup(config_type);
    ck_assert_int_eq(csifw_start(g_service_handle), CSIFW_ERROR);
    csi_service_teardown();
    SETSOCKOPT_API_FAIL = 0;
}
END_TEST

void utc_csimanager(TCase *tc)
{
    tcase_add_test(tc, test_csifw_registerService_valid1);
    tcase_add_test(tc, test_csifw_registerService_valid2);
    tcase_add_test(tc, test_csifw_registerService_valid3);
    tcase_add_test(tc, test_csifw_registerService_valid4);
    tcase_add_test(tc, test_csifw_register_UnregisterService_invalid);
    tcase_add_test(tc, test_csifw_registerService_invalid_interval);
    tcase_add_test(tc, test_csifw_registerService_invalid_config1);
    tcase_add_test(tc, test_csifw_registerService_invalid_config2);
    tcase_add_test(tc, test_csifw_registerService_fail);
    tcase_add_test(tc, test_csifw_unregister_fail);

    tcase_add_test(tc, test_csifw_start_stop_flow);
    tcase_add_test(tc, test_csifw_start_stop_wm_cb_sta_connected);
    tcase_add_test(tc, test_csifw_start_stop_wm_cb_sta_disconnected);
    tcase_add_test(tc, test_csifw_start_fail_1);
    tcase_add_test(tc, test_csifw_start_fail_2);
    tcase_add_test(tc, test_csifw_start_fail_3);
    tcase_add_test(tc, test_csifw_stop_fail);
    
    tcase_add_test(tc, test_csifw_set_get_interval1);
    tcase_add_test(tc, test_csifw_set_get_interval2);
    tcase_add_test(tc, test_csifw_set_interval_fail);
    tcase_add_test(tc, test_csifw_get_interval_fail);
    tcase_add_test(tc, test_csifw_get_config);
    tcase_add_test(tc, test_csifw_get_config_fail);
    tcase_add_test(tc, test_csifw_get_ap_mac_addr);
    tcase_add_test(tc, test_csifw_get_ap_mac_addr_fail);

    tcase_add_test(tc, test_csi_framework_null_context);
    tcase_add_test(tc, test_start_csi_framework_already_running);
    tcase_add_test(tc, test_stop_csi_framework_already_stopped);

    tcase_add_test(tc, test_csi_framework_taskMngrReg_Fail);
    tcase_add_test(tc, test_csi_framework_PcktRecvInit_Fail);
    tcase_add_test(tc, test_csi_framework_PcktRecvStart_Fail);
    tcase_add_test(tc, test_csi_framework_PcktRecvStop_Fail);
    tcase_add_test(tc, test_csi_framework_PingInit_Fail);
}
