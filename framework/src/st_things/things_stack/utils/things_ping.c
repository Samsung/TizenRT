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

#define THINGS_PING_ENABLE      1

#include <sys/types.h>
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

#ifdef THINGS_PING_ENABLE
#include "oickeepalive.h"
#endif
#include "easy-setup/resource_handler.h"
#include "cloud/cloud_manager.h"

// Logging tag for module name.
#define TAG "[things_ping]"

#define DEFAULT_CONTEXT_VALUE   0x99
#define PING_UNIT           60	// 60 seconds is minimum value of ping.
#define NET_TROUBLE_DELAY   5	// 5 seconds
#define MAX_INTERVAL_EA     4	// Cloud Support 4 EA Interval class.
#define MAX_RETRY_CNT       10

#define KEEPALIVE_RESOURCE_URI "/oic/ping"

typedef void *(*pthread_func_type)(void *);

/**
 * Holds ping thread information
 */
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


pthread_mutex_t mutex_ping_list;

// Start with empty queue.
static list_s *list = NULL;
static const char INTERVAL_ARRAY[] = "inarray";
static const char INTERVAL[] = "in";
// Start with empty things_ping_s.
things_ping_s* p_ping = NULL;

//--------------------------------------------------------------------------------
/**
 * @brief Static type prototype for creating thd_ping_loop thread.
 * @param ping : Structure of things_ping_s
 */
static void *thd_ping_loop(things_ping_s *ping);

//--------------------------------------------------------------------------------
/**
 * @brief Static type prototype for handle ping resource discovery.
 * @param ctx : context value
 * @param handle : Handle to an OCDoResource invocation.
 * @param client_response : response from peer.
 * @return OCStackApplicationResult : application result.
 */
static OCStackApplicationResult discover_ping_resource_handler(void *ctx, OCDoHandle handle, OCClientResponse *client_response);

//--------------------------------------------------------------------------------
/**
 * @brief Static type prototype for handle ping request.
 * @param ctx : context value
 * @param handle : Handle to an OCDoResource invocation.
 * @param client_response : response from peer.
 * @return OCStackApplicationResult : application result.
 */
static OCStackApplicationResult handler_ping_request(void *ctx, OCDoHandle handle, OCClientResponse *client_response);

//--------------------------------------------------------------------------------
/**
 * @brief Static type prototype for creating things_ping_s.
 * @param remote_addr : Remote address
 * @param port : port number
 * @return structure of things_ping_s
 */
static things_ping_s *create_things_ping_s(const char *remote_addr, const uint16_t port);

//--------------------------------------------------------------------------------
/**
 * @brief State type prototype for terminate things_ping_s.
 * @param ping : Structure of things_ping_s
 */
static void terminate_things_ping_s(things_ping_s *ping);

//--------------------------------------------------------------------------------
/**
 * @brief Static type prototype for matching ip.
 * @param ping : Structure of things_ping_s
 * @param target_ip : IP of target device.
 * @return success = 1, otherwise 0.
 */
static int is_ip_key_equal(things_ping_s *ping, const char *target_ip);

//--------------------------------------------------------------------------------
/**
 * @brief Static type prototype for checking ping thread. Creating ping thread or
 * destroy ping thread like that.
 * @param ping : Structure of things_ping_s
 */
static void check_ping_thread(things_ping_s *ping);

//--------------------------------------------------------------------------------
/**
 * @biref Static type prototype for set the state of Ping of things_ping_s.
 * @param ping : Structure of things_ping_s
 * @param state : State of Ping of things_ping_s
 */
static void set_mask(things_ping_s *ping, ping_state_e state);

//--------------------------------------------------------------------------------
/**
 * @biref Static type prototype for get the state of Ping of things_ping_s.
 * @param ping : Structure of things_ping_s
 * @param state : State of Ping of things_ping_s
 * @return success = true, failure = false.
 */
static bool get_mask(things_ping_s *ping, ping_state_e state);

//--------------------------------------------------------------------------------
/**
 * @biref Static type prototype for set the state of Ping of things_ping_s.
 * @param ping : Structure of things_ping_s
 * @param preState : State of Ping of things_ping_s
 * @param preStateThen : bool value
 * @param post_state : State of Ping of things_ping_s
 * @return success = true, failure = false.
 */
static bool cas_mask(things_ping_s *ping, ping_state_e preState, bool preStateThen, ping_state_e post_state);

//--------------------------------------------------------------------------------
/**
 * @biref Static type prototype for unset the state of Ping of things_ping_s.
 * @param ping : Structure of things_ping_s
 * @param state : State of Ping of things_ping_s
 */
static void unset_mask(things_ping_s *ping, ping_state_e state);

//--------------------------------------------------------------------------------
/**
 * @brief Static type prototype for find ping resource to cloud.
 * @param ping : Structure of things_ping_s
 * @return 1 = TCP_session not connected, error case return sleeptime.
 */
static int find_resource_oic_ping(things_ping_s *ping);

//--------------------------------------------------------------------------------
/**
 * @brief Static type prototype for sending common-Ping request for /oic/ping to Cloud.
 * @parma ping : Structure of things_ping_s
 * @return success = sleeptime, failure = 0.
 */
static int send_things_ping_request(things_ping_s *ping);

//--------------------------------------------------------------------------------
/**
 * @brief Static type prototype for setting interval value in things_ping_s interval array.
 * @param ping : Structure of things_ping_s
 * @param intervals : Array of intervals
 * @param int_ea : Estmated interval value
 * @param confirm_state : State of ping
 * @return success = true, failure = false
 */
static bool set_interval(things_ping_s *ping, int64_t *intervals, int int_ea, ping_state_e confirm_state);

//--------------------------------------------------------------------------------
/**
 * @brief Static prototype for defined the interval value for things_ping_s ping interval array.
 * @param ping : Structure of things_ping_s
 * @return success = true, failure = false
 */
static bool set_def_interval(things_ping_s *ping);

//--------------------------------------------------------------------------------
/**
 * @brief Static prototype for creating ping thread.
 * @param ping : Structure of things_ping_s
 * @return success = true, failure = false
 */
static bool things_ping_create_thread(things_ping_s *ping);

