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

#define _BSD_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include <netdb.h>
#ifdef __USE_MISC
#include <arpa/inet.h>
#include <errno.h>
#endif

#include "octypes.h"
#include "ocstack.h"
#include "ocpayload.h"
#include "rd_client.h"

#include "utils/things_malloc.h"
#include "utils/things_string.h"
#include "logging/things_logger.h"
#include "easy-setup/es_common.h"
#include "cloud_connector.h"
#include "utils/things_wait_handler.h"
#include "framework/things_data_manager.h"
#include <wifi_manager/wifi_manager.h>

#define TAG "[cloud_cntor]"

#define HOSTENT_INTERNAL_HEADER_SIZE        100

/** Account Device-Profile publish URI.*/
#define OC_RSRVD_ACCOUNT_DEVPROFILE_URI    "/oic/account/profile/device"

static OCRepPayload *make_dev_profile_payload(const st_device_s *dev_info);

int CICheckDomain(const char *DomainName, char **pIP)
{
	char ipbuffer[20];
	memset(ipbuffer, 0, 20);
	char bytes[4];
	struct hostent *shost;
	unsigned int ip4_address = 0;

	if (DomainName != NULL) {
		shost = gethostbyname((const char *)DomainName);
		if (shost != NULL) {
			memcpy(&ip4_address, shost->h_addr, sizeof(in_addr_t));
			bytes[0] = ip4_address & 0XFF;
			bytes[1] = (ip4_address >> 8) & 0XFF;
			bytes[2] = (ip4_address >> 16) & 0XFF;
			bytes[3] = (ip4_address >> 24) & 0XFF;

			snprintf(ipbuffer, sizeof(ipbuffer), "%d.%d.%d.%d", bytes[0], bytes[1], bytes[2], bytes[3]);

			THINGS_LOG_D(TAG, "DNS resolved IP for[%s] is = %s", DomainName, ipbuffer);

			if (pIP != NULL) {
				*pIP = things_strdup(ipbuffer);
			}
			return 0;
		}
	} 

	THINGS_LOG_E(TAG, " Failed to get the IP");

	return -1;
}

/**
 * @brief              Entry Point for checking about internet & Cloud connection.
 * @param DomainName   wanted Domain-Name string to look-up IP address. (unit: const char*)
 * @param pIP          Variable pointer to store Founded IP address. (unit: char**)
 * @return             success : return 0 \n
 *                     failure : return -1
 */
int ci_connection_pre_check(const char *DomainName, char **pIP)
{
	int n_err = -1;
	n_err = CICheckDomain("samsung.com", NULL);
	if (n_err != 0) {
		THINGS_LOG_E(TAG, "AP is Not Connected to Internet");
		return n_err;
	}
	/** If AP Connected but DomainName is Null, do not check Domain return Success **/
	if (DomainName == NULL) {
		n_err = 0;
		THINGS_LOG_V(TAG, "AP Connected but DomainName is Null\n");
	} else {
		n_err = CICheckDomain(DomainName, pIP);
		THINGS_LOG_D(TAG, "Found IP = %s, n_err = %d", *pIP, n_err);
	}
	return n_err;
}

/**
 * @brief                  Send Sign-UP request to Cloud.
 * @param host             wanted Cloud address. (unit: const char*) \n
 *                         Ex) coaps+tcp://123.123.123.123:1234
 * @param device_id         Identifier of device. (unit: const char*)
 * @param event_data        Data structure pointer for Cloud-Provisioning Data.
 * @param response         Call-Back function when cloud response against this request.
 * @param timeoutHandler   Call function when Time-out is occurred.
 * @param timeout          Data structure pointer for Time-out processing Data.
 * @return                 success : OC_STACK_OK \n
 *                         failure : other value
 */
