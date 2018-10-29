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

#define _POSIX_C_SOURCE 200809L
#define _BSD_SOURCE

#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#include "ocstack.h"
#include "ocpayload.h"
#include "cautilinterface.h"

#include "easy-setup/es_common.h"
#include "cacommon.h"
#include "utils/things_string.h"
#include "things_def.h"
#include "logging/things_logger.h"
#include "utils/things_malloc.h"
#include "utils/things_string.h"
#include "framework/things_data_manager.h"
#include "cloud_connector.h"
#include "cloud_manager.h"
#include "cloud_evtpend.h"
#include "things_types.h"
#include "easy-setup/easysetup.h"
#include "easy-setup/easysetup_manager.h"
#include "easy-setup/resource_handler.h"
#include "utils/things_network.h"
#include "NSProviderInterface.h"
#include "utils/things_ping.h"
#include "framework/things_security_manager.h"
#include "utils/things_rtos_util.h"

#define MAX_CI_ADDRESS          256
#define RESOURCE_DISCOVERY_QUERY "/oic/res"
#define RESOURCE_DISCOVERY_QUERY_DI "/oic/res?di="

#define ERR_UPPER_UNIT  10000	//!< Unit of Network-State for Cloud Error-Code
#define MAX_SIGNUP_SENDNUM  1	//!< Max re-transmission count of send_cnt_sign_up variable.
#define MAX_RETRY_RSCPUBLISH    10	//!< Max re-transmission count of retranslate_rsc_publish_cnt variable.

#define TAG "[cloudmnger]"

typedef void *(*pthread_func_type)(void *);

/*! Cloud Session Management enum. */
typedef enum {
	CISESS_NULL = -2,
	CISESS_APDISCON = -1,		//!< -1 : AP Disconnected.
	CISESS_STOP = 0,			//!<  0 : Cloud Connection Disconnected & AP Connected.
	CISESS_BUSY = 1,			//!<  1 : Cloud Connection Connecting & Connected. (Busy)
	CISESS_STOP_TRIGGER = 2,	//!<  2 : Triggered for Cloud Connection disconnect.
	CISESS_SIGNOUT = 3			//!<  3 : Sign-Out from Cloud.
} ci_session_level_e;

/*! Provisioned Data + Time-Out Data. */
typedef struct es_cloud_event_timeout_s {
	things_timeout_s timeOut;			//!< Time-Out value(unit: seconds)
	es_cloud_prov_data_s event;	//!< Cloud Provisioned Data
} es_cloud_event_timeout_s;

volatile static ci_session_level_e g_qis_cloud_thread_running = CISESS_APDISCON;	//!< Variable for managing Cloud session.(No cached)

static int send_cnt_sign_up = 0;	//!< Counter for Sign-UP re-transmission process.
static int retranslate_rsc_publish_cnt = 0;	//!< Counter for Resource-Publish re-transmission process.

static pthread_mutex_t g_es_mutex = PTHREAD_MUTEX_INITIALIZER;	//!< Mutex variable for thread-safety of start_ci_connection().
static pthread_mutex_t g_es_tcp_session_mutex = PTHREAD_MUTEX_INITIALIZER;	//!< Mutex variable for thread-safety of ci_retry_stop_by_tcp_cb().

static char g_cloud_ip[IP_PORT] = { 0, };	//!< Current Connected Cloud Ipv4 address.
static char g_cloud_port[IP_PORT] = { 0, };	//!< Current Connected Cloud Port number.
static char g_cloud_address[MAX_CI_ADDRESS] = { 0, };	//!< Current Connected Cloud Address.

static es_cloud_signup_s *signed_up_data = NULL;

static things_server_builder_s *g_server_builder = NULL;	//!< Server-Builder pointer to get Resource List.
static things_resource_s **g_resource_lists = NULL;	//!< Resource List pointer to be publishing resources to Cloud.
static int g_len = 0;			//!< EA of Resources in "g_resource_lists" variable.

static things_cloud_con_result_func_type things_res_cb_function = NULL;	//!< call-back Function pointer : it's called when finish Cloud-Connection.

static OCStackResult publish_resource_main_dev_into_cloud(things_resource_s **list, int length, things_timeout_s *timeout);
static int cloud_retry_sign_up(es_cloud_prov_data_s *event_data, things_timeout_s *timeout);
static char *make_cloud_address(char *ip, char *port, const char *ci_addr);
static void cloud_request_retry_trigger(things_timeout_s *timeout);
static es_cloud_prov_data_s *if_failed_then_retry(OCDoHandle handle, OCStackResult result, int *n_err);
static void force_session_stop(ci_session_level_e state);
static int es_cloud_state_set_and_notify(things_cloud_status_e state, es_error_code_e es_err, OCClientResponse *response, ci_error_code_e *ci_err);
static int get_cloud_code(OCClientResponse *response, OCMethod method, ci_error_code_e *err);
static CAResponseResult_t things_result_to_network_result(OCStackResult oc_code, OCMethod method);
static char *make_ip_port(char *p_ip, char *p_port);
static es_cloud_event_timeout_s *clone_data_add_timeout(es_cloud_prov_data_s *cloud_data, things_timeout_s *timeout);
static void ci_finish_cloud_con(int result);
static void publish_dev_profile_into_cloud(things_timeout_s *timeout);

static void *handle_signup_timeout(things_timeout_s *timeout)
{
	THINGS_LOG_D(TAG, "Sign-UP is Time-Out.");

	es_cloud_prov_data_s *pend_cloud_data = NULL;

	if (timeout == NULL) {
		THINGS_LOG_E(TAG, "[Error] timeout is NULL.");
		return 0;
	}

	send_cnt_sign_up++;

	if (send_cnt_sign_up <= MAX_SIGNUP_SENDNUM) {
		if ((pend_cloud_data = ci_cp_get_pended_data()) != NULL) {
			timeout->cur_counter = timeout->cur_num;
			THINGS_LOG_V(TAG, "Sign-UP Request Send is re-tryed(%d).", send_cnt_sign_up);
			if (cloud_retry_sign_up(pend_cloud_data, timeout) == 0) {
				send_cnt_sign_up = MAX_SIGNUP_SENDNUM + 1;
			}
		} else {
			send_cnt_sign_up = MAX_SIGNUP_SENDNUM + 1;
		}
	}

	if (send_cnt_sign_up > MAX_SIGNUP_SENDNUM) {
		THINGS_LOG_V(TAG, "Sign-UP Re-Try Request Send is failed.");

		things_ping_unset_mask(g_cloud_ip, PING_ST_ISCLOUD | PING_ST_SIGNIN);

		if (es_cloud_state_set_and_notify(ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, ES_ERRCODE_NO_RESPONSE_FROM_CLOUD_SERVER, NULL, NULL) != 0) {
			THINGS_LOG_D(TAG, "StateAndNotify is failed.");
		}

		force_session_stop(CISESS_NULL);
		send_cnt_sign_up = 0;
	}

	THINGS_LOG_D(TAG, "Exit.");
	return 0;
}

static void *handle_signin_timeout(things_timeout_s *timeout)
{
	THINGS_LOG_D(TAG, "Sign-IN is Time-Out.");

	if (timeout == NULL) {
		THINGS_LOG_E(TAG, "[Error] timeout is NULL.");
		return 0;
	}

	int thres_hold = timeout->ori_num * 10;	// PDF thres_hold 0.2 --> 5

	if (timeout->cur_num < thres_hold && (timeout->cur_num = (int)things_next_time_out((long long)timeout->ori_num, (long long)timeout->cur_num)) != -1) {
		if (timeout->cur_num > thres_hold) {
			timeout->cur_num = thres_hold;
		}
		timeout->cur_counter = timeout->cur_num;

		THINGS_LOG_V(TAG, "Sign-IN Request Send is re-tryed. (PDF interval:%d)", timeout->cur_num);

		force_session_stop(CISESS_SIGNOUT);
		cloud_request_retry_trigger(timeout);
	} else {
		THINGS_LOG_V(TAG, "Sign-IN Re-Try Request Send is failed.");

		things_ping_unset_mask(g_cloud_ip, PING_ST_ISCLOUD | PING_ST_SIGNIN);

		if (es_cloud_state_set_and_notify(ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, ES_ERRCODE_NO_RESPONSE_FROM_CLOUD_SERVER, NULL, NULL) != 0) {
			THINGS_LOG_D(TAG, "StateAndNotify is failed.");
		}

		force_session_stop(CISESS_NULL);
	}

	THINGS_LOG_D(TAG, "Exit.");
	return 0;
}

static void *handle_publish_timeout(things_timeout_s *timeout)
{
	THINGS_LOG_D(TAG, "Resource-Publish is Time-Out.");

	if (timeout == NULL) {
		THINGS_LOG_E(TAG, "[Error] timeout is NULL.");
		return 0;
	}

	int thres_hold = timeout->ori_num * 5;	// PDF thres_hold 0.2 --> 5

	if (timeout->cur_num < thres_hold && (timeout->cur_num = (int)things_next_time_out((long long)timeout->ori_num, (long long)timeout->cur_num)) != -1) {
		if (timeout->cur_num > thres_hold) {
			timeout->cur_num = thres_hold;
		}
		timeout->cur_counter = timeout->cur_num;

		THINGS_LOG_V(TAG, "RSC_Publish Request Send is re-tryed. (PDF interval:%d)", timeout->cur_num);
		publish_resource_into_cloud(RSC_PUB_MAIN_ONLY, timeout);
	} else {
		THINGS_LOG_V(TAG, "Resource-Publish Re-Try Request Sending is failed.");

		if (es_cloud_state_set_and_notify(ES_STATE_FAILED_TO_PUBLISH_RESOURCES_TO_CLOUD, ES_ERRCODE_NO_RESPONSE_FROM_CLOUD_SERVER, NULL, NULL) != 0) {
			THINGS_LOG_D(TAG, "StateAndNotify is failed.");
		}

		force_session_stop(CISESS_NULL);
	}

	THINGS_LOG_D(TAG, "Exit.");
	return 0;
}

static void *handle_dev_profile_timeout(things_timeout_s *timeout)
{
	THINGS_LOG_D(TAG, "Device Profile Publish is Time-Out.");

	if (timeout == NULL) {
		THINGS_LOG_E(TAG, "[Error] timeout is NULL.");
		return 0;
	}

	int thres_hold = timeout->ori_num * 5;	// PDF thres_hold 0.2 --> 5

	if (timeout->cur_num < thres_hold && (timeout->cur_num = (int)things_next_time_out((long long)timeout->ori_num, (long long)timeout->cur_num)) != -1) {
		if (timeout->cur_num > thres_hold) {
			timeout->cur_num = thres_hold;
		}
		timeout->cur_counter = timeout->cur_num;

		THINGS_LOG_V(TAG, "Device Profile Request Send is re-tryed. (PDF interval:%d)", timeout->cur_num);
		publish_dev_profile_into_cloud(timeout);
	} else {
		THINGS_LOG_V(TAG, "Device Profile publish Re-Try Request Sending is failed.");
	}

	THINGS_LOG_D(TAG, "Exit.");
	return 0;
}

