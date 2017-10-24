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

///
/// This sample provides the way to create cloud sample
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "cacommon.h"
#include "cacommonutil.h"
#include "ocstack.h"
#include "ocpayload.h"
#include "logger.h"
#include "mbedtls/ssl_ciphersuites.h"
#include "rd_client.h"

/* This definition is only for testing Cloud authentication */
/* Releated code will be removed */

#ifdef CONFIG_TLS_WITH_SSS
#include "mbedtls/see_api.h"
#include "mbedtls/x509_crt.h"
#include "mbedtls/pem.h"
unsigned int g_iotivity_key_index;
unsigned int g_iotivity_key_curve;
unsigned int g_iotivity_cert_index;
unsigned int g_iotivity_cacert_index;
#endif

#ifdef CONFIG_IOTIVITY_SSS_STORAGE
#include "mbedtls/sss_storage.h"
#endif

#ifdef CONFIG_ENABLE_IOTIVITY_SECURED
#include "file_data.c"
#endif

#define TAG       "CLOUDSERVER"
#define DEFAULT_CONTEXT_VALUE   0x99
#define DEFAULT_AUTH_SIGNUP   "/oic/account"
#define DEFAULT_AUTH_SESSION  "/oic/account/session"
#define DEFAULT_AUTH_REFRESH  "/oic/account/tokenrefresh"
#define DEFAULT_PUBLISH_QUERY   "/oic/rd?rt=oic.wk.rdpub"
#define DEFAULT_DISCOVER_QUERY  "/oic/res?rt=core.light"

#define DEFAULT_DB_FILE         "/mnt/cloud.dat"
#define DEFAULT_DB_SECURE_FILE  "/mnt/cloudsecure.dat"
#define DEFUALT_ROOTCA_FILE "/mnt/rootca.crt"

#ifdef CONFIG_IOTIVITY_SSS_STORAGE
#define IOTIVITY_TEST_STACKSIZE 8192
#else
#define IOTIVITY_TEST_STACKSIZE 4096
#endif

#define IOTIVITY_TEST_PRI     100
#define IOTIVITY_TEST_SCHED_POLICIY SCHED_RR

#define SIGNUP_OK   4
#define SIGNIN_OK   4
#define SIGNOUT_OK  4
#define SECURE_MODE 1

#ifdef CONFIG_ENABLE_IOTIVITY_SECURED
// TODO : remove this once provisioning is supported
/**
 * Macro to verify argument is not equal to NULL.
 * eg: VERIFY_NON_NULL(TAG, ptrData, ERROR,OC_STACK_ERROR);
 */
