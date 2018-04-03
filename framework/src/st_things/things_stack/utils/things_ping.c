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

#define OICPING_ENABLE      1

#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include "ocpayload.h"
#include "octypes.h"
#include "easy-setup/es_common.h"
#include "utils/things_malloc.h"
#include "utils/things_string.h"
#include "logging/things_logger.h"
#include "things_list.h"
#include "things_ping.h"

#ifdef __ST_THINGS_RTOS__
#include "things_rtos_util.h"
#endif

#ifdef OICPING_ENABLE
#include "oickeepalive.h"
#endif
#include "easy-setup/resource_handler.h"
#include "cloud/cloud_manager.h"

#include "things_thread.h"

#define TAG "[things_ping]"

#define DEFAULT_CONTEXT_VALUE   0x99
#define PING_UNIT           60	// 60 seconds is minimum value of ping.
#define NET_TROUBLE_DELAY   5	// 5 seconds
#define MAX_INTERVAL_EA     4	// Cloud Support 4 EA Interval class.

#define KEEPALIVE_RESOURCE_URI "/oic/ping"

typedef void *(*pthread_func_type)(void *);
typedef struct things_ping_s {
	pthread_mutex_t mutex_state;	// mutex for State variable.
	ping_state_e bit_mask_state;	// State of Ping

	pthread_mutex_t mutex_thread;	// mutex for handle_thread variable.
	pthread_t handle_thread;	// Ping-Thread Handler
	bool continue_thread;		// Continue Ping-Thread Flag.
	char *addr;					// IP address of remote
	uint16_t port;				// Port number for remote

	pthread_mutex_t mutex_int;	// mutex for interval variable.
	int64_t interval[MAX_INTERVAL_EA];	// Interval EA  (unit : minute , max : 60)
	int int_seq;				// sequence of interval
} things_ping_s;

static list_s *list = NULL;
static const char INTERVAL_ARRAY[] = "inarray";
static const char INTERVAL[] = "in";
things_ping_s* p_ping = NULL;

static void *thd_ping_loop(things_ping_s *ping);
static OCStackApplicationResult discover_ping_resource_handler(void *ctx, OCDoHandle handle, OCClientResponse *client_response);
static OCStackApplicationResult handler_ping_request(void *ctx, OCDoHandle handle, OCClientResponse *client_response);
static OCStackApplicationResult handler_ping_update_request(void *ctx, OCDoHandle handle, OCClientResponse *client_response);

static things_ping_s *create_things_ping_s(const char *remote_addr, const uint16_t port);
static void terminate_things_ping_s(things_ping_s *ping);
static int is_state_key_equal(things_ping_s *ping, const ping_state_e value);
static int is_ip_key_equal(things_ping_s *ping, const char *target_ip);
static void check_ping_thread(things_ping_s *ping);
static void set_mask(things_ping_s *ping, ping_state_e state);
static bool get_mask(things_ping_s *ping, ping_state_e state);
static bool cas_mask(things_ping_s *ping, ping_state_e preState, bool preStateThen, ping_state_e post_state);
static void unset_mask(things_ping_s *ping, ping_state_e state);
static int find_resource_oic_ping(things_ping_s *ping);
static int send_things_ping_request(things_ping_s *ping);
static bool set_interval_send_update(things_ping_s *ping, int64_t *intervals, int int_ea);
static bool set_interval(things_ping_s *ping, int64_t *intervals, int int_ea, ping_state_e confirm_state);
static bool set_def_interval(things_ping_s *ping);
static bool things_ping_create_thread(things_ping_s *ping);
static bool things_ping_destroy_thread(things_ping_s *ping);
static void things_rep_payload_value_print(OCRepPayloadValue *value);
static char *make_host_addr(const char *ip, const uint16_t port);
static OCStackResult send_keep_alive_update_request(const char *host, int64_t *intervals, int int_ea, OCCallbackData *cb);

#ifdef OICPING_ENABLE
bool things_ping_init(void)
{
	THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

	bool res = false;

	if (list == NULL) {
		if ((list = create_list()) == NULL) {
			THINGS_LOG_ERROR(THINGS_ERROR, TAG, "memory allocation is failed.");
			return res;
		}

		res = true;
	} else {
		res = true;
	}

	p_ping = NULL;
	THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
	return res;
}

bool things_ping_terminate(void)
{
	THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

	things_ping_s *ping = NULL;

	if (list) {
		THINGS_LOG(THINGS_DEBUG, TAG, "things_ping_s terminate.");
		while ((ping = (things_ping_s *) list->pop(list)) != NULL) {
			THINGS_LOG_D(THINGS_DEBUG, TAG, "Terminate ping.(%s)", ping->addr);
			if (cas_mask(ping, PING_ST_STARTTHREAD, true, PING_ST_INIT) == true) {
				things_ping_destroy_thread(ping);
			}
			terminate_things_ping_s(ping);
		}

		THINGS_LOG_D(THINGS_DEBUG, TAG, "Terminate ping-list.");
		terminate_list(list);
		list = NULL;
	}

	THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
	return true;
}

bool things_ping_set_mask(const char *remote_addr, uint16_t port, ping_state_e state)
{
	THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

	things_node_s *node = NULL;
	things_ping_s *ping = NULL;

	if (remote_addr == NULL || strlen(remote_addr) == 0) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid arguments.(remote_addr=%s, state=%d)", remote_addr, state);
		return false;
	}

	if (list == NULL) {
		THINGS_LOG_D(THINGS_INFO, TAG, "OICPing Module is not initialized.");
		return false;
	}

	node = list->find_by_key(list, (key_compare) is_ip_key_equal, remote_addr);
	if (node == NULL) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Not Found things_node_s for remote(%s). So, Create Node.", remote_addr);
		if ((ping = create_things_ping_s(remote_addr, port)) == NULL) {
			THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "memory allocation is failed.");
			return false;
		}
		list->insert(list, (void *)ping);
	} else {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Found things_node_s for remote(%s).", remote_addr);
		ping = node->item;
	}

	if (ping == NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Not exist things_ping_s in remote_addr_Node.(%s)", remote_addr);
		return false;
	}

	if (port != 0) {
		ping->port = port;
	}

	set_mask(ping, state);
	check_ping_thread(ping);

	THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
	return true;
}

