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

#include <stdio.h>
#include "include/csifw.h"
#include "include/PingGenerator.h"
#include "include/CSIParser.h"
#include "include/CSIPacketReceiver.h"
#include "include/CSINetworkMonitor.h"

COLLECT_STATE g_service_state = CSI_STATE_UNITIALIZED;

typedef enum _CLIENT_STATE {
	CSI_CLIENT_UNREGISTERED = 0,      /* CLIENT NOT REGISTERED      */
	CSI_CLIENT_STOP = 1,              /* CLIENT NOT COLLECTING DATA */
	CSI_CLIENT_START = 2,             /* CLIENT COLLECTING DATA     */
} CLIENT_STATE;

typedef struct CSIFW_CLIENT_INFO_T{
	pthread_t client_tid; // thread id of client thread
	CLIENT_STATE state;
	client_raw_data_listener raw_cb;
	client_parsed_data_listener parsed_cb;
	void* client_data;
} csifw_client_info_t;

int SYSTEM_TAG_CSIFW;
static float *g_parsed_buffptr;
static uint16_t gParsedDataBufferLen;
static unsigned int g_service_interval;
static csi_config_type_t g_csi_config_type;

/* Array of pointers to registered clients' struct */
static csifw_client_info_t g_client_info[CSIFW_MAX_NUM_APPS];
static unsigned int g_num_clients;
static unsigned int g_num_start_clients;
static unsigned int g_count_parsed_clients;
static sem_t g_sema;

/* Private functions */
static int get_client_idx(void);
static int get_empty_idx(void);
static CSIFW_RES start_csi(void);
static CSIFW_RES stop_csi(CSIFW_REASON reason);
static int register_csi_client(csi_config_type_t config_type, client_raw_data_listener raw_callback, client_parsed_data_listener parsed_callback, unsigned int interval, void* ptr);
static CSIFW_RES start_csi_client(void);
static CSIFW_RES stop_csi_client(void);
static CSIFW_RES unregister_csi_client(void);
static CSIFW_RES unregister_csi_client(void);
static void nw_state_notify_clients(CONNECTION_STATE state);

#define WAIT_SEMAPHORE(semaphore) \
    if (sem_wait(&(semaphore)) < OK) { \
        CSIFW_LOGE("Semaphore wait failed, errno: %d", get_errno()); \
        return CSIFW_ERROR; \
    }
#define POST_SEMAPHORE(semaphore) \
	sem_post(&(semaphore));

static void csi_network_state_listener(CONNECTION_STATE state)
{
	CSIFW_LOGD("Network Status %d", state);
	//Start Stop CSI config
	if (state == WIFI_DISCONNECTED) {
		if (g_service_state == CSI_COLLECT_STATE_STARTED) {
			CSIFW_LOGD("Stopping CSI Service due to wifi disconnection");
			if (stop_csi(CSIFW_WIFI_DISCONNECTED) != CSIFW_OK) {
				CSIFW_LOGE("CSI Stop service failed");
			}
			g_service_state = CSI_COLLECT_STATE_START_WAITING_FOR_NW;
		}
	} else if (state == WIFI_CONNECTED) {
	 	if(g_service_state == CSI_COLLECT_STATE_START_WAITING_FOR_NW) {
			CSIFW_LOGD("Starting service on wifi reconnection");
			if (start_csi() != CSIFW_OK) {
				CSIFW_LOGE("CSI Start service failed");
			} else {
				g_service_state = CSI_COLLECT_STATE_STARTED;
			}
		}
	}
	nw_state_notify_clients(state);
}

static void CSIRawDataListener(CSIFW_RES res, int raw_csi_buff_len, unsigned char *raw_csi_buff, int raw_csi_data_len)
{
	//Send data to clients
	if (g_count_parsed_clients > 0) {
		getParsedData(raw_csi_buff, raw_csi_buff_len, g_csi_config_type, g_parsed_buffptr, &gParsedDataBufferLen);
	}
	for(int i = 0; i < CSIFW_MAX_NUM_APPS; i++) {
		//send raw
		if (g_client_info[i].state == CSI_CLIENT_START) {
			if (g_client_info[i].raw_cb) {
				g_client_info[i].raw_cb(res, raw_csi_buff_len, raw_csi_buff, g_client_info[i].client_data);
			}
		//send parsed
			if (g_client_info[i].parsed_cb) {
				g_client_info[i].parsed_cb(res, gParsedDataBufferLen, g_parsed_buffptr, g_client_info[i].client_data);
			}
		}
	}
}