static int set_def_cloud_info(es_cloud_signup_s *cloud_info, const char *cloud_addr, const es_cloud_prov_data_s *prov_data)
{
	int result = 1;
	char *ip_port = NULL;
	char *m_domain = NULL;
	char *t_domain = NULL;

	if (cloud_info == NULL || cloud_addr == NULL || cloud_addr[0] == 0) {
		THINGS_LOG_E(TAG, "Parameter value is invalid.(cloud_info=0x%X, coud_addr=%s)", cloud_info, cloud_addr);
		return 0;
	}

	THINGS_LOG_D(TAG, "cloud_addr = %s", cloud_addr);

	if (cloud_info->address != NULL) {
		things_free(cloud_info->address);
		cloud_info->address = NULL;
	}

	if (cloud_info->port != NULL) {
		things_free(cloud_info->port);
		cloud_info->port = NULL;
	}

	if (cloud_info->domain != NULL) {
		things_free(cloud_info->domain);
		cloud_info->domain = NULL;
	}

	ip_port = strstr(cloud_addr, "://");
	if (ip_port == NULL) {
		THINGS_LOG_E(TAG, "cloud_addr is invalid.");
		return 0;
	}
	ip_port = ip_port + 3;

	if ((cloud_info->address = (char *)things_malloc(sizeof(char) * MAX_CI_ADDRESS)) == NULL) {
		THINGS_LOG_E(TAG, "Cloud_info address memory allocation is Failed.");
		return 0;
	}

	things_strncpy(cloud_info->address, ip_port, MAX_CI_ADDRESS);	// Ip:Port  save.

	// if exist Cloud m_domain Name, then Save Name and Port.
	if (cloud_info->redirect_uri != NULL && strlen(cloud_info->redirect_uri) > 0) {
		m_domain = things_strdup(cloud_info->redirect_uri);
	} else if (prov_data != NULL) {
		m_domain = things_strdup(prov_data->host_name);
	}

	if (m_domain != NULL && strlen(m_domain) != 0) {
		t_domain = m_domain;
		if (strstr(m_domain, "://") != NULL) {
			t_domain = strstr(m_domain, "://") + 3;
		}

		if (strchr(t_domain, ':') != NULL) {
			ip_port = strchr(t_domain, ':');
			if (ip_port) {
				*ip_port = NULL;
			}
			ip_port++;
		} else {
			if ((ip_port = strchr(ip_port, ':')) == NULL) {
				THINGS_LOG_E(TAG, "Not exit Port number.");
				result = 0;
				goto GOTO_OUT;
			}
			ip_port++;
		}

		THINGS_LOG_D(TAG, "m_domain = %s", t_domain);
		THINGS_LOG_D(TAG, "Port   = %s", ip_port);

		int length_port = strlen(ip_port) + 1;
		int length_domain = strlen(t_domain) + 1;

		if ((cloud_info->port = (char *)things_malloc(sizeof(char) * length_port)) == NULL) {
			THINGS_LOG_E(TAG, "Cloud_info port memory allocation is Failed.");
			result = 0;
			goto GOTO_OUT;
		}

		if ((cloud_info->domain = (char *)things_malloc(sizeof(char) * length_domain)) == NULL) {
			THINGS_LOG_E(TAG, "Cloud_info domain-name memory allocation is Failed.");
			result = 0;
			goto GOTO_OUT;
		}

		things_strncpy(cloud_info->port, ip_port, length_port);	// Cloud Port save.

		things_strncpy(cloud_info->domain, t_domain, length_domain);	// Cloud domain name save.
	}

	if (cloud_info->refresh_token == NULL) {
		if (prov_data == NULL || prov_data->refreshtoken[0] == 0) {
			THINGS_LOG_E(TAG, "Not exist Refresh Token. please check your system.");
			result = 0;
			goto GOTO_OUT;
		}

		THINGS_LOG_D(TAG, "refreshtoken = %s", prov_data->refreshtoken);

		int length_refresh = strlen(prov_data->refreshtoken) + 1;

		if ((cloud_info->refresh_token = (char *)things_malloc(sizeof(char) * length_refresh)) == NULL) {
			THINGS_LOG_E(TAG, "Cloud_info port memory allocation is Failed.");
			result = 0;
			goto GOTO_OUT;
		}

		things_strncpy(cloud_info->refresh_token, prov_data->refreshtoken, length_refresh);	// Cloud Refresh Token save.
	}

GOTO_OUT:
	if (m_domain) {
		things_free(m_domain);
		m_domain = NULL;
		t_domain = NULL;
	}
	ip_port = NULL;

	if (result == 0) {
		if (cloud_info->address) {
			things_free(cloud_info->address);
			cloud_info->address = NULL;
		}

		if (cloud_info->port) {
			things_free(cloud_info->port);
			cloud_info->port = NULL;
		}

		if (cloud_info->domain) {
			things_free(cloud_info->domain);
			cloud_info->domain = NULL;
		}
	}

	return result;
}

OCStackApplicationResult handle_register_cb(void *ctx, OCDoHandle handle, OCClientResponse *client_response)
{
	int res = OC_STACK_DELETE_TRANSACTION;
	int n_err = 0;
	es_cloud_prov_data_s *proved_data = NULL;

	if (ctx != (void *)DEFAULT_CONTEXT_VALUE) {
		THINGS_LOG_D(TAG, "Invalid Register callback received");
	}

	THINGS_LOG_D(TAG, "Register response received code: %s(%d)", get_result(client_response->result), client_response->result);
	if ((proved_data = if_failed_then_retry(handle, client_response->result, &n_err)) == NULL) {
		if (n_err == 1 || n_err == 4) {	// OC_STACK_COMM_ERROR or Handle is invalid.
			THINGS_LOG_D(TAG, "OC_STACK_COMM_ERROR(1) or Handle InValid(4) : %d", n_err);
			return OC_STACK_DELETE_TRANSACTION;
		}

		THINGS_LOG_E(TAG, "System Error occurred.(proved_data=0x%X, n_err=%d)", proved_data, n_err);
		if (es_cloud_state_set_and_notify(ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, ER_ERRCODE_SYSTEM_ERROR, NULL, NULL) != 0) {
			THINGS_LOG_E(TAG, "StateAndNotify is failed.");
		}

		return OC_STACK_DELETE_TRANSACTION;
	}

	send_cnt_sign_up = 0;

	if (client_response->payload != NULL && client_response->payload->type == PAYLOAD_TYPE_REPRESENTATION) {
		THINGS_LOG_D(TAG, "PAYLOAD_TYPE_REPRESENTATION received");
		THINGS_LOG_D(TAG, "You can login using received session variable after disconnected or reboot");

		OCRepPayloadValue *val = ((OCRepPayload *) client_response->payload)->values;
		while (val != NULL) {
			if (val->type == OCREP_PROP_INT) {
				THINGS_LOG_D(TAG, "Key: %s, Value: %lld, int ", val->name, val->i);
			} else if (val->type == OCREP_PROP_STRING) {
				THINGS_LOG_D(TAG, "Key: %s, Value: %s, string", val->name, val->str);
			} else {
				THINGS_LOG_D(TAG, "Un supported val Type.(0x%d)", val->type);
			}

			val = val->next;
		}

		if (client_response->result != OC_STACK_RESOURCE_CHANGED) {
			ci_error_code_e ci_err = ERRCI_NO_ERROR;

			things_ping_unset_mask(g_cloud_ip, PING_ST_ISCLOUD | PING_ST_SIGNIN);

			if (es_cloud_state_set_and_notify(ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, (ci_error_code_e) NULL, client_response, &ci_err) != 0) {
				THINGS_LOG_D(TAG, "StateAndNotify is failed.");
				res = OC_STACK_DELETE_TRANSACTION;
				goto GOTO_OUT;
			}

			switch (ci_err) {
			case ERRCI_NOT_ACCEPTABLE:
				THINGS_LOG_D(TAG, "Cloud received duplicated-request in 500msec. retry-again");
				if (g_qis_cloud_thread_running != CISESS_APDISCON) {
					cloud_request_retry_trigger(NULL);
				}
				break;
			case ERRCI_INTERNAL_SERVER_ERROR:
				THINGS_LOG_D(TAG, "[ERRCODE]Cloud Server has a issue, Check your Cloud Server.");
				esm_get_network_status();	// State return
				break;
			case ERRCI_SAMSUNG_ACCOUNT_UNAUTHORIZED_TOKEN:
			case ERRCI_SAMSUNG_ACCOUNT_AUTHORIZATION_FAILED:
			case ERRCI_FORBIDDEN:
			case ERRCI_USER_NOT_FOUND:
				THINGS_LOG_D(TAG, "[ERRCODE] %d : fix the information and try again.", ci_err);
				if (cloud_retry_sign_in(NULL) != 0) {
					THINGS_LOG_E(TAG, "Cloud connection Re-try is failed.");
				}
				res = OC_STACK_DELETE_TRANSACTION;
				break;
			case ERRCI_DEVICE_DUPLICATE_PARAM:
				THINGS_LOG_D(TAG, "[ERRCODE] %d : check your device_id and try again.", ci_err);
				;				// TODO : need cloud definition.
				break;
			default:
				THINGS_LOG_D(TAG, "Not Support This Cloud-Error-Code(%d) Exception", ci_err);
				esm_get_network_status();	// State return
				res = OC_STACK_DELETE_TRANSACTION;
				break;
			}

			goto GOTO_OUT;
		}

		if (es_cloud_signup_init(&signed_up_data) == false) {
			es_cloud_state_set_and_notify(ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, ER_ERRCODE_SYSTEM_ERROR, NULL, NULL);
			THINGS_LOG_E(TAG, "signed_up_data-memory allocation is Failed, try again.");
			res = OC_STACK_DELETE_TRANSACTION;
			goto GOTO_OUT;
		}
		// [Kes] get Signed-data but mandatory key-value are follows. (accesstoken / refreshtoken / token_type / expiresin / uid)
		OCRepPayloadGetPropString(((OCRepPayload *) client_response->payload), KEY_TOKEN_ACCESS, &signed_up_data->access_token);
		OCRepPayloadGetPropString(((OCRepPayload *) client_response->payload), KEY_TOKEN_ACCESS_REFRESH, &signed_up_data->refresh_token);
		OCRepPayloadGetPropString(((OCRepPayload *) client_response->payload), KEY_TOKEN_TYPE, &signed_up_data->token_type);
		OCRepPayloadGetPropInt(((OCRepPayload *) client_response->payload), KEY_EXPIRE_TIME, &signed_up_data->expire_time);
		OCRepPayloadGetPropString(((OCRepPayload *) client_response->payload), KEY_ID_USER, &signed_up_data->uid);
		OCRepPayloadGetPropString(((OCRepPayload *) client_response->payload), KEY_SERVER_REDIRECT_URI, &signed_up_data->redirect_uri);
		OCRepPayloadGetPropString(((OCRepPayload *) client_response->payload), KEY_CERTIFICATE_FILE, &signed_up_data->certificate);
		OCRepPayloadGetPropString(((OCRepPayload *) client_response->payload), KEY_SERVER_ID, &signed_up_data->sid);

#ifdef __SECURED__
		if (sm_save_cloud_acl(signed_up_data->sid) != 0) {	// OIC_SEC_OK(0)
			es_cloud_state_set_and_notify(ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, ER_ERRCODE_SYSTEM_ERROR, NULL, NULL);
			THINGS_LOG_E(TAG, "It's failed to save Cloud UUID to ACL list.");
			res = OC_STACK_DELETE_TRANSACTION;
			goto GOTO_OUT;
		}
#endif							//__SECURED__

		if (set_def_cloud_info(signed_up_data, g_cloud_address, proved_data) == 0) {
			es_cloud_state_set_and_notify(ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, ER_ERRCODE_SYSTEM_ERROR, NULL, NULL);
			THINGS_LOG_E(TAG, "signed_up_data default setting is Failed.");
			res = OC_STACK_DELETE_TRANSACTION;
			goto GOTO_OUT;
		}

		THINGS_LOG_D(TAG, "expiresin : %lld", signed_up_data->expire_time);

		if (signed_up_data->access_token == NULL || strlen(signed_up_data->access_token) < 1) {
			THINGS_LOG_E(TAG, "Session Key is NULL");

			if (es_cloud_state_set_and_notify(ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, ES_ERRCODE_INVALID_ACCESSTOKEN, NULL, NULL) != 0) {
				THINGS_LOG_D(TAG, "StateAndNotify is failed.");
				res = OC_STACK_DELETE_TRANSACTION;
				goto GOTO_OUT;
			}
		} else {
			THINGS_LOG_D(TAG, "Write Sign-Up data to file.");
			if (dm_update_things_cloud(signed_up_data) == 0) {
				THINGS_LOG_E(TAG, "Sign-UP data can not write to info file.");
				res = OC_STACK_DELETE_TRANSACTION;
				goto GOTO_OUT;
			}

			THINGS_LOG_D(TAG, "Start Login Into Cloud");
			log_in_out_to_cloud(true, NULL);
			res = OC_STACK_DELETE_TRANSACTION;
		}
	} else {
		THINGS_LOG_E(TAG, "Received Failed-Response of Sign-UP without payload from Cloud Server.");

		if (es_cloud_state_set_and_notify(ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, ES_ERRCODE_UNKNOWN, NULL, NULL) != 0) {
			THINGS_LOG_E(TAG, "StateAndNotify is failed.");
			res = OC_STACK_DELETE_TRANSACTION;
			goto GOTO_OUT;
		}
	}

GOTO_OUT:
	if (proved_data) {
		things_free(proved_data);
		proved_data = NULL;
	}

	return res;
}

/**
 * @brief                 Call-Back function of "OCDoResource" for Cloud Sign-IN response.
 * @param ctx             User Data. (Not used)
 * @param handle          OCDoHandle of Request correspond to this Cloud-Response.
 * @param client_response  Cloud Response Data.
 * @return                Success return OC_STACK_DELETE_TRANSACTION (1)  \n
 *                        Failed  return OC_STACK_DELETE_TRANSACTION (0)
 */
