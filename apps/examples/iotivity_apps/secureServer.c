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
#include "ocserver.h"

#include "file_data.c"
#include "iotivity_common.h"

#if defined(CONFIG_TLS_WITH_SSS)
#include "tls/see_api.h"
#endif

#define TAG "SECURESERVER_JUSTWORKS"
#define STORAGE_MOUNT_POINT "/mnt/"
#define DB_NAME data_oic_svr_db_server_dat

static int gObserveNotifyType = 3;

int gQuitFlag;
int gLightUnderObservation;

Observers interestedObservers[SAMPLE_MAX_NUM_OBSERVATIONS];

pthread_t threadId_observe;
pthread_t threadId_presence;

static bool observeThreadStarted;

/* Structure to represent a LED resource */
typedef struct LEDRESOURCE {
	OCResourceHandle handle;
	bool state;
	int power;
} LEDResource;

#ifdef WITH_PRESENCE
#define numPresenceResources (2)
#endif

static LEDResource LED;
// This variable determines instance number of the LED resource.
// Used by POST method to create a new instance of LED resource.
static int gCurrLedInstance = 0;
#define SAMPLE_MAX_NUM_POST_INSTANCE  2
static LEDResource gLedInstance[SAMPLE_MAX_NUM_POST_INSTANCE];

//Secure Virtual Resource database for Iotivity Server
//It contains Server's Identity and the PSK credentials
//of other devices which the server trusts
static char *CRED_FILE = "/mnt/oic_svr_db.dat";
static char *PROP_FILE = "/mnt/device_properties.dat";

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
#elif defined(CONFIG_ARCH_BOARD_ARTIK05X_FAMILY)
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
		printf("Failed to allocate Payload\n");
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
		printf("Incoming payload not a representation\n");
		return NULL;
	}

	OCRepPayload *input = (OCRepPayload *)(ehRequest->payload);

	LEDResource *currLEDResource = &LED;

	if (ehRequest->resource == gLedInstance[0].handle) {
		currLEDResource = &gLedInstance[0];
		gResourceUri = (char *)"/a/led/0";
	} else if (ehRequest->resource == gLedInstance[1].handle) {
		currLEDResource = &gLedInstance[1];
		gResourceUri = (char *)"/a/led/1";
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

	return sec_getPayload(gResourceUri, currLEDResource->power, currLEDResource->state);
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
			OCRepPayloadSetUri(respPLPost_led, gResourceUri);
			OCRepPayloadSetPropString(respPLPost_led, "createduri", newLedUri);

			if (0 == createLEDResource(newLedUri, &gLedInstance[gCurrLedInstance], false, 0)) {
				printf("Created new LED instance\n");
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
		printf("Payload was NULL\n");
		ehResult = OC_EH_ERROR;
	}

	return ehResult;
}

OCEntityHandlerResult sec_ProcessDeleteRequest(OCEntityHandlerRequest *ehRequest)
{
	if (ehRequest == NULL) {
		OIC_LOG(INFO, TAG, "The ehRequest is NULL");
		return OC_EH_ERROR;
	}
	OCEntityHandlerResult ehResult = OC_EH_OK;

	OIC_LOG_V(INFO, TAG, "\n\nExecuting %s for resource %p ", __func__, ehRequest->resource);

	/*
	 * In the sample below, the application will:
	 * 1a. pass the delete request to the c stack
	 * 1b. internally, the c stack figures out what needs to be done and does it accordingly
	 *    (e.g. send observers notification, remove observers...)
	 * 1c. the c stack returns with the result whether the request is fullfilled.
	 * 2. optionally, app removes observers out of its array 'interestedObservers'
	 */

	if ((ehRequest != NULL) && (ehRequest->resource == LED.handle)) {
		//Step 1: Ask stack to do the work.
		OCStackResult result = OCDeleteResource(ehRequest->resource);

		if (result == OC_STACK_OK) {
			OIC_LOG(INFO, TAG, "\n\nDelete Resource operation succeeded.");
			ehResult = OC_EH_OK;

			//Step 2: clear observers who wanted to observe this resource at the app level.
			for (uint8_t i = 0; i < SAMPLE_MAX_NUM_OBSERVATIONS; i++) {
				if (interestedObservers[i].resourceHandle == ehRequest->resource) {
					interestedObservers[i].valid = false;
					interestedObservers[i].observationId = 0;
					interestedObservers[i].resourceHandle = NULL;
				}
			}
		} else if (result == OC_STACK_NO_RESOURCE) {
			OIC_LOG(INFO, TAG, "\n\nThe resource doesn't exist or it might have been deleted.");
			ehResult = OC_EH_RESOURCE_DELETED;
		} else {
			OIC_LOG(INFO, TAG, "\n\nEncountered error from OCDeleteResource().");
			ehResult = OC_EH_ERROR;
		}
	} else if (ehRequest->resource != LED.handle) {
		//Let's this app not supporting DELETE on some resources so
		//consider the DELETE request is received for a non-support resource.
		OIC_LOG(INFO, TAG, "\n\nThe request is received for a non-support resource.");
		ehResult = OC_EH_FORBIDDEN;
	}

	return ehResult;
}