CSIFW_RES csi_service_init(csi_config_type_t config_type, client_raw_data_listener raw_callback, client_parsed_data_listener parsed_callback, unsigned int interval, void* ptr)
{	
	CSIFW_LOGE("CSIPID[%d]", pthread_self());
	if (g_num_clients == 0) {
		if (sem_init(&g_sema, 0, 1) < OK) {
			CSIFW_LOGE("Semaphore init failed, errno: %d", get_errno());
			return CSIFW_ERROR;
		}
	}
	// manage client
	int idx = register_csi_client(config_type, raw_callback, parsed_callback, interval, ptr);
	if (idx == -1) {
		CSIFW_LOGE("Failed to register client");
		return CSIFW_ERROR;
	}
	// manage csifw init
	if (g_num_clients != 1) {
		CSIFW_LOGE("Already Initialized with %d client(s).", g_num_clients);
		return CSIFW_OK;
	}

	// if num_client == 1 -> Init csi service
	CSIFW_LOGI("Initializing CSIFW");
	CSIFW_RES res = CSIFW_OK;
	g_csi_config_type = config_type;
	g_service_interval = interval;
		ping_generator_change_interval(g_service_interval);
	
	// if (g_csi_config_type == HT_CSI_DATA || g_csi_config_type == HT_CSI_DATA_ACC1) {
	// 	ping_generator_change_interval(g_service_interval);
	// } else if (g_csi_config_type == NON_HT_CSI_DATA || g_csi_config_type == NON_HT_CSI_DATA_ACC1) {
	// 	ping_generator_change_interval(0);
	// }

	gParsedDataBufferLen = CSIFW_MAX_RAW_BUFF_LEN;
	g_parsed_buffptr = (float *)malloc(sizeof(float) * gParsedDataBufferLen);
	if (!g_parsed_buffptr) {
		CSIFW_LOGE("Failed to allocate memory for parsed data buffer, size: %zu", sizeof(float) * gParsedDataBufferLen);
		// if init fails remove the client
		if (unregister_csi_client() != CSIFW_OK) {
			CSIFW_LOGE("Client unregister fail.");
		}
		return CSIFW_ERROR;
	}
	res = csi_packet_receiver_init(config_type, interval, CSIRawDataListener);
	if (res != CSIFW_OK) {
		CSIFW_LOGE("csi_packet_receiver_init failed");
		if (unregister_csi_client() != CSIFW_OK) {
			CSIFW_LOGE("Client unregister fail.");
		}
		if (!g_parsed_buffptr) {
			free(g_parsed_buffptr);
			g_parsed_buffptr = NULL;
		}
		return CSIFW_ERROR;
	}

	CSIFW_LOGD("CSI Packet receiver init done");
	res = network_monitor_init(csi_network_state_listener);
	if (res != CSIFW_OK) {
		CSIFW_LOGE("network Monitor init failed");
	} else {
		g_service_state = CSI_STATE_INITIALIZED;
	}
	return res;
}

CSIFW_RES csi_service_start(void)
{	
	CSIFW_RES res = start_csi_client();
	if (res != CSIFW_OK){
		CSIFW_LOGE("Client start failed");
		return res;
	}
	CSIFW_LOGI("Client start success");
	// now manage csi service start
	if (g_service_state == CSI_COLLECT_STATE_STARTED) {
		CSIFW_LOGE("CSI Service already running");
		return CSIFW_OK;
	}
	// if service not running check wifi connection state
	if (checkWifiConnection() != CSIFW_OK) {
		CSIFW_LOGE("Wifi not connected. Waiting for wifi connection to start");
		g_service_state = CSI_COLLECT_STATE_START_WAITING_FOR_NW;
		return CSIFW_ERROR_WIFI_NOT_CONNECTED;
	}
	res = start_csi();
	if (res != CSIFW_OK) {
		CSIFW_LOGE("Start CSI fail %d", res);
		if (stop_csi_client() != CSIFW_OK) {
			CSIFW_LOGE("Client stop fail.");
		}
		return res;
	}
	g_service_state = CSI_COLLECT_STATE_STARTED;
	return res;
}

