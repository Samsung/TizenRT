/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
///////////////////////////////////////////////////////////////////////
//NOTE :  This sample server is generated based on ocserverbasicops.cpp
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include "ocstack.h"
#include "logger.h"
#include "ocpayload.h"

#include "file_data.c"

#if defined(CONFIG_TLS_WITH_SSS)
#include "tls/see_api.h"
#endif

#define TAG "SECURESERVER_JUSTWORKS"

int sec_gQuitFlag = 0;

/* Structure to represent a LED resource */
typedef struct LEDRESOURCE {
	OCResourceHandle handle;
	bool state;
	int power;
} LEDResource;

static LEDResource LED;
// This variable determines instance number of the LED resource.
// Used by POST method to create a new instance of LED resource.
static int gCurrLedInstance = 0;
#define SAMPLE_MAX_NUM_POST_INSTANCE  2
static LEDResource gLedInstance[SAMPLE_MAX_NUM_POST_INSTANCE];

char *sec_gResourceUri = (char *)"/a/led";

//Secure Virtual Resource database for Iotivity Server
//It contains Server's Identity and the PSK credentials
//of other devices which the server trusts

static char *CRED_FILE = "/mnt/oic_svr_db_server_justworks.dat";

/* Function that creates a new LED resource by calling the
 * OCCreateResource() method.
 */
int createLEDResource(char *uri, LEDResource *ledResource, bool resourceState, int resourcePower);

/* This method converts the payload to JSON format */
OCRepPayload *sec_constructResponse(OCEntityHandlerRequest *ehRequest);

/* Following methods process the PUT, GET, POST
 * requests
 */
OCEntityHandlerResult sec_ProcessGetRequest(OCEntityHandlerRequest *ehRequest, OCRepPayload **payload);
OCEntityHandlerResult sec_ProcessPutRequest(OCEntityHandlerRequest *ehRequest, OCRepPayload **payload);
OCEntityHandlerResult sec_ProcessPostRequest(OCEntityHandlerRequest *ehRequest, OCEntityHandlerResponse *response, OCRepPayload **payload);

/* Entity Handler callback functions */
OCEntityHandlerResult sec_OCEntityHandlerCb(OCEntityHandlerFlag flag, OCEntityHandlerRequest *entityHandlerRequest, void *callbackParam);

#if defined(LWIP_HAVE_LOOPIF) && LWIP_HAVE_LOOPIF == 1
#define NET_DEVNUM 1
#else
#define NET_DEVNUM 0
#endif

#if defined(CONFIG_ENC28J60) || defined(CONFIG_WICED)
#define NET_DEVNAME "en"
#elif defined(CONFIG_ARCH_BOARD_SIDK_S5JT200)
#define NET_DEVNAME "wl"
#else
#error "undefined CONFIG_NET_<type>, check your .config"
#endif

#define IOTIVITY_TEST_STACKSIZE   16384
#define IOTIVITY_TEST_PRI   100
#define IOTIVITY_TEST_SCHED_POLICIY SCHED_RR

const char *sec_getResult(OCStackResult result)
{
	switch (result) {
	case OC_STACK_OK:
		return "OC_STACK_OK";
	case OC_STACK_RESOURCE_CREATED:
		return "OC_STACK_RESOURCE_CREATED";
	case OC_STACK_RESOURCE_DELETED:
		return "OC_STACK_RESOURCE_DELETED";
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
#ifdef WITH_PRESENCE
	case OC_STACK_PRESENCE_STOPPED:
		return "OC_STACK_PRESENCE_STOPPED";
#endif
	case OC_STACK_ERROR:
		return "OC_STACK_ERROR";
	default:
		return "UNKNOWN";
	}
}

OCRepPayload *sec_getPayload(const char *uri, int64_t power, bool state)
{
	OCRepPayload *payload = OCRepPayloadCreate();
	if (!payload) {
		OIC_LOG(ERROR, TAG, "Failed to allocate Payload");
		return NULL;
	}

	OCRepPayloadSetUri(payload, uri);
	OCRepPayloadSetPropBool(payload, "state", state);
	OCRepPayloadSetPropInt(payload, "power", power);

	return payload;
}