OCStackResult things_cloud_signup(const char *host, const char *device_id, const es_cloud_prov_data_s *event_data, OCClientResponseHandler response, things_check_time_out_call_func timeoutHandler, things_timeout_s *timeout)
{
	OCDoHandle g_req_handle = NULL;
	OCStackResult result = OC_STACK_ERROR;
	char targetUri[MAX_URI_LENGTH * 2] = { 0, };
	OCCallbackData cb_data;
	OCRepPayload *registerPayload = NULL;
	OCRepPayload *aafPayload = NULL;
	char *mnid = NULL;	

	if (host == NULL || device_id == NULL || event_data == NULL || (event_data->accesstoken[0] == 0 && event_data->auth_code[0] == 0)) {
		THINGS_LOG_E(TAG, "Invalid event_data.");
		goto no_memory;
	}

	snprintf(targetUri, MAX_URI_LENGTH * 2, "%s%s", host, OC_RSRVD_ACCOUNT_URI);
	THINGS_LOG_V(TAG, "targetUri : %s", targetUri);

	memset(&cb_data, 0, sizeof(OCCallbackData));
	cb_data.cb = response;
	cb_data.cd = NULL;
	cb_data.context = (void *)DEFAULT_CONTEXT_VALUE;

	registerPayload = OCRepPayloadCreate();
	if (!registerPayload) {
		THINGS_LOG_E(TAG, "registerPlayload is NULL");
		goto no_memory;
	}

	THINGS_LOG_D(TAG, "di              : %s", device_id);
	THINGS_LOG_D(TAG, "authprovider    : %s", event_data->auth_provider);
	THINGS_LOG_D(TAG, "devicetype      : resource_server");

	OCRepPayloadSetPropString(registerPayload, "di", device_id);
	OCRepPayloadSetPropString(registerPayload, "authprovider", event_data->auth_provider);
	OCRepPayloadSetPropString(registerPayload, "devicetype", "resource_server");

	if (event_data->accesstoken[0] != 0) {
		THINGS_LOG_D(TAG, "accesstoken     : %s", event_data->accesstoken);
		OCRepPayloadSetPropString(registerPayload, "accesstoken", event_data->accesstoken);

		if (event_data->uid[0] != 0) {
			THINGS_LOG_D(TAG, "uid             : %s", event_data->uid);
			OCRepPayloadSetPropString(registerPayload, "uid", event_data->uid);
		}
	} else {
		THINGS_LOG_D(TAG, "authcode        : %s", event_data->auth_code);
		OCRepPayloadSetPropString(registerPayload, "authcode", event_data->auth_code);
	}

	if (event_data->client_id[0] != 0) {
		THINGS_LOG_D(TAG, "clientid        : %s", event_data->client_id);
		OCRepPayloadSetPropString(registerPayload, THINGS_RSRVD_ES_CLIENTID, event_data->client_id);
	}

	THINGS_LOG_V(TAG, "OCToCATransportFlags(CT_ADAPTER_TCP)");

	if (dm_get_easy_setup_use_artik_crt() == true) {
		THINGS_LOG_V(TAG, "haaf    : true");
		OCRepPayloadSetPropBool(registerPayload, "haaf", true);

		aafPayload = OCRepPayloadCreate();
		if (aafPayload == NULL) {
			THINGS_LOG_E(TAG, "[Error] memory allocation is failed.(aafPayload)");
			goto no_memory;
		}

		mnid = (char *)dm_get_mnid();
		THINGS_LOG_V(TAG, "aaf.mnId   %s", mnid);
		OCRepPayloadSetPropString(aafPayload, "mnid", mnid);
		OCRepPayloadSetPropObject(registerPayload, "aaf", aafPayload);
	}

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
	//CASelectCipherSuite(0x35, (1 << 4));
#endif

	if (things_is_empty_request_handle() == true) {
		result = OCDoResource(&g_req_handle, OC_REST_POST, targetUri, NULL, (OCPayload *) registerPayload, CT_ADAPTER_TCP, OC_LOW_QOS, &cb_data, NULL, 0);

		if (result == OC_STACK_OK && timeoutHandler != NULL) {
			if (things_add_request_handle(g_req_handle) == NULL) {
				THINGS_LOG_E(TAG, "[Error] ReqHandles array space is small.");
				goto no_memory;
			}

			things_create_time_out_process(g_req_handle, timeoutHandler, timeout);
		}
	} else {
		OCRepPayloadDestroy(registerPayload);
	}

	return result;

no_memory:
	OCRepPayloadDestroy(aafPayload);
	OCRepPayloadDestroy(registerPayload);
	return OC_STACK_NO_MEMORY;
}