//--------------------------------------------------------------------------------
/**
 * @brief Static prototype for destroy ping thread.
 * @param ping : Structure of things_ping_set_maskr
 * @return success = true, failure = false
 */
static bool things_ping_destroy_thread(things_ping_s *ping);

//--------------------------------------------------------------------------------
/**
 * @brief Static prototype for printing response value of payload.
 * @param value : Structure of OC response payload
 * @return success = true, failure = false
 */
static void things_rep_payload_value_print(OCRepPayloadValue *value);

//--------------------------------------------------------------------------------
/**
 * @brief Static prototype for making host address.
 * @param ip : IP address
 * @param port : Port number
 * @return : string value, failure return NULL value
 */
static char *make_host_addr(const char *ip, const uint16_t port);

//-------------------------------------------------------------------------------
/* Enable things Ping. */
#ifdef THINGS_PING_ENABLE

/**
 * Function definition for initialize OIC device keepAlive for ping resource.
 */
bool things_ping_init(void)
{
	THINGS_LOG_D(TAG, "Enter.");

	bool res = false;

	// Create a pthread mutex.
	pthread_mutex_init(&mutex_ping_list, NULL);

	// Lock the mutex.
	pthread_mutex_lock(&mutex_ping_list);

	if (list == NULL) {
		// Create and initiate a Queue.
		if ((list = create_list()) == NULL) {
			THINGS_LOG_E(TAG, "memory allocation is failed.");
			pthread_mutex_unlock(&mutex_ping_list);
			return res;
		}

		res = true;
	} else {
		res = true;
	}
	// Unlock the mutex.
	pthread_mutex_unlock(&mutex_ping_list);

	p_ping = NULL;
	THINGS_LOG_D(TAG, "Exit.");
	return res;
}

//-------------------------------------------------------------------------------
/**
 * Function definition for terminate OIC device keepAlive for ping resource.
 */
void things_ping_terminate(void)
{
	THINGS_LOG_D(TAG, "Enter.");

	// Lock the mutex.
	pthread_mutex_lock(&mutex_ping_list);
	if (list) {
		THINGS_LOG_D(TAG, "things_ping_s terminate.");
		things_ping_s *ping = NULL;
		while ((ping = (things_ping_s *) list->pop(list)) != NULL) {
			THINGS_LOG_D(TAG, "Terminate ping.(%s)", ping->addr);
			if (cas_mask(ping, PING_ST_STARTTHREAD, true, PING_ST_INIT) == true) {
				// Destroy ping thread.
				things_ping_destroy_thread(ping);
			}
			// Terminate the ping.
			terminate_things_ping_s(ping);
		}

		THINGS_LOG_D(TAG, "Terminate ping-list.");
		// Terminate the queue.
		terminate_list(list);
		list = NULL;
	}

	// Unlock the mutex.
	pthread_mutex_unlock(&mutex_ping_list);
	pthread_mutex_destroy(&mutex_ping_list);

	THINGS_LOG_D(TAG, "Exit.");
}

//----------------------------------------------------------------------------------
/**
 * Function definition for setting mask for things_ping.
 */
bool things_ping_set_mask(const char *remote_addr, uint16_t port, ping_state_e state)
{
	THINGS_LOG_D(TAG, "Enter.");

	// Make a empty list.
	things_node_s *node = NULL;
	// Initialize things_ping structure with NULL.
	things_ping_s *ping = NULL;

	// Check the validation of remote address.
	if (remote_addr == NULL || strlen(remote_addr) == 0) {
		THINGS_LOG_E(TAG, "Invalid arguments.(remote_addr=%s, state=%d)", remote_addr, state);
		return false;
	}

	// Lock the mutex.
	pthread_mutex_lock(&mutex_ping_list);
	if (list == NULL) {
		THINGS_LOG_V(TAG, "OICPing Module is not initialized.");
		pthread_mutex_unlock(&mutex_ping_list);
		return false;
	}

	// Find the things_node for remote address in Queue?
	node = list->find_by_key(list, (key_compare) is_ip_key_equal, remote_addr);
	// If things node for remote address is not persent in Queue.
	if (node == NULL) {
		THINGS_LOG_D(TAG, "Not Found things_node_s for remote(%s). So, Create Node.", remote_addr);
		// Create things_ping.
		if ((ping = create_things_ping_s(remote_addr, port)) == NULL) {
			THINGS_LOG_E(TAG, "memory allocation is failed.");
			pthread_mutex_unlock(&mutex_ping_list);
			return false;
		}
		// Insert the things node in Queue.
		list->insert(list, (void *)ping);
	} else {
		// Found the things_node for remote address in Queue.
		THINGS_LOG_D(TAG, "Found things_node_s for remote(%s).", remote_addr);
		// Set the item of things node in things_ping_s.
		ping = node->item;
	}
	// Unlock the mutex.
	pthread_mutex_unlock(&mutex_ping_list);

	if (ping == NULL) {
		THINGS_LOG_E(TAG, "Not exist things_ping_s in remote_addr_Node.(%s)", remote_addr);
		return false;
	}

	// Set the port number in things_ping_s.
	if (port != 0) {
		ping->port = port;
	}

	// Set the bit mask state.
	set_mask(ping, state);

	/**
	 * check the ping thread.Based on the bit mask.It will create the ping thread,
	 * destroy the ping thread and terminate the things_ping_s.
	 */
	check_ping_thread(ping);

	THINGS_LOG_D(TAG, "Exit.");
	return true;
}

//-------------------------------------------------------------------------------
/**
 * Function definition for unset mask for things_ping.
 */