//This function takes the request as an input and returns the response
OCRepPayload *sec_constructResponse(OCEntityHandlerRequest *ehRequest)
{
	if (ehRequest->payload && ehRequest->payload->type != PAYLOAD_TYPE_REPRESENTATION) {
		OIC_LOG(ERROR, TAG, "Incoming payload not a representation");
		return NULL;
	}

	OCRepPayload *input = (OCRepPayload *)(ehRequest->payload);

	LEDResource *currLEDResource = &LED;

	if (ehRequest->resource == gLedInstance[0].handle) {
		currLEDResource = &gLedInstance[0];
		sec_gResourceUri = (char *)"/a/led/0";
	} else if (ehRequest->resource == gLedInstance[1].handle) {
		currLEDResource = &gLedInstance[1];
		sec_gResourceUri = (char *)"/a/led/1";
	}

	if (OC_REST_PUT == ehRequest->method) {
		// Get pointer to query
		int64_t pow;
		if (OCRepPayloadGetPropInt(input, "power", &pow)) {
			currLEDResource->power = pow;
		}

		bool state;
		if (OCRepPayloadGetPropBool(input, "state", &state)) {
			currLEDResource->state = state;
		}
	}

	return sec_getPayload(sec_gResourceUri, currLEDResource->power, currLEDResource->state);
}

OCEntityHandlerResult sec_ProcessGetRequest(OCEntityHandlerRequest *ehRequest, OCRepPayload **payload)
{
	OCEntityHandlerResult ehResult;

	OCRepPayload *getResp = sec_constructResponse(ehRequest);

	if (getResp) {
		*payload = getResp;
		ehResult = OC_EH_OK;
	} else {
		ehResult = OC_EH_ERROR;
	}

	return ehResult;
}

OCEntityHandlerResult sec_ProcessPutRequest(OCEntityHandlerRequest *ehRequest, OCRepPayload **payload)
{
	OCEntityHandlerResult ehResult;

	OCRepPayload *putResp = sec_constructResponse(ehRequest);

	if (putResp) {
		*payload = putResp;
		ehResult = OC_EH_OK;
	} else {
		ehResult = OC_EH_ERROR;
	}

	return ehResult;
}

OCEntityHandlerResult sec_ProcessPostRequest(OCEntityHandlerRequest *ehRequest, OCEntityHandlerResponse *response, OCRepPayload **payload)
{
	OCRepPayload *respPLPost_led = NULL;
	OCEntityHandlerResult ehResult = OC_EH_OK;

	/*
	 * The entity handler determines how to process a POST request.
	 * Per the REST paradigm, POST can also be used to update representation of existing
	 * resource or create a new resource.
	 * In the sample below, if the POST is for /a/led then a new instance of the LED
	 * resource is created with default representation (if representation is included in
	 * POST payload it can be used as initial values) as long as the instance is
	 * lesser than max new instance count. Once max instance count is reached, POST on
	 * /a/led updated the representation of /a/led (just like PUT)
	 */

	if (ehRequest->resource == LED.handle) {
		if (gCurrLedInstance < SAMPLE_MAX_NUM_POST_INSTANCE) {
			// Create new LED instance
			char newLedUri[15] = "/a/led/";
			int newLedUriLength = strlen(newLedUri);
			snprintf(newLedUri + newLedUriLength, sizeof(newLedUri) - newLedUriLength, "%d", gCurrLedInstance);

			respPLPost_led = OCRepPayloadCreate();
			OCRepPayloadSetUri(respPLPost_led, sec_gResourceUri);
			OCRepPayloadSetPropString(respPLPost_led, "createduri", newLedUri);

			if (0 == createLEDResource(newLedUri, &gLedInstance[gCurrLedInstance], false, 0)) {
				OIC_LOG(INFO, TAG, "Created new LED instance");
				gLedInstance[gCurrLedInstance].state = 0;
				gLedInstance[gCurrLedInstance].power = 0;
				gCurrLedInstance++;
				strncpy((char *)response->resourceUri, newLedUri, MAX_URI_LENGTH);
				ehResult = OC_EH_RESOURCE_CREATED;
			}
		} else {
			respPLPost_led = sec_constructResponse(ehRequest);
		}
	} else {
		for (int i = 0; i < SAMPLE_MAX_NUM_POST_INSTANCE; i++) {
			if (ehRequest->resource == gLedInstance[i].handle) {
				if (i == 0) {
					respPLPost_led = sec_constructResponse(ehRequest);
					break;
				} else if (i == 1) {
					respPLPost_led = sec_constructResponse(ehRequest);
				}
			}
		}
	}

	if (respPLPost_led != NULL) {
		*payload = respPLPost_led;
		ehResult = OC_EH_OK;
	} else {
		OIC_LOG_V(INFO, TAG, "Payload was NULL");
		ehResult = OC_EH_ERROR;
	}

	return ehResult;
}