/**
 * @brief                  Send Sign-IN request to Cloud.
 * @param host             wanted Cloud address. (unit: const char*) \n
 *                         Ex) coaps+tcp://123.123.123.123:1234
 * @param uId              Identifier of User. (unit: const char*)
 * @param device_id         Identifier of device. (unit: const char*)
 * @param accesstoken      received access-token from Cloud.
 * @param response         Call-Back function when cloud response against this request.
 * @param timeoutHandler   Call function when Time-out is occurred.
 * @param timeout          Data structure pointer for Time-out processing Data.
 * @return                 success : OC_STACK_OK \n
 *                         failure : other value
 */
OCStackResult things_cloud_session(const char *host, const char *uId, const char *device_id, const char *accesstoken, bool isLogin, OCClientResponseHandler response, things_check_time_out_call_func timeoutHandler, things_timeout_s *timeout)
{
	OCDoHandle g_req_handle = NULL;
	OCStackResult result = OC_STACK_ERROR;
	char targetUri[MAX_URI_LENGTH * 2] = { 0, };
	char *coreVer = NULL;
	char *IoTivityVer = NULL;

	if (accesstoken == NULL || accesstoken[0] == 0) {
		THINGS_LOG_E(TAG, "accesstoken is invalid.(0x%X)", accesstoken);
		return OC_STACK_ERROR;
	}

	snprintf(targetUri, MAX_URI_LENGTH * 2, "%s%s", host, OC_RSRVD_ACCOUNT_SESSION_URI);

	THINGS_LOG_V(TAG, "Initiate Session with : %s", targetUri);

	OCCallbackData cb_data;
	memset(&cb_data, 0, sizeof(OCCallbackData));
	cb_data.cb = response;
	cb_data.cd = NULL;
	cb_data.context = (void *)DEFAULT_CONTEXT_VALUE;

	OCRepPayload *loginoutPayload = OCRepPayloadCreate();
	if (!loginoutPayload) {
		goto no_memory;
	}

	if (OCGetPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_SPEC_VERSION, (void **)&coreVer) != OC_STACK_OK || (IoTivityVer = things_strdup(IOTIVITY_VERSION)) == NULL)
//            OCGetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_PLATFORM_VERSION, (void**)&IoTivityVer) != OC_STACK_OK )
	{
		THINGS_LOG_E(TAG, "Getting Core_Spec_Ver or IoTivity_Ver is failed.");
		goto no_memory;
	}

	THINGS_LOG_V(TAG, "coreversion      : %s", coreVer);
	THINGS_LOG_V(TAG, "verticalversion  : %s", IoTivityVer);

	if (uId != NULL) {
		OCRepPayloadSetPropString(loginoutPayload, KEY_ID_USER, uId);
	}
	OCRepPayloadSetPropString(loginoutPayload, KEY_ID_DEVICE, device_id);
	OCRepPayloadSetPropString(loginoutPayload, KEY_TOKEN_ACCESS, accesstoken);
	OCRepPayloadSetPropBool(loginoutPayload, KEY_LOGINOUT, isLogin);
	OCRepPayloadSetPropString(loginoutPayload, KEY_ICORE_VER, coreVer);
	OCRepPayloadSetPropString(loginoutPayload, KEY_IOTIVITY_VER, IoTivityVer);

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
	//CASelectCipherSuite(0x35, (1 << 4));
#endif

	if (things_is_empty_request_handle() == true) {
		result = OCDoResource(&g_req_handle, OC_REST_POST, targetUri, NULL, (OCPayload *) loginoutPayload, CT_ADAPTER_TCP, OC_LOW_QOS, &cb_data, NULL, 0);

		if (result == OC_STACK_OK && timeoutHandler != NULL) {
			if (things_add_request_handle(g_req_handle) == NULL) {
				THINGS_LOG_E(TAG, "[Error] ReqHandles array space is small.");
				goto no_memory;
			}

			things_create_time_out_process(g_req_handle, timeoutHandler, timeout);
		}
	} else {
		OCRepPayloadDestroy(loginoutPayload);
	}

	if (IoTivityVer) {
		things_free(IoTivityVer);
		IoTivityVer = NULL;
	}
	if (coreVer) {
		things_free(coreVer);
		coreVer = NULL;
	}
	return result;