int things_ping_get_mask(const char* remote_addr, ping_state_e state)
{
	THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");
	things_node_s *node = NULL;
	things_ping_s *ping = NULL;
	
	if (remote_addr == NULL || strlen(remote_addr) == 0) {
		THINGS_LOG_V(THINGS_ERROR, TAG, "Invalid arguments.(remote_addr=%s, state=%d)", remote_addr, state);
		return -1;
	}
	
	if (list == NULL) {
		THINGS_LOG_D(THINGS_INFO, TAG, "OICPing Module is not initialized.");
		return -1;
	}
	
	node = list->find_by_key(list, (key_compare) is_ip_key_equal, remote_addr);
	if (node == NULL) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Not Found OICnode for remote(%s).", remote_addr);
		return -1;
	} else {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Found OICnode for remote(%s).", remote_addr);
		ping = node->item;
	}
	
	if (ping == NULL) {
		THINGS_LOG_V(THINGS_ERROR, TAG, "Not exist oicping_s in remote_addr_Node.(%s)", remote_addr);
		return false;
	}
	
	THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
	return (int) get_mask(ping, state);
}


bool oic_ping_unset_mask(const char *remote_addr, ping_state_e state)
{
	THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");
	things_node_s *node = NULL;
	things_ping_s *ping = NULL;

	if (remote_addr == NULL || strlen(remote_addr) == 0) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid arguments.(remote_addr=%s, state=%d)", remote_addr, state);
		return false;
	}

	if (list == NULL) {
		THINGS_LOG_D(THINGS_INFO, TAG, "OICPing Module is not initialized.");
		return false;
	}

	node = list->find_by_key(list, (key_compare) is_ip_key_equal, remote_addr);
	if (node == NULL) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Not Found things_node_s for remote(%s).", remote_addr);
		return false;
	} else {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Found things_node_s for remote(%s).", remote_addr);
		ping = node->item;
	}

	if (ping == NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Not exist things_ping_s in remote_addr_Node.(%s)", remote_addr);
		return false;
	}

	unset_mask(ping, state);

	check_ping_thread(ping);
	THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
	return true;
}

bool things_ping_set_cloud_interval(int64_t *intervals, int int_ea)
{
	THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");
	
	bool res = false;
	things_node_s* node = NULL;
	things_ping_s* ping = NULL;
	
	if (intervals == NULL || int_ea == 0) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Invalid arguments.(intervals=0x%X, int_ea=%d)", intervals, int_ea);
		return false;
		}
		
	if (list == NULL) {
		THINGS_LOG_D(THINGS_INFO, TAG, "OICPing Module is not initialized.");
		return false;
	}

	node = list->find_by_key(list, (key_compare) is_state_key_equal, PING_ST_ISCLOUD);
	if (node == NULL) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Not Found OICnode for Cloud-Connection.");
		return false;
	}

	if ((ping = node->item) == NULL) {
		THINGS_LOG_V(THINGS_ERROR, TAG, "Not exist oicping_s in Cloud_Node.");
		return false;
	}
	THINGS_LOG_D(THINGS_DEBUG, TAG, "Found OICnode for Cloud-Connection(IP:Port=%s:%d).", ping->addr, ping->port);

	res = set_interval_send_update(ping, intervals, int_ea);

	THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
	return res;
}

bool things_ping_set_interval(const char *remote_addr, int64_t *intervals, int int_ea)
{
	THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

	bool res = false;
	things_node_s* node = NULL;
	things_ping_s* ping = NULL;

	if (remote_addr == NULL || intervals == NULL || int_ea == 0) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Invalid arguments.(remote=%s, intervals=0x%X, int_ea=%d)", remote_addr, intervals, int_ea);
		return false;
	}

	if (list == NULL) {
		THINGS_LOG_D(THINGS_INFO, TAG, "OICPing Module is not initialized.");
		return false;
	}

	node = list->find_by_key(list, (key_compare) is_ip_key_equal, remote_addr);
	if (node == NULL) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Not Found OICnode for remote(%s).", remote_addr);
		return false;
	}

	THINGS_LOG_D(THINGS_DEBUG, TAG, "Found OICnode for remote(%s).", remote_addr);
	if ((ping = node->item) == NULL) {
		THINGS_LOG_V(THINGS_ERROR, TAG, "Not exist oicping_s in remote_addr_Node.(%s)", remote_addr);
		return false;
	}

	res = set_interval_send_update(ping, intervals, int_ea);

	THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
	return res;
}

#else
bool things_ping_init(void)
{
	return true;
}

bool things_ping_terminate(void)
{
	return true;
}

bool things_ping_set_mask(const char *remote_addr, uint16_t port, ping_state_e state)
{
	return true;
}

int things_ping_get_mask(const char *remote_addr, PING_STATE state)
{
	return 1;
}

bool things_ping_unset_mask(const char *remote_addr, ping_state_e state)
{
	return true;
}

bool things_ping_set_cloud_interval(int64_t *intervals, int int_ea)
{
	return true;
}

bool things_ping_set_interval(const char *remote_addr, int64_t *intervals, int int_ea)
{
	return true;
}
#endif

/***************************************************************************************
 *
 * Interval Ping-Thread Definition Function.
 *
 ***************************************************************************************/
static void *__attribute__((optimize("O0"))) thd_ping_loop(things_ping_s *ping)
{
	THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

	int sleepMinute = 0;

	if (ping == NULL || ping->addr == NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error]: Invalid arguments.(pint=0x%X)", ping);
		return NULL;
	}

	THINGS_LOG_D(THINGS_DEBUG, TAG, "IP=%s, handle=0x%X, State=0x%X", ping->addr, ping->handle_thread, ping->bit_mask_state);

	pthread_mutex_lock(&ping->mutex_int);
	for (int i = 0; i < MAX_INTERVAL_EA; i++) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Interval[%d]=%lld", i, ping->interval[i]);
	}

	THINGS_LOG_D(THINGS_DEBUG, TAG, "Intseq= %d", ping->int_seq);
	THINGS_LOG_D(THINGS_DEBUG, TAG, "ContinuThread= %d", ping->continue_thread);

	if (ping->interval[0] == (int64_t) 0 || ping->continue_thread == true) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error]: Invalid Interval-Time or Already Run-Thread for this ping.(interval[0]=%lld, continueFlag=%d)", ping->interval[0], ping->continue_thread);
		pthread_mutex_unlock(&ping->mutex_int);
		return NULL;
	}
	pthread_mutex_unlock(&ping->mutex_int);

	THINGS_LOG_D(THINGS_DEBUG, TAG, "Start FindResource for /oic/ping to Cloud(%s)", ping->addr);
	do {						// /oic/ping Resource Finding Start
		int sleepTime = 1;
		sleepTime = find_resource_oic_ping(ping);
		sleep(sleepTime);
	} while (!ping->continue_thread && ping->handle_thread);

	p_ping = ping;
	int sleepDelay = 10;

	THINGS_LOG_D(THINGS_DEBUG, TAG, "Start common-Ping request for /oic/ping to Cloud(%s)", ping->addr);
	while (ping->continue_thread) {	// common-ping request Sending Start

		if (sleepMinute == 0) {
			sleepMinute = send_things_ping_request(ping);
		}

		if (sleepMinute) {
#ifdef __ST_THINGS_RTOS__
			int sleepCount = PING_UNIT - NET_TROUBLE_DELAY - sleepDelay;
			sleepDelay = 0;
			int cCount = 0;
			while (1) {
				if (cCount > sleepCount) {
					break;
				}

				sleep(1);

				if (ping->continue_thread == false) {
					sleepMinute = 0;
					break;
				}
				cCount++;
			}
#else
			sleep(PING_UNIT - NET_TROUBLE_DELAY);
#endif

			sleepMinute--;
		} else {
			sleep(1);
		}

		if (get_mask(ping, PING_ST_INTUPDATED) == true) {
			sleepMinute = 0;
			unset_mask(ping, PING_ST_INTUPDATED);
		}
	}

	unset_mask(ping, PING_ST_STARTTHREAD | PING_ST_DISCOVERY | PING_ST_REQUEST | PING_ST_INTUPDATE | PING_ST_TIMEOUT);
	ping->continue_thread = false;
	set_def_interval(ping);
	p_ping = NULL;
	THINGS_LOG_V(THINGS_INFO, TAG, "\e[35mSTOP PING THREAD / TARGET : %s\e[m", ping->addr);
	THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
	return (void *)1;
}

