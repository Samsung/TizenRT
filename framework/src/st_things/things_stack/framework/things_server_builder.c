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
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "things_def.h"
#include "things_common.h"
#include "logging/things_logger.h"
#include "utils/things_string.h"
#include "utils/things_malloc.h"
#include "utils/things_string.h"
#include "ocpayload.h"
#include "things_resource.h"
#include "things_server_builder.h"


#include "easy-setup/resource_handler.h"
#ifdef __ST_THINGS_RTOS__
#include "utils/things_rtos_util.h"
#endif
#define TAG "[things_svrbldr]"

#define DEFAULT_DATA_MODEL_VERSIONS "res.1.1.0,sh.1.1.0"

#define KEY_ATTR_DEVICE_NAME                "n"
#define KEY_ATTR_PLATFORM_VENDERID          "vid"

static pthread_t g_thread_id_server;
static pthread_t g_thread_id_presence;

static int g_quit_flag = 0;
static uint g_presence_flag = 0;
static int g_presence_duration = 20;
static volatile bool is_presence = false;

things_server_builder_s *g_builder = NULL;

void *server_execute_loop(void *param)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	while (!g_quit_flag) {
		if (OCProcess() != OC_STACK_OK) {
			THINGS_LOG_E(TAG, "OCProcess Error");
			//   need to insert error handling logic from here
		}
		//   The proper time period for looping need to be decided..
		usleep(10 * 1000);
	}
	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);

	return NULL;
}

void register_req_handler(struct things_server_builder_s *builder, request_handler_cb handler)
{
	if (handler == NULL || builder == NULL) {
		THINGS_LOG_E(TAG, "Invalid Param");
		return;
	}
	builder->handler = handler;
}

struct things_resource_s *create_resource(struct things_server_builder_s *builder, char *uri, char *type, char *interface, int isDiscoverable, int isObserable, int isSecure)
{
	things_resource_s *res = NULL;
	OCResourceHandle hd = NULL;
	uint8_t rsc_properties = OC_DISCOVERABLE;

	if (builder->handler == NULL) {
		THINGS_LOG_E(TAG, "Handler for serverbuilder is not registered");
		return res;
	}

	if (1 != isDiscoverable) {
		rsc_properties = OC_ACTIVE;
	}

	if (1 == isObserable) {
		rsc_properties |= OC_OBSERVABLE;
	}

	if (1 == isSecure) {
#ifdef __SECURED__
		rsc_properties |= OC_SECURE;
#else
		THINGS_LOG_D(TAG, "Stack is in UNSECURED Mode");
#endif
	}

	OCStackResult ret = OCCreateResource(&hd,
										 type,
										 interface,
										 uri,
										 builder->handler,
										 NULL,
										 rsc_properties);

	if (ret != OC_STACK_OK) {
		THINGS_LOG_V(TAG, "Resource Creation Failed - ret = %d, %s", ret, uri);
		return NULL;
	}

	res = things_create_resource_inst(NULL, hd, NULL, NULL);

	if (NULL == res) {
		THINGS_LOG_E(TAG, "things_create_resource_inst is failed");
		return res;
	}

	res->res_type = things_strdup(type);

	builder->gres_arr[builder->res_num++] = res;

	THINGS_LOG_D(TAG, "Created hd [%x], prop [0x%X] uri : %s", res->resource_handle, rsc_properties, res->uri);
	THINGS_LOG_D(TAG, "DISCOVERABLE : %s", (isDiscoverable == 1 ? "YES" : "NO"));
	THINGS_LOG_D(TAG, "OBSERABLE : %s", (isObserable == 1 ? "YES" : "NO"));
	THINGS_LOG_D(TAG, "SECURE : %s", (isSecure == 1 ? "YES" : "NO"));

	return res;
}
#ifdef CONFIG_ST_THINGS_COLLECTION
struct things_resource_s *create_collection_resource(struct things_server_builder_s *builder, char *uri, char *type)
{
	things_resource_s *res = NULL;
	OCResourceHandle hd = NULL;
	uint8_t rsc_properties = OC_DISCOVERABLE | OC_OBSERVABLE;

	if (builder->handler == NULL) {
		THINGS_LOG_E(TAG, "Handler for serverbuilder is not registered");
		return res;
	}
#ifdef __SECURED__

	rsc_properties |= OC_SECURE;

#endif							//#ifdef __SECURED__

	OCStackResult ret = OCCreateResource(&hd,
										 type,
										 OIC_INTERFACE_LINKEDLIST,
										 uri,
										 builder->handler,
										 NULL,
										 rsc_properties);

	if (ret != OC_STACK_OK) {
		THINGS_LOG_V(TAG, "Resource Creation Failed - ret = %d, %s", ret, uri);
		return NULL;
	}
	
	OCBindResourceTypeToResource(hd, OIC_RTYPE_COLLECTION_WK);
	OCBindResourceInterfaceToResource(hd, OIC_INTERFACE_BATCH);

	res = things_create_resource_inst(NULL, hd, NULL, NULL);

