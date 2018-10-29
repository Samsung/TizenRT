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


#include "resource_handler.h"

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#ifdef __ST_THINGS_RTOS__
#include <net/lwip/inet.h>
#else
#include <arpa/inet.h>
#endif
#include "things_api.h"
#include "octypes.h"
#include "ocpayload.h"
#include "logging/things_logger.h"
#include "cloud/cloud_manager.h"
#include "utils/things_string.h"
#include "utils/things_malloc.h"
#include "utils/things_string.h"
#include "cautilinterface.h"
#include "framework/things_common.h"

#include "utils/things_network.h"

#define ES_RH_TAG "[ezsetup-RH]"

static prov_resource_s g_prov_resource;
static wifi_resource_s g_wifi_resource;
static cloud_resource_s g_cloud_resource;
static dev_conf_resource_s g_dev_conf_resource;

/**
 * @var g_wifi_data
 * @brief Structure for holding the target information required to
 * connect to the target network
 */
static es_wifi_prov_data_s g_wifi_data;

/**
 * @var g_dev_conf_data
 * @brief Structure for holding the device information
 */
static es_dev_conf_prov_data_s g_dev_conf_data;

/**
 * @var g_cloud_data
 * @brief Structure for holding the cloud information required to
 * connect to CI Server
 */
static es_cloud_prov_data_s g_cloud_data;

static wifi_prov_state_e es_wifi_prov_state = WIFI_INIT;

static pthread_mutex_t g_status_mutex = PTHREAD_MUTEX_INITIALIZER;

//-----------------------------------------------------------------------------
// Private internal function prototypes
//-----------------------------------------------------------------------------
OCEntityHandlerResult things_entity_handler_cb(OCEntityHandlerFlag flag, OCEntityHandlerRequest *eh_request, void *callback);
OCEntityHandlerResult process_get_request(OCEntityHandlerRequest *eh_request, OCRepPayload **payload);
OCEntityHandlerResult process_put_request(OCEntityHandlerRequest *eh_request, OCRepPayload **payload);
OCEntityHandlerResult process_post_request(OCEntityHandlerRequest *eh_request, OCRepPayload **payload);
void update_prov_resource(OCEntityHandlerRequest *eh_request, OCRepPayload *input);
void update_wifi_resource(OCRepPayload *input);
bool update_cloud_resource(OCRepPayload *input);
void update_dev_conf_resource(OCRepPayload *input);
static int check_ci_server_ipv4(char *ci_server, es_cloud_prov_data_s *cloud_data);

es_wifi_cb g_wifi_rsrc_evt_cb = NULL;
es_cloud_cb g_cloud_rsrc_evt_cb = NULL;
es_dev_conf_cb g_dev_conf_rsrc_evt_cb = NULL;

static inline void delete_child_payload(OCRepPayload *payload)
{
	if (payload == NULL) {
		return;
	}

	delete_child_payload(payload->next);
	OCPayloadDestroy((OCPayload *) payload);
}

static bool compare_resource_interface(const char *from, const char *iface)
{
	bool ret = false;
	char *str = NULL;
	char *ptr = NULL;

	if (from == NULL || iface == NULL) {
		THINGS_LOG_E(ES_RH_TAG, "invalid input data.(from=%s, iface=%s)", from, iface);
		return ret;
	}

	THINGS_LOG_D(ES_RH_TAG, "Input data.(from=%s, iface=%s)", from, iface);

	if ((str = things_strdup(from)) == NULL) {
		THINGS_LOG_E(ES_RH_TAG, "things_strdup function is failed.");
		return ret;
	}

	THINGS_LOG_D(ES_RH_TAG, "strFrom=%s", str);

	ptr = str;
	while ((ptr = strtok(ptr, ";")) != NULL) {
		if (strstr(ptr, ".if.")) {
			char *if_ptr = NULL;
			if_ptr = strtok(ptr, "=");
			if_ptr = strtok(NULL, "=");

			THINGS_LOG_D(ES_RH_TAG, "if_ptr=%s", if_ptr);

			if (if_ptr != NULL && !strncmp(if_ptr, iface, strlen(iface))) {
				THINGS_LOG_D(ES_RH_TAG, "Found interface.(%s)", iface);
				ret = true;
				break;
			}
		}

		ptr = NULL;
	}

	things_free(str);
	return ret;
}

static OCRepPayload *make_rep_payload(OCResourceHandle rsc_handle, OCDevAddr *dev_addr)
{
	OCRepPayload *add = NULL;
	char *resource_type[1] = { NULL, };
	char *resource_interface[1] = { NULL, };
	size_t dimensions[MAX_REP_ARRAY_DEPTH] = { 1, 0, 0 };

	if (rsc_handle == NULL || dev_addr == NULL) {
		THINGS_LOG_E(ES_RH_TAG, "Invalid input data.(rsc_handle=0x%X, dev_addr=0x%X)", rsc_handle, dev_addr);
		return NULL;
	}

	if ((add = OCRepPayloadCreate()) == NULL) {
		THINGS_LOG_E(ES_RH_TAG, "Failed to allocate Payload");
		return NULL;
	}

	add->base.type = PAYLOAD_TYPE_REPRESENTATION;

	if (rsc_handle == g_wifi_resource.handle) {
		resource_type[0] = things_strdup(THINGS_RSRVD_ES_RES_TYPE_WIFI);
		resource_interface[0] = things_strdup(OC_RSRVD_INTERFACE_DEFAULT);
		OCRepPayloadSetPropString(add, OC_RSRVD_HREF, THINGS_RSRVD_ES_URI_WIFI);
	} else if (rsc_handle == g_dev_conf_resource.handle) {
		resource_type[0] = things_strdup(THINGS_RSRVD_ES_RES_TYPE_DEVCONF);
		resource_interface[0] = things_strdup(OC_RSRVD_INTERFACE_DEFAULT);
		OCRepPayloadSetPropString(add, OC_RSRVD_HREF, THINGS_RSRVD_ES_URI_DEVCONF);
	} else if (rsc_handle == g_cloud_resource.handle) {
		resource_type[0] = things_strdup(THINGS_RSRVD_ES_RES_TYPE_CLOUDSERVER);
		resource_interface[0] = things_strdup(OC_RSRVD_INTERFACE_DEFAULT);
		OCRepPayloadSetPropString(add, OC_RSRVD_HREF, THINGS_RSRVD_ES_URI_CLOUDSERVER);
	} else {
		THINGS_LOG_E(ES_RH_TAG, "Not supported resource handler(rsc_handle=0x%X)", rsc_handle);
		OCPayloadDestroy((OCPayload *) add);
		return NULL;
	}

	OCRepPayloadSetStringArray(add, OC_RSRVD_RESOURCE_TYPE, (const char **)resource_type, dimensions);
	OCRepPayloadSetStringArray(add, OC_RSRVD_INTERFACE, (const char **)resource_interface, dimensions);
	/*! Added by st_things for Memory leak fix */
	if (resource_type[0]) {
		things_free(resource_type[0]);
	}
	if (resource_interface[0]) {
		things_free(resource_interface[0]);
	}

	OCResourceProperty p = OCGetResourceProperties(rsc_handle);
	OCRepPayload *policy = OCRepPayloadCreate();
	if (!policy) {
		THINGS_LOG_E(ES_RH_TAG, "Failed to allocate Payload");
		OCPayloadDestroy((OCPayload *) add);
		return NULL;
	}

	OCRepPayloadSetPropInt(policy, OC_RSRVD_BITMAP, ((p & OC_DISCOVERABLE) | (p & OC_OBSERVABLE)));
	if (p & OC_SECURE) {
		OCRepPayloadSetPropBool(policy, OC_RSRVD_SECURE, p & OC_SECURE);
		uint16_t securePort = CAGetAssignedPortNumber(dev_addr->adapter, dev_addr->flags);
		OCRepPayloadSetPropInt(policy, OC_RSRVD_HOSTING_PORT, securePort);
	}

	OCRepPayloadSetPropObject(add, OC_RSRVD_POLICY, policy);
	OCPayloadDestroy((OCPayload *) policy);

	return add;
}

void set_wifi_prov_state(wifi_prov_state_e value)
{
	if (es_wifi_prov_state == (value - 1) || value == WIFI_INIT) {
		es_wifi_prov_state = value;
		THINGS_LOG_D(ES_RH_TAG, "Set wifi_prov_state_e=%d", es_wifi_prov_state);
	}
}

wifi_prov_state_e get_wifi_prov_state(void)
{
	return es_wifi_prov_state;
}

void register_wifi_rsrc_event_callback(es_wifi_cb cb)
{
	g_wifi_rsrc_evt_cb = cb;
}

void register_cloud_rsrc_event_callback(es_cloud_cb cb)
{
	g_cloud_rsrc_evt_cb = cb;
}

void register_dev_conf_rsrc_event_callback(es_dev_conf_cb cb)
{
	g_dev_conf_rsrc_evt_cb = cb;
}

void unregister_resource_event_callback()
{
	if (g_wifi_rsrc_evt_cb) {
		g_wifi_rsrc_evt_cb = NULL;
	}
	if (g_cloud_rsrc_evt_cb) {
		g_cloud_rsrc_evt_cb = NULL;
	}
	if (g_dev_conf_rsrc_evt_cb) {
		g_dev_conf_rsrc_evt_cb = NULL;
	}
}

void get_target_network_info_from_prov_resource(char *name, char *pass)
{
	if (name != NULL && pass != NULL) {
		things_strncpy(name, g_wifi_resource.ssid, WIFIMGR_SSID_LEN);
		things_strncpy(pass, g_wifi_resource.cred, WIFIMGR_PASSPHRASE_LEN);
	}
}

OCStackResult init_prov_resource(bool is_secured)
{
	es_set_state(ES_STATE_INIT);
	g_prov_resource.last_err_code = ES_ERRCODE_NO_ERROR;
	g_prov_resource.vd_err_code = ERRCI_NO_ERROR;

	OCStackResult res = OC_STACK_ERROR;
	if (is_secured) {
		res = OCCreateResource(&g_prov_resource.handle, THINGS_RSRVD_ES_RES_TYPE_PROV, OC_RSRVD_INTERFACE_DEFAULT, THINGS_RSRVD_ES_URI_PROV, things_entity_handler_cb, NULL, OC_DISCOVERABLE | OC_OBSERVABLE | OC_SECURE);
	} else {
		res = OCCreateResource(&g_prov_resource.handle, THINGS_RSRVD_ES_RES_TYPE_PROV, OC_RSRVD_INTERFACE_DEFAULT, THINGS_RSRVD_ES_URI_PROV, things_entity_handler_cb, NULL, OC_DISCOVERABLE | OC_OBSERVABLE);
	}

	if (res) {
		THINGS_LOG_D(ES_RH_TAG, "Created Prov resource with result: %s", get_result(res));
		return res;
	}

	res = OCBindResourceTypeToResource(g_prov_resource.handle, OC_RSRVD_RESOURCE_TYPE_COLLECTION);
	if (res) {
		THINGS_LOG_D(ES_RH_TAG, "Binding Resource type with result: %s", get_result(res));
		return res;
	}

	res = OCBindResourceInterfaceToResource(g_prov_resource.handle, OC_RSRVD_INTERFACE_LL);
	if (res) {
		THINGS_LOG_D(ES_RH_TAG, "Created Prov resource with result: %s", get_result(res));
		return res;
	}

	res = OCBindResourceInterfaceToResource(g_prov_resource.handle, OC_RSRVD_INTERFACE_BATCH);
	if (res) {
		THINGS_LOG_D(ES_RH_TAG, "Created Prov resource with result: %s", get_result(res));
		return res;
	}

	THINGS_LOG_D(ES_RH_TAG, "Created Prov resource with result: %s", get_result(res));
	return res;
}