OCStackApplicationResult handle_login_cb(void *ctx, OCDoHandle handle, OCClientResponse *client_response)
{
	THINGS_LOG_D(TAG, "Login callback received");

	int n_err = 0;
	es_cloud_prov_data_s *proved_data = NULL;
	OCStackApplicationResult res = OC_STACK_DELETE_TRANSACTION;

	if (ctx != (void *)DEFAULT_CONTEXT_VALUE) {
		THINGS_LOG_D(TAG, "Invalid Login callback received");
	}

	THINGS_LOG_D(TAG, "Login response received code: %s(%d)", get_result(client_response->result), client_response->result);
	proved_data = if_failed_then_retry(handle, client_response->result, &n_err);

	if (proved_data) {
		things_free(proved_data);
		proved_data = NULL;
	}

	if (n_err >= 3 || n_err == 1) {	// mem alloc error, handle is invalid.
		THINGS_LOG_D(TAG, "Memory alloc error or InValid Resp CB.(%d)", n_err);
		return OC_STACK_DELETE_TRANSACTION;
	}

	if (client_response->payload != NULL && client_response->payload->type == PAYLOAD_TYPE_REPRESENTATION) {
		THINGS_LOG_D(TAG, "PAYLOAD_TYPE_REPRESENTATION received");

		OCRepPayloadValue *val = ((OCRepPayload *) client_response->payload)->values;
		while (val) {
			if (val->type == OCREP_PROP_INT) {
				THINGS_LOG_D(TAG, "Key: %s, Value: %lld, int", val->name, val->i);
			} else if (val->type == OCREP_PROP_STRING) {
				THINGS_LOG_D(TAG, "Key: %s, Value: %s, string", val->name, val->str);
			} else {
				THINGS_LOG_D(TAG, "Un supported val Type.(0x%d)", val->type);
			}

			val = val->next;
		}

		if (client_response->result != OC_STACK_RESOURCE_CHANGED) {
			ci_error_code_e ci_err = ERRCI_NO_ERROR;

			things_ping_unset_mask(g_cloud_ip, PING_ST_SIGNIN);

			if (es_cloud_state_set_and_notify(ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, (ci_error_code_e) NULL, client_response, &ci_err) != 0) {
				THINGS_LOG_D(TAG, "StateAndNotify is failed.");
				return OC_STACK_DELETE_TRANSACTION;
			}

			switch (ci_err) {
			case ERRCI_INTERNAL_SERVER_ERROR:
				THINGS_LOG_D(TAG, "[ERRCODE]Cloud Server has a issue, Check your Cloud Server.");
				things_ping_unset_mask(g_cloud_ip, PING_ST_ISCLOUD);
				esm_get_network_status();	// State return
				break;
			case ERRCI_TOKEN_VALIDATION_FAILED:
			case ERRCI_TOKEN_EXPIRED:
				THINGS_LOG_D(TAG, "[ERRCODE] AccessToken \"%d\", refresh AccessToken and try again.", ci_err);
				refresh_token_into_cloud();	// refresh accesstoken.
				break;
			case ERRCI_FORBIDDEN:
				THINGS_LOG_D(TAG, "[ERRCODE]-> URI is invalid, fix the URI and try again.");
				cloud_retry_sign_in(NULL);	// TODO : need re-code
				break;
			case ERRCI_DEVICE_NOT_FOUND:
				THINGS_LOG_D(TAG, "[ERRCODE]Device Not Found, Check your device ID and User ID coupling.");
				things_ping_unset_mask(g_cloud_ip, PING_ST_TCPCONNECT | PING_ST_ISCLOUD);
				esm_get_network_status();	// State return
				if (things_reset(NULL, RST_AUTO_RESET) == -1) {
					THINGS_LOG_E(TAG, "[Error] things_reset is failed.");
				}
				break;
			default:
				THINGS_LOG_D(TAG, "Not Support This Cloud-Error-Code(%d) Exception", ci_err);
				things_ping_unset_mask(g_cloud_ip, PING_ST_ISCLOUD);
				esm_get_network_status();	// State return
				break;
			}

			return OC_STACK_DELETE_TRANSACTION;
		}

		if (es_cloud_state_set_and_notify(ES_STATE_REGISTERED_TO_CLOUD, ES_ERRCODE_NO_ERROR, NULL, NULL) != 0) {
			THINGS_LOG_D(TAG, "StateAndNotify is failed.");
			return OC_STACK_DELETE_TRANSACTION;
		}

		esm_save_easysetup_state(ES_COMPLETE);

		things_ping_set_mask(g_cloud_ip, atoi(g_cloud_port), PING_ST_SIGNIN);

		// [ysy] Plublish resources to cloud
		THINGS_LOG_D(TAG, "Start OCCloudPUblish");
		retranslate_rsc_publish_cnt = 0;
		publish_resource_into_cloud(RSC_PUB_MAIN_ONLY, NULL);
#ifdef ENABLE_PUSH_NOTY
		NSProviderEnableRemoteService(g_cloud_address);
#endif

		res = OC_STACK_DELETE_TRANSACTION;
	} else if (client_response->result != OC_STACK_RESOURCE_CHANGED) {
		THINGS_LOG_E(TAG, "Received Failed-Response of Sign-IN without payload from Cloud Server.");

		if (es_cloud_state_set_and_notify(ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, ES_ERRCODE_UNKNOWN, NULL, NULL) != 0) {
			THINGS_LOG_E(TAG, "StateAndNotify is failed.");
			return OC_STACK_DELETE_TRANSACTION;
		}
	}

	return res;
}

/**
 * @brief                 Call-Back function of "OCDoResource" for Cloud Sign-OUT response.
 * @param ctx             User Data. (Not used)
 * @param handle          OCDoHandle of Request correspond to this Cloud-Response.
 * @param client_response  Cloud Response Data.
 * @return                Success return OC_STACK_DELETE_TRANSACTION (1) \n
 *                        Failed  return OC_STACK_DELETE_TRANSACTION (0)
 */
OCStackApplicationResult handle_logout_cb(void *ctx, OCDoHandle handle, OCClientResponse *client_response)
{
	THINGS_LOG_D(TAG, "Logout callback received");

	OCStackApplicationResult res = OC_STACK_DELETE_TRANSACTION;

	if (ctx != (void *)DEFAULT_CONTEXT_VALUE) {
		THINGS_LOG_D(TAG, "Invalid Logout callback received");
	}

	THINGS_LOG_D(TAG, "Logout response received code: %s(%d)", get_result(client_response->result), client_response->result);

	if (client_response->payload != NULL && client_response->payload->type == PAYLOAD_TYPE_REPRESENTATION) {
		THINGS_LOG_D(TAG, "PAYLOAD_TYPE_REPRESENTATION received");

		OCRepPayloadValue *val = ((OCRepPayload *) client_response->payload)->values;
		while (val) {
			if (val->type == OCREP_PROP_INT) {
				THINGS_LOG_D(TAG, "Key: %s, Value: %lld, int", val->name, val->i);
			} else if (val->type == OCREP_PROP_STRING) {
				THINGS_LOG_D(TAG, "Key: %s, Value: %s, string", val->name, val->str);
			} else {
				THINGS_LOG_D(TAG, "Un supported val Type.(0x%d)", val->type);
			}

			val = val->next;
		}
	}

	if (g_qis_cloud_thread_running != CISESS_BUSY && g_qis_cloud_thread_running != CISESS_APDISCON) {
		things_es_enrollee_state_e es_err = ES_STATE_INIT;

		if (esm_get_network_status() == true) {
			es_err = ES_STATE_CONNECTED_TO_ENROLLER;
		}

		if (es_cloud_state_set_and_notify(es_err, ES_ERRCODE_NO_ERROR, NULL, NULL) != 0) {
			THINGS_LOG_E(TAG, "StateAndNotify is failed.");
		}
	}

	res = OC_STACK_DELETE_TRANSACTION;

	return res;
}

static bool check_comm_error_retrans(OCStackResult result)
{
#define MAX_COMM_ERROR_RETRANSMISSION       10
	bool ret = false;
	static int retransmission_count = 0;

	if (result == OC_STACK_COMM_ERROR) {	// Don't Send Request Message.
		THINGS_LOG_D(TAG, "Request-Send is Failed.");
		if (g_qis_cloud_thread_running != CISESS_APDISCON && retransmission_count < MAX_COMM_ERROR_RETRANSMISSION) {
			THINGS_LOG_D(TAG, "Re-transmission try.");
			retransmission_count++;
			ret = true;
		} else {
			retransmission_count = 0;
		}
	} else {
		retransmission_count = 0;
	}

	return ret;
#undef MAX_COMM_ERROR_RETRANSMISSION
}

OCStackApplicationResult handle_refresh_token_cb(void *ctx, OCDoHandle handle, OCClientResponse *client_response)
{
	THINGS_LOG_D(TAG, "Refresh Token callback received");

	OCStackApplicationResult res = OC_STACK_DELETE_TRANSACTION;

	if (ctx != (void *)DEFAULT_CONTEXT_VALUE) {
		THINGS_LOG_D(TAG, "Invalid Login/out callback received");
	}

	THINGS_LOG_D(TAG, "Refresh Token response received code: %s(%d)", get_result(client_response->result), client_response->result);

	if (check_comm_error_retrans(client_response->result) == true) {
		THINGS_LOG_D(TAG, "Re-Start Refresh-Token Process.");
		refresh_token_into_cloud();
	}

	if (client_response->payload != NULL && client_response->payload->type == PAYLOAD_TYPE_REPRESENTATION) {
		THINGS_LOG_D(TAG, "PAYLOAD_TYPE_REPRESENTATION received");

		OCRepPayloadValue *val = ((OCRepPayload *) client_response->payload)->values;
		while (val != NULL) {
			if (val->type == OCREP_PROP_INT) {
				THINGS_LOG_D(TAG, "Key: %s, Value: %lld, int", val->name, val->i);
			} else if (val->type == OCREP_PROP_STRING) {
				THINGS_LOG_D(TAG, "Key: %s, Value: %s, string", val->name, val->str);
			} else {
				THINGS_LOG_D(TAG, "Un supported val Type.(0x%d)", val->type);
			}

			val = val->next;
		}

		if (client_response->result != OC_STACK_RESOURCE_CHANGED) {
			ci_error_code_e ci_err = ERRCI_NO_ERROR;
			// Method don't care because issue is not occurred by it.
			if (get_cloud_code(client_response, OC_REST_POST, &ci_err) == 0) {
				ci_err = ERRCI_UNKNOWN;
			}

			THINGS_LOG_D(TAG, "Cloud ErrCode = %d", ci_err);
//            es_set_cloud_error_code(ci_err);

			switch (ci_err) {
			case ERRCI_INTERNAL_SERVER_ERROR:
				THINGS_LOG_D(TAG, "[ERRCODE]Cloud Server has a issue, Check your Cloud Server.");
				esm_get_network_status();	// State return
				break;
			case ERRCI_TOKEN_VALIDATION_FAILED:
				THINGS_LOG_D(TAG, "[ERRCODE]-> RefreshToken is invalid, fix RefreshToken and try again.");
				esm_get_network_status();	// State return
				break;
			case ERRCI_SAMSUNG_ACCOUNT_UNAUTHORIZED_TOKEN:
				THINGS_LOG_D(TAG, "[ERRCODE]-> RefreshToken is mismatched with client ID, fix RefreshToken or Client ID.");
				esm_get_network_status();	// State return
				break;
			case ERRCI_TOKEN_EXPIRED:
				THINGS_LOG_D(TAG, "[ERRCODE]-> RefreshToken is expired, refresh RefreshToken and try again.");
				esm_get_network_status();	// State return
				break;
			case ERRCI_FORBIDDEN:
				THINGS_LOG_D(TAG, "[ERRCODE]-> URI is invalid, fix the URI and try again.");
				esm_get_network_status();	// State return
				break;
			default:
				THINGS_LOG_D(TAG, "Not Support This Cloud-Error-Code(%d) Exception", ci_err);
				esm_get_network_status();	// State return
				break;
			}

			return OC_STACK_DELETE_TRANSACTION;
		}

		things_free(signed_up_data->access_token);
		things_free(signed_up_data->refresh_token);
		things_free(signed_up_data->token_type);
		signed_up_data->expire_time = 0;

		OCRepPayloadGetPropString(((OCRepPayload *) client_response->payload), KEY_TOKEN_ACCESS, &signed_up_data->access_token);
		OCRepPayloadGetPropString(((OCRepPayload *) client_response->payload), KEY_TOKEN_ACCESS_REFRESH, &signed_up_data->refresh_token);
		OCRepPayloadGetPropString(((OCRepPayload *) client_response->payload), KEY_TOKEN_TYPE, &signed_up_data->token_type);
		OCRepPayloadGetPropInt(((OCRepPayload *) client_response->payload), KEY_EXPIRE_TIME, &signed_up_data->expire_time);

		THINGS_LOG_D(TAG, "expiresin : %lld", signed_up_data->expire_time);

		if (signed_up_data->access_token == NULL || strlen(signed_up_data->access_token) < 1) {
			THINGS_LOG_E(TAG, "Session Key is NULL. Refresh Token Failed.");
		} else {
			THINGS_LOG_D(TAG, "Write Sign-Up data to file.");
			if (dm_update_things_cloud(signed_up_data) == 1) {
				THINGS_LOG_D(TAG, "Re-Try Sign-IN step.");
				force_session_stop(CISESS_SIGNOUT);
				log_in_out_to_cloud(true, NULL);
			} else {
				THINGS_LOG_E(TAG, "Refreshed Sign-UP data can not write to info file.");
			}
		}

		res = OC_STACK_DELETE_TRANSACTION;
	}

	return res;
}

