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
#ifndef __OCF_MYLIGHT_H__
#define __OCF_MYLIGHT_H__

#include <iotivity_config.h>
#include <platform_features.h>
#include <ocstack.h>
#include <logger.h>
#include <ocpayload.h>

#define SVR_DB_PATH "/mnt/oic_svr_db.dat"
#define SVR_JSON_PATH "/mnt/oic_svr_db.json"
#define DEVICE_DB_PATH "/mnt/device_properties.dat"
#define INTROSPECTION_DB_PATH "/mnt/introspection.dat"
#define INTROSPECTION_JSON_PATH "/mnt/introspection.json"
#define SETUP_INFO_PATH "/mnt/setup_info.dat"

#define _UNUSED_ __attribute__((unused))

#define DBG(fmt, args...) fprintf(stdout, "\e[1;32m<%s:%d> " fmt "\e[0m\n", \
		__FILE__, __LINE__, ##args)
#define MSG(fmt, args...) fprintf(stdout, "\e[1;34m" fmt "\e[0m\n", ##args)

struct ocf_ops {
	OCEntityHandler get;
	OCEntityHandler put;
	OCEntityHandler post;
	OCEntityHandler del;
	OCEntityHandler register_observe;
	OCEntityHandler deregister_observe;
};

struct ocf_dev_ops {
	OCDeviceEntityHandler get;
	OCDeviceEntityHandler put;
	OCDeviceEntityHandler post;
	OCDeviceEntityHandler del;
};

OCEntityHandlerResult ocf_mylight_handler(OCEntityHandlerFlag flag,
		OCEntityHandlerRequest *req, void *user_data);
OCEntityHandlerResult ocf_mylight_dev_handler(OCEntityHandlerFlag flag,
		OCEntityHandlerRequest *req, char *uri, void *user_data);

void ocf_mylight_verbose_payload(const char *indent, OCPayload *payload);
void ocf_mylight_verbose_response(OCEntityHandlerResponse *resp);
void ocf_mylight_verbose_request(OCEntityHandlerFlag flag,
		OCEntityHandlerRequest *req);

int ocf_mylight_get_quit_flag(void);
void ocf_mylight_set_quit_flag(int with_reboot);

int ocf_mylight_playform_init(void);

const char *ocf_mylight_device_get_name(void);
int ocf_mylight_device_set_name(const char *name);
int ocf_mylight_device_init(void);

int ocf_mylight_light_init(void);
void ocf_mylight_light_exit(void);
int ocf_mylight_light_get_handle(OCResourceHandle *handle);
const char *ocf_mylight_light_peek_uri(void);
int ocf_mylight_light_set_status(bool status);
int ocf_mylight_light_get_status(bool *status);

#ifdef CONFIG_EXAMPLES_OCFMYLIGHT_DIMMER
int ocf_mylight_dimm_init(void);
void ocf_mylight_dimm_exit(void);
int ocf_mylight_dimm_get_handle(OCResourceHandle *handle);
const char *ocf_mylight_dimm_peek_uri(void);
int ocf_mylight_dimm_set_level(int64_t value);
int ocf_mylight_dimm_get_level(int64_t *value);
#endif

int ocf_mylight_security_init(void);
void ocf_mylight_security_free(void);

int ocf_mylight_notify_init(void);
void ocf_mylight_notify_exit(void);
int ocf_mylight_notify_add(unsigned int rsrc_id, OCObservationId obs_id);
int ocf_mylight_notify_del(unsigned int rsrc_id, OCObservationId obs_id);
int ocf_mylight_notify_emit(unsigned int rsrc_id);

int ocf_mylight_userinput_init(void);
void ocf_mylight_remove_db(void);
void ocf_mylight_recovery_default(void);

int ocf_mylight_reset_svr(void);
int ocf_mylight_reset_introspection(void);

const char *ocf_mylight_setup_get_name(void);
const char *ocf_mylight_setup_get_wifi_ssid(void);
const char *ocf_mylight_setup_get_wifi_pwd(void);
int ocf_mylight_setup_get_wifi_security(void);
const OCUUIdentity *ocf_mylight_setup_get_uuid(void);
int ocf_mylight_setup_set_name(const char *name, size_t len);
int ocf_mylight_setup_set_wifi_ssid(const char *ssid, size_t len);
int ocf_mylight_setup_set_wifi_pwd(const char *pwd, size_t len);
int ocf_mylight_setup_set_wifi_security(int security);
int ocf_mylight_setup_set_failback_time(time_t value);
time_t ocf_mylight_setup_get_failback_time(void);
const char *ocf_mylight_setup_get_timezone(void);
int ocf_mylight_setup_set_timezone(const char *zone, size_t len);
void ocf_mylight_setup_reset(void);
int ocf_mylight_setup_load(void);
int ocf_mylight_setup_save(void);

int ocf_mylight_wifi_init(void);
int ocf_mylight_wifi_scan(void);
int ocf_mylight_wifi_connect(void);

int ocf_mylight_ntp_init(void);
#ifdef CONFIG_LIBC_LOCALTIME
int ocf_mylight_tz_setup(const char *tzname, unsigned char *data,
		unsigned int len);
int ocf_mylight_tz_select(const char *tzname);
#endif

/**
 * Prototype of json2cbor.c function
 */
void ConvertJSONFileToCBORFile(const char *jsonFileName,
		const char *cborFileName);

#endif
