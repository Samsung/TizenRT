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
#include <ocstack.h>
#include "logger.h"
#include "ocpayload.h"
#include "iotivity_common.h"

#define TAG "IOTIVITY COMMON"

char *gResourceUri = (char *)"/a/light/0";
const char *deviceType = "oic.d.light";
const char *dateOfManufacture = "2017-07-11";
const char *deviceName = "artik05x_tinyara";
const char *firmwareVersion = "LSI_1.0";
const char *manufacturerName = "Samsung LSI";
const char *operatingSystemVersion = "myOS";
const char *hardwareVersion = "ARTIK_LSI_05x";
const char *platformID = "C0FFEE00-BAB0-FACE-B00C-00BA5EBA1100";
const char *manufacturerLink = "https://www.iotivity.org";
const char *modelNumber = "ARTIK05x";
const char *platformVersion = "TINYARA_1.0";
const char *supportLink = "https://www.iotivity.org";
const char *version = "IOTIVITY_1.3_REL";
const char *systemTime = "2017-07-11T16:06:00+09:00";
const char *specVersion = "ocf.1.1.0";
const char *dataModelVersions = "ocf.res.1.1.0,ocf.sh.1.1.0";
const char *piid = "C0FFEE00-BAB0-FACE-B00C-000000000000";

OCPlatformInfo platformInfo;
OCDeviceInfo deviceInfo;

const char *getResult(OCStackResult result)
{
	switch (result) {
	case OC_STACK_OK:
		return "OC_STACK_OK";
	case OC_STACK_RESOURCE_CREATED:
		return "OC_STACK_RESOURCE_CREATED";
	case OC_STACK_RESOURCE_DELETED:
		return "OC_STACK_RESOURCE_DELETED";
	case OC_STACK_RESOURCE_CHANGED:
		return "OC_STACK_RESOURCE_CHANGED";
	case OC_STACK_INVALID_URI:
		return "OC_STACK_INVALID_URI";
	case OC_STACK_INVALID_QUERY:
		return "OC_STACK_INVALID_QUERY";
	case OC_STACK_INVALID_IP:
		return "OC_STACK_INVALID_IP";
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
	case OC_STACK_UNAUTHORIZED_REQ:
		return "OC_STACK_UNAUTHORIZED_REQ";
#ifdef WITH_PRESENCE
	case OC_STACK_PRESENCE_STOPPED:
		return "OC_STACK_PRESENCE_STOPPED";
	case OC_STACK_PRESENCE_TIMEOUT:
		return "OC_STACK_PRESENCE_TIMEOUT";
#endif
	case OC_STACK_ERROR:
		return "OC_STACK_ERROR";
	default:
		return "UNKNOWN";
	}
}

void DeletePlatformInfoLocal(void)
{
	free(platformInfo.platformID);
	free(platformInfo.manufacturerName);
	free(platformInfo.manufacturerUrl);
	free(platformInfo.modelNumber);
	free(platformInfo.dateOfManufacture);
	free(platformInfo.platformVersion);
	free(platformInfo.operatingSystemVersion);
	free(platformInfo.hardwareVersion);
	free(platformInfo.firmwareVersion);
	free(platformInfo.supportUrl);
	free(platformInfo.systemTime);
}

void DeleteDeviceInfoLocal(void)
{
	free(deviceInfo.deviceName);
	free(deviceInfo.specVersion);
	OCFreeOCStringLL(deviceInfo.dataModelVersions);
	OCFreeOCStringLL(deviceInfo.types);
}

bool DuplicateString(char **targetString, const char *sourceString)
{
	if (!sourceString) {
		return false;
	} else {
		*targetString = (char *)malloc(strlen(sourceString) + 1);

		if (*targetString) {
			strncpy(*targetString, sourceString, (strlen(sourceString) + 1));
			return true;
		}
	}
	return false;
}

