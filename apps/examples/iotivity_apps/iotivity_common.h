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

#ifndef __IOTIVITY_COMMON__H
#define __IOTIVITY_COMMON__H
#include <stdio.h>
#include <stdlib.h>


#include <ocstack.h>
#include "logger.h"
#include "ocpayload.h"

#ifdef __TIZENRT__
#define SVR_DB_PATH "/mnt/oic_svr_db.dat"
#define SVR_JSON_PATH "/mnt/oic_svr_db.json"
#define DEVICE_DB_PATH "/mnt/device_properties.dat"
#define INTROSPECTION_DB_PATH "/mnt/introspection.dat"
#define INTROSPECTION_JSON_PATH "/mnt/introspection.json"
#define WIFI_INFO_PATH "/mnt/wifi_info.txt"
#else
#define SVR_DB_PATH "oic_svr_db.dat"
#define SVR_JSON_PATH "oic_svr_db.json"
#define DEVICE_DB_PATH "device_properties.dat"
#define INTROSPECTION_DB_PATH "introspection.dat"
#define INTROSPECTION_JSON_PATH "introspection.json"
#define WIFI_INFO_PATH "wifi_info.txt"
#endif

#define DBG(fmt, args...) fprintf(stdout, "\e[1;32m<%s:%d> " fmt "\e[0m\n", \
		__FILE__, __LINE__, ##args)
#define MSG(fmt, args...) fprintf(stdout, "\e[1;34m" fmt "\e[0m\n", ##args)

#define _UNUSED_ __attribute__((unused))

#define SAMPLE_MAX_NUM_OBSERVATIONS 	8

extern char *gResourceUri;
extern const char *deviceType;
extern const char *dateOfManufacture;
extern const char *deviceName;
extern const char *firmwareVersion;
extern const char *manufacturerName;
extern const char *operatingSystemVersion;
extern const char *hardwareVersion;
extern const char *platformID;
extern const char *manufacturerLink;
extern const char *modelNumber;
extern const char *platformVersion;
extern const char *supportLink;
extern const char *version;
extern const char *systemTime;
extern const char *specVersion;
extern const char *dataModelVersions;
extern const char *piid;

extern OCPlatformInfo platformInfo;
extern OCDeviceInfo deviceInfo;

/* Structure to represent a LED resource */
typedef struct LEDRESOURCE {
	OCResourceHandle handle;
	bool state;
	int power;
} LEDResource;

static LEDResource LED;

static bool observeThreadStarted;
pthread_t threadId_observe;

int gQuitFlag;

const char *getResult(OCStackResult result);
void DeletePlatformInfoLocal(void);
void DeleteDeviceInfoLocal(void);
bool DuplicateString(char **targetString, const char *sourceString);
void DeletePlatformInfo(void);
void DeleteDeviceInfoLocal(void);
bool DuplicateString(char **targetString, const char *sourceString);
void DeletePlatformInfo(void);
OCStackResult SetPlatformInfo(const char *platformID, const char *manufacturerName, const char *manufacturerUrl, const char *modelNumber, const char *dateOfManufacture, const char *platformVersion, const char *operatingSystemVersion, const char *hardwareVersion, const char *firmwareVersion, const char *supportUrl, const char *systemTime);
OCStackResult SetDeviceInfoLocal(const char *deviceName, const char *specVersion, const char *dataModelVersions, const deviceType);
OCEntityHandlerResult ProcessNonExistingResourceRequest(OCEntityHandlerRequest *ptr);
OCEntityHandlerResult OCNOPEntityHandlerCb(OCEntityHandlerFlag flag, OCEntityHandlerRequest *entityHandlerRequest, void *callbackParam);

OCEntityHandlerResult ocf_mylight_handler(OCEntityHandlerFlag flag, OCEntityHandlerRequest *req, void *user_data);
/* This method converts the payload to JSON format */
OCRepPayload *sec_constructResponse(OCEntityHandlerRequest *ehRequest);

/* Following methods process the PUT, GET, POST
 * requests
 */
OCEntityHandlerResult sec_ProcessGetRequest(OCEntityHandlerRequest *ehRequest, OCRepPayload **payload);
OCEntityHandlerResult sec_ProcessPutRequest(OCEntityHandlerRequest *ehRequest, OCRepPayload **payload);
OCEntityHandlerResult sec_ProcessPostRequest(OCEntityHandlerRequest *ehRequest, OCEntityHandlerResponse *response, OCRepPayload **payload);
int initLightResource(char *uri, LEDResource *resource, bool resourceState, int resourcePower);
const char *sec_get_light_url(unsigned int id);
OCRepPayload *sec_get_current_value(OCRepPayload *payload, unsigned int id);
int sec_get_current_hadle(unsigned int id, OCResourceHandle *handle);

int ocf_mylight_notify_emit(unsigned int light_id);

#endif