no_memory:
	if (IoTivityVer) {
		things_free(IoTivityVer);
		IoTivityVer = NULL;
	}
	if (coreVer) {
		things_free(coreVer);
		coreVer = NULL;
	}
	OCRepPayloadDestroy(loginoutPayload);
	return OC_STACK_NO_MEMORY;
}

/**
 * @brief                  Send Resources Publish request to Cloud.
 * @param host             wanted Cloud address. (unit: const char*)\n
 *                         Ex) coaps+tcp://123.123.123.123:1234
 * @param list             Resource List pointer for Publishing to Cloud.
 * @param length           Count of Resource in Resource List(list).
 * @param response         Call-Back function when cloud response against this request.
 * @param timeoutHandler   Call function when Time-out is occurred.
 * @param timeout          Data structure pointer for Time-out processing Data.
 * @return                 success : OC_STACK_OK \n
 *                         failure : other value
 */
OCStackResult things_cloud_rsc_publish(char *host, things_resource_s **list, int length, OCClientResponseHandler response, things_check_time_out_call_func timeoutHandler, things_timeout_s *timeout)
{
	OCStackResult result = OC_STACK_ERROR;
	THINGS_LOG_V(TAG, "Resource Publish Start.");

	OCDoHandle g_req_handle = NULL;
	OCCallbackData cb_data;
	cb_data.cb = response;
	cb_data.context = (void *)DEFAULT_CONTEXT_VALUE;
	cb_data.cd = NULL;

	OCResourceHandle resourceHandles[length + 2];

	THINGS_LOG_V(TAG, "Publishing [%d] number of resources ", length);
	int iter = 0;
	for (iter = 0; iter < length; iter++) {
		resourceHandles[iter] = list[iter]->resource_handle;

		THINGS_LOG_D(TAG, "URI : %s ", list[iter]->uri);
	}

	uint8_t numResource = 0;
	OCGetNumberOfResources(&numResource);
	if (numResource > 0) {
		for (uint8_t index = 0; index < numResource; index++) {
			OCResourceHandle handle = OCGetResourceHandle(index);

			const char *uri = OCGetResourceUri(handle);

			if (0 == strncmp(uri, OC_RSRVD_DEVICE_URI, strlen(OC_RSRVD_DEVICE_URI))
				|| 0 == strncmp(uri, OC_RSRVD_PLATFORM_URI, strlen(OC_RSRVD_PLATFORM_URI))) {
				resourceHandles[iter++] = handle;
			}
		}
	}

	if (things_is_empty_request_handle() == true) {
		result = OCRDPublish(&g_req_handle, host, CT_ADAPTER_TCP, resourceHandles, iter, &cb_data, OC_LOW_QOS);

		if (result == OC_STACK_OK && timeoutHandler != NULL) {
			if (things_add_request_handle(g_req_handle) == NULL) {
				THINGS_LOG_E(TAG, "[Error] ReqHandles array space is small.");
				return OC_STACK_ERROR;
			}

			things_create_time_out_process(g_req_handle, timeoutHandler, timeout);
		}
	}

	return result;
}

OCStackResult things_cloud_rsc_publish_with_device_id(char *host, const char *id, things_resource_s **list, int length, OCClientResponseHandler response, things_check_time_out_call_func timeoutHandler, things_timeout_s *timeout)
{
	OCStackResult result = OC_STACK_ERROR;
	THINGS_LOG_V(TAG, "Resource Publish Start.");

	OCDoHandle g_req_handle = NULL;
	OCCallbackData cb_data;
	cb_data.cb = response;
	cb_data.context = (void *)DEFAULT_CONTEXT_VALUE;
	cb_data.cd = NULL;

	OCResourceHandle resourceHandles[length + 2];

	THINGS_LOG_V(TAG, "Device ID ==> %s ", id);
	THINGS_LOG_V(TAG, "Publishing [%d] number of resources ", length);

	int iter = 0;
	for (iter = 0; iter < length; iter++) {
		resourceHandles[iter] = list[iter]->resource_handle;
		THINGS_LOG_D(TAG, "URI : %s ", list[iter]->uri);
	}

	if (iter > 0) {
		result = OCRDPublishWithDeviceId(&g_req_handle, host, id, CT_ADAPTER_TCP, resourceHandles, iter, &cb_data, OC_LOW_QOS);
	}
//    if(result == OC_STACK_OK && timeoutHandler != NULL )
//    {
//        if( things_add_request_handle(g_req_handle) == NULL )
//        {
//            THINGS_LOG_E(TAG, "[Error] ReqHandles array space is small.");
//            return OC_STACK_ERROR;
//        }
//
//        things_create_time_out_process(g_req_handle, timeoutHandler, timeout);
//    }

	return result;
}