OCStackResult init_wifi_resource(bool is_secured)
{
	OCStackResult res = OC_STACK_ERROR;

	g_wifi_resource.supported_freq = WiFi_BOTH;
	g_wifi_resource.supported_mode[0] = WiFi_11A;
	g_wifi_resource.supported_mode[1] = WiFi_11B;
	g_wifi_resource.supported_mode[2] = WiFi_11G;
	g_wifi_resource.supported_mode[3] = WiFi_11N;
	g_wifi_resource.supported_mode[4] = WiFi_11AC;
	g_wifi_resource.num_mode = 5;
	g_wifi_resource.auth_type = NONE_AUTH;
	g_wifi_resource.enc_type = NONE_ENC;
	memset(g_wifi_resource.ssid, 0, sizeof(char) *WIFIMGR_SSID_LEN);
	memset(g_wifi_resource.cred, 0, sizeof(char) *WIFIMGR_PASSPHRASE_LEN);
	g_wifi_resource.discovery_channel = 1;

	if (is_secured) {
		res = OCCreateResource(&g_wifi_resource.handle, THINGS_RSRVD_ES_RES_TYPE_WIFI, OC_RSRVD_INTERFACE_DEFAULT, THINGS_RSRVD_ES_URI_WIFI, things_entity_handler_cb, NULL, OC_DISCOVERABLE | OC_OBSERVABLE | OC_SECURE);
	} else {
		res = OCCreateResource(&g_wifi_resource.handle, THINGS_RSRVD_ES_RES_TYPE_WIFI, OC_RSRVD_INTERFACE_DEFAULT, THINGS_RSRVD_ES_URI_WIFI, things_entity_handler_cb, NULL, OC_DISCOVERABLE | OC_OBSERVABLE);
	}

	THINGS_LOG_D(ES_RH_TAG, "Created WiFi resource with result: %s", get_result(res));
	return res;
}

static void init_cloud_resource_data(cloud_resource_s *p_cloud_resource)
{
	if (p_cloud_resource == NULL) {
		return;
	}

	memset(p_cloud_resource->auth_code, 0, sizeof(char) *THINGS_STRING_MAX_VALUE);
	memset(p_cloud_resource->auth_provider, 0, sizeof(char) *THINGS_STRING_MAX_VALUE);
	memset(p_cloud_resource->ci_server, 0, sizeof(char) *THINGS_STRING_MAX_VALUE);
	memset(p_cloud_resource->server_id, 0, sizeof(char) *THINGS_STRING_MAX_VALUE);
	memset(p_cloud_resource->client_id, 0, sizeof(char) *THINGS_STRING_MAX_VALUE);
	memset(p_cloud_resource->accesstoken, 0, sizeof(char) *THINGS_STRING_MAX_VALUE);
	p_cloud_resource->actoken_type = 0;
	memset(p_cloud_resource->refreshtoken, 0, sizeof(char) *THINGS_STRING_MAX_VALUE);
	memset(p_cloud_resource->uid, 0, sizeof(char) *THINGS_STRING_MAX_VALUE);

}

void init_es_cloud_prov_data(es_cloud_prov_data_s *p_cloud_data)
{
	if (p_cloud_data == NULL) {
		return;
	}

	memset(p_cloud_data->auth_code, 0, sizeof(char) *THINGS_STRING_MAX_VALUE);
	memset(p_cloud_data->auth_provider, 0, sizeof(char) *THINGS_STRING_MAX_VALUE);
	memset(p_cloud_data->ci_server, 0, sizeof(char) *THINGS_STRING_MAX_VALUE);
	memset(p_cloud_data->host_name, 0, sizeof(char) *THINGS_STRING_MAX_VALUE);
	memset(p_cloud_data->ip, 0, sizeof(char) *IP_PORT);
	memset(p_cloud_data->port, 0, sizeof(char) *IP_PORT);
	memset(p_cloud_data->server_id, 0, sizeof(char) *THINGS_STRING_MAX_VALUE);
	memset(p_cloud_data->client_id, 0, sizeof(char) *THINGS_STRING_MAX_VALUE);
	memset(p_cloud_data->accesstoken, 0, sizeof(char) *THINGS_STRING_MAX_VALUE);
	p_cloud_data->actoken_type = 0;
	memset(p_cloud_data->refreshtoken, 0, sizeof(char) *THINGS_STRING_MAX_VALUE);
	memset(p_cloud_data->uid, 0, sizeof(char) *THINGS_STRING_MAX_VALUE);
}

void clone_es_cloud_prov_data(es_cloud_prov_data_s *des, es_cloud_prov_data_s *src)
{
	if (des == NULL || src == NULL) {
		return;
	}

	memcpy(des->auth_code, src->auth_code, sizeof(char) *THINGS_STRING_MAX_VALUE);
	memcpy(des->auth_provider, src->auth_provider, sizeof(char) *THINGS_STRING_MAX_VALUE);
	memcpy(des->ci_server, src->ci_server, sizeof(char) *THINGS_STRING_MAX_VALUE);
	memcpy(des->host_name, src->host_name, sizeof(char) *THINGS_STRING_MAX_VALUE);
	memcpy(des->ip, src->ip, sizeof(char) *IP_PORT);
	memcpy(des->port, src->port, sizeof(char) *IP_PORT);
	memcpy(des->server_id, src->server_id, sizeof(char) *THINGS_STRING_MAX_VALUE);
	memcpy(des->client_id, src->client_id, sizeof(char) *THINGS_STRING_MAX_VALUE);
	memcpy(des->accesstoken, src->accesstoken, sizeof(char) *THINGS_STRING_MAX_VALUE);
	des->actoken_type = src->actoken_type;
	memcpy(des->refreshtoken, src->refreshtoken, sizeof(char) *THINGS_STRING_MAX_VALUE);
	memcpy(des->uid, src->uid, sizeof(char) *THINGS_STRING_MAX_VALUE);
}

static void init_es_wifi_prov_data(es_wifi_prov_data_s *p_wifi_data)
{
	if (p_wifi_data == NULL) {
		return;
	}

	memset(p_wifi_data->ssid, 0, sizeof(char) *WIFIMGR_SSID_LEN);
	memset(p_wifi_data->pwd, 0, sizeof(char) *WIFIMGR_PASSPHRASE_LEN);
	p_wifi_data->authtype = -1;
	p_wifi_data->enctype = -1;
	p_wifi_data->discovery_channel = -1;
}

void set_ssid_in_wifi_resource(const char *ssid)
{
	if (ssid == NULL) {
		memset(g_wifi_resource.ssid, 0, sizeof(char) *WIFIMGR_SSID_LEN);
		memset(g_wifi_resource.cred, 0, sizeof(char) *WIFIMGR_PASSPHRASE_LEN);
		g_wifi_resource.auth_type = NONE_AUTH;
		g_wifi_resource.enc_type = NONE_ENC;
		g_wifi_resource.discovery_channel = 0;
	} else if (strncmp(ssid, g_wifi_data.ssid, strlen(ssid)) == 0) {
		things_strncpy(g_wifi_resource.ssid, g_wifi_data.ssid, sizeof(char) *WIFIMGR_SSID_LEN);
		things_strncpy(g_wifi_resource.cred, g_wifi_data.pwd, sizeof(char) *WIFIMGR_PASSPHRASE_LEN);
		g_wifi_resource.auth_type = g_wifi_data.authtype;
		g_wifi_resource.enc_type = g_wifi_data.enctype;
		g_wifi_resource.discovery_channel = g_wifi_data.discovery_channel;
	} else {
		things_strncpy(g_wifi_resource.ssid, ssid, sizeof(char) *WIFIMGR_SSID_LEN);
		memset(g_wifi_resource.cred, 0, sizeof(char) *WIFIMGR_PASSPHRASE_LEN);
		g_wifi_resource.auth_type = NONE_AUTH;
		g_wifi_resource.enc_type = NONE_ENC;
		g_wifi_resource.discovery_channel = 1;
	}

	THINGS_LOG_D(ES_RH_TAG, "Connected SSID=%s", ssid);
	THINGS_LOG_D(ES_RH_TAG, "g_wifi_data.ssid=%s", g_wifi_data.ssid);
	THINGS_LOG_D(ES_RH_TAG, "g_wifi_resource.ssid=%s", g_wifi_resource.ssid);
}

OCStackResult init_cloud_server_resource(bool is_secured)
{
	OCStackResult res = OC_STACK_ERROR;

	init_cloud_resource_data(&g_cloud_resource);

	if (is_secured) {
		res = OCCreateResource(&g_cloud_resource.handle, THINGS_RSRVD_ES_RES_TYPE_CLOUDSERVER, OC_RSRVD_INTERFACE_DEFAULT, THINGS_RSRVD_ES_URI_CLOUDSERVER, things_entity_handler_cb, NULL, OC_DISCOVERABLE | OC_OBSERVABLE | OC_SECURE);
	} else {
		res = OCCreateResource(&g_cloud_resource.handle, THINGS_RSRVD_ES_RES_TYPE_CLOUDSERVER, OC_RSRVD_INTERFACE_DEFAULT, THINGS_RSRVD_ES_URI_CLOUDSERVER, things_entity_handler_cb, NULL, OC_DISCOVERABLE | OC_OBSERVABLE);
	}

	THINGS_LOG_D(ES_RH_TAG, "Created CloudServer resource with result: %s", get_result(res));
	return res;
}