OCStackApplicationResult handle_main_dev_publish_cb(void *ctx, OCDoHandle handle, OCClientResponse *client_response)
{
	THINGS_LOG_D(TAG, "Publish callback received");

	if (ctx != (void *)DEFAULT_CONTEXT_VALUE) {
		THINGS_LOG_D(TAG, "Invalid Publish callback received");
	}

	THINGS_LOG_D(TAG, "Publish resource response received, code: %s(%d)", get_result(client_response->result), client_response->result);
	if (things_cas_request_handle(handle, NULL) == 1) {
		things_del_all_request_handle();

		if (check_comm_error_retrans(client_response->result) == true) {
			THINGS_LOG_D(TAG, "Re-Start publish_resource_into_cloud");
			publish_resource_into_cloud(RSC_PUB_MAIN_ONLY, NULL);
		}

	} else {
		THINGS_LOG_E(TAG, "Response Handle(0x%X) is not exist in the Request Handle list.", handle);
		return OC_STACK_DELETE_TRANSACTION;
	}

	if (client_response->payload != NULL && client_response->payload->type == PAYLOAD_TYPE_REPRESENTATION) {
		OCRepPayloadValue *val = ((OCRepPayload *) client_response->payload)->values;
		while (val != NULL) {
			if (val->type == OCREP_PROP_INT) {
				THINGS_LOG_D(TAG, "Key: %s, Value: %lld, int", val->name, val->i);
			} else if (val->type == OCREP_PROP_STRING) {
				THINGS_LOG_D(TAG, "Key: %s, Value: %s, string", val->name, val->str);
			} else {
				THINGS_LOG_D(TAG, "Un supported val Type.(0x%d)", val->type);
			}

			val = val->next;
		}

		if (client_response->result != OC_STACK_RESOURCE_CHANGED) {
			ci_error_code_e ci_err = ERRCI_NO_ERROR;

			if (es_cloud_state_set_and_notify(ES_STATE_FAILED_TO_PUBLISH_RESOURCES_TO_CLOUD, (ci_error_code_e) NULL, client_response, &ci_err) != 0) {
				THINGS_LOG_D(TAG, "StateAndNotify is failed.");
				PROFILING_TIME("Cloud Provisioning End.");
				return OC_STACK_DELETE_TRANSACTION;
			}

			switch (ci_err) {
			case ERRCI_INTERNAL_SERVER_ERROR:
				THINGS_LOG_D(TAG, "[ERRCODE]Cloud Server has a issue, Check your Cloud Server.");
				esm_get_network_status();	// State return
				break;
			case ERRCI_TOKEN_VALIDATION_FAILED:
			case ERRCI_TOKEN_EXPIRED:
				THINGS_LOG_D(TAG, "[ERRCODE]-> AccessToken \"%d\", refresh AccessToken and try again.", ci_err);
				refresh_token_into_cloud();	// refresh accesstoken.
				break;
			case ERRCI_FORBIDDEN:
				THINGS_LOG_D(TAG, "[ERRCODE]-> URI is invalid, fix the URI and try again.");
				break;
			case ERRCI_RDLINK_INVALID_PARAM:
				THINGS_LOG_D(TAG, "[ERRCODE]-> Resource List Cbor syntax is invalid. Re-Translation.");
				if (retranslate_rsc_publish_cnt < MAX_RETRY_RSCPUBLISH) {
					publish_resource_into_cloud(RSC_PUB_MAIN_ONLY, NULL);
					retranslate_rsc_publish_cnt++;
				} else {
					THINGS_LOG_V(TAG, "[INFO] Resource Publish is Re-translated about %d\n Please check Network Line.", retranslate_rsc_publish_cnt);
					retranslate_rsc_publish_cnt = 0;
					esm_get_network_status();	// State return
				}
				break;
			default:
				THINGS_LOG_D(TAG, "Not Support This Cloud-Error-Code(%d) Exception", ci_err);
				break;
			}
		}

	} else if (client_response->result != OC_STACK_RESOURCE_CHANGED) {
		THINGS_LOG_E(TAG, "Received Failed-Response of Resource-Publish without payload from Cloud Server.");

		if (es_cloud_state_set_and_notify(ES_STATE_FAILED_TO_PUBLISH_RESOURCES_TO_CLOUD, ES_ERRCODE_UNKNOWN, NULL, NULL) != 0) {
			THINGS_LOG_E(TAG, "StateAndNotify is failed.");
			PROFILING_TIME("Cloud Provisioning End.");
			return OC_STACK_DELETE_TRANSACTION;
		}
	}

	if (client_response->result == OC_STACK_RESOURCE_CHANGED) {
		if (es_cloud_state_set_and_notify(ES_STATE_PUBLISHED_RESOURCES_TO_CLOUD, ES_ERRCODE_NO_ERROR, NULL, NULL) != 0) {
			THINGS_LOG_D(TAG, "StateAndNotify is failed.");
			PROFILING_TIME("Cloud Provisioning End.");
			return OC_STACK_DELETE_TRANSACTION;
		}
#ifdef CONFIG_ST_THINGS_SUPPORT_SUB_DEVICE
		publish_resource_into_cloud(RSC_PUB_SUB_ALL, NULL);
#endif
		publish_dev_profile_into_cloud(NULL);
	}

	PROFILING_TIME("Cloud Provisioning End.");
	return OC_STACK_DELETE_TRANSACTION;
}

#ifdef CONFIG_ST_THINGS_SUPPORT_SUB_DEVICE
OCStackApplicationResult handle_sub_dev_publish_cb(void *ctx, OCDoHandle handle, OCClientResponse *client_response)
{
	THINGS_LOG_D(TAG, "Sub-Device resource Publish callback received");

	if (ctx != (void *)DEFAULT_CONTEXT_VALUE) {
		THINGS_LOG_D(TAG, "Invalid Publish callback received");
	}

	THINGS_LOG_D(TAG, "Publish resource response received, code: %s(%d)", get_result(client_response->result), client_response->result);
	if (check_comm_error_retrans(client_response->result) == true) {
		THINGS_LOG_D(TAG, "Resource Publish Request-Send is Failed.");
		publish_resource_into_cloud(RSC_PUB_SUB_ALL, NULL);
	}

	if (client_response->payload != NULL && client_response->payload->type == PAYLOAD_TYPE_REPRESENTATION) {
		OCRepPayloadValue *val = ((OCRepPayload *) client_response->payload)->values;
		while (val != NULL) {
			if (val->type == OCREP_PROP_INT) {
				THINGS_LOG_D(TAG, "Key: %s, Value: %lld, int", val->name, val->i);
			} else if (val->type == OCREP_PROP_STRING) {
				THINGS_LOG_D(TAG, "Key: %s, Value: %s, string", val->name, val->str);
			} else {
				THINGS_LOG_D(TAG, "Un supported val Type.(0x%d)", val->type);
			}

			val = val->next;
		}

		if (client_response->result != OC_STACK_RESOURCE_CHANGED) {
			ci_error_code_e ci_err = ERRCI_NO_ERROR;

			if (get_cloud_code(client_response, OC_REST_POST, &ci_err) == 0) {
				ci_err = ERRCI_UNKNOWN;
			}
//            es_set_cloud_error_code(ci_err);

			switch (ci_err) {
			case ERRCI_INTERNAL_SERVER_ERROR:
				THINGS_LOG_D(TAG, "[ERRCODE]Cloud Server has a issue, Check your Cloud Server.");
				esm_get_network_status();	// State return
				break;
			case ERRCI_TOKEN_VALIDATION_FAILED:
			case ERRCI_TOKEN_EXPIRED:
				THINGS_LOG_D(TAG, "[ERRCODE]-> AccessToken \"%d\", refresh AccessToken and try again.", ci_err);
				refresh_token_into_cloud();	// refresh accesstoken.
				break;
			default:
				THINGS_LOG_D(TAG, "Not Support This Cloud-Error-Code(%d) Exception", ci_err);
				break;
			}
		}

	}

	return OC_STACK_DELETE_TRANSACTION;
}
#endif

OCStackApplicationResult handle_dev_profile_cb(void *ctx, OCDoHandle handle, OCClientResponse *client_response)
{
	THINGS_LOG_D(TAG, "Device Profile Publish callback received");

	if (ctx != (void *)DEFAULT_CONTEXT_VALUE) {
		THINGS_LOG_D(TAG, "Invalid Publish callback received");
	}

	things_del_all_request_handle();

	THINGS_LOG_D(TAG, "Device Profile Publish response received, code: %s(%d)", get_result(client_response->result), client_response->result);
	if (check_comm_error_retrans(client_response->result) == true) {
		THINGS_LOG_D(TAG, "Device Profile Publish Request-Send is Failed.");
		publish_dev_profile_into_cloud(NULL);
	}

	if (client_response->payload != NULL && client_response->payload->type == PAYLOAD_TYPE_REPRESENTATION) {
		OCRepPayloadValue *val = ((OCRepPayload *) client_response->payload)->values;
		while (val != NULL) {
			if (val->type == OCREP_PROP_INT) {
				THINGS_LOG_D(TAG, "Key: %s, Value: %lld, int", val->name, val->i);
			} else if (val->type == OCREP_PROP_STRING) {
				THINGS_LOG_D(TAG, "Key: %s, Value: %s, string", val->name, val->str);
			} else {
				THINGS_LOG_D(TAG, "Un supported val Type.(0x%d)", val->type);
			}

			val = val->next;
		}

		if (client_response->result != OC_STACK_RESOURCE_CHANGED) {
			ci_error_code_e ci_err = ERRCI_NO_ERROR;

			if (get_cloud_code(client_response, OC_REST_POST, &ci_err) == 0) {
				ci_err = ERRCI_UNKNOWN;
			}
//            es_set_cloud_error_code(ci_err);

			switch (ci_err) {
			case ERRCI_INTERNAL_SERVER_ERROR:
				THINGS_LOG_D(TAG, "[ERRCODE]Cloud Server has a issue, Check your Cloud Server.");
				esm_get_network_status();	// State return
				break;
			case ERRCI_TOKEN_VALIDATION_FAILED:
			case ERRCI_TOKEN_EXPIRED:
				THINGS_LOG_D(TAG, "[ERRCODE]-> AccessToken \"%d\", refresh AccessToken and try again.", ci_err);
				refresh_token_into_cloud();	// refresh accesstoken.
				break;
			default:
				THINGS_LOG_D(TAG, "Not Support This Cloud-Error-Code(%d) Exception", ci_err);
				break;
			}
		}

	}

	return OC_STACK_DELETE_TRANSACTION;
}

OCStackApplicationResult handle_find_resource_cb(void *ctx, OCDoHandle handle, OCClientResponse *client_response)
{
	if (ctx == (void *)DEFAULT_CONTEXT_VALUE) {
		THINGS_LOG_D(TAG, "Callback Context for DISCOVER query recvd successfully");
	}

	if (client_response) {
		THINGS_LOG_D(TAG, "StackResult: %s(%d)", get_result(client_response->result), client_response->result);

		//char* connectionType = getConnectivityType (client_response->connType);
		//THINGS_LOG_D(TAG, "Discovered on %s", connectionType.c_str());
		THINGS_LOG_D(TAG, "Device =============> Discovered @ %s:%d", client_response->devAddr.addr, client_response->devAddr.port);

		if (client_response->payload != NULL && client_response->payload->type == PAYLOAD_TYPE_REPRESENTATION) {
			OCRepPayloadValue *val = ((OCRepPayload *) client_response->payload)->values;
			while (val != NULL) {
				if (val->type == OCREP_PROP_INT) {
					THINGS_LOG_D(TAG, "Key: %s, Value: %lld, int", val->name, val->i);
				} else if (val->type == OCREP_PROP_STRING) {
					THINGS_LOG_D(TAG, "Key: %s, Value: %s, string", val->name, val->str);
				} else {
					THINGS_LOG_D(TAG, "Un supported val Type.(0x%d)", val->type);
				}

				val = val->next;
			}
		}

		if (client_response->result != OC_STACK_OK) {
			THINGS_LOG_V(TAG, "Refresh Token process Start.");
			refresh_token_into_cloud();
		}

	} else {
		THINGS_LOG_V(TAG, "handle_find_resource_cb received Null client_response");
	}

	return OC_STACK_DELETE_TRANSACTION;
}