void ProcessObserveRegister(OCEntityHandlerRequest *ehRequest)
{
	int ret;
	OIC_LOG_V(INFO, TAG, "Received observation registration request with observation Id %d", ehRequest->obsInfo.obsId);

	if (!observeThreadStarted) {
		pthread_attr_t attr;
		struct sched_param sparam;
		pthread_attr_init(&attr);
		sparam.sched_priority = PTHREAD_DEFAULT_PRIORITY;
		(void)pthread_attr_setschedparam(&attr, &sparam);
#ifdef CONFIG_IOTIVITY_PTHREAD_STACKSIZE
		(void)pthread_attr_setstacksize(&attr, CONFIG_IOTIVITY_PTHREAD_STACKSIZE);
#else
		(void)pthread_attr_setstacksize(&attr, PTHREAD_STACK_DEFAULT);
#endif
		pthread_create(&threadId_observe, &attr, ChangeLightRepresentation, (void *)NULL);
		ret = pthread_create(&threadId_observe, &attr, ChangeLightRepresentation, (void *)NULL);
		if (ret != 0) {
			printf("%s: pthread_create failed[%d]\n", __func__, ret);
			return;
		}
		pthread_setname_np(threadId_observe, "ChangeLightRepresentation");
		observeThreadStarted = 1;
	}
	for (uint8_t i = 0; i < SAMPLE_MAX_NUM_OBSERVATIONS; i++) {
		if (interestedObservers[i].valid == false) {
			interestedObservers[i].observationId = ehRequest->obsInfo.obsId;
			interestedObservers[i].valid = true;
			gLightUnderObservation = 1;
			break;
		}
	}
}

void ProcessObserveDeregister(OCEntityHandlerRequest *ehRequest)
{
	bool clientStillObserving = false;

	OIC_LOG_V(INFO, TAG, "Received observation deregistration request for observation Id %d", ehRequest->obsInfo.obsId);
	for (uint8_t i = 0; i < SAMPLE_MAX_NUM_OBSERVATIONS; i++) {
		if (interestedObservers[i].observationId == ehRequest->obsInfo.obsId) {
			interestedObservers[i].valid = false;
		}
		if (interestedObservers[i].valid == true)
			// Even if there is one single client observing we continue notifying entity handler
		{
			clientStillObserving = true;
		}
	}
	if (clientStillObserving == false) {
		gLightUnderObservation = 0;
	}
}

OCEntityHandlerResult sec_OCEntityHandlerCb(OCEntityHandlerFlag flag, OCEntityHandlerRequest *entityHandlerRequest, void *callbackParam)
{
	printf("Inside entity handler - flags: 0x%x\n", flag);
	(void)callbackParam;
	OCEntityHandlerResult ehResult = OC_EH_ERROR;

	OCEntityHandlerResponse response;
	memset(&response, 0, sizeof(response));

	// Validate pointer
	if (!entityHandlerRequest) {
		printf("Invalid request pointer\n");
		return OC_EH_ERROR;
	}

	OCRepPayload *payload = NULL;

	if (flag & OC_REQUEST_FLAG) {
		printf("Flag includes OC_REQUEST_FLAG\n");
		if (entityHandlerRequest) {
			if (OC_REST_GET == entityHandlerRequest->method) {
				printf("Received OC_REST_GET from client\n");
				ehResult = sec_ProcessGetRequest(entityHandlerRequest, &payload);
			} else if (OC_REST_PUT == entityHandlerRequest->method) {
				printf("Received OC_REST_PUT from client\n");
				ehResult = sec_ProcessPutRequest(entityHandlerRequest, &payload);
			} else if (OC_REST_POST == entityHandlerRequest->method) {
				printf("Received OC_REST_POST from client\n");
				ehResult = sec_ProcessPostRequest(entityHandlerRequest, &response, &payload);
			} else {
				printf("Received unsupported method %d from client\n", entityHandlerRequest->method);
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
					printf("Error sending response\n");
					ehResult = OC_EH_ERROR;
				}
			}
		}
	}

	OCPayloadDestroy(response.payload);
	return ehResult;
}