/**
 * @brief                  Send Dev-Profile Publish request to Cloud.
 * @param host             wanted Cloud address. (unit: const char*)\n
 *                         Ex) coaps+tcp://123.123.123.123:1234
 * @param response         Call-Back function when cloud response against this request.
 * @param timeoutHandler   Call function when Time-out is occurred.
 * @param timeout          Data structure pointer for Time-out processing Data.
 * @return                 success : OC_STACK_OK \n
 *                         failure : other value
 */
OCStackResult things_cloud_dev_profile_publish(char *host, OCClientResponseHandler response, things_check_time_out_call_func timeoutHandler, things_timeout_s *timeout)
{
	THINGS_LOG_D(TAG, "Device-profile Publish Start.");

	st_device_s *dev_info = NULL;
	int cntArrayPayload = 0;
	OCRepPayload **arrayPayload = NULL;
	OCRepPayload *payload = NULL;
	OCCallbackData cb_data;
	OCDoHandle g_req_handle = NULL;
	OCStackResult result = OC_STACK_ERROR;
	char targetUri[MAX_URI_LENGTH * 2] = { 0, };
	char *coreVer = NULL;
	char *IoTivityVer = NULL;

	snprintf(targetUri, MAX_URI_LENGTH * 2, "%s%s", host, OC_RSRVD_ACCOUNT_DEVPROFILE_URI);

	THINGS_LOG_V(TAG, "Initiate DevProfile with : %s", targetUri);

	cb_data.cb = response;
	cb_data.context = (void *)DEFAULT_CONTEXT_VALUE;
	cb_data.cd = NULL;

	payload = OCRepPayloadCreate();
	if (!payload) {
		THINGS_LOG_E(TAG, "[Error] memory allocation is failed.(payload)");
		goto no_memory;
	}

	if ((arrayPayload = (OCRepPayload **) things_malloc(sizeof(OCRepPayload *))) == NULL) {
		THINGS_LOG_E(TAG, "[Error] memory allocation is failed.(arrayPayload)");
		goto no_memory;
	}

	if ((dev_info = dm_get_info_of_dev(0)) == NULL) {
		THINGS_LOG_E(TAG, "[Error] device info structure is NULL");
		goto no_memory;
	}

	if (dev_info->is_physical == 1) {
		if ((arrayPayload[cntArrayPayload] = make_dev_profile_payload(dev_info)) == NULL) {
			THINGS_LOG_E(TAG, "It's failed making payload of Device-Info.(seq=%d)", cntArrayPayload);
			goto no_memory;
		}
		cntArrayPayload++;
	}


	if (cntArrayPayload < 1) {
		THINGS_LOG_E(TAG, "[Error] pysical device is not exist. So, Can not Sending request for Device-Profile.");
		goto no_memory;
	}

	size_t dimensions[MAX_REP_ARRAY_DEPTH] = { cntArrayPayload, 0, 0 };
	OCRepPayloadSetPropObjectArray(payload, "devices", arrayPayload, dimensions);

	if (things_is_empty_request_handle() == true) {
		result = OCDoResource(&g_req_handle, OC_REST_POST, targetUri, NULL, (OCPayload *) payload, CT_ADAPTER_TCP, OC_LOW_QOS, &cb_data, NULL, 0);

		if (result == OC_STACK_OK && timeoutHandler != NULL) {
			if (things_add_request_handle((OCDoHandle) 1) == NULL) {
				THINGS_LOG_E(TAG, "[Error] ReqHandles array space is small.");
				/*for SVACE Warning */
				if (arrayPayload) {
					for (int i = 0; i < cntArrayPayload; i++) {
						OCPayloadDestroy((OCPayload *) arrayPayload[i]);
					}
					things_free(arrayPayload);
					arrayPayload = NULL;
				}
				return OC_STACK_ERROR;
			}

			things_create_time_out_process((OCDoHandle) 1, timeoutHandler, timeout);
		}
	}

	if (arrayPayload) {
		for (int i = 0; i < cntArrayPayload; i++) {
			OCPayloadDestroy((OCPayload *) arrayPayload[i]);
		}
		things_free(arrayPayload);
		arrayPayload = NULL;
	}

	THINGS_LOG_D(TAG, "Exit. -Success-");
	return result;

no_memory:

	if (arrayPayload) {
		for (int i = 0; i < cntArrayPayload; i++) {
			OCPayloadDestroy((OCPayload *) arrayPayload[i]);
		}
		things_free(arrayPayload);
		arrayPayload = NULL;
	}
	OCRepPayloadDestroy(payload);

	THINGS_LOG_D(TAG, "Exit. -Fail-");
	return OC_STACK_NO_MEMORY;
}