OCStackResult init_dev_conf_resource(bool is_secured)
{
	OCStackResult res = OC_STACK_ERROR;

	memset(g_dev_conf_resource.devName, 0, sizeof(char) *MAX_DEVICELEN);
	memset(g_dev_conf_resource.country, 0, sizeof(char) *THINGS_STRING_MAX_VALUE);
	memset(g_dev_conf_resource.language, 0, sizeof(char) *THINGS_STRING_MAX_VALUE);
	memset(g_dev_conf_resource.device_type, 0, sizeof(char) *THINGS_STRING_MAX_VALUE);
#ifdef CONFIG_ST_THINGS_SUPPORT_SUB_DEVICE
	memset(g_dev_conf_resource.device_sub_type, 0, sizeof(char) *THINGS_STRING_MAX_VALUE);
#endif
	memset(g_dev_conf_resource.datetime, 0, sizeof(char) *THINGS_STRING_MAX_VALUE);

	if (is_secured) {
		res = OCCreateResource(&g_dev_conf_resource.handle, THINGS_RSRVD_ES_RES_TYPE_DEVCONF, OC_RSRVD_INTERFACE_DEFAULT, THINGS_RSRVD_ES_URI_DEVCONF, things_entity_handler_cb, NULL, OC_DISCOVERABLE | OC_OBSERVABLE | OC_SECURE);
	} else {
		res = OCCreateResource(&g_dev_conf_resource.handle, THINGS_RSRVD_ES_RES_TYPE_DEVCONF, OC_RSRVD_INTERFACE_DEFAULT, THINGS_RSRVD_ES_URI_DEVCONF, things_entity_handler_cb, NULL, OC_DISCOVERABLE | OC_OBSERVABLE);
	}

	THINGS_LOG_D(ES_RH_TAG, "Created dev_conf_s resource with result: %s", get_result(res));
	return res;
}

void update_prov_resource(OCEntityHandlerRequest *eh_request, OCRepPayload *input)
{
	THINGS_LOG_V(ES_RH_TAG, "g_prov_resource.status %d", get_enrollee_state());

	if (eh_request->query) {
		if (strstr(eh_request->query, OC_RSRVD_INTERFACE_BATCH)) {
			// When Provisioning resource has a POST with BatchInterface
			update_cloud_resource(input);
			update_wifi_resource(input);
			update_dev_conf_resource(input);
		}
	}
}

void update_wifi_resource(OCRepPayload *input)
{
	init_es_wifi_prov_data(&g_wifi_data);

	char *ssid = NULL;
	if (OCRepPayloadGetPropString(input, THINGS_RSRVD_ES_SSID, &ssid) == false || ssid == NULL) {
		return;
	}

	THINGS_LOG_D(ES_RH_TAG, "Current AP SSID: %s", g_wifi_resource.ssid);
	if (strncmp(ssid, g_wifi_resource.ssid, strlen(ssid)) == 0) {
		THINGS_LOG_D(ES_RH_TAG, "Already connected SSID(%s).", ssid);
		things_free(ssid);
		return;
	}

	things_strncpy(g_wifi_data.ssid, ssid, sizeof(g_wifi_data.ssid));
	THINGS_LOG_D(ES_RH_TAG, "g_wifi_data.ssid : %s", g_wifi_data.ssid);

	char *cred = NULL;
	if (OCRepPayloadGetPropString(input, THINGS_RSRVD_ES_CRED, &cred)) {
		things_strncpy(g_wifi_data.pwd, cred, sizeof(g_wifi_data.pwd));
		THINGS_LOG_D(ES_RH_TAG, "g_wifi_data.pwd %s", g_wifi_data.pwd);
	}

	int64_t auth_type = -1;
	if (OCRepPayloadGetPropInt(input, THINGS_RSRVD_ES_AUTHTYPE, &auth_type)) {
		g_wifi_data.authtype = auth_type;
		THINGS_LOG_D(ES_RH_TAG, "g_wifi_data.authtype %u", g_wifi_data.authtype);
	}

	int64_t enc_type = -1;
	if (OCRepPayloadGetPropInt(input, THINGS_RSRVD_ES_ENCTYPE, &enc_type)) {
		g_wifi_data.enctype = enc_type;
		THINGS_LOG_D(ES_RH_TAG, "g_wifi_data.enctype %u", g_wifi_data.enctype);
	}

	int64_t channel = -1;
	if (OCRepPayloadGetPropInt(input, THINGS_RSRVD_ES_VENDOR_DISCOVERYCHANNEL, &channel)) {
		g_wifi_data.discovery_channel = channel;
		THINGS_LOG_D(ES_RH_TAG, "g_wifi_data.discovery_channel %u", g_wifi_data.discovery_channel);
	}

	if (ssid || cred || auth_type != -1 || enc_type != -1) {
		THINGS_LOG_V(ES_RH_TAG, "Send WiFiRsrc Callback To ES");
		PROFILING_TIME("WiFi Provisioning Start.");

		// TODO : Need to check appropriateness of g_wifi_data
		if (g_wifi_rsrc_evt_cb != NULL) {
			g_wifi_rsrc_evt_cb(ES_OK, &g_wifi_data);
		} else {
			THINGS_LOG_E(ES_RH_TAG, "g_wifi_rsrc_evt_cb is NULL");
		}
	}

	/*! Added by st_things for memory Leak fix
	 */
	if (ssid) {
		things_free(ssid);
	}
	if (cred) {
		things_free(cred);
	}
}

bool update_cloud_resource(OCRepPayload *input)
{
	bool res = false;
	init_cloud_resource_data(&g_cloud_resource);
	init_es_cloud_prov_data(&g_cloud_data);

	char *auth_code = NULL;
	if (OCRepPayloadGetPropString(input, THINGS_RSRVD_ES_AUTHCODE, &auth_code)) {
		things_strncpy(g_cloud_resource.auth_code, auth_code, sizeof(g_cloud_resource.auth_code));
		things_strncpy(g_cloud_data.auth_code, auth_code, sizeof(g_cloud_data.auth_code));
		THINGS_LOG_D(ES_RH_TAG, "g_cloud_resource.auth_code %s", g_cloud_resource.auth_code);
	}

	char *accesstoken = NULL;
	if (OCRepPayloadGetPropString(input, THINGS_RSRVD_ES_VENDOR_ACCESS_TOKEN, &accesstoken)) {
		things_strncpy(g_cloud_resource.accesstoken, accesstoken, sizeof(g_cloud_resource.accesstoken));
		things_strncpy(g_cloud_data.accesstoken, accesstoken, sizeof(g_cloud_data.accesstoken));
		THINGS_LOG_D(ES_RH_TAG, "g_cloud_resource.accesstoken %s", g_cloud_resource.accesstoken);
	}

	int accesstokenType = NULL;
	if (OCRepPayloadGetPropInt(input, THINGS_RSRVD_ES_VENDOR_ACCESS_TOKEN_TYPE, &accesstokenType)) {
		g_cloud_resource.actoken_type = accesstokenType;
		g_cloud_data.actoken_type = accesstokenType;
		THINGS_LOG_D(ES_RH_TAG, "g_cloud_resource.actoken_type %d", g_cloud_resource.actoken_type);
	}

	char *refreshtoken = NULL;
	if (OCRepPayloadGetPropString(input, SC_RSRVD_ES_VENDOR_REFRESH_TOKEN, &refreshtoken)) {
		things_strncpy(g_cloud_resource.refreshtoken, refreshtoken, sizeof(g_cloud_resource.refreshtoken));
		things_strncpy(g_cloud_data.refreshtoken, refreshtoken, sizeof(g_cloud_data.refreshtoken));
		THINGS_LOG_D(ES_RH_TAG, "g_cloud_resource.refreshtoken %s", g_cloud_resource.refreshtoken);
	}

	char *uid = NULL;
	if (OCRepPayloadGetPropString(input, THINGS_RSRVD_ES_VENDOR_USER_ID, &uid)) {
		things_strncpy(g_cloud_resource.uid, uid, sizeof(g_cloud_resource.uid));
		things_strncpy(g_cloud_data.uid, uid, sizeof(g_cloud_data.uid));
		THINGS_LOG_D(ES_RH_TAG, "g_cloud_resource.uid %s", g_cloud_resource.uid);
	}

	char *auth_provider = NULL;
	if (OCRepPayloadGetPropString(input, THINGS_RSRVD_ES_AUTHPROVIDER, &auth_provider)) {
		things_strncpy(g_cloud_resource.auth_provider, auth_provider, sizeof(g_cloud_resource.auth_provider));
		things_strncpy(g_cloud_data.auth_provider, auth_provider, sizeof(g_cloud_data.auth_provider));
		THINGS_LOG_D(ES_RH_TAG, "g_cloud_resource.auth_provider %s", g_cloud_resource.auth_provider);
	}

	char *ci_server = NULL;
	if (OCRepPayloadGetPropString(input, THINGS_RSRVD_ES_CISERVER, &ci_server)) {
		if (check_ci_server_ipv4(ci_server, &g_cloud_data) == 1) {
			things_strncpy(g_cloud_resource.ci_server, ci_server, sizeof(g_cloud_resource.ci_server));
			THINGS_LOG_D(ES_RH_TAG, "g_cloud_resource.ci_server %s", g_cloud_resource.ci_server);
		} else {
			THINGS_LOG_V(ES_RH_TAG, "check_ci_server_ipv4 is failed.");
			g_prov_resource.last_err_code = ER_ERRCODE_INVALID_PROV_PAYLOAD;
			g_prov_resource.vd_err_code = ERRCI_NO_ERROR;
		}
	}

	char *server_id = NULL;
	if (OCRepPayloadGetPropString(input, THINGS_RSRVD_ES_SERVERID, &server_id)) {
		things_strncpy(g_cloud_resource.server_id, server_id, sizeof(g_cloud_resource.server_id));
		things_strncpy(g_cloud_data.server_id, server_id, sizeof(g_cloud_data.server_id));
		THINGS_LOG_D(ES_RH_TAG, "g_cloud_resource.server_id %s", g_cloud_resource.server_id);
	}

	char *client_id = NULL;
	if (OCRepPayloadGetPropString(input, THINGS_RSRVD_ES_VENDOR_CLIENTID, &client_id)) {
		things_strncpy(g_cloud_resource.client_id, client_id, sizeof(g_cloud_resource.client_id));
		things_strncpy(g_cloud_data.client_id, client_id, sizeof(g_cloud_data.client_id));
		THINGS_LOG_D(ES_RH_TAG, "g_cloud_resource.client_id %s", g_cloud_resource.client_id);
	}

	if ((g_cloud_data.auth_code[0] || (g_cloud_data.accesstoken[0] && g_cloud_data.refreshtoken[0])) && g_cloud_data.auth_provider[0] && g_cloud_data.ci_server[0]) {
		THINGS_LOG_V(ES_RH_TAG, "Send CloudRsrc Callback To ES");
		PROFILING_TIME("Cloud Provisioning Start.");

		if (g_cloud_rsrc_evt_cb != NULL) {
			es_set_state(ES_STATE_INIT);
			g_prov_resource.last_err_code = ERRCI_NO_ERROR;
			g_prov_resource.vd_err_code = ERRCI_NO_ERROR;

			g_cloud_rsrc_evt_cb(ES_OK, &g_cloud_data);
			res = true;
		} else {
			THINGS_LOG_E(ES_RH_TAG, "g_cloud_rsrc_evt_cb is NULL");
			g_prov_resource.last_err_code = ER_ERRCODE_SYSTEM_ERROR;
			g_prov_resource.vd_err_code = ERRCI_NO_ERROR;
		}
	} else {
		THINGS_LOG_D(ES_RH_TAG, "Cloud Provisioning Data is invalid: auth_code=%s auth_provider=%s ci_server=%s", g_cloud_data.auth_code, g_cloud_data.auth_provider, g_cloud_data.ci_server);
	}

	if (auth_code) {
		things_free(auth_code);
	}

	if (accesstoken) {
		things_free(accesstoken);
	}

	/*! Added by st_things for leak fix */
	if (refreshtoken) {
		things_free(refreshtoken);
	}
	if (uid) {
		things_free(uid);
	}

	if (auth_provider) {
		things_free(auth_provider);
	}

	if (ci_server) {
		things_free(ci_server);
	}

	if (server_id) {
		things_free(server_id);
	}

	if (client_id) {
		things_free(client_id);
	}

	return res;
}