static OCStackApplicationResult discover_ping_resource_handler(void *ctx, OCDoHandle handle, OCClientResponse *client_response)
{
	THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

	int64_t *interval = NULL;
	size_t dimensions[MAX_REP_ARRAY_DEPTH] = { 0, 0, 0 };
	things_ping_s *ping = (things_ping_s *) ctx;

	if (ctx == (void *)DEFAULT_CONTEXT_VALUE || ctx == NULL || ping->addr == NULL) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "content value is invalid.(ctx=0x%X)", ctx);
		return OC_STACK_DELETE_TRANSACTION;
	}

	if (client_response) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "ctx=0x%X", ctx);
		THINGS_LOG_D(THINGS_DEBUG, TAG, "StackResult: %s(%d)", get_result(client_response->result), client_response->result);

		THINGS_LOG_D(THINGS_DEBUG, TAG, "Device =============> Discovered @ %s:%d", client_response->devAddr.addr, client_response->devAddr.port);

		if (strncmp(ping->addr, client_response->devAddr.addr, strlen(client_response->devAddr.addr)) != 0) {
			THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error]: Mismatching address.(ping->addr=%s, recvAddr=%s)", ping->addr, client_response->devAddr.addr);
			return OC_STACK_DELETE_TRANSACTION;
		}
		// Parsing payload.
		if (client_response->payload != NULL && client_response->payload->type == PAYLOAD_TYPE_REPRESENTATION) {
			OCRepPayloadValue *val = ((OCRepPayload *) client_response->payload)->values;
			while (val != NULL) {
				things_rep_payload_value_print(val);
				val = val->next;
			}

			OCRepPayloadGetIntArray(((const OCRepPayload *)client_response->payload), INTERVAL_ARRAY, &interval, dimensions);
			THINGS_LOG_D(THINGS_DEBUG, TAG, "dimension=[%d, %d, %d]", dimensions[0], dimensions[1], dimensions[2]);
			for (int i = 0; i < dimensions[0]; i++) {
				THINGS_LOG_D(THINGS_DEBUG, TAG, "interval[%d]=%d", i, interval[i]);
			}
		}
		// Result processing.
		switch (client_response->result) {
		case OC_STACK_OK:
			THINGS_LOG_D(THINGS_DEBUG, TAG, "Discovery Success.");
			if (interval == NULL || interval[0] == 0 || dimensions[0] == 0) {
				THINGS_LOG_D(THINGS_INFO, TAG, "Invalid received remote interval or dimensions.");
				return OC_STACK_DELETE_TRANSACTION;
			}

			if (set_interval(ping, interval, dimensions[0], PING_ST_DISCOVERY) == true) {
				ping->continue_thread = true;
			}
			break;
		case OC_STACK_TIMEOUT:
			THINGS_LOG_D(THINGS_ERROR, TAG, "Occurred Time-Out Event.");
			unset_mask(ping, PING_ST_SIGNIN|PING_ST_TCPCONNECT);
			check_ping_thread(ping);
			CAUtilTCPDisconnectSession(ping->addr, ping->port, 64);
			THINGS_LOG_V(THINGS_ERROR, TAG, "DISCOVER PING RESOURCE TIME OUT, CLOUD DISCONNECT");
			break;
		default:
			THINGS_LOG_D(THINGS_DEBUG, TAG, "Not Support StackResult: %s(%d)", get_result(client_response->result), client_response->result);
			break;
		}
	} else {
		THINGS_LOG_D(THINGS_INFO, TAG, "discover_ping_resource_handler received Null client_response");
	}
	/*! Added by st_things for memory Leak fix
	 */
	if (NULL != interval) {
		things_free(interval);
	}

	THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
	return OC_STACK_DELETE_TRANSACTION;
}

static OCStackApplicationResult handler_ping_request(void *ctx, OCDoHandle handle, OCClientResponse *client_response)
{
	THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

	things_ping_s *ping = (things_ping_s *) ctx;

	if (ctx == (void *) DEFAULT_CONTEXT_VALUE || ctx == NULL || ping->addr == NULL) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "content value is invalid.(ctx=0x%X)", ctx);
		return OC_STACK_DELETE_TRANSACTION;
	}

	if (client_response) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "ctx=0x%X", ctx);
		THINGS_LOG_D(THINGS_DEBUG, TAG, "StackResult: %s(%d)", get_result(client_response->result), client_response->result);

		THINGS_LOG_D(THINGS_DEBUG, TAG, "Device =============> Discovered @ %s:%d", client_response->devAddr.addr, client_response->devAddr.port);

		if (strncmp(ping->addr, client_response->devAddr.addr, strlen(client_response->devAddr.addr)) != 0) {
			THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error]: Mismatching address.(ping->addr=%s, recvAddr=%s)", ping->addr, client_response->devAddr.addr);
			return OC_STACK_DELETE_TRANSACTION;
		}
		// Parsing payload.
		if (client_response->payload != NULL && client_response->payload->type == PAYLOAD_TYPE_REPRESENTATION) {
			OCRepPayloadValue *val = ((OCRepPayload *) client_response->payload)->values;
			while (val != NULL) {
				things_rep_payload_value_print(val);
				val = val->next;
			}
		}
		// Result processing.
		switch (client_response->result) {
		case OC_STACK_OK:
			THINGS_LOG_D(THINGS_INFO, TAG, "common-ping request Success.");
			if (get_mask(ping, PING_ST_REQUEST) == true && get_mask(ping, PING_ST_TIMEOUT) == false) {
				unset_mask(ping, PING_ST_REQUEST);
			}
			break;
		case OC_STACK_TIMEOUT:
			THINGS_LOG_D(THINGS_ERROR, TAG, "Occurred Time-Out Event.");
			unset_mask(ping, PING_ST_SIGNIN|PING_ST_TCPCONNECT);
			check_ping_thread(ping);
			CAUtilTCPDisconnectSession(ping->addr, ping->port, 64);
			THINGS_LOG_D(THINGS_ERROR, TAG, "SEND PING TIME OUT, CLOUD DISCONNECT");
			break;
		default:
			THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Not Support StackResult: %s(%d)", get_result(client_response->result), client_response->result);
			break;
		}
	} else {
		THINGS_LOG_D(THINGS_WARNING, TAG, "handler_ping_request received Null client_response");
	}

	THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
	return OC_STACK_DELETE_TRANSACTION;
}