bool things_ping_unset_mask(const char *remote_addr, ping_state_e state)
{
	THINGS_LOG_D(TAG, "Enter.");
	// Make a empty list.
	things_node_s *node = NULL;
	// Initialize things_ping structure with NULL.
	things_ping_s *ping = NULL;

	// Check the validation of remote address.
	if (remote_addr == NULL || strlen(remote_addr) == 0) {
		THINGS_LOG_E(TAG, "Invalid arguments.(remote_addr=%s, state=%d)", remote_addr, state);
		return false;
	}

	// Lock the mutex.
	pthread_mutex_lock(&mutex_ping_list);
	if (list == NULL) {
		THINGS_LOG_V(TAG, "OICPing Module is not initialized.");
		pthread_mutex_unlock(&mutex_ping_list);
		return false;
	}

	// Find the things_node for remote address in Queue?
	node = list->find_by_key(list, (key_compare) is_ip_key_equal, remote_addr);

	// If things node for remote address is not persent in Queue.
	if (node == NULL) {
		THINGS_LOG_D(TAG, "Not Found things_node_s for remote(%s).", remote_addr);
		pthread_mutex_unlock(&mutex_ping_list);
		return false;
	} else {
		// Found the things_node for remote address in Queue.
		THINGS_LOG_D(TAG, "Found things_node_s for remote(%s).", remote_addr);
		// Set the item of things node in things_ping_s.
		ping = node->item;
	}
	// Unlock the mutex.
	pthread_mutex_unlock(&mutex_ping_list);

	if (ping == NULL) {
		THINGS_LOG_E(TAG, "Not exist things_ping_s in remote_addr_Node.(%s)", remote_addr);
		return false;
	}

	// Unset the bit mask state.
	unset_mask(ping, state);

	/**
	 * Check the ping thread.Based on the bit mask.It will create the ping thread,
	 * destroy the ping thread and terminate the things_ping_s.
	 */
	check_ping_thread(ping);
	THINGS_LOG_D(TAG, "Exit.");
	return true;
}

//-------------------------------------------------------------------------------
/* Disable things ping. */
#else

//-------------------------------------------------------------------------------
/**
 * Function definition for initialize OIC device keepAlive for ping resource.
 */
bool things_ping_init(void)
{
	return true;
}

//-------------------------------------------------------------------------------
/**
 * Function definition for terminate OIC device keepAlive for ping resource.
 */
void things_ping_terminate(void)
{
}

//----------------------------------------------------------------------------------
/**
 * Function definition for setting mask for things_ping.
 */
bool things_ping_set_mask(const char *remote_addr, uint16_t port, ping_state_e state)
{
	return true;
}

//-------------------------------------------------------------------------------
/**
 * Function definition for unset mask for things_ping.
 */
bool things_ping_unset_mask(const char *remote_addr, ping_state_e state)
{
	return true;
}
#endif

//-------------------------------------------------------------------------------
/**
 * Interval Ping-Thread Definition Function.
 *
 */