OCEntityHandlerResult OCDeviceEntityHandlerCb(OCEntityHandlerFlag flag, OCEntityHandlerRequest *entityHandlerRequest, char *uri, void *callbackParam)
{
	OIC_LOG_V(INFO, TAG, "Inside device default entity handler - flags: 0x%x, uri: %s", flag, uri);

	OCEntityHandlerResult ehResult = OC_EH_OK;
	OCEntityHandlerResponse response;

	// Validate pointer
	if (!entityHandlerRequest) {
		OIC_LOG(ERROR, TAG, "Invalid request pointer");
		return OC_EH_ERROR;
	}
	// Initialize certain response fields
	response.numSendVendorSpecificHeaderOptions = 0;
	memset(response.sendVendorSpecificHeaderOptions, 0, sizeof response.sendVendorSpecificHeaderOptions);
	memset(response.resourceUri, 0, sizeof response.resourceUri);
	OCRepPayload *payload = NULL;

	if (flag & OC_REQUEST_FLAG) {
		OIC_LOG(INFO, TAG, "Flag includes OC_REQUEST_FLAG");

		if (entityHandlerRequest->resource == NULL) {
			OIC_LOG(INFO, TAG, "Received request from client to a non-existing resource");
			ehResult = ProcessNonExistingResourceRequest(entityHandlerRequest);
		} else if (OC_REST_GET == entityHandlerRequest->method) {
			OIC_LOG(INFO, TAG, "Received OC_REST_GET from client");
			ehResult = sec_ProcessGetRequest(entityHandlerRequest, &payload);
		} else if (OC_REST_PUT == entityHandlerRequest->method) {
			OIC_LOG(INFO, TAG, "Received OC_REST_PUT from client");
			ehResult = sec_ProcessPutRequest(entityHandlerRequest, &payload);
		} else if (OC_REST_DELETE == entityHandlerRequest->method) {
			OIC_LOG(INFO, TAG, "Received OC_REST_DELETE from client");
			ehResult = sec_ProcessDeleteRequest(entityHandlerRequest);
		} else {
			OIC_LOG_V(INFO, TAG, "Received unsupported method %d from client", entityHandlerRequest->method);
			ehResult = OC_EH_ERROR;
		}
		// If the result isn't an error or forbidden, send response
		if (!((ehResult == OC_EH_ERROR) || (ehResult == OC_EH_FORBIDDEN))) {
			// Format the response.  Note this requires some info about the request
			response.requestHandle = entityHandlerRequest->requestHandle;
			response.resourceHandle = entityHandlerRequest->resource;
			response.ehResult = ehResult;
			response.payload = (OCPayload *) payload;
			// Indicate that response is NOT in a persistent buffer
			response.persistentBufferFlag = 0;

			// Send the response
			if (OCDoResponse(&response) != OC_STACK_OK) {
				OIC_LOG(ERROR, TAG, "Error sending response");
				ehResult = OC_EH_ERROR;
			}
		}
	}
	if (flag & OC_OBSERVE_FLAG) {
		OIC_LOG(INFO, TAG, "Flag includes OC_OBSERVE_FLAG");
		if (OC_OBSERVE_REGISTER == entityHandlerRequest->obsInfo.action) {
			OIC_LOG(INFO, TAG, "Received OC_OBSERVE_REGISTER from client");
		} else if (OC_OBSERVE_DEREGISTER == entityHandlerRequest->obsInfo.action) {
			OIC_LOG(INFO, TAG, "Received OC_OBSERVE_DEREGISTER from client");
		}
	}

	return ehResult;
}