static OCStackApplicationResult handler_ping_update_request(void *ctx, OCDoHandle handle, OCClientResponse *client_response)
{
	THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

	int64_t *interval = NULL;
	size_t dimensions[MAX_REP_ARRAY_DEPTH] = { 0, 0, 0 };
	things_ping_s *ping = (things_ping_s *) ctx;
	(void *)handle;

	if (ctx == (void *)DEFAULT_CONTEXT_VALUE || ctx == NULL || ping->addr == NULL) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "content value is invalid.(ctx=0x%X)", ctx);
		return OC_STACK_DELETE_TRANSACTION;
	}

	if (client_response) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "ctx=0x%X", ctx);
		THINGS_LOG_D(THINGS_DEBUG, TAG, "StackResult: %s(%d)", get_result(client_response->result), client_response->result);

		THINGS_LOG_D(THINGS_DEBUG, TAG, "Device =============> Discovered @ %s:%d", client_response->devAddr.addr, client_response->devAddr.port);

		if (strncmp(ping->addr, client_response->devAddr.addr, strlen(client_response->devAddr.addr)) != 0) {
			THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error]: Mismatching address.(ping->addr=%s, recvAddr=%s)", ping->addr, client_response->devAddr.addr);
			return OC_STACK_DELETE_TRANSACTION;
		}
		// Parsing payload.
		if (client_response->payload != NULL && client_response->payload->type == PAYLOAD_TYPE_REPRESENTATION) {
			OCRepPayloadValue *val = ((OCRepPayload *) client_response->payload)->values;
			while (val != NULL) {
				things_rep_payload_value_print(val);
				val = val->next;
			}

			OCRepPayloadGetIntArray(((const OCRepPayload *)client_response->payload), INTERVAL_ARRAY, &interval, dimensions);
			THINGS_LOG_D(THINGS_DEBUG, TAG, "dimension=[%d, %d, %d]", dimensions[0], dimensions[1], dimensions[2]);
			for (int i = 0; i < dimensions[0]; i++) {
				THINGS_LOG_D(THINGS_DEBUG, TAG, "interval[%d]=%d", i, interval[i]);
			}
		}
		// Result processing.
		switch (client_response->result) {
		case OC_STACK_RESOURCE_CHANGED:
			THINGS_LOG_D(THINGS_DEBUG, TAG, "update-Ping request Success.");
			if (set_interval(ping, interval, dimensions[0], PING_ST_INTUPDATE) == true) {
				THINGS_LOG_D(THINGS_DEBUG, TAG, "Success Apply Interval.");
			}
			break;
		case OC_STACK_TIMEOUT:
			THINGS_LOG_D(THINGS_ERROR, TAG, "Occurred Time-Out Event.");
			unset_mask(ping, PING_ST_SIGNIN|PING_ST_TCPCONNECT);
			check_ping_thread(ping);
			CAUtilTCPDisconnectSession(ping->addr, ping->port, 64);
			THINGS_LOG_D(THINGS_ERROR, TAG, "SEND PING TIME OUT, CLOUD DISCONNECT");
			break;
		default:
			THINGS_LOG_D(THINGS_DEBUG, TAG, "Not Support StackResult: %s(%d)", get_result(client_response->result), client_response->result);
			break;
		}
	} else {
		THINGS_LOG_D(THINGS_INFO, TAG, "handler_ping_update_request received Null client_response");
	}
	/*! Added by st_things for memory Leak fix
	 */
	if (NULL != interval) {
		things_free(interval);
	}
	THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
	return OC_STACK_DELETE_TRANSACTION;
}

/***************************************************************************************
 *
 * Interval Function Definition Collection.
 *
 ***************************************************************************************/
static things_ping_s *create_things_ping_s(const char *remote_addr, const uint16_t port)
{
	things_ping_s *ping = NULL;

	if (remote_addr == NULL || strlen(remote_addr) == 0) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "remote addr is NULL.");
		return NULL;
	}

	ping = (things_ping_s *) things_malloc(sizeof(things_ping_s));
	if (ping == NULL) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "memory allocation is failed.");
		return NULL;
	}

	ping->addr = things_strdup(remote_addr);
	ping->port = port;
	ping->continue_thread = false;
	pthread_mutex_init(&ping->mutex_int, NULL);
	pthread_mutex_init(&ping->mutex_state, NULL);
	pthread_mutex_init(&ping->mutex_thread, NULL);

	pthread_mutex_lock(&ping->mutex_thread);
	ping->handle_thread = 0;
	pthread_mutex_unlock(&ping->mutex_thread);

	set_def_interval(ping);

	set_mask(ping, PING_ST_INIT);
	return ping;
}

static void terminate_things_ping_s(things_ping_s *ping)
{
	THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

	if (ping == NULL) {
		THINGS_LOG(THINGS_DEBUG, TAG, "ping is NULL.");
		return;
	}

	THINGS_LOG(THINGS_DEBUG, TAG, "ping thread destroy check.");
	if (get_mask(ping, PING_ST_TCPCONNECT) == false && get_mask(ping, PING_ST_STARTTHREAD) == true) {	// need to destroy ping-thread.
		things_ping_destroy_thread(ping);
	}

	set_mask(ping, PING_ST_INIT);

	pthread_mutex_destroy(&ping->mutex_int);
	pthread_mutex_destroy(&ping->mutex_state);
	pthread_mutex_destroy(&ping->mutex_thread);

	if (ping->addr) {
		THINGS_LOG(THINGS_DEBUG, TAG, "ping->addr memory free.");
		things_free(ping->addr);
		ping->addr = NULL;
	}

	things_free(ping);

	THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
}