OCStackApplicationResult handle_publish_topic_cb(void *ctx, OCDoHandle handle, OCClientResponse *client_response)
{
	THINGS_LOG_D(TAG, "Publish Topic callback received");

	OCStackApplicationResult res = OC_STACK_DELETE_TRANSACTION;

	if (ctx != (void *)DEFAULT_CONTEXT_VALUE) {
		THINGS_LOG_D(TAG, "Invalid Publish Topic callback received");
	}

	THINGS_LOG_D(TAG, "Publish Topic response received code: %s(%d)", get_result(client_response->result), client_response->result);

	if (client_response->payload != NULL && client_response->payload->type == PAYLOAD_TYPE_REPRESENTATION) {
		THINGS_LOG_D(TAG, "PAYLOAD_TYPE_REPRESENTATION received");

		OCRepPayloadValue *val = ((OCRepPayload *) client_response->payload)->values;
		while (val) {
			if (val->type == OCREP_PROP_INT) {
				THINGS_LOG_D(TAG, "Key: %s, Value: %lld, int", val->name, val->i);
			} else if (val->type == OCREP_PROP_STRING) {
				THINGS_LOG_D(TAG, "Key: %s, Value: %s, string", val->name, val->str);
			} else {
				THINGS_LOG_D(TAG, "Un supported val Type.(0x%d)", val->type);
			}

			val = val->next;
		}

		if (client_response->result != OC_STACK_RESOURCE_CHANGED) {
			ci_error_code_e ci_err = ERRCI_NO_ERROR;

			if (0 == get_cloud_code(client_response, OC_REST_POST, &ci_err)) {
				ci_err = ERRCI_UNKNOWN;
			}

			switch (ci_err) {
			case ERRCI_INTERNAL_SERVER_ERROR:
				THINGS_LOG_D(TAG, "[ERRCODE]Cloud Server has a issue, Check your Cloud Server.");
				esm_get_network_status();	// State return
				break;
			case ERRCI_TOKEN_VALIDATION_FAILED:
			case ERRCI_TOKEN_EXPIRED:
				THINGS_LOG_D(TAG, "[ERRCODE] AccessToken \"%d\", refresh AccessToken and try again.", ci_err);
				refresh_token_into_cloud();	// refresh accesstoken.
				break;
			default:
				THINGS_LOG_D(TAG, "Not Support This Cloud-Error-Code(%d) Exception", ci_err);
				//esm_get_network_status();  // Don't need to sign out on this session
				break;
			}
		}

		res = OC_STACK_DELETE_TRANSACTION;
	}

	return res;
}

static OCStackResult publish_resource_main_dev_into_cloud(things_resource_s **list, int length, things_timeout_s *timeout)
{
	es_error_code_e es_err = ES_ERRCODE_UNKNOWN;
	OCStackResult res = OC_STACK_ERROR;

	if (list == NULL || length < 1) {
		THINGS_LOG_E(TAG, "Invalid Parameter Inserted");
		es_cloud_state_set_and_notify(ES_STATE_FAILED_TO_PUBLISH_RESOURCES_TO_CLOUD, ER_ERRCODE_SYSTEM_ERROR, NULL, NULL);
		return res;
	} else {
		if (es_cloud_state_set_and_notify(ES_STATE_PUBLISHING_RESOURCES_TO_CLOUD, ES_ERRCODE_NO_ERROR, NULL, NULL) != 0) {
			THINGS_LOG_D(TAG, "StateAndNotify is failed.");
			return res;
		}

		if (ci_connection_pre_check(NULL, NULL) == 0) {
			res = things_cloud_rsc_publish(g_cloud_address, list, length, handle_main_dev_publish_cb, handle_publish_timeout, timeout);
		} else {
			THINGS_LOG_V(TAG, "AP is not connected to internet.");
			es_err = ES_ERRCODE_NO_INTERNETCONNECTION;
		}

		if (res != OC_STACK_OK) {
			THINGS_LOG_D(TAG, "Unable to publish user resources to cloud : %d", res);

			if (es_cloud_state_set_and_notify(ES_STATE_FAILED_TO_PUBLISH_RESOURCES_TO_CLOUD, es_err, NULL, NULL) != 0) {
				THINGS_LOG_D(TAG, "StateAndNotify is failed.");
				return res;
			}

			return res;
		}
	}

	return res;
}

OCStackResult publish_resource_into_cloud(rp_target_e target, things_timeout_s *timeout)
{
	THINGS_LOG_D(TAG, "Enter.");

#ifdef CONFIG_ST_THINGS_SUPPORT_SUB_DEVICE
	int sub_dev_pub_fail = -1;
#endif
	OCStackResult res = OC_STACK_OK;

	if (target == RSC_PUB_ALL || target == RSC_PUB_MAIN_ONLY) {
		THINGS_LOG_D(TAG, "Main-Device Resource Publish Start.");
		res = publish_resource_main_dev_into_cloud(g_resource_lists, g_len, timeout);
		usleep(10000);			// 1 device resource-publish per 10 msec.
	}

#ifdef CONFIG_ST_THINGS_SUPPORT_SUB_DEVICE
	if (res == OC_STACK_OK && target != RSC_PUB_MAIN_ONLY) {
		THINGS_LOG_D(TAG, "Sub-Devices Resource Publish Start.");
		int device_cnt = 0;
		char *device_id = NULL;
		st_device_s **dList = NULL;

		if (1 == dm_get_device_information(&device_cnt, &dList) && device_cnt > 1) {
			for (int index = 1; index < device_cnt; index++) {
				// Get device id of hosted ones..
				if (dList[index]->is_physical == 1) {
					if (sub_dev_pub_fail == -1) {
						sub_dev_pub_fail = 0;
					}

					device_id = dList[index]->device_id;

					THINGS_LOG_D(TAG, "Sub-device ID=%s", device_id);
					if (device_id == NULL || device_id[0] == 0) {
						THINGS_LOG_D(TAG, "not exist device ID. So, skip publish resource.");
						sub_dev_pub_fail = 1;
						continue;
					}

					OCStackResult result = things_cloud_rsc_publish_with_device_id(g_cloud_address,
										   device_id,
										   dList[index]->pchild_resources,
										   dList[index]->capa_cnt,
										   handle_sub_dev_publish_cb,
										   handle_publish_timeout,
										   timeout);
					if (result != OC_STACK_OK) {
						sub_dev_pub_fail = 1;
					}

					device_id = NULL;
					usleep(10000);	// 1 device resource-publish per 10 msec.
				}
			}
		}

		if (dList) {
			things_free(dList);
			dList = NULL;
		}

		if (target == RSC_PUB_SUB_ALL || target == RSC_PUB_NEED_SUB_ONLY) {
			switch (sub_dev_pub_fail) {
			case 1:			// occurred error
				THINGS_LOG_V(TAG, "Sending-request is failed for Sub-device publish.");
				res = OC_STACK_ERROR;
				break;
			case -1:			// not exist sub-dev
				THINGS_LOG_V(TAG, "Not exist sub-device publish.");
				res = OC_STACK_ERROR;
				break;
			case 0:
				THINGS_LOG_V(TAG, "Sending-request is Success for Sub-device publish.");
				res = OC_STACK_OK;
				break;
			}
		}
	}
#endif

	THINGS_LOG_D(TAG, "Exit.");
	return res;
}

OCStackResult log_in_out_to_cloud(bool value, things_timeout_s *timeout)
{
	int port = 0;
	char *ci_ip = NULL;
	char *ci_domian = NULL;
	es_error_code_e es_err = ES_ERRCODE_UNKNOWN;
	OCStackResult res = OC_STACK_ERROR;
	OCClientResponseHandler callback = NULL;
	things_check_time_out_call_func calltimeout = NULL;

	if (signed_up_data == NULL || signed_up_data->access_token == NULL || strlen(signed_up_data->access_token) < 1) {
		THINGS_LOG_E(TAG, "No Session Key Retrived from the Cloud ");
		return res;
	}

	if (signed_up_data->domain != NULL && (signed_up_data->port == NULL || signed_up_data->port[0] == 0)) {
		THINGS_LOG_E(TAG, "domain=%s but, not exist port number.(%s)", signed_up_data->domain, signed_up_data->port);
		return res;
	}

	if (value == true) {
		callback = handle_login_cb;
		calltimeout = handle_signin_timeout;
		if (es_cloud_state_set_and_notify(ES_STATE_REGISTERING_TO_CLOUD, ES_ERRCODE_NO_ERROR, NULL, NULL) != 0) {
			THINGS_LOG_D(TAG, "StateAndNotify is failed.");
			return res;
		}

		if (signed_up_data->domain != NULL) {
			ci_domian = things_strdup(signed_up_data->domain);
		}
		THINGS_LOG_D(TAG, "*********** Start Sign-IN ********** ci_domian : %s", ci_domian);
	} else {
		callback = handle_logout_cb;
		calltimeout = NULL;
		g_qis_cloud_thread_running = CISESS_SIGNOUT;
		if (es_cloud_state_set_and_notify(ES_STATE_LOGOUT_FROM_CLOUD, ES_ERRCODE_NO_ERROR, NULL, NULL) != 0) {
			THINGS_LOG_D(TAG, "StateAndNotify is failed.");
			return res;
		}
		THINGS_LOG_D(TAG, "*********** Start Sign-OUT **********");
	}

	if (ci_connection_pre_check(ci_domian, &ci_ip) == 0) {
		if (ci_ip != NULL) {
			if (make_cloud_address(ci_ip, signed_up_data->port, NULL) == NULL) {
				THINGS_LOG_E(TAG, "Making CloudAddress is failed.");
				goto GOTO_OUT;
			}
			things_strncpy(g_cloud_ip, ci_ip, IP_PORT);
			things_strncpy(g_cloud_port, signed_up_data->port, IP_PORT);

			port = atoi(g_cloud_port);
			if (g_cloud_ip[0] == NULL || g_cloud_port[0] == NULL || 0 >= port || port > 65535) {
				THINGS_LOG_E(TAG, "Cloud info is invalid.(g_cloud_ip=%s, g_cloud_port=%s, port=%d)", g_cloud_ip, g_cloud_port, port);
				goto GOTO_OUT;
			}
		}

		if (g_cloud_address[0] == 0) {
			THINGS_LOG_E(TAG, "g_cloud_address is invalid.(0x%X)", g_cloud_address);
			goto GOTO_OUT;
		}

		THINGS_LOG_V(TAG, "Cloud Addr : %s", g_cloud_address);
		THINGS_LOG_V(TAG, "device ID : %s", OCGetServerInstanceIDString());
		THINGS_LOG_V(TAG, "access_token : %s", signed_up_data->access_token);
		THINGS_LOG_V(TAG, "refresh token : %s", signed_up_data->refresh_token);
		THINGS_LOG_V(TAG, "user ID : %s", signed_up_data->uid);
		THINGS_LOG_V(TAG, "toke type : %s", signed_up_data->token_type);
		THINGS_LOG_V(TAG, "expire Time : %lld", signed_up_data->expire_time);
		THINGS_LOG_V(TAG, "certificate : %s", signed_up_data->certificate);
		THINGS_LOG_V(TAG, "redirect URI : %s", signed_up_data->redirect_uri);
		THINGS_LOG_V(TAG, "sid : %s", signed_up_data->sid);

		if (value == true) {
			things_ping_set_mask(g_cloud_ip, port, PING_ST_ISCLOUD);
		} else {
			things_ping_unset_mask(g_cloud_ip, PING_ST_ISCLOUD | PING_ST_SIGNIN | PING_ST_TCPCONNECT);
		}

		res = things_cloud_session(g_cloud_address, signed_up_data->uid, OCGetServerInstanceIDString(), signed_up_data->access_token, value, callback, calltimeout, timeout);

		THINGS_LOG_D(TAG, "OCCloudLogInOut return : %d", res);
	} else {
		THINGS_LOG_V(TAG, "AP is not connected to internet.");
		es_err = ES_ERRCODE_NO_INTERNETCONNECTION;
	}

GOTO_OUT:
	if (ci_ip != NULL) {
		things_free(ci_ip);
		ci_ip = NULL;
	}

	if (ci_domian != NULL) {
		things_free(ci_domian);
		ci_domian = NULL;
	}

	if (res != OC_STACK_OK) {
		if (value == true) {
			things_ping_unset_mask(g_cloud_ip, PING_ST_ISCLOUD);

			THINGS_LOG_E(TAG, "Cloud Server LogIn Failed : %d", res);
			if (es_cloud_state_set_and_notify(ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, es_err, NULL, NULL) != 0) {
				THINGS_LOG_D(TAG, "StateAndNotify is failed.");
				return res;
			}
		} else {
			THINGS_LOG_E(TAG, "Cloud Server LogOut Failed : %d", res);
			esm_get_network_status();
			return res;
		}
	}

	return res;
}

/**
 * @brief                 Call function to send request for Finding myself-resource to Cloud.
 * @return                Success return OC_STACK_OK (0) \n
 *                        Failed  return other OCStackResult value (-)
 */