void *ChangeLightRepresentation(void *param)
{
	(void)param;
	OCStackResult result = OC_STACK_ERROR;

	uint8_t j = 0;
	uint8_t numNotifies = (SAMPLE_MAX_NUM_OBSERVATIONS) / 2;
	OCObservationId obsNotify[numNotifies];

	while (!gQuitFlag) {
		sleep(3);
		LED.power += 5;
		if (gLightUnderObservation) {
			OIC_LOG_V(INFO, TAG, " =====> Notifying stack of new power level %d\n", Light.power);
			if (gObserveNotifyType == 1) {
				// Notify list of observers. Alternate observers on the list will be notified.
				j = 0;
				for (uint8_t i = 0; i < SAMPLE_MAX_NUM_OBSERVATIONS; (i = i + 2)) {
					if (interestedObservers[i].valid == true) {
						obsNotify[j] = interestedObservers[i].observationId;
						j++;
					}
				}

				OCRepPayload *payload = getPayload(gResourceUri, LED.power, LED.state);
				result = OCNotifyListOfObservers(LED.handle, obsNotify, j, payload, OC_NA_QOS);
				OCRepPayloadDestroy(payload);
			} else if (gObserveNotifyType == 0) {
				// Notifying all observers
				result = OCNotifyAllObservers(LED.handle, OC_NA_QOS);
				if (OC_STACK_NO_OBSERVERS == result) {
					OIC_LOG(INFO, TAG, "=======> No more observers exist, stop sending observations");
					gLightUnderObservation = 0;
				}
			} else {
				OIC_LOG(ERROR, TAG, "Incorrect notification type selected");
			}
		}
	}
	return NULL;
}

#ifdef WITH_PRESENCE
void *presenceNotificationGenerator(void *param)
{
	uint8_t secondsBeforePresence = 10;
	OIC_LOG_V(INFO, TAG, "Will send out presence in %u seconds", secondsBeforePresence);
	sleep(secondsBeforePresence);
	(void)param;
	OCDoHandle presenceNotificationHandles[numPresenceResources];
	OCStackResult res = OC_STACK_OK;

	char *presenceNotificationResources[numPresenceResources] = { "oic.r.switch.binary" };
	char *presenceNotificationUris[numPresenceResources] = { gResourceUri };

	for (int i = 0; i < numPresenceResources; i++) {
		if (res == OC_STACK_OK) {
			sleep(1);
			res = OCCreateResource(&presenceNotificationHandles[i], presenceNotificationResources[i], OC_RSRVD_INTERFACE_DEFAULT, presenceNotificationUris[i], OCNOPEntityHandlerCb, NULL, OC_DISCOVERABLE | OC_OBSERVABLE);
		}
		if (res != OC_STACK_OK) {
			OIC_LOG_V(ERROR, TAG, "\"Presence Notification Generator\" failed to create resource " "%s with result %s.", presenceNotificationResources[i], getResult(res));
			break;
		}
		OIC_LOG_V(INFO, TAG, PCF("Created %s for presence notification"), presenceNotificationUris[i]);
	}
	sleep(5);
	for (int i = 0; i < numPresenceResources; i++) {
		if (res == OC_STACK_OK) {
			res = OCDeleteResource(presenceNotificationHandles[i]);
		}
		if (res != OC_STACK_OK) {
			OIC_LOG_V(ERROR, TAG, "\"Presence Notification Generator\" failed to delete " "resource %s.", presenceNotificationResources[i]);
			break;
		}
		OIC_LOG_V(INFO, TAG, PCF("Deleted %s for presence notification"), presenceNotificationUris[i]);
	}

	OIC_LOG(INFO, TAG, "================ stopping presence");
	OCStopPresence();

	return NULL;
}
#endif