void update_dev_conf_resource(OCRepPayload *input)
{
	char *country = NULL;
	if (OCRepPayloadGetPropString(input, THINGS_RSRVD_ES_COUNTRY, &country)) {
		things_strncpy(g_dev_conf_resource.country, country, sizeof(g_dev_conf_resource.country));
		things_strncpy(g_dev_conf_data.country, country, sizeof(g_dev_conf_data.country));
		THINGS_LOG_D(ES_RH_TAG, "g_dev_conf_resource.country %s", g_dev_conf_resource.country);
	}

	char *language = NULL;
	if (OCRepPayloadGetPropString(input, THINGS_RSRVD_ES_LANGUAGE, &language)) {
		things_strncpy(g_dev_conf_resource.language, language, sizeof(g_dev_conf_resource.language));
		things_strncpy(g_dev_conf_data.language, language, sizeof(g_dev_conf_data.language));
		THINGS_LOG_D(ES_RH_TAG, "g_dev_conf_resource.language %s", g_dev_conf_resource.language);
	}

	char *datetime = NULL;
	if (OCRepPayloadGetPropString(input, THINGS_RSRVD_ES_VENDOR_UTC_DATE_TIME, &datetime)) {
		things_strncpy(g_dev_conf_resource.datetime, datetime, sizeof(g_dev_conf_resource.datetime));
		things_strncpy(g_dev_conf_data.datetime, datetime, sizeof(g_dev_conf_data.datetime));
		THINGS_LOG_D(ES_RH_TAG, "g_dev_conf_resource.datetime %s", g_dev_conf_resource.datetime);
	}

	if (country || language || datetime) {
		THINGS_LOG_V(ES_RH_TAG, "Send DevConfRsrc Callback To ES");

		// TODO : Need to check appropriateness of g_dev_conf_data
		if (g_dev_conf_rsrc_evt_cb != NULL) {
			g_dev_conf_rsrc_evt_cb(ES_OK, &g_dev_conf_data);
		} else {
			THINGS_LOG_E(ES_RH_TAG, "g_dev_conf_rsrc_evt_cb is NULL");
		}
	}

	/*! Added by st_things for memory Leak fix
	 */
	if (country) {
		things_free(country);
	}
	if (language) {
		things_free(language);
	}
	if (datetime) {
		things_free(datetime);
	}
}

OCRepPayload *construct_response_of_wifi(const char *interface)
{
	OCRepPayload *payload = OCRepPayloadCreate();
	if (!payload) {
		THINGS_LOG_E(ES_RH_TAG, "Failed to allocate Payload");
		return NULL;
	}

	THINGS_LOG_D(ES_RH_TAG, "constructResponse wifi res");
	OCRepPayloadSetUri(payload, THINGS_RSRVD_ES_URI_WIFI);

	OCRepPayload *rep_payload = NULL;
	OCRepPayload *temp_payload = NULL;
	if (interface != NULL && !strncmp(interface, OC_RSRVD_INTERFACE_BATCH, strlen(OC_RSRVD_INTERFACE_BATCH))) {	// Temporary Code.
		char *interfaces[1] = { NULL, };
		char *resource_types[1] = { NULL, };
		size_t interfaces_dimensions[MAX_REP_ARRAY_DEPTH] = { 1, 0, 0 };
		size_t resource_types_dimensions[MAX_REP_ARRAY_DEPTH] = { 1, 0, 0 };

		rep_payload = OCRepPayloadCreate();
		if (!rep_payload) {
			THINGS_LOG_E(ES_RH_TAG, "Failed to allocate rep_payload");
			goto GOTO_ERROR;
		}
		// Swap
		temp_payload = payload;
		payload = rep_payload;

		interfaces[0] = things_strdup(OC_RSRVD_INTERFACE_DEFAULT);
		OCRepPayloadSetStringArray(payload, OC_RSRVD_INTERFACE, (const char **)interfaces, interfaces_dimensions);

		resource_types[0] = things_strdup(THINGS_RSRVD_ES_RES_TYPE_WIFI);
		OCRepPayloadSetStringArray(payload, OC_RSRVD_RESOURCE_TYPE, (const char **)resource_types, resource_types_dimensions);

    /*! Added by st_things for memory Leak fix
		 */
		if (interfaces[0]) {
			things_free(interfaces[0]);
		}
		if (resource_types[0]) {
			things_free(resource_types[0]);
		}
	} else {
		OCRepPayloadAddInterface(payload, OC_RSRVD_INTERFACE_DEFAULT);
		OCRepPayloadAddResourceType(payload, THINGS_RSRVD_ES_RES_TYPE_WIFI);
	}

	size_t dimensions[MAX_REP_ARRAY_DEPTH] = { g_wifi_resource.num_mode, 0, 0 };
	int64_t *modes_64 = (int64_t *)things_malloc(g_wifi_resource.num_mode * sizeof(int64_t));
	for (int i = 0; i < g_wifi_resource.num_mode; ++i) {
		modes_64[i] = g_wifi_resource.supported_mode[i];
	}

	OCRepPayloadSetIntArray(payload, THINGS_RSRVD_ES_SUPPORTEDWIFIMODE, (int64_t *) modes_64, dimensions);
	OCRepPayloadSetPropInt(payload, THINGS_RSRVD_ES_SUPPORTEDWIFIFREQ, g_wifi_resource.supported_freq);
	OCRepPayloadSetPropString(payload, THINGS_RSRVD_ES_SSID, g_wifi_resource.ssid);
	OCRepPayloadSetPropString(payload, THINGS_RSRVD_ES_CRED, g_wifi_resource.cred);
	OCRepPayloadSetPropInt(payload, THINGS_RSRVD_ES_AUTHTYPE, (int)g_wifi_resource.auth_type);
	OCRepPayloadSetPropInt(payload, THINGS_RSRVD_ES_ENCTYPE, (int)g_wifi_resource.enc_type);
	OCRepPayloadSetPropInt(payload, THINGS_RSRVD_ES_VENDOR_DISCOVERYCHANNEL, (int)g_wifi_resource.discovery_channel);

	if (interface != NULL && !strncmp(interface, OC_RSRVD_INTERFACE_BATCH, strlen(OC_RSRVD_INTERFACE_BATCH))) {
		payload = temp_payload;
		OCRepPayloadSetPropObject(payload, OC_RSRVD_REPRESENTATION, rep_payload);
		OCPayloadDestroy((OCPayload *) rep_payload);
		rep_payload = NULL;
		temp_payload = NULL;
	}
	/*! Added by st_things for memory Leak fix
	 */
	if (temp_payload != NULL) {
		OCPayloadDestroy((OCPayload *) temp_payload);
	}
	/*! Added by st_things for memory Leak fix
	 */
	if (modes_64 != NULL) {
		things_free(modes_64);
	}

	return payload;

GOTO_ERROR:
	if (payload != NULL) {
		OCPayloadDestroy((OCPayload *) payload);
		payload = NULL;
	}

	return NULL;
}

OCRepPayload *construct_response_of_cloud(const char *interface)
{
	OCRepPayload *payload = OCRepPayloadCreate();
	if (!payload) {
		THINGS_LOG_E(ES_RH_TAG, "Failed to allocate Payload");
		return NULL;
	}

	THINGS_LOG_V(ES_RH_TAG, "constructResponse cloud res");
	OCRepPayloadSetUri(payload, THINGS_RSRVD_ES_URI_CLOUDSERVER);

	OCRepPayload *rep_payload = NULL;
	OCRepPayload *temp_payload = NULL;
	if (interface != NULL && !strncmp(interface, OC_RSRVD_INTERFACE_BATCH, strlen(OC_RSRVD_INTERFACE_BATCH))) {	// Temporary Code.
		char *interfaces[1] = { NULL, };
		char *resource_types[1] = { NULL, };
		size_t interfaces_dimensions[MAX_REP_ARRAY_DEPTH] = { 1, 0, 0 };
		size_t resource_types_dimensions[MAX_REP_ARRAY_DEPTH] = { 1, 0, 0 };

		rep_payload = OCRepPayloadCreate();
		if (!rep_payload) {
			THINGS_LOG_E(ES_RH_TAG, "Failed to allocate rep_payload");
			goto GOTO_ERROR;
		}
		// Swap
		temp_payload = payload;
		payload = rep_payload;

		interfaces[0] = things_strdup(OC_RSRVD_INTERFACE_DEFAULT);
		OCRepPayloadSetStringArray(payload, OC_RSRVD_INTERFACE, (const char **)interfaces, interfaces_dimensions);

		resource_types[0] = things_strdup(THINGS_RSRVD_ES_RES_TYPE_CLOUDSERVER);
		OCRepPayloadSetStringArray(payload, OC_RSRVD_RESOURCE_TYPE, (const char **)resource_types, resource_types_dimensions);

		/*! Added by st_things for memory Leak fix
		 */
		if (interfaces[0]) {
			things_free(interfaces[0]);
		}
		if (resource_types[0]) {
			things_free(resource_types[0]);
		}
	} else {
		OCRepPayloadAddInterface(payload, OC_RSRVD_INTERFACE_DEFAULT);
		OCRepPayloadAddResourceType(payload, THINGS_RSRVD_ES_RES_TYPE_CLOUDSERVER);
	}

	OCRepPayloadSetPropString(payload, THINGS_RSRVD_ES_AUTHCODE, g_cloud_resource.auth_code);
	OCRepPayloadSetPropString(payload, THINGS_RSRVD_ES_AUTHPROVIDER, g_cloud_resource.auth_provider);
	OCRepPayloadSetPropString(payload, THINGS_RSRVD_ES_CISERVER, g_cloud_resource.ci_server);
	OCRepPayloadSetPropString(payload, THINGS_RSRVD_ES_SERVERID, g_cloud_resource.server_id);
	OCRepPayloadSetPropString(payload, THINGS_RSRVD_ES_VENDOR_CLIENTID, g_cloud_resource.client_id);
	OCRepPayloadSetPropString(payload, THINGS_RSRVD_ES_VENDOR_ACCESS_TOKEN, g_cloud_resource.accesstoken);
	OCRepPayloadSetPropInt(payload, THINGS_RSRVD_ES_VENDOR_ACCESS_TOKEN_TYPE, g_cloud_resource.actoken_type);
	OCRepPayloadSetPropString(payload, SC_RSRVD_ES_VENDOR_REFRESH_TOKEN, g_cloud_resource.refreshtoken);
	OCRepPayloadSetPropString(payload, THINGS_RSRVD_ES_VENDOR_USER_ID, g_cloud_resource.uid);

	if (interface != NULL && !strncmp(interface, OC_RSRVD_INTERFACE_BATCH, strlen(OC_RSRVD_INTERFACE_BATCH))) {
		payload = temp_payload;
		OCRepPayloadSetPropObject(payload, OC_RSRVD_REPRESENTATION, rep_payload);
		OCPayloadDestroy((OCPayload *) rep_payload);
		rep_payload = NULL;
		temp_payload = NULL;
	}
	/*! Added by st_things for memory Leak fix
	 */
	if (temp_payload != NULL) {
		OCPayloadDestroy((OCPayload *) temp_payload);
	}

	return payload;

GOTO_ERROR:
	if (payload != NULL) {
		OCPayloadDestroy((OCPayload *) payload);
		payload = NULL;
	}

	return NULL;
}