static int is_state_key_equal(things_ping_s *ping, const ping_state_e value)
{
	int res = 0;
	if (ping == NULL || ping->addr == NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid arguments.(ping=0x%X, targetIP=%s)", ping);
		return res;
	}

	if (get_mask(ping, value) == true) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Found ping-struct(%s) for State(%d)", ping->addr, value);
		res = 1;
	}

	return res;
}

static int is_ip_key_equal(things_ping_s *ping, const char *targetIP)
{
	int res = 0;
	if (ping == NULL || ping->addr == NULL || targetIP == NULL || strlen(targetIP) == 0) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid arguments.(ping=0x%X, targetIP=%s)", ping, targetIP);
		return res;
	}

	if (strncmp(ping->addr, targetIP, strlen(targetIP)) == 0) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Found ping-struct(%s) for targetIP(%s)", ping->addr, targetIP);
		res = 1;
	}

	return res;
}

static void check_ping_thread(things_ping_s *ping)
{
	THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

	if (ping == NULL || ping->addr == NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid arguments.(ping=0x%X)", ping);
		return;
	}

	if (list == NULL) {
		THINGS_LOG_D(THINGS_INFO, TAG, "OICPing Module is not initialized.");
		return;
	}

	if (get_mask(ping, PING_ST_TCPCONNECT) == true && get_mask(ping, PING_ST_STARTTHREAD) == false) {	// need to create ping-thread.
		if (get_mask(ping, PING_ST_ISCLOUD) == false || (get_mask(ping, PING_ST_ISCLOUD) == true && get_mask(ping, PING_ST_SIGNIN) == true)) {	// create thread
			things_ping_create_thread(ping);
		}
	}

	if (get_mask(ping, PING_ST_TCPCONNECT) == false && get_mask(ping, PING_ST_STARTTHREAD) == true) {	// need to destroy ping-thread.
		things_ping_destroy_thread(ping);
	}

	if (get_mask(ping, PING_ST_ISCLOUD | PING_ST_SIGNIN | PING_ST_TCPCONNECT) == false) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Not need things_ping_s, So, Terminate things_ping_s.(IP=%s)", ping->addr);
		if ((ping = list->erase_by_key(list, is_ip_key_equal, ping->addr)) != NULL) {
			THINGS_LOG_D(THINGS_DEBUG, TAG, "Erase node-element Success in List.");
			terminate_things_ping_s(ping);
		}
	}

	THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
}

static void set_mask(things_ping_s *ping, ping_state_e state)
{
	THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");
	if (ping == NULL) {
		return;
	}

	pthread_mutex_lock(&ping->mutex_state);
	THINGS_LOG_D(THINGS_DEBUG, TAG, "(B) bit_mask_state = 0x%X", ping->bit_mask_state);
	if (state != PING_ST_INIT) {
		ping->bit_mask_state |= state;
	} else {
		ping->bit_mask_state = state;
	}
	THINGS_LOG_D(THINGS_DEBUG, TAG, "(A) bit_mask_state = 0x%X", ping->bit_mask_state);
	pthread_mutex_unlock(&ping->mutex_state);

	THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
}

static bool get_mask(things_ping_s *ping, ping_state_e state)
{
	bool res = true;

	if (ping == NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "ping is NULL.");
		return false;
	}

	pthread_mutex_lock(&ping->mutex_state);
	res = (ping->bit_mask_state & state) != 0 ? true : false;
	pthread_mutex_unlock(&ping->mutex_state);

	return res;
}

static bool cas_mask(things_ping_s *ping, ping_state_e preState, bool preStateThen, ping_state_e postState)
{
	bool res = false;

	if (ping == NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "ping is NULL.");
		return false;
	}

	pthread_mutex_lock(&ping->mutex_state);
	THINGS_LOG_D(THINGS_DEBUG, TAG, "(B) bit_mask_state = 0x%X", ping->bit_mask_state);
	bool state = (ping->bit_mask_state & preState) != 0 ? true : false;
	if (state == preStateThen) {
		if (postState != PING_ST_INIT) {
			ping->bit_mask_state |= postState;
		} else {
			ping->bit_mask_state = postState;
		}
		res = true;
	}
	THINGS_LOG_D(THINGS_DEBUG, TAG, "(A) bit_mask_state = 0x%X", ping->bit_mask_state);
	pthread_mutex_unlock(&ping->mutex_state);

	THINGS_LOG_D(THINGS_DEBUG, TAG, "result = %d", res);
	return res;
}

static void unset_mask(things_ping_s *ping, ping_state_e state)
{
	THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");
	if (ping == NULL) {
		return;
	}

	pthread_mutex_lock(&ping->mutex_state);
	THINGS_LOG_D(THINGS_DEBUG, TAG, "(B) bit_mask_state = 0x%X", ping->bit_mask_state);
	if (state != PING_ST_INIT) {
		ping->bit_mask_state &= (~state);
	} else {
		ping->bit_mask_state = state;
	}
	THINGS_LOG_D(THINGS_DEBUG, TAG, "(A) bit_mask_state = 0x%X", ping->bit_mask_state);
	pthread_mutex_unlock(&ping->mutex_state);

	THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
}

#ifdef OICPING_ENABLE
static int find_resource_oic_ping(things_ping_s *ping)
{
	THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

	if (get_mask(ping, PING_ST_TCPCONNECT) == false) {
		return 1;
	}

	int sleepTime = 1;			// seconds
	OCDoHandle g_req_handle = NULL;
	OCCallbackData cb;
	char *hostAddr = NULL;

	cb.cb = discover_ping_resource_handler;
	cb.cd = NULL;
	cb.context = (void *)ping;

	if (ping == NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "ping struct is NULL.");
		return sleepTime;
	}

	THINGS_LOG_D(THINGS_DEBUG, TAG, "cb.context=0x%X", cb.context);

	if ((hostAddr = make_host_addr(ping->addr, ping->port)) == NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Memory Allocation is failed.");
		return sleepTime;
	}

	//while (cas_mask(ping, PING_ST_REQUEST | PING_ST_INTUPDATE | PING_ST_TIMEOUT, false, PING_ST_DISCOVERY) == false) {
	//	sleep(1);				// sleep 1 second
	//}

	if (get_mask(ping, PING_ST_TIMEOUT) == true) {
		THINGS_LOG_V(THINGS_ERROR, TAG, "Ping thread timeout");
		return sleepTime;
	}

	set_mask(ping, PING_ST_DISCOVERY);

	THINGS_LOG_D(THINGS_DEBUG, TAG, "Send OCFindKeepAliveResouce request to %s.", hostAddr);
	if (OCFindKeepAliveResource(&g_req_handle, hostAddr, &cb) == OC_STACK_OK) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Waiting for /oic/ping Cloud-response.(%s)", ping->addr);
		sleepTime = 5;
	} else {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "[Error]: Ping Resource Discovery-request is failed.");
		unset_mask(ping, PING_ST_DISCOVERY);
	}
	/*! Added by st_things for memory Leak fix
	 */
	if (hostAddr) {
		things_free(hostAddr);
	}
	THINGS_LOG_D(THINGS_DEBUG, TAG, "Exit.(sleepTime=%d)", sleepTime);
	return sleepTime;
}