/* Initialize oic_svr_db_server_justworks.dat file
*  Current implementation:  CRED_FILE data is loaded/hardcoded in byte_array(data_oic_svr_db_server_dat)
*/
int init_oic_svr_db_server_justworks_file()
{
	int ret = -1;
	FILE *mfile = NULL;
	printf("[%s] Init %s file called\n", TAG, CRED_FILE);

	unlink(PROP_FILE);
	unlink(CRED_FILE);
	printf("removed db file\n");
	mfile = fopen(CRED_FILE, 'r');
	if (NULL == mfile) {
		/* This will happen during first boot */
		printf("[%s] creating file %s \n", TAG, CRED_FILE);
		mfile = fopen(CRED_FILE, "wb");
		if (!mfile) {
			printf("[%s] ERROR creating a file\n", TAG);
			return -1;
		}
		/* write to new file from byte array: data_oic_svr_db_server_dat */
		printf("[%s] Writing: oic_svr_db_server_justworks_dat size =%d\n", TAG, sizeof(DB_NAME));
		ret = fwrite(DB_NAME, sizeof(char), sizeof(DB_NAME), mfile);
		if (ret != sizeof(DB_NAME)) {
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
	char path_buf[CONFIG_SMARTFS_MAXNAMLEN + sizeof(STORAGE_MOUNT_POINT)];
	memset(path_buf, 0, sizeof(path_buf));
	strcat(path_buf, STORAGE_MOUNT_POINT);
	strcat(path_buf, path);
	(void)path;
	printf("fopen : %s\n", path_buf);
	return fopen(path_buf, mode);
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

	observeThreadStarted = false;
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

#ifdef RA_ADAPTER
	OCSetRAInfo(&rainfo);
#endif
	if (OCInit(NULL, 0, OC_SERVER) != OC_STACK_OK) {
		printf("[%s] OCStack init error \n", TAG);
		return -1;
	}
#ifdef WITH_PRESENCE
	if (OCStartPresence(0) != OC_STACK_OK) {
		OIC_LOG(ERROR, TAG, "OCStack presence/discovery error");
		return 0;
	}
#endif

	/* Set Platform and device information */
	OCSetDefaultDeviceEntityHandler(OCDeviceEntityHandlerCb, NULL);
	OCStackResult registrationResult = SetPlatformInfo(platformID, manufacturerName, manufacturerLink, modelNumber,
									   dateOfManufacture, platformVersion, operatingSystemVersion, hardwareVersion,
									   firmwareVersion, supportLink, systemTime);
	if (registrationResult != OC_STACK_OK) {
		printf("Platform info setting failed locally!\n");
		exit(EXIT_FAILURE);
	}

	registrationResult = OCSetPlatformInfo(platformInfo);

	if (registrationResult != OC_STACK_OK) {
		printf("Platform Registration failed!");
		exit(EXIT_FAILURE);
	}

	registrationResult = SetDeviceInfoLocal(deviceName, specVersion, dataModelVersions, deviceType);

	if (registrationResult != OC_STACK_OK) {
		printf("Device info setting failed locally!");
		exit(EXIT_FAILURE);
	}

	registrationResult = OCSetDeviceInfo(deviceInfo);

	if (registrationResult != OC_STACK_OK) {
		printf("Device Registration failed!");
		exit(EXIT_FAILURE);
	}

	/*
	 * Declare and create the example resource: LED
	 */
	createLEDResource(gResourceUri, &LED, false, 0);

	for (uint8_t i = 0; i < SAMPLE_MAX_NUM_OBSERVATIONS; i++) {
		interestedObservers[i].valid = false;
	}

	timeout.tv_sec = 0;
	timeout.tv_nsec = 100000000L;

	// Break from loop with Ctrl-C
	printf("[%s] Entering ocserver main loop \n", TAG);
	// signal(SIGINT, handleSigInt);
	DeletePlatformInfoLocal();
	DeleteDeviceInfoLocal();

	while (!gQuitFlag) {
		if (OCProcess() != OC_STACK_OK) {
			printf("[%s] OCStack process error \n", TAG);
			return -1;
		}
		nanosleep(&timeout, NULL);
	}

	printf("[%s] Exiting ocserver main loop \n", TAG);

	if (observeThreadStarted) {
		pthread_cancel(threadId_observe);
		pthread_join(threadId_observe, NULL);
	}

	pthread_cancel(threadId_presence);
	pthread_join(threadId_presence, NULL);

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
										 "oic.r.switch.binary",
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