OCRepPayload *construct_response_of_dev_conf(const char *interface)
{
	OCRepPayload *payload = OCRepPayloadCreate();
	if (!payload) {
		THINGS_LOG_E(ES_RH_TAG, "Failed to allocate Payload");
		return NULL;
	}

	THINGS_LOG_V(ES_RH_TAG, "constructResponse devConf res");
	OCRepPayloadSetUri(payload, THINGS_RSRVD_ES_URI_DEVCONF);

	OCRepPayload *rep_payload = NULL;
	OCRepPayload *temp_payload = NULL;
	if (interface != NULL && !strncmp(interface, OC_RSRVD_INTERFACE_BATCH, strlen(OC_RSRVD_INTERFACE_BATCH))) {	// Temporary Code.
		char *interfaces[1] = { NULL, };
		char *resource_types[1] = { NULL, };
		size_t interfaces_dimensions[MAX_REP_ARRAY_DEPTH] = { 1, 0, 0 };
		size_t resource_types_dimensions[MAX_REP_ARRAY_DEPTH] = { 1, 0, 0 };

		rep_payload = OCRepPayloadCreate();
		if (!rep_payload) {
			THINGS_LOG_E(ES_RH_TAG, "Failed to allocate rep_payload");
			goto GOTO_ERROR;
		}
		// Swap
		temp_payload = payload;
		payload = rep_payload;

		interfaces[0] = things_strdup(OC_RSRVD_INTERFACE_DEFAULT);
		OCRepPayloadSetStringArray(payload, OC_RSRVD_INTERFACE, (const char **)interfaces, interfaces_dimensions);

		resource_types[0] = things_strdup(THINGS_RSRVD_ES_RES_TYPE_DEVCONF);
		OCRepPayloadSetStringArray(payload, OC_RSRVD_RESOURCE_TYPE, (const char **)resource_types, resource_types_dimensions);
		/*! Added by st_things for memory Leak fix
		 */
		if (interfaces[0]) {
			things_free(interfaces[0]);
		}
		if (resource_types[0]) {
			things_free(resource_types[0]);
		}
	} else {
		OCRepPayloadAddInterface(payload, OC_RSRVD_INTERFACE_DEFAULT);
		OCRepPayloadAddResourceType(payload, THINGS_RSRVD_ES_RES_TYPE_DEVCONF);
	}

	OCRepPayloadSetPropString(payload, THINGS_RSRVD_ES_DEVNAME, g_dev_conf_resource.devName);
	OCRepPayloadSetPropString(payload, THINGS_RSRVD_ES_LANGUAGE, g_dev_conf_resource.language);
	OCRepPayloadSetPropString(payload, THINGS_RSRVD_ES_COUNTRY, g_dev_conf_resource.country);
	OCRepPayloadSetPropString(payload, THINGS_RSRVD_ES_VENDOR_DEVTYPE, g_dev_conf_resource.device_type);
#ifdef CONFIG_ST_THINGS_SUPPORT_SUB_DEVICE
	OCRepPayloadSetPropString(payload, THINGS_RSRVD_ES_VENDOR_DEVSUBTYPE, g_dev_conf_resource.device_sub_type);
#endif
	OCRepPayloadSetPropString(payload, THINGS_RSRVD_ES_VENDOR_UTC_DATE_TIME, g_dev_conf_resource.datetime);

	if (interface != NULL && !strncmp(interface, OC_RSRVD_INTERFACE_BATCH, strlen(OC_RSRVD_INTERFACE_BATCH))) {
		payload = temp_payload;
		OCRepPayloadSetPropObject(payload, OC_RSRVD_REPRESENTATION, rep_payload);
		OCPayloadDestroy((OCPayload *) rep_payload);
		rep_payload = NULL;
		temp_payload = NULL;
	}
	/*! Added by st_things for memory Leak fix
	 */
	if (temp_payload != NULL) {
		OCPayloadDestroy((OCPayload *) temp_payload);
	}

	return payload;

GOTO_ERROR:
	if (payload != NULL) {
		OCPayloadDestroy((OCPayload *) payload);
		payload = NULL;
	}

	return NULL;
}

OCRepPayload *construct_response_of_prov(OCEntityHandlerRequest *eh_request)
{
	int childResCnt = 0;
	const OCRepPayload *arrayPayload[3] = { NULL, };
	OCRepPayload *payload = NULL;

	if (g_wifi_resource.handle == NULL || g_dev_conf_resource.handle == NULL || g_cloud_resource.handle == NULL) {
		THINGS_LOG_E(ES_RH_TAG, "mandatory Resource handle is NULL.(WiFi=0x%X, dev_conf_s=0x%X, Cloud=0x%X)", g_wifi_resource.handle, g_dev_conf_resource.handle, g_cloud_resource.handle);
		return NULL;
	}

	if ((payload = OCRepPayloadCreate()) == NULL) {
		THINGS_LOG_E(ES_RH_TAG, "Failed to allocate Payload");
		return NULL;
	}
	// Requested interface is Link list interface
	if (!eh_request->query || (eh_request->query && (strlen(eh_request->query) > 0)) || (eh_request->query && compare_resource_interface(eh_request->query, OC_RSRVD_INTERFACE_LL)) || (eh_request->query && compare_resource_interface(eh_request->query, OC_RSRVD_INTERFACE_DEFAULT))) {
		if ((arrayPayload[childResCnt] = make_rep_payload(g_wifi_resource.handle, &eh_request->devAddr)) == NULL) {
			THINGS_LOG_E(ES_RH_TAG, "It's failed making payload of wifi_resource_s-Representation");
			goto GOTO_FAILED;
		}
		childResCnt++;

		if ((arrayPayload[childResCnt] = make_rep_payload(g_dev_conf_resource.handle, &eh_request->devAddr)) == NULL) {
			THINGS_LOG_E(ES_RH_TAG, "It's failed making payload of dev_conf_resource_s-Representation");
			goto GOTO_FAILED;
		}
		childResCnt++;

		if ((arrayPayload[childResCnt] = make_rep_payload(g_cloud_resource.handle, &eh_request->devAddr)) == NULL) {
			THINGS_LOG_E(ES_RH_TAG, "It's failed making payload of cloud_resource_s-Representation");
			goto GOTO_FAILED;
		}
		childResCnt++;

		size_t dimensions[MAX_REP_ARRAY_DEPTH] = { childResCnt, 0, 0 };

		if (!eh_request->query || !strncmp(eh_request->query, "", strlen("")) || compare_resource_interface(eh_request->query, OC_RSRVD_INTERFACE_LL) == false) {
			THINGS_LOG_V(ES_RH_TAG, "constructResponse prov res");
			OCRepPayloadSetUri(payload, THINGS_RSRVD_ES_URI_PROV);
			OCRepPayloadAddInterface(payload, OC_RSRVD_INTERFACE_DEFAULT);
			OCRepPayloadAddInterface(payload, OC_RSRVD_INTERFACE_LL);
			OCRepPayloadAddInterface(payload, OC_RSRVD_INTERFACE_BATCH);
			OCRepPayloadAddResourceType(payload, THINGS_RSRVD_ES_RES_TYPE_PROV);
			OCRepPayloadAddResourceType(payload, OC_RSRVD_RESOURCE_TYPE_COLLECTION);

			OCRepPayloadSetPropInt(payload, THINGS_RSRVD_ES_PROVSTATUS, get_enrollee_state());
			OCRepPayloadSetPropInt(payload, THINGS_RSRVD_ES_LATHINGS_ERRORCODE, g_prov_resource.last_err_code);
			OCRepPayloadSetPropInt(payload, THINGS_RSRVD_ES_VENDOR_ERRORCODE, g_prov_resource.vd_err_code);
		}

		OCRepPayloadSetPropObjectArray(payload, THINGS_RSRVD_ES_LINKS, arrayPayload, dimensions);

		for (int i = 0; i < childResCnt; i++) {
			OCPayloadDestroy((OCPayload *) arrayPayload[i]);
		}

	} else if (eh_request->query && compare_resource_interface(eh_request->query, OC_RSRVD_INTERFACE_BATCH)) {
		THINGS_LOG_V(ES_RH_TAG, "constructResponse prov res");
		OCRepPayloadSetUri(payload, THINGS_RSRVD_ES_URI_PROV);

		OCRepPayload *rep_payload = NULL;
		char *interfaces[3] = { NULL, };
		char *resource_types[2] = { NULL, };
		size_t interfaces_dimensions[MAX_REP_ARRAY_DEPTH] = { 3, 0, 0 };
		size_t resource_types_dimensions[MAX_REP_ARRAY_DEPTH] = { 2, 0, 0 };

		rep_payload = OCRepPayloadCreate();
		if (!rep_payload) {
			THINGS_LOG_E(ES_RH_TAG, "Failed to allocate rep_payload");
			goto GOTO_FAILED;
		}

		interfaces[0] = things_strdup(OC_RSRVD_INTERFACE_DEFAULT);
		interfaces[1] = things_strdup(OC_RSRVD_INTERFACE_LL);
		interfaces[2] = things_strdup(OC_RSRVD_INTERFACE_BATCH);
		OCRepPayloadSetStringArray(rep_payload, OC_RSRVD_INTERFACE, (const char **)interfaces, interfaces_dimensions);

		resource_types[0] = things_strdup(THINGS_RSRVD_ES_RES_TYPE_PROV);
		resource_types[1] = things_strdup(OC_RSRVD_RESOURCE_TYPE_COLLECTION);
		OCRepPayloadSetStringArray(rep_payload, OC_RSRVD_RESOURCE_TYPE, (const char **)resource_types, resource_types_dimensions);

		OCRepPayloadSetPropInt(rep_payload, THINGS_RSRVD_ES_PROVSTATUS, get_enrollee_state());
		OCRepPayloadSetPropInt(rep_payload, THINGS_RSRVD_ES_LATHINGS_ERRORCODE, g_prov_resource.last_err_code);
		OCRepPayloadSetPropInt(rep_payload, THINGS_RSRVD_ES_VENDOR_ERRORCODE, g_prov_resource.vd_err_code);

		OCRepPayloadSetPropObject(payload, OC_RSRVD_REPRESENTATION, rep_payload);
		OCPayloadDestroy((OCPayload *) rep_payload);
		/*! Added by st_things for memory Leak fix
		 */
		if (interfaces[0]) {
			things_free(interfaces[0]);
		}
		if (interfaces[1]) {
			things_free(interfaces[1]);
		}
		if (interfaces[2]) {
			things_free(interfaces[2]);
		}
		if (resource_types[0]) {
			things_free(resource_types[0]);
		}
		if (resource_types[1]) {
			things_free(resource_types[1]);
		}
	}

	if (eh_request->query) {
		if (compare_resource_interface(eh_request->query, OC_RSRVD_INTERFACE_BATCH)) {	// When Provisioning resource has a GET with BatchInterface
			OCRepPayload *head = payload;

			payload->next = construct_response_of_wifi(OC_RSRVD_INTERFACE_BATCH);
			if (payload->next == NULL) {
				goto GOTO_FAILED;
			}

			payload = payload->next;
			payload->next = construct_response_of_cloud(OC_RSRVD_INTERFACE_BATCH);
			if (payload->next == NULL) {
				payload = head;
				goto GOTO_FAILED;
			}

			payload = payload->next;
			payload->next = construct_response_of_dev_conf(OC_RSRVD_INTERFACE_BATCH);
			if (payload->next == NULL) {
				payload = head;
				goto GOTO_FAILED;
			}

			payload = head;
		}
	}

	return payload;

GOTO_FAILED:
	for (int i = 0; i < childResCnt; i++) {
		OCPayloadDestroy((OCPayload *) arrayPayload[i]);
	}

	delete_child_payload(payload);

	return NULL;
}