/**
 * @brief                  Send Refresh access-token request to Cloud.\n
 *                         Set should call refresh before expiresin of refresh-token or when receive 4.01 during sign-in
 * @param host             wanted Cloud address. (unit: const char*) \n
 *                         Ex) coaps+tcp://123.123.123.123:1234
 * @param uId              Identifier of User.
 * @param device_id         Identifier of Device.
 * @param refreshtoken     Token for refreshing access-token from Cloud.
 * @param response         Call-Back function when cloud response against this request.
 * @return                 success : OC_STACK_OK \n
 *                         failure : other value
 */
OCStackResult things_cloud_refresh(const char *host, const char *uId, const char *device_id, const char *refreshtoken, OCClientResponseHandler response)
{
	char targetUri[MAX_URI_LENGTH * 2] = { 0, };
	snprintf(targetUri, MAX_URI_LENGTH * 2, "%s%s", host, OC_RSRVD_ACCOUNT_TOKEN_REFRESH_URI);

	OCCallbackData cb_data;
	memset(&cb_data, 0, sizeof(OCCallbackData));
	cb_data.cb = response;
	cb_data.cd = NULL;
	cb_data.context = (void *)DEFAULT_CONTEXT_VALUE;

	OCRepPayload *refreshPayload = OCRepPayloadCreate();
	if (!refreshPayload) {
		goto no_memory;
	}

	if (uId != NULL) {
		OCRepPayloadSetPropString(refreshPayload, KEY_ID_USER, uId);
	}
	OCRepPayloadSetPropString(refreshPayload, KEY_ID_DEVICE, device_id);
	OCRepPayloadSetPropString(refreshPayload, KEY_TYPE_GRANT, VALUE_TYPE_GRANT_TOKEN);
	OCRepPayloadSetPropString(refreshPayload, KEY_TOKEN_ACCESS_REFRESH, refreshtoken);

	THINGS_LOG_D(TAG, "targetURI    =%s", targetUri);
	THINGS_LOG_D(TAG, "uid          =%s", uId);
	THINGS_LOG_D(TAG, "di           =%s", device_id);
	THINGS_LOG_D(TAG, "granttype    =%s", VALUE_TYPE_GRANT_TOKEN);
	THINGS_LOG_D(TAG, "refreshtoken =%s", refreshtoken);

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
	//CASelectCipherSuite(0x35, (1 << 4));
#endif

	return OCDoResource(NULL, OC_REST_POST, targetUri, NULL, (OCPayload *) refreshPayload, CT_ADAPTER_TCP, OC_LOW_QOS, &cb_data, NULL, 0);

no_memory:
	OCRepPayloadDestroy(refreshPayload);
	return OC_STACK_NO_MEMORY;
}

OCStackResult things_cloud_topic_publish_topic(const char *host, const char *topic, OCRepPayload *message, OCClientResponseHandler response)
{
	char targetUri[MAX_URI_LENGTH * 2] = { 0, };

	snprintf(targetUri, MAX_URI_LENGTH * 2, "%s%s", host, topic);

	OCCallbackData cb_data;
	memset(&cb_data, 0, sizeof(OCCallbackData));
	cb_data.cb = response;
	cb_data.cd = NULL;
	cb_data.context = (void *)DEFAULT_CONTEXT_VALUE;

	if (!message) {
		goto no_memory;
	}
#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
	//CASelectCipherSuite(0x35, (1 << 4));
#endif
	return OCDoResource(NULL, OC_REST_POST, targetUri, NULL, (OCPayload *) message, CT_ADAPTER_TCP, OC_LOW_QOS, &cb_data, NULL, 0);

no_memory:
	OCRepPayloadDestroy(message);
	return OC_STACK_NO_MEMORY;
}