	if (NULL == res) {
		THINGS_LOG_E(TAG, "things_create_resource_inst is failed");
		return res;
	}

	res->res_type = things_strdup(type);

	builder->gres_arr[builder->res_num++] = res;

	THINGS_LOG_V(TAG, "Created hd [%x], prop [0x%X] uri : %s", res->resource_handle, rsc_properties, uri);

	return res;
}
#endif

void delete_resource(struct things_server_builder_s *builder)
{
	for (size_t iter = 0; iter < builder->res_num; iter++) {
		OCStackResult ret = OCDeleteResource((OCResourceHandle)(builder->gres_arr[iter]->resource_handle));
		if (ret != OC_STACK_OK) {
			THINGS_LOG_E(TAG, "Failed to delete the resource");
		}
	}
}

int add_interface_type(things_resource_s *resource, char *interface)
{
	if (NULL != resource && NULL != interface) {
		if (NULL != resource->resource_handle) {
			if (OC_STACK_OK == OCBindResourceInterfaceToResource(resource->resource_handle, interface)) {
				return OC_STACK_OK;
			}
		}
	}

	return OC_STACK_ERROR;
}

int add_resource_type(things_resource_s *resource, char *type)
{
	if (NULL != resource && NULL != type) {
		if (NULL != resource->resource_handle) {
			if (OC_STACK_OK == OCBindResourceTypeToResource(resource->resource_handle, type)) {
				return OC_STACK_OK;
			}
		}
	}

	return OC_STACK_ERROR;
}

void things_bind(struct things_resource_s *res, struct things_resource_s *bind)
{
	if (res == NULL || bind == NULL) {
		THINGS_LOG_E(TAG, "Invalid Resource");
		return;
	}

	if (res->resource_handle == bind->resource_handle) {
		THINGS_LOG_D(TAG, "It's identical resource");
		return;
	}

	OCStackResult ret = OCBindResource((OCResourceHandle)(res->resource_handle),
									   (OCResourceHandle)(bind->resource_handle));
	if (ret != OC_STACK_OK) {
		THINGS_LOG_V(TAG, "bind Failed ");
	}
}

void bind_all(struct things_resource_s *res, struct things_resource_s *bind[], size_t size)
{
	for (size_t iter = 0; iter < size; iter++) {
		things_bind(res, bind[iter]);
		//    This time value achieved via testing
		//       May need to re-test to find the opt. value for give platform.
		usleep(100 * 1000);
	}
}

struct things_resource_s *get_resource(things_server_builder_s *builder, const char *uri)
{
	things_resource_s *ret = NULL;
	for (size_t iter = 0; iter < builder->res_num; iter++) {
		const char *rURI = OCGetResourceUri((OCResourceHandle)(builder->gres_arr[iter]->resource_handle));

		if (things_string_compare(rURI, uri) == 0) {
			THINGS_LOG_D(TAG, "URI Compare : %s , %s", uri, rURI);
			ret = builder->gres_arr[iter];
			break;
		}
	}
	return ret;
}

void init_builder(struct things_server_builder_s *builder, request_handler_cb cb)
{
	OCTransportAdapter m_transport = (OC_ADAPTER_IP | OC_ADAPTER_TCP);
	if (OC_STACK_OK != OCInit2(OC_CLIENT_SERVER, OC_IP_USE_V4, OC_IP_USE_V4, m_transport)) {
		THINGS_LOG_E(TAG, "RESOURCE SERVER START FAILED");
		return;
	}

	THINGS_LOG_V(TAG, "Resource Server IS NOW STARTING...");

	g_quit_flag = 0;

	pthread_create_rtos(&g_thread_id_server, NULL, server_execute_loop, (void *)NULL, THINGS_STACK_SERVEREXCETUE_LOOP_THREAD);

	register_req_handler(builder, cb);
}

void deinit_builder(things_server_builder_s *builder)
{
	if (builder != NULL) {
		g_quit_flag = 1;
		pthread_cancel(g_thread_id_server);
		pthread_join(g_thread_id_server, NULL);
		pthread_detach(g_thread_id_server);
		g_thread_id_server = NULL;

		pthread_cancel(g_thread_id_presence);
		pthread_join(g_thread_id_presence, NULL);
		pthread_detach(g_thread_id_presence);
		g_thread_id_presence = NULL;

		// 1.    Need to unregister those registered resource in the Stack
		// 2.    Free the payload of each resources
		for (size_t iter = 0; iter < builder->res_num; iter++) {
			if (builder->gres_arr[iter]->rep != NULL) {
				things_release_representation_inst(builder->gres_arr[iter]->rep);
			}
		}
	}

	if (OCStop() != OC_STACK_OK) {
		THINGS_LOG_E(TAG, "OCStack process error");
	}
}