OCStackResult create_easysetup_resources(bool is_secured, es_resource_mask_e resource_mask)
{
	OCStackResult res = OC_STACK_ERROR;
	bool maskFlag = false;

	res = init_prov_resource(is_secured);
	if (res != OC_STACK_OK) {
		// TODO: destroy logic will be added
		THINGS_LOG_V(ES_RH_TAG, "init_prov_resource result: %s", get_result(res));

		return res;
	}

	if ((resource_mask & ES_WIFI_RESOURCE) == ES_WIFI_RESOURCE) {
		maskFlag = true;
		res = init_wifi_resource(is_secured);
		if (res != OC_STACK_OK) {
			THINGS_LOG_V(ES_RH_TAG, "init_wifi_resource result: %s", get_result(res));
			return res;
		}

		res = OCBindResource(g_prov_resource.handle, g_wifi_resource.handle);
		if (res != OC_STACK_OK) {
			THINGS_LOG_V(ES_RH_TAG, "bind wifi_resource_s result: %s", get_result(res));
			return res;
		}

	}

	if ((resource_mask & ES_CLOUD_RESOURCE) == ES_CLOUD_RESOURCE) {
		maskFlag = true;
		res = init_cloud_server_resource(is_secured);
		if (res != OC_STACK_OK) {
			THINGS_LOG_V(ES_RH_TAG, "initCloudResource result: %s", get_result(res));
			return res;
		}

		res = OCBindResource(g_prov_resource.handle, g_cloud_resource.handle);
		if (res != OC_STACK_OK) {
			THINGS_LOG_V(ES_RH_TAG, "bind cloud_resource_s result: %s", get_result(res));
			return res;
		}
	}

	if ((resource_mask & ES_DEVCONF_RESOURCE) == ES_DEVCONF_RESOURCE) {
		maskFlag = true;
		res = init_dev_conf_resource(is_secured);
		if (res != OC_STACK_OK) {
			THINGS_LOG_V(ES_RH_TAG, "initDevConf result: %s", get_result(res));
			return res;
		}

		res = OCBindResource(g_prov_resource.handle, g_dev_conf_resource.handle);
		if (res != OC_STACK_OK) {
			THINGS_LOG_V(ES_RH_TAG, "bind dev_conf_resource_s result: %s", get_result(res));
			return res;
		}
	}

	if (maskFlag == false) {
		THINGS_LOG_V(ES_RH_TAG, "Invalid ResourceMask");
		return OC_STACK_ERROR;

	}

	THINGS_LOG_D(ES_RH_TAG, "Created all resources with result: %s", get_result(res));

	return res;
}

OCStackResult delete_provisioning_resource()
{
	OCStackResult res = OCDeleteResource(g_prov_resource.handle);
	if (res != OC_STACK_OK) {
		THINGS_LOG_V(ES_RH_TAG, "Deleting Prov resource error with result: %s", get_result(res));
	}

	return res;
}

OCStackResult delete_easysetup_resources()
{
	OCStackResult res = OC_STACK_ERROR;
	if (g_wifi_resource.handle != NULL) {
		res = OCUnBindResource(g_prov_resource.handle, g_wifi_resource.handle);
		if (res != OC_STACK_OK) {
			THINGS_LOG_V(ES_RH_TAG, "Unbind wifi_resource_s error with result: %s", get_result(res));
		}
	}
	if (g_cloud_resource.handle != NULL) {
		res = OCUnBindResource(g_prov_resource.handle, g_cloud_resource.handle);
		if (res != OC_STACK_OK) {
			THINGS_LOG_V(ES_RH_TAG, "Unbind cloud_resource_s error with result: %s", get_result(res));
		}
	}
	if (g_dev_conf_resource.handle != NULL) {
		res = OCUnBindResource(g_prov_resource.handle, g_dev_conf_resource.handle);
		if (res != OC_STACK_OK) {
			THINGS_LOG_V(ES_RH_TAG, "Unbind dev_conf_resource_s error with result: %s", get_result(res));
		}
	}

	if (g_wifi_resource.handle != NULL) {
		res = OCDeleteResource(g_wifi_resource.handle);
		if (res != OC_STACK_OK) {
			THINGS_LOG_V(ES_RH_TAG, "Deleting WiFi resource error with result: %s", get_result(res));
		}
	}

	if (g_cloud_resource.handle != NULL) {
		res = OCDeleteResource(g_cloud_resource.handle);
		if (res != OC_STACK_OK) {
			THINGS_LOG_V(ES_RH_TAG, "Deleting CloudServer resource error with result: %s", get_result(res));
		}
	}

	if (g_dev_conf_resource.handle != NULL) {
		res = OCDeleteResource(g_dev_conf_resource.handle);
		if (res != OC_STACK_OK) {
			THINGS_LOG_V(ES_RH_TAG, "Deleting dev_conf_s resource error with result: %s", get_result(res));
		}
	}

	if (g_prov_resource.handle != NULL) {
		res = OCDeleteResource(g_prov_resource.handle);
		if (res != OC_STACK_OK) {
			THINGS_LOG_V(ES_RH_TAG, "Deleting Prov resource error with result: %s", get_result(res));
		}
	}

	return res;
}

OCEntityHandlerResult process_get_request(OCEntityHandlerRequest *eh_request, OCRepPayload **payload)
{
	OCEntityHandlerResult eh_result = OC_EH_ERROR;
	if (!eh_request) {
		THINGS_LOG_E(ES_RH_TAG, "Request is Null");
		return eh_result;
	}
	if (eh_request->payload && eh_request->payload->type != PAYLOAD_TYPE_REPRESENTATION) {
		THINGS_LOG_E(ES_RH_TAG, "Incoming payload not a representation");
		return eh_result;
	}

	OCRepPayload *getResp = NULL;

	if (eh_request->resource == g_prov_resource.handle) {
		getResp = construct_response_of_prov(eh_request);
	} else if (eh_request->resource == g_wifi_resource.handle) {
		getResp = construct_response_of_wifi(OC_RSRVD_INTERFACE_DEFAULT);
	} else if (eh_request->resource == g_cloud_resource.handle) {
		getResp = construct_response_of_cloud(OC_RSRVD_INTERFACE_DEFAULT);
	} else if (eh_request->resource == g_dev_conf_resource.handle) {
		getResp = construct_response_of_dev_conf(OC_RSRVD_INTERFACE_DEFAULT);
	}

	if (!getResp) {
		THINGS_LOG_E(ES_RH_TAG, "constructResponse failed");
		return OC_EH_ERROR;
	}

	*payload = getResp;
	eh_result = OC_EH_OK;
	//eh_result = OC_EH_CONTENT;

	return eh_result;
}

OCEntityHandlerResult process_post_request(OCEntityHandlerRequest *eh_request, OCRepPayload **payload)
{
	THINGS_LOG_D(ES_RH_TAG, "process_post_request enter");
	OCEntityHandlerResult eh_result = OC_EH_ERROR;
	if (eh_request->payload && eh_request->payload->type != PAYLOAD_TYPE_REPRESENTATION) {
		THINGS_LOG_E(ES_RH_TAG, "Incoming payload not a representation");
		return eh_result;
	}

	OCRepPayload *input = (OCRepPayload *)(eh_request->payload);
	if (!input) {
		THINGS_LOG_E(ES_RH_TAG, "Failed to parse");
		return eh_result;
	}
	// TBD : Discuss about triggering flag (to be existed or not)
	// ES_PS_PROVISIONING_COMPLETED state indicates that already provisioning is completed.
	// A new request for provisioning means overriding existing network provisioning information.

	if (eh_request->resource == g_prov_resource.handle) {
		update_prov_resource(eh_request, input);
	} else if (eh_request->resource == g_wifi_resource.handle) {
		update_wifi_resource(input);
	} else if (eh_request->resource == g_cloud_resource.handle) {
		update_cloud_resource(input);
	} else if (eh_request->resource == g_dev_conf_resource.handle) {
		update_dev_conf_resource(input);
	}

	OCRepPayload *getResp = NULL;
	if (eh_request->resource == g_prov_resource.handle) {
		getResp = construct_response_of_prov(eh_request);
	} else if (eh_request->resource == g_wifi_resource.handle) {
		getResp = construct_response_of_wifi(OC_RSRVD_INTERFACE_DEFAULT);
	} else if (eh_request->resource == g_cloud_resource.handle) {
		getResp = construct_response_of_cloud(OC_RSRVD_INTERFACE_DEFAULT);
	} else if (eh_request->resource == g_dev_conf_resource.handle) {
		getResp = construct_response_of_dev_conf(OC_RSRVD_INTERFACE_DEFAULT);
	}

	if (!getResp) {
		THINGS_LOG_E(ES_RH_TAG, "constructResponse failed");
		return OC_EH_ERROR;
	}

	*payload = getResp;
	eh_result = OC_EH_OK;
	//eh_result = OC_EH_CHANGED;

	return eh_result;
}

OCEntityHandlerResult process_put_request(OCEntityHandlerRequest *eh_request, OCRepPayload **payload)
{
	(void)eh_request;
	(void)payload;
	OCEntityHandlerResult eh_result = OC_EH_ERROR;

	return eh_result;
}

/**
 * This is the entity handler for the registered resource.
 * This is invoked by OCStack whenever it recevies a request for this resource.
 */