OCEntityHandlerResult sec_OCEntityHandlerCb(OCEntityHandlerFlag flag, OCEntityHandlerRequest *entityHandlerRequest, void *callbackParam)
{
	OIC_LOG_V(INFO, TAG, "Inside entity handler - flags: 0x%x", flag);
	(void)callbackParam;
	OCEntityHandlerResult ehResult = OC_EH_ERROR;

	OCEntityHandlerResponse response;
	memset(&response, 0, sizeof(response));

	// Validate pointer
	if (!entityHandlerRequest) {
		OIC_LOG(ERROR, TAG, "Invalid request pointer");
		return OC_EH_ERROR;
	}

	OCRepPayload *payload = NULL;

	if (flag & OC_REQUEST_FLAG) {
		OIC_LOG(INFO, TAG, "Flag includes OC_REQUEST_FLAG");
		if (entityHandlerRequest) {
			if (OC_REST_GET == entityHandlerRequest->method) {
				OIC_LOG(INFO, TAG, "Received OC_REST_GET from client");
				ehResult = sec_ProcessGetRequest(entityHandlerRequest, &payload);
			} else if (OC_REST_PUT == entityHandlerRequest->method) {
				OIC_LOG(INFO, TAG, "Received OC_REST_PUT from client");
				ehResult = sec_ProcessPutRequest(entityHandlerRequest, &payload);
			} else if (OC_REST_POST == entityHandlerRequest->method) {
				OIC_LOG(INFO, TAG, "Received OC_REST_POST from client");
				ehResult = sec_ProcessPostRequest(entityHandlerRequest, &response, &payload);
			} else {
				OIC_LOG_V(INFO, TAG, "Received unsupported method %d from client", entityHandlerRequest->method);
				ehResult = OC_EH_ERROR;
			}

			if (ehResult == OC_EH_OK && ehResult != OC_EH_FORBIDDEN) {
				// Format the response.  Note this requires some info about the request
				response.requestHandle = entityHandlerRequest->requestHandle;
				response.resourceHandle = entityHandlerRequest->resource;
				response.ehResult = ehResult;
				response.payload = (OCPayload *)(payload);
				response.numSendVendorSpecificHeaderOptions = 0;
				memset(response.sendVendorSpecificHeaderOptions, 0, sizeof(response.sendVendorSpecificHeaderOptions));
				memset(response.resourceUri, 0, sizeof(response.resourceUri));
				// Indicate that response is NOT in a persistent buffer
				response.persistentBufferFlag = 0;

				// Send the response
				if (OCDoResponse(&response) != OC_STACK_OK) {
					OIC_LOG(ERROR, TAG, "Error sending response");
					ehResult = OC_EH_ERROR;
				}
			}
		}
	}

	OCPayloadDestroy(response.payload);
	return ehResult;
}

#if 0
/* SIGINT handler: set sec_gQuitFlag to 1 for graceful termination */
void handleSigInt(int signum)
{
	if (signum == SIGINT) {
		sec_gQuitFlag = 1;
	}
}
#endif