OCStackResult find_cloud_resource(void)
{
	OCStackResult ret = OC_STACK_ERROR;

	char *sz_query_uri = NULL;
	int length = MAX_CI_ADDRESS;
	int length_query = 0;

	length += strlen(RESOURCE_DISCOVERY_QUERY_DI);
	length += strlen(OCGetServerInstanceIDString());
	length_query = sizeof(char) * (length + 2);

	if ((sz_query_uri = (char *)things_malloc(length_query)) == NULL) {
		THINGS_LOG_E(TAG, "sz_query_uri memory allocation is failed.");
		return OC_STACK_ERROR;
	}

	memset(sz_query_uri, 0, length_query);
	if (things_strcat(sz_query_uri, length_query, g_cloud_address) == NULL) {
		ret = OC_STACK_NO_MEMORY;
	}
	if (things_strcat(sz_query_uri, length_query, RESOURCE_DISCOVERY_QUERY_DI) == NULL) {
		ret = OC_STACK_NO_MEMORY;
	}
	if (things_strcat(sz_query_uri, length_query, OCGetServerInstanceIDString()) == NULL) {
		ret = OC_STACK_NO_MEMORY;
	}
	THINGS_LOG_D(TAG, "Find Resource to Cloud URI : %s", sz_query_uri);

	OCCallbackData cb_data;
	cb_data.cb = handle_find_resource_cb;
	cb_data.context = (void *)DEFAULT_CONTEXT_VALUE;
	cb_data.cd = NULL;

	if (ret != OC_STACK_NO_MEMORY) {
		ret = OCDoResource(NULL, OC_REST_DISCOVER, sz_query_uri, NULL, 0, CT_DEFAULT, OC_MEDIUM_QOS, &cb_data, NULL, 0);
	}

	if (ret != OC_STACK_OK) {
		THINGS_LOG_E(TAG, "Find Resource in Cloud is failed.");
	}

	things_free(sz_query_uri);

	return ret;
}

static OCStackResult register_server_into_cloud(es_cloud_prov_data_s *event_data, things_timeout_s *timeout)
{
	OCStackResult res = OC_STACK_ERROR;

	if (event_data == NULL || (event_data->auth_code[0] == 0 && event_data->accesstoken[0] == 0)) {
		THINGS_LOG_E(TAG, "Invalid event_data.");
		return res;
	}

	THINGS_LOG_D(TAG, "AuthCode     : %s", event_data->auth_code);
	THINGS_LOG_D(TAG, "Access Token : %s", event_data->accesstoken);

	if (signed_up_data != NULL) {
		es_cloud_signup_clear(signed_up_data);
		signed_up_data = NULL;
	}

	if (g_cloud_ip[0] == NULL || g_cloud_port[0] == NULL) {
		THINGS_LOG_E(TAG, "g_cloud_ip is NULL or g_cloud_port is NULL");
		return res;
	}
	int port = atoi(g_cloud_port);
	if (0 >= port || port > 0xffff) {
		THINGS_LOG_E(TAG, "Cloud info is invalid.(port=%d)", port);
		return res;
	}

	if (es_cloud_state_set_and_notify(ES_STATE_REGISTERING_TO_CLOUD, ES_ERRCODE_NO_ERROR, NULL, NULL) != 0) {
		THINGS_LOG_E(TAG, "StateAndNotify is failed.");
		return res;
	}

	things_ping_set_mask(g_cloud_ip, (uint16_t) port, PING_ST_ISCLOUD);

	// Get Session Key
	res = things_cloud_signup(g_cloud_address, OCGetServerInstanceIDString(), event_data, handle_register_cb, handle_signup_timeout, timeout);

	if (res != OC_STACK_OK) {
		THINGS_LOG_E(TAG, "Cloud Server Registration Failed : %d", res);
		things_ping_unset_mask(g_cloud_ip, PING_ST_ISCLOUD);
		if (es_cloud_state_set_and_notify(ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, ES_ERRCODE_UNKNOWN, NULL, NULL) != 0) {
			THINGS_LOG_D(TAG, "StateAndNotify is failed.");
			return res;
		}
	}

	return res;
}

OCStackResult refresh_token_into_cloud(void)
{
	OCStackResult res = OC_STACK_ERROR;
	if (signed_up_data == NULL || signed_up_data->access_token == NULL || strlen(signed_up_data->access_token) < 1) {
		THINGS_LOG_E(TAG, "No Session Key Retrived from the Cloud ");
	} else {
		res = things_cloud_refresh(g_cloud_address, signed_up_data->uid, OCGetServerInstanceIDString(), signed_up_data->refresh_token, handle_refresh_token_cb);

		THINGS_LOG_D(TAG, "OCCloud Refresh Token return : %d", res);
	}

	if (res != OC_STACK_OK) {
		THINGS_LOG_E(TAG, "Cloud access_token refresh Failed : %d", res);
	}

	return res;
}

int push_notification_to_cloud(const char *uri, OCRepPayload *payload)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	int ret = 0;

	if (things_get_reset_mask(RST_NOTI_MODULE_DISABLE) == true) {
		THINGS_LOG_V(TAG, "Notification Module Disable.");
		ret = 0;
	} else if (NULL != payload) {
		if (OC_STACK_OK == things_cloud_topic_publish_topic(g_cloud_address, uri, payload, handle_publish_topic_cb)) {
			ret = 1;
		}
	}

	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);

	return ret;
}

static char *make_cloud_address(char *ip, char *port, const char *ci_addr)
{
	char *ipport = (char *)ci_addr;
	bool is_ci_addr_null = true;
	THINGS_LOG_D(TAG, "ip=%s, port=%s, ci_addr=%s", ip, port, ci_addr);

	if (ci_addr != NULL && strlen(ci_addr) > 0) {	// ci address.
		is_ci_addr_null = false;
		char *point = strstr(ci_addr, DEFAULT_COAP_TCP_HOST);
		if (point) {
			ipport = point + strlen(DEFAULT_COAP_TCP_HOST);
		}
	} else if ((ipport = make_ip_port(ip, port)) == NULL) {	// ip , port coupling.
		return NULL;
	}

	// Update Cloud Address.
	memset(g_cloud_address, 0, MAX_CI_ADDRESS);
	if (things_strcat(g_cloud_address, MAX_CI_ADDRESS, DEFAULT_COAP_TCP_HOST) == NULL) {
		if (is_ci_addr_null) {
			things_free(ipport);
		}
		THINGS_LOG_E(TAG, "things_strcat() is failed.");
		return NULL;
	}

	if (things_strcat(g_cloud_address, MAX_CI_ADDRESS, ipport) == NULL) {
		if (is_ci_addr_null) {
			things_free(ipport);
		}
		THINGS_LOG_E(TAG, "things_strcat() is failed.");
		return NULL;
	}

	if (is_ci_addr_null) {
		things_free(ipport);
	}

	return g_cloud_address;
}

static int start_ci_connection(const char *cloud_adress, es_cloud_prov_data_s *event_data, things_resource_s **list, int length, things_timeout_s *timeout)
{
	int ret = 0;
	OCStackResult result = OC_STACK_ERROR;

	g_resource_lists = list;
	g_len = length;

	if (make_cloud_address(NULL, NULL, cloud_adress) == NULL) {
		THINGS_LOG_E(TAG, "Making Cloud Address is failed.");
		result = OC_STACK_NO_MEMORY;
	}

	THINGS_LOG_D(TAG, "CI Svr Addr         : %s", g_cloud_address);
	THINGS_LOG_D(TAG, "CI Svr AuthCode     : %s", event_data->auth_code);
	THINGS_LOG_D(TAG, "CI Svr Accesstoken  : %s", event_data->accesstoken);
	THINGS_LOG_D(TAG, "CI Svr Uid          : %s", event_data->uid);
	THINGS_LOG_D(TAG, "CI Svr AuthProvider : %s", event_data->auth_provider);
	THINGS_LOG_D(TAG, "CI Svr client_id     : %s", event_data->client_id);

	if (timeout != NULL) {
		THINGS_LOG_D(TAG, "CI Svr timeout_cnt     : %d", timeout->cur_counter);
		THINGS_LOG_D(TAG, "CI Svr timeout_cur_num : %d", timeout->cur_num);
		THINGS_LOG_D(TAG, "CI Svr timeout_ori_num : %d", timeout->ori_num);
	}

	if (result != OC_STACK_NO_MEMORY) {
		if (event_data != NULL && (event_data->auth_code[0] != 0 || event_data->accesstoken[0] != 0)) {
			result = register_server_into_cloud(event_data, timeout);
		} else {
			if (es_cloud_signup_init(&signed_up_data) == true && (ret = dm_load_legacy_cloud_data(&signed_up_data)) == 1) {
				result = log_in_out_to_cloud(true, timeout);
			} else {
				THINGS_LOG_V(TAG, "Sign-UP data load failed.");
			}
		}
	}

	if (OC_STACK_OK != result) {
		THINGS_LOG_E(TAG, "start_ci_connection() is failed.");
	} else {
		ret = 1;				// Success.
	}

	return ret;
}

static void *ci_connection_init_loop(es_cloud_event_timeout_s *param)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	int res = 0;
	char *ci_ip = NULL;
	char *ci_ip_port = NULL;
	char *ci_host = NULL;
	es_error_code_e es_err = ES_ERRCODE_UNKNOWN;

	ci_cp_del_is_there_cp();

	if (param == NULL) {
		THINGS_LOG_E(TAG, "[Error] parameter is NULL.");

		if (es_cloud_state_set_and_notify(ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, ER_ERRCODE_SYSTEM_ERROR, NULL, NULL) != 0) {
			THINGS_LOG_D(TAG, "StateAndNotify is failed.");
		}

		force_session_stop(CISESS_NULL);
		return NULL;
	}

	es_cloud_prov_data_s *event_data = &param->event;

	if (event_data->host_name[0] != 0) {
		ci_host = &(event_data->host_name[0]);
	} else if (event_data->ip[0] != 0) {
		ci_host = &(event_data->ip[0]);
	}

	THINGS_LOG_D(TAG, "* param add= 0x%X *", param);
	THINGS_LOG_D(TAG, "* ci_host   = %s *", ci_host);

	// Cross check if a network is available
	if (esm_get_network_status()) {
		if (g_server_builder != NULL) {
			THINGS_LOG_V(TAG, "##########################");
			THINGS_LOG_V(TAG, "  Start Cloud Connection ");
			THINGS_LOG_V(TAG, "##########################");

			if (ci_connection_pre_check(ci_host, &ci_ip) == 0) {
				ci_ip_port = make_ip_port(ci_ip, event_data->port);

				things_strncpy(g_cloud_ip, ci_ip, IP_PORT);
				things_strncpy(g_cloud_port, event_data->port, IP_PORT);

				pthread_mutex_lock(&g_es_mutex);
				res = start_ci_connection(dm_get_things_cloud_address(ci_ip_port), event_data, g_server_builder->gres_arr, g_server_builder->res_num, &param->timeOut);
				switch (res) {
				case 1:		// Success
					usleep(500000);	// 0.5 sec wait (need for Cloud CI Server cooling.)
					break;
				case 0:		// Fail start_ci_connection.
					es_err = ES_ERRCODE_UNKNOWN;
					break;
				case -1:		// Json Parsing error. or Memory Alloc error. or Invalid argument error.
					es_err = ER_ERRCODE_SYSTEM_ERROR;
					break;
				case -2:		// Invalid Contents of Json file.
					es_err = ER_ERRCODE_INVALID_SAVED_CLOUD_DATA;
					break;
				}
				THINGS_LOG_D(TAG, "res = %d, es_err = %d", res, es_err);
				pthread_mutex_unlock(&g_es_mutex);

				if (ci_ip_port) {
					things_free(ci_ip_port);
					ci_ip_port = NULL;
				}
				if (ci_ip) {
					things_free(ci_ip);
					ci_ip = NULL;
				}
			} else {
				THINGS_LOG_E(TAG, "AP is Not Connected to Internet.");
				es_err = ES_ERRCODE_NO_INTERNETCONNECTION;
			}
		} else {
			THINGS_LOG_E(TAG, "ServerBuilder is NULL.");
			es_err = ER_ERRCODE_SYSTEM_ERROR;
		}
	} else {
		THINGS_LOG_E(TAG, "Device is not connected to AP.");
		es_err = ES_ERRCODE_UNKNOWN;
	}

	if (res != 1) {
		THINGS_LOG_E(TAG, "###################################");
		THINGS_LOG_E(TAG, "  Exiting without Cloud Connection ");
		THINGS_LOG_E(TAG, "###################################");

		if (es_cloud_state_set_and_notify(ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, es_err, NULL, NULL) != 0) {
			THINGS_LOG_E(TAG, "StateAndNotify is failed.");
		}

		force_session_stop(CISESS_NULL);
	}

	things_free(param);
	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);

	return NULL;
}

static void *ci_connection_waiting_loop(es_cloud_event_timeout_s *param)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);
	THINGS_LOG_D(TAG, "Sleep before cloud_retry_sign_up");
	sleep(1);
	if (param == NULL) {
		THINGS_LOG_E(TAG, "[Error] parameter is NULL.");
		ci_cp_del_is_there_cp();
		ci_cp_del_pended_data();
		es_cloud_state_set_and_notify(ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, ER_ERRCODE_SYSTEM_ERROR, NULL, NULL);
		esm_get_network_status();	// State return
		return NULL;
	}

	while (g_qis_cloud_thread_running != CISESS_STOP) {
		THINGS_LOG_D(TAG, "Waiting ...");
		usleep(100000);
	}

	ci_cp_pend_event_data(&(param->event));
	g_qis_cloud_thread_running = CISESS_BUSY;
	THINGS_LOG_D(TAG, "Create CloudInit thread");

	ci_connection_init_loop(param);

	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);

	return NULL;
}