static OCRepPayload *make_dev_profile_payload(const st_device_s *dev_info)
{
	THINGS_LOG_D(TAG, "Enter.");

	bool res = false;
	char *manuFactory = NULL;
	char *coreVer = NULL;
	char *IoTivityVer = NULL;
	OCRepPayload *payload = NULL;

	if (dev_info == NULL) {
		THINGS_LOG_E(TAG, "input argument is invalid.(dev_info is null)");
		return NULL;
	}

	if ((payload = OCRepPayloadCreate()) == NULL) {
		THINGS_LOG_E(TAG, "memory allocation is failed.(payload)");
		return NULL;
	}

	if (OCGetPropertyValue(PAYLOAD_TYPE_DEVICE, OC_RSRVD_SPEC_VERSION, (void **)&coreVer) != OC_STACK_OK) {
		THINGS_LOG_E(TAG, "Getting CoreVersion of IoTivity is failed.");
		goto GOTO_OUT;
	}

	IoTivityVer = things_strdup(IOTIVITY_VERSION);

	if (OCGetPropertyValue(PAYLOAD_TYPE_PLATFORM, OC_RSRVD_MFG_NAME, (void **)&manuFactory) != OC_STACK_OK) {
		THINGS_LOG_E(TAG, "Getting manufacturer-name is failed.");
		goto GOTO_OUT;
	}

	OCRepPayloadSetPropString(payload, "di", dev_info->device_id);
	OCRepPayloadSetPropString(payload, "n", dev_info->name);
	OCRepPayloadSetPropString(payload, "icv", coreVer);
	OCRepPayloadSetPropString(payload, "dmv", IoTivityVer);
//    OCRepPayloadSetPropString(payload, "vrv", );
	OCRepPayloadSetPropString(payload, "rt", dev_info->type);
	OCRepPayloadSetPropString(payload, "mnmn", manuFactory);
	OCRepPayloadSetPropString(payload, "mnmo", dev_info->model_num);
	OCRepPayloadSetPropString(payload, "mnpv", dev_info->ver_p);
	OCRepPayloadSetPropString(payload, "mnos", dev_info->ver_os);
	OCRepPayloadSetPropString(payload, "mnhw", dev_info->ver_hw);
	OCRepPayloadSetPropString(payload, "mnfv", dev_info->ver_fw);
	OCRepPayloadSetPropString(payload, "vid", dev_info->vid);

	THINGS_LOG_D(TAG, "di   = %s", dev_info->device_id);
	THINGS_LOG_D(TAG, "n    = %s", dev_info->name);
	THINGS_LOG_D(TAG, "icv  = %s", coreVer);
	THINGS_LOG_D(TAG, "dmv  = %s", IoTivityVer);
	THINGS_LOG_D(TAG, "rt   = %s", dev_info->type);
	THINGS_LOG_D(TAG, "mnmn = %s", manuFactory);
	THINGS_LOG_D(TAG, "mnmo = %s", dev_info->model_num);
	THINGS_LOG_D(TAG, "mnpv = %s", dev_info->ver_p);
	THINGS_LOG_D(TAG, "mnos = %s", dev_info->ver_os);
	THINGS_LOG_D(TAG, "mnhw = %s", dev_info->ver_hw);
	THINGS_LOG_D(TAG, "mnfv = %s", dev_info->ver_fw);
	THINGS_LOG_D(TAG, "vid  = %s", dev_info->vid);

	res = true;

GOTO_OUT:
	if (coreVer) {
		things_free(coreVer);
		coreVer = NULL;
	}

	if (IoTivityVer) {
		things_free(IoTivityVer);
		IoTivityVer = NULL;
	}

	if (manuFactory) {
		things_free(manuFactory);
		manuFactory = NULL;
	}

	if (res == false) {
		OCPayloadDestroy((OCPayload *) payload);
		payload = NULL;
	}

	THINGS_LOG_D(TAG, "Exit. payload=0x%X", payload);
	return payload;
}