OCEntityHandlerResult things_entity_handler_cb(OCEntityHandlerFlag flag, OCEntityHandlerRequest *entity_handler_request, void *callback)
{
	(void)callback;
	OCEntityHandlerResult eh_ret = OC_EH_ERROR;
	OCEntityHandlerResponse response;
	OCRepPayload *payload = NULL;

	THINGS_LOG_D(ES_RH_TAG, "Called OCEntityHandler.");

	// Validate pointer
	if (!entity_handler_request) {
		THINGS_LOG_E(ES_RH_TAG, "Invalid request pointer");
		return OC_EH_ERROR;
	}

	if (flag & OC_REQUEST_FLAG) {
		if (things_get_reset_mask(RST_EASY_SETUP_DISABLE) == true) {
			THINGS_LOG_V(ES_RH_TAG, "Easy Setup Disable.");
			eh_ret = OC_EH_NOT_ACCEPTABLE;
		} else if (g_prov_resource.handle != NULL) {
			if (OC_REST_GET == entity_handler_request->method) {
				THINGS_LOG_V(ES_RH_TAG, "Received GET request");

				eh_ret = process_get_request(entity_handler_request, &payload);

			} else if (OC_REST_POST == entity_handler_request->method) {
				THINGS_LOG_V(ES_RH_TAG, "Received OC_REST_POST from client");

				eh_ret = process_post_request(entity_handler_request, &payload);
			} else {
				THINGS_LOG_E(ES_RH_TAG, "Not yet Supported Request case(%d).", entity_handler_request->method);
				eh_ret = OC_EH_ERROR;
			}
		} else {
			THINGS_LOG_E(ES_RH_TAG, "EZ-Setup Resource not Registered Yet~!!!!!");
		}
		
		THINGS_LOG_V(ES_RH_TAG, "\t\tRespone : (%d) ", (int)eh_ret);

		response.numSendVendorSpecificHeaderOptions = 0;
		memset(response.sendVendorSpecificHeaderOptions, 0, sizeof(response.sendVendorSpecificHeaderOptions));
		memset(response.resourceUri, 0, sizeof(response.resourceUri));

		// Format the response.  Note this requires some info about the request
		response.requestHandle = entity_handler_request->requestHandle;
		response.resourceHandle = entity_handler_request->resource;
		response.ehResult = eh_ret;
		response.payload = (OCPayload *)(payload);
		// Indicate that response is NOT in a persistent buffer
		response.persistentBufferFlag = 0;

		// Send the response
		if (OCDoResponse(&response) != OC_STACK_OK) {
			THINGS_LOG_E(ES_RH_TAG, "Error sending response");
			eh_ret = OC_EH_ERROR;
		}
		/*! Added by st_things for memory Leak fix
			*/
		if (payload) {
			OCPayloadDestroy((OCPayload *) payload);
		}
		payload = NULL;
	}

	if (flag & OC_OBSERVE_FLAG) {
		THINGS_LOG_D(ES_RH_TAG, "Flag includes OC_OBSERVE_FLAG");
		if (OC_OBSERVE_REGISTER == entity_handler_request->obsInfo.action) {
			THINGS_LOG_V(ES_RH_TAG, "Received OC_OBSERVE_REGISTER from client");
		} else if (OC_OBSERVE_DEREGISTER == entity_handler_request->obsInfo.action) {
			THINGS_LOG_V(ES_RH_TAG, "Received OC_OBSERVE_DEREGISTER from client");
		}
	}

	THINGS_LOG_D(ES_RH_TAG, "Common Exit OCEntityHandler.");

	set_wifi_prov_state(WIFI_RESPONSE);

	return eh_ret;
}

OCStackResult prov_rsc_notify_all_observers(void)
{
	OCStackResult ret = OCNotifyAllObservers(g_prov_resource.handle, OC_HIGH_QOS);

	switch (ret) {
	case OC_STACK_OK:
		THINGS_LOG_V(ES_RH_TAG, "Sent notification to Cloud Resource Observers");
		break;
	case OC_STACK_NO_OBSERVERS:
		THINGS_LOG_V(ES_RH_TAG, "There is no exist registered Observer client.");
		break;
	default:
		THINGS_LOG_V(ES_RH_TAG, "Send notification Error : %d", ret);
		break;
	}

	ret = OCNotifyAllObservers(g_cloud_resource.handle, OC_HIGH_QOS);

	switch (ret) {
	case OC_STACK_OK:
		THINGS_LOG_V(ES_RH_TAG, "Sent notification to Cloud Resource Observers");
		break;
	case OC_STACK_NO_OBSERVERS:
		THINGS_LOG_V(ES_RH_TAG, "There is no exist registered Observer client.");
		break;
	default:
		THINGS_LOG_V(ES_RH_TAG, "Send notification Error : %d", ret);
		break;
	}

	return ret;
}

things_es_enrollee_state_e get_enrollee_state(void)
{
	things_es_enrollee_state_e Status = ES_STATE_INIT;

	pthread_mutex_lock(&g_status_mutex);
	Status = g_prov_resource.status;
	pthread_mutex_unlock(&g_status_mutex);

	return Status;
}

OCStackResult set_device_property(es_device_property *device_property)
{
	THINGS_LOG_D(ES_RH_TAG, "set_device_property IN");

	g_wifi_resource.supported_freq = (device_property->WiFi).freq;
	THINGS_LOG_D(ES_RH_TAG, "WiFi Freq : %d", g_wifi_resource.supported_freq);

	int mode_idx = 0;
	while ((device_property->WiFi).mode[mode_idx] != WiFi_EOF) {
		g_wifi_resource.supported_mode[mode_idx] = (device_property->WiFi).mode[mode_idx];
		THINGS_LOG_D(ES_RH_TAG, "WiFi Mode : %d", g_wifi_resource.supported_mode[mode_idx]);
		mode_idx++;
	}
	g_wifi_resource.num_mode = mode_idx;

	things_strncpy(g_dev_conf_resource.devName, (device_property->dev_conf_s).device_name, MAX_DEVICELEN);
	THINGS_LOG_D(ES_RH_TAG, "Device Name : %s", g_dev_conf_resource.devName);

	things_strncpy(g_dev_conf_resource.device_type, (device_property->dev_conf_s).device_type, THINGS_STRING_MAX_VALUE);
	THINGS_LOG_D(ES_RH_TAG, "Device device_type : %s", g_dev_conf_resource.device_type);

#ifdef CONFIG_ST_THINGS_SUPPORT_SUB_DEVICE
	things_strncpy(g_dev_conf_resource.device_sub_type, (device_property->dev_conf_s).device_sub_type, THINGS_STRING_MAX_VALUE);
	THINGS_LOG_D(ES_RH_TAG, "Sub Device device_type : %s", g_dev_conf_resource.device_sub_type);
#endif

	THINGS_LOG_D(ES_RH_TAG, "set_device_property OUT");
	return OC_STACK_OK;
}

OCStackResult set_enrollee_state(things_es_enrollee_state_e es_state)
{
	THINGS_LOG_D(ES_RH_TAG, "set_enrollee_state IN");

	pthread_mutex_lock(&g_status_mutex);
	OCStackResult result = OC_STACK_ERROR;
	things_es_enrollee_state_e preState = g_prov_resource.status;

	if (g_prov_resource.last_err_code == ER_ERRCODE_SYSTEM_ERROR) {
		g_prov_resource.status = ES_STATE_INIT;
		THINGS_LOG_V(ES_RH_TAG, "Please Check your System.(Status=%s , ErrCode=%s)", get_prov_status(g_prov_resource.status), get_prov_errcode(g_prov_resource.last_err_code));
		goto GOTO_OUT;
	}

	switch (es_state) {
	case ES_STATE_REGISTERING_TO_CLOUD:
		if (preState != ES_STATE_CONNECTED_TO_ENROLLER && preState != es_state) {
			THINGS_LOG_V(ES_RH_TAG, "State(%s) is not allowed by Status Policy.(pre-status: %s)", get_prov_status(es_state), get_prov_status(g_prov_resource.status));
			goto GOTO_OUT;
		}
		break;
	case ES_STATE_REGISTERED_TO_CLOUD:
		if (preState != ES_STATE_REGISTERING_TO_CLOUD && preState != es_state) {
			THINGS_LOG_V(ES_RH_TAG, "State(%s) is not allowed by Status Policy.(pre-status: %s)", get_prov_status(es_state), get_prov_status(g_prov_resource.status));
			goto GOTO_OUT;
		}
		break;
	case ES_STATE_PUBLISHING_RESOURCES_TO_CLOUD:
		if (preState != ES_STATE_REGISTERED_TO_CLOUD && preState != ES_STATE_PUBLISHED_RESOURCES_TO_CLOUD && preState != es_state) {
			THINGS_LOG_V(ES_RH_TAG, "State(%s) is not allowed by Status Policy.(pre-status: %s)", get_prov_status(es_state), get_prov_status(g_prov_resource.status));
			goto GOTO_OUT;
		}
		break;
	case ES_STATE_PUBLISHED_RESOURCES_TO_CLOUD:
		if (preState != ES_STATE_PUBLISHING_RESOURCES_TO_CLOUD && preState != es_state) {
			THINGS_LOG_V(ES_RH_TAG, "State(%s) is not allowed by Status Policy.(pre-status: %s)", get_prov_status(es_state), get_prov_status(g_prov_resource.status));
			goto GOTO_OUT;
		}
		break;
	}

	if (g_prov_resource.status == es_state) {
		result = OC_STACK_DUPLICATE_UUID;
	} else {
		g_prov_resource.status = es_state;
		result = OC_STACK_OK;
	}

	THINGS_LOG_V(ES_RH_TAG, "Enrollee Status : %s (%d)", get_prov_status(g_prov_resource.status), g_prov_resource.status);

GOTO_OUT:
	THINGS_LOG_D(ES_RH_TAG, "set_enrollee_state OUT");
	pthread_mutex_unlock(&g_status_mutex);
	return result;
}

OCStackResult set_cloud_err_code(ci_error_code_e es_err_code)
{
	THINGS_LOG_D(ES_RH_TAG, "set_cloud_err_code IN");

	g_prov_resource.vd_err_code = es_err_code;
	THINGS_LOG_V(ES_RH_TAG, "Cloud ErrorCode : %d", g_prov_resource.vd_err_code);

	THINGS_LOG_D(ES_RH_TAG, "set_cloud_err_code OUT");
	return OC_STACK_OK;
}

OCStackResult set_enrollee_err_code(es_error_code_e es_err_code)
{
	THINGS_LOG_D(ES_RH_TAG, "set_enrollee_err_code IN");

	if (g_prov_resource.last_err_code == ER_ERRCODE_SYSTEM_ERROR) {
		es_set_state(ES_STATE_INIT);
		THINGS_LOG_V(ES_RH_TAG, "Please Check your System.(Status=%s , ErrCode=%s)", get_prov_status(get_enrollee_state()), get_prov_errcode(g_prov_resource.last_err_code));
		return OC_STACK_ERROR;
	}

	g_prov_resource.last_err_code = es_err_code;
	THINGS_LOG_V(ES_RH_TAG, "Enrollee ErrorCode : %s(%d)", get_prov_errcode(g_prov_resource.last_err_code), g_prov_resource.last_err_code);

	THINGS_LOG_D(ES_RH_TAG, "set_enrollee_err_code OUT");
	return OC_STACK_OK;
}