static int send_things_ping_request(things_ping_s *ping)
{
	THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

	if (get_mask(ping, PING_ST_TCPCONNECT) == false) {
		return 1;
	}

	int sleepTime = 0;			// minute
	OCDoHandle g_req_handle = NULL;
	OCStackResult result = OC_STACK_ERROR;
	OCCallbackData cb;
	int64_t interval = 0;
	char *hostAddr = NULL;
	OCRepPayload *payload = NULL;

	cb.cb = handler_ping_request;
	cb.cd = NULL;
	cb.context = (void *)ping;

	if (ping == NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "ping struct is NULL.");
		return sleepTime;
	}

	THINGS_LOG_D(THINGS_DEBUG, TAG, "cb.context=0x%X", cb.context);

	if ((hostAddr = make_host_addr(ping->addr, ping->port)) == NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "hostAddr Memory Allocation is failed.");
		return sleepTime;
	}

	payload = OCRepPayloadCreate();
	if (!payload) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "payload Memory Allocation is failed.");
		/*! Added by st_things for memory Leak fix
		 */
		if (hostAddr) {
			things_free(hostAddr);
		}
		return sleepTime;
	}

	//while (cas_mask(ping, PING_ST_DISCOVERY | PING_ST_INTUPDATE | PING_ST_TIMEOUT, false, PING_ST_REQUEST) == false) {
	//	sleep(1);				// sleep 1 second
	//}

	if (get_mask(ping, PING_ST_TIMEOUT) == true) {
		THINGS_LOG_V(THINGS_ERROR, TAG, "Ping thread timeout");
		return sleepTime;
	}

	set_mask(ping, PING_ST_REQUEST);

	pthread_mutex_lock(&ping->mutex_int);
	interval = ping->interval[ping->int_seq];
	// period send request to Cloud for interval hand-shaking /oic/ping resource.
	OCRepPayloadSetPropInt(payload, INTERVAL, interval);
	THINGS_LOG_D(THINGS_DEBUG, TAG, "interval = %lld", interval);

	THINGS_LOG_D(THINGS_DEBUG, TAG, "Send OCSendKeepAliveRequest request to %s.", hostAddr);
	if ((result = OCSendKeepAliveRequest(&g_req_handle, hostAddr, payload, &cb)) == OC_STACK_OK)
	//if( (result= SendKeepAliveRequest(hostAddr, interval, &cb)) == OC_STACK_OK )
	{
		THINGS_LOG_D(THINGS_INFO, TAG, "\e[35mSending Success about common-Ping request.\e[m");
		do {
			ping->int_seq = (ping->int_seq + 1) % MAX_INTERVAL_EA;
		} while (ping->interval[ping->int_seq] == 0);

		sleepTime = interval;
	}
	unset_mask(ping, PING_ST_INTUPDATED);
	pthread_mutex_unlock(&ping->mutex_int);

	if (result != OC_STACK_OK) {
		THINGS_LOG_V(THINGS_ERROR, TAG, "\e[31m[Error]: Sending about common-Ping request is failed. Result : %d\e[m", result);
		unset_mask(ping, PING_ST_REQUEST);
		OCRepPayloadDestroy(payload);
	}
	/*! Added by st_things for memory Leak fix
	 */
	if (hostAddr) {
		things_free(hostAddr);
	}
	THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
	return sleepTime;
}

static bool set_interval_send_update(things_ping_s *ping, int64_t *intervals, int int_ea)
{
	THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

//    int i = 0;
	bool res = false;
	OCStackResult result = OC_STACK_ERROR;
	OCCallbackData cb;
	char *hostAddr = NULL;

	cb.cb = handler_ping_update_request;
	cb.cd = NULL;
	cb.context = (void *)ping;

	if (ping == NULL || intervals == NULL || int_ea == 0) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Invalid arguments.(ping=%0x%X, intervals=0x%X, int_ea=%d)", ping, intervals, int_ea);
		return false;
	}

	if (get_mask(ping, PING_ST_ISCLOUD) == false) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Interval Update & Sending update-Ping request is Support only for Cloud.");
		return false;
	}

	if (int_ea > MAX_INTERVAL_EA) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "int_ea is over %d. So, fix to %d", MAX_INTERVAL_EA, MAX_INTERVAL_EA);
		int_ea = MAX_INTERVAL_EA;
	}

	if (get_mask(ping, PING_ST_STARTTHREAD) == false || ping->continue_thread == false) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Ping-Thread is not ready.");
		return false;
	}

	if ((hostAddr = make_host_addr(ping->addr, ping->port)) == NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Memory Allocation is failed.");
		return false;
	}

	//while (cas_mask(ping, PING_ST_DISCOVERY | PING_ST_REQUEST | PING_ST_TIMEOUT, false, PING_ST_INTUPDATE) == false) {
	//	sleep(1);				// sleep 1 second
	//}

	if (get_mask(ping, PING_ST_TIMEOUT) == true) {
		THINGS_LOG_V(THINGS_ERROR, TAG, "Ping thread is time out");
		return false;
	}

	set_mask(ping, PING_ST_INTUPDATE);

//    pthread_mutex_lock( &ping->mutex_int );
	result = send_keep_alive_update_request(hostAddr, intervals, int_ea, &cb);