void set_device_info(things_server_builder_s *builder, char *device_name, char *device_type)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	THINGS_LOG_D(TAG, "[/oic/d] name :%s", device_name);
	THINGS_LOG_D(TAG, "[/oic/d] type :%s", device_type);

	OCDeviceInfo device_info;
	device_info.deviceName = NULL;
	device_info.types = NULL;
	device_info.specVersion = NULL;
	device_info.dataModelVersions = NULL;

	things_string_duplicate(device_name, &device_info.deviceName);
	things_string_duplicate(OC_SPEC_VERSION, &device_info.specVersion);
	device_info.dataModelVersions = OCCreateOCStringLL(DEFAULT_DATA_MODEL_VERSIONS);
	device_info.types = OCCreateOCStringLL(device_type);
	OCResourcePayloadAddStringLL(&device_info.types, OC_RSRVD_RESOURCE_TYPE_DEVICE);
	OCSetDeviceInfo(device_info);

	// OCSetPropertyValue(PAYLOAD_TYPE_DEVICE, KEY_ATTR_DEVICE_NAME, device_name);

	things_free(device_info.deviceName);
	things_free(device_info.specVersion);
	OCFreeOCStringLL(device_info.dataModelVersions);
	OCFreeOCStringLL(device_info.types);

	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
}

void set_platform_info(things_server_builder_s *builder, char *model_num, char *pver, char *osver, char *hwver, char *fwver, char *venderid, char *mnmn, char *mn_url)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	if (model_num == NULL || strlen(model_num) < 1) {
		THINGS_LOG_E(TAG, "Invalid input for registering platform Info");
		return;
	}

	THINGS_LOG_D(TAG, "[/oic/p] Manufacturer :%s", mnmn);
	THINGS_LOG_D(TAG, "[/oic/p] Model Name :%s", model_num);
	THINGS_LOG_D(TAG, "[/oic/p] Ver. Plaform :%s", pver);
	THINGS_LOG_D(TAG, "[/oic/p] Ver. OS :%s", osver);
	THINGS_LOG_D(TAG, "[/oic/p] Ver. HW :%s", hwver);
	THINGS_LOG_D(TAG, "[/oic/p] Ver. FW :%s", fwver);
	THINGS_LOG_D(TAG, "[/oic/p] Ver. vid :%s", venderid);

	OCPlatformInfo platform_info;

	platform_info.platformID = (char *)OCGetServerInstanceIDString();
	platform_info.manufacturerName = mnmn;
	platform_info.modelNumber = model_num;
	platform_info.platformVersion = pver;
	platform_info.operatingSystemVersion = osver;
	platform_info.hardwareVersion = hwver;
	platform_info.firmwareVersion = fwver;

	platform_info.manufacturerUrl = mn_url;
	platform_info.dateOfManufacture = NULL;
	platform_info.supportUrl = NULL;
	platform_info.systemTime = NULL;

	OCSetPropertyValue(PAYLOAD_TYPE_PLATFORM, KEY_ATTR_PLATFORM_VENDERID, venderid);

	OCSetPlatformInfo(platform_info);

	THINGS_LOG_D(TAG, THINGS_FUNC_EXIT);
}

things_server_builder_s *get_builder_instance()
{
	if (g_builder == NULL) {
		g_builder = (things_server_builder_s *) things_malloc(sizeof(things_server_builder_s));
		if (g_builder != NULL) {
			g_builder->init_module = &init_builder;
			g_builder->deinit_module = &deinit_builder;
			g_builder->set_device_info = &set_device_info;
			g_builder->set_platform_info = &set_platform_info;
			g_builder->create_resource = &create_resource;
			// g_builder->CreateActiveResource = &CreateActiveResource;
#ifdef CONFIG_ST_THINGS_COLLECTION
			g_builder->create_collection_resource = &create_collection_resource;
#endif
			g_builder->get_resource = &get_resource;
			g_builder->delete_resource = &delete_resource;
			g_builder->add_interface_type = &add_interface_type;
			g_builder->add_resource_type = &add_resource_type;
			g_builder->bind = &things_bind;
			g_builder->bind_all = &bind_all;
			g_builder->broadcast_presence = NULL; //&BroadcastPresence;
			g_builder->res_num = 0;
			g_builder->handler = NULL;

			is_presence = false;

			return g_builder;
		} else {
			THINGS_LOG_E(TAG, "Not enough Memory for Builder Instance");
			return NULL;
		}
	} else {
		THINGS_LOG_D(TAG, "Builder Instance Already Created");
		return g_builder;
	}
}

void release_builder_instance(things_server_builder_s *builder)
{
	if (builder) {
		//    Thread should be terminated first..
		if (!g_quit_flag) {
			builder->deinit_module(builder);
		}

		if (builder->res_num > 0) {
			for (size_t iter = 0; iter < builder->res_num; iter++) {
				if (builder->gres_arr[iter] != NULL) {
					/*! Added by st_things for memory Leak fix*/
					release_resource_inst_impl(builder->gres_arr[iter]);
					builder->gres_arr[iter] = NULL;
				}
			}
		}

		things_free(builder);
		g_builder = NULL;
		is_presence = false;
	}
}