void *cloud_data_cb_esm(es_cloud_prov_data_s *event_data)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	THINGS_LOG_D(TAG, "g_qis_cloud_thread_running = %d", g_qis_cloud_thread_running);

	pthread_t cthread_handler;
	es_cloud_event_timeout_s *cloned_data = NULL;
#if 0							// pkcloud
	if (ci_cp_cas_is_there_cp_if_false() == true) {
		THINGS_LOG_D(TAG, "Already exist data of Cloud Provisioning.");
		return NULL;
	}
#endif
	switch (g_qis_cloud_thread_running) {
	case CISESS_APDISCON:
		// Backup loose try..
		ci_cp_pend_event_data(event_data);
		return NULL;
	case CISESS_BUSY:			// Occupied
	case CISESS_STOP_TRIGGER:	// Re-Start triggered.
	case CISESS_SIGNOUT:		// doing Auto Log-Out
		break;
	case CISESS_STOP:			// can start.
		things_del_all_request_handle();
		send_cnt_sign_up = 0;
		ci_cp_pend_event_data(event_data);
		g_qis_cloud_thread_running = CISESS_BUSY;

		if ((cloned_data = clone_data_add_timeout(event_data, NULL)) != NULL) {
			int retp = pthread_create_rtos(&cthread_handler, NULL, (pthread_func_type) ci_connection_init_loop, (void *)cloned_data, THINGS_STACK_CICONNETION_INIT_THREAD);
			if (retp != 0) {
				THINGS_LOG_E(TAG, "Create thread is failed.");
				things_free(cloned_data);
				cloned_data = NULL;
				g_qis_cloud_thread_running = CISESS_STOP;
				es_cloud_state_set_and_notify(ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, ER_ERRCODE_SYSTEM_ERROR, NULL, NULL);
			}
		} else {
			THINGS_LOG_E(TAG, "clone_data_add_timeout is failed.");
		}
		return NULL;
	default:
		THINGS_LOG_E(TAG, "Not Surpport value(%d) of g_qis_cloud_thread_running.", g_qis_cloud_thread_running);
		return NULL;
	}

	things_del_all_request_handle();
	send_cnt_sign_up = 0;

	if (cloud_retry_sign_up(event_data, NULL) == 0) {
		THINGS_LOG_E(TAG, "cloud_retry_sign_up is failed.");
		ci_cp_del_pended_data();
		ci_cp_del_is_there_cp();
		es_cloud_state_set_and_notify(ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, ER_ERRCODE_SYSTEM_ERROR, NULL, NULL);
		esm_get_network_status();	// State return
	}
	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);

	return NULL;
}

int cloud_retry_sign_in(things_timeout_s *timeout)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	pthread_t cthread_handler;
	es_cloud_prov_data_s dummy_data;
	es_cloud_event_timeout_s *cloned_data = NULL;

	if (ci_cp_get_is_there_cp() == true) {
		THINGS_LOG_D(TAG, "There is a CloudProvisioning data. Cloud-retry is skiped.");
		return 1;
	}

	force_session_stop(CISESS_NULL);

	// Need to check the running state of the thread...
	if (g_qis_cloud_thread_running == CISESS_STOP) {
		g_qis_cloud_thread_running = CISESS_BUSY;
		THINGS_LOG_D(TAG, "Create CloudInit thread");

		// cloud_data setting.
		init_es_cloud_prov_data(&dummy_data);

		if ((cloned_data = clone_data_add_timeout(&dummy_data, timeout)) == NULL || pthread_create_rtos(&cthread_handler, NULL, (pthread_func_type) ci_connection_init_loop, (void *)cloned_data, THINGS_STACK_CICONNETION_INIT_THREAD) != 0) {
			THINGS_LOG_E(TAG, "Create thread is failed.");
			things_free(cloned_data);
			cloned_data = NULL;
			g_qis_cloud_thread_running = CISESS_STOP;
			es_cloud_state_set_and_notify(ES_STATE_FAILED_TO_REGISTER_TO_CLOUD, ER_ERRCODE_SYSTEM_ERROR, NULL, NULL);
		}
	}

	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);

	return 0;
}

static int cloud_retry_sign_up(es_cloud_prov_data_s *event_data, things_timeout_s *timeout)
{
	THINGS_LOG_D(TAG, "Enter.");

	pthread_t cthread_handler;
	es_cloud_event_timeout_s *cloned_data = NULL;

	if (event_data == NULL) {
		THINGS_LOG_E(TAG, "event_data is NULL.");
		return 0;
	}

	g_qis_cloud_thread_running = CISESS_STOP_TRIGGER;
	esm_get_network_status();	// State return

	if ((cloned_data = clone_data_add_timeout(event_data, timeout)) == NULL || pthread_create_rtos(&cthread_handler, NULL, (pthread_func_type) ci_connection_waiting_loop, (void *)cloned_data, THINGS_STACK_CICONNETION_WAIT_THREAD) != 0) {
		THINGS_LOG_E(TAG, "Create thread is failed.");
		things_free(cloned_data);
		cloned_data = NULL;
		return 0;
	}

	THINGS_LOG_D(TAG, "Exit.");
	return 1;
}

static void cloud_request_retry_trigger(things_timeout_s *timeout)
{
	// Stored Backup data is base on time-out.
	es_cloud_prov_data_s *pendedEvent = NULL;
	if ((pendedEvent = ci_cp_get_pended_data()) != NULL) {
		ci_cp_del_is_there_cp();
		cloud_data_cb_esm(pendedEvent);	// 1. Last Cloud Provisioning-ed data. or Last Send Failed data.(Sign-UP)
	} else {
		cloud_retry_sign_in(timeout);	// default. Cloud retry. (Sign-IN / Resource Publish failed data)
	}
}

static es_cloud_prov_data_s *if_failed_then_retry(OCDoHandle handle, OCStackResult result, int *n_err)
{
	THINGS_LOG_D(TAG, "latestRequest 0x%X, RespCB:0x%X]", things_get_request_handle(), handle);

	es_cloud_prov_data_s *pended_data = NULL;
	es_cloud_prov_data_s *clone_data = NULL;
	*n_err = 0;

	ci_cp_enter_pend_data_block();
	if (things_cas_request_handle(handle, NULL) == 1) {
		if (result == OC_STACK_COMM_ERROR) {	// Don't Send Request Message.
			if (g_qis_cloud_thread_running != CISESS_APDISCON) {
				cloud_request_retry_trigger(NULL);
			}
			*n_err = 1;			// communication error.
		} else {
			things_del_all_request_handle();

			if ((pended_data = ci_cp_get_pended_data()) == NULL) {
				THINGS_LOG_V(TAG, "Not exist Pended Event Data.");
				*n_err = 2;		// not exist pended_data.
				goto GOTO_OUT;
			}

			if ((clone_data = things_malloc(sizeof(es_cloud_prov_data_s))) == NULL) {
				THINGS_LOG_E(TAG, "memory allocation is failed for clone_data.");
				*n_err = 3;		// mem allocation error.
				goto GOTO_OUT;
			}

			clone_es_cloud_prov_data(clone_data, pended_data);
		}
	} else {
		THINGS_LOG_E(TAG, "Response Handle(0x%X) is not exist in the Request Handle list.", handle);
		*n_err = 4;
	}

GOTO_OUT:
	ci_cp_release_penddata_block();

	if (*n_err != 1 && *n_err != 4) {
		ci_cp_del_pended_data();
	}

	return clone_data;
}

void ci_stop_cloud_connection(void *CBfunc)
{
	force_session_stop(CISESS_NULL);
	esm_get_network_status();	// State return
	sleep(1);

	things_res_cb_function = NULL;
	if (CBfunc) {
		things_res_cb_function = (things_cloud_con_result_func_type *) CBfunc;
	}
}
static void force_session_stop(ci_session_level_e state)
{
	things_del_all_request_handle();
	if (state == CISESS_SIGNOUT) {
		g_qis_cloud_thread_running = CISESS_SIGNOUT;
	} else {
		g_qis_cloud_thread_running = CISESS_STOP_TRIGGER;
	}
	esm_get_network_status();	// State return
}

int ci_retry_stop_by_wifi_cb(bool is_retry)
{
	if (is_retry == true) {		// Last Failed Cloud Connection Retry. (Permanent setting)
		things_del_all_request_handle();
		g_qis_cloud_thread_running = CISESS_STOP;
		cloud_request_retry_trigger(NULL);
	} else if (is_retry == false) {	// if Cloud Session valid, then Stop Cloud Connection. And Don't Retry.
		things_del_all_request_handle();
		g_qis_cloud_thread_running = CISESS_APDISCON;
		esm_get_network_status();	// State return
	}

	return 1;
}

// -1 : System Error.  0 : No process.  1 : Sign-IN.
int ci_retry_stop_by_tcp_cb(const char *addr_ip, const int port)
{
	int ret = 1;

	pthread_mutex_lock(&g_es_tcp_session_mutex);

	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	int ci_port = 0;

	if (g_qis_cloud_thread_running != CISESS_BUSY || g_cloud_port[0] == 0 || g_cloud_ip[0] == 0) {
		THINGS_LOG_D(TAG, "Don't need something process. IP=%s, Port=%s, RunningState=%d", g_cloud_ip, g_cloud_port, g_qis_cloud_thread_running);
		ret = 0;
		goto GOTO_OUT;
	}

	THINGS_LOG_D(TAG, "IP = %s, port = %d", addr_ip, port);	// I expect that ( IP = 52.37.99.111, port = 443 )

	if ((ci_port = atoi(g_cloud_port)) <= 0) {
		THINGS_LOG_E(TAG, "[Error] Cloud Port is not invalid.(g_cloud_port = %s = %d)", g_cloud_port, ci_port);
		ret = -1;
		goto GOTO_OUT;
	}

	if (strncmp(g_cloud_ip, addr_ip, strlen(addr_ip)) != 0 || ci_port != port) {
		THINGS_LOG_V(TAG, "We Support IP:Port is \"%s:%s\"", g_cloud_ip, g_cloud_port);
		if (strncmp(g_cloud_ip, addr_ip, strlen(addr_ip)) != 0) {
			THINGS_LOG_D(TAG, "Delete OIC_Ping for IP=%s.(Cur_Cloud=%s)", addr_ip, g_cloud_ip);
			things_ping_unset_mask(addr_ip, PING_ST_ISCLOUD);
		}
		ret = 0;
		goto GOTO_OUT;
	}
	// Case Cloud Connection is BUSY & AP Connected.
	if (es_get_cloud_login_state() == false) {
		THINGS_LOG_V(TAG, "process Re-Transmission in Time-Out Process instated of this.");
		ret = 0;
		goto GOTO_OUT;
	}
	// Force Strong Re Sign-IN or new Sign-UP start.
	force_session_stop(CISESS_SIGNOUT);
	cloud_request_retry_trigger(NULL);

GOTO_OUT:
	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
	pthread_mutex_unlock(&g_es_tcp_session_mutex);

	return ret;
}

bool es_cloud_session_stop_trigger(things_es_enrollee_state_e es_state)
{
	if (g_qis_cloud_thread_running != CISESS_APDISCON) {
		if ((g_qis_cloud_thread_running == CISESS_STOP_TRIGGER || es_state <= ES_STATE_CONNECTED_TO_ENROLLER)
			&& g_qis_cloud_thread_running != CISESS_SIGNOUT && es_get_cloud_login_state() == true) {
			THINGS_LOG_D(TAG, "Sign-Out Start.");
			log_in_out_to_cloud(false, NULL);
			return true;
		}

		THINGS_LOG_D(TAG, "es_state=%s, get_enrollee_state()=%s", get_prov_status(es_state), get_prov_status(get_enrollee_state()));

		if (es_state <= ES_STATE_CONNECTED_TO_ENROLLER) {
			if (g_qis_cloud_thread_running != CISESS_BUSY && g_qis_cloud_thread_running != CISESS_APDISCON && es_get_cloud_login_state() == false) {
				THINGS_LOG_D(TAG, "Setting Cloud Semaphore init.");
				g_qis_cloud_thread_running = CISESS_STOP;
			}
		}
	}

	THINGS_LOG_D(TAG, "g_qis_cloud_thread_running=%d", g_qis_cloud_thread_running);

	//THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);

	return false;
}