const char *get_result(OCStackResult result)
{
	switch (result) {
	case OC_STACK_OK:
		return "OC_STACK_OK";
	case OC_STACK_INVALID_URI:
		return "OC_STACK_INVALID_URI";
	case OC_STACK_INVALID_QUERY:
		return "OC_STACK_INVALID_QUERY";
	case OC_STACK_INVALID_IP:
		return "OC_STACK_INVALID_IP";
	case OC_STACK_RESOURCE_CHANGED:
		return "OC_STACK_RESOURCE_CHANGED";
	case OC_STACK_INVALID_PORT:
		return "OC_STACK_INVALID_PORT";
	case OC_STACK_INVALID_CALLBACK:
		return "OC_STACK_INVALID_CALLBACK";
	case OC_STACK_INVALID_METHOD:
		return "OC_STACK_INVALID_METHOD";
	case OC_STACK_NO_MEMORY:
		return "OC_STACK_NO_MEMORY";
	case OC_STACK_COMM_ERROR:
		return "OC_STACK_COMM_ERROR";
	case OC_STACK_INVALID_PARAM:
		return "OC_STACK_INVALID_PARAM";
	case OC_STACK_NOTIMPL:
		return "OC_STACK_NOTIMPL";
	case OC_STACK_NO_RESOURCE:
		return "OC_STACK_NO_RESOURCE";
	case OC_STACK_RESOURCE_ERROR:
		return "OC_STACK_RESOURCE_ERROR";
	case OC_STACK_SLOW_RESOURCE:
		return "OC_STACK_SLOW_RESOURCE";
	case OC_STACK_NO_OBSERVERS:
		return "OC_STACK_NO_OBSERVERS";
	case OC_STACK_ERROR:
		return "OC_STACK_ERROR";
	case OC_STACK_UNAUTHORIZED_REQ:
		return "OC_STACK_UNAUTHORIZED_REQ";
	default:
		THINGS_LOG_D(ES_RH_TAG, "Not Supported result value.(%d)", result);
		return "Not defined OCResult.";
	}
}

const char *get_prov_status(things_es_enrollee_state_e status)
{
	switch (status) {
	case ES_STATE_INIT:
		return "ES_STATE_INIT";
	case ES_STATE_CONNECTING_TO_ENROLLER:
		return "ES_STATE_CONNECTING_TO_ENROLLER";
	case ES_STATE_CONNECTED_TO_ENROLLER:
		return "ES_STATE_CONNECTED_TO_ENROLLER";
	case ES_STATE_FAILED_TO_CONNECT_TO_ENROLLER:
		return "ES_STATE_FAILED_TO_CONNECT_TO_ENROLLER";
	case ES_STATE_REGISTERING_TO_CLOUD:
		return "ES_STATE_REGISTERING_TO_CLOUD";
	case ES_STATE_REGISTERED_TO_CLOUD:
		return "ES_STATE_REGISTERED_TO_CLOUD";
	case ES_STATE_FAILED_TO_REGISTER_TO_CLOUD:
		return "ES_STATE_FAILED_TO_REGISTER_TO_CLOUD";
	case ES_STATE_PUBLISHING_RESOURCES_TO_CLOUD:
		return "ES_STATE_PUBLISHING_RESOURCES_TO_CLOUD";
	case ES_STATE_PUBLISHED_RESOURCES_TO_CLOUD:
		return "ES_STATE_PUBLISHED_RESOURCES_TO_CLOUD";
	case ES_STATE_FAILED_TO_PUBLISH_RESOURCES_TO_CLOUD:
		return "ES_STATE_FAILED_TO_PUBLISH_RESOURCES_TO_CLOUD";
	case ES_STATE_LOGOUT_FROM_CLOUD:
		return "ES_STATE_LOGOUT_FROM_CLOUD";
	default:
		THINGS_LOG_D(ES_RH_TAG, "Not Supported State value.(%d)", status);
		return "Not defined State";
	}
}

const char *get_prov_errcode(es_error_code_e lastErrcode)
{
	switch (lastErrcode) {
	case ES_ERRCODE_NO_ERROR:
		return "ES_ERRCODE_NO_ERROR";
	case ES_ERRCODE_SSID_NOT_FOUND:
		return "ES_ERRCODE_SSID_NOT_FOUND";
	case ES_ERRCODE_PW_WRONG:
		return "ES_ERRCODE_PW_WRONG";
	case ES_ERRCODE_IP_NOT_ALLOCATED:
		return "ES_ERRCODE_IP_NOT_ALLOCATED";
	case ES_ERRCODE_NO_INTERNETCONNECTION:
		return "ES_ERRCODE_NO_INTERNETCONNECTION";
	case ES_ERRCODE_TIMEOUT:
		return "ES_ERRCODE_TIMEOUT";
	case ES_ERRCODE_FAILED_TO_ACCESS_CLOUD_SERVER:
		return "ES_ERRCODE_FAILED_TO_ACCESS_CLOUD_SERVER";
	case ES_ERRCODE_NO_RESPONSE_FROM_CLOUD_SERVER:
		return "ES_ERRCODE_NO_RESPONSE_FROM_CLOUD_SERVER";
	case ES_ERRCODE_INVALID_AUTHCODE:
		return "ES_ERRCODE_INVALID_AUTHCODE";
	case ES_ERRCODE_INVALID_ACCESSTOKEN:
		return "ES_ERRCODE_INVALID_ACCESSTOKEN";
	case ES_ERRCODE_FAILED_TO_REFRESH_ACCESSTOKEN:
		return "ES_ERRCODE_FAILED_TO_REFRESH_ACCESSTOKEN";
	case ES_ERRCODE_FAILED_TO_FIND_REGISTERED_DEVICE_IN_CLOUD:
		return "ES_ERRCODE_FAILED_TO_FIND_REGISTERED_DEVICE_IN_CLOUD";
	case ES_ERRCODE_FAILED_TO_FIND_REGISTERED_USER_IN_CLOUD:
		return "ES_ERRCODE_FAILED_TO_FIND_REGISTERED_USER_IN_CLOUD";
	case ER_ERRCODE_INVALID_PROV_PAYLOAD:
		return "ER_ERRCODE_INVALID_PROV_PAYLOAD";
	case ER_ERRCODE_SYSTEM_ERROR:
		return "ER_ERRCODE_SYSTEM_ERROR";
	case ES_ERRCODE_UNKNOWN:
		return "ES_ERRCODE_VENDOR_ERROR";
	default:
		THINGS_LOG_D(ES_RH_TAG, "Not Supported Error value.(%d)", lastErrcode);
		return "Not Defined last_err_code";
	}
}

/*********************************************************************
 *
 * Support CI server format.
 *
 * - IP:Port
 * - coaps+tcp://IP
 * - coaps+tcp://IP:Port
 * - coaps+tcp://HostName
 * - coaps+tcp://HostName:Port
 *
 *********************************************************************/
static int check_ci_server_ipv4(char *ci_server, es_cloud_prov_data_s *cloudData)
{
	int length = 0;
	char *pContext = NULL;
	char *originCIServer = ci_server;
	char *port = NULL;
	struct in_addr ip;
	ip.s_addr = 0;

	if (ci_server == NULL || cloudData == NULL) {
		THINGS_LOG_D(ES_RH_TAG, "ci_server(0x%X)/cloudData(0x%X) is NULL", ci_server, cloudData);
		return 0;
	}

	if (strlen(ci_server) == 0) {
		THINGS_LOG_D(ES_RH_TAG, "ci_server's length is 0");
		return 0;
	}

	if (strstr(ci_server, UNSUPPORT_COAP_TCP_HOST) != NULL) {
		THINGS_LOG_V(ES_RH_TAG, "%s is not support Type", UNSUPPORT_COAP_TCP_HOST);
		return -1;
	}
	// Port Number analysis.
	if ((pContext = strstr(ci_server, DEFAULT_COAP_TCP_HOST)) != NULL) {
		THINGS_LOG_D(ES_RH_TAG, "%s type format", DEFAULT_COAP_TCP_HOST);

		ci_server = pContext + strlen(DEFAULT_COAP_TCP_HOST);
		port = DEFAULT_COAP_PORT;
	}

	THINGS_LOG_D(ES_RH_TAG, "[Port] ci_server = %s, port=%s", ci_server, port);

	if ((pContext = strchr(ci_server, ':')) != NULL) {
		if (*(pContext + 1) == 0 || strlen(pContext + 1) > 5) {
			THINGS_LOG_V(ES_RH_TAG, "Port Num is out of range.(%s)", pContext + 1);
			return -1;
		}

		if (atoi(pContext + 1) == 0) {
			THINGS_LOG_V(ES_RH_TAG, "Invalid Port Num.(Port is not number : %s)", pContext + 1);
			return -1;
		}

		port = pContext + 1;
		*pContext = 0;
	}

	if (port == NULL || (length = strlen(ci_server)) == 0) {
		THINGS_LOG_V(ES_RH_TAG, "Unsupported CI-server format.(port-num is NULL)", originCIServer);
		return -1;
	}
	// ci_server analysis (IP/HostName)
	THINGS_LOG_D(ES_RH_TAG, "[IP] ci_server = %s", ci_server);

	if (inet_aton(ci_server, &ip) == 0 || ip.s_addr == 0) {
		THINGS_LOG_D(ES_RH_TAG, "ci_server is Host Name.(%s)", ci_server);
		things_strncpy(cloudData->host_name, ci_server, sizeof(cloudData->host_name));
		memset(cloudData->ip, 0, IP_PORT);
		things_strncpy(cloudData->port, port, sizeof(cloudData->port));
	} else {
		THINGS_LOG_D(ES_RH_TAG, "ci_server is ip address.(%s)", ci_server);
		things_strncpy(cloudData->ip, ci_server, sizeof(cloudData->ip));
		memset(cloudData->host_name, 0, THINGS_STRING_MAX_VALUE);
		things_strncpy(cloudData->port, port, sizeof(cloudData->port));
	}

	THINGS_LOG_D(ES_RH_TAG, "Host=%s, IP=%s, Port=%s", cloudData->host_name, cloudData->ip, cloudData->port);

	things_strncpy(cloudData->ci_server, ci_server, sizeof(cloudData->ci_server));
	cloudData->ci_server[length] = ':';
	cloudData->ci_server[length + 1] = 0;
	if (things_strcat(cloudData->ci_server, THINGS_STRING_MAX_VALUE, (const char *)cloudData->port) == NULL) {
		THINGS_LOG_D(ES_RH_TAG, "things_strcat is failed.");
		return -1;
	}

	THINGS_LOG_D(ES_RH_TAG, "cloudData->ci_server = %s", cloudData->ci_server);

	return 1;
}