static void *__attribute__((optimize("O0"))) thd_ping_loop(things_ping_s *ping)
{
	THINGS_LOG_D(TAG, "Enter.");

	int sleepMinute = 0;

	// Check validation of things ping and ping's address.
	if (ping == NULL || ping->addr == NULL) {
		THINGS_LOG_E(TAG, "[Error]: Invalid arguments.(pint=0x%X)", ping);
		return NULL;
	}

	THINGS_LOG_D(TAG, "IP=%s, handle=0x%X, State=0x%X", ping->addr, ping->handle_thread, ping->bit_mask_state);

	// Lock the mutex.
	pthread_mutex_lock(&ping->mutex_int);
	for (int i = 0; i < MAX_INTERVAL_EA; i++) {
		THINGS_LOG_D(TAG, "Interval[%d]=%lld", i, ping->interval[i]);
	}

	THINGS_LOG_D(TAG, "Intseq= %d", ping->int_seq);
	THINGS_LOG_D(TAG, "ContinuThread= %d", ping->continue_thread);

	if (ping->interval[0] == (int64_t) 0 || ping->continue_thread == true) {
		THINGS_LOG_E(TAG, "[Error]: Invalid Interval-Time or Already Run-Thread for this ping.(interval[0]=%lld, continueFlag=%d)", ping->interval[0], ping->continue_thread);
		pthread_mutex_unlock(&ping->mutex_int);
		return NULL;
	}
	// Unlock the mutex.
	pthread_mutex_unlock(&ping->mutex_int);

	// Start to find resource for ping resource uri "/oic/ping" to cloud.
	THINGS_LOG_D(TAG, "Start FindResource for /oic/ping to Cloud(%s)", ping->addr);
	// Set the retry count value is 10.
	int retry_cnt = MAX_RETRY_CNT;
	do {						// /oic/ping Resource Finding Start
		/* /oic/ping resource discovery into cloud.If it is success.Then set the
		 * sleepTime is 5. */
		int sleepTime = find_resource_oic_ping(ping);
		sleep(sleepTime);
	} while (!ping->continue_thread && ping->handle_thread && (retry_cnt--) > 0);

	p_ping = ping;
	int sleepDelay = 10;

	THINGS_LOG_D(TAG, "Start common-Ping request for /oic/ping to Cloud(%s)", ping->addr);
	while (ping->continue_thread) {	// common-ping request Sending Start

		if (sleepMinute == 0) {
			// Sending common-Ping request for /oic/ping to Cloud.
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
	THINGS_LOG_V(TAG, "\e[35mSTOP PING THREAD / TARGET : %s\e[m", ping->addr);
	THINGS_LOG_D(TAG, "Exit.");
	return (void *)1;
}

//-------------------------------------------------------------------------------
/**
 * Callback definition for handle ping resource discovery.
 */
static OCStackApplicationResult discover_ping_resource_handler(void *ctx, OCDoHandle handle, OCClientResponse *client_response)
{
	THINGS_LOG_D(TAG, "Enter.");

	int64_t *interval = NULL;
	things_ping_s *ping = (things_ping_s *) ctx;

	if (ctx == (void *)DEFAULT_CONTEXT_VALUE || ctx == NULL || ping->addr == NULL) {
		THINGS_LOG_D(TAG, "content value is invalid.(ctx=0x%X)", ctx);
		return OC_STACK_DELETE_TRANSACTION;
	}

	if (client_response) {
		size_t dimensions[MAX_REP_ARRAY_DEPTH] = { 0, 0, 0 };
		THINGS_LOG_D(TAG, "ctx=0x%X", ctx);
		THINGS_LOG_D(TAG, "StackResult: %s(%d)", get_result(client_response->result), client_response->result);

		THINGS_LOG_D(TAG, "Device =============> Discovered @ %s:%d", client_response->devAddr.addr, client_response->devAddr.port);

		if (strncmp(ping->addr, client_response->devAddr.addr, strlen(client_response->devAddr.addr)) != 0) {
			THINGS_LOG_E(TAG, "[Error]: Mismatching address.(ping->addr=%s, recvAddr=%s)", ping->addr, client_response->devAddr.addr);
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
			THINGS_LOG_D(TAG, "dimension=[%d, %d, %d]", dimensions[0], dimensions[1], dimensions[2]);
			for (int i = 0; i < dimensions[0]; i++) {
				THINGS_LOG_D(TAG, "interval[%d]=%d", i, interval[i]);
			}
		}
		// Result processing.
		switch (client_response->result) {
		case OC_STACK_OK:			// Discovery Success.
			THINGS_LOG_D(TAG, "Discovery Success.");
			if (interval == NULL || interval[0] == 0 || dimensions[0] == 0) {
				THINGS_LOG_V(TAG, "Invalid received remote interval or dimensions.");
				return OC_STACK_DELETE_TRANSACTION;
			}

			if (set_interval(ping, interval, dimensions[0], PING_ST_DISCOVERY) == true) {
				ping->continue_thread = true;
			}
			break;
		case OC_STACK_TIMEOUT:			// Ping resource Time-Out.
			THINGS_LOG_E(TAG, "Occurred Time-Out Event.");
			unset_mask(ping, PING_ST_SIGNIN|PING_ST_TCPCONNECT);
			check_ping_thread(ping);
			// Disconnect the TCP session.
			CAUtilTCPDisconnectSession(ping->addr, ping->port, 64);
			THINGS_LOG_E(TAG, "DISCOVER PING RESOURCE TIME OUT, CLOUD DISCONNECT");
			break;
		default:
			THINGS_LOG_D(TAG, "Not Support StackResult: %s(%d)", get_result(client_response->result), client_response->result);
			break;
		}
	} else {
		THINGS_LOG_V(TAG, "discover_ping_resource_handler received Null client_response");
	}
	/*! Added by st_things for memory Leak fix
	 */
	if (NULL != interval) {
		things_free(interval);
	}

	THINGS_LOG_D(TAG, "Exit.");
	return OC_STACK_DELETE_TRANSACTION;
}

//-------------------------------------------------------------------------------
/**
 * Callback definition for handle ping request.
 */
static OCStackApplicationResult handler_ping_request(void *ctx, OCDoHandle handle, OCClientResponse *client_response)
{
	THINGS_LOG_D(TAG, "Enter.");

	things_ping_s *ping = (things_ping_s *) ctx;

	// Check the validation of content value.
	if (ctx == (void *) DEFAULT_CONTEXT_VALUE || ctx == NULL || ping->addr == NULL) {
		THINGS_LOG_D(TAG, "content value is invalid.(ctx=0x%X)", ctx);
		return OC_STACK_DELETE_TRANSACTION;
	}

	// Handle the response from client side of server's query.
	if (client_response) {
		THINGS_LOG_D(TAG, "ctx=0x%X", ctx);
		THINGS_LOG_D(TAG, "StackResult: %s(%d)", get_result(client_response->result), client_response->result);

		THINGS_LOG_D(TAG, "Device =============> Discovered @ %s:%d", client_response->devAddr.addr, client_response->devAddr.port);

		// Compare the ping's address and client response device address.
		if (strncmp(ping->addr, client_response->devAddr.addr, strlen(client_response->devAddr.addr)) != 0) {
			THINGS_LOG_E(TAG, "[Error]: Mismatching address.(ping->addr=%s, recvAddr=%s)", ping->addr, client_response->devAddr.addr);
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
		case OC_STACK_OK:			// Common-ping request Success
			THINGS_LOG_V(TAG, "common-ping request Success.");

			/**
			 * Get the status of ping.If time-out occured. Then unset the mask value of ping
			 * for common-ping request to cloud.
			 */
			if (get_mask(ping, PING_ST_REQUEST) == true && get_mask(ping, PING_ST_TIMEOUT) == false) {
				unset_mask(ping, PING_ST_REQUEST);
			}
			break;
		case OC_STACK_TIMEOUT:			// Ping resource Time-Out.
			THINGS_LOG_E(TAG, "Occurred Time-Out Event.");
			unset_mask(ping, PING_ST_SIGNIN|PING_ST_TCPCONNECT);
			check_ping_thread(ping);
			// Disconnect the TCP session.
			CAUtilTCPDisconnectSession(ping->addr, ping->port, 64);
			THINGS_LOG_E(TAG, "SEND PING TIME OUT, CLOUD DISCONNECT");
			break;
		default:
			THINGS_LOG_E(TAG, "Not Support StackResult: %s(%d)", get_result(client_response->result), client_response->result);
			break;
		}
	} else {
		THINGS_LOG_W(TAG, "handler_ping_request received Null client_response");
	}

	THINGS_LOG_D(TAG, "Exit.");
	return OC_STACK_DELETE_TRANSACTION;
}

//--------------------------------------------------------------------------------
/**
 * Interval Function Definition Collection.
 */
static things_ping_s *create_things_ping_s(const char *remote_addr, const uint16_t port)
{
	// Initialize things_ping structure with NULL.
	things_ping_s *ping = NULL;

	// Check the validation of remote address.
	if (remote_addr == NULL || strlen(remote_addr) == 0) {
		THINGS_LOG_E(TAG, "remote addr is NULL.");
		return NULL;
	}

	// Allocate dynamic memory for ping.
	ping = (things_ping_s *) things_malloc(sizeof(things_ping_s));
	if (ping == NULL) {
		THINGS_LOG_E(TAG, "memory allocation is failed.");
		return NULL;
	}

	// Set the remote address in OICping.
	ping->addr = things_strdup(remote_addr);
	// Set the port number in OICPing.
	ping->port = port;
	ping->continue_thread = false;
	// Create mutex for interval varaible.
	pthread_mutex_init(&ping->mutex_int, NULL);
	// Create mutex for state variable.
	pthread_mutex_init(&ping->mutex_state, NULL);
	// Create mutex for handle thread variable.
	pthread_mutex_init(&ping->mutex_thread, NULL);

	// Lock the mutex thread variable.
	pthread_mutex_lock(&ping->mutex_thread);
	// Initialize ping thread-handler with 0.
	ping->handle_thread = 0;
	// Unlock the mutex thread variable.
	pthread_mutex_unlock(&ping->mutex_thread);

	set_def_interval(ping);

	// Set the mask for Ping as Ping initialization.
	set_mask(ping, PING_ST_INIT);
	return ping;
}

//--------------------------------------------------------------------------------
/**
 * Function definition for terminate things_ping_s.
 */
static void terminate_things_ping_s(things_ping_s *ping)
{
	THINGS_LOG_D(TAG, "Enter.");

	if (ping == NULL) {
		THINGS_LOG_D(TAG, "ping is NULL.");
		return;
	}

	THINGS_LOG_D(TAG, "ping thread destroy check.");
	/**
	 * Check the ping thread destory.If TCP connection is disabled and Ping thread created.
	 * Then need to destory the ping-thread.
	 */
	if (get_mask(ping, PING_ST_TCPCONNECT) == false && get_mask(ping, PING_ST_STARTTHREAD) == true) {	// need to destroy ping-thread.
		things_ping_destroy_thread(ping);
	}

	// Set the mask for Ping as Ping initialization
	set_mask(ping, PING_ST_INIT);

	// Destory the mutex of interval variable.
	pthread_mutex_destroy(&ping->mutex_int);
	// Destory the mutex of state variable.
	pthread_mutex_destroy(&ping->mutex_state);
	// Destory the mutex of thread variable.
	pthread_mutex_destroy(&ping->mutex_thread);

	// Free the memory of pings's address.
	if (ping->addr) {
		THINGS_LOG_D(TAG, "ping->addr memory free.");
		things_free(ping->addr);
		ping->addr = NULL;
	}

	// Free the memory of things_ping_s.
	things_free(ping);

	THINGS_LOG_D(TAG, "Exit.");
}

//--------------------------------------------------------------------------------
/**
 * Function definition for matching IP
 */
static int is_ip_key_equal(things_ping_s *ping, const char *targetIP)
{
	int res = 0;
	// Check validation of ping,ping's address and target IP.
	if (ping == NULL || ping->addr == NULL || targetIP == NULL || strlen(targetIP) == 0) {
		THINGS_LOG_E(TAG, "Invalid arguments.(ping=0x%X, targetIP=%s)", ping, targetIP);
		return res;
	}

	// Compare the ping's IP address and target IP address.
	if (strncmp(ping->addr, targetIP, strlen(targetIP)) == 0) {
		THINGS_LOG_D(TAG, "Found ping-struct(%s) for targetIP(%s)", ping->addr, targetIP);
		res = 1;
	}

	return res;
}

//--------------------------------------------------------------------------------
/**
 *  Function defnition for checking ping thread.
 */
static void check_ping_thread(things_ping_s *ping)
{
	THINGS_LOG_D(TAG, "Enter.");

	// Check validation of Ping and Ping's address.
	if (ping == NULL || ping->addr == NULL) {
		THINGS_LOG_E(TAG, "Invalid arguments.(ping=0x%X)", ping);
		return;
	}

	pthread_mutex_lock(&mutex_ping_list);
	if (list == NULL) {
		THINGS_LOG_V(TAG, "OICPing Module is not initialized.");
		pthread_mutex_unlock(&mutex_ping_list);
		return;
	}

	/**
	 * Get the state of Ping.If TCP session is connected and Ping thread is not created.
	 * Then need to create the ping thread.
	 */
	if (get_mask(ping, PING_ST_TCPCONNECT) == true && get_mask(ping, PING_ST_STARTTHREAD) == false) {	// need to create ping-thread.

		/**
		 * If the remote client is not cloud CIserver or remote client is cloud CIserver
		 * and Sign_In is complete.Then create ping thread.
		 */
		if (get_mask(ping, PING_ST_ISCLOUD) == false || (get_mask(ping, PING_ST_ISCLOUD) == true && get_mask(ping, PING_ST_SIGNIN) == true)) {	// create ping thread
			things_ping_create_thread(ping);
		}
	}

	/**
	 * Get the state of Ping.If the TCP session is disabled and ping-thread is created.
	 * Than need to destory ping-thread.
	 */
	if (get_mask(ping, PING_ST_TCPCONNECT) == false && get_mask(ping, PING_ST_STARTTHREAD) == true) {	// need to destroy ping-thread.
		things_ping_destroy_thread(ping);
	}

	/**
	 * Get the state of Ping.If the bitwise or value of remote client is cloud CIserver,Sign-IN complete and TCP session connected is not
	 * matched with the state of Ping.Then no need of things_ping_s. and terminate it.
	 */
	if (get_mask(ping, PING_ST_ISCLOUD | PING_ST_SIGNIN | PING_ST_TCPCONNECT) == false) {
		THINGS_LOG_D(TAG, "Not need things_ping_s, So, Terminate things_ping_s.(IP=%s)", ping->addr);
		if ((ping = list->erase_by_key(list, (key_compare)is_ip_key_equal, ping->addr)) != NULL) {
			THINGS_LOG_D(TAG, "Erase node-element Success in List.");
			terminate_things_ping_s(ping);
		}
	}

	pthread_mutex_unlock(&mutex_ping_list);

	THINGS_LOG_D(TAG, "Exit.");
}

//--------------------------------------------------------------------------------
/**
 * Function definition for set the state of Ping of things_ping_s.
 */
static void set_mask(things_ping_s *ping, ping_state_e state)
{
	THINGS_LOG_D(TAG, "Enter.");
	if (ping == NULL) {
		return;
	}

	// Lock the mutex of state variable.
	pthread_mutex_lock(&ping->mutex_state);
	THINGS_LOG_D(TAG, "(B) bit_mask_state = 0x%X", ping->bit_mask_state);

	/**
	 * If the ping's state is not equal to Ping initialization.Then set the things's ping with
	 * bitwise OR ping's state.Otherwise directly assign the value.
	 */
	if (state != PING_ST_INIT) {
		ping->bit_mask_state |= state;
	} else {
		ping->bit_mask_state = state;
	}
	THINGS_LOG_D(TAG, "(A) bit_mask_state = 0x%X", ping->bit_mask_state);
	pthread_mutex_unlock(&ping->mutex_state);

	THINGS_LOG_D(TAG, "Exit.");
}

//--------------------------------------------------------------------------------
/**
 * Function definition for get the state of ping of things_ping_s.
 */
static bool get_mask(things_ping_s *ping, ping_state_e state)
{
	bool res = true;

	// Check the validation of Ping.
	if (ping == NULL) {
		THINGS_LOG_E(TAG, "ping is NULL.");
		return false;
	}

	// Lock the mutex of state variable.
	pthread_mutex_lock(&ping->mutex_state);
	res = (ping->bit_mask_state & state) != 0 ? true : false;
	pthread_mutex_unlock(&ping->mutex_state);

	return res;
}

//--------------------------------------------------------------------------------
/**
 * Function definition for set the state of ping of things_ping_s.
 */
static bool cas_mask(things_ping_s *ping, ping_state_e preState, bool preStateThen, ping_state_e postState)
{
	bool res = false;

	if (ping == NULL) {
		THINGS_LOG_E(TAG, "ping is NULL.");
		return false;
	}

	pthread_mutex_lock(&ping->mutex_state);
	THINGS_LOG_D(TAG, "(B) bit_mask_state = 0x%X", ping->bit_mask_state);
	bool state = (ping->bit_mask_state & preState) != 0 ? true : false;
	if (state == preStateThen) {
		if (postState != PING_ST_INIT) {
			ping->bit_mask_state |= postState;
		} else {
			ping->bit_mask_state = postState;
		}
		res = true;
	}
	THINGS_LOG_D(TAG, "(A) bit_mask_state = 0x%X", ping->bit_mask_state);
	pthread_mutex_unlock(&ping->mutex_state);

	THINGS_LOG_D(TAG, "result = %d", res);
	return res;
}

//--------------------------------------------------------------------------------
/**
 * Function definition for unset the ping state of things_ping_s.
 */
static void unset_mask(things_ping_s *ping, ping_state_e state)
{
	THINGS_LOG_D(TAG, "Enter.");
	if (ping == NULL) {
		return;
	}

	pthread_mutex_lock(&ping->mutex_state);
	THINGS_LOG_D(TAG, "(B) bit_mask_state = 0x%X", ping->bit_mask_state);
	if (state != PING_ST_INIT) {
		ping->bit_mask_state &= (~state);
	} else {
		ping->bit_mask_state = state;
	}
	THINGS_LOG_D(TAG, "(A) bit_mask_state = 0x%X", ping->bit_mask_state);
	pthread_mutex_unlock(&ping->mutex_state);

	THINGS_LOG_D(TAG, "Exit.");
}

#ifdef THINGS_PING_ENABLE
//--------------------------------------------------------------------------------
/**
 * Function definition for find ping resource to cloud.
 */
static int find_resource_oic_ping(things_ping_s *ping)
{
	THINGS_LOG_D(TAG, "Enter.");

	// If the state of ping is not TCP session connected than nothing to do.
	if (get_mask(ping, PING_ST_TCPCONNECT) == false) {
		return 1;
	}

	int sleepTime = 1;			// seconds
	// Request handler to discover ping resource.
	OCDoHandle g_req_handle = NULL;
	// Callback to pass information from application to OC stack.
	OCCallbackData cb;
	char *hostAddr = NULL;

	// call the callback to handle discovery of ping resource.
	cb.cb = discover_ping_resource_handler;
	cb.cd = NULL;
	cb.context = (void *)ping;

	if (ping == NULL) {
		THINGS_LOG_E(TAG, "ping struct is NULL.");
		goto error;
	}

	THINGS_LOG_D(TAG, "cb.context=0x%X", cb.context);

	// Make host address.
	if ((hostAddr = make_host_addr(ping->addr, ping->port)) == NULL) {
		THINGS_LOG_E(TAG, "Memory Allocation is failed.");
		goto error;
	}

	//while (cas_mask(ping, PING_ST_REQUEST | PING_ST_INTUPDATE | PING_ST_TIMEOUT, false, PING_ST_DISCOVERY) == false) {
	//	sleep(1);				// sleep 1 second
	//}

	// Check if the state of ping is not occured time-out with regard to any request.
	if (get_mask(ping, PING_ST_TIMEOUT) == true) {
		THINGS_LOG_E(TAG, "Ping thread timeout");
		goto error;
	}

	// Set the state of ping is doing discovery in cloud.
	set_mask(ping, PING_ST_DISCOVERY);

	THINGS_LOG_D(TAG, "Send OCFindKeepAliveResouce request to %s.", hostAddr);
	iotivity_api_lock();
	// Send request to discovers  /oic/ping resource on host address.
	OCStackResult res = OCFindKeepAliveResource(&g_req_handle, hostAddr, &cb);
	iotivity_api_unlock();
	if (res == OC_STACK_OK) {
		THINGS_LOG_D(TAG, "Waiting for /oic/ping Cloud-response.(%s)", ping->addr);
		sleepTime = 5;
	} else {
		THINGS_LOG_E(TAG, "[Error]: Ping Resource Discovery-request is failed.");
		unset_mask(ping, PING_ST_DISCOVERY);
	}
	/*! Added by st_things for memory Leak fix
	 */
error:
	if (hostAddr) {
		things_free(hostAddr);
	}
	THINGS_LOG_D(TAG, "Exit.(sleepTime=%d)", sleepTime);
	return sleepTime;
}

//--------------------------------------------------------------------------------
/**
 * Function definition for sending common-Ping request for /oic/ping to Cloud.
 */
static int send_things_ping_request(things_ping_s *ping)
{
	THINGS_LOG_D(TAG, "Enter.");

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
		THINGS_LOG_E(TAG, "ping struct is NULL.");
		goto error;
	}

	THINGS_LOG_D(TAG, "cb.context=0x%X", cb.context);

	if ((hostAddr = make_host_addr(ping->addr, ping->port)) == NULL) {
		THINGS_LOG_E(TAG, "hostAddr Memory Allocation is failed.");
		goto error;
	}

	payload = OCRepPayloadCreate();
	if (!payload) {
		THINGS_LOG_E(TAG, "payload Memory Allocation is failed.");
		goto error;
	}

	//while (cas_mask(ping, PING_ST_DISCOVERY | PING_ST_INTUPDATE | PING_ST_TIMEOUT, false, PING_ST_REQUEST) == false) {
	//	sleep(1);				// sleep 1 second
	//}

	if (get_mask(ping, PING_ST_TIMEOUT) == true) {
		THINGS_LOG_E(TAG, "Ping thread timeout");
		goto error;
	}

	set_mask(ping, PING_ST_REQUEST);

	pthread_mutex_lock(&ping->mutex_int);
	interval = ping->interval[ping->int_seq];
	// Period send request to Cloud for interval hand-shaking /oic/ping resource.
	OCRepPayloadSetPropInt(payload, INTERVAL, interval);
	THINGS_LOG_D(TAG, "interval = %lld", interval);

	THINGS_LOG_D(TAG, "Send OCSendKeepAliveRequest request to %s.", hostAddr);
	iotivity_api_lock();
	// Send common-Ping request message to remote endpoint address.
	result = OCSendKeepAliveRequest(&g_req_handle, hostAddr, (OCPayload *)payload, &cb);
	iotivity_api_unlock();
	if (result == OC_STACK_OK)
	//if( (result= SendKeepAliveRequest(hostAddr, interval, &cb)) == OC_STACK_OK )
	{
		THINGS_LOG_V(TAG, "\e[35mSending Success about common-Ping request.\e[m");
		do {
			ping->int_seq = (ping->int_seq + 1) % MAX_INTERVAL_EA;
		} while (ping->interval[ping->int_seq] == 0);

		sleepTime = interval;
	}
	unset_mask(ping, PING_ST_INTUPDATED);
	pthread_mutex_unlock(&ping->mutex_int);

	if (result != OC_STACK_OK) {
		THINGS_LOG_E(TAG, "\e[31m[Error]: Sending about common-Ping request is failed. Result : %d\e[m", result);
		unset_mask(ping, PING_ST_REQUEST);
		OCRepPayloadDestroy(payload);
	}

error:
	if (hostAddr) {
		things_free(hostAddr);
	}
	THINGS_LOG_D(TAG, "Exit.");
	return sleepTime;
}
#else
//--------------------------------------------------------------------------------
/**
 * Function definition for find ping resource to cloud.
 */
static int find_resource_oic_ping(things_ping_s *ping)
{
	return 1;
}

//--------------------------------------------------------------------------------
/**
 * Function definition for sending common-Ping request for /oic/ping to Cloud.
 */
static int send_things_ping_request(things_ping_s *ping)
{
	return 1;
}
#endif

//--------------------------------------------------------------------------------
/**
 * Function definition for find minimum interval value from interval array.
 */
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

//--------------------------------------------------------------------------------
/**
 * Function definition for setting interval value in things_ping_s interval array.
 */
static bool set_interval(things_ping_s *ping, int64_t *intervals, int int_ea, ping_state_e confirmState)
{
	THINGS_LOG_D(TAG, "Enter.");

	bool res = false;
	int64_t min_interval = 0;

	if (ping == NULL || intervals == NULL || int_ea == 0) {
		THINGS_LOG_D(TAG, "Invalid arguments.(ping=%0x%X, intervals=0x%X, int_ea=%d)", ping, intervals, int_ea);
		return false;
	}

	if (int_ea > MAX_INTERVAL_EA) {
		THINGS_LOG_D(TAG, "int_ea is over %d. So, fix to %d", MAX_INTERVAL_EA, MAX_INTERVAL_EA);
		int_ea = MAX_INTERVAL_EA;
	}

	if (confirmState == PING_ST_INIT) {
		confirmState = PING_ALL_FLAG;
	}

	pthread_mutex_lock(&ping->mutex_int);
	if (get_mask(ping, confirmState) == true && get_mask(ping, PING_ST_TIMEOUT) == false) {
		// Find minimum interval value from interval array.
		min_interval = find_minimum_from_intervals(intervals, int_ea);
		memset(ping->interval, 0, sizeof(int64_t) * MAX_INTERVAL_EA);
		int i = 0;

		// Set the things_ping_s interval array.
		for (i = 0; i < int_ea; i++) {
			THINGS_LOG_D(TAG, "Setting interval[%d]=%d", i, min_interval);
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

	THINGS_LOG_D(TAG, "Exit.");
	return res;
}

//--------------------------------------------------------------------------------
/**
 * Function definition for defined the interval value for things_ping_s ping interval array.
 */
static bool set_def_interval(things_ping_s *ping)
{
	if (ping == NULL) {
		THINGS_LOG_E(TAG, "ping is NULL.");
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

//--------------------------------------------------------------------------------
/**
 * Function definition for creating ping thread.
 */
static bool things_ping_create_thread(things_ping_s *ping)
{
	THINGS_LOG_D(TAG, "Enter.");

	bool res = false;

	// Check the validation of ping, ping's address and ping's port no.
	if (ping == NULL || ping->addr == NULL || ping->port == 0) {
		THINGS_LOG_E(TAG, "Invalid arguments.(ping=0x%X)", ping);
		if (ping) {
			THINGS_LOG_E(TAG, "Invalid arguments.(addr=%s, port=%d)", ping->addr, ping->port);
		}
		return res;
	}

	pthread_mutex_lock(&ping->mutex_thread);
	if (ping->handle_thread == 0) {
		// Create pthread for ping.
		if (pthread_create_rtos(&ping->handle_thread, NULL, (pthread_func_type) thd_ping_loop, ping, THINGS_STACK_PING_THREAD) != 0) {
			THINGS_LOG_E(TAG, "Create thd_ping_loop thread is failed.");
			ping->handle_thread = 0;
			goto GOTO_OUT;
		}

		set_mask(ping, PING_ST_STARTTHREAD);
		res = true;
	}
GOTO_OUT:
	pthread_mutex_unlock(&ping->mutex_thread);

	THINGS_LOG_D(TAG, "Exit.(result=%d)", res);
	return res;
}

//--------------------------------------------------------------------------------
/**
 * Function definition for destory ping thread.
 */
static bool things_ping_destroy_thread(things_ping_s *ping)
{
	THINGS_LOG_D(TAG, "Enter.");
	bool res = false;

	// Check the validation of ping, ping's address.
	if (ping == NULL || ping->addr == NULL) {
		THINGS_LOG_E(TAG, "Invalid arguments.(ping=0x%X)", ping);
		return res;
	}

	if (ping->handle_thread) {
		pthread_t handle = ping->handle_thread;
		ping->handle_thread = 0;
		ping->continue_thread = false;
#ifdef __ST_THINGS_RTOS__
		pthread_detach(handle);
		while (1) {
			if (get_mask(ping, PING_ST_STARTTHREAD) == true) {
				THINGS_LOG_D(TAG, "Wait destroy ping thread 1 sec");
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

	THINGS_LOG_D(TAG, "Exit.(result=%d)", res);
	return res;
}

//--------------------------------------------------------------------------------
/**
 * Function definition for printing response value of payload.
 */
static void things_rep_payload_value_print(OCRepPayloadValue *value)
{
	switch (value->type) {
	case OCREP_PROP_INT:
		THINGS_LOG_D(TAG, "Key: %s, Value: %lld, int", value->name, value->i);
		break;
	case OCREP_PROP_DOUBLE:
		THINGS_LOG_D(TAG, "Key: %s, Value: %f, double", value->name, value->d);
		break;
	case OCREP_PROP_STRING:
		THINGS_LOG_D(TAG, "Key: %s, Value: %s, string", value->name, value->str);
		break;
	case OCREP_PROP_BOOL:
		THINGS_LOG_D(TAG, "Key: %s, Value: %d, bool", value->name, value->b);
		break;
	case OCREP_PROP_BYTE_STRING:
		THINGS_LOG_D(TAG, "Key: %s(binary) size : %d", value->name, value->ocByteStr.len);
		//OCPrintByteValue( value->ocByteStr.bytes, value->ocByteStr.len );
		break;
	case OCREP_PROP_OBJECT:
		// Note: Only prints the URI (if available), to print further, you'll
		// need to dig into the object better!
		THINGS_LOG_D(TAG, "Key: %s(OCRep):%s", value->name, value->obj->uri);
		break;
	case OCREP_PROP_ARRAY:
		switch (value->arr.type) {
		case OCREP_PROP_INT:
			THINGS_LOG_D(TAG, "Key: %s(int array):%zu x %zu x %zu", value->name, value->arr.dimensions[0], value->arr.dimensions[1], value->arr.dimensions[2]);
			for (size_t iter = 0; iter < value->arr.dimensions[0]; iter++) {
				THINGS_LOG_D(TAG, "Key: \t(int):%d", value->arr.iArray[iter]);
			}
			break;
		case OCREP_PROP_DOUBLE:
			THINGS_LOG_D(TAG, "Key: %s(double array):%zu x %zu x %zu", value->name, value->arr.dimensions[0], value->arr.dimensions[1], value->arr.dimensions[2]);
			for (size_t iter = 0; iter < value->arr.dimensions[0]; iter++) {
				THINGS_LOG_D(TAG, "Key: \t(double):%f", value->arr.dArray[iter]);
			}
			break;
		case OCREP_PROP_BOOL:
			THINGS_LOG_D(TAG, "Key: %s(bool array):%zu x %zu x %zu", value->name, value->arr.dimensions[0], value->arr.dimensions[1], value->arr.dimensions[2]);
			for (size_t iter = 0; iter < value->arr.dimensions[0]; iter++) {
				THINGS_LOG_D(TAG, "Key: \t(bool):%s", value->arr.bArray[iter] ? "true" : "false");
			}
			break;
		case OCREP_PROP_STRING:
			THINGS_LOG_D(TAG, "Key: %s(string array):%zu x %zu x %zu", value->name, value->arr.dimensions[0], value->arr.dimensions[1], value->arr.dimensions[2]);
			for (size_t iter = 0; iter < value->arr.dimensions[0]; iter++) {
				THINGS_LOG_D(TAG, "Key: \t(string):%s", value->arr.strArray[iter]);
			}
			break;
		case OCREP_PROP_BYTE_STRING:
			THINGS_LOG_D(TAG, "Key: %s(byte array):%zu x %zu x %zu", value->name, value->arr.dimensions[0], value->arr.dimensions[1], value->arr.dimensions[2]);
			break;
		default:
			THINGS_LOG_E(TAG, "Key: %s <-- Unknown/unsupported array type!", value->name);
			break;
		}
		break;
	default:
		THINGS_LOG_D(TAG, "Un supported val Type.(0x%d)", value->type);
		break;
	}
}

//-------------------------------------------------------------------------------
/**
 * Function definition for making host address.
 */
static char *make_host_addr(const char *ip, const uint16_t port)
{
	THINGS_LOG_D(TAG, "Enter.");

	// Initialize a host string with NULL
	char *host = NULL;
	int length = 0;

	// Print IP address and port number.
	THINGS_LOG_D(TAG, "ip=%s, port=%d", ip, port);
	if (ip == NULL || strlen(ip) == 0 || port == 0) {
		THINGS_LOG_E(TAG, "Invalid arguments.");
		return NULL;
	}

	// Calculate length to assign memory for host.
	length = strlen(ip) + 7 + strlen(DEFAULT_COAP_TCP_HOST);
	// Create dynamic memory for host.
	if ((host = (char *)things_malloc(sizeof(char) * length)) == NULL) {
		THINGS_LOG_E(TAG, "memory allocation is failed.");
		return NULL;
	}
	// Update Cloud Address.
	memset(host, 0, length);
	if (snprintf(host, length, "%s%s:%d", DEFAULT_COAP_TCP_HOST, ip, port) >= length) {
		THINGS_LOG_E(TAG, "[Error] host buffer is overflowed.");
		things_free(host);
		host = NULL;
	}

	// Print the host address.
	THINGS_LOG_D(TAG, "host = %s", host);

	THINGS_LOG_D(TAG, "Exit.");
	return host;
}