/* Initialize oic_svr_db_server_justworks.dat file
*  Current implementation:  CRED_FILE data is loaded/hardcoded in byte_array(data_oic_svr_db_server_justworks_dat)
*/
int init_oic_svr_db_server_justworks_file()
{
	int ret = -1;
	FILE *mfile = NULL;
	printf("[%s] Init %s file called\n", TAG, CRED_FILE);
	mfile = fopen(CRED_FILE, 'r');
	if (NULL == mfile) {
		/* This will happen during first boot */
		printf("[%s] creating file %s \n", TAG, CRED_FILE);
		mfile = fopen(CRED_FILE, "wb");
		if (!mfile) {
			printf("[%s] ERROR creating a file\n", TAG);
			return -1;
		}
		/* write to new file from byte array: data_oic_svr_db_server_justworks_dat */
		printf("[%s] Writing: oic_svr_db_server_justworks_dat size =%d\n", TAG, sizeof(data_oic_svr_db_server_justworks_dat));
		ret = fwrite(data_oic_svr_db_server_justworks_dat, sizeof(char), sizeof(data_oic_svr_db_server_justworks_dat), mfile);
		if (ret != sizeof(data_oic_svr_db_server_justworks_dat)) {
			printf("[%s] Write failed %d returning NULL\n", TAG, ret);
			return -1;
		}
	}
	ret = fclose(mfile);
	if (ret != 0) {
		printf("[%s] Close failed %d returning NULL\n", TAG, ret);
		return -1;
	}
	return 0;
}

FILE *secureserver_fopen(const char *path, const char *mode)
{
	(void)path;
	return fopen(CRED_FILE, mode);
}

OCStackResult initSecureServerPersistentStorage()
{
	//Initialize Persistent Storage for SVR database
	static OCPersistentStorage ps = { secureserver_fopen, fread, fwrite, fclose, unlink };

	return OCRegisterPersistentStorageHandler(&ps);
}

int secureServer_main_cb(int argc, char *argv[])
{
	struct timespec timeout;

#if defined(CONFIG_TLS_WITH_SSS)
	see_init();
#endif

	printf("[%s] OCServer is starting...\n", TAG);
	/* Intialize Persistent Storage */
	if (OC_STACK_OK != initSecureServerPersistentStorage()) {
		printf("[%s] initSecureServerPersistentStorage failed \n", TAG);
		return -2;
	}
	/* Intialize Required Files */
	if (init_oic_svr_db_server_justworks_file() != 0) {
		printf("[%s] init_oic_svr_db_server_justworks_file failed \n", TAG);
		return -1;
	}
	/* Intialize OCStack */
	if (OCInit(NULL, 0, OC_SERVER) != OC_STACK_OK) {
		printf("[%s] OCStack init error \n", TAG);
		return -1;
	}

	/*
	 * Declare and create the example resource: LED
	 */
	createLEDResource(sec_gResourceUri, &LED, false, 0);

	timeout.tv_sec = 0;
	timeout.tv_nsec = 100000000L;

	// Break from loop with Ctrl-C
	printf("[%s] Entering ocserver main loop \n", TAG);
	// signal(SIGINT, handleSigInt);
	while (!sec_gQuitFlag) {
		if (OCProcess() != OC_STACK_OK) {
			printf("[%s] OCStack process error \n", TAG);
			return -1;
		}
		nanosleep(&timeout, NULL);
	}

	printf("[%s] Exiting ocserver main loop \n", TAG);

	if (OCStop() != OC_STACK_OK) {
		printf("[%s] OCStack process error \n", TAG);
	}
#if defined(CONFIG_TLS_WITH_SSS)
	see_free();
#endif

	return 0;
}

int createLEDResource(char *uri, LEDResource *ledResource, bool resourceState, int resourcePower)
{
	if (!uri) {
		printf("[%s] Resource URI cannot be NULL \n", TAG);
		return -1;
	}

	ledResource->state = resourceState;
	ledResource->power = resourcePower;
	OCStackResult res = OCCreateResource(&(ledResource->handle),
										 "core.led",
										 OC_RSRVD_INTERFACE_DEFAULT,
										 uri,
										 sec_OCEntityHandlerCb,
										 NULL,
										 OC_DISCOVERABLE | OC_OBSERVABLE | OC_SECURE);
	printf("[%s] Created LED resource with result: %s \n", TAG, sec_getResult(res));

	return 0;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int secureServer_main(int argc, char *argv[])
#endif
{
	task_create("iotivity_secureServer", IOTIVITY_TEST_PRI, IOTIVITY_TEST_STACKSIZE, secureServer_main_cb, (FAR char *const *)NULL);
	return 0;
}