CSIFW_RES csi_service_stop(CSIFW_REASON reason)
{
	//manage client
	CSIFW_LOGE("CSIPID[%d]", pthread_self());
	CSIFW_RES res = CSIFW_OK;
	res = stop_csi_client();
	if (res != CSIFW_OK) {
		CSIFW_LOGE("Client stop fail");
		return res;
	}
	CSIFW_LOGI("Client stopped");
	if (reason == CSIFW_WIFI_DISCONNECTED) {
		CSIFW_LOGI("[CSIFW_REASON] CSIFW_WIFI_DISCONNECTED %d", reason);
	} else {
		CSIFW_LOGI("[CSIFW_REASON] CSIFW_NORMAL %d", reason);
	}

	if (g_service_state == CSI_COLLECT_STATE_STOPPED) {
		CSIFW_LOGI("CSIFW service already stopped");
		return CSIFW_OK; // already stopped due to N/W disconnection
	}
	if (reason == CSIFW_NORMAL && g_num_start_clients != 0) {
			CSIFW_LOGI("CSIFW service will continue running for %d client(s).", g_num_start_clients);
			return CSIFW_OK;
	}
	res = stop_csi(reason);
	if (res != CSIFW_OK) {
		CSIFW_LOGE("Stop CSI fail. %d", res);
		return res;
	}
	g_service_state = CSI_COLLECT_STATE_STOPPED;
	return res;
}

CSIFW_RES csi_get_ap_mac_addr(csifw_mac_info *mac_info) {
	return csi_packet_receiver_get_mac_addr(mac_info);
}

CSIFW_RES csi_service_deinit(void)
{
	CSIFW_LOGD("csi_service_deinit");
	CSIFW_LOGE("CSIPID[%d]", pthread_self());
	//manage client
	CSIFW_RES res = unregister_csi_client();
	if (res != CSIFW_OK) {
		CSIFW_LOGE("Failed to unregister client");
		return res;
	}
	CSIFW_LOGI("Client unregistered with CSIFW");

	// manage csifw deinit
	if (g_num_clients != 0) {
		CSIFW_LOGI("CSIFW will not deinit due to %d registered client(s).", g_num_clients);
		return CSIFW_OK;
	}
	res = csi_packet_receiver_deinit();
	if (res != CSIFW_OK) {
		CSIFW_LOGE("CSI packet receiver deinit failed");
		res = CSIFW_ERROR;
	}
	if (g_parsed_buffptr) {
		free(g_parsed_buffptr);
		g_parsed_buffptr = NULL;
	}
	if (sem_destroy(&g_sema) < OK) {
		CSIFW_LOGE("Semaphore destroy failed, errno: %d", get_errno());
	}
	CSIFW_LOGD("csi_service_deinit done");
	res = network_monitor_deinit();
	if (res != CSIFW_OK) {
		CSIFW_LOGE("N/W monitor deinit failed");
		res = CSIFW_ERROR;
	}
	return res;
}

CSIFW_RES csi_service_change_interval(unsigned int interval)
{
	CSIFW_LOGD("csi_service_change_interval");
	CSIFW_LOGE("CSIPID[%d]", pthread_self());
	if (g_service_state != CSI_COLLECT_STATE_STARTED) {
		CSIFW_LOGE("CSIFW service not running");
		return CSIFW_ERROR; // 
	}
	if(g_service_interval == interval){
		CSIFW_LOGE("Same interval already set");
		return CSIFW_ERROR; //
	}
	WAIT_SEMAPHORE(g_sema)
	g_service_interval = interval;
	int res = CSIFW_OK;
		ping_generator_change_interval(interval);

	// if(g_csi_config_type == HT_CSI_DATA || g_csi_config_type == HT_CSI_DATA_ACC1){
	// 	ping_generator_change_interval(interval);
	// }
	// else{
	// 	res = csi_packet_change_interval(interval);
	// }
	POST_SEMAPHORE(g_sema);
	return res;
}

unsigned int csi_service_get_current_interval(void)
{
	CSIFW_LOGD("csi_service_get_current_interval");
	return g_service_interval;
}

