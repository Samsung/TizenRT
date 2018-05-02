/* ****************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#include "octypes.h"
#include "easysetup.h"
#include "logging/things_logger.h"
#include "resource_handler.h"
#include "oic_string.h"
#include "cloud/cloud_manager.h"
#include "things_api.h"

#ifdef CONFIG_ST_THINGS_FOTA
#include "fota/fmwup_api.h"
#endif

#define ES_ENROLLEE_TAG "[easysetup]"

static bool g_is_secured = false;

static es_provisioning_callbacks_s g_es_provisioning_cb;
static es_device_property g_es_device_property;
static things_get_easysetup_state_func_type g_notify_easysetup_state = NULL;

void es_register_notify_easysetup_state(void *func)
{
	g_notify_easysetup_state = (things_get_easysetup_state_func_type) func;
}

void ESWiFiRsrcCallback(es_result_e es_result, es_wifi_prov_data_s *event_data)
{
	THINGS_LOG_D(ES_ENROLLEE_TAG, "ESWiFiRsrcCallback IN");

	if (es_result != ES_OK) {
		THINGS_LOG_E(ES_ENROLLEE_TAG, "ESWiFiRsrcCallback ES_ERROR Occured");
		return;
	}
	// deliver data to es_provisioning_callbacks_s
	if (g_es_provisioning_cb.wifi_prov_cb != NULL) {
		g_es_provisioning_cb.wifi_prov_cb(event_data);
	} else {
		THINGS_LOG_E(ES_ENROLLEE_TAG, "wifi_prov_cb is NULL");
		return;
	}
}

void ESCloudRsrcCallback(es_result_e es_resoult, es_cloud_prov_data_s *event_data)
{
	THINGS_LOG_D(ES_ENROLLEE_TAG, "ESCloudRsrcCallback IN");

	if (es_resoult != ES_OK) {
		THINGS_LOG_E(ES_ENROLLEE_TAG, "ESCloudRsrcCallback ES_ERROR Occured");
		return;
	}

	if (g_es_provisioning_cb.cloud_data_prov_cb != NULL) {
		g_es_provisioning_cb.cloud_data_prov_cb(event_data);
	} else {
		THINGS_LOG_E(ES_ENROLLEE_TAG, "cloud_data_prov_cb is NULL");
		return;
	}
}

void ESDevconfRsrcallback(es_result_e es_resoult, es_dev_conf_prov_data_s *event_data)
{
	THINGS_LOG_D(ES_ENROLLEE_TAG, "ESDevconfRsrcallback IN");

	if (es_resoult != ES_OK) {
		THINGS_LOG_E(ES_ENROLLEE_TAG, "ESDevconfRsrcallback ES_ERROR Occured");
		return;
	}

	if (g_es_provisioning_cb.dev_conf_prov_cb != NULL) {
		g_es_provisioning_cb.dev_conf_prov_cb(event_data);
	} else {
		THINGS_LOG_E(ES_ENROLLEE_TAG, "dev_conf_prov_cb is NULL");
		return;
	}

	THINGS_LOG_D(ES_ENROLLEE_TAG, "ESDevconfRsrcallback OUT");
}

es_result_e es_init_enrollee(bool is_secured, es_resource_mask_e resource_mask, es_provisioning_callbacks_s callbacks)
{
	THINGS_LOG_D(ES_ENROLLEE_TAG, "es_init_enrollee IN");

	g_is_secured = is_secured;
	g_es_provisioning_cb.wifi_prov_cb = NULL;
	g_es_provisioning_cb.dev_conf_prov_cb = NULL;
	g_es_provisioning_cb.cloud_data_prov_cb = NULL;

	if ((resource_mask & ES_WIFI_RESOURCE) == ES_WIFI_RESOURCE) {
		if (callbacks.wifi_prov_cb != NULL) {
			g_es_provisioning_cb.wifi_prov_cb = callbacks.wifi_prov_cb;
			register_wifi_rsrc_event_callback(ESWiFiRsrcCallback);
		} else {
			THINGS_LOG_E(ES_ENROLLEE_TAG, "wifi_prov_cb NULL");
			return ES_ERROR;
		}
	}
	if ((resource_mask & ES_DEVCONF_RESOURCE) == ES_DEVCONF_RESOURCE) {
		if (callbacks.dev_conf_prov_cb != NULL) {
			g_es_provisioning_cb.dev_conf_prov_cb = callbacks.dev_conf_prov_cb;
			register_dev_conf_rsrc_event_callback(ESDevconfRsrcallback);
		} else {
			THINGS_LOG_E(ES_ENROLLEE_TAG, "dev_conf_prov_cb NULL");
			return ES_ERROR;
		}
	}
	if ((resource_mask & ES_CLOUD_RESOURCE) == ES_CLOUD_RESOURCE) {
		if (callbacks.cloud_data_prov_cb != NULL) {
			g_es_provisioning_cb.cloud_data_prov_cb = callbacks.cloud_data_prov_cb;
			register_cloud_rsrc_event_callback(ESCloudRsrcCallback);
		} else {
			THINGS_LOG_E(ES_ENROLLEE_TAG, "cloud_data_prov_cb NULL");
			return ES_ERROR;
		}
	}

	if (create_easysetup_resources(g_is_secured, resource_mask) != OC_STACK_OK) {
		unregister_resource_event_callback();

		if (delete_easysetup_resources() != OC_STACK_OK) {
			THINGS_LOG_E(ES_ENROLLEE_TAG, "Deleting prov resource ES_ERROR!!");
		}

		return ES_ERROR;
	}

	THINGS_LOG_D(ES_ENROLLEE_TAG, "es_init_enrollee OUT");
	return ES_OK;
}

things_es_enrollee_state_e es_get_state(void)
{
	return get_enrollee_state();
}

bool es_get_cloud_login_state(void)
{
	switch (get_enrollee_state()) {
	case ES_STATE_REGISTERED_TO_CLOUD:
	case ES_STATE_PUBLISHING_RESOURCES_TO_CLOUD:
	case ES_STATE_PUBLISHED_RESOURCES_TO_CLOUD:
	case ES_STATE_FAILED_TO_PUBLISH_RESOURCES_TO_CLOUD:
		return true;
	}

	return false;
}

es_result_e es_set_device_property(es_device_property *device_property)
{
	THINGS_LOG_D(ES_ENROLLEE_TAG, "es_set_device_property IN");

	if (set_device_property(device_property) != OC_STACK_OK) {
		THINGS_LOG_E(ES_ENROLLEE_TAG, "es_set_device_property ES_ERROR");
		return ES_ERROR;
	}

	int mode_idx = 0;
	while ((device_property->WiFi).mode[mode_idx] != WiFi_EOF) {
		(g_es_device_property.WiFi).mode[mode_idx] = (device_property->WiFi).mode[mode_idx];
		THINGS_LOG_D(ES_ENROLLEE_TAG, "WiFi Mode : %d", (g_es_device_property.WiFi).mode[mode_idx]);
		mode_idx++;
	}
	(g_es_device_property.WiFi).freq = (device_property->WiFi).freq;
	THINGS_LOG_D(ES_ENROLLEE_TAG, "WiFi Freq : %d", (g_es_device_property.WiFi).freq);

	things_strncpy((g_es_device_property.dev_conf_s).device_name, (device_property->dev_conf_s).device_name, MAX_DEVICELEN);
	THINGS_LOG_D(ES_ENROLLEE_TAG, "Device Name : %s", (g_es_device_property.dev_conf_s).device_name);

	things_strncpy((g_es_device_property.dev_conf_s).device_type, (device_property->dev_conf_s).device_type, MAX_DEVICELEN);
	THINGS_LOG_D(ES_ENROLLEE_TAG, "Device Type : %s", (g_es_device_property.dev_conf_s).device_type);

	THINGS_LOG_D(ES_ENROLLEE_TAG, "es_set_device_property OUT");
	return ES_OK;
}

es_result_e es_set_state(things_es_enrollee_state_e es_state)
{
	//THINGS_LOG_D(ES_ENROLLEE_TAG, "es_set_state IN");
	OCStackResult res = OC_STACK_ERROR;

	if (es_state >= ES_STATE_EOF) {
		THINGS_LOG_E(ES_ENROLLEE_TAG, "Invalid things_es_enrollee_state_e : %d", es_state);
		return ES_ERROR;
	}

	if (es_cloud_session_stop_trigger(es_state) == true) {
		THINGS_LOG_E(ES_ENROLLEE_TAG, "Cloud Connection Log-Out ing...");
		return ES_ERROR;
	}

	if ((res = set_enrollee_state(es_state)) == OC_STACK_ERROR) {
		THINGS_LOG_E(ES_ENROLLEE_TAG, "es_set_state setting is failed.");
		return ES_ERROR;
	}

	if (g_notify_easysetup_state != NULL && res == OC_STACK_OK) {
		THINGS_LOG_D(ES_ENROLLEE_TAG, "Notify Easy-Setup State to THINGS_APP. %d", es_state);
#ifdef CONFIG_ST_THINGS_FOTA
		if (es_state == ES_STATE_PUBLISHED_RESOURCES_TO_CLOUD) {
			int ret = fmwup_check_firmware_upgraded();
			if (ret != 0) {
				THINGS_LOG_D("fmwup_check_firmware_upgraded : [%d]", ret);
			}
		}
#endif
		g_notify_easysetup_state(es_state);
	}

	THINGS_LOG_D(ES_ENROLLEE_TAG, "Set ESState succesfully : %d", es_state);
	//THINGS_LOG_D(ES_ENROLLEE_TAG, "es_set_state OUT");
	return ES_OK;
}

static es_error_code_e ci_errcode_to_es_errcode(ci_error_code_e cm_err_code)
{
	es_error_code_e es_err_code = ES_ERRCODE_NO_ERROR;

	switch (cm_err_code) {
	case ERRCI_NO_ERROR:
	case ERRCI_SUCCESS:
		es_err_code = ES_ERRCODE_NO_ERROR;
		break;
	case ERRCI_SAMSUNG_ACCOUNT_AUTHORIZATION_FAILED:
	case ERRCI_SAMSUNG_ACCOUNT_UNAUTHORIZED_TOKEN:
		es_err_code = ES_ERRCODE_INVALID_AUTHCODE;
		break;
	case ERRCI_TOKEN_VALIDATION_FAILED:
		es_err_code = ES_ERRCODE_INVALID_ACCESSTOKEN;
		break;
	case ERRCI_DEVICE_NOT_FOUND:
		es_err_code = ES_ERRCODE_FAILED_TO_FIND_REGISTERED_DEVICE_IN_CLOUD;
		break;
	case ERRCI_USER_NOT_FOUND:
		es_err_code = ES_ERRCODE_FAILED_TO_FIND_REGISTERED_USER_IN_CLOUD;
		break;
	default:
		es_err_code = ES_ERRCODE_UNKNOWN;
		break;
	}

	return es_err_code;
}

es_result_e es_set_error_code(es_error_code_e es_err_code)
{
	THINGS_LOG_D(ES_ENROLLEE_TAG, "es_set_error_code IN");

	if (es_err_code > ES_ERRCODE_UNKNOWN) {
		THINGS_LOG_E(ES_ENROLLEE_TAG, "Invalid es_set_error_code : %d", es_err_code);
		return ES_ERROR;
	}

	if (set_enrollee_err_code(es_err_code) != OC_STACK_OK) {
		THINGS_LOG_E(ES_ENROLLEE_TAG, "es_set_error_code ES_ERROR");
		return ES_ERROR;
	}

	THINGS_LOG_D(ES_ENROLLEE_TAG, "Set es_error_code_e successfully : %d", es_err_code);
	return ES_OK;
}

es_result_e es_set_cloud_error_code(ci_error_code_e es_err_code)
{
	THINGS_LOG_D(ES_ENROLLEE_TAG, "es_set_cloud_error_code IN");

	es_error_code_e esError = ES_ERRCODE_NO_ERROR;

	if (es_err_code > ERRCI_UNKNOWN) {
		THINGS_LOG_E(ES_ENROLLEE_TAG, "Invalid es_set_cloud_error_code : %d", es_err_code);
		return ES_ERROR;
	}

	esError = ci_errcode_to_es_errcode(es_err_code);

	if (set_cloud_err_code(es_err_code) != OC_STACK_OK) {
		THINGS_LOG_E(ES_ENROLLEE_TAG, "es_set_cloud_error_code ES_ERROR");
		return ES_ERROR;
	}

	THINGS_LOG_D(ES_ENROLLEE_TAG, "Set es_error_code_e successfully : %d", es_err_code);

	es_set_error_code(esError);

	THINGS_LOG_D(ES_ENROLLEE_TAG, "es_set_cloud_error_code OUT");
	return ES_OK;
}

es_result_e es_terminate_enrollee()
{
	THINGS_LOG_D(ES_ENROLLEE_TAG, "es_terminate_enrollee IN");

	unregister_resource_event_callback();

	//Delete Prov resource
	if (delete_easysetup_resources() != OC_STACK_OK) {
		THINGS_LOG_E(ES_ENROLLEE_TAG, "Deleting prov resource ES_ERROR!!");
		return ES_ERROR;
	}

	THINGS_LOG_D(ES_ENROLLEE_TAG, "es_terminate_enrollee success");
	return ES_OK;
}