void DeletePlatformInfo(void)
{
	OIC_LOG(INFO, TAG, "Deleting platform info.");

	free(platformInfo.platformID);
	platformInfo.platformID = NULL;

	free(platformInfo.manufacturerName);
	platformInfo.manufacturerName = NULL;

	free(platformInfo.manufacturerUrl);
	platformInfo.manufacturerUrl = NULL;

	free(platformInfo.modelNumber);
	platformInfo.modelNumber = NULL;

	free(platformInfo.dateOfManufacture);
	platformInfo.dateOfManufacture = NULL;

	free(platformInfo.platformVersion);
	platformInfo.platformVersion = NULL;

	free(platformInfo.operatingSystemVersion);
	platformInfo.operatingSystemVersion = NULL;

	free(platformInfo.hardwareVersion);
	platformInfo.hardwareVersion = NULL;

	free(platformInfo.firmwareVersion);
	platformInfo.firmwareVersion = NULL;

	free(platformInfo.supportUrl);
	platformInfo.supportUrl = NULL;

	free(platformInfo.systemTime);
	platformInfo.systemTime = NULL;
}

OCStackResult SetPlatformInfo(const char *platformID, const char *manufacturerName, const char *manufacturerUrl, const char *modelNumber, const char *dateOfManufacture, const char *platformVersion, const char *operatingSystemVersion, const char *hardwareVersion, const char *firmwareVersion, const char *supportUrl, const char *systemTime)
{

	bool success = true;

	if (manufacturerName != NULL && (strlen(manufacturerName) > MAX_MANUFACTURER_NAME_LENGTH)) {
		return OC_STACK_INVALID_PARAM;
	}

	if (manufacturerUrl != NULL && (strlen(manufacturerUrl) > MAX_MANUFACTURER_URL_LENGTH)) {
		return OC_STACK_INVALID_PARAM;
	}

	if (!DuplicateString(&platformInfo.platformID, platformID)) {
		success = false;
	}

	if (!DuplicateString(&platformInfo.manufacturerName, manufacturerName)) {
		success = false;
	}

	if (!DuplicateString(&platformInfo.manufacturerUrl, manufacturerUrl)) {
		success = false;
	}

	if (!DuplicateString(&platformInfo.modelNumber, modelNumber)) {
		success = false;
	}

	if (!DuplicateString(&platformInfo.dateOfManufacture, dateOfManufacture)) {
		success = false;
	}

	if (!DuplicateString(&platformInfo.platformVersion, platformVersion)) {
		success = false;
	}

	if (!DuplicateString(&platformInfo.operatingSystemVersion, operatingSystemVersion)) {
		success = false;
	}

	if (!DuplicateString(&platformInfo.hardwareVersion, hardwareVersion)) {
		success = false;
	}

	if (!DuplicateString(&platformInfo.firmwareVersion, firmwareVersion)) {
		success = false;
	}

	if (!DuplicateString(&platformInfo.supportUrl, supportUrl)) {
		success = false;
	}

	if (!DuplicateString(&platformInfo.systemTime, systemTime)) {
		success = false;
	}

	if (success) {
		return OC_STACK_OK;
	}

	DeletePlatformInfo();
	return OC_STACK_ERROR;
}

OCStackResult SetDeviceInfoLocal(const char *deviceName, const char *specVersion, const char *dataModelVersions, const deviceType)
{
	if (!DuplicateString(&deviceInfo.deviceName, deviceName)) {
		return OC_STACK_ERROR;
	}
	if (!DuplicateString(&deviceInfo.specVersion, specVersion)) {
		return OC_STACK_ERROR;
	}
	OCFreeOCStringLL(deviceInfo.dataModelVersions);
	deviceInfo.dataModelVersions = OCCreateOCStringLL(dataModelVersions);
	if (!deviceInfo.dataModelVersions) {
		return OC_STACK_ERROR;
	}

	OCFreeOCStringLL(deviceInfo.types);
	deviceInfo.types = OCCreateOCStringLL(deviceType);
	if (!deviceInfo.types) {
		return OC_STACK_ERROR;
	}

	return OC_STACK_OK;
}

OCEntityHandlerResult ProcessNonExistingResourceRequest(OCEntityHandlerRequest *ptr)
{
	OIC_LOG_V(INFO, TAG, "\n\nExecuting %s ", __func__);

	return OC_EH_RESOURCE_NOT_FOUND;
}

OCEntityHandlerResult OCNOPEntityHandlerCb(OCEntityHandlerFlag flag, OCEntityHandlerRequest *entityHandlerRequest, void *callbackParam)
{
	// This is callback is associated with the 2 presence notification
	// resources. They are non-operational.
	return OC_EH_OK;
}