static int es_cloud_state_set_and_notify(things_cloud_status_e state, es_error_code_e es_err, OCClientResponse *response, ci_error_code_e *ci_err)
{
	const char *strState = get_prov_status(state);

	if (strState == NULL || strncmp(strState, "UNKNOWN", strlen("UNKNOWN")) == 0 || state == ES_STATE_INIT || state == ES_STATE_CONNECTED_TO_ENROLLER) {
		THINGS_LOG_D(TAG, "state is invalid value(%d)", state);
		return -1;
	}

	if (es_set_state(state) == ES_ERROR) {
		THINGS_LOG_D(TAG, "Cloud State(%s) is failed.", strState);
		return 1;
	}

	if (response != NULL && ci_err != NULL) {
		// Method don't care because issue is not occurred by it.
		if (get_cloud_code(response, OC_REST_POST, ci_err) == 0) {
			*ci_err = ERRCI_UNKNOWN;
		}

		es_set_cloud_error_code(*ci_err);
	} else {
		THINGS_LOG_D(TAG, "Provisioning last_err_code = %d", es_err);
		if (es_err == ES_ERRCODE_UNKNOWN) {
			es_set_cloud_error_code(ERRCI_UNKNOWN);
		} else {
			es_set_error_code(es_err);
		}
	}

	prov_rsc_notify_all_observers();

	switch (state) {
	case ES_STATE_FAILED_TO_REGISTER_TO_CLOUD:
	case ES_STATE_FAILED_TO_PUBLISH_RESOURCES_TO_CLOUD:
		ci_finish_cloud_con(0);
		break;
	case ES_STATE_PUBLISHED_RESOURCES_TO_CLOUD:
		ci_finish_cloud_con(1);
		break;
	}

	return 0;
}

void es_cloud_signup_clear(es_cloud_signup_s *data)
{
	if (data == NULL) {
		return;
	}

	if (data->domain != NULL) {
		things_free(data->domain);
		data->domain = NULL;
	}

	if (data->address != NULL) {
		things_free(data->address);
		data->address = NULL;
	}

	if (data->port != NULL) {
		things_free(data->port);
		data->port = NULL;
	}

	if (data->access_token != NULL) {
		things_free(data->access_token);
		data->access_token = NULL;
	}

	if (data->refresh_token != NULL) {
		things_free(data->refresh_token);
		data->refresh_token = NULL;
	}

	if (data->token_type != NULL) {
		things_free(data->token_type);
		data->token_type = NULL;
	}

	if (data->certificate != NULL) {
		things_free(data->certificate);
		data->certificate = NULL;
	}

	if (data->redirect_uri != NULL) {
		things_free(data->redirect_uri);
		data->redirect_uri = NULL;
	}

	if (data->sid != NULL) {
		things_free(data->sid);
		data->sid = NULL;
	}

	if (data->uid != NULL) {
		things_free(data->uid);
		data->uid = NULL;
	}

	data->expire_time = CLOUD_EXPIRESIN_INVALID;

	things_free(data);
}

bool es_cloud_signup_init(es_cloud_signup_s **data)
{
	if (data == NULL) {
		return false;
	}

	if (*data != NULL) {
		es_cloud_signup_clear(*data);
		*data = NULL;
	}

	*data = (es_cloud_signup_s *) things_malloc(sizeof(es_cloud_signup_s));
	if (*data == NULL) {
		THINGS_LOG_D(TAG, "ESCloud_SignedUp struct data is NULL.");
		return false;
	}

	(*data)->domain = NULL;
	(*data)->address = NULL;
	(*data)->port = NULL;
	(*data)->access_token = NULL;
	(*data)->refresh_token = NULL;
	(*data)->certificate = NULL;
	(*data)->expire_time = CLOUD_EXPIRESIN_INVALID;
	(*data)->redirect_uri = NULL;
	(*data)->sid = NULL;
	(*data)->token_type = NULL;
	(*data)->uid = NULL;

	return true;
}

void *es_cloud_init(things_server_builder_s *server_builder)
{
	unsigned int time_v = (unsigned int)time(NULL);

	if (time_v == -1) {
		THINGS_LOG_V(TAG, "Random Number Initialization is Failed.");
	} else {
		THINGS_LOG_D(TAG, "Random Number Initialization is Success.");
		srand(time_v);
	}

	if (things_ping_init() == false) {
		THINGS_LOG_D(TAG, "OICPing init is failed.");
		return NULL;
	}

	things_del_all_request_handle();
	memset(g_cloud_ip, 0, IP_PORT);
	memset(g_cloud_port, 0, IP_PORT);
	g_server_builder = server_builder;
	g_resource_lists = NULL;
	g_len = 0;
//    gDelDeviceCompletFunc = NULL;
	send_cnt_sign_up = 0;
	retranslate_rsc_publish_cnt = 0;

	ci_cp_init_event_pend();

	CARegisterNetworkMonitorHandler((CAAdapterStateChangedCB) things_adapter_state_cb, (CAConnectionStateChangedCB) things_tcp_session_state_cb);

	return (void *)g_server_builder;
}

void es_cloud_terminate(void)
{
	force_session_stop(CISESS_NULL);
	ci_cp_del_pended_data();
	ci_cp_del_is_there_cp();

	CAUnregisterNetworkMonitorHandler((CAAdapterStateChangedCB) things_adapter_state_cb, (CAConnectionStateChangedCB) things_tcp_session_state_cb);

	esm_get_network_status();

	es_cloud_signup_clear(signed_up_data);
	signed_up_data = NULL;
	memset(g_cloud_address, 0, MAX_CI_ADDRESS);
	memset(g_cloud_ip, 0, IP_PORT);
	memset(g_cloud_port, 0, IP_PORT);

	g_server_builder = NULL;
	g_resource_lists = NULL;
	g_len = 0;

//    gDelDeviceCompletFunc = NULL;
	send_cnt_sign_up = 0;
	retranslate_rsc_publish_cnt = 0;

	things_ping_terminate();
}
static int get_cloud_code(OCClientResponse *response, OCMethod method, ci_error_code_e *err)
{
	THINGS_LOG_D(TAG, "Enter");

	int ret = 0;
	OCRepPayloadValue *val = NULL;
	CAResponseResult_t rawResult = CA_EMPTY;

	if (response == NULL || response->payload == NULL || err == NULL || method == OC_REST_NOMETHOD) {
		THINGS_LOG_E(TAG, "response(0x%X) or err(0x%X) is NULL", response, err);
		return ret;
	}

	rawResult = things_result_to_network_result(response->result, method);
	*err = rawResult * ERR_UPPER_UNIT;

	THINGS_LOG_D(TAG, "result = %d, rawResult = %d, method = %d, *err = %d", response->result, rawResult, method, *err);

	val = ((OCRepPayload *) response->payload)->values;

	while (val != NULL) {
		if (strncmp(val->name, CLOUD_ERROR_CODE, strlen(CLOUD_ERROR_CODE)) == 0) {
			if (val->type != OCREP_PROP_INT) {
				THINGS_LOG_E(TAG, "DataType of Cloud-Err-Code is not Integer.");
			} else {
				*err += (ci_error_code_e) val->i;
				ret = 1;
			}

			break;
		}

		val = val->next;
	}

	THINGS_LOG_D(TAG, "*err = %d, ret = %d", *err, ret);

	THINGS_LOG_D(TAG, "Exit");
	return ret;
}

static CAResponseResult_t things_result_to_network_result(OCStackResult oc_code, OCMethod method)
{
	CAResponseResult_t ret = CA_EMPTY;

	switch (oc_code) {
	case OC_STACK_OK:
		switch (method) {
		case OC_REST_PUT:
		case OC_REST_POST:
			// This Response Code is like HTTP 204 "No Content" but only used in
			// response to POST and PUT requests.
			ret = CA_CHANGED;
			break;
		case OC_REST_GET:
			// This Response Code is like HTTP 200 "OK" but only used in response to
			// GET requests.
			ret = CA_CONTENT;
			break;
		default:
			// This should not happen but,
			// give it a value just in case but output an error
			ret = CA_CONTENT;
			THINGS_LOG_E(TAG, "Unexpected OC_STACK_OK return code for method [%d].", method);
		}
		break;
	case OC_STACK_RESOURCE_CREATED:
		ret = CA_CREATED;
		break;
	case OC_STACK_RESOURCE_DELETED:
		ret = CA_DELETED;
		break;
	case OC_STACK_RESOURCE_CHANGED:
		ret = CA_CHANGED;
		break;
	case OC_STACK_INVALID_QUERY:
		ret = CA_BAD_REQ;
		break;
	case OC_STACK_INVALID_OPTION:
		ret = CA_BAD_OPT;
		break;
	case OC_STACK_FORBIDDEN_REQ:
		ret = CA_FORBIDDEN_REQ;
		break;
	case OC_STACK_NO_RESOURCE:
		ret = CA_NOT_FOUND;
		break;
	case OC_STACK_COMM_ERROR:
		ret = CA_RETRANSMIT_TIMEOUT;
		break;
	case OC_STACK_UNAUTHORIZED_REQ:
		ret = CA_UNAUTHORIZED_REQ;
		break;
	case OC_STACK_INTERNAL_SERVER_ERROR:
		ret = CA_INTERNAL_SERVER_ERROR;
		break;
	default:
		break;
	}
	return ret;
}

static char *make_ip_port(char *p_ip, char *p_port)
{
	char *ipport = NULL;
	int length = 0;

	if (p_ip == NULL || p_port == NULL) {
		THINGS_LOG_E(TAG, "IP=%s, or Port=%s is NULL.", p_ip, p_port);
		return NULL;
	}

	length = strlen(p_ip) + strlen(p_port) + 2;
	if ((ipport = (char *)things_malloc(length)) == NULL) {
		THINGS_LOG_E(TAG, "Memory allocation is failed.");
		return NULL;
	}

	memset(ipport, 0, length);
	if (things_strcat(ipport, length, p_ip) == NULL) {
		THINGS_LOG_E(TAG, "things_strcat is failed.");
		things_free(ipport);
		return NULL;
	}
	ipport[strlen(p_ip)] = ':';
	if (things_strcat(ipport, length, p_port) == NULL) {
		THINGS_LOG_E(TAG, "things_strcat is failed.");
		things_free(ipport);
		return NULL;
	}

	THINGS_LOG_D(TAG, "IP=%s, Port=%s, ipport=%s", p_ip, p_port, ipport);
	return ipport;
}

static es_cloud_event_timeout_s *clone_data_add_timeout(es_cloud_prov_data_s *cloud_data, things_timeout_s *timeout)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	es_cloud_event_timeout_s *cloned_data = NULL;

	if (cloud_data == NULL) {
		THINGS_LOG_E(TAG, "cloud_data is NULL.");
		return NULL;
	}

	if ((cloned_data = (es_cloud_event_timeout_s *) things_malloc(sizeof(es_cloud_event_timeout_s) * 1)) == NULL) {
		THINGS_LOG_E(TAG, "Memory allocation is failed.");
		return NULL;
	}

	clone_es_cloud_prov_data(&cloned_data->event, cloud_data);

	if (timeout == NULL) {
		THINGS_LOG_D(TAG, "Default Setting to cloned Cloud Data.");
		cloned_data->timeOut.cur_counter = DEFAULT_TIMEOUT;
		cloned_data->timeOut.cur_num = DEFAULT_TIMEOUT;
		cloned_data->timeOut.ori_num = DEFAULT_TIMEOUT;
	} else {
		THINGS_LOG_D(TAG, "Custom Setting to cloned Cloud Data.");
		cloned_data->timeOut.cur_counter = timeout->cur_counter;
		cloned_data->timeOut.cur_num = timeout->cur_num;
		cloned_data->timeOut.ori_num = timeout->ori_num;
	}

	THINGS_LOG_D(TAG, "cur_counter = %d, cur_num = %d, ori_num = %d", cloned_data->timeOut.cur_counter, cloned_data->timeOut.cur_num, cloned_data->timeOut.ori_num);
	THINGS_LOG_D(TAG, "cloned_data add= 0x%X", cloned_data);
	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
	return cloned_data;
}

static void ci_finish_cloud_con(int result)
{
	char *domain = NULL;
	char *access_token = NULL;
	es_cloud_signup_s *Cloud_data = NULL;

	if (things_res_cb_function) {
		dm_load_legacy_cloud_data(&Cloud_data);	// TODO read Cloud-Data.
		if (Cloud_data != NULL) {
			if (Cloud_data->domain != NULL) {
				domain = things_strdup(Cloud_data->domain);
			}
			if (Cloud_data->access_token != NULL) {
				access_token = things_strdup(Cloud_data->access_token);
			}
		}

		things_res_cb_function(result, domain, access_token);
		things_res_cb_function = NULL;
	}

	if (domain) {
		things_free(domain);
		domain = NULL;
	}

	if (access_token) {
		things_free(access_token);
		access_token = NULL;
	}
}

static void publish_dev_profile_into_cloud(things_timeout_s *timeout)
{
	if (things_cloud_dev_profile_publish(g_cloud_address, handle_dev_profile_cb, handle_dev_profile_timeout, timeout) != OC_STACK_OK) {
		THINGS_LOG_E(TAG, "Publish Device Profile request is failed.");
	}
}