//    if( (result = send_keep_alive_update_request(host, intervals, int_ea, &cb)) == OC_STACK_OK )     // send interval-update request to Cloud. & set State.
//    {
//        memset(ping->interval, 0, sizeof(int64_t)*MAX_INTERVAL_EA);
//        for(i=0; i < int_ea; i++)
//        {
//            ping->interval[i] = intervals[i];
//        }
//        ping->int_seq = 0;
//        res = true;
//    }
//    pthread_mutex_unlock( &ping->mutex_int );

	if (result != OC_STACK_OK) {
		THINGS_LOG_ERROR(THINGS_ERROR, TAG, "[Error]: Sending about update-Ping request is failed.");
		unset_mask(ping, PING_ST_INTUPDATE);
	} else {
		THINGS_LOG(THINGS_DEBUG, TAG, "Success Sending update-Ping request.");
		res = true;
	}
	/*! Added by st_things for memory Leak fix
	 */
	if (hostAddr) {
		things_free(hostAddr);
	}

	THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
	return res;
}
#else
static int find_resource_oic_ping(things_ping_s *ping)
{
	return 1;
}

static int send_things_ping_request(things_ping_s *ping)
{
	return 1;
}

static bool set_interval_send_update(things_ping_s *ping, int64_t *intervals, int int_ea)
{
	return true;
}
#endif

int64_t find_minimum_from_intervals(int64_t *intervals, int int_ea)
{
	int64_t min = 32768;
	int i;

	for (i = 0; i < int_ea; i++) {
		if (intervals[i] < min) {
			min = intervals[i];
		}
	}

	return min;
}

static bool set_interval(things_ping_s *ping, int64_t *intervals, int int_ea, ping_state_e confirmState)
{
	THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

	int i = 0;
	bool res = false;
	int64_t min_interval = 0;

	if (ping == NULL || intervals == NULL || int_ea == 0) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Invalid arguments.(ping=%0x%X, intervals=0x%X, int_ea=%d)", ping, intervals, int_ea);
		return false;
	}

	if (int_ea > MAX_INTERVAL_EA) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "int_ea is over %d. So, fix to %d", MAX_INTERVAL_EA, MAX_INTERVAL_EA);
		int_ea = MAX_INTERVAL_EA;
	}

	if (confirmState == PING_ST_INIT) {
		confirmState = PING_ALL_FLAG;
	}

	pthread_mutex_lock(&ping->mutex_int);
	if (get_mask(ping, confirmState) == true && get_mask(ping, PING_ST_TIMEOUT) == false) {
		min_interval = find_minimum_from_intervals(intervals, int_ea);
		memset(ping->interval, 0, sizeof(int64_t) * MAX_INTERVAL_EA);
		for (i = 0; i < int_ea; i++) {
			THINGS_LOG_D(THINGS_DEBUG, TAG, "Setting interval[%d]=%d", i, min_interval);
			ping->interval[i] = min_interval;
		}
		ping->int_seq = 0;
		res = true;

		if (confirmState != PING_ALL_FLAG) {
			unset_mask(ping, confirmState);
		}

		if (confirmState == PING_ST_INTUPDATE) {
			set_mask(ping, PING_ST_INTUPDATED);
		}
	}
	pthread_mutex_unlock(&ping->mutex_int);

	THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
	return res;
}

static bool set_def_interval(things_ping_s *ping)
{
	if (ping == NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "ping is NULL.");
		return false;
	}

	pthread_mutex_lock(&ping->mutex_int);
	memset(ping->interval, 0, sizeof(int64_t) * MAX_INTERVAL_EA);
	ping->interval[0] = (int64_t) 1;
	ping->interval[1] = (int64_t) 2;
	ping->interval[2] = (int64_t) 4;
	ping->interval[3] = (int64_t) 8;
	ping->int_seq = 0;
	pthread_mutex_unlock(&ping->mutex_int);

	return true;
}

static bool things_ping_create_thread(things_ping_s *ping)
{
	THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

	bool res = false;

	if (ping == NULL || ping->addr == NULL || ping->port == 0) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid arguments.(ping=0x%X)", ping);
		if (ping) {
			THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid arguments.(addr=%s, port=%d)", ping->addr, ping->port);
		}
		return res;
	}

	pthread_mutex_lock(&ping->mutex_thread);
	if (ping->handle_thread == 0) {
		if (pthread_create_rtos(&ping->handle_thread, NULL, (pthread_func_type) thd_ping_loop, ping, THINGS_STACK_PING_THREAD) != 0) {
			THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Create thd_ping_loop thread is failed.");
			ping->handle_thread = 0;
			goto GOTO_OUT;
		}

		set_mask(ping, PING_ST_STARTTHREAD);
		res = true;
	}
GOTO_OUT:
	pthread_mutex_unlock(&ping->mutex_thread);

	THINGS_LOG_D(THINGS_DEBUG, TAG, "Exit.(result=%d)", res);
	return res;
}

static bool things_ping_destroy_thread(things_ping_s *ping)
{
	THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");
	bool res = false;

	if (ping == NULL || ping->addr == NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid arguments.(ping=0x%X)", ping);
		return res;
	}

	pthread_mutex_lock(&ping->mutex_thread);
	if (ping->handle_thread) {
		pthread_t handle = ping->handle_thread;
		ping->handle_thread = 0;
		ping->continue_thread = false;
#ifdef __ST_THINGS_RTOS__
		pthread_detach(handle);
		while (1) {
			if (get_mask(ping, PING_ST_STARTTHREAD) == true) {
				THINGS_LOG_V(THINGS_DEBUG, TAG, "Wait destroy ping thread 1 sec");
				sleep(1);
			} else {
				break;
			}
		}
#else							/* there is problem in artik during thread_cancel hence avoiding it */
		pthread_cancel(ping->handle_thread);
		pthread_detach(ping->handle_thread);
#endif
		ping->continue_thread = false;
		set_def_interval(ping);

		unset_mask(ping, PING_ST_STARTTHREAD | PING_ST_DISCOVERY | PING_ST_REQUEST | PING_ST_INTUPDATE | PING_ST_TIMEOUT);
		res = true;
	}
	pthread_mutex_unlock(&ping->mutex_thread);

	THINGS_LOG_D(THINGS_DEBUG, TAG, "Exit.(result=%d)", res);
	return res;
}