static CSIFW_RES start_csi(void)
{
	CSIFW_RES res = csi_packet_receiver_start_collect();
	if (res != CSIFW_OK){
		CSIFW_LOGE("CSI packet receiver start failed");
		return res;
	}
	res = ping_generator_start();
	if (res != CSIFW_OK){
		CSIFW_LOGE("CSI ping generator start failed");
	}
	return res;
}

static CSIFW_RES stop_csi(CSIFW_REASON reason)
{
	CSIFW_RES res = ping_generator_stop();
	if (res != CSIFW_OK) {
		CSIFW_LOGE("CSI ping generator stop failed");
	}
	res = csi_packet_receiver_stop_collect(reason);
	if (res != CSIFW_OK) {
		CSIFW_LOGE("CSI packet receiver stop failed");
	}
	return res;
}

/* Client handling functions */

static int get_client_idx(void)
{
	pthread_t tid = pthread_self();
	if (g_num_clients == 0) {
		return -1;
	}
	for (int i = 0; i < CSIFW_MAX_NUM_APPS; i++) {
		if (g_client_info[i].client_tid == tid) {
			return i;
		}
	}
	return -1;
}

static int get_empty_idx(void)
{
	if (g_num_clients == 0) {
		return 0;
	}
	for (int i = 0; i < CSIFW_MAX_NUM_APPS; i++) {
		if (g_client_info[i].client_tid == 0) {
			return i;
		}
	}
	return -1;
}

/**
 * @brief Register a CSI client with the CSI service.
 *
 * @param config_type Type of CSI data to collect.
 * @param raw_callback Callback function to receive raw CSI data.
 * @param parsed_callback Callback function to receive parsed CSI data.
 * @param interval Interval between CSI data collections.
 * @param ptr Pointer to client-specific data.
 *
 * @return Index of register client on success, -1 in case of failure.
 */
static int register_csi_client(csi_config_type_t config_type, client_raw_data_listener raw_callback, client_parsed_data_listener parsed_callback, unsigned int interval, void* ptr)
{
	CSIFW_LOGE("CSIPID[%d]", pthread_self());
	// checks on client

	// max client registered
	if (g_num_clients == CSIFW_MAX_NUM_APPS) {
		CSIFW_LOGE("Max number of clients reached");
		return -1;
	}
	// config_type valid?
	if (config_type <= MIN_CSI_CONFIG_TYPE || config_type >= MAX_CSI_CONFIG_TYPE) {
		CSIFW_LOGE("Invalid config type");
		return -1;
	}
	// if num_client > 1 -> is config and interval same as first client
	if (g_num_clients != 0) {
		if (g_csi_config_type != config_type) {
			CSIFW_LOGE("Already initialized with different config type: %d", g_csi_config_type);
			return -1;
		}
		if (g_service_interval != interval) {
			CSIFW_LOGE("[Warning!!!]Already initialized with different interval: %d", g_service_interval);  // new service is allowed for csi data, but not allowed to change interval.
		}
	}

	// register the client
	pthread_t tid = pthread_self();
	/* Check if already registered */
	int idx = get_client_idx();
	if (idx != -1) {
		CSIFW_LOGE("Client with ID: %d already registered at index %d", tid, idx);
		return -1;
	}
	/* get empty index */
	idx = get_empty_idx();
	/* Take sem */
	if (sem_wait(&g_sema) < OK) {
		CSIFW_LOGE("Semaphore wait failed, errno: %d", get_errno());
		return CSIFW_ERROR;
	}
	g_client_info[idx].client_tid = tid;
	g_client_info[idx].state = CSI_CLIENT_STOP;
	g_client_info[idx].raw_cb = raw_callback;
	g_client_info[idx].parsed_cb = parsed_callback;
	g_client_info[idx].client_data = ptr;
	g_num_clients++;
	/* Give sem */
	sem_post(&g_sema);
	CSIFW_LOGI("Idx%d: Client(%d) registered with CSIFW. Total clients: %d", idx, tid, g_num_clients);
	return idx; // return index of client
}