#define VERIFY_NON_NULL(tag, arg, logLevel, retValue) { if (NULL == (arg)) \
            { OIC_LOG((logLevel), tag, #arg " is NULL"); return retValue; } }

/**
 * Macro to verify success of operation.
 * eg: VERIFY_SUCCESS(TAG, OC_STACK_OK == foo(), ERROR, OC_STACK_ERROR);
 */
#define VERIFY_SUCCESS(tag, op, logLevel, retValue) { if (!(op)) \
            {OIC_LOG((logLevel), tag, #op " failed!!"); return retValue;} }

OicUuid_t WILDCARD_SUBJECT_ID_LOCAL = { "*" };

size_t WILDCARD_SUBJECT_ID_LEN_LOCAL = 1;
const char *TRUST_CA_LOCAL = "trust_ca";
trustCertChainContext_t g_trustCertChainNotifier;
#endif

const char *coaps_tcp_header = "coaps+tcp://";
const char *coap_tcp_header = "coap+tcp://";
char g_empty[MAX_URI_LENGTH * 2] = { 0 };
char g_host[MAX_URI_LENGTH * 2] = { 0 };
char g_uID[MAX_URI_LENGTH * 2] = { 0 };
char g_accessToken[MAX_URI_LENGTH * 2] = { 0 };
char g_refreshToken[MAX_URI_LENGTH * 2] = { 0 };
char g_authProviderName[MAX_URI_LENGTH * 2] = { 0 };
char g_authProviderCode[MAX_URI_LENGTH * 2] = { 0 };

char *gLightResourceUri = (char *)"/a/led";

static char *dbfname = DEFAULT_DB_FILE;
static char *dbsecfname = DEFAULT_DB_SECURE_FILE;
static char *rootcafname = DEFUALT_ROOTCA_FILE;
static uint16_t g_credId = 0;
static uint16_t g_mode = 0;
static uint8_t g_signup_done = 0;
static uint8_t g_signin_done = 0;

int gNumObserveNotifies = 0;
int g_runningMode = 0;

typedef struct {
	char *accesstoken;
	char *refreshtoken;
	char *tokentype;
	long expiresin;
	char *uid;
	char *redirecturi;
	char *certificate;
	char *sid;
} sessionObject_t;

static sessionObject_t sessionObject = { NULL, NULL, NULL, 0, NULL, NULL, NULL, NULL };

/**
 * Session free function
 *
 * @return  OCStackResult application result
 */
static void SessionFree()
{
	OICFree(sessionObject.accesstoken);
	OICFree(sessionObject.refreshtoken);
	OICFree(sessionObject.tokentype);
	OICFree(sessionObject.uid);
	OICFree(sessionObject.redirecturi);
	OICFree(sessionObject.certificate);
	OICFree(sessionObject.sid);

	memset(&sessionObject, 0, sizeof(sessionObject_t));
}

/**
 * Session parse payload
 *
 * @param[in] payload
 * @param[in] sessionObject session data
 * @return  OCStackResult application result
 */
static OCStackResult SessionParsePayload(OCRepPayload *payload)
{
	VERIFY_NON_NULL_RET(payload, TAG, "NULL payload", OC_STACK_ERROR);

	SessionFree();

	if (!OCRepPayloadGetPropString(payload, OC_RSRVD_ACCESS_TOKEN, &sessionObject.accesstoken)) {
		OIC_LOG_V(ERROR, TAG, "Can't get: %s", OC_RSRVD_ACCESS_TOKEN);
	}
	if (!OCRepPayloadGetPropString(payload, OC_RSRVD_REFRESH_TOKEN, &sessionObject.refreshtoken)) {
		OIC_LOG_V(ERROR, TAG, "Can't get: %s", OC_RSRVD_REFRESH_TOKEN);
	}
	if (!OCRepPayloadGetPropString(payload, OC_RSRVD_TOKEN_TYPE, &sessionObject.tokentype)) {
		OIC_LOG_V(ERROR, TAG, "Can't get: %s", OC_RSRVD_TOKEN_TYPE);
	}
	int64_t tmp = 0;
	if (!OCRepPayloadGetPropInt(payload, OC_RSRVD_EXPIRES_IN, &tmp)) {
		OIC_LOG_V(ERROR, TAG, "Can't get: %s", OC_RSRVD_EXPIRES_IN);
	} else {
		sessionObject.expiresin = tmp;
	}
	if (!OCRepPayloadGetPropString(payload, OC_RSRVD_USER_UUID, &sessionObject.uid)) {
		OIC_LOG_V(ERROR, TAG, "Can't get: %s", OC_RSRVD_USER_UUID);
	}
	if (!OCRepPayloadGetPropString(payload, OC_RSRVD_REDIRECT_URI, &sessionObject.redirecturi)) {
		OIC_LOG_V(ERROR, TAG, "Can't get: %s", OC_RSRVD_REDIRECT_URI);
	}
	if (!OCRepPayloadGetPropString(payload, OC_RSRVD_CERTIFICATE, &sessionObject.certificate)) {
		OIC_LOG_V(ERROR, TAG, "Can't get: %s", OC_RSRVD_CERTIFICATE);
	}
	if (!OCRepPayloadGetPropString(payload, OC_RSRVD_SUBJECT_ID, &sessionObject.sid)) {
		OIC_LOG_V(ERROR, TAG, "Can't get: %s", OC_RSRVD_SUBJECT_ID);
	}

	return OC_STACK_OK;
}

OCStackResult OCCloudSignup(const char *host, const char *deviceId, const char *authprovider, const char *authcode, OCClientResponseHandler response)
{
	char targetUri[MAX_URI_LENGTH * 2] = { 0, };
	snprintf(targetUri, MAX_URI_LENGTH * 2, "%s%s", host, DEFAULT_AUTH_SIGNUP);

	OCCallbackData cbData;
	memset(&cbData, 0, sizeof(OCCallbackData));
	cbData.cb = response;
	cbData.cd = NULL;
	cbData.context = (void *)DEFAULT_CONTEXT_VALUE;

	printf("OCCloudSignup with \n");
	printf("Host: %s\n", targetUri);
	printf("Dev id: %s\n", deviceId);
	printf("Auth Provider: %s\n", authprovider);
	printf("Auth Code: %s\n", authcode);

	OCRepPayload *registerPayload = OCRepPayloadCreate();
	if (!registerPayload) {
		goto no_memory;
	}

	OCRepPayloadSetPropString(registerPayload, "di", deviceId);
	OCRepPayloadSetPropString(registerPayload, "authprovider", authprovider);
	OCRepPayloadSetPropString(registerPayload, "authcode", authcode);

	return OCDoResource(NULL, OC_REST_POST, targetUri, NULL, (OCPayload *) registerPayload, CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);

no_memory:
	OCRepPayloadDestroy(registerPayload);
	return OC_STACK_NO_MEMORY;
}

OCStackResult OCCloudSession(const char *host, const char *query, const char *uId, const char *deviceId, const char *accesstoken, bool isLogin, OCClientResponseHandler response)
{
	char targetUri[MAX_URI_LENGTH * 2] = { 0, };
	snprintf(targetUri, MAX_URI_LENGTH * 2, "%s%s", host, query);

	OCCallbackData cbData;
	memset(&cbData, 0, sizeof(OCCallbackData));
	cbData.cb = response;
	cbData.cd = NULL;
	cbData.context = (void *)DEFAULT_CONTEXT_VALUE;

	printf("OCCloudSignin with \n");
	printf("Host: %s\n", targetUri);
	printf("Dev id: %s\n", deviceId);
	printf("UID : %s\n", uId);
	printf("Acess Token: %s\n", accesstoken);

	OCRepPayload *loginoutPayload = OCRepPayloadCreate();
	if (!loginoutPayload) {
		goto no_memory;
	}

	if (uId != NULL) {
		OCRepPayloadSetPropString(loginoutPayload, "uid", uId);
	}

	if (deviceId != NULL) {
		OCRepPayloadSetPropString(loginoutPayload, "di", deviceId);
	}

	if (accesstoken != NULL) {
		OCRepPayloadSetPropString(loginoutPayload, "accesstoken", accesstoken);
	}
	OCRepPayloadSetPropBool(loginoutPayload, "login", isLogin);

	return OCDoResource(NULL, OC_REST_POST, targetUri, NULL, (OCPayload *) loginoutPayload, CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);

no_memory:
	OCRepPayloadDestroy(loginoutPayload);
	return OC_STACK_NO_MEMORY;
}

//Client should call refresh before expiresin or when receive 4.01 during sign-in
OCStackResult OCCloudRefresh(const char *host, const char *query, const char *uId, const char *deviceId, const char *refreshtoken, OCClientResponseHandler response)
{
	char targetUri[MAX_URI_LENGTH * 2] = { 0, };
	snprintf(targetUri, MAX_URI_LENGTH * 2, "%s%s", host, query);

	OCCallbackData cbData;
	memset(&cbData, 0, sizeof(OCCallbackData));
	cbData.cb = response;
	cbData.cd = NULL;
	cbData.context = (void *)DEFAULT_CONTEXT_VALUE;

	OCRepPayload *refreshPayload = OCRepPayloadCreate();
	if (!refreshPayload) {
		goto no_memory;
	}

	OCRepPayloadSetPropString(refreshPayload, "uid", uId);
	OCRepPayloadSetPropString(refreshPayload, "di", deviceId);
	OCRepPayloadSetPropString(refreshPayload, "granttype", "refresh_token");
	OCRepPayloadSetPropString(refreshPayload, "refreshtoken", refreshtoken);

	return OCDoResource(NULL, OC_REST_POST, targetUri, NULL, (OCPayload *) refreshPayload, CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);

no_memory:
	OCRepPayloadDestroy(refreshPayload);
	return OC_STACK_NO_MEMORY;
}

OCStackResult OCCloudLogin(const char *host, const char *uId, const char *deviceId, const char *accesstoken, OCClientResponseHandler response)
{
	return OCCloudSession(host, DEFAULT_AUTH_SESSION, uId, deviceId, accesstoken, true, response);
}

OCStackResult OCCloudLogout(const char *host, OCClientResponseHandler response)
{
	return OCCloudSession(host, DEFAULT_AUTH_SESSION, NULL, NULL, NULL, false, response);
}

////////////////////////////////////////Device Sample
#define SAMPLE_MAX_NUM_POST_INSTANCE  2
typedef struct LIGHTRESOURCE {
	OCResourceHandle handle;
	bool state;
	int power;
} LightResource;
static LightResource gLightInstance[SAMPLE_MAX_NUM_POST_INSTANCE];

OCRepPayload *responsePayload(const char *uri, int64_t power, bool state)
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

OCEntityHandlerResult cloudConstructResponse(OCEntityHandlerRequest *ehRequest, OCRepPayload **payload)
{
	if (ehRequest->payload && ehRequest->payload->type != PAYLOAD_TYPE_REPRESENTATION) {
		printf("Incoming payload not a representation\n");
		return OC_EH_ERROR;
	}

	LightResource *currLightResource = NULL;

	if (ehRequest->resource == gLightInstance[0].handle) {
		currLightResource = &gLightInstance[0];
		gLightResourceUri = (char *)"/a/led/0";
	} else if (ehRequest->resource == gLightInstance[1].handle) {
		currLightResource = &gLightInstance[1];
		gLightResourceUri = (char *)"/a/led/1";
	}

	if (OC_REST_PUT == ehRequest->method) {
		// Get pointer to query
		int64_t pow;
		OCRepPayload *input = (OCRepPayload *)(ehRequest->payload);

		if (OCRepPayloadGetPropInt(input, "power", &pow)) {
			currLightResource->power = pow;
		}

		bool state;
		if (OCRepPayloadGetPropBool(input, "state", &state)) {
			currLightResource->state = state;
		}
	}

	*payload = responsePayload(gLightResourceUri, currLightResource->power, currLightResource->state);
	return OC_EH_OK;
}

OCEntityHandlerResult cloudProcessRequest(OCEntityHandlerRequest *ehRequest, OCRepPayload **payload)
{
	OCEntityHandlerResult Resp = cloudConstructResponse(ehRequest, payload);
	if (Resp != OC_EH_OK) {
		printf("constructResponse failed\n");
		return OC_EH_ERROR;
	}

	return OC_EH_OK;
}

#define SAMPLE_MAX_NUM_OBSERVATIONS  2
static bool observeThreadStarted = false;
int gCloudLightUnderObservation = 0;
pthread_t threadId_observe;
typedef struct {
	OCObservationId observationId;
	bool valid;
	OCResourceHandle resourceHandle;
} Observers;
Observers interestedObservers[SAMPLE_MAX_NUM_OBSERVATIONS];

void *cloudChangeLightRepresentation(void *param)
{
	(void)param;
	OCStackResult result = OC_STACK_ERROR;

	while (true) {
		sleep(3);
		gLightInstance[0].power += 1;
		gLightInstance[1].power += 3;

		if (gCloudLightUnderObservation) {
			printf(" =====> Notifying stack of new power level %d \n ", gLightInstance[0].power);
			printf(" =====> Notifying stack of new power level %d \n ", gLightInstance[1].power);
			// Notifying all observers
			result = OCNotifyAllObservers(gLightInstance[0].handle, OC_NA_QOS);
			result = OCNotifyAllObservers(gLightInstance[1].handle, OC_NA_QOS);
			printf(" =====> Notifying result %d \n", (int)result);
		}
	}
	return NULL;
}

void cloudProcessObserveRegister(OCEntityHandlerRequest *ehRequest)
{
	printf("Received observation registration request with observation Id %u \n", ehRequest->obsInfo.obsId);

	if (!observeThreadStarted) {
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		(void)pthread_attr_setschedparam(&attr, PTHREAD_DEFAULT_PRIORITY);
#ifdef CONFIG_IOTIVITY_PTHREAD_STACKSIZE
		(void)pthread_attr_setstacksize(&attr, CONFIG_IOTIVITY_PTHREAD_STACKSIZE);
#else
		(void)pthread_attr_setstacksize(&attr, PTHREAD_STACK_DEFAULT);
#endif
		pthread_create(&threadId_observe, &attr, cloudChangeLightRepresentation, (void *)NULL);
		observeThreadStarted = 1;
	}
	for (uint8_t i = 0; i < SAMPLE_MAX_NUM_OBSERVATIONS; i++) {
		if (interestedObservers[i].valid == false) {
			interestedObservers[i].observationId = ehRequest->obsInfo.obsId;
			interestedObservers[i].valid = true;
			gCloudLightUnderObservation = 1;
			break;
		}
	}
}

void cloudProcessObserveDeregister(OCEntityHandlerRequest *ehRequest)
{
	bool clientStillObserving = false;
	printf("Received observation deregistration request for observation Id  %u \n", ehRequest->obsInfo.obsId);
	for (uint8_t i = 0; i < SAMPLE_MAX_NUM_OBSERVATIONS; i++) {
		if (interestedObservers[i].observationId == ehRequest->obsInfo.obsId) {
			interestedObservers[i].valid = false;
		}
		if (interestedObservers[i].valid == true) {
			// Even if there is one single client observing we continue notifying entity handler
			clientStillObserving = true;
		}
	}
	if (clientStillObserving == false) {
		gCloudLightUnderObservation = 0;
	}
}

OCEntityHandlerResult cloudOCEntityHandlerCb(OCEntityHandlerFlag flag, OCEntityHandlerRequest *entityHandlerRequest, void *callback)
{
	OCEntityHandlerResult ehResult = OC_EH_ERROR;

	// Validate pointer
	if (!entityHandlerRequest) {
		printf("Invalid request pointer \n");
		return OC_EH_ERROR;
	}

	OCRepPayload *payload = NULL;

	if (flag & OC_REQUEST_FLAG) {
		printf("Flag includes OC_REQUEST_FLAG \n");

		switch (entityHandlerRequest->method) {
		case OC_REST_GET:
			ehResult = cloudProcessRequest(entityHandlerRequest, &payload);
			break;
		case OC_REST_PUT:
			ehResult = cloudProcessRequest(entityHandlerRequest, &payload);
			break;
		default:
			printf("Method %d doesn't supported! \n", entityHandlerRequest->method);
			break;
		}

		// If the result isn't an error or forbidden, send response
		if (ehResult == OC_EH_OK) {
			// Format the response.  Note this requires some info about the request
			OCEntityHandlerResponse response;
			memset(&response, 0, sizeof(OCEntityHandlerResponse));
			response.requestHandle = entityHandlerRequest->requestHandle;
			response.resourceHandle = entityHandlerRequest->resource;
			response.ehResult = ehResult;
			response.payload = (OCPayload *)(payload);
			response.numSendVendorSpecificHeaderOptions = 0;
			// Indicate that response is NOT in a persistent buffer
			response.persistentBufferFlag = 0;

			// Send the response
			if (OCDoResponse(&response) != OC_STACK_OK) {
				printf("Error sending response\n");
				ehResult = OC_EH_ERROR;
			}
		}
	}

	if (flag & OC_OBSERVE_FLAG) {
		printf("Flag includes OC_OBSERVE_FLAG\n");
		if (OC_OBSERVE_REGISTER == entityHandlerRequest->obsInfo.action) {
			printf("Received OC_OBSERVE_REGISTER from client\n");
			cloudProcessObserveRegister(entityHandlerRequest);
		} else if (OC_OBSERVE_DEREGISTER == entityHandlerRequest->obsInfo.action) {
			printf("Received OC_OBSERVE_DEREGISTER from client\n");
			cloudProcessObserveDeregister(entityHandlerRequest);
		}
	}
	return ehResult;
}

int cloudcreateLightResource(char *uri, LightResource *lightResource)
{
	if (!uri) {
		printf("Resource URI cannot be NULL\n");
		return -1;
	}
	uint8_t resourceProperties = 0;
	lightResource->state = false;
	lightResource->power = 0;
	resourceProperties = OC_DISCOVERABLE | OC_OBSERVABLE;
	if (g_mode == SECURE_MODE) {
		resourceProperties = (resourceProperties | OC_SECURE);
	}
	OCStackResult res = OCCreateResource(&(lightResource->handle),
										 "core.light",
										 "oc.mi.def",
										 uri,
										 cloudOCEntityHandlerCb,
										 NULL,
										 resourceProperties);
	if (res != OC_STACK_OK) {
		printf("Failed to create Light resource with uri: %s \n", uri);
	} else {
		printf("Created Light resource with uri: %s \n", uri);
	}

	return res;
}

OCStackApplicationResult handlePublishCB(void *ctx, OCDoHandle handle, OCClientResponse *clientResponse)
{
	if (ctx != (void *)DEFAULT_CONTEXT_VALUE) {
		printf("Invalid Publish callback received\n");
	}
	printf("Publish resource response received, code:%d \n", (int)clientResponse->result);

	return OC_STACK_KEEP_TRANSACTION;
}

void PublishResources(char *host)
{
	printf("Publishing resources...\n");
	printf("host: %s\n", host);

	if (cloudcreateLightResource((char *)"/a/light/0", &gLightInstance[0]) != 0) {
		printf("Unable to create sample resource\n");
	}

	if (cloudcreateLightResource((char *)"/a/light/1", &gLightInstance[1]) != 0) {
		printf("Unable to create sample resource\n");
	}

	OCResourceHandle resourceHandles[2] = { gLightInstance[0].handle, gLightInstance[1].handle, };
	OCCallbackData cbData;
	cbData.cb = handlePublishCB;
	cbData.context = (void *)DEFAULT_CONTEXT_VALUE;
	cbData.cd = NULL;

	printf("Publish default resources\n");

	OCDeviceInfo devInfoRoomLight;
	OCStringLL deviceType;

	deviceType.value = "oic.d.light";
	deviceType.next = NULL;
	devInfoRoomLight.deviceName = "Living Room Light";
	devInfoRoomLight.types = &deviceType;
	devInfoRoomLight.specVersion = NULL;
	devInfoRoomLight.dataModelVersions = NULL;

	OCStackResult res = OCSetDeviceInfo(devInfoRoomLight);

	if (res != OC_STACK_OK) {
		printf("Setting device info failed\n");
	}

	res = OCRDPublish(host, CT_ADAPTER_TCP, NULL, 0, &cbData, OC_LOW_QOS);
	if (res != OC_STACK_OK) {
		printf("Unable to publish default resources to cloud\n");
	}

	printf("Publish user resources\n");;

	res = OCRDPublish(host, CT_ADAPTER_TCP, resourceHandles, 2, &cbData, OC_LOW_QOS);
	if (res != OC_STACK_OK) {
		printf("Unable to publish user resources to cloud\n");
	}
	return;
}

/*
 *Client Sample
 */
void PrintRepresentation(OCRepPayloadValue *val)
{
	while (val) {
		printf("Key: %s Value:", val->name);
		switch (val->type) {
		case OCREP_PROP_NULL:
			printf(" Value: None\n");
			break;

		case OCREP_PROP_INT:
			printf(" Value: %d\n", val->i);
			break;

		case OCREP_PROP_DOUBLE:
			printf(" Value: %f\n", val->d);
			break;

		case OCREP_PROP_BOOL:
			printf(" Value: %u\n", val->b);
			break;

		case OCREP_PROP_STRING:
			printf(" Value: %s\n", val->str);
			break;

		case OCREP_PROP_BYTE_STRING:
			printf(" Value: %s\n", "[ByteString]");
			break;

		case OCREP_PROP_OBJECT:
			printf(" Value: %s\n", "[Object]");
			break;

		case OCREP_PROP_ARRAY:
			printf(" Value: %s\n", "[Array]");
			break;
		}

		val = val->next;
	}
}

OCStackApplicationResult obsReqCB(void *ctx, OCDoHandle handle, OCClientResponse *clientResponse)
{
	printf("Observe response received from %s \n", clientResponse->resourceUri);

	if (ctx != (void *)DEFAULT_CONTEXT_VALUE) {
		printf("Invalid Put callback received\n");
	}

	if (clientResponse) {
		if (clientResponse->payload == NULL) {
			printf("No payload received\n");
		}

		OCRepPayloadValue *val = ((OCRepPayload *) clientResponse->payload)->values;

		PrintRepresentation(val);

		gNumObserveNotifies++;
		if (gNumObserveNotifies > 5) {	//large number to test observing in DELETE case.
			printf("Cancelling with OC_HIGH_QOS\n");
			if (OCCancel(handle, OC_HIGH_QOS, NULL, 0) != OC_STACK_OK) {
				printf("Observe cancel error\n");
			}
		}
		if (clientResponse->sequenceNumber == OC_OBSERVE_REGISTER) {
			printf("This also serves as a registration confirmation\n");
		} else if (clientResponse->sequenceNumber == OC_OBSERVE_DEREGISTER) {
			printf("This also serves as a deregistration confirmation\n");
			return OC_STACK_DELETE_TRANSACTION;
		} else if (clientResponse->sequenceNumber == OC_OBSERVE_NO_OPTION) {
			printf("This also tells you that registration/deregistration failed\n");
			return OC_STACK_DELETE_TRANSACTION;
		}
	} else {
		printf("obsReqCB received Null clientResponse\n");
	}

	return OC_STACK_KEEP_TRANSACTION;
}

void ObserveResource(char **uri)
{
	OCCallbackData cbData;
	cbData.cb = obsReqCB;
	cbData.context = (void *)DEFAULT_CONTEXT_VALUE;
	cbData.cd = NULL;

	printf("Request OBSERVE to resource %s\n", *uri);

	OCStackResult res = OCDoResource(NULL, OC_REST_OBSERVE, *uri, NULL, NULL,
									 CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);
	printf("Requesting OBSERVE res=%d\n", (int)res);
}

OCStackApplicationResult putReqCB(void *ctx, OCDoHandle handle, OCClientResponse *clientResponse)
{
	printf("Put response received from %s\n ", clientResponse->resourceUri);

	if (ctx != (void *)DEFAULT_CONTEXT_VALUE) {
		printf("Invalid Put callback received\n");
	}

	if (clientResponse->payload == NULL) {
		printf("No payload received\n");
	}

	OCRepPayloadValue *val = ((OCRepPayload *) clientResponse->payload)->values;

	PrintRepresentation(val);

	char requestUri[MAX_URI_LENGTH * 2] = { 0, };
	snprintf(requestUri, MAX_URI_LENGTH * 2, "%s%s", g_host, clientResponse->resourceUri);

	ObserveResource(&requestUri);

	return OC_STACK_KEEP_TRANSACTION;
}

OCPayload *putRequestPayload()
{
	OCRepPayload *payload = OCRepPayloadCreate();

	if (!payload) {
		printf("Failed to create put payload object\n");
		exit(1);
	}

	OCRepPayloadSetPropInt(payload, "power", 15);
	OCRepPayloadSetPropBool(payload, "state", true);

	return (OCPayload *) payload;
}

void PutResource(char **uri)
{
	OCCallbackData cbData;
	cbData.cb = putReqCB;
	cbData.context = (void *)DEFAULT_CONTEXT_VALUE;
	cbData.cd = NULL;

	printf("Request PUT to resource%s\n", *uri);

	OCStackResult res = OCDoResource(NULL, OC_REST_PUT, *uri, NULL, putRequestPayload(),
									 CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);

	printf("Requesting PUT res=%d\n", res);
}

OCStackApplicationResult handleGetCB(void *ctx, OCDoHandle handle, OCClientResponse *clientResponse)
{
	printf("Get response received from %s\n", clientResponse->resourceUri);

	if (ctx != (void *)DEFAULT_CONTEXT_VALUE) {
		printf("Invalid Get callback received\n");
	}

	if (clientResponse->payload == NULL) {
		printf("No payload received\n");
	}

	if (clientResponse->payload != NULL && clientResponse->payload->type == PAYLOAD_TYPE_REPRESENTATION) {
		OCRepPayloadValue *val = ((OCRepPayload *) clientResponse->payload)->values;

		PrintRepresentation(val);

		char requestUri[MAX_URI_LENGTH * 2] = { 0, };
		snprintf(requestUri, MAX_URI_LENGTH * 2, "%s%s", g_host, clientResponse->resourceUri);

		PutResource(&requestUri);

	}

	return OC_STACK_KEEP_TRANSACTION;
}

void GetResource(char **uri)
{
	OCCallbackData cbData;
	cbData.cb = handleGetCB;
	cbData.context = (void *)DEFAULT_CONTEXT_VALUE;
	cbData.cd = NULL;

	printf("Request GET to resource %s\n", *uri);

	OCStackResult res = OCDoResource(NULL, OC_REST_GET, *uri, NULL, NULL,
									 CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0);

	printf("Requesting GET res=%d\n", (int)res);
}

// This is a function called back when a device is discovered
OCStackApplicationResult discoveryReqCB(void *ctx, OCDoHandle handle, OCClientResponse *clientResponse)
{
	if (ctx == (void *)DEFAULT_CONTEXT_VALUE) {
		printf("Callback Context for DISCOVER query recvd successfully\n");
	}

	if (clientResponse) {
		printf("StackResult: %d\n", (int)clientResponse->result);

		OCDiscoveryPayload *payload = (OCDiscoveryPayload *) clientResponse->payload;
		if (!payload) {
			printf("Empty payload\n");
			return OC_STACK_DELETE_TRANSACTION;
		}

		OCResourcePayload *resource = (OCResourcePayload *) payload->resources;
		if (!resource) {
			printf("No resources in payload\n");
			return OC_STACK_DELETE_TRANSACTION;
		}

		while (resource) {
			printf("Found Resource %s\n", resource->uri);

			char requestUri[MAX_URI_LENGTH * 2] = { 0, };
			snprintf(requestUri, MAX_URI_LENGTH * 2, "%s%s", g_host, resource->uri);

			GetResource(&requestUri);

			resource = resource->next;
		}
	} else {
		printf("discoveryReqCB received Null clientResponse\n");
	}
	return OC_STACK_KEEP_TRANSACTION;
}

void DiscoverResources(char *host)
{
	printf("Running as Client mode\n");

	char requestQuery[MAX_URI_LENGTH * 2] = { 0, };
	snprintf(requestQuery, MAX_URI_LENGTH * 2, "%s%s", host, DEFAULT_DISCOVER_QUERY);

	printf("Finding resources...\n");
	printf("%s\n", requestQuery);

	OCCallbackData cbData;

	cbData.cb = discoveryReqCB;
	cbData.context = (void *)DEFAULT_CONTEXT_VALUE;
	cbData.cd = NULL;

	if (OCDoResource(NULL, OC_REST_DISCOVER, requestQuery, NULL, 0, CT_ADAPTER_TCP, OC_LOW_QOS, &cbData, NULL, 0) != OC_STACK_OK) {
		printf("Unable to find resources from cloud\n");
	}
}

/////////////////////////////////////////////Common sample

static OCStackApplicationResult handleCloudSignInResponse(void *ctx, OCDoHandle handle, OCClientResponse *response)
{
	OC_UNUSED(ctx);
	OC_UNUSED(handle);

	VERIFY_NON_NULL_RET(response, TAG, "Received NULL response", OC_STACK_DELETE_TRANSACTION);

	if (response->payload) {
		printf("Payload received \n");
	}

	if (response->result != SIGNIN_OK) {
		printf("Cloud Sign In error: result: %d \n", response->result);
		return OC_STACK_DELETE_TRANSACTION;
	}

	int64_t tmp = 0;
	if (!OCRepPayloadGetPropInt((OCRepPayload *) response->payload, OC_RSRVD_EXPIRES_IN, &tmp)) {
		printf("Can't get: %s \n", OC_RSRVD_EXPIRES_IN);
	} else {
		printf("Key: %s Value: %d \n", OC_RSRVD_EXPIRES_IN, tmp);
		sessionObject.expiresin = tmp;
		printf("Cloud Sign In OK \n");

		if (g_runningMode == 1) {
			PublishResources(g_host);
		} else if (g_runningMode == 2) {
			DiscoverResources(g_host);
		}
	}
	g_signin_done = 1;
	return OC_STACK_DELETE_TRANSACTION;
}

static OCStackApplicationResult handleCloudSignUpResponse(void *ctx, OCDoHandle handle, OCClientResponse *clientResponse)
{
	if (ctx != (void *)DEFAULT_CONTEXT_VALUE) {
		printf("Invalid Register callback received \n");
	}

	VERIFY_NON_NULL_RET(clientResponse, TAG, "Received NULL response", OC_STACK_DELETE_TRANSACTION);

	if (clientResponse->result != SIGNUP_OK) {
		printf("Cloud Sign - Up error: %d \n", clientResponse->result);
		return OC_STACK_DELETE_TRANSACTION;
	}

	if (clientResponse->payload != NULL && clientResponse->payload->type == PAYLOAD_TYPE_REPRESENTATION) {

		printf("PAYLOAD_TYPE_REPRESENTATION received \n");
		printf("You can Sign - In using received uid and accesstoken variable when disconnected or reboot \n");

		OCRepPayloadValue *val = ((OCRepPayload *) clientResponse->payload)->values;
		PrintRepresentation(val);
		SessionParsePayload((OCRepPayload *) clientResponse->payload);
		snprintf(g_uID, MAX_URI_LENGTH * 2, "%s", sessionObject.uid);
		snprintf(g_accessToken, MAX_URI_LENGTH * 2, "%s", sessionObject.accesstoken);
	}

	printf("Cloud Sign Up OK \n");
	printf("uID Value =  '%s' and AcessToken Value =  '%s'\n", g_uID, g_accessToken);
	g_signup_done = 1;

	return OC_STACK_DELETE_TRANSACTION;
}

void PrintUsage()
{
	printf(" \ n ");
	printf(" Usage(Sign - Up to cloud): \n");
	printf(" TASH >> cloud_device <cmd-id> <addr: port > <auth provider > <auth code > <mode > \n");
	printf("         <cmd-id>: [1 - Sign-Up to cloud 2- Sign-In to cloud] Use \"1\" here\n");
	printf("         <addr:port>: Cloud Address, \"172.16.10.60:5683\"\n");
	printf("         <auth provider>: String value\n");
	printf("         <auth code>: String value\n");
	printf("         <mode> With/Without Security:0 - Non Secure, 1 - Secure\n");
	printf("example: TASH>> iotivityCldServ 1 172.16.10.60:5683 github e37776fae12f62f0cd06 1 \n");
	printf("\n");
	printf("Usage (Sign-In to cloud): \n");
	printf("TASH>> cloud_device <cmd-id> <addr:port> <uid> <accesstoken> <mode> \n");
	printf("         <cmd-id>: [1 - Sign-Up to cloud 2- Sign-In to cloud] Use \"2\" here\n");
	printf("         <addr:port>: Cloud Address, \"172.16.10.60:5683\"\n");
	printf("         <uid>: String value, Provided by response of cloud sign-up\n\tor kind of registration portal \n");
	printf("         <accesstoken>: String value, Provided by response of cloud sign-up\n\tor kind of registration portal \n");
	printf("         <mode> With/Without Security:0 - Non Secure, 1 - Secure\n");
	printf("example: TASH>> iotivityCldServ 2 172.16.10.60:5683 bce90122-d437-4504-9bbb-b69762181e1f 168b81ddb0b1faea5ee00d30d4796948d53eefb3 1 \n");
	printf("\n");
}

bool readFile(const char *name, OCByteString *out)
{
	FILE *file = NULL;
	int length = 0;
	uint8_t *buffer = NULL;
	bool result = false;

	//Open file
	file = fopen(name, "rb");
	if (!file) {
		OIC_LOG_V(ERROR, TAG, "Unable to open file %s", name);
		return result;
	}
	//Get file length
	if (fseek(file, 0, SEEK_END)) {
		OIC_LOG(ERROR, TAG, "Failed to SEEK_END");
		goto exit;
	}

	length = ftell(file);
	if (length < 0) {
		OIC_LOG(ERROR, TAG, "Failed to ftell");
		goto exit;
	}

	if (fseek(file, 0, SEEK_SET)) {
		OIC_LOG(ERROR, TAG, "Failed to SEEK_SET");
		goto exit;
	}
	//Allocate memory
	buffer = (uint8_t *)malloc(length);
	if (!buffer) {
		OIC_LOG(ERROR, TAG, "Failed to allocate buffer");
		goto exit;
	}
	//Read file contents into buffer
	size_t count = 1;
	size_t realCount = fread(buffer, length, count, file);
	if (realCount != count) {
		OIC_LOG_V(ERROR, TAG, "Read %d bytes %zu times instead of %zu", length, realCount, count);
		goto exit;
	}

	out->bytes = buffer;
	out->len = length;

	result = true;
exit:
	fclose(file);
	return result;
}

#ifdef CONFIG_ENABLE_IOTIVITY_SECURED

OCStackResult OCSaveTrustCertChain(uint8_t *trustCertChain, size_t chainSize, OicEncodingType_t encodingType, uint16_t *credId)
{
	OIC_LOG(DEBUG, TAG, "IN SRPSaveTrustCertChain");
	VERIFY_NON_NULL(TAG, trustCertChain, ERROR, OC_STACK_INVALID_PARAM);
	VERIFY_NON_NULL(TAG, credId, ERROR, OC_STACK_INVALID_PARAM);

	OCStackResult res = OC_STACK_ERROR;

	OicSecCred_t *cred = (OicSecCred_t *) OICCalloc(1, sizeof(*cred));
	VERIFY_NON_NULL(TAG, cred, ERROR, OC_STACK_NO_MEMORY);

	memcpy(cred->subject.id, &WILDCARD_SUBJECT_ID_LOCAL, WILDCARD_SUBJECT_ID_LEN_LOCAL);

	cred->credUsage = (char *)OICCalloc(1, strlen(TRUST_CA_LOCAL) + 1);
	VERIFY_NON_NULL(TAG, cred->credUsage, ERROR, OC_STACK_NO_MEMORY);
	OICStrcpy(cred->credUsage, strlen(TRUST_CA_LOCAL) + 1, TRUST_CA_LOCAL);

	cred->credType = SIGNED_ASYMMETRIC_KEY;

	if (encodingType == OIC_ENCODING_PEM) {
		cred->optionalData.data = (uint8_t *) OICCalloc(1, chainSize + 1);
		VERIFY_NON_NULL(TAG, cred->optionalData.data, ERROR, OC_STACK_NO_MEMORY);
		cred->optionalData.len = chainSize + 1;
	} else {
		cred->optionalData.data = (uint8_t *) OICCalloc(1, chainSize);
		VERIFY_NON_NULL(TAG, cred->optionalData.data, ERROR, OC_STACK_NO_MEMORY);
		cred->optionalData.len = chainSize;
	}
	memcpy(cred->optionalData.data, trustCertChain, chainSize);
	cred->optionalData.encoding = encodingType;

	res = AddCredential(cred);
	if (res != OC_STACK_OK) {
		DeleteCredList(cred);
		return res;
	}
	*credId = cred->credId;

	if (g_trustCertChainNotifier.callback) {
		uint8_t *certChain = (uint8_t *) OICCalloc(1, sizeof(uint8_t) * chainSize);
		VERIFY_NON_NULL(TAG, certChain, ERROR, OC_STACK_NO_MEMORY);
		memcpy(certChain, trustCertChain, chainSize);
		g_trustCertChainNotifier.callback(g_trustCertChainNotifier.context, credId, certChain, chainSize);
		OICFree(certChain);
	}

	OIC_LOG(DEBUG, TAG, "OUT SRPSaveTrustCertChain");

	return res;
}

static OCStackResult saveTrustCert(void)
{
	OCStackResult res = OC_STACK_ERROR;
	OIC_LOG(INFO, TAG, "Save Trust Cert. Chain into Cred of SVR");

	ByteArray_t trustCertChainArray = { 0, 0 };

#if defined(CONFIG_TLS_WITH_SSS) && defined(CONFIG_HW_ECDSA)
	unsigned char *buf = NULL;
	unsigned int buflen = SEE_BUF_MAX_SIZE;

	buf = malloc(SEE_BUF_MAX_SIZE);

	if (buf == NULL) {
		OIC_LOG(ERROR, TAG, "Failed to allocate buffer");
		return OC_STACK_ERROR;
	}

	/* Load rootca certificate from SSS */
	if ((res = see_get_certificate(buf, &buflen, g_iotivity_cacert_index, CERT_PEM)) != 0) {
		OIC_LOG(ERROR, TAG, "Failed to get certificate");
		free(buf);
		return OC_STACK_ERROR;
	}

	trustCertChainArray.data = malloc(buflen);

	if (trustCertChainArray.data == NULL) {
		OIC_LOG(ERROR, TAG, "Failed to allocate buffer");
		free(buf);
		return OC_STACK_ERROR;
	}

	memcpy(trustCertChainArray.data, buf, buflen);
	free(buf);

	trustCertChainArray.len = buflen;
#else
	const char *filename = "/mnt/rootca.crt";

	if (!readFile(filename, (OCByteString *)&trustCertChainArray)) {
		OIC_LOG_V(ERROR, TAG, "Can't read %s file", filename);
		OICFree(((OCByteString *)&trustCertChainArray)->bytes);
		return OC_STACK_ERROR;
	}
	OIC_LOG_BUFFER(DEBUG, TAG, trustCertChainArray.data, trustCertChainArray.len);
#endif
	res = OCSaveTrustCertChain(trustCertChainArray.data, trustCertChainArray.len, OIC_ENCODING_PEM, &g_credId);

	if (OC_STACK_OK != res) {
		OIC_LOG(ERROR, TAG, "OCSaveTrustCertChain API error");
	} else {
		OIC_LOG_V(INFO, TAG, "CredId of Saved Trust Cert. Chain into Cred of SVR : %d.\n", g_credId);
	}
	OICFree(trustCertChainArray.data);

	return res;
}

/* Initialize rootca.crt file
 */
int init_rootcacrt_file()
{
	int ret = -1;
	FILE *mfile = NULL;
	printf("[%s] Init %s file called\n", TAG, rootcafname);

	mfile = fopen(rootcafname, 'r');
	if (NULL == mfile) {
		/* This will happen during first boot */
		printf("[%s] creating file %s \n", TAG, rootcafname);
		mfile = fopen(rootcafname, "wb");
		if (NULL == mfile) {
			printf("[%s] ERROR creating a file\n", TAG);
			return -1;
		}
		/* write to new file from byte array: data_cloud_dat */
		printf("[%s] Writing: rootca crt size =%d\n", TAG, sizeof(data_rootca_crt));
		ret = fwrite(data_rootca_crt, sizeof(char), sizeof(data_rootca_crt), mfile);
		if (ret != sizeof(data_rootca_crt)) {
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

/* Initialize cloud.dat file
 */
int init_clouddat_file()
{
	int ret = -1;
	FILE *mfile = NULL;
	printf("[%s] Init %s file called\n", TAG, dbsecfname);
#ifdef CONFIG_IOTIVITY_SSS_STORAGE
	mfile = sss_open(dbsecfname, 'r');
#else
	mfile = fopen(dbsecfname, 'r');
#endif

	if (NULL == mfile) {
		/* This will happen during first boot */
		printf("[%s] creating file %s \n", TAG, dbsecfname);
#ifdef CONFIG_IOTIVITY_SSS_STORAGE
		mfile = sss_open(dbsecfname, "wb");
#else
		mfile = fopen(dbsecfname, "wb");
#endif
		if (!mfile) {
			printf("[%s] ERROR creating a file\n", TAG);
			return -1;
		}
		/* write to new file from byte array: data_cloud_dat */
		printf("[%s] Writing: cloud dat size =%d\n", TAG, sizeof(data_cloud_dat));
#ifdef CONFIG_IOTIVITY_SSS_STORAGE
		ret = sss_write(data_cloud_dat, sizeof(char), sizeof(data_cloud_dat), mfile);
#else
		ret = fwrite(data_cloud_dat, sizeof(char), sizeof(data_cloud_dat), mfile);
#endif
		if (ret != sizeof(data_cloud_dat)) {
			printf("[%s] Write failed %d returning NULL\n", TAG, ret);
			return -1;
		}
	}
#ifdef CONFIG_IOTIVITY_SSS_STORAGE
	ret = sss_close(mfile);
#else
	ret = fclose(mfile);
#endif
	if (ret != 0) {
		printf("[%s] Close failed %d returning NULL\n", TAG, ret);
		return -1;
	}

	return 0;
}

#endif

FILE *cloudserver_fopen(const char *path, const char *mode)
{
	OC_UNUSED(path);
	if (g_mode == SECURE_MODE) {
		return fopen(dbsecfname, mode);
	} else {
		return fopen(dbfname, mode);
	}
}

OCStackResult initCloudServerPersistentStorage()
{
	//Initialize Persistent Storage for SVR database
#ifdef CONFIG_IOTIVITY_SSS_STORAGE
	static OCPersistentStorage ps = { sss_open, sss_read, sss_write, sss_close, sss_unlink };
#else
	static OCPersistentStorage ps = { cloudserver_fopen, fread, fwrite, fclose, unlink };
#endif
	return OCRegisterPersistentStorageHandler(&ps);
}

int iotivity_cloud_signin(int argc, char *argv[])
{
	printf("IoTivity Cloud Address  = '%s'\n", g_host);
	printf("Sign-In to cloud using uID =  '%s' and AcessToken Value =  '%s'\n", g_uID, g_accessToken);

	OCStackResult res;

	g_runningMode = 1;

	res = OC_STACK_ERROR;

	res = OCCloudLogin(g_host, g_uID, OCGetServerInstanceIDString(), g_accessToken, handleCloudSignInResponse);

	printf("OCCloudLogin return %d\n", (int)res);

	while (true) {
		if (OCProcess() != OC_STACK_OK) {
			printf("OCProcess process error\n");
		}
		sleep(1);
	}

	if (OCStop() != OC_STACK_OK) {
		printf("OCStop process error\n");
	}

	snprintf(g_host, MAX_URI_LENGTH * 2, "%s", g_empty);
	snprintf(g_accessToken, MAX_URI_LENGTH * 2, "%s", g_empty);
	return 0;
}

int iotivity_cloud_signup(int argc, char *argv[])
{
	if (g_authProviderName == NULL || g_authProviderCode == NULL || g_host == NULL) {
		printf("Incorrect Parameters!!!\n");
		return 1;
	}

	printf("IoTivity Cloud Address  = '%s'\n", g_host);
	printf("OAuth Provider Name     = '%s'\n", g_authProviderName);
	printf("OAuth Provider Code     = '%s'\n", g_authProviderCode);

	OCStackResult res;

	printf("Sign-Up to cloud using %s %s\n", g_authProviderName, g_authProviderCode);

	res = OC_STACK_ERROR;
	res = OCCloudSignup(g_host, OCGetServerInstanceIDString(), g_authProviderName, g_authProviderCode, handleCloudSignUpResponse);

	printf("OCCloudSignup return %d\n", (int)res);

	printf("Waiting response..\n");

	while (true) {
		if (OCProcess() != OC_STACK_OK) {
			printf("OCProcess process error\n");
		}
		sleep(1);
		if (g_signup_done) {
			break;
		}
	}
	if (g_signup_done) {
		iotivity_cloud_signin(argc, argv);
	}

	snprintf(g_authProviderName, MAX_URI_LENGTH * 2, "%s", g_empty);
	snprintf(g_authProviderCode, MAX_URI_LENGTH * 2, "%s", g_empty);

	return 0;
}

#if defined(CONFIG_TLS_WITH_SSS) && defined(CONFIG_HW_ECDSA)

//this callback will be invoked to get key context based on key usage
void *OCGetHwKey(const char *service, const char *usage, const char *keytype)
{
	printf("%s In \n", __func__);

	if (service == NULL || usage == NULL) {
		return NULL;
	}

	/* Debug Print */
	printf("service : %s\n", service);
	printf("usage   : %s\n", usage);
	printf("keytype : %s\n", keytype);

	mbedtls_pk_context *pkey;

	pkey = malloc(sizeof(mbedtls_pk_context));

	if (pkey == NULL) {
		printf("%s : pkey context alloc failed\n", __func__);
		return NULL;
	}

	printf("%s Out \n", __func__);
	return (void *)pkey;
}

//this callback will free key context that was retreived from TZ
int OCFreeHwKey(void *keyContext)
{
	printf("%s In \n", __func__);

	if (keyContext == NULL) {
		return -1;
	}

	free(keyContext);

	printf("%s Out \n", __func__);

	return 0;
}

//this callback will be invoked to load own certificate in case of TZ
int OCGetOwnCertFromHw(const void *keyContext, uint8_t **certChain, size_t *certChainLen)
{
	int ret;
	unsigned char *buf;
	unsigned int buflen = SEE_BUF_MAX_SIZE;

	(void *)keyContext;

	printf("%s In \n", __func__);

	if (certChain == NULL || certChainLen == NULL) {
		return -1;
	}

	buf = (unsigned char *)malloc(buflen);

	if (buf == NULL) {
		return -1;
	}

	if ((ret = see_get_certificate(buf, &buflen, g_iotivity_cert_index, CERT_PEM)) != 0) {
		printf("see_get_cert fail %d\n", ret);
		free(buf);
		return -1;
	}

	*certChain = malloc(buflen);

	if (*certChain == NULL) {
		free(buf);
		return -1;
	}

	memcpy(*certChain, buf, buflen);
	*certChainLen = buflen;

	free(buf);

	printf("%s Out \n", __func__);
	return 0;
}

//this callback will be invoked to load private key in case of TZ
int OCSetupPkContextFromHw(mbedtls_pk_context *ctx, void *keyContext)
{
	mbedtls_pk_context *pkey = keyContext;

	printf("%s In \n", __func__);

	if (ctx == NULL || pkey == NULL) {
		return -1;
	}

	mbedtls_pk_init(pkey);

	const mbedtls_pk_info_t *pk_info;

	if ((pk_info = mbedtls_pk_info_from_type(MBEDTLS_PK_ECKEY)) == NULL) {
		mbedtls_pk_free(pkey);
		free(pkey);
		return -1;
	}

	if (mbedtls_pk_setup(pkey, pk_info)) {
		mbedtls_pk_free(pkey);
		free(pkey);
		return -1;
	}

	mbedtls_ecdsa_init((mbedtls_ecdsa_context *) pkey->pk_ctx);
	((mbedtls_ecdsa_context *)(pkey->pk_ctx))->key_index = g_iotivity_key_index;
	((mbedtls_ecdsa_context *)(pkey->pk_ctx))->grp.id = g_iotivity_key_curve;

	ctx->pk_info = pkey->pk_info;
	ctx->pk_ctx = pkey->pk_ctx;

	printf("%s Out \n", __func__);

	return 0;
}

const char test_ecc_crt[] =
	"-----BEGIN CERTIFICATE-----\r\n" "MIICaDCCAgygAwIBAgIBAjAMBggqhkjOPQQDAgUAMHAxLTArBgNVBAMTJFNhbXN1\r\n" "bmcgRWxlY3Ryb25pY3MgT0NGIFJvb3QgQ0EgVEVTVDEUMBIGA1UECxMLT0NGIFJv\r\n" "b3QgQ0ExHDAaBgNVBAoTE1NhbXN1bmcgRWxlY3Ryb25pY3MxCzAJBgNVBAYTAktS\r\n" "MCAXDTE2MTEyNDAyNDcyN1oYDzIwNjkxMjMxMTQ1OTU5WjBwMS0wKwYDVQQDEyRT\r\n" "YW1zdW5nIEVsZWN0cm9uaWNzIE9DRiBSb290IENBIFRFU1QxFDASBgNVBAsTC09D\r\n" "RiBSb290IENBMRwwGgYDVQQKExNTYW1zdW5nIEVsZWN0cm9uaWNzMQswCQYDVQQG\r\n" "EwJLUjBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABBzzury7p8HANVn+v4CIa2h/\r\n" "R/SAt3VVst+vTv4/kR+lgU1OEiT3t9+mOWE7J+oddpRofFW2DdeJkpfQUVOn4NOj\r\n" "gZIwgY8wDgYDVR0PAQH/BAQDAgHGMC4GA1UdHwQnMCUwI6AhoB+GHWh0dHA6Ly9j\r\n" "YS5zYW1zdW5naW90cy5jb20vY3JsMA8GA1UdEwEB/wQFMAMBAf8wPAYIKwYBBQUH\r\n" "AQEEMDAuMCwGCCsGAQUFBzABhiBodHRwOi8vb2NzcC10ZXN0LnNhbXN1bmdpb3Rz\r\n" "LmNvbTAMBggqhkjOPQQDAgUAA0gAMEUCIQCIsi3BcOQMXO/pCiUA+S75bYFWS27E\r\n" "GAq9e2E3+hQ2TAIgWrTieFAZ5xRH3BnSHG+XEF2HPD99y/SYSa6T59YW+jE=\r\n" "-----END CERTIFICATE-----\r\n\0";

const char test_ecc_devcrt[] =
	"-----BEGIN CERTIFICATE-----\r\n"
	"MIICoTCCAkWgAwIBAgIUSEEwMVQxNzAxMTcwMzAwMTk5OTkwDAYIKoZIzj0EAwIF\r\n"
	"ADCBgzE4MDYGA1UEAxMvU2Ftc3VuZyBFbGVjdHJvbmljcyBPQ0YgSEEgRGV2aWNl\r\n"
	"IFN1YkNBIHYxIFRFU1QxHDAaBgNVBAsTE09DRiBIQSBEZXZpY2UgU3ViQ0ExHDAa\r\n"
	"BgNVBAoTE1NhbXN1bmcgRWxlY3Ryb25pY3MxCzAJBgNVBAYTAktSMCAXDTE3MDEx\r\n"
	"NzEwMTE1MFoYDzIwNjkxMjMxMTQ1OTU5WjCBkzFOMEwGA1UEAxNFT0NGIERldmlj\r\n"
	"ZSBURVNUOiBBcHBsaWFuY2UgUFVGICg1NWRlM2FhZC05NGNiLTQ0MmMtYTBkZi1m\r\n"
	"MzBiODFjZDlhYmIpMRYwFAYDVQQLEw1PQ0YgSEEgRGV2aWNlMRwwGgYDVQQKExNT\r\n"
	"YW1zdW5nIEVsZWN0cm9uaWNzMQswCQYDVQQGEwJLUjBZMBMGByqGSM49AgEGCCqG\r\n"
	"SM49AwEHA0IABNVhjeR/LQmN3hWHkEZfd+yKXU/jf1nWOsHB9NvtxfVeV0o3m9Jm\r\n"
	"hSUNVnKopljFRhYO1lWSY+UVJMTTa4P8azWjgYAwfjAOBgNVHQ8BAf8EBAMCA8gw\r\n"
	"LgYDVR0fBCcwJTAjoCGgH4YdaHR0cDovL2NhLnNhbXN1bmdpb3RzLmNvbS9jcmww\r\n"
	"PAYIKwYBBQUHAQEEMDAuMCwGCCsGAQUFBzABhiBodHRwOi8vb2NzcC10ZXN0LnNh\r\n"
	"bXN1bmdpb3RzLmNvbTAMBggqhkjOPQQDAgUAA0gAMEUCIDI7RD298XQ/IHhAaGVd\r\n"
	"CZtJOdlXvCGZmBC3QYcxCR6RAiEA3mavz69h/tn2RM4mI+wgMt2xXukb6j7GQL/z\r\n"
	"cm6LyCk=\r\n"
	"-----END CERTIFICATE-----\r\n\0"
	"-----BEGIN CERTIFICATE-----\r\n"
	"MIICkjCCAjagAwIBAgIUfSmsDder32Y6TTSZ/0P8y8T3kVIwDAYIKoZIzj0EAwIF\r\n"
	"ADBwMS0wKwYDVQQDEyRTYW1zdW5nIEVsZWN0cm9uaWNzIE9DRiBSb290IENBIFRF\r\n" "U1QxFDASBgNVBAsTC09DRiBSb290IENBMRwwGgYDVQQKExNTYW1zdW5nIEVsZWN0\r\n" "cm9uaWNzMQswCQYDVQQGEwJLUjAgFw0xNjEyMDkwMTM3NDVaGA8yMDY5MTIzMTE0\r\n" "NTk1OVowgYMxODA2BgNVBAMTL1NhbXN1bmcgRWxlY3Ryb25pY3MgT0NGIEhBIERl\r\n" "dmljZSBTdWJDQSB2MSBURVNUMRwwGgYDVQQLExNPQ0YgSEEgRGV2aWNlIFN1YkNB\r\n" "MRwwGgYDVQQKExNTYW1zdW5nIEVsZWN0cm9uaWNzMQswCQYDVQQGEwJLUjBZMBMG\r\n" "ByqGSM49AgEGCCqGSM49AwEHA0IABAJmBp1E4Oklec0Eo4QUSsx7Bu8DT4G7iSTs\r\n" "7UhPqbjcTxhoUOyRUNXVsVoWXj8R0YHPRa9/VUL8T98AX8ukN+6jgZUwgZIwDgYD\r\n" "VR0PAQH/BAQDAgHGMC4GA1UdHwQnMCUwI6AhoB+GHWh0dHA6Ly9jYS5zYW1zdW5n\r\n" "aW90cy5jb20vY3JsMBIGA1UdEwEB/wQIMAYBAf8CAQAwPAYIKwYBBQUHAQEEMDAu\r\n" "MCwGCCsGAQUFBzABhiBodHRwOi8vb2NzcC10ZXN0LnNhbXN1bmdpb3RzLmNvbTAM\r\n" "BggqhkjOPQQDAgUAA0gAMEUCIE9pLI8LBrhL/0udGudlPzpb04Z6MTvfzemX5OJN\r\n" "Tf7uAiEA9RAQNZj80rGkxoIgFGekywvuBybpXJrrZu/BP2TWkFU=\r\n" "-----END CERTIFICATE-----\r\n\0";

const char test_ecc_devkey[] = "-----BEGIN EC PRIVATE KEY-----\r\n" "MHcCAQEEIP/uqXU1dQ58bC1rURl3QpxchfsLrPCNfMNDzI5PxQ45oAoGCCqGSM49\r\n" "AwEHoUQDQgAE1WGN5H8tCY3eFYeQRl937IpdT+N/WdY6wcH02+3F9V5XSjeb0maF\r\n" "JQ1WcqimWMVGFg7WVZJj5RUkxNNrg/xrNQ==\r\n" "-----END EC PRIVATE KEY-----\r\n";

void iotivity_sss_setup_postkey(void)
{
	int ret;
	unsigned char *cacrt;
	unsigned int cacrt_len;
	unsigned char *devcrt;
	unsigned int devcrt_len;
	unsigned char *devkey;
	unsigned int devkey_len;
	unsigned int key_type;
	unsigned int option;

	/* Please setup special key and cert for connecting with cloud */
	cacrt = test_ecc_crt;
	cacrt_len = sizeof(test_ecc_crt);
	devcrt = test_ecc_devcrt;
	devcrt_len = sizeof(test_ecc_devcrt);
	devkey = test_ecc_devkey;
	devkey_len = sizeof(test_ecc_devkey);
	key_type = SECURE_STORAGE_TYPE_KEY_ECC;
	option = MBEDTLS_ECP_DP_SECP256R1;

	/* Setup certificates index for testing */
	g_iotivity_cert_index = 1;
	g_iotivity_cacert_index = 2;

	/* Mandatory : CA cert */
	if ((ret = see_set_certificate(cacrt, cacrt_len, g_iotivity_cacert_index, CERT_PEM)) != 0) {
		printf("Error: set ca cert fail %d\n", ret);
		return;
	}

	/* Optional : Others */
	if (devcrt) {
		if ((ret = see_set_certificate(devcrt, devcrt_len, g_iotivity_cert_index, CERT_PEM)) != 0) {
			printf("Error: set cloud cert fail %d\n", ret);
		}
	}

	unsigned char *header;
	unsigned char *footer;

	if (devkey) {
		/* 2. Setup key for testing */
		g_iotivity_key_index = see_get_keyindex(key_type);
		if (key_type == SECURE_STORAGE_TYPE_KEY_ECC) {
			g_iotivity_key_curve = option;
			header = "-----BEGIN EC PRIVATE KEY-----";
			footer = "-----END EC PRIVATE KEY-----";
		} else {
			return -1;
		}

		/* Optional *//* PEM */
		if (devkey[0] == 0x2d && devkey[1] == 0x2d) {
			unsigned int use_len = 0;
			mbedtls_pem_context pem;
			mbedtls_pem_init(&pem);

			ret = mbedtls_pem_read_buffer(&pem, header, footer, devkey, NULL, 0, &use_len);
			if (ret != 0) {
				printf("Error: parse key fail %x\n", -ret);
			}

			if ((ret = see_setup_key(pem.buf, pem.buflen, key_type, g_iotivity_key_index)) != 0) {
				printf("Error: set cloud key fail %d\n", ret);
			}
			mbedtls_pem_free(&pem);
		}
		/* Optional *//* DER */
		else if (devkey[0] == 0x30 && devkey[1] == 0x82) {
			if ((ret = see_setup_key(devkey, devkey_len, key_type, g_iotivity_key_index)) != 0) {
				printf("Error: set cloud key fail %d\n", ret);
			}
		} else {
			printf("Invalid key encoding\n");
		}
	}

}
#endif

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int cloudServer_main(int argc, char *argv[])
#endif
{
#ifdef CONFIG_TLS_WITH_SSS
	see_init();
#endif

	if (argc == 6) {
		int cmd_id = atoi(argv[1]);
		g_mode = atoi(argv[5]);
		if (g_mode == 1) {
			snprintf(g_host, MAX_URI_LENGTH * 2, "%s%s", coaps_tcp_header, argv[2]);
		} else {
			snprintf(g_host, MAX_URI_LENGTH * 2, "%s%s", coap_tcp_header, argv[2]);
		}

		/* Intialize Persistent Storage */
		if (OC_STACK_OK != initCloudServerPersistentStorage()) {
			printf("[%s] initCloudServerPersistentStorage failed \n", TAG);
			return -2;
		}

		/* Intialize Required Files */
#ifdef CONFIG_ENABLE_IOTIVITY_SECURED
		if (g_mode == SECURE_MODE) {
#if defined(CONFIG_TLS_WITH_SSS) && defined(CONFIG_HW_ECDSA)
			/* Only for Post key test */
			iotivity_sss_setup_postkey();
#else
			if (init_rootcacrt_file() != 0) {
				printf("[%s] init_rootcacrt_file failed \n", TAG);
				return -1;
			}
#endif
			if (init_clouddat_file() != 0) {
				printf("[%s] init_clouddat_file failed \n", TAG);
				return -1;
			}
		}
#endif
		/* Intialize OCStack */
		if (OCInit(NULL, 0, OC_CLIENT_SERVER) != OC_STACK_OK) {
			printf("OCStack init error\n");
			return 2;
		}
#ifdef CONFIG_ENABLE_IOTIVITY_SECURED
		/* Set Cipher Suite and Save Trust Cert */
		if (g_mode == SECURE_MODE) {
#if defined(CONFIG_TLS_WITH_SSS) && defined(CONFIG_HW_ECDSA)
#if defined(USE_BOARD_KEY)
			g_iotivity_cert_index = FACTORYKEY_IOTIVITY_ECC_CERT;
			g_iotivity_cacert_index = FACTORYKEY_IOTIVITY_SUB_CA_CERT;
			g_iotivity_key_index = FACTORYKEY_IOTIVITY_ECC;
			g_iotivity_key_curve = MBEDTLS_ECP_DP_SECP256R1;
#endif
			if (SetHwPkixCallbacks(OCGetHwKey, OCFreeHwKey, OCGetOwnCertFromHw, OCSetupPkContextFromHw)) {
				printf("SetHwPkixCallbacks failed\n");
				return -1;
			}
#endif
			uint16_t cipher = MBEDTLS_TLS_RSA_WITH_AES_128_GCM_SHA256;
			if (CA_STATUS_OK != CASelectCipherSuite(cipher, CA_ADAPTER_TCP)) {
				printf("[%s]  CASelectCipherSuite returned an error\n", TAG);
			}
			if (OC_STACK_OK != saveTrustCert()) {
				printf("[%s] saveTrustCert failed \n", TAG);
				return -1;
			}
		}
#endif
		if (cmd_id == 1) {
			snprintf(g_authProviderName, MAX_URI_LENGTH * 2, "%s", argv[3]);
			snprintf(g_authProviderCode, MAX_URI_LENGTH * 2, "%s", argv[4]);
			task_create("iotivity-cloud-signup", IOTIVITY_TEST_PRI, IOTIVITY_TEST_STACKSIZE, iotivity_cloud_signup, (FAR char *const *)NULL);
			return 0;

		} else if (cmd_id == 2) {
			snprintf(g_uID, MAX_URI_LENGTH * 2, "%s", argv[3]);
			snprintf(g_accessToken, MAX_URI_LENGTH * 2, "%s", argv[4]);
			task_create("iotivity-cloud-signin", IOTIVITY_TEST_PRI, IOTIVITY_TEST_STACKSIZE, iotivity_cloud_signin, (FAR char *const *)NULL);
			return 0;
		} else {
			printf("[%s] Invalid Command Id\n", TAG);
			PrintUsage();
			return 0;
		}
		return 0;
	}
	PrintUsage();

	return 0;
}