static void things_rep_payload_value_print(OCRepPayloadValue *value)
{
	switch (value->type) {
	case OCREP_PROP_INT:
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Key: %s, Value: %lld, int", value->name, value->i);
		break;
	case OCREP_PROP_DOUBLE:
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Key: %s, Value: %f, double", value->name, value->d);
		break;
	case OCREP_PROP_STRING:
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Key: %s, Value: %s, string", value->name, value->str);
		break;
	case OCREP_PROP_BOOL:
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Key: %s, Value: %d, bool", value->name, value->b);
		break;
	case OCREP_PROP_BYTE_STRING:
		THINGS_LOG_V(THINGS_DEBUG, TAG, "Key: %s(binary) size : %d", value->name, value->ocByteStr.len);
		//OCPrintByteValue( value->ocByteStr.bytes, value->ocByteStr.len );
		break;
	case OCREP_PROP_OBJECT:
		// Note: Only prints the URI (if available), to print further, you'll
		// need to dig into the object better!
		THINGS_LOG_V(THINGS_DEBUG, TAG, "Key: %s(OCRep):%s", value->name, value->obj->uri);
		break;
	case OCREP_PROP_ARRAY:
		switch (value->arr.type) {
		case OCREP_PROP_INT:
			THINGS_LOG_V(THINGS_DEBUG, TAG, "Key: %s(int array):%zu x %zu x %zu", value->name, value->arr.dimensions[0], value->arr.dimensions[1], value->arr.dimensions[2]);
			for (size_t iter = 0; iter < value->arr.dimensions[0]; iter++) {
				THINGS_LOG_V(THINGS_DEBUG, TAG, "Key: \t(int):%d", value->arr.iArray[iter]);
			}
			break;
		case OCREP_PROP_DOUBLE:
			THINGS_LOG_V(THINGS_DEBUG, TAG, "Key: %s(double array):%zu x %zu x %zu", value->name, value->arr.dimensions[0], value->arr.dimensions[1], value->arr.dimensions[2]);
			for (size_t iter = 0; iter < value->arr.dimensions[0]; iter++) {
				THINGS_LOG_V(THINGS_DEBUG, TAG, "Key: \t(double):%f", value->arr.dArray[iter]);
			}
			break;
		case OCREP_PROP_BOOL:
			THINGS_LOG_V(THINGS_DEBUG, TAG, "Key: %s(bool array):%zu x %zu x %zu", value->name, value->arr.dimensions[0], value->arr.dimensions[1], value->arr.dimensions[2]);
			for (size_t iter = 0; iter < value->arr.dimensions[0]; iter++) {
				THINGS_LOG_V(THINGS_DEBUG, TAG, "Key: \t(bool):%s", value->arr.bArray[iter] ? "true" : "false");
			}
			break;
		case OCREP_PROP_STRING:
			THINGS_LOG_V(THINGS_DEBUG, TAG, "Key: %s(string array):%zu x %zu x %zu", value->name, value->arr.dimensions[0], value->arr.dimensions[1], value->arr.dimensions[2]);
			for (size_t iter = 0; iter < value->arr.dimensions[0]; iter++) {
				THINGS_LOG_V(THINGS_DEBUG, TAG, "Key: \t(string):%s", value->arr.strArray[iter]);
			}
			break;
		case OCREP_PROP_BYTE_STRING:
			THINGS_LOG_V(THINGS_DEBUG, TAG, "Key: %s(byte array):%zu x %zu x %zu", value->name, value->arr.dimensions[0], value->arr.dimensions[1], value->arr.dimensions[2]);
			break;
		default:
			THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Key: %s <-- Unknown/unsupported array type!", value->name);
			break;
		}
		break;
	default:
		THINGS_LOG_D(THINGS_DEBUG, TAG, "Un supported val Type.(0x%d)", value->type);
		break;
	}
}

static char *make_host_addr(const char *ip, const uint16_t port)
{
	THINGS_LOG(THINGS_DEBUG, TAG, "Enter.");

	char *host = NULL;
	int length = 0;

	THINGS_LOG_D(THINGS_DEBUG, TAG, "ip=%s, port=%d", ip, port);
	if (ip == NULL || strlen(ip) == 0 || port == 0) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Invalid arguments.");
		return NULL;
	}

	length = strlen(ip) + 7 + strlen(DEFAULT_COAP_TCP_HOST);
	if ((host = (char *)things_malloc(sizeof(char) * length)) == NULL) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "memory allocation is failed.");
		return NULL;
	}
	// Update Cloud Address.
	memset(host, 0, length);
	if (snprintf(host, length, "%s%s:%d", DEFAULT_COAP_TCP_HOST, ip, port) >= length) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "[Error] host buffer is overflowed.");
		things_free(host);
		host = NULL;
	}

	THINGS_LOG_D(THINGS_DEBUG, TAG, "host = %s", host);

	THINGS_LOG(THINGS_DEBUG, TAG, "Exit.");
	return host;
}

static OCStackResult send_keep_alive_update_request(const char *host, int64_t *intervals, int int_ea, OCCallbackData *cb)
{
	OCDoHandle g_req_handle = NULL;
	OCStackResult result = OC_STACK_ERROR;
	char targetUri[MAX_URI_LENGTH * 2] = { 0, };
	OCRepPayload *payload = NULL;
	size_t dimensions[MAX_REP_ARRAY_DEPTH] = { 0, 0, 0 };

	if (host == NULL || strlen(host) == 0 || intervals == NULL || int_ea == 0) {
		THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "Invalid event_data.(host=%s, intervals=0x%X, int_ea=%d)", host, intervals, int_ea);
		result = OC_STACK_NO_MEMORY;
		goto no_memory;
	}

	snprintf(targetUri, MAX_URI_LENGTH * 2, "%s%s", host, KEEPALIVE_RESOURCE_URI);
	THINGS_LOG_D(THINGS_DEBUG, TAG, "oicping Update Request to : %s", targetUri);

	payload = OCRepPayloadCreate();
	if (!payload) {
		result = OC_STACK_NO_MEMORY;
		goto no_memory;
	}

	THINGS_LOG_D(THINGS_DEBUG, TAG, "int_ea=%d", int_ea);
	for (int i = 0; i < int_ea; i++) {
		THINGS_LOG_D(THINGS_DEBUG, TAG, "intervals[%d]=%d", i, intervals[i]);
	}

	dimensions[0] = (size_t) int_ea;
	OCRepPayloadSetIntArray(payload, INTERVAL_ARRAY, intervals, dimensions);
	THINGS_LOG_D(THINGS_DEBUG, TAG, "key     : %s", INTERVAL_ARRAY);

//#if defined (__WITH_DTLS__) || defined(__WITH_TLS__)
//    CASelectCipherSuite(0x35, (1 << 4));
//#endif

	THINGS_LOG_D(THINGS_DEBUG, TAG, "Send OCSendKeepAliveRequest request to %s.", host);
	result = OCSendKeepAliveRequest(&g_req_handle, host, payload, cb);
//    result = OCDoResource(&g_req_handle, OC_REST_POST, targetUri,
//                          NULL, (OCPayload *) payload, CT_ADAPTER_TCP,
//                          OC_HIGH_QOS, cb, NULL, 0);

	if (result != OC_STACK_OK) {
		THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "oicping Update Request Sending is failed.(URI: %s)", targetUri);
		OCRepPayloadDestroy(payload);
	}

no_memory:
	return result;
}