static CSIFW_RES start_csi_client(void)
{
	CSIFW_LOGE("CSIPID[%d]", pthread_self());
	int tid = pthread_self();
	/* get client index */
	int idx = get_client_idx();
	if (idx == -1) {
		CSIFW_LOGE("Client(%d) start failed, client not registered.", tid);
		return CSIFW_ERROR_CLIENT_NOT_REGISTERED;
	}
	if (g_client_info[idx].state == CSI_CLIENT_START) {
		CSIFW_LOGE("Client already started");
		return CSIFW_ERROR;
	}
	/* Take sem */
	if (sem_wait(&g_sema) < OK) {
		CSIFW_LOGE("Semaphore wait failed, errno: %d", get_errno());
		return CSIFW_ERROR;
	}
	g_client_info[idx].state = CSI_CLIENT_START;
	g_num_start_clients++;
	if (g_client_info[idx].parsed_cb) {
		g_count_parsed_clients++; // count clients who need parsed data
	}
	/* Give sem */
	sem_post(&g_sema);
	CSIFW_LOGI("Client(%d) start success", tid);
	return CSIFW_OK;
}

static CSIFW_RES stop_csi_client(void)
{
	// find client index and stop
	int tid = pthread_self();
	/* get client index */
	int idx = get_client_idx();
	if (idx == -1) {
		CSIFW_LOGE("Client(%d) stop failed, client not registered.", tid);
		return CSIFW_ERROR_CLIENT_NOT_REGISTERED;
	}
	if (g_client_info[idx].state == CSI_CLIENT_STOP) {
		CSIFW_LOGE("Client already stopped");
		return CSIFW_ERROR;
	}
	/* Take sem */
	if (sem_wait(&g_sema) < OK) {
		CSIFW_LOGE("Semaphore wait failed, errno: %d", get_errno());
		return CSIFW_ERROR;
	}
	g_client_info[idx].state = CSI_CLIENT_STOP;
	g_num_start_clients--;
	if (g_client_info[idx].parsed_cb) {
		g_count_parsed_clients--; // count clients who need parsed data
	}
	/* Give sem */
	sem_post(&g_sema);
	CSIFW_LOGI("Client(%d) stop success", tid);
	return CSIFW_OK;
}

static CSIFW_RES unregister_csi_client(void)
{
	CSIFW_RES res = CSIFW_OK;
	if (g_num_clients == 0) {
		CSIFW_LOGE("No clients registered");
		return CSIFW_ERROR_CLIENT_NOT_REGISTERED;
	}
	pthread_t tid = pthread_self();
	int idx = get_client_idx();
	if (idx == -1) {
		CSIFW_LOGE("Client with ID: %d not registered", tid);
		return CSIFW_ERROR_CLIENT_NOT_REGISTERED;
	}
	if (g_client_info[idx].state != CSI_CLIENT_STOP) {
		CSIFW_LOGD("Client (pid: %d) not stopped. Stopping client before unregistering", tid);
		res = csi_service_stop(CSIFW_NORMAL);
		if (res != CSIFW_OK) {
			CSIFW_LOGE("Failed to stop client");
		}
	}
	//unregister after stop
	/* Take sem */
	if (sem_wait(&g_sema) < OK) {
		CSIFW_LOGE("Semaphore wait failed, errno: %d", get_errno());
		return CSIFW_ERROR;
	}
	g_client_info[idx].state = CSI_CLIENT_UNREGISTERED;
	g_client_info[idx].client_tid = 0;
	g_num_clients--;
	/* Give sem */
	sem_post(&g_sema);
	CSIFW_LOGI("Idx%d: Client(%d) unregistered with CSIFW. Total clients: %d", idx, tid, g_num_clients);
	return res;
}

static void nw_state_notify_clients(CONNECTION_STATE state)
{
	CSIFW_RES res = CSIFW_OK_WIFI_CONNECTED;
	if(state == WIFI_DISCONNECTED) {
		res = CSIFW_ERROR_WIFI_DIS_CONNECTED;
	}
	for(int i = 0; i < CSIFW_MAX_NUM_APPS; i++) {
		//send raw
		if (g_client_info[i].state != CSI_CLIENT_UNREGISTERED) {
			if (g_client_info[i].raw_cb) {
				g_client_info[i].raw_cb(res, 0, NULL, g_client_info[i].client_data);
			}
		}
	}
}

